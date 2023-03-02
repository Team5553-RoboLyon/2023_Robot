// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
#ifdef _NLIBS
// ------------------------------------------------------------------------------------------------------------------------------------------
// -
// -
// -
#include "../Containers/NHashMap.h"
#include "../NTexture.h"
#include "../NCore.h"
#include "../Image/NTga.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/File/NFile.h"
#include "lib/N/NString.h"

#include "NLibraries.h"

NOAHASHMAP TexturesLibrary;

Nbool _delete_unreferenced_texture(void *poahashmapdata)
{
	if (!((NTEXTURE *)poahashmapdata)->ReferenceCount)
	{
		printf("\n PURGE TEXTURE ---> Texture %s Deleted.", ((NTEXTURE *)poahashmapdata)->pName);
		NDeleteTexture((NTEXTURE *)poahashmapdata);
		return NTRUE;
	}
	else
	{
		return NFALSE;
	}
}

void _delete_texture(void *poahashmapdata)
{
	NDeleteTexture((NTEXTURE *)poahashmapdata);
}

void NInitTexturesLibrary()
{
	NHASHMAP_DESC desc;
	Nmem0(&desc, NHASHMAP_DESC);
	FLAG_ON(desc.Flags, FLAG_NHASHMAP_DESC_USE_ALLPARAMS);
	desc.Capacity_PowOfTwo = CONSTANT_NTEXTURESLIBRARY_CAPACITY;
	desc.Hash_FCT = NHashMap_optimizedFletcher32_SmallString_Hash_FCT;
	desc.KeyCompare_FCT = NHashMap_String_Compare_FCT;
	NSetupOAHashMap(&TexturesLibrary, &desc);
}

void NDisableTexturesLibrary()
{
	NErrorIf(NGetOAHashMapCount(&TexturesLibrary), NERROR_HASHMAP_WRONG_BUCKETCOUNT); // It has to be Empty
	NClearOAHashMap(&TexturesLibrary, NULL);
}

// return incoming ptexture or the one which was previously inserted with the same keyname
NTEXTURE *NInsertLibraryTexture(const NTEXTURE *ptexture)
{
	NErrorIf(!ptexture->pName, NERROR_HASHMAP_INVALID_KEY);
	NErrorIf(!strlen(ptexture->pName), NERROR_HASHMAP_INVALID_KEY);

	NTEXTURE *pret = (NTEXTURE *)NInsertOAHashMapEntry(&TexturesLibrary, ptexture->pName, ptexture);
	NErrorIf(pret != ptexture, NERROR_NLIBRARY_NON_UNIQUE_ENTRY); // That means an already inserted texture with the same Name was replaced by "ptexture"
																  // pTexture->pName should be unique ! So in DEBUG ... CRASH !
	return pret;												  // In RELEASE just return the pointer
}

NTEXTURE *NLookupLibraryTexture(const Nchar *pkeyname)
{
	return (NTEXTURE *)NLookupOAHashMapEntry(&TexturesLibrary, (const void *)pkeyname);
}

NTEXTURE *NGetLibraryTexture(const Nchar *pkeyname)
{
	NTEXTURE *ptexture = (NTEXTURE *)NLookupOAHashMapEntry(&TexturesLibrary, (const void *)pkeyname);
	if (!ptexture && NCheckFileType(pkeyname, EXTENSION_TGA))
	{
		// 		NErrorIf(NIsRootRelativePathFileName(pkeyname)!=NIsRootRelativeDefaultPathSystem(),NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY );
		ptexture = NCreateTextureFromTga(pkeyname, pkeyname);
		if (ptexture)
		{
			NInsertOAHashMapEntry(&TexturesLibrary, ptexture->pName, ptexture);
		}
#ifdef _DEBUG
		else
		{
			NErrorIf(1, NERROR_NULL_POINTER); // Not an error but it seems strange user ask for a texture that doesn't exist !
			return NULL;
		}
#endif
	}
	return ptexture;
}

Nu32 NPurgeTextureLibrary()
{
#ifdef _DEBUG
	printf("\n PURGE TEXTURE ---> Texture nb BEFORE purge:%d", TexturesLibrary.BucketCount);
	Nu32 ret = NClearAllOAHashMapEntriesXtd(&TexturesLibrary, _delete_unreferenced_texture);
	printf("\n PURGE TEXTURE ---> Texture nb AFTER purge:%d", TexturesLibrary.BucketCount);
	return ret;
#endif

#ifndef _DEBUG
	return NClearAllOAHashMapEntriesXtd(&TexturesLibrary, _delete_unreferenced_texture);
#endif
}

Nu32 NGetTexturesLibraryCapacity()
{
	return TexturesLibrary.Capacity;
}
Nu32 NGetTexturesLibraryCount()
{
	return TexturesLibrary.BucketCount;
}
NTEXTURE *NLookupLibraryTextureByValidIndex(const Nu32 validentry_index)
{
	return (NTEXTURE *)NLookupValidOAHashMapIndexEntry(&TexturesLibrary, validentry_index);
}
Nu32 NGetLibraryTextureValidIndex(const Nchar *pkeyname)
{
	return NLookupValidOAHashMapEntryIndex(&TexturesLibrary, pkeyname);
}

void NDeleteAllLibraryTextures()
{
	NClearAllOAHashMapEntries(&TexturesLibrary, _delete_texture);
}

void NDeleteLibraryTexture(const Nchar *pkeyname)
{
	NClearOAHashMapEntry(&TexturesLibrary, pkeyname, _delete_texture);
}

NTEXTURE *NRemoveLibraryTexture(const Nchar *pkeyname)
{
	return (NTEXTURE *)NRemoveOAHashMapEntry(&TexturesLibrary, pkeyname);
}

// ------------------------------------------------------------------------------------------
// void NReloadAllLibraryTextures
// ------------------------------------------------------------------------------------------
// Description :
//	Especially for Android... (evenif this function stills available for all systems )
//	To fix/patch the OpenGL context lost on Android Systems...
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NReloadAllLibraryTextures()
{
	Nu32 i;
	NTEXTURE *ptexture;
	Nchar *ptexturename;

	// Pass trough all the Textures Library (OAHashMap) to retrieve valid "Texture Ptr"
	for (i = 0; i < TexturesLibrary.Capacity; i++)
	{
		ptexture = (NTEXTURE *)NGetOAHashMapIndexEntry(&TexturesLibrary, i);
		if (ptexture)
		{
			ptexturename = NStrDuplicate(ptexture->pName);
			NChangeTextureImageFromTga(ptexture, ptexturename);
			Nfree(ptexturename);
		}
	}
}

/*
// TEXTURES LIBRARY BANK Management
Nu8 NGetTextureBank(NTEXTURE *ptexture)
{
	return ptexture->BankID;
}


Nu32 NReadTextureBank(NFILE *pfile, const Nu8 bank_id)
{

}
Nu32 NWriteTextureBank(NFILE *pfile, const Nu8 bank_id)
{

}
Nu32 NLoadTextureBank(const Nchar *pname, const Nu8 bank_id)
{

}
Nu32 NSaveTextureBank(const Nchar *pname, const Nu8 bank_id)
{

}
void NReplaceTextureBank(const Nchar *pname, const Nu8 bank_id)
{

}
void NDeleteTextureBank(const Nu8 bank_id)
{

}*/

// -
// -
// -
// ------------------------------------------------------------------------------------------------------------------------------------------
#endif // _NLIBS
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
