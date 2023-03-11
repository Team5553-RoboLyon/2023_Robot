#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#ifndef __NEDITOR_H
#define __NEDITOR_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NEditor.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// Notes:
//	These functions and process help to create and use tools trough the NUI system.
//	Principles to create several tools & associated buttons:
//	1)	Registered a Tool
//
//	2)	Create buttons  with the tool registered index as user value, like this
//
//		NCreateUISoftPushButtonf( parent, x, y, " tool name ", back_frame,icon_frame,style,Button_Event_Proc,NRegisterSingleTouchTool(Main_Tool_Fct,tool_user_data);
//
//		"Button_Event_Proc" will be the same for all the buttons.
//		you can use the default N function included : 'NDefault_ToolButton_Event_Proc'
//		This default function will simply Call 'NSetCurrentTool' when user clicks on the button.
//		... if you want, you can make your own function to do some specific things before calling 'NSetCurrentTool'.
//				
//		NRegisterSingleTouchTool(Tool_EventHandle,Tool_TimeHandle,tool_user_data)
//
//	3)	Thanks to 'NBindGameStateEventDrivenHandle' Bind an handling function to dispatch messages to current tool and lets it work.
//
//		NBindGameStateEventDrivenHandle(STATE_EDITOR,NDefault_Editor_EventDrivenHandle);
//
//		You can use the default N function included : 'NDefault_Editor_EventDrivenHandle'
//		This default function will simply Call 'NCallCurrentTool' when an EVENT occurs.
//		AND will also call 'NCallEditorDefaultEventHandle' if the current tool returns NFALSE or if there is no current tool
//
//		------------------------------------
//		NDefault_Editor_EventDrivenHandle
//		------------------------------------
//		+ NCallCurrentTool
//		|		|
//		|		+-> NCallCurrentToolCallBack   
//		|			(This/these call(s) is/are done by user 'from inside'. I mean from pTool_EventHandle source code. It's not obligatory !)
//		|
//		+-> return NFALSE -> NCallEditorDefaultEventHandle
//
//		notes:  The tool_callback function is called by the tool itself. there is no use restrictions about these calls. Each tool can perform, or not several
//				calls sometimes before acting, sometimes after. Look for the tool documentation to know. 
//				BUT, in general the "good practice" will be:
//				A Tool will call its callback only AFTER doing what is suppose to do. 
//				... Like this, everything stays simple.		
//
//	4)	Thanks to 'NBindGameStateTimeDrivenHandle' Bind a time handling function to display tool's stuff on screen or perform other timed tool processes.
//
//		NBindGameStateEventDrivenHandle(STATE_EDITOR,NDefault_Editor_EventDrivenHandle);
//
//		You can use the default N function included : 'NDefault_Editor_TimeDrivenHandle'
//		This default function will simply Call 'NCallCurrentToolTimeHandle'.
// 
//		------------------------------------
//		NDefault_Editor_TimeDrivenHandle
//		------------------------------------
//		+ NCallCurrentToolTimeHandle

#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"

#include "lib/N/UI/NUI.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/Touch/NTouchListener.h"
#include "lib/N/NCamera.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------


typedef struct
{	
	void					*pSelectedOwner;	// A pointer on the "stuff" which own the selected entity (can be NULL). for exemple, it could be a NSPLINE* which own a selected NSPLINEKNOT*
	void					*pSelected;			// A pointer on the entity selected
	Nu32					SelectParam;
	Nf32					ZfromCam;			// To be deleted !! WRONG use !!
	//NVEC3				HitPoint;			
}NEDITOR_SELECTION;


/*
typedef Ns32	NEDITOR_RESULT;
#define NEDITOR_RESULT_TOOL_NULL		 0	//|
#define NEDITOR_RESULT_TOOL_ON		 1	//|- Simple Rule: result < 0, means tool doesn't do what it has to do. 
#define NEDITOR_RESULT_TOOL_OFF	-1	//|				  result > 0, means tool does what it has to do.	 
*/
typedef enum
{
	NEDITOR_RESULT_TOOL_NO_TOUCH_EVENT	=	-2,	// means incoming event is not a touch event, so tool is not called !
	NEDITOR_RESULT_TOOL_NULL			=	-1,	// means no current tool ! 
	NEDITOR_RESULT_TOOL_OFF				=	 0,	// means current tool gets event and return OFF ( i did nothing )
	NEDITOR_RESULT_TOOL_ON				=	 1	// means current tool gets event and return ON ( i did something )

}NEDITOR_RESULT;

// typedef Nbool (*NEDITOR_DEFAULT_EVENTHANDLE)( const NEVENT* pevent );															// when there is no current tool or if the current tool does nothing (return NFALSE) N calls this default function, if there is one.
// typedef Nbool (*NEDITOR_TOOL_EVENTHANDLE)(const NEVENT* pevent,NEDITOR_SELECTION *pselection,const Nu32 tool_param );				// A Tool return NTRUE if it selects something, else NFALSE.
//typedef void				(*NEDITOR_DEFAULT_EVENTHANDLE)( const NEVENT* pevent, const NEDITOR_RESULT tool_eventhandle_result );															// when there is no current tool or if the current tool does nothing (return NFALSE) N calls this default function, if there is one.
typedef NEDITOR_RESULT		(*NEDITOR_TOOL_EVENTHANDLE)(const NEVENT* pevent,NEDITOR_SELECTION *pselection,const Nu32 tool_param );				// A Tool return NTRUE if it selects something, else NFALSE.
//typedef void				(*NEDITOR_TOOL_CALLBACK)(const NTOOLEVENT* ptoolevent,const Nu32 tool_param );		//
typedef void				(*NEDITOR_TOOL_TIMEHANDLE)(const NTIME* ptime,const NEDITOR_SELECTION *pselection,const Nu32 tool_param );		//
typedef void				(*NEDITOR_TOOL_STARTSTOPHANDLE)(const NEDITOR_SELECTION *pselection,const Nu32 tool_param );				//

/*

#define	SNKS_SPS_NULL				0

#define	SNKS_SPS_ENABLE				1	
#define	SNKS_SPS_PRE				2
#define	SNKS_SPS_POST				3
#define	SNKS_SPS_DISABLE			4	

#define	SNKS_SPS_ADD				1
#define	SNKS_SPS_SUPR				2
#define	SNKS_SPS_MOVE				3

#define	SNKS_SPS_KNOT_POSITION		1
#define	SNKS_SPS_KNOT_TA_HANDLE		2	
#define	SNKS_SPS_KNOT_TA_SEGMENT	3	
#define	SNKS_SPS_KNOT_TB_HANDLE		3
#define	SNKS_SPS_KNOT_TB_SEGMENT	4

#define NMAKE_EDITOR_TOOLEVENT_TYPE(toolid_16,instantid_4,param_4,param_8)	(((toolid_16)&0xFFFF)|(((instantid_4)&0xF)<<16)|(((param_4)&0xF)<<20)|(((param_8)&0xFF)<<24))

#define		SNKS_SPS_ENABLE_ADD		SNKS_MAKE_SPS_MESSAGE(SNKS_SPS_ENABLE,SNKS_SPS_ADD,SNKS_SPS_NULL,SNKS_SPS_NULL)
#define		SNKS_SPS_PRE_ADD		SNKS_MAKE_SPS_MESSAGE(SNKS_SPS_PRE,SNKS_SPS_ADD,SNKS_SPS_NULL,SNKS_SPS_NULL)
#define		SNKS_SPS_POST_ADD		SNKS_MAKE_SPS_MESSAGE(SNKS_SPS_POST,SNKS_SPS_ADD,SNKS_SPS_NULL,SNKS_SPS_NULL)
#define		SNKS_SPS_DISABLE_ADD	SNKS_MAKE_SPS_MESSAGE(SNKS_SPS_ENABLE,SNKS_SPS_ADD,SNKS_SPS_NULL,SNKS_SPS_NULL)


typedef struct  
{
	Nu32		Type;
}NTOOLEVENT;

typedef struct  
{
	Nu16		Type;
	Nu16		Params;
}SNKS_SPS_MESSAGE_ENABLE_ADD;

typedef struct  
{
	Nu16		Type;
	Nu16		Params;

}SNKS_SPS_MESSAGE_PRE_ADD;



#define SNKS_SPS_ENABLE_MOVE_POSITION		NMAKELONG(SNKS_SPS_ENABLE_MOVE,SNKS_SPS_SPLINEKNOT_POSITION)
#define SNKS_SPS_ENABLE_MOVE_TA_HANDLE		NMAKELONG(SNKS_SPS_ENABLE_MOVE,SNKS_SPS_SPLINEKNOT_TA_HANDLE)
#define SNKS_SPS_ENABLE_MOVE_POSITION		NMAKELONG(SNKS_SPS_ENABLE_MOVE,SNKS_SPS_SPLINEKNOT_POSITION)
*/

typedef struct
{	
	NEDITOR_TOOL_STARTSTOPHANDLE	pTool_StartHandle;
	NEDITOR_TOOL_STARTSTOPHANDLE	pTool_StopHandle;
	NEDITOR_TOOL_EVENTHANDLE		pTool_EventHandle;		// A pointer on the Main tool function. the one which be called by the 'NCallcurrentTool' function
	NEDITOR_TOOL_TIMEHANDLE			pTool_TimeHandle;
	Nu8								TouchCountMax;
	Nu8								Available_Nu8;
	Nu16							Available_Nu16;
	Nu32							ToolParam;
}NEDITOR_TOOL;

#define FLAG_NEDITOR_FACECAMERA_WORKPLANE		BIT_0		// Work Plane Normal Vector is aligned on Camera Z Axis
#define FLAG_NEDITOR_ONCAMERATARGET_WORKPLANE	BIT_1		// Work Plane Point is the Target Camera

// #define FLAG_NEDITOR_INFOS					BIT_1 	??? to do ???

typedef struct  
{	
	Nu32						Flags;
	NTOUCH_LISTENER				Listener;
	NEDITOR_TOOL				*pCurrentTool;
	NARRAY						ToolsArray;
	NEDITOR_SELECTION			Selection;
	//NEDITOR_DEFAULT_EVENTHANDLE	pDefault_EventHandle;

	// 3D Work Plane
	NVEC3					WorkPlane_Point; 
	NVEC3					WorkPlane_NormalVector;
	NCAMERA						*pWorkCamera;
	

/*
	??? to do ???
	// Infos
	Nchar							Info_Text[256];
	NUT_DRAWTEXT					Info_TextCfg;
	NVEC3						Info_TextPosition;
	NUT_DRAW_RENDERING_MODE_ENUM	Info_TextDrawMode;
*/
}NEDITOR;

extern NEDITOR				NEditor;

void						NInitializeEditor(const Nu32 nbtoolcapacity);
void						NQuitEditor();

// Tool
Nu32						NRegisterSingleTouchTool( NEDITOR_TOOL_STARTSTOPHANDLE ptool_starthandle, NEDITOR_TOOL_STARTSTOPHANDLE ptool_stophandle, NEDITOR_TOOL_EVENTHANDLE ptool_eventhandle,NEDITOR_TOOL_TIMEHANDLE ptool_timehandle, const Nu32 tool_param);
Nu32						NRegisterMultiTouchTool(const Nu8 touch_count_max, NEDITOR_TOOL_STARTSTOPHANDLE ptool_starthandle, NEDITOR_TOOL_STARTSTOPHANDLE ptool_stophandle, NEDITOR_TOOL_EVENTHANDLE ptool_eventhandle,NEDITOR_TOOL_TIMEHANDLE ptool_timehandle, const Nu32 tool_param);

inline void					NUnRegisterTool(NEDITOR_TOOL *ptool){NEraseArrayElementPtr(&NEditor.ToolsArray,(NBYTE*)ptool,NULL);}
inline void					NUnRegisterToolByID(const Nu32 tool_id){NEraseArrayElement(&NEditor.ToolsArray,tool_id,NULL);}

inline	void				NResetToolSelection(NEDITOR_SELECTION *ps){memset(ps,0,sizeof(NEDITOR_SELECTION));}
NEDITOR_RESULT				NCallCurrentTool(const NEVENT *psource_event, NEVENT *ptool_event);
inline	void				NCallCurrentToolTimeHandle(const NTIME *ptime){ if(NEditor.pCurrentTool!=NULL)if(NEditor.pCurrentTool->pTool_TimeHandle){ NEditor.pCurrentTool->pTool_TimeHandle(ptime,(const NEDITOR_SELECTION*)&NEditor.Selection,(const Nu32)NEditor.pCurrentTool->ToolParam);} }
void						NSetCurrentTool(const Nu32 tool_id);
inline	Nu32				NGetCurrentTool() { return NEditor.pCurrentTool ? NGetArrayIndex(&NEditor.ToolsArray, (NBYTE*)NEditor.pCurrentTool) : NVOID; }
inline	Nu32				NGetCurrentToolParam(){if(NEditor.pCurrentTool){return NEditor.pCurrentTool->ToolParam;}else{return NULL;}}
inline	void				NSetCurrentToolParam(Nu32 param){if(NEditor.pCurrentTool){NEditor.pCurrentTool->ToolParam=param;}}

inline	Nu32				NGetToolParam(Nu32 toolid) { return ((NEDITOR_TOOL*)NGetArrayPtr(&NEditor.ToolsArray, toolid))->ToolParam; }
inline	void				NSetToolParam(Nu32 toolid,Nu32 param) { ((NEDITOR_TOOL*)NGetArrayPtr(&NEditor.ToolsArray, toolid))->ToolParam = param; } 



// inline	void			NCallEditorDefaultEventHandle(const NEVENT *pevent,const NEDITOR_RESULT tool_eventhandle_result){ NErrorIf(!NEditor.pDefault_EventHandle,NERROR_NULL_POINTER);NEditor.pDefault_EventHandle(pevent, tool_eventhandle_result); }
// void						NSetEditorDefaultEventHandle(NEDITOR_DEFAULT_EVENTHANDLE	pdefaulteventhandle);

NUI_EVENT_RESULT			NDefault_ToolButton_EventProc(NUI *pui, const NEVENT *pevent);
void						NDefault_Editor_EventDrivenHandle(const NEVENT* pevent);
void						NDefault_Editor_TimeDrivenHandle(const NTIME* ptime);

inline	NEDITOR_SELECTION*	NGetEditorSelection(){return &NEditor.Selection;}

void						NGetEditorWorkPlaneNormalVector(NVEC3 *pn);
inline void					NSetEditorWorkPlaneNormalVector(const NVEC3 *pn){NEditor.WorkPlane_NormalVector = *pn;}
inline void					NSetEditorWorkPlaneNormalVectorf(const Nf32 x,const Nf32 y,const Nf32 z){NVec3Set(&NEditor.WorkPlane_NormalVector,x,y,z);}

void						NGetEditorWorkPlanePoint(NVEC3 *pn);
inline void					NSetEditorWorkPlanePoint(const NVEC3 *p) { NEditor.WorkPlane_Point = *p; }
inline void					NSetEditorWorkPlanePointf(const Nf32 x, const Nf32 y, const Nf32 z) { NVec3Set(&NEditor.WorkPlane_Point, x, y, z); }

void						NSetEditorWorkCamera(NCAMERA *pcamera);
inline NCAMERA*				NGetEditorWorkCamera(){return NEditor.pWorkCamera;}

inline void					NActivateFaceCameraEditorWorkPlane(){FLAG_ON(NEditor.Flags,FLAG_NEDITOR_FACECAMERA_WORKPLANE);}
inline void					NUnactivateFaceCameraEditorWorkPlane(){FLAG_OFF(NEditor.Flags,FLAG_NEDITOR_FACECAMERA_WORKPLANE);}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NEDITOR_H 

// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR

  