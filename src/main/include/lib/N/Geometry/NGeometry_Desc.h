#ifndef __NGEOMETRY_DESC_H
#define __NGEOMETRY_DESC_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometry_Desc.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
#include "./Components/NGeometryComponent_Base.h"
#include "./Components/NGeometryComponent_Mesh.h"
*/

/*
// ------------------------------------------------------------------------------------------------------------------------------------------
// Purpose of NGEOMETRY_DESC is: allowing user to create all the different geometry structure. 
// The specific geometry creation functions are all used this structure as parameter.
// Until now, all the NGEOMETRY_Tx structures can be created with the description of the 2 main components BASE and MESH
typedef struct
{
	NGEOMETRY_COMPONENT_BASE_DESC		Base;
	NGEOMETRY_COMPONENT_MESH_DESC		Mesh;
}NGEOMETRY_DESC;
// ... This structure is going to evolve to integrate description parameter of the new geometry structures.
// ... and some new Component Description may appears like NGEOMETRY_COMPONENT_UVMATRIX_DESC.
// All the new parameters are going to be add through COMPONENT_DESC*/

// ***************************************************************************************
// **							Private	Functions										**
// ***************************************************************************************
/*
// PRIVATE !!!! LOW LEVEL
#define NMAKE_TEXUNIT_UVUNIT(texturenb,uvunitnb)	NMAKELONG(texturenb,uvunitnb)
#define NGET_TEXUNITNB(tuv)							NLOWORD(uv)
#define NGET_UVUNITNB(tuv)							NHIWORD(uv)
void NInitializeGeometryFunctions_LUT();
NGEOMETRY_RENDER_FUNCTION_ID_ENUM	NGetGeometryRenderFunctionID(NGEOMETRY_RFCT render_function);
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NGEOMETRY_H 

