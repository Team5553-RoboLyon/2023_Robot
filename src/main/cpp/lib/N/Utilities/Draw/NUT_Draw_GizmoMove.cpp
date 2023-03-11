#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_Draw.h"
#include "NUT_DrawPencil.h"

extern NMATRIX3x3				NUT_DrawOrthonormal;

void NUT_Draw_GizmoMove(const NUT_GIZMO_MOVE *pgm, const NCOLOR *pactivecolor, const NUT_DRAWTEXT *ptxtparam)
{
	// Colors of GizmoMove are Fixed and NON EDITABLE excepted the ACTIVE COLOR
	//		XAXIS color -> RED		(fixed)
	//		YAXIS color -> GREEN	(fixed)
	//		ZAXIS color -> BLUE		(fixed)
	
	Nu32 i;
	NVEC3		o;
	NUTDRAWVERTEX	v0;
	NUTDRAWVERTEX	v[5];

	NCOLOR	col[3]=	{{NCOLOR_PRESET3F_RED,1.0f},{NCOLOR_PRESET3F_GREEN,1.0f},{NCOLOR_PRESET3F_BLUE_BABY,1.0f}};
	
	v0.Position_3f.x = pgm->Matrix.Translation.x;
	v0.Position_3f.y = pgm->Matrix.Translation.y;
	v0.Position_3f.z = pgm->Matrix.Translation.z;

	if(pgm->CurrentAxisID != NU8_MAX)
	{
		if(pactivecolor)
			col[pgm->CurrentAxisID] = *pactivecolor;
		else
			NSetColorf(&col[pgm->CurrentAxisID],NCOLOR_PRESET3F_YELLOW_LEMON,1.0f);

		if( ISFLAG_ON(pgm->Flags,FLAG_NUT_GIZMO_MOVE_WHOLE_AXIS) )
		{
			NSetColorf(&v[0].Color0_4f,NCOLOR_PRESET3F_ORANGE_GOLD,0.5f);
			NSetColorf(&v[1].Color0_4f,NCOLOR_PRESET3F_ORANGE_GOLD,0.5f);
			
			v[0].Position_3f.x = v0.Position_3f.x + pgm->Matrix.V4[pgm->CurrentAxisID].x*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;
			v[0].Position_3f.y = v0.Position_3f.y + pgm->Matrix.V4[pgm->CurrentAxisID].y*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;
			v[0].Position_3f.z = v0.Position_3f.z + pgm->Matrix.V4[pgm->CurrentAxisID].z*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;

			v[1].Position_3f.x = v0.Position_3f.x - pgm->Matrix.V4[pgm->CurrentAxisID].x*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;
			v[1].Position_3f.y = v0.Position_3f.y - pgm->Matrix.V4[pgm->CurrentAxisID].y*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;
			v[1].Position_3f.z = v0.Position_3f.z - pgm->Matrix.V4[pgm->CurrentAxisID].z*CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH;

			NUT_DrawPencil_Line(&v[0],&v[1]);

			if( ISFLAG_ON(pgm->Flags,FLAG_NUT_GIZMO_MOVE_AXIS_MARKINGS) )
			{

				v[0].Position_3f.x = v0.Position_3f.x + pgm->Matrix.V4[pgm->CurrentAxisID].x*pgm->SnapOrigin.coord[pgm->CurrentAxisID];
				v[0].Position_3f.y = v0.Position_3f.y + pgm->Matrix.V4[pgm->CurrentAxisID].y*pgm->SnapOrigin.coord[pgm->CurrentAxisID];
				v[0].Position_3f.z = v0.Position_3f.z + pgm->Matrix.V4[pgm->CurrentAxisID].z*pgm->SnapOrigin.coord[pgm->CurrentAxisID];

				NMATRIX3x3	m3x3,mt3x3;
				NVEC3	p0;

				m3x3.XAxis = *(NVEC3*)&pgm->Matrix.XAxis;
				m3x3.YAxis = *(NVEC3*)&pgm->Matrix.YAxis;
				m3x3.ZAxis = *(NVEC3*)&pgm->Matrix.ZAxis;
				NTransposeMatrix3x3O(&mt3x3,&m3x3);
				NMulVector3ByMatrix3x3(&p0,&mt3x3,(NVEC3*)&pgm->Matrix.Translation);
				p0.coord[pgm->CurrentAxisID] = floor( p0.coord[pgm->CurrentAxisID]/pgm->SnapStep.coord[pgm->CurrentAxisID] + 0.5f ) * pgm->SnapStep.coord[pgm->CurrentAxisID];
				NMulVector3ByMatrix3x3(&p0,&m3x3,&p0);
				NErrorIf(NISODD(CONSTANT_NUT_GIZMO_MOVE_NB_MARKINGS),NERROR_SYSTEM_INVALID_CONSTANT); // CONSTANT_NUT_GIZMO_MOVE_NB_MARKINGS has to be EVEN !
				for(i=0;i<(CONSTANT_NUT_GIZMO_MOVE_NB_MARKINGS/2);i++)
				{
					v[0].Position_3f.x = p0.x + pgm->Matrix.V4[pgm->CurrentAxisID].x*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[0].Position_3f.y = p0.y + pgm->Matrix.V4[pgm->CurrentAxisID].y*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[0].Position_3f.z = p0.z + pgm->Matrix.V4[pgm->CurrentAxisID].z*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[1].Position_3f.x = v[0].Position_3f.x + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].x*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					v[1].Position_3f.y = v[0].Position_3f.y + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].y*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					v[1].Position_3f.z = v[0].Position_3f.z + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].z*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					NUT_DrawPencil_Line(&v[0],&v[1]);

					v[0].Position_3f.x = p0.x - pgm->Matrix.V4[pgm->CurrentAxisID].x*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[0].Position_3f.y = p0.y - pgm->Matrix.V4[pgm->CurrentAxisID].y*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[0].Position_3f.z = p0.z - pgm->Matrix.V4[pgm->CurrentAxisID].z*pgm->SnapStep.coord[pgm->CurrentAxisID]*(Nf32)i;
					v[1].Position_3f.x = v[0].Position_3f.x + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].x*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					v[1].Position_3f.y = v[0].Position_3f.y + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].y*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					v[1].Position_3f.z = v[0].Position_3f.z + pgm->Matrix.V4[(pgm->CurrentAxisID+1)%3].z*pgm->AxisSize*CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO;
					NUT_DrawPencil_Line(&v[0],&v[1]);
				}
			}
		}
	}
	
	for(i=0;i<3;i++)
	{
		v0.Color0_4f = col[i];
		v[0].Color0_4f = col[i];
		
		v[0].Position_3f.x = v0.Position_3f.x + pgm->Matrix.V4[i].x * pgm->AxisSize;
		v[0].Position_3f.y = v0.Position_3f.y + pgm->Matrix.V4[i].y * pgm->AxisSize;
		v[0].Position_3f.z = v0.Position_3f.z + pgm->Matrix.V4[i].z * pgm->AxisSize;

		NUT_DrawPencil_Line(&v0,v);

		// Pyramidal arrow
		o.x = v0.Position_3f.x + pgm->Matrix.V4[i].x * pgm->AxisSize*(1.0f-CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO);
		o.y = v0.Position_3f.y + pgm->Matrix.V4[i].y * pgm->AxisSize*(1.0f-CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO);
		o.z = v0.Position_3f.z + pgm->Matrix.V4[i].z * pgm->AxisSize*(1.0f-CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO);

		v[1].Position_3f.x = o.x + pgm->Matrix.V4[(i+1)%3].x * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[1].Position_3f.y = o.y + pgm->Matrix.V4[(i+1)%3].y * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[1].Position_3f.z = o.z + pgm->Matrix.V4[(i+1)%3].z * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[1].Color0_4f = col[i];

		v[2].Position_3f.x = o.x + pgm->Matrix.V4[(i+2)%3].x * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[2].Position_3f.y = o.y + pgm->Matrix.V4[(i+2)%3].y * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[2].Position_3f.z = o.z + pgm->Matrix.V4[(i+2)%3].z * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[2].Color0_4f = col[i];
		
		v[3].Position_3f.x = o.x - pgm->Matrix.V4[(i+1)%3].x * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[3].Position_3f.y = o.y - pgm->Matrix.V4[(i+1)%3].y * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[3].Position_3f.z = o.z - pgm->Matrix.V4[(i+1)%3].z * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[3].Color0_4f = col[i];
		
		v[4].Position_3f.x = o.x - pgm->Matrix.V4[(i+2)%3].x * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[4].Position_3f.y = o.y - pgm->Matrix.V4[(i+2)%3].y * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[4].Position_3f.z = o.z - pgm->Matrix.V4[(i+2)%3].z * pgm->AxisSize*(CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO*0.5f);
		v[4].Color0_4f = col[i];

		NUT_DrawPencil_LineFan(v,5);
		NUT_DrawPencil_LineLoop(&v[1],4);

	}
}


