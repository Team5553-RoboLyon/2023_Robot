// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NPostUpdate.cpp									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NFlags.h"
#include "../NType.h"
#include "../Containers/NArray.h"
#include "../Core/NTime.h"
#include "../NCore.h"
#include "../NPostUpdate.h"


NPOSTUPDATE	*NpPostUpdates;

Nbool NPostUpdatesInitialization()
{
	NpPostUpdates = (NPOSTUPDATE *)Nmalloc( sizeof(NPOSTUPDATE)*NPOSTUPDATE_ENUM_SIZE );
	if(!NpPostUpdates)
		return NFALSE;
	memset(NpPostUpdates,0,sizeof(NPOSTUPDATE)*NPOSTUPDATE_ENUM_SIZE);

	return NTRUE;
}

void NPostUpdatesDisable()
{
	Nfree(NpPostUpdates);
}

NPOSTUPDATE_HANDLE NBindPostUpdateHandle(const NPOSTUPDATEUID uid, const NPOSTUPDATE_HANDLE phandle)
{
	NPOSTUPDATE_HANDLE oldproc;

	oldproc = NpPostUpdates[uid].pHandle;
	NpPostUpdates[uid].pHandle = phandle;

	return oldproc;
}

Nu32 NBindPostUpdate_u32DataA(const NPOSTUPDATEUID uid,const Nu32 u32DataA)
{
	Nu32 old;

	old = NpPostUpdates[uid].u32_DataA;
	NpPostUpdates[uid].u32_DataA = u32DataA;

	return old;
}

Nu32 NBindPostUpdate_u32DataB(const NPOSTUPDATEUID uid,const Nu32 u32DataB)
{
	Nu32 old;

	old = NpPostUpdates[uid].u32_DataB;
	NpPostUpdates[uid].u32_DataB = u32DataB;

	return old;
}

void NPostUpdatePause(const NPOSTUPDATEUID uid)
{
	FLAG_OFF(NpPostUpdates[uid].Flags,FLAG_POSTUPDATE_PROCESS_ACTIVE);
}

void NPostUpdateResume(const NPOSTUPDATEUID uid)
{
	FLAG_ON(NpPostUpdates[uid].Flags,FLAG_POSTUPDATE_PROCESS_ACTIVE);
}


/*
// Globales variables
NARRAY *pBoundPostUpdateProcessAddressArray; // Store a pointer on a valid Array of pointers on NPOSTUPDATE_PROCESS structures.

NARRAY* NBindPostUpdateProcessAddressArray(NARRAY *parray)
{
	NARRAY *pold;

	pold = pBoundPostUpdateProcessAddressArray;
	pBoundPostUpdateProcessAddressArray = parray;

	return pold;
}

NPOSTUPDATE_PROCESS* NSetUpPostUpdateProcess(NPOSTUPDATE_PROCESS* pup, NPOSTUPDATE_FCT pprocess, Nu32 dataA, Nu32 dataB )
{
	pup->pProcess					= pprocess;
	pup->u32_DataA					= dataA;
	pup->u32_DataB					= dataB;
	return pup;
}

NPOSTUPDATE_PROCESS* NClearPostUpdateProcess(NPOSTUPDATE_PROCESS* pup)
{
	memset(pup,0,sizeof(NPOSTUPDATE_PROCESS));
	return pup;
}

NPOSTUPDATE_PROCESS* NCreatePostUpdateProcess( NPOSTUPDATE_FCT pprocess, Nu32 dataA, Nu32 dataB )
{
	NPOSTUPDATE_PROCESS	*pup;

	pup = (NPOSTUPDATE_PROCESS *)malloc(sizeof(NPOSTUPDATE_PROCESS));
	if(!pup)
		return NULL;

	if(!NSetUpPostUpdateProcess(pup,pprocess,dataA,dataB ))
	{
		free(pup);
		pup = NULL;
	}

	return pup;
}

Nbool NDeletePostUpdateProcess(NPOSTUPDATE_PROCESS*  ppup)
{
	if(!NClearPostUpdateProcess(ppup))
		return NFALSE;

	free(ppup);
	return NTRUE;
}

void NArrayDestructor_PostUpdateProcess(NBYTE *ptr)
{
	NClearPostUpdateProcess( (NPOSTUPDATE_PROCESS*)ptr );
	// !!! TODO: Manage the case : 'ClearFunction return NULL (clear aborts) ...'   
}
*/
