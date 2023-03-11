#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "lib/N/NType.h"
#include "lib/N/NCStandard.h"
#include "lib/N/Containers/NArray.h"
#include "NUT_Editor_ATxC_Gizmo.h"
#include "NUT_Editor_ATxC_Tool.h"


NUT_EDITOR_ATXC_TOOLDATA*	NUT_Editor_Setup_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NUT_EDITOR_ATXC_GIZMO_DESC *pgizmodesc )
{
	Nmem0(ptooldata,NUT_EDITOR_ATXC_TOOLDATA);
	NUT_ATxC_SetUp_Gizmo( &ptooldata->HUDGizmo, pgizmodesc );
	NSetupArray(&ptooldata->EntityArray,CONSTANT_NUT_EDITOR_ATXC_ENTITY_ARRAY_INITIAL_CAPACITY, sizeof(NUT_EDITOR_ATXC_ENTITY) );
	return ptooldata;
}

NUT_EDITOR_ATXC_TOOLDATA*	NUT_Editor_Create_ATxC_ToolData(const NUT_EDITOR_ATXC_GIZMO_DESC *pgizmodesc )
{
	return NUT_Editor_Setup_ATxC_ToolData(NEW(NUT_EDITOR_ATXC_TOOLDATA),pgizmodesc );
}

void	NUT_Editor_Clear_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	NUT_EDITOR_ATXC_TOOLDATA	*ptooldata_cast = (NUT_EDITOR_ATXC_TOOLDATA*)ptooldata;

	NUT_ATxC_Clear_Gizmo( &ptooldata_cast->HUDGizmo );
	NClearArray( &ptooldata_cast->EntityArray, NULL );
	Nmem0(	ptooldata_cast, NUT_EDITOR_ATXC_TOOLDATA);
}

void	NUT_Editor_Delete_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	NUT_Editor_Clear_ATxC_ToolData(ptooldata);
	Nfree(ptooldata);
}

// ------------------------------------------------------------------------------------------
// NUT_Editor_ATxC_Tool_BindEntity
// ------------------------------------------------------------------------------------------
// Description :
// Store data about an entity to allow engine edit its texture and UVs in the right way trough
// the ATxC Tool.
// 
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_EDITOR_ATXC_TOOLDATA				*ptooldata					
//	const	NUT_EDITOR_ATXC_ENTITY_DESC				*pdesc						
//	const	NUT_EDITOR_ATXC_PICK_AND_START_CALLBACK	pickandstart_callback		
//	const	NUT_EDITOR_ATXC_UPDATE_CALLBACK			update_callback				
//	const	NARRAY									*parrayoftextureptr			
//	const	NVEC2								*puvscale_orthonormal_size	
// Out :
//			Nu32	Entity ID (useful for deleting)
//
// ------------------------------------------------------------------------------------------
Nu32 NUT_Editor_ATxC_Tool_BindEntity(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NUT_EDITOR_ATXC_ENTITY_DESC *pdesc)
{
	Nu32 id = NGetArraySize(&ptooldata->EntityArray);
	NUpSizeArray(&ptooldata->EntityArray,1,NULL);
	NUT_Editor_ATxC_SetupEntity( (NUT_EDITOR_ATXC_ENTITY*)NGetLastArrayPtr(&ptooldata->EntityArray),pdesc);
	return id;
}

// ------------------------------------------------------------------------------------------
// NUT_Editor_ATxC_Tool_UnbindEntity
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_EDITOR_ATXC_TOOLDATA	*ptooldata		The ToolData associated with the ATxC tool.
//			Nu32						id				Id of the entity ( == array index of the entity )
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Editor_ATxC_Tool_UnbindEntity(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, Nu32	id)
{
	Nu32 curid = NVOID;

	if(ptooldata->pCurrentEntity)
	{
		curid = NGetArrayIndex(&ptooldata->EntityArray, (NBYTE*)ptooldata->pCurrentEntity );
	}

	NEraseArrayElement( &ptooldata->EntityArray,id,NULL );

	// Preserve the current entity, even if there is a memory managment...
	if(curid != NVOID)
	{
		if(curid == id)
			ptooldata->pCurrentEntity = NULL;
		else
		{
			if(curid > id )
				curid--;
			ptooldata->pCurrentEntity = (NUT_EDITOR_ATXC_ENTITY *)NGetArrayPtr(&ptooldata->EntityArray,curid);
		}
	}

	// Unbind entity was the default one ...
	if(id == ptooldata->HUDGizmo.DefaultEntityID)
		ptooldata->HUDGizmo.DefaultEntityID = NVOID;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
