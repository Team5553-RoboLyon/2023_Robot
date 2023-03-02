#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"


void NUT_Draw_3DMatrix3x3(const NMATRIX3x3	*pm3x3, const NVEC3 *pposition, const Nf32 scale)
{
	NUTDRAWVERTEX	v0;
	NUTDRAWVERTEX	v1;



	// We are  going to read and draw this matrix as an orthogonal
	v0.Position_3f = *pposition;
	
	// XAXIS: RED
	NSetColorf(&v0.Color0_4f,NCOLOR_PRESET3F_RED,1.0f);
	v1.Color0_4f = v0.Color0_4f;
	NVec3Scale(&v1.Position_3f,&pm3x3->XAxis,scale);
	NVec3AddTo(&v1.Position_3f,&v0.Position_3f);
	NUT_DrawPencil_Line(&v0,&v1);
	
	// YAXIS : GREEN
	NSetColorf(&v0.Color0_4f,NCOLOR_PRESET3F_GREEN,1.0f);
	v1.Color0_4f = v0.Color0_4f;
	NVec3Scale(&v1.Position_3f,&pm3x3->YAxis,scale);
	NVec3AddTo(&v1.Position_3f,&v0.Position_3f);
	NUT_DrawPencil_Line(&v0,&v1);
	
	// ZAXIS : BLUE
	NSetColorf(&v0.Color0_4f,NCOLOR_PRESET3F_BLUE,1.0f);
	v1.Color0_4f = v0.Color0_4f;
	NVec3Scale(&v1.Position_3f,&pm3x3->ZAxis,scale);
	NVec3AddTo(&v1.Position_3f,&v0.Position_3f);
	NUT_DrawPencil_Line(&v0,&v1);

}

