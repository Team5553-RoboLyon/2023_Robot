#pragma once
/*
#ifndef __NSAFECONVERSION_H
#define __NSAFECONVERSION_H
*/
#include "lib/N//NType.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#ifdef _DEBUG
// Don't use the functions ... Use the defines !
inline Nu32	_debug_safeNu32to4BitsValue(Nu32 val){NErrorIf(val>15,NERROR_4BITS_VALUE_OUTOFRANGE);return val;}
inline Nu8	_debug_safeNu32toNu8(Nu32 val){NErrorIf(val>NU8_MAX,NERROR_NU8_OUTOFRANGE);return (Nu8)val;}
inline Nu16	_debug_safeNu32toNu16(Nu32 val){NErrorIf(val>NU16_MAX,NERROR_NU16_OUTOFRANGE);return (Nu16)val;}

inline Nu32	_debug_safeNs32to4BitsValue(Ns32 val){NErrorIf(val>15 || val<0,NERROR_4BITS_VALUE_OUTOFRANGE);return val;}
inline Nu8	_debug_safeNs32toNu8(Ns32 val){NErrorIf(val>NU8_MAX || val<0,NERROR_NU8_OUTOFRANGE);return (Nu8)val;}
inline Nu16	_debug_safeNs32toNu16(Ns32 val){NErrorIf(val>NU16_MAX || val<0,NERROR_NU16_OUTOFRANGE);return (Nu16)val;}
inline Nu32	_debug_safeNs32toNu32(Ns32 val){NErrorIf(val<0,NERROR_NU32_NEGATIVE_VALUE_OUTOFRANGE);return (Nu32)val;}

inline Nu32	_debug_safeNinttoNs32(Nint val){NErrorIf(val<-NS32_MAX || val > NS32_MAX,NERROR_NS32_OUTOFRANGE);return (Ns32)val;}
inline Nu8	_debug_safeNinttoNu8(Nint val){NErrorIf(val>NU8_MAX || val<0,NERROR_NU8_OUTOFRANGE);return (Nu8)val;}

#define _SafeNu32To4BitsValue(Nu32_val)	_debug_safeNu32to4BitsValue( Nu32_val )
#define _SafeNu32ToNu8(Nu32_val)		_debug_safeNu32toNu8( Nu32_val )
#define _SafeNu32ToNu16(Nu32_val)		_debug_safeNu32toNu16( Nu32_val )

#define _SafeNs32To4BitsValue(Ns32_val)	_debug_safeNs32to4BitsValue( Ns32_val )
#define _SafeNs32ToNu8(Ns32_val)		_debug_safeNs32toNu8( Ns32_val )
#define _SafeNs32ToNu16(Ns32_val)		_debug_safeNs32toNu16( Ns32_val )
#define _SafeNs32ToNu32(Ns32_val)		_debug_safeNs32toNu32( Ns32_val )

#define _SafeNintToNs32(Nint_val)		_debug_safeNinttoNs32( Nint_val )
#define _SafeNintToNu8(Nint_val)		_debug_safeNinttoNu8( Nint_val )
#endif
*/


// #ifndef _DEBUG

#define _SafeNu32To4BitsValue(Nu32_val)	(Nu32_val)
#define _SafeNu32ToNu8(Nu32_val)		((Nu8)( Nu32_val ))
#define _SafeNu32ToNu16(Nu32_val)		((Nu16)( Nu32_val ))

#define _SafeNs32To4BitsValue(Ns32_val)	( Ns32_val )
#define _SafeNs32ToNu8(Ns32_val)		((Nu8)( Ns32_val ))
#define _SafeNs32ToNu16(Ns32_val)		((Nu16)( Ns32_val ))
#define _SafeNs32ToNu32(Ns32_val)		((Nu32)( Ns32_val ))

#define _SafeNintToNs32(Nint_val)		((Ns32)( Nint_val ))
#define _SafeNintToNu8(Nint_val)		((Nu8)( Nint_val ))

// #endif

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	//  __NSAFECONVERSION_H
*/
