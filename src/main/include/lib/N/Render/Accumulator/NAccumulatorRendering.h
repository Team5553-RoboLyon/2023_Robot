#ifndef __NACCUMULALTOR_RENDERING_H
#define __NACCUMULALTOR_RENDERING_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NAccumulatorRendering.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NAccumulatorRendering.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + LOW LEVEL & PRIVATE functions : Accumulator Rendering
void NRenderAccumulator_NoBlendNoLight(NACCUMULATOR *pacc);
void NRenderAccumulator_BlendNoLight(NACCUMULATOR *pacc);
void NRenderAccumulator_NoBlendLight(NACCUMULATOR *pacc);
void NRenderAccumulator_BlendLight(NACCUMULATOR *pacc);
void NRenderAccumulator_HUD(NACCUMULATOR *pacc);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif //  __NACCUMULALTOR_RENDERING_H 

