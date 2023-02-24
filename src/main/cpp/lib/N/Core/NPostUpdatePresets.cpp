// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NPostUpdatePresets.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../Core/NTime.h"
#include "../NPostUpdate.h"
#include "../NPool.h"
#include "../NStructure.h"

#include "../NPostUpdatePresets.h"
/*
// =======================================================================================
// POOL POST UPDATE PROCESS
// =======================================================================================
static void _PoolPostUpdateProcess_FCT(NTIME *ptime, Nu32 data)
{
	NUpdatePool((NPOOL *)data);
}

static void _PoolPostUpdateProcess_ClearCallBack(Nu32 data)
{
	NDeletePool((NPOOL*)data);
}

NPOSTUPDATE_PROCESS *NCreatePostUpdateProcess_Pool(NPOOL *pool)
{
	return NCreatePostUpdateProcess( _PoolPostUpdateProcess_FCT,_PoolPostUpdateProcess_ClearCallBack ,(Nu32)pool );
}

void NDeletePostUpdateProcess_Pool(NPOSTUPDATE_PROCESS * pup)
{
	NDeletePostUpdateProcess(pup);
}

// =======================================================================================
// PHYSIC STUCTURES POST UPDATE PROCESS
// =======================================================================================
static NPOSTUPDATE_PROCESS	NPhysicStructurePostUpdateProcess; // Just for deleting the process and avoiding create it twice.
static void NPhysicStructuresPostUpdateProcess_FCT(NTIME *ptime, Nu32 data)
{
	NPhysicStructures_Update(ptime);
}

void NCreatePostUpdateProcess_PhysicStructures()
{
	if(!NPhysicStructurePostUpdateProcess)
		NPhysicStructurePostUpdateProcess = NCreatePostUpdateProcess(NPhysicStructuresPostUpdateProcess_FCT,NULL,0);
}

void NDeletePostUpdateProcess_PhysicStructures()
{
	if(NPhysicStructurePostUpdateProcess)
		NDeletePostUpdateProcess(NPhysicStructurePostUpdateProcess);
}

*/