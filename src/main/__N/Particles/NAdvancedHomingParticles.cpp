#include "../NCStandard.h"
#include "../NType.h"
#include "../GL/Ngl.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Render/NFrustum_Culling.h"
#include "../NFastRandExtend.h"
#include "../Utilities/Maths/NUT_MathsMisc.h"
#include "../NSpline.h"
//#include "../Utilities/NUT_Draw.h"
#include "../NCore.h"

#include "NAdvancedHomingParticles.h"

// ------------------------------------------------------------------------------------------
// NCreateAdvancedHomingParticlesEmitter
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

NADVANCEDHOMINGPARTICLES_EMITTER* NCreateAdvancedHomingParticlesEmitter( const NADVANCEDHOMINGPARTICLES_EMITTER_DESC *pdesc )
{
	return NSetUpAdvancedHomingParticlesEmitter(NEW(NADVANCEDHOMINGPARTICLES_EMITTER),pdesc);
}

NADVANCEDHOMINGPARTICLES_EMITTER* NSetUpAdvancedHomingParticlesEmitter( NADVANCEDHOMINGPARTICLES_EMITTER *pemitter, const NADVANCEDHOMINGPARTICLES_EMITTER_DESC *pdesc )
{

	NSetupParticlesEmitterComponent_Core(&pemitter->Core,&pdesc->CoreDesc,sizeof(NADVANCEDHOMINGPARTICLE),NRenderableUpdate_AdvancedHomingParticlesEmitter);
	NSetupParticlesEmitterComponent_LUT(&pemitter->Lut,&pdesc->LutDesc);
	NSetupParticlesEmitterComponent_Homing(&pemitter->Homing,&pdesc->HomingDesc);
	NSetupParticlesEmitterComponent_Advanced(&pemitter->Advanced,&pdesc->AdvancedDesc);
	return pemitter;
}


// ------------------------------------------------------------------------------------------
// NDeleteAdvancedHomingParticlesEmitter
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
void NDeleteAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER* pemitter)
{
	NClearAdvancedHomingParticlesEmitter(pemitter);
	Nfree(pemitter);
}
void NClearAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER* pemitter)
{
	NClearParticlesEmitterComponent_Advanced(&pemitter->Advanced);
	NClearParticlesEmitterComponent_Homing(&pemitter->Homing);
	NClearParticlesEmitterComponent_LUT(&pemitter->Lut);
	NClearParticlesEmitterComponent_Core(&pemitter->Core);
}

// ------------------------------------------------------------------------------------------
// NRenderableUpdate_AdvancedHomingParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Update the particle emitter ( used into the N_Update() )
//	... And extract it's unique geometry to the right Accumulator !
// ------------------------------------------------------------------------------------------
// In  :
//		pemitter: ptr on the emitter to update
//		ptime: ptr on a time structure which contains all time informations
// Out :
//		
// ------------------------------------------------------------------------------------------
void NRenderableUpdate_AdvancedHomingParticlesEmitter(NRENDERABLE *prenderable, void* powner, const NTIME *ptime)
{
	// =========================================================================
	// EXTRACT (and return if no extraction ... )
	if( !prenderable->Extract_FCT( prenderable, powner )||ISFLAG_ON( NEngineCoreFlags,FLAG_NENGINE_CORE_PAUSE_PARTICLES ) )
	{
		return;
	}
	// =========================================================================
	NADVANCEDHOMINGPARTICLES_EMITTER		*pemitter = (NADVANCEDHOMINGPARTICLES_EMITTER*)powner;		
	NPARTICLELUT				*plut;
	NADVANCEDHOMINGPARTICLE		*pparticleb;
	NVEC3					*pcenter;
	NVEC3					up,right;
	NVEC3					right_plus_up,right_minus_up;
	NVEC3					At,Bt;
	NVEC3					A,B,D;
	NVEC3					i_rot,j_rot;
	Nf32						co,si,coa,sia;
	Nu32						i;
//	Nu32						vertex		= 0;
	Nf32						fdelay		= ptime->Nf32_Delay;
	NADVANCEDHOMINGPARTICLE		*pparticle	= (NADVANCEDHOMINGPARTICLE*)pemitter->Core.pParticlesList;
	NGEOMETRY					*pgeom		= NGetFirstIncludedGeometry(prenderable);
	NPARTICLEVERTEX				*pvertex	= (NPARTICLEVERTEX*)NGetFirstMeshVertexPtr(&pgeom->Mesh);
	
	// =========================================================================
	// Calculate the number of new particles to be awake during this update loop
	// =========================================================================
	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
		NApplyBirthRate(&pemitter->Core,fdelay);

	// =====================
	// Particles Update Loop
	// =====================
	NUT_ExtractUpSideFromModelViewMatrix(&up,&right);
	for( i=0;i<pemitter->Core.ParticlesNb;i++,pvertex+=4,pparticle++ )
	{
		pparticle->Homing.Age += fdelay;
		// ==============
		// Alive Particle
		// ==============
		if( pparticle->Homing.Age < pparticle->Homing.LifeSpan )
		{
			// Update Speed ...
			pparticle->Homing.Speed			+= pparticle->Homing.Acceleration*fdelay;
			pparticle->Homing.SplineRatio	+= pparticle->Homing.Speed*fdelay;
			if( pparticle->Homing.SplineRatio >= 1.0f )
			{
				pparticle->Homing.SplineRatio = 1.0f;
				if( ISFLAG_ON(pemitter->Homing.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_HOMING_DEATH_AT_END) )
					pparticle->Homing.Age = pparticle->Homing.LifeSpan;// Particle will be considered as DEAD to the next update
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

			// Calculate Rotation
			pparticle->Homing.Rotation += pparticle->Homing.RotationSpeed*fdelay;

			// CALLBACK
			if(pemitter->Advanced.pAdvancedParticle_UpdateCallBack)
				pemitter->Advanced.pAdvancedParticle_UpdateCallBack((const void*)pparticle,(const void*)pemitter);
			
			// Build particle mesh
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// For Advanced version particle can be rectangle and the PARTICLE.BirthExtents param
			// will be used.
			//
			//		d---------c
			//		|		  ^
			//		|         |
			//		|    x---->
			//		| center  |
			//		|         |
			//		a ------- b
			//
			//	vertex a = center - (right + up);
			//	vertex b = center + (right - up);
			//	vertex c = center + (right + up);
			//	vertex d = center - (right - up);
			plut = &pemitter->Lut.pLUT[ (Nu16)( (pparticle->Homing.Age/pparticle->Homing.LifeSpan)*(Nf32)(pemitter->Lut.LUT_Size-1) ) ] ;

			NFastCosSin((Nu32)pparticle->Homing.Rotation,&co,&si);
			co *= plut->Scale;
			si *= plut->Scale;

			coa = co*pparticle->Advanced.Extents.x;
			sia = si*pparticle->Advanced.Extents.x; 
			i_rot.x = coa*right.x + sia*up.x;
			i_rot.y = coa*right.y + sia*up.y;
			i_rot.z = coa*right.z + sia*up.z;

			// Notes: cos(a+pi/2) = -sin(a) AND  sin(a+pi/2) = cos(a)
			coa = -si*pparticle->Advanced.Extents.y;
			sia =  co*pparticle->Advanced.Extents.y;
			j_rot.x = coa*right.x + sia*up.x;				
			j_rot.y = coa*right.y + sia*up.y;
			j_rot.z = coa*right.z + sia*up.z;

			right_plus_up.x = i_rot.x + j_rot.x;
			right_plus_up.y = i_rot.y + j_rot.y;
			right_plus_up.z = i_rot.z + j_rot.z;

			right_minus_up.x = i_rot.x - j_rot.x;
			right_minus_up.y = i_rot.y - j_rot.y;
			right_minus_up.z = i_rot.z - j_rot.z;

			pcenter = &pparticle->Homing.Position;
			NVec3Sub((NVEC3*)pvertex,		pcenter,&right_plus_up	);	// vertex a
			NVec3Add((NVEC3*)(pvertex+1),	pcenter,&right_minus_up	);	// vertex b
			NVec3Add((NVEC3*)(pvertex+2),	pcenter,&right_plus_up	);	// vertex c
			NVec3Sub((NVEC3*)(pvertex+3),	pcenter,&right_minus_up	);	// vertex d
			
			pvertex->Color0_4f		=	plut->Color;
			(pvertex+1)->Color0_4f	=	plut->Color;
			(pvertex+2)->Color0_4f	=	plut->Color;
			(pvertex+3)->Color0_4f	=	plut->Color;
		}
		// =============
		// Dead Particle
		// =============
		else
		{
			// CALLBACK
			if( pemitter->Advanced.pAdvancedParticle_DeathCallBack )
				pemitter->Advanced.pAdvancedParticle_DeathCallBack((const void*)pparticle,(const void*)pemitter);

			// Replace Dead Particle by the Current Last Alive Particle
			// ---------------------------------------------------------
			pparticleb = &((NADVANCEDHOMINGPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb-1];
			memcpy( pparticle, pparticleb,sizeof(NADVANCEDHOMINGPARTICLE));		// Data copy from the last awake particle of the list instead of the particle we are falling asleep.
			//  IN case of AUTO EMISSION ...  Initialize rejected particle with its specific birth param.
			if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
			{
				NSetupParticleComponent_Homing(&pparticleb->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
				NSetupParticleComponent_Advanced(&pparticleb->Advanced,&pemitter->Advanced,NULL);			
			}
			// Because the last alive particle has replaced the dead one, we have to make one step back to update it
			pparticle--;i--;pvertex-=4;						
			pemitter->Core.ParticlesNb--;										
		}
	}

	// At the End
	pgeom->Mesh.PrimitiveArray.Size = pemitter->Core.ParticlesNb * 2; // x 2 because 2 triangles for each particle
}
// Refresh just one time after "all" ParticlesEmitter params changes ...
void NRefreshPrecalculatedAdvancedHomingParticles(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter)
{
	// No needs of refreshing in case of lake of "All Used" particles or AUTO EMISSION TURN OFF !
	if( ISFLAG_OFF(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) || pemitter->Core.ParticlesNb == pemitter->Core.ParticlesNbMax )
	{
		return;
	}
	else
	{
		NADVANCEDHOMINGPARTICLE	*pparticle = &((NADVANCEDHOMINGPARTICLE*)(pemitter->Core.pParticlesList))[pemitter->Core.ParticlesNb]; // First asleep particle
		for(Nu16 i = (pemitter->Core.ParticlesNbMax-pemitter->Core.ParticlesNb);i!=0;i--,pparticle++)
		{
			NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
			NSetupParticleComponent_Advanced(&pparticle->Advanced,&pemitter->Advanced,NULL);			
		}
	}
}

void NEmitAdvancedHomingParticles(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter,const Nu16 nbparticles,const NVEC3 *pstart,const NVEC3 *pend)
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
	NADVANCEDHOMINGPARTICLE	*pparticle = &((NADVANCEDHOMINGPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb];
	for(i=max;i!=0;i--,pparticle++)											
	{																				
		NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,pemission,ptarget);			
		NSetupParticleComponent_Advanced(&pparticle->Advanced,&pemitter->Advanced,NULL);			
	}

	pemitter->Core.ParticlesNb += max;
}

void NResetAdvancedHomingParticlesEmitter(NADVANCEDHOMINGPARTICLES_EMITTER *pemitter)
{

	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
	{
		// Reset and pre-calculate all 'Alive' particles for next 'awaking'
		NADVANCEDHOMINGPARTICLE *pparticle = (NADVANCEDHOMINGPARTICLE*)pemitter->Core.pParticlesList;
		for(Nu16 i=pemitter->Core.ParticlesNb;i!=0;i--,pparticle++)								
		{														
			NSetupParticleComponent_Homing(&pparticle->Homing,&pemitter->Homing,&pemitter->Homing.DefaultEmissionPoint,&pemitter->Homing.DefaultTargetPoint);			
			NSetupParticleComponent_Advanced(&pparticle->Advanced,&pemitter->Advanced,NULL);			
		}
	}

	pemitter->Core.ParticlesNb = 0;
}