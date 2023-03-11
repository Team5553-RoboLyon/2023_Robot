#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCore.h"
#include "../../NCamera.h"
#include "../../UI/NUICustom.h"

#include "../NFrustum_Culling.h"
#include "lib/N/NRenderCell.h"
#include "NRenderable_Extract.h"

// For One NEXTRACTSET SECTION we should have, at least, 16 (2x8) different extraction functions to cover all the possibilities
// Actually we have 12 different extraction functions, 8 for 3D and 4 for HUD (because there is no functions to deal with Frustum Culling for the HUD )

// 3D : Highlevel	FLAG_NEXTRACTSET_3D set to ON
// ------------------------------------------------------------------------------------------------------------------------------------------
// #1: single instance,	single geometry,	no culling		NEXTRACT_SECTION_xx
// #2: single instance,	multi geometries,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIGEOMETRIES
// #3: single instance,	single geometry,	Frustumculling	NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_FRUSTUMCULLING
// #4: single instance,	multi geometries,	Frustumculling	NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIGEOMETRIES|FLAG_NEXTRACTSET_FRUSTUMCULLING

// #5: multi instances,	single geometry,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES
// #6: multi instances,	multi geometries,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES|FLAG_NEXTRACTSET_MULTIGEOMETRIES
// #7: multi instances,	single geometry,	Frustumculling	NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES|FLAG_NEXTRACTSET_FRUSTUMCULLING
// #8: multi instances,	multi geometries,	Frustumculling	NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES|FLAG_NEXTRACTSET_MULTIGEOMETRIES|FLAG_NEXTRACTSET_FRUSTUMCULLING

// #.: And more in case of additional user flags (like other type of culling)
//
// HUD : Highlevel	FLAG_NEXTRACTSET_HUD set to ON (no frustum culling)
// ------------------------------------------------------------------------------------------------------------------------------------------
// #1: single instance,	single geometry,	no culling		NEXTRACT_SECTION_xx
// #2: single instance,	multi geometries,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIGEOMETRIES
// #5: multi instances,	single geometry,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES
// #6: multi instances,	multi geometries,	no culling		NEXTRACT_SECTION_xx|FLAG_NEXTRACTSET_MULTIINSTANCES|FLAG_NEXTRACTSET_MULTIGEOMETRIES


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to 3D Accumulators
// +
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY_FRUSTUMCULLING
Nu32 NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	Nf32				sortingkey;

	if( !NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		// ExtractAll Collection Geometries... one by one ! 
		NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
		for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
		{
			NExtract3DRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
		}
		return pgc->DisplayArray.Size;
	}
}

Nu32 NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	Nf32				sortingkey;

	if( ((NUI*)powner)->pUITransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		sortingkey = NGetZ_CameraBase(&v);
	}
	else
	{
		sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
	}

	// ExtractAll Collection Geometries... one by one ! 
	NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
	for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
	{
		NExtract3DRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
	}
	return pgc->DisplayArray.Size;
}

Nu32 NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	Nf32				sortingkey;

	if( !NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		NExtract3DRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode, sortingkey );
		return 1;
	}
}

Nu32 NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	Nf32				sortingkey;

	if( ((NUI*)powner)->pUITransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		sortingkey = NGetZ_CameraBase(&v);
	}
	else
	{
		sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
	}

	NExtract3DRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode, sortingkey );
	return 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to HUD Accumulator
Nu32 NRenderableExtract_UICustom_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	NExtractHUDRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode );
	return 1;
}
Nu32 NRenderableExtract_UICustom_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;

	// ExtractAll Collection Geometries... one by one ! 
	NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
	for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
	{
		NExtractHUDRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode );
	}
	return pgc->DisplayArray.Size;

}

////////////////////////////
//////////////////////////// !!! WITH GEOMETRIES FLASH implementation
////////////////////////////

static inline Nu32 _flashupdate(NUI_CUSTOM	*pgv)
{
	// FLASH COUNT UPDATE
	if(pgv->CurrentFlashTickDuration < pgv->FlashTickDuration)
	{
		pgv->CurrentFlashTickDuration++;
	}
	else
	{
		pgv->CurrentFlashTickDuration = 0;

		if( pgv->CurrentFlashTickID < 7 )
			pgv->CurrentFlashTickID ++;
		else
			pgv->CurrentFlashTickID = 0;
	}
	
	return BITGET(pgv->FlashTickPattern,pgv->CurrentFlashTickID);
}

Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	
	if(!_flashupdate(pgc))
		return 0;

	Nf32				sortingkey;
	if( !NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		// ExtractAll Collection Geometries... one by one ! 
		NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
		for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
		{
			NExtract3DRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
		}
		return pgc->DisplayArray.Size;
	}
}

Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	
	if(!_flashupdate(pgc))
		return 0;

	Nf32				sortingkey;
	if( ((NUI*)powner)->pUITransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		sortingkey = NGetZ_CameraBase(&v);
	}
	else
	{
		sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
	}

	// ExtractAll Collection Geometries... one by one ! 
	NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
	for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
	{
		NExtract3DRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
	}
	return pgc->DisplayArray.Size;
}

Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	if(!_flashupdate(pgc))
		return 0;

	Nf32				sortingkey;
	if( !NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		NExtract3DRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode, sortingkey );
		return 1;
	}
}

Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	if(!_flashupdate(pgc))
		return 0;

	Nf32				sortingkey;
	if( ((NUI*)powner)->pUITransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		sortingkey = NGetZ_CameraBase(&v);
	}
	else
	{
		sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
	}

	NExtract3DRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode, sortingkey );
	return 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to HUD Accumulator
Nu32 NRenderableExtract_Flash_UICustom_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	if(!_flashupdate(pgc))
		return 0;
	NExtractHUDRenderCell(*(NGEOMETRY**)pgc->DisplayArray.pFirst,((NUI*)powner)->pUITransformHNode );
	return 1;
}
Nu32 NRenderableExtract_Flash_UICustom_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CUSTOM	*pgc = (NUI_CUSTOM*)powner;
	if(!_flashupdate(pgc))
		return 0;
	// ExtractAll Collection Geometries... one by one ! 
	NGEOMETRY	**ppgeom = (NGEOMETRY**)pgc->DisplayArray.pFirst;
	for(Nu32 i=pgc->DisplayArray.Size;i!=0;i--,ppgeom++)
	{
		NExtractHUDRenderCell(*ppgeom,((NUI*)powner)->pUITransformHNode );
	}
	return pgc->DisplayArray.Size;

}

