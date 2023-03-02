#ifndef __NPOOL_H
#define __NPOOL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NPool.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NCStandard.h"
#include "NType.h"
#include "NFlags.h"
#include "./Containers/NNode.h"
#include "./Containers/NXNode.h"
#include "./Containers/NArray.h"
#include "./Maths/NVec3f32.h"
#include "./Core/NTime.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NPOOLXNODELIST_ARRAYCAPACITY			4
#define NPOOLTAG_TYPE_VALUEMAX					255
typedef struct  
{
	NNODEDEF
	
	Nu8			Type;				// 0 to 255 = NPOOLTAG_TYPE_VALUEMAX
	//--------------------------------------
	Nu8			User_8;				// 8  bits User value
	Nu16		User_16;			// 16 bits User value
	//--------------------------------------
	Nu32		A_User_32;			// 32 bits User value
	Nu32		B_User_32;			// 32 bits User value
	//--------------------------------------
	NVEC3	Pos;
	NXNODE		*pLinkedXNode;
}NPOOLTAG;

//typedef void (*NPOOL_TAGLINK_CALLBACK)( NPOOLTAG *ptag, Nu32 userdata );
//typedef void (*NPOOL_NODELISTINSERT_CALLBACK)( NNODE *pnode, Nu32 userdata );
//typedef void (*NPOOL_NODEENABLE_CALLBACK)( NPOOLTAG *ptag, Nu32 userdata );
typedef void (*NPOOL_XNODEENABLE_CALLBACK)( NPOOLTAG *ptag, Nu32 userdata );

typedef struct  
{
	NXNODELIST						*pEnableXNodeList;
	Nf32							DistanceActivation;
	Nf32							DistanceInactivation;
}NPOOLDESC;


typedef struct  
{
	NXNODELIST						*pXNodeList;
	NPOOL_XNODEENABLE_CALLBACK		_XNodeEnable_CallBack;
	NPOOL_XNODEENABLE_CALLBACK		_XNodeDisable_CallBack;
	Nf32							DetectionRadius;		// Radius used to activate tag(or not) with a type which links it with this NPOOLXNODELIST  
	Nu32							UserData;
}NPOOLXNODELIST;

typedef struct  
{
	NXNODELIST	*pEnableXNodeList;	// The Unique Enable List where the taglinked node are stored.
	NARRAY		PoolXNodeListArray;	// ... of Several NPOOLXNODELIST ! maximum 1024 (index from 0 to 1023, with poollist 0 as enable poollist)
	NNODELIST	TagList;

	NNODE		*pFirstInactiveTag;
	Nf32		DistanceActivation;
	Nf32		DistanceInactivation;
}NPOOL;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
void					NPool_PostUpdateHandle(const NTIME *ptime, Nu32 dataA,Nu32 dataB);

void					NClearPool(NPOOL *ppool);
NPOOL*					NSetUpPool(NPOOL *ppool,const NPOOLDESC *pdesc);
NPOOL*					NCreatePool(const NPOOLDESC *pdesc);
void					NDeletePool(NPOOL *ppool);

void					NDeleteAllPoolTags(NPOOL *ppool);

void					NDisablePoolTag(NPOOL *ppool, NPOOLTAG* ptag);
void					NDisableAllPoolTag(NPOOL *ppool);
void					NUpdatePool(NPOOL *ppool);

NPOOLXNODELIST*			NPoolBindXNodeList(NPOOL *ppool, NXNODELIST *pxlist,const NPOOL_XNODEENABLE_CALLBACK enable_callback, const NPOOL_XNODEENABLE_CALLBACK disable_callback,const Nf32 detectionradius, const Nu32 userdata);
Nbool					NPoolUnbindXNodeList(NPOOL *ppool, NXNODELIST *plist);
inline Nu32				NGetPoolXNodeListNumber(const NPOOL *ppool){return NGetArraySize(&ppool->PoolXNodeListArray);}

NPOOLTAG*				NCreatePoolTag(NPOOL *ppool, const Nu8 type, const NVEC3 *ppos);
void					NDeletePoolTag(NPOOL *ppool, NPOOLTAG *ptag);
inline NXNODE*			NGetPoolTagLinkedXNode(const NPOOLTAG* ptag){return ptag->pLinkedXNode;}
inline	Nu8				NGetPoolTagType(const NPOOLTAG *ptag){return ptag->Type;}
inline	Ns16			NGetPoolTagTypeMaxValue(const NPOOL *ppool){return (Ns16)(NGetArraySize(&ppool->PoolXNodeListArray) - 1 );}
inline	Nu8				NGetPoolTagUser8(const NPOOLTAG *ptag){return ptag->User_8;}
inline	Nu16			NGetPoolTagUser16(const NPOOLTAG *ptag){return ptag->User_16;}
inline	Nu32			NGetPoolTagUser32A(const NPOOLTAG *ptag){return ptag->A_User_32;}
inline	Nu32			NGetPoolTagUser32B(const NPOOLTAG *ptag){return ptag->B_User_32;}

inline	void			NSetPooltagPosition(NPOOLTAG *ptag,const NVEC3 *ppos){ptag->Pos = *ppos;}
Ns16					NSetPoolTagType(NPOOL *ppool, NPOOLTAG *ptag, const Nu8 type);
inline void				NSetPoolTagUser8(NPOOLTAG *ptag,const Nu8 u8){ptag->User_8 = u8;}
inline void				NSetPoolTagUser16(NPOOLTAG *ptag,const Nu16 u16){ptag->User_16 = u16;}
inline void				NSetPoolTagUser32A(NPOOLTAG *ptag,const Nu32 u32A){ptag->A_User_32 = u32A;}
inline void				NSetPoolTagUser32B(NPOOLTAG *ptag,const Nu32 u32B){ptag->B_User_32 = u32B;}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NPOOL_H 

