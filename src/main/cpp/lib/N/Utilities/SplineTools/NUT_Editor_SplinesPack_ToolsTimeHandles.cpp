#ifdef _NEDITOR
// -----------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **		NUT_Editor_SplinesPack_ToolsTimeHandles.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/NEditor.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "../../Utilities/Draw/NUT_Draw.h"
#include "NUT_Editor_SplinesPack_Tools.h"

void NUT_SplinesPackTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	NSPLINE						*pspline;
	NUT_SPACK_TOOLDATA			*pdata		= (NUT_SPACK_TOOLDATA*)tool_param;
	NSPLINE						**ppspline	= (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);
	NCOLOR						color;

	for (Nu32 i = 0; i < pdata->ArrayOfSplinesPtr.Size; i++, ppspline++)
	{
		pspline = *ppspline;

		if (pspline == (NSPLINE*)pselection->pSelectedOwner)
		{
			color = pdata->DrawSpline_Param.Color;
			pdata->DrawSpline_Param.Color = pdata->SelectionColor;		// Current selected Spline Color
			NUT_Draw_Spline(pspline, &pdata->DrawSpline_Param);
			pdata->DrawSpline_Param.Color = color;
		}
		else
		{
			NUT_Draw_Spline(pspline, &pdata->DrawSpline_Param);
		}
	}
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR