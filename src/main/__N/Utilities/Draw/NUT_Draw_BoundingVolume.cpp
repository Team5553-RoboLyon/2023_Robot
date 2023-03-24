#include "../../NCStandard.h"
#include "../../Miscellaneous/NColor.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

void NUT_Draw_BoundingSphere(const NBOUNDINGSPHERE *psphere, const NCOLOR *pcolor)
{
	NUT_Draw_XYCircle(&psphere->Center,psphere->fRadius,pcolor );
	NUT_Draw_XZCircle(&psphere->Center,psphere->fRadius,pcolor );
	NUT_Draw_ZYCircle(&psphere->Center,psphere->fRadius,pcolor );
}

void NUT_Draw_AABB(const NAABB *paabb, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v,v1;
	Nf32			width	= paabb->fXMax - paabb->fXMin;
	Nf32			height	= paabb->fYMax - paabb->fYMin;
	Nf32			depth	= paabb->fZMax - paabb->fZMin;
	
	v.Color0_4f = v1.Color0_4f = *pcolor;

	// DRAW !
	v.Position_3f = paabb->VMin;
	// 1a)Back quad
	NUT_DrawPencil_From(&v);
	v.Position_3f.x += width;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.y += height;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.x -= width;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.y -= height;
	NUT_DrawPencil_LineTo(&v);

	// 1b) From back to Front
	v.Position_3f.z += depth;
	NUT_DrawPencil_LineTo(&v);

	// 2)Front quad
	v.Position_3f.x += width;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.y += height;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.x -= width;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.y -= height;
	NUT_DrawPencil_LineTo(&v);
	
	// 3) The 3 missing segments
	v.Position_3f.y += height;
	v1 = v;
	v1.Position_3f.z -= depth;
	NUT_DrawPencil_Line(&v,&v1);
	v.Position_3f.x += width;
	v1.Position_3f.x = v.Position_3f.x;
	NUT_DrawPencil_Line(&v,&v1);
	v.Position_3f.y -= height;
	v1.Position_3f.y = v.Position_3f.y;
	NUT_DrawPencil_Line(&v,&v1);
}




