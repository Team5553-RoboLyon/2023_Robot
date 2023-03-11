#ifndef __NPOSTUPDATE_H
#define __NPOSTUPDATE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NPostUpdate.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Core/NTime.h"
#include "NPostUpdatesUID.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **									 Structures										**
// ***************************************************************************************
// "Handle" function
typedef void (*NPOSTUPDATE_HANDLE)(const NTIME *ptime, Nu32 dataA, Nu32 dataB);

// a PostUpdate (Private struct)
#define FLAG_POSTUPDATE_PROCESS_ACTIVE		BIT_0
typedef struct
{
	Nu32						Flags;
	NPOSTUPDATE_HANDLE			pHandle;
	Nu32						u32_DataA;
	Nu32						u32_DataB;
}NPOSTUPDATE;


// ***************************************************************************************
// **									 Functions										**
// ***************************************************************************************
Nbool				NPostUpdatesInitialization();
void				NPostUpdatesDisable();

NPOSTUPDATE_HANDLE	NBindPostUpdateHandle(const NPOSTUPDATEUID uid, const NPOSTUPDATE_HANDLE phandle);
Nu32				NBindPostUpdate_u32DataA(const NPOSTUPDATEUID uid, const Nu32 u32DataA);
Nu32				NBindPostUpdate_u32DataB(const NPOSTUPDATEUID uid, const Nu32 u32DataB);

void				NPostUpdatePause(const NPOSTUPDATEUID uid);
void				NPostUpdateResume(const NPOSTUPDATEUID uid);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NPOSTUPDATE_H 

