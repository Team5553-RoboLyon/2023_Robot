#ifndef __NRENDERABLE_H
#define __NRENDERABLE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NRenderable.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/NTransformationTree.h"
#include "lib/N/NBoundingVolumes.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **								Structures											**
	// ***************************************************************************************
	/*

	Project .....

	typedef struct
	{
		NRENDERABLE_CORE			*pNNode_Previous;
		NRENDERABLE_CORE			*pNNode_Next;

		NRENDERABLE_UPDATE_FCT		Update_FCT;
		NRENDERABLE_EXTRACT_FCT		Extract_FCT;
		Nu32						Type;
	}NRENDERABLE_CORE or NRNODE;

	typedef struct
	{
		NRENDERABLE_CORE			Core;
		NRENDERABLE_CORE			*pNHNodeList_Parent;
		NRENDERABLE_CORE			*pNHNodeList_First;
		NRENDERABLE_CORE			*pNHNodeList_Last;

	}NRENDERABLE_CORE_H or NRHNODE;

	typedef struct
	{
		NRENDERABLE_CORE			Core;

		NARRAY						GeomArray;
		NBOUNDINGSPHERE				BoundingSphere;
		NTRANSFORM_HNODE			*pTransformHNode;	// Unique for a while. Will be replace by instance list
	}NRENDERABLE;

	typedef struct
	{
		NRENDERABLE_CORE			Core;

		NARRAY						GeomArray;
		NBOUNDINGSPHERE				BoundingSphere;
		NARRAY						TransformHNodeArray;
	}NRENDERABLE_INSTANCE;

	typedef struct
	{
		NRENDERABLE_CORE_H			CoreH;

		//

	}NRENDERABLE_POOL;

	*/

	typedef struct NRENDERABLE NRENDERABLE;
	typedef Nu32 (*NRENDERABLE_EXTRACT_FCT)(const NRENDERABLE *pr, const void *powner);
	typedef void (*NRENDERABLE_UPDATE_FCT)(NRENDERABLE *pr, void *powner, const NTIME *ptime);
	struct NRENDERABLE
	{
		NARRAY GeomArray;
		NTRANSFORM_HNODE *pTransformHNode; // Unique for a while. Will be replace by instance list
		NBOUNDINGSPHERE BoundingSphere;
		NRENDERABLE_UPDATE_FCT Update_FCT;
		NRENDERABLE_EXTRACT_FCT Extract_FCT;
	};
	typedef struct
	{
		Nu32 IncludedGeometryPreallocatedCapacity;
		NTRANSFORM_HNODE *pTransformHNode;
		NBOUNDINGSPHERE BoundingSphere;
		NRENDERABLE_UPDATE_FCT Update_FCT;
		NRENDERABLE_EXTRACT_FCT Extract_FCT;
	} NRENDERABLE_DESC;
	/*
	// THE FUTURE of NRENDERABLE ....
	struct NINSTANCE
	{
		NTRANSFORM_HNODE			*pTransformHNode;
		Nf32						SortingKey;
	};
	struct NRENDERABLE
	{
		NRENDERABLE_UPDATE_FCT		Update_FCT;
		NRENDERABLE_EXTRACT_FCT		Cull_FCT;
		NEXTRACT_FCT				Extract_FCT;
		NARRAY						GeomArray;
		NBOUNDINGSPHERE				BoundingSphere;
		void						*pInstanceList;
		Nu32						InstanceNb;
	};
	*/
	/*
	// THE PAST of NRENDERABLE ....
	typedef struct NRENDERABLE NRENDERABLE;
	typedef void (*NUPDATE_AND_EXTRACT_FCT)( NRENDERABLE* pr, void* powner, const NTIME *ptime );
	struct NRENDERABLE
	{
		NTRANSFORM_HNODE		*pTransformHNode;
		//---------------------------------------	//	|
		NBOUNDING_VOLUME		BoundingVolume;		//	|---> Is going to be replaced by a unique pointer
		NARRAY					GeomArray;			//	|---> on a NMESH structure ! By the way, NRENDERABLE is going to evolve as an NINSTANCE !
		//---------------------------------------	//	|
		NUPDATE_AND_EXTRACT_FCT	UpdateAndExtract_FCT;
		//NCULL_AND_SORT_FCT	CullAndSort_FCT;	//	|
		//Nf32					SortingKey;			//	|---> TODO ! Also replace BoundingVolume by Bounding Sphere !!!!
	};
	typedef struct
	{
		NTRANSFORM_HNODE		*pTransformHNode;
		NBOUNDING_VOLUME		BoundingVolume;
		Nu32					IncludedGeometryPreallocatedCapacity;
		NUPDATE_AND_EXTRACT_FCT	UpdateAndExtract_FCT;
	}NRENDERABLE_DESC;
	*/
	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	NRENDERABLE *NSetupRenderable(NRENDERABLE *pr, const NRENDERABLE_DESC *pdesc);
	NRENDERABLE *NCreateRenderable(const NRENDERABLE_DESC *pdesc);
	void NClearRenderable(NRENDERABLE *pr);
	void NDeleteRenderable(NRENDERABLE *pr);

#define FLAG_NCOPYRENDERABLE_DONTCOPY_TRANSFORMHNODE BIT_0
	NRENDERABLE *NCopyRenderable(NRENDERABLE *pdest, const NRENDERABLE *psrc, const Nu32 flags);

#define FLAG_NDUPLICATERENDERABLE_DONTDUPLICATE_TRANSFORMHNODE BIT_0
	NRENDERABLE *NDuplicateRenderable(const NRENDERABLE *psrc, const Nu32 flags);

	inline void NSetRenderableTransformHNode(NRENDERABLE *pr, NTRANSFORM_HNODE *pthn)
	{
		if (pr->pTransformHNode)
		{
			NUnregisterTransformHNodeReference(pr->pTransformHNode);
		}
		if (pthn)
		{
			NRegisterTransformHNodeReference(pthn);
		}
		pr->pTransformHNode = pthn;
	}
	inline void NSetRenderableUpdateFCT(NRENDERABLE *pr, const NRENDERABLE_UPDATE_FCT pfct) { pr->Update_FCT = pfct; }
	inline void NSetRenderableExtractFCT(NRENDERABLE *pr, const NRENDERABLE_EXTRACT_FCT pfct) { pr->Extract_FCT = pfct; }

	// ***************************************************************************************
	// **							Included Geometries Functions							**
	// ***************************************************************************************
	inline Nu32 NGetIncludedGeometriesNumber(const NRENDERABLE *pr) { return pr->GeomArray.Size; }
	inline NGEOMETRY *NGetFirstIncludedGeometry(const NRENDERABLE *pr)
	{
		NErrorIf(!pr->GeomArray.Size, NERROR_ARRAY_ELEMENT_NOT_FOUND);
		return (NGEOMETRY *)pr->GeomArray.pFirst; /* *(NGEOMETRY*)NGetFirstArrayPtr(&pr->GeomArray)*/
		;
	}
	inline NGEOMETRY *NGetIncludedGeometryByIndex(const NRENDERABLE *pr, const Nu32 index)
	{
		NErrorIf(index >= pr->GeomArray.Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);
		return (NGEOMETRY *)NGetArrayPtr(&pr->GeomArray, index);
	}
	Nu32 NGetIncludedGeometryIndex(const NRENDERABLE *pr, const NGEOMETRY *pgeom);

	// replace NRenderableGeometryPushBack
	inline NGEOMETRY *NGeometryEmplaceBack(NRENDERABLE *pr, NGEOMETRY_DESC *pgdesc)
	{
		NUpSizeArray(&pr->GeomArray, 1, NULL);
		return NSetupGeometry((NGEOMETRY *)NGetLastArrayPtr(&pr->GeomArray), pgdesc);
	}
	inline NGEOMETRY *NGeometryPushBack(NRENDERABLE *pr, NGEOMETRY *pgeom) { return (NGEOMETRY *)NArrayPushBack(&pr->GeomArray, (const NBYTE *)pgeom); }

	// Replace NRenderableGeometryIncluded
	Nbool NIsGeometryIncluded(const NRENDERABLE *pr, const NGEOMETRY *pgeom);
	void NDeleteIncludedGeometry(NRENDERABLE *pr, NGEOMETRY *pgeom);
	void NDeleteIncludedGeometryByIndex(NRENDERABLE *pr, const Nu32 index);
	// Replace void NEraseRenderableGeometryArray(NRENDERABLE *pr);
	void NDeleteAllIncludedGeometries(NRENDERABLE *pr);
// Replace NGetRenderableGeometryByTexture
// NGEOMETRY*		NGetBoundGeometryByTexture( const NRENDERABLE* pr, const NTEXTURE *ptext );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NRENDERABLE_H
