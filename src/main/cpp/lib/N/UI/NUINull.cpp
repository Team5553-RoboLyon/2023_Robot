#include "lib/N/NCStandard.h"

#include "NUI.h"
#include "NUINull.h"

NUI *NCreateUINull(NUI *pparent, const Nu32 user32)
{

	NUI_DESC uidesc;

	memset(&uidesc, 0, sizeof(NUI_DESC));
	FLAG_ON(uidesc.Flags_Style, FLAG_NUIDS_NO_GEOMETRY_EXTRACTION);
	FLAG_ON(uidesc.Flags_Style, FLAG_NUIDS_DO_NOT_BLOCK_TOUCH_EVENT_FOR_GAMESTATES); // Not necessary because UINull doesn't listen touch event ! But any way ...

	// None of the Event Flags is set to ON so, no event listening and no event interception !
	return NCreateUI(pparent, &uidesc, user32);
}

NUI *NCreateUINullXtd(NUI *pparent, const NUI_EVENT_HANDLE event_handle, const Nu32 listen_intercept_flags_style, NUI_LISTEN_TOUCH_MODE_ENUM touch_mode, const Nu32 user32)
{
	// Check that only listen and/or intercept uidesc_style flags are used ...
	NErrorIf(!FLAGS_TEST(listen_intercept_flags_style, MASK_NUIDS_LISTEN_EVENT | MASK_NUIDS_INTERCEPT_EVENT, listen_intercept_flags_style), NERROR_INCONSISTENT_PARAMETERS);
	// UINull needs an event_handle to listen and/or intercept events ...
	NErrorIf(listen_intercept_flags_style && !event_handle, NERROR_INCONSISTENT_PARAMETERS);
	// If user define "touch_mode", he has to set FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE to ON !
	NErrorIf(ISFLAG_OFF(listen_intercept_flags_style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE) && touch_mode, NERROR_INCONSISTENT_PARAMETERS);

	NUI_DESC uidesc;
	memset(&uidesc, 0, sizeof(NUI_DESC));

	// Event Flags set according with user whishes !

	uidesc.Event_Proc = event_handle;
	FLAG_ON(uidesc.Flags_Style, listen_intercept_flags_style);
	FLAG_ON(uidesc.Flags_Style, FLAG_NUIDS_NO_GEOMETRY_EXTRACTION);

	// FLAG_ON(uidesc.Flags_Style,FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); not necessary thx to the previous 'NErrorIf' check
	uidesc.TouchMode = touch_mode;
	return NCreateUI(pparent, &uidesc, user32);
}
