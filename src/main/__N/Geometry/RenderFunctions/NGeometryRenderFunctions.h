#ifndef __NGEOMETRYRENDERFUNCTIONS_H
#define __NGEOMETRYRENDERFUNCTIONS_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NGeometryRenderFunctions.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NCStandard.h"
#include "../../NTransformationTree.h"
#include "../NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Render Function Name understanding:
// -----------------------------------
//
//			NRenderGeometry_#ExplicitName#_#TextureUnitFormat_and_Number#_#VertexFormat#_#PrimitiveFormat#
//							|			 | |							| |			   | |				 |				
//			NRenderGeometry_|   Part I   |_|         Part II            |_|  Part III  |_|   Part IV     |
//							|			 | | TU0  : No texture Unit		| |			   | |				 |				
//							|			 | | TUTn : n TextureUnit T		| |			   | |				 |				
//							|			 | | TUTMn: n TextureUnit TM	| |			   | |				 |				
// 
// Notice RenderFunction Name and NGEOMETRY_SPECS Name are close together. It's normal because there is a specific Render function
// for each Geometry Specs... 

// PRIMITIVE POINT
void NRenderGeometry_Point_TU0_V3_POINT(const NGEOMETRY* pgeom);
void NRenderGeometry_ColorPoint_TU0_V3C4_POINT(const NGEOMETRY* pgeom);

// PRIMITIVE NLINE
void NRenderGeometry_Line_TU0_V3_LINE(const NGEOMETRY *pgeom);
void NRenderGeometry_ColorLine_TU0_V3C4_LINE(const NGEOMETRY *pgeom);
void NRenderGeometry_TexturedLine_TUT1_V3C4T2_LINE(const NGEOMETRY* pgeom);

// PRIMITIVE NTRIANGLE
void NRenderGeometry_FlatTriangle_TU0_V3_TRIANGLE(const NGEOMETRY* pgeom);
void NRenderGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE(const NGEOMETRY* pgeom);
void NRenderGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom);

void NRenderGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom);
void NRenderGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NGEOMETRYRENDERFUNCTIONS_H 

