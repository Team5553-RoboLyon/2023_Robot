#ifndef __NPARTICLECOMPONENT_HOMING_H
#define __NPARTICLECOMPONENT_HOMING_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticleComponent_Homing.h							**
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
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_POINT_3F					0.0f,0.0f,0.0f		// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_POINT_3F					0.0f,0.0f,1.0f		// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_EXTENTS_3F				1.0f,1.0f,1.0f		// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_EXTENTS_3F					1.0f,1.0f,1.0f		// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_FROM_TARGET_SQUARE_DIST_THRESHOLD	0.0f	
#define	DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_SPEEDDAMPING						1.0f				// |  O  |     |  	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_LIFESPAN_BASE						0.5f				// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_LIFESPAN_VARIATION				1.5f				// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_BASE				NANGLE0				// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_VARIATION			NANGLE0				// |  O  |     |   	 |     |     |     |	

#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_SPEED_BASE			(Nf32)NANGLE90		// |  O  |     |   	 |     |     |     |		
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_SPEED_VARIATION	(Nf32)(2*NANGLEMAX)	// |  O  |     |   	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_SPEED_BASE					0.1f				// |  O  |     |  	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_SPEED_VARIATION				0.1f				// |  O  |     |  	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ACCELERATION_BASE			0.1f				// |  O  |     |  	 |     |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ACCELERATION_VARIATION		0.1f				// |  O  |     |  	 |     |     |     |	

#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_TANGENT_BASE_3F			-0.5f, 0.5f, 0.0f	// |     |     |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_TANGENT_VARIATION_3F		 1.0f, 1.0f, 0.0f	// |     |     |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_TANGENT_BASE_3F			-0.5f,-0.5f, 0.0f	// |     |     |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_TANGENT_VARIATION_3F		 1.0f,-1.0f, 0.0f	// |     |     |  O	 |  O  |     |     |	
	
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + DESCRIPTION FLAGS
//																										// | Std | Adv |H.Std|H.Adv|     |     |
//																										//  ----- ----- ----- ----- ----- -----	
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_EMISSION_POINT					BIT_0	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_TARGET_POINT					BIT_1	// |  O  |  O  |  	 |     |     |     |	
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_EXTENTS						BIT_2	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_EXTENTS							BIT_3	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SPEEDDAMPING							BIT_4	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_LIFESPAN_AND_VARIATION					BIT_5	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATION_AND_VARIATION			BIT_6	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION		BIT_7	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_SPEED_AND_VARIATION				BIT_8	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ACCELERATION_AND_VARIATION		BIT_9	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_TANGENT_AND_VARIATION			BIT_10	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_TANGENT_AND_VARIATION			BIT_11	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALEMISSIONPOSITION_HANDLE		BIT_12	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALTARGETPOSITION_HANDLE		BIT_13	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_EMISSIONTANGENT_HANDLE			BIT_14	// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_TARGETTANGENT_HANDLE				BIT_15	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_FROM_TARGET_DIST_THRESHOLD				BIT_16	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_ALL										0x1FFFF

#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_DEATH_AT_END								BIT_29	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_EMISSIONPOINT					BIT_30	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_TARGETPOINT						BIT_31	// |     |     |  O	 |  O  |     |     |

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + COMPONENT FLAGS
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_AT_END										BIT_0	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_FROM_TARGET_DIST_THRESHOLD					BIT_1	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_EMISSIONPOINT						BIT_2	// |     |     |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_TARGETPOINT							BIT_3	// |     |     |  O	 |  O  |     |     |

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// a Particle homing Component
typedef struct					//													NPARTICLE_COMPONENT_STANDARD
{								//											|		
	Nf32		Age;			// Particle Age								|	Nf32		Age;			
	Nf32		LifeSpan;		// Particle Lifespan						|	Nf32		LifeSpan;		
	Nf32		Rotation;		// Particle Current Angle					|	Nf32		Rotation;		
	Nf32		RotationSpeed;	// Particle Current Rotation (Angle) Speed	|	Nf32		RotationSpeed;	
	NVEC3	Position;		// Particle Current Position				|	NVEC3	Position;		
	Nf32		Speed;			// Particle Current speed					|	NVEC3	Speed;			
	
	Nf32		Acceleration;	// Particle Current Acceleration			
	Nf32		SplineRatio;	// Particle Current Spline Ratio				
	
	NVEC3	*pStart;
	NVEC3	LocalStart;
	NVEC3	StartTangent;

	NVEC3	*pEnd;
	NVEC3	LocalEnd;
	NVEC3	EndTangent;
}NPARTICLE_COMPONENT_HOMING;

typedef struct  
{
	Nu32							Flags;							// Description Flags

	NVEC3						DefaultEmissionPoint;			// Particles Emitter position of the Emission point (there is just One per emitter actualy )
	NVEC3						DefaultTargetPoint;				// Homing Particles "End" Position. 
	NCCS_3DCOORD					EmissionExtents;				// Particles Emitter Extents used for particles Distribution (could be Cartesian, Cylindric or Spheric !)
	NCCS_3DCOORD					TargetExtents;					// Particles Emitter Extents used for End position Distribution (could be Cartesian, Cylindric or Spheric !)
	Nf32							FromTargetDistanceThreshold;	// When a particle is close to target -> Death. ( if this param is used. cf FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_FROM_END_DIST_THRESHOLD)
	Nf32							SpeedDamping;				
	NPARTICLEPARAMS_NF32			LifeSpan;
	NPARTICLEPARAMS_NF32			BirthRotation;
	NPARTICLEPARAMS_NF32			BirthRotationSpeed;
	NPARTICLEPARAMS_NF32			BirthSpeed;
	NPARTICLEPARAMS_NF32			BirthAcceleration;				// Particle Emission Acceleration reference (before random range addition)
	NPARTICLEPARAMS_NCCS_3DCOORD	EmissionTangent;
	NPARTICLEPARAMS_NCCS_3DCOORD	TargetTangent;
	NPARTICLE_SETUP_POSITION		pParticleSetupLocalEmissionPosition;
	NPARTICLE_SETUP_POSITION		pParticleSetupLocalTargetPosition;
	NPARTICLE_SETUP_TANGENT			pParticleSetupEmissionTangent;
	NPARTICLE_SETUP_TANGENT			pParticleSetupTargetTangent;
}NPARTICLE_EMITTERCOMPONENT_HOMING_DESC;

typedef struct 
{
	Nu32							Flags;								// HOMING Flags !!!

	NVEC3						DefaultEmissionPoint;				// Particles Emitter position of the Emission point (there is just One per emitter actualy )
	NVEC3						DefaultTargetPoint;					// Homing Particles "End" Position. 
	NCCS_3DCOORD					EmissionExtents;					// Particles Emitter Extents used for particles Distribution (could be Cartesian, Cylindric or Spheric !)
	NCCS_3DCOORD					TargetExtents;						// Particles Emitter Extents used for End position Distribution (could be Cartesian, Cylindric or Spheric !)
	Nf32							FromTargetSquareDistanceThreshold;	// When a particle is close to target -> Death. ( if this param is used )
	Nf32							SpeedDamping;				
	NPARTICLEPARAMS_NF32			LifeSpan;
	NPARTICLEPARAMS_NF32			BirthRotation;
	NPARTICLEPARAMS_NF32			BirthRotationSpeed;
	NPARTICLEPARAMS_NF32			BirthSpeed;
	NPARTICLEPARAMS_NF32			BirthAcceleration;					// Particle Emission Acceleration reference (before random range addition)
	NPARTICLEPARAMS_NCCS_3DCOORD	EmissionTangent;
	NPARTICLEPARAMS_NCCS_3DCOORD	TargetTangent;
	NPARTICLE_SETUP_POSITION		pParticleSetupLocalEmissionPosition;
	NPARTICLE_SETUP_POSITION		pParticleSetupLocalTargetPosition;
	NPARTICLE_SETUP_TANGENT			pParticleSetupEmissionTangent;
	NPARTICLE_SETUP_TANGENT			pParticleSetupTargetTangent;
}NPARTICLE_EMITTERCOMPONENT_HOMING;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Setup Particle Emitter Component Desc Parameters
inline void	NSetupParticleEmitterComponent_Homing_Desc_ParticleDiesAtEnd(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_DEATH_AT_END);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_KeepReferenceOnEmissionPoint(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_EMISSIONPOINT);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_KeepReferenceOnTargetPoint(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_TARGETPOINT);}

inline void	NSetupParticleEmitterComponent_Homing_Desc_DefaultEmissionPoint(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NVEC3 *pv){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_EMISSION_POINT); pdesc->DefaultEmissionPoint =  *pv;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_DefaultEmissionPointf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 x,const Nf32 y,const Nf32 z){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_EMISSION_POINT); NVec3Set( &pdesc->DefaultEmissionPoint,x,y,z );}
inline void	NSetupParticleEmitterComponent_Homing_Desc_DefaultTargetPoint(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NVEC3 *pv){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_TARGET_POINT); pdesc->DefaultTargetPoint =  *pv;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_DefaultTargetPointf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 x,const Nf32 y,const Nf32 z){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_TARGET_POINT); NVec3Set( &pdesc->DefaultTargetPoint,x,y,z );}

inline void	NSetupParticleEmitterComponent_Homing_Desc_FromTargetDistanceThreshold(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 dist){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_FROM_TARGET_DIST_THRESHOLD); pdesc->FromTargetDistanceThreshold = dist;}


inline void	NSetupParticleEmitterComponent_Homing_Desc_CartesianEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 x,const Nf32 y,const Nf32 z){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_EXTENTS); NVec3Set( &pdesc->EmissionExtents.Cartesian,x,y,z );}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastSphericEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radius,const Nu32 inclination,const Nu32 azimuth){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_EXTENTS); NSetFastSphericf(&pdesc->EmissionExtents.FastSpheric,radius,inclination,azimuth );}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastCylindricEmissionExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radius,const Nf32 height,const Nu32 azimuth){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_EXTENTS); NSetFastCylindricf( &pdesc->EmissionExtents.FastCylindric,radius,height,azimuth );}

inline void	NSetupParticleEmitterComponent_Homing_Desc_CartesianTargetExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 x,const Nf32 y,const Nf32 z){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_EXTENTS); NVec3Set( &pdesc->TargetExtents.Cartesian,x,y,z );}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastSphericTargetExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radius,const Nu32 inclination,const Nu32 azimuth){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_EXTENTS); NSetFastSphericf(&pdesc->TargetExtents.FastSpheric,radius,inclination,azimuth );}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastCylindricTargetExtentsf(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radius,const Nf32 height,const Nu32 azimuth){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_EXTENTS); NSetFastCylindricf( &pdesc->TargetExtents.FastCylindric,radius,height,azimuth );}

inline void	NSetupParticleEmitterComponent_Homing_Desc_SpeedDamping(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 speeddamping){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SPEEDDAMPING); pdesc->SpeedDamping = speeddamping;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_LifeSpan_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 base,const Nf32 var){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_LIFESPAN_AND_VARIATION);pdesc->LifeSpan.Base = base;pdesc->LifeSpan.Variation = var;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_BirthRotation_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 base,const Nf32 var){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATION_AND_VARIATION);pdesc->BirthRotation.Base = base;pdesc->BirthRotation.Variation = var;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_BirthRotationSpeed_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 base,const Nf32 var){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION);pdesc->BirthRotationSpeed.Base = base;pdesc->BirthRotationSpeed.Variation = var;}

inline void	NSetupParticleEmitterComponent_Homing_Desc_BirthSpeed_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 base,const Nf32 var){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_SPEED_AND_VARIATION);pdesc->BirthSpeed.Base = base;pdesc->BirthSpeed.Variation = var;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_BirthAcceleration_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 base,const Nf32 var){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ACCELERATION_AND_VARIATION);pdesc->BirthAcceleration.Base = base;pdesc->BirthAcceleration.Variation = var;}

inline void	NSetupParticleEmitterComponent_Homing_Desc_CartesianEmissionTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 xbase,const Nf32 ybase,const Nf32 zbase,const Nf32 xvar,const Nf32 yvar,const Nf32 zvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_TANGENT_AND_VARIATION);NVec3Set(&pdesc->EmissionTangent.Base.Cartesian,xbase,ybase,zbase);NVec3Set(&pdesc->EmissionTangent.Variation.Cartesian,xvar,yvar,zvar);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastSphericEmissionTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radiusbase,const Nu32 inclibase,const Nu32 azimbase,const Nf32 radiusvar,const Nu32 inclivar,const Nu32 azimvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_TANGENT_AND_VARIATION);NSetFastSphericf(&pdesc->EmissionTangent.Base.FastSpheric,radiusbase,inclibase,azimbase);NSetFastSphericf(&pdesc->EmissionTangent.Variation.FastSpheric,radiusvar,inclivar,azimvar);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastCylindricEmissionTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radiusbase,const Nf32 hbase,const Nu32 azimbase,const Nf32 radiusvar,const Nf32 hvar,const Nu32 azimvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_TANGENT_AND_VARIATION);NSetFastCylindricf(&pdesc->EmissionTangent.Base.FastCylindric,radiusbase,hbase,azimbase);NSetFastCylindricf(&pdesc->EmissionTangent.Variation.FastCylindric,radiusvar,hvar,azimvar);}

inline void	NSetupParticleEmitterComponent_Homing_Desc_CartesianTargetTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 xbase,const Nf32 ybase,const Nf32 zbase,const Nf32 xvar,const Nf32 yvar,const Nf32 zvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_TANGENT_AND_VARIATION);NVec3Set(&pdesc->TargetTangent.Base.Cartesian,xbase,ybase,zbase);NVec3Set(&pdesc->TargetTangent.Variation.Cartesian,xvar,yvar,zvar);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastSphericTargetTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radiusbase,const Nu32 inclibase,const Nu32 azimbase,const Nf32 radiusvar,const Nu32 inclivar,const Nu32 azimvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_TANGENT_AND_VARIATION);NSetFastSphericf(&pdesc->TargetTangent.Base.FastSpheric,radiusbase,inclibase,azimbase);NSetFastSphericf(&pdesc->TargetTangent.Variation.FastSpheric,radiusvar,inclivar,azimvar);}
inline void	NSetupParticleEmitterComponent_Homing_Desc_FastCylindricTargetTangentf_Params(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const Nf32 radiusbase,const Nf32 hbase,const Nu32 azimbase,const Nf32 radiusvar,const Nf32 hvar,const Nu32 azimvar){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_TANGENT_AND_VARIATION);NSetFastCylindricf(&pdesc->TargetTangent.Base.FastCylindric,radiusbase,hbase,azimbase);NSetFastCylindricf(&pdesc->TargetTangent.Variation.FastCylindric,radiusvar,hvar,azimvar);}

inline void	NSetupParticleEmitterComponent_Homing_Desc_LocalEmissionPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NPARTICLE_SETUP_POSITION phandle){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALEMISSIONPOSITION_HANDLE);pdesc->pParticleSetupLocalEmissionPosition = phandle;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_LocalTargetPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NPARTICLE_SETUP_POSITION phandle){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALTARGETPOSITION_HANDLE);pdesc->pParticleSetupLocalTargetPosition = phandle;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_EmissionTangent_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NPARTICLE_SETUP_TANGENT phandle){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_EMISSIONTANGENT_HANDLE);pdesc->pParticleSetupEmissionTangent = phandle;}
inline void	NSetupParticleEmitterComponent_Homing_Desc_TargetTangent_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc,const NPARTICLE_SETUP_TANGENT phandle){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_TARGETTANGENT_HANDLE);pdesc->pParticleSetupTargetTangent = phandle;}

// Change Particle EmitterComponent Parameters
inline void	NSetParticleEmitterComponent_Homing_DefaultEmissionPoint(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NVEC3 *pv){pemitter_homcomponent->DefaultEmissionPoint=*pv;}
inline void	NSetParticleEmitterComponent_Homing_DefaultEmissionPointf(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const Nf32 x,const Nf32 y,const Nf32 z){NVec3Set( &pemitter_homcomponent->DefaultEmissionPoint,x,y,z );}
inline void	NSetParticleEmitterComponent_Homing_CCSEmissionExtents(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->EmissionExtents = *pccs;}
inline void	NSetParticleEmitterComponent_Homing_DefaultTargetPoint(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NVEC3 *pv){pemitter_homcomponent->DefaultTargetPoint=*pv;}
inline void	NSetParticleEmitterComponent_Homing_DefaultTargetPointf(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const Nf32 x,const Nf32 y,const Nf32 z){NVec3Set( &pemitter_homcomponent->DefaultTargetPoint,x,y,z );}
inline void	NSetParticleEmitterComponent_Homing_CCSTargetExtents(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->TargetExtents = *pccs;}

inline void	NSetParticleEmitterComponent_Homing_SpeedDamping(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 speed_damping){pemitter_homcomponent->SpeedDamping = speed_damping;}
inline void	NSetParticleEmitterComponent_Homing_LifeSpanBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 lifespanbase){pemitter_homcomponent->LifeSpan.Base = lifespanbase;}
inline void	NSetParticleEmitterComponent_Homing_LifeSpanVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 *plifespanvar){pemitter_homcomponent->LifeSpan.Variation = *plifespanvar;}
inline void	NSetParticleEmitterComponent_Homing_BirthRotationBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 rotation){pemitter_homcomponent->BirthRotation.Base = rotation;}
inline void	NSetParticleEmitterComponent_Homing_BirthRotationVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 rotation_var){pemitter_homcomponent->BirthRotation.Variation = rotation_var;}
inline void	NSetParticleEmitterComponent_Homing_BirthRotationSpeedBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 rspeed_base){pemitter_homcomponent->BirthRotationSpeed.Base = rspeed_base;}
inline void	NSetParticleEmitterComponent_Homing_BirthRotationSpeedVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 rspeed_var){pemitter_homcomponent->BirthRotationSpeed.Variation = rspeed_var;}
inline void	NSetParticleEmitterComponent_Homing_BirthSpeedBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 pbirthspeed_base){pemitter_homcomponent->BirthSpeed.Base = pbirthspeed_base;}
inline void	NSetParticleEmitterComponent_Homing_BirthSpeedVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 pbirthspeed_var){pemitter_homcomponent->BirthSpeed.Variation = pbirthspeed_var;}
inline void	NSetParticleEmitterComponent_Homing_BirthAccelerationBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 pbirthacc_base){pemitter_homcomponent->BirthAcceleration.Base = pbirthacc_base;}
inline void	NSetParticleEmitterComponent_Homing_BirthAccelerationVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent, const Nf32 pbirthacc_var){pemitter_homcomponent->BirthAcceleration.Variation = pbirthacc_var;}
inline void	NSetParticleEmitterComponent_Homing_CCSEmissionTangentBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->EmissionTangent.Base = *pccs;}
inline void	NSetParticleEmitterComponent_Homing_CCSEmissionTangentVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->EmissionTangent.Variation = *pccs;}
inline void	NSetParticleEmitterComponent_Homing_CCSTargetTangentBase(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->TargetTangent.Base = *pccs;}
inline void	NSetParticleEmitterComponent_Homing_CCSTargetTangentVariation(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NCCS_3DCOORD *pccs){pemitter_homcomponent->TargetTangent.Variation = *pccs;}
// Change Particle EmitterComponent Homing Parameters:Handles
inline void	NSetParticleEmitterComponent_Homing_LocalEmissionPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NPARTICLE_SETUP_POSITION phandle){pemitter_homcomponent->pParticleSetupLocalEmissionPosition = phandle;}
inline void	NSetParticleEmitterComponent_Homing_LocalTargetPosition_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NPARTICLE_SETUP_POSITION phandle){pemitter_homcomponent->pParticleSetupLocalTargetPosition = phandle;}
inline void	NSetParticleEmitterComponent_Homing_EmissionTangent_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NPARTICLE_SETUP_TANGENT phandle){pemitter_homcomponent->pParticleSetupEmissionTangent = phandle;}
inline void	NSetParticleEmitterComponent_Homing_TargetTangent_SetupHandle(NPARTICLE_EMITTERCOMPONENT_HOMING *pemitter_homcomponent,const NPARTICLE_SETUP_TANGENT phandle){pemitter_homcomponent->pParticleSetupTargetTangent = phandle;}

// ***************************************************************************************
// **				LOW LEVEL:PRIVATE Inline Setup & Clear Functions					**
// ***************************************************************************************
inline void	NSetupParticleComponent_Homing( NPARTICLE_COMPONENT_HOMING *pparticlecomponent, const NPARTICLE_EMITTERCOMPONENT_HOMING *pemittercomponent, const NVEC3 *pemissionpoint, const NVEC3 *ptargetpoint )
{
	pparticlecomponent->Age				= 0.0f;
	pparticlecomponent->SplineRatio		= 0.0f;
	pparticlecomponent->LifeSpan		= NFastRandEx( pemittercomponent->LifeSpan.Base,pemittercomponent->LifeSpan.Variation );
	pparticlecomponent->Rotation		= pemittercomponent->BirthRotation.Base + pemittercomponent->BirthRotation.Variation*NFastRand();
	pparticlecomponent->RotationSpeed	= NFastRandEx( pemittercomponent->BirthRotationSpeed.Base,pemittercomponent->BirthRotationSpeed.Variation );
	pparticlecomponent->Acceleration	= NFastRandEx( pemittercomponent->BirthAcceleration.Base,pemittercomponent->BirthAcceleration.Variation );
	pparticlecomponent->Speed			= NFastRandEx( pemittercomponent->BirthSpeed.Base,pemittercomponent->BirthSpeed.Variation );

	// START POSITION
	if( ISFLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_EMISSIONPOINT) )
	{
		pparticlecomponent->pStart	= (NVEC3*)pemissionpoint;
		NVec3Set(&pparticlecomponent->LocalStart,0,0,0);
	}
	else
	{
		pparticlecomponent->pStart		= NULL;
		pparticlecomponent->LocalStart	= *pemissionpoint;
	}
	pemittercomponent->pParticleSetupLocalEmissionPosition(&pparticlecomponent->LocalStart,&pemittercomponent->EmissionExtents,&pparticlecomponent->LocalStart);

	// END POSITION
	if( ISFLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_TARGETPOINT) )
	{
		pparticlecomponent->pEnd	= (NVEC3*)ptargetpoint;
		NVec3Set(&pparticlecomponent->LocalEnd,0,0,0);
	}
	else
	{
		pparticlecomponent->pEnd = NULL;
		pparticlecomponent->LocalEnd = *ptargetpoint;
	}
	pemittercomponent->pParticleSetupLocalTargetPosition(&pparticlecomponent->LocalEnd,&pemittercomponent->TargetExtents,&pparticlecomponent->LocalEnd);

	// EMISSION & TARGET TANGENT
	pemittercomponent->pParticleSetupEmissionTangent(&pparticlecomponent->StartTangent,&pemittercomponent->EmissionTangent);
	pemittercomponent->pParticleSetupTargetTangent(&pparticlecomponent->EndTangent,&pemittercomponent->TargetTangent);
}

inline void	NClearParticleComponent_Homing( NPARTICLE_COMPONENT_HOMING *pparticlecomponent )
{
	Nmem0(pparticlecomponent,NPARTICLE_COMPONENT_HOMING);
}

inline void NSetupParticlesEmitterComponent_Homing( NPARTICLE_EMITTERCOMPONENT_HOMING *pemittercomponent, const NPARTICLE_EMITTERCOMPONENT_HOMING_DESC *pdesc )
{
	// 	Nu32						Flags;					
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_DEATH_AT_END) )
	{
		FLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_AT_END);
	}
	else
	{
		FLAG_OFF(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_AT_END);
	}
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_EMISSIONPOINT) )
	{
		FLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_EMISSIONPOINT);
	}
	else
	{
		FLAG_OFF(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_EMISSIONPOINT);
	}
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_KEEP_REF_ON_TARGETPOINT) )
	{
		FLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_TARGETPOINT);
	}
	else
	{
		FLAG_OFF(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_KEEP_REF_ON_TARGETPOINT);
	}

	//NVEC3						DefaultEmissionPoint;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_EMISSION_POINT) )
	{
		pemittercomponent->DefaultEmissionPoint	= pdesc->DefaultEmissionPoint;
	}
	else
	{
		NVec3Set(&pemittercomponent->DefaultEmissionPoint, DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_POINT_3F );
	}
	//NVEC3						DefaultTargetPoint;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_DEFAULT_TARGET_POINT) )
	{
		pemittercomponent->DefaultTargetPoint	= pdesc->DefaultTargetPoint;
	}
	else
	{
		NVec3Set(&pemittercomponent->DefaultTargetPoint, DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_POINT_3F );
	}
	//NCCS_3DCOORD					EmissionExtents;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_EXTENTS) )
	{
		pemittercomponent->EmissionExtents	= pdesc->EmissionExtents;
	}
	else
	{
		NVec3Set(&pemittercomponent->EmissionExtents.Cartesian, DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_EXTENTS_3F );
	}
	//NCCS_3DCOORD					TargetExtents;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_EXTENTS) )
	{
		pemittercomponent->TargetExtents	= pdesc->TargetExtents;
	}
	else
	{
		NVec3Set(&pemittercomponent->TargetExtents.Cartesian, DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_EXTENTS_3F );
	}
	//Nf32							FromTargetDistanceThreshold;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_FROM_TARGET_DIST_THRESHOLD) )
	{
		pemittercomponent->FromTargetSquareDistanceThreshold = pdesc->FromTargetDistanceThreshold*pdesc->FromTargetDistanceThreshold;
		FLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_FROM_TARGET_DIST_THRESHOLD);
	}
	else
	{
		pemittercomponent->FromTargetSquareDistanceThreshold = DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_FROM_TARGET_SQUARE_DIST_THRESHOLD;
		FLAG_OFF(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_FROM_TARGET_DIST_THRESHOLD);
	}
	//Nf32							SpeedDamping;				
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SPEEDDAMPING) )
	{
		pemittercomponent->SpeedDamping = pdesc->SpeedDamping;
	}
	else
	{
		pemittercomponent->SpeedDamping = DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_SPEEDDAMPING;
	}
	//NPARTICLEPARAMS_NF32			LifeSpan;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_LIFESPAN_AND_VARIATION) )
	{
		pemittercomponent->LifeSpan = pdesc->LifeSpan;
	}
	else
	{
		pemittercomponent->LifeSpan.Base		= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_LIFESPAN_BASE;
		pemittercomponent->LifeSpan.Variation	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_LIFESPAN_VARIATION;
	}
	//	NPARTICLEPARAMS_NF32			BirthRotation;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATION_AND_VARIATION) )
	{
		pemittercomponent->BirthRotation = pdesc->BirthRotation;
	}
	else
	{
		pemittercomponent->BirthRotation.Base		= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_BASE;
		pemittercomponent->BirthRotation.Variation	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_VARIATION;
	}
	//NPARTICLEPARAMS_NF32			BirthRotationSpeed;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ROTATIONSPEED_AND_VARIATION) )
	{
		pemittercomponent->BirthRotationSpeed = pdesc->BirthRotationSpeed;
	}
	else
	{
		pemittercomponent->BirthRotationSpeed.Base		= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_SPEED_BASE;
		pemittercomponent->BirthRotationSpeed.Variation	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ROTATION_SPEED_VARIATION;
	}
	//	NPARTICLEPARAMS_NF32		BirthSpeed;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_SPEED_AND_VARIATION) )
	{
		pemittercomponent->BirthSpeed = pdesc->BirthSpeed;
	}
	else
	{
		pemittercomponent->BirthSpeed.Base		= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_SPEED_BASE;
		pemittercomponent->BirthSpeed.Variation	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_SPEED_VARIATION;
	}
	//NPARTICLEPARAMS_NF32			BirthAcceleration;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_BIRTH_ACCELERATION_AND_VARIATION) )
	{
		pemittercomponent->BirthAcceleration = pdesc->BirthAcceleration;
	}
	else
	{
		pemittercomponent->BirthAcceleration.Base		= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ACCELERATION_BASE;
		pemittercomponent->BirthAcceleration.Variation	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_BIRTH_ACCELERATION_VARIATION;
	}
	//NPARTICLEPARAMS_NCCS_3DCOORD	EmissionTangent;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_EMISSION_TANGENT_AND_VARIATION) )
	{
		pemittercomponent->EmissionTangent	= pdesc->EmissionTangent;
	}
	else
	{
		NVec3Set(&pemittercomponent->EmissionTangent.Base.Cartesian,DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_TANGENT_BASE_3F );
		NVec3Set(&pemittercomponent->EmissionTangent.Variation.Cartesian,DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_EMISSION_TANGENT_VARIATION_3F );
	}
	//NPARTICLEPARAMS_NCCS_3DCOORD	TargetTangent;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_TARGET_TANGENT_AND_VARIATION) )
	{
		pemittercomponent->TargetTangent = pdesc->TargetTangent;
	}
	else
	{
		NVec3Set(&pemittercomponent->TargetTangent.Base.Cartesian,DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_TANGENT_BASE_3F );
		NVec3Set(&pemittercomponent->TargetTangent.Variation.Cartesian,DEFAULT_NPARTICLE_EMITTERCOMPONENT_HOMING_TARGET_TANGENT_VARIATION_3F );
	}

	//	NPARTICLE_SETUP_POSITION		pParticleSetupEmissionPosition;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALEMISSIONPOSITION_HANDLE) )
	{
		pemittercomponent->pParticleSetupLocalEmissionPosition = pdesc->pParticleSetupLocalEmissionPosition;
	}
	else
	{
		pemittercomponent->pParticleSetupLocalEmissionPosition = DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_EMISSIONPOSITION_HANDLE;
	}
	//NPARTICLE_SETUP_POSITION			pParticleSetupLocalTargetPosition;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_LOCALTARGETPOSITION_HANDLE) )
	{
		pemittercomponent->pParticleSetupLocalTargetPosition = pdesc->pParticleSetupLocalTargetPosition;
	}
	else
	{
		pemittercomponent->pParticleSetupLocalTargetPosition = DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_TARGETPOSITION_HANDLE;
	}
	//NPARTICLE_SETUP_TANGENT			pParticleSetupEmissionTangent;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_EMISSIONTANGENT_HANDLE) )
	{
		pemittercomponent->pParticleSetupEmissionTangent = pdesc->pParticleSetupEmissionTangent;
	}
	else
	{
		pemittercomponent->pParticleSetupEmissionTangent = DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_EMISSIONTANGENT_HANDLE;
	}
	//NPARTICLE_SETUP_TANGENT			pParticleSetupTargetTangent;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DESC_USE_SETUP_TARGETTANGENT_HANDLE) )
	{
		pemittercomponent->pParticleSetupTargetTangent = pdesc->pParticleSetupTargetTangent;
	}
	else
	{
		pemittercomponent->pParticleSetupTargetTangent = DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_TARGETTANGENT_HANDLE;
	}
}

inline void NClearParticlesEmitterComponent_Homing( NPARTICLE_EMITTERCOMPONENT_HOMING *pemittercomponent )
{
	Nmem0(pemittercomponent,NPARTICLE_EMITTERCOMPONENT_HOMING);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NPARTICLECOMPONENT_HOMING_H 

