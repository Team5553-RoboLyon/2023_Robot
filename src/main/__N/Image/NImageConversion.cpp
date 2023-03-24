// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NImageConversion.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../GL/Ngl.h"
#include "../Core/NSafeConversion.h"
#include "../Miscellaneous/NColor.h"
#include "../NErrorHandling.h"
#include "NImage.h"

// ------------------------------------------------------------------------------------------
// NConvertImageToRGB16
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NConvertImageToRGB16( NIMAGE *pimg )
{
	Nu32	i;
	Nu8		*pnewsurface8;
	Nu16	*pdst16;
	Nu8		*psrc8;
	Nu32	img_size;
	Nu8*	pcolor;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID),NERROR_IMAGE_INVALID_IMAGEID);

	switch(pimg->ImageID)
	{
		case NIMAGEID_VOID:
			return 0;

		case NIMAGEID_GRAY8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16			= (Nu16*)pnewsurface8;
			psrc8			= (Nu8*)pimg->pSurface;

			for( i=img_size;i!=0;i--,psrc8++,pdst16++ )
			{
				*pdst16 = RGB5( ((*psrc8)>>3) ,((*psrc8)>>3),((*psrc8)>>3) );
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;
			break;

		case NIMAGEID_RGB5:// Nothing to do
			NErrorIf(1,NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT); // Just to see, it's happening. but actually its not a bug ! 
			return 0;

		case NIMAGEID_GRAYA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16			= (Nu16*)pnewsurface8;
			psrc8			= (Nu8*)pimg->pSurface;

			for( i=img_size;i!=0;i--,psrc8+=2,pdst16++ )
			{
				*pdst16 = RGB5( ((*psrc8)>>3) ,((*psrc8)>>3),((*psrc8)>>3) );
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;
			break;

		case NIMAGEID_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16			= (Nu16*)pnewsurface8;
			psrc8			= (Nu8*)pimg->pSurface;

			for( i=img_size;i!=0;i--,psrc8+=3,pdst16++ )
			{
				*pdst16 = RGB5( ((psrc8[0])>>3) ,((psrc8[1])>>3),((psrc8[2])>>3) );
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;
			break;

		case NIMAGEID_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16			= (Nu16*)pnewsurface8;
			psrc8			= (Nu8*)pimg->pSurface;

			for( i=img_size;i!=0;i--,psrc8+=4,pdst16++ )
			{
				*pdst16 = RGB5( ((psrc8[0])>>3) ,((psrc8[1])>>3),((psrc8[2])>>3) );
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;
			break;

		case NIMAGEID_COLMAP_RGB8:
			img_size = pimg->Height*pimg->Height;
			psrc8 = pimg->pSurface;
			pnewsurface8 = (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16 = (Nu16*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst16++ )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*3]; // 3 BYTES per each RGB 
				*pdst16 = RGB5( (*pcolor)>>3,(*(pcolor+1))>>3,(*(pcolor+2))>>3 );
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;			
			break;

		case NIMAGEID_COLMAP_RGBA8:
			img_size = pimg->Height*pimg->Height;
			psrc8 = pimg->pSurface;
			pnewsurface8 = (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst16 = (Nu16*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst16++ )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*4]; // 4 BYTES per each RGB 
				*pdst16 = RGB5( (*pcolor)>>3,(*(pcolor+1))>>3,(*(pcolor+2))>>3 );
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB5;			
			break;

		default:
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); // Impossible to pass here ... but anyway
			return  0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NConvertImageToRGB24
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NConvertImageToRGB24( NIMAGE *pimg )
{
	Nu32	i;
	Nu8		*pnewsurface8;
	Nu8		*pdst8;
	Nu16	*psrc16;
	Nu8		*psrc8;
	Nu32	img_size;
	Nu8*	pcolor;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID),NERROR_IMAGE_INVALID_IMAGEID);
	
	switch(pimg->ImageID)
	{
		case NIMAGEID_VOID:
			return 0;

		case NIMAGEID_GRAY8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=3 )
			{
				pdst8[0] = *psrc8;
				pdst8[1] = *psrc8;
				pdst8[2] = *psrc8;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;
			break;

		case NIMAGEID_RGB5:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc16			= (Nu16*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc16++,pdst8+=3 )
			{
				pdst8[0] = _SafeNintToNu8( RGB5_R( *psrc16 ) );
				pdst8[1] = _SafeNintToNu8( RGB5_G( *psrc16 ) );
				pdst8[2] = _SafeNintToNu8( RGB5_B( *psrc16 ) );
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;
			break;

		case NIMAGEID_GRAYA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=2,pdst8+=3 )
			{
				pdst8[0] = *psrc8;
				pdst8[1] = *psrc8;
				pdst8[2] = *psrc8;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;
			break;

		case NIMAGEID_RGB8:// Nothing to do
			NErrorIf(1,NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT); // Just to see, it's happening. but actually its not a bug ! 
			return 0;

		case NIMAGEID_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=4,pdst8+=3 )
			{
				pdst8[0] = psrc8[0];
				pdst8[1] = psrc8[1];
				pdst8[2] = psrc8[2];
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;
			break;

		case NIMAGEID_COLMAP_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=3 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*3]; // 3 BYTES per each RGB color map entry
				pdst8[0] = pcolor[0];
				pdst8[1] = pcolor[1];
				pdst8[2] = pcolor[2];
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;			
			break;

		case NIMAGEID_COLMAP_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*3*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=3 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*4]; // 4 BYTES per each RGBA color map entry 
				pdst8[0] = pcolor[0];
				pdst8[1] = pcolor[1];
				pdst8[2] = pcolor[2];
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGB8;			
			break;

		default:
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); // Impossible to pass here ... but anyway
			return  0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NConvertImageToRGB32
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NConvertImageToRGBA32( NIMAGE *pimg )
{
	Nu32	i;
	Nu8		*pnewsurface8;
	Nu8		*pdst8;
	Nu16	*psrc16;
	Nu8		*psrc8;
	Nu32	img_size;
	Nu8*	pcolor;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID),NERROR_IMAGE_INVALID_IMAGEID);

	switch(pimg->ImageID)
	{
		case NIMAGEID_VOID:
			return 0;

		case NIMAGEID_GRAY8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=4 )
			{
				pdst8[0] = *psrc8;
				pdst8[1] = *psrc8;
				pdst8[2] = *psrc8;
				pdst8[3] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;
			break;

		case NIMAGEID_RGB5:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc16			= (Nu16*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc16++,pdst8+=4 )
			{
				pdst8[0] =  _SafeNintToNu8( RGB5_R( *psrc16 ) );
				pdst8[1] =  _SafeNintToNu8( RGB5_G( *psrc16 ) );
				pdst8[2] =  _SafeNintToNu8( RGB5_B( *psrc16 ) );
				pdst8[3] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;
			break;

		case NIMAGEID_GRAYA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=2,pdst8+=4 )
			{
				pdst8[0] = psrc8[0];
				pdst8[1] = psrc8[0];
				pdst8[2] = psrc8[0];
				pdst8[3] = psrc8[1];
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;
			break;

		case NIMAGEID_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=3,pdst8+=4 )
			{
				pdst8[0] = psrc8[0];
				pdst8[1] = psrc8[1];
				pdst8[2] = psrc8[2];
				pdst8[3] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;
			break;

		case NIMAGEID_RGBA8: // Nothing to do
			NErrorIf(1,NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT); // Just to see, it's happening. but actually its not a bug ! 
			return 0;

		case NIMAGEID_COLMAP_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=4 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*3]; // 3 BYTES per each RGB color map entry
				pdst8[0] = pcolor[0];
				pdst8[1] = pcolor[1];
				pdst8[2] = pcolor[2];
				pdst8[3] = 255;
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;			
			break;

		case NIMAGEID_COLMAP_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*4*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=4 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*4]; // 4 BYTES per each RGBA color map entry 
				pdst8[0] = pcolor[0];
				pdst8[1] = pcolor[1];
				pdst8[2] = pcolor[2];
				pdst8[3] = pcolor[3];
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_RGBA8;			
			break;

		default:
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); // Impossible to pass here ... but anyway
			return  0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NConvertImageToGray8
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NConvertImageToGRAY8( NIMAGE *pimg )
{
	Nu32	i;
	Nu8		*pnewsurface8;
	Nu8		*pdst8;
	Nu16	*psrc16;
	Nu8		*psrc8;
	Nu32	img_size;
	Nu8*	pcolor;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID),NERROR_IMAGE_INVALID_IMAGEID);

	switch(pimg->ImageID)
	{
		case NIMAGEID_VOID:
			return 0;

		case NIMAGEID_GRAY8:
			NErrorIf(1,NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT); // Just to see, it's happening. but actually its not a bug ! 
			return 0;

		case NIMAGEID_RGB5:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc16			= (Nu16*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc16++,pdst8++ )
			{
				*pdst8 = (Nu8)(( RGB5_R( *psrc16 )+RGB5_G( *psrc16 )+RGB5_B( *psrc16 ) )/3);
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_GRAYA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=2,pdst8++ )
			{
				*pdst8 = *psrc8;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=3,pdst8++ )
			{
				*pdst8 = (Nu8)( (psrc8[0]+psrc8[1]+psrc8[2] )/3);
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_RGBA8: // Nothing to do
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=4,pdst8++ )
			{
				*pdst8 = (Nu8)( (psrc8[0]+psrc8[1]+psrc8[2] )/3);
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			return 0;

		case NIMAGEID_COLMAP_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8++ )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*3]; // 3 BYTES per each RGB color map entry
				*pdst8 = (Nu8)( (pcolor[0]+pcolor[1]+pcolor[2] )/3);
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_COLMAP_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu8));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8++ )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*4]; // 4 BYTES per each RGBA color map entry 
				*pdst8 = (Nu8)( (pcolor[0]+pcolor[1]+pcolor[2] )/3);
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		default:
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); // Impossible to pass here ... but anyway
			return  0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NConvertImagetoGRAY16
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu8 NConvertImageToGRAY16( NIMAGE *pimg )
{
	Nu32	i;
	Nu8		*pnewsurface8;
	Nu8		*pdst8;
	Nu16	*psrc16;
	Nu8		*psrc8;
	Nu32	img_size;
	Nu8*	pcolor;

	NErrorIf(!pimg, NERROR_NULL_POINTER);
	NErrorIf(!NIsValidImageID(pimg->ImageID),NERROR_IMAGE_INVALID_IMAGEID);

	switch(pimg->ImageID)
	{
		case NIMAGEID_VOID:
			return 0;

		case NIMAGEID_GRAY8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=2 )
			{
				pdst8[0] = *psrc8;
				pdst8[1] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_RGB5:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc16			= (Nu16*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc16++,pdst8+=2 )
			{
				pdst8[0] = (Nu8)(( RGB5_R( *psrc16 )+RGB5_G( *psrc16 )+RGB5_B( *psrc16 ) )/3);
				pdst8[1] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_GRAYA8:
			NErrorIf(1,NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT); // Just to see, it's happening. but actually its not a bug ! 
			return 0;

		case NIMAGEID_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=3,pdst8+=2 )
			{
				pdst8[0] = (Nu8)( (psrc8[0]+psrc8[1]+psrc8[2] )/3);
				pdst8[1] = 255;
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_RGBA8: // Nothing to do
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8+=4,pdst8+=2 )
			{
				pdst8[0] = (Nu8)( (psrc8[0]+psrc8[1]+psrc8[2] )/3);
				pdst8[1] = psrc8[3];
			}

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			return 0;

		case NIMAGEID_COLMAP_RGB8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=2 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*3]; // 3 BYTES per each RGB color map entry
				
				pdst8[0] = (Nu8)( (pcolor[0]+pcolor[1]+pcolor[2] )/3);
				pdst8[1] = 255;
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		case NIMAGEID_COLMAP_RGBA8:
			img_size		= (Nu32)(pimg->Height*pimg->Height);
			psrc8			= (Nu8*)pimg->pSurface;
			pnewsurface8	= (Nu8*)Nmalloc(img_size*sizeof(Nu16));
			pdst8			= (Nu8*)pnewsurface8;

			for( i=img_size;i!=0;i--,psrc8++,pdst8+=2 )
			{
				pcolor	= &pimg->pColorMap[(*psrc8)*4]; // 4 BYTES per each RGBA color map entry 

				pdst8[0] = (Nu8)( (pcolor[0]+pcolor[1]+pcolor[2] )/3);
				pdst8[1] = pcolor[3];
			}

			Nfree(pimg->pColorMap);
			pimg->pColorMap		= NULL;
			pimg->ColorMapSize	= 0;

			Nfree(pimg->pSurface);
			pimg->pSurface	= pnewsurface8;
			pimg->ImageID	= NIMAGEID_GRAY8;
			break;

		default:
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); // Impossible to pass here ... but anyway
			return  0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NIMAGE* NConformImageToTextureCreationConstraint
// ------------------------------------------------------------------------------------------
// Description :
//	To be accepted as a texture image format has to be "texture constraint compliant"
//	that's the purpose of this function.
//	Make the right Image conversion to be compatible with texture creation
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NIMAGE* NConformImageFormatToTextureCreationConstraint(NIMAGE *pimage)
{
	NErrorIf(!pimage, NERROR_NULL_POINTER);
	if( pimage )
	{
		// Check incoming format and convert it if necessary
		switch(pimage->ImageID)
		{
		case NIMAGEID_VOID:
			// Will be rejected,"NCreateTextureFromTga" is going to return NULL.
			break;

		case NIMAGEID_GRAY8:
			// Will be accepted
			break;

		case NIMAGEID_RGB5:
			// Need to be converted to be accepted as a Texture
			NConvertImageToRGB24(pimage);
			break;

		case NIMAGEID_GRAYA8:
			// Will be accepted
			break;

		case NIMAGEID_RGB8:
			// Will be accepted
			break;

		case NIMAGEID_RGBA8:
			// Will be accepted
			break;

		case NIMAGEID_COLMAP_RGB8:
			// Need to be converted to be accepted as a Texture
			NConvertImageToRGB24(pimage);
			break;

		case NIMAGEID_COLMAP_RGBA8:
			// Need to be converted to be accepted as a Texture
			NConvertImageToRGBA32(pimage);
			break;

		default:
			// Impossible to pass here ... but anyway
			NErrorIf(1,NERROR_IMAGE_INVALID_IMAGEID); 
			break;
		}
	}
		return pimage;
}


