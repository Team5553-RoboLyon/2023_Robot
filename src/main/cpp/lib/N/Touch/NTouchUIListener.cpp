#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "NTouchUIListener.h"
#include "../UI/NUI.h"

// UI Touch Listening Management.
// These 2 NODE LISTS store the NTOUCH_UI_LISTENER structures used and referenced by UIs to Listen Touch as quick as possible.
extern NNODELIST UIListener_List;
extern NNODELIST AvailableUIListener_List;
extern Nu8 NTouchUICompatibility;

#ifdef _DEBUG
#include "lib/N/Utilities/Draw/NUT_Draw.h"

static void NDrawUIListenerInfos_Recursive(NTOUCH_UI_LISTENER *plistener, const Nf32 x, const Nf32 y)
{
	Nchar bld[128] = {0};
	Nchar txt[1024] = {0};

	if (plistener->pUI->pName)
		sprintf(bld, "List-'%s' ->Siz:%d - Count:%d - PrvCount: %d CountIn: %d PrvCountIn: %d Cap: %d \n", plistener->pUI->pName, plistener->Listener.LTouchBufferSize, plistener->Listener.LTouchCount, plistener->Listener.PreviousLTouchCount, plistener->Listener.LTouchCountIn, plistener->Listener.PreviousLTouchCountIn, plistener->Listener.LTouchBufferCapacity);
	else
		sprintf(bld, "List-'NoName' ->Siz:%d - Count:%d - PrvCount: %d CountIn: %d PrvCountIn: %d Cap: %d \n", plistener->Listener.LTouchBufferSize, plistener->Listener.LTouchCount, plistener->Listener.PreviousLTouchCount, plistener->Listener.LTouchCountIn, plistener->Listener.PreviousLTouchCountIn, plistener->Listener.LTouchBufferCapacity);

	strcat(txt, bld);

	NVEC3 pos;
	NVec3Set(&pos, x, y, 0);
	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
	NUT_Draw_Text(txt, &pos, NULL);

	NTOUCH_UI_LISTENER *pchildlistener = (NTOUCH_UI_LISTENER *)plistener->ChildrenList.pLast;
	while (NIS_NODE_VALID(pchildlistener, &plistener->ChildrenList))
	{
		NDrawUIListenerInfos_Recursive(pchildlistener, x + 0.05f, y - 0.05f);
		pchildlistener = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(pchildlistener);
	}
}
void NDrawUIListenerInfos(const Nf32 x, const Nf32 y)
{
	NTOUCH_UI_LISTENER *plistener = (NTOUCH_UI_LISTENER *)UIListener_List.pLast;
	while (NIS_NODE_VALID(plistener, &UIListener_List))
	{
		NDrawUIListenerInfos_Recursive(plistener, x, y);
		plistener = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(plistener);
	}
}
#endif

NTOUCH_UI_LISTENER *NSetupTouchUIListener(NTOUCH_UI_LISTENER *pui_listener)
{
	NErrorIf(!pui_listener, NERROR_NULL_POINTER);
	Nmem0(pui_listener, NTOUCH_UI_LISTENER);
	NSetupTouchListener(&pui_listener->Listener, 0); // No buffer allocation at creation, function "NSetListenerTouchCountMax" will do the job !
	// Hierarchy management:
	NSetupHNode((NHIERARCHY_NODE *)pui_listener);
	return pui_listener;
}

NTOUCH_UI_LISTENER *NCreateTouchUIListener()
{
	return NSetupTouchUIListener(NEW(NTOUCH_UI_LISTENER));
}

void NClearTouchUIListener(NTOUCH_UI_LISTENER *pui_listener)
{
	// A listener available for clearing must be:
	//		Without any "parent"
	//		Without any children
	//		Without any link with "UI"
	//		Without any Listened Touch registered ( Listener Clear function will handle that point )
	NErrorIf(pui_listener->pParent || pui_listener->pUI, NERROR_SYSTEM_CHECK);
	NErrorIf(NIS_NODE_VALID(pui_listener->ChildrenList.pFirst, &pui_listener->ChildrenList), NERROR_SYSTEM_CHECK);

	// It could be "in" or "out" of the Root list (which is "UIListener_List")
	if (pui_listener->pPrevious || pui_listener->pNext)
	{
		NNodeRemove((NNODE *)pui_listener);
	}
	NClearTouchListener(&pui_listener->Listener);
	Nmem0(pui_listener, NTOUCH_UI_LISTENER);
}

void NDeleteTouchUIListener(NTOUCH_UI_LISTENER *pui_listener)
{
	NClearTouchUIListener(pui_listener);
	Nfree(pui_listener);
}

NTOUCH_UI_LISTENER *NBindTouchUIListener(NUI *pui)
{
	NErrorIf(pui->pTouchUIListener, NERROR_NON_NULL_POINTER);

	NTOUCH_UI_LISTENER *plistener;

	//	1)	Try to get this NEW Listener from the AvailableUIListener_List, where all the unused ones
	//		are stored instead of being deleted ( to avoid Memory fragmentation )
	if (NIS_NODE_VALID(AvailableUIListener_List.pFirst, &AvailableUIListener_List))
	{
		plistener = (NTOUCH_UI_LISTENER *)AvailableUIListener_List.pFirst;
		NErrorIf(plistener->pParent, NERROR_NON_NULL_POINTER); // It has to be NULL !!!
		NNodeRemove((NNODE *)plistener);
	}
	//	2)	But if the AvailableUIListener_List is empty, just create a NEW Listener !
	else
	{
		plistener = (NTOUCH_UI_LISTENER *)NCreateTouchUIListener();
	}

	// BIND pui and the NEW Listener
	plistener->pUI = pui;
	pui->pTouchUIListener = plistener;

	// Get Back TouchCountMax
	NSetTouchListenerTouchCountMax(&plistener->Listener, pui->TouchCountMax);

	// Get Back Touch capture mode (at Start or on the Fly),...
	// Even if UI touch process uses directly  FLAG_NUI_CAPTURE_TOUCH_START instead of it !!!
	// But it's to stay consistent !!!!
	NErrorIf(plistener->Listener.Flags != 0, NERROR_SYSTEM_CHECK);

	// Hierarchy management:
	//		a)	Listener is consider as a "root" into Listener Hierarchy if none of the Parents of the UI backward hierarchy have a Listener.
	//		b)	Listener is consider as a "child" into Listener Hierarchy if ONE Parent of the UI backward hierarchy is linked with, have a Listener.
	NTOUCH_UI_LISTENER *plistener_parent = NULL;
	NUI *pparent = (NUI *)pui->pParent;
	while (pparent)
	{
		if ((plistener_parent = pparent->pTouchUIListener)) // !!! = and NOT == because we want retrieve "pparent->pTouchUIListener"
			break;
		else
			pparent = (NUI *)pparent->pParent;
	}

	if (plistener_parent)
	{
		NHNodeSetParent_BeginningInsert((NHIERARCHY_NODE *)plistener_parent, (NHIERARCHY_NODE *)plistener);
	}
	else
	{
		NNodeInsertBeginning((NNODE *)plistener, &UIListener_List);
	}

	return plistener;
}

void NUnbindTouchUIListener(NTOUCH_UI_LISTENER *puilistener)
{
	NErrorIf(!puilistener, NERROR_NULL_POINTER);
	NErrorIf(!puilistener->pUI, NERROR_INCONSISTENT_VALUES);																																																																		  // a listener to unbind need to be bound to a pui !!!!
	NErrorIf(puilistener->pUI->pTouchUIListener != puilistener, NERROR_INCONSISTENT_VALUES);																																																										  // a listener to unbind need to be bound to a pui with reciprocity !!!!
	NErrorIf(NHAS_HNODE_CHILDREN(puilistener), NERROR_SYSTEM_CHECK);																																																																  // a listener to unbind can NOT have children !!!
	NErrorIf(ISFLAG_ON(puilistener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN) || puilistener->Listener.LTouchBufferSize || puilistener->Listener.LTouchCount || puilistener->Listener.PreviousLTouchCount || puilistener->Listener.LTouchCountIn || puilistener->Listener.PreviousLTouchCountIn, NERROR_INCONSISTENT_VALUES); // a listener to unbind can not have listened Touch Registered !!!!

	puilistener->pUI->pTouchUIListener = NULL;
	puilistener->pUI = NULL;
	puilistener->Listener.Flags = 0;
	puilistener->Listener.LTouchBufferSize = 0;
	puilistener->Listener.LTouchCount = 0;
	puilistener->Listener.PreviousLTouchCount = 0;
	puilistener->Listener.LTouchCountIn = 0;
	puilistener->Listener.PreviousLTouchCountIn = 0;
	puilistener->Listener.LTouchCountMax = 0; // Reset to 0, just to be clean.
	// 	puilistener->Listener.pLTouchBuffer				= keep its current value (buffer still the same !)
	// 	puilistener->Listener.LTouchBufferCapacity		= keep its current value (buffer still the same !)

	// Hierarchy management:
	if (puilistener->pParent)
		NHNodeSetParent_NULL((NHIERARCHY_NODE *)puilistener);
	else if (puilistener->pNext) // or ->pPrevious
		NNodeRemove((NNODE *)puilistener);

	NNodeInsertEnd((NNODE *)puilistener, &AvailableUIListener_List);

	// Reset the Touch UI Compatibility value ...
	if (NIS_NODELIST_EMPTY(&UIListener_List))
		NTouchUICompatibility = 0;
// else ... Thanks to the "TouchCompatibility rules", all "puilistener->pUI->TouchCompatibility" are the same or = 0
//			so, if there are some Active UIListeners, their "puilistener->pUI->TouchCompatibility"  are the same and equal 'NTouchUICompatibility' or are = 0
//			so, there is nothing more to do, except a full check in debug mode ...
#ifdef _DEBUG
	else
	{
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Check the "TouchCompatibility rules"
		// Thanks to the "TouchCompatibility rules",
		// All "puil->pUI->TouchCompatibility" are the same and equal to NTouchUICompatibility, or are = 0
		NTOUCH_UI_LISTENER *puil = (NTOUCH_UI_LISTENER *)UIListener_List.pFirst;
		while (NIS_NODE_VALID(puil, &UIListener_List))
		{
			NErrorIf(puil->pUI->TouchCompatibility != NTouchUICompatibility && puil->pUI->TouchCompatibility != 0, NERROR_SYSTEM_CHECK);
			puil = (NTOUCH_UI_LISTENER *)NGET_NEXT_NODE(puil);
		}
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
#endif
}