#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "NUI.h"
#include "NUICheckBox.h"
#include "NUIRenderableUpdate.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"

static inline void _UIUpdateBoundingBOX(NUI *pui)
{
	NUI *pparent;

	// Update Interactive Area ?
	// Notice that for a right and complete BoundingBox update, the Transformation Tree has to be fully updated.
	// So, the latests modifications (the one bellow) are going to be "on air" (visible on screen)  at the next turn.
	if ((ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_IF_THN_CHANGED) && (!NIsUISynchronizedWithTransformHNode(pui))) || ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE))
	{
		pui->BoundingBoxUpdating_Proc(pui);
		pparent = (NUI *)((NHIERARCHY_NODE *)pui)->pParent;
		if (pparent && ISFLAG_ON(pparent->Flags, FLAG_NUI_BBOX_UPDATE_FROM_CHILDREN_BBOX))
		{
			FLAG_ON(pparent->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
		}
		// Reset Flags:
		FLAG_OFF(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

		// Synchronize
		NSynchronizeUIWithTransformHNode(pui);
		NErrorIf(!NIsUISynchronizedWithTransformHNode(pui), NERROR_SYSTEM_CHECK);
	}
}

static inline void _UIAllChildrenRenderableUpdate(NUI *pui, const NTIME *ptime)
{
	NNODE *pnode;
	pnode = (NNODE *)pui->ChildrenList.pFirst;
	while (NIS_NODE_VALID(pnode, &pui->ChildrenList))
	{
		((NUI *)pnode)->pRenderable->Update_FCT(((NUI *)pnode)->pRenderable, pnode, ptime);
		pnode = (NNODE *)pnode->pNext;
	}
}

void NRenderableUpdate_UI(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{

	// Extraction
	prenderable->Extract_FCT(prenderable, powner); // return value is not tested because we have to continue, even if there is no geometries
												   // extraction here, because without geometries we can also having a text !!! OR ... Children !!!
	NTEXT *ptext = ((NUI *)powner)->pText;
	if (ptext)
	{
		NErrorIf(ptext->pRenderable->pTransformHNode != ((NUI *)powner)->pRenderable->pTransformHNode && ptext->pRenderable->pTransformHNode != ((NUI *)powner)->pUITransformHNode, NERROR_SYSTEM_DATASTRUCTURE_CORRUPTION);
		ptext->pRenderable->Update_FCT(ptext->pRenderable, ptext, ptime);
	}
	// Recursive Update
	_UIAllChildrenRenderableUpdate((NUI *)powner, ptime);
	// Bounding Box Update
	_UIUpdateBoundingBOX((NUI *)powner);

	// -----------------------------------------------------------------------------------------------------------------
	// DRAW UI BBOX and UI PickingGeometryMesh
	// -----------------------------------------------------------------------------------------------------------------

	/*
		#ifdef _DEBUG
		NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		NUT_SetDrawConstructionPlane(_PLANE_XY);
		NCOLOR	color = {NCOLOR_PRESET3F_BLUE_ELECTRIC,1};
		NUT_Draw_Rectangle(&((NUI *)powner)->BoundingBox,&color);

		if(((NUI*)powner)->pPickingGeometryMesh )
		{
			NUT_DRAW_TRIANGLES_ARRAY	triarray;
			triarray.Color = color;
			triarray.pFirstVertexPosition	= (NVEC3*)((NUI*)powner)->PickingGeometryTransformedVertexArray.pFirst;
			triarray.pTrianglesArray		= &((NUI*)powner)->pPickingGeometryMesh->PrimitiveArray;
			triarray.VertexStride			= ((NUI*)powner)->PickingGeometryTransformedVertexArray.ElementSize;
			NUT_Draw_TrianglesArray(&triarray);
		}
		#endif
	*/

	// -----------------------------------------------------------------------------------------------------------------

	/*
	#ifdef _DEBUG
		NUI *pui = (NUI*)powner;
		// UI Picking MESH
		if(pui->pPickingGeometryMesh)
		{
			NUT_DRAW	desc;
			NSetColorf(&desc.TriArray.Color,NCOLOR_PRESET3F_BLUE_ELECTRIC);
			desc.TriArray.pFirstVertexPosition	= (NVEC3*)pui->PickingGeometryTransformedVertexArray.pFirst;
			desc.TriArray.VertexStride			= pui->PickingGeometryTransformedVertexArray.ElementSize;
			desc.TriArray.pTrianglesArray	= &pui->pPickingGeometryMesh->PrimitiveArray;
			desc.TriArray.pDrawCallBack		= NUT_GetDrawCallBack(NUT_DRAW_ID_TRIARRAY);
			NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		}
		else if(pui->BoundingBoxUpdating_Proc == NUpdateUIBBox_From_TextMesh)
		{
			NUT_DRAW	desc;
			NSetColorf(&desc.TriArray.Color,NCOLOR_PRESET3F_BLUE_ELECTRIC);
			desc.TriArray.VertexStride			= pui->PickingGeometryTransformedVertexArray.ElementSize;
			desc.TriArray.pDrawCallBack			= NUT_GetDrawCallBack(NUT_DRAW_ID_TRIARRAY);
			NGEOMETRY *pgeom	= NGetFirstIncludedGeometry(pui->pText->pRenderable);
			NVEC3	*pv = (NVEC3*)pui->PickingGeometryTransformedVertexArray.pFirst;
			for(Nu32 g=NGetIncludedGeometriesNumber(pui->pText->pRenderable);g!=0;g--,pv = pv + pgeom->Mesh.VertexArray.Size,pgeom++)
			{
				desc.TriArray.pFirstVertexPosition	= pv;
				desc.TriArray.pTrianglesArray		= &pgeom->Mesh.PrimitiveArray;
				NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
			}
		}
		// NUIFAME BBOX
		NUT_DRAW	desc;
		NSetColorf(&desc.BBox.Color,NCOLOR_PRESET3F_YELLOW);
		NVec3Set(&desc.BBox.ConstructionPlane_DirectorVector,0,0,1);
		desc.BBox.BoundingBox		= pui->BoundingBox;
		desc.BBox.pDrawCallBack		= NUT_GetDrawCallBack(NUT_DRAW_ID_BOUNDINGBOX);
		NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
	#endif
	*/
}

void NRenderableUpdateWithoutExtract_UI(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{
	// No extraction
	// Recursive Update
	_UIAllChildrenRenderableUpdate((NUI *)powner, ptime);
	// Bounding Box Update
	_UIUpdateBoundingBOX((NUI *)powner);
}

void NRenderableUpdate_UI_Hidden(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{
	// void
}

void NRenderableUpdateWithoutExtract_UI_Hidden(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{
	// void
}