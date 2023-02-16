#pragma once

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NL2DOrthogonalCoordinateSystemTools.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "../N/NType.h"
#include "../N/Event/NEvent.h"
#include "../N/Core/NTime.h"
#include "../N/NEditor.h"

#ifdef  _NEDITOR
NEDITOR_RESULT			NL2DOCS_FreeTransform(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
//NEDITOR_RESULT			NL2DOCS_Scale(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
void					NL2DOCS_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
#endif//_NEDITOR
