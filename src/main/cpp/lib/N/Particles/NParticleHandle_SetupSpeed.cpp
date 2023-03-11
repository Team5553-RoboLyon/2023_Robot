#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFastRandExtend.h"
#include "lib/N/N3DCoordSystemConversion_CartesianCylindric.h"
#include "lib/N/N3DCoordSystemConversion_CartesianSpheric.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NErrorHandling.h"

#include "NParticleSetupHandles.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +																														+
// +										Generic Particles SPEED SetUp Functions											+
// +																														+
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NParticleHandle_SetupCartesianSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params)
{
	NFastRandEx_Cartesian(pparticle_speed, &pspeed_params->Base.Cartesian, &pspeed_params->Variation.Cartesian);
}
void NParticleHandle_SetupFastSphericZSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params)
{
	NFASTSPHERIC fs;
	NFastRandEx_FastSpheric(&fs, &pspeed_params->Base.FastSpheric, &pspeed_params->Variation.FastSpheric);
	NFastSphericZToCartesian(pparticle_speed, &fs);
}
void NParticleHandle_SetupFastCylindricZSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params)
{
	NFASTCYLINDRIC fc;
	NFastRandEx_FastCylindric(&fc, &pspeed_params->Base.FastCylindric, &pspeed_params->Variation.FastCylindric);
	NFastCylindricZToCartesian(pparticle_speed, &fc);
}
