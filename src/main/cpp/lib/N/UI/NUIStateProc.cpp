#include "../NCStandard.h"
#include "../NFastAnimatedValue.h"
#include "NUI.h"


void NUpdateUIState_Recursive(NUI *pui, const NTIME	*ptime)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);

	// Update UI State
	pui->UIState_Proc(pui,ptime);

	// Call update of all children
	NNODE *pnode = (NNODE*)pui->ChildrenList.pFirst;
	while( NIS_NODE_VALID(pnode,&pui->ChildrenList) )
	{
		NUpdateUIState_Recursive((NUI*)pnode,ptime);
		pnode=(NNODE*)pnode->pNext;
	}
}

void NDefaultUIEventToState_Handle(NUI *pui, const NEVENT *pevent)
{
	// Nothing !	
}

void NDefaultUIState_Handle(NUI *pui, const NTIME *ptime)
{
	// Nothing !	
}

