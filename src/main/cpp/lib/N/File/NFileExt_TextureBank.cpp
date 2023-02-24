#include "lib/N/NCStandard.h"
#include "../NCore.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "lib/N/File/NFile.h"
#include "../File/NFileExt.h"
#include "lib/N/NErrorHandling.h"
#include "../Containers/NHashMap.h"
#include "../Libraries/NLibraries.h"

/*
extern NOAHASHMAP	TexturesLibrary;

// ------------------------------------------------------------------------------------------
// NFileWriteTextureBank
// ------------------------------------------------------------------------------------------
// Description :
//	Write all Textures in Library that belong to the Bank 'bank_id'
//	Writings may be just name references ( = textures names ) or full data ( write all TGA inside the saved bank!)
//	But in both cases Textures Parameters ( Wrap,Filters) are going to be written for each texture of the bank.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile		a Valid pointer on an open file
//		bank_id		a valid bank_id (0 to 255)
// Out :
// ------------------------------------------------------------------------------------------
#define FLAG_NFileWriteTextureBank_TEXTURE_PARAMETERS_INCLUDED		FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED
#define FLAG_NFileWriteTextureBank_TEXTURE_IMAGEDATA_INCLUDED		FLAG_NHEADER_TEXTURE_BANK_TEXTURE_IMAGEDATA_INCLUDED
void NFileWriteTextureBank(NFILE *pfile, const Nu8 bank_id, const Nu32 writetexturebank_flags)
{
	NErrorIf((writetexturebank_flags&MASK_NHEADER_TEXTURE_BANK_ELEMENTS_INCLUDED) != writetexturebank_flags, NERROR_UNAUTHORIZED_FLAG );
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	NHEADER_TEXTURE_BANK	header;
	NTEXTURE				*ptexture;
	Nu32					i;
	Nu32					namesize;

	NFileWriteVersionTag(pfile,VERSION_NHEADER_TEXTURE_BANK);
	Ns32 offset = NFileTell(pfile);
	// Just move forward instead writing the header, it will be written with the right values at the end of the process.
	NFileSeek(pfile,sizeof(NHEADER_TEXTURE_BANK),NFILE_SEEK_CUR);
	Nmem0(&header,NHEADER_TEXTURE_BANK);
	header.Flags |= (writetexturebank_flags&MASK_NHEADER_TEXTURE_BANK_ELEMENTS_INCLUDED);

	// Pass trough all the Textures Library (OAHashMap) to retrieve Textures that belong to the bank.
	for(i=0;i<TexturesLibrary.Size;i++ )
	{
		ptexture = (NTEXTURE*)NGetOAHashMapIndexEntry(&TexturesLibrary,i);
		if(ptexture && ptexture->BankID == bank_id)
		{
//			NErrorIf(!NIsRootRelativePathFileName(ptexture->pName),NERROR_FILE_FILENAME_MUST_BE_ROOT_RELATIVE);
			namesize = strlen(ptexture->pName);
			NErrorIf(!namesize,NERROR_NULL_VALUE);

			NFileWrite(pfile,&namesize,sizeof(Nu32),1);
			NFileWrite(pfile,ptexture->pName,sizeof(Nchar),namesize);

			if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED))
			{
				// TODO
				NErrorIf(1,NERROR_SYSTEM_TODO); // 	NFileWriteTGA(pfile,pimage) or NFileWriteImageData(pfile,pimage);
				// TODO								are not implemented yet...
			}

			if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED))
				NFileWriteTextureParameters(pfile,ptexture);

			header.BankSize ++;
		}
	}

	// Seek back and Write Header content at the right place.
	Ns32 offsetb = NFileTell(pfile);
	NFileSeek(pfile,offset,NFILE_SEEK_SET);
	NFileWrite(pfile,&header,sizeof(NHEADER_TEXTURE_BANK),1);
	// ...and seek to the end position
	NFileSeek(pfile,offsetb,NFILE_SEEK_SET);
}

// ------------------------------------------------------------------------------------------
// NFileReadTextureBank
// ------------------------------------------------------------------------------------------
// Description :
//	Read all Texture OpenGl parameters used by NEngine ...
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			: a Valid pointer on an open file
//		bank_id			: a bank id between 0 and 254 (255 is system reserved and mean: UNUSED)
// Out :
//
// ------------------------------------------------------------------------------------------
void NFileReadTextureBank(NFILE *pfile, const Nu8 bank_id)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	NHEADER_TEXTURE_BANK	header;
	Nu32					version;
	Nu32					i;
	Nu32					namesize;
	NTEXTURE				*ptexture = NULL;
	Nchar					txname[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];
	Nbool					backto_AbsolutePathSystem = NFALSE;

	// Check Incoming version.
	NFileReadVersionTag(pfile,&version);
	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
		case NGETVERSION_MAIN( VERSION_NHEADER_TEXTURE_BANK ):
			Nmem0(&header,NHEADER_TEXTURE_BANK);
			NFileRead(pfile,&header,sizeof(NHEADER_TEXTURE_BANK),1);

			// Turn ON 'RootRelativeDefaultPathSystem' if necessary
/ *
			if( !NIsRootRelativeDefaultPathSystem() )
			{
				backto_AbsolutePathSystem = NTRUE;
				NSetRootRelativePathSystemAsDefault();
			}
* /

			for(i=header.BankSize;i!=0;i--)
			{
				NFileRead(pfile,&namesize,sizeof(Nu32),1);
				NFileRead(pfile,txname,sizeof(Nchar),namesize);
				txname[namesize]=0; // string end character.
				ptexture = NGetLibraryTexture(txname);
				NErrorIf(!ptexture,NERROR_TEXTURE_BANK_TEXTURE_NAME_DOESNT_EXIST);

				ptexture->BankID = bank_id; // If requested texture was already present into the TextureLibrary with an another BankId
											// this one is going to be replace by the new one. So take care

				if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED))
				{
					// TODO
					NErrorIf(1,NERROR_SYSTEM_TODO); // 	NFileReadTGA(pfile,pimage) or NFilereadImageData(pfile,pimage);
					// TODO								are not implemented yet...
				}

				if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED))
					NFileReadTextureParameters(pfile,ptexture);
			}
/ *
			// Go back to AbsolutePathSystem if 'RootRelativeDefaultPathSystem' was forced to ON just before the "loop"
			if(backto_AbsolutePathSystem)
				NSetAbsolutePathSystemAsDefault();
* /
			break;

			// Unknown version:
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			break;
	}
}

*/
