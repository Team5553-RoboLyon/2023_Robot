#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "../../NType.h"
#include "../../NCStandard.h"
#include "NUT_Editor_ATxC_Gizmo.h"



NUT_EDITOR_ATXC_GIZMO*	NUT_ATxC_SetUp_Gizmo(NUT_EDITOR_ATXC_GIZMO *pgizmo, const NUT_EDITOR_ATXC_GIZMO_DESC *pdesc )
{
	// 'pdesc' may be NULL. In that case, all the gizmo is going to stay with the default params ...
	
	Nmem0(pgizmo,NUT_EDITOR_ATXC_GIZMO);
	// 1) All the default Params
	FLAG_OFF(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START );
	FLAG_OFF(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_KEEP_SELECTION );
	FLAG_OFF(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_MOVE_TO_PICK_POSITION );
	
	NVec2Set(&pgizmo->DragHandleSize, DEFAULT_NUT_EDITOR_ATXC_DRAG_HANDLE_SIZE_2F );
	NVec2Set(&pgizmo->HandleSize, DEFAULT_NUT_EDITOR_ATXC_HANDLE_SIZE_2F );
	NVec2Set(&pgizmo->DefaultPosition,DEFAULT_NUT_EDITOR_ATXC_POSITION2F);
	
	NSetColorf( &pgizmo->PassiveColor,DEFAULT_NUT_EDITOR_ATXC_PASSIVE_COLOR4F );
	NSetColorf( &pgizmo->ActiveColor,DEFAULT_NUT_EDITOR_ATXC_ACTIVE_COLOR4F );
	NSetColorf( &pgizmo->InUseColor,DEFAULT_NUT_EDITOR_ATXC_INUSE_COLOR4F);
	
	pgizmo->DefaultEntityID = NVOID; // maybe 0 will be better, but with NVOID we will have CRASH for borderline uses
	
	if(pdesc)
	{
		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START) )
		{
			FLAG_ON(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START );
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_KEEP_SELECTION) )
		{
			FLAG_ON(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_KEEP_SELECTION );
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_MOVE_TO_PICK_POSITION) )
		{
			FLAG_ON(pgizmo->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_MOVE_TO_PICK_POSITION );
		}

		// Params
		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DRAG_HANDLE_SIZE) )
		{
			pgizmo->DragHandleSize = pdesc->DragHandleSize;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_GIZMO_HANDLE_SIZE) )
		{
			pgizmo->HandleSize = pdesc->HandleSize;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_PASSIVECOLOR) )
		{
			pgizmo->PassiveColor = pdesc->PassiveColor;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_ACTIVECOLOR) )
		{
			pgizmo->ActiveColor = pdesc->ActiveColor;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_INUSECOLOR) )
		{
			pgizmo->InUseColor = pdesc->InUseColor;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DEFAULT_POSITION) )
		{
			pgizmo->DefaultPosition = pdesc->DefaultPosition;
		}

		if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DEFAULT_ENTITY_ID) )
		{
			pgizmo->DefaultEntityID = pdesc->DefaultEntityID;
		}

	}

	// Setup initial position at default position
	// if user set FLAG_NUT_EDITOR_ATXC_GIZMO_MOVE_TO_PICK_POSITION this initial position is going to be overwrite at the first pick.
	// if not, it will still like this until user drag/move it ...
	pgizmo->Position.x = pgizmo->DefaultPosition.x - pgizmo->DragHandleSize.x*0.5f; // Main Gizmo Box X Center at X position 
	pgizmo->Position.y = pgizmo->DefaultPosition.y - pgizmo->DragHandleSize.y*0.5f; // Main Gizmo Box Y Center at Y position

	return pgizmo;
}

void	NUT_ATxC_Clear_Gizmo(void *pgizmo)
{
	Nmem0(pgizmo,NUT_EDITOR_ATXC_GIZMO);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
