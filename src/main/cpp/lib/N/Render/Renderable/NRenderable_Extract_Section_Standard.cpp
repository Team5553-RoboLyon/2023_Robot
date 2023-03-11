#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCore.h"
#include "../../NCamera.h"

#include "../NFrustum_Culling.h"
#include "lib/N/NRenderCell.h"
#include "NRenderable_Extract.h"

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

Nu32 NRenderableExtract_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			sortingkey;

	if( NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,prenderable->pTransformHNode, sortingkey );
		return 1;
	}
	else
	{
		return 0;
	}
}

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			sortingkey;

	if( NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		NExtractAll3DRenderCell(prenderable, sortingkey );
		return 1;
	}
	else
	{
		return 0;
	}
}

Nu32 NRenderableExtract_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	//NErrorIf(prenderable->GeomArray.pFirst == NULL, NERROR_RENDERABLE_EXTRACT)
	if( prenderable->pTransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,prenderable->pTransformHNode, NGetZ_CameraBase(&v) );
	}
	else
	{
		NExtract3DRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,NULL, NGetZ_CameraBase(&prenderable->BoundingSphere.Center) );
	}
	return 1;
}

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	if( prenderable->pTransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NExtractAll3DRenderCell(prenderable, NGetZ_CameraBase(&v)  );
	}
	else
	{
		NExtractAll3DRenderCell(prenderable, NGetZ_CameraBase(&prenderable->BoundingSphere.Center)  );
	}
	return 1;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Extract to HUD Accumulator
// +
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_SINGLEGEOMETRY
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_MULTIGEOMETRY
// +	NRENDERABLE_EXTRACT_HUD_SINGLEINSTANCE_MULTIGEOMETRY_UI_CHECKBOX

Nu32 NRenderableExtract_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner)
{
	NExtractHUDRenderCell( (NGEOMETRY*)prenderable->GeomArray.pFirst,prenderable->pTransformHNode );
	return 1;
}

Nu32 NRenderableExtract_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner)
{
	NExtractAllHUDRenderCell( prenderable );
	return 1;
}


Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomOrder(const NRENDERABLE *prenderable, const void *powner)
{
	if( prenderable->pTransformHNode )
	{
		NVEC3	v;
		NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NExtractAllOrdered3DRenderCell(prenderable, NGetZ_CameraBase(&v)  );
	}
	else
	{
		NExtractAllOrdered3DRenderCell(prenderable, NGetZ_CameraBase(&prenderable->BoundingSphere.Center)  );
	}
	return 1;
}

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomZ(const NRENDERABLE *prenderable, const void *powner)
{
	NVEC3	v;
	Nu32		i;
	NGEOMETRY *pgeom = (NGEOMETRY*)prenderable->GeomArray.pFirst;
	if( prenderable->pTransformHNode )
	{
		for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,(NVEC3*)pgeom->Mesh.VertexArray.pFirst);
			NExtract3DRenderCell(pgeom, prenderable->pTransformHNode,NGetZ_CameraBase(&v) );
		}
	}
	else
	{
		for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
		{
			NExtract3DRenderCell(pgeom, prenderable->pTransformHNode,NGetZ_CameraBase((NVEC3*)pgeom->Mesh.VertexArray.pFirst) );
		}
	}
	return 1;
}

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomOrder(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			sortingkey;

	if( NRenderableCulling_vs_3DFrustum(prenderable,&sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		NExtractAllOrdered3DRenderCell(prenderable, sortingkey );
		return 1;
	}
	else
	{
		return 0;
	}
}

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomZ(const NRENDERABLE *prenderable, const void *powner)
{
	Nf32			unused_sortingkey; // "unused" ... by the geometries because they recalculate their own sorting key 
									   // according with their first vertex position instead of using the unique sorting key based
									   // on the Renderable boundingSphere center.

	if( NRenderableCulling_vs_3DFrustum(prenderable,&unused_sortingkey) != NFRUSTUM_CULLING_OUTSIDE )
	{
		Nu32		i;
		NVEC3	v;
		NGEOMETRY *pgeom = (NGEOMETRY*)prenderable->GeomArray.pFirst;
		if( prenderable->pTransformHNode )
		{
			for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
			{
				NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,(NVEC3*)pgeom->Mesh.VertexArray.pFirst);
				NExtract3DRenderCell(pgeom, prenderable->pTransformHNode,NGetZ_CameraBase(&v) );
			}
		}
		else
		{
			for(i=prenderable->GeomArray.Size;i!=0;i--,pgeom++)
			{
				NExtract3DRenderCell(pgeom, prenderable->pTransformHNode,NGetZ_CameraBase((NVEC3*)pgeom->Mesh.VertexArray.pFirst) );
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}