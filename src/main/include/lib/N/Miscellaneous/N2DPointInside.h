#ifndef __N2DPOINTINSIDE_H
#define __N2DPOINTINSIDE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								N2DPointInside.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../NRectf32.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    Nbool N2DPointInside2DTriangle(const NVEC2 *pA, const NVEC2 *pB, const NVEC2 *pC, const NVEC2 *p);
    Nbool N2DPointInside2DRectangle(const NRECTf32 *prect, const NVEC2 *p);
    Nbool N2DPointInside2DRectangleEx(const NVEC2 *pcenter, const NVEC2 *pextent, const NVEC2 *p);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __N2DPOINTINSIDE_H
