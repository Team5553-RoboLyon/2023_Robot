#ifndef __NUI_CUSTOM_H
#define __NUI_CUSTOM_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUICustom.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Define
#define DEFAULT_NUI_CUSTOM_SIZE_X					DEFAULT_NUI_SIZE_X
#define DEFAULT_NUI_CUSTOM_SIZE_Y					DEFAULT_NUI_SIZE_Y
#define DEFAULT_NUI_CUSTOM_FLASH_TICK_DURATION		8
#define DEFAULT_NUI_CUSTOM_FLASH_TICK_PATTERN		0x55 // 0101 0101
#define DEFAULT_NUI_CUSTOM_DISPLAY_ARRAY_CAPACITY	1
// ***************************************************************************************
// *								FLAG_NUI_GEOM_COLLECTION								 *
// ***************************************************************************************

//	NUI_CUSTOM_DESC.UI.Flags_Style (starts at BIT_24)
#define	FLAG_NUIDS_USEPARAM_CUSTOM_FLASH_TICK_DURATION			BIT_24
#define	FLAG_NUIDS_USEPARAM_CUSTOM_FLASH_TICK_PATTERN			BIT_25
#define	FLAG_NUIDS_CUSTOM_DISPLAY_BACKGROUND					BIT_26
#define	FLAG_NUIDS_CUSTOM_DISPLAY_ICON							BIT_27
#define	FLAG_NUIDS_CUSTOM_DISPLAY_FIRST_EXTRA_GEOMETRY			BIT_28

//	NUI_CUSTOM_DESC.UI.Flags_Core (starts at BIT_24)
#define	FLAG_NUIDC_CUSTOM_COMMAND_DOWN							BIT_24
#define	FLAG_NUIDC_CUSTOM_COMMAND_UP							BIT_25
#define	FLAG_NUIDC_USEPARAM_CUSTOM_DISPLAY_ARRAY_CAPACITY		BIT_26 // Display array capacity set by user 

//	NUI_CUSTOM.Flags (starts at BIT_24)
#define	FLAG_NUI_CUSTOM_COMMAND_DOWN							BIT_24
#define	FLAG_NUI_CUSTOM_COMMAND_UP								BIT_25

// ***************************************************************************************
// *								Structures											 *
// ***************************************************************************************
// An UI Geometry Viewer
//#define FLAG_NUI_CUSTOM_xx			BIT_0
//#define FLAG_NUI_CUSTOM_xx			BIT_1
//#define FLAG_NUI_CUSTOM_xx			BIT_2
//#define FLAG_NUI_CUSTOM_xx			BIT_3
//#define FLAG_NUI_CUSTOM_xx			BIT_4
//#define FLAG_NUI_CUSTOM_xx			BIT_5
//#define FLAG_NUI_CUSTOM_xx			BIT_6
//#define FLAG_NUI_CUSTOM_xx			BIT_7
typedef struct
{
	NUI			UI;
	
	Nu32		FlashTickPattern				:8;		//|
	Nu32		CurrentFlashTickID				:3;		//| 0 to 7 max ( to store current bit pattern ID between the 8 of them )
	Nu32		FlashTickDuration				:9;		//|	0 to 512 "calls" max. "1 call" means "1 call to the extract function".
	Nu32		CurrentFlashTickDuration		:9;		//| 0 to 512 "calls" max. "1 call" means "1 call to the extract function".
	Nu32		UIBasicGeomNb					:3;		//| 0,1 or 2 (Background and/or Icon or none of them ? Just to have it quickly...)
	
	NEXTRACTSET	HighLevelExtractSet;			// Due to dynamic Extract function change, we need to keep it from "NUI_DESC"
	NARRAY		DisplayArray;					// Array of all the geometry Ptr to extract... 
												// All of them, ... "Background" AND "Geometries" AND "Icon" ... 
}NUI_CUSTOM;

// An UI Custom Description
typedef struct
{
	NUI_DESC			UIDesc;
	NUIGEOMETRY_DESC	*pGeometryDescList;
	Nu32				GeometryDescNumber;
	Nu32				FlashTickPattern		:8;		//|
	Nu32				FlashTickDuration		:9;		//|_32 Bits at all
														//|
	Nu32				Available_15bits		:15;	//|
	Nu32				DisplayArrayCapacity;
}NUI_CUSTOM_DESC;

// ***************************************************************************************
// *								Functions											 *
// ***************************************************************************************
NUI_CUSTOM*		NCreateUICustom(NUI *parent, NUI_CUSTOM_DESC *pgvdesc, const Nu32 user32);
NUI_CUSTOM*		NSetupUICustom(NUI *parent, NUI_CUSTOM *pgv, NUI_CUSTOM_DESC *pgvdesc, const Nu32 user32);
void			NClearUICustom(NUI *pui);

Nu32			NInsertUICustomExtraGeometry(NUI_CUSTOM *pgc, const NUIGEOMETRY_DESC *pgdesc);
void			NEraseUICustomExtraGeometry(NUI_CUSTOM *pgc, const Nu32 index);

inline	Nu32	NGetUICustomBasicGeometriesNumber(NUI_CUSTOM *pgv){return (Nu32)pgv->UIBasicGeomNb;}
inline	Nu32	NGetUICustomExtraGeometriesNumber(NUI_CUSTOM *pgv)
{
	// This function returns the number of Extra geometries, Background and or icon geometries excluded !
	// To retrieve the total number of geometries use 'NGetIncludedGeometriesNumber' with the UI Renderable as parameter.
	NErrorIf(pgv->UIBasicGeomNb > pgv->UI.pRenderable->GeomArray.Size,NERROR_INCONSISTENT_VALUES);
	return ( pgv->UI.pRenderable->GeomArray.Size - pgv->UIBasicGeomNb);
}

#define FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE	BIT_0
#define FLAG_NUI_CUSTOM_EXCLUSIVE_DISPLAY			BIT_1
void			 NDisplayUICustomBatch( NUI_CUSTOM *pgv, const Nu32* pNu32_bit_field_display_mask, const Nu32 flags_nui_custom);
const NGEOMETRY* NDisplayUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom);

inline Nu32		NGetUICustomFirstExtraGeometryID(const NUI_CUSTOM *pgv){return pgv->UIBasicGeomNb;}
inline Nu32		NGetUICustomExtraGeometryIDFromGeometryID(const NUI_CUSTOM *pgv,const Nu32 id){NErrorIf((id<pgv->UIBasicGeomNb)||(id>=pgv->UI.pRenderable->GeomArray.Size),NERROR_INCONSISTENT_PARAMETERS);return (id - pgv->UIBasicGeomNb);}
inline Nu32		NGetUICustomGeometryIDFromExtraGeometryID(const NUI_CUSTOM *pgv,const Nu32 xtra_id){NErrorIf(xtra_id + pgv->UIBasicGeomNb >=pgv->UI.pRenderable->GeomArray.Size,NERROR_INCONSISTENT_PARAMETERS);return (xtra_id + pgv->UIBasicGeomNb);}

const NGEOMETRY* NGetUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom);
const NGEOMETRY* NGetUICustomFirstDisplayedExtraGeometry(NUI_CUSTOM *pgv);

void			NHideUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom);
void			NHideAllUICustomGeometries(NUI_CUSTOM *pgv);


void			NUICustomFlashON(NUI_CUSTOM *pgv);
void			NUICustomFlashOFF(NUI_CUSTOM *pgv);
inline void		NSetUICustomFlashTickDuration(NUI_CUSTOM *pgv, const Nu32 flashtickduration){NErrorIf(flashtickduration>0x1FF,NERROR_VALUE_OUTOFRANGE);pgv->FlashTickDuration=(flashtickduration&0x1FF);}
inline void		NSetUICustomFlashTickPattern(NUI_CUSTOM *pgv, const Nu8 flashtickpattern){pgv->FlashTickPattern = flashtickpattern;}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUI_CUSTOM_H

