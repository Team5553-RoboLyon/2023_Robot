#pragma once


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NVersion.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------
// VERSIONNING RULES
// Given a version number MAJOR.MINOR.MICRO, increment the:
//	.MAJOR version when you make incompatible API changes,
//	.MINOR version when you add functionality in a backwards-compatible manner, and
//	.MICRO version when you make backwards-compatible bug fixes.
// ( Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.MICRO format.)
// ---------------------------------------------------------------------------------------------------------------------
#define NMAKEVERSION(_maj,_min,_mic)	( (Nu32)(((Nu32)_maj)<<16 | ((Nu32)_min)<<8 | ((Nu32)_mic)) )
#define	NGETVERSION_MAJOR(a)	( (((Nu32)a) >> 16) & 0xff )
#define	NGETVERSION_MINOR(a)	( (((Nu32)a) >> 8) & 0xff )
#define	NGETVERSION_MICRO(a)	( ((Nu32)a) & 0xff )
#define	NGETVERSION_MAIN(a)		( (((Nu32)a) >> 8) & 0xffff )						// 'Main version' contains 'Major' and 'Minor' without 'Micro'

#define NMAKESIGNATURE(a,b,c,d) ( (((Nu32)(d)) << 24) | (((Nu32)(c)) << 16) | (((Nu32)(b)) << 8) | ((Nu32)a) )
#define NSIGNATURECMP(a,b)		(((a) == (b)) ? 1 : 0)
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
#ifdef __cplusplus
}
#endif	// __cpluplus
*/

