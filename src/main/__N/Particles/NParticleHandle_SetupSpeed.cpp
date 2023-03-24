#include "../NCStandard.h"
#include "../NType.h"
#include "../NFastRandExtend.h"
#include "../N3DCoordSystemConversion_CartesianCylindric.h"
#include "../N3DCoordSystemConversion_CartesianSpheric.h"
#include "../GL/Ngl.h"
#include "../NErrorHandling.h"

#include "NParticleSetupHandles.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +																														+
// +										Generic Particles SPEED SetUp Functions											+
// +																														+
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NParticleHandle_SetupCartesianSpeed( NVEC3 *pparticle_speed,	const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params )
{
	NFastRandEx_Cartesian(pparticle_speed,&pspeed_params->Base.Cartesian,&pspeed_params->Variation.Cartesian);
}
void NParticleHandle_SetupFastSphericZSpeed( NVEC3 *pparticle_speed,	const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params )
{
	NFASTSPHERIC	fs;
	NFastRandEx_FastSpheric(&fs,&pspeed_params->Base.FastSpheric,&pspeed_params->Variation.FastSpheric);
	NFastSphericZToCartesian(pparticle_speed,&fs);
}
void NParticleHandle_SetupFastCylindricZSpeed( NVEC3 *pparticle_speed,	const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params )
{
	NFASTCYLINDRIC		fc;
	NFastRandEx_FastCylindric(&fc,&pspeed_params->Base.FastCylindric,&pspeed_params->Variation.FastCylindric);
	NFastCylindricZToCartesian(pparticle_speed,&fc);
}

