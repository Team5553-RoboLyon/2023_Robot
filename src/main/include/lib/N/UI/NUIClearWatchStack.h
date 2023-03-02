#ifndef __NUI_CLEAR_WATCH_STACK_H
#define __NUI_CLEAR_WATCH_STACK_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NUIClearWatchStack.h								**
// **																					**
// **						LOW LEVEL MECHANISM used by Engine							**
// **							 schouldn't by used by User								**
// **																					**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// UI constants
#define	CONSTANT_UICLEARWATCHSTACK_INITIAL_CAPACITY						8 // Capacity used to setup the 'UIClearWatchStack' Array.

void NInitUIClearWatchStack();
void NQuitUIClearWatchStack();

void NUIClearWatchStackPush(const NUI *pui);
NUI* NUIClearWatchStackPop();
void NUIClearWatchStackPoke(const NUI *pui);
NUI* NUIClearWatchStackPeek();
void NUIClearWatchStackRemove();

NUI* NUIClearWatchStackPeekI(const Nu32 index);
void NDecreaseUIClearWatchStackSize(const Nu32 decrease);
void NSetUIClearWatchStackSize(const Nu32 size);
Nu32 NGetUIClearWatchStackSize();

void NUIClearWatchStack_ClearNotification(const NUI *pui);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUI_CLEAR_WATCH_STACK_H 

