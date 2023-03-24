#ifndef __NUI_CHECKBOX_H
#define __NUI_CHECKBOX_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUICheckBox.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Define
// #define NUI_CHECKBOX_CHECKTEXTS_SIZE							8		// Maximum Size of the Check/Uncheck Text strings reserved into each CheckButton.

#define DEFAULT_NUI_CHECKBOX_CHECK_TEXT							"On"	// "Start"	"In"	"Good"	"Yes"
#define DEFAULT_NUI_CHECKBOX_UNCHECK_TEXT						"Off"	// "Stop"	"Out"	"Bad"	"No"

#define DEFAULT_NUI_CHECKBOX_SIZE_X								DEFAULT_NUI_SIZE_X
#define DEFAULT_NUI_CHECKBOX_SIZE_Y								DEFAULT_NUI_SIZE_Y

// ***************************************************************************************
// *								FLAG_NUI_CHECKBOX									 *
// ***************************************************************************************
//	NUI_CHECKBOX.UI.Flags (starts at BIT_24)
#define		FLAG_NUI_CHECKBOX_CHECK									BIT_24 // THE CORE Flag of the controller. If it sets to ON CheckBox is CHECKED, and UNCHECKED if it isn't.
#define		FLAG_NUI_CHECKBOX_STATUS_PRINTING						BIT_25 // CORE Flag computed at creation. ON if CHECKBOX has something to print (MESH or/and TEXT), OFF if not (seems rare and strange, but ...).
#define		FLAG_NUI_CHECKBOX_CHECK_MESH							BIT_26 // ON if there is a CHECK MESH
#define		FLAG_NUI_CHECKBOX_UNCHECK_MESH							BIT_27 // ON if there is an UNCHECK MESH
#define		FLAG_NUI_CHECKBOX_PUSH_DOWN_TOGGLE						BIT_28 // ON to switch CHECKBOX status at PUSH_DOWN (MEANS managing CANCEL ...)instead at PUSH_UP (default one) 
#define		FLAG_NUI_CHECKBOX_COMMAND_TOGGLE						BIT_29 // ON to send NCOMMAND_CHECKBOX_TOGGLE message instead of NCOMMAND_CHECKBOX_CHECK/NCOMMAND_CHECKBOX_UNCHECK

//	NUI_CHECKBOX_DESC.UI.Flags_Style (starts at BIT_24)
#define		FLAG_NUIDS_CHECKBOX_STATUS_PRINTING						BIT_24
#define		FLAG_NUIDS_CHECKBOX_CHECK_MESH							BIT_25
#define		FLAG_NUIDS_CHECKBOX_CHECK_TEXT							BIT_26
#define		FLAG_NUIDS_CHECKBOX_UNCHECK_MESH						BIT_27
#define		FLAG_NUIDS_CHECKBOX_UNCHECK_TEXT						BIT_28
#define		FLAG_NUIDS_USEPARAM_CHECKBOX_CHECK_TEXT					BIT_29
#define		FLAG_NUIDS_USEPARAM_CHECKBOX_UNCHECK_TEXT				BIT_30

//	NUI_CHECKBOX_DESC.UI.Flags_Core (starts at BIT_24)
#define		FLAG_NUIDC_CHECKBOX_INITIAL_STATUS_CHECK				BIT_24
#define		FLAG_NUIDC_CHECKBOX_PUSH_DOWN_TOGGLE					BIT_25	// ON to switch CHECKBOX status at PUSH_DOWN (MEANS managing CANCEL ...)instead at PUSH_UP (default one) 
#define		FLAG_NUIDC_CHECKBOX_COMMAND_TOGGLE						BIT_26	// ON to send NCOMMAND_CHECKBOX_TOGGLE message instead of NCOMMAND_CHECKBOX_CHECK/NCOMMAND_CHECKBOX_UNCHECK
// ***************************************************************************************
// *								Structures											 *
// ***************************************************************************************
// A CheckBox
typedef struct
{
	NUI	UI;

	NGEOMETRY	*pCheckTag_Geometry;	// These geometries are included into the UI RENDERABLE (for sure) !
	NGEOMETRY	*pUnCheckTag_Geometry;	// Here we just have 2 valid pointers !

	Nchar		Check_Text[NUI_CHECKTEXTS_STRING_MAXSIZE];
	Nchar		UnCheck_Text[NUI_CHECKTEXTS_STRING_MAXSIZE];
}NUI_CHECKBOX;

// a CheckBox Description
typedef struct
{
	NUI_DESC			UIDesc;
	// Check
	Nchar				*pCheck_Text;
	NUIGEOMETRY_DESC	CheckDesc;	
	// UnCheck
	Nchar				*pUnCheck_Text;
	NUIGEOMETRY_DESC	UnCheckDesc;	
}NUI_CHECKBOX_DESC;

// ***************************************************************************************
// *								Functions											 *
// ***************************************************************************************
NUI_CHECKBOX*		NCreateUICheckBox(NUI *parent, NUI_CHECKBOX_DESC *pcbdesc, const Nu32 user32);
NUI_CHECKBOX*		NSetupUICheckBox(NUI *parent, NUI_CHECKBOX *pcb, NUI_CHECKBOX_DESC *pcbdesc, const Nu32 user32);
void				NClearUICheckBox(NUI *pui);
//void				NDeleteUICheckBox(void *pfr); !!! Doesn't Exist !!! Use 'NDeleteUI'

void				NUICheckBox_SetCurrentStatus(NUI_CHECKBOX *pcb, const Nbool bstatus, const Nbool send_command_message, const Nbool _if_status_updated_only);
Nbool				NUICheckBox_GetCurrentStatus(const NUI_CHECKBOX *pcb);


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUI_CHECKBOX_H 

