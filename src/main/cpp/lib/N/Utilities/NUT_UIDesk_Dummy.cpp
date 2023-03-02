#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"
#include "NUT_UIDesk_Dummy.h"

extern NUT_UIDESK_PANEL			UIDeskPanel;

NUT_UIDESK_DUMMY	NUT_UIDesk_LastCreatedUI_Dummy;

NUT_UIDESK_DUMMY* NUT_UIDesk_ExtractUIDummy(NUT_UIDESK_DUMMY *pdummy)
{
	*pdummy=NUT_UIDesk_LastCreatedUI_Dummy;
	return pdummy;
}
NUT_UIDESK_DUMMY* NUT_UIDesk_ExtractPanelDummy(NUT_UIDESK_DUMMY *pdummy)
{
	NErrorIf(ISFLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN),NERROR_NUT_UIDESK_PANEL_IS_CLOSE );
	pdummy->WorldPosition	= UIDeskPanel.Panel_WorldPosition;
	pdummy->Size			= UIDeskPanel.Panel_Size;
	return pdummy;
}

void NUT_UIDesk_StoreUIDummyData(const NVEC3 *pposition,const NVEC3 *psize)
{
	if(pposition)
		NUT_UIDesk_LastCreatedUI_Dummy.WorldPosition = *pposition;
	else
		NVec3Set(&NUT_UIDesk_LastCreatedUI_Dummy.WorldPosition,0,0,0);
	if(psize)
		NUT_UIDesk_LastCreatedUI_Dummy.Size = *psize;
	else
		NVec3Set(&NUT_UIDesk_LastCreatedUI_Dummy.Size,0,0,0);
}