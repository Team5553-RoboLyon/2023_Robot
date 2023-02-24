// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NPool.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "lib/N/NCStandard.h"
#include "../Containers/NArray.h"
#include "../NPool.h"
#include "../NCamera.h"
#include "lib/N/NErrorHandling.h"

#include "../NCore.h"
// extern	Nbool			bRendering;
//  ***************************************************************************************
//  ***************************************************************************************
//  **																					**
//  **									FUNCTIONS										**
//  **																					**
//  ***************************************************************************************
//  ***************************************************************************************

// POST UPDATE PROCESS
// =======================================================================================
/*
There are 2 ways to manage Pool updating.
You can perform update by your own or use the PostUpdate process manager.
	1)To perform it by yourself: use when you want the 'NPoolUpdate' function.

	2)To use the postupdate process mechanism, register the Pool Post-Update by
	  a call to 'NCreatePostUpdateProcess_Pool' and delete it by a call
	  of 'NDeletePostUpdateProcess_Pool'.
*/

// POOL CORE
// =======================================================================================

// ------------------------------------------------------------------------------------------
// NCreatePool
// ------------------------------------------------------------------------------------------
// Description :
//			Create a Pool of ... something.
//
//			A Pool is made with several lists:
//						A list of 'enable Nodes'.
//						Several list of 'disable Nodes'.
//						A list of simple 'PoolTags'.
//
//			Each 'engine turn' the pool is updated to determine which Tag is Active and which Tag is not.
//			Nodes are spread on the Active Tags.
// ------------------------------------------------------------------------------------------
// In  :
//		NNODELIST		*ppoollist	a pointer on a valid Nodelist to store the pool into.
//									it can be NULL.
//
//		NPOOLDESC		*pdesc		a pointer on NPOOLDESC structure which contains all the things
//									necessary to build in the Pool.
//									see bellow some details about parameters of 'NPOOLDESC'
// Out :
//		NPOOL*						A valid pointer on the new POOL.
//									or NULL, if a problem rise during creation process.
// ------------------------------------------------------------------------------------------
// Understand 'Flags' and 'Parameters' for Pool Creation:
/*
	The main pool purpose is managing huge number of complex objects with 'CONTROL and OPTIMIZATION'.
	Saving time and memory...
	Imagine you have to manage an entire Forest, with thousands of animated trees, animated grass, vegetation,etc ...
	You can create thousands of 3D trees objects and animate each of them, but you are going to have serious problems
	with Memory and UI rate.
	The Pool system uses an another way:
		+ First, it limits the maximum 'visible at the same time' complex objects to a fix number.
		  This number of complex objects is created at the beginning and stay the same.
		+ Second, instead of having thousands of complex objects a pool have thousands of 'Pooltags', smaller pieces
		  that consume a smaller amount of memory ( just a NVEC3 position and some other things ).
		+ The process is simple, each 'engine turn' the pool determine which tag is Active and which tag is not.
		  After this first process, the pool spread the complex objects on the valid tags.

	It's much more than "just" a "graphical frustrum Optimization" which exclude some geometries because they are
	not "on screen".

	1)
	Nf32 DistanceActivation		AND		Nf32 DistanceInactivation
	--------------------------------------------------------------
	To determine if a Tag is Active or not the program needs 2 distances.
			'DistanceActivation', usually the shortest. If the distance between the 'REF' and the tag is less than this one,
			 then Tag is set to active.
			'DistanceInactivation', usually the biggest. If the distance between the 'REF' and the tag is bigger than this one,
			 then Tag is set to inactive.
			 If the distance between the 'REF' and the tag is in 'between', nothing changes.
	2)
	NNODELIST	*pEnableNodeList
	----------------------------

*/

NPOOL *NSetUpPool(NPOOL *ppool, const NPOOLDESC *pdesc)
{
	NErrorIf(!ppool || !pdesc, NERROR_NULL_POINTER);
	NErrorIf(!pdesc->pEnableXNodeList, NERROR_POOL_ENABLE_XNODELIST_MISSING);
	memset(ppool, 0, sizeof(NPOOL));

	// Activation Distances
	ppool->DistanceInactivation = pdesc->DistanceInactivation;
	ppool->DistanceActivation = pdesc->DistanceActivation;

	// Tags List
	// NSetupNodeList(&ppool->TagList);
	NSETUP_NODELIST(&ppool->TagList);
	ppool->pFirstInactiveTag = (NNODE *)ppool->TagList.pFirst;

	// ... and PoolLists (XNodelist in fact, but with 2 functions associated with)
	NSetupArray(&ppool->PoolXNodeListArray, NPOOLXNODELIST_ARRAYCAPACITY, sizeof(NPOOLXNODELIST));

	// ... And the Enable List !
	ppool->pEnableXNodeList = pdesc->pEnableXNodeList;

	return ppool;
}
NPOOL *NCreatePool(const NPOOLDESC *pdesc)
{
	return NSetUpPool(NEW(NPOOL), pdesc);
}

void NClearPool(void *ppool)
{
	NNODE *pnode, *pnodeb;

	// if( bRendering )
	if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_RENDERING))
	{
		NErrorIf(1, NERROR_POOL_UNAUTHORIZED_CLEAR_REQUEST);
		return; // Because Rendering is in Progress...
	}			// It's better to avoid any suppressions.

	// FIRST: Delete All Pooltags
	// By doing this we also disable all the enable node.
	// and, all of them will be back to their original list.
	pnode = (NNODE *)((NPOOL *)ppool)->TagList.pFirst;
	while (pnode != (NNODE *)&((NPOOL *)ppool)->TagList)
	{
		pnodeb = pnode;
		pnode = (NNODE *)pnode->pNext;

		NDeletePoolTag((NPOOL *)ppool, (NPOOLTAG *)pnodeb);
	}

	// ... AND CLEAR all the NodelistArray
	NClearArray(&((NPOOL *)ppool)->PoolXNodeListArray, NULL);
}
// ------------------------------------------------------------------------------------------
// NDeletePool
// ------------------------------------------------------------------------------------------
// Description :
//			Create a Pool of ... something.
// ------------------------------------------------------------------------------------------
// In  :
//		NPOOL		*ppool			a valid pointer on the pool to delete.
// Out :
//		NNODE*						NVOID if not ( deleting was not performed ).
//									a valid pointer on the original node linked with the pool
// ------------------------------------------------------------------------------------------
void NDeletePool(NPOOL *ppool)
{
	NClearPool(ppool);
	Nfree(ppool);
}

static inline void _EnableTagLinkedXNode(NXNODELIST *penablexnodelist, NPOOLXNODELIST *ppoollist, NPOOLTAG *ptag)
{
	NXNodeRemove(ptag->pLinkedXNode); // From its "ppoollist->pList"
	NXNodeInsertEnd(ptag->pLinkedXNode, penablexnodelist);
	ppoollist->_XNodeEnable_CallBack(ptag, ppoollist->UserData);
}

static inline void _DisableTagLinkedXNode(NPOOLXNODELIST *ppoolxlist, NPOOLTAG *ptag)
{
	NXNodeRemove(ptag->pLinkedXNode); // From the main "penablenodelist"
	NXNodeInsertEnd(ptag->pLinkedXNode, ppoolxlist->pXNodeList);
	ppoolxlist->_XNodeDisable_CallBack(ptag, ppoolxlist->UserData);
}

// ------------------------------------------------------------------------------------------
// NUpdatePool
// ------------------------------------------------------------------------------------------
// Description :
//			...
// ------------------------------------------------------------------------------------------
// In  :
//		NPOOL		*ppool			a valid pointer on the pool to delete.
//
// Out :
//
// ------------------------------------------------------------------------------------------
static inline void _UpdatePool(NPOOL *ppool)
{
	NPOOLTAG *ptag;
	NPOOLTAG *ptagb;
	NNODE *plastactive;
	NXNODE *pxnode;
	NCAMERA *pcamera; // TODO : point directly to the NVEC3 used by the pool.
	Nf32 fdist;
	NPOOLXNODELIST *ppoolx;
	//	NVEC3	vdist;

	plastactive = (NNODE *)&ppool->TagList;
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	ppoolx = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);
	pcamera = NGetCamera();

	while ((NNODE *)ptag != (NNODE *)&ppool->TagList)
	{
		// Store the next PoolTag right now.
		ptagb = (NPOOLTAG *)ptag->pNext;

		// Check for the PoolTag activation !
		// ..............................................................
		// Usually we used the distance between target camera And pooltag
		// BUT ...
		// For this specific game (Snakes) we just need to have a X dstance comparison !
		// So let's do that !

		// a) usual distance comparison ...
		// 		NVec3Sub(&vdist,&ptag->Pos,&pcamera->TargetPos);
		// 		fdist = NABS( NFastSqrt(vdist.x*vdist.x + vdist.y*vdist.y + vdist.z*vdist.z) - ppoolx[ptag->Type].DetectionRadius );

		// b) Snake Specific method
		fdist = NABS(NABS(ptag->Pos.x - pcamera->TargetPos.x) - ppoolx[ptag->Type].DetectionRadius);
		// ..............................................................
		if (fdist > ppool->DistanceInactivation)
		{
			// Node Disable !
			if (ptag->pLinkedXNode)
			{
				_DisableTagLinkedXNode(&ppoolx[ptag->Type], ptag);
				ptag->pLinkedXNode = NULL;
			}
		}
		else if (fdist < ppool->DistanceActivation)
		{
			// Node Enable !
			if (!ptag->pLinkedXNode)
			{
				// pxnode = XFIRST(ppoolx[ptag->Type].pXNodeList);
				// if(XVALID(pxnode,ppoolx[ptag->Type].pXNodeList))
				pxnode = (NXNODE *)ppoolx[ptag->Type].pXNodeList->pFirst;
				if (pxnode != (NXNODE *)ppoolx[ptag->Type].pXNodeList)
				{
					ptag->pLinkedXNode = pxnode;
					_EnableTagLinkedXNode(ppool->pEnableXNodeList, &ppoolx[ptag->Type], ptag);
				}
				// TODO: Maybe 'TagLink_CallBack' and '_EnableNode' can be merge into a single and unique function
				//	So, in that way we are going to remove one call back (TagLink_CallBack)
				/*
								if( ptag->pLinkedNode )
									ppoolx[ptag->Type]._TagLink_CallBack(ptag,ppoolx[ptag->Type].UserData); // Can be use to perform some special Init (like reset structure skin )
				*/
			}
			// TODO: Maybe the lines below are just a waste of time. Check ...

			if (ptag->pLinkedXNode)
			{
				// An Active Pooltag ( ... == with a linked node) Rise on the top of the list !
				// ... but we try to avoid any unnecessary Node Moves
				// ... and the trick to avoid as much as possible unnecessary node moves
				// is to remember and use the last active tag checked during this loop.
				// So, if the Current Tag (ptag), which is active for sure, is not the next node on the right of the lastactive
				// that means that there is one or more inactive node between the lastactive tag and ptag
				// so ptag is remove from its place and relink just at the right of lastactive. And ptag becomes lasactive...
				// do you copy ?
				if ((NNODE *)ptag != plastactive->pNext)
				{
					NNodeRemove((NNODE *)ptag);
					NNodeInsertAfter((NNODE *)ptag, plastactive);
				}
				plastactive = (NNODE *)ptag;
			}
		}

		// Move to the next PoolTag, already read ...
		ptag = ptagb;
	}
	ppool->pFirstInactiveTag = (NNODE *)plastactive->pNext; //	Easy to understand ...
															//	Just on the right of the last Active Tag we have ... the first inactive one !
}

void NUpdatePool(NPOOL *ppool)
{
	_UpdatePool(ppool);
}

void NPool_PostUpdateHandle(const NTIME *ptime, Nu32 dataA, Nu32 dataB)
{
	_UpdatePool((NPOOL *)dataA);
}

NPOOLXNODELIST *NPoolBindXNodeList(NPOOL *ppool, NXNODELIST *pxlist, const NPOOL_XNODEENABLE_CALLBACK enable_callback, const NPOOL_XNODEENABLE_CALLBACK disable_callback, const Nf32 detectionradius, const Nu32 userdata)
{
	NPOOLXNODELIST pnodelist;

	pnodelist.pXNodeList = pxlist;
	pnodelist._XNodeDisable_CallBack = disable_callback;
	pnodelist._XNodeEnable_CallBack = enable_callback;
	pnodelist.DetectionRadius = detectionradius;
	pnodelist.UserData = userdata;
	return (NPOOLXNODELIST *)NArrayPushBack(&ppool->PoolXNodeListArray, (NBYTE *)&pnodelist);
}

Nbool NPoolUnbindXNodeList(NPOOL *ppool, NXNODELIST *pxlist)
{
	Nu32 i, id;
	NPOOLXNODELIST *pnlist;
	Nbool bfind;
	NPOOLTAG *ptag;

	// first: check if the list is really binded
	bfind = NFALSE;
	pnlist = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);
	for (i = 0; i < NGetArraySize(&ppool->PoolXNodeListArray); i++)
	{
		if (pxlist == pnlist->pXNodeList)
		{
			bfind = NTRUE;
			id = i;
			break;
		}
		pnlist++;
	}

	if (!bfind)
		return NFALSE;

	// Now we are sure that plist is binded, and we know its 'id'
	// First of all we have to disable all node which belong to the future unbind list from the main "penablenodelist"
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while (ptag != (NPOOLTAG *)&ppool->TagList)
	{
		if (ptag->Type == id)
		{
			// Node Disable !
			if (ptag->pLinkedXNode)
			{
				_DisableTagLinkedXNode(pnlist, ptag);
				ptag->pLinkedXNode = NULL;
			}
		}

		ptag = (NPOOLTAG *)ptag->pNext;
	}

	// And, after recompute tag type, in fact all the "type" which represents a list on the right of "plist"
	// have to be changed due to the "plist" removal. (decrease by one, obviously)
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while (ptag != (NPOOLTAG *)&ppool->TagList)
	{
		if (ptag->Type > id)
			ptag->Type--;

		ptag = (NPOOLTAG *)ptag->pNext;
	}

	//... ok, do it
	NEraseArrayElement(&ppool->PoolXNodeListArray, id, NULL);
	return NTRUE;
}

NPOOLTAG *NCreatePoolTag(NPOOL *ppool, const Nu8 type, const NVEC3 *ppos)
{
	NPOOLTAG *ptag;

	/*
		ptag = (NPOOLTAG*)malloc(sizeof(NPOOLTAG));
		if(!ptag)
			return NULL;
	*/
	ptag = NEW(NPOOLTAG);
	memset(ptag, 0, sizeof(NPOOLTAG));

	ptag->Type = type;
	ptag->Pos = *ppos;

	NNodeInsertEnd((NNODE *)ptag, &ppool->TagList);
	return ptag;
}

void NDeletePoolTag(NPOOL *ppool, NPOOLTAG *ptag)
{
	NPOOLXNODELIST *pxnodelist;

	// An Enable node is linked with this TAG,... WE HAVE TO disable it !
	if (ptag->pLinkedXNode)
	{
		pxnodelist = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);
		_DisableTagLinkedXNode(&pxnodelist[ptag->Type], ptag);
	}

	NNodeRemove((NNODE *)ptag);
	Nfree(ptag);
}

Ns16 NSetPoolTagType(NPOOL *ppool, NPOOLTAG *ptag, const Nu8 type)
{
	NPOOLXNODELIST *pxnodelist;
	Ns16 old;

	// WARNING ! Into Pooltag structure, Type is coded with only 10 bits
	// so we prefer to check that here !
	if (type >= NGetArraySize(&ppool->PoolXNodeListArray))
		return -1;

	if (ptag->pLinkedXNode)
	{
		pxnodelist = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);
		_DisableTagLinkedXNode(&pxnodelist[ptag->Type], ptag);
		ptag->pLinkedXNode = NULL;
	}

	old = (Ns16)ptag->Type;
	ptag->Type = type;

	return old;
}

void NDisablePoolTag(NPOOL *ppool, NPOOLTAG *ptag)
{
	NPOOLXNODELIST *pxnodelist;

	if (ptag->pLinkedXNode)
	{
		pxnodelist = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);
		_DisableTagLinkedXNode(&pxnodelist[ptag->Type], ptag);
		ptag->pLinkedXNode = NULL;
	}
}

void NDisableAllPoolTag(NPOOL *ppool)
{
	NPOOLTAG *ptag;
	NPOOLXNODELIST *pxnodelist;

	pxnodelist = (NPOOLXNODELIST *)NGetFirstArrayPtr(&ppool->PoolXNodeListArray);

	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while (ptag != (NPOOLTAG *)&ppool->TagList)
	{
		if (ptag->pLinkedXNode)
		{
			_DisableTagLinkedXNode(&pxnodelist[ptag->Type], ptag);
			ptag->pLinkedXNode = NULL;
		}

		ptag = (NPOOLTAG *)ptag->pNext;
	}
}

// ------------------------------------------------------------------------------------------
// NDeleteAllPoolTags
// ------------------------------------------------------------------------------------------
// Description :
//			Delete All the Pooltags which are listed into a Pool.
//			Pooltags are deleted one by one with a call of the right "_NodeDisable_CallBack" function
//			and the linked node is re-inserted into is original owner bind list.
// ------------------------------------------------------------------------------------------
// In	:
//			ppool:	A valid pointer on a pool
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDeleteAllPoolTags(NPOOL *ppool)
{
	NPOOLTAG *ptag, *ptagb;

	// Delete all the Pool Tags
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while (ptag != (NPOOLTAG *)&ppool->TagList)
	{
		ptagb = ptag;
		ptag = (NPOOLTAG *)ptag->pNext;

		NDeletePoolTag(ppool, ptagb);
	}
}

// ------------------------------------------------------------------------------------------
// NClearPool
// ------------------------------------------------------------------------------------------
// Description :
//		Delete all pooltags AND erase all the bind list with the most efficient way.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NClearPool(NPOOL *ppool)
{
	NDeleteAllPoolTags(ppool);
	NEraseArray(&ppool->PoolXNodeListArray, NULL);
}

// PREVIOUS VERSIONS
//
/*
inline NNODE* _GetAvailableNode(NPOOL *ppool)
{
	NNODE		*pnode;

	pnode = ppool->pFirstAvailableNode;
	if( pnode != (NNODE*)&ppool->NodeList )
	{
		ppool->pFirstAvailableNode = pnode->pNext;
		return pnode;
	}
	else
		return NULL;
}

inline void _ReturnWastedNodeBack(NPOOL *ppool,NNODE *pnode)
{
	NNodeRemove(pnode,&ppool->NodeList);
	NNodeInsertEnd(pnode,&ppool->NodeList);
}*/

/*
inline NNODE* _GetAvailableNode(NPOOL *ppool,Nu32 type)
{
	NNODE		*pnode;
	NPOOLLIST	*poollist;

	poollist = (NPOOLLIST *)NGetFirstArrayPtr(&ppool->PoolListArray);

	pnode = poollist[type].pList->pFirst;
	if( pnode != (NNODE*)poollist[type].pList )
	{
		NNodeRemove(pnode,poollist[type].pList);
		if(poollist[type].ToChangeEnableStatus_NodeNumber)
		{
			poollist[type].ToChangeEnableStatus_NodeNumber --;
			NNodeInsertEnd(pnode,poollist[0].pList);
		}
		else
		{
			NNodeInsertBeginning(pnode,poollist[0].pList);
			poollist[0].ToChangeEnableStatus_NodeNumber ++;
		}

		return pnode;
	}
	else
		return NULL;
}*/

/*
inline void _ReturnWastedNodeBack(NPOOL *ppool,Nu32 type,NNODE *pnode)
{
	NNODE		*pnode;
	NPOOLLIST	*poollist;

	poollist = (NPOOLLIST *)NGetFirstArrayPtr(&ppool->PoolListArray);

	NNodeRemove(pnode,poollist[0].pList);
	NNodeInsertBeginning(pnode,poollist[type].pList);
	poollist[type].ToChangeEnableStatus_NodeNumber ++;
}
*/

/*
Nbool NCreatePoolNodeCopies(NPOOL *ppool, Nu32 nodenb)
{
	Nu32	i;
	NNODE	*pnode;

	for(i=0;i<nodenb;i++)
	{
		pnode = ppool->_CreateNodeCopy(ppool->pOriginalNode);
		if(!pnode)
			return NFALSE;
		ppool->_InitializeFromOriginal(ppool->pOriginalNode,pnode);
		NNodeInsertEnd(pnode,&ppool->NodeList);
	}
	return NTRUE;
}
*/