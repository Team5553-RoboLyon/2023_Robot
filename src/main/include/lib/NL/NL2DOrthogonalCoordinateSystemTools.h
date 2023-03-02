#pragma once

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NL2DOrthogonalCoordinateSystemTools.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "lib/N/NType.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/NEditor.h"

#ifdef  _NEDITOR
NEDITOR_RESULT			NL2DOCS_FreeTransform(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
//NEDITOR_RESULT			NL2DOCS_Scale(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
void					NL2DOCS_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
#endif//_NEDITOR
