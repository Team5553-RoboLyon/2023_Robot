#include "lib/N/NCStandard.h"
#include "lib/N/NString.h"
#include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "lib/N/Event/NEvent.h"
#include "NUI.h"
#include "NUIColor.h"
#include "NUIPush.h"
#include "NUICheckBox.h"

static inline void _checkbox_toggle_and_command(NUI *pcb, Nbool bdelayed)
{
	NEVENT nevent;

	if (ISFLAG_ON(pcb->Flags, FLAG_NUI_CHECKBOX_CHECK))
	{
		FLAG_OFF(pcb->Flags, FLAG_NUI_CHECKBOX_CHECK);

		// Send Event
		memset(&nevent, 0, sizeof(NEVENT));
		nevent.Head.pRecipient = pcb;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_UI;
		// nevent.Head.Param				= 0; useless because of 'memset'

		if (ISFLAG_ON(pcb->Flags, FLAG_NUI_CHECKBOX_COMMAND_TOGGLE))
			nevent.Type = NCOMMAND_CHECKBOX_TOGGLE;
		else
			nevent.Type = NCOMMAND_CHECKBOX_UNCHECK;

		nevent.CheckBox.bCheck = NFALSE;
		if (bdelayed)
		{
			NPostDelayedUIEvent(&nevent);
		}
		else
		{
			NSendUICommandEvent(&nevent);
		}
	}
	else
	{
		FLAG_ON(pcb->Flags, FLAG_NUI_CHECKBOX_CHECK);

		// Send Event
		memset(&nevent, 0, sizeof(NEVENT));
		nevent.Head.pRecipient = pcb;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_UI;
		// nevent.Head.Param				= 0; useless because of 'memset'

		if (ISFLAG_ON(pcb->Flags, FLAG_NUI_CHECKBOX_COMMAND_TOGGLE))
			nevent.Type = NCOMMAND_CHECKBOX_TOGGLE;
		else
			nevent.Type = NCOMMAND_CHECKBOX_CHECK;

		nevent.CheckBox.bCheck = NTRUE;

		if (bdelayed)
		{
			NPostDelayedUIEvent(&nevent);
		}
		else
		{
			NSendUICommandEvent(&nevent);
		}
	}
}

static inline void _printcurrentstatus(NUI_CHECKBOX *pcb)
{
	Nchar fullcheckbox_caption[NUI_CHECKTEXTS_STRING_MAXSIZE + CONSTANT_NUI_NAME_SIZEMAX];

	if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_STATUS_PRINTING))
	{
		if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK))
		{
			if (pcb->Check_Text[0] != 0)
			{
				// Rebuild UIText 'caption + check_text'
				fullcheckbox_caption[0] = 0;
				if (pcb->UI.pName)
					strcpy(fullcheckbox_caption, pcb->UI.pName); // Using strcpy instead of NStrCopy because we are sure of all the parameters !
				strcat(fullcheckbox_caption, pcb->Check_Text);	 // Using strcat instead of NStrCat because we are sure of all the parameters !
				NBuildUITextMeshFromString((NUI *)pcb, fullcheckbox_caption);
			}
			// Without Check_Text, there is No TextMesh building. Use FLAG_NUIDS_DISPLAY_NAME to build a caption "once" at setup.
			// Even if there is a UI.pName. Why ? Because without Check_text the checkbox status toggle has no impact on
			// the UI Text Displayer so No reason to rebuild it ...
		}
		else
		{
			if (pcb->UnCheck_Text[0] != 0)
			{
				// Rebuild UIText 'caption + check_text'
				fullcheckbox_caption[0] = 0;
				if (pcb->UI.pName)
					strcpy(fullcheckbox_caption, pcb->UI.pName); // Using strcpy instead of NStrCopy because we are sure of all the parameters !
				strcat(fullcheckbox_caption, pcb->UnCheck_Text); // Using strcat instead of NStrCat because we are sure of all the parameters !
				NBuildUITextMeshFromString((NUI *)pcb, fullcheckbox_caption);
			}
			// Without UnCheck_Text, there is No TextMesh building... (see explanations above, for Check_text )
		}
	}
}

void NUICheckBox_EventToState_Handle(NUI *pui, const NEVENT *pevent)
{
	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event (which is weird for a CheckBox but anyway ...)
	//  in that case function returns immediately ...
	if ((NUI *)pevent->Head.pRecipient != pui)
		return;

	// Color Update ?
	if (ISFLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE))
	{
		switch (pevent->Type)
		{
		case NTOUCH_START:
			if (NIsUIFirstTouchMoveIn(pui))
				NSetUICheckUnCheckColor((NUI_CHECKBOX *)pui, NUI_COLORSET_FOCUS);
			break;

		case NTOUCH_MOVE:
			if (NIsUIFirstTouchMoveIn(pui))
				NSetUICheckUnCheckColor((NUI_CHECKBOX *)pui, NUI_COLORSET_FOCUS);
			else if (NIsUILastTouchMoveOut(pui))
				NSetUICheckUnCheckColor((NUI_CHECKBOX *)pui, NUI_COLORSET_ENABLE);
			break;

		case NTOUCH_END:
			if (NIsUILastTouchEnded(pui))
				NSetUICheckUnCheckColor((NUI_CHECKBOX *)pui, NUI_COLORSET_ENABLE);
			break;
		}
	}
	// First, call the EventToState 'Push' function.
	// It will manage 'ALONE' the UI "State" and will returns the current State (after updating it) AND some KEY_INSTANT.
	// Actually, purpose of 'NUICheckBox_EventToState_Handle' is just sending appropriate Direct EVENT according
	// with these KEY_INSTANTs.
	switch (NGET_UIPUSH_PREHANDLE_KEY_INSTANT(NUIPush_EventToState_PreHandle(pui, pevent)))
	{
	case NUI_KEY_INSTANT_PUSH_DOWN_CANCEL:
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE))
		{
			_checkbox_toggle_and_command(pui, NFALSE);
			_printcurrentstatus((NUI_CHECKBOX *)pui);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_UP:
		if (ISFLAG_OFF(pui->Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE))
		{
			_checkbox_toggle_and_command(pui, NFALSE);
			_printcurrentstatus((NUI_CHECKBOX *)pui);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_DOWN:
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE))
		{
			_checkbox_toggle_and_command(pui, NFALSE);
			_printcurrentstatus((NUI_CHECKBOX *)pui);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_RESET:
		// Nothing to do yet ...
		break;

	default:
		break;
	}
}

static void NUICheckBox_States_Handle(NUI *pui, const NTIME *ptime)
{
	switch (NGET_UIPUSH_PREHANDLE_KEY_INSTANT(NUIPush_States_PreHandle(pui, ptime)))
	{
	// There are only 2 possible DELAYED Key Instants:
	// ( NUI_KEY_INSTANT_PUSH_DOWN_CANCEL is never Delayed ! )
	// Handle DELAYED Key Instant PUSH UP
	case NUI_KEY_INSTANT_PUSH_UP:
		if (ISFLAG_OFF(pui->Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE))
		{
			_checkbox_toggle_and_command(pui, NTRUE);
			_printcurrentstatus((NUI_CHECKBOX *)pui);
		}
		break;

	// Handle DELAYED Key Instant PUSH DOWN
	case NUI_KEY_INSTANT_PUSH_DOWN:
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE))
		{
			_checkbox_toggle_and_command(pui, NTRUE);
			_printcurrentstatus((NUI_CHECKBOX *)pui);
		}
		break;
	}
}

NUI_CHECKBOX *NCreateUICheckBox(NUI *parent, NUI_CHECKBOX_DESC *pcbdesc, const Nu32 user32)
{
	return NSetupUICheckBox(parent, NEW(NUI_CHECKBOX), pcbdesc, user32);
}

NUI_CHECKBOX *NSetupUICheckBox(NUI *parent, NUI_CHECKBOX *pcb, NUI_CHECKBOX_DESC *pcbdesc, const Nu32 user32)
{

	NPushUISetUpProcess((NUI *)pcb, (NUI_DESC *)pcbdesc);

	memset(pcb, 0, sizeof(NUI_CHECKBOX));

	// Force additionnal TransformHNode Creation !
	FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_UI_TRANSFORMHNODE);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CALL PARENT STRUCTURE SETUP
	//		Here, parent structure is NUI.
#ifdef _DEBUG
	if (!NSetupUI(parent, (NUI *)pcb, (NUI_DESC *)pcbdesc, user32))
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (NUI_SETUP or/and NUI_ENABLE) ...
		// This should never happen !
		// (Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI)
		NErrorIf(1, NERROR_UI_SETUPPROCESS_STACK_CORRUPTED);
	}
#else
	NSetupUI(parent, (NUI *)pcb, (NUI_DESC *)pcbdesc, user32);
#endif
	// + -------------------------------------------------------------------------------------------------------------------------
	// + RE-SETUP System data & functions
	//		Overwrite some parameters
	pcb->UI.CurrentState = NUI_STATE_PUSH_UP_REST;
	pcb->UI.UIState_Proc = NUICheckBox_States_Handle;
	pcb->UI.UIEventToState_Proc = NUICheckBox_EventToState_Handle;
	pcb->UI.ClearUI_Proc = NClearUICheckBox;

	// Specific Default Touch Mode ( different than Simple UI )
	if (ISFLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE))
		NSetUIListenTouchMode((NUI *)pcb, NUI_LISTEN_TOUCH_MODE_BASIC_TRACK);

	// + -------------------------------------------------------------------------------------------------------------------------
	// INTIAL CHECKBOX STATUS
	//		Retrieve and keep the requested current CheckBox status
	if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_CHECKBOX_INITIAL_STATUS_CHECK))
		FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK);
	else
		FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK);

	if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_CHECKBOX_PUSH_DOWN_TOGGLE))
		FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE);
	else
		FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE);

	if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_CHECKBOX_COMMAND_TOGGLE))
		FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_COMMAND_TOGGLE);
	else
		FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_COMMAND_TOGGLE);

	// + -------------------------------------------------------------------------------------------------------------------------
	// CHECK FOR PRINTING
	//		Check for Printing Request validity.
	//		To be able to print on screen its current status, a CheckBox needs some GFX resources.
	//		Like texts and/or geometries.
	if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_STATUS_PRINTING))
	{
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_MESH) ||
			ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_MESH) ||
			ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_TEXT) ||
			ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_TEXT))
		{
			FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_STATUS_PRINTING);
		}
		else
		{
			FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_STATUS_PRINTING);
		}
	}
	else
	{
		FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_STATUS_PRINTING);
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CREATE ADDITIONNAL GFX ELEMENTS
	//			Create GFX additional elements (only if requested)
	//			possibilities:
	//				-Check mesh geometry BASIC (a quad with or without texture) or Check mesh geometry Custom
	//				-Check Text BASIC("ON")	or Check Text custom
	//				-UnCheck mesh geometry BASIC (a quad with or without texture) or UnCheck mesh geometry Custom
	//				-UnCheck Text BASIC("ON") or UnCheck Text custom

	if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_STATUS_PRINTING))
	{
		// Check Text
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_TEXT))
		{
			if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_CHECK_TEXT))
			{
				NStrCopy(pcb->Check_Text, pcbdesc->pCheck_Text, NUI_CHECKTEXTS_STRING_MAXSIZE);
			}
			else
				NStrCopy(pcb->Check_Text, DEFAULT_NUI_CHECKBOX_CHECK_TEXT, NUI_CHECKTEXTS_STRING_MAXSIZE);
		}
		else
			pcb->Check_Text[0] = 0;

		// Check Mesh
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_MESH))
		{
			// If user doesn't specify color range and colorgradientID ... N has to setup default color build data
			if ((pcbdesc->CheckDesc.ColorBuild.Flags & MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT) == 0)
			{
				// 'pcbdesc->CheckDesc.ColorBuild' is suppose to be full of ZERO ... check that !
				NMemoryCheck(&pcbdesc->CheckDesc.ColorBuild, sizeof(NBUILDBASE_COLOR), 0);
				// N Is going to overwrite 'pcbdesc->CheckDesc.ColorBuild' with specific parameters  to use
				// NUI_STYLE range of color as default color build data
				FLAG_ON(pcbdesc->CheckDesc.ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
				pcbdesc->CheckDesc.ColorBuild.pColorRange = (NCOLOR *)NGetUIColorSetPtr(&pcbdesc->UIDesc.pStyle->ColorSet, NUI_COLORSET_CHECK, NExtractUIDescColorState(&pcbdesc->UIDesc));
				pcbdesc->CheckDesc.ColorBuild.ColorRangeSize = CONSTANT_NUI_COLORSET_COLORRANGE_SIZE;
			}
			pcb->pCheckTag_Geometry = NCreateUIGeometry((NUI *)pcb, &pcbdesc->CheckDesc);
			FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK_MESH);
		}
		else
			pcb->pCheckTag_Geometry = NULL;

		// UnCheck Text
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_TEXT))
		{
			if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_UNCHECK_TEXT))
			{
				NStrCopy(pcb->UnCheck_Text, pcbdesc->pUnCheck_Text, NUI_CHECKTEXTS_STRING_MAXSIZE);
			}
			else
				NStrCopy(pcb->UnCheck_Text, DEFAULT_NUI_CHECKBOX_UNCHECK_TEXT, NUI_CHECKTEXTS_STRING_MAXSIZE);
		}
		else
			pcb->UnCheck_Text[0] = 0;

		// UnCheck Mesh
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_MESH))
		{

			// If user doesn't specify color range and colorgradientID ... N has to setup default color build data
			if ((pcbdesc->UnCheckDesc.ColorBuild.Flags & MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT) == 0)
			{
				// 'pcbdesc->UnCheckDesc.ColorBuild' is suppose to be full of ZERO ... check that !
				NMemoryCheck(&pcbdesc->UnCheckDesc.ColorBuild, sizeof(NBUILDBASE_COLOR), 0);
				// N Is going to overwrite 'pcbdesc->UnCheckDesc.ColorBuild' with specific parameters  to use
				// NUI_STYLE range of color as default color build data
				FLAG_ON(pcbdesc->UnCheckDesc.ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
				pcbdesc->UnCheckDesc.ColorBuild.pColorRange = (NCOLOR *)NGetUIColorSetPtr(&pcbdesc->UIDesc.pStyle->ColorSet, NUI_COLORSET_UNCHECK, NExtractUIDescColorState(&pcbdesc->UIDesc));
				pcbdesc->UnCheckDesc.ColorBuild.ColorRangeSize = CONSTANT_NUI_COLORSET_COLORRANGE_SIZE;
			}

			pcb->pUnCheckTag_Geometry = NCreateUIGeometry((NUI *)pcb, &pcbdesc->UnCheckDesc);
			FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_UNCHECK_MESH);
		}
		else
			pcb->pUnCheckTag_Geometry = NULL;
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CHANGE EXTRACT FUNTION
	//			According with the CheckBox status there is always one geometry hidden.
	//			'pUnCheckTag_Geometry' is hidden if CheckBox is set to CHECK
	//			'pCheckTag_Geometry' is hidden if CheckBox is set to UNCHECK
	//			For sure, thats true only if these geometries are requested by user (Check box doesn't need these additional geometries to
	//			work )...
	//			So, if CheckBox has Check and/or UnCheck geometries we need to use a specific extract function
	//			This function will only extract needed geometries (so not all of them) according with the CheckBox Status.
	//			... otherwise, without these 2 geometries, no need for a special Extract Function, the UI generic one stills used.
	if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK_MESH) || ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_UNCHECK_MESH))
	{
		NErrorIf(!pcb->UI.pRenderable->GeomArray.Size, NERROR_SYSTEM_GURU_MEDITATION); // It's impossible, so if it's happenning ... BAD !!!
		NEXTRACTSET lowlevelextractset = 0;
		if (pcb->UI.pRenderable->GeomArray.Size == 1)
		{
			lowlevelextractset = NEXTRACTSET_SECTION_UICHECKBOX;
		}
		else // (pcb->UI.pRenderable->GeomArray.Size > 1)
		{
			lowlevelextractset = NEXTRACTSET_SECTION_UICHECKBOX | FLAG_NEXTRACTSET_MULTIGEOMETRIES;
		}

		NEXTRACTSET highlevelextractset = 0;
		if (ISFLAG_ON(((NUI_DESC *)pcbdesc)->Flags_Core, FLAG_NUIDC_USEPARAM_HIGHLEVEL_EXTRACTSET))
		{
			highlevelextractset = ((NUI_DESC *)pcbdesc)->HighLevelExtractSet;
		}
		else
		{
			highlevelextractset = DEFAULT_NUI_RENDERABLE_HIGHLEVEL_EXTRACTSET;
		}
		NSetRenderableExtractFCT(pcb->UI.pRenderable, NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(lowlevelextractset, highlevelextractset)));
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + 'PRINT' initial status
	_printcurrentstatus(pcb);

	return (NUI_CHECKBOX *)NPopUISetUpProcess((NUI *)pcb, (NUI_DESC *)pcbdesc);
}

void NClearUICheckBox(NUI *pui)
{
	// a)Clear CheckBox Section
	((NUI_CHECKBOX *)pui)->pCheckTag_Geometry = NULL;	// no need to delete ! geometry included into UI Renderable.
	((NUI_CHECKBOX *)pui)->pUnCheckTag_Geometry = NULL; // ... they will be both delete with NClearUI function !

	memset(((NUI_CHECKBOX *)pui)->Check_Text, 0, NUI_CHECKTEXTS_STRING_MAXSIZE * sizeof(Nchar));
	memset(((NUI_CHECKBOX *)pui)->UnCheck_Text, 0, NUI_CHECKTEXTS_STRING_MAXSIZE * sizeof(Nchar));

	// b)Clear UI Section
	NClearUI_Basics(pui);
}

void NUICheckBox_SetCurrentStatus(NUI_CHECKBOX *pcb, const Nbool bstatus, const Nbool send_command_message, const Nbool _if_status_updated_only)
{
	NEVENT nevent;
	Nbool bsend = NFALSE;

	// Status update
	if (bstatus)
	{
		if (ISFLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK))
		{
			FLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK);
			_printcurrentstatus(pcb);

			bsend = NTRUE;
		}
	}
	else
	{
		if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK))
		{
			FLAG_OFF(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK);
			_printcurrentstatus(pcb);

			bsend = NTRUE;
		}
	}

	if (!_if_status_updated_only)
		bsend = NTRUE;

	if (bsend && send_command_message)
	{
		memset(&nevent, 0, sizeof(NEVENT));
		nevent.Head.pRecipient = pcb;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_UI;
		// nevent.Head.Param				= 0; useless because of 'memset'

		if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_COMMAND_TOGGLE))
		{
			nevent.Type = NCOMMAND_CHECKBOX_TOGGLE;
			nevent.CheckBox.bCheck = bstatus;
		}
		else
		{
			if (bstatus)
			{
				nevent.Type = NCOMMAND_CHECKBOX_CHECK;
				nevent.CheckBox.bCheck = NTRUE;
			}
			else
			{
				nevent.Type = NCOMMAND_CHECKBOX_UNCHECK;
				nevent.CheckBox.bCheck = NFALSE;
			}
		}

		NSendUICommandEvent(&nevent);
	}
}

// ------------------------------------------------------------------------------------------
// NUIX_CheckBox_GetStatus
// ------------------------------------------------------------------------------------------
// Description :
//	Return Current checkbox Status (checked or not)
// ------------------------------------------------------------------------------------------
// In	:
//			pui		a valid pointer on a CheckBox UI
//
// Out :
//			Nbool		NTRUE if checkbox is checked, NFALSE if it isn't
//
// ------------------------------------------------------------------------------------------
Nbool NUICheckBox_GetCurrentStatus(const NUI_CHECKBOX *pcb)
{
	if (ISFLAG_ON(pcb->UI.Flags, FLAG_NUI_CHECKBOX_CHECK))
		return NTRUE;
	else
		return NFALSE;
}
