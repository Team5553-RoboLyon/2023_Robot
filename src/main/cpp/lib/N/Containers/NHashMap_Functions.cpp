// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								 NHashMap_Functions.cpp							**
// **																					**
// **							  All the Hastable embed functions						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NHashMap.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + 4 Different functions groups
//
//	1/typedef Nu32 (*NHASHTABLE_HASH_FCT)(const void *pkey);
//	2/typedef Nu32 (*NHASHTABLE_KEYCOMPARE_FCT)(const void*pkey1, const void*pkey2);
//	3/typedef void (*NHASHTABLE_KEYCOPY_FCT)(void*pkeydst, const void*pkeysrc);
//	4/typedef void (*NHASHTABLE_KEYCLEAR_FCT)(void*pkey);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + typedef Nu32 (*NHASHTABLE_HASH_FCT)(void *pkey)

// ------------------------------------------------------------------------------------------
// NHashMap_Fletcher32_String_Hash_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		Hash a key.
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :	As all the NHash functions it returns a Nu32
// ------------------------------------------------------------------------------------------
Nu32 NHashMap_optimizedFletcher32_SmallString_Hash_FCT(const void *pkey)
{

	// This function assume pkey is a pointer on a Nchar chain terminated by a Null character.
	// Works fine for "small strings", i.e length < 718 characters ( 359 * 2 )
	NErrorIf(strlen((Nchar *)pkey) > 718, NERROR_STRING_TOO_LONG);

	Nu16 *pdata = (Nu16 *)pkey;					 // Pointer to the data to be summed
	Nu32 len = (strlen((Nchar *)pkey) + 1) >> 1; // Length in 16-bit words
	Nu32 sum1 = 0xffff;
	Nu32 sum2 = 0xffff;

	do
	{
		sum1 += *pdata++;
		sum2 += sum1;

	} while (--len);

	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	// Second reduction step to reduce sums to 16 bits
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// return identifier
	return sum2 << 16 | sum1;
}
// ------------------------------------------------------------------------------------------
// NHashMap_Fletcher32_String_Hash_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		Hash a key.
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :	As all the NHash functions it returns a Nu32
// ------------------------------------------------------------------------------------------
Nu32 NHashMap_Fletcher32_String_Hash_FCT(const void *pkey)
{
	// This function assume pkey is a pointer on a Nchar chain terminated by a Null character.

	Nu16 *pdata = (Nu16 *)pkey;					 // Pointer to the data to be summed
	Nu32 len = (strlen((Nchar *)pkey) + 1) >> 1; // Length in 16-bit words
	Nu32 sum1 = 0xffff;
	Nu32 sum2 = 0xffff;
	Nu32 tlen;

	while (len)
	{
		tlen = len > 359 ? 359 : len;
		len -= tlen;
		do
		{
			sum1 += *pdata++;
			sum2 += sum1;

		} while (--tlen);

		sum1 = (sum1 & 0xffff) + (sum1 >> 16);
		sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	}

	// Second reduction step to reduce sums to 16 bits
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// return identifier
	return sum2 << 16 | sum1;
}
// ------------------------------------------------------------------------------------------
// NHashMap_OAAT_String_Hash_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		One At A Time algorythm from Bob Jenkins
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :	As all the NHash functions it returns a Nu32
// ------------------------------------------------------------------------------------------
Nu32 NHashMap_OAAT_String_Hash_FCT(const void *pkey)
{
	Nu32 hash = 0;
	Nu32 len = strlen((Nchar *)pkey);
	for (Nu32 i = 0; i < len; i++)
	{
		hash += ((Nchar *)pkey)[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Nu32 (*NHASHTABLE_KEYCOMPARE_FCT)(const void*pkey1, const void*pkey2)
// +

// ------------------------------------------------------------------------------------------
// NHashMap_String_Compare_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		Compare 2 keys.
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :	As all the compare HashTable key compare function this function returns ...
//				0	if both keys are equal
//				!=0  if pkey1 != pkey2
// ------------------------------------------------------------------------------------------
Ns32 NHashMap_String_Compare_FCT(const void *pkey1, const void *pkey2)
{
	// This function assume both pkey1 and pkey are strings terminated by a Null character.

	if (!pkey1)
	{
		if (pkey2)
			return -1; // here we assume pkey1 < pkey2
		else
			return 0; // here we assume pkey1 == pkey2;
	}
	else if (!pkey2)
	{
		if (pkey1)
			return 1; // here we assume pkey2 < pkey1
		else
			return 0; // here we assume pkey1 == pkey2;
	}
	else // pkey1 AND pkey2 are both != 0
	{
		return (Ns32)strcmp((Nchar *)pkey1, (Nchar *)pkey2);
	}
}

// ------------------------------------------------------------------------------------------
// NHashMap_SimpleNu32ID_Hash_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		Doesn't Hash anything, just return the Nu32 KeyID value !!!
//
// ------------------------------------------------------------------------------------------
// In  :	A pointer to the Nu32 ID
//
// Out :	As all the NHash functions it returns a Nu32
// ------------------------------------------------------------------------------------------
Nu32 NHashMap_SimpleNu32ID_Hash_FCT(const void *pkey)
{
	return *(Nu32 *)pkey;
}

// ------------------------------------------------------------------------------------------
// NHashMap_SimpleNu32ID_Compare_FCT
// ------------------------------------------------------------------------------------------
// Description :
//		Compare 2 keys.
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :	As all the compare HashTable key compare function this function returns ...
//				0	if both keys are equal
//				!=0  if pkey1 != pkey2
// ------------------------------------------------------------------------------------------
Ns32 NHashMap_SimpleNu32ID_Compare_FCT(const void *pkey1, const void *pkey2)
{
	// This function assume both pkey1 and pkey are strings terminated by a Null character.

	if (!pkey1)
	{
		if (pkey2)
			return -1; // here we assume pkey1 < pkey2
		else
			return 0; // here we assume pkey1 == pkey2;
	}
	else if (!pkey2)
	{
		if (pkey1)
			return 1; // here we assume pkey2 < pkey1
		else
			return 0; // here we assume pkey1 == pkey2;
	}
	else // pkey1 AND pkey2 are both != 0
	{
		return (Ns32)(!(*(Nu32 *)pkey1 == *(Nu32 *)pkey2));
	}
}