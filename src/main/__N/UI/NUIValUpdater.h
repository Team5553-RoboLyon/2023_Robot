#ifndef __NUI_FRAMEVALUPDATER_H
#define __NUI_FRAMEVALUPDATER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						  NUIValUpdater.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Constant
#define	CONSTANT_NUI_VALUPDATER_VALUE_PRINT_SIZE_MAX		16

#define	CONSTANT_NUI_VALUPDATER_STR_BUILD_NAME_AND_NF32		"%s%.3f"
#define	CONSTANT_NUI_VALUPDATER_STR_BUILD_NF32				"%.3f"
#define	CONSTANT_NUI_VALUPDATER_STR_BUILD_NAME_AND_INTEGER	"%s%d"
#define	CONSTANT_NUI_VALUPDATER_STR_BUILD_INTEGER			"%d"

// NUI_VALUPDATER.Flags (starts at BIT_24)
#define	FLAG_NUI_VALUPDATER_VALUE_DISPLAY					BIT_24	// Display value trough UI text.
#define	FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY		BIT_25	// Cast the displayed value as an integer. By default displayed value are casted as Nf32 (.###)
#define	FLAG_NUI_VALUPDATER_USECURSORX						BIT_26	// Cursor X moves drive value updates. By default Cursor Y moves are used.
#define	FLAG_NUI_VALUPDATER_STATIC_TOUCH_END_COMMAND		BIT_27	// a TOUCH_START/TOUCH_END without any TOUCH_MOVE on the controller will increase the value by one step.
#define	FLAG_NUI_VALUPDATER_INPROGRESS_COMMAND				BIT_28	// Send a NCOMMAND_VALUPDATER_NS32 (or NCOMMAND_VALUPDATER_NF32) each time the value involve ( so along TOUCH_MOVE ). By default NCOMMAND is sent only at TOUCH_END.
#define	FLAG_NUI_VALUPDATER_LOOP							BIT_29	// Updated value may loop from max to min and from min to max.
#define	FLAG_NUI_VALUPDATER_ENABLE_HIGHSPEED				BIT_30	// Turn high speed updating system to ON. Value changes are going to be faster when cursor MOVE reach HighSpeedThreshold.
#define	FLAG_NUI_VALUPDATER_INTEGER_CASTING_FOR_COMMAND		BIT_31	// Cast the value send trough COMMAND as an integer. By default this value are casted as Nf32.
																		// There is no link between PRINT CASTING and COMMAND CASTING. Both are working separately.	  	
																		// So you can cast as an INTEGER for printing and as a Nf32 for COMMAND.
//	NUI_VALUPDATER_DESC.Flags_Style (starts at BIT_24)
#define	FLAG_NUIDS_VALUPDATER_VALUE_DISPLAY					BIT_24	// Display value trough UI text.
#define	FLAG_NUIDS_VALUPDATER_INTEGER_CASTING_FOR_DISPLAY	BIT_25	// Cast the displayed value as an integer. By default displayed value are casted as Nf32 (.###)

//	NUI_VALUPDATER_DESC.Flags_Core (starts at BIT_24)
#define	FLAG_NUIDC_VALUPDATER_USECURSORX					BIT_24	// Cursor X moves drive value updates. By default Cursor Y moves are used.
#define	FLAG_NUIDC_VALUPDATER_STATIC_TOUCH_END_COMMAND		BIT_25	// a TOUCH_START/TOUCH_END without any TOUCH_MOVE on the controller will increase the value by one step.
#define	FLAG_NUIDC_VALUPDATER_INPROGRESS_COMMAND			BIT_26	// Send a NCOMMAND_VALUPDATER_NS32 (or NCOMMAND_VALUPDATER_NF32) each time the value involve ( so along TOUCH_MOVE ). By default NCOMMAND is sent only at TOUCH_END.
#define	FLAG_NUIDC_VALUPDATER_LOOP							BIT_27	// Updated value may loop from max to min and from min to max.
#define	FLAG_NUIDC_VALUPDATER_ENABLE_HIGHSPEED				BIT_28	// Turn high speed updating system to ON. Value changes are going to be faster when cursor MOVE reach HighSpeedThreshold.
#define	FLAG_NUIDC_VALUPDATER_INTEGER_CASTING_FOR_COMMAND	BIT_29	// Cast the value send trough COMMAND as an integer. By default this value are casted as Nf32.
																		// There is no link between PRINT CASTING and COMMAND CASTING. Both are working separately.	  
																		// So you can cast as an INTEGER for printing and as a Nf32 for COMMAND.

// NUI_VALUPDATER.XFlags (starts at BIT_0, to BIT_15)
#define FLAG_NUIX_VALUPDATER_MOVED							BIT_0	//	Internal use.	To check if cursor moved between TOUCH_START and TOUCH_END ...
#define FLAG_NUIX_VALUPDATER_DISPLAY_REQUEST				BIT_1	//	Internal use.	To perform text building (value display) at the right time, into the UI states management loop.

typedef struct
{
	NUI					UI;

	Ns16				Touch_Previous;
	Ns16				Touch_Current;
	
	Nf32				DisplayedValue;		
	Nf32				CurrentValue;		//
	Nf32				PreviousValue;		// Use for cancel
	Nf32				Max;				//
	Nf32				Min;				//
	Nf32				Step;				//	
	Nf32				Speed;				// calculated from desc.value "PixelMoveByStep" and step. It represents how the value progress for one pixel move.
	Nf32				Bank;				// Speed (value/pixel) is added to bank until bank >= step.
											// then, step is added to currentvalue (if current value stays into min/max limits)
	Nf32				HighSpeedThreshold;	// limit in pixel beyond this limit the cursor move is consider HighSpeedFactor times greater ...
	Nf32				HighSpeedFactor;	// the multiplier factor ...

	Nu16				DigitNumber;
	Nu16				XFlags;				// Additionnal FLAGS.			
	
}NUI_VALUPDATER;

typedef struct
{
	NUI_DESC		UIDesc;

	Nf32				InitialValue;		//
	Nf32				Max;				//
	Nf32				Min;				//
	Nf32				Step;				//	
	Nf32				PixelMoveByStep;	// each time the cursor performs a move of 'PixelMoveByStep' pixels, current value changes of one step
	Nf32				HighSpeedThreshold;	// limit in pixel beyond this limit the cursor move is consider HighSpeedFactor times greater ...
	Nf32				HighSpeedFactor;	// the multiplier factor ...
	
	Nu16				DigitNumber;
}NUI_VALUPDATER_DESC;

NUI_VALUPDATER*	NCreateUIValUpdater(NUI *parent, NUI_VALUPDATER_DESC *pvalupdesc, const Nu32 user32);
NUI_VALUPDATER*	NSetupUIValUpdater(NUI *parent, NUI_VALUPDATER *pvalup, NUI_VALUPDATER_DESC *pvalupdesc, const Nu32 user32);
void			NClearUIValUpdater(NUI* pui);
//void			NDeleteUIValUpdater(void *pfr); !!! Doesn't Exist !!! Use 'NDeleteUI'

void			NUIValUpdater_SetCurrentValuei(NUI_VALUPDATER *pvu, const Ns32 val,const Nbool send_command_message);
void			NUIValUpdater_SetMinValuei(NUI_VALUPDATER *pvu, const Ns32 minval,const Nbool send_command_message);
void			NUIValUpdater_SetMaxValuei(NUI_VALUPDATER *pvu, const Ns32 maxval,const Nbool send_command_message);
void			NUIValUpdater_SetCurrentValuef(NUI_VALUPDATER *pvu, const Nf32 val,const Nbool send_command_message);
void			NUIValUpdater_SetMinValuef(NUI_VALUPDATER *pvu, const Nf32 minval,const Nbool send_command_message);
void			NUIValUpdater_SetMaxValuef(NUI_VALUPDATER *pvu, const Nf32 maxval,const Nbool send_command_message);

inline	void	NUIValUpdater_SetPixelMoveByStep(NUI_VALUPDATER *pvu, const Nf32 pixmov){pvu->Speed = pvu->Step/pixmov;}	
inline	void	NUIValUpdater_SetStep(NUI_VALUPDATER *pvu, const Nf32 step){pvu->Speed = step*pvu->Step/pvu->Speed; pvu->Step=step;}	
inline	void	NUIValUpdater_SetHighSpeedThreshold(NUI_VALUPDATER *pvu, const Nf32 hsthres){pvu->HighSpeedThreshold = hsthres;}	
inline	void	NUIValUpdater_SetHighSpeedFactor(NUI_VALUPDATER *pvu, const Nf32 hsfact){pvu->HighSpeedFactor = hsfact;}	
inline	void	NUIValUpdater_UseCursorX(NUI_VALUPDATER *pvu){FLAG_ON(pvu->UI.Flags,FLAG_NUI_VALUPDATER_USECURSORX);}
inline	void	NUIValUpdater_UseCursorY(NUI_VALUPDATER *pvu){FLAG_OFF(pvu->UI.Flags,FLAG_NUI_VALUPDATER_USECURSORX);}


inline	Ns32	NUIValUpdater_GetCurrentValuei(const NUI_VALUPDATER *pvu){return (Ns32)pvu->CurrentValue;}
inline	Nf32	NUIValUpdater_GetCurrentValuef(const NUI_VALUPDATER *pvu){return pvu->CurrentValue;}

// LOW LEVEL
Nchar*			NUIValUpdater_SetupDisplayableFloatCastString(Nchar *pdest, const Nchar *pcaption, const Nf32 currentvalue, const Nu16 digitnumber);
Nchar*			NUIValUpdater_SetupDisplayableIntegerCastString(Nchar *pdest, const Nchar *pcaption, const Nf32 currentvalue);

// PRIVATE FUNCTIONS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUI_EXTENDEDH 
