#ifndef __NFONT_H
#define __NFONT_H

// ***************************************************************************************
// **																					**
// **								   NFont.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Containers/NNode.h"
#include "NTexture.h"
#include "NCoordinateSystemConversions.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ***************************************************************************************

// FONT Infos
//
//	line0 ---------------------------------------------------
//										-		-			-
//										|		|			|
//										|		| YOffset	|
//										|		|			|
//		 0 ------- 1 -----	Top			|		-			|
//		 |         |					| BASE				|
//		 |         |					|					| LineHeight	
//		 |  CHAR   |					|					|
//		 |         |					|					|
//		 |         |					|					|
//		 3 ------- 2 -----	Bottom		-					|
//															|
//															-	
//	line1 ---------------------------------------------------
//
#define CONSTANT_NFONT_TAB_SPACE_COUNT					8	// Number of space between two TabStops

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
#define EXTENSION_FNT				".fnt"				// Extension file for the text file which describe the font 
#define NFONT_CHAR_TABLE_SIZE		256

// a Character description
typedef struct NCHAR NCHAR;
struct NCHAR
{
 //	Ns32		X; // not necessary to keep it 
 //	Ns32		Y; // not necessary to keep it
	Ns32		Width;
	Ns32		Height;
	Ns32		XOffset;
	Ns32		YOffset;
	Ns32		XAdvance;

	Ns32		Page;
	
	// PreCalculated UVs
	Nf32		U0;
	Nf32		V0;
	Nf32		USize;
	Nf32		VSize;

	// PreCalculated ViewPort Height relative data
	// !! These data are ViewPort dependent So they have to be updated when ViewPort Change !
	Nf32		HRelWidth;
	Nf32		HRelHeight;
	Nf32		HRelXOffset;
	Nf32		HRelYOffset;
	Nf32		HRelXAdvance;
};

// a Font (with its textures and characters descriptions)
#define FLAG_NFONT_ADAPTATIVE_SIZE		BIT_0
typedef struct NFONT NFONT;
struct NFONT
{
	NNODEDEF
	
	Ns32		CommonLineHeight;	//
	Ns32		Base;				//
	Ns32		PageWidth;			//
	Ns32		PageHeight;			//
	Ns32		PageNb;				// Number of associated Textures to Store all the font characters in memory 
	NTEXTURE	**pNTextures;		//
	
	Ns32		NCharNb;			//
	NCHAR		*pNCharTable;		//

	// Precalculated Data (ViewPort Dependent) !
	Nf32		HRelCommonLineHeight;
	Nf32		HRelBase;
	Nf32		HRelTab;

	Nu32		Flags;
};

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
void				NInitializeFontsList();
void				NDeleteFontsList();

NFONT*				NCreateFontFromFile(const char *filename, const Nbool _adaptative_size);
void				NDeleteFont(NFONT *pfont);
void				NUpdateFontConstants(NFONT *pfont);
NFONT*				NGetDefaultFont();
Nu32				NGetAlphaEffectiveFontPageNumber(const NFONT *pfont);
inline Nu32			NGetFontPageNumber(const NFONT *pfont){return pfont->PageNb;}	

inline Nf32			NGetFontLineHeight(const NFONT *pfont){return pfont->HRelCommonLineHeight;}
inline void			NScaleFontLineHeight(NFONT *pfont, const Nf32 lh_scale){pfont->HRelCommonLineHeight *= lh_scale;NErrorIf(!pfont->HRelCommonLineHeight, NERROR_NULL_VALUE);}
inline void			NSetFontLineHeight(NFONT *pfont, const Nf32 lh){pfont->HRelCommonLineHeight = lh;NErrorIf(!pfont->HRelCommonLineHeight, NERROR_NULL_VALUE);}
inline void			NSetFontDefaultLineHeight(NFONT *pfont){pfont->HRelCommonLineHeight = NSize_PixToHRel_H(pfont->CommonLineHeight);NErrorIf(!pfont->HRelCommonLineHeight, NERROR_NULL_VALUE);}

inline Nf32			NGetFontLineBase(const NFONT *pfont){return pfont->HRelBase;}
inline void			NScaleFontLineBase(NFONT *pfont, const Nf32 lb_scale){pfont->HRelBase *= lb_scale;NErrorIf(!pfont->HRelBase, NERROR_NULL_VALUE);}
inline void			NSetFontLineBase(NFONT *pfont, const Nf32 lb){pfont->HRelBase = lb;NErrorIf(!pfont->HRelBase, NERROR_NULL_VALUE);}
inline void			NSetFontDefaultLineBase(NFONT *pfont){pfont->HRelBase = NCoord_PixToHRel_Y(pfont->Base);NErrorIf(!pfont->HRelBase, NERROR_NULL_VALUE);}

inline Nf32			NGetFontTabSize(const NFONT *pfont){return pfont->HRelTab;}
inline void			NScaleFontTabSize(NFONT *pfont, const Nf32 tab_size_scale){pfont->HRelTab *= tab_size_scale;NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);}
inline void			NSetFontTabSize(NFONT *pfont, const Nu32 space_count){pfont->HRelTab = (Nf32)space_count*pfont->pNCharTable[' '].HRelXAdvance;NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);}
inline void			NSetFontTabSizef(NFONT *pfont, const Nf32 tab_size){pfont->HRelTab = tab_size;NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);}
inline void			NSetFontDefaultTab(NFONT *pfont){pfont->HRelTab = (Nf32)CONSTANT_NFONT_TAB_SPACE_COUNT*pfont->pNCharTable[' '].HRelXAdvance;NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);}


#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NFONT_H 

