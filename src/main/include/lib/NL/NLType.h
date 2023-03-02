#pragma once

/*
#ifdef _NIOS
//typedef long long int     __int64;
#define __int64             long long int
	typedef unsigned long   DWORD;
	typedef unsigned short  WORD;
	typedef unsigned char   NBYTE;
#endif

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

// types redefinition
// -------------------
typedef	int					nlint;

typedef	char				nlchar;
typedef	unsigned char		nlbool;
typedef unsigned char		nlbyte;

typedef unsigned char		nlu8;
typedef unsigned short		nlu16;
typedef unsigned long		nlu32;
typedef unsigned __int64	nlu64;

typedef signed char			nls8;
typedef signed short		nls16;
typedef signed long			nls32; // 'signed long' is a synonym of 'long'
typedef signed __int64		nls64;

typedef float				nlf32;
typedef double				nlf64;


// a useful "VOID" value 
// ---------------------
#define NL_VOID				0xFFFFFFFF	// = NU32_MAX


// WORD, LONG, NBYTE manipulations
// ...............................
#define NL_LOWORD(l)					( (l)&0xFFFF )
#define NL_HIWORD(l)					( ((l)&0xFFFF0000)>>16	)
#define NL_MAKELONG(lo,hi)				( ((lo)&0xFFFF)|(((hi)&0xFFFF)<<16) )

#define NL_LOBYTE(w)					( (w)&0xFF )
#define NL_HIBYTE(w)					( ((w)&0xFF00) >> 8 )
#define NL_MAKEWORD(lo,hi)				( ((lo)&0xFF)|(((hi)&0xFF)<<8) )

#define NL_LONIBBLE(b)					( (b)&0xF )
#define NL_HINIBBLE(b)					( ((b)&0xF0)>>4 )
#define NL_MAKEBYTE(lonibble,hinibble)	( ((lonibble)&0xF) | (((hinibble)&0xF)<<4) )
