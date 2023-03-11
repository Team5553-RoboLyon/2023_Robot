#ifndef __NTEXTURE_ATLAS_H
#define __NTEXTURE_ATLAS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTextureAtlas.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "lib/N/GL/Ngl.h"
#include "lib/N/Image/NImage.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NScreenPoint.h"
#include "lib/N/NScreenRect.h"
#include "lib/N/NRectf32.h"
#include "lib/N/NTexture.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/Containers/NHashMap.h"
#include "lib/N/Miscellaneous/NVertexTexCoordBuild.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures and enum										**
// ***************************************************************************************
#define EXTENSION_ATLAS ".txt"				 // Extension file for the text file which describe 'the Atlas'
#define CONSTANT_NATLAS_INITIALIZING_SIZE 32 // Size of ATLAS (OAHASHMAP actually) at N initialization.

	// an ATLAS Element
	/*
	#define NATLAS_ELEMENT_NAMESIZE		64
	typedef struct
	{
		Nchar			name[NATLAS_ELEMENT_NAMESIZE];
		Nu32			UID;			// Unique ID computed from the "name" of the Element
		NRECTs16		UsedOAARects16;	// Surface definition of the used texture rectangle.
		NTEXTURE		*ptexture;		// Texture where this Atlas Element is stored (as a rectangular part of it)
	}NATLAS_ELEMENT;
	*/

	typedef struct
	{
		Nchar *pName;			 // Has to be Unique !
		NTEXTURE *pTexture;		 // Texture where this Atlas Element is stored (as a rectangular part of it)
		NRECTs16 UsedOAARects16; // Surface definition of the used texture rectangle.
	} NATLAS_ELEMENT;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	Nu32 NGetAtlasCount();

	NATLAS_ELEMENT *NCreateAtlasElement(const Nchar *pname, NTEXTURE *ptexture, const NRECTs16 *prect);
	void NDeleteAtlasElement(Nchar *pname);

	NATLAS_ELEMENT *NLookupAtlasElement(const Nchar *pelement_name);
	NATLAS_ELEMENT *NLookupAtlasElementByValidIndex(const Nu32 validentry_index);
	Nu32 NGetAtlasElementValidIndex(const Nchar *pkeyname);

	typedef void (*NLOADATLASELEMENT_CALLBACK)(const NATLAS_ELEMENT *patlas_element, Nu32 Nu32_param);
	void NLoadAtlasElements(const char *filename, NTEXTURE *ptexture, const NLOADATLASELEMENT_CALLBACK callback, Nu32 Nu32_param);

	inline NTEXTURE *NGetAtlasElementTexture(const NATLAS_ELEMENT *pel) { return pel->pTexture; }
	NRECTf32 *NGetAtlasElementUsedOAARectf32(NRECTf32 *poaarect, const NATLAS_ELEMENT *pel);
	inline void NGetAtlasElementUsedOAARects16(const NATLAS_ELEMENT *pel, NRECTs16 *pscr_uvrect) { *pscr_uvrect = pel->UsedOAARects16; };

	// The size of the Atlas Element in pixel ( the simplest, with OAARect system integrated only, there is no reference system here )
	inline NVEC2s16 *NGetAtlasElementPixSize(NVEC2s16 *psize, const NATLAS_ELEMENT *pel) { return NGetOAARects16Size(psize, &pel->UsedOAARects16); }

	// The size of the Atlas Element in fPix,Norm and Hrel ( more complex, with OAARect system AND ReferenceViewPort System both integrated )
	inline NVEC2 *NGetAtlasElementfPixSize(NVEC2 *psize, const NATLAS_ELEMENT *pel, const Nbool _refpix_src)
	{
		NVEC2s16 sizes16;
		if (_refpix_src)
		{
			return NSize_RefPixTofPix(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
		else
		{
			return NSize_PixTofPix(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
	}
	inline NVEC2 *NGetAtlasElementHRelSize(NVEC2 *psize, const NATLAS_ELEMENT *pel, const Nbool _refpix_src)
	{
		NVEC2s16 sizes16;
		if (_refpix_src)
		{
			return NSize_RefPixToHRel(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
		else
		{
			return NSize_PixToHRel(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
	}
	inline NVEC2 *NGetAtlasElementNormSize(NVEC2 *psize, const NATLAS_ELEMENT *pel, const Nbool _refpix_src)
	{
		NVEC2s16 sizes16;
		if (_refpix_src)
		{
			return NSize_RefPixToNorm(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
		else
		{
			return NSize_PixToNorm(psize, NGetOAARects16Size(&sizes16, &pel->UsedOAARects16));
		}
	}

	// LOW LEVEL (INTERNAL USE)
	void NInitializeAtlas();
	void NClearAtlas();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NTEXTURE_ATLAS_H
