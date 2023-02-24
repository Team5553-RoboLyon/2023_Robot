#ifndef __NIMAGE_H
#define __NIMAGE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NImage.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "lib/N/GL/Ngl.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//	IMAGE ID
//	Image ID uses 7 bits (bit 7, still available)		C
//	each ID is unique and contains infos inside.	  A	o
//	-Color Map (or not)	: bit (6)					  L	l
//	-BPP:				  bits (5 4 3)			   B  P	C
//	-ALPHA	   (or Not)	  bits (2)				C  P  H	n
//  -Number of TrueColor  bits (1 0)            M  P  A l
// --------------------------------------------|-|---|-|--|
#define NIMAGEID_VOID 0x0 //	0 000 0	00 |- VOID IMAGE (full of 0)

#define NIMAGEID_GRAY8 0x9	 //  0 001 0	01 |
#define NIMAGEID_RGB5 0x13	 //	0 010 0	11 |
#define NIMAGEID_GRAYA8 0x15 //	0 010 1	01 |- NIMAGE ID (TRUE COLOR)
#define NIMAGEID_RGB8 0x1B	 //	0 011 0	11 |
#define NIMAGEID_RGBA8 0x27	 //	0 100 1 11 |

#define NIMAGEID_COLMAP_RGB8 0x48  //	1 001 0	00 |- MIMAGE ID(COLORMAP)
#define NIMAGEID_COLMAP_RGBA8 0x4C //  1 001 1	00 |
// --------------------------------------------------------
// BPP
#define NIMAGE_BPP8 8
#define NIMAGE_BPP16 16
#define NIMAGE_BPP24 24
#define NIMAGE_BPP32 32

// COLOR MAP
#define NIMAGE_COLORMAP_SIZEMAX NU16_MAX
#define NIMAGE_COLORMAP_256 256

//  FLAGs IMAGE CHANNEL INFOs used by "NGetImageChannelInfo" and "NGetImageAlphaChannelInfo " functions
#define FLAG_NIMAGECHANNEL_EXIST BIT_0								   // Image channel exist
#define FLAG_NIMAGECHANNEL_IS_FLAT BIT_1							   // Image channel contains only one color (White only or Black only or one single gray only )
#define FLAG_NIMAGECHANNEL_CONTAINS_0 BIT_2							   // Image channel contains at least one pixel with a value of 0
#define FLAG_NIMAGECHANNEL_CONTAINS_255 BIT_3						   // Image channel contains at least one pixel with a value of 255 (or 31 for 16 bits image)
#define FLAG_NIMAGECHANNEL_CONTAINS_INTERMEDIATE_VALUE BIT_4		   // Image channel contains at least one pixel with a value between 0 and 255 (or 31 ...)
#define FLAG_NIMAGECHANNEL_CONTAINS_MULTIPLE_INTERMEDIATE_VALUES BIT_5 // Image channel contains multiple pixels with a value between 0 and 255 (or 31 ... )

// MACRO
#define NIMAGE_COLORMAP(imageid) ((imageid)&0x40)		 // retrieve 0-means no color map-			or 64(0x40)-means color map
#define NIMAGE_ALPHA(imageid) ((imageid)&0x4)			 // retrieve 0-means no ALPHA infos-			or 4(0x4) -means ALPHA infos.
#define NIMAGE_TRUECOLORCHANNEL(imageid) ((imageid)&0x3) // retrieve 0,1,2,3 - the exact number of true color channel
#define NIMAGE_BPP(imageid) ((imageid)&0x38)			 // Retrieve 8,16,24,32 - the exact number of bits used

	// 8 Available flags !
	// #define FLAG_IMAGE_
	typedef struct
	{
		Nu8 ImageID;
		Nu8 Flags;
		Nu16 ColorMapSize;
		Nu16 Width;
		Nu16 Height;
		Nu8 *pSurface;
		Nu8 *pColorMap;
	} NIMAGE;

	typedef struct
	{
		Nu8 ImageID;
		Nu16 Width;
		Nu16 Height;
		Nu16 ColorMapSize;
	} NIMAGEDESC;

	NIMAGE *NSetUpImage(NIMAGE *pimg, const NIMAGEDESC *pdesc);
	NIMAGE *NCreateImage(const NIMAGEDESC *pdesc);
	void NClearImage(void *pimg);
	void NDeleteImage(void *pimg);

	inline Nu16 NGetImageHeight(const NIMAGE *pimg) { return pimg->Height; }
	inline Nu16 NGetImageWidth(const NIMAGE *pimg) { return pimg->Width; }
	inline Nu8 *NGetImageSurface(const NIMAGE *pimg) { return pimg->pSurface; }
	inline Nu8 *NGetImageColorMap(const NIMAGE *pimg) { return pimg->pColorMap; }
	Nbool NIsValidImageID(const Nu8 imageid);

	Nu8 NGetImageChannelInfo(const NIMAGE *pimg, const Nu8 channel_index);
	Nu8 NGetImageAlphaChannelInfo(const NIMAGE *pimg);

	void NFlipImageHorizontally(NIMAGE *pimage);
	void NFlipImageVertically(NIMAGE *pimage);
	void NFlipImageVerticallyB(NIMAGE *pimage);

	Nu8 NConvertImageToRGB16(NIMAGE *pimg);
	Nu8 NConvertImageToRGB24(NIMAGE *pimg);
	Nu8 NConvertImageToRGBA32(NIMAGE *pimg);
	Nu8 NConvertImageToGRAY8(NIMAGE *pimg);
	Nu8 NConvertImageToGRAY16(NIMAGE *pimg);
	NIMAGE *NConformImageFormatToTextureCreationConstraint(NIMAGE *pimage);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NCAMERA_H
