#ifndef __NUT_XMESH_H
#define __NUT_XMESH_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_XMesh.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../Containers/NArray.h"
#include "../Geometry/Components/NGeometryPrimitive.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// POINTs and EDGE INDEX into a triangle.
// Based on the winding order rules:
//		edge 0 is made with point 0 and 1
//		edge 1 is made with point 1 and 2
//		edge 2 is made with point 2 and 0
#define NGET_EDGE_P0(edge) (edge)				   // |
#define NGET_EDGE_P1(edge) ((edge + 1) % 3)		   // |
#define NGET_EDGE_POUT(edge) ((edge + 2) % 3)	   // |- edge is a triangle edge index (0,1 or 2)
#define NGET_NEXT_XTRI_EDGE(edge) ((edge + 1) % 3) // |
#define NGET_PREV_XTRI_EDGE(edge) ((edge + 2) % 3) // |
// EDGE INDEX into a XQuad.
#define NGET_OPPOSITE_XQUAD_EDGE(edge) ((edge + 2) % 4) // |_ edge is a quad edge id (0,1,2 or 3)
#define NGET_NEXT_XQUAD_EDGE(edge) ((edge + 1) % 4)		// |

#define NUT_XQUAD_EAST 0
#define NUT_XQUAD_NORTH 1
#define NUT_XQUAD_WEST 2
#define NUT_XQUAD_SOUTH 3

#define DEFAULT_NUT_XQUAD_QUALITY_THRESHOLD 0.5f
#define DEFAULT_NUT_XTRI_COPLANARITY_THRESHOLD 0.8660f // cos(30�), so a tolerance of 30� for coplanarity
#define DEFAULT_NUT_XMESH_XTRI_ARRAY_CAPACITY 1
#define DEFAULT_NUT_XQUAD_AREA_CAPACITY 1
#define DEFAULT_NUT_AREA_ARRAY_CAPACITY 1
#define DEFAULT_NUT_RPOLY_ARRAY_CAPACITY 1

	typedef struct NUT_XTRI NUT_XTRI;
	typedef struct NUT_XQUAD NUT_XQUAD;

	typedef struct
	{
		NARRAY XTriArray;
	} NUT_XTRI_ARRAY;

	typedef struct
	{
		NARRAY XQuadArray;
	} NUT_XQUAD_ARRAY;

	typedef struct
	{
		NARRAY XQuadPtrArray;
	} NUT_XAREA;

	typedef struct
	{
		NARRAY XAreaArray;
	} NUT_XAREA_ARRAY;

	// + NUT_XRPOLY
	// +	--> Rectilinear Polygon: A group of XQUADS that have coherent and not overlapped positions on the same 2D Grid
	typedef struct
	{
		NARRAY XQuadPtrArray;
	} NUT_XRPOLY;

	typedef struct
	{
		NARRAY XRPolyArray;
	} NUT_XRPOLY_ARRAY;

#define FLAG_NUT_XTRI_USED_BY_QUAD BIT_1						 // XTRI is referenced into an XQUAD.
#define FLAG_NUT_XTRI_PATCH_PROCESS_LOCK BIT_2					 // |
#define FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK BIT_3 // |-Volatile Flags reserved and used by specific process
#define FLAG_NUT_XTRI_RPOLY_TEXCOORD_PROCESS_LOCK BIT_4			 // |

	struct NUT_XTRI
	{
		Nu32 Flags;
		Nu8 QuadDiagonal;		   // 0,1 or 2 (or 255 if not). This XTRI makes a XQUAD trough one of its 3 edges ( which is the diagonal of the XQUAD). QuadDiagonal is the ID of this edge.
		Nu8 AdjEdge[3];			   // For each Edge,it's index into its Adjacent Triangle. ( edge 0 of this triangle is the same than edge 2 of adjacent triangle trough this edge )
		Nf32 EdgeQuality[3];	   // Quality for each of the 3 pairs [Xtri,Adjacent Xtri0],[Xtri,Adjacent Xtri1],[Xtri,Adjacent Xtri2]
		NUT_XTRI *pAdjXTri[3];	   // 3 pointers on each Adjacent XTri
		NUT_XTRI *pAssociatedXtri; // Pointer on the second Xtri of the Quad.
		NUT_XQUAD *pXQuad;		   // The XQuad that the Xtri belongs with.
		NTRIANGLE *pTri;		   // A pointer on the Primitive
		NVEC3 *pPoint[3];		   // 3 pointers on each vertex position.
		NVEC3 Normal;			   // Triangle Normal
	};

	typedef struct
	{
		NUT_XTRI *pXTri; // |_Necessary to identify an Edge of the XQuad trough it's XTriangle
		Nu8 XTriEdge;	 // |
		Nu8 XQuadEdge;	 // | The same edge but identify into the oriented XQUAD group base ...
						 //	 ... It's one of the 4 values: _XEAST,_XNORTH,_XWEST,_XSOUTH.
		Nu16 Available_Nu16;
		NVEC2s32 XGridCoords; // Precomputed XGridCoords for this XQuad.
	} NUT_XQUADNODE;

// NUT_XQUAD.GridCoordsInfos:
//
// |___________|___________|
// |07|06|05|04|03|02|01|00|
// -------------------------
// |C |		   |4 BITS used|
// |O |		   |	---	   |
// |M |		   |1 for each |
// |P |		   |EDGE	   |
// |U |		   |ON	:Closed|
// |T |		   |OFF: Open  |
// |E |		   |		   |
// |D |		   |		   |
// -------------------------
#define FLAG_NUT_XQUAD_XBUILDINFOS_GRIDCOORDS_COMPUTED BIT_7

#define NUT_XQUAD_XBUILDINFOS_GRIDCOORDS_COMPUTED(infos) ((infos) |= FLAG_NUT_XQUAD_XBUILDINFOS_GRIDCOORDS_COMPUTED)
#define NUT_XQUAD_XBUILDINFOS_IS_GRIDCOORDS_COMPUTED(infos) ((infos)&FLAG_NUT_XQUAD_XBUILDINFOS_GRIDCOORDS_COMPUTED)

#define NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(infos, quadedgeid) ((infos) |= (0x1 << (quadedgeid)))
#define NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(infos, quadedgeid) ((infos) & (0x1 << (quadedgeid)))

	/*
	#define MAKE_NUT_XQUAD_EDGE(xtri_id,xtriedge_id)			(((xtriedge_id)&0xF)|(((xtri_id)&0xF)<< 4))
	#define GET_NUT_XQUAD_EDGE_XTRIEDGE(xquad_edge)				((xquad_edge)&0xF)
	#define GET_NUT_XQUAD_EDGE_XTRI(xquad_edge)					(((xquad_edge)>>4)&0xF)
	*/

#define FLAG_NUT_XQUAD_IN_AREA BIT_0					 // XQUAD is referenced into a XArea.
#define FLAG_NUT_XQUAD_IN_RPOLY BIT_1					 // XQUAD is referenced into a XRPoly.
#define FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK BIT_2			 // |_Volatile Flags reserved and used by specific process
#define FLAG_NUT_XQUAD_RPOLY_TEXCOORD_PROCESS_LOCK BIT_3 // |
	struct NUT_XQUAD
	{
		Nu8 Flags;
		Nu8 XBuildInfos; // XBUILD temporary Data: EDGE OPEN / CLOSE and XQUAD COMPUTED or NOT. Internal Used Only. Never reusable.
		Nu16 Available_Nu16;
		NVEC2s32 XBuildGridCoords; // XBUILD temporary Data: Used to create XRPOLY. Internal Used Only. Never reusable.
		NUT_XTRI *pXTri[2];		   // The 2 X Triangles linked with.

		NUT_XAREA *pXArea;
		NUT_XRPOLY *pXRPoly;
	};

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **																			FUNCTIONS																		**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + XMESH BASICS
	// +
	// +-+ Arrays of X structures
	// +
	inline NUT_XTRI_ARRAY *NUT_SetupXTriArray(NUT_XTRI_ARRAY *pxtriarray, Nu32 capacity) { return (NUT_XTRI_ARRAY *)NSetupArray((NARRAY *)pxtriarray, capacity, sizeof(NUT_XTRI)); }
	inline NUT_XTRI_ARRAY *NUT_CreateXTriArray(Nu32 capacity) { return (NUT_XTRI_ARRAY *)NCreateArray(capacity, sizeof(NUT_XTRI)); }
	inline void NUT_ClearXTriArray(NUT_XTRI_ARRAY *pxtriarray) { NClearArray((NARRAY *)pxtriarray, NULL); }
	inline void NUT_DeleteXTriArray(NUT_XTRI_ARRAY *pxtriarray) { NDeleteArray((NARRAY *)pxtriarray, NULL); }

	inline NUT_XQUAD_ARRAY *NUT_SetupXQuadArray(NUT_XQUAD_ARRAY *pxquadarray, Nu32 capacity) { return (NUT_XQUAD_ARRAY *)NSetupArray((NARRAY *)pxquadarray, capacity, sizeof(NUT_XQUAD)); }
	inline NUT_XQUAD_ARRAY *NUT_CreateXQuadArray(Nu32 capacity) { return (NUT_XQUAD_ARRAY *)NCreateArray(capacity, sizeof(NUT_XQUAD)); }
	inline void NUT_ClearXQuadArray(NUT_XQUAD_ARRAY *pxquadarray) { NClearArray((NARRAY *)pxquadarray, NULL); }
	inline void NUT_DeleteXQuadArray(NUT_XQUAD_ARRAY *pxquadarray) { NDeleteArray((NARRAY *)pxquadarray, NULL); }
	// +
	// +-+ Arrays of X structures pointers
	// +
	inline NUT_XAREA *NUT_SetupXArea(NUT_XAREA *pxarea, Nu32 capacity) { return (NUT_XAREA *)NSetupArray((NARRAY *)pxarea, capacity, sizeof(NUT_XQUAD *)); }
	inline NUT_XAREA *NUT_CreateXArea(Nu32 capacity) { return (NUT_XAREA *)NCreateArray(capacity, sizeof(NUT_XQUAD *)); }
	void NUT_ClearXArea(NUT_XAREA *pxarea);
	void NUT_DeleteXArea(NUT_XAREA *pxarea);

	inline NUT_XAREA_ARRAY *NUT_SetupXAreaArray(NUT_XAREA_ARRAY *pxareaarray, Nu32 capacity) { return (NUT_XAREA_ARRAY *)NSetupArray((NARRAY *)pxareaarray, capacity, sizeof(NARRAY)); }
	inline NUT_XAREA_ARRAY *NUT_CreateXAreaArray(Nu32 capacity) { return (NUT_XAREA_ARRAY *)NCreateArray(capacity, sizeof(NARRAY)); }
	void NUT_ClearXAreaArray(NUT_XAREA_ARRAY *pxareaarray);
	void NUT_DeleteXAreaArray(NUT_XAREA_ARRAY *pxareaarray);

	inline NUT_XRPOLY *NUT_SetupXRPoly(NUT_XRPOLY *pxrpoly, Nu32 capacity) { return (NUT_XRPOLY *)NSetupArray((NARRAY *)pxrpoly, capacity, sizeof(NUT_XQUAD *)); }
	inline NUT_XRPOLY *NUT_CreateXRPoly(Nu32 capacity) { return (NUT_XRPOLY *)NCreateArray(capacity, sizeof(NUT_XQUAD *)); }
	void NUT_ClearXRPoly(NUT_XRPOLY *pxrpoly);
	void NUT_DeleteXRPoly(NUT_XRPOLY *pxrpoly);

	inline NUT_XRPOLY_ARRAY *NUT_SetupXRPolyArray(NUT_XRPOLY_ARRAY *pxrpolyarray, Nu32 capacity) { return (NUT_XRPOLY_ARRAY *)NSetupArray((NARRAY *)pxrpolyarray, capacity, sizeof(NARRAY)); }
	inline NUT_XRPOLY_ARRAY *NUT_CreateXRPolyArray(Nu32 capacity) { return (NUT_XRPOLY_ARRAY *)NCreateArray(capacity, sizeof(NARRAY)); }
	void NUT_ClearXRPolyArray(NUT_XRPOLY_ARRAY *pxrpolyarray);
	void NUT_DeleteXRPolyArray(NUT_XRPOLY_ARRAY *pxrpolyarray);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + XMESH BUILD Functions
// +
// +
// +-+ The flag "FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2" may be used with the followings:
// +
// +			NUT_UpdateXtriAdjacents
// +			NUT_BuildXQuadArray
// +			NUT_BuildXAreaArray
// +			NUT_BuildGeometryXMesh
// +
#define FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2 BIT_0 // There are 2 adjacent NGEOMETRY triangles per NGEOMETRY triangle at maximum.
	// +
	// +-+ Functions XTri
	NUT_XTRI_ARRAY *NUT_BuildXTriArray(NUT_XTRI_ARRAY *pout_xtriarray, const NARRAY *ptriangle_array, const Nu32 triangle_range_first_index, const Nu32 triangle_range_size, const NARRAY *pvertex_array, const Nu32 vertex_range_first_index, const Nu32 vertex_range_size);
	NUT_XTRI *NUT_UpdateXtriAdjacents(NUT_XTRI_ARRAY *pxtriarray, const Nf32 cosphi_min, const Nu32 flags);
	// +
	// +-+ Functions XQuad
	// +
	NVEC3 *NUT_ComputeXQuadNormal(NVEC3 *pnormal, const NUT_XQUAD *pxquad);
	Nu32 NUT_GetXQuadOrientation(const NUT_XQUAD *px);
	void NUT_ResetXQuadArrayXBuildData(NARRAY *parrayofxquadptr);
	// (NUT_BuildXQuadArray is not used by XMesh Pipeline itself. XRPolys are made from XArea and NOT from XQuadArray. But if user just want to get an Array of XQuads, and just that... thats the function! )
	NUT_XQUAD_ARRAY *NUT_BuildXQuadArray(NUT_XQUAD_ARRAY *pout_xquadarray, const NUT_XTRI_ARRAY *pxtriarray, const Nf32 xquad_quality, const Nf32 cosphi_min, const Nu32 flags);

	// +
	// +-+ Functions XArea
	// +
	// (NUT_BuildXAreaArray create an array of xarea AND an array of XQuads. Actually, XArea is an Array of XQuad pointers, and these XQuads are located into the XQuad array. This XQuads Array was build on the fly along XAreas building process. )
	NUT_XAREA_ARRAY *NUT_BuildXAreaArray(NUT_XAREA_ARRAY *pout_xareaarray, NUT_XQUAD_ARRAY *pout_xquadarray, const NUT_XTRI_ARRAY *pxtriarray, const Nf32 xquad_quality, const Nf32 cosphi_min, const Nu32 flags);
	inline void NUT_ResetXAreaXQuadXBuildData(NUT_XAREA *pxarea) { NUT_ResetXQuadArrayXBuildData(&pxarea->XQuadPtrArray); }
	void NUT_ReAddressing_XArea_Array(NUT_XAREA_ARRAY *pxquadareaarray);

	// +
	// +-+ Functions XRPoly
	// +
	void NUT_BuildXRPolyArrayFromXArea(NUT_XRPOLY_ARRAY *pout_xrpolyarray, const NUT_XAREA *pxarea, const NUT_XQUAD_ARRAY *pxquadarray, const NUT_XTRI_ARRAY *pxtriarray);
	void NUT_BuildXRPolyArrayFromXAreaArray(NUT_XRPOLY_ARRAY *pout_xrpolyarray, const NUT_XAREA_ARRAY *pxarea_array, const NUT_XQUAD_ARRAY *pxquadarray, const NUT_XTRI_ARRAY *pxtriarray);
	inline void NUT_ResetXRPolyXQuadXBuildData(NUT_XRPOLY *pxrpoly) { NUT_ResetXQuadArrayXBuildData(&pxrpoly->XQuadPtrArray); }
	Nu32 NUT_GetXRPolyCommonEdgesCount(const NUT_XRPOLY *pxrpoly1, const NUT_XRPOLY *pxrpoly2);
	void NUT_ReAddressing_XRPoly_Array(NUT_XRPOLY_ARRAY *pxrpolyarray);
	// +
	// +-+ "smart" Function(s)
	// +
	inline void NUT_BuildGeometryXMesh(NUT_XTRI_ARRAY *pout_xtriarray, NUT_XQUAD_ARRAY *pout_xquadarray, NUT_XAREA_ARRAY *pout_xareaarray, NUT_XRPOLY_ARRAY *pout_xrpolyarray, const NGEOMETRY *pgeometry, const Nf32 xquad_quality, const Nf32 cosphi_min, const Nu32 flags)
	{
		NUT_BuildXTriArray(pout_xtriarray, &pgeometry->Mesh.PrimitiveArray, 0, pgeometry->Mesh.PrimitiveArray.Size, &pgeometry->Mesh.VertexArray, 0, pgeometry->Mesh.VertexArray.Size);
		NUT_BuildXAreaArray(pout_xareaarray, pout_xquadarray, pout_xtriarray, xquad_quality, cosphi_min, flags);
		NUT_BuildXRPolyArrayFromXAreaArray(pout_xrpolyarray, pout_xareaarray, pout_xquadarray, pout_xtriarray);
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + XMESH TOOLS
	// +
	// +	XMesh give to user a smart point of view on a NGEOMETRY. So trough XMESH Data it's possible to perform some high level actions.
	// +	Like QUAD Face mapping (remember: there are NO QUADs inside NGEOMETRY, but XMESH functions are able to recognize them into
	// +	a "Triangles Soup")
	// +-+
	void NUT_BuildXTriTexCoords_FaceMap(const NUT_XTRI *pxtri, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *ptri_3_texcoords, const Nu32 flags_nut_texcoord_builder);
	void NUT_BuildXQuadTexCoords_FaceMap(const NUT_XQUAD *pxquad, const Nu8 offset_to_textcoord_2f, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder);

	void NUT_BuildXRPolyTexCoords_FaceMap(const NUT_XRPOLY *pxrpoly, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder);
	void NUT_BuildXRPolyArrayTexCoords_FaceMap(const NUT_XRPOLY_ARRAY *pxrpoly_array, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder);
	void NUT_BuildXQuadArrayIsolatedXQuadTexCoords_FaceMap(const NUT_XQUAD_ARRAY *parrayofxquad, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder);
	void NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap(const NUT_XTRI_ARRAY *parrayofxtri, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *ptri_3_texcoords, const Nu32 flags_nut_texcoord_builder);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_XMESH_H
