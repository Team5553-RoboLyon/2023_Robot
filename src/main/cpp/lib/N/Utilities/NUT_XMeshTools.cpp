#include "lib/N/NCStandard.h"
#include "../Containers/NArray.h"
#include "../Geometry/NGeometryMisc.h"
#include "../Maths/NMatrix3x3.h"
#include "../Utilities/NUT_Geometry.h"
#include "lib/N/NErrorHandling.h"
#include "NUT_XMesh.h"

// ------------------------------------------------------------------------------------------
// void NUT_BuildXTriTexCoords_FaceMap
// ------------------------------------------------------------------------------------------
// Description :
//	Build FaceMap TexCoord for an XTri without any consideration of it's belonging (XQUAD)
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BuildXTriTexCoords_FaceMap(const NUT_XTRI *pxtri, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *ptri_3_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
	{
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[0]) + offset_to_textcoord))->x = ptri_3_texcoords[0].x;
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[1]) + offset_to_textcoord))->x = ptri_3_texcoords[1].x;
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[2]) + offset_to_textcoord))->x = ptri_3_texcoords[2].x;
	}
	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
	{
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[0]) + offset_to_textcoord))->y = ptri_3_texcoords[0].y;
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[1]) + offset_to_textcoord))->y = ptri_3_texcoords[1].y;
		((NTEXCOORD_2f *)(((NBYTE *)pxtri->pPoint[2]) + offset_to_textcoord))->y = ptri_3_texcoords[2].y;
	}
	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
	{
		((NTEXCOORD_3f *)(((NBYTE *)pxtri->pPoint[0]) + offset_to_textcoord))->z = ptri_3_texcoords[0].z;
		((NTEXCOORD_3f *)(((NBYTE *)pxtri->pPoint[1]) + offset_to_textcoord))->z = ptri_3_texcoords[1].z;
		((NTEXCOORD_3f *)(((NBYTE *)pxtri->pPoint[2]) + offset_to_textcoord))->z = ptri_3_texcoords[2].z;
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap
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
void NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap(const NUT_XTRI_ARRAY *parrayofxtri, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *ptri_3_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	NErrorIf(!parrayofxtri || !ptri_3_texcoords, NERROR_NULL_POINTER);
	NErrorIf((flags_nut_texcoord_builder & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW) == 0, NERROR_INCONSISTENT_FLAGS);

	NUT_XTRI *pxtri = (NUT_XTRI *)parrayofxtri->XTriArray.pFirst;
	for (Nu32 i = parrayofxtri->XTriArray.Size; i != 0; i--, pxtri++)
	{
		if (ISFLAG_OFF(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
		{
			NErrorIf(pxtri->pXQuad, NERROR_XMESH_INCONSISTENCY);
			NUT_BuildXTriTexCoords_FaceMap(pxtri, offset_to_textcoord, ptri_3_texcoords, flags_nut_texcoord_builder);
		}
		else
		{
			NErrorIf(!pxtri->pXQuad, NERROR_XMESH_INCONSISTENCY);
		}
	}
}
// ------------------------------------------------------------------------------------------
// void NUT_BuildXQuadTexCoords_FaceMap
// ------------------------------------------------------------------------------------------
// Description :
//	Build FaceMap TexCoord for an XQuad without any consideration of it's belonging (XArea,XRPoly)
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BuildXQuadTexCoords_FaceMap(const NUT_XQUAD *pxquad, const Nu8 offset_to_textcoord_2f, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	Nu32 orientation0, orientation;
	Nu32 edge;

	// retrieve orientation of the Next of Diagonal Edge of Xtri0
	orientation0 = NUT_GetXQuadOrientation(pxquad);

	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
	{
		orientation = orientation0;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[0]->QuadDiagonal);

		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->x = pquad_4_texcoords[orientation].x;
		edge = NGET_NEXT_XTRI_EDGE(edge);

		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->x = pquad_4_texcoords[orientation].x;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[1]->QuadDiagonal);

		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->x = pquad_4_texcoords[orientation].x;
		edge = NGET_NEXT_XTRI_EDGE(edge);

		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->x = pquad_4_texcoords[orientation].x;
	}
	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
	{
		orientation = orientation0;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[0]->QuadDiagonal);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->y = pquad_4_texcoords[orientation].y;
		edge = NGET_NEXT_XTRI_EDGE(edge);

		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->y = pquad_4_texcoords[orientation].y;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[1]->QuadDiagonal);
		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->y = pquad_4_texcoords[orientation].y;
		edge = NGET_NEXT_XTRI_EDGE(edge);
		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_2f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->y = pquad_4_texcoords[orientation].y;
	}
	if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
	{
		orientation = orientation0;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[0]->QuadDiagonal);
		((NTEXCOORD_3f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->z = pquad_4_texcoords[orientation].z;
		edge = NGET_NEXT_XTRI_EDGE(edge);
		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_3f *)(((NBYTE *)pxquad->pXTri[0]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->z = pquad_4_texcoords[orientation].z;
		edge = NGET_NEXT_XTRI_EDGE(pxquad->pXTri[1]->QuadDiagonal);
		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_3f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->z = pquad_4_texcoords[orientation].z;
		edge = NGET_NEXT_XTRI_EDGE(edge);
		orientation = NGET_NEXT_XQUAD_EDGE(orientation);
		((NTEXCOORD_3f *)(((NBYTE *)pxquad->pXTri[1]->pPoint[NGET_EDGE_P0(edge)]) + offset_to_textcoord_2f))->z = pquad_4_texcoords[orientation].z;
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_BuildXQuadArrayIsolatedXQuadTexCoords_FaceMap
// ------------------------------------------------------------------------------------------
// Description :
//	Inside an XQuad Array find all the XQuads that are not belong to an XArea and/or an XRPoly
//	and build FaceMap texCoord for each of them.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BuildXQuadArrayIsolatedXQuadTexCoords_FaceMap(const NUT_XQUAD_ARRAY *parrayofxquad, const Nu8 offset_to_textcoord_2f, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	NErrorIf(!parrayofxquad || !pquad_4_texcoords, NERROR_NULL_POINTER);
	NErrorIf((flags_nut_texcoord_builder & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW) == 0, NERROR_INCONSISTENT_FLAGS);

	NUT_XQUAD *pxquad = (NUT_XQUAD *)parrayofxquad->XQuadArray.pFirst;
	for (Nu32 i = parrayofxquad->XQuadArray.Size; i != 0; i--, pxquad++)
	{
		// An Isolated XQuad is a Quad that is not inserted into an XAREA and not inserted into an XPATCH
		// It's not suppose to happen excepted when user calls 'NUT_BuildXQuadArray' to build a xquad Array.
		// in that case, all XQuad are "isolated".
		if (FLAGS_TEST(pxquad->Flags, FLAG_NUT_XQUAD_IN_AREA | FLAG_NUT_XQUAD_IN_RPOLY, 0))
		{
			NErrorIf(pxquad->pXArea || pxquad->pXRPoly, NERROR_XMESH_INCONSISTENCY);
			NUT_BuildXQuadTexCoords_FaceMap(pxquad, offset_to_textcoord_2f, pquad_4_texcoords, flags_nut_texcoord_builder);
		}
		else
		{
			NErrorIf(!pxquad->pXArea && !pxquad->pXRPoly, NERROR_XMESH_INCONSISTENCY);
			NErrorIf(!pxquad->pXArea && pxquad->pXRPoly, NERROR_XMESH_INCONSISTENCY);
		}
	}
}
// ------------------------------------------------------------------------------------------
// Nu32 NUT_BuildXRPolyTexCoords_FaceMap
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//			const NUT_XRPOLY	*pxrpoly					A valid pointer on a NUT_XRPOLY structure build in the right way
//
//			const Nu8			offset_to_textcoord_2f		NUT_XRPOLY can retrieve associated vertex pointers but it doesn't know
//															the relative offset of the texture coordinates to fill in. This parameter
//															does this job.
//
//			const NTEXCOORD_2f  *pquad_4_texcoords			FaceMaping uses only 4 different texture coordinates, one for each corner
//															of the texture rectangle mapped.
//															! IMPORTANT !
//															These 4 Texture Coordinates are suppose to be in that specific order:
//																		0... TexCoord of the Right-Bottom corner( == P0 of XQUAD EAST Edge)
//																		1... TexCoord of the Right-Top corner	( == P0 of XQUAD NORTH Edge)
//																		2... TexCoord of the Left-Top corner	( == P0 of XQUAD WEST Edge)
//																		3... TexCoord of the Left-Bottom corner ( == P0 of XQUAD SOUTH Edge)
//
// Out :
//			Nothing
// ------------------------------------------------------------------------------------------
void NUT_BuildXRPolyTexCoords_FaceMap(const NUT_XRPOLY *pxrpoly, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	if (!pxrpoly->XQuadPtrArray.Size)
		return;

	NErrorIf(!pxrpoly || !pquad_4_texcoords, NERROR_NULL_POINTER);
	NErrorIf((flags_nut_texcoord_builder & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW) == 0, NERROR_INCONSISTENT_FLAGS);

	// The 4 quad-corner Texture Coordinates
	//					N
	//	2(0,0)---------------------1(1,0)							2 --- 1 --- 2 --- 1 --- 2 --
	//		| .						|								|	  |		|	  |		|
	//		|     .					|								|	  |		|	  |		|
	//	West|	      .				|East		AND the spread ...	3 --- 0 --- 3 --- 0 --- 3 --
	//		|				.		|								|	  |		|	  |		|
	//		|					.	|								|	  |		|	  |		|
	//	3(0,1)---------------------0(1,1)							2 --- 1 --- 2 --- 1 --- 2 --
	//					S											|	  |		|	  |		|
	Nu32 orientation;
	NUT_XQUADNODE *pxqnode;
	NUT_XQUADNODE xqnode;

	NUT_XTRI *pxtri;
	NUT_XQUAD *pxquad;
	NUT_XQUAD **ppxquad;
	Nu32 i;
	Nu32 xquadedge, xtriedge;
	NARRAY xquadnode_queue;
	NVEC2s32 parentrelativegridcoords[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	NUT_XTRI *src_pxtri, *dst_pxtri;
	Nu32 src_xtriedge, dst_xtriedge;
	NTEXCOORD_3f *psrc_texcoord_3f, *pdst_texcoord_3f;

	ppxquad = (NUT_XQUAD **)pxrpoly->XQuadPtrArray.pFirst;
	for (i = pxrpoly->XQuadPtrArray.Size; i != 0; i--, ppxquad++)
	{
		pxquad = *ppxquad;

		// Check some flags first
		NErrorIf(ISFLAG_OFF(pxquad->Flags, FLAG_NUT_XQUAD_IN_RPOLY), NERROR_XQUAD_NOT_IN_RPOLY);

		// Process Flags checks
		NErrorIf(ISFLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK), NERROR_XQUAD_ALREADY_LOCKED);
		NErrorIf(ISFLAG_ON(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK), NERROR_XTRI_ALREADY_LOCKED);
		NErrorIf(ISFLAG_ON(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK), NERROR_XTRI_ALREADY_LOCKED);

		// Process Flags Set ...
		FLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK);
		FLAG_ON(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);
		FLAG_ON(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + RESET XQUAD XBuild Temporary Data
	// +
	// +
	// +-+	This function is going to use XBuild temporary data of XQuads, so Reset them !
	// +
	NUT_ResetXRPolyXQuadXBuildData((NUT_XRPOLY *)pxrpoly);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + TEMPORARY BUFFER(s) SETUP
	// +
	// +
	// +-+	This function is going to use a temporary NArray for an explicitly queue-based implementation of the propagation
	// +	recursive process.
	// +
	NSetupArray(&xquadnode_queue, pxrpoly->XQuadPtrArray.Size, sizeof(NUT_XQUADNODE));

	orientation = NUT_GetXQuadOrientation(*(NUT_XQUAD **)pxrpoly->XQuadPtrArray.pFirst);

	// Specific Boot for the XQuadNode Queue
	// notice that 'xqnode.XQuadEdge' is setup thanks to the "NUT_GetXQuadOrientation" function.
	pxquad = *(NUT_XQUAD **)pxrpoly->XQuadPtrArray.pFirst;
	Nmem0(&xqnode, NUT_XQUADNODE);
	xqnode.pXTri = pxquad->pXTri[0];
	xqnode.XTriEdge = NGET_NEXT_XTRI_EDGE(xqnode.pXTri->QuadDiagonal);
	xqnode.XQuadEdge = (Nu8)NUT_GetXQuadOrientation(*(NUT_XQUAD **)pxrpoly->XQuadPtrArray.pFirst);
	xqnode.XGridCoords.x = 0;
	xqnode.XGridCoords.y = 0;
	NArrayPushBack(&xquadnode_queue, (NBYTE *)&xqnode);

	// Build the first XQuad TexCoords .. (the 4)
	xquadedge = xqnode.XQuadEdge;
	xtriedge = xqnode.XTriEdge;
	pxtri = xqnode.pXTri;
	for (i = 4; i != 0; i--)
	{
		pdst_texcoord_3f = (NTEXCOORD_3f *)(((NBYTE *)pxtri->pPoint[NGET_EDGE_P0(xtriedge)]) + offset_to_textcoord);
		if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
			pdst_texcoord_3f->x = pquad_4_texcoords[xquadedge].x;
		if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
			pdst_texcoord_3f->y = pquad_4_texcoords[xquadedge].y;
		if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
			pdst_texcoord_3f->z = pquad_4_texcoords[xquadedge].z;
		// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
		xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
		if (NGET_NEXT_XTRI_EDGE(xtriedge) == pxtri->QuadDiagonal)
		{
			pxtri = pxtri->pAssociatedXtri;
			xtriedge = NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal);
		}
		else
		{
			xtriedge = NGET_NEXT_XTRI_EDGE(xtriedge);
		}
	}

	// Build all the other XRPoly XQuads TexCoords ...
	while (xquadnode_queue.Size)
	{
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + GET THE FIRST XQUADNODE OF THE QUEUE AND TRY INSERT
		// + XQUAD INTO THE CURRENT OPEN PATCH
		// +
		// +-+ Get a pointer of the first element of the Queue
		// +
		pxqnode = (NUT_XQUADNODE *)xquadnode_queue.pFirst;
		// +
		// + DEBUG
		if (NUT_XQUAD_XBUILDINFOS_IS_GRIDCOORDS_COMPUTED(pxqnode->pXTri->pXQuad->XBuildInfos))
		{
			if ((pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) || (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x))
			{
				// ! GRID COORDS ERROR !
				// Incoming grid coordinates don't match with the ones that are already calculated and store into this XQuad...
				// This is the result of a non uniform XQuads repartition... AND IT'S NOT SUPPOSE TO HAPPEN HERE !!!
				// A patch is consistent group of XQuads, all together consistent against a same Grid... so there is a BUG HERE !
				NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				NClearArray(&xquadnode_queue, NULL);
				return;
			}
			// else
			// {
			//		No Error: just continue... Nothing more to do
			// }
		}
		// else
		//{
		//  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//  + DEFINE XQUAD TEXCOORDS
		//  +
		//  +-+	It should be here but for a quicker/smarter implementation,
		//  +	it will be done just few lines below with the 4 neighbor propagation loop.
		//}

		// ++++++++++++++++++++++++++++++++++++++++++++++++
		// + Propagate XQuad to its 4 neighbors
		// +
		// +-+	Propagation will start from XQuad Edge 0 and progress to the 3 others, one after one :0,1,2 and 3.
		//		Notice that a new xqnode is created an push into the Queue only if the XQuad edge to pass trough is OPEN ...
		//		... That means only if this edge is not flagged as a CLOSE Edge.
		//		Why ? : Because this flag says that the current XQUAD was already evaluated ( or asked for evaluation )
		//				from the Neighbor XQuad in that direction (trough this edge) so, it will be useless to do the backward evaluation
		//				by asking evaluation from current XQuad to this neighbor.
		xquadedge = pxqnode->XQuadEdge;
		xtriedge = pxqnode->XTriEdge;
		pxtri = pxqnode->pXTri;

		for (i = 4; i != 0; i--)
		{
			// If XQuadEdge of the CURRENT 'XQUADNODE.XQUAD' is OPEN ... Propagate trough it AND BUILD TEXCCORDS of NEXT XQUAD.
			if (!NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(pxtri->pXQuad->XBuildInfos, xquadedge))
			{
				// PROPAGATION ...
				// Trough this edge
				xqnode.pXTri = pxtri->pAdjXTri[xtriedge];
				if (!xqnode.pXTri)
				{
					NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos, xquadedge);
				}
				else
				{
					// Only the XQuads from 'pxrpoly' are going to be used.
					if (xqnode.pXTri->pXQuad && (xqnode.pXTri->pXQuad->pXRPoly == pxrpoly))
					{
						xqnode.XTriEdge = pxtri->AdjEdge[xtriedge];
						xqnode.XQuadEdge = NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
						xqnode.XGridCoords.x = pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
						xqnode.XGridCoords.y = pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
						// Close the Edge of the NEW XQUADNODE to avoid going back ! (smart !!!)
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos, xqnode.XQuadEdge);
						NArrayPushBack(&xquadnode_queue, (NBYTE *)&xqnode);

						// COMPUTE TEXCOORD HERE !!! Because it's simpler !!!
						// Current XQUAD EDGE is the same than the NEXT XQUAD 'opposite' EDGE (current XQUAD EAST == next XQUAD WEST )
						// And this Edge will be the symmetry axis for NEXT XQUAD 'opposite' of 'opposite' Edge.
						// That means, we are going to compute next XQUAD EAST ! ( ... which is the opposite Edge of next XQUAD WEST )
						// And that means, this next XQUAD EAST is going to be a simple copy ( in term of texture coordinates) of
						// current XQUAD WEST ( remember , symmetry with current XQUAD EAST as symmetry axis !!! )
						//	For example:
						//		First XQUAD 'A' EAST EDGE is the symmetry axis for NEXT XQUAD B EAST EDGE...
						//	2 --- 1 --- 2 --- 1 --- 2 --
						//	|  A  *	 B	|	  |		|
						//	|	  *		|	  |		|
						//	3 --- 0 --- 3 --- 0 --- 3 --
						//	'xquadedge' is the symmetry axis. So, Current xquad opposite edge of 'xquadedge' is going to be the "edge"
						//	to copy/paste as next XQUAD opposite of opposite.
						//
						// So, retrieve the associated xtriedge of the current xquad opposite edge of xquadedge.
						src_pxtri = pxtri->pAssociatedXtri;
						if (xtriedge == (Nu32)NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal))
						{
							src_xtriedge = (Nu32)NGET_NEXT_XTRI_EDGE(src_pxtri->QuadDiagonal);
						}
						else
						{
							src_xtriedge = NGET_PREV_XTRI_EDGE(src_pxtri->QuadDiagonal);
						}
						// And retrieve the associated xtriedge of the next xquad opposite of opposite
						dst_pxtri = xqnode.pXTri->pAssociatedXtri;
						if (xqnode.XTriEdge == NGET_NEXT_XTRI_EDGE(xqnode.pXTri->QuadDiagonal))
						{
							dst_xtriedge = NGET_NEXT_XTRI_EDGE(dst_pxtri->QuadDiagonal);
						}
						else
						{
							dst_xtriedge = NGET_PREV_XTRI_EDGE(dst_pxtri->QuadDiagonal);
						}

						// Setup TexCoord of This Edge P0.
						// Notice "pxtri->pPoint[NGET_EDGE_P0(xtriedge)]" is originaly a vertex position, but also vertex address itself  because
						// vertex position is suppose to be always the first member of any kind of NVERTEX Structure.
						pdst_texcoord_3f = (NTEXCOORD_3f *)(((NBYTE *)dst_pxtri->pPoint[NGET_EDGE_P0(dst_xtriedge)]) + offset_to_textcoord);
						psrc_texcoord_3f = (NTEXCOORD_3f *)(((NBYTE *)src_pxtri->pPoint[NGET_EDGE_P1(src_xtriedge)]) + offset_to_textcoord);
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
							pdst_texcoord_3f->x = psrc_texcoord_3f->x;
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
							pdst_texcoord_3f->y = psrc_texcoord_3f->y;
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
							pdst_texcoord_3f->z = psrc_texcoord_3f->z;
						// Setup TexCoord of This Edge P1.
						// Notice "pxtri->pPoint[NGET_EDGE_P1(xtriedge)]" is originaly a vertex position, but also vertex address itself  because
						// vertex position is suppose to be always the first member of any kind of NVERTEX Structure.
						pdst_texcoord_3f = (NTEXCOORD_3f *)(((NBYTE *)dst_pxtri->pPoint[NGET_EDGE_P1(dst_xtriedge)]) + offset_to_textcoord);
						psrc_texcoord_3f = (NTEXCOORD_3f *)(((NBYTE *)src_pxtri->pPoint[NGET_EDGE_P0(src_xtriedge)]) + offset_to_textcoord);
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
							pdst_texcoord_3f->x = psrc_texcoord_3f->x;
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
							pdst_texcoord_3f->y = psrc_texcoord_3f->y;
						if (ISFLAG_ON(flags_nut_texcoord_builder, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
							pdst_texcoord_3f->z = psrc_texcoord_3f->z;
					}
					else // Trough this Current XQuad Edge, there is no NEXT XQUAD ( there is one but not from 'pxrpoly')
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos, xquadedge);
					}
				}
			}

			// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
			xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
			if (NGET_NEXT_XTRI_EDGE(xtriedge) == pxtri->QuadDiagonal)
			{
				pxtri = pxtri->pAssociatedXtri;
				xtriedge = NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal);
			}
			else
			{
				xtriedge = NGET_NEXT_XTRI_EDGE(xtriedge);
			}
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++
		// + Remove pxqnode from the Queue ( it is the first one of the list... )
		// + Because of the possible memory reallocation we prefer using it's index (0) instead of 'pxqnode' pointer !
		// +
		NEraseArrayElement(&xquadnode_queue, 0, NULL);

	} // while( xquadnode_queue.Size )

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BUFFERS CLEAR
	// +
	// +
	// +-+	Clear all the temporary buffers:
	// +			xquadnode_queue
	// +
	NClearArray(&xquadnode_queue, NULL);
}

void NUT_BuildXRPolyArrayTexCoords_FaceMap(const NUT_XRPOLY_ARRAY *pxrpoly_array, const Nu8 offset_to_textcoord, const NTEXCOORD_3f *pquad_4_texcoords, const Nu32 flags_nut_texcoord_builder)
{
	NUT_XRPOLY *pxrpoly = (NUT_XRPOLY *)pxrpoly_array->XRPolyArray.pFirst;
	for (Nu32 n = pxrpoly_array->XRPolyArray.Size; n != 0; n--, pxrpoly++)
	{
		NUT_BuildXRPolyTexCoords_FaceMap(pxrpoly, offset_to_textcoord, pquad_4_texcoords, flags_nut_texcoord_builder);
	}
}