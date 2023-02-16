#include "../NCStandard.h"
#include "../Containers/NArray.h"
#include "../Maths/NMatrix3x3.h"

#include "../Geometry/NGeometryMisc.h"
#include "../NErrorHandling.h"
#include "./Maths/NUT_MathsMisc.h"
#include "NUT_XMesh.h"


// ------------------------------------------------------------------------------------------
// NUT_XTRI* NUT_BuildXTris
// ------------------------------------------------------------------------------------------
// Description :
//		Purpose of this function is just building an "X mirror"  of an existing geometry triangles list
//		An Xtriangle is going to be create and setup in the right way for each triangle of the incoming list.
// ------------------------------------------------------------------------------------------
// In	:
//			NARRAY *pout_xtriarray		a valid pointer on an Xtris Array.
//										1/	This array may income initialized by user as an XTRI Array
//											In that case new XTri are going to be ADD into the Array ( without destroying potential previous inserted XTRIs !)
//										2/	Or this array may income just completely cleared (Full of ZEROs). 
//											In that case the function will setup the array for the user with the right capacity.  	
//
//			const NARRAY *ptriangle_array				a valid pointer on the 'source' triangle array ( it should be NGEOMETRY.NMESH.PrimitiveArray )
//			const Nu32	 triangle_range_first_index		First triangle index into the Triangle Array to work with.
//			const Nu32	 triangle_range_size			number of triangles to work with
//
//			const NARRAY *pvertex_array					a valid pointer on the 'source' geometry vertex array (the one the triangle list refers to )
//			const Nu32	 vertex_range_first_index		First vertex index into the Vertex Array to work with.
//			const Nu32	 vertex_range_size				number of vertex to work with
//
//														Notice that the 'vertex_range_first_index' and 'vertex_range_size' parameters
//														and 'triangle_range_first_index' and 'triangle_range_size' parameters help user
//														to define precisely which triangle are available for use. They are working all together
//														and the process will build XTriangle only from triangles that are into the 'triangle_range' with its
//														3 vertex inside the 'vertex_range'.
// Out :
//			pout_xtriarray itself
//
// ------------------------------------------------------------------------------------------
NUT_XTRI_ARRAY* NUT_BuildXTriArray( NUT_XTRI_ARRAY *pout_xtriarray, const NARRAY *ptriangle_array, const Nu32 triangle_range_first_index, const Nu32 triangle_range_size, const NARRAY *pvertex_array, const Nu32 vertex_range_first_index, const Nu32 vertex_range_size  )
{
	NErrorIf(!pout_xtriarray, NERROR_NULL_POINTER);
	
	NErrorIf(!ptriangle_array,NERROR_NULL_POINTER);
	NErrorIf(!triangle_range_size,NERROR_NULL_VALUE);
	NErrorIf(triangle_range_first_index>=ptriangle_array->Size,NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(triangle_range_first_index+triangle_range_size>ptriangle_array->Size,NERROR_INCONSISTENT_PARAMETERS);

	NErrorIf(!pvertex_array,NERROR_NULL_POINTER);
	NErrorIf(vertex_range_size<3,NERROR_INCONSISTENT_PARAMETERS);// vertex_range_size must be > 3 ( to have at least one triangle !!)
	NErrorIf(vertex_range_first_index>=pvertex_array->Size,NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(vertex_range_first_index+vertex_range_size>pvertex_array->Size,NERROR_INCONSISTENT_PARAMETERS);

	Nu32			last_vertex_index = vertex_range_first_index + vertex_range_size - 1;
	
	Nu32			n;
	NUT_XTRI		xtri;
	NTRIANGLE		*ptri;
	
	NVEC3		v;

	// +--+	Out XTri Array Setup 
	//		Notice that this function is going to work "directly" into the Allocated Memory. Even if it's an array.
	//		It's why the array size is set to the capacity...
	//		if there is no Out Xtri array requested by user, a simple memory buffer is reserved and will be destroy
	//		at the end of this process.
	//		Both ways (array or simple memory buffer) are going to be managed in the same way for the next steps...
	//
	if(pout_xtriarray->XTriArray.ElementSize == 0)
	{
		NUT_SetupXTriArray( pout_xtriarray,triangle_range_size );
	}
	else if(pout_xtriarray->XTriArray.ElementSize == sizeof(NUT_XTRI) )
	{
		// If 'pout_xtriarray' is already setup, it has to have enough capacity to insert all the new XTriangles.
		// If not ... CRASH in debug Mode. Do nothing in Release and return.
		// It's like this because of memory reallocation. Indeed 'reallocation' may change addresses of XTriangles and it's BAD !!!
		// Because these address are used by others XMesh structure, like XQUAD ... 
		// So if user wants using the same pout_xtriarray for several successive XTriangles building, he has to setup this array by himself
		// with enough capacity right at the beginning.
		NErrorIf(pout_xtriarray->XTriArray.Size + triangle_range_size > pout_xtriarray->XTriArray.Capacity, NERROR_ARRAY_WRONG_SIZE);
		return pout_xtriarray;
	}
	else
	{
		NErrorIf(1,NERROR_ARRAY_WRONG_ELEMENT_SIZE);
		return pout_xtriarray;
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + XTRIS Setup
	// +
	// +
	Nmem0(&xtri,NUT_XTRI);

	ptri = (NTRIANGLE*)NGetArrayPtr(ptriangle_array,triangle_range_first_index);
// 	pxtri= pxtrilist;
	for(n=triangle_range_size;n!=0;n--,ptri++/*,pxtri++*/)
	{	
		// Check the 3 Triangle vertex, they must be inside the incoming vertex range
		if(ptri->i0 < vertex_range_first_index || ptri->i0 > last_vertex_index )
			continue;
		if(ptri->i1 < vertex_range_first_index || ptri->i1 > last_vertex_index )
			continue;
		if(ptri->i2 < vertex_range_first_index || ptri->i2 > last_vertex_index )
			continue;
		
		// Compute and Check Triangle Normal. An invalid normal ( NComputeTriangleNormal return 0) means triangle
		// is a degenerated one. So, we are going to ignore it ! Xtri
		xtri.pPoint[0]		= (NVEC3*)NGetArrayPtr(pvertex_array,ptri->i0);
		xtri.pPoint[1]		= (NVEC3*)NGetArrayPtr(pvertex_array,ptri->i1);
		xtri.pPoint[2]		= (NVEC3*)NGetArrayPtr(pvertex_array,ptri->i2);
		if( !NComputeTriangleNormal( &xtri.Normal,xtri.pPoint[0],xtri.pPoint[1],xtri.pPoint[2]) )
			continue;
		
		// Double check for a degenerated Triangle, Edge by Edge... 
		NVec3Sub(&v,xtri.pPoint[0],xtri.pPoint[1]);
		if(NVec3Length(&v)<=NF32_EPSILON_VECTOR_LENGTH)
			continue;
		NVec3Sub(&v,xtri.pPoint[0],xtri.pPoint[2]);
		if(NVec3Length(&v)<=NF32_EPSILON_VECTOR_LENGTH)
			continue;
		NVec3Sub(&v,xtri.pPoint[2],xtri.pPoint[1]);
		if(NVec3Length(&v)<=NF32_EPSILON_VECTOR_LENGTH)
			continue;


		// Build XTri
		xtri.Flags			= 0;
		xtri.pTri			= (NTRIANGLE*)ptri;
		
		xtri.QuadDiagonal	= 255;  // |
		xtri.pXQuad			= NULL; // |-Means none !
		xtri.pAssociatedXtri= NULL; // |

		xtri.EdgeQuality[0]	= -1.0f;// |
		xtri.EdgeQuality[1]	= -1.0f;// |-Means Worse possible Quality !
		xtri.EdgeQuality[2]	= -1.0f;// |

		xtri.AdjEdge[0]		= 255;  // |
		xtri.AdjEdge[1]		= 255;  // |-Means none !
		xtri.AdjEdge[2]		= 255;  // |
		
		xtri.pAdjXTri[0]	= NULL; // |
		xtri.pAdjXTri[1]	= NULL; // |-Means none !
		xtri.pAdjXTri[2]	= NULL; // |
		
		// XTriangles Array is suppose to have the right capacity !
		NErrorIf(NIsArrayFull(&pout_xtriarray->XTriArray), NERROR_MEMORY_UNEXPECTED_REALLOCATION_WILL_OCCUR);
		NArrayPushBack(&pout_xtriarray->XTriArray,(NBYTE*)&xtri);
	}
	return pout_xtriarray;
}

// ------------------------------------------------------------------------------------------
// void NUT_ComputeXQuadNormal
// ------------------------------------------------------------------------------------------
// Description :
//	Compute a unique normal vector for the incoming quad.
//	Its done using the normals average of the 2 triangles which belongs to the quad.
// ------------------------------------------------------------------------------------------
// In	:
//			NVEC3		*pnormal		a valid pointer on a NVEC3 where result will be stored
//			const NUT_XQUAD *pxquad			a valid pointer on the NUT_XQUAD to calculate the normal
// Out :
//		1 if the quad normal was computed in the right way
//		0 if one (or two) of the quad triangle normals is(are) null vector.	
// ------------------------------------------------------------------------------------------
NVEC3* NUT_ComputeXQuadNormal(NVEC3 *pnormal, const NUT_XQUAD *pxquad)
{
	NErrorIf(!pxquad->pXTri[0]||!pxquad->pXTri[0],NERROR_NULL_POINTER);

	NVec3Add(pnormal,&pxquad->pXTri[0]->Normal,&pxquad->pXTri[1]->Normal);
	NVec3ScaleBy(pnormal,0.5f);
	return pnormal;
}

// ------------------------------------------------------------------------------------------
// Nu32 NUT_GetXQuadOrientation
// ------------------------------------------------------------------------------------------
// Description :
//	Return an evaluation of the direction facing to the next edge of the Diagonal edge
//	of the XTri 0 of XQuad.
//	This evaluation uses the Quad Normal vector as the forward vector of a computed reference
//	Orthonormal. N uses the given Matrix3x3 to define orientation of each quad side.
//	... because triangle vertex, and so on, XQuad vertex winding order is constant (it's a rule!)
//	... the function only returns the ordinal value of "Next edge of the Diagonal edge of the XTri 0"
//	By convention this value has a name which could be:
//				0 ...	NUT_XQUAD_EAST	---> Oriented to the X+ Axis of the XQuad Reference Orthonormal.
//				1 ...	NUT_XQUAD_NORTH	---> Oriented to the Y+ Axis of the XQuad Reference Orthonormal.
//				2 ...	NUT_XQUAD_WEST	---> Oriented to the X- Axis of the XQuad Reference Orthonormal.
//				3 ...	NUT_XQUAD_SOUTH	---> Oriented to the Y- Axis of the XQuad Reference Orthonormal.
// ------------------------------------------------------------------------------------------
// In	:
//		NUT_XQUAD *px	A valid pointer on a NUT_XQUAD structure.
// 
// Out :
//	one of this 4 values:
//			NUT_XQUAD_EAST, NUT_XQUAD_NORTH,NUT_XQUAD_WEST or NUT_XQUAD_SOUTH
//			(which are 0,1,2 or 3)				
// ------------------------------------------------------------------------------------------
Nu32 NUT_GetXQuadOrientation(const NUT_XQUAD *px)
{
	NMATRIX3x3	matrix;
	NVEC3	midedge[4];
	NVEC3	v02,v13;
	NVEC3	n;
	Nf32		dotv02,dotv13;
	Nu32		edge0,edge1,edge2,edge3;
//	Nu32		tritop,edgtop,triright,edgright,tribot,edgbot,trileft,edgleft;

	// Quad Normal And Matrix from this Normal 
	NUT_ComputeXQuadNormal(&n,px);
	//NForwardVectorMatrix3x3(&matrix,&n);
	//NUT_BuildHughesMoller3DOrthonormal(&matrix,&n);
	NUT_Build3DOrthonormal(&matrix,&n,NULL,NULL);
	// 4 middle points. One for each Quad Edges.
	edge0 = NGET_NEXT_XTRI_EDGE(px->pXTri[0]->QuadDiagonal); 
	NVec3Add( &midedge[0], px->pXTri[0]->pPoint[NGET_EDGE_P1(edge0)], px->pXTri[0]->pPoint[NGET_EDGE_P0(edge0)] );
	edge1 = NGET_NEXT_XTRI_EDGE(edge0);
	NVec3Add( &midedge[1], px->pXTri[0]->pPoint[NGET_EDGE_P1(edge1)], px->pXTri[0]->pPoint[NGET_EDGE_P0(edge1)] );
	
	edge2 = NGET_NEXT_XTRI_EDGE(px->pXTri[1]->QuadDiagonal); 
	NVec3Add( &midedge[2], px->pXTri[1]->pPoint[NGET_EDGE_P1(edge2)], px->pXTri[1]->pPoint[NGET_EDGE_P0(edge2)] );
	edge3 = NGET_NEXT_XTRI_EDGE(edge2);
	NVec3Add( &midedge[3], px->pXTri[1]->pPoint[NGET_EDGE_P1(edge3)], px->pXTri[1]->pPoint[NGET_EDGE_P0(edge3)] );

	// Quad medians and dot products 
	NVec3Sub(&v02,&midedge[2],&midedge[0]);
	NVec3Sub(&v13,&midedge[3],&midedge[1]);
	dotv02 = NVec3DotProduct(&v02,&matrix.XAxis);
	dotv13 = NVec3DotProduct(&v13,&matrix.XAxis);

	// Tests and Sorting TOP,RIGHT,BOTTOM,LEFT 
	// Return only the value for "Tri0 Edge 0"
	if( NABS(dotv02)>=NABS(dotv13) )
	{
		if(dotv02>=0.0f)
		{
/*
			triright = 1;
			edgright = edge2;
			trileft = 0;
			edgleft = edge0;
*/
			return NUT_XQUAD_WEST;
		}
		else
		{
/*
			triright = 0;
			edgright = edge0;
			trileft = 1;
			edgleft = edge2;
*/
			return NUT_XQUAD_EAST;
		}
/*
		dotv13 = NVec3DotProduct(&v13,&matrix.YAxis);
		if(dotv13>=0.0f)
		{
			tritop = 1;
			edgtop = edge3;
			tribot = 0;
			edgbot = edge1;
		}
		else
		{
			tritop = 0;
			edgtop = edge1;
			tribot = 1;
			edgbot = edge3;
		}
*/
	}
	else
	{
/*
		if(dotv13>=0.0f)
		{
			triright = 1;
			edgright = edge3;
			trileft = 0;
			edgleft = edge1;
		}
		else
		{
			triright = 0;
			edgright = edge1;
			trileft = 1;
			edgleft = edge3;
		}
*/
		dotv02 = NVec3DotProduct(&v02,&matrix.YAxis);
		if(dotv02>=0.0f)
		{
/*
			tritop = 1;
			edgtop = edge2;
			tribot = 0;
			edgbot = edge0;
*/
			return NUT_XQUAD_SOUTH;
		}
		else
		{
/*
			tritop = 0;
			edgtop = edge0;
			tribot = 1;
			edgbot = edge2;
*/
			return NUT_XQUAD_NORTH;
		}
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_ResetXQuadArrayXBuildData
// ------------------------------------------------------------------------------------------
// Description :
//	Reset all the 'XBuild' member located into the NUT_XQUAD  structure.
//	They are:
//			Nu8				XBuildInfos;	
//			NVEC2s32		XBuildGridCoords;			
//
//	! NUT_XQUAD.pXRPoly is not reset by this function !
//	! NUT_XQUAD.pXArea  is not reset by this function !
//
//	Notice that this function is used BEFORE performing a recursive XQuads XBuild members ... Not after.
//	That means the function ( like XRPOLY builder ) that uses XBuild Members let them as they are 
//	with garbages inside ...
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_ResetXQuadArrayXBuildData(NARRAY *parrayofxquadptr)
{
	NErrorIf(parrayofxquadptr->ElementSize != sizeof(NUT_XQUAD*),NERROR_XMESH_INCONSISTENCY);

	NUT_XQUAD **ppxquad = (NUT_XQUAD**)parrayofxquadptr->pFirst;
	for(Nu32 i=parrayofxquadptr->Size;i!=0;i--,ppxquad++)
	{
		(*ppxquad)->XBuildInfos			= 0;
		(*ppxquad)->XBuildGridCoords.x	= 0;
		(*ppxquad)->XBuildGridCoords.y	= 0;
	}
}
