#ifdef _NEDITOR
// -----------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **				NUT_Editor_SplinesPack_ToolsTimeHandles.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/NEditor.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "../../Utilities/Draw/NUT_Draw.h"
#include "NUT_Editor_NVec3f32Pack_Tools.h"

void NUT_NVec3f32PackTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	NUT_V3PACK_TOOLDATA			*pdata		= (NUT_V3PACK_TOOLDATA*)tool_param;
	NBYTE						*powner;
	NVEC3f32					*pv3;

	for (Nu32 i = 0; i < pdata->pArrayOfElements->Size; i++ )
	{
		powner = NGetArrayPtr(pdata->pArrayOfElements, i);
		pv3 = (NVEC3f32*)(powner + pdata->OffsetToV3);

		if (powner == (NBYTE*)pselection->pSelectedOwner)
		{
			NUT_Draw_Mark(pv3, &pdata->TickExtend, &pdata->SelectionColor);
		}
		else
		{
			NUT_Draw_Mark(pv3,&pdata->TickExtend,&pdata->TickColor);
		}
	}
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR