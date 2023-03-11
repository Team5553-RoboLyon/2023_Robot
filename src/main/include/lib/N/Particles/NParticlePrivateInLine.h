#ifndef __NPARTICLE_PRIVATE_INLINE_H
#define __NPARTICLE_PRIVATE_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticlePrivateInLine.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/Geometry/Components/NGeometryAppearance.h"
#include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	//
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + CONSTANT ans DEFAULT VALUES
	// ***************************************************************************************
	// **								 Structures											**
	// ***************************************************************************************
	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	// ***************************************************************************************
	// **				LOW LEVEL:PRIVATE Inline Functions									**
	// ***************************************************************************************
	void NExtractUpSideFromModelViewMatrix(NVEC3 *up, NVEC3 *right)
	{
		GLfloat modelview[16];

		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

		right->x = modelview[0];
		right->y = modelview[4];
		right->z = modelview[8];

		up->x = modelview[1];
		up->y = modelview[5];
		up->z = modelview[9];
	}

	inline void NApplyBirthRate(NPARTICLE_EMITTERCOMPONENT_CORE *pemittercore, const Nf32 fdelay)
	{
		pemittercore->ParticleNbToAwake += fdelay * pemittercore->BirthRate;
		Nf32 fnewparticlenb = floorf(pemittercore->ParticleNbToAwake);
		pemittercore->ParticleNbToAwake -= fnewparticlenb; // We keep the rest for the next update loop.
		pemittercore->ParticlesNb = NMIN(pemittercore->ParticlesNb + (Nu16)fnewparticlenb, pemittercore->ParticlesNbMax);
	}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPARTICLE_PRIVATE_INLINE_H
