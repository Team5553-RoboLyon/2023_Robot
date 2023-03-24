#include "../NCStandard.h"
#include "../NCore.h"

#include "../Render/Renderable/NRenderable.h"
#include "../Render/Renderable/NRenderable_Extract.h"
#include "../Render/Renderable/NRenderable_ExtractSet.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

#include "../Render/NFrustum_Culling.h"
#include "../NViewport.h"
#include "../NErrorHandling.h"
#include "../NString.h"
#include "../Geometry/NGeometryBoundingVolume.h"
#include "../Geometry/NGeometryVertexTransformations.h"
#include "../Geometry/Components/NGeometryVertex.h"
#include "../Geometry/Components/NGeometryTextureCoordinate.h"
#include "../Utilities/NUT_Shape.h"

#include "NUIClearWatchStack.h"
#include "NUIRenderableUpdate.h"
#include "NUIColor.h"
#include "NUI.h"


// GLobales
NARRAY							ActiveUIRootsList;	
extern Nu32						NEngineCoreFlags;

#ifdef _DEBUG
static inline Nbool _is_ui_in_root_list(const NUI* pui)
{
	NUI		**ptr;
	ptr = (NUI**)NGetFirstArrayPtr(&ActiveUIRootsList);
	for( Nu32 i=0;i<ActiveUIRootsList.Size;i++,ptr++)
	{	
		if(*ptr == pui)
		{
			NErrorIf(pui->pParent, NERROR_SYSTEM_CHECK); // Impossible, but just in case ...
			return NTRUE;
		}
	}
	return NFALSE;
}
#endif

Nbool NEnableUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);
	// The following case is not suppose to happen... 
	// If it does that means user UI is already Enable with a disable Parent !!!!! VERY BAD !!!
	NErrorIf( NIsUIEnable(pui) && pui->pParent && NIsUIDisable((NUI*)pui->pParent),NERROR_UI_ENABLE_UI_PARENT_MUST_BE_ENABLE ); // UI is already Enable with a disable Parent !!!!! VERY BAD !!!

	// Is UI disable ?
	if( ISFLAG_OFF( pui->Flags,FLAG_NUI_ENABLE ) )
	{
		// 0)	ENABLE RULE #1: Enable UI MUST have Enable Parent or NULL parent
		//		ENABLE RULE #2: Disable UI may have Enable/Disable/NULL Parent
		if(pui->pParent && NIsUIDisable((NUI*)pui->pParent) )
		{
			NErrorIf(1, NERROR_UI_ENABLE_UI_PARENT_MUST_BE_ENABLE); // UI is Disable and can not be enable with a disable parent !
			return NFALSE;
		}
		// 0) Flag UI as "Enable"
		FLAG_ON( pui->Flags,FLAG_NUI_ENABLE );
		// 1) Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		NEVENT	nevent;
		Nmem0(&nevent,NEVENT);
		NFillupEventHead(nevent,NUI_ENABLE,0,NEVENT_RECIPIENT_CAST_UI,pui);
		//NSendUIEvent(&nevent,NULL);
		NSendUICoreEvent(&nevent);
		if(NUIClearWatchStackPop()!=pui || ISFLAG_OFF( pui->Flags,FLAG_NUI_ENABLE ) )
		{
			return NFALSE; // UI is not Enable, is deleted or disable !!!
		}

		// 2) Update 'UI' Color
		if(ISFLAG_ON(pui->Flags,FLAG_NUI_COLOR_UPDATE))
			NSetUIColor(pui,NUI_COLORSET_ENABLE);
		
		// 3) Propagate to children ... if there are some ...
		if( NIS_NODE_VALID(pui->ChildrenList.pFirst,&pui->ChildrenList) )
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

			// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
			// Do this from the first to the last ...
			#ifdef _DEBUG	
			Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
			#endif
			Nu32 stacksize = 0;
			NUI *puichild = (NUI*)pui->ChildrenList.pFirst;
			while( NIS_NODE_VALID(puichild,&pui->ChildrenList) )
			{
				if( ISFLAG_ON(puichild->FlagsXtd,FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL) )
				{
					NUIClearWatchStackPush(puichild);
					stacksize ++;
				}
			
				puichild = (NUI*)NGET_NEXT_NODE(puichild);
			}

			// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
			// ( because it could be deleted during the process ... ) 
			// pop them one after one from the stack...
			// that means call them from the Last UI Children to the First ( due to the way they were pushed into the array)
			while(stacksize)
			{
				if( (puichild = NUIClearWatchStackPop()) )
				{
					NEnableUI(puichild);
				}
				stacksize--;
			}
			#ifdef _DEBUG
			NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
			#endif

			if(NUIClearWatchStackPop()!=pui)
				return NFALSE; // UI is not Enable, is deleted !!!
			else
				return NTRUE;
		}
		else
		{
			return NTRUE;
		}
	}
	else
	{
		return NTRUE; // UI is already Enable. So return NTRUE.
	}
}

Nbool NDisableUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);
	NErrorIf( NIsUIEnable(pui) && pui->pParent && NIsUIDisable((NUI*)pui->pParent),NERROR_UI_ENABLE_UI_PARENT_MUST_BE_ENABLE ); 

	// Is UI Enable ?
	if( ISFLAG_ON( pui->Flags,FLAG_NUI_ENABLE ) )
	{
		NUI *puichild;

		// 1a) 	Find all the 'pui' value in the Delayed Event Array that request an enable recipient and set them to NULL !
		//		... if the event type is not compatible with a Disable UI ( _UICOMMAND event type is not )
		NDelayedUIEventArray_UIDisable_Notification(pui);

		// 0) First of all ... Cancel all Potential Touch Intercepting/Listening.
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

		NUITouchCancelAll(pui,NTRUE);

		if(NUIClearWatchStackPop()!=pui)
			return NFALSE;  // UI is not Disable, is deleted !!!

		// 1) Flag UI as DISABLE
		FLAG_OFF( pui->Flags,FLAG_NUI_ENABLE );

		// 2) Send uicore Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		NEVENT	nevent;
		Nmem0(&nevent,NEVENT);
		NFillupEventHead(nevent,NUI_DISABLE,0,NEVENT_RECIPIENT_CAST_UI,pui);
		//NSendUIEvent(&nevent,NULL);
		NSendUICoreEvent(&nevent);
		if(NUIClearWatchStackPop()!=pui ||ISFLAG_ON( pui->Flags,FLAG_NUI_ENABLE ))
			return NFALSE;  // UI is not Disable, is deleted or Enable !!!

		// 3)  Update 'UI' Color
		if(ISFLAG_ON(pui->Flags,FLAG_NUI_COLOR_UPDATE))
			NSetUIColor(pui,NUI_COLORSET_DISABLE);

		// 4) Propagate to children ... if there are some ...
		if( NIS_NODE_VALID(pui->ChildrenList.pFirst,&pui->ChildrenList) )
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

			// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
			// Do this from the last to the first ...
			#ifdef _DEBUG	
			Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
			#endif
			Nu32 stacksize = 0;
			puichild = (NUI*)pui->ChildrenList.pFirst;
			while( NIS_NODE_VALID(puichild,&pui->ChildrenList) )
			{
				NUIClearWatchStackPush(puichild);
				stacksize ++;
				puichild = (NUI*)NGET_NEXT_NODE(puichild);
			}

			// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
			// ( because it could be deleted during the process ... ) 
			// Call them from the first to the last in that order ...
			// that means from the Last UI Children to the First ( due to the way they were pushed into the array)
			while(stacksize)
			{
				if( puichild = NUIClearWatchStackPop() )
				{
					NDisableUI(puichild);
				}
				stacksize--;
			}
			#ifdef _DEBUG
			NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
			#endif

			if(NUIClearWatchStackPop()!=pui)
				return NFALSE; // UI is not Enable, is deleted !!!
			else
				return NTRUE;
		}
		else
		{
			return NTRUE;
		}
	}
	else
	{
		return NTRUE;
	}
}
// ------------------------------------------------------------------------------------------
// Nbool NShowUI
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//	UI Visibility Status after process.
//		NFALSE	if UI still hidden !
//		NTRUE	if UI is showed !
//
//		 1 if UI visibility changes from HIDE to SHOW.
//		 0 if UI visibility still HIDDEN or if UI is deleted ( due to user nested calls of NHideUI or NDeleteUI )
// ------------------------------------------------------------------------------------------
Nbool NShowUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);

	// Is UI Hidden ?
	if( ISFLAG_OFF( pui->Flags,FLAG_NUI_SHOW ) )
	{
		// 0) UI PArent (if it exists) must be Visible !
		if(pui->pParent && ISFLAG_OFF( ((NUI*)pui->pParent)->Flags,FLAG_NUI_SHOW ) )
		{
			NErrorIf(1, NERROR_UI_PARENT_OF_VISIBLE_UI_MUST_BE_VISIBLE);
			return NFALSE;
		}

		NErrorIf(pui->pRenderable->Update_FCT != NRenderableUpdate_UI_Hidden && pui->pRenderable->Update_FCT != NRenderableUpdateWithoutExtract_UI_Hidden, NERROR_UI_SHOWED_MUST_HAVE_HIDDEN_RENDER_UPDATE_FCT);
		
		// 1a) Flag UI as "Showed"
		//		It's important to setup it Before sending UICore Event ...
		//		Like this, for user inside the UICore Event Response, NIsUIVisible(pui) will return NTRUE 
		FLAG_ON( pui->Flags,FLAG_NUI_SHOW );
		
		// 1b) Update Renderable update function
		if(pui->pRenderable->Update_FCT == NRenderableUpdate_UI_Hidden)
			pui->pRenderable->Update_FCT = NRenderableUpdate_UI;
		else if(pui->pRenderable->Update_FCT == NRenderableUpdateWithoutExtract_UI_Hidden)
			pui->pRenderable->Update_FCT = NRenderableUpdateWithoutExtract_UI;
		#ifdef _DEBUG
		else
		{
			NErrorIf(1,NERROR_SYSTEM_CHECK);// UNKNOWN UI Renderable Update function !!!
		}
		#endif
		
		// 1c) if UI is a Root, insert it at the End of the Active UI Root List !!!
		if(((NHIERARCHY_NODE*)pui)->pParent == NULL )
		{
			NArrayPushBack(&ActiveUIRootsList,(NBYTE*)&pui);

			// Update FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN if necessary ...
			if( !FLAGS_TEST(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,0) )
				FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
		}

		// 2a) Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		NEVENT nevent;
		Nmem0(&nevent,NEVENT);
		NFillupEventHead(nevent,NUI_SHOW,0,NEVENT_RECIPIENT_CAST_UI,pui);
		NSendUICoreEvent(&nevent);
		// 2b) Check for a potential nested Delete or a nested NHideUI call
		if( NUIClearWatchStackPop()!= pui || ISFLAG_OFF( pui->Flags,FLAG_NUI_SHOW ) )
		{
			NErrorIf(_is_ui_in_root_list(pui), NERROR_SYSTEM_CHECK);
			return NFALSE; // UI is not Showed, is deleted or hidden !!!
		}

		// 3) Propagate to children ... if there are some ...
		if( NIS_NODE_VALID(pui->ChildrenList.pFirst,&pui->ChildrenList) )
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

			// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
			// Do this from the last to the first ...
			#ifdef _DEBUG	
			Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
			#endif
			Nu32 stacksize = 0;
			NUI *puichild = (NUI*)pui->ChildrenList.pFirst;
			while( NIS_NODE_VALID(puichild,&pui->ChildrenList) )
			{
				if( ISFLAG_ON(puichild->FlagsXtd,FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL) )
				{
					NUIClearWatchStackPush(puichild);
					stacksize ++;
				}

				puichild = (NUI*)NGET_NEXT_NODE(puichild);
			}

			// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
			// ( because it could be deleted during the process ... ) 
			// Call them from the first to the last in that order ...
			// that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
			while(stacksize)
			{
				if( puichild = NUIClearWatchStackPop() )
				{
					NShowUI(puichild);
				}
				stacksize--;
			}
			#ifdef _DEBUG
			NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
			#endif

			if(NUIClearWatchStackPop()!=pui)
				return NFALSE; // UI is not Showed, is deleted !!!
			else
				return NTRUE;
		}
		return NTRUE;
	}
	else
	{
		return NTRUE; // UI is already SHOWED. So return NTRUE.
	}
}


Nbool NHideUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);


	// Is UI "Showed" ?
	if( ISFLAG_ON( pui->Flags,FLAG_NUI_SHOW ) )
	{
		NUI *puichild;

		// 1a) 	Find all the 'pui' value in the Delayed Event Array and set them to NULL !
		//		if the event type is not compatible with a hidden UI ( _UICOMMAND event type is not )
		NDelayedUIEventArray_UIHide_Notification(pui);

		// 1b) Cancel all Potential Touch Intercepting/Listening.
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		
		NUITouchCancelAll(pui,NTRUE);	// Its a recursive process so UI and all it's children are all going to "loose" their touches, if they have some ones ...
										// ... all of them in this "NUITouchCancelAll" call. So recursive 'NUIHide' call is going to pass here too ... and make the call for nothing ... 	
		if(NUIClearWatchStackPop()!=pui)
			return NFALSE;

		NErrorIf(pui->pTouchUIListener, NERROR_SYSTEM_CHECK);
		
		// 2a) Flag UI as HIDDEN ...
		//		It's important to setup it Before sending UICore Event ...
		//		Like this, for user inside the UICore Event Response, NIsUIVisible(pui) will return NFALSE 
		FLAG_OFF( pui->Flags,FLAG_NUI_SHOW );
		// 2b) And update Renderable update function
		if(pui->pRenderable->Update_FCT == NRenderableUpdate_UI)
			pui->pRenderable->Update_FCT= NRenderableUpdate_UI_Hidden;
		else if(pui->pRenderable->Update_FCT == NRenderableUpdateWithoutExtract_UI)
			pui->pRenderable->Update_FCT= NRenderableUpdateWithoutExtract_UI_Hidden;
		#ifdef _DEBUG
		else
		{
			NErrorIf(1,NERROR_SYSTEM_CHECK);// UNKNOWN UI Renderable Update function !!!
		}
		#endif

		// 2c) And Remove it from Active UI Root List (... If it's necessary )
		Nu32	i;
		#ifdef _DEBUG
		Nbool debug_bfound = NFALSE;
		#endif
		if( ((NHIERARCHY_NODE*)pui)->pParent == NULL )
		{
			NUI		**ptr;
			ptr = (NUI**)NGetFirstArrayPtr(&ActiveUIRootsList);
			for( i=0;i<ActiveUIRootsList.Size;i++,ptr++)
			{	
				if(*ptr == pui)
				{
					#ifdef _DEBUG
					debug_bfound = NTRUE;
					#endif

					NEraseArrayElement(&ActiveUIRootsList,i,NULL);

					// Update FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN if necessary ...
					// If pui is flagged touch move In (one or two of the flags )
					if( !FLAGS_TEST(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,0) )
					{
						// Then check ActiveUIRootList without 'pui' inside... And update NEngineCoreFlags if necessary
						if( !NAreActiveUIRootsInterestedByTouchMoveIn() )
							FLAG_OFF( NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN );
					}
					break;
				}
			}
			#ifdef _DEBUG
			NErrorIf(!debug_bfound,NERROR_SYSTEM_CHECK);
			#endif
		}

		// 3) Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		NEVENT	nevent;
		Nmem0(&nevent,NEVENT);
		NFillupEventHead(nevent,NUI_HIDE,0,NEVENT_RECIPIENT_CAST_UI,pui);
		NSendUICoreEvent(&nevent);
		if( NUIClearWatchStackPop()!=pui || ISFLAG_ON( pui->Flags,FLAG_NUI_SHOW ) )
		{
			NErrorIf(_is_ui_in_root_list(pui), NERROR_SYSTEM_CHECK);
			return NFALSE; // UI is not Hidden, is deleted or showed !!! ... by some user nested calls of NHideUI / NDelete inside 'NSendUICoreEvent'
		}


		// 4) Propagate to children ... if there are some ...
		if( NIS_NODE_VALID(pui->ChildrenList.pFirst,&pui->ChildrenList) )
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

			// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
			// Do this from the first to the last ...
			#ifdef _DEBUG	
			Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
			#endif
			Nu32 stacksize = 0;
			puichild = (NUI*)pui->ChildrenList.pFirst;
			while( NIS_NODE_VALID(puichild,&pui->ChildrenList) )
			{
				NUIClearWatchStackPush(puichild);
				stacksize ++;
				puichild = (NUI*)NGET_NEXT_NODE(puichild);
			}

			// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
			// ( because it could be deleted during the process ... ) 
			// Call them from the first to the last in that order ...
			// that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
			while(stacksize)
			{
				if( puichild = NUIClearWatchStackPop() )
				{
					NHideUI(puichild); // NTRUE, because here we are sure 'force_children_visibility_state_to_hide is NTRUE' 
				}
				stacksize--;
			}
			#ifdef _DEBUG
			NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
			#endif
			
			if(NUIClearWatchStackPop()!=pui)
				return NFALSE; // UI is not Hidden, is deleted !!!
			else
				return NTRUE;
		}
		else
		{
			return NTRUE;
		}
	}
	else
	{
		return NTRUE;	 // UI is already HIDDEN. So return NTRUE.
	}
}

void	NInitializeActiveUIRootsList()
{
	NSetupArray( &ActiveUIRootsList,DEFAULT_NUI_ACTIVEROOTS_NUMBER,sizeof(NUI*) );
}

void	NClearActiveUIRootsList()
{
	NClearArray( &ActiveUIRootsList,NULL );
}

/*

Nu32 NHasUIForwardHierarchyTouchFocus(const NUI* pui)
{
	// Instead of starting from pui to parse all 'descending' hierarchy 
	// to look for for pFocusedUI, we are going to perform the research by starting
	// from pFocusedUI itself and go backward to see if pFocusedUI or one of it's parents
	// is 'pui'... and, in that case that means pFocusedUI is a child of pui or pui itself.
	// Doing in that way seem's faster because an UI as only one parent, but may have a lot of children !
	NUI *pfocuschain_ui = pFocusedUI;
	while(pfocuschain_ui)
	{
		if(pfocuschain_ui == pui)
			return 1;
		pfocuschain_ui = (NUI*)pfocuschain_ui->pParent;
	}
	return 0;
}

inline NUI_EVENT_RESULT N_TOUCH_KILL_SetupAndSend(NEVENT *pevent, const Nu32 which, const Nu32 infos)
{
	Nmem0(pevent,NEVENT);
	pevent->Type					= N_TOUCH_KILL;
	pevent->Touch.pUIRecipient		= pFocusedUI;
	pevent->Touch.Which				= which; // Doesn't work now but it will ...
	pevent->Touch.Infos				= infos;
	//nevent.Touch.Position;		
	//nevent.Touch.Relative;

	pFocusedUI						= NULL;
	NUI_EVENT_RESULT uiresult =  NSendUIEvent(pevent);
	NErrorIf(pFocusedUI != NULL, NERROR_UI_TOUCH_KILL_EVENT_IGNORED);
	return uiresult;
}

static Ns32 _HideUI_Recursive( NUI *pui)
{
	if(ISFLAG_ON( pui->Flags,FLAG_NUI_SHOW ))
	{
		NEVENT	nevent;
		NNODE	*pa;
		Nbool	_loop;

		FLAG_OFF( pui->Flags,FLAG_NUI_SHOW );

		// Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

		Nmem0(&nevent,NEVENT);
		nevent.Type	= N_UI_HIDE;
		nevent.UINotification.pUIRecipient = pui;
		NSendUIEvent(&nevent);

		if(NUIClearWatchStackPop()!=pui)
			return 0;

		NErrorIf(ISFLAG_ON( pui->Flags,FLAG_NUI_SHOW ),NERROR_UI_HIDE_EVENT_IGNORED);

		// ... ANd take care of its all children.
		// ... There are 2 nested loops to handle some tricky cases.
		// Example: Inside N_UI_HIDE processing of the first 'ui' child  user delete the second and the third 'ui' child
		//			but it still 2 others children available ( number 3 and 4 ). So, how to continue _HideUI_Recursive for them ???
		//			Thats the purpose of these 2 nested loops. Actually main concept is simple, we are going to parse AGAIN 'ui' children
		//			list from the beginning if a delete occurs, and in the main time verify 'ui' stills available too.
		//			By doing this some "non-deleted" children may be "visited" several times, but it doesn't matter, thanks to the flag
		//			FLAG_NUI_SHOW tested at the beginning of '_HideUI_Recursive' , we are sure that they are going to receive only one  N_UI_HIDE event.
		do 
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			_loop = NFALSE;
			pa=(NNODE*)((NHIERARCHY_NODE*)pui)->ChildrenList.pFirst;
			while(pa!=(NNODE*)&((NHIERARCHY_NODE*)pui)->ChildrenList)
			{
				if( _HideUI_Recursive((NUI*)pa) )
				{
					pa = (NNODE*)(pa->pNext);
				}
				else
				{
					_loop = NTRUE;
					break;
				}

			}
			if(NUIClearWatchStackPop()!=pui)
				return 0;
		} while(_loop);

		return 1;
	}
	return 1;
}

NUI* NHideUI(NUI *pui)
{
	// UI is already inactive
	if( ISFLAG_ON( pui->Flags,FLAG_NUI_SHOW ) )
	{
		NEVENT	nevent;
		NUI		**ptr;
		Nu32	index;

		// Is it necessary to KILL the Touch Focus ?
		// N will send a N_TOUCH_KILL event to the current Focused UI
		// SOME DETAILS ABOUT A 'TRICKY' CASE:
		//
		// 1/ user calls	NHideUI(ui)
		// 2/				NHideUI(ui) send an event	N_TOUCH_KILL to a focused child of ui or to ui ( during the 'Forward Hierarchy kill TouchFocus process' )
		// 3/											Processing N_TOUCH_KILL event calls NDeleteUI(ui)
		// 4/																				NDeleteUI(ui) calls NHideUI(ui) !!!
		// 5/																				... But this second time, pFocusedUI is different, and not
		//																					... So, +
		//																							|
		// +----------------------------------------------------------------------------------------+
		// |
		// 6/ This second call of NHideUI(ui) doesn't engage any N_TOUCH_KILL event, and ui is "hidden".
		// 7/ ui is deleted
		// 8/ return back to the N_TOUCH_KILL process, and return 'inside' the First call of NHideUI(ui) during the 
		//    descending hierarchy kill focus loop.
		// 9/ ui is deleted so, it's references inside UIClearWatchStack are NULL ! So ... NHideUI(ui) returns.

		// Function will first send a N_TOUCH_KILL event to the current Focused UI if this one is a part of the .
		// Forward hierarchy of 'pui'
		if( NHasUIForwardHierarchyTouchFocus(pui) )
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			N_TOUCH_KILL_SetupAndSend(&nevent,CONSTANT_ALL_TOUCH_IDS, N_TOUCH_KILL_FROM_FCT_NHideUI);
			if(NUIClearWatchStackPop()!=pui)
				return NULL;
		}
		// From here, we are sure of 2 things: 
		//
		// I/	"pui" and it's descending Hierarchy doesn't have the Touch Focus, even if there is an UI with the Touch Focus ! 
		//		(in that case we are sure this Focused UI is not in the descending hierarchy of "pui") 
		//
		// II/	"pui" still available ( it was not deleted during the potential 'Forward Hierarchy kill TouchFocus process'.
		//
		// Is UI a Root ?
		if( ((NHIERARCHY_NODE*)pui)->pParent == NULL )
		{
#ifdef _DEBUG
			Nbool bfound = NFALSE;
#endif
			// We look for the UI into the table to remove it
			ptr = (NUI**)NGetFirstArrayPtr(&ActiveUIRootsList);
			for( index=0;index<ActiveUIRootsList.Size;index++,ptr++)
			{	
				if(*ptr == pui)
				{
					NEraseArrayElement(&ActiveUIRootsList,index,NULL);
#ifdef _DEBUG
					bfound = NTRUE;
#endif
					break;
				}
			}
#ifdef _DEBUG
			NErrorIf(!bfound, NERROR_SYSTEM_CHECK);
#endif
		}
		// Send Notification Events to hierarchy 
		if( _HideUI_Recursive(pui) )
		{
			return pui;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return pui;
	}
}

static Ns32 _ShowUI_Recursive( NUI *pui)
{
	if(ISFLAG_OFF( pui->Flags,FLAG_NUI_SHOW ))
	{
		NEVENT	nevent;
		NNODE	*pa;
		Nbool	_loop;

		FLAG_ON( pui->Flags,FLAG_NUI_SHOW );

		// Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

		Nmem0(&nevent,NEVENT);
		nevent.Type	= N_UI_SHOW;
		nevent.UINotification.pUIRecipient = pui;
		NSendUIEvent(&nevent);

		if(NUIClearWatchStackPop()!=pui)
			return 0;

		NErrorIf(ISFLAG_OFF( pui->Flags,FLAG_NUI_SHOW ),NERROR_UI_SHOW_EVENT_IGNORED);

		// ... ANd take care of its all children.
		// ... There are 2 nested loops to handle some tricky cases.
		// Example: Inside N_UI_HIDE processing of the first 'ui' child  user delete the second and the third 'ui' child
		//			but it still 2 others children available ( number 3 and 4 ). So, how to continue _HideUI_Recursive for them ???
		//			Thats the purpose of these 2 nested loops. Actually main concept is simple, we are going to parse AGAIN 'ui' children
		//			list from the beginning if a delete occurs, and in the main time verify 'ui' stills available too.
		//			By doing this some "non-deleted" children may be "visited" several times, but it doesn't matter, thanks to the flag
		//			FLAG_NUI_SHOW tested at the beginning of '_ShowUI_Recursive' we are sure that they are going to receive only one  N_UI_SHOW event..
		do 
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			_loop = NFALSE;
			pa=(NNODE*)((NHIERARCHY_NODE*)pui)->ChildrenList.pFirst;
			while(pa!=(NNODE*)&((NHIERARCHY_NODE*)pui)->ChildrenList)
			{
				if( _ShowUI_Recursive((NUI*)pa) )
				{
					pa = (NNODE*)(pa->pNext);
				}
				else
				{
					_loop = NTRUE;
					break;
				}

			}
			if(NUIClearWatchStackPop()!=pui)
				return 0;
		} while(_loop);

		return 1;
	}
	return 1;
}

NUI* NShowUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);

	if(pui->pParent && ISFLAG_OFF( ((NUI*)pui->pParent)->Flags,FLAG_NUI_SHOW ) )
	{
		NErrorIf(1, NERROR_UI_PARENT_OF_VISIBLE_UI_MUST_BE_VISIBLE);
		return pui;
	}

	// UI is already active
	if( ISFLAG_OFF( pui->Flags,FLAG_NUI_SHOW ) )
	{
		// Send Notification Events to hierarchy 
		if( _ShowUI_Recursive(pui) )
		{
			// UI is a Root
			if( ((NHIERARCHY_NODE*)pui)->pParent == NULL )
			{
				NArrayPushBack(&ActiveUIRootsList,(NBYTE*)&pui);
			}
			return pui;
		}
		// _ShowUI_Recursive return 0, that means pui doesn't exist anymore ...
		else
		{
			return NULL;
		}
	}
	else
	{
		return pui;
	}
}

static Ns32 _DisableUI_Recursive( NUI *pui)
{
	if(ISFLAG_ON( pui->Flags,FLAG_NUI_ENABLE ))
	{
		NEVENT	nevent;
		NNODE	*pa;
		Nbool	_loop;

		FLAG_OFF( pui->Flags,FLAG_NUI_ENABLE );

		// Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

		Nmem0(&nevent,NEVENT);
		nevent.Type	= N_UI_DISABLE;
		nevent.UINotification.pUIRecipient = pui;
		NSendUIEvent(&nevent);

		if(NUIClearWatchStackPop()!=pui)
			return 0;

		NErrorIf(ISFLAG_ON( pui->Flags,FLAG_NUI_ENABLE ),NERROR_UI_DISABLE_EVENT_IGNORED);

		// ... ANd take care of its all children.
		// ... There are 2 nested loops to handle some tricky cases.
		// Example: Inside N_UI_HIDE processing of the first 'ui' child  user delete the second and the third 'ui' child
		//			but it still 2 others children available ( number 3 and 4 ). So, how to continue _HideUI_Recursive for them ???
		//			Thats the purpose of these 2 nested loops. Actually main concept is simple, we are going to parse AGAIN 'ui' children
		//			list from the beginning if a delete occurs, and in the main time verify 'ui' stills available too.
		//			By doing this some "non-deleted" children may be "visited" several times, but it doesn't matter, thanks to the flag
		//			FLAG_NUI_ENABLE tested at the beginning of '_HideUI_Recursive'
		do 
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			_loop = NFALSE;
			pa=(NNODE*)((NHIERARCHY_NODE*)pui)->ChildrenList.pFirst;
			while(pa!=(NNODE*)&((NHIERARCHY_NODE*)pui)->ChildrenList)
			{
				if( _DisableUI_Recursive((NUI*)pa) )
				{
					pa = (NNODE*)(pa->pNext);
				}
				else
				{
					_loop = NTRUE;
					break;
				}

			}
			if(NUIClearWatchStackPop()!=pui)
				return 0;
		} while(_loop);

		return 1;
	}
	return 1;
}

NUI* NDisableUI(NUI *pui)
{
	// UI is already Disable
	if( ISFLAG_ON( pui->Flags,FLAG_NUI_ENABLE ) )
	{
		NEVENT	nevent;

		// Is it necessary to KILL the Touch Focus ?
		// N will send a N_TOUCH_KILL event to the current Focused UI
		//
		// SOME DETAILS ABOUT A 'TRICKY' CASE:
		//
		// 1/ user calls	NHideUI(ui)
		// 2/				NHideUI(ui) send an event	N_TOUCH_KILL to a focused child of ui or to ui ( during the 'Forward Hierarchy kill TouchFocus process' )
		// 3/											Processing N_TOUCH_KILL event calls NDeleteUI(ui)
		// 4/																				NDeleteUI(ui) calls NHideUI(ui) !!!
		// 5/																				... But this second time, pFocusedUI is different, and not
		//																					... So, +
		//																							|
		// +----------------------------------------------------------------------------------------+
		// |
		// 6/ This second call of NHideUI(ui) doesn't engage any N_TOUCH_KILL event, and ui is "hidden".
		// 7/ ui is deleted
		// 8/ return back to the N_TOUCH_KILL process, and return 'inside' the First call of NHideUI(ui) during the 
		//    descending hierarchy kill focus loop.
		// 9/ ui is deleted so, it's references inside UIClearWatchStack are NULL ! So ... NHideUI(ui) returns.

		if( NHasUIForwardHierarchyTouchFocus(pui) )
		{
			// Function will first send a N_TOUCH_KILL event to the current Focused UI.
			// Forward hierarchy must be without any Touch Focus
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			N_TOUCH_KILL_SetupAndSend(&nevent,CONSTANT_ALL_TOUCH_IDS, N_TOUCH_KILL_FROM_FCT_NDisableUI);
			if(NUIClearWatchStackPop()!=pui)
				return NULL;
		}
		// From here, we are sure of 2 things: 
		//
		// I/	"pui" and it's descending Hierarchy doesn't have the Touch Focus, even if there is an UI with the Touch Focus ! 
		//		(in that case we are sure this Focused UI is not in the descending hierarchy of "pui") 
		//
		// II/	"pui" still available ( it was not deleted during the potential 'Forward Hierarchy kill TouchFocus process'.
		//

		// Send Notification Events to hierarchy 
		if( _DisableUI_Recursive(pui) )
		{
			return pui;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return pui;
	}
}

static Ns32 _EnableUI_Recursive( NUI *pui)
{
	if(ISFLAG_OFF( pui->Flags,FLAG_NUI_ENABLE ))
	{
		NEVENT	nevent;
		NNODE	*pa;
		Nbool	_loop;

		FLAG_ON( pui->Flags,FLAG_NUI_ENABLE );

		// Send Notification Event to UI
		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 

		Nmem0(&nevent,NEVENT);
		nevent.Type	= N_UI_ENABLE;
		nevent.UINotification.pUIRecipient = pui;
		NSendUIEvent(&nevent);

		if(NUIClearWatchStackPop()!=pui)
			return 0;

		NErrorIf(ISFLAG_OFF( pui->Flags,FLAG_NUI_ENABLE ),NERROR_UI_ENABLE_EVENT_IGNORED);

		// ... ANd take care of its all children.
		// ... There are 2 nested loops to handle some tricky cases.
		// Example: Inside N_UI_HIDE processing of the first 'ui' child  user delete the second and the third 'ui' child
		//			but it still 2 others children available ( number 3 and 4 ). So, how to continue _HideUI_Recursive for them ???
		//			Thats the purpose of these 2 nested loops. Actually main concept is simple, we are going to parse AGAIN 'ui' children
		//			list from the beginning if a delete occurs, and in the main time verify 'ui' stills available too.
		//			By doing this some "non-deleted" children may be "visited" several times, but it doesn't matter, thanks to the flag
		//			FLAG_NUI_SHOW tested at the beginning of '_ShowUI_Recursive' we are sure that they are going to receive only one  N_UI_SHOW event..
		do 
		{
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			_loop = NFALSE;
			pa=(NNODE*)((NHIERARCHY_NODE*)pui)->ChildrenList.pFirst;
			while(pa!=(NNODE*)&((NHIERARCHY_NODE*)pui)->ChildrenList)
			{
				if( _EnableUI_Recursive((NUI*)pa) )
				{
					pa = (NNODE*)(pa->pNext);
				}
				else
				{
					_loop = NTRUE;
					break;
				}

			}
			if(NUIClearWatchStackPop()!=pui)
				return 0;
		} while(_loop);

		return 1;
	}
	return 1;
}

NUI* NEnableUI(NUI *pui)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);

	if(pui->pParent && ISFLAG_OFF( ((NUI*)pui->pParent)->Flags,FLAG_NUI_ENABLE ) )
	{
		NErrorIf(1, NERROR_UI_ENABLE_UI_PARENT_MUST_BE_ENABLE);
		return pui;
	}

	// UI is already Enable
	if( ISFLAG_OFF( pui->Flags,FLAG_NUI_ENABLE ) )
	{
		// Send Notification Events to hierarchy 
		if( _EnableUI_Recursive(pui) )
		{
			return pui;
		}
		// _EnableUI_Recursive return 0, that means pui doesn't exist anymore ...
		else
		{
			return NULL;
		}
	}
	else
	{
		return pui;
	}
}
*/
