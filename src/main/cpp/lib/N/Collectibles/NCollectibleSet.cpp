#include "../NCStandard.h"
#include "NCollectibles.h"


NCOLLECTIBLE_SET* NSetupCollectibleSet( NCOLLECTIBLE_SET *pcset, const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity )
{
	NSetupArray(&pcset->CollectiblePtrArray,(Nu32)collectible_ptr_capacity,sizeof(NCOLLECTIBLE*));
	NSetupArray(&pcset->PickerArray,picker_capacity,sizeof(NCOLLECTIBLE_PICKER));

	return pcset;
}

NCOLLECTIBLE_SET* NCreateCollectibleSet( const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity )
{
	return NSetupCollectibleSet( NEW( NCOLLECTIBLE_SET ),collectible_ptr_capacity,picker_capacity );
}

static void _erase_collectible_ptr(void *ptr)
{
	// set the collectible BindID with the "not bind code"
	(*(NCOLLECTIBLE**)ptr)->BindID	= 0xFFFF;
}

void NClearCollectibleSet(NCOLLECTIBLE_SET *pcset)
{
	NClearArray(&pcset->CollectiblePtrArray,_erase_collectible_ptr);
	NClearArray(&pcset->PickerArray,NULL);
}

void NDeleteCollectibleSet(NCOLLECTIBLE_SET *pcset)
{
	NClearCollectibleSet(pcset);
	Nfree(pcset);
}

