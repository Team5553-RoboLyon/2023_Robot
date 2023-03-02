#include "lib/N/NCStandard.h"
#include "NUI.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "NUIPush.h"
#include "NUICustom.h"

inline void _update_uicustom_extractFCT(NUI_CUSTOM *pgv)
{
	// !!! pgc->HighLevelExtractSet has to be setup in the right way because it WILL BE USED !
	NEXTRACTSET lowlevelextractset = 0;
	if (pgv->DisplayArray.Size == 1)
	{
		lowlevelextractset = NEXTRACTSET_SECTION_UICUSTOM;
	}
	else // !=1 (it should always be >1 but multigeometries extract function will work for NO GEOMETRY too, not into the fastest way but ... )
	{
		lowlevelextractset = NEXTRACTSET_SECTION_UICUSTOM | FLAG_NEXTRACTSET_MULTIGEOMETRIES;
	}
	NSetRenderableExtractFCT(pgv->UI.pRenderable, NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(lowlevelextractset, pgv->HighLevelExtractSet)));
}

void NUICustom_EventToState_Handle(NUI *pui, const NEVENT *pevent)
{
	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event (which is weird for a UICustom but anyway ...)
	//  in that case function returns immediately ...
	if ((NUI *)pevent->Head.pRecipient != pui)
		return;

	// First, call the EventToState 'Push' function.
	// It will manage 'ALONE' the UI "State".
	// Actually, purpose of 'NUICustom_EventToState_Handle' is just sending appropriate Direct EVENT according
	// with push KEY_INSTANTs.
	NEVENT nevent;

	switch (NGET_UIPUSH_PREHANDLE_KEY_INSTANT(NUIPush_EventToState_PreHandle(pui, pevent)))
	{
	case NUI_KEY_INSTANT_PUSH_DOWN_CANCEL:
		// Send CANCEL Command Event ...
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CUSTOM_COMMAND_DOWN))
		{
			memset(&nevent, 0, sizeof(NEVENT));
			NFillupEventHead(nevent, NCOMMAND_UICUSTOM_CANCEL_DOWN, 0, NEVENT_RECIPIENT_CAST_UI, pui);
			NSendUICommandEvent(&nevent);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_UP:
		// Send Command Event ...
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CUSTOM_COMMAND_UP))
		{
			memset(&nevent, 0, sizeof(NEVENT));
			NFillupEventHead(nevent, NCOMMAND_UICUSTOM_UP, 0, NEVENT_RECIPIENT_CAST_UI, pui);
			NSendUICommandEvent(&nevent);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_DOWN:
		printf("\n NUI_KEY_INSTANT_PUSH_DOWN");
		// Send Command Event ...
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CUSTOM_COMMAND_DOWN))
		{
			memset(&nevent, 0, sizeof(NEVENT));
			NFillupEventHead(nevent, NCOMMAND_UICUSTOM_DOWN, 0, NEVENT_RECIPIENT_CAST_UI, pui);
			NSendUICommandEvent(&nevent);
		}
		break;

	case NUI_KEY_INSTANT_PUSH_RESET:
		// Nothing to do yet ...
		break;

	default:
		break;
	}
}

static void NUICustom_States_Handle(NUI *pui, const NTIME *ptime)
{
	NEVENT nevent;

	switch (NGET_UIPUSH_PREHANDLE_KEY_INSTANT(NUIPush_States_PreHandle(pui, ptime)))
	{
	// There are only 2 possible DELAYED Key Instants:
	// ( NUI_KEY_INSTANT_PUSH_DOWN_CANCEL is never Delayed ! )
	// Handle DELAYED Key Instant PUSH UP
	case NUI_KEY_INSTANT_PUSH_UP:
		// Send Command Event ...
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CUSTOM_COMMAND_UP))
		{
			memset(&nevent, 0, sizeof(NEVENT));
			NFillupEventHead(nevent, NCOMMAND_UICUSTOM_UP, 0, NEVENT_RECIPIENT_CAST_UI, pui);
			NPostDelayedUIEvent(&nevent);
		}
		break;

	// Handle DELAYED Key Instant PUSH DOWN
	case NUI_KEY_INSTANT_PUSH_DOWN:
		printf("\n NUI_KEY_INSTANT_PUSH_DOWN");
		// Send Command Event ...
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_CUSTOM_COMMAND_DOWN))
		{
			memset(&nevent, 0, sizeof(NEVENT));
			NFillupEventHead(nevent, NCOMMAND_UICUSTOM_DOWN, 0, NEVENT_RECIPIENT_CAST_UI, pui);
			NPostDelayedUIEvent(&nevent);
		}
		break;

	default:
		break;
	}
}

NUI_CUSTOM *NCreateUICustom(NUI *parent, NUI_CUSTOM_DESC *pgvdesc, const Nu32 user32)
{
	return NSetupUICustom(parent, NEW(NUI_CUSTOM), pgvdesc, user32);
}

NUI_CUSTOM *NSetupUICustom(NUI *parent, NUI_CUSTOM *pgv, NUI_CUSTOM_DESC *pgvdesc, const Nu32 user32)
{
	NPushUISetUpProcess((NUI *)pgv, (NUI_DESC *)pgvdesc);

	memset(pgv, 0, sizeof(NUI_CUSTOM));
	// + -------------------------------------------------------------------------------------------------------------------------
	// + CALL MAIN STRUCTURE SETUP
	//		Here, parent structure is NUI.
#ifdef _DEBUG
	if (!NSetupUI(parent, (NUI *)pgv, (NUI_DESC *)pgvdesc, user32))
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (N_UI_SETUP or/and N_UI_ENABLE) ...
		// This should never happen !
		// (Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI)
		NErrorIf(1, NERROR_UI_SETUPPROCESS_STACK_CORRUPTED);
	}
#else
	NSetupUI(parent, (NUI *)pgv, (NUI_DESC *)pgvdesc, user32);
#endif
	// + -------------------------------------------------------------------------------------------------------------------------
	// + KEEP THE NUMBER OF CREATED UI BASIC GEOMETRIES ( Background ? Icon ? )
	// +
	// + This number should be 0,1 or 2 at maximum.
	// + But because "pgv->UIBasicGeomNb" is a 3 bits value, its better to test it before get it !!!
	// + ... and we are going to test it against 3 which is the maximum possible value for 3 bits (obviously)!!!
	NErrorIf(pgv->UI.pRenderable->GeomArray.Size > 3, NERROR_VALUE_OUTOFRANGE);
	pgv->UIBasicGeomNb = pgv->UI.pRenderable->GeomArray.Size;

	// + -------------------------------------------------------------------------------------------------------------------------
	// + RE-SETUP System data & functions
	//		Overwrite some parameters
	pgv->UI.CurrentState = NUI_STATE_PUSH_UP_REST;				 // To take care about "Push".
	pgv->UI.UIState_Proc = NUICustom_States_Handle;				 // To take care about "Push".
	pgv->UI.UIEventToState_Proc = NUICustom_EventToState_Handle; // To take care about "Push".
	pgv->UI.ClearUI_Proc = NClearUICustom;

	// Specific Default Touch Mode ( different than Simple UI )
	if (ISFLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE))
		NSetUIListenTouchMode((NUI *)pgv, NUI_LISTEN_TOUCH_MODE_BASIC_TRACK);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CREATE ALL THE GEOMETRIES OF THE Viewer
	//
	NUIGEOMETRY_DESC *pgd = pgvdesc->pGeometryDescList;
	for (Nu32 i = pgvdesc->GeometryDescNumber; i != 0; i--, pgd++)
	{
		NCreateUIGeometry((NUI *)pgv, pgd);
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + SETUP the DISPLAY ARRAY
	//
	NGEOMETRY *pgeom;
	Nu32 capacity = 0;
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_DISPLAY_ARRAY_CAPACITY))
	{
		capacity = pgvdesc->DisplayArrayCapacity;
	}
	else
	{
		// By default capacity will be set to 1
		capacity = DEFAULT_NUI_CUSTOM_DISPLAY_ARRAY_CAPACITY;
	}
	NSetupArray(&pgv->DisplayArray, capacity, sizeof(NGEOMETRY *));

	// Insert Background geometry into Display Array ?
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_CUSTOM_DISPLAY_BACKGROUND))
	{
		pgeom = NGetUIBackgroundGeometry((NUI *)pgv);
		if (pgeom)
			NArrayPushBack(&pgv->DisplayArray, (NBYTE *)&pgeom);
	}

	// Insert First Extra Geometry into Display Array ?
	// By default all the geometries of the Viewer still hidden.
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_CUSTOM_DISPLAY_FIRST_EXTRA_GEOMETRY))
	{
		// "First EXTRA Geometry Index" belongs to the presence of background and/or icon into the UI Renderable Geometry Array.
		// So to be sure there are some "Extra geometries" into the viewer at this stage we have to test
		// "pgv->UIBasicGeomNb" against the total number of geometries included into the renderable right now ...
		// ( Here, a test like if(NGetUICustomExtraGeometriesNumber(pgv)>0 works fine too !)
		if (pgv->UIBasicGeomNb < NGetIncludedGeometriesNumber(pgv->UI.pRenderable))
		{
			pgeom = NGetIncludedGeometryByIndex(pgv->UI.pRenderable, pgv->UIBasicGeomNb);
			NArrayPushBack(&pgv->DisplayArray, (NBYTE *)&pgeom);
		}
	}
	// Insert Icon geometry into Collection ? (as the last element)
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_CUSTOM_DISPLAY_ICON))
	{
		pgeom = NGetUIIconGeometry((NUI *)pgv);
		if (pgeom)
			NArrayPushBack(&pgv->DisplayArray, (NBYTE *)&pgeom);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + CHANGE EXTRACT FUNCTION
	// +		According with the "Collection"  size
	// +
	// +
	if (ISFLAG_ON(((NUI_DESC *)pgvdesc)->Flags_Core, FLAG_NUIDC_USEPARAM_HIGHLEVEL_EXTRACTSET))
	{
		pgv->HighLevelExtractSet = ((NUI_DESC *)pgvdesc)->HighLevelExtractSet;
	}
	else
	{
		pgv->HighLevelExtractSet = DEFAULT_NUI_RENDERABLE_HIGHLEVEL_EXTRACTSET;
	}
	_update_uicustom_extractFCT(pgv);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SETUP MISC
	// +
	// +
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CUSTOM_FLASH_TICK_DURATION))
	{
		pgv->FlashTickDuration = pgvdesc->FlashTickDuration;
	}
	else
	{
		pgv->FlashTickDuration = DEFAULT_NUI_CUSTOM_FLASH_TICK_DURATION;
	}

	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CUSTOM_FLASH_TICK_PATTERN))
	{
		pgv->FlashTickPattern = pgvdesc->FlashTickPattern;
	}
	else
	{
		pgv->FlashTickPattern = DEFAULT_NUI_CUSTOM_FLASH_TICK_PATTERN;
	}

	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_CUSTOM_COMMAND_UP))
		FLAG_ON(pgv->UI.Flags, FLAG_NUI_CUSTOM_COMMAND_UP);
	else
		FLAG_OFF(pgv->UI.Flags, FLAG_NUI_CUSTOM_COMMAND_UP);

	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_CUSTOM_COMMAND_DOWN))
		FLAG_ON(pgv->UI.Flags, FLAG_NUI_CUSTOM_COMMAND_DOWN);
	else
		FLAG_OFF(pgv->UI.Flags, FLAG_NUI_CUSTOM_COMMAND_DOWN);

	return (NUI_CUSTOM *)NPopUISetUpProcess((NUI *)pgv, (NUI_DESC *)pgvdesc);
}

void NClearUICustom(NUI *pui)
{
	// a)Clear Geometry Collection Section
	NClearArray(&((NUI_CUSTOM *)pui)->DisplayArray, NULL);
	// b)Clear UI Section
	NClearUI_Basics(pui);
}

// ------------------------------------------------------------------------------------------
// NSetUICustomDisplayArray
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NDisplayUICustomBatch(NUI_CUSTOM *pgv, const Nu32 *pNu32_bit_field_display_mask, const Nu32 flags_nui_custom)
{
	NErrorIf(ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXCLUSIVE_DISPLAY), NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(!pNu32_bit_field_display_mask || !pgv, NERROR_NULL_POINTER);

	Nu32 size;
	NGEOMETRY *pgeom;

	// 1)Setup scope
	if (ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE))
	{
		pgeom = &((NGEOMETRY *)pgv->UI.pRenderable->GeomArray.pFirst)[pgv->UIBasicGeomNb];
		size = pgv->UI.pRenderable->GeomArray.Size - pgv->UIBasicGeomNb;
	}
	else
	{
		pgeom = (NGEOMETRY *)pgv->UI.pRenderable->GeomArray.pFirst;
		size = pgv->UI.pRenderable->GeomArray.Size;
	}
	// 2) Make Display Array  empty
	pgv->DisplayArray.Size = 0;
	// 3) Here we are, Insert !!!
	for (Nu32 i = 0; i < size; i++, pgeom++)
	{
		if (Nu32FIELD_BITGET(pNu32_bit_field_display_mask, i))
			NArrayPushBack(&pgv->DisplayArray, (NBYTE *)&pgeom);
	}
	// 4) Update Extract function
	_update_uicustom_extractFCT(pgv);
}

// ------------------------------------------------------------------------------------------
// NDisplayUICustomGeometry
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//			NUI_CUSTOM *pgv		Valid pointer on a NUICustom
//
//			const Nu32 index				index of the geometry to Display. depending the base of counting, index 0 will be he first
//											extra geometry or Background(or Icon) if there is one.
//
//			const Nbool extra_index_base	NTRUE to start counting index from the first extra geometry.
//											in that way potential background and icon remains inaccessible.
//											NFALSE to start counting index from 0. In that way if there is a background and/or an icon
//											User can insert/remove them from display list as other extra geometries.
// Out :
//
// ------------------------------------------------------------------------------------------
const NGEOMETRY *NDisplayUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom)
{
	NGEOMETRY *pgeom;

	// 1) Retrieve the right geometry
	if (ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE))
		pgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index + pgv->UIBasicGeomNb);
	else
		pgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index);

	if (ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXCLUSIVE_DISPLAY))
	{
		// 2a) Just a Quick Resize of the Display Array
		//  NResizeArray(&pgv->DisplayArray,0,NULL);
		pgv->DisplayArray.Size = 0; // ... just because its the same here, and ...quicker !
	}
	else
	{
		// 2b) Check if the found geometry is already showed, or not
		//		If geometry is already inserted into the collection array... nothing to do, just return!
		NGEOMETRY **ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
		for (Nu32 i = pgv->DisplayArray.Size; i != 0; i--, ppgeom++)
		{
			if (*ppgeom == pgeom)
				return pgeom;
		}
	}
	// 3) Here we are, Insert !!!
	NArrayPushBack(&pgv->DisplayArray, (NBYTE *)&pgeom);
	// 4) Update Extract function
	_update_uicustom_extractFCT(pgv);

	return pgeom;
}

const NGEOMETRY *NGetUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom)
{
	// 1) Retrieve the right geometry
	if (ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE))
		return (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index + pgv->UIBasicGeomNb);
	else
		return (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index);
}

const NGEOMETRY *NGetUICustomFirstDisplayedExtraGeometry(NUI_CUSTOM *pgv)
{
	NGEOMETRY *pbkg = NGetUIBackgroundGeometry((NUI *)pgv);
	NGEOMETRY *picon = NGetUIIconGeometry((NUI *)pgv);
	// Check if the displayed geometry is Extra or not
	NGEOMETRY **ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
	for (Nu32 i = pgv->DisplayArray.Size; i != 0; i--, ppgeom++)
	{
		if ((*ppgeom != pbkg) && (*ppgeom != picon))
			return *ppgeom;
	}

	return NULL;
}

void NHideUICustomGeometry(NUI_CUSTOM *pgv, const Nu32 index, const Nu32 flags_nui_custom)
{
	NGEOMETRY *pgeom;

	// 1) Retrieve the right geometry
	if (ISFLAG_ON(flags_nui_custom, FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE))
		pgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index + pgv->UIBasicGeomNb);
	else
		pgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, index);

	// 2) Look for this geometry into the collection array and remove it
	NGEOMETRY **ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
	for (Nu32 i = 0; i < pgv->DisplayArray.Size; i++, ppgeom++)
	{
		if (*ppgeom == pgeom)
		{
			NEraseArrayElement(&pgv->DisplayArray, i, NULL);
			// 3) Update Extract function
			_update_uicustom_extractFCT(pgv);
			break;
		}
	}
}
void NHideAllUICustomGeometries(NUI_CUSTOM *pgv)
{
	NEraseArray(&pgv->DisplayArray, NULL);
	_update_uicustom_extractFCT(pgv);
}

void NUICustomFlashON(NUI_CUSTOM *pgv)
{
	if (ISFLAG_OFF(pgv->HighLevelExtractSet, FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH))
	{
		FLAG_ON(pgv->HighLevelExtractSet, FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH);
		_update_uicustom_extractFCT(pgv);
	}
	else
		return;
}

void NUICustomFlashOFF(NUI_CUSTOM *pgv)
{
	if (ISFLAG_ON(pgv->HighLevelExtractSet, FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH))
	{
		FLAG_OFF(pgv->HighLevelExtractSet, FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH);
		_update_uicustom_extractFCT(pgv);
	}
	else
		return;
}

// Return an ID into the Extra geometry base index !!!
Nu32 NInsertUICustomExtraGeometry(NUI_CUSTOM *pgv, const NUIGEOMETRY_DESC *pgdesc)
{
	Nu32 id, i;
	NGEOMETRY **ppgeom;
	Nbool update_display_ptr;
	// About potential memory reallocation and geometries addresses update.
	// "NCreateUIGeometry" manages itself the pui->pPickingGeometryMesh Address
	// But UICustom has to manage geometry pointers store into the display array ...

	// Pointer to Index
	update_display_ptr = NFALSE;
	if (pgv->DisplayArray.Size && pgv->UI.pRenderable->GeomArray.Size == pgv->UI.pRenderable->GeomArray.Capacity)
	{
		ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
		for (i = 0; i < pgv->DisplayArray.Size; i++, ppgeom++)
		{
			*ppgeom = (NGEOMETRY *)NGetArrayIndex(&pgv->UI.pRenderable->GeomArray, (NBYTE *)(*ppgeom));
		}
	}

	id = pgv->UI.pRenderable->GeomArray.Size - pgv->UIBasicGeomNb;
	NCreateUIGeometry((NUI *)pgv, pgdesc);

	// Index to Pointers
	if (update_display_ptr)
	{
		ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
		for (i = 0; i < pgv->DisplayArray.Size; i++, ppgeom++)
		{
			*ppgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, (Nu32)(*ppgeom));
		}
	}

	return id;
}

void NEraseUICustomExtraGeometry(NUI_CUSTOM *pgv, const Nu32 index_into_extra_geom_base)
{
	Nu32 id, i;
	NGEOMETRY **ppgeom;
	NGEOMETRY *pgeom;
	Nbool update_display_ptr, bupdatepickingmesh;

	id = index_into_extra_geom_base + pgv->UIBasicGeomNb;

	// Hide first, to remove geometry Ptr from Collection array !
	NHideUICustomGeometry(pgv, id, 0);

	bupdatepickingmesh = NFALSE;
	if (pgv->UI.pPickingGeometryMesh && pgv->UI.pRenderable->GeomArray.Size == pgv->UI.pRenderable->GeomArray.Capacity)
	{
		// Check if "pPickingGeometryMesh" belongs to a renderable geometry
		pgeom = (NGEOMETRY *)pgv->UI.pRenderable->GeomArray.pFirst;
		for (i = 0; i < pgv->UI.pRenderable->GeomArray.Size; i++, pgeom++)
		{
			if (&pgeom->Mesh == pgv->UI.pPickingGeometryMesh)
			{
				// Deleted geometry is the one used for picking ...
				if (i == id)
				{
					NResetUIPickingMesh((NUI *)pgv, NULL);
					break;
				}
				else
				{
					bupdatepickingmesh = NTRUE;
					pgv->UI.pPickingGeometryMesh = (NMESH *)i;
					break;
				}
			}
		}
	}

	// About potential memory reallocation and geometries addresses update.
	// pui->pPickingGeometryMesh" Address and geometry pointers store into the display array have to be handle here ...
	// Display geometry Pointers to Index
	update_display_ptr = NFALSE;
	if (pgv->DisplayArray.Size && pgv->UI.pRenderable->GeomArray.Size == pgv->UI.pRenderable->GeomArray.Capacity)
	{
		ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
		for (i = 0; i < pgv->DisplayArray.Size; i++, ppgeom++)
		{
			*ppgeom = (NGEOMETRY *)NGetArrayIndex(&pgv->UI.pRenderable->GeomArray, (NBYTE *)(*ppgeom));
		}
	}

	NDeleteIncludedGeometryByIndex(pgv->UI.pRenderable, index_into_extra_geom_base + pgv->UIBasicGeomNb);

	// In case of Geometry array reallocation.
	if (bupdatepickingmesh)
	{
		if ((Nu32)pgv->UI.pPickingGeometryMesh > id)
		{
			pgv->UI.pPickingGeometryMesh = &((NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, (Nu32)pgv->UI.pPickingGeometryMesh - 1))->Mesh;
		}
		// Geometry Index is lower than deleted geometry (store before in the array )
		else
		{
			pgv->UI.pPickingGeometryMesh = &((NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, (Nu32)pgv->UI.pPickingGeometryMesh))->Mesh;
		}
	}

	// Index to Display geometry Pointers
	if (update_display_ptr)
	{
		ppgeom = (NGEOMETRY **)pgv->DisplayArray.pFirst;
		for (i = 0; i < pgv->DisplayArray.Size; i++, ppgeom++)
		{
			// Geometry Index is greater than deleted geometry (store after in the array)
			if ((Nu32)(*ppgeom) > id)
			{
				*ppgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, (Nu32)(*ppgeom) - 1);
			}
			// Geometry Index is lower than deleted geometry (store before in the array )
			else
			{
				*ppgeom = (NGEOMETRY *)NGetArrayPtr(&pgv->UI.pRenderable->GeomArray, (Nu32)(*ppgeom));
			}
		}
	}
}
