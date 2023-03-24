#ifndef __NGEOMETRYSTRUCTURESID_H
#define __NGEOMETRYSTRUCTURESID_H
#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryStructuresID.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
typedef enum
{
	NGEOMETRY_ID_BM=0,			// Geometry "smallest" structure: Base+Mesh only, without Maps
	NGEOMETRY_ID_BMM,			// Geometry with the 3 components Base,Mesh and Maps
	NGEOMETRY_ID_BMMX,			// Geometry with 4 components Base,Mesh,Maps ans Extend
	//NGEOMETRY_ID_AVAILABLE,	// the last one ID available
	//----------------------	
	NGEOMETRY_ID_ENUM_SIZE		// MAX size is 4 !!! (and max ID is 3) This ID is going to be stored on a 2 bits value !!!
}NGEOMETRY_ID_ENUM;
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYSTRUCTURESID_H 

