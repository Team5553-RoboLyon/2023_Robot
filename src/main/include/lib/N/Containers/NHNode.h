#ifndef __NHNODE_H
#define __NHNODE_H

#include "lib/N/NType.h"
#include "NNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
TODO !!! ITS WORKING !!!
And replace all the Node functions by MACRO !
In that way it will be easy to create 2 version of a structure one without node inside and one with node inside if its necessary !!!
And it will continue to work ... YEP !! We will have only the creation /desctruction function to Re-build ! all the others will continue to
working because the First part of the structure is going to stay the same one
(PREVIOUS/NEXT are going to be at the bottom of the structure !!! )

#define DBL_LINKED_LIST_NODE(type)		type	*pLinkListNode_PREVIOUS;\
										type	*pLinkListNode_NEXT;

typedef struct NODETEST NODETEST;
struct NODETEST
{
	Nf32 data;
	LINKLISTNODE(NODETEST)  // At the End of the structure !!!
};
*/

// ***************************************************************************************
// **						 Structures and 'HIERARCHIC NODE define'					**
// ***************************************************************************************
// to put on the top of all the structure definition of a "hierarchic node" element
#define NHIERARCHY_NODEDEF \
	void *pNext;           \
	void *pPrevious;       \
	void *pParent;         \
	NNODELIST ChildrenList;

	// a "Hierarchic Node" of a Doubly-Linked List
	typedef struct NHIERARCHY_NODE NHIERARCHY_NODE;
	struct NHIERARCHY_NODE
	{
		// NHIERARCHY_NODEDEF
		void *pNext;
		void *pPrevious;
		void *pParent;
		NNODELIST ChildrenList;
	};

	// a Callback function prototype used for operation on hierarchic node ( destruction, ... )
	// typedef Nu8 (*NHNODE_CALLBACK)(NHIERARCHY_NODE*pnode);

#define NSETUP_HNODE(n) \
	NSETUP_NODE(n);     \
	NSETUP_NODELIST(&(n)->ChildrenList);

#define NHAS_HNODE_CHILDREN(n) (!(NIS_NODELIST_EMPTY(&(n)->ChildrenList)))

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************

	inline NHIERARCHY_NODE *NSetupHNode(NHIERARCHY_NODE *phn)
	{
		phn->pParent = NULL;
		phn->pPrevious = NULL;
		phn->pNext = NULL;
		NSetupNodeList(&phn->ChildrenList);
		/*
			NSETUP_NODE(phn);
			NSETUP_NODELIST(&phn->ChildrenList);
		*/
		return phn;
	}

	inline void NHNodeSetParent_EndInsert(NHIERARCHY_NODE *parent, NHIERARCHY_NODE *pn)
	{
		if (pn->pPrevious || pn->pNext) // For a HNode pNext and pPrevious are used into the Parent Children List...
		{
			NErrorIf(!pn->pParent, NERROR_NULL_POINTER); // Not completely true, so maybe it will necessary to remove this check ...
														 // for example, a root Hnode (without parent) may be inserted into a NNodeList as a node ....
														 // It could be , it could be , so ... if it happens ...
			NNodeRemove((NNODE *)pn);
		}

		if (parent)
			NNodeInsertEnd((NNODE *)pn, &parent->ChildrenList);

		pn->pParent = parent;
	}

	inline void NHNodeSetParent_BeginningInsert(NHIERARCHY_NODE *parent, NHIERARCHY_NODE *pn)
	{
		if (pn->pPrevious || pn->pNext) // For a HNode pNext and pPrevious are used into the Parent Children List...
		{
			NErrorIf(!pn->pParent, NERROR_NULL_POINTER);
			NNodeRemove((NNODE *)pn);
		}

		if (parent)
			NNodeInsertBeginning((NNODE *)pn, &parent->ChildrenList);

		pn->pParent = parent;
	}

	inline void NHNodeSetParent_NULL(NHIERARCHY_NODE *pn)
	{
		if (pn->pPrevious || pn->pNext) // For a HNode pNext and pPrevious are used into the Parent Children List...
		{
			NErrorIf(!pn->pParent, NERROR_NULL_POINTER);
			NNodeRemove((NNODE *)pn);
		}
		pn->pParent = NULL;
	}
	inline NHIERARCHY_NODE *NHNodeChildrenListInitialization(NHIERARCHY_NODE *phn)
	{
		//	NSetupNodeList(&phn->ChildrenList);
		NSETUP_NODELIST(&phn->ChildrenList);
		return phn;
	}

	inline NNODELIST *NGetHNodeChildrenList(NHIERARCHY_NODE *phn) { return (&phn->ChildrenList); }

	inline Nu32 NGetHNodeChildrenCount(const NHIERARCHY_NODE *phn)
	{
		NHIERARCHY_NODE *pa = (NHIERARCHY_NODE *)phn->ChildrenList.pFirst;
		Nu32 nb = 0;

		while (NNodeIsValid((NNODE *)pa, &phn->ChildrenList))
		{
			nb++;
			pa = (NHIERARCHY_NODE *)pa->pNext;
		}
		return nb;
	}

	inline Nu32 NGetHNodeDescendantCount(const NHIERARCHY_NODE *phn)
	{
		NHIERARCHY_NODE *pa = (NHIERARCHY_NODE *)phn->ChildrenList.pFirst;
		Nu32 nb = 0;

		while (NNodeIsValid((NNODE *)pa, &phn->ChildrenList))
		{
			nb += NGetHNodeDescendantCount(pa) + 1;
			pa = (NHIERARCHY_NODE *)pa->pNext;
		}
		return nb;
	}

	inline Nu32 NGetHNodeDeep(const NHIERARCHY_NODE *phn)
	{
		NHIERARCHY_NODE *pp = (NHIERARCHY_NODE *)phn->pParent;
		if (!pp)
		{
			return 0;
		} // An Orphan (like a RootNode) has a Deep of 0 Means "No Parent "
		Nu32 deep = 0;

		while (pp != NULL)
		{
			deep++;
			pp = (NHIERARCHY_NODE *)pp->pParent;
		}
		return deep;
	}

/*
NHIERARCHY_NODE*	NCreateHNode(NHIERARCHY_NODE *parent, long size,NHNODE_CALLBACK createcallback,Nbool insert_end);
void				NDeleteHNode(NHIERARCHY_NODE*pnode,NHNODE_CALLBACK delcallback);
*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NHNODE_H
