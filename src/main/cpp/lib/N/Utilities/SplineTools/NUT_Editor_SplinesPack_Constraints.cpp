#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../NCStandard.h"
#include "../../UI/NUI.h"
#include "../../NCamera.h"
#include "../../NCoordinateSystemConversions.h"
#include "../../NEditor.h"
#include "../../NSpline.h"
#include "NUT_Editor_SplinesPack_Tools.h"


// ------------------------------------------------------------------------------------------
// NUT_CreateSplinesPackConstraint
// ------------------------------------------------------------------------------------------
// Description :
//		Create a constraint which will be used by splinespack Tool to manage all the same pack
//		splinepoints positions. Each constraint is performed like a simple test, like this:
//
//		if ( OPERAND_A -TEST- OPERAND_B ) -is true- -then- ( ACTION )
//
//			OperandA	: x,y or z of the spline point position which is edited by user.
//			Test		: SMALLER, GREATER, DIFFERENT 
//			OperandB	: x,y or z of an another spline point position or of one of the 3 internal values (NVEC3).
//			Action		: OperandA = OperandB, or OperandB = OperandA
//
//		To well defined each constraint  we have to describe 3 params per Operand, one test and one Action.
//
// ------------------------------------------------------------------------------------------
// In	:
//			pt:					A valid pointer on a SNKS_SPLINESPACK_TOOLDATA structure, well initialized.
//
//			Aspline_id:			To define OperandA. Index of a spline pointer  stored into the ToolData structure (**ppfirstspline)
//								of course, this index is 0 < Aspline_id < SplineNumber !
//								
//			Asplinepoint_ref:	To define OperandA. Type of splinePoint.
//														_START_KNOT
//														_END_KNOT
//														_IN_BETWEEN_KNOT
//														_KNOT
//			
//			Av3_coord_ref:		To define OperandA. Type of position coordinates.
//														_X
//														_Y
//														_Z
//
//			test_ref:			To define the test to perform between OperandA and OperandB. There are 3 different possible tests
//														_GREATER
//														_SMALLER
//														_DIFFERENT
//
//			Bspline_id_or_value_tag:	To define OperandB.  Index of a spline pointer  stored into the ToolData structure (**ppfirstspline)
//										of course, this index is 0 < Bspline_id_or_value_tag < SplineNumber !
//										Or,
//										OperandB can be something else than a splinepoint position, it can be an internal value 
//										To define OperandB as an internal Value use
//														_SPC_INTERNAL_VALUE
//
//			Bsplinepoint_ref_or_value_ref:	To define OperandB. Type of splinePoint (if 'Bspline_id_or_value_tag' represents a spline id).
//														_START_KNOT
//														_END_KNOT
//														_IN_BETWEEN_KNOT
//														_KNOT
//
//											If 'Bspline_id_or_value_tag' is set to '_SPC_INTERNAL_VALUE', then 'Bsplinepoint_ref_or_value_ref'
//											then 'Bsplinepoint_ref_or_value_ref' has to be equal to one of these values:
//														_A
//														_B
//														_C
//														_VALUE
//
//			Bv3_coord_ref:		To define OperandA. Type of position coordinates.
//														_X
//														_Y
//														_Z
//
//			then_ref:			To define the Action to perform if the test return true.
//								There is only 2 possibilities.
//								OperandA = OperanB or OperandB = OperanA, to define which one you want use:
//														_THEN_OpA_EQUAL_OpB
//														_THEN_OpB_EQUAL_OpA
//
// Out :
//		NTRUE if the constraint is successfully created, NFALSE if not.
//
// ------------------------------------------------------------------------------------------
// Notes:
// A)	To decide which constraint has to be applied, the program uses the full definition of
//		'OperandA' stored into the constraint structure. Actually, if the current edited splinepoint 
//		is equal to "OperandA', then the constraint test is performed and the constraint will be applied
//		if necessary.
//
// B)	Some parameters combination are not possible and the 'Snks_CreateSplinesPackConstraint' function
//		will return NFALSE if you try to create one constraint with those wrong combinations.
//		The wrong combinations are:
//				'Aspline_id' can't be set with '_SPC_INTERNAL_VALUE' 

//				if 'then_ref' is set to '_THEN_OpA_EQUAL_OpB', it's not possible to set 'Bsplinepoint_ref_or_value_ref' to '_KNOT' or to '_VALUE' 

//
//		To simply understand why, take care of these simple rules:
//			a1) OperandA is always a unique splinepoint (...position coordinate value).
//			a2) OperandA is always THE current selected and modified splinepoint

//			So, when 'Asplinepoint_ref' is set to '_START_KNOT' that means: if current selected splinepoint is the startpoint of 'Aspline_id'.
//				when 'Asplinepoint_ref' is set to '_IN_BETWEEN_KNOT' that means: if current selected splinepoint is not startpoint and not endpoint of 'Aspline_id'
//				when 'Asplinepoint_ref' is set to '_KNOT' that means: current selected splinepoint can be the start point, the end point or an in_between point  of 'Aspline_id'.
//
//			b1) OperandB has to be unique if the test result is the ACTION '_THEN_OpA_EQUAL_OpB'.
//				Because A is unique (rule a1) and it's impossible for it to be equal to several value.
//			b2) OperandB can be Multiple if  test result is the ACTION '_THEN_OpB_EQUAL_OpA'.
//				Because it's possible for several values to be equal to a unique one. In that case, each of these values are tested against OperandA
//				and are set to OperandA if the test return NTRUE, 
//			b3) OperandB can be a splinepoint or an internal value. (or multiple splinepoints and multiple internal values if the linked ACTION is '_THEN_OpB_EQUAL_OpA' )

//			So, when 'then_ref' is set to '_THEN_OpA_EQUAL_OpB' it's not possible to set 'Bsplinepoint_ref_or_value_ref' to '_KNOT' or to '_VALUE'
//				in that case, OperandB can be a Splinepoint defined with a spline Id ('Bspline_id_or_value_tag'), a splinepoint Reference ('Bsplinepoint_ref_or_value_ref') and a coordinate reference ('Bv3_coord_ref').
//				Or it can be an internal value. to do that, set 'Bspline_id_or_value_tag' to '_SPC_INTERNAL_VALUE' and choose one of the 3 internal position vector3
//				by setting 'Bsplinepoint_ref_or_value_ref' to '_A' ''_B' or '_C'  and a coordinate reference ('Bv3_coord_ref') set to '_X,_Y or _Z'.
//
Nbool NUT_CreateSplinesPackConstraint(NUT_SPACK_TOOLDATA *pt, const Nu32 Aspline_id,const NUT_SPACK_SPLINEPOINT_ENUM Asplinepoint_ref,const NV3COORD_ENUM Av3_coord_ref,const NUT_SPACK_TEST_ENUM test_ref,const Nu32 Bspline_id_or_value_tag,const NUT_SPACK_SPLINEPOINT_ENUM Bsplinepoint_ref_or_value_ref,const NV3COORD_ENUM Bv3_coord_ref,const NUT_SPACK_THEN_ENUM then_ref)
{
	NUT_SPACK_CONSTRAINT	spc;


	// Error check:
	if(Aspline_id == NUT_SPACK_INTERNAL_VALUE)			// Operand A represents a spline point specific position value (x, y or z) only.
	{												// Not an internal value !
		NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
		return NFALSE;									
	}														
	// PreSet:
	memset(&spc,0,sizeof(NUT_SPACK_CONSTRAINT));
	spc.OperandA_SplineID		= Aspline_id;
	spc.OperandA_SplinePointID	= _SafeNs32ToNu8(Asplinepoint_ref);
	spc.OperandA_V3_Coord		= _SafeNs32ToNu8(Av3_coord_ref);

	spc.OperandB_SplineID		= Bspline_id_or_value_tag;
	spc.OperandB_SplinePointID	= _SafeNu32ToNu8(Bsplinepoint_ref_or_value_ref);
	spc.OperandB_V3_Coord		= _SafeNu32ToNu8(Bv3_coord_ref);

	spc.Test					= _SafeNu32ToNu8(test_ref);
	//spc.Flags					= 0;


	switch(then_ref)
	{
		case _THEN_OpA_EQUAL_OpB:
			// Error check:
			if(Bspline_id_or_value_tag == NUT_SPACK_INTERNAL_VALUE) // USert refers to internal value 
			{
				if(Bsplinepoint_ref_or_value_ref == _VALUE)				// Impossible to set A from "several" B
				{														// because having '_VALUE' for B means, all the Values !
					NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);			// But the same thing works fine with '_THEN_OpB_EQUAL_OpA'
					return NFALSE;										// because in that case, it's possible to set "several" B from A ! Do you copy ?
				}															
			}
			else // User refer to a specific Spline ID
			{
				if(Bsplinepoint_ref_or_value_ref == _INBETWEEN_KNOT)	// Impossible to set A from "several" B
				{														// because having '_INBETWEEN_KNOT' for B means, all the points in between Start and End !
					NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);			// But the same thing works fine with '_THEN_OpB_EQUAL_OpA'
					return NFALSE;										// because in that case, it's possible to set "several" B from A ! Do you copy ?
				}

				if(Bsplinepoint_ref_or_value_ref == _KNOT)				// Impossible to set A from "several" B
				{														// because having '_KNOT' for B means, all the points Start and End included !
					NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);			// ... so it doesn't work (like for _INBETWEEN_KNOT)
					return NFALSE;											
				}															
			}


			NArrayPushBack(&pt->Constraints_OpA_Equal_OpB,(NBYTE*)&spc);
			break;

		case _THEN_OpB_EQUAL_OpA:
			NArrayPushBack(&pt->Constraints_OpB_Equal_OpA,(NBYTE*)&spc);
			break;

		default:
			return NFALSE;
	}
	return NTRUE;
}

// ------------------------------------------------------------------------------------------
// _ApplyConstraint_A_Equal_B 
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//				SNKS_SPLINESPACK_TOOLDATA *pt		A valid pointer on a SNKS_SPLINESPACK_TOOLDATA to work with.
//		const SNKS_SPLINESPACK_CONSTRAINT *pc		A valid pointer on a SNKS_SPLINESPACK_CONSTRAINT that belongs to pt.
//					  const NSPLINE *psplineA		A valid pointer on a NSPLINE.This is the current modified Spline. 
//							NSPLINEKNOT *pspA		A valid pointer on a NSPLINEKNOT. 
//													This is the current modified SplineKnot, probably updated by user... 
//													Because we are doing _ApplyConstraint_A_Equal_B, this pspA is going to be updated from
//													opB values, accordingly with the constraint test (_DIFFERENT,SMALLER,_GREATER)
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ApplyConstraint_A_Equal_B( NUT_SPACK_TOOLDATA *pt, const NUT_SPACK_CONSTRAINT *pc, const NSPLINE *psplineA, NSPLINEKNOT *pspA )
{
	NSPLINE			*psplineB;
	NSPLINEKNOT		*pspB;
	Nf32			a;
	Nf32			*pvalueB;

	// !! OperandB can Only be unique ! (STARTPOINT or ENDPOINT or INTERNALVALUE)
	//	because it has to be clearly identified !
	if(pc->OperandB_SplineID == NUT_SPACK_INTERNAL_VALUE)
	{
		psplineB	= NULL;
		pspB		= &pt->InternalSplinePoint[pc->OperandB_SplinePointID];
	}
	else
	{
		psplineB	= *(NSPLINE**)NGetArrayPtr(&pt->ArrayOfSplinesPtr,pc->OperandB_SplineID);

		// get the splinepoint
		if(pc->OperandB_SplinePointID == _START_KNOT)
			pspB = NGetFirstSplineKnot(psplineB);
		else// if(pc->Params.OperandB_SplinePointID == _END_KNOT).. its not posible for OperandB_SplinePointID to be equal to _INBETWEEN_KNOT or _ALL_POINT. These values are forbiden at creation !!!
			pspB = NGetLastSplineKnot(psplineB);

	}
	// point the right component !
	pvalueB = &pspB->Position.coord[pc->OperandB_V3_Coord];

	switch(pc->Test)
	{
		case _DIFFERENT:
			if( pspA->Position.coord[pc->OperandA_V3_Coord] != *pvalueB )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);
				
				a = *pvalueB - pspA->Position.coord[pc->OperandA_V3_Coord];
				pspA->Position.coord[pc->OperandA_V3_Coord] += a;
				pspA->TA.coord[pc->OperandA_V3_Coord] += a;
				pspA->TB.coord[pc->OperandA_V3_Coord] += a;
				
				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);
			}
			break;

		case _GREATER:
			if( pspA->Position.coord[pc->OperandA_V3_Coord] > *pvalueB )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);

				a = *pvalueB - pspA->Position.coord[pc->OperandA_V3_Coord];
				pspA->Position.coord[pc->OperandA_V3_Coord] += a;
				pspA->TA.coord[pc->OperandA_V3_Coord] += a;
				pspA->TB.coord[pc->OperandA_V3_Coord] += a;

				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);
			}
			break;

		case _SMALLER:
			if( pspA->Position.coord[pc->OperandA_V3_Coord] < *pvalueB )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);

				a = *pvalueB - pspA->Position.coord[pc->OperandA_V3_Coord];
				pspA->Position.coord[pc->OperandA_V3_Coord] += a;
				pspA->TA.coord[pc->OperandA_V3_Coord] += a;
				pspA->TB.coord[pc->OperandA_V3_Coord] += a;

				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineA,pspA,pc->OperandA_V3_Coord,psplineB,pspB,pc->OperandB_V3_Coord);
			}
			break;
	}
}
// ------------------------------------------------------------------------------------------
// _ApplyConstraint_B_Equal_A 
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//				SNKS_SPLINESPACK_TOOLDATA *pt		A valid pointer on a SNKS_SPLINESPACK_TOOLDATA to work with.
//		const SNKS_SPLINESPACK_CONSTRAINT *pc		A valid pointer on a SNKS_SPLINESPACK_CONSTRAINT that belongs to pt.
//					  const NSPLINE *psplineA		A valid pointer on a NSPLINE.This is the current modified Spline. 
//							NSPLINEKNOT *pspA		A valid pointer on a NSPLINEKNOT. 
//													This is the current modified SplineKnot, probably updated by user... 
//													Because we are doing _ApplyConstraint_B_Equal_A, the OperandB is going to be updated from
//													this pspA values, accordingly with the constraint test (_DIFFERENT,SMALLER,_GREATER)
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ApplyConstraint_B_Equal_A( NUT_SPACK_TOOLDATA *pt, const NUT_SPACK_CONSTRAINT *pc, const NSPLINE *psplineA, NSPLINEKNOT *pspA )
{
	NSPLINE						*psplineB;
	NSPLINEKNOT					*pspB;
	Nu32						_from,_to,_endid,i;
	Nbool						single;
	Nf32						a;
	NUT_SPACK_SPLINEPOINT_ENUM	b_pointid;

	// !! OperandB can be single or Multiple !
	single = NFALSE;_from = 0;_to = 0;_endid = 0;

	if(pc->OperandB_SplineID == NUT_SPACK_INTERNAL_VALUE)
	{
		// No Mother Spline for an internal value !!! OBVIOUSLY !!!
		psplineB = NULL;

		if(pc->OperandB_SplinePointID == _VALUE)
		{
			pspB = &pt->InternalSplinePoint[0];
			_from = 0;_to = NUT_SPACK_INTERNAL_VALUE_NB;
		}
		else
		{
			single = NTRUE;
			pspB = &pt->InternalSplinePoint[pc->OperandB_SplinePointID];
		}
	}
	else
	{
		psplineB = *(NSPLINE**)NGetArrayPtr(&pt->ArrayOfSplinesPtr,pc->OperandB_SplineID); 
		switch(pc->OperandB_SplinePointID)
		{
			case _START_KNOT:
				single	= NTRUE;
				pspB	= NGetFirstSplineKnot(psplineB);
				break;
			case _END_KNOT:
				single	= NTRUE;
				pspB	= NGetLastSplineKnot(psplineB);
				break;
			case _INBETWEEN_KNOT:
				pspB	= NGetFirstSplineKnot(psplineB)+1;
				_from	= 1;
				_to		= NGetArraySize(&psplineB->KnotArray)-1;
				_endid	= NGetArraySize(&psplineB->KnotArray)-1;
				break;
			case _KNOT:
				pspB	= NGetFirstSplineKnot(psplineB);
				_from	= 0;
				_to		= NGetArraySize(&psplineB->KnotArray);
				_endid	= NGetArraySize(&psplineB->KnotArray)-1;
				break;
		}
	}

	switch(pc->Test)
	{
	case _DIFFERENT:
		if(single)
		{
			if( pspA->Position.coord[pc->OperandA_V3_Coord] != pspB->Position.coord[pc->OperandB_V3_Coord] )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
				pspB->Position.coord[pc->OperandB_V3_Coord] += a;
				pspB->TA.coord[pc->OperandB_V3_Coord] += a;
				pspB->TB.coord[pc->OperandB_V3_Coord] += a;

				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				if(psplineB) // pspB is an InternalValue if psplineB is NULL !
				{
					NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, (NUT_SPACK_SPLINEPOINT_ENUM)pc->OperandB_SplinePointID );
				}
			}
		}
		else
		{
			for(i=_from;i<_to;i++,pspB++)
			{
				if( pspA->Position.coord[pc->OperandA_V3_Coord] != pspB->Position.coord[pc->OperandB_V3_Coord] )
				{
					// CALL BACK 'PRE' CONSTRAINTS
					if(pt->PreApplyConstraints_CallBack)
						pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
					pspB->Position.coord[pc->OperandB_V3_Coord] += a;
					pspB->TA.coord[pc->OperandB_V3_Coord] += a;
					pspB->TB.coord[pc->OperandB_V3_Coord] += a;

					// CALL BACK 'POST' CONSTRAINTS
					if(pt->PostApplyConstraints_CallBack)
						pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					if(psplineB) // pspB is an InternalValue if psplineB is NULL !
					{
						if( i == 0 )
							b_pointid = _START_KNOT;
						else if( i == _endid )
							b_pointid = _END_KNOT;
						else
							b_pointid = _INBETWEEN_KNOT;

						NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, b_pointid );
					}
				}
			}
		}
		break;

	case _GREATER:
		if(single)
		{
			if( pspA->Position.coord[pc->OperandA_V3_Coord] > pspB->Position.coord[pc->OperandB_V3_Coord] )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
				pspB->Position.coord[pc->OperandB_V3_Coord] += a;
				pspB->TA.coord[pc->OperandB_V3_Coord] += a;
				pspB->TB.coord[pc->OperandB_V3_Coord] += a;

				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				if(psplineB) // pspB is an InternalValue if psplineB is NULL !
				{
					NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, (NUT_SPACK_SPLINEPOINT_ENUM)pc->OperandB_SplinePointID );
				}
			}
		}
		else
		{
			for(i=_from;i<_to;i++,pspB++)
			{
				if( pspA->Position.coord[pc->OperandA_V3_Coord] != pspB->Position.coord[pc->OperandB_V3_Coord] )
				{
					// CALL BACK 'PRE' CONSTRAINTS
					if(pt->PreApplyConstraints_CallBack)
						pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
					pspB->Position.coord[pc->OperandB_V3_Coord] += a;
					pspB->TA.coord[pc->OperandB_V3_Coord] += a;
					pspB->TB.coord[pc->OperandB_V3_Coord] += a;

					// CALL BACK 'POST' CONSTRAINTS
					if(pt->PostApplyConstraints_CallBack)
						pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					if(psplineB) // pspB is an InternalValue if psplineB is NULL !
					{
						if( i == 0 )
							b_pointid = _START_KNOT;
						else if( i == _endid )
							b_pointid = _END_KNOT;
						else
							b_pointid = _INBETWEEN_KNOT;

						NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, b_pointid );
					}
				}
			}
		}
		break;

	case _SMALLER:
		if(single)
		{
			if( pspA->Position.coord[pc->OperandA_V3_Coord] < pspB->Position.coord[pc->OperandB_V3_Coord] )
			{
				// CALL BACK 'PRE' CONSTRAINTS
				if(pt->PreApplyConstraints_CallBack)
					pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
				pspB->Position.coord[pc->OperandB_V3_Coord] += a;
				pspB->TA.coord[pc->OperandB_V3_Coord] += a;
				pspB->TB.coord[pc->OperandB_V3_Coord] += a;

				// CALL BACK 'POST' CONSTRAINTS
				if(pt->PostApplyConstraints_CallBack)
					pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

				if(psplineB) // pspB is an InternalValue if psplineB is NULL !
				{
					NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, (NUT_SPACK_SPLINEPOINT_ENUM)pc->OperandB_SplinePointID );
				}
			}
		}
		else
		{
			for(i=_from;i<_to;i++,pspB++)
			{
				if( pspA->Position.coord[pc->OperandA_V3_Coord] != pspB->Position.coord[pc->OperandB_V3_Coord] )
				{
					// CALL BACK 'PRE' CONSTRAINTS
					if(pt->PreApplyConstraints_CallBack)
						pt->PreApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					a = pspA->Position.coord[pc->OperandA_V3_Coord] - pspB->Position.coord[pc->OperandB_V3_Coord];
					pspB->Position.coord[pc->OperandB_V3_Coord] += a;
					pspB->TA.coord[pc->OperandB_V3_Coord] += a;
					pspB->TB.coord[pc->OperandB_V3_Coord] += a;

					// CALL BACK 'POST' CONSTRAINTS
					if(pt->PostApplyConstraints_CallBack)
						pt->PostApplyConstraints_CallBack(psplineB,pspB,pc->OperandB_V3_Coord,psplineA,pspA,pc->OperandA_V3_Coord);

					if(psplineB) // pspB is an InternalValue if psplineB is NULL !
					{
						if( i == 0 )
							b_pointid = _START_KNOT;
						else if( i == _endid )
							b_pointid = _END_KNOT;
						else
							b_pointid = _INBETWEEN_KNOT;

						NUT_ApplySplinesPackConstraints( pt, psplineB, pspB, b_pointid );
					}
				}
			}
		}
		break;
	}
}

// ------------------------------------------------------------------------------------------
// NUT_ApplySplinesPackConstraints
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
Nbool NUT_ApplySplinesPackConstraints(NUT_SPACK_TOOLDATA *pt, NSPLINE *pspline, NSPLINEKNOT *psp, const NUT_SPACK_SPLINEPOINT_ENUM Asplinepoint_ref )
{
	NUT_SPACK_CONSTRAINT	spc;
	NUT_SPACK_CONSTRAINT	*pspc;
	NSPLINE					**ppspline;

	Nu32					i;

	// Constraint unactivated !
	if(ISFLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_CONSTRAINT_ON))
		return NFALSE;

	// SplineKnot Already under Constraint ... Avoiding Cyclic Constraints Calls !
	if( ISFLAG_ON( psp->Flags, FLAG_NSPLINEKNOT_NUT_SPACK_CONSTRAINT_LOCK ) )
		return NFALSE;

	// Internal Values Update Call back ?
	// Call it to let user updates Internal Constraint values before using them ... 
	if( pt->InternalValues_CallBack )
	{
		pt->InternalValues_CallBack(	&pt->InternalSplinePoint[_A].Position,
										&pt->InternalSplinePoint[_B].Position,
										&pt->InternalSplinePoint[_C].Position,
										pspline,psp,Asplinepoint_ref ); 
	}

	// Lock Temporary the Spline knot to avoid cyclic constraint apply
	FLAG_ON( psp->Flags, FLAG_NSPLINEKNOT_NUT_SPACK_CONSTRAINT_LOCK );

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 1/
	// Build up a temporary "SPLINESPACK_CONSTRAINT" of OperandA
	// based on the incoming parameters ('pspline' and 'Asplinepoint_ref')
	// 
	memset(&spc,0,sizeof(NUT_SPACK_CONSTRAINT));
	ppspline = (NSPLINE**)NGetFirstArrayPtr(&pt->ArrayOfSplinesPtr);
	for(i=0;i<NGetArraySize(&pt->ArrayOfSplinesPtr);i++,ppspline++)
	{
		if( pspline == *ppspline )
			spc.OperandA_SplineID = i;
	}
	spc.OperandA_SplinePointID	= _SafeNu32ToNu8(Asplinepoint_ref);

	// 2/
	// Look for this temporary  "SPLINESPACK_CONSTRAINT" description ( of OperandA )
	// into the "Constraints_OpA_Equal_OpB" array.
	// Each time a corresponding element is found Engine use it !
	pspc = (NUT_SPACK_CONSTRAINT*)NGetFirstArrayPtr(&pt->Constraints_OpA_Equal_OpB);
	for(i=0;i<NGetArraySize(&pt->Constraints_OpA_Equal_OpB);i++,pspc++)
	{
		if(pspc->OperandA_SplineID == spc.OperandA_SplineID)
			if((pspc->OperandA_SplinePointID == spc.OperandA_SplinePointID)||(pspc->OperandA_SplinePointID == _KNOT))
				_ApplyConstraint_A_Equal_B( pt, pspc, pspline, psp );
	}
	
	// 3/
	// Look for this temporary  "SPLINESPACK_CONSTRAINT" description ( of OperandA )
	// into the "Constraints_OpB_Equal_OpA" array.
	// Each time a corresponding element is found Engine use it !
	pspc = (NUT_SPACK_CONSTRAINT*)NGetFirstArrayPtr(&pt->Constraints_OpB_Equal_OpA);
	for(i=0;i<NGetArraySize(&pt->Constraints_OpB_Equal_OpA);i++,pspc++)
	{
		if(pspc->OperandA_SplineID == spc.OperandA_SplineID)
			if((pspc->OperandA_SplinePointID == spc.OperandA_SplinePointID)||(pspc->OperandA_SplinePointID == _KNOT))
				_ApplyConstraint_B_Equal_A( pt, pspc, pspline, psp );
	}
	// 
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Remove Temporary lock
	FLAG_OFF( psp->Flags, FLAG_NSPLINEKNOT_NUT_SPACK_CONSTRAINT_LOCK );
	return NTRUE;
}

// -----------------------------------------------------------------------
#endif	//_NEDITOR