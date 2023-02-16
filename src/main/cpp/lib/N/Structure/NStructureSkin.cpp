#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"
#include "../NStructure.h"
#include "../Maths/NMatrix3x3.h"
//#include "../Maths/NMatrix.h"
#include "../Miscellaneous/NVertexColorBuild.h"
#include "../Miscellaneous/NVertexTexCoordBuild.h"

#include "../NErrorHandling.h"


void NSkinDestructor_ArrayCallBack(void* pel)
{
	NSKIN	*pskin;

	pskin=(NSKIN*)pel;

	switch(pskin->common.SkinType)
	{
		case NSKIN_TYPE_ROPE:
/*
			if(pskin->Rope.pSideAVertebra)
			{
				NDeleteArray(pskin->Rope.pSideAVertebra,NULL);
				pskin->Rope.pSideAVertebra = NULL;
			}

			if(pskin->Rope.pSideBVertebra)
			{
				NDeleteArray(pskin->Rope.pSideBVertebra,NULL);
				pskin->Rope.pSideBVertebra = NULL;
			}
*/
			NClearArray(&pskin->Rope.VertebrasArray,NULL);
			return;
/*
		case NSKIN_TYPE_BILLBOARD:
		case NSKIN_TYPE_GRID:
			return;
*/
		default:
			break;
	}
}


static void _skinvertex_ptoi(NSKIN*pskin)
{
	pskin->common.pFirstVertex = (NSKINVERTEX*)NGetArrayIndex(&pskin->common.pGeometry->Mesh.VertexArray,(NBYTE*)pskin->common.pFirstVertex);
}

static void _skinvertex_itop(NSKIN*pskin)
{
	pskin->common.pFirstVertex = (NSKINVERTEX*)NGetArrayPtr(&pskin->common.pGeometry->Mesh.VertexArray,(Nu32)pskin->common.pFirstVertex);
}

static inline Nbool _skinarrayvertex_ptoi(NSTRUCTURE *pstruct, const NGEOMETRY* pgeom)
{
	Nu32	i;
	NSKIN	*pskin;
	Nbool	repoint;

	repoint = NFALSE;
	pskin =(NSKIN*)NGetFirstArrayPtr(&pstruct->SkinArray);
	for(i=0;i<NGetArraySize(&pstruct->SkinArray);i++,pskin++)
	{
		// We have only to deal with the skin which are using this geometry !
		// ... the one which will have vertex reallocation ! 
		if(pskin->common.pGeometry == pgeom)
		{
			repoint = NTRUE;
			_skinvertex_ptoi(pskin);
		}
	}

	return repoint;
}
static inline void _skinarrayvertex_itop(NSTRUCTURE *pstruct, const NGEOMETRY* pgeom)
{
	Nu32	i;
	NSKIN	*pskin;
	
	pskin =(NSKIN*)NGetFirstArrayPtr(&pstruct->SkinArray);
	for(i=0;i<NGetArraySize(&pstruct->SkinArray);i++,pskin++)
	{
		// We have only to deal with the skin which are using this geometry !
		// ... the one which will have vertex reallocation ! 
		if(pskin->common.pGeometry == pgeom)
		{
			_skinvertex_itop(pskin);
		}
	}
}
// ----------------------------------------------------------------------------------------------------------------------------------
// CreateSkin_Billboard
// ----------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set up and organize a geometry as a skin of a Physic structure
//	Depending parameters of "pbuilder" , the right number of vertex and triangles are build and set up.
//	
// ----------------------------------------------------------------------------------------------------------------------------------
// In  :
//			pstruct					a VALID POINTER on a STRUCTURE structure 
//									with is lists of NJOINT, SPRINGS and CONSTRAINTS.
//
//			pgeom					A valid pointer of a GEOMETRY (which has to be one of those included into the Structure Renderable )
//									So pgeom has to point on a pre-created geometry !
//			pbuilder				A valid pointer of a SKIN_BUILDER structure which contains all the user parameters
//									to build the Skin.
//									
//										- Skin_Scheme			Type of the mesh created
//																		
//										- SkinRef_Type			Type of the structure elements used to create and update the skin.
//																It can be NJOINT, SPRINGS or CONSTRAINTS.
//
//										- SkinRef_RangeStart	element (index) which the billboard is connected with (like a HotSpot). 
//																for a SkinRef_type different from 'NJOINT' ( a SPRING or whatever) the Engine
//																is going to use the "A joint" pointed by the segment.
//
//										- SkinRef_RangeSize		In that specific case, Range_size represents an "index distance" from 'SkinRef_RangeStart'.
//																The element pointed is 	used to orient the Billboard.
//																To perform that we need 2 points (2 JOINTS) which we are going to calculate the vector AB.
//																'SkinRef_RangeStart + SkinRef_RangeSize' indicates the point 'A'.
//																for a SkinRef_type different from 'NJOINT' ( a SPRING or whatever) the Engine
//																is going to use the "A joint" pointed by the segment.
//
//										- SkinRef_SubRangeSize	In that specific case, Range_size represents an element (index) used to orient the Billboard.
//																To perform that we need 2 points (2 JOINTS) which we are going to calculate the vector AB.
//																'SkinRef_RangeStart + SkinRef_SubRangeSize' indicates the point 'B'.
//																for a SkinRef_type different from 'NJOINT' ( a SPRING or whatever) the Engine
//																is going to use the "B joint" pointed by the segment.

// Out :
//			A pointer  on the new skin
// ----------------------------------------------------------------------------------------------------------------------------------
NSKIN* NCreateSkin_Billboard(NSTRUCTURE *pstruct, NGEOMETRY *pgeom, const NSKIN_DESC *pbuilder )
{

	Nu32			nbvertex,nbtri;
	NARRAY			*pref_array;

	NSKIN			skin;
	NTRIANGLE		*ptri;
	NSKINVERTEX		*pvertex;
	
	Nu32			firstvertex_index,firstprimitive_index;
	Nbool			repoint;
	NMATRIX3x3		txc_matrix;
	NVEC3		color_position_3f;

	// ===================================================================================
	// Initial Checks
	// ===================================================================================
	NErrorIf( !pgeom, NERROR_STRUCTURE_SKIN_VALID_GEOMETRY_REQUESTED );
	NErrorIf( !pstruct->pRenderable, NERROR_STRUCTURE_SKIN_VALID_RENDERABLE_REQUESTED );
	NErrorIf( !NIsGeometryIncluded( pstruct->pRenderable, pgeom),NERROR_STRUCTURE_SKIN_UNKNOWN_GEOMETRY );
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)!=NSKINVERTEX_FORMAT, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)!=NPRIMITIVE_FORMAT_TRIANGLE, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT );
	NErrorIf(pbuilder->BillBoard.Ref_StructureElementType != NSTRUCTURE_ELEMENT_JOINT, NERROR_STRUCTURE_SKIN_UNAUTHORIZED_STRUCTURE_ELEMENT_TYPE);

	// ==================================================================================
	// Get the right array of references elements for skin building process
	// ==================================================================================
	pref_array = &pstruct->JointArray;

	// ==================================================================================
	// SKIN MESH CREATION
	// ==================================================================================
	memset(&skin,0,sizeof(NSKIN));

	switch(pbuilder->BillBoard.Structure_Scheme)
	{
		// =================================================================
		// One Quad aligned along the the ref (spring, constraint, if any)
		// =================================================================
		case NSKIN_SCHEME_BILLBOARD_1QUAD:
			// ================
			// Structure Scheme
			// ================
			// n = size of one subrange ( = size of one joints column )
			// Vertex Index Distribution (follow the CCW winding Order)
			//		2 ----- 1
			//		| .  T0 |
			//		|   +   |
			//		| T1  . |
			//		3 ----- 0
			// =============
			// Vertex number
			// =============
			// To create the skin we need to have 4 vertex
			nbvertex = 4;
			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}
			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);
			
			// ===============
			// Triangle number
			// ===============
			// To create the skin we need to have 2 triangles
			nbtri = 2;
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//		2 ----- 1					2 ----- 3 
			//		| .  T0 |					| .  T0 |	
			//		|   +   |					|   +   |	
			//		| T1  . |					| T1  . |	
			//		3 ----- 0					1 ----- 0 
			// Notice that vertex are always to be updated as CCW winding Order at skin update
			// So to force CW order we just made some index permutation at triangle creation
			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->BillBoard.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				//T0
				ptri[firstprimitive_index].i0	= firstvertex_index;
				ptri[firstprimitive_index].i1	= firstvertex_index+2;
				ptri[firstprimitive_index].i2	= firstvertex_index+1;
				//T1
				ptri[firstprimitive_index+1].i0	= firstvertex_index;
				ptri[firstprimitive_index+1].i1	= firstvertex_index+3;
				ptri[firstprimitive_index+1].i2	= firstvertex_index+2;
			}
			else // CCW, the default one
			{
				//T0
				ptri[firstprimitive_index].i0	= firstvertex_index;
				ptri[firstprimitive_index].i1	= firstvertex_index+1;
				ptri[firstprimitive_index].i2	= firstvertex_index+2;
				//T1	
				ptri[firstprimitive_index+1].i0	= firstvertex_index;
				ptri[firstprimitive_index+1].i1	= firstvertex_index+2;
				ptri[firstprimitive_index+1].i2	= firstvertex_index+3;
			}

			// ===============================================
			// Vertex attributes Set up (TexCoords and Colors )
			// ===============================================
			pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
			//
			//  TexCoords_2f			
			//							 
			//	0,0 --- 0,1
			//	 |       |
			//	 |       |
			//	 |       |
			//	1,0 --- 1,1
			//

			// FLAGS_NSKIN_DESC_NTXCBUILD_... may be all to OFF even if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to ON.
			// But, if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to OFF, all FLAGS_NSKIN_DESC_NTXCBUILD_... have to be set to OFF too.
			// Just check that before continuing...
			NErrorIf( ISFLAG_OFF(pbuilder->BillBoard.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) && pbuilder->BillBoard.TexCoordBuild.Flags!=0,NERROR_INCONSISTENT_PARAMETERS );
			
			if( ISFLAG_ON(pbuilder->BillBoard.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->BillBoard.TexCoordBuild);
				// CCW Left-Top
				pvertex[firstvertex_index+2].TexCoord0_2f.x	= txc_matrix.Origin.x;		
				pvertex[firstvertex_index+2].TexCoord0_2f.y	= txc_matrix.Origin.y;		
				// CCW Right-Top
				pvertex[firstvertex_index+1].TexCoord0_2f.x	= txc_matrix.Origin.x + txc_matrix.XAxis.x;
				pvertex[firstvertex_index+1].TexCoord0_2f.y	= txc_matrix.Origin.y + txc_matrix.XAxis.y;
				// CCW Right-Bottom
				pvertex[firstvertex_index].TexCoord0_2f.x	= txc_matrix.Origin.x + txc_matrix.XAxis.x + txc_matrix.YAxis.x;	
				pvertex[firstvertex_index].TexCoord0_2f.y	= txc_matrix.Origin.y + txc_matrix.XAxis.y + txc_matrix.YAxis.y;
				// CCW Left-Bottom
				pvertex[firstvertex_index+3].TexCoord0_2f.x	= txc_matrix.Origin.x + txc_matrix.YAxis.x;	
				pvertex[firstvertex_index+3].TexCoord0_2f.y	= txc_matrix.Origin.y + txc_matrix.YAxis.y;
			}
			
			if( ISFLAG_ON(pbuilder->BillBoard.Flags,FLAG_NSKIN_DESC_BUILD_COLORS) )
			{
				// CCW Left-Top
				NVec3Set(&color_position_3f,0,0,0);
				NGetBuildBaseColorVertexColor(	&pvertex[firstvertex_index+2].Color0_4f,
												2,4,
												(Nf32*)&color_position_3f,
												&pbuilder->BillBoard.ColorBuild );
				
				// CCW Right-Top
				NVec3Set(&color_position_3f,1,0,0);
				NGetBuildBaseColorVertexColor(	&pvertex[firstvertex_index+1].Color0_4f,
												1,4,
												(Nf32*)&color_position_3f,
												&pbuilder->BillBoard.ColorBuild );

				// CCW Right-Bottom
				NVec3Set(&color_position_3f,1,1,0);
				NGetBuildBaseColorVertexColor(	&pvertex[firstvertex_index].Color0_4f,
												0,4,
												(Nf32*)&color_position_3f,
												&pbuilder->BillBoard.ColorBuild );
				// CCW Left-Bottom
				NVec3Set(&color_position_3f,0,1,0);
				NGetBuildBaseColorVertexColor(	&pvertex[firstvertex_index+3].Color0_4f,
												3,4,
												(Nf32*)&color_position_3f,
												&pbuilder->BillBoard.ColorBuild );

			}

			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// ===================
			// Skin Creation 
			// ===================
			skin.BillBoard.SkinType				= NSKIN_TYPE_BILLBOARD;
			skin.BillBoard.StructureElementType	= pbuilder->BillBoard.Ref_StructureElementType;
			skin.BillBoard.pGeometry			= pgeom;
			skin.BillBoard.pFirstVertex			= (NSKINVERTEX*)NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex_index);
			skin.BillBoard.fScale				= pbuilder->BillBoard.fScale;
			skin.BillBoard.RatioWidth_Height	= pbuilder->BillBoard.RatioWidth_Height;
			skin.BillBoard.pCenter				= (NJOINT*)NGetArrayPtr(pref_array,pbuilder->BillBoard.Ref_Center);
			skin.BillBoard.pOrient_A			= (NJOINT*)NGetArrayPtr(pref_array,pbuilder->BillBoard.Ref_Orient_A);
			skin.BillBoard.pOrient_B			= (NJOINT*)NGetArrayPtr(pref_array,pbuilder->BillBoard.Ref_Orient_B);
			skin.BillBoard.VShift				= pbuilder->BillBoard.VShift;
			skin.BillBoard.pUpdateSkinGeometry	= NUpdateSkin_Billboard_Quad;
			
			return (NSKIN*)NArrayPushBack(&pstruct->SkinArray,(NBYTE*)&skin);
            
        default:
            NErrorIf(1,NERROR_INVALID_CASE);
            break;
	}

	return NULL; // just to avoid a compiler Warning. The program will never pass here
}




// ----------------------------------------------------------------------------------------------------------------------------------
// CreateSkin_Grid
// ----------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set up and organize a geometry as a skin of a Physic structure
//	Depending parameters of "pbuilder" , the right number of vertex and triangles are build and set up.
//	
// ----------------------------------------------------------------------------------------------------------------------------------
// In  :
//			pstruct					a VALID POINTER on a STRUCTURE structure 
//									with is lists of NJOINT, SPRINGS and CONSTRAINTS.
//
//			pgeom					A valid pointer of a GEOMETRY (usually a part of an NOBBECT, but not necessary)
//
//			pbuilder				A valid pointer of a SKIN_BUILDER structure which contains all the user parameters
//									to build the Skin.
//									
//										- Skin_Scheme			Type of the mesh created
//																		
//										- SkinRef_Type			Type of the structure elements used to create and update the skin.
//																It can be NJOINT, SPRINGS or CONSTRAINTS.
//
//										- SkinRef_RangeStart	First element (index) of the range used. 
//
//										- SkinRef_RangeSize		Size of the elements range used (ALL).
//
//										- SkinRef_SubRangeSize	Size of ONE grid column
// Out :
//			A pointer  on the new skin
// ----------------------------------------------------------------------------------------------------------------------------------
NSKIN* NCreateSkin_Grid(NSTRUCTURE *pstruct, NGEOMETRY *pgeom, const NSKIN_DESC *pbuilder )
{
	Nu32		nbvertex,nbtri/*,nbcolumn,columnsize*/;

	Nu32		indextri,indexvert,indexcenter;
	Nu32		i,j;
	Nu32		firstprimitive_index,firstvertex_index,firstcenter_index;
	NARRAY		*pref_array;

	Nbool		repoint;
	NSKIN		skin;
	NTRIANGLE	*ptri;
	NSKINVERTEX	*pvertex;
	NMATRIX3x3	txc_matrix;
	NVEC3	color_position_3f;
	// ===================================================================================
	// Initial Checks
	// ===================================================================================
	NErrorIf( !pgeom, NERROR_STRUCTURE_SKIN_VALID_GEOMETRY_REQUESTED );
	NErrorIf( !pstruct->pRenderable, NERROR_STRUCTURE_SKIN_VALID_RENDERABLE_REQUESTED );
	NErrorIf( !NIsGeometryIncluded( pstruct->pRenderable, pgeom),NERROR_STRUCTURE_SKIN_UNKNOWN_GEOMETRY );
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)!=NSKINVERTEX_FORMAT, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)!=NPRIMITIVE_FORMAT_TRIANGLE, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT );
	NErrorIf(pbuilder->Grid.Ref_StructureElementType != NSTRUCTURE_ELEMENT_JOINT, NERROR_STRUCTURE_SKIN_UNAUTHORIZED_STRUCTURE_ELEMENT_TYPE);

	// ==================================================================================
	// Get the right array of references elements for skin building process
	// ==================================================================================
	pref_array = &pstruct->JointArray;
	
	NErrorIf((pbuilder->Grid.Ref_First + pbuilder->Grid.Ref_RangeSize) > NGetArraySize(pref_array), NERROR_STRUCTURE_SKIN_NOT_ENOUGH_REF_ELEMENT_TO_BUILD_REQUESTED_SKIN );// Is there enough JOINTS in the Geometry Array ?
	NErrorIf(!pbuilder->Grid.I_CellsNb||!pbuilder->Grid.J_CellsNb,NERROR_STRUCTURE_SKIN_NOT_ENOUGH_REF_ELEMENT_TO_BUILD_REQUESTED_SKIN );	// Is there at least 2 SubRangeSizes (Column) into RangeSize ?

	// ==================================================================================
	// SKIN MESH CREATION
	// ==================================================================================
	memset(&skin,0,sizeof(NSKIN));

	switch(pbuilder->Grid.Structure_Scheme)
	{
		// ============================================
		// A Quads Grids along a Grid of JOINTS
		// ============================================
		case NSKIN_SCHEME_GRID_QUADS:
			// ================
			// Structure Scheme
			// ================
			// Vertex Index distribution
			// n = size of one subrange ( = size of one joints column )
			// 
			//		0 ------  n -----  2n ------- 3n ------ 4n
			//		| .       |			|		  |         |
			//		|    .    |			|         |         |
			//		|       . |			|         |         |
			//		1 ------ n+1 ----- 2n+1------3n+1----- 4n+1
			//		|         |			|         |         |
			//		|         |			|         |         |
			//		|         |			|         |         |
			//		2 ------ n+2 ------2n+2------3n+2------4n+2
			//		|         |			|         |         | 
			//		|         |			|         |         |
			//		|         |			|         |         |
			//		3 ------ n+3 ------2n+3------3n+3------4n+3
			//		|         |			|         |         |
			//		|         |			|         |         |
			//		|         |			|         |         |
			//	   n-1 ----- 2n-1------3n-1------4n-1 ------5n-1
			// =============
			// Vertex number
			// =============
			// To create the skin we need to pre-create enough vertex/uvs and color elements.
			nbvertex = pbuilder->Grid.Ref_RangeSize;
			NErrorIf(nbvertex != (pbuilder->Grid.I_CellsNb+1)*(pbuilder->Grid.J_CellsNb+1), NERROR_INCONSISTENT_PARAMETERS );

			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}

			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

			// ===============
			// Triangle number
			// ===============
			nbtri = pbuilder->Grid.I_CellsNb*pbuilder->Grid.J_CellsNb*2;	// triangles number calculation based on Main and secondary lines number.
			
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//		2 ----- 1					2 ----- 3 
			//		| .  T0 |					| .  T0 |
			//		|   +   |					|   +   |
			//		| T1  . |					| T1  . |	
			//		3 ----- 0					1 ----- 0 
			//	T0(0,1,2) = (n+1,n,0)			T0(0,2,3) = (n+1,0,n)
			//	T1(0,2,3) = (n+1,0,1)			T1(0,1,2) = (n+1,1,0)
			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				for(j=0;j<pbuilder->Grid.J_CellsNb;j++)
				{
					for(i=0;i<pbuilder->Grid.I_CellsNb;i++)
					{
						indexvert	=	firstvertex_index + j*(pbuilder->Grid.I_CellsNb+1) + i;
						indextri	=	firstprimitive_index + ( j*pbuilder->Grid.I_CellsNb + i )*2;
						
						// Reminder: ...+(pbuilder->Grid.I_CellsNb+1) to get the next "along J" vertex
						ptri[indextri].i0	= indexvert+pbuilder->Grid.I_CellsNb+2;
						ptri[indextri].i1	= indexvert;
						ptri[indextri].i2	= indexvert+pbuilder->Grid.I_CellsNb+1;

						ptri[indextri+1].i0	= indexvert+pbuilder->Grid.I_CellsNb+2;
						ptri[indextri+1].i1 = indexvert+1;
						ptri[indextri+1].i2 = indexvert;
					}
				}
			}
			else // CCW face building: the default one !
			{
				for(j=0;j<pbuilder->Grid.J_CellsNb;j++)
				{
					for(i=0;i<pbuilder->Grid.I_CellsNb;i++)
					{
						indexvert	=	firstvertex_index + j*(pbuilder->Grid.I_CellsNb+1) + i;
						indextri	=	firstprimitive_index + ( j*pbuilder->Grid.I_CellsNb + i )*2;

						// Reminder: ...+(pbuilder->Grid.I_CellsNb+1) to get the next "along J" vertex
						ptri[indextri].i0	= indexvert+pbuilder->Grid.I_CellsNb+2;
						ptri[indextri].i1	= indexvert+pbuilder->Grid.I_CellsNb+1;
						ptri[indextri].i2	= indexvert;

						ptri[indextri+1].i0	= indexvert+pbuilder->Grid.I_CellsNb+2;
						ptri[indextri+1].i1 = indexvert;
						ptri[indextri+1].i2 = indexvert+1;
					}
				}
			}

			// ===============================================
			// Vertex attributes Setting up (UVs and Colors )
			// ===============================================
			// FLAGS_NSKIN_DESC_NTXCBUILD_... may be all to OFF even if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to ON.
			// But, if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to OFF, all FLAGS_NSKIN_DESC_NTXCBUILD_... have to be set to OFF too.
			// Just check that before continuing...
			NErrorIf( ISFLAG_OFF(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) && pbuilder->Grid.TexCoordBuild.Flags!=0,NERROR_INCONSISTENT_PARAMETERS );

			if( ISFLAG_ON(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Grid.TexCoordBuild);
				pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
				
				// TexCoords are going to be distributed on all the effective texture surface.
				// This whole surface is define thanks to the TexCoordBuilding 2D Matrix, so let's doing some update
				NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)pbuilder->Grid.I_CellsNb);
				NVec3ScaleBy(&txc_matrix.YAxis, 1.0f/(Nf32)pbuilder->Grid.J_CellsNb);

				for(j=0;j<pbuilder->Grid.J_CellsNb+1;j++)
				{
					for(i=0;i<pbuilder->Grid.I_CellsNb+1;i++)
					{
						indexvert	=	firstvertex_index + j*(pbuilder->Grid.I_CellsNb+1) + i;
						pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+(Nf32)j*txc_matrix.YAxis.x;		
						pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+(Nf32)j*txc_matrix.YAxis.y;		
					}
				}
			}
			
			// COLORS
			if( ISFLAG_ON(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_BUILD_COLORS) )
			{
				color_position_3f.z = 0.0f;
				for(j=0;j<pbuilder->Grid.J_CellsNb+1;j++)
				{
					color_position_3f.y = (Nf32)j/(Nf32)pbuilder->Grid.J_CellsNb;
					for(i=0;i<pbuilder->Grid.I_CellsNb+1;i++)
					{
						color_position_3f.x = (Nf32)i/(Nf32)pbuilder->Grid.I_CellsNb;
						indexvert	=	firstvertex_index + j*(pbuilder->Grid.I_CellsNb+1) + i;
						
						NGetBuildBaseColorVertexColor(	&pvertex[indexvert].Color0_4f,
														indexvert,nbvertex,
														(Nf32*)&color_position_3f,
														&pbuilder->Grid.ColorBuild );

					}
				}
			}
			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// =======================
			// Skin structure Creation 
			// =======================
			memset(&skin,0,sizeof(NSKIN));
			skin.Grid.SkinType					= NSKIN_TYPE_GRID;
			skin.Grid.StructureElementType		= pbuilder->Grid.Ref_StructureElementType; // It can be only NJOINT
			skin.Grid.pGeometry					= pgeom;
			skin.Grid.pFirstVertex				= (NSKINVERTEX*)NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex_index);
			skin.Grid.pFirst					= (void*)NGetArrayPtr(pref_array,pbuilder->Grid.Ref_First);
			skin.Grid.RangeSize					= pbuilder->Grid.Ref_RangeSize;
 			skin.Grid.I_CellsNb					= pbuilder->Grid.I_CellsNb;
 			skin.Grid.J_CellsNb					= pbuilder->Grid.J_CellsNb;

			skin.Grid.pUpdateSkinGeometry		= NUpdateSkin_Grid_Quads;

			return (NSKIN*)NArrayPushBack(&pstruct->SkinArray,(NBYTE*)&skin);
        

			case NSKIN_SCHEME_GRID_FANQUADS:
				// ================
				// Structure Scheme
				// ================
				// n = size of one subrange ( = size of one joints column )
				//
				//		0 ------  n -----  2n 
				//		|         |			|
				//		|    .3n  |			| 
				//		|         |			| 
				//		1 ------ n+1 ----- 2n+1
				//		|         |			| 
				//		|    .3n+1|			| 
				//		|         |			| 
				//		2 ------ n+2 ------2n+2
				//		|         |			| 
				//		|    .3n+2|			| 
				//		|         |			| 
				//	   n-1 ----- 2n-1------3n-1
				// =============
				// Vertex number
				// =============
				// To create the skin we need to pre-create enough vertex/uvs and color elements.
				nbvertex = pbuilder->Grid.Ref_RangeSize;
				firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
				firstcenter_index = firstvertex_index + (pbuilder->Grid.I_CellsNb+1)*(pbuilder->Grid.J_CellsNb+1);

				repoint = NFALSE;
				if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
				{
					// A memory reallocation is going to happen !
					// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
					repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
				}
				NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

				// ===============
				// Triangle number
				// ===============
				nbtri = (pbuilder->Grid.I_CellsNb)*(pbuilder->Grid.J_CellsNb)*4;

				firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
				NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

				// =========================
				// Triangle index Setting up
				// =========================
				//	CCW Winding Order			CW Winding Order
				//		3 ----- 2					3 ----- 4
				//		| .	b . |					| .	b . |
				//		| c 0 a |					| c 0 a |
				//		| . d . |					| . d . |
				//		4 ----- 1					2 ----- 1
				//	Ta(0,1,2) = (c,n+1,n)			T0(0,4,1) = (c,n,n+1)
				//	Tb(0,2,3) = (c,n,0)				T1(0,3,4) = (c,0,n)
				//	Tb(0,3,4) = (c,0,1)				T1(0,2,3) = (c,1,0)
				//	Tb(0,4,1) = (c,1,n+1)			T1(0,1,2) = (c,n+1,1)

				ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
				if( ISFLAG_ON(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
				{
					for(j=0;j<pbuilder->Grid.J_CellsNb;j++)
					{
						for(i=0;i<pbuilder->Grid.I_CellsNb;i++)
						{
							indexcenter =	firstcenter_index	+  j*pbuilder->Grid.I_CellsNb + i;
							indexvert	=	firstvertex_index	+  j*(pbuilder->Grid.I_CellsNb+1) + i;
							indextri	=	firstprimitive_index+ ( j*pbuilder->Grid.I_CellsNb + i )*4;
							//triangle a	
							ptri[indextri].i0	= indexcenter;
							ptri[indextri].i1	= indexvert+pbuilder->Grid.I_CellsNb+1;
							ptri[indextri].i2	= indexvert+pbuilder->Grid.I_CellsNb+2;
							//triangle b	
							ptri[indextri+1].i0	= indexcenter;
							ptri[indextri+1].i1	= indexvert;
							ptri[indextri+1].i2	= indexvert+pbuilder->Grid.I_CellsNb+1;
							//triangle c	
							ptri[indextri+2].i0	= indexcenter;
							ptri[indextri+2].i1	= indexvert+1;
							ptri[indextri+2].i2	= indexvert;
							//triangle d	
							ptri[indextri+3].i0	= indexcenter;
							ptri[indextri+3].i1	= indexvert+pbuilder->Grid.I_CellsNb+2;
							ptri[indextri+3].i2	= indexvert+1;
						}
					}
				}
				else // CCW the default one
				{
					for(j=0;j<pbuilder->Grid.J_CellsNb;j++)
					{
						for(i=0;i<pbuilder->Grid.I_CellsNb;i++)
						{
							indexcenter =	firstcenter_index	+  j*pbuilder->Grid.I_CellsNb + i;
							indexvert	=	firstvertex_index	+ j*(pbuilder->Grid.I_CellsNb+1) + i;
							indextri	=	firstprimitive_index+ ( j*pbuilder->Grid.I_CellsNb + i )*4;

							ptri[indextri].i0	= indexcenter;
							ptri[indextri].i1	= indexvert+pbuilder->Grid.I_CellsNb+2;
							ptri[indextri].i2	= indexvert+pbuilder->Grid.I_CellsNb+1;

							ptri[indextri+1].i0	= indexcenter;
							ptri[indextri+1].i1	= indexvert+pbuilder->Grid.I_CellsNb+1;
							ptri[indextri+1].i2	= indexvert;

							ptri[indextri+2].i0	= indexcenter;
							ptri[indextri+2].i1	= indexvert;
							ptri[indextri+2].i2	= indexvert+1;

							ptri[indextri+3].i0	= indexcenter;
							ptri[indextri+3].i1	= indexvert+1;
							ptri[indextri+3].i2	= indexvert+pbuilder->Grid.I_CellsNb+2;
						}
					}
				}


				// ===============================================
				// Vertex attributes Setting up (UVs and Colors )
				// ===============================================
				// FLAGS_NSKIN_DESC_NTXCBUILD_... may be all to OFF even if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to ON.
				// But, if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to OFF, all FLAGS_NSKIN_DESC_NTXCBUILD_... have to be set to OFF too.
				// Just check that before continuing...
				NErrorIf( ISFLAG_OFF(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) && pbuilder->Grid.TexCoordBuild.Flags!=0,NERROR_INCONSISTENT_PARAMETERS );

				if( ISFLAG_ON(pbuilder->Grid.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
				{
					NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Grid.TexCoordBuild);
					pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);

					// TexCoords are going to be distributed on all the effective texture surface.
					// This whole surface is define thanks to the TexCoordBuilding 2D Matrix, so let's doing some update
					NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)pbuilder->Grid.I_CellsNb);
					NVec3ScaleBy(&txc_matrix.YAxis, 1.0f/(Nf32)pbuilder->Grid.J_CellsNb);
					
					// The 4 Quad perimeter Vertex
					for(j=0;j<pbuilder->Grid.J_CellsNb+1;j++)
					{
						for(i=0;i<pbuilder->Grid.I_CellsNb+1;i++)
						{
							indexvert	=	firstvertex_index	+ j*(pbuilder->Grid.I_CellsNb+1) + i;
							
							pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+(Nf32)j*txc_matrix.YAxis.x;		
							pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+(Nf32)j*txc_matrix.YAxis.y;		
						}
					}

					// Adapt Building Matrix to place origin on the "first center"
					txc_matrix.Origin.x += 0.5f*(txc_matrix.XAxis.x+txc_matrix.YAxis.x);		
					txc_matrix.Origin.y += 0.5f*(txc_matrix.XAxis.y+txc_matrix.YAxis.y);		
					// The Quad center Vertex
					for(j=0;j<pbuilder->Grid.J_CellsNb;j++)
					{
						for(i=0;i<pbuilder->Grid.I_CellsNb;i++)
						{
							indexcenter =	firstcenter_index	+  j*pbuilder->Grid.I_CellsNb + i;

							pvertex[indexcenter].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+(Nf32)j*txc_matrix.YAxis.x;		
							pvertex[indexcenter].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+(Nf32)j*txc_matrix.YAxis.y;		
						}
					}
				}
				// COLOR
				// TODO

				// ===============================================================================================
				// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
				// ===============================================================================================
				if(repoint)
				{
					_skinarrayvertex_itop(pstruct,pgeom);
				}

				// =======================
				// Skin structure Creation 
				// =======================
				memset(&skin,0,sizeof(NSKIN));
				skin.Grid.SkinType					= NSKIN_TYPE_GRID;
				skin.Grid.StructureElementType		= pbuilder->Grid.Ref_StructureElementType; // It can be only NJOINT
				skin.Grid.pGeometry					= pgeom;
				skin.Grid.pFirstVertex				= (NSKINVERTEX*)NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex_index);
				skin.Grid.pFirst					= (void*)NGetArrayPtr(pref_array,pbuilder->Grid.Ref_First);
				skin.Grid.RangeSize					= pbuilder->Grid.Ref_RangeSize;
				//skin.Grid.ColumnSize				= pbuilder->Grid.Ref_ColumnSize;
				skin.Grid.I_CellsNb					= pbuilder->Grid.I_CellsNb;
				skin.Grid.J_CellsNb					= pbuilder->Grid.J_CellsNb;

				skin.Grid.pUpdateSkinGeometry		= NUpdateSkin_Grid_FanQuads;

				return (NSKIN*)NArrayPushBack(&pstruct->SkinArray,(NBYTE*)&skin);

			default:
				NErrorIf(1,NERROR_INVALID_CASE);
				break;
	}
	return NULL; // just to avoid a compiler Warning. The program will never pass here
}



// ----------------------------------------------------------------------------------------------------------------------------------
// CreateSkin_Rope
// ----------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set up and organize a geometry as a skin of a Physic structure
//	Depending parameters of "pbuilder" , the right number of vertex and triangles are build and set up.
//	
// ----------------------------------------------------------------------------------------------------------------------------------
// In  :
//			pstruct					a VALID POINTER on a STRUCTURE structure 
//									with is lists of NJOINT, SPRINGS and CONSTRAINTS.
//
//			pgeom					A valid pointer of a GEOMETRY (usually a part of an NOBBECT, but not necessary)
//
//			pbuilder				A valid pointer of a SKIN_BUILDER structure which contains all the user parameters
//									to build the Skin.
//									
//										- Skin_Scheme			Type of the mesh created
//																		
//										- SkinRef_Type			Type of the structure elements used to create and update the skin.
//																It can be NJOINT, SPRINGS or CONSTRAINTS.
//
//										- SkinRef_RangeStart	First element (index) of the range used. 
//
//										- SkinRef_RangeSize		Size of the elements range used.
//
//										- SkinRef_SubRangeSize	-NOT USED for a Rope-
// Out :
//			A pointer  on the new skin
// ----------------------------------------------------------------------------------------------------------------------------------
NSKIN* NCreateSkin_Rope(NSTRUCTURE *pstruct, NGEOMETRY	*pgeom, const NSKIN_DESC *pbuilder )
{
	NSKIN				skin;
	Nu32				nbvertex,nbtri;
	Nu32				firstvertex_index,firstprimitive_index;
	Nu32				indextri,indexvert;
	Nu32				i;
	Nu32				topA,topB;
	NARRAY				*pref_array;

	NTRIANGLE			*ptri;
	NSKINVERTEX			*pvertex;

	Nbool				repoint;
	NMATRIX3x3			txc_matrix;
	NSKIN_ROPE_VERTEBRA vertebra;

	// ===================================================================================
	// Initial Check
	// ===================================================================================
	NErrorIf( !pgeom, NERROR_STRUCTURE_SKIN_VALID_GEOMETRY_REQUESTED );
	NErrorIf( !pstruct->pRenderable, NERROR_STRUCTURE_SKIN_VALID_RENDERABLE_REQUESTED );
	NErrorIf( !NIsGeometryIncluded( pstruct->pRenderable, pgeom),NERROR_STRUCTURE_SKIN_UNKNOWN_GEOMETRY );
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)!=NSKINVERTEX_FORMAT, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)!=NPRIMITIVE_FORMAT_TRIANGLE, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT );

	// ==================================================================================
	// Verifications
	// ==================================================================================
	switch(pbuilder->Rope.Ref_StructureElementType)
	{
		case NSTRUCTURE_ELEMENT_JOINT:
			return NULL; // Impossible to create a Rope skin based only on joint.
			break;

		case NSTRUCTURE_ELEMENT_SPRING:
			pref_array = &pstruct->SpringArray;
			break;

		case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
			pref_array = &pstruct->SimpleConstraintArray;
			break;

		case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
			pref_array = &pstruct->MinMaxConstraintArray;
			break;

		case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
			pref_array = &pstruct->SpecialConstraintArray;
			break;

		default:
			return NULL;
			break;
	}

	memset(&skin,0,sizeof(NSKIN));

// ==================================================================================
// SKIN MESH CREATION
// ==================================================================================
	switch(pbuilder->Rope.Structure_Scheme)
	{
	// ============================================
	// 2 Quads rows along a Spin of chained springs
	// ============================================
		case NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS:
			// ================
			// Structure Scheme
			// ================
			// Vertex Index distribution
			//												TOP B     TOP A
			//					v+2 ----- v-1 ----- 6 ------- 3 ------- 0
			//					|         |			|		  |         |
			//					|         |			|         |         |
			//					|         |			|         |         |
			//	  (J) -- Sn -- v+3 --S3-- v --S2--- 7 --S1--- 4 --S0--- 1
			//	   +			|         |			|         |         |
			//	   |			|         |			|         |         |
			//	   |			|         |			|         |         |
			//	   |		   v+4 ----- v+1 ------ 8 ------- 5 ------- 2
			//	   |	
			//	   |
			//	The last joint
			//	Note that a Vertebra with its 2 sides is like this : 
			//
			//											+
			//											|
			//											|
			//											|
			//							jA ------------ jB
			//											|
			//											|
			//											|
			//											+
			//
			// A rope is a succession of vertebra like this, with one vertebra for each constraint.
			// Total number of vertebra is equal to the number of constraints used by the rope.
			//
			//						+ 			+   		+   		+
			//						|  			|			|			|
			//						|  			|			|			|
			//						|  			|			|			|
			//			 jn-------- jn-1--------jn-2------- j...------- j0
			//						|  			|			|			|
			//						|  			|			|			|
			//						|  			|			|			|
			//						+  			+			+			+


			// =============
			// Vertex number
			// =============
			// To create the skin we need to have at minimum: range_size*3 vertex
			nbvertex = pbuilder->Rope.Ref_RangeSize*3;

			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}
			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

			// ===============
			// Triangle number
			// ===============
			// To create the skin we need to have: 
			nbtri = (pbuilder->Rope.Ref_RangeSize-1)*4;
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//		2 ----- 1					2 ----- 3 
			//		| .  T0 |					| .  T0 |
			//		|   +   |					|   +   |
			//		| T1  . |					| T1  . |	
			//		3 ----- 0					1 ----- 0 
			//	T0(0,1,2) = (A+1,A,B)			T0(0,2,3) = (A+1,B,A)
			//	T1(0,2,3) = (A+1,B,B+1)			T1(0,1,2) = (A+1,B+1,B)
			//
			//		2 ----- 1					2 ----- 3 
			//		| .  T2 |					| .  T2 |
			//		|   +   |					|   +   |
			//		| T3  . |					| T3  . |	
			//		3 ----- 0					1 ----- 0 
			//	T2(0,1,2) = (A+2,A+1,B+1)		T2(0,2,3) = (A+2,B+1,A+1)
			//	T3(0,2,3) = (A+2,B+1,B+2)		T3(0,1,2) = (A+2,B+2,B+1)

 			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize-1;i++)
				{
					topA = firstvertex_index + i*3;
					topB = firstvertex_index + (i+1)*3;

					indextri = firstprimitive_index + i*4;
					ptri[indextri].i0 = topA+1;		ptri[indextri+1].i0 = topA+1;	
					ptri[indextri].i1 = topB;		ptri[indextri+1].i1 = topB+1;
					ptri[indextri].i2 = topA;		ptri[indextri+1].i2 = topB;

					ptri[indextri+2].i0 = topA+2;	ptri[indextri+3].i0 = topA+2;	
					ptri[indextri+2].i1 = topB+1;	ptri[indextri+3].i1 = topB+2;
					ptri[indextri+2].i2 = topA+1;	ptri[indextri+3].i2 = topB+1;

					NErrorIf( (topA+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
					NErrorIf( (topB+2)>=NGetArraySize(&pgeom->Mesh.VertexArray),NERROR_GEOMETRY_OUTOFRANGE_VERTEXID );
					NErrorIf( (indextri+2)>=NGetArraySize(&pgeom->Mesh.PrimitiveArray), NERROR_GEOMETRY_OUTOFRANGE_PRIMITIVEID);
				}
			}
			else // CCW The default One
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize-1;i++)
				{
					topA = firstvertex_index + i*3;
					topB = firstvertex_index + (i+1)*3;

					indextri = firstprimitive_index + i*4;
					ptri[indextri].i0 = topA+1;		ptri[indextri+1].i0 = topA+1;	
					ptri[indextri].i1 = topA;		ptri[indextri+1].i1 = topB;
					ptri[indextri].i2 = topB;		ptri[indextri+1].i2 = topB+1;

					ptri[indextri+2].i0 = topA+2;	ptri[indextri+3].i0 = topA+2;	
					ptri[indextri+2].i1 = topA+1;	ptri[indextri+3].i1 = topB+1;
					ptri[indextri+2].i2 = topB+1;	ptri[indextri+3].i2 = topB+2;

					NErrorIf( (topA+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
					NErrorIf( (topB+2)>=NGetArraySize(&pgeom->Mesh.VertexArray),NERROR_GEOMETRY_OUTOFRANGE_VERTEXID );
					NErrorIf( (indextri+2)>=NGetArraySize(&pgeom->Mesh.PrimitiveArray), NERROR_GEOMETRY_OUTOFRANGE_PRIMITIVEID);
				}
			}
			// =========================
			// UVs Setting up
			// =========================
			// FLAGS_NSKIN_DESC_NTXCBUILD_... may be all to OFF even if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to ON.
			// But, if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to OFF, all FLAGS_NSKIN_DESC_NTXCBUILD_... have to be set to OFF too.
			// Just check that before continuing...
			NErrorIf( ISFLAG_OFF(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) && pbuilder->Rope.TexCoordBuild.Flags!=0,NERROR_INCONSISTENT_PARAMETERS );

			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Rope.TexCoordBuild);
				pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);

				// TexCoords are going to be distributed on all the effective texture surface.
				// This whole surface is define thanks to the TexCoordBuilding 2D Matrix, so let's doing some update
				NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)(pbuilder->Rope.Ref_RangeSize-1)); // Ref_RangeSize-1 represents the number of Quads in one row.
				// We keep 	txc_matrix.XAxis as it is.

				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					indexvert = firstvertex_index + i*3;
						
					// TOP
					pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x;		
					pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y;		
					// MIDDLE
					pvertex[indexvert+1].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+0.5f*txc_matrix.YAxis.x;		
					pvertex[indexvert+1].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+0.5f*txc_matrix.YAxis.y;		
					// BOTTOM
					pvertex[indexvert+2].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+txc_matrix.YAxis.x;		
					pvertex[indexvert+2].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+txc_matrix.YAxis.y;		
					NErrorIf( (indexvert+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
				}
			}
			
			// COLOR: TOdo


			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// Set up the right Callback function for this type of Rope
			skin.Rope.pUpdateSkinGeometry		= NUpdateSkin_Rope_Quads_DoubleRows;
			break;

	// ==================================================
	// only one Quads row along a Spin of chained springs
	// ==================================================
		case NSKIN_SCHEME_ROPE_QUADS_ONEROW:
			// ================
			// Structure Scheme
			// ================
			//												TOP B     TOP A
			//					v+1 ----- v-1 ----- 4 ------- 2 ------- 0
			//					|         |			|		  |         |
			//					|         |			|         |         |
			//					|         |			|         |         |
			//	  (J) -- Sn --     --S3--   --S2---   --S1---   --S0--- 
			//	   +			|         |			|         |         |
			//	   |			|         |			|         |         |
			//	   |			|         |			|         |         |
			//	   |		   v+2 -----  v  ------ 5 ------- 3 ------- 1
			//	   |	
			//	   |
			//	The last joint
			// =============
			// Vertex number
			// =============
			// To create the skin we need to have at minimum: range_size*2 vertex
			nbvertex = pbuilder->Rope.Ref_RangeSize*2;
			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}

			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

			// ===============
			// Triangle number
			// ===============
			// To create the skin we need to have: 
			nbtri = (pbuilder->Rope.Ref_RangeSize-1)*2;
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//		2 ----- 1					2 ----- 3 
			//		| .  T0 |					| .  T0 |
			//		|   +   |					|   +   |
			//		| T1  . |					| T1  . |	
			//		3 ----- 0					1 ----- 0 
			//	T0(0,1,2) = (A+1,A,B)			T0(0,2,3) = (A+1,B,A)
			//	T1(0,2,3) = (A+1,B,B+1)			T1(0,1,2) = (A+1,B+1,B)
			//
			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize-1;i++)
				{
					topA = firstvertex_index+i*2;
					topB = firstvertex_index+(i+1)*2;

					indextri = firstprimitive_index+i*2;
					ptri[indextri].i0 = topA+1;		ptri[indextri+1].i0 = topA+1;	
					ptri[indextri].i1 = topB;		ptri[indextri+1].i1 = topB+1;
					ptri[indextri].i2 = topA;		ptri[indextri+1].i2 = topB;
				}
			}
			else // CCW
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize-1;i++)
				{
					topA = firstvertex_index+i*2;
					topB = firstvertex_index+(i+1)*2;

					indextri = firstprimitive_index+i*2;
					ptri[indextri].i0 = topA+1;		ptri[indextri+1].i0 = topA+1;	
					ptri[indextri].i1 = topA;		ptri[indextri+1].i1 = topB;
					ptri[indextri].i2 = topB;		ptri[indextri+1].i2 = topB+1;
				}
			}
			// =========================
			// UVs Setting up
			// =========================
			//	0,1 ----- 1,1 	
			//	 |         |	
			//	 |         |	
			//	 |         |	
			//	0,1 ----- 1,1	
			// FLAGS_NSKIN_DESC_NTXCBUILD_... may be all to OFF even if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to ON.
			// But, if FLAG_NSKIN_DESC_BUILD_TEXCOORDS is set to OFF, all FLAGS_NSKIN_DESC_NTXCBUILD_... have to be set to OFF too.
			// Just check that before continuing...
			NErrorIf( ISFLAG_OFF(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) && pbuilder->Rope.TexCoordBuild.Flags!=0,NERROR_INCONSISTENT_PARAMETERS );
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Rope.TexCoordBuild);
				pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);

				// TexCoords are going to be distributed on all the effective texture surface.
				// This whole surface is define thanks to the TexCoordBuilding 2D Matrix, so let's doing some update
				NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)(pbuilder->Rope.Ref_RangeSize-1)); // Ref_RangeSize represents the number of Quads in one row.
				// We keep 	txc_matrix.XAxis as it is.

				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					indexvert = firstvertex_index + i*2;
						
					// TOP
					pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x;		
					pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y;		
					// BOTTOM
					pvertex[indexvert+1].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+txc_matrix.YAxis.x;		
					pvertex[indexvert+1].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+txc_matrix.YAxis.y;		
					NErrorIf( (indexvert+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
				}
			}
			// COLOR

			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// Set up the right Callback function for this type of Rope
			skin.Rope.pUpdateSkinGeometry		= NUpdateSkin_Rope_Quads_OneRow;
			break;

// ==================================================
// 2 Triangles rows along a Spin of chained springs
// ==================================================
		case NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW:
			// ================
			// Structure Scheme
			// ================
			//							    A
			//		       .6      .3      .0
			//		     .  |    .  |    .  |
			//		   .    |  .    |  .    |
			//		 .      |.     B|.      |
			//	   9----SN--7--S1---4--S0---1 
			//		 .      |.      |.      |
			//		   .    |  .    |  .    |
			//		     .  |    .  |    .  |
			//			   .8      .5      .2
			// =============
			// Vertex number
			// =============
			// To create the skin we need to have at minimum: range_size*3+1 vertex
			nbvertex = pbuilder->Rope.Ref_RangeSize*3+1;
			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}

			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

			// ===============
			// Triangle number
			// ===============
			// To create the skin we need to have at minimum: range_size*2 triangles
			nbtri = pbuilder->Rope.Ref_RangeSize*2;
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//
			//		       .1				       .3
			//		     .  |				     .  |
			//		   .    |				   .    |
			//		 .   T0 |				 .   T0 |
			//	   2--------0			   2--------0
			//		 .   T1 |				 .   T1 |
			//		   .    |				   .    |
			//		     .  |				     .  |
			//			   .3					   .1
			//	T0(0,1,2) = (A+1,A,B)			T0(0,2,3) = (A+1,B,A)
			//	T1(0,2,3) = (A+1,B,A+2)			T1(0,1,2) = (A+1,A+2,B)
			//
			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					topA		= firstvertex_index+i*3;
					indextri	= firstprimitive_index+i*2;
					
					topB = topA+4;
					if(i==pbuilder->Rope.Ref_RangeSize-1)
						topB--;

					ptri[indextri].i0 = topA+1;	
					ptri[indextri].i1 = topB;	
					ptri[indextri].i2 = topA;	
					
					ptri[indextri+1].i0 = topA+1;	
					ptri[indextri+1].i1 = topA+2;	
					ptri[indextri+1].i2 = topB;	
				}
			}
			else // CCW
			{
				ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					topA		= firstvertex_index+i*3;
					indextri	= firstprimitive_index+i*2;

					topB = topA+4;
					if(i==pbuilder->Rope.Ref_RangeSize-1)
						topB--;

					ptri[indextri].i0 = topA+1;	
					ptri[indextri].i1 = topA;	
					ptri[indextri].i2 = topB;	

					ptri[indextri+1].i0 = topA+1;	
					ptri[indextri+1].i1 = topB;	
					ptri[indextri+1].i2 = topA+2;	
				}
			}
			// =========================
			// UVs Setting up
			// =========================
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Rope.TexCoordBuild);
				pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);

				// TexCoords are going to be distributed on all the effective texture surface.
				// This whole surface is define thanks to the TexCoordBuilding 2D Matrix, so let's doing some update
				NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)pbuilder->Rope.Ref_RangeSize); // Ref_RangeSize represents the number of Quads in one row.
				// We keep 	txc_matrix.XAxis as it is.

				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					indexvert = firstvertex_index + i*3;
					NErrorIf( (indexvert+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
					
					// TOP
					pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x;		
					pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y;		
					// MIDDLE
					pvertex[indexvert+1].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+0.5f*txc_matrix.YAxis.x;		
					pvertex[indexvert+1].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+0.5f*txc_matrix.YAxis.y;		
					// BOTTOM
					pvertex[indexvert+2].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+txc_matrix.YAxis.x;		
					pvertex[indexvert+2].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+txc_matrix.YAxis.y;		
				}

				// And the last Vertex ... at the tail
				// It has to be considered as a MIDDLE one
				indexvert = firstvertex_index + pbuilder->Rope.Ref_RangeSize*3;
				NErrorIf( (indexvert)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);

				pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)pbuilder->Rope.Ref_RangeSize*txc_matrix.XAxis.x+0.5f*txc_matrix.YAxis.x;		
				pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)pbuilder->Rope.Ref_RangeSize*txc_matrix.XAxis.y+0.5f*txc_matrix.YAxis.y;		
			}
			// COLOR

			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// Set up the right Callback function for this type of Rope
			skin.Rope.pUpdateSkinGeometry		= NUpdateSkin_Rope_Quads_DoubleRows;
			break;

			// ==================================================
			// only one Triangles row along a Spin of chained springs
			// ==================================================
		case NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW:
			// ================
			// Structure Scheme
			// ================
			//							  TOP A
			//		       .6      .3      .0
			//		     .  |    .  |    .  |
			//		   .    |  .    |  .    |
			//		 .      |.      |.      |
			//	   8----SN--5--S1---2--S0--- 
			//		 .      |.      |.      |
			//		   .    |  .    |  .    |
			//		     .  |    .  |    .  |
			//			   .7      .4      .1
			// =============
			// Vertex number
			// =============
			// To create the skin we need to have at minimum: range_size*3 vertex
			nbvertex = pbuilder->Rope.Ref_RangeSize*3;
			firstvertex_index = NGetArraySize(&pgeom->Mesh.VertexArray);
			repoint = NFALSE;
			if((firstvertex_index+nbvertex)>NGetArrayCapacity(&pgeom->Mesh.VertexArray))
			{
				// A memory reallocation is going to happen !
				// So if some skin reference vertex (NVERTEX*) these ones need to be updated.
				repoint = _skinarrayvertex_ptoi(pstruct,pgeom);
			}

			NUpSizeArray(&pgeom->Mesh.VertexArray,nbvertex,NULL);

			// ===============
			// Triangle number
			// ===============
			// To create the skin we need to have at minimum: range_size triangles
			nbtri = pbuilder->Rope.Ref_RangeSize;
			firstprimitive_index = NGetArraySize(&pgeom->Mesh.PrimitiveArray);
			NUpSizeArray(&pgeom->Mesh.PrimitiveArray,nbtri,NULL);

			// =========================
			// Triangle index Setting up
			// =========================
			//	CCW Winding Order			CW Winding Order
			//				A						A
			//		       .1				       .2
			//		     .  |				     .  |
			//		   .    |				   .    |
			//		 .      |				 .      |
			//	   2	T0	|			   1	T0	|
			//		 .		|				 .		|
			//		   .    |				   .    |
			//		     .  |				     .  |
			//			   .0					   .0
			//	T0(0,1,2) = (A+1,A,A+2)			T0(0,1,2) = (A+1,A+2,A)

			ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING) )
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					topA = firstvertex_index+i*3;

					indextri = firstprimitive_index+i;
					ptri[indextri].i0 = topA+1;	
					ptri[indextri].i1 = topA+2;	
					ptri[indextri].i2 = topA;	
				}
			}
			else // CCW
			{
				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					topA = firstvertex_index+i*3;

					indextri = firstprimitive_index+i;
					ptri[indextri].i0 = topA+1;	
					ptri[indextri].i1 = topA;	
					ptri[indextri].i2 = topA+2;	
				}
			}
			// =========================
			// UVs Setting up
			// =========================
			if( ISFLAG_ON(pbuilder->Rope.Flags,FLAG_NSKIN_DESC_BUILD_TEXCOORDS) )
			{
				NGetBuildBaseTexCoordMatrix3x3(&txc_matrix,&pbuilder->Rope.TexCoordBuild);
				pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);

				// TexCoords are going to be distributed on all the effective texture surface.
				// This whole surface is define thanks to the TexCoordBuilding 2D Matrix
				NVec3ScaleBy(&txc_matrix.XAxis, 1.0f/(Nf32)pbuilder->Rope.Ref_RangeSize); // Ref_RangeSize represents the number of Quads in one row.
				// We keep 	txc_matrix.XAxis as it is.

				for(i=0;i<pbuilder->Rope.Ref_RangeSize;i++)
				{
					indexvert = firstvertex_index + i*3;
					NErrorIf( (indexvert+2)>=NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);

					// TOP
					pvertex[indexvert].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x;		
					pvertex[indexvert].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y;		
					// BOTTOM
					pvertex[indexvert+1].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)i*txc_matrix.XAxis.x+txc_matrix.YAxis.x;		
					pvertex[indexvert+1].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)i*txc_matrix.XAxis.y+txc_matrix.YAxis.y;		
					// MIDDLE
					pvertex[indexvert+2].TexCoord0_2f.x = txc_matrix.Origin.x + (Nf32)(i+1)*txc_matrix.XAxis.x+0.5f*txc_matrix.YAxis.x;		
					pvertex[indexvert+2].TexCoord0_2f.y = txc_matrix.Origin.y + (Nf32)(i+1)*txc_matrix.XAxis.y+0.5f*txc_matrix.YAxis.y;		
				}
			}
			// COLOR







			// ===============================================================================================
			// Potential Vertex reallocation was done, so, we can registered pointer on the reallocated vertex
			// ===============================================================================================
			if(repoint)
			{
				_skinarrayvertex_itop(pstruct,pgeom);
			}

			// Set up the right Callback function for this type of Rope
			skin.Rope.pUpdateSkinGeometry		= NUpdateSkin_Rope_Quads_DoubleRows;
			break;

			default:
				return NULL;
	}

	// ==================================================================
	// Finalize the Skin Rope parameters setting up
	// all the settings below are set just once, here, because these ones are common
	// for all the rope types ...
	// Actually there is only one specific parameter which is:
	//		- skin.Rope.pUpdateSkinGeometry
	// It is set at the end of each case above ....
	// ==================================================================
	// skin.Rope.pUpdateSkinGeometry	= Already set in relationship with the requested type of rope
	//									  Have a quick look at the end of each "case" above ....	

	skin.Rope.SkinType					= NSKIN_TYPE_ROPE;
	skin.Rope.StructureElementType		= pbuilder->Rope.Ref_StructureElementType;
	skin.Rope.pGeometry					= pgeom;
	skin.Rope.pFirstVertex				= (NSKINVERTEX*)NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex_index);
//	skin.Rope.TwistBulgeFactor			= pbuilder->Rope.TwistBulgeFactor;
	skin.Rope.pFirst					= (void*)NGetArrayPtr(pref_array,pbuilder->Rope.Ref_First);
	skin.Rope.RangeSize					= pbuilder->Rope.Ref_RangeSize;

/*
	skin.Rope.pSideAVertebra			= NCreateArray(pbuilder->Rope.Ref_RangeSize+1,sizeof(Nf32));//pbuilder->Rope.pSideAVertebra;
	skin.Rope.pSideBVertebra			= NCreateArray(pbuilder->Rope.Ref_RangeSize+1,sizeof(Nf32));//pbuilder->Rope.pSideBVertebra;
	NResizeArray(skin.Rope.pSideAVertebra,(pbuilder->Rope.Ref_RangeSize+1),(NBYTE*)&pbuilder->Rope.VertebraSize,NULL );
	NResizeArray(skin.Rope.pSideBVertebra,(pbuilder->Rope.Ref_RangeSize+1),(NBYTE*)&pbuilder->Rope.VertebraSize, NULL );
*/
	vertebra.Bulge = pbuilder->Rope.TwistBulgeFactor;
	vertebra.SideA = pbuilder->Rope.VertebraSize;
	vertebra.SideB = pbuilder->Rope.VertebraSize;
	NSetupArray(&skin.Rope.VertebrasArray,pbuilder->Rope.Ref_RangeSize,sizeof(NSKIN_ROPE_VERTEBRA));
	NResizeArray(&skin.Rope.VertebrasArray,pbuilder->Rope.Ref_RangeSize,(NBYTE*)&vertebra, NULL );
	return (NSKIN*)NArrayPushBack(&pstruct->SkinArray,(NBYTE*)&skin);
}

NSKIN_SCHEME NGetSkinSheme(const NUPDATESKIN_FCT fct)
{
	if(fct==NUpdateSkin_Billboard_Quad)
		return NSKIN_SCHEME_BILLBOARD_1QUAD;
	else if(fct==NUpdateSkin_Grid_Quads)
		return NSKIN_SCHEME_GRID_QUADS;
	else if(fct==NUpdateSkin_Grid_FanQuads)
		return NSKIN_SCHEME_GRID_FANQUADS;
	else if(fct==NUpdateSkin_Rope_Quads_DoubleRows)
		return NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS;
	else if(fct==NUpdateSkin_Rope_Quads_OneRow)
		return NSKIN_SCHEME_ROPE_QUADS_ONEROW;
	else if(fct==NUpdateSkin_Rope_Triangles_DoubleRows)
		return NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW;
	else if(fct==NUpdateSkin_Rope_Triangles_OneRow)
		return NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW;

	return NSKIN_SCHEME_VOID;
}
NUPDATESKIN_FCT	NSetSkinUpdateFunction(NSKIN *pskin, const NSKIN_SCHEME sh)
{
	NUPDATESKIN_FCT old;

	old = pskin->common.pUpdateSkinGeometry;
	switch(sh)
	{
		case NSKIN_SCHEME_BILLBOARD_1QUAD:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Billboard_Quad;
			break;  

		case NSKIN_SCHEME_GRID_QUADS:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Grid_Quads;
			break;  

		case NSKIN_SCHEME_GRID_FANQUADS:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Grid_FanQuads;
			break;  

		case NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Rope_Quads_DoubleRows;
			break;  

		case NSKIN_SCHEME_ROPE_QUADS_ONEROW:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Rope_Quads_OneRow;
			break;  
		
		case NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Rope_Triangles_DoubleRows;
			break;  
		
		case NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW:
			pskin->common.pUpdateSkinGeometry = NUpdateSkin_Rope_Triangles_OneRow;
			break;

		default:
			old = NULL;
			break;
	}
	return old;
}

/*
void DeleteSkin(NSKIN *pskin)
{
	// Remove NNODE from the Doubly-Linked List
	NNodeRemove((NNODE*)pskin,&SkinList);
	
	// Free Memory allocated by the Skin
	if(pskin->pVertebraLength_LUT)
	{
		NDeleteFastAnimatedValueLUT(pskin->pVertebraLength_LUT);
	}
	free(pskin);
}
*/

/*

Old method
------------
void Skins_Update()
{
	NNODE	*pnode;
	SKIN	*pskin;

	pnode = (NNODE*)SkinList.pFirst;
	while( pnode != (NNODE*)&SkinList )
	{
		pskin = (SKIN*)pnode;
		pskin->pUpdateSkinGeometry(pskin);

		pnode=(NNODE*)pnode->pNext;
	}
}
*/


// ----------------------------------------------------------------------------------------------------------------------------------
// NDeleteSkin
// ----------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Delete a Structure skin and all its dependencies
//		!!! IMPORTANT !!! Deleting a SKIN imply deleting associated VERTEX and TRIANGLE which are included 
//		!!! IMPORTANT !!! into one of the Structure Renderable Geometry ...
//		!!! IMPORTANT !!! After this operation the geometry may be empty ( 0 vertex, 0 primitive )
//		!!! IMPORTANT !!! So, call 'NDeleteEmptyStructureGeometries' of 'NDeletePhysicStructureGeometry'.
//		!!! IMPORTANT !!! DONT USE NDeleteGeometry because some Structure pointers reference the geometry and have to be updated !!! 
// ----------------------------------------------------------------------------------------------------------------------------------
// In  :
//			pstructure					a VALID POINTER on a STRUCTURE structure 
//										with is lists of NJOINT, SPRINGS and CONSTRAINTS and SKINs.
//
//			pskin						A valid pointer of a SKIN to delete
// Out :
// ----------------------------------------------------------------------------------------------------------------------------------
void NDeleteSkin(NSTRUCTURE *pstructure, NSKIN *pskin)
{
	// Debug check #1
	NErrorIf(!NIsValidArrayElementPtr(&pstructure->SkinArray,(NBYTE*)pskin), NERROR_ARRAY_PTR_BEYOND_LIMITS);
	NGEOMETRY	*pgeom = pskin->common.pGeometry;
	NErrorIf(!NIsGeometryIncluded(pstructure->pRenderable,pgeom), NERROR_STRUCTURE_SKIN_UNKNOWN_GEOMETRY);
	
	
	Nu32		nbvertex;
	Nu32		nbtri;
	Nu32		i;

	NTRIANGLE	*ptri;
	Nu32		first_tri;
	Nu32		first_vertex;
	NSKIN		*psk;

#ifdef _DEBUG	
	Nu32		dbg_i;
	Nu32		dbg_nbskin; 	
	NSKIN		*dbg_psk;
#endif

	// RULE:
	// Each skin has a pointer to a first vertex. This one belongs to the skin linked geometry.
	// For each skin there is a UNIQUE RANGE of CONSECUTIVE vertex which starts from the first one.
	// Total vertex number depends of the skin type.
	// Before erasing the skin we have to erase all its linked geometry dependencies (vertex, uvs, primitives ... )

	// So:
	// We have a pointer on the first Vertex.
	// We have to look for the exact number of vertex. They are consecutive, that's a rule.
	// ( This exact number, as the exact number of triangles used by the skin, is computable from the skin type and from some other skin parameters... )
	// We have to look for each primitive (triangle) which uses at least one vertex of the skin vertex range.

	// Let's go !
	// 1) Exact number of consecutive vertex use by the skin
	//		... And Exact number of triangles used by the skin:
	switch( NGetSkinSheme(pskin->common.pUpdateSkinGeometry) )
	{
		case NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS:
			nbvertex	= pskin->Rope.RangeSize*3;
			nbtri		= (pskin->Rope.RangeSize-1)*4;
			break;

		case NSKIN_SCHEME_ROPE_QUADS_ONEROW:
			nbvertex	= pskin->Rope.RangeSize*2;
			nbtri		= (pskin->Rope.RangeSize-1)*2;
			break;

		case NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW:
			nbvertex	= pskin->Rope.RangeSize*3+1;
			nbtri		= pskin->Rope.RangeSize*2;
			break;

		case NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW:
			nbvertex	= pskin->Rope.RangeSize*3;
			nbtri		= pskin->Rope.RangeSize;
			break;

		case NSKIN_SCHEME_GRID_QUADS:
			nbvertex	= pskin->Grid.RangeSize;
			nbtri		= pskin->Grid.I_CellsNb*pskin->Grid.J_CellsNb*2; 
			break;

		case NSKIN_SCHEME_GRID_FANQUADS:
			nbvertex	= pskin->Grid.RangeSize;
			nbtri		= pskin->Grid.I_CellsNb*pskin->Grid.J_CellsNb*4; 
			break;

		case NSKIN_SCHEME_BILLBOARD_1QUAD:
			nbvertex= 4;
			nbtri	= 2;
			break;

        default:
            NErrorIf(1,NERROR_INVALID_CASE);
            break;
	}
	
	// 2)	!!!! IS THERE ANY OTHER SKIN USING THE SAME GEOMETRY ?????
	//		Each skin create it's own vertex and primitives. So if a Physic Structure Geometry has the exact number of vertex
	//		used by the skin, then we can be sure that SKIN is the Only One using the geometry ...

	if( nbvertex != pgeom->Mesh.VertexArray.Size )
	{

		// Just some checks to detect very strange cases ...
#ifdef _DEBUG
		dbg_nbskin = 0; 	
		dbg_psk = (NSKIN*)pstructure->SkinArray.pFirst;
		for(dbg_i=pstructure->SkinArray.Size;dbg_i!=0;dbg_i--,dbg_psk++)
		{
			if( dbg_psk->common.pGeometry == pgeom )
			{
				dbg_nbskin++;
			}
		}
		// Number Skin that reference the same geometry than pskin should be greater than 1...
		// Thx to "Debug check #1"(at the top of this function,  we are sure pskin is an element of pstructure skin array
		// So if Skin number is <= 1 and if nbvertex is different than the geometry nbvertex, that means
		// geometry contains some vertex created from an unknown way ...
		// It's not impossible, maybe it's not a bug, but if it's happen it's better to check and understand and maybe 
		// make the code involve here !
		NErrorIf(dbg_nbskin <= 1, NERROR_SYSTEM_CHECK ); 
#endif
		// 2.1)	Finding the first primitive which use at least one vertex of the skin vertex range
		//		This first triangle is the first of a range of nbtri !  
		//		( it has to be like this, otherwise there is a problem ...)
		first_vertex = NGetArrayIndex(&pgeom->Mesh.VertexArray,(NBYTE*)pskin->common.pFirstVertex);
		NErrorIf( (first_vertex + nbvertex) > NGetArraySize(&pgeom->Mesh.VertexArray), NERROR_ARRAY_WRONG_SIZE ); // Not enough vertex in the array ... it's impossible ... So ... it's bad !

		first_tri = NVOID;
		ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
		for(i=0;i<NGetArraySize(&pgeom->Mesh.PrimitiveArray);i++,ptri++)
		{
			if( (ptri->i0 >= first_vertex && ptri->i0<(first_vertex+nbvertex))|| 
				(ptri->i1 >= first_vertex && ptri->i1<(first_vertex+nbvertex))|| 
				(ptri->i2 >= first_vertex && ptri->i2<(first_vertex+nbvertex)) )
			{
				first_tri = i;
				break;
			}
		}
		NErrorIf(first_tri == NVOID,NERROR_ARRAY_ELEMENT_NOT_FOUND); // No triangle found ! It's strange...  unnecessary to do this, but anyway in debug mode we can !
		NErrorIf( (first_tri + nbtri) > NGetArraySize(&pgeom->Mesh.PrimitiveArray),NERROR_ARRAY_WRONG_SIZE); // Not enough primitive in the array ... Impossible,but ... So ... it's bad, it's a bug !

		// 2.2) Look for the other skin which are using the same geometry than the one we are going to delete.
		//		The problem we have to solve are:
		//		- 'pGeometry' pointer for all skins (if the geometry is deleted, all geometry address are going to change.)
		//		- 'pFirstVertex' pointer for each skin which reference this geometry
		//		- Triangles index for all the other triangles included into the geometry primitives list
		ptri = (NTRIANGLE*)NGetFirstArrayPtr(&pgeom->Mesh.PrimitiveArray);
		for(i=pgeom->Mesh.PrimitiveArray.Size;i!=0;i--,ptri++)
		{
			if( ptri->i0 > first_vertex)
				ptri->i0 -= nbvertex;

			if( ptri->i1 > first_vertex)
				ptri->i1 -= nbvertex;

			if( ptri->i2 > first_vertex)
				ptri->i2 -= nbvertex;
		}

		// 2.3) VERTEX ... From Pointer To Index
		psk = (NSKIN*)NGetFirstArrayPtr(&pstructure->SkinArray);
		for(i=pstructure->SkinArray.Size;i!=0;i--,psk++)
		{
			if( psk == pskin )
				continue;

			if(psk->common.pGeometry == pgeom)
			{
				// a) pFirstVertex pointer re-assign part I (store index instead of address ! just for a while ...)
				psk->common.pFirstVertex = (NSKINVERTEX*)NGetArrayIndex(&pgeom->Mesh.VertexArray,(NBYTE*)psk->common.pFirstVertex);
			}
		}
		// 2.4) From here we consider we have a range of 'nbtri' triangles with 'first_tri' as the first one.
		//    So we are going to delete them all.
		NEraseArrayRange(&pgeom->Mesh.PrimitiveArray,first_tri,nbtri,NULL);

		// 2.5) Deleting Vertex, UVs and Colors (Ranges of ... )
		NEraseArrayRange(&pgeom->Mesh.VertexArray,first_vertex,nbvertex,NULL);

		// 2.6) VERTEX ... return back to Pointer (from Index)
		psk = (NSKIN*)NGetFirstArrayPtr(&pstructure->SkinArray);
		for(i=pstructure->SkinArray.Size;i!=0;i--,psk++)
		{
			if( psk == pskin )
				continue;

			if(psk->common.pGeometry == pgeom)
			{
				// a) pFirstVertex pointer re-assign part II ( Restore address from index value temporary stored instead... see part I)
				if( (Nu32)psk->common.pFirstVertex > first_vertex )
					psk->common.pFirstVertex = (NSKINVERTEX *)NGetArrayPtr(&pgeom->Mesh.VertexArray,(Nu32)psk->common.pFirstVertex - nbvertex );
				else
					psk->common.pFirstVertex = (NSKINVERTEX *)NGetArrayPtr(&pgeom->Mesh.VertexArray,(Nu32)psk->common.pFirstVertex );
			}
		}
	}
	else // nbvertex == pgeom->Mesh.VertexArray.Size !!!
	{
		// In that specific case, with only ONE SKIN using the geometry
		// We have just to Erase geometry Mesh
		
		// Just some checks to detect very strange cases ...
#ifdef _DEBUG
		dbg_nbskin = 0; 	
		dbg_psk = (NSKIN*)NGetFirstArrayPtr(&pstructure->SkinArray);
		for(dbg_i=NGetArraySize(&pstructure->SkinArray);dbg_i!=0;dbg_i--,dbg_psk++)
		{
			if( dbg_psk->common.pGeometry == pgeom )
			{
				dbg_nbskin++;
			}
		}
		// Number of Skin using the geometry has to be 1 !
		// because the number of vertex used by the skin matches exactly with the number of geometry vertex...
		// So, these vertex are supposed to be those of the skin ... 	
		NErrorIf(dbg_nbskin != 1, NERROR_SYSTEM_GURU_MEDITATION );
#endif
		
		NEraseGeometryMesh(pgeom);
	}
	// 3) Deleting Skin
	NEraseArrayElementPtr(&pstructure->SkinArray,(NBYTE*)pskin,NSkinDestructor_ArrayCallBack );
}

void NErasePhysicStructureSkinArray(NSTRUCTURE *ps)
{
	NEraseArray(&ps->SkinArray,NSkinDestructor_ArrayCallBack);
}

/*
static inline void _setskinUV_rope_quads_doublerows(NSKIN *pskin,const NTEXCOORD_2f *puvorigin, const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
// 	Nu32			first_index;
	Nu32			i;
// 	Nu32			index;
	Nf32			u_propagation;
	Nf32			v_propagation;
	Nf32			util,vtil;
	NSKINVERTEX		*pvertex;


	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_U) )
	{
		util = puvtiling->x;
	}	
	else
	{
		util = puvtiling->x/(Nf32)pskin->Rope.RangeSize;
	}

	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_V) )
	{
		vtil = puvtiling->y;
	}	
	else
	{
		vtil = puvtiling->y/2.0f; // because of DOUBLEROWS
	}

	pvertex = (NSKINVERTEX*)NGetFirstArrayPtr(&pskin->Rope.pGeometry->Mesh.VertexArray);
	// Because UVs index are the same than Vertex Index
	// we will get the first skin vertex index and use it as the index of the first UV of the skin.
	
	//first_index = NGetArrayIndex(&pskin->Rope.pGeometry->Mesh.VertexArray,(NBYTE*)pskin->Rope.pFirstVertex);
	pvertex = pskin->Rope.pFirstVertex;
	u_propagation = 1.0f;
	v_propagation = 0.0f;

	for(i=(pskin->Rope.RangeSize+1);i!=0;i--)
	{
		//index = first_index + i*3;

		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_U))
			u_propagation = 1.0f - u_propagation;
		else
			u_propagation = (Nf32)i;
		pvertex->TexCoord0_2f.x =  puvorigin->x + u_propagation*util;		
		pvertex->TexCoord0_2f.y =  puvorigin->y;						// 0 X	
		pvertex++;
// 		puv[index].x	= puvorigin->x + u_propagation*util;		
// 		puv[index].y	= puvorigin->y;							// 0 X	

		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_U))
			u_propagation = 1.0f - u_propagation;
		else
			u_propagation = (Nf32)i;
		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_V))
			v_propagation = 1.0f - v_propagation;
		else
			v_propagation = 1.0f;
		pvertex->TexCoord0_2f.x = puvorigin->x + u_propagation*util;		
		pvertex->TexCoord0_2f.y = puvorigin->y + v_propagation*vtil;		// 1 X
		pvertex++;

// 		puv[index+1].x	= puvorigin->x + u_propagation*util;		
// 		puv[index+1].y	= puvorigin->y + v_propagation*vtil;		// 1 X

		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_U))
			u_propagation = 1.0f - u_propagation;
		else
			u_propagation = (Nf32)i;
		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_V))
			v_propagation = 1.0f - v_propagation;
		else
			v_propagation = 2.0f;
		pvertex->TexCoord0_2f.x = puvorigin->x + u_propagation*util;		
		pvertex->TexCoord0_2f.y = puvorigin->y + v_propagation*vtil;		// 2 X
		pvertex++;
// 		puv[index+2].x	= puvorigin->x + u_propagation*util;		
// 		puv[index+2].y	= puvorigin->y + v_propagation*vtil;		// 2 X
	}
}

static inline void _setskinUV_grid_quads(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	Nu32		i,j;	
	Nf32		u_propagation;
	Nf32		v_propagation;
	Nf32		util,vtil;
	NSKINVERTEX	*pvertex,*pfirst_vertex;


	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_U) )
	{
		util = puvtiling->x;
	}	
	else
	{
		util = puvtiling->x/(Nf32)pskin->Grid.I_CellsNb;
	}

	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_V) )
	{
		vtil = puvtiling->y;
	}	
	else
	{
		vtil = puvtiling->y/(Nf32)pskin->Grid.J_CellsNb;
	}

	//first_index = NGetArrayIndex(&pskin->Grid.pGeometry->VertexArray,(NBYTE*)pskin->Grid.pFirstVertex);
	pfirst_vertex = pskin->Grid.pFirstVertex;
	u_propagation = 1.0f;
	v_propagation = 1.0f;

	for(j=0;j<pskin->Grid.J_CellsNb+1;j++)
	{
		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_V))
			v_propagation = 1.0f - v_propagation;
		else
			v_propagation = (Nf32)j;

		for(i=0;i<pskin->Grid.I_CellsNb+1;i++)
		{
			if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_U))
				u_propagation = 1.0f - u_propagation;
			else
				u_propagation = (Nf32)i;

			pvertex	=	pfirst_vertex + j*(pskin->Grid.I_CellsNb+1)+ i;

			pvertex->TexCoord0_2f.x = puvorigin->x + u_propagation * util;		
			pvertex->TexCoord0_2f.y = puvorigin->y + v_propagation * vtil;		
		}
	}
}

static inline void _setskinUV_grid_fanquads(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	Nu32	i,j;
	Nf32	u_propagation;
	Nf32	v_propagation;
	Nf32	util,vtil;
	NSKINVERTEX	*pvertex,*pvertex2,*pfirst_vertex;
	NSKINVERTEX	*pcenter,*pfirstcenter_vertex;



	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_U) )
	{
		util = puvtiling->x;
	}	
	else
	{
		util = puvtiling->x/(Nf32)pskin->Grid.I_CellsNb;
	}

	if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_RELATIVETILING_V) )
	{
		vtil = puvtiling->y;
	}	
	else
	{
		vtil = puvtiling->y/(Nf32)pskin->Grid.J_CellsNb;
	}

// 	first_index = NGetArrayIndex(&pskin->Grid.pGeometry->VertexArray,(NBYTE*)pskin->Grid.pFirstVertex);
// 	firstcenter_index = first_index + (pskin->Grid.I_CellsNb+1)*(pskin->Grid.J_CellsNb+1);
	pfirst_vertex		= pskin->Grid.pFirstVertex;
	pfirstcenter_vertex	= pfirst_vertex + (pskin->Grid.I_CellsNb+1)*(pskin->Grid.J_CellsNb+1);
	u_propagation = 1.0f;
	v_propagation = 1.0f;

	for(j=0;j<pskin->Grid.J_CellsNb+1;j++)
	{
		if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_V))
			v_propagation = 1.0f - v_propagation;
		else
			v_propagation = (Nf32)j;

		for(i=0;i<pskin->Grid.I_CellsNb+1;i++)
		{
			if( ISFLAG_ON(flags,FLAG_NSKIN_DESC_UV_MIRROR_U))
				u_propagation = 1.0f - u_propagation;
			else
				u_propagation = (Nf32)i;

			pvertex	=	pfirst_vertex + j*(pskin->Grid.I_CellsNb+1)+ i;

			pvertex->TexCoord0_2f.x = puvorigin->x + u_propagation * util;		
			pvertex->TexCoord0_2f.y = puvorigin->y + v_propagation * vtil;		
		}
	}

	// and the center ...
	for(j=0;j<pskin->Grid.J_CellsNb;j++)
	{
		for(i=0;i<pskin->Grid.I_CellsNb;i++)
		{
			pvertex			= pfirst_vertex + j*(pskin->Grid.I_CellsNb+1)+ i;
			pvertex2		= pvertex + pskin->Grid.I_CellsNb+2;
			pcenter			= pfirstcenter_vertex + j*pskin->Grid.I_CellsNb + i;
			
			//NVec2Lerp((NVEC2*)&puv[center_index],(NVEC2*)&puv[index],(NVEC2*)&puv[index+pskin->Grid.I_CellsNb+2],0.5f);
			NVec2Lerp((NVEC2*)&pcenter->TexCoord0_2f,(NVEC2*)&pvertex->TexCoord0_2f,(NVEC2*)&pvertex2->TexCoord0_2f,0.5f);
		}
	}
}



static inline void _setskinUV_rope_quads_onerow(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	// TODO ...
}
static inline void _setskinUV_rope_triangles_doublerows(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
 	// TODO ...
}
static inline void _setskinUV_rope_triangles_onerow(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	// TODO ...
}

static inline void _setskinUV_billboard_1quad(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	// TODO ...
}
// for flags use 'FLAG_NSKIN_DESC_UV_xxx" type.
void NSetSkinUVs(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags)
{
	switch( NGetSkinSheme(pskin->common.pUpdateSkinGeometry) )
	{
		case NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS:
			_setskinUV_rope_quads_doublerows(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_ROPE_QUADS_ONEROW:
			_setskinUV_rope_quads_onerow(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW:
			_setskinUV_rope_triangles_doublerows(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW:
			_setskinUV_rope_triangles_onerow(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_GRID_QUADS:
			_setskinUV_grid_quads(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_GRID_FANQUADS:
			_setskinUV_grid_fanquads(pskin,puvorigin,puvtiling,flags);
			break;
		case NSKIN_SCHEME_BILLBOARD_1QUAD:
			_setskinUV_billboard_1quad(pskin,puvorigin,puvtiling,flags);
			break;
        default:
            NErrorIf(1,NERROR_INVALID_CASE);
            break;
	}
}
*/