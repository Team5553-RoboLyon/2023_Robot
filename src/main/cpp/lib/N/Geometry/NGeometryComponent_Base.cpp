#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NArray.h"
#include "../Geometry/NGeometry.h"
#include "NGeometryBoundingVolume.h"



static void _eraseallgeometrycomponentmeshcontent(NGEOMETRY_MESH *pmesh)
{
	NArrayEraseAll(&pmesh->PrimitiveArray,NULL);
	NArrayEraseAll(&pmesh->VertexArray,NULL);
	NArrayEraseAll(&pmesh->ColorArray,NULL);
}
void NEraseAllGeometry3DContent(NGEOMETRY_BASE *pgeom)
{
	switch(pgeom->GeometryID)
	{
		case NGEOMETRY_ID_BASE:
			return;
		case NGEOMETRY_ID_T0:
			_eraseallgeometrycomponentmeshcontent(&((NGEOMETRY_T1*)pgeom)->Mesh );
			return;
		case NGEOMETRY_ID_T1:
			_eraseallgeometrycomponentmeshcontent(&((NGEOMETRY_T1*)pgeom)->Mesh );
			NArrayEraseAll(&((NGEOMETRY_T1*)pgeom)->UVArray,NULL);
			break;
		case NGEOMETRY_ID_T2:
			_eraseallgeometrycomponentmeshcontent(&((NGEOMETRY_T2*)pgeom)->Mesh );
			NArrayEraseAll(&((NGEOMETRY_T2*)pgeom)->UVArray,NULL);
			break;
		case NGEOMETRY_ID_T3:
			_eraseallgeometrycomponentmeshcontent(&((NGEOMETRY_T3*)pgeom)->Mesh );
			NArrayEraseAll(&((NGEOMETRY_T3*)pgeom)->UVArray,NULL);
			break;
		case NGEOMETRY_ID_TN:
			_eraseallgeometrycomponentmeshcontent(&((NGEOMETRY_TN*)pgeom)->Mesh );
			NArrayEraseAll(&((NGEOMETRY_TN*)pgeom)->UVArray,NULL);
			break;

		default:
			NErrorIf(1,NERROR_GEOMETRY_UNKNOWN_ID);
			break;
	}
}


// ------------------------------------------------------------------------------------------
// NSetGeometryTexture
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetGeometryTexture(NGEOMETRY_BASE *pgeom, const NTEXTURE *ptexture, const Nu32 texture_unit, const Nu32 uv_unit)
{
#ifdef _DEBUG
	NGEOMETRY_RENDER_FUNCTION_ID_ENUM	id = NGetGeometryRenderFunctionID(pgeom->NRender);
	NErrorIf( texture_unit >= NGET_TEXUNITNB(NGeometry_TexUnit_UVUnit_LUT[id]), NERROR_GEOMETRY_OUTOFRANGE_TEXUNIT )
	NErrorIf( uv_unit >= NGET_UVUNITNB(NGeometry_TexUnit_UVUnit_LUT[id]), NERROR_GEOMETRY_OUTOFRANGE_UVUNIT )
#endif

	switch(pgeom->GeometryID)
	{
		case NGEOMETRY_ID_BASE:	// No Texture / No UvUnit !
			return;
		case NGEOMETRY_ID_T0:	// No Texture / No UvUnit !
			return;
		case NGEOMETRY_ID_T1:
			((NGEOMETRY_T1*)pgeom)->TextureUnit.GLTexture	= ptexture->GL_Texture;
			((NGEOMETRY_T1*)pgeom)->TextureUnit.pFirstUV	= (NUV*)NArrayGetIndexPtr( &((NGEOMETRY_T1*)pgeom)->UVArray, uv_unit*NArrayGetSize(&((NGEOMETRY_T1*)pgeom)->Mesh.VertexArray) );
			return;
		case NGEOMETRY_ID_T2:
			((NGEOMETRY_T2*)pgeom)->TextureUnit[texture_unit].GLTexture	= ptexture->GL_Texture;
			((NGEOMETRY_T2*)pgeom)->TextureUnit[texture_unit].pFirstUV	= (NUV*)NArrayGetIndexPtr( &((NGEOMETRY_T2*)pgeom)->UVArray, uv_unit*NArrayGetSize(&((NGEOMETRY_T2*)pgeom)->Mesh.VertexArray) );
			return;
		case NGEOMETRY_ID_T3:
			((NGEOMETRY_T3*)pgeom)->TextureUnit[texture_unit].GLTexture	= ptexture->GL_Texture;
			((NGEOMETRY_T3*)pgeom)->TextureUnit[texture_unit].pFirstUV	= (NUV*)NArrayGetIndexPtr( &((NGEOMETRY_T3*)pgeom)->UVArray, uv_unit*NArrayGetSize(&((NGEOMETRY_T3*)pgeom)->Mesh.VertexArray) );
			return;
		case NGEOMETRY_ID_TN:
			NTEXTUREUNIT *ptxu	= (NTEXTUREUNIT*)NArrayGetPtr(&((NGEOMETRY_TN*)pgeom)->TextureUnitArray,texture_unit);
			ptxu->GLTexture		= ptexture->GL_Texture;
			ptxu->pFirstUV		= (NUV*)NArrayGetIndexPtr( &((NGEOMETRY_TN*)pgeom)->UVArray, uv_unit*NArrayGetSize(&((NGEOMETRY_TN*)pgeom)->Mesh.VertexArray) );
			return;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNKNOWN_ID);
			return;
	}
}

// ------------------------------------------------------------------------------------------
// NCopyGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	Copy a geometry into an another one !
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NCopyGeometry(NGEOMETRY_BASE *pgeodst,const NGEOMETRY_BASE *pgeosrc)
{
	// Source and destination MUST have the same ID !!!
	NErrorIf(pgeosrc->GeometryID != pgeodst->GeometryID, NERROR_GEOMETRY_INCOMPATIBLE_TYPE);
	pgeodst->StateSetID		= pgeosrc->StateSetID;
	pgeodst->pNTexture		= pgeosrc->pNTexture;
	pgeodst->NRender		= pgeosrc->NRender;
	
	NArrayCopy(&pgeodst->VertexArray,&pgeosrc->VertexArray);
	NArrayCopy(&pgeodst->ColorArray,&pgeosrc->ColorArray);
	NArrayCopy(&pgeodst->UVArray,&pgeosrc->UVArray);
	NArrayCopy(&pgeodst->PrimitiveArray,&pgeosrc->PrimitiveArray);
}



// Geometry shape manipulations
// Be aware thats all these transformations are definitive
// GEOMETRY BUILDING TRANSFORMATIONS
// "Building ?" that means these transformations are absolute and definitive.
//	They change permanently Vertex positions !!

void NShiftGeometry(NGEOMETRY *pgeom,const NVECTOR3 *pshift)
{
	Nu32	i;
	NVERTEX *pv;

	pv = (NVERTEX*)NArrayGetPtr(&pgeom->VertexArray);
	for(i=0;i<NArrayGetSize(&pgeom->VertexArray);i++,pv++)
	{
		NVector3AddTo((NVECTOR3*)pv,pshift);
	}
}

void NShiftGeometryf(NGEOMETRY *pgeom,const Nf32 x, const Nf32 y, const Nf32 z)
{
	Nu32	i;
	NVERTEX *pv;

	pv = (NVERTEX*)NArrayGetPtr(&pgeom->VertexArray);
	for(i=0;i<NArrayGetSize(&pgeom->VertexArray);i++,pv++)
	{
		pv->x += x;
		pv->y += y;
		pv->z += z;
	}
}

void NMulGeometryVertexByMatrix4x4(NGEOMETRY *pgeom,const NMATRIX *pmatrix)
{
	NVERTEX		*pv;
	Nu32		i;

	pv = (NVERTEX*)NArrayGetPtr(&pgeom->VertexArray);
	for(i=0;i<NArrayGetSize(&pgeom->VertexArray);i++,pv++)
	{
		NMulVector3ByMatrix4x4((NVECTOR3*)pv,pmatrix,(NVECTOR3*)pv);
	}
}

void NMulGeometryVertexByMatrix3x3(NGEOMETRY *pgeom,const NMATRIX *pmatrix)
{
	NVERTEX		*pv;
	Nu32		i;

	pv = (NVERTEX*)NArrayGetPtr(&pgeom->VertexArray);
	for(i=0;i<NArrayGetSize(&pgeom->VertexArray);i++,pv++)
	{
		NMulVector3ByMatrix3x3((NVECTOR3*)pv,pmatrix,(NVECTOR3*)pv);
	}
}


// ------------------------------------------------------------------------------------------
// NResetGeometryCenterf
// ------------------------------------------------------------------------------------------
// Description :
// To replace a geometry around 0,0,0. According to the anchorx,anchory,anchorz parameters
// ... object will be anchored by this point ( which is relative to the object AABB )to be place at the 0,0,0 origin.)
// ... and all its vertex will be recalculated !
//	value for anchor x !
//		ANCHOR_LEFT		
//		ANCHOR_MIDWIDTH	
//		ANCHOR_RIGHT		
//	value for anchor y !
//		ANCHOR_TOP			
//		ANCHOR_MIDHEIGHT	
//		ANCHOR_BOTTOM		
//	value for anchor z !
//		ANCHOR_BACK		
//		ANCHOR_MIDDEPTH	
//		ANCHOR_FRONT		

// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NResetGeometryCenterf(NGEOMETRY *pgeom, const Nf32 anchorx, const Nf32 anchory, const Nf32 anchorz )
{
	NAABB		aabb;
	NVECTOR3	size;
	NVECTOR3	origin;
	Nu32		i;
	NVERTEX		*pvertex;

	NGetGeometryAABB( pgeom,&aabb );
	NGetAABBSize(&aabb,&size);
	
	// THE Origin LEFT/TOP/BACK
	origin.x = aabb.fXMin + size.x*anchorx;
	origin.y = aabb.fYMax + size.y*anchory;
	origin.z = aabb.fZMin + size.z*anchorz;
	
	pvertex = (NVERTEX*)NArrayGetPtr(&pgeom->VertexArray);
	for( i=NArrayGetSize(&pgeom->VertexArray);i!=0;i--,pvertex ++ )
	{
		pvertex->x -= origin.x;
		pvertex->y -= origin.y;
		pvertex->z -= origin.z;
	}
}

void NInitializeGeometryFunctions_LUT()
{
	// NMAKE_TEXTURE_UVUNIT( number of textures , number of uv units) 
	// Notes: 
	// Texture number still not variable for T0,T1,T2,T3 and TN, but it as always to be set ! An inconsistency with the Geometry structure will occurs an Error ... 
	// UVUnit number is 0 for GeometryBase and Geometry_T0. It may be anything for all the other Geometry structures and still a variable parameter setup here for each "Render_fct_id"
	// So it's possible to create a NGEOMETRY_T1 thanks to "NCreateGeometry_T1" with only 1 Texture and ask having 1 to n UVUnit ...

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_VOID]					= NULL; // ?
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_VOID]					= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_VOID]					= 0;

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_LINES_FLAT]			= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_LINES_FLAT]			= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_LINES_FLAT]			= 0;

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_LINES_TEX]			= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_LINES_TEX]			= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_LINES_TEX]			= NMAKE_TEXUNIT_UVUNIT(1,1);

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_FLAT]		= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_FLAT]		= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_TRIANGLES_FLAT]		= 0;

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_TEX]		= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_TEX]		= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_TRIANGLES_TEX]		= NMAKE_TEXUNIT_UVUNIT(1,1);

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_ENV]		= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_ENV]		= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_TRIANGLES_ENV]		= NMAKE_TEXUNIT_UVUNIT(2,1);

	NGeometryRenderFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_COMBINE2]	= NULL;
	NGeometryCreateFunctions_LUT[NGEOMETRY_RENDER_TRIANGLES_COMBINE2]	= NULL;
	NGeometry_TexUnit_UVUnit_LUT[NGEOMETRY_RENDER_TRIANGLES_COMBINE2]	= NMAKE_TEXUNIT_UVUNIT(2,1);


	NGeometryClearFunctions_LUT[NGEOMETRY_ID_BASE]	= ;
	NGeometryClearFunctions_LUT[NGEOMETRY_ID_T0]	= ;
	NGeometryClearFunctions_LUT[NGEOMETRY_ID_T1]	= ;
	NGeometryClearFunctions_LUT[NGEOMETRY_ID_T2]	= ;
	NGeometryClearFunctions_LUT[NGEOMETRY_ID_T3]	= ;
	NGeometryClearFunctions_LUT[NGEOMETRY_ID_TN]	= ;
}
