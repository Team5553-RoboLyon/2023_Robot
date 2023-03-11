#ifdef _NEDITOR
// -----------------------------------------------------------------------
// #include "lib/N/NCStandard.h"
// #include "lib/N/Event/NEvent.h"
// #include "lib/N/NCamera.h"
#include "lib/N/NViewport.h"
#include "lib/N/NEditor.h"
#include "lib/N/NCore.h"
#include "lib/N/Event/NEventTouch.h"
// GLOBALES
NEDITOR NEditor;

// ------------------------------------------------------------------------------------------
// NInitializeEditor
// ------------------------------------------------------------------------------------------
// Description :
//	Initialize all the data/structures used by the NEditor functions.
//	It's important to call this function one time at the beginning.
// ------------------------------------------------------------------------------------------
// In  :
//		nbtoolcapacity: number of tool (not necessary) to anticipate the tools array capacity.
//
// Out :
//		NTRUE if everything is OK.
//		NFALSE, if there is a problem
// ------------------------------------------------------------------------------------------
void NInitializeEditor(const Nu32 nbtoolcapacity)
{
	memset(&NEditor, 0, sizeof(NEDITOR));
	NSetEditorWorkPlaneNormalVectorf(0, 0, 1); // Work Plane is XY PLANE
	NSetEditorWorkPlanePointf(0, 0, 0);		   // Work Plane pass trough World Origin

	NSetupArray(&NEditor.ToolsArray, nbtoolcapacity, sizeof(NEDITOR_TOOL));
	// Initialize the Listener with 1 Touch
	NSetupTouchListener(&NEditor.Listener, 1);
	// NEditor.pDefault_EventHandle = NEditor_Default_EventHandle;
	FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_EDITOR_INITIALIZED);
}

// ------------------------------------------------------------------------------------------
// NQuitEditor
// ------------------------------------------------------------------------------------------
// Description :
//	Clear/Delete all the data/structures initialized and set up by/for NEditor.
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
// ------------------------------------------------------------------------------------------
void NQuitEditor()
{
	if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_EDITOR_INITIALIZED))
	{
		NClearArray(&NEditor.ToolsArray, NULL);
		NClearTouchListener(&NEditor.Listener);
		FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_EDITOR_INITIALIZED);
	}
}

// ------------------------------------------------------------------------------------------
// NRegisterSingleTouchTool
// ------------------------------------------------------------------------------------------
// Description :
//		Insert a new tool structure into the global ToolArray.
// ------------------------------------------------------------------------------------------
// In  :
//			ptool_fct:	a Valid NEDITOR_TOOL_FCT function (the tool) which will 'eat' event messages
//						to work.
//
//			tool_param:
// Out :
//			Tool ID.
// ------------------------------------------------------------------------------------------
Nu32 NRegisterSingleTouchTool(NEDITOR_TOOL_STARTSTOPHANDLE ptool_starthandle, NEDITOR_TOOL_STARTSTOPHANDLE ptool_stophandle, NEDITOR_TOOL_EVENTHANDLE ptool_eventhandle, NEDITOR_TOOL_TIMEHANDLE ptool_timehandle, const Nu32 tool_param)
{
	NEDITOR_TOOL tool;
	Nu32 ret;

	memset(&tool, 0, sizeof(NEDITOR_TOOL));
	tool.pTool_StartHandle = ptool_starthandle;
	tool.pTool_StopHandle = ptool_stophandle;
	tool.pTool_EventHandle = ptool_eventhandle;
	tool.pTool_TimeHandle = ptool_timehandle;
	tool.ToolParam = tool_param;
	tool.TouchCountMax = 1; // Single Touch !
	ret = NGetArraySize(&NEditor.ToolsArray);
	NArrayPushBack(&NEditor.ToolsArray, (NBYTE *)&tool);
	return ret;
}

// ------------------------------------------------------------------------------------------
// NRegisterMultiTouchTool
// ------------------------------------------------------------------------------------------
// Description :
//		Insert a new tool structure into the global ToolArray.
// ------------------------------------------------------------------------------------------
// In  :
//			ptool_fct:	a Valid NEDITOR_TOOL_FCT function (the tool) which will 'eat' event messages
//						to work.
//
//			tool_param:
// Out :
//			Tool ID.
// ------------------------------------------------------------------------------------------
Nu32 NRegisterMultiTouchTool(const Nu8 touch_count_max, NEDITOR_TOOL_STARTSTOPHANDLE ptool_starthandle, NEDITOR_TOOL_STARTSTOPHANDLE ptool_stophandle, NEDITOR_TOOL_EVENTHANDLE ptool_eventhandle, NEDITOR_TOOL_TIMEHANDLE ptool_timehandle, const Nu32 tool_param)
{
	NEDITOR_TOOL tool;
	Nu32 ret;

	memset(&tool, 0, sizeof(NEDITOR_TOOL));
	tool.pTool_StartHandle = ptool_starthandle;
	tool.pTool_StopHandle = ptool_stophandle;
	tool.pTool_EventHandle = ptool_eventhandle;
	tool.pTool_TimeHandle = ptool_timehandle;
	tool.ToolParam = tool_param;
	tool.TouchCountMax = touch_count_max;
	ret = NGetArraySize(&NEditor.ToolsArray);
	NArrayPushBack(&NEditor.ToolsArray, (NBYTE *)&tool);
	return ret;
}

void NSetCurrentTool(const Nu32 tool_id)
{
	NEDITOR_TOOL *pnew;
	NEVENT nevent;

	if (tool_id != NVOID)
	{
		pnew = (NEDITOR_TOOL *)NGetArrayPtr(&NEditor.ToolsArray, tool_id);
		if (pnew != NEditor.pCurrentTool)
		{
			if (NEditor.pCurrentTool)
			{
				if (NGetListenerTouchCount(&NEditor.Listener))
				{
					NTouchListenerCancelAll(&NEditor.Listener);
					NFillupEventHead(nevent, NTOUCH_CANCEL, NEVENT_PARAM_TOUCH_CANCEL__ALL, NEVENT_RECIPIENT_CAST_NEDITOR_TOOL, NEditor.pCurrentTool);
					nevent.Touch.TouchSequence.TouchCount = 0;
					NEditor.pCurrentTool->pTool_EventHandle(&nevent, &NEditor.Selection, NEditor.pCurrentTool->ToolParam);
				}

				if (NEditor.pCurrentTool->pTool_StopHandle)
					NEditor.pCurrentTool->pTool_StopHandle(&NEditor.Selection, NEditor.pCurrentTool->ToolParam);
			}

			NResetToolSelection(&NEditor.Selection);
			NEditor.pCurrentTool = pnew;

			if (NEditor.pCurrentTool)
			{
				NSetTouchListenerTouchCountMax(&NEditor.Listener, NEditor.pCurrentTool->TouchCountMax);
				if (NEditor.pCurrentTool->pTool_StartHandle)
					NEditor.pCurrentTool->pTool_StartHandle(&NEditor.Selection, NEditor.pCurrentTool->ToolParam);
			}
		}
	}
	else
	{
		if (NEditor.pCurrentTool)
		{
			if (NGetListenerTouchCount(&NEditor.Listener))
			{
				NTouchListenerCancelAll(&NEditor.Listener);
				NFillupEventHead(nevent, NTOUCH_CANCEL, NEVENT_PARAM_TOUCH_CANCEL__ALL, NEVENT_RECIPIENT_CAST_NEDITOR_TOOL, NEditor.pCurrentTool);
				nevent.Touch.TouchSequence.TouchCount = 0;
				NEditor.pCurrentTool->pTool_EventHandle(&nevent, &NEditor.Selection, NEditor.pCurrentTool->ToolParam);
			}

			if (NEditor.pCurrentTool->pTool_StopHandle)
				NEditor.pCurrentTool->pTool_StopHandle(&NEditor.Selection, NEditor.pCurrentTool->ToolParam);
		}
		NResetToolSelection(&NEditor.Selection);
		NEditor.pCurrentTool = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// NEDITOR_RESULT		NCallCurrentTool
// ------------------------------------------------------------------------------------------
// Description :
//		Call the current tool define by 'SetCurrentTool'.
//
// ------------------------------------------------------------------------------------------
// In	:
//			const NEVENT *psource_event		Event receive by Tool Owner or by system
//											!!! Tool manage only TOUCH EVENT !!! and will return NEDITOR_RESULT_TOOL_NULL
//											if psource_event is not a Touch Event.
//
//			NEVENT *ptool_event				Because of MultiTouch, Tool has to listen incoming 'psource_event' against its
//											own listening constraints.
//											For example: What happen if 'psource_event' describe the start of 4 simultaneous TOUCHs
//											and if current tool manage only 2 TOUCHs ?
//											... simple actually.
//											As usual, before calling tool, 'psource_event' touch data  are listen again
//											thanks to a touch listener which knows current tool constraints.
//											A specific event is build up and send to the current tool.
//											So,
//
//											If 'ptool_event' is NON NULL. It will be build up by 'NCallCurrentTool' and send to Current Tool.
//											At the end, user can get back the event really send to the Tool.
//
//											If 'ptool_event' is NULL. 'NCallCurrentTool' is going to build it's own local tool event.
//											At the end, this event is going to be lost.
//
// Out :
//		one of the explicit 	NEDITOR_RESULT values
//
//								NEDITOR_RESULT_TOOL_NO_TOUCH_EVENT	 means incoming event is not a touch event, so tool is not called !
//								NEDITOR_RESULT_TOOL_NULL			 means no current tool !
//								NEDITOR_RESULT_TOOL_OFF				 means current tool gets event and return OFF ( i did nothing )
//								NEDITOR_RESULT_TOOL_ON				 means current tool gets event and return ON ( i did something )
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NCallCurrentTool(const NEVENT *psource_event, NEVENT *ptool_event)
{
	if (NEditor.pCurrentTool != NULL)
	{
		if (NGET_EVENT_MAIN(psource_event->Type) == _TOUCH)
		{
			NEVENT tool_event;
			NEVENT *pused_event;

			if (ptool_event)
				pused_event = ptool_event;
			else
				pused_event = &tool_event;

			NTOUCH_LISTENER *psource_event_touchlistener = NGetEventRecipientTouchListener(psource_event);
			// Listen Process
			if (psource_event_touchlistener->LTouchCountMax == NEditor.pCurrentTool->TouchCountMax)
			{
				// A simple copy is ok !
				NCopyTouchListener(&NEditor.Listener, psource_event_touchlistener);
				pused_event->Touch.TouchSequence = psource_event->Touch.TouchSequence;
				pused_event->Head.Type = psource_event->Head.Type;
				pused_event->Head.Param = psource_event->Head.Param;
				pused_event->Head.RecipientCast = NEVENT_RECIPIENT_CAST_NEDITOR_TOOL;
				pused_event->Head.pRecipient = NEditor.pCurrentTool;
			}
			else
			{
				// A real listening process is needed !
				// Check Correlation between Event SubType and Touch Phase still available ( it must !!! )
				// 				NErrorIf(_START!=NTOUCH_PHASE_START || _MOVE!=NTOUCH_PHASE_MOVE || _END!=NTOUCH_PHASE_END || _CANCEL!=NTOUCH_PHASE_CANCEL, NERROR_SYSTEM_NENGINEVERSION_CONFLICT );// Some Engine core modification change the initial correlation between event substype and touch phase ... it's wrong !
				// 				NListenTouchSequence(&NEditor.Listener, _SafeNu32ToNu8(NGET_EVENT_SUB(psource_event->Type)), &pused_event->Touch.TouchSequence, &psource_event->Touch.TouchSequence );
				NListenTouchEvent(&NEditor.Listener, pused_event, psource_event);
				pused_event->Head.RecipientCast = NEVENT_RECIPIENT_CAST_NEDITOR_TOOL;
				pused_event->Head.pRecipient = NEditor.pCurrentTool;
			}
			return NEditor.pCurrentTool->pTool_EventHandle(pused_event, &NEditor.Selection, NEditor.pCurrentTool->ToolParam);
		}
		else
		{
			return NEDITOR_RESULT_TOOL_NO_TOUCH_EVENT;
		}
	}
	else
	{
		return NEDITOR_RESULT_TOOL_NULL;
	}
}

/*
void NCallCurrentToolTimeHandle(const NTIME *ptime)
{
	if(NEditor.pCurrentTool!=NULL && NEditor.pCurrentTool->pTool_TimeHandle)
	{
		NEditor.pCurrentTool->pTool_TimeHandle(ptime,(const NEDITOR_SELECTION*)&NEditor.Selection,(const Nu32)NEditor.pCurrentTool->ToolParam);

		// Tools Infos ?
		??? to do ???
		if(ISFLAG_ON(NEditor.Flags,FLAG_NEDITOR_INFOS))
		{
			NUT_DRAW_RENDERING_MODE_ENUM prevmode = NUT_GetDrawMode();
			NUT_SetDrawMode(NEditor.Info_TextDrawMode);
			NUT_Draw_Text(NEditor.Info_Text,&NEditor.Info_TextPosition,&NEditor.Info_TextCfg);
			NUT_SetDrawMode(prevmode);
		}

	}
}
*/

NUI_EVENT_RESULT NDefault_ToolButton_EventProc(NUI *pui, const NEVENT *pevent)
{
	switch (pevent->Type)
	{
	case NTOUCH_END:
		NSetCurrentTool(NGetUIUserData(pui));
		return NUI_EVENT_RESULT_PROCESSED;

	default:
		return NDefaultUIEvent_Handle(pui, pevent);
	}
}

void NDefault_Editor_EventDrivenHandle(const NEVENT *pevent)
{
	/*
		// A tool can't work if an UI has the focus !
		if( NGetUIFocusCount() )
			return;
	*/
	if (NCallCurrentTool(pevent, NULL) != NEDITOR_RESULT_TOOL_ON)
	{
		// ... Do something else like camera moves ...
		// instead of NULL pass a pointer to a valid NEVENT structure to get back the event send to the current tool
		// ... if you need it of course.
	}
}

void NDefault_Editor_TimeDrivenHandle(const NTIME *ptime)
{
	NCallCurrentToolTimeHandle(ptime);
}

void NSetEditorWorkCamera(NCAMERA *pcamera)
{
	if (pcamera)
		NEditor.pWorkCamera = pcamera;
	else
		NEditor.pWorkCamera = NGetCamera();
}

void NGetEditorWorkPlaneNormalVector(NVEC3 *pn)
{
	if (ISFLAG_ON(NEditor.Flags, FLAG_NEDITOR_FACECAMERA_WORKPLANE))
	{
		NUpdateCameraAxis(NEditor.pWorkCamera);
		*pn = NEditor.pWorkCamera->ZAxis;
	}
	else
	{
		*pn = NEditor.WorkPlane_NormalVector;
	}
}

void NGetEditorWorkPlanePoint(NVEC3 *pn)
{
	if (ISFLAG_ON(NEditor.Flags, FLAG_NEDITOR_ONCAMERATARGET_WORKPLANE))
	{
		*pn = NEditor.pWorkCamera->TargetPos;
	}
	else
	{
		*pn = NEditor.WorkPlane_Point;
	}
}

// -----------------------------------------------------------------------
#endif //_NEDITOR