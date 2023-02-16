#ifndef __NGEOMETRYLUTS_H
#define __NGEOMETRYLUTS_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								GeometryLuts.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "./RenderFunctions/NGeometryRenderFunctionsID.h"

typedef struct 
{
	NGEOMETRY_RENDERFCT		RenderFunction;
	Nu16					GeometryID;
	Nu16					VertexPrimitiveDetails;
	Nu32					VertexStrides;
}NGEOMETRY_INFOS;
void								NInitializeGeometryLUTs();
NGEOMETRY_RENDERFCT					NGetGeometryRenderFunctionByID(NGEOMETRY_RENDER_FUNCTION_ID_ENUM render_function_id);
NGEOMETRY_RENDER_FUNCTION_ID_ENUM	NGetGeometryRenderFunctionID(NGEOMETRY_RENDERFCT render_function);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYLUTS_H 

