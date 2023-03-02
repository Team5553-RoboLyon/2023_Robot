#include "lib/N/NCStandard.h"
#include "../NCoordinateSystemConversions.h"
#include "../Utilities/NUT_X.h"
#include "NUI.h"
#include "NUISlide.h"

void NInsertUISlidePin(NUI_SLIDE *pslayer, const NVEC3 *pposition, const NVEC3 *pspeed_threshold_dir, const Nf32 speed_threshold, const Nf32 captureradius, const Nbool conform_to_constraint_context)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + STEP 1/ Setup and Conform Pin position to potential Constraint AABB or Spline or Segment if user asks for.
	// +
	NUI_SLIDE_PIN pin;
	Nmem0(&pin, NUI_SLIDE_PIN);
	pin.Position = *pposition;
	if (conform_to_constraint_context)
	{
		NConformPositionToConstraintContext(&pin.Position, pslayer);
	}
	if (pspeed_threshold_dir)
	{
		FLAG_ON(pin.Flags, FLAG_NUI_SLIDE_PIN_USE_SPEED_THRESHOLD_DIRECTION);
		pin.SpeedThresholdDirection = *pspeed_threshold_dir;
		NVec3Normalize(&pin.SpeedThresholdDirection);
	}
	pin.SquareCaptureRadius = captureradius * captureradius;
	pin.SquareSpeedThreshold = speed_threshold * speed_threshold;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + STEP 2/ Debug only. Advanced Check !
// + We are going to check if an already inserted pin has almost the same position.
// + An Error occurs if we found one.
// +
#ifdef _DEBUG
	NVEC3 dbg_v;
	NUI_SLIDE_PIN *dbg_pin = (NUI_SLIDE_PIN *)pslayer->PinArray.pFirst;
	for (Nu32 dbg_i = pslayer->PinArray.Size; dbg_i != 0; dbg_i--, dbg_pin++)
	{
		NVec3Sub(&dbg_v, pposition, &dbg_pin->Position);
		if (NVec3Length(&dbg_v) <= CONSTANT_NUI_SLIDE_EPSILON_SNAPPOINT_DISTANCE)
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
	}
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + STEP 3/ Insert !!!
	// +
	/*NUI_SLIDE_PIN *ppin = */ (NUI_SLIDE_PIN *)NArrayPushBack(&pslayer->PinArray, (NBYTE *)&pin);
}
