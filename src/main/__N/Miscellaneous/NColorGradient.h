#ifndef __NCOLOR_GRADIENT_H
#define __NCOLOR_GRADIENT_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NColorGradient.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../Maths/NVec3f32.h"
#include "NColor.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Purpose of NColorGradient is to read (or extract) a color from a color range in a specific way.
// To define this way user has to MAKE a NCOLORGRADIENTID with the Macro "NMAKECOLORGRADIENTID"
// NMAKECOLORGRADIENTID has 3 params:
//		1)type
//		2)lerp
//		3)ang
//
//		1)type, is the type of color gradient to use. User needs to choose one type between the 8 of the 
//		NCOLORGRADIENT_TYPE_ENUM.
//		
//		2)lerp, is the mode of lerp using by color calculations. User needs to choose one mode between the 2 of the
//		NCOLORRANGE_LERP_MODE_ENUM.
//
//		3)ang, is an angle value into the NANGLE units. ang<=65535.
//		It will be used by some different NCOLOR GRADIENT TYPES and ignored by all the others.
//		In details:
//			NCOLORGRADIENT_TYPE_1D_LINEAR		'ang' ignored.
//			NCOLORGRADIENT_TYPE_2D_LINEAR		'ang' orients the color axis.
//			NCOLORGRADIENT_TYPE_2D_RADIAL		'ang' ignored.
//			NCOLORGRADIENT_TYPE_2D_ANGULAR		'ang' is the starting angle of the circular (360°) color range repartition.	
//			NCOLORGRADIENT_TYPE_2D_DIAMOND		'ang' ignored.
//			NCOLORGRADIENT_TYPE_2D_REFLECTED	'ang' orients the color axis.
//			NCOLORGRADIENT_TYPE_2D_4CORNERS		'ang' ignored.
//			NCOLORGRADIENT_TYPE_3D_CUBE			'ang' ignored.
//		
//		examples:
//			NMAKECOLORGRADIENTID(NCOLORGRADIENT_TYPE_2D_LINEAR,NCOLORRANGE_LERP_CLAMP,NANGLE45)
//				describe a linear color gradient, clamped oriented along an axis which makes an  angle of NANGLE45 (45°).
//
//			NMAKECOLORGRADIENTID(NCOLORGRADIENT_TYPE_RADIAL,NCOLORRANGE_LERP_REPEAT,0)
//				describe a radial color gradient, repeated. ang is not going to be used.
//
typedef enum
{
	NCOLORGRADIENT_TYPE_1D_LINEAR=0,
	NCOLORGRADIENT_TYPE_2D_LINEAR,
	NCOLORGRADIENT_TYPE_2D_RADIAL,
	NCOLORGRADIENT_TYPE_2D_ANGULAR,
	NCOLORGRADIENT_TYPE_2D_DIAMOND,
	NCOLORGRADIENT_TYPE_2D_REFLECTED,
	NCOLORGRADIENT_TYPE_2D_4CORNERS,
	NCOLORGRADIENT_TYPE_3D_CUBE,
	// -------------------------
	NCOLORGRADIENT_TYPE_ENUM_SIZE		// Max 256 with max type ID = 255. Because this ID is going to be packed on 8 bits inside the  "Color Gradient ID" 32 bits value.
}NCOLORGRADIENT_TYPE_ENUM;




//
//	NCOLORGRADIENTID
//
//	|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|
//	|31|30|29|28|.|27|26|25|24|.|23|22|21|20|.|19|18|17|16|.|15|14|13|12|.|11|10|09|08|.|07|06|05|04|.|03|02|01|00|
//	--------------------------------------------------------------- -----------------------------------------------
//	|	COLOR GRADIENT TYPE	  |.|		Available   | LERP|.|		 				ANGLE		                  |
//	|		  8 Bits		  |.|		 6 Bits     |2Bits|.|					 16 bis value					  |
//	--------------------------------------------------------------- -----------------------------------------------

typedef Nu32	NCOLORGRADIENTID;
#define NMAKECOLORGRADIENTID(type,lerp,ang)			( (((type)&0xFF)<<24)|(((lerp)&0x3)<<16)|((ang)&0xFFFF) )		
#define NGETCOLORGRADIENT_TYPE(cg)					(((cg)>>24)&0XFF)
#define NGETCOLORGRADIENT_LERP(cg)					(((cg)>>16)&0x3)
#define NGETCOLORGRADIENT_ANGLE(cg)					((cg)&0x0000FFFF)

// Some Presets:
#define NCOLORGRADIENTID_CLAMP_COLOR_LIST			NMAKECOLORGRADIENTID(NCOLORGRADIENT_TYPE_1D_LINEAR,NCOLORRANGE_LERP_CLAMP,0)
#define NCOLORGRADIENTID_REPEAT_COLOR_LIST			NMAKECOLORGRADIENTID(NCOLORGRADIENT_TYPE_1D_LINEAR,NCOLORRANGE_LERP_REPEAT,0)


NCOLOR* NExtractGradientColor(NCOLOR *presult, const NCOLOR *pcolorange, const Nu32 colorangesize, const Nf32 *pcolorposition, const NCOLORGRADIENTID gradientid );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NCOLOR_GRADIENT_H 

