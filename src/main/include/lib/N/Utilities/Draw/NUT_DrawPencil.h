#ifndef __NUT_DRAWPENCIL_H
#define __NUT_DRAWPENCIL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_DrawPencil.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

extern NGEOMETRY	*NUT_pCurrentDrawableShapeGeometry;

inline void NUT_DrawPencil_Line(const NUTDRAWVERTEX *pv0, const NUTDRAWVERTEX *pv1)
{
	NLINE			line;

	line.i0 = NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray.Size;
	line.i1 = line.i0+1;
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.PrimitiveArray,(const NBYTE*)&line);
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,(const NBYTE*)pv0);
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,(const NBYTE*)pv1);
}

inline void NUT_DrawPencil_From(const NUTDRAWVERTEX *pv0)
{
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,(const NBYTE*)pv0);
}

inline void NUT_DrawPencil_LineTo(const NUTDRAWVERTEX *pv1)
{
	NLINE	line;

	// We need to have one previous Vertex at least !
	NErrorIf( !NGetArraySize(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray),NERROR_NULL_VALUE ); 

	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,(const NBYTE*)pv1);
	line.i0 = NGetArraySize(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray)-2;
	line.i1 = line.i0+1;
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.PrimitiveArray,(NBYTE*)&line);
}

inline void NUT_DrawPencil_LineFan(const NUTDRAWVERTEX *pv1, const Nu32 nbr)
{
	NErrorIf(nbr<2, NERROR_INCONSISTENT_PARAMETERS);

	NLINE	line;
	Nu32 first = NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray.Size;
	
	NUpSizeArray(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,nbr,NULL);
	NUTDRAWVERTEX *pv = &((NUTDRAWVERTEX*)NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray.pFirst)[first];
	memcpy(pv,pv1,nbr*sizeof(NUTDRAWVERTEX));
	
	line.i0 = first;
	for(line.i1=first+1;line.i1<first+nbr;line.i1++)
	{
		NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.PrimitiveArray,(NBYTE*)&line);
	}
}

inline void NUT_DrawPencil_LineLoop(const NUTDRAWVERTEX *pv1, const Nu32 nbr)
{
	NErrorIf(nbr<2, NERROR_INCONSISTENT_PARAMETERS);

	NLINE	line;
	Nu32	i;
	Nu32 first = NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray.Size;

	NUpSizeArray(&NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray,nbr,NULL);
	NUTDRAWVERTEX *pv = &((NUTDRAWVERTEX*)NUT_pCurrentDrawableShapeGeometry->Mesh.VertexArray.pFirst)[first];
	memcpy(pv,pv1,nbr*sizeof(NUTDRAWVERTEX));

	line.i0 = first;
	line.i1 = first+1;
	for(i=1;i<nbr;i++,line.i0++,line.i1++)
	{	
		NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.PrimitiveArray,(NBYTE*)&line);
	}
	// loop
	line.i1 = first;
	NArrayPushBack(&NUT_pCurrentDrawableShapeGeometry->Mesh.PrimitiveArray,(NBYTE*)&line);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUT_DRAWPENCIL_H

