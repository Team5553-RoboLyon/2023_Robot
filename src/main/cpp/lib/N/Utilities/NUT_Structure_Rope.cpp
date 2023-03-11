#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"

#include "lib/N/NStructure.h"
#include "NUT_Structure.h"

// Simple: put all Rope joints along a spline.
void NUT_PhysicRopePosing(NSTRUCTURE *pstructure, const NSPLINE *pspline)
{
	// A Rope is just a chain of joints linked together with constraints.
	// So a rope in pretty close to a spline which is a chain of segments
}

// ------------------------------------------------------------------------------------------
// NUT_PhysicRopeAlign
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//		pstructure:	ptr on a valid structure which is a ROPE
//
//		axis:		ptr on a valid NVEC3. Use a normalized vector.
//
//		method:		The function place the each "jointB" from the "jointA" connected with.
//					The length used to perform that is the one describe by this value
// Out :
//		The total length of the Rope after the full process
//
// ------------------------------------------------------------------------------------------
Nf32 NUT_PhysicRopeAlign(NSTRUCTURE *pstructure, const NVEC3 *axis, const NUT_PHYSIC_ROPE_ALIGN method)
{
	Nu32 i;
	NCONSTRAINT *pc;
	NVEC3 v;
	Nf32 fulllength = 0.0f;

	switch (method)
	{
	case NUT_PHYSIC_ROPE_ALIGN_USE_SPRING_RESTLENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SpringArray);
		for (i = 0; i < NGetArraySize(&pstructure->SpringArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fRestLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incomming axis it's not necessary normalized !
		}
		break;

	case NUT_PHYSIC_ROPE_ALIGN_USE_SIMPLECONSTRAINT_LENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SimpleConstraintArray);
		for (i = 0; i < NGetArraySize(&pstructure->SimpleConstraintArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incomming axis it's not necessary normalized !
		}
		break;

	case NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MINLENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->MinMaxConstraintArray);
		for (i = 0; i < NGetArraySize(&pstructure->MinMaxConstraintArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fMinLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incomming axis it's not necessary normalized !
		}
		break;

	case NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MINLENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SpecialConstraintArray);
		for (i = 0; i < NGetArraySize(&pstructure->SpecialConstraintArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fMinLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incomming axis it's not necessary normalized !
		}
		break;

	case NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MAXLENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->MinMaxConstraintArray);
		for (i = 0; i < NGetArraySize(&pstructure->MinMaxConstraintArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fMaxLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incomming axis it's not necessary normalized !
		}
		break;

	case NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MAXLENGTH:
		pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SpecialConstraintArray);
		for (i = 0; i < NGetArraySize(&pstructure->SpecialConstraintArray); i++, pc++)
		{
			NVec3Scale(&v, axis, pc->fMaxLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			pc->pjB->OldPosition = pc->pjB->CurrentPosition;
			NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
			fulllength += NVec3Length(&v); // Because incoming axis it's not necessary normalized !
		}
		break;
	}
	return fulllength;
}