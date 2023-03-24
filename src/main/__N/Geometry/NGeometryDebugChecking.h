#ifdef _DEBUG
#ifndef __NGEOMETRYDEBUGCHECKING_H
#define __NGEOMETRYDEBUGCHECKING_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NGeometryDebugChecking.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

void NDebugCheckingGeometry_Point_TU0_V3_POINT(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_ColorPoint_TU0_V3C4_POINT(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_Line_TU0_V3_LINE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_ColorLine_TU0_V3C4_LINE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_TexturedLine_TUT1_V3C4T2_LINE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_FlatTriangle_TU0_V3_TRIANGLE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE(const NGEOMETRY *pgeom);
void NDebugCheckingGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom);
void NDebugCheckingGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NGEOMETRYDEBUGCHECKING_H 
#endif	// _DEBUG

