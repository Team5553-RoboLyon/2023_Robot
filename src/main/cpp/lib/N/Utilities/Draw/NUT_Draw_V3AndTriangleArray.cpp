#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

extern NGEOMETRY				*NUT_pCurrentDrawableShapeGeometry;
void NUT_Draw_Vector3Array(const NARRAY *pvector3array, const NCOLOR *pcolor)
{
	NErrorIf(pvector3array->ElementSize != sizeof(NVEC3), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NVEC3 *pv =(NVEC3*)NGetFirstArrayPtr(pvector3array);
	NUTDRAWVERTEX	vertex;
	vertex.Color0_4f	= *pcolor;
	vertex.Position_3f	= *pv;
	NUT_DrawPencil_From(&vertex);
	for (Nu32 i=NGetArraySize(pvector3array)-1;i!=0;i--,vertex.Position_3f=*(pv++))//for (i=NGetArraySize(parray)-1;i!=0;i--,pv++,vertex.Position_3f=*pv)
	{
		NUT_DrawPencil_LineTo(&vertex);
	}
}

// Notes:
// This function is going to "extract" vertex position from the vertex array which contains unknown vertex format.
void NUT_Draw_TrianglesArray(const NUT_DRAW_TRIANGLES_ARRAY *ptriarray)
{
	NUTDRAWVERTEX	vert;	
	NTRIANGLE		*ptri	= (NTRIANGLE*)ptriarray->pTrianglesArray->pFirst;
	NBYTE			*pv		= (NBYTE*)ptriarray->pFirstVertexPosition;
	Nu32			stride  = ptriarray->VertexStride;	
	Nu32			i;

	vert.Color0_4f = ptriarray->Color;
	for (i=ptriarray->pTrianglesArray->Size;i!=0;i--,ptri++)
	{
		vert.Position_3f = *(NVEC3*)(pv + stride*ptri->i0);	NUT_DrawPencil_From(&vert); 
		vert.Position_3f = *(NVEC3*)(pv + stride*ptri->i1);	NUT_DrawPencil_LineTo(&vert); 
		vert.Position_3f = *(NVEC3*)(pv + stride*ptri->i2);	NUT_DrawPencil_LineTo(&vert); 
		vert.Position_3f = *(NVEC3*)(pv + stride*ptri->i0);	NUT_DrawPencil_LineTo(&vert); 
	}
}
