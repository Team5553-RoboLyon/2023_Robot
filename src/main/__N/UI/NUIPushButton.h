#ifndef __NUI_PUSHBUTTON_H
#define __NUI_PUSHBUTTON_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									   NUIPushButton.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//	NUI_PUSHBUTTON.Flags (starts at BIT_24)
#define		FLAG_NUI_PUSHBUTTONCOMMAND_DOWN					BIT_24
#define		FLAG_NUI_PUSHBUTTONCOMMAND_UP					BIT_25

//	NUI_PUSHBUTTON_DESC.Flags_Style (starts at BIT_24)
//#define		FLAG_NUIDS_PUSHBUTTON_xxx						BIT_24

//	NUI_PUSHBUTTON_DESC.Flags_Core (starts at BIT_24)
#define		FLAG_NUIDC_PUSHBUTTONCOMMAND_DOWN				BIT_24
#define		FLAG_NUIDC_PUSHBUTTONCOMMAND_UP					BIT_25

typedef struct
{
	NUI	UI;
}NUI_PUSHBUTTON;

typedef struct
{
	NUI_DESC	UIDesc;
}NUI_PUSHBUTTON_DESC;


NUI_PUSHBUTTON*	NCreateUIPushButton(NUI *parent, NUI_PUSHBUTTON_DESC *pbuttondesc, const Nu32 user32);
NUI_PUSHBUTTON*	NSetupUIPushButton(NUI *parent, NUI_PUSHBUTTON *pui, NUI_PUSHBUTTON_DESC *pbuttondesc, const Nu32 user32);
void			NClearUIPushButton(NUI *pui);
//void			NDeleteUIPushButton(void *pfr); !!! Doesn't Exist !!! Use 'NDeleteUI'
inline	void	NEnableUIPushButtonCommandDown(NUI_PUSHBUTTON* pb){FLAG_ON(pb->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN);}
inline	void	NEnableUIPushButtonCommandUp(NUI_PUSHBUTTON* pb){FLAG_ON(pb->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP);}
inline	void	NDisableUIPushButtonCommandDown(NUI_PUSHBUTTON* pb){FLAG_OFF(pb->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN);}
inline	void	NDisableUIPushButtonCommandUp(NUI_PUSHBUTTON* pb){FLAG_OFF(pb->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP);}

// PRIVATE FUNCTIONS

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUI_PUSHBUTTON_H 

