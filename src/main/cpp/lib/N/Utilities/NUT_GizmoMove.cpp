#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_X.h"
#include "NUT_3DMouse.h"

#include "NUT_GizmoMove.h"
// ------------------------------------------------------------------------------------------
// NUT_GIZMO_MOVE* NUT_SetupGizmoMove
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_GIZMO_MOVE* NUT_SetupGizmoMove(NUT_GIZMO_MOVE *pgm, const Nf32 axis_size, const Nu8 default_activated_axis, const NMATRIX *pmat, const Nu16 disableaxis_flags)
{
	NErrorIf((default_activated_axis>2)&& (default_activated_axis!= CONSTANT_NUT_GIZMOMOVE_NO_DEFAULT_AXIS),NERROR_INCONSISTENT_PARAMETERS );
	NErrorIf(!axis_size,NERROR_NULL_VALUE);
	NErrorIf(!FLAGS_TEST(disableaxis_flags,MASK_NUT_GIZMO_MOVE_DISABLE_AXIS,disableaxis_flags),NERROR_UNAUTHORIZED_FLAG);
	NErrorIf((default_activated_axis<3)&&(((disableaxis_flags)&(1<<default_activated_axis))==1),NERROR_INCONSISTENT_PARAMETERS);
	Nmem0(pgm,NUT_GIZMO_MOVE);
	pgm->Flags |= (disableaxis_flags&MASK_NUT_GIZMO_MOVE_DISABLE_AXIS); // Just to keep only the flags that we are interested in.
	pgm->AxisSize = axis_size;
	// By default, Activation Distance is calculated as to b 5% of the AxisSize ( Activation distance may be considered as the half thickness
	// of the axis ).Notice that we are going to store the square of it.
	// It remains possible to change it by using 'NUT_SetGizmoMove_ActivationDistance'
	pgm->ActivationSquareDistance = CONSTANT_NUT_GIZMO_MOVE_ACTIV_DIST_FROM_AXIS_SIZE_FACTOR*
									CONSTANT_NUT_GIZMO_MOVE_ACTIV_DIST_FROM_AXIS_SIZE_FACTOR*
									axis_size*axis_size;
	
	// If user set the default axis as a locked axis ... it will CRASH in debug Mode but in Release it's manage differently
	// ... and Engine just set currentAxis ID to ... no one.
	if( (default_activated_axis<3)&&(((disableaxis_flags)&(1<<default_activated_axis))==1) )
	{
		pgm->CurrentAxisID = CONSTANT_NUT_GIZMOMOVE_NO_DEFAULT_AXIS;
	}
	else
	{
		pgm->CurrentAxisID = default_activated_axis;
	}
	
	if(pmat)
	{
		pgm->Matrix = *pmat;
	}
	else
	{
		NIdentityMatrix(&pgm->Matrix);
	}

	// default parameters ( they can be changed but by calling some specifics functions )
	// SNAP
	FLAG_OFF(pgm->Flags,FLAG_NUT_GIZMO_MOVE_SNAP);	//|
	NVec3Set(&pgm->SnapOrigin,0,0,0);			//| Useless but just to be Cristal clear about what we want to have.
	pgm->SnapStrength = 0.0f;						//|

	NVec3Set(&pgm->SnapStep,1.0f,1.0f,1.0f);		//| It's not going to be used like this (FLAG_NUT_GIZMO_MOVE_SNAP set to OFF). But SnapStep components  NEVER can be equal to ZERO !!!
	return pgm;
}
// ------------------------------------------------------------------------------------------
// NUT_GIZMO_MOVE* NUT_CreateGizmoMove
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_GIZMO_MOVE* NUT_CreateGizmoMove(const Nf32 axis_size, const Nu8 default_activated_axis, const NMATRIX *pmat, const Nu16 disableaxis_flags)
{
	return NUT_SetupGizmoMove(NEW(NUT_GIZMO_MOVE),axis_size,default_activated_axis,pmat,default_activated_axis);
}

void NUT_ClearGizmoMove(NUT_GIZMO_MOVE *pgm)
{
	Nmem0(pgm,NUT_GIZMO_MOVE);
}

void NUT_DeleteGizmoMove(NUT_GIZMO_MOVE *pgm)
{
	NUT_ClearGizmoMove(pgm);
	Nfree(pgm);
}
// ------------------------------------------------------------------------------------------
// NUT_GIZMOMOVE_RESULT NUT_GizmoMove_TouchDown
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
inline NUT_GIZMOMOVE_RESULT _gizmove_pick( NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB )
{
	NUT_3DLINEXLINE_RESULT		xres;
	NVEC3					D;
	Nf32						f;
	Nu32						i;
	Nf32						fdist = pgm->ActivationSquareDistance;

	pgm->CurrentAxisID = CONSTANT_NUT_GIZMOMOVE_NO_DEFAULT_AXIS; // means none

	// Pick GizmoMove Axis
	for(i=0;i<3;i++)
	{
		if( !(pgm->Flags&(1<<i)) ) // Axis is not Disable 
		{
			D.x = pgm->Matrix.Translation.x + pgm->Matrix.V4[i].x * pgm->AxisSize;		// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 
			D.y = pgm->Matrix.Translation.y + pgm->Matrix.V4[i].y * pgm->AxisSize;		// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 
			D.z = pgm->Matrix.Translation.z + pgm->Matrix.V4[i].z * pgm->AxisSize;		// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 

			// The Tested Line		AB		is based on the "3DMouseSegment" ( the segment from camera 3D World position to mouse 3D World position )
			// The Tested Line		CD		is based on the Axis.
			NUT_LineXLine( &xres,pA,pB,(NVEC3*)&pgm->Matrix.Translation,&D );
			if(xres.ParamCoordCD >= 0.0f && xres.ParamCoordCD <= 1.0f )
			{
				f = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&xres);
				if(  f < fdist )
				{
					fdist	= f;
					pgm->CurrentAxisID	= (Nu8)i;
					pgm->StartHandle	= xres.PointOnCD;
				}
			}
		}
	}

	// ... complete the setup
	if(pgm->CurrentAxisID != CONSTANT_NUT_GIZMOMOVE_NO_DEFAULT_AXIS)
	{
		pgm->CurrentHandle = pgm->PreviousHandle = pgm->StartHandle;
		NVec3Sub(&pgm->HandleShift,(NVEC3*)&pgm->Matrix.Translation,&pgm->StartHandle);
		return 1;
	}

	return 0;
}

NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_2D(NUT_GIZMO_MOVE *pgm, const NVEC2s16 *ptouchdown)
{
	NVEC3					A,B;
	// Get 3Dtouch from 2DTouch.
	NUT_Get3DMouseSegment(&A,&B,NULL,ptouchdown);
	return _gizmove_pick(pgm,&A,&B);
}
NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_3D(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB)
{
	return _gizmove_pick(pgm,pA,pB);
}

NUT_GIZMOMOVE_RESULT NUT_PickGizmoMove_3DRay(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pdir)
{
	NVEC3 B;
	B.x = pA->x + pdir->x;
	B.y = pA->y + pdir->y;
	B.z = pA->z + pdir->z;
	return _gizmove_pick(pgm,pA,&B);
}


static inline void _handle_to_translation(NUT_GIZMO_MOVE *pgm)
{
	NMATRIX3x3	m3x3,mt3x3;
	NVEC3	p,pt;
	
	p.x = pgm->CurrentHandle.x + pgm->HandleShift.x;
	p.y = pgm->CurrentHandle.y + pgm->HandleShift.y;
	p.z = pgm->CurrentHandle.z + pgm->HandleShift.z;

	if( ISFLAG_ON(pgm->Flags,FLAG_NUT_GIZMO_MOVE_SNAP) && pgm->CurrentAxisID!=NU8_MAX )
	{
		m3x3.XAxis = *(NVEC3*)&pgm->Matrix.XAxis;
		m3x3.YAxis = *(NVEC3*)&pgm->Matrix.YAxis;
		m3x3.ZAxis = *(NVEC3*)&pgm->Matrix.ZAxis;
		NTransposeMatrix3x3O(&mt3x3,&m3x3);
		NMulVector3ByMatrix3x3(&pt,&mt3x3,&p);
		Nf32 f = floor( pt.coord[pgm->CurrentAxisID]/pgm->SnapStep.coord[pgm->CurrentAxisID] + 0.5f ) * pgm->SnapStep.coord[pgm->CurrentAxisID];
		if( NABS( pt.coord[pgm->CurrentAxisID] - f )<=pgm->SnapStrength )
		{
			pt.coord[pgm->CurrentAxisID] = f;
			NMulVector3ByMatrix3x3(&p,&m3x3,&pt);
		}
	}

	pgm->PreviousTranslation	= pgm->Matrix.Translation;

	pgm->Matrix.Translation.x	= p.x;
	pgm->Matrix.Translation.y	= p.y;
	pgm->Matrix.Translation.z	= p.z;
}
// ------------------------------------------------------------------------------------------
// NUT_GIZMOMOVE_RESULT NUT_GizmoMove_Update
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
inline void _gizmomove_update(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB)
{
	NUT_3DLINEXLINE_RESULT	xres;
	NVEC3 D;
	// Note that we are not going to use AxisSize anymore here. It's normal because we don't care of axis length after it was chosen,
	// we just still interested to move along the entire line ...
	D.x = pgm->Matrix.Translation.x + pgm->Matrix.V4[pgm->CurrentAxisID].x ;// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 
	D.y = pgm->Matrix.Translation.y + pgm->Matrix.V4[pgm->CurrentAxisID].y ;// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 
	D.z = pgm->Matrix.Translation.z + pgm->Matrix.V4[pgm->CurrentAxisID].z ;// V4[0] is X Axis, V4[1] is Y Axis, V4[2] is Z Axis ... 

	// The Tested Line		AB		is based on the "3DMouseSegment" ( the segment from camera 3D World position to mouse 3D World position )
	// The Tested Line		CD		is based on the Axis.
	NUT_LineXLine( &xres,pA,pB,(NVEC3*)&pgm->Matrix.Translation,&D );

	pgm->PreviousHandle			= pgm->CurrentHandle;
	pgm->CurrentHandle			= xres.PointOnCD;
	_handle_to_translation(pgm);
}


NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_2D(NUT_GIZMO_MOVE *pgm, const NVEC2s16 *ptouchmove)
{
	(FLAG_NUT_GIZMO_MOVE_ON|(1<<pgm->CurrentAxisID));

	if( (pgm->CurrentAxisID != NU8_MAX) && FLAGS_TEST(pgm->Flags, FLAG_NUT_GIZMO_MOVE_ON|(1<<pgm->CurrentAxisID),FLAG_NUT_GIZMO_MOVE_ON ))
	{
		NVEC3					A,B;
		// Get 3D Mouse from 2D Mouse.
		NUT_Get3DMouseSegment(&A,&B,NULL,ptouchmove);
		_gizmomove_update(pgm,&A,&B);
		return 1;
	}
	return 0;
}

NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_3D(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pB)
{
	if( (pgm->CurrentAxisID != NU8_MAX) && FLAGS_TEST(pgm->Flags, FLAG_NUT_GIZMO_MOVE_ON|(1<<pgm->CurrentAxisID),FLAG_NUT_GIZMO_MOVE_ON ))
	{
		_gizmomove_update(pgm,pA,pB);
		return 1;
	}
	return 0;
}
NUT_GIZMOMOVE_RESULT NUT_UpdateGizmoMove_3DRay(NUT_GIZMO_MOVE *pgm, const NVEC3 *pA, const NVEC3 *pdir)
{
	if( (pgm->CurrentAxisID != NU8_MAX) && FLAGS_TEST(pgm->Flags, FLAG_NUT_GIZMO_MOVE_ON|(1<<pgm->CurrentAxisID),FLAG_NUT_GIZMO_MOVE_ON ))
	{
		NVEC3 B;
		B.x = pA->x + pdir->x;
		B.y = pA->y + pdir->y;
		B.z = pA->z + pdir->z;
		_gizmomove_update(pgm,pA,&B);
		return 1;
	}
	return 0;
}
// ------------------------------------------------------------------------------------------
// void NUT_PlaceGizmoMove
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_PlaceGizmoMove(NUT_GIZMO_MOVE *pgm, const NVEC3 *ppos)
{
	// Set Position
	if(ppos)
	{
		pgm->Matrix.Translation.x = ppos->x;
		pgm->Matrix.Translation.y = ppos->y;
		pgm->Matrix.Translation.z = ppos->z;
	}
	else
	{
		pgm->Matrix.Translation.x = 0.0f;
		pgm->Matrix.Translation.y = 0.0f;
		pgm->Matrix.Translation.z = 0.0f;
	}
	
	// And Set All the Handles
	pgm->StartHandle = pgm->PreviousHandle = pgm->CurrentHandle = *(NVEC3*)&pgm->Matrix.Translation;
	pgm->HandleShift.x = pgm->HandleShift.y = pgm->HandleShift.z = 0.0f; 
	pgm->PreviousTranslation = pgm->Matrix.Translation;
}

