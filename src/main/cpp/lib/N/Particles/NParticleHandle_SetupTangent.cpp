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
// +										Generic Particles SetUp Tangent Handles											+
// +																														+
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NParticleHandle_SetupCartesianTangent( NVEC3 *pparticle_keytangent,	const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents )
{
	NFastRandEx_Cartesian(pparticle_keytangent, &pkeytangentextents->Base.Cartesian,&pkeytangentextents->Variation.Cartesian);
}

void NParticleHandle_SetupFastSphericZTangent( NVEC3 *pparticle_keytangent,	const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents )
{
	NFASTSPHERIC	sph;
	NFastRandEx_FastSpheric(&sph,&pkeytangentextents->Base.FastSpheric,&pkeytangentextents->Variation.FastSpheric);
	NFastSphericZToCartesian(pparticle_keytangent,&sph);
}
void NParticleHandle_SetupFastCylindricZTangent( NVEC3 *pparticle_keytangent,	const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents )
{
	NFASTCYLINDRIC	fc;
	NFastRandEx_FastCylindric(&fc,&pkeytangentextents->Base.FastCylindric,&pkeytangentextents->Variation.FastCylindric);
	NFastCylindricZToCartesian(pparticle_keytangent,&fc);
}
