#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCore.h"
#include "../../NCamera.h"
#include "../../UI/NUICheckBox.h"

#include "../NFrustum_Culling.h"
#include "../NRenderCell.h"
#include "NRenderable_Extract.h"

//#include "../../Utilities/NUT_Draw.h"

// For One NEXTRACTSET SECTION we should have, at least, 16 (2x8) different extraction functions to cover all the possibilities
//
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



Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;
	Nf32				sortingkey;
	Nu32				extracted = 0;

	if( NFrustumCulling_BSphere_vs_3DFrustum(&prenderable->BoundingSphere,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&sortingkey) == NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
			pexception = pcheckbox->pUnCheckTag_Geometry;
		else
			pexception = pcheckbox->pCheckTag_Geometry;

		// ExtractAllGeometries... one by one, except the exception ! 
		NGEOMETRY	*pgeom = (NGEOMETRY*)pcheckbox->UI.pRenderable->GeomArray.pFirst;
		for(Nu32 i=pcheckbox->UI.pRenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			if(pgeom != pexception)
			{
				NExtract3DRenderCell(pgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
				extracted++;
			}
		}
		return extracted;
	}
}

Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;
	Nu32				extracted = 0;
	Nf32				sortingkey;

	if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
		pexception = pcheckbox->pUnCheckTag_Geometry;
	else
		pexception = pcheckbox->pCheckTag_Geometry;

	if( prenderable->pTransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		sortingkey = NGetZ_CameraBase(&v);
	}
	else
	{
		sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
	}

	// ExtractAllGeometries... one by one, except the exception ! 
	NGEOMETRY	*pgeom = (NGEOMETRY*)pcheckbox->UI.pRenderable->GeomArray.pFirst;
	for(Nu32 i=pcheckbox->UI.pRenderable->GeomArray.Size;i!=0;i--,pgeom++)
	{
		if(pgeom != pexception)
		{
			NExtract3DRenderCell(pgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
			extracted++;
		}
	}
	return extracted;
}

Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;
	Nf32				sortingkey;
//	Nu32				extracted = 0;

	if( NFrustumCulling_BSphere_vs_3DFrustum(&prenderable->BoundingSphere,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&sortingkey) == NFRUSTUM_CULLING_OUTSIDE )
	{
		return 0;
	}
	else
	{
		if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
			pexception = pcheckbox->pUnCheckTag_Geometry;
		else
			pexception = pcheckbox->pCheckTag_Geometry;

		NGEOMETRY	*pgeom = (NGEOMETRY *)pcheckbox->UI.pRenderable->GeomArray.pFirst;
		if(pgeom != pexception)
		{
			NExtract3DRenderCell(pgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;
	Nf32				sortingkey;

	if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
		pexception = pcheckbox->pUnCheckTag_Geometry;
	else
		pexception = pcheckbox->pCheckTag_Geometry;

	NGEOMETRY	*pgeom = (NGEOMETRY*)pcheckbox->UI.pRenderable->GeomArray.pFirst;
	if(pgeom != pexception)
	{
		if( prenderable->pTransformHNode )
		{
			NVEC3	v;
			NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
			sortingkey = NGetZ_CameraBase(&v);
		}
		else
		{
			sortingkey = NGetZ_CameraBase(&prenderable->BoundingSphere.Center);
		}
		NExtract3DRenderCell(pgeom,((NUI*)powner)->pUITransformHNode, sortingkey );
		return 1;
	}
	else
	{
		return 0;
	}
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to 3D Accumulators
// +
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY_FRUSTUMCULLING


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to HUD Accumulator
Nu32 NRenderableExtract_UICheckBox_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;

	if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
		pexception = pcheckbox->pUnCheckTag_Geometry;
	else
		pexception = pcheckbox->pCheckTag_Geometry;

	NGEOMETRY	*pgeom = (NGEOMETRY*)pcheckbox->UI.pRenderable->GeomArray.pFirst;
	if(pgeom != pexception)
	{
		NExtractHUDRenderCell(pgeom,((NUI*)powner)->pUITransformHNode);
		return 1;
	}
	return 0;
}
Nu32 NRenderableExtract_UICheckBox_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NUI_CHECKBOX	*pcheckbox = (NUI_CHECKBOX*)powner;
	NGEOMETRY			*pexception;
	Nu32				extracted = 0;

	if(ISFLAG_ON(pcheckbox->UI.Flags,FLAG_NUI_CHECKBOX_CHECK))
		pexception = pcheckbox->pUnCheckTag_Geometry;
	else
		pexception = pcheckbox->pCheckTag_Geometry;

	// ExtractAllGeometries... one by one, except the exception ! 
	NGEOMETRY	*pgeom = (NGEOMETRY*)pcheckbox->UI.pRenderable->GeomArray.pFirst;
	for(Nu32 i=pcheckbox->UI.pRenderable->GeomArray.Size;i!=0;i--,pgeom++)
	{
		if(pgeom != pexception)
		{
			NExtractHUDRenderCell(pgeom,((NUI*)powner)->pUITransformHNode);
			extracted++;
		}
	}
	return extracted;
}
