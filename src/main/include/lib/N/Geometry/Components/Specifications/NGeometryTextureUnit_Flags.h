#ifndef __NGEOMETRY_TEXTUREUNIT_FLAGS_H_
#define __NGEOMETRY_TEXTUREUNIT_FLAGS_H_


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NGeometryTextureUnit_Flags.h							**	
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../../NFlags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FLAG_NTEXTURE_UNIT_SPECS_xx are simple flags to describe NTEXTURE_UNIT_FORMAT content. 
// These FLAGS are combined with some others to create NVERTEX_DESC_xx DESCRIPTION values
#define FLAG_NTEXTURE_UNIT_SPECS_T			BIT_0	// ON: Texture Ptr.		OFF: No Texture Ptr
#define FLAG_NTEXTURE_UNIT_SPECS_M			BIT_1	// ON: Matrix Ptr.		OFF: No Matrix Ptr

// MASK_NTEXTURE_UNIT_SPECS_xx to isolate descriptions and perform right tests (are compositions from FLAG_NTEXTURE_UNIT_SPECS_xx too ... ).
#define MASK_NTEXTURE_UNIT_SPECS_TM			(FLAG_NTEXTURE_UNIT_SPECS_T|FLAG_NTEXTURE_UNIT_SPECS_M)

// FLAGS_NTEXTURE_UNIT_SPECS_xx are compositions from FLAG_NTEXTURE_UNIT_SPECS_xx.
// This first list contains the all the possibilities for each TextureUnit
#define FLAGS_NTEXTURE_UNIT_SPECS_NULL		0		
#define FLAGS_NTEXTURE_UNIT_SPECS_T			(FLAG_NTEXTURE_UNIT_SPECS_T)
#define FLAGS_NTEXTURE_UNIT_SPECS_TM		(FLAG_NTEXTURE_UNIT_SPECS_T|FLAG_NTEXTURE_UNIT_SPECS_M)

//	NIS_NTEXTURE_UNIT_SPECS_xx 
//	... to easy evaluate a Nu32 TextureUnitSpecs_Flags  !
//  These MACROS are evaluate to ZERO if NO and NON-ZERO if YES for the true/false ones
#define NCHECK_NTEXTURE_UNIT_SPECS_T(tuspecs_Nu32flags)			(tuspecs_Nu32flags&FLAG_NTEXTURE_UNIT_SPECS_T)
#define NCHECK_NTEXTURE_UNIT_SPECS_M(tuspecs_Nu32flags)			(tuspecs_Nu32flags&FLAG_NTEXTURE_UNIT_SPECS_M)
#define NCHECK_NTEXTURE_UNIT_SPECS_T_only(tuspecs_Nu32flags)	FLAGS_TEST(tuspecs_Nu32flags,MASK_NTEXTURE_UNIT_SPECS_TM,FLAGS_NTEXTURE_UNIT_SPECS_T)
#define NCHECK_NTEXTURE_UNIT_SPECS_TM(tuspecs_Nu32flags)		FLAGS_TEST(tuspecs_Nu32flags,MASK_NTEXTURE_UNIT_SPECS_TM,FLAGS_NTEXTURE_UNIT_SPECS_TM)

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRY_TEXTUREUNIT_FLAGS_H_

