// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NSCHashTable.cpp									**
// **																					**
// **							Separate Chaining Hash Table							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NMath.h"
#include "lib/N/Containers/NHashMap.h"

NSCHASHMAP *NSetupSCHashMap(NSCHASHMAP *phashmap, const NHASHMAP_DESC *pdesc)
{
	// Params collect
	if (ISFLAG_ON(pdesc->Flags, FLAG_NHASHMAP_DESC_USE_CAPACITY_POWOFTWO))
	{
		NErrorIf(!pdesc->Capacity_PowOfTwo, NERROR_WRONG_VALUE);				// its a constraint ! Hastmap capacity has to be a power of two !
		NErrorIf(!NIsPowerOfTwo(pdesc->Capacity_PowOfTwo), NERROR_WRONG_VALUE); // its a constraint ! Hastmap capacity has to be a power of two !
		phashmap->Capacity = pdesc->Capacity_PowOfTwo;
	}
	else
	{
		phashmap->Capacity = DEFAULT_NSCHASHMAP_CAPACITYPOWOFTWO;
	}

	if (ISFLAG_ON(pdesc->Flags, FLAG_NHASHMAP_DESC_USE_HASH_FCT))
	{
		phashmap->Hash_FCT = pdesc->Hash_FCT;
	}
	else
	{
		phashmap->Hash_FCT = DEFAULT_NSCHASHMAP_HASH_FCT;
	}

	if (ISFLAG_ON(pdesc->Flags, FLAG_NHASHMAP_DESC_USE_KEYCOMPARE_FCT))
	{
		phashmap->KeyCompare_FCT = pdesc->KeyCompare_FCT;
	}
	else
	{
		phashmap->KeyCompare_FCT = DEFAULT_NSCHASHMAP_KEYCOMPARE_FCT;
	}

	phashmap->ppBuckets = (NSCHASHENTRY **)Nmalloc(sizeof(NSCHASHENTRY *) * phashmap->Capacity);
	memset(phashmap->ppBuckets, 0, sizeof(NSCHASHENTRY *) * phashmap->Capacity);
	phashmap->BucketCount = 0;
	phashmap->BucketCountMax = (Nu32)((Nf32)phashmap->Capacity * CONSTANT_NSCHASHMAP_MAX_DENSITY);
	NErrorIf(!phashmap->BucketCountMax, NERROR_SYSTEM_CHECK); // at minimum 2*CONSTANT_NSCHASHMAP_MAX_DENSITY should be = 1
	return phashmap;
}

void NClearSCHashMap(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor)
{
	NSCHASHENTRY **ppbuck = phashmap->ppBuckets;
	NSCHASHENTRY *pbuck, *pbuckdel;

	for (Nu32 i = phashmap->Capacity; i != 0; i--, ppbuck++)
	{
		if (*ppbuck)
		{
			pbuck = *ppbuck;
			while (pbuck)
			{
				if (pdatadestructor)
					pdatadestructor(pbuck->pData);
				phashmap->BucketCount--;

				pbuckdel = pbuck;
				pbuck = pbuck->pNext;
				Nfree(pbuckdel);
			}
		}
		else if (!phashmap->BucketCount) // To avoid parsing all the Buckets list if its not necessary
		{
			break;
		}
	}

	Nfree(phashmap->ppBuckets);
	Nmem0(phashmap, NSCHASHMAP);
}

NSCHASHMAP *NCreateSCHashMap(const NHASHMAP_DESC *pdesc)
{
	return NSetupSCHashMap(NEW(NSCHASHMAP), pdesc);
}

void NDeleteSCHashMap(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor)
{
	NClearSCHashMap(phashmap, pdatadestructor);
	Nfree(phashmap);
}

void NReHashSCHashMap(NSCHASHMAP *phashmap)
{
	Nu32 oldcapacity = phashmap->Capacity;
	NSCHASHENTRY **ppoldbuckets = phashmap->ppBuckets;
	NSCHASHENTRY **ppoldbuck = ppoldbuckets;
	NSCHASHENTRY *poldbuck, *poldbuck_next;
	Nu32 id;

	NSCHASHENTRY *pnewbuck;

	phashmap->Capacity = oldcapacity << 1;
	phashmap->ppBuckets = (NSCHASHENTRY **)Nmalloc(sizeof(NSCHASHENTRY *) * phashmap->Capacity);
	memset(phashmap->ppBuckets, 0, sizeof(NSCHASHENTRY *) * phashmap->Capacity);

	phashmap->BucketCountMax = (Nu32)((Nf32)phashmap->Capacity * CONSTANT_NSCHASHMAP_MAX_DENSITY);
	NErrorIf(!phashmap->BucketCountMax, NERROR_SYSTEM_CHECK); // at minimum 2*CONSTANT_NSCHASHMAP_MAX_DENSITY should be = 1

	for (Nu32 i = oldcapacity; i != 0; i--, ppoldbuck++)
	{
		poldbuck = *ppoldbuck;
		while (poldbuck)
		{
			poldbuck_next = poldbuck->pNext;

			// Manual Re-inserting to avoid Nfree and "Re"-Nmalloc each entry !
			poldbuck->pNext = NULL; // Re-insertion setup
			id = phashmap->Hash_FCT(poldbuck->pKey) & (phashmap->Capacity - 1);
			pnewbuck = phashmap->ppBuckets[id];
			if (!pnewbuck)
			{
				phashmap->ppBuckets[id] = poldbuck;
			}
			else
			{
				while (pnewbuck->pNext)
				{
					pnewbuck = pnewbuck->pNext;
				}
				pnewbuck->pNext = poldbuck;
			}

			poldbuck = poldbuck_next;
		}
	}
	Nfree(ppoldbuckets);
}

// return incoming "pdata" in case of successful insertion, or "pold data" in case of replacement !
void *NInsertSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey, const void *pdata)
{
	NErrorIf(!pdata || !pkey, NERROR_NULL_POINTER);
	if (phashmap->BucketCount >= phashmap->BucketCountMax)
		NReHashSCHashMap(phashmap);

	// Insert it
	Nu32 id = phashmap->Hash_FCT(pkey) & (phashmap->Capacity - 1);
	NSCHASHENTRY *pbuck = phashmap->ppBuckets[id];

	if (!pbuck) // No Collision
	{
		// Create the Entry
		NSCHASHENTRY *pnew = (NSCHASHENTRY *)Nmalloc(sizeof(NSCHASHENTRY));
		pnew->pData = (void *)pdata;
		pnew->pKey = (void *)pkey;
		pnew->pNext = NULL;

		phashmap->ppBuckets[id] = pnew;
		phashmap->BucketCount++;
		return (void *)pdata;
	}
	else // Collision !
	{
		while (1)
		{
			// Target bucket is occupied by data with the same key, NEW DATA are going to replace older
			if (phashmap->KeyCompare_FCT(pbuck->pKey, pkey) == 0)
			{
				void *pold = pbuck->pData;
				pbuck->pData = (void *)pdata;
				return pold;
			}
			else if (pbuck->pNext) // There is a next one, so take it !
			{
				pbuck = pbuck->pNext;
			}
			else // No next one, so create it and store it !
			{
				NSCHASHENTRY *pnew = (NSCHASHENTRY *)Nmalloc(sizeof(NSCHASHENTRY));
				pnew->pData = (void *)pdata;
				pnew->pKey = (void *)pkey;
				pnew->pNext = NULL;
				pbuck->pNext = pnew;
				phashmap->BucketCount++;
				return (void *)pdata;
			}
		}
	}
}

void *NRemoveSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey)
{
	NSCHASHENTRY **pprevnext = &phashmap->ppBuckets[phashmap->Hash_FCT(pkey) & (phashmap->Capacity - 1)];
	NSCHASHENTRY *pbuck = *pprevnext;

	while (pbuck)
	{
		if (phashmap->KeyCompare_FCT(pbuck->pKey, pkey) == 0)
		{
			// Found ! So break and repair the chain
			// ... And delete the bucket !
			void *pdata = pbuck->pData;
			*pprevnext = pbuck->pNext;
			Nfree(pbuck);
			phashmap->BucketCount--;
			return pdata;
		}

		pprevnext = &pbuck->pNext;
		pbuck = pbuck->pNext;
	}
	// Not found !
	return NULL;
}
void NClearSCHashMapEntry(NSCHASHMAP *phashmap, const void *pkey, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor)
{
	if (pdatadestructor)
		pdatadestructor(NRemoveSCHashMapEntry(phashmap, pkey));
	else
		NRemoveSCHashMapEntry(phashmap, pkey);
}
void NRemoveAllSCHashMapEntries(NSCHASHMAP *phashmap)
{
	NSCHASHENTRY **ppbuck = phashmap->ppBuckets;
	NSCHASHENTRY *pbuck, *pbuckdel;

	for (Nu32 i = phashmap->Capacity; i != 0; i--, ppbuck++)
	{
		if (*ppbuck)
		{
			pbuck = *ppbuck;
			while (pbuck)
			{
				phashmap->BucketCount--;
				pbuckdel = pbuck;
				pbuck = pbuck->pNext;
				Nfree(pbuckdel);
			}
		}
		else if (!phashmap->BucketCount) // To avoid parsing all the Buckets list if its not necessary
		{
			break;
		}
	}
}
void NClearAllSCHashMapEntries(NSCHASHMAP *phashmap, const NHASHENTRYDATA_DESTRUCTOR_CALLBACK pdatadestructor)
{
	NSCHASHENTRY **ppbuck = phashmap->ppBuckets;
	NSCHASHENTRY *pbuck, *pbuckdel;

	for (Nu32 i = phashmap->Capacity; i != 0; i--, ppbuck++)
	{
		if (*ppbuck)
		{
			pbuck = *ppbuck;
			while (pbuck)
			{
				if (pdatadestructor)
					pdatadestructor(pbuck->pData);
				phashmap->BucketCount--;

				pbuckdel = pbuck;
				pbuck = pbuck->pNext;
				Nfree(pbuckdel);
			}
		}
		else if (!phashmap->BucketCount) // To avoid parsing all the Buckets list if its not necessary
		{
			break;
		}
	}
}

void *NLookupSCHashMapEntry(const NSCHASHMAP *phashmap, const void *pkey)
{
	NSCHASHENTRY *pbuck = phashmap->ppBuckets[phashmap->Hash_FCT(pkey) & (phashmap->Capacity - 1)];
	while (pbuck)
	{
		if (phashmap->KeyCompare_FCT(pbuck->pKey, pkey) == 0)
		{
			return pbuck->pData; // Got it !
		}
		pbuck = pbuck->pNext;
	}

	// Not found !
	return NULL;
}