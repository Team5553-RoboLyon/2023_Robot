#include "lib/N/NCStandard.h"

// #include "../NUT.h"

Nu32 NUT_MakeUIDFromString(const char *ename)
{
	// Optimized Method
	const Nu16 *data = (Nu16 *)ename;	 // Pointer to the data to be summed
	Nu32 len = (strlen(ename) + 1) >> 1; // Length in 16-bit words
	Nu32 sum1 = 0xffff;
	Nu32 sum2 = 0xffff;

	while (len--)
	{
		sum1 += *data++;
		sum2 += sum1;
	}

	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// Second reduction step to reduce sums to 16 bits
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// return identifier
	return sum2 << 16 | sum1;
	//---

	//--- Original Method
	// const	u16*	data = (u16*)ename;				// Pointer to the data to be summed
	// u32				len  = (strlen(ename)+1) >> 1;   // Length in 16-bit words
	// u32				sum1 = 0xffff;
	// u32				sum2 = 0xffff;

	// while (len)
	//{
	//	unsigned tlen = len > 360 ? 360 : len;
	//	len -= tlen;
	//	do
	//	{
	//		sum1 += *data++;
	//		sum2 += sum1;

	//	} while (--tlen);

	//	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	//	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	//}

	//// Second reduction step to reduce sums to 16 bits
	// sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	// sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	//// return identifier
	// return sum2 << 16 | sum1;
	//---

	//--- Methode au pif (fonctionne mais peut faire mieux)
	// u32 ecode = 0;
	// const char* buff = ename;
	// s32 i = strlen(ename);

	// while ( i )
	//{
	//	ecode += (i+1)*((*buff)-i)*((*buff)+i);
	//	//ecode += (i+1)*((*buff)-i)*(i+(*buff)*(i+1));

	//	buff++;
	//	i--;
	//}

	// return ecode;
	//---
}

//===============================================================================================================================================================================================
