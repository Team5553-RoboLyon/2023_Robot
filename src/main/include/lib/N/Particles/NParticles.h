#ifndef __NPARTICLES_H
#define __NPARTICLES_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticles.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
//#include "../NNode.h"
#include "../Maths/NVec3f32.h"
#include "../NCCS_3DCoord.h"
#include "../Geometry/NGeometry.h"
#include "../NTexture.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Core/NTime.h"
#include "../NFastRandExtend.h"

#include "NParticleComponent_Core.h"
#include "NParticleComponent_LUT.h"
#include "NParticleComponent_Standard.h"


#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// a Particle
typedef struct
{
	NPARTICLE_COMPONENT_STANDARD	Std;
}NPARTICLE;

typedef struct  
{
	NPARTICLE_EMITTERCOMPONENT_CORE_DESC		CoreDesc;
	NPARTICLE_EMITTERCOMPONENT_LUT_DESC			LutDesc;
	NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC	StdDesc;
}NPARTICLES_EMITTER_DESC;
typedef struct 
{
	NPARTICLE_EMITTERCOMPONENT_CORE		Core;
	NPARTICLE_EMITTERCOMPONENT_LUT		Lut;
	NPARTICLE_EMITTERCOMPONENT_STANDARD	Std;
}NPARTICLES_EMITTER;


// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Particles Emitter
NPARTICLES_EMITTER*	NSetupParticlesEmitter(NPARTICLES_EMITTER *pemitter, const NPARTICLES_EMITTER_DESC *pdesc);
NPARTICLES_EMITTER*	NCreateParticlesEmitter(const NPARTICLES_EMITTER_DESC *pdesc);
void				NClearParticlesEmitter(NPARTICLES_EMITTER* pemitter);
void				NDeleteParticlesEmitter(NPARTICLES_EMITTER* pemitter);

// Emitter Commands
// To emit particles from a specific emitter at a specific point is space. if specific emission point is NULL, the default emission point is used.
void				NEmitParticles(NPARTICLES_EMITTER *pemitter,const Nu16 nbparticles,const NVEC3 *pspecific_emissionpoint);

// Refresh just one time after "all" ParticlesEmitter params changes ...
// User can change emitter standard  parameter at any time. But, to speed up particles update some particles may be already pre-calculated
// from previous params. So after using one or several "NSetParticleEmitterComponent_Standard_xxx" functions to change particle emitter standard parameters
// it's better to call the "NRefreshPrecalculatedParticles" to re-calculate with the new params all the particles already calculated.
void				NRefreshPrecalculatedParticles(NPARTICLES_EMITTER *pemitter);

// To 'kill' all alive particles
void				NResetParticlesEmitter(NPARTICLES_EMITTER *pemitter);
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + LOW LEVEL - PRIVATE USE
// +
void NRenderableUpdate_ParticlesEmitter(NRENDERABLE *prenderable, void* powner, const NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NPARTICLES_H 

