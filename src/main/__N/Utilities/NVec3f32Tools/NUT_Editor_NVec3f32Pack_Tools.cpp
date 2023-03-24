#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../NCStandard.h"
#include "../../UI/NUI.h"
#include "../../NCamera.h"
#include "../../Maths/NVec3f32.h"
#include "../../NCoordinateSystemConversions.h"
#include "../../NEditor.h"
#include "../../Utilities/NUT_X.h"
#include "../../Utilities/NUT_3DMouse.h"
#include "../../Utilities/Maths/NUT_MathsMisc.h"
#include "../../Utilities/NUT_Spline.h"

#include "../../Event/NEventTouch.h"
#include "../../Maths/NMatrix3x3.h"
#include "NUT_Editor_NVec3f32Pack_Tools.h"

static inline void _lookfor_v3(const NARRAY *parrayofelement, const Nu32 offset_to_v3, const NVEC2s16 *ppointer, NEDITOR_SELECTION *pselect, NVEC3 *phitpoint)
{
	Nu32					i;
	NBYTE					*pel;
	NVEC3f32				*pv3;
	NVEC3					mouselineA,mouselineB;
	NUT_3DLINEXLINE_RESULT	keep_result, result;
	Nf32					squared_dist,min_squared_dist;
	NVEC3					v;
	NVEC3					AB;

	// Full 3D research.
	// At this stage we just have a ray ( mouse 3D ray ) and an array of elements to look into...
	// So we are to determine the closest element to the mouse 3D ray and if this element is close enough
	// to the ray it will be the One !
	// Lets go !
	min_squared_dist = NUT_NV3PACK_DETECTION_DISTANCE_SQUARED;
	NResetToolSelection(pselect);
	NUT_Get3DMouseSegment(&mouselineA, &mouselineB, NULL, ppointer );
	pel = (NBYTE*)NGetFirstArrayPtr(parrayofelement);
	for(i=0;i<NGetArraySize(parrayofelement);i++, pel += parrayofelement->ElementSize)
	{
		pv3 = (NVEC3f32*)((pel) + offset_to_v3);
		NVec3Sub(&AB,&mouselineB,&mouselineA);
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB,&mouselineA,&AB,pv3);
		NVec3Sub(&v,pv3,&result.PointOnAB);
		squared_dist = NVec3SquareLength(&v);
		if(squared_dist<min_squared_dist)
		{
			min_squared_dist		= squared_dist;
			pselect->pSelectedOwner	= (void*)(pel);
			pselect->pSelected		= (void*)pv3;
			pselect->SelectParam	= i;
			keep_result				= result;
		}
	}

	if(pselect->pSelected)
		*phitpoint = keep_result.PointOnAB;
}

// ------------------------------------------------------------------------------------------
// NUT_NVec3f32PackTool_Move
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//		
//		NTOUCH_MOVE:	"A Point is selected and moved (position changes)."
//			CallBack is going to be called  only if there is a selected Point and
//			after moving it. 
//			The "NEDITOR_SELECTION"	structure contains the selected point informations.
//
//		NTOUCH_END:
//			CallBack is going to be called  ONLY if a real move occurred before.
//			( obviously if there is a selected Point ... )
//			Be aware that the selected point position is exactly the same than the last 'NTOUCH_MOVE' callback call.
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NUT_NVec3f32PackTool_Move(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	static	Nbool				bmoved = NFALSE;
	static	NVEC3				hitpoint ={0,0,0};
	static	Nf32				additional_tangent_portion;
	static	Nu32				touch_id = 0;

	NUT_V3PACK_TOOLDATA			*pdata;
	NVEC3						*pv3;
	NVEC3						mouseray_origin,mouseray_dirvect;
	NVEC3						planenormal,mouse_on_workplane;

	switch(pevent->Type)
	{
		case NTOUCH_START:
			bmoved = NFALSE;
			pdata = (NUT_V3PACK_TOOLDATA *)tool_param;
			_lookfor_v3(pdata->pArrayOfElements, pdata->OffsetToV3, NGetEventTouchPositionPtr(pevent,0), pselect, &hitpoint);
			if( pselect->pSelected )
				return NEDITOR_RESULT_TOOL_ON;

			break;
		case NTOUCH_MOVE:
			pdata	= (NUT_V3PACK_TOOLDATA *)tool_param;
			pv3		= (NVEC3f32*)pselect->pSelected;
			
			if(pv3)
			{
				NGetEditorWorkPlaneNormalVector(&planenormal);
				NUT_Get3DMouseRay(&mouseray_origin,&mouseray_dirvect,NULL,NGetEventTouchPositionPtr(pevent,0));
				
				if (NUT_LineXPLane(&mouseray_origin, &mouseray_dirvect, &hitpoint, &planenormal, &mouse_on_workplane))
				{
					// MOVE
					// pdata->SetPosition_CallBack(ptr sur l'array où sont stockés les elements, index de l'élément , pointeur sur le NVEC3 inclu dans l'élément slectionné , pointeur sur la position 3D du curseur );
					if (pdata->SetPosition_CallBack)
						pdata->SetPosition_CallBack(pdata->pArrayOfElements, pselect->SelectParam, pv3, &mouse_on_workplane);
					else
						*pv3 = mouse_on_workplane;
					bmoved = NTRUE;
	
					return NEDITOR_RESULT_TOOL_ON;
				}
			}
			break;
		
		case NTOUCH_END:
			if( pselect->pSelected && bmoved )
			{
				NResetToolSelection(pselect);
				bmoved = NFALSE;
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				NResetToolSelection(pselect);
			}
			break;

		default:
			break;
	}


	return NEDITOR_RESULT_TOOL_OFF;
}


// ------------------------------------------------------------------------------------------
// NUT_NVec3f32PackTool_Erase
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//	
//		NTOUCH_END:	"Selected SplinePoint was deleted. Remaining accessible selection informations are obsolete."
//			CallBack is going to be called  ONLY if a SplinePoint is really deleted, and it will be call
//			AFTER the deleting process, BUT before reseting "NEDITOR_SELECTION" data. So, deleted SplinePoint
//			informations remain accessible even if they are obsolete.
//			Take care ! the deleted spline point is really deleted !!!
//			Don't try to use the "NEDITOR_SELECTION" informations to access ...
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NUT_NVec3f32PackTool_Erase(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	NUT_V3PACK_TOOLDATA			*pdata;
	NVEC3f32					localhitpoint;

	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			pdata = (NUT_V3PACK_TOOLDATA *)tool_param;
			_lookfor_v3(pdata->pArrayOfElements, pdata->OffsetToV3, NGetEventTouchPositionPtr(pevent, 0), pselect, &localhitpoint);

			if (pselect->pSelected)
			{
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		case NTOUCH_MOVE:
			// The tool didn't do something, except un-select the splinepoint if there was one !
			pselect->pSelectedOwner	= NULL;
			pselect->pSelected		= NULL;
			pselect->SelectParam	= 0;
			pselect->ZfromCam		= 0;
			break;

		case NTOUCH_END:
			if(pselect->pSelected)
			{
				pdata = (NUT_V3PACK_TOOLDATA *)tool_param;

				// CALL BACK ERASE
				if (pdata->Erase_CallBack)
					pdata->Erase_CallBack(pdata->pArrayOfElements, pselect->SelectParam);
				else
					NEraseArrayElement(pdata->pArrayOfElements, pselect->SelectParam, NULL);
				
				pselect->pSelectedOwner	= NULL;
				pselect->pSelected		= NULL;
				pselect->SelectParam	= 0;
				pselect->ZfromCam		= 0;
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT NUT_NVec3f32PackTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool			bmoved = NFALSE;
	NBYTE					*pcreatedelement;
	Nu32					createdelementindex;
	NVEC3f32				*pv3;
	NUT_V3PACK_TOOLDATA		*pdata;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	NVEC3					result;

	switch (pevent->Type)
	{
	case NTOUCH_START:
		pdata = (NUT_V3PACK_TOOLDATA *)tool_param;
	
		NUT_Get3DMouseRay(&mouseline_point,&mouseline_dir,NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point,&mouseline_dir,&plane_point,&plane_dir, &result);

		// CALL BACK "INSERT"

		pcreatedelement		= NULL;
		createdelementindex = NVOID;

		if (pdata->Insert_CallBack)
		{
			pdata->Insert_CallBack(pdata->pArrayOfElements, &createdelementindex); // user can fill up , or not the pcreatedelement pointer with the adress of the element he created himself.
			if(createdelementindex != NVOID)
				pcreatedelement = NGetArrayPtr(pdata->pArrayOfElements, createdelementindex);
		}
		else
		{
			createdelementindex = pdata->pArrayOfElements->Size;
			pcreatedelement = NArrayAllocBack(pdata->pArrayOfElements);
			memset(pcreatedelement, 0, pdata->pArrayOfElements->ElementSize);
		}
		
		if(pcreatedelement)
		{
			pv3 = (NVEC3f32*)(pcreatedelement + pdata->OffsetToV3);

			// pdata->SetPosition_CallBack(ptr sur l'array où sont stockés les elements, index de l'élément , pointeur sur le NVEC3 inclu dans l'élément slectionné , pointeur sur la position 3D du curseur );
			if (pdata->SetPosition_CallBack)
				pdata->SetPosition_CallBack(pdata->pArrayOfElements, createdelementindex, pv3, &result);
			else
			{
				*pv3 = result;
			}

			pselect->pSelected		= (void*)pv3;
			pselect->pSelectedOwner = (void*)pcreatedelement;
			pselect->SelectParam	= createdelementindex;
			pselect->ZfromCam		= 0.0f; // Unused ?
		}
		else
		{
			NResetToolSelection(pselect);
		}
		return NEDITOR_RESULT_TOOL_ON;
	
	case NTOUCH_MOVE:
		pdata = (NUT_V3PACK_TOOLDATA *)tool_param;
		// 			ptouch=(NEVENT_TOUCH*)pevent;
		pv3= (NVEC3f32*)pselect->pSelected;
		if (pv3)
		{
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);

			if (NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &result))
			{
					// MOVE
					// pdata->SetPosition_CallBack(ptr sur l'array où sont stocké les elements, index de l'élément sélectionné, pointeur sur le NVEC3 inclu dans l'élément , pointeur sur la position 3D du curseur );
					if (pdata->SetPosition_CallBack)
						pdata->SetPosition_CallBack(pdata->pArrayOfElements, pselect->SelectParam, pv3, &result);
					else
						*pv3 = result;

					bmoved = NTRUE;

				return NEDITOR_RESULT_TOOL_ON;
			}
		}
		break;

	case NTOUCH_END:
		if (pselect->pSelected && bmoved)
		{
			// 				NCallCurrentToolCallBack(pevent);
			NResetToolSelection(pselect);
			bmoved = NFALSE;
			return NEDITOR_RESULT_TOOL_ON;
		}
		else
		{
			NResetToolSelection(pselect);
		}
	default:
		break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR