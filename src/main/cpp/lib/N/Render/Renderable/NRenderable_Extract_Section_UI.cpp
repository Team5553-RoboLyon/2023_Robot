#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCore.h"
#include "../../NCamera.h"
#include "lib/N/UI/NUI.h"

#include "../NFrustum_Culling.h"
#include "lib/N/NRenderCell.h"
#include "NRenderable_Extract.h"

//#include "../../Utilities/NUT_Draw.h"


// There are 4 extract Functions base models:
// ------------------------------------------
//	NRenderableExtract_SingleGeometry_SingleInstance
//	NRenderableExtract_SingleGeometry_MultiInstances
//	NRenderableExtract_MultiGeometries_SingleInstance
//	NRenderableExtract_MultiGeometries_MultiInstances
// For the moment only 2 models are implemented, because there is no Instance management right now... but very soon
//
// On the top of this, there is different possible Culling methods, and 2 main Accumulator groups, 3D & HUD,
// which multiply the number of Extract functions...                                                                                                                                                                                                                                                                                                                               
//
//				NRENDERABLE_EXTRACT_3D_SINGLEGEOMETRY_SINGLEINSTANCE,
// NRENDERABLE_EXTRACT_3D_SINGLEGEOMETRY_MULTIINSTANCE,
//				NRENDERABLE_EXTRACT_3D_MULTIGEOMETRY_SINGLEINSTANCE,
// NRENDERABLE_EXTRACT_3D_MULTIGEOMETRY_MULTIINSTANCE,
// 
//				NRENDERABLE_EXTRACT_3D_SINGLEGEOMETRY_SINGLEINSTANCE_FRUSTUMCULLING,
// NRENDERABLE_EXTRACT_3D_SINGLEGEOMETRY_MULTIINSTANCE_FRUSTUMCULLING,
//				NRENDERABLE_EXTRACT_3D_MULTIGEOMETRY_SINGLEINSTANCE_FRUSTUMCULLING,
// NRENDERABLE_EXTRACT_3D_MULTIGEOMETRY_MULTIINSTANCE_FRUSTUMCULLING,
// 
//				NRENDERABLE_EXTRACT_HUD_SINGLEGEOMETRY_SINGLEINSTANCE,
// NRENDERABLE_EXTRACT_HUD_SINGLEGEOMETRY_MULTIINSTANCE,
//				NRENDERABLE_EXTRACT_HUD_MULTIGEOMETRY_SINGLEINSTANCE,
// NRENDERABLE_EXTRACT_HUD_MULTIGEOMETRY_MULTIINSTANCE,


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to 3D Accumulators
// +
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING
// +	NRENDERABLE_EXTRACT_3D_SINGLEINSTANCE_MULTIGEOMETRY_FRUSTUMCULLING

Nu32 NRenderableExtract_UI_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			sortingkey;

	if( NFrustumCulling_BSphere_vs_3DFrustum(&prenderable->BoundingSphere,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,((NUI*)powner)->pUITransformHNode, sortingkey );
		return 1;
	}
	else
	{
		return 0;
	}
}

Nu32 NRenderableExtract_UI_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			sortingkey;

	if( NFrustumCulling_BSphere_vs_3DFrustum(&prenderable->BoundingSphere,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		NGEOMETRY *pgeom = (NGEOMETRY*)prenderable->GeomArray.pFirst;
		for(Nu32 i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			NExtract3DRenderCell(pgeom, ((NUI*)powner)->pUITransformHNode,sortingkey );
		}
		return prenderable->GeomArray.Size;
	}
	else
	{
		return 0;
	}
}

Nu32 NRenderableExtract_UI_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	//NErrorIf(prenderable->GeomArray.pFirst == NULL, NERROR_RENDERABLE_EXTRACT)
	if( ((NUI*)powner)->pUITransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,((NUI*)powner)->pUITransformHNode, NGetZ_CameraBase(&v) );
	}
	else
	{
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,NULL, NGetZ_CameraBase(&prenderable->BoundingSphere.Center) );
	}
	return 1;
}

Nu32 NRenderableExtract_UI_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	Nu32		i;
	NGEOMETRY	*pgeom=(NGEOMETRY*)prenderable->GeomArray.pFirst;;
	if( prenderable->pTransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		//NExtractAll3DRenderCell(prenderable, NGetZ_CameraBase(&v)  );
		// ... replace by:
		for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			NExtract3DRenderCell(pgeom, ((NUI*)powner)->pUITransformHNode,NGetZ_CameraBase(&v) );
		}
	}
	else
	{
		// NExtractAll3DRenderCell(prenderable, NGetZ_CameraBase(&prenderable->BoundingSphere.Center)  );
		// ... replace by:
		for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			NExtract3DRenderCell(pgeom, ((NUI*)powner)->pUITransformHNode,NGetZ_CameraBase(&prenderable->BoundingSphere.Center) );
		}
	}
	return prenderable->GeomArray.Size;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to HUD Accumulator
// +
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_SINGLEGEOMETRY
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_MULTIGEOMETRY
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_MULTIGEOMETRY_UI_CHECKBOX

Nu32 NRenderableExtract_UI_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NExtractHUDRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,((NUI*)powner)->pUITransformHNode );
	return 1;
}

Nu32 NRenderableExtract_UI_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	// NExtractAllHUDRenderCell( prenderable );
	// ... replace by:
	NGEOMETRY *pgeom = (NGEOMETRY*)prenderable->GeomArray.pFirst;
	for(Nu32 i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
	{
		if( pgeom->Mesh.PrimitiveArray.Size )
		{
			NExtractHUDRenderCell(pgeom,((NUI*)powner)->pUITransformHNode);
/*
			NUT_DRAW	desc;
			NGetGeometryAABB(pgeom,&desc.AABB.AABB);
			NMulVector3ByMatrixO(&desc.AABB.AABB.VMin,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&desc.AABB.AABB.VMin);
			NMulVector3ByMatrixO(&desc.AABB.AABB.VMax,&((NUI*)powner)->pUITransformHNode->WorldMatrix,&desc.AABB.AABB.VMax);
			NSetColorf(&desc.AABB.Color,NCOLOR_PRESET3F_BLUE_ELECTRIC);
			desc.AABB.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_AABB);
			NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
*/
		}
	}
	return prenderable->GeomArray.Size;
}


