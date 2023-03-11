#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/NErrorHandling.h"
#include "NGeometryBoundingVolume.h"

extern NVERTEX_SPECS NVertexSpecs[];

void NGetGeometryAABB(const NGEOMETRY *pgeom, NAABB *paabb)
{
	// Need vertex position !!! Obviously, there is always ones, and always at the top of vertex structure, but anyway ...
	NErrorIf(!NCHECK_VERTEX_SPECS_POSITION(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags), NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT);
	if (!pgeom->Mesh.VertexArray.Size)
	{
		Nmem0(paabb, NAABB);
		return;
	}

	Nu32 vertexsize = pgeom->Mesh.VertexArray.ElementSize;
	NVEC3 *pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
	paabb->fXMin = pv->x;
	paabb->fYMin = pv->y;
	paabb->fZMin = pv->z;
	paabb->fXMax = pv->x;
	paabb->fYMax = pv->y;
	paabb->fZMax = pv->z;
	// move to the next one.
	pv = (NVEC3 *)((NBYTE *)pv + vertexsize);
	for (Nu32 i = pgeom->Mesh.VertexArray.Size - 1; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + vertexsize))
	{
		if (pv->x > paabb->fXMax)
			paabb->fXMax = pv->x;
		else if (pv->x < paabb->fXMin)
			paabb->fXMin = pv->x;

		if (pv->y > paabb->fYMax)
			paabb->fYMax = pv->y;
		else if (pv->y < paabb->fYMin)
			paabb->fYMin = pv->y;

		if (pv->z > paabb->fZMax)
			paabb->fZMax = pv->z;
		else if (pv->z < paabb->fZMin)
			paabb->fZMin = pv->z;
	}
}

void NGetGeometryArrayAABB(NAABB *paabb, const NARRAY *pgeomarray)
{
	Nu32 i, g;
	Nu32 vertexsize;
	NVEC3 *pv;
	Nbool aabbsetup = NFALSE;

	NErrorIf(pgeomarray->ElementSize != sizeof(NGEOMETRY), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NGEOMETRY *pgeom = (NGEOMETRY *)pgeomarray->pFirst;
	for (g = pgeomarray->Size; g != 0; g--, pgeom++)
	{
		// Need vertex position !!! Obviously, there is always ones, and always at the top of vertex structure, but anyway ...
		NErrorIf(!NCHECK_VERTEX_SPECS_POSITION(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags), NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT);
		vertexsize = pgeom->Mesh.VertexArray.ElementSize;
		pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		if (!aabbsetup && pgeom->Mesh.VertexArray.Size)
		{
			pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
			paabb->fXMin = pv->x;
			paabb->fYMin = pv->y;
			paabb->fZMin = pv->z;
			paabb->fXMax = pv->x;
			paabb->fYMax = pv->y;
			paabb->fZMax = pv->z;
			aabbsetup = NTRUE;
		}

		for (i = pgeom->Mesh.VertexArray.Size; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + vertexsize))
		{
			if (pv->x > paabb->fXMax)
				paabb->fXMax = pv->x;
			else if (pv->x < paabb->fXMin)
				paabb->fXMin = pv->x;

			if (pv->y > paabb->fYMax)
				paabb->fYMax = pv->y;
			else if (pv->y < paabb->fYMin)
				paabb->fYMin = pv->y;

			if (pv->z > paabb->fZMax)
				paabb->fZMax = pv->z;
			else if (pv->z < paabb->fZMin)
				paabb->fZMin = pv->z;
		}
	}
	if (!aabbsetup)
	{
		Nmem0(paabb, NAABB);
	}
}

/*
void NSetGeometryAABBSize( NGEOMETRY *pgeom,const NVEC3 *pnewsize,const NVEC3 *panchor )
{
	NAABB		aabb;
	NVEC3	size;
	NVEC3	center;
	NVEC3	scale;
	Nu32		i;
	NVERTEX		*pvertex;

	NGetGeometryAABB( pgeom,&aabb );
	NGetAABBSize(&size,&aabb);

	if( (size.x == pnewsize->x) && (size.y == pnewsize->y) && (size.z == pnewsize->z) )
		return;

	center.x	= aabb.fXMin + size.x*panchor->x;
	center.y	= aabb.fYMax - size.y*panchor->y;
	center.z	= aabb.fZMin + size.z*panchor->z;

	if(size.x)
		scale.x		= pnewsize->x / size.x;
	else
		scale.x		= 1.0f;

	if(size.y)
		scale.y		= pnewsize->y / size.y;
	else
		scale.y		= 1.0f;

	if(size.z)
		scale.z		= pnewsize->z / size.z;
	else
		scale.z		= 1.0f;

	pvertex = (NVERTEX*)NGetFirstArrayPtr(&pgeom->VertexArray);
	for( i=NGetArraySize(&pgeom->VertexArray);i!=0;i--,pvertex ++ )
	{
		pvertex->x = center.x + (pvertex->x - center.x)*scale.x;
		pvertex->y = center.y + (pvertex->y - center.y)*scale.y;
		pvertex->z = center.z + (pvertex->z - center.z)*scale.z;
	}
}
*/

// ------------------------------------------------------------------------------------------
// NGetBoundingSphereFromGeometryArray
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NGetBoundingSphereFromGeometryArray(NBOUNDINGSPHERE *pbs, const NARRAY *pgeometryarray)
{
	NErrorIf(pgeometryarray->ElementSize != sizeof(NGEOMETRY), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	Nf32 max_dist = 0;
	Nf32 f;
	Nu32 nb = 0;
	Nu32 i, j;
	Nu32 next_vertex;
	NVEC3 v;
	NVEC3 *pv;
	NGEOMETRY *pgeom = (NGEOMETRY *)pgeometryarray->pFirst;

	// Find Center:
	NVec3Set(&v, 0.0f, 0.0f, 0.0f);
	for (i = pgeometryarray->Size; i != 0; i--, pgeom++)
	{
		// Need vertex position !!! Obviously, there is always ones, and always at the top of vertex structure, but anyway ...
		NErrorIf(!NCHECK_VERTEX_SPECS_POSITION(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags), NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT);

		pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		next_vertex = pgeom->Mesh.VertexArray.ElementSize;

		for (j = pgeom->Mesh.VertexArray.Size; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + next_vertex))
		{
			NVec3AddTo(&v, (NVEC3 *)pv);
		}
		nb += pgeom->Mesh.VertexArray.Size;
	}

	// Continue and Find Radius (... or STOP )
	if (nb)
	{
		NVec3Scale(&pbs->Center, &v, 1.0f / (Nf32)nb);

		pgeom = (NGEOMETRY *)pgeometryarray->pFirst;
		for (i = pgeometryarray->Size; i != 0; i--, pgeom++)
		{
			pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
			next_vertex = pgeom->Mesh.VertexArray.ElementSize;

			for (j = pgeom->Mesh.VertexArray.Size; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + next_vertex))
			{
				NVec3Sub(&v, pv, &pbs->Center);
				f = NVec3SquareLength(&v);
				if (f > max_dist)
				{
					max_dist = f;
				}
			}
		}
		pbs->fRadius = sqrt(max_dist);
	}
	else
	{
		pbs->Center = v; // with v = 0,0,0
		pbs->fRadius = 0.0f;
	}
}

// ------------------------------------------------------------------------------------------
// NGetBoundingSphereCenterFromGeometryArray
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NGetBoundingSphereCenterFromGeometryArray(NVEC3 *pcenter, const NARRAY *pgeometryarray)
{
	NErrorIf(pgeometryarray->ElementSize != sizeof(NGEOMETRY), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	//	Nf32		max_dist=0;
	Nu32 nb = 0;
	Nu32 i, j;
	NVEC3 v;
	NVEC3 *pv;
	NGEOMETRY *pgeom = (NGEOMETRY *)pgeometryarray->pFirst;
	Nu32 stride;

	// Find Center:
	NVec3Set(&v, 0.0f, 0.0f, 0.0f);
	for (i = pgeometryarray->Size; i != 0; i--, pgeom++)
	{
		pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		stride = pgeom->Mesh.VertexArray.ElementSize;
		for (j = pgeom->Mesh.VertexArray.Size; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			NVec3AddTo(&v, pv);
		}
		nb += pgeom->Mesh.VertexArray.Size;
	}

	if (nb)
	{
		NVec3Scale(pcenter, &v, 1.0f / (Nf32)nb);
	}
	else
	{
		*pcenter = v; // with v = 0,0,0
	}
}

// ------------------------------------------------------------------------------------------
// NGetBoundingSphereFromGeometry
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NGetBoundingSphereFromGeometry(NBOUNDINGSPHERE *pbs, const NGEOMETRY *pgeom)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);

	Nf32 max_dist = 0;
	Nf32 f;
	Nu32 j;
	NVEC3 v;

	Nu32 nb = pgeom->Mesh.VertexArray.Size;

	// Find Center:
	NVec3Set(&v, 0.0f, 0.0f, 0.0f);
	if (nb)
	{
		NVEC3 *pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		Nu32 stride = pgeom->Mesh.VertexArray.ElementSize;
		for (j = nb; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			NVec3AddTo(&v, (NVEC3 *)pv);
		}
		NVec3Scale(&pbs->Center, &v, 1.0f / (Nf32)nb);

		// ... And Radius
		pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		for (j = nb; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			NVec3Sub(&v, pv, &pbs->Center);
			f = NVec3SquareLength(&v);
			if (f > max_dist)
			{
				max_dist = f;
			}
		}
		pbs->fRadius = sqrt(max_dist);
	}
	else
	{
		pbs->Center = v; // with v = 0,0,0
		pbs->fRadius = 0.0f;
	}
}

// ------------------------------------------------------------------------------------------
// NGetBoundingSphereCenterFromGeometry
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NGetBoundingSphereCenterFromGeometry(NVEC3 *pcenter, const NGEOMETRY *pgeom)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);

	Nu32 nb = pgeom->Mesh.VertexArray.Size;
	NVEC3 v;

	// Find Center:
	NVec3Set(&v, 0.0f, 0.0f, 0.0f);
	if (nb)
	{
		NVEC3 *pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		Nu32 stride = pgeom->Mesh.VertexArray.ElementSize;

		for (Nu32 j = nb; j != 0; j--, pv = (NVEC3 *)((NBYTE *)pv + stride))
		{
			NVec3AddTo(&v, pv);
		}
		NVec3Scale(pcenter, &v, 1.0f / (Nf32)nb);
	}
	else
	{
		*pcenter = v; // with v = 0,0,0
	}
}