#ifndef __NPARTICLESETUPHANDLES_H
#define __NPARTICLESETUPHANDLES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticleSetupHandles.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../NCCS_3DCoord.h"
#include "NParticleParams.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DEFAULT_NPARTICLECOMPONENT_EMITTER_STANDARD_SETUP_EMISSIONPOSITION_HANDLE NParticleHandle_SetupPointPosition
#define DEFAULT_NPARTICLECOMPONENT_EMITTER_STANDARD_SETUP_EMISSIONSPEED_HANDLE NParticleHandle_SetupCartesianSpeed

#define DEFAULT_NPARTICLECOMPONENT_EMITTER_ADVANCED_SETUP_EXTENTS_HANDLE NParticleHandle_SetupConstrainExtents

#define DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_EMISSIONPOSITION_HANDLE NParticleHandle_SetupPointPosition
#define DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_TARGETPOSITION_HANDLE NParticleHandle_SetupPointPosition
#define DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_EMISSIONTANGENT_HANDLE NParticleHandle_SetupCartesianTangent
#define DEFAULT_NPARTICLECOMPONENT_EMITTER_HOMING_SETUP_TARGETTANGENT_HANDLE NParticleHandle_SetupCartesianTangent

    typedef void (*NPARTICLE_SETUP_POSITION)(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    typedef void (*NPARTICLE_SETUP_EXTENTS)(NVEC2 *pparticle_extents, const NPARTICLEPARAMS_NVECTOR2 *pextents_params);
    typedef void (*NPARTICLE_SETUP_TANGENT)(NVEC3 *pparticle_keytangent, const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents_params);
    typedef void (*NPARTICLE_SETUP_SPEED)(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params);

    // ***************************************************************************************
    // **								 Functions											**
    // ***************************************************************************************
    // Position
    void NParticleHandle_SetupPointPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    void NParticleHandle_SetupCartesianPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    void NParticleHandle_SetupFastSphericZPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    void NParticleHandle_SetupFastCylindricZPosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    void NParticleHandle_SetupFastSphericZOutlinePosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);
    void NParticleHandle_SetupFastCylindricZOutlinePosition(NVEC3 *pparticle_position, const NCCS_3DCOORD *pextents, const NVEC3 *prefpoint);

    // Speed
    void NParticleHandle_SetupCartesianSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params);
    void NParticleHandle_SetupFastSphericZSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params);
    void NParticleHandle_SetupFastCylindricZSpeed(NVEC3 *pparticle_speed, const NPARTICLEPARAMS_NCCS_3DCOORD *pspeed_params);
    // Advanced Extents
    void NParticleHandle_SetupConstrainExtents(NVEC2 *padvparticle_extents, const NPARTICLEPARAMS_NVECTOR2 *pextents_params);
    void NParticleHandle_SetupFreeExtents(NVEC2 *padvparticle_extents, const NPARTICLEPARAMS_NVECTOR2 *pextents_params);
    // Homing Tangents
    void NParticleHandle_SetupCartesianTangent(NVEC3 *pparticle_keytangent, const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents);
    void NParticleHandle_SetupFastSphericZTangent(NVEC3 *pparticle_keytangent, const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents);
    void NParticleHandle_SetupFastCylindricZTangent(NVEC3 *pparticle_keytangent, const NPARTICLEPARAMS_NCCS_3DCOORD *pkeytangentextents);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPARTICLESETUPHANDLES_H
