#ifndef __NFASTANIMATEDVALUE_H
#define __NFASTANIMATEDVALUE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NFastAnimatedValue.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// a Value ( Key or Not) in the LUT 
typedef struct
{
	Nu8			IsKey;
	Nu8			Lerp;
	Nf32		fKeyValue;	// The Key Value (to animate)
}NFASTKEY;

// a FAST Animated Value Head structure 
typedef struct
{
	Nu16		Size;
	Nu16		NbKey;
	NFASTKEY	*pKeyList;
}NFASTANIMATEDVALUE;

// All the LERP interpolation Presets
typedef enum
{
	LERP_LEVEL1,
	LERP_LEVEL2S,
	LERP_LEVEL2N,
	LERP_LEVEL3S,
	LERP_LEVEL3N
}LERP_LEVEL;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NFASTANIMATEDVALUE*	NCreateFastAnimatedValueLUT(const Nu16 size);
Nbool				NDeleteFastAnimatedValueLUT(NFASTANIMATEDVALUE* panim);

void				NScaleFastAnimatedValueLUT(NFASTANIMATEDVALUE* panim,const Nf32 scale );

NFASTKEY*			NInsertFastKey(NFASTANIMATEDVALUE *panim, const Nf32 time, const Nf32 fvalue, const LERP_LEVEL lerp);
float				NReadFastAnimatedValue(const NFASTANIMATEDVALUE* panim,const Nf32 time);
inline	Nf32		NReadFastAnimatedValueByIndex(const NFASTANIMATEDVALUE* panim,const Nu16 index){return panim->pKeyList[index].fKeyValue;};

Nbool				NFastAnimatedValueCopy(NFASTANIMATEDVALUE *pdest,const NFASTANIMATEDVALUE *psrc);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NFASTANIMATEDVALUE_H 

