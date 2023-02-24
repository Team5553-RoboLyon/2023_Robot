#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/GL/NGL.h"
#include "lib/N/Containers/NNode.h"

#include "lib/N/Collectibles/NCollectibles.h"

NCOLLECTIBLE_PICKER *NSetupCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker, const NVEC3 *ppickerposition, const Nf32 radius)
{
	ppicker->Radius = radius;
	ppicker->pPosition = (NVEC3 *)ppickerposition;
	return ppicker;
}

void NClearCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker)
{
	Nmem0(ppicker, NCOLLECTIBLE_PICKER);
}

NCOLLECTIBLE_PICKER *NCreateCollectiblePicker(const NVEC3 *ppickerposition, const Nf32 radius)
{
	return NSetupCollectiblePicker(NEW(NCOLLECTIBLE_PICKER), ppickerposition, radius);
}

void NDeleteCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker)
{
	NClearCollectiblePicker(ppicker);
	Nfree(ppicker);
}
