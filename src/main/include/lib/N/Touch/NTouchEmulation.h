// WINDOWS ONLY !!! WINDOWS ONLY !!! WINDOWS ONLY !!! 
#ifdef _NWINDOWS

#ifndef __NTOUCH_EMULATION_H
#define __NTOUCH_EMULATION_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NTouchEmulation.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define FLAG_NTOUCH_EMUL_TOUCH_STARTED	BIT_0		// Set to ON when "Player" press the key describes into NTOUCH_EMUL

typedef struct  
{
	Nu32		Flags;

	Nu32		KeyCode;		// Associated keyboard key
	NVEC2	HRelPosition;	// Touch Position in WinCoord HRel system
}NTOUCH_EMUL;

void			NBindTouchEmul(const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y);
void			NUnbindTouchEmul( const Nu32 keycode );
void			NUnbindAllTouchEmul();

// Private/Internal Use
void NTouchEmulInit(const Nu32 capacity);
void NTouchEmulQuit();

NTOUCH_EMUL*	NSetupTouchEmul(NTOUCH_EMUL *ptouchemul, const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y);
NTOUCH_EMUL*	NCreateTouchEmul(const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y);
void			NClearTouchEmul(NTOUCH_EMUL *ptouchemul);
void			NDeleteTouchEmul(NTOUCH_EMUL *ptouchemul);


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NTOUCH_EMULATION_H 

#endif //  __NWINDOWS
