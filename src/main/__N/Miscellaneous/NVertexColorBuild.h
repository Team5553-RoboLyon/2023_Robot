#ifndef __NVERTEXCOLOR_BUILD_H
#define __NVERTEXCOLOR_BUILD_H
// ***************************************************************************************
// **																					**
// **								NVertexColorBuild.h									**
// **																					**
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../NMatrix.h"
#include "../NScreenRect.h"
#include "../NScreenPoint.h"
#include "../Maths/NVec3f32.h"
#include "../NRectf32.h"
#include "../Core/NSafeConversion.h"
#include "../Maths/NMatrix3x3.h"
#include "../Geometry/NGeometry.h"
#include "../Miscellaneous/NColorGradient.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_NBUILDBASE_COLOR_COLORSETID_MAX		1	// the biggest ID
#define DEFAULT_NBUILDBASE_COLOR_COLORSETID				0
// Colors are common vertex components.
// To build them we need a range of color and some hints to use it in the desired way...
//		1/a specific vertex index and the vertex range size
//		2/a range of n color	
//		3/a color gradient ID value
//		4/a range of 2 flags set to ON or OFF 
// ... through these 2 functions:
//		
//		NCOLOR* NBuildVertexColor(NCOLOR *presult, const Nu32 vertexrange_index, const Nu32 vertexrange_size, const NCOLOR *pcolorange, const Nu32 colorangesize, const Nf32 *pcolorposition, const NCOLORGRADIENTID gradientid, const Nu32 vertexcolor_build_context_flags )
//		void	NGetBuildColorRange(NCOLOR **ppcolor_range, Nu32 *pcolor_rangesize, const NCOLOR *pdefault_color, const Nu32 vertexcolor_build_context_flags )
/*
#define FLAG_NVCOLBUILD_USEPARAMS_COLORRANGE		BIT_0
#define FLAG_NVCOLBUILD_USEPARAM_COLORGRADIENTID	BIT_1
#define MASK_NVCOLBUILD								(0x3)  //( BIT_0 to BIT_1 included )
*/
#define FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE		BIT_0
#define FLAG_NBUILDBASE_COLOR_USEPARAM_COLORGRADIENTID	BIT_1
#define FLAG_NBUILDBASE_COLOR_USEPARAM_COLORSETID		BIT_2

#define MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT	(FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE|FLAG_NBUILDBASE_COLOR_USEPARAM_COLORGRADIENTID)
#define MASK_NBUILDBASE_COLOR_COLORSET					FLAG_NBUILDBASE_COLOR_USEPARAM_COLORSETID
#define MASK_NBUILDBASE_COLOR_ALL_FLAGS					(MASK_NBUILDBASE_COLOR_COLORSET|MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT)

typedef struct  
{
	Nu16							Flags;
	Nu8								ColorSetID;
	Nu8								Available_Nu8;

	NCOLOR							*pColorRange;
	Nu32							ColorRangeSize;
	NCOLORGRADIENTID				ColorGradientID;
}NBUILDBASE_COLOR;
 
 
// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + N BASIC FUNCTIONS
// +
NBUILDBASE_COLOR*	NSetupBuildBaseColor(NBUILDBASE_COLOR *pbbcolor, const Nu16 flags, const Nu8 colorsetid, const NCOLOR *pcolorrange, const Nu32 colorangesize, const NCOLORGRADIENTID colorgradientid );
NBUILDBASE_COLOR*	NCreateBuildBaseColor(const Nu16 flags, const Nu8 colorsetid, const NCOLOR *pcolorrange, const Nu32 colorangesize, const NCOLORGRADIENTID colorgradientid );
void				NClearBuildBaseColor(NBUILDBASE_COLOR *pbbcolor);
void				NDeleteBuildBaseColor(NBUILDBASE_COLOR *pbbcolor);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + N LOW LEVEL (Private Internal Use)
// +
Nu8		NGetBuildBaseColorSetID(const NBUILDBASE_COLOR *pbbcolor );
NCOLOR* NGetBuildBaseColorVertexColor(NCOLOR *pcolor_result, const Nu32 vertexrange_index, const Nu32 vertexrange_size, const Nf32 *pcolorposition, const NBUILDBASE_COLOR *pvcolbuild );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NVERTEXCOLOR_BUILD_H 

