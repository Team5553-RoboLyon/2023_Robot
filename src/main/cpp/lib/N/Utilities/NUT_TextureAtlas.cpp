#include "../NString.h"
#include "../File/NFile.h"
#include "../Image/NTga.h"
#include "../BasicElements/NTextureAtlas.h"

#include "../Libraries/NLibraries.h"
#include "NUT_TextureAtlas.h"


// ------------------------------------------------------------------------------------------
// void NUT_LoadAtlasList
// ------------------------------------------------------------------------------------------
// Description :
//	Open an external file (which needs to have the right extension ... 'EXTENSION_ATLAS_LIST' )
//	Read it, and open each atlas description or image written into.
//	So, to be clear:
//			- if the line is terminated by 'EXTENSION_ATLAS' function is going to remove this extension and load the line as a TGA file
//			  ... and after that function is going to load 'Atlas_Elements' associated with this texture.																			
//			- if the line is terminated by 'EXTENSION_TGA' function is going to the line as a TGA file and will create a unique atlas element
//			  from this texture ( with identity value for texture coordinates)
// ------------------------------------------------------------------------------------------
// In	:
//			const char*						 filename					filename list ( of Atlas or image )
//			const Nbool						 using_texture_library		NTRUE or NFALSE to use, or not, Texture Library
//			const NLOADATLASELEMENT_CALLBACK callback					a call back to call after each ATLAS ELEMENT creation
//				  Nu32						 Nu32_param					a Nu32 pass trough the callback
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_LoadAtlasList(const char* filename, const Nbool using_texture_library, const NLOADATLASELEMENT_CALLBACK callback, Nu32 Nu32_param)
{
	NATLAS_ELEMENT	*pel;
	NTEXTURE		*ptexture;
	Nchar			tempstring[256];
	Nchar			tempstringb[256];

	NFILE			*pfile;
	Nu32			l;

	NErrorIf( !NCheckFileType(filename,EXTENSION_ATLAS_LIST),NERROR_FILE_INVALID_EXTENSION );

	pfile = NFileOpen(filename, FLAG_NFILE_READ);
	NFileSeek(pfile, 0, NFILE_SEEK_SET);
	
	tempstring[0]=0;
	while( NFileGetString(tempstring, 256, pfile) ) 
	{
		if( ( l=strlen(tempstring) ) )
		{
			l--;
			while( (tempstring[l]=='\n')||(tempstring[l]=='\r'))
			{
				tempstring[l]=0; // to remove the potential end line characters (one or two)
				l--;
			}
		}

		if( strlen(tempstring) )
		{
			// Check 'tempstring' extension
			// 1/ It's an ATLAS.
			if( NCheckFileType(tempstring, EXTENSION_ATLAS) )
			{
				// Load associated TGA first !
				NStrGet_String_BeforeLabel(tempstring,EXTENSION_ATLAS,tempstringb);
				NStrCat(tempstringb,EXTENSION_TGA,256);
				
				ptexture = NULL;
				if( using_texture_library )
				{
					ptexture = NGetLibraryTexture(tempstringb);
				}
				else
				{
					ptexture = NCreateTextureFromTga(tempstringb,tempstringb);
				}

				// ... and load Atlas Elements
				NErrorIf(!ptexture,NERROR_NULL_POINTER);
				if( ptexture )
				{
					NLoadAtlasElements(tempstring,ptexture,callback,Nu32_param);					
				}
			}
			// 2/ It's a simple TGA
			else if( NCheckFileType(tempstring, EXTENSION_TGA) )
			{
				ptexture = NULL;
				if( using_texture_library )
				{
					ptexture = NGetLibraryTexture(tempstring);
				}
				else
				{
					ptexture = NCreateTextureFromTga(tempstring,tempstring);
				}
				NErrorIf(!ptexture,NERROR_NULL_POINTER);
				if( ptexture )
				{
					if( (pel = NCreateAtlasElement(tempstring,ptexture,NULL)) )
					{	
						if(callback)
							callback(pel,Nu32_param);
					}
				}
			}
			// 3/ It's something else ???
			else
			{
				NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
				break; // In release mode, quit process by breaking the loop
			}
		}
		else
		{
			break;
		}
	}

	NFileClose(pfile);
}
