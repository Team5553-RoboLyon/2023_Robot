
#include "lib/N/NCStandard.h"
//#include "../GL/Ngl.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/NType.h"

#include "lib/N/Maths/NMath.h"

/*
// ==========================
// LUT
// ==========================
long						SqrtLUT[256];			// Fast SquareRoot Look-Up Table
Nf32						RandLUT[RANDLUT_SIZE];	// Fast Rand Look-Up Table
Nf32						CosLUT[NANGLELUT_SIZE];	// Fast Cosine Look-Up Table
Nf32						SinLUT[NANGLELUT_SIZE];	// Fast Sine Look-Up Table
*/

Nf32 NNewtoncbrtf32(const Nf32 x,const Nf32 err)
{
	if (!x)
		return 0.0f;
	Nf32 a = 1.0f;
	Nf32 local_err = NPOW3(a) - x;
	Nu32 iter = 0;

	while (1)
	{
		if ((NABS(local_err) < err) || (iter > NNEWTON_METHOD_MAX_ITER))
			return a;
		else
		{
			a = a - local_err / (3.0f*NPOW2(a));
			local_err = NPOW3(a) - x;
			iter++;
		}
	}

	/*
	for (Nu32 i = 0; i < 4; i++)
	{
		a = a - (NPOW3(a) - x) / (3.0f*NPOW2(a));
	}
	*/
	return a;
}

/*
// Function evaluated and slower than implemented Ncbrtf32(...) and NNewtoncbrtf32(...) functions
#define ITERATE
Nf32 NFastcbrtf32(const Nf32 x)
{
	float fr, r;
	int ex, shx;
	// Argument reduction
	fr = frexp(x, &ex);		// separate into mantissa and exponent
	shx = ex % 3;
	if (shx > 0)
		shx -= 3;			// compute shx such that (ex - shx) is divisible by 3
	ex = (ex - shx) / 3;	// exponent of cube root
	fr = ldexp(fr, shx);
	// 0.125 <= fr < 1.0
#ifdef ITERATE
// Compute seed with a quadratic qpproximation
	fr = (-0.46946116F * fr + 1.072302F) * fr + 0.3812513F;		// 0.5<=fr<1
	r = ldexp(fr, ex);											// 6 bits of precision
	// Newton-Raphson iterations
	r = (float)(2.0 / 3.0) * r + (float)(1.0 / 3.0) * x / (r * r); // 12 bits
	r = (float)(2.0 / 3.0) * r + (float)(1.0 / 3.0) * x / (r * r); // 24 bits
#else //ITERATE
// Use quartic rational polynomial with error < 2^(-24)
	fr = ((((45.2548339756803022511987494 * fr +
		192.2798368355061050458134625) * fr +
		119.1654824285581628956914143) * fr +
		13.43250139086239872172837314) * fr +
		0.1636161226585754240958355063)
		/
		((((14.80884093219134573786480845 * fr +
			151.9714051044435648658557668) * fr +
			168.5254414101568283957668343) * fr +
			33.9905941350215598754191872) * fr +
			1.0);
	r = ldexp(fr, ex); // 24 bits of precision
#endif
	return r;
}

*/

// ------------------------------------------------------------------------------------------
// NFastSqrt
// ------------------------------------------------------------------------------------------
// Description :
//	Not precise but Fast SquareRoot (Graphics Gems 1).
// ------------------------------------------------------------------------------------------
// In  :
//		f	: float number we want calculate the square root
//
// Out :
//		square root result.
//
// ------------------------------------------------------------------------------------------
Nf32 NFastSqrt(const Nf32 f)
{
	/*
	Ns32	e;
	Nu32	*num;

	num = (Nu32*)&f;

	if( f==0.0f )
		return 0.0f;

	e = (*num >> 23) - 127;

	*num &= 0x7fffff;
	if( e & 0x01 )
		*num |= 0x800000;

	e >>= 1;

	*num = ( (SqrtLUT[*num >> 16]) << 16) | ((e + 127) << 23);
	return f;
	*/
	return (Nf32)NFastSqrt64((Nf64)f);
}


// ------------------------------------------------------------------------------------------
// NFastSqrt
// ------------------------------------------------------------------------------------------
// Description :
//	http://www.azillionmonkeys.com/qed/sqroot.html
// 
//  !!! NEVER TESTED !!!
// ------------------------------------------------------------------------------------------
// In  :
//		y	: double we want calculate the square root
//
// Out :
//		square root result.
//	
// ------------------------------------------------------------------------------------------
Nf64 NFastSqrt64(const Nf64 y)
{
	double x, z, tempf;
	unsigned long* tfptr = ((unsigned long*)&tempf) + 1;

	tempf = y;
	*tfptr = (0xbfcdd90a - *tfptr) >> 1; /* estimate of 1/sqrt(y) */
	x = tempf;
	z = y * 0.5;                        /* hoist out the �/2�    */
	x = (1.5 * x) - (x * x) * (x * z);         /* iteration formula     */
	x = (1.5 * x) - (x * x) * (x * z);
	x = (1.5 * x) - (x * x) * (x * z);
	x = (1.5 * x) - (x * x) * (x * z);
	x = (1.5 * x) - (x * x) * (x * z);
	return x * y;
}

// ------------------------------------------------------------------------------------------
// FSinCos
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Sine and the Cosine.
//	
// ------------------------------------------------------------------------------------------
// In  :
//		angle		: the angle (rad).
//		cosangle	: Cosine.
//		sinangle	: Sine.
//
// Out :
//
// ------------------------------------------------------------------------------------------
/*
inline void NSinCos(float angle,float *cosangle,float *sinangle)
{
	__asm
	{
		fld			dword ptr [esp + 4]
		fsincos
		mov			edx, [esp + 8]
		mov			ecx, [esp + 12]
		fstp		dword ptr [edx]
		fstp		dword ptr [ecx]
	}
}
*/
// TODO 2 functions NSinCos ??????
/*
static __inline void NSinCos(float ang,float *cosangle,float *sinangle)
{
__asm
{
fld			ang
fsincos
mov			edx,cosangle
mov			ecx,sinangle
fstp		dword ptr [edx]
fstp		dword ptr [ecx]
}
}
*/
/*
// ------------------------------------------------------------------------------------------
// NFastRand
// ------------------------------------------------------------------------------------------
// Description :
//	Not precise but Fast SquareRoot (Graphics Gems 1).
// ------------------------------------------------------------------------------------------
// In  :
//		f	: float number we want calculate the square root
//
// Out :
//		square root result.
//
// ------------------------------------------------------------------------------------------
static Nu32	RandLUTIndex;
Nf32 NFastRand()
{
	RandLUTIndex = ( RandLUTIndex +1 ) & (RANDLUT_SIZE-1);
	return RandLUT[RandLUTIndex];
}
*/
/*
// ------------------------------------------------------------------------------------------
// void NFastRandRange
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			Nf32	*pfirst		A valid pointer on a Nf32 range.
//		const Nu32	size		size of the range. 
//								-->	Rule:	Size < RANDLUT_SIZE, if not it will be CRASH in debug mode ! 
//																 ... and will read unexpected memory in RELEASE mode.
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NFastRandRange(Nf32 *pfirst,const Nu32 size)
{
	NErrorIf(size>RANDLUT_SIZE-1,NERROR_FASTRAND_SIZE_OUTOFRANGE);
	// index of the first read element :  RandLUTIndex + 1
	// index of the last read element  : (RandLUTIndex + 1) + (size-1)
	// LUT max Index				   : RANDLUT_SIZE-1
	//
	// --> index of the last read element has to be, at maximum <= LUT max index
	// --> (RandLUTIndex + 1) + (size-1) <= RANDLUT_SIZE-1
	// -->        RandLUTIndex + size    <= RANDLUT_SIZE-1
	// all elements are positive integers, so
	// -->		  RandLUTIndex + size    <  RANDLUT_SIZE
	// --> Opposite case is:
	// -->		  RandLUTIndex + size    >=  RANDLUT_SIZE
	//					or
	// -->		  RandLUTIndex + size    >   RANDLUT_SIZE-1 which is better because RANDLUT_SIZE-1 is a constant !

	if( (RandLUTIndex+size) > (RANDLUT_SIZE-1) )
	{
		RandLUTIndex = 0;
	}
	memcpy(pfirst,&RandLUT[RandLUTIndex+1],size);
	RandLUTIndex += size;
}
*/
/*
// ------------------------------------------------------------------------------------------
// Nf32 const * NGetFastRandRangePtr 
// ------------------------------------------------------------------------------------------
// Description :
//	ADVANCED USER. To speed up your program, instead of calling "n successive times" the function
//	NFastRand, you can call 'NFastRandRange' to copy all the rand data you want in 1 call.
//	But, sometimes, you want manipulate each value before store it, so there is no benefit to call
//  'NFastRandRange'. So instead of calling "n successive times" the function NFastRand you may ask a pointer
//	on the LUT table, guarantee valid for the range you want and use it as a normal pointer n successive time !
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nf32* NGetFastRandRangePtr(const Nu32 size)
{
	NErrorIf(size>RANDLUT_SIZE-1,NERROR_FASTRAND_SIZE_OUTOFRANGE);
	// index of the first read element :  RandLUTIndex + 1
	// index of the last read element  : (RandLUTIndex + 1) + (size-1)
	// LUT max Index				   : RANDLUT_SIZE-1
	//
	// --> index of the last read element has to be, at maximum <= LUT max index
	// --> (RandLUTIndex + 1) + (size-1) <= RANDLUT_SIZE-1
	// -->        RandLUTIndex + size    <= RANDLUT_SIZE-1
	// all elements are positive integers, so
	// -->		  RandLUTIndex + size    <  RANDLUT_SIZE
	// --> Opposite case is:
	// -->		  RandLUTIndex + size    >=  RANDLUT_SIZE
	//					or
	// -->		  RandLUTIndex + size    >   RANDLUT_SIZE-1 which is better because RANDLUT_SIZE-1 is a constant !

	if( (RandLUTIndex+size) > (RANDLUT_SIZE-1) )
	{
		RandLUTIndex = 0;
	}
	Nf32 *ptr = &RandLUT[RandLUTIndex+1];
	RandLUTIndex += size;
	return ptr;
}


void NRewindFastRandLUT()
{
	RandLUTIndex =	0;
}

Nf32 NFastSin(const Nu32 a)
{
	return SinLUT[a & NANGLEMAX];
}

Nf32 NFastCos(const Nu32 a)
{
	return CosLUT[a & NANGLEMAX];
}

void NFastCosSin(Nu32 a,Nf32* co,Nf32* si)
{
	a &= NANGLEMAX;
	*co = CosLUT[a];
	*si = SinLUT[a];
}

void NFastVCosSin(Nu32 a,Nf32*cossin)
{
	a &= NANGLEMAX;
	cossin[0] = CosLUT[a];
	cossin[1] = SinLUT[a];
}

// ------------------------------------------------------------------------------------------
// CalcCosSinLUT
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Fast square Roor Look-Up Table
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
static void CalcCosSinLUT()
{
	Nu32	i;
	Nf32	fa;

	for(i=0;i<NANGLELUT_SIZE;i++)
	{
		fa = (360.0f*(Nf32)i)/NANGLELUT_SIZE;
		CosLUT[i] = cos(NDEGtoRAD(fa));
		SinLUT[i] = sin(NDEGtoRAD(fa));
	}
}

// ------------------------------------------------------------------------------------------
// CalcSqrtLUT
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Fast square Roor Look-Up Table
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
static void CalcSqrtLUT()
{
	Nu32	i;
	Nf32	f;
	Nu32	*fi;

	fi = (Nu32*)&f;

	for(i = 0; i <= 0x7f; i++)
	{
		*fi = 0;

		*fi = (i << 16) | (127 << 23);
		f = (Nf32)sqrt(f);

		SqrtLUT[i] = (*fi & 0x7fffff) >> 16;

		*fi = 0;
		*fi = (i << 16) | (128 << 23);
		f = (Nf32)sqrt(f);
		SqrtLUT[i + 0x80] = (*fi & 0x7fffff) >> 16;
	}
}

// ------------------------------------------------------------------------------------------
// CalcRandLUT
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Fast Rand Look-Up Table
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void CalcRandLUT()
{
	Nf32	*pval;
	Nf32	normalizer;
	Nu32	i;

	pval = RandLUT;
	normalizer = 1.0f/RAND_MAX;
	srand(13);// ;-)
	for( i=0;i<RANDLUT_SIZE;i++,pval++ )
	{
		*pval = (Nf32)rand()*normalizer;
	}
}

void NInitializeMathLUTs()
{
	CalcSqrtLUT();
	CalcRandLUT();
	CalcCosSinLUT();
}
*/
// ------------------------------------------------------------------------------------------
// NFresnelIntegralsf32
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Fresnel Integral C(s) and S(s) ( approximation )
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NFresnelIntegralsf32(const Nf32 s, Nf32 *cf, Nf32 *sf)
{
	Nf32 f = (1.0f + 0.926f*s) / (2.0f + 1.792f*s + 3.104f*s*s);
	Nf32 g = 1.0f / (2.0f + 4.142f*s + 3.492f*s*s + 6.670f*s*s*s);
	Nf32 an = (NF32_PI*s*s) / 2.0f;
	Nf32 cn = cos(an);
	Nf32 sn = sin(an);

	//*cf = 0.5f + f * sn - g * cn;
	//*sf = 0.5f - f * cn - g * sn;
	
	*cf = ( s > 0 )? (0.5f + f * sn - g * cn) : -(0.5f + f * sn - g * cn);
	*sf = ( s > 0 )? (0.5f - f * cn - g * sn) : -(0.5f - f * cn - g * sn);


}

// ------------------------------------------------------------------------------------------
// NFresnelIntegralsf64
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the Fresnel Integral C(x) and S(x) ( approximation )
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NFresnelIntegralsf64(const Nf64 s, Nf64 *cf, Nf64 *sf)
{
	Nf64 f = (1.0 + 0.926*s) / (2.0 + 1.792*s + 3.104*s*s);
	Nf64 g = 1.0 / (2.0 + 4.142*s + 3.492*s*s + 6.670*s*s*s);
	Nf64 an = (NF64_PI*s*s) / 2.0;
	Nf64 cn = cos(an);
	Nf64 sn = sin(an);

	//*cf = 0.5 + f * sn - g * cn;
	//*sf = 0.5 - f * cn - g * sn;

	*cf = (s > 0) ? (0.5 + f * sn - g * cn) : -(0.5 + f * sn - g * cn);
	*sf = (s > 0) ? (0.5 - f * cn - g * sn) : -(0.5 - f * cn - g * sn);
}

Nf64 NSincf64(const Nf64 a)
{
	if (fabs(a) < 1e-9)
	{
		return 1.0 - 1.0 / 6.0 * a * a;
	}
	else
	{
		return sin(a) / a;
	}
}

Nf32 NSinc(const Nf32 a)
{
	if (fabs(a) < 1e-9f)
	{
		return 1.0f - 1.0f / 6.0f * a * a;
	}
	else
	{
		return sinf(a) / a;
	}
}