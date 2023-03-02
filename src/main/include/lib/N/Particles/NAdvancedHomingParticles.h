#ifndef __NADVANCEDHOMINGPARTICLES_H
#define __NADVANCEDHOMINGPARTICLES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NAdvancedHomingParticles.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
// #include "lib/N/Containers/NNode.h"
#include "../Maths/NVec3f32.h"
#include "../NSpheric.h"
#include "../NCylindric.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../NTexture.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Core/NTime.h"
#include "../NCCS_3DCoord.h"
#include "../NFastRandExtend.h"

#include "NParticleComponent_Core.h"
#include "NParticleComponent_LUT.h"
#include "NParticleComponent_Homing.h"
#include "NParticleComponent_Advanced.h"

#include "NParticleSetupHandles.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **								 Structures											**
	// ***************************************************************************************
	// a Advanced Homing Particle
	typedef struct
	{
		NPARTICLE_COMPONENT_HOMING Homing;
		NPARTICLE_COMPONENT_ADVANCED Advanced;
	} NADVANCEDHOMINGPARTICLE;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE_DESC CoreDesc;
		NPARTICLE_EMITTERCOMPONENT_LUT_DESC LutDesc;
		NPARTICLE_EMITTERCOMPONENT_HOMING_DESC HomingDesc;
		NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC AdvancedDesc;
	} NADVANCEDHOMINGPARTICLES_EMITTER_DESC;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE Core;
		NPARTICLE_EMITTERCOMPONENT_LUT Lut;
		NPARTICLE_EMITTERCOMPONENT_HOMING Homing;
		NPARTICLE_EMITTERCOMPONENT_ADVANCED Advanced;
	} NADVANCEDHOMINGPARTICLES_EMITTER;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	NADVANCEDHOMINGPARTICLES_EMITTER *NSetUpAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER *pemit, const NADVANCEDHOMINGPARTICLES_EMITTER_DESC *pdesc);
	NADVANCEDHOMINGPARTICLES_EMITTER *NCreateAdvancedHomingParticlesEmitter(const NADVANCEDHOMINGPARTICLES_EMITTER_DESC *pdesc);
	void NDeleteAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER *pemit);
	void NClearAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER *pemit);

	// Emitter Commands
	// To emit particles from a specific emitter at a specific point is space. if  emission point is NULL, the default emission point is used, same thing for  target point.
	void NEmitAdvancedHomingParticles(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter, const Nu16 nbparticles, const NVEC3 *pspecific_emissionpoint, const NVEC3 *pspecific_targetpoint);
	// Refresh just one time after "all" ParticlesEmitter params changes ... (Only if its necessary, of course)
	// User can change emitter  parameters at any time. But, to speed up particles update some particles may be already pre-calculated
	// from previous params. So after using one or several "NSetParticleEmitterComponent_Homing_xxx" or/and "NSetParticleEmitterComponent_Advanced_xxx" functions to change particle emitter parameters
	// it's better to call the "NRefreshPrecalculatedAdvancedHomingParticles" to re-calculate with the new params all the particles already calculated.
	void NRefreshPrecalculatedAdvancedHomingParticles(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter);

	// To Kill All ALive Particles
	void NResetAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOW LEVEL - PRIVATE USE
	// +
	void NRenderableUpdate_AdvancedHomingParticlesEmitter(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NADVANCEDHOMINGPARTICLES_H
