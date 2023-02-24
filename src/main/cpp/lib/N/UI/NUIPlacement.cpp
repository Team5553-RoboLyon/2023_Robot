// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NUIPlacementDesc.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "NUI.h"

void NGetUIPlacementAbsoluteComputedPosition(NVEC3 *presult, const NUIPLACEMENT *pplac)
{
	NVEC3 from;

	if (pplac->pParent)
	{
		NGetUIPlacementAbsoluteComputedPosition(&from, pplac->pParent);

		from.x += pplac->pParent->Size.x * (pplac->ParentAnchor.x - pplac->pParent->Anchor.x); // |
		from.y += pplac->pParent->Size.y * (pplac->ParentAnchor.y - pplac->pParent->Anchor.y); //	|- We always want position of the left-bottom-back corner !
		from.z += pplac->pParent->Size.z * (pplac->ParentAnchor.z - pplac->pParent->Anchor.z); // |	So, we remove Anchor for that reason
	}
	else
		NVec3Set(&from, 0, 0, 0);

	presult->x = from.x + pplac->RelativePosition.x;
	presult->y = from.y + pplac->RelativePosition.y;
	presult->z = from.z + pplac->RelativePosition.z;
}

void NGetUIPlacementShifting(NVEC3 *presult, const NUIPLACEMENT *pplac)
{
	NVEC3 abspos;

	NGetUIPlacementAbsoluteComputedPosition(&abspos, pplac);

	presult->x = abspos.x - pplac->Anchor.x * pplac->Size.x;
	presult->y = abspos.y - pplac->Anchor.y * pplac->Size.y;
	presult->z = abspos.z - pplac->Anchor.z * pplac->Size.z;
}
