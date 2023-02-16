#ifndef __NCCS_3DCOORD_H
#define __NCCS_3DCOORD_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NCCS_3DCoord.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"
#include "NCylindric.h"
#include "NSpheric.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// NCCS_3DCOORD is just an union of the 3 types (5 actually)  of 3D coordinates currently used in NENGINE
// Be careful, it's a Union, without any information to know which kind of coordinates it is.
// if you fill up a NCCS_3DCOORD like a Cartesian, use it like a Cartesian !!! Otherwise it could doing some weird things ...

typedef union
{
	NVEC3		Cartesian;
	
	NCYLINDRIC		Cylindric;
	NSPHERIC		Spheric;

	NFASTCYLINDRIC	FastCylindric;
	NFASTSPHERIC	FastSpheric;
}NCCS_3DCOORD;
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NCCS_3DCOORD_H 

