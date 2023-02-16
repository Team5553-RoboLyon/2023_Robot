#include "../NCStandard.h"
#include "../Containers/NArray.h"
#include "../NErrorHandling.h"
#include "../Geometry/NGeometryMisc.h"

#include "NUT_geometry.h"

void NUT_CloneGeometryContent(NGEOMETRY *pgeom, const NVEC3 *pvspacing, const Nu32 along_xaxis, const Nu32 along_yaxis, const Nu32 along_zaxis )
{
	Nu32		i,j,k;
	Nu16		tri;
	Nu32		clone_index,total_clones;
	Nu32		vertex_index_shifting;
	Nu32		vert;
	NBYTE		*pfirstvertex,*ptargetvertex;
	NBYTE		*pfirstprim,*ptargetprim;
	NVEC3	vshift,vshift_one;
	NTRIANGLE	*ptri;
	NVEC3	*pv;

	// keep in mind current geometry content size (vertex and primitive)
	Nu32 nbvertex	= pgeom->Mesh.VertexArray.Size;
	Nu32 nbprim		= pgeom->Mesh.PrimitiveArray.Size;
	Nu32 vertex_size= pgeom->Mesh.VertexArray.ElementSize;
	Nu32 prim_size	= pgeom->Mesh.PrimitiveArray.ElementSize;

	// The total request number of copies (first one included) is ...
	NErrorIf( !nbprim, NERROR_GEOMETRY_NOT_ENOUGH_PRIMITIVE );
	NErrorIf( !nbvertex, NERROR_GEOMETRY_NOT_ENOUGH_VERTEX);
	total_clones = along_xaxis*along_yaxis*along_zaxis;
	NErrorIf( total_clones <= 1, NERROR_GEOMETRY_TOTAL_CLONES_NUMBER_HAS_TO_BE_GREATER_THAN_ONE );
	// else ...
	// First step: vshift_one & memory allocation 
	if(pvspacing)
	{
		vshift_one = *pvspacing;
	}
	else
	{
		vshift_one.x = vshift_one.y = vshift_one.z = 0.0f;
	}

	if( nbvertex )
		NSetArrayCapacity(&pgeom->Mesh.VertexArray,nbvertex*total_clones,NULL);
	if( nbprim )
		NSetArrayCapacity(&pgeom->Mesh.PrimitiveArray,nbprim*total_clones,NULL);
	
	pfirstvertex	= ptargetvertex = (NBYTE*)pgeom->Mesh.VertexArray.pFirst;
	pfirstprim		= ptargetprim	= (NBYTE*)pgeom->Mesh.PrimitiveArray.pFirst;

	vshift.x = vshift.y = vshift.z = 0.0f;
	for( clone_index =1; clone_index < total_clones; clone_index++ )// We start at 1 because clone 0 already exists !
	{																// this is the original which is going to be duplicated
		// Copy
		ptargetvertex += nbvertex*vertex_size;
		memcpy(ptargetvertex,	pfirstvertex,	nbvertex*vertex_size);
		pgeom->Mesh.VertexArray.Size += nbvertex;
		
		ptargetprim += nbprim*prim_size;
		memcpy(ptargetprim,	pfirstprim,	nbprim*prim_size);
		pgeom->Mesh.PrimitiveArray.Size += nbprim;

		// Reassign triangle index
		ptri = (NTRIANGLE*)ptargetprim;
		vertex_index_shifting = nbvertex*clone_index;
		for(tri=0;tri<nbprim;tri++)
		{
			ptri[tri].i0 += vertex_index_shifting;
			ptri[tri].i1 += vertex_index_shifting;
			ptri[tri].i2 += vertex_index_shifting;
		}

		// Shift vertex coordinates!
		i = clone_index%along_xaxis;
		j = (clone_index/along_xaxis)%along_yaxis;
		k =	clone_index/(along_xaxis*along_yaxis);
		
		vshift.x = (Nf32)i*vshift_one.x;
		vshift.y = (Nf32)j*vshift_one.y;
		vshift.z = (Nf32)k*vshift_one.z;
		
		pv = (NVEC3*)ptargetvertex;
		for(vert=0; vert<nbvertex; vert++, pv=(NVEC3*)((NBYTE*)pv+vertex_size))
		{
			pv->x+=vshift.x;
			pv->y+=vshift.y;
			pv->z+=vshift.z;
		}
	}
}
