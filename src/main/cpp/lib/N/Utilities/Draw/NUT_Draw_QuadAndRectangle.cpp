#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_Draw.h"
#include "NUT_DrawPencil.h"

// extern NGEOMETRY				*NUT_pCurrentDrawableShapeGeometry;
// extern NGEOMETRY				NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ENUM_SIZE]; // 0 means 3D, 1 means ORTHO
extern NMATRIX3x3				NUT_DrawOrthonormal;

void NUT_Draw_Quad(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor)
{
	//		0 ------- 1
	//		|         |
	//		|    +C   |
	//		|         |
	//		3 ------- 2

	NVEC3		i,j;
	NUTDRAWVERTEX	v0,va;

	NErrorIf((pextents->x < 0.0f)||(pextents->y < 0.0f), NERROR_INVALID_EXTENT );
	v0.Color0_4f = va.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pextents->x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pextents->y);
	// 0---1
	v0.Position_3f.x = pcenter->x - i.x + j.x;
	v0.Position_3f.y = pcenter->y - i.y + j.y;
	v0.Position_3f.z = pcenter->z - i.z + j.z;
	NUT_DrawPencil_From(&v0);

	va.Position_3f.x = pcenter->x + i.x + j.x;
	va.Position_3f.y = pcenter->y + i.y + j.y;
	va.Position_3f.z = pcenter->z + i.z + j.z;
	NUT_DrawPencil_LineTo(&va);

	// 1
	// |
	// |
	// |
	// 2
	va.Position_3f.x = pcenter->x + i.x - j.x;
	va.Position_3f.y = pcenter->y + i.y - j.y;
	va.Position_3f.z = pcenter->z + i.z - j.z;
	NUT_DrawPencil_LineTo(&va);

	// 3---2
	va.Position_3f.x = pcenter->x - i.x - j.x;
	va.Position_3f.y = pcenter->y - i.y - j.y;
	va.Position_3f.z = pcenter->z - i.z - j.z;
	NUT_DrawPencil_LineTo(&va);

	// 0
	// |
	// |
	// |
	// 3
	NUT_DrawPencil_LineTo(&v0);
}

void NUT_Draw_Rectangle(const NRECTf32 *prectangle, const NCOLOR *pcolor)
{
	//		0 ------- 1
	//		|         |
	//		|    +C   |
	//		|         |
	//		3 ------- 2

	NVEC3		i,j;
	NUTDRAWVERTEX	v0,va;
	v0.Color0_4f = va.Color0_4f = *pcolor;
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, prectangle->right - prectangle->left);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	prectangle->top - prectangle->bottom);

	// 0---1
	NVec3Set(&v0.Position_3f,prectangle->left,prectangle->top,0.0f );
	NVec3Add(&va.Position_3f,&v0.Position_3f,&i);
	NUT_DrawPencil_From(&v0);
	NUT_DrawPencil_LineTo(&va);

	// 1
	// |
	// |
	// |
	// 2
	NVec3SubFrom(&va.Position_3f,&j);
	NUT_DrawPencil_LineTo(&va);

	// 3---2
	NVec3SubFrom(&va.Position_3f,&i);
	NUT_DrawPencil_LineTo(&va);

	// 0
	// |
	// |
	// |
	// 3
	NUT_DrawPencil_LineTo(&v0);
}







