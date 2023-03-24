#ifdef _NWINDOWS

#include "../NCStandard.h"
#include "../Containers/NRingBuffer.h"
#include "../Containers/NNode.h"
#include "../Containers/NArray.h"


#include "../NErrorHandling.h"
#include "../NCoordinateSystemConversions.h"
#include "NTouchStream.h"
#include "NTouchUIListener.h"
#include "NTouch.h"

#include "NTouchEmulation.h"

NARRAY	NTouchEmulArray;

void NTouchEmulInit(const Nu32 capacity)
{
	NSetupArray(&NTouchEmulArray,capacity,sizeof(NTOUCH_EMUL));
}

void NTouchEmulQuit()
{
	NErrorIf(NTouchEmulArray.Size, NERROR_NON_NULL_VALUE); // Touch Emul Array should be empty !!!
	NClearArray(&NTouchEmulArray,NULL);
}

NTOUCH_EMUL*	NSetupTouchEmul(NTOUCH_EMUL *ptouchemul, const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y)
{
	// Checks
#ifdef _DEBUG
	NErrorIf(!ptouchemul, NERROR_NULL_POINTER);
	NErrorIf(keycode == VK_ESCAPE,	NERROR_SYSTEM_RESERVED_VALUE);	//	_ Key associated with a TouchEmul structure must be different than ...
	NErrorIf(keycode == VK_TAB,		NERROR_SYSTEM_RESERVED_VALUE);	//	...the ones already used by system to emulate natively core functionalities
	NErrorIf( wincoord_hrel_x < 0.0f || wincoord_hrel_x > NCoord_NormToHRel_X(1.0f),  NERROR_VALUE_OUTOFRANGE);  // x touch has to stay "inside" regular screen width  ( 0, 1 )
	NErrorIf( wincoord_hrel_y < NCoord_NormToHRel_Y(-1.0f) || wincoord_hrel_y > 0.0f,  NERROR_VALUE_OUTOFRANGE); // y touch has to stay "inside" regular screen height (-1,0)
#endif

	ptouchemul->Flags			= 0;
	ptouchemul->KeyCode			= (Nu32)keycode;
	ptouchemul->HRelPosition.x	= wincoord_hrel_x;
	ptouchemul->HRelPosition.y	= wincoord_hrel_y;

	return ptouchemul;
}

NTOUCH_EMUL*	NCreateTouchEmul(const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y)
{
	return NSetupTouchEmul(NEW(NTOUCH_EMUL),keycode,wincoord_hrel_x,wincoord_hrel_y);
}

void			NClearTouchEmul(NTOUCH_EMUL *ptouchemul)
{
	Nmem0(ptouchemul,NTOUCH_EMUL);
}

void			NDeleteTouchEmul(NTOUCH_EMUL *ptouchemul)
{
	NClearTouchEmul(ptouchemul);
	Nfree(ptouchemul);
}


void	NBindTouchEmul(const Nu32 keycode, const Nf32 wincoord_hrel_x, const Nf32 wincoord_hrel_y)
{
	NSetupTouchEmul((NTOUCH_EMUL*)NArrayAllocBack(&NTouchEmulArray),keycode,wincoord_hrel_x,wincoord_hrel_y);
}

void	NUnbindTouchEmul( const Nu32 keycode )
{
	NTOUCH_EMUL *ptm = (NTOUCH_EMUL*)NTouchEmulArray.pFirst;
	for(Ns32 i=0;i<(Ns32)NTouchEmulArray.Size;i++,ptm ++)
	{
		if(ptm->KeyCode == keycode)
		{
			NErrorIf(ISFLAG_ON(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED), NERROR_TOUCH_EMUL_HAS_TO_BE_UNUSED);
			NQuickEraseArrayElementPtr(&NTouchEmulArray,(NBYTE*)ptm,NULL);
			i--;ptm--;
		}
	}
}

void	NUnbindAllTouchEmul()
{
#ifdef _DEBUG
	NTOUCH_EMUL *ptm = (NTOUCH_EMUL*)NTouchEmulArray.pFirst;
	for(Ns32 i=0;i<(Ns32)NTouchEmulArray.Size;i++,ptm ++)
	{

		// It seems a Touch emul stills activated, that means the associated KEY is currently DOWN ...
		NErrorIf(ISFLAG_ON(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED), NERROR_TOUCH_EMUL_HAS_TO_BE_UNUSED);
	}
#endif
	NEraseArray(&NTouchEmulArray,NULL);
}
#endif // _NWINDOWS
