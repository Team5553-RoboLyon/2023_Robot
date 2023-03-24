#include "NGeometryBlend.h"
#include "../../NString.h"
#include "../../NErrorHandling.h"
#include "../../File/NFile.h" 
#include "../../File/NFileExt.h" 

// ------------------------------------------------------------------------------------------
// NSetupBlend
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NBLEND*	NSetupBlend(NBLEND *pblend, const Nchar *pname, const GLenum equation, const GLenum sfactor, const GLenum dfactor )
{
	Nmem0(pblend,NBLEND);
	pblend->pName = NStrDuplicate(pname);
	pblend->Blend0_Equation	= equation;
	pblend->Blend0_Sfactor	= sfactor;
	pblend->Blend0_Dfactor	= dfactor;

	return pblend;
}
// ------------------------------------------------------------------------------------------
// NCreateBlend
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NBLEND*	NCreateBlend(const Nchar *pname, GLenum equation, GLenum sfactor, GLenum dfactor )
{
	return NSetupBlend(NEW(NBLEND),pname,equation,sfactor,dfactor);
}

// ------------------------------------------------------------------------------------------
// NClearBlend
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void	NClearBlend(NBLEND *pblend)
{
	if(pblend->pName)
	{
		Nfree(pblend->pName);
	}
	Nmem0(pblend,NBLEND);
}
// ------------------------------------------------------------------------------------------
// NDeleteBlend
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void	NDeleteBlend(NBLEND *pblend)
{
	NClearBlend(pblend);
	Nfree(pblend);
}

NBLEND*	NSetupBlend_Separate(NBLEND *pblend, const Nchar *pname, const GLenum equation0, const GLenum sfactor0, const GLenum dfactor0, const GLenum equation1, const GLenum sfactor1, const GLenum dfactor1 )
{
	Nmem0(pblend,NBLEND);
	
	FLAG_ON(pblend->Flags,FLAG_NBLEND_SEPARATE);
	pblend->pName = NStrDuplicate(pname);
	
	pblend->Blend0_Equation	= equation0;
	pblend->Blend0_Sfactor	= sfactor0;
	pblend->Blend0_Dfactor	= dfactor0;

	pblend->Blend1_Equation	= equation1;
	pblend->Blend1_Sfactor	= sfactor1;
	pblend->Blend1_Dfactor	= dfactor1;
	return pblend;

}
// ------------------------------------------------------------------------------------------
// NCreateBlend_Separate
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NBLEND*	NCreateBlend_Separate(const Nchar *pname,const GLenum equation0, const GLenum sfactor0, const GLenum dfactor0, const GLenum equation1, const GLenum sfactor1, const GLenum dfactor1 )
{
	return NSetupBlend_Separate(NEW(NBLEND),pname, equation0, sfactor0, dfactor0, equation1, sfactor1, dfactor1 );
}


NBLEND* NCreateBlendFromFile( const Nchar* pblendname, const Nchar* pfilename )
{
	// First basic check (debug only)
	NErrorIf( !pfilename,NERROR_NULL_POINTER );
	NErrorIf( !NCheckFileType(pfilename, EXTENSION_NBLEND),NERROR_FILE_INVALID_EXTENSION );

	// Open file check  BLEND signature and read Header
	NFILE *pfile = NFileOpen( pfilename,FLAG_NFILE_READ|FLAG_NFILE_BINARY );
	if(pfile)
	{
		// File signature verification
		// .......................................................................
		Nu32	signature;
		if( NFileReadSignatureTag(pfile,&signature) != 1)
		{
			NFileClose(pfile);
			return NULL;
		}

		NErrorIf( !NSIGNATURECMP( signature,SIGNATURE_NBLEND), NERROR_FILE_UNKNOWN_SIGNATURE  );
		
		NBLEND *pblend = NEW(NBLEND);
		Nmem0(pblend,NBLEND);
		if( !NFileReadBlendValues(pfile,pblend) )
		{
			NDeleteBlend(pblend);
			NFileClose(pfile);
			return NULL;
		}
		pblend->pName = NStrDuplicate(pblendname);

		NFileClose(pfile); // EvenIf NFileClose returns Failed ... No need to test because loading is finished just before !!!! 
		return pblend;
	}
	else
	{
		return NULL;
	}
}

NBLEND* NSaveBlend( const NBLEND *pblend, const Nchar* pfilename )
{
	// First basic check (debug only)
	NErrorIf( !pfilename,NERROR_NULL_POINTER );
	NErrorIf( !NCheckFileType(pfilename, EXTENSION_NBLEND),NERROR_FILE_INVALID_EXTENSION );

	// Open file check  BLEND signature and read Header
	NFILE *pfile = NFileOpen( pfilename,FLAG_NFILE_WRITE|FLAG_NFILE_BINARY );
	if(pfile)
	{
		if( NFileWriteSignatureTag(pfile,SIGNATURE_NBLEND) != 1 )
		{
			NFileClose(pfile);
			return NULL;
		}

		if( !NFileWriteBlendValues(pfile,pblend) )
		{
			NFileClose(pfile);
			return NULL;
		}

		if( NFileClose(pfile) != 0)	// It's saving, so if file doesn't close in the right way it could be impossible to open it later ...
			return NULL;			// So it's why function returns NULL ( for saving failure ).

		return (NBLEND*)pblend;
	}
	else
	{
		return NULL;
	}
}
