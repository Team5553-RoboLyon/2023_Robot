#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

void NUT_Draw_2DAAColorCage(const NUT_DRAW_COLORCAGE *pdraw)
{
	NUT_2DAACOLORCAGE	*pcc	= pdraw->pColorCage;
	NCOLOR				*pccol = (NCOLOR*)NGetFirstArrayPtr(&pcc->ColorArray);
	Nf32				ksize	= pdraw->KnotSize;
	Nu32				i,j;
	Nu32				id;
	NVEC3			vi,vj;
	NVEC3			vsubi,vsubj;
	NVEC3			v0,v1;
	NUTDRAWVERTEX		vert0,vert1;

	vert0.Color0_4f = vert1.Color0_4f = pdraw->Color;
	
	NVec3Set(&vi,0,0,0);
	NVec3Set(&vj,0,0,0);
	vi.coord[pcc->Is_X] = pcc->Size.x;
	vj.coord[pcc->Is_Y] = pcc->Size.y;

	NVec3Set(&vsubi,0,0,0);
	NVec3Set(&vsubj,0,0,0);
	vsubi.coord[pcc->Is_X] = pcc->Size.x/(Nf32)pcc->SegmentNb[_I];
	vsubj.coord[pcc->Is_Y] = pcc->Size.y/(Nf32)pcc->SegmentNb[_J];
	
	// DRAW LINES (I and J)
	vert0.Position_3f = pcc->Origin;
	NVec3Add(&vert1.Position_3f,&vert0.Position_3f,&vj);
	for(i=pcc->SegmentNb[_I]+1;i!=0;i--,NVec3AddTo(&vert0.Position_3f,&vsubi),NVec3AddTo(&vert1.Position_3f,&vsubi))
	{
		NUT_DrawPencil_From(&vert0);
		NUT_DrawPencil_LineTo(&vert1);
	}
	
	vert0.Position_3f = pcc->Origin;
	NVec3Add(&vert1.Position_3f,&vert0.Position_3f,&vi);
	for(j=pcc->SegmentNb[_J]+1;j!=0;j--,NVec3AddTo(&vert0.Position_3f,&vsubj),NVec3AddTo(&vert1.Position_3f,&vsubj))
	{
		NUT_DrawPencil_From(&vert0);
		NUT_DrawPencil_LineTo(&vert1);
	}

	// DRAW KNOTS
	v1 = v0 = pcc->Origin;
	id = 0;
	for(j=pcc->SegmentNb[_J]+1;j!=0;j--,NVec3AddTo(&v0,&vsubj),v1=v0)
	{
		for(i=pcc->SegmentNb[_I]+1;i!=0;i--,NVec3AddTo(&v1,&vsubi),id++)
		{
			NErrorIf(id>=NGetArraySize(&pcc->ColorArray),NERROR_ARRAY_INDEX_BEYOND_LIMITS);
			// using ColorCage colors
			NUT_Draw_XYSquare(&v1,ksize,&pccol[id]);
		}
	}
}