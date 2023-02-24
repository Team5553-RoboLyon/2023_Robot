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
// +										Generic Particles SetUp Position Handles										+
// +																														+
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NParticleHandle_SetupPointPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD * /*pextents*/, const NVEC3 *prefpoint)
{
	*pparticle_position = *prefpoint;
}
void NParticleHandle_SetupCartesianPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint)
{
	NFastRandExX_Cartesian(pparticle_position, prefpoint, &pextents->Cartesian);
}
void NParticleHandle_SetupFastSphericZPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint)
{
	NFASTSPHERIC fs;
	NVEC3 v;
	fs.Radius = pextents->FastSpheric.Radius * NFastRand();
	fs.Inclination = (Nu32)((Nf32)pextents->FastSpheric.Inclination * NFastRand());
	fs.Azimuth = (Nu32)((Nf32)pextents->FastSpheric.Azimuth * NFastRand());
	NFastSphericZToCartesian(&v, &fs);

	pparticle_position->x = prefpoint->x + v.x;
	pparticle_position->y = prefpoint->y + v.y;
	pparticle_position->z = prefpoint->z + v.z;
}
void NParticleHandle_SetupFastCylindricZPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint)
{
	NFASTCYLINDRIC fc;
	NVEC3 v;
	fc.Radius = pextents->FastCylindric.Radius * NFastRand();
	fc.Height = pextents->FastCylindric.Height * NFastRand();
	fc.Azimuth = (Nu32)((Nf32)pextents->FastCylindric.Azimuth * NFastRand());
	NFastCylindricZToCartesian(&v, &fc);

	pparticle_position->x = prefpoint->x + v.x;
	pparticle_position->y = prefpoint->y + v.y;
	pparticle_position->z = prefpoint->z + v.z;
}

void NParticleHandle_SetupFastSphericZOutlinePosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint)
{
	NFASTSPHERIC fs;
	NVEC3 v;
	fs.Radius = pextents->FastSpheric.Radius; // Constant Radius ! All generated position are going to be "ON" the sphere surface
	fs.Inclination = (Nu32)((Nf32)pextents->FastSpheric.Inclination * NFastRand());
	fs.Azimuth = (Nu32)((Nf32)pextents->FastSpheric.Azimuth * NFastRand());
	NFastSphericZToCartesian(&v, &fs);

	pparticle_position->x = prefpoint->x + v.x;
	pparticle_position->y = prefpoint->y + v.y;
	pparticle_position->z = prefpoint->z + v.z;
}
void NParticleHandle_SetupFastCylindricZOutlinePosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint)
{
	NFASTCYLINDRIC fc;
	NVEC3 v;
	fc.Radius = pextents->FastCylindric.Radius; // Constant Radius ! All generated position are going to be "ON" the Cylinder surface ( top/bottom excepted )
	fc.Height = pextents->FastCylindric.Height * NFastRand();
	fc.Azimuth = (Nu32)((Nf32)pextents->FastCylindric.Azimuth * NFastRand());
	NFastCylindricZToCartesian(&v, &fc);

	pparticle_position->x = prefpoint->x + v.x;
	pparticle_position->y = prefpoint->y + v.y;
	pparticle_position->z = prefpoint->z + v.z;
}
