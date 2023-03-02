#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../Maths/NMatrix3x3.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

extern NGEOMETRY			*NUT_pCurrentDrawableShapeGeometry;
extern NGEOMETRY			NUT_DrawableShapeGeometry[]; // 0 means 3D, 1 means ORTHO
extern NMATRIX3x3			NUT_DrawOrthonormal;


void NUT_Draw_Angle(const NVEC3f32 *pO, const NVEC2f32 *pA, const Nf32 angle, const NCOLOR *pcolor)
{

	Nf32			norm = NVec2Length(pA);
//	NErrorIf(norm == 0.0f, NERROR_INVALID_EXTENT); // pA must be a 'non null-vector'

	// i2d,j2d base 2d 'directe'. ( règle de la main droite)
	NVEC2	i2d, j2d;
	NVEC2	vang;

	i2d		= *pA;
	j2d.x	= -i2d.y;
	j2d.y	=  i2d.x;

	NUTDRAWVERTEX	v0, va;
	v0.Color0_4f = va.Color0_4f = *(NCOLOR*)pcolor;
	// A
	va.Position_3f.x = pO->x + i2d.x*NUT_DrawOrthonormal.Side.x + i2d.y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + i2d.x*NUT_DrawOrthonormal.Side.y + i2d.y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + i2d.x*NUT_DrawOrthonormal.Side.z + i2d.y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_From(&va);
	// ................... O
	v0.Position_3f = *pO;
	NUT_DrawPencil_LineTo(&v0);

	// .................. B ( angle AÔB )
	Nf32	cosaob = cosf(angle);
	Nf32	sinaob = sinf(angle);

	vang.x = cosaob * i2d.x + sinaob * j2d.x;
	vang.y = cosaob * i2d.y + sinaob * j2d.y;

	va.Position_3f.x = pO->x + vang.x*NUT_DrawOrthonormal.Side.x + vang.y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + vang.x*NUT_DrawOrthonormal.Side.y + vang.y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + vang.x*NUT_DrawOrthonormal.Side.z + vang.y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_LineTo(&va);

	// Arc
	i2d.x *= 0.25f;
	i2d.y *= 0.25f;
	j2d.x *= 0.25f;
	j2d.y *= 0.25f;

	va.Position_3f.x = pO->x + i2d.x*NUT_DrawOrthonormal.Side.x + i2d.y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + i2d.x*NUT_DrawOrthonormal.Side.y + i2d.y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + i2d.x*NUT_DrawOrthonormal.Side.z + i2d.y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_From(&va);

	Nf32 arcangle;
	for (Nu32 i = 1; i < 13; i++)
	{
		arcangle = ((Nf32)i / 12.0f)*angle;
		cosaob = cosf(arcangle);
		sinaob = sinf(arcangle);

		vang.x = cosaob * i2d.x + sinaob * j2d.x;
		vang.y = cosaob * i2d.y + sinaob * j2d.y;

		va.Position_3f.x = pO->x + vang.x*NUT_DrawOrthonormal.Side.x + vang.y*NUT_DrawOrthonormal.Up.x;
		va.Position_3f.y = pO->y + vang.x*NUT_DrawOrthonormal.Side.y + vang.y*NUT_DrawOrthonormal.Up.y;
		va.Position_3f.z = pO->z + vang.x*NUT_DrawOrthonormal.Side.z + vang.y*NUT_DrawOrthonormal.Up.z;
		NUT_DrawPencil_LineTo(&va);
	}
}

void NUT_Draw_FastAngle(const NVEC3f32 *pO, const NVEC2f32 *pA, const Nu32 fastangle, const NCOLOR *pcolor)
{

	Nf32			norm = NVec2Length(pA);
	NErrorIf(norm == 0.0f, NERROR_INVALID_EXTENT); // pA must be a 'non null-vector'

	// i2d,j2d base 2d 'directe'. ( règle de la main droite)
	NVEC2	i2d, j2d;
	NVEC2	vang;

	i2d		= *pA;
	j2d.x	= -i2d.y;
	j2d.y	=  i2d.x;

	NUTDRAWVERTEX	v0, va;
	v0.Color0_4f = va.Color0_4f = *(NCOLOR*)pcolor;
	// A
	va.Position_3f.x = pO->x + pA->x*NUT_DrawOrthonormal.Side.x + pA->y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + pA->x*NUT_DrawOrthonormal.Side.y + pA->y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + pA->x*NUT_DrawOrthonormal.Side.z + pA->y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_From(&va);
	// ................... O
	v0.Position_3f = *pO;
	NUT_DrawPencil_LineTo(&v0);

	// .................. B ( angle AÔB )
	Nf32	cosaob, sinaob;
	NFastCosSin(fastangle, &cosaob, &sinaob);

	vang.x = cosaob * i2d.x + sinaob * j2d.x;
	vang.y = cosaob * i2d.y + sinaob * j2d.y;

	va.Position_3f.x = pO->x + vang.x*NUT_DrawOrthonormal.Side.x + vang.y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + vang.x*NUT_DrawOrthonormal.Side.y + vang.y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + vang.x*NUT_DrawOrthonormal.Side.z + vang.y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_LineTo(&va);

	// Arc
	i2d.x *= 0.25f;
	i2d.y *= 0.25f;
	j2d.x *= 0.25f;
	j2d.y *= 0.25f;

	va.Position_3f.x = pO->x + i2d.x*NUT_DrawOrthonormal.Side.x + i2d.y*NUT_DrawOrthonormal.Up.x;
	va.Position_3f.y = pO->y + i2d.x*NUT_DrawOrthonormal.Side.y + i2d.y*NUT_DrawOrthonormal.Up.y;
	va.Position_3f.z = pO->z + i2d.x*NUT_DrawOrthonormal.Side.z + i2d.y*NUT_DrawOrthonormal.Up.z;
	NUT_DrawPencil_From(&va);

	Nu32 fastarcangle;
	for (Nu32 i = 1; i < 13; i++)
	{
		fastarcangle = (Nu32)(((Nf32)i / 12.0f)*fastangle);
		NFastCosSin(fastarcangle, &cosaob, &sinaob);

		vang.x = cosaob * i2d.x + sinaob * j2d.x;
		vang.y = cosaob * i2d.y + sinaob * j2d.y;

		va.Position_3f.x = pO->x + vang.x*NUT_DrawOrthonormal.Side.x + vang.y*NUT_DrawOrthonormal.Up.x;
		va.Position_3f.y = pO->y + vang.x*NUT_DrawOrthonormal.Side.y + vang.y*NUT_DrawOrthonormal.Up.y;
		va.Position_3f.z = pO->z + vang.x*NUT_DrawOrthonormal.Side.z + vang.y*NUT_DrawOrthonormal.Up.z;
		NUT_DrawPencil_LineTo(&va);
	}

}
