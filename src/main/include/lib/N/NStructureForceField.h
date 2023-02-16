#ifndef __NSTRUCTUREFORCEFIELD_H
#define __NSTRUCTUREFORCEFIELD_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NStructureForceField.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"
#include "NFlags.h"
#include "NBoundingVolumes.h"
#include "NStructure.h"
#include "./Particles/NParticleComponent_Core.h"
#include "./Particles/NParticleComponent_Standard.h"
#include "./Containers/NNode.h"
#include "./Containers/NXNode.h"
#include "./Containers/NArray.h"
#include "./Core/NTime.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef unsigned char		NSTRUCTUREFORCEFIELD_RESULT;
typedef union NSTRUCTUREFORCEFIELD NSTRUCTUREFORCEFIELD;

typedef NSTRUCTUREFORCEFIELD_RESULT (*NAPPLYFORCEFIELD_FCT)(NSTRUCTUREFORCEFIELD* pforcefield, Nf32 fdelay);
#define NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE			0		// remove from NpActivePhysicStructureForceFieldsPtrArray
#define NSTRUCTUREFORCEFIELD_RESULT_CONTINUE				1

typedef struct 
{
	// + --- Common part --------------------
	NAPPLYFORCEFIELD_FCT	pUpdateAndApplyForceField;
	NAABB					AABB;
	// + ------------------------------------
}NSTRUCTUREFORCEFIELD_BASIC;

typedef struct
{
	// + --- Common part --------------------
	NAPPLYFORCEFIELD_FCT	pUpdateAndApplyForceField;
	NAABB					AABB;
	// + ------------------------------------
	NVEC3				Epicenter;
	Nf32					Intensity;
	Nf32					LifeSpan;
	Nf32					PropagationVelocity;
	Nf32					Radius;
	Nf32					Age;
}NSTRUCTUREFORCEFIELD_SHOCKWAVE;

typedef void (*NPARTICLEFORCEFIELD_CALLBACK)(const NPARTICLE_COMPONENT_STANDARD *pparticle, const NJOINT *pjoint );
typedef struct
{
	// + --- Common part --------------------
	NAPPLYFORCEFIELD_FCT			pUpdateAndApplyForceField;
	NAABB							AABB;
	// + ------------------------------------
	NPARTICLE_EMITTERCOMPONENT_CORE	*pEmitterCore;			// Each alive particle of this emitter will generate a forcefield
	Nf32							Radius;					// ForceField Radius of One Particle
	Nf32							Intensity;				// ForceField Intensity of One Particle
	Nf32							LifeSpan;				// Of the whole forcefields ( of this object actually )
	Nf32							ParticleSpeedDamping;	// for active particle ( in contact with a structure )
	Nf32							ParticleAddLifeSpan;	// for active particle ( in contact with a structure )
	NPARTICLEFORCEFIELD_CALLBACK	Influence_CallBack;		// Function called each turn for each contact between a particle and a joint. 	
}NSTRUCTUREFORCEFIELD_PARTICLES;

union NSTRUCTUREFORCEFIELD
{
	NAPPLYFORCEFIELD_FCT				pUpdateAndApplyForceField;

	NSTRUCTUREFORCEFIELD_BASIC			Basic;
	NSTRUCTUREFORCEFIELD_SHOCKWAVE		ShockWave;
	NSTRUCTUREFORCEFIELD_PARTICLES		Particles;
};



Nbool								NActivatePhysicStructureForceField_Basic( const NAPPLYFORCEFIELD_FCT applyforcefield_fct );
Nbool								NActivatePhysicStructureForceField_ShockWave( const NVEC3 *pepicenter, const Nf32 intensity, const Nf32 lifespan, const Nf32 propagation_velocity );
Nbool								NActivatePhysicStructureForceField_Particles( const NPARTICLE_EMITTERCOMPONENT_CORE *pemitter_core, const Nf32  radius, const Nf32 intensity, const Nf32 lifespan, const Nf32 part_speeddamping, const Nf32 part_addlife, NPARTICLEFORCEFIELD_CALLBACK influence_callback );

// LOW LEVEL USE
NSTRUCTUREFORCEFIELD*				NSetupPhysicStructureForceField_Basic(NSTRUCTUREFORCEFIELD* pff, const NAPPLYFORCEFIELD_FCT applyforcefield_fct);
NSTRUCTUREFORCEFIELD*				NSetupPhysicStructureForceField_ShockWave(NSTRUCTUREFORCEFIELD* pff, const NVEC3 *pepicenter, const Nf32 intensity, const Nf32 lifespan, const Nf32 propagation_velocity );
NSTRUCTUREFORCEFIELD*				NSetupPhysicStructureForceField_Particles(NSTRUCTUREFORCEFIELD* pff, const NPARTICLE_EMITTERCOMPONENT_CORE *pemitter_core, const Nf32  radius, const Nf32 intensity, const Nf32 lifespan, const Nf32 part_speeddamping, const Nf32 part_addlife, NPARTICLEFORCEFIELD_CALLBACK influence_callback );

void								NClearPhysicStructureForceField(NSTRUCTUREFORCEFIELD *pff);

/*
void								NBindPhysicStructureForceField(NSTRUCTUREFORCEFIELD_BASIC* pff);
void								NUnbindPhysicStructureForceField(NSTRUCTUREFORCEFIELD_BASIC* pff);
void								NClearPhysicStructureForceFieldBasic(void *pff);
NSTRUCTUREFORCEFIELD_BASIC*			NCreatePhysicStructureForceFieldBasic(NAPPLYFORCEFIELD_FCT applyforcefield_fct);
void								NDeletePhysicStructureForceFieldBasic(void *pff);
void								NClearPhysicStructureForceFieldShockWave(void *pff);
NSTRUCTUREFORCEFIELD_SHOCKWAVE*		NCreatePhysicStructureForceFieldShockWave( NVEC3 *pepicenter, Nf32 intensity, Nf32 lifespan, Nf32 propagation_velocity );
void								NDeletePhysicStructureForceFieldShockWave(void *psw);
*/
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSTRUCTUREFORCEFIELD_H 