#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/Render/Renderable/NRenderable.h"
#include "lib/N/Render/NFrustum_Culling.h"
#include "lib/N/NCore.h"
#include "lib/N/Utilities/Maths/NUT_MathsMisc.h"
#include "NAdvancedParticles.h"

/*
// Extern Global
extern NNODELIST						NParticlesObstaclesList;
*/

// ------------------------------------------------------------------------------------------
// NCreateAdvancedParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Create a Particles emitter
// ------------------------------------------------------------------------------------------
// In  :
//		pdesc	:			Emitter Description
// Out :
//		ptr on the new Advanced Emitter
// ------------------------------------------------------------------------------------------
NADVANCEDPARTICLES_EMITTER *NCreateAdvancedParticlesEmitter(const NADVANCEDPARTICLES_EMITTER_DESC *pdesc)
{
	return NSetUpAdvancedParticlesEmitter(NEW(NADVANCEDPARTICLES_EMITTER), pdesc);
}

NADVANCEDPARTICLES_EMITTER *NSetUpAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter, const NADVANCEDPARTICLES_EMITTER_DESC *pdesc)
{
	NSetupParticlesEmitterComponent_Core(&pemitter->Core, &pdesc->CoreDesc, sizeof(NADVANCEDPARTICLE), NRenderableUpdate_AdvancedParticlesEmitter);
	NSetupParticlesEmitterComponent_LUT(&pemitter->Lut, &pdesc->LutDesc);
	NSetupParticlesEmitterComponent_Standard(&pemitter->Std, &pdesc->StdDesc);
	NSetupParticlesEmitterComponent_Advanced(&pemitter->Advanced, &pdesc->AdvancedDesc);
	return pemitter;
}

// ------------------------------------------------------------------------------------------
// NDeleteAdvancedParticlesEmitter
// ------------------------------------------------------------------------------------------
// Description :
//	Delete a Particles emitter
// ------------------------------------------------------------------------------------------
// In  :
//		pemitter: ptr on the emitter to delete
//
// Out :
// ------------------------------------------------------------------------------------------
void NDeleteAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter)
{
	NClearAdvancedParticlesEmitter(pemitter);
	Nfree(pemitter);
}
void NClearAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter)
{
	NClearParticlesEmitterComponent_Advanced(&pemitter->Advanced);
	NClearParticlesEmitterComponent_Standard(&pemitter->Std);
	NClearParticlesEmitterComponent_LUT(&pemitter->Lut);
	NClearParticlesEmitterComponent_Core(&pemitter->Core);
}

// ------------------------------------------------------------------------------------------
// NRenderableUpdate_AdvancedParticlesEmitter
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
void NRenderableUpdate_AdvancedParticlesEmitter(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{
	// =========================================================================
	// EXTRACT (and return if no extraction ... )
	if (!prenderable->Extract_FCT(prenderable, powner) || ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PARTICLES))
	{
		return;
	}

	Nu32 i;
	NADVANCEDPARTICLE *pparticleb;
	Nf32 co, si, coa, sia;
	NVEC3 up, right;
	NVEC3 right_plus_up, right_minus_up;
	NVEC3 i_rot, j_rot;
	NVEC3 *pcenter;
	NPARTICLELUT *plut;

	// ====
	// Init
	// ====
	NADVANCEDPARTICLES_EMITTER *pemitter = (NADVANCEDPARTICLES_EMITTER *)powner;
	NADVANCEDPARTICLE *pparticle = (NADVANCEDPARTICLE *)pemitter->Core.pParticlesList;
	NGEOMETRY *pgeom = NGetFirstIncludedGeometry(pemitter->Core.pRenderable);
	NPARTICLEVERTEX *pvertex = (NPARTICLEVERTEX *)NGetFirstMeshVertexPtr(&pgeom->Mesh);
	Nf32 fdelay = ptime->Nf32_Delay;

	NUT_ExtractUpSideFromModelViewMatrix(&up, &right);

	// =========================================================================
	// Calculate the number of new particles to AUTO awake during this update loop
	// =========================================================================
	if (ISFLAG_ON(pemitter->Core.Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION))
		NApplyBirthRate(&pemitter->Core, fdelay);

	// =====================
	// Particles Update Loop
	// =====================
	for (i = 0; i < pemitter->Core.ParticlesNb; i++, pvertex += 4, pparticle++)
	{
		pparticle->Std.Age += fdelay;
		// ==============
		// Alive Particle
		// ==============
		if (pparticle->Std.Age < pparticle->Std.LifeSpan)
		{
			// Calculate Rotation Speed Position
			pparticle->Std.Rotation += pparticle->Std.RotationSpeed * fdelay;

			pparticle->Std.Speed.x = pemitter->Std.SpeedDamping * pparticle->Std.Speed.x + fdelay * pparticle->Advanced.AllForces.x;
			pparticle->Std.Speed.y = pemitter->Std.SpeedDamping * pparticle->Std.Speed.y + fdelay * pparticle->Advanced.AllForces.y;
			pparticle->Std.Speed.z = pemitter->Std.SpeedDamping * pparticle->Std.Speed.z + fdelay * pparticle->Advanced.AllForces.z;

			pparticle->Std.Position.x += fdelay * pparticle->Std.Speed.x;
			pparticle->Std.Position.y += fdelay * pparticle->Std.Speed.y;
			pparticle->Std.Position.z += fdelay * pparticle->Std.Speed.z;

			// Reset "All Forces": (At minimum it will be equal to Gravity ...)
			pparticle->Advanced.AllForces = pemitter->Std.Gravity;

			// CALLBACK
			if (pemitter->Advanced.pAdvancedParticle_UpdateCallBack)
				pemitter->Advanced.pAdvancedParticle_UpdateCallBack((const void *)pparticle, (const void *)pemitter);

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
			plut = &pemitter->Lut.pLUT[(Nu16)((pparticle->Std.Age / pparticle->Std.LifeSpan) * (Nf32)(pemitter->Lut.LUT_Size - 1))];

			NFastCosSin((Nu32)pparticle->Std.Rotation, &co, &si);
			co *= plut->Scale;
			si *= plut->Scale;

			coa = co * pparticle->Advanced.Extents.x;
			sia = si * pparticle->Advanced.Extents.x;
			i_rot.x = coa * right.x + sia * up.x;
			i_rot.y = coa * right.y + sia * up.y;
			i_rot.z = coa * right.z + sia * up.z;

			// Notes: cos(a+pi/2) = -sin(a) AND  sin(a+pi/2) = cos(a)
			coa = -si * pparticle->Advanced.Extents.y;
			sia = co * pparticle->Advanced.Extents.y;
			j_rot.x = coa * right.x + sia * up.x;
			j_rot.y = coa * right.y + sia * up.y;
			j_rot.z = coa * right.z + sia * up.z;

			right_plus_up.x = i_rot.x + j_rot.x;
			right_plus_up.y = i_rot.y + j_rot.y;
			right_plus_up.z = i_rot.z + j_rot.z;

			right_minus_up.x = i_rot.x - j_rot.x;
			right_minus_up.y = i_rot.y - j_rot.y;
			right_minus_up.z = i_rot.z - j_rot.z;

			pcenter = &pparticle->Std.Position;
			NVec3Sub((NVEC3 *)pvertex, pcenter, &right_plus_up);		// vertex a
			NVec3Add((NVEC3 *)(pvertex + 1), pcenter, &right_minus_up); // vertex b
			NVec3Add((NVEC3 *)(pvertex + 2), pcenter, &right_plus_up);	// vertex c
			NVec3Sub((NVEC3 *)(pvertex + 3), pcenter, &right_minus_up); // vertex d
			// TODO: 4 colors LUT & memcpy all of them in one "memcpy" call
			pvertex->Color0_4f = plut->Color;
			(pvertex + 1)->Color0_4f = plut->Color;
			(pvertex + 2)->Color0_4f = plut->Color;
			(pvertex + 3)->Color0_4f = plut->Color;
		}
		// =============
		// Dead Particle
		// =============
		else
		{
			// CALLBACK
			if (pemitter->Advanced.pAdvancedParticle_DeathCallBack)
				pemitter->Advanced.pAdvancedParticle_DeathCallBack((const void *)pparticle, (const void *)pemitter);

			pparticleb = &((NADVANCEDPARTICLE *)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb - 1];
			// Data copy from the last awake particle of the list ON the particle we are falling asleep.
			memcpy(pparticle, pparticleb, sizeof(NADVANCEDPARTICLE));

			// IN case of AUTO EMISSION ... After-Last particle (of the list) Pre-initialization for its future awaking
			if (ISFLAG_ON(pemitter->Core.Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION))
			{
				NSetupParticleComponent_Standard(&pparticleb->Std, &pemitter->Std, &pemitter->Std.DefaultEmissionPoint);
				NSetupParticleComponent_Advanced(&pparticleb->Advanced, &pemitter->Advanced, &pemitter->Std.Gravity);
			}
			// Because the last alive particle has replaced the dead one, we have to make one step back to update it
			pparticle--;
			i--;
			pvertex -= 4;
			pemitter->Core.ParticlesNb--;
		}
	}
	// At the End
	pgeom->Mesh.PrimitiveArray.Size = pemitter->Core.ParticlesNb * 2; // x 2 because 2 triangles for each particle
}
void NRefreshPrecalculatedAdvancedParticles(NADVANCEDPARTICLES_EMITTER *pemitter)
{
	if (ISFLAG_OFF(pemitter->Core.Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION) || pemitter->Core.ParticlesNb == pemitter->Core.ParticlesNbMax)
	{
		return;
	}
	else
	{
		NADVANCEDPARTICLE *padvparticle = &((NADVANCEDPARTICLE *)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb]; // First asleep particle
		for (Nu16 i = (pemitter->Core.ParticlesNbMax - pemitter->Core.ParticlesNb); i != 0; i--, padvparticle++)
		{
			NSetupParticleComponent_Standard(&padvparticle->Std, &pemitter->Std, &pemitter->Std.DefaultEmissionPoint);
			NSetupParticleComponent_Advanced(&padvparticle->Advanced, &pemitter->Advanced, &pemitter->Std.Gravity);
		}
	}
}

void NEmitAdvancedParticles(NADVANCEDPARTICLES_EMITTER *pemitter, const Nu16 nbparticles, const NVEC3 *pspecific_emissionpoint)
{
	Nu16 i;
	NADVANCEDPARTICLE *part;
	Nu16 max;
	NVEC3 *pemission;

	if ((pemitter->Core.ParticlesNb + nbparticles) > pemitter->Core.ParticlesNbMax)
		max = pemitter->Core.ParticlesNbMax - pemitter->Core.ParticlesNb;
	else
		max = nbparticles;

	if (pspecific_emissionpoint)
		pemission = (NVEC3 *)pspecific_emissionpoint;
	else
		pemission = &pemitter->Std.DefaultEmissionPoint;

	part = &((NADVANCEDPARTICLE *)pemitter->Core.pParticlesList)[pemitter->Core.ParticlesNb];
	// "part" may be out of range ! (when pemitter->ParticlesNb == pemitter->ParticlesNbMax). But it doesn't matter...
	// We are going to use it and write in memory ONLY if its not beyond limit, thanks to "max" value.
	for (i = max; i != 0; i--, part++)
	{
		NSetupParticleComponent_Standard(&part->Std, &pemitter->Std, pemission);
		NSetupParticleComponent_Advanced(&part->Advanced, &pemitter->Advanced, &pemitter->Std.Gravity);
	}
	pemitter->Core.ParticlesNb += max;
}

void NResetAdvancedParticlesEmitter(NADVANCEDPARTICLES_EMITTER *pemitter)
{

	if (ISFLAG_ON(pemitter->Core.Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION))
	{
		// Reset and pre-calculate all 'Alive' particles for next 'awaking'
		NADVANCEDPARTICLE *padvparticle = (NADVANCEDPARTICLE *)pemitter->Core.pParticlesList;
		for (Nu16 i = pemitter->Core.ParticlesNb; i != 0; i--, padvparticle++)
		{
			NSetupParticleComponent_Standard(&padvparticle->Std, &pemitter->Std, &pemitter->Std.DefaultEmissionPoint);
			NSetupParticleComponent_Advanced(&padvparticle->Advanced, &pemitter->Advanced, &pemitter->Std.Gravity);
		}
	}

	pemitter->Core.ParticlesNb = 0;
}