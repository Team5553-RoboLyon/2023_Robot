// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTga.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../GL/Ngl.h"
#include "../File/NFile.h"
#include "../Miscellaneous/NColor.h"
#include "../Core/NLimits.h"
#include "../Core/NSafeConversion.h"
#include "NImage.h"
#include "NTga.h"


NIMAGE*	NCreateImageFromTga( const Nchar *pfilename )
{
	NFILE			*pfile;
	NHEADER_TGA		tgaheader;
	NIMAGEDESC		imagedesc;
	Nu32			offset;
	Nu32			i;
	Nu8				read_byte[4]={255}; // the 4 bytes will have a value of 255
	Nu8				*psurface;
	Nu8				*psurfacemax;
	Nu8				*pcolormap;
	Nu16			read16;
	Nu8				rleheader;
	Nu8				rlepacksize; // Nu8 is enough,  value max will be 128 !!!
	Nu8				i8;
	
	// First basic check (debug only)
	NErrorIf( !pfilename,NERROR_NULL_POINTER );

	NErrorIf( !NCheckFileType(pfilename, EXTENSION_TGA),NERROR_FILE_INVALID_EXTENSION );
	// Open file an read TGA Header
	pfile = NFileOpen( pfilename,FLAG_NFILE_READ|FLAG_NFILE_BINARY );
	if(!pfile)
		return NULL;
	
	memset(&tgaheader,0,sizeof(NHEADER_TGA));
	NFileRead(pfile,&tgaheader,sizeof(NHEADER_TGA),1);
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + CHECK FOR TGA FILE FORMAT RECOGNITION LIMITATIONS
	//		N Doesn't manage all the TGA file format
	//		for example 4 Bpp image is not supported.
	//		So, before loading supported data,there are some previous checks
	//		to perform. Purpose: Detect unsupported data
	// Check for Image Type
	// ++++++++++++++++++++
	// Managed 'ImageType' values are:
	// 		0000	0  no image data is present
	//
	// 		0001	1  uncompressed color-mapped image
	// 		0010	2  uncompressed true-color image
	// 		0011	3  uncompressed black-and-white (gray scale) image
	// 		
	//		1001	9  run-length encoded color-mapped image
	// 		1010	10 run-length encoded true-color image
	// 		1011	11 run-length encoded black-and-white (gray scale) image
	//
	//	Notes:
	//		All the managed Image type are coded on the bits 0,1 and 3 (mask = 1011 = 0xB )
	//		There are 3 basic types (color map, true color and gray scale) in 2 modes, "uncompressed" and "run-length encoded"
	//		mask 0x3 help to retrieve the basic types and mask 0x8 to retrieve compression ( true or false )  
	NErrorIf(  tgaheader.ImageType != ( tgaheader.ImageType & 0xB ) ,NERROR_TGA_UNKNOWN_TYPE );
	NErrorIf(  !tgaheader.ImageType									,NERROR_TGA_IMAGEDATA_MISSING );
	
	// Check for Image Bpp
	// ++++++++++++++++++++
	// Managed 'Image Bpp' values are:
	// 		8 Bits per Pixel
	// 		16 Bits per Pixel
	// 		24 Bits per Pixel
	// 		32 Bits per Pixel
	NErrorIf( (tgaheader.Bpp < 8) || (tgaheader.Bpp > 32), NERROR_TGA_UNKNOWN_BPP );
	NErrorIf( ( (tgaheader.Bpp>>3)<<3 )!= tgaheader.Bpp, NERROR_TGA_UNKNOWN_BPP );

/*
	NErrorIf( !tgaheader.ImageType ,NERROR_TGA_IMAGEDATA_MISSING );
*/
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NIMAGE SETUP
	//		Image Buffer Setup. TGA Image data will be loaded into it.
	Nmem0(&imagedesc,NIMAGEDESC);
	imagedesc.Width		= tgaheader.Width;
	imagedesc.Height	= tgaheader.Height;

	// Choose the most suitable NImage Type ID
	switch(tgaheader.ImageType)
	{
	  //case 0: // no image data is present --- DATA MISSING generate an Error at the beginning of this function
		case 1: // uncompressed color-mapped image
		case 9: // run-length encoded color-mapped image
			NErrorIf( tgaheader.ColorMapDepth<16, NERROR_TGA_INCONSISTENT_COLORMAP_DEPTH ); // 16,24 or 32 ONLY !
			NErrorIf( !tgaheader.ColorMapType, NERROR_TGA_COLORMAP_MISSING );
			NErrorIf((!tgaheader.ColorMapLength)||(tgaheader.ColorMapLength>NIMAGE_COLORMAP_SIZEMAX),NERROR_IMAGE_INVALID_COLORMAP_SIZE );
			NErrorIf( tgaheader.Bpp!=8 , NERROR_TGA_COLORMAP_TYPE_SHOULD_HAVE_8BPP );

			imagedesc.ColorMapSize	= tgaheader.ColorMapLength;

			if( tgaheader.ColorMapDepth <= 24 )
			{
				imagedesc.ImageID = NIMAGEID_COLMAP_RGB8;
			}
			else
			{
				imagedesc.ImageID = NIMAGEID_COLMAP_RGBA8;
			}
			break;
		
		case 2: // uncompressed true-color image
		case 10:// run-length encoded true-color image
			NErrorIf((tgaheader.Bpp!=16)&&(tgaheader.Bpp!=24)&&(tgaheader.Bpp!=32),NERROR_TGA_INCONSISTENT_TYPE_AND_BPP);
			if( tgaheader.Bpp == 16 )
			{
				imagedesc.ImageID = NIMAGEID_RGB5;
			}
			else if( tgaheader.Bpp == 24 )
			{
				imagedesc.ImageID = NIMAGEID_RGB8;
			}
			else
			{
				imagedesc.ImageID = NIMAGEID_RGBA8;
			}
			break;
		
		case 3:	// uncompressed black-and-white (gray scale) image
		case 11:// run-length encoded black-and-white (gray scale) image
			NErrorIf((tgaheader.Bpp!=8)&&(tgaheader.Bpp!=16),NERROR_TGA_INCONSISTENT_TYPE_AND_BPP);
			if(tgaheader.Bpp == 8) // 8 bits
			{
				imagedesc.ImageID = NIMAGEID_GRAY8;
			}
			else // 16 bits
			{
				imagedesc.ImageID = NIMAGEID_GRAYA8;
			}
			break;
	}
	// So, let's go, and CREATE the destination image !!!
	// All the right memory allocations will be done by the NSetUpImage function ...
	// ... according to the "image description" parameters.
	 NIMAGE *pimg = NCreateImage(&imagedesc);

	// Seek into the file to the exact position where image data are.
	//		18 is the exact size of the NHEADERTGA structure.
	//		IDLength is the exact size of a the image ID field which is stored in the file just after the header.
	offset = 18 + tgaheader.IDLength;
	NFileSeek(pfile,offset,NFILE_SEEK_SET);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + COLOR MAP LOADING (if there is one !)
	if( tgaheader.ColorMapType ) // means there is one color map to load !
	{
		NErrorIf( tgaheader.ColorMapDepth<16, NERROR_TGA_INCONSISTENT_COLORMAP_DEPTH ); // 16,24 or 32 ONLY !
		NErrorIf((!tgaheader.ColorMapLength)||(tgaheader.ColorMapLength>NIMAGE_COLORMAP_SIZEMAX),NERROR_IMAGE_INVALID_COLORMAP_SIZE );
		NErrorIf( tgaheader.Bpp!=8 , NERROR_TGA_COLORMAP_TYPE_SHOULD_HAVE_8BPP );
		NErrorIf( (tgaheader.ImageType!=1)&&(tgaheader.ImageType!=9),	NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );

		pcolormap = NGetImageColorMap(pimg);

		// Read ColorMap
		switch( tgaheader.ColorMapDepth )
		{
			case 16: // 5-5-5 !!! No 16bits Color Map for NENGINE !!!, so convert it into 24 bits Color Map
				for( i=tgaheader.ColorMapLength; i!=0; i--,pcolormap +=3 )
				{
					NFileRead( pfile,&read16 ,sizeof(Nu16), 1 );
					pcolormap[0] =  _SafeNintToNu8( RGB5_R(read16));
					pcolormap[1] =  _SafeNintToNu8( RGB5_G(read16));
					pcolormap[2] =  _SafeNintToNu8( RGB5_B(read16));
				}
				break;
			case 24:
				for( i=tgaheader.ColorMapLength; i!=0; i--,pcolormap +=3 )
				{
					NFileRead( pfile,read_byte ,sizeof(Nu8), 3 );
					pcolormap[0] = read_byte[2];
					pcolormap[1] = read_byte[1];
					pcolormap[2] = read_byte[0];
				}
				break;
			case 32:
				for( i=tgaheader.ColorMapLength; i!=0; i--,pcolormap +=4 )
				{
					NFileRead( pfile,read_byte ,sizeof(Nu8), 4 );
					//*((Nu32*)pcolormap) = RGBA8( read_byte[2], read_byte[1], read_byte[0], read_byte[3] );
					pcolormap[0] = read_byte[2];
					pcolormap[1] = read_byte[1];
					pcolormap[2] = read_byte[0];
					pcolormap[3] = read_byte[3];
				}
				break;
		}

		// Color map is now loaded, So set the right file position
		// and continue loading for all the surface data 
		offset = 18 + tgaheader.IDLength + (tgaheader.ColorMapDepth >> 3)*tgaheader.ColorMapLength;
		NFileSeek(pfile,offset,NFILE_SEEK_SET);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SURFACE DATA LOADING
	//		+ RLE
	//		+ or RAW
	//
	// + RUN LENGHT ENCODING (RLE)
	if( tgaheader.ImageType&0x8 )
	{
		switch( tgaheader.Bpp )
		{
			// 8 Bits / Pixel : Color Map or GrayScale
			case 8:
				// For a RLE 8 Bpp Tga file, ImageType should be 9 or 11 ...
				// ... that means COLOR MAP 8 bits or GRAY SCALE (without ALPHA)

				NErrorIf( (tgaheader.ImageType!=9)&&(tgaheader.ImageType!=11),	NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				
				psurface		= NGetImageSurface(pimg);
				psurfacemax		= psurface + tgaheader.Width*tgaheader.Height*1;// 1 for one Nu8 (1 NBYTE) !
				while ( psurface < psurfacemax )
				{
					// Read first byte: THE RLE Header !
					NFileRead( pfile,&rleheader,sizeof(Nu8),1 );
					// Extract the number of consecutive pixels concerned by the header.
					rlepacksize = 1 + (rleheader&0x7f); 

					if (rleheader & 0x80)// RLE pack (pixels are encoded ...)
					{
						// Run-length packet to read !
						// Simple: 2 steps
						//		first, read one "normal" 'color map' indexed pixel/'one channel' gray pixel.
						//		second, apply this index to 'n' consecutive pixels ( n =  rle_pack_size )
						NFileRead( pfile,read_byte,sizeof(Nu8),1 );
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface ++)
						{
							*psurface =	*read_byte;
						}
					}
					else // Not an RLE pack (These pixels are not encoded.)
					{
						// Non Run-Length-Encoded packet to read !
						// Simple: 1 Step, read all the packet pixels as "normal" 'color map' indexed pixel/'one channel' gray pixel, that's all.
						NFileRead( pfile,psurface,sizeof(Nu8),rlepacksize );
						psurface += rlepacksize;
						/*
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface ++)
						{
							NFileRead( pfile,read_byte,1,sizeof(Nu8) );
							*psurface =	*read_byte;
						}
						*/
					}
				}				
				break;

			// 16 Bits / Pixel : True Color		5 5 5
			case 16:
				// for a RLE 16 Bpp Tga file, ImageType should be 10 or 11 ...
				// ... that means TRUE COLOR 16 bits or GRAY SCALE (with ALPHA)
				NErrorIf( (tgaheader.ImageType!=10)&&(tgaheader.ImageType!=11),	NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				NErrorIf( tgaheader.ColorMapType,	NERROR_TGA_RGB16_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,	NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );

				psurface		= NGetImageSurface(pimg);
				psurfacemax		= psurface + tgaheader.Width*tgaheader.Height*2;// 2 for two Nu8 (2 BYTES) !
				while ( psurface < psurfacemax )
				{
					// Read first byte: THE RLE Header !
					NFileRead( pfile,&rleheader,sizeof(Nu8),1 );
					// Extract the number of consecutive pixels concerned by the header.
					rlepacksize = 1 + (rleheader&0x7f); 

					if (rleheader & 0x80)// RLE pack (pixels are encoded ...)
					{
						// Run-length packet to read !
						// Simple: 2 steps
						//		first, read one "normal" RVB24 pixel.
						//		second, apply these values to 'n' consecutive pixels ( n =  rle_pack_size )
						NFileRead( pfile,&read16,sizeof(Nu16),1 );
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 2)
						{
							*((Nu16*)psurface) = read16;
						}
					}
					else // Not an RLE pack (These pixels are not encoded.)
					{
						// Non Run-Length-Encoded packet to read !
						// Simple: 1 Step, read all the packet pixels as "normal" RVB16 pixel, that's all.
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 2)
						{
							NFileRead( pfile,&read16,sizeof(Nu16),1 );
							*((Nu16*)psurface) = read16;
						}
					}
				}				
				break;

			// 24 Bits / Pixel : True Color		8 8 8 RLE
			case 24:
				// for a RLE 24 Bpp Tga file, ImageType should be 10 ...
				NErrorIf( tgaheader.ImageType!=10,	NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				NErrorIf( tgaheader.ColorMapType,	NERROR_TGA_RGB24_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,	NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );
				
				psurface		= NGetImageSurface(pimg);
				psurfacemax		= psurface + tgaheader.Width*tgaheader.Height*3;// 3 for three Nu8 (3 BYTES) !
				while ( psurface < psurfacemax )
				{
					// Read first byte: THE RLE Header !
					NFileRead( pfile,&rleheader,sizeof(Nu8),1 );
					// Extract the number of consecutive pixels concerned by the header.
					rlepacksize = 1 + (rleheader&0x7f); 

					if (rleheader & 0x80)// RLE pack (pixels are encoded ...)
					{
						// Run-length packet to read !
						// Simple: 2 steps
						//		first, read one "normal" RVB24 pixel.
						//		second, apply these values to 'n' consecutive pixels ( n =  rle_pack_size )
						NFileRead( pfile,read_byte,sizeof(Nu8),3 );
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 3)
						{
							psurface[0] =	read_byte[2];
							psurface[1] =	read_byte[1];
							psurface[2] =	read_byte[0];
						}
					}
					else // Not an RLE pack (These pixels are not encoded.)
					{
						// Non Run-Length-Encoded packet to read !
						// Simple: 1 Step, read all the packet pixels as "normal" RVB24 pixel, that's all.
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 3)
						{
							NFileRead( pfile,read_byte,sizeof(Nu8),3 );
							psurface[0] =	read_byte[2];
							psurface[1] =	read_byte[1];
							psurface[2] =	read_byte[0];
						}
					}
				}				
				break;

			// 32 Bits / Pixel : True Color		8 8 8 8
			case 32:
				// for a RLE 32 Bpp Tga file, ImageType should be 10 ...
				NErrorIf( tgaheader.ImageType!=10,	NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				NErrorIf( tgaheader.ColorMapType,	NERROR_TGA_RGB32_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,	NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );

				psurface		= NGetImageSurface(pimg);
				psurfacemax		= psurface + tgaheader.Width*tgaheader.Height*4;// 4 for four Nu8 (4 BYTES) !
				while ( psurface < psurfacemax )
				{
					// Read first byte: THE RLE Header !
					NFileRead( pfile,&rleheader,sizeof(Nu8),1 );
					// Extract the number of consecutive pixels concerned by the header.
					rlepacksize = 1 + (rleheader&0x7f); 

					if (rleheader & 0x80)// RLE pack (pixels are encoded ...)
					{
						// Run-length packet to read !
						// Simple: 2 steps
						//		first, read one "normal" RVB32 pixel.
						//		second, apply these values to 'n' consecutive pixels ( n =  rle_pack_size )
						NFileRead( pfile,read_byte,sizeof(Nu8),4 );
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 4)
						{
							psurface[0] =	read_byte[2];
							psurface[1] =	read_byte[1];
							psurface[2] =	read_byte[0];
							psurface[3] =	read_byte[3];
						}
					}
					else // Not an RLE pack (These pixels are not encoded.)
					{
						// Non Run-Length-Encoded packet to read !
						// Simple: 1 Step, read all the packet pixels as "normal" RVB24 pixel, that's all.
						for (i8 = rlepacksize; i8!=0 ; i8--, psurface += 4)
						{
							NFileRead( pfile,read_byte,sizeof(Nu8),4 );
							psurface[0] =	read_byte[2];
							psurface[1] =	read_byte[1];
							psurface[2] =	read_byte[0];
							psurface[3] =	read_byte[3];
						}
					}
				}				
				break;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + RAW DATA
	else
	{
		switch( tgaheader.Bpp )
		{
			// 8 Bits / Pixel : Color Map or GrayScale
			case 8:
				// for a 8 Bpp Tga file, ImageType should be 1 or 3 ...
				// ... that means COLOR MAP 8 bits or GRAY SCALE (without ALPHA)
				NErrorIf( (tgaheader.ImageType!=1)&&(tgaheader.ImageType!=3), NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				
				if(tgaheader.ImageType == 1)		// Means Color Map
				{
					NFileRead( pfile, NGetImageSurface(pimg), sizeof(Nu8) , tgaheader.Height*tgaheader.Width );
				}
				else // (tgaheader.ImageType == 3)	// Means gray scale 
				{
					NErrorIf( tgaheader.ColorMapType,		NERROR_TGA_GRAYSCALE_SHOULDNT_HAVE_COLORMAP );
					NErrorIf( tgaheader.ColorMapLength	,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
					NErrorIf( tgaheader.ColorMapDepth,		NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );

					// Read Image Data
					offset = 18 + tgaheader.IDLength + (tgaheader.ColorMapDepth >> 3)*tgaheader.ColorMapLength;
					NFileSeek( pfile,offset,NFILE_SEEK_SET);
					NFileRead( pfile,NGetImageSurface(pimg), sizeof(Nu8), tgaheader.Height*tgaheader.Width );
				}
				break;

			// 16 Bits / Pixel : True Color		5 5 5
			case 16:
				// For a 16 Bpp Tga file, ImageType should be 2 or 3...
				// ... that means TRUE COLOR 16 bits or GRAY SCALE (with ALPHA)

				NErrorIf( (tgaheader.ImageType!=2)&&(tgaheader.ImageType!=3), NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );

				NErrorIf( tgaheader.ColorMapType,		NERROR_TGA_RGB16_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength	,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,		NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );

				// Read Image Data
				offset = 18 + tgaheader.IDLength + (tgaheader.ColorMapDepth >> 3)*tgaheader.ColorMapLength;
				NFileSeek( pfile,offset,NFILE_SEEK_SET);
				NFileRead( pfile,NGetImageSurface(pimg), sizeof(Nu16), tgaheader.Width*tgaheader.Height );
				break;
			
				// 24 Bits / Pixel : True Color		8 8 8 
			case 24:
				// for a 24 Bpp Tga file, ImageType should be 2 ...
				// ... that means TRUE COLOR 24 bits
				NErrorIf( tgaheader.ImageType!=2, NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );
				
				NErrorIf( tgaheader.ColorMapType,		NERROR_TGA_RGB24_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength	,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,		NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );
				
				// Read Image Data
				offset = 18 + tgaheader.IDLength + (tgaheader.ColorMapDepth >> 3)*tgaheader.ColorMapLength;
				NFileSeek( pfile,offset,NFILE_SEEK_SET);

				psurface = NGetImageSurface(pimg);
				for( i=tgaheader.Width*tgaheader.Height; i!=0; i--,psurface +=3 )
				{
					NFileRead( pfile,read_byte ,sizeof(Nu8), 3 );
					psurface[0] = read_byte[2];
					psurface[1] = read_byte[1];
					psurface[2] = read_byte[0];
				}
				break;
			
				// 32 Bits / Pixel : True Color		8 8 8 8
			case 32:
				// for a 32 Bpp Tga file, ImageType should be 2 ...
				// ... that means TRUE COLOR 32 bits
				NErrorIf( tgaheader.ImageType!=2, NERROR_TGA_INCONSISTENT_TYPE_AND_BPP );

				NErrorIf( tgaheader.ColorMapType,		NERROR_TGA_RGB32_SHOULDNT_HAVE_COLORMAP );
				NErrorIf( tgaheader.ColorMapLength	,	NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL );
				NErrorIf( tgaheader.ColorMapDepth,		NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL );
				psurface = NGetImageSurface(pimg);

				for( i=tgaheader.Width*tgaheader.Height; i!=0; i--,psurface +=4 )
				{
					NFileRead( pfile,read_byte ,sizeof(Nu8), 4 );
					psurface[0] = read_byte[2];
					psurface[1] = read_byte[1];
					psurface[2] = read_byte[0];
					psurface[3] = read_byte[3];
				}
				break;
		}

	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + FLIP IMAGE ?
	//		According to the TGA file format, image data may be inverted
	//		BIT_4 and BIT_5 of the 'HEADER_TGA.Flag' member allow us to know
	//		the Image orientation, vertically and horizontally 
	//
	//									IMAGE ORIGIN
	//		SCREEN DESTINATION	 |		Ver.	Hor.
	//		 OF FIRST PIXEL		 |		Bit5	Bit4
	//		--------------------------------------------
	//			BOTTOM LEFT		 |		 0		 0
	//		--------------------------------------------
	//			BOTTOM RIGHT	 |		 0		 1
	//		--------------------------------------------
	//			TOP LEFT		 |		 1		 0	
	//		--------------------------------------------
	//			TOP RIGHT		 |		 1		 1
	//		--------------------------------------------
	//
	//		N needs image with a TOP LEFT first pixel.
	if( ISFLAG_OFF(tgaheader.Flag,BIT_5) )
	{
		NFlipImageVertically(pimg);
	}
	
	if( ISFLAG_ON(tgaheader.Flag,BIT_4) )
	{
		NFlipImageHorizontally(pimg);
	}
	

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + END:

	NFileClose(pfile);

	return pimg;
}

Nbool NSaveTga( NIMAGE *psrc_img, const Nchar *pfilename )
{
	return NFALSE;
}
Nbool NSaveTgaRLE( NIMAGE *psrc_img, const Nchar *pfilename )
{
	return NFALSE;
}