#ifndef __NRENDERABLE_UPDATE_H
#define __NRENDERABLE_UPDATE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NRenderable_Update.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NRenderable.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum
{
	NRENDERABLE_UPDATE_NULL	= 0,						// NULL pointer returned by the function "NGetRenderableUpdateFunctionID" if it doesn't find the function in the LUT.
	NRENDERABLE_UPDATE_VOID,							// Pointer to 'NRenderable_UpdateAndExtract_Void' returned. It's not NULL it's just a function that does nothing !

	NRENDERABLE_UPDATE_USER0,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER1,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER2,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER3,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER4,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER5,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER6,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER7,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER8,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER9,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER10,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER11,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER12,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER13,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER14,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER15,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER16,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER17,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER18,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER19,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER20,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER21,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER22,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER23,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER24,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER25,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER26,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER27,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER28,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER29,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER30,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)
	NRENDERABLE_UPDATE_USER31,							// User Function or "NRenderable_UpdateAndExtract_Void" (default value)

	NRENDERABLE_UPDATE_PARTICLESEMITTER,				// NRenderable_UpdateAndExtract_3D_ParticleEmitter
	NRENDERABLE_UPDATE_ADVANCED_HOMING_PARTICLESEMITTER,
	NRENDERABLE_UPDATE_ADVANCED_PARTICLESEMITTER,
	NRENDERABLE_UPDATE_HOMING_PARTICLESEMITTER,

	NRENDERABLE_UPDATE_PHYSIC_STRUCTURE,				// NRenderable_UpdateAndExtract_3D_PhysicStructure
	NRENDERABLE_UPDATE_TEXT,							// NRenderable_UpdateAndExtract_3D_Text
	NRENDERABLE_UPDATE_UI,								// NRenderable_UpdateAndExtract_3D_UI
	NRENDERABLE_UPDATE_UI_WITHOUT_EXTRACTION,			// NRenderable_UpdateWithOutExtract_UI
	//----------------------
	NRENDERABLE_UPDATE_FCT_ID_ENUM_SIZE
}NRENDERABLE_UPDATE_FCT_ID_ENUM;


void NRenderableUpdate_Basic(NRENDERABLE *prenderable, void* powner, const NTIME *ptime);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NRENDERABLE_UPDATE_H 

