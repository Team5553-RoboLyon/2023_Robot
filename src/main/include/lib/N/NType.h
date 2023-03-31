#pragma once

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


/*
// On Android NULL is not defined
#ifndef NULL
#define NULL    0
#endif

// On ANDROID and IOS __int64 is not defined ...
#ifndef __int64
	#ifdef _X64
		#define __int64		long
	#else // _X64
		#define __int64		long long int
	#endif // _X64
#endif // __int64
*/
#define __int64		long long int

// type redefinition
typedef	int					Nint;

typedef	char				Nchar;
//typedef	wchar_t				Nchar;

typedef	unsigned char		Nbool;
typedef unsigned char		NBYTE;

typedef unsigned char		Nu8;
typedef unsigned short		Nu16;
typedef unsigned long		Nu32;
typedef unsigned __int64	Nu64;

typedef signed char			Ns8;
typedef signed short		Ns16;
typedef signed long			Ns32; // 'signed long' is a synonym of 'long'
typedef signed __int64		Ns64;

typedef float				Nf32;
typedef double				Nf64;

// boolean
#define NTRUE				1
#define NFALSE				0

// a useful "VOID" value  
#define NVOID				0xFFFFFFFF	// = NU32_MAX


// Versions without bits masks... 
// ..........................................................
#define NLOWORD(l)						( (l)&0xFFFF )
#define NHIWORD(l)						( ((l)&0xFFFF0000)>>16	)
#define NMAKELONG(lo,hi)				( ((lo)&0xFFFF)|(((hi)&0xFFFF)<<16) ) // TODO: SWITCH (lo,hi) to (hi,lo)

#define NLOBYTE(w)						( (w)&0xFF )
#define NHIBYTE(w)						( ((w)&0xFF00) >> 8 )
#define NMAKEWORD(hi,lo)				( (((hi)&0xFF)<<8) | ((lo)&0xFF) )

#define NLONIBBLE(b)					( (b)&0xF )
#define NHINIBBLE(b)					( ((b)&0xF0)>>4 )
#define NMAKEBYTE(hinibble,lonibble)	( (((hinibble)&0xF)<<4) | ((lonibble)&0xF) )

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

