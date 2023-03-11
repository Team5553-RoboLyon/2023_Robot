#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_UPDATA_H
#define __NUT_EDITOR_ATXC_UPDATA_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Updata.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NEditor.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// TXUV UPDATA COMPONENTS:
//	All the UPDATA types send to user trough the Update Call back are made from these basic components
#define NUT_EDITOR_ATXC_TEXTURE				0x1
#define NUT_EDITOR_ATXC_TEXTURE_WRAP_S		0x2
#define NUT_EDITOR_ATXC_TEXTURE_WRAP_T		0x3
#define NUT_EDITOR_ATXC_TEXTURE_MIN_FILTER	0x4
#define NUT_EDITOR_ATXC_TEXTURE_MAG_FILTER	0x5

#define NUT_EDITOR_ATXC_TXCOFFSET			0x6
#define NUT_EDITOR_ATXC_TXCSCALE			0x7
#define NUT_EDITOR_ATXC_BLEND				0x8
#define NUT_EDITOR_ATXC_MATERIAL			0x9
#define NUT_EDITOR_ATXC_GEOMSPECS			0xA

#define NUT_EDITOR_ATXC_NULL				0x0 // Suppose a previous data existed and was replaced by a NULL pointer !
#define NUT_EDITOR_ATXC_NEW					0x1 // Suppose there are NO previous data and a new one is set !
#define NUT_EDITOR_ATXC_UPDATE				0x2	// Suppose a previous data existed and was replaced by a new one !

// TXUV UPDATA types
//	They work as Event types. User receive an Updata structure which is an union of different specific structure which describe
//	the current user modification (UV origin or scale,texture,blend,material,compatible geomspecs )
//  So User can analyze an Updata Type in different ways:
//		1) Simply like this:  switch(UPDATA.Type) to analyze all the different cases with their subtleties ( NEW_TEXTURE means current texture is NULL but incoming update is NON NULL, NEW_UVORIGIN means no UVS but now we need them with these values, UPDATE_UVORIGIN means new values for UVOrign... )   
//		2) or like this: switch( NLOWORD(UPDATA.Type) ) to retrieve the "WHAT IS UPDATED" component (Texture, UV Origin or UV Scale) (TEXTURE means, texture changes ! simple isn't it ?)
//		3) or like this: switch( NLOWORD(UPDATA.Type) ) to retrieve the "HOW IS UPDATED" component (NULL, NEW or UPDATE) ( give to user some precision about the change )

#define NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE			NMAKELONG(NUT_EDITOR_ATXC_TEXTURE,			NUT_EDITOR_ATXC_NULL) 
// #define NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE_WRAP_S	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_S,	NUT_EDITOR_ATXC_NULL)			// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE_WRAP_T	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_T,	NUT_EDITOR_ATXC_NULL)			// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE_MIN_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MIN_FILTER,	NUT_EDITOR_ATXC_NULL)	// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE_MAG_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MAG_FILTER,	NUT_EDITOR_ATXC_NULL)	// it should never happen !!!
//#define NUT_EDITOR_ATXC_UPDATA_NULL_TXCOFFSET		NMAKELONG(NUT_EDITOR_ATXC_TXCOFFSET,		NUT_EDITOR_ATXC_NULL)				// it should never happen !!!
//#define NUT_EDITOR_ATXC_UPDATA_NULL_TXCSCALE		NMAKELONG(NUT_EDITOR_ATXC_TXCSCALE,			NUT_EDITOR_ATXC_NULL)				// it should never happen !!!
#define NUT_EDITOR_ATXC_UPDATA_NULL_BLEND			NMAKELONG(NUT_EDITOR_ATXC_BLEND,			NUT_EDITOR_ATXC_NULL)
#define NUT_EDITOR_ATXC_UPDATA_NULL_MATERIAL		NMAKELONG(NUT_EDITOR_ATXC_MATERIAL,			NUT_EDITOR_ATXC_NULL)
//#define NUT_EDITOR_ATXC_UPDATA_NULL_GEOMSPECS		NMAKELONG(NUT_EDITOR_ATXC_GEOMSPECS,		NUT_EDITOR_ATXC_NULL)// It's not possible.
// #define NUT_EDITOR_ATXC_UPDATA_NULL_USEDOAARECTF32	NMAKELONG(NUT_EDITOR_ATXC_USEDOAARECTF32,	NUT_EDITOR_ATXC_NULL) 

#define NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE			NMAKELONG(NUT_EDITOR_ATXC_TEXTURE,			NUT_EDITOR_ATXC_NEW)
// #define NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE_WRAP_S		NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_S,		NUT_EDITOR_ATXC_NEW)	// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE_WRAP_T		NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_T,		NUT_EDITOR_ATXC_NEW)	// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE_MIN_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MIN_FILTER,	NUT_EDITOR_ATXC_NEW)	// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE_MAG_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MAG_FILTER,	NUT_EDITOR_ATXC_NEW)	// it should never happen !!!
#define NUT_EDITOR_ATXC_UPDATA_NEW_TXCOFFSET		NMAKELONG(NUT_EDITOR_ATXC_TXCOFFSET,		NUT_EDITOR_ATXC_NEW)// it may happen !!!
#define NUT_EDITOR_ATXC_UPDATA_NEW_TXCSCALE			NMAKELONG(NUT_EDITOR_ATXC_TXCSCALE,			NUT_EDITOR_ATXC_NEW)// it may happen !!!
#define NUT_EDITOR_ATXC_UPDATA_NEW_BLEND			NMAKELONG(NUT_EDITOR_ATXC_BLEND,			NUT_EDITOR_ATXC_NEW)
#define NUT_EDITOR_ATXC_UPDATA_NEW_MATERIAL			NMAKELONG(NUT_EDITOR_ATXC_MATERIAL,			NUT_EDITOR_ATXC_NEW)
//#define NUT_EDITOR_ATXC_UPDATA_NEW_GEOMSPECS		NMAKELONG(NUT_EDITOR_ATXC_GEOMSPECS,		NUT_EDITOR_ATXC_NEW)// it should never happen !!!
// #define NUT_EDITOR_ATXC_UPDATA_NEW_USEDOAARECTF32	NMAKELONG(NUT_EDITOR_ATXC_USEDOAARECTF32,	NUT_EDITOR_ATXC_NEW)

#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE		NMAKELONG(NUT_EDITOR_ATXC_TEXTURE,			NUT_EDITOR_ATXC_UPDATE)
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_WRAP_S		NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_S,		NUT_EDITOR_ATXC_UPDATE)			
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_WRAP_T		NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_WRAP_T,		NUT_EDITOR_ATXC_UPDATE)			
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_MIN_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MIN_FILTER,	NUT_EDITOR_ATXC_UPDATE)	
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_MAG_FILTER	NMAKELONG(NUT_EDITOR_ATXC_TEXTURE_MAG_FILTER,	NUT_EDITOR_ATXC_UPDATE)	
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCOFFSET		NMAKELONG(NUT_EDITOR_ATXC_TXCOFFSET,		NUT_EDITOR_ATXC_UPDATE)
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE		NMAKELONG(NUT_EDITOR_ATXC_TXCSCALE,			NUT_EDITOR_ATXC_UPDATE)
//#define NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSINGLE	NMAKELONG(NUT_EDITOR_ATXC_TXCSINGLE,		NUT_EDITOR_ATXC_UPDATE)
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_BLEND			NMAKELONG(NUT_EDITOR_ATXC_BLEND,			NUT_EDITOR_ATXC_UPDATE)
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_MATERIAL		NMAKELONG(NUT_EDITOR_ATXC_MATERIAL,			NUT_EDITOR_ATXC_UPDATE)
#define NUT_EDITOR_ATXC_UPDATA_UPDATE_GEOMSPECS		NMAKELONG(NUT_EDITOR_ATXC_GEOMSPECS,		NUT_EDITOR_ATXC_UPDATE)
// #define NUT_EDITOR_ATXC_UPDATA_UPDATE_USEDOAARECTF32 NMAKELONG(NUT_EDITOR_ATXC_USEDOAARECTF32,	NUT_EDITOR_ATXC_UPDATE)

typedef struct  
{
	Nu32			Type;		// 
	NTEXCOORD_2f	Current;	// Current TxC values.
	NTEXCOORD_2f	Previous;	// Previous TxC values.
	Nu8				TxCSet;		// Edited texCoordSet
}NUT_EDITOR_ATXC_UPDATA_TEXCOORD;

typedef struct  
{
	Nu32			Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_WRAP_S or ..._WRAP_T or ..._MIN_FILTER or ..._MAG_FILTER
	NTEXTURE		*pTexture;
	NATLAS_ELEMENT	*pAtlas_Element;
	Nu8				TextureUnit;
}NUT_EDITOR_ATXC_UPDATA_TEXTURE;

typedef struct  
{
	Nu32			Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE or NUT_EDITOR_ATXC_UPDATE_NEW_TEXTURE
	NTEXTURE		*pTexture;
	NATLAS_ELEMENT	*pAtlas_Element;
	Nu8				TextureUnit;
	GLint			Param;
}NUT_EDITOR_ATXC_UPDATA_TEXTURE_GLPARAM;
/*
typedef struct  
{
	Nu32		Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_USEDOAARECTF32 or NUT_EDITOR_ATXC_UPDATE_NEW_USEDOAARECTF32 or NUT_EDITOR_ATXC_UPDATE_NULL_USEDOAARECTF32
	NRECTf32	UsedOAARectf32;
	Nu32		TxCSet;		// Edited texCoordSet
}NUT_EDITOR_ATXC_UPDATA_USEDOAARECTF32;
*/

typedef struct  
{
	Nu32		Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE or NUT_EDITOR_ATXC_UPDATE_NEW_TEXTURE
	NBLEND		*pBlend;
}NUT_EDITOR_ATXC_UPDATA_BLEND;
typedef struct  
{
	Nu32		Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE or NUT_EDITOR_ATXC_UPDATE_NEW_TEXTURE
	NMATERIAL	*pMaterial;
}NUT_EDITOR_ATXC_UPDATA_MATERIAL;
typedef struct  
{
	Nu32					Type;		// NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE or NUT_EDITOR_ATXC_UPDATE_NEW_TEXTURE
	NGEOMETRY_FORMAT_ENUM	GeometryFormat;
}NUT_EDITOR_ATXC_UPDATA_COMPATIBLE_GEOMSPECS;

typedef union
{
	Nu32										Type;
	NUT_EDITOR_ATXC_UPDATA_TEXCOORD				TexCoord;	
	NUT_EDITOR_ATXC_UPDATA_TEXTURE				Texture;
	NUT_EDITOR_ATXC_UPDATA_TEXTURE_GLPARAM		TextureGLParam;
	NUT_EDITOR_ATXC_UPDATA_BLEND				Blend;
	NUT_EDITOR_ATXC_UPDATA_MATERIAL				Material;
	NUT_EDITOR_ATXC_UPDATA_COMPATIBLE_GEOMSPECS	GeomSpecs;
}NUT_EDITOR_ATXC_UPDATA;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUT_EDITOR_ATXC_UPDATA_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR

