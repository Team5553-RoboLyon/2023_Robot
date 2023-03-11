#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_Draw.h"
#include "NUT_DrawPencil.h"

extern NMATRIX3x3			NUT_DrawOrthonormal;

void NUT_Draw_Right(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	//	    1
	//	    |\
	//		| \
	//		|+ 0
	//	    | / 
	//	    |/
	//		2
	NVEC3		i,j;
	NUTDRAWVERTEX	v;

	NErrorIf((pextents->x < 0.0f)||(pextents->y < 0.0f), NERROR_INVALID_EXTENT );
	v.Color0_4f = v.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pextents->x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pextents->y);
	// 1
	//	\
	//	 \
	//	  0
	v.Position_3f.x = pcenter->x + i.x;
	v.Position_3f.y = pcenter->y + i.y;
	v.Position_3f.z = pcenter->z + i.z;
	NUT_DrawPencil_From(&v);

	v.Position_3f.x = pcenter->x - i.x + j.x;
	v.Position_3f.y = pcenter->y - i.y + j.y;
	v.Position_3f.z = pcenter->z - i.z + j.z;
	NUT_DrawPencil_LineTo(&v);

	// 1
	// |
	// |
	// |
	// 2
	v.Position_3f.x = pcenter->x - i.x - j.x;
	v.Position_3f.y = pcenter->y - i.y - j.y;
	v.Position_3f.z = pcenter->z - i.z - j.z;
	NUT_DrawPencil_LineTo(&v);

	//	   0
	//	  /
	//	 /
	//	2
	v.Position_3f.x = pcenter->x + i.x;
	v.Position_3f.y = pcenter->y + i.y;
	v.Position_3f.z = pcenter->z + i.z;
	NUT_DrawPencil_LineTo(&v);
}

void NUT_Draw_Left(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	//		2
	//	   /|
	//	  / |
	//	 /	|
	//	0	|
	//	 \  | 
	//	  \ |
	//	   \|
	//		1
	NVEC3		i,j;
	NUTDRAWVERTEX	v;

	NErrorIf((pextents->x < 0.0f)||(pextents->y < 0.0f), NERROR_INVALID_EXTENT );
	v.Color0_4f = v.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pextents->x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pextents->y);
	//	0	
	//	 \   
	//	  \ 
	//	   \
	//		1
	v.Position_3f.x = pcenter->x - i.x;
	v.Position_3f.y = pcenter->y - i.y;
	v.Position_3f.z = pcenter->z - i.z;
	NUT_DrawPencil_From(&v);

	v.Position_3f.x = pcenter->x + i.x - j.x;
	v.Position_3f.y = pcenter->y + i.y - j.y;
	v.Position_3f.z = pcenter->z + i.z - j.z;
	NUT_DrawPencil_LineTo(&v);

	// 2
	// |
	// |
	// |
	// |
	// |
	// 1
	v.Position_3f.x = pcenter->x + i.x + j.x;
	v.Position_3f.y = pcenter->y + i.y + j.y;
	v.Position_3f.z = pcenter->z + i.z + j.z;
	NUT_DrawPencil_LineTo(&v);

	//		2
	//	   /
	//	  / 
	//	 /	
	//	0	
	v.Position_3f.x = pcenter->x - i.x;
	v.Position_3f.y = pcenter->y - i.y;
	v.Position_3f.z = pcenter->z - i.z;
	NUT_DrawPencil_LineTo(&v);
}

void NUT_Draw_Up(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{

	//	   0	
	//	  / \   
	//	 /   \ 
	//	/     \
	// 1 ----- 2
	NVEC3		i,j;
	NUTDRAWVERTEX	v;

	NErrorIf((pextents->x < 0.0f)||(pextents->y < 0.0f), NERROR_INVALID_EXTENT );
	v.Color0_4f = v.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pextents->x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pextents->y);
	//		0
	//	   /
	//	  / 
	//	 /	
	//	1	
	v.Position_3f.x = pcenter->x + j.x;
	v.Position_3f.y = pcenter->y + j.y;
	v.Position_3f.z = pcenter->z + j.z;
	NUT_DrawPencil_From(&v);

	v.Position_3f.x = pcenter->x - i.x - j.x;
	v.Position_3f.y = pcenter->y - i.y - j.y;
	v.Position_3f.z = pcenter->z - i.z - j.z;
	NUT_DrawPencil_LineTo(&v);

	// 1 ------- 2
	v.Position_3f.x = pcenter->x + i.x - j.x;
	v.Position_3f.y = pcenter->y + i.y - j.y;
	v.Position_3f.z = pcenter->z + i.z - j.z;
	NUT_DrawPencil_LineTo(&v);

	//	0	
	//	 \   
	//	  \ 
	//	   \
	//		2
	v.Position_3f.x = pcenter->x + j.x;
	v.Position_3f.y = pcenter->y + j.y;
	v.Position_3f.z = pcenter->z + j.z;
	NUT_DrawPencil_LineTo(&v);
}

void NUT_Draw_Down(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	// 2 ----- 1
	//	\     /
	//	 \   /
	//	  \ /
	//	   0
	NVEC3		i,j;
	NUTDRAWVERTEX	v;

	NErrorIf((pextents->x < 0.0f)||(pextents->y < 0.0f), NERROR_INVALID_EXTENT );
	v.Color0_4f = v.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pextents->x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pextents->y);
	//		1
	//	   /
	//	  / 
	//	 /	
	//	0	
	v.Position_3f.x = pcenter->x - j.x;
	v.Position_3f.y = pcenter->y - j.y;
	v.Position_3f.z = pcenter->z - j.z;
	NUT_DrawPencil_From(&v);

	v.Position_3f.x = pcenter->x + i.x + j.x;
	v.Position_3f.y = pcenter->y + i.y + j.y;
	v.Position_3f.z = pcenter->z + i.z + j.z;
	NUT_DrawPencil_LineTo(&v);

	// 2 ------ 1
	v.Position_3f.x = pcenter->x - i.x + j.x;
	v.Position_3f.y = pcenter->y - i.y + j.y;
	v.Position_3f.z = pcenter->z - i.z + j.z;
	NUT_DrawPencil_LineTo(&v);

	//	2	
	//	 \   
	//	  \ 
	//	   \
	//		0
	v.Position_3f.x = pcenter->x - j.x;
	v.Position_3f.y = pcenter->y - j.y;
	v.Position_3f.z = pcenter->z - j.z;
	NUT_DrawPencil_LineTo(&v);
}