#ifndef __NGEOMETRYBLEND_H
#define __NGEOMETRYBLEND_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								 NGeometryBlend.h									**
// **																					**
// ***************************************************************************************
#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NFlags.h"
#include "../../GL/Ngl.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Blend Params is a structure which store all the parameters used to Blend the geometry render result with the backgroung image.
// There are 2 Formats: Standard and Separate
typedef enum
{
	NBLEND_FORMAT_NULL = 0,		
	NBLEND_FORMAT_STANDARD,		// 1 set of 3 params used by "glBlendEquation" and "glBlendFunc"
	NBLEND_FORMAT_SEPARATE,		// 2 sets of 3 params used by "glBlendEquationSeparate" and "glBlendFuncSeparate"
	//NBLEND_FORMAT_AVAILABLE,	// ID available
	//----------------------	
	NBLEND_FORMAT_ENUM_SIZE			// MAX size is 4 !!! (and max ID is 3) This ID is going to be stored on a 2 bits value !!!
}NBLEND_FORMAT_ENUM;

// ------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// **							  Structures & Defines									**
// ***************************************************************************************
// Notes: a Material store all the lightning parameters
#define FLAG_NBLEND_SEPARATE		BIT_0	// ON means NENGINE is going to use the 6 Blend params trough "glBlend...Separate" functions.
											// OFF means NENGINE will only use the 3 first blend params trough "glBlend..." functions.
typedef struct
{
	Nchar	*pName;

	Nu32	Flags;				// 
	GLenum	Blend0_Equation;	// To Set up "glBlendEquation"	or "glBlendEquationSeparate" - Not available on OPENGL ES 1.1
	GLenum	Blend0_Sfactor;		// To Set up "glBlendFunc"		or "glBlendFuncSeparate"
	GLenum	Blend0_Dfactor;		// To Set up "glBlendFunc"		or "glBlendFuncSeparate"

	GLenum	Blend1_Equation;	// To Set up "glBlendEquationSeparate"						 - Not available on OPENGL ES 1.1
	GLenum	Blend1_Sfactor;		// To Set up "glBlendFuncSeparate"							 - Not available on OPENGL ES 1.1
	GLenum	Blend1_Dfactor;		// To Set up "glBlendFuncSeparate"							 - Not available on OPENGL ES 1.1
}NBLEND;

// GLenum	Blend_Equation Possible Values: 
// ---------------------------------------
// GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT

// GLenum	Blend_Sfactor Possible Values:
// ---------------------------------------
// GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
// GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, and GL_SRC_ALPHA_SATURATE

// GLenum	Blend_Dfactor Possible Values: 
// ---------------------------------------
// GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 
// GL_DST_ALPHA, and GL_ONE_MINUS_DST_ALPHA

// PRESETS to use with "NSetupBlend","NSetupBlend_Separate","NCreateBlend", "NCreateBlend_Separate"
#define NBLEND_PRESET_ALPHA					GL_FUNC_ADD,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA
#define NBLEND_PRESET_PREMUL_ALPHA			GL_FUNC_ADD,GL_ONE,GL_ONE_MINUS_SRC_ALPHA
#define NBLEND_PRESET_ADDITIVE				GL_FUNC_ADD,GL_ONE,GL_ONE
#define NBLEND_PRESET_MULTIPLY				GL_FUNC_ADD,GL_DST_COLOR,GL_ONE_MINUS_SRC_COLOR

#define NBLEND_PRESET_ALPHA_SEPARATE		GL_FUNC_ADD,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_FUNC_ADD,GL_ONE,GL_ZERO
#define NBLEND_PRESET_PREMUL_ALPHA_SEPARATE	GL_FUNC_ADD,GL_ONE,GL_ONE_MINUS_SRC_ALPHA,GL_FUNC_ADD,GL_ONE,GL_ZERO
#define NBLEND_PRESET_ADDITIVE_SEPARATE		GL_FUNC_ADD,GL_ONE,GL_ONE,GL_FUNC_ADD,GL_ONE,GL_ZERO
#define NBLEND_PRESET_MULTIPLY_SEPARATE		GL_FUNC_ADD,GL_SRC_COLOR,GL_ONE,GL_ONE_MINUS_SRC_COLOR,GL_ZERO

// File Extensions
#define EXTENSION_NBLEND			".nbl"	 
// File Signatures
#define SIGNATURE_NBLEND			NMAKESIGNATURE('N','B','L','D')

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NBLEND*			NSetupBlend(NBLEND *pblend, const Nchar *pname, const GLenum equation, const GLenum sfactor, const GLenum dfactor );
NBLEND*			NSetupBlend_Separate(NBLEND *pblend, const Nchar *pname, const GLenum equation0, const GLenum sfactor0, const GLenum dfactor0, const GLenum equation1, const GLenum sfactor1, const GLenum dfactor1 );
NBLEND*			NCreateBlend(const Nchar *pname, GLenum equation, GLenum sfactor, GLenum dfactor );
NBLEND*			NCreateBlend_Separate(const Nchar *pname, const GLenum equation0, const GLenum sfactor0, const GLenum dfactor0, const GLenum equation1, const GLenum sfactor1, const GLenum dfactor1 );
void			NClearBlend(NBLEND *pblend);
void			NDeleteBlend(NBLEND *pblend);

NBLEND*			NCreateBlendFromFile( const Nchar* pblendname, const Nchar* pfilename );
NBLEND*			NSaveBlend( const NBLEND *pblend, const Nchar* pfilename );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYBLEND_H 

