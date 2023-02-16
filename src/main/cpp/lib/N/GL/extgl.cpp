/* ----------------------------------------------------------------------------
Copyright (c) 2001-2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/
/*
    Lev Povalahev

    levp@gmx.net

    http://www.levp.de/3d/

*/

/* VERSION 1.05 (see header for details) */

// #include <stdafx.h>
#include "windows.h"
#include <stdio.h>
#include <string.h>

#include "extgl.h"

#if !defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#include <GL/glx.h>
#endif /* _WIN32 */

/* turn off the warning for the borland compiler*/
#ifdef __BORLANDC__
#pragma warn -8064
#pragma warn -8065
#endif /* __BORLANDC__	*/

/* function variables */

/*-------------------------------------*/
/* WGL stuff */
/*-------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

/* WGL_ARB_buffer_region */

#ifdef WGL_ARB_buffer_region
wglCreateBufferRegionARBPROC wglCreateBufferRegionARB = NULL;
wglDeleteBufferRegionARBPROC wglDeleteBufferRegionARB = NULL;
wglSaveBufferRegionARBPROC wglSaveBufferRegionARB = NULL;
wglRestoreBufferRegionARBPROC wglRestoreBufferRegionARB = NULL;
#endif /* WGL_ARB_buffer_region */

/* WGL_ARB_extension_string */

#ifdef WGL_ARB_extensions_string
wglGetExtensionsStringARBPROC wglGetExtensionsStringARB = NULL;
#endif /* WGL_ARB_extension_string */

/* WGL_ARB_make_current_read */

#ifdef WGL_ARB_make_current_read
wglMakeContextCurrentARBPROC wglMakeContextCurrentARB = NULL;
wglGetCurrentReadDCARBPROC wglGetCurrentReadDCARB = NULL;
#endif /* WGL_ARB_make_current_read*/

/* WGL_ARB_pbuffer */

#ifdef WGL_ARB_pbuffer
wglCreatePbufferARBPROC wglCreatePbufferARB = NULL;
wglGetPbufferDCARBPROC wglGetPbufferDCARB = NULL;
wglReleasePbufferDCARBPROC wglReleasePbufferDCARB = NULL;
wglDestroyPbufferARBPROC wglDestroyPbufferARB = NULL;
wglQueryPbufferARBPROC wglQueryPbufferARB = NULL;
#endif /* WGL_ARB_pbuffer */

/* WGL_ARB_pixel_format */

#ifdef WGL_ARB_pixel_format
wglGetPixelFormatAttribivARBPROC wglGetPixelFormatAttribivARB = NULL;
wglGetPixelFormatAttribfvARBPROC wglGetPixelFormatAttribfvARB = NULL;
wglChoosePixelFormatARBPROC wglChoosePixelFormatARB = NULL;
#endif /* WGL_ARB_pixel_format */

/* WGL_ARB_render_texture */

#ifdef WGL_ARB_render_texture
wglBindTexImageARBPROC wglBindTexImageARB = NULL;
wglReleaseTexImageARBPROC wglReleaseTexImageARB = NULL;
wglSetPbufferAttribARBPROC wglSetPbufferAttribARB = NULL;
#endif /* WGL_ARB_render_texture */

/* WGL_EXT_display_color_table */

#ifdef WGL_EXT_display_color_table
wglCreateDisplayColorTableEXTPROC wglCreateDisplayColorTableEXT = NULL;
wglLoadDisplayColorTableEXTPROC wglLoadDisplayColorTableEXT = NULL;
wglBindDisplayColorTableEXTPROC wglBindDisplayColorTableEXT = NULL;
wglDestroyDisplayColorTableEXTPROC wglDestroyDisplayColorTableEXT = NULL;
#endif /* WGL_EXT_display_color_table */

/* WGL_EXT_extension_string */

#ifdef WGL_EXT_extensions_string
wglGetExtensionsStringEXTPROC wglGetExtensionsStringEXT = NULL;
#endif /* WGL_EXT_extension_string */

/* WGL_EXT_make_current_read */

#ifdef WGL_EXT_make_current_read
wglMakeContextCurrentEXTPROC wglMakeContextCurrentEXT = NULL;
wglGetCurrentReadDCEXTPROC wglGetCurrentReadDCEXT = NULL;
#endif /* WGL_EXT_make_current_read */

/* WGL_EXT_pbuffer */

#ifdef WGL_EXT_pbuffer
wglCreatePbufferEXTPROC wglCreatePbufferEXT = NULL;
wglGetPbufferDCEXTPROC wglGetPbufferDCEXT = NULL;
wglReleasePbufferDCEXTPROC wglReleasePbufferDCEXT = NULL;
wglDestroyPbufferEXTPROC wglDestroyPbufferEXT = NULL;
wglQueryPbufferEXTPROC wglQueryPbufferEXT = NULL;
#endif /* WGL_EXT_pbuffer */

/* WGL_EXT_pixel_format */

#ifdef WGL_EXT_pixel_format
wglGetPixelFormatAttribivEXTPROC wglGetPixelFormatAttribivEXT = NULL;
wglGetPixelFormatAttribfvEXTPROC wglGetPixelFormatAttribfvEXT = NULL;
wglChoosePixelFormatEXTPROC wglChoosePixelFormatEXT = NULL;
#endif /* WGL_EXT_pixel_format */

/* WGL_EXT_swap_control */

#ifdef WGL_EXT_swap_control
wglSwapIntervalEXTPROC wglSwapIntervalEXT = NULL;
wglGetSwapIntervalEXTPROC wglGetSwapIntervalEXT = NULL;
#endif /* WGL_EXT_swap_control */

/* WGL_I3D_digital_video_control */

#ifdef WGL_I3D_digital_video_control
wglGetDigitalVideoParametersI3DPROC wglGetDigitalVideoParametersI3D = NULL;
wglSetDigitalVideoParametersI3DPROC wglSetDigitalVideoParametersI3D = NULL;
#endif /* WGL_I3D_digital_video_control */

/* WGL_I3D_gamma */

#ifdef WGL_I3D_gamma
wglGetGammaTableParametersI3DPROC wglGetGammaTableParametersI3D = NULL;
wglSetGammaTableParametersI3DPROC wglSetGammaTableParametersI3D = NULL;
wglGetGammaTableI3DPROC wglGetGammaTableI3D = NULL;
wglSetGammaTableI3DPROC wglSetGammaTableI3D = NULL;
#endif /* WGL_I3D_gamma */

/* WGL_I3D_genlock */

#ifdef WGL_I3D_genlock
wglEnableGenlockI3DPROC wglEnableGenlockI3D = NULL;
wglDisableGenlockI3DPROC wglDisableGenlockI3D = NULL;
wglIsEnabledGenlockI3DPROC wglIsEnabledGenlockI3D = NULL;
wglGenlockSourceI3DPROC wglGenlockSourceI3D = NULL;
wglGetGenlockSourceI3DPROC wglGetGenlockSourceI3D = NULL;
wglGenlockSourceEdgeI3DPROC wglGenlockSourceEdgeI3D = NULL;
wglGetGenlockSourceEdgeI3DPROC wglGetGenlockSourceEdgeI3D = NULL;
wglGenlockSampleRateI3DPROC wglGenlockSampleRateI3D = NULL;
wglGetGenlockSampleRateI3DPROC wglGetGenlockSampleRateI3D = NULL;
wglGenlockSourceDelayI3DPROC wglGenlockSourceDelayI3D = NULL;
wglGetGenlockSourceDelayI3DPROC wglGetGenlockSourceDelayI3D = NULL;
wglQueryGenlockMaxSourceDelayI3DPROC wglQueryGenlockMaxSourceDelayI3D = NULL;
#endif /* WGL_I3D_genlock */

/* WGL_I3D_image_buffer */

#ifdef WGL_I3D_image_buffer
wglCreateImageBufferI3DPROC wglCreateImageBufferI3D = NULL;
wglDestroyImageBufferI3DPROC wglDestroyImageBufferI3D = NULL;
wglAssociateImageBufferEventsI3DPROC wglAssociateImageBufferEventsI3D = NULL;
wglReleaseImageBufferEventsI3DPROC wglReleaseImageBufferEventsI3D = NULL;
#endif /* WGL_I3D_image_buffer */

/* WGL_I3D_swap_frame_lock */

#ifdef WGL_I3D_swap_frame_lock
wglEnableFrameLockI3DPROC wglEnableFrameLockI3D = NULL;
wglDisableFrameLockI3DPROC wglDisableFrameLockI3D = NULL;
wglIsEnabledFrameLockI3DPROC wglIsEnabledFrameLockI3D = NULL;
wglQueryFrameLockMasterI3DPROC wglQueryFrameLockMasterI3D = NULL;
#endif /* WGL_I3D_swap_frame_lock */

/* WGL_I3D_swap_frame_usage */

#ifdef WGL_I3D_swap_frame_usage
wglGetFrameUsageI3DPROC wglGetFrameUsageI3D = NULL;
wglBeginFrameTrackingI3DPROC wglBeginFrameTrackingI3D = NULL;
wglEndFrameTrackingI3DPROC wglEndFrameTrackingI3D = NULL;
wglQueryFrameTrackingI3DPROC wglQueryFrameTrackingI3D = NULL;
#endif /* WGL_I3D_swap_frame_usage */

/* VAR */

#endif /* WIN32 */

/*-------------------------------------*/
/*---WGL STUFF END---------------------*/
/*-------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

/* GL_VERSION_1_2 */

#ifdef GL_VERSION_1_2
glDrawRangeElementsPROC glDrawRangeElements = NULL;
glTexImage3DPROC glTexImage3D = NULL;
glTexSubImage3DPROC glTexSubImage3D = NULL;
glCopyTexSubImage3DPROC glCopyTexSubImage3D = NULL;
#endif /* GL_VERSION_1_2 */

/* GL_VERSION_1_3 */

#ifdef GL_VERSION_1_3
glActiveTexturePROC glActiveTexture = NULL;
glClientActiveTexturePROC glClientActiveTexture = NULL;
glMultiTexCoord1dPROC glMultiTexCoord1d = NULL;
glMultiTexCoord1dvPROC glMultiTexCoord1dv = NULL;
glMultiTexCoord1fPROC glMultiTexCoord1f = NULL;
glMultiTexCoord1fvPROC glMultiTexCoord1fv = NULL;
glMultiTexCoord1iPROC glMultiTexCoord1i = NULL;
glMultiTexCoord1ivPROC glMultiTexCoord1iv = NULL;
glMultiTexCoord1sPROC glMultiTexCoord1s = NULL;
glMultiTexCoord1svPROC glMultiTexCoord1sv = NULL;
glMultiTexCoord2dPROC glMultiTexCoord2d = NULL;
glMultiTexCoord2dvPROC glMultiTexCoord2dv = NULL;
glMultiTexCoord2fPROC glMultiTexCoord2f = NULL;
glMultiTexCoord2fvPROC glMultiTexCoord2fv = NULL;
glMultiTexCoord2iPROC glMultiTexCoord2i = NULL;
glMultiTexCoord2ivPROC glMultiTexCoord2iv = NULL;
glMultiTexCoord2sPROC glMultiTexCoord2s = NULL;
glMultiTexCoord2svPROC glMultiTexCoord2sv = NULL;
glMultiTexCoord3dPROC glMultiTexCoord3d = NULL;
glMultiTexCoord3dvPROC glMultiTexCoord3dv = NULL;
glMultiTexCoord3fPROC glMultiTexCoord3f = NULL;
glMultiTexCoord3fvPROC glMultiTexCoord3fv = NULL;
glMultiTexCoord3iPROC glMultiTexCoord3i = NULL;
glMultiTexCoord3ivPROC glMultiTexCoord3iv = NULL;
glMultiTexCoord3sPROC glMultiTexCoord3s = NULL;
glMultiTexCoord3svPROC glMultiTexCoord3sv = NULL;
glMultiTexCoord4dPROC glMultiTexCoord4d = NULL;
glMultiTexCoord4dvPROC glMultiTexCoord4dv = NULL;
glMultiTexCoord4fPROC glMultiTexCoord4f = NULL;
glMultiTexCoord4fvPROC glMultiTexCoord4fv = NULL;
glMultiTexCoord4iPROC glMultiTexCoord4i = NULL;
glMultiTexCoord4ivPROC glMultiTexCoord4iv = NULL;
glMultiTexCoord4sPROC glMultiTexCoord4s = NULL;
glMultiTexCoord4svPROC glMultiTexCoord4sv = NULL;
glLoadTransposeMatrixfPROC glLoadTransposeMatrixf = NULL;
glLoadTransposeMatrixdPROC glLoadTransposeMatrixd = NULL;
glMultTransposeMatrixfPROC glMultTransposeMatrixf = NULL;
glMultTransposeMatrixdPROC glMultTransposeMatrixd = NULL;
glCompressedTexImage3DPROC glCompressedTexImage3D = NULL;
glCompressedTexImage2DPROC glCompressedTexImage2D = NULL;
glCompressedTexImage1DPROC glCompressedTexImage1D = NULL;
glCompressedTexSubImage3DPROC glCompressedTexSubImage3D = NULL;
glCompressedTexSubImage2DPROC glCompressedTexSubImage2D = NULL;
glCompressedTexSubImage1DPROC glCompressedTexSubImage1D = NULL;
glGetCompressedTexImagePROC glGetCompressedTexImage = NULL;
glSampleCoveragePROC glSampleCoverage = NULL;
#endif /* GL_VERSION_1_3 */

/* GL_VERSION_1_4 */

#ifdef GL_VERSION_1_4
/*#ifndef GL_VERSION_1_2
glBlendColorPROC glBlendColor = NULL;
glBlendEquationPROC glBlendEquation = NULL;
#endif *//* GL_VERSION_1_2 */
glFogCoordfPROC glFogCoordf = NULL;
glFogCoordfvPROC glFogCoordfv = NULL;
glFogCoorddPROC glFogCoordd = NULL;
glFogCoorddvPROC glFogCoorddv = NULL;
glFogCoordPointerPROC glFogCoordPointer = NULL;
glMultiDrawArraysPROC glMultiDrawArrays = NULL;
glMultiDrawElementsPROC glMultiDrawElements = NULL;
glPointParameterfPROC glPointParameterf = NULL;
glPointParameterfvPROC glPointParameterfv = NULL;
glSecondaryColor3bPROC glSecondaryColor3b = NULL;
glSecondaryColor3bvPROC glSecondaryColor3bv = NULL;
glSecondaryColor3dPROC glSecondaryColor3d = NULL;
glSecondaryColor3dvPROC glSecondaryColor3dv = NULL;
glSecondaryColor3fPROC glSecondaryColor3f = NULL;
glSecondaryColor3fvPROC glSecondaryColor3fv = NULL;
glSecondaryColor3iPROC glSecondaryColor3i = NULL;
glSecondaryColor3ivPROC glSecondaryColor3iv = NULL;
glSecondaryColor3sPROC glSecondaryColor3s = NULL;
glSecondaryColor3svPROC glSecondaryColor3sv = NULL;
glSecondaryColor3ubPROC glSecondaryColor3ub = NULL;
glSecondaryColor3ubvPROC glSecondaryColor3ubv = NULL;
glSecondaryColor3uiPROC glSecondaryColor3ui = NULL;
glSecondaryColor3uivPROC glSecondaryColor3uiv = NULL;
glSecondaryColor3usPROC glSecondaryColor3us = NULL;
glSecondaryColor3usvPROC glSecondaryColor3usv = NULL;
glSecondaryColorPointerPROC glSecondaryColorPointer = NULL;
glBlendFuncSeparatePROC glBlendFuncSeparate = NULL;
glWindowPos2dPROC glWindowPos2d = NULL;
glWindowPos2fPROC glWindowPos2f = NULL;
glWindowPos2iPROC glWindowPos2i = NULL;
glWindowPos2sPROC glWindowPos2s = NULL;
glWindowPos2dvPROC glWindowPos2dv = NULL;
glWindowPos2fvPROC glWindowPos2fv = NULL;
glWindowPos2ivPROC glWindowPos2iv = NULL;
glWindowPos2svPROC glWindowPos2sv = NULL;
glWindowPos3dPROC glWindowPos3d = NULL;
glWindowPos3fPROC glWindowPos3f = NULL;
glWindowPos3iPROC glWindowPos3i = NULL;
glWindowPos3sPROC glWindowPos3s = NULL;
glWindowPos3dvPROC glWindowPos3dv = NULL;
glWindowPos3fvPROC glWindowPos3fv = NULL;
glWindowPos3ivPROC glWindowPos3iv = NULL;
glWindowPos3svPROC glWindowPos3sv = NULL;
#endif /* GL_VERSION_1_4 */

#ifdef GL_VERSION_1_5
glGenQueriesPROC glGenQueries = NULL;
glDeleteQueriesPROC glDeleteQueries = NULL;
glIsQueryPROC glIsQuery = NULL;
glBeginQueryPROC glBeginQuery = NULL;
glEndQueryPROC glEndQuery = NULL;
glGetQueryivPROC glGetQueryiv = NULL;
glGetQueryObjectivPROC glGetQueryObjectiv = NULL;
glGetQueryObjectuivPROC glGetQueryObjectuiv = NULL;
glBindBufferPROC glBindBuffer = NULL;
glDeleteBuffersPROC glDeleteBuffers = NULL;
glGenBuffersPROC glGenBuffers = NULL;
glIsBufferPROC glIsBuffer = NULL;
glBufferDataPROC glBufferData = NULL;
glBufferSubDataPROC glBufferSubData = NULL;
glGetBufferSubDataPROC glGetBufferSubData = NULL;
glMapBufferPROC glMapBuffer = NULL;
glUnmapBufferPROC glUnmapBuffer = NULL;
glGetBufferParameterivPROC glGetBufferParameteriv = NULL;
glGetBufferPointervPROC glGetBufferPointerv = NULL;
#endif /* GL_VERSION_1_5 */

#ifdef GL_VERSION_2_0
glBlendEquationSeparatePROC glBlendEquationSeparate = NULL;
glDrawBuffersPROC glDrawBuffers = NULL;
glStencilOpSeparatePROC glStencilOpSeparate = NULL;
glStencilFuncSeparatePROC glStencilFuncSeparate = NULL;
glStencilMaskSeparatePROC glStencilMaskSeparate = NULL;
glAttachShaderPROC glAttachShader = NULL;
glBindAttribLocationPROC glBindAttribLocation = NULL;
glCompileShaderPROC glCompileShader = NULL;
glCreateProgramPROC glCreateProgram = NULL;
glCreateShaderPROC glCreateShader = NULL;
glDeleteProgramPROC glDeleteProgram = NULL;
glDeleteShaderPROC glDeleteShader = NULL;
glDetachShaderPROC glDetachShader = NULL;
glDisableVertexAttribArrayPROC glDisableVertexAttribArray = NULL;
glEnableVertexAttribArrayPROC glEnableVertexAttribArray = NULL;
glGetActiveAttribPROC glGetActiveAttrib = NULL;
glGetActiveUniformPROC glGetActiveUniform = NULL;
glGetAttachedShadersPROC glGetAttachedShaders = NULL;
glGetAttribLocationPROC glGetAttribLocation = NULL;
glGetProgramivPROC glGetProgramiv = NULL;
glGetProgramInfoLogPROC glGetProgramInfoLog = NULL;
glGetShaderivPROC glGetShaderiv = NULL;
glGetShaderInfoLogPROC glGetShaderInfoLog = NULL;
glGetShaderSourcePROC glGetShaderSource = NULL;
glGetUniformLocationPROC glGetUniformLocation = NULL;
glGetUniformfvPROC glGetUniformfv = NULL;
glGetUniformivPROC glGetUniformiv = NULL;
glGetVertexAttribdvPROC glGetVertexAttribdv = NULL;
glGetVertexAttribfvPROC glGetVertexAttribfv = NULL;
glGetVertexAttribivPROC glGetVertexAttribiv = NULL;
glGetVertexAttribPointervPROC glGetVertexAttribPointerv = NULL;
glIsProgramPROC glIsProgram = NULL;
glIsShaderPROC glIsShader = NULL;
glLinkProgramPROC glLinkProgram = NULL;
glShaderSourcePROC glShaderSource = NULL;
glUseProgramPROC glUseProgram = NULL;
glUniform1fPROC glUniform1f = NULL;
glUniform2fPROC glUniform2f = NULL;
glUniform3fPROC glUniform3f = NULL;
glUniform4fPROC glUniform4f = NULL;
glUniform1iPROC glUniform1i = NULL;
glUniform2iPROC glUniform2i = NULL;
glUniform3iPROC glUniform3i = NULL;
glUniform4iPROC glUniform4i = NULL;
glUniform1fvPROC glUniform1fv = NULL;
glUniform2fvPROC glUniform2fv = NULL;
glUniform3fvPROC glUniform3fv = NULL;
glUniform4fvPROC glUniform4fv = NULL;
glUniform1ivPROC glUniform1iv = NULL;
glUniform2ivPROC glUniform2iv = NULL;
glUniform3ivPROC glUniform3iv = NULL;
glUniform4ivPROC glUniform4iv = NULL;
glUniformMatrix2fvPROC glUniformMatrix2fv = NULL;
glUniformMatrix3fvPROC glUniformMatrix3fv = NULL;
glUniformMatrix4fvPROC glUniformMatrix4fv = NULL;
glValidateProgramPROC glValidateProgram = NULL;
glVertexAttrib1dPROC glVertexAttrib1d = NULL;
glVertexAttrib1dvPROC glVertexAttrib1dv = NULL;
glVertexAttrib1fPROC glVertexAttrib1f = NULL;
glVertexAttrib1fvPROC glVertexAttrib1fv = NULL;
glVertexAttrib1sPROC glVertexAttrib1s = NULL;
glVertexAttrib1svPROC glVertexAttrib1sv = NULL;
glVertexAttrib2dPROC glVertexAttrib2d = NULL;
glVertexAttrib2dvPROC glVertexAttrib2dv = NULL;
glVertexAttrib2fPROC glVertexAttrib2f = NULL;
glVertexAttrib2fvPROC glVertexAttrib2fv = NULL;
glVertexAttrib2sPROC glVertexAttrib2s = NULL;
glVertexAttrib2svPROC glVertexAttrib2sv = NULL;
glVertexAttrib3dPROC glVertexAttrib3d = NULL;
glVertexAttrib3dvPROC glVertexAttrib3dv = NULL;
glVertexAttrib3fPROC glVertexAttrib3f = NULL;
glVertexAttrib3fvPROC glVertexAttrib3fv = NULL;
glVertexAttrib3sPROC glVertexAttrib3s = NULL;
glVertexAttrib3svPROC glVertexAttrib3sv = NULL;
glVertexAttrib4NbvPROC glVertexAttrib4Nbv = NULL;
glVertexAttrib4NivPROC glVertexAttrib4Niv = NULL;
glVertexAttrib4NsvPROC glVertexAttrib4Nsv = NULL;
glVertexAttrib4NubPROC glVertexAttrib4Nub = NULL;
glVertexAttrib4NubvPROC glVertexAttrib4Nubv = NULL;
glVertexAttrib4NuivPROC glVertexAttrib4Nuiv = NULL;
glVertexAttrib4NusvPROC glVertexAttrib4Nusv = NULL;
glVertexAttrib4bvPROC glVertexAttrib4bv = NULL;
glVertexAttrib4dPROC glVertexAttrib4d = NULL;
glVertexAttrib4dvPROC glVertexAttrib4dv = NULL;
glVertexAttrib4fPROC glVertexAttrib4f = NULL;
glVertexAttrib4fvPROC glVertexAttrib4fv = NULL;
glVertexAttrib4ivPROC glVertexAttrib4iv = NULL;
glVertexAttrib4sPROC glVertexAttrib4s = NULL;
glVertexAttrib4svPROC glVertexAttrib4sv = NULL;
glVertexAttrib4ubvPROC glVertexAttrib4ubv = NULL;
glVertexAttrib4uivPROC glVertexAttrib4uiv = NULL;
glVertexAttrib4usvPROC glVertexAttrib4usv = NULL;
glVertexAttribPointerPROC glVertexAttribPointer = NULL;
#endif /* GL_VERSION_2_0 */

#endif /* WIN32 */

/* GL_APPLE_element_array */

#ifdef GL_APPLE_element_array
glElementPointerAPPLEPROC glElementPointerAPPLE = NULL;
glDrawElementArrayAPPLEPROC glDrawElementArrayAPPLE = NULL;
glDrawRangeElementArrayAPPLEPROC glDrawRangeElementArrayAPPLE = NULL;
glMultiDrawElementArrayAPPLEPROC glMultiDrawElementArrayAPPLE = NULL;
glMultiDrawRangeElementArrayAPPLEPROC glMultiDrawRangeElementArrayAPPLE = NULL;
#endif /* GL_APPLE_element_array */

/* GL_APPLE_fence */

#ifdef GL_APPLE_fence
glGenFencesAPPLEPROC glGenFencesAPPLE = NULL;
glDeleteFencesAPPLEPROC glDeleteFencesAPPLE = NULL;
glSetFenceAPPLEPROC glSetFenceAPPLE = NULL;
glIsFenceAPPLEPROC glIsFenceAPPLE = NULL;
glTestFenceAPPLEPROC glTestFenceAPPLE = NULL;
glFinishFenceAPPLEPROC glFinishFenceAPPLE = NULL;
glTestObjectAPPLEPROC glTestObjectAPPLE = NULL;
glFinishObjectAPPLEPROC glFinishObjectAPPLE = NULL;
#endif /* GL_APPLE_fence */

/* GL_APPLE_vertex_array_object */

#ifdef GL_APPLE_vertex_array_object
glBindVertexArrayAPPLEPROC glBindVertexArrayAPPLE = NULL;
glDeleteVertexArraysAPPLEPROC glDeleteVertexArraysAPPLE = NULL;
glGenVertexArraysAPPLEPROC glGenVertexArraysAPPLE = NULL;
glIsVertexArrayAPPLEPROC glIsVertexArrayAPPLE = NULL;
#endif /* GL_APPLE_vertex_array_object */

/* GL_APPLE_vertex_array_range */

#ifdef GL_APPLE_vertex_array_range
glVertexArrayRangeAPPLEPROC glVertexArrayRangeAPPLE = NULL;
glFlushVertexArrayRangeAPPLEPROC glFlushVertexArrayRangeAPPLE = NULL;
glVertexArrayParameteriAPPLEPROC glVertexArrayParameteriAPPLE = NULL;
#endif /* GL_APPLE_vertex_array_range */

/* GL_ARB_imaging */

#ifdef GL_ARB_imaging
glBlendColorPROC glBlendColor = NULL;
glBlendEquationPROC glBlendEquation = NULL;
glColorTablePROC glColorTable = NULL;
glColorTableParameterfvPROC glColorTableParameterfv = NULL;
glColorTableParameterivPROC glColorTableParameteriv = NULL;
glCopyColorTablePROC glCopyColorTable = NULL;
glGetColorTablePROC glGetColorTable = NULL;
glGetColorTableParameterfvPROC glGetColorTableParameterfv = NULL;
glGetColorTableParameterivPROC glGetColorTableParameteriv = NULL;
glColorSubTablePROC glColorSubTable = NULL;
glCopyColorSubTablePROC glCopyColorSubTable = NULL;
glConvolutionFilter1DPROC glConvolutionFilter1D = NULL;
glConvolutionFilter2DPROC glConvolutionFilter2D = NULL;
glConvolutionParameterfPROC glConvolutionParameterf = NULL;
glConvolutionParameterfvPROC glConvolutionParameterfv = NULL;
glConvolutionParameteriPROC glConvolutionParameteri = NULL;
glConvolutionParameterivPROC glConvolutionParameteriv = NULL;
glCopyConvolutionFilter1DPROC glCopyConvolutionFilter1D = NULL;
glCopyConvolutionFilter2DPROC glCopyConvolutionFilter2D = NULL;
glGetConvolutionFilterPROC glGetConvolutionFilter = NULL;
glGetConvolutionParameterfvPROC glGetConvolutionParameterfv = NULL;
glGetConvolutionParameterivPROC glGetConvolutionParameteriv = NULL;
glGetSeparableFilterPROC glGetSeparableFilter = NULL;
glSeparableFilter2DPROC glSeparableFilter2D = NULL;
glGetHistogramPROC glGetHistogram = NULL;
glGetHistogramParameterfvPROC glGetHistogramParameterfv = NULL;
glGetHistogramParameterivPROC glGetHistogramParameteriv = NULL;
glGetMinmaxPROC glGetMinmax = NULL;
glGetMinmaxParameterfvPROC glGetMinmaxParameterfv = NULL;
glGetMinmaxParameterivPROC glGetMinmaxParameteriv = NULL;
glHistogramPROC glHistogram = NULL;
glMinmaxPROC glMinmax = NULL;
glResetHistogramPROC glResetHistogram = NULL;
glResetMinmaxPROC glResetMinmax = NULL;
#endif /* GL_ARB_imaging */

/* GL_ARB_matrix_palette */

#ifdef GL_ARB_matrix_palette
glCurrentPaletteMatrixARBPROC glCurrentPaletteMatrixARB = NULL;
glMatrixIndexubvARBPROC glMatrixIndexubvARB = NULL;
glMatrixIndexusvARBPROC glMatrixIndexusvARB = NULL;
glMatrixIndexuivARBPROC glMatrixIndexuivARB = NULL;
glMatrixIndexPointerARBPROC glMatrixIndexPointerARB = NULL;
#endif /* GL_ARB_matrix_palette */

/* GL_ARB_multisample */

#ifdef GL_ARB_multisample
glSampleCoverageARBPROC glSampleCoverageARB = NULL;
#endif /* GL_ARB_multisample */

/* GL_ARB_multitexture */

#ifdef GL_ARB_multitexture
glActiveTextureARBPROC glActiveTextureARB = NULL;
glClientActiveTextureARBPROC glClientActiveTextureARB = NULL;
glMultiTexCoord1dARBPROC glMultiTexCoord1dARB = NULL;
glMultiTexCoord1dvARBPROC glMultiTexCoord1dvARB = NULL;
glMultiTexCoord1fARBPROC glMultiTexCoord1fARB = NULL;
glMultiTexCoord1fvARBPROC glMultiTexCoord1fvARB = NULL;
glMultiTexCoord1iARBPROC glMultiTexCoord1iARB = NULL;
glMultiTexCoord1ivARBPROC glMultiTexCoord1ivARB = NULL;
glMultiTexCoord1sARBPROC glMultiTexCoord1sARB = NULL;
glMultiTexCoord1svARBPROC glMultiTexCoord1svARB = NULL;
glMultiTexCoord2dARBPROC glMultiTexCoord2dARB = NULL;
glMultiTexCoord2dvARBPROC glMultiTexCoord2dvARB = NULL;
glMultiTexCoord2fARBPROC glMultiTexCoord2fARB = NULL;
glMultiTexCoord2fvARBPROC glMultiTexCoord2fvARB = NULL;
glMultiTexCoord2iARBPROC glMultiTexCoord2iARB = NULL;
glMultiTexCoord2ivARBPROC glMultiTexCoord2ivARB = NULL;
glMultiTexCoord2sARBPROC glMultiTexCoord2sARB = NULL;
glMultiTexCoord2svARBPROC glMultiTexCoord2svARB = NULL;
glMultiTexCoord3dARBPROC glMultiTexCoord3dARB = NULL;
glMultiTexCoord3dvARBPROC glMultiTexCoord3dvARB = NULL;
glMultiTexCoord3fARBPROC glMultiTexCoord3fARB = NULL;
glMultiTexCoord3fvARBPROC glMultiTexCoord3fvARB = NULL;
glMultiTexCoord3iARBPROC glMultiTexCoord3iARB = NULL;
glMultiTexCoord3ivARBPROC glMultiTexCoord3ivARB = NULL;
glMultiTexCoord3sARBPROC glMultiTexCoord3sARB = NULL;
glMultiTexCoord3svARBPROC glMultiTexCoord3svARB = NULL;
glMultiTexCoord4dARBPROC glMultiTexCoord4dARB = NULL;
glMultiTexCoord4dvARBPROC glMultiTexCoord4dvARB = NULL;
glMultiTexCoord4fARBPROC glMultiTexCoord4fARB = NULL;
glMultiTexCoord4fvARBPROC glMultiTexCoord4fvARB = NULL;
glMultiTexCoord4iARBPROC glMultiTexCoord4iARB = NULL;
glMultiTexCoord4ivARBPROC glMultiTexCoord4ivARB = NULL;
glMultiTexCoord4sARBPROC glMultiTexCoord4sARB = NULL;
glMultiTexCoord4svARBPROC glMultiTexCoord4svARB = NULL;
#endif /* GL_ARB_multitexture */

/* GL_ARB_occlusion_query */

#ifdef GL_ARB_occlusion_query
glGenQueriesARBPROC glGenQueriesARB = NULL;
glDeleteQueriesARBPROC glDeleteQueriesARB = NULL;
glIsQueryARBPROC glIsQueryARB = NULL;
glBeginQueryARBPROC glBeginQueryARB = NULL;
glEndQueryARBPROC glEndQueryARB = NULL;
glGetQueryivARBPROC glGetQueryivARB = NULL;
glGetQueryObjectivARBPROC glGetQueryObjectivARB = NULL;
glGetQueryObjectuivARBPROC glGetQueryObjectuivARB = NULL;
#endif /* GL_ARB_occlusion_query */

/* GL_ARB_shader_objects */

#ifdef GL_ARB_shader_objects
glDeleteObjectARBPROC glDeleteObjectARB = NULL;
glGetHandleARBPROC glGetHandleARB = NULL;
glDetachObjectARBPROC glDetachObjectARB = NULL;
glCreateShaderObjectARBPROC glCreateShaderObjectARB = NULL;
glShaderSourceARBPROC glShaderSourceARB = NULL;
glCompileShaderARBPROC glCompileShaderARB = NULL;
glCreateProgramObjectARBPROC glCreateProgramObjectARB = NULL;
glAttachObjectARBPROC glAttachObjectARB = NULL;
glLinkProgramARBPROC glLinkProgramARB = NULL;
glUseProgramObjectARBPROC glUseProgramObjectARB = NULL;
glValidateProgramARBPROC glValidateProgramARB = NULL;
glUniform1fARBPROC glUniform1fARB = NULL;
glUniform2fARBPROC glUniform2fARB = NULL;
glUniform3fARBPROC glUniform3fARB = NULL;
glUniform4fARBPROC glUniform4fARB = NULL;
glUniform1iARBPROC glUniform1iARB = NULL;
glUniform2iARBPROC glUniform2iARB = NULL;
glUniform3iARBPROC glUniform3iARB = NULL;
glUniform4iARBPROC glUniform4iARB = NULL;
glUniform1fvARBPROC glUniform1fvARB = NULL;
glUniform2fvARBPROC glUniform2fvARB = NULL;
glUniform3fvARBPROC glUniform3fvARB = NULL;
glUniform4fvARBPROC glUniform4fvARB = NULL;
glUniform1ivARBPROC glUniform1ivARB = NULL;
glUniform2ivARBPROC glUniform2ivARB = NULL;
glUniform3ivARBPROC glUniform3ivARB = NULL;
glUniform4ivARBPROC glUniform4ivARB = NULL;
glUniformMatrix2fvARBPROC glUniformMatrix2fvARB = NULL;
glUniformMatrix3fvARBPROC glUniformMatrix3fvARB = NULL;
glUniformMatrix4fvARBPROC glUniformMatrix4fvARB = NULL;
glGetObjectParameterfvARBPROC glGetObjectParameterfvARB = NULL;
glGetObjectParameterivARBPROC glGetObjectParameterivARB = NULL;
glGetInfoLogARBPROC glGetInfoLogARB = NULL;
glGetAttachedObjectsARBPROC glGetAttachedObjectsARB = NULL;
glGetUniformLocationARBPROC glGetUniformLocationARB = NULL;
glGetActiveUniformARBPROC glGetActiveUniformARB = NULL;
glGetUniformfvARBPROC glGetUniformfvARB = NULL;
glGetUniformivARBPROC glGetUniformivARB = NULL;
glGetShaderSourceARBPROC glGetShaderSourceARB = NULL;
#endif /* GL_ARB_shader_objects */

/* GL_ARB_vertex_shader */

#ifdef GL_ARB_vertex_shader
glBindAttribLocationARBPROC glBindAttribLocationARB = NULL;
glGetActiveAttribARBPROC glGetActiveAttribARB = NULL;
glGetAttribLocationARBPROC glGetAttribLocationARB = NULL;
#endif /* GL_ARB_vertex_shader */

/* GL_ARB_point_parameters */

#ifdef GL_ARB_point_parameters
glPointParameterfARBPROC glPointParameterfARB = NULL;
glPointParameterfvARBPROC glPointParameterfvARB = NULL;
#endif /* GL_ABR_point_parameters */

/* GL_ARB_texture_compression */

#ifdef GL_ARB_texture_compression
glCompressedTexImage3DARBPROC glCompressedTexImage3DARB = NULL;
glCompressedTexImage2DARBPROC glCompressedTexImage2DARB = NULL;
glCompressedTexImage1DARBPROC glCompressedTexImage1DARB = NULL;
glCompressedTexSubImage3DARBPROC glCompressedTexSubImage3DARB = NULL;
glCompressedTexSubImage2DARBPROC glCompressedTexSubImage2DARB = NULL;
glCompressedTexSubImage1DARBPROC glCompressedTexSubImage1DARB = NULL;
glGetCompressedTexImageARBPROC glGetCompressedTexImageARB = NULL;
#endif /* GL_ARB_texture_compression */

/* GL_ARB_transpose_matrix */

#ifdef GL_ARB_transpose_matrix
glLoadTransposeMatrixfARBPROC glLoadTransposeMatrixfARB = NULL;
glLoadTransposeMatrixdARBPROC glLoadTransposeMatrixdARB = NULL;
glMultTransposeMatrixfARBPROC glMultTransposeMatrixfARB = NULL;
glMultTransposeMatrixdARBPROC glMultTransposeMatrixdARB = NULL;
#endif /* GL_ARB_transpose_matrix */

/* GL_ARB_vertex_blend */

#ifdef GL_ARB_vertex_blend
glWeightbvARBPROC glWeightbvARB = NULL;
glWeightsvARBPROC glWeightsvARB = NULL;
glWeightivARBPROC glWeightivARB = NULL;
glWeightfvARBPROC glWeightfvARB = NULL;
glWeightdvARBPROC glWeightdvARB = NULL;
glWeightubvARBPROC glWeightubvARB = NULL;
glWeightusvARBPROC glWeightusvARB = NULL;
glWeightuivARBPROC glWeightuivARB = NULL;
glWeightPointerARBPROC glWeightPointerARB = NULL;
glVertexBlendARBPROC glVertexBlendARB = NULL;
#endif /* GL_ARB_vertex_blend */

/* GL_ARB_vertex_buffer_object */

#ifdef GL_ARB_vertex_buffer_object
glBindBufferARBPROC glBindBufferARB = NULL;
glDeleteBuffersARBPROC glDeleteBuffersARB = NULL;
glGenBuffersARBPROC glGenBuffersARB = NULL;
glIsBufferARBPROC glIsBufferARB = NULL;
glBufferDataARBPROC glBufferDataARB = NULL;
glBufferSubDataARBPROC glBufferSubDataARB = NULL;
glGetBufferSubDataARBPROC glGetBufferSubDataARB = NULL;
glMapBufferARBPROC glMapBufferARB = NULL;
glUnmapBufferARBPROC glUnmapBufferARB = NULL;
glGetBufferParameterivARBPROC glGetBufferParameterivARB = NULL;
glGetBufferPointervARBPROC glGetBufferPointervARB = NULL;
#endif /* GL_ARB_vertex_buffer_object */

/* GL_ARB_vertex_program */

#ifdef GL_ARB_vertex_program
glVertexAttrib1sARBPROC glVertexAttrib1sARB = NULL;
glVertexAttrib1fARBPROC glVertexAttrib1fARB = NULL;
glVertexAttrib1dARBPROC glVertexAttrib1dARB = NULL;
glVertexAttrib2sARBPROC glVertexAttrib2sARB = NULL;
glVertexAttrib2fARBPROC glVertexAttrib2fARB = NULL;
glVertexAttrib2dARBPROC glVertexAttrib2dARB = NULL;
glVertexAttrib3sARBPROC glVertexAttrib3sARB = NULL;
glVertexAttrib3fARBPROC glVertexAttrib3fARB = NULL;
glVertexAttrib3dARBPROC glVertexAttrib3dARB = NULL;
glVertexAttrib4sARBPROC glVertexAttrib4sARB = NULL;
glVertexAttrib4fARBPROC glVertexAttrib4fARB = NULL;
glVertexAttrib4dARBPROC glVertexAttrib4dARB = NULL;
glVertexAttrib4NubARBPROC glVertexAttrib4NubARB = NULL;
glVertexAttrib1svARBPROC glVertexAttrib1svARB = NULL;
glVertexAttrib1fvARBPROC glVertexAttrib1fvARB = NULL;
glVertexAttrib1dvARBPROC glVertexAttrib1dvARB = NULL;
glVertexAttrib2svARBPROC glVertexAttrib2svARB = NULL;
glVertexAttrib2fvARBPROC glVertexAttrib2fvARB = NULL;
glVertexAttrib2dvARBPROC glVertexAttrib2dvARB = NULL;
glVertexAttrib3svARBPROC glVertexAttrib3svARB = NULL;
glVertexAttrib3fvARBPROC glVertexAttrib3fvARB = NULL;
glVertexAttrib3dvARBPROC glVertexAttrib3dvARB = NULL;
glVertexAttrib4bvARBPROC glVertexAttrib4bvARB = NULL;
glVertexAttrib4svARBPROC glVertexAttrib4svARB = NULL;
glVertexAttrib4ivARBPROC glVertexAttrib4ivARB = NULL;
glVertexAttrib4ubvARBPROC glVertexAttrib4ubvARB = NULL;
glVertexAttrib4usvARBPROC glVertexAttrib4usvARB = NULL;
glVertexAttrib4uivARBPROC glVertexAttrib4uivARB = NULL;
glVertexAttrib4fvARBPROC glVertexAttrib4fvARB = NULL;
glVertexAttrib4dvARBPROC glVertexAttrib4dvARB = NULL;
glVertexAttrib4NbvARBPROC glVertexAttrib4NbvARB = NULL;
glVertexAttrib4NsvARBPROC glVertexAttrib4NsvARB = NULL;
glVertexAttrib4NivARBPROC glVertexAttrib4NivARB = NULL;
glVertexAttrib4NubvARBPROC glVertexAttrib4NubvARB = NULL;
glVertexAttrib4NusvARBPROC glVertexAttrib4NusvARB = NULL;
glVertexAttrib4NuivARBPROC glVertexAttrib4NuivARB = NULL;
glVertexAttribPointerARBPROC glVertexAttribPointerARB = NULL;
glEnableVertexAttribArrayARBPROC glEnableVertexAttribArrayARB = NULL;
glDisableVertexAttribArrayARBPROC glDisableVertexAttribArrayARB = NULL;
glProgramStringARBPROC glProgramStringARB = NULL;
glBindProgramARBPROC glBindProgramARB = NULL;
glDeleteProgramsARBPROC glDeleteProgramsARB = NULL;
glGenProgramsARBPROC glGenProgramsARB = NULL;
glProgramEnvParameter4dARBPROC glProgramEnvParameter4dARB = NULL;
glProgramEnvParameter4dvARBPROC glProgramEnvParameter4dvARB = NULL;
glProgramEnvParameter4fARBPROC glProgramEnvParameter4fARB = NULL;
glProgramEnvParameter4fvARBPROC glProgramEnvParameter4fvARB = NULL;
glProgramLocalParameter4dARBPROC glProgramLocalParameter4dARB = NULL;
glProgramLocalParameter4dvARBPROC glProgramLocalParameter4dvARB = NULL;
glProgramLocalParameter4fARBPROC glProgramLocalParameter4fARB = NULL;
glProgramLocalParameter4fvARBPROC glProgramLocalParameter4fvARB = NULL;
glGetProgramEnvParameterdvARBPROC glGetProgramEnvParameterdvARB = NULL;
glGetProgramEnvParameterfvARBPROC glGetProgramEnvParameterfvARB = NULL;
glGetProgramLocalParameterdvARBPROC glGetProgramLocalParameterdvARB = NULL;
glGetProgramLocalParameterfvARBPROC glGetProgramLocalParameterfvARB = NULL;
glGetProgramivARBPROC glGetProgramivARB = NULL;
glGetProgramStringARBPROC glGetProgramStringARB = NULL;
glGetVertexAttribdvARBPROC glGetVertexAttribdvARB = NULL;
glGetVertexAttribfvARBPROC glGetVertexAttribfvARB = NULL;
glGetVertexAttribivARBPROC glGetVertexAttribivARB = NULL;
glGetVertexAttribPointervARBPROC glGetVertexAttribPointervARB = NULL;
glIsProgramARBPROC glIsProgramARB = NULL;
#endif /* GL_ARB_vertex_program */

/* GL_ARB_window_pos */

#ifdef GL_ARB_window_pos
glWindowPos2dARBPROC glWindowPos2dARB = NULL;
glWindowPos2fARBPROC glWindowPos2fARB = NULL;
glWindowPos2iARBPROC glWindowPos2iARB = NULL;
glWindowPos2sARBPROC glWindowPos2sARB = NULL;
glWindowPos2dvARBPROC glWindowPos2dvARB = NULL;
glWindowPos2fvARBPROC glWindowPos2fvARB = NULL;
glWindowPos2ivARBPROC glWindowPos2ivARB = NULL;
glWindowPos2svARBPROC glWindowPos2svARB = NULL;
glWindowPos3dARBPROC glWindowPos3dARB = NULL;
glWindowPos3fARBPROC glWindowPos3fARB = NULL;
glWindowPos3iARBPROC glWindowPos3iARB = NULL;
glWindowPos3sARBPROC glWindowPos3sARB = NULL;
glWindowPos3dvARBPROC glWindowPos3dvARB = NULL;
glWindowPos3fvARBPROC glWindowPos3fvARB = NULL;
glWindowPos3ivARBPROC glWindowPos3ivARB = NULL;
glWindowPos3svARBPROC glWindowPos3svARB = NULL;
#endif /* GL_ARB_window_pos */

/* GL_ATI_draw_buffers */

#ifdef GL_ATI_draw_buffers
glDrawBuffersATIPROC glDrawBuffersATI = NULL;
#endif /* GL_ATI_draw_buffers */

/* GL_ATI_element_array */

#ifdef GL_ATI_element_array
glElementPointerATIPROC glElementPointerATI = NULL;
glDrawElementArrayATIPROC glDrawElementArrayATI = NULL;
glDrawRangeElementArrayATIPROC glDrawRangeElementArrayATI = NULL;
#endif /* GL_ATI_element_array */

/* GL_ATI_envmap_bumpmap */

#ifdef GL_ATI_envmap_bumpmap
glTexBumpParameterivATIPROC glTexBumpParameterivATI = NULL;
glTexBumpParameterfvATIPROC glTexBumpParameterfvATI = NULL;
glGetTexBumpParameterivATIPROC glGetTexBumpParameterivATI = NULL;
glGetTexBumpParameterfvATIPROC glGetTexBumpParameterfvATI = NULL;
#endif /* GL_ATI_envmap_bumpmap */

/* GL_ATI_fragment_shader */

#ifdef GL_ATI_fragment_shader
glGenFragmentShadersATIPROC glGenFragmentShadersATI = NULL;
glBindFragmentShaderATIPROC glBindFragmentShaderATI = NULL;
glDeleteFragmentShaderATIPROC glDeleteFragmentShaderATI = NULL;
glBeginFragmentShaderATIPROC glBeginFragmentShaderATI = NULL;
glEndFragmentShaderATIPROC glEndFragmentShaderATI = NULL;
glPassTexCoordATIPROC glPassTexCoordATI = NULL;
glSampleMapATIPROC glSampleMapATI = NULL;
glColorFragmentOp1ATIPROC glColorFragmentOp1ATI = NULL;
glColorFragmentOp2ATIPROC glColorFragmentOp2ATI = NULL;
glColorFragmentOp3ATIPROC glColorFragmentOp3ATI = NULL;
glAlphaFragmentOp1ATIPROC glAlphaFragmentOp1ATI = NULL;
glAlphaFragmentOp2ATIPROC glAlphaFragmentOp2ATI = NULL;
glAlphaFragmentOp3ATIPROC glAlphaFragmentOp3ATI = NULL;
glSetFragmentShaderConstantATIPROC glSetFragmentShaderConstantATI = NULL;
#endif /* GL_ATI_fragment_shader */

/* GL_ATI_map_object_buffer */

#ifdef GL_ATI_map_object_buffer
glMapObjectBufferATIPROC glMapObjectBufferATI = NULL;
glUnmapObjectBufferATIPROC glUnmapObjectBufferATI = NULL;
#endif /* GL_ATI_map_object_buffer */

/* GL_ATI_pn_triangles */

#ifdef GL_ATI_pn_triangles
glPNTrianglesiATIPROC glPNTrianglesiATI = NULL;
glPNTrianglesfATIPROC glPNTrianglesfATI = NULL;
#endif /* GL_ATI_pn_triangles */

/* GL_ATI_separate_stencil */

#ifdef GL_ATI_separate_stencil
glStencilOpSeparateATIPROC glStencilOpSeparateATI = NULL;
glStencilFuncSeparateATIPROC glStencilFuncSeparateATI = NULL;
#endif /* GL_ATI_separate_stencil */

/* GL_ATI_vertex_attrib_array_object */

#ifdef GL_ATI_vertex_attrib_array_object
glVertexAttribArrayObjectATIPROC glVertexAttribArrayObjectATI = NULL;
glGetVertexAttribArrayObjectfvATIPROC glGetVertexAttribArrayObjectfvATI = NULL;
glGetVertexAttribArrayObjectivATIPROC glGetVertexAttribArrayObjectivATI = NULL;
#endif /* GL_ATI_vertex_attrib_array_object */

/* GL_ATI_vertex_array_object */

#ifdef GL_ATI_vertex_array_object
glNewObjectBufferATIPROC glNewObjectBufferATI = NULL;
glIsObjectBufferATIPROC glIsObjectBufferATI = NULL;
glUpdateObjectBufferATIPROC glUpdateObjectBufferATI = NULL;
glGetObjectBufferfvATIPROC glGetObjectBufferfvATI = NULL;
glGetObjectBufferivATIPROC glGetObjectBufferivATI = NULL;
glFreeObjectBufferATIPROC glFreeObjectBufferATI = NULL;
glArrayObjectATIPROC glArrayObjectATI = NULL;
glGetArrayObjectfvATIPROC glGetArrayObjectfvATI = NULL;
glGetArrayObjectivATIPROC glGetArrayObjectivATI = NULL;
glVariantArrayObjectATIPROC glVariantArrayObjectATI = NULL;
glGetVariantArrayObjectfvATIPROC glGetVariantArrayObjectfvATI = NULL;
glGetVariantArrayObjectivATIPROC glGetVariantArrayObjectivATI = NULL;
#endif /* GL_ATI_vertex_array_object */

/* GL_ATI_vertex_streams */

#ifdef GL_ATI_vertex_streams
glClientActiveVertexStreamATIPROC glClientActiveVertexStreamATI = NULL;
glVertexBlendEnviATIPROC glVertexBlendEnviATI = NULL;
glVertexBlendEnvfATIPROC glVertexBlendEnvfATI = NULL;
glVertexStream1sATIPROC glVertexStream1sATI = NULL;
glVertexStream1svATIPROC glVertexStream1svATI = NULL;
glVertexStream1iATIPROC glVertexStream1iATI = NULL;
glVertexStream1ivATIPROC glVertexStream1ivATI = NULL;
glVertexStream1fATIPROC glVertexStream1fATI = NULL;
glVertexStream1fvATIPROC glVertexStream1fvATI = NULL;
glVertexStream1dATIPROC glVertexStream1dATI = NULL;
glVertexStream1dvATIPROC glVertexStream1dvATI = NULL;
glVertexStream2sATIPROC glVertexStream2sATI = NULL;
glVertexStream2svATIPROC glVertexStream2svATI = NULL;
glVertexStream2iATIPROC glVertexStream2iATI = NULL;
glVertexStream2ivATIPROC glVertexStream2ivATI = NULL;
glVertexStream2fATIPROC glVertexStream2fATI = NULL;
glVertexStream2fvATIPROC glVertexStream2fvATI = NULL;
glVertexStream2dATIPROC glVertexStream2dATI = NULL;
glVertexStream2dvATIPROC glVertexStream2dvATI = NULL;
glVertexStream3sATIPROC glVertexStream3sATI = NULL;
glVertexStream3svATIPROC glVertexStream3svATI = NULL;
glVertexStream3iATIPROC glVertexStream3iATI = NULL;
glVertexStream3ivATIPROC glVertexStream3ivATI = NULL;
glVertexStream3fATIPROC glVertexStream3fATI = NULL;
glVertexStream3fvATIPROC glVertexStream3fvATI = NULL;
glVertexStream3dATIPROC glVertexStream3dATI = NULL;
glVertexStream3dvATIPROC glVertexStream3dvATI = NULL;
glVertexStream4sATIPROC glVertexStream4sATI = NULL;
glVertexStream4svATIPROC glVertexStream4svATI = NULL;
glVertexStream4iATIPROC glVertexStream4iATI = NULL;
glVertexStream4ivATIPROC glVertexStream4ivATI = NULL;
glVertexStream4fATIPROC glVertexStream4fATI = NULL;
glVertexStream4fvATIPROC glVertexStream4fvATI = NULL;
glVertexStream4dATIPROC glVertexStream4dATI = NULL;
glVertexStream4dvATIPROC glVertexStream4dvATI = NULL;
glNormalStream3bATIPROC glNormalStream3bATI = NULL;
glNormalStream3bvATIPROC glNormalStream3bvATI = NULL;
glNormalStream3sATIPROC glNormalStream3sATI = NULL;
glNormalStream3svATIPROC glNormalStream3svATI = NULL;
glNormalStream3iATIPROC glNormalStream3iATI = NULL;
glNormalStream3ivATIPROC glNormalStream3ivATI = NULL;
glNormalStream3fATIPROC glNormalStream3fATI = NULL;
glNormalStream3fvATIPROC glNormalStream3fvATI = NULL;
glNormalStream3dATIPROC glNormalStream3dATI = NULL;
glNormalStream3dvATIPROC glNormalStream3dvATI = NULL;
#endif /* GL_ATI_vertex_streams */

/* GL_ARB_draw_buffers */

#ifdef GL_ARB_draw_buffers
glDrawBuffersARBPROC glDrawBuffersARB = NULL;
#endif /* GL_ARB_draw_buffers */

/* GL_ARB_color_buffer_float */

#ifdef GL_ARB_color_buffer_float
glClampColorARBPROC glClampColorARB = NULL;
#endif /* GL_ARB_color_buffer_float */

/* GL_EXT_blend_color */

#ifdef GL_EXT_blend_color
glBlendColorEXTPROC glBlendColorEXT = NULL;
#endif /* GL_EXT_blend_color */

/* GL_EXT_blend_func_separate */

#ifdef GL_EXT_blend_func_separate
glBlendFuncSeparateEXTPROC glBlendFuncSeparateEXT = NULL;
#endif /* GL_EXT_blend_func_separate */

/* GL_EXT_blend_minmax */

#ifdef GL_EXT_blend_minmax
glBlendEquationEXTPROC glBlendEquationEXT = NULL;
#endif /* GL_EXT_blend_minmax */

/* GL_EXT_color_subtable */

#ifdef GL_EXT_color_subtable
glColorSubTableEXTPROC glColorSubTableEXT = NULL;
glCopyColorSubTableEXTPROC glCopyColorSubTableEXT = NULL;
#endif /* GL_EXT_color_subtable */

/* GL_EXT_compiled_vertex_array */

#ifdef GL_EXT_compiled_vertex_array
glLockArraysEXTPROC glLockArraysEXT = NULL;
glUnlockArraysEXTPROC glUnlockArraysEXT = NULL;
#endif /* GL_EXT_compiled_vertex_array */

/* GL_EXT_convolution */

#ifdef GL_EXT_convolution
glConvolutionFilter1DEXTPROC glConvolutionFilter1DEXT = NULL;
glConvolutionFilter2DEXTPROC glConvolutionFilter2DEXT = NULL;
glConvolutionParameterfEXTPROC glConvolutionParameterfEXT = NULL;
glConvolutionParameterfvEXTPROC glConvolutionParameterfvEXT = NULL;
glConvolutionParameteriEXTPROC glConvolutionParameteriEXT = NULL;
glConvolutionParameterivEXTPROC glConvolutionParameterivEXT = NULL;
glCopyConvolutionFilter1DEXTPROC glCopyConvolutionFilter1DEXT = NULL;
glCopyConvolutionFilter2DEXTPROC glCopyConvolutionFilter2DEXT = NULL;
glGetConvolutionFilterEXTPROC glGetConvolutionFilterEXT = NULL;
glGetConvolutionParameterfvEXTPROC glGetConvolutionParameterfvEXT = NULL;
glGetConvolutionParameterivEXTPROC glGetConvolutionParameterivEXT = NULL;
glGetSeparableFilterEXTPROC glGetSeparableFilterEXT = NULL;
glSeparableFilter2DEXTPROC glSeparableFilter2DEXT = NULL;
#endif /* GL_EXT_convolution */

/* GL_EXT_coordinate_frame */

#ifdef GL_EXT_coordinate_frame
glTangent3bEXTPROC glTangent3bEXT = NULL;
glTangent3bvEXTPROC glTangent3bvEXT = NULL;
glTangent3dEXTPROC glTangent3dEXT = NULL;
glTangent3dvEXTPROC glTangent3dvEXT = NULL;
glTangent3fEXTPROC glTangent3fEXT = NULL;
glTangent3fvEXTPROC glTangent3fvEXT = NULL;
glTangent3iEXTPROC glTangent3iEXT = NULL;
glTangent3ivEXTPROC glTangent3ivEXT = NULL;
glTangent3sEXTPROC glTangent3sEXT = NULL;
glTangent3svEXTPROC glTangent3svEXT = NULL;
glBinormal3bEXTPROC glBinormal3bEXT = NULL;
glBinormal3bvEXTPROC glBinormal3bvEXT = NULL;
glBinormal3dEXTPROC glBinormal3dEXT = NULL;
glBinormal3dvEXTPROC glBinormal3dvEXT = NULL;
glBinormal3fEXTPROC glBinormal3fEXT = NULL;
glBinormal3fvEXTPROC glBinormal3fvEXT = NULL;
glBinormal3iEXTPROC glBinormal3iEXT = NULL;
glBinormal3ivEXTPROC glBinormal3ivEXT = NULL;
glBinormal3sEXTPROC glBinormal3sEXT = NULL;
glBinormal3svEXTPROC glBinormal3svEXT = NULL;
glTangentPointerEXTPROC glTangentPointerEXT = NULL;
glBinormalPointerEXTPROC glBinormalPointerEXT = NULL;
#endif /* GL_EXT_coordinate_frame */

/* GL_EXT_copy_texture */

#ifdef GL_EXT_copy_texture
glCopyTexImage1DEXTPROC glCopyTexImage1DEXT = NULL;
glCopyTexImage2DEXTPROC glCopyTexImage2DEXT = NULL;
glCopyTexSubImage1DEXTPROC glCopyTexSubImage1DEXT = NULL;
glCopyTexSubImage2DEXTPROC glCopyTexSubImage2DEXT = NULL;
glCopyTexSubImage3DEXTPROC glCopyTexSubImage3DEXT = NULL;
#endif /* GL_EXT_copy_texture */

/* GL_EXT_cull_vertex */

#ifdef GL_EXT_cull_vertex
glCullParameterfvEXTPROC glCullParameterfvEXT = NULL;
glCullParameterdvEXTPROC glCullParameterdvEXT = NULL;
#endif /* GL_EXT_cull_vertex */

/* GL_EXT_depth_bounds_test */

#ifdef GL_EXT_depth_bounds_test
glDepthBoundsEXTPROC glDepthBoundsEXT = NULL;
#endif /* GL_EXT_depth_bounds_test */

/* GL_EXT_blend_equation_separate */

#ifdef GL_EXT_blend_equation_separate
glBlendEquationSeparateEXTPROC glBlendEquationSeparateEXT = NULL;
#endif /* GL_EXT_blend_equation_separate */

/* GL_EXT_draw_range_elements */

#ifdef GL_EXT_draw_range_elements
glDrawRangeElementsEXTPROC glDrawRangeElementsEXT = NULL;
#endif /* GL_EXT_draw_range_elements  */

/* GL_EXT_fog_coord */

#ifdef GL_EXT_fog_coord
glFogCoordfEXTPROC glFogCoordfEXT = NULL;
glFogCoordfvEXTPROC glFogCoordfvEXT = NULL;
glFogCoorddEXTPROC glFogCoorddEXT = NULL;
glFogCoorddvEXTPROC glFogCoorddvEXT = NULL;
glFogCoordPointerEXTPROC glFogCoordPointerEXT = NULL;
#endif /* GL_EXT_for_color */

/* GL_EXT_framebuffer_object */

#ifdef GL_EXT_framebuffer_object
glIsRenderbufferEXTPROC glIsRenderbufferEXT = NULL;
glBindRenderbufferEXTPROC glBindRenderbufferEXT = NULL;
glDeleteRenderbuffersEXTPROC glDeleteRenderbuffersEXT = NULL;
glGenRenderbuffersEXTPROC glGenRenderbuffersEXT = NULL;
glRenderbufferStorageEXTPROC glRenderbufferStorageEXT = NULL;
glGetRenderbufferParameterivEXTPROC glGetRenderbufferParameterivEXT = NULL;
glIsFramebufferEXTPROC glIsFramebufferEXT = NULL;
glBindFramebufferEXTPROC glBindFramebufferEXT = NULL;
glDeleteFramebuffersEXTPROC glDeleteFramebuffersEXT = NULL;
glGenFramebuffersEXTPROC glGenFramebuffersEXT = NULL;
glCheckFramebufferStatusEXTPROC glCheckFramebufferStatusEXT = NULL;
glFramebufferTexture1DEXTPROC glFramebufferTexture1DEXT = NULL;
glFramebufferTexture2DEXTPROC glFramebufferTexture2DEXT = NULL;
glFramebufferTexture3DEXTPROC glFramebufferTexture3DEXT = NULL;
glFramebufferRenderbufferEXTPROC glFramebufferRenderbufferEXT = NULL;
glGetFramebufferAttachmentParameterivEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
glGenerateMipmapEXTPROC glGenerateMipmapEXT = NULL;
#endif /* GL_EXT_framebuffer_object */

/* GL_EXT_histogram */

#ifdef GL_EXT_histogram
glGetHistogramEXTPROC glGetHistogramEXT = NULL;
glGetHistogramParameterfvEXTPROC glGetHistogramParameterfvEXT = NULL;
glGetHistogramParameterivEXTPROC glGetHistogramParameterivEXT = NULL;
glGetMinMaxEXTPROC glGetMinMaxEXT = NULL;
glGetMinMaxParameterfvEXTPROC glGetMinMaxParameterfvEXT = NULL;
glGetMinMaxParameterivEXTPROC glGetMinMaxParameterivEXT = NULL;
glHistogramEXTPROC glHistogramEXT = NULL;
glResetMinMaxEXTPROC glResetMinMaxEXT = NULL;
#endif /* GL_EXT_histogram */

/* GL_EXT_index_array_formats */

#ifdef GL_EXT_index_array_formats
#endif /* GL_EXT_index_array_formats */

/* GL_EXT_index_func */

#ifdef GL_EXT_index_func
glIndexFuncEXTPROC glIndexFuncEXT = NULL;
#endif /* GL_EXT_index_func */

/* GL_EXT_light_texture */

#ifdef GL_EXT_light_texture
glApplyTextureEXTPROC glApplyTextureEXT = NULL;
glTextureLightEXTPROC glTextureLightEXT = NULL;
glTextureMaterialEXTPROC glTextureMaterialEXT = NULL;
#endif /* GL_EXT_light_texture */

/* GL_EXT_index_material */

#ifdef GL_EXT_index_material
glIndexMaterialEXTPROC glIndexMaterialEXT = NULL;
#endif /* GL_EXT_index_material */

/* GL_EXT_multi_draw_arrays */

#ifdef GL_EXT_multi_draw_arrays
glMultiDrawArraysEXTPROC glMultiDrawArraysEXT = NULL;
glMultiDrawElementsEXTPROC glMultiDrawElementsEXT = NULL;
#endif /* GL_EXT_multi_draw_arrays */

/* GL_EXT_multisample */

#ifdef GL_EXT_multisample
glSampleMaskEXTPROC glSampleMaskEXT = NULL;
glSamplePatternEXTPROC glSamplePatternEXT = NULL;
#endif /* GL_EXT_multisample */

/* GL_EXT_paletted_texture */

#ifdef GL_EXT_paletted_texture
glColorTableEXTPROC glColorTableEXT = NULL;
glGetColorTableEXTPROC glGetColorTableEXT = NULL;
glGetColorTableParameterivEXTPROC glGetColorTableParameterivEXT = NULL;
glGetColorTableParameterfvEXTPROC glGetColorTableParameterfvEXT = NULL;
#endif /* GL_EXT_paletted_texture */

/* GL_EXT_pixel_transform */

#ifdef GL_EXT_pixel_transform
glPixelTransformParameteriEXTPROC glPixelTransformParameteriEXT = NULL;
glPixelTransformParameterfEXTPROC glPixelTransformParameterfEXT = NULL;
glPixelTransformParameterivEXTPROC glPixelTransformParameterivEXT = NULL;
glPixelTransformParameterfvEXTPROC glPixelTransformParameterfvEXT = NULL;
#endif /* GL_EXT_pixel_transform */

/* GL_EXT_point_parameters */

#ifdef GL_EXT_point_parameters
glPointParameterfEXTPROC glPointParameterfEXT = NULL;
glPointParameterfvEXTPROC glPointParameterfvEXT = NULL;
#endif /* GL_EXT_point_parameters */

/* GL_EXT_polygon_offset */

#ifdef GL_EXT_polygon_offset
glPolygonOffsetEXTPROC glPolygonOffsetEXT = NULL;
#endif /* GL_EXT_polygon_offset */

/* GL_NV_register_combiners */

#ifdef GL_NV_register_combiners
glCombinerParameterfvNVPROC glCombinerParameterfvNV = NULL;
glCombinerParameterfNVPROC  glCombinerParameterfNV = NULL;
glCombinerParameterivNVPROC glCombinerParameterivNV = NULL;
glCombinerParameteriNVPROC glCombinerParameteriNV = NULL;
glCombinerInputNVPROC glCombinerInputNV = NULL;
glCombinerOutputNVPROC glCombinerOutputNV = NULL;
glFinalCombinerInputNVPROC glFinalCombinerInputNV = NULL;
glGetCombinerInputParameterfvNVPROC glGetCombinerInputParameterfvNV = NULL;
glGetCombinerInputParameterivNVPROC glGetCombinerInputParameterivNV = NULL;
glGetCombinerOutputParameterfvNVPROC glGetCombinerOutputParameterfvNV = NULL;
glGetCombinerOutputParameterivNVPROC glGetCombinerOutputParameterivNV = NULL;
glGetFinalCombinerInputParameterfvNVPROC glGetFinalCombinerInputParameterfvNV = NULL;
glGetFinalCombinerInputParameterivNVPROC glGetFinalCombinerInputParameterivNV = NULL;
#endif /* GL_NV_register_combiners */

/* GL_EXT_secondary_color */

#ifdef GL_EXT_secondary_color
glSecondaryColor3bEXTPROC glSecondaryColor3bEXT = NULL;
glSecondaryColor3bvEXTPROC glSecondaryColor3bvEXT = NULL;
glSecondaryColor3dEXTPROC glSecondaryColor3dEXT = NULL;
glSecondaryColor3dvEXTPROC glSecondaryColor3dvEXT = NULL;
glSecondaryColor3fEXTPROC glSecondaryColor3fEXT = NULL;
glSecondaryColor3fvEXTPROC glSecondaryColor3fvEXT = NULL;
glSecondaryColor3iEXTPROC glSecondaryColor3iEXT = NULL;
glSecondaryColor3ivEXTPROC glSecondaryColor3ivEXT = NULL;
glSecondaryColor3sEXTPROC glSecondaryColor3sEXT = NULL;
glSecondaryColor3svEXTPROC glSecondaryColor3svEXT = NULL;
glSecondaryColor3ubEXTPROC glSecondaryColor3ubEXT = NULL;
glSecondaryColor3ubvEXTPROC glSecondaryColor3ubvEXT = NULL;
glSecondaryColor3uiEXTPROC glSecondaryColor3uiEXT = NULL;
glSecondaryColor3uivEXTPROC glSecondaryColor3uivEXT = NULL;
glSecondaryColor3usEXTPROC glSecondaryColor3usEXT = NULL;
glSecondaryColor3usvEXTPROC glSecondaryColor3usvEXT = NULL;
glSecondaryColorPointerEXTPROC glSecondaryColorPointerEXT = NULL;
#endif /* GL_EXT_secondary_color */

/* GL_EXT_stencil_two_side */

#ifdef GL_EXT_stencil_two_side
glActiveStencilFaceEXTPROC glActiveStencilFaceEXT = NULL;
#endif /* GL_EXT_stencil_two_side */

/* GL_EXT_subtexture */

#ifdef GL_EXT_subtexture
glTexSubImage1DEXTPROC glTexSubImage1DEXT = NULL;
glTexSubImage2DEXTPROC glTexSubImage2DEXT = NULL;
glTexSubImage3DEXTPROC glTexSubImage3DEXT = NULL;
#endif /* GL_EXT_subtexture */

/* GL_EXT_texture */

#ifdef GL_EXT_texture
#endif /* GL_EXT_texture */

/* GL_EXT_texture3D */

#ifdef GL_EXT_texture3D
glTexImage3DEXTPROC glTexImage3DEXT = NULL;
#endif /* GL_EXT_texture3D */

/* GL_EXT_texture_object */

#ifdef GL_EXT_texture_object
glAreTexturesResidentEXTPROC glAreTexturesResidentEXT = NULL;
glBindTextureEXTPROC glBindTextureEXT = NULL;
glDeleteTexturesEXTPROC glDeleteTexturesEXT = NULL;
glGenTexturesEXTPROC glGenTexturesEXT = NULL;
glIsTextureEXTPROC glIsTextureEXT = NULL;
glPrioritizeTexturesEXTPROC glPrioritizeTexturesEXT = NULL;
#endif /* GL_EXT_texture_object */

/* GL_EXT_texture_perturb_normal */

#ifdef GL_EXT_texture_perturb_normal
glTextureNormalEXTPROC glTextureNormalEXT = NULL;
#endif /* GL_EXT_texture_perturb_normal */

/* GL_EXT_vertex_array */

#ifdef GL_EXT_vertex_array
glArrayElementEXTPROC glArrayElementEXT = NULL;
glColorPointerEXTPROC glColorPointerEXT = NULL;
glDrawArraysEXTPROC glDrawArraysEXT = NULL;
glEdgeFlagPointerEXTPROC glEdgeFlagPointerEXT = NULL;
glGetPointervEXTPROC glGetPointervEXT = NULL;
glIndexPointerEXTPROC glIndexPointerEXT = NULL;
glNormalPointerEXTPROC glNormalPointerEXT = NULL;
glTexCoordPointerEXTPROC glTexCoordPointerEXT = NULL;
glVertexPointerEXTPROC glVertexPointerEXT = NULL;
#endif /* GL_EXT_vertex_array */

/* GL_EXT_vertex_shader */

#ifdef GL_EXT_vertex_shader
glBeginVertexShaderEXTPROC glBeginVertexShaderEXT = NULL;
glEndVertexShaderEXTPROC glEndVertexShaderEXT = NULL;
glBindVertexShaderEXTPROC glBindVertexShaderEXT = NULL;
glGenVertexShadersEXTPROC glGenVertexShadersEXT = NULL;
glDeleteVertexShaderEXTPROC glDeleteVertexShaderEXT = NULL;
glShaderOp1EXTPROC glShaderOp1EXT = NULL;
glShaderOp2EXTPROC glShaderOp2EXT = NULL;
glShaderOp3EXTPROC glShaderOp3EXT = NULL;
glSwizzleEXTPROC glSwizzleEXT = NULL;
glWriteMaskEXTPROC glWriteMaskEXT = NULL;
glInsertComponentEXTPROC glInsertComponentEXT = NULL;
glExtractComponentEXTPROC glExtractComponentEXT = NULL;
glGenSymbolsEXTPROC glGenSymbolsEXT = NULL;
glSetInvariantEXTPROC glSetInvariantEXT = NULL;
glSetLocalConstantEXTPROC glSetLocalConstantEXT = NULL;
glVariantbvEXTPROC glVariantbvEXT = NULL;
glVariantsvEXTPROC glVariantsvEXT = NULL;
glVariantivEXTPROC glVariantivEXT = NULL;
glVariantfvEXTPROC glVariantfvEXT = NULL;
glVariantdvEXTPROC glVariantdvEXT = NULL;
glVariantubvEXTPROC glVariantubvEXT = NULL;
glVariantusvEXTPROC glVariantusvEXT = NULL;
glVariantuivEXTPROC glVariantuivEXT = NULL;
glVariantPointerEXTPROC glVariantPointerEXT = NULL;
glEnableVariantClientStateEXTPROC glEnableVariantClientStateEXT = NULL;
glDisableVariantClientStateEXTPROC glDisableVariantClientStateEXT = NULL;
glBindLightParameterEXTPROC glBindLightParameterEXT = NULL;
glBindMaterialParameterEXTPROC glBindMaterialParameterEXT = NULL;
glBindTexGenParameterEXTPROC glBindTexGenParameterEXT = NULL;
glBindTextureUnitParameterEXTPROC glBindTextureUnitParameterEXT = NULL;
glBindParameterEXTPROC glBindParameterEXT = NULL;
glIsVariantEnabledEXTPROC glIsVariantEnabledEXT = NULL;
glGetVariantBooleanvEXTPROC glGetVariantBooleanvEXT = NULL;
glGetVariantIntegervEXTPROC glGetVariantIntegervEXT = NULL;
glGetVariantFloatvEXTPROC glGetVariantFloatvEXT = NULL;
glGetVariantPointervEXTPROC glGetVariantPointervEXT = NULL;
glGetInvariantBooleanvEXTPROC glGetInvariantBooleanvEXT = NULL;
glGetInvariantIntegervEXTPROC glGetInvariantIntegervEXT = NULL;
glGetInvariantFloatvEXTPROC glGetInvariantFloatvEXT = NULL;
glGetLocalConstantBooleanvEXTPROC glGetLocalConstantBooleanvEXT = NULL;
glGetLocalConstantIntegervEXTPROC glGetLocalConstantIntegervEXT = NULL;
glGetLocalConstantFloatvEXTPROC glGetLocalConstantFloatvEXT = NULL;
#endif /* GL_EXT_vertex_shader */

/* GL_EXT_vertex_weighting */

#ifdef GL_EXT_vertex_weighting
glVertexWeightfEXTPROC glVertexWeightfEXT = NULL;
glVertexWeightfvEXTPROC glVertexWeightfvEXT = NULL;
glVertexWeightPointerEXTPROC glVertexWeightPointerEXT = NULL;
#endif /* GL_EXT_vertex_weighting */

/* GL_HP_image_transform */

#ifdef GL_HP_image_transform
glImageTransformParameteriHPPROC glImageTransformParameteriHP = NULL;
glImageTransformParameterfHPPROC glImageTransformParameterfHP = NULL;
glImageTransformParameterivHPPROC glImageTransformParameterivHP = NULL;
glImageTransformParameterfvHPPROC glImageTransformParameterfvHP = NULL;
glGetImageTransformParameterivHPPROC glGetImageTransformParameterivHP = NULL;
glGetImageTransformParameterfvHPPROC glGetImageTransformParameterfvHP = NULL;
#endif /* GL_HP_image_transform */

/* GL_IBM_multimode_draw_arrays */

#ifdef GL_IBM_multimode_draw_arrays
glMultiModeDrawArraysIBMPROC glMultiModeDrawArraysIBM = NULL;
glMultiModeDrawElementsIBMPROC glMultiModeDrawElementsIBM = NULL;
#endif /* GL_IBM_multimode_draw_arrays */

/* GL_IBM_vertex_array_lists */

#ifdef GL_IBM_vertex_array_lists
glColorPointerListIBMPROC glColorPointerListIBM = NULL;
glSecondaryColorPointerListIBMPROC glSecondaryColorPointerListIBM = NULL;
glEdgeFlagPointerListIBMPROC glEdgeFlagPointerListIBM = NULL;
glFogCoordPointerListIBMPROC glFogCoordPointerListIBM = NULL;
glIndexPointerListIBMPROC glIndexPointerListIBM = NULL;
glNormalPointerListIBMPROC glNormalPointerListIBM = NULL;
glTexCoordPointerListIBMPROC glTexCoordPointerListIBM = NULL;
glVertexPointerListIBMPROC glVertexPointerListIBM = NULL;
#endif /* GL_IBM_vertex_array_lists */

/* GL_INTEL_parallel_arrays */

#ifdef GL_INTEL_parallel_arrays
glVertexPointervINTELPROC glVertexPointervINTEL = NULL;
glNormalPointervINTELPROC glNormalPointervINTEL = NULL;
glColorPointervINTELPROC glColorPointervINTEL = NULL;
glTexCoordPointervINTELPROC glTexCoordPointervINTEL = NULL;
#endif /* GL_INTEL_parallel_arrays */

/* GL_KTX_buffer_region */

#ifdef GL_KTX_buffer_region
glBufferRegionEnabledPROC glBufferRegionEnabled = NULL;
glNewBufferRegionPROC glNewBufferRegion = NULL;
glDeleteBufferRegionPROC glDeleteBufferRegion = NULL;
glReadBufferRegionPROC glReadBufferRegion = NULL;
glDrawBufferRegionPROC glDrawBufferRegion = NULL;
#endif /* GL_KTX_buffer_region */

/* GL_MESA_resize_buffers */

#ifdef GL_MESA_resize_buffers
glResizeBuffersMESAPROC glResizeBuffersMESA = NULL;
#endif /* GL_MESA_resize_buffers */

/* GL_MESA_window_pos */

#ifdef GL_MESA_window_pos
glWindowPos2dMESAPROC glWindowPos2dMESA = NULL;
glWindowPos2dvMESAPROC glWindowPos2dvMESA = NULL;
glWindowPos2fMESAPROC glWindowPos2fMESA = NULL;
glWindowPos2fvMESAPROC glWindowPos2fvMESA = NULL;
glWindowPos2iMESAPROC glWindowPos2iMESA = NULL;
glWindowPos2ivMESAPROC glWindowPos2ivMESA = NULL;
glWindowPos2sMESAPROC glWindowPos2sMESA = NULL;
glWindowPos2svMESAPROC glWindowPos2svMESA = NULL;
glWindowPos3dMESAPROC glWindowPos3dMESA = NULL;
glWindowPos3dvMESAPROC glWindowPos3dvMESA = NULL;
glWindowPos3fMESAPROC glWindowPos3fMESA = NULL;
glWindowPos3fvMESAPROC glWindowPos3fvMESA = NULL;
glWindowPos3iMESAPROC glWindowPos3iMESA = NULL;
glWindowPos3ivMESAPROC glWindowPos3ivMESA = NULL;
glWindowPos3sMESAPROC glWindowPos3sMESA = NULL;
glWindowPos3svMESAPROC glWindowPos3svMESA = NULL;
glWindowPos4dMESAPROC glWindowPos4dMESA = NULL;
glWindowPos4dvMESAPROC glWindowPos4dvMESA = NULL;
glWindowPos4fMESAPROC glWindowPos4fMESA = NULL;
glWindowPos4fvMESAPROC glWindowPos4fvMESA = NULL;
glWindowPos4iMESAPROC glWindowPos4iMESA = NULL;
glWindowPos4ivMESAPROC glWindowPos4ivMESA = NULL;
glWindowPos4sMESAPROC glWindowPos4sMESA = NULL;
glWindowPos4svMESAPROC glWindowPos4svMESA = NULL;
#endif /* GL_MESA_window_pos */

/* GL_NV_element_array */

#ifdef GL_NV_element_array
glElementPointerNVPROC glElementPointerNV = NULL;
glDrawElementArrayNVPROC glDrawElementArrayNV = NULL;
glDrawRangeElementArrayNVPROC glDrawRangeElementArrayNV = NULL;
glMultiDrawElementArrayNVPROC glMultiDrawElementArrayNV = NULL;
glMultiDrawRangeElementArrayNVPROC glMultiDrawRangeElementArrayNV = NULL;
#endif /* GL_NV_element_array */

/* GL_NV_evaluators */

#ifdef GL_NV_evaluators
glMapControlPointsNVPROC glMapControlPointsNV = NULL;
glMapParameterivNVPROC glMapParameterivNV = NULL;
glMapParameterfvNVPROC glMapParameterfvNV = NULL;
glGetMapControlPointsNVPROC glGetMapControlPointsNV = NULL;
glGetMapParameterivNVPROC glGetMapParameterivNV = NULL;
glGetMapParameterfvNVPROC glGetMapParameterfvNV = NULL;
glGetMapAttribParameterivNVPROC glGetMapAttribParameterivNV = NULL;
glGetMapAttribParameterfvNVPROC glGetMapAttribParameterfvNV = NULL;
glEvalMapsNVPROC glEvalMapsNV = NULL;
#endif /* GL_NV_evaluators */

/* GL_NV_fence */

#ifdef GL_NV_fence
glGenFencesNVPROC glGenFencesNV = NULL;
glDeleteFencesNVPROC glDeleteFencesNV = NULL;
glSetFenceNVPROC glSetFenceNV = NULL;
glTestFenceNVPROC glTestFenceNV = NULL;
glFinishFenceNVPROC glFinishFenceNV = NULL;
glIsFenceNVPROC glIsFenceNV = NULL;
glGetFenceivNVPROC glGetFenceivNV = NULL;
#endif /* GL_NV_fence */

/* GL_NV_fragment_program */

#ifdef GL_NV_fragment_program
glProgramNamedParameter4fNVPROC glProgramNamedParameter4fNV = NULL;
glProgramNamedParameter4dNVPROC glProgramNamedParameter4dNV = NULL;
glProgramNamedParameter4fvNVPROC glProgramNamedParameter4fvNV = NULL;
glProgramNamedParameter4dvNVPROC glProgramNamedParameter4dvNV = NULL;
glGetProgramNamedParameterfvNVPROC glGetProgramNamedParameterfvNV = NULL;
glGetProgramNamedParameterdvNVPROC glGetProgramNamedParameterdvNV = NULL;
#endif /* GL_NV_fragment_program */

/* GL_NV_half_float */

#ifdef GL_NV_half_float
glVertex2hNVPROC glVertex2hNV = NULL;
glVertex2hvNVPROC glVertex2hvNV = NULL;
glVertex3hNVPROC glVertex3hNV = NULL;
glVertex3hvNVPROC glVertex3hvNV = NULL;
glVertex4hNVPROC glVertex4hNV = NULL;
glVertex4hvNVPROC glVertex4hvNV = NULL;
glNormal3hNVPROC glNormal3hNV = NULL;
glNormal3hvNVPROC glNormal3hvNV = NULL;
glColor3hNVPROC glColor3hNV = NULL;
glColor3hvNVPROC glColor3hvNV = NULL;
glColor4hNVPROC glColor4hNV = NULL;
glColor4hvNVPROC glColor4hvNV = NULL;
glTexCoord1hNVPROC glTexCoord1hNV = NULL;
glTexCoord1hvNVPROC glTexCoord1hvNV = NULL;
glTexCoord2hNVPROC glTexCoord2hNV = NULL;
glTexCoord2hvNVPROC glTexCoord2hvNV = NULL;
glTexCoord3hNVPROC glTexCoord3hNV = NULL;
glTexCoord3hvNVPROC glTexCoord3hvNV = NULL;
glTexCoord4hNVPROC glTexCoord4hNV = NULL;
glTexCoord4hvNVPROC glTexCoord4hvNV = NULL;
glMultiTexCoord1hNVPROC glMultiTexCoord1hNV = NULL;
glMultiTexCoord1hvNVPROC glMultiTexCoord1hvNV = NULL;
glMultiTexCoord2hNVPROC glMultiTexCoord2hNV = NULL;
glMultiTexCoord2hvNVPROC glMultiTexCoord2hvNV = NULL;
glMultiTexCoord3hNVPROC glMultiTexCoord3hNV = NULL;
glMultiTexCoord3hvNVPROC glMultiTexCoord3hvNV = NULL;
glMultiTexCoord4hNVPROC glMultiTexCoord4hNV = NULL;
glMultiTexCoord4hvNVPROC glMultiTexCoord4hvNV = NULL;
glFogCoordhNVPROC glFogCoordhNV = NULL;
glFogCoordhvNVPROC glFogCoordhvNV = NULL;
glSecondaryColor3hNVPROC glSecondaryColor3hNV = NULL;
glSecondaryColor3hvNVPROC glSecondaryColor3hvNV = NULL;
glVertexWeighthNVPROC glVertexWeighthNV = NULL;
glVertexWeighthvNVPROC glVertexWeighthvNV = NULL;
glVertexAttrib1hNVPROC glVertexAttrib1hNV = NULL;
glVertexAttrib1hvNVPROC glVertexAttrib1hvNV = NULL;
glVertexAttrib2hNVPROC glVertexAttrib2hNV = NULL;
glVertexAttrib2hvNVPROC glVertexAttrib2hvNV = NULL;
glVertexAttrib3hNVPROC glVertexAttrib3hNV = NULL;
glVertexAttrib3hvNVPROC glVertexAttrib3hvNV = NULL;
glVertexAttrib4hNVPROC glVertexAttrib4hNV = NULL;
glVertexAttrib4hvNVPROC glVertexAttrib4hvNV = NULL;
glVertexAttribs1hvNVPROC glVertexAttribs1hvNV = NULL;
glVertexAttribs2hvNVPROC glVertexAttribs2hvNV = NULL;
glVertexAttribs3hvNVPROC glVertexAttribs3hvNV = NULL;
glVertexAttribs4hvNVPROC glVertexAttribs4hvNV = NULL;
#endif /* GL_NV_half_float */

/* GL_NV_occlusion_query */

#ifdef GL_NV_occlusion_query
glGenOcclusionQueriesNVPROC glGenOcclusionQueriesNV = NULL;
glDeleteOcclusionQueriesNVPROC glDeleteOcclusionQueriesNV = NULL;
glIsOcclusionQueryNVPROC glIsOcclusionQueryNV = NULL;
glBeginOcclusionQueryNVPROC glBeginOcclusionQueryNV = NULL;
glEndOcclusionQueryNVPROC glEndOcclusionQueryNV = NULL;
glGetOcclusionQueryivNVPROC glGetOcclusionQueryivNV = NULL;
glGetOcclusionQueryuivNVPROC glGetOcclusionQueryuivNV = NULL;
#endif /* GL_NV_occlusion_query */

/* GL_NV_pixel_data_range */

#ifdef GL_NV_pixel_data_range
glPixelDataRangeNVPROC glPixelDataRangeNV = NULL;
glFlushPixelDataRangeNVPROC glFlushPixelDataRangeNV = NULL;
#endif /* GL_NV_pixel_data_range */

/* GL_NV_point_sprite */

#ifdef GL_NV_point_sprite
glPointParameteriNVPROC glPointParameteriNV = NULL;
glPointParameterivNVPROC glPointParameterivNV = NULL;
#endif /* GL_NV_point_sprite */

/* GL_NV_primitive_restart */

#ifdef GL_NV_primitive_restart
glPrimitiveRestartNVPROC glPrimitiveRestartNV = NULL;
glPrimitiveRestartIndexNVPROC glPrimitiveRestartIndexNV = NULL;
#endif /* GL_NV_primitive_restart */

/* GL_NV_register_combiners2 */

#ifdef GL_NV_register_combiners2
glCombinerStageParameterfvNVPROC glCombinerStageParameterfvNV = NULL;
glGetCombinerStageParameterfvNVPROC glGetCombinerStageParameterfvNV = NULL;
#endif /* GL_NV_register_combiners2 */

/* GL_NV_vertex_array_range */

#ifdef GL_NV_vertex_array_range
glFlushVertexArrayRangeNVPROC glFlushVertexArrayRangeNV = NULL;
glVertexArrayRangeNVPROC glVertexArrayRangeNV = NULL;

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
wglAllocateMemoryNVPROC wglAllocateMemoryNV = NULL;
wglFreeMemoryNVPROC wglFreeMemoryNV = NULL;
#else
glXAllocateMemoryNVPROC glXAllocateMemoryNV = NULL;
glXFreeMemoryNVPROC glXFreeMemoryNV = NULL;
#endif /* WIN32 */

#endif /* GL_NV_vertex_array_range */

/* GL_NV_vertex_program */

#ifdef GL_NV_vertex_program
glBindProgramNVPROC glBindProgramNV = NULL;
glDeleteProgramsNVPROC glDeleteProgramsNV = NULL;
glExecuteProgramNVPROC glExecuteProgramNV = NULL;
glGenProgramsNVPROC glGenProgramsNV = NULL;
glAreProgramsResidentNVPROC glAreProgramsResidentNV = NULL;
glRequestResidentProgramsNVPROC glRequestResidentProgramsNV = NULL;
glGetProgramParameterfvNVPROC glGetProgramParameterfvNV = NULL;
glGetProgramParameterdvNVPROC glGetProgramParameterdvNV = NULL;
glGetProgramivNVPROC glGetProgramivNV = NULL;
glGetProgramStringNVPROC glGetProgramStringNV = NULL;
glGetTrackMatrixivNVPROC glGetTrackMatrixivNV = NULL;
glGetVertexAttribdvNVPROC glGetVertexAttribdvNV = NULL;
glGetVertexAttribfvNVPROC glGetVertexAttribfvNV = NULL;
glGetVertexAttribivNVPROC glGetVertexAttribivNV = NULL;
glGetVertexAttribPointervNVPROC glGetVertexAttribPointervNV = NULL;
glIsProgramNVPROC glIsProgramNV = NULL;
glLoadProgramNVPROC glLoadProgramNV = NULL;
glProgramParameter4fNVPROC glProgramParameter4fNV = NULL;
glProgramParameter4dNVPROC glProgramParameter4dNV = NULL;
glProgramParameter4dvNVPROC glProgramParameter4dvNV = NULL;
glProgramParameter4fvNVPROC glProgramParameter4fvNV = NULL;
glProgramParameters4dvNVPROC glProgramParameters4dvNV = NULL;
glProgramParameters4fvNVPROC glProgramParameters4fvNV = NULL;
glTrackMatrixNVPROC glTrackMatrixNV = NULL;
glVertexAttribPointerNVPROC glVertexAttribPointerNV = NULL;
glVertexAttrib1sNVPROC glVertexAttrib1sNV = NULL;
glVertexAttrib1fNVPROC glVertexAttrib1fNV = NULL;
glVertexAttrib1dNVPROC glVertexAttrib1dNV = NULL;
glVertexAttrib2sNVPROC glVertexAttrib2sNV = NULL;
glVertexAttrib2fNVPROC glVertexAttrib2fNV = NULL;
glVertexAttrib2dNVPROC glVertexAttrib2dNV = NULL;
glVertexAttrib3sNVPROC glVertexAttrib3sNV = NULL;
glVertexAttrib3fNVPROC glVertexAttrib3fNV = NULL;
glVertexAttrib3dNVPROC glVertexAttrib3dNV = NULL;
glVertexAttrib4sNVPROC glVertexAttrib4sNV = NULL;
glVertexAttrib4fNVPROC glVertexAttrib4fNV = NULL;
glVertexAttrib4dNVPROC glVertexAttrib4dNV = NULL;
glVertexAttrib4ubNVPROC glVertexAttrib4ubNV = NULL;
glVertexAttrib1svNVPROC glVertexAttrib1svNV = NULL;
glVertexAttrib1fvNVPROC glVertexAttrib1fvNV = NULL;
glVertexAttrib1dvNVPROC glVertexAttrib1dvNV = NULL;
glVertexAttrib2svNVPROC glVertexAttrib2svNV = NULL;
glVertexAttrib2fvNVPROC glVertexAttrib2fvNV = NULL;
glVertexAttrib2dvNVPROC glVertexAttrib2dvNV = NULL;
glVertexAttrib3svNVPROC glVertexAttrib3svNV = NULL;
glVertexAttrib3fvNVPROC glVertexAttrib3fvNV = NULL;
glVertexAttrib3dvNVPROC glVertexAttrib3dvNV = NULL;
glVertexAttrib4svNVPROC glVertexAttrib4svNV = NULL;
glVertexAttrib4fvNVPROC glVertexAttrib4fvNV = NULL;
glVertexAttrib4dvNVPROC glVertexAttrib4dvNV = NULL;
glVertexAttrib4ubvNVPROC glVertexAttrib4ubvNV = NULL;
glVertexAttribs1svNVPROC glVertexAttribs1svNV = NULL;
glVertexAttribs1fvNVPROC glVertexAttribs1fvNV = NULL;
glVertexAttribs1dvNVPROC glVertexAttribs1dvNV = NULL;
glVertexAttribs2svNVPROC glVertexAttribs2svNV = NULL;
glVertexAttribs2fvNVPROC glVertexAttribs2fvNV = NULL;
glVertexAttribs2dvNVPROC glVertexAttribs2dvNV = NULL;
glVertexAttribs3svNVPROC glVertexAttribs3svNV = NULL;
glVertexAttribs3fvNVPROC glVertexAttribs3fvNV = NULL;
glVertexAttribs3dvNVPROC glVertexAttribs3dvNV = NULL;
glVertexAttribs4svNVPROC glVertexAttribs4svNV = NULL;
glVertexAttribs4fvNVPROC glVertexAttribs4fvNV = NULL;
glVertexAttribs4dvNVPROC glVertexAttribs4dvNV = NULL;
glVertexAttribs4ubvNVPROC glVertexAttribs4ubvNV = NULL;
#endif /* GL_NV_vertex_program */

/* GL_PGI_misc_hints */

#ifdef GL_PGI_misc_hints
glHintPGIPROC glHintPGI = NULL;
#endif /* GL_PGI_misc_hints */

/* GL_SGI_color_table */

#ifdef GL_SGI_color_table
glColorTableSGIPROC glColorTableSGI = NULL;
glColorTableParameterfvSGIPROC glColorTableParameterfvSGI = NULL;
glColorTableParameterivSGIPROC glColorTableParameterivSGI = NULL;
glCopyColorTableSGIPROC glCopyColorTableSGI = NULL;
glGetColorTableSGIPROC glGetColorTableSGI = NULL;
glGetColorTableParameterfvSGIPROC glGetColorTableParameterfvSGI = NULL;
glGetColorTableParameterivSGIPROC glGetColorTableParameterivSGI = NULL;
#endif /* GL_SGI_color_table */

/* GL_SGIS_detail_texture */

#ifdef GL_SGIS_detail_texture
glDetailTexFuncSGISPROC glDetailTexFuncSGIS = NULL;
glGetDetailTexFuncSGISPROC glGetDetailTexFuncSGIS = NULL;
#endif /* GL_SGIS_detail_texture */

/* GL_SGIS_fog_function */

#ifdef GL_SGIS_fog_function
glFogFuncSGISPROC glFogFuncSGIS = NULL;
glGetFogFuncSGISPROC glGetFogFuncSGIS = NULL;
#endif /* GL_SGIS_fog_function */

/* GL_SGIS_multisample */

#ifdef GL_SGIS_multisample
glSampleMaskSGISPROC glSampleMaskSGIS = NULL;
glSamplePatternSGISPROC glSamplePatternSGIS = NULL;
#endif /* GL_SGIS_multisample */

/* GL_SGIS_multitexture */

#ifdef GL_SGIS_multitexture
glSelectTextureSGISPROC glSelectTextureSGIS = NULL;
glSelectTextureTransformSGISPROC glSelectTextureTransformSGIS = NULL;
glSelectTextureCoordsetSGISPROC glSelectTextureCoordsetSGIS = NULL;
glMultitexCoord1dSGISPROC glMultitexCoord1dSGIS = NULL;
glMultitexCoord1dvSGISPROC glMultitexCoord1dvSGIS = NULL;
glMultitexCoord1fSGISPROC glMultitexCoord1fSGIS = NULL;
glMultitexCoord1fvSGISPROC glMultitexCoord1fvSGIS = NULL;
glMultitexCoord1iSGISPROC glMultitexCoord1iSGIS = NULL;
glMultitexCoord1ivSGISPROC glMultitexCoord1ivSGIS = NULL;
glMultitexCoord1sSGISPROC glMultitexCoord1sSGIS = NULL;
glMultitexCoord1svSGISPROC glMultitexCoord1svSGIS = NULL;
glMultitexCoord2dSGISPROC glMultitexCoord2dSGIS = NULL;
glMultitexCoord2dvSGISPROC glMultitexCoord2dvSGIS = NULL;
glMultitexCoord2fSGISPROC glMultitexCoord2fSGIS = NULL;
glMultitexCoord2fvSGISPROC glMultitexCoord2fvSGIS = NULL;
glMultitexCoord2iSGISPROC glMultitexCoord2iSGIS = NULL;
glMultitexCoord2ivSGISPROC glMultitexCoord2ivSGIS = NULL;
glMultitexCoord2sSGISPROC glMultitexCoord2sSGIS = NULL;
glMultitexCoord2svSGISPROC glMultitexCoord2svSGIS = NULL;
glMultitexCoord3dSGISPROC glMultitexCoord3dSGIS = NULL;
glMultitexCoord3dvSGISPROC glMultitexCoord3dvSGIS = NULL;
glMultitexCoord3fSGISPROC glMultitexCoord3fSGIS = NULL;
glMultitexCoord3fvSGISPROC glMultitexCoord3fvSGIS = NULL;
glMultitexCoord3iSGISPROC glMultitexCoord3iSGIS = NULL;
glMultitexCoord3ivSGISPROC glMultitexCoord3ivSGIS = NULL;
glMultitexCoord3sSGISPROC glMultitexCoord3sSGIS = NULL;
glMultitexCoord3svSGISPROC glMultitexCoord3svSGIS = NULL;
glMultitexCoord4dSGISPROC glMultitexCoord4dSGIS = NULL;
glMultitexCoord4dvSGISPROC glMultitexCoord4dvSGIS = NULL;
glMultitexCoord4fSGISPROC glMultitexCoord4fSGIS = NULL;
glMultitexCoord4fvSGISPROC glMultitexCoord4fvSGIS = NULL;
glMultitexCoord4iSGISPROC glMultitexCoord4iSGIS = NULL;
glMultitexCoord4ivSGISPROC glMultitexCoord4ivSGIS = NULL;
glMultitexCoord4sSGISPROC glMultitexCoord4sSGIS = NULL;
glMultitexCoord4svSGISPROC glMultitexCoord4svSGIS = NULL;
#endif /* GL_SGIS_multitexture */

/* GL_SGIS_pixel_texture */

#ifdef GL_SGIS_pixel_texture
glPixelTexGenParameteriSGISPROC glPixelTexGenParameteriSGIS = NULL;
glPixelTexGenParameterivSGISPROC glPixelTexGenParameterivSGIS = NULL;
glPixelTexGenParameterfSGISPROC glPixelTexGenParameterfSGIS = NULL;
glPixelTexGenParameterfvSGISPROC glPixelTexGenParameterfvSGIS = NULL;
glGetPixelTexGenParameterivSGISPROC glGetPixelTexGenParameterivSGIS = NULL;
glGetPixelTexGenParameterfvSGISPROC glGetPixelTexGenParameterfvSGIS = NULL;
#endif /* GL_SGIS_pixel_texture */

/* GL_SGIS_point_parameters */

#ifdef GL_SGIS_point_parameters
glPointParameterfSGISPROC glPointParameterfSGIS = NULL;
glPointParameterfvSGISPROC glPointParameterfvSGIS = NULL;
#endif /* GL_SGIS_point_parameters */

/* GL_SGIS_sharpen_texture */

#ifdef GL_SGIS_sharpen_texture
glSharpenTexFuncSGISPROC glSharpenTexFuncSGIS = NULL;
glGetSharpenTexFuncSGISPROC glGetSharpenTexFuncSGIS = NULL;
#endif /* GL_SGIS_sharpen_texture */

/* GL_SGIS_texture4D */

#ifdef GL_SGIS_texture4D
glTexImage4DSGISPROC glTexImage4DSGIS = NULL;
glTexSubImage4DSGISPROC glTexSubImage4DSGIS = NULL;
#endif /* GL_SGIS_texture4D */

/* GL_SGIS_texture_color_mask */

#ifdef GL_SGIS_texture_color_mask
glTextureColorMaskSGISPROC glTextureColorMaskSGIS = NULL;
#endif /* GL_SGIS_texture_color_mask */

/* GL_SGIS_texture_filter4 */

#ifdef GL_SGIS_texture_filter4
glGetTexFilterFuncSGISPROC glGetTexFilterFuncSGIS = NULL;
glTexFilterFuncSGISPROC glTexFilterFuncSGIS = NULL;
#endif /* GL_SGIS_texture_filter4 */

/* GL_SGIX_async */

#ifdef GL_SGIX_async
glAsyncMarkerSGIXPROC glAsyncMarkerSGIX = NULL;
glFinishAsyncSGIXPROC glFinishAsyncSGIX = NULL;
glPollAsyncSGIXPROC glPollAsyncSGIX = NULL;
glGenAsyncMarkersSGIXPROC glGenAsyncMarkersSGIX = NULL;
glDeleteAsyncMarkersSGIXPROC glDeleteAsyncMarkersSGIX = NULL;
glIsAsyncMarkerSGIXPROC glIsAsyncMarkerSGIX = NULL;
#endif /* GL_SGIX_async */

/* GL_SGIX_flush_raster */

#ifdef GL_SGIX_flush_raster
glFlushRasterSGIXPROC glFlushRasterSGIX = NULL;
#endif /* GL_SGIX_flush_raster */

/* GL_SGIX_fragment_lighting */

#ifdef GL_SGIX_fragment_lighting
glFragmentColorMaterialSGIXPROC glFragmentColorMaterialSGIX = NULL;
glFragmentLightfSGIXPROC glFragmentLightfSGIX = NULL;
glFragmentLightfvSGIXPROC glFragmentLightfvSGIX = NULL;
glFragmentLightiSGIXPROC glFragmentLightiSGIX = NULL;
glFragmentLightivSGIXPROC glFragmentLightivSGIX = NULL;
glFragmentLightModelfSGIXPROC glFragmentLightModelfSGIX = NULL;
glFragmentLightModelfvSGIXPROC glFragmentLightModelfvSGIX = NULL;
glFragmentLightModeliSGIXPROC glFragmentLightModeliSGIX = NULL;
glFragmentLightModelivSGIXPROC glFragmentLightModelivSGIX = NULL;
glFragmentMaterialfSGIXPROC glFragmentMaterialfSGIX = NULL;
glFragmentMaterialfvSGIXPROC glFragmentMaterialfvSGIX = NULL;
glFragmentMaterialiSGIXPROC glFragmentMaterialiSGIX = NULL;
glFragmentMaterialivSGIXPROC glFragmentMaterialivSGIX = NULL;
glGetFragmentLightfvSGIXPROC glGetFragmentLightfvSGIX = NULL;
glGetFragmentLightivSGIXPROC glGetFragmentLightivSGIX = NULL;
glGetFragmentMaterialfvSGIXPROC glGetFragmentMaterialfvSGIX = NULL;
glGetFragmentMaterialivSGIXPROC glGetFragmentMaterialivSGIX = NULL;
glLightEnviSGIXPROC glLightEnviSGIX = NULL;
#endif /* GL_SGIX_fragment_lighting */

/* GL_SGIX_framezoom */

#ifdef GL_SGIX_framezoom
glFrameZoomSGIXPROC glFrameZoomSGIX = NULL;
#endif /* GL_SGIX_framezoom */

/* GL_SGIX_instruments */

#ifdef GL_SGIX_instruments
glGetInstrumentsSGIXPROC glGetInstrumentsSGIX = NULL;
glInstrumentsBufferSGIXPROC glInstrumentsBufferSGIX = NULL;
glPollInstrumentsSGIXPROC glPollInstrumentsSGIX = NULL;
glReadInstrumentsSGIXPROC glReadInstrumentsSGIX = NULL;
glStartInstrumentsSGIXPROC glStartInstrumentsSGIX = NULL;
glStopInstrumentsSGIXPROC glStopInstrumentsSGIX = NULL;
#endif /* GL_SGIX_instruments */

/* GL_SGIX_list_priority */

#ifdef GL_SGIX_list_priority
glGetListParameterfvSGIXPROC glGetListParameterfvSGIX = NULL;
glGetListParameterivSGIXPROC glGetListParameterivSGIX = NULL;
glListParameterfSGIXPROC glListParameterfSGIX = NULL;
glListParameterfvSGIXPROC glListParameterfvSGIX = NULL;
glListParameteriSGIXPROC glListParameteriSGIX = NULL;
glListParameterivSGIXPROC glListParameterivSGIX = NULL;
#endif /* GL_SGIX_list_priority */

/* GL_SGIX_pixel_texture */

#ifdef GL_SGIX_pixel_texture
glPixelTexGenSGIXPROC glPixelTexGenSGIX = NULL;
#endif /* GL_SGIX_pixel_texture */

/* GL_SGIX_polynomial_ffd */

#ifdef GL_SGIX_polynomial_ffd
glDeformationMap3dSGIXPROC glDeformationMap3dSGIX = NULL;
glDeformationMap3fSGIXPROC glDeformationMap3fSGIX = NULL;
glDeformSGIXPROC glDeformSGIX = NULL;
glLoadIdentityDeformationMapSGIXPROC glLoadIdentityDeformationMapSGIX = NULL;
#endif /* GL_SGIX_polynomial_ffd */

/* GL_SGIX_reference_plane */

#ifdef GL_SGIX_reference_plane
glReferencePlaneSGIXPROC glReferencePlaneSGIX = NULL;
#endif /* GL_SGIX_reference_plane */

/* GL_SGIX_sprite */

#ifdef GL_SGIX_sprite
glSpriteParameterfSGIXPROC glSpriteParameterfSGIX = NULL;
glSpriteParameterfvSGIXPROC glSpriteParameterfvSGIX = NULL;
glSpriteParameteriSGIXPROC glSpriteParameteriSGIX = NULL;
glSpriteParameterivSGIXPROC glSpriteParameterivSGIX = NULL;
#endif /* GL_SGIX_sprite */

/* GL_SGIX_tag_sample_buffer */

#ifdef GL_SGIX_tag_sample_buffer
glTagSampleBufferSGIXPROC glTagSampleBufferSGIX = NULL;
#endif /* GL_SGIX_tag_sample_buffer */

/* GL_SUN_global_alpha */

#ifdef GL_SUN_global_alpha
glGlobalAlphaFactorbSUNPROC glGlobalAlphaFactorbSUN = NULL;
glGlobalAlphaFactorsSUNPROC glGlobalAlphaFactorsSUN = NULL;
glGlobalAlphaFactoriSUNPROC glGlobalAlphaFactoriSUN = NULL;
glGlobalAlphaFactorfSUNPROC glGlobalAlphaFactorfSUN = NULL;
glGlobalAlphaFactordSUNPROC glGlobalAlphaFactordSUN = NULL;
glGlobalAlphaFactorubSUNPROC glGlobalAlphaFactorubSUN = NULL;
glGlobalAlphaFactorusSUNPROC glGlobalAlphaFactorusSUN = NULL;
glGlobalAlphaFactoruiSUNPROC glGlobalAlphaFactoruiSUN = NULL;
#endif /* GL_SUN_global_alpha */

/* GL_SUN_mesh_array */

#ifdef GL_SUN_mesh_array
glDrawMeshArraysSUNPROC glDrawMeshArraysSUN = NULL;
#endif /* GL_SUN_mesh_array */

/* GL_SUN_multi_draw_arrays */

#ifdef GL_SUN_multi_draw_arrays
glMultidrawArraysSUNPROC glMultidrawArraysSUN = NULL;
glMultidrawElementsSUNPROC glMultidrawElementsSUN = NULL;
#endif /* GL_SUN_multi_draw_arrays */

/* GL_SUN_triangle_list */

#ifdef GL_SUN_triangle_list
glReplacementCodeuiSUNPROC glReplacementCodeuiSUN = NULL;
glReplacementCodeusSUNPROC glReplacementCodeusSUN = NULL;
glReplacementCodeubSUNPROC glReplacementCodeubSUN = NULL;
glReplacementCodeuivSUNPROC glReplacementCodeuivSUN = NULL;
glReplacementCodeusvSUNPROC glReplacementCodeusvSUN = NULL;
glReplacementCodeubvSUNPROC glReplacementCodeubvSUN = NULL;
glReplacementCodePointerSUNPROC glReplacementCodePointerSUN = NULL;
#endif /* GL_SUN_triangle_list */

/* GL_SUN_vertex */

#ifdef GL_SUN_vertex
glColor4ubVertex2fSUNPROC glColor4ubVertex2fSUN = NULL;
glColor4ubVertex2fvSUNPROC glColor4ubVertex2fvSUN = NULL;
glColor4ubVertex3fSUNPROC glColor4ubVertex3fSUN = NULL;
glColor4ubVertex3fvSUNPROC glColor4ubVertex3fvSUN = NULL;
glColor3fVertex3fSUNPROC glColor3fVertex3fSUN = NULL;
glColor3fVertex3fvSUNPROC glColor3fVertex3fvSUN = NULL;
glNormal3fVertex3fSUNPROC glNormal3fVertex3fSUN = NULL;
glNormal3fVertex3fvSUNPROC glNormal3fVertex3fvSUN = NULL;
glColor4fNormal3fVertex3fSUNPROC glColor4fNormal3fVertex3fSUN = NULL;
glColor4fNormal3fVertex3fvSUNPROC glColor4fNormal3fVertex3fvSUN = NULL;
glTexCoord2fVertex3fSUNPROC glTexCoord2fVertex3fSUN = NULL;
glTexCoord2fVertex3fvSUNPROC glTexCoord2fVertex3fvSUN = NULL;
glTexCoord4fVertex4fSUNPROC glTexCoord4fVertex4fSUN = NULL;
glTexCoord4fVertex4fvSUNPROC glTexCoord4fVertex4fvSUN = NULL;
glTexCoord2fColor4ubVertex3fSUNPROC glTexCoord2fColor4ubVertex3fSUN = NULL;
glTexCoord2fColor4ubVertex3fvSUNPROC glTexCoord2fColor4ubVertex3fvSUN = NULL;
glTexCoord2fColor3fVertex3fSUNPROC glTexCoord2fColor3fVertex3fSUN = NULL;
glTexCoord2fColor3fVertex3fvSUNPROC glTexCoord2fColor3fVertex3fvSUN = NULL;
glTexCoord2fNormal3fVertex3fSUNPROC glTexCoord2fNormal3fVertex3fSUN = NULL;
glTexCoord2fNormal3fVertex3fvSUNPROC glTexCoord2fNormal3fVertex3fvSUN = NULL;
glTexCoord2fColor4fNormal3fVertex3fSUNPROC glTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
glTexCoord2fColor4fNormal3fVertex3fvSUNPROC glTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
glTexCoord4fColor4fNormal3fVertex4fSUNPROC glTexCoord4fColor4fNormal3fVertex4fSUN = NULL;
glTexCoord4fColor4fNormal3fVertex4fvSUNPROC glTexCoord4fColor4fNormal3fVertex4fvSUN = NULL;
glReplacementCodeuiVertex3fSUNPROC glReplacementCodeuiVertex3fSUN = NULL;
glReplacementCodeuiVertex3fvSUNPROC glReplacementCodeuiVertex3fvSUN = NULL;
glReplacementCodeuiColor4ubVertex3fSUNPROC glReplacementCodeuiColor4ubVertex3fSUN = NULL;
glReplacementCodeuiColor4ubVertex3fvSUNPROC glReplacementCodeuiColor4ubVertex3fvSUN = NULL;
glReplacementCodeuiColor3fVertex3fSUNPROC glReplacementCodeuiColor3fVertex3fSUN = NULL;
glReplacementCodeuiColor3fVertex3fvSUNPROC glReplacementCodeuiColor3fVertex3fvSUN = NULL;
glReplacementCodeuiNormal3fVertex3fSUNPROC glReplacementCodeuiNormal3fVertex3fSUN = NULL;
glReplacementCodeuiNormal3fVertex3fvSUNPROC glReplacementCodeuiNormal3fVertex3fvSUN = NULL;
glReplacementCodeuiColor4fNormal3fVertex3fSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fSUN = NULL;
glReplacementCodeuiColor4fNormal3fVertex3fvSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fvSUN = NULL;
glReplacementCodeuiTexCoord2fVertex3fSUNPROC glReplacementCodeuiTexCoord2fVertex3fSUN = NULL;
glReplacementCodeuiTexCoord2fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fVertex3fvSUN = NULL;
glReplacementCodeuiTexCoord2fNormal3fVertex3fSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = NULL;
glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = NULL;
glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
#endif /* GL_SUN_vertex */

/* GL_SUNX_constant_data */

#ifdef GL_SUNX_constant_data
glFinishTextureSUNXPROC glFinishTextureSUNX = NULL;
#endif /* GL_SUNX_constant_data */

/* GL_WIN_swap_hint */

#ifdef GL_WIN_swap_hint
glAddSwapHintRectWINPROC glAddSwapHintRectWIN = NULL;
#endif /* GL_WIN_swap_hint */

/* other definitions */
static int extgl_error = 0;

struct ExtensionTypes extgl_Extensions;

struct ExtensionTypes SupportedExtensions; /* deprecated, please do not use */


char extgl_ExtString[5000] = "";
const GLubyte *g_extensions = NULL;

/** returns true if the extention is available */
bool QueryExtension(const char *name)
{
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(name, ' ');
    if (where || *name == '\0')
        return false;
    /* It takes a bit of care to be fool-proof about parsing the
         OpenGL extensions string. Don't be fooled by sub-strings,
        etc. */
    start = g_extensions;
    for (;;)
    {
        where = (GLubyte *) strstr((const char *) start, name);
        if (!where)
            break;
        terminator = where + strlen(name);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
            {
                sprintf(extgl_ExtString, "%s%s ", extgl_ExtString, name);
                return true;
            }

        start = terminator;
    }
    return false;
}

int extgl_ExtensionSupported(const char *name)
{
    char tmp[100];
    sprintf(tmp, "%s ", name);
    if (strstr(extgl_ExtString, tmp))
        return 1;
    return 0;
}

/* getProcAddress */
void *extgl_GetProcAddress(char *name)
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
    void *t = (void*)wglGetProcAddress(name);
    if (t == NULL)
    {
        extgl_error = 1;
    }
    return t;
#else
    void *t = (void*)glXGetProcAddressARB((char*)name);
    if (t == NULL)
    {
        extgl_error = 1;
    }
    return t;
#endif
}

/*-----------------------------------------------------*/
/* WGL stuff */
/*-----------------------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

void extgl_InitWGLARBBufferRegion()
{
#ifdef WGL_ARB_buffer_region
    if (!extgl_Extensions.wgl.ARB_buffer_region)
        return;
    wglCreateBufferRegionARB = (wglCreateBufferRegionARBPROC) extgl_GetProcAddress("wglCreateBufferRegionARB");
    wglDeleteBufferRegionARB = (wglDeleteBufferRegionARBPROC) extgl_GetProcAddress("wglDeleteBufferRegionARB");
    wglSaveBufferRegionARB = (wglSaveBufferRegionARBPROC) extgl_GetProcAddress("wglSaveBufferRegionARB");
    wglRestoreBufferRegionARB = (wglRestoreBufferRegionARBPROC) extgl_GetProcAddress("wglRestoreBufferRegionARB");
#endif
}

void extgl_InitWGLARBMakeCurrentRead()
{
#ifdef WGL_ARB_make_current_read
    if (!extgl_Extensions.wgl.ARB_make_current_read)
        return;
    wglMakeContextCurrentARB = (wglMakeContextCurrentARBPROC) extgl_GetProcAddress("wglMakeContextCurrentARB");
    wglGetCurrentReadDCARB = (wglGetCurrentReadDCARBPROC) extgl_GetProcAddress("wglGetCurrentReadDCARB");
#endif
}

void extgl_InitWGLARBPbuffer()
{
#ifdef WGL_ARB_pbuffer
    if (!extgl_Extensions.wgl.ARB_pbuffer)
        return;
    wglCreatePbufferARB = (wglCreatePbufferARBPROC) extgl_GetProcAddress("wglCreatePbufferARB");
    wglGetPbufferDCARB = (wglGetPbufferDCARBPROC) extgl_GetProcAddress("wglGetPbufferDCARB");
    wglReleasePbufferDCARB = (wglReleasePbufferDCARBPROC) extgl_GetProcAddress("wglReleasePbufferDCARB");
    wglDestroyPbufferARB = (wglDestroyPbufferARBPROC) extgl_GetProcAddress("wglDestroyPbufferARB");
    wglQueryPbufferARB = (wglQueryPbufferARBPROC) extgl_GetProcAddress("wglQueryPbufferARB");
#endif
}

void extgl_InitWGLARBPixelFormat()
{
#ifdef WGL_ARB_pixel_format
    if (!extgl_Extensions.wgl.ARB_pixel_format)
        return;
    wglGetPixelFormatAttribivARB = (wglGetPixelFormatAttribivARBPROC) extgl_GetProcAddress("wglGetPixelFormatAttribivARB");
    wglGetPixelFormatAttribfvARB = (wglGetPixelFormatAttribfvARBPROC) extgl_GetProcAddress("wglGetPixelFormatAttribfvARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARBPROC) extgl_GetProcAddress("wglChoosePixelFormatARB");
#endif
}

void extgl_InitWGLARBRenderTexture()
{
#ifdef WGL_ARB_render_texture
    if (!extgl_Extensions.wgl.ARB_render_texture)
        return;
    wglBindTexImageARB = (wglBindTexImageARBPROC) extgl_GetProcAddress("wglBindTexImageARB");
    wglReleaseTexImageARB = (wglReleaseTexImageARBPROC) extgl_GetProcAddress("wglReleaseTexImageARB");
    wglSetPbufferAttribARB = (wglSetPbufferAttribARBPROC) extgl_GetProcAddress("wglSetPbufferAttribARB");
#endif
}

void extgl_InitWGLEXTDisplayColorTable()
{
#ifdef WGL_EXT_display_color_table
	if (!extgl_Extensions.wgl.EXT_display_color_table)
		return;
	wglCreateDisplayColorTableEXT = (wglCreateDisplayColorTableEXTPROC) extgl_GetProcAddress("wglCreateDisplayColorTableEXT");
	wglLoadDisplayColorTableEXT = (wglLoadDisplayColorTableEXTPROC) extgl_GetProcAddress("wglLoadDisplayColorTableEXT");
	wglBindDisplayColorTableEXT = (wglBindDisplayColorTableEXTPROC) extgl_GetProcAddress("wglBindDisplayColorTableEXT");
	wglDestroyDisplayColorTableEXT = (wglDestroyDisplayColorTableEXTPROC) extgl_GetProcAddress("wglDestroyDisplayColorTableEXT");
#endif
}

void extgl_InitWGLEXTMakeCurrentRead()
{
#ifdef WGL_EXT_make_current_read
    if (!extgl_Extensions.wgl.EXT_make_current_read)
        return;
	wglMakeContextCurrentEXT = (wglMakeContextCurrentEXTPROC) extgl_GetProcAddress("wglMakeContextCurrentEXT");
	wglGetCurrentReadDCEXT = (wglGetCurrentReadDCEXTPROC) extgl_GetProcAddress("wglGetCurrentReadDCEXT");
#endif
}

void extgl_InitWGLEXTPbuffer()
{
#ifdef WGL_EXT_pbuffer
    if (!extgl_Extensions.wgl.EXT_pbuffer)
        return;
	wglCreatePbufferEXT = (wglCreatePbufferEXTPROC) extgl_GetProcAddress("wglCreatePbufferEXT");
	wglGetPbufferDCEXT = (wglGetPbufferDCEXTPROC) extgl_GetProcAddress("wglGetPbufferDCEXT");
	wglReleasePbufferDCEXT = (wglReleasePbufferDCEXTPROC) extgl_GetProcAddress("wglReleasePbufferDCEXT");
	wglDestroyPbufferEXT = (wglDestroyPbufferEXTPROC) extgl_GetProcAddress("wglDestroyPbufferEXT");
	wglQueryPbufferEXT = (wglQueryPbufferEXTPROC) extgl_GetProcAddress("wglQueryPbufferEXT");
#endif
}

void extgl_InitWGLEXTPixelFormat()
{
#ifdef WGL_EXT_pixel_format
    if (!extgl_Extensions.wgl.EXT_pixel_format)
        return;
	wglGetPixelFormatAttribivEXT = (wglGetPixelFormatAttribivEXTPROC) extgl_GetProcAddress("wglGetPixelFormatAttribivEXT");
	wglGetPixelFormatAttribfvEXT = (wglGetPixelFormatAttribfvEXTPROC) extgl_GetProcAddress("wglGetPixelFormatAttribfvEXT");
	wglChoosePixelFormatEXT = (wglChoosePixelFormatEXTPROC) extgl_GetProcAddress("wglChoosePixelFormatEXT");
#endif
}

void extgl_InitWGLEXTSwapControl()
{
#ifdef WGL_EXT_swap_control
    if (!extgl_Extensions.wgl.EXT_swap_control)
        return;
    wglSwapIntervalEXT = (wglSwapIntervalEXTPROC) extgl_GetProcAddress("wglSwapIntervalEXT");
    wglGetSwapIntervalEXT = (wglGetSwapIntervalEXTPROC) extgl_GetProcAddress("wglGetSwapIntervalEXT");
#endif
}

void extgl_InitWGLI3DDigitalVideoControl()
{
#ifdef WGL_I3D_digital_video_control
    if (!extgl_Extensions.wgl.I3D_digital_video_control)
        return;
	wglGetDigitalVideoParametersI3D = (wglGetDigitalVideoParametersI3DPROC) extgl_GetProcAddress("wglGetDigitalVideoParametersI3D");
	wglSetDigitalVideoParametersI3D = (wglSetDigitalVideoParametersI3DPROC) extgl_GetProcAddress("wglSetDigitalVideoParametersI3D");
#endif
}

void extgl_InitWGLI3DGamma()
{
#ifdef WGL_I3D_gamma
    if (!extgl_Extensions.wgl.I3D_gamma)
        return;
	wglGetGammaTableParametersI3D = (wglGetGammaTableParametersI3DPROC) extgl_GetProcAddress("wglGetGammaTableParametersI3D");
	wglSetGammaTableParametersI3D = (wglSetGammaTableParametersI3DPROC) extgl_GetProcAddress("wglSetGammaTableParametersI3D");
	wglGetGammaTableI3D = (wglGetGammaTableI3DPROC) extgl_GetProcAddress("wglGetGammaTableI3D");
	wglSetGammaTableI3D = (wglSetGammaTableI3DPROC) extgl_GetProcAddress("wglSetGammaTableI3D");
#endif
}

void extgl_InitWGLI3DGenlock()
{
#ifdef WGL_I3D_genlock
    if (!extgl_Extensions.wgl.I3D_genlock)
        return;
	wglEnableGenlockI3D = (wglEnableGenlockI3DPROC) extgl_GetProcAddress("wglEnableGenlockI3D");
	wglDisableGenlockI3D = (wglDisableGenlockI3DPROC) extgl_GetProcAddress("wglDisableGenlockI3D");
	wglIsEnabledGenlockI3D = (wglIsEnabledGenlockI3DPROC) extgl_GetProcAddress("wglIsEnabledGenlockI3D");
	wglGenlockSourceI3D = (wglGenlockSourceI3DPROC) extgl_GetProcAddress("wglGenlockSourceI3D");
	wglGetGenlockSourceI3D = (wglGetGenlockSourceI3DPROC) extgl_GetProcAddress("wglGetGenlockSourceI3D");
	wglGenlockSourceEdgeI3D = (wglGenlockSourceEdgeI3DPROC) extgl_GetProcAddress("wglGenlockSourceEdgeI3D");
	wglGetGenlockSourceEdgeI3D = (wglGetGenlockSourceEdgeI3DPROC) extgl_GetProcAddress("wglGetGenlockSourceEdgeI3D");
	wglGenlockSampleRateI3D = (wglGenlockSampleRateI3DPROC) extgl_GetProcAddress("wglGenlockSampleRateI3D");
	wglGetGenlockSampleRateI3D = (wglGetGenlockSampleRateI3DPROC) extgl_GetProcAddress("wglGetGenlockSampleRateI3D");
	wglGenlockSourceDelayI3D = (wglGenlockSourceDelayI3DPROC) extgl_GetProcAddress("wglGenlockSourceDelayI3D");
	wglGetGenlockSourceDelayI3D = (wglGetGenlockSourceDelayI3DPROC) extgl_GetProcAddress("wglGetGenlockSourceDelayI3D");
	wglQueryGenlockMaxSourceDelayI3D = (wglQueryGenlockMaxSourceDelayI3DPROC) extgl_GetProcAddress("wglQueryGenlockMaxSourceDelayI3D");
#endif
}

void extgl_InitWGLI3DImageBuffer()
{
#ifdef WGL_I3D_image_buffer
    if (!extgl_Extensions.wgl.I3D_image_buffer)
        return;
	wglCreateImageBufferI3D = (wglCreateImageBufferI3DPROC) extgl_GetProcAddress("wglCreateImageBufferI3D");
	wglDestroyImageBufferI3D = (wglDestroyImageBufferI3DPROC) extgl_GetProcAddress("wglDestroyImageBufferI3D");
	wglAssociateImageBufferEventsI3D = (wglAssociateImageBufferEventsI3DPROC) extgl_GetProcAddress("wglAssociateImageBufferEventsI3D");
	wglReleaseImageBufferEventsI3D = (wglReleaseImageBufferEventsI3DPROC) extgl_GetProcAddress("wglReleaseImageBufferEventsI3D");
#endif
}

void extgl_InitWGLI3DSwapFrameLock()
{
#ifdef WGL_I3D_swap_frame_lock
    if (!extgl_Extensions.wgl.I3D_swap_frame_lock)
        return;
	wglEnableFrameLockI3D = (wglEnableFrameLockI3DPROC) extgl_GetProcAddress("wglEnableFrameLockI3D");
	wglDisableFrameLockI3D = (wglDisableFrameLockI3DPROC) extgl_GetProcAddress("wglDisableFrameLockI3D");
	wglIsEnabledFrameLockI3D = (wglIsEnabledFrameLockI3DPROC) extgl_GetProcAddress("wglIsEnabledFrameLockI3D");
	wglQueryFrameLockMasterI3D = (wglQueryFrameLockMasterI3DPROC) extgl_GetProcAddress("wglQueryFrameLockMasterI3D");
#endif
}


void extgl_InitWGLI3DSwapFrameUsage()
{
#ifdef WGL_I3D_swap_frame_usage
	if (!extgl_Extensions.wgl.I3D_swap_frame_usage)
		return;
	wglGetFrameUsageI3D = (wglGetFrameUsageI3DPROC) extgl_GetProcAddress("wglGetFrameUsageI3D");
	wglBeginFrameTrackingI3D = (wglBeginFrameTrackingI3DPROC) extgl_GetProcAddress("wglBeginFrameTrackingI3D");
	wglEndFrameTrackingI3D = (wglEndFrameTrackingI3DPROC) extgl_GetProcAddress("wglEndFrameTrackingI3D");
	wglQueryFrameTrackingI3D = (wglQueryFrameTrackingI3DPROC) extgl_GetProcAddress("wglQueryFrameTrackingI3D");
#endif
}

const GLubyte *g_wextensions = NULL;

/** returns true if the extention is available */
bool QueryWGLExtension(const char *name)
{
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(name, ' ');
    if (where || *name == '\0')
        return 0;
	if (g_wextensions == NULL) return FALSE;
    /* It takes a bit of care to be fool-proof about parsing the
         OpenGL extensions string. Don't be fooled by sub-strings,
        etc. */
    start = g_wextensions;
    for (;;)
    {
        where = (GLubyte *) strstr((const char *) start, name);
        if (!where)
            break;
        terminator = where + strlen(name);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return true;
        start = terminator;
    }
    return false;
}

void extgl_InitSupportedWGLExtensions()
{
	extgl_Extensions.wgl.WGL3DFX_multisample = QueryWGLExtension("WGL_3DFX_multisample");
	extgl_Extensions.wgl.ARB_buffer_region = QueryWGLExtension("WGL_ARB_buffer_region");
	extgl_Extensions.wgl.ARB_make_current_read = QueryWGLExtension("WGL_ARB_make_current_read");
	extgl_Extensions.wgl.ARB_multisample = QueryWGLExtension("WGL_ARB_multisample");
	extgl_Extensions.wgl.ARB_pbuffer = QueryWGLExtension("WGL_ARB_pbuffer");
	extgl_Extensions.wgl.ARB_pixel_format = QueryWGLExtension("WGL_ARB_pixel_format");
	extgl_Extensions.wgl.ARB_render_texture = QueryWGLExtension("WGL_ARB_render_texture");
	extgl_Extensions.wgl.ATI_pixel_format_float = QueryWGLExtension("WGL_ATI_pixel_format_float");
	extgl_Extensions.wgl.EXT_depth_float = QueryWGLExtension("WGL_EXT_depth_float");
	extgl_Extensions.wgl.EXT_display_color_table = QueryWGLExtension("WGL_EXT_display_color_table");
	extgl_Extensions.wgl.EXT_make_current_read = QueryWGLExtension("WGL_EXT_make_current_read");
	extgl_Extensions.wgl.EXT_multisample = QueryWGLExtension("WGL_EXT_multisample");
	extgl_Extensions.wgl.EXT_pixel_format = QueryWGLExtension("WGL_EXT_pixel_format");
	extgl_Extensions.wgl.EXT_swap_control = QueryWGLExtension("WGL_EXT_swap_control");
    extgl_Extensions.wgl.EXT_pbuffer = QueryWGLExtension("WGL_EXT_pbuffer");
    extgl_Extensions.wgl.I3D_digital_video_control = QueryWGLExtension("WGL_I3D_digital_video_control");
    extgl_Extensions.wgl.I3D_gamma = QueryWGLExtension("WGL_I3D_gamma");
    extgl_Extensions.wgl.I3D_genlock = QueryWGLExtension("WGL_I3D_genlock");
    extgl_Extensions.wgl.I3D_image_buffer = QueryWGLExtension("WGL_I3D_image_buffer");
	extgl_Extensions.wgl.I3D_swap_frame_lock = QueryWGLExtension("WGL_I3D_swap_frame_lock");
	extgl_Extensions.wgl.I3D_swap_frame_usage = QueryWGLExtension("WGL_I3D_swap_frame_usage");
	extgl_Extensions.wgl.NV_float_buffer = QueryWGLExtension("WGL_NV_float_buffer");
	extgl_Extensions.wgl.NV_render_depth_texture = QueryWGLExtension("WGL_NV_render_depth_texture");
	extgl_Extensions.wgl.NV_render_texture_rectangle = QueryWGLExtension("WGL_NV_render_texture_rectangle");
}

int extgl_InitializeWGL()
{
	extgl_error = 0;
	wglGetExtensionsStringARB = (wglGetExtensionsStringARBPROC) extgl_GetProcAddress("wglGetExtensionsStringARB");
	wglGetExtensionsStringEXT = (wglGetExtensionsStringEXTPROC) extgl_GetProcAddress("wglGetExtensionsStringEXT");
	extgl_Extensions.wgl.ARB_extensions_string = wglGetExtensionsStringARB != NULL;
	extgl_Extensions.wgl.EXT_extensions_string = wglGetExtensionsStringEXT != NULL;
	extgl_error = 0;
	if (wglGetExtensionsStringARB == NULL)
		if (wglGetExtensionsStringEXT == NULL)
			return false;
		else
			g_wextensions = (GLubyte*)wglGetExtensionsStringEXT();
	else
		g_wextensions = (GLubyte*)wglGetExtensionsStringARB(wglGetCurrentDC());

	extgl_InitSupportedWGLExtensions();

	extgl_InitWGLARBBufferRegion();
	extgl_InitWGLARBMakeCurrentRead();
	extgl_InitWGLARBPbuffer();
	extgl_InitWGLARBPixelFormat();
	extgl_InitWGLARBRenderTexture();
	extgl_InitWGLEXTDisplayColorTable();
	extgl_InitWGLEXTMakeCurrentRead();
	extgl_InitWGLEXTPbuffer();
	extgl_InitWGLEXTPixelFormat();
	extgl_InitWGLEXTSwapControl();
	extgl_InitWGLI3DDigitalVideoControl();
	extgl_InitWGLI3DGamma();
	extgl_InitWGLI3DGenlock();
	extgl_InitWGLI3DImageBuffer();
	extgl_InitWGLI3DSwapFrameLock();
	extgl_InitWGLI3DSwapFrameUsage();

    return extgl_error;
}

#endif /* WIN32 */

/*-----------------------------------------------------*/
/* WGL stuff END*/
/*-----------------------------------------------------*/

void extgl_InitAPPLEElementArray()
{
#ifdef GL_APPLE_element_array
    if (!extgl_Extensions.APPLE_element_array)
        return;
	glElementPointerAPPLE = (glElementPointerAPPLEPROC) extgl_GetProcAddress("glElementPointerAPPLE");
	glDrawElementArrayAPPLE = (glDrawElementArrayAPPLEPROC) extgl_GetProcAddress("glDrawElementArrayAPPLE");
	glDrawRangeElementArrayAPPLE = (glDrawRangeElementArrayAPPLEPROC) extgl_GetProcAddress("glDrawRangeElementArrayAPPLE");
	glMultiDrawElementArrayAPPLE = (glMultiDrawElementArrayAPPLEPROC) extgl_GetProcAddress("glMultiDrawElementArrayAPPLE");
	glMultiDrawRangeElementArrayAPPLE = (glMultiDrawRangeElementArrayAPPLEPROC) extgl_GetProcAddress("glMultiDrawRangeElementArrayAPPLE");
#endif /* GL_APPLE_element_array */
}
void extgl_InitAPPLEFence()
{
#ifdef GL_APPLE_fence
    if (!extgl_Extensions.APPLE_fence)
        return;
	glGenFencesAPPLE = (glGenFencesAPPLEPROC) extgl_GetProcAddress("glGenFencesAPPLE");
	glDeleteFencesAPPLE = (glDeleteFencesAPPLEPROC) extgl_GetProcAddress("glDeleteFencesAPPLE");
	glSetFenceAPPLE = (glSetFenceAPPLEPROC) extgl_GetProcAddress("glSetFenceAPPLE");
	glIsFenceAPPLE = (glIsFenceAPPLEPROC) extgl_GetProcAddress("glIsFenceAPPLE");
	glTestFenceAPPLE = (glTestFenceAPPLEPROC) extgl_GetProcAddress("glTestFenceAPPLE");
	glFinishFenceAPPLE = (glFinishFenceAPPLEPROC) extgl_GetProcAddress("glFinishFenceAPPLE");
	glTestObjectAPPLE = (glTestObjectAPPLEPROC) extgl_GetProcAddress("glTestObjectAPPLE");
	glFinishObjectAPPLE = (glFinishObjectAPPLEPROC) extgl_GetProcAddress("glFinishObjectAPPLE");
#endif /* GL_APPLE_fence */
}
void extgl_InitAPPLEVertexArrayObject()
{
#ifdef GL_APPLE_vertex_array_object
    if (!extgl_Extensions.APPLE_vertex_array_object)
        return;
	glBindVertexArrayAPPLE = (glBindVertexArrayAPPLEPROC) extgl_GetProcAddress("glBindVertexArrayAPPLE");
	glDeleteVertexArraysAPPLE = (glDeleteVertexArraysAPPLEPROC) extgl_GetProcAddress("glDeleteVertexArraysAPPLE");
	glGenVertexArraysAPPLE = (glGenVertexArraysAPPLEPROC) extgl_GetProcAddress("glGenVertexArraysAPPLE");
	glIsVertexArrayAPPLE = (glIsVertexArrayAPPLEPROC) extgl_GetProcAddress("glIsVertexArrayAPPLE");
#endif /* GL_APPLE_vertex_array_object */
}
void extgl_InitAPPLEVertexArrayRange()
{
#ifdef GL_APPLE_vertex_array_range
    if (!extgl_Extensions.APPLE_vertex_array_range)
        return;
	glVertexArrayRangeAPPLE = (glVertexArrayRangeAPPLEPROC) extgl_GetProcAddress("glVertexArrayRangeAPPLE");
	glFlushVertexArrayRangeAPPLE = (glFlushVertexArrayRangeAPPLEPROC) extgl_GetProcAddress("glFlushVertexArrayRangeAPPLE");
	glVertexArrayParameteriAPPLE = (glVertexArrayParameteriAPPLEPROC) extgl_GetProcAddress("glVertexArrayParameteriAPPLE");
#endif /* GL_APPLE_vertex_array_range */
}

void extgl_InitARBFragmentProgram()
{
#ifdef GL_ARB_fragment_program
    if (!extgl_Extensions.ARB_fragment_program)
        return;
    glProgramStringARB = (glProgramStringARBPROC) extgl_GetProcAddress("glProgramStringARB");
    glBindProgramARB = (glBindProgramARBPROC) extgl_GetProcAddress("glBindProgramARB");
    glDeleteProgramsARB = (glDeleteProgramsARBPROC) extgl_GetProcAddress("glDeleteProgramsARB");
    glGenProgramsARB = (glGenProgramsARBPROC) extgl_GetProcAddress("glGenProgramsARB");
    glProgramEnvParameter4dARB = (glProgramEnvParameter4dARBPROC) extgl_GetProcAddress("glProgramEnvParameter4dARB");
    glProgramEnvParameter4dvARB = (glProgramEnvParameter4dvARBPROC) extgl_GetProcAddress("glProgramEnvParameter4dvARB");
    glProgramEnvParameter4fARB = (glProgramEnvParameter4fARBPROC) extgl_GetProcAddress("glProgramEnvParameter4fARB");
    glProgramEnvParameter4fvARB = (glProgramEnvParameter4fvARBPROC) extgl_GetProcAddress("glProgramEnvParameter4fvARB");
    glProgramLocalParameter4dARB = (glProgramLocalParameter4dARBPROC) extgl_GetProcAddress("glProgramLocalParameter4dARB");
    glProgramLocalParameter4dvARB = (glProgramLocalParameter4dvARBPROC) extgl_GetProcAddress("glProgramLocalParameter4dvARB");
    glProgramLocalParameter4fARB = (glProgramLocalParameter4fARBPROC) extgl_GetProcAddress("glProgramLocalParameter4fARB");
    glProgramLocalParameter4fvARB = (glProgramLocalParameter4fvARBPROC) extgl_GetProcAddress("glProgramLocalParameter4fvARB");
    glGetProgramEnvParameterdvARB = (glGetProgramEnvParameterdvARBPROC) extgl_GetProcAddress("glGetProgramEnvParameterdvARB");
    glGetProgramEnvParameterfvARB = (glGetProgramEnvParameterfvARBPROC) extgl_GetProcAddress("glGetProgramEnvParameterfvARB");
    glGetProgramLocalParameterdvARB = (glGetProgramLocalParameterdvARBPROC) extgl_GetProcAddress("glGetProgramLocalParameterdvARB");
    glGetProgramLocalParameterfvARB = (glGetProgramLocalParameterfvARBPROC) extgl_GetProcAddress("glGetProgramLocalParameterfvARB");
    glGetProgramivARB = (glGetProgramivARBPROC) extgl_GetProcAddress("glGetProgramivARB");
    glGetProgramStringARB = (glGetProgramStringARBPROC) extgl_GetProcAddress("glGetProgramStringARB");
    glIsProgramARB = (glIsProgramARBPROC) extgl_GetProcAddress("glIsProgramARB");
#endif  /* GL_ARB_fragment_program */
}

void extgl_InitARBFragmentProgramShadow()
{
#ifdef GL_ARB_fragment_program_shadow
    if (!extgl_Extensions.ARB_fragment_program_shadow)
        return;
#endif  /* GL_ARB_fragment_program_shadow */
}


void extgl_InitARBFragmentShader()
{
#ifdef GL_ARB_fragment_shader
    if (!extgl_Extensions.ARB_fragment_shader)
        return;
#endif  /* GL_ARB_fragment_shader */
}

void extgl_InitARBImaging()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_ARB_imaging
    if (!extgl_Extensions.ARB_imaging)
        return;
    glBlendColor = (glBlendColorPROC) extgl_GetProcAddress("glBlendColor");
    glBlendEquation = (glBlendEquationPROC) extgl_GetProcAddress("glBlendEquation");
    glColorTable = (glColorTablePROC) extgl_GetProcAddress("glColorTable");
    glColorTableParameterfv = (glColorTableParameterfvPROC) extgl_GetProcAddress("glColorTableParameterfv");
    glColorTableParameteriv = (glColorTableParameterivPROC) extgl_GetProcAddress("glColorTableParameteriv");
    glCopyColorTable = (glCopyColorTablePROC) extgl_GetProcAddress("glCopyColorTable");
    glGetColorTable = (glGetColorTablePROC) extgl_GetProcAddress("glGetColorTable");
    glGetColorTableParameterfv = (glGetColorTableParameterfvPROC) extgl_GetProcAddress("glGetColorTableParameterfv");
    glGetColorTableParameteriv = (glGetColorTableParameterivPROC) extgl_GetProcAddress("glGetColorTableParameteriv");
    glColorSubTable = (glColorSubTablePROC) extgl_GetProcAddress("glColorSubTable");
    glCopyColorSubTable = (glCopyColorSubTablePROC) extgl_GetProcAddress("glCopyColorSubTable");
    glConvolutionFilter1D = (glConvolutionFilter1DPROC) extgl_GetProcAddress("glConvolutionFilter1D");
    glConvolutionFilter2D = (glConvolutionFilter2DPROC) extgl_GetProcAddress("glConvolutionFilter2D");
    glConvolutionParameterf = (glConvolutionParameterfPROC) extgl_GetProcAddress("glConvolutionParameterf");
    glConvolutionParameterfv = (glConvolutionParameterfvPROC) extgl_GetProcAddress("glConvolutionParameterfv");
    glConvolutionParameteri = (glConvolutionParameteriPROC) extgl_GetProcAddress("glConvolutionParameteri");
    glConvolutionParameteriv = (glConvolutionParameterivPROC) extgl_GetProcAddress("glConvolutionParameteriv");
    glCopyConvolutionFilter1D = (glCopyConvolutionFilter1DPROC) extgl_GetProcAddress("glCopyConvolutionFilter1D");
    glCopyConvolutionFilter2D = (glCopyConvolutionFilter2DPROC) extgl_GetProcAddress("glCopyConvolutionFilter2D");
    glGetConvolutionFilter = (glGetConvolutionFilterPROC) extgl_GetProcAddress("glGetConvolutionFilter");
    glGetConvolutionParameterfv = (glGetConvolutionParameterfvPROC) extgl_GetProcAddress("glGetConvolutionParameterfv");
    glGetConvolutionParameteriv = (glGetConvolutionParameterivPROC) extgl_GetProcAddress("glGetConvolutionParameteriv");
    glGetSeparableFilter = (glGetSeparableFilterPROC) extgl_GetProcAddress("glGetSeparableFilter");
    glSeparableFilter2D = (glSeparableFilter2DPROC) extgl_GetProcAddress("glSeparableFilter2D");
    glGetHistogram = (glGetHistogramPROC) extgl_GetProcAddress("glGetHistogram");
    glGetHistogramParameterfv = (glGetHistogramParameterfvPROC) extgl_GetProcAddress("glGetHistogramParameterfv");
    glGetHistogramParameteriv = (glGetHistogramParameterivPROC) extgl_GetProcAddress("glGetHistogramParameteriv");
    glGetMinmax = (glGetMinmaxPROC) extgl_GetProcAddress("glGetMinmax");
    glGetMinmaxParameterfv = (glGetMinmaxParameterfvPROC) extgl_GetProcAddress("glGetMinmaxParameterfv");
    glGetMinmaxParameteriv = (glGetMinmaxParameterivPROC) extgl_GetProcAddress("glGetMinmaxParameteriv");
    glHistogram = (glHistogramPROC) extgl_GetProcAddress("glHistogram");
    glMinmax = (glMinmaxPROC) extgl_GetProcAddress("glMinmax");
    glResetHistogram = (glResetHistogramPROC) extgl_GetProcAddress("glResetHistogram");
    glResetMinmax = (glResetMinmaxPROC) extgl_GetProcAddress("glResetMinmax");
#endif /* GL_ARB_imaging */
#endif /* WIN32 */
}

void extgl_InitARBMatrixPalette()
{
#ifdef GL_ARB_matrix_palette
    if (!extgl_Extensions.ARB_matrix_palette)
        return;
    glCurrentPaletteMatrixARB = (glCurrentPaletteMatrixARBPROC) extgl_GetProcAddress("glCurrentPaletteMatrixARB");
    glMatrixIndexubvARB = (glMatrixIndexubvARBPROC) extgl_GetProcAddress("glMatrixIndexubvARB");
    glMatrixIndexusvARB = (glMatrixIndexusvARBPROC) extgl_GetProcAddress("glMatrixIndexusvARB");
    glMatrixIndexuivARB = (glMatrixIndexuivARBPROC) extgl_GetProcAddress("glMatrixIndexuivARB");
    glMatrixIndexPointerARB = (glMatrixIndexPointerARBPROC) extgl_GetProcAddress("glMatrixIndexPointerARB");
#endif
}

void extgl_InitARBMultisample()
{
#ifdef GL_ARB_multisample
    if (!extgl_Extensions.ARB_multisample)
        return;
    glSampleCoverageARB = (glSampleCoverageARBPROC) extgl_GetProcAddress("glSampleCoverageARB");
#endif
}

void extgl_InitARBMultitexture()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_ARB_multitexture
    if (!extgl_Extensions.ARB_multitexture)
        return;
    glActiveTextureARB = (glActiveTextureARBPROC) extgl_GetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB = (glClientActiveTextureARBPROC) extgl_GetProcAddress("glClientActiveTextureARB");

    glMultiTexCoord1dARB = (glMultiTexCoord1dARBPROC) extgl_GetProcAddress("glMultiTexCoord1dARB");
    glMultiTexCoord1dvARB = (glMultiTexCoord1dvARBPROC) extgl_GetProcAddress("glMultiTexCoord1dvARB");
    glMultiTexCoord1fARB = (glMultiTexCoord1fARBPROC) extgl_GetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord1fvARB = (glMultiTexCoord1fvARBPROC) extgl_GetProcAddress("glMultiTexCoord1fvARB");
    glMultiTexCoord1iARB = (glMultiTexCoord1iARBPROC) extgl_GetProcAddress("glMultiTexCoord1iARB");
    glMultiTexCoord1ivARB = (glMultiTexCoord1ivARBPROC) extgl_GetProcAddress("glMultiTexCoord1ivARB");
    glMultiTexCoord1sARB = (glMultiTexCoord1sARBPROC) extgl_GetProcAddress("glMultiTexCoord1sARB");
    glMultiTexCoord1svARB = (glMultiTexCoord1svARBPROC) extgl_GetProcAddress("glMultiTexCoord1svARB");

    glMultiTexCoord2dARB = (glMultiTexCoord2dARBPROC) extgl_GetProcAddress("glMultiTexCoord2dARB");
    glMultiTexCoord2dvARB = (glMultiTexCoord2dvARBPROC) extgl_GetProcAddress("glMultiTexCoord2dvARB");
    glMultiTexCoord2fARB = (glMultiTexCoord2fARBPROC) extgl_GetProcAddress("glMultiTexCoord2fARB");
    glMultiTexCoord2fvARB = (glMultiTexCoord2fvARBPROC) extgl_GetProcAddress("glMultiTexCoord2fvARB");
    glMultiTexCoord2iARB = (glMultiTexCoord2iARBPROC) extgl_GetProcAddress("glMultiTexCoord2iARB");
    glMultiTexCoord2ivARB = (glMultiTexCoord2ivARBPROC) extgl_GetProcAddress("glMultiTexCoord2ivARB");
    glMultiTexCoord2sARB = (glMultiTexCoord2sARBPROC) extgl_GetProcAddress("glMultiTexCoord2sARB");
    glMultiTexCoord2svARB = (glMultiTexCoord2svARBPROC) extgl_GetProcAddress("glMultiTexCoord2svARB");

    glMultiTexCoord3dARB = (glMultiTexCoord3dARBPROC) extgl_GetProcAddress("glMultiTexCoord3dARB");
    glMultiTexCoord3dvARB = (glMultiTexCoord3dvARBPROC) extgl_GetProcAddress("glMultiTexCoord3dvARB");
    glMultiTexCoord3fARB = (glMultiTexCoord3fARBPROC) extgl_GetProcAddress("glMultiTexCoord3fARB");
    glMultiTexCoord3fvARB = (glMultiTexCoord3fvARBPROC) extgl_GetProcAddress("glMultiTexCoord3fvARB");
    glMultiTexCoord3iARB = (glMultiTexCoord3iARBPROC) extgl_GetProcAddress("glMultiTexCoord3iARB");
    glMultiTexCoord3ivARB = (glMultiTexCoord3ivARBPROC) extgl_GetProcAddress("glMultiTexCoord3ivARB");
    glMultiTexCoord3sARB = (glMultiTexCoord3sARBPROC) extgl_GetProcAddress("glMultiTexCoord3sARB");
    glMultiTexCoord3svARB = (glMultiTexCoord3svARBPROC) extgl_GetProcAddress("glMultiTexCoord3svARB");

    glMultiTexCoord4dARB = (glMultiTexCoord4dARBPROC) extgl_GetProcAddress("glMultiTexCoord4dARB");
    glMultiTexCoord4dvARB = (glMultiTexCoord4dvARBPROC) extgl_GetProcAddress("glMultiTexCoord4dvARB");
    glMultiTexCoord4fARB = (glMultiTexCoord4fARBPROC) extgl_GetProcAddress("glMultiTexCoord4fARB");
    glMultiTexCoord4fvARB = (glMultiTexCoord4fvARBPROC) extgl_GetProcAddress("glMultiTexCoord4fvARB");
    glMultiTexCoord4iARB = (glMultiTexCoord4iARBPROC) extgl_GetProcAddress("glMultiTexCoord4iARB");
    glMultiTexCoord4ivARB = (glMultiTexCoord4ivARBPROC) extgl_GetProcAddress("glMultiTexCoord4ivARB");
    glMultiTexCoord4sARB = (glMultiTexCoord4sARBPROC) extgl_GetProcAddress("glMultiTexCoord4sARB");
    glMultiTexCoord4svARB = (glMultiTexCoord4svARBPROC) extgl_GetProcAddress("glMultiTexCoord4svARB");
#endif /* GL_ARB_multitexture */
#endif /* WIN32 */
}

void extgl_InitARBOcclusionQuery()
{
#ifdef GL_ARB_occlusion_query
    if (!extgl_Extensions.ARB_occlusion_query)
        return;
	glGenQueriesARB = (glGenQueriesARBPROC) extgl_GetProcAddress("glGenQueriesARB");
	glDeleteQueriesARB = (glDeleteQueriesARBPROC) extgl_GetProcAddress("glDeleteQueriesARB");
	glIsQueryARB = (glIsQueryARBPROC) extgl_GetProcAddress("glIsQueryARB");
	glBeginQueryARB = (glBeginQueryARBPROC) extgl_GetProcAddress("glBeginQueryARB");
	glEndQueryARB = (glEndQueryARBPROC) extgl_GetProcAddress("glEndQueryARB");
	glGetQueryivARB = (glGetQueryivARBPROC) extgl_GetProcAddress("glGetQueryivARB");
	glGetQueryObjectivARB = (glGetQueryObjectivARBPROC) extgl_GetProcAddress("glGetQueryObjectivARB");
	glGetQueryObjectuivARB = (glGetQueryObjectuivARBPROC) extgl_GetProcAddress("glGetQueryObjectuivARB");
#endif
}

void extgl_InitARBShaderObjects()
{
#ifdef GL_ARB_shader_objects
    if (!extgl_Extensions.ARB_shader_objects)
        return;
	glDeleteObjectARB = (glDeleteObjectARBPROC) extgl_GetProcAddress("glDeleteObjectARB");
	glGetHandleARB = (glGetHandleARBPROC) extgl_GetProcAddress("glGetHandleARB");
	glDetachObjectARB = (glDetachObjectARBPROC) extgl_GetProcAddress("glDetachObjectARB");
	glCreateShaderObjectARB = (glCreateShaderObjectARBPROC) extgl_GetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (glShaderSourceARBPROC) extgl_GetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (glCompileShaderARBPROC) extgl_GetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB = (glCreateProgramObjectARBPROC) extgl_GetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB = (glAttachObjectARBPROC) extgl_GetProcAddress("glAttachObjectARB");
	glLinkProgramARB = (glLinkProgramARBPROC) extgl_GetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (glUseProgramObjectARBPROC) extgl_GetProcAddress("glUseProgramObjectARB");
	glValidateProgramARB = (glValidateProgramARBPROC) extgl_GetProcAddress("glValidateProgramARB");
	glUniform1fARB = (glUniform1fARBPROC) extgl_GetProcAddress("glUniform1fARB");
	glUniform2fARB = (glUniform2fARBPROC) extgl_GetProcAddress("glUniform2fARB");
	glUniform3fARB = (glUniform3fARBPROC) extgl_GetProcAddress("glUniform3fARB");
	glUniform4fARB = (glUniform4fARBPROC) extgl_GetProcAddress("glUniform4fARB");
	glUniform1iARB = (glUniform1iARBPROC) extgl_GetProcAddress("glUniform1iARB");
	glUniform2iARB = (glUniform2iARBPROC) extgl_GetProcAddress("glUniform2iARB");
	glUniform3iARB = (glUniform3iARBPROC) extgl_GetProcAddress("glUniform3iARB");
	glUniform4iARB = (glUniform4iARBPROC) extgl_GetProcAddress("glUniform4iARB");
	glUniform1fvARB = (glUniform1fvARBPROC) extgl_GetProcAddress("glUniform1fvARB");
	glUniform2fvARB = (glUniform2fvARBPROC) extgl_GetProcAddress("glUniform2fvARB");
	glUniform3fvARB = (glUniform3fvARBPROC) extgl_GetProcAddress("glUniform3fvARB");
	glUniform4fvARB = (glUniform4fvARBPROC) extgl_GetProcAddress("glUniform4fvARB");
	glUniform1ivARB = (glUniform1ivARBPROC) extgl_GetProcAddress("glUniform1ivARB");
	glUniform2ivARB = (glUniform2ivARBPROC) extgl_GetProcAddress("glUniform2ivARB");
	glUniform3ivARB = (glUniform3ivARBPROC) extgl_GetProcAddress("glUniform3ivARB");
	glUniform4ivARB = (glUniform4ivARBPROC) extgl_GetProcAddress("glUniform4ivARB");
	glUniformMatrix2fvARB = (glUniformMatrix2fvARBPROC) extgl_GetProcAddress("glUniformMatrix2fvARB");
	glUniformMatrix3fvARB = (glUniformMatrix3fvARBPROC) extgl_GetProcAddress("glUniformMatrix3fvARB");
	glUniformMatrix4fvARB = (glUniformMatrix4fvARBPROC) extgl_GetProcAddress("glUniformMatrix4fvARB");
	glGetObjectParameterfvARB = (glGetObjectParameterfvARBPROC) extgl_GetProcAddress("glGetObjectParameterfvARB");
	glGetObjectParameterivARB = (glGetObjectParameterivARBPROC) extgl_GetProcAddress("glGetObjectParameterivARB");
	glGetInfoLogARB = (glGetInfoLogARBPROC) extgl_GetProcAddress("glGetInfoLogARB");
	glGetAttachedObjectsARB = (glGetAttachedObjectsARBPROC) extgl_GetProcAddress("glGetAttachedObjectsARB");
	glGetUniformLocationARB = (glGetUniformLocationARBPROC) extgl_GetProcAddress("glGetUniformLocationARB");
	glGetActiveUniformARB = (glGetActiveUniformARBPROC) extgl_GetProcAddress("glGetActiveUniformARB");
	glGetUniformfvARB = (glGetUniformfvARBPROC) extgl_GetProcAddress("glGetUniformfvARB");
	glGetUniformivARB = (glGetUniformivARBPROC) extgl_GetProcAddress("glGetUniformivARB");
	glGetShaderSourceARB = (glGetShaderSourceARBPROC) extgl_GetProcAddress("glGetShaderSourceARB");
#endif
}

void extgl_InitARBVertexShader()
{
#ifdef GL_ARB_vertex_shader
    if (!extgl_Extensions.ARB_vertex_shader)
        return;
	glBindAttribLocationARB = (glBindAttribLocationARBPROC) extgl_GetProcAddress("glBindAttribLocationARB");
	glGetActiveAttribARB = (glGetActiveAttribARBPROC) extgl_GetProcAddress("glGetActiveAttribARB");
	glGetAttribLocationARB = (glGetAttribLocationARBPROC) extgl_GetProcAddress("glGetAttribLocationARB");
#endif
}

void extgl_InitARBPointParameters()
{
#ifdef GL_ARB_point_parameters
    if (!extgl_Extensions.ARB_point_parameters)
        return;
	glPointParameterfARB = (glPointParameterfARBPROC) extgl_GetProcAddress("glPointParameterfARB");
	glPointParameterfvARB = (glPointParameterfvARBPROC) extgl_GetProcAddress("glPointParameterfvARB");
#endif
}

void extgl_InitARBTextureCompression()
{
#ifdef GL_ARB_texture_compression
    if (!extgl_Extensions.ARB_texture_compression)
        return;
    glCompressedTexImage3DARB = (glCompressedTexImage3DARBPROC) extgl_GetProcAddress("glCompressedTexImage3DARB");
    glCompressedTexImage2DARB = (glCompressedTexImage2DARBPROC) extgl_GetProcAddress("glCompressedTexImage2DARB");
    glCompressedTexImage1DARB = (glCompressedTexImage1DARBPROC) extgl_GetProcAddress("glCompressedTexImage1DARB");
    glCompressedTexSubImage3DARB = (glCompressedTexSubImage3DARBPROC) extgl_GetProcAddress("glCompressedTexSubImage3DARB");
    glCompressedTexSubImage2DARB = (glCompressedTexSubImage2DARBPROC) extgl_GetProcAddress("glCompressedTexSubImage2DARB");
    glCompressedTexSubImage1DARB = (glCompressedTexSubImage1DARBPROC) extgl_GetProcAddress("glCompressedTexSubImage1DARB");
    glGetCompressedTexImageARB = (glGetCompressedTexImageARBPROC) extgl_GetProcAddress("glGetCompressedTexImageARB");
#endif
}

void extgl_InitARBTransposeMatrix()
{
#ifdef GL_ARB_transpose_matrix
    if (!extgl_Extensions.ARB_transpose_matrix)
        return;
    glLoadTransposeMatrixfARB = (glLoadTransposeMatrixfARBPROC) extgl_GetProcAddress("glLoadTransposeMatrixfARB");
    glLoadTransposeMatrixdARB = (glLoadTransposeMatrixdARBPROC) extgl_GetProcAddress("glLoadTransposeMatrixdARB");
    glMultTransposeMatrixfARB = (glMultTransposeMatrixfARBPROC) extgl_GetProcAddress("glMultTransposeMatrixfARB");
    glMultTransposeMatrixdARB = (glMultTransposeMatrixdARBPROC) extgl_GetProcAddress("glMultTransposeMatrixdARB");
#endif
}

void extgl_InitARBVertexBlend()
{
#ifdef GL_ARB_vertex_blend
    if (!extgl_Extensions.ARB_vertex_blend)
        return;
    glWeightbvARB = (glWeightbvARBPROC) extgl_GetProcAddress("glWeightbvARB");
    glWeightsvARB = (glWeightsvARBPROC) extgl_GetProcAddress("glWeightsvARB");
    glWeightivARB = (glWeightivARBPROC) extgl_GetProcAddress("glWeightivARB");
    glWeightfvARB = (glWeightfvARBPROC) extgl_GetProcAddress("glWeightfvARB");
    glWeightdvARB = (glWeightdvARBPROC) extgl_GetProcAddress("glWeightdvARB");
    glWeightubvARB = (glWeightubvARBPROC) extgl_GetProcAddress("glWeightubvARB");
    glWeightusvARB = (glWeightusvARBPROC) extgl_GetProcAddress("glWeightusvARB");
    glWeightuivARB = (glWeightuivARBPROC) extgl_GetProcAddress("glWeightuivARB");
    glWeightPointerARB = (glWeightPointerARBPROC) extgl_GetProcAddress("glWeightPointerARB");
    glVertexBlendARB = (glVertexBlendARBPROC) extgl_GetProcAddress("glVertexBlendARB");
#endif
}

void extgl_InitARBVertexBufferObject()
{
#ifdef GL_ARB_vertex_buffer_object
    if (!extgl_Extensions.ARB_vertex_buffer_object)
        return;
    glBindBufferARB = (glBindBufferARBPROC) extgl_GetProcAddress("glBindBufferARB");
    glDeleteBuffersARB = (glDeleteBuffersARBPROC) extgl_GetProcAddress("glDeleteBuffersARB");
    glGenBuffersARB = (glGenBuffersARBPROC) extgl_GetProcAddress("glGenBuffersARB");
    glIsBufferARB = (glIsBufferARBPROC) extgl_GetProcAddress("glIsBufferARB");
    glBufferDataARB = (glBufferDataARBPROC) extgl_GetProcAddress("glBufferDataARB");
    glBufferSubDataARB = (glBufferSubDataARBPROC) extgl_GetProcAddress("glBufferSubDataARB");
    glGetBufferSubDataARB = (glGetBufferSubDataARBPROC) extgl_GetProcAddress("glGetBufferSubDataARB");
    glMapBufferARB = (glMapBufferARBPROC) extgl_GetProcAddress("glMapBufferARB");
    glUnmapBufferARB = (glUnmapBufferARBPROC) extgl_GetProcAddress("glUnmapBufferARB");
    glGetBufferParameterivARB = (glGetBufferParameterivARBPROC) extgl_GetProcAddress("glGetBufferParameterivARB");
    glGetBufferPointervARB = (glGetBufferPointervARBPROC) extgl_GetProcAddress("glGetBufferPointervARB");
#endif /* GL_ARB_vertex_buffer_object */
}

void extgl_InitARBVertexProgram()
{
#ifdef GL_ARB_vertex_program
    if (!extgl_Extensions.ARB_vertex_program)
        return;
    glVertexAttrib1sARB = (glVertexAttrib1sARBPROC) extgl_GetProcAddress("glVertexAttrib1sARB");
    glVertexAttrib1fARB = (glVertexAttrib1fARBPROC) extgl_GetProcAddress("glVertexAttrib1fARB");
    glVertexAttrib1dARB = (glVertexAttrib1dARBPROC) extgl_GetProcAddress("glVertexAttrib1dARB");
    glVertexAttrib2sARB = (glVertexAttrib2sARBPROC) extgl_GetProcAddress("glVertexAttrib2sARB");
    glVertexAttrib2fARB = (glVertexAttrib2fARBPROC) extgl_GetProcAddress("glVertexAttrib2fARB");
    glVertexAttrib2dARB = (glVertexAttrib2dARBPROC) extgl_GetProcAddress("glVertexAttrib2dARB");
    glVertexAttrib3sARB = (glVertexAttrib3sARBPROC) extgl_GetProcAddress("glVertexAttrib3sARB");
    glVertexAttrib3fARB = (glVertexAttrib3fARBPROC) extgl_GetProcAddress("glVertexAttrib3fARB");
    glVertexAttrib3dARB = (glVertexAttrib3dARBPROC) extgl_GetProcAddress("glVertexAttrib3dARB");
    glVertexAttrib4sARB = (glVertexAttrib4sARBPROC) extgl_GetProcAddress("glVertexAttrib4sARB");
    glVertexAttrib4fARB = (glVertexAttrib4fARBPROC) extgl_GetProcAddress("glVertexAttrib4fARB");
    glVertexAttrib4dARB = (glVertexAttrib4dARBPROC) extgl_GetProcAddress("glVertexAttrib4dARB");
    glVertexAttrib4NubARB = (glVertexAttrib4NubARBPROC) extgl_GetProcAddress("glVertexAttrib4NubARB");
    glVertexAttrib1svARB = (glVertexAttrib1svARBPROC) extgl_GetProcAddress("glVertexAttrib1svARB");
    glVertexAttrib1fvARB = (glVertexAttrib1fvARBPROC) extgl_GetProcAddress("glVertexAttrib1fvARB");
    glVertexAttrib1dvARB = (glVertexAttrib1dvARBPROC) extgl_GetProcAddress("glVertexAttrib1dvARB");
    glVertexAttrib2svARB = (glVertexAttrib2svARBPROC) extgl_GetProcAddress("glVertexAttrib2svARB");
    glVertexAttrib2fvARB = (glVertexAttrib2fvARBPROC) extgl_GetProcAddress("glVertexAttrib2fvARB");
    glVertexAttrib2dvARB = (glVertexAttrib2dvARBPROC) extgl_GetProcAddress("glVertexAttrib2dvARB");
    glVertexAttrib3svARB = (glVertexAttrib3svARBPROC) extgl_GetProcAddress("glVertexAttrib3svARB");
    glVertexAttrib3fvARB = (glVertexAttrib3fvARBPROC) extgl_GetProcAddress("glVertexAttrib3fvARB");
    glVertexAttrib3dvARB = (glVertexAttrib3dvARBPROC) extgl_GetProcAddress("glVertexAttrib3dvARB");
    glVertexAttrib4bvARB = (glVertexAttrib4bvARBPROC) extgl_GetProcAddress("glVertexAttrib4bvARB");
    glVertexAttrib4svARB = (glVertexAttrib4svARBPROC) extgl_GetProcAddress("glVertexAttrib4svARB");
    glVertexAttrib4ivARB = (glVertexAttrib4ivARBPROC) extgl_GetProcAddress("glVertexAttrib4ivARB");
    glVertexAttrib4ubvARB = (glVertexAttrib4ubvARBPROC) extgl_GetProcAddress("glVertexAttrib4ubvARB");
    glVertexAttrib4usvARB = (glVertexAttrib4usvARBPROC) extgl_GetProcAddress("glVertexAttrib4usvARB");
    glVertexAttrib4uivARB = (glVertexAttrib4uivARBPROC) extgl_GetProcAddress("glVertexAttrib4uivARB");
    glVertexAttrib4fvARB = (glVertexAttrib4fvARBPROC) extgl_GetProcAddress("glVertexAttrib4fvARB");
    glVertexAttrib4dvARB = (glVertexAttrib4dvARBPROC) extgl_GetProcAddress("glVertexAttrib4dvARB");
    glVertexAttrib4NbvARB = (glVertexAttrib4NbvARBPROC) extgl_GetProcAddress("glVertexAttrib4NbvARB");
    glVertexAttrib4NsvARB = (glVertexAttrib4NsvARBPROC) extgl_GetProcAddress("glVertexAttrib4NsvARB");
    glVertexAttrib4NivARB = (glVertexAttrib4NivARBPROC) extgl_GetProcAddress("glVertexAttrib4NivARB");
    glVertexAttrib4NubvARB = (glVertexAttrib4NubvARBPROC) extgl_GetProcAddress("glVertexAttrib4NubvARB");
    glVertexAttrib4NusvARB = (glVertexAttrib4NusvARBPROC) extgl_GetProcAddress("glVertexAttrib4NusvARB");
    glVertexAttrib4NuivARB = (glVertexAttrib4NuivARBPROC) extgl_GetProcAddress("glVertexAttrib4NuivARB");
    glVertexAttribPointerARB = (glVertexAttribPointerARBPROC) extgl_GetProcAddress("glVertexAttribPointerARB");
    glEnableVertexAttribArrayARB = (glEnableVertexAttribArrayARBPROC) extgl_GetProcAddress("glEnableVertexAttribArrayARB");
    glDisableVertexAttribArrayARB = (glDisableVertexAttribArrayARBPROC) extgl_GetProcAddress("glDisableVertexAttribArrayARB");
    glProgramStringARB = (glProgramStringARBPROC) extgl_GetProcAddress("glProgramStringARB");
    glBindProgramARB = (glBindProgramARBPROC) extgl_GetProcAddress("glBindProgramARB");
    glDeleteProgramsARB = (glDeleteProgramsARBPROC) extgl_GetProcAddress("glDeleteProgramsARB");
    glGenProgramsARB = (glGenProgramsARBPROC) extgl_GetProcAddress("glGenProgramsARB");
    glProgramEnvParameter4dARB = (glProgramEnvParameter4dARBPROC) extgl_GetProcAddress("glProgramEnvParameter4dARB");
    glProgramEnvParameter4dvARB = (glProgramEnvParameter4dvARBPROC) extgl_GetProcAddress("glProgramEnvParameter4dvARB");
    glProgramEnvParameter4fARB = (glProgramEnvParameter4fARBPROC) extgl_GetProcAddress("glProgramEnvParameter4fARB");
    glProgramEnvParameter4fvARB = (glProgramEnvParameter4fvARBPROC) extgl_GetProcAddress("glProgramEnvParameter4fvARB");
    glProgramLocalParameter4dARB = (glProgramLocalParameter4dARBPROC) extgl_GetProcAddress("glProgramLocalParameter4dARB");
    glProgramLocalParameter4dvARB = (glProgramLocalParameter4dvARBPROC) extgl_GetProcAddress("glProgramLocalParameter4dvARB");
    glProgramLocalParameter4fARB = (glProgramLocalParameter4fARBPROC) extgl_GetProcAddress("glProgramLocalParameter4fARB");
    glProgramLocalParameter4fvARB = (glProgramLocalParameter4fvARBPROC) extgl_GetProcAddress("glProgramLocalParameter4fvARB");
    glGetProgramEnvParameterdvARB = (glGetProgramEnvParameterdvARBPROC) extgl_GetProcAddress("glGetProgramEnvParameterdvARB");
    glGetProgramEnvParameterfvARB = (glGetProgramEnvParameterfvARBPROC) extgl_GetProcAddress("glGetProgramEnvParameterfvARB");
    glGetProgramLocalParameterdvARB = (glGetProgramLocalParameterdvARBPROC) extgl_GetProcAddress("glGetProgramLocalParameterdvARB");
    glGetProgramLocalParameterfvARB = (glGetProgramLocalParameterfvARBPROC) extgl_GetProcAddress("glGetProgramLocalParameterfvARB");
    glGetProgramivARB = (glGetProgramivARBPROC) extgl_GetProcAddress("glGetProgramivARB");
    glGetProgramStringARB = (glGetProgramStringARBPROC) extgl_GetProcAddress("glGetProgramStringARB");
    glGetVertexAttribdvARB = (glGetVertexAttribdvARBPROC) extgl_GetProcAddress("glGetVertexAttribdvARB");
    glGetVertexAttribfvARB = (glGetVertexAttribfvARBPROC) extgl_GetProcAddress("glGetVertexAttribfvARB");
    glGetVertexAttribivARB = (glGetVertexAttribivARBPROC) extgl_GetProcAddress("glGetVertexAttribivARB");
    glGetVertexAttribPointervARB = (glGetVertexAttribPointervARBPROC) extgl_GetProcAddress("glGetVertexAttribPointervARB");
    glIsProgramARB = (glIsProgramARBPROC) extgl_GetProcAddress("glIsProgramARB");
#endif
}

void extgl_InitARBWindowPos()
{
#ifdef GL_ARB_window_pos
    if (!extgl_Extensions.ARB_window_pos)
        return;
    glWindowPos2dARB = (glWindowPos2dARBPROC) extgl_GetProcAddress("glWindowPos2dARB");
    glWindowPos2fARB = (glWindowPos2fARBPROC) extgl_GetProcAddress("glWindowPos2fARB");
    glWindowPos2iARB = (glWindowPos2iARBPROC) extgl_GetProcAddress("glWindowPos2iARB");
    glWindowPos2sARB = (glWindowPos2sARBPROC) extgl_GetProcAddress("glWindowPos2sARB");
    glWindowPos2dvARB = (glWindowPos2dvARBPROC) extgl_GetProcAddress("glWindowPos2dvARB");
    glWindowPos2fvARB = (glWindowPos2fvARBPROC) extgl_GetProcAddress("glWindowPos2fvARB");
    glWindowPos2ivARB = (glWindowPos2ivARBPROC) extgl_GetProcAddress("glWindowPos2ivARB");
    glWindowPos2svARB = (glWindowPos2svARBPROC) extgl_GetProcAddress("glWindowPos2svARB");
    glWindowPos3dARB = (glWindowPos3dARBPROC) extgl_GetProcAddress("glWindowPos3dARB");
    glWindowPos3fARB = (glWindowPos3fARBPROC) extgl_GetProcAddress("glWindowPos3fARB");
    glWindowPos3iARB = (glWindowPos3iARBPROC) extgl_GetProcAddress("glWindowPos3iARB");
    glWindowPos3sARB = (glWindowPos3sARBPROC) extgl_GetProcAddress("glWindowPos3sARB");
    glWindowPos3dvARB = (glWindowPos3dvARBPROC) extgl_GetProcAddress("glWindowPos3dvARB");
    glWindowPos3fvARB = (glWindowPos3fvARBPROC) extgl_GetProcAddress("glWindowPos3fvARB");
    glWindowPos3ivARB = (glWindowPos3ivARBPROC) extgl_GetProcAddress("glWindowPos3ivARB");
    glWindowPos3svARB = (glWindowPos3svARBPROC) extgl_GetProcAddress("glWindowPos3svARB");
#endif
}


void extgl_InitATIDrawBuffers()
{
#ifdef GL_ATI_draw_buffers
    if (!extgl_Extensions.ATI_draw_buffers)
        return;
    glDrawBuffersATI = (glDrawBuffersATIPROC) extgl_GetProcAddress("glDrawBuffersATI");
#endif /* GL_ATI_draw_buffers */
}

void extgl_InitATIElementArray()
{
#ifdef GL_ATI_element_array
    if (!extgl_Extensions.ATI_element_array)
        return;
    glElementPointerATI = (glElementPointerATIPROC) extgl_GetProcAddress("glElementPointerATI");
    glDrawElementArrayATI = (glDrawElementArrayATIPROC) extgl_GetProcAddress("glDrawElementArrayATI");
    glDrawRangeElementArrayATI = (glDrawRangeElementArrayATIPROC) extgl_GetProcAddress("glDrawRangeElementArrayATI");
#endif
}

void extgl_InitATIEnvmapBumpmap()
{
#ifdef GL_ATI_envmap_bumpmap
    if (!extgl_Extensions.ATI_envmap_bumpmap)
        return;
    glTexBumpParameterivATI = (glTexBumpParameterivATIPROC) extgl_GetProcAddress("glTexBumpParameterivATI");
    glTexBumpParameterfvATI = (glTexBumpParameterfvATIPROC) extgl_GetProcAddress("glTexBumpParameterfvATI");
    glGetTexBumpParameterivATI = (glGetTexBumpParameterivATIPROC) extgl_GetProcAddress("glGetTexBumpParameterivATI");
    glGetTexBumpParameterfvATI = (glGetTexBumpParameterfvATIPROC) extgl_GetProcAddress("glGetTexBumpParameterfvATI");
#endif
}

void extgl_InitATIFragmentShader()
{
#ifdef GL_ATI_fragment_shader
    if (!extgl_Extensions.ATI_fragment_shader)
        return;
    glGenFragmentShadersATI = (glGenFragmentShadersATIPROC) extgl_GetProcAddress("glGenFragmentShadersATI");
    glBindFragmentShaderATI = (glBindFragmentShaderATIPROC) extgl_GetProcAddress("glBindFragmentShaderATI");
    glDeleteFragmentShaderATI = (glDeleteFragmentShaderATIPROC) extgl_GetProcAddress("glDeleteFragmentShaderATI");
    glBeginFragmentShaderATI = (glBeginFragmentShaderATIPROC) extgl_GetProcAddress("glBeginFragmentShaderATI");
    glEndFragmentShaderATI = (glEndFragmentShaderATIPROC) extgl_GetProcAddress("glEndFragmentShaderATI");
    glPassTexCoordATI = (glPassTexCoordATIPROC) extgl_GetProcAddress("glPassTexCoordATI");
    glSampleMapATI = (glSampleMapATIPROC) extgl_GetProcAddress("glSampleMapATI");
    glColorFragmentOp1ATI = (glColorFragmentOp1ATIPROC) extgl_GetProcAddress("glColorFragmentOp1ATI");
    glColorFragmentOp2ATI = (glColorFragmentOp2ATIPROC) extgl_GetProcAddress("glColorFragmentOp2ATI");
    glColorFragmentOp3ATI = (glColorFragmentOp3ATIPROC) extgl_GetProcAddress("glColorFragmentOp3ATI");
    glAlphaFragmentOp1ATI = (glAlphaFragmentOp1ATIPROC) extgl_GetProcAddress("glAlphaFragmentOp1ATI");
    glAlphaFragmentOp2ATI = (glAlphaFragmentOp2ATIPROC) extgl_GetProcAddress("glAlphaFragmentOp2ATI");
    glAlphaFragmentOp3ATI = (glAlphaFragmentOp3ATIPROC) extgl_GetProcAddress("glAlphaFragmentOp3ATI");
    glSetFragmentShaderConstantATI = (glSetFragmentShaderConstantATIPROC) extgl_GetProcAddress("glSetFragmentShaderConstantATI");
#endif
}

void extgl_InitATIMapObjectBuffer()
{
#ifdef GL_ATI_map_object_buffer
    if (!extgl_Extensions.ATI_map_object_buffer)
        return;
    glMapObjectBufferATI = (glMapObjectBufferATIPROC) extgl_GetProcAddress("glMapObjectBufferATI");
    glUnmapObjectBufferATI = (glUnmapObjectBufferATIPROC) extgl_GetProcAddress("glUnmapObjectBufferATI");
#endif /* GL_ATI_map_object_buffer */
}

void extgl_InitATIPNTriangles()
{
#ifdef GL_ATI_pn_triangles
    if (!extgl_Extensions.ATI_pn_triangles)
        return;
    glPNTrianglesiATI = (glPNTrianglesiATIPROC) extgl_GetProcAddress("glPNTrianglesiATI");
    glPNTrianglesfATI = (glPNTrianglesfATIPROC) extgl_GetProcAddress("glPNTrianglesfATI");
#endif
}

void extgl_InitATISeparateStencil()
{
#ifdef GL_ATI_separate_stencil
    if (!extgl_Extensions.ATI_separate_stencil)
        return;
    glStencilOpSeparateATI = (glStencilOpSeparateATIPROC) extgl_GetProcAddress("glStencilOpSeparateATI");
    glStencilFuncSeparateATI = (glStencilFuncSeparateATIPROC) extgl_GetProcAddress("glStencilFuncSeparateATI");
#endif /* GL_ATI_separate_stencil */
}

void extgl_InitATIVertexAttribArrayObject()
{
#ifdef GL_ATI_vertex_attrib_array_object
	if (!extgl_Extensions.ATI_vertex_attrib_array_object)
		return;
	glVertexAttribArrayObjectATI = (glVertexAttribArrayObjectATIPROC) extgl_GetProcAddress("glVertexAttribArrayObjectATI");
	glGetVertexAttribArrayObjectfvATI = (glGetVertexAttribArrayObjectfvATIPROC) extgl_GetProcAddress("glGetVertexAttribArrayObjectfvATI");
	glGetVertexAttribArrayObjectivATI = (glGetVertexAttribArrayObjectivATIPROC) extgl_GetProcAddress("glGetVertexAttribArrayObjectivATI");
#endif /* GL_ATI_vertex_attrib_array_object */
}

void extgl_InitATIVertexArrayObject()
{
#ifdef GL_ATI_vertex_array_object
    if (!extgl_Extensions.ATI_vertex_array_object)
        return;
    glNewObjectBufferATI = (glNewObjectBufferATIPROC) extgl_GetProcAddress("glNewObjectBufferATI");
    glIsObjectBufferATI = (glIsObjectBufferATIPROC) extgl_GetProcAddress("glIsObjectBufferATI");
    glUpdateObjectBufferATI = (glUpdateObjectBufferATIPROC) extgl_GetProcAddress("glUpdateObjectBufferATI");
    glGetObjectBufferfvATI = (glGetObjectBufferfvATIPROC) extgl_GetProcAddress("glGetObjectBufferfvATI");
    glGetObjectBufferivATI = (glGetObjectBufferivATIPROC) extgl_GetProcAddress("glGetObjectBufferivATI");
    glFreeObjectBufferATI = (glFreeObjectBufferATIPROC) extgl_GetProcAddress("glFreeObjectBufferATI");
    glArrayObjectATI = (glArrayObjectATIPROC) extgl_GetProcAddress("glArrayObjectATI");
    glGetArrayObjectfvATI = (glGetArrayObjectfvATIPROC) extgl_GetProcAddress("glGetArrayObjectfvATI");
    glGetArrayObjectivATI = (glGetArrayObjectivATIPROC) extgl_GetProcAddress("glGetArrayObjectivATI");
    glVariantArrayObjectATI = (glVariantArrayObjectATIPROC) extgl_GetProcAddress("glVariantArrayObjectATI");
    glGetVariantArrayObjectfvATI = (glGetVariantArrayObjectfvATIPROC) extgl_GetProcAddress("glGetVariantArrayObjectfvATI");
    glGetVariantArrayObjectivATI = (glGetVariantArrayObjectivATIPROC) extgl_GetProcAddress("glGetVariantArrayObjectivATI");
#endif
}

void extgl_InitATIVertexStreams()
{
#ifdef GL_ATI_vertex_streams
    if (!extgl_Extensions.ATI_vertex_streams)
        return;
    glClientActiveVertexStreamATI = (glClientActiveVertexStreamATIPROC) extgl_GetProcAddress("glClientActiveVertexStreamATI");
    glVertexBlendEnviATI = (glVertexBlendEnviATIPROC) extgl_GetProcAddress("glVertexBlendEnviATI");
    glVertexBlendEnvfATI = (glVertexBlendEnvfATIPROC) extgl_GetProcAddress("glVertexBlendEnvfATI");
	glVertexStream1sATI = (glVertexStream1sATIPROC) extgl_GetProcAddress("glVertexStream1sATI");
	glVertexStream1svATI = (glVertexStream1svATIPROC) extgl_GetProcAddress("glVertexStream1svATI");
	glVertexStream1iATI = (glVertexStream1iATIPROC) extgl_GetProcAddress("glVertexStream1iATI");
	glVertexStream1ivATI = (glVertexStream1ivATIPROC) extgl_GetProcAddress("glVertexStream1ivATI");
	glVertexStream1fATI = (glVertexStream1fATIPROC) extgl_GetProcAddress("glVertexStream1fATI");
	glVertexStream1fvATI = (glVertexStream1fvATIPROC) extgl_GetProcAddress("glVertexStream1fvATI");
	glVertexStream1dATI = (glVertexStream1dATIPROC) extgl_GetProcAddress("glVertexStream1dATI");
	glVertexStream1dvATI = (glVertexStream1dvATIPROC) extgl_GetProcAddress("glVertexStream1dvATI");
    glVertexStream2sATI = (glVertexStream2sATIPROC) extgl_GetProcAddress("glVertexStream2sATI");
    glVertexStream2svATI = (glVertexStream2svATIPROC) extgl_GetProcAddress("glVertexStream2svATI");
    glVertexStream2iATI = (glVertexStream2iATIPROC) extgl_GetProcAddress("glVertexStream2iATI");
    glVertexStream2ivATI = (glVertexStream2ivATIPROC) extgl_GetProcAddress("glVertexStream2ivATI");
    glVertexStream2fATI = (glVertexStream2fATIPROC) extgl_GetProcAddress("glVertexStream2fATI");
    glVertexStream2fvATI = (glVertexStream2fvATIPROC) extgl_GetProcAddress("glVertexStream2fvATI");
    glVertexStream2dATI = (glVertexStream2dATIPROC) extgl_GetProcAddress("glVertexStream2dATI");
    glVertexStream2dvATI = (glVertexStream2dvATIPROC) extgl_GetProcAddress("glVertexStream2dvATI");
    glVertexStream3sATI = (glVertexStream3sATIPROC) extgl_GetProcAddress("glVertexStream3sATI");
    glVertexStream3svATI = (glVertexStream3svATIPROC) extgl_GetProcAddress("glVertexStream3svATI");
    glVertexStream3iATI = (glVertexStream3iATIPROC) extgl_GetProcAddress("glVertexStream3iATI");
    glVertexStream3ivATI = (glVertexStream3ivATIPROC) extgl_GetProcAddress("glVertexStream3ivATI");
    glVertexStream3fATI = (glVertexStream3fATIPROC) extgl_GetProcAddress("glVertexStream3fATI");
    glVertexStream3fvATI = (glVertexStream3fvATIPROC) extgl_GetProcAddress("glVertexStream3fvATI");
    glVertexStream3dATI = (glVertexStream3dATIPROC) extgl_GetProcAddress("glVertexStream3dATI");
    glVertexStream3dvATI = (glVertexStream3dvATIPROC) extgl_GetProcAddress("glVertexStream3dvATI");
    glVertexStream4sATI = (glVertexStream4sATIPROC) extgl_GetProcAddress("glVertexStream4sATI");
    glVertexStream4svATI = (glVertexStream4svATIPROC) extgl_GetProcAddress("glVertexStream4svATI");
    glVertexStream4iATI = (glVertexStream4iATIPROC) extgl_GetProcAddress("glVertexStream4iATI");
    glVertexStream4ivATI = (glVertexStream4ivATIPROC) extgl_GetProcAddress("glVertexStream4ivATI");
    glVertexStream4fATI = (glVertexStream4fATIPROC) extgl_GetProcAddress("glVertexStream4fATI");
    glVertexStream4fvATI = (glVertexStream4fvATIPROC) extgl_GetProcAddress("glVertexStream4fvATI");
    glVertexStream4dATI = (glVertexStream4dATIPROC) extgl_GetProcAddress("glVertexStream4dATI");
    glVertexStream4dvATI = (glVertexStream4dvATIPROC) extgl_GetProcAddress("glVertexStream4dvATI");
    glNormalStream3bATI = (glNormalStream3bATIPROC) extgl_GetProcAddress("glNormalStream3bATI");
    glNormalStream3bvATI = (glNormalStream3bvATIPROC) extgl_GetProcAddress("glNormalStream3bvATI");
    glNormalStream3sATI = (glNormalStream3sATIPROC) extgl_GetProcAddress("glNormalStream3sATI");
    glNormalStream3svATI = (glNormalStream3svATIPROC) extgl_GetProcAddress("glNormalStream3svATI");
    glNormalStream3iATI = (glNormalStream3iATIPROC) extgl_GetProcAddress("glNormalStream3iATI");
    glNormalStream3ivATI = (glNormalStream3ivATIPROC) extgl_GetProcAddress("glNormalStream3ivATI");
    glNormalStream3fATI = (glNormalStream3fATIPROC) extgl_GetProcAddress("glNormalStream3fATI");
    glNormalStream3fvATI = (glNormalStream3fvATIPROC) extgl_GetProcAddress("glNormalStream3fvATI");
    glNormalStream3dATI = (glNormalStream3dATIPROC) extgl_GetProcAddress("glNormalStream3dATI");
    glNormalStream3dvATI = (glNormalStream3dvATIPROC) extgl_GetProcAddress("glNormalStream3dvATI");
#endif
}

void extgl_InitARBDrawBuffers()
{
#ifdef GL_ARB_draw_buffers
	if (!extgl_Extensions.ARB_draw_buffers)
		return;
	glDrawBuffersARB = (glDrawBuffersARBPROC) extgl_GetProcAddress("glDrawBuffersARB");
#endif /* GL_ARB_draw_buffers */
}

void extgl_InitARBColorBufferFloat()
{
#ifdef GL_ARB_color_buffer_float
	if (!extgl_Extensions.ARB_color_buffer_float)
		return;
	glClampColorARB = (glClampColorARBPROC) extgl_GetProcAddress("glClampColorARB");
#endif /* GL_ARB_color_buffer_float */
}

void extgl_InitEXTBlendColor()
{
#ifdef GL_EXT_blend_color
    if (!extgl_Extensions.EXT_blend_color)
        return;
    glBlendColorEXT = (glBlendColorEXTPROC) extgl_GetProcAddress("glBlendColorEXT");
#endif /* GL_EXT_blend_color */
}

void extgl_InitEXTBlendFuncSeparate()
{
#ifdef GL_EXT_blend_func_separate
    if (!extgl_Extensions.EXT_blend_func_separate)
        return;
    glBlendFuncSeparateEXT = (glBlendFuncSeparateEXTPROC) extgl_GetProcAddress("glBlendFuncSeparateEXT");
#endif
}

void extgl_InitEXTBlendMinmax()
{
#ifdef GL_EXT_blend_minmax
    if (!extgl_Extensions.EXT_blend_minmax)
        return;
    glBlendEquationEXT = (glBlendEquationEXTPROC) extgl_GetProcAddress("glBlendEquationEXT");
#endif /* GL_EXT_blend_minmax */
}

void extgl_InitEXTColorSubtable()
{
#ifdef GL_EXT_color_subtable
    if (!extgl_Extensions.EXT_color_subtable)
        return;
    glColorSubTableEXT = (glColorSubTableEXTPROC) extgl_GetProcAddress("glColorSubTableEXT");
    glCopyColorSubTableEXT = (glCopyColorSubTableEXTPROC) extgl_GetProcAddress("glCopyColorSubTableEXT");
#endif
}

void extgl_InitEXTCompiledVertexArray()
{
#ifdef GL_EXT_compiled_vertex_array
    if (!extgl_Extensions.EXT_compiled_vertex_array)
        return;
    glLockArraysEXT = (glLockArraysEXTPROC) extgl_GetProcAddress("glLockArraysEXT");
    glUnlockArraysEXT = (glUnlockArraysEXTPROC) extgl_GetProcAddress("glUnlockArraysEXT");
#endif
}

void extgl_InitEXTConvolution()
{
#ifdef GL_EXT_convolution
    if (!extgl_Extensions.EXT_convolution)
        return;
	glConvolutionFilter1DEXT = (glConvolutionFilter1DEXTPROC) extgl_GetProcAddress("glConvolutionFilter1DEXT");
	glConvolutionFilter2DEXT = (glConvolutionFilter2DEXTPROC) extgl_GetProcAddress("glConvolutionFilter2DEXT");
	glConvolutionParameterfEXT = (glConvolutionParameterfEXTPROC) extgl_GetProcAddress("glConvolutionParameterfEXT");
	glConvolutionParameterfvEXT = (glConvolutionParameterfvEXTPROC) extgl_GetProcAddress("glConvolutionParameterfvEXT");
	glConvolutionParameteriEXT = (glConvolutionParameteriEXTPROC) extgl_GetProcAddress("glConvolutionParameteriEXT");
	glConvolutionParameterivEXT = (glConvolutionParameterivEXTPROC) extgl_GetProcAddress("glConvolutionParameterivEXT");
	glCopyConvolutionFilter1DEXT = (glCopyConvolutionFilter1DEXTPROC) extgl_GetProcAddress("glCopyConvolutionFilter1DEXT");
	glCopyConvolutionFilter2DEXT = (glCopyConvolutionFilter2DEXTPROC) extgl_GetProcAddress("glCopyConvolutionFilter2DEXT");
	glGetConvolutionFilterEXT = (glGetConvolutionFilterEXTPROC) extgl_GetProcAddress("glGetConvolutionFilterEXT");
	glGetConvolutionParameterfvEXT = (glGetConvolutionParameterfvEXTPROC) extgl_GetProcAddress("glGetConvolutionParameterfvEXT");
	glGetConvolutionParameterivEXT = (glGetConvolutionParameterivEXTPROC) extgl_GetProcAddress("glGetConvolutionParameterivEXT");
	glGetSeparableFilterEXT = (glGetSeparableFilterEXTPROC) extgl_GetProcAddress("glGetSeparableFilterEXT");
	glSeparableFilter2DEXT = (glSeparableFilter2DEXTPROC) extgl_GetProcAddress("glSeparableFilter2DEXT");
#endif
}

void extgl_InitEXTCoordinateFrame()
{
#ifdef GL_EXT_coordinate_frame
    if (!extgl_Extensions.EXT_coordinate_frame)
		return;
	glTangent3bEXT = (glTangent3bEXTPROC) extgl_GetProcAddress("glTangent3bEXT");
	glTangent3bvEXT = (glTangent3bvEXTPROC) extgl_GetProcAddress("glTangent3bvEXT");
	glTangent3dEXT = (glTangent3dEXTPROC) extgl_GetProcAddress("glTangent3dEXT");
	glTangent3dvEXT = (glTangent3dvEXTPROC) extgl_GetProcAddress("glTangent3dvEXT");
	glTangent3fEXT = (glTangent3fEXTPROC) extgl_GetProcAddress("glTangent3fEXT");
	glTangent3fvEXT = (glTangent3fvEXTPROC) extgl_GetProcAddress("glTangent3fvEXT");
	glTangent3iEXT = (glTangent3iEXTPROC) extgl_GetProcAddress("glTangent3iEXT");
	glTangent3ivEXT = (glTangent3ivEXTPROC) extgl_GetProcAddress("glTangent3ivEXT");
	glTangent3sEXT = (glTangent3sEXTPROC) extgl_GetProcAddress("glTangent3sEXT");
	glTangent3svEXT = (glTangent3svEXTPROC) extgl_GetProcAddress("glTangent3svEXT");
	glBinormal3bEXT = (glBinormal3bEXTPROC) extgl_GetProcAddress("glBinormal3bEXT");
	glBinormal3bvEXT = (glBinormal3bvEXTPROC) extgl_GetProcAddress("glBinormal3bvEXT");
	glBinormal3dEXT = (glBinormal3dEXTPROC) extgl_GetProcAddress("glBinormal3dEXT");
	glBinormal3dvEXT = (glBinormal3dvEXTPROC) extgl_GetProcAddress("glBinormal3dvEXT");
	glBinormal3fEXT = (glBinormal3fEXTPROC) extgl_GetProcAddress("glBinormal3fEXT");
	glBinormal3fvEXT = (glBinormal3fvEXTPROC) extgl_GetProcAddress("glBinormal3fvEXT");
	glBinormal3iEXT = (glBinormal3iEXTPROC) extgl_GetProcAddress("glBinormal3iEXT");
	glBinormal3ivEXT = (glBinormal3ivEXTPROC) extgl_GetProcAddress("glBinormal3ivEXT");
	glBinormal3sEXT = (glBinormal3sEXTPROC) extgl_GetProcAddress("glBinormal3sEXT");
	glBinormal3svEXT = (glBinormal3svEXTPROC) extgl_GetProcAddress("glBinormal3svEXT");
	glTangentPointerEXT = (glTangentPointerEXTPROC) extgl_GetProcAddress("glTangentPointerEXT");
	glBinormalPointerEXT = (glBinormalPointerEXTPROC) extgl_GetProcAddress("glBinormalPointerEXT");
#endif
}

void extgl_InitEXTCopyTexture()
{
#ifdef GL_EXT_copy_texture
    if (!extgl_Extensions.EXT_copy_texture)
        return;
    glCopyTexImage1DEXT = (glCopyTexImage1DEXTPROC) extgl_GetProcAddress("glCopyTexImage1DEXT");
    glCopyTexImage2DEXT = (glCopyTexImage2DEXTPROC) extgl_GetProcAddress("glCopyTexImage2DEXT");
    glCopyTexSubImage1DEXT = (glCopyTexSubImage1DEXTPROC) extgl_GetProcAddress("glCopyTexSubImage1DEXT");
    glCopyTexSubImage2DEXT = (glCopyTexSubImage2DEXTPROC) extgl_GetProcAddress("glCopyTexSubImage2DEXT");
    glCopyTexSubImage3DEXT = (glCopyTexSubImage3DEXTPROC) extgl_GetProcAddress("glCopyTexSubImage3DEXT");
#endif
}

void extgl_InitEXTCullVertex()
{
#ifdef GL_EXT_cull_vertex
    if (!extgl_Extensions.EXT_cull_vertex)
        return;
    glCullParameterfvEXT = (glCullParameterfvEXTPROC) extgl_GetProcAddress("glCullParameterfvEXT");
    glCullParameterdvEXT = (glCullParameterdvEXTPROC) extgl_GetProcAddress("glCullParameterdvEXT");
#endif
}

void extgl_InitEXTDepthBoundsTest()
{
#ifdef GL_EXT_depth_bounds_test
    if (!extgl_Extensions.EXT_depth_bounds_test)
        return;
    glDepthBoundsEXT = (glDepthBoundsEXTPROC) extgl_GetProcAddress("glDepthBoundsEXT");
#endif /* GL_EXT_depth_bounds_test */
}

void extgl_InitEXTBlendEquationSeparate()
{
#ifdef GL_EXT_blend_equation_separate
	if (!extgl_Extensions.EXT_blend_equation_separate)
		return;
	glBlendEquationSeparateEXT = (glBlendEquationSeparateEXTPROC) extgl_GetProcAddress("glBlendEquationSeparateEXT");
#endif /* GL_EXT_blend_equation_separate */
}

void extgl_InitEXTDrawRangeElements()
{
#ifdef GL_EXT_draw_range_elements
    if (!extgl_Extensions.EXT_draw_range_elements)
        return;
    glDrawRangeElementsEXT = (glDrawRangeElementsEXTPROC) extgl_GetProcAddress("glDrawRangeElementsEXT");
#endif
}

void extgl_InitEXTFogCoord()
{
#ifdef GL_EXT_fog_coord
    if (!extgl_Extensions.EXT_fog_coord)
        return;
    glFogCoordfEXT = (glFogCoordfEXTPROC) extgl_GetProcAddress("glFogCoordfEXT");
    glFogCoordfvEXT = (glFogCoordfvEXTPROC) extgl_GetProcAddress("glFogCoordfvEXT");
    glFogCoorddEXT = (glFogCoorddEXTPROC) extgl_GetProcAddress("glFogCoorddEXT");
    glFogCoorddvEXT = (glFogCoorddvEXTPROC) extgl_GetProcAddress("glFogCoorddvEXT");
    glFogCoordPointerEXT = (glFogCoordPointerEXTPROC) extgl_GetProcAddress("glFogCoordPointerEXT");
#endif
}

void extgl_InitEXTFramebufferObject()
{
#ifdef GL_EXT_framebuffer_object
	if (!extgl_Extensions.EXT_framebuffer_object)
		return;
	glIsRenderbufferEXT = (glIsRenderbufferEXTPROC) extgl_GetProcAddress("glIsRenderbufferEXT");
	glBindRenderbufferEXT = (glBindRenderbufferEXTPROC) extgl_GetProcAddress("glBindRenderbufferEXT");
	glDeleteRenderbuffersEXT = (glDeleteRenderbuffersEXTPROC) extgl_GetProcAddress("glDeleteRenderbuffersEXT");
	glGenRenderbuffersEXT = (glGenRenderbuffersEXTPROC) extgl_GetProcAddress("glGenRenderbuffersEXT");
	glRenderbufferStorageEXT = (glRenderbufferStorageEXTPROC) extgl_GetProcAddress("glRenderbufferStorageEXT");
	glGetRenderbufferParameterivEXT = (glGetRenderbufferParameterivEXTPROC) extgl_GetProcAddress("glGetRenderbufferParameterivEXT");
	glIsFramebufferEXT = (glIsFramebufferEXTPROC) extgl_GetProcAddress("glIsFramebufferEXT");
	glBindFramebufferEXT = (glBindFramebufferEXTPROC) extgl_GetProcAddress("glBindFramebufferEXT");
	glDeleteFramebuffersEXT = (glDeleteFramebuffersEXTPROC) extgl_GetProcAddress("glDeleteFramebuffersEXT");
	glGenFramebuffersEXT = (glGenFramebuffersEXTPROC) extgl_GetProcAddress("glGenFramebuffersEXT");
	glCheckFramebufferStatusEXT = (glCheckFramebufferStatusEXTPROC) extgl_GetProcAddress("glCheckFramebufferStatusEXT");
	glFramebufferTexture1DEXT = (glFramebufferTexture1DEXTPROC) extgl_GetProcAddress("glFramebufferTexture1DEXT");
	glFramebufferTexture2DEXT = (glFramebufferTexture2DEXTPROC) extgl_GetProcAddress("glFramebufferTexture2DEXT");
	glFramebufferTexture3DEXT = (glFramebufferTexture3DEXTPROC) extgl_GetProcAddress("glFramebufferTexture3DEXT");
	glFramebufferRenderbufferEXT = (glFramebufferRenderbufferEXTPROC) extgl_GetProcAddress("glFramebufferRenderbufferEXT");
	glGetFramebufferAttachmentParameterivEXT = (glGetFramebufferAttachmentParameterivEXTPROC) extgl_GetProcAddress("glGetFramebufferAttachmentParameterivEXT");
	glGenerateMipmapEXT = (glGenerateMipmapEXTPROC) extgl_GetProcAddress("glGenerateMipmapEXT");
#endif
}

void extgl_InitEXTHistogram()
{
#ifdef GL_EXT_histogram
    if (!extgl_Extensions.EXT_histogram)
        return;
    glGetHistogramEXT = (glGetHistogramEXTPROC) extgl_GetProcAddress("glGetHistogramEXT");
    glGetHistogramParameterfvEXT = (glGetHistogramParameterfvEXTPROC) extgl_GetProcAddress("glGetHistogramParameterfvEXT");
    glGetHistogramParameterivEXT = (glGetHistogramParameterivEXTPROC) extgl_GetProcAddress("glGetHistogramParameterivEXT");
    glGetMinMaxEXT = (glGetMinMaxEXTPROC) extgl_GetProcAddress("glGetMinMaxEXT");
    glGetMinMaxParameterfvEXT = (glGetMinMaxParameterfvEXTPROC) extgl_GetProcAddress("glGetMinMaxParameterfvEXT");
    glGetMinMaxParameterivEXT = (glGetMinMaxParameterivEXTPROC) extgl_GetProcAddress("glGetMinMaxParameterivEXT");
    glHistogramEXT = (glHistogramEXTPROC) extgl_GetProcAddress("glHistogramEXT");
    glResetMinMaxEXT = (glResetMinMaxEXTPROC) extgl_GetProcAddress("glResetMinMaxEXT");
#endif
}

void extgl_InitEXTIndexFunc()
{
#ifdef GL_EXT_index_func
    if (!extgl_Extensions.EXT_index_func)
        return;
	glIndexFuncEXT = (glIndexFuncEXTPROC) extgl_GetProcAddress("glIndexFuncEXT");
#endif /* GL_EXT_index_func */
}

void extgl_InitEXTIndexMaterial()
{
#ifdef GL_EXT_index_material
    if (!extgl_Extensions.EXT_index_material)
        return;
	glIndexMaterialEXT = (glIndexMaterialEXTPROC) extgl_GetProcAddress("glIndexMaterialEXT");
#endif /* GL_EXT_index_material */
}

void extgl_InitEXTLightTexture()
{
#ifdef GL_EXT_light_texture
    if (!extgl_Extensions.EXT_light_texture)
        return;
	glApplyTextureEXT = (glApplyTextureEXTPROC) extgl_GetProcAddress("glApplyTextureEXT");
	glTextureLightEXT = (glTextureLightEXTPROC) extgl_GetProcAddress("glTextureLightEXT");
	glTextureMaterialEXT = (glTextureMaterialEXTPROC) extgl_GetProcAddress("glTextureMaterialEXT");
#endif /* GL_EXT_index_material */
}

void extgl_InitEXTMultiDrawArrays()
{
#ifdef GL_EXT_multi_draw_arrays
    if (!extgl_Extensions.EXT_multi_draw_arrays)
        return;
    glMultiDrawArraysEXT = (glMultiDrawArraysEXTPROC) extgl_GetProcAddress("glMultiDrawArraysEXT");
    glMultiDrawElementsEXT = (glMultiDrawElementsEXTPROC) extgl_GetProcAddress("glMultiDrawElementsEXT");
#endif
}

void extgl_InitEXTMultisample()
{
#ifdef GL_EXT_multisample
    if (!extgl_Extensions.EXT_multisample)
        return;
    glSampleMaskEXT = (glSampleMaskEXTPROC) extgl_GetProcAddress("glSampleMaskEXT");
    glSamplePatternEXT = (glSamplePatternEXTPROC) extgl_GetProcAddress("glSamplePatternEXT");
#endif
}

void extgl_InitEXTPaletteTexture()
{
#ifdef GL_EXT_paletted_texture
    if (!extgl_Extensions.EXT_paletted_texture)
        return;
	glColorTableEXT = (glColorTableEXTPROC) extgl_GetProcAddress("glColorTableEXT");
	glGetColorTableEXT = (glGetColorTableEXTPROC) extgl_GetProcAddress("glGetColorTableEXT");
	glGetColorTableParameterivEXT = (glGetColorTableParameterivEXTPROC) extgl_GetProcAddress("glGetColorTableParameterivEXT");
	glGetColorTableParameterfvEXT = (glGetColorTableParameterfvEXTPROC) extgl_GetProcAddress("glGetColorTableParameterfvEXT");
#endif /* GL_EXT_paletted_texture */
}

void extgl_InitEXTPixelTransform()
{
#ifdef GL_EXT_pixel_transform
    if (!extgl_Extensions.EXT_pixel_transform)
        return;
	glPixelTransformParameteriEXT = (glPixelTransformParameteriEXTPROC) extgl_GetProcAddress("glPixelTransformParameteriEXT");
	glPixelTransformParameterfEXT = (glPixelTransformParameterfEXTPROC) extgl_GetProcAddress("glPixelTransformParameterfEXT");
	glPixelTransformParameterivEXT = (glPixelTransformParameterivEXTPROC) extgl_GetProcAddress("glPixelTransformParameterivEXT");
	glPixelTransformParameterfvEXT = (glPixelTransformParameterfvEXTPROC) extgl_GetProcAddress("glPixelTransformParameterfvEXT");
#endif /* GL_EXT_pixel_transform */
}

void extgl_InitEXTPointParameters()
{
#ifdef GL_EXT_point_parameters
    if (!extgl_Extensions.EXT_point_parameters)
        return;
    glPointParameterfEXT = (glPointParameterfEXTPROC) extgl_GetProcAddress("glPointParameterfEXT");
    glPointParameterfvEXT = (glPointParameterfvEXTPROC) extgl_GetProcAddress("glPointParameterfvEXT");
#endif
}

void extgl_InitEXTPolygonOffset()
{
#ifdef GL_EXT_polygon_offset
    if (!extgl_Extensions.EXT_polygon_offset)
        return;
    glPolygonOffsetEXT = (glPolygonOffsetEXTPROC) extgl_GetProcAddress("glPolygonOffsetEXT");
#endif /* GL_EXT_polygon_offset */
}

void extgl_InitEXTSecondaryColor()
{
#ifdef GL_EXT_secondary_color
    if (!extgl_Extensions.EXT_secondary_color)
        return;
    glSecondaryColor3bEXT = (glSecondaryColor3bEXTPROC) extgl_GetProcAddress("glSecondaryColor3bEXT");
    glSecondaryColor3bvEXT = (glSecondaryColor3bvEXTPROC) extgl_GetProcAddress("glSecondaryColor3bvEXT");
    glSecondaryColor3dEXT = (glSecondaryColor3dEXTPROC) extgl_GetProcAddress("glSecondaryColor3dEXT");
    glSecondaryColor3dvEXT = (glSecondaryColor3dvEXTPROC) extgl_GetProcAddress("glSecondaryColor3dvEXT");
    glSecondaryColor3fEXT = (glSecondaryColor3fEXTPROC) extgl_GetProcAddress("glSecondaryColor3fEXT");
    glSecondaryColor3fvEXT = (glSecondaryColor3fvEXTPROC) extgl_GetProcAddress("glSecondaryColor3fvEXT");
    glSecondaryColor3iEXT = (glSecondaryColor3iEXTPROC) extgl_GetProcAddress("glSecondaryColor3iEXT");
    glSecondaryColor3ivEXT = (glSecondaryColor3ivEXTPROC) extgl_GetProcAddress("glSecondaryColor3ivEXT");
    glSecondaryColor3sEXT = (glSecondaryColor3sEXTPROC) extgl_GetProcAddress("glSecondaryColor3sEXT");
    glSecondaryColor3svEXT = (glSecondaryColor3svEXTPROC) extgl_GetProcAddress("glSecondaryColor3svEXT");
    glSecondaryColor3ubEXT = (glSecondaryColor3ubEXTPROC) extgl_GetProcAddress("glSecondaryColor3ubEXT");
    glSecondaryColor3ubvEXT = (glSecondaryColor3ubvEXTPROC) extgl_GetProcAddress("glSecondaryColor3ubvEXT");
    glSecondaryColor3uiEXT = (glSecondaryColor3uiEXTPROC) extgl_GetProcAddress("glSecondaryColor3uiEXT");
    glSecondaryColor3uivEXT = (glSecondaryColor3uivEXTPROC) extgl_GetProcAddress("glSecondaryColor3uivEXT");
    glSecondaryColor3usEXT = (glSecondaryColor3usEXTPROC) extgl_GetProcAddress("glSecondaryColor3usEXT");
    glSecondaryColor3usvEXT = (glSecondaryColor3usvEXTPROC) extgl_GetProcAddress("glSecondaryColor3usvEXT");
    glSecondaryColorPointerEXT = (glSecondaryColorPointerEXTPROC) extgl_GetProcAddress("glSecondaryColorPointerEXT");
#endif
}

void extgl_InitEXTStencilTwoSide()
{
#ifdef GL_EXT_stencil_two_side
    if (!extgl_Extensions.EXT_stencil_two_side)
        return;
    glActiveStencilFaceEXT = (glActiveStencilFaceEXTPROC) extgl_GetProcAddress("glActiveStencilFaceEXT");
#endif
}

void extgl_InitEXTSubtexture()
{
#ifdef GL_EXT_subtexture
    if (!extgl_Extensions.EXT_subtexture)
        return;
	glTexSubImage1DEXT = (glTexSubImage1DEXTPROC) extgl_GetProcAddress("glTexSubImage1DEXT");
	glTexSubImage2DEXT = (glTexSubImage2DEXTPROC) extgl_GetProcAddress("glTexSubImage2DEXT");
#endif /* GL_EXT_subtexture */
}

void extgl_InitEXTTexture3D()
{
#ifdef GL_EXT_texture3D
    if (!extgl_Extensions.EXT_texture3D)
        return;
	glTexImage3DEXT = (glTexImage3DEXTPROC) extgl_GetProcAddress("glTexImage3DEXT");
	glTexSubImage3DEXT = (glTexSubImage3DEXTPROC) extgl_GetProcAddress("glTexSubImage3DEXT");
#endif /* GL_EXT_texture3D */
}

void extgl_InitEXTTextureObject()
{
#ifdef GL_EXT_texture_object
    if (!extgl_Extensions.EXT_texture_object)
        return;
	glAreTexturesResidentEXT = (glAreTexturesResidentEXTPROC) extgl_GetProcAddress("glAreTexturesResidentEXT");
	glBindTextureEXT = (glBindTextureEXTPROC) extgl_GetProcAddress("glBindTextureEXT");
	glDeleteTexturesEXT = (glDeleteTexturesEXTPROC) extgl_GetProcAddress("glDeleteTexturesEXT");
	glGenTexturesEXT = (glGenTexturesEXTPROC) extgl_GetProcAddress("glGenTexturesEXT");
	glIsTextureEXT = (glIsTextureEXTPROC) extgl_GetProcAddress("glIsTextureEXT");
	glPrioritizeTexturesEXT = (glPrioritizeTexturesEXTPROC) extgl_GetProcAddress("glPrioritizeTexturesEXT");
#endif
}

void extgl_InitEXTTexturePerturbNormal()
{
#ifdef GL_EXT_texture_perturb_normal
    if (!extgl_Extensions.EXT_texture_perturb_normal)
        return;
	glTextureNormalEXT = (glTextureNormalEXTPROC) extgl_GetProcAddress("glTextureNormalEXT");
#endif
}

void extgl_InitEXTVertexArray()
{
#ifdef GL_EXT_vertex_array
    if (!extgl_Extensions.EXT_vertex_array)
        return;
	glArrayElementEXT = (glArrayElementEXTPROC) extgl_GetProcAddress("glArrayElementEXT");
	glColorPointerEXT = (glColorPointerEXTPROC) extgl_GetProcAddress("glColorPointerEXT");
	glDrawArraysEXT = (glDrawArraysEXTPROC) extgl_GetProcAddress("glDrawArraysEXT");
	glEdgeFlagPointerEXT = (glEdgeFlagPointerEXTPROC) extgl_GetProcAddress("glEdgeFlagPointerEXT");
	glGetPointervEXT = (glGetPointervEXTPROC) extgl_GetProcAddress("glGetPointervEXT");
	glIndexPointerEXT = (glIndexPointerEXTPROC) extgl_GetProcAddress("glIndexPointerEXT");
	glNormalPointerEXT = (glNormalPointerEXTPROC) extgl_GetProcAddress("glNormalPointerEXT");
	glTexCoordPointerEXT = (glTexCoordPointerEXTPROC) extgl_GetProcAddress("glTexCoordPointerEXT");
	glVertexPointerEXT = (glVertexPointerEXTPROC) extgl_GetProcAddress("glVertexPointerEXT");
#endif /* GL_EXT_vertex_array */
}

void extgl_InitEXTVertexShader()
{
#ifdef GL_EXT_vertex_shader
    if (!extgl_Extensions.EXT_vertex_shader)
        return;
    glBeginVertexShaderEXT = (glBeginVertexShaderEXTPROC) extgl_GetProcAddress("glBeginVertexShaderEXT");
    glEndVertexShaderEXT = (glEndVertexShaderEXTPROC) extgl_GetProcAddress("glEndVertexShaderEXT");
    glBindVertexShaderEXT = (glBindVertexShaderEXTPROC) extgl_GetProcAddress("glBindVertexShaderEXT");
    glGenVertexShadersEXT = (glGenVertexShadersEXTPROC) extgl_GetProcAddress("glGenVertexShadersEXT");
    glDeleteVertexShaderEXT = (glDeleteVertexShaderEXTPROC) extgl_GetProcAddress("glDeleteVertexShaderEXT");
    glShaderOp1EXT = (glShaderOp1EXTPROC) extgl_GetProcAddress("glShaderOp1EXT");
    glShaderOp2EXT = (glShaderOp2EXTPROC) extgl_GetProcAddress("glShaderOp2EXT");
    glShaderOp3EXT = (glShaderOp3EXTPROC) extgl_GetProcAddress("glShaderOp3EXT");
    glSwizzleEXT = (glSwizzleEXTPROC) extgl_GetProcAddress("glSwizzleEXT");
    glWriteMaskEXT = (glWriteMaskEXTPROC) extgl_GetProcAddress("glWriteMaskEXT");
    glInsertComponentEXT = (glInsertComponentEXTPROC) extgl_GetProcAddress("glInsertComponentEXT");
    glExtractComponentEXT = (glExtractComponentEXTPROC) extgl_GetProcAddress("glExtractComponentEXT");
    glGenSymbolsEXT = (glGenSymbolsEXTPROC) extgl_GetProcAddress("glGenSymbolsEXT");
    glSetInvariantEXT = (glSetInvariantEXTPROC) extgl_GetProcAddress("glSetInvarianceEXT");
    glSetLocalConstantEXT = (glSetLocalConstantEXTPROC) extgl_GetProcAddress("glSetLocalConstantEXT");
    glVariantbvEXT = (glVariantbvEXTPROC) extgl_GetProcAddress("glVariantbvEXT");
    glVariantsvEXT = (glVariantsvEXTPROC) extgl_GetProcAddress("glVariantsvEXT");
    glVariantivEXT = (glVariantivEXTPROC) extgl_GetProcAddress("glVariantivEXT");
    glVariantfvEXT = (glVariantfvEXTPROC) extgl_GetProcAddress("glVariantfvEXT");
    glVariantdvEXT = (glVariantdvEXTPROC) extgl_GetProcAddress("glVariantdvEXT");
    glVariantubvEXT = (glVariantubvEXTPROC) extgl_GetProcAddress("glVariantubvEXT");
    glVariantusvEXT = (glVariantusvEXTPROC) extgl_GetProcAddress("glVariantusvEXT");
    glVariantuivEXT = (glVariantuivEXTPROC) extgl_GetProcAddress("glVariantuivEXT");
    glVariantPointerEXT = (glVariantPointerEXTPROC) extgl_GetProcAddress("glVariantPointerEXT");
    glEnableVariantClientStateEXT = (glEnableVariantClientStateEXTPROC) extgl_GetProcAddress("glEnableVariantClientStateEXT");
    glDisableVariantClientStateEXT = (glDisableVariantClientStateEXTPROC) extgl_GetProcAddress("glDisableVariantClientStateEXT");
    glBindLightParameterEXT = (glBindLightParameterEXTPROC) extgl_GetProcAddress("glBindLightParameterEXT");
    glBindMaterialParameterEXT = (glBindMaterialParameterEXTPROC) extgl_GetProcAddress("glBindMaterialParameterEXT");
    glBindTexGenParameterEXT = (glBindTexGenParameterEXTPROC) extgl_GetProcAddress("glBindTexGenParameterEXT");
    glBindTextureUnitParameterEXT = (glBindTextureUnitParameterEXTPROC) extgl_GetProcAddress("glBindTextureUnitParameterEXT");
    glBindParameterEXT = (glBindParameterEXTPROC) extgl_GetProcAddress("glBindParameterEXT");
    glIsVariantEnabledEXT = (glIsVariantEnabledEXTPROC) extgl_GetProcAddress("glIsVariantEnabledEXT");
    glGetVariantBooleanvEXT = (glGetVariantBooleanvEXTPROC) extgl_GetProcAddress("glGetVariantBooleanvEXT");
    glGetVariantIntegervEXT = (glGetVariantIntegervEXTPROC) extgl_GetProcAddress("glGetVariantIntegervEXT");
    glGetVariantFloatvEXT = (glGetVariantFloatvEXTPROC) extgl_GetProcAddress("glGetVariantFloatvEXT");
    glGetVariantPointervEXT = (glGetVariantPointervEXTPROC) extgl_GetProcAddress("glGetVariantPointervEXT");
    glGetInvariantBooleanvEXT = (glGetInvariantBooleanvEXTPROC) extgl_GetProcAddress("glGetInvariantBooleanvEXT");
    glGetInvariantIntegervEXT = (glGetInvariantIntegervEXTPROC) extgl_GetProcAddress("glGetInvariantIntegervEXT");
    glGetInvariantFloatvEXT = (glGetInvariantFloatvEXTPROC) extgl_GetProcAddress("glGetInvariantFloatvEXT");
    glGetLocalConstantBooleanvEXT = (glGetLocalConstantBooleanvEXTPROC) extgl_GetProcAddress("glGetLocalConstantBooleanvEXT");
    glGetLocalConstantIntegervEXT = (glGetLocalConstantIntegervEXTPROC) extgl_GetProcAddress("glGetLocalConstantIntegervEXT");
    glGetLocalConstantFloatvEXT = (glGetLocalConstantFloatvEXTPROC) extgl_GetProcAddress("glGetLocalConstantFloatvEXT");
#endif
}

void extgl_InitEXTVertexWeighting()
{
#ifdef GL_EXT_vertex_weighting
    if (!extgl_Extensions.EXT_vertex_weighting)
        return;
    glVertexWeightfEXT = (glVertexWeightfEXTPROC) extgl_GetProcAddress("glVertexWeightfEXT");
    glVertexWeightfvEXT = (glVertexWeightfvEXTPROC) extgl_GetProcAddress("glVertexWeightfvEXT");
    glVertexWeightPointerEXT = (glVertexWeightPointerEXTPROC) extgl_GetProcAddress("glVertexWeightPointerEXT");
#endif
}


void extgl_InitHPImageTransform()
{
#ifdef GL_HP_image_transform
    if (!extgl_Extensions.HP_image_transform)
        return;
	glImageTransformParameteriHP = (glImageTransformParameteriHPPROC) extgl_GetProcAddress("glImageTransformParameteriHP");
	glImageTransformParameterfHP = (glImageTransformParameterfHPPROC) extgl_GetProcAddress("glImageTransformParameterfHP");
	glImageTransformParameterivHP = (glImageTransformParameterivHPPROC) extgl_GetProcAddress("glImageTransformParameterivHP");
	glImageTransformParameterfvHP = (glImageTransformParameterfvHPPROC) extgl_GetProcAddress("glImageTransformParameterfvHP");
	glGetImageTransformParameterivHP = (glGetImageTransformParameterivHPPROC) extgl_GetProcAddress("glGetImageTransformParameterivHP");
	glGetImageTransformParameterfvHP = (glGetImageTransformParameterfvHPPROC) extgl_GetProcAddress("glGetImageTransformParameterfvHP");
#endif
}


void extgl_InitIBMMultimodeDrawArrays()
{
#ifdef GL_IBM_multimode_draw_arrays
    if (!extgl_Extensions.IBM_multimode_draw_arrays)
        return;
	glMultiModeDrawArraysIBM = (glMultiModeDrawArraysIBMPROC) extgl_GetProcAddress("glMultiModeDrawArraysIBM");
	glMultiModeDrawElementsIBM = (glMultiModeDrawElementsIBMPROC) extgl_GetProcAddress("glMultiModeDrawElementsIBM");
#endif
}

void extgl_InitIBMVertexArrayLists()
{
#ifdef GL_IBM_vertex_array_lists
    if (!extgl_Extensions.IBM_vertex_array_lists)
        return;
	glColorPointerListIBM = (glColorPointerListIBMPROC) extgl_GetProcAddress("glColorPointerListIBM");
	glSecondaryColorPointerListIBM = (glSecondaryColorPointerListIBMPROC) extgl_GetProcAddress("glSecondaryColorPointerListIBM");
	glEdgeFlagPointerListIBM = (glEdgeFlagPointerListIBMPROC) extgl_GetProcAddress("glEdgeFlagPointerListIBM");
	glFogCoordPointerListIBM = (glFogCoordPointerListIBMPROC) extgl_GetProcAddress("glFogCoordPointerListIBM");
	glIndexPointerListIBM = (glIndexPointerListIBMPROC) extgl_GetProcAddress("glIndexPointerListIBM");
	glNormalPointerListIBM = (glNormalPointerListIBMPROC) extgl_GetProcAddress("glNormalPointerListIBM");
	glTexCoordPointerListIBM = (glTexCoordPointerListIBMPROC) extgl_GetProcAddress("glTexCoordPointerListIBM");
	glVertexPointerListIBM = (glVertexPointerListIBMPROC) extgl_GetProcAddress("glVertexPointerListIBM");
#endif
}


void extgl_InitINTELParallelArrays()
{
#ifdef GL_INTEL_parallel_arrays
    if (!extgl_Extensions.INTEL_parallel_arrays)
        return;
	glVertexPointervINTEL = (glVertexPointervINTELPROC) extgl_GetProcAddress("glVertexPointervINTEL");
	glNormalPointervINTEL = (glNormalPointervINTELPROC) extgl_GetProcAddress("glNormalPointervINTEL");
	glColorPointervINTEL = (glColorPointervINTELPROC) extgl_GetProcAddress("glColorPointervINTEL");
	glTexCoordPointervINTEL = (glTexCoordPointervINTELPROC) extgl_GetProcAddress("glTexCoordPointervINTEL");
#endif
}

void extgl_InitKTXBufferRegion()
{
#ifdef GL_KTX_buffer_region
    if (!extgl_Extensions.KTX_buffer_region)
        return;
	glBufferRegionEnabled = (glBufferRegionEnabledPROC) extgl_GetProcAddress("glBufferRegionEnabled");
	glNewBufferRegion = (glNewBufferRegionPROC) extgl_GetProcAddress("glNewBufferRegion");
	glDeleteBufferRegion = (glDeleteBufferRegionPROC) extgl_GetProcAddress("glDeleteBufferRegion");
	glReadBufferRegion = (glReadBufferRegionPROC) extgl_GetProcAddress("glReadBufferRegion");
	glDrawBufferRegion = (glDrawBufferRegionPROC) extgl_GetProcAddress("glDrawBufferRegion");
#endif
}

void extgl_InitMESAResizeBuffers()
{
#ifdef GL_MESA_resize_buffers
    if (!extgl_Extensions.MESA_resize_buffers)
        return;
	glResizeBuffersMESA = (glResizeBuffersMESAPROC) extgl_GetProcAddress("glResizeBuffersMESA");
#endif
}

void extgl_InitMESAWindowPos()
{
#ifdef GL_MESA_window_pos
    if (!extgl_Extensions.MESA_window_pos)
        return;
	glWindowPos2dMESA = (glWindowPos2dMESAPROC) extgl_GetProcAddress("glWindowPos2dMESA");
	glWindowPos2dvMESA = (glWindowPos2dvMESAPROC) extgl_GetProcAddress("glWindowPos2dvMESA");
	glWindowPos2fMESA = (glWindowPos2fMESAPROC) extgl_GetProcAddress("glWindowPos2fMESA");
	glWindowPos2fvMESA = (glWindowPos2fvMESAPROC) extgl_GetProcAddress("glWindowPos2fvMESA");
	glWindowPos2iMESA = (glWindowPos2iMESAPROC) extgl_GetProcAddress("glWindowPos2iMESA");
	glWindowPos2ivMESA = (glWindowPos2ivMESAPROC) extgl_GetProcAddress("glWindowPos2ivMESA");
	glWindowPos2sMESA = (glWindowPos2sMESAPROC) extgl_GetProcAddress("glWindowPos2sMESA");
	glWindowPos2svMESA = (glWindowPos2svMESAPROC) extgl_GetProcAddress("glWindowPos2svMESA");
	glWindowPos3dMESA = (glWindowPos3dMESAPROC) extgl_GetProcAddress("glWindowPos3dMESA");
	glWindowPos3dvMESA = (glWindowPos3dvMESAPROC) extgl_GetProcAddress("glWindowPos3dvMESA");
	glWindowPos3fMESA = (glWindowPos3fMESAPROC) extgl_GetProcAddress("glWindowPos3fMESA");
	glWindowPos3fvMESA = (glWindowPos3fvMESAPROC) extgl_GetProcAddress("glWindowPos3fvMESA");
	glWindowPos3iMESA = (glWindowPos3iMESAPROC) extgl_GetProcAddress("glWindowPos3iMESA");
	glWindowPos3ivMESA = (glWindowPos3ivMESAPROC) extgl_GetProcAddress("glWindowPos3ivMESA");
	glWindowPos3sMESA = (glWindowPos3sMESAPROC) extgl_GetProcAddress("glWindowPos3sMESA");
	glWindowPos3svMESA = (glWindowPos3svMESAPROC) extgl_GetProcAddress("glWindowPos3svMESA");
	glWindowPos4dMESA = (glWindowPos4dMESAPROC) extgl_GetProcAddress("glWindowPos4dMESA");
	glWindowPos4dvMESA = (glWindowPos4dvMESAPROC) extgl_GetProcAddress("glWindowPos4dvMESA");
	glWindowPos4fMESA = (glWindowPos4fMESAPROC) extgl_GetProcAddress("glWindowPos4fMESA");
	glWindowPos4fvMESA = (glWindowPos4fvMESAPROC) extgl_GetProcAddress("glWindowPos4fvMESA");
	glWindowPos4iMESA = (glWindowPos4iMESAPROC) extgl_GetProcAddress("glWindowPos4iMESA");
	glWindowPos4ivMESA = (glWindowPos4ivMESAPROC) extgl_GetProcAddress("glWindowPos4ivMESA");
	glWindowPos4sMESA = (glWindowPos4sMESAPROC) extgl_GetProcAddress("glWindowPos4sMESA");
	glWindowPos4svMESA = (glWindowPos4svMESAPROC) extgl_GetProcAddress("glWindowPos4svMESA");
#endif
}


void extgl_InitNVElementArray()
{
#ifdef GL_NV_element_array
    if (!extgl_Extensions.NV_element_array)
        return;
    glElementPointerNV = (glElementPointerNVPROC) extgl_GetProcAddress("glElementPointerNV");
    glDrawElementArrayNV = (glDrawElementArrayNVPROC) extgl_GetProcAddress("glDrawElementArrayNV");
    glDrawRangeElementArrayNV = (glDrawRangeElementArrayNVPROC) extgl_GetProcAddress("glDrawRangeElementArrayNV");
    glMultiDrawElementArrayNV = (glMultiDrawElementArrayNVPROC) extgl_GetProcAddress("glMultiDrawElementArrayNV");
    glMultiDrawRangeElementArrayNV = (glMultiDrawRangeElementArrayNVPROC) extgl_GetProcAddress("glMultiDrawRangeElementArrayNV");
#endif
}

void extgl_InitNVEvaluators()
{
#ifdef GL_NV_evaluators
    if (!extgl_Extensions.NV_evaluators)
        return;
    glMapControlPointsNV = (glMapControlPointsNVPROC) extgl_GetProcAddress("glMapControlPointsNV");
    glMapParameterivNV = (glMapParameterivNVPROC) extgl_GetProcAddress("glMapParameterivNV");
    glMapParameterfvNV = (glMapParameterfvNVPROC) extgl_GetProcAddress("glMapParameterfvNV");
    glGetMapControlPointsNV = (glGetMapControlPointsNVPROC) extgl_GetProcAddress("glGetMapControlPointsNV");
    glGetMapParameterivNV = (glGetMapParameterivNVPROC) extgl_GetProcAddress("glGetMapParameterivNV");
    glGetMapParameterfvNV = (glGetMapParameterfvNVPROC) extgl_GetProcAddress("glGetMapParameterfvNV");
    glGetMapAttribParameterivNV = (glGetMapAttribParameterivNVPROC) extgl_GetProcAddress("glGetMapAttribParameterivNV");
    glGetMapAttribParameterfvNV = (glGetMapAttribParameterfvNVPROC) extgl_GetProcAddress("glGetMapAttribParameterfvNV");
    glEvalMapsNV = (glEvalMapsNVPROC) extgl_GetProcAddress("glEvalMapsNV");
#endif
}

void extgl_InitNVFence()
{
#ifdef GL_NV_fence
    if (!extgl_Extensions.NV_fence)
        return;
    glGenFencesNV = (glGenFencesNVPROC) extgl_GetProcAddress("glGenFencesNV");
    glDeleteFencesNV = (glDeleteFencesNVPROC) extgl_GetProcAddress("glDeleteFencesNV");
    glSetFenceNV = (glSetFenceNVPROC) extgl_GetProcAddress("glSetFenceNV");
    glTestFenceNV = (glTestFenceNVPROC) extgl_GetProcAddress("glTestFenceNV");
    glFinishFenceNV = (glFinishFenceNVPROC) extgl_GetProcAddress("glFinishFenceNV");
    glIsFenceNV = (glIsFenceNVPROC) extgl_GetProcAddress("glIsFenceNV");
    glGetFenceivNV = (glGetFenceivNVPROC) extgl_GetProcAddress("glGetFenceivNV");
#endif
}

void extgl_InitNVFragmentProgram()
{
#ifdef GL_NV_fragment_program
    if (!extgl_Extensions.NV_fragment_program)
        return;
    glProgramNamedParameter4fNV = (glProgramNamedParameter4fNVPROC) extgl_GetProcAddress("glProgramNamedParameter4fNV");
    glProgramNamedParameter4dNV = (glProgramNamedParameter4dNVPROC) extgl_GetProcAddress("glProgramNamedParameter4dNV");
    glProgramNamedParameter4fvNV = (glProgramNamedParameter4fvNVPROC) extgl_GetProcAddress("glProgramNamedParameter4fvNV");
    glProgramNamedParameter4dvNV = (glProgramNamedParameter4dvNVPROC) extgl_GetProcAddress("glProgramNamedParameter4dvNV");
    glGetProgramNamedParameterfvNV = (glGetProgramNamedParameterfvNVPROC) extgl_GetProcAddress("glGetProgramNamedParameterfvNV");
    glGetProgramNamedParameterdvNV = (glGetProgramNamedParameterdvNVPROC) extgl_GetProcAddress("glGetProgramNamedParameterdvNV");
#endif /* GL_NV_fragment_program */
}

void extgl_InitNVHalfFloat()
{
#ifdef GL_NV_half_float
    if (!extgl_Extensions.NV_half_float)
        return;
    glVertex2hNV = (glVertex2hNVPROC) extgl_GetProcAddress("glVertex2hNV");
    glVertex2hvNV = (glVertex2hvNVPROC) extgl_GetProcAddress("glVertex2hvNV");
    glVertex3hNV = (glVertex3hNVPROC) extgl_GetProcAddress("glVertex3hNV");
    glVertex3hvNV = (glVertex3hvNVPROC) extgl_GetProcAddress("glVertex3hvNV");
    glVertex4hNV = (glVertex4hNVPROC) extgl_GetProcAddress("glVertex4hNV");
    glVertex4hvNV = (glVertex4hvNVPROC) extgl_GetProcAddress("glVertex4hvNV");
    glNormal3hNV = (glNormal3hNVPROC) extgl_GetProcAddress("glNormal3hNV");
    glNormal3hvNV = (glNormal3hvNVPROC) extgl_GetProcAddress("glNormal3hvNV");
    glColor3hNV = (glColor3hNVPROC) extgl_GetProcAddress("glColor3hNV");
    glColor3hvNV = (glColor3hvNVPROC) extgl_GetProcAddress("glColor3hvNV");
    glColor4hNV = (glColor4hNVPROC) extgl_GetProcAddress("glColor4hNV");
    glColor4hvNV = (glColor4hvNVPROC) extgl_GetProcAddress("glColor4hvNV");
    glTexCoord1hNV = (glTexCoord1hNVPROC) extgl_GetProcAddress("glTexCoord1hNV");
    glTexCoord1hvNV = (glTexCoord1hvNVPROC) extgl_GetProcAddress("glTexCoord1hvNV");
    glTexCoord2hNV = (glTexCoord2hNVPROC) extgl_GetProcAddress("glTexCoord2hNV");
    glTexCoord2hvNV = (glTexCoord2hvNVPROC) extgl_GetProcAddress("glTexCoord2hvNV");
    glTexCoord3hNV = (glTexCoord3hNVPROC) extgl_GetProcAddress("glTexCoord3hNV");
    glTexCoord3hvNV = (glTexCoord3hvNVPROC) extgl_GetProcAddress("glTexCoord3hvNV");
    glTexCoord4hNV = (glTexCoord4hNVPROC) extgl_GetProcAddress("glTexCoord4hNV");
    glTexCoord4hvNV = (glTexCoord4hvNVPROC) extgl_GetProcAddress("glTexCoord4hvNV");

    if ((extgl_Extensions.OpenGL13) || (extgl_Extensions.ARB_multitexture))
    {
        glMultiTexCoord1hNV = (glMultiTexCoord1hNVPROC) extgl_GetProcAddress("glMultiTexCoord1hNV");
        glMultiTexCoord1hvNV = (glMultiTexCoord1hvNVPROC) extgl_GetProcAddress("glMultiTexCoord1hvNV");
        glMultiTexCoord2hNV = (glMultiTexCoord2hNVPROC) extgl_GetProcAddress("glMultiTexCoord2hNV");
        glMultiTexCoord2hvNV = (glMultiTexCoord2hvNVPROC) extgl_GetProcAddress("glMultiTexCoord2hvNV");
        glMultiTexCoord3hNV = (glMultiTexCoord3hNVPROC) extgl_GetProcAddress("glMultiTexCoord3hNV");
        glMultiTexCoord3hvNV = (glMultiTexCoord3hvNVPROC) extgl_GetProcAddress("glMultiTexCoord3hvNV");
        glMultiTexCoord4hNV = (glMultiTexCoord4hNVPROC) extgl_GetProcAddress("glMultiTexCoord4hNV");
        glMultiTexCoord4hvNV = (glMultiTexCoord4hvNVPROC) extgl_GetProcAddress("glMultiTexCoord4hvNV");
    }

    if (extgl_Extensions.EXT_fog_coord)
    {
        glFogCoordhNV = (glFogCoordhNVPROC) extgl_GetProcAddress("glFogCoordhNV");
        glFogCoordhvNV = (glFogCoordhvNVPROC) extgl_GetProcAddress("glFogCoordhvNV");
    }

    if (extgl_Extensions.EXT_secondary_color)
    {
        glSecondaryColor3hNV = (glSecondaryColor3hNVPROC) extgl_GetProcAddress("glSecondaryColor3hNV");
        glSecondaryColor3hvNV = (glSecondaryColor3hvNVPROC) extgl_GetProcAddress("glSecondaryColor3hvNV");
    }

    if (extgl_Extensions.EXT_vertex_weighting)
    {
        glVertexWeighthNV = (glVertexWeighthNVPROC) extgl_GetProcAddress("glVertexWeighthNV");
        glVertexWeighthvNV = (glVertexWeighthvNVPROC) extgl_GetProcAddress("glVertexWeighthvNV");
    }

    if (extgl_Extensions.NV_vertex_program)
    {
        glVertexAttrib1hNV = (glVertexAttrib1hNVPROC) extgl_GetProcAddress("glVertexAttrib1hNV");
        glVertexAttrib1hvNV = (glVertexAttrib1hvNVPROC) extgl_GetProcAddress("glVertexAttrib1hvNV");
        glVertexAttrib2hNV = (glVertexAttrib2hNVPROC) extgl_GetProcAddress("glVertexAttrib2hNV");
        glVertexAttrib2hvNV = (glVertexAttrib2hvNVPROC) extgl_GetProcAddress("glVertexAttrib2hvNV");
        glVertexAttrib3hNV = (glVertexAttrib3hNVPROC) extgl_GetProcAddress("glVertexAttrib3hNV");
        glVertexAttrib3hvNV = (glVertexAttrib3hvNVPROC) extgl_GetProcAddress("glVertexAttrib3hvNV");
        glVertexAttrib4hNV = (glVertexAttrib4hNVPROC) extgl_GetProcAddress("glVertexAttrib4hNV");
        glVertexAttrib4hvNV = (glVertexAttrib4hvNVPROC) extgl_GetProcAddress("glVertexAttrib4hvNV");
        glVertexAttribs1hvNV = (glVertexAttribs1hvNVPROC) extgl_GetProcAddress("glVertexAttribs1hvNV");
        glVertexAttribs2hvNV = (glVertexAttribs2hvNVPROC) extgl_GetProcAddress("glVertexAttribs2hvNV");
        glVertexAttribs3hvNV = (glVertexAttribs3hvNVPROC) extgl_GetProcAddress("glVertexAttribs3hvNV");
        glVertexAttribs4hvNV = (glVertexAttribs4hvNVPROC) extgl_GetProcAddress("glVertexAttribs4hvNV");
    }
#endif /* GL_NV_half_float */
}

void extgl_InitNVOcclusionQuery()
{
#ifdef GL_NV_occlusion_query
    if (!extgl_Extensions.NV_occlusion_query)
        return;
    glGenOcclusionQueriesNV = (glGenOcclusionQueriesNVPROC) extgl_GetProcAddress("glGenOcclusionQueriesNV");
    glDeleteOcclusionQueriesNV = (glDeleteOcclusionQueriesNVPROC) extgl_GetProcAddress("glDeleteOcclusionQueriesNV");
    glIsOcclusionQueryNV = (glIsOcclusionQueryNVPROC) extgl_GetProcAddress("glIsOcclusionQueryNV");
    glBeginOcclusionQueryNV = (glBeginOcclusionQueryNVPROC) extgl_GetProcAddress("glBeginOcclusionQueryNV");
    glEndOcclusionQueryNV = (glEndOcclusionQueryNVPROC) extgl_GetProcAddress("glEndOcclusionQueryNV");
    glGetOcclusionQueryivNV = (glGetOcclusionQueryivNVPROC) extgl_GetProcAddress("glGetOcclusionQueryivNV");
    glGetOcclusionQueryuivNV = (glGetOcclusionQueryuivNVPROC) extgl_GetProcAddress("glGetOcclusionQueryuivNV");
#endif
}

void extgl_InitNVPixelDataRange()
{
#ifdef GL_NV_pixel_data_range
    if (!extgl_Extensions.NV_pixel_data_range)
        return;
    glPixelDataRangeNV = (glPixelDataRangeNVPROC) extgl_GetProcAddress("glPixelDataRangeNV");
    glFlushPixelDataRangeNV = (glFlushPixelDataRangeNVPROC) extgl_GetProcAddress("glFlushPixelDataRangeNV");
#endif /* GL_NV_pixel_data_range */
}

void extgl_InitNVPointSprite()
{
#ifdef GL_NV_point_sprite
    if (!extgl_Extensions.NV_point_sprite)
        return;
    glPointParameteriNV = (glPointParameteriNVPROC) extgl_GetProcAddress("glPointParameteriNV");
    glPointParameterivNV = (glPointParameterivNVPROC) extgl_GetProcAddress("glPointParameterivNV");
#endif
}
void extgl_InitNVPrimitiveRestart()
{
#ifdef GL_NV_primitive_restart
    if (!extgl_Extensions.NV_primitive_restart)
        return;
    glPrimitiveRestartNV = (glPrimitiveRestartNVPROC) extgl_GetProcAddress("glPrimitiveRestartNV");
    glPrimitiveRestartIndexNV = (glPrimitiveRestartIndexNVPROC) extgl_GetProcAddress("glPrimitiveRestartIndexNV");
#endif /* GL_NV_primitive_restart */
}

void extgl_InitNVRegisterCombiners()
{
#ifdef GL_NV_register_combiners
    if (!extgl_Extensions.NV_register_combiners)
        return;
    glCombinerParameterfvNV = (glCombinerParameterfvNVPROC) extgl_GetProcAddress("glCombinerParameterfvNV");
    glCombinerParameterfNV = (glCombinerParameterfNVPROC) extgl_GetProcAddress("glCombinerParameterfNV");
    glCombinerParameterivNV = (glCombinerParameterivNVPROC) extgl_GetProcAddress("glCombinerParameterivNV");
    glCombinerParameteriNV = (glCombinerParameteriNVPROC) extgl_GetProcAddress("glCombinerParameteriNV");
    glCombinerInputNV = (glCombinerInputNVPROC) extgl_GetProcAddress("glCombinerInputNV");
    glCombinerOutputNV = (glCombinerOutputNVPROC) extgl_GetProcAddress("glCombinerOutputNV");
    glFinalCombinerInputNV = (glFinalCombinerInputNVPROC) extgl_GetProcAddress("glFinalCombinerInputNV");
    glGetCombinerInputParameterfvNV = (glGetCombinerInputParameterfvNVPROC) extgl_GetProcAddress("glGetCombinerInputParameterfvNV");
    glGetCombinerInputParameterivNV = (glGetCombinerInputParameterivNVPROC) extgl_GetProcAddress("glGetCombinerInputParameterivNV");
    glGetCombinerOutputParameterfvNV = (glGetCombinerOutputParameterfvNVPROC) extgl_GetProcAddress("glGetCombinerOutputParameterfvNV");
    glGetCombinerOutputParameterivNV = (glGetCombinerOutputParameterivNVPROC) extgl_GetProcAddress("glGetCombinerOutputParameterivNV");
    glGetFinalCombinerInputParameterfvNV = (glGetFinalCombinerInputParameterfvNVPROC) extgl_GetProcAddress("glGetFinalCombinerInputParameterfvNV");
    glGetFinalCombinerInputParameterivNV = (glGetFinalCombinerInputParameterivNVPROC) extgl_GetProcAddress("glGetFinalCombinerInputParameterivNV");
#endif
}

void extgl_InitNVRegisterCombiners2()
{
#ifdef GL_NV_register_combiners
    if (!extgl_Extensions.NV_register_combiners2)
        return;
    glCombinerStageParameterfvNV = (glCombinerStageParameterfvNVPROC) extgl_GetProcAddress("glCombinerStageParameterfvNV");
    glGetCombinerStageParameterfvNV = (glGetCombinerStageParameterfvNVPROC) extgl_GetProcAddress("glGetCombinerStageParameterfvNV");
#endif
}

void extgl_InitNVVertexArrayRange()
{
#ifdef GL_NV_vertex_array_range
    if (!extgl_Extensions.NV_vertex_array_range)
        return;
    glFlushVertexArrayRangeNV = (glFlushVertexArrayRangeNVPROC) extgl_GetProcAddress("glFlushVertexArrayRangeNV");
    glVertexArrayRangeNV = (glVertexArrayRangeNVPROC) extgl_GetProcAddress("glVertexArrayRangeNV");
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
    wglAllocateMemoryNV = (wglAllocateMemoryNVPROC) extgl_GetProcAddress("wglAllocateMemoryNV");
    wglFreeMemoryNV = (wglFreeMemoryNVPROC) extgl_GetProcAddress("wglFreeMemoryNV");
#else
    glXAllocateMemoryNV = (glXAllocateMemoryNVPROC) extgl_GetProcAddress("glXAllocateMemoryNV");
    glXFreeMemoryNV = (glXFreeMemoryNVPROC) extgl_GetProcAddress("glXFreeMemoryNV");
#endif /* WIN32 */
#endif
}

void extgl_InitNVVertexProgram()
{
#ifdef GL_NV_vertex_program
    if (!extgl_Extensions.NV_vertex_program)
        return;
    glBindProgramNV = (glBindProgramNVPROC) extgl_GetProcAddress("glBindProgramNV");
    glDeleteProgramsNV = (glDeleteProgramsNVPROC) extgl_GetProcAddress("glDeleteProgramsNV");
    glExecuteProgramNV = (glExecuteProgramNVPROC) extgl_GetProcAddress("glExecuteProgramNV");
    glGenProgramsNV = (glGenProgramsNVPROC) extgl_GetProcAddress("glGenProgramsNV");
    glAreProgramsResidentNV = (glAreProgramsResidentNVPROC) extgl_GetProcAddress("glAreProgramsResidentNV");
    glRequestResidentProgramsNV = (glRequestResidentProgramsNVPROC) extgl_GetProcAddress("glRequestResidentProgramsNV");
    glGetProgramParameterfvNV = (glGetProgramParameterfvNVPROC) extgl_GetProcAddress("glGetProgramParameterfvNV");
    glGetProgramParameterdvNV = (glGetProgramParameterdvNVPROC) extgl_GetProcAddress("glGetProgramParameterdvNV");
    glGetProgramivNV = (glGetProgramivNVPROC) extgl_GetProcAddress("glGetProgramivNV");
    glGetProgramStringNV = (glGetProgramStringNVPROC) extgl_GetProcAddress("glGetProgramStringNV");
    glGetTrackMatrixivNV = (glGetTrackMatrixivNVPROC) extgl_GetProcAddress("glGetTrackMatrixivNV");
    glGetVertexAttribdvNV = (glGetVertexAttribdvNVPROC) extgl_GetProcAddress("glGetVertexAttribdvNV");
    glGetVertexAttribfvNV = (glGetVertexAttribfvNVPROC) extgl_GetProcAddress("glGetVertexAttribfvNV");
    glGetVertexAttribivNV = (glGetVertexAttribivNVPROC) extgl_GetProcAddress("glGetVertexAttribivNV");
    glGetVertexAttribPointervNV = (glGetVertexAttribPointervNVPROC) extgl_GetProcAddress("glGetVertexAttribPointervNV");
    glIsProgramNV = (glIsProgramNVPROC) extgl_GetProcAddress("glIsProgramNV");
    glLoadProgramNV = (glLoadProgramNVPROC) extgl_GetProcAddress("glLoadProgramNV");
    glProgramParameter4fNV = (glProgramParameter4fNVPROC) extgl_GetProcAddress("glProgramParameter4fNV");
    glProgramParameter4dNV = (glProgramParameter4dNVPROC) extgl_GetProcAddress("glProgramParameter4dNV");
    glProgramParameter4dvNV = (glProgramParameter4dvNVPROC) extgl_GetProcAddress("glProgramParameter4dvNV");
    glProgramParameter4fvNV = (glProgramParameter4fvNVPROC) extgl_GetProcAddress("glProgramParameter4fvNV");
    glProgramParameters4dvNV = (glProgramParameters4dvNVPROC) extgl_GetProcAddress("glProgramParameters4dvNV");
    glProgramParameters4fvNV = (glProgramParameters4fvNVPROC) extgl_GetProcAddress("glProgramParameters4fvNV");
    glTrackMatrixNV = (glTrackMatrixNVPROC) extgl_GetProcAddress("glTrackMatrixNV");
    glVertexAttribPointerNV = (glVertexAttribPointerNVPROC) extgl_GetProcAddress("glVertexAttribPointerNV");
    glVertexAttrib1sNV = (glVertexAttrib1sNVPROC) extgl_GetProcAddress("glVertexAttrib1sNV");
    glVertexAttrib1fNV = (glVertexAttrib1fNVPROC) extgl_GetProcAddress("glVertexAttrib1fNV");
    glVertexAttrib1dNV = (glVertexAttrib1dNVPROC) extgl_GetProcAddress("glVertexAttrib1dNV");
    glVertexAttrib2sNV = (glVertexAttrib2sNVPROC) extgl_GetProcAddress("glVertexAttrib2sNV");
    glVertexAttrib2fNV = (glVertexAttrib2fNVPROC) extgl_GetProcAddress("glVertexAttrib2fNV");
    glVertexAttrib2dNV = (glVertexAttrib2dNVPROC) extgl_GetProcAddress("glVertexAttrib2dNV");
    glVertexAttrib3sNV = (glVertexAttrib3sNVPROC) extgl_GetProcAddress("glVertexAttrib3sNV");
    glVertexAttrib3fNV = (glVertexAttrib3fNVPROC) extgl_GetProcAddress("glVertexAttrib3fNV");
    glVertexAttrib3dNV = (glVertexAttrib3dNVPROC) extgl_GetProcAddress("glVertexAttrib3dNV");
    glVertexAttrib4sNV = (glVertexAttrib4sNVPROC) extgl_GetProcAddress("glVertexAttrib4sNV");
    glVertexAttrib4fNV = (glVertexAttrib4fNVPROC) extgl_GetProcAddress("glVertexAttrib4fNV");
    glVertexAttrib4dNV = (glVertexAttrib4dNVPROC) extgl_GetProcAddress("glVertexAttrib4dNV");
    glVertexAttrib4ubNV = (glVertexAttrib4ubNVPROC) extgl_GetProcAddress("glVertexAttrib4ubNV");
    glVertexAttrib1svNV = (glVertexAttrib1svNVPROC) extgl_GetProcAddress("glVertexAttrib1svNV");
    glVertexAttrib1fvNV = (glVertexAttrib1fvNVPROC) extgl_GetProcAddress("glVertexAttrib1fvNV");
    glVertexAttrib1dvNV = (glVertexAttrib1dvNVPROC) extgl_GetProcAddress("glVertexAttrib1dvNV");
    glVertexAttrib2svNV = (glVertexAttrib2svNVPROC) extgl_GetProcAddress("glVertexAttrib2svNV");
    glVertexAttrib2fvNV = (glVertexAttrib2fvNVPROC) extgl_GetProcAddress("glVertexAttrib2fvNV");
    glVertexAttrib2dvNV = (glVertexAttrib2dvNVPROC) extgl_GetProcAddress("glVertexAttrib2dvNV");
    glVertexAttrib3svNV = (glVertexAttrib3svNVPROC) extgl_GetProcAddress("glVertexAttrib3svNV");
    glVertexAttrib3fvNV = (glVertexAttrib3fvNVPROC) extgl_GetProcAddress("glVertexAttrib3fvNV");
    glVertexAttrib3dvNV = (glVertexAttrib3dvNVPROC) extgl_GetProcAddress("glVertexAttrib3dvNV");
    glVertexAttrib4svNV = (glVertexAttrib4svNVPROC) extgl_GetProcAddress("glVertexAttrib4svNV");
    glVertexAttrib4fvNV = (glVertexAttrib4fvNVPROC) extgl_GetProcAddress("glVertexAttrib4fvNV");
    glVertexAttrib4dvNV = (glVertexAttrib4dvNVPROC) extgl_GetProcAddress("glVertexAttrib4dvNV");
    glVertexAttrib4ubvNV = (glVertexAttrib4ubvNVPROC) extgl_GetProcAddress("glVertexAttrib4ubvNV");
    glVertexAttribs1svNV = (glVertexAttribs1svNVPROC) extgl_GetProcAddress("glVertexAttribs1svNV");
    glVertexAttribs1fvNV = (glVertexAttribs1fvNVPROC) extgl_GetProcAddress("glVertexAttribs1fvNV");
    glVertexAttribs1dvNV = (glVertexAttribs1dvNVPROC) extgl_GetProcAddress("glVertexAttribs1dvNV");
    glVertexAttribs2svNV = (glVertexAttribs2svNVPROC) extgl_GetProcAddress("glVertexAttribs2svNV");
    glVertexAttribs2fvNV = (glVertexAttribs2fvNVPROC) extgl_GetProcAddress("glVertexAttribs2fvNV");
    glVertexAttribs2dvNV = (glVertexAttribs2dvNVPROC) extgl_GetProcAddress("glVertexAttribs2dvNV");
    glVertexAttribs3svNV = (glVertexAttribs3svNVPROC) extgl_GetProcAddress("glVertexAttribs3svNV");
    glVertexAttribs3fvNV = (glVertexAttribs3fvNVPROC) extgl_GetProcAddress("glVertexAttribs3fvNV");
    glVertexAttribs3dvNV = (glVertexAttribs3dvNVPROC) extgl_GetProcAddress("glVertexAttribs3dvNV");
    glVertexAttribs4svNV = (glVertexAttribs4svNVPROC) extgl_GetProcAddress("glVertexAttribs4svNV");
    glVertexAttribs4fvNV = (glVertexAttribs4fvNVPROC) extgl_GetProcAddress("glVertexAttribs4fvNV");
    glVertexAttribs4dvNV = (glVertexAttribs4dvNVPROC) extgl_GetProcAddress("glVertexAttribs4dvNV");
    glVertexAttribs4ubvNV = (glVertexAttribs4ubvNVPROC) extgl_GetProcAddress("glVertexAttribs4ubvNV");
#endif
}


void extgl_InitOpenGL1_2()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_VERSION_1_2
    if (!extgl_Extensions.OpenGL12)
        return;
    glTexImage3D = (glTexImage3DPROC) extgl_GetProcAddress("glTexImage3D");
    glTexSubImage3D = (glTexSubImage3DPROC) extgl_GetProcAddress("glTexSubImage3D");
    glCopyTexSubImage3D = (glCopyTexSubImage3DPROC) extgl_GetProcAddress("glCopyTexSubImage3D");
    glDrawRangeElements = (glDrawRangeElementsPROC) extgl_GetProcAddress("glDrawRangeElements");
#endif /* GL_VERSION_1_2 */
#endif /* WIN32 */
}
void extgl_InitOpenGL1_3()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_VERSION_1_3
    if (!extgl_Extensions.OpenGL13)
        return;
    glActiveTexture = (glActiveTexturePROC) extgl_GetProcAddress("glActiveTexture");
    glClientActiveTexture = (glClientActiveTexturePROC) extgl_GetProcAddress("glClientActiveTexture");

    glMultiTexCoord1d = (glMultiTexCoord1dPROC) extgl_GetProcAddress("glMultiTexCoord1d");
    glMultiTexCoord1dv = (glMultiTexCoord1dvPROC) extgl_GetProcAddress("glMultiTexCoord1dv");
    glMultiTexCoord1f = (glMultiTexCoord1fPROC) extgl_GetProcAddress("glMultiTexCoord1f");
    glMultiTexCoord1fv = (glMultiTexCoord1fvPROC) extgl_GetProcAddress("glMultiTexCoord1fv");
    glMultiTexCoord1i = (glMultiTexCoord1iPROC) extgl_GetProcAddress("glMultiTexCoord1i");
    glMultiTexCoord1iv = (glMultiTexCoord1ivPROC) extgl_GetProcAddress("glMultiTexCoord1iv");
    glMultiTexCoord1s = (glMultiTexCoord1sPROC) extgl_GetProcAddress("glMultiTexCoord1s");
    glMultiTexCoord1sv = (glMultiTexCoord1svPROC) extgl_GetProcAddress("glMultiTexCoord1sv");

    glMultiTexCoord2d = (glMultiTexCoord2dPROC) extgl_GetProcAddress("glMultiTexCoord2d");
    glMultiTexCoord2dv = (glMultiTexCoord2dvPROC) extgl_GetProcAddress("glMultiTexCoord2dv");
    glMultiTexCoord2f = (glMultiTexCoord2fPROC) extgl_GetProcAddress("glMultiTexCoord2f");
    glMultiTexCoord2fv = (glMultiTexCoord2fvPROC) extgl_GetProcAddress("glMultiTexCoord2fv");
    glMultiTexCoord2i = (glMultiTexCoord2iPROC) extgl_GetProcAddress("glMultiTexCoord2i");
    glMultiTexCoord2iv = (glMultiTexCoord2ivPROC) extgl_GetProcAddress("glMultiTexCoord2iv");
    glMultiTexCoord2s = (glMultiTexCoord2sPROC) extgl_GetProcAddress("glMultiTexCoord2s");
    glMultiTexCoord2sv = (glMultiTexCoord2svPROC) extgl_GetProcAddress("glMultiTexCoord2sv");

    glMultiTexCoord3d = (glMultiTexCoord3dPROC) extgl_GetProcAddress("glMultiTexCoord3d");
    glMultiTexCoord3dv = (glMultiTexCoord3dvPROC) extgl_GetProcAddress("glMultiTexCoord3dv");
    glMultiTexCoord3f = (glMultiTexCoord3fPROC) extgl_GetProcAddress("glMultiTexCoord3f");
    glMultiTexCoord3fv = (glMultiTexCoord3fvPROC) extgl_GetProcAddress("glMultiTexCoord3fv");
    glMultiTexCoord3i = (glMultiTexCoord3iPROC) extgl_GetProcAddress("glMultiTexCoord3i");
    glMultiTexCoord3iv = (glMultiTexCoord3ivPROC) extgl_GetProcAddress("glMultiTexCoord3iv");
    glMultiTexCoord3s = (glMultiTexCoord3sPROC) extgl_GetProcAddress("glMultiTexCoord3s");
    glMultiTexCoord3sv = (glMultiTexCoord3svPROC) extgl_GetProcAddress("glMultiTexCoord3sv");

    glMultiTexCoord4d = (glMultiTexCoord4dPROC) extgl_GetProcAddress("glMultiTexCoord4d");
    glMultiTexCoord4dv = (glMultiTexCoord4dvPROC) extgl_GetProcAddress("glMultiTexCoord4dv");
    glMultiTexCoord4f = (glMultiTexCoord4fPROC) extgl_GetProcAddress("glMultiTexCoord4f");
    glMultiTexCoord4fv = (glMultiTexCoord4fvPROC) extgl_GetProcAddress("glMultiTexCoord4fv");
    glMultiTexCoord4i = (glMultiTexCoord4iPROC) extgl_GetProcAddress("glMultiTexCoord4i");
    glMultiTexCoord4iv = (glMultiTexCoord4ivPROC) extgl_GetProcAddress("glMultiTexCoord4iv");
    glMultiTexCoord4s = (glMultiTexCoord4sPROC) extgl_GetProcAddress("glMultiTexCoord4s");
    glMultiTexCoord4sv = (glMultiTexCoord4svPROC) extgl_GetProcAddress("glMultiTexCoord4sv");

    glLoadTransposeMatrixf = (glLoadTransposeMatrixfPROC) extgl_GetProcAddress("glLoadTransposeMatrixf");
    glLoadTransposeMatrixd = (glLoadTransposeMatrixdPROC) extgl_GetProcAddress("glLoadTransposeMatrixd");
    glMultTransposeMatrixf = (glMultTransposeMatrixfPROC) extgl_GetProcAddress("glMultTransposeMatrixf");
    glMultTransposeMatrixd = (glMultTransposeMatrixdPROC) extgl_GetProcAddress("glMultTransposeMatrixd");
    glCompressedTexImage3D = (glCompressedTexImage3DPROC) extgl_GetProcAddress("glCompressedTexImage3D");
    glCompressedTexImage2D = (glCompressedTexImage2DPROC) extgl_GetProcAddress("glCompressedTexImage2D");
    glCompressedTexImage1D = (glCompressedTexImage1DPROC) extgl_GetProcAddress("glCompressedTexImage1D");
    glCompressedTexSubImage3D = (glCompressedTexSubImage3DPROC) extgl_GetProcAddress("glCompressedTexSubImage3D");
    glCompressedTexSubImage2D = (glCompressedTexSubImage2DPROC) extgl_GetProcAddress("glCompressedTexSubImage2D");
    glCompressedTexSubImage1D = (glCompressedTexSubImage1DPROC) extgl_GetProcAddress("glCompressedTexSubImage1D");
    glGetCompressedTexImage = (glGetCompressedTexImagePROC) extgl_GetProcAddress("glGetCompressedTexImage");

    glSampleCoverage = (glSampleCoveragePROC) extgl_GetProcAddress("glSampleCoverage");
#endif /* GL_VERSION_1_3 */
#endif /* WIN32 */
}

void extgl_InitOpenGL1_4()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_VERSION_1_4
    if (!extgl_Extensions.OpenGL14)
        return;
    glBlendColor = (glBlendColorPROC) extgl_GetProcAddress("glBlendColor");
    glBlendEquation = (glBlendEquationPROC) extgl_GetProcAddress("glBlendEquation");
    glFogCoordf = (glFogCoordfPROC) extgl_GetProcAddress("glFogCoordf");
    glFogCoordfv = (glFogCoordfvPROC) extgl_GetProcAddress("glFogCoordfv");
    glFogCoordd = (glFogCoorddPROC) extgl_GetProcAddress("glFogCoordd");
    glFogCoorddv = (glFogCoorddvPROC) extgl_GetProcAddress("glFogCoorddv");
    glFogCoordPointer = (glFogCoordPointerPROC) extgl_GetProcAddress("glFogCoordPointer");
    glMultiDrawArrays = (glMultiDrawArraysPROC) extgl_GetProcAddress("glMultiDrawArrays");
    glMultiDrawElements = (glMultiDrawElementsPROC) extgl_GetProcAddress("glMultiDrawElements");
    glPointParameterf = (glPointParameterfPROC) extgl_GetProcAddress("glPointParameterf");
    glPointParameterfv = (glPointParameterfvPROC) extgl_GetProcAddress("glPointParameterfv");
    glSecondaryColor3b = (glSecondaryColor3bPROC) extgl_GetProcAddress("glSecondaryColor3b");
    glSecondaryColor3bv = (glSecondaryColor3bvPROC) extgl_GetProcAddress("glSecondaryColor3bv");
    glSecondaryColor3d = (glSecondaryColor3dPROC) extgl_GetProcAddress("glSecondaryColor3d");
    glSecondaryColor3dv = (glSecondaryColor3dvPROC) extgl_GetProcAddress("glSecondaryColor3dv");
    glSecondaryColor3f = (glSecondaryColor3fPROC) extgl_GetProcAddress("glSecondaryColor3f");
    glSecondaryColor3fv = (glSecondaryColor3fvPROC) extgl_GetProcAddress("glSecondaryColor3fv");
    glSecondaryColor3i = (glSecondaryColor3iPROC) extgl_GetProcAddress("glSecondaryColor3i");
    glSecondaryColor3iv = (glSecondaryColor3ivPROC) extgl_GetProcAddress("glSecondaryColor3iv");
    glSecondaryColor3s = (glSecondaryColor3sPROC) extgl_GetProcAddress("glSecondaryColor3s");
    glSecondaryColor3sv = (glSecondaryColor3svPROC) extgl_GetProcAddress("glSecondaryColor3sv");
    glSecondaryColor3ub = (glSecondaryColor3ubPROC) extgl_GetProcAddress("glSecondaryColor3ub");
    glSecondaryColor3ubv = (glSecondaryColor3ubvPROC) extgl_GetProcAddress("glSecondaryColor3ubv");
    glSecondaryColor3ui = (glSecondaryColor3uiPROC) extgl_GetProcAddress("glSecondaryColor3ui");
    glSecondaryColor3uiv = (glSecondaryColor3uivPROC) extgl_GetProcAddress("glSecondaryColor3uiv");
    glSecondaryColor3us = (glSecondaryColor3usPROC) extgl_GetProcAddress("glSecondaryColor3us");
    glSecondaryColor3usv = (glSecondaryColor3usvPROC) extgl_GetProcAddress("glSecondaryColor3usv");
    glSecondaryColorPointer = (glSecondaryColorPointerPROC) extgl_GetProcAddress("glSecondaryColorPointer");
    glBlendFuncSeparate = (glBlendFuncSeparatePROC) extgl_GetProcAddress("glBlendFuncSeparate");
    glWindowPos2d = (glWindowPos2dPROC) extgl_GetProcAddress("glWindowPos2d");
    glWindowPos2f = (glWindowPos2fPROC) extgl_GetProcAddress("glWindowPos2f");
    glWindowPos2i = (glWindowPos2iPROC) extgl_GetProcAddress("glWindowPos2i");
    glWindowPos2s = (glWindowPos2sPROC) extgl_GetProcAddress("glWindowPos2s");
    glWindowPos2dv = (glWindowPos2dvPROC) extgl_GetProcAddress("glWindowPos2dv");
    glWindowPos2fv = (glWindowPos2fvPROC) extgl_GetProcAddress("glWindowPos2fv");
    glWindowPos2iv = (glWindowPos2ivPROC) extgl_GetProcAddress("glWindowPos2iv");
    glWindowPos2sv = (glWindowPos2svPROC) extgl_GetProcAddress("glWindowPos2sv");
    glWindowPos3d = (glWindowPos3dPROC) extgl_GetProcAddress("glWindowPos3d");
    glWindowPos3f = (glWindowPos3fPROC) extgl_GetProcAddress("glWindowPos3f");
    glWindowPos3i = (glWindowPos3iPROC) extgl_GetProcAddress("glWindowPos3i");
    glWindowPos3s = (glWindowPos3sPROC) extgl_GetProcAddress("glWindowPos3s");
    glWindowPos3dv = (glWindowPos3dvPROC) extgl_GetProcAddress("glWindowPos3dv");
    glWindowPos3fv = (glWindowPos3fvPROC) extgl_GetProcAddress("glWindowPos3fv");
    glWindowPos3iv = (glWindowPos3ivPROC) extgl_GetProcAddress("glWindowPos3iv");
    glWindowPos3sv = (glWindowPos3svPROC) extgl_GetProcAddress("glWindowPos3sv");
#endif /* GL_VERSION_1_4 */
#endif /* WIN32 */
}

void extgl_InitOpenGL1_5()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_VERSION_1_5
    if (!extgl_Extensions.OpenGL15)
        return;
    glGenQueries = (glGenQueriesPROC) extgl_GetProcAddress("glGenQueries");
    glDeleteQueries = (glDeleteQueriesPROC) extgl_GetProcAddress("glDeleteQueries");
    glIsQuery = (glIsQueryPROC) extgl_GetProcAddress("glIsQuery");
    glBeginQuery = (glBeginQueryPROC) extgl_GetProcAddress("glBeginQuery");
    glEndQuery = (glEndQueryPROC) extgl_GetProcAddress("glEndQuery");
    glGetQueryiv = (glGetQueryivPROC) extgl_GetProcAddress("glGetQueryiv");
    glGetQueryObjectiv = (glGetQueryObjectivPROC) extgl_GetProcAddress("glGetQueryObjectiv");
    glGetQueryObjectuiv = (glGetQueryObjectuivPROC) extgl_GetProcAddress("glGetQueryObjectuiv");
    glBindBuffer = (glBindBufferPROC) extgl_GetProcAddress("glBindBuffer");
    glDeleteBuffers = (glDeleteBuffersPROC) extgl_GetProcAddress("glDeleteBuffers");
    glGenBuffers = (glGenBuffersPROC) extgl_GetProcAddress("glGenBuffers");
    glIsBuffer = (glIsBufferPROC) extgl_GetProcAddress("glIsBuffer");
    glBufferData = (glBufferDataPROC) extgl_GetProcAddress("glBufferData");
    glBufferSubData = (glBufferSubDataPROC) extgl_GetProcAddress("glBufferSubData");
    glGetBufferSubData = (glGetBufferSubDataPROC) extgl_GetProcAddress("glGetBufferSubData");
    glMapBuffer = (glMapBufferPROC) extgl_GetProcAddress("glMapBuffer");
    glUnmapBuffer = (glUnmapBufferPROC) extgl_GetProcAddress("glUnmapBuffer");
    glGetBufferParameteriv = (glGetBufferParameterivPROC) extgl_GetProcAddress("glGetBufferParameteriv");
    glGetBufferPointerv = (glGetBufferPointervPROC) extgl_GetProcAddress("glGetBufferPointerv");
#endif /* GL_VERSION_1_5 */
#endif /* WIN32 */
}

void extgl_InitOpenGL2_0()
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#ifdef GL_VERSION_2_0
	if (!extgl_Extensions.OpenGL20)
		return;
	glBlendEquationSeparate = (glBlendEquationSeparatePROC) extgl_GetProcAddress("glBlendEquationSeparate");
	glDrawBuffers = (glDrawBuffersPROC) extgl_GetProcAddress("glDrawBuffers");
	glStencilOpSeparate = (glStencilOpSeparatePROC) extgl_GetProcAddress("glStencilOpSeparate");
	glStencilFuncSeparate = (glStencilFuncSeparatePROC) extgl_GetProcAddress("glStencilFuncSeparate");
	glStencilMaskSeparate = (glStencilMaskSeparatePROC) extgl_GetProcAddress("glStencilMaskSeparate");
	glAttachShader = (glAttachShaderPROC) extgl_GetProcAddress("glAttachShader");
	glBindAttribLocation = (glBindAttribLocationPROC) extgl_GetProcAddress("glBindAttribLocation");
	glCompileShader = (glCompileShaderPROC) extgl_GetProcAddress("glCompileShader");
	glCreateProgram = (glCreateProgramPROC) extgl_GetProcAddress("glCreateProgram");
	glCreateShader = (glCreateShaderPROC) extgl_GetProcAddress("glCreateShader");
	glDeleteProgram = (glDeleteProgramPROC) extgl_GetProcAddress("glDeleteProgram");
	glDeleteShader = (glDeleteShaderPROC) extgl_GetProcAddress("glDeleteShader");
	glDetachShader = (glDetachShaderPROC) extgl_GetProcAddress("glDetachShader");
	glDisableVertexAttribArray = (glDisableVertexAttribArrayPROC) extgl_GetProcAddress("glDisableVertexAttribArray");
	glEnableVertexAttribArray = (glEnableVertexAttribArrayPROC) extgl_GetProcAddress("glEnableVertexAttribArray");
	glGetActiveAttrib = (glGetActiveAttribPROC) extgl_GetProcAddress("glGetActiveAttrib");
	glGetActiveUniform = (glGetActiveUniformPROC) extgl_GetProcAddress("glGetActiveUniform");
	glGetAttachedShaders = (glGetAttachedShadersPROC) extgl_GetProcAddress("glGetAttachedShaders");
	glGetAttribLocation = (glGetAttribLocationPROC) extgl_GetProcAddress("glGetAttribLocation");
	glGetProgramiv = (glGetProgramivPROC) extgl_GetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (glGetProgramInfoLogPROC) extgl_GetProcAddress("glGetProgramInfoLog");
	glGetShaderiv = (glGetShaderivPROC) extgl_GetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (glGetShaderInfoLogPROC) extgl_GetProcAddress("glGetShaderInfoLog");
	glGetShaderSource = (glGetShaderSourcePROC) extgl_GetProcAddress("glGetShaderSource");
	glGetUniformLocation = (glGetUniformLocationPROC) extgl_GetProcAddress("glGetUniformLocation");
	glGetUniformfv = (glGetUniformfvPROC) extgl_GetProcAddress("glGetUniformfv");
	glGetUniformiv = (glGetUniformivPROC) extgl_GetProcAddress("glGetUniformiv");
	glGetVertexAttribdv = (glGetVertexAttribdvPROC) extgl_GetProcAddress("glGetVertexAttribdv");
	glGetVertexAttribfv = (glGetVertexAttribfvPROC) extgl_GetProcAddress("glGetVertexAttribfv");
	glGetVertexAttribiv = (glGetVertexAttribivPROC) extgl_GetProcAddress("glGetVertexAttribiv");
	glGetVertexAttribPointerv = (glGetVertexAttribPointervPROC) extgl_GetProcAddress("glGetVertexAttribPointerv");
	glIsProgram = (glIsProgramPROC) extgl_GetProcAddress("glIsProgram");
	glIsShader = (glIsShaderPROC) extgl_GetProcAddress("glIsShader");
	glLinkProgram = (glLinkProgramPROC) extgl_GetProcAddress("glLinkProgram");
	glShaderSource = (glShaderSourcePROC) extgl_GetProcAddress("glShaderSource");
	glUseProgram = (glUseProgramPROC) extgl_GetProcAddress("glUseProgram");
	glUniform1f = (glUniform1fPROC) extgl_GetProcAddress("glUniform1f");
	glUniform2f = (glUniform2fPROC) extgl_GetProcAddress("glUniform2f");
	glUniform3f = (glUniform3fPROC) extgl_GetProcAddress("glUniform3f");
	glUniform4f = (glUniform4fPROC) extgl_GetProcAddress("glUniform4f");
	glUniform1i = (glUniform1iPROC) extgl_GetProcAddress("glUniform1i");
	glUniform2i = (glUniform2iPROC) extgl_GetProcAddress("glUniform2i");
	glUniform3i = (glUniform3iPROC) extgl_GetProcAddress("glUniform3i");
	glUniform4i = (glUniform4iPROC) extgl_GetProcAddress("glUniform4i");
	glUniform1fv = (glUniform1fvPROC) extgl_GetProcAddress("glUniform1fv");
	glUniform2fv = (glUniform2fvPROC) extgl_GetProcAddress("glUniform2fv");
	glUniform3fv = (glUniform3fvPROC) extgl_GetProcAddress("glUniform3fv");
	glUniform4fv = (glUniform4fvPROC) extgl_GetProcAddress("glUniform4fv");
	glUniform1iv = (glUniform1ivPROC) extgl_GetProcAddress("glUniform1iv");
	glUniform2iv = (glUniform2ivPROC) extgl_GetProcAddress("glUniform2iv");
	glUniform3iv = (glUniform3ivPROC) extgl_GetProcAddress("glUniform3iv");
	glUniform4iv = (glUniform4ivPROC) extgl_GetProcAddress("glUniform4iv");
	glUniformMatrix2fv = (glUniformMatrix2fvPROC) extgl_GetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (glUniformMatrix3fvPROC) extgl_GetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (glUniformMatrix4fvPROC) extgl_GetProcAddress("glUniformMatrix4fv");
	glValidateProgram = (glValidateProgramPROC) extgl_GetProcAddress("glValidateProgram");
	glVertexAttrib1d = (glVertexAttrib1dPROC) extgl_GetProcAddress("glVertexAttrib1d");
	glVertexAttrib1dv = (glVertexAttrib1dvPROC) extgl_GetProcAddress("glVertexAttrib1dv");
	glVertexAttrib1f = (glVertexAttrib1fPROC) extgl_GetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (glVertexAttrib1fvPROC) extgl_GetProcAddress("glVertexAttrib1fv");
	glVertexAttrib1s = (glVertexAttrib1sPROC) extgl_GetProcAddress("glVertexAttrib1s");
	glVertexAttrib1sv = (glVertexAttrib1svPROC) extgl_GetProcAddress("glVertexAttrib1sv");
	glVertexAttrib2d = (glVertexAttrib2dPROC) extgl_GetProcAddress("glVertexAttrib2d");
	glVertexAttrib2dv = (glVertexAttrib2dvPROC) extgl_GetProcAddress("glVertexAttrib2dv");
	glVertexAttrib2f = (glVertexAttrib2fPROC) extgl_GetProcAddress("glVertexAttrib2f");
	glVertexAttrib2fv = (glVertexAttrib2fvPROC) extgl_GetProcAddress("glVertexAttrib2fv");
	glVertexAttrib2s = (glVertexAttrib2sPROC) extgl_GetProcAddress("glVertexAttrib2s");
	glVertexAttrib2sv = (glVertexAttrib2svPROC) extgl_GetProcAddress("glVertexAttrib2sv");
	glVertexAttrib3d = (glVertexAttrib3dPROC) extgl_GetProcAddress("glVertexAttrib3d");
	glVertexAttrib3dv = (glVertexAttrib3dvPROC) extgl_GetProcAddress("glVertexAttrib3dv");
	glVertexAttrib3f = (glVertexAttrib3fPROC) extgl_GetProcAddress("glVertexAttrib3f");
	glVertexAttrib3fv = (glVertexAttrib3fvPROC) extgl_GetProcAddress("glVertexAttrib3fv");
	glVertexAttrib3s = (glVertexAttrib3sPROC) extgl_GetProcAddress("glVertexAttrib3s");
	glVertexAttrib3sv = (glVertexAttrib3svPROC) extgl_GetProcAddress("glVertexAttrib3sv");
	glVertexAttrib4Nbv = (glVertexAttrib4NbvPROC) extgl_GetProcAddress("glVertexAttrib4Nbv");
	glVertexAttrib4Niv = (glVertexAttrib4NivPROC) extgl_GetProcAddress("glVertexAttrib4Niv");
	glVertexAttrib4Nsv = (glVertexAttrib4NsvPROC) extgl_GetProcAddress("glVertexAttrib4Nsv");
	glVertexAttrib4Nub = (glVertexAttrib4NubPROC) extgl_GetProcAddress("glVertexAttrib4Nub");
	glVertexAttrib4Nubv = (glVertexAttrib4NubvPROC) extgl_GetProcAddress("glVertexAttrib4Nubv");
	glVertexAttrib4Nuiv = (glVertexAttrib4NuivPROC) extgl_GetProcAddress("glVertexAttrib4Nuiv");
	glVertexAttrib4Nusv = (glVertexAttrib4NusvPROC) extgl_GetProcAddress("glVertexAttrib4Nusv");
	glVertexAttrib4bv = (glVertexAttrib4bvPROC) extgl_GetProcAddress("glVertexAttrib4bv");
	glVertexAttrib4d = (glVertexAttrib4dPROC) extgl_GetProcAddress("glVertexAttrib4d");
	glVertexAttrib4dv = (glVertexAttrib4dvPROC) extgl_GetProcAddress("glVertexAttrib4dv");
	glVertexAttrib4f = (glVertexAttrib4fPROC) extgl_GetProcAddress("glVertexAttrib4f");
	glVertexAttrib4fv = (glVertexAttrib4fvPROC) extgl_GetProcAddress("glVertexAttrib4fv");
	glVertexAttrib4iv = (glVertexAttrib4ivPROC) extgl_GetProcAddress("glVertexAttrib4iv");
	glVertexAttrib4s = (glVertexAttrib4sPROC) extgl_GetProcAddress("glVertexAttrib4s");
	glVertexAttrib4sv = (glVertexAttrib4svPROC) extgl_GetProcAddress("glVertexAttrib4sv");
	glVertexAttrib4ubv = (glVertexAttrib4ubvPROC) extgl_GetProcAddress("glVertexAttrib4ubv");
	glVertexAttrib4uiv = (glVertexAttrib4uivPROC) extgl_GetProcAddress("glVertexAttrib4uiv");
	glVertexAttrib4usv = (glVertexAttrib4usvPROC) extgl_GetProcAddress("glVertexAttrib4usv");
	glVertexAttribPointer = (glVertexAttribPointerPROC) extgl_GetProcAddress("glVertexAttribPointer");
#endif /* GL_VERSION_2_0 */
#endif /* WIN32 */
}

void extgl_InitPGIMiscHints()
{
#ifdef GL_PGI_misc_hints
    if (!extgl_Extensions.PGI_misc_hints)
        return;
    glHintPGI = (glHintPGIPROC) extgl_GetProcAddress("glHintPGI");
#endif
}


void extgl_InitSGIColorTable()
{
#ifdef GL_SGI_color_table
    if (!extgl_Extensions.SGI_color_table)
        return;
	glColorTableSGI = (glColorTableSGIPROC) extgl_GetProcAddress("glColorTableSGI");
	glColorTableParameterfvSGI = (glColorTableParameterfvSGIPROC) extgl_GetProcAddress("glColorTableParameterfvSGI");
	glColorTableParameterivSGI = (glColorTableParameterivSGIPROC) extgl_GetProcAddress("glColorTableParameterivSGI");
	glCopyColorTableSGI = (glCopyColorTableSGIPROC) extgl_GetProcAddress("glCopyColorTableSGI");
	glGetColorTableSGI = (glGetColorTableSGIPROC) extgl_GetProcAddress("glGetColorTableSGI");
	glGetColorTableParameterfvSGI = (glGetColorTableParameterfvSGIPROC) extgl_GetProcAddress("glGetColorTableParameterfvSGI");
	glGetColorTableParameterivSGI = (glGetColorTableParameterivSGIPROC) extgl_GetProcAddress("glGetColorTableParameterivSGI");
#endif
}

void extgl_InitSGISDetailTexture()
{
#ifdef GL_SGIS_detail_texture
    if (!extgl_Extensions.SGIS_detail_texture)
        return;
	glDetailTexFuncSGIS = (glDetailTexFuncSGISPROC) extgl_GetProcAddress("glDetailTexFuncSGIS");
	glGetDetailTexFuncSGIS = (glGetDetailTexFuncSGISPROC) extgl_GetProcAddress("glGetDetailTexFuncSGIS");
#endif
}

void extgl_InitSGISFogFunction()
{
#ifdef GL_SGIS_fog_function
    if (!extgl_Extensions.SGIS_fog_function)
        return;
	glFogFuncSGIS = (glFogFuncSGISPROC) extgl_GetProcAddress("glFogFuncSGIS");
	glGetFogFuncSGIS = (glGetFogFuncSGISPROC) extgl_GetProcAddress("glGetFogFuncSGIS");
#endif
}

void extgl_InitSGISMultisample()
{
#ifdef GL_SGIS_multisample
    if (!extgl_Extensions.SGIS_multisample)
        return;
	glSampleMaskSGIS = (glSampleMaskSGISPROC) extgl_GetProcAddress("glSampleMaskSGIS");
	glSamplePatternSGIS = (glSamplePatternSGISPROC) extgl_GetProcAddress("glSamplePatternSGIS");
#endif
}

void extgl_InitSGISMultitexture()
{
#ifdef GL_SGIS_multitexture
    if (!extgl_Extensions.SGIS_multitexture)
        return;
	glSelectTextureSGIS = (glSelectTextureSGISPROC) extgl_GetProcAddress("glSelectTextureSGIS");
	glSelectTextureTransformSGIS = (glSelectTextureTransformSGISPROC) extgl_GetProcAddress("glSelectTextureTransformSGIS");
	glSelectTextureCoordsetSGIS = (glSelectTextureCoordsetSGISPROC) extgl_GetProcAddress("glSelectTextureCoordsetSGIS");
	glMultitexCoord1dSGIS = (glMultitexCoord1dSGISPROC) extgl_GetProcAddress("glMultitexCoord1dSGIS");
	glMultitexCoord1dvSGIS = (glMultitexCoord1dvSGISPROC) extgl_GetProcAddress("glMultitexCoord1dvSGIS");
	glMultitexCoord1fSGIS = (glMultitexCoord1fSGISPROC) extgl_GetProcAddress("glMultitexCoord1fSGIS");
	glMultitexCoord1fvSGIS = (glMultitexCoord1fvSGISPROC) extgl_GetProcAddress("glMultitexCoord1fvSGIS");
	glMultitexCoord1iSGIS = (glMultitexCoord1iSGISPROC) extgl_GetProcAddress("glMultitexCoord1iSGIS");
	glMultitexCoord1ivSGIS = (glMultitexCoord1ivSGISPROC) extgl_GetProcAddress("glMultitexCoord1ivSGIS");
	glMultitexCoord1sSGIS = (glMultitexCoord1sSGISPROC) extgl_GetProcAddress("glMultitexCoord1sSGIS");
	glMultitexCoord1svSGIS = (glMultitexCoord1svSGISPROC) extgl_GetProcAddress("glMultitexCoord1svSGIS");
	glMultitexCoord2dSGIS = (glMultitexCoord2dSGISPROC) extgl_GetProcAddress("glMultitexCoord2dSGIS");
	glMultitexCoord2dvSGIS = (glMultitexCoord2dvSGISPROC) extgl_GetProcAddress("glMultitexCoord2dvSGIS");
	glMultitexCoord2fSGIS = (glMultitexCoord2fSGISPROC) extgl_GetProcAddress("glMultitexCoord2fSGIS");
	glMultitexCoord2fvSGIS = (glMultitexCoord2fvSGISPROC) extgl_GetProcAddress("glMultitexCoord2fvSGIS");
	glMultitexCoord2iSGIS = (glMultitexCoord2iSGISPROC) extgl_GetProcAddress("glMultitexCoord2iSGIS");
	glMultitexCoord2ivSGIS = (glMultitexCoord2ivSGISPROC) extgl_GetProcAddress("glMultitexCoord2ivSGIS");
	glMultitexCoord2sSGIS = (glMultitexCoord2sSGISPROC) extgl_GetProcAddress("glMultitexCoord2sSGIS");
	glMultitexCoord2svSGIS = (glMultitexCoord2svSGISPROC) extgl_GetProcAddress("glMultitexCoord2svSGIS");
	glMultitexCoord3dSGIS = (glMultitexCoord3dSGISPROC) extgl_GetProcAddress("glMultitexCoord3dSGIS");
	glMultitexCoord3dvSGIS = (glMultitexCoord3dvSGISPROC) extgl_GetProcAddress("glMultitexCoord3dvSGIS");
	glMultitexCoord3fSGIS = (glMultitexCoord3fSGISPROC) extgl_GetProcAddress("glMultitexCoord3fSGIS");
	glMultitexCoord3fvSGIS = (glMultitexCoord3fvSGISPROC) extgl_GetProcAddress("glMultitexCoord3fvSGIS");
	glMultitexCoord3iSGIS = (glMultitexCoord3iSGISPROC) extgl_GetProcAddress("glMultitexCoord3iSGIS");
	glMultitexCoord3ivSGIS = (glMultitexCoord3ivSGISPROC) extgl_GetProcAddress("glMultitexCoord3ivSGIS");
	glMultitexCoord3sSGIS = (glMultitexCoord3sSGISPROC) extgl_GetProcAddress("glMultitexCoord3sSGIS");
	glMultitexCoord3svSGIS = (glMultitexCoord3svSGISPROC) extgl_GetProcAddress("glMultitexCoord3svSGIS");
	glMultitexCoord4dSGIS = (glMultitexCoord4dSGISPROC) extgl_GetProcAddress("glMultitexCoord4dSGIS");
	glMultitexCoord4dvSGIS = (glMultitexCoord4dvSGISPROC) extgl_GetProcAddress("glMultitexCoord4dvSGIS");
	glMultitexCoord4fSGIS = (glMultitexCoord4fSGISPROC) extgl_GetProcAddress("glMultitexCoord4fSGIS");
	glMultitexCoord4fvSGIS = (glMultitexCoord4fvSGISPROC) extgl_GetProcAddress("glMultitexCoord4fvSGIS");
	glMultitexCoord4iSGIS = (glMultitexCoord4iSGISPROC) extgl_GetProcAddress("glMultitexCoord4iSGIS");
	glMultitexCoord4ivSGIS = (glMultitexCoord4ivSGISPROC) extgl_GetProcAddress("glMultitexCoord4ivSGIS");
	glMultitexCoord4sSGIS = (glMultitexCoord4sSGISPROC) extgl_GetProcAddress("glMultitexCoord4sSGIS");
	glMultitexCoord4svSGIS = (glMultitexCoord4svSGISPROC) extgl_GetProcAddress("glMultitexCoord4svSGIS");
#endif
}

void extgl_InitSGISPixelTexture()
{
#ifdef GL_SGIS_pixel_texture
    if (!extgl_Extensions.SGIS_pixel_texture)
        return;
	glPixelTexGenParameteriSGIS = (glPixelTexGenParameteriSGISPROC) extgl_GetProcAddress("glPixelTexGenParameteriSGIS");
	glPixelTexGenParameterivSGIS = (glPixelTexGenParameterivSGISPROC) extgl_GetProcAddress("glPixelTexGenParameterivSGIS");
	glPixelTexGenParameterfSGIS = (glPixelTexGenParameterfSGISPROC) extgl_GetProcAddress("glPixelTexGenParameterfSGIS");
	glPixelTexGenParameterfvSGIS = (glPixelTexGenParameterfvSGISPROC) extgl_GetProcAddress("glPixelTexGenParameterfvSGIS");
	glGetPixelTexGenParameterivSGIS = (glGetPixelTexGenParameterivSGISPROC) extgl_GetProcAddress("glGetPixelTexGenParameterivSGIS");
	glGetPixelTexGenParameterfvSGIS = (glGetPixelTexGenParameterfvSGISPROC) extgl_GetProcAddress("glGetPixelTexGenParameterfvSGIS");
#endif
}

void extgl_InitSGISPointParameters()
{
#ifdef GL_SGIS_point_parameters
    if (!extgl_Extensions.SGIS_point_parameters)
        return;
	glPointParameterfSGIS = (glPointParameterfSGISPROC) extgl_GetProcAddress("glPointParameterfSGIS");
	glPointParameterfvSGIS = (glPointParameterfvSGISPROC) extgl_GetProcAddress("glPointParameterfvSGIS");
#endif
}

void extgl_InitSGISSharpenTexture()
{
#ifdef GL_SGIS_sharpen_texture
    if (!extgl_Extensions.SGIS_sharpen_texture)
        return;
	glSharpenTexFuncSGIS = (glSharpenTexFuncSGISPROC) extgl_GetProcAddress("glSharpenTexFuncSGIS");
	glGetSharpenTexFuncSGIS = (glGetSharpenTexFuncSGISPROC) extgl_GetProcAddress("glGetSharpenTexFuncSGIS");
#endif
}

void extgl_InitSGISTexture4D()
{
#ifdef GL_SGIS_texture4D
    if (!extgl_Extensions.SGIS_texture4D)
        return;
	glTexImage4DSGIS = (glTexImage4DSGISPROC) extgl_GetProcAddress("glTexImage4DSGIS");
	glTexSubImage4DSGIS = (glTexSubImage4DSGISPROC) extgl_GetProcAddress("glTexSubImage4DSGIS");
#endif
}

void extgl_InitSGISTextureColorMask()
{
#ifdef GL_SGIS_texture_color_mask
    if (!extgl_Extensions.SGIS_texture_color_mask)
        return;
	glTextureColorMaskSGIS = (glTextureColorMaskSGISPROC) extgl_GetProcAddress("glTextureColorMaskSGIS");
#endif
}

void extgl_InitSGISTextureFilter4()
{
#ifdef GL_SGIS_texture_filter4
    if (!extgl_Extensions.SGIS_texture_filter4)
        return;
    glGetTexFilterFuncSGIS = (glGetTexFilterFuncSGISPROC) extgl_GetProcAddress("glGetTexFilterFuncSGIS");
    glTexFilterFuncSGIS = (glTexFilterFuncSGISPROC) extgl_GetProcAddress("glTexFilterFuncSGIS");
#endif
}

void extgl_InitSGIXAsync()
{
#ifdef GL_SGIX_async
    if (!extgl_Extensions.SGIX_asyncr)
        return;
	glAsyncMarkerSGIX = (glAsyncMarkerSGIXPROC) extgl_GetProcAddress("glAsyncMarkerSGIX");
	glFinishAsyncSGIX = (glFinishAsyncSGIXPROC) extgl_GetProcAddress("glFinishAsyncSGIX");
	glPollAsyncSGIX = (glPollAsyncSGIXPROC) extgl_GetProcAddress("glPollAsyncSGIX");
	glGenAsyncMarkersSGIX = (glGenAsyncMarkersSGIXPROC) extgl_GetProcAddress("glGenAsyncMarkersSGIX");
	glDeleteAsyncMarkersSGIX = (glDeleteAsyncMarkersSGIXPROC) extgl_GetProcAddress("glDeleteAsyncMarkersSGIX");
	glIsAsyncMarkerSGIX = (glIsAsyncMarkerSGIXPROC) extgl_GetProcAddress("glIsAsyncMarkerSGIX");
#endif
}

void extgl_InitSGIXFlushRaster()
{
#ifdef GL_SGIX_flush_raster
    if (!extgl_Extensions.SGIX_flush_raster)
        return;
    glFlushRasterSGIX = (glFlushRasterSGIXPROC) extgl_GetProcAddress("glFlushRasterSGIX");
#endif
}

void extgl_InitSGIXFragmentLighting()
{
#ifdef GL_SGIX_fragment_lighting
    if (!extgl_Extensions.SGIX_fragment_lighting)
        return;
	glFragmentColorMaterialSGIX = (glFragmentColorMaterialSGIXPROC) extgl_GetProcAddress("glFragmentColorMaterialSGIX");
	glFragmentLightfSGIX = (glFragmentLightfSGIXPROC) extgl_GetProcAddress("glFragmentLightfSGIX");
	glFragmentLightfvSGIX = (glFragmentLightfvSGIXPROC) extgl_GetProcAddress("glFragmentLightfvSGIX");
	glFragmentLightiSGIX = (glFragmentLightiSGIXPROC) extgl_GetProcAddress("glFragmentLightiSGIX");
	glFragmentLightivSGIX = (glFragmentLightivSGIXPROC) extgl_GetProcAddress("glFragmentLightivSGIX");
	glFragmentLightModelfSGIX = (glFragmentLightModelfSGIXPROC) extgl_GetProcAddress("glFragmentLightModelfSGIX");
	glFragmentLightModelfvSGIX = (glFragmentLightModelfvSGIXPROC) extgl_GetProcAddress("glFragmentLightModelfvSGIX");
	glFragmentLightModeliSGIX = (glFragmentLightModeliSGIXPROC) extgl_GetProcAddress("glFragmentLightModeliSGIX");
	glFragmentLightModelivSGIX = (glFragmentLightModelivSGIXPROC) extgl_GetProcAddress("glFragmentLightModelivSGIX");
	glFragmentMaterialfSGIX = (glFragmentMaterialfSGIXPROC) extgl_GetProcAddress("glFragmentMaterialfSGIX");
	glFragmentMaterialfvSGIX = (glFragmentMaterialfvSGIXPROC) extgl_GetProcAddress("glFragmentMaterialfvSGIX");
	glFragmentMaterialiSGIX = (glFragmentMaterialiSGIXPROC) extgl_GetProcAddress("glFragmentMaterialiSGIX");
	glFragmentMaterialivSGIX = (glFragmentMaterialivSGIXPROC) extgl_GetProcAddress("glFragmentMaterialivSGIX");
	glGetFragmentLightfvSGIX = (glGetFragmentLightfvSGIXPROC) extgl_GetProcAddress("glGetFragmentLightfvSGIX");
	glGetFragmentLightivSGIX = (glGetFragmentLightivSGIXPROC) extgl_GetProcAddress("glGetFragmentLightivSGIX");
	glGetFragmentMaterialfvSGIX = (glGetFragmentMaterialfvSGIXPROC) extgl_GetProcAddress("glGetFragmentMaterialfvSGIX");
	glGetFragmentMaterialivSGIX = (glGetFragmentMaterialivSGIXPROC) extgl_GetProcAddress("glGetFragmentMaterialivSGIX");
	glLightEnviSGIX = (glLightEnviSGIXPROC) extgl_GetProcAddress("glLightEnviSGIX");
#endif
}

void extgl_InitSGIXFramezoom()
{
#ifdef GL_SGIX_framezoom
    if (!extgl_Extensions.SGIX_framezoom)
        return;
    glFrameZoomSGIX = (glFrameZoomSGIXPROC) extgl_GetProcAddress("glFrameZoomSGIX");
#endif
}

void extgl_InitSGIXInstruments()
{
#ifdef GL_SGIX_instruments
    if (!extgl_Extensions.SGIX_instruments)
        return;
	glGetInstrumentsSGIX = (glGetInstrumentsSGIXPROC) extgl_GetProcAddress("glGetInstrumentsSGIX");
	glInstrumentsBufferSGIX = (glInstrumentsBufferSGIXPROC) extgl_GetProcAddress("glInstrumentsBufferSGIX");
	glPollInstrumentsSGIX = (glPollInstrumentsSGIXPROC) extgl_GetProcAddress("glPollInstrumentsSGIX");
	glReadInstrumentsSGIX = (glReadInstrumentsSGIXPROC) extgl_GetProcAddress("glReadInstrumentsSGIX");
	glStartInstrumentsSGIX = (glStartInstrumentsSGIXPROC) extgl_GetProcAddress("glStartInstrumentsSGIX");
	glStopInstrumentsSGIX = (glStopInstrumentsSGIXPROC) extgl_GetProcAddress("glStopInstrumentsSGIX");
#endif
}

void extgl_InitSGIXListPriority()
{
#ifdef GL_SGIX_list_priority
    if (!extgl_Extensions.SGIX_list_priority)
        return;
	glGetListParameterfvSGIX = (glGetListParameterfvSGIXPROC) extgl_GetProcAddress("glGetListParameterfvSGIX");
	glGetListParameterivSGIX = (glGetListParameterivSGIXPROC) extgl_GetProcAddress("glGetListParameterivSGIX");
	glListParameterfSGIX = (glListParameterfSGIXPROC) extgl_GetProcAddress("glListParameterfSGIX");
	glListParameterfvSGIX = (glListParameterfvSGIXPROC) extgl_GetProcAddress("glListParameterfvSGIX");
	glListParameteriSGIX = (glListParameteriSGIXPROC) extgl_GetProcAddress("glListParameteriSGIX");
	glListParameterivSGIX = (glListParameterivSGIXPROC) extgl_GetProcAddress("glListParameterivSGIX");
#endif
}

void extgl_InitSGIXPixelTexture()
{
#ifdef GL_SGIX_pixel_texture
    if (!extgl_Extensions.SGIX_pixel_texture)
        return;
    glPixelTexGenSGIX = (glPixelTexGenSGIXPROC) extgl_GetProcAddress("glPixelTexGenSGIX");
#endif
}

void extgl_InitSGIXPolynomialFfd()
{
#ifdef GL_SGIX_polynomial_ffd
    if (!extgl_Extensions.SGIX_polynomial_ffd)
        return;
	glDeformationMap3dSGIX = (glDeformationMap3dSGIXPROC) extgl_GetProcAddress("glDeformationMap3dSGIX");
	glDeformationMap3fSGIX = (glDeformationMap3fSGIXPROC) extgl_GetProcAddress("glDeformationMap3fSGIX");
	glDeformSGIX = (glDeformSGIXPROC) extgl_GetProcAddress("glDeformSGIX");
	glLoadIdentityDeformationMapSGIX = (glLoadIdentityDeformationMapSGIXPROC) extgl_GetProcAddress("glLoadIdentityDeformationMapSGIX");
#endif
}

void extgl_InitSGIXReferencePlane()
{
#ifdef GL_SGIX_reference_plane
    if (!extgl_Extensions.SGIX_reference_plane)
        return;
	glReferencePlaneSGIX = (glReferencePlaneSGIXPROC) extgl_GetProcAddress("glReferencePlaneSGIX");
#endif
}

void extgl_InitSGIXSprite()
{
#ifdef GL_SGIX_sprite
    if (!extgl_Extensions.SGIX_sprite)
        return;
	glSpriteParameterfSGIX = (glSpriteParameterfSGIXPROC) extgl_GetProcAddress("glSpriteParameterfSGIX");
	glSpriteParameterfvSGIX = (glSpriteParameterfvSGIXPROC) extgl_GetProcAddress("glSpriteParameterfvSGIX");
	glSpriteParameteriSGIX = (glSpriteParameteriSGIXPROC) extgl_GetProcAddress("glSpriteParameteriSGIX");
	glSpriteParameterivSGIX = (glSpriteParameterivSGIXPROC) extgl_GetProcAddress("glSpriteParameterivSGIX");
#endif
}

void extgl_InitSGIXTagSampleBuffer()
{
#ifdef GL_SGIX_tag_sample_buffer
    if (!extgl_Extensions.SGIX_tag_sample_buffer)
        return;
	glTagSampleBufferSGIX = (glTagSampleBufferSGIXPROC) extgl_GetProcAddress("glTagSampleBufferSGIX");
#endif
}

void extgl_InitSUNGlobalAlpha()
{
#ifdef GL_SUN_global_alpha
    if (!extgl_Extensions.SUN_global_alpha)
        return;
	glGlobalAlphaFactorbSUN = (glGlobalAlphaFactorbSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactorbSUN");
	glGlobalAlphaFactorsSUN = (glGlobalAlphaFactorsSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactorsSUN");
	glGlobalAlphaFactoriSUN = (glGlobalAlphaFactoriSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactoriSUN");
	glGlobalAlphaFactorfSUN = (glGlobalAlphaFactorfSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactorfSUN");
	glGlobalAlphaFactordSUN = (glGlobalAlphaFactordSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactordSUN");
	glGlobalAlphaFactorubSUN = (glGlobalAlphaFactorubSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactorubSUN");
	glGlobalAlphaFactorusSUN = (glGlobalAlphaFactorusSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactorusSUN");
	glGlobalAlphaFactoruiSUN = (glGlobalAlphaFactoruiSUNPROC) extgl_GetProcAddress("glGlobalAlphaFactoruiSUN");
#endif
}

void extgl_InitSUNMeshArray()
{
#ifdef GL_SUN_mesh_array
    if (!extgl_Extensions.SUN_mesh_array)
        return;
	glDrawMeshArraysSUN = (glDrawMeshArraysSUNPROC) extgl_GetProcAddress("glDrawMeshArraysSUN");
#endif
}

void extgl_InitSUNMultiDrawArrays()
{
#ifdef GL_SUN_multi_draw_arrays
    if (!extgl_Extensions.SUN_multi_draw_arrays)
        return;
    glMultidrawArraysSUN = (glMultidrawArraysSUNPROC) extgl_GetProcAddress("glMultidrawArraysSUN");
    glMultidrawElementsSUN = (glMultidrawElementsSUNPROC) extgl_GetProcAddress("glMultidrawElementsSUN");
#endif
}

void extgl_InitSUNTriangleList()
{
#ifdef GL_SUN_triangle_list
    if (!extgl_Extensions.SUN_triangle_list)
        return;
	glReplacementCodeuiSUN = (glReplacementCodeuiSUNPROC) extgl_GetProcAddress("glReplacementCodeuiSUN");
	glReplacementCodeusSUN = (glReplacementCodeusSUNPROC) extgl_GetProcAddress("glReplacementCodeusSUN");
	glReplacementCodeubSUN = (glReplacementCodeubSUNPROC) extgl_GetProcAddress("glReplacementCodeubSUN");
	glReplacementCodeuivSUN = (glReplacementCodeuivSUNPROC) extgl_GetProcAddress("glReplacementCodeuivSUN");
	glReplacementCodeusvSUN = (glReplacementCodeusvSUNPROC) extgl_GetProcAddress("glReplacementCodeusvSUN");
	glReplacementCodeubvSUN = (glReplacementCodeubvSUNPROC) extgl_GetProcAddress("glReplacementCodeubvSUN");
	glReplacementCodePointerSUN = (glReplacementCodePointerSUNPROC) extgl_GetProcAddress("glReplacementCodePointerSUN");
#endif
}

void extgl_InitSUNVertex()
{
#ifdef GL_SUN_vertex
    if (!extgl_Extensions.SUN_vertex)
        return;
	glColor4ubVertex2fSUN = (glColor4ubVertex2fSUNPROC) extgl_GetProcAddress("glColor4ubVertex2fSUN");
	glColor4ubVertex2fvSUN = (glColor4ubVertex2fvSUNPROC) extgl_GetProcAddress("glColor4ubVertex2fvSUN");
	glColor4ubVertex3fSUN = (glColor4ubVertex3fSUNPROC) extgl_GetProcAddress("glColor4ubVertex3fSUN");
	glColor4ubVertex3fvSUN = (glColor4ubVertex3fvSUNPROC) extgl_GetProcAddress("glColor4ubVertex3fvSUN");
	glColor3fVertex3fSUN = (glColor3fVertex3fSUNPROC) extgl_GetProcAddress("glColor3fVertex3fSUN");
	glColor3fVertex3fvSUN = (glColor3fVertex3fvSUNPROC) extgl_GetProcAddress("glColor3fVertex3fvSUN");
	glNormal3fVertex3fSUN = (glNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glNormal3fVertex3fSUN");
	glNormal3fVertex3fvSUN = (glNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glNormal3fVertex3fvSUN");
	glColor4fNormal3fVertex3fSUN = (glColor4fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glColor4fNormal3fVertex3fSUN");
	glColor4fNormal3fVertex3fvSUN = (glColor4fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glColor4fNormal3fVertex3fvSUN");
	glTexCoord2fVertex3fSUN = (glTexCoord2fVertex3fSUNPROC) extgl_GetProcAddress("glTexCoord2fVertex3fSUN");
	glTexCoord2fVertex3fvSUN = (glTexCoord2fVertex3fvSUNPROC) extgl_GetProcAddress("glTexCoord2fVertex3fvSUN");
	glTexCoord4fVertex4fSUN = (glTexCoord4fVertex4fSUNPROC) extgl_GetProcAddress("glTexCoord4fVertex4fSUN");
	glTexCoord4fVertex4fvSUN = (glTexCoord4fVertex4fvSUNPROC) extgl_GetProcAddress("glTexCoord4fVertex4fvSUN");
	glTexCoord2fColor4ubVertex3fSUN = (glTexCoord2fColor4ubVertex3fSUNPROC) extgl_GetProcAddress("glTexCoord2fColor4ubVertex3fSUN");
	glTexCoord2fColor4ubVertex3fvSUN = (glTexCoord2fColor4ubVertex3fvSUNPROC) extgl_GetProcAddress("glTexCoord2fColor4ubVertex3fvSUN");
	glTexCoord2fColor3fVertex3fSUN = (glTexCoord2fColor3fVertex3fSUNPROC) extgl_GetProcAddress("glTexCoord2fColor3fVertex3fSUN");
	glTexCoord2fColor3fVertex3fvSUN = (glTexCoord2fColor3fVertex3fvSUNPROC) extgl_GetProcAddress("glTexCoord2fColor3fVertex3fvSUN");
	glTexCoord2fNormal3fVertex3fSUN = (glTexCoord2fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glTexCoord2fNormal3fVertex3fSUN");
	glTexCoord2fNormal3fVertex3fvSUN = (glTexCoord2fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glTexCoord2fNormal3fVertex3fvSUN");
	glTexCoord2fColor4fNormal3fVertex3fSUN = (glTexCoord2fColor4fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glTexCoord2fColor4fNormal3fVertex3fSUN");
	glTexCoord2fColor4fNormal3fVertex3fvSUN = (glTexCoord2fColor4fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glTexCoord2fColor4fNormal3fVertex3fvSUN");
	glTexCoord4fColor4fNormal3fVertex4fSUN = (glTexCoord4fColor4fNormal3fVertex4fSUNPROC) extgl_GetProcAddress("glTexCoord4fColor4fNormal3fVertex4fSUN");
	glTexCoord4fColor4fNormal3fVertex4fvSUN = (glTexCoord4fColor4fNormal3fVertex4fvSUNPROC) extgl_GetProcAddress("glTexCoord4fColor4fNormal3fVertex4fvSUN");
	glReplacementCodeuiVertex3fSUN = (glReplacementCodeuiVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiVertex3fSUN");
	glReplacementCodeuiVertex3fvSUN = (glReplacementCodeuiVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiVertex3fvSUN");
	glReplacementCodeuiColor4ubVertex3fSUN = (glReplacementCodeuiColor4ubVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor4ubVertex3fSUN");
	glReplacementCodeuiColor4ubVertex3fvSUN = (glReplacementCodeuiColor4ubVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor4ubVertex3fvSUN");
	glReplacementCodeuiColor3fVertex3fSUN = (glReplacementCodeuiColor3fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor3fVertex3fSUN");
	glReplacementCodeuiColor3fVertex3fvSUN = (glReplacementCodeuiColor3fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor3fVertex3fvSUN");
	glReplacementCodeuiNormal3fVertex3fSUN = (glReplacementCodeuiNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiNormal3fVertex3fSUN");
	glReplacementCodeuiNormal3fVertex3fvSUN = (glReplacementCodeuiNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiNormal3fVertex3fvSUN");
	glReplacementCodeuiColor4fNormal3fVertex3fSUN = (glReplacementCodeuiColor4fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor4fNormal3fVertex3fSUN");
	glReplacementCodeuiColor4fNormal3fVertex3fvSUN = (glReplacementCodeuiColor4fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiColor4fNormal3fVertex3fvSUN");
	glReplacementCodeuiTexCoord2fVertex3fSUN = (glReplacementCodeuiTexCoord2fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fVertex3fSUN");
	glReplacementCodeuiTexCoord2fVertex3fvSUN = (glReplacementCodeuiTexCoord2fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fVertex3fvSUN");
	glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = (glReplacementCodeuiTexCoord2fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN");
	glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = (glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN");
	glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = (glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN");
	glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = (glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUNPROC) extgl_GetProcAddress("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN");
#endif
}

void extgl_InitSUNXConstantData()
{
#ifdef GL_SUNX_constant_data
    if (!extgl_Extensions.SUNX_constant_data)
        return;
    glFinishTextureSUNX = (glFinishTextureSUNXPROC) extgl_GetProcAddress("glFinishTextureSUNX");
#endif
}

void extgl_InitSupportedExtensions()
{
	g_extensions = glGetString(GL_EXTENSIONS);

    char *s = (char*) glGetString(GL_VERSION);
    extgl_ExtString[0] = 0;
    if (!s)
        return;
	extgl_Extensions.OpenGL12 = 0;
	extgl_Extensions.OpenGL13 = 0;
	extgl_Extensions.OpenGL14 = 0;
	extgl_Extensions.OpenGL15 = 0;
	extgl_Extensions.OpenGL20 = 0;

	//s = strstr(s, "2.");
	//if (s != NULL)
	if ( (s[0] >= '2') || (s[1] != '.') ) // If version >= 2.0 or '.' is after the second pos (>= 10.0)
	{
		extgl_Extensions.OpenGL12 = 1;
		extgl_Extensions.OpenGL13 = 1;
		extgl_Extensions.OpenGL14 = 1;
		extgl_Extensions.OpenGL15 = 1;
		extgl_Extensions.OpenGL20 = 1;
	}
	else
	{
		//char *s = (char*) glGetString(GL_VERSION);
		//s = strstr(s, "1.");
		//if (s != NULL)
		if ( (s[0] == '1') && (s[1] == '.') ) // If version >= 1.0
		{
			if ( s[2] >= '5' )
			{
				extgl_Extensions.OpenGL12 = 1;
				extgl_Extensions.OpenGL13 = 1;
				extgl_Extensions.OpenGL14 = 1;
				extgl_Extensions.OpenGL15 = 1;
			} else
			if( s[2] == '4' )
			{
				extgl_Extensions.OpenGL12 = 1;
				extgl_Extensions.OpenGL13 = 1;
				extgl_Extensions.OpenGL14 = 1;
			} else
			if( s[2] == '3' )
			{
				extgl_Extensions.OpenGL12 = 1;
				extgl_Extensions.OpenGL13 = 1;
			}  else
			if( s[2] == '2' )
			{
				extgl_Extensions.OpenGL12 = 1;
			}
		}
	}
	extgl_Extensions.GL3DFX_multisample = QueryExtension("GL_3DFX_multisample");
	extgl_Extensions.GL3DFX_texture_compression_FXT1 = QueryExtension("GL_3DFX_texture_compression_FXT1");
	extgl_Extensions.APPLE_client_storage = QueryExtension("GL_APPLE_client_storage");
	extgl_Extensions.APPLE_element_array = QueryExtension("GL_APPLE_element_array");
	extgl_Extensions.APPLE_fence = QueryExtension("GL_APPLE_fence");
	extgl_Extensions.APPLE_specular_vector = QueryExtension("GL_APPLE_specular_vector");
	extgl_Extensions.APPLE_transform_hint = QueryExtension("GL_APPLE_transform_hint");
	extgl_Extensions.APPLE_vertex_array_object = QueryExtension("GL_APPLE_vertex_array_object");
	extgl_Extensions.APPLE_vertex_array_range = QueryExtension("GL_APPLE_vertex_array_range");
	extgl_Extensions.APPLE_ycbcr_422 = QueryExtension("GL_APPLE_ycbcr_422");
	extgl_Extensions.ARB_depth_texture = QueryExtension("GL_ARB_depth_texture");
	extgl_Extensions.ARB_fragment_program = QueryExtension("GL_ARB_fragment_program");
	extgl_Extensions.ARB_fragment_program_shadow = QueryExtension("GL_ARB_fragment_program_shadow");
	extgl_Extensions.ARB_fragment_shader = QueryExtension("GL_ARB_fragment_shader");
	extgl_Extensions.ARB_imaging = QueryExtension("GL_ARB_imaging");
	extgl_Extensions.ARB_matrix_palette = QueryExtension("GL_ARB_matrix_palette");
	extgl_Extensions.ARB_multisample = QueryExtension("GL_ARB_multisample");
	extgl_Extensions.ARB_multitexture = QueryExtension("GL_ARB_multitexture");
	extgl_Extensions.ARB_occlusion_query = QueryExtension("GL_ARB_occlusion_query");
	extgl_Extensions.ARB_shader_objects = QueryExtension("GL_ARB_shader_objects");
	extgl_Extensions.ARB_vertex_shader = QueryExtension("GL_ARB_vertex_shader");
	extgl_Extensions.ARB_fragment_shader = QueryExtension("GL_ARB_fragment_shader");
	extgl_Extensions.ARB_shading_language_100 = QueryExtension("GL_ARB_shading_language_100");
	extgl_Extensions.ARB_texture_non_power_of_two = QueryExtension("GL_ARB_texture_non_power_of_two");
	extgl_Extensions.ARB_point_sprite = QueryExtension("GL_ARB_point_sprite");
	extgl_Extensions.ARB_fragment_program_shadow = QueryExtension("GL_ARB_fragment_program_shadow");
	extgl_Extensions.ARB_point_parameters = QueryExtension("GL_ARB_point_parameters");
	extgl_Extensions.ARB_shadow = QueryExtension("GL_ARB_shadow");
	extgl_Extensions.ARB_shadow_ambient = QueryExtension("GL_ARB_shadow_ambient");
	extgl_Extensions.ARB_texture_border_clamp = QueryExtension("GL_ARB_texture_border_clamp");
	extgl_Extensions.ARB_texture_compression = QueryExtension("GL_ARB_texture_compression");
	extgl_Extensions.ARB_texture_cube_map = QueryExtension("GL_ARB_texture_cube_map");
	extgl_Extensions.ARB_texture_env_add = QueryExtension("GL_ARB_texture_env_add");
	extgl_Extensions.ARB_texture_env_combine = QueryExtension("GL_ARB_texture_env_combine");
	extgl_Extensions.ARB_texture_env_crossbar = QueryExtension("GL_ARB_texture_env_crossbar");
	extgl_Extensions.ARB_texture_env_dot3 = QueryExtension("GL_ARB_texture_env_dot3");
	extgl_Extensions.ARB_texture_mirrored_repeat = QueryExtension("GL_ARB_texture_mirrored_repeat");
	extgl_Extensions.ARB_transpose_matrix = QueryExtension("GL_ARB_transpose_matrix");
	extgl_Extensions.ARB_vertex_blend = QueryExtension("GL_ARB_vertex_blend");
	extgl_Extensions.ARB_vertex_buffer_object = QueryExtension("GL_ARB_vertex_buffer_object");
	extgl_Extensions.ARB_vertex_program = QueryExtension("GL_ARB_vertex_program");
	extgl_Extensions.ARB_window_pos = QueryExtension("GL_ARB_window_pos");
	extgl_Extensions.ATIX_point_sprites = QueryExtension("GL_ATIX_point_sprites");
	extgl_Extensions.ATIX_texture_env_combine3 = QueryExtension("GL_ATIX_texture_env_combine3");
	extgl_Extensions.ATIX_texture_env_route = QueryExtension("GL_ATIX_texture_env_route");
	extgl_Extensions.ATIX_vertex_shader_output_point_size = QueryExtension("GL_ATIX_vertex_shader_output_point_size");
	extgl_Extensions.ATI_draw_buffers = QueryExtension("GL_ATI_draw_buffers");
	extgl_Extensions.ATI_element_array = QueryExtension("GL_ATI_element_array");
	extgl_Extensions.ATI_envmap_bumpmap = QueryExtension("GL_ATI_envmap_bumpmap");
	extgl_Extensions.ATI_fragment_shader = QueryExtension("GL_ATI_fragment_shader");
	extgl_Extensions.ATI_map_object_buffer = QueryExtension("GL_ATI_map_object_buffer");
	extgl_Extensions.ATI_pn_triangles = QueryExtension("GL_ATI_pn_triangles");
	extgl_Extensions.ATI_point_cull_mode = QueryExtension("GL_ATI_point_cull_mode");
	extgl_Extensions.ATI_separate_stencil = QueryExtension("GL_ATI_separate_stencil");
	extgl_Extensions.ATI_vertex_attrib_array_object = QueryExtension("GL_ATI_vertex_attrib_array_object");
	extgl_Extensions.ATI_text_fragment_shader = QueryExtension("GL_ATI_text_fragment_shader");
	extgl_Extensions.ATI_texture_env_combine3 = QueryExtension("GL_ATI_texture_env_combine3");
	extgl_Extensions.ATI_texture_compression_3dc = QueryExtension("GL_ATI_texture_compression_3dc");
	extgl_Extensions.ATI_texture_float = QueryExtension("GL_ATI_texture_float");
	extgl_Extensions.ATI_texture_mirror_once = QueryExtension("GL_ATI_texture_mirror_once");
	extgl_Extensions.ATI_vertex_array_object = QueryExtension("GL_ATI_vertex_array_object");
	extgl_Extensions.ATI_vertex_streams = QueryExtension("GL_ATI_vertex_streams");
	extgl_Extensions.EXT_422_pixels = QueryExtension("GL_EXT_422_pixels");
	extgl_Extensions.EXT_abgr = QueryExtension("GL_EXT_abgr");
	extgl_Extensions.EXT_bgra = QueryExtension("GL_EXT_bgra");
	extgl_Extensions.ARB_draw_buffers = QueryExtension("GL_ARB_draw_buffers");
	extgl_Extensions.ARB_color_buffer_float = QueryExtension("GL_ARB_color_buffer_float");
	extgl_Extensions.EXT_blend_color = QueryExtension("GL_EXT_blend_color");
	extgl_Extensions.EXT_blend_func_separate = QueryExtension("GL_EXT_blend_func_separate");
	extgl_Extensions.EXT_blend_logic_op = QueryExtension("GL_EXT_blend_logic_op");
	extgl_Extensions.EXT_blend_minmax = QueryExtension("GL_EXT_blend_minmax");
	extgl_Extensions.EXT_blend_subtract = QueryExtension("GL_EXT_blend_subtract");
	extgl_Extensions.EXT_clip_volume_hint = QueryExtension("GL_EXT_clip_volume_hint");
	extgl_Extensions.EXT_cmyka = QueryExtension("GL_EXT_cmyka");
	extgl_Extensions.EXT_color_matrix = QueryExtension("GL_EXT_color_matrix");
	extgl_Extensions.EXT_color_subtable = QueryExtension("GL_EXT_color_subtable");
	extgl_Extensions.EXT_compiled_vertex_array = QueryExtension("GL_EXT_compiled_vertex_array");
	extgl_Extensions.EXT_convolution = QueryExtension("GL_EXT_convolution");
	extgl_Extensions.EXT_coordinate_frame = QueryExtension("GL_EXT_coordinate_frame");
	extgl_Extensions.EXT_copy_texture = QueryExtension("GL_EXT_copy_texture");
	extgl_Extensions.EXT_cull_vertex = QueryExtension("GL_EXT_cull_vertex");
	extgl_Extensions.EXT_depth_bounds_test = QueryExtension("GL_EXT_depth_bounds_test");
	extgl_Extensions.EXT_blend_equation_separate = QueryExtension("GL_EXT_blend_equation_separate");
	extgl_Extensions.MESA_pack_invert = QueryExtension("GL_MESA_pack_invert");
	extgl_Extensions.MESA_ycbcr_texture = QueryExtension("GL_MESA_ycbcr_texture");
	extgl_Extensions.EXT_pixel_buffer_object = QueryExtension("GL_EXT_pixel_buffer_object");
	extgl_Extensions.NV_fragment_program_option = QueryExtension("GL_NV_fragment_program_option");
	extgl_Extensions.NV_fragment_program2 = QueryExtension("GL_NV_fragment_program2");
	extgl_Extensions.NV_vertex_program2_option = QueryExtension("GL_NV_vertex_program2_option");
	extgl_Extensions.NV_vertex_program3 = QueryExtension("GL_NV_vertex_program3");
	extgl_Extensions.EXT_draw_range_elements = QueryExtension("GL_EXT_draw_range_elements");
	extgl_Extensions.EXT_fog_coord = QueryExtension("GL_EXT_fog_coord");
	extgl_Extensions.EXT_framebuffer_object = QueryExtension("GL_EXT_framebuffer_object");
	extgl_Extensions.EXT_histogram = QueryExtension("GL_EXT_histogram");
	extgl_Extensions.EXT_index_array_formats = QueryExtension("GL_EXT_index_array_formats");
	extgl_Extensions.EXT_index_func = QueryExtension("GL_EXT_index_func");
	extgl_Extensions.EXT_index_material = QueryExtension("GL_EXT_index_material");
	extgl_Extensions.EXT_index_texture = QueryExtension("GL_EXT_index_texture");
	extgl_Extensions.EXT_light_texture = QueryExtension("GL_EXT_light_texture");
	extgl_Extensions.EXT_misc_attribute = QueryExtension("GL_EXT_misc_attribute");
	extgl_Extensions.EXT_multi_draw_arrays = QueryExtension("GL_EXT_multi_draw_arrays");
	extgl_Extensions.EXT_multisample = QueryExtension("GL_EXT_multisample");
	extgl_Extensions.EXT_packed_pixels = QueryExtension("GL_EXT_packed_pixels");
	extgl_Extensions.EXT_paletted_texture = QueryExtension("GL_EXT_paletted_texture");
	extgl_Extensions.EXT_pixel_transform = QueryExtension("GL_EXT_pixel_transform");
	extgl_Extensions.EXT_pixel_transform_color_table = QueryExtension("GL_EXT_pixel_transform_color_table");
	extgl_Extensions.EXT_point_parameters = QueryExtension("GL_EXT_point_parameters");
	extgl_Extensions.EXT_polygon_offset = QueryExtension("GL_EXT_polygon_offset");
	extgl_Extensions.EXT_rescale_normal = QueryExtension("GL_EXT_rescale_normal");
	extgl_Extensions.EXT_secondary_color = QueryExtension("GL_EXT_secondary_color");
	extgl_Extensions.EXT_separate_specular_color = QueryExtension("GL_EXT_separate_specular_color");
	extgl_Extensions.EXT_shadow_funcs = QueryExtension("GL_EXT_shadow_funcs");
	extgl_Extensions.EXT_shared_texture_palette = QueryExtension("GL_EXT_shared_texture_palette");
	extgl_Extensions.EXT_stencil_two_side = QueryExtension("GL_EXT_stencil_two_side");
	extgl_Extensions.EXT_stencil_wrap = QueryExtension("GL_EXT_stencil_wrap");
	extgl_Extensions.EXT_subtexture = QueryExtension("GL_EXT_subtexture");
	extgl_Extensions.EXT_texgen_reflection = QueryExtension("GL_EXT_texgen_reflection");
	extgl_Extensions.EXT_texture = QueryExtension("GL_EXT_texture");
	extgl_Extensions.EXT_texture3D = QueryExtension("GL_EXT_texture3D");
	extgl_Extensions.EXT_texture_compression_s3tc = QueryExtension("GL_EXT_texture_compression_s3tc");
	extgl_Extensions.EXT_texture_cube_map = QueryExtension("GL_EXT_texture_cube_map");
	extgl_Extensions.EXT_texture_edge_clamp = QueryExtension("GL_EXT_texture_edge_clamp");
	extgl_Extensions.EXT_texture_env_add = QueryExtension("GL_EXT_texture_env_add");
	extgl_Extensions.EXT_texture_env_combine = QueryExtension("GL_EXT_texture_env_combine");
	extgl_Extensions.EXT_texture_env_dot3 = QueryExtension("GL_EXT_texture_env_dot3");
	extgl_Extensions.EXT_texture_filter_anisotropic = QueryExtension("GL_EXT_texture_filter_anisotropic");
	extgl_Extensions.EXT_texture_lod = QueryExtension("GL_EXT_texture_lod");
	extgl_Extensions.EXT_texture_lod_bias = QueryExtension("GL_EXT_texture_lod_bias");
	extgl_Extensions.EXT_texture_object = QueryExtension("GL_EXT_texture_object");
	extgl_Extensions.EXT_texture_perturb_normal = QueryExtension("GL_EXT_texture_perturb_normal");
	extgl_Extensions.EXT_texture_rectangle = QueryExtension("GL_EXT_texture_rectangle");
	extgl_Extensions.EXT_vertex_array = QueryExtension("GL_EXT_vertex_array");
	extgl_Extensions.EXT_vertex_shader = QueryExtension("GL_EXT_vertex_shader");
	extgl_Extensions.EXT_vertex_weighting = QueryExtension("GL_EXT_vertex_weighting");
	extgl_Extensions.FfdMaskSGIX = QueryExtension("GL_FfdMaskSGIX");
	extgl_Extensions.HP_convolution_border_modes = QueryExtension("GL_HP_convolution_border_modes");
	extgl_Extensions.HP_image_transform = QueryExtension("GL_HP_image_transform");
	extgl_Extensions.HP_occlusion_test = QueryExtension("GL_HP_occlusion_test");
	extgl_Extensions.HP_texture_lighting = QueryExtension("GL_HP_texture_lighting");
	extgl_Extensions.IBM_cull_vertex = QueryExtension("GL_IBM_cull_vertex");
	extgl_Extensions.IBM_multimode_draw_arrays = QueryExtension("GL_IBM_multimode_draw_arrays");
	extgl_Extensions.IBM_vertex_array_lists = QueryExtension("GL_IBM_vertex_array_lists");
	extgl_Extensions.IBM_rasterpos_clip = QueryExtension("GL_IBM_rasterpos_clip");
	extgl_Extensions.IBM_texture_mirrored_repeat = QueryExtension("GL_IBM_texture_mirrored_repeat");
	extgl_Extensions.INGR_color_clamp = QueryExtension("GL_INGR_color_clamp");
	extgl_Extensions.INGR_interlace_read = QueryExtension("GL_INGR_interlace_read");
	extgl_Extensions.INGR_palette_buffer = QueryExtension("GL_INGR_palette_buffer");
	extgl_Extensions.INTEL_parallel_arrays = QueryExtension("GL_INTEL_parallel_arrays");
	extgl_Extensions.INTEL_texture_scissor = QueryExtension("GL_INTEL_texture_scissor");
	extgl_Extensions.KTX_buffer_region = QueryExtension("GL_KTX_buffer_region");
	extgl_Extensions.MESA_resize_buffers = QueryExtension("GL_MESA_resize_buffers");
	extgl_Extensions.MESA_window_pos = QueryExtension("GL_MESA_window_pos");
	extgl_Extensions.NVX_ycrcb = QueryExtension("GL_NVX_ycrcb");
	extgl_Extensions.NV_blend_square = QueryExtension("GL_NV_blend_square");
	extgl_Extensions.NV_copy_depth_to_color = QueryExtension("GL_NV_copy_depth_to_color");
	extgl_Extensions.NV_depth_clamp = QueryExtension("GL_NV_depth_clamp");
	extgl_Extensions.NV_element_array = QueryExtension("GL_NV_element_array");
	extgl_Extensions.NV_evaluators = QueryExtension("GL_NV_evaluators");
	extgl_Extensions.NV_fence = QueryExtension("GL_NV_fence");
	extgl_Extensions.NV_float_buffer = QueryExtension("GL_NV_float_buffer");
	extgl_Extensions.NV_fog_distance = QueryExtension("GL_NV_fog_distance");
	extgl_Extensions.NV_fragment_program = QueryExtension("GL_NV_fragment_program");
	extgl_Extensions.NV_half_float = QueryExtension("GL_NV_half_float");
	extgl_Extensions.NV_light_max_exponent = QueryExtension("GL_NV_light_max_exponent");
	extgl_Extensions.NV_multisample_filter_hint = QueryExtension("GL_NV_multisample_filter_hint");
	extgl_Extensions.NV_occlusion_query = QueryExtension("GL_NV_occlusion_query");
	extgl_Extensions.NV_packed_depth_stencil = QueryExtension("GL_NV_packed_depth_stencil");
	extgl_Extensions.NV_pixel_data_range = QueryExtension("GL_NV_pixel_data_range");
	extgl_Extensions.NV_point_sprite = QueryExtension("GL_NV_point_sprite");
	extgl_Extensions.NV_primitive_restart = QueryExtension("GL_NV_primitive_restart");
	extgl_Extensions.NV_register_combiners = QueryExtension("GL_NV_register_combiners");
	extgl_Extensions.NV_register_combiners2 = QueryExtension("GL_NV_register_combiners2");
	extgl_Extensions.NV_texgen_emboss = QueryExtension("GL_NV_texgen_emboss");
	extgl_Extensions.NV_texgen_reflection = QueryExtension("GL_NV_texgen_reflection");
	extgl_Extensions.NV_texture_compression_vtc = QueryExtension("GL_NV_texture_compression_vtc");
	extgl_Extensions.NV_texture_env_combine4 = QueryExtension("GL_NV_texture_env_combine4");
	extgl_Extensions.NV_texture_expand_normal = QueryExtension("GL_NV_texture_expand_normal");
	extgl_Extensions.NV_texture_rectangle = QueryExtension("GL_NV_texture_rectangle");
	extgl_Extensions.NV_texture_shader = QueryExtension("GL_NV_texture_shader");
	extgl_Extensions.NV_texture_shader2 = QueryExtension("GL_NV_texture_shader2");
	extgl_Extensions.NV_texture_shader3 = QueryExtension("GL_NV_texture_shader3");
	extgl_Extensions.NV_vertex_array_range = QueryExtension("GL_NV_vertex_array_range");
	extgl_Extensions.NV_vertex_array_range2 = QueryExtension("GL_NV_vertex_array_range2");
	extgl_Extensions.NV_vertex_program = QueryExtension("GL_NV_vertex_program");
	extgl_Extensions.NV_vertex_program1_1 = QueryExtension("GL_NV_vertex_program1_1");
	extgl_Extensions.NV_vertex_program2 = QueryExtension("GL_NV_vertex_program2");
	extgl_Extensions.OML_interlace = QueryExtension("GL_OML_interlace");
	extgl_Extensions.OML_resample = QueryExtension("GL_OML_resample");
	extgl_Extensions.OML_subsample = QueryExtension("GL_OML_subsample");
	extgl_Extensions.PGI_misc_hints = QueryExtension("GL_PGI_misc_hints");
	extgl_Extensions.PGI_vertex_hints = QueryExtension("GL_PGI_vertex_hints");
	extgl_Extensions.REND_screen_coordinates = QueryExtension("GL_REND_screen_coordinates");
	extgl_Extensions.S3_s3tc = QueryExtension("GL_S3_s3tc");
	extgl_Extensions.SGIS_detail_texture = QueryExtension("GL_SGIS_detail_texture");
	extgl_Extensions.SGIS_fog_function = QueryExtension("GL_SGIS_fog_function");
	extgl_Extensions.SGIS_generate_mipmap = QueryExtension("GL_SGIS_generate_mipmap");
	extgl_Extensions.SGIS_multisample = QueryExtension("GL_SGIS_multisample");
	extgl_Extensions.SGIS_multitexture = QueryExtension("GL_SGIS_multitexture");
	extgl_Extensions.SGIS_pixel_texture = QueryExtension("GL_SGIS_pixel_texture");
	extgl_Extensions.SGIS_point_line_texgen = QueryExtension("GL_SGIS_point_line_texgen");
	extgl_Extensions.SGIS_point_parameters = QueryExtension("GL_SGIS_point_parameters");
	extgl_Extensions.SGIS_sharpen_texture = QueryExtension("GL_SGIS_sharpen_texture");
	extgl_Extensions.SGIS_texture4D = QueryExtension("GL_SGIS_texture4D");
	extgl_Extensions.SGIS_texture_border_clamp = QueryExtension("GL_SGIS_texture_border_clamp");
	extgl_Extensions.SGIS_texture_color_mask = QueryExtension("GL_SGIS_texture_color_mask");
	extgl_Extensions.SGIS_texture_edge_clamp = QueryExtension("GL_SGIS_texture_edge_clamp");
	extgl_Extensions.SGIS_texture_filter4 = QueryExtension("GL_SGIS_texture_filter4");
	extgl_Extensions.SGIS_texture_lod = QueryExtension("GL_SGIS_texture_lod");
	extgl_Extensions.SGIS_texture_select = QueryExtension("GL_SGIS_texture_select");
	extgl_Extensions.SGIX_async_histogram = QueryExtension("GL_SGIX_async_histogram");
	extgl_Extensions.SGIX_async_pixel = QueryExtension("GL_SGIX_async_pixel");
	extgl_Extensions.SGIX_asyncr = QueryExtension("GL_SGIX_asyncr");
	extgl_Extensions.SGIX_blend_alpha_minmax = QueryExtension("GL_SGIX_blend_alpha_minmax");
	extgl_Extensions.SGIX_calligraphic_fragment = QueryExtension("GL_SGIX_calligraphic_fragment");
	extgl_Extensions.SGIX_clipmap = QueryExtension("GL_SGIX_clipmap");
	extgl_Extensions.SGIX_convolution_accuracy = QueryExtension("GL_SGIX_convolution_accuracy");
	extgl_Extensions.SGIX_depth_texture = QueryExtension("GL_SGIX_depth_texture");
	extgl_Extensions.SGIX_flush_raster = QueryExtension("GL_SGIX_flush_raster");
	extgl_Extensions.SGIX_fog_offset = QueryExtension("GL_SGIX_fog_offset");
	extgl_Extensions.SGIX_fog_scale = QueryExtension("GL_SGIX_fog_scale");
	extgl_Extensions.SGIX_fragment_lighting = QueryExtension("GL_SGIX_fragment_lighting");
	extgl_Extensions.SGIX_framezoom = QueryExtension("GL_SGIX_framezoom");
	extgl_Extensions.SGIX_impact_pixel_texture = QueryExtension("GL_SGIX_impact_pixel_texture");
	extgl_Extensions.SGIX_instruments = QueryExtension("GL_SGIX_instruments");
	extgl_Extensions.SGIX_interlace = QueryExtension("GL_SGIX_interlace");
	extgl_Extensions.SGIX_ir_instrument1 = QueryExtension("GL_SGIX_ir_instrument1");
	extgl_Extensions.SGIX_list_priority = QueryExtension("GL_SGIX_list_priority");
	extgl_Extensions.SGIX_pixel_texture = QueryExtension("GL_SGIX_pixel_texture");
	extgl_Extensions.SGIX_pixel_tiles = QueryExtension("GL_SGIX_pixel_tiles");
	extgl_Extensions.SGIX_polynomial_ffd = QueryExtension("GL_SGIX_polynomial_ffd");
	extgl_Extensions.SGIX_reference_plane = QueryExtension("GL_SGIX_reference_plane");
	extgl_Extensions.SGIX_resample = QueryExtension("GL_SGIX_resample");
	extgl_Extensions.SGIX_scalebias_hint = QueryExtension("GL_SGIX_scalebias_hint");
	extgl_Extensions.SGIX_shadow = QueryExtension("GL_SGIX_shadow");
	extgl_Extensions.SGIX_shadow_ambient = QueryExtension("GL_SGIX_shadow_ambient");
	extgl_Extensions.SGIX_sprite = QueryExtension("GL_SGIX_sprite");
	extgl_Extensions.SGIX_subsample = QueryExtension("GL_SGIX_subsample");
	extgl_Extensions.SGIX_tag_sample_buffer = QueryExtension("GL_SGIX_tag_sample_buffer");
	extgl_Extensions.SGIX_texture_add_env = QueryExtension("GL_SGIX_texture_add_env");
	extgl_Extensions.SGIX_texture_coordinate_clamp = QueryExtension("GL_SGIX_texture_coordinate_clamp");
	extgl_Extensions.SGIX_texture_lod_bias = QueryExtension("GL_SGIX_texture_lod_bias");
	extgl_Extensions.SGIX_texture_multi_buffer = QueryExtension("GL_SGIX_texture_multi_buffer");
	extgl_Extensions.SGIX_texture_scale_bias = QueryExtension("GL_SGIX_texture_scale_bias");
	extgl_Extensions.SGIX_vertex_preclip = QueryExtension("GL_SGIX_vertex_preclip");
	extgl_Extensions.SGIX_ycrcb = QueryExtension("GL_SGIX_ycrcb");
	extgl_Extensions.SGIX_ycrcb_subsample = QueryExtension("GL_SGIX_ycrcb_subsample");
	extgl_Extensions.SGIX_ycrcba = QueryExtension("GL_SGIX_ycrcba");
	extgl_Extensions.SGI_color_matrix = QueryExtension("GL_SGI_color_matrix");
	extgl_Extensions.SGI_color_table = QueryExtension("GL_SGI_color_table");
	extgl_Extensions.SGI_depth_pass_instrument = QueryExtension("GL_SGI_depth_pass_instrument");
	extgl_Extensions.SGI_texture_color_table = QueryExtension("GL_SGI_texture_color_table");
	extgl_Extensions.SUNX_constant_data = QueryExtension("GL_SUNX_constant_data");
	extgl_Extensions.SUN_convolution_border_modes = QueryExtension("GL_SUN_convolution_border_modes");
	extgl_Extensions.SUN_global_alpha = QueryExtension("GL_SUN_global_alpha");
	extgl_Extensions.SUN_mesh_array = QueryExtension("GL_SUN_mesh_array");
	extgl_Extensions.SUN_multi_draw_arrays = QueryExtension("GL_SUN_multi_draw_arrays");
	extgl_Extensions.SUN_slice_accum = QueryExtension("GL_SUN_slice_accum");
	extgl_Extensions.SUN_triangle_list = QueryExtension("GL_SUN_triangle_list");
	extgl_Extensions.SUN_vertex = QueryExtension("GL_SUN_vertex");
	extgl_Extensions.WIN_phong_shading = QueryExtension("GL_WIN_phong_shading");
	extgl_Extensions.WIN_specular_fog = QueryExtension("GL_WIN_specular_fog");
	extgl_Extensions.WIN_swap_hint = QueryExtension("GL_WIN_swap_hint");
}

void extgl_InitWINSwapHint()
{
#ifdef GL_WIN_swap_hint
    if (!extgl_Extensions.WIN_swap_hint)
        return;
    glAddSwapHintRectWIN = (glAddSwapHintRectWINPROC) extgl_GetProcAddress("glAddSwapHintRectWIN");
#endif
}

/* extgl_Init the extensions and load all the functions */
int extgl_Initialize()
{
    extgl_error = 0;
    extgl_InitSupportedExtensions();

    /* first load the extensions */
	extgl_InitAPPLEElementArray();
	extgl_InitAPPLEFence();
	extgl_InitAPPLEVertexArrayObject();
	extgl_InitAPPLEVertexArrayRange();
	extgl_InitARBFragmentProgram();
	extgl_InitARBFragmentProgramShadow();
	extgl_InitARBFragmentShader();
	extgl_InitARBImaging();
	extgl_InitARBMatrixPalette();
	extgl_InitARBMultisample();
	extgl_InitARBMultitexture();
	extgl_InitARBOcclusionQuery();
	extgl_InitARBPointParameters();
	extgl_InitARBShaderObjects();
	extgl_InitARBTextureCompression();
	extgl_InitARBTransposeMatrix();
	extgl_InitARBVertexBlend();
	extgl_InitARBVertexBufferObject();
	extgl_InitARBVertexProgram();
	extgl_InitARBVertexShader();
	extgl_InitARBWindowPos();
	extgl_InitATIDrawBuffers();
	extgl_InitATIElementArray();
	extgl_InitATIEnvmapBumpmap();
	extgl_InitATIFragmentShader();
	extgl_InitATIMapObjectBuffer();
	extgl_InitATIPNTriangles();
	extgl_InitATISeparateStencil();
	extgl_InitATIVertexAttribArrayObject();
	extgl_InitATIVertexArrayObject();
	extgl_InitATIVertexAttribArrayObject();
	extgl_InitATIVertexStreams();
	extgl_InitARBDrawBuffers();
	extgl_InitEXTBlendColor();
	extgl_InitEXTBlendFuncSeparate();
	extgl_InitEXTBlendMinmax();
	extgl_InitEXTColorSubtable();
	extgl_InitEXTCompiledVertexArray();
	extgl_InitEXTConvolution();
	extgl_InitEXTCoordinateFrame();
	extgl_InitEXTCopyTexture();
	extgl_InitEXTCullVertex();
	extgl_InitEXTDepthBoundsTest();
	extgl_InitEXTBlendEquationSeparate();
	extgl_InitEXTDrawRangeElements();
	extgl_InitEXTFogCoord();
	extgl_InitEXTFramebufferObject();
	extgl_InitEXTHistogram();
	extgl_InitEXTIndexFunc();
	extgl_InitEXTIndexMaterial();
	extgl_InitEXTLightTexture();
	extgl_InitEXTMultiDrawArrays();
	extgl_InitEXTMultisample();
	extgl_InitEXTPaletteTexture();
	extgl_InitEXTPixelTransform();
	extgl_InitEXTPointParameters();
	extgl_InitEXTPolygonOffset();
	extgl_InitEXTSecondaryColor();
	extgl_InitEXTStencilTwoSide();
	extgl_InitEXTSubtexture();
	extgl_InitEXTTexture3D();
	extgl_InitEXTTextureObject();
	extgl_InitEXTTexturePerturbNormal();
	extgl_InitEXTVertexArray();
	extgl_InitEXTVertexShader();
	extgl_InitEXTVertexWeighting();
	extgl_InitHPImageTransform();
	extgl_InitIBMMultimodeDrawArrays();
	extgl_InitIBMVertexArrayLists();
	extgl_InitINTELParallelArrays();
	extgl_InitKTXBufferRegion();
	extgl_InitMESAResizeBuffers();
	extgl_InitMESAWindowPos();
	extgl_InitNVElementArray();
	extgl_InitNVEvaluators();
	extgl_InitNVFence();
	extgl_InitNVFragmentProgram();
	extgl_InitNVHalfFloat();
	extgl_InitNVOcclusionQuery();
	extgl_InitNVPixelDataRange();
	extgl_InitNVPointSprite();
	extgl_InitNVPrimitiveRestart();
	extgl_InitNVRegisterCombiners();
	extgl_InitNVRegisterCombiners2();
	extgl_InitNVVertexArrayRange();
	extgl_InitNVVertexProgram();
	extgl_InitPGIMiscHints();
	extgl_InitSGIColorTable();
	extgl_InitSGISDetailTexture();
	extgl_InitSGISFogFunction();
	extgl_InitSGISMultisample();
	extgl_InitSGISMultitexture();
	extgl_InitSGISPixelTexture();
	extgl_InitSGISSharpenTexture();
	extgl_InitSGISTexture4D();
	extgl_InitSGISTextureColorMask();
	extgl_InitSGISTextureFilter4();
	extgl_InitSGIXAsync();
	extgl_InitSGIXFlushRaster();
	extgl_InitSGIXFragmentLighting();
	extgl_InitSGIXFramezoom();
	extgl_InitSGIXInstruments();
	extgl_InitSGIXListPriority();
	extgl_InitSGIXPixelTexture();
	extgl_InitSGIXPolynomialFfd();
	extgl_InitSGIXReferencePlane();
	extgl_InitSUNGlobalAlpha();
	extgl_InitSUNMeshArray();
	extgl_InitSUNMultiDrawArrays();
	extgl_InitSUNTriangleList();
	extgl_InitSUNVertex();
	extgl_InitSUNXConstantData();
	extgl_InitWINSwapHint();

   /* now load core opengl */
    extgl_InitOpenGL1_2();
    extgl_InitOpenGL1_3();
    extgl_InitOpenGL1_4();
	extgl_InitOpenGL1_5();
	extgl_InitOpenGL2_0();

    /* load WGL extensions */
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
    extgl_InitializeWGL();
#endif

    SupportedExtensions = extgl_Extensions;
    return extgl_error;
}

/* deprecated function please do not use it, use extgl_Initialize() instead */
int glInitialize()
{
    return extgl_Initialize();
}

//#if defined(DEBUG) || defined(_DEBUG)
	#ifdef WIN32

		#define ShowText 													\
			if (bto_a_file) fwrite(&strBuffer, 1, strlen(strBuffer)* sizeof(TCHAR), fp);	\
			else OutputDebugString( strBuffer );							\

		void extgl_DisplayValues(bool bto_a_file)
		{
			GLint tmpMax;
			GLint tmpMax1[2];
			GLboolean tmpBool;
			GLfloat tmpFloat;
			GLfloat tmpFloat1[2];
			char extensions[4096];
			TCHAR strBuffer[4096];
			TCHAR strExt[8192];
			char *token;
			FILE *fp = NULL;

			*strBuffer = (TCHAR)0;
			*strExt = (TCHAR)0;
			if (bto_a_file)
			{
				LPTSTR lpszSystemInfo;     // pointer to system information string
				lpszSystemInfo = strBuffer;
				DWORD tmpSize = 4096;
				GetComputerName(strBuffer,&tmpSize);
				sprintf (strExt, TEXT("Log/OpenGlDrv_%s.txt"), strBuffer);

				_unlink ( strExt );

				fp = fopen(strExt, "wt");
				if (!fp) bto_a_file = false;
			}

			/* Establish string and get the first token: */
			/* Make a copy of the string instead of a pointer */
			/* If not, strtok will screw up the memory */
			strcpy(extensions, (const char *)glGetString(GL_EXTENSIONS));
			token = strtok( extensions, TEXT(" ") );

			*strBuffer = (TCHAR)0;
			*strExt = (TCHAR)0;
			while( token != NULL )
			{
				/* While there are tokens in "string" */
				sprintf (strExt, TEXT("%s%s\n"), strExt, token);
				/* Get next token: */
				token = strtok( NULL, TEXT(" ") );
			}

			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);

			//sprintf( strBuffer, TEXT("Date: %02d/%02d/%02d Time: %02dh%02d:%02d\n"), sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond );
			//ShowText;

			//sprintf( strBuffer, TEXT("CPU Type: %d, Freq: %fMHz, Vendor: %s, Brand Name: %s \nCMOV: %s, 3DNow: %s, 3DNowExt: %s, MMX: %s, MMXExt: %s \nSSE: %s, SSE2: %s, SSE3: %s, Number of CPU Core: %d \n\n"), cpuFamily, cpuMHz, cpuVendor, cpuBrandName, cpuCMOV == true ? "YES" : "NO", cpu3DNow == true ? "YES" : "NO", cpu3DNowExt == true ? "YES" : "NO", cpuMMX == true ? "YES" : "NO", cpuMMXExt == true ? "YES" : "NO", cpuSSE == true ? "YES" : "NO", cpuSSE2 == true ? "YES" : "NO", cpuSSE3 == true ? "YES" : "NO", cpuNumberOfCore);
			//ShowText;

			//sprintf( strBuffer, TEXT("Hyper-Threading Technology is supported in hardware: %s \nHyper-Threading Technology is enabled: %s Number of Logical Processor per Package: %d \n\n"), cpuHT == true ? "YES" : "NO", cpuHyperThreadingStatus, cpuProcessorPerPackage);
			//ShowText;

			//sprintf( strBuffer, TEXT("OpenGl Version: %s \nHardware: %s \nCard: %s\n"), glGetString ( GL_VERSION ), glGetString ( GL_RENDERER ), glGetString ( GL_VENDOR ) );
			//ShowText;

			//sprintf( strBuffer, TEXT("Extensions:\n%s\n"), strExt );
			//ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGL 1.1 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			// Wing Compatible Maximum values
			sprintf (strBuffer, TEXT("OPENGL 1.1 Wing Compatible Maximum values\n") );
			glGetIntegerv(GL_MAX_LIST_NESTING, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax display-list call nesting: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_EVAL_ORDER, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax evaluator polynomial order: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_LIGHTS, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax number of lights: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_CLIP_PLANES, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax number of user clipping planes: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax height or width of a texture image (without borders): %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax size of a glPixelMap translation table: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax depth of the attribute stack: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax modelview-matrix stack depth: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax selection-name stack depth: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax projection-matrix stack depth: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax depth of texture matrix stack: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tmpMax1);
			sprintf (strBuffer, TEXT("%s\tMax viewport dimensions: %d x %d\n"), strBuffer, tmpMax1[0], tmpMax1[1]); tmpMax1[0] = tmpMax1[1] = 0;
			glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &tmpMax);
			sprintf (strBuffer, TEXT("%s\tMax Client Attrib Stack Depth: %d\n"), strBuffer, tmpMax); tmpMax = 0;

			ShowText;

			glGetIntegerv(GL_SUBPIXEL_BITS, &tmpMax);
			sprintf (strBuffer, TEXT("\tNumber of bits of subpixel precision in x and y: %d\n"), tmpMax); tmpMax = 0;
			glGetBooleanv(GL_AUX_BUFFERS, &tmpBool);
			sprintf (strBuffer, TEXT("%s\tNumber of auxiliary buffers: %d\n"), strBuffer, tmpBool); tmpBool = 0;
			glGetBooleanv(GL_RGBA_MODE, &tmpBool);
			sprintf (strBuffer, TEXT("%s\tRGBA Mode - True if color buffers store RGBA: %d\n"), strBuffer, tmpBool); tmpBool = 0;
			glGetBooleanv(GL_INDEX_MODE, &tmpBool);
			sprintf (strBuffer, TEXT("%s\tIndex Mode - True if color buffers store indexes: %d\n"), strBuffer, tmpBool); tmpBool = 0;
			glGetBooleanv(GL_DOUBLEBUFFER, &tmpBool);
			sprintf (strBuffer, TEXT("%s\tDouble Buffer Mode - True if front and back buffers exist: %d\n"), strBuffer, tmpBool); tmpBool = 0;
			glGetFloatv(GL_STEREO, &tmpFloat);
			sprintf (strBuffer, TEXT("%s\tStereo Mode - True if left and right buffers exist: %f\n"), strBuffer, tmpFloat); tmpFloat = 0.0f;
			glGetFloatv(GL_POINT_SIZE_RANGE, tmpFloat1);
			sprintf (strBuffer, TEXT("%s\tPoint Size Range - Range (low to high) of antialiased point sizes: %f < Range < %f\n"), strBuffer, tmpFloat1[0], tmpFloat1[1]); tmpFloat1[0] = tmpFloat1[1] = 0.0f;
			glGetFloatv(GL_POINT_SIZE_GRANULARITY, &tmpFloat);
			sprintf (strBuffer, TEXT("%s\tPoint Size Granularity - Antialiased point size granularity: %f\n"), strBuffer, tmpFloat); tmpFloat = 0.0f;
			glGetFloatv(GL_LINE_WIDTH_RANGE, tmpFloat1);
			sprintf (strBuffer, TEXT("%s\tLine Width Range - Range (low to high) of antialiased line widths: %f < Range < %f\n"), strBuffer, tmpFloat1[0], tmpFloat1[1]); tmpFloat1[0] = tmpFloat1[1] = 0.0f;
			glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &tmpFloat);
			sprintf (strBuffer, TEXT("%s\tLine Width Granularity - Antialiased line-width granularity: %f\n"), strBuffer, tmpFloat); tmpFloat = 0.0f;

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGl 1.2 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.OpenGL12)
			{
				sprintf (strBuffer, TEXT("OPENGL 1.2 Maximum values\n") );
				glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Elements Vertices: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Elements Indices: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax 3D Texture Size: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Color Matrix Stack Depth: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("OPENGL 1.2 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGl 1.3 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.OpenGL13)
			{
				sprintf (strBuffer, TEXT("OPENGL 1.3 Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_UNITS, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Texture Units: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Cube Map Texture Size: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("OPENGL 1.3 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGl 1.4 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.OpenGL14)
			{
				sprintf (strBuffer, TEXT("OPENGL 1.4 Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &tmpMax);
				sprintf (strBuffer, TEXT("%s\tMax Texture Lod Bias: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("OPENGL 1.4 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGl 1.5 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.OpenGL15)
			{
				sprintf (strBuffer, TEXT("OPENGL 1.5 supported\n") );
			}
			else sprintf (strBuffer, TEXT("OPENGL 1.5 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------OpenGl 2.0 Maximum values------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.OpenGL20)
			{
				sprintf (strBuffer, TEXT("OPENGL 2.0 supported\n") );
			}
			else sprintf (strBuffer, TEXT("OPENGL 2.0 Not supported by this driver.\n") );

			ShowText;

	/*-------------------------------------------------------------------*/
	/*------------EXTENSIONS---------------------------------------------*/
	/*-------------------------------------------------------------------*/
			sprintf (strBuffer, TEXT("\nEXTENSIONS.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_MULTITEXTURE---------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_multitexture)
			{
				sprintf (strBuffer, TEXT("\tARB_MULTITEXTURE Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Units ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_MULTITEXTURE Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_TEXTURE_COMPRESSION--------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_texture_compression)
			{
				sprintf (strBuffer, TEXT("\tARB_TEXTURE_COMPRESSION Maximum values\n") );
				glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax number of compressed texture Format ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_TEXTURE_COMPRESSION Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_TEXTURE_CUBE_MAP-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_texture_cube_map)
			{
				sprintf (strBuffer, TEXT("\tARB_TEXTURE_CUBE_MAP Maximum values\n") );
				glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tCube Map Texture Size ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_TEXTURE_CUBE_MAP Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_VERTEX_BLEND---------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_vertex_blend)
			{
				sprintf (strBuffer, TEXT("\tARB_VERTEX_BLEND Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Units ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_VERTEX_BLEND Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_MATRIX_PALETTE-------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_matrix_palette)
			{
				sprintf (strBuffer, TEXT("\tARB_MATRIX_PALETTE Maximum values\n") );
				glGetIntegerv(GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Matrix Palette Stack Depth ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_PALETTE_MATRICES_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Palette Matrices ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_MATRIX_PALETTE Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_VERTEX_PROGRAM-------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_vertex_program)
			{
				sprintf (strBuffer, TEXT("\tARB_VERTEX_PROGRAM Maximum values\n") );
				glGetIntegerv(GL_MAX_PROGRAM_MATRICES_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Program Matrices ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Attribs ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Program Matrix Stack Depth ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;

				if (glGetProgramivARB)
				{
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Temporaries ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Temporaries ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Parameters ARB: %d\n"), strBuffer, tmpMax);
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Attribs ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Attribs ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Address Registers ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Address Registers ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Local Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Env Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				}
			}
			else sprintf (strBuffer, TEXT("\tARB_VERTEX_PROGRAM Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_FRAGMENT_PROGRAM-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_fragment_program)
			{
				sprintf (strBuffer, TEXT("\tARB_FRAGMENT_PROGRAM Maximum values\n") );
				glGetIntegerv(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Program Matrix Stack Depth ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Coords ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Image Units ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;

				if (glGetProgramivARB)
				{
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Temporaries ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Temporaries ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Parameters ARB: %d\n"), strBuffer, tmpMax);
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Attribs ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Attribs ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Address Registers ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Address Registers ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Local Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Env Parameters ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;

					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Alu Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Tex Instruxtions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Tex Indirections ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Alu Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Tex Instructions ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Native Tex Indirections ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				}
			}
			else sprintf (strBuffer, TEXT("\tARB_FRAGMENT_PROGRAM Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_VERTEX_SHADER--------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_vertex_shader)
			{
				sprintf (strBuffer, TEXT("\tARB_VERTEX_SHADER Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Uniform Components ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VARYING_FLOATS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Varying Floats ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Texture Units ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Combined Texture Image Units ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_VERTEX_SHADER Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_FRAGMENT_SHADER------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_fragment_shader)
			{
				sprintf (strBuffer, TEXT("\tARB_FRAGMENT_SHADER Maximum values\n") );
				glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Fragment Uniform Components ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_FRAGMENT_SHADER Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_OCCLUSION_QUERY------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_occlusion_query)
			{
				sprintf (strBuffer, TEXT("\tARB_OCCLUSION_QUERY Maximum values\n") );
				glGetQueryivARB(GL_SAMPLES_PASSED_ARB, GL_QUERY_COUNTER_BITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Query Counter Bits ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_OCCLUSION_QUERY Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_DRAW_BUFFERS---------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_draw_buffers)
			{
				sprintf (strBuffer, TEXT("\tARB_DRAW_BUFFERS Maximum values\n") );
				glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Draw Buffers ARB: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tARB_DRAW_BUFFERS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ARB_FRAGMENT_PROGRAM_SHADOW----------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ARB_fragment_program_shadow)
			{
				sprintf (strBuffer, TEXT("\tARB_FRAGMENT_PROGRAM_SHADOW is supported\n") );
			}

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_TEXTURE_FILTER_ANISOTROPIC-------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_texture_filter_anisotropic)
			{
				sprintf (strBuffer, TEXT("\tEXT_TEXTURE_FILTER_ANISOTROPIC Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Max Anisotropy Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_TEXTURE_FILTER_ANISOTROPIC Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_DRAW_RANGE_ELEMENTS--------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_draw_range_elements)
			{
				sprintf (strBuffer, TEXT("\tEXT_DRAW_RANGE_ELEMENTS Maximum values\n") );
				glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Elements Vertices Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Elements Indices Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_DRAW_RANGE_ELEMENTS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_TEXTURE_RECTANGLE----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_texture_rectangle)
			{
				sprintf (strBuffer, TEXT("\tEXT_TEXTURE_RECTANGLE Maximum values\n") );
				glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Rectangle Texture Size Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_TEXTURE_RECTANGLE Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_TEXTURE3D------------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_texture3D)
			{
				sprintf (strBuffer, TEXT("\tEXT_TEXTURE3D Maximum values\n") );
				glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax 3D Texture Size EXT: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_TEXTURE3D Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_VERTEX_SHADER--------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_vertex_shader)
			{
				sprintf (strBuffer, TEXT("\tEXT_VERTEX_SHADER Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Shader Instruction Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_SHADER_VARIANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Shader Variants Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_SHADER_INVARIANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Shader Invariants Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Shader Local Constants Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_VERTEX_SHADER_LOCALS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Shader Locals Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Optimized Vertex Shader Instructions Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Optimized Vertex Shader Variants Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Optimized Vertex Shader Invariants Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Optimized Vertex Shader Local Constants: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Optimized Vertex Shader Locals Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_VERTEX_SHADER Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------EXT_TEXTURE_LOD_BIAS-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.EXT_texture_lod_bias)
			{
				sprintf (strBuffer, TEXT("\tEXT_TEXTURE_LOD_BIAS Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS_EXT, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Lod Bias Ext: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tEXT_TEXTURE_LOD_BIAS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_VERTEX_ARRAY_RANGE----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_vertex_array_range)
			{
				sprintf (strBuffer, TEXT("\tNV_VERTEX_ARRAY_RANGE Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Array Range Element NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_VERTEX_ARRAY_RANGE Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_REGISTER_COMBINERS----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_register_combiners)
			{
				sprintf (strBuffer, TEXT("\tNV_REGISTER_COMBINERS Maximum values\n") );
				glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax General Combiners NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_REGISTER_COMBINERS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_TEXTURE_RECTANGLE-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_texture_rectangle)
			{
				sprintf (strBuffer, TEXT("\tNV_TEXTURE_RECTANGLE Maximum values\n") );
				glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Rectangle Texture Size NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_TEXTURE_RECTANGLE Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_VERTEX_PROGRAM--------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_vertex_program)
			{
				sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM Maximum values\n") );
				glGetIntegerv(GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Track Matrix Stack Depth NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_TRACK_MATRICES_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Track Matrices NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_LIGHT_MAX_EXPONENT----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_light_max_exponent)
			{
				sprintf (strBuffer, TEXT("\tNV_LIGHT_MAX_EXPONENT Maximum values\n") );
				glGetIntegerv(GL_MAX_SHININESS_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Shininess NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_SPOT_EXPONENT_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Spot Exponent NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_LIGHT_MAX_EXPONENT Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_EVALUATORS------------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_evaluators)
			{
				sprintf (strBuffer, TEXT("\tNV_EVALUATORS Maximum values\n") );
				glGetIntegerv(GL_MAX_MAP_TESSELLATION_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Map Tesselation: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_RATIONAL_EVAL_ORDER_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Rational Eval Order: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_EVALUATORS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_FRAGMENT_PROGRAM------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_fragment_program)
			{
				sprintf (strBuffer, TEXT("\tNV_FRAGMENT_PROGRAM Maximum values\n") );
				glGetIntegerv(GL_MAX_TEXTURE_COORDS_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Coords NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Texture Image Units NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Fragment Program Local Parameters NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_FRAGMENT_PROGRAM Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_FRAGMENT_PROGRAM2-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_fragment_program2)
			{
				sprintf (strBuffer, TEXT("\tNV_FRAGMENT_PROGRAM2 Maximum values\n") );
				if (glGetProgramivARB)
				{
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Execution Instructions: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_CALL_DEPTH_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Call Depth NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_IF_DEPTH_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program If Depth NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOOP_DEPTH_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Loop Depth NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOOP_COUNT_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Loop Count NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				}
			}
			else sprintf (strBuffer, TEXT("\tNV_FRAGMENT_PROGRAM2 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_VERTEX_PROGRAM2_OPTION------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_vertex_program2_option)
			{
				/* reuse GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV */
				/* reuse GL_MAX_PROGRAM_CALL_DEPTH_NV */
				sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM2_OPTION Maximum values\n") );
				if (glGetProgramivARB)
				{
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Execution Instructions: %d\n"), strBuffer, tmpMax); tmpMax = 0;
					glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_CALL_DEPTH_NV, &tmpMax);
					sprintf (strBuffer, TEXT("%s\t\tMax Program Call Depth NV: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				}
			}
			else sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM2_OPTION Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_VERTEX_PROGRAM3-------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_vertex_program3)
			{
				sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM3 Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Texture Image Units: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_VERTEX_PROGRAM3 Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------NV_OCCLUSION_QUERY-----------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.NV_occlusion_query)
			{
				sprintf (strBuffer, TEXT("\tNV_OCCLUSION_QUERY Maximum values\n") );
				glGetIntegerv(GL_QUERY_COUNTER_BITS_ARB, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Query Counter Bits\n\t\t(alias of GL_ARB_occlusion_query): %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tNV_OCCLUSION_QUERY Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ATI_PN_TRIANGLES---------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ATI_pn_triangles)
			{
				sprintf (strBuffer, TEXT("\tATI_PN_TRIANGLES Maximum values\n") );
				glGetIntegerv(GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax PN Triangles Tesselation Level ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tATI_PN_TRIANGLES Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ATI_DRAW_BUFFERS---------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ATI_draw_buffers)
			{
				sprintf (strBuffer, TEXT("\tATI_DRAW_BUFFERS Maximum values\n") );
				glGetIntegerv(GL_MAX_DRAW_BUFFERS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Draw Buffers ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tATI_DRAW_BUFFERS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ATI_VERTEX_STREAMS-------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ATI_vertex_streams)
			{
				sprintf (strBuffer, TEXT("\tATI_VERTEX_STREAMS Maximum values\n") );
				glGetIntegerv(GL_MAX_VERTEX_STREAMS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Vertex Streams ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tATI_VERTEX_STREAMS Not supported by this driver.\n") );

			ShowText;

			/*-------------------------------------------------------------------*/
			/*------------ATI_FRAGMENT_SHADER-------------------------------------*/
			/*-------------------------------------------------------------------*/
			if (extgl_Extensions.ATI_fragment_shader)
			{
				sprintf (strBuffer, TEXT("\tATI_FRAGMENT_SHADER Maximum values\n") );
				glGetIntegerv(GL_NUM_FRAGMENT_REGISTERS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Fragment Registers ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_FRAGMENT_CONSTANTS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Fragment Constants ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_PASSES_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Passes ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_INSTRUCTIONS_PER_PASS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Instructions per Pass ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_INSTRUCTIONS_TOTAL_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Instructions in Total ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_LOOPBACK_COMPONENTS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Loopback Components ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
				glGetIntegerv(GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI, &tmpMax);
				sprintf (strBuffer, TEXT("%s\t\tMax Number of Input Interpolator Components ATI: %d\n"), strBuffer, tmpMax); tmpMax = 0;
			}
			else sprintf (strBuffer, TEXT("\tATI_FRAGMENT_SHADER Not supported by this driver.\n") );

			ShowText;

			if (bto_a_file) fclose (fp);

		} // extgl_DisplayValues(void)
	#endif // #ifdef WIN32
//#endif // #if defined(DEBUG) | defined(_DEBUG)

/* turn on the warning for the borland compiler*/
#ifdef __BORLANDC__
#pragma warn .8064
#pragma warn .8065
#endif /* __BORLANDC__	*/

