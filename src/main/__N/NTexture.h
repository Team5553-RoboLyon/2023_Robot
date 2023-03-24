#ifndef __NTEXTURE_H
#define __NTEXTURE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTexture.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "./GL/Ngl.h"
#include "./Image/NImage.h"
#include "./Containers/NNode.h"
#include "./Containers/NArray.h"
#include "NType.h"
#include "NFlags.h"
#include "NScreenPoint.h"
#include "NScreenRect.h"
#include "NRectf32.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures and enum										**
// ***************************************************************************************
// Texture Flags															// These flags correspond with the following NIMAGECHANNEL Flags (for Alpha Channel of course)
#define FLAG_NTEXTURE_HAS_ALPHA										BIT_0	// FLAG_NIMAGECHANNEL_EXIST									// Image channel exist
#define FLAG_NTEXTURE_ALPHA_IS_FLAT									BIT_1	// FLAG_NIMAGECHANNEL_IS_FLAT								// Image channel contains only one color (White only or Black only or one single gray only )
#define FLAG_NTEXTURE_ALPHA_CONTAINS_0								BIT_2	// FLAG_NIMAGECHANNEL_CONTAINS_0							// Image channel contains at least one pixel with a value of 0
#define FLAG_NTEXTURE_ALPHA_CONTAINS_255							BIT_3	// FLAG_NIMAGECHANNEL_CONTAINS_255							// Image channel contains at least one pixel with a value of 255 (or 31 for 16 bits image)
#define FLAG_NTEXTURE_ALPHA_CONTAINS_INTERMEDIATE_VALUE				BIT_4	// FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE			// Image channel contains at least one pixel with a value between 0 and 255 (or 31 ...)
#define FLAG_NTEXTURE_ALPHA_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES	BIT_5	// FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES	// Image channel contains multiple pixels with a value between 0 and 255 (or 31 ... )
#define FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE							BIT_6	// an alpha channel is effective if its image rendering contribution may be real.
//#define FLAG_NTEXTURE_xxx											BIT_7	// and cant be done otherwise. For example: a flat ALpha channel full of ZERO or full of 255
																			// is not considered as an effective Alpha... because we can obtain a similar result without any Alpha channel.
																			//		.The full of ZERO one will generate an invisible geometry. 
																			//		.The full of 255 one will generate a non-blended geometry, without Z writing																		
																			// Obviously, user can decide to use these "non effective" alpha channel anyway																			
#define MASK_NTEXTURE_ALPHA_CHANNEL_INFOS							0x7F	// BIT_0 to BIT_6 included
typedef struct
{
	GLint   InternalFormat;  
	GLuint  Width;            
	GLuint  Height;
	GLenum  Format;          // RGB, RGBA, LUMINANCE, LUMINANCE ALPHA
	GLenum	Type;
	GLvoid	*pImageData;    
}NTEXTUREGLINFO;

typedef struct 
{
	Nu8				Flags;			// 8 flags 
	Nu8				Available_Nu8;
	Nu16			ReferenceCount;	// Texture Reference Counter.
	
	Nchar			*pName;			// Should be unique, and should be filename without the "datapath" which is different by machine
	NVEC2s16		size;			// Texture Size in Pixel.
	
	GLint			MagFilter;
	GLint			MinFilter;
	GLint			WrapS;
	GLint			WrapT;
	GLuint			GL_Texture;	
}NTEXTURE;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Textures
NTEXTURE*					NSetupTexture( NTEXTURE *ptexture, const Nchar *ptexture_name, const NIMAGE *pimg );
NTEXTURE*					NCreateTexture( const Nchar *ptexture_name, const NIMAGE *pimg );
void						NClearTexture(NTEXTURE* pt);
void						NDeleteTexture(NTEXTURE* pt);


NTEXTURE*					NCreateTextureFromTga(const char* ptexture_name, const char* ptga_filename );
NTEXTURE*					NChangeTextureImageFromTga(NTEXTURE *ptexture, const Nchar* ptga_filename);
Nbool 						NGetTextureGLInfo( NTEXTUREGLINFO *ptexture_glinfo, const NIMAGE *pimg );
inline const NVEC2s16*	NGetTextureSizes16(const NTEXTURE *ptexture){return &ptexture->size;}
inline Nbool				NIsTextureAlphaEffective(const NTEXTURE *pt){if(ISFLAG_ON(pt->Flags,FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE))return NTRUE;else return NFALSE;}

//inline void					NSetTextureBank(NTEXTURE *ptexture, const Nu8 bank_id){ptexture->BankID = bank_id;}
inline void					NSetTextureGLFilters(NTEXTURE *ptexture,GLint minfilter_param,GLint magfilter_param){ptexture->MinFilter = minfilter_param; ptexture->MagFilter = magfilter_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter_param );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter_param );}
inline void					NSetTextureGLMinFilter(NTEXTURE *ptexture,GLint minfilter_param){ptexture->MinFilter = minfilter_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter_param );}
inline void					NSetTextureGLMagFilter(NTEXTURE *ptexture,GLint magfilter_param){ptexture->MagFilter = magfilter_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter_param );}

inline void					NSetTextureGLWrap(NTEXTURE *ptexture,GLint wraps_param,GLint wrapt_param){ptexture->WrapS = wraps_param; ptexture->WrapT = wrapt_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps_param );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt_param );}
inline void					NSetTextureGLWrapS(NTEXTURE *ptexture,GLint wraps_param){ptexture->WrapS = wraps_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps_param );}
inline void					NSetTextureGLWrapT(NTEXTURE *ptexture,GLint wrapt_param){ptexture->WrapT = wrapt_param; glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt_param );}

//inline void					NSetTextureGLParameter(NTEXTURE *ptexture,GLenum pname, GLint param){glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );glTexParameteri( GL_TEXTURE_2D, pname, param );}

inline void					NRegisterTextureReference(NTEXTURE *ptexture){NErrorIf(ptexture->ReferenceCount==NU16_MAX,NERROR_VALUE_OUTOFRANGE);ptexture->ReferenceCount++;}
inline void					NUnregisterTextureReference(NTEXTURE *ptexture){NErrorIf(ptexture->ReferenceCount==0,NERROR_VALUE_OUTOFRANGE);ptexture->ReferenceCount--;}
inline Nu16					NGetTextureReferenceCount(const NTEXTURE *ptexture){return ptexture->ReferenceCount;}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NTEXTURE_H 

