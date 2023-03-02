#ifndef __NGEOMETRY_APPEARANCE_FLAGS_H
#define __NGEOMETRY_APPEARANCE_FLAGS_H



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryAppearance_Flags.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define FLAG_NAPPEARANCE_SPECS_TU_ULTD				BIT_0		   
#define FLAG_NAPPEARANCE_SPECS_TU_TEXTURE			BIT_1		   
#define FLAG_NAPPEARANCE_SPECS_TU_MATRIX			BIT_2		   
#define FLAG_NAPPEARANCE_SPECS_MATERIAL				BIT_3		   
#define FLAG_NAPPEARANCE_SPECS_BLEND				BIT_4		   
// ... All flags from BIT_5 until BIT_31 are AVAILABLE

// MASK_NAPPEARANCE_SPECS_xx to isolate descriptions and perform right tests
#define MASK_NAPPEARANCE_SPECS_BLEND_MATERIAL		(FLAG_NAPPEARANCE_SPECS_MATERIAL|FLAG_NAPPEARANCE_SPECS_BLEND)
#define MASK_NAPPEARANCE_SPECS_TU_CONFIG			(FLAG_NAPPEARANCE_SPECS_TU_ULTD|FLAG_NAPPEARANCE_SPECS_TU_TEXTURE|FLAG_NAPPEARANCE_SPECS_TU_MATRIX)
#define MASK_NAPPEARANCE_SPECS						(MASK_NAPPEARANCE_SPECS_TU_CONFIG|MASK_NAPPEARANCE_SPECS_BLEND_MATERIAL)

// FLAGS_NAPPEARANCE_SPECS_xx are compositions from FLAG_NMATERIAL_LAYOUT_xx. 
#define FLAGS_NAPPEARANCE_SPECS_NULL				0
#define FLAGS_NAPPEARANCE_SPECS_TT					(FLAG_NAPPEARANCE_SPECS_TU_TEXTURE|FLAG_NAPPEARANCE_SPECS_BLEND|FLAG_NAPPEARANCE_SPECS_MATERIAL)									
#define FLAGS_NAPPEARANCE_SPECS_TM					(FLAG_NAPPEARANCE_SPECS_TU_TEXTURE|FLAG_NAPPEARANCE_SPECS_TU_MATRIX|FLAG_NAPPEARANCE_SPECS_BLEND|FLAG_NAPPEARANCE_SPECS_MATERIAL)
#define FLAGS_NAPPEARANCE_SPECS_ULTD_T				(FLAG_NAPPEARANCE_SPECS_TU_TEXTURE|FLAG_NAPPEARANCE_SPECS_TU_ULTD|FLAG_NAPPEARANCE_SPECS_BLEND|FLAG_NAPPEARANCE_SPECS_MATERIAL)
#define FLAGS_NAPPEARANCE_SPECS_ULTD_TM				(FLAG_NAPPEARANCE_SPECS_TU_TEXTURE|FLAG_NAPPEARANCE_SPECS_TU_MATRIX|FLAG_NAPPEARANCE_SPECS_TU_ULTD|FLAG_NAPPEARANCE_SPECS_BLEND|FLAG_NAPPEARANCE_SPECS_MATERIAL)

//	NCHECK_APPEARANCE_SPECS_xx 
//	... to easy evaluate a Nu32 NAPPEARANCE_SPECS flags!
//  These MACROS are evaluate to ZERO if NO and NON-ZERO if YES 
#define NCHECK_APPEARANCE_SPECS_TU_NULL(appear_specs_nu32flags)					FLAGS_TEST(appear_specs_nu32flags,MASK_NAPPEARANCE_SPECS,0)
#define NCHECK_APPEARANCE_SPECS_TU_TEXTURE(appear_specs_nu32flags)				(appear_specs_nu32flags&FLAG_NAPPEARANCE_SPECS_TU_TEXTURE)
#define NCHECK_APPEARANCE_SPECS_TU_MATRIX(appear_specs_nu32flags)				(appear_specs_nu32flags&FLAG_NAPPEARANCE_SPECS_TU_MATRIX)
#define NCHECK_APPEARANCE_SPECS_TU_ULTD(appear_specs_nu32flags)					(appear_specs_nu32flags&FLAG_NAPPEARANCE_SPECS_TU_ULTD)
#define NCHECK_APPEARANCE_SPECS_MATERIAL(appear_specs_nu32flags)				(appear_specs_nu32flags&FLAG_NAPPEARANCE_SPECS_MATERIAL)
#define NCHECK_APPEARANCE_SPECS_BLEND(appear_specs_nu32flags)					(appear_specs_nu32flags&FLAG_NAPPEARANCE_SPECS_BLEND)

#define NCHECK_APPEARANCE_SPECS_TU_TT(appear_specs_nu32flags)					FLAGS_TEST(appear_specs_nu32flags,MASK_NAPPEARANCE_SPECS,FLAGS_NAPPEARANCE_SPECS_TT)
#define NCHECK_APPEARANCE_SPECS_TU_TM(appear_specs_nu32flags)					FLAGS_TEST(appear_specs_nu32flags,MASK_NAPPEARANCE_SPECS,FLAGS_NAPPEARANCE_SPECS_TM)
#define NCHECK_APPEARANCE_SPECS_TU_ULTD_T(appear_specs_nu32flags)				FLAGS_TEST(appear_specs_nu32flags,MASK_NAPPEARANCE_SPECS,FLAGS_NAPPEARANCE_SPECS_ULTD_T)
#define NCHECK_APPEARANCE_SPECS_TU_ULTD_TM(appear_specs_nu32flags)				FLAGS_TEST(appear_specs_nu32flags,MASK_NAPPEARANCE_SPECS,FLAGS_NAPPEARANCE_SPECS_ULTD_TM)

#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRY_APPEARANCE_FLAGS_H 

