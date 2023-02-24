#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "../Geometry/Components/NGeometryAppearance.h"
#include "lib/N/Geometry/NGeometry.h"

// #include "../Render/Renderable/NRenderable_UpdateAndExtract.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/NErrorHandling.h"
#include "../Render/NFrustum_Culling.h"
#include "../Render/Renderable/NRenderable_Update.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "../Utilities/NUT_Shape.h"
#include "lib/N/Collectibles/NCollectibles.h"

static void NGetCollectibleElementPosition_DefaultFCT(NVEC3 *pvpos, const NCOLLECTIBLE *pcollectible, const Nu32 i)
{
	NVec3Set(pvpos, (Nf32)i, 0, 0);
}

static Nbool NCollectibleElementPicking_DefaultCallback(const NCOLLECTIBLE_PICKER *ppicker, const NCOLLECTIBLE *pcollectible, const Nu32 i, NVEC3 *pvposition)
{
	return NTRUE;
}

// ------------------------------------------------------------------------------------------
// NSetUpCollectible
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NCOLLECTIBLE *NSetupCollectible(NCOLLECTIBLE *pcollec, const NCOLLECTIBLE_DESC *pdesc)
{
	NRENDERABLE_DESC desc;
	NGEOMETRY_DESC geomdesc;
	NGEOMETRY *pgeom;
	Nu32 i;
	NUT_QUAD_DESC quad_desc;
	NCOLLECTIBLEVERTEX *pvertex;

	NErrorIf(!pdesc || !pcollec, NERROR_NULL_POINTER);
	NErrorIf(!pdesc->ElementCount, NERROR_NULL_VALUE); // A collectible needs to have, at least, one element inside.

	Nmem0(pcollec, NCOLLECTIBLE);

	pcollec->BindID = 0xFFFF; // Not Inserted in Collectible Set

	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_COUNT))
	{
		pcollec->ElementCount = pdesc->ElementCount;
	}
	else
	{
		pcollec->ElementCount = DEFAULT_NCOLLECTIBLE_ELEMENT_COUNT;
	}

	// ----------------------------------------------------------------------------------------------------------------------
	// Renderable Creation
	memset(&desc, 0, sizeof(NRENDERABLE_DESC));
	desc.IncludedGeometryPreallocatedCapacity = 1;
	NEXTRACTSET highlevelextractset = 0;
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USE_HIGHLEVEL_EXTRACTSET))
	{
		highlevelextractset = pdesc->HighLevelExtractSet;
	}
	else
	{
		highlevelextractset = DEFAULT_NCOLLECTIBLE_HIGHLEVEL_EXTRACTSET;
	}
	NErrorIf(NGET_EXTRACTSET_LOWLEVEL(highlevelextractset), NERROR_RENDERABLE_EXTRACTSET_UNAUTHORIZED_FLAGS_USING);
	desc.BoundingSphere.fRadius = DEFAULT_NCOLLECTIBLE_RENDERABLE_BOUNDINGSPHERE_RADIUS;
	desc.Update_FCT = NRenderableUpdate_Basic;
	desc.Extract_FCT = NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(CONSTANT_NCOLLECTIBLE_LOWLEVEL_EXTRACTSET, highlevelextractset));
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_TRANSFORMHNODE))
		desc.pTransformHNode = pdesc->pTransformHNode;
	pcollec->pRenderable = NCreateRenderable(&desc);

	// Geometry creation
	NGEOMETRY_FORMAT_ENUM rsid;
	NSTATESET user_stateset_flags;

	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_GEOMETRY_FORMAT))
	{
		rsid = pdesc->GeometryFormat;
	}
	else
	{
		rsid = DEFAULT_NCOLLECTIBLE_DESC_GEOMETRY_SPECS_ID;
	}
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USE_USERSTATESET))
	{
		user_stateset_flags = pdesc->UserStateSet;
	}
	else
	{
		user_stateset_flags = DEFAULT_NCOLLECTIBLE_USERSTATESET;
	}

	// Geometry RSID Checks:
	//		MESH:			NVERTEX_V3C4T2 and NPRIMITIVE_TRIANGLE  only.
	//		APPEARANCE:		At least 1 TextureUnit (obviously !)
	//						No other constraints ...
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(NGetGeometrySpecs(rsid)->IDS) != NPRIMITIVE_FORMAT_TRIANGLE, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(NGetGeometrySpecs(rsid)->IDS) != NVERTEX_FORMAT_V3C4T2, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT);
	NErrorIf(NGetGeometrySpecs(rsid)->UsedTextureUnitNumber < 1, NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);

	NFillupGeometryDesc(&geomdesc, rsid, user_stateset_flags, pcollec->ElementCount * 2, pcollec->ElementCount * 4);
	pgeom = NGeometryEmplaceBack(pcollec->pRenderable, &geomdesc);
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_APPEARANCE))
	{
		NSetGeometryAppearance(pgeom, &pdesc->Appearance_Raw);
	}

	// Quads Creation
	memset(&quad_desc, 0, sizeof(NUT_QUAD_DESC));
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR))
	{
		quad_desc.CoreDesc.ConstructionPlane_DirectorVector = pdesc->ConstructionPlane_DirectorVector;
		FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR);
	}
	else
	{
		// this is the default option: using XY plane ...
		quad_desc.CoreDesc.Construction_Plane = _PLANE_XY;
		FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	}
	// Define quad position ( just build it from its own center )
	FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_ANCHOR);
	NVec3Set(&quad_desc.CoreDesc.Anchor, ANCHOR_MIDWIDTH, ANCHOR_MIDHEIGHT, 0);
	// Define Size
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_SIZE))
	{
		FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_SIZE);
		quad_desc.CoreDesc.Size = pdesc->ElementSize;
	}
	// Define Scale
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_SCALE))
	{
		FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_SCALE);
		quad_desc.CoreDesc.Scale = pdesc->ElementScale;
	}
	// Building Colors: just transfer data to quad_desc.
	quad_desc.CoreDesc.ColorBuild = pdesc->ColorBuild;
	// Building TexCoords: just transfer data to quad_desc.
	quad_desc.CoreDesc.TexCoordBuild = pdesc->TexCoordBuild;

	// Create Mesh of quads:
	// thanks to NUT_InsertQuad, everything will be created and push back to the geometry
	// without any memory reallocation (Thanks to geometry capacities preallocation)
	// .. lets go !
	for (i = 0; i < pcollec->ElementCount; i++)
	{
		NUT_InsertQuad(pgeom, &quad_desc);
		BITSET(pcollec->BitShape, i);
	}
	// All inserted quads are exactly the same. We need to store Vertex description for one of them  to be able to rebuild them
	// if a reshaping is requested.
	pvertex = (NCOLLECTIBLEVERTEX *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	memcpy(&pcollec->VertexShape, pvertex, 4 * sizeof(NCOLLECTIBLEVERTEX)); // 4 vertex for a quad !

	// + END
	// Link with the function to get each billboard position
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_GET_ELEMENT_POSITION))
	{
		pcollec->pGetElementPosition = pdesc->pGetElementPosition;
	}
	else
	{
		pcollec->pGetElementPosition = NGetCollectibleElementPosition_DefaultFCT;
	}

	// Link with the callback function call before validate each collectible picking
	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_PICKING_CALLBACK))
	{
		pcollec->pPicking_CallBack = pdesc->pPicking_CallBack;
	}
	else
	{
		pcollec->pPicking_CallBack = NCollectibleElementPicking_DefaultCallback;
	}

	if (ISFLAG_ON(pdesc->Flags, FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_RADIUS))
	{
		pcollec->ElementRadius = pdesc->ElementRadius;
	}
	else
	{
		pcollec->ElementRadius = DEFAULT_NCOLLECTIBLE_ELEMENT_RADIUS;
	}

	return pcollec;
}

void NClearCollectible(NCOLLECTIBLE *pcollectible)
{
	// It seems the collectible is bind to a Collectible Set ...
	NErrorIf(pcollectible->BindID != 0xFFFF, NERROR_UNAUTHORIZED_ACTION);
	NDeleteRenderable(pcollectible->pRenderable);
	memset(pcollectible, 0, sizeof(NCOLLECTIBLE));
}

NCOLLECTIBLE *NCreateCollectible(const NCOLLECTIBLE_DESC *pdesc)
{
	return NSetupCollectible(NEW(NCOLLECTIBLE), pdesc);
}

void NDeleteCollectible(NCOLLECTIBLE *pcollec)
{
	NClearCollectible(pcollec);
	Nfree(pcollec);
}

void NUpdateCollectibleAABB(NCOLLECTIBLE *pcollec)
{
	NVEC3 v;

	// Setup BoundingBox for Updating
	NVec3Set(&pcollec->AABB.VMax, -NF32_MAX, -NF32_MAX, -NF32_MAX);
	NVec3Set(&pcollec->AABB.VMin, NF32_MAX, NF32_MAX, NF32_MAX);

	for (Nu32 i = 0; i < pcollec->ElementCount; i++)
	{
		if (BITGET(pcollec->BitShape, i))
		{
			pcollec->pGetElementPosition(&v, pcollec, i);

			// Bounding box updating
			if (v.x - pcollec->ElementRadius < pcollec->AABB.fXMin)
				pcollec->AABB.fXMin = v.x - pcollec->ElementRadius;
			if (v.x + pcollec->ElementRadius > pcollec->AABB.fXMax)
				pcollec->AABB.fXMax = v.x + pcollec->ElementRadius;

			if (v.y - pcollec->ElementRadius < pcollec->AABB.fYMin)
				pcollec->AABB.fYMin = v.y - pcollec->ElementRadius;
			if (v.y + pcollec->ElementRadius > pcollec->AABB.fYMax)
				pcollec->AABB.fYMax = v.y + pcollec->ElementRadius;

			if (v.z - pcollec->ElementRadius < pcollec->AABB.fZMin)
				pcollec->AABB.fZMin = v.z - pcollec->ElementRadius;
			if (v.z + pcollec->ElementRadius > pcollec->AABB.fZMax)
				pcollec->AABB.fZMax = v.z + pcollec->ElementRadius;
		}
	}
}
// ------------------------------------------------------------------------------------------
// NReshapeCollectible
// ------------------------------------------------------------------------------------------
// Description :
//		Rebuild the Collectible mesh from the incoming bitshape.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetCollectibleBitShape(NCOLLECTIBLE *pcollec, const Nu32 bitshape)
{
	// NErrorIf(!NIsDetectableCollectible(pcollec),NERROR_COLLECTIBLE_MUST_BE_ENABLE);

	Nu32 i, j, firstvertex;
	NVEC3 v;
	NGEOMETRY *pgeom;
	NCOLLECTIBLEVERTEX *pvertex;
	NTRIANGLE *ptri;

	pgeom = (NGEOMETRY *)pcollec->pRenderable->GeomArray.pFirst;

	// All the  geometry 3d data resize to 0
	// but notice nothing is really destroy, just array sizes set to 0. It's important for UV,color and primitives ...
	// They still there, always the same for each quadruplet of vertex
	NResizeArray(&pgeom->Mesh.VertexArray, 0, NULL, NULL);
	NResizeArray(&pgeom->Mesh.PrimitiveArray, 0, NULL, NULL);

	pvertex = (NCOLLECTIBLEVERTEX *)pgeom->Mesh.VertexArray.pFirst;
	ptri = (NTRIANGLE *)pgeom->Mesh.PrimitiveArray.pFirst;
	firstvertex = 0;

	// printf("\n SetCollectibleBitShape : %d BB",pcollec->CollectiblesNumber);

	// Setup BoundingBox for Updating
	NVec3Set(&pcollec->AABB.VMax, -NF32_MAX, -NF32_MAX, -NF32_MAX);
	NVec3Set(&pcollec->AABB.VMin, NF32_MAX, NF32_MAX, NF32_MAX);

	pcollec->BitShape = bitshape;

	for (i = 0; i < pcollec->ElementCount; i++)
	{
		if (BITGET(pcollec->BitShape, i))
		{
			pcollec->pGetElementPosition(&v, pcollec, i);
			for (j = 0; j < 4; j++, pvertex++)
			{
				pvertex->Position_3f.x = v.x + pcollec->VertexShape[j].Position_3f.x;
				pvertex->Position_3f.y = v.y + pcollec->VertexShape[j].Position_3f.y;
				pvertex->Position_3f.z = v.z + pcollec->VertexShape[j].Position_3f.z;
			}

			firstvertex += 4;
			pgeom->Mesh.VertexArray.Size += 4;
			pgeom->Mesh.PrimitiveArray.Size += 2;

			// Bounding box updating
			if (v.x - pcollec->ElementRadius < pcollec->AABB.fXMin)
				pcollec->AABB.fXMin = v.x - pcollec->ElementRadius;
			if (v.x + pcollec->ElementRadius > pcollec->AABB.fXMax)
				pcollec->AABB.fXMax = v.x + pcollec->ElementRadius;

			if (v.y - pcollec->ElementRadius < pcollec->AABB.fYMin)
				pcollec->AABB.fYMin = v.y - pcollec->ElementRadius;
			if (v.y + pcollec->ElementRadius > pcollec->AABB.fYMax)
				pcollec->AABB.fYMax = v.y + pcollec->ElementRadius;

			if (v.z - pcollec->ElementRadius < pcollec->AABB.fZMin)
				pcollec->AABB.fZMin = v.z - pcollec->ElementRadius;
			if (v.z + pcollec->ElementRadius > pcollec->AABB.fZMax)
				pcollec->AABB.fZMax = v.z + pcollec->ElementRadius;
		}
	}
	// Update Renderable Bounding sphere
	NGetAABBBoundingSphere(&pcollec->pRenderable->BoundingSphere, &pcollec->AABB);
}

// ------------------------------------------------------------------------------------------
// Nu32 NGetEnableCollectibleElementCount
// ------------------------------------------------------------------------------------------
// Description :
//	a NCOLLECTIBLE is an object that can be single or multiple ( contain 1 to 32 "collectible")
//	This is like this to optimize collectible management for numerous collectibles like "coins".
//	Instead having 1 coin = 1 collectible we have 1 to 32 coins = 1 collectible !!!
//	THIS IS Faster !!!
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NGetEnableCollectibleElementCount(NCOLLECTIBLE *pcollec)
{
	Nu32 nb = 0;
	for (Nu32 i = 0; i < pcollec->ElementCount; i++)
	{
		if (BITGET(pcollec->BitShape, i))
		{
			nb++;
		}
	}
	return nb;
}