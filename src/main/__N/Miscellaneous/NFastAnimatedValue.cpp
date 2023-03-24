// ==========================================================================================
// FastAnimatedValue.cpp
// ------------------------------------------------------------------------------------------
// Author  : 
// Created : 14/01/2012
// Updated : 
// ------------------------------------------------------------------------------------------
// To store and restore a value (float) animation as fast as possible by using a LUT
// ==========================================================================================
#include "../NCStandard.h"
#include "../NType.h"

#include "../Maths/NVec2f32.h"
#include "../Core/NSafeConversion.h"
#include "../NFastAnimatedValue.h"
// ------------------------------------------------------------------------------------------
// NCreateFastAnimatedValueLUT
// ------------------------------------------------------------------------------------------
// Description :
// Create a structure NFASTANIMATEDVALUE and the LUT within
// ------------------------------------------------------------------------------------------
// In  :
//		"size" of the LUT (number of element inside. 	
// Out :
//		a pointer on the NFASTANIMATEDVALUE created
// ------------------------------------------------------------------------------------------
NFASTANIMATEDVALUE* NCreateFastAnimatedValueLUT(const Nu16 size)
{
	NFASTANIMATEDVALUE	*panim;
	// =================
	// Memory Allocation
	// =================
	// 'THE' Fast Animated Value:
/*
	panim = (NFASTANIMATEDVALUE*)malloc( sizeof(NFASTANIMATEDVALUE) );
	if( !panim )
		return NULL;
*/
	panim = NEW(NFASTANIMATEDVALUE);
	memset( panim,0,sizeof( NFASTANIMATEDVALUE ) );
	
	// The list of keys
	panim->pKeyList = (NFASTKEY*)Nmalloc( size*sizeof(NFASTKEY) );
	if( !panim->pKeyList )
	{
		Nfree( panim );
		return NULL;
	}
	memset( panim->pKeyList,0,size*sizeof(NFASTKEY) );
	panim->Size = size;
	panim->NbKey = 0;

	return panim;
}

// ------------------------------------------------------------------------------------------
// NDeleteFastAnimatedValueLUT
// ------------------------------------------------------------------------------------------
// Description :
// Delete a structure NFASTANIMATEDVALUE and the LUT within
// ------------------------------------------------------------------------------------------
// In  :
//		* panim:	of the structure to delete. 	
// Out :
//		1
// ------------------------------------------------------------------------------------------
Nbool NDeleteFastAnimatedValueLUT(NFASTANIMATEDVALUE* panim)
{
	Nfree(panim->pKeyList);
	Nfree(panim);
	return 1;
}

// ------------------------------------------------------------------------------------------
// NReadFastAnimatedValue
// ------------------------------------------------------------------------------------------
// Description :
//	return a precomputed animated value stored in the LUT
// ------------------------------------------------------------------------------------------
// In  :
//			*panim	:	a pointer of a NFASTANIMATEDVALUE to read the LUT 
//			time	:	a float value, 0 <= time <= 1, it's a moment in the animation
//						(0 for beginning, 1 for the end )
// Out :
//			pre-calculated animated value (float) stored in the LUT
// ------------------------------------------------------------------------------------------
float NReadFastAnimatedValue(const NFASTANIMATEDVALUE* panim, const Nf32 time )
{
	return panim->pKeyList[(Nu16)(time*(Nf32)(panim->Size-1.0f))].fKeyValue;
}

// ------------------------------------------------------------------------------------------
// NScaleFastAnimatedValueLUT
// ------------------------------------------------------------------------------------------
// Description :
//	Scale all the values by a value
// ------------------------------------------------------------------------------------------
// In  :
//			*panim	:	a pointer of a NFASTANIMATEDVALUE to read the LUT 
//			scale	:	a float value. All the LUT values are going to be scaled by this one
// Out :
//			-
// ------------------------------------------------------------------------------------------
void NScaleFastAnimatedValueLUT(NFASTANIMATEDVALUE* panim,const Nf32 scale )
{
	Nu32		i;
	NFASTKEY	*pk;

	pk = panim->pKeyList;

	for(i=0;i<panim->Size;i++)
	{
		pk->fKeyValue *= scale;
		pk++;
	}
}
// ------------------------------------------------------------------------------------------
// NInsertFastKey
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate and store in the LUT all the intermediate values between all the keys,
//	the new one included.
// ------------------------------------------------------------------------------------------
// In  :
//			*panim	:	a pointer of a NFASTANIMATEDVALUE to read the LUT 
//			time	:	a float value, 0 <= time <= 1, it's a moment in the animation
//						 (0 for beginning, 1 for the end )
//			fvalue	:	key value at this precise time
//			lerp	:	Define the type of LERP used to calculate intermediate values from this key to 
//						the next one.
// Out :
//			a pointer on the KEY struct in the LUT.
// ------------------------------------------------------------------------------------------
NFASTKEY* NInsertFastKey(NFASTANIMATEDVALUE *panim, const Nf32 time, const Nf32 fvalue, const LERP_LEVEL lerp)
{
	Nu32		index,previndex,nextindex;
	Nu32		i;
	NFASTKEY	*pkey;

	Nf32		fval0,fdist,fdate0,fduration;
	
	NVEC2	A,B,C,D;
	NVEC2	L1,L2,L3,L4,L5,R;
	
	LERP_LEVEL	usedlerp;

	// ====================================
	// Some initializations for the new Key
	// ====================================
	index = (Nu32)(time*(Nf32)(panim->Size-1));
	pkey = panim->pKeyList;
	pkey[index].IsKey = 1;
	pkey[index].Lerp = _SafeNintToNu8(lerp);
	pkey[index].fKeyValue = fvalue;

	if(!panim->NbKey)
	{
		previndex = -1;
		nextindex = -1;
	}
	else
	{
		// =======================
		// look for a previous key
		// =======================
		previndex = -1;
		for(i=index-1;i>=0;i--)
		{
			if(pkey[i].IsKey)
			{
				previndex=i;
				break;
			}
		}
		// ===================
		// look for a next key
		// ===================
		nextindex = -1;
		for(i=index+1;i<panim->Size;i++)
		{
			if(pkey[i].IsKey)
			{
				nextindex=i;
				break;
			}
		}
	}

	// ===============================================================
	// Calculate and Store all the values between -PrevKey-Key-NextKey- 
	// ===============================================================
	// ===================================
	// From Key to PreviousKey Calculation
	// ===================================
	if(previndex!=-1)
	{
		fval0		= pkey[previndex].fKeyValue;
		fdate0		= (float)previndex;
		fdist		= pkey[index].fKeyValue - pkey[previndex].fKeyValue;
		fduration	= (float)(index-previndex);

		usedlerp = (LERP_LEVEL)pkey[previndex].Lerp;

		A.x = (Nf32)previndex;
		A.y = pkey[previndex].fKeyValue;
		
		B.x = (Nf32)index;
		B.y = pkey[previndex].fKeyValue;
		
		C.x	= (Nf32)previndex;
		C.y =  pkey[index].fKeyValue;

		D.x	= (Nf32)index;
		D.y =  pkey[index].fKeyValue;

	}
	else
	{
		fval0		= pkey[index].fKeyValue;
		fdate0		= 0.0f;
		fdist		= 0.0f;
		fduration	= 1.0f;// just to avoid an error message;

		usedlerp = LERP_LEVEL1;
		
		A.x = (Nf32)index;
		A.y = pkey[index].fKeyValue;

		B.x = (Nf32)index;
		B.y = pkey[index].fKeyValue;

		C.x	= (Nf32)index;
		C.y =  pkey[index].fKeyValue;

		D.x	= (Nf32)index;
		D.y =  pkey[index].fKeyValue;
	}

	for(i=index-1;i>=0;i--)
	{
		if(pkey[i].IsKey)
			break;
		else
		{
			//pkey[i].fKeyValue = fval0 + fdist*((float)i-fdate0)/fduration;
 			fdist = ((Nf32)i-fdate0)/fduration;
			
			switch(usedlerp)
			{
				case LERP_LEVEL1:
					NVec2Lerp(&R,&A,&D,fdist);
					break;

				case LERP_LEVEL2S:
					NVec2Lerp(&L1,&A,&B,fdist);
					NVec2Lerp(&L2,&B,&D,fdist);

					NVec2Lerp(&R,&L1,&L2,fdist);
					break;

				case LERP_LEVEL2N:
					NVec2Lerp(&L1,&A,&C,fdist);
					NVec2Lerp(&L2,&C,&D,fdist);

					NVec2Lerp(&R,&L1,&L2,fdist);
					break;
				
				case LERP_LEVEL3S:
					NVec2Lerp(&L1,&A,&B,fdist);
					NVec2Lerp(&L2,&B,&C,fdist);
					NVec2Lerp(&L3,&C,&D,fdist);

					NVec2Lerp(&L4,&L1,&L2,fdist);
					NVec2Lerp(&L5,&L2,&L3,fdist);

					NVec2Lerp(&R,&L4,&L5,fdist);
					break;
				
				case LERP_LEVEL3N:
					NVec2Lerp(&L1,&A,&C,fdist);
					NVec2Lerp(&L2,&C,&B,fdist);
					NVec2Lerp(&L3,&B,&D,fdist);

					NVec2Lerp(&L4,&L1,&L2,fdist);
					NVec2Lerp(&L5,&L2,&L3,fdist);

					NVec2Lerp(&R,&L4,&L5,fdist);
			}
			pkey[i].fKeyValue = R.y;
		}
	}

	// ================================
	// From Key to NextsKey Calculation
	// ================================
	if(nextindex!=-1)
	{
		fval0		= pkey[index].fKeyValue;
		fdate0		= (Nf32)index;
		fdist		= pkey[nextindex].fKeyValue-pkey[index].fKeyValue;
		fduration	= (Nf32)(nextindex-index);

		usedlerp = (LERP_LEVEL)pkey[index].Lerp;

		A.x = (Nf32)index;
		A.y = pkey[index].fKeyValue;

		B.x = (Nf32)nextindex;
		B.y = pkey[index].fKeyValue;

		C.x	= (Nf32)index;
		C.y =  pkey[nextindex].fKeyValue;

		D.x	= (Nf32)nextindex;
		D.y =  pkey[nextindex].fKeyValue;

	}
	else
	{
		fval0		= pkey[index].fKeyValue;
		fdate0		= 0.0f;
		fdist		= 0.0f;
		fduration	= 1.0f;// just to avoid an error message;

		usedlerp = LERP_LEVEL1;

		A.x = (Nf32)index;
		A.y = pkey[index].fKeyValue;

		B.x = (Nf32)index;
		B.y = pkey[index].fKeyValue;

		C.x	= (Nf32)index;
		C.y =  pkey[index].fKeyValue;

		D.x	= (Nf32)index;
		D.y =  pkey[index].fKeyValue;
	}

	for(i=index+1;i<panim->Size;i++)
	{
		if(pkey[i].IsKey)
			break;
		else
		{
			//pkey[i].fKeyValue = fval0 + fdist*((float)i-fdate0)/fduration;
			fdist = ((Nf32)i-fdate0)/fduration;

			switch(usedlerp)
			{
			case LERP_LEVEL1:
				NVec2Lerp(&R,&A,&D,fdist);
				break;

			case LERP_LEVEL2S:
				NVec2Lerp(&L1,&A,&B,fdist);
				NVec2Lerp(&L2,&B,&D,fdist);

				NVec2Lerp(&R,&L1,&L2,fdist);
				break;

			case LERP_LEVEL2N:
				NVec2Lerp(&L1,&A,&C,fdist);
				NVec2Lerp(&L2,&C,&D,fdist);

				NVec2Lerp(&R,&L1,&L2,fdist);
				break;

			case LERP_LEVEL3S:
				NVec2Lerp(&L1,&A,&B,fdist);
				NVec2Lerp(&L2,&B,&C,fdist);
				NVec2Lerp(&L3,&C,&D,fdist);

				NVec2Lerp(&L4,&L1,&L2,fdist);
				NVec2Lerp(&L5,&L2,&L3,fdist);

				NVec2Lerp(&R,&L4,&L5,fdist);
				break;

			case LERP_LEVEL3N:
				NVec2Lerp(&L1,&A,&C,fdist);
				NVec2Lerp(&L2,&C,&B,fdist);
				NVec2Lerp(&L3,&B,&D,fdist);

				NVec2Lerp(&L4,&L1,&L2,fdist);
				NVec2Lerp(&L5,&L2,&L3,fdist);

				NVec2Lerp(&R,&L4,&L5,fdist);
			}

			pkey[i].fKeyValue = R.y;
		}
	}

	panim->NbKey ++;


	// DEBUG CONSOLE OUTPUT
/*
	for(i=0;i<panim->Size;i++)
	{
		printf("Index:%d IsKey:%d Value:%f\n",i,pkey[i].IsKey,pkey[i].fKeyValue);
	}
	printf("-------------------------------------------\n\n");
*/

	return &pkey[index];
}

Nbool NFastAnimatedValueCopy(NFASTANIMATEDVALUE *pdest,const NFASTANIMATEDVALUE *psrc)
{
	if(pdest->Size != psrc->Size)
		return false;
	else
	{
		memcpy(pdest->pKeyList,psrc->pKeyList,psrc->Size*sizeof(NFASTKEY));
		pdest->NbKey = psrc->NbKey;
		return true;
	}
}

/*
Nbool NFastAnimatedValueTranspose(NFASTANIMATEDVALUE *pdest,const NFASTANIMATEDVALUE *psrc)
{
	// Destination LUT need to have enough space to 
	if(pdest->Size < psrc->NbKey)
		return false;
	else
	{
		memcpy(pdest->pKeyList,psrc->pKeyList,psrc->Size*sizeof(NFASTKEY));
		pdest->NbKey = psrc->NbKey;
		return true;
	}
}
*/
