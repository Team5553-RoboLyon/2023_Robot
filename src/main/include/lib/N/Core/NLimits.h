#ifndef __NLIMITS_H
#define __NLIMITS_H

#ifdef __cplusplus
extern "C"
{
#endif
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + GLOBAL LIMITS
// +
// + Minimum Positive Values
#define NU8_MIN				(Nu8)0		
#define NU16_MIN			(Nu16)0		
#define NU32_MIN			(Nu32)0		
#define NU64_MIN			(Nu64)0		

#define NS8_MIN				(Ns8)0	
#define NS16_MIN			(Ns16)0
#define NS32_MIN			(Ns32)0
#define NS64_MIN			(Ns64)0

#define NF32_MIN			1.175494351e-38F		
#define NF64_MIN			1.175494351e-38			
// +
// + Maximum Positive Values
// +
#define NU8_MAX				(Nu8)255	
#define NU16_MAX			(Nu16)65535	
#define NU32_MAX			(Nu32)4294967295	
#define NU64_MAX			(Nu64)18446744073709551615	

#define NS8_MAX				(Ns8)127	
#define NS16_MAX			(Ns16)32767	
#define NS32_MAX			(Ns32)2147483647	
#define NS64_MAX			(Ns64)9223372036854775807	

#define NF32_MAX			3.402823466e+38F			
#define NF64_MAX			1.7976931348623158e+308 		

#define NF32_EPSILON		1.192092896e-07F			// old used was ... 0.00001f					// small Nf32 (=small float)
#define NF64_EPSILON		2.2204460492503131e-016		 

	
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif //  __NLIMITS_H 

