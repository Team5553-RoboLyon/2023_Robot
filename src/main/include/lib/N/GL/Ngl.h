#ifndef __NGL_H
#define __NGL_H

#ifdef _NWINDOWS
// GL libraries for WINDOWS
#include "extgl.h"
#include <GL/gl.h>
#endif

#ifdef _NIOS
// GL libraries for IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef _NANDROID
// GL libraries for ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

#include "../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#if defined _NIOS || defined _NANDROID
typedef float				  GLdouble;

#define glFrustum             glFrustumf
#define glTranslated          glTranslatef
#define glGetDoublev          glGetFloatv
#define glOrtho               glOrthof
#define GL_CLAMP			  GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_BORDER	  GL_CLAMP_TO_EDGE
#define GL_MIRRORED_REPEAT    GL_MIRRORED_REPEAT_OES
#define GL_FUNC_ADD           GL_FUNC_ADD_OES
#endif


#define CONSTANT_DEFAULT_CLEAR_COLOR4F	25.0f/255.0f, 56.0f/255.0f, 50.0f/255.0f, 0.0f

Nbool Ngl_Init();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NGL_H 
