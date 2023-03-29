#pragma once
/*
#ifndef __NVECLIMITS_H
#define __NVECLIMITS_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NVecLimits.h										**
// **																					**
// ***************************************************************************************
//#include "../NCStandard.h"
//#include "../NType.h"
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// Specific Nf32 limits
#define NF32_EPSILON_VECTOR_LENGTH			0.000001f	// = 1 / 1000 000
#define NF32_VERYSMALL_VECTOR_LENGTH		0.00001f	// = 1 / 100 000
#define NF32_SMALL_VECTOR_LENGTH			0.0001f		// = 1 / 10 000

#define NF32_EPSILON_VECTOR_DOTPRODUCT		0.000001f
#define NF32_VERYSMALL_VECTOR_DOTPRODUCT	0.00001f
#define NF32_SMALL_VECTOR_DOTPRODUCT		0.0001f

// Specific Nf64 limits
#define NF64_EPSILON_VECTOR_LENGTH			0.000000001		// = 1 / 1000 000 000
#define NF64_VERYSMALL_VECTOR_LENGTH		0.000001		// = 1 / 1000 000
#define NF64_SMALL_VECTOR_LENGTH			0.00001			// = 1 / 100 000 

#define NF64_EPSILON_VECTOR_DOTPRODUCT		0.000000001
#define NF64_VERYSMALL_VECTOR_DOTPRODUCT	0.000001
#define NF64_SMALL_VECTOR_DOTPRODUCT		0.00001

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NVECLIMITS_H 

*/