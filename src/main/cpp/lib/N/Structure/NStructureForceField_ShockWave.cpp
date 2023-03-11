// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						  NStructureForceField_ShockWave.cpp						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/NStructure.h"
#include "lib/N/NStructureForceField.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/NErrorHandling.h"

extern NARRAY NActivePhysicStructure_ForceFields_Array;

static NSTRUCTUREFORCEFIELD_RESULT _updateandapplyshockwave(NSTRUCTUREFORCEFIELD *pff, const Nf32 fdelay)
{
	NSTRUCTUREFORCEFIELD_SHOCKWAVE *psw;
	Nu32 i;
	NSTRUCTURE **pps;
	NSTRUCTURE *pstruct;
	NJOINT *pj;
	Nu32 j;
	NVEC3 v;
	Nf32 currentintensity;
	Nf32 square_radius;
	Nf32 f;

	psw = (NSTRUCTUREFORCEFIELD_SHOCKWAVE *)pff;

	psw->Age += fdelay;
	if (psw->Age <= psw->LifeSpan)
	{
		// Update ShockWave size
		psw->Radius += psw->PropagationVelocity * fdelay;

		//-----------------------------------------------------------------------------------------
		/*
				NUT_DRAW_ELLIPSE	ellipse;
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.Center = psw->Epicenter;
				ellipse.Extents.x = psw->Radius;
				ellipse.Extents.y = psw->Radius;
				ellipse.SliceNb = 128;
				NSetColorf(&ellipse.Color,NCOLOR_PRESET3F_WHITE,1.0f);
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
				NUT_Draw_Ellipse(&ellipse);
		*/
		//-----------------------------------------------------------------------------------------

		square_radius = psw->Radius * psw->Radius;

		// Update ShockWave AABB
		psw->AABB.fXMax = psw->Epicenter.x + psw->Radius;
		psw->AABB.fYMax = psw->Epicenter.y + psw->Radius;
		// psw->AABB.fZMax = psw->Epicenter.z + psw->Radius;
		psw->AABB.fXMin = psw->Epicenter.x - psw->Radius;
		psw->AABB.fYMin = psw->Epicenter.y - psw->Radius;
		// psw->AABB.fZMin = psw->Epicenter.z - psw->Radius;

		pps = (NSTRUCTURE **)NGetFirstArrayPtr(&NActivePhysicStructuresPtr_Array);
		for (i = 0; i < NGetArraySize(&NActivePhysicStructuresPtr_Array); i++, pps++)
		{
			pstruct = *pps;

			if (ISFLAG_ON(pstruct->Flags, FLAG_NSTRUCTURE_FORCEFIELD_INSENSITIVE))
				continue;

			// check for influence
			if (psw->AABB.fXMax < pstruct->JointsAABB.fXMin)
				continue;

			if (psw->AABB.fXMin > pstruct->JointsAABB.fXMax)
				continue;

			if (psw->AABB.fYMax < pstruct->JointsAABB.fYMin)
				continue;

			if (psw->AABB.fYMin > pstruct->JointsAABB.fYMax)
				continue;

			// We don't need this for Snakes !
			// -------------------------------------------------
			/*
						if(psw->AABB.fZMax < pstruct->JointsAABB.fZMin)
							continue;

						if(psw->AABB.fZMin > pstruct->JointsAABB.fZMax)
							continue;
			*/
			// -------------------------------------------------

			// If we are here, that means  physic structure is under forcefield influence
			// So apply !
			currentintensity = psw->Intensity * (psw->LifeSpan - psw->Age);
			pj = (NJOINT *)NGetFirstArrayPtr(&pstruct->JointArray);
			for (j = 0; j < NGetArraySize(&pstruct->JointArray); j++, pj++)
			{
				NVec3Sub(&v, &pj->CurrentPosition, &psw->Epicenter);
				f = v.x * v.x + v.y * v.y; //+v.z*v.z
				if (f <= square_radius)
				{
					NVec3ScaleBy(&v, currentintensity / NFastSqrt(f));
					NAddForceToJoint(pj, &v);

					/*
										// Detach !
										if(ISFLAG_ON(pj->Flags,FLAG_JOINT_FIXED) && NVec3SquareLength(&pj->vForcesAccumulator) > 100000.0f )
										{
											printf("\n force accumulator: %.5f",NVec3SquareLength(&pj->vForcesAccumulator));
											FLAG_OFF(pj->Flags,FLAG_JOINT_FIXED);
										}
					*/
				}
			}
		}
		return NSTRUCTUREFORCEFIELD_RESULT_CONTINUE; // == 1, means this forcefield has to be continued.
	}
	else
	{
		return NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE; // == 0, means this force field has to stop,
															// and it will be remove from the updated forcefields array ...
	}
}

NSTRUCTUREFORCEFIELD *NSetupPhysicStructureForceField_ShockWave(NSTRUCTUREFORCEFIELD *pff, const NVEC3 *pepicenter, const Nf32 intensity, const Nf32 lifespan, const Nf32 propagation_velocity)
{
	// 	NSTRUCTUREFORCEFIELD_SHOCKWAVE	*psw;
	//
	// 	psw = (NSTRUCTUREFORCEFIELD_SHOCKWAVE*)pff;
	//
	// 	psw->pUpdateAndApplyForceField	= _updateandapplyshockwave;
	// 	psw->Epicenter					= *pepicenter;
	// 	psw->Intensity					= intensity;
	// 	psw->LifeSpan					= lifespan;
	// 	psw->PropagationVelocity		= propagation_velocity;
	// 	psw->Age						= 0.0f;
	// 	psw->Radius						= 0.0f;

	Nmem0(pff, NSTRUCTUREFORCEFIELD);
	pff->ShockWave.pUpdateAndApplyForceField = _updateandapplyshockwave;
	pff->ShockWave.Epicenter = *pepicenter;
	pff->ShockWave.Intensity = intensity;
	pff->ShockWave.LifeSpan = lifespan;
	pff->ShockWave.PropagationVelocity = propagation_velocity;
	pff->ShockWave.Age = 0.0f;
	pff->ShockWave.Radius = 0.0f;

	return pff;
}

Nbool NActivatePhysicStructureForceField_ShockWave(const NVEC3 *pepicenter, const Nf32 intensity, const Nf32 lifespan, const Nf32 propagation_velocity)
{
	NSTRUCTUREFORCEFIELD *pff;

	if (NActivePhysicStructure_ForceFields_Array.Size < NActivePhysicStructure_ForceFields_Array.Capacity)
	{
		// SetUp Data
		pff = (NSTRUCTUREFORCEFIELD *)NActivePhysicStructure_ForceFields_Array.pFirst;
		NSetupPhysicStructureForceField_ShockWave(&pff[NActivePhysicStructure_ForceFields_Array.Size], pepicenter, intensity, lifespan, propagation_velocity);
		NActivePhysicStructure_ForceFields_Array.Size += 1;
		return NTRUE;
	}
	return NFALSE;
}
