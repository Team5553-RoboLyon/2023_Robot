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
#include "lib/N/NErrorHandling.h"
#include "lib/N/File/NFile.h"
#include "NLibraries.h"

NOAHASHMAP BlendsLibrary;

void NInitBlendsLibrary()
{
	NHASHMAP_DESC desc;
	Nmem0(&desc, NHASHMAP_DESC);
	FLAG_ON(desc.Flags, FLAG_NHASHMAP_DESC_USE_ALLPARAMS);
	desc.Capacity_PowOfTwo = CONSTANT_NBLENDSLIBRARY_CAPACITY;
	desc.Hash_FCT = NHashMap_optimizedFletcher32_SmallString_Hash_FCT;
	desc.KeyCompare_FCT = NHashMap_String_Compare_FCT;
	NSetupOAHashMap(&BlendsLibrary, &desc);
}

void NDisableBlendsLibrary()
{
	NErrorIf(NGetOAHashMapCount(&BlendsLibrary), NERROR_HASHMAP_WRONG_BUCKETCOUNT);
	NClearOAHashMap(&BlendsLibrary, NULL);
}

// return incoming pblend or the one which was previously inserted with the same "keyname".
NBLEND *NInsertLibraryBlend(const NBLEND *pblend)
{
	NErrorIf(!pblend->pName, NERROR_HASHMAP_INVALID_KEY);
	NErrorIf(!strlen(pblend->pName), NERROR_HASHMAP_INVALID_KEY);
	NBLEND *pret = (NBLEND *)NInsertOAHashMapEntry(&BlendsLibrary, pblend->pName, pblend);
	NErrorIf(pret != pblend, NERROR_NLIBRARY_NON_UNIQUE_ENTRY); // That means an already inserted Blend with the same Name was replaced by incoming "pblend"
																// pblend->pName should be unique ! So in DEBUG ... CRASH !
	return pret;												// In RELEASE just return the pointer
}

NBLEND *NLookupLibraryBlend(const Nchar *pkeyname)
{
	return (NBLEND *)NLookupOAHashMapEntry(&BlendsLibrary, (const void *)pkeyname);
}

NBLEND *NGetLibraryBlend(const Nchar *pkeyname)
{
	// This function try to get the "pkeyname" blend into the library and, if is not into try to load it as an external file.
	// !!! important !!! To use this capabilities, "pkeyname" must be the name of the blendfile, exactly like for texture.
	// For example: "maps/gradient0.tga" is a texture pkeyname
	//				"blend/additive.nbl" should be a blend name
	NBLEND *pblend = (NBLEND *)NLookupOAHashMapEntry(&BlendsLibrary, (const void *)pkeyname);
	if (!pblend)
	{
		//		NErrorIf(NIsRootRelativePathFileName(pkeyname)!=NIsRootRelativeDefaultPathSystem(),NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY );
		if (NCheckFileType(pkeyname, EXTENSION_NBLEND))
		{
			pblend = NCreateBlendFromFile(pkeyname, pkeyname);
			NErrorIf(!pblend, NERROR_FILE_READ_ERROR);
			NInsertOAHashMapEntry(&BlendsLibrary, pblend->pName, pblend);
		}
	}
	return pblend;
}

Nu32 NGetBlendsLibraryCapacity()
{
	return BlendsLibrary.Capacity;
}
Nu32 NGetBlendsLibraryCount()
{
	return BlendsLibrary.BucketCount;
}
NBLEND *NLookupLibraryBlendByValidIndex(const Nu32 validentry_index)
{
	return (NBLEND *)NLookupValidOAHashMapIndexEntry(&BlendsLibrary, validentry_index);
}
Nu32 NGetLibraryBlendValidIndex(const Nchar *pkeyname)
{
	return NLookupValidOAHashMapEntryIndex(&BlendsLibrary, pkeyname);
}

static void _delete_blend(void *ptr)
{
	NDeleteBlend((NBLEND *)ptr);
}

void NDeleteAllLibraryBlends()
{
	NClearAllOAHashMapEntries(&BlendsLibrary, _delete_blend);
}
void NDeleteLibraryBlend(const Nchar *pkeyname)
{
	NClearOAHashMapEntry(&BlendsLibrary, pkeyname, _delete_blend);
}
NBLEND *NRemoveLibraryBlend(const Nchar *pkeyname)
{
	return (NBLEND *)NRemoveOAHashMapEntry(&BlendsLibrary, pkeyname);
}

// -
// -
// -
// ------------------------------------------------------------------------------------------------------------------------------------------
#endif // _NLIBS
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
