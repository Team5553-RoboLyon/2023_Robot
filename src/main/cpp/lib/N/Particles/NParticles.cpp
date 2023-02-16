#include "../NCStandard.h"
#include "../NType.h"
#include "../NCore.h"
#include "../GL/Ngl.h"
#include "../Geometry/NGeometry.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Render/NFrustum_Culling.h"
#include "../Utilities/Maths/NUT_MathsMisc.h"

#include "NParticles.h"

/*
// debug matrix concept
#include "../Utilities/Draw/NUT_Draw.h"
#include "../NCamera.h"
#include "../NCoordinateSystemConversions.h"
// debug matrix concept
*/


// ------------------------------------------------------------------------------------------
// NCreateParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Create a Particles emitter
// ------------------------------------------------------------------------------------------
// In  :
//		pemitterlist	:	Th list where the emitter will be inserted in. ( can be NULL )
//		emitter_position:	ptr on a vector with the location of the emitter
//		particlenbmax	:	max number of particles usable at the same time by this emitter
//		ptexture		:	valid ptr on the texture used by particles to render.
// Out :
//		ptr on the new emitter
// ------------------------------------------------------------------------------------------
NPARTICLES_EMITTER* NCreateParticlesEmitter( const NPARTICLES_EMITTER_DESC *pdesc )
{
	return NSetupParticlesEmitter(NEW(NPARTICLES_EMITTER),pdesc);
}

NPARTICLES_EMITTER* NSetupParticlesEmitter( NPARTICLES_EMITTER *pemitter, const NPARTICLES_EMITTER_DESC *pdesc )
{
	NSetupParticlesEmitterComponent_Core(&pemitter->Core,&pdesc->CoreDesc,sizeof(NPARTICLE),NRenderableUpdate_ParticlesEmitter);
	NSetupParticlesEmitterComponent_LUT(&pemitter->Lut,&pdesc->LutDesc);
	NSetupParticlesEmitterComponent_Standard(&pemitter->Std,&pdesc->StdDesc);
	return pemitter;
}

// ------------------------------------------------------------------------------------------
// NDeleteParticlesEmitter
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
void NDeleteParticlesEmitter(NPARTICLES_EMITTER* pemitter)
{
	NClearParticlesEmitter(pemitter);
	Nfree(pemitter);
}
void NClearParticlesEmitter(NPARTICLES_EMITTER* pemitter)
{
	NClearParticlesEmitterComponent_Standard(&pemitter->Std);
	NClearParticlesEmitterComponent_LUT(&pemitter->Lut);
	NClearParticlesEmitterComponent_Core(&pemitter->Core);
}
// ------------------------------------------------------------------------------------------
// NUpdateParticlesEmitter
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
void NRenderableUpdate_ParticlesEmitter(NRENDERABLE *prenderable, void* powner, const NTIME *ptime)
{
	// =========================================================================
	// EXTRACT (and return if no extraction ... )
	if( !prenderable->Extract_FCT( prenderable,powner )||ISFLAG_ON( NEngineCoreFlags,FLAG_NENGINE_CORE_PAUSE_PARTICLES ) )
	{
		return;
	}

	NPARTICLES_EMITTER* pemitter = (NPARTICLES_EMITTER*)powner;
	Nu32				i;
	NPARTICLE			*pparticleb;
	NVEC3			up,right;
	NVEC3			right_plus_up,right_plus_up2;
	NVEC3			*pcenter;
	Nf32				co,si;
	NPARTICLELUT		*plut;

	// ====
	// Init
	// ====
	NPARTICLE			*pparticle	= (NPARTICLE*)pemitter->Core.pParticlesList;
	NGEOMETRY			*pgeom		= (NGEOMETRY*)pemitter->Core.pRenderable->GeomArray.pFirst;//NGetFirstIncludedGeometry(pemitter->Core.pRenderable);
	NPARTICLEVERTEX		*pvertex	= (NPARTICLEVERTEX*)pgeom->Mesh.VertexArray.pFirst;//NGetFirstMeshVertexPtr(&pgeom->Mesh);

// 	NCOLOR		*pcolor		= NGetFirstGeometryColorPtr(&pgeom->Mesh);
	Nf32		fdelay		= ptime->Nf32_Delay;

	NUT_ExtractUpSideFromModelViewMatrix(&up,&right);

/*
	// debug matrix concept
	Nchar str[1024];
	NVEC3	vtext;
	NMATRIX		base;
	NExtractCameraMatrixFromGLModelViewMatrix(&base);
	//NExtractOrthonormalBasisFromModelViewMatrix(&base);
	NVec3Set( &vtext,NCoord_NormToHRel_X(0.1f),NCoord_NormToHRel_Y(-0.1f), 0.0f );
	sprintf(str,"glx: %.5f %.5f %.5f %.5f nx: %.5f %.5f %.5f %.5f\ngly: %.5f %.5f %.5f %.5f ny: %.5f %.5f %.5f %.5f\nglz: %.5f %.5f %.5f %.5f nz: %.5f %.5f %.5f %.5f\nglw: %.5f %.5f %.5f %.5f nw: %.5f %.5f %.5f %.5f\n " ,
	base.Side.x, base.Up.x,base.Forward.x, base.Translation.x,NGetCamera()->XAxis.x,NGetCamera()->YAxis.x,NGetCamera()->ZAxis.x,NGetCamera()->Pos.x,
	base.Side.y, base.Up.y,base.Forward.y, base.Translation.y,NGetCamera()->XAxis.y,NGetCamera()->YAxis.y,NGetCamera()->ZAxis.y,NGetCamera()->Pos.y,
	base.Side.z, base.Up.z,base.Forward.z, base.Translation.z,NGetCamera()->XAxis.z,NGetCamera()->YAxis.z,NGetCamera()->ZAxis.z,NGetCamera()->Pos.z,
	base.Side.w, base.Up.w,base.Forward.w, base.Translation.w,0.0f,0.0f,0.0f,1.0f);
	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
	NUT_SetDrawConstructionPlane(_PLANE_XY );
	NUT_Draw_Text(str,&vtext,NULL);
	// debug matrix concept
*/



	// ===========================================================================
	// Calculate the number of new particles to AUTO awake during this update loop
	// ===========================================================================
	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
		NApplyBirthRate(&pemitter->Core,fdelay);
	
	// =====================
	// Particles Update Loop
	// =====================

	for(i=0;i<pemitter->Core.ParticlesNb;i++,pvertex+=4,pparticle++) 
	{
		pparticle->Std.Age+=fdelay;
		// ==============
		// Alive Particle
		// ==============
		if(pparticle->Std.Age < pparticle->Std.LifeSpan)
		{
			// Calculate Speed Position Rotation 
			pparticle->Std.Rotation += pparticle->Std.RotationSpeed*fdelay;
		
			pparticle->Std.Speed.x = pemitter->Std.SpeedDamping*pparticle->Std.Speed.x + fdelay*pemitter->Std.Gravity.x;
			pparticle->Std.Speed.y = pemitter->Std.SpeedDamping*pparticle->Std.Speed.y + fdelay*pemitter->Std.Gravity.y;
			pparticle->Std.Speed.z = pemitter->Std.SpeedDamping*pparticle->Std.Speed.z + fdelay*pemitter->Std.Gravity.z;

			pparticle->Std.Position.x += fdelay*pparticle->Std.Speed.x;
			pparticle->Std.Position.y += fdelay*pparticle->Std.Speed.y;
			pparticle->Std.Position.z += fdelay*pparticle->Std.Speed.z;
			// Build particle mesh
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// !!! OPTIMIZED Building !!!
			// For this optimized version we consider particle as SQUARE only.
			// LUT.Scale is used as half square Diagonal Size ! 
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
			plut = &pemitter->Lut.pLUT[ (Nu16)( (pparticle->Std.Age/pparticle->Std.LifeSpan)*(Nf32)(pemitter->Lut.LUT_Size-1) ) ] ;
			
			NFastCosSin((Nu32)pparticle->Std.Rotation,&co,&si);
			co *= plut->Scale;
			si *= plut->Scale;
			right_plus_up.x = co*right.x + si*up.x;
			right_plus_up.y = co*right.y + si*up.y;
			right_plus_up.z = co*right.z + si*up.z;
			// notes: cos(a+pi/2) = -sin(a) AND sin(a+pi/2) = cos(a). So ...
			right_plus_up2.x = co*up.x - si*right.x;
			right_plus_up2.y = co*up.y - si*right.y;
			right_plus_up2.z = co*up.z - si*right.z;

			pcenter = &pparticle->Std.Position;
			NVec3Sub((NVEC3*)pvertex,pcenter,&right_plus_up);			// vertex a
			NVec3Add((NVEC3*)(pvertex+1),pcenter,&right_plus_up2);	// vertex b
			NVec3Add((NVEC3*)(pvertex+2),pcenter,&right_plus_up);		// vertex c
			NVec3Sub((NVEC3*)(pvertex+3),pcenter,&right_plus_up2);	// vertex d
			// TODO: 4 colors LUT & memcpy all of them in one "memcpy" call 
			pvertex->Color0_4f		= plut->Color;
			(pvertex+1)->Color0_4f	= plut->Color;
			(pvertex+2)->Color0_4f	= plut->Color;
			(pvertex+3)->Color0_4f	= plut->Color;
		}
		// =============
		// Dead Particle
		// =============
		else
		{
			pparticleb = &((NPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb-1];
			// Data copy from the last awake particle of the list ON the particle we are falling asleep.
			memcpy( pparticle, pparticleb,sizeof(NPARTICLE));				
			
			// IN case of AUTO EMISSION ... After-Last particle (of the list) Pre-initialization for its future awaking
			if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
				NSetupParticleComponent_Standard(&pparticleb->Std,&pemitter->Std,&pemitter->Std.DefaultEmissionPoint);	
			
			// Because the last alive particle has replaced the dead one, we have to make one step back to update it
			pparticle--;i--;pvertex-=4;							
			pemitter->Core.ParticlesNb--;										
		}
	}
	// At the End
	pgeom->Mesh.PrimitiveArray.Size = pemitter->Core.ParticlesNb * 2; // x 2 because 2 triangles for each particle
}

// Refresh just one time after "all" ParticlesEmitter params changes ...
void NRefreshPrecalculatedParticles(NPARTICLES_EMITTER *pemitter)
{
	// No needs of refreshing in case of lake of "All Used" particles or AUTO EMISSION TURN OFF !
	if( ISFLAG_OFF(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) || pemitter->Core.ParticlesNb == pemitter->Core.ParticlesNbMax )
	{
		return;
	}
	else
	{
		NPARTICLE	*pparticle = &((NPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb]; // First asleep particle
		for(Nu16 i = (pemitter->Core.ParticlesNbMax-pemitter->Core.ParticlesNb);i!=0;i--,pparticle++)
		{
			NSetupParticleComponent_Standard(&pparticle->Std,&pemitter->Std,&pemitter->Std.DefaultEmissionPoint);
		}
	}
}
void NEmitParticles(NPARTICLES_EMITTER *pemitter,const Nu16 nbparticles,const NVEC3 *pspecific_emissionpoint)
{
	Nu16		i;
	NPARTICLE	*part;
	Nu16		max;
	NVEC3	*pemission;

	if( (pemitter->Core.ParticlesNb + nbparticles) > pemitter->Core.ParticlesNbMax )
		max = pemitter->Core.ParticlesNbMax - pemitter->Core.ParticlesNb;
	else
		max = nbparticles;

	if( pspecific_emissionpoint )
		pemission = (NVEC3*)pspecific_emissionpoint;
	else
		pemission = &pemitter->Std.DefaultEmissionPoint;

	part = &((NPARTICLE*)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb];
	// "part" may be out of range ! (when pemitter->ParticlesNb == pemitter->ParticlesNbMax). But it doesn't matter...
	// We are going to use it and write in memory ONLY if its not beyond limit, thanks to "max" value.
	for(i=max;i!=0;i--,part++)								
	{														
		NSetupParticleComponent_Standard(&part->Std,&pemitter->Std,pemission);
	}
	pemitter->Core.ParticlesNb += max;
}

void NResetParticlesEmitter(NPARTICLES_EMITTER *pemitter)
{
	
	if( ISFLAG_ON(pemitter->Core.Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) )
	{
		// Reset and pre-calculate all 'Alive' particles for next 'awaking'
		NPARTICLE *part = (NPARTICLE*)pemitter->Core.pParticlesList;
		for(Nu16 i=pemitter->Core.ParticlesNb;i!=0;i--,part++)								
		{														
			NSetupParticleComponent_Standard(&part->Std,&pemitter->Std,&pemitter->Std.DefaultEmissionPoint);
		}
	}
	
	pemitter->Core.ParticlesNb = 0;
}