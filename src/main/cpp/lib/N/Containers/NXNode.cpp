// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NXNode.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "NXNode.h"

NXNODELIST *NSetUpXNodeList(NXNODELIST *plist)
{
	plist->pX = NULL; // ! Never used !
	plist->pFirst = (NXNODE *)plist;
	plist->pLast = (NXNODE *)plist;

	return plist;
}

NXNODELIST *NClearXNodeList(NXNODELIST *pxnlist, NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor)
{
	NXNODE *pxn, *pxnb;

	pxn = NGetFirstXNode(pxnlist);
	while (pxn != (NXNODE *)pxnlist)
	{
		pxnb = pxn;
		pxn = pxn->pNext;
		NDeleteXNode(pxnb, nxn_X_destructor);
	}

	pxnlist->pX = NULL;
	pxnlist->pFirst = NULL;
	pxnlist->pLast = NULL;

	return pxnlist;
}

NXNODELIST *NCreateXNodeList(void *px)
{
	return NSetUpXNodeList(NEW(NXNODELIST));
}

void NDeleteXNodeList(NXNODELIST *pxnlist, NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor)
{
	Nfree(NClearXNodeList(pxnlist, nxn_X_destructor));
}

NXNODE *NSetUpXNode(NXNODE *pxn, void *px)
{
	pxn->pX = px;
	pxn->pPrevious = NULL;
	pxn->pNext = NULL;
	return pxn;
}

NXNODE *NClearXNode(NXNODE *pxn, NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor)
{
	// ! Clear an NXNode means:
	// "FIRST OF ALL Destroy ->pX data linked with it !" but only, of course, if there is an available destructor !
	if (nxn_X_destructor)
		nxn_X_destructor(pxn->pX);

	// Remove Node from its mother list before clearing !
	if (!NXNodeIsOrphan(pxn))
		NXNodeRemove(pxn);

	memset(pxn, 0, sizeof(NXNODE));
	return pxn;
}

NXNODE *NCreateXNode(void *px)
{
	/*
		if(bXFactory && XVALID( XFIRST(&NASleepXNodeList),&NASleepXNodeList) )
			return NSetUpXNode( NXNodeRemove( XFIRST(&NASleepXNodeList) ),px );
		else
	*/
	return NSetUpXNode(NEW(NXNODE), px);
}

void NDeleteXNode(NXNODE *pxn, NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor)
{
	/*
		NClearXNode(pxn,nxn_X_destructor);
		if(bXFactory)
			NXNodeInsertEnd(pxn,&NASleepXNodeList);
		else
	*/
	NClearXNode(pxn, nxn_X_destructor);
	Nfree(pxn);
}
