#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"


void NUT_Draw_PhysicStructure(const NUT_DRAW_PHYSICSTRUCTURE *pdraw)
{
	Nu32			i;
	NJOINT			*pj;
	NCONSTRAINT		*pc;
	NUTDRAWVERTEX	v;

	NErrorIf(!pdraw->pStructure,NERROR_INVALID_PARAMETER);

	v.Color0_4f = pdraw->Color;

	switch(pdraw->ElementType)
	{
		case NSTRUCTURE_ELEMENT_JOINT:
			pj = (NJOINT*)NGetFirstJointPtr(pdraw->pStructure);
			for(i=NGetArraySize(&pdraw->pStructure->JointArray);i!=0;i--,pj++)
			{
				v.Position_3f.x = pj->CurrentPosition.x - pdraw->JointSize;
				v.Position_3f.y = pj->CurrentPosition.y;
				v.Position_3f.z = pj->CurrentPosition.z;
				NUT_DrawPencil_From(&v);
				v.Position_3f.x = pj->CurrentPosition.x + pdraw->JointSize;
				NUT_DrawPencil_LineTo(&v);

				v.Position_3f.x = pj->CurrentPosition.x;
				v.Position_3f.y = pj->CurrentPosition.y - pdraw->JointSize;
				//v.Position_3f.z = pj->CurrentPosition.z;
				NUT_DrawPencil_From(&v);
				v.Position_3f.y = pj->CurrentPosition.y + pdraw->JointSize;
				NUT_DrawPencil_LineTo(&v);

				//v.Position_3f.x = pj->CurrentPosition.x;
				v.Position_3f.y = pj->CurrentPosition.y;
				v.Position_3f.z = pj->CurrentPosition.z - pdraw->JointSize;
				NUT_DrawPencil_From(&v);
				v.Position_3f.z = pj->CurrentPosition.z + pdraw->JointSize;
				NUT_DrawPencil_LineTo(&v);
			}
			break;
		case NSTRUCTURE_ELEMENT_SPRING:
			pc = (NCONSTRAINT*)NGetFirstArrayPtr(&pdraw->pStructure->SpringArray);
			for(i=NGetArraySize(&pdraw->pStructure->SpringArray);i!=0;i--,pc++)
			{	
				v.Position_3f = pc->pjA->CurrentPosition;	NUT_DrawPencil_From(&v);
				v.Position_3f = pc->pjB->CurrentPosition;	NUT_DrawPencil_LineTo(&v);
			}
			break;
		case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
			pc = (NCONSTRAINT*)NGetFirstArrayPtr(&pdraw->pStructure->SimpleConstraintArray);
			for(i=NGetArraySize(&pdraw->pStructure->SimpleConstraintArray);i!=0;i--,pc++)
			{
				v.Position_3f = pc->pjA->CurrentPosition;	NUT_DrawPencil_From(&v);
				v.Position_3f = pc->pjB->CurrentPosition;	NUT_DrawPencil_LineTo(&v);
			}
			break;
		case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
			pc = (NCONSTRAINT*)NGetFirstArrayPtr(&pdraw->pStructure->MinMaxConstraintArray);
			for(i=NGetArraySize(&pdraw->pStructure->MinMaxConstraintArray);i!=0;i--,pc++)
			{
				v.Position_3f = pc->pjA->CurrentPosition;	NUT_DrawPencil_From(&v);
				v.Position_3f = pc->pjB->CurrentPosition;	NUT_DrawPencil_LineTo(&v);
			}
			break;
		case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
			pc = (NCONSTRAINT*)NGetFirstArrayPtr(&pdraw->pStructure->SpecialConstraintArray);
			for(i=NGetArraySize(&pdraw->pStructure->SpecialConstraintArray);i!=0;i--,pc++)
			{
				v.Position_3f = pc->pjA->CurrentPosition;	NUT_DrawPencil_From(&v);
				v.Position_3f = pc->pjB->CurrentPosition;	NUT_DrawPencil_LineTo(&v);
			}
			break;
		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			break;
	}
}
