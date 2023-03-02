#include "lib/N/NCStandard.h"
#include "../Containers/NArray.h"
#include "../NMatrix.h"
#include "../NViewport.h"
#include "../Miscellaneous/N2DPointInside.h"
#include "../Utilities/NUT_Pick.h"
#include "NUI.h"

extern NARRAY ActiveUIRootsList;

NUI *NPickUIRecursive(const NUI *pui, const NVEC2 *phreltouchpos)
{
	NUI *pret;
	NNODE *pnode;

	if (FLAGS_TEST(pui->Flags, MASK_NUI_ACTIVE, MASK_NUI_ACTIVE) && pui->Pick_Proc(pui, phreltouchpos)) // FLAG_UI_SHOW and FLAG_UI_ENABLE set to ON !
	{
		// Parse the list backward from the last to the first
		pnode = (NNODE *)pui->ChildrenList.pLast;
		while ((NNODE *)pnode != (NNODE *)&pui->ChildrenList)
		{
			if ((pret = NPickUIRecursive((NUI *)pnode, phreltouchpos)))
			{
				return pret;
			}

			pnode = (NNODE *)pnode->pPrevious;
		}

		/*
				OBSOLET

				// If FLAG_NUI_LET_TOUCH_FOCUS_ON_CHILDREN is set to ON, that means
				// UI cant have Focus but accepts to let one its children having it if its possible.
				// So if we are here, that means UI was picked, but no one of its children... So if the flag is set
				// to ON, that means UI cant be the Focused 'UI', so just return NULL...
				if( ISFLAG_ON(pui->Flags,FLAG_NUI_LET_TOUCH_FOCUS_ON_CHILDREN) )
				{
					return NULL;
				}
				else
				{
					return (NUI*)pui;
				}

				OBSOLET
		*/
		return (NUI *)pui;
	}
	else
	{
		return NULL;
	}
}

/*
NUI* NPickUIRecursive(const NNODELIST *pchildrenlist,const NVEC2 *phreltouchpos)
{
	NUI			*pui,*pret;
	NNODE		*pnode;

	// Parse the list backward from the last to the first
	pnode = (NNODE*)pchildrenlist->pLast;
	while( (NNODE*)pnode != (NNODE*)pchildrenlist)
	{
		pui = (NUI*)pnode;
		if( FLAGS_TEST(pui->Flags,MASK_NUI_ACTIVE,MASK_NUI_ACTIVE) ) // FLAG_UI_SHOW and FLAG_UI_ENABLE set to ON !
		{
			if( pui->Pick_Proc(pui,phreltouchpos) )
			{
				if( pret = NPickUIRecursive(&pui->ChildrenList,phreltouchpos) )
				{
					return pret;
				}
				else
				{
					// If FLAG_NUI_LET_TOUCH_FOCUS_ON_CHILDREN is set to ON, that means
					// UI cant have Focus but accepts to let one its children having it if its possible.
					// So if we are here, that means UI was picked, but no one of its children... So if the flag is set
					// to ON, that means UI cant be the Focused 'UI', so just continue the seeking loop...
					if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LET_TOUCH_FOCUS_ON_CHILDREN) )
						return pui;
				}
			}
		}
		pnode=(NNODE*)pnode->pPrevious;
	}

	return NULL;
}
*/

NUI *NPickUI(const NVEC2s16 *ppos)
{
	// Call of all the Current UI UI Handles
	// Starting with the Last UIRoot (NGetLastArrayPtr) and finishing with the first.
	// In that way, the more fresh UIRoot is called first !
	Nu32 i;
	NUI *pui;
	NVEC2 hreltouchpos;

	hreltouchpos.x = NCoord_PixToHRel_X(ppos->x);
	hreltouchpos.y = NCoord_PixToHRel_Y(ppos->y); // NCoord_PixToHRel_Y(-ppos->y);

	NUI **ptr = (NUI **)NGetLastArrayPtr(&ActiveUIRootsList);
	for (i = ActiveUIRootsList.Size; i != 0; i--, ptr--)
	{
		if ((pui = NPickUIRecursive(*ptr, &hreltouchpos)))
		{
			return pui;
		}
	}
	return NULL;
}

NUI *NPickUI_HRel(const NVEC2 *phreltouchpos)
{
	// Call of all the Current UI UI Handles
	// Starting with the Last UIRoot (NGetLastArrayPtr) and finishing with the first.
	// In that way, the more fresh UIRoot is called first !
	Nu32 i;
	NUI *pui;
	NUI **ptr = (NUI **)NGetLastArrayPtr(&ActiveUIRootsList);
	for (i = ActiveUIRootsList.Size; i != 0; i--, ptr--)
	{
		if ((pui = NPickUIRecursive(*ptr, phreltouchpos)))
		{
			return pui;
		}
	}
	return NULL;
}

void NUpdateUIBBox_Void(NUI *pui)
{
	// Do nothing, but is called ...
}

void NUpdateUIBBox_From_ChildrenBBox(NUI *pui)
{
	NNODE *pnode;
	NRECTf32 *pbbox;

	pnode = (NNODE *)pui->ChildrenList.pFirst;
	if ((NNODE *)pnode == (NNODE *)&pui->ChildrenList)
		return;

	pbbox = &pui->BoundingBox;

	// Init 'pui' bounding box as the first child BBox...
	*pbbox = ((NUI *)pnode)->BoundingBox;
	pnode = (NNODE *)pnode->pNext;

	while ((NNODE *)pnode != (NNODE *)&pui->ChildrenList)
	{
		if (((NUI *)pnode)->BoundingBox.left < pbbox->left)
			pbbox->left = ((NUI *)pnode)->BoundingBox.left;

		if (((NUI *)pnode)->BoundingBox.right > pbbox->right)
			pbbox->right = ((NUI *)pnode)->BoundingBox.right;

		if (((NUI *)pnode)->BoundingBox.bottom < pbbox->bottom)
			pbbox->bottom = ((NUI *)pnode)->BoundingBox.bottom;

		if (((NUI *)pnode)->BoundingBox.top > pbbox->top)
			pbbox->top = ((NUI *)pnode)->BoundingBox.top;

		pnode = (NNODE *)pnode->pNext;
	}
}

void NUpdateUIBBox_From_PickingMesh(NUI *pui)
{
	NErrorIf(pui->pPickingGeometryMesh->VertexArray.Size != pui->PickingGeometryTransformedVertexArray.Size, NERROR_UI_WRONG_TRANSFORMED_VERTEX_ARRAY_SIZE);

	// -----------------------------------
	// TODO precalc Global Ortho Matrix
	// It will be replace the code bellow

	// Multiply each picking Mesh  vertex by the result matrix to obtain the picking mesh projected vertex
	// ... And, parse all of these projected vertex to define the 2D BBox.

	// Strange Init ...	// No too strange actually.
	// values Pre-stored into the BboundingBox are "default value inverted.
	// left takes right default, right takes left default, top takes bottom default and bottom takes top default ...
	// So in that way all "ON AIR" BBOX are going to be NMIN-NMAX Tested properly against these values to determine the parent bbox.
	NRECTf32 *pbbox = &pui->BoundingBox;
	/*
		pbbox->left		= NGetViewPort_AspectRatio();	// NGetViewPort_AspectRatio()+1; >> a Margin of 1 could be better ?
		pbbox->right	= 0;								//-1; >> a Margin of 1 could be better ?
		pbbox->bottom	= 0;								// 1; >> a Margin of 1 could be better ?
		pbbox->top		=-1;								//-2; >> a Margin of 1 could be better ?
	*/

	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pui->pPickingGeometryMesh->VertexArray);	 // -> !!! UNKNOWN VERTEX FORMAT HERE !!!
	NVEC3 *pr = (NVEC3 *)NGetFirstArrayPtr(&pui->PickingGeometryTransformedVertexArray); // -> An array of NVEC3 actually
	Nu32 stride = pui->pPickingGeometryMesh->VertexArray.ElementSize;

	// First Vertex for Initialization:
	NMulVector3ByMatrixO(pr, (NMATRIX *)&pui->pUITransformHNode->WorldMatrix, pv);
	pbbox->right = pbbox->left = pr->x;
	pbbox->top = pbbox->bottom = pr->y;
	// Parse all the next picking mesh vertex
	pv = (NVEC3 *)((NBYTE *)pv + stride);
	pr++;
	for (Nu32 i = pui->pPickingGeometryMesh->VertexArray.Size - 1; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride), pr++)
	{
		NMulVector3ByMatrixO(pr, (NMATRIX *)&pui->pUITransformHNode->WorldMatrix, pv);

		// looking for Min/Max  (BoundingBox evaluation)
		if (pr->x > pbbox->right)
			pbbox->right = pr->x;
		else if (pr->x < pbbox->left)
			pbbox->left = pr->x;

		if (pr->y > pbbox->top)
			pbbox->top = pr->y;
		else if (pr->y < pbbox->bottom)
			pbbox->bottom = pr->y;
	}
}

void NUpdateUIBBox_From_TextMesh(NUI *pui)
{
	NRECTf32 *pbbox;
	NVEC3 *pr;
	NGEOMETRY *pgeom;
	NVEC3 *pv;
	Nu32 i;
	Nu32 nbvertex, g;
	Nu32 stride;
	// -----------------------------------
	// Multiply each picking Mesh  vertex by the result matrix to obtain the picking mesh projected vertex
	// ... And, parse all of these projected vertex to define the 2D BBox.

	// Strange Init ...	// No too strange actually.
	// values Pre-stored into the BboundingBox are default value inverted.
	// left takes right default, right takes left default, top takes bottom default and bottom takes top default ...
	// So in that way all "ON AIR" BBOX are going to be NMIN-NMAX Tested properly against these values to determine the parent bbox.
	pbbox = &pui->BoundingBox;

	pbbox->left = NGetViewPort_AspectRatio(); // NGetViewPort_AspectRatio()+1; >> a Margin of 1 could be better ?
	pbbox->right = 0;						  //-1; >> a Margin of 1 could be better ?
	pbbox->bottom = 0;						  // 1; >> a Margin of 1 could be better ?
	pbbox->top = -1;						  //-2; >> a Margin of 1 could be better ?

	// -----------------------------------------------------------------------------------------
	// Resize TransformedVertexArray !
	nbvertex = 0;
	pgeom = NGetFirstIncludedGeometry(pui->pText->pRenderable);
	for (g = NGetIncludedGeometriesNumber(pui->pText->pRenderable); g != 0; g--, pgeom++)
	{
		nbvertex += pgeom->Mesh.VertexArray.Size;
	}
	NResizeArray(&pui->PickingGeometryTransformedVertexArray, nbvertex, NULL, NULL);
	// -----------------------------------------------------------------------------------------

	pr = (NVEC3 *)NGetFirstArrayPtr(&pui->PickingGeometryTransformedVertexArray);
	pgeom = NGetFirstIncludedGeometry(pui->pText->pRenderable);
	for (g = NGetIncludedGeometriesNumber(pui->pText->pRenderable); g != 0; g--, pgeom++ /*,pr--*/)
	{
		pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
		stride = pgeom->Mesh.VertexArray.ElementSize;
		// Parse all the picking mesh vertex
		for (i = pgeom->Mesh.VertexArray.Size; i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride), pr++)
		{
			NMulVector3ByMatrixO(pr, (NMATRIX *)&pui->pUITransformHNode->WorldMatrix, (NVEC3 *)pv);

			// looking for Min/Max  (BoundingBox evaluation)
			if (pr->x > pbbox->right)
				pbbox->right = pr->x;

			if (pr->x < pbbox->left) // !!! NOT 'else if' due to the ''strange init''
				pbbox->left = pr->x;

			if (pr->y > pbbox->top)
				pbbox->top = pr->y;

			if (pr->y < pbbox->bottom) // !!! NOT 'else if' due to the ''strange init''
				pbbox->bottom = pr->y;
		}
	}
}

NUI_PICK_RESULT NPickUIPickingTextMesh(const NUI *pui, const NVEC2 *pmouse_normalized)
{
	Nu32 i, g;
	NVEC3 *pr;
	NGEOMETRY *pgeom;
	NTRIANGLE *ptri;

	// Using the 2D UI Bounding Box
	if (pmouse_normalized->x >= pui->BoundingBox.left)
	{
		if (pmouse_normalized->y <= pui->BoundingBox.top)
		{
			if (pmouse_normalized->x <= pui->BoundingBox.right)
			{
				if (pmouse_normalized->y >= pui->BoundingBox.bottom)
				{
					// We are going using transformed vertex coordinates from 'PickingGeometryTransformedVertexArray'
					// (because Renderable text geometries vertex were transformed here !)
					// ... and the Primitive from all the text Renderable geometries ...
					pr = (NVEC3 *)NGetFirstArrayPtr(&pui->PickingGeometryTransformedVertexArray);
					pgeom = NGetFirstIncludedGeometry(pui->pText->pRenderable);
					for (g = NGetIncludedGeometriesNumber(pui->pText->pRenderable); g != 0; g--, pr += pgeom->Mesh.VertexArray.Size, pgeom++)
					{
						ptri = (NTRIANGLE *)pgeom->Mesh.PrimitiveArray.pFirst;
						// Parse all the picking mesh triangle using the transformed vertex (2D) to perform the test
						for (i = pgeom->Mesh.PrimitiveArray.Size; i != 0; i--, ptri++)
						{
							if (N2DPointInside2DTriangle((NVEC2 *)&pr[ptri->i0],
														 (NVEC2 *)&pr[ptri->i1],
														 (NVEC2 *)&pr[ptri->i2],
														 pmouse_normalized))
								return (FLAG_NUI_PICK_RESULT_IS_PICKED | FLAG_NUI_PICK_RESULT_PICK_CHILDREN);
						}
					}
				}
			}
		}
	}

	return 0;
}

NUI_PICK_RESULT NPickUIBBox(const NUI *pui, const NVEC2 *pmouse_normalized)
{
	// Using the 2D UI Bounding Box
	if (pmouse_normalized->x >= pui->BoundingBox.left)
	{
		if (pmouse_normalized->y <= pui->BoundingBox.top)
		{
			if (pmouse_normalized->x <= pui->BoundingBox.right)
			{
				if (pmouse_normalized->y >= pui->BoundingBox.bottom)
				{
					return (FLAG_NUI_PICK_RESULT_IS_PICKED | FLAG_NUI_PICK_RESULT_PICK_CHILDREN);
				}
			}
		}
	}

	return 0;
}

NUI_PICK_RESULT NPickUIPickingMesh(const NUI *pui, const NVEC2 *pmouse_normalized)
{

	// Using the 2D UI Bounding Box
	if (pmouse_normalized->x >= pui->BoundingBox.left)
	{
		if (pmouse_normalized->y <= pui->BoundingBox.top)
		{
			if (pmouse_normalized->x <= pui->BoundingBox.right)
			{
				if (pmouse_normalized->y >= pui->BoundingBox.bottom)
				{
					NErrorIf(!pui->pPickingGeometryMesh, NERROR_UI_FRAME_CUSTOM_PICKING_GEOMETRYMESH_REQUESTED);

					NMESH *pgeom_mesh = pui->pPickingGeometryMesh;
					NTRIANGLE *ptri = (NTRIANGLE *)pgeom_mesh->PrimitiveArray.pFirst;
					NVEC3 *pr = (NVEC3 *)pui->PickingGeometryTransformedVertexArray.pFirst;

					// Parse all the picking mesh triangle using the transformed vertex (2D) to perform the test
					for (Nu32 i = pgeom_mesh->PrimitiveArray.Size; i != 0; i--, ptri++)
					{
						if (N2DPointInside2DTriangle((NVEC2 *)&pr[ptri->i0],
													 (NVEC2 *)&pr[ptri->i1],
													 (NVEC2 *)&pr[ptri->i2],
													 pmouse_normalized))
							return (FLAG_NUI_PICK_RESULT_IS_PICKED | FLAG_NUI_PICK_RESULT_PICK_CHILDREN);
					}
				}
			}
		}
	}

	return 0;
}
