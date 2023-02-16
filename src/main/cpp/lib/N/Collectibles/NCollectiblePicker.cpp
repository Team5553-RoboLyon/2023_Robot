#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../Geometry/NGeometry.h"
#include "../Containers/NNode.h"

#include "NCollectibles.h"

NCOLLECTIBLE_PICKER*	NSetupCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker,const NVEC3 *ppickerposition,const Nf32 radius)
{
	ppicker->Radius			= radius;
	ppicker->pPosition		= (NVEC3*)ppickerposition;
	return ppicker;
}

void	NClearCollectiblePicker(NCOLLECTIBLE_PICKER* ppicker)
{
	Nmem0(ppicker,NCOLLECTIBLE_PICKER);
}

NCOLLECTIBLE_PICKER*	NCreateCollectiblePicker(const NVEC3 *ppickerposition,const Nf32 radius)
{
	return NSetupCollectiblePicker( NEW(NCOLLECTIBLE_PICKER), ppickerposition,radius );
}

void	NDeleteCollectiblePicker(NCOLLECTIBLE_PICKER* ppicker)
{
	NClearCollectiblePicker(ppicker);
	Nfree(ppicker);
}
