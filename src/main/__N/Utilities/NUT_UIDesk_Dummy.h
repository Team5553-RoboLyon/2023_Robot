#ifndef __NUT_UIDESK_DUMMY_H
#define __NUT_UIDESK_DUMMY_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_UIDesk_Dummy.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../Maths/NVec3f32.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct  
{
	NVEC3	WorldPosition;
	NVEC3	Size;
}NUT_UIDESK_DUMMY;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NUT_UIDESK_DUMMY*	NUT_UIDesk_ExtractUIDummy(NUT_UIDESK_DUMMY *pdummy);
NUT_UIDESK_DUMMY*	NUT_UIDesk_ExtractPanelDummy(NUT_UIDESK_DUMMY *pdummy);
inline NVEC3*	NUT_UIDesk_GetPositionFromDummy(NVEC3 *pposition,const Nf32 anchor_x, const Nf32 anchor_y, const Nf32 anchor_z,const NUT_UIDESK_DUMMY *pdummy)
{
	pposition->x = pdummy->WorldPosition.x + pdummy->Size.x*anchor_x;
	pposition->y = pdummy->WorldPosition.y + pdummy->Size.y*anchor_y;
	pposition->z = pdummy->WorldPosition.z + pdummy->Size.z*anchor_z;
	return pposition;
}

// PRIVATE/INTERNAL USE 
// These 2 functions are used to store needed dummy params at the right time inside the UIDesk_Set and UIDesk_Panel 
// positioning/sizing functions.
void			NUT_UIDesk_StoreUIDummyData(const NVEC3 *pposition,const NVEC3 *psize);
inline void		NUT_UIDesk_StoreUIDummyDataf(const Nf32 x,const Nf32 y, const Nf32 z, const Nf32 sx,const Nf32 sy, const Nf32 sz){NVEC3 pos;NVEC3 size;NVec3Set(&pos,x,y,z);NVec3Set(&size,sx,sy,sz);NUT_UIDesk_StoreUIDummyData(&pos,&size);}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_UIDESK_SET_H 

