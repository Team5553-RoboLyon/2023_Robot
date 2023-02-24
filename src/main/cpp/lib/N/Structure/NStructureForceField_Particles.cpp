// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						   NStructureForceField_Particles.cpp						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "../NStructure.h"
#include "../Particles/NParticleComponent_Standard.h"
#include "../NStructureForceField.h"
#include "lib/N/NErrorHandling.h"

#include "lib/N/Utilities/Draw/NUT_Draw.h"

extern NARRAY NActivePhysicStructure_ForceFields_Array;

/*
static NSTRUCTUREFORCEFIELD_RESULT	_updateandapplyparticles( NSTRUCTUREFORCEFIELD *pff, const Nf32 fdelay )
{
	NSTRUCTUREFORCEFIELD_PARTICLES	*psp = (NSTRUCTUREFORCEFIELD_PARTICLES*)pff;
	Nu32							i,j,k;
	Nu32							stride;
	NSTRUCTURE						**pps;
	NSTRUCTURE						*pstruct;
	NJOINT							*pj;
	NVEC3						v;
	Nf32							f;
	Nf32							squareradius = psp->Radius * psp->Radius;
	NPARTICLE_COMPONENT_STANDARD	*pparticle;

	if( psp->pEmitterCore->ParticlesNb )
	{
		// 0) Update ForceField AABB:
		psp			= (NSTRUCTUREFORCEFIELD_PARTICLES*)pff;
		stride		= psp->pEmitterCore->ParticleStructSize;
		pparticle	= (NPARTICLE_COMPONENT_STANDARD*)psp->pEmitterCore->pParticlesList;
		psp->AABB.fXMax =  psp->AABB.fXMin = pparticle->Position.x;
		psp->AABB.fYMax =  psp->AABB.fYMin = pparticle->Position.y;
		psp->AABB.fZMax =  psp->AABB.fZMin = pparticle->Position.z;
		pparticle = (NPARTICLE_COMPONENT_STANDARD*)(((NBYTE*)pparticle) + stride);
		for(i=psp->pEmitterCore->ParticlesNb-1;i!=0;i--, pparticle = (NPARTICLE_COMPONENT_STANDARD*)(((NBYTE*)pparticle) + stride) )
		{
			if( pparticle->Position.x > psp->AABB.fXMax )
				psp->AABB.fXMax = pparticle->Position.x;
			else if( pparticle->Position.x < psp->AABB.fXMin )
				psp->AABB.fXMin = pparticle->Position.x;

			if( pparticle->Position.y > psp->AABB.fYMax )
				psp->AABB.fYMax = pparticle->Position.y;
			else if( pparticle->Position.y < psp->AABB.fYMin )
				psp->AABB.fYMin = pparticle->Position.y;

// 			if( pparticle->Position.z > psp->AABB.fZMax )
// 				psp->AABB.fZMax = pparticle->Position.z;
// 			else if( pparticle->Position.z < psp->AABB.fZMin )
// 				psp->AABB.fZMin = pparticle->Position.z;
		}

		// 1) Apply Force Field:
		pps = (NSTRUCTURE**)NActivePhysicStructuresPtr_Array.pFirst;
		for(i=NActivePhysicStructuresPtr_Array.Size;i!=0;i--,pps++)
		{
			pstruct = *pps;

			if( ISFLAG_ON(pstruct->Flags,FLAG_NSTRUCTURE_FORCEFIELD_INSENSITIVE) )
				continue;

			// check for influence
			if(psp->AABB.fXMax < pstruct->JointsAABB.fXMin)
				continue;

			if(psp->AABB.fXMin > pstruct->JointsAABB.fXMax)
				continue;

			if(psp->AABB.fYMax < pstruct->JointsAABB.fYMin)
				continue;

			if(psp->AABB.fYMin > pstruct->JointsAABB.fYMax)
				continue;

			// We don't need this for Snakes !
			// -------------------------------------------------

// 			if(psp->AABB.fZMax < pstruct->JointsAABB.fZMin)
// 				continue;
//
// 			if(psp->AABB.fZMin > pstruct->JointsAABB.fZMax)
// 				continue;

			// -------------------------------------------------

			// If we are here, that means  physic structure is under forcefield influence
			// So apply !
			pparticle = (NPARTICLE_COMPONENT_STANDARD*)psp->pEmitterCore->pParticlesList;
			for(k=psp->pEmitterCore->ParticlesNb;k!=0;k--, pparticle = (NPARTICLE_COMPONENT_STANDARD*)(((NBYTE*)pparticle) + stride) )
			{
				pj=(NJOINT*)pstruct->JointArray.pFirst;
				for( j=pstruct->JointArray.Size;j!=0;j--,pj++ )
				{
					NVec3Sub(&v,&pj->CurrentPosition,&pparticle->Position);
					f = v.x*v.x + v.y*v.y;// + v.z*v.z; just for snake !!!!
					if( f <= squareradius )
					{
						NVec3ScaleBy(&v,(squareradius - f)*psp->Intensity);
						v.z = 0.0f; // just for Snake.
						NAddForceToJoint(pj,&v);
						NVec3ScaleBy(&pparticle->Speed,0.990f);
						pparticle->LifeSpan+=0.005f;
					}
				}
			}
		}
		return NSTRUCTUREFORCEFIELD_RESULT_CONTINUE;// == 1, means this forcefield has to be continued.
	}
	else
	{
		return NSTRUCTUREFORCEFIELD_RESULT_CONTINUE;//NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE;	// == 0, means this force field has to stop,
		// and it will be remove from the updated forcefields array ...
	}
}*/

static NSTRUCTUREFORCEFIELD_RESULT _updateandapplyparticles(NSTRUCTUREFORCEFIELD *pff, const Nf32 fdelay)
{
	NSTRUCTUREFORCEFIELD_PARTICLES *psp = (NSTRUCTUREFORCEFIELD_PARTICLES *)pff;
	Nu32 i, j, k;
	Nu32 stride;
	NSTRUCTURE **pps;
	NSTRUCTURE *pstruct;
	NJOINT *pj;
	NVEC3 v;
	Nf32 f;
	Nf32 squareradius;
	NPARTICLE_COMPONENT_STANDARD *pparticle;

	if (psp->LifeSpan >= fdelay)
	{
		psp->LifeSpan -= fdelay;

		if (psp->pEmitterCore->ParticlesNb)
		{
			squareradius = psp->Radius * psp->Radius;
			stride = psp->pEmitterCore->ParticleStructSize;

			pparticle = (NPARTICLE_COMPONENT_STANDARD *)psp->pEmitterCore->pParticlesList;
			for (k = psp->pEmitterCore->ParticlesNb; k != 0; k--, pparticle = (NPARTICLE_COMPONENT_STANDARD *)(((NBYTE *)pparticle) + stride))
			{

				// 1) Apply Force Field:
				pps = (NSTRUCTURE **)NActivePhysicStructuresPtr_Array.pFirst;
				for (i = NActivePhysicStructuresPtr_Array.Size; i != 0; i--, pps++)
				{
					pstruct = *pps;

					if (ISFLAG_ON(pstruct->Flags, FLAG_NSTRUCTURE_FORCEFIELD_INSENSITIVE))
						continue;

					// check for influence
					if (pparticle->Position.x + psp->Radius < pstruct->JointsAABB.fXMin)
						continue;

					if (pparticle->Position.x - psp->Radius > pstruct->JointsAABB.fXMax)
						continue;

					if (pparticle->Position.y + psp->Radius < pstruct->JointsAABB.fYMin)
						continue;

					if (pparticle->Position.y - psp->Radius > pstruct->JointsAABB.fYMax)
						continue;

					// We don't need this for Snakes !
					// -------------------------------------------------
					// 				if(pparticle->Position.z + psp->Radius < pstruct->JointsAABB.fZMin)
					// 					continue;
					//
					// 				if(pparticle->Position.z - psp->Radius > pstruct->JointsAABB.fZMax)
					// 					continue;
					// -------------------------------------------------

					// If we are here, that means  physic structure is under particle forcefield influence
					// So apply !
					pj = (NJOINT *)pstruct->JointArray.pFirst;
					for (j = pstruct->JointArray.Size; j != 0; j--, pj++)
					{
						NVec3Sub(&v, &pj->CurrentPosition, &pparticle->Position);
						f = v.x * v.x + v.y * v.y; // + v.z*v.z; just for snake !!!!
						if (f <= squareradius)
						{
							NVec3ScaleBy(&v, (squareradius - f) * psp->Intensity);
							v.z = 0.0f; // just for Snake.
							NAddForceToJoint(pj, &v);
							NVec3ScaleBy(&pparticle->Speed, psp->ParticleSpeedDamping);
							pparticle->LifeSpan += psp->ParticleAddLifeSpan;

							if (psp->Influence_CallBack)
								psp->Influence_CallBack(pparticle, pj);
							i = 1; // Trick to stop parsing structure... A particle force field affect only one structure
								   // So after dealing with all the joints of this structure, the 'i loop' will stop !
						}
					}
				}
			}
		}
		return NSTRUCTUREFORCEFIELD_RESULT_CONTINUE;
	}
	else
	{
		return NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE;
		// == 0, means this force field has to stop,
		// and it will be remove from the updated forcefields array ...
	}
}

/*
static void _default_influence_callback(const NSTRUCTUREFORCEFIELD_PARTICLES *pforcefield, const NPARTICLE_COMPONENT_STANDARD *pparticle, const NJOINT *pjoint )
{
	// nothing.
}
*/

NSTRUCTUREFORCEFIELD *NSetupPhysicStructureForceField_Particles(NSTRUCTUREFORCEFIELD *pff, const NPARTICLE_EMITTERCOMPONENT_CORE *pemitter_core, const Nf32 radius, const Nf32 intensity, const Nf32 lifespan, const Nf32 part_speeddamping, const Nf32 part_addlife, NPARTICLEFORCEFIELD_CALLBACK influence_callback)
{
	NErrorIf(!pemitter_core, NERROR_NULL_POINTER);

	Nmem0(pff, NSTRUCTUREFORCEFIELD);
	pff->Particles.pUpdateAndApplyForceField = _updateandapplyparticles;
	pff->Particles.pEmitterCore = (NPARTICLE_EMITTERCOMPONENT_CORE *)pemitter_core;
	pff->Particles.Intensity = intensity;
	pff->Particles.Radius = radius;
	pff->Particles.LifeSpan = lifespan;
	pff->Particles.ParticleAddLifeSpan = part_addlife;
	pff->Particles.ParticleSpeedDamping = part_speeddamping;
	// 	if(influence_callback)
	pff->Particles.Influence_CallBack = influence_callback;
	// 	else
	// 		pff->Particles.Influence_CallBack		= _default_influence_callback;

	return pff;
}

Nbool NActivatePhysicStructureForceField_Particles(const NPARTICLE_EMITTERCOMPONENT_CORE *pemitter_core, const Nf32 radius, const Nf32 intensity, const Nf32 lifespan, const Nf32 part_speeddamping, const Nf32 part_addlife, NPARTICLEFORCEFIELD_CALLBACK influence_callback)
{
	NSTRUCTUREFORCEFIELD *pff;
	Nu32 i;
	// 0) Restriction:	Only One force field Particle per Emitter !
	//					So, check if there is an already created force field particle linked with the same particle emitter
	//					this one is going to be used and reset with the new incoming parameters !
	pff = (NSTRUCTUREFORCEFIELD *)NActivePhysicStructure_ForceFields_Array.pFirst;
	for (i = NActivePhysicStructure_ForceFields_Array.Size; i -= 0; i--, pff++)
	{
		if (pff->pUpdateAndApplyForceField == _updateandapplyparticles && ((NSTRUCTUREFORCEFIELD_PARTICLES *)pff)->pEmitterCore == pemitter_core)
		{
			NSetupPhysicStructureForceField_Particles(pff, pemitter_core, radius, intensity, lifespan, part_speeddamping, part_addlife, influence_callback);
			return NTRUE;
		}
	}

	// 1) If we are here, that means there is no already created Force Filed Particles referencing the incominf particle Emitter...
	if (NActivePhysicStructure_ForceFields_Array.Size < NActivePhysicStructure_ForceFields_Array.Capacity)
	{
		// SetUp Data
		pff = (NSTRUCTUREFORCEFIELD *)NActivePhysicStructure_ForceFields_Array.pFirst;
		NSetupPhysicStructureForceField_Particles(&pff[NActivePhysicStructure_ForceFields_Array.Size], pemitter_core, radius, intensity, lifespan, part_speeddamping, part_addlife, influence_callback);
		NActivePhysicStructure_ForceFields_Array.Size += 1;
		return NTRUE;
	}
	return NFALSE;
}
