#ifndef __NTYPE_H
#define __NTYPE_H

#include "lib/N/NCompilerPragma.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _NIOS
// typedef long long int       __int64;
#define __int64 long long int
	typedef unsigned long DWORD;
	typedef unsigned short WORD;
	typedef unsigned char NBYTE;
#endif

// On Android NULL is not defined
#ifndef NULL
#define NULL 0
#endif

// On ANDROID and IOS __int64 is not defined ...
#ifndef __int64
#ifdef _X64
#define __int64 long
#else // _X64
#define __int64 long long int
#endif // _X64
#endif // __int64

	// type redefinition
	typedef int Nint;

	typedef char Nchar;
	typedef unsigned char Nbool;
	typedef unsigned char NBYTE;

	typedef unsigned char Nu8;
	typedef unsigned short Nu16;
	typedef unsigned long Nu32;
	typedef unsigned __int64 Nu64;

	typedef signed char Ns8;
	typedef signed short Ns16;
	typedef signed long Ns32; // 'signed long' is a synonym of 'long'
	typedef signed __int64 Ns64;

	typedef float Nf32;
	typedef double Nf64;

// boolean
#define NTRUE 1
#define NFALSE 0

// a useful "VOID" value
#define NVOID 0xFFFFFFFF // = NU32_MAX

// WORD, LONG, NBYTE manipulations
// -------------------------------------------------------------------------------------------------
// Versions with bits masks... (are they really necessary here ? )
// ..........................................................
// #define NLOWORD(l) ( (Nu16)( ((Nu16)(l)) & 0xFFFF ) )
// #define NHIWORD(l) ( (Nu16)( (((Nu32)(l)) >> 16) & 0xFFFF) )
// #define NMAKELONG(lo,hi) ((Nu32) ((((Nu32)((Nu16)(lo))) & 0xFFFF)|( (((Nu32)((Nu16)(hi)))& 0xFFFF) << 16) ))
//
// #define NLOBYTE(w) (  (Nu8)( ((Nu8)(w)) & 0xFF )  )
// #define NHIBYTE(w) (  (Nu8)( (((Nu16)(w)) >> 8) & 0xFF )  )
// #define NMAKEWORD(hi,lo)( (Nu16) (( ((Nu16)((Nu8)(lo))) & 0xFF ) | ( ( ((Nu16)((Nu8)(hi))) & 0xFF ) << 8 )) )

// Versions without bits masks...
// ..........................................................
#define NLOWORD(l) ((l)&0xFFFF)
#define NHIWORD(l) (((l)&0xFFFF0000) >> 16)
#define NMAKELONG(lo, hi) (((lo)&0xFFFF) | (((hi)&0xFFFF) << 16)) // TODO: SWITCH (lo,hi) to (hi,lo)

#define NLOBYTE(w) ((w)&0xFF)
#define NHIBYTE(w) (((w)&0xFF00) >> 8)
#define NMAKEWORD(hi, lo) ((((hi)&0xFF) << 8) | ((lo)&0xFF))

#define NLONIBBLE(b) ((b)&0xF)
#define NHINIBBLE(b) (((b)&0xF0) >> 4)
#define NMAKEBYTE(hinibble, lonibble) ((((hinibble)&0xF) << 4) | ((lonibble)&0xF))

/*
// Old Syntax, for memory ...
//---------------------------
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define MAKELONG(lo,hi) ( (LONG) (((WORD) (low)) | ((DWORD) ((WORD) (hi))) << 16))
#define LOBYTE(w) ((NBYTE)(w))
#define HIBYTE(w) ((NBYTE)(((WORD)(w) >> 8) & 0xFF))
#define MAKELONG(loword, hiword) (loword & 0xffff) | ((hiword & 0xffff) << 16)
#define MAKEWORD(lobyte, hibyte)(lobyte & 0xff) | ((hibyte & 0xff) << 8)

// And some other syntaxes ...
//----------------------------
#define MAKEWORD(a, b)      ((WORD)(((NBYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((NBYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((NBYTE)(((DWORD_PTR)(w)) & 0xff ))
#define HIBYTE(w)           ((NBYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NTYPE_H
