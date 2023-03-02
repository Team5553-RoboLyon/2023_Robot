#ifndef __NPARTICLE_CONSTANTS_H
#define __NPARTICLE_CONSTANTS_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticle_Constants.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// SPEED DAMPING ( Inverted !!! 1.0f means no speed Damping ... but you know that!
#define CONSTANT_NPARTICLE_SPEEDDAMPING_MIN							1.0f
#define CONSTANT_NPARTICLE_SPEEDDAMPING_MAX							0.0f
																		
#define CONSTANT_NPARTICLE_SPEEDDAMPING_NULL						1.0f		//	| min
#define CONSTANT_NPARTICLE_SPEEDDAMPING_LOW							0.99f		//	|
#define CONSTANT_NPARTICLE_SPEEDDAMPING_MEDIUM						0.98f		//	|
#define CONSTANT_NPARTICLE_SPEEDDAMPING_HIGH						0.95f		//	|
#define CONSTANT_NPARTICLE_SPEEDDAMPING_VERYHIGH					0.85f		//	V max
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NPARTICLE_CONSTANTS_H 

