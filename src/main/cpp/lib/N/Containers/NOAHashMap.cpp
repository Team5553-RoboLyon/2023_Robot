﻿// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NOAHashMap.cpp									**
// **																					**
// **							  Open Addressing Hash Map (or Table)					**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../NMath.h"
#include "NHashMap.h"


NOAHASHMAP	*NSetupOAHashMap(NOAHASHMAP *phashmap,  const NHASHMAP_DESC *pdesc)
{
	// Params collect
	if(ISFLAG_ON(pdesc->Flags,FLAG_NHASHMAP_DESC_USE_CAPACITY_POWOFTWO))
	{
		NErrorIf(!NIsPowerOfTwo(pdesc->Capacity_PowOfTwo),NERROR_WRONG_VALUE); // its a constraint ! Hashmap Capacity has to be a power of two ! 
		phashmap->Capacity = pdesc->Capacity_PowOfTwo;
	}
	else
	{
		phashmap->Capacity = DEFAULT_NOAHASHMAP_CAPACITYPOWOFTWO;
	}

	if(ISFLAG_ON(pdesc->Flags,FLAG_NHASHMAP_DESC_USE_HASH_FCT))
	{
		phashmap->Hash_FCT = pdesc->Hash_FCT;
	}
	else
	{
		phashmap->Hash_FCT = DEFAULT_NOAHASHMAP_HASH_FCT;
	}

	if(ISFLAG_ON(pdesc->Flags,FLAG_NHASHMAP_DESC_USE_KEYCOMPARE_FCT))
	{
		phashmap->KeyCompare_FCT = pdesc->KeyCompare_FCT;
	}
	else
	{
		phashmap->KeyCompare_FCT = DEFAULT_NOAHASHMAP_KEYCOMPARE_FCT;
	}
	
	phashmap->pBuckets	= (NOAHASHENTRY *)Nmalloc(sizeof(NOAHASHENTRY)*phashmap->Capacity);
	memset(phashmap->pBuckets,0,sizeof(NOAHASHENTRY)*phashmap->Capacity);
	phashmap->BucketCount	= 0;
	phashmap->BucketCountMax = (Nu32)((Nf32)phashmap->Capacity*CONSTANT_NOAHASHMAP_MAX_DENSITY);
	NErrorIf(!phashmap->BucketCountMax, NERROR_SYSTEM_CHECK); // at minimum 2*CONSTANT_NOAHASHMAP_MAX_DENSITY should be = 1

	return phashmap;
}

void NClearOAHashMap(NOAHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor)
{
	NOAHASHENTRY *pentry = phashmap->pBuckets;
	if( pdatadestructor )
	{
		for(Nu32 i = phashmap->Capacity;i!=0;i--,pentry++)
		{
			if(ISFLAG_ON(pentry->Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				pdatadestructor(pentry->pData);
			}
		}
	}

	Nfree(phashmap->pBuckets);
	Nmem0(phashmap,NOAHASHMAP);
}

NOAHASHMAP *NCreateOAHashMap(const NHASHMAP_DESC *pdesc)
{
	return NSetupOAHashMap(NEW(NOAHASHMAP),pdesc);
}

void NDeleteOAHashMap(NOAHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor )
{
	NClearOAHashMap(phashmap,pdatadestructor);
	Nfree(phashmap);
}

void NReHashOAHashMap(NOAHASHMAP *phashmap)
{
	Nu32			oldcapacity		= phashmap->Capacity;
	NOAHASHENTRY	*poldbuckets	= phashmap->pBuckets;
	NOAHASHENTRY	*pbuck			= poldbuckets;

	phashmap->BucketCount	= 0;
	phashmap->Capacity		= oldcapacity<<1;
	phashmap->pBuckets		= (NOAHASHENTRY *)Nmalloc(sizeof(NOAHASHENTRY)*phashmap->Capacity);
	memset(phashmap->pBuckets,0,sizeof(NOAHASHENTRY)*phashmap->Capacity);
	phashmap->BucketCountMax = (Nu32)((Nf32)phashmap->Capacity*CONSTANT_NOAHASHMAP_MAX_DENSITY);
	NErrorIf(!phashmap->BucketCountMax, NERROR_SYSTEM_CHECK); // at minimum 2*CONSTANT_NOAHASHMAP_MAX_DENSITY should be = 1

	for(Nu32 i=oldcapacity;i!=0;i--,pbuck++)
	{
		if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
			NInsertOAHashMapEntry(phashmap,pbuck->pKey,pbuck->pData);
	}
	
	Nfree(poldbuckets);
}


// return incoming "pdata" in case of successful insertion, or "pold data" in case of replacement !
void* NInsertOAHashMapEntry(NOAHASHMAP *phashmap, const void *pkey, const void *pdata)
{
	NErrorIf(!pdata||!pkey,NERROR_NULL_POINTER);

	if(phashmap->BucketCount >= phashmap->BucketCountMax)
		NReHashOAHashMap(phashmap);


	Nu32 id = phashmap->Hash_FCT(pkey)&(phashmap->Capacity-1);
	while(1)
	{
		if(ISFLAG_OFF(phashmap->pBuckets[id].Flags,FLAG_NOAHASHENTRY_ACTIVE)) // Target bucket is free
		{
			FLAG_ON(phashmap->pBuckets[id].Flags,FLAG_NOAHASHENTRY_ACTIVE);			
			phashmap->pBuckets[id].pData	= (void*)pdata;									
			phashmap->pBuckets[id].pKey		= (void*)pkey;
			phashmap->BucketCount ++;
			return (void*)pdata;
		}
		else if( phashmap->KeyCompare_FCT(phashmap->pBuckets[id].pKey,pkey)==0 )// Target bucket is occupied by data with the same key, NEW DATA are going to replace older
		{
			void *pold = phashmap->pBuckets[id].pData;
			phashmap->pBuckets[id].pData = (void*)pdata;
			return pold;
		}
		else // Collision !!! So lets see the next bucket ...
		{
			id = (id+1)&(phashmap->Capacity-1);
		}
	}
}


void* NRemoveOAHashMapEntry(NOAHASHMAP *phashmap, const void *pkey)
{
	Nu32	i = phashmap->Hash_FCT(pkey)&(phashmap->Capacity-1);
	// Targeted Bucket is empty, so, pKey is not in the table !
	if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
	{
		return NULL;
	}
	else
	{
		for(Nu32 b=phashmap->Capacity;b!=0;b--,i=(i+1)&(phashmap->Capacity-1))
		{
			if(phashmap->KeyCompare_FCT(phashmap->pBuckets[i].pKey,pkey)==0)
			{
				void	*pdata = phashmap->pBuckets[i].pData;
				// Compress
				Nu32	j = i;
				Nu32	k;
				while(1)
				{  
					j = (j+1)&(phashmap->Capacity-1);// modulo
					if(ISFLAG_OFF(phashmap->pBuckets[j].Flags,FLAG_NOAHASHENTRY_ACTIVE))
					{
						break;
					}
					k = phashmap->Hash_FCT(phashmap->pBuckets[j].pKey)&(phashmap->Capacity-1);
					NErrorIf(i==j,NERROR_SYSTEM_GURU_MEDITATION);												
					if( (k <= i) || (k > j) )
					{
						phashmap->pBuckets[i] = phashmap->pBuckets[j];
						i = j;
					}
				}
				// Return initial suppressed bucket associated  data
				FLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE);
				phashmap->BucketCount--;
				return pdata;
			}
			else if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				return NULL;
			}
		}
		return NULL;
	}
}
void NClearOAHashMapEntry(NOAHASHMAP *phashmap, const void *pkey, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor )
{
	if(pdatadestructor)
		pdatadestructor(NRemoveOAHashMapEntry(phashmap,pkey));
	else
		NRemoveOAHashMapEntry(phashmap,pkey);
}

void NRemoveAllOAHashMapEntries(NOAHASHMAP *phashmap)
{
	if(!phashmap->BucketCount)
		return; 

	memset(phashmap->pBuckets,0,sizeof(NOAHASHENTRY)*phashmap->Capacity);
	phashmap->BucketCount = 0; // In release mode there is no progressive decrease, and no test, just force it to zero !

}
void NClearAllOAHashMapEntries(NOAHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor )
{
	if(!phashmap->BucketCount)
		return;

	if(pdatadestructor)
	{
		NOAHASHENTRY *pbuck = phashmap->pBuckets;
		for(Nu32 i= phashmap->Capacity;i!=0;i--,pbuck++)
		{
			if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				pdatadestructor(pbuck->pData);
				pbuck->pData = NULL;
				pbuck->pKey	 = NULL;
				pbuck->Flags = 0;

				phashmap->BucketCount--;
				if(!phashmap->BucketCount)
					break;
			}
		}
		NErrorIf(phashmap->BucketCount,NERROR_HASHMAP_WRONG_BUCKETCOUNT); // it must ZERO !
	}
	else
	{
		memset(phashmap->pBuckets,0,sizeof(NOAHASHENTRY)*phashmap->Capacity);
		phashmap->BucketCount = 0;
	}
}

Nu32 NClearAllOAHashMapEntriesXtd(NOAHASHMAP *phashmap, const NHASHENTRYDATA_CONDITIONAL_DESTRUCTOR_CALLBACK pconditionaldatadestructor )
{
	NErrorIf(!pconditionaldatadestructor, NERROR_NULL_POINTER);
	if(!phashmap->BucketCount)
		return 0;

	Nu32 cleared = 0;
	NOAHASHENTRY *pbuck = phashmap->pBuckets;
	for(Nu32 i= phashmap->Capacity;i!=0;i--,pbuck++)
	{
		if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE) && pconditionaldatadestructor(pbuck->pData))
		{
			pbuck->pData = NULL;
			pbuck->pKey	 = NULL;
			pbuck->Flags = 0;
			cleared ++;
			phashmap->BucketCount--;
			if(!phashmap->BucketCount)
				break;
		}
	}
	return cleared;
}

void* NLookupOAHashMapEntry(const NOAHASHMAP *phashmap, const void *pkey)
{
	NErrorIf(!pkey,NERROR_NULL_POINTER);
	Nu32	i = phashmap->Hash_FCT(pkey)&(phashmap->Capacity-1);
	// Targeted Bucket is empty, so, pKey is not in the table !
	if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
	{
		return NULL;
	}
	else
	{
		for(Nu32 b=phashmap->Capacity;b!=0;b--,i=(i+1)&(phashmap->Capacity-1))
		{
			if(phashmap->KeyCompare_FCT(phashmap->pBuckets[i].pKey,pkey)==0)
			{
				return phashmap->pBuckets[i].pData;
			}
			else if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				return NULL;
			}
		}
	}

	return NULL;
}
// ------------------------------------------------------------------------------------------
// NGetOAHashMapIndexEntry
// ------------------------------------------------------------------------------------------
// Description :
// It's not the purpose of "Hashmap" to be parsed as an array, from the first element to the last.
// But, in some advanced programming it would be necessary to do this kind of thing. 
// So, 'NGetOAHashMapIndexEntry' return the pData pointer of the bucket N° index. Be aware that this entry may
// be NULL ptr.
// ------------------------------------------------------------------------------------------
// In	:
//	
//	
// Out :
//	
//	
// ------------------------------------------------------------------------------------------
void* NGetOAHashMapIndexEntry(const NOAHASHMAP *phashmap, const Nu32 entry_index)
{
	NErrorIf( entry_index>=phashmap->Capacity,NERROR_HASHMAP_INDEX_BEYOND_LIMITS );
	if(ISFLAG_ON(phashmap->pBuckets[entry_index].Flags,FLAG_NOAHASHENTRY_ACTIVE))
		return phashmap->pBuckets[entry_index].pData;
	else
		return NULL;
}


// ------------------------------------------------------------------------------------------
// NGetValidOAHashMapEntry
// ------------------------------------------------------------------------------------------
// Description :
// Same purpose than NGetOAHashMapEntry. But this function only look into VALID buckets !
// So 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void* NLookupValidOAHashMapIndexEntry(const NOAHASHMAP *phashmap, const Nu32 validentry_index)
{
	NErrorIf(validentry_index>=phashmap->BucketCount,NERROR_HASHMAP_INDEX_BEYOND_LIMITS);

	Nu32			validbucket = 0;
	NOAHASHENTRY *pbuck = phashmap->pBuckets;
	for(Nu32 i= phashmap->Capacity;i!=0;i--,pbuck++)
	{
		if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
		{
			if(validentry_index == validbucket)
				return pbuck->pData;
			else
				validbucket++;
		}
	}
	NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // Should never happens
	return NULL;
}

/*
Nu32 NLookupOAHashMapEntryIndex(NOAHASHMAP *phashmap, const void *pkey)
{
	NErrorIf(!pkey,NERROR_NULL_POINTER);
	Nu32	i = phashmap->Hash_FCT(pkey)&(phashmap->Size-1);
	// Targeted Bucket is empty, so, pKey is not in the table !
	if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
	{
		return NVOID;
	}
	else
	{
		for(Nu32 b=phashmap->Size;b!=0;b--,i=(i+1)&(phashmap->Size-1))
		{
			if(phashmap->KeyCompare_FCT(phashmap->pBuckets[i].pKey,pkey)==0)
			{
				return i;
			}
			else if(ISFLAG_OFF(phashmap->pBuckets[i].Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				return NVOID;
			}
		}
	}
	return NVOID;
}
*/

Nu32 NLookupValidOAHashMapEntryIndex(const NOAHASHMAP *phashmap, const void *pkey)
{
	NErrorIf(!pkey,NERROR_NULL_POINTER);

	Nu32			validbucket = 0;
	NOAHASHENTRY	*pbuck = phashmap->pBuckets;
	for(Nu32 i= phashmap->Capacity;i!=0;i--,pbuck++)
	{
		if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
		{
			if(phashmap->KeyCompare_FCT(pbuck->pKey,pkey)==0)
				return validbucket;
			else
				validbucket++;

			if(validbucket>=phashmap->BucketCount)
				break;
		}
	}
	NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // Should never happens
	return NVOID;
}


/*

.... Some potential functions to think about ...


NOAHASHENTRY *NGetFirstValidOAHashMapEntry(const NOAHASHMAP *phashmap)
{
	NErrorIf(!phashmap, NERROR_NULL_POINTER);

	if(!phashmap->BucketCount)
	{
		return NULL;
	}
	else
	{
		NOAHASHENTRY *pbuck = phashmap->pBuckets;
		while(pbuck)
		{
			if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
			{
				return pbuck;
			}
			else
			{
				pbuck++;
				NErrorIf(pbuck >= phashmap->pBuckets + phashmap->Size,NERROR_SYSTEM_GURU_MEDITATION); // Should never happens, because "phashmap->BucketCount != 0" so there is one VALID BUCKET AT LEAST !!!!!
			}
		}
	}
}

NOAHASHENTRY *NGetNextValidOAHashMapEntry(const NOAHASHMAP *phashmap,NOAHASHENTRY *pentry)
{
	NErrorIf(!phashmap || !pentry, NERROR_NULL_POINTER);

	NOAHASHENTRY *pbuck		= pentry + 1;
	NOAHASHENTRY *poverlast = phashmap->pBuckets + phashmap->Size; // Invalid Address, just after the last valid entry address
	while(pbuck < poverlast)
	{
		if(ISFLAG_ON(pbuck->Flags,FLAG_NOAHASHENTRY_ACTIVE))
		{
			return pbuck;
		}
		else
		{
			pbuck++;
		}
	}

}

NOAHASHENTRY *NGetFirstOAHashMapEntry(const NOAHASHMAP *phashmap)
{
	return phashmap->pBuckets;
}

NOAHASHENTRY *NGetLastOAHashMapEntry(const NOAHASHMAP *phashmap)
{
	return phashmap->pBuckets + phashmap->Size;
}

#define NISVALIDOAHASHMAPENTRY(pentry)	ISFLAG_ON((pentry)->Flags,FLAG_NOAHASHENTRY_ACTIVE)
#define NGETOAHASHMAPENTRYDATA(pentry)	((pentry)->pData)
*/