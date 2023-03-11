#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCore.h"

#include "NRenderable.h"
//extern	Nbool			bRendering;

static inline void _delete_included_geometry(void *pgeom){NClearGeometry(pgeom);}
// ------------------------------------------------------------------------------------------
// NSetUpRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
NRENDERABLE* NSetupRenderable( NRENDERABLE *pr, const NRENDERABLE_DESC *pdesc )
{
	NSetupArray(&pr->GeomArray,pdesc->IncludedGeometryPreallocatedCapacity, sizeof(NGEOMETRY) );

	pr->BoundingSphere	= pdesc->BoundingSphere;
	pr->Update_FCT		= pdesc->Update_FCT;
	pr->Extract_FCT		= pdesc->Extract_FCT;
	
	if( pdesc->pTransformHNode )
	{
		pr->pTransformHNode	= pdesc->pTransformHNode;
		NRegisterTransformHNodeReference(pr->pTransformHNode);
	}
	else
	{
		pr->pTransformHNode = NULL;
	}
	return pr;
}
// ------------------------------------------------------------------------------------------
// NCreateRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
NRENDERABLE	*NCreateRenderable(const NRENDERABLE_DESC *pdesc)
{
	return NSetupRenderable(NEW(NRENDERABLE),pdesc);
}
// ------------------------------------------------------------------------------------------
// NClearRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearRenderable( NRENDERABLE *pr )
{
	if(pr->pTransformHNode)
	{
		NUnregisterTransformHNodeReference(pr->pTransformHNode);
	}

	NClearArray( &pr->GeomArray, _delete_included_geometry );
	Nmem0(pr,NRENDERABLE);
}
// ------------------------------------------------------------------------------------------
// NDeleteRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteRenderable(NRENDERABLE *pr)
{
	NClearRenderable(pr);
	Nfree(pr);
}
// ------------------------------------------------------------------------------------------
// NCopyRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
NRENDERABLE* NCopyRenderable(NRENDERABLE *pdest, const NRENDERABLE *psrc, const Nu32 flags)
{
	NErrorIf(!pdest || !psrc, NERROR_NULL_POINTER);
	
	NRENDERABLE_DESC	rdesc;

	if( ISFLAG_ON(flags,FLAG_NCOPYRENDERABLE_DONTCOPY_TRANSFORMHNODE) )	
	{
		rdesc.pTransformHNode = pdest->pTransformHNode;
	}
	else
	{
		rdesc.pTransformHNode = psrc->pTransformHNode;
	}
	rdesc.BoundingSphere						= psrc->BoundingSphere;
	rdesc.Update_FCT							= psrc->Update_FCT;
	rdesc.Extract_FCT							= psrc->Extract_FCT;
	rdesc.IncludedGeometryPreallocatedCapacity	= psrc->GeomArray.Capacity;
	NClearRenderable(pdest);	// it will manage Included geometry deleting , Thn ref/Unref, ... , all of them in the right way .
	NSetupRenderable(pdest,&rdesc);

	// Geometries
	NGEOMETRY	geom;
	NGEOMETRY	*pgeom = (NGEOMETRY*)psrc->GeomArray.pFirst;
	for(Nu32 i=psrc->GeomArray.Size;i!=0;i--,pgeom++)
	{	
		Nmem0(&geom,NGEOMETRY);
		NGeometryPushBack( pdest, NCopyGeometry(&geom,pgeom) );
	}
	return pdest;
}
// ------------------------------------------------------------------------------------------
// NDuplicateRenderable
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
NRENDERABLE* NDuplicateRenderable(const NRENDERABLE *psrc, const Nu32 flags)
{
	NErrorIf(!psrc, NERROR_NULL_POINTER);

	NRENDERABLE_DESC	rdesc;
	if( ISFLAG_ON(flags,FLAG_NDUPLICATERENDERABLE_DONTDUPLICATE_TRANSFORMHNODE) )	
	{
		rdesc.pTransformHNode = NULL;
	}
	else
	{
		rdesc.pTransformHNode = psrc->pTransformHNode;
	}
	rdesc.BoundingSphere	= psrc->BoundingSphere;
	rdesc.Update_FCT		= psrc->Update_FCT;
	rdesc.Extract_FCT		= psrc->Extract_FCT;
	rdesc.IncludedGeometryPreallocatedCapacity	= psrc->GeomArray.Capacity;
	NRENDERABLE *pdest = NSetupRenderable(NEW(NRENDERABLE),&rdesc); // it will manageThn ref/Unref in the right way .
	
	// Geometries
	NGEOMETRY	geom;
	NGEOMETRY	*pgeom = (NGEOMETRY*)psrc->GeomArray.pFirst;
	for(Nu32 i=psrc->GeomArray.Size;i!=0;i--,pgeom++)
	{	
		Nmem0(&geom,NGEOMETRY);
		NGeometryPushBack( pdest, NCopyGeometry(&geom,pgeom) );
	}
	return pdest;
}

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// RENDERABLE & INCLUDED GEOMETRIES 
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
void NDeleteIncludedGeometry(NRENDERABLE *pr, NGEOMETRY *pgeom)
{
/*
	NGEOMETRY	*pgeomcheck = (NGEOMETRY*)pr->GeomArray.pFirst;
	for(Nu32 i=pr->GeomArray.Size;i!=0;i--,pgeomcheck++)
	{
		if(pgeomcheck == pgeom)
		{
			NEraseArrayElementPtr(&pr->GeomArray,(NBYTE*)pgeom,_delete_included_geometry);
			return;
		}
	}
*/
	NEraseArrayElementPtr(&pr->GeomArray,(NBYTE*)pgeom,_delete_included_geometry);
}
void NDeleteIncludedGeometryByIndex(NRENDERABLE *pr, const Nu32 index)
{
	NEraseArrayElement(&pr->GeomArray,index,_delete_included_geometry);
}

void	NDeleteAllIncludedGeometries(NRENDERABLE *pr)
{
	NEraseArray( &pr->GeomArray,_delete_included_geometry );
}

Nbool	NIsGeometryIncluded(const NRENDERABLE *pr,const NGEOMETRY *pgeom)
{
	NGEOMETRY *pgeomcheck = (NGEOMETRY*)pr->GeomArray.pFirst;
	for(Nu32 i=pr->GeomArray.Size;i!=0;i--,pgeomcheck++)
	{
		if(pgeomcheck == pgeom)
			return NTRUE;
	}
	return NFALSE;
}

Nu32 NGetIncludedGeometryIndex(const NRENDERABLE *pr,const NGEOMETRY *pgeom)
{
	NGEOMETRY *plookup = (NGEOMETRY*)pr->GeomArray.pFirst;
	for(Nu32 i=0;i<pr->GeomArray.Size;i++,plookup++)
	{
		if(plookup == pgeom)
			return i;
	}
	return NVOID;
}

