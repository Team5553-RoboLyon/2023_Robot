// EventBuffer doesn't exist on windwos. instead, NEVENT are translated one by one from Windows Event Buffer ...

//#if defined _NIOS || defined _NANDROID    
#ifndef __N_EVENT_BUFFER_H
#define __N_EVENT_BUFFER_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NEventBuffer.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "./NEvent.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CONSTANT_EVENT_BUFFER_CAPACITY		32

void		NInitEventBuffer();
void		NQuitEventBuffer();

NEVENT*		NEventAllocBack();

NEVENT*		NPollEvent();
void		NFreePolledEvent();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NEVENT_BUFFER_H 
//#endif //_NIOS || _NANDROID    
