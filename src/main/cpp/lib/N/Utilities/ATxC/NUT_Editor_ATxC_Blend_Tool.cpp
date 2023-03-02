#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "../../NCStandard.h"
#include "../../Libraries/NLibraries.h"
#include "NUT_Editor_ATxC_Tool.h"
#include "NUT_Editor_ATxC_Gizmo_Blend_InLine.h"

Nu32 NUT_ATxC_GetBlendListSize(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	NARRAY *parray = ptooldata->pCurrentEntity->pArrayOfBlendPtr;

	if( parray && parray->Size )
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
		{
			return parray->Size + 1;
		}
		else
		{
			return parray->Size;
		}
	}
	else if( NGetBlendsLibraryCount() )
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
		{
			return NGetBlendsLibraryCount() + 1;
		}
		else
		{
			return NGetBlendsLibraryCount();
		}
	}
	else
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
			return 1;
		else
			return 0;
	}
}
// return NVOID if blend is not included !
// return 0 if pblend == NULL && ... GIZMO_INCLUDE_NULL_BLEND !
// ... return 0 also if pblend != 0 and GIZMO doesn't include NULL BLEND.
Nu32 NUT_ATxC_GetBlendIndex(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NBLEND *pblend)
{
	NBLEND		**pptr;
	Nu32		index;
	Nu32		indexshifting;
	Nu32		i;
	NARRAY		*parray;

	// 1/ +---+ pblend == NULL
	if( !pblend )
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
			return 0;
		else
			return NVOID;
	}

	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
		indexshifting = 1;
	else
		indexshifting = 0;

	// 2/ +---+ pblend != NULL 
	parray = ptooldata->pCurrentEntity->pArrayOfBlendPtr;
	if( parray && parray->Size )
	{
		// look for the current blend into the Array of Preloaded blend Ptr
		index = NVOID;
		pptr = (NBLEND**)NGetFirstArrayPtr(parray);
		for (i=0;i< NGetArraySize(parray);i++,pptr++)
		{
			if(*pptr == pblend)
			{
				return i+indexshifting;
			}
		}
		NErrorIf(1,NERROR_ARRAY_ELEMENT_NOT_FOUND ); // TODO manage this case by including the not found blend into the array ?
		return NVOID;
	}
	else if( NGetBlendsLibraryCount() ) // By default use the Whole Blend library to work in.
	{
		// look for the current blend into the Texture Library
		i = NGetLibraryBlendValidIndex(pblend->pName);
		NErrorIf(i==NVOID,NERROR_ARRAY_ELEMENT_NOT_FOUND ); // TODO manage this case by including the not blend texture into the library ?
		if(i==NVOID)
			return NVOID;
		else
			return (i+indexshifting);
	}
	else 
	{
		// ATxC Tool doesn't have any blend list to look for into.
		NErrorIf(1,NERROR_ARRAY_ELEMENT_NOT_FOUND ); // TODO manage this case by including the not found blend into the different possible lists ?
		return NVOID;
	}
}

NBLEND* NUT_ATxC_GetBlendIndexPtr(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 blendindex)
{
	NARRAY *parray = ptooldata->pCurrentEntity->pArrayOfBlendPtr;
	if( parray && parray->Size )
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
		{
			if(blendindex == 0)
			{
				return NULL;
			}
			else
			{
				NErrorIf(blendindex-1>=parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
				return *(NBLEND**)NGetArrayPtr(parray,blendindex-1);
			}
		}
		else
		{
			NErrorIf(blendindex>=parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
			return *(NBLEND**)NGetArrayPtr(ptooldata->pCurrentEntity->pArrayOfBlendPtr,blendindex);
		}
	}
	else if( NGetBlendsLibraryCount() ) // By default use the Whole Texture library to work in.
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) )
		{
			if(blendindex == 0)
			{
				return NULL;
			}
			else
			{
				return NLookupLibraryBlendByValidIndex(blendindex-1);
			}
		}
		else
		{
			return NLookupLibraryBlendByValidIndex(blendindex);
		}
	}
	else 
	{
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED ) && blendindex == 0 )
			return NULL;

		// User ask for a blend but there is NO BLEND ! NO WHERE !
		NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
	}

	NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
	return NULL;
}
void NUT_ATxC_SetBlendHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 blendindex)
{
	NVEC2	vex;
	Nf32		_cos,_sin;
	Nu32		sectornb = NUT_ATxC_GetBlendListSize(ptooldata);

	NErrorIf(!sectornb,NERROR_NULL_VALUE);
	NErrorIf(blendindex>=sectornb,NERROR_VALUE_OUTOFRANGE);

	Nf32 angle=(NUT_EDITOR_ATXC_ARC_SELECTOR_ID_BLEND-1.0f)*90.0f + CONSTANT_NUT_ATXC_ARC_START + ((Nf32)blendindex+0.5f)*CONSTANT_NUT_ATXC_ARC_LENGTH/(Nf32)sectornb;
	NFastCosSin( NDEGtoANGLE(angle), &_cos,&_sin );
	NUT_ATxC_GetArcSelector_Extents( &vex, &ptooldata->HUDGizmo );
	ptooldata->HUDGizmo.BlendHandlePosition.x = vex.x * _cos;
	ptooldata->HUDGizmo.BlendHandlePosition.y = vex.y * _sin;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
