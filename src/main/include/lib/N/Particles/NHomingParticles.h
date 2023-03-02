#ifndef __NHOMINGPARTICLES_H
#define __NHOMINGPARTICLES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NHomingParticles.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
// #include "../NNode.h"
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
#include "NParticleComponent_Homing.h"
#include "NParticleComponent_LUT.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **								 Structures											**
	// ***************************************************************************************
	// a Homing Particle
	typedef struct
	{
		NPARTICLE_COMPONENT_HOMING Homing;
	} NHOMINGPARTICLE;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE_DESC CoreDesc;
		NPARTICLE_EMITTERCOMPONENT_LUT_DESC LutDesc;
		NPARTICLE_EMITTERCOMPONENT_HOMING_DESC HomingDesc;
	} NHOMINGPARTICLES_EMITTER_DESC;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE Core;
		NPARTICLE_EMITTERCOMPONENT_LUT Lut;
		NPARTICLE_EMITTERCOMPONENT_HOMING Homing;
	} NHOMINGPARTICLES_EMITTER;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	NHOMINGPARTICLES_EMITTER *NSetUpHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER *pemit, const NHOMINGPARTICLES_EMITTER_DESC *pdesc);
	NHOMINGPARTICLES_EMITTER *NCreateHomingParticlesEmitter(const NHOMINGPARTICLES_EMITTER_DESC *pdesc);
	void NDeleteHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER *pemitter);
	void NClearHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER *pemitter);
	// Emitter Commands
	// To emit particles from a specific emitter at a specific point is space. if  emission point is NULL, the default emission point is used, same thing for  target point.
	void NEmitHomingParticles(NHOMINGPARTICLES_EMITTER *pemitter, const Nu16 nbparticles, const NVEC3 *pemissionpoint, const NVEC3 *ptargetpoint);

	// Refresh just one time after "all" ParticlesEmitter params changes ...
	// User can change emitter  parameters at any time. But, to speed up particles update some particles may be already pre-calculated
	// from previous params. So after using one or several "NSetParticleEmitterComponent_Homing_xxx" functions to change particle emitter parameters
	// it's better to call the "NRefreshPrecalculatedHomingParticles" to re-calculate with the new params all the particles already calculated.
	void NRefreshPrecalculatedHomingParticles(NHOMINGPARTICLES_EMITTER *pemitter);

	// To Kill all Alive Particles
	void NResetHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER *pemitter);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOW LEVEL - PRIVATE USE
	// +
	void NRenderableUpdate_HomingParticlesEmitter(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NHOMINGPARTICLES_H
