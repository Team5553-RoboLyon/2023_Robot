#include "../NCStandard.h"
#include "../NType.h"
#include "../NFlags.h"
#include "../NStructure.h"

// This is a little bit special for Physic elements because they have to be updated all together with a special time !
// So keeping a pointer on it for that into a specific array to update all of them in the the same specific time loop.
//
void NRenderableUpdate_PhysicStructure(NRENDERABLE *prenderable, void* powner, const NTIME *ptime)
{
	if( prenderable->Extract_FCT(prenderable,powner) )
	{
		NArrayPushBack(&NActivePhysicStructuresPtr_Array,(NBYTE*)&powner);
		FLAG_ON(((NSTRUCTURE*)powner)->Flags, FLAG_NSTRUCTURE_ACTIVE);
	}
}
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

