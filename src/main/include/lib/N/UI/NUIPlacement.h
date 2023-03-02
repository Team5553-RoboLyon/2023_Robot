#ifndef __NUI_PLACEMENT_H
#define __NUI_PLACEMENT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUIPlacement.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	typedef struct NUIPLACEMENT NUIPLACEMENT;
	struct NUIPLACEMENT
	{
		NUIPLACEMENT *pParent;	// Pointer on the Object Parent
		NVEC3 ParentAnchor;		// Origin Point relative to the Object Parent Base
		NVEC3 Anchor;			// Anchor Point relative to the Object Base
		NVEC3 RelativePosition; // Anchor Point Coordinates from Origin Point
		NVEC3 Size;				// Size of the Object
	};

	// UI graphic elements placement
	// inline void			NSetUIPlacementDescRenderingOrder(NUIPLACEMENT *pplac,const Nu32 order){pplac->RenderingOrder = order;}
	inline void NSetUIPlacementParent(NUIPLACEMENT *pplac, NUIPLACEMENT *ppar, const NVEC3 *panchor)
	{
		pplac->pParent = ppar;
		pplac->ParentAnchor = *panchor;
	}
	inline void NSetUIPlacementParentf(NUIPLACEMENT *pplac, NUIPLACEMENT *ppar, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		pplac->pParent = ppar;
		pplac->ParentAnchor.x = x;
		pplac->ParentAnchor.y = y;
		pplac->ParentAnchor.z = z;
	}

	inline void NSetUIPlacementAnchor(NUIPLACEMENT *pplac, const NVEC3 *panchor) { pplac->Anchor = *panchor; }
	inline void NSetUIPlacementAnchorf(NUIPLACEMENT *pplac, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		pplac->Anchor.x = x;
		pplac->Anchor.y = y;
		pplac->Anchor.z = z;
	}

	inline void NSetUIPlacementRelativePosition(NUIPLACEMENT *pplac, const NVEC3 *ppos) { pplac->RelativePosition = *ppos; }
	inline void NSetUIPlacementRelativePositionf(NUIPLACEMENT *pplac, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		pplac->RelativePosition.x = x;
		pplac->RelativePosition.y = y;
		pplac->RelativePosition.z = z;
	}

	inline void NSetUIPlacementSize(NUIPLACEMENT *pplac, const NVEC3 *psize) { pplac->Size = *psize; }
	inline void NSetUIPlacementSizef(NUIPLACEMENT *pplac, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		pplac->Size.x = x;
		pplac->Size.y = y;
		pplac->Size.z = z;
	}

	void NGetUIPlacementAbsoluteComputedPosition(NVEC3 *presult, const NUIPLACEMENT *pplac);
	void NGetUIPlacementShifting(NVEC3 *presult, const NUIPLACEMENT *pplac);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUI_PLACEMENT_H
