#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "../../NCore.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NViewport.h"
#include "lib/N/Maths/NMatrix3x3.h"
//#include "../../Utilities/Maths/NUT_MathsMisc.h"
//#include "../../Utilities/Maths/NUT_Matrix.h"
#include "lib/N/Maths/NUT_MathsMisc.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

// Globals
NGEOMETRY				*NUT_pCurrentDrawableShapeGeometry;
NGEOMETRY				NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ENUM_SIZE]; // 0 means 3D, 1 means ORTHO
NMATRIX3x3				NUT_DrawOrthonormal = {	1.0f,0.0f,0.0f,
												0.0f,1.0f,0.0f,
												0.0f,0.0f,1.0f }; // => _PLANE_XY

extern NSTATESET		NCurrentStateSet;
extern NACCUMULATOR		NAccumulator[];

void NUT_SetDrawConstructionPlaneMatrix(const NMATRIX *pmtx)
{
	NUT_DrawOrthonormal.XAxis = *(NVEC3f32*)&pmtx->XAxis;
	NUT_DrawOrthonormal.YAxis = *(NVEC3f32*)&pmtx->YAxis;
	NUT_DrawOrthonormal.ZAxis = *(NVEC3f32*)&pmtx->ZAxis;
}

void NUT_SetDrawConstructionPlaneMatrix3x3(const NMATRIX3x3 *pmtx)
{
	NUT_DrawOrthonormal = *pmtx;
}

void NUT_SetDrawConstructionPlaneUnitDirectorVector(const NVEC3 *punit_director_vector )
{
	//	NForwardVectorMatrix3x3(&NUT_DrawOrthonormal, punit_director_vector);
	//	NUT_BuildHughesMoller3DOrthonormal(&NUT_DrawOrthonormal, punit_director_vector);

	// Purpose of this function is building the 'NUT_DrawOrthonormal' matrix 3x3 from a forward vector.
	NUT_Build3DOrthonormal(&NUT_DrawOrthonormal,punit_director_vector,NULL,NULL);
}

void NUT_SetDrawConstructionPlane(const NAAPLANE_ENUM axis_aligned_plane )
{
	switch(axis_aligned_plane)
	{
		case _PLANE_XY:
			NVec3Set(	&NUT_DrawOrthonormal.Side,		1.0f,	0.0f,	0.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Up,		0.0f,	1.0f,	0.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Forward,	0.0f,	0.0f,	1.0f	);
			break;

		case _PLANE_XZ:
			NVec3Set(	&NUT_DrawOrthonormal.Side,		1.0f,	0.0f,	0.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Up,		0.0f,	0.0f,	1.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Forward,	0.0f,	1.0f,	0.0f	);
			break;

		case _PLANE_ZY:
			NVec3Set(	&NUT_DrawOrthonormal.Side,		0.0f,	0.0f,	1.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Up,		0.0f,	1.0f,	0.0f	);
			NVec3Set(	&NUT_DrawOrthonormal.Forward,	1.0f,	0.0f,	0.0f	);
			break;

		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			break;
	}
}
void NUT_SetDrawMode(const NUT_DRAW_RENDERING_MODE_ENUM draw_mode)
{
	NUT_pCurrentDrawableShapeGeometry = &NUT_DrawableShapeGeometry[draw_mode];
}
NUT_DRAW_RENDERING_MODE_ENUM NUT_GetDrawMode()
{
#ifdef _DEBUG
	if( NUT_pCurrentDrawableShapeGeometry == &NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D] )
		return NUT_DRAW_RENDERING_MODE_3D;
	else if( NUT_pCurrentDrawableShapeGeometry == &NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC] )
		return NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC;
	else
	{
		// It should never happen but if the 2 previous tests failed ... crash	
		NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);
		return NUT_DRAW_RENDERING_MODE_ENUM_SIZE; // just to return something but it will never happen, 'NError' CRASH before.
	}
#endif
#ifndef _DEBUG
	// In release mode, and because we know there are only 2 possibilities ...
	// one test is enough
	if( NUT_pCurrentDrawableShapeGeometry == &NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D] )
		return NUT_DRAW_RENDERING_MODE_3D;
	else
		return NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC;
#endif
}
void NUT_RenderDrawings()
{
	// 3D Rendering
	if( NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D].Mesh.PrimitiveArray.Size )
	{
		NUpdateCurrentStateSet( NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D].Core.IDS)].StateSet );
		NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D].Core.RenderFunction(&NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D]);
		NEraseGeometryMesh(&NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_3D]);
	}
	// Orthographic Rendering
	if( NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC].Mesh.PrimitiveArray.Size )
	{
		// Matrix Context Setting Up
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0.0f,NGetViewPort_AspectRatio(),-1,0.0f,-1.0f,1.0f);
		// NOrthoMatrixf(0.0f,NGetViewPort_AspectRatio(),-1.0f,0.0f,-1.0f,1.0f);
		
		// Drawing
		NUpdateCurrentStateSet( NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC].Core.IDS)].StateSet );
		NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC].Core.RenderFunction(&NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC]);
		NEraseGeometryMesh(&NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC]);

		// Restore Previous Matrix Context 
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}

/*
void NUT_Draw_3DSegment(const NVEC3 *A, const NVEC3 *B, const NCOLOR *color)
{
	NUT_DRAW	desc;
	
	memset(&desc,0,sizeof(NUT_DRAW));
	desc.Segment.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_SEGMENT);
	NVec3Copy(&desc.Segment.A,A);
	NVec3Copy(&desc.Segment.B,B);
	if(color)
		NColorCopy(&desc.Segment.Color,color);
	else
		NSetColorf(&desc.Segment.Color,1,1,1,1);

	NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_3D);
}

void NUT_Draw_OrthographicSegment(const NVEC3 *A, const NVEC3 *B, const NCOLOR *color)
{
	NUT_DRAW	desc;

	//	desc.Segment.Type = NUT_DRAW_SEGMENT;
	desc.Segment.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_SEGMENT);
	NVec3Copy(&desc.Segment.A,A);
	NVec3Copy(&desc.Segment.B,B);
	if(color)
		NColorCopy(&desc.Segment.Color,color);
	else
		NSetColorf(&desc.Segment.Color,1,1,1,1);

	NUT_Draw(&desc,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
}

void NUT_Editor_Draw_3DColorCage(const NUT_2DAACOLORCAGE *pcc)
{
	NUT_DRAW	shape;

	memset(&shape,0,sizeof(NUT_DRAW));
	shape.ColorCage.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_COLORCAGE);
	shape.ColorCage.pColorCage			= (NUT_2DAACOLORCAGE *)pcc;
	NUT_Draw(&shape,NUT_DRAW_RENDERING_MODE_3D);
}

void NUT_Editor_Draw_OrthographicColorCage(const NUT_2DAACOLORCAGE *pcc)
{
	NUT_DRAW	shape;

	memset(&shape,0,sizeof(NUT_DRAW));
	shape.ColorCage.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_COLORCAGE);
	shape.ColorCage.pColorCage			= (NUT_2DAACOLORCAGE *)pcc;
	NUT_Draw(&shape,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
}

*/
