#ifndef __NUT_SLIDER_H
#define __NUT_SLIDER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Slider.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NFlags.h"
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NScreenPoint.h"
#include "lib/N/NMatrix.h"

#include "lib/N/NErrorHandling.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DEFAULT_NUT_SLIDER_SIZE_2f 0.250f, 0.025f ///< A quarter of a screen height for length and 10% of that for thickness
#define DEFAULT_NUT_SLIDER_VDIR_2f 0.000f, 1.000f ///< Vertical "up" Direction

#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_NULL 0	///< at 0  (0%)		Snap is NULL. There is no snap at all.
#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_SUBTLE 16 ///< at 16 (6.25%)	Snap is subtlle. A less value ( 8 for example ) may work too but will be extremly subtle.
#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_LOW 32	///< at 32 (12.5%)	Snap is gentle.
#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM 64 ///< at 64 (25%)	Snap is strong.
#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_HIGH 96	///< at 32 (37.5%)	Snap is very strong.
#define CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MAX 128	///< from 128 (50%) Snap to slider ticks is "total".
													///< Pointer position is going to be attract by its low tick (50%) or by its high one (50%) without any "free" place between the 2.

#define FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE BIT_0	  ///< To print current output value as draw text @see NUT_ShowSliderOutputValue @see NUT_HideSliderOutputValue
#define FLAG_NUT_SLIDER_ORIENT_OUTPUTVALUE BIT_1  ///< To orient output value (draw text) along the Slider direction. @see NUT_OrientSliderOutputValue @see NUT_AlignSliderOutputValue
#define FLAG_NUT_SLIDER_SHOW_TICKS BIT_2		  ///< To draw step as ticks along slider (if there are steps obviously) @see NUT_ShowSliderTicks @see NUT_HideSliderTicks
#define FLAG_NUT_SLIDER_SNAP BIT_3				  ///< To Activate Deactivate SNAP mechanism @see NUT_ActivateSliderSnap @see NUT_DeactivateSliderSnap
#define FLAG_NUT_SLIDER_LOCKDOWN BIT_4			  ///< To lockdown slider inside Bounding Rect (  be aware that slider doesn't be locked down if the Bounding rect is too small )
												  ///< @see NUT_LockDownSlider @see NUT_UnlockDownSlider
#define FLAG_NUT_SLIDER_FIXED_BASE_POSITION BIT_5 ///< To fix the slider base at a specific position.
												  ///< This FLAG is automatically set to ON if user pass a valid NVEC2f32 pointer as 'pfixedbase position' in the followings:
												  ///< @see NUT_SetupSlider @see NUT_CreateSlider @see NUT_FixedSliderBasePosition
												  ///< ... and set it to OFF if user pass � NULL pointer.
#define FLAG_NUT_SLIDER_RELATIVE_HANDLE BIT_6	  ///< When user place a Slider, this one may have a fixed position ( FLAG_NUT_SLIDER_FIXED_BASE_POSITION set to ON )
												  ///< or a floating position recalculated at each placement call. In that latter case, Slider cursor is going to "jump" to stay on track
												  ///< with the pointer position. Set \e FLAG_NUT_SLIDER_RELATIVE_HANDLE to ON is going to avoid this "jump", Slider cursor is going to be driven
												  ///< from the pointer pickposition.
												  ///< @see NUT_EnableSliderRelativeHandle @see NUT_DisableSliderRelativeHandle
	// #define FLAG_NUT_SLIDER_xxx						BIT_7
	typedef struct
	{
		Nu8 Flags;		  ///< 8 Flags to customize Slider behavior and appearance.
		Nu8 SnapStrength; ///< 0 to NU8MAX (prefer using CONSTANT_NUT_SLIDER_SNAP_STRENGTH__xxx) to define the handle behavior regarding steps.
		Nu16 InterSteps;  ///< Number of intermediate steps along the Slider. Each one is visible as a tick
		Nf32 Min;		  ///< Slider minimum Value.
		Nf32 Max;		  ///< Slider maximum Value.
		// Internal
		Nf32 Magnitude;			 ///< Current Position of the cursor (slider handle) from Slider base along Slider axis.
		NVEC2f32 VDir;			 ///< Orient the slider from Origin (Min) to the positive direction ( to the Max)
		NVEC2f32 Size;			 ///< \e x is the 'length' of the slider. \e y is the 'thickness' of the slider.
		NVEC2f32 Base;			 ///< The slider 'start', where magnitude = 0 and value = Min.
		NVEC3f32 RelativeHandle; ///< Internal NVEC3f32 vector used by Slider functions when the flag FLAG_NUT_SLIDER_RELATIVE_HANDLE is set to ON.
		NRECTf32 LockedDownRect; ///< The slider will be placed "INSIDE" the lockedDownRect ( if it's possible ) by the function(s) NUT_PlaceSlider...
	} NUT_SLIDER;

	NUT_SLIDER *NUT_SetupSlider(NUT_SLIDER *pslider, const Nf32 valuemin, const Nf32 valuemax, const NVEC2 *pdirection, const NVEC2 *psize, const Nu16 intermediate_steps, const Nu8 snap_strength, const NVEC2f32 *pfixedbaseposition);
	NUT_SLIDER *NUT_CreateSlider(const Nf32 valuemin, const Nf32 valuemax, const NVEC2 *pdirection, const NVEC2 *psize, const Nu16 intermediate_steps, const Nu8 snap_strength, const NVEC2f32 *pfixedbaseposition);
	void NUT_ClearSlider(NUT_SLIDER *pslider);
	void NUT_DeleteSlider(NUT_SLIDER *pslider);

	void NUT_PlaceSliderOnScreenPlane(NUT_SLIDER *pslider, const Nf32 currentvalue, const NVEC2s16 *ppointer);
	Nf32 NUT_UpdateSliderOnScreenPlane(NUT_SLIDER *pslider, const NVEC2s16 *ppointer);

	void NUT_PlaceSlider(NUT_SLIDER *pslider, const Nf32 currentvalue, const NVEC2s16 *ppointer, const NMATRIX *pmtx);
	Nf32 NUT_UpdateSlider(NUT_SLIDER *pslider, const NVEC2s16 *ppointer, const NMATRIX *pmtx);

	inline void NUT_SetSliderLockedDownRect(NUT_SLIDER *ps, const NRECTf32 *prect, const Nbool bactivate)
	{
		ps->LockedDownRect = *prect;
		if (bactivate)
		{
			FLAG_ON(ps->Flags, FLAG_NUT_SLIDER_LOCKDOWN);
		}
		else
		{
			FLAG_OFF(ps->Flags, FLAG_NUT_SLIDER_LOCKDOWN);
		}
	}
	inline void NUT_SetSliderLockedDownRectf(NUT_SLIDER *ps, const Nf32 left, const Nf32 right, const Nf32 bottom, const Nf32 top, Nbool bactivate)
	{
		ps->LockedDownRect.left = left;
		ps->LockedDownRect.right = right;
		ps->LockedDownRect.bottom = bottom;
		ps->LockedDownRect.top = top;
		if (bactivate)
		{
			FLAG_ON(ps->Flags, FLAG_NUT_SLIDER_LOCKDOWN);
		}
		else
		{
			FLAG_OFF(ps->Flags, FLAG_NUT_SLIDER_LOCKDOWN);
		}
	}
	inline void NUT_SetSliderLimits(NUT_SLIDER *pslider, const Nf32 valuemin, const Nf32 valuemax)
	{
		NErrorIf(valuemin >= valuemax, NERROR_INCONSISTENT_VALUES);
		pslider->Min = valuemin;
		pslider->Max = valuemax;
	}
	inline void NUT_SetSliderDirection(NUT_SLIDER *pslider, const NVEC2 *pdir)
	{
		if (pdir)
		{
			pslider->VDir = *pdir;
			NVec2Normalize(&pslider->VDir);
		}
		else
		{
			NVec2Set(&pslider->VDir, DEFAULT_NUT_SLIDER_VDIR_2f);
		}
	}
	inline void NUT_SetSliderSize(NUT_SLIDER *pslider, const NVEC2 *psize)
	{
		if (psize)
		{
			pslider->Size = *psize;
		}
		else
		{
			NVec2Set(&pslider->Size, DEFAULT_NUT_SLIDER_SIZE_2f);
		}
	}
	inline void NUT_SetSliderSnapStrength(NUT_SLIDER *pslider, const Nu8 snapstrength) { pslider->SnapStrength = snapstrength; }
	inline void NUT_SetSliderIntermediateStepsNumber(NUT_SLIDER *pslider, const Nu16 intersteps) { pslider->InterSteps = intersteps; }

	inline void NUT_ActivateSliderProperties(NUT_SLIDER *pslider, const Nu8 flags_properties) { FLAG_ON(pslider->Flags, flags_properties); }
	inline void NUT_DeactivateSliderProperties(NUT_SLIDER *pslider, const Nu8 flags_properties) { FLAG_OFF(pslider->Flags, flags_properties); }

	inline void NUT_FixedSliderBasePosition(NUT_SLIDER *pslider, const NVEC2f32 *pv)
	{
		if (pv)
		{
			FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION);
			pslider->Base = *pv;
		}
		else
		{
			FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION);
			pslider->Base.x = 0.0f;
			pslider->Base.y = 0.0f;
		}
	}
	inline void NUT_LockDownSlider(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_LOCKDOWN); }
	inline void NUT_UnlockDownSlider(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_LOCKDOWN); }
	inline void NUT_ActivateSliderSnap(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_SNAP); }
	inline void NUT_DeactivateSliderSnap(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_SNAP); }
	inline void NUT_ShowSliderOutputValue(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE); }
	inline void NUT_HideSliderOutputValue(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE); }
	inline void NUT_ShowSliderTicks(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_SHOW_TICKS); }
	inline void NUT_HideSliderTicks(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_SHOW_TICKS); }
	inline void NUT_OrientSliderOutputValue(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_ORIENT_OUTPUTVALUE); }
	inline void NUT_AlignSliderOutputValue(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_ORIENT_OUTPUTVALUE); }
	inline void NUT_EnableSliderRelativeHandle(NUT_SLIDER *pslider) { FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_RELATIVE_HANDLE); }
	inline void NUT_DisableSliderRelativeHandle(NUT_SLIDER *pslider) { FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_RELATIVE_HANDLE); }

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_SLIDER_H
