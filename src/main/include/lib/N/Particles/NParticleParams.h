#ifndef __NPARTICLEPARAMS_H
#define __NPARTICLEPARAMS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NParticleParams.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NCCS_3DCoord.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + Particles "Random Range Params" for "under control random particle params setup"
	typedef struct
	{
		Nf32 Base;
		Nf32 Variation;
	} NPARTICLEPARAMS_NF32;
	typedef struct
	{
		NVEC2 Base;
		NVEC2 Variation;
	} NPARTICLEPARAMS_NVECTOR2;
	typedef struct
	{
		NCCS_3DCOORD Base;
		NCCS_3DCOORD Variation;
	} NPARTICLEPARAMS_NCCS_3DCOORD;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPARTICLEPARAMS_H
