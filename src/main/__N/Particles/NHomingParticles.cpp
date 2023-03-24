#include "../NCStandard.h"
#include "../NType.h"
#include "../GL/Ngl.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Render/NFrustum_Culling.h"
#include "../NFastRandExtend.h"
//#include "../Utilities/NUT_Draw.h"
#include "../NCore.h"
#include "../Utilities/Maths/NUT_MathsMisc.h"
#include "../NSpline.h"
#include "NHomingParticles.h"


// ------------------------------------------------------------------------------------------
// NCreateHomingParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Create a Homing Particles emitter
// ------------------------------------------------------------------------------------------
// In  :
//		pemitterlist	:	Th list where the emitter will be inserted in. ( can be NULL )
//		emitter_position:	ptr on a vector with the location of the emitter
//		particlenbmax	:	max number of particles usable at the same time by this emitter
//		ptexture		:	valid ptr on the texture used by particles to render.
// Out :
//		ptr on the new emitter
// ------------------------------------------------------------------------------------------
NHOMINGPARTICLES_EMITTER* NCreateHomingParticlesEmitter( const NHOMINGPARTICLES_EMITTER_DESC *pdesc )
{
	return NSetUpHomingParticlesEmitter(NEW(NHOMINGPARTICLES_EMITTER),pdesc);
}

NHOMINGPARTICLES_EMITTER* NSetUpHomingParticlesEmitter( NHOMINGPARTICLES_EMITTER *pemitter, const NHOMINGPARTICLES_EMITTER_DESC *pdesc )
{
	NSetupParticlesEmitterComponent_Core(&pemitter->Core,&pdesc->CoreDesc,sizeof(NHOMINGPARTICLE),NRenderableUpdate_HomingParticlesEmitter);
	NSetupParticlesEmitterComponent_LUT(&pemitter->Lut,&pdesc->LutDesc);
	NSetupParticlesEmitterComponent_Homing(&pemitter->Homing,&pdesc->HomingDesc);
	return pemitter;
}

// ------------------------------------------------------------------------------------------
// NDeleteHomingParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Delete a Particles emitter
// ------------------------------------------------------------------------------------------
// In  :
//		pemitter: ptr on the emitter to delete
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
void NDeleteHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER* pemitter)
{
	NClearHomingParticlesEmitter(pemitter);
	Nfree(pemitter);
}

void NClearHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER* pemitter)
{
	NClearParticlesEmitterComponent_Homing(&pemitter->Homing);
	NClearParticlesEmitterComponent_LUT(&pemitter->Lut);
	NClearParticlesEmitterComponent_Core(&pemitter->Core);
}

// ------------------------------------------------------------------------------------------
// NUpdateHomingParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Update the particle emitter ( used into the N_Update() )
// ------------------------------------------------------------------------------------------
// In  :
//		pemitter: ptr on the emitter to update
//		ptime: ptr on a time structure which contains all time informations
// Out :
//		
// ------------------------------------------------------------------------------------------
void NRenderableUpdate_HomingParticlesEmitter(NRENDERABLE *prenderable, void* powner, const NTIME *ptime)
{
	// =========================================================================
	// EXTRACT (and return if no extraction ... )
	if( !prenderable->Extract_FCT( prenderable, powner )||ISFLAG_ON( NEngineCoreFlags,FLAG_NENGINE_CORE_PAUSE_PARTICLES ) )
	{
		return;
	}
	// =========================================================================
	NHOMINGPARTICLES_EMITTER	*pemitter = (NHOMINGPARTICLES_EMITTER *)powner;
	NPARTICLELUT		*plut;
	NHOMINGPARTICLE		*pparticleb;
	NVEC3			*pcenter;
	NVEC3			up,right;
	NVEC3			right_plus_up,right_plus_up2;
	NVEC3			A,B,At,Bt,D;
	Nf32				co,si;
	Nu32				i;
// 	Nu32				vertex		= 0;
	Nf32				fdelay		= ptime->Nf32_Delay;
	NHOMINGPARTICLE		*pparticle	= (NHOMINGPARTICLE*)pemitter->Core.pParticlesList;
	NGEOMETRY			*pgeom		= NGetFirstIncludedGeometry(pemitter->Core.pRenderable);
	NPARTICLEVERTEX		*pvertex	= (NPARTICLEVERTEX*)NGetFirstMeshVertexPtr(&pgeom->Mesh);

	NUT_ExtractUpSideFromModelViewMatrix(&up,&right);

	// =========================================================================
	// Calculate the number of new particles to be awake during this update loop
	// =========================================================================
	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
		NApplyBirthRate(&pemitter->Core,fdelay);

	// =====================
	// Particles Update Loop
	// =====================
	for(i=0;i<pemitter->Core.ParticlesNb;i++, pparticle ++)
	{
		pparticle->Homing.Age += fdelay;
		// ==============
		// Alive Particle
		// ==============
		if( pparticle->Homing.Age < pparticle->Homing.LifeSpan )
		{
			// Update Speed
			pparticle->Homing.Speed			= pparticle->Homing.Speed*pemitter->Homing.SpeedDamping + pparticle->Homing.Acceleration*fdelay;
			pparticle->Homing.SplineRatio  += pparticle->Homing.Speed*fdelay;
			
			if( pparticle->Homing.SplineRatio >= 1.0f )
			{
				pparticle->Homing.SplineRatio = 1.0f;
				if( ISFLAG_ON(pemitter->Homing.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_AT_END) )
					pparticle->Homing.Age = pparticle->Homing.LifeSpan;
			}

			// ...  And position
			if( !pparticle->Homing.pStart )
			{
				A = pparticle->Homing.LocalStart;
			}
			else
			{
				A.x = pparticle->Homing.pStart->x + pparticle->Homing.LocalStart.x;
				A.y = pparticle->Homing.pStart->y + pparticle->Homing.LocalStart.y;
				A.z = pparticle->Homing.pStart->z + pparticle->Homing.LocalStart.z;
			}

			if( !pparticle->Homing.pEnd )		
			{
				B = pparticle->Homing.LocalEnd;
			}
			else
			{
				B.x = pparticle->Homing.pEnd->x + pparticle->Homing.LocalEnd.x;
				B.y = pparticle->Homing.pEnd->y + pparticle->Homing.LocalEnd.y;
				B.z = pparticle->Homing.pEnd->z + pparticle->Homing.LocalEnd.z;
			}
			At.x = A.x + pparticle->Homing.StartTangent.x;
			At.y = A.y + pparticle->Homing.StartTangent.y;
			At.z = A.z + pparticle->Homing.StartTangent.z;
			
			Bt.x = B.x + pparticle->Homing.EndTangent.x;
			Bt.y = B.y + pparticle->Homing.EndTangent.y;
			Bt.z = B.z + pparticle->Homing.EndTangent.z;

			NBezierXtdPoint3f32(&pparticle->Homing.Position,pparticle->Homing.SplineRatio, &A,&At,&Bt,&B);			
			// From End Distance threshold for Death ? ( if particle is too close from path End, Engine considers particle  
			if( ISFLAG_ON(pemitter->Homing.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_FROM_TARGET_DIST_THRESHOLD) )
			{
				D.x = B.x - pparticle->Homing.Position.x;
				D.y = B.y - pparticle->Homing.Position.y;
				D.z = B.z - pparticle->Homing.Position.z;
				if( NVec3SquareLength(&D) <= pemitter->Homing.FromTargetSquareDistanceThreshold )
					pparticle->Homing.Age = pparticle->Homing.LifeSpan; // Particle will be considered as DEAD to the next update
			}
			
			// ... And Rotation
			pparticle->Homing.Rotation += pparticle->Homing.RotationSpeed*fdelay;

			// Build particle mesh
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// !!! OPTIM !!!
			// For this optimized version we consider particle as SQUARE only.
			// "NPARTICLES_EMITTER.BirthExtents" Still unused and and LUT.Scale is used as half square Diagonal Size ! 
			// Because the optimization consist to build the square from these diagonals which are orthogonal and save
			// several multiplication and addition of NVEC3 ...
			//		
			//			 b	
			//		   	 +
			//		   .   .
			//		 .       . 
			//	  a +    +    + c   with the 2 orthogonal diagonals. 
			//		 .	     .
			//		   .   .	
			//			 +
			//			 d	
			//
			plut = &pemitter->Lut.pLUT[ (Nu16)( (pparticle->Homing.Age/pparticle->Homing.LifeSpan)*(Nf32)(pemitter->Lut.LUT_Size-1) ) ] ;

			NFastCosSin((Nu32)pparticle->Homing.Rotation,&co,&si);
			co *= plut->Scale;
			si *= plut->Scale;
			right_plus_up.x = co*right.x + si*up.x;
			right_plus_up.y = co*right.y + si*up.y;
			right_plus_up.z = co*right.z + si*up.z;
			// notes: cos(a+pi/2) = -sin(a) AND sin(a+pi/2) = cos(a). So ...
			right_plus_up2.x = co*up.x - si*right.x;
			right_plus_up2.y = co*up.y - si*right.y;
			right_plus_up2.z = co*up.z - si*right.z;

			pcenter = &pparticle->Homing.Position;
			NVec3Sub((NVEC3*)pvertex,pcenter,&right_plus_up);		// vertex a
			pvertex->Color0_4f = plut->Color;
			pvertex++;
			NVec3Add((NVEC3*)pvertex,pcenter,&right_plus_up2);	// vertex b
			pvertex->Color0_4f = plut->Color;
			pvertex++;
			NVec3Add((NVEC3*)pvertex,pcenter,&right_plus_up);		// vertex c
			pvertex->Color0_4f = plut->Color;
			pvertex++;
			NVec3Sub((NVEC3*)pvertex,pcenter,&right_plus_up2);	// vertex d
			pvertex->Color0_4f = plut->Color;
			pvertex++;
		}
		// =============
		// Dead Particle
		// =============
		else
		{
			// Replace Dead Particle by the Current Last Alive Particle
			// ---------------------------------------------------------
			pparticleb = &((NHOMINGPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb-1];
			memcpy( pparticle, pparticleb,sizeof(NHOMINGPARTICLE));	
			//  IN case of AUTO EMISSION ...  Initialize rejected particle with its specific birth param.
			if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
			{
				NSetupParticleComponent_Homing(&pparticleb->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
			}
			// Because the last alive particle has replaced the dead one, we have to make one step back to update it
			pparticle--;i--;							
			pemitter->Core.ParticlesNb--;										
		}
	}
	// At the End
	pgeom->Mesh.PrimitiveArray.Size = pemitter->Core.ParticlesNb * 2; // x 2 because 2 triangles for each particle
}
// Refresh just one time after "all" ParticlesEmitter params changes ...
void NRefreshPrecalculatedHomingParticles(NHOMINGPARTICLES_EMITTER *pemitter)
{
	// No needs of refreshing in case of lake of "All Used" particles or AUTO EMISSION TURN OFF !
	if( ISFLAG_OFF(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) || pemitter->Core.ParticlesNb == pemitter->Core.ParticlesNbMax )
	{
		return;
	}
	else
	{
		NHOMINGPARTICLE	*pparticle = &((NHOMINGPARTICLE*)(pemitter->Core.pParticlesList))[pemitter->Core.ParticlesNb]; // First asleep particle
		for(Nu16 i = (pemitter->Core.ParticlesNbMax-pemitter->Core.ParticlesNb);i!=0;i--,pparticle++)
		{
			NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
		}
	}
}

void NEmitHomingParticles(NHOMINGPARTICLES_EMITTER *pemitter,const Nu16 nbparticles,const NVEC3 *pstart,const NVEC3 *pend)
{
	Nu16		i;
	Nu16		max;
	NVEC3	*pemission,*ptarget;

	if( (pemitter->Core.ParticlesNb + nbparticles) > pemitter->Core.ParticlesNbMax )
		max = pemitter->Core.ParticlesNbMax - pemitter->Core.ParticlesNb;
	else
		max = nbparticles;

	if(pstart)
		pemission = (NVEC3*)pstart;
	else
		pemission = &pemitter->Homing.DefaultEmissionPoint;
	
	if(pend)
		ptarget = (NVEC3*)pend;
	else
		ptarget = &pemitter->Homing.DefaultTargetPoint;

	// !!! It may be out of range ! (when pemitter->ParticlesNb == pemitter->ParticlesNbMax) 
	// But it doesn't matter, we are going to use it and write in memory
	// ONLY if its not beyond limit, thanks to "max" value tested some lines bellow.
	NHOMINGPARTICLE	*pparticle = &((NHOMINGPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb];
	for(i=max;i!=0;i--,pparticle++)											
	{																				
		NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,pemission,ptarget);			
	}

	pemitter->Core.ParticlesNb += max;
}

void NResetHomingParticlesEmitter(NHOMINGPARTICLES_EMITTER *pemitter)
{

	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
	{
		// Reset and pre-calculate all 'Alive' particles for next 'awaking'
		NHOMINGPARTICLE *pparticle = (NHOMINGPARTICLE*)pemitter->Core.pParticlesList;
		for(Nu16 i=pemitter->Core.ParticlesNb;i!=0;i--,pparticle++)								
		{														
			NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
		}
	}

	pemitter->Core.ParticlesNb = 0;
}