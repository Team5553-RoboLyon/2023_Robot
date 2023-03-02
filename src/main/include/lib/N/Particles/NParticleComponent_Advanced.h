#ifndef __NPARTICLECOMPONENT_ADVANCED_H
#define __NPARTICLECOMPONENT_ADVANCED_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NParticleComponent_Advanced.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../NCCS_3DCoord.h"
#include "NParticleSetupHandles.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_ADVANCED_BIRTH_EXTENTS_2F 0.5f, 0.5f			  // |     |  O  |  	 |  O  |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_ADVANCED_BIRTH_EXTENTS_VARIATION_2F 0.5f, 0.5f // |     |  O  |  	 |  O  |     |     |

// Std	 : NPARTICLE
// Adv	 : NADVANCEDPARTICLE
// H.Std : NHOMINGPARTICLE
// H.Adv : NADVANCEDHOMINGPARTICLE
//
// + NADVANCEDPARTICLES_EMITTER_DESC_PARAMS FLAGS														// | Std | Adv |H.Std|H.Adv|     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_BIRTH_EXTENTS_AND_VARIATION BIT_0 // |     |  O  |   	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_SETUPEXTENTS_HANDLE BIT_1			// |     |  O  |   	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_UPDATE_CALLBACK BIT_2				// |     |  O  |  	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_DEATH_CALLBACK BIT_3				// |     |  O  |  	 |  O  |     |     |
	// ***************************************************************************************
	// **								 Structures											**
	// ***************************************************************************************
	typedef void (*NPARTICLECOMPONENT_ADVANCED_CALLBACK)(const void *pparticle, const void *pemitter); // The Component doesn't know it's particles emitter owner !!!

	typedef struct
	{
		NVEC2 Extents;	 // Particle Extent(s) ( define particle size )
		NVEC3 AllForces; // Particle Forces Apply to
	} NPARTICLE_COMPONENT_ADVANCED;

	typedef struct
	{
		Nu32 Flags;
		NPARTICLEPARAMS_NVECTOR2 BirthExtents;
		NPARTICLE_SETUP_EXTENTS pAdvancedParticle_SetupExtents;
		NPARTICLECOMPONENT_ADVANCED_CALLBACK pAdvancedParticle_UpdateCallBack;
		NPARTICLECOMPONENT_ADVANCED_CALLBACK pAdvancedParticle_DeathCallBack;
	} NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC;

	typedef struct
	{
		NPARTICLEPARAMS_NVECTOR2 BirthExtents;
		NPARTICLE_SETUP_EXTENTS pAdvancedParticle_SetupExtents;
		NPARTICLECOMPONENT_ADVANCED_CALLBACK pAdvancedParticle_UpdateCallBack;
		NPARTICLECOMPONENT_ADVANCED_CALLBACK pAdvancedParticle_DeathCallBack;
	} NPARTICLE_EMITTERCOMPONENT_ADVANCED;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	// Setup Particle Emitter Component Desc Parameters
	inline void NSetupParticleEmitterComponent_Advanced_Desc_BirthExtentsf_Params(NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC *pdesc, const Nf32 xbase, const Nf32 ybase, const Nf32 xvar, const Nf32 yvar)
	{
		FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_BIRTH_EXTENTS_AND_VARIATION);
		NVec2Set(&pdesc->BirthExtents.Base, xbase, ybase);
		NVec2Set(&pdesc->BirthExtents.Variation, xvar, yvar);
	}
	inline void NSetupParticleEmitterComponent_Advanced_BirthExtents_SetupHandle(NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC *pdesc, const NPARTICLE_SETUP_EXTENTS phandle)
	{
		FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_SETUPEXTENTS_HANDLE);
		pdesc->pAdvancedParticle_SetupExtents = phandle;
	}
	inline void NSetupParticleEmitterComponent_Advanced_Update_CallBack(NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC *pdesc, const NPARTICLECOMPONENT_ADVANCED_CALLBACK pcallback)
	{
		FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_UPDATE_CALLBACK);
		pdesc->pAdvancedParticle_UpdateCallBack = pcallback;
	}
	inline void NSetupParticleEmitterComponent_Advanced_Death_CallBack(NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC *pdesc, const NPARTICLECOMPONENT_ADVANCED_CALLBACK pcallback)
	{
		FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_DEATH_CALLBACK);
		pdesc->pAdvancedParticle_DeathCallBack = pcallback;
	}
	// Change Advanced Particles Emitter Parameters
	inline void NSetParticlesEmitterComponent_Advanced_BirthExtentsBase(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NVEC2 *pextents) { pemitter_advparams->BirthExtents.Base = *pextents; }
	inline void NSetParticlesEmitterComponent_Advanced_BirthExtentsVariation(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NVEC2 *pextents_var) { pemitter_advparams->BirthExtents.Variation = *pextents_var; }
	// Change Advanced Particles Emitter Parameters:Handles
	inline void NSetParticlesEmitterComponent_Advanced_BirthExtents_SetupHandle(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NPARTICLE_SETUP_EXTENTS phandle) { pemitter_advparams->pAdvancedParticle_SetupExtents = phandle; }
	inline void NSetParticlesEmitterComponent_Advanced_Update_CallBack(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NPARTICLECOMPONENT_ADVANCED_CALLBACK pcallback) { pemitter_advparams->pAdvancedParticle_UpdateCallBack = pcallback; }
	inline void NSetParticlesEmitterComponent_Advanced_Death_CallBack(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NPARTICLECOMPONENT_ADVANCED_CALLBACK pcallback) { pemitter_advparams->pAdvancedParticle_DeathCallBack = pcallback; }

	// ***************************************************************************************
	// **				LOW LEVEL:PRIVATE Inline  Setup Function							**
	// ***************************************************************************************
	inline void NSetupParticleComponent_Advanced(NPARTICLE_COMPONENT_ADVANCED *pparticle_advparams, const NPARTICLE_EMITTERCOMPONENT_ADVANCED *padvemitter, NVEC3 *pinitialforce)
	{
		padvemitter->pAdvancedParticle_SetupExtents(&pparticle_advparams->Extents, &padvemitter->BirthExtents);
		if (pinitialforce)
		{
			pparticle_advparams->AllForces = *pinitialforce; // At init the force apply to ( = Gravity... more often )
		}
		else
		{
			NVec3Set(&pparticle_advparams->AllForces, 0, 0, 0);
		}
	}
	inline void NClearParticleComponent_Advanced(NPARTICLE_COMPONENT_ADVANCED *pparticle_advparams)
	{
		Nmem0(pparticle_advparams, NPARTICLE_COMPONENT_ADVANCED);
	}
	inline void NSetupParticlesEmitterComponent_Advanced(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams, const NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC *pdesc)
	{
		// 	NPARTICLEPARAMS_NVECTOR2		BirthExtents;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_BIRTH_EXTENTS_AND_VARIATION))
		{
			pemitter_advparams->BirthExtents = pdesc->BirthExtents;
		}
		else
		{
			NVec2Set(&pemitter_advparams->BirthExtents.Base, DEFAULT_NPARTICLE_EMITTERCOMPONENT_ADVANCED_BIRTH_EXTENTS_2F);
			NVec2Set(&pemitter_advparams->BirthExtents.Variation, DEFAULT_NPARTICLE_EMITTERCOMPONENT_ADVANCED_BIRTH_EXTENTS_VARIATION_2F);
		}
		// 	NPARTICLE_SETUP_EXTENTS			pAdvancedParticle_SetupExtents;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_SETUPEXTENTS_HANDLE))
		{
			pemitter_advparams->pAdvancedParticle_SetupExtents = pdesc->pAdvancedParticle_SetupExtents;
		}
		else
		{
			pemitter_advparams->pAdvancedParticle_SetupExtents = DEFAULT_NPARTICLECOMPONENT_EMITTER_ADVANCED_SETUP_EXTENTS_HANDLE;
		}
		// 	NADVANCEDPARTICLE_CALLBACK		pAdvancedParticle_UpdateCallBack;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_UPDATE_CALLBACK))
		{
			pemitter_advparams->pAdvancedParticle_UpdateCallBack = pdesc->pAdvancedParticle_UpdateCallBack;
		}
		else
		{
			pemitter_advparams->pAdvancedParticle_UpdateCallBack = NULL;
		}
		// 	NADVANCEDPARTICLE_CALLBACK		pAdvancedParticle_DeathCallBack;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC_USE_DEATH_CALLBACK))
		{
			pemitter_advparams->pAdvancedParticle_DeathCallBack = pdesc->pAdvancedParticle_DeathCallBack;
		}
		else
		{
			pemitter_advparams->pAdvancedParticle_DeathCallBack = NULL;
		}
	}
	inline void NClearParticlesEmitterComponent_Advanced(NPARTICLE_EMITTERCOMPONENT_ADVANCED *pemitter_advparams)
	{
		Nmem0(pemitter_advparams, NPARTICLE_EMITTERCOMPONENT_ADVANCED);
	}
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPARTICLECOMPONENT_ADVANCED_H
