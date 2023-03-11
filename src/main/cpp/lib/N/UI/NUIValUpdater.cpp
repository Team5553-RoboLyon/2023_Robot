#include "lib/N/NCStandard.h"
#include "NUI.h"
#include "NUIPush.h"
#include "NUIValUpdater.h"
#include "lib/N/Event/NEventTouch.h"

Nchar *NUIValUpdater_SetupDisplayableIntegerCastString(Nchar *pdest, const Nchar *pcaption, const Nf32 currentvalue)
{
	if (pcaption)
	{
		sprintf(pdest, CONSTANT_NUI_VALUPDATER_STR_BUILD_NAME_AND_INTEGER, pcaption, (Ns32)currentvalue);
	}
	else
	{
		sprintf(pdest, CONSTANT_NUI_VALUPDATER_STR_BUILD_INTEGER, (Ns32)currentvalue);
	}
	return pdest;
}

Nchar *NUIValUpdater_SetupDisplayableFloatCastString(Nchar *pdest, const Nchar *pcaption, const Nf32 currentvalue, const Nu16 digitnumber)
{
	Nchar builder[16];
	builder[0] = 0;

	if (pcaption)
	{
		sprintf(builder, "%s%d%s", "%s%.", digitnumber, "f");
		sprintf(pdest, builder, pcaption, currentvalue);
	}
	else
	{
		sprintf(builder, "%s%d%s", "%.", digitnumber, "f");
		sprintf(pdest, builder, currentvalue);
	}
	return pdest;
}
static inline void _printcurrentvalue(NUI_VALUPDATER *pvu)
{
	Nchar str[CONSTANT_NUI_NAME_SIZEMAX + CONSTANT_NUI_VALUPDATER_VALUE_PRINT_SIZE_MAX]; // to print at maximum pui.pName  + value

	if (pvu->DisplayedValue != pvu->CurrentValue)
	{
		str[0] = 0;
		if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY))
		{
			NUIValUpdater_SetupDisplayableIntegerCastString(str, pvu->UI.pName, pvu->CurrentValue);
		}
		else
		{
			NUIValUpdater_SetupDisplayableFloatCastString(str, pvu->UI.pName, pvu->CurrentValue, pvu->DigitNumber);
		}
		NBuildUITextMeshFromString((NUI *)pvu, str);
		pvu->DisplayedValue = pvu->CurrentValue;
	}
}
static inline Nu32 _increasevaluebyonestep(NUI_VALUPDATER *pvalup, const Nu32 resetbank)
{
	Nf32 val;

	if (resetbank)
		pvalup->Bank = 0;
	else
		pvalup->Bank -= pvalup->Step;

	// keep currentvalue in mind to later test change
	val = pvalup->CurrentValue;

	// increase
	pvalup->CurrentValue += pvalup->Step;
	if (pvalup->CurrentValue > pvalup->Max)
	{
		if (ISFLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_LOOP))
			pvalup->CurrentValue = pvalup->Min;
		else
			pvalup->CurrentValue = pvalup->Max;
	}

	// Changed ?
	if (pvalup->CurrentValue != val)
		return 1;
	else
		return 0;
}

static inline Nu32 _decreasevaluebyonestep(NUI_VALUPDATER *pvalup, const Nu32 resetbank)
{
	Nf32 val;

	if (resetbank)
		pvalup->Bank = 0;
	else
		pvalup->Bank += pvalup->Step;

	// keep currentvalue in mind to later test change
	val = pvalup->CurrentValue;

	// decrease
	pvalup->CurrentValue -= pvalup->Step;
	if (pvalup->CurrentValue < pvalup->Min)
	{
		if (ISFLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_LOOP))
			pvalup->CurrentValue = pvalup->Max;
		else
			pvalup->CurrentValue = pvalup->Min;
	}

	// Changed ?
	if (pvalup->CurrentValue != val)
		return 1;
	else
		return 0;
}

/*
static inline void _valupdater_inprogress_command(NUI *pui)
{
	NEVENT nevent;

	if( ISFLAG_ON(pui->Flags,FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND) )
	{
		memset(&nevent,0,sizeof(NEVENT));
		if( ISFLAG_ON(pui->Flags,FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND) )
		{
			nevent.Type = NCOMMAND_VALUPDATER_NS32;
			nevent.ValUpdateri.Value_Ns32 = (Ns32)((NUI_VALUPDATER*)pui)->CurrentValue;
		}
		else
		{
			nevent.Type = NCOMMAND_VALUPDATER_NF32;
			nevent.ValUpdaterf.Value_Nf32 = ((NUI_VALUPDATER*)pui)->CurrentValue;
		}
		pui->Event_Proc(pui,&nevent);
	}
}
static inline void _valupdater_end_command(NUI *pui)
{
	NEVENT nevent;

	memset(&nevent,0,sizeof(NEVENT));
	if( ISFLAG_ON(pui->Flags,FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND) )
	{
		nevent.Type = NCOMMAND_VALUPDATER_NS32;
		nevent.ValUpdateri.Value_Ns32 = (Ns32)((NUI_VALUPDATER*)pui)->CurrentValue;
	}
	else
	{
		nevent.Type = NCOMMAND_VALUPDATER_NF32;
		nevent.ValUpdaterf.Value_Nf32 = ((NUI_VALUPDATER*)pui)->CurrentValue;
	}
	pui->Event_Proc(pui,&nevent);
}
*/
static inline void _valupdater_send_command(NUI *pui)
{
	NEVENT nevent;
	memset(&nevent, 0, sizeof(NEVENT));
	if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND))
	{
		NFillupEventHead(nevent, NCOMMAND_VALUPDATER_NS32, 0, NEVENT_RECIPIENT_CAST_UI, pui);
		nevent.ValUpdateri.Value_Ns32 = (Ns32)((NUI_VALUPDATER *)pui)->CurrentValue;
	}
	else
	{
		NFillupEventHead(nevent, NCOMMAND_VALUPDATER_NF32, 0, NEVENT_RECIPIENT_CAST_UI, pui);
		nevent.ValUpdaterf.Value_Nf32 = ((NUI_VALUPDATER *)pui)->CurrentValue;
	}
	NSendUICommandEvent(&nevent);
}

void NUIValUpdater_EventToState_Handle(NUI *pui, const NEVENT *pevent)
{
	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event (which is weird for a UIValUpdater but anyway ...)
	//  in that case function returns immediately ...
	if ((NUI *)pevent->Head.pRecipient != pui)
		return;

	Nf32 delta;
	Nu32 value_change;
	Nf32 tmp;

	switch (pevent->Type)
	{
	case NTOUCH_START:
		if (NIsUIFirstTouchStarted(pui))
		{
			((NUI_VALUPDATER *)pui)->Bank = 0.0f; // reset Bank
			FLAG_OFF(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_MOVED);

			// Store Current Value as "Cancel value"
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT))
			{
				((NUI_VALUPDATER *)pui)->PreviousValue = ((NUI_VALUPDATER *)pui)->CurrentValue;
			}

			/*
							if(ISFLAG_ON(pui->Flags,FLAG_NUI_VALUPDATER_USECURSORX))
							{
								((NUI_VALUPDATER*)pui)->Touch_Previous = pevent->Touch.Position.x;
							}
							else
							{
								((NUI_VALUPDATER*)pui)->Touch_Previous = pevent->Touch.Position.y;
							}
			*/
		}
		break;

	case NTOUCH_MOVE:
		value_change = 0;
		// UNDER TOUCH CANCEL ?
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT))
		{
			if (NIsUIFirstTouchMoveIn(pui))
			{
				if (((NUI_VALUPDATER *)pui)->PreviousValue != ((NUI_VALUPDATER *)pui)->CurrentValue)
				{
					tmp = ((NUI_VALUPDATER *)pui)->CurrentValue;
					((NUI_VALUPDATER *)pui)->CurrentValue = ((NUI_VALUPDATER *)pui)->PreviousValue;
					((NUI_VALUPDATER *)pui)->PreviousValue = tmp;
					value_change = 1;
				}
			}
			else if (NIsUILastTouchMoveOut(pui))
			{
				if (((NUI_VALUPDATER *)pui)->PreviousValue != ((NUI_VALUPDATER *)pui)->CurrentValue)
				{
					tmp = ((NUI_VALUPDATER *)pui)->CurrentValue;
					((NUI_VALUPDATER *)pui)->CurrentValue = ((NUI_VALUPDATER *)pui)->PreviousValue;
					((NUI_VALUPDATER *)pui)->PreviousValue = tmp;

					// We have to do this now because just after, in that case we are going to "break" and go out the
					// current handle.
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
					{
						// To Send Display later during FrameState handling
						FLAG_ON(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST);
					}
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND))
					{
						// To Send command later during FrameState handling
						_valupdater_send_command(pui);
						// 							FLAG_ON(((NUI_VALUPDATER*)pui)->XFlags,FLAG_NUIX_VALUPDATER_SEND_COMMAND_REQUEST);
					}
				}
				break;
			}
		}
		// ... Let's MOVE !
		FLAG_ON(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_MOVED);
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_USECURSORX))
		{
			delta = (Nf32)NGetEventFastestTouchCoordMove(pevent, _X);
			//				((NUI_VALUPDATER*)pui)->Touch_Previous = pevent->Touch.Position.x;
		}
		else
		{
			delta = (Nf32)NGetEventFastestTouchCoordMove(pevent, _Y);
			//				((NUI_VALUPDATER*)pui)->Touch_Previous = pevent->Touch.Position.y;
		}
		// High Speed ?
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED))
			if (NABS(delta) > ((NUI_VALUPDATER *)pui)->HighSpeedThreshold)
				delta *= ((NUI_VALUPDATER *)pui)->HighSpeedFactor;
		((NUI_VALUPDATER *)pui)->Bank += delta * ((NUI_VALUPDATER *)pui)->Speed;
		while (NABS(((NUI_VALUPDATER *)pui)->Bank) >= NABS(((NUI_VALUPDATER *)pui)->Step))
		{
			if (((NUI_VALUPDATER *)pui)->Bank >= ((NUI_VALUPDATER *)pui)->Step)
				value_change += _increasevaluebyonestep((NUI_VALUPDATER *)pui, ISFLAG_OFF(pui->Flags, FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED));
			else if (((NUI_VALUPDATER *)pui)->Bank <= -((NUI_VALUPDATER *)pui)->Step)
				value_change += _decreasevaluebyonestep((NUI_VALUPDATER *)pui, ISFLAG_OFF(pui->Flags, FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED));
		}

		if (value_change)
		{
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
			{
				// To Send Display later during FrameState handling
				FLAG_ON(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST);
			}
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND))
			{
				// Send delayed command Event
				_valupdater_send_command(pui);
				//					FLAG_ON(((NUI_VALUPDATER*)pui)->XFlags,FLAG_NUIX_VALUPDATER_SEND_COMMAND_REQUEST);
			}
		}
		break;

	case NTOUCH_END:
		if (NIsUILastTouchEnded(pui))
		{
			// 1/ NO TOUCH MOVE !!! So no value update until yet, even if FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND is set to ON...
			if (ISFLAG_OFF(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_MOVED))
			{
				// ... no move ... so ... increase value by one step !!!
				if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_STATIC_TOUCH_END_COMMAND) && _increasevaluebyonestep((NUI_VALUPDATER *)pui, NTRUE))
				{
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
					{
						// To Send Display later during FrameState handling
						FLAG_ON(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST);
					}
					// To Send command later during FrameState handling
					_valupdater_send_command(pui);
				}
			}
			// 2/ TOUCH MOVE occurred, so value was certainly updated ...
			else
			{
				if (ISFLAG_OFF(pui->Flags, FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND) && (((NUI_VALUPDATER *)pui)->CurrentValue != ((NUI_VALUPDATER *)pui)->PreviousValue))
				{
					// To Send command later during FrameState handling
					_valupdater_send_command(pui);
				}
			}
		}
		break;

	case NTOUCH_CANCEL:
		break;

	default:
		break;
	}

	NUIPush_EventToState_PreHandle(pui, pevent);
}

static void NUIValUpdater_States_Handle(NUI *pui, const NTIME *ptime)
{
	// Look for demands from EventToState Handle
	/*
		if( ISFLAG_ON(((NUI_VALUPDATER *)pui)->XFlags,FLAG_NUIX_VALUPDATER_SEND_COMMAND_REQUEST) )
		{
			_valupdater_send_command(pui);
			FLAG_OFF(((NUI_VALUPDATER *)pui)->XFlags,FLAG_NUIX_VALUPDATER_SEND_COMMAND_REQUEST);
		}
	*/

	if (ISFLAG_ON(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST))
	{
		_printcurrentvalue((NUI_VALUPDATER *)pui);
		FLAG_OFF(((NUI_VALUPDATER *)pui)->XFlags, FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST);
	}

	NUIPush_States_PreHandle(pui, ptime);
}

NUI_VALUPDATER *NCreateUIValUpdater(NUI *parent, NUI_VALUPDATER_DESC *pvalupdesc, const Nu32 user32)
{
	return NSetupUIValUpdater(parent, NEW(NUI_VALUPDATER), pvalupdesc, user32);
}

NUI_VALUPDATER *NSetupUIValUpdater(NUI *parent, NUI_VALUPDATER *pvalup, NUI_VALUPDATER_DESC *pvalupdesc, const Nu32 user32)
{
	NPushUISetUpProcess((NUI *)pvalup, (NUI_DESC *)pvalupdesc);

	// Call Parent Structure Setup (here, parent structure is NUI)
#ifdef _DEBUG
	if (!NSetupUI(parent, (NUI *)pvalup, (NUI_DESC *)pvalupdesc, user32))
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (NUI_SETUP or/and NUI_ENABLE) ...
		// This should never happen !
		// Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI
		NErrorIf(1, NERROR_UI_SETUPPROCESS_STACK_CORRUPTED);
	}
#else
	NSetupUI(parent, (NUI *)pvalup, (NUI_DESC *)pvalupdesc, user32);
#endif

	// to ZERO:
	pvalup->Bank = 0.0f;
	// Re-Setup System data & functions ( could be overwritten )
	pvalup->UI.CurrentState = NUI_STATE_PUSH_UP_REST;
	pvalup->UI.UIState_Proc = NUIValUpdater_States_Handle;
	pvalup->UI.UIEventToState_Proc = NUIValUpdater_EventToState_Handle;
	pvalup->UI.ClearUI_Proc = NClearUIValUpdater;

	// Specific Default Touch Mode ( different than Simple UI )
	if (ISFLAG_OFF(pvalupdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE))
		NSetUIListenTouchMode((NUI *)pvalup, NUI_LISTEN_TOUCH_MODE_BASIC_TRACK);

	pvalup->Max = pvalupdesc->Max;
	pvalup->Min = pvalupdesc->Min;
	pvalup->Speed = pvalupdesc->Step / pvalupdesc->PixelMoveByStep;
	pvalup->Step = pvalupdesc->Step;
	pvalup->CurrentValue = pvalupdesc->InitialValue;
	pvalup->PreviousValue = pvalup->CurrentValue;

	if (pvalup->CurrentValue > pvalup->Max) // It seems not necessary, maybe ...
		pvalup->CurrentValue = pvalup->Max; // But, we want to be sure that all values
	if (pvalup->CurrentValue < pvalup->Min) // are consistent after initialization process.
		pvalup->CurrentValue = pvalup->Min; // It's why we perform these tests ...

	pvalup->HighSpeedFactor = pvalupdesc->HighSpeedFactor;
	pvalup->HighSpeedThreshold = pvalupdesc->HighSpeedThreshold;

	pvalup->DigitNumber = pvalupdesc->DigitNumber;

	// Notice that the 2 values below will have an effect only if FLAG_NUIX_VALUEMODIFIER_ENABLE_HIGHSPEED is set to ON !
	// HighSpeedFactor is a multiplier factor to INCREASE the change value speed, so it cant be less than 1 !
	// (it would be greater than 2 at minimum !)
	if (pvalup->HighSpeedFactor < 1.0f)
		pvalup->HighSpeedFactor = 1.0f;

	// HighSpeedThreshold has to be greater than 0 !
	if (pvalup->HighSpeedThreshold < 0.0f)
		pvalup->HighSpeedFactor = 0.0f;

	// FLAGS check and set
	pvalup->XFlags = 0;
	if (pvalup->UI.pText) // We need an already created UI text
	{
		if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Style, FLAG_NUIDS_VALUPDATER_VALUE_DISPLAY))
		{
			FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY);

			if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Style, FLAG_NUIDS_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY))
				FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY);
		}
	}

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_INTEGER_CASTING_FOR_COMMAND))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND);

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_ENABLE_HIGHSPEED))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED);

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_INPROGRESS_COMMAND))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND);

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_USECURSORX))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_USECURSORX);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_USECURSORX);

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_STATIC_TOUCH_END_COMMAND))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_STATIC_TOUCH_END_COMMAND);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_STATIC_TOUCH_END_COMMAND);

	if (ISFLAG_ON(pvalupdesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_LOOP))
		FLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_LOOP);
	else
		FLAG_OFF(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_LOOP);

	if (ISFLAG_ON(pvalup->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
		_printcurrentvalue(pvalup);

	return (NUI_VALUPDATER *)NPopUISetUpProcess((NUI *)pvalup, (NUI_DESC *)pvalupdesc);
}

void NClearUIValUpdater(NUI *pui)
{
	NUI_VALUPDATER *pvalup = (NUI_VALUPDATER *)pui;

	// a)Clear Value Updater Section
	pvalup->Max = 0.0f;
	pvalup->Min = 0.0f;
	pvalup->Speed = 0.0f;
	pvalup->Step = 0.0f;
	pvalup->DisplayedValue = 0.0f;
	pvalup->CurrentValue = 0.0f;
	pvalup->PreviousValue = 0.0f;
	pvalup->HighSpeedFactor = 0.0f;
	pvalup->HighSpeedThreshold = 0.0f;
	pvalup->DigitNumber = 0;

	pvalup->Bank = 0.0f;
	pvalup->XFlags = 0;

	pvalup->Touch_Previous = 0;
	pvalup->Touch_Current = 0;

	// a)Clear UI Section
	NClearUI_Basics(pui);
}

void NUIValUpdater_SetCurrentValuef(NUI_VALUPDATER *pvu, const Nf32 val, const Nbool send_command_message)
{
	pvu->CurrentValue = val;

	if (pvu->CurrentValue < pvu->Min)
		pvu->CurrentValue = pvu->Min;

	if (pvu->CurrentValue > pvu->Max)
		pvu->CurrentValue = pvu->Max;

	if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
		_printcurrentvalue(pvu);

	if (send_command_message)
		_valupdater_send_command((NUI *)pvu);
}

void NUIValUpdater_SetCurrentValuei(NUI_VALUPDATER *pvu, const Ns32 val, const Nbool send_command_message)
{
	pvu->CurrentValue = (Nf32)val;

	if (pvu->CurrentValue < pvu->Min)
		pvu->CurrentValue = pvu->Min;

	if (pvu->CurrentValue > pvu->Max)
		pvu->CurrentValue = pvu->Max;

	if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
		_printcurrentvalue(pvu);

	if (send_command_message)
		_valupdater_send_command((NUI *)pvu);
}

void NUIValUpdater_SetMinValuei(NUI_VALUPDATER *pvu, const Ns32 minval, const Nbool send_command_message)
{
	if ((Nf32)minval >= pvu->Max)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		return;
	}
	pvu->Min = (Nf32)minval;

	if (pvu->CurrentValue < pvu->Min)
	{
		pvu->CurrentValue = pvu->Min;

		if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
			_printcurrentvalue(pvu);

		if (send_command_message)
			_valupdater_send_command((NUI *)pvu);
	}
}

void NUIValUpdater_SetMaxValuei(NUI_VALUPDATER *pvu, const Ns32 maxval, const Nbool send_command_message)
{
	if ((Nf32)maxval <= pvu->Min)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		return;
	}
	pvu->Max = (Nf32)maxval;

	if (pvu->CurrentValue > pvu->Max)
	{
		pvu->CurrentValue = pvu->Max;

		if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
			_printcurrentvalue(pvu);

		if (send_command_message)
			_valupdater_send_command((NUI *)pvu);
	}
}
void NUIValUpdater_SetMinValuef(NUI_VALUPDATER *pvu, const Nf32 minval, const Nbool send_command_message)
{
	if (minval >= pvu->Max)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		return;
	}
	pvu->Min = minval;

	if (pvu->CurrentValue < pvu->Min)
	{
		pvu->CurrentValue = pvu->Min;

		if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
			_printcurrentvalue(pvu);

		if (send_command_message)
			_valupdater_send_command((NUI *)pvu);
	}
}

void NUIValUpdater_SetMaxValuef(NUI_VALUPDATER *pvu, const Nf32 maxval, const Nbool send_command_message)
{
	if (maxval <= pvu->Min)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		return;
	}
	pvu->Max = maxval;

	if (pvu->CurrentValue > pvu->Max)
	{
		pvu->CurrentValue = pvu->Max;

		if (ISFLAG_ON(pvu->UI.Flags, FLAG_NUI_VALUPDATER_VALUE_DISPLAY))
			_printcurrentvalue(pvu);

		if (send_command_message)
			_valupdater_send_command((NUI *)pvu);
	}
}
