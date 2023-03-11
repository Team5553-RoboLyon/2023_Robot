// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NImageTools.cpp									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "lib/N/NErrorHandling.h"
#include "NImage.h"

// ------------------------------------------------------------------------------------------
// NIsValidImageID
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nbool NIsValidImageID(const Nu8 imageid)
{
	switch (imageid)
	{
	case NIMAGEID_VOID:
	case NIMAGEID_GRAY8:
	case NIMAGEID_RGB5:
	case NIMAGEID_GRAYA8:
	case NIMAGEID_RGB8:
	case NIMAGEID_RGBA8:
	case NIMAGEID_COLMAP_RGB8:
	case NIMAGEID_COLMAP_RGBA8:
		return NTRUE;
	default:
		return NFALSE;
	}
}

// ------------------------------------------------------------------------------------------
// NGetImageChannelInfo
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
// if only one of these 3 flags is set to on, channel is flat !
#define MASK_NIMAGECHANNEL_FLAT FLAG_NIMAGECHANNEL_CONTAINS_0 | FLAG_NIMAGECHANNEL_CONTAINS_255 | FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE

Nu8 NGetImageChannelInfo(const NIMAGE *pimg, const Nu8 channel_index)
{
	Nu8 info = 0;
	Nu32 i;
	Nu8 *psurface8;
	Nu16 *psurface16;
	Nu8 shift;
	Nu8 preval;
	Nu32 img_size;
	Nu8 val;
	Nu8 fullflags = FLAG_NIMAGECHANNEL_CONTAINS_0 | FLAG_NIMAGECHANNEL_CONTAINS_255 | FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE | FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID), NERROR_IMAGE_INVALID_IMAGEID);

	if (pimg->ImageID == NIMAGEID_VOID)
		return 0;

	// Does Channel exist ?
	if (channel_index >= (Nu8)(NIMAGE_TRUECOLORCHANNEL(pimg->ImageID) + NIMAGE_ALPHA(pimg->ImageID)))
		return 0;

	// From Here, we are sure ... requested channel exists for this image !
	FLAG_ON(info, FLAG_NIMAGECHANNEL_EXIST);

	if (pimg->ImageID == NIMAGEID_RGB5)
	{
		psurface16 = (Nu16 *)pimg->pSurface;
		img_size = pimg->Height * pimg->Width;
		shift = 10 - 5 * channel_index;
		preval = 0;

		for (i = img_size; i != 0; i--, psurface16++)
		{
			val = (Nu8)(((*psurface16) >> shift) & 0x1F);

			if (val == 0)
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_0);
			}
			else if (val == 0x1F) // 0x1F is the max 5 bits value ( == 31 )
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_255);
			}
			else
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE);

				if (preval && (val != preval))
					FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES);

				preval = val;
			}

			// we can stop if all the flags are already set to ON.
			if ((info & fullflags) == info)
				break;
		}
	}
	else
	{
		psurface8 = (Nu8 *)(pimg->pSurface + channel_index);
		img_size = pimg->Height * pimg->Width;
		shift = channel_index + 1;
		preval = 0;

		for (i = img_size; i != 0; i--, psurface8 += shift)
		{
			if (*psurface8 == 0)
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_0);
			}
			else if (*psurface8 == 255)
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_255);
			}
			else
			{
				FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE);

				if (preval && (*psurface8 != preval))
					FLAG_ON(info, FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES);

				preval = *psurface8;
			}

			// we can stop if all the flags are already set to ON.
			if (fullflags == info)
				break;
		}
	}

	// ... Final Test
	if (ISFLAG_OFF(info, FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES))
	{
		if (FLAGS_TEST(info, MASK_NIMAGECHANNEL_FLAT, FLAG_NIMAGECHANNEL_CONTAINS_0) ||
			FLAGS_TEST(info, MASK_NIMAGECHANNEL_FLAT, FLAG_NIMAGECHANNEL_CONTAINS_255) ||
			FLAGS_TEST(info, MASK_NIMAGECHANNEL_FLAT, FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE))
		{
			FLAG_ON(info, FLAG_NIMAGECHANNEL_IS_FLAT);
		}
	}

	return info;
}

// ------------------------------------------------------------------------------------------
// NGetImageAlphaChannelInfo
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NGetImageAlphaChannelInfo(const NIMAGE *pimg)
{
	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID), NERROR_IMAGE_INVALID_IMAGEID);

	switch (pimg->ImageID)
	{
	case NIMAGEID_RGBA8:
		return NGetImageChannelInfo(pimg, 3);

	case NIMAGEID_GRAYA8:
		return NGetImageChannelInfo(pimg, 1);

	default:
		return 0;
	}

	//	return 0;
}

void NFlipImageHorizontally(NIMAGE *pimage)
{
	Nu32 half_width;
	Nu32 lastpix;
	Nu32 pitch;
	Nu8 *ptr8a, *ptr8b, pix8;
	Nu16 *ptr16a, *ptr16b, pix16;
	Nu32 *ptr32a, *ptr32b, pix32;
	Nu32 i, j;

	NErrorIf(!pimage, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimage->ImageID), NERROR_IMAGE_INVALID_IMAGEID);
	NErrorIf(pimage->ImageID == NIMAGEID_VOID, NERROR_TEXTURE_UNSUPPORTED_IMAGEID);

	half_width = (pimage->Width >> 1);							// == /2
	pitch = pimage->Width * (NIMAGE_BPP(pimage->ImageID) >> 3); // with in BYTES

	switch (NIMAGE_BPP(pimage->ImageID))
	{
	case NIMAGE_BPP8:
		lastpix = pimage->Width - 1;
		for (j = 0; j < pimage->Height; j++)
		{
			ptr8a = (Nu8 *)(pimage->pSurface + j * pitch);
			ptr8b = ptr8a + lastpix;

			for (i = half_width; i != 0; i--, ptr8a++, ptr8b--)
			{
				pix8 = *ptr8a;
				*ptr8a = *ptr8b;
				*ptr8b = pix8;
			}
		}
		break;

	case NIMAGE_BPP16:
		lastpix = pimage->Width - 1;
		for (j = 0; j < pimage->Height; j++)
		{
			ptr16a = (Nu16 *)(pimage->pSurface + j * pitch);
			ptr16b = ptr16a + lastpix;

			for (i = half_width; i != 0; i--, ptr16a++, ptr16b--)
			{
				pix16 = *ptr16a;
				*ptr16a = *ptr16b;
				*ptr16b = pix16;
			}
		}
		break;

	case NIMAGE_BPP24:
		lastpix = (pimage->Width - 1) * 3;
		for (j = 0; j < pimage->Height; j++)
		{
			ptr8a = (Nu8 *)(pimage->pSurface + j * pitch);
			ptr8b = ptr8a + lastpix;
			for (i = half_width; i != 0; i--, ptr8a += 3, ptr8b -= 3)
			{
				// 0
				pix8 = ptr8a[0];
				ptr8a[0] = ptr8b[0];
				ptr8b[0] = pix8;
				// 1
				pix8 = ptr8a[1];
				ptr8a[1] = ptr8b[1];
				ptr8b[1] = pix8;
				// 2
				pix8 = ptr8a[2];
				ptr8a[2] = ptr8b[2];
				ptr8b[2] = pix8;
			}
		}
		break;

	case NIMAGE_BPP32:
		lastpix = pimage->Width - 1;
		for (j = 0; j < pimage->Height; j++)
		{
			ptr32a = (Nu32 *)(pimage->pSurface + j * pitch);
			ptr32b = ptr32a + lastpix;

			for (i = half_width; i != 0; i--, ptr32a++, ptr32b--)
			{
				pix32 = *ptr32a;
				*ptr32a = *ptr32b;
				*ptr32b = pix32;
			}
		}
		break;
	}
}

// Using a temporary buffer as smaller as possible ( size of one line of the image ... )
// This function consumes less memory than the "NFlipImageVerticallyB" but could be slower due to a lot of "memcpy" calls
void NFlipImageVertically(NIMAGE *pimage)
{
	Nu32 i;
	Nu8 *oneline_buf;
	Nu32 pitch;
	Nu32 half_height;
	Nu8 *ptr1, *ptr2;

	NErrorIf(!pimage, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimage->ImageID), NERROR_IMAGE_INVALID_IMAGEID);
	NErrorIf(pimage->ImageID == NIMAGEID_VOID, NERROR_TEXTURE_UNSUPPORTED_IMAGEID);

	pitch = pimage->Width * (NIMAGE_BPP(pimage->ImageID) >> 3); // with in BYTES

	oneline_buf = (Nu8 *)Nmalloc(pitch);
	half_height = (Nu32)(pimage->Height >> 1); // == /2
	ptr1 = pimage->pSurface;
	ptr2 = pimage->pSurface + (pimage->Height - 1) * pitch;

	for (i = half_height; i != 0; i--, ptr1 += pitch, ptr2 -= pitch)
	{
		memcpy(oneline_buf, ptr1, pitch);
		memcpy(ptr1, ptr2, pitch);
		memcpy(ptr2, oneline_buf, pitch);
	}

	Nfree(oneline_buf);
}

// Using a temporary buffer with the same size as the image ...
// This function consumes more memory than the NFlipImageVertically but in the other hand it could be faster due to
// a minimum number of memcpy calls
void NFlipImageVerticallyB(NIMAGE *pimage)
{
	Nu32 i;
	Nu8 *tmp_buf;
	Nu32 pitch;
	Nu8 *ptr1, *ptr2;

	NErrorIf(!pimage, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimage->ImageID), NERROR_IMAGE_INVALID_IMAGEID);
	NErrorIf(pimage->ImageID == NIMAGEID_VOID, NERROR_TEXTURE_UNSUPPORTED_IMAGEID);

	pitch = pimage->Width * (NIMAGE_BPP(pimage->ImageID) >> 3); // with in BYTES

	tmp_buf = (Nu8 *)Nmalloc(pimage->Width * pitch);
	ptr1 = pimage->pSurface;
	ptr2 = tmp_buf + (pimage->Height - 1) * pitch;

	for (i = pimage->Height; i != 0; i--, ptr1 += pitch, ptr2 -= pitch)
	{
		memcpy(ptr2, ptr1, pitch);
	}
	memcpy(pimage->pSurface, tmp_buf, pimage->Width * pitch);
	Nfree(tmp_buf);
}