#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"
#include "../NStructure.h"
#include "../NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// static inline ... set of functions to federate the code ... a little.
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//
// Out :
// ------------------------------------------------------------------------------------------
static inline void _constraintarray_ptoi(NARRAY*pcarray,const NARRAY*pjarray)
{
	Nu32		i;
	NCONSTRAINT	*pc;

	pc = (NCONSTRAINT*)NGetFirstArrayPtr(pcarray);
	for(i=0;i<NGetArraySize(pcarray);i++,pc++)
	{
		pc->pjA = (NJOINT*)NGetArrayIndex(pjarray,(NBYTE*)pc->pjA);
		pc->pjB = (NJOINT*)NGetArrayIndex(pjarray,(NBYTE*)pc->pjB);
	}
}
static inline void _constraintarray_itop(NARRAY*pcarray,const NARRAY*pjarray)
{
	Nu32		i;
	NCONSTRAINT	*pc;

	pc = (NCONSTRAINT*)NGetFirstArrayPtr(pcarray);
	for(i=0;i<NGetArraySize(pcarray);i++,pc++)
	{
		pc->pjA = (NJOINT*)NGetArrayPtr(pjarray,(Nu32)pc->pjA);
		pc->pjB = (NJOINT*)NGetArrayPtr(pjarray,(Nu32)pc->pjB);
	}
}
static inline void _skin_ptoi(const NSTRUCTURE *ps, NSKIN* pskin,const NSTRUCTURE_ELEMENT_TYPE_ENUM _ref)
{
	const NARRAY *pref_array;

	if(pskin->common.StructureElementType == _ref)
	{
		switch(pskin->common.StructureElementType)
		{
			case NSTRUCTURE_ELEMENT_JOINT:
				pref_array = &ps->JointArray;
				break;
			case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
				pref_array = &ps->MinMaxConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
				pref_array = &ps->SimpleConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
				pref_array = &ps->SpecialConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SPRING:
				pref_array = &ps->SpringArray;
				break;
            default:
                NErrorIf(1,NERROR_INVALID_CASE);
                break;
		}

		switch(pskin->common.SkinType)
		{
			case NSKIN_TYPE_BILLBOARD:
				pskin->BillBoard.pCenter		= (NJOINT*)NGetArrayIndex(pref_array,(NBYTE*)pskin->BillBoard.pCenter);
				pskin->BillBoard.pOrient_A		= (NJOINT*)NGetArrayIndex(pref_array,(NBYTE*)pskin->BillBoard.pOrient_A);
				pskin->BillBoard.pOrient_B		= (NJOINT*)NGetArrayIndex(pref_array,(NBYTE*)pskin->BillBoard.pOrient_B);
				break;

			case NSKIN_TYPE_ROPE:
				pskin->Rope.pFirst				= (void*)NGetArrayIndex(pref_array,(NBYTE*)pskin->Rope.pFirst);
				break;

			case NSKIN_TYPE_GRID:
				pskin->Grid.pFirst				= (void*)NGetArrayIndex(pref_array,(NBYTE*)pskin->Grid.pFirst);
				break;
            default:
                NErrorIf(1,NERROR_INVALID_CASE);
                break;
		}
	}
}
static inline void _skin_itop(const NSTRUCTURE *ps, NSKIN* pskin,const NSTRUCTURE_ELEMENT_TYPE_ENUM _ref)
{
	const NARRAY *pref_array;

	if(pskin->common.StructureElementType == _ref)
	{
		switch(pskin->common.StructureElementType)
		{
			case NSTRUCTURE_ELEMENT_JOINT:
				pref_array = &ps->JointArray;
				break;
			case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
				pref_array = &ps->MinMaxConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
				pref_array = &ps->SimpleConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
				pref_array = &ps->SpecialConstraintArray;
				break;
			case NSTRUCTURE_ELEMENT_SPRING:
				pref_array = &ps->SpringArray;
				break;
            default:
                NErrorIf(1,NERROR_INVALID_CASE);
                break;
		}

		switch(pskin->common.SkinType)
		{
		case NSKIN_TYPE_BILLBOARD:
			pskin->BillBoard.pCenter		= (NJOINT*)NGetArrayPtr(pref_array,(Nu32)pskin->BillBoard.pCenter);
			pskin->BillBoard.pOrient_A		= (NJOINT*)NGetArrayPtr(pref_array,(Nu32)pskin->BillBoard.pOrient_A);
			pskin->BillBoard.pOrient_B		= (NJOINT*)NGetArrayPtr(pref_array,(Nu32)pskin->BillBoard.pOrient_B);
			break;

		case NSKIN_TYPE_ROPE:
			pskin->Rope.pFirst				= (void*)NGetArrayPtr(pref_array,(Nu32)pskin->Rope.pFirst);
			break;

		case NSKIN_TYPE_GRID:
			pskin->Grid.pFirst				= (void*)NGetArrayPtr(pref_array,(Nu32)pskin->Grid.pFirst);
			break;
         
        default:
            NErrorIf(1,NERROR_INVALID_CASE);
            break;
                
		}
	}
}
static inline void _skinarray_ptoi(NSTRUCTURE *ps,const NSTRUCTURE_ELEMENT_TYPE_ENUM _ref)
{
	Nu32	i;
	NSKIN	*pskin;

	pskin =(NSKIN*)NGetFirstArrayPtr(&ps->SkinArray);
	for(i=0;i<NGetArraySize(&ps->SkinArray);i++,pskin++)
	{
		_skin_ptoi(ps, pskin,_ref);
	}
}
static inline void _skinarray_itop(NSTRUCTURE *ps,const NSTRUCTURE_ELEMENT_TYPE_ENUM _ref)
{
	Nu32	i;
	NSKIN	*pskin;

	pskin =(NSKIN*)NGetFirstArrayPtr(&ps->SkinArray);
	for(i=0;i<NGetArraySize(&ps->SkinArray);i++,pskin++)
	{
		_skin_itop(ps, pskin,_ref);
	}
}
static inline void _convertjointptr_to_jointindex(NSTRUCTURE *ps)
{
	// Constraint conversion
	_constraintarray_ptoi(&ps->SpringArray,&ps->JointArray);
	_constraintarray_ptoi(&ps->SimpleConstraintArray,&ps->JointArray);
	_constraintarray_ptoi(&ps->MinMaxConstraintArray,&ps->JointArray);
	_constraintarray_ptoi(&ps->SpecialConstraintArray,&ps->JointArray);
	
	//Skin conversion
	_skinarray_ptoi(ps,NSTRUCTURE_ELEMENT_JOINT);
}
static inline void  _convertjointindex_to_jointptr(NSTRUCTURE *ps)
{
	// Constraint conversion
	_constraintarray_itop(&ps->SpringArray,&ps->JointArray);
	_constraintarray_itop(&ps->SimpleConstraintArray,&ps->JointArray);
	_constraintarray_itop(&ps->MinMaxConstraintArray,&ps->JointArray);
	_constraintarray_itop(&ps->SpecialConstraintArray,&ps->JointArray);

	//Skin conversion
	_skinarray_itop(ps,NSTRUCTURE_ELEMENT_JOINT);
}
static inline NARRAY* _extract_target_constraint_array(const NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder)
{
	if(builder)
	{
		switch(builder->Constraint_Type)
		{
		case NSTRUCTURE_ELEMENT_SPRING:
			return  (NARRAY*)&ps->SpringArray;

		case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
			return (NARRAY*)&ps->SimpleConstraintArray;

		case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
			return (NARRAY*)&ps->SpecialConstraintArray;

		case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
			return (NARRAY*)&ps->MinMaxConstraintArray;
		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			return NULL;
		}
	}
	else
	{
		return (NARRAY*)&ps->SimpleConstraintArray;
	}
}
static inline void _build_spring_strip(NARRAY *pconstraint_array, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 offset_to_next_joint, NCONSTRAINT *pcdraft )
{
	Nu32		i;

	pcdraft->fFriction	= builder->fFriction;
	pcdraft->fK			= builder->fK;

	if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
	{
		NVEC3	v;

		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NVec3Sub(&v,&pcdraft->pjB->CurrentPosition,&pcdraft->pjA->CurrentPosition);
			pcdraft->fRestLength	= NVec3Length(&v)*builder->fRestLength;
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
	else
	{
		pcdraft->fRestLength	= builder->fRestLength;
		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
}
static inline void _build_simpleconstraint_strip(NARRAY *pconstraint_array, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 offset_to_next_joint, NCONSTRAINT *pcdraft )
{
	Nu32		i;
	NVEC3	v;
	
	if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
	{

		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NVec3Sub(&v,&pcdraft->pjB->CurrentPosition,&pcdraft->pjA->CurrentPosition);
			pcdraft->fLength	= NVec3Length(&v) * builder->fLength;
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
	else
	{
		pcdraft->fLength	= NVec3Length(&v) * builder->fLength;
		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
}

static inline void _build_specialconstraint_strip(NARRAY *pconstraint_array, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 offset_to_next_joint, NCONSTRAINT *pcdraft )
{
	Nu32		i;

	if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
	{
		NVEC3	v;
		Nf32		dist;

		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NVec3Sub(&v,&pcdraft->pjB->CurrentPosition,&pcdraft->pjA->CurrentPosition);
			dist = NVec3Length(&v);
			pcdraft->fMinLength	= dist * builder->fMinLength;
			pcdraft->fMaxLength	= dist * builder->fMaxLength;
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
	else
	{
		pcdraft->fMinLength	= builder->fMinLength;
		pcdraft->fMaxLength	= builder->fMaxLength;
		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
}

static inline void _build_minmaxconstraint_strip(NARRAY *pconstraint_array, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 offset_to_next_joint, NCONSTRAINT *pcdraft )
{
	Nu32		i;

	if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
	{
		NVEC3	v;
		Nf32		dist;

		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NVec3Sub(&v,&pcdraft->pjB->CurrentPosition,&pcdraft->pjA->CurrentPosition);
			dist = NVec3Length(&v);
			pcdraft->fMinLength	= dist * builder->fMinLength;
			pcdraft->fMaxLength	= dist * builder->fMaxLength;
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
	else
	{
		pcdraft->fMinLength	= builder->fMinLength;
		pcdraft->fMaxLength	= builder->fMaxLength;
		for(i=number_of_constraints;i!=0;i--,pcdraft->pjA +=offset_to_next_joint,pcdraft->pjB +=offset_to_next_joint)
		{
			NArrayPushBack( pconstraint_array,(NBYTE*)pcdraft );
		}
	}
}
// ------------------------------------------------------------------------------------------
// void NCreateJoint
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
void NCreateJoint(NSTRUCTURE *ps, const NJOINT_DESC *builder)
{
	NJOINT		jointdraft;
	Nbool		repoint;
	
	// note:	NJOINT_DESC.shift_Vector will stay unused here.
	//			This parameter is only used for a range creation.

	// !!! FIRST OF ALL: 
	// We want to know if this Joint Creation may cause a memory reallocation.
	// In that case we have to anticipate the full reconnection of all 
	// the NJOINT* included into constraint and skin
	if( (NGetArraySize(&ps->JointArray)+1) > NGetArrayCapacity(&ps->JointArray))
	{
		repoint = NTRUE;
		_convertjointptr_to_jointindex(ps);
	}
	else
		repoint = NFALSE;

	if(builder)
	{
		jointdraft.fRadius	= builder->fRadius;
		jointdraft.fMass	= builder->fMass;
		jointdraft.vSpeed = builder->vSpeed;
		NVec3Set(&jointdraft.vForcesAccumulator,0,0,0);
		jointdraft.CurrentPosition = builder->CurrentPosition;
		jointdraft.OldPosition = builder->OldPosition;
	}
	else
	{
		memset(&jointdraft,0,sizeof(NJOINT));
		jointdraft.fRadius	= DEFAULT_JOINT_RADIUS;
		jointdraft.fMass	= DEFAULT_JOINT_MASS;
	}

	NArrayPushBack(&ps->JointArray,(NBYTE*)&jointdraft);

	if(repoint)
		_convertjointindex_to_jointptr(ps);
}
// ------------------------------------------------------------------------------------------
// void NCreateJointsRange
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
void NCreateJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints, const NJOINT_DESC *builder,const NVEC3 *pshift)
{
	Nu32		 i;
	NJOINT		jointdraft;
	NVEC3	vshift;
	Nbool		repoint;

	// !!! FIRST OF ALL: 
	// We want to know if this Joint Creation may cause a memory reallocation.
	// In that case we have to anticipate the full reconnection of all 
	// the NJOINT* included into constraint and skin
	if( (NGetArraySize(&ps->JointArray)+number_of_joints) > NGetArrayCapacity(&ps->JointArray))
	{
		repoint = NTRUE;
		_convertjointptr_to_jointindex(ps);
	}
	else
		repoint = NFALSE;

	if(builder)
	{
		jointdraft.fRadius	= builder->fRadius;
		jointdraft.fMass	= builder->fMass;
		jointdraft.vSpeed = builder->vSpeed;
		NVec3Set(&jointdraft.vForcesAccumulator,0,0,0);
		jointdraft.CurrentPosition = builder->CurrentPosition;
		jointdraft.OldPosition = builder->OldPosition;
	}
	else
	{
		memset(&jointdraft,0,sizeof(NJOINT));
		jointdraft.fRadius	= DEFAULT_JOINT_RADIUS;
		jointdraft.fMass	= DEFAULT_JOINT_MASS;
	}

	if(pshift)
	{
		vshift = *pshift;
	}
	else
	{	
		NVec3Set(&vshift,0,0,0);
	}

	for(i=0;i<number_of_joints;i++)
	{
		NArrayPushBack(&ps->JointArray,(NBYTE*)&jointdraft);

		NVec3AddTo(&jointdraft.CurrentPosition,&vshift);
		NVec3AddTo(&jointdraft.OldPosition,&vshift);
	}

	if(repoint)
		_convertjointindex_to_jointptr(ps);
}
// ------------------------------------------------------------------------------------------
// NCONSTRAINT* NCreateConstraint
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
NCONSTRAINT* NCreateConstraint(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 indexA, const Nu32 indexB)
{
	NCONSTRAINT		csdraft;
	NCONSTRAINT		*pc;
	NARRAY			*parray;
	NVEC3		v;
	Nf32			dist;

	// =====================================================================================
	// First of All: fill up the NCONSTRAINT structure with 0
	// =====================================================================================
	memset(&csdraft,0,sizeof(NCONSTRAINT));

	// =====================================
	// Get a pointer on each of the 2 JOINTS
	// =====================================
	csdraft.pjA = (NJOINT*)NGetArrayPtr(&ps->JointArray,indexA);
	csdraft.pjB = (NJOINT*)NGetArrayPtr(&ps->JointArray,indexB);

	// =====================================================================================
	// Prepare the common part of the draft which will be copied for the created constraint
	// =====================================================================================
	if(builder)
	{
		switch(builder->Constraint_Type)
		{
			case NSTRUCTURE_ELEMENT_SPRING:
				parray = &ps->SpringArray;
				
				csdraft.fFriction	= builder->fFriction;
				csdraft.fK			= builder->fK;

				if( ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST) )
				{
					NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
					csdraft.fRestLength	= NVec3Length(&v) * builder->fRestLength;
				}
				else
				{
					csdraft.fRestLength	= builder->fRestLength;
				}
				break;

			case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
				parray = &ps->SimpleConstraintArray;
				
				if( ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST) )
				{
					NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
					csdraft.fLength	= NVec3Length(&v) * builder->fLength;
				}
				else
				{
					csdraft.fLength = builder->fLength;
				}
				break;

			case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
				parray = &ps->SpecialConstraintArray;

				if( ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST) )
				{
					NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
					dist = NVec3Length(&v);

					csdraft.fMinLength = dist * builder->fMinLength;
					csdraft.fMaxLength = dist * builder->fMaxLength;
				}
				else
				{
					csdraft.fMinLength = builder->fMinLength;
					csdraft.fMaxLength = builder->fMaxLength;
				}
				break;

			case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
				parray = &ps->MinMaxConstraintArray;

				if( ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST) )
				{
					NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
					dist = NVec3Length(&v);

					csdraft.fMinLength = dist * builder->fMinLength;
					csdraft.fMaxLength = dist * builder->fMaxLength;
				}
				else
				{
					csdraft.fMinLength = builder->fMinLength;
					csdraft.fMaxLength = builder->fMaxLength;
				}
				break;
            default:
                NErrorIf(1,NERROR_INVALID_CASE);
                break;
			}
	}
	else
	{
		// without a builder we assume constraints created are going to be simple constraints.
		parray = &ps->SimpleConstraintArray;
		csdraft.fLength = DEFAULT_CONSTRAINT_LENGTH;
	}
	
	// !!! 
	// If this Constraint Creation may cause a memory reallocation.
	// then we have to anticipate the full reconnection of all 
	// the NCONSTRAINT* included into skin
	if( NGetArraySize(parray)==NGetArrayCapacity(parray) )
	{
		_skinarray_ptoi(ps,builder->Constraint_Type);
		pc = (NCONSTRAINT*)NArrayPushBack(parray,(NBYTE*)&csdraft);
		_skinarray_itop(ps,builder->Constraint_Type);
	}
	else
	{
		pc = (NCONSTRAINT*)NArrayPushBack(parray,(NBYTE*)&csdraft);
	}
	return pc;
}

// ------------------------------------------------------------------------------------------
// void NCreateConstraintStrip
// ------------------------------------------------------------------------------------------
// Description :
//	Create a range of constraint as a strip. 
//	In that scheme a range of successive joint ( successive by their index in the "JointsArray")
//	is going to be connected by successive constraints:
//
//	+----------+----------+----------+----- - - -  -  -   -  -
//	J.0		   J.1		  J.2		 J.n
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NCreateConstraintStrip(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 firstjoint_index, const Nu32 offset_to_next_joint )
{
	NCONSTRAINT				csdraft;
	NARRAY					*parray;
	NJOINT					*pjoint;
	Nbool					repoint;
	NCONSTRAINT_DESC		default_builder;

	NErrorIf(!offset_to_next_joint,NERROR_WRONG_VALUE);
	NErrorIf(!number_of_constraints,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_CONSTRAINTS);
	NErrorIf(firstjoint_index+number_of_constraints*offset_to_next_joint >= NGetNbJoints(ps),NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS);

	// =====================================================================================
	// First of All: fill up the NCONSTRAINT structure with 0
	// =====================================================================================
	memset(&csdraft,0,sizeof(NCONSTRAINT));

	// =====================================================================================
	// CREATE the Constraints following the primitive scheme
	// =====================================================================================
	 parray = _extract_target_constraint_array(ps,builder);

	// =====================================================
	// Verify if memory is going to be reallocated
	 // !!! 
	 // If this Constraint Creation may cause a memory reallocation
	 // then we have to anticipate the full reconnection of all 
	 // the NCONSTRAINT* included into skin
	// =====================================================
	if( (NGetArraySize(parray)+number_of_constraints) > NGetArrayCapacity(parray) )
	{
		repoint = NTRUE;
		_skinarray_ptoi(ps,builder->Constraint_Type);
	}
	else
	{
		repoint = NFALSE;
	}
	// =====================================================
	// Verify if there is enough Joints to create this RANGE
	// =====================================================
	// Purpose of this function is not JOINT creation so if it seems that
	// the number of joints is to low we quit without creating the constraints
	NErrorIf( (firstjoint_index + number_of_constraints + 1) > NGetArraySize(&ps->JointArray),NERROR_VALUE_OUTOFRANGE );
	// ======================
	// Create the constraints
	// ======================
	pjoint = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
	Nmem0(&csdraft,NCONSTRAINT);
	csdraft.pjA = &pjoint[firstjoint_index];
	csdraft.pjB = csdraft.pjA+offset_to_next_joint;

	if(builder)
	{
		switch(builder->Constraint_Type)
		{
			case NSTRUCTURE_ELEMENT_SPRING:
				_build_spring_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
				break;

			case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
				_build_simpleconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
				break;

			case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
				_build_specialconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
				break;

			case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
				_build_minmaxconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
				break;

		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			break;
		}
	}
	else
	{
		// without a builder we assume constraints created are going to be simple constraints without _RANGE_LOOP
		Nmem0(&default_builder,NCONSTRAINT_DESC);
		default_builder.Constraint_Type = NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT;
		default_builder.fLength = DEFAULT_CONSTRAINT_LENGTH;
		FLAG_ON(default_builder.Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST);
		_build_simpleconstraint_strip(parray,&default_builder,number_of_constraints,offset_to_next_joint,&csdraft );
	}

	if(repoint)
		_skinarray_itop(ps,builder->Constraint_Type);

}
// ------------------------------------------------------------------------------------------
// void NCreateConstraintLoop
// ------------------------------------------------------------------------------------------
// Description :
//	Create a range of constraint as a loop. 
//	In that scheme a range of successive joint ( successive by their index in the "JointsArray")
//	is going to be connected by successive constraints and the last one (of the range)is going to be connected 
//	to the first one, creating a loop by this way.
//	   J.0	   J.1	   J.2
//		+-------+-------+
//		|				|
//		|				|
//		|				|
//	J.n	+				+ J.3
//		|				|
//		|				|
//		|				|
//		+-------+-------+
//	   J.6	   J.5	   J.4

// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NCreateConstraintLoop(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 firstjoint_index, const Nu32 offset_to_next_joint )
{
	NCONSTRAINT			csdraft;
	NARRAY				*parray;
	NJOINT				*pjoint;
	Nf32				dist;
	NVEC3			v;
	Nbool				repoint;
	NCONSTRAINT_DESC	default_builder;

	NErrorIf(!offset_to_next_joint,NERROR_WRONG_VALUE);
	NErrorIf(number_of_constraints < 2,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_CONSTRAINTS); // at least 2 constraint to make a loop.
	NErrorIf(firstjoint_index+number_of_constraints*offset_to_next_joint>=NGetNbJoints(ps),NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS);

	// =====================================================================================
	// First of All: fill up the NCONSTRAINT structure with 0
	// =====================================================================================
	memset(&csdraft,0,sizeof(NCONSTRAINT));

	// =====================================================================================
	// CREATE the Constraints following the primitive scheme
	// =====================================================================================
	parray = _extract_target_constraint_array(ps,builder);

	// =====================================================
	// Verify if memory is going to be reallocated
	// !!! 
	// If this Constraint Creation may cause a memory reallocation.
	// then we have to anticipate the full reconnection of all 
	// the NCONSTRAINT* included into skin
	// =====================================================
	if( (NGetArraySize(parray)+number_of_constraints) > NGetArrayCapacity(parray) )
	{
		repoint = NTRUE;
		_skinarray_ptoi(ps,builder->Constraint_Type);
	}
	else
	{
		repoint = NFALSE;
	}
	// =====================================================
	// Verify if there is enough Joints to create this RANGE
	// ( Because it's a loop, number of need joints is equal to
	//   number of constraints )
	// =====================================================
	NErrorIf( (firstjoint_index + number_of_constraints ) > NGetArraySize(&ps->JointArray), NERROR_VALUE_OUTOFRANGE );
	// ======================
	// Create the constraints
	// ======================
	pjoint = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
	Nmem0(&csdraft,NCONSTRAINT);
	csdraft.pjA = &pjoint[firstjoint_index];
	csdraft.pjB = csdraft.pjA+offset_to_next_joint;

	if(builder)
	{
		switch(builder->Constraint_Type)
		{
		case NSTRUCTURE_ELEMENT_SPRING:
			_build_spring_strip(parray,builder,number_of_constraints-1,offset_to_next_joint,&csdraft );
			// One more to LOOP
			csdraft.pjB = &pjoint[firstjoint_index];
			if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
			{
				NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
				csdraft.fRestLength	= NVec3Length(&v)*builder->fRestLength;
			}
			NArrayPushBack( parray,(NBYTE*)&csdraft );
			break;

		case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
			_build_simpleconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
			// One more to LOOP
			csdraft.pjB = &pjoint[firstjoint_index];
			if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
			{
				NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
				csdraft.fLength	= NVec3Length(&v) * builder->fLength;
			}
			NArrayPushBack( parray,(NBYTE*)&csdraft );
			break;

		case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
			_build_specialconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
			// One more to LOOP
			csdraft.pjB = &pjoint[firstjoint_index];
			if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
			{
				NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
				dist = NVec3Length(&v);
				csdraft.fMinLength	= dist * builder->fMinLength;
				csdraft.fMaxLength	= dist * builder->fMaxLength;
			}
			NArrayPushBack( parray,(NBYTE*)&csdraft );
			break;

		case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
			_build_minmaxconstraint_strip(parray,builder,number_of_constraints,offset_to_next_joint,&csdraft );
			// One more to LOOP
			csdraft.pjB = &pjoint[firstjoint_index];
			if(ISFLAG_ON(builder->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
			{
				NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
				dist = NVec3Length(&v);
				csdraft.fMinLength	= dist * builder->fMinLength;
				csdraft.fMaxLength	= dist * builder->fMaxLength;
			}
			NArrayPushBack( parray,(NBYTE*)&csdraft );
			break;

		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			break;
		}
	}
	else
	{
		// without a builder we assume constraints created are going to be simple constraints without _RANGE_LOOP
		Nmem0(&default_builder,NCONSTRAINT_DESC);
		default_builder.Constraint_Type = NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT;
		default_builder.fLength = DEFAULT_CONSTRAINT_LENGTH;
		FLAG_ON(default_builder.Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST);
		_build_simpleconstraint_strip(parray,&default_builder,number_of_constraints,offset_to_next_joint,&csdraft );
		
		// One more to LOOP
		csdraft.pjB = &pjoint[firstjoint_index];
		NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
		csdraft.fLength	= NVec3Length(&v) * default_builder.fLength;
		NArrayPushBack( parray,(NBYTE*)&csdraft );
	}

	if(repoint)
		_skinarray_itop(ps,builder->Constraint_Type);
}

// ------------------------------------------------------------------------------------------
// void NCreateConstraintPattern
// ------------------------------------------------------------------------------------------
// Description :
//  a pattern is a list of "relative index pair" (2 by 2 relative to a "first index"  )
//	and an offset to the next first and a repetition number...  
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NCreateConstraintPattern(NSTRUCTURE *ps, const NCONSTRAINT_DESC *pcdesc, const NCONSTRAINT_PATTERN *ppattern, const Nu32 pattern_size,const Nu32 offset_to_next_pattern, const Nu32 repetition, const Nu32 joint_index_origin )
{
	Nu32						i,j;
	Nu32						base;

	NCONSTRAINT					csdraft;
	NARRAY						*parray;
	NJOINT						*pjoint;
	Nf32						dist;
	NVEC3					v;
	Nbool						repoint;
	NCONSTRAINT_PATTERN			*ppattern_element;

	NErrorIf(!ppattern,NERROR_NULL_POINTER);
	NErrorIf(!repetition ,NERROR_NULL_VALUE); // At least "repetition" should be equal to 1.
	NErrorIf(!pattern_size ,NERROR_NULL_VALUE); // At least "pattern_size" should be equal to 1.
	NErrorIf(!offset_to_next_pattern ,NERROR_NULL_VALUE); // At least "offset_to_next_pattern" should be equal to 1.
	NErrorIf(joint_index_origin >= ps->JointArray.Size,NERROR_INDEX_OUTOFRANGE );
	// =====================================================================================
	// First of All: fill up the NCONSTRAINT structure with 0
	// =====================================================================================
	memset(&csdraft,0,sizeof(NCONSTRAINT));

	// =====================================================================================
	// CREATE the Constraints following the primitive scheme
	// =====================================================================================
	parray = _extract_target_constraint_array(ps,pcdesc);

	// =====================================================
	// Verify if memory is going to be reallocated
	// !!! 
	// If this Constraint Creation may cause a memory reallocation.
	// then we have to anticipate the full reconnection of all 
	// the NCONSTRAINT* included into skin
	// =====================================================
	if( ( NGetArraySize(parray)+pattern_size*repetition ) > NGetArrayCapacity(parray) )
	{
		repoint = NTRUE;
		_skinarray_ptoi(ps,pcdesc->Constraint_Type);
	}
	else
	{
		repoint = NFALSE;
	}
	// =====================================================
	// Verify if there is enough Joints to create these pattern
	// =====================================================
#ifdef _DEBUG
	// todo
#endif
	// ======================
	// Create the constraints
	// ======================
	pjoint = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
	Nmem0(&csdraft,NCONSTRAINT);

	if(pcdesc)
	{
		switch(pcdesc->Constraint_Type)
		{
			case NSTRUCTURE_ELEMENT_SPRING:
				csdraft.fFriction	= pcdesc->fFriction;
				csdraft.fK			= pcdesc->fK;

				if(ISFLAG_ON(pcdesc->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
				{
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
							csdraft.fRestLength	= NVec3Length(&v)*pcdesc->fRestLength;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				else
				{
					csdraft.fRestLength	= pcdesc->fRestLength;
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				break;

			case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
				if(ISFLAG_ON(pcdesc->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
				{
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
							csdraft.fLength	= NVec3Length(&v)*pcdesc->fLength;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				else
				{
					csdraft.fLength	= pcdesc->fLength;
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				break;

			case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
				if(ISFLAG_ON(pcdesc->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
				{
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
							dist = NVec3Length(&v);
							csdraft.fMinLength	= dist * pcdesc->fMinLength;
							csdraft.fMaxLength	= dist * pcdesc->fMaxLength;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				else
				{
					csdraft.fMinLength	= pcdesc->fMinLength;
					csdraft.fMaxLength	= pcdesc->fMaxLength;
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				break;

			case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
				if(ISFLAG_ON(pcdesc->Flags,FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST))
				{
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
							dist = NVec3Length(&v);
							csdraft.fMinLength	= dist * pcdesc->fMinLength;
							csdraft.fMaxLength	= dist * pcdesc->fMaxLength;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				else
				{
					csdraft.fMinLength	= pcdesc->fMinLength;
					csdraft.fMaxLength	= pcdesc->fMaxLength;
					base = joint_index_origin;
					for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
					{
						ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
						for(j=pattern_size;j!=0;j--,ppattern_element++)
						{
							csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
							csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
							NArrayPushBack( parray,(NBYTE*)&csdraft );
						}
					}
				}
				break;

			default:
				NErrorIf(1,NERROR_INVALID_CASE);
				break;
		}
	}
	else
	{
		// without 'pcdesc' we assume constraints created are going to be simple constraints with ..._AS_RATIO set to ON
		base = joint_index_origin;
		for( i=repetition;i-=0;i--,base+=offset_to_next_pattern )
		{
			ppattern_element = (NCONSTRAINT_PATTERN*)ppattern;
			for(j=pattern_size;j!=0;j--,ppattern_element++)
			{
				csdraft.pjA = pjoint+base+ppattern_element->JointAIndex;
				csdraft.pjB = pjoint+base+ppattern_element->JointBIndex;
				NVec3Sub(&v,&csdraft.pjB->CurrentPosition,&csdraft.pjA->CurrentPosition);
				csdraft.fLength	= NVec3Length(&v)*DEFAULT_CONSTRAINT_LENGTH;
				NArrayPushBack( parray,(NBYTE*)&csdraft );
			}
		}
	}

	if(repoint)
		_skinarray_itop(ps,pcdesc->Constraint_Type);
}
// ------------------------------------------------------------------------------------------
// void NCreateConstraintPatternSeries
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
void NCreateConstraintPatternSeries(NSTRUCTURE *ps, const NCONSTRAINT_DESC *pcdesc, const NCONSTRAINT_PATTERN *ppattern, const Nu32 pattern_size,const Nu32 offset_to_next_pattern, const Nu32 repetition, const Nu32 joint_index_origin, const Nu32 serie_offset_to_next_joint_index_origin, const Nu32 serie_number )
{
	Nu32	a;
	Nu32	joint_origin = joint_index_origin;
	for(a=serie_number;a!=0;a--,joint_origin+=serie_offset_to_next_joint_index_origin)
		NCreateConstraintPattern(ps,pcdesc,ppattern,pattern_size,offset_to_next_pattern,repetition,joint_origin);
}
// ------------------------------------------------------------------------------------------
// void NSetStructureJointRangeRadius
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
void NSetStructureJointRangeRadius(NJOINT*pfirstjoint,const Nu32 range_size,const Nf32 scale_factor, const Nf32 minradius, const Nf32 maxradius, const NFASTANIMATEDVALUE*ptable)
{
	Nu32	i;
	NJOINT	*pj;

	pj=pfirstjoint;
	for(i=0;i<range_size;i++)
	{
		pj->fRadius = NReadFastAnimatedValueByIndex(ptable,(Nu16)i)*scale_factor;
		if(pj->fRadius<minradius)
			pj->fRadius=minradius;
		if(pj->fRadius>maxradius)
			pj->fRadius=maxradius;

		pj++;
	}
}
// ------------------------------------------------------------------------------------------
// void NFreezeJointsRange
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
void NFreezeJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints_first_included, const Nu32 firstjoint, const Ns32 stride )
{

	NErrorIf(!stride,NERROR_WRONG_VALUE);
	NErrorIf(!number_of_joints_first_included,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS);
	NErrorIf(firstjoint+stride*(number_of_joints_first_included-1)>=NGetNbJoints(ps),NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS );
	NErrorIf(firstjoint+stride*(number_of_joints_first_included-1)< 0,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS );
	
	NJOINT	*pj;
	Nu32	a;
	
	pj=(NJOINT*)NGetArrayPtr(&ps->JointArray,firstjoint);
	for(a=number_of_joints_first_included;a!=0;a--,pj+=stride)
	{
		NFreezeJoint(pj);
	}
}
// ------------------------------------------------------------------------------------------
// void NUnfreezeJointsRange
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
void NUnfreezeJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints_first_included, const Nu32 firstjoint, const Ns32 stride )
{

	NErrorIf(!stride,NERROR_WRONG_VALUE);
	NErrorIf(!number_of_joints_first_included,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS);
	NErrorIf(firstjoint+stride*(number_of_joints_first_included-1)>=NGetNbJoints(ps),NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS );
	NErrorIf(firstjoint+stride*(number_of_joints_first_included-1)< 0,NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS );

	NJOINT	*pj;
	Nu32	a;

	pj=(NJOINT*)NGetArrayPtr(&ps->JointArray,firstjoint);
	for(a=number_of_joints_first_included;a!=0;a--,pj+=stride)
	{
		NUnfreezeJoint(pj);
	}
}
// ------------------------------------------------------------------------------------------
// Nu32	NFreezeJointsChunk
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
Nu32	NFreezeJointsChunk(NSTRUCTURE *ps,const NJOINTSCHUNK *pchunk)
{
	NJOINT		*pj;
	Nu32		i;
	Nu32		nb = 0;
	NVEC3	v;
	Nf32		f;

	switch(pchunk->Type)
	{
		case NJOINTSCHUNK_TYPE_INSPHERE:
			pj	= (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
			f	= pchunk->Sphere.fRadius*pchunk->Sphere.fRadius;
			for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
			{
				NVec3Sub(&v,&pj->CurrentPosition,&pchunk->Sphere.Center);
				if(NVec3SquareLength(&v)<=f)
				{
					NFreezeJoint(pj);
					nb++;
				}
			}
			return nb;

		case NJOINTSCHUNK_TYPE_INAABB:
			pj = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
			for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
			{
				if( NIsVector3IncludedIntoAABB(&pj->CurrentPosition,&pchunk->AABB) )
				{
					NFreezeJoint(pj);
					nb++;
				}
			}
			return nb;

		case NJOINTSCHUNK_TYPE_RIGHTSIDEOFPLANE:
			pj = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
			for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
			{
				NVec3Sub(&v,&pj->CurrentPosition,&pchunk->Plane.Origin);
				if(NVec3DotProduct(&v,&pchunk->Plane.DirectorVector)>=0.0f)
				{
					NFreezeJoint(pj);
					nb++;
				}
			}
			return nb;

		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			return 0;
	}

//	return 0;
}
// ------------------------------------------------------------------------------------------
// Nu32	NUnfreezeJointsChunk
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
Nu32	NUnfreezeJointsChunk(NSTRUCTURE *ps,const NJOINTSCHUNK *pchunk)
{
	NJOINT		*pj;
	Nu32		i;
	Nu32		nb = 0;
	NVEC3	v;
	Nf32		f;

	switch(pchunk->Type)
	{
	case NJOINTSCHUNK_TYPE_INSPHERE:
		pj	= (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
		f	= pchunk->Sphere.fRadius*pchunk->Sphere.fRadius;
		for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
		{
			NVec3Sub(&v,&pj->CurrentPosition,&pchunk->Sphere.Center);
			if(NVec3SquareLength(&v)<=f)
			{
				NUnfreezeJoint(pj);
				nb++;
			}
		}
		return nb;

	case NJOINTSCHUNK_TYPE_INAABB:
		pj = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
		for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
		{
			if( NIsVector3IncludedIntoAABB(&pj->CurrentPosition,&pchunk->AABB) )
			{
				NUnfreezeJoint(pj);
				nb++;
			}
		}
		return nb;

	case NJOINTSCHUNK_TYPE_RIGHTSIDEOFPLANE:
		pj = (NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
		for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++)
		{
			NVec3Sub(&v,&pj->CurrentPosition,&pchunk->Plane.Origin);
			if(NVec3DotProduct(&v,&pchunk->Plane.DirectorVector)>=0.0f)
			{
				NUnfreezeJoint(pj);
				nb++;
			}
		}
		return nb;

	default:
		NErrorIf(1,NERROR_INVALID_CASE);
		return 0;
	}

//	return 0;
}
// ------------------------------------------------------------------------------------------
// void NUseCurrentSpringsLengthAsRestLength
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
void NUseCurrentSpringsLengthAsRestLength(NSTRUCTURE *pstructure, const Nf32 length_ratio)
{
	Nu32		i;
	NCONSTRAINT	*pspring;
	NVEC3	v;

	pspring = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SpringArray);
	for(i=NGetArraySize(&pstructure->SpringArray);i!=0;i--,pspring++)
	{
		NVec3Sub(&v,&pspring->pjB->CurrentPosition,&pspring->pjA->CurrentPosition);
		pspring->fRestLength =  NVec3Length(&v)*length_ratio;
	}
}
// ------------------------------------------------------------------------------------------
// void NUseCurrentSimpleConstraintLengthAsLength
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
void NUseCurrentSimpleConstraintLengthAsLength(NSTRUCTURE *pstructure, const Nf32 length_ratio)
{
	Nu32		i;
	NCONSTRAINT	*pc;
	NVEC3	v;

	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SimpleConstraintArray);
	for(i=NGetArraySize(&pstructure->SimpleConstraintArray);i!=0;i--,pc++)
	{
		NVec3Sub(&v,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
		pc->fLength =  NVec3Length(&v)*length_ratio;
	}
}
// ------------------------------------------------------------------------------------------
// void NUseCurrentSpecialConstraintLengthAsLengths
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
void NUseCurrentSpecialConstraintLengthAsLengths(NSTRUCTURE *pstructure, const Nf32 length_minratio,const Nf32 length_maxratio)
{
	Nu32		i;
	NCONSTRAINT	*pc;
	NVEC3	v;
	Nf32		f;

	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->SpecialConstraintArray);
	for(i=0;i<NGetArraySize(&pstructure->SpecialConstraintArray);i++,pc++)
	{
		NVec3Sub(&v,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
		f =  NVec3Length(&v);
		pc->fMinLength = f*length_minratio;
		pc->fMaxLength = f*length_maxratio;
	}
}
// ------------------------------------------------------------------------------------------
// void NUseCurrentMinMaxConstraintLengthAsLengths
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
void NUseCurrentMinMaxConstraintLengthAsLengths(NSTRUCTURE *pstructure, const Nf32 length_minratio,const Nf32 length_maxratio)
{
	Nu32		i;
	NCONSTRAINT	*pc;
	NVEC3	v;
	Nf32		f;

	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&pstructure->MinMaxConstraintArray);
	for(i=0;i<NGetArraySize(&pstructure->MinMaxConstraintArray);i++,pc++)
	{
		NVec3Sub(&v,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
		f =  NVec3Length(&v);
		pc->fMinLength = f*length_minratio;
		pc->fMaxLength = f*length_maxratio;
	}
}