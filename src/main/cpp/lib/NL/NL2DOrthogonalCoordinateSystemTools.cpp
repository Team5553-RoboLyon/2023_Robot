
#include "../N/UI/NUI.h"
#include "../N/Event/NEventTouch.h"
#include "NL2DOrthogonalCoordinateSystemToolData.h"
#include "NL2DOrthogonalCoordinateSystemTools.h"

NEDITOR_RESULT NL2DOCS_FreeTransform(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool				bmoved = NFALSE;
	static	NVEC2				stored_ocs_unit = { 0 };
	static	NVEC2				stored_ocs_size = { 0 };

	static	Nu32				touch_id = 0;

	NL2DOCS_TOOLDATA			*pdata = (NL2DOCS_TOOLDATA*)tool_param;
	Nu32						i;
	NL2DOCS						**ppocs;
	NL2DOCS						*pocs;
	Nu32						handleid;
	switch (pevent->Type)
	{
	case NTOUCH_START:
		bmoved = NFALSE;
		NResetToolSelection(pselect);

		ppocs = (NL2DOCS**)pdata->m_ArrayOfOcsPtr.pFirst;
		for (i = 0; i < pdata->m_ArrayOfOcsPtr.Size; i++, ppocs++)
		{
			handleid = NUT_PickXYGizmoRectf32Handle(&pdata->m_Gizmo, (*ppocs)->getBoundingBox(), NGetEventTouchPositionPtr(pevent, 0));
			if (NUT_IsXYGizmoRectf32HandleOrRectPicked(handleid))
			{
				pselect->pSelected = (void*)*ppocs;
				if (!NUT_IsXYGizmoRectf32RectPickedOnly(handleid))
					break; // continue to loop if none of the 8 possible handle is picked ( pick in rect only ) but STOP looping if one of those handle is picked.
			}
		}

		if(pselect->pSelected)
			return NEDITOR_RESULT_TOOL_ON;
		break;

	case NTOUCH_MOVE:
		if (pselect->pSelected)
		{
			if (NUT_UpdateXYGizmoRectf32(&pdata->m_Gizmo, NGetEventTouchPositionPtr(pevent, 0)))
			{
				pocs = (NL2DOCS*)pselect->pSelected;
				pocs->setBoundingBoxKeepUnitCount(&pdata->m_Gizmo.UpdatedRect);
				return NEDITOR_RESULT_TOOL_ON;
			}
		}
		break;
		
	case NTOUCH_END:
		if (pselect->pSelected)
		{
			if (NUT_EndXYGizmoRectf32(&pdata->m_Gizmo, NGetEventTouchPositionPtr(pevent, 0)))
			{
				pocs = (NL2DOCS*)pselect->pSelected;
				pocs->setBoundingBoxKeepUnitCount(&pdata->m_Gizmo.UpdatedRect);
			}
		}

		NUT_ResetGizmoRectf32(&pdata->m_Gizmo);

		if (pselect->pSelected && bmoved)
		{
			NResetToolSelection(pselect);
			bmoved = NFALSE;
			return NEDITOR_RESULT_TOOL_ON;
		}

		NResetToolSelection(pselect);
		bmoved = NFALSE;
		break;

	default:
		break;
	}

	return NEDITOR_RESULT_TOOL_OFF;
}
/*
NEDITOR_RESULT NL2DOCS_Scale(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool				bmoved = NFALSE;
	static	NVEC2				stored_ocs_unit = { 0 };
	static	NVEC2				stored_ocs_size = { 0 };

	static	Nu32				touch_id = 0;

	NL2DOCS_TOOLDATA			*pdata = (NL2DOCS_TOOLDATA*)tool_param;
	Nf32						dmin, f;
	Nu32						i;
	NL2DOCS						**ppocs;
	NL2DOCS						*pocs;
	NRECTf32					rect;
	Nu32						handleid;
	switch (pevent->Type)
	{
	case NTOUCH_START:
		bmoved = NFALSE;
		NResetToolSelection(pselect);

		ppocs = (NL2DOCS**)pdata->m_ArrayOfOcsPtr.pFirst;
		for (i = 0; i < pdata->m_ArrayOfOcsPtr.Size; i++, ppocs++)
		{
			handleid = NUT_PickXYGizmoRectf32Handle(&pdata->m_Gizmo, (*ppocs)->getBoundingBox(), NGetEventTouchPositionPtr(pevent, 0));
			if (NUT_IsXYGizmoRectf32HandleOrRectPicked(handleid))
			{
				pselect->pSelected = (void*)*ppocs;
				if (!NUT_IsXYGizmoRectf32RectPickedOnly(handleid))
					break; // continue to loop if none of the 8 possible handle is picked ( pick in rect only ) but STOP looping if one of those handle is picked.
			}
		}

		if (pselect->pSelected)
			return NEDITOR_RESULT_TOOL_ON;
		break;

	case NTOUCH_MOVE:
		if (pselect->pSelected)
		{
			if (NUT_UpdateXYGizmoRectf32(&pdata->m_Gizmo, NGetEventTouchPositionPtr(pevent, 0)))
			{
				pocs = (NL2DOCS*)pselect->pSelected;
				pocs->setBoundingBoxKeepUnitCount(&pdata->m_Gizmo.UpdatedRect);
				return NEDITOR_RESULT_TOOL_ON;
			}
		}
		break;

	case NTOUCH_END:
		if (pselect->pSelected)
		{
			if (NUT_EndXYGizmoRectf32(&pdata->m_Gizmo, NGetEventTouchPositionPtr(pevent, 0)))
			{
				pocs = (NL2DOCS*)pselect->pSelected;
				pocs->setBoundingBoxKeepUnitCount(&pdata->m_Gizmo.UpdatedRect);
			}
		}

		NUT_ResetGizmoRectf32(&pdata->m_Gizmo);

		if (pselect->pSelected && bmoved)
		{
			NResetToolSelection(pselect);
			bmoved = NFALSE;
			return NEDITOR_RESULT_TOOL_ON;
		}

		NResetToolSelection(pselect);
		bmoved = NFALSE;
		break;

	default:
		break;
	}

	return NEDITOR_RESULT_TOOL_OFF;
}
*/
void	NL2DOCS_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	NL2DOCS_TOOLDATA	*pdata = (NL2DOCS_TOOLDATA*)tool_param;
	NCOLOR				col		= { NCOLOR_PRESET3F_BRONZE_DEEP,1};
	NCOLOR				scol	= {NCOLOR_PRESET3F_WHITE,1 };

	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
	NUT_SetDrawConstructionPlane(_PLANE_XY);
	NUT_Draw_XYGizmoRectf32(&pdata->m_Gizmo, &col, &scol);
}

