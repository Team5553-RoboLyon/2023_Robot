#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/NCore.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/Core/NSafeConversion.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/Event/NEventBuffer.h"
#include "lib/N/Touch/NTouchStream.h"
#include "lib/N/Touch/NTouch.h"

extern Nu32 NEngineCoreFlags;

#ifdef _NWINDOWS
#include "lib/N/Touch/NTouchEmulation.h"
extern NARRAY NTouchEmulArray;
#endif

// Nu8 NTranslateEvent(const MSG *pmsg,NEVENT *pevent);

// ------------------------------------------------------------------------------------------
// Nu8	NWindowsPollEvent
// ------------------------------------------------------------------------------------------
// Description :
//	Retrieve the oldest pending event in the array and copy it into the pevent structure.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//	Returns 1 if there is a pending event or 0 if there are none available.
//
// ------------------------------------------------------------------------------------------
/*
Nu8	NWindowsPollEvent(NEVENT *pevent)
{
/ *
	SDL_Event		sdlevent;
	sdlevent.type	= SDL_NOEVENT;
	if( SDL_PollEvent(&sdlevent) )
	{
		return NTranslateEvent(&sdlevent,pevent);
	}
	else
		return 0;
* /
	MSG	msg = {0};
	if(GetMessage(&msg,NULL,0,0))
	{
		return NTranslateEvent(&msg,pevent);
	}
	else
		return 0;
}*/

// ------------------------------------------------------------------------------------------
// Nu8 NTranslateEvent
// ------------------------------------------------------------------------------------------
// Description :
//	Only for Windows. This function setup a NEVENT structure from an 'SDL_Event'
//
//	Notes about _TOUCH_EXTENDED:
//
//					This extension is not completely implemented ! (2017/04/11)
//					... and maybe it will never be !!!
//					The initial idea was having the lines bellow in PC implementation ( with full mouse event ) on user side:
//							switch(pevent->Type)
//							{
//								case NTOUCH_START:
//								case NTOUCH_START_X:
//									...
//									return NEVENT_RESULT_PROCESSED;
//
//								case NTOUCH_MOVE:
//								case NTOUCH_MOVE_X:
//									...
//									return NEVENT_RESULT_PROCESSED;
//
//								case NTOUCH_END:
//								case NTOUCH_END_X:
//									...
//									return NEVENT_RESULT_PROCESSED;
//
//								case NTOUCH_CANCEL:	!!! there is no NTOUCH_CANCEL_X because a cancel is for the whole NTOUCH with all its 'buttons'
//									...
//									return NEVENT_RESULT_PROCESSED;
//							}
//
//							BUT ! if the 'NTranslateEvent' is written for the NTOUCH_xxx_X event ...
//							... all the dispatch process needs to be update and extended !!!
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//		1 for translated
//		0 for non translated. That should be interpreted as NO EVENT !!!
// ------------------------------------------------------------------------------------------
#ifdef _NWINDOWS
Nu8 NCreateNEventFromMSG(const MSG *pmsg)
{
	static NVEC2s16 PreviousMousePosition;
	NEVENT *pevent;
	NTOUCH *ptouchstream;
	NTOUCH_EMUL *ptm;
	Nu32 i, keymatch;

#ifdef _NOUT_SYS_EVENT
	printf("\n\n -------------------------------------------------------------------------------------------------- ");
#endif
	switch (LOWORD(pmsg->message))
	{
	case WM_QUIT:
	{
#ifdef _NOUT_SYS_EVENT
		printf("\n> Translate WM_QUIT to NSYS_QUIT.");
#endif

		pevent = (NEVENT *)NEventAllocBack();
		pevent->Head.Type = NSYS_QUIT;
		pevent->Head.Param = 0;
		pevent->Head.pRecipient = NULL;
		pevent->Head.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;
	}
		return 1;

	case WM_KEYDOWN:
		switch (pmsg->wParam)
		{
		// CANCEL_ALL Emulation
		case VK_ESCAPE:
#ifdef _NOUT_SYS_EVENT
			printf("\n> Translate VK_ESCAPE  to NTOUCH_CANCEL (All).");
#endif
			pevent = (NEVENT *)NEventAllocBack();
			pevent->Type = NTOUCH_CANCEL;
			pevent->Touch.Param = 0;
			pevent->Touch.pRecipient = NULL;
			pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;
			pevent->Touch.TouchSequence.TouchCount = 0;
			return 1;

		// RETURN BACK Emulation
		case VK_TAB:
#ifdef _NOUT_SYS_EVENT
			printf("\n> Translate VK_TAB  to NSYS_RETURN_BACK.");
#endif
			pevent = (NEVENT *)NEventAllocBack();
			pevent->Type = NKEY_DOW_BACK;
			pevent->Head.Param = 0;
			pevent->Head.pRecipient = NULL;
			pevent->Head.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;
			return 1;

		default:
			// TOUCH EMULATIONs ?
			if (NTouchEmulArray.Size)
			{
				keymatch = 0;

				// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent = NULL;
				ptm = (NTOUCH_EMUL *)NTouchEmulArray.pFirst;
				for (i = 0; i < NTouchEmulArray.Size; i++, ptm++)
				{
					if (ptm->KeyCode == pmsg->wParam)
					{
						NErrorIf(ISFLAG_ON(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED), NERROR_SYSTEM_CHECK);
						FLAG_ON(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED);

						// Event Touch
						if (!pevent)
						{
							pevent = (NEVENT *)NEventAllocBack();
							pevent->Type = NTOUCH_START;
							pevent->Touch.Param = 0;
							pevent->Touch.pRecipient = NULL;
							pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;
							pevent->Touch.TouchSequence.TouchCount = 0;
						}

						pevent->Touch.TouchSequence.TouchIndex[pevent->Touch.TouchSequence.TouchCount] = _SafeNu32ToNu8(NTouchStream.Head);
						pevent->Touch.TouchSequence.TouchCount += 1;

						// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						ptouchstream = (NTOUCH *)NRingBufferAllocBack(&NTouchStream);
						ptouchstream->SystemID = NMAKE_TOUCH_SYSTEMID_FROM_KEY(ptm->KeyCode, keymatch);
						ptouchstream->Flags = NTOUCH_DEVICE_KEYBOARD; // ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);
						ptouchstream->Phase = NTOUCH_PHASE_START;
						ptouchstream->TapCount = 1;
						NCoord_HRelToPix(&ptouchstream->Position, &ptm->HRelPosition);
						ptouchstream->PreviousPosition = ptouchstream->Position;

#ifdef _NOUT_SYS_EVENT
						printf("\n> EVENT --WM_KEYDOWN -- Translated to TOUCH START x:%d, y:%d", ptouchstream->Position.x, ptouchstream->Position.y);
#endif

						keymatch++;
					}
				}

				if (!keymatch)
				{
#ifdef _NOUT_SYS_EVENT
					printf("\n> EVENT -- WM_KEYDOWN -- Not Translated.");
#endif

					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
#ifdef _NOUT_SYS_EVENT
				printf("\n> EVENT -- WM_KEYDOWN -- Not Translated.");
#endif
				return 0;
			}
		}
		break;

	case WM_KEYUP:
		// TOUCH EMULATIONs ?
		if (NTouchEmulArray.Size)
		{
			keymatch = 0;

			// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// Event Head building
			pevent = NULL;
			ptm = (NTOUCH_EMUL *)NTouchEmulArray.pFirst;
			for (i = 0; i < NTouchEmulArray.Size; i++, ptm++)
			{
				if (ptm->KeyCode == pmsg->wParam)
				{
					NErrorIf(ISFLAG_OFF(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED), NERROR_SYSTEM_CHECK);
					FLAG_OFF(ptm->Flags, FLAG_NTOUCH_EMUL_TOUCH_STARTED);

					// Event Touch
					if (!pevent)
					{
						pevent = (NEVENT *)NEventAllocBack();
						pevent->Type = NTOUCH_END;
						pevent->Touch.Param = 0;
						pevent->Touch.pRecipient = NULL;
						pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;
						pevent->Touch.TouchSequence.TouchCount = 0;
					}

					pevent->Touch.TouchSequence.TouchIndex[pevent->Touch.TouchSequence.TouchCount] = _SafeNu32ToNu8(NTouchStream.Head);
					pevent->Touch.TouchSequence.TouchCount += 1;

					// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					ptouchstream = (NTOUCH *)NRingBufferAllocBack(&NTouchStream);
					ptouchstream->SystemID = NMAKE_TOUCH_SYSTEMID_FROM_KEY(ptm->KeyCode, keymatch);
					ptouchstream->Flags = NTOUCH_DEVICE_KEYBOARD; // ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);
					ptouchstream->Phase = NTOUCH_PHASE_END;
					ptouchstream->TapCount = 1;
					NCoord_HRelToPix(&ptouchstream->Position, &ptm->HRelPosition);
					ptouchstream->PreviousPosition = ptouchstream->Position;

#ifdef _NOUT_SYS_EVENT
					printf("\n> EVENT -- WM_KEYUP -- Translated to TOUCH END x:%d, y:%d", ptouchstream->Position.x, ptouchstream->Position.y);
#endif

					keymatch++;
				}
			}

			if (!keymatch)
			{
#ifdef _NOUT_SYS_EVENT
				printf("\n> EVENT -- WM_KEYUP -- Not Translated.");
#endif
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n> EVENT -- WM_KEYUP -- Not Translated.");
#endif

			return 0;
		}
		break;

	case WM_LBUTTONDOWN:
		// Touch ( Only with LEFT BUTTON )
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n> Translate WM_LBUTTONDOWN to NTOUCH_START.");
#endif

			// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// Event Head building
			pevent = (NEVENT *)NEventAllocBack();
			pevent->Touch.Type = NTOUCH_START;
			pevent->Touch.Param = 0;
			pevent->Touch.pRecipient = NULL;
			pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;

			// Event Touch
			pevent->Touch.TouchSequence.TouchIndex[0] = _SafeNu32ToNu8(NTouchStream.Head);
			pevent->Touch.TouchSequence.TouchCount = 1;

			// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			ptouchstream = (NTOUCH *)NRingBufferAllocBack(&NTouchStream);
			ptouchstream->SystemID = CONSTANT_NTOUCH_SYSTEMID__WINDOWS_MOUSE;
			ptouchstream->Flags = NTOUCH_DEVICE_MOUSE; // ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);
			ptouchstream->Phase = NTOUCH_PHASE_START;
			ptouchstream->TapCount = 1;
			ptouchstream->Position.x = GET_X_LPARAM(pmsg->lParam);
			ptouchstream->Position.y = GET_Y_LPARAM(pmsg->lParam);
			ptouchstream->PreviousPosition = ptouchstream->Position;

			// store historic
			PreviousMousePosition = ptouchstream->Position;
			return 1;
		}
		break;

	case WM_LBUTTONUP:
		// Touch ( Only with LEFT BUTTON )
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n> Translate WM_LBUTTONUP to NTOUCH_END.");
#endif
			// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// Event Head building
			pevent = (NEVENT *)NEventAllocBack();
			pevent->Touch.Type = NTOUCH_END;
			pevent->Touch.Param = 0;
			pevent->Touch.pRecipient = NULL;
			pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;

			// Event Touch
			pevent->Touch.TouchSequence.TouchIndex[0] = _SafeNu32ToNu8(NTouchStream.Head);
			pevent->Touch.TouchSequence.TouchCount = 1;

			ptouchstream = (NTOUCH *)NRingBufferAllocBack(&NTouchStream);
			ptouchstream->SystemID = CONSTANT_NTOUCH_SYSTEMID__WINDOWS_MOUSE;
			ptouchstream->Flags = NTOUCH_DEVICE_MOUSE; // ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);
			ptouchstream->Phase = NTOUCH_PHASE_END;
			ptouchstream->TapCount = 1;
			ptouchstream->Position.x = GET_X_LPARAM(pmsg->lParam);
			ptouchstream->Position.y = GET_Y_LPARAM(pmsg->lParam);
			ptouchstream->PreviousPosition = ptouchstream->Position;

			// store historic
			PreviousMousePosition = ptouchstream->Position;
			return 1;
		}
		break;

	case WM_MOUSEMOVE:
		// Touch ( Only with LEFT BUTTON pressed )
		if (pmsg->wParam & MK_LBUTTON)
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n> Translate WM_MOUSEMOVE + BUTTON LEFT to NTOUCH_MOVE.");
#endif
			// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// Event Head building
			pevent = (NEVENT *)NEventAllocBack();
			pevent->Touch.Type = NTOUCH_MOVE;
			pevent->Touch.Param = 0;
			pevent->Touch.pRecipient = NULL;
			pevent->Touch.RecipientCast = NEVENT_RECIPIENT_CAST_NULL;

			// Event Touch
			pevent->Touch.TouchSequence.TouchIndex[0] = _SafeNu32ToNu8(NTouchStream.Head);
			pevent->Touch.TouchSequence.TouchCount = 1;

			ptouchstream = (NTOUCH *)NRingBufferAllocBack(&NTouchStream);
			ptouchstream->SystemID = CONSTANT_NTOUCH_SYSTEMID__WINDOWS_MOUSE;
			ptouchstream->Flags = NTOUCH_DEVICE_MOUSE; // ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);
			ptouchstream->Phase = NTOUCH_PHASE_MOVE;
			ptouchstream->TapCount = 1;
			ptouchstream->Position.x = GET_X_LPARAM(pmsg->lParam);
			ptouchstream->Position.y = GET_Y_LPARAM(pmsg->lParam);
			ptouchstream->PreviousPosition = PreviousMousePosition;
			// store historic
			PreviousMousePosition = ptouchstream->Position;

			return 1;
		}
		else
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n> EVENT -- WM_MOUSEMOVE -- without Left button pressed >>> Not Translated.");
#endif
			return 0;
		}
		break;

	default:
#ifdef _NOUT_SYS_EVENT
		printf("\n> Unknow Event. No translation.");
#endif
		return 0;
	}
}
#endif

/*

// PREVIOUS VERSION TO REMEMBER TRY of MOUSE/NTOUCH integrations ....

#ifdef _NWINDOWS
Nu8 NTranslateEvent(const SDL_Event *sdlevent,NEVENT *pevent)
{
	NTOUCH	*ptouchstream;
	printf("\nDEBUG-->NTranslateEvent Start translating." );

	switch(sdlevent->type)
	{
		case SDL_NOEVENT:
			NErrorIf(1,NERROR_SYSTEM_CHECK); // It's not suppose to happen !
			//pevent->Type = NSYS_NOEVENT;
			return 0;

		case SDL_QUIT:
			printf("\nDEBUG------------------> Translate SDL_QUIT to NSYS_QUIT." );
			pevent->Type = NSYS_QUIT;
			return 1;

// If _NMOUSE_MODE is set to ON, then SDL_MOUSEMOTION is going to be translated as NTOUCH_MOVE ... ALWAYS.
// even if there was no SDL_MOUSEBUTTONDOWN before.

		case SDL_KEYDOWN:
			switch( sdlevent->key.keysym.sym )
			{
				// CANCEL_ALL Emulation
				case SDLK_ESCAPE:
					printf("\nDEBUG------------------> Translate SDLK_ESCAPE  to NTOUCH_CANCEL (All)." );
					pevent->Type = NTOUCH_CANCEL;
					pevent->Touch.TouchSequence.TouchCount = 0;
					return 1;

				case SDLK_TAB:
					printf("\nDEBUG------------------> Translate SDLK_TAB  to NSYS_RETURN_BACK." );
					pevent->Type = NSYS_RETURN_BACK;
					return 1;

				default:
					printf("\nDEBUG------------------> EVENT -- SDL_KEYDOWN -- Not Translated ." );
					pevent->Type = NSYS_NOT_TRANSLATED;
					return 1;
			}

		case SDL_MOUSEBUTTONDOWN:
			// Touch ( Only with LEFT BUTTON )
			if( ((SDL_MouseButtonEvent*)sdlevent)->button ==  SDL_BUTTON_LEFT )
			{
				NErrorIf(!(((SDL_MouseButtonEvent*)sdlevent)->state & SDL_BUTTON(SDL_BUTTON_LEFT)),NERROR_SYSTEM_CHECK);

				printf("\nDEBUG------------------> Translate SDL_MOUSEBUTTONDOWN + BUTTON LEFT to NTOUCH_START." );

				// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_START;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= _SafeNu32ToNu8( NTouchStream.Head );
				pevent->Touch.TouchSequence.TouchCount		= 1;

				// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				printf("\nDEBUG------------------> NRingBufferAllocBack call........	from NTOUCH_START" );
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= (Nu32)((SDL_MouseButtonEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				#ifndef _TOUCH_EXTENDED
				// Thanks to 'NTouchCoreInit() checks' we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= NMOUSE_BUTTON(NMOUSE_BUTTON_LEFT);			// To remove all other potential button infos
				ptouchstream->XButton			= _SafeNu32To4BitsValue(NMOUSE_BUTTON_LEFT);	// To remove all other potential button infos
				#endif

				#ifdef  _TOUCH_EXTENDED
				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= _SafeNu32To4BitsValue(NMOUSE_BUTTON_LEFT);
				#endif

				ptouchstream->Phase				= NTOUCH_PHASE_START;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseButtonEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->PreviousPosition.y= ((SDL_MouseButtonEvent*)sdlevent)->y;


				return 1;
			}
			#ifndef  _TOUCH_EXTENDED
			else
			{
				printf("\nDEBUG------------------> EVENT -- SDL_MOUSEBUTTONDOWN -- Not Translated ." );
				pevent->Type = NSYS_NOT_TRANSLATED;
				return 1;
			}
			#endif
			#ifdef  _TOUCH_EXTENDED
			// or TouchX ?
			else
			{
				// + TOUCHX EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_START_X;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= NTouchStream.Head;
				pevent->Touch.TouchSequence.TouchCount		= 1;

				// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= ((SDL_MouseButtonEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= _SafeNu32To4BitsValue(((SDL_MouseButtonEvent*)sdlevent)->button);

				ptouchstream->Phase				= NTOUCH_PHASE_START;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseButtonEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->PreviousPosition.y= ((SDL_MouseButtonEvent*)sdlevent)->y;

				return 1;
			}
			#endif
			break;

		case SDL_MOUSEBUTTONUP:
			// Touch ( Only with LEFT BUTTON )
			if( ((SDL_MouseButtonEvent*)sdlevent)->button ==  SDL_BUTTON_LEFT )
			{
				NErrorIf(((SDL_MouseButtonEvent*)sdlevent)->state & SDL_BUTTON(SDL_BUTTON_LEFT),NERROR_SYSTEM_CHECK);

				printf("\nDEBUG------------------> Translate SDL_MOUSEBUTTONUP + BUTTON LEFT to NTOUCH_END." );

				// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_END;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= _SafeNu32ToNu8( NTouchStream.Head );
				pevent->Touch.TouchSequence.TouchCount		= 1;

				printf("\nDEBUG------------------> NRingBufferAllocBack call........	from NTOUCH_END" );
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= (Nu32)((SDL_MouseButtonEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				#ifndef _TOUCH_EXTENDED
				ptouchstream->XState			= 0; // To remove all other potential button infos
				ptouchstream->XButton			= _SafeNu32To4BitsValue(NMOUSE_BUTTON_LEFT);
				#endif
				#ifdef  _TOUCH_EXTENDED
				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= _SafeNu32To4BitsValue(NMOUSE_BUTTON_LEFT);
				#endif
				ptouchstream->Phase				= NTOUCH_PHASE_END;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseButtonEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->PreviousPosition.y= ((SDL_MouseButtonEvent*)sdlevent)->y;

				return 1;
			}
			#ifndef  _TOUCH_EXTENDED
			else
			{
				printf("\nDEBUG------------------> EVENT -- SDL_MOUSEBUTTONUP -- Not Translated ." );
				pevent->Type = NSYS_NOT_TRANSLATED;
				return 1;
			}
			#endif
			#ifdef  _TOUCH_EXTENDED
			// or TouchX ?
			else
			{
				// + TOUCHX EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_END_X;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= NTouchStream.Head;
				pevent->Touch.TouchSequence.TouchCount		= 1;

				// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= (Nu32)((SDL_MouseButtonEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= _SafeNu32To4BitsValue(((SDL_MouseButtonEvent*)sdlevent)->button);

				ptouchstream->Phase				= NTOUCH_PHASE_END;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseButtonEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->PreviousPosition.y= ((SDL_MouseButtonEvent*)sdlevent)->y;

				return 1;
			}
			#endif
			break;

		case SDL_MOUSEMOTION:
			// Touch ( Only with LEFT BUTTON pressed )
			if( ((SDL_MouseMotionEvent*)sdlevent)->state & SDL_BUTTON(SDL_BUTTON_LEFT) )
			{
				printf("\nDEBUG------------------> Translate SDL_MOUSEMOTION + BUTTON LEFT to NTOUCH_MOVE." );

				// + TOUCH EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_MOVE;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= _SafeNu32ToNu8( NTouchStream.Head );
				pevent->Touch.TouchSequence.TouchCount		= 1;

				printf("\nDEBUG------------------> NRingBufferAllocBack call........	from NTOUCH_MOVE" );
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= (Nu32)((SDL_MouseMotionEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				#ifndef _TOUCH_EXTENDED
				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= NMOUSE_BUTTON(NMOUSE_BUTTON_LEFT); // To remove all other potential button infos
				ptouchstream->XButton			= 0; // = 0 while mouse move
				#endif

				#ifdef  _TOUCH_EXTENDED
				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= 0; // = 0 while mouse move
				#endif

				ptouchstream->Phase				= NTOUCH_PHASE_MOVE;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseMotionEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseMotionEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseMotionEvent*)sdlevent)->x - ((SDL_MouseMotionEvent*)sdlevent)->xrel;
				ptouchstream->PreviousPosition.y= ((SDL_MouseMotionEvent*)sdlevent)->y - ((SDL_MouseMotionEvent*)sdlevent)->yrel;

				return 1;
			}
			#ifndef  _TOUCH_EXTENDED
			else
			{
				printf("\nDEBUG------------------> EVENT -- SDL_MOUSEMOTION -- Not Translated ." );
				pevent->Type = NSYS_NOT_TRANSLATED;
				return 1;
			}
			#endif
			#ifdef  _TOUCH_EXTENDED
			// or TouchX ?
			else if( ((SDL_MouseMotionEvent*)sdlevent)->state ) // !!! At least one button as to be pressed !!!
			{
				// + TOUCHX EVENT +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// Event Head building
				pevent->Touch.Type				= NTOUCH_MOVE_X;
				pevent->Touch.Param				= 0;
				pevent->Touch.pRecipient		= NULL;
				pevent->Touch.RecipientCast		= NEVENT_RECIPIENT_CAST_NULL;

				// Event Touch
				pevent->Touch.TouchSequence.TouchIndex[0]	= NTouchStream.Head;
				pevent->Touch.TouchSequence.TouchCount		= 1;

				// + TOUCH STREAM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				ptouchstream = (NTOUCH*)NRingBufferAllocBack(&NTouchStream);
				ptouchstream->SystemID			= (Nu32)((SDL_MouseButtonEvent*)sdlevent)->which;
				ptouchstream->Flags				= NTOUCH_DEVICE_MOUSE; //ptouchstream->Flags = 0; NSET_TOUCH_DEVICE(ptouchstream->Flags,NTOUCH_DEVICE_MOUSE);

				// Thanks to 'NTouchCoreInit()' checks we are sure SDL and N are compatible about mouse bouton states and id
				ptouchstream->XState			= ((SDL_MouseButtonEvent*)sdlevent)->state;
				ptouchstream->XButton			= 0; // = 0 while mouse move

				ptouchstream->Phase				= NTOUCH_PHASE_MOVE;
				ptouchstream->TapCount			= 1;
				ptouchstream->Position.x		= ((SDL_MouseButtonEvent*)sdlevent)->x;
				ptouchstream->Position.y		= ((SDL_MouseButtonEvent*)sdlevent)->y;
				ptouchstream->PreviousPosition.x= ((SDL_MouseMotionEvent*)sdlevent)->x - ((SDL_MouseMotionEvent*)sdlevent)->xrel;
				ptouchstream->PreviousPosition.y= ((SDL_MouseMotionEvent*)sdlevent)->y - ((SDL_MouseMotionEvent*)sdlevent)->yrel;

				return 1;
			}
			else // mouse move but without any button pressed
			{
				 return 0; // ! this case is not handle by N ENGINE !!!
			}
			#endif
			break;
/ *
		case SDL_USEREVENT:
			switch( sdlevent->user.code )
			{

				case N_ANIMATED_UI_IS_UP:
					pevent->Type = N_ANIMATED_UI_IS_UP;
					pevent->UIState.UIID = (Nu32)sdlevent->user.data1;
					break;

				case N_ANIMATED_UI_IS_DOWN:
					pevent->Type = N_ANIMATED_UI_IS_DOWN;
					pevent->UIState.UIID = (Nu32)sdlevent->user.data1;
					break;

				default:
					break;

			}
			break;
* /
		default:
			printf("\nDEBUG------------------> Unknow Event. No translation." );
			pevent->Type = NSYS_NOT_TRANSLATED;
			return 1;
	}
}
#endif
*/
