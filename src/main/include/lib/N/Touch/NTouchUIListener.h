#ifndef __NTOUCH_UI_LISTENER_H
#define __NTOUCH_UI_LISTENER_H

//								 PRIVATE N INTERNAL ONLY
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							 NTouchCore_UIListener.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../Containers/NHNode.h"
#include "NTouchListener.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Use the NTOUCH_LISTENER.Flags to specify UI is listening hidden, because of it's children.
	// The only way for an UI to have a NTOUCH_UI_LISTENER is to capture touch at "Start" or/and at "Move In".
	// AND ...
	//	1) Being an INTERCEPTOR
	//	2) or being a leaf listener !
	//
	// But there is an exception: When UI has children capable of touch Move In Capture.
	// In that case, UI needs to have a NTOUCH_UI_LISTENER too.
	// This one will be flagged "HIDDEN" to allow engine recognizing the situation.

	// #define FLAG_NTOUCH_UI_LISTENER_NTOUCH_LISTENER_HIDDEN			BIT_4

	typedef struct NUI NUI;
	typedef struct
	{
		NHIERARCHY_NODEDEF
		NUI *pUI;
		NTOUCH_LISTENER Listener;
	} NTOUCH_UI_LISTENER;

	NTOUCH_UI_LISTENER *NSetupTouchUIListener(NTOUCH_UI_LISTENER *pui_listener);
	NTOUCH_UI_LISTENER *NCreateTouchUIListener();
	void NClearTouchUIListener(NTOUCH_UI_LISTENER *pui_listener);
	void NDeleteTouchUIListener(NTOUCH_UI_LISTENER *pui_listener);
	NTOUCH_UI_LISTENER *NBindTouchUIListener(NUI *pui);
	void NUnbindTouchUIListener(NTOUCH_UI_LISTENER *puilistener);

#ifdef _DEBUG
	void NDrawUIListenerInfos(const Nf32 x, const Nf32 y);
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NTOUCH_UI_LISTENER_H
