#ifndef __NADVANCEDPARTICLES_H
#define __NADVANCEDPARTICLES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NAdvancedParticles.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
// #include "../NNode.h"
#include "../Maths/NVec3f32.h"
#include "../NCCS_3DCoord.h"
#include "../NTexture.h"
#include "../Core/NTime.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../Render/Renderable/NRenderable.h"
#include "../NFastRandExtend.h"

#include "NParticleComponent_Core.h"
#include "NParticleComponent_LUT.h"
#include "NParticleComponent_Standard.h"
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
	// an Advanced Particle ... for Advanced Emitter
	typedef struct
	{
		NPARTICLE_COMPONENT_STANDARD Std;	   // "Standard" part
		NPARTICLE_COMPONENT_ADVANCED Advanced; // "Advanced" part
	} NADVANCEDPARTICLE;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE_DESC CoreDesc;
		NPARTICLE_EMITTERCOMPONENT_LUT_DESC LutDesc;
		NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC StdDesc;
		NPARTICLE_EMITTERCOMPONENT_ADVANCED_DESC AdvancedDesc;
	} NADVANCEDPARTICLES_EMITTER_DESC;

	typedef struct
	{
		NPARTICLE_EMITTERCOMPONENT_CORE Core;
		NPARTICLE_EMITTERCOMPONENT_LUT Lut;
		NPARTICLE_EMITTERCOMPONENT_STANDARD Std;
		NPARTICLE_EMITTERCOMPONENT_ADVANCED Advanced;
	} NADVANCEDPARTICLES_EMITTER;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	// particles Emitter
	NADVANCEDPARTICLES_EMITTER *NSetUpAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter, const NADVANCEDPARTICLES_EMITTER_DESC *pdesc);
	NADVANCEDPARTICLES_EMITTER *NCreateAdvancedParticlesEmitter(const NADVANCEDPARTICLES_EMITTER_DESC *pdesc);
	void NClearAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter);
	void NDeleteAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter);
	// Emitter Commands
	// To emit particles from a specific emitter at a specific point is space. if specific emission point is NULL, the default emission point is used.
	void NEmitAdvancedParticles(NADVANCEDPARTICLES_EMITTER *pemitter, const Nu16 nbparticles, const NVEC3 *pspecific_emissionpoint);
	// Refresh just one time after "all" ParticlesEmitter params changes ... (Only if its necessary, of course)
	// User can change emitter  parameters at any time. But, to speed up particles update some particles may be already pre-calculated
	// from previous params. So after using one or several "NSetParticleEmitterComponent_Standard_xxx" or/and "NSetParticleEmitterComponent_Advanced_xxx" functions to change particle emitter parameters
	// it's better to call the "NRefreshPrecalculatedAdvancedParticles" to re-calculate with the new params all the particles already calculated.
	void NRefreshPrecalculatedAdvancedParticles(NADVANCEDPARTICLES_EMITTER *pemitter);

	// To 'kill' all alive particles
	void NResetAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOW LEVEL - PRIVATE USE
	// +
	void NRenderableUpdate_AdvancedParticlesEmitter(NRENDERABLE *prenderable, void *powner, const NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NADVANCEDPARTICLES_H
