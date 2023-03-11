#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NRectf32.h"
#include "lib/N/Containers/NArray.h"
#include "../../NViewport.h"
#include "lib/N/Miscellaneous/N2DPointInside.h"
#include "../../NCoordinateSystemConversions.h"

#include "lib/N/Utilities/Draw/NUT_Draw.h"
//#include "./Draw/NUT_DrawText.h"

#include "../NUT_3DMouse.h"
#include "NUT_Editor_ATxC_Tool.h"



// ------------------------------------------------------------------------------------------
// In	:
// Out :
//
// ------------------------------------------------------------------------------------------
NUT_EDITOR_ATXC_ENTITY* NUT_Editor_ATxC_SetupEntity(NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_ENTITY_DESC *pdesc)
{
	NErrorIf(!pdesc,NERROR_NULL_POINTER);
	NErrorIf(!pdesc->Pick_CallBack,NERROR_NULL_POINTER);		// |
	NErrorIf(!pdesc->GetElements_CallBack,NERROR_NULL_POINTER);	// |- These 3 ones are not optional
	NErrorIf(!pdesc->Update_CallBack,NERROR_NULL_POINTER);		// |

	Nu32	i;

	Nmem0(pentity,NUT_EDITOR_ATXC_ENTITY);
	pentity->Pick_CallBack			= (NUT_EDITOR_ATXC_PICK_CALLBACK)pdesc->Pick_CallBack;
	pentity->GetElements_CallBack	= (NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK)pdesc->GetElements_CallBack;
	pentity->Update_CallBack		= (NUT_EDITOR_ATXC_UPDATE_CALLBACK)pdesc->Update_CallBack;
	

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_ENTRYPTR) )
		pentity->pEntry = (void*)pdesc->pEntry;
	
	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_TEXTUREPTR) )
	{
		for(i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
		{
			pentity->pArrayOfTexturePtr[i] = pdesc->pArrayOfTexturePtr[i];
		}

#ifdef _DEBUG
		// Check Texture Integrity.
		// To work in the right way, these texture Arrays should not have null pointer inside.
		// That means all their elements have to be valid texture pointer ...
		// Let's check that...in debug mode for sure.
		for(i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
		{
			if( pentity->pArrayOfTexturePtr[i] != NULL && (Nu32)pentity->pArrayOfTexturePtr[i] != NVOID )
			{
				NTEXTURE **debug_ptr = (NTEXTURE**)pentity->pArrayOfTexturePtr[i]->pFirst;
				for(Nu32 debug_i = pentity->pArrayOfTexturePtr[i]->Size; debug_i != 0; debug_i--,debug_ptr++)
				{
					NErrorIf(*debug_ptr == NULL, NERROR_NULL_POINTER); // No null pointer !!!
				}
			}
		}
#endif
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_ATLASELEMENTPTR) )
	{
		for(i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
		{
			pentity->pArrayOfAtlasElementPtr[i] = pdesc->pArrayOfAtlasElementPtr[i];
		}

#ifdef _DEBUG
		// Check Atlas Element Integrity.
		// To work in the right way, these Atlas Element Arrays should not have null pointer inside.
		// That means all their elements have to be valid Atlas Element pointer ...
		// Let's check that...in debug mode for sure.
		for(i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
		{
			if( pentity->pArrayOfAtlasElementPtr[i] != NULL && (Nu32)pentity->pArrayOfAtlasElementPtr[i] != NVOID )
			{
				NATLAS_ELEMENT **debug_ptrb = (NATLAS_ELEMENT**)pentity->pArrayOfAtlasElementPtr[i]->pFirst;
				for(Nu32 debug_j = pentity->pArrayOfAtlasElementPtr[i]->Size; debug_j != 0; debug_j--,debug_ptrb++)
				{
					NErrorIf(*debug_ptrb == NULL, NERROR_NULL_POINTER); // No null pointer !!!
				}
			}
		}
#endif
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_BLENDPTR) )
	{
		pentity->pArrayOfBlendPtr = pdesc->pArrayOfBlendPtr;
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_MATERIALPTR) )
	{
		pentity->pArrayOfMaterialPtr = pdesc->pArrayOfMaterialPtr;
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_OFFSET_TOUCHSCALE) )
	{
		pentity->EditingTxCOffset_TouchScale	= pdesc->EditingTxCOffset_TouchScale;
	}
	else
	{
		NVec2Set(&pentity->EditingTxCOffset_TouchScale,DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_TOUCHSCALE_2F);
	}
	
	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_TOUCHSCALE) )
	{
		pentity->EditingTxCScale_TouchScale	= pdesc->EditingTxCScale_TouchScale;
	}
	else
	{
		NVec2Set(&pentity->EditingTxCScale_TouchScale,DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_TOUCHSCALE_2F);
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_OFFSET_MINMAX) )
	{
		pentity->EditingTxCOffset_Min	= pdesc->EditingTxCOffset_Min;
		pentity->EditingTxCOffset_Max	= pdesc->EditingTxCOffset_Max;
	}
	else
	{
		pentity->EditingTxCOffset_Min	= DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_MIN_1F;
		pentity->EditingTxCOffset_Max	= DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_MAX_1F;
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_SPEED_SWITCH_THRESHOLD) )
	{
		pentity->EditingTxCScale_SpeedSwitchThreshold	= pdesc->EditingTxCScale_SpeedSwitchThreshold;
	}
	else
	{
		NSetTexCoord2f(&pentity->EditingTxCScale_SpeedSwitchThreshold,DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_SPEED_SWITCH_THRESHOLD_2F);
	}

	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_SPEED_MAX) )
	{
		pentity->EditingTxCScale_SpeedMax	= pdesc->EditingTxCScale_SpeedMax;
	}
	else
	{
		NSetTexCoord2f(&pentity->EditingTxCScale_SpeedMax,DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_SPEED_MAX_2F);
	}
	
	if( ISFLAG_ON(pdesc->Flags,FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_BASESCALE) )
	{
		pentity->EditingTxCScale_BaseScale	= pdesc->EditingTxCScale_BaseScale;
	}
	else
	{
		NSetTexCoord2f(&pentity->EditingTxCScale_BaseScale,DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_BASESCALE_2F);
	}
		
	return pentity;
}

void NUT_Editor_ATxC_ClearEntity(NUT_EDITOR_ATXC_ENTITY *pentity)
{
	Nmem0(pentity,NUT_EDITOR_ATXC_ENTITY);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
