// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NImage.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "../Miscellaneous/NColor.h"
#include "../Core/NLimits.h"
#include "lib/N/NErrorHandling.h"
#include "NImage.h"

// ------------------------------------------------------------------------------------------
// NSetUpImage
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//			pimg		Valid pointer on a NIMAGE structure
//			pimg_desc	pointer on a Image Description Structure, or NULL to setup pimg as an empty NIMAGE
// Out :
//			pimg itself.
// ------------------------------------------------------------------------------------------
NIMAGE *NSetUpImage(NIMAGE *pimg, const NIMAGEDESC *pdesc)
{
	Nmem0(pimg, NIMAGE);

	if (!pdesc)
	{
		return pimg;
	}

	NErrorIf(!NIsValidImageID(pdesc->ImageID), NERROR_IMAGE_INVALID_IMAGEID);

	// For a void image, Nmem0 is the only thing we have to do (because NIMAGEVOID == 0, we dont have to write 'pimg->ImageID = NIMAGEID_VOID;')
	if (pdesc->ImageID == NIMAGEID_VOID)
		return pimg;

	// For all the other types, width and height has to be defined !
	NErrorIf(!pdesc->Width, NERROR_IMAGE_NULL_WIDTH);
	NErrorIf(!pdesc->Height, NERROR_IMAGE_NULL_HEIGHT);

	pimg->Height = pdesc->Height;
	pimg->Width = pdesc->Width;
	pimg->ImageID = pdesc->ImageID;
	pimg->pSurface = (Nu8 *)Nmalloc(pimg->Height * pimg->Width * (NIMAGE_BPP(pimg->ImageID) >> 3));

	// Color Map ?
	if (NIMAGE_COLORMAP(pimg->ImageID))
	{
		NErrorIf((!pdesc->ColorMapSize) || (pdesc->ColorMapSize > NIMAGE_COLORMAP_SIZEMAX), NERROR_IMAGE_INVALID_COLORMAP_SIZE);
		pimg->ColorMapSize = pdesc->ColorMapSize;
		if (NIMAGE_ALPHA(pimg->ImageID))
			pimg->pColorMap = (Nu8 *)Nmalloc(4 * pimg->ColorMapSize * sizeof(Nu8)); // 4 channels : RGBA
		else
			pimg->pColorMap = (Nu8 *)Nmalloc(3 * pimg->ColorMapSize * sizeof(Nu8)); // 3 channels : RGB
	}
	return pimg;
}

// ------------------------------------------------------------------------------------------
// NCreateImage
// ------------------------------------------------------------------------------------------
// Description :
//	Create an Image
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NIMAGE *NCreateImage(const NIMAGEDESC *pdesc)
{
	return NSetUpImage(NEW(NIMAGE), pdesc);
}

// ------------------------------------------------------------------------------------------
// NClearImage
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearImage(void *pimg)
{
	if (((NIMAGE *)pimg)->pSurface)
	{
		Nfree(((NIMAGE *)pimg)->pSurface);
	}

	if (((NIMAGE *)pimg)->pColorMap)
	{
		Nfree(((NIMAGE *)pimg)->pColorMap);
	}

	Nmem0(pimg, NIMAGE);
}

// ------------------------------------------------------------------------------------------
// NDeleteImage
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteImage(void *pimg)
{
	NClearImage(pimg);
	Nfree(pimg);
}
