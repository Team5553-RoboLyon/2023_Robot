/* Small parts were taken from glext.h, here's the lisence: */

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
** 
** http://oss.sgi.com/projects/FreeB
** 
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
** 
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
** 
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
*/

/*  Most parts copyright (c) 2001-2002 Lev Povalahev under this lisence: */

/* ----------------------------------------------------------------------------
Copyright (c) 2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.
    * The name of the author may be not used to endorse or promote products 
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
    GL_draw_range_elements support added by Benjamin Karaban
  
    Lev Povalahev contact information:
    
    levp@gmx.net

    http://www.levp.de/3d/
*/
/* These extensions are supported:
GL_3DFX_multisample
GL_3DFX_texture_compression_FXT1
GL_APPLE_client_storage
GL_APPLE_element_array
GL_APPLE_fence
GL_APPLE_specular_vector
GL_APPLE_transform_hint
GL_APPLE_vertex_array_object
GL_APPLE_vertex_array_range
GL_APPLE_ycbcr_422
GL_ARB_color_buffer_float
GL_ARB_depth_texture
GL_ARB_draw_buffers
GL_ARB_fragment_program
GL_ARB_fragment_program_shadow
GL_ARB_fragment_shader
GL_ARB_half_float_pixel
GL_ARB_imaging
GL_ARB_matrix_palette
GL_ARB_multisample
GL_ARB_multitexture
GL_ARB_occlusion_query
GL_ARB_point_parameters
GL_ARB_point_sprite
GL_ARB_shader_objects
GL_ARB_shading_language_100
GL_ARB_shadow
GL_ARB_shadow_ambient
GL_ARB_texture_border_clamp
GL_ARB_texture_compression
GL_ARB_texture_cube_map
GL_ARB_texture_env_add
GL_ARB_texture_env_combine
GL_ARB_texture_env_crossbar
GL_ARB_texture_env_dot3
GL_ARB_texture_mirrored_repeat
GL_ARB_texture_non_power_of_two
GL_ARB_texture_rectangle
GL_ARB_transpose_matrix
GL_ARB_vertex_blend
GL_ARB_vertex_buffer_object
GL_ARB_vertex_program
GL_ARB_vertex_shader
GL_ARB_window_pos
GL_ATIX_point_sprites
GL_ATIX_texture_env_combine3
GL_ATIX_texture_env_route
GL_ATIX_vertex_shader_output_point_size
GL_ATI_draw_buffers
GL_ATI_element_array
GL_ATI_envmap_bumpmap
GL_ATI_fragment_shader
GL_ATI_map_object_buffer
GL_ATI_pn_triangles
GL_ATI_separate_stencil
GL_ATI_text_fragment_shader
GL_ATI_texture_env_combine3
GL_ATI_texture_compression_3dc
GL_ATI_texture_float
GL_ATI_texture_mirror_once
GL_ATI_vertex_array_object
GL_ATI_vertex_attrib_array_object
GL_ATI_vertex_streams
GL_EXT_422_pixels
GL_EXT_abgr
GL_EXT_bgra
GL_EXT_blend_color
GL_EXT_blend_func_separate
GL_EXT_blend_logic_op
GL_EXT_blend_minmax
GL_EXT_blend_subtract
GL_EXT_clip_volume_hint
GL_EXT_cmyka
GL_EXT_color_matrix
GL_EXT_color_subtable
GL_EXT_compiled_vertex_array
GL_EXT_convolution
GL_EXT_coordinate_frame
GL_EXT_copy_texture
GL_EXT_cull_vertex
GL_EXT_depth_bounds_test
GL_EXT_draw_range_elements
GL_EXT_fog_coord
GL_EXT_framebuffer_object
GL_EXT_histogram
GL_EXT_index_array_formats
GL_EXT_index_func
GL_EXT_index_material
GL_EXT_index_texture
GL_EXT_light_texture
GL_EXT_misc_attribute
GL_EXT_multi_draw_arrays
GL_EXT_multisample
GL_EXT_packed_pixels
GL_EXT_paletted_texture
GL_EXT_pixel_transform
GL_EXT_pixel_transform_color_table
GL_EXT_point_parameters
GL_EXT_polygon_offset
GL_EXT_rescale_normal
GL_EXT_secondary_color
GL_EXT_separate_specular_color
GL_EXT_shadow_funcs
GL_EXT_shared_texture_palette
GL_EXT_stencil_two_side
GL_EXT_stencil_wrap
GL_EXT_subtexture
GL_EXT_texgen_reflection
GL_EXT_texture
GL_EXT_texture3D
GL_EXT_texture_compression_s3tc
GL_EXT_texture_cube_map
GL_EXT_texture_edge_clamp
GL_EXT_texture_env_add
GL_EXT_texture_env_combine
GL_EXT_texture_env_dot3
GL_EXT_texture_filter_anisotropic
GL_EXT_texture_lod
GL_EXT_texture_lod_bias
GL_EXT_texture_object
GL_EXT_texture_perturb_normal
GL_EXT_texture_rectangle
GL_EXT_vertex_array
GL_EXT_vertex_shader
GL_EXT_vertex_weighting
GL_FfdMaskSGIX
GL_HP_convolution_border_modes
GL_HP_image_transform
GL_HP_occlusion_test
GL_HP_texture_lighting
GL_IBM_cull_vertex
GL_IBM_multimode_draw_arrays
GL_IBM_rasterpos_clip
GL_IBM_texture_mirrored_repeat
GL_IBM_vertex_array_lists
GL_INGR_color_clamp
GL_INGR_interlace_read
GL_INGR_palette_buffer
GL_INTEL_parallel_arrays
GL_INTEL_texture_scissor
GL_KTX_buffer_region
GL_MESA_resize_buffers
GL_MESA_window_pos
GL_NVX_ycrcb
GL_NV_blend_square
GL_NV_copy_depth_to_color
GL_NV_depth_clamp
GL_NV_element_array
GL_NV_evaluators
GL_NV_fence
GL_NV_float_buffer
GL_NV_fog_distance
GL_NV_fragment_program
GL_NV_half_float
GL_NV_light_max_exponent
GL_NV_multisample_filter_hint
GL_NV_occlusion_query
GL_NV_packed_depth_stencil
GL_NV_pixel_data_range
GL_NV_point_sprite
GL_NV_primitive_restart
GL_NV_register_combiners
GL_NV_register_combiners2
GL_NV_texgen_emboss
GL_NV_texgen_reflection
GL_NV_texture_compression_vtc
GL_NV_texture_env_combine4
GL_NV_texture_expand_normal
GL_NV_texture_rectangle
GL_NV_texture_shader
GL_NV_texture_shader2
GL_NV_texture_shader3
GL_NV_vertex_array_range
GL_NV_vertex_array_range2
GL_NV_vertex_program
GL_NV_vertex_program1_1
GL_NV_vertex_program2
GL_OES_read_format
GL_OML_interlace
GL_OML_resample
GL_OML_subsample
GL_PGI_misc_hints
GL_PGI_vertex_hints
GL_REND_screen_coordinates
GL_S3_s3tc
GL_SGIS_detail_texture
GL_SGIS_fog_function
GL_SGIS_generate_mipmap
GL_SGIS_multisample
GL_SGIS_multitexture
GL_SGIS_pixel_texture
GL_SGIS_point_line_texgen
GL_SGIS_point_parameters
GL_SGIS_sharpen_texture
GL_SGIS_texture4D
GL_SGIS_texture_border_clamp
GL_SGIS_texture_color_mask
GL_SGIS_texture_edge_clamp
GL_SGIS_texture_filter4
GL_SGIS_texture_lod
GL_SGIS_texture_select
GL_SGIX_async
GL_SGIX_async_histogram
GL_SGIX_async_pixel
GL_SGIX_blend_alpha_minmax
GL_SGIX_calligraphic_fragment
GL_SGIX_clipmap
GL_SGIX_convolution_accuracy
GL_SGIX_depth_texture
GL_SGIX_flush_raster
GL_SGIX_fog_offset
GL_SGIX_fog_scale
GL_SGIX_fragment_lighting
GL_SGIX_framezoom
GL_SGIX_impact_pixel_texture
GL_SGIX_instruments
GL_SGIX_interlace
GL_SGIX_ir_instrument1
GL_SGIX_list_priority
GL_SGIX_pixel_texture
GL_SGIX_pixel_tiles
GL_SGIX_polynomial_ffd
GL_SGIX_reference_plane
GL_SGIX_resample
GL_SGIX_scalebias_hint
GL_SGIX_shadow
GL_SGIX_shadow_ambient
GL_SGIX_sprite
GL_SGIX_subsample
GL_SGIX_tag_sample_buffer
GL_SGIX_texture_add_env
GL_SGIX_texture_coordinate_clamp
GL_SGIX_texture_lod_bias
GL_SGIX_texture_multi_buffer
GL_SGIX_texture_scale_bias
GL_SGIX_vertex_preclip
GL_SGIX_ycrcb
GL_SGIX_ycrcb_subsample
GL_SGIX_ycrcba
GL_SGI_color_matrix
GL_SGI_color_table
GL_SGI_depth_pass_instrument
GL_SGI_texture_color_table
GL_SUNX_constant_data
GL_SUN_convolution_border_modes
GL_SUN_global_alpha
GL_SUN_mesh_array
GL_SUN_multi_draw_arrays
GL_SUN_slice_accum
GL_SUN_triangle_list
GL_SUN_vertex
GL_WIN_phong_shading
GL_WIN_specular_fog
GL_WIN_swap_hint
WGL_3DFX_multisample
WGL_ARB_buffer_region
WGL_ARB_extensions_string
WGL_ARB_make_current_read
WGL_ARB_multisample
WGL_ARB_pbuffer
WGL_ARB_pixel_format
WGL_ARB_pixel_format_float
WGL_ARB_render_texture
WGL_ATI_pixel_format_float
WGL_EXT_depth_float
WGL_EXT_display_color_table
WGL_EXT_extensions_string
WGL_EXT_make_current_read
WGL_EXT_multisample
WGL_EXT_pbuffer
WGL_EXT_pixel_format
WGL_EXT_swap_control
WGL_I3D_digital_video_control
WGL_I3D_gamma
WGL_I3D_genlock
WGL_I3D_image_buffer
WGL_I3D_swap_frame_lock
WGL_I3D_swap_frame_usage
WGL_NV_float_buffer
WGL_NV_render_depth_texture
WGL_NV_render_texture_rectangle
*/


/*
07.12.05 Changes based on GL_GLEXT_VERSION 29
	New defines, and defines fix in GL_EXT_framebuffer_object
	Added
	GL_ATI_texture_compression_3dc

05.19.05 Added
	GL_EXT_framebuffer_object
	WGL_I3D_swap_frame_usage

VERSION 2.0

CHANGES based on GL_GLEXT_VERSION 26, WGL_WGLEXT_VERSION 6 and GLX_GLXEXT_VERSION 10

02.01.05 Added
	OpenGL 2.0 Support
	GL_ARB_color_buffer_float
	GL_ARB_half_float_pixel
	GL_ARB_texture_rectangle
	GL_ATI_vertex_attrib_array_object
	GL_OES_read_format
	WGL_EXT_display_color_table
	WGL_ARB_pixel_format_float

VERSION 1.5

CHANGES

08.30.04 Added
	GL_ARB_draw_buffers

VERSION 1.05

05.26.04 Added
	GL_ARB_fragment_program_shadow
	GL_ARB_fragment_shader
	GL_ARB_point_sprite
	GL_ARB_shader_objects
	GL_ARB_shading_language_100
	GL_ARB_texture_non_power_of_two
	GL_ARB_vertex_shader

03.07.03 Added
	GL_3DFX_multisample
	GL_3DFX_texture_compression_FXT1
	GL_APPLE_client_storage
	GL_APPLE_element_array
	GL_APPLE_fence
	GL_APPLE_specular_vector
	GL_APPLE_transform_hint
	GL_APPLE_vertex_array_object
	GL_APPLE_vertex_array_range
	GL_APPLE_ycbcr_422
	GL_ATIX_texture_env_combine3
	GL_ATIX_vertex_shader_output_point_size
	GL_EXT_422_pixels
	GL_EXT_blend_logic_op
	GL_EXT_clip_volume_hint
	GL_EXT_cmyka
	GL_EXT_color_matrix
	GL_EXT_color_subtable
	GL_EXT_convolution
	GL_EXT_coordinate_frame
	GL_EXT_copy_texture
	GL_EXT_histogram
	GL_EXT_index_array_formats
	GL_EXT_index_func
	GL_EXT_index_material
	GL_EXT_index_texture
	GL_EXT_light_texture
	GL_EXT_misc_attribute
	GL_EXT_multisample
	GL_EXT_packed_pixels
	GL_EXT_paletted_texture
	GL_EXT_pixel_transform
	GL_EXT_pixel_transform_color_table
	GL_EXT_polygon_offset
	GL_EXT_rescale_normal
	GL_EXT_shared_texture_palette
	GL_EXT_subtexture
	GL_EXT_texgen_reflection
	GL_EXT_texture
	GL_EXT_texture3D
	GL_EXT_texture_cube_map
	GL_EXT_texture_edge_clamp
	GL_EXT_texture_env_add
	GL_EXT_texture_env_dot3
	GL_EXT_texture_lod
	GL_EXT_texture_object
	GL_EXT_texture_perturb_normal
	GL_EXT_vertex_array
	GL_FfdMaskSGIX
	GL_HP_convolution_border_modes
	GL_HP_image_transform
	GL_HP_texture_lighting
	GL_IBM_cull_vertex
	GL_IBM_multimode_draw_arrays
	GL_IBM_rasterpos_clip
	GL_IBM_texture_mirrored_repeat
	GL_IBM_vertex_array_lists
	GL_INGR_color_clamp
	GL_INGR_interlace_read
	GL_INGR_palette_buffer
	GL_INTEL_parallel_arrays
	GL_INTEL_texture_scissor
	GL_KTX_buffer_region
	GL_MESA_resize_buffers
	GL_MESA_window_pos
	GL_NVX_ycrcb
	GL_NV_multisample_filter_hint
	GL_NV_texgen_emboss
	GL_NV_texture_compression_vtc
	GL_OML_interlace
	GL_OML_resample
	GL_OML_subsample
	GL_PGI_misc_hints
	GL_PGI_vertex_hints
	GL_REND_screen_coordinates
	GL_S3_s3tc
	GL_SGIS_detail_texture
	GL_SGIS_fog_function
	GL_SGIS_multisample
	GL_SGIS_multitexture
	GL_SGIS_pixel_texture
	GL_SGIS_point_line_texgen
	GL_SGIS_point_parameters
	GL_SGIS_sharpen_texture
	GL_SGIS_texture4D
	GL_SGIS_texture_border_clamp
	GL_SGIS_texture_color_mask
	GL_SGIS_texture_edge_clamp
	GL_SGIS_texture_filter4
	GL_SGIS_texture_lod
	GL_SGIS_texture_select
	GL_SGIX_async
	GL_SGIX_async_histogram
	GL_SGIX_async_pixel
	GL_SGIX_blend_alpha_minmax
	GL_SGIX_calligraphic_fragment
	GL_SGIX_clipmap
	GL_SGIX_convolution_accuracy
	GL_SGIX_flush_raster
	GL_SGIX_fog_offset
	GL_SGIX_fog_scale
	GL_SGIX_fragment_lighting
	GL_SGIX_framezoom
	GL_SGIX_impact_pixel_texture
	GL_SGIX_instruments
	GL_SGIX_interlace
	GL_SGIX_ir_instrument1
	GL_SGIX_list_priority
	GL_SGIX_pixel_texture
	GL_SGIX_pixel_tiles
	GL_SGIX_polynomial_ffd
	GL_SGIX_reference_plane
	GL_SGIX_resample
	GL_SGIX_scalebias_hint
	GL_SGIX_shadow_ambient
	GL_SGIX_sprite
	GL_SGIX_subsample
	GL_SGIX_tag_sample_buffer
	GL_SGIX_texture_add_env
	GL_SGIX_texture_coordinate_clamp
	GL_SGIX_texture_lod_bias
	GL_SGIX_texture_multi_buffer
	GL_SGIX_texture_scale_bias
	GL_SGIX_vertex_preclip
	GL_SGIX_ycrcb
	GL_SGIX_ycrcb_subsample
	GL_SGIX_ycrcba
	GL_SGI_color_matrix
	GL_SGI_color_table
	GL_SGI_depth_pass_instrument
	GL_SGI_texture_color_table
	GL_SUNX_constant_data
	GL_SUN_convolution_border_modes
	GL_SUN_global_alpha
	GL_SUN_mesh_array
	GL_SUN_multi_draw_arrays
	GL_SUN_slice_accum
	GL_SUN_triangle_list
	GL_SUN_vertex
	GL_WIN_phong_shading
	GL_WIN_specular_fog
	GL_WIN_swap_hint
	WGL_3DFX_multisample
	WGL_EXT_make_current_read
	WGL_EXT_multisample
	WGL_EXT_pbuffer
	WGL_EXT_pixel_format
	WGL_I3D_digital_video_control
	WGL_I3D_gamma
	WGL_I3D_genlock
	WGL_I3D_image_buffer
	WGL_I3D_swap_frame_lock
	WGL_NV_float_buffer

	- Alphabetical ordering of most of the extensions
	- Added "GL_" in the extension headers names (cause it was confusing enough)
	- QueryExtension returns a boolean instead of an int
	- Added void extgl_DisplayValues(bool bto_a_file) gives you information on the current OpenGl driver. In a file or in the debug console.

VERSION 1.04

08.06.03 Added GL_EXT_depth_bounds_test

01.06.03 Added GL_EXT_texture_rectangle

13.04.03 Fixed a typo in ARBvbo

24.03.03 I'm starting version numbering and protocolling the changes here from this release on. 
This version will be 1.01. The additions in this version will be 
ARB_vertex_buffer_object and hopefully cygwin support. Fixed a typo in a license.
*/
#ifndef __EXTGL_H__
#define __EXTGL_H__

/*-----------------------------------------*/
/*-----------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) && !defined(APIENTRY)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __gl_h_
#error extgl.h should be included before gl.h
#endif
#ifdef __GL_H_
#error extgl.h should be included before gl.h
#endif

#ifndef WINGDIAPI
#define WINGDIAPI
#endif

#define __glext_h_
#define __GLEXT_H_

#define __gl_h_
#define __GL_H__

#ifndef APIENTRY
#define APIENTRY
#endif

#define GLAPI extern
#define GLAPIENTRY __stdcall

#ifdef __cplusplus
extern "C" {
#endif

/* stuff for NV_half_float extension (datatype and conversion functions) 
   I hope this type will be reused for other 2 byte float extensions */

union ieee_half /* ieee-like 16 bit float */ 
{
    unsigned short bits;
    struct 
    {
        unsigned long m : 10; /* mantissa */
        unsigned long e : 5;  /* exponent */
        unsigned long s : 1;  /* sign */
    } ieee;
};

union ieee_single /* ieee-754 single floating point type */
{
    float f;
    struct 
    {
        unsigned long m : 23;  /* mantissa */
        unsigned long e : 8;   /* exponent */
        unsigned long s : 1;   /* sign */
    } ieee;
};

/* the actual typedef */

typedef unsigned short GLhalf;

/* conversion functions */

/* half to float */
__inline static float htof(GLhalf val) 
{
    union ieee_half h;
    union ieee_single sng;
    h.bits = val;
    sng.ieee.s = h.ieee.s;

    /* special cases */
    if ((h.ieee.e==0) && (h.ieee.m==0)) 
    {  /* zero */
        sng.ieee.m=0;
        sng.ieee.e=0;
    }
    else if ((h.ieee.e==0) && (h.ieee.m!=0)) 
    { /* denorm -- denorm half will fit in non-denorm single */
        const float half_denorm = (1.0f/16384.0f); // 2^-14
        float mantissa = ((float)(h.ieee.m)) / 1024.0f;
        float sgn = (h.ieee.s)? -1.0f :1.0f;
        sng.f = sgn*mantissa*half_denorm;
    }
    else if ((h.ieee.e==31) && (h.ieee.m==0)) 
    { /* infinity */
        sng.ieee.e = 0xff;
        sng.ieee.m = 0;
    }
    else if ((h.ieee.e==31) && (h.ieee.m!=0)) 
    { /* NaN */
        sng.ieee.e = 0xff;
        sng.ieee.m = 1;
    }
    else 
    {
        sng.ieee.e = h.ieee.e+112;
        sng.ieee.m = (h.ieee.m << 13);
    }

    return sng.f;
}

/* float to half */
__inline static GLhalf ftoh(float val) 
{
    union ieee_single f;
    union ieee_half h;
    f.f = val;
    
    h.ieee.s = f.ieee.s;

    /* special cases */


    if ((f.ieee.e==0) && (f.ieee.m==0)) 
    { /* zero */
        h.ieee.m = 0;
        h.ieee.e = 0;
    }
    else if ((f.ieee.e==0) && (f.ieee.m!=0)) 
    { /* denorm -- denorm float maps to 0 half */
        h.ieee.m = 0;
        h.ieee.e = 0;
    }           
    else if ((f.ieee.e==0xff) && (f.ieee.m==0)) 
    { /* infinity */
        h.ieee.m = 0;
        h.ieee.e = 31;
    }
    else if ((f.ieee.e==0xff) && (f.ieee.m!=0)) 
    { /* NaN */
        h.ieee.m = 1;
        h.ieee.e = 31;
    }
    else 
    { /* normal case */
        int new_exp = f.ieee.e-127;
        if (new_exp<-24) 
        { /* half can't map such numbers */
            h.ieee.m = 0;
            h.ieee.e = 0;
        }

        if (new_exp<-14) 
        { /* this maps to a denorm */
            unsigned int exp_val = (unsigned int) (-14 - new_exp); // 2^-exp_val
            h.ieee.e = 0;
            switch (exp_val) 
            {
            case 0: h.ieee.m = 0; break; /* logical error */
            case 1: h.ieee.m = 512 + (f.ieee.m>>14); break;
            case 2: h.ieee.m = 256 + (f.ieee.m>>15); break;
            case 3: h.ieee.m = 128 + (f.ieee.m>>16); break;
            case 4: h.ieee.m = 64 + (f.ieee.m>>17); break;
            case 5: h.ieee.m = 32 + (f.ieee.m>>18); break;
            case 6: h.ieee.m = 16 + (f.ieee.m>>19); break;
            case 7: h.ieee.m = 8 + (f.ieee.m>>20); break;
            case 8: h.ieee.m = 4 + (f.ieee.m>>21); break;
            case 9: h.ieee.m = 2 + (f.ieee.m>>22); break;
            case 10: h.ieee.m = 1; break;
            }
        }
        else if (new_exp>15) 
        { /* map this value to infinity */
            h.ieee.m = 0;
            h.ieee.e = 31;
        }
        else 
        {
            h.ieee.e = new_exp+15;
            h.ieee.m = (f.ieee.m >> 13);
        }
    }
return h.bits;
}

/* OpenGL 1.1 definition */

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

#if !defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#include <GL/glx.h>
#endif /* _WIN32 */

/* for mingw compatibility */
typedef void (*_GLfuncptr)();

#define GL_VERSION_1_1                                          1
#define GL_ACCUM                                                0x0100
#define GL_LOAD                                                 0x0101
#define GL_RETURN                                               0x0102
#define GL_MULT                                                 0x0103
#define GL_ADD                                                  0x0104
#define GL_NEVER                                                0x0200
#define GL_LESS                                                 0x0201
#define GL_EQUAL                                                0x0202
#define GL_LEQUAL                                               0x0203
#define GL_GREATER                                              0x0204
#define GL_NOTEQUAL                                             0x0205
#define GL_GEQUAL                                               0x0206
#define GL_ALWAYS                                               0x0207
#define GL_CURRENT_BIT                                          0x00000001
#define GL_POINT_BIT                                            0x00000002
#define GL_LINE_BIT                                             0x00000004
#define GL_POLYGON_BIT                                          0x00000008
#define GL_POLYGON_STIPPLE_BIT                                  0x00000010
#define GL_PIXEL_MODE_BIT                                       0x00000020
#define GL_LIGHTING_BIT                                         0x00000040
#define GL_FOG_BIT                                              0x00000080
#define GL_DEPTH_BUFFER_BIT                                     0x00000100
#define GL_ACCUM_BUFFER_BIT                                     0x00000200
#define GL_STENCIL_BUFFER_BIT                                   0x00000400
#define GL_VIEWPORT_BIT                                         0x00000800
#define GL_TRANSFORM_BIT                                        0x00001000
#define GL_ENABLE_BIT                                           0x00002000
#define GL_COLOR_BUFFER_BIT                                     0x00004000
#define GL_HINT_BIT                                             0x00008000
#define GL_EVAL_BIT                                             0x00010000
#define GL_LIST_BIT                                             0x00020000
#define GL_TEXTURE_BIT                                          0x00040000
#define GL_SCISSOR_BIT                                          0x00080000
#define GL_ALL_ATTRIB_BITS                                      0x000fffff
#define GL_POINTS                                               0x0000
#define GL_LINES                                                0x0001
#define GL_LINE_LOOP                                            0x0002
#define GL_LINE_STRIP                                           0x0003
#define GL_TRIANGLES                                            0x0004
#define GL_TRIANGLE_STRIP                                       0x0005
#define GL_TRIANGLE_FAN                                         0x0006
#define GL_QUADS                                                0x0007
#define GL_QUAD_STRIP                                           0x0008
#define GL_POLYGON                                              0x0009
#define GL_ZERO                                                 0
#define GL_ONE                                                  1
#define GL_SRC_COLOR                                            0x0300
#define GL_ONE_MINUS_SRC_COLOR                                  0x0301
#define GL_SRC_ALPHA                                            0x0302
#define GL_ONE_MINUS_SRC_ALPHA                                  0x0303
#define GL_DST_ALPHA                                            0x0304
#define GL_ONE_MINUS_DST_ALPHA                                  0x0305
#define GL_DST_COLOR                                            0x0306
#define GL_ONE_MINUS_DST_COLOR                                  0x0307
#define GL_SRC_ALPHA_SATURATE                                   0x0308
#define GL_TRUE                                                 1
#define GL_FALSE                                                0
#define GL_CLIP_PLANE0                                          0x3000
#define GL_CLIP_PLANE1                                          0x3001
#define GL_CLIP_PLANE2                                          0x3002
#define GL_CLIP_PLANE3                                          0x3003
#define GL_CLIP_PLANE4                                          0x3004
#define GL_CLIP_PLANE5                                          0x3005
#define GL_BYTE                                                 0x1400
#define GL_UNSIGNED_BYTE                                        0x1401
#define GL_SHORT                                                0x1402
#define GL_UNSIGNED_SHORT                                       0x1403
#define GL_INT                                                  0x1404
#define GL_UNSIGNED_INT                                         0x1405
#define GL_FLOAT                                                0x1406
#define GL_2_BYTES                                              0x1407
#define GL_3_BYTES                                              0x1408
#define GL_4_BYTES                                              0x1409
#define GL_DOUBLE                                               0x140A
#define GL_NONE                                                 0
#define GL_FRONT_LEFT                                           0x0400
#define GL_FRONT_RIGHT                                          0x0401
#define GL_BACK_LEFT                                            0x0402
#define GL_BACK_RIGHT                                           0x0403
#define GL_FRONT                                                0x0404
#define GL_BACK                                                 0x0405
#define GL_LEFT                                                 0x0406
#define GL_RIGHT                                                0x0407
#define GL_FRONT_AND_BACK                                       0x0408
#define GL_AUX0                                                 0x0409
#define GL_AUX1                                                 0x040A
#define GL_AUX2                                                 0x040B
#define GL_AUX3                                                 0x040C
#define GL_NO_ERROR                                             0
#define GL_INVALID_ENUM                                         0x0500
#define GL_INVALID_VALUE                                        0x0501
#define GL_INVALID_OPERATION                                    0x0502
#define GL_STACK_OVERFLOW                                       0x0503
#define GL_STACK_UNDERFLOW                                      0x0504
#define GL_OUT_OF_MEMORY                                        0x0505
#define GL_2D                                                   0x0600
#define GL_3D                                                   0x0601
#define GL_3D_COLOR                                             0x0602
#define GL_3D_COLOR_TEXTURE                                     0x0603
#define GL_4D_COLOR_TEXTURE                                     0x0604
#define GL_PASS_THROUGH_TOKEN                                   0x0700
#define GL_POINT_TOKEN                                          0x0701
#define GL_LINE_TOKEN                                           0x0702
#define GL_POLYGON_TOKEN                                        0x0703
#define GL_BITMAP_TOKEN                                         0x0704
#define GL_DRAW_PIXEL_TOKEN                                     0x0705
#define GL_COPY_PIXEL_TOKEN                                     0x0706
#define GL_LINE_RESET_TOKEN                                     0x0707
#define GL_EXP                                                  0x0800
#define GL_EXP2                                                 0x0801
#define GL_CW                                                   0x0900
#define GL_CCW                                                  0x0901
#define GL_COEFF                                                0x0A00
#define GL_ORDER                                                0x0A01
#define GL_DOMAIN                                               0x0A02
#define GL_CURRENT_COLOR                                        0x0B00
#define GL_CURRENT_INDEX                                        0x0B01
#define GL_CURRENT_NORMAL                                       0x0B02
#define GL_CURRENT_TEXTURE_COORDS                               0x0B03
#define GL_CURRENT_RASTER_COLOR                                 0x0B04
#define GL_CURRENT_RASTER_INDEX                                 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS                        0x0B06
#define GL_CURRENT_RASTER_POSITION                              0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID                        0x0B08
#define GL_CURRENT_RASTER_DISTANCE                              0x0B09
#define GL_POINT_SMOOTH                                         0x0B10
#define GL_POINT_SIZE                                           0x0B11
#define GL_POINT_SIZE_RANGE                                     0x0B12
#define GL_POINT_SIZE_GRANULARITY                               0x0B13
#define GL_LINE_SMOOTH                                          0x0B20
#define GL_LINE_WIDTH                                           0x0B21
#define GL_LINE_WIDTH_RANGE                                     0x0B22
#define GL_LINE_WIDTH_GRANULARITY                               0x0B23
#define GL_LINE_STIPPLE                                         0x0B24
#define GL_LINE_STIPPLE_PATTERN                                 0x0B25
#define GL_LINE_STIPPLE_REPEAT                                  0x0B26
#define GL_LIST_MODE                                            0x0B30
#define GL_MAX_LIST_NESTING                                     0x0B31
#define GL_LIST_BASE                                            0x0B32
#define GL_LIST_INDEX                                           0x0B33
#define GL_POLYGON_MODE                                         0x0B40
#define GL_POLYGON_SMOOTH                                       0x0B41
#define GL_POLYGON_STIPPLE                                      0x0B42
#define GL_EDGE_FLAG                                            0x0B43
#define GL_CULL_FACE                                            0x0B44
#define GL_CULL_FACE_MODE                                       0x0B45
#define GL_FRONT_FACE                                           0x0B46
#define GL_LIGHTING                                             0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER                             0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE                                 0x0B52
#define GL_LIGHT_MODEL_AMBIENT                                  0x0B53
#define GL_SHADE_MODEL                                          0x0B54
#define GL_COLOR_MATERIAL_FACE                                  0x0B55
#define GL_COLOR_MATERIAL_PARAMETER                             0x0B56
#define GL_COLOR_MATERIAL                                       0x0B57
#define GL_FOG                                                  0x0B60
#define GL_FOG_INDEX                                            0x0B61
#define GL_FOG_DENSITY                                          0x0B62
#define GL_FOG_START                                            0x0B63
#define GL_FOG_END                                              0x0B64
#define GL_FOG_MODE                                             0x0B65
#define GL_FOG_COLOR                                            0x0B66
#define GL_DEPTH_RANGE                                          0x0B70
#define GL_DEPTH_TEST                                           0x0B71
#define GL_DEPTH_WRITEMASK                                      0x0B72
#define GL_DEPTH_CLEAR_VALUE                                    0x0B73
#define GL_DEPTH_FUNC                                           0x0B74
#define GL_ACCUM_CLEAR_VALUE                                    0x0B80
#define GL_STENCIL_TEST                                         0x0B90
#define GL_STENCIL_CLEAR_VALUE                                  0x0B91
#define GL_STENCIL_FUNC                                         0x0B92
#define GL_STENCIL_VALUE_MASK                                   0x0B93
#define GL_STENCIL_FAIL                                         0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL                              0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS                              0x0B96
#define GL_STENCIL_REF                                          0x0B97
#define GL_STENCIL_WRITEMASK                                    0x0B98
#define GL_MATRIX_MODE                                          0x0BA0
#define GL_NORMALIZE                                            0x0BA1
#define GL_VIEWPORT                                             0x0BA2
#define GL_MODELVIEW_STACK_DEPTH                                0x0BA3
#define GL_PROJECTION_STACK_DEPTH                               0x0BA4
#define GL_TEXTURE_STACK_DEPTH                                  0x0BA5
#define GL_MODELVIEW_MATRIX                                     0x0BA6
#define GL_PROJECTION_MATRIX                                    0x0BA7
#define GL_TEXTURE_MATRIX                                       0x0BA8
#define GL_ATTRIB_STACK_DEPTH                                   0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH                            0x0BB1
#define GL_ALPHA_TEST                                           0x0BC0
#define GL_ALPHA_TEST_FUNC                                      0x0BC1
#define GL_ALPHA_TEST_REF                                       0x0BC2
#define GL_DITHER                                               0x0BD0
#define GL_BLEND_DST                                            0x0BE0
#define GL_BLEND_SRC                                            0x0BE1
#define GL_BLEND                                                0x0BE2
#define GL_LOGIC_OP_MODE                                        0x0BF0
#define GL_INDEX_LOGIC_OP                                       0x0BF1
#define GL_COLOR_LOGIC_OP                                       0x0BF2
#define GL_AUX_BUFFERS                                          0x0C00
#define GL_DRAW_BUFFER                                          0x0C01
#define GL_READ_BUFFER                                          0x0C02
#define GL_SCISSOR_BOX                                          0x0C10
#define GL_SCISSOR_TEST                                         0x0C11
#define GL_INDEX_CLEAR_VALUE                                    0x0C20
#define GL_INDEX_WRITEMASK                                      0x0C21
#define GL_COLOR_CLEAR_VALUE                                    0x0C22
#define GL_COLOR_WRITEMASK                                      0x0C23
#define GL_INDEX_MODE                                           0x0C30
#define GL_RGBA_MODE                                            0x0C31
#define GL_DOUBLEBUFFER                                         0x0C32
#define GL_STEREO                                               0x0C33
#define GL_RENDER_MODE                                          0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT                          0x0C50
#define GL_POINT_SMOOTH_HINT                                    0x0C51
#define GL_LINE_SMOOTH_HINT                                     0x0C52
#define GL_POLYGON_SMOOTH_HINT                                  0x0C53
#define GL_FOG_HINT                                             0x0C54
#define GL_TEXTURE_GEN_S                                        0x0C60
#define GL_TEXTURE_GEN_T                                        0x0C61
#define GL_TEXTURE_GEN_R                                        0x0C62
#define GL_TEXTURE_GEN_Q                                        0x0C63
#define GL_PIXEL_MAP_I_TO_I                                     0x0C70
#define GL_PIXEL_MAP_S_TO_S                                     0x0C71
#define GL_PIXEL_MAP_I_TO_R                                     0x0C72
#define GL_PIXEL_MAP_I_TO_G                                     0x0C73
#define GL_PIXEL_MAP_I_TO_B                                     0x0C74
#define GL_PIXEL_MAP_I_TO_A                                     0x0C75
#define GL_PIXEL_MAP_R_TO_R                                     0x0C76
#define GL_PIXEL_MAP_G_TO_G                                     0x0C77
#define GL_PIXEL_MAP_B_TO_B                                     0x0C78
#define GL_PIXEL_MAP_A_TO_A                                     0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE                                0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE                                0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE                                0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE                                0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE                                0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE                                0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE                                0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE                                0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE                                0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE                                0x0CB9
#define GL_UNPACK_SWAP_BYTES                                    0x0CF0
#define GL_UNPACK_LSB_FIRST                                     0x0CF1
#define GL_UNPACK_ROW_LENGTH                                    0x0CF2
#define GL_UNPACK_SKIP_ROWS                                     0x0CF3
#define GL_UNPACK_SKIP_PIXELS                                   0x0CF4
#define GL_UNPACK_ALIGNMENT                                     0x0CF5
#define GL_PACK_SWAP_BYTES                                      0x0D00
#define GL_PACK_LSB_FIRST                                       0x0D01
#define GL_PACK_ROW_LENGTH                                      0x0D02
#define GL_PACK_SKIP_ROWS                                       0x0D03
#define GL_PACK_SKIP_PIXELS                                     0x0D04
#define GL_PACK_ALIGNMENT                                       0x0D05
#define GL_MAP_COLOR                                            0x0D10
#define GL_MAP_STENCIL                                          0x0D11
#define GL_INDEX_SHIFT                                          0x0D12
#define GL_INDEX_OFFSET                                         0x0D13
#define GL_RED_SCALE                                            0x0D14
#define GL_RED_BIAS                                             0x0D15
#define GL_ZOOM_X                                               0x0D16
#define GL_ZOOM_Y                                               0x0D17
#define GL_GREEN_SCALE                                          0x0D18
#define GL_GREEN_BIAS                                           0x0D19
#define GL_BLUE_SCALE                                           0x0D1A
#define GL_BLUE_BIAS                                            0x0D1B
#define GL_ALPHA_SCALE                                          0x0D1C
#define GL_ALPHA_BIAS                                           0x0D1D
#define GL_DEPTH_SCALE                                          0x0D1E
#define GL_DEPTH_BIAS                                           0x0D1F
#define GL_MAX_EVAL_ORDER                                       0x0D30
#define GL_MAX_LIGHTS                                           0x0D31
#define GL_MAX_CLIP_PLANES                                      0x0D32
#define GL_MAX_TEXTURE_SIZE                                     0x0D33
#define GL_MAX_PIXEL_MAP_TABLE                                  0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH                               0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH                            0x0D36
#define GL_MAX_NAME_STACK_DEPTH                                 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH                           0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH                              0x0D39
#define GL_MAX_VIEWPORT_DIMS                                    0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH                        0x0D3B
#define GL_SUBPIXEL_BITS                                        0x0D50
#define GL_INDEX_BITS                                           0x0D51
#define GL_RED_BITS                                             0x0D52
#define GL_GREEN_BITS                                           0x0D53
#define GL_BLUE_BITS                                            0x0D54
#define GL_ALPHA_BITS                                           0x0D55
#define GL_DEPTH_BITS                                           0x0D56
#define GL_STENCIL_BITS                                         0x0D57
#define GL_ACCUM_RED_BITS                                       0x0D58
#define GL_ACCUM_GREEN_BITS                                     0x0D59
#define GL_ACCUM_BLUE_BITS                                      0x0D5A
#define GL_ACCUM_ALPHA_BITS                                     0x0D5B
#define GL_NAME_STACK_DEPTH                                     0x0D70
#define GL_AUTO_NORMAL                                          0x0D80
#define GL_MAP1_COLOR_4                                         0x0D90
#define GL_MAP1_INDEX                                           0x0D91
#define GL_MAP1_NORMAL                                          0x0D92
#define GL_MAP1_TEXTURE_COORD_1                                 0x0D93
#define GL_MAP1_TEXTURE_COORD_2                                 0x0D94
#define GL_MAP1_TEXTURE_COORD_3                                 0x0D95
#define GL_MAP1_TEXTURE_COORD_4                                 0x0D96
#define GL_MAP1_VERTEX_3                                        0x0D97
#define GL_MAP1_VERTEX_4                                        0x0D98
#define GL_MAP2_COLOR_4                                         0x0DB0
#define GL_MAP2_INDEX                                           0x0DB1
#define GL_MAP2_NORMAL                                          0x0DB2
#define GL_MAP2_TEXTURE_COORD_1                                 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2                                 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3                                 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4                                 0x0DB6
#define GL_MAP2_VERTEX_3                                        0x0DB7
#define GL_MAP2_VERTEX_4                                        0x0DB8
#define GL_MAP1_GRID_DOMAIN                                     0x0DD0
#define GL_MAP1_GRID_SEGMENTS                                   0x0DD1
#define GL_MAP2_GRID_DOMAIN                                     0x0DD2
#define GL_MAP2_GRID_SEGMENTS                                   0x0DD3
#define GL_TEXTURE_1D                                           0x0DE0
#define GL_TEXTURE_2D                                           0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER                              0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE                                 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE                                 0x0DF2
#define GL_SELECTION_BUFFER_POINTER                             0x0DF3
#define GL_SELECTION_BUFFER_SIZE                                0x0DF4
#define GL_TEXTURE_WIDTH                                        0x1000
#define GL_TEXTURE_HEIGHT                                       0x1001
#define GL_TEXTURE_INTERNAL_FORMAT                              0x1003
#define GL_TEXTURE_BORDER_COLOR                                 0x1004
#define GL_TEXTURE_BORDER                                       0x1005
#define GL_DONT_CARE                                            0x1100
#define GL_FASTEST                                              0x1101
#define GL_NICEST                                               0x1102
#define GL_LIGHT0                                               0x4000
#define GL_LIGHT1                                               0x4001
#define GL_LIGHT2                                               0x4002
#define GL_LIGHT3                                               0x4003
#define GL_LIGHT4                                               0x4004
#define GL_LIGHT5                                               0x4005
#define GL_LIGHT6                                               0x4006
#define GL_LIGHT7                                               0x4007
#define GL_AMBIENT                                              0x1200
#define GL_DIFFUSE                                              0x1201
#define GL_SPECULAR                                             0x1202
#define GL_POSITION                                             0x1203
#define GL_SPOT_DIRECTION                                       0x1204
#define GL_SPOT_EXPONENT                                        0x1205
#define GL_SPOT_CUTOFF                                          0x1206
#define GL_CONSTANT_ATTENUATION                                 0x1207
#define GL_LINEAR_ATTENUATION                                   0x1208
#define GL_QUADRATIC_ATTENUATION                                0x1209
#define GL_COMPILE                                              0x1300
#define GL_COMPILE_AND_EXECUTE                                  0x1301
#define GL_CLEAR                                                0x1500
#define GL_AND                                                  0x1501
#define GL_AND_REVERSE                                          0x1502
#define GL_COPY                                                 0x1503
#define GL_AND_INVERTED                                         0x1504
#define GL_NOOP                                                 0x1505
#define GL_XOR                                                  0x1506
#define GL_OR                                                   0x1507
#define GL_NOR                                                  0x1508
#define GL_EQUIV                                                0x1509
#define GL_INVERT                                               0x150A
#define GL_OR_REVERSE                                           0x150B
#define GL_COPY_INVERTED                                        0x150C
#define GL_OR_INVERTED                                          0x150D
#define GL_NAND                                                 0x150E
#define GL_SET                                                  0x150F
#define GL_EMISSION                                             0x1600
#define GL_SHININESS                                            0x1601
#define GL_AMBIENT_AND_DIFFUSE                                  0x1602
#define GL_COLOR_INDEXES                                        0x1603
#define GL_MODELVIEW                                            0x1700
#define GL_PROJECTION                                           0x1701
#define GL_TEXTURE                                              0x1702
#define GL_COLOR                                                0x1800
#define GL_DEPTH                                                0x1801
#define GL_STENCIL                                              0x1802
#define GL_COLOR_INDEX                                          0x1900
#define GL_STENCIL_INDEX                                        0x1901
#define GL_DEPTH_COMPONENT                                      0x1902
#define GL_RED                                                  0x1903
#define GL_GREEN                                                0x1904
#define GL_BLUE                                                 0x1905
#define GL_ALPHA                                                0x1906
#define GL_RGB                                                  0x1907
#define GL_RGBA                                                 0x1908
#define GL_LUMINANCE                                            0x1909
#define GL_LUMINANCE_ALPHA                                      0x190A
#define GL_BITMAP                                               0x1A00
#define GL_POINT                                                0x1B00
#define GL_LINE                                                 0x1B01
#define GL_FILL                                                 0x1B02
#define GL_RENDER                                               0x1C00
#define GL_FEEDBACK                                             0x1C01
#define GL_SELECT                                               0x1C02
#define GL_FLAT                                                 0x1D00
#define GL_SMOOTH                                               0x1D01
#define GL_KEEP                                                 0x1E00
#define GL_REPLACE                                              0x1E01
#define GL_INCR                                                 0x1E02
#define GL_DECR                                                 0x1E03
#define GL_VENDOR                                               0x1F00
#define GL_RENDERER                                             0x1F01
#define GL_VERSION                                              0x1F02
#define GL_EXTENSIONS                                           0x1F03
#define GL_S                                                    0x2000
#define GL_T                                                    0x2001
#define GL_R                                                    0x2002
#define GL_Q                                                    0x2003
#define GL_MODULATE                                             0x2100
#define GL_DECAL                                                0x2101
#define GL_TEXTURE_ENV_MODE                                     0x2200
#define GL_TEXTURE_ENV_COLOR                                    0x2201
#define GL_TEXTURE_ENV                                          0x2300
#define GL_EYE_LINEAR                                           0x2400
#define GL_OBJECT_LINEAR                                        0x2401
#define GL_SPHERE_MAP                                           0x2402
#define GL_TEXTURE_GEN_MODE                                     0x2500
#define GL_OBJECT_PLANE                                         0x2501
#define GL_EYE_PLANE                                            0x2502
#define GL_NEAREST                                              0x2600
#define GL_LINEAR                                               0x2601
#define GL_NEAREST_MIPMAP_NEAREST                               0x2700
#define GL_LINEAR_MIPMAP_NEAREST                                0x2701
#define GL_NEAREST_MIPMAP_LINEAR                                0x2702
#define GL_LINEAR_MIPMAP_LINEAR                                 0x2703
#define GL_TEXTURE_MAG_FILTER                                   0x2800
#define GL_TEXTURE_MIN_FILTER                                   0x2801
#define GL_TEXTURE_WRAP_S                                       0x2802
#define GL_TEXTURE_WRAP_T                                       0x2803
#define GL_CLAMP                                                0x2900
#define GL_REPEAT                                               0x2901
#define GL_CLIENT_PIXEL_STORE_BIT                               0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT                              0x00000002
#define GL_CLIENT_ALL_ATTRIB_BITS                               0xffffffff
#define GL_POLYGON_OFFSET_FACTOR                                0x8038
#define GL_POLYGON_OFFSET_UNITS                                 0x2A00
#define GL_POLYGON_OFFSET_POINT                                 0x2A01
#define GL_POLYGON_OFFSET_LINE                                  0x2A02
#define GL_POLYGON_OFFSET_FILL                                  0x8037
#define GL_ALPHA4                                               0x803B
#define GL_ALPHA8                                               0x803C
#define GL_ALPHA12                                              0x803D
#define GL_ALPHA16                                              0x803E
#define GL_LUMINANCE4                                           0x803F
#define GL_LUMINANCE8                                           0x8040
#define GL_LUMINANCE12                                          0x8041
#define GL_LUMINANCE16                                          0x8042
#define GL_LUMINANCE4_ALPHA4                                    0x8043
#define GL_LUMINANCE6_ALPHA2                                    0x8044
#define GL_LUMINANCE8_ALPHA8                                    0x8045
#define GL_LUMINANCE12_ALPHA4                                   0x8046
#define GL_LUMINANCE12_ALPHA12                                  0x8047
#define GL_LUMINANCE16_ALPHA16                                  0x8048
#define GL_INTENSITY                                            0x8049
#define GL_INTENSITY4                                           0x804A
#define GL_INTENSITY8                                           0x804B
#define GL_INTENSITY12                                          0x804C
#define GL_INTENSITY16                                          0x804D
#define GL_R3_G3_B2                                             0x2A10
#define GL_RGB4                                                 0x804F
#define GL_RGB5                                                 0x8050
#define GL_RGB8                                                 0x8051
#define GL_RGB10                                                0x8052
#define GL_RGB12                                                0x8053
#define GL_RGB16                                                0x8054
#define GL_RGBA2                                                0x8055
#define GL_RGBA4                                                0x8056
#define GL_RGB5_A1                                              0x8057
#define GL_RGBA8                                                0x8058
#define GL_RGB10_A2                                             0x8059
#define GL_RGBA12                                               0x805A
#define GL_RGBA16                                               0x805B
#define GL_TEXTURE_RED_SIZE                                     0x805C
#define GL_TEXTURE_GREEN_SIZE                                   0x805D
#define GL_TEXTURE_BLUE_SIZE                                    0x805E
#define GL_TEXTURE_ALPHA_SIZE                                   0x805F
#define GL_TEXTURE_LUMINANCE_SIZE                               0x8060
#define GL_TEXTURE_INTENSITY_SIZE                               0x8061
#define GL_PROXY_TEXTURE_1D                                     0x8063
#define GL_PROXY_TEXTURE_2D                                     0x8064
#define GL_TEXTURE_PRIORITY                                     0x8066
#define GL_TEXTURE_RESIDENT                                     0x8067
#define GL_TEXTURE_BINDING_1D                                   0x8068
#define GL_TEXTURE_BINDING_2D                                   0x8069
#define GL_VERTEX_ARRAY                                         0x8074
#define GL_NORMAL_ARRAY                                         0x8075
#define GL_COLOR_ARRAY                                          0x8076
#define GL_INDEX_ARRAY                                          0x8077
#define GL_TEXTURE_COORD_ARRAY                                  0x8078
#define GL_EDGE_FLAG_ARRAY                                      0x8079
#define GL_VERTEX_ARRAY_SIZE                                    0x807A
#define GL_VERTEX_ARRAY_TYPE                                    0x807B
#define GL_VERTEX_ARRAY_STRIDE                                  0x807C
#define GL_NORMAL_ARRAY_TYPE                                    0x807E
#define GL_NORMAL_ARRAY_STRIDE                                  0x807F
#define GL_COLOR_ARRAY_SIZE                                     0x8081
#define GL_COLOR_ARRAY_TYPE                                     0x8082
#define GL_COLOR_ARRAY_STRIDE                                   0x8083
#define GL_INDEX_ARRAY_TYPE                                     0x8085
#define GL_INDEX_ARRAY_STRIDE                                   0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE                             0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE                             0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE                           0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE                               0x808C
#define GL_VERTEX_ARRAY_POINTER                                 0x808E
#define GL_NORMAL_ARRAY_POINTER                                 0x808F
#define GL_COLOR_ARRAY_POINTER                                  0x8090
#define GL_INDEX_ARRAY_POINTER                                  0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER                          0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER                              0x8093
#define GL_V2F                                                  0x2A20
#define GL_V3F                                                  0x2A21
#define GL_C4UB_V2F                                             0x2A22
#define GL_C4UB_V3F                                             0x2A23
#define GL_C3F_V3F                                              0x2A24
#define GL_N3F_V3F                                              0x2A25
#define GL_C4F_N3F_V3F                                          0x2A26
#define GL_T2F_V3F                                              0x2A27
#define GL_T4F_V4F                                              0x2A28
#define GL_T2F_C4UB_V3F                                         0x2A29
#define GL_T2F_C3F_V3F                                          0x2A2A
#define GL_T2F_N3F_V3F                                          0x2A2B
#define GL_T2F_C4F_N3F_V3F                                      0x2A2C
#define GL_T4F_C4F_N3F_V4F                                      0x2A2D
#define GL_LOGIC_OP	                                 GL_INDEX_LOGIC_OP
#define GL_TEXTURE_COMPONENTS               GL_TEXTURE_INTERNAL_FORMAT

/* functions */

WINGDIAPI void APIENTRY glAccum (GLenum op, GLfloat value);
WINGDIAPI void APIENTRY glAlphaFunc (GLenum func, GLclampf ref);
WINGDIAPI GLboolean APIENTRY glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
WINGDIAPI void APIENTRY glArrayElement (GLint i);
WINGDIAPI void APIENTRY glBegin (GLenum mode);
WINGDIAPI void APIENTRY glBindTexture (GLenum target, GLuint texture);
WINGDIAPI void APIENTRY glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
WINGDIAPI void APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor);
WINGDIAPI void APIENTRY glCallList (GLuint list);
WINGDIAPI void APIENTRY glCallLists (GLsizei n, GLenum type, const GLvoid *lists);
WINGDIAPI void APIENTRY glClear (GLbitfield mask);
WINGDIAPI void APIENTRY glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
WINGDIAPI void APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
WINGDIAPI void APIENTRY glClearDepth (GLclampd depth);
WINGDIAPI void APIENTRY glClearIndex (GLfloat c);
WINGDIAPI void APIENTRY glClearStencil (GLint s);
WINGDIAPI void APIENTRY glClipPlane (GLenum plane, const GLdouble *equation);
WINGDIAPI void APIENTRY glColor3b (GLbyte red, GLbyte green, GLbyte blue);
WINGDIAPI void APIENTRY glColor3bv (const GLbyte *v);
WINGDIAPI void APIENTRY glColor3d (GLdouble red, GLdouble green, GLdouble blue);
WINGDIAPI void APIENTRY glColor3dv (const GLdouble *v);
WINGDIAPI void APIENTRY glColor3f (GLfloat red, GLfloat green, GLfloat blue);
WINGDIAPI void APIENTRY glColor3fv (const GLfloat *v);
WINGDIAPI void APIENTRY glColor3i (GLint red, GLint green, GLint blue);
WINGDIAPI void APIENTRY glColor3iv (const GLint *v);
WINGDIAPI void APIENTRY glColor3s (GLshort red, GLshort green, GLshort blue);
WINGDIAPI void APIENTRY glColor3sv (const GLshort *v);
WINGDIAPI void APIENTRY glColor3ub (GLubyte red, GLubyte green, GLubyte blue);
WINGDIAPI void APIENTRY glColor3ubv (const GLubyte *v);
WINGDIAPI void APIENTRY glColor3ui (GLuint red, GLuint green, GLuint blue);
WINGDIAPI void APIENTRY glColor3uiv (const GLuint *v);
WINGDIAPI void APIENTRY glColor3us (GLushort red, GLushort green, GLushort blue);
WINGDIAPI void APIENTRY glColor3usv (const GLushort *v);
WINGDIAPI void APIENTRY glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
WINGDIAPI void APIENTRY glColor4bv (const GLbyte *v);
WINGDIAPI void APIENTRY glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
WINGDIAPI void APIENTRY glColor4dv (const GLdouble *v);
WINGDIAPI void APIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
WINGDIAPI void APIENTRY glColor4fv (const GLfloat *v);
WINGDIAPI void APIENTRY glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
WINGDIAPI void APIENTRY glColor4iv (const GLint *v);
WINGDIAPI void APIENTRY glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
WINGDIAPI void APIENTRY glColor4sv (const GLshort *v);
WINGDIAPI void APIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
WINGDIAPI void APIENTRY glColor4ubv (const GLubyte *v);
WINGDIAPI void APIENTRY glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
WINGDIAPI void APIENTRY glColor4uiv (const GLuint *v);
WINGDIAPI void APIENTRY glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
WINGDIAPI void APIENTRY glColor4usv (const GLushort *v);
WINGDIAPI void APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
WINGDIAPI void APIENTRY glColorMaterial (GLenum face, GLenum mode);
WINGDIAPI void APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
WINGDIAPI void APIENTRY glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
WINGDIAPI void APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
WINGDIAPI void APIENTRY glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
WINGDIAPI void APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
WINGDIAPI void APIENTRY glCullFace (GLenum mode);
WINGDIAPI void APIENTRY glDeleteLists (GLuint list, GLsizei range);
WINGDIAPI void APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
WINGDIAPI void APIENTRY glDepthFunc (GLenum func);
WINGDIAPI void APIENTRY glDepthMask (GLboolean flag);
WINGDIAPI void APIENTRY glDepthRange (GLclampd zNear, GLclampd zFar);
WINGDIAPI void APIENTRY glDisable (GLenum cap);
WINGDIAPI void APIENTRY glDisableClientState (GLenum array);
WINGDIAPI void APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);
WINGDIAPI void APIENTRY glDrawBuffer (GLenum mode);
WINGDIAPI void APIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
WINGDIAPI void APIENTRY glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
WINGDIAPI void APIENTRY glEdgeFlag (GLboolean flag);
WINGDIAPI void APIENTRY glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glEdgeFlagv (const GLboolean *flag);
WINGDIAPI void APIENTRY glEnable (GLenum cap);
WINGDIAPI void APIENTRY glEnableClientState (GLenum array);
WINGDIAPI void APIENTRY glEnd (void);
WINGDIAPI void APIENTRY glEndList (void);
WINGDIAPI void APIENTRY glEvalCoord1d (GLdouble u);
WINGDIAPI void APIENTRY glEvalCoord1dv (const GLdouble *u);
WINGDIAPI void APIENTRY glEvalCoord1f (GLfloat u);
WINGDIAPI void APIENTRY glEvalCoord1fv (const GLfloat *u);
WINGDIAPI void APIENTRY glEvalCoord2d (GLdouble u, GLdouble v);
WINGDIAPI void APIENTRY glEvalCoord2dv (const GLdouble *u);
WINGDIAPI void APIENTRY glEvalCoord2f (GLfloat u, GLfloat v);
WINGDIAPI void APIENTRY glEvalCoord2fv (const GLfloat *u);
WINGDIAPI void APIENTRY glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
WINGDIAPI void APIENTRY glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
WINGDIAPI void APIENTRY glEvalPoint1 (GLint i);
WINGDIAPI void APIENTRY glEvalPoint2 (GLint i, GLint j);
WINGDIAPI void APIENTRY glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
WINGDIAPI void APIENTRY glFinish (void);
WINGDIAPI void APIENTRY glFlush (void);
WINGDIAPI void APIENTRY glFogf (GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glFogfv (GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glFogi (GLenum pname, GLint param);
WINGDIAPI void APIENTRY glFogiv (GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glFrontFace (GLenum mode);
WINGDIAPI void APIENTRY glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
WINGDIAPI GLuint APIENTRY glGenLists (GLsizei range);
WINGDIAPI void APIENTRY glGenTextures (GLsizei n, GLuint *textures);
WINGDIAPI void APIENTRY glGetBooleanv (GLenum pname, GLboolean *params);
WINGDIAPI void APIENTRY glGetClipPlane (GLenum plane, GLdouble *equation);
WINGDIAPI void APIENTRY glGetDoublev (GLenum pname, GLdouble *params);
WINGDIAPI GLenum APIENTRY glGetError (void);
WINGDIAPI void APIENTRY glGetFloatv (GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetIntegerv (GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetLightiv (GLenum light, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetMapdv (GLenum target, GLenum query, GLdouble *v);
WINGDIAPI void APIENTRY glGetMapfv (GLenum target, GLenum query, GLfloat *v);
WINGDIAPI void APIENTRY glGetMapiv (GLenum target, GLenum query, GLint *v);
WINGDIAPI void APIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetPixelMapfv (GLenum map, GLfloat *values);
WINGDIAPI void APIENTRY glGetPixelMapuiv (GLenum map, GLuint *values);
WINGDIAPI void APIENTRY glGetPixelMapusv (GLenum map, GLushort *values);
WINGDIAPI void APIENTRY glGetPointerv (GLenum pname, GLvoid* *params);
WINGDIAPI void APIENTRY glGetPolygonStipple (GLubyte *mask);
WINGDIAPI const GLubyte * APIENTRY glGetString (GLenum name);
WINGDIAPI void APIENTRY glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
WINGDIAPI void APIENTRY glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
WINGDIAPI void APIENTRY glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
WINGDIAPI void APIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
WINGDIAPI void APIENTRY glHint (GLenum target, GLenum mode);
WINGDIAPI void APIENTRY glIndexMask (GLuint mask);
WINGDIAPI void APIENTRY glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glIndexd (GLdouble c);
WINGDIAPI void APIENTRY glIndexdv (const GLdouble *c);
WINGDIAPI void APIENTRY glIndexf (GLfloat c);
WINGDIAPI void APIENTRY glIndexfv (const GLfloat *c);
WINGDIAPI void APIENTRY glIndexi (GLint c);
WINGDIAPI void APIENTRY glIndexiv (const GLint *c);
WINGDIAPI void APIENTRY glIndexs (GLshort c);
WINGDIAPI void APIENTRY glIndexsv (const GLshort *c);
WINGDIAPI void APIENTRY glIndexub (GLubyte c);
WINGDIAPI void APIENTRY glIndexubv (const GLubyte *c);
WINGDIAPI void APIENTRY glInitNames (void);
WINGDIAPI void APIENTRY glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer);
WINGDIAPI GLboolean APIENTRY glIsEnabled (GLenum cap);
WINGDIAPI GLboolean APIENTRY glIsList (GLuint list);
WINGDIAPI GLboolean APIENTRY glIsTexture (GLuint texture);
WINGDIAPI void APIENTRY glLightModelf (GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glLightModelfv (GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glLightModeli (GLenum pname, GLint param);
WINGDIAPI void APIENTRY glLightModeliv (GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glLightf (GLenum light, GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glLighti (GLenum light, GLenum pname, GLint param);
WINGDIAPI void APIENTRY glLightiv (GLenum light, GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glLineStipple (GLint factor, GLushort pattern);
WINGDIAPI void APIENTRY glLineWidth (GLfloat width);
WINGDIAPI void APIENTRY glListBase (GLuint base);
WINGDIAPI void APIENTRY glLoadIdentity (void);
WINGDIAPI void APIENTRY glLoadMatrixd (const GLdouble *m);
WINGDIAPI void APIENTRY glLoadMatrixf (const GLfloat *m);
WINGDIAPI void APIENTRY glLoadName (GLuint name);
WINGDIAPI void APIENTRY glLogicOp (GLenum opcode);
WINGDIAPI void APIENTRY glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
WINGDIAPI void APIENTRY glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
WINGDIAPI void APIENTRY glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
WINGDIAPI void APIENTRY glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
WINGDIAPI void APIENTRY glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
WINGDIAPI void APIENTRY glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
WINGDIAPI void APIENTRY glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
WINGDIAPI void APIENTRY glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
WINGDIAPI void APIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glMateriali (GLenum face, GLenum pname, GLint param);
WINGDIAPI void APIENTRY glMaterialiv (GLenum face, GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glMatrixMode (GLenum mode);
WINGDIAPI void APIENTRY glMultMatrixd (const GLdouble *m);
WINGDIAPI void APIENTRY glMultMatrixf (const GLfloat *m);
WINGDIAPI void APIENTRY glNewList (GLuint list, GLenum mode);
WINGDIAPI void APIENTRY glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
WINGDIAPI void APIENTRY glNormal3bv (const GLbyte *v);
WINGDIAPI void APIENTRY glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz);
WINGDIAPI void APIENTRY glNormal3dv (const GLdouble *v);
WINGDIAPI void APIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
WINGDIAPI void APIENTRY glNormal3fv (const GLfloat *v);
WINGDIAPI void APIENTRY glNormal3i (GLint nx, GLint ny, GLint nz);
WINGDIAPI void APIENTRY glNormal3iv (const GLint *v);
WINGDIAPI void APIENTRY glNormal3s (GLshort nx, GLshort ny, GLshort nz);
WINGDIAPI void APIENTRY glNormal3sv (const GLshort *v);
WINGDIAPI void APIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
WINGDIAPI void APIENTRY glPassThrough (GLfloat token);
WINGDIAPI void APIENTRY glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values);
WINGDIAPI void APIENTRY glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values);
WINGDIAPI void APIENTRY glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values);
WINGDIAPI void APIENTRY glPixelStoref (GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glPixelStorei (GLenum pname, GLint param);
WINGDIAPI void APIENTRY glPixelTransferf (GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glPixelTransferi (GLenum pname, GLint param);
WINGDIAPI void APIENTRY glPixelZoom (GLfloat xfactor, GLfloat yfactor);
WINGDIAPI void APIENTRY glPointSize (GLfloat size);
WINGDIAPI void APIENTRY glPolygonMode (GLenum face, GLenum mode);
WINGDIAPI void APIENTRY glPolygonOffset (GLfloat factor, GLfloat units);
WINGDIAPI void APIENTRY glPolygonStipple (const GLubyte *mask);
WINGDIAPI void APIENTRY glPopAttrib (void);
WINGDIAPI void APIENTRY glPopClientAttrib (void);
WINGDIAPI void APIENTRY glPopMatrix (void);
WINGDIAPI void APIENTRY glPopName (void);
WINGDIAPI void APIENTRY glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
WINGDIAPI void APIENTRY glPushAttrib (GLbitfield mask);
WINGDIAPI void APIENTRY glPushClientAttrib (GLbitfield mask);
WINGDIAPI void APIENTRY glPushMatrix (void);
WINGDIAPI void APIENTRY glPushName (GLuint name);
WINGDIAPI void APIENTRY glRasterPos2d (GLdouble x, GLdouble y);
WINGDIAPI void APIENTRY glRasterPos2dv (const GLdouble *v);
WINGDIAPI void APIENTRY glRasterPos2f (GLfloat x, GLfloat y);
WINGDIAPI void APIENTRY glRasterPos2fv (const GLfloat *v);
WINGDIAPI void APIENTRY glRasterPos2i (GLint x, GLint y);
WINGDIAPI void APIENTRY glRasterPos2iv (const GLint *v);
WINGDIAPI void APIENTRY glRasterPos2s (GLshort x, GLshort y);
WINGDIAPI void APIENTRY glRasterPos2sv (const GLshort *v);
WINGDIAPI void APIENTRY glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
WINGDIAPI void APIENTRY glRasterPos3dv (const GLdouble *v);
WINGDIAPI void APIENTRY glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
WINGDIAPI void APIENTRY glRasterPos3fv (const GLfloat *v);
WINGDIAPI void APIENTRY glRasterPos3i (GLint x, GLint y, GLint z);
WINGDIAPI void APIENTRY glRasterPos3iv (const GLint *v);
WINGDIAPI void APIENTRY glRasterPos3s (GLshort x, GLshort y, GLshort z);
WINGDIAPI void APIENTRY glRasterPos3sv (const GLshort *v);
WINGDIAPI void APIENTRY glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
WINGDIAPI void APIENTRY glRasterPos4dv (const GLdouble *v);
WINGDIAPI void APIENTRY glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
WINGDIAPI void APIENTRY glRasterPos4fv (const GLfloat *v);
WINGDIAPI void APIENTRY glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
WINGDIAPI void APIENTRY glRasterPos4iv (const GLint *v);
WINGDIAPI void APIENTRY glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
WINGDIAPI void APIENTRY glRasterPos4sv (const GLshort *v);
WINGDIAPI void APIENTRY glReadBuffer (GLenum mode);
WINGDIAPI void APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
WINGDIAPI void APIENTRY glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
WINGDIAPI void APIENTRY glRectdv (const GLdouble *v1, const GLdouble *v2);
WINGDIAPI void APIENTRY glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
WINGDIAPI void APIENTRY glRectfv (const GLfloat *v1, const GLfloat *v2);
WINGDIAPI void APIENTRY glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
WINGDIAPI void APIENTRY glRectiv (const GLint *v1, const GLint *v2);
WINGDIAPI void APIENTRY glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
WINGDIAPI void APIENTRY glRectsv (const GLshort *v1, const GLshort *v2);
WINGDIAPI GLint APIENTRY glRenderMode (GLenum mode);
WINGDIAPI void APIENTRY glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
WINGDIAPI void APIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
WINGDIAPI void APIENTRY glScaled (GLdouble x, GLdouble y, GLdouble z);
WINGDIAPI void APIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z);
WINGDIAPI void APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
WINGDIAPI void APIENTRY glSelectBuffer (GLsizei size, GLuint *buffer);
WINGDIAPI void APIENTRY glShadeModel (GLenum mode);
WINGDIAPI void APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask);
WINGDIAPI void APIENTRY glStencilMask (GLuint mask);
WINGDIAPI void APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
WINGDIAPI void APIENTRY glTexCoord1d (GLdouble s);
WINGDIAPI void APIENTRY glTexCoord1dv (const GLdouble *v);
WINGDIAPI void APIENTRY glTexCoord1f (GLfloat s);
WINGDIAPI void APIENTRY glTexCoord1fv (const GLfloat *v);
WINGDIAPI void APIENTRY glTexCoord1i (GLint s);
WINGDIAPI void APIENTRY glTexCoord1iv (const GLint *v);
WINGDIAPI void APIENTRY glTexCoord1s (GLshort s);
WINGDIAPI void APIENTRY glTexCoord1sv (const GLshort *v);
WINGDIAPI void APIENTRY glTexCoord2d (GLdouble s, GLdouble t);
WINGDIAPI void APIENTRY glTexCoord2dv (const GLdouble *v);
WINGDIAPI void APIENTRY glTexCoord2f (GLfloat s, GLfloat t);
WINGDIAPI void APIENTRY glTexCoord2fv (const GLfloat *v);
WINGDIAPI void APIENTRY glTexCoord2i (GLint s, GLint t);
WINGDIAPI void APIENTRY glTexCoord2iv (const GLint *v);
WINGDIAPI void APIENTRY glTexCoord2s (GLshort s, GLshort t);
WINGDIAPI void APIENTRY glTexCoord2sv (const GLshort *v);
WINGDIAPI void APIENTRY glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
WINGDIAPI void APIENTRY glTexCoord3dv (const GLdouble *v);
WINGDIAPI void APIENTRY glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
WINGDIAPI void APIENTRY glTexCoord3fv (const GLfloat *v);
WINGDIAPI void APIENTRY glTexCoord3i (GLint s, GLint t, GLint r);
WINGDIAPI void APIENTRY glTexCoord3iv (const GLint *v);
WINGDIAPI void APIENTRY glTexCoord3s (GLshort s, GLshort t, GLshort r);
WINGDIAPI void APIENTRY glTexCoord3sv (const GLshort *v);
WINGDIAPI void APIENTRY glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
WINGDIAPI void APIENTRY glTexCoord4dv (const GLdouble *v);
WINGDIAPI void APIENTRY glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
WINGDIAPI void APIENTRY glTexCoord4fv (const GLfloat *v);
WINGDIAPI void APIENTRY glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
WINGDIAPI void APIENTRY glTexCoord4iv (const GLint *v);
WINGDIAPI void APIENTRY glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
WINGDIAPI void APIENTRY glTexCoord4sv (const GLshort *v);
WINGDIAPI void APIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param);
WINGDIAPI void APIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glTexGend (GLenum coord, GLenum pname, GLdouble param);
WINGDIAPI void APIENTRY glTexGendv (GLenum coord, GLenum pname, const GLdouble *params);
WINGDIAPI void APIENTRY glTexGenf (GLenum coord, GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glTexGeni (GLenum coord, GLenum pname, GLint param);
WINGDIAPI void APIENTRY glTexGeniv (GLenum coord, GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
WINGDIAPI void APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
WINGDIAPI void APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param);
WINGDIAPI void APIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
WINGDIAPI void APIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param);
WINGDIAPI void APIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
WINGDIAPI void APIENTRY glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
WINGDIAPI void APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
WINGDIAPI void APIENTRY glTranslated (GLdouble x, GLdouble y, GLdouble z);
WINGDIAPI void APIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z);
WINGDIAPI void APIENTRY glVertex2d (GLdouble x, GLdouble y);
WINGDIAPI void APIENTRY glVertex2dv (const GLdouble *v);
WINGDIAPI void APIENTRY glVertex2f (GLfloat x, GLfloat y);
WINGDIAPI void APIENTRY glVertex2fv (const GLfloat *v);
WINGDIAPI void APIENTRY glVertex2i (GLint x, GLint y);
WINGDIAPI void APIENTRY glVertex2iv (const GLint *v);
WINGDIAPI void APIENTRY glVertex2s (GLshort x, GLshort y);
WINGDIAPI void APIENTRY glVertex2sv (const GLshort *v);
WINGDIAPI void APIENTRY glVertex3d (GLdouble x, GLdouble y, GLdouble z);
WINGDIAPI void APIENTRY glVertex3dv (const GLdouble *v);
WINGDIAPI void APIENTRY glVertex3f (GLfloat x, GLfloat y, GLfloat z);
WINGDIAPI void APIENTRY glVertex3fv (const GLfloat *v);
WINGDIAPI void APIENTRY glVertex3i (GLint x, GLint y, GLint z);
WINGDIAPI void APIENTRY glVertex3iv (const GLint *v);
WINGDIAPI void APIENTRY glVertex3s (GLshort x, GLshort y, GLshort z);
WINGDIAPI void APIENTRY glVertex3sv (const GLshort *v);
WINGDIAPI void APIENTRY glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
WINGDIAPI void APIENTRY glVertex4dv (const GLdouble *v);
WINGDIAPI void APIENTRY glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
WINGDIAPI void APIENTRY glVertex4fv (const GLfloat *v);
WINGDIAPI void APIENTRY glVertex4i (GLint x, GLint y, GLint z, GLint w);
WINGDIAPI void APIENTRY glVertex4iv (const GLint *v);
WINGDIAPI void APIENTRY glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
WINGDIAPI void APIENTRY glVertex4sv (const GLshort *v);
WINGDIAPI void APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
WINGDIAPI void APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

/* OpenGL 1.2 */

#ifndef GL_VERSION_1_2
#define GL_ARB_imaging 1
#define GL_VERSION_1_2 1
#define GL_RESCALE_NORMAL                                       0x803A
#define GL_CLAMP_TO_EDGE                                        0x812F
#define GL_MAX_ELEMENTS_VERTICES                                0x80E8
#define GL_MAX_ELEMENTS_INDICES                                 0x80E9
#define GL_BGR                                                  0x80E0
#define GL_BGRA                                                 0x80E1
#define GL_UNSIGNED_BYTE_3_3_2                                  0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV                              0x8362
#define GL_UNSIGNED_SHORT_5_6_5                                 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV                             0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4                               0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                           0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1                               0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV                           0x8366
#define GL_UNSIGNED_INT_8_8_8_8                                 0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV                             0x8367
#define GL_UNSIGNED_INT_10_10_10_2                              0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV                          0x8368
#define GL_LIGHT_MODEL_COLOR_CONTROL                            0x81F8
#define GL_SINGLE_COLOR                                         0x81F9
#define GL_SEPARATE_SPECULAR_COLOR                              0x81FA
#define GL_TEXTURE_MIN_LOD                                      0x813A
#define GL_TEXTURE_MAX_LOD                                      0x813B
#define GL_TEXTURE_BASE_LEVEL                                   0x813C
#define GL_TEXTURE_MAX_LEVEL                                    0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE                              0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY                        0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE                              0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY                        0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE                             0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE                             0x846E
#define GL_PACK_SKIP_IMAGES                                     0x806B
#define GL_PACK_IMAGE_HEIGHT                                    0x806C
#define GL_UNPACK_SKIP_IMAGES                                   0x806D
#define GL_UNPACK_IMAGE_HEIGHT                                  0x806E
#define GL_TEXTURE_3D                                           0x806F
#define GL_PROXY_TEXTURE_3D                                     0x8070
#define GL_TEXTURE_DEPTH                                        0x8071
#define GL_TEXTURE_WRAP_R                                       0x8072
#define GL_MAX_3D_TEXTURE_SIZE                                  0x8073
#define GL_TEXTURE_BINDING_3D                                   0x806A
#define GL_COLOR_TABLE                                          0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE                         0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE                        0x80D2
#define GL_PROXY_COLOR_TABLE                                    0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE                   0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE                  0x80D5
#define GL_COLOR_TABLE_SCALE                                    0x80D6
#define GL_COLOR_TABLE_BIAS                                     0x80D7
#define GL_COLOR_TABLE_FORMAT                                   0x80D8
#define GL_COLOR_TABLE_WIDTH                                    0x80D9
#define GL_COLOR_TABLE_RED_SIZE                                 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE                               0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE                                0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE                               0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE                           0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE                           0x80DF
#define GL_CONVOLUTION_1D                                       0x8010
#define GL_CONVOLUTION_2D                                       0x8011
#define GL_SEPARABLE_2D                                         0x8012
#define GL_CONVOLUTION_BORDER_MODE                              0x8013
#define GL_CONVOLUTION_FILTER_SCALE                             0x8014
#define GL_CONVOLUTION_FILTER_BIAS                              0x8015
#define GL_REDUCE                                               0x8016
#define GL_CONVOLUTION_FORMAT                                   0x8017
#define GL_CONVOLUTION_WIDTH                                    0x8018
#define GL_CONVOLUTION_HEIGHT                                   0x8019
#define GL_MAX_CONVOLUTION_WIDTH                                0x801A
#define GL_MAX_CONVOLUTION_HEIGHT                               0x801B
#define GL_POST_CONVOLUTION_RED_SCALE                           0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE                         0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE                          0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE                         0x801F
#define GL_POST_CONVOLUTION_RED_BIAS                            0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS                          0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS                           0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS                          0x8023
#define GL_CONSTANT_BORDER                                      0x8151
#define GL_REPLICATE_BORDER                                     0x8153
#define GL_CONVOLUTION_BORDER_COLOR                             0x8154
#define GL_COLOR_MATRIX                                         0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH                             0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH                         0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE                          0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE                        0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE                         0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE                        0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS                           0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS                         0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS                          0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS                         0x80BB
#define GL_HISTOGRAM                                            0x8024
#define GL_PROXY_HISTOGRAM                                      0x8025
#define GL_HISTOGRAM_WIDTH                                      0x8026
#define GL_HISTOGRAM_FORMAT                                     0x8027
#define GL_HISTOGRAM_RED_SIZE                                   0x8028
#define GL_HISTOGRAM_GREEN_SIZE                                 0x8029
#define GL_HISTOGRAM_BLUE_SIZE                                  0x802A
#define GL_HISTOGRAM_ALPHA_SIZE                                 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE                             0x802C
#define GL_HISTOGRAM_SINK                                       0x802D
#define GL_MINMAX                                               0x802E
#define GL_MINMAX_FORMAT                                        0x802F
#define GL_MINMAX_SINK                                          0x8030
#define GL_TABLE_TOO_LARGE                                      0x8031
#define GL_BLEND_EQUATION                                       0x8009
#define GL_MIN                                                  0x8007
#define GL_MAX                                                  0x8008
#define GL_FUNC_ADD                                             0x8006
#define GL_FUNC_SUBTRACT                                        0x800A
#define GL_FUNC_REVERSE_SUBTRACT                                0x800B
#define GL_BLEND_COLOR                                          0x8005
#define GL_CONSTANT_COLOR                                       0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR                             0x8002
#define GL_CONSTANT_ALPHA                                       0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA                             0x8004

typedef void (APIENTRY * glBlendColorPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
typedef void (APIENTRY * glBlendEquationPROC) (GLenum mode );
typedef void (APIENTRY * glColorSubTablePROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data );
typedef void (APIENTRY * glColorTablePROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table );
typedef void (APIENTRY * glColorTableParameterfvPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glColorTableParameterivPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glConvolutionFilter1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image );
typedef void (APIENTRY * glConvolutionFilter2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image );
typedef void (APIENTRY * glConvolutionParameterfPROC) (GLenum target, GLenum pname, GLfloat params );
typedef void (APIENTRY * glConvolutionParameterfvPROC) (GLenum target, GLenum pname, const GLfloat *params );
typedef void (APIENTRY * glConvolutionParameteriPROC) (GLenum target, GLenum pname, GLint params );
typedef void (APIENTRY * glConvolutionParameterivPROC) (GLenum target, GLenum pname, const GLint *params );
typedef void (APIENTRY * glCopyColorSubTablePROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glCopyColorTablePROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glCopyConvolutionFilter1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glCopyConvolutionFilter2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * glCopyTexSubImage3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
typedef void (APIENTRY * glDrawRangeElementsPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
typedef void (APIENTRY * glGetColorTablePROC) (GLenum target, GLenum format, GLenum type, GLvoid *table );
typedef void (APIENTRY * glGetColorTableParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetColorTableParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glGetConvolutionFilterPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image );
typedef void (APIENTRY * glGetConvolutionParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetConvolutionParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glGetHistogramPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values );
typedef void (APIENTRY * glGetHistogramParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetHistogramParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glGetMinmaxPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values );
typedef void (APIENTRY * glGetMinmaxParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetMinmaxParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glGetSeparableFilterPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );
typedef void (APIENTRY * glHistogramPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink );
typedef void (APIENTRY * glMinmaxPROC) (GLenum target, GLenum internalformat, GLboolean sink );
typedef void (APIENTRY * glResetHistogramPROC) (GLenum target );
typedef void (APIENTRY * glResetMinmaxPROC) (GLenum target );
typedef void (APIENTRY * glSeparableFilter2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column );
typedef void (APIENTRY * glTexImage3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
typedef void (APIENTRY * glTexSubImage3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glBlendColorPROC glBlendColor;
extern glBlendEquationPROC glBlendEquation;
extern glColorSubTablePROC glColorSubTable;
extern glColorTablePROC glColorTable;
extern glColorTableParameterfvPROC glColorTableParameterfv;
extern glColorTableParameterivPROC glColorTableParameteriv;
extern glConvolutionFilter1DPROC glConvolutionFilter1D;
extern glConvolutionFilter2DPROC glConvolutionFilter2D;
extern glConvolutionParameterfPROC glConvolutionParameterf;
extern glConvolutionParameterfvPROC glConvolutionParameterfv;
extern glConvolutionParameteriPROC glConvolutionParameteri;
extern glConvolutionParameterivPROC glConvolutionParameteriv;
extern glCopyColorSubTablePROC glCopyColorSubTable;
extern glCopyColorTablePROC glCopyColorTable;
extern glCopyConvolutionFilter1DPROC glCopyConvolutionFilter1D;
extern glCopyConvolutionFilter2DPROC glCopyConvolutionFilter2D;
extern glCopyTexSubImage3DPROC glCopyTexSubImage3D;
extern glDrawRangeElementsPROC glDrawRangeElements;
extern glGetColorTablePROC glGetColorTable;
extern glGetColorTableParameterfvPROC glGetColorTableParameterfv;
extern glGetColorTableParameterivPROC glGetColorTableParameteriv;
extern glGetConvolutionFilterPROC glGetConvolutionFilter;
extern glGetConvolutionParameterfvPROC glGetConvolutionParameterfv;
extern glGetConvolutionParameterivPROC glGetConvolutionParameteriv;
extern glGetHistogramPROC glGetHistogram;
extern glGetHistogramParameterfvPROC glGetHistogramParameterfv;
extern glGetHistogramParameterivPROC glGetHistogramParameteriv;
extern glGetMinmaxPROC glGetMinmax;
extern glGetMinmaxParameterfvPROC glGetMinmaxParameterfv;
extern glGetMinmaxParameterivPROC glGetMinmaxParameteriv;
extern glGetSeparableFilterPROC glGetSeparableFilter;
extern glHistogramPROC glHistogram;
extern glMinmaxPROC glMinmax;
extern glResetHistogramPROC glResetHistogram;
extern glResetMinmaxPROC glResetMinmax;
extern glSeparableFilter2DPROC glSeparableFilter2D;
extern glTexImage3DPROC glTexImage3D;
extern glTexSubImage3DPROC glTexSubImage3D;

#else

extern void APIENTRY glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern void APIENTRY glBlendEquation (GLenum mode );
extern void APIENTRY glColorSubTable (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data );
extern void APIENTRY glColorTable (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table );
extern void APIENTRY glColorTableParameterfv (GLenum target, GLenum pname, const GLfloat *params);
extern void APIENTRY glColorTableParameteriv (GLenum target, GLenum pname, const GLint *params);
extern void APIENTRY glConvolutionFilter1D (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image );
extern void APIENTRY glConvolutionFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image );
extern void APIENTRY glConvolutionParameterf (GLenum target, GLenum pname, GLfloat params );
extern void APIENTRY glConvolutionParameterfv (GLenum target, GLenum pname, const GLfloat *params );
extern void APIENTRY glConvolutionParameteri (GLenum target, GLenum pname, GLint params );
extern void APIENTRY glConvolutionParameteriv (GLenum target, GLenum pname, const GLint *params );
extern void APIENTRY glCopyColorSubTable (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width );
extern void APIENTRY glCopyColorTable (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
extern void APIENTRY glCopyConvolutionFilter1D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
extern void APIENTRY glCopyConvolutionFilter2D (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern void APIENTRY glCopyTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
extern void APIENTRY glDrawRangeElements (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
extern void APIENTRY glGetColorTable (GLenum target, GLenum format, GLenum type, GLvoid *table );
extern void APIENTRY glGetColorTableParameterfv (GLenum target, GLenum pname, GLfloat *params );
extern void APIENTRY glGetColorTableParameteriv (GLenum target, GLenum pname, GLint *params );
extern void APIENTRY glGetConvolutionFilter (GLenum target, GLenum format, GLenum type, GLvoid *image );
extern void APIENTRY glGetConvolutionParameterfv (GLenum target, GLenum pname, GLfloat *params );
extern void APIENTRY glGetConvolutionParameteriv (GLenum target, GLenum pname, GLint *params );
extern void APIENTRY glGetHistogram (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values );
extern void APIENTRY glGetHistogramParameterfv (GLenum target, GLenum pname, GLfloat *params );
extern void APIENTRY glGetHistogramParameteriv (GLenum target, GLenum pname, GLint *params );
extern void APIENTRY glGetMinmax (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values );
extern void APIENTRY glGetMinmaxParameterfv (GLenum target, GLenum pname, GLfloat *params );
extern void APIENTRY glGetMinmaxParameteriv (GLenum target, GLenum pname, GLint *params );
extern void APIENTRY glGetSeparableFilter (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );
extern void APIENTRY glHistogram (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink );
extern void APIENTRY glMinmax (GLenum target, GLenum internalformat, GLboolean sink );
extern void APIENTRY glResetHistogram (GLenum target );
extern void APIENTRY glResetMinmax (GLenum target );
extern void APIENTRY glSeparableFilter2D (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column );
extern void APIENTRY glTexImage3D (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
extern void APIENTRY glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

#endif /* WIN32 */

#endif /* GL_VERSION_1_2 */

/* OpenGL 1.3 */

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
#define GL_TEXTURE0                                             0x84C0
#define GL_TEXTURE1                                             0x84C1
#define GL_TEXTURE2                                             0x84C2
#define GL_TEXTURE3                                             0x84C3
#define GL_TEXTURE4                                             0x84C4
#define GL_TEXTURE5                                             0x84C5
#define GL_TEXTURE6                                             0x84C6
#define GL_TEXTURE7                                             0x84C7
#define GL_TEXTURE8                                             0x84C8
#define GL_TEXTURE9                                             0x84C9
#define GL_TEXTURE10                                            0x84CA
#define GL_TEXTURE11                                            0x84CB
#define GL_TEXTURE12                                            0x84CC
#define GL_TEXTURE13                                            0x84CD
#define GL_TEXTURE14                                            0x84CE
#define GL_TEXTURE15                                            0x84CF
#define GL_TEXTURE16                                            0x84D0
#define GL_TEXTURE17                                            0x84D1
#define GL_TEXTURE18                                            0x84D2
#define GL_TEXTURE19                                            0x84D3
#define GL_TEXTURE20                                            0x84D4
#define GL_TEXTURE21                                            0x84D5
#define GL_TEXTURE22                                            0x84D6
#define GL_TEXTURE23                                            0x84D7
#define GL_TEXTURE24                                            0x84D8
#define GL_TEXTURE25                                            0x84D9
#define GL_TEXTURE26                                            0x84DA
#define GL_TEXTURE27                                            0x84DB
#define GL_TEXTURE28                                            0x84DC
#define GL_TEXTURE29                                            0x84DD
#define GL_TEXTURE30                                            0x84DE
#define GL_TEXTURE31                                            0x84DF
#define GL_ACTIVE_TEXTURE                                       0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE                                0x84E1
#define GL_MAX_TEXTURE_UNITS                                    0x84E2

#define GL_NORMAL_MAP                                           0x8511
#define GL_REFLECTION_MAP                                       0x8512
#define GL_TEXTURE_CUBE_MAP                                     0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP                             0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X                          0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X                          0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y                          0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y                          0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z                          0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z                          0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP                               0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE                            0x851C

#define GL_COMPRESSED_ALPHA                                     0x84E9
#define GL_COMPRESSED_LUMINANCE                                 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA                           0x84EB
#define GL_COMPRESSED_INTENSITY                                 0x84EC
#define GL_COMPRESSED_RGB                                       0x84ED
#define GL_COMPRESSED_RGBA                                      0x84EE
#define GL_TEXTURE_COMPRESSION_HINT                             0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE                        0x86A0
#define GL_TEXTURE_COMPRESSED                                   0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS                       0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS                           0x86A3

#define GL_MULTISAMPLE                                          0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE                             0x809E
#define GL_SAMPLE_ALPHA_TO_ONE                                  0x809F
#define GL_SAMPLE_COVERAGE                                      0x80A0
#define GL_SAMPLE_BUFFERS                                       0x80A8
#define GL_SAMPLES                                              0x80A9
#define GL_SAMPLE_COVERAGE_VALUE                                0x80AA
#define GL_SAMPLE_COVERAGE_INVERT                               0x80AB
#define GL_MULTISAMPLE_BIT                                      0x20000000

#define GL_TRANSPOSE_MODELVIEW_MATRIX                           0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX                          0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX                             0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX                               0x84E6

#define GL_COMBINE                                              0x8570
#define GL_COMBINE_RGB                                          0x8571
#define GL_COMBINE_ALPHA                                        0x8572
#define GL_SOURCE0_RGB                                          0x8580
#define GL_SOURCE1_RGB                                          0x8581
#define GL_SOURCE2_RGB                                          0x8582
#define GL_SOURCE0_ALPHA                                        0x8588
#define GL_SOURCE1_ALPHA                                        0x8589
#define GL_SOURCE2_ALPHA                                        0x858A
#define GL_OPERAND0_RGB                                         0x8590
#define GL_OPERAND1_RGB                                         0x8591
#define GL_OPERAND2_RGB                                         0x8592
#define GL_OPERAND0_ALPHA                                       0x8598
#define GL_OPERAND1_ALPHA                                       0x8599
#define GL_OPERAND2_ALPHA                                       0x859A
#define GL_RGB_SCALE                                            0x8573
#define GL_ADD_SIGNED                                           0x8574
#define GL_INTERPOLATE                                          0x8575
#define GL_SUBTRACT                                             0x84E7
#define GL_CONSTANT                                             0x8576
#define GL_PRIMARY_COLOR                                        0x8577
#define GL_PREVIOUS                                             0x8578
#define GL_DOT3_RGB                                             0x86AE
#define GL_DOT3_RGBA                                            0x86AF
#define GL_CLAMP_TO_BORDER                                      0x812D

typedef void (APIENTRY * glActiveTexturePROC) (GLenum texture );
typedef void (APIENTRY * glClientActiveTexturePROC) (GLenum texture );
typedef void (APIENTRY * glCompressedTexImage1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glGetCompressedTexImagePROC) (GLenum target, GLint lod, GLvoid *img );
typedef void (APIENTRY * glMultiTexCoord1dPROC) (GLenum target, GLdouble s );
typedef void (APIENTRY * glMultiTexCoord1dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord1fPROC) (GLenum target, GLfloat s );
typedef void (APIENTRY * glMultiTexCoord1fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord1iPROC) (GLenum target, GLint s );
typedef void (APIENTRY * glMultiTexCoord1ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord1sPROC) (GLenum target, GLshort s );
typedef void (APIENTRY * glMultiTexCoord1svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord2dPROC) (GLenum target, GLdouble s, GLdouble t );
typedef void (APIENTRY * glMultiTexCoord2dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord2fPROC) (GLenum target, GLfloat s, GLfloat t );
typedef void (APIENTRY * glMultiTexCoord2fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord2iPROC) (GLenum target, GLint s, GLint t );
typedef void (APIENTRY * glMultiTexCoord2ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord2sPROC) (GLenum target, GLshort s, GLshort t );
typedef void (APIENTRY * glMultiTexCoord2svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord3dPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r );
typedef void (APIENTRY * glMultiTexCoord3dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord3fPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r );
typedef void (APIENTRY * glMultiTexCoord3fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord3iPROC) (GLenum target, GLint s, GLint t, GLint r );
typedef void (APIENTRY * glMultiTexCoord3ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord3sPROC) (GLenum target, GLshort s, GLshort t, GLshort r );
typedef void (APIENTRY * glMultiTexCoord3svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord4dPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
typedef void (APIENTRY * glMultiTexCoord4dvPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord4fPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
typedef void (APIENTRY * glMultiTexCoord4fvPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord4iPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q );
typedef void (APIENTRY * glMultiTexCoord4ivPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord4sPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
typedef void (APIENTRY * glMultiTexCoord4svPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glLoadTransposeMatrixdPROC) (const GLdouble m[16] );
typedef void (APIENTRY * glLoadTransposeMatrixfPROC) (const GLfloat m[16] );
typedef void (APIENTRY * glMultTransposeMatrixdPROC) (const GLdouble m[16] );
typedef void (APIENTRY * glMultTransposeMatrixfPROC) (const GLfloat m[16] );
typedef void (APIENTRY * glSampleCoveragePROC) (GLclampf value, GLboolean invert );

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glActiveTexturePROC glActiveTexture;
extern glClientActiveTexturePROC glClientActiveTexture;
extern glMultiTexCoord1dPROC glMultiTexCoord1d;
extern glMultiTexCoord1dvPROC glMultiTexCoord1dv;
extern glMultiTexCoord1fPROC glMultiTexCoord1f;
extern glMultiTexCoord1fvPROC glMultiTexCoord1fv;
extern glMultiTexCoord1iPROC glMultiTexCoord1i;
extern glMultiTexCoord1ivPROC glMultiTexCoord1iv;
extern glMultiTexCoord1sPROC glMultiTexCoord1s;
extern glMultiTexCoord1svPROC glMultiTexCoord1sv;
extern glMultiTexCoord2dPROC glMultiTexCoord2d;
extern glMultiTexCoord2dvPROC glMultiTexCoord2dv;
extern glMultiTexCoord2fPROC glMultiTexCoord2f;
extern glMultiTexCoord2fvPROC glMultiTexCoord2fv;
extern glMultiTexCoord2iPROC glMultiTexCoord2i;
extern glMultiTexCoord2ivPROC glMultiTexCoord2iv;
extern glMultiTexCoord2sPROC glMultiTexCoord2s;
extern glMultiTexCoord2svPROC glMultiTexCoord2sv;
extern glMultiTexCoord3dPROC glMultiTexCoord3d;
extern glMultiTexCoord3dvPROC glMultiTexCoord3dv;
extern glMultiTexCoord3fPROC glMultiTexCoord3f;
extern glMultiTexCoord3fvPROC glMultiTexCoord3fv;
extern glMultiTexCoord3iPROC glMultiTexCoord3i;
extern glMultiTexCoord3ivPROC glMultiTexCoord3iv;
extern glMultiTexCoord3sPROC glMultiTexCoord3s;
extern glMultiTexCoord3svPROC glMultiTexCoord3sv;
extern glMultiTexCoord4dPROC glMultiTexCoord4d;
extern glMultiTexCoord4dvPROC glMultiTexCoord4dv;
extern glMultiTexCoord4fPROC glMultiTexCoord4f;
extern glMultiTexCoord4fvPROC glMultiTexCoord4fv;
extern glMultiTexCoord4iPROC glMultiTexCoord4i;
extern glMultiTexCoord4ivPROC glMultiTexCoord4iv;
extern glMultiTexCoord4sPROC glMultiTexCoord4s;
extern glMultiTexCoord4svPROC glMultiTexCoord4sv;
extern glLoadTransposeMatrixfPROC glLoadTransposeMatrixf;
extern glLoadTransposeMatrixdPROC glLoadTransposeMatrixd;
extern glMultTransposeMatrixfPROC glMultTransposeMatrixf;
extern glMultTransposeMatrixdPROC glMultTransposeMatrixd;
extern glCompressedTexImage3DPROC glCompressedTexImage3D;
extern glCompressedTexImage2DPROC glCompressedTexImage2D;
extern glCompressedTexImage1DPROC glCompressedTexImage1D;
extern glCompressedTexSubImage3DPROC glCompressedTexSubImage3D;
extern glCompressedTexSubImage2DPROC glCompressedTexSubImage2D;
extern glCompressedTexSubImage1DPROC glCompressedTexSubImage1D;
extern glGetCompressedTexImagePROC glGetCompressedTexImage;
extern glSampleCoveragePROC glSampleCoverage;

#else

extern void APIENTRY glActiveTexture (GLenum texture );
extern void APIENTRY glClientActiveTexture (GLenum texture );
extern void APIENTRY glCompressedTexImage1D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glCompressedTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glCompressedTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
extern void APIENTRY glGetCompressedTexImage (GLenum target, GLint lod, GLvoid *img );
extern void APIENTRY glMultiTexCoord1d (GLenum target, GLdouble s );
extern void APIENTRY glMultiTexCoord1dv (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord1f (GLenum target, GLfloat s );
extern void APIENTRY glMultiTexCoord1fv (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord1i (GLenum target, GLint s );
extern void APIENTRY glMultiTexCoord1iv (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord1s (GLenum target, GLshort s );
extern void APIENTRY glMultiTexCoord1sv (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord2d (GLenum target, GLdouble s, GLdouble t );
extern void APIENTRY glMultiTexCoord2dv (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord2f (GLenum target, GLfloat s, GLfloat t );
extern void APIENTRY glMultiTexCoord2fv (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord2i (GLenum target, GLint s, GLint t );
extern void APIENTRY glMultiTexCoord2iv (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord2s (GLenum target, GLshort s, GLshort t );
extern void APIENTRY glMultiTexCoord2sv (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord3d (GLenum target, GLdouble s, GLdouble t, GLdouble r );
extern void APIENTRY glMultiTexCoord3dv (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord3f (GLenum target, GLfloat s, GLfloat t, GLfloat r );
extern void APIENTRY glMultiTexCoord3fv (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord3i (GLenum target, GLint s, GLint t, GLint r );
extern void APIENTRY glMultiTexCoord3iv (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord3s (GLenum target, GLshort s, GLshort t, GLshort r );
extern void APIENTRY glMultiTexCoord3sv (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord4d (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
extern void APIENTRY glMultiTexCoord4dv (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
extern void APIENTRY glMultiTexCoord4fv (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord4i (GLenum target, GLint s, GLint t, GLint r, GLint q );
extern void APIENTRY glMultiTexCoord4iv (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord4s (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
extern void APIENTRY glMultiTexCoord4sv (GLenum target, const GLshort *v );
extern void APIENTRY glLoadTransposeMatrixd (const GLdouble m[16] );
extern void APIENTRY glLoadTransposeMatrixf (const GLfloat m[16] );
extern void APIENTRY glMultTransposeMatrixd (const GLdouble m[16] );
extern void APIENTRY glMultTransposeMatrixf (const GLfloat m[16] );
extern void APIENTRY glSampleCoverage (GLclampf value, GLboolean invert );

#endif /* WIN32 */

#endif /* GL_VERSION_1_3 */

/* OpenGL 1.4 */

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4

#define GL_GENERATE_MIPMAP                                      0x8191
#define GL_GENERATE_MIPMAP_HINT                                 0x8192
#define GL_DEPTH_COMPONENT16                                    0x81A5
#define GL_DEPTH_COMPONENT24                                    0x81A6
#define GL_DEPTH_COMPONENT32                                    0x81A7
#define GL_TEXTURE_DEPTH_SIZE                                   0x884A
#define GL_DEPTH_TEXTURE_MODE                                   0x884B
#define GL_TEXTURE_COMPARE_MODE                                 0x884C
#define GL_TEXTURE_COMPARE_FUNC                                 0x884D
#define GL_COMPARE_R_TO_TEXTURE                                 0x884E
#define GL_FOG_COORDINATE_SOURCE                                0x8450
#define GL_FOG_COORDINATE                                       0x8451
#define GL_FRAGMENT_DEPTH                                       0x8452
#define GL_CURRENT_FOG_COORDINATE                               0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE                            0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE                          0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER                         0x8456
#define GL_FOG_COORDINATE_ARRAY                                 0x8457
#define GL_POINT_SIZE_MIN                                       0x8126
#define GL_POINT_SIZE_MAX                                       0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE                            0x8128
#define GL_POINT_DISTANCE_ATTENUATION                           0x8129
#define GL_COLOR_SUM                                            0x8458
#define GL_CURRENT_SECONDARY_COLOR                              0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE                           0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE                           0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE                         0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER                        0x845D
#define GL_SECONDARY_COLOR_ARRAY                                0x845E
#define GL_BLEND_DST_RGB                                        0x80C8
#define GL_BLEND_SRC_RGB                                        0x80C9
#define GL_BLEND_DST_ALPHA                                      0x80CA
#define GL_BLEND_SRC_ALPHA                                      0x80CB
#define GL_INCR_WRAP                                            0x8507
#define GL_DECR_WRAP                                            0x8508
#define GL_TEXTURE_FILTER_CONTROL                               0x8500
#define GL_TEXTURE_LOD_BIAS                                     0x8501
#define GL_MAX_TEXTURE_LOD_BIAS                                 0x84FD
#define GL_MIRRORED_REPEAT                                      0x8370

typedef void (APIENTRY * glFogCoordfPROC) (GLfloat coord);
typedef void (APIENTRY * glFogCoordfvPROC) (const GLfloat *coord);
typedef void (APIENTRY * glFogCoorddPROC) (GLdouble coord);
typedef void (APIENTRY * glFogCoorddvPROC) (const GLdouble *coord);
typedef void (APIENTRY * glFogCoordPointerPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * glMultiDrawArraysPROC) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * glMultiDrawElementsPROC) (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
typedef void (APIENTRY * glPointParameterfPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glPointParameterfvPROC) (GLenum pname, GLfloat *params);
typedef void (APIENTRY * glSecondaryColor3bPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (APIENTRY * glSecondaryColor3bvPROC) (const GLbyte *v);
typedef void (APIENTRY * glSecondaryColor3dPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (APIENTRY * glSecondaryColor3dvPROC) (const GLdouble *v);
typedef void (APIENTRY * glSecondaryColor3fPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (APIENTRY * glSecondaryColor3fvPROC) (const GLfloat *v);
typedef void (APIENTRY * glSecondaryColor3iPROC) (GLint red, GLint green, GLint blue);
typedef void (APIENTRY * glSecondaryColor3ivPROC) (const GLint *v);
typedef void (APIENTRY * glSecondaryColor3sPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (APIENTRY * glSecondaryColor3svPROC) (const GLshort *v);
typedef void (APIENTRY * glSecondaryColor3ubPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (APIENTRY * glSecondaryColor3ubvPROC) (const GLubyte *v);
typedef void (APIENTRY * glSecondaryColor3uiPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (APIENTRY * glSecondaryColor3uivPROC) (const GLuint *v);
typedef void (APIENTRY * glSecondaryColor3usPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (APIENTRY * glSecondaryColor3usvPROC) (const GLushort *v);
typedef void (APIENTRY * glSecondaryColorPointerPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (APIENTRY * glBlendFuncSeparatePROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (APIENTRY * glWindowPos2dPROC) (GLdouble x, GLdouble y);
typedef void (APIENTRY * glWindowPos2fPROC) (GLfloat x, GLfloat y);
typedef void (APIENTRY * glWindowPos2iPROC) (GLint x, GLint y);
typedef void (APIENTRY * glWindowPos2sPROC) (GLshort x, GLshort y);
typedef void (APIENTRY * glWindowPos2dvPROC) (const GLdouble *p);
typedef void (APIENTRY * glWindowPos2fvPROC) (const GLfloat *p);
typedef void (APIENTRY * glWindowPos2ivPROC) (const GLint *p);
typedef void (APIENTRY * glWindowPos2svPROC) (const GLshort *p);
typedef void (APIENTRY * glWindowPos3dPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glWindowPos3fPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glWindowPos3iPROC) (GLint x, GLint y, GLint z);
typedef void (APIENTRY * glWindowPos3sPROC) (GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glWindowPos3dvPROC) (const GLdouble *p);
typedef void (APIENTRY * glWindowPos3fvPROC) (const GLfloat *p);
typedef void (APIENTRY * glWindowPos3ivPROC) (const GLint *p);
typedef void (APIENTRY * glWindowPos3svPROC) (const GLshort *p);

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glFogCoordfPROC glFogCoordf;
extern glFogCoordfvPROC glFogCoordfv;
extern glFogCoorddPROC glFogCoordd;
extern glFogCoorddvPROC glFogCoorddv;
extern glFogCoordPointerPROC glFogCoordPointer;
extern glMultiDrawArraysPROC glMultiDrawArrays;
extern glMultiDrawElementsPROC glMultiDrawElements;
extern glPointParameterfPROC glPointParameterf;
extern glPointParameterfvPROC glPointParameterfv;
extern glSecondaryColor3bPROC glSecondaryColor3b;
extern glSecondaryColor3bvPROC glSecondaryColor3bv;
extern glSecondaryColor3dPROC glSecondaryColor3d;
extern glSecondaryColor3dvPROC glSecondaryColor3dv;
extern glSecondaryColor3fPROC glSecondaryColor3f;
extern glSecondaryColor3fvPROC glSecondaryColor3fv;
extern glSecondaryColor3iPROC glSecondaryColor3i;
extern glSecondaryColor3ivPROC glSecondaryColor3iv;
extern glSecondaryColor3sPROC glSecondaryColor3s;
extern glSecondaryColor3svPROC glSecondaryColor3sv;
extern glSecondaryColor3ubPROC glSecondaryColor3ub;
extern glSecondaryColor3ubvPROC glSecondaryColor3ubv;
extern glSecondaryColor3uiPROC glSecondaryColor3ui;
extern glSecondaryColor3uivPROC glSecondaryColor3uiv;
extern glSecondaryColor3usPROC glSecondaryColor3us;
extern glSecondaryColor3usvPROC glSecondaryColor3usv;
extern glSecondaryColorPointerPROC glSecondaryColorPointer;
extern glBlendFuncSeparatePROC glBlendFuncSeparate;
extern glWindowPos2dPROC glWindowPos2d;
extern glWindowPos2fPROC glWindowPos2f;
extern glWindowPos2iPROC glWindowPos2i;
extern glWindowPos2sPROC glWindowPos2s;
extern glWindowPos2dvPROC glWindowPos2dv;
extern glWindowPos2fvPROC glWindowPos2fv;
extern glWindowPos2ivPROC glWindowPos2iv;
extern glWindowPos2svPROC glWindowPos2sv;
extern glWindowPos3dPROC glWindowPos3d;
extern glWindowPos3fPROC glWindowPos3f;
extern glWindowPos3iPROC glWindowPos3i;
extern glWindowPos3sPROC glWindowPos3s;
extern glWindowPos3dvPROC glWindowPos3dv;
extern glWindowPos3fvPROC glWindowPos3fv;
extern glWindowPos3ivPROC glWindowPos3iv;
extern glWindowPos3svPROC glWindowPos3sv;

#else

extern void APIENTRY glFogCoordf (GLfloat coord);
extern void APIENTRY glFogCoordfv (const GLfloat *coord);
extern void APIENTRY glFogCoordd (GLdouble coord);
extern void APIENTRY glFogCoorddv (const GLdouble *coord);
extern void APIENTRY glFogCoordPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void APIENTRY glMultiDrawArrays (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
extern void APIENTRY glMultiDrawElements (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
extern void APIENTRY glPointParameterf (GLenum pname, GLfloat param);
extern void APIENTRY glPointParameterfv (GLenum pname, GLfloat *params);
extern void APIENTRY glSecondaryColor3b (GLbyte red, GLbyte green, GLbyte blue);
extern void APIENTRY glSecondaryColor3bv (const GLbyte *v);
extern void APIENTRY glSecondaryColor3d (GLdouble red, GLdouble green, GLdouble blue);
extern void APIENTRY glSecondaryColor3dv (const GLdouble *v);
extern void APIENTRY glSecondaryColor3f (GLfloat red, GLfloat green, GLfloat blue);
extern void APIENTRY glSecondaryColor3fv (const GLfloat *v);
extern void APIENTRY glSecondaryColor3i (GLint red, GLint green, GLint blue);
extern void APIENTRY glSecondaryColor3iv (const GLint *v);
extern void APIENTRY glSecondaryColor3s (GLshort red, GLshort green, GLshort blue);
extern void APIENTRY glSecondaryColor3sv (const GLshort *v);
extern void APIENTRY glSecondaryColor3ub (GLubyte red, GLubyte green, GLubyte blue);
extern void APIENTRY glSecondaryColor3ubv (const GLubyte *v);
extern void APIENTRY glSecondaryColor3ui (GLuint red, GLuint green, GLuint blue);
extern void APIENTRY glSecondaryColor3uiv (const GLuint *v);
extern void APIENTRY glSecondaryColor3us (GLushort red, GLushort green, GLushort blue);
extern void APIENTRY glSecondaryColor3usv (const GLushort *v);
extern void APIENTRY glSecondaryColorPointer (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
extern void APIENTRY glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern void APIENTRY glWindowPos2d (GLdouble x, GLdouble y);
extern void APIENTRY glWindowPos2f (GLfloat x, GLfloat y);
extern void APIENTRY glWindowPos2i (GLint x, GLint y);
extern void APIENTRY glWindowPos2s (GLshort x, GLshort y);
extern void APIENTRY glWindowPos2dv (const GLdouble *p);
extern void APIENTRY glWindowPos2fv (const GLfloat *p);
extern void APIENTRY glWindowPos2iv (const GLint *p);
extern void APIENTRY glWindowPos2sv (const GLshort *p);
extern void APIENTRY glWindowPos3d (GLdouble x, GLdouble y, GLdouble z);
extern void APIENTRY glWindowPos3f (GLfloat x, GLfloat y, GLfloat z);
extern void APIENTRY glWindowPos3i (GLint x, GLint y, GLint z);
extern void APIENTRY glWindowPos3s (GLshort x, GLshort y, GLshort z);
extern void APIENTRY glWindowPos3dv (const GLdouble *p);
extern void APIENTRY glWindowPos3fv (const GLfloat *p);
extern void APIENTRY glWindowPos3iv (const GLint *p);
extern void APIENTRY glWindowPos3sv (const GLshort *p);

#endif /* WIN32 */

#endif /* GL_VERSION_1_4 */

/* OpenGL 1.5 */

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1

/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

/* GL types for handling shader object handles and characters */
typedef char GLcharARB;		/* native character */
typedef unsigned int GLhandleARB;	/* shader object handle */

/* GL type for representing NVIDIA "half" floating point type in host memory */
typedef unsigned short GLhalfNV;

/* GL types for "half" precision (s10e5) float data in host memory */
typedef unsigned short GLhalfARB;

// GL types for handling large vertex buffer objects
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;


#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_QUERY_COUNTER_BITS             0x8864
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING     0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING     0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING    0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_BUFFER_ACCESS                  0x88BB
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_SAMPLES_PASSED                 0x8914
#define GL_FOG_COORD_SOURCE               GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD                      GL_FOG_COORDINATE
#define GL_CURRENT_FOG_COORD              GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_TYPE           GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORD_ARRAY_STRIDE         GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_POINTER        GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY                GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC0_RGB                       GL_SOURCE0_RGB
#define GL_SRC1_RGB                       GL_SOURCE1_RGB
#define GL_SRC2_RGB                       GL_SOURCE2_RGB
#define GL_SRC0_ALPHA                     GL_SOURCE0_ALPHA
#define GL_SRC1_ALPHA                     GL_SOURCE1_ALPHA
#define GL_SRC2_ALPHA                     GL_SOURCE2_ALPHA

typedef void (APIENTRY * glGenQueriesPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glDeleteQueriesPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRY * glIsQueryPROC) (GLuint id);
typedef void (APIENTRY * glBeginQueryPROC) (GLenum target, GLuint id);
typedef void (APIENTRY * glEndQueryPROC) (GLenum target);
typedef void (APIENTRY * glGetQueryivPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectivPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectuivPROC) (GLuint id, GLenum pname, GLuint *params);
typedef void (APIENTRY * glBindBufferPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * glDeleteBuffersPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * glGenBuffersPROC) (GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * glIsBufferPROC) (GLuint buffer);
typedef void (APIENTRY * glBufferDataPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * glBufferSubDataPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void (APIENTRY * glGetBufferSubDataPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
typedef GLvoid* (APIENTRY * glMapBufferPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY * glUnmapBufferPROC) (GLenum target);
typedef void (APIENTRY * glGetBufferParameterivPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetBufferPointervPROC) (GLenum target, GLenum pname, GLvoid* *params);

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glGenQueriesPROC glGenQueries;
extern glDeleteQueriesPROC glDeleteQueries;
extern glIsQueryPROC glIsQuery;
extern glBeginQueryPROC glBeginQuery;
extern glEndQueryPROC glEndQuery;
extern glGetQueryivPROC glGetQueryiv;
extern glGetQueryObjectivPROC glGetQueryObjectiv;
extern glGetQueryObjectuivPROC glGetQueryObjectuiv;
extern glBindBufferPROC glBindBuffer;
extern glDeleteBuffersPROC glDeleteBuffers;
extern glGenBuffersPROC glGenBuffers;
extern glIsBufferPROC glIsBuffer;
extern glBufferDataPROC glBufferData;
extern glBufferSubDataPROC glBufferSubData;
extern glGetBufferSubDataPROC glGetBufferSubData;
extern glMapBufferPROC glMapBuffer;
extern glUnmapBufferPROC glUnmapBuffer;
extern glGetBufferParameterivPROC glGetBufferParameteriv;
extern glGetBufferPointervPROC glGetBufferPointerv;

#else

extern void APIENTRY glGenQueries (GLsizei n, GLuint *ids);
extern void APIENTRY glDeleteQueries (GLsizei n, const GLuint *ids);
extern GLboolean APIENTRY glIsQuery (GLuint id);
extern void APIENTRY glBeginQuery (GLenum target, GLuint id);
extern void APIENTRY glEndQuery (GLenum target);
extern void APIENTRY glGetQueryiv (GLenum target, GLenum pname, GLint *params);
extern void APIENTRY glGetQueryObjectiv (GLuint id, GLenum pname, GLint *params);
extern void APIENTRY glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params);
extern void APIENTRY glBindBuffer (GLenum target, GLuint buffer);
extern void APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers);
extern void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers);
extern GLboolean APIENTRY glIsBuffer (GLuint buffer);
extern void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
extern void APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
extern void APIENTRY glGetBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
extern GLvoid* APIENTRY glMapBuffer (GLenum target, GLenum access);
extern GLboolean APIENTRY glUnmapBuffer (GLenum target);
extern void APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params);
extern void APIENTRY glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params);

#endif /* defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) */

#endif /* GL_VERSION_1_5 */

/* OpenGL 2.0 */

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1

/* GL type for program/shader text */
typedef char GLchar;			/* native character */

#define GL_BLEND_EQUATION_RGB             GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_POINT_SPRITE                   0x8861
#define GL_COORD_REPLACE                  0x8862
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS             0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAG_SHADER                0x8B30
#define GL_VERT_SHADER                  0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_MAX_VARYING_FLOATS             0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN      0x8CA0
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5

typedef void (APIENTRY * glBlendEquationSeparatePROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRY * glDrawBuffersPROC) (GLsizei n, const GLenum *bufs);
typedef void (APIENTRY * glStencilOpSeparatePROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRY * glStencilFuncSeparatePROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (APIENTRY * glStencilMaskSeparatePROC) (GLenum face, GLuint mask);
typedef void (APIENTRY * glAttachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * glBindAttribLocationPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (APIENTRY * glCompileShaderPROC) (GLuint shader);
typedef GLuint (APIENTRY * glCreateProgramPROC) (void);
typedef GLuint (APIENTRY * glCreateShaderPROC) (GLenum type);
typedef void (APIENTRY * glDeleteProgramPROC) (GLuint program);
typedef void (APIENTRY * glDeleteShaderPROC) (GLuint shader);
typedef void (APIENTRY * glDetachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY * glDisableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY * glEnableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY * glGetActiveAttribPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRY * glGetActiveUniformPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRY * glGetAttachedShadersPROC) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
typedef GLint (APIENTRY * glGetAttribLocationPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY * glGetProgramivPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetProgramInfoLogPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY * glGetShaderivPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetShaderInfoLogPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY * glGetShaderSourcePROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef GLint (APIENTRY * glGetUniformLocationPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY * glGetUniformfvPROC) (GLuint program, GLint location, GLfloat *params);
typedef void (APIENTRY * glGetUniformivPROC) (GLuint program, GLint location, GLint *params);
typedef void (APIENTRY * glGetVertexAttribdvPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRY * glGetVertexAttribfvPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVertexAttribivPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetVertexAttribPointervPROC) (GLuint index, GLenum pname, GLvoid* *pointer);
typedef GLboolean (APIENTRY * glIsProgramPROC) (GLuint program);
typedef GLboolean (APIENTRY * glIsShaderPROC) (GLuint shader);
typedef void (APIENTRY * glLinkProgramPROC) (GLuint program);
typedef void (APIENTRY * glShaderSourcePROC) (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
typedef void (APIENTRY * glUseProgramPROC) (GLuint program);
typedef void (APIENTRY * glUniform1fPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY * glUniform2fPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY * glUniform3fPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY * glUniform4fPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY * glUniform1iPROC) (GLint location, GLint v0);
typedef void (APIENTRY * glUniform2iPROC) (GLint location, GLint v0, GLint v1);
typedef void (APIENTRY * glUniform3iPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRY * glUniform4iPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRY * glUniform1fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform2fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform3fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform4fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform1ivPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform2ivPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform3ivPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform4ivPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniformMatrix2fvPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glUniformMatrix3fvPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glUniformMatrix4fvPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glValidateProgramPROC) (GLuint program);
typedef void (APIENTRY * glVertexAttrib1dPROC) (GLuint index, GLdouble x);
typedef void (APIENTRY * glVertexAttrib1dvPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib1fPROC) (GLuint index, GLfloat x);
typedef void (APIENTRY * glVertexAttrib1fvPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib1sPROC) (GLuint index, GLshort x);
typedef void (APIENTRY * glVertexAttrib1svPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib2dPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRY * glVertexAttrib2dvPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib2fPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRY * glVertexAttrib2fvPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib2sPROC) (GLuint index, GLshort x, GLshort y);
typedef void (APIENTRY * glVertexAttrib2svPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib3dPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glVertexAttrib3dvPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib3fPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glVertexAttrib3fvPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib3sPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glVertexAttrib3svPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4NbvPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4NivPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4NsvPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4NubPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (APIENTRY * glVertexAttrib4NubvPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4NuivPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttrib4NusvPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttrib4bvPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4dPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glVertexAttrib4dvPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4fPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glVertexAttrib4fvPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib4ivPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4sPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glVertexAttrib4svPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4ubvPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4uivPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttrib4usvPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttribPointerPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glBlendEquationSeparatePROC glBlendEquationSeparate;
extern glDrawBuffersPROC glDrawBuffers;
extern glStencilOpSeparatePROC glStencilOpSeparate;
extern glStencilFuncSeparatePROC glStencilFuncSeparate;
extern glStencilMaskSeparatePROC glStencilMaskSeparate;
extern glAttachShaderPROC glAttachShader;
extern glBindAttribLocationPROC glBindAttribLocation;
extern glCompileShaderPROC glCompileShader;
extern glCreateProgramPROC glCreateProgram;
extern glCreateShaderPROC glCreateShader;
extern glDeleteProgramPROC glDeleteProgram;
extern glDeleteShaderPROC glDeleteShader;
extern glDetachShaderPROC glDetachShader;
extern glDisableVertexAttribArrayPROC glDisableVertexAttribArray;
extern glEnableVertexAttribArrayPROC glEnableVertexAttribArray;
extern glGetActiveAttribPROC glGetActiveAttrib;
extern glGetActiveUniformPROC glGetActiveUniform;
extern glGetAttachedShadersPROC glGetAttachedShaders;
extern glGetAttribLocationPROC glGetAttribLocation;
extern glGetProgramivPROC glGetProgramiv;
extern glGetProgramInfoLogPROC glGetProgramInfoLog;
extern glGetShaderivPROC glGetShaderiv;
extern glGetShaderInfoLogPROC glGetShaderInfoLog;
extern glGetShaderSourcePROC glGetShaderSource;
extern glGetUniformLocationPROC glGetUniformLocation;
extern glGetUniformfvPROC glGetUniformfv;
extern glGetUniformivPROC glGetUniformiv;
extern glGetVertexAttribdvPROC glGetVertexAttribdv;
extern glGetVertexAttribfvPROC glGetVertexAttribfv;
extern glGetVertexAttribivPROC glGetVertexAttribiv;
extern glGetVertexAttribPointervPROC glGetVertexAttribPointerv;
extern glIsProgramPROC glIsProgram;
extern glIsShaderPROC glIsShader;
extern glLinkProgramPROC glLinkProgram;
extern glShaderSourcePROC glShaderSource;
extern glUseProgramPROC glUseProgram;
extern glUniform1fPROC glUniform1f;
extern glUniform2fPROC glUniform2f;
extern glUniform3fPROC glUniform3f;
extern glUniform4fPROC glUniform4f;
extern glUniform1iPROC glUniform1i;
extern glUniform2iPROC glUniform2i;
extern glUniform3iPROC glUniform3i;
extern glUniform4iPROC glUniform4i;
extern glUniform1fvPROC glUniform1fv;
extern glUniform2fvPROC glUniform2fv;
extern glUniform3fvPROC glUniform3fv;
extern glUniform4fvPROC glUniform4fv;
extern glUniform1ivPROC glUniform1iv;
extern glUniform2ivPROC glUniform2iv;
extern glUniform3ivPROC glUniform3iv;
extern glUniform4ivPROC glUniform4iv;
extern glUniformMatrix2fvPROC glUniformMatrix2fv;
extern glUniformMatrix3fvPROC glUniformMatrix3fv;
extern glUniformMatrix4fvPROC glUniformMatrix4fv;
extern glValidateProgramPROC glValidateProgram;
extern glVertexAttrib1dPROC glVertexAttrib1d;
extern glVertexAttrib1dvPROC glVertexAttrib1dv;
extern glVertexAttrib1fPROC glVertexAttrib1f;
extern glVertexAttrib1fvPROC glVertexAttrib1fv;
extern glVertexAttrib1sPROC glVertexAttrib1s;
extern glVertexAttrib1svPROC glVertexAttrib1sv;
extern glVertexAttrib2dPROC glVertexAttrib2d;
extern glVertexAttrib2dvPROC glVertexAttrib2dv;
extern glVertexAttrib2fPROC glVertexAttrib2f;
extern glVertexAttrib2fvPROC glVertexAttrib2fv;
extern glVertexAttrib2sPROC glVertexAttrib2s;
extern glVertexAttrib2svPROC glVertexAttrib2sv;
extern glVertexAttrib3dPROC glVertexAttrib3d;
extern glVertexAttrib3dvPROC glVertexAttrib3dv;
extern glVertexAttrib3fPROC glVertexAttrib3f;
extern glVertexAttrib3fvPROC glVertexAttrib3fv;
extern glVertexAttrib3sPROC glVertexAttrib3s;
extern glVertexAttrib3svPROC glVertexAttrib3sv;
extern glVertexAttrib4NbvPROC glVertexAttrib4Nbv;
extern glVertexAttrib4NivPROC glVertexAttrib4Niv;
extern glVertexAttrib4NsvPROC glVertexAttrib4Nsv;
extern glVertexAttrib4NubPROC glVertexAttrib4Nub;
extern glVertexAttrib4NubvPROC glVertexAttrib4Nubv;
extern glVertexAttrib4NuivPROC glVertexAttrib4Nuiv;
extern glVertexAttrib4NusvPROC glVertexAttrib4Nusv;
extern glVertexAttrib4bvPROC glVertexAttrib4bv;
extern glVertexAttrib4dPROC glVertexAttrib4d;
extern glVertexAttrib4dvPROC glVertexAttrib4dv;
extern glVertexAttrib4fPROC glVertexAttrib4f;
extern glVertexAttrib4fvPROC glVertexAttrib4fv;
extern glVertexAttrib4ivPROC glVertexAttrib4iv;
extern glVertexAttrib4sPROC glVertexAttrib4s;
extern glVertexAttrib4svPROC glVertexAttrib4sv;
extern glVertexAttrib4ubvPROC glVertexAttrib4ubv;
extern glVertexAttrib4uivPROC glVertexAttrib4uiv;
extern glVertexAttrib4usvPROC glVertexAttrib4usv;
extern glVertexAttribPointerPROC glVertexAttribPointer;

#else

extern void APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha);
extern void APIENTRY glDrawBuffers (GLsizei n, const GLenum *bufs);
extern void APIENTRY glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void APIENTRY glStencilFuncSeparate (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern void APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask);
extern void APIENTRY glAttachShader (GLuint program, GLuint shader);
extern void APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar *name);
extern void APIENTRY glCompileShader (GLuint shader);
extern GLuint APIENTRY glCreateProgram (void);
extern GLuint APIENTRY glCreateShader (GLenum type);
extern void APIENTRY glDeleteProgram (GLuint program);
extern void APIENTRY glDeleteShader (GLuint shader);
extern void APIENTRY glDetachShader (GLuint program, GLuint shader);
extern void APIENTRY glDisableVertexAttribArray (GLuint index);
extern void APIENTRY glEnableVertexAttribArray (GLuint index);
extern void APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
extern GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar *name);
extern void APIENTRY glGetProgramiv (GLuint program, GLenum pname, GLint *params);
extern void APIENTRY glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint *params);
extern void APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void APIENTRY glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar *name);
extern void APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat *params);
extern void APIENTRY glGetUniformiv (GLuint program, GLint location, GLint *params);
extern void APIENTRY glGetVertexAttribdv (GLuint index, GLenum pname, GLdouble *params);
extern void APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params);
extern void APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params);
extern void APIENTRY glGetVertexAttribPointerv (GLuint index, GLenum pname, GLvoid* *pointer);
extern GLboolean APIENTRY glIsProgram (GLuint program);
extern GLboolean APIENTRY glIsShader (GLuint shader);
extern void APIENTRY glLinkProgram (GLuint program);
extern void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
extern void APIENTRY glUseProgram (GLuint program);
extern void APIENTRY glUniform1f (GLint location, GLfloat v0);
extern void APIENTRY glUniform2f (GLint location, GLfloat v0, GLfloat v1);
extern void APIENTRY glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void APIENTRY glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void APIENTRY glUniform1i (GLint location, GLint v0);
extern void APIENTRY glUniform2i (GLint location, GLint v0, GLint v1);
extern void APIENTRY glUniform3i (GLint location, GLint v0, GLint v1, GLint v2);
extern void APIENTRY glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat *value);
extern void APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat *value);
extern void APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat *value);
extern void APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat *value);
extern void APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint *value);
extern void APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint *value);
extern void APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint *value);
extern void APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint *value);
extern void APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void APIENTRY glValidateProgram (GLuint program);
extern void APIENTRY glVertexAttrib1d (GLuint index, GLdouble x);
extern void APIENTRY glVertexAttrib1dv (GLuint index, const GLdouble *v);
extern void APIENTRY glVertexAttrib1f (GLuint index, GLfloat x);
extern void APIENTRY glVertexAttrib1fv (GLuint index, const GLfloat *v);
extern void APIENTRY glVertexAttrib1s (GLuint index, GLshort x);
extern void APIENTRY glVertexAttrib1sv (GLuint index, const GLshort *v);
extern void APIENTRY glVertexAttrib2d (GLuint index, GLdouble x, GLdouble y);
extern void APIENTRY glVertexAttrib2dv (GLuint index, const GLdouble *v);
extern void APIENTRY glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y);
extern void APIENTRY glVertexAttrib2fv (GLuint index, const GLfloat *v);
extern void APIENTRY glVertexAttrib2s (GLuint index, GLshort x, GLshort y);
extern void APIENTRY glVertexAttrib2sv (GLuint index, const GLshort *v);
extern void APIENTRY glVertexAttrib3d (GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void APIENTRY glVertexAttrib3dv (GLuint index, const GLdouble *v);
extern void APIENTRY glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void APIENTRY glVertexAttrib3fv (GLuint index, const GLfloat *v);
extern void APIENTRY glVertexAttrib3s (GLuint index, GLshort x, GLshort y, GLshort z);
extern void APIENTRY glVertexAttrib3sv (GLuint index, const GLshort *v);
extern void APIENTRY glVertexAttrib4Nbv (GLuint index, const GLbyte *v);
extern void APIENTRY glVertexAttrib4Niv (GLuint index, const GLint *v);
extern void APIENTRY glVertexAttrib4Nsv (GLuint index, const GLshort *v);
extern void APIENTRY glVertexAttrib4Nub (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void APIENTRY glVertexAttrib4Nubv (GLuint index, const GLubyte *v);
extern void APIENTRY glVertexAttrib4Nuiv (GLuint index, const GLuint *v);
extern void APIENTRY glVertexAttrib4Nusv (GLuint index, const GLushort *v);
extern void APIENTRY glVertexAttrib4bv (GLuint index, const GLbyte *v);
extern void APIENTRY glVertexAttrib4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void APIENTRY glVertexAttrib4dv (GLuint index, const GLdouble *v);
extern void APIENTRY glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void APIENTRY glVertexAttrib4fv (GLuint index, const GLfloat *v);
extern void APIENTRY glVertexAttrib4iv (GLuint index, const GLint *v);
extern void APIENTRY glVertexAttrib4s (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void APIENTRY glVertexAttrib4sv (GLuint index, const GLshort *v);
extern void APIENTRY glVertexAttrib4ubv (GLuint index, const GLubyte *v);
extern void APIENTRY glVertexAttrib4uiv (GLuint index, const GLuint *v);
extern void APIENTRY glVertexAttrib4usv (GLuint index, const GLushort *v);
extern void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

#endif /* defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) */
#endif /* GL_VERSION_2_0 */


/*-------------------------------------------------------------------*/
/*------------EXTENSIONS---------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*------------GL_3DFX_MULTISAMPLE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_3DFX_multisample
#define GL_3DFX_multisample 1

#define GL_MULTISAMPLE_3DFX										0x86B2
#define GL_SAMPLE_BUFFERS_3DFX									0x86B3
#define GL_SAMPLES_3DFX											0x86B4
#define GL_MULTISAMPLE_BIT_3DFX									0x20000000

#endif /* GL_3DFX_multisample */

/*-------------------------------------------------------------------*/
/*------------GL_3DFX_TEXTURE_COMPRESSION_FXT1-----------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_3DFX_texture_compression_FXT1
#define GL_3DFX_texture_compression_FXT1 1

#define GL_COMPRESSED_RGB_FXT1_3DFX								0x86B0
#define GL_COMPRESSED_RGBA_FXT1_3DFX							0x86B1

#endif /* GL_3DFX_texture_compression_FXT1 */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_CLIENT_STORAGE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_client_storage
#define GL_APPLE_client_storage 1

#define GL_UNPACK_CLIENT_STORAGE_APPLE							0x85B2

#endif /* GL_APPLE_client_storage */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_ELEMENT_ARRAY---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_element_array
#define GL_APPLE_element_array 1

#define GL_ELEMENT_ARRAY_APPLE									0x8768
#define GL_ELEMENT_ARRAY_TYPE_APPLE								0x8769
#define GL_ELEMENT_ARRAY_POINTER_APPLE							0x876A

typedef void (APIENTRY * glElementPointerAPPLEPROC) (GLenum type, const GLvoid *pointer);
typedef void (APIENTRY * glDrawElementArrayAPPLEPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * glDrawRangeElementArrayAPPLEPROC) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void (APIENTRY * glMultiDrawElementArrayAPPLEPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * glMultiDrawRangeElementArrayAPPLEPROC) (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);

extern glElementPointerAPPLEPROC glElementPointerAPPLE;
extern glDrawElementArrayAPPLEPROC glDrawElementArrayAPPLE;
extern glDrawRangeElementArrayAPPLEPROC glDrawRangeElementArrayAPPLE;
extern glMultiDrawElementArrayAPPLEPROC glMultiDrawElementArrayAPPLE;
extern glMultiDrawRangeElementArrayAPPLEPROC glMultiDrawRangeElementArrayAPPLE;

#endif /* GL_APPLE_element_array */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_FENCE-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_fence
#define GL_APPLE_fence 1

#define GL_DRAW_PIXELS_APPLE									0x8A0A
#define GL_FENCE_APPLE											0x8A0B

typedef void (APIENTRY * glGenFencesAPPLEPROC) (GLsizei n, GLuint *fences);
typedef void (APIENTRY * glDeleteFencesAPPLEPROC) (GLsizei n, const GLuint *fences);
typedef void (APIENTRY * glSetFenceAPPLEPROC) (GLuint fence);
typedef GLboolean (APIENTRY * glIsFenceAPPLEPROC) (GLuint fence);
typedef GLboolean (APIENTRY * glTestFenceAPPLEPROC) (GLuint fence);
typedef void (APIENTRY * glFinishFenceAPPLEPROC) (GLuint fence);
typedef GLboolean (APIENTRY * glTestObjectAPPLEPROC) (GLenum object, GLuint name);
typedef void (APIENTRY * glFinishObjectAPPLEPROC) (GLenum object, GLint name);

extern glGenFencesAPPLEPROC glGenFencesAPPLE;
extern glDeleteFencesAPPLEPROC glDeleteFencesAPPLE;
extern glSetFenceAPPLEPROC glSetFenceAPPLE;
extern glIsFenceAPPLEPROC glIsFenceAPPLE;
extern glTestFenceAPPLEPROC glTestFenceAPPLE;
extern glFinishFenceAPPLEPROC glFinishFenceAPPLE;
extern glTestObjectAPPLEPROC glTestObjectAPPLE;
extern glFinishObjectAPPLEPROC glFinishObjectAPPLE;

#endif /* GL_APPLE_fence */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_SPECULAR_VECTOR-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_specular_vector
#define GL_APPLE_specular_vector 1

#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE					0x85B0

#endif /* GL_APPLE_specular_vector */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_TRANSFORM_HINT--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_transform_hint
#define GL_APPLE_transform_hint 1

#define GL_TRANSFORM_HINT_APPLE									0x85B1

#endif /* GL_APPLE_transform_hint */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_VERTEX_ARRAY_OBJECT---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_vertex_array_object
#define GL_APPLE_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING_APPLE							0x85B5

typedef void (APIENTRY * glBindVertexArrayAPPLEPROC) (GLuint array);
typedef void (APIENTRY * glDeleteVertexArraysAPPLEPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY * glGenVertexArraysAPPLEPROC) (GLsizei n, const GLuint *arrays);
typedef GLboolean (APIENTRY * glIsVertexArrayAPPLEPROC) (GLuint array);

extern glBindVertexArrayAPPLEPROC glBindVertexArrayAPPLE;
extern glDeleteVertexArraysAPPLEPROC glDeleteVertexArraysAPPLE;
extern glGenVertexArraysAPPLEPROC glGenVertexArraysAPPLE;
extern glIsVertexArrayAPPLEPROC glIsVertexArrayAPPLE;

#endif /* GL_APPLE_vertex_array_object */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_VERTEX_ARRAY_RANGE----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_vertex_array_range
#define GL_APPLE_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_APPLE								0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE						0x851E
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE						0x851F
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE						0x8521
#define GL_STORAGE_CACHED_APPLE									0x85BE
#define GL_STORAGE_SHARED_APPLE									0x85BF

typedef void (APIENTRY * glVertexArrayRangeAPPLEPROC) (GLsizei length, GLvoid *pointer);
typedef void (APIENTRY * glFlushVertexArrayRangeAPPLEPROC) (GLsizei length, GLvoid *pointer);
typedef void (APIENTRY * glVertexArrayParameteriAPPLEPROC) (GLenum pname, GLint param);

extern glVertexArrayRangeAPPLEPROC glVertexArrayRangeAPPLE;
extern glFlushVertexArrayRangeAPPLEPROC glFlushVertexArrayRangeAPPLE;
extern glVertexArrayParameteriAPPLEPROC glVertexArrayParameteriAPPLE;

#endif /* GL_APPLE_vertex_array_range */

/*-------------------------------------------------------------------*/
/*------------GL_APPLE_YCBCR_422-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_APPLE_ycbcr_422
#define GL_APPLE_ycbcr_422 1

#define GL_YCBCR_422_APPLE										0x85B9
#define GL_UNSIGNED_SHORT_8_8_APPLE								0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE							0x85BB

#endif /* GL_APPLE_ycbcr_422 */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_DEPTH_TEXTURE-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture 1

#define GL_DEPTH_COMPONENT16_ARB                                0x81A5
#define GL_DEPTH_COMPONENT24_ARB                                0x81A6
#define GL_DEPTH_COMPONENT32_ARB                                0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB                               0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB                               0x884B

#endif /* GL_ARB_depth_texture */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_FRAGMENT_PROGRAM--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program 1

/* no new entry points, all of ARB_vertex_program reused */

#define GL_FRAGMENT_PROGRAM_ARB                                 0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB                         0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB                         0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB                         0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB                  0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB                  0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB                  0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB                     0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB                     0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB                     0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB              0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB              0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB              0x8810
#define GL_MAX_TEXTURE_COORDS_ARB                               0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB                          0x8872

#endif /* GL_ARB_fragment_program */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_MATRIX_PALETTE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_matrix_palette
#define GL_ARB_matrix_palette 1

#define GL_MATRIX_PALETTE_ARB                                   0x8840
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB                   0x8841
#define GL_MAX_PALETTE_MATRICES_ARB                             0x8842
#define GL_CURRENT_PALETTE_MATRIX_ARB                           0x8843
#define GL_MATRIX_INDEX_ARRAY_ARB                               0x8844
#define GL_CURRENT_MATRIX_INDEX_ARB                             0x8845
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB                          0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB                          0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB                        0x8848
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB                       0x8849

typedef void (APIENTRY * glCurrentPaletteMatrixARBPROC) (GLint index);
typedef void (APIENTRY * glMatrixIndexubvARBPROC) (GLint size, GLubyte *indices);
typedef void (APIENTRY * glMatrixIndexusvARBPROC) (GLint size, GLushort *indices);
typedef void (APIENTRY * glMatrixIndexuivARBPROC) (GLint size, GLuint *indices);
typedef void (APIENTRY * glMatrixIndexPointerARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);

extern glCurrentPaletteMatrixARBPROC glCurrentPaletteMatrixARB;
extern glMatrixIndexubvARBPROC glMatrixIndexubvARB;
extern glMatrixIndexusvARBPROC glMatrixIndexusvARB;
extern glMatrixIndexuivARBPROC glMatrixIndexuivARB;
extern glMatrixIndexPointerARBPROC glMatrixIndexPointerARB;

#endif /* GL_ARB_matrix_palette */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_MULTISAMPLE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_multisample
#define GL_ARB_multisample 1

#define GL_MULTISAMPLE_ARB                                      0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB                         0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB                              0x809F
#define GL_SAMPLE_COVERAGE_ARB                                  0x80A0
#define GL_SAMPLE_BUFFERS_ARB                                   0x80A8
#define GL_SAMPLES_ARB                                          0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB                            0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB                           0x80AB
#define GL_MULTISAMPLE_BIT_ARB                                  0x20000000

/* GLX constants for multisample */
#if !defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#define GLX_SAMPLE_BUFFERS_ARB                                  100000
#define GLX_SAMPLES_ARB                                         100001
#endif /* _WIN32 */ 

typedef void (APIENTRY * glSampleCoverageARBPROC) (GLclampf value, GLboolean invert );

extern glSampleCoverageARBPROC glSampleCoverageARB;

#endif /* GL_ARB_multisample */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_MULTITEXTURE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1
#define GL_TEXTURE0_ARB                                         0x84C0
#define GL_TEXTURE1_ARB                                         0x84C1
#define GL_TEXTURE2_ARB                                         0x84C2
#define GL_TEXTURE3_ARB                                         0x84C3
#define GL_TEXTURE4_ARB                                         0x84C4
#define GL_TEXTURE5_ARB                                         0x84C5
#define GL_TEXTURE6_ARB                                         0x84C6
#define GL_TEXTURE7_ARB                                         0x84C7
#define GL_TEXTURE8_ARB                                         0x84C8
#define GL_TEXTURE9_ARB                                         0x84C9
#define GL_TEXTURE10_ARB                                        0x84CA
#define GL_TEXTURE11_ARB                                        0x84CB
#define GL_TEXTURE12_ARB                                        0x84CC
#define GL_TEXTURE13_ARB                                        0x84CD
#define GL_TEXTURE14_ARB                                        0x84CE
#define GL_TEXTURE15_ARB                                        0x84CF
#define GL_TEXTURE16_ARB                                        0x84D0
#define GL_TEXTURE17_ARB                                        0x84D1
#define GL_TEXTURE18_ARB                                        0x84D2
#define GL_TEXTURE19_ARB                                        0x84D3
#define GL_TEXTURE20_ARB                                        0x84D4
#define GL_TEXTURE21_ARB                                        0x84D5
#define GL_TEXTURE22_ARB                                        0x84D6
#define GL_TEXTURE23_ARB                                        0x84D7
#define GL_TEXTURE24_ARB                                        0x84D8
#define GL_TEXTURE25_ARB                                        0x84D9
#define GL_TEXTURE26_ARB                                        0x84DA
#define GL_TEXTURE27_ARB                                        0x84DB
#define GL_TEXTURE28_ARB                                        0x84DC
#define GL_TEXTURE29_ARB                                        0x84DD
#define GL_TEXTURE30_ARB                                        0x84DE
#define GL_TEXTURE31_ARB                                        0x84DF
#define GL_ACTIVE_TEXTURE_ARB                                   0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB                            0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB                                0x84E2

typedef void (APIENTRY * glActiveTextureARBPROC) (GLenum texture );
typedef void (APIENTRY * glClientActiveTextureARBPROC) (GLenum texture );
typedef void (APIENTRY * glMultiTexCoord1dARBPROC) (GLenum target, GLdouble s );
typedef void (APIENTRY * glMultiTexCoord1dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord1fARBPROC) (GLenum target, GLfloat s );
typedef void (APIENTRY * glMultiTexCoord1fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord1iARBPROC) (GLenum target, GLint s );
typedef void (APIENTRY * glMultiTexCoord1ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord1sARBPROC) (GLenum target, GLshort s );
typedef void (APIENTRY * glMultiTexCoord1svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord2dARBPROC) (GLenum target, GLdouble s, GLdouble t );
typedef void (APIENTRY * glMultiTexCoord2dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord2fARBPROC) (GLenum target, GLfloat s, GLfloat t );
typedef void (APIENTRY * glMultiTexCoord2fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord2iARBPROC) (GLenum target, GLint s, GLint t );
typedef void (APIENTRY * glMultiTexCoord2ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord2sARBPROC) (GLenum target, GLshort s, GLshort t );
typedef void (APIENTRY * glMultiTexCoord2svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord3dARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r );
typedef void (APIENTRY * glMultiTexCoord3dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord3fARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r );
typedef void (APIENTRY * glMultiTexCoord3fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord3iARBPROC) (GLenum target, GLint s, GLint t, GLint r );
typedef void (APIENTRY * glMultiTexCoord3ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord3sARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r );
typedef void (APIENTRY * glMultiTexCoord3svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord4dARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
typedef void (APIENTRY * glMultiTexCoord4dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord4fARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
typedef void (APIENTRY * glMultiTexCoord4fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord4iARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q );
typedef void (APIENTRY * glMultiTexCoord4ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord4sARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
typedef void (APIENTRY * glMultiTexCoord4svARBPROC) (GLenum target, const GLshort *v );

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

extern glActiveTextureARBPROC glActiveTextureARB;
extern glClientActiveTextureARBPROC glClientActiveTextureARB;
extern glMultiTexCoord1dARBPROC glMultiTexCoord1dARB;
extern glMultiTexCoord1dvARBPROC glMultiTexCoord1dvARB;
extern glMultiTexCoord1fARBPROC glMultiTexCoord1fARB;
extern glMultiTexCoord1fvARBPROC glMultiTexCoord1fvARB;
extern glMultiTexCoord1iARBPROC glMultiTexCoord1iARB;
extern glMultiTexCoord1ivARBPROC glMultiTexCoord1ivARB;
extern glMultiTexCoord1sARBPROC glMultiTexCoord1sARB;
extern glMultiTexCoord1svARBPROC glMultiTexCoord1svARB;
extern glMultiTexCoord2dARBPROC glMultiTexCoord2dARB;
extern glMultiTexCoord2dvARBPROC glMultiTexCoord2dvARB;
extern glMultiTexCoord2fARBPROC glMultiTexCoord2fARB;
extern glMultiTexCoord2fvARBPROC glMultiTexCoord2fvARB;
extern glMultiTexCoord2iARBPROC glMultiTexCoord2iARB;
extern glMultiTexCoord2ivARBPROC glMultiTexCoord2ivARB;
extern glMultiTexCoord2sARBPROC glMultiTexCoord2sARB;
extern glMultiTexCoord2svARBPROC glMultiTexCoord2svARB;
extern glMultiTexCoord3dARBPROC glMultiTexCoord3dARB;
extern glMultiTexCoord3dvARBPROC glMultiTexCoord3dvARB;
extern glMultiTexCoord3fARBPROC glMultiTexCoord3fARB;
extern glMultiTexCoord3fvARBPROC glMultiTexCoord3fvARB;
extern glMultiTexCoord3iARBPROC glMultiTexCoord3iARB;
extern glMultiTexCoord3ivARBPROC glMultiTexCoord3ivARB;
extern glMultiTexCoord3sARBPROC glMultiTexCoord3sARB;
extern glMultiTexCoord3svARBPROC glMultiTexCoord3svARB;
extern glMultiTexCoord4dARBPROC glMultiTexCoord4dARB;
extern glMultiTexCoord4dvARBPROC glMultiTexCoord4dvARB;
extern glMultiTexCoord4fARBPROC glMultiTexCoord4fARB;
extern glMultiTexCoord4fvARBPROC glMultiTexCoord4fvARB;
extern glMultiTexCoord4iARBPROC glMultiTexCoord4iARB;
extern glMultiTexCoord4ivARBPROC glMultiTexCoord4ivARB;
extern glMultiTexCoord4sARBPROC glMultiTexCoord4sARB;
extern glMultiTexCoord4svARBPROC glMultiTexCoord4svARB;

#else

extern void APIENTRY glActiveTextureARB (GLenum texture );
extern void APIENTRY glClientActiveTextureARB (GLenum texture );
extern void APIENTRY glMultiTexCoord1dARB (GLenum target, GLdouble s );
extern void APIENTRY glMultiTexCoord1dvARB (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord1fARB (GLenum target, GLfloat s );
extern void APIENTRY glMultiTexCoord1fvARB (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord1iARB (GLenum target, GLint s );
extern void APIENTRY glMultiTexCoord1ivARB (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord1sARB (GLenum target, GLshort s );
extern void APIENTRY glMultiTexCoord1svARB (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord2dARB (GLenum target, GLdouble s, GLdouble t );
extern void APIENTRY glMultiTexCoord2dvARB (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord2fARB (GLenum target, GLfloat s, GLfloat t );
extern void APIENTRY glMultiTexCoord2fvARB (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord2iARB (GLenum target, GLint s, GLint t );
extern void APIENTRY glMultiTexCoord2ivARB (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord2sARB (GLenum target, GLshort s, GLshort t );
extern void APIENTRY glMultiTexCoord2svARB (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord3dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r );
extern void APIENTRY glMultiTexCoord3dvARB (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord3fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r );
extern void APIENTRY glMultiTexCoord3fvARB (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord3iARB (GLenum target, GLint s, GLint t, GLint r );
extern void APIENTRY glMultiTexCoord3ivARB (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord3sARB (GLenum target, GLshort s, GLshort t, GLshort r );
extern void APIENTRY glMultiTexCoord3svARB (GLenum target, const GLshort *v );
extern void APIENTRY glMultiTexCoord4dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
extern void APIENTRY glMultiTexCoord4dvARB (GLenum target, const GLdouble *v );
extern void APIENTRY glMultiTexCoord4fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
extern void APIENTRY glMultiTexCoord4fvARB (GLenum target, const GLfloat *v );
extern void APIENTRY glMultiTexCoord4iARB (GLenum target, GLint s, GLint t, GLint r, GLint q );
extern void APIENTRY glMultiTexCoord4ivARB (GLenum target, const GLint *v );
extern void APIENTRY glMultiTexCoord4sARB (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
extern void APIENTRY glMultiTexCoord4svARB (GLenum target, const GLshort *v );

#endif /* WIN32 */

#endif /* GL_ARB_multitexture */


/*-------------------------------------------------------------------*/
/*------------GL_ARB_POINT_PARAMETERS--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_point_parameters
#define GL_ARB_point_parameters 1

#define GL_POINT_SIZE_MIN_ARB                                   0x8126
#define GL_POINT_SIZE_MAX_ARB                                   0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB                        0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB                       0x8129

typedef void (APIENTRY * glPointParameterfARBPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glPointParameterfvARBPROC) (GLenum pname, GLfloat *params);

extern glPointParameterfARBPROC glPointParameterfARB;
extern glPointParameterfvARBPROC glPointParameterfvARB;

#endif /* GL_ARB_point_parameters */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_SHADOW------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_shadow
#define GL_ARB_shadow 1

#define GL_TEXTURE_COMPARE_MODE_ARB                             0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB                             0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB                             0x884E

#endif /* GL_ARB_shadow */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_SHADOW_AMBIENT----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient 1

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB                       0x80BF 

#endif /* GL_ARB_shadow_ambient */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_WINDOW_POS--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_window_pos
#define GL_ARB_window_pos 1

typedef void (APIENTRY * glWindowPos2dARBPROC) (GLdouble x, GLdouble y);
typedef void (APIENTRY * glWindowPos2fARBPROC) (GLfloat x, GLfloat y);
typedef void (APIENTRY * glWindowPos2iARBPROC) (GLint x, GLint y);
typedef void (APIENTRY * glWindowPos2sARBPROC) (GLshort x, GLshort y);
typedef void (APIENTRY * glWindowPos2dvARBPROC) (const GLdouble *p);
typedef void (APIENTRY * glWindowPos2fvARBPROC) (const GLfloat *p);
typedef void (APIENTRY * glWindowPos2ivARBPROC) (const GLint *p);
typedef void (APIENTRY * glWindowPos2svARBPROC) (const GLshort *p);
typedef void (APIENTRY * glWindowPos3dARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glWindowPos3fARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glWindowPos3iARBPROC) (GLint x, GLint y, GLint z);
typedef void (APIENTRY * glWindowPos3sARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glWindowPos3dvARBPROC) (const GLdouble *p);
typedef void (APIENTRY * glWindowPos3fvARBPROC) (const GLfloat *p);
typedef void (APIENTRY * glWindowPos3ivARBPROC) (const GLint *p);
typedef void (APIENTRY * glWindowPos3svARBPROC) (const GLshort *p);

extern glWindowPos2dARBPROC glWindowPos2dARB;
extern glWindowPos2fARBPROC glWindowPos2fARB;
extern glWindowPos2iARBPROC glWindowPos2iARB;
extern glWindowPos2sARBPROC glWindowPos2sARB;
extern glWindowPos2dvARBPROC glWindowPos2dvARB;
extern glWindowPos2fvARBPROC glWindowPos2fvARB;
extern glWindowPos2ivARBPROC glWindowPos2ivARB;
extern glWindowPos2svARBPROC glWindowPos2svARB;
extern glWindowPos3dARBPROC glWindowPos3dARB;
extern glWindowPos3fARBPROC glWindowPos3fARB;
extern glWindowPos3iARBPROC glWindowPos3iARB;
extern glWindowPos3sARBPROC glWindowPos3sARB;
extern glWindowPos3dvARBPROC glWindowPos3dvARB;
extern glWindowPos3fvARBPROC glWindowPos3fvARB;
extern glWindowPos3ivARBPROC glWindowPos3ivARB;
extern glWindowPos3svARBPROC glWindowPos3svARB;

#endif /* GL_ARB_window_pos */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_VERTEX_PROGRAM----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program 1

#define GL_COLOR_SUM_ARB                                        0x8458
#define GL_VERTEX_PROGRAM_ARB                                   0x8620
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB                      0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB                         0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB                       0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB                         0x8625
#define GL_CURRENT_VERTEX_ATTRIB_ARB                            0x8626
#define GL_PROGRAM_LENGTH_ARB                                   0x8627
#define GL_PROGRAM_STRING_ARB                                   0x8628
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB                   0x862E
#define GL_MAX_PROGRAM_MATRICES_ARB                             0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB                       0x8640
#define GL_CURRENT_MATRIX_ARB                                   0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB                        0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB                          0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB                      0x8645
#define GL_PROGRAM_ERROR_POSITION_ARB                           0x864B
#define GL_PROGRAM_BINDING_ARB                                  0x8677
#define GL_MAX_VERTEX_ATTRIBS_ARB                               0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB                   0x886A
#define GL_PROGRAM_ERROR_STRING_ARB                             0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB                             0x8875
#define GL_PROGRAM_FORMAT_ARB                                   0x8876
#define GL_PROGRAM_INSTRUCTIONS_ARB                             0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB                         0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB                      0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB                  0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB                              0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB                          0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB                       0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB                   0x88A7
#define GL_PROGRAM_PARAMETERS_ARB                               0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB                           0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB                        0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB                    0x88AB
#define GL_PROGRAM_ATTRIBS_ARB                                  0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB                              0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB                           0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB                       0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB                        0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB                    0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB                 0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB             0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB                     0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB                       0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB                      0x88B6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB                         0x88B7
#define GL_MATRIX0_ARB                                          0x88C0
#define GL_MATRIX1_ARB                                          0x88C1
#define GL_MATRIX2_ARB                                          0x88C2
#define GL_MATRIX3_ARB                                          0x88C3
#define GL_MATRIX4_ARB                                          0x88C4
#define GL_MATRIX5_ARB                                          0x88C5
#define GL_MATRIX6_ARB                                          0x88C6
#define GL_MATRIX7_ARB                                          0x88C7
#define GL_MATRIX8_ARB                                          0x88C8
#define GL_MATRIX9_ARB                                          0x88C9
#define GL_MATRIX10_ARB                                         0x88CA
#define GL_MATRIX11_ARB                                         0x88CB
#define GL_MATRIX12_ARB                                         0x88CC
#define GL_MATRIX13_ARB                                         0x88CD
#define GL_MATRIX14_ARB                                         0x88CE
#define GL_MATRIX15_ARB                                         0x88CF
#define GL_MATRIX16_ARB                                         0x88D0
#define GL_MATRIX17_ARB                                         0x88D1
#define GL_MATRIX18_ARB                                         0x88D2
#define GL_MATRIX19_ARB                                         0x88D3
#define GL_MATRIX20_ARB                                         0x88D4
#define GL_MATRIX21_ARB                                         0x88D5
#define GL_MATRIX22_ARB                                         0x88D6
#define GL_MATRIX23_ARB                                         0x88D7
#define GL_MATRIX24_ARB                                         0x88D8
#define GL_MATRIX25_ARB                                         0x88D9
#define GL_MATRIX26_ARB                                         0x88DA
#define GL_MATRIX27_ARB                                         0x88DB
#define GL_MATRIX28_ARB                                         0x88DC
#define GL_MATRIX29_ARB                                         0x88DD
#define GL_MATRIX30_ARB                                         0x88DE
#define GL_MATRIX31_ARB                                         0x88DF

typedef void (APIENTRY * glVertexAttrib1sARBPROC) (GLuint index, GLshort x);
typedef void (APIENTRY * glVertexAttrib1fARBPROC) (GLuint index, GLfloat x);
typedef void (APIENTRY * glVertexAttrib1dARBPROC) (GLuint index, GLdouble x);
typedef void (APIENTRY * glVertexAttrib2sARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (APIENTRY * glVertexAttrib2fARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRY * glVertexAttrib2dARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRY * glVertexAttrib3sARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glVertexAttrib3fARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glVertexAttrib3dARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glVertexAttrib4sARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glVertexAttrib4fARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glVertexAttrib4dARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glVertexAttrib4NubARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (APIENTRY * glVertexAttrib1svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib1fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib1dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib2svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib2fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib2dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib3svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib3fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib3dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4bvARBPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4ivARBPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4ubvARBPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4usvARBPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttrib4uivARBPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttrib4fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib4dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4NbvARBPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4NsvARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4NivARBPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4NubvARBPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4NusvARBPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttrib4NuivARBPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttribPointerARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * glEnableVertexAttribArrayARBPROC) (GLuint index);
typedef void (APIENTRY * glDisableVertexAttribArrayARBPROC) (GLuint index);
typedef void (APIENTRY * glProgramStringARBPROC) (GLenum target, GLenum format, GLsizei len, const GLvoid *string); 
typedef void (APIENTRY * glBindProgramARBPROC) (GLenum target, GLuint program);
typedef void (APIENTRY * glDeleteProgramsARBPROC) (GLsizei n, const GLuint *programs);
typedef void (APIENTRY * glGenProgramsARBPROC) (GLsizei n, GLuint *programs);
typedef void (APIENTRY * glProgramEnvParameter4dARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramEnvParameter4dvARBPROC) (GLenum target, GLuint index, const GLdouble *params);
typedef void (APIENTRY * glProgramEnvParameter4fARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramEnvParameter4fvARBPROC) (GLenum target, GLuint index, const GLfloat *params);
typedef void (APIENTRY * glProgramLocalParameter4dARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramLocalParameter4dvARBPROC) (GLenum target, GLuint index, const GLdouble *params);
typedef void (APIENTRY * glProgramLocalParameter4fARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramLocalParameter4fvARBPROC) (GLenum target, GLuint index, const GLfloat *params);
typedef void (APIENTRY * glGetProgramEnvParameterdvARBPROC) (GLenum target, GLuint index, GLdouble *params);
typedef void (APIENTRY * glGetProgramEnvParameterfvARBPROC) (GLenum target, GLuint index, GLfloat *params);
typedef void (APIENTRY * glGetProgramLocalParameterdvARBPROC) (GLenum target, GLuint index, GLdouble *params);
typedef void (APIENTRY * glGetProgramLocalParameterfvARBPROC) (GLenum target, GLuint index, GLfloat *params);
typedef void (APIENTRY * glGetProgramivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetProgramStringARBPROC) (GLenum target, GLenum pname, GLvoid *string);
typedef void (APIENTRY * glGetVertexAttribdvARBPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRY * glGetVertexAttribfvARBPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVertexAttribivARBPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetVertexAttribPointervARBPROC) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (APIENTRY * glIsProgramARBPROC) (GLuint program);

extern glVertexAttrib1sARBPROC glVertexAttrib1sARB;
extern glVertexAttrib1fARBPROC glVertexAttrib1fARB;
extern glVertexAttrib1dARBPROC glVertexAttrib1dARB;
extern glVertexAttrib2sARBPROC glVertexAttrib2sARB;
extern glVertexAttrib2fARBPROC glVertexAttrib2fARB;
extern glVertexAttrib2dARBPROC glVertexAttrib2dARB;
extern glVertexAttrib3sARBPROC glVertexAttrib3sARB;
extern glVertexAttrib3fARBPROC glVertexAttrib3fARB;
extern glVertexAttrib3dARBPROC glVertexAttrib3dARB;
extern glVertexAttrib4sARBPROC glVertexAttrib4sARB;
extern glVertexAttrib4fARBPROC glVertexAttrib4fARB;
extern glVertexAttrib4dARBPROC glVertexAttrib4dARB;
extern glVertexAttrib4NubARBPROC glVertexAttrib4NubARB;
extern glVertexAttrib1svARBPROC glVertexAttrib1svARB;
extern glVertexAttrib1fvARBPROC glVertexAttrib1fvARB;
extern glVertexAttrib1dvARBPROC glVertexAttrib1dvARB;
extern glVertexAttrib2svARBPROC glVertexAttrib2svARB;
extern glVertexAttrib2fvARBPROC glVertexAttrib2fvARB;
extern glVertexAttrib2dvARBPROC glVertexAttrib2dvARB;
extern glVertexAttrib3svARBPROC glVertexAttrib3svARB;
extern glVertexAttrib3fvARBPROC glVertexAttrib3fvARB;
extern glVertexAttrib3dvARBPROC glVertexAttrib3dvARB;
extern glVertexAttrib4bvARBPROC glVertexAttrib4bvARB;
extern glVertexAttrib4svARBPROC glVertexAttrib4svARB;
extern glVertexAttrib4ivARBPROC glVertexAttrib4ivARB;
extern glVertexAttrib4ubvARBPROC glVertexAttrib4ubvARB;
extern glVertexAttrib4usvARBPROC glVertexAttrib4usvARB;
extern glVertexAttrib4uivARBPROC glVertexAttrib4uivARB;
extern glVertexAttrib4fvARBPROC glVertexAttrib4fvARB;
extern glVertexAttrib4dvARBPROC glVertexAttrib4dvARB;
extern glVertexAttrib4NbvARBPROC glVertexAttrib4NbvARB;
extern glVertexAttrib4NsvARBPROC glVertexAttrib4NsvARB;
extern glVertexAttrib4NivARBPROC glVertexAttrib4NivARB;
extern glVertexAttrib4NubvARBPROC glVertexAttrib4NubvARB;
extern glVertexAttrib4NusvARBPROC glVertexAttrib4NusvARB;
extern glVertexAttrib4NuivARBPROC glVertexAttrib4NuivARB;
extern glVertexAttribPointerARBPROC glVertexAttribPointerARB;
extern glEnableVertexAttribArrayARBPROC glEnableVertexAttribArrayARB;
extern glDisableVertexAttribArrayARBPROC glDisableVertexAttribArrayARB;
extern glProgramStringARBPROC glProgramStringARB;
extern glBindProgramARBPROC glBindProgramARB;
extern glDeleteProgramsARBPROC glDeleteProgramsARB;
extern glGenProgramsARBPROC glGenProgramsARB;
extern glProgramEnvParameter4dARBPROC glProgramEnvParameter4dARB;
extern glProgramEnvParameter4dvARBPROC glProgramEnvParameter4dvARB;
extern glProgramEnvParameter4fARBPROC glProgramEnvParameter4fARB;
extern glProgramEnvParameter4fvARBPROC glProgramEnvParameter4fvARB;
extern glProgramLocalParameter4dARBPROC glProgramLocalParameter4dARB;
extern glProgramLocalParameter4dvARBPROC glProgramLocalParameter4dvARB;
extern glProgramLocalParameter4fARBPROC glProgramLocalParameter4fARB;
extern glProgramLocalParameter4fvARBPROC glProgramLocalParameter4fvARB;
extern glGetProgramEnvParameterdvARBPROC glGetProgramEnvParameterdvARB;
extern glGetProgramEnvParameterfvARBPROC glGetProgramEnvParameterfvARB;
extern glGetProgramLocalParameterdvARBPROC glGetProgramLocalParameterdvARB;
extern glGetProgramLocalParameterfvARBPROC glGetProgramLocalParameterfvARB;
extern glGetProgramivARBPROC glGetProgramivARB;
extern glGetProgramStringARBPROC glGetProgramStringARB;
extern glGetVertexAttribdvARBPROC glGetVertexAttribdvARB;
extern glGetVertexAttribfvARBPROC glGetVertexAttribfvARB;
extern glGetVertexAttribivARBPROC glGetVertexAttribivARB;
extern glGetVertexAttribPointervARBPROC glGetVertexAttribPointervARB;
extern glIsProgramARBPROC glIsProgramARB;

#endif /* GL_ARB_vertex_program */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_BORDER_CLAMP----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_border_clamp
#define GL_ARB_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_ARB                                  0x812D

#endif /* GL_ARB_texture_border_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_COMPRESSION-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression 1

#define GL_COMPRESSED_ALPHA_ARB                                 0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB                             0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB                       0x84EB
#define GL_COMPRESSED_INTENSITY_ARB                             0x84EC
#define GL_COMPRESSED_RGB_ARB                                   0x84ED
#define GL_COMPRESSED_RGBA_ARB                                  0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB                         0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB                    0x86A0
#define GL_TEXTURE_COMPRESSED_ARB                               0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB                   0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB                       0x86A3

typedef void (APIENTRY * glCompressedTexImage1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glGetCompressedTexImageARBPROC) (GLenum target, GLint lod, GLvoid *img );

extern glCompressedTexImage3DARBPROC glCompressedTexImage3DARB;
extern glCompressedTexImage2DARBPROC glCompressedTexImage2DARB;
extern glCompressedTexImage1DARBPROC glCompressedTexImage1DARB; 
extern glCompressedTexSubImage3DARBPROC glCompressedTexSubImage3DARB;
extern glCompressedTexSubImage2DARBPROC glCompressedTexSubImage2DARB;
extern glCompressedTexSubImage1DARBPROC glCompressedTexSubImage1DARB;
extern glGetCompressedTexImageARBPROC glGetCompressedTexImageARB;

#endif /* GL_ARB_texture_compression */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_CUBE_MAP--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1

#define GL_NORMAL_MAP_ARB                                       0x8511
#define GL_REFLECTION_MAP_ARB                                   0x8512
#define GL_TEXTURE_CUBE_MAP_ARB                                 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB                         0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB                      0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB                      0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB                      0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB                      0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB                      0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB                      0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB                           0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB                        0x851C

#endif /* GL_ARB_texture_cube_map */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_ENV_ADD---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_env_add
#define GL_ARB_texture_env_add 1

#endif /* GL_ARB_texture_env_add */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_ENV_COMBINE-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine 1

#define GL_COMBINE_ARB                                          0x8570
#define GL_COMBINE_RGB_ARB                                      0x8571
#define GL_COMBINE_ALPHA_ARB                                    0x8572
#define GL_RGB_SCALE_ARB                                        0x8573
#define GL_ADD_SIGNED_ARB                                       0x8574
#define GL_INTERPOLATE_ARB                                      0x8575
#define GL_CONSTANT_ARB                                         0x8576
#define GL_PRIMARY_COLOR_ARB                                    0x8577
#define GL_PREVIOUS_ARB                                         0x8578
#define GL_SOURCE0_RGB_ARB                                      0x8580
#define GL_SOURCE1_RGB_ARB                                      0x8581
#define GL_SOURCE2_RGB_ARB                                      0x8582
#define GL_SOURCE0_ALPHA_ARB                                    0x8588
#define GL_SOURCE1_ALPHA_ARB                                    0x8589
#define GL_SOURCE2_ALPHA_ARB                                    0x858A
#define GL_OPERAND0_RGB_ARB                                     0x8590
#define GL_OPERAND1_RGB_ARB                                     0x8591
#define GL_OPERAND2_RGB_ARB                                     0x8592
#define GL_OPERAND0_ALPHA_ARB                                   0x8598
#define GL_OPERAND1_ALPHA_ARB                                   0x8599
#define GL_OPERAND2_ALPHA_ARB                                   0x859A

#endif /* GL_ARB_texture_env_combine */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_ENV_CROSSBAR----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_env_crossbar
#define GL_ARB_texture_env_crossbar 1

#endif /* GL_ARB_texture_env_crossbar */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_ENV_DOT3--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_env_dot3
#define GL_ARB_texture_env_dot3 1

#define GL_DOT3_RGB_ARB                                         0x86AE
#define GL_DOT3_RGBA_ARB                                        0x86AF

#endif /* GL_ARB_texture_env_dot3 */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_MIRRORED_REPEAT-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB                                  0x8370

#endif /* GL_ARB_texture_mirrored_repeat */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TRANSPOSE_MATRIX--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix 1

#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB                       0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB                      0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB                         0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB                           0x84E6

typedef void (APIENTRY * glLoadTransposeMatrixdARBPROC) (const GLdouble m[16] );
typedef void (APIENTRY * glLoadTransposeMatrixfARBPROC) (const GLfloat m[16] );
typedef void (APIENTRY * glMultTransposeMatrixdARBPROC) (const GLdouble m[16] );
typedef void (APIENTRY * glMultTransposeMatrixfARBPROC) (const GLfloat m[16] );

extern glLoadTransposeMatrixfARBPROC glLoadTransposeMatrixfARB;
extern glLoadTransposeMatrixdARBPROC glLoadTransposeMatrixdARB;
extern glMultTransposeMatrixfARBPROC glMultTransposeMatrixfARB;
extern glMultTransposeMatrixdARBPROC glMultTransposeMatrixdARB;

#endif /* GL_ARB_transpose_matrix */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_VERTEX_BLEND------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_vertex_blend
#define GL_ARB_vertex_blend 1

#define GL_MAX_VERTEX_UNITS_ARB                                 0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB                              0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB                                 0x86A6      
#define GL_VERTEX_BLEND_ARB                                     0x86A7
#define GL_CURRENT_WEIGHT_ARB                                   0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB                                0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB                              0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB                                0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB                             0x86AC
#define GL_WEIGHT_ARRAY_ARB                                     0x86AD
#define GL_MODELVIEW0_ARB                                       0x1700
#define GL_MODELVIEW1_ARB                                       0x850A
#define GL_MODELVIEW2_ARB                                       0x8722
#define GL_MODELVIEW3_ARB                                       0x8723
#define GL_MODELVIEW4_ARB                                       0x8724
#define GL_MODELVIEW5_ARB                                       0x8725
#define GL_MODELVIEW6_ARB                                       0x8726
#define GL_MODELVIEW7_ARB                                       0x8727
#define GL_MODELVIEW8_ARB                                       0x8728
#define GL_MODELVIEW9_ARB                                       0x8729
#define GL_MODELVIEW10_ARB                                      0x872A
#define GL_MODELVIEW11_ARB                                      0x872B
#define GL_MODELVIEW12_ARB                                      0x872C
#define GL_MODELVIEW13_ARB                                      0x872D
#define GL_MODELVIEW14_ARB                                      0x872E
#define GL_MODELVIEW15_ARB                                      0x872F
#define GL_MODELVIEW16_ARB                                      0x8730
#define GL_MODELVIEW17_ARB                                      0x8731
#define GL_MODELVIEW18_ARB                                      0x8732
#define GL_MODELVIEW19_ARB                                      0x8733
#define GL_MODELVIEW20_ARB                                      0x8734
#define GL_MODELVIEW21_ARB                                      0x8735
#define GL_MODELVIEW22_ARB                                      0x8736
#define GL_MODELVIEW23_ARB                                      0x8737
#define GL_MODELVIEW24_ARB                                      0x8738
#define GL_MODELVIEW25_ARB                                      0x8739
#define GL_MODELVIEW26_ARB                                      0x873A
#define GL_MODELVIEW27_ARB                                      0x873B
#define GL_MODELVIEW28_ARB                                      0x873C
#define GL_MODELVIEW29_ARB                                      0x873D
#define GL_MODELVIEW30_ARB                                      0x873E
#define GL_MODELVIEW31_ARB                                      0x873F

typedef void (APIENTRY * glWeightbvARBPROC) (GLint size, GLbyte *weights);
typedef void (APIENTRY * glWeightsvARBPROC) (GLint size, GLshort *weights);
typedef void (APIENTRY * glWeightivARBPROC) (GLint size, GLint *weights);
typedef void (APIENTRY * glWeightfvARBPROC) (GLint size, GLfloat *weights);
typedef void (APIENTRY * glWeightdvARBPROC) (GLint size, GLdouble *weights);
typedef void (APIENTRY * glWeightubvARBPROC) (GLint size, GLubyte *weights);
typedef void (APIENTRY * glWeightusvARBPROC) (GLint size, GLushort *weights);
typedef void (APIENTRY * glWeightuivARBPROC) (GLint size, GLuint *weights);
typedef void (APIENTRY * glWeightPointerARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (APIENTRY * glVertexBlendARBPROC) (GLint count);

extern glWeightbvARBPROC glWeightbvARB;
extern glWeightsvARBPROC glWeightsvARB;
extern glWeightivARBPROC glWeightivARB;
extern glWeightfvARBPROC glWeightfvARB;
extern glWeightdvARBPROC glWeightdvARB;
extern glWeightubvARBPROC glWeightubvARB;
extern glWeightusvARBPROC glWeightusvARB;
extern glWeightuivARBPROC glWeightuivARB;
extern glWeightPointerARBPROC glWeightPointerARB;
extern glVertexBlendARBPROC glVertexBlendARB;

#endif /* GL_ARB_vertex_blend */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_VERTEX_BUFFER_OBJECT----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1

// GL types for handling large vertex buffer objects
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

#define GL_BUFFER_SIZE_ARB                              0x8764        
#define GL_BUFFER_USAGE_ARB                             0x8765        
#define GL_ARRAY_BUFFER_ARB                             0x8892        
#define GL_ELEMENT_ARRAY_BUFFER_ARB                     0x8893        
#define GL_ARRAY_BUFFER_BINDING_ARB                     0x8894        
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB             0x8895        
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB              0x8896        
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB              0x8897        
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB               0x8898        
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB               0x8899        
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB       0x889A        
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB           0x889B        
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB		0x889C             
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB      0x889D        
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB              0x889E        
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB       0x889F        
#define GL_READ_ONLY_ARB                                0x88B8        
#define GL_WRITE_ONLY_ARB                               0x88B9        
#define GL_READ_WRITE_ARB                               0x88BA        
#define GL_BUFFER_ACCESS_ARB                            0x88BB        
#define GL_BUFFER_MAPPED_ARB                            0x88BC        
#define GL_BUFFER_MAP_POINTER_ARB                       0x88BD        
#define GL_STREAM_DRAW_ARB                              0x88E0        
#define GL_STREAM_READ_ARB                              0x88E1        
#define GL_STREAM_COPY_ARB                              0x88E2        
#define GL_STATIC_DRAW_ARB                              0x88E4        
#define GL_STATIC_READ_ARB                              0x88E5        
#define GL_STATIC_COPY_ARB                              0x88E6        
#define GL_DYNAMIC_DRAW_ARB                             0x88E8        
#define GL_DYNAMIC_READ_ARB                             0x88E9        
#define GL_DYNAMIC_COPY_ARB                             0x88EA        

typedef void (APIENTRY * glBindBufferARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * glDeleteBuffersARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * glGenBuffersARBPROC) (GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * glIsBufferARBPROC) (GLuint buffer);
typedef void (APIENTRY * glBufferDataARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * glBufferSubDataARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (APIENTRY * glGetBufferSubDataARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef void* (APIENTRY * glMapBufferARBPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY * glUnmapBufferARBPROC) (GLenum target);
typedef void (APIENTRY * glGetBufferParameterivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetBufferPointervARBPROC) (GLenum target, GLenum pname, GLvoid **params);

extern glBindBufferARBPROC glBindBufferARB;
extern glDeleteBuffersARBPROC glDeleteBuffersARB;
extern glGenBuffersARBPROC glGenBuffersARB;
extern glIsBufferARBPROC glIsBufferARB;
extern glBufferDataARBPROC glBufferDataARB;
extern glBufferSubDataARBPROC glBufferSubDataARB;
extern glGetBufferSubDataARBPROC glGetBufferSubDataARB;
extern glMapBufferARBPROC glMapBufferARB;
extern glUnmapBufferARBPROC glUnmapBufferARB;
extern glGetBufferParameterivARBPROC glGetBufferParameterivARB;
extern glGetBufferPointervARBPROC glGetBufferPointervARB;

#endif /* GL_ARB_vertex_buffer_object */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_OCCLUSION_QUERY--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1

#define GL_QUERY_COUNTER_BITS_ARB								0x8864
#define GL_CURRENT_QUERY_ARB									0x8865
#define GL_QUERY_RESULT_ARB										0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB							0x8867
#define GL_SAMPLES_PASSED_ARB									0x8914

typedef void (APIENTRY * glGenQueriesARBPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glDeleteQueriesARBPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRY * glIsQueryARBPROC) (GLuint id);
typedef void (APIENTRY * glBeginQueryARBPROC) (GLenum target, GLuint id);
typedef void (APIENTRY * glEndQueryARBPROC) (GLenum target);
typedef void (APIENTRY * glGetQueryivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectivARBPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectuivARBPROC) (GLuint id, GLenum pname, GLuint *params);

extern glGenQueriesARBPROC glGenQueriesARB;
extern glDeleteQueriesARBPROC glDeleteQueriesARB;
extern glIsQueryARBPROC glIsQueryARB;
extern glBeginQueryARBPROC glBeginQueryARB;
extern glEndQueryARBPROC glEndQueryARB;
extern glGetQueryivARBPROC glGetQueryivARB;
extern glGetQueryObjectivARBPROC glGetQueryObjectivARB;
extern glGetQueryObjectuivARBPROC glGetQueryObjectuivARB;

#endif /* GL_ARB_occlusion_query */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_SHADER_OBJECTS----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects 1

#define GL_PROGRAM_OBJECT_ARB					0x8B40
#define GL_SHADER_OBJECT_ARB					0x8B48
#define GL_OBJECT_TYPE_ARB						0x8B4E
#define GL_OBJECT_SUBTYPE_ARB					0x8B4F
#define GL_FLOAT_VEC2_ARB						0x8B50
#define GL_FLOAT_VEC3_ARB						0x8B51
#define GL_FLOAT_VEC4_ARB						0x8B52
#define GL_INT_VEC2_ARB							0x8B53
#define GL_INT_VEC3_ARB							0x8B54
#define GL_INT_VEC4_ARB							0x8B55
#define GL_BOOL_ARB								0x8B56
#define GL_BOOL_VEC2_ARB						0x8B57
#define GL_BOOL_VEC3_ARB						0x8B58
#define GL_BOOL_VEC4_ARB						0x8B59
#define GL_FLOAT_MAT2_ARB						0x8B5A
#define GL_FLOAT_MAT3_ARB						0x8B5B
#define GL_FLOAT_MAT4_ARB						0x8B5C
#define GL_OBJECT_DELETE_STATUS_ARB				0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB			0x8B81
#define GL_OBJECT_LINK_STATUS_ARB				0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB			0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB			0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB			0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB			0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB	0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB		0x8B88

typedef void (APIENTRY * glDeleteObjectARBPROC) (GLhandleARB obj);
typedef GLhandleARB (APIENTRY * glGetHandleARBPROC) (GLenum pname);
typedef void (APIENTRY * glDetachObjectARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef GLhandleARB (APIENTRY * glCreateShaderObjectARBPROC) (GLenum shaderType);
typedef void (APIENTRY * glShaderSourceARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
typedef void (APIENTRY * glCompileShaderARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB (APIENTRY * glCreateProgramObjectARBPROC) (void);
typedef void (APIENTRY * glAttachObjectARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (APIENTRY * glLinkProgramARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY * glUseProgramObjectARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY * glValidateProgramARBPROC) (GLhandleARB programObj);
typedef void (APIENTRY * glUniform1fARBPROC) (GLint location, GLfloat v0);
typedef void (APIENTRY * glUniform2fARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY * glUniform3fARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY * glUniform4fARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY * glUniform1iARBPROC) (GLint location, GLint v0);
typedef void (APIENTRY * glUniform2iARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (APIENTRY * glUniform3iARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRY * glUniform4iARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRY * glUniform1fvARBPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform2fvARBPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform3fvARBPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform4fvARBPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY * glUniform1ivARBPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform2ivARBPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform3ivARBPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniform4ivARBPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRY * glUniformMatrix2fvARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glUniformMatrix3fvARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glUniformMatrix4fvARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY * glGetObjectParameterfvARBPROC) (GLhandleARB obj, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetObjectParameterivARBPROC) (GLhandleARB obj, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetInfoLogARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
typedef void (APIENTRY * glGetAttachedObjectsARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
typedef GLint (APIENTRY * glGetUniformLocationARBPROC) (GLhandleARB programObj, const GLcharARB *name);
typedef void (APIENTRY * glGetActiveUniformARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
typedef void (APIENTRY * glGetUniformfvARBPROC) (GLhandleARB programObj, GLint location, GLfloat *params);
typedef void (APIENTRY * glGetUniformivARBPROC) (GLhandleARB programObj, GLint location, GLint *params);
typedef void (APIENTRY * glGetShaderSourceARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);

extern glDeleteObjectARBPROC glDeleteObjectARB;
extern glGetHandleARBPROC glGetHandleARB;
extern glDetachObjectARBPROC glDetachObjectARB;
extern glCreateShaderObjectARBPROC glCreateShaderObjectARB;
extern glShaderSourceARBPROC glShaderSourceARB;
extern glCompileShaderARBPROC glCompileShaderARB;
extern glCreateProgramObjectARBPROC glCreateProgramObjectARB;
extern glAttachObjectARBPROC glAttachObjectARB;
extern glLinkProgramARBPROC glLinkProgramARB;
extern glUseProgramObjectARBPROC glUseProgramObjectARB;
extern glValidateProgramARBPROC glValidateProgramARB;
extern glUniform1fARBPROC glUniform1fARB;
extern glUniform2fARBPROC glUniform2fARB;
extern glUniform3fARBPROC glUniform3fARB;
extern glUniform4fARBPROC glUniform4fARB;
extern glUniform1iARBPROC glUniform1iARB;
extern glUniform2iARBPROC glUniform2iARB;
extern glUniform3iARBPROC glUniform3iARB;
extern glUniform4iARBPROC glUniform4iARB;
extern glUniform1fvARBPROC glUniform1fvARB;
extern glUniform2fvARBPROC glUniform2fvARB;
extern glUniform3fvARBPROC glUniform3fvARB;
extern glUniform4fvARBPROC glUniform4fvARB;
extern glUniform1ivARBPROC glUniform1ivARB;
extern glUniform2ivARBPROC glUniform2ivARB;
extern glUniform3ivARBPROC glUniform3ivARB;
extern glUniform4ivARBPROC glUniform4ivARB;
extern glUniformMatrix2fvARBPROC glUniformMatrix2fvARB;
extern glUniformMatrix3fvARBPROC glUniformMatrix3fvARB;
extern glUniformMatrix4fvARBPROC glUniformMatrix4fvARB;
extern glGetObjectParameterfvARBPROC glGetObjectParameterfvARB;
extern glGetObjectParameterivARBPROC glGetObjectParameterivARB;
extern glGetInfoLogARBPROC glGetInfoLogARB;
extern glGetAttachedObjectsARBPROC glGetAttachedObjectsARB;
extern glGetUniformLocationARBPROC glGetUniformLocationARB;
extern glGetActiveUniformARBPROC glGetActiveUniformARB;
extern glGetUniformfvARBPROC glGetUniformfvARB;
extern glGetUniformivARBPROC glGetUniformivARB;
extern glGetShaderSourceARBPROC glGetShaderSourceARB;

#endif /* GL_ARB_shader_objects */


/*-------------------------------------------------------------------*/
/*------------GL_ARB_VERTEX_SHADER-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_vertex_shader
#define GL_ARB_vertex_shader 1

#define GL_VERTEX_SHADER_ARB						0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB		0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB					0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB		0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB		0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB				0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB	0x8B8A

typedef void (APIENTRY * glBindAttribLocationARBPROC) (GLhandleARB programObj, GLuint index, const GLcharARB *name);
typedef void (APIENTRY * glGetActiveAttribARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
typedef GLint (APIENTRY * glGetAttribLocationARBPROC) (GLhandleARB programObj, const GLcharARB *name);

extern glBindAttribLocationARBPROC glBindAttribLocationARB;
extern glGetActiveAttribARBPROC glGetActiveAttribARB;
extern glGetAttribLocationARBPROC glGetAttribLocationARB;

#endif /* GL_ARB_vertex_shader */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_FRAGMENT_SHADER---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_fragment_shader
#define GL_ARB_fragment_shader 1

#define GL_FRAGMENT_SHADER_ARB					0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB	0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB	0x8B8B

#endif /* GL_ARB_fragment_shader */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_SHADING_LANGUAGE_100----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_shading_language_100
#define GL_ARB_shading_language_100 1

#define GL_SHADING_LANGUAGE_VERSION_ARB		0x8B8C

#endif /* GL_ARB_shading_language_100 */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_NON_POWER_OF_TWO------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_non_power_of_two
#define GL_ARB_texture_non_power_of_two 1

#endif /* GL_ARB_texture_non_power_of_two */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_POINT_SPRITE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_point_sprite
#define GL_ARB_point_sprite 1

#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862

#endif /* GL_ARB_point_sprite */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_FRAGMENT_PROGRAM_SHADOW-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_fragment_program_shadow
#define GL_ARB_fragment_program_shadow 1

#endif /* GL_ARB_fragment_program_shadow */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_DRAW_BUFFERS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_draw_buffers
#define GL_ATI_draw_buffers 1
        
#define GL_MAX_DRAW_BUFFERS_ATI                                 0x8824
#define GL_DRAW_BUFFER0_ATI                                     0x8825
#define GL_DRAW_BUFFER1_ATI                                     0x8826
#define GL_DRAW_BUFFER2_ATI                                     0x8827
#define GL_DRAW_BUFFER3_ATI                                     0x8828
#define GL_DRAW_BUFFER4_ATI                                     0x8829
#define GL_DRAW_BUFFER5_ATI                                     0x882A
#define GL_DRAW_BUFFER6_ATI                                     0x882B
#define GL_DRAW_BUFFER7_ATI                                     0x882C
#define GL_DRAW_BUFFER8_ATI                                     0x882D
#define GL_DRAW_BUFFER9_ATI                                     0x882E
#define GL_DRAW_BUFFER10_ATI                                    0x882F
#define GL_DRAW_BUFFER11_ATI                                    0x8830
#define GL_DRAW_BUFFER12_ATI                                    0x8831
#define GL_DRAW_BUFFER13_ATI                                    0x8832
#define GL_DRAW_BUFFER14_ATI                                    0x8833
#define GL_DRAW_BUFFER15_ATI                                    0x8834

typedef void (APIENTRY * glDrawBuffersATIPROC) (GLsizei n, const GLenum *bufs);

extern glDrawBuffersATIPROC glDrawBuffersATI;

#endif /* GL_ATI_draw_buffers */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_ELEMENT_ARRAY-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_element_array
#define GL_ATI_element_array 1

#define GL_ELEMENT_ARRAY_ATI                                    0x8768
#define GL_ELEMENT_ARRAY_TYPE_ATI                               0x8769
#define GL_ELEMENT_ARRAY_POINTER_ATI                            0x876A

typedef void (APIENTRY * glElementPointerATIPROC) (GLenum type, const GLvoid *pointer);
typedef void (APIENTRY * glDrawElementArrayATIPROC) (GLenum mode, GLsizei count);
typedef void (APIENTRY * glDrawRangeElementArrayATIPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count);

extern glElementPointerATIPROC glElementPointerATI;
extern glDrawElementArrayATIPROC glDrawElementArrayATI;
extern glDrawRangeElementArrayATIPROC glDrawRangeElementArrayATI;

#endif /* GL_ATI_element_array */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_ENVMAP_BUMPMAP----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_envmap_bumpmap
#define GL_ATI_envmap_bumpmap 1

#define GL_BUMP_ROT_MATRIX_ATI                                  0x8775
#define GL_BUMP_ROT_MATRIX_SIZE_ATI                             0x8776
#define GL_BUMP_NUM_TEX_UNITS_ATI                               0x8777
#define GL_BUMP_TEX_UNITS_ATI                                   0x8778
#define GL_DUDV_ATI                                             0x8779
#define GL_DU8DV8_ATI                                           0x877A
#define GL_BUMP_ENVMAP_ATI                                      0x877B
#define GL_BUMP_TARGET_ATI                                      0x877C

typedef void (APIENTRY * glTexBumpParameterivATIPROC) (GLenum pname, GLint *param);
typedef void (APIENTRY * glTexBumpParameterfvATIPROC) (GLenum pname, GLfloat *param);
typedef void (APIENTRY * glGetTexBumpParameterivATIPROC) (GLenum pname, GLint *param);
typedef void (APIENTRY * glGetTexBumpParameterfvATIPROC) (GLenum pname, GLfloat *param);

extern glTexBumpParameterivATIPROC glTexBumpParameterivATI;
extern glTexBumpParameterfvATIPROC glTexBumpParameterfvATI;
extern glGetTexBumpParameterivATIPROC glGetTexBumpParameterivATI;
extern glGetTexBumpParameterfvATIPROC glGetTexBumpParameterfvATI;

#endif /* GL_ATI_envmap_bumpmap */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_FRAGMENT_SHADER---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_fragment_shader
#define GL_ATI_fragment_shader 1

#define GL_FRAGMENT_SHADER_ATI                                  0x8920
#define GL_REG_0_ATI                                            0x8921
#define GL_REG_1_ATI                                            0x8922
#define GL_REG_2_ATI                                            0x8923
#define GL_REG_3_ATI                                            0x8924
#define GL_REG_4_ATI                                            0x8925
#define GL_REG_5_ATI                                            0x8926
#define GL_REG_6_ATI                                            0x8927
#define GL_REG_7_ATI                                            0x8928
#define GL_REG_8_ATI                                            0x8929
#define GL_REG_9_ATI                                            0x892A
#define GL_REG_10_ATI                                           0x892B
#define GL_REG_11_ATI                                           0x892C
#define GL_REG_12_ATI                                           0x892D
#define GL_REG_13_ATI                                           0x892E
#define GL_REG_14_ATI                                           0x892F
#define GL_REG_15_ATI                                           0x8930
#define GL_REG_16_ATI                                           0x8931
#define GL_REG_17_ATI                                           0x8932
#define GL_REG_18_ATI                                           0x8933
#define GL_REG_19_ATI                                           0x8934
#define GL_REG_20_ATI                                           0x8935
#define GL_REG_21_ATI                                           0x8936
#define GL_REG_22_ATI                                           0x8937
#define GL_REG_23_ATI                                           0x8938
#define GL_REG_24_ATI                                           0x8939
#define GL_REG_25_ATI                                           0x893A
#define GL_REG_26_ATI                                           0x893B
#define GL_REG_27_ATI                                           0x893C
#define GL_REG_28_ATI                                           0x893D
#define GL_REG_29_ATI                                           0x893E
#define GL_REG_30_ATI                                           0x893F
#define GL_REG_31_ATI                                           0x8940
#define GL_CON_0_ATI                                            0x8941
#define GL_CON_1_ATI                                            0x8942
#define GL_CON_2_ATI                                            0x8943
#define GL_CON_3_ATI                                            0x8944
#define GL_CON_4_ATI                                            0x8945
#define GL_CON_5_ATI                                            0x8946
#define GL_CON_6_ATI                                            0x8947
#define GL_CON_7_ATI                                            0x8948
#define GL_CON_8_ATI                                            0x8949
#define GL_CON_9_ATI                                            0x894A
#define GL_CON_10_ATI                                           0x894B
#define GL_CON_11_ATI                                           0x894C
#define GL_CON_12_ATI                                           0x894D
#define GL_CON_13_ATI                                           0x894E
#define GL_CON_14_ATI                                           0x894F
#define GL_CON_15_ATI                                           0x8950
#define GL_CON_16_ATI                                           0x8951
#define GL_CON_17_ATI                                           0x8952
#define GL_CON_18_ATI                                           0x8953
#define GL_CON_19_ATI                                           0x8954
#define GL_CON_20_ATI                                           0x8955
#define GL_CON_21_ATI                                           0x8956
#define GL_CON_22_ATI                                           0x8957
#define GL_CON_23_ATI                                           0x8958
#define GL_CON_24_ATI                                           0x8959
#define GL_CON_25_ATI                                           0x895A
#define GL_CON_26_ATI                                           0x895B
#define GL_CON_27_ATI                                           0x895C
#define GL_CON_28_ATI                                           0x895D
#define GL_CON_29_ATI                                           0x895E
#define GL_CON_30_ATI                                           0x895F
#define GL_CON_31_ATI                                           0x8960
#define GL_MOV_ATI                                              0x8961
#define GL_ADD_ATI                                              0x8963
#define GL_MUL_ATI                                              0x8964
#define GL_SUB_ATI                                              0x8965
#define GL_DOT3_ATI                                             0x8966
#define GL_DOT4_ATI                                             0x8967
#define GL_MAD_ATI                                              0x8968
#define GL_LERP_ATI                                             0x8969
#define GL_CND_ATI                                              0x896A
#define GL_CND0_ATI                                             0x896B
#define GL_DOT2_ADD_ATI                                         0x896C
#define GL_SECONDARY_INTERPOLATOR_ATI                           0x896D
#define GL_NUM_FRAGMENT_REGISTERS_ATI                           0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_ATI                           0x896F
#define GL_NUM_PASSES_ATI                                       0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI                        0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI                           0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI                0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_ATI                          0x8974
#define GL_COLOR_ALPHA_PAIRING_ATI                              0x8975
#define GL_SWIZZLE_STR_ATI                                      0x8976
#define GL_SWIZZLE_STQ_ATI                                      0x8977
#define GL_SWIZZLE_STR_DR_ATI                                   0x8978
#define GL_SWIZZLE_STQ_DQ_ATI                                   0x8979
#define GL_SWIZZLE_STRQ_ATI                                     0x897A
#define GL_SWIZZLE_STRQ_DQ_ATI                                  0x897B
#define GL_RED_BIT_ATI                                          0x00000001
#define GL_GREEN_BIT_ATI                                        0x00000002
#define GL_BLUE_BIT_ATI                                         0x00000004
#define GL_2X_BIT_ATI                                           0x00000001
#define GL_4X_BIT_ATI                                           0x00000002
#define GL_8X_BIT_ATI                                           0x00000004
#define GL_HALF_BIT_ATI                                         0x00000008
#define GL_QUARTER_BIT_ATI                                      0x00000010
#define GL_EIGHTH_BIT_ATI                                       0x00000020
#define GL_SATURATE_BIT_ATI                                     0x00000040
#define GL_COMP_BIT_ATI                                         0x00000002
#define GL_NEGATE_BIT_ATI                                       0x00000004
#define GL_BIAS_BIT_ATI                                         0x00000008

typedef GLuint (APIENTRY * glGenFragmentShadersATIPROC) (GLuint range);
typedef void (APIENTRY * glBindFragmentShaderATIPROC) (GLuint id);
typedef void (APIENTRY * glDeleteFragmentShaderATIPROC) (GLuint id);
typedef void (APIENTRY * glBeginFragmentShaderATIPROC) (GLvoid);
typedef void (APIENTRY * glEndFragmentShaderATIPROC) (GLvoid);
typedef void (APIENTRY * glPassTexCoordATIPROC) (GLuint dst, GLuint coord, GLenum swizzle);
typedef void (APIENTRY * glSampleMapATIPROC) (GLuint dst, GLuint interp, GLenum swizzle);
typedef void (APIENTRY * glColorFragmentOp1ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (APIENTRY * glColorFragmentOp2ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (APIENTRY * glColorFragmentOp3ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (APIENTRY * glAlphaFragmentOp1ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (APIENTRY * glAlphaFragmentOp2ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (APIENTRY * glAlphaFragmentOp3ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (APIENTRY * glSetFragmentShaderConstantATIPROC) (GLuint dst, const GLfloat *value);

extern glGenFragmentShadersATIPROC glGenFragmentShadersATI;
extern glBindFragmentShaderATIPROC glBindFragmentShaderATI;
extern glDeleteFragmentShaderATIPROC glDeleteFragmentShaderATI;
extern glBeginFragmentShaderATIPROC glBeginFragmentShaderATI;
extern glEndFragmentShaderATIPROC glEndFragmentShaderATI;
extern glPassTexCoordATIPROC glPassTexCoordATI;
extern glSampleMapATIPROC glSampleMapATI;
extern glColorFragmentOp1ATIPROC glColorFragmentOp1ATI;
extern glColorFragmentOp2ATIPROC glColorFragmentOp2ATI;
extern glColorFragmentOp3ATIPROC glColorFragmentOp3ATI;
extern glAlphaFragmentOp1ATIPROC glAlphaFragmentOp1ATI;
extern glAlphaFragmentOp2ATIPROC glAlphaFragmentOp2ATI;
extern glAlphaFragmentOp3ATIPROC glAlphaFragmentOp3ATI;
extern glSetFragmentShaderConstantATIPROC glSetFragmentShaderConstantATI;

#endif /* GL_ATI_fragment_shader */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_MAP_OBJECT_BUFFER-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_map_object_buffer
#define GL_ATI_map_object_buffer 1

typedef void (APIENTRY * glMapObjectBufferATIPROC) (GLuint buffer);
typedef void (APIENTRY * glUnmapObjectBufferATIPROC) (GLuint buffer);

extern glMapObjectBufferATIPROC glMapObjectBufferATI;
extern glUnmapObjectBufferATIPROC glUnmapObjectBufferATI;

#endif /* GL_ATI_map_object_buffer */ 

/*-------------------------------------------------------------------*/
/*------------GL_ATI_PN_TRIANGLES------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_pn_triangles
#define GL_ATI_pn_triangles 1

#define GL_PN_TRIANGLES_ATI                                     0x87F0
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI               0x87F1
#define GL_PN_TRIANGLES_POINT_MODE_ATI                          0x87F2
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI                         0x87F3
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI                   0x87F4
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI                   0x87F5
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI                    0x87F6
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI                  0x87F7
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI               0x87F8

typedef void (APIENTRY * glPNTrianglesiATIPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glPNTrianglesfATIPROC) (GLenum pname, GLfloat param);

extern glPNTrianglesiATIPROC glPNTrianglesiATI;
extern glPNTrianglesfATIPROC glPNTrianglesfATI;

#endif /* GL_ATI_pn_triangles */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_POINT_CULL_MODE---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_point_cull_mode
#define GL_ATI_point_cull_mode 1

#define GL_POINT_CULL_MODE_ATI                                  0x60b3
#define GL_POINT_CULL_CENTER_ATI                                0x60b4
#define GL_POINT_CLIP_ATI                                       0x60b5

#endif /* GL_ATI_point_cull_mode */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_SEPARATE_STENCIL--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_separate_stencil
#define GL_ATI_separate_stencil 1

#define GL_STENCIL_BACK_FUNC_ATI                                0x8800
#define GL_STENCIL_BACK_FAIL_ATI                                0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI                     0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI                     0x8803

typedef void (APIENTRY * glStencilOpSeparateATIPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRY * glStencilFuncSeparateATIPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);

extern glStencilOpSeparateATIPROC glStencilOpSeparateATI;
extern glStencilFuncSeparateATIPROC glStencilFuncSeparateATI;

#endif /* GL_ATI_separate_stencil */


/*-------------------------------------------------------------------*/
/*------------GL_ATI_VERTEX_ATTRIB_ARRAY_OBJECT----------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_vertex_attrib_array_object
#define GL_ATI_vertex_attrib_array_object 1

typedef void (APIENTRY * glVertexAttribArrayObjectATIPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);
typedef void (APIENTRY * glGetVertexAttribArrayObjectfvATIPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVertexAttribArrayObjectivATIPROC) (GLuint index, GLenum pname, GLint *params);

extern glVertexAttribArrayObjectATIPROC glVertexAttribArrayObjectATI;
extern glGetVertexAttribArrayObjectfvATIPROC glGetVertexAttribArrayObjectfvATI;
extern glGetVertexAttribArrayObjectivATIPROC glGetVertexAttribArrayObjectivATI;

#endif /* GL_ATI_vertex_attrib_array_object */

/*-------------------------------------------------------------------*/
/*------------GL_OES_READ_FORMAT-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_OES_read_format
#define GL_OES_read_format 1

#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES	0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES	0x8B9B

#endif /* GL_OES_read_format */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_TEXT_FRAGMENT_SHADER----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_text_fragment_shader
#define GL_ATI_text_fragment_shader 1

#define GL_TEXT_FRAGMENT_SHADER_ATI                             0x8200

#endif /* GL_ATI_text_fragment_shader */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_TEXTURE_ENV_COMBINE3----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_texture_env_combine3
#define GL_ATI_texture_env_combine3 1

#define GL_MODULATE_ADD_ATI                                     0x8744
#define GL_MODULATE_SIGNED_ADD_ATI                              0x8745
#define GL_MODULATE_SUBTRACT_ATI                                0x8746

#endif /* GL_ATI_texture_env_combine3 */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_TEXTURE_COMPRESSION_3DC-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_texture_compression_3dc
#define GL_ATI_texture_compression_3dc 1

#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI					0x8837

#endif /* GL_ATI_texture_compression_3dc */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_TEXTURE_FLOAT-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_texture_float
#define GL_ATI_texture_float 1

#define GL_RGBA_FLOAT32_ATI                                     0x8814
#define GL_RGB_FLOAT32_ATI                                      0x8815
#define GL_ALPHA_FLOAT32_ATI                                    0x8816
#define GL_INTENSITY_FLOAT32_ATI                                0x8817
#define GL_LUMINANCE_FLOAT32_ATI                                0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI                          0x8819
#define GL_RGBA_FLOAT16_ATI                                     0x881A
#define GL_RGB_FLOAT16_ATI                                      0x881B
#define GL_ALPHA_FLOAT16_ATI                                    0x881C
#define GL_INTENSITY_FLOAT16_ATI                                0x881D
#define GL_LUMINANCE_FLOAT16_ATI                                0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI                          0x881F

#endif /* GL_ATI_texture_float */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_TEXTURE_MIRROR_ONCE-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_texture_mirror_once
#define GL_ATI_texture_mirror_once 1

#define GL_MIRROR_CLAMP_ATI                                     0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI                             0x8743

#endif

/*-------------------------------------------------------------------*/
/*------------GL_ATI_VERTEX_ARRAY_OBJECT-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_vertex_array_object
#define GL_ATI_vertex_array_object 1

#define GL_STATIC_ATI                                           0x8760
#define GL_DYNAMIC_ATI                                          0x8761
#define GL_PRESERVE_ATI                                         0x8762
#define GL_DISCARD_ATI                                          0x8763
#define GL_OBJECT_BUFFER_SIZE_ATI                               0x8764
#define GL_OBJECT_BUFFER_USAGE_ATI                              0x8765
#define GL_ARRAY_OBJECT_BUFFER_ATI                              0x8766
#define GL_ARRAY_OBJECT_OFFSET_ATI                              0x8767

typedef GLuint (APIENTRY * glNewObjectBufferATIPROC) (GLsizei size, const GLvoid *pointer, GLenum usage);
typedef GLboolean (APIENTRY * glIsObjectBufferATIPROC) (GLuint buffer);
typedef void (APIENTRY * glUpdateObjectBufferATIPROC) (GLuint buffer, GLuint offset, GLsizei size, const GLvoid *pointer, GLenum preserve);
typedef void (APIENTRY * glGetObjectBufferfvATIPROC) (GLuint buffer, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetObjectBufferivATIPROC) (GLuint buffer, GLenum pname, GLint *params);
typedef void (APIENTRY * glFreeObjectBufferATIPROC) (GLuint buffer);
typedef void (APIENTRY * glArrayObjectATIPROC) (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
typedef void (APIENTRY * glGetArrayObjectfvATIPROC) (GLenum array, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetArrayObjectivATIPROC) (GLenum array, GLenum pname, GLint *params);
typedef void (APIENTRY * glVariantArrayObjectATIPROC) (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
typedef void (APIENTRY * glGetVariantArrayObjectfvATIPROC) (GLuint id, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVariantArrayObjectivATIPROC) (GLuint id, GLenum pname, GLint *params);

extern glNewObjectBufferATIPROC glNewObjectBufferATI;
extern glIsObjectBufferATIPROC glIsObjectBufferATI;
extern glUpdateObjectBufferATIPROC glUpdateObjectBufferATI;
extern glGetObjectBufferfvATIPROC glGetObjectBufferfvATI;
extern glGetObjectBufferivATIPROC glGetObjectBufferivATI;
extern glFreeObjectBufferATIPROC glFreeObjectBufferATI;
extern glArrayObjectATIPROC glArrayObjectATI;
extern glGetArrayObjectfvATIPROC glGetArrayObjectfvATI;
extern glGetArrayObjectivATIPROC glGetArrayObjectivATI;
extern glVariantArrayObjectATIPROC glVariantArrayObjectATI;
extern glGetVariantArrayObjectfvATIPROC glGetVariantArrayObjectfvATI;
extern glGetVariantArrayObjectivATIPROC glGetVariantArrayObjectivATI;

#endif /* GL_ATI_vertex_array_object */

/*-------------------------------------------------------------------*/
/*------------GL_ATI_VERTEX_STREAMS----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATI_vertex_streams 
#define GL_ATI_vertex_streams 1

#define GL_MAX_VERTEX_STREAMS_ATI                               0x876B  
#define GL_VERTEX_SOURCE_ATI                                    0x876C
#define GL_VERTEX_STREAM0_ATI                                   0x876D
#define GL_VERTEX_STREAM1_ATI                                   0x876E
#define GL_VERTEX_STREAM2_ATI                                   0x876F
#define GL_VERTEX_STREAM3_ATI                                   0x8770
#define GL_VERTEX_STREAM4_ATI                                   0x8771
#define GL_VERTEX_STREAM5_ATI                                   0x8772
#define GL_VERTEX_STREAM6_ATI                                   0x8773
#define GL_VERTEX_STREAM7_ATI                                   0x8774

typedef void (APIENTRY * glClientActiveVertexStreamATIPROC) (GLenum stream);
typedef void (APIENTRY * glVertexBlendEnviATIPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glVertexBlendEnvfATIPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glVertexStream1sATIPROC) (GLenum stream, GLshort x);
typedef void (APIENTRY * glVertexStream1svATIPROC) (GLenum stream, const GLshort *coords);
typedef void (APIENTRY * glVertexStream1iATIPROC) (GLenum stream, GLint x);
typedef void (APIENTRY * glVertexStream1ivATIPROC) (GLenum stream, const GLint *coords);
typedef void (APIENTRY * glVertexStream1fATIPROC) (GLenum stream, GLfloat x);
typedef void (APIENTRY * glVertexStream1fvATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (APIENTRY * glVertexStream1dATIPROC) (GLenum stream, GLdouble x);
typedef void (APIENTRY * glVertexStream1dvATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (APIENTRY * glVertexStream2sATIPROC) (GLenum stream, GLshort x, GLshort y);
typedef void (APIENTRY * glVertexStream2svATIPROC) (GLenum stream, const GLshort *v);
typedef void (APIENTRY * glVertexStream2iATIPROC) (GLenum stream, GLint x, GLint y);
typedef void (APIENTRY * glVertexStream2ivATIPROC) (GLenum stream, const GLint *v);
typedef void (APIENTRY * glVertexStream2fATIPROC) (GLenum stream, GLfloat x, GLfloat y);
typedef void (APIENTRY * glVertexStream2fvATIPROC) (GLenum stream, const GLfloat *v);
typedef void (APIENTRY * glVertexStream2dATIPROC) (GLenum stream, GLdouble x, GLdouble y);
typedef void (APIENTRY * glVertexStream2dvATIPROC) (GLenum stream, const GLdouble *v);
typedef void (APIENTRY * glVertexStream3sATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glVertexStream3svATIPROC) (GLenum stream, const GLshort *v);
typedef void (APIENTRY * glVertexStream3iATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (APIENTRY * glVertexStream3ivATIPROC) (GLenum stream, const GLint *v);
typedef void (APIENTRY * glVertexStream3fATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glVertexStream3fvATIPROC) (GLenum stream, const GLfloat *v);
typedef void (APIENTRY * glVertexStream3dATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glVertexStream3dvATIPROC) (GLenum stream, const GLdouble *v);
typedef void (APIENTRY * glVertexStream4sATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glVertexStream4svATIPROC) (GLenum stream, const GLshort *v);
typedef void (APIENTRY * glVertexStream4iATIPROC) (GLenum stream, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRY * glVertexStream4ivATIPROC) (GLenum stream, const GLint *v);
typedef void (APIENTRY * glVertexStream4fATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glVertexStream4fvATIPROC) (GLenum stream, const GLfloat *v);
typedef void (APIENTRY * glVertexStream4dATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glVertexStream4dvATIPROC) (GLenum stream, const GLdouble *v);
typedef void (APIENTRY * glNormalStream3bATIPROC) (GLenum stream, GLbyte x, GLbyte y, GLbyte z);
typedef void (APIENTRY * glNormalStream3bvATIPROC) (GLenum stream, const GLbyte *v);
typedef void (APIENTRY * glNormalStream3sATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glNormalStream3svATIPROC) (GLenum stream, const GLshort *v);
typedef void (APIENTRY * glNormalStream3iATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (APIENTRY * glNormalStream3ivATIPROC) (GLenum stream, const GLint *v);
typedef void (APIENTRY * glNormalStream3fATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glNormalStream3fvATIPROC) (GLenum stream, const GLfloat *v);
typedef void (APIENTRY * glNormalStream3dATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glNormalStream3dvATIPROC) (GLenum stream, const GLdouble *v);


extern glClientActiveVertexStreamATIPROC glClientActiveVertexStreamATI;
extern glVertexBlendEnviATIPROC glVertexBlendEnviATI;
extern glVertexBlendEnvfATIPROC glVertexBlendEnvfATI;
extern glVertexStream1sATIPROC glVertexStream1sATI;
extern glVertexStream1svATIPROC glVertexStream1svATI;
extern glVertexStream1iATIPROC glVertexStream1iATI;
extern glVertexStream1ivATIPROC glVertexStream1ivATI;
extern glVertexStream1fATIPROC glVertexStream1fATI;
extern glVertexStream1fvATIPROC glVertexStream1fvATI;
extern glVertexStream1dATIPROC glVertexStream1dATI;
extern glVertexStream1dvATIPROC glVertexStream1dvATI;
extern glVertexStream2sATIPROC glVertexStream2sATI;
extern glVertexStream2svATIPROC glVertexStream2svATI;
extern glVertexStream2iATIPROC glVertexStream2iATI;
extern glVertexStream2ivATIPROC glVertexStream2ivATI;
extern glVertexStream2fATIPROC glVertexStream2fATI;
extern glVertexStream2fvATIPROC glVertexStream2fvATI;
extern glVertexStream2dATIPROC glVertexStream2dATI;
extern glVertexStream2dvATIPROC glVertexStream2dvATI;
extern glVertexStream3sATIPROC glVertexStream3sATI;
extern glVertexStream3svATIPROC glVertexStream3svATI;
extern glVertexStream3iATIPROC glVertexStream3iATI;
extern glVertexStream3ivATIPROC glVertexStream3ivATI;
extern glVertexStream3fATIPROC glVertexStream3fATI;
extern glVertexStream3fvATIPROC glVertexStream3fvATI;
extern glVertexStream3dATIPROC glVertexStream3dATI;
extern glVertexStream3dvATIPROC glVertexStream3dvATI;
extern glVertexStream4sATIPROC glVertexStream4sATI;
extern glVertexStream4svATIPROC glVertexStream4svATI;
extern glVertexStream4iATIPROC glVertexStream4iATI;
extern glVertexStream4ivATIPROC glVertexStream4ivATI;
extern glVertexStream4fATIPROC glVertexStream4fATI;
extern glVertexStream4fvATIPROC glVertexStream4fvATI;
extern glVertexStream4dATIPROC glVertexStream4dATI;
extern glVertexStream4dvATIPROC glVertexStream4dvATI;
extern glNormalStream3bATIPROC glNormalStream3bATI;
extern glNormalStream3bvATIPROC glNormalStream3bvATI;
extern glNormalStream3sATIPROC glNormalStream3sATI;
extern glNormalStream3svATIPROC glNormalStream3svATI;
extern glNormalStream3iATIPROC glNormalStream3iATI;
extern glNormalStream3ivATIPROC glNormalStream3ivATI;
extern glNormalStream3fATIPROC glNormalStream3fATI;
extern glNormalStream3fvATIPROC glNormalStream3fvATI;
extern glNormalStream3dATIPROC glNormalStream3dATI;
extern glNormalStream3dvATIPROC glNormalStream3dvATI;

#endif /* GL_ATI_vertex_streams */

/*-------------------------------------------------------------------*/
/*------------GL_ATIX_POINT_SPRITES----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATIX_point_sprites
#define GL_ATIX_point_sprites 1

#define GL_TEXTURE_POINT_MODE_ATIX                              0x60b0
#define	GL_TEXTURE_POINT_ONE_COORD_ATIX                         0x60b1
#define	GL_TEXTURE_POINT_SPRITE_ATIX                            0x60b2
#define GL_POINT_SPRITE_CULL_MODE_ATIX                          0x60b3
#define GL_POINT_SPRITE_CULL_CENTER_ATIX                        0x60b4
#define GL_POINT_SPRITE_CULL_CLIP_ATIX                          0x60b5

#endif /* GL_ATIX_point_sprites */

/*-------------------------------------------------------------------*/
/*------------GL_ATIX_TEXTURE_ENV_COMBINE3---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATIX_texture_env_combine3
#define GL_ATIX_texture_env_combine3 1

#define GL_MODULATE_ADD_ATIX									0x8744
#define GL_MODULATE_SIGNED_ADD_ATIX								0x8745
#define GL_MODULATE_SUBTRACT_ATIX								0x8746

#endif /* GL_ATIX_texture_env_combine3 */

/*-------------------------------------------------------------------*/
/*------------GL_ATIX_TEXTURE_ENV_ROUTE------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATIX_texture_env_route
#define GL_ATIX_texture_env_route 1

#define GL_SECONDARY_COLOR_ATIX                                 0x8747
#define GL_TEXTURE_OUTPUT_RGB_ATIX                              0x8748
#define GL_TEXTURE_OUTPUT_ALPHA_ATIX                            0x8749

#endif /* GL_ATIX_texture_env_route */

/*-------------------------------------------------------------------*/
/*------------GL_ATIX_VERTEX_SHADER_OUTPUT_POINT_SIZE----------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ATIX_vertex_shader_output_point_size
#define GL_ATIX_vertex_shader_output_point_size 1

#define GL_OUTPUT_POINT_SIZE_ATIX								0x610E

#endif /* GL_ATIX_vertex_shader_output_point_size */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_422_PIXELS--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_422_pixels
#define GL_EXT_422_pixels 1

#define GL_422_EXT												0x80CC
#define GL_422_REV_EXT											0x80CD
#define GL_422_AVERAGE_EXT										0x80CE
#define GL_422_REV_AVERAGE_EXT									0x80CF

#endif /* GL_EXT_422_pixels */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BGRA--------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1

#define GL_BGR_EXT                                              0x80E0
#define GL_BGRA_EXT                                             0x80E1

#endif /* GL_EXT_bgra */


/*-------------------------------------------------------------------*/
/*------------GL_ARB_DRAW_BUFFERS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ARB           0x8824
#define GL_DRAW_BUFFER0_ARB               0x8825
#define GL_DRAW_BUFFER1_ARB               0x8826
#define GL_DRAW_BUFFER2_ARB               0x8827
#define GL_DRAW_BUFFER3_ARB               0x8828
#define GL_DRAW_BUFFER4_ARB               0x8829
#define GL_DRAW_BUFFER5_ARB               0x882A
#define GL_DRAW_BUFFER6_ARB               0x882B
#define GL_DRAW_BUFFER7_ARB               0x882C
#define GL_DRAW_BUFFER8_ARB               0x882D
#define GL_DRAW_BUFFER9_ARB               0x882E
#define GL_DRAW_BUFFER10_ARB              0x882F
#define GL_DRAW_BUFFER11_ARB              0x8830
#define GL_DRAW_BUFFER12_ARB              0x8831
#define GL_DRAW_BUFFER13_ARB              0x8832
#define GL_DRAW_BUFFER14_ARB              0x8833
#define GL_DRAW_BUFFER15_ARB              0x8834

typedef void (APIENTRY * glDrawBuffersARBPROC) (GLsizei n, const GLenum *bufs);

extern glDrawBuffersARBPROC glDrawBuffersARB;

#endif /* GL_ARB_draw_buffers */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_RECTANGLE-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_rectangle
#define GL_ARB_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_ARB			0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB	0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB		0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB	0x84F8

#endif /* GL_ARB_texture_rectangle */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_COLOR_BUFFER_FLOAT------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_color_buffer_float
#define GL_ARB_color_buffer_float 1

typedef void (APIENTRY * glClampColorARBPROC) (GLenum target, GLenum clamp);

extern glClampColorARBPROC glClampColorARB;

#endif /* GL_ARB_color_buffer_float */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_HALF_FLOAT_PIXEL--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel 1

#define GL_HALF_FLOAT_ARB                 0x140B

#endif /* GL_ARB_half_float_pixel */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_TEXTURE_FLOAT-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_texture_float
#define GL_ARB_texture_float 1

#define GL_TEXTURE_RED_TYPE_ARB           0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB         0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB          0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB         0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB     0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB     0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB         0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB        0x8C17
#define GL_RGBA32F_ARB                    0x8814
#define GL_RGB32F_ARB                     0x8815
#define GL_ALPHA32F_ARB                   0x8816
#define GL_INTENSITY32F_ARB               0x8817
#define GL_LUMINANCE32F_ARB               0x8818
#define GL_LUMINANCE_ALPHA32F_ARB         0x8819
#define GL_RGBA16F_ARB                    0x881A
#define GL_RGB16F_ARB                     0x881B
#define GL_ALPHA16F_ARB                   0x881C
#define GL_INTENSITY16F_ARB               0x881D
#define GL_LUMINANCE16F_ARB               0x881E
#define GL_LUMINANCE_ALPHA16F_ARB         0x881F

#endif /* GL_ARB_texture_float */

/*-------------------------------------------------------------------*/
/*------------GL_ARB_PIXEL_BUFFER_OBJECT-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_ARB			0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB			0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB	0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB	0x88EF

#endif /* GL_ARB_pixel_buffer_object */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_ABGR--------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1

#define GL_ABGR_EXT                                             0x8000

#endif /* GL_EXT_abgr */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_COLOR-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1

#define GL_CONSTANT_COLOR_EXT                                   0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT                         0x8002
#define GL_CONSTANT_ALPHA_EXT                                   0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT                         0x8004
#define GL_BLEND_COLOR_EXT                                      0x8005

typedef void (APIENTRY * glBlendColorEXTPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

extern glBlendColorEXTPROC glBlendColorEXT;

#endif /* GL_EXT_blend_color */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_FUNC_SEPARATE---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate 1

#define GL_BLEND_DST_RGB_EXT                                    0x80C8
#define GL_BLEND_SRC_RGB_EXT                                    0x80C9
#define GL_BLEND_DST_ALPHA_EXT                                  0x80CA
#define GL_BLEND_SRC_ALPHA_EXT                                  0x80CB

typedef void (APIENTRY * glBlendFuncSeparateEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

extern glBlendFuncSeparateEXTPROC glBlendFuncSeparateEXT;

#endif /* GL_EXT_blend_func_separate */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_LOGIC_OP----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op 1

#endif /* GL_EXT_blend_logic_op */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_MINMAX------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1

#define GL_FUNC_ADD_EXT                                         0x8006
#define GL_MIN_EXT                                              0x8007
#define GL_MAX_EXT                                              0x8008
#define GL_BLEND_EQUATION_EXT                                   0x8009

typedef void (APIENTRY * glBlendEquationEXTPROC) (GLenum mode);

extern glBlendEquationEXTPROC glBlendEquationEXT;

#endif /* GL_EXT_blend_minmax */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_SUBTRACT----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract 1

#define GL_FUNC_SUBTRACT_EXT                                    0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT                            0x800B

#endif /* GL_EXT_blend_subtract */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_CLIP_VOLUME_HINT--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_clip_volume_hint
#define GL_EXT_clip_volume_hint 1

#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT						0x80F0

#endif /* GL_EXT_clip_volume_hint */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_CMYKA-------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka 1

#define GL_CMYK_EXT												0x800C
#define GL_CMYKA_EXT											0x800D
#define GL_PACK_CMYK_HINT_EXT									0x800E
#define GL_UNPACK_CMYK_HINT_EXT									0x800F

#endif /* GL_EXT_cmyka */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_COLOR_MATRIX------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_color_matrix
#define GL_EXT_color_matrix 1

#endif /* GL_EXT_color_matrix */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_COLOR_SUBTABLE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_color_subtable
#define GL_EXT_color_subtable 1

typedef void (APIENTRY * glColorSubTableEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
typedef void (APIENTRY * glCopyColorSubTableEXTPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

extern glColorSubTableEXTPROC glColorSubTableEXT;
extern glCopyColorSubTableEXTPROC glCopyColorSubTableEXT;

#endif /* GL_EXT_color_subtable */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_COMPILED_VERTEX_ARRAY---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_compiled_vertex_array
#define GL_EXT_compiled_vertex_array 1

#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT                         0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT                         0x81A9

typedef void (APIENTRY * glLockArraysEXTPROC) (GLint first, GLsizei count);
typedef void (APIENTRY * glUnlockArraysEXTPROC) ();

extern glLockArraysEXTPROC glLockArraysEXT;
extern glUnlockArraysEXTPROC glUnlockArraysEXT;

#endif /* GL_EXT_compiled_vertex_array */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_CONVOLUTION-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_convolution
#define GL_EXT_convolution 1

#define GL_CONVOLUTION_1D_EXT									0x8010
#define GL_CONVOLUTION_2D_EXT									0x8011
#define GL_SEPARABLE_2D_EXT										0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT							0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT							0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT							0x8015
#define GL_REDUCE_EXT											0x8016
#define GL_CONVOLUTION_FORMAT_EXT								0x8017
#define GL_CONVOLUTION_WIDTH_EXT								0x8018
#define GL_CONVOLUTION_HEIGHT_EXT								0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT							0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT							0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT						0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT						0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT						0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT						0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT						0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT						0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT						0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT						0x8023

typedef void (APIENTRY * glConvolutionFilter1DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
typedef void (APIENTRY * glConvolutionFilter2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
typedef void (APIENTRY * glConvolutionParameterfEXTPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void (APIENTRY * glConvolutionParameterfvEXTPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glConvolutionParameteriEXTPROC) (GLenum target, GLenum pname, GLint params);
typedef void (APIENTRY * glConvolutionParameterivEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glCopyConvolutionFilter1DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (APIENTRY * glCopyConvolutionFilter2DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * glGetConvolutionFilterEXTPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image);
typedef void (APIENTRY * glGetConvolutionParameterfvEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetConvolutionParameterivEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetSeparableFilterEXTPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
typedef void (APIENTRY * glSeparableFilter2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

extern glConvolutionFilter1DEXTPROC glConvolutionFilter1DEXT;
extern glConvolutionFilter2DEXTPROC glConvolutionFilter2DEXT;
extern glConvolutionParameterfEXTPROC glConvolutionParameterfEXT;
extern glConvolutionParameterfvEXTPROC glConvolutionParameterfvEXT;
extern glConvolutionParameteriEXTPROC glConvolutionParameteriEXT;
extern glConvolutionParameterivEXTPROC glConvolutionParameterivEXT;
extern glCopyConvolutionFilter1DEXTPROC glCopyConvolutionFilter1DEXT;
extern glCopyConvolutionFilter2DEXTPROC glCopyConvolutionFilter2DEXT;
extern glGetConvolutionFilterEXTPROC glGetConvolutionFilterEXT;
extern glGetConvolutionParameterfvEXTPROC glGetConvolutionParameterfvEXT;
extern glGetConvolutionParameterivEXTPROC glGetConvolutionParameterivEXT;
extern glGetSeparableFilterEXTPROC glGetSeparableFilterEXT;
extern glSeparableFilter2DEXTPROC glSeparableFilter2DEXT;

#endif /* GL_EXT_convolution */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_COORDINATE_FRAME--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_coordinate_frame
#define GL_EXT_coordinate_frame 1

#define GL_TANGENT_ARRAY_EXT									0x8439
#define GL_BINORMAL_ARRAY_EXT									0x843A
#define GL_CURRENT_TANGENT_EXT									0x843B
#define GL_CURRENT_BINORMAL_EXT									0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT								0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT								0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT								0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT							0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT							0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT							0x8443
#define GL_MAP1_TANGENT_EXT										0x8444
#define GL_MAP2_TANGENT_EXT										0x8445
#define GL_MAP1_BINORMAL_EXT									0x8446
#define GL_MAP2_BINORMAL_EXT									0x8447

typedef void (APIENTRY * glTangent3bEXTPROC) (GLbyte tx, GLbyte ty, GLbyte tz);
typedef void (APIENTRY * glTangent3bvEXTPROC) (const GLbyte *v);
typedef void (APIENTRY * glTangent3dEXTPROC) (GLdouble tx, GLdouble ty, GLdouble tz);
typedef void (APIENTRY * glTangent3dvEXTPROC) (const GLdouble *v);
typedef void (APIENTRY * glTangent3fEXTPROC) (GLfloat tx, GLfloat ty, GLfloat tz);
typedef void (APIENTRY * glTangent3fvEXTPROC) (const GLfloat *v);
typedef void (APIENTRY * glTangent3iEXTPROC) (GLint tx, GLint ty, GLint tz);
typedef void (APIENTRY * glTangent3ivEXTPROC) (const GLint *v);
typedef void (APIENTRY * glTangent3sEXTPROC) (GLshort tx, GLshort ty, GLshort tz);
typedef void (APIENTRY * glTangent3svEXTPROC) (const GLshort *v);
typedef void (APIENTRY * glBinormal3bEXTPROC) (GLbyte bx, GLbyte by, GLbyte bz);
typedef void (APIENTRY * glBinormal3bvEXTPROC) (const GLbyte *v);
typedef void (APIENTRY * glBinormal3dEXTPROC) (GLdouble bx, GLdouble by, GLdouble bz);
typedef void (APIENTRY * glBinormal3dvEXTPROC) (const GLdouble *v);
typedef void (APIENTRY * glBinormal3fEXTPROC) (GLfloat bx, GLfloat by, GLfloat bz);
typedef void (APIENTRY * glBinormal3fvEXTPROC) (const GLfloat *v);
typedef void (APIENTRY * glBinormal3iEXTPROC) (GLint bx, GLint by, GLint bz);
typedef void (APIENTRY * glBinormal3ivEXTPROC) (const GLint *v);
typedef void (APIENTRY * glBinormal3sEXTPROC) (GLshort bx, GLshort by, GLshort bz);
typedef void (APIENTRY * glBinormal3svEXTPROC) (const GLshort *v);
typedef void (APIENTRY * glTangentPointerEXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * glBinormalPointerEXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);

extern glTangent3bEXTPROC glTangent3bEXT;
extern glTangent3bvEXTPROC glTangent3bvEXT;
extern glTangent3dEXTPROC glTangent3dEXT;
extern glTangent3dvEXTPROC glTangent3dvEXT;
extern glTangent3fEXTPROC glTangent3fEXT;
extern glTangent3fvEXTPROC glTangent3fvEXT;
extern glTangent3iEXTPROC glTangent3iEXT;
extern glTangent3ivEXTPROC glTangent3ivEXT;
extern glTangent3sEXTPROC glTangent3sEXT;
extern glTangent3svEXTPROC glTangent3svEXT;
extern glBinormal3bEXTPROC glBinormal3bEXT;
extern glBinormal3bvEXTPROC glBinormal3bvEXT;
extern glBinormal3dEXTPROC glBinormal3dEXT;
extern glBinormal3dvEXTPROC glBinormal3dvEXT;
extern glBinormal3fEXTPROC glBinormal3fEXT;
extern glBinormal3fvEXTPROC glBinormal3fvEXT;
extern glBinormal3iEXTPROC glBinormal3iEXT;
extern glBinormal3ivEXTPROC glBinormal3ivEXT;
extern glBinormal3sEXTPROC glBinormal3sEXT;
extern glBinormal3svEXTPROC glBinormal3svEXT;
extern glTangentPointerEXTPROC glTangentPointerEXT;
extern glBinormalPointerEXTPROC glBinormalPointerEXT;

#endif /* GL_EXT_coordinate_frame */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_COPY_TEXTURE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture 1

typedef void (APIENTRY * glCopyTexImage1DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (APIENTRY * glCopyTexImage2DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (APIENTRY * glCopyTexSubImage1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (APIENTRY * glCopyTexSubImage2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * glCopyTexSubImage3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

extern glCopyTexImage1DEXTPROC glCopyTexImage1DEXT;
extern glCopyTexImage2DEXTPROC glCopyTexImage2DEXT;
extern glCopyTexSubImage1DEXTPROC glCopyTexSubImage1DEXT;
extern glCopyTexSubImage2DEXTPROC glCopyTexSubImage2DEXT;
extern glCopyTexSubImage3DEXTPROC glCopyTexSubImage3DEXT;

#endif /* GL_EXT_copy_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_CULL_VERTEX-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_cull_vertex
#define GL_EXT_cull_vertex 1

#define GL_CULL_VERTEX_EXT                                      0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT                         0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT                      0x81AC

typedef void (APIENTRY * glCullParameterfvEXTPROC) (GLenum pname, GLfloat *params);
typedef void (APIENTRY * glCullParameterdvEXTPROC) (GLenum pname, GLdouble *params);

extern glCullParameterfvEXTPROC glCullParameterfvEXT;
extern glCullParameterdvEXTPROC glCullParameterdvEXT;
 

#endif /* GL_EXT_cull_vertex */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_DEPTH_BOUNDS_TEST-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test 1

#define GL_DEPTH_BOUNDS_TEST_EXT                                0x8890
#define GL_DEPTH_BOUNDS_EXT                                     0x8891

typedef void (APIENTRY * glDepthBoundsEXTPROC) (GLclampd zmin, GLclampd zmax);

extern glDepthBoundsEXTPROC glDepthBoundsEXT;

#endif /* GL_EXT_depth_bounds_test */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_MIRROR_CLAMP----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp 1

#define GL_MIRROR_CLAMP_EXT               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT       0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT     0x8912

#endif /* GL_EXT_texture_mirror_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_BLEND_EQUATION_SEPARATE-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate 1

#define GL_BLEND_EQUATION_RGB_EXT         GL_BLEND_EQUATION
#define GL_BLEND_EQUATION_ALPHA_EXT       0x883D

typedef void (APIENTRY * glBlendEquationSeparateEXTPROC) (GLenum modeRGB, GLenum modeAlpha);

extern glBlendEquationSeparateEXTPROC glBlendEquationSeparateEXT;

#endif /* GL_EXT_blend_equation_separate */

/*-------------------------------------------------------------------*/
/*------------GL_MESA_PACK_INVERT------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_MESA_pack_invert
#define GL_MESA_pack_invert 1

#define GL_PACK_INVERT_MESA				0x8758

#endif /* GL_MESA_pack_invert */

/*-------------------------------------------------------------------*/
/*------------GL_MESA_YCBCR_TEXTURE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_MESA_ycbcr_texture
#define GL_MESA_ycbcr_texture 1

#define GL_UNSIGNED_SHORT_8_8_MESA        0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_MESA    0x85BB
#define GL_YCBCR_MESA                     0x8757

#endif /* GL_MESA_ycbcr_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_PIXEL_BUFFER_OBJECT-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_pixel_buffer_object
#define GL_EXT_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_EXT          0x88EB
#define GL_PIXEL_UNPACK_BUFFER_EXT        0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT  0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF

#endif /* GL_EXT_pixel_buffer_object */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FRAGMENT_PROGRAM_OPTION--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_fragment_program_option
#define GL_NV_fragment_program_option 1

#endif /* GL_NV_fragment_program_option */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FRAGMENT_PROGRAM2--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_fragment_program2
#define GL_NV_fragment_program2 1

#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV      0x88F5
#define GL_MAX_PROGRAM_IF_DEPTH_NV        0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV      0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV      0x88F8

#endif /* GL_NV_fragment_program2 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_PROGRAM2_OPTION---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_program2_option
#define GL_NV_vertex_program2_option 1

/* reuse GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV */
/* reuse GL_MAX_PROGRAM_CALL_DEPTH_NV */

#endif /* GL_NV_vertex_program2_option */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_PROGRAM3----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_program3
#define GL_NV_vertex_program3 1

/* reuse GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB */

#endif /* GL_NV_vertex_program3 */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_DRAW_RANGE_ELEMENTS-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_draw_range_elements
#define GL_EXT_draw_range_elements 1

typedef void (APIENTRY * glDrawRangeElementsEXTPROC) ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

extern glDrawRangeElementsEXTPROC glDrawRangeElementsEXT;

#define GL_MAX_ELEMENTS_VERTICES_EXT                            0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT                             0x80E9

#endif

/*-------------------------------------------------------------------*/
/*------------GL_EXT_FOG_COORD---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord 1

#define GL_FOG_COORDINATE_SOURCE_EXT                            0x8450
#define GL_FOG_COORDINATE_EXT                                   0x8451
#define GL_FRAGMENT_DEPTH_EXT                                   0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT                           0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT                        0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT                      0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT                     0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT                             0x8457

typedef void (APIENTRY * glFogCoordfEXTPROC) (GLfloat coord);
typedef void (APIENTRY * glFogCoordfvEXTPROC) (const GLfloat *coord);
typedef void (APIENTRY * glFogCoorddEXTPROC) (GLdouble coord);
typedef void (APIENTRY * glFogCoorddvEXTPROC) (const GLdouble *coord);
typedef void (APIENTRY * glFogCoordPointerEXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);

extern glFogCoordfEXTPROC glFogCoordfEXT;
extern glFogCoordfvEXTPROC glFogCoordfvEXT;
extern glFogCoorddEXTPROC glFogCoorddEXT;
extern glFogCoorddvEXTPROC glFogCoorddvEXT;
extern glFogCoordPointerEXTPROC glFogCoordPointerEXT;

#endif /* GL_EXT_fog_coord */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_FRAMEBUFFER_OBJECT------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT					0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT							0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT								0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT								0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT				0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT				0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT				0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT		0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT		0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT								0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT				0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT		0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT		0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT				0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT					0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT				0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT				0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT							0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS_EXT							0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT								0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT								0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT								0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT								0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT								0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT								0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT								0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT								0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT								0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT								0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT								0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT								0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT								0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT								0x8CED
#define GL_COLOR_ATTACHMENT14_EXT								0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT								0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT									0x8D00
#define GL_STENCIL_ATTACHMENT_EXT								0x8D20
#define GL_FRAMEBUFFER_EXT										0x8D40
#define GL_RENDERBUFFER_EXT										0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT								0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT								0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT						0x8D44
#define GL_STENCIL_INDEX1_EXT									0x8D46
#define GL_STENCIL_INDEX4_EXT									0x8D47
#define GL_STENCIL_INDEX8_EXT									0x8D48
#define GL_STENCIL_INDEX16_EXT									0x8D49
#define GL_RENDERBUFFER_RED_SIZE_EXT							0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT							0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT							0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT							0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT							0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT						0x8D55

typedef GLboolean (APIENTRY * glIsRenderbufferEXTPROC) (GLuint renderbuffer);
typedef void (APIENTRY * glBindRenderbufferEXTPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY * glDeleteRenderbuffersEXTPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRY * glGenRenderbuffersEXTPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY * glRenderbufferStorageEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY * glGetRenderbufferParameterivEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (APIENTRY * glIsFramebufferEXTPROC) (GLuint framebuffer);
typedef void (APIENTRY * glBindFramebufferEXTPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY * glDeleteFramebuffersEXTPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY * glGenFramebuffersEXTPROC) (GLsizei n, GLuint *framebuffers);
typedef GLenum (APIENTRY * glCheckFramebufferStatusEXTPROC) (GLenum target);
typedef void (APIENTRY * glFramebufferTexture1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * glFramebufferTexture2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * glFramebufferTexture3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (APIENTRY * glFramebufferRenderbufferEXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRY * glGetFramebufferAttachmentParameterivEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (APIENTRY * glGenerateMipmapEXTPROC) (GLenum target);

extern glIsRenderbufferEXTPROC glIsRenderbufferEXT;
extern glBindRenderbufferEXTPROC glBindRenderbufferEXT;
extern glDeleteRenderbuffersEXTPROC glDeleteRenderbuffersEXT;
extern glGenRenderbuffersEXTPROC glGenRenderbuffersEXT;
extern glRenderbufferStorageEXTPROC glRenderbufferStorageEXT;
extern glGetRenderbufferParameterivEXTPROC glGetRenderbufferParameterivEXT;
extern glIsFramebufferEXTPROC glIsFramebufferEXT;
extern glBindFramebufferEXTPROC glBindFramebufferEXT;
extern glDeleteFramebuffersEXTPROC glDeleteFramebuffersEXT;
extern glGenFramebuffersEXTPROC glGenFramebuffersEXT;
extern glCheckFramebufferStatusEXTPROC glCheckFramebufferStatusEXT;
extern glFramebufferTexture1DEXTPROC glFramebufferTexture1DEXT;
extern glFramebufferTexture2DEXTPROC glFramebufferTexture2DEXT;
extern glFramebufferTexture3DEXTPROC glFramebufferTexture3DEXT;
extern glFramebufferRenderbufferEXTPROC glFramebufferRenderbufferEXT;
extern glGetFramebufferAttachmentParameterivEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern glGenerateMipmapEXTPROC glGenerateMipmapEXT;

#endif /* GL_EXT_framebuffer_object */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_HISTOGRAM---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_histogram
#define GL_EXT_histogram 1

#define GL_HISTOGRAM_EXT										0x8024
#define GL_PROXY_HISTOGRAM_EXT									0x8025
#define GL_HISTOGRAM_WIDTH_EXT									0x8026
#define GL_HISTOGRAM_FORMAT_EXT									0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT								0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT								0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT								0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT								0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT							0x802C
#define GL_HISTOGRAM_SINK_EXT									0x802D
#define GL_MINMAX_EXT											0x802E
#define GL_MINMAX_FORMAT_EXT									0x802F
#define GL_MINMAX_SINK_EXT										0x8030
#define GL_TABLE_TOO_LARGE_EXT									0x8031

typedef void (APIENTRY * glGetHistogramEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
typedef void (APIENTRY * glGetHistogramParameterfvEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetHistogramParameterivEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetMinMaxEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
typedef void (APIENTRY * glGetMinMaxParameterfvEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetMinMaxParameterivEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glHistogramEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (APIENTRY * glMinMaxEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (APIENTRY * glResetHistogramEXTPROC) (GLenum target);
typedef void (APIENTRY * glResetMinMaxEXTPROC) (GLenum target);

extern glGetHistogramEXTPROC glGetHistogramEXT;
extern glGetHistogramParameterfvEXTPROC glGetHistogramParameterfvEXT;
extern glGetHistogramParameterivEXTPROC	glGetHistogramParameterivEXT;
extern glGetMinMaxEXTPROC glGetMinMaxEXT;
extern glGetMinMaxParameterfvEXTPROC glGetMinMaxParameterfvEXT;
extern glGetMinMaxParameterivEXTPROC glGetMinMaxParameterivEXT;
extern glHistogramEXTPROC glHistogramEXT;
extern glResetMinMaxEXTPROC glResetMinMaxEXT;

#endif /* GL_EXT_histogram */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_INDEX_ARRAY_FORMATS-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_index_array_formats
#define GL_EXT_index_array_formats 1

#define GL_IUI_V2F_EXT                    						0x81AD
#define GL_IUI_V3F_EXT                    						0x81AE
#define GL_IUI_N3F_V2F_EXT                						0x81AF
#define GL_IUI_N3F_V3F_EXT                						0x81B0
#define GL_T2F_IUI_V2F_EXT                						0x81B1
#define GL_T2F_IUI_V3F_EXT                						0x81B2
#define GL_T2F_IUI_N3F_V2F_EXT            						0x81B3
#define GL_T2F_IUI_N3F_V3F_EXT            						0x81B4

#endif /* GL_EXT_index_array_formats */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_INDEX_FUNC--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_index_func
#define GL_EXT_index_func 1

#define GL_INDEX_TEST_EXT										0x81B5
#define GL_INDEX_TEST_FUNC_EXT									0x81B6
#define GL_INDEX_TEST_REF_EXT									0x81B7

typedef void (APIENTRY * glIndexFuncEXTPROC) (GLenum func, GLclampf ref);

extern glIndexFuncEXTPROC glIndexFuncEXT;

#endif /* GL_EXT_index_func */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_INDEX_MATERIAL----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_index_material
#define GL_EXT_index_material 1

#define GL_INDEX_MATERIAL_EXT									0x81B8
#define GL_INDEX_MATERIAL_PARAMETER_EXT							0x81B9
#define GL_INDEX_MATERIAL_FACE_EXT								0x81BA

typedef void (APIENTRY * glIndexMaterialEXTPROC) (GLenum face, GLenum mode);

extern glIndexMaterialEXTPROC glIndexMaterialEXT;

#endif /* GL_EXT_index_material */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_INDEX_TEXTURE-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_index_texture
#define GL_EXT_index_texture 1

#endif /* GL_EXT_index_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_LIGHT_TEXTURE-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_light_texture
#define GL_EXT_light_texture 1

#define GL_FRAGMENT_MATERIAL_EXT          0x8349
#define GL_FRAGMENT_NORMAL_EXT            0x834A
#define GL_FRAGMENT_COLOR_EXT             0x834C
#define GL_ATTENUATION_EXT                0x834D
#define GL_SHADOW_ATTENUATION_EXT         0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT   0x834F
#define GL_TEXTURE_LIGHT_EXT              0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT      0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352

typedef void (APIENTRY * glApplyTextureEXTPROC) (GLenum mode);
typedef void (APIENTRY * glTextureLightEXTPROC) (GLenum pname);
typedef void (APIENTRY * glTextureMaterialEXTPROC) (GLenum face, GLenum mode);

extern glApplyTextureEXTPROC glApplyTextureEXT;
extern glTextureLightEXTPROC glTextureLightEXT;
extern glTextureMaterialEXTPROC glTextureMaterialEXT;

#endif /* GL_EXT_light_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_MISC_ATTRIBUTE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_misc_attribute
#define GL_EXT_misc_attribute 1

#endif /* GL_EXT_misc_attribute */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_MULTI_DRAW_ARRAYS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays 1

typedef void (APIENTRY * glMultiDrawArraysEXTPROC) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * glMultiDrawElementsEXTPROC) (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);

extern glMultiDrawArraysEXTPROC glMultiDrawArraysEXT;
extern glMultiDrawElementsEXTPROC glMultiDrawElementsEXT;

#endif /* GL_EXT_multi_draw_arrays */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_MULTISAMPLE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_multisample
#define GL_EXT_multisample 1

#define GL_MULTISAMPLE_EXT										0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT								0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT								0x809F
#define GL_SAMPLE_MASK_EXT										0x80A0
#define GL_1PASS_EXT											0x80A1
#define GL_2PASS_0_EXT											0x80A2
#define GL_2PASS_1_EXT											0x80A3
#define GL_4PASS_0_EXT											0x80A4
#define GL_4PASS_1_EXT											0x80A5
#define GL_4PASS_2_EXT											0x80A6
#define GL_4PASS_3_EXT											0x80A7
#define GL_SAMPLE_BUFFERS_EXT									0x80A8
#define GL_SAMPLES_EXT											0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT								0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT								0x80AB
#define GL_SAMPLE_PATTERN_EXT									0x80AC
#define GL_MULTISAMPLE_BIT_EXT									0x20000000

typedef void (APIENTRY * glSampleMaskEXTPROC) (GLclampf value, GLboolean invert);
typedef void (APIENTRY * glSamplePatternEXTPROC) (GLenum pattern);

extern glSampleMaskEXTPROC glSampleMaskEXT;
extern glSamplePatternEXTPROC glSamplePatternEXT;

#endif /* GL_EXT_multisample */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_PACKED_PIXELS-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels 1

#define GL_UNSIGNED_BYTE_3_3_2_EXT        						0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT     						0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     						0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT       						0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT    						0x8036

#endif /* GL_EXT_packed_pixels */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_PALETTED_TEXTURE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture 1

#define GL_COLOR_INDEX1_EXT               						0x80E2
#define GL_COLOR_INDEX2_EXT               						0x80E3
#define GL_COLOR_INDEX4_EXT               						0x80E4
#define GL_COLOR_INDEX8_EXT               						0x80E5
#define GL_COLOR_INDEX12_EXT              						0x80E6
#define GL_COLOR_INDEX16_EXT              						0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT         						0x80ED

typedef void (APIENTRY * glColorTableEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void (APIENTRY * glGetColorTableEXTPROC) (GLenum target, GLenum format, GLenum type, GLvoid *data);
typedef void (APIENTRY * glGetColorTableParameterivEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetColorTableParameterfvEXTPROC) (GLenum target, GLenum pname, GLfloat *params);

extern glColorTableEXTPROC glColorTableEXT;
extern glGetColorTableEXTPROC glGetColorTableEXT;
extern glGetColorTableParameterivEXTPROC glGetColorTableParameterivEXT;
extern glGetColorTableParameterfvEXTPROC glGetColorTableParameterfvEXT;

#endif /* GL_EXT_paletted_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_PIXEL_TRANSFORM---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_pixel_transform
#define GL_EXT_pixel_transform 1

#define GL_PIXEL_TRANSFORM_2D_EXT								0x8330
#define GL_PIXEL_MAG_FILTER_EXT									0x8331
#define GL_PIXEL_MIN_FILTER_EXT									0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT								0x8333
#define GL_CUBIC_EXT											0x8334
#define GL_AVERAGE_EXT											0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT					0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT				0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT						0x8338

typedef void (APIENTRY * glPixelTransformParameteriEXTPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY * glPixelTransformParameterfEXTPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY * glPixelTransformParameterivEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glPixelTransformParameterfvEXTPROC) (GLenum target, GLenum pname, const GLfloat *params);

extern glPixelTransformParameteriEXTPROC glPixelTransformParameteriEXT;
extern glPixelTransformParameterfEXTPROC glPixelTransformParameterfEXT;
extern glPixelTransformParameterivEXTPROC glPixelTransformParameterivEXT;
extern glPixelTransformParameterfvEXTPROC glPixelTransformParameterfvEXT;

#endif /* GL_EXT_pixel_transform */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_PIXEL_TRANSFORM_COLOR_TABLE---------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_pixel_transform_color_table
#define GL_EXT_pixel_transform_color_table 1

#endif /* GL_EXT_pixel_transform_color_table */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_POINT_PARAMETERS--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters 1

#define GL_POINT_SIZE_MIN_EXT                                   0x8126
#define GL_POINT_SIZE_MAX_EXT                                   0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT                        0x8128
#define GL_DISTANCE_ATTENUATION_EXT                             0x8129

typedef void (APIENTRY * glPointParameterfEXTPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glPointParameterfvEXTPROC) (GLenum pname, const GLfloat *params);

extern glPointParameterfEXTPROC glPointParameterfEXT;
extern glPointParameterfvEXTPROC glPointParameterfvEXT;

#endif /* GL_EXT_point_parameters */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_POLYGON_OFFSET----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset 1

#define GL_POLYGON_OFFSET_EXT									0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT							0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT								0x8039

typedef void (APIENTRY * glPolygonOffsetEXTPROC) (GLfloat factor, GLfloat bias);

extern glPolygonOffsetEXTPROC glPolygonOffsetEXT;

#endif /* GL_EXT_polygon_offset */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_RESCALE_NORMAL----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal 1

#define GL_RESCALE_NORMAL_EXT									0x803A

#endif /* GL_EXT_rescale_normal */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_SECONDARY_COLOR---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_secondary_color
#define GL_EXT_secondary_color 1

#define GL_COLOR_SUM_EXT                                        0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT                          0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT                       0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT                       0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT                     0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT                    0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT                            0x845E

typedef void (APIENTRY * glSecondaryColor3bEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (APIENTRY * glSecondaryColor3bvEXTPROC) (const GLbyte *v);
typedef void (APIENTRY * glSecondaryColor3dEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (APIENTRY * glSecondaryColor3dvEXTPROC) (const GLdouble *v);
typedef void (APIENTRY * glSecondaryColor3fEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (APIENTRY * glSecondaryColor3fvEXTPROC) (const GLfloat *v);
typedef void (APIENTRY * glSecondaryColor3iEXTPROC) (GLint red, GLint green, GLint blue);
typedef void (APIENTRY * glSecondaryColor3ivEXTPROC) (const GLint *v);
typedef void (APIENTRY * glSecondaryColor3sEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (APIENTRY * glSecondaryColor3svEXTPROC) (const GLshort *v);
typedef void (APIENTRY * glSecondaryColor3ubEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (APIENTRY * glSecondaryColor3ubvEXTPROC) (const GLubyte *v);
typedef void (APIENTRY * glSecondaryColor3uiEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (APIENTRY * glSecondaryColor3uivEXTPROC) (const GLuint *v);
typedef void (APIENTRY * glSecondaryColor3usEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (APIENTRY * glSecondaryColor3usvEXTPROC) (const GLushort *v);
typedef void (APIENTRY * glSecondaryColorPointerEXTPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);

extern glSecondaryColor3bEXTPROC glSecondaryColor3bEXT;
extern glSecondaryColor3bvEXTPROC glSecondaryColor3bvEXT;
extern glSecondaryColor3dEXTPROC glSecondaryColor3dEXT;
extern glSecondaryColor3dvEXTPROC glSecondaryColor3dvEXT;
extern glSecondaryColor3fEXTPROC glSecondaryColor3fEXT;
extern glSecondaryColor3fvEXTPROC glSecondaryColor3fvEXT;
extern glSecondaryColor3iEXTPROC glSecondaryColor3iEXT;
extern glSecondaryColor3ivEXTPROC glSecondaryColor3ivEXT;
extern glSecondaryColor3sEXTPROC glSecondaryColor3sEXT;
extern glSecondaryColor3svEXTPROC glSecondaryColor3svEXT;
extern glSecondaryColor3ubEXTPROC glSecondaryColor3ubEXT;
extern glSecondaryColor3ubvEXTPROC glSecondaryColor3ubvEXT;
extern glSecondaryColor3uiEXTPROC glSecondaryColor3uiEXT;
extern glSecondaryColor3uivEXTPROC glSecondaryColor3uivEXT;
extern glSecondaryColor3usEXTPROC glSecondaryColor3usEXT;
extern glSecondaryColor3usvEXTPROC glSecondaryColor3usvEXT;
extern glSecondaryColorPointerEXTPROC glSecondaryColorPointerEXT;

#endif /* GL_EXT_secondary_color */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_SEPARATE_SPECULAR_COLOR-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color 1

#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT                        0x81F8
#define GL_SINGLE_COLOR_EXT                                     0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT                          0x81FA

#endif /* GL_EXT_separate_specular_color */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_SHADOW_FUNCS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_shadow_funcs
#define GL_EXT_shadow_funcs 1

#endif /* GL_EXT_shadow_funcs */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_SHARED_TEXTURE_PALETTE--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1

#define GL_SHARED_TEXTURE_PALETTE_EXT							0x81FB

#endif /* GL_EXT_shared_texture_palette */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_STENCIL_TWO_SIDE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_stencil_two_side
#define GL_EXT_stencil_two_side 1

typedef void (APIENTRY * glActiveStencilFaceEXTPROC) (GLenum face);

extern glActiveStencilFaceEXTPROC glActiveStencilFaceEXT;

#define GL_STENCIL_TEST_TWO_SIDE_EXT                            0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT                              0x8911

#endif /* GL_EXT_stencil_two_side */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_STENCIL_WRAP------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_stencil_wrap
#define GL_EXT_stencil_wrap 1

#define GL_INCR_WRAP_EXT                                        0x8507
#define GL_DECR_WRAP_EXT                                        0x8508

#endif /* GL_EXT_stencil_wrap */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_SUBTEXTURE--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture 1

typedef void (APIENTRY * glTexSubImage1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * glTexSubImage2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

extern glTexSubImage1DEXTPROC glTexSubImage1DEXT;
extern glTexSubImage2DEXTPROC glTexSubImage2DEXT;

#endif /* GL_EXT_subtexture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXGEN_REFLECTION-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texgen_reflection
#define GL_EXT_texgen_reflection 1

#define GL_NORMAL_MAP_EXT										0x8511
#define GL_REFLECTION_MAP_EXT									0x8512

#endif /* GL_EXT_texgen_reflection */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture
#define GL_EXT_texture 1

#define GL_ALPHA4_EXT											0x803B
#define GL_ALPHA8_EXT                     						0x803C
#define GL_ALPHA12_EXT                    						0x803D
#define GL_ALPHA16_EXT                    						0x803E
#define GL_LUMINANCE4_EXT                 						0x803F
#define GL_LUMINANCE8_EXT                 						0x8040
#define GL_LUMINANCE12_EXT                						0x8041
#define GL_LUMINANCE16_EXT                						0x8042
#define GL_LUMINANCE4_ALPHA4_EXT          						0x8043
#define GL_LUMINANCE6_ALPHA2_EXT          						0x8044
#define GL_LUMINANCE8_ALPHA8_EXT          						0x8045
#define GL_LUMINANCE12_ALPHA4_EXT         						0x8046
#define GL_LUMINANCE12_ALPHA12_EXT        						0x8047
#define GL_LUMINANCE16_ALPHA16_EXT        						0x8048
#define GL_INTENSITY_EXT                  						0x8049
#define GL_INTENSITY4_EXT                 						0x804A
#define GL_INTENSITY8_EXT                 						0x804B
#define GL_INTENSITY12_EXT                						0x804C
#define GL_INTENSITY16_EXT                						0x804D
#define GL_RGB2_EXT                       						0x804E
#define GL_RGB4_EXT                       						0x804F
#define GL_RGB5_EXT                       						0x8050
#define GL_RGB8_EXT                       						0x8051
#define GL_RGB10_EXT                      						0x8052
#define GL_RGB12_EXT                      						0x8053
#define GL_RGB16_EXT                      						0x8054
#define GL_RGBA2_EXT                      						0x8055
#define GL_RGBA4_EXT                      						0x8056
#define GL_RGB5_A1_EXT                    						0x8057
#define GL_RGBA8_EXT                      						0x8058
#define GL_RGB10_A2_EXT                   						0x8059
#define GL_RGBA12_EXT                     						0x805A
#define GL_RGBA16_EXT                     						0x805B
#define GL_TEXTURE_RED_SIZE_EXT           						0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT         						0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT          						0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT         						0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT     						0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT     						0x8061
#define GL_REPLACE_EXT                    						0x8062
#define GL_PROXY_TEXTURE_1D_EXT           						0x8063
#define GL_PROXY_TEXTURE_2D_EXT           						0x8064
#define GL_TEXTURE_TOO_LARGE_EXT          						0x8065

#endif /* GL_EXT_texture */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE3D---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D 1

#define GL_PACK_SKIP_IMAGES_EXT           						0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT          						0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT         						0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT        						0x806E
#define GL_TEXTURE_3D_EXT                 						0x806F
#define GL_PROXY_TEXTURE_3D_EXT           						0x8070
#define GL_TEXTURE_DEPTH_EXT              						0x8071
#define GL_TEXTURE_WRAP_R_EXT             						0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT        						0x8073

typedef void (APIENTRY * glTexImage3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * glTexSubImage3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

extern glTexImage3DEXTPROC glTexImage3DEXT;
extern glTexSubImage3DEXTPROC glTexSubImage3DEXT;

#endif /* GL_EXT_texture3D */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_COMPRESSION_S3TC------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                         0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                        0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                        0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                        0x83F3

#endif /* GL_EXT_texture_compression_s3tc */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_CUBE_MAP--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map 1

#define GL_NORMAL_MAP_EXT                 						0x8511
#define GL_REFLECTION_MAP_EXT             						0x8512
#define GL_TEXTURE_CUBE_MAP_EXT           						0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT   						0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 						0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 						0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 						0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 						0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 						0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 						0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT     						0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT  						0x851C

#endif /* GL_EXT_texture_cube_map */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_EDGE_CLAMP------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_EXT									0x812F

#endif /* GL_EXT_texture_edge_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_ENV_ADD---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_env_add
#define GL_EXT_texture_env_add 1

#endif /* GL_EXT_texture_env_add */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_ENV_DOT3--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_env_dot3
#define GL_EXT_texture_env_dot3 1

#define GL_DOT3_RGB_EXT                   0x8740
#define GL_DOT3_RGBA_EXT                  0x8741

#endif /* GL_EXT_texture_env_dot3 */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_texture_env_combine-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_env_combine
#define GL_EXT_texture_env_combine 1

#define GL_COMBINE_EXT                                          0x8570
#define GL_COMBINE_RGB_EXT                                      0x8571
#define GL_COMBINE_ALPHA_EXT                                    0x8572
#define GL_RGB_SCALE_EXT                                        0x8573
#define GL_ADD_SIGNED_EXT                                       0x8574
#define GL_INTERPOLATE_EXT                                      0x8575
#define GL_CONSTANT_EXT                                         0x8576
#define GL_PRIMARY_COLOR_EXT                                    0x8577
#define GL_PREVIOUS_EXT                                         0x8578
#define GL_SOURCE0_RGB_EXT                                      0x8580
#define GL_SOURCE1_RGB_EXT                                      0x8581
#define GL_SOURCE2_RGB_EXT                                      0x8582
#define GL_SOURCE0_ALPHA_EXT                                    0x8588
#define GL_SOURCE1_ALPHA_EXT                                    0x8589
#define GL_SOURCE2_ALPHA_EXT                                    0x858A
#define GL_OPERAND0_RGB_EXT                                     0x8590
#define GL_OPERAND1_RGB_EXT                                     0x8591
#define GL_OPERAND2_RGB_EXT                                     0x8592
#define GL_OPERAND0_ALPHA_EXT                                   0x8598
#define GL_OPERAND1_ALPHA_EXT                                   0x8599
#define GL_OPERAND2_ALPHA_EXT                                   0x859A

#endif /* GL_EXT_texture_env_combine */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_FILTER_ANISOTROPIC----------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY_EXT                           0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT                       0x84FF

#endif /* GL_EXT_texture_filter_anisotropic */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_LOD-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_lod
#define GL_EXT_texture_lod 1

#endif /* GL_EXT_texture_lod */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_LOD_BIAS--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias 1

#define GL_TEXTURE_FILTER_CONTROL_EXT                           0x8500
#define GL_TEXTURE_LOD_BIAS_EXT                                 0x8501
#define GL_MAX_TEXTURE_LOD_BIAS_EXT                             0x84FD

#endif /* GL_EXT_texture_lod_bias */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_OBJECT----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object 1

#define GL_TEXTURE_PRIORITY_EXT									0x8066
#define GL_TEXTURE_RESIDENT_EXT									0x8067
#define GL_TEXTURE_1D_BINDING_EXT								0x8068
#define GL_TEXTURE_2D_BINDING_EXT								0x8069
#define GL_TEXTURE_3D_BINDING_EXT								0x806A

typedef GLboolean (APIENTRY * glAreTexturesResidentEXTPROC) (GLsizei n, const GLuint *textures, GLboolean *residences);
typedef void (APIENTRY * glBindTextureEXTPROC) (GLenum target, GLuint texture);
typedef void (APIENTRY * glDeleteTexturesEXTPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY * glGenTexturesEXTPROC) (GLsizei n, GLuint *textures);
typedef GLboolean (APIENTRY * glIsTextureEXTPROC) (GLuint texture);
typedef void (APIENTRY * glPrioritizeTexturesEXTPROC) (GLsizei n, const GLuint *textures, const GLclampf *priorities);

extern glAreTexturesResidentEXTPROC glAreTexturesResidentEXT;
extern glBindTextureEXTPROC glBindTextureEXT;
extern glDeleteTexturesEXTPROC glDeleteTexturesEXT;
extern glGenTexturesEXTPROC glGenTexturesEXT;
extern glIsTextureEXTPROC glIsTextureEXT;
extern glPrioritizeTexturesEXTPROC glPrioritizeTexturesEXT;

#endif /* GL_EXT_texture_object */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_PERTURB_NORMAL--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_perturb_normal
#define GL_EXT_texture_perturb_normal 1

#define GL_PERTURB_EXT											0x85AE
#define GL_TEXTURE_NORMAL_EXT									0x85AF

typedef void (APIENTRY * glTextureNormalEXTPROC) (GLenum mode);

extern glTextureNormalEXTPROC glTextureNormalEXT;

#endif /* GL_EXT_texture_perturb_normal */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_TEXTURE_RECTANGLE-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_texture_rectangle
#define GL_EXT_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_EXT                                0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT                        0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT                          0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT                       0x84F8

#endif /* GL_EXT_texture_rectangle */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_VERTEX_ARRAY------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array 1

#define GL_VERTEX_ARRAY_EXT               						0x8074
#define GL_NORMAL_ARRAY_EXT               						0x8075
#define GL_COLOR_ARRAY_EXT                						0x8076
#define GL_INDEX_ARRAY_EXT                						0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT        						0x8078
#define GL_EDGE_FLAG_ARRAY_EXT            						0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT          						0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT          						0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT        						0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT         						0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT          						0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT        						0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT         						0x8080
#define GL_COLOR_ARRAY_SIZE_EXT           						0x8081
#define GL_COLOR_ARRAY_TYPE_EXT           						0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT         						0x8083
#define GL_COLOR_ARRAY_COUNT_EXT          						0x8084
#define GL_INDEX_ARRAY_TYPE_EXT           						0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT         						0x8086
#define GL_INDEX_ARRAY_COUNT_EXT          						0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT   						0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT   						0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 						0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT  						0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT     						0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT      						0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT       						0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT       						0x808F
#define GL_COLOR_ARRAY_POINTER_EXT        						0x8090
#define GL_INDEX_ARRAY_POINTER_EXT        						0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT						0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT							0x8093

typedef void (APIENTRY * glArrayElementEXTPROC) (GLint i);
typedef void (APIENTRY * glColorPointerEXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * glDrawArraysEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * glEdgeFlagPointerEXTPROC) (GLsizei stride, GLsizei count, const GLboolean *pointer);
typedef void (APIENTRY * glGetPointervEXTPROC) (GLenum pname, GLvoid* *params);
typedef void (APIENTRY * glIndexPointerEXTPROC) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * glNormalPointerEXTPROC) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * glTexCoordPointerEXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * glVertexPointerEXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);

extern glArrayElementEXTPROC glArrayElementEXT;
extern glColorPointerEXTPROC glColorPointerEXT;
extern glDrawArraysEXTPROC glDrawArraysEXT;
extern glEdgeFlagPointerEXTPROC glEdgeFlagPointerEXT;
extern glGetPointervEXTPROC glGetPointervEXT;
extern glIndexPointerEXTPROC glIndexPointerEXT;
extern glNormalPointerEXTPROC glNormalPointerEXT;
extern glTexCoordPointerEXTPROC glTexCoordPointerEXT;
extern glVertexPointerEXTPROC glVertexPointerEXT;

#endif /* GL_EXT_vertex_array */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_VERTEX_SHADER-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_vertex_shader
#define GL_EXT_vertex_shader 1

#define GL_VERTEX_SHADER_EXT                                    0x8780
#define GL_VERTEX_SHADER_BINDING_EXT                            0x8781
#define GL_OP_INDEX_EXT                                         0x8782
#define GL_OP_NEGATE_EXT                                        0x8783
#define GL_OP_DOT3_EXT                                          0x8784
#define GL_OP_DOT4_EXT                                          0x8785
#define GL_OP_MUL_EXT                                           0x8786
#define GL_OP_ADD_EXT                                           0x8787
#define GL_OP_MADD_EXT                                          0x8788
#define GL_OP_FRAC_EXT                                          0x8789
#define GL_OP_MAX_EXT                                           0x878A
#define GL_OP_MIN_EXT                                           0x878B
#define GL_OP_SET_GE_EXT                                        0x878C
#define GL_OP_SET_LT_EXT                                        0x878D
#define GL_OP_CLAMP_EXT                                         0x878E
#define GL_OP_FLOOR_EXT                                         0x878F
#define GL_OP_ROUND_EXT                                         0x8790
#define GL_OP_EXP_BASE_2_EXT                                    0x8791
#define GL_OP_LOG_BASE_2_EXT                                    0x8792
#define GL_OP_POWER_EXT                                         0x8793
#define GL_OP_RECIP_EXT                                         0x8794
#define GL_OP_RECIP_SQRT_EXT                                    0x8795
#define GL_OP_SUB_EXT                                           0x8796
#define GL_OP_CROSS_PRODUCT_EXT                                 0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT                               0x8798
#define GL_OP_MOV_EXT                                           0x8799
#define GL_OUTPUT_VERTEX_EXT                                    0x879A
#define GL_OUTPUT_COLOR0_EXT                                    0x879B
#define GL_OUTPUT_COLOR1_EXT                                    0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT                            0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT                            0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT                            0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT                            0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT                            0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT                            0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT                            0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT                            0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT                            0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT                            0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT                           0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT                           0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT                           0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT                           0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT                           0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT                           0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT                           0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT                           0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT                           0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT                           0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT                           0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT                           0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT                           0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT                           0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT                           0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT                           0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT                           0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT                           0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT                           0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT                           0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT                           0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT                           0x87BC
#define GL_OUTPUT_FOG_EXT                                       0x87BD
#define GL_SCALAR_EXT                                           0x87BE
#define GL_VECTOR_EXT                                           0x87BF
#define GL_MATRIX_EXT                                           0x87C0
#define GL_VARIANT_EXT                                          0x87C1
#define GL_INVARIANT_EXT                                        0x87C2
#define GL_LOCAL_CONSTANT_EXT                                   0x87C3
#define GL_LOCAL_EXT                                            0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT                   0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT                       0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT                     0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT                0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT                         0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT         0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT             0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT           0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT      0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT               0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT                       0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT                           0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT                         0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT                    0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT                             0x87D3
#define GL_VERTEX_SHADER_OPTIMIZED_EXT                          0x87D4
#define GL_X_EXT                                                0x87D5
#define GL_Y_EXT                                                0x87D6
#define GL_Z_EXT                                                0x87D7
#define GL_W_EXT                                                0x87D8
#define GL_NEGATIVE_X_EXT                                       0x87D9
#define GL_NEGATIVE_Y_EXT                                       0x87DA
#define GL_NEGATIVE_Z_EXT                                       0x87DB
#define GL_NEGATIVE_W_EXT                                       0x87DC
#define GL_ZERO_EXT                                             0x87DD
#define GL_ONE_EXT                                              0x87DE
#define GL_NEGATIVE_ONE_EXT                                     0x87DF
#define GL_NORMALIZED_RANGE_EXT                                 0x87E0
#define GL_FULL_RANGE_EXT                                       0x87E1
#define GL_CURRENT_VERTEX_EXT                                   0x87E2
#define GL_MVP_MATRIX_EXT                                       0x87E3
#define GL_VARIANT_VALUE_EXT                                    0x87E4
#define GL_VARIANT_DATATYPE_EXT                                 0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT                             0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT                               0x87E7
#define GL_VARIANT_ARRAY_EXT                                    0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT                            0x87E9
#define GL_INVARIANT_VALUE_EXT                                  0x87EA
#define GL_INVARIANT_DATATYPE_EXT                               0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT                             0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT                          0x87ED

typedef void (APIENTRY * glBeginVertexShaderEXTPROC) ();
typedef void (APIENTRY * glEndVertexShaderEXTPROC) ();
typedef void (APIENTRY * glBindVertexShaderEXTPROC) (GLuint id);
typedef GLuint (APIENTRY * glGenVertexShadersEXTPROC) (GLuint range);
typedef void (APIENTRY * glDeleteVertexShaderEXTPROC) (GLuint id);
typedef void (APIENTRY * glShaderOp1EXTPROC) (GLenum op, GLuint res, GLuint arg1);
typedef void (APIENTRY * glShaderOp2EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void (APIENTRY * glShaderOp3EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void (APIENTRY * glSwizzleEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (APIENTRY * glWriteMaskEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (APIENTRY * glInsertComponentEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef void (APIENTRY * glExtractComponentEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLuint (APIENTRY * glGenSymbolsEXTPROC) (GLenum dataType, GLenum storageType, GLenum range, GLuint components);
typedef void (APIENTRY * glSetInvariantEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (APIENTRY * glSetLocalConstantEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (APIENTRY * glVariantbvEXTPROC) (GLuint id, GLbyte *addr);
typedef void (APIENTRY * glVariantsvEXTPROC) (GLuint id, GLshort *addr);
typedef void (APIENTRY * glVariantivEXTPROC) (GLuint id, GLint *addr);
typedef void (APIENTRY * glVariantfvEXTPROC) (GLuint id, GLfloat *addr);
typedef void (APIENTRY * glVariantdvEXTPROC) (GLuint id, GLdouble *addr);
typedef void (APIENTRY * glVariantubvEXTPROC) (GLuint id, GLubyte *addr);
typedef void (APIENTRY * glVariantusvEXTPROC) (GLuint id, GLushort *addr);
typedef void (APIENTRY * glVariantuivEXTPROC) (GLuint id, GLuint *addr);
typedef void (APIENTRY * glVariantPointerEXTPROC) (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
typedef void (APIENTRY * glEnableVariantClientStateEXTPROC) (GLuint id);
typedef void (APIENTRY * glDisableVariantClientStateEXTPROC) (GLuint id);
typedef GLuint (APIENTRY * glBindLightParameterEXTPROC) (GLenum light, GLenum value);
typedef GLuint (APIENTRY * glBindMaterialParameterEXTPROC) (GLenum face, GLenum value);
typedef GLuint (APIENTRY * glBindTexGenParameterEXTPROC) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint (APIENTRY * glBindTextureUnitParameterEXTPROC) (GLenum unit, GLenum value);
typedef GLuint (APIENTRY * glBindParameterEXTPROC) (GLenum value);
typedef GLboolean (APIENTRY * glIsVariantEnabledEXTPROC) (GLuint id, GLenum cap);
typedef void (APIENTRY * glGetVariantBooleanvEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (APIENTRY * glGetVariantIntegervEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (APIENTRY * glGetVariantFloatvEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (APIENTRY * glGetVariantPointervEXTPROC) (GLuint id, GLenum value, GLvoid **data);
typedef void (APIENTRY * glGetInvariantBooleanvEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (APIENTRY * glGetInvariantIntegervEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (APIENTRY * glGetInvariantFloatvEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (APIENTRY * glGetLocalConstantBooleanvEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (APIENTRY * glGetLocalConstantIntegervEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (APIENTRY * glGetLocalConstantFloatvEXTPROC) (GLuint id, GLenum value, GLfloat *data);

extern glBeginVertexShaderEXTPROC glBeginVertexShaderEXT;
extern glEndVertexShaderEXTPROC glEndVertexShaderEXT;
extern glBindVertexShaderEXTPROC glBindVertexShaderEXT;
extern glGenVertexShadersEXTPROC glGenVertexShadersEXT;
extern glDeleteVertexShaderEXTPROC glDeleteVertexShaderEXT;
extern glShaderOp1EXTPROC glShaderOp1EXT;
extern glShaderOp2EXTPROC glShaderOp2EXT;
extern glShaderOp3EXTPROC glShaderOp3EXT;
extern glSwizzleEXTPROC glSwizzleEXT;
extern glWriteMaskEXTPROC glWriteMaskEXT;
extern glInsertComponentEXTPROC glInsertComponentEXT;
extern glExtractComponentEXTPROC glExtractComponentEXT;
extern glGenSymbolsEXTPROC glGenSymbolsEXT;
extern glSetInvariantEXTPROC glSetInvariantEXT;
extern glSetLocalConstantEXTPROC glSetLocalConstantEXT;
extern glVariantbvEXTPROC glVariantbvEXT;
extern glVariantsvEXTPROC glVariantsvEXT;
extern glVariantivEXTPROC glVariantivEXT;
extern glVariantfvEXTPROC glVariantfvEXT;
extern glVariantdvEXTPROC glVariantdvEXT;
extern glVariantubvEXTPROC glVariantubvEXT;
extern glVariantusvEXTPROC glVariantusvEXT;
extern glVariantuivEXTPROC glVariantuivEXT;
extern glVariantPointerEXTPROC glVariantPointerEXT;
extern glEnableVariantClientStateEXTPROC glEnableVariantClientStateEXT;
extern glDisableVariantClientStateEXTPROC glDisableVariantClientStateEXT;
extern glBindLightParameterEXTPROC glBindLightParameterEXT;
extern glBindMaterialParameterEXTPROC glBindMaterialParameterEXT;
extern glBindTexGenParameterEXTPROC glBindTexGenParameterEXT;
extern glBindTextureUnitParameterEXTPROC glBindTextureUnitParameterEXT;
extern glBindParameterEXTPROC glBindParameterEXT;
extern glIsVariantEnabledEXTPROC glIsVariantEnabledEXT;
extern glGetVariantBooleanvEXTPROC glGetVariantBooleanvEXT;
extern glGetVariantIntegervEXTPROC glGetVariantIntegervEXT;
extern glGetVariantFloatvEXTPROC glGetVariantFloatvEXT;
extern glGetVariantPointervEXTPROC glGetVariantPointervEXT;
extern glGetInvariantBooleanvEXTPROC glGetInvariantBooleanvEXT;
extern glGetInvariantIntegervEXTPROC glGetInvariantIntegervEXT;
extern glGetInvariantFloatvEXTPROC glGetInvariantFloatvEXT;
extern glGetLocalConstantBooleanvEXTPROC glGetLocalConstantBooleanvEXT;
extern glGetLocalConstantIntegervEXTPROC glGetLocalConstantIntegervEXT;
extern glGetLocalConstantFloatvEXTPROC glGetLocalConstantFloatvEXT;

#endif /* GL_EXT_vertex_shader */

/*-------------------------------------------------------------------*/
/*------------GL_EXT_VERTEX_WEIGHTING--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_EXT_vertex_weighting
#define GL_EXT_vertex_weighting 1

#define GL_MODELVIEW0_STACK_DEPTH_EXT                           0x0BA3  /* alias to GL_MODELVIEW_STACK_DEPTH */
#define GL_MODELVIEW1_STACK_DEPTH_EXT                           0x8502
#define GL_MODELVIEW0_MATRIX_EXT                                0x0BA6  /* alias to GL_MODELVIEW_MATRIX */
#define GL_MODELVIEW1_MATRIX_EXT                                0x8506
#define GL_VERTEX_WEIGHTING_EXT                                 0x8509
#define GL_MODELVIEW0_EXT                                       0x1700  /* alias to GL_MODELVIEW */
#define GL_MODELVIEW1_EXT                                       0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT                            0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT                              0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT                         0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT                         0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT                       0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT                      0x8510

typedef void (APIENTRY * glVertexWeightfEXTPROC) (GLfloat weight);
typedef void (APIENTRY * glVertexWeightfvEXTPROC) (const GLfloat *weight);
typedef void (APIENTRY * glVertexWeightPointerEXTPROC) (GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer);

extern glVertexWeightfEXTPROC glVertexWeightfEXT;
extern glVertexWeightfvEXTPROC glVertexWeightfvEXT;
extern glVertexWeightPointerEXTPROC glVertexWeightPointerEXT;

#endif /* GL_EXT_vertex_weighting */

/*-------------------------------------------------------------------*/
/*------------GL_FFDMASKSGIX-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_FfdMaskSGIX
#define GL_FfdMaskSGIX 1

#define GL_TEXTURE_DEFORMATION_BIT_SGIX							0x00000001
#define GL_GEOMETRY_DEFORMATION_BIT_SGIX						0x00000002

#endif /* GL_FfdMaskSGIX */

/*-------------------------------------------------------------------*/
/*------------GL_HP_CONVOLUTION_BORDER_MODES-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_HP_convolution_border_modes
#define GL_HP_convolution_border_modes 1

#define GL_IGNORE_BORDER_HP										0x8150
#define GL_CONSTANT_BORDER_HP									0x8151
#define GL_REPLICATE_BORDER_HP									0x8153
#define GL_CONVOLUTION_BORDER_COLOR_HP							0x8154

#endif /* GL_HP_convolution_border_modes */

/*-------------------------------------------------------------------*/
/*------------GL_HP_IMAGE_TRANSFORM----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_HP_image_transform
#define GL_HP_image_transform 1

#define GL_IMAGE_SCALE_X_HP										0x8155
#define GL_IMAGE_SCALE_Y_HP										0x8156
#define GL_IMAGE_TRANSLATE_X_HP									0x8157
#define GL_IMAGE_TRANSLATE_Y_HP									0x8158
#define GL_IMAGE_ROTATE_ANGLE_HP								0x8159
#define GL_IMAGE_ROTATE_ORIGIN_X_HP								0x815A
#define GL_IMAGE_ROTATE_ORIGIN_Y_HP								0x815B
#define GL_IMAGE_MAG_FILTER_HP									0x815C
#define GL_IMAGE_MIN_FILTER_HP									0x815D
#define GL_IMAGE_CUBIC_WEIGHT_HP								0x815E
#define GL_CUBIC_HP												0x815F
#define GL_AVERAGE_HP											0x8160
#define GL_IMAGE_TRANSFORM_2D_HP								0x8161
#define GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP					0x8162
#define GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP			0x8163

typedef void (APIENTRY * glImageTransformParameteriHPPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY * glImageTransformParameterfHPPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY * glImageTransformParameterivHPPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glImageTransformParameterfvHPPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glGetImageTransformParameterivHPPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetImageTransformParameterfvHPPROC) (GLenum target, GLenum pname, GLfloat *params);

extern glImageTransformParameteriHPPROC glImageTransformParameteriHP;
extern glImageTransformParameterfHPPROC glImageTransformParameterfHP;
extern glImageTransformParameterivHPPROC glImageTransformParameterivHP;
extern glImageTransformParameterfvHPPROC glImageTransformParameterfvHP;
extern glGetImageTransformParameterivHPPROC glGetImageTransformParameterivHP;
extern glGetImageTransformParameterfvHPPROC glGetImageTransformParameterfvHP;

#endif /* GL_HP_image_transform */

/*-------------------------------------------------------------------*/
/*------------GL_HP_OCCLUSION_TEST-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_HP_occlusion_test
#define GL_HP_occlusion_test 1

#define GL_OCCLUSION_TEST_HP                                    0x8165
#define GL_OCCLUSION_TEST_RESULT_HP                             0x8166

#endif /* GL_HP_occlusion_test */

/*-------------------------------------------------------------------*/
/*------------GL_HP_TEXTURE_LIGHTING---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_HP_texture_lighting
#define GL_HP_texture_lighting 1

#define GL_TEXTURE_LIGHTING_MODE_HP								0x8167
#define GL_TEXTURE_POST_SPECULAR_HP								0x8168
#define GL_TEXTURE_PRE_SPECULAR_HP								0x8169

#endif /* GL_HP_texture_lighting */

/*-------------------------------------------------------------------*/
/*------------GL_IBM_CULL_VERTEX-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_IBM_cull_vertex
#define GL_IBM_cull_vertex 1

#define GL_CULL_VERTEX_IBM										103050

#endif /* GL_IBM_cull_vertex */

/*-------------------------------------------------------------------*/
/*------------GL_IBM_MULTIMODE_DRAW_ARRAYS---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_IBM_multimode_draw_arrays
#define GL_IBM_multimode_draw_arrays 1

typedef void (APIENTRY * glMultiModeDrawArraysIBMPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
typedef void (APIENTRY * glMultiModeDrawElementsIBMPROC) (const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, GLint modestride);

extern glMultiModeDrawArraysIBMPROC glMultiModeDrawArraysIBM;
extern glMultiModeDrawElementsIBMPROC glMultiModeDrawElementsIBM;

#endif /* GL_IBM_multimode_draw_arrays */

/*-------------------------------------------------------------------*/
/*------------GL_IBM_RASTERPOS_CLIP----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_IBM_rasterpos_clip
#define GL_IBM_rasterpos_clip 1

#define GL_RASTER_POSITION_UNCLIPPED_IBM						0x19262

#endif /* GL_IBM_rasterpos_clip */

/*-------------------------------------------------------------------*/
/*------------GL_IBM_TEXTURE_MIRRORED_REPEAT-------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_IBM_texture_mirrored_repeat
#define GL_IBM_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_IBM									0x8370

#endif /* GL_IBM_texture_mirrored_repeat */

/*-------------------------------------------------------------------*/
/*------------GL_IBM_VERTEX_ARRAY_LISTS------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_IBM_vertex_array_lists
#define GL_IBM_vertex_array_lists 1

#define GL_VERTEX_ARRAY_LIST_IBM								103070
#define GL_NORMAL_ARRAY_LIST_IBM								103071
#define GL_COLOR_ARRAY_LIST_IBM									103072
#define GL_INDEX_ARRAY_LIST_IBM									103073
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM							103074
#define GL_EDGE_FLAG_ARRAY_LIST_IBM								103075
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM						103076
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM						103077
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM							103080
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM							103081
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM							103082
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM							103083
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM					103084
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM						103085
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM					103086
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM				103087

typedef void (APIENTRY * glColorPointerListIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glSecondaryColorPointerListIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glEdgeFlagPointerListIBMPROC) (GLint stride, const GLboolean* *pointer, GLint ptrstride);
typedef void (APIENTRY * glFogCoordPointerListIBMPROC) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glIndexPointerListIBMPROC) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glNormalPointerListIBMPROC) (GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glTexCoordPointerListIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
typedef void (APIENTRY * glVertexPointerListIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);

extern glColorPointerListIBMPROC glColorPointerListIBM;
extern glSecondaryColorPointerListIBMPROC glSecondaryColorPointerListIBM;
extern glEdgeFlagPointerListIBMPROC glEdgeFlagPointerListIBM;
extern glFogCoordPointerListIBMPROC glFogCoordPointerListIBM;
extern glIndexPointerListIBMPROC glIndexPointerListIBM;
extern glNormalPointerListIBMPROC glNormalPointerListIBM;
extern glTexCoordPointerListIBMPROC glTexCoordPointerListIBM;
extern glVertexPointerListIBMPROC glVertexPointerListIBM;

#endif /* GL_IBM_vertex_array_lists */

/*-------------------------------------------------------------------*/
/*------------GL_INGR_COLOR_CLAMP------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_INGR_color_clamp
#define GL_INGR_color_clamp 1

#define GL_RED_MIN_CLAMP_INGR									0x8560
#define GL_GREEN_MIN_CLAMP_INGR									0x8561
#define GL_BLUE_MIN_CLAMP_INGR									0x8562
#define GL_ALPHA_MIN_CLAMP_INGR									0x8563
#define GL_RED_MAX_CLAMP_INGR									0x8564
#define GL_GREEN_MAX_CLAMP_INGR									0x8565
#define GL_BLUE_MAX_CLAMP_INGR									0x8566
#define GL_ALPHA_MAX_CLAMP_INGR									0x8567

#endif /* GL_INGR_color_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_INGR_INTERLACE_READ---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_INGR_interlace_read
#define GL_INGR_interlace_read 1

#define GL_INTERLACE_READ_INGR									0x8568

#endif /* GL_INGR_interlace_read */

/*-------------------------------------------------------------------*/
/*------------GL_INGR_PALETTE_BUFFER---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_INGR_palette_buffer
#define GL_INGR_palette_buffer 1

#endif /* GL_INGR_palette_buffer */

/*-------------------------------------------------------------------*/
/*------------GL_INTEL_PARALLEL_ARRAYS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_INTEL_parallel_arrays
#define GL_INTEL_parallel_arrays 1

#define GL_PARALLEL_ARRAYS_INTEL								0x83F4
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL					0x83F5
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL					0x83F6
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL					0x83F7
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL			0x83F8

typedef void (APIENTRY * glVertexPointervINTELPROC) (GLint size, GLenum type, const GLvoid* *pointer);
typedef void (APIENTRY * glNormalPointervINTELPROC) (GLenum type, const GLvoid* *pointer);
typedef void (APIENTRY * glColorPointervINTELPROC) (GLint size, GLenum type, const GLvoid* *pointer);
typedef void (APIENTRY * glTexCoordPointervINTELPROC) (GLint size, GLenum type, const GLvoid* *pointer);

extern glVertexPointervINTELPROC glVertexPointervINTEL;
extern glNormalPointervINTELPROC glNormalPointervINTEL;
extern glColorPointervINTELPROC glColorPointervINTEL;
extern glTexCoordPointervINTELPROC glTexCoordPointervINTEL;

#endif /* GL_INTEL_parallel_arrays */

/*-------------------------------------------------------------------*/
/*------------GL_INTEL_TEXTURE_SCISSOR-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_INTEL_texture_scissor
#define GL_INTEL_texture_scissor 1

#endif /* GL_INTEL_texture_scissor */

/*-------------------------------------------------------------------*/
/*------------GL_KTX_BUFFER_REGION-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_KTX_buffer_region
#define GL_KTX_buffer_region 1

#define GL_KTX_FRONT_REGION										0x0
#define GL_KTX_BACK_REGION										0x1
#define GL_KTX_Z_REGION											0x2
#define GL_KTX_STENCIL_REGION									0x3

typedef GLuint (APIENTRY * glBufferRegionEnabledPROC) ( void );
typedef GLuint (APIENTRY * glNewBufferRegionPROC) ( GLenum region );
typedef void (APIENTRY * glDeleteBufferRegionPROC) ( GLenum region );
typedef void (APIENTRY * glReadBufferRegionPROC) ( GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * glDrawBufferRegionPROC) ( GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);

extern glBufferRegionEnabledPROC glBufferRegionEnabled;
extern glNewBufferRegionPROC glNewBufferRegion;
extern glDeleteBufferRegionPROC glDeleteBufferRegion;
extern glReadBufferRegionPROC glReadBufferRegion;
extern glDrawBufferRegionPROC glDrawBufferRegion;

#endif /* GL_KTX_buffer_region */

/*-------------------------------------------------------------------*/
/*------------GL_MESA_RESIZE_BUFFERS---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_MESA_resize_buffers
#define GL_MESA_resize_buffers 1

typedef void (APIENTRY * glResizeBuffersMESAPROC) (void);

extern glResizeBuffersMESAPROC glResizeBuffersMESA;

#endif /* GL_MESA_resize_buffers */

/*-------------------------------------------------------------------*/
/*------------GL_MESA_WINDOW_POS-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_MESA_window_pos
#define GL_MESA_window_pos 1

typedef void (APIENTRY * glWindowPos2dMESAPROC) (GLdouble x, GLdouble y);
typedef void (APIENTRY * glWindowPos2dvMESAPROC) (const GLdouble *v);
typedef void (APIENTRY * glWindowPos2fMESAPROC) (GLfloat x, GLfloat y);
typedef void (APIENTRY * glWindowPos2fvMESAPROC) (const GLfloat *v);
typedef void (APIENTRY * glWindowPos2iMESAPROC) (GLint x, GLint y);
typedef void (APIENTRY * glWindowPos2ivMESAPROC) (const GLint *v);
typedef void (APIENTRY * glWindowPos2sMESAPROC) (GLshort x, GLshort y);
typedef void (APIENTRY * glWindowPos2svMESAPROC) (const GLshort *v);
typedef void (APIENTRY * glWindowPos3dMESAPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glWindowPos3dvMESAPROC) (const GLdouble *v);
typedef void (APIENTRY * glWindowPos3fMESAPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glWindowPos3fvMESAPROC) (const GLfloat *v);
typedef void (APIENTRY * glWindowPos3iMESAPROC) (GLint x, GLint y, GLint z);
typedef void (APIENTRY * glWindowPos3ivMESAPROC) (const GLint *v);
typedef void (APIENTRY * glWindowPos3sMESAPROC) (GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glWindowPos3svMESAPROC) (const GLshort *v);
typedef void (APIENTRY * glWindowPos4dMESAPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glWindowPos4dvMESAPROC) (const GLdouble *v);
typedef void (APIENTRY * glWindowPos4fMESAPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glWindowPos4fvMESAPROC) (const GLfloat *v);
typedef void (APIENTRY * glWindowPos4iMESAPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRY * glWindowPos4ivMESAPROC) (const GLint *v);
typedef void (APIENTRY * glWindowPos4sMESAPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glWindowPos4svMESAPROC) (const GLshort *v);

extern glWindowPos2dMESAPROC glWindowPos2dMESA;
extern glWindowPos2dvMESAPROC glWindowPos2dvMESA;
extern glWindowPos2fMESAPROC glWindowPos2fMESA;
extern glWindowPos2fvMESAPROC glWindowPos2fvMESA;
extern glWindowPos2iMESAPROC glWindowPos2iMESA;
extern glWindowPos2ivMESAPROC glWindowPos2ivMESA;
extern glWindowPos2sMESAPROC glWindowPos2sMESA;
extern glWindowPos2svMESAPROC glWindowPos2svMESA;
extern glWindowPos3dMESAPROC glWindowPos3dMESA;
extern glWindowPos3dvMESAPROC glWindowPos3dvMESA;
extern glWindowPos3fMESAPROC glWindowPos3fMESA;
extern glWindowPos3fvMESAPROC glWindowPos3fvMESA;
extern glWindowPos3iMESAPROC glWindowPos3iMESA;
extern glWindowPos3ivMESAPROC glWindowPos3ivMESA;
extern glWindowPos3sMESAPROC glWindowPos3sMESA;
extern glWindowPos3svMESAPROC glWindowPos3svMESA;
extern glWindowPos4dMESAPROC glWindowPos4dMESA;
extern glWindowPos4dvMESAPROC glWindowPos4dvMESA;
extern glWindowPos4fMESAPROC glWindowPos4fMESA;
extern glWindowPos4fvMESAPROC glWindowPos4fvMESA;
extern glWindowPos4iMESAPROC glWindowPos4iMESA;
extern glWindowPos4ivMESAPROC glWindowPos4ivMESA;
extern glWindowPos4sMESAPROC glWindowPos4sMESA;
extern glWindowPos4svMESAPROC glWindowPos4svMESA;

#endif /* GL_MESA_window_pos */

/*-------------------------------------------------------------------*/
/*------------GL_NVX_YCRCB-------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NVX_ycrcb
#define GL_NVX_ycrcb 1

#endif /* GL_NVX_ycrcb */

/*-------------------------------------------------------------------*/
/*------------GL_NV_BLEND_SQUARE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_blend_square
#define GL_NV_blend_square 1

#endif /* GL_NV_blend_square */

/*-------------------------------------------------------------------*/
/*------------GL_NV_COPY_DEPTH_TO_COLOR------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_copy_depth_to_color
#define GL_NV_copy_depth_to_color 1

#define GL_DEPTH_STENCIL_TO_RGBA_NV                             0x886E
#define GL_DEPTH_STENCIL_TO_BGRA_NV                             0x886F

#endif /* GL_NV_copy_depth_to_color */

/*-------------------------------------------------------------------*/
/*------------GL_NV_DEPTH_CLAMP--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_depth_clamp
#define GL_NV_depth_clamp 1

#define GL_DEPTH_CLAMP_NV                                       0x864F

#endif /* GL_NV_depth_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_NV_ELEMENT_ARRAY------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_element_array
#define GL_NV_element_array 1

#define GL_ELEMENT_ARRAY_TYPE_NV                                0x8769
#define GL_ELEMENT_ARRAY_POINTER_NV                             0x876A

typedef void (APIENTRY * glElementPointerNVPROC) (GLenum type, const GLvoid *pointer);
typedef void (APIENTRY * glDrawElementArrayNVPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * glDrawRangeElementArrayNVPROC) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void (APIENTRY * glMultiDrawElementArrayNVPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * glMultiDrawRangeElementArrayNVPROC) (GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);

extern glElementPointerNVPROC glElementPointerNV;
extern glDrawElementArrayNVPROC glDrawElementArrayNV;
extern glDrawRangeElementArrayNVPROC glDrawRangeElementArrayNV;
extern glMultiDrawElementArrayNVPROC glMultiDrawElementArrayNV;
extern glMultiDrawRangeElementArrayNVPROC glMultiDrawRangeElementArrayNV;

#endif /* GL_NV_element_array */

/*-------------------------------------------------------------------*/
/*------------GL_NV_EVALUATORS---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_evaluators
#define GL_NV_evaluators 1

#define GL_EVAL_2D_NV                                           0x86C0
#define GL_EVAL_TRIANGULAR_2D_NV                                0x86C1
#define GL_MAP_TESSELLATION_NV                                  0x86C2
#define GL_MAP_ATTRIB_U_ORDER_NV                                0x86C3
#define GL_MAP_ATTRIB_V_ORDER_NV                                0x86C4
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV                      0x86C5
#define GL_EVAL_VERTEX_ATTRIB0_NV                               0x86C6
#define GL_EVAL_VERTEX_ATTRIB1_NV                               0x86C7
#define GL_EVAL_VERTEX_ATTRIB2_NV                               0x86C8
#define GL_EVAL_VERTEX_ATTRIB3_NV                               0x86C9
#define GL_EVAL_VERTEX_ATTRIB4_NV                               0x86CA
#define GL_EVAL_VERTEX_ATTRIB5_NV                               0x86CB
#define GL_EVAL_VERTEX_ATTRIB6_NV                               0x86CC
#define GL_EVAL_VERTEX_ATTRIB7_NV                               0x86CD
#define GL_EVAL_VERTEX_ATTRIB8_NV                               0x86CE
#define GL_EVAL_VERTEX_ATTRIB9_NV                               0x86CF
#define GL_EVAL_VERTEX_ATTRIB10_NV                              0x86D0
#define GL_EVAL_VERTEX_ATTRIB11_NV                              0x86D1
#define GL_EVAL_VERTEX_ATTRIB12_NV                              0x86D2
#define GL_EVAL_VERTEX_ATTRIB13_NV                              0x86D3
#define GL_EVAL_VERTEX_ATTRIB14_NV                              0x86D4
#define GL_EVAL_VERTEX_ATTRIB15_NV                              0x86D5
#define GL_MAX_MAP_TESSELLATION_NV                              0x86D6
#define GL_MAX_RATIONAL_EVAL_ORDER_NV                           0x86D7

typedef void (APIENTRY * glMapControlPointsNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
typedef void (APIENTRY * glMapParameterivNVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glMapParameterfvNVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glGetMapControlPointsNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
typedef void (APIENTRY * glGetMapParameterivNVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetMapParameterfvNVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetMapAttribParameterivNVPROC) (GLenum target, GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetMapAttribParameterfvNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glEvalMapsNVPROC) (GLenum target, GLenum mode);

extern glMapControlPointsNVPROC glMapControlPointsNV;
extern glMapParameterivNVPROC glMapParameterivNV;
extern glMapParameterfvNVPROC glMapParameterfvNV;
extern glGetMapControlPointsNVPROC glGetMapControlPointsNV;
extern glGetMapParameterivNVPROC glGetMapParameterivNV;
extern glGetMapParameterfvNVPROC glGetMapParameterfvNV;
extern glGetMapAttribParameterivNVPROC glGetMapAttribParameterivNV;
extern glGetMapAttribParameterfvNVPROC glGetMapAttribParameterfvNV;
extern glEvalMapsNVPROC glEvalMapsNV;

#endif /* GL_NV_evaluators */

/*-------------------------------------------------------------------*/
/*------------GL_NV_HALF_FLOAT---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_half_float
#define GL_NV_half_float 1

#define GL_HALF_FLOAT_NV                                        0x140B

typedef void (APIENTRY * glVertex2hNVPROC) (GLhalf x, GLhalf y);
typedef void (APIENTRY * glVertex2hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glVertex3hNVPROC) (GLhalf x, GLhalf y, GLhalf z);
typedef void (APIENTRY * glVertex3hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glVertex4hNVPROC) (GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (APIENTRY * glVertex4hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glNormal3hNVPROC) (GLhalf nx, GLhalf ny, GLhalf nz);
typedef void (APIENTRY * glNormal3hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glColor3hNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (APIENTRY * glColor3hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glColor4hNVPROC) (GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
typedef void (APIENTRY * glColor4hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glTexCoord1hNVPROC) (GLhalf s);
typedef void (APIENTRY * glTexCoord1hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glTexCoord2hNVPROC) (GLhalf s, GLhalf t);
typedef void (APIENTRY * glTexCoord2hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glTexCoord3hNVPROC) (GLhalf s, GLhalf t, GLhalf r);
typedef void (APIENTRY * glTexCoord3hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glTexCoord4hNVPROC) (GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (APIENTRY * glTexCoord4hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glMultiTexCoord1hNVPROC) (GLenum target, GLhalf s);
typedef void (APIENTRY * glMultiTexCoord1hvNVPROC) (GLenum target, const GLhalf *v);
typedef void (APIENTRY * glMultiTexCoord2hNVPROC) (GLenum target, GLhalf s, GLhalf t);
typedef void (APIENTRY * glMultiTexCoord2hvNVPROC) (GLenum target, const GLhalf *v);
typedef void (APIENTRY * glMultiTexCoord3hNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r);
typedef void (APIENTRY * glMultiTexCoord3hvNVPROC) (GLenum target, const GLhalf *v);
typedef void (APIENTRY * glMultiTexCoord4hNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (APIENTRY * glMultiTexCoord4hvNVPROC) (GLenum target, const GLhalf *v);
typedef void (APIENTRY * glFogCoordhNVPROC) (GLhalf fog);
typedef void (APIENTRY * glFogCoordhvNVPROC) (const GLhalf *fog);
typedef void (APIENTRY * glSecondaryColor3hNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (APIENTRY * glSecondaryColor3hvNVPROC) (const GLhalf *v);
typedef void (APIENTRY * glVertexWeighthNVPROC) (GLhalf weight);
typedef void (APIENTRY * glVertexWeighthvNVPROC) (const GLhalf *weight);
typedef void (APIENTRY * glVertexAttrib1hNVPROC) (GLuint index, GLhalf x);
typedef void (APIENTRY * glVertexAttrib1hvNVPROC) (GLuint index, const GLhalf *v);
typedef void (APIENTRY * glVertexAttrib2hNVPROC) (GLuint index, GLhalf x, GLhalf y);
typedef void (APIENTRY * glVertexAttrib2hvNVPROC) (GLuint index, const GLhalf *v);
typedef void (APIENTRY * glVertexAttrib3hNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z);
typedef void (APIENTRY * glVertexAttrib3hvNVPROC) (GLuint index, const GLhalf *v);
typedef void (APIENTRY * glVertexAttrib4hNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (APIENTRY * glVertexAttrib4hvNVPROC) (GLuint index, const GLhalf *v);
typedef void (APIENTRY * glVertexAttribs1hvNVPROC) (GLuint index, GLsizei n, const GLhalf *v);
typedef void (APIENTRY * glVertexAttribs2hvNVPROC) (GLuint index, GLsizei n, const GLhalf *v);
typedef void (APIENTRY * glVertexAttribs3hvNVPROC) (GLuint index, GLsizei n, const GLhalf *v);
typedef void (APIENTRY * glVertexAttribs4hvNVPROC) (GLuint index, GLsizei n, const GLhalf *v);

extern glVertex2hNVPROC glVertex2hNV;
extern glVertex2hvNVPROC glVertex2hvNV;
extern glVertex3hNVPROC glVertex3hNV;
extern glVertex3hvNVPROC glVertex3hvNV;
extern glVertex4hNVPROC glVertex4hNV;
extern glVertex4hvNVPROC glVertex4hvNV;
extern glNormal3hNVPROC glNormal3hNV;
extern glNormal3hvNVPROC glNormal3hvNV;
extern glColor3hNVPROC glColor3hNV;
extern glColor3hvNVPROC glColor3hvNV;
extern glColor4hNVPROC glColor4hNV;
extern glColor4hvNVPROC glColor4hvNV;
extern glTexCoord1hNVPROC glTexCoord1hNV;
extern glTexCoord1hvNVPROC glTexCoord1hvNV;
extern glTexCoord2hNVPROC glTexCoord2hNV;
extern glTexCoord2hvNVPROC glTexCoord2hvNV;
extern glTexCoord3hNVPROC glTexCoord3hNV;
extern glTexCoord3hvNVPROC glTexCoord3hvNV;
extern glTexCoord4hNVPROC glTexCoord4hNV;
extern glTexCoord4hvNVPROC glTexCoord4hvNV;
extern glMultiTexCoord1hNVPROC glMultiTexCoord1hNV;
extern glMultiTexCoord1hvNVPROC glMultiTexCoord1hvNV;
extern glMultiTexCoord2hNVPROC glMultiTexCoord2hNV;
extern glMultiTexCoord2hvNVPROC glMultiTexCoord2hvNV;
extern glMultiTexCoord3hNVPROC glMultiTexCoord3hNV;
extern glMultiTexCoord3hvNVPROC glMultiTexCoord3hvNV;
extern glMultiTexCoord4hNVPROC glMultiTexCoord4hNV;
extern glMultiTexCoord4hvNVPROC glMultiTexCoord4hvNV;
extern glFogCoordhNVPROC glFogCoordhNV;
extern glFogCoordhvNVPROC glFogCoordhvNV;
extern glSecondaryColor3hNVPROC glSecondaryColor3hNV;
extern glSecondaryColor3hvNVPROC glSecondaryColor3hvNV;
extern glVertexWeighthNVPROC glVertexWeighthNV;
extern glVertexWeighthvNVPROC glVertexWeighthvNV;
extern glVertexAttrib1hNVPROC glVertexAttrib1hNV;
extern glVertexAttrib1hvNVPROC glVertexAttrib1hvNV;
extern glVertexAttrib2hNVPROC glVertexAttrib2hNV;
extern glVertexAttrib2hvNVPROC glVertexAttrib2hvNV;
extern glVertexAttrib3hNVPROC glVertexAttrib3hNV;
extern glVertexAttrib3hvNVPROC glVertexAttrib3hvNV;
extern glVertexAttrib4hNVPROC glVertexAttrib4hNV;
extern glVertexAttrib4hvNVPROC glVertexAttrib4hvNV;
extern glVertexAttribs1hvNVPROC glVertexAttribs1hvNV;
extern glVertexAttribs2hvNVPROC glVertexAttribs2hvNV;
extern glVertexAttribs3hvNVPROC glVertexAttribs3hvNV;
extern glVertexAttribs4hvNVPROC glVertexAttribs4hvNV;


#endif /* GL_NV_half_float */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FENCE--------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_fence
#define GL_NV_fence 1

#define GL_ALL_COMPLETED_NV                                     0x84F2
#define GL_FENCE_STATUS_NV                                      0x84F3
#define GL_FENCE_CONDITION_NV                                   0x84F4

typedef void (APIENTRY * glGenFencesNVPROC) (GLsizei n, GLuint *fences);
typedef void (APIENTRY * glDeleteFencesNVPROC) (GLsizei n, const GLuint *fences);
typedef void (APIENTRY * glSetFenceNVPROC) (GLuint fence, GLenum condition);
typedef GLboolean (APIENTRY * glTestFenceNVPROC) (GLuint fence);
typedef void (APIENTRY * glFinishFenceNVPROC) (GLuint fence);
typedef GLboolean (APIENTRY * glIsFenceNVPROC) (GLuint fence);
typedef void (APIENTRY * glGetFenceivNVPROC) (GLuint fence, GLenum pname, GLint *params);

extern glGenFencesNVPROC glGenFencesNV;
extern glDeleteFencesNVPROC glDeleteFencesNV;
extern glSetFenceNVPROC glSetFenceNV;
extern glTestFenceNVPROC glTestFenceNV;
extern glFinishFenceNVPROC glFinishFenceNV;
extern glIsFenceNVPROC glIsFenceNV;
extern glGetFenceivNVPROC glGetFenceivNV;

#endif /* GL_NV_fence */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FLOAT_BUFFER-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_float_buffer
#define GL_NV_float_buffer 1

#define GL_FLOAT_R_NV                                           0x8880
#define GL_FLOAT_RG_NV                                          0x8881
#define GL_FLOAT_RGB_NV                                         0x8882
#define GL_FLOAT_RGBA_NV                                        0x8883
#define GL_FLOAT_R32_NV                                         0x8885
#define GL_FLOAT_R16_NV                                         0x8884
#define GL_FLOAT_R32_NV                                         0x8885
#define GL_FLOAT_RG16_NV                                        0x8886
#define GL_FLOAT_RG32_NV                                        0x8887
#define GL_FLOAT_RGB16_NV                                       0x8888
#define GL_FLOAT_RGB32_NV                                       0x8889
#define GL_FLOAT_RGBA16_NV                                      0x888A
#define GL_FLOAT_RGBA32_NV                                      0x888B
#define GL_TEXTURE_FLOAT_COMPONENTS_NV                          0x888C
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV                           0x888D
#define GL_FLOAT_RGBA_MODE_NV                                   0x888E

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#define WGL_FLOAT_COMPONENTS_NV                                 0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV                0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV               0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV              0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV             0x20B4
#define WGL_TEXTURE_FLOAT_R_NV                                  0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV                                 0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV                                0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV                               0x20B8
#endif /* _WIN32 */

#endif /* GL_NV_float_buffer */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FOG_DISTANCE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_fog_distance
#define GL_NV_fog_distance 1

#define GL_FOG_DISTANCE_MODE_NV                                 0x855A
#define GL_EYE_RADIAL_NV                                        0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV                                0x855C

#endif /* GL_NV_fog_distance */

/*-------------------------------------------------------------------*/
/*------------GL_NV_FRAGMENT_PROGRAM---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_fragment_program
#define GL_NV_fragment_program 1

#define GL_FRAGMENT_PROGRAM_NV                                  0x8870
#define GL_MAX_TEXTURE_COORDS_NV                                0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV                           0x8872
#define GL_FRAGMENT_PROGRAM_BINDING_NV                          0x8873
#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV             0x8868
#define GL_PROGRAM_ERROR_STRING_NV                              0x8874

typedef void (APIENTRY * glProgramNamedParameter4fNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramNamedParameter4dNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramNamedParameter4fvNVPROC) (GLuint id, GLsizei len, const GLubyte *name, const GLfloat v[]);
typedef void (APIENTRY * glProgramNamedParameter4dvNVPROC) (GLuint id, GLsizei len, const GLubyte *name, const GLdouble v[]);
typedef void (APIENTRY * glGetProgramNamedParameterfvNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
typedef void (APIENTRY * glGetProgramNamedParameterdvNVPROC) (GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);

extern glProgramNamedParameter4fNVPROC glProgramNamedParameter4fNV;
extern glProgramNamedParameter4dNVPROC glProgramNamedParameter4dNV;
extern glProgramNamedParameter4fvNVPROC glProgramNamedParameter4fvNV;
extern glProgramNamedParameter4dvNVPROC glProgramNamedParameter4dvNV;
extern glGetProgramNamedParameterfvNVPROC glGetProgramNamedParameterfvNV;
extern glGetProgramNamedParameterdvNVPROC glGetProgramNamedParameterdvNV;

#endif /* GL_NV_fragment_program */

/*-------------------------------------------------------------------*/
/*------------GL_NV_LIGHT_MAX_EXPONENT-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_light_max_exponent
#define GL_NV_light_max_exponent 1

#define GL_MAX_SHININESS_NV                                     0x8504
#define GL_MAX_SPOT_EXPONENT_NV                                 0x8505

#endif /* GL_NV_light_max_exponent */

/*-------------------------------------------------------------------*/
/*------------GL_NV_MULTISAMPLE_FILTER_HINT--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_multisample_filter_hint
#define GL_NV_multisample_filter_hint 1

#define GL_MULTISAMPLE_FILTER_HINT_NV							0x8534

#endif

/*-------------------------------------------------------------------*/
/*------------GL_NV_OCCLUSION_QUERY----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query 1

#define GL_PIXEL_COUNTER_BITS_NV                                0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV                        0x8865
#define GL_PIXEL_COUNT_NV                                       0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV                             0x8867

typedef void (APIENTRY * glGenOcclusionQueriesNVPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glDeleteOcclusionQueriesNVPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRY * glIsOcclusionQueryNVPROC) (GLuint id);
typedef void (APIENTRY * glBeginOcclusionQueryNVPROC) (GLuint id);
typedef void (APIENTRY * glEndOcclusionQueryNVPROC) (void);
typedef void (APIENTRY * glGetOcclusionQueryivNVPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetOcclusionQueryuivNVPROC) (GLuint id, GLenum pname, GLuint *params);

extern glGenOcclusionQueriesNVPROC glGenOcclusionQueriesNV;
extern glDeleteOcclusionQueriesNVPROC glDeleteOcclusionQueriesNV;
extern glIsOcclusionQueryNVPROC glIsOcclusionQueryNV;
extern glBeginOcclusionQueryNVPROC glBeginOcclusionQueryNV;
extern glEndOcclusionQueryNVPROC glEndOcclusionQueryNV;
extern glGetOcclusionQueryivNVPROC glGetOcclusionQueryivNV;
extern glGetOcclusionQueryuivNVPROC glGetOcclusionQueryuivNV;

#endif /* GL_NV_occlusion_query */

/*-------------------------------------------------------------------*/
/*------------GL_NV_PACKED_DEPTH_STENCIL-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_packed_depth_stencil
#define GL_NV_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_NV                                     0x84F9
#define GL_UNSIGNED_INT_24_8_NV                                 0x84FA

#endif /* GL_NV_packed_depth_stencil */

/*-------------------------------------------------------------------*/
/*------------GL_NV_PIXEL_DATA_RANGE---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_pixel_data_range
#define GL_NV_pixel_data_range 1
                                                            
#define GL_WRITE_PIXEL_DATA_RANGE_NV                            0x8878
#define GL_READ_PIXEL_DATA_RANGE_NV                             0x8879
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV                     0x887A
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV                      0x887B
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV                    0x887C
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV                     0x887D

typedef void (APIENTRY * glPixelDataRangeNVPROC) (GLenum target, GLsizei length, GLvoid *pointer);
typedef void (APIENTRY * glFlushPixelDataRangeNVPROC) (GLenum target);

extern glPixelDataRangeNVPROC glPixelDataRangeNV;
extern glFlushPixelDataRangeNVPROC glFlushPixelDataRangeNV;

#endif /* GL_NV_pixel_data_range */

/*-------------------------------------------------------------------*/
/*------------GL_NV_POINT_SPRITE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_point_sprite
#define GL_NV_point_sprite 1

#define GL_POINT_SPRITE_NV                                      0x8861
#define GL_COORD_REPLACE_NV                                     0x8862
#define GL_POINT_SPRITE_R_MODE_NV                               0x8863

typedef void (APIENTRY * glPointParameteriNVPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glPointParameterivNVPROC) (GLenum pname, const GLint *params);

extern glPointParameteriNVPROC glPointParameteriNV;
extern glPointParameterivNVPROC glPointParameterivNV;

#endif /* GL_NV_point_sprite */

/*-------------------------------------------------------------------*/
/*------------GL_NV_PRIMITIVE_RESTART--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_primitive_restart
#define GL_NV_primitive_restart 1

#define GL_PRIMITIVE_RESTART_NV                                 0x8558
#define GL_PRIMITIVE_RESTART_INDEX_NV                           0x8559

typedef void (APIENTRY * glPrimitiveRestartNVPROC) ();
typedef void (APIENTRY * glPrimitiveRestartIndexNVPROC) (GLuint index);

extern glPrimitiveRestartNVPROC glPrimitiveRestartNV;
extern glPrimitiveRestartIndexNVPROC glPrimitiveRestartIndexNV;

#endif /* GL_NV_primitive_restart */

/*-------------------------------------------------------------------*/
/*------------GL_NV_REGISTER_COMBINERS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_register_combiners
#define GL_NV_register_combiners 1

#define GL_REGISTER_COMBINERS_NV                                0x8522
#define GL_COMBINER0_NV                                         0x8550
#define GL_COMBINER1_NV                                         0x8551
#define GL_COMBINER2_NV                                         0x8552
#define GL_COMBINER3_NV                                         0x8553
#define GL_COMBINER4_NV                                         0x8554
#define GL_COMBINER5_NV                                         0x8555
#define GL_COMBINER6_NV                                         0x8556
#define GL_COMBINER7_NV                                         0x8557
#define GL_VARIABLE_A_NV                                        0x8523
#define GL_VARIABLE_B_NV                                        0x8524
#define GL_VARIABLE_C_NV                                        0x8525
#define GL_VARIABLE_D_NV                                        0x8526
#define GL_VARIABLE_E_NV                                        0x8527
#define GL_VARIABLE_F_NV                                        0x8528
#define GL_VARIABLE_G_NV                                        0x8529
#define GL_CONSTANT_COLOR0_NV                                   0x852A
#define GL_CONSTANT_COLOR1_NV                                   0x852B
#define GL_PRIMARY_COLOR_NV                                     0x852C
#define GL_SECONDARY_COLOR_NV                                   0x852D
#define GL_SPARE0_NV                                            0x852E
#define GL_SPARE1_NV                                            0x852F
#define GL_UNSIGNED_IDENTITY_NV                                 0x8536
#define GL_UNSIGNED_INVERT_NV                                   0x8537
#define GL_EXPAND_NORMAL_NV                                     0x8538
#define GL_EXPAND_NEGATE_NV                                     0x8539
#define GL_HALF_BIAS_NORMAL_NV                                  0x853A
#define GL_HALF_BIAS_NEGATE_NV                                  0x853B
#define GL_SIGNED_IDENTITY_NV                                   0x853C
#define GL_SIGNED_NEGATE_NV                                     0x853D
#define GL_E_TIMES_F_NV                                         0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV                       0x8532
#define GL_SCALE_BY_TWO_NV                                      0x853E
#define GL_SCALE_BY_FOUR_NV                                     0x853F
#define GL_SCALE_BY_ONE_HALF_NV                                 0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV                         0x8541
#define GL_DISCARD_NV                                           0x8530
#define GL_COMBINER_INPUT_NV                                    0x8542
#define GL_COMBINER_MAPPING_NV                                  0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV                          0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV                           0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV                           0x8546
#define GL_COMBINER_MUX_SUM_NV                                  0x8547
#define GL_COMBINER_SCALE_NV                                    0x8548
#define GL_COMBINER_BIAS_NV                                     0x8549
#define GL_COMBINER_AB_OUTPUT_NV                                0x854A
#define GL_COMBINER_CD_OUTPUT_NV                                0x854B
#define GL_COMBINER_SUM_OUTPUT_NV                               0x854C
#define GL_NUM_GENERAL_COMBINERS_NV                             0x854E
#define GL_COLOR_SUM_CLAMP_NV                                   0x854F
#define GL_MAX_GENERAL_COMBINERS_NV                             0x854D

typedef void (APIENTRY * glCombinerParameterfvNVPROC) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glCombinerParameterfNVPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glCombinerParameterivNVPROC) (GLenum pname, const GLint *params);
typedef void (APIENTRY * glCombinerParameteriNVPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glCombinerInputNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRY * glCombinerOutputNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (APIENTRY * glFinalCombinerInputNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (APIENTRY * glGetCombinerInputParameterfvNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetCombinerInputParameterivNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetCombinerOutputParameterfvNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetCombinerOutputParameterivNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetFinalCombinerInputParameterfvNVPROC) (GLenum variable, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetFinalCombinerInputParameterivNVPROC) (GLenum variable, GLenum pname, GLint *params);

extern glCombinerParameterfvNVPROC glCombinerParameterfvNV;
extern glCombinerParameterfNVPROC  glCombinerParameterfNV;
extern glCombinerParameterivNVPROC glCombinerParameterivNV;
extern glCombinerParameteriNVPROC glCombinerParameteriNV;
extern glCombinerInputNVPROC glCombinerInputNV;
extern glCombinerOutputNVPROC glCombinerOutputNV;
extern glFinalCombinerInputNVPROC glFinalCombinerInputNV;
extern glGetCombinerInputParameterfvNVPROC glGetCombinerInputParameterfvNV;
extern glGetCombinerInputParameterivNVPROC glGetCombinerInputParameterivNV;
extern glGetCombinerOutputParameterfvNVPROC glGetCombinerOutputParameterfvNV;
extern glGetCombinerOutputParameterivNVPROC glGetCombinerOutputParameterivNV;
extern glGetFinalCombinerInputParameterfvNVPROC glGetFinalCombinerInputParameterfvNV;
extern glGetFinalCombinerInputParameterivNVPROC glGetFinalCombinerInputParameterivNV;

#endif /* GL_NV_register_combiners */

/*-------------------------------------------------------------------*/
/*------------GL_NV_REGISTER_COMBINERS2------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_register_combiners2
#define GL_NV_register_combiners2

#define GL_PER_STAGE_CONSTANTS_NV                               0x8535

typedef void (APIENTRY * glCombinerStageParameterfvNVPROC) (GLenum stage, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glGetCombinerStageParameterfvNVPROC) (GLenum stage, GLenum pname, GLfloat *params);

extern glCombinerStageParameterfvNVPROC glCombinerStageParameterfvNV;
extern glGetCombinerStageParameterfvNVPROC glGetCombinerStageParameterfvNV;

#endif /* GL_NV_register_combiners2 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXGEN_EMBOSS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texgen_emboss
#define GL_NV_texgen_emboss

#define GL_EMBOSS_LIGHT_NV										0x855D
#define GL_EMBOSS_CONSTANT_NV									0x855E
#define GL_EMBOSS_MAP_NV										0x855F

#endif /* GL_NV_texgen_emboss */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXGEN_REFLECTION--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection 1

#define GL_NORMAL_MAP_NV                                        0x8511
#define GL_REFLECTION_MAP_NV                                    0x8512

#endif /* GL_NV_texgen_reflection */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_COMPRESSION_VTC--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_compression_vtc
#define GL_NV_texture_compression_vtc 1

#endif /* GL_NV_texture_compression_vtc */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_ENV_COMBINE4-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_env_combine4
#define GL_NV_texture_env_combine4 1

#define GL_COMBINE4_NV                                          0x8503
#define GL_SOURCE3_RGB_NV                                       0x8583
#define GL_SOURCE3_ALPHA_NV                                     0x858B
#define GL_OPERAND3_RGB_NV                                      0x8593
#define GL_OPERAND3_ALPHA_NV                                    0x859B

#endif /* GL_NV_texture_env_combine4 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_EXPAND_NORMAL----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_expand_normal
#define GL_NV_texture_expand_normal 1

#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV                       0x888F

#endif /* GL_NV_texture_expand_normal */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_RECTANGLE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_NV                                 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV                         0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV                           0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV                        0x84F8

#endif /* GL_NV_texture_rectangle */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_SHADER-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader 1

#define GL_OFFSET_TEXTURE_RECTANGLE_NV                          0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV                    0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV                     0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV                 0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV                            0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV                        0x86DB
#define GL_DSDT_MAG_INTENSITY_NV                                0x86DC
#define GL_SHADER_CONSISTENT_NV                                 0x86DD
#define GL_TEXTURE_SHADER_NV                                    0x86DE
#define GL_SHADER_OPERATION_NV                                  0x86DF
#define GL_CULL_MODES_NV                                        0x86E0
#define GL_OFFSET_TEXTURE_MATRIX_NV                             0x86E1
#define GL_OFFSET_TEXTURE_SCALE_NV                              0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV                               0x86E3
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV                          GL_OFFSET_TEXTURE_MATRIX_NV
#define GL_OFFSET_TEXTURE_2D_SCALE_NV                           GL_OFFSET_TEXTURE_SCALE_NV
#define GL_OFFSET_TEXTURE_2D_BIAS_NV                            GL_OFFSET_TEXTURE_BIAS_NV
#define GL_PREVIOUS_TEXTURE_INPUT_NV                            0x86E4
#define GL_CONST_EYE_NV                                         0x86E5
#define GL_PASS_THROUGH_NV                                      0x86E6
#define GL_CULL_FRAGMENT_NV                                     0x86E7
#define GL_OFFSET_TEXTURE_2D_NV                                 0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV                           0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV                           0x86EA
#define GL_DOT_PRODUCT_NV                                       0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV                         0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV                            0x86EE
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV                      0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV                      0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV                      0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV            0x86F3
#define GL_HILO_NV                                              0x86F4
#define GL_DSDT_NV                                              0x86F5
#define GL_DSDT_MAG_NV                                          0x86F6
#define GL_DSDT_MAG_VIB_NV                                      0x86F7
#define GL_HILO16_NV                                            0x86F8
#define GL_SIGNED_HILO_NV                                       0x86F9
#define GL_SIGNED_HILO16_NV                                     0x86FA
#define GL_SIGNED_RGBA_NV                                       0x86FB
#define GL_SIGNED_RGBA8_NV                                      0x86FC
#define GL_SIGNED_RGB_NV                                        0x86FE
#define GL_SIGNED_RGB8_NV                                       0x86FF
#define GL_SIGNED_LUMINANCE_NV                                  0x8701
#define GL_SIGNED_LUMINANCE8_NV                                 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV                            0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV                          0x8704
#define GL_SIGNED_ALPHA_NV                                      0x8705
#define GL_SIGNED_ALPHA8_NV                                     0x8706
#define GL_SIGNED_INTENSITY_NV                                  0x8707
#define GL_SIGNED_INTENSITY8_NV                                 0x8708
#define GL_DSDT8_NV                                             0x8709
#define GL_DSDT8_MAG8_NV                                        0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV                             0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV                         0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV                       0x870D
#define GL_HI_SCALE_NV                                          0x870E
#define GL_LO_SCALE_NV                                          0x870F
#define GL_DS_SCALE_NV                                          0x8710
#define GL_DT_SCALE_NV                                          0x8711
#define GL_MAGNITUDE_SCALE_NV                                   0x8712
#define GL_VIBRANCE_SCALE_NV                                    0x8713
#define GL_HI_BIAS_NV                                           0x8714
#define GL_LO_BIAS_NV                                           0x8715
#define GL_DS_BIAS_NV                                           0x8716
#define GL_DT_BIAS_NV                                           0x8717
#define GL_MAGNITUDE_BIAS_NV                                    0x8718
#define GL_VIBRANCE_BIAS_NV                                     0x8719
#define GL_TEXTURE_BORDER_VALUES_NV                             0x871A
#define GL_TEXTURE_HI_SIZE_NV                                   0x871B
#define GL_TEXTURE_LO_SIZE_NV                                   0x871C
#define GL_TEXTURE_DS_SIZE_NV                                   0x871D
#define GL_TEXTURE_DT_SIZE_NV                                   0x871E
#define GL_TEXTURE_MAG_SIZE_NV                                  0x871F

#endif /* GL_NV_texture_shader */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_SHADER2----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_shader2
#define GL_NV_texture_shader2

#define GL_DOT_PRODUCT_TEXTURE_3D_NV                            0x86EF
#define GL_HILO_NV                                              0x86F4
#define GL_DSDT_NV                                              0x86F5
#define GL_DSDT_MAG_NV                                          0x86F6
#define GL_DSDT_MAG_VIB_NV                                      0x86F7
#define GL_UNSIGNED_INT_S8_S8_8_8_NV                            0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV                        0x86DB
#define GL_SIGNED_RGBA_NV                                       0x86FB
#define GL_SIGNED_RGBA8_NV                                      0x86FC
#define GL_SIGNED_RGB_NV                                        0x86FE
#define GL_SIGNED_RGB8_NV                                       0x86FF
#define GL_SIGNED_LUMINANCE_NV                                  0x8701
#define GL_SIGNED_LUMINANCE8_NV                                 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV                            0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV                          0x8704
#define GL_SIGNED_ALPHA_NV                                      0x8705
#define GL_SIGNED_ALPHA8_NV                                     0x8706
#define GL_SIGNED_INTENSITY_NV                                  0x8707
#define GL_SIGNED_INTENSITY8_NV                                 0x8708
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV                         0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV                       0x870D
#define GL_HILO16_NV                                            0x86F8
#define GL_SIGNED_HILO_NV                                       0x86F9
#define GL_SIGNED_HILO16_NV                                     0x86FA
#define GL_DSDT8_NV                                             0x8709
#define GL_DSDT8_MAG8_NV                                        0x870A
#define GL_DSDT_MAG_INTENSITY_NV                                0x86DC
#define GL_DSDT8_MAG8_INTENSITY8_NV                             0x870B 

#endif /* GL_NV_texture_shader2 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_TEXTURE_SHADER3----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_texture_shader3
#define GL_NV_texture_shader3 1

#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV                      0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV                0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV               0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV         0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV                            0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV                     0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV                 0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV          0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV                         0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV                          0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV                    0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV                          0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV                            0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV                  0x885D
#define GL_HILO8_NV                                             0x885E
#define GL_SIGNED_HILO8_NV                                      0x885F
#define GL_FORCE_BLUE_TO_ONE_NV                                 0x8860

#endif /* GL_NV_texture_shader3 */

/*-------------------------------------------------------------------*/
/*------------NV_VERTEX_ARRAY_RANGE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_array_range
#define GL_NV_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_NV                                0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV                         0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV                          0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV                    0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV                        0x8521

typedef void (APIENTRY * glFlushVertexArrayRangeNVPROC) (void);
typedef void (APIENTRY * glVertexArrayRangeNVPROC) (GLsizei size, const GLvoid *pointer);

extern glFlushVertexArrayRangeNVPROC glFlushVertexArrayRangeNV;
extern glVertexArrayRangeNVPROC glVertexArrayRangeNV;

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

typedef void * (APIENTRY * wglAllocateMemoryNVPROC) (GLsizei size, GLfloat readFrequency, GLfloat writeFrequency, GLfloat priority);
typedef void (APIENTRY * wglFreeMemoryNVPROC) (void *pointer);

extern wglAllocateMemoryNVPROC wglAllocateMemoryNV;
extern wglFreeMemoryNVPROC wglFreeMemoryNV;

#else

typedef void * (APIENTRY * glXAllocateMemoryNVPROC) (GLsizei size, GLfloat readFrequency, GLfloat writeFrequency, GLfloat priority);
typedef void (APIENTRY * glXFreeMemoryNVPROC) (void *pointer);

extern glXAllocateMemoryNVPROC glXAllocateMemoryNV;
extern glXFreeMemoryNVPROC glXFreeMemoryNV;

#endif /* WIN32 */

#endif /* GL_NV_vertex_array_range */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_ARRAY_RANGE2------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_array_range2
#define GL_NV_vertex_array_range2 1

#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV                  0x8533

#endif /* GL_NV_vertex_array_range2 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_PROGRAM-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_program
#define GL_NV_vertex_program 1

#define GL_VERTEX_PROGRAM_NV                                    0x8620
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV                         0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV                           0x8643
#define GL_VERTEX_STATE_PROGRAM_NV                              0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV                                 0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV                               0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV                                 0x8625
#define GL_CURRENT_ATTRIB_NV                                    0x8626
#define GL_PROGRAM_PARAMETER_NV                                 0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV                              0x8645
#define GL_PROGRAM_TARGET_NV                                    0x8646
#define GL_PROGRAM_LENGTH_NV                                    0x8627
#define GL_PROGRAM_RESIDENT_NV                                  0x8647
#define GL_PROGRAM_STRING_NV                                    0x8628
#define GL_TRACK_MATRIX_NV                                      0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV                            0x8649
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV                      0x862E
#define GL_MAX_TRACK_MATRICES_NV                                0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV                        0x8640
#define GL_CURRENT_MATRIX_NV                                    0x8641
#define GL_VERTEX_PROGRAM_BINDING_NV                            0x864A
#define GL_PROGRAM_ERROR_POSITION_NV                            0x864B
#define GL_MODELVIEW_PROJECTION_NV                              0x8629
#define GL_MATRIX0_NV                                           0x8630
#define GL_MATRIX1_NV                                           0x8631
#define GL_MATRIX2_NV                                           0x8632
#define GL_MATRIX3_NV                                           0x8633
#define GL_MATRIX4_NV                                           0x8634
#define GL_MATRIX5_NV                                           0x8635
#define GL_MATRIX6_NV                                           0x8636
#define GL_MATRIX7_NV                                           0x8637
#define GL_IDENTITY_NV                                          0x862A
#define GL_INVERSE_NV                                           0x862B
#define GL_TRANSPOSE_NV                                         0x862C
#define GL_INVERSE_TRANSPOSE_NV                                 0x862D
#define GL_VERTEX_ATTRIB_ARRAY0_NV                              0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV                              0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV                              0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV                              0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV                              0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV                              0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV                              0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV                              0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV                              0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV                              0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV                             0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV                             0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV                             0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV                             0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV                             0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV                             0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV                             0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV                             0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV                             0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV                             0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV                             0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV                             0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV                             0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV                             0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV                             0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV                             0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV                            0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV                            0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV                            0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV                            0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV                            0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV                            0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV                             0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV                             0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV                             0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV                             0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV                             0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV                             0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV                             0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV                             0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV                             0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV                             0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV                            0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV                            0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV                            0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV                            0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV                            0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV                            0x867F

typedef void (APIENTRY * glBindProgramNVPROC) (GLenum target, GLuint id);
typedef void (APIENTRY * glDeleteProgramsNVPROC) (GLsizei n, const GLuint *ids);
typedef void (APIENTRY * glExecuteProgramNVPROC) (GLenum target, GLuint id, const GLfloat *params);
typedef void (APIENTRY * glGenProgramsNVPROC) (GLsizei n, GLuint *ids);
typedef GLboolean (APIENTRY * glAreProgramsResidentNVPROC) (GLsizei n, const GLuint *ids, GLboolean *residences);
typedef void (APIENTRY * glRequestResidentProgramsNVPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glGetProgramParameterfvNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetProgramParameterdvNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRY * glGetProgramivNVPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetProgramStringNVPROC) (GLuint id, GLenum pname, GLubyte *program);
typedef void (APIENTRY * glGetTrackMatrixivNVPROC) (GLenum target, GLuint address, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetVertexAttribdvNVPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRY * glGetVertexAttribfvNVPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVertexAttribivNVPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetVertexAttribPointervNVPROC) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (APIENTRY * glIsProgramNVPROC) (GLuint id);
typedef void (APIENTRY * glLoadProgramNVPROC) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
typedef void (APIENTRY * glProgramParameter4fNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramParameter4dNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramParameter4dvNVPROC) (GLenum target, GLuint index, const GLdouble *params);
typedef void (APIENTRY * glProgramParameter4fvNVPROC) (GLenum target, GLuint index, const GLfloat *params);
typedef void (APIENTRY * glProgramParameters4dvNVPROC) (GLenum target, GLuint index, GLuint num, const GLdouble *params);
typedef void (APIENTRY * glProgramParameters4fvNVPROC) (GLenum target, GLuint index, GLuint num, const GLfloat *params);
typedef void (APIENTRY * glTrackMatrixNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (APIENTRY * glVertexAttribPointerNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * glVertexAttrib1sNVPROC) (GLuint index, GLshort x);
typedef void (APIENTRY * glVertexAttrib1fNVPROC) (GLuint index, GLfloat x);
typedef void (APIENTRY * glVertexAttrib1dNVPROC) (GLuint index, GLdouble x);
typedef void (APIENTRY * glVertexAttrib2sNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void (APIENTRY * glVertexAttrib2fNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRY * glVertexAttrib2dNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRY * glVertexAttrib3sNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glVertexAttrib3fNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glVertexAttrib3dNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glVertexAttrib4sNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glVertexAttrib4fNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glVertexAttrib4dNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glVertexAttrib4ubNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (APIENTRY * glVertexAttrib1svNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib1fvNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib1dvNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib2svNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib2fvNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib2dvNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib3svNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib3fvNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib3dvNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4svNVPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4fvNVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib4dvNVPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4ubvNVPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttribs1svNVPROC) (GLuint index, GLsizei n, const GLshort *v);
typedef void (APIENTRY * glVertexAttribs1fvNVPROC) (GLuint index, GLsizei n, const GLfloat *v);
typedef void (APIENTRY * glVertexAttribs1dvNVPROC) (GLuint index, GLsizei n, const GLdouble *v);
typedef void (APIENTRY * glVertexAttribs2svNVPROC) (GLuint index, GLsizei n, const GLshort *v);
typedef void (APIENTRY * glVertexAttribs2fvNVPROC) (GLuint index, GLsizei n, const GLfloat *v);
typedef void (APIENTRY * glVertexAttribs2dvNVPROC) (GLuint index, GLsizei n, const GLdouble *v);
typedef void (APIENTRY * glVertexAttribs3svNVPROC) (GLuint index, GLsizei n, const GLshort *v);
typedef void (APIENTRY * glVertexAttribs3fvNVPROC) (GLuint index, GLsizei n, const GLfloat *v);
typedef void (APIENTRY * glVertexAttribs3dvNVPROC) (GLuint index, GLsizei n, const GLdouble *v);
typedef void (APIENTRY * glVertexAttribs4svNVPROC) (GLuint index, GLsizei n, const GLshort *v);
typedef void (APIENTRY * glVertexAttribs4fvNVPROC) (GLuint index, GLsizei n, const GLfloat *v);
typedef void (APIENTRY * glVertexAttribs4dvNVPROC) (GLuint index, GLsizei n, const GLdouble *v);
typedef void (APIENTRY * glVertexAttribs4ubvNVPROC) (GLuint index, GLsizei n, const GLubyte *v);

extern glBindProgramNVPROC glBindProgramNV;
extern glDeleteProgramsNVPROC glDeleteProgramsNV;
extern glExecuteProgramNVPROC glExecuteProgramNV;
extern glGenProgramsNVPROC glGenProgramsNV;
extern glAreProgramsResidentNVPROC glAreProgramsResidentNV;
extern glRequestResidentProgramsNVPROC glRequestResidentProgramsNV;
extern glGetProgramParameterfvNVPROC glGetProgramParameterfvNV;
extern glGetProgramParameterdvNVPROC glGetProgramParameterdvNV;
extern glGetProgramivNVPROC glGetProgramivNV;
extern glGetProgramStringNVPROC glGetProgramStringNV;
extern glGetTrackMatrixivNVPROC glGetTrackMatrixivNV;
extern glGetVertexAttribdvNVPROC glGetVertexAttribdvNV;
extern glGetVertexAttribfvNVPROC glGetVertexAttribfvNV;
extern glGetVertexAttribivNVPROC glGetVertexAttribivNV;
extern glGetVertexAttribPointervNVPROC glGetVertexAttribPointervNV;
extern glIsProgramNVPROC glIsProgramNV;
extern glLoadProgramNVPROC glLoadProgramNV;
extern glProgramParameter4fNVPROC glProgramParameter4fNV;
extern glProgramParameter4dNVPROC glProgramParameter4dNV;
extern glProgramParameter4dvNVPROC glProgramParameter4dvNV;
extern glProgramParameter4fvNVPROC glProgramParameter4fvNV;
extern glProgramParameters4dvNVPROC glProgramParameters4dvNV;
extern glProgramParameters4fvNVPROC glProgramParameters4fvNV;
extern glTrackMatrixNVPROC glTrackMatrixNV;
extern glVertexAttribPointerNVPROC glVertexAttribPointerNV;
extern glVertexAttrib1sNVPROC glVertexAttrib1sNV;
extern glVertexAttrib1fNVPROC glVertexAttrib1fNV;
extern glVertexAttrib1dNVPROC glVertexAttrib1dNV;
extern glVertexAttrib2sNVPROC glVertexAttrib2sNV;
extern glVertexAttrib2fNVPROC glVertexAttrib2fNV;
extern glVertexAttrib2dNVPROC glVertexAttrib2dNV;
extern glVertexAttrib3sNVPROC glVertexAttrib3sNV;
extern glVertexAttrib3fNVPROC glVertexAttrib3fNV;
extern glVertexAttrib3dNVPROC glVertexAttrib3dNV;
extern glVertexAttrib4sNVPROC glVertexAttrib4sNV;
extern glVertexAttrib4fNVPROC glVertexAttrib4fNV;
extern glVertexAttrib4dNVPROC glVertexAttrib4dNV;
extern glVertexAttrib4ubNVPROC glVertexAttrib4ubNV;
extern glVertexAttrib1svNVPROC glVertexAttrib1svNV;
extern glVertexAttrib1fvNVPROC glVertexAttrib1fvNV;
extern glVertexAttrib1dvNVPROC glVertexAttrib1dvNV;
extern glVertexAttrib2svNVPROC glVertexAttrib2svNV;
extern glVertexAttrib2fvNVPROC glVertexAttrib2fvNV;
extern glVertexAttrib2dvNVPROC glVertexAttrib2dvNV;
extern glVertexAttrib3svNVPROC glVertexAttrib3svNV;
extern glVertexAttrib3fvNVPROC glVertexAttrib3fvNV;
extern glVertexAttrib3dvNVPROC glVertexAttrib3dvNV;
extern glVertexAttrib4svNVPROC glVertexAttrib4svNV;
extern glVertexAttrib4fvNVPROC glVertexAttrib4fvNV;
extern glVertexAttrib4dvNVPROC glVertexAttrib4dvNV;
extern glVertexAttrib4ubvNVPROC glVertexAttrib4ubvNV;
extern glVertexAttribs1svNVPROC glVertexAttribs1svNV;
extern glVertexAttribs1fvNVPROC glVertexAttribs1fvNV;
extern glVertexAttribs1dvNVPROC glVertexAttribs1dvNV;
extern glVertexAttribs2svNVPROC glVertexAttribs2svNV;
extern glVertexAttribs2fvNVPROC glVertexAttribs2fvNV;
extern glVertexAttribs2dvNVPROC glVertexAttribs2dvNV;
extern glVertexAttribs3svNVPROC glVertexAttribs3svNV;
extern glVertexAttribs3fvNVPROC glVertexAttribs3fvNV;
extern glVertexAttribs3dvNVPROC glVertexAttribs3dvNV;
extern glVertexAttribs4svNVPROC glVertexAttribs4svNV;
extern glVertexAttribs4fvNVPROC glVertexAttribs4fvNV;
extern glVertexAttribs4dvNVPROC glVertexAttribs4dvNV;
extern glVertexAttribs4ubvNVPROC glVertexAttribs4ubvNV;

#endif /* GL_NV_vertex_program */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_PROGRAM1_1--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_program1_1
#define GL_NV_vertex_program1_1

#endif /* GL_NV_vertex_program1_1 */

/*-------------------------------------------------------------------*/
/*------------GL_NV_VERTEX_PROGRAM2----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_NV_vertex_program2
#define GL_NV_vertex_program2 1

#endif /* GL_NV_vertex_program2 */

/*-------------------------------------------------------------------*/
/*------------GL_OML_INTERLACE---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_OML_interlace
#define GL_OML_interlace 1

#define GL_INTERLACE_OML										0x8980
#define GL_INTERLACE_READ_OML									0x8981

#endif /* GL_OML_interlace */

/*-------------------------------------------------------------------*/
/*------------GL_OML_RESAMPLE----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_OML_resample
#define GL_OML_resample 1

#define GL_PACK_RESAMPLE_OML									0x8984
#define GL_UNPACK_RESAMPLE_OML									0x8985
#define GL_RESAMPLE_REPLICATE_OML								0x8986
#define GL_RESAMPLE_ZERO_FILL_OML								0x8987
#define GL_RESAMPLE_AVERAGE_OML									0x8988
#define GL_RESAMPLE_DECIMATE_OML								0x8989

#endif /* GL_OML_resample */

/*-------------------------------------------------------------------*/
/*------------GL_OML_SUBSAMPLE---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_OML_subsample
#define GL_OML_subsample 1

#define GL_FORMAT_SUBSAMPLE_24_24_OML							0x8982
#define GL_FORMAT_SUBSAMPLE_244_244_OML							0x8983

#endif /* GL_OML_subsample */

/*-------------------------------------------------------------------*/
/*------------GL_PGI_MISC_HINTS--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_PGI_misc_hints
#define GL_PGI_misc_hints 1

#define GL_PREFER_DOUBLEBUFFER_HINT_PGI							0x1A1F8
#define GL_CONSERVE_MEMORY_HINT_PGI								0x1A1FD
#define GL_RECLAIM_MEMORY_HINT_PGI								0x1A1FE
#define GL_NATIVE_GRAPHICS_HANDLE_PGI							0x1A202
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI						0x1A203
#define GL_NATIVE_GRAPHICS_END_HINT_PGI							0x1A204
#define GL_ALWAYS_FAST_HINT_PGI									0x1A20C
#define GL_ALWAYS_SOFT_HINT_PGI									0x1A20D
#define GL_ALLOW_DRAW_OBJ_HINT_PGI								0x1A20E
#define GL_ALLOW_DRAW_WIN_HINT_PGI								0x1A20F
#define GL_ALLOW_DRAW_FRG_HINT_PGI								0x1A210
#define GL_ALLOW_DRAW_MEM_HINT_PGI								0x1A211
#define GL_STRICT_DEPTHFUNC_HINT_PGI							0x1A216
#define GL_STRICT_LIGHTING_HINT_PGI								0x1A217
#define GL_STRICT_SCISSOR_HINT_PGI								0x1A218
#define GL_FULL_STIPPLE_HINT_PGI								0x1A219
#define GL_CLIP_NEAR_HINT_PGI									0x1A220
#define GL_CLIP_FAR_HINT_PGI									0x1A221
#define GL_WIDE_LINE_HINT_PGI									0x1A222
#define GL_BACK_NORMALS_HINT_PGI								0x1A223

typedef void (APIENTRY * glHintPGIPROC) (GLenum target, GLint mode);

extern glHintPGIPROC glHintPGI;

#endif /* GL_PGI_misc_hints */

/*-------------------------------------------------------------------*/
/*------------GL_PGI_VERTEX_HINTS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_PGI_vertex_hints
#define GL_PGI_vertex_hints 1

#define GL_VERTEX_DATA_HINT_PGI									0x1A22A
#define GL_VERTEX_CONSISTENT_HINT_PGI							0x1A22B
#define GL_MATERIAL_SIDE_HINT_PGI								0x1A22C
#define GL_MAX_VERTEX_HINT_PGI									0x1A22D
#define GL_COLOR3_BIT_PGI										0x00010000
#define GL_COLOR4_BIT_PGI										0x00020000
#define GL_EDGEFLAG_BIT_PGI										0x00040000
#define GL_INDEX_BIT_PGI										0x00080000
#define GL_MAT_AMBIENT_BIT_PGI									0x00100000
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI						0x00200000
#define GL_MAT_DIFFUSE_BIT_PGI									0x00400000
#define GL_MAT_EMISSION_BIT_PGI									0x00800000
#define GL_MAT_COLOR_INDEXES_BIT_PGI							0x01000000
#define GL_MAT_SHININESS_BIT_PGI								0x02000000
#define GL_MAT_SPECULAR_BIT_PGI									0x04000000
#define GL_NORMAL_BIT_PGI										0x08000000
#define GL_TEXCOORD1_BIT_PGI									0x10000000
#define GL_TEXCOORD2_BIT_PGI									0x20000000
#define GL_TEXCOORD3_BIT_PGI									0x40000000
#define GL_TEXCOORD4_BIT_PGI									0x80000000
#define GL_VERTEX23_BIT_PGI										0x00000004
#define GL_VERTEX4_BIT_PGI										0x00000008

#endif /* GL_PGI_vertex_hints */

/*-------------------------------------------------------------------*/
/*------------GL_REND_SCREEN_COORDINATES-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_REND_screen_coordinates
#define GL_REND_screen_coordinates 1

#define GL_SCREEN_COORDINATES_REND								0x8490
#define GL_INVERTED_SCREEN_W_REND								0x8491

#endif /* GL_REND_screen_coordinates */

/*-------------------------------------------------------------------*/
/*------------GL_S3_S3TC---------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_S3_s3tc
#define GL_S3_s3tc 1

#define GL_RGB_S3TC												0x83A0
#define GL_RGB4_S3TC											0x83A1
#define GL_RGBA_S3TC											0x83A2
#define GL_RGBA4_S3TC											0x83A3

#endif /* GL_S3_s3tc */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_DETAIL_TEXTURE---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_detail_texture
#define GL_SGIS_detail_texture 1

#define GL_DETAIL_TEXTURE_2D_SGIS								0x8095
#define GL_DETAIL_TEXTURE_2D_BINDING_SGIS						0x8096
#define GL_LINEAR_DETAIL_SGIS									0x8097
#define GL_LINEAR_DETAIL_ALPHA_SGIS								0x8098
#define GL_LINEAR_DETAIL_COLOR_SGIS								0x8099
#define GL_DETAIL_TEXTURE_LEVEL_SGIS							0x809A
#define GL_DETAIL_TEXTURE_MODE_SGIS								0x809B
#define GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS						0x809C

typedef void (APIENTRY * glDetailTexFuncSGISPROC) (GLenum target, GLsizei n, const GLfloat *points);
typedef void (APIENTRY * glGetDetailTexFuncSGISPROC) (GLenum target, GLfloat *points);

extern glDetailTexFuncSGISPROC glDetailTexFuncSGIS;
extern glGetDetailTexFuncSGISPROC glGetDetailTexFuncSGIS;

#endif /* GL_SGIS_detail_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_FOG_FUNCTION-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_fog_function
#define GL_SGIS_fog_function 1

#define GL_FOG_FUNC_SGIS										0x812A
#define GL_FOG_FUNC_POINTS_SGIS									0x812B
#define GL_MAX_FOG_FUNC_POINTS_SGIS								0x812C

typedef void (APIENTRY * glFogFuncSGISPROC) (GLsizei n, const GLfloat *points);
typedef void (APIENTRY * glGetFogFuncSGISPROC) (GLfloat *points);

extern glFogFuncSGISPROC glFogFuncSGIS;
extern glGetFogFuncSGISPROC glGetFogFuncSGIS;

#endif /* GL_SGIS_fog_function */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_GENERATE_MIPMAP--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap 1

#define GL_GENERATE_MIPMAP_SGIS                                 0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS                            0x8192

#endif /* GL_SGIS_generate_mipmap */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_MULTISAMPLE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_multisample
#define GL_SGIS_multisample 1

#define GL_MULTISAMPLE_SGIS										0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS							0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS								0x809F
#define GL_SAMPLE_MASK_SGIS										0x80A0
#define GL_1PASS_SGIS											0x80A1
#define GL_2PASS_0_SGIS											0x80A2
#define GL_2PASS_1_SGIS											0x80A3
#define GL_4PASS_0_SGIS											0x80A4
#define GL_4PASS_1_SGIS											0x80A5
#define GL_4PASS_2_SGIS											0x80A6
#define GL_4PASS_3_SGIS											0x80A7
#define GL_SAMPLE_BUFFERS_SGIS									0x80A8
#define GL_SAMPLES_SGIS											0x80A9
#define GL_SAMPLE_MASK_VALUE_SGIS								0x80AA
#define GL_SAMPLE_MASK_INVERT_SGIS								0x80AB
#define GL_SAMPLE_PATTERN_SGIS									0x80AC

typedef void (APIENTRY * glSampleMaskSGISPROC) (GLclampf value, GLboolean invert);
typedef void (APIENTRY * glSamplePatternSGISPROC) (GLenum pattern);

extern glSampleMaskSGISPROC glSampleMaskSGIS;
extern glSamplePatternSGISPROC glSamplePatternSGIS;

#endif /* GL_SGIS_multisample */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_MULTITEXTURE-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_multitexture
#define GL_SGIS_multitexture 1

#define TEXTURE0_SGIS                       					0x835E
#define TEXTURE1_SGIS                       					0x835F
#define GL_SELECTED_TEXTURE_SGIS            					0x83C0
#define GL_SELECTED_TEXTURE_COORD_SET_SGIS  					0x83C1
#define GL_SELECTED_TEXTURE_TRANSFORM_SGIS  					0x83C2
#define GL_MAX_TEXTURES_SGIS                					0x83C3
#define GL_MAX_TEXTURE_COORD_SETS_SGIS      					0x83C4
#define GL_TEXTURE_ENV_COORD_SET_SGIS       					0x83C5
#define GL_TEXTURE0_SGIS                    					0x83C6
#define GL_TEXTURE1_SGIS                    					0x83C7
#define GL_TEXTURE2_SGIS                    					0x83C8
#define GL_TEXTURE3_SGIS                    					0x83C9
#define GL_TEXTURE4_SGIS                    					0x83CA
#define GL_TEXTURE5_SGIS                    					0x83CB
#define GL_TEXTURE6_SGIS                    					0x83CC
#define GL_TEXTURE7_SGIS                    					0x83CD
#define GL_TEXTURE8_SGIS                    					0x83CE
#define GL_TEXTURE9_SGIS                    					0x83CF
#define GL_TEXTURE10_SGIS                   					0x83D0
#define GL_TEXTURE11_SGIS                   					0x83D1
#define GL_TEXTURE12_SGIS                   					0x83D2
#define GL_TEXTURE13_SGIS                   					0x83D3
#define GL_TEXTURE14_SGIS                   					0x83D4
#define GL_TEXTURE15_SGIS                   					0x83D5
#define GL_TEXTURE16_SGIS                   					0x83D6
#define GL_TEXTURE17_SGIS                   					0x83D7
#define GL_TEXTURE18_SGIS                   					0x83D8
#define GL_TEXTURE19_SGIS                   					0x83D9
#define GL_TEXTURE20_SGIS                   					0x83DA
#define GL_TEXTURE21_SGIS                   					0x83DB
#define GL_TEXTURE22_SGIS                   					0x83DC
#define GL_TEXTURE23_SGIS                   					0x83DD
#define GL_TEXTURE24_SGIS                   					0x83DE
#define GL_TEXTURE25_SGIS                   					0x83DF
#define GL_TEXTURE26_SGIS                   					0x83E0
#define GL_TEXTURE27_SGIS                   					0x83E1
#define GL_TEXTURE28_SGIS                   					0x83E2
#define GL_TEXTURE29_SGIS                   					0x83E3
#define GL_TEXTURE30_SGIS                   					0x83E4
#define GL_TEXTURE31_SGIS                   					0x83E5

typedef void (APIENTRY * glSelectTextureSGISPROC) (GLenum texture);
typedef void (APIENTRY * glSelectTextureTransformSGISPROC) (GLenum texture);
typedef void (APIENTRY * glSelectTextureCoordsetSGISPROC) (GLenum texture);
typedef void (APIENTRY * glMultitexCoord1dSGISPROC) (GLenum texture, GLdouble s);
typedef void (APIENTRY * glMultitexCoord1dvSGISPROC) (GLenum texture, const GLdouble *v);
typedef void (APIENTRY * glMultitexCoord1fSGISPROC) (GLenum texture, GLfloat s);
typedef void (APIENTRY * glMultitexCoord1fvSGISPROC) (GLenum texture, const GLfloat *v);
typedef void (APIENTRY * glMultitexCoord1iSGISPROC) (GLenum texture, GLint s);
typedef void (APIENTRY * glMultitexCoord1ivSGISPROC) (GLenum texture, const GLint *v);
typedef void (APIENTRY * glMultitexCoord1sSGISPROC) (GLenum texture, GLshort s);
typedef void (APIENTRY * glMultitexCoord1svSGISPROC) (GLenum texture, const GLshort *v);
typedef void (APIENTRY * glMultitexCoord2dSGISPROC) (GLenum texture, GLdouble s, GLdouble t);
typedef void (APIENTRY * glMultitexCoord2dvSGISPROC) (GLenum texture, const GLdouble *v);
typedef void (APIENTRY * glMultitexCoord2fSGISPROC) (GLenum texture, GLfloat s, GLfloat t);
typedef void (APIENTRY * glMultitexCoord2fvSGISPROC) (GLenum texture, const GLfloat *v);
typedef void (APIENTRY * glMultitexCoord2iSGISPROC) (GLenum texture, GLint s, GLint t);
typedef void (APIENTRY * glMultitexCoord2ivSGISPROC) (GLenum texture, const GLint *v);
typedef void (APIENTRY * glMultitexCoord2sSGISPROC) (GLenum texture, GLshort s, GLshort t);
typedef void (APIENTRY * glMultitexCoord2svSGISPROC) (GLenum texture, const GLshort *v);
typedef void (APIENTRY * glMultitexCoord3dSGISPROC) (GLenum texture, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY * glMultitexCoord3dvSGISPROC) (GLenum texture, const GLdouble *v);
typedef void (APIENTRY * glMultitexCoord3fSGISPROC) (GLenum texture, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY * glMultitexCoord3fvSGISPROC) (GLenum texture, const GLfloat *v);
typedef void (APIENTRY * glMultitexCoord3iSGISPROC) (GLenum texture, GLint s, GLint t, GLint r);
typedef void (APIENTRY * glMultitexCoord3ivSGISPROC) (GLenum texture, const GLint *v);
typedef void (APIENTRY * glMultitexCoord3sSGISPROC) (GLenum texture, GLshort s, GLshort t, GLshort r);
typedef void (APIENTRY * glMultitexCoord3svSGISPROC) (GLenum texture, const GLshort *v);
typedef void (APIENTRY * glMultitexCoord4dSGISPROC) (GLenum texture, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (APIENTRY * glMultitexCoord4dvSGISPROC) (GLenum texture, const GLdouble *v);
typedef void (APIENTRY * glMultitexCoord4fSGISPROC) (GLenum texture, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY * glMultitexCoord4fvSGISPROC) (GLenum texture, const GLfloat *v);
typedef void (APIENTRY * glMultitexCoord4iSGISPROC) (GLenum texture, GLint s, GLint t, GLint r, GLint q);
typedef void (APIENTRY * glMultitexCoord4ivSGISPROC) (GLenum texture, const GLint *v);
typedef void (APIENTRY * glMultitexCoord4sSGISPROC) (GLenum texture, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (APIENTRY * glMultitexCoord4svSGISPROC) (GLenum texture, const GLshort *v);

extern glSelectTextureSGISPROC glSelectTextureSGIS;
extern glSelectTextureTransformSGISPROC glSelectTextureTransformSGIS;
extern glSelectTextureCoordsetSGISPROC glSelectTextureCoordsetSGIS;
extern glMultitexCoord1dSGISPROC glMultitexCoord1dSGIS;
extern glMultitexCoord1dvSGISPROC glMultitexCoord1dvSGIS;
extern glMultitexCoord1fSGISPROC glMultitexCoord1fSGIS;
extern glMultitexCoord1fvSGISPROC glMultitexCoord1fvSGIS;
extern glMultitexCoord1iSGISPROC glMultitexCoord1iSGIS;
extern glMultitexCoord1ivSGISPROC glMultitexCoord1ivSGIS;
extern glMultitexCoord1sSGISPROC glMultitexCoord1sSGIS;
extern glMultitexCoord1svSGISPROC glMultitexCoord1svSGIS;
extern glMultitexCoord2dSGISPROC glMultitexCoord2dSGIS;
extern glMultitexCoord2dvSGISPROC glMultitexCoord2dvSGIS;
extern glMultitexCoord2fSGISPROC glMultitexCoord2fSGIS;
extern glMultitexCoord2fvSGISPROC glMultitexCoord2fvSGIS;
extern glMultitexCoord2iSGISPROC glMultitexCoord2iSGIS;
extern glMultitexCoord2ivSGISPROC glMultitexCoord2ivSGIS;
extern glMultitexCoord2sSGISPROC glMultitexCoord2sSGIS;
extern glMultitexCoord2svSGISPROC glMultitexCoord2svSGIS;
extern glMultitexCoord3dSGISPROC glMultitexCoord3dSGIS;
extern glMultitexCoord3dvSGISPROC glMultitexCoord3dvSGIS;
extern glMultitexCoord3fSGISPROC glMultitexCoord3fSGIS;
extern glMultitexCoord3fvSGISPROC glMultitexCoord3fvSGIS;
extern glMultitexCoord3iSGISPROC glMultitexCoord3iSGIS;
extern glMultitexCoord3ivSGISPROC glMultitexCoord3ivSGIS;
extern glMultitexCoord3sSGISPROC glMultitexCoord3sSGIS;
extern glMultitexCoord3svSGISPROC glMultitexCoord3svSGIS;
extern glMultitexCoord4dSGISPROC glMultitexCoord4dSGIS;
extern glMultitexCoord4dvSGISPROC glMultitexCoord4dvSGIS;
extern glMultitexCoord4fSGISPROC glMultitexCoord4fSGIS;
extern glMultitexCoord4fvSGISPROC glMultitexCoord4fvSGIS;
extern glMultitexCoord4iSGISPROC glMultitexCoord4iSGIS;
extern glMultitexCoord4ivSGISPROC glMultitexCoord4ivSGIS;
extern glMultitexCoord4sSGISPROC glMultitexCoord4sSGIS;
extern glMultitexCoord4svSGISPROC glMultitexCoord4svSGIS;

#endif /* GL_SGIS_multitexture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_PIXEL_TEXTURE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_pixel_texture
#define GL_SGIS_pixel_texture 1

#define GL_PIXEL_TEXTURE_SGIS									0x8353
#define GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS						0x8354
#define GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS						0x8355
#define GL_PIXEL_GROUP_COLOR_SGIS								0x8356

typedef void (APIENTRY * glPixelTexGenParameteriSGISPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glPixelTexGenParameterivSGISPROC) (GLenum pname, const GLint *params);
typedef void (APIENTRY * glPixelTexGenParameterfSGISPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glPixelTexGenParameterfvSGISPROC) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glGetPixelTexGenParameterivSGISPROC) (GLenum pname, GLint *params);
typedef void (APIENTRY * glGetPixelTexGenParameterfvSGISPROC) (GLenum pname, GLfloat *params);

extern glPixelTexGenParameteriSGISPROC glPixelTexGenParameteriSGIS;
extern glPixelTexGenParameterivSGISPROC glPixelTexGenParameterivSGIS;
extern glPixelTexGenParameterfSGISPROC glPixelTexGenParameterfSGIS;
extern glPixelTexGenParameterfvSGISPROC glPixelTexGenParameterfvSGIS;
extern glGetPixelTexGenParameterivSGISPROC glGetPixelTexGenParameterivSGIS;
extern glGetPixelTexGenParameterfvSGISPROC glGetPixelTexGenParameterfvSGIS;

#endif /* GL_SGIS_pixel_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_POINT_LINE_TEXGEN------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_point_line_texgen
#define GL_SGIS_point_line_texgen 1

#define GL_EYE_DISTANCE_TO_POINT_SGIS							0x81F0
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS						0x81F1
#define GL_EYE_DISTANCE_TO_LINE_SGIS							0x81F2
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS							0x81F3
#define GL_EYE_POINT_SGIS										0x81F4
#define GL_OBJECT_POINT_SGIS									0x81F5
#define GL_EYE_LINE_SGIS										0x81F6
#define GL_OBJECT_LINE_SGIS										0x81F7

#endif /* GL_SGIS_point_line_texgen */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_POINT_PARAMETERS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_point_parameters
#define GL_SGIS_point_parameters 1

#define GL_POINT_SIZE_MIN_SGIS									0x8126
#define GL_POINT_SIZE_MAX_SGIS									0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_SGIS						0x8128
#define GL_DISTANCE_ATTENUATION_SGIS							0x8129

typedef void (APIENTRY * glPointParameterfSGISPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glPointParameterfvSGISPROC) (GLenum pname, const GLfloat *params);

extern glPointParameterfSGISPROC glPointParameterfSGIS;
extern glPointParameterfvSGISPROC glPointParameterfvSGIS;

#endif /* GL_SGIS_point_parameters */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_SHARPEN_TEXTURE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_sharpen_texture
#define GL_SGIS_sharpen_texture 1

#define GL_LINEAR_SHARPEN_SGIS									0x80AD
#define GL_LINEAR_SHARPEN_ALPHA_SGIS							0x80AE
#define GL_LINEAR_SHARPEN_COLOR_SGIS							0x80AF
#define GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS						0x80B0

typedef void (APIENTRY * glSharpenTexFuncSGISPROC) (GLenum target, GLsizei n, const GLfloat *points);
typedef void (APIENTRY * glGetSharpenTexFuncSGISPROC) (GLenum target, GLfloat *points);

extern glSharpenTexFuncSGISPROC glSharpenTexFuncSGIS;
extern glGetSharpenTexFuncSGISPROC glGetSharpenTexFuncSGIS;

#endif /* GL_SGIS_sharpen_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE4D--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture4D
#define GL_SGIS_texture4D 1

#define GL_PACK_SKIP_VOLUMES_SGIS								0x8130
#define GL_PACK_IMAGE_DEPTH_SGIS								0x8131
#define GL_UNPACK_SKIP_VOLUMES_SGIS								0x8132
#define GL_UNPACK_IMAGE_DEPTH_SGIS								0x8133
#define GL_TEXTURE_4D_SGIS										0x8134
#define GL_PROXY_TEXTURE_4D_SGIS								0x8135
#define GL_TEXTURE_4DSIZE_SGIS									0x8136
#define GL_TEXTURE_WRAP_Q_SGIS									0x8137
#define GL_MAX_4D_TEXTURE_SIZE_SGIS								0x8138
#define GL_TEXTURE_4D_BINDING_SGIS								0x814F

typedef void (APIENTRY * glTexImage4DSGISPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * glTexSubImage4DSGISPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels);

extern glTexImage4DSGISPROC glTexImage4DSGIS;
extern glTexSubImage4DSGISPROC glTexSubImage4DSGIS;

#endif /* GL_SGIS_texture4D */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_BORDER_CLAMP---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_border_clamp
#define GL_SGIS_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_SGIS									0x812D

#endif /* GL_SGIS_texture_border_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_COLOR_MASK-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_color_mask
#define GL_SGIS_texture_color_mask 1

#define GL_TEXTURE_COLOR_WRITEMASK_SGIS							0x81EF

typedef void (APIENTRY * glTextureColorMaskSGISPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

extern glTextureColorMaskSGISPROC glTextureColorMaskSGIS;

#endif /* GL_SGIS_texture_color_mask */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_EDGE_CLAMP-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_edge_clamp
#define GL_SGIS_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_SGIS									0x812F

#endif /* GL_SGIS_texture_edge_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_FILTER4--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_filter4
#define GL_SGIS_texture_filter4 1

#define GL_FILTER4_SGIS											0x8146
#define GL_TEXTURE_FILTER4_SIZE_SGIS							0x8147

typedef void (APIENTRY * glGetTexFilterFuncSGISPROC) (GLenum target, GLenum filter, GLfloat *weights);
typedef void (APIENTRY * glTexFilterFuncSGISPROC) (GLenum target, GLenum filter, GLsizei n, const GLfloat *weights);

extern glGetTexFilterFuncSGISPROC glGetTexFilterFuncSGIS;
extern glTexFilterFuncSGISPROC glTexFilterFuncSGIS;

#endif /* GL_SGIS_texture_filter4 */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_LOD------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod 1

#define GL_TEXTURE_MIN_LOD_SGIS                                 0x813A
#define GL_TEXTURE_MAX_LOD_SGIS                                 0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS                              0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS                               0x813D

#endif /* GL_SGIS_texture_lod */

/*-------------------------------------------------------------------*/
/*------------GL_SGIS_TEXTURE_SELECT---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIS_texture_select
#define GL_SGIS_texture_select 1

#define GL_DUAL_ALPHA4_SGIS										0x8110
#define GL_DUAL_ALPHA8_SGIS										0x8111
#define GL_DUAL_ALPHA12_SGIS									0x8112
#define GL_DUAL_ALPHA16_SGIS									0x8113
#define GL_DUAL_LUMINANCE4_SGIS									0x8114
#define GL_DUAL_LUMINANCE8_SGIS									0x8115
#define GL_DUAL_LUMINANCE12_SGIS								0x8116
#define GL_DUAL_LUMINANCE16_SGIS								0x8117
#define GL_DUAL_INTENSITY4_SGIS									0x8118
#define GL_DUAL_INTENSITY8_SGIS									0x8119
#define GL_DUAL_INTENSITY12_SGIS								0x811A
#define GL_DUAL_INTENSITY16_SGIS								0x811B
#define GL_DUAL_LUMINANCE_ALPHA4_SGIS							0x811C
#define GL_DUAL_LUMINANCE_ALPHA8_SGIS							0x811D
#define GL_QUAD_ALPHA4_SGIS										0x811E
#define GL_QUAD_ALPHA8_SGIS										0x811F
#define GL_QUAD_LUMINANCE4_SGIS									0x8120
#define GL_QUAD_LUMINANCE8_SGIS									0x8121
#define GL_QUAD_INTENSITY4_SGIS									0x8122
#define GL_QUAD_INTENSITY8_SGIS									0x8123
#define GL_DUAL_TEXTURE_SELECT_SGIS								0x8124
#define GL_QUAD_TEXTURE_SELECT_SGIS								0x8125

#endif /* GL_SGIS_texture_select */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_ASYNC------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_async
#define GL_SGIX_async 1

#define GL_ASYNC_MARKER_SGIX									0x8329

typedef void (APIENTRY * glAsyncMarkerSGIXPROC) (GLuint marker);
typedef GLint (APIENTRY * glFinishAsyncSGIXPROC) (GLuint *markerp);
typedef GLint (APIENTRY * glPollAsyncSGIXPROC) (GLuint *markerp);
typedef GLuint (APIENTRY * glGenAsyncMarkersSGIXPROC) (GLsizei range);
typedef void (APIENTRY * glDeleteAsyncMarkersSGIXPROC) (GLuint marker, GLsizei range);
typedef GLboolean (APIENTRY * glIsAsyncMarkerSGIXPROC) (GLuint marker);

extern glAsyncMarkerSGIXPROC glAsyncMarkerSGIX;
extern glFinishAsyncSGIXPROC glFinishAsyncSGIX;
extern glPollAsyncSGIXPROC glPollAsyncSGIX;
extern glGenAsyncMarkersSGIXPROC glGenAsyncMarkersSGIX;
extern glDeleteAsyncMarkersSGIXPROC glDeleteAsyncMarkersSGIX;
extern glIsAsyncMarkerSGIXPROC glIsAsyncMarkerSGIX;

#endif /* GL_SGIX_async */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_ASYNC_HISTOGRAM--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_async_histogram
#define GL_SGIX_async_histogram 1

#define GL_ASYNC_HISTOGRAM_SGIX									0x832C
#define GL_MAX_ASYNC_HISTOGRAM_SGIX								0x832D

#endif /* GL_SGIX_async_histogram */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_ASYNC_PIXEL------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_async_pixel
#define GL_SGIX_async_pixel 1

#define GL_ASYNC_TEX_IMAGE_SGIX									0x835C
#define GL_ASYNC_DRAW_PIXELS_SGIX								0x835D
#define GL_ASYNC_READ_PIXELS_SGIX								0x835E
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX								0x835F
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX							0x8360
#define GL_MAX_ASYNC_READ_PIXELS_SGIX							0x8361

#endif /* GL_SGIX_async_pixel */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_BLEND_ALPHA_MINMAX-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_blend_alpha_minmax
#define GL_SGIX_blend_alpha_minmax 1

#define GL_ALPHA_MIN_SGIX										0x8320
#define GL_ALPHA_MAX_SGIX										0x8321

#endif /* GL_SGIX_blend_alpha_minmax */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_CALLIGRAPHIC_FRAGMENT--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_calligraphic_fragment
#define GL_SGIX_calligraphic_fragment 1

#define GL_CALLIGRAPHIC_FRAGMENT_SGIX							0x8183

#endif /* GL_SGIX_calligraphic_fragment */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_CLIPMAP----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_clipmap
#define GL_SGIX_clipmap 1

#define GL_LINEAR_CLIPMAP_LINEAR_SGIX							0x8170
#define GL_TEXTURE_CLIPMAP_CENTER_SGIX							0x8171
#define GL_TEXTURE_CLIPMAP_FRAME_SGIX							0x8172
#define GL_TEXTURE_CLIPMAP_OFFSET_SGIX							0x8173
#define GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX					0x8174
#define GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX						0x8175
#define GL_TEXTURE_CLIPMAP_DEPTH_SGIX							0x8176
#define GL_MAX_CLIPMAP_DEPTH_SGIX								0x8177
#define GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX						0x8178
#define GL_NEAREST_CLIPMAP_NEAREST_SGIX							0x844D
#define GL_NEAREST_CLIPMAP_LINEAR_SGIX							0x844E
#define GL_LINEAR_CLIPMAP_NEAREST_SGIX							0x844F

#endif /* GL_SGIX_clipmap */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_CONVOLUTION_ACCURACY---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_convolution_accuracy
#define GL_SGIX_convolution_accuracy 1

#define GL_CONVOLUTION_HINT_SGIX								0x8316

#endif /* GL_SGIX_convolution_accuracy */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_DEPTH_TEXTURE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_depth_texture
#define GL_SGIX_depth_texture 1

#define GL_DEPTH_COMPONENT16_SGIX                                0x81A5
#define GL_DEPTH_COMPONENT24_SGIX                                0x81A6
#define GL_DEPTH_COMPONENT32_SGIX                                0x81A7

#endif /* GL_SGIX_depth_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_FLUSH_RASTER-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_flush_raster
#define GL_SGIX_flush_raster 1

typedef void (APIENTRY * glFlushRasterSGIXPROC) (void);

extern glFlushRasterSGIXPROC glFlushRasterSGIX;

#endif /* GL_SGIX_flush_raster */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_FOG_OFFSET-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_fog_offset
#define GL_SGIX_fog_offset 1

#define GL_FOG_OFFSET_SGIX										0x8198
#define GL_FOG_OFFSET_VALUE_SGIX								0x8199

#endif /* GL_SGIX_fog_offset */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_FOG_SCALE--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_fog_scale
#define GL_SGIX_fog_scale 1

#define GL_FOG_SCALE_SGIX										0x81FC
#define GL_FOG_SCALE_VALUE_SGIX									0x81FD

#endif /* GL_SGIX_fog_scale */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_FRAGMENT_LIGHTING------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_fragment_lighting
#define GL_SGIX_fragment_lighting 1

#define GL_FRAGMENT_LIGHTING_SGIX								0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_SGIX							0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX					0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX				0x8403
#define GL_MAX_FRAGMENT_LIGHTS_SGIX								0x8404
#define GL_MAX_ACTIVE_LIGHTS_SGIX								0x8405
#define GL_CURRENT_RASTER_NORMAL_SGIX							0x8406
#define GL_LIGHT_ENV_MODE_SGIX									0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX				0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX					0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX					0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX		0x840B
#define GL_FRAGMENT_LIGHT0_SGIX									0x840C
#define GL_FRAGMENT_LIGHT1_SGIX									0x840D
#define GL_FRAGMENT_LIGHT2_SGIX									0x840E
#define GL_FRAGMENT_LIGHT3_SGIX									0x840F
#define GL_FRAGMENT_LIGHT4_SGIX									0x8410
#define GL_FRAGMENT_LIGHT5_SGIX									0x8411
#define GL_FRAGMENT_LIGHT6_SGIX									0x8412
#define GL_FRAGMENT_LIGHT7_SGIX									0x8413

typedef void (APIENTRY * glFragmentColorMaterialSGIXPROC) (GLenum face, GLenum mode);
typedef void (APIENTRY * glFragmentLightfSGIXPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (APIENTRY * glFragmentLightfvSGIXPROC) (GLenum light, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glFragmentLightiSGIXPROC) (GLenum light, GLenum pname, GLint param);
typedef void (APIENTRY * glFragmentLightivSGIXPROC) (GLenum light, GLenum pname, const GLint *params);
typedef void (APIENTRY * glFragmentLightModelfSGIXPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glFragmentLightModelfvSGIXPROC) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glFragmentLightModeliSGIXPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glFragmentLightModelivSGIXPROC) (GLenum pname, const GLint *params);
typedef void (APIENTRY * glFragmentMaterialfSGIXPROC) (GLenum face, GLenum pname, GLfloat param);
typedef void (APIENTRY * glFragmentMaterialfvSGIXPROC) (GLenum face, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glFragmentMaterialiSGIXPROC) (GLenum face, GLenum pname, GLint param);
typedef void (APIENTRY * glFragmentMaterialivSGIXPROC) (GLenum face, GLenum pname, const GLint *params);
typedef void (APIENTRY * glGetFragmentLightfvSGIXPROC) (GLenum light, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetFragmentLightivSGIXPROC) (GLenum light, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetFragmentMaterialfvSGIXPROC) (GLenum face, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetFragmentMaterialivSGIXPROC) (GLenum face, GLenum pname, GLint *params);
typedef void (APIENTRY * glLightEnviSGIXPROC) (GLenum pname, GLint param);

extern glFragmentColorMaterialSGIXPROC glFragmentColorMaterialSGIX;
extern glFragmentLightfSGIXPROC glFragmentLightfSGIX;
extern glFragmentLightfvSGIXPROC glFragmentLightfvSGIX;
extern glFragmentLightiSGIXPROC glFragmentLightiSGIX;
extern glFragmentLightivSGIXPROC glFragmentLightivSGIX;
extern glFragmentLightModelfSGIXPROC glFragmentLightModelfSGIX;
extern glFragmentLightModelfvSGIXPROC glFragmentLightModelfvSGIX;
extern glFragmentLightModeliSGIXPROC glFragmentLightModeliSGIX;
extern glFragmentLightModelivSGIXPROC glFragmentLightModelivSGIX;
extern glFragmentMaterialfSGIXPROC glFragmentMaterialfSGIX;
extern glFragmentMaterialfvSGIXPROC glFragmentMaterialfvSGIX;
extern glFragmentMaterialiSGIXPROC glFragmentMaterialiSGIX;
extern glFragmentMaterialivSGIXPROC glFragmentMaterialivSGIX;
extern glGetFragmentLightfvSGIXPROC glGetFragmentLightfvSGIX;
extern glGetFragmentLightivSGIXPROC glGetFragmentLightivSGIX;
extern glGetFragmentMaterialfvSGIXPROC glGetFragmentMaterialfvSGIX;
extern glGetFragmentMaterialivSGIXPROC glGetFragmentMaterialivSGIX;
extern glLightEnviSGIXPROC glLightEnviSGIX;

#endif /* GL_SGIX_fragment_lighting */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_FRAMEZOOM--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_framezoom
#define GL_SGIX_framezoom 1

#define GL_FRAMEZOOM_SGIX										0x818B
#define GL_FRAMEZOOM_FACTOR_SGIX								0x818C
#define GL_MAX_FRAMEZOOM_FACTOR_SGIX							0x818D

typedef void (APIENTRY * glFrameZoomSGIXPROC) (GLint factor);

extern glFrameZoomSGIXPROC glFrameZoomSGIX;

#endif /* GL_SGIX_framezoom */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_IMPACT_PIXEL_TEXTURE---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_impact_pixel_texture
#define GL_SGIX_impact_pixel_texture 1

#define GL_PIXEL_TEX_GEN_Q_CEILING_SGIX							0x8184
#define GL_PIXEL_TEX_GEN_Q_ROUND_SGIX							0x8185
#define GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX							0x8186
#define GL_PIXEL_TEX_GEN_ALPHA_REPLACE_SGIX						0x8187
#define GL_PIXEL_TEX_GEN_ALPHA_NO_REPLACE_SGIX					0x8188
#define GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX							0x8189
#define GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX							0x818A

#endif /* GL_SGIX_impact_pixel_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_INSTRUMENTS------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_instruments
#define GL_SGIX_instruments 1

#define GL_INSTRUMENT_BUFFER_POINTER_SGIX						0x8180
#define GL_INSTRUMENT_MEASUREMENTS_SGIX							0x8181

typedef GLint (APIENTRY * glGetInstrumentsSGIXPROC) (void);
typedef void (APIENTRY * glInstrumentsBufferSGIXPROC) (GLsizei size, GLint *buffer);
typedef GLint (APIENTRY * glPollInstrumentsSGIXPROC) (GLint *marker_p);
typedef void (APIENTRY * glReadInstrumentsSGIXPROC) (GLint marker);
typedef void (APIENTRY * glStartInstrumentsSGIXPROC) (void);
typedef void (APIENTRY * glStopInstrumentsSGIXPROC) (GLint marker);

extern glGetInstrumentsSGIXPROC glGetInstrumentsSGIX;
extern glInstrumentsBufferSGIXPROC glInstrumentsBufferSGIX;
extern glPollInstrumentsSGIXPROC glPollInstrumentsSGIX;
extern glReadInstrumentsSGIXPROC glReadInstrumentsSGIX;
extern glStartInstrumentsSGIXPROC glStartInstrumentsSGIX;
extern glStopInstrumentsSGIXPROC glStopInstrumentsSGIX;

#endif /* GL_SGIX_instruments */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_INTERLACE--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_interlace
#define GL_SGIX_interlace 1

#define GL_INTERLACE_SGIX										0x8094

#endif /* GL_SGIX_interlace */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_IR_INSTRUMENT1---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_ir_instrument1
#define GL_SGIX_ir_instrument1 1

#define GL_IR_INSTRUMENT1_SGIX									0x817F

#endif /* GL_SGIX_ir_instrument1 */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_LIST_PRIORITY----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_list_priority
#define GL_SGIX_list_priority 1

#define GL_LIST_PRIORITY_SGIX									0x8182

typedef void (APIENTRY * glGetListParameterfvSGIXPROC) (GLuint list, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetListParameterivSGIXPROC) (GLuint list, GLenum pname, GLint *params);
typedef void (APIENTRY * glListParameterfSGIXPROC) (GLuint list, GLenum pname, GLfloat param);
typedef void (APIENTRY * glListParameterfvSGIXPROC) (GLuint list, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glListParameteriSGIXPROC) (GLuint list, GLenum pname, GLint param);
typedef void (APIENTRY * glListParameterivSGIXPROC) (GLuint list, GLenum pname, const GLint *params);

extern glGetListParameterfvSGIXPROC glGetListParameterfvSGIX;
extern glGetListParameterivSGIXPROC glGetListParameterivSGIX;
extern glListParameterfSGIXPROC glListParameterfSGIX;
extern glListParameterfvSGIXPROC glListParameterfvSGIX;
extern glListParameteriSGIXPROC glListParameteriSGIX;
extern glListParameterivSGIXPROC glListParameterivSGIX;

#endif /* GL_SGIX_list_priority */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_PIXEL_TEXTURE----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_pixel_texture
#define GL_SGIX_pixel_texture 1

#define GL_PIXEL_TEX_GEN_SGIX									0x8139
#define GL_PIXEL_TEX_GEN_MODE_SGIX								0x832B

typedef void (APIENTRY * glPixelTexGenSGIXPROC) (GLenum mode);

extern glPixelTexGenSGIXPROC glPixelTexGenSGIX;

#endif /* GL_SGIX_pixel_texture */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_PIXEL_TILES------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_pixel_tiles
#define GL_SGIX_pixel_tiles 1

#define GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX						0x813E
#define GL_PIXEL_TILE_CACHE_INCREMENT_SGIX						0x813F
#define GL_PIXEL_TILE_WIDTH_SGIX								0x8140
#define GL_PIXEL_TILE_HEIGHT_SGIX								0x8141
#define GL_PIXEL_TILE_GRID_WIDTH_SGIX							0x8142
#define GL_PIXEL_TILE_GRID_HEIGHT_SGIX							0x8143
#define GL_PIXEL_TILE_GRID_DEPTH_SGIX							0x8144
#define GL_PIXEL_TILE_CACHE_SIZE_SGIX							0x8145

#endif /* GL_SGIX_pixel_tiles */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_POLYNOMIAL_FFD---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_polynomial_ffd
#define GL_SGIX_polynomial_ffd 1

#define GL_GEOMETRY_DEFORMATION_SGIX							0x8194
#define GL_TEXTURE_DEFORMATION_SGIX								0x8195
#define GL_DEFORMATIONS_MASK_SGIX								0x8196
#define GL_MAX_DEFORMATION_ORDER_SGIX							0x8197

typedef void (APIENTRY * glDeformationMap3dSGIXPROC) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points);
typedef void (APIENTRY * glDeformationMap3fSGIXPROC) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points);
typedef void (APIENTRY * glDeformSGIXPROC) (GLbitfield mask);
typedef void (APIENTRY * glLoadIdentityDeformationMapSGIXPROC) (GLbitfield mask);

extern glDeformationMap3dSGIXPROC glDeformationMap3dSGIX;
extern glDeformationMap3fSGIXPROC glDeformationMap3fSGIX;
extern glDeformSGIXPROC glDeformSGIX;
extern glLoadIdentityDeformationMapSGIXPROC glLoadIdentityDeformationMapSGIX;

#endif /* GL_SGIX_polynomial_ffd */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_REFERENCE_PLANE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_reference_plane
#define GL_SGIX_reference_plane 1

#define GL_REFERENCE_PLANE_SGIX									0x817D
#define GL_REFERENCE_PLANE_EQUATION_SGIX						0x817E

typedef void (APIENTRY * glReferencePlaneSGIXPROC) (const GLdouble *equation);

extern glReferencePlaneSGIXPROC glReferencePlaneSGIX;

#endif /* GL_SGIX_reference_plane */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_RESAMPLE---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_resample
#define GL_SGIX_resample 1

#define GL_PACK_RESAMPLE_SGIX									0x842C
#define GL_UNPACK_RESAMPLE_SGIX									0x842D
#define GL_RESAMPLE_REPLICATE_SGIX								0x842E
#define GL_RESAMPLE_ZERO_FILL_SGIX								0x842F
#define GL_RESAMPLE_DECIMATE_SGIX								0x8430

#endif /* GL_SGIX_resample */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_SCALEBIAS_HINT---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_scalebias_hint
#define GL_SGIX_scalebias_hint 1

#define GL_SCALEBIAS_HINT_SGIX									0x8322

#endif /* GL_SGIX_scalebias_hint */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_SHADOW-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_shadow
#define GL_SGIX_shadow 1

#define GL_TEXTURE_COMPARE_SGIX                                 0x819A
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX                        0x819B
#define GL_TEXTURE_LEQUAL_R_SGIX                                0x819C
#define GL_TEXTURE_GEQUAL_R_SGIX                                0x819D

#endif /* GL_SGIX_shadow */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_SHADOW_AMBIENT---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_shadow_ambient
#define GL_SGIX_shadow_ambient 1

#define GL_SHADOW_AMBIENT_SGIX									0x80BF

#endif /* GL_SGIX_shadow_ambient */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_SPRITE-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_sprite
#define GL_SGIX_sprite 1

#define GL_SPRITE_SGIX											0x8148
#define GL_SPRITE_MODE_SGIX										0x8149
#define GL_SPRITE_AXIS_SGIX										0x814A
#define GL_SPRITE_TRANSLATION_SGIX								0x814B
#define GL_SPRITE_AXIAL_SGIX									0x814C
#define GL_SPRITE_OBJECT_ALIGNED_SGIX							0x814D
#define GL_SPRITE_EYE_ALIGNED_SGIX								0x814E

typedef void (APIENTRY * glSpriteParameterfSGIXPROC) (GLenum pname, GLfloat param);
typedef void (APIENTRY * glSpriteParameterfvSGIXPROC) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glSpriteParameteriSGIXPROC) (GLenum pname, GLint param);
typedef void (APIENTRY * glSpriteParameterivSGIXPROC) (GLenum pname, const GLint *params);

extern glSpriteParameterfSGIXPROC glSpriteParameterfSGIX;
extern glSpriteParameterfvSGIXPROC glSpriteParameterfvSGIX;
extern glSpriteParameteriSGIXPROC glSpriteParameteriSGIX;
extern glSpriteParameterivSGIXPROC glSpriteParameterivSGIX;

#endif /* GL_SGIX_sprite */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_SUBSAMPLE--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_subsample
#define GL_SGIX_subsample 1

#define GL_PACK_SUBSAMPLE_RATE_SGIX								0x85A0
#define GL_UNPACK_SUBSAMPLE_RATE_SGIX							0x85A1
#define GL_PIXEL_SUBSAMPLE_4444_SGIX							0x85A2
#define GL_PIXEL_SUBSAMPLE_2424_SGIX							0x85A3
#define GL_PIXEL_SUBSAMPLE_4242_SGIX							0x85A4

#endif /* GL_SGIX_subsample */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TAG_SAMPLE_BUFFER------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_tag_sample_buffer
#define GL_SGIX_tag_sample_buffer 1

typedef void (APIENTRY * glTagSampleBufferSGIXPROC) (void);

extern glTagSampleBufferSGIXPROC glTagSampleBufferSGIX;

#endif /* GL_SGIX_tag_sample_buffer */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TEXTURE_ADD_ENV--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_texture_add_env
#define GL_SGIX_texture_add_env 1

#define GL_TEXTURE_ENV_BIAS_SGIX								0x80BE

#endif /* GL_SGIX_texture_add_env */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TEXTURE_COORDINATE_CLAMP-----------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_texture_coordinate_clamp
#define GL_SGIX_texture_coordinate_clamp 1

#define GL_TEXTURE_MAX_CLAMP_S_SGIX								0x8369
#define GL_TEXTURE_MAX_CLAMP_T_SGIX								0x836A
#define GL_TEXTURE_MAX_CLAMP_R_SGIX								0x836B

#endif /* GL_SGIX_texture_coordinate_clamp */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TEXTURE_LOD_BIAS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_texture_lod_bias
#define GL_SGIX_texture_lod_bias 1

#define GL_TEXTURE_LOD_BIAS_S_SGIX								0x818E
#define GL_TEXTURE_LOD_BIAS_T_SGIX								0x818F
#define GL_TEXTURE_LOD_BIAS_R_SGIX								0x8190

#endif /* GL_SGIX_texture_lod_bias */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TEXTURE_MULTI_BUFFER---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_texture_multi_buffer
#define GL_SGIX_texture_multi_buffer 1

#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX						0x812E

#endif /* GL_SGIX_texture_multi_buffer */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_TEXTURE_SCALE_BIAS-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_texture_scale_bias
#define GL_SGIX_texture_scale_bias 1

#define GL_POST_TEXTURE_FILTER_BIAS_SGIX						0x8179
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX						0x817A
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX					0x817B
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX					0x817C

#endif /* GL_SGIX_texture_scale_bias */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_VERTEX_PRECLIP---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_vertex_preclip
#define GL_SGIX_vertex_preclip 1

#define GL_VERTEX_PRECLIP_SGIX									0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX								0x83EF

#endif /* GL_SGIX_vertex_preclip */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_YCRCB------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_ycrcb
#define GL_SGIX_ycrcb 1

#define GL_YCRCB_422_SGIX										0x81BB
#define GL_YCRCB_444_SGIX										0x81BC

#endif /* GL_SGIX_ycrcb */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_YCRCB_SUBSAMPLE--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_ycrcb_subsample
#define GL_SGIX_ycrcb_subsample 1

#endif /* GL_SGIX_ycrcb_subsample */

/*-------------------------------------------------------------------*/
/*------------GL_SGIX_YCRCBA-----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGIX_ycrcba
#define GL_SGIX_ycrcba 1

#define GL_YCRCB_SGIX											0x8318
#define GL_YCRCBA_SGIX											0x8319

#endif /* GL_SGIX_ycrcba */

/*-------------------------------------------------------------------*/
/*------------GL_SGI_COLOR_MATRIX------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGI_color_matrix
#define GL_SGI_color_matrix 1

#define GL_COLOR_MATRIX_SGI										0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI							0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI						0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI						0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI					0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI						0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI					0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI						0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI						0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI						0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI						0x80BB

#endif /* GL_SGI_color_matrix */

/*-------------------------------------------------------------------*/
/*------------GL_SGI_COLOR_TABLE-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGI_color_table
#define GL_SGI_color_table 1

#define GL_COLOR_TABLE_SGI										0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI						0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI					0x80D2
#define GL_PROXY_COLOR_TABLE_SGI								0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI				0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI				0x80D5
#define GL_COLOR_TABLE_SCALE_SGI								0x80D6
#define GL_COLOR_TABLE_BIAS_SGI									0x80D7
#define GL_COLOR_TABLE_FORMAT_SGI								0x80D8
#define GL_COLOR_TABLE_WIDTH_SGI								0x80D9
#define GL_COLOR_TABLE_RED_SIZE_SGI								0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_SGI							0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_SGI							0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI							0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI						0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI						0x80DF

typedef void (APIENTRY * glColorTableSGIPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void (APIENTRY * glColorTableParameterfvSGIPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glColorTableParameterivSGIPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glCopyColorTableSGIPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (APIENTRY * glGetColorTableSGIPROC) (GLenum target, GLenum format, GLenum type, GLvoid *table);
typedef void (APIENTRY * glGetColorTableParameterfvSGIPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetColorTableParameterivSGIPROC) (GLenum target, GLenum pname, GLint *params);

extern glColorTableSGIPROC glColorTableSGI;
extern glColorTableParameterfvSGIPROC glColorTableParameterfvSGI;
extern glColorTableParameterivSGIPROC glColorTableParameterivSGI;
extern glCopyColorTableSGIPROC glCopyColorTableSGI;
extern glGetColorTableSGIPROC glGetColorTableSGI;
extern glGetColorTableParameterfvSGIPROC glGetColorTableParameterfvSGI;
extern glGetColorTableParameterivSGIPROC glGetColorTableParameterivSGI;

#endif /* GL_SGI_color_table */

/*-------------------------------------------------------------------*/
/*------------GL_SGI_DEPTH_PASS_INSTRUMENT---------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGI_depth_pass_instrument
#define GL_SGI_depth_pass_instrument 1

#define GL_DEPTH_PASS_INSTRUMENT_SGIX							0x8310
#define GL_DEPTH_PASS_INSTRUMENT_COUNTERS_SGIX					0x8311
#define GL_DEPTH_PASS_INSTRUMENT_MAX_SGIX						0x8312

#endif /* GL_SGI_depth_pass_instrument */

/*-------------------------------------------------------------------*/
/*------------GL_SGI_TEXTURE_COLOR_TABLE-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SGI_texture_color_table
#define GL_SGI_texture_color_table 1

#define GL_TEXTURE_COLOR_TABLE_SGI								0x80BC
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI						0x80BD

#endif /* GL_SGI_texture_color_table */

/*-------------------------------------------------------------------*/
/*------------GL_SUNX_CONSTANT_DATA----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUNX_constant_data
#define GL_SUNX_constant_data 1

#define GL_UNPACK_CONSTANT_DATA_SUNX							0x81D5
#define GL_TEXTURE_CONSTANT_DATA_SUNX							0x81D6

typedef void (APIENTRY * glFinishTextureSUNXPROC) (void);

extern glFinishTextureSUNXPROC glFinishTextureSUNX;

#endif /* GL_SUNX_constant_data */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_CONVOLUTION_BORDER_MODES------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_convolution_border_modes
#define GL_SUN_convolution_border_modes 1

#define GL_WRAP_BORDER_SUN										0x81D4

#endif /* GL_SUN_convolution_border_modes */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_GLOBAL_ALPHA------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_global_alpha
#define GL_SUN_global_alpha 1

#define GL_GLOBAL_ALPHA_SUN										0x81D9
#define GL_GLOBAL_ALPHA_FACTOR_SUN								0x81DA

typedef void (APIENTRY * glGlobalAlphaFactorbSUNPROC) (GLbyte factor);
typedef void (APIENTRY * glGlobalAlphaFactorsSUNPROC) (GLshort factor);
typedef void (APIENTRY * glGlobalAlphaFactoriSUNPROC) (GLint factor);
typedef void (APIENTRY * glGlobalAlphaFactorfSUNPROC) (GLfloat factor);
typedef void (APIENTRY * glGlobalAlphaFactordSUNPROC) (GLdouble factor);
typedef void (APIENTRY * glGlobalAlphaFactorubSUNPROC) (GLubyte factor);
typedef void (APIENTRY * glGlobalAlphaFactorusSUNPROC) (GLushort factor);
typedef void (APIENTRY * glGlobalAlphaFactoruiSUNPROC) (GLuint factor);

extern glGlobalAlphaFactorbSUNPROC glGlobalAlphaFactorbSUN;
extern glGlobalAlphaFactorsSUNPROC glGlobalAlphaFactorsSUN;
extern glGlobalAlphaFactoriSUNPROC glGlobalAlphaFactoriSUN;
extern glGlobalAlphaFactorfSUNPROC glGlobalAlphaFactorfSUN;
extern glGlobalAlphaFactordSUNPROC glGlobalAlphaFactordSUN;
extern glGlobalAlphaFactorubSUNPROC glGlobalAlphaFactorubSUN;
extern glGlobalAlphaFactorusSUNPROC glGlobalAlphaFactorusSUN;
extern glGlobalAlphaFactoruiSUNPROC glGlobalAlphaFactoruiSUN;

#endif /* GL_SUN_global_alpha */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_MESH_ARRAY--------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_mesh_array
#define GL_SUN_mesh_array 1

#define GL_QUAD_MESH_SUN										0x8614
#define GL_TRIANGLE_MESH_SUN									0x8615

typedef void (APIENTRY * glDrawMeshArraysSUNPROC) (GLenum mode, GLint first, GLsizei count, GLsizei width);

extern glDrawMeshArraysSUNPROC glDrawMeshArraysSUN;

#endif /* GL_SUN_mesh_array */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_MULTI_DRAW_ARRAYS-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_multi_draw_arrays
#define GL_SUN_multi_draw_arrays 1

typedef void (APIENTRY * glMultidrawArraysSUNPROC) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
typedef void (APIENTRY * glMultidrawElementsSUNPROC) (GLenum mode, GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);

extern glMultidrawArraysSUNPROC glMultidrawArraysSUN;
extern glMultidrawElementsSUNPROC glMultidrawElementsSUN;

#endif /* GL_SUN_multi_draw_arrays */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_SLICE_ACCUM-------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_slice_accum
#define GL_SUN_slice_accum 1

#define GL_SLICE_ACCUM_SUN										0x85CC

#endif /* GL_SUN_slice_accum */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_TRIANGLE_LIST-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_triangle_list
#define GL_SUN_triangle_list 1

#define GL_RESTART_SUN											0x0001
#define GL_REPLACE_MIDDLE_SUN									0x0002
#define GL_REPLACE_OLDEST_SUN									0x0003
#define GL_TRIANGLE_LIST_SUN									0x81D7
#define GL_REPLACEMENT_CODE_SUN									0x81D8
#define GL_REPLACEMENT_CODE_ARRAY_SUN							0x85C0
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN						0x85C1
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN					0x85C2
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN					0x85C3
#define GL_R1UI_V3F_SUN											0x85C4
#define GL_R1UI_C4UB_V3F_SUN									0x85C5
#define GL_R1UI_C3F_V3F_SUN										0x85C6
#define GL_R1UI_N3F_V3F_SUN										0x85C7
#define GL_R1UI_C4F_N3F_V3F_SUN									0x85C8
#define GL_R1UI_T2F_V3F_SUN										0x85C9
#define GL_R1UI_T2F_N3F_V3F_SUN									0x85CA
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN								0x85CB

typedef void (APIENTRY * glReplacementCodeuiSUNPROC) (GLuint code);
typedef void (APIENTRY * glReplacementCodeusSUNPROC) (GLushort code);
typedef void (APIENTRY * glReplacementCodeubSUNPROC) (GLubyte code);
typedef void (APIENTRY * glReplacementCodeuivSUNPROC) (const GLuint *code);
typedef void (APIENTRY * glReplacementCodeusvSUNPROC) (const GLushort *code);
typedef void (APIENTRY * glReplacementCodeubvSUNPROC) (const GLubyte *code);
typedef void (APIENTRY * glReplacementCodePointerSUNPROC) (GLenum type, GLsizei stride, const GLvoid* *pointer);

extern glReplacementCodeuiSUNPROC glReplacementCodeuiSUN;
extern glReplacementCodeusSUNPROC glReplacementCodeusSUN;
extern glReplacementCodeubSUNPROC glReplacementCodeubSUN;
extern glReplacementCodeuivSUNPROC glReplacementCodeuivSUN;
extern glReplacementCodeusvSUNPROC glReplacementCodeusvSUN;
extern glReplacementCodeubvSUNPROC glReplacementCodeubvSUN;
extern glReplacementCodePointerSUNPROC glReplacementCodePointerSUN;

#endif /* GL_SUN_triangle_list */

/*-------------------------------------------------------------------*/
/*------------GL_SUN_VERTEX------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_SUN_vertex
#define GL_SUN_vertex 1

typedef void (APIENTRY * glColor4ubVertex2fSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
typedef void (APIENTRY * glColor4ubVertex2fvSUNPROC) (const GLubyte *c, const GLfloat *v);
typedef void (APIENTRY * glColor4ubVertex3fSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glColor4ubVertex3fvSUNPROC) (const GLubyte *c, const GLfloat *v);
typedef void (APIENTRY * glColor3fVertex3fSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glColor3fVertex3fvSUNPROC) (const GLfloat *c, const GLfloat *v);
typedef void (APIENTRY * glNormal3fVertex3fSUNPROC) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glNormal3fVertex3fvSUNPROC) (const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glColor4fNormal3fVertex3fSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glColor4fNormal3fVertex3fvSUNPROC) (const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glTexCoord2fVertex3fSUNPROC) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glTexCoord2fVertex3fvSUNPROC) (const GLfloat *tc, const GLfloat *v);
typedef void (APIENTRY * glTexCoord4fVertex4fSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glTexCoord4fVertex4fvSUNPROC) (const GLfloat *tc, const GLfloat *v);
typedef void (APIENTRY * glTexCoord2fColor4ubVertex3fSUNPROC) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glTexCoord2fColor4ubVertex3fvSUNPROC) (const GLfloat *tc, const GLubyte *c, const GLfloat *v);
typedef void (APIENTRY * glTexCoord2fColor3fVertex3fSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glTexCoord2fColor3fVertex3fvSUNPROC) (const GLfloat *tc, const GLfloat *c, const GLfloat *v);
typedef void (APIENTRY * glTexCoord2fNormal3fVertex3fSUNPROC) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glTexCoord2fNormal3fVertex3fvSUNPROC) (const GLfloat *tc, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glTexCoord2fColor4fNormal3fVertex3fSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glTexCoord2fColor4fNormal3fVertex3fvSUNPROC) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glTexCoord4fColor4fNormal3fVertex4fSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glTexCoord4fColor4fNormal3fVertex4fvSUNPROC) (const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiVertex3fSUNPROC) (GLuint rc, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiColor4ubVertex3fSUNPROC) (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiColor4ubVertex3fvSUNPROC) (const GLuint *rc, const GLubyte *c, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiColor3fVertex3fSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiColor3fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *c, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiNormal3fVertex3fSUNPROC) (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiNormal3fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiColor4fNormal3fVertex3fSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiColor4fNormal3fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fVertex3fSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *tc, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fNormal3fVertex3fSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUNPROC) (const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);

extern glColor4ubVertex2fSUNPROC glColor4ubVertex2fSUN;
extern glColor4ubVertex2fvSUNPROC glColor4ubVertex2fvSUN;
extern glColor4ubVertex3fSUNPROC glColor4ubVertex3fSUN;
extern glColor4ubVertex3fvSUNPROC glColor4ubVertex3fvSUN;
extern glColor3fVertex3fSUNPROC glColor3fVertex3fSUN;
extern glColor3fVertex3fvSUNPROC glColor3fVertex3fvSUN;
extern glNormal3fVertex3fSUNPROC glNormal3fVertex3fSUN;
extern glNormal3fVertex3fvSUNPROC glNormal3fVertex3fvSUN;
extern glColor4fNormal3fVertex3fSUNPROC glColor4fNormal3fVertex3fSUN;
extern glColor4fNormal3fVertex3fvSUNPROC glColor4fNormal3fVertex3fvSUN;
extern glTexCoord2fVertex3fSUNPROC glTexCoord2fVertex3fSUN;
extern glTexCoord2fVertex3fvSUNPROC glTexCoord2fVertex3fvSUN;
extern glTexCoord4fVertex4fSUNPROC glTexCoord4fVertex4fSUN;
extern glTexCoord4fVertex4fvSUNPROC glTexCoord4fVertex4fvSUN;
extern glTexCoord2fColor4ubVertex3fSUNPROC glTexCoord2fColor4ubVertex3fSUN;
extern glTexCoord2fColor4ubVertex3fvSUNPROC glTexCoord2fColor4ubVertex3fvSUN;
extern glTexCoord2fColor3fVertex3fSUNPROC glTexCoord2fColor3fVertex3fSUN;
extern glTexCoord2fColor3fVertex3fvSUNPROC glTexCoord2fColor3fVertex3fvSUN;
extern glTexCoord2fNormal3fVertex3fSUNPROC glTexCoord2fNormal3fVertex3fSUN;
extern glTexCoord2fNormal3fVertex3fvSUNPROC glTexCoord2fNormal3fVertex3fvSUN;
extern glTexCoord2fColor4fNormal3fVertex3fSUNPROC glTexCoord2fColor4fNormal3fVertex3fSUN;
extern glTexCoord2fColor4fNormal3fVertex3fvSUNPROC glTexCoord2fColor4fNormal3fVertex3fvSUN;
extern glTexCoord4fColor4fNormal3fVertex4fSUNPROC glTexCoord4fColor4fNormal3fVertex4fSUN;
extern glTexCoord4fColor4fNormal3fVertex4fvSUNPROC glTexCoord4fColor4fNormal3fVertex4fvSUN;
extern glReplacementCodeuiVertex3fSUNPROC glReplacementCodeuiVertex3fSUN;
extern glReplacementCodeuiVertex3fvSUNPROC glReplacementCodeuiVertex3fvSUN;
extern glReplacementCodeuiColor4ubVertex3fSUNPROC glReplacementCodeuiColor4ubVertex3fSUN;
extern glReplacementCodeuiColor4ubVertex3fvSUNPROC glReplacementCodeuiColor4ubVertex3fvSUN;
extern glReplacementCodeuiColor3fVertex3fSUNPROC glReplacementCodeuiColor3fVertex3fSUN;
extern glReplacementCodeuiColor3fVertex3fvSUNPROC glReplacementCodeuiColor3fVertex3fvSUN;
extern glReplacementCodeuiNormal3fVertex3fSUNPROC glReplacementCodeuiNormal3fVertex3fSUN;
extern glReplacementCodeuiNormal3fVertex3fvSUNPROC glReplacementCodeuiNormal3fVertex3fvSUN;
extern glReplacementCodeuiColor4fNormal3fVertex3fSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fSUN;
extern glReplacementCodeuiColor4fNormal3fVertex3fvSUNPROC glReplacementCodeuiColor4fNormal3fVertex3fvSUN;
extern glReplacementCodeuiTexCoord2fVertex3fSUNPROC glReplacementCodeuiTexCoord2fVertex3fSUN;
extern glReplacementCodeuiTexCoord2fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fVertex3fvSUN;
extern glReplacementCodeuiTexCoord2fNormal3fVertex3fSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;
extern glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;
extern glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;
extern glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUNPROC glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;

#endif /* GL_SUN_vertex */

/*-------------------------------------------------------------------*/
/*------------GL_WIN_PHONG_SHADING-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading 1

#define GL_PHONG_WIN											0x80EA
#define GL_PHONG_HINT_WIN										0x80EB

#endif /* GL_WIN_phong_shading */

/*-------------------------------------------------------------------*/
/*------------GL_WIN_SPECULAR_FOG------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_WIN_specular_fog
#define GL_WIN_specular_fog 1

#define GL_FOG_SPECULAR_TEXTURE_WIN								0x80EC

#endif /* GL_WIN_specular_fog */

/*-------------------------------------------------------------------*/
/*------------GL_WIN_SWAP_HINT---------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef GL_WIN_swap_hint
#define GL_WIN_swap_hint 1

typedef void (APIENTRY * glAddSwapHintRectWINPROC) (GLint x, GLint y, GLsizei width, GLsizei height);

extern glAddSwapHintRectWINPROC glAddSwapHintRectWIN;

#endif /* GL_WIN_swap_hint */

/*-------------------------------------------------------------------*/
/*------------END GL EXTENSIONS--------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*------------WGL EXTENSIONS HERE------------------------------------*/
/*-------------------------------------------------------------------*/

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

/*-------------------------------------------------------------------*/
/*------------WGL_3DFX_MULTISAMPLE-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_3DFX_multisample
#define WGL_3DFX_multisample 1

#define WGL_SAMPLE_BUFFERS_3DFX									0x2060
#define WGL_SAMPLES_3DFX										0x2061

#endif /* WGL_3DFX_multisample */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_BUFFER_REGION----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_buffer_region
#define WGL_ARB_buffer_region 1

#define WGL_FRONT_COLOR_BUFFER_BIT_ARB							0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB							0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB								0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB								0x00000008

typedef HANDLE (APIENTRY * wglCreateBufferRegionARBPROC) (HDC hDC, int iLayerPlane, UINT uType);
typedef VOID (APIENTRY * wglDeleteBufferRegionARBPROC) (HANDLE hRegion);
typedef BOOL (APIENTRY * wglSaveBufferRegionARBPROC) (HANDLE hRegion, int x, int y, int width, int height);
typedef BOOL (APIENTRY * wglRestoreBufferRegionARBPROC) (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);

extern wglCreateBufferRegionARBPROC wglCreateBufferRegionARB;
extern wglDeleteBufferRegionARBPROC wglDeleteBufferRegionARB;
extern wglSaveBufferRegionARBPROC wglSaveBufferRegionARB;
extern wglRestoreBufferRegionARBPROC wglRestoreBufferRegionARB;

#endif /* WGL_ARB_buffer_region */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_EXTENSION_STRING-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string 1

typedef const char* (APIENTRY * wglGetExtensionsStringARBPROC) (HDC hdc);

extern wglGetExtensionsStringARBPROC wglGetExtensionsStringARB;

#endif /* WGL_ARB_extensions_string */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_MAKE_CURRENT_READ------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_make_current_read
#define WGL_ARB_make_current_read 1

#define ERROR_INVALID_PIXEL_TYPE_ARB                            0x2043
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB	                0x2054

typedef BOOL (APIENTRY * wglMakeContextCurrentARBPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
typedef HDC (APIENTRY * wglGetCurrentReadDCARBPROC) (void);

extern wglMakeContextCurrentARBPROC wglMakeContextCurrentARB;
extern wglGetCurrentReadDCARBPROC wglGetCurrentReadDCARB;

#endif /* WGL_ARB_make_current_read */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_MULTISAMPLE------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_multisample
#define WGL_ARB_multisample 1

#define WGL_SAMPLE_BUFFERS_ARB                                  0x2041
#define WGL_SAMPLES_ARB                                         0x2042

#endif /* WGL_ARB_multisample */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_PBUFFER----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer 1

#define WGL_DRAW_TO_PBUFFER_ARB                                 0x202D
#define WGL_DRAW_TO_PBUFFER_ARB                                 0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB                              0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB                               0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB                              0x2030
#define WGL_PBUFFER_LARGEST_ARB                                 0x2033
#define WGL_PBUFFER_WIDTH_ARB                                   0x2034
#define WGL_PBUFFER_HEIGHT_ARB                                  0x2035
#define WGL_PBUFFER_LOST_ARB                                    0x2036

DECLARE_HANDLE(HPBUFFERARB);

typedef HPBUFFERARB (APIENTRY * wglCreatePbufferARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (APIENTRY * wglGetPbufferDCARBPROC) (HPBUFFERARB hPbuffer);
typedef int (APIENTRY * wglReleasePbufferDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);
typedef BOOL (APIENTRY * wglDestroyPbufferARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL (APIENTRY * wglQueryPbufferARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

extern wglCreatePbufferARBPROC wglCreatePbufferARB;
extern wglGetPbufferDCARBPROC wglGetPbufferDCARB;
extern wglReleasePbufferDCARBPROC wglReleasePbufferDCARB;
extern wglDestroyPbufferARBPROC wglDestroyPbufferARB;
extern wglQueryPbufferARBPROC wglQueryPbufferARB;

#endif /* WGL_ARB_pbuffer */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_PIXEL_FORMAT-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1

#define WGL_NUMBER_PIXEL_FORMATS_ARB                            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                                  0x2002
#define WGL_ACCELERATION_ARB                                    0x2003
#define WGL_NEED_PALETTE_ARB                                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB                             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB                              0x2006
#define WGL_SWAP_METHOD_ARB                                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                                0x2009
#define WGL_TRANSPARENT_ARB                                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB                           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB                         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB                          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB                         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB                         0x203B
#define WGL_SHARE_DEPTH_ARB                                     0x200C
#define WGL_SHARE_STENCIL_ARB                                   0x200D
#define WGL_SHARE_ACCUM_ARB                                     0x200E
#define WGL_SUPPORT_GDI_ARB                                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                                   0x2011
#define WGL_STEREO_ARB                                          0x2012
#define WGL_PIXEL_TYPE_ARB                                      0x2013
#define WGL_COLOR_BITS_ARB                                      0x2014
#define WGL_RED_BITS_ARB                                        0x2015
#define WGL_RED_SHIFT_ARB                                       0x2016
#define WGL_GREEN_BITS_ARB                                      0x2017
#define WGL_GREEN_SHIFT_ARB                                     0x2018
#define WGL_BLUE_BITS_ARB                                       0x2019
#define WGL_BLUE_SHIFT_ARB                                      0x201A
#define WGL_ALPHA_BITS_ARB                                      0x201B
#define WGL_ALPHA_SHIFT_ARB                                     0x201C
#define WGL_ACCUM_BITS_ARB                                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                                0x2021
#define WGL_DEPTH_BITS_ARB                                      0x2022
#define WGL_STENCIL_BITS_ARB                                    0x2023
#define WGL_AUX_BUFFERS_ARB                                     0x2024
#define WGL_NO_ACCELERATION_ARB                                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB                            0x2026
#define WGL_FULL_ACCELERATION_ARB                               0x2027
#define WGL_SWAP_EXCHANGE_ARB                                   0x2028
#define WGL_SWAP_COPY_ARB                                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                                  0x202A
#define WGL_TYPE_RGBA_ARB                                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                                 0x202C

typedef BOOL (APIENTRY * wglGetPixelFormatAttribivARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (APIENTRY * wglGetPixelFormatAttribfvARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (APIENTRY * wglChoosePixelFormatARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

extern wglGetPixelFormatAttribivARBPROC wglGetPixelFormatAttribivARB;
extern wglGetPixelFormatAttribfvARBPROC wglGetPixelFormatAttribfvARB;
extern wglChoosePixelFormatARBPROC wglChoosePixelFormatARB;

#endif /* WGL_ARB_pixel_format */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_RENDER_TEXTURE---------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_render_texture
#define WGL_ARB_render_texture 1

#define WGL_BIND_TO_TEXTURE_RGB_ARB                             0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB                            0x2071
#define WGL_TEXTURE_FORMAT_ARB                                  0x2072
#define WGL_TEXTURE_TARGET_ARB                                  0x2073
#define WGL_MIPMAP_TEXTURE_ARB                                  0x2074
#define WGL_TEXTURE_RGB_ARB                                     0x2075
#define WGL_TEXTURE_RGBA_ARB                                    0x2076
#define WGL_NO_TEXTURE_ARB                                      0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB                                0x2078
#define WGL_TEXTURE_1D_ARB                                      0x2079
#define WGL_TEXTURE_2D_ARB                                      0x207A
#define WGL_NO_TEXTURE_ARB                                      0x2077
#define WGL_MIPMAP_LEVEL_ARB                                    0x207B
#define WGL_CUBE_MAP_FACE_ARB                                   0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB                     0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB                     0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB                     0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB                     0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB                     0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB                     0x2082
#define WGL_FRONT_LEFT_ARB                                      0x2083
#define WGL_FRONT_RIGHT_ARB                                     0x2084
#define WGL_BACK_LEFT_ARB                                       0x2085
#define WGL_BACK_RIGHT_ARB                                      0x2086
#define WGL_AUX0_ARB                                            0x2087
#define WGL_AUX1_ARB                                            0x2088
#define WGL_AUX2_ARB                                            0x2089
#define WGL_AUX3_ARB                                            0x208A
#define WGL_AUX4_ARB                                            0x208B
#define WGL_AUX5_ARB                                            0x208C
#define WGL_AUX6_ARB                                            0x208D
#define WGL_AUX7_ARB                                            0x208E
#define WGL_AUX8_ARB                                            0x208F
#define WGL_AUX9_ARB                                            0x2090

typedef BOOL (APIENTRY * wglBindTexImageARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (APIENTRY * wglReleaseTexImageARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (APIENTRY * wglSetPbufferAttribARBPROC) (HPBUFFERARB hPbuffer, const int *piAttribList);

extern wglBindTexImageARBPROC wglBindTexImageARB;
extern wglReleaseTexImageARBPROC wglReleaseTexImageARB;
extern wglSetPbufferAttribARBPROC wglSetPbufferAttribARB;

#endif /*WGL_ARB_render_texture */

/*-------------------------------------------------------------------*/
/*------------WGL_ARB_PIXEL_FORMAT_FLOAT-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ARB_pixel_format_float
#define WGL_ARB_pixel_format_float 1

#define WGL_TYPE_RGBA_FLOAT_ARB        0x21A0

#endif /* WGL_ARB_pixel_format_float */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_DISPLAY_COLOR_TABLE----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_display_color_table
#define WGL_EXT_display_color_table 1

typedef BOOL (APIENTRY * wglBindTexImageARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (APIENTRY * wglCreateDisplayColorTableEXTPROC) (GLushort id);
typedef BOOL (APIENTRY * wglLoadDisplayColorTableEXTPROC) (const GLushort *table, GLuint length);
typedef BOOL (APIENTRY * wglBindDisplayColorTableEXTPROC) (GLushort id);
typedef VOID (APIENTRY * wglDestroyDisplayColorTableEXTPROC) (GLushort id);

extern wglCreateDisplayColorTableEXTPROC wglCreateDisplayColorTableEXT;
extern wglLoadDisplayColorTableEXTPROC wglLoadDisplayColorTableEXT;
extern wglBindDisplayColorTableEXTPROC wglBindDisplayColorTableEXT;
extern wglDestroyDisplayColorTableEXTPROC wglDestroyDisplayColorTableEXT;

#endif /* WGL_EXT_display_color_table */

/*-------------------------------------------------------------------*/
/*------------WGL_ATI_PIXEL_FORMAT_FLOAT-----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_ATI_pixel_format_float
#define WGL_ATI_pixel_format_float 1

#define GL_RGBA_FLOAT_MODE_ATI                                  0x8820
#define GL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI                      0x8835
#define WGL_TYPE_RGBA_FLOAT_ATI                                 0x21A0

#endif /* WGL_ATI_pixel_format_float */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_DEPTH_FLOAT------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_depth_float
#define WGL_EXT_depth_float 1

#define WGL_DEPTH_FLOAT_EXT							            0x2040

#endif /* WGL_EXT_depth_float */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_EXTENSION_STRING-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_extensions_string
#define WGL_EXT_extensions_string 1

typedef const char* (APIENTRY * wglGetExtensionsStringEXTPROC) ();

extern wglGetExtensionsStringEXTPROC wglGetExtensionsStringEXT;

#endif /* WGL_EXT_extensions_string */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_MAKE_CURRENT_READ------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_make_current_read
#define WGL_EXT_make_current_read 1

#define ERROR_INVALID_PIXEL_TYPE_EXT							0x2043

typedef BOOL (WINAPI * wglMakeContextCurrentEXTPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
typedef HDC (WINAPI * wglGetCurrentReadDCEXTPROC) (void);

extern wglMakeContextCurrentEXTPROC wglMakeContextCurrentEXT;
extern wglGetCurrentReadDCEXTPROC wglGetCurrentReadDCEXT;

#endif /* WGL_EXT_make_current_read */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_multisample------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_multisample
#define WGL_EXT_multisample 1

#define WGL_SAMPLE_BUFFERS_EXT									0x2041
#define WGL_SAMPLES_EXT											0x2042

#endif /* WGL_EXT_multisample */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_PBUFFER----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_pbuffer
#define WGL_EXT_pbuffer 1

#define WGL_DRAW_TO_PBUFFER_EXT       							0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT    							0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT     							0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT    							0x2030
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT 							0x2031
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT							0x2032
#define WGL_PBUFFER_LARGEST_EXT       							0x2033
#define WGL_PBUFFER_WIDTH_EXT         							0x2034
#define WGL_PBUFFER_HEIGHT_EXT        							0x2035

DECLARE_HANDLE(HPBUFFEREXT);

typedef HPBUFFEREXT (WINAPI * wglCreatePbufferEXTPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (WINAPI * wglGetPbufferDCEXTPROC) (HPBUFFEREXT hPbuffer);
typedef int (WINAPI * wglReleasePbufferDCEXTPROC) (HPBUFFEREXT hPbuffer, HDC hDC);
typedef BOOL (WINAPI * wglDestroyPbufferEXTPROC) (HPBUFFEREXT hPbuffer);
typedef BOOL (WINAPI * wglQueryPbufferEXTPROC) (HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);

extern wglCreatePbufferEXTPROC wglCreatePbufferEXT;
extern wglGetPbufferDCEXTPROC wglGetPbufferDCEXT;
extern wglReleasePbufferDCEXTPROC wglReleasePbufferDCEXT;
extern wglDestroyPbufferEXTPROC wglDestroyPbufferEXT;
extern wglQueryPbufferEXTPROC wglQueryPbufferEXT;

#endif /* WGL_EXT_pbuffer */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_PIXEL_FORMAT-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_pixel_format
#define WGL_EXT_pixel_format 1

#define WGL_NUMBER_PIXEL_FORMATS_EXT							0x2000
#define WGL_DRAW_TO_WINDOW_EXT      							0x2001
#define WGL_DRAW_TO_BITMAP_EXT      							0x2002
#define WGL_ACCELERATION_EXT        							0x2003
#define WGL_NEED_PALETTE_EXT        							0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT 							0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT  							0x2006
#define WGL_SWAP_METHOD_EXT         							0x2007
#define WGL_NUMBER_OVERLAYS_EXT     							0x2008
#define WGL_NUMBER_UNDERLAYS_EXT    							0x2009
#define WGL_TRANSPARENT_EXT         							0x200A
#define WGL_TRANSPARENT_VALUE_EXT   							0x200B
#define WGL_SHARE_DEPTH_EXT         							0x200C
#define WGL_SHARE_STENCIL_EXT       							0x200D
#define WGL_SHARE_ACCUM_EXT         							0x200E
#define WGL_SUPPORT_GDI_EXT         							0x200F
#define WGL_SUPPORT_OPENGL_EXT      							0x2010
#define WGL_DOUBLE_BUFFER_EXT       							0x2011
#define WGL_STEREO_EXT              							0x2012
#define WGL_PIXEL_TYPE_EXT          							0x2013
#define WGL_COLOR_BITS_EXT          							0x2014
#define WGL_RED_BITS_EXT            							0x2015
#define WGL_RED_SHIFT_EXT           							0x2016
#define WGL_GREEN_BITS_EXT          							0x2017
#define WGL_GREEN_SHIFT_EXT         							0x2018
#define WGL_BLUE_BITS_EXT           							0x2019
#define WGL_BLUE_SHIFT_EXT          							0x201A
#define WGL_ALPHA_BITS_EXT          							0x201B
#define WGL_ALPHA_SHIFT_EXT         							0x201C
#define WGL_ACCUM_BITS_EXT          							0x201D
#define WGL_ACCUM_RED_BITS_EXT      							0x201E
#define WGL_ACCUM_GREEN_BITS_EXT    							0x201F
#define WGL_ACCUM_BLUE_BITS_EXT     							0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT    							0x2021
#define WGL_DEPTH_BITS_EXT          							0x2022
#define WGL_STENCIL_BITS_EXT        							0x2023
#define WGL_AUX_BUFFERS_EXT         							0x2024
#define WGL_NO_ACCELERATION_EXT     							0x2025
#define WGL_GENERIC_ACCELERATION_EXT							0x2026
#define WGL_FULL_ACCELERATION_EXT   							0x2027
#define WGL_SWAP_EXCHANGE_EXT       							0x2028
#define WGL_SWAP_COPY_EXT           							0x2029
#define WGL_SWAP_UNDEFINED_EXT      							0x202A
#define WGL_TYPE_RGBA_EXT           							0x202B
#define WGL_TYPE_COLORINDEX_EXT     							0x202C

typedef BOOL (WINAPI * wglGetPixelFormatAttribivEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues);
typedef BOOL (WINAPI * wglGetPixelFormatAttribfvEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues);
typedef BOOL (WINAPI * wglChoosePixelFormatEXTPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

extern wglGetPixelFormatAttribivEXTPROC wglGetPixelFormatAttribivEXT;
extern wglGetPixelFormatAttribfvEXTPROC wglGetPixelFormatAttribfvEXT;
extern wglChoosePixelFormatEXTPROC wglChoosePixelFormatEXT;

#endif /* WGL_EXT_pixel_format */

/*-------------------------------------------------------------------*/
/*------------WGL_EXT_SWAP_CONTROL-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_EXT_swap_control
#define WGL_EXT_swap_control 1

typedef BOOL (APIENTRY * wglSwapIntervalEXTPROC) (int interval);
typedef int (APIENTRY * wglGetSwapIntervalEXTPROC) (void);

extern wglSwapIntervalEXTPROC wglSwapIntervalEXT;
extern wglGetSwapIntervalEXTPROC wglGetSwapIntervalEXT;

#endif /* WGL_EXT_swap_control */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_DIGITAL_VIDEO_CONTROL--------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_digital_video_control
#define WGL_I3D_digital_video_control 1

#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D			0x2050
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D				0x2051
#define WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D					0x2052
#define WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D					0x2053

typedef BOOL (WINAPI * wglGetDigitalVideoParametersI3DPROC) (HDC hDC, int iAttribute, int *piValue);
typedef BOOL (WINAPI * wglSetDigitalVideoParametersI3DPROC) (HDC hDC, int iAttribute, const int *piValue);

extern wglGetDigitalVideoParametersI3DPROC wglGetDigitalVideoParametersI3D;
extern wglSetDigitalVideoParametersI3DPROC wglSetDigitalVideoParametersI3D;

#endif /* WGL_I3D_digital_video_control */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_GAMMA------------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_gamma
#define WGL_I3D_gamma 1

#define WGL_GAMMA_TABLE_SIZE_I3D								0x204E
#define WGL_GAMMA_EXCLUDE_DESKTOP_I3D							0x204F

typedef BOOL (WINAPI * wglGetGammaTableParametersI3DPROC) (HDC hDC, int iAttribute, int *piValue);
typedef BOOL (WINAPI * wglSetGammaTableParametersI3DPROC) (HDC hDC, int iAttribute, const int *piValue);
typedef BOOL (WINAPI * wglGetGammaTableI3DPROC) (HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue);
typedef BOOL (WINAPI * wglSetGammaTableI3DPROC) (HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue);

extern wglGetGammaTableParametersI3DPROC wglGetGammaTableParametersI3D;
extern wglSetGammaTableParametersI3DPROC wglSetGammaTableParametersI3D;
extern wglGetGammaTableI3DPROC wglGetGammaTableI3D;
extern wglSetGammaTableI3DPROC wglSetGammaTableI3D;

#endif /* WGL_I3D_gamma */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_GENLOCK----------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_genlock
#define WGL_I3D_genlock 1

#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D						0x2044
#define WGL_GENLOCK_SOURCE_EXTENAL_SYNC_I3D						0x2045
#define WGL_GENLOCK_SOURCE_EXTENAL_FIELD_I3D					0x2046
#define WGL_GENLOCK_SOURCE_EXTENAL_TTL_I3D						0x2047
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D						0x2048
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D					0x2049
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D						0x204A
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D						0x204B
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D						0x204C

typedef BOOL (WINAPI * wglEnableGenlockI3DPROC) (HDC hDC);
typedef BOOL (WINAPI * wglDisableGenlockI3DPROC) (HDC hDC);
typedef BOOL (WINAPI * wglIsEnabledGenlockI3DPROC) (HDC hDC, BOOL *pFlag);
typedef BOOL (WINAPI * wglGenlockSourceI3DPROC) (HDC hDC, UINT uSource);
typedef BOOL (WINAPI * wglGetGenlockSourceI3DPROC) (HDC hDC, UINT *uSource);
typedef BOOL (WINAPI * wglGenlockSourceEdgeI3DPROC) (HDC hDC, UINT uEdge);
typedef BOOL (WINAPI * wglGetGenlockSourceEdgeI3DPROC) (HDC hDC, UINT *uEdge);
typedef BOOL (WINAPI * wglGenlockSampleRateI3DPROC) (HDC hDC, UINT uRate);
typedef BOOL (WINAPI * wglGetGenlockSampleRateI3DPROC) (HDC hDC, UINT *uRate);
typedef BOOL (WINAPI * wglGenlockSourceDelayI3DPROC) (HDC hDC, UINT uDelay);
typedef BOOL (WINAPI * wglGetGenlockSourceDelayI3DPROC) (HDC hDC, UINT *uDelay);
typedef BOOL (WINAPI * wglQueryGenlockMaxSourceDelayI3DPROC) (HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay);

extern wglEnableGenlockI3DPROC wglEnableGenlockI3D;
extern wglDisableGenlockI3DPROC wglDisableGenlockI3D;
extern wglIsEnabledGenlockI3DPROC wglIsEnabledGenlockI3D;
extern wglGenlockSourceI3DPROC wglGenlockSourceI3D;
extern wglGetGenlockSourceI3DPROC wglGetGenlockSourceI3D;
extern wglGenlockSourceEdgeI3DPROC wglGenlockSourceEdgeI3D;
extern wglGetGenlockSourceEdgeI3DPROC wglGetGenlockSourceEdgeI3D;
extern wglGenlockSampleRateI3DPROC wglGenlockSampleRateI3D;
extern wglGetGenlockSampleRateI3DPROC wglGetGenlockSampleRateI3D;
extern wglGenlockSourceDelayI3DPROC wglGenlockSourceDelayI3D;
extern wglGetGenlockSourceDelayI3DPROC wglGetGenlockSourceDelayI3D;
extern wglQueryGenlockMaxSourceDelayI3DPROC wglQueryGenlockMaxSourceDelayI3D;

#endif /* WGL_I3D_genlock */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_IMAGE_BUFFER-----------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_image_buffer
#define WGL_I3D_image_buffer 1

#define WGL_IMAGE_BUFFER_MIN_ACCESS_I3D							0x00000001
#define WGL_IMAGE_BUFFER_LOCK_I3D								0x00000002

typedef LPVOID (WINAPI * wglCreateImageBufferI3DPROC) (HDC hDC, DWORD dwSize, UINT uFlags);
typedef BOOL (WINAPI * wglDestroyImageBufferI3DPROC) (HDC hDC, LPVOID pAddress);
typedef BOOL (WINAPI * wglAssociateImageBufferEventsI3DPROC) (HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count);
typedef BOOL (WINAPI * wglReleaseImageBufferEventsI3DPROC) (HDC hDC, const LPVOID *pAddress, UINT count);

extern wglCreateImageBufferI3DPROC wglCreateImageBufferI3D;
extern wglDestroyImageBufferI3DPROC wglDestroyImageBufferI3D;
extern wglAssociateImageBufferEventsI3DPROC wglAssociateImageBufferEventsI3D;
extern wglReleaseImageBufferEventsI3DPROC wglReleaseImageBufferEventsI3D;

#endif /* WGL_I3D_image_buffer */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_SWAP_FRAME_LOCK--------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_swap_frame_lock
#define WGL_I3D_swap_frame_lock 1

typedef BOOL (WINAPI * wglEnableFrameLockI3DPROC) (void);
typedef BOOL (WINAPI * wglDisableFrameLockI3DPROC) (void);
typedef BOOL (WINAPI * wglIsEnabledFrameLockI3DPROC) (BOOL *pFlag);
typedef BOOL (WINAPI * wglQueryFrameLockMasterI3DPROC) (BOOL *pFlag);

extern wglEnableFrameLockI3DPROC wglEnableFrameLockI3D;
extern wglDisableFrameLockI3DPROC wglDisableFrameLockI3D;
extern wglIsEnabledFrameLockI3DPROC wglIsEnabledFrameLockI3D;
extern wglQueryFrameLockMasterI3DPROC wglQueryFrameLockMasterI3D;

#endif /* WGL_I3D_swap_frame_lock */

/*-------------------------------------------------------------------*/
/*------------WGL_I3D_SWAP_FRAME_USAGE-------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_I3D_swap_frame_usage
#define WGL_I3D_swap_frame_usage 1

typedef BOOL (WINAPI * wglGetFrameUsageI3DPROC) (float *pUsage);
typedef BOOL (WINAPI * wglBeginFrameTrackingI3DPROC) (void);
typedef BOOL (WINAPI * wglEndFrameTrackingI3DPROC) (void);
typedef BOOL (WINAPI * wglQueryFrameTrackingI3DPROC) (DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage);

extern wglGetFrameUsageI3DPROC wglGetFrameUsageI3D;
extern wglBeginFrameTrackingI3DPROC wglBeginFrameTrackingI3D;
extern wglEndFrameTrackingI3DPROC wglEndFrameTrackingI3D;
extern wglQueryFrameTrackingI3DPROC wglQueryFrameTrackingI3D;

#endif /* WGL_I3D_swap_frame_usage */

/*-------------------------------------------------------------------*/
/*------------WGL_NV_FLOAT_BUFFER------------------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_NV_float_buffer
#define WGL_NV_float_buffer 1

#define WGL_FLOAT_COMPONENTS_NV									0x20B0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV				0x20B1
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV				0x20B2
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV				0x20B3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV				0x20B4
#define WGL_TEXTURE_FLOAT_R_NV									0x20B5
#define WGL_TEXTURE_FLOAT_RG_NV									0x20B6
#define WGL_TEXTURE_FLOAT_RGB_NV								0x20B7
#define WGL_TEXTURE_FLOAT_RGBA_NV								0x20B8

#endif /* WGL_NV_float_buffer */

/*-------------------------------------------------------------------*/
/*------------WGL_NV_RENDER_DEPTH_TEXTURE----------------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_NV_render_depth_texture
#define WGL_NV_render_depth_texture 1

#define WGL_BIND_TO_TEXTURE_DEPTH_NV                            0x20A3
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV                  0x20A4
#define WGL_DEPTH_TEXTURE_FORMAT_NV                             0x20A5
#define WGL_TEXTURE_DEPTH_COMPONENT_NV                          0x20A6
#define WGL_NO_TEXTURE_ARB                                      0x2077
#define WGL_DEPTH_COMPONENT_NV                                  0x20A7

#endif /* WGL_NV_render_depth_texture */

/*-------------------------------------------------------------------*/
/*------------WGL_NV_RENDER_TEXTURE_RECTANGLE-----------------------*/
/*-------------------------------------------------------------------*/

#ifndef WGL_NV_render_texture_rectangle 
#define WGL_NV_render_texture_rectangle 1

#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV                    0x20A0
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV                   0x20A1
#define WGL_TEXTURE_RECTANGLE_NV                                0x20A2

#endif /* WGL_NV_render_texture_rectangle */

/*-------------------------------------------------------------------*/
/*------------END WGL EXTENSIONS-------------------------------------*/
/*-------------------------------------------------------------------*/

#endif /* WIN32 */

/* helper stuff */

/* I use int here because C does not know bool */

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)

struct WGLExtensionTypes
{
	bool WGL3DFX_multisample;
	bool ARB_buffer_region;
	bool ARB_extensions_string;
	bool ARB_make_current_read;
	bool ARB_multisample;
	bool ARB_pbuffer;
	bool ARB_pixel_format;
	bool ARB_render_texture;
	bool ATI_pixel_format_float;
	bool EXT_depth_float;
	bool EXT_display_color_table;
	bool EXT_extensions_string;
	bool EXT_make_current_read;
	bool EXT_multisample;
	bool EXT_pbuffer;
	bool EXT_pixel_format;
	bool EXT_swap_control;
	bool I3D_digital_video_control;
	bool I3D_gamma;
	bool I3D_genlock;
	bool I3D_image_buffer;
	bool I3D_swap_frame_lock;
	bool I3D_swap_frame_usage;
	bool NV_float_buffer;
	bool NV_render_depth_texture;
	bool NV_render_texture_rectangle;
};

#else /* No WIN32 */

struct GLXExtensionTypes
{

};

#endif /* WIN32 */

struct ExtensionTypes
{
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) /* WGL extensions */   
    struct WGLExtensionTypes wgl;
#else /* no WIN32 */
/*    struct GLXExtensionTypes glx;*/
#endif /* WIN32 */
	bool APPLE_client_storage;
	bool APPLE_element_array;
	bool APPLE_fence;
	bool APPLE_specular_vector;
	bool APPLE_transform_hint;
	bool APPLE_vertex_array_range;
	bool APPLE_ycbcr_422;
	bool ARB_color_buffer_float;
	bool ARB_depth_texture;
	bool ARB_draw_buffers;
	bool ARB_fragment_program;
	bool ARB_fragment_program_shadow;
	bool ARB_fragment_shader;
	bool ARB_imaging;
	bool ARB_matrix_palette;
	bool ARB_multisample;
	bool ARB_multitexture;
	bool ARB_occlusion_query;
	bool ARB_point_parameters;
	bool ARB_point_sprite;
	bool ARB_shader_objects;
	bool ARB_shading_language_100;
	bool ARB_shadow;
	bool ARB_shadow_ambient;
	bool ARB_texture_border_clamp;
	bool ARB_texture_compression;
	bool ARB_texture_cube_map;
	bool ARB_texture_env_add;
	bool ARB_texture_env_combine;
	bool ARB_texture_env_crossbar;
	bool ARB_texture_env_dot3;
	bool ARB_texture_mirrored_repeat;
	bool ARB_texture_non_power_of_two;
	bool ARB_transpose_matrix;
	bool ARB_vertex_blend;
	bool ARB_vertex_buffer_object;
	bool ARB_vertex_program;
	bool ARB_vertex_shader;
	bool ARB_window_pos;
	bool ATIX_point_sprites;
	bool ATIX_texture_env_combine3;
	bool ATIX_texture_env_route;
	bool ATIX_vertex_shader_output_point_size;
	bool ATI_draw_buffers;
	bool ATI_element_array;
	bool ATI_envmap_bumpmap;
	bool ATI_fragment_shader;
	bool ATI_map_object_buffer;
	bool ATI_pn_triangles;
	bool ATI_point_cull_mode;
	bool ATI_separate_stencil;
	bool ATI_text_fragment_shader;
	bool ATI_texture_env_combine3;
	bool ATI_texture_compression_3dc;
	bool ATI_texture_float;
	bool ATI_texture_mirror_once;
	bool ATI_vertex_array_object;
	bool ATI_vertex_attrib_array_object;
	bool ATI_vertex_streams;
	bool EXT_422_pixels;
	bool EXT_abgr;
	bool EXT_bgra;
	bool EXT_blend_color;
	bool EXT_blend_equation_separate;
	bool EXT_blend_func_separate;
	bool EXT_blend_logic_op;
	bool EXT_blend_minmax;
	bool EXT_blend_subtract;
	bool EXT_clip_volume_hint;
	bool EXT_cmyka;
	bool EXT_color_matrix;
	bool EXT_color_subtable;
	bool EXT_compiled_vertex_array;
	bool EXT_convolution;
	bool EXT_coordinate_frame;
	bool EXT_copy_texture;
	bool EXT_cull_vertex;
	bool EXT_depth_bounds_test;
	bool EXT_draw_range_elements;
	bool EXT_fog_coord;
	bool EXT_framebuffer_object;
	bool EXT_histogram;
	bool EXT_index_array_formats;
	bool EXT_index_func;
	bool EXT_index_material;
	bool EXT_index_texture;
	bool EXT_light_texture;
	bool EXT_misc_attribute;
	bool EXT_multi_draw_arrays;
	bool EXT_multisample;
	bool EXT_packed_pixels;
	bool EXT_paletted_texture;
	bool EXT_pixel_buffer_object;
	bool EXT_pixel_transform;
	bool EXT_pixel_transform_color_table;
	bool EXT_point_parameters;
	bool EXT_polygon_offset;
	bool EXT_rescale_normal;
	bool EXT_secondary_color;
	bool EXT_separate_specular_color;
	bool EXT_shadow_funcs;
	bool EXT_shared_texture_palette;
	bool EXT_stencil_two_side;
	bool EXT_stencil_wrap;
	bool EXT_subtexture;
	bool EXT_texgen_reflection;
	bool EXT_texture3D;
	bool EXT_texture;
	bool EXT_texture_compression_s3tc;
	bool EXT_texture_cube_map;
	bool EXT_texture_edge_clamp;
	bool EXT_texture_env_add;
	bool EXT_texture_env_combine;
	bool EXT_texture_env_dot3;
	bool EXT_texture_filter_anisotropic;
	bool EXT_texture_lod;
	bool EXT_texture_lod_bias;
	bool EXT_texture_object;
	bool EXT_texture_perturb_normal;
	bool EXT_texture_rectangle;
	bool EXT_vertex_array;
	bool EXT_vertex_shader;
	bool EXT_vertex_weighting;
	bool FfdMaskSGIX;
	bool GL3DFX_multisample;
	bool GL3DFX_texture_compression_FXT1;
	bool HP_convolution_border_modes;
	bool HP_image_transform;
	bool HP_occlusion_test;
	bool HP_texture_lighting;
	bool IBM_cull_vertex;
	bool IBM_multimode_draw_arrays;
	bool IBM_rasterpos_clip;
	bool IBM_texture_mirrored_repeat;
	bool IBM_vertex_array_lists;
	bool INGR_color_clamp;
	bool INGR_interlace_read;
	bool INGR_palette_buffer;
	bool INTEL_parallel_arrays;
	bool INTEL_texture_scissor;
	bool KTX_buffer_region;
	bool MESA_pack_invert;
	bool MESA_resize_buffers;
	bool MESA_window_pos;
	bool MESA_ycbcr_texture;
	bool NVX_ycrcb;
	bool NV_blend_square;
	bool NV_copy_depth_to_color;
	bool NV_depth_clamp;
	bool NV_element_array;
	bool NV_evaluators;
	bool NV_fence;
	bool NV_float_buffer;
	bool NV_fog_distance;
	bool NV_fragment_program2;
	bool NV_fragment_program;
	bool NV_fragment_program_option;
	bool NV_half_float;
	bool NV_light_max_exponent;
	bool NV_multisample_filter_hint;
	bool NV_occlusion_query;
	bool NV_packed_depth_stencil;
	bool NV_pixel_data_range;
	bool NV_point_sprite;
	bool NV_primitive_restart;
	bool NV_register_combiners2;
	bool NV_register_combiners;
	bool NV_texgen_emboss;
	bool NV_texgen_reflection;
	bool NV_texture_compression_vtc;
	bool NV_texture_env_combine4;
	bool NV_texture_expand_normal;
	bool NV_texture_rectangle;
	bool NV_texture_shader2;
	bool NV_texture_shader3;
	bool NV_texture_shader;
	bool NV_vertex_array_range2;
	bool NV_vertex_array_range;
	bool NV_vertex_program1_1;
	bool NV_vertex_program2;
	bool NV_vertex_program2_option;
	bool NV_vertex_program3;
	bool NV_vertex_program;
	bool OML_interlace;
	bool OML_resample;
	bool OML_subsample;
	bool PGI_misc_hints;
	bool PGI_vertex_hints;
	bool REND_screen_coordinates;
	bool S3_s3tc;
	bool SGIS_detail_texture;
	bool SGIS_fog_function;
	bool SGIS_generate_mipmap;
	bool SGIS_multisample;
	bool SGIS_multitexture;
	bool SGIS_pixel_texture;
	bool SGIS_point_line_texgen;
	bool SGIS_point_parameters;
	bool SGIS_sharpen_texture;
	bool SGIS_texture4D;
	bool SGIS_texture_border_clamp;
	bool SGIS_texture_color_mask;
	bool SGIS_texture_edge_clamp;
	bool SGIS_texture_filter4;
	bool SGIS_texture_lod;
	bool SGIS_texture_select;
	bool SGIX_async_histogram;
	bool SGIX_async_pixel;
	bool SGIX_asyncr;
	bool SGIX_blend_alpha_minmax;
	bool SGIX_calligraphic_fragment;
	bool SGIX_clipmap;
	bool SGIX_convolution_accuracy;
	bool SGIX_depth_texture;
	bool SGIX_flush_raster;
	bool SGIX_fog_offset;
	bool SGIX_fog_scale;
	bool SGIX_fragment_lighting;
	bool SGIX_framezoom;
	bool SGIX_impact_pixel_texture;
	bool SGIX_instruments;
	bool SGIX_interlace;
	bool SGIX_ir_instrument1;
	bool SGIX_list_priority;
	bool SGIX_pixel_texture;
	bool SGIX_pixel_tiles;
	bool SGIX_polynomial_ffd;
	bool SGIX_reference_plane;
	bool SGIX_resample;
	bool SGIX_scalebias_hint;
	bool SGIX_shadow;
	bool SGIX_shadow_ambient;
	bool SGIX_sprite;
	bool SGIX_subsample;
	bool SGIX_tag_sample_buffer;
	bool SGIX_texture_add_env;
	bool SGIX_texture_coordinate_clamp;
	bool SGIX_texture_lod_bias;
	bool SGIX_texture_multi_buffer;
	bool SGIX_texture_scale_bias;
	bool SGIX_vertex_preclip;
	bool SGIX_ycrcb;
	bool SGIX_ycrcb_subsample;
	bool SGIX_ycrcba;
	bool SGI_color_matrix;
	bool SGI_color_table;
	bool SGI_depth_pass_instrument;
	bool SGI_texture_color_table;
	bool SUNX_constant_data;
	bool SUN_convolution_border_modes;
	bool SUN_global_alpha;
	bool SUN_mesh_array;
	bool SUN_multi_draw_arrays;
	bool SUN_slice_accum;
	bool SUN_triangle_list;
	bool SUN_vertex;
	bool WIN_phong_shading;
	bool WIN_specular_fog;
	bool WIN_swap_hint;
    bool APPLE_vertex_array_object;

	bool OpenGL12;
	bool OpenGL13;
	bool OpenGL14;
	bool OpenGL15;
	bool OpenGL20;
};

extern struct ExtensionTypes extgl_Extensions;

extern struct ExtensionTypes SupportedExtensions; /* deprecated, please do not use */

/* initializes everything, call this right after the rc is created. the function returns 0 if successful */
int extgl_Initialize();

/* returns true if a given extension is supported by extgl - if an extension is supported by the ogl driver
but is not supported by extgl this will return false */
int extgl_ExtensionSupported(const char *name);

int glInitialize(); /* deprecated, please do not use */

//#if defined(DEBUG) | defined(_DEBUG)
	#ifdef WIN32
		void extgl_DisplayValues(bool bto_a_file = false);
	#endif // #ifdef WIN32
//#endif // #if defined(DEBUG) | defined(_DEBUG)


#ifdef __cplusplus
}
#endif

#endif /* __EXTGL_H__ */
