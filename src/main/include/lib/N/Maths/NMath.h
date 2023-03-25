/*
#ifndef __NMATH_H
#define __NMATH_H
*/
#pragma once

#include	<math.h>
#include	<cmath>
#include	"lib/N/NType.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NMath.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
/*
// Init Math LUTs
#define RANDLUT_SIZE		16384					// Number of pre-calculated Rand float number stored in the RandLUT
													// !! power of two !!
#define NANGLELUT_SIZE		4096					// for 360.00�, so with around 0.09 precision.
													// !! power of two !!
*/

#define NANGLE0			0
#define NANGLE5			(NANGLELUT_SIZE / 72)
#define NANGLE10		(NANGLELUT_SIZE / 36)
#define NANGLE15		(NANGLELUT_SIZE / 24)
#define NANGLE20		(NANGLELUT_SIZE / 18)
#define NANGLE30		(NANGLELUT_SIZE / 12)
#define NANGLE36		(NANGLELUT_SIZE / 10)
#define NANGLE45		(NANGLELUT_SIZE / 8)
#define NANGLE60		(NANGLELUT_SIZE / 6)
#define NANGLE72		(NANGLELUT_SIZE / 5)
#define NANGLE90		(NANGLELUT_SIZE / 4)			// 1024
#define NANGLE120		(NANGLELUT_SIZE / 3)			
#define NANGLE180		(NANGLELUT_SIZE / 2)			// 2048
#define NANGLE225		( (Nu32)( (Nf32)NANGLELUT_SIZE*(5.0f/8.0f) ) )
#define NANGLE270		(NANGLELUT_SIZE - NANGLE90)
#define NANGLE315		(NANGLELUT_SIZE - NANGLE45)
#define NANGLE360		NANGLELUT_SIZE

#define NANGLEMAX		(NANGLELUT_SIZE - 1) // represents the last available index into the ANGLE LUT

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + USEFUL MATHEMATICAL CONSTANTS
// +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NF32
// +
// +--+ PI
#define NF32_PI			3.1415927f		// PI
#define NF32_2PI		6.2831853f		// 2*PI
#define NF32_PI_2		1.5707963f		// PI/2	
#define NF32_PI_3		1.0471976f		// PI/3
#define NF32_PI_4		0.7853982f		// PI/4
#define NF32_1_PI		0.3183099f		// 1/PI
#define NF32_1_2PI		0.1591549f		// 1/(2*PI)
#define NF32_2_PI		0.6366198f		// 2/PI
#define NF32_1_SQRTPI	0.5641896f		// 1/sqrt(PI)
#define NF32_2_SQRTPI	1.1283792f		// 2/sqrt(PI)
// +
// +--+ SQRT(2)
// +
#define NF32_SQRT2		1.4142136f		// Sqrt(2)
#define NF32_1_SQRT2	0.7071068f		// 1/Sqrt(2)
#define NF32_SQRT2_2	0.7071068f		// Sqrt(2)/2 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NF64
// +
// +--+ PI
#define NF64_PI			3.14159265358979323846		// PI
#define NF64_2PI		6.28318530717958647693		// 2*PI
#define NF64_PI_2		1.57079632679489661923		// PI/2	
#define NF64_PI_3		1.04719755119659774615		// PI/3
#define NF64_PI_4		0.78539816339744830962		// PI/4
#define NF64_1_PI		0.31830988618379067154		// 1/PI
#define NF64_1_2PI		0.15915494309189533577		// 1/(2*PI)
#define NF64_2_PI		0.63661977236758134308		// 2/PI
#define NF64_1_SQRTPI	0.56418958354775628695		// 1/sqrt(PI)
#define NF64_2_SQRTPI	1.12837916709551257390		// 2/sqrt(PI)
// +
// +--+ SQRT(2)
// +
#define NF64_SQRT2		1.41421356237309504880		// Sqrt(2)
#define NF64_1_SQRT2	0.70710678118654752440		// 1/Sqrt(2)
#define NF64_SQRT2_2	0.70710678118654752440		// Sqrt(2)/2 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Quick MACROS functions
#define NDEGtoRAD(deg)		((NF32_PI / 180.0f) * (deg))	// Deg -> Radian
#define NRADtoDEG(rad)		((180.0f / NF32_PI) * (rad))	// Radian -> Deg
#define NDEGtoANGLE(deg)	(	(Nu32)(	(deg)*( (Nf32)NANGLELUT_SIZE /  360.0f	)	)	)
#define NRADtoANGLE(rad)	(	(Nu32)( (rad)*( (Nf32)NANGLELUT_SIZE /NF32_2PI	)	)	)
#define NANGLEtoDEG(ang)	(	( (Nf32)(ang)*360.0f	)/ (Nf32)NANGLELUT_SIZE	)
#define NANGLEtoRAD(ang)	(	( (Nf32)(ang)*NF32_2PI )/ (Nf32)NANGLELUT_SIZE	)

#define NPOW2(a)				((a)*(a))			// a^2
#define NPOW3(a)				((a)*(a)*(a))		// a^3
#define NPOW4(a)				((a)*(a)*(a)*(a))	// a^4

#define NABS(a) (((a) < 0) ? -(a) : (a))							// Absolute value
#define	NMAX(a,b) (((a) > (b)) ? (a) : (b))							// Maximum between 2 values
#define	NMIN(a,b) (((a) < (b)) ? (a) : (b))							// Minimum between 2 values
//#define NCLAMP(_min,_val,_max)	( NMIN( NMAX(_min,_val),_max ) )	// Clamp a value against min,max values
#define NROUND(fval)			( ( (fval) >= 0.0f ) ? ((Ns32)((fval) + 0.5f)) : ((Ns32)((fval) - 0.5f)) )
#define NSIGN(a) (((a)<0) ? -1:1)									// Sign of a reel

#define NCLAMP(mn,a,mx)	( ((a)<(mn)) ? (mn) : ((a)>(mx)) ? (mx) : (a) )

#define NLERP(a,b,t)	( a + (b - a)*t )

// #define ZERO_IFNEG(a) (((a) < 0) ? 0	: (a))		// 0 if negative value
// #define ZERO_IFPOS(a) (((a) > 0) ?(a)   :  0 )		// 0 if positive value

#define NISODD(a) ((a)&1)							// Test if a number is a Odd number (2xn + 1 ) if not is Even (2xn) !

#define NBOOL(a)	(((a)==0) ? 0:1)				// return 1 if a is non zero, return 0 otherwise

// FUNCTIONS	
// void				NInitializeMathLUTs();

// Math
inline void			NCosSin(Nf32 angle_rad,Nf32 *cosangle,Nf32 *sinangle)
{
//*cosangle = cos(angle); *sinangle = sin(angle); // Temporaire. On doit ici trouver le code assembleur pour renvoyer Sin et cos en m�me temps !
/*
#ifdef _NWINDOWS
	__asm
	{
		fld			angle_rad
			fsincos
			mov			edx,cosangle
			mov			ecx,sinangle
			fstp		dword ptr [edx]
		fstp		dword ptr [ecx]
	}	
#endif
#ifdef _NIOS
    *cosangle=cosf(angle_rad);
    *sinangle=sinf(angle_rad);
#endif

#ifdef _NANDROID
	*cosangle=cosf(angle_rad);
	*sinangle=sinf(angle_rad);
#endif
*/
	/*
	__asm
	{
		fld			dword ptr [esp + 4]
		fsincos
		mov			edx, [esp + 8]
		mov			ecx, [esp + 12]
		fstp		dword ptr [edx]
		fstp		dword ptr [ecx]
	}
	*/
}

#define				NNEWTON_METHOD_MAX_ITER				32
Nf32				NNewtoncbrtf32(const Nf32 x, const Nf32 err);

inline Nf32			Ncbrtf32(const Nf32 x) {return (x < 0) ? -powf(-x, 1.0f / 3.0f) : powf(x, 1.0f / 3.0f); }
inline Nf64			Ncbrtf64(const Nf64 x) { return (x < 0) ? -pow(-x, 1.0 / 3.0) : pow(x, 1.0 / 3.0); }

inline Nbool		NIsPowerOfTwo(const Nu32 n){ return ( n && ( (n & (n - 1)) == 0 ) ); }
Nf32				NFastSqrt(const Nf32 f);
Nf64				NFastSqrt64(const Nf64 y);
/*
// TRIGONOMETRY
Nf32				NFastSin(const Nu32 a);
Nf32				NFastCos(const Nu32 a);
void				NFastCosSin(Nu32 a,Nf32* co,Nf32* si);
void				NFastVCosSin(Nu32 a,Nf32* cossin); // "cossin" is a pointer on 2 successive Nf32: cossin[0] and cossin[1]
*/
Nf64				NSincf64(const Nf64 a);
Nf32				NSinc(const Nf32 a);
/*
// RANDOMIZE
Nf32				NFastRand();
void				NFastRandRange(Nf32 *pfirst,const Nu32 size);
Nf32*				NGetFastRandRangePtr(const Nu32 size);
void				NRewindFastRandLUT(); // To replace FastRandLut headReader to index 0 (beginning of the table)
*/

/*
inline Nf32			NEllipsePerimeterRamanujanApprox(const Nf32 radius_a, const Nf32 radius_b){return  NF32_PI*(3.0f*(radius_a+radius_b)-NFastSqrt((3*radius_a+radius_b)*(radius_a+3*radius_b)));}
inline Nf32			NCirclePerimeter(const Nf32 radius){return NF32_2PI*radius;}		
*/

void				NFresnelIntegralsf32(const Nf32 s, Nf32 *cf, Nf32 *sf);
void				NFresnelIntegralsf64(const Nf64 s, Nf64 *cf, Nf64 *sf);

inline Nu32			NPowerOfTwoCeil(Nu32 x){ if (x <= 1) return 1;	Nu32 pw = 2; x--; while (x >>= 1) pw <<= 1;	return pw; }
inline Nu32			NPowerOfTwoFloor(Nu32 x) { Nu32 pw = 2; while (x >>= 1) pw <<= 1;	return pw; }
inline Nf32			NModulo2PIf32(Nf32 angle) { angle = fmod(angle, NF32_2PI); if (angle < 0.0f) { angle += NF32_2PI; }return angle; }
inline Nf64			NModulo2PIf64(Nf64 angle) { angle = fmod(angle, NF64_2PI); if (angle < 0.0) { angle += NF64_2PI; }return angle; }
/*
Nf32 modulo2PI(Nf32 a)
{
	Nu32 _continue;
	do
	{
		_continue = 0;

		if (a > NF32_2PI)
		{
			a -= NF32_2PI;
			_continue = 1;
		}
		else if (a < 0.0f)
		{
			a += NF32_2PI;
			_continue = 1;
		}
	} while (_continue);

	NErrorIf(_isnan(a), NERROR_VALUE_OUTOFRANGE);
	NErrorIf(a > NF32_2PI, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(a < 0.0f, NERROR_VALUE_OUTOFRANGE);

	return a;
}
 */

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NMATH_H 
*/
