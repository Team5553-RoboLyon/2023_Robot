#include "lib/N/NCStandard.h"
#include "../NUsualCoordinates.h"
#include "../UI/NUI.h"
#include "../UI/NUIValUpdater.h"

#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

// ------------------------------------------------------------------------------------------
// NUIXCreateValueModifierButton
// ------------------------------------------------------------------------------------------
// Description :
//			Create a Value Modifier button. Simple ...
// ------------------------------------------------------------------------------------------
// In  :
//		parent		Parent UI ( a menu, a layer, ... all NUI* are working
//		x			x coordinate of the UI
//		y			y coordinate of the UI
//		pxdesc		a pointer on NUIXT_VALUEMODIFIERDESC structure which contains all the things
//					necessary to build in the UI.
//					See bellow to understand 'Flags' and "parameters' of 'pxdesc'
// Out :
//		a valid pointer of the created UI or NULL if a problem happens
// ------------------------------------------------------------------------------------------
// Understand 'Flags' for 'ValueModifierButton' Creation:
/*
	FIRST OF ALL:
	As all extended UI, this UI is a normal UI with an extension.
	This normal UI is considered like a button with an extension.
	In that case, the extension allows the button to manage a value between specific min and specific max
	and, thanks to a simple callback function gives this value to the program.
	Without anymore code, just the callback !

	So, parameters and ExtendFlags of pxdesc  ...

	1)
	Usually the way to change the current value of a ValueModifierButton is to hold it down and move the cursor
	vertically. if you prefer to move the cursor Horizontally to change the value, use the flag FLAG_NUIX_VALUEMODIFIER_USECURSORX.
		FLAG_ON(pxdesc.ExtendFlags,FLAG_NUIX_VALUEMODIFIER_USECURSORX);
		or
		Nothing (default)

	2)
	By default, there is no value printed on a ValueModifierButton but it can be.
	By setting On the flag FLAG_NUIX_VALUEMODIFIER_VALUEPRINTING, the current value will be printed at the end of the button caption
	if there is one, otherwise alone, in the middle of the button.
	Notice that this value is a Nf32 value and is printed like this (a floating point value).
	If you want to print it as an integer value set the flag FLAG_NUIX_VALUEMODIFIER_INTEGER_CASTING_FOR_PRINTING to on.
	Setting on the flag FLAG_NUIX_VALUEMODIFIER_INTEGER_CASTING_FOR_PRINTING
	without setting on the flag FLAG_NUIX_VALUEMODIFIER_INTEGER_CASTING_FOR_PRINTING doesn't do anything ... obviously.
	Notice that the value stills to be a floating point value even if is printed as an integer.
		FLAG_ON(pxdesc.ExtendFlags,FLAG_NUIX_VALUEMODIFIER_VALUEPRINTING);
		FLAG_ON(pxdesc.ExtendFlags,FLAG_NUIX_VALUEMODIFIER_INTEGER_CASTING_FOR_PRINTING);
		or
		Nothing (default)

	3)
	By default the ValueCallback function is called at TOUCH_END only, even if value is updated more often (during TOUCH_MOVE actually).
	If you want to call this ValueCallback function each time the current value is really updated then set the flag
	FLAG_NUIX_VALUEMODIFIER_VALUECALLBACK_ASNECESSARY to on.
		FLAG_ON(pxdesc.ExtendFlags,FLAG_NUIX_VALUEMODIFIER_VALUECALLBACK_ASNECESSARY);
		or
		Nothing (default)

	4)
	pframe_caption
	Set this string ptr if you want to have a button caption.
	This Creation function will do all necessary to right initialize the caption.
	If you have asked to print the current value too. The current value will be added at the end of caption.

	5)
	patlas_backrefname
	Set here the name of a valid Atlas_element. It will be used as a background image.
	This Creation function will do all necessary to right initialize this backframe.

	6)
	patlas_iconrefname
	Like the previous point but for icon.

	7)
	pstyle
	This parameter is transfered to the NUICreate function. It can be NULL ( NUICreate will use the default style)

	8)
	FrameSize
	You have to setup a right size for the button. It's not automatic.
	FrameSize parameters are going to be transfered to NCreateUI function with the right parameters
	to be sure that the function will use this size.

	9)
	initialvalue
	Current value of the button will be initialize with this one.

	10) Min and Max
	Represent the maximum and the minimum value that the currentvalue can reach.

	11)step
	Each time the value changes its by step. (+or- one step at a time)

	12)
	PixelMoveByStep
	Each time the cursor performs a move of 'PixelMoveByStep' pixels, current value changes of one step (+or-)

	13/
	valueUpdated_callBack
	The callback function called when current value changes. Call frequency depends on user choices
	(each time or only at TOUCH_END see (3) for details )
	NEngine calls this function with a valid pointer on a Nf32 value which points on the button current value !
	So trough the function you can get the value and use it.

	14/
	UI_userdata;
	The userdata parameter as usual. It will be transfered to the NCreateUI function.
*/
static inline void _PrepareUIBasicValUpdaterDesc(NUI_VALUPDATER_DESC *pvudesc, const Nchar *pcaption, const Nbool integer_command_and_display, const NUI_EVENT_HANDLE event_proc)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NVEC3 text_size;
	// NVEC3		icon_size;
	NVEC3 bkg_size;
	NVEC3 anchor, parent_origin, relative_pos;

	NVec3Set(&text_size, 0, 0, 0);
	// NVec3Set(&icon_size,	0,0,0);
	NVec3Set(&bkg_size, 0, 0, 0);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	// +
	pvudesc->UIDesc.Event_Proc = event_proc;
	pvudesc->UIDesc.pStyle = NUT_pCurrentUIDeskSet->pStyle;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_PUSH))
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_UI_TRANSFORMHNODE);				 // It's enough, for a push UI, to enable push animation ...
		FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS); // NUT PUSH controllers doing like this...
	}																					 // instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
		FLAG_ON(pvudesc->UIDesc.UITextDesc.Flags, FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EVENT : UI CORE / UI COMMAND / UI NOTIFY
	// +

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CAPTION/TEXT
	// + This Controller has a text display device its absolutely necessary to print values.
	// +
	FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);

	// We are going to estimate the printed value string max size.
	// Its important to help Background Size definition from text displayer size into the NUT_UICreation generic process.
	//
	NVEC3 tsizea, tsizeb, tsizec;
	Nchar str[CONSTANT_NUI_NAME_SIZEMAX + CONSTANT_NUI_VALUPDATER_VALUE_PRINT_SIZE_MAX]; // to print at maximum pui.pName + value

	if (integer_command_and_display)
	{
		NUIValUpdater_SetupDisplayableIntegerCastString(str, pcaption, pvudesc->Min);
		NGetStringSize(&tsizea, str, pvudesc->UIDesc.pStyle->pFont);
		NUIValUpdater_SetupDisplayableIntegerCastString(str, pcaption, pvudesc->Max);
		NGetStringSize(&tsizeb, str, pvudesc->UIDesc.pStyle->pFont);
		NUIValUpdater_SetupDisplayableIntegerCastString(str, pcaption, pvudesc->InitialValue);
		NGetStringSize(&tsizec, str, pvudesc->UIDesc.pStyle->pFont);
	}
	else
	{
		NUIValUpdater_SetupDisplayableFloatCastString(str, pcaption, pvudesc->Min, pvudesc->DigitNumber);
		NGetStringSize(&tsizea, str, pvudesc->UIDesc.pStyle->pFont);
		NUIValUpdater_SetupDisplayableFloatCastString(str, pcaption, pvudesc->Max, pvudesc->DigitNumber);
		NGetStringSize(&tsizeb, str, pvudesc->UIDesc.pStyle->pFont);
		NUIValUpdater_SetupDisplayableFloatCastString(str, pcaption, pvudesc->InitialValue, pvudesc->DigitNumber);
		NGetStringSize(&tsizec, str, pvudesc->UIDesc.pStyle->pFont);
	}
	text_size.x = NMAX(NMAX(tsizea.x, tsizeb.x), tsizec.x);
	text_size.y = NMAX(NMAX(tsizea.y, tsizeb.y), tsizec.y);
	text_size.z = NMAX(NMAX(tsizea.z, tsizeb.z), tsizec.z);
	NSetUITextDescSize(&pvudesc->UIDesc.UITextDesc, &text_size);

	NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
	NSetUITextDescAnchor(&pvudesc->UIDesc.UITextDesc, &anchor);
	NSetUITextDescParent(&pvudesc->UIDesc.UITextDesc, &pvudesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
	NSetUITextDescRelativePosition(&pvudesc->UIDesc.UITextDesc, &relative_pos);

	NUT_UIDesk_Apply_TextAlignment(&pvudesc->UIDesc.UITextDesc);
	NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pvudesc->UIDesc.UITextDesc);
	NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pvudesc->UIDesc.UITextDesc);

	if (pcaption)
	{
		pvudesc->UIDesc.pName = (Nchar *)pcaption;
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME);
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_COPY_NAME_STRING); // Necessary to rebuild caption+value each time value changes
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BACKGROUND GEOMETRY
	// + ValUpdater may have a BACKGROUND its not an obligation.
	// + But in all cases we have to figure out its size en position.
	// + NUI setup is going to create a background only if 'FLAG_NUIDS_BACKGROUND' is set to on.
	// + So if we don't want to build it, just set this flag to OFF, but keep using "backgroundesc.placement" it's easier !
	// +
	NSetUIGeometryDescAnchor(&pvudesc->UIDesc.UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pvudesc->UIDesc.UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pvudesc->UIDesc.UIBackgroundDesc, 0, 0, 0);
	NUT_UIDesk_ApplyUI_Size(&bkg_size, &bkg_size, NULL, &text_size, NULL);
	NSetUIGeometryDescSize(&pvudesc->UIDesc.UIBackgroundDesc, &bkg_size);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_DEFAULT_BKGD))
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND);
		pvudesc->UIDesc.GeometryPreAllocatedCapacity = 1; // Just the Background
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pvudesc->UIDesc.UIBackgroundDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD, &pvudesc->UIDesc.UIBackgroundDesc);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC/UIDS Flags.
	// +
	// + --- + Event Management Flags
	//			NUT supposes user wants to receive all events if he defines an "event_proc", and in an opposite way ...
	//			... NUT supposes user doesn't want to receive any event if he doesn't define an "event_proc".
	if (event_proc)
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	 // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	 // Set to ON as default behavior. User may update it after creation
		NUT_UIDesk_Apply_TouchMode(&pvudesc->UIDesc);
	}
	else
	{
		FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);		  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); // Set to OFF as default behavior. User may update it after creation
	}

	FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);	 // Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	if (ISFLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND))
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
	}
	else
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT);
	}

	if (integer_command_and_display)
	{
		FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_INTEGER_CASTING_FOR_COMMAND);
		FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY);
	}
	else
	{
		FLAG_OFF(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_INTEGER_CASTING_FOR_COMMAND);
		FLAG_OFF(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY);
	}

	// + --- + Always ON
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_PRECISE_PICKING);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);

	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_USECURSORX);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_STATIC_TOUCH_END_COMMAND);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_ENABLE_HIGHSPEED);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_INPROGRESS_COMMAND);
	FLAG_ON(pvudesc->UIDesc.Flags_Core, FLAG_NUIDC_VALUPDATER_LOOP);
	FLAG_ON(pvudesc->UIDesc.Flags_Style, FLAG_NUIDS_VALUPDATER_VALUE_DISPLAY);
}

NUI_VALUPDATER *NUT_CreateUIFlatValUpdateri(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Ns32 minvalue, const Ns32 maxvalue, const Ns32 initialvalue, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_VALUPDATER_DESC vudesc;
	NUI_VALUPDATER *pframe_valupdater;

	// Extended Data setting up ...
	memset(&vudesc, 0, sizeof(NUI_VALUPDATER_DESC));

	// Specifics Parameters
	vudesc.Step = 1.0f;
	vudesc.PixelMoveByStep = 10; // Cursor have to perform a 2 pixels move to change current value of one step.
	vudesc.HighSpeedFactor = 10.0f;
	vudesc.HighSpeedThreshold = 4;
	vudesc.DigitNumber = 0;
	// user define
	vudesc.InitialValue = (Nf32)initialvalue;
	vudesc.Max = (Nf32)maxvalue;
	vudesc.Min = (Nf32)minvalue;

	_PrepareUIBasicValUpdaterDesc(&vudesc, pcaption, NTRUE, event_proc);
	// ...And create:
	pframe_valupdater = NCreateUIValUpdater(parent, &vudesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_valupdater, &vudesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// NSetUIPositionf((NUI*)pframe_valupdater, x, y, 0.0f);
	return pframe_valupdater;
}

NUI_VALUPDATER *NUT_CreateUIFlatValUpdaterf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Nf32 minvalue, const Nf32 maxvalue, const Nf32 initialvalue, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_VALUPDATER_DESC vudesc;
	NUI_VALUPDATER *pframe_valupdater;

	// Extended Data setting up ...
	memset(&vudesc, 0, sizeof(NUI_VALUPDATER_DESC));

	// Specifics Parameters
	vudesc.Step = 0.001f;
	vudesc.PixelMoveByStep = 2; // Cursor have to perform a 2 pixels move to change current value of one step.
	vudesc.HighSpeedFactor = 10.0f;
	vudesc.HighSpeedThreshold = 4;
	vudesc.DigitNumber = 3;
	// user define
	vudesc.InitialValue = initialvalue;
	vudesc.Max = maxvalue;
	vudesc.Min = minvalue;

	_PrepareUIBasicValUpdaterDesc(&vudesc, pcaption, NFALSE, event_proc);
	// ...And create:
	pframe_valupdater = NCreateUIValUpdater(parent, &vudesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_valupdater, &vudesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	//	NSetUIPositionf((NUI*)pframe_valupdater, x, y, 0.0f);
	return pframe_valupdater;
}
