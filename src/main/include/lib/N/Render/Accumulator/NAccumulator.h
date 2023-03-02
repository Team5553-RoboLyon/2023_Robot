#ifndef __NACCUMULALTOR_H
#define __NACCUMULALTOR_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NAccumulator.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../GL/Ngl.h"
#include "../../NFlags.h"
#include "../../Containers/NArray.h"
#include "../../NTexture.h"
#include "NStateSet.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************
#define DEFAULT_NACCUMULATOR_CAPACITY		64

typedef enum
{
	NACCUM_FLAT = 0,				// Flat geometries
	// --------------------------------
	//NACCUM_FLAT_LIGHT,			// Doesn't exist for optimization... Flat and lighted geometries
	//NACCUM_FLAT_BLEND_LIGHT,		// Doesn't exist for optimization... Flat, blended and lighted geometries
	//NACCUM_FLAT_BLEND,			// Doesn't exist for optimization... Flat and blended geometries
	// --------------------------------
	NACCUM_TEXTURES,				// Textured geometries (without light and blending).
	NACCUM_TEXTURES_LIGHT,			// Textured and lighted geometries
	NACCUM_TEXTURES_BLEND_LIGHT,	// Textured, blended and lighted geometries
	NACCUM_TEXTURES_BLEND,			// Textured and blended geometries
	// --------------------------------
	NACCUM_HUD,						// Special Accumulator for HUD rendering only 	
	// --------------------------------
	NACCUM_ID_ENUM_SIZE				// !!! Have to be <= NU8_MAX !!!
}NACCUM_ID_ENUM;	

typedef struct NACCUMULATOR		NACCUMULATOR;
typedef void (*ACCUMULATOR_RENDER_FCT)(NACCUMULATOR *pacc);
struct NACCUMULATOR 
{
	ACCUMULATOR_RENDER_FCT	RenderAccumulator;
	NARRAY					AccumulatedRenderCellsArray;
	NSTATESET				StateSet;
};

// ***************************************************************************************
// **								 Privates Globales									**
// ***************************************************************************************
extern NACCUMULATOR				NAccumulator[NACCUM_ID_ENUM_SIZE];

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NACCUM_ID_ENUM		NGetAccumulatorID(const NSTATESET stateset_flags);
inline Nu32			NGetAccumulatorSize(const NACCUMULATOR* pacc){return pacc->AccumulatedRenderCellsArray.Size;}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + LOW LEVEL & PRIVATE functions : Setup & Clear / N_Init & N_Disable
NACCUMULATOR*		NSetupAccumulator(NACCUMULATOR *pacc, const Nu32 capacity,const ACCUMULATOR_RENDER_FCT	render_function, NSTATESET stateset );
void				NClearAccumulator(void *pacc);

void				NInit_Accumulators();
void				NDisable_Accumulators();
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + LOW LEVEL & PRIVATE functions : Accumulator Rendering

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NACCUMULALTOR_H 

