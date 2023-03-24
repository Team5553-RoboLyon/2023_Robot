#ifndef __NUT_BATCHLOAD_H
#define __NUT_BATCHLOAD_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_BatchLoad.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NTexture.h"
#include "../Geometry/Components/NGeometryBlend.h"
#include "../Geometry/Components/NGeometryMaterial.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Libraries File Extensions
#define NBATCHLOAD_TEXTURES_FILE_EXTENSION	".txt"			// Extension file for the text file which describes Texture filenames to load.
#define NBATCHLOAD_BLENDS_FILE_EXTENSION	".txt"			// Extension file for the text file which describes Blend filenames to load.
#define NBATCHLOAD_MATERIALS_FILE_EXTENSION	".txt"			// Extension file for the text file which describes Material filenames to load.

typedef void	(*NBATCHLOADTEXTURES_CALLBACK)( const NTEXTURE *ploadtexture, Nu32 Nu32_param );
typedef void	(*NBATCHLOADBLENDS_CALLBACK)( const NBLEND *ploadblend, Nu32 Nu32_param );
typedef void	(*NBATCHLOADMATERIALS_CALLBACK)( const NMATERIAL *ploadmaterial, Nu32 Nu32_param );

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **																			FUNCTIONS																		**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
void NBatchLoadTextures(const Nchar *pbatchfilename, const NBATCHLOADTEXTURES_CALLBACK callback, Nu32 param);
void NBatchLoadBlends(const Nchar *pbatchfilename, const NBATCHLOADBLENDS_CALLBACK callback, Nu32 Nu32_param);

// void NLoadTexturesLibrary(const Nchar *plibraryfilename, const NLOADTEXTURESLIBRARY_CALLBACK callback, Nu32 param);
// void NLoadBlendsLibrary(const Nchar *plibraryfilename,const NLOADBLENDSLIBRARY_CALLBACK callback, Nu32 Nu32_param);
// void NLoadMaterialsLibrary(const Nchar *plibraryfilename,const NLOADMETERIALSLIBRARY_CALLBACK callback, Nu32 Nu32_param);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_BATCHLOAD_H 

