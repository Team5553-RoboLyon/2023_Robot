#include "../NCoordinateSystemConversions.h"
#include "../NRectf32.h"
#include "../Utilities/NUT_3DMouse.h"
#include "../Utilities/NUT_X.h"

#include "NUT_Slider.h"

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Setup a \b NUT_SLIDER structure with the incoming arguments.
 *			
 *	@param valuemin				Minimum value in the range of [valuemin, valuemax]. \e NUT_UpdateSlider function will return this value when cursor is at its lower position.
 *	@param valuemax				Maximum value in the range of [valuemin, valuemax]. \e NUT_UpdateSlider function will return this value when cursor is at its higher position.
 *	@param direction			A slider is like a straigth line ( with some ticks on it ). \e pdirection is the 2D direction of this line, oriented from slider base to its top.
 *								It's not necessary to normalize \b pdirection, the function is going to normalize it systematically.
 *	@param psize				pointer on a NVEC2f32 structure which represents the Slider Size. The \e x dimension represents the length and \e y dimension the width (= ticks length)
 *	@param intermediate_steps	Number of intermediate steps along the slider. This value represent the number of ticks spread along the slider the 2 ones which they are of each end not included.
 *								This unique step has a length equal to the whole slider length ( = psize.x ).
 *								So, with 2 intermediate steps, a slider is going to have 3 ticks, with the 2 end ticks included. 4 ticks with 3 intermediate steps, and so on...
 *	@param snap_strength		a 8 bits value which represents a % of the intermediate step segment length . Internal operation to figure out this % is : \b snap_strength/255
 *								Cursor is going to be snapped to a specific tick if the distance from the cursor to this tick is lower than this %.
 *								Use on of the predefine values :
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_NULL
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_SUBTLE
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_LOW
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_HIGH
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MAX
 *
 *	@param pfixedbaseposition	A pointer to a NVECf32 structure which contains the slider base 2D position on the choosen Drawing Base XY Plane.
 *								If \e pfixedbaseposition is NULL, the Slider is going to be considered as a \b Floating Slider. That means the position of it's base is going to be recalculated
 *								at each \e NUT_PlaceSlider call. A Floating Slider is build in a specific manner to place the slider cursor at the  pointer position ( = mouse position ).
 *								If \e pfixedbaseposition is NON NULL, the Slider is going to be considered as a \e Fixed Slider and always placed at this position, whatever the pointer position is.
 *
 *	@return						The function returns \e pslider itself with all its members setup accordingly with the incommming parameters.							 	
 */
// ------------------------------------------------------------------------------------------
NUT_SLIDER* NUT_SetupSlider(NUT_SLIDER* pslider,const Nf32 valuemin, const Nf32 valuemax,const NVEC2* pdirection,const NVEC2* psize, const Nu16 intermediate_steps, const Nu8 snap_strength, const NVEC2f32 *pfixedbaseposition)
{
	Nmem0(pslider,NUT_SLIDER);
	NUT_SetSliderLimits(pslider,valuemin,valuemax);
	NUT_SetSliderSize(pslider,psize);
	NUT_SetSliderDirection(pslider,pdirection);
	pslider->InterSteps		= intermediate_steps;
	pslider->SnapStrength	= snap_strength;
	
	// Default properties:
	// Analyzing some values to decide properties default state (activate or not)
	FLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE);	// by default ON !
	if(pslider->SnapStrength)
		FLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SNAP);			// ON only if Snap Strength may have an effect	
	if(pslider->InterSteps)
		FLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_TICKS);		// ON only if there is something to draw ...	
	FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_LOCKDOWN);			// by default OFF !

	if (pfixedbaseposition)
	{
		FLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION); pslider->Base = *pfixedbaseposition;
	}
	else
	{
		FLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION);
	}

	return pslider;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Create a \b NUT_SLIDER structure with the incoming arguments.
 *
 *	@param pslider	pointer on the slider structure to setup.
 *	@param valuemin				Minimum value in the range of [valuemin, valuemax]. \e NUT_UpdateSlider function will return this value when cursor is at its lower position.
 *	@param valuemax				Maximum value in the range of [valuemin, valuemax]. \e NUT_UpdateSlider function will return this value when cursor is at its higher position.
 *	@param direction			A slider is like a straigth line ( with some ticks on it ). \e pdirection is the 2D direction of this line, oriented from slider base to its top.
 *								It's not necessary to normalize \b pdirection, the function is going to normalize it systematically.
 *	@param psize				pointer on a NVEC2f32 structure which represents the Slider Size. The \e x dimension represents the length and \e y dimension the width (= ticks length)
 *	@param intermediate_steps	Number of intermediate steps along the slider. This value represent the number of ticks spread along the slider the 2 ones which they are of each end not included.
 *								This unique step has a length equal to the whole slider length ( = psize.x ).
 *								So, with 2 intermediate steps, a slider is going to have 3 ticks, with the 2 end ticks included. 4 ticks with 3 intermediate steps, and so on...
 *	@param snap_strength		a 8 bits value which represents a % of the intermediate step segment length . Internal operation to figure out this % is : \b snap_strength/255
 *								Cursor is going to be snapped to a specific tick if the distance from the cursor to this tick is lower than this %.
 *								Use on of the predefine values :
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_NULL
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_SUBTLE
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_LOW
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_HIGH
 *								+ \e CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MAX
 *
 *	@param pfixedbaseposition	A pointer to a NVECf32 structure which contains the slider base 2D position on the choosen Drawing Base XY Plane.
 *								If \e pfixedbaseposition is NULL, the Slider is going to be considered as a \b Floating Slider. That means the position of it's base is going to be recalculated
 *								at each \e NUT_PlaceSlider call. A Floating Slider is build in a specific manner to place the slider cursor at the  pointer position ( = mouse position ).
 *								If \e pfixedbaseposition is NON NULL, the Slider is going to be considered as a \e Fixed Slider and always placed at this position, whatever the pointer position is.
 *
 *	@return						The function returns a pointer on a new Slider structure with all its members setup accordingly with the incommming parameters.
 */
 // ------------------------------------------------------------------------------------------
NUT_SLIDER* NUT_CreateSlider(const Nf32 valuemin, const Nf32 valuemax,const NVEC2* pdirection,const NVEC2* psize, const Nu16 intermediate_steps, const Nu8 snap_strength, const NVEC2f32 *pfixedbaseposition)
{
	return NUT_SetupSlider(NEW(NUT_SLIDER),valuemin,valuemax,pdirection,psize,intermediate_steps,snap_strength, pfixedbaseposition);
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Clear a \b NUT_SLIDER structure.
 *
 *	@param	\e pslider					pointer on the slider structure to clear.
 */
 // ------------------------------------------------------------------------------------------
void NUT_ClearSlider(NUT_SLIDER *pslider)
{
	Nmem0(pslider,NUT_SLIDER);
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Delete a \b NUT_SLIDER structure.
 *			As usual structure is going to be Cleared before the Nfree call.
 *
 *	@param	\e pslider					pointer on the slider structure to delete.
 */
 // ------------------------------------------------------------------------------------------
void NUT_DeleteSlider(NUT_SLIDER *pslider)
{
	NUT_ClearSlider(pslider);
	Nfree(pslider);
}

// ------------------------------------------------------------------------------------------
// NUT_PlaceSliderOnScreenPlane
// ------------------------------------------------------------------------------------------
//		Put in place the slider on the XY Plane of the screen ( with origin 0,0,0)
//		And compute the slider placement.
//		In that way, current value is going to be exactly "under" the "ppointer" screen position.
// ------------------------------------------------------------------------------------------
// In  :
// Out :
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Put in place the slider on the screen plane.
 *
 *			And compute the slider placement.
 *			In that way, current value is going to be exactly "under" the "ppointer" screen position.
 *	@param	pslider		pointer on the slider structure to delete.
 */
 // ------------------------------------------------------------------------------------------
void NUT_PlaceSliderOnScreenPlane(NUT_SLIDER *pslider, const Nf32 currentvalue, const NVEC2s16 *ppointer)
{
	NVEC2f32 sliderpointer,vmag;

	// Get the current Slider value
	Nf32 incoming_val = NCLAMP(pslider->Min,currentvalue,pslider->Max);
	
	// 'Build' the slider, that means compute the slider base position from the data we have.
	// That means from current value, pointer position and slider direction.
	// Lets say that at activation, Slider's handle is right under the pointer, so ...
	// 
	//	(currentvalue - pslider->Min)	current value 'from' min 
	//	pslider->Size.x					'magnitude' of the Slider itself (it's 'length' on screen)
	//	(pslider->Max - pslider->Min)	'magnitude' of the slider value
	//	f								in this calculus f represents the current value On the slider (from the slider Origin)
	//									... and thats exactly what we are looking for... THE SLIDER ORIGIN !
	pslider->Magnitude		= (incoming_val - pslider->Min)*pslider->Size.x/(pslider->Max - pslider->Min);
	vmag.x = pslider->Magnitude*pslider->VDir.x;
	vmag.y = pslider->Magnitude*pslider->VDir.y;

	sliderpointer.x = NCoord_PixToHRel_X(ppointer->x);
	sliderpointer.y = NCoord_PixToHRel_Y(ppointer->y);

	if (ISFLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION))
	{
		// So retrieve slider base position from pointer position
		pslider->Base.x = sliderpointer.x - vmag.x;
		pslider->Base.y = sliderpointer.y - vmag.y;
	}
	/*else
	{
	  Nothing to do, pslider->Base remains unchanged.
	}
	 */

	if (ISFLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_LOCKDOWN))
	{
		NRECTf32	baserect = pslider->LockedDownRect;
		NVEC2f32	v;

		// To lock down the whole slider segment in a rect, define the baserect where the Slider base is lock down in.
		// This rect is just the LockedDownRect minus the bounding rect size of the Slider.
		v.x = pslider->Size.x * pslider->VDir.x;
		v.y = pslider->Size.x * pslider->VDir.y;
		
		if (v.x >= 0.0f)
			baserect.right	-= v.x;
		else
			baserect.left	-= v.x;

		if (v.y >= 0.0f)
			baserect.top	-= v.y;
		else
			baserect.bottom	-= v.x;

		if ((baserect.right < baserect.left) || (baserect.top < baserect.bottom))
			return;

		pslider->Base.x = NCLAMP(baserect.left, pslider->Base.x, baserect.right);
		pslider->Base.y = NCLAMP(baserect.bottom, pslider->Base.y, baserect.top);
	}

	if ( ISFLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_RELATIVE_HANDLE) )
	{
		pslider->RelativeHandle.x = (pslider->Base.x + vmag.x) - sliderpointer.x;
		pslider->RelativeHandle.y = (pslider->Base.y + vmag.y) - sliderpointer.y;
		pslider->RelativeHandle.z = 0.0f;
	}
	else
	{
		pslider->RelativeHandle.x = 0.0f;
		pslider->RelativeHandle.y = 0.0f;
		pslider->RelativeHandle.z = 0.0f;
	}
}

// ------------------------------------------------------------------------------------------
// NUT_PlaceSlider
// ------------------------------------------------------------------------------------------
//		Put in place the slider on the XY Plane of the screen ( with origin 0,0,0)
//		And compute the slider origin ( which is not the Base Origin of course ).
//		In that way, current value is going to be exactly "under" the "ppointer" screen position.
// ------------------------------------------------------------------------------------------
// In  :
// Out :
// ------------------------------------------------------------------------------------------
void NUT_PlaceSlider(NUT_SLIDER *pslider, const Nf32 currentvalue, const NVEC2s16 *ppointer, const NMATRIX *pmtx)
{
	// Get the current Slider value
	Nf32 incoming_val = NCLAMP(pslider->Min, currentvalue, pslider->Max);

	// 'Build' the slider, that means compute the slider Origin from the data we have.
	// That means from current value, pointer position and slider direction.
	// Lets say that at activation, Slider's handle is right under the pointer, so ...
	// 
	//	(currentvalue - pslider->Min)	current value 'from' min 
	//	pslider->Size.x					'magnitude' of the Slider itself (it's 'length' on screen)
	//	(pslider->Max - pslider->Min)	'magnitude' of the slider value
	//	f								in this calculus f represents the current value On the slider (from the slider Origin)
	//									... and thats exactly what we are looking for... THE SLIDER ORIGIN !
	
	NVEC3f32	vmag3f32;				// (WORLD base )3D  vector of the slider magnitude. (WORLD base )
	NVEC3f32	vcursor3f32;			// (WORLD base )3D position of the Slider cursor ( slider base + slider magnitude )
	NVEC3f32	sliderpointer3f32;		// (WORLD base )3D intersection point between the XY plane associated with pmtx and the 3D mouse Ray
	NVEC3f32	vbase3f32;				// (WORLD base )3D position of the Slider base.
	Nf32		x, y;
	Nbool		blockdown = 0;

	pslider->Magnitude = (incoming_val - pslider->Min)*pslider->Size.x / (pslider->Max - pslider->Min);
	x = pslider->Magnitude*pslider->VDir.x;
	y = pslider->Magnitude*pslider->VDir.y;

	vmag3f32.x = x * pmtx->XAxis.x + y * pmtx->YAxis.x; // - 0.0f * pmtx->ZAxis.x;	| VDir.z doesn't exist, because slider is a planar shape included in the XY plane of incomming matrix. 
	vmag3f32.y = x * pmtx->XAxis.y + y * pmtx->YAxis.y; // - 0.0f * pmtx->ZAxis.y;	| So, all the points included in this plane have z = 0. 
	vmag3f32.z = x * pmtx->XAxis.z + y * pmtx->YAxis.z; // - 0.0f * pmtx->ZAxis.z;	|

	NVEC3f32	o, dir;
	NUT_Get3DMouseRay(&o, &dir, NULL, ppointer);
	if (!NUT_LineXPLane(&o, &dir, (NVEC3f32*)&pmtx->Origin, (NVEC3f32*)&pmtx->ZAxis, &sliderpointer3f32))
	{
		// There is no intersection between pointer Ray and XY Plane !
		// Assume this default situation means position at Matrix origin (0,0,0).
		sliderpointer3f32.x = 0.0f;
		sliderpointer3f32.y = 0.0f;
		sliderpointer3f32.z = 0.0f;
	}

	if (ISFLAG_OFF(pslider->Flags, FLAG_NUT_SLIDER_FIXED_BASE_POSITION))
	{
		// sliderpointer3f32 is the intersection point between the XY plane associated with pmtx and the 3D mouse Ray
		// So lets build 'so' the 3D Base of the Slider from incomming Matrix Origin
		vbase3f32.x = sliderpointer3f32.x - vmag3f32.x - pmtx->Origin.x;
		vbase3f32.y = sliderpointer3f32.y - vmag3f32.y - pmtx->Origin.y;
		vbase3f32.z = sliderpointer3f32.z - vmag3f32.z - pmtx->Origin.z;

		// Slider is especially build in that way ( to have slider cursor exactly 'under' the pointer position
		vcursor3f32 = sliderpointer3f32;

		// pslider->Base, 2D coordinates of the Slider base in the XY Plane of the associated MATRIX.
		pslider->Base.x = NVec3DotProduct(&vbase3f32, (NVEC3f32*)&pmtx->XAxis);
		pslider->Base.y = NVec3DotProduct(&vbase3f32, (NVEC3f32*)&pmtx->YAxis);
	}
	/*else
	{
	  Nothing to do, pslider->Base remains unchanged.
	}
	*/

	// check for potential lockdown.
	// Assume that check is not necessary if Slider base is in fixed mode.
	if (ISFLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_LOCKDOWN))
	{
		NRECTf32	baserect = pslider->LockedDownRect;
		NVEC2f32	v;

		// To lock down the whole slider segment in a rect, define the baserect where the Slider base is lock down in.
		// This rect is just the LockedDownRect minus the bounding rect size of the Slider.
		v.x = pslider->Size.x * pslider->VDir.x;
		v.y = pslider->Size.x * pslider->VDir.y;

		if (v.x >= 0.0f)
			baserect.right -= v.x;
		else
			baserect.left -= v.x;

		if (v.y >= 0.0f)
			baserect.top -= v.y;
		else
			baserect.bottom -= v.x;

		if ((baserect.right < baserect.left) || (baserect.top < baserect.bottom))
			return;

		pslider->Base.x = NCLAMP(baserect.left, pslider->Base.x, baserect.right);
		pslider->Base.y = NCLAMP(baserect.bottom, pslider->Base.y, baserect.top);

		blockdown = 1;
	}

	if (ISFLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_RELATIVE_HANDLE))
	{
		if (blockdown)
		{
			// Rebuild 'vbase3f32' and 'vcursor3f32' to integrate  lock down restriction
			vbase3f32.x = pmtx->Origin.x + pslider->Base.x * pmtx->XAxis.x + pslider->Base.y * pmtx->YAxis.x; // - 0.0f * pmtx->ZAxis.x;
			vbase3f32.y = pmtx->Origin.y + pslider->Base.x * pmtx->XAxis.y + pslider->Base.y * pmtx->YAxis.y; // - 0.0f * pmtx->ZAxis.y;
			vbase3f32.z = pmtx->Origin.z + pslider->Base.x * pmtx->XAxis.z + pslider->Base.y * pmtx->YAxis.z; // - 0.0f * pmtx->ZAxis.z;

			vcursor3f32.x = vbase3f32.x + vmag3f32.x;
			vcursor3f32.y = vbase3f32.y + vmag3f32.y;
			vcursor3f32.z = vbase3f32.z + vmag3f32.z;
		}

		pslider->RelativeHandle.x = vcursor3f32.x - sliderpointer3f32.x;
		pslider->RelativeHandle.y = vcursor3f32.y - sliderpointer3f32.y;
		pslider->RelativeHandle.z = vcursor3f32.z - sliderpointer3f32.z;
	}
	else
	{
		pslider->RelativeHandle.x = 0.0f;
		pslider->RelativeHandle.y = 0.0f;
		pslider->RelativeHandle.z = 0.0f;
	}
}

Nf32 NUT_UpdateSliderOnScreenPlane(NUT_SLIDER *pslider,const NVEC2s16 *ppointer)
{
	NVEC2	v;
	Nf32		f;

	v.x =  NCoord_PixToHRel_X(ppointer->x) - pslider->Base.x + pslider->RelativeHandle.x;
	v.y =  NCoord_PixToHRel_Y(ppointer->y) - pslider->Base.y + pslider->RelativeHandle.y;
	f = NCLAMP(0.0f,NVec2DotProduct(&v,&pslider->VDir),pslider->Size.x);
	
	// Snap ?
	// Take care of SnapStrength and InterSteps values to a good result.
	if( ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SNAP) )
	{
		Nf32 fstep	= pslider->Size.x/(Nf32)(pslider->InterSteps+1);
		Nf32 fsnap	= fstep*(Nf32)pslider->SnapStrength/255.0f;
		Nf32 f2		= floor( f/fstep + 0.5f )*fstep;
		
		if( NABS(f-f2)<=fsnap )
			f = f2;
	}
	pslider->Magnitude = f;
	return  pslider->Min + ( pslider->Max - pslider->Min )*f/pslider->Size.x;
}

Nf32 NUT_UpdateSlider(NUT_SLIDER *pslider, const NVEC2s16 *ppointer, const NMATRIX *pmtx)
{
	Nf32		f;
	NVEC3f32	o, dir, vrp, sliderpointer3f32;
	NVEC3f32	so,sv;
	
	NUT_Get3DMouseRay(&o, &dir, NULL, ppointer);
	// sliderpointer3f32 is the intersection point between the XY plane associated with pmtx and the 3D mouse Ray.
	if (NUT_LineXPLane(&o, &dir, (NVEC3f32*)&pmtx->Origin, (NVEC3f32*)&pmtx->ZAxis, &sliderpointer3f32))
	{
		sliderpointer3f32.x += pslider->RelativeHandle.x;
		sliderpointer3f32.y += pslider->RelativeHandle.y;
		sliderpointer3f32.z += pslider->RelativeHandle.z;

		// So, lets build 'so' the 3D base position of the Slider from the matrix Origin.
		so.x = pmtx->Origin.x + pslider->Base.x* pmtx->XAxis.x + pslider->Base.y * pmtx->YAxis.x; // + 0.0f * pmtx->ZAxis.x;	| 
		so.y = pmtx->Origin.y + pslider->Base.x* pmtx->XAxis.y + pslider->Base.y * pmtx->YAxis.y; // + 0.0f * pmtx->ZAxis.y;	|-VDir.z doesn't exist! Slider is a planar shape ! so, VDir.z = 0.0f !
		so.z = pmtx->Origin.z + pslider->Base.x* pmtx->XAxis.z + pslider->Base.y * pmtx->YAxis.z; // + 0.0f * pmtx->ZAxis.z;	|
		// and, lets build 'sv' the 3D Slider VDir from the matrix Origin.
		sv.x = pslider->VDir.x* pmtx->XAxis.x + pslider->VDir.y * pmtx->YAxis.x; // + 0.0f * pmtx->ZAxis.x;	| 
		sv.y = pslider->VDir.x* pmtx->XAxis.y + pslider->VDir.y * pmtx->YAxis.y; // + 0.0f * pmtx->ZAxis.y;	|- VDir.z doesn't exist, Slider is a planar shape ! so, pslider->VDir.z = 0.0f !
		sv.z = pslider->VDir.x* pmtx->XAxis.z + pslider->VDir.y * pmtx->YAxis.z; // + 0.0f * pmtx->ZAxis.z;	|
		NVec3Normalize(&sv);

		// Position of intersection Point between pointer Ray and XY plane, relative to the 3D base position from the matrix Origin. 
		vrp.x = sliderpointer3f32.x - so.x;
		vrp.y = sliderpointer3f32.y - so.y;
		vrp.z = sliderpointer3f32.z - so.z;

		// vrp Projection on 'sv'
		f = NCLAMP(0.0f, NVec3DotProduct(&sv, &vrp), pslider->Size.x);

		// Snap ?
		// Take care of SnapStrength and InterSteps values to a good result.
		if (ISFLAG_ON(pslider->Flags, FLAG_NUT_SLIDER_SNAP))
		{
			Nf32 fstep = pslider->Size.x / (Nf32)(pslider->InterSteps + 1);
			Nf32 fsnap = fstep * (Nf32)pslider->SnapStrength / 255.0f;
			Nf32 f2 = floor(f / fstep + 0.5f)*fstep;

			if (NABS(f - f2) <= fsnap)
				f = f2;
		}

		pslider->Magnitude = f;
		return  pslider->Min + (pslider->Max - pslider->Min)*f / pslider->Size.x;
	}
	else
	{
		// There is no intersection between pointer Ray and XY Plane !
		// Assume this default situation means a NULL magnitude.
		pslider->Magnitude = 0.0f;
		return  pslider->Min;
	}
}
