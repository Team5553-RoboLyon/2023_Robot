#ifndef __NPARTICLECOMPONENT_STANDARD_H
#define __NPARTICLECOMPONENT_STANDARD_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticleComponent_Standard.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
// #include "../NNode.h"
#include "../Maths/NVec3f32.h"
#include "../NCCS_3DCoord.h"
#include "../NTexture.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Core/NTime.h"
#include "../NFastRandExtend.h"

#include "NParticleParams.h"
#include "NParticleSetupHandles.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + CONSTANT ans DEFAULT VALUES
// Std	 : NPARTICLE
// Adv	 : NADVANCEDPARTICLE
// H.Std : NHOMINGPARTICLE
// H.Adv : NADVANCEDHOMINGPARTICLE
//
// | Std | Adv | Hom |     |     |     |
//  ----- ----- ----- ----- ----- -----
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_GRAVITY_3F 0.0f, -18.0f, 0.0f						 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_EMISSION_POINT_3F 0.0f, 0.0f, 0.0f					 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_EMISSION_EXTENTS_3F 1.0f, 1.0f, 1.0f				 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_SPEEDDAMPING 1.0f									 // |  O  |     |  	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_LIFESPAN_BASE 0.5f									 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_LIFESPAN_VARIATION 1.5f								 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_BASE NANGLE0							 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_VARIATION NANGLE0					 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_SPEED_BASE (Nf32) NANGLE90			 // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_SPEED_VARIATION (Nf32)(2 * NANGLEMAX) // |  O  |     |   	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_SPEED_3F 5.0f, 5.0f, 5.0f						 // |  O  |     |  	 |     |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_SPEED_VARIATION_3F 10.0f, 10.0f, 10.0f		 // |  O  |     |  	 |     |     |     |
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + DESCRIPTION FLAGS
//																										// | Std | Adv |H.Std|H.Adv|     |     |
//																										//  ----- ----- ----- ----- ----- -----
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_GRAVITY BIT_0							  // |  O  |  O  |  	 |     |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_DEFAULT_EMISSION_POINT BIT_1			  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_EMISSION_EXTENTS BIT_2				  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SPEEDDAMPING BIT_3					  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_LIFESPAN_AND_VARIATION BIT_4			  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATION_AND_VARIATION BIT_5	  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION BIT_6 // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_SPEED_AND_VARIATION BIT_7		  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONPOSITION_HANDLE BIT_8	  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONSPEED_HANDLE BIT_9		  // |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_ALL 0x3FF

	// ***************************************************************************************
	// **								 Structures											**
	// ***************************************************************************************
	// a Particle
	typedef struct
	{
		Nf32 Age;			// Particle Age
		Nf32 LifeSpan;		// Particle Lifespan
		Nf32 Rotation;		// Particle Current Angle
		Nf32 RotationSpeed; // Particle Current Rotation (Angle) Speed
		NVEC3 Position;		// Particle Current Position
		NVEC3 Speed;		// Particle Current speed
	} NPARTICLE_COMPONENT_STANDARD;

	typedef struct
	{
		Nu32 Flags;

		NVEC3 Gravity;
		NVEC3 DefaultEmissionPoint;	  // Particles Emitter position of the Emission point (there is just One per emitter actually )
		NCCS_3DCOORD EmissionExtents; // Particles Emitter Extents used for particles Distribution (could be Cartesian, Cylindric or Spheric !)
		Nf32 SpeedDamping;
		NPARTICLEPARAMS_NF32 LifeSpan;
		NPARTICLEPARAMS_NF32 BirthRotation;
		NPARTICLEPARAMS_NF32 BirthRotationSpeed;
		NPARTICLEPARAMS_NCCS_3DCOORD BirthSpeed;
		NPARTICLE_SETUP_POSITION pParticleSetupEmissionPosition;
		NPARTICLE_SETUP_SPEED pParticleSetupEmissionSpeed;
	} NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC;

	typedef struct
	{
		// Emitter Flags
		Nu32 Flags; // Emitter Flags
		// Particle Emission Params
		NVEC3 DefaultEmissionPoint;	  // Particles Emitter position
		NCCS_3DCOORD EmissionExtents; // Particles Emitter Extents used for particles Distribution (could be Cartesian, Cylindric or Spheric !)
		// Particle Update Params
		NVEC3 Gravity;
		Nf32 SpeedDamping;
		// Particle Setup Params
		NPARTICLEPARAMS_NF32 LifeSpan;
		NPARTICLEPARAMS_NF32 BirthRotation;
		NPARTICLEPARAMS_NF32 BirthRotationSpeed;
		NPARTICLEPARAMS_NCCS_3DCOORD BirthSpeed;
		// Handles
		NPARTICLE_SETUP_POSITION pParticleSetupEmissionPosition;
		NPARTICLE_SETUP_SPEED pParticleSetupEmissionSpeed;
	} NPARTICLE_EMITTERCOMPONENT_STANDARD;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	// Setup Particle Emitter Component Desc Parameters

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EMISSION SETUP
	// +
	// Default Emission Point:
	//	To setup the default Emission point. Particle birth position is going to be randomly choose inside a volume around this Emission point.
	//	This emission volume is going to have "EmissionExtents".
	inline void NSetupParticleEmitterComponent_Standard_Desc_DefaultEmissionPoint(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const NVEC3 *pv)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_DEFAULT_EMISSION_POINT);
		pemitter_std_desc->DefaultEmissionPoint = *pv;
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_DefaultEmissionPointf(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_DEFAULT_EMISSION_POINT);
		NVec3Set(&pemitter_std_desc->DefaultEmissionPoint, x, y, z);
	}

	// Emission Extents:
	// To define Emission Extents params. Choose one of these 3 coordinate system and be consistent with the choice of Emission Position Handle.
	inline void NSetupParticleEmitterComponent_Standard_Desc_CartesianEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_EMISSION_EXTENTS);
		NVec3Set(&pemitter_std_desc->EmissionExtents.Cartesian, x, y, z);
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_FastSphericEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 radius, const Nu32 inclination, const Nu32 azimuth)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_EMISSION_EXTENTS);
		NSetFastSphericf(&pemitter_std_desc->EmissionExtents.FastSpheric, radius, inclination, azimuth);
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_FastCylindricEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 radius, const Nf32 height, const Nu32 azimuth)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_EMISSION_EXTENTS);
		NSetFastCylindricf(&pemitter_std_desc->EmissionExtents.FastCylindric, radius, height, azimuth);
	}

	// Emission Position Handle:
	// To define the function used to define the position of each new born particle.
	// It can be one of the followings:
	//		NParticleHandle_SetupPointPosition
	//		NParticleHandle_SetupCartesianPosition
	//		NParticleHandle_SetupFastSphericZPosition
	//		NParticleHandle_SetupFastCylindricZPosition
	// This handle is going to use Emission Extents params and emission point (default and specific) to compute each particle born position.
	// !!! IMPORTANT !!! Be careful to choose compatible Emission Extents parameters and Emission Position Handle.
	// !!! IMPORTANT !!! Both need to use the same coordinates system
	inline void NSetupParticleEmitterComponent_Standard_Desc_EmissionPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const NPARTICLE_SETUP_POSITION phandle)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONPOSITION_HANDLE);
		pemitter_std_desc->pParticleSetupEmissionPosition = phandle;
	}

	// SpeedDamping:
	// SpeedDamping scale each particle speed 'at each turn'
	inline void NSetupParticleEmitterComponent_Standard_Desc_SpeedDamping(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 speeddamping)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SPEEDDAMPING);
		pemitter_std_desc->SpeedDamping = speeddamping;
	}
	// Gravity:
	// To define Gravity force vector used by the emitter.
	// Gravity is added to each particle speed 'at each turn'
	inline void NSetupParticleEmitterComponent_Standard_Desc_Gravity(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const NVEC3 *pgravity)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_GRAVITY);
		pemitter_std_desc->Gravity = *(NVEC3 *)pgravity;
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_Gravityf(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_GRAVITY);
		NVec3Set(&pemitter_std_desc->Gravity, x, y, z);
	}
	// About gravity and speed damping calculus:
	//
	//						new particle speed	=	particlespeed*speeddamping + delay*gravity
	//
	// Life Span:
	// To define particle life span base and variation.
	// Each particle life span is going to be define like this:
	//
	//						particle lifespan	=	base + rand*variation
	//
	inline void NSetupParticleEmitterComponent_Standard_Desc_LifeSpan_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 base, const Nf32 var)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_LIFESPAN_AND_VARIATION);
		pemitter_std_desc->LifeSpan.Base = base;
		pemitter_std_desc->LifeSpan.Variation = var;
	}

	// Birth Rotation:
	// To define initial particle rotation ( at birth ).
	// Each particle initial rotation is going to be define like this:
	//
	//						particle initial rotation	=	base + rand*variation
	//
	inline void NSetupParticleEmitterComponent_Standard_Desc_BirthRotation_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 base, const Nf32 var)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATION_AND_VARIATION);
		pemitter_std_desc->BirthRotation.Base = base;
		pemitter_std_desc->BirthRotation.Variation = var;
	}

	// Birth Rotation Speed:
	// To define particle birth rotation Speed.
	// Each particle initial rotation speed is going to be define like this:
	//
	//						particle initial rotation speed	=	base + rand*variation
	//
	inline void NSetupParticleEmitterComponent_Standard_Desc_BirthRotationSpeed_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 base, const Nf32 var)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION);
		pemitter_std_desc->BirthRotationSpeed.Base = base;
		pemitter_std_desc->BirthRotationSpeed.Variation = var;
	}

	// Birth Speed.
	// To define Particle Birth Speed. Choose one of these 3 coordinate system and be consistent with the choice of Emission Speed Setup Handle.
	inline void NSetupParticleEmitterComponent_Standard_Desc_CartesianBirthSpeedf_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 xbase, const Nf32 ybase, const Nf32 zbase, const Nf32 xvar, const Nf32 yvar, const Nf32 zvar)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_SPEED_AND_VARIATION);
		NVec3Set(&pemitter_std_desc->BirthSpeed.Base.Cartesian, xbase, ybase, zbase);
		NVec3Set(&pemitter_std_desc->BirthSpeed.Variation.Cartesian, xvar, yvar, zvar);
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_FastSphericBirthSpeedf_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 radiusbase, const Nu32 inclibase, const Nu32 azimbase, const Nf32 radiusvar, const Nu32 inclivar, const Nu32 azimvar)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_SPEED_AND_VARIATION);
		NSetFastSphericf(&pemitter_std_desc->BirthSpeed.Base.FastSpheric, radiusbase, inclibase, azimbase);
		NSetFastSphericf(&pemitter_std_desc->BirthSpeed.Variation.FastSpheric, radiusvar, inclivar, azimvar);
	}
	inline void NSetupParticleEmitterComponent_Standard_Desc_FastCylindricBirthSpeedf_Params(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const Nf32 radiusbase, const Nf32 hbase, const Nu32 azimbase, const Nf32 radiusvar, const Nf32 hvar, const Nu32 azimvar)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_SPEED_AND_VARIATION);
		NSetFastCylindricf(&pemitter_std_desc->BirthSpeed.Base.FastCylindric, radiusbase, hbase, azimbase);
		NSetFastCylindricf(&pemitter_std_desc->BirthSpeed.Variation.FastCylindric, radiusvar, hvar, azimvar);
	}

	// Emission Speed Setup Handle:
	// To define the function used to define the speed of each new born particle.
	// It can be one of the followings:
	//		NParticleHandle_SetupCartesianSpeed
	//		NParticleHandle_SetupFastSphericZSpeed
	//		NParticleHandle_SetupFastCylindricZSpeed
	// !!! IMPORTANT !!! Be careful to choose compatible Birth Speed parameters and Emission Speed Setup Handle.
	// !!! IMPORTANT !!! Both need to use the same coordinates system
	inline void NSetupParticleEmitterComponent_Standard_Desc_EmissionSpeed_SetupHandle(NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pemitter_std_desc, const NPARTICLE_SETUP_SPEED phandle)
	{
		FLAG_ON(pemitter_std_desc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONSPEED_HANDLE);
		pemitter_std_desc->pParticleSetupEmissionSpeed = phandle;
	}

	// Change Particle Emitter Component Parameters:
	// After Emitter particle Creation it still possible to change it's parameters by using these functions.
	// !!! IMPORTANT !!!
	// After using one of several of these functions it's recommended to call "NRefreshPrecalculatedParticles" for standard emitter
	// ... or "NRefreshPrecalculatedAdvancedParticles" for advanced particles emitter to recompute all the potential pre calculated particle.
	//
	inline void NSetParticleEmitterComponent_Standard_DefaultEmissionPoint(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NVEC3 *pv) { pemitter_stdparams->DefaultEmissionPoint = *pv; }
	inline void NSetParticleEmitterComponent_Standard_DefaultEmissionPointf(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 x, const Nf32 y, const Nf32 z) { NVec3Set(&pemitter_stdparams->DefaultEmissionPoint, x, y, z); }
	inline void NSetParticleEmitterComponent_Standard_CCSEmissionExtents(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NCCS_3DCOORD *pccs) { pemitter_stdparams->EmissionExtents = *pccs; }
	inline void NSetParticleEmitterComponent_Standard_Gravity(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NVEC3 *pv) { pemitter_stdparams->Gravity = *pv; }
	inline void NSetParticleEmitterComponent_Standard_Gravityf(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 x, const Nf32 y, const Nf32 z) { NVec3Set(&pemitter_stdparams->Gravity, x, y, z); }
	inline void NSetParticleEmitterComponent_Standard_SpeedDamping(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 speed_damping) { pemitter_stdparams->SpeedDamping = speed_damping; }
	inline void NSetParticleEmitterComponent_Standard_LifeSpanBase(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 lifespanbase) { pemitter_stdparams->LifeSpan.Base = lifespanbase; }
	inline void NSetParticleEmitterComponent_Standard_LifeSpanVariation(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 *plifespanvar) { pemitter_stdparams->LifeSpan.Variation = *plifespanvar; }
	inline void NSetParticleEmitterComponent_Standard_BirthRotationBase(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 rotation) { pemitter_stdparams->BirthRotation.Base = rotation; }
	inline void NSetParticleEmitterComponent_Standard_BirthRotationVariation(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 rotation_var) { pemitter_stdparams->BirthRotation.Variation = rotation_var; }
	inline void NSetParticleEmitterComponent_Standard_BirthRotationSpeedBase(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 rspeed_base) { pemitter_stdparams->BirthRotationSpeed.Base = rspeed_base; }
	inline void NSetParticleEmitterComponent_Standard_BirthRotationSpeedVariation(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const Nf32 rspeed_var) { pemitter_stdparams->BirthRotationSpeed.Variation = rspeed_var; }
	inline void NSetParticleEmitterComponent_Standard_CCSBirthSpeedBase(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NCCS_3DCOORD *pbirthspeed_base) { pemitter_stdparams->BirthSpeed.Base = *pbirthspeed_base; }
	inline void NSetParticleEmitterComponent_Standard_CCSBirthSpeedVariation(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NCCS_3DCOORD *pbirthspeed_var) { pemitter_stdparams->BirthSpeed.Variation = *pbirthspeed_var; }
	inline void NSetParticleEmitterComponent_Standard_EmissionPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NPARTICLE_SETUP_POSITION phandle) { pemitter_stdparams->pParticleSetupEmissionPosition = phandle; }
	inline void NSetParticleEmitterComponent_Standard_EmissionSpeed_SetupHandle(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemitter_stdparams, const NPARTICLE_SETUP_SPEED phandle) { pemitter_stdparams->pParticleSetupEmissionSpeed = phandle; }

	// ***************************************************************************************
	// **				LOW LEVEL:PRIVATE Inline Setup & Clear Functions					**
	// ***************************************************************************************
	inline void NSetupParticleComponent_Standard(NPARTICLE_COMPONENT_STANDARD *pparticlecomponent, const NPARTICLE_EMITTERCOMPONENT_STANDARD *pemittercomponent, const NVEC3 *pemissionpoint)
	{
		pparticlecomponent->Age = 0.0f;
		pparticlecomponent->LifeSpan = NFastRandEx(pemittercomponent->LifeSpan.Base, pemittercomponent->LifeSpan.Variation);
		pparticlecomponent->Rotation = NFastRandEx(pemittercomponent->BirthRotation.Base, pemittercomponent->BirthRotation.Variation);
		pparticlecomponent->RotationSpeed = NFastRandEx(pemittercomponent->BirthRotationSpeed.Base, pemittercomponent->BirthRotationSpeed.Variation);

		pemittercomponent->pParticleSetupEmissionPosition(&pparticlecomponent->Position, &pemittercomponent->EmissionExtents, pemissionpoint);
		pemittercomponent->pParticleSetupEmissionSpeed(&pparticlecomponent->Speed, &pemittercomponent->BirthSpeed);
	}
	inline void NClearParticleComponent_Standard(NPARTICLE_COMPONENT_STANDARD *pparticlecomponent)
	{
		Nmem0(pparticlecomponent, NPARTICLE_COMPONENT_STANDARD);
	}

	inline void NSetupParticlesEmitterComponent_Standard(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemittercomponent, const NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC *pdesc)
	{
		// 	NVEC3						Gravity;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_GRAVITY))
		{
			pemittercomponent->Gravity = pdesc->Gravity;
		}
		else
		{
			NVec3Set(&pemittercomponent->Gravity, DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_GRAVITY_3F);
		}
		// NVEC3						DefaultEmissionPoint;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_DEFAULT_EMISSION_POINT))
		{
			pemittercomponent->DefaultEmissionPoint = pdesc->DefaultEmissionPoint;
		}
		else
		{
			NVec3Set(&pemittercomponent->DefaultEmissionPoint, DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_EMISSION_POINT_3F);
		}
		// NCCS_3DCOORD					EmissionExtents;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_EMISSION_EXTENTS))
		{
			pemittercomponent->EmissionExtents = pdesc->EmissionExtents;
		}
		else
		{
			NVec3Set(&pemittercomponent->EmissionExtents.Cartesian, DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_EMISSION_EXTENTS_3F);
		}
		// Nf32							SpeedDamping;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SPEEDDAMPING))
		{
			pemittercomponent->SpeedDamping = pdesc->SpeedDamping;
		}
		else
		{
			pemittercomponent->SpeedDamping = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_SPEEDDAMPING;
		}
		// NPARTICLEPARAMS_NF32			LifeSpan;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_LIFESPAN_AND_VARIATION))
		{
			pemittercomponent->LifeSpan = pdesc->LifeSpan;
		}
		else
		{
			pemittercomponent->LifeSpan.Base = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_LIFESPAN_BASE;
			pemittercomponent->LifeSpan.Variation = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_LIFESPAN_VARIATION;
		}
		//	NPARTICLEPARAMS_NF32			BirthRotation;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATION_AND_VARIATION))
		{
			pemittercomponent->BirthRotation = pdesc->BirthRotation;
		}
		else
		{
			pemittercomponent->BirthRotation.Base = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_BASE;
			pemittercomponent->BirthRotation.Variation = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_VARIATION;
		}
		// NPARTICLEPARAMS_NF32			BirthRotationSpeed;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION))
		{
			pemittercomponent->BirthRotationSpeed = pdesc->BirthRotationSpeed;
		}
		else
		{
			pemittercomponent->BirthRotationSpeed.Base = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_SPEED_BASE;
			pemittercomponent->BirthRotationSpeed.Variation = DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_ROTATION_SPEED_VARIATION;
		}
		//	NPARTICLEPARAMS_NCCS_3DCOORD	BirthSpeed;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_BIRTH_SPEED_AND_VARIATION))
		{
			pemittercomponent->BirthSpeed = pdesc->BirthSpeed;
		}
		else
		{
			NVec3Set(&pemittercomponent->BirthSpeed.Base.Cartesian, DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_SPEED_3F);
			NVec3Set(&pemittercomponent->BirthSpeed.Variation.Cartesian, DEFAULT_NPARTICLE_EMITTERCOMPONENT_STANDARD_BIRTH_SPEED_VARIATION_3F);
		}

		//	NPARTICLE_SETUP_POSITION		pParticleSetupEmissionPosition;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONPOSITION_HANDLE))
		{
			pemittercomponent->pParticleSetupEmissionPosition = pdesc->pParticleSetupEmissionPosition;
		}
		else
		{
			pemittercomponent->pParticleSetupEmissionPosition = DEFAULT_NPARTICLECOMPONENT_EMITTER_STANDARD_SETUP_EMISSIONPOSITION_HANDLE;
		}
		// NPARTICLE_SETUP_SPEED			pParticleSetupEmissionSpeed;
		if (ISFLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_STANDARD_DESC_USE_SETUP_EMISSIONSPEED_HANDLE))
		{
			pemittercomponent->pParticleSetupEmissionSpeed = pdesc->pParticleSetupEmissionSpeed;
		}
		else
		{
			pemittercomponent->pParticleSetupEmissionSpeed = DEFAULT_NPARTICLECOMPONENT_EMITTER_STANDARD_SETUP_EMISSIONSPEED_HANDLE;
		}
	}

	inline void NClearParticlesEmitterComponent_Standard(NPARTICLE_EMITTERCOMPONENT_STANDARD *pemittercomponent)
	{
		Nmem0(pemittercomponent, NPARTICLE_EMITTERCOMPONENT_STANDARD);
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPARTICLECOMPONENT_STANDARD_H
