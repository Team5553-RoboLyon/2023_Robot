#ifndef __NGEOMETRY_PRIMITIVE_FLAGS_H_
#define __NGEOMETRY_PRIMITIVE_FLAGS_H_


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NGeometryPrimitive_Flags.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../../NFlags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// FLAG_NPRIMITIVE_SPECS_xx are simple flags to describe NPRIMITIVE_FORMAT content. 
// These FLAGS are combined with some others to create FLAGS_NPRIMITIVE_SPECS_xx DESCRIPTION values

#define FLAG_NPRIMITIVE_SPECS_POINT			BIT_0	
#define FLAG_NPRIMITIVE_SPECS_LINE			BIT_1	
#define FLAG_NPRIMITIVE_SPECS_TRIANGLE		BIT_2	
#define FLAG_NPRIMITIVE_SPECS_FAN			BIT_3	
#define FLAG_NPRIMITIVE_SPECS_STRIP			BIT_4	
#define FLAG_NPRIMITIVE_SPECS_LOOP			BIT_5	
// ... All the other BITS, from 6 to 31 are available

// MASK_NPRIMITIVE_SPECS_xx
#define MASK_NPRIMITIVE_SPECS_PRIMITIVE					0x7		// Point/line/triangle
#define MASK_NPRIMITIVE_SPECS_SERIE						0x38	// Fan/Strip/Loop

// FLAGS_NPRIMITIVE_SPECS_xx are compositions from FLAG_NPRIMITIVE_DESC_xx.
#define FLAGS_NPRIMITIVE_SPECS_POINT					(FLAG_NPRIMITIVE_SPECS_POINT)
#define FLAGS_NPRIMITIVE_SPECS_LINE						(FLAG_NPRIMITIVE_SPECS_LINE)
#define FLAGS_NPRIMITIVE_SPECS_LINESTRIP				(FLAG_NPRIMITIVE_SPECS_LINE|FLAG_NPRIMITIVE_SPECS_STRIP)	
#define FLAGS_NPRIMITIVE_SPECS_LINELOOP					(FLAG_NPRIMITIVE_SPECS_LINE|FLAG_NPRIMITIVE_SPECS_LOOP)
#define FLAGS_NPRIMITIVE_SPECS_TRIANGLE					(FLAG_NPRIMITIVE_SPECS_TRIANGLE)
#define FLAGS_NPRIMITIVE_SPECS_TRIANGLESTRIP			(FLAG_NPRIMITIVE_SPECS_TRIANGLE|FLAG_NPRIMITIVE_SPECS_STRIP)
#define FLAGS_NPRIMITIVE_SPECS_TRIANGLEFAN				(FLAG_NPRIMITIVE_SPECS_TRIANGLE|FLAG_NPRIMITIVE_SPECS_FAN)


//	NCHECK_PRIMITIVE_SPECS_xx
//	... to easy evaluate a Nu32 Primitive_Flags value !

#define NCHECK_PRIMITIVE_SPECS_POINT(primspecs_Nu32flags)			FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE,FLAGS_NPRIMITIVE_SPECS_POINT)
#define NCHECK_PRIMITIVE_SPECS_LINE(primspecs_Nu32flags)			FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE,FLAGS_NPRIMITIVE_SPECS_LINE)
#define NCHECK_PRIMITIVE_SPECS_TRIANGLE(primspecs_Nu32flags)		FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE,FLAGS_NPRIMITIVE_SPECS_TRIANGLE)

#define NCHECK_PRIMITIVE_SPECS_LINESTRIP(primspecs_Nu32flags)		FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE|MASK_NPRIMITIVE_SPECS_SERIE,FLAGS_NPRIMITIVE_SPECS_LINESTRIP)
#define NCHECK_PRIMITIVE_SPECS_LINELOOP(primspecs_Nu32flags)		FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE|MASK_NPRIMITIVE_SPECS_SERIE,FLAGS_NPRIMITIVE_SPECS_LINELOOP)
#define NCHECK_PRIMITIVE_SPECS_TRIANGLESTRIP(primspecs_Nu32flags)	FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE|MASK_NPRIMITIVE_SPECS_SERIE,FLAGS_NPRIMITIVE_SPECS_TRIANGLESTRIP)
#define NCHECK_PRIMITIVE_SPECS_TRIANGLEFAN(primspecs_Nu32flags)		FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_PRIMITIVE|MASK_NPRIMITIVE_SPECS_SERIE,FLAGS_NPRIMITIVE_SPECS_TRIANGLEFAN)

#define NCHECK_PRIMITIVE_SPECS_STRIP(primspecs_Nu32flags)			FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_SERIE,FLAG_NPRIMITIVE_SPECS_STRIP)
#define NCHECK_PRIMITIVE_SPECS_FAN(primspecs_Nu32flags)				FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_SERIE,FLAG_NPRIMITIVE_SPECS_FAN)
#define NCHECK_PRIMITIVE_SPECS_LOOP(primspecs_Nu32flags)			FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_SERIE,FLAG_NPRIMITIVE_SPECS_LOOP)
#define NCHECK_PRIMITIVE_SPECS_NOSERIE(primspecs_Nu32flags)			FLAGS_TEST(primspecs_Nu32flags,MASK_NPRIMITIVE_SPECS_SERIE,0)



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRY_PRIMITIVE_FLAGS_H_

