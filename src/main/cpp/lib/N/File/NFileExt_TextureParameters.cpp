#include "../NCStandard.h"
#include "../NType.h"
#include "../NString.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// NFileWriteTextureParameters
// ------------------------------------------------------------------------------------------
// Description :
//	Write all Texture OpenGl parameters used by NEngine ...
// ------------------------------------------------------------------------------------------
// In  :
//		pfile		:	a Valid pointer on an open file
//		ptexture	:	a valid pointer on a valid NTEXTURE Structure. 
// Out :
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteTextureParameters(NFILE *pfile, const NTEXTURE *ptexture)
{
	NErrorIf(!ptexture || !pfile, NERROR_NULL_POINTER);

	NHEADER_TEXTURE_PARAMETERS	header;
	Nmem0(&header,NHEADER_TEXTURE_PARAMETERS);
	glBindTexture(GL_TEXTURE_2D,ptexture->GL_Texture);
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	&header.Min_Filter );
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	&header.Mag_Filter );
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		&header.Wrap_s );
	glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		&header.Wrap_t );
	
	if( NFileWriteVersionTag(pfile,VERSION_NHEADER_TEXTURE_PARAMETERS) != 1)
		return 0;
	
	if( NFileWrite(pfile,&header,sizeof(NHEADER_TEXTURE_PARAMETERS),1) != 1)
		return 0;

	return 1;
}

// ------------------------------------------------------------------------------------------
// NFileReadTextureParameters
// ------------------------------------------------------------------------------------------
// Description :
//	Read all Texture OpenGl parameters used by NEngine ...
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			: a Valid pointer on an open file
//		ptexture	:	a valid pointer on a valid NTEXTURE Structure. 
// Out :
//		
// ------------------------------------------------------------------------------------------
Nu32 NFileReadTextureParameters(NFILE *pfile, NTEXTURE *ptexture)
{
	NErrorIf(!ptexture || !pfile, NERROR_NULL_POINTER);

	NHEADER_TEXTURE_PARAMETERS	header;
	Nu32						version;
	
	// Check Incoming version.
	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
	case NGETVERSION_MAIN( VERSION_NHEADER_TEXTURE_PARAMETERS ):
		Nmem0(&header,NHEADER_TEXTURE_PARAMETERS);
		
		if( NFileRead(pfile,&header,sizeof(NHEADER_TEXTURE_PARAMETERS),1) != 1 )
			return 0;

		glBindTexture(GL_TEXTURE_2D,ptexture->GL_Texture);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	header.Min_Filter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	header.Mag_Filter );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		header.Wrap_s );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		header.Wrap_t );
		// store the same data directly into texture structure
		ptexture->MinFilter	= header.Min_Filter;
		ptexture->MagFilter	= header.Mag_Filter;
		ptexture->WrapS		= header.Wrap_s;
		ptexture->WrapT		= header.Wrap_t;


		return 1;

		// Unknown version:
	default:
		NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}

