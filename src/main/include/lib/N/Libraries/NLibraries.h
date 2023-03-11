// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
#ifdef _NLIBS
// ------------------------------------------------------------------------------------------------------------------------------------------
// -
// -
// -
#ifndef __NLIBRARIES_H
#define __NLIBRARIES_H

#include "lib/N/NType.h" 
#include "lib/N/NTexture.h"
#include "lib/N/Geometry/Components/NGeometryBlend.h"
#include "lib/N/Geometry/Components/NGeometryMaterial.h"
#include "lib/N/Containers/NHashMap.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NLibraries.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#define CONSTANT_NTEXTURESLIBRARY_CAPACITY			64	// power of two !
#define CONSTANT_NMATERIALSLIBRARY_CAPACITY			16	// power of two !
#define CONSTANT_NBLENDSLIBRARY_CAPACITY			16	// power of two !



// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **																			FUNCTIONS																		**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************

// High Level Functions based on Library. These 3 functions are going to 'do their best' to return a valid pointer.
// First by looking up into the appropriate NUT_Library	and without success ...
// By trying to load the requested element as an external file. In that case, after loading, the loaded file is automatically
// inserted into the library.
NTEXTURE*		NGetLibraryTexture(const Nchar *pkeyname);
NBLEND*			NGetLibraryBlend(const Nchar *pkeyname);
NMATERIAL*		NGetLibraryMaterial(const Nchar *pkeyname);


// Textures Library
void			NInitTexturesLibrary();
void			NDisableTexturesLibrary();
Nu32			NPurgeTextureLibrary();

void			NReloadAllLibraryTextures(); // --> especially made for ANDROID and OpenGl context lost

		void			NDeleteAllLibraryTextures();
		void			NDeleteLibraryTexture(const Nchar *pkeyname);

Nu32			NGetTexturesLibraryCount();
Nu32			NGetTexturesLibraryCapacity();

NTEXTURE*		NInsertLibraryTexture(const NTEXTURE *ptexture);
		NTEXTURE*		NRemoveLibraryTexture(const Nchar *pkeyname);

NTEXTURE*		NLookupLibraryTexture(const Nchar *pkeyname);
NTEXTURE*		NLookupLibraryTextureByValidIndex(const Nu32 validentry_index);
Nu32			NGetLibraryTextureValidIndex(const Nchar *pkeyname);



// Blend Library
void			NInitBlendsLibrary();
void			NDisableBlendsLibrary();

void			NDeleteAllLibraryBlends();
void			NDeleteLibraryBlend(const Nchar *pkeyname);

Nu32			NGetBlendsLibraryCount();
Nu32			NGetBlendsLibraryCapacity();

NBLEND*			NInsertLibraryBlend(const NBLEND *pblend);
NBLEND*			NRemoveLibraryBlend(const Nchar *pkeyname);

NBLEND*			NLookupLibraryBlend(const Nchar *pkeyname);
NBLEND*			NLookupLibraryBlendByValidIndex(const Nu32 validentry_index);
Nu32			NGetLibraryBlendValidIndex(const Nchar *pkeyname);


// Material Library
void			NInitMaterialsLibrary();
void			NDisableMaterialsLibrary();

void			NDeleteAllLibraryMaterials();
void			NDeleteLibraryMaterial(const Nchar *pkeyname);

Nu32			NGetMaterialsLibraryCount();
Nu32			NGetMaterialsLibraryCapacity();

NMATERIAL*		NInsertLibraryMaterial(const NMATERIAL *pmat);
NMATERIAL*		NRemoveLibraryMaterial(const Nchar *pkeyname);

NMATERIAL*		NLookupLibraryMaterial(const Nchar *pkeyname);
NMATERIAL*		NLookupLibraryMaterialByValidIndex(const Nu32 validentry_index);
Nu32			NGetLibraryMaterialValidIndex(const Nchar *pkeyname);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NLIBRARIES_H 
// -
// -
// -
// ------------------------------------------------------------------------------------------------------------------------------------------
#endif // _NLIBS
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
