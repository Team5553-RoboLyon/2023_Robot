#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "lib/N/Geometry/NGeometry.h"
// #include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/Collectibles/NCollectibles.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"

NCOLLECTIBLE_SET NCollectibleSet;

void NInitCollectiblePicking(const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity)
{
	NSetupCollectibleSet(&NCollectibleSet, collectible_ptr_capacity, picker_capacity);
}

void NQuitCollectiblePicking()
{
	NClearCollectibleSet(&NCollectibleSet);
}

void NEnableCollectibleDetection(NCOLLECTIBLE *pcollectible)
{
#ifdef _DEBUG
	NErrorIf(pcollectible->BindID != 0xFFFF, NERROR_UNAUTHORIZED_ACTION); // It seems the collectible is already bind ...
	NCOLLECTIBLE **dbg_ptr = (NCOLLECTIBLE **)NCollectibleSet.CollectiblePtrArray.pFirst;
	for (Nu32 dbg_i = NCollectibleSet.CollectiblePtrArray.Size; dbg_i != 0; dbg_i--, dbg_ptr++)
	{
		NErrorIf(*dbg_ptr == pcollectible, NERROR_ARRAY_ELEMENT_ALREADY_EXIST);
	}
#endif
	pcollectible->BindID = _SafeNu32ToNu16(NCollectibleSet.CollectiblePtrArray.Size);
	NArrayPushBack(&NCollectibleSet.CollectiblePtrArray, (NBYTE *)&pcollectible);
}

void NDisableCollectibleDetection(NCOLLECTIBLE *pcollectible)
{
	// Check if collectible is bind for true ...
	NErrorIf(pcollectible->BindID == 0xFFFF, NERROR_UNAUTHORIZED_ACTION);

	// Hand made Quick Erase ( the last collectible ptr of the array takes the place of the unbound one )
	NCOLLECTIBLE **ppcol = (NCOLLECTIBLE **)NCollectibleSet.CollectiblePtrArray.pFirst;
	ppcol[pcollectible->BindID] = ppcol[NCollectibleSet.CollectiblePtrArray.Size - 1];
	ppcol[pcollectible->BindID]->BindID = pcollectible->BindID;
	pcollectible->BindID = 0xFFFF;

	NCollectibleSet.CollectiblePtrArray.Size -= 1;
}

void NDisableAllCollectibleDetection()
{
	NCOLLECTIBLE **ppcol = (NCOLLECTIBLE **)NCollectibleSet.CollectiblePtrArray.pFirst;
	for (Nu32 j = NCollectibleSet.CollectiblePtrArray.Size; j != 0; j--, ppcol++)
	{
		NErrorIf((*ppcol)->BindID == 0xFFFF, NERROR_SYSTEM_CHECK); // Check if collectible is bind for true ...
		(*ppcol)->BindID = 0xFFFF;
	}

	NCollectibleSet.CollectiblePtrArray.Size = 0;
}

Nu32 NInsertCollectiblePicker(const NVEC3 *ppickerposition, const Nf32 radius)
{
#ifdef _DEBUG
	NCOLLECTIBLE_PICKER *ppicker_dbg = (NCOLLECTIBLE_PICKER *)NCollectibleSet.PickerArray.pFirst;
	for (Nu32 i = NCollectibleSet.PickerArray.Size; i != 0; i--, ppicker_dbg++)
	{
		NErrorIf(ppicker_dbg->pPosition == ppickerposition && ppicker_dbg->Radius == radius, NERROR_COLLECTIBLE_ALREADY_INSERTED_PICKER);
	}
#endif

	NCOLLECTIBLE_PICKER *ppicker = (NCOLLECTIBLE_PICKER *)NArrayAllocBack(&NCollectibleSet.PickerArray);
	ppicker->pPosition = (NVEC3 *)ppickerposition;
	ppicker->Radius = radius;
	return NCollectibleSet.PickerArray.Size - 1;
}

void NEraseCollectiblePicker(const Nu32 index)
{
	NQuickEraseArrayElement(&NCollectibleSet.PickerArray, index, NULL);
}

void NEraseAllCollectiblePicker()
{
	NCollectibleSet.PickerArray.Size = 0;
}
Nu32 NGetCollectiblePickerCount()
{
	return NCollectibleSet.PickerArray.Size;
}

// ------------------------------------------------------------------------------------------
// void NUpdateCollectiblePicking
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is to process of Collectible picking by each Picker.
//	So, each picker of the PickerPtrArray List is going to parse the entire CollectiblePtrArray
//	to look for a potential Collectible element to pick.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUpdateCollectiblePicking()
{
	Nu32 i, j, k;
	NAABB picker_aabb;
	NVEC3 element_position, vdist;
	Nf32 pick_distance;
	Nu32 new_bitshape;
	NCOLLECTIBLE **ppcollectible;
	NCOLLECTIBLE *pcollectible;
	NCOLLECTIBLE_PICKER *picker;

	/*
		NCOLOR color={NCOLOR_PRESET3F_PINK_CHINESE,1.0f};
		NUT_DRAW_ELLIPSE	ellipse;
	*/

	picker = (NCOLLECTIBLE_PICKER *)NCollectibleSet.PickerArray.pFirst;
	for (i = NCollectibleSet.PickerArray.Size; i != 0; i--, picker++)
	{
		// Compute picker AABB
		picker_aabb.fXMax = picker->pPosition->x + picker->Radius;
		picker_aabb.fXMin = picker->pPosition->x - picker->Radius;
		picker_aabb.fYMax = picker->pPosition->y + picker->Radius;
		picker_aabb.fYMin = picker->pPosition->y - picker->Radius;
		// 		picker_aabb.fZMax = picker->pPosition->z + picker->Radius;
		// 		picker_aabb.fZMin = picker->pPosition->z - picker->Radius;

		/*
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
				NUT_Draw_AABB(&picker_aabb,&color);
				NUT_SetDrawConstructionPlane(_PLANE_XY);
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 16;
				ellipse.Center	= *picker->pPosition;
				ellipse.Extents.x = ellipse.Extents.y = picker->Radius;
				NSetColorf(&ellipse.Color,NCOLOR_PRESET3F_YELLOW,1.0f);
				NUT_Draw_Ellipse(&ellipse);
		*/

		ppcollectible = (NCOLLECTIBLE **)NCollectibleSet.CollectiblePtrArray.pFirst;
		for (j = NCollectibleSet.CollectiblePtrArray.Size; j != 0; j--, ppcollectible++)
		{
			pcollectible = *ppcollectible;

			/*
						NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
						NUT_Draw_AABB(&pcollectible->AABB,&color);
			*/

			// 0) Test Picker AABB against Collectible AABB
			if (picker_aabb.fXMax < pcollectible->AABB.fXMin)
				continue;

			if (picker_aabb.fXMin > pcollectible->AABB.fXMax)
				continue;

			if (picker_aabb.fYMax < pcollectible->AABB.fYMin)
				continue;

			if (picker_aabb.fYMin > pcollectible->AABB.fYMax)
				continue;
			/*
						if( picker_aabb.fZMax < pcollectible->AABB.fZMin )
							continue;
						if( picker_aabb.fZMin > pcollectible->AABB.fZMax )
							continue;
			*/
			// 1) Test Picker against each Collectible Element
			new_bitshape = pcollectible->BitShape;
			// Compute picking distance between Picker and Collectible element
			pick_distance = picker->Radius + pcollectible->ElementRadius;
			for (k = 0; k < pcollectible->ElementCount; k++)
			{
				if (BITGET(new_bitshape, k)) //  That means collectible is active and visible !
				{
					pcollectible->pGetElementPosition(&element_position, pcollectible, k);

					/*
										NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
										NUT_SetDrawConstructionPlane(_PLANE_XY);
										Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
										ellipse.SliceNb	= 16;
										ellipse.Center	= element_position;
										ellipse.Extents.x = ellipse.Extents.y = pcollectible->ElementRadius;
										NSetColorf(&ellipse.Color,NCOLOR_PRESET3F_YELLOW,1.0f);
										NUT_Draw_Ellipse(&ellipse);
					*/

					// 2) Test Picker AABB against Element AABB
					if (picker_aabb.fXMax < element_position.x - pcollectible->ElementRadius)
						continue;

					if (picker_aabb.fXMin > element_position.x + pcollectible->ElementRadius)
						continue;

					if (picker_aabb.fYMax < element_position.y - pcollectible->ElementRadius)
						continue;

					if (picker_aabb.fYMin > element_position.y + pcollectible->ElementRadius)
						continue;
					/*
										if( picker_aabb.fZMax < element_position.z - pcollectible->ElementRadius )
											continue;

										if( picker_aabb.fZMin > element_position.z + pcollectible->ElementRadius )
											continue;
					*/
					// 3) Test Precise Distance between Picker and Element
					/*
					// 3D
					NVec3Sub(&vdist,&element_position,picker->pPosition);
					if( (NVec3FastLength(&vdist) <= pick_distance) && pcollectible->pPicking_CallBack(picker,pcollectible,k,&element_position) )
					{
						BITCLEAR(new_bitshape,k);
					}
					*/
					// 2D
					vdist.x = element_position.x - picker->pPosition->x;
					vdist.y = element_position.y - picker->pPosition->y;
					if ((NVec2FastLength((NVEC2 *)&vdist) <= pick_distance) && pcollectible->pPicking_CallBack(picker, pcollectible, k, &element_position))
					{
						BITCLEAR(new_bitshape, k);
					}
				}
			}

			if (pcollectible->BitShape != new_bitshape)
				NSetCollectibleBitShape(pcollectible, new_bitshape);
		}
	}
}

/*
static inline void _UpdateCollectiblesPickingArray(NARRAY *pcollectiblepickingcouple_addressarray)
{
	Nu32						i;
	NCOLLECTIBLE_PICKING_COUPLE	**ppcpl;

	ppcpl = (NCOLLECTIBLE_PICKING_COUPLE**)pcollectiblepickingcouple_addressarray->pFirst;
	for(i=pcollectiblepickingcouple_addressarray->Size;i!=0;i--,ppcpl++)
	{
		(*ppcpl)->pPicking_FCT(*ppcpl);
	}
}

void NUpdateCollectiblesPickingArray(NARRAY *pcollectiblepickingcouple_addressarray)
{
	_UpdateCollectiblesPickingArray(pcollectiblepickingcouple_addressarray);
}

void NCollectiblesPickingArray_PostUpdateHandle(const NTIME *ptime, Nu32 dataA,Nu32 dataB)
{
	_UpdateCollectiblesPickingArray((NARRAY*)dataA);
}

static inline void _UpdateCollectiblesPickingXNodeList(NXNODELIST *pcollectiblepickingcouple_addressXNodeList)
{
	NXNODE	*pxn;

	pxn = XFIRST(pcollectiblepickingcouple_addressXNodeList);
	while( XVALID(pxn,pcollectiblepickingcouple_addressXNodeList) )
	{
		((NCOLLECTIBLE_PICKING_COUPLE	*)XX(pxn))->pPicking_FCT((NCOLLECTIBLE_PICKING_COUPLE	*)XX(pxn));
		XNEXT(pxn);
	}
}

void NUpdateCollectiblesPickingXNodeList(NXNODELIST *pcollectiblepickingcouple_addressXNodeList)
{
	_UpdateCollectiblesPickingXNodeList(pcollectiblepickingcouple_addressXNodeList);
}

void NCollectiblesPickingXNodeList_PostUpdateHandle(const NTIME *ptime, Nu32 dataA,Nu32 dataB)
{
	_UpdateCollectiblesPickingXNodeList((NXNODELIST*)dataA);
}

*/
