#ifndef __NTGA_H
#define __NTGA_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTga.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NErrorHandling.h"
#include "NImage.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define		EXTENSION_TGA		".tga"
#pragma pack(push, 1)
typedef struct
{
	Nu8		IDLength;		// Identification Length
	Nu8		ColorMapType;	// Color map type
	Nu8		ImageType;		// Image Type

	Nu16	ColorMapOrigin;	// First color map entry index
	Nu16	ColorMapLength;	// Number of color map entry
	Nu8		ColorMapDepth;	// Size of one color map entry

	Nu16	Origin_X;		// Image : X origin
	Nu16	Origin_Y;		// Image : Y origin
	Nu16	Width;			 
	Nu16	Height;		 
	Nu8		Bpp;			// Per Pixel Bit number
	Nu8		Flag;			
}NHEADER_TGA;
#pragma pack(pop)

NIMAGE*	NCreateImageFromTga( const Nchar *pfilename );
Nbool	NSaveTga( NIMAGE *psrc_img, const Nchar *pfilename );
Nbool	NSaveTgaRLE( NIMAGE *psrc_img, const Nchar *pfilename );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NTGA_H 

