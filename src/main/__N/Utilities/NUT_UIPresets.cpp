#include "../NCStandard.h"
#include "NUT_UIPresets.h"
#include "NUT_Shape.h"

// ------------------------------------------------------------------------------------------
// NMESH* NUT_CreateUIPickingQuad(const NUIPLACEMENT *place_desc)
// ------------------------------------------------------------------------------------------
// Description :
//	Create a simple Quad from NUIPLACEMENT data.
//	Purpose of this function is creating 'CustomGeometryPickingMesh' based on UI background
//	Placement desc, when user  doesn't want a background.
//	So in that specific case we need to create a picking mesh.
//	Note:
//		This function is a simplest version of the "Quad" NUT_SHAPE  creation.
//		Simplest ... because, no Texture, no Colors, no Uv, no options ... just a quad based
//		on the NUIPLACEMENT datas...
// ------------------------------------------------------------------------------------------
// In	:
//			const NUIPLACEMENT *place_desc
// 
// Out :
//			pointer on a valid NMESH freshly created
//
// ------------------------------------------------------------------------------------------
NMESH* NUT_CreateUIPickingQuad(const NUIPLACEMENT *place_desc)
{
	//	Vertex				
	//		0 ------- 1		
	//		| .       |		
	//		|    +    |		
	//		|       . |		
	//		3 ------- 2		
	//
	// Setup Building Matrix ( from inline function 'extract_build_matrix', but directly with 4 NVEC3 because of the simplification
	// of this version, but it follows the same logical way...)
	NVEC3	vwidth;
	NVEC3	vheight;
	NVEC3	vdepth;
	NVEC3	v0;
	NVec3Set(&vwidth	,place_desc->Size.x	,0					,0); // ---> X Axis (i) ---> pmatrix->V4[0]
	NVec3Set(&vheight,0					,place_desc->Size.y	,0); // ---> Y Axis (j) ---> pmatrix->V4[1]
	NVec3Set(&vdepth	,0					,0					,1); // ---> Z Axis (k) ---> pmatrix->V4[2]
	NGetUIPlacementShifting( &v0,place_desc);					 // ---> Origin (0) ---> pmatrix->V4[3]

	// ----------------------------------------------------------------------------------------
	// VERTEX POSITION INSERTION
	// ----------------------------------------------------------------------------------------
	NVERTEX_V3			vertex;
	NTRIANGLE			triangle;
	NMESH_CAPACITIES	capacities;
	NFillupMeshCapacities(&capacities,2,4);
	NMESH				*pmesh		= NCreateMesh( (Nu16)sizeof(NTRIANGLE),(Nu16)sizeof(NVERTEX_V3),&capacities);

	// Vertex 0
	vertex.Position_3f = v0;
	NArrayPushBack(&pmesh->VertexArray,(NBYTE*)&vertex);
	// Vertex 1
	NVec3AddTo(&vertex.Position_3f,&vwidth);
	NArrayPushBack(&pmesh->VertexArray,(NBYTE*)&vertex);
	// Vertex 2
	NVec3SubFrom(&vertex.Position_3f,&vheight);
	NArrayPushBack(&pmesh->VertexArray,(NBYTE*)&vertex);
	// Vertex 3
	NVec3SubFrom(&vertex.Position_3f,&vwidth);
	NArrayPushBack(&pmesh->VertexArray,(NBYTE*)&vertex);

	// Primitives
	triangle.i0 = 0;
	triangle.i1 = 1;
	triangle.i2 = 2;
	NArrayPushBack(&pmesh->PrimitiveArray,(NBYTE*)&triangle);
	triangle.i0 = 0;
	triangle.i1 = 2;
	triangle.i2 = 3;
	NArrayPushBack(&pmesh->PrimitiveArray,(NBYTE*)&triangle);

	return pmesh;
}
