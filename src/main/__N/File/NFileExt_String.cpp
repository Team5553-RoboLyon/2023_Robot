#include "../NCStandard.h"
#include "../NType.h"
#include "../NString.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../NErrorHandling.h"


/*
// ------------------------------------------------------------------------------------------
// NFileWriteString
// ------------------------------------------------------------------------------------------
// Description :
// Write a string into a Binary file
// ------------------------------------------------------------------------------------------
// In  :
//		pfile	:	a Valid pointer on an open file
//		pstr	:	a valid pointer on a valid String of Nchar, 
//					terminated with a null character
//		count	:	Number of char to write, null terminated char EXCLUDED !
//					The number of char written is going to be len(pstr)
//					at maximum, even if 'count' is greater than this value.
//					if count is less than len(pstr) only the first "count" charaters
//					are going to be write.
// Out :
//		number of character written.	
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteString(NFILE *pfile, const Nchar *pstr, const Nu32 count)
{
	NHEADER_STRING	header;

	NFileWriteVersionTag(pfile,VERSION_NHEADER_STRING);
	Nmem0(&header,NHEADER_STRING);
	
	if(pstr)
	{
		header.Size = NMIN(strlen(pstr),count);
	}
	else
	{
		header.Size = 0;
	}

	NFileWrite(pfile,&header,sizeof(NHEADER_STRING),1);
	if(header.Size)
	{
		return NFileWrite(pfile,pstr,sizeof(Nchar),header.Size);
	}
	else
	{
		return 0;
	}
}

// ------------------------------------------------------------------------------------------
// NFileReadTextureReference
// ------------------------------------------------------------------------------------------
// Description :
// Read a texture Pointer reference written with "NFileWriteTextureReference"
// !! This function doesn't load a texture file but a reference on it.
// It's help full reload something which have pointers on textures. 
// Notice that if the texture referenced is not already loaded in memory this function
// is going to load it before returning a valid texture pointer.
// !!! As the "NCreateTextureFromFile" or "NCreateTexture" functions this one doesn't increase the texture 
// reference counter. So you have to do it by yourself !
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			:	a Valid pointer on an open file
// Out :
//		a valid pointer on a valid NTEXTURE or NULL if there is a problem
// ------------------------------------------------------------------------------------------
Nchar* NFileReadString(NFILE *pfile,Nchar *pstr, const Nu32 count)
{
	NHEADER_STRING	header;
	Nu32						version;


	// Check Incoming version.
	NFileReadVersionTag(pfile,&version);
	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
		case NGETVERSION_MAIN( VERSION_NHEADER_STRING ):
			memset(&header,0,sizeof(NHEADER_STRING));
			NFileRead(pfile,&header,sizeof(NHEADER_STRING),1);
			if( header.Size == 0 )
				return NULL; // it's not an error, just a reference on a NULL String pointer was "written"
				
			//todo: if pstr then read but copy only count character. and if not pstr tehn "malloc" its why 
			// we thought that it was not a good idea to write this function !!!

			return pstr;

		// Unknown version:
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return NULL;
	}
	return NULL;
}
*/

