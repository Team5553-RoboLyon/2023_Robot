#ifndef __NXNODE_H
#define __NXNODE_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NXNode.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
// **						Structures and 'XNODE define'								**
// ***************************************************************************************
// to put on the top of all the structure definition of a "simple node" element
typedef struct NXNODE NXNODE;
#define NXNODEDEF		\
	void	*pX;		\
	NXNODE	*pNext;		\
	NXNODE	*pPrevious;	

// a  "Simple XNode" of a Doubly-Linked List
struct NXNODE
{
	void	*pX;
	NXNODE	*pNext;
	NXNODE	*pPrevious;
};

// a Doubly-Linked XList (Head)
typedef struct
{
	void	*pX;
	NXNODE	*pFirst;
	NXNODE	*pLast;
}NXNODELIST;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
#define			XVALID(nx,nxlist)		( (nx != (NXNODE*)nxlist)? NTRUE:NFALSE )	// A valid nxnode has to be different than its nxlist header.
#define			XFIRST(pxlist)			( (pxlist)->pFirst )
#define			XNEXT(pxnode)			( (pxnode)=(pxnode)->pNext )
#define			XPREV(pxnode)			( (pxnode)=(pxnode)->pPrevious )
#define			XX(pxn)					((pxn)->pX)

// a Callback function prototype used for operation on node ( destruction, ... )
//typedef void (*NXNODE_CALLBACK)(void *pX);
typedef void (*NNODE_X_DESTRUCTOR_CALLBACK)(void*px);	

// .......................................................................................
// NXNODELIST
NXNODELIST*			NSetUpXNodeList(NXNODELIST* plist);
NXNODELIST*			NClearXNodeList(NXNODELIST* pxnlist,NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor);
NXNODELIST*			NCreateXNodeList(void *px);
void				NDeleteXNodeList(NXNODELIST* pxnlist,NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor);

// .......................................................................................
// NXNODE
NXNODE*			NSetUpXNode(NXNODE *pxn,void *px);
NXNODE*			NClearXNode(NXNODE* pxn,NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor);
NXNODE*			NCreateXNode(void *px);
void			NDeleteXNode(NXNODE* pxn,NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor);

inline	NXNODE*	NGetFirstXNode(NXNODELIST* plist){return plist->pFirst;}
inline	NXNODE*	NGetLastXNode(NXNODELIST* plist){return plist->pLast;}

inline	NXNODE*	NXNodeInsertEnd(NXNODE* pnew,NXNODELIST* plist)
				{
					// from pnew
					pnew->pNext		= plist->pLast->pNext;
					pnew->pPrevious	= plist->pLast;
					// To pnew
					plist->pLast->pNext = pnew;
					plist->pLast		= pnew;

					return pnew;
				}

inline	NXNODE*	NXNodeInsertBeginning(NXNODE* pnew,NXNODELIST* plist)
				{					
					// from pnew
					pnew->pNext		= plist->pFirst;
					pnew->pPrevious = (NXNODE*)plist;
					// To pnew
					plist->pFirst->pPrevious = pnew;
					plist->pFirst			 = pnew;

					return pnew;
				}

inline	NXNODE*	NXNodeInsertAfter(NXNODE* pnew,NXNODE* pref)
				{					
					// from pnew
					pnew->pNext		= pref->pNext;
					pnew->pPrevious = pref;
					// To pnew
					pref->pNext->pPrevious	= pnew;
					pref->pNext				= pnew;

					return pnew;
				}

inline	NXNODE*	NXNodeInsertBefore(NXNODE* pnew,NXNODE* pref)
				{					
					// from pnew
					pnew->pNext		= pref;
					pnew->pPrevious = pref->pPrevious;
					// To pnew
					pref->pPrevious->pNext	= pnew;
					pref->pPrevious			= pnew;
					
					return pnew;
				}

inline	NXNODE*	NXNodeRemove(NXNODE* pn)
				{
					pn->pPrevious->pNext	= pn->pNext;
					pn->pNext->pPrevious	= pn->pPrevious;
					pn->pNext				= NULL;
					pn->pPrevious			= NULL;
					
					return pn;
				}

inline	Nbool	NXNodeIsValid(const NXNODE* pn,const NXNODELIST* plist)
{
	if(  pn != (NXNODE*)plist )
		return NTRUE;
	else
		return NFALSE;
}

inline void*	NGetXNodeX(const NXNODE* pxn){return pxn->pX;}
inline void*	NSetXNodeX(NXNODE* pxn,void *px){void *prev;prev = pxn->pX;pxn->pX = px;return prev;}
inline NXNODE*	NGetXNodeByX(const NXNODELIST *pxnlist,const void *ptr){NXNODE*pxn = XFIRST(pxnlist);while(XVALID(pxn,pxnlist)){if(XX(pxn)==ptr){return pxn;}XNEXT(pxn);}return NULL;}

inline	Nbool	NXNodeIsOrphan(const NXNODE* pn)
{
	if(  !pn->pNext || !pn->pPrevious)
		return NTRUE;
	else
		return NFALSE;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __N_H 

