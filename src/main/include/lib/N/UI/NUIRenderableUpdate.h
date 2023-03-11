#ifndef __NUI_RENDERABLEUPDATE_H
#define __NUI_RENDERABLEUPDATE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NUIRenderableUpdate.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/Render/Renderable/NRenderable.h"
#include "lib/N/Core/NTime.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Showed UI
    void NRenderableUpdate_UI(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);
    void NRenderableUpdateWithoutExtract_UI(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);

    // Hidden UI
    void NRenderableUpdate_UI_Hidden(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);
    void NRenderableUpdateWithoutExtract_UI_Hidden(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUI_RENDERABLEUPDATE_H
