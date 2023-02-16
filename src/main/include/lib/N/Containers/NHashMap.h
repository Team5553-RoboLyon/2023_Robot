#ifndef __NHASHMAP_H
#define __NHASHMAP_H

#include "../NCStandard.h"
#include "../NType.h"
#include "../NFlags.h"
#include "../NErrorHandling.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **									NHashMap.h										**
// ***************************************************************************************
#define CONSTANT_NOAHASHMAP_MAX_DENSITY		0.7f
#define DEFAULT_NOAHASHMAP_CAPACITYPOWOFTWO	64
#define DEFAULT_NOAHASHMAP_HASH_FCT			NHashMap_optimizedFletcher32_SmallString_Hash_FCT
#define DEFAULT_NOAHASHMAP_KEYCOMPARE_FCT	NHashMap_String_Compare_FCT

#define CONSTANT_NSCHASHMAP_MAX_DENSITY		0.8f
#define DEFAULT_NSCHASHMAP_CAPACITYPOWOFTWO	64
#define DEFAULT_NSCHASHMAP_HASH_FCT			NHashMap_optimizedFletcher32_SmallString_Hash_FCT
#define DEFAULT_NSCHASHMAP_KEYCOMPARE_FCT	NHashMap_String_Compare_FCT


typedef void (*NHASHENTRYDATA_DESTRUCTOR_CALLBACK)(void *pdata);
typedef Nbool(*NHASHENTRYDATA_CONDITIONAL_DESTRUCTOR_CALLBACK)(void *pdata);
typedef Nu32 (*NHASHMAP_HASH_FCT)(const void *pkey);
typedef Ns32 (*NHASHMAP_KEYCOMPARE_FCT)(const void *pkey1,const void *pkey2);

typedef struct NSCHASHENTRY	NSCHASHENTRY;
struct NSCHASHENTRY
{
	void			*pData;
	void			*pKey;
	NSCHASHENTRY	*pNext;
};

typedef struct  
{
	NSCHASHENTRY				**ppBuckets;
	Nu32						Capacity;			
	Nu32						BucketCount;
	Nu32						BucketCountMax;	// Calculated at each Capacity change with CONSTANT_NOAHASHMAP_MAX_DENSITY.
	NHASHMAP_HASH_FCT			Hash_FCT;
	NHASHMAP_KEYCOMPARE_FCT		KeyCompare_FCT;
}NSCHASHMAP;

#define FLAG_NOAHASHENTRY_ACTIVE	BIT_0
#define FLAG_NOAHASHENTRY_BUSY		BIT_1
typedef struct NOAHASHENTRY	NOAHASHENTRY;
struct NOAHASHENTRY
{
	void			*pData;
	void			*pKey;
	Nu32			Flags;
};

typedef struct  
{
	NOAHASHENTRY				*pBuckets;
	Nu32						Capacity;//Size;			
	Nu32						BucketCountMax;	// Calculated at each Capacity change with CONSTANT_NOAHASHMAP_MAX_DENSITY.
	Nu32						BucketCount;

	NHASHMAP_HASH_FCT			Hash_FCT;
	NHASHMAP_KEYCOMPARE_FCT		KeyCompare_FCT;
}NOAHASHMAP;

#define FLAG_NHASHMAP_DESC_USE_CAPACITY_POWOFTWO		BIT_0
#define FLAG_NHASHMAP_DESC_USE_HASH_FCT					BIT_1
#define FLAG_NHASHMAP_DESC_USE_KEYCOMPARE_FCT			BIT_2

#define FLAG_NHASHMAP_DESC_USE_ALLPARAMS				(FLAG_NHASHMAP_DESC_USE_CAPACITY_POWOFTWO|FLAG_NHASHMAP_DESC_USE_HASH_FCT|FLAG_NHASHMAP_DESC_USE_KEYCOMPARE_FCT)

typedef struct  
{
	Nu32						Flags;
	Nu32						Capacity_PowOfTwo;
	NHASHMAP_HASH_FCT			Hash_FCT;
	NHASHMAP_KEYCOMPARE_FCT		KeyCompare_FCT;
}NHASHMAP_DESC;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
// - Open Addressing Hash Map
//
NOAHASHMAP*				NSetupOAHashMap(NOAHASHMAP *phashMap, const NHASHMAP_DESC *pdesc);
NOAHASHMAP*				NCreateOAHashMap(const NHASHMAP_DESC pdesc);
void					NClearOAHashMap(NOAHASHMAP *phashMap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor);
void					NDeleteOAHashMap(NOAHASHMAP *phashMap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );

void*					NInsertOAHashMapEntry(NOAHASHMAP *phashMap, const void *pkey, const void *pdata);
void*					NLookupOAHashMapEntry(const NOAHASHMAP *phashmap, const void *pkey);
void					NClearOAHashMapEntry(NOAHASHMAP *phashmap, const void *pkey, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );
void					NClearAllOAHashMapEntries(NOAHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );
Nu32					NClearAllOAHashMapEntriesXtd(NOAHASHMAP *phashmap, const NHASHENTRYDATA_CONDITIONAL_DESTRUCTOR_CALLBACK pconditionaldatadestructor );

void*					NRemoveOAHashMapEntry(NOAHASHMAP *phashmap, const void *pkey);
void					NRemoveAllOAHashMapEntries(NOAHASHMAP *phashmap);

inline Nu32				NGetOAHashMapCount(const NOAHASHMAP *phashmap){return phashmap->BucketCount;}
inline Nu32				NGetOAHashMapCapacity(const NOAHASHMAP *phashmap){return phashmap->Capacity;}

// Low Level
void					NReHashOAHashMap(NOAHASHMAP *phashMap);

void*					NGetOAHashMapIndexEntry(const NOAHASHMAP *phashmap, const Nu32 entry_index);

void*					NLookupValidOAHashMapIndexEntry(const NOAHASHMAP *phashmap, const Nu32 validentry_index);
Nu32					NLookupValidOAHashMapEntryIndex(const NOAHASHMAP *phashmap, const void *pkey);

// Nu32					NLookupOAHashMapEntryIndex(NOAHASHMAP *phashmap, const void *pkey);
// ---------------------------------------------------------------------------------------
// - Separate Chaining Hash Map
//
NSCHASHMAP*				NSetupSCHashMap(NSCHASHMAP *phashmap,  const NHASHMAP_DESC *pdesc);
void					NClearSCHashMap(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor);
NSCHASHMAP*				NCreateSCHashMap(const NHASHMAP_DESC *pdesc);
void					NDeleteSCHashMap(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );

void*					NInsertSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey, const void *pdata);
void*					NLookupSCHashMapEntry(const NSCHASHMAP *phashmap, const void *pkey);
void					NClearSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );
void					NClearAllSCHashMapEntries(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor );

void*					NRemoveSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey);
void					NRemoveAllSCHashMapEntries(NSCHASHMAP *phashmap);


inline Nu32				NGetSCHashMapCount(const NSCHASHMAP *phashmap){return phashmap->BucketCount;}
inline Nu32				NGetSCHashMapCapacity(const NSCHASHMAP *phashmap){return phashmap->Capacity;}

//Low Level
void					NReHashSCHashMap(NSCHASHMAP *phashmap);

// ***************************************************************************************
// **								 HashMap Callbacks									**
// ***************************************************************************************
// Hash functions
Nu32			NHashMap_optimizedFletcher32_SmallString_Hash_FCT(const void *pkey);	// the default one
Nu32			NHashMap_Fletcher32_String_Hash_FCT(const void *pkey);
Nu32			NHashMap_OAAT_String_Hash_FCT(const void *pkey);
Nu32			NHashMap_SimpleNu32ID_Hash_FCT(const void* pkey);

// KeyCompare functions
Ns32			NHashMap_String_Compare_FCT(const void*pkey1, const void*pkey2);		// the default one
Ns32			NHashMap_SimpleNu32ID_Compare_FCT(const void* pkey1, const void* pkey2);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NHASHMAP_H 

