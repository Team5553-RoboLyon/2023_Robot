#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFastRandExtend.h"
#include "../N3DCoordSystemConversion_CartesianCylindric.h"
#include "../N3DCoordSystemConversion_CartesianSpheric.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NErrorHandling.h"

#include "NParticleSetupHandles.h"
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +																														+
// +									(Advanced) Particles Extents SetUp Handle											+
// +																														+
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NParticleHandle_SetupConstrainExtents(NVEC2 *padvparticle_extents, const NPARTICLEPARAMS_NVECTOR2 *pextents_params)
{
	NVec2Scale(padvparticle_extents, &pextents_params->Base, 1.0f + NFastRand() * pextents_params->Variation.x);
}
void NParticleHandle_SetupFreeExtents(NVEC2 *padvparticle_extents, const NPARTICLEPARAMS_NVECTOR2 *pextents_params)
{
	NFastRandEx_NVec2(padvparticle_extents, &pextents_params->Base, &pextents_params->Variation);
}