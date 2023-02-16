#include "NGeometryMaterial.h"
#include "../../NString.h"
#include "../../NErrorHandling.h"
#include "../../File/NFile.h" 
#include "../../File/NFileExt.h" 

// ------------------------------------------------------------------------------------------
// NSetupMaterial
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NMATERIAL*	NSetupMaterial(NMATERIAL *pmat, const Nchar *pname, const NMATERIAL_DESC *pdesc )
{
	Nmem0(pmat,NMATERIAL);
	pmat->pName = NStrDuplicate(pname);
	pmat->AmbientColor	= pdesc->AmbientColor;
	pmat->DiffuseColor	= pdesc->DiffuseColor;
	pmat->EmissionColor = pdesc->EmissionColor;
	pmat->SpecularColor	= pdesc->SpecularColor;
	pmat->Shininess		= pdesc->Shininess;

	return pmat;
}
// ------------------------------------------------------------------------------------------
// NCreateMaterial
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NMATERIAL*	NCreateMaterial(const Nchar *pname, const NMATERIAL_DESC *pdesc )
{
	return NSetupMaterial(NEW(NMATERIAL),pname,pdesc);
}

// ------------------------------------------------------------------------------------------
// NClearMaterial
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void	NClearMaterial(NMATERIAL *pmaterial)
{
	if(pmaterial->pName)
	{
		Nfree(pmaterial->pName);
	}
	Nmem0(pmaterial,NMATERIAL);
}
// ------------------------------------------------------------------------------------------
// NDeleteMaterial
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void	NDeleteMaterial(NMATERIAL *pmaterial)
{
	NClearMaterial(pmaterial);
	Nfree(pmaterial);
}

NMATERIAL* NCreateMaterialFromFile( const Nchar* pmaterialname, const Nchar* pfilename )
{
	// First basic check (debug only)
	NErrorIf( !pfilename,NERROR_NULL_POINTER );
	NErrorIf( !NCheckFileType(pfilename, EXTENSION_NMATERIAL),NERROR_FILE_INVALID_EXTENSION );

	// Open file check  BLEND signature and read Header
	NFILE *pfile = NFileOpen( pfilename,FLAG_NFILE_READ|FLAG_NFILE_BINARY );
	if(pfile)
	{
		// File signature verification
		// .......................................................................
		Nu32	signature;
		if( NFileReadSignatureTag(pfile,&signature) != 1 )
		{
			NFileClose(pfile);
			return NULL;
		}
		
		NErrorIf( !NSIGNATURECMP( signature,SIGNATURE_NMATERIAL), NERROR_FILE_UNKNOWN_SIGNATURE  );
		NMATERIAL *pmaterial = NEW(NMATERIAL);
		Nmem0(pmaterial,NMATERIAL);
		
		if( !NFileReadMaterialValues(pfile,pmaterial) )
		{
			NDeleteMaterial(pmaterial);
			NFileClose(pfile);
			return NULL;
		}
		
		pmaterial->pName = NStrDuplicate(pmaterialname);
		
		NFileClose(pfile); // EvenIf NFileClose returns Failed ... No need to test because loading is finished just before !!!! 
		return pmaterial;
	}
	else
	{
		return NULL;
	}
}

NMATERIAL* NSaveMaterial( const NMATERIAL *pmaterial, const Nchar* pfilename )
{
	// First basic check (debug only)
	NErrorIf( !pfilename,NERROR_NULL_POINTER );
	NErrorIf( !NCheckFileType(pfilename, EXTENSION_NMATERIAL),NERROR_FILE_INVALID_EXTENSION );

	// Open file check  BLEND signature and read Header
	NFILE *pfile = NFileOpen( pfilename,FLAG_NFILE_WRITE|FLAG_NFILE_BINARY );
	if(pfile)
	{
		if(NFileWriteSignatureTag(pfile,SIGNATURE_NMATERIAL)!=1)
		{
			NFileClose(pfile);
			return NULL;
		}

		if( !NFileWriteMaterialValues(pfile,pmaterial) )
		{
			NFileClose(pfile);
			return NULL;
		}

		if( NFileClose(pfile) != 0)	// It's saving, so if file doesn't close in the right way it could be impossible to open it later ...
			return NULL;			// So it's why function returns NULL ( for saving failure ).
		
		return (NMATERIAL*)pmaterial;
	}
	else
	{
		return NULL;
	}
}
