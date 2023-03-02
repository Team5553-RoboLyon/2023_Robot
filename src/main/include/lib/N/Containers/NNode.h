#ifndef __NNODE_H
#define __NNODE_H

#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NNode.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
// **						Structures and 'NODE define'								**
// ***************************************************************************************
// to put on the top of all the structure definition of a "simple node" element
#define NNODEDEF \
	void *pNext; \
	void *pPrevious;

	// a Doubly-Linked List (Head)
	typedef struct
	{
		void *pFirst;
		void *pLast;
	} NNODELIST;

	// a  "Simple Node" of a Doubly-Linked List
	typedef struct NNODE NNODE;
	struct NNODE
	{
		// NNODEDEF
		void *pNext;
		void *pPrevious;
	};

// NNODE
#define NGET_FIRST_NODE(l) (l)->pFirst	 // (l) A valid pointer on a NodeList
#define NGET_LAST_NODE(l) (l)->pLast	 // (l) A valid pointer on a NodeList
#define NGET_NEXT_NODE(n) (n)->pNext	 // (n) A valid pointer on a Node
#define NGET_PREV_NODE(n) (n)->pPrevious // (n) A valid pointer on a Node
#define NSETUP_NODE(n) \
	(n)->pNext = NULL; \
	(n)->pPrevious = NULL;

#define NIS_NODELIST_EMPTY(plist) ((plist)->pFirst == (void *)(plist))
#define NIS_NODE_VALID(pn, plist) ((NNODE *)(pn) != (NNODE *)(plist)) // (pn) A valid node has to be different than (plist) the list header.

// inline Nbool NIsNodeListEmpty( NNODELIST *plist){ return ( plist->pFirst != (void*)plist);}
//  NNODELIST
#define NSETUP_NODELIST(l) \
	(l)->pFirst = l;       \
	(l)->pLast = l;

#define NCLEAR_NODELIST(l)                                                                             \
	NErrorIf(((l)->pFirst != (void *)(l)) || ((l)->pLast != (void *)(l)), NERROR_NNODELIST_NOT_EMPTY); \
	(l)->pFirst = NULL;                                                                                \
	(l)->pLast = NULL;

	// a Callback function prototype used for operation on node ( destruction, ... )
	typedef void (*NNODE_CALLBACK)(NNODE *pnode);

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	inline NNODE *NGetFirstNode(const NNODELIST *plist) { return (NNODE *)NGET_FIRST_NODE(plist); }
	inline NNODE *NGetLastNode(const NNODELIST *plist) { return (NNODE *)NGET_LAST_NODE(plist); }

	/*
	inline NNODE*	NSetupNode(NNODE* pnode)
	{
		pnode->pNext		= NULL;
		pnode->pPrevious	= NULL;
		return pnode;
	}

	*/
	// inline NNODE*	NCreateNode(NNODE* pnode)...Doesn't exist. Because a simple 'NNODE' structure is a part of something else ...
	//												It doesn't make sens having it alone ...
	//												Good Practice is for a structure which has NNODE members into...
	//												NSOMETHING* NSetupSomethingElse(NSOMETHING *ptr, ... )
	//												{
	//													NSetupNode((NNODE*)ptr);
	//													...
	//													return ptr;
	//												}

	/*
	inline void NClearNode(NNODE *pnode, NNODE_CALLBACK destructor_callback)
	{
		NNodeRemove(pnode);
		if(destructor_callback)
			destructor_callback(pnode);
	}

	inline void NDeleteNode(NNODE *pnode, NNODE_CALLBACK destructor_callback)
	{
		NClearNode(pnode,destructor_callback);
		Nfree(pnode);
	}
	*/

	inline NNODELIST *NSetupNodeList(NNODELIST *plist)
	{
		plist->pFirst = plist;
		plist->pLast = plist;

		return plist;
	}

	/*
	inline	NNODELIST*	NCreateNodeList()
	{
		return NSetupNodeList(NEW(NNODELIST));
	}

	inline	void		NClearNodeList(NNODELIST* plist, NNODE_CALLBACK node_destructor_callback )
	{
		NNODE *pb;
		NNODE *pa =(NNODE*)plist->pFirst;

		while( NIS_NODE_VALID(pa,plist) )
		{
			pb = pa;
			pa = (NNODE*)pa->pNext;
			NDeleteNode(pb,node_destructor_callback);
		}
	}

	inline	void		NDeleteNodeList(NNODELIST* plist, NNODE_CALLBACK node_destructor_callback )
	{
		NClearNodeList(plist,node_destructor_callback);
		Nfree(plist);
	}*/

	/*

	#define NNodeInsertEnd(type,pnew,plist)						\
		pnew->pNext	= ( (type*)( plist->pLast ) )->pNext;		\
		pnew->pPrevious	= plist->pLast;							\
		((type*)(plist->pLast))->pNext = pnew;					\
		plist->pLast = pnew;

	*/

	inline void NNodeInsertEnd(NNODE *pnew, NNODELIST *plist)
	{
		// from pnew
		NErrorIf(((NNODE *)(plist->pLast))->pNext != (NNODE *)plist, NERROR_SYSTEM_CHECK);
		pnew->pNext = ((NNODE *)(plist->pLast))->pNext;
		pnew->pPrevious = plist->pLast;
		// To pnew
		((NNODE *)(plist->pLast))->pNext = pnew;
		plist->pLast = pnew;
	}

	inline void NNodeInsertBeginning(NNODE *pnew, NNODELIST *plist)
	{
		// from pnew
		pnew->pNext = plist->pFirst;
		pnew->pPrevious = plist;
		// To pnew
		((NNODE *)(plist->pFirst))->pPrevious = pnew;
		plist->pFirst = pnew;
	}

	inline void NNodeInsertAfter(NNODE *pnew, NNODE *pref)
	{
		// from pnew
		pnew->pNext = pref->pNext;
		pnew->pPrevious = pref;
		// To pnew
		((NNODE *)(pref->pNext))->pPrevious = pnew;
		pref->pNext = pnew;
	}

	inline void NNodeInsertBefore(NNODE *pnew, NNODE *pref)
	{
		// from pnew
		pnew->pNext = pref;
		pnew->pPrevious = pref->pPrevious;
		// To pnew
		((NNODE *)(pref->pPrevious))->pNext = pnew;
		pref->pPrevious = pnew;
	}

	inline void NNodeRemove(NNODE *pn)
	{
		((NNODE *)(pn->pPrevious))->pNext = pn->pNext;
		((NNODE *)(pn->pNext))->pPrevious = pn->pPrevious;
		pn->pNext = NULL;
		pn->pPrevious = NULL;
	}

	inline Nbool NNodeIsValid(const NNODE *pn, const NNODELIST *plist)
	{
		if (pn != (const NNODE *)plist)
			return NTRUE;
		else
			return NFALSE;
	}

	inline Nbool NNodeIsOrphan(const NNODE *pn)
	{
		if (!pn->pNext || !pn->pPrevious)
			return NTRUE;
		else
			return NFALSE;
	}

	// Advanced functions
	inline Nu32 NGetNodeListCount(const NNODELIST *plist)
	{
		NNODE *pa = (NNODE *)plist->pFirst;
		Nu32 nb = 0;

		while (NIS_NODE_VALID(pa, plist))
		{
			nb++;
			pa = (NNODE *)pa->pNext;
		}
		return nb;
	}

	inline Nu32 NNodeGetIndex(const NNODELIST *plist, const NNODE *phn)
	{
		Nu32 id = 0;
		NNODE *pa = (NNODE *)plist->pFirst;

		while (NNodeIsValid(pa, plist))
		{
			if (pa == phn)
			{
				return id;
			}
			else
			{
				id++;
			}
			pa = (NNODE *)pa->pNext;
		}
		NErrorIf(1, NERROR_NNODE_INVALID_NODE);
		return NU32_MAX;
	}

	inline NNODE *NNodeGetIndexedNode(const NNODELIST *plist, const Nu32 id)
	{
		Nu32 i;
		NNODE *pa = (NNODE *)plist->pFirst;

		NErrorIf(id >= NGetNodeListCount(plist), NERROR_NNODE_INVALID_INDEX);
		for (i = id; i != 0; i--, pa = (NNODE *)pa->pNext)
		{
		}
		return pa;
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __N_H

// ***************************************************************************************
// **						Structures and 'SNODE define'								**
// ***************************************************************************************
/*
// to put on the top of all the structure definition of a "simple node" element
#define NSNODEDEF		\
	void	*pNext;		\

// a Doubly-Linked List (Head)
typedef struct
{
	void	*pFirst;
}NSNODELIST;

// a  "Simple Node" of a Simply-Linked List
typedef struct NSNODE NSNODE;
struct NSNODE
{
	//NSNODEDEF
	void	*pNext;
};

// ***************************************************************************************
// **						Structures and 'NNODEX define'								**
// ***************************************************************************************
TODO: NNODEX
#define NNODEXDEF		\
void		*pNext;		\
void		*pPrevious;	\
void		*pOwner;

special functions:
inline	void	NNodeXInsertEnd(NNODEX* pnew,NNODELIST* plist)
{
// from pnew
pnew->pNext	= ( (NNODE*)( plist->pLast ) )->pNext;
pnew->pPrevious	= plist->pLast;
// To pnew
((NNODE*)(plist->pLast))->pNext = pnew;
plist->pLast = pnew;
// Keep Owner
pnew->pOwner = (void*)plist;
}

inline	void	NNodeXInsertBeginning(NNODEX* pnew,NNODELIST* plist)
{
// from pnew
pnew->pNext = plist->pFirst;
pnew->pPrevious = plist;
// To pnew
((NNODE*)(plist->pFirst))->pPrevious = pnew;
plist->pFirst = pnew;
// Keep Owner
pnew->pOwner = (void*)plist;
}

inline	void	NNodeXInsertAfter(NNODEX* pnew,NNODEX* pref)
{
// from pnew
pnew->pNext = pref->pNext;
pnew->pPrevious = pref;
// To pnew
((NNODE*)(pref->pNext))->pPrevious = pnew;
pref->pNext = pnew;
// Keep Owner
pnew->pOwner = pref->pOwner;
}

inline	void	NNodeXInsertBefore(NNODEX* pnew,NNODEX* pref)
{
// from pnew
pnew->pNext = pref;
pnew->pPrevious = pref->pPrevious;
// To pnew
( (NNODE*)(pref->pPrevious) )->pNext = pnew;
pref->pPrevious = pnew;
// Keep Owner
pnew->pOwner = = pref->pOwner;
}

inline	void	NNodeXRemove(NNODEX* pn)
{
((NNODE*)(pn->pPrevious))->pNext = pn->pNext;
((NNODE*)(pn->pNext))->pPrevious = pn->pPrevious;
}
*/
