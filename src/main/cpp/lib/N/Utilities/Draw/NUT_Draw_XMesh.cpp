#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "lib/N/Utilities/NUT_XMesh.h"
#include "NUT_Draw.h"
#include "NUT_DrawPencil.h"

extern NMATRIX3x3				NUT_DrawOrthonormal;

void NUT_Draw_IsolatedXtri(const NUT_XTRI_ARRAY	*pxtriarray, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	vert0,vert1;	
	NUT_XTRI		*pxtri = (NUT_XTRI*)pxtriarray->XTriArray.pFirst;

	vert0.Color0_4f = *pcolor;
	vert1.Color0_4f = *pcolor;

	for (Nu32 i=pxtriarray->XTriArray.Size;i!=0;i--,pxtri++)
	{
		if(ISFLAG_OFF(pxtri->Flags,FLAG_NUT_XTRI_USED_BY_QUAD))
		{
			vert0.Position_3f = *pxtri->pPoint[0];
			NUT_DrawPencil_From(&vert0); 
			vert1.Position_3f = *pxtri->pPoint[1];
			NUT_DrawPencil_LineTo(&vert1); 
			vert1.Position_3f = *pxtri->pPoint[2];
			NUT_DrawPencil_LineTo(&vert1); 
			NUT_DrawPencil_LineTo(&vert0); 
		}
	}
}

void NUT_Draw_XArea(const NUT_XAREA *pxarea, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	vert;	
	NUT_XQUAD		**ppxquad;
	NUT_XQUAD		*pxquad;
	NUT_XTRI		*pxtri;
	Nu32			xtriedge;
	Nu32			xquadedge;

	vert.Color0_4f = *pcolor;
	ppxquad = (NUT_XQUAD**)pxarea->XQuadPtrArray.pFirst;
	for (Nu32 i=pxarea->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;
		
		pxtri		= pxquad->pXTri[0];
		xtriedge	= NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal);

		
		for(xquadedge=0;xquadedge<4;xquadedge++)
		{
			vert.Position_3f = *pxtri->pPoint[NGET_EDGE_P0(xtriedge)];
			NUT_DrawPencil_From(&vert);
			vert.Position_3f = *pxtri->pPoint[NGET_EDGE_P1(xtriedge)];
			NUT_DrawPencil_LineTo(&vert); 

			// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
			if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
			{
				pxtri	= pxtri->pAssociatedXtri;
				xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
			}
			else
			{
				xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
			}
		}
	}
}

void NUT_Draw_XRPoly(const NUT_XRPOLY *pxrpoly, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	vert;	
	NUT_XQUAD		**ppxquad;
	NUT_XQUAD		*pxquad;
	NUT_XTRI		*pxtri;
	Nu32			xtriedge;
	Nu32			xquadedge;

	vert.Color0_4f = *pcolor;
	ppxquad = (NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst;
	for (Nu32 i=pxrpoly->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;

		pxtri		= pxquad->pXTri[0];
		xtriedge	= NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal);


		for(xquadedge=0;xquadedge<4;xquadedge++)
		{
			vert.Position_3f = *pxtri->pPoint[NGET_EDGE_P0(xtriedge)];
			NUT_DrawPencil_From(&vert);
			vert.Position_3f = *pxtri->pPoint[NGET_EDGE_P1(xtriedge)];
			NUT_DrawPencil_LineTo(&vert); 

			// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
			if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
			{
				pxtri	= pxtri->pAssociatedXtri;
				xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
			}
			else
			{
				xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
			}
		}
	}
}













