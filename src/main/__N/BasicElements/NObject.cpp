#include "../NCStandard.h"
#include "../GL/Ngl.h"

#include "../Geometry/NGeometry.h"
#include "../NObject.h"
#include "../Containers/NNode.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "../Render/NFrustum_Culling.h"
//extern NNODELIST NRenderableEntitiesList[NRENDERABLE_LISTSIZE];

//NNODELIST	Active3DObjectsList;

// ------------------------------------------------------------------------------------------
// NCreate3DObject
// ------------------------------------------------------------------------------------------
// Description :
//	Create a 3D Object. 
//	!!! without any geometry inside!!!
//	!!! User has to insert them after ...
// ------------------------------------------------------------------------------------------
// In  :
//		geometry_prealloacated_capacity:	Number of geometry preallocated (not created) 
//											by the included Renderable
// Out :
//		ptr on the new object
// ------------------------------------------------------------------------------------------
NOBJECT* NCreate3DObject(const NOBJECT_DESC *pdesc)
{
	return NSetup3DObject(NEW(NOBJECT),pdesc);
}

NOBJECT* NSetup3DObject(NOBJECT *pobj, const NOBJECT_DESC *pdesc)
{
	NRENDERABLE_DESC	desc;
	Nmem0(&desc,NRENDERABLE_DESC);
	
	if(pdesc && ISFLAG_ON(pdesc->Flags,FLAG_NOBJECT_DESC_USEPARAM_BOUNDINGSPHERE))
	{
		desc.BoundingSphere	= pdesc->BoundingSphere;
	}
	else
	{
		desc.BoundingSphere.fRadius	= DEFAULT_NOBJECT_RENDERABLE_BOUNDINGSPHERE_RADIUS;
		NVec3Set(&desc.BoundingSphere.Center, DEFAULT_NOBJECT_RENDERABLE_BOUNDINGSPHERE_CENTER_3F);
	}
	if(pdesc && ISFLAG_ON(pdesc->Flags,FLAG_NOBJECT_DESC_USEPARAM_TRANSFORMHNODE))
	{
		desc.pTransformHNode = pdesc->pTransformHNode;
	}
	else
	{
		desc.pTransformHNode = NULL;
	}
	if(pdesc && ISFLAG_ON(pdesc->Flags,FLAG_NOBJECT_DESC_USEPARAM_INCLUDED_GEOMETRY_PREALLOCATED_CAPACITY))
	{
		desc.IncludedGeometryPreallocatedCapacity = pdesc->IncludedGeometryPreallocatedCapacity;
	}
	else
	{
		desc.IncludedGeometryPreallocatedCapacity = 0;
	}
	NEXTRACTSET	highlevelextractset;
	if(pdesc && ISFLAG_ON(pdesc->Flags,FLAG_NOBJECT_DESC_USEPARAM_HIGHLEVELEXTRACTSET))
	{
		highlevelextractset = pdesc->HighLevelExtractSet;
	}
	else
	{
		highlevelextractset = DEFAULT_NOBJECT_HIGHLEVEL_EXTRACTSET;
	}

	NEXTRACTSET	lowlevelextractset;
	switch(desc.IncludedGeometryPreallocatedCapacity)
	{
		case 0:
			lowlevelextractset = NEXTRACTSET_SECTION_VOID;	// Associated ExtractSet Function will extract nothing !
			// NGetRenderableExtractFunctionByExtractSet( NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,0) ); could works too 
			// but NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,0) -> NRENDERABLE_EXTRACT_FCT_VOID has to be defined... let's see
			break;

		case 1:
			lowlevelextractset = NEXTRACTSET_SECTION_STANDARD;	// Associated ExtractSet Function will extract the unique geometry !
			break;

		default:
			lowlevelextractset = (NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES); 	// Associated ExtractSet Function will extract all the array geometries !
			break;
	}

	desc.Update_FCT								= NRenderableUpdate_Basic;
	desc.Extract_FCT							= NGetRenderableExtractFunctionByExtractSet( NMAKE_NEXTRACTSET_2P(lowlevelextractset,highlevelextractset) );
	pobj->pRenderable							= NCreateRenderable(&desc);

	return pobj;
}

void NClear3DObject(void* pobj)
{
	NDeleteRenderable(((NOBJECT*)pobj)->pRenderable);
}

// ------------------------------------------------------------------------------------------
// NDelete3DObject
// ------------------------------------------------------------------------------------------
// Description :
//	Delete a 3D Object
// ------------------------------------------------------------------------------------------
// In  :
//		pobj: ptr on the Object to delete
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
void NDelete3DObject(void* pobj)
{
	NClear3DObject(pobj);
	Nfree(pobj);
}
