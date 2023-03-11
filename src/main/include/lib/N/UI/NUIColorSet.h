#ifndef __NUI_COLORSET_H
#define __NUI_COLORSET_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUIColorSet.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/Miscellaneous/NColor.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************
// UI ColorSet Constant
#define CONSTANT_NUI_COLORSET_COLORRANGE_SIZE 4 // Each color range contains 4 colors.

// UI ColorSet Default
#define DEFAULT_NUI_COLORSET_BKG_ENABLE_COLOR_4F NCOLOR_PRESET3F_LIGHTGRAY, 1
#define DEFAULT_NUI_COLORSET_BKG_DISABLE_COLOR_4F NCOLOR_PRESET3F_DARKGRAY, 1
#define DEFAULT_NUI_COLORSET_BKG_FOCUS_COLOR_4F NCOLOR_PRESET3F_WHITE, 1

#define DEFAULT_NUI_COLORSET_ICON_ENABLE_COLOR_4F NCOLOR_PRESET3F_LIGHTGRAY, 1
#define DEFAULT_NUI_COLORSET_ICON_DISABLE_COLOR_4F NCOLOR_PRESET3F_DARKGRAY, 1
#define DEFAULT_NUI_COLORSET_ICON_FOCUS_COLOR_4F NCOLOR_PRESET3F_WHITE, 1

#define DEFAULT_NUI_COLORSET_TEXT_ENABLE_COLOR_4F NCOLOR_PRESET3F_GRAY, 1
#define DEFAULT_NUI_COLORSET_TEXT_DISABLE_COLOR_4F NCOLOR_PRESET3F_BLACK, 1
#define DEFAULT_NUI_COLORSET_TEXT_FOCUS_COLOR_4F NCOLOR_PRESET3F_LIGHTGRAY, 1

#define DEFAULT_NUI_COLORSET_CHECK_ENABLE_COLOR_4F NCOLOR_PRESET3F_GREEN, 1
#define DEFAULT_NUI_COLORSET_CHECK_DISABLE_COLOR_4F NCOLOR_PRESET3F_GREEN_EMERALD, 1
#define DEFAULT_NUI_COLORSET_CHECK_FOCUS_COLOR_4F NCOLOR_PRESET3F_GREEN_SPRING, 1

#define DEFAULT_NUI_COLORSET_UNCHECK_ENABLE_COLOR_4F NCOLOR_PRESET3F_RED, 1
#define DEFAULT_NUI_COLORSET_UNCHECK_DISABLE_COLOR_4F NCOLOR_PRESET3F_RED_CRIMSON, 1
#define DEFAULT_NUI_COLORSET_UNCHECK_FOCUS_COLOR_4F NCOLOR_PRESET3F_ORANGE_RED, 1

	// UI ColorSet Struct and Enum

	typedef enum
	{
		NUI_COLORSET_ENABLE = 0, // Is also the default One
		NUI_COLORSET_DISABLE = 1,
		NUI_COLORSET_FOCUS = 2,
		// ----------------------------
		NUI_COLORSET_STATE_ENUM_SIZE = 3
	} NUI_COLORSET_STATE_ENUM;

	typedef enum
	{
		NUI_COLORSET_BKG = 0,
		NUI_COLORSET_ICON = 1,
		NUI_COLORSET_TEXT = 2,
		NUI_COLORSET_CHECK = 3,
		NUI_COLORSET_UNCHECK = 4,
		// --------------------------------
		NUI_COLORSET_ELEMENT_ENUM_SIZE = 5
	} NUI_COLORSET_ELEMENT_ENUM;

// COLORSET BIT FIELD ...
#define NUI_IS_COLORSET_DESC_RANGE_DEFINED(field, el, st) BYTEFIELD_BITGET(field, el *NUI_COLORSET_STATE_ENUM_SIZE + st)

	typedef struct
	{
		NCOLOR Colors[NUI_COLORSET_ELEMENT_ENUM_SIZE][NUI_COLORSET_STATE_ENUM_SIZE][CONSTANT_NUI_COLORSET_COLORRANGE_SIZE]; // Used for Quad building (UnCheckMesh).
	} NUI_COLORSET;

	typedef struct
	{
		Nu8 BitField[BYTEFIELD_SIZE(NUI_COLORSET_ELEMENT_ENUM_SIZE * NUI_COLORSET_STATE_ENUM_SIZE)];
		NCOLOR Colors[NUI_COLORSET_ELEMENT_ENUM_SIZE][NUI_COLORSET_STATE_ENUM_SIZE][CONSTANT_NUI_COLORSET_COLORRANGE_SIZE]; // Used for Quad building (UnCheckMesh).
	} NUI_COLORSET_DESC;

	NUI_COLORSET *NSetupUIColorSet(NUI_COLORSET *puic, const NUI_COLORSET_DESC *pdesc);
	NUI_COLORSET *NCreateUIColorSet(const NUI_COLORSET_DESC *pdesc);
	void NClearUIColorSet(NUI_COLORSET *puic);
	void NDeleteUIColorSet(NUI_COLORSET *puic);

	const NCOLOR *NGetUIColorSetPtr(const NUI_COLORSET *pcolorset, const NUI_COLORSET_ELEMENT_ENUM el, const NUI_COLORSET_STATE_ENUM state);

	inline void NFillupUIColorSetDesc(NUI_COLORSET_DESC *pdesc, const NUI_COLORSET_ELEMENT_ENUM el, const NUI_COLORSET_STATE_ENUM state, const NCOLOR *pcolor_range)
	{
		memcpy(&pdesc->Colors[el][state][0], pcolor_range, CONSTANT_NUI_COLORSET_COLORRANGE_SIZE * sizeof(NCOLOR));
		BYTEFIELD_BITSET(pdesc->BitField, el * NUI_COLORSET_STATE_ENUM_SIZE + state);
	}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUI_H
