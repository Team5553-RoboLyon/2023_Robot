#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

// "NUT_Draw_BasicAAShapes.cpp" means Draw Basic Axis Aligned Shapes

void NUT_Draw_XYSquare(const NVEC3	*pcenter,const Nf32 side,const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v0;

	// Color
	v0.Color0_4f = *pcolor;

	//		0 ------- 1
	//		|         |
	//		|    +C   |
	//		|         |
	//		3 ------- 2
	// 0---1
	v0.Position_3f.x = pcenter->x - side*0.5f;
	v0.Position_3f.y = pcenter->y + side*0.5f;
	v0.Position_3f.z = pcenter->z;

	NUT_DrawPencil_From(&v0);
	v0.Position_3f.x += side;
	NUT_DrawPencil_LineTo(&v0);

	// 1
	// |
	// |
	// |
	// 2
	v0.Position_3f.y-=side;
	NUT_DrawPencil_LineTo(&v0);
	// 3---2
	v0.Position_3f.x-=side;
	NUT_DrawPencil_LineTo(&v0);

	// 0
	// |
	// |
	// |
	// 3
	v0.Position_3f.y+=side;
	NUT_DrawPencil_LineTo(&v0);
}

void NUT_Draw_XYQuad(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v0;

	// Color
	v0.Color0_4f = *pcolor;

	//		0 ------- 1
	//		|         |
	//		|    +C   |
	//		|         |
	//		3 ------- 2
	// 0---1
	v0.Position_3f.x = pcenter->x - pextents->x;
	v0.Position_3f.y = pcenter->y + pextents->y;
	v0.Position_3f.z = pcenter->z;

	NUT_DrawPencil_From(&v0);
	v0.Position_3f.x = pcenter->x + pextents->x;
	NUT_DrawPencil_LineTo(&v0);

	// 1
	// |
	// |
	// |
	// 2
	v0.Position_3f.y = pcenter->y - pextents->y;
	NUT_DrawPencil_LineTo(&v0);
	// 3---2
	v0.Position_3f.x = pcenter->x - pextents->x;
	NUT_DrawPencil_LineTo(&v0);

	// 0
	// |
	// |
	// |
	// 3
	v0.Position_3f.y = pcenter->y + pextents->y;
	NUT_DrawPencil_LineTo(&v0);

}
void NUT_Draw_XYMark(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	//	    3
	//	    |
	//	0 --+-- 1
	//	    |
	//	    2

	// NVEC3		i, j;
	NUTDRAWVERTEX	v;

	NErrorIf((pextents->x < 0.0f) || (pextents->y < 0.0f), NERROR_INVALID_EXTENT);
	v.Color0_4f = v.Color0_4f = *pcolor;
	// 0---1
	v.Position_3f.x = pcenter->x - pextents->x;
	v.Position_3f.y = pcenter->y;
	v.Position_3f.z = pcenter->z;
	NUT_DrawPencil_From(&v);
	v.Position_3f.x = pcenter->x + pextents->x;
	NUT_DrawPencil_LineTo(&v);

	// 3
	// |
	// |
	// |
	// 2
	v.Position_3f.x = pcenter->x;
	v.Position_3f.y = pcenter->y - pextents->y;
	v.Position_3f.z = pcenter->z;
	NUT_DrawPencil_From(&v);
	v.Position_3f.y = pcenter->y + pextents->y;
	NUT_DrawPencil_LineTo(&v);
}
void NUT_Draw_XYCircle(const NVEC3	*pcenter,const Nf32 radius, const NCOLOR *pcolor)
{
	Nu32			angle;
	NUTDRAWVERTEX	v,v0;
	Nf32			fcos,fsin;

	v0.Color0_4f = v.Color0_4f = *pcolor;

	v0.Position_3f		= *pcenter;
	v0.Position_3f.x	+= radius;
	NUT_DrawPencil_From(&v0);
	v.Position_3f.z = pcenter->z;
	for(angle=0;angle<NANGLELUT_SIZE;angle+=CONSTANT_NUT_DRAW_AACIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
	{
		NFastCosSin(angle,&fcos,&fsin);
		v.Position_3f.x = pcenter->x + radius*fcos;
		v.Position_3f.y = pcenter->y + radius*fsin;
		//v.Position_3f.z = pcenter->z; already done, so not necessary to repeat it for each draw !
		NUT_DrawPencil_LineTo(&v);
	}
	NUT_DrawPencil_LineTo(&v0);
}
void NUT_Draw_XZCircle(const NVEC3	*pcenter,const Nf32 radius, const NCOLOR *pcolor)
{
	Nu32			angle;
	NUTDRAWVERTEX	v,v0;
	Nf32			fcos,fsin;

	v0.Color0_4f = v.Color0_4f = *pcolor;

	v0.Position_3f		= *pcenter;
	v0.Position_3f.x	+= radius;
	NUT_DrawPencil_From(&v0);
	v.Position_3f.y = pcenter->y;
	for(angle=0;angle<NANGLELUT_SIZE;angle+=CONSTANT_NUT_DRAW_AACIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
	{
		NFastCosSin(angle,&fcos,&fsin);
		v.Position_3f.x = pcenter->x + radius*fcos;
		v.Position_3f.z = pcenter->z + radius*fsin;
		//v.Position_3f.y = pcenter->y; already done, so not necessary to repeat it for each draw !
		NUT_DrawPencil_LineTo(&v);
	}
	NUT_DrawPencil_LineTo(&v0);
}

void NUT_Draw_ZYCircle(const NVEC3	*pcenter,const Nf32 radius, const NCOLOR *pcolor)
{
	Nu32			angle;
	NUTDRAWVERTEX	v,v0;
	Nf32			fcos,fsin;

	v0.Color0_4f = v.Color0_4f = *pcolor;

	v0.Position_3f	= *pcenter;
	v0.Position_3f.z += radius;
	NUT_DrawPencil_From(&v0);
	v.Position_3f.x = pcenter->x;
	for(angle=0;angle<NANGLELUT_SIZE;angle+=CONSTANT_NUT_DRAW_AACIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
	{
		NFastCosSin(angle,&fcos,&fsin);
		v.Position_3f.z = pcenter->z + radius*fcos;
		v.Position_3f.y = pcenter->y + radius*fsin;
		//v.Position_3f.x = pcenter->z; already done, so not necessary to repeat it for each draw !
		NUT_DrawPencil_LineTo(&v);
	}
	NUT_DrawPencil_LineTo(&v0);
}

void NUT_Draw_XYTriangleR(const NVEC3 *pcenter,const Nf32 side,const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v0;
	Nf32			halfside;

	v0.Color0_4f = *pcolor;
	//		2 -
	//		|    -     
	//		|    +C  -0
	//		|    -     
	//		1 - 
	halfside = side*0.5f;
	// 0--->1
	v0.Position_3f.x = pcenter->x + halfside;
	v0.Position_3f.y = pcenter->y;
	v0.Position_3f.z = pcenter->z;
	NUT_DrawPencil_From(&v0);
	v0.Position_3f.x -= side;
	v0.Position_3f.y -= halfside;
	NUT_DrawPencil_LineTo(&v0);
	// 2
	// ^
	// |
	// |
	// 1
	v0.Position_3f.y += side;
	NUT_DrawPencil_LineTo(&v0);

	//2--->0
	v0.Position_3f.x += side;
	v0.Position_3f.y -= halfside;
	NUT_DrawPencil_LineTo(&v0);
}

void NUT_Draw_XYTriangleL(const NVEC3 *pcenter,const Nf32 side,const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v0;
	Nf32			halfside;

	v0.Color0_4f = *pcolor;
	//					2
	//					|     
	//			0	+c	|
	//					|
	//					1
	halfside = side*0.5f;

	// 0--->1
	v0.Position_3f.x = pcenter->x - halfside;
	v0.Position_3f.y = pcenter->y;
	v0.Position_3f.z = pcenter->z;
	NUT_DrawPencil_From(&v0);
	v0.Position_3f.x += side;
	v0.Position_3f.y -= halfside;
	NUT_DrawPencil_LineTo(&v0);
	// 2
	// ^
	// |
	// |
	// 1
	v0.Position_3f.y += side;
	NUT_DrawPencil_LineTo(&v0);
	//2--->0
	v0.Position_3f.x -= side;
	v0.Position_3f.y -= halfside;
	NUT_DrawPencil_LineTo(&v0);
}
