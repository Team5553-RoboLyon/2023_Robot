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
#include "../NErrorHandling.h"
#include "../File/NFile.h"
#include "NLibraries.h"

NOAHASHMAP	MaterialsLibrary;

void NInitMaterialsLibrary()
{
	NHASHMAP_DESC	desc;
	Nmem0(&desc,NHASHMAP_DESC);
	FLAG_ON(desc.Flags,FLAG_NHASHMAP_DESC_USE_ALLPARAMS);
	desc.Capacity_PowOfTwo	= CONSTANT_NMATERIALSLIBRARY_CAPACITY;
	desc.Hash_FCT			= NHashMap_optimizedFletcher32_SmallString_Hash_FCT;
	desc.KeyCompare_FCT		= NHashMap_String_Compare_FCT;
	NSetupOAHashMap(&MaterialsLibrary,&desc);
}

void NDisableMaterialsLibrary()
{
	NErrorIf(NGetOAHashMapCount(&MaterialsLibrary),NERROR_HASHMAP_WRONG_BUCKETCOUNT);
	NClearOAHashMap(&MaterialsLibrary,NULL);
}

// return incoming pmat or the one which was previously inserted with the same "keyname". 
NMATERIAL* NInsertLibraryMaterial(const NMATERIAL *pmat)
{
	NErrorIf(!pmat->pName,NERROR_HASHMAP_INVALID_KEY);
	NErrorIf(!strlen(pmat->pName),NERROR_HASHMAP_INVALID_KEY);
	NMATERIAL *pret = (NMATERIAL*)NInsertOAHashMapEntry(&MaterialsLibrary,pmat->pName,pmat);
	NErrorIf(pret != pmat,NERROR_NLIBRARY_NON_UNIQUE_ENTRY);	// That means an already inserted Material with the same Name was replaced by incoming "pmat"
																// pmat->pName should be unique ! So in DEBUG ... CRASH !
	return pret;												// In RELEASE just return the pointer
}

NMATERIAL* NLookupLibraryMaterial(const Nchar *pkeyname)
{
	return (NMATERIAL*)NLookupOAHashMapEntry(&MaterialsLibrary,(const void*)pkeyname);
}

NMATERIAL* NGetLibraryMaterial(const Nchar *pkeyname)
{
	NMATERIAL *pmat = (NMATERIAL*)NLookupOAHashMapEntry(&MaterialsLibrary,(const void*)pkeyname);
	if( !pmat && NCheckFileType(pkeyname, EXTENSION_NMATERIAL) )
	{
//		NErrorIf(NIsRootRelativePathFileName(pkeyname)!=NIsRootRelativeDefaultPathSystem(),NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY );
		pmat = NCreateMaterialFromFile(pkeyname,pkeyname);
		NInsertOAHashMapEntry(&MaterialsLibrary,pmat->pName,pmat);
	}
	return pmat;
}



Nu32 NGetMaterialsLibraryCapacity()
{
	return MaterialsLibrary.Capacity;
}
Nu32 NGetMaterialsLibraryCount()
{
	return MaterialsLibrary.BucketCount;
}
NMATERIAL* NLookupLibraryMaterialByValidIndex(const Nu32 validentry_index)
{
	return (NMATERIAL*)NLookupValidOAHashMapIndexEntry(&MaterialsLibrary,validentry_index);
}
Nu32 NGetLibraryMaterialValidIndex(const Nchar *pkeyname)
{
	return NLookupValidOAHashMapEntryIndex(&MaterialsLibrary,pkeyname);
}
void _delete_material(void* ptr)
{
	NDeleteMaterial((NMATERIAL*)ptr);
}
void NDeleteAllLibraryMaterials()
{
	NClearAllOAHashMapEntries(&MaterialsLibrary,_delete_material);
}
void NDeleteLibraryMaterial(const Nchar *pkeyname)
{
	NClearOAHashMapEntry(&MaterialsLibrary,pkeyname,_delete_material);
}
NMATERIAL* NRemoveLibraryMaterial(const Nchar *pkeyname)
{
	return (NMATERIAL*)NRemoveOAHashMapEntry(&MaterialsLibrary,pkeyname);
}

// -
// -
// -
// ------------------------------------------------------------------------------------------------------------------------------------------
#endif // _NLIBS
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NLIBS' FLAG IS SET TO ON !!!

