#ifndef __NUT_GIZMO_H
#define __NUT_GIZMO_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Gizmo.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "../NFlags.h"
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../NMatrix.h"
#include "../NScreenPoint.h"
#include "lib/N/NErrorHandling.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_NUT_GIZMO_MOVE_NB_MARKINGS 6						   // as to be a multiple of 2
#define CONSTANT_NUT_GIZMO_MOVE_ACTIV_DIST_FROM_AXIS_SIZE_FACTOR 0.05f // 5%
#define CONSTANT_NUT_GIZMO_MOVE_AXIS_ARROW_SIZE_RATIO 0.1f			   // should be ( ... ACTIV_DIST_SIZE_FACTOR x 2 )
#define CONSTANT_NUT_GIZMO_MOVE_AXIS_MAX_LENGTH 100000.0f			   // used to draw 'WholeAxis'
#define CONSTANT_NUT_GIZMOMOVE_NO_DEFAULT_AXIS NU8_MAX

#define FLAG_NUT_GIZMO_MOVE_DISABLE_X_AXIS BIT_0 //|
#define FLAG_NUT_GIZMO_MOVE_DISABLE_Y_AXIS BIT_1 //|_0,1,2 like axis ID ! (and it's important)
#define FLAG_NUT_GIZMO_MOVE_DISABLE_Z_AXIS BIT_2 //|

#define FLAG_NUT_GIZMO_MOVE_DISABLE_PLANAR_MOVE BIT_3
#define FLAG_NUT_GIZMO_MOVE_ON BIT_4
#define FLAG_NUT_GIZMO_MOVE_SNAP BIT_5
#define FLAG_NUT_GIZMO_MOVE_WHOLE_AXIS BIT_6
#define FLAG_NUT_GIZMO_MOVE_AXIS_MARKINGS BIT_7

	/*
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_8
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_9
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_10
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_11
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_12
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_13
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_14
	#define FLAG_NUT_GIZMO_MOVE_xxx											BIT_15
	*/

#define MASK_NUT_GIZMO_MOVE_DISABLE_AXIS (FLAG_NUT_GIZMO_MOVE_DISABLE_X_AXIS | FLAG_NUT_GIZMO_MOVE_DISABLE_Y_AXIS | FLAG_NUT_GIZMO_MOVE_DISABLE_Z_AXIS)

	typedef struct
	{
		Nu16 Flags;
		Nu8 CurrentAxisID;
		Nu8 Available_Nu8;

		NMATRIX Matrix;
		NVEC4 PreviousTranslation;

		Nf32 ActivationSquareDistance;
		Nf32 AxisSize; // To pick and draw Axis ( Different than the scale factor potentially included into the Matrix )

		Nf32 SnapStrength;
		NVEC3 SnapOrigin; // Axis relative ( from position ,... along axis, in case of oriented Matrix)
		NVEC3 SnapStep;

		NVEC3 StartHandle;
		NVEC3 PreviousHandle;
		NVEC3 CurrentHandle;
		NVEC3 HandleShift;

	} NUT_GIZMO_MOVE;

	typedef Ns32 NUT_GIZMOMOVE_RESULT;

	NUT_GIZMO_MOVE *NUT_SetupGizmoMove(NUT_GIZMO_MOVE *pgm, const Nf32 axis_size, const Nu8 default_activated_axis, const NMATRIX *pmat, const Nu16 disableaxis_flags);
	NUT_GIZMO_MOVE *NUT_CreateGizmoMove(const Nf32 axis_size, const Nu8 default_activated_axis, const NMATRIX *pmat, const Nu16 disableaxis_flags);
	void NUT_ClearGizmoMove(NUT_GIZMO_MOVE *pgm);
	void NUT_DeleteGizmoMove(NUT_GIZMO_MOVE *pgm);

	inline void NUT_SetGizmoMove_ActivationDistance(NUT_GIZMO_MOVE *pgm, const Nf32 activdist) { pgm->ActivationSquareDistance = activdist * activdist; }

	// Pick Functions (suppose to be used at Touch Down )
	// These functions are doing the same ( but with different parameters ... like this its possible to save useless multiple call to 2DMouse->3DMouse function )
	NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_2D(NUT_GIZMO_MOVE *pgm, const NVEC2s16 *ptouchdown);
	NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_3D(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB);
	NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_3DRay(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pdir);
	// Update Functions (suppose to be used at Touch Move )
	// These functions are doing the same ( but with different parameters ... like this its possible to save useless multiple call to 2DMouse->3DMouse function )
	NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_2D(NUT_GIZMO_MOVE *pgm, const NVEC2s16 *ptouchmove);
	NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_3D(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB);
	NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_3DRay(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pdir);

	void NUT_PlaceGizmoMove(NUT_GIZMO_MOVE *pgm, const NVEC3 *ppos);

	inline void NUT_EnableGizmoMove(NUT_GIZMO_MOVE *pgm) { FLAG_ON(pgm->Flags, FLAG_NUT_GIZMO_MOVE_ON); }
	inline void NUT_EnableGizmoMove_AxisMarkings(NUT_GIZMO_MOVE *pgm) { FLAG_ON(pgm->Flags, FLAG_NUT_GIZMO_MOVE_AXIS_MARKINGS); }
	inline void NUT_EnableGizmoMove_Snap(NUT_GIZMO_MOVE *pgm) { FLAG_ON(pgm->Flags, FLAG_NUT_GIZMO_MOVE_SNAP); }
	inline void NUT_EnableGizmoMove_WholeAxis(NUT_GIZMO_MOVE *pgm) { FLAG_ON(pgm->Flags, FLAG_NUT_GIZMO_MOVE_WHOLE_AXIS); }

	inline void NUT_DisableGizmoMove(NUT_GIZMO_MOVE *pgm) { FLAG_OFF(pgm->Flags, FLAG_NUT_GIZMO_MOVE_ON); }
	inline void NUT_DisableGizmoMove_AxisMarkings(NUT_GIZMO_MOVE *pgm) { FLAG_OFF(pgm->Flags, FLAG_NUT_GIZMO_MOVE_AXIS_MARKINGS); }
	inline void NUT_DisableGizmoMove_Snap(NUT_GIZMO_MOVE *pgm) { FLAG_OFF(pgm->Flags, FLAG_NUT_GIZMO_MOVE_SNAP); }
	inline void NUT_DisableGizmoMove_WholeAxis(NUT_GIZMO_MOVE *pgm) { FLAG_OFF(pgm->Flags, FLAG_NUT_GIZMO_MOVE_WHOLE_AXIS); }

	inline void NUT_SetGizmoMove_SnapOrigin(NUT_GIZMO_MOVE *pgm, const NVEC3 *porigin) { pgm->SnapOrigin = *porigin; }
	inline void NUT_SetGizmoMove_SnapOriginf(NUT_GIZMO_MOVE *pgm, const Nf32 ox, const Nf32 oy, const Nf32 oz)
	{
		pgm->SnapOrigin.x = ox;
		pgm->SnapOrigin.y = oy;
		pgm->SnapOrigin.z = oz;
	}
	inline void NUT_SetGizmoMove_SnapStep(NUT_GIZMO_MOVE *pgm, const NVEC3 *pstep)
	{
		NErrorIf(!pstep->x || !pstep->y || !pstep->z, NERROR_INCONSISTENT_PARAMETERS);
		pgm->SnapStep = *pstep;
	}
	inline void NUT_SetGizmoMove_SnapStepf(NUT_GIZMO_MOVE *pgm, const Nf32 stepx, const Nf32 stepy, const Nf32 stepz)
	{
		NErrorIf(!stepx || !stepy || !stepz, NERROR_INCONSISTENT_PARAMETERS);
		pgm->SnapStep.x = stepx;
		pgm->SnapStep.y = stepy;
		pgm->SnapStep.z = stepz;
	}
	inline void NUT_SetGizmoMove_SnapStrength(NUT_GIZMO_MOVE *pgm, const Nf32 strength) { pgm->SnapStrength = strength; }

	inline NVEC3 *NUT_GetGizmoMove_Position(NVEC3 *prm, const NUT_GIZMO_MOVE *pgm)
	{
		*prm = *(NVEC3 *)&pgm->Matrix.Translation;
		return prm;
	}
	inline NVEC3 *NUT_GetGizmoMove_RelativeMove(NVEC3 *prm, const NUT_GIZMO_MOVE *pgm)
	{
		NVec3Sub(prm, (NVEC3 *)&pgm->Matrix.Translation, (NVEC3 *)&pgm->PreviousTranslation);
		return prm;
	}

	inline NVEC3 *NUT_GetGizmoMove_HandlePosition(NVEC3 *prm, const NUT_GIZMO_MOVE *pgm)
	{
		*prm = pgm->CurrentHandle;
		return prm;
	}
	inline NVEC3 *NUT_GetGizmoMove_PreviousHandlePosition(NVEC3 *prm, const NUT_GIZMO_MOVE *pgm)
	{
		*prm = pgm->PreviousHandle;
		return prm;
	}

// Notes:	To draw a GizmoMove on Screen have a look at NUT_Draw.h
//			(Use the function NUT_Draw_GizmoMove with its parameters...)

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_GIZMO_H
