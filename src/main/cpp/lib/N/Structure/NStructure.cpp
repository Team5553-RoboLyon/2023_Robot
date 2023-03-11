#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/Containers/NXNode.h"
#include "lib/N/NPostUpdate.h"

#include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "lib/N/Render/NFrustum_Culling.h"
#include "lib/N/NCore.h"
#include "lib/N/NErrorHandling.h"

#include "lib/N/NStructureForceField.h"
#include "lib/N/NStructure.h"

extern NXNODELIST *NpActivePhysicStructureCollisionCoupleList; // Global define into "NStructureCollision.cpp" for engine internal use only.
extern NARRAY NActivePhysicStructure_ForceFields_Array;		   // Global define into "NStructureForceField.cpp" for engine internal use only.
void _skindestructor_callback(void *pel);					   // Function defined into the "NStructureSkin.cpp" for an engine internal use only.

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									GLOBAL VALUES									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// NNODELIST			NActivePhysicStructureList;
// NNODELIST			NActivePhysicStructureCollisionCoupleList;

NARRAY NActivePhysicStructuresPtr_Array;
Nf32 fSimulationTimeStep;
Nf32 fAirFriction;
// NNODELIST	SkinList;

/*
float GetAirFriction()
{
	return fAirFriction;
}
*/
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									FUNCTIONS										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// POST UPDATE PROCESS
// =======================================================================================
/*
There are 2 ways to manage PhysicStructures updating.
You can perform update by your own or use the PostUpdate process manager.
	1)To perform it by yourself: use when you want the 'NPhysicStructures_Update' function.

	2)To use the postupdate process mechanism, register a PhysicStructures Post-Update, include "NPostUpdatePresets.h"
	in your file and make a call of 'NCreatePostUpdateProcess_PhysicStructures' to create it.
	Delete it by a call of 'NDeletePostUpdateProcess_PhysicStructures' or let the 'N_Disable()' function perform that for you.
	( this function will delete automatically all the postupdate process registered if they still in memorybefore leaving the application.)
*/

// PHYSIC STRUCTURE CORE
// =======================================================================================

// ------------------------------------------------------------------------------------------
// void NInitializePhysicStructuresList
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
void NInitializePhysicStructures()
{
	// Initialize Global NNODELISTs
	//	NSetupNodeList(&NActivePhysicStructureList);
	//	NSetupNodeList(&NActivePhysicStructureCollisionCoupleList);
	NSetupArray(&NActivePhysicStructuresPtr_Array, DEFAULT_PHYSICACCUMULATOR_CAPACITY, sizeof(NSTRUCTURE *));
	NSetupArray(&NActivePhysicStructure_ForceFields_Array, DEFAULT_PHYSICSTRUCTURE_FORCEFIELDS_ARRAY_CAPACITY, sizeof(NSTRUCTUREFORCEFIELD));

	// Initialize Global values
	fSimulationTimeStep = SIMULATION_TIMESTEP;
	fAirFriction = DEFAULT_AIRFRICTION;
}

void NDisablePhysicStructures()
{
	NClearArray(&NActivePhysicStructuresPtr_Array, NULL);
	NClearArray(&NActivePhysicStructure_ForceFields_Array, NULL);
	fSimulationTimeStep = 0.0f;
	fAirFriction = 0.0f;
}

static inline void _CheckStructuresCollision(const NTIME *ptime)
{
	NXNODE *pxnode;
	NSTRUCTURE_COLLISONCOUPLE *pcol;
	NJOINT *pjA, *pjB;
	Nu32 i, j;
	Nf32 fdist;
	Nf32 coldist;
	NVEC3 AB;
	NSTRUCTURE_COLLISION_RESULT result;

	// Collision list
	pxnode = (NXNODE *)NpActivePhysicStructureCollisionCoupleList->pFirst;
	while ((NXNODE *)pxnode != (NXNODE *)NpActivePhysicStructureCollisionCoupleList)
	{
		pcol = (NSTRUCTURE_COLLISONCOUPLE *)NGetXNodeX(pxnode);
		if (ISFLAG_ON(pcol->pStructureA->Flags, FLAG_NSTRUCTURE_ACTIVE) && ISFLAG_ON(pcol->pStructureB->Flags, FLAG_NSTRUCTURE_ACTIVE))
		{
			if (NStructureCollisionAABBvsAABB(pcol->pStructureA, pcol->pStructureB))
			{
				// Test collision
				pjA = (NJOINT *)NGetFirstArrayPtr(&pcol->pStructureA->JointArray);
				for (i = NGetArraySize(&pcol->pStructureA->JointArray); i != 0; i--, pjA++)
				{
					pjB = (NJOINT *)NGetFirstArrayPtr(&pcol->pStructureB->JointArray);
					for (j = NGetArraySize(&pcol->pStructureB->JointArray); j != 0; j--, pjB++)
					{
						// all++;
						NVec3Sub(&AB, &pjB->CurrentPosition, &pjA->CurrentPosition);
						AB.z = 0.0f;

						coldist = pjA->fRadius + pjB->fRadius; // instead of pjB->Radius we had: + 1.0f;

						if (AB.x > coldist || AB.x < -coldist)
							continue;
						if (AB.y > coldist || AB.y < -coldist)
							continue;
						// 						if( AB.z > coldist || AB.z < -coldist )
						// 							continue;

						// Here we are, there is a high probability of collision
						fdist = NFastSqrt(AB.x * AB.x + AB.y * AB.y);

						if (fdist <= coldist)
						{
							// 							NVec3Lerp(&vcol,&pjA->CurrentPosition,&pjB->CurrentPosition,fdist/coldist_square);
							// 							fdist = coldist_square - fdist;
							// 							NVec3Scale(&v,&AB,REPULSION_IMPULSION*fdist);
							// 							NAddForceToJoint(pjB,&v);

							result.SeparatingLimit = coldist;
							result.SeparatingDistance = fdist;
							result.pjA = pjA;
							result.pjB = pjB;
							// CallBack Function Call
							pcol->CollisionResponse(pcol, &result);
						}
					}
				}
			}
		}
		XNEXT(pxnode);
	}
}

void NUpdatePhysicStructures(const NTIME *ptime)
{
	NSTRUCTUREFORCEFIELD *pff, *pffstart;
	NSTRUCTURE **pps;
	NSTRUCTURE *ps;
	NSKIN *psk;
	Nu32 IterationsNumber, a, i, j;
	Nf32 fdelay;

	if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PHYSIC))
	{
		NActivePhysicStructuresPtr_Array.Size = 0;
		return;
	}

	IterationsNumber = (Nu32)(ptime->Nf32_Delay / fSimulationTimeStep) + 1;
	fdelay = ptime->Nf32_Delay / (Nf32)IterationsNumber;

	// printf("\n Physic Updates: %d  number of physic structure: %d", IterationsNumber, NActivePhysicStructuresPtr_Array.Size);

	for (a = 0; a < IterationsNumber; a++)
	{
		// Update forcefields
		pffstart = (NSTRUCTUREFORCEFIELD *)NGetFirstArrayPtr(&NActivePhysicStructure_ForceFields_Array);
		pff = pffstart;

		for (i = 0; i < NActivePhysicStructure_ForceFields_Array.Size; i++, pff++)
		{
			if (!pff->pUpdateAndApplyForceField(pff, fdelay)) // !!! return value NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE == 0 !!!
			{
				// Remove ForceField pointer from array !
				// !!! Because Forcefield processing order is not important we are going to do a "quick" remove !!!
				memcpy(pff, &pffstart[NActivePhysicStructure_ForceFields_Array.Size - 1], sizeof(NSTRUCTUREFORCEFIELD));
				pff--;
				i--;
				NActivePhysicStructure_ForceFields_Array.Size--;
			}
		}

		// Structure by structure
		pps = (NSTRUCTURE **)NGetFirstArrayPtr(&NActivePhysicStructuresPtr_Array);
		for (i = NActivePhysicStructuresPtr_Array.Size; i != 0; i--, pps++)
		{
			ps = *pps;
			ps->pAccumulateForce(ps, fdelay);
			ps->pIntegration(ps, fdelay);
			ps->pSatisfyConstraint(ps, fdelay);

			// Update Structure AABB (Joints Radius included !)
			NUpdateStructureAABB(ps);
		}

		// Collisions ?
		if (NpActivePhysicStructureCollisionCoupleList)
			_CheckStructuresCollision(ptime);
	}

	// LAST PASS
	pps = (NSTRUCTURE **)NGetFirstArrayPtr(&NActivePhysicStructuresPtr_Array);
	for (i = NGetArraySize(&NActivePhysicStructuresPtr_Array); i != 0; i--, pps++)
	{
		// Skin
		ps = *pps;
		psk = (NSKIN *)NGetFirstArrayPtr(&ps->SkinArray);
		for (j = NGetArraySize(&ps->SkinArray); j != 0; j--, psk++)
		{
			psk->common.pUpdateSkinGeometry(psk);
		}

		// BV
		if (ISFLAG_ON(ps->Flags, FLAG_NSTRUCTURE_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB))
			NGetAABBBoundingSphere(&ps->pRenderable->BoundingSphere, &ps->JointsAABB);

		// Remove  systematically the Active Flag.
		// This flag is Set to On when Structure Ptr was pushed onto the "NActivePhysicStructuresPtr_Array"
		FLAG_OFF(ps->Flags, FLAG_NSTRUCTURE_ACTIVE);
	}

	NActivePhysicStructuresPtr_Array.Size = 0;
}

// ------------------------------------------------------------------------------------------
// NSetupPhysicStructure
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
NSTRUCTURE *NSetupPhysicStructure(NSTRUCTURE *pstructure, const NSTRUCTURE_DESC *builder)
{
	NRENDERABLE_DESC desc;

	memset(pstructure, 0, sizeof(NSTRUCTURE));

	// ======================================================
	// Renderable creation (only if requested,..., of course)
	// ======================================================

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_WITH_RENDERABLE))
	{
		memset(&desc, 0, sizeof(NRENDERABLE_DESC));
		if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_RENDERABLEGEOMETRY_CAPACITY))
			desc.IncludedGeometryPreallocatedCapacity = builder->RenderableGeometryCapacity;
		else
			desc.IncludedGeometryPreallocatedCapacity = 0;

		desc.BoundingSphere.fRadius = DEFAULT_NSTRUCTURE_RENDERABLE_BOUNDINGSPHERE_RADIUS;

		NEXTRACTSET lowlevelextractset;
		switch (desc.IncludedGeometryPreallocatedCapacity)
		{
		case 0:
			lowlevelextractset = NEXTRACTSET_SECTION_VOID;
			// NGetRenderableExtractFunctionByExtractSet( NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,0) ); could works too
			// but NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,0) -> NRENDERABLE_EXTRACT_FCT_VOID has to be defined... let's see
			break;
		case 1:
			lowlevelextractset = NEXTRACTSET_SECTION_STANDARD;
			break;
		default: // > 1
			lowlevelextractset = NEXTRACTSET_SECTION_STANDARD | FLAG_NEXTRACTSET_MULTIGEOMETRIES;
			break;
		}

		NEXTRACTSET highlevelextractset;
		if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_HIGHLEVEL_EXTRACTSET))
		{
			highlevelextractset = builder->HighLevelExtractSet;
		}
		else
		{
			highlevelextractset = DEFAULT_NSTRUCTURE_RENDERABLE_HIGHLEVEL_EXTRACTSET;
		}
		NErrorIf(NGET_EXTRACTSET_LOWLEVEL(highlevelextractset), NERROR_RENDERABLE_EXTRACTSET_UNAUTHORIZED_FLAGS_USING);
		desc.Update_FCT = NRenderableUpdate_PhysicStructure;
		desc.Extract_FCT = NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(lowlevelextractset, highlevelextractset));
		pstructure->pRenderable = NCreateRenderable(&desc);

		// By default BV Sphere is not update each Turn, but still the same. In most cases it will be enough
		// but if user wants to update BV Sphere each turn, he can ...
		if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB))
		{
			FLAG_ON(pstructure->Flags, FLAG_NSTRUCTURE_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB);
		}
	}

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSARRAY_CAPACITY))
		NSetupArray(&pstructure->JointArray, builder->JointsArrayCapacity, sizeof(NJOINT));
	else
		NSetupArray(&pstructure->JointArray, 0, sizeof(NJOINT));

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_SPRINGS_CAPACITY))
		NSetupArray(&pstructure->SpringArray, builder->SpringsArrayCapacity, sizeof(NCONSTRAINT));
	else
		NSetupArray(&pstructure->SpringArray, 0, sizeof(NCONSTRAINT));

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_SIMPLE_CONSTRAINTS_CAPACITY))
		NSetupArray(&pstructure->SimpleConstraintArray, builder->SimpleConstraintsArrayCapacity, sizeof(NCONSTRAINT));
	else
		NSetupArray(&pstructure->SimpleConstraintArray, 0, sizeof(NCONSTRAINT));

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_MINMAX_CONSTRAINTS_CAPACITY))
		NSetupArray(&pstructure->MinMaxConstraintArray, builder->MinMaxConstraintsArrayCapacity, sizeof(NCONSTRAINT));
	else
		NSetupArray(&pstructure->MinMaxConstraintArray, 0, sizeof(NCONSTRAINT));

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_SPECIAL_CONSTRAINTS_CAPACITY))
		NSetupArray(&pstructure->SpecialConstraintArray, builder->SpecialConstraintsArrayCapacity, sizeof(NCONSTRAINT));
	else
		NSetupArray(&pstructure->SpecialConstraintArray, 0, sizeof(NCONSTRAINT));

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_SKINARRAY_CAPACITY))
		NSetupArray(&pstructure->SkinArray, builder->SkinArrayCapacity, sizeof(NSKIN));
	else
		NSetupArray(&pstructure->SkinArray, 0, sizeof(NSKIN));

	// Other Data Init
	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_VGRAVITY))
		pstructure->vGravity = builder->vGravity;
	else
		NVec3Set(&pstructure->vGravity, DEFAULT_NSTRUCTURE_VGRAVITY_3f);

	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSPEEDDAMPING))
		pstructure->JointSpeedDamping = builder->JointSpeedDamping;
	else
		pstructure->JointSpeedDamping = NDEFAULT_JOINTSPEEDDAMPING;

	// ==============
	// FUNCTIONS link
	// ==============
	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_BIND_EULER_PHYSIC_FUNCTIONS))
	{
		NBind_Physic_EulerFunctions(pstructure);
	}
	else // The default one ...
	{
		NBind_Physic_VerletFunctions(pstructure);
	}
	// 2nd pass: force use of a specific function
	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_ACCUMULATEFORCES_USERFUNCTION))
	{
		pstructure->pAccumulateForce = builder->pAccumulateForce_UserFunction;
	}
	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_INTEGRATION_USERFUNCTION))
	{
		pstructure->pIntegration = builder->pIntegration_UserFunction;
	}
	if (ISFLAG_ON(builder->Flags, FLAG_NSTRUCTURE_DESC_USEPARAM_SATISFYCONSTRAINT_USERFUNCTION))
	{
		pstructure->pSatisfyConstraint = builder->pSatisfyConstraint_UserFunction;
	}

	return pstructure;
}

// ------------------------------------------------------------------------------------------
// NCreatePhysicStructure
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
NSTRUCTURE *NCreatePhysicStructure(const NSTRUCTURE_DESC *builder)
{
	return NSetupPhysicStructure(NEW(NSTRUCTURE), builder);
}

void NClearPhysicStructure(NSTRUCTURE *ps)
{
	NErrorIf(ps->CollisionReferencesCounter, NERROR_PHYSICSTRUCTURE_CLEARREQUEST_UNAUTHORIZED_WITH_EXISTING_COLLISION_REF);

	if (ps->pRenderable)
		NDeleteRenderable(ps->pRenderable);

	// Free Memory
	NClearArray(&ps->JointArray, NULL);
	NClearArray(&ps->SpringArray, NULL);
	NClearArray(&ps->SimpleConstraintArray, NULL);
	NClearArray(&ps->MinMaxConstraintArray, NULL);
	NClearArray(&ps->SpecialConstraintArray, NULL);
	NClearArray(&ps->SkinArray, NSkinDestructor_ArrayCallBack);

	Nmem0(ps, NSTRUCTURE);
}

// ------------------------------------------------------------------------------------------
// NDeletePhysicStructure
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
void NDeletePhysicStructure(NSTRUCTURE *ps)
{
	NClearPhysicStructure(ps);
	Nfree(ps);
}

void NErasePhysicStructureElementArray(NSTRUCTURE *ps, NSTRUCTURE_ELEMENT_TYPE_ENUM element_type)
{
	if (element_type == NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE)
	{
		NEraseArray(&ps->JointArray, NULL);
		NEraseArray(&ps->SpringArray, NULL);
		NEraseArray(&ps->SimpleConstraintArray, NULL);
		NEraseArray(&ps->MinMaxConstraintArray, NULL);
		NEraseArray(&ps->SpecialConstraintArray, NULL);
	}
	else
	{
		NEraseArray(&ps->ElementArray[element_type], NULL);
	}
}

/*
void	NErasePhysicStructureAllContents(NSTRUCTURE *ps)
{
	if(ps->pRenderable)
		NEraseRenderableGeometryArray(ps->pRenderable);

	NErasePhysicStructureSkinArray(ps);

	NEraseArray(&ps->JointArray,NULL);
	NEraseArray(&ps->SpringArray,NULL);
	NEraseArray(&ps->SimpleConstraintArray,NULL);
	NEraseArray(&ps->MinMaxConstraintArray,NULL);
	NEraseArray(&ps->SpecialConstraintArray,NULL);
}
*/

// ------------------------------------------------------------------------------------------
// NMovePhysicStructure
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
void NMovePhysicStructure(NSTRUCTURE *ps, const NVEC3 *pv)
{
	Nu32 i;
	NJOINT *pj;
	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = 0; i < NGetArraySize(&ps->JointArray); i++)
	{
		NVec3AddTo(&pj->CurrentPosition, pv);
		NVec3AddTo(&pj->OldPosition, pv);
		pj++;
	}
}

// ------------------------------------------------------------------------------------------
// NSetPhysicStructurePosition
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//			ps:				a valid pointer on a physic structure
//			ref_jointid:	index of the structure joint which going to be the reference of the move !
//							after function processing, the whole structure will be shifted AND this joint
//							will be at the position given by 'pv'
//			pv:				a NVEC3 which represents the desired position for the "ref_jointid"
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetPhysicStructurePosition(NSTRUCTURE *ps, const Nu32 ref_jointid, const NVEC3 *pv)
{
	Nu32 i;
	NJOINT *pj;
	NVEC3 shift;

	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	NVec3Sub(&shift, pv, &pj[ref_jointid].CurrentPosition);

	for (i = 0; i < NGetArraySize(&ps->JointArray); i++)
	{
		NVec3AddTo(&pj->CurrentPosition, &shift);
		NVec3AddTo(&pj->OldPosition, &shift);
		pj++;
	}
}

/*
static inline void _constraintarray_linkedjoint_ptoi(const NARRAY *pjarray,NARRAY *pcarray)
{
	Nu32			i;
	NCONSTRAINT		*pc;

	pc = (NCONSTRAINT*)NGetFirstArrayPtr(pcarray);
	for(i=0;i<NGetArraySize(pcarray);i++,pc++)
	{
		pc->pjA = (NJOINT*)NGetArrayIndex(pjarray,(NBYTE*)pc->pjA);
		pc->pjB = (NJOINT*)NGetArrayIndex(pjarray,(NBYTE*)pc->pjB);
	}
}

static inline void _constraintarray_linkedjoint_itop(const NARRAY *pjarray,NARRAY *pcarray)
{
	Nu32			i;
	NCONSTRAINT		*pc;

	pc = (NCONSTRAINT*)NGetFirstArrayPtr(pcarray);
	for(i=0;i<NGetArraySize(pcarray);i++,pc++)
	{
		pc->pjA = (NJOINT*)NGetArrayPtr(pjarray,(Nu32)pc->pjA);
		pc->pjB = (NJOINT*)NGetArrayPtr(pjarray,(Nu32)pc->pjB);
	}
}

static inline void _skinarray_allptoi(NSTRUCTURE *ps)
{
	Nu32	i;
	NSKIN	*pskin;
	NARRAY	*pref_array;

	pskin = (NSKIN*)NGetFirstArrayPtr(&ps->SkinArray);
	for(i=0;i<NGetArraySize(&ps->SkinArray);i++,pskin++)
	{
		// specific part first ...
		switch(pskin->common.SkinType)
		{
			case NSKIN_TYPE_BILLBOARD:
				pskin->BillBoard.pCenter	= (NJOINT*)NGetArrayIndex(&ps->JointArray,(NBYTE*)pskin->BillBoard.pCenter);
				pskin->BillBoard.pOrient_A	= (NJOINT*)NGetArrayIndex(&ps->JointArray,(NBYTE*)pskin->BillBoard.pOrient_A);
				pskin->BillBoard.pOrient_B	= (NJOINT*)NGetArrayIndex(&ps->JointArray,(NBYTE*)pskin->BillBoard.pOrient_B);
				break;

			case NSKIN_TYPE_ROPE:
				switch(pskin->Rope.StructureElementType)
				{
/ *
					case NSTRUCTURE_ELEMENT_JOINT:				// Not used by Rope !
						pref_array = &ps->JointArray;			// Not used by Rope !
						break;									// Not used by Rope !
* /
					case NSTRUCTURE_ELEMENT_SPRING:
						pref_array = &ps->SpringArray;
						break;

					case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
						pref_array = &ps->SimpleConstraintArray;
						break;

					case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
						pref_array = &ps->MinMaxConstraintArray;
						break;

					case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
						pref_array = &ps->SpecialConstraintArray;
						break;
					default:
						NErrorIf( 1,NERROR_INVALID_CASE );
						break;
				}
				pskin->Rope.pFirst = (void*)NGetArrayIndex(pref_array,(NBYTE*)pskin->Rope.pFirst);
				break;

			case NSKIN_TYPE_GRID:
				pref_array = &ps->JointArray;			// The only available one !
				pskin->Grid.pFirst = (void*)NGetArrayIndex(pref_array,(NBYTE*)pskin->Grid.pFirst);
				break;

			default:
				NErrorIf( 1,NERROR_INVALID_CASE );
				break;
		}
		// ... and the common part.
		pskin->common.pFirstVertex	= (NSKINVERTEX*)NGetArrayIndex(&pskin->common.pGeometry->Mesh.VertexArray,(NBYTE*)pskin->common.pFirstVertex);
		pskin->common.pGeometry		= (NGEOMETRY *)NGetArrayIndex(&ps->pRenderable->GeometryArray,(NBYTE*)pskin->common.pGeometry);
	}
}

static inline void _skinarray_allitop(NSTRUCTURE *ps)
{
	Nu32	i;
	NSKIN	*pskin;
	NARRAY	*pref_array;

	pskin = (NSKIN*)NGetFirstArrayPtr(&ps->SkinArray);
	for(i=0;i<NGetArraySize(&ps->SkinArray);i++,pskin++)
	{
		// specific part first ...
		switch(pskin->common.SkinType)
		{
			case NSKIN_TYPE_BILLBOARD:
				pskin->BillBoard.pCenter	= (NJOINT*)NGetArrayPtr(&ps->JointArray,(Nu32)pskin->BillBoard.pCenter);
				pskin->BillBoard.pOrient_A	= (NJOINT*)NGetArrayPtr(&ps->JointArray,(Nu32)pskin->BillBoard.pOrient_A);
				pskin->BillBoard.pOrient_B	= (NJOINT*)NGetArrayPtr(&ps->JointArray,(Nu32)pskin->BillBoard.pOrient_B);
				break;

			case NSKIN_TYPE_ROPE:
				switch(pskin->Rope.StructureElementType)
				{
/ *
					case NSTRUCTURE_ELEMENT_JOINT:				// Not used by Rope !
						pref_array = &ps->JointArray;			// Not used by Rope !
						break;									// Not used by Rope !
* /
					case NSTRUCTURE_ELEMENT_SPRING:
						pref_array = &ps->SpringArray;
						break;

					case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
						pref_array = &ps->SimpleConstraintArray;
						break;

					case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
						pref_array = &ps->MinMaxConstraintArray;
						break;

					case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
						pref_array = &ps->SpecialConstraintArray;
						break;
					default:
						NErrorIf( 1,NERROR_INVALID_CASE );
						break;
				}
				pskin->Rope.pFirst = (void*)NGetArrayPtr(pref_array,(Nu32)pskin->Rope.pFirst);
				break;

			case NSKIN_TYPE_GRID:
				pref_array = &ps->JointArray;			// The only available one !
				pskin->Grid.pFirst = (void*)NGetArrayPtr(pref_array,(Nu32)pskin->Grid.pFirst);
				break;
			default:
				NErrorIf( 1,NERROR_INVALID_CASE );
				break;
		}
		// ... and the common part.
		pskin->common.pGeometry		= (NGEOMETRY *)NGetArrayPtr(&ps->pRenderable->GeometryArray,(Nu32)pskin->common.pGeometry);
		pskin->common.pFirstVertex	= (NSKINVERTEX*)NGetArrayPtr(&pskin->common.pGeometry->VertexArray,(Nu32)pskin->common.pFirstVertex);
	}
}
*/

static inline void _constraint_linked_joint_redirection(NARRAY *pc_array_dst, const NARRAY *pj_array_dst, const NARRAY *pc_array_src, const NARRAY *pj_array_src)
{
	NErrorIf(pc_array_src->ElementSize != pc_array_dst->ElementSize, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pc_array_src->Size != pc_array_dst->Size, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pc_array_src->Capacity != pc_array_dst->Capacity, NERROR_INCONSISTENT_PARAMETERS);

	NErrorIf(pj_array_src->ElementSize != pj_array_dst->ElementSize, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pj_array_src->Size != pj_array_dst->Size, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pj_array_src->Capacity != pj_array_dst->Capacity, NERROR_INCONSISTENT_PARAMETERS);

	NCONSTRAINT *pc_src = (NCONSTRAINT *)pc_array_src->pFirst;
	NCONSTRAINT *pc_dst = (NCONSTRAINT *)pc_array_dst->pFirst;

	for (Nu32 i = pc_array_src->Size; i != 0; i--, pc_src++, pc_dst++)
	{
		pc_dst->pjA = (NJOINT *)NGetArrayPtr(pj_array_dst, NGetArrayIndex(pj_array_src, (NBYTE *)pc_src->pjA));
		pc_dst->pjB = (NJOINT *)NGetArrayPtr(pj_array_dst, NGetArrayIndex(pj_array_src, (NBYTE *)pc_src->pjB));
	}
}

void NCopyPhysicStructure(NSTRUCTURE *pdst, const NSTRUCTURE *psrc)
{
	Nu32 i;

	// -------------------------------------------------------------------------------------------------------------------------
	// 1/ Copy All the "easy to copy" parameters ...
	pdst->Flags = psrc->Flags;
	pdst->pAccumulateForce = psrc->pAccumulateForce;
	pdst->pIntegration = psrc->pIntegration;
	pdst->pSatisfyConstraint = psrc->pSatisfyConstraint;
	pdst->JointSpeedDamping = psrc->JointSpeedDamping;
	pdst->vGravity = psrc->vGravity;
	pdst->JointsAABB = psrc->JointsAABB;
	// pdst->CollisionReferencesCounter = ... This parameter still the same !

	// -------------------------------------------------------------------------------------------------------------------------
	// 2/ Copy Renderable data ... 'pTransformHNode' excepted. This parameter still the same even if there is a copy.
	// If user wants to share the same TransformHnode he has to do this by himself !
	// Destination renderable is going to have the exact source Renderable geometries number (same geometry array size) and the same geometry array capacity !
	// (First we need to have a clean renderable allocated into the destination structure )
	// So, if there is some difference between source and destination in term of size and/or capacity, we have potentially to delete some geometries first !
	// The functions below may cause memory reallocation it's why we perform skin pointers indexation before !
	if (psrc->pRenderable)
	{
		if (pdst->pRenderable)
		{
			NCopyRenderable(pdst->pRenderable, psrc->pRenderable, FLAG_NCOPYRENDERABLE_DONTCOPY_TRANSFORMHNODE);
		}
		else
		{
			pdst->pRenderable = NDuplicateRenderable(psrc->pRenderable, FLAG_NDUPLICATERENDERABLE_DONTDUPLICATE_TRANSFORMHNODE);
		}
	}
	else // psrc->pRenderable == NULL (That means no SRC renderable)
	{
		if (pdst->pRenderable)
		{
			NDeleteRenderable(pdst->pRenderable);
			pdst->pRenderable = NULL;
		}
	}
	// -------------------------------------------------------------------------------------------------------------------------
	// 3/ Copy all the Physic arrays ...
	NCopyArray(&pdst->JointArray, &psrc->JointArray);
	NCopyArray(&pdst->SpringArray, &psrc->SpringArray);
	NCopyArray(&pdst->SimpleConstraintArray, &psrc->SimpleConstraintArray);
	NCopyArray(&pdst->MinMaxConstraintArray, &psrc->MinMaxConstraintArray);
	NCopyArray(&pdst->SpecialConstraintArray, &psrc->SpecialConstraintArray);
	// ... and update all the internal pointers
	_constraint_linked_joint_redirection(&pdst->SpringArray, &pdst->JointArray, &psrc->SpringArray, &psrc->JointArray);
	_constraint_linked_joint_redirection(&pdst->SimpleConstraintArray, &pdst->JointArray, &psrc->SimpleConstraintArray, &psrc->JointArray);
	_constraint_linked_joint_redirection(&pdst->MinMaxConstraintArray, &pdst->JointArray, &psrc->MinMaxConstraintArray, &psrc->JointArray);
	_constraint_linked_joint_redirection(&pdst->SpecialConstraintArray, &pdst->JointArray, &psrc->SpecialConstraintArray, &psrc->JointArray);
	// -------------------------------------------------------------------------------------------------------------------------
	// 4/ And The skins ...
	NCopyArray(&pdst->SkinArray, &psrc->SkinArray);

	NSKIN *pskin_dst = (NSKIN *)pdst->SkinArray.pFirst;
	NSKIN *pskin_src = (NSKIN *)psrc->SkinArray.pFirst;
	for (i = psrc->SkinArray.Size; i != 0; i--, pskin_src++, pskin_dst++)
	{
		// specific skin part first ...
		switch (pskin_src->common.SkinType)
		{
		case NSKIN_TYPE_BILLBOARD:
			pskin_dst->BillBoard.pCenter = (NJOINT *)NGetArrayPtr(&pdst->JointArray, NGetArrayIndex(&psrc->JointArray, (NBYTE *)pskin_src->BillBoard.pCenter));
			pskin_dst->BillBoard.pOrient_A = (NJOINT *)NGetArrayPtr(&pdst->JointArray, NGetArrayIndex(&psrc->JointArray, (NBYTE *)pskin_src->BillBoard.pOrient_A));
			pskin_dst->BillBoard.pOrient_B = (NJOINT *)NGetArrayPtr(&pdst->JointArray, NGetArrayIndex(&psrc->JointArray, (NBYTE *)pskin_src->BillBoard.pOrient_B));
			break;

		case NSKIN_TYPE_ROPE:
			NErrorIf(pskin_dst->Rope.StructureElementType != pskin_src->Rope.StructureElementType, NERROR_STRUCTURE_SKIN_UNEXPECTED_VALUE);
			pskin_dst->Rope.pFirst = (void *)NGetArrayPtr(&pdst->ElementArray[pskin_dst->Rope.StructureElementType],
														  NGetArrayIndex(&psrc->ElementArray[pskin_src->Rope.StructureElementType], (NBYTE *)pskin_src->Rope.pFirst));

			// 				NCopyArray(pskin_dst->Rope.pSideAVertebra,pskin_src->Rope.pSideAVertebra);
			// 				NCopyArray(pskin_dst->Rope.pSideBVertebra,pskin_src->Rope.pSideBVertebra);
			NCopyArray(&pskin_dst->Rope.VertebrasArray, &pskin_src->Rope.VertebrasArray);
			break;

		case NSKIN_TYPE_GRID:
			NErrorIf(pskin_dst->Grid.StructureElementType != NSTRUCTURE_ELEMENT_JOINT, NERROR_STRUCTURE_SKIN_UNEXPECTED_VALUE);
			pskin_dst->Grid.pFirst = (void *)NGetArrayPtr(&pdst->JointArray, NGetArrayIndex(&psrc->JointArray, (NBYTE *)pskin_src->Grid.pFirst));
			break;
		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			break;
		}
		// ... and the common part.
		NErrorIf(pdst->pRenderable->GeomArray.Size != psrc->pRenderable->GeomArray.Size, NERROR_SYSTEM_GURU_MEDITATION); /// This should never happen ....
		pskin_dst->common.pGeometry = NGetIncludedGeometryByIndex(pdst->pRenderable, NGetIncludedGeometryIndex(psrc->pRenderable, pskin_src->common.pGeometry));
		pskin_dst->common.pFirstVertex = (NSKINVERTEX *)NGetArrayPtr(&pskin_dst->common.pGeometry->Mesh.VertexArray, NGetArrayIndex(&pskin_src->common.pGeometry->Mesh.VertexArray, (NBYTE *)pskin_src->common.pFirstVertex));
	}
}
/*
void NCreateJointsRange(NSTRUCTURE *ps,DWORD number_of_joints,NJOINT_DESC *builder)
{
	DWORD i;
	NJOINT	jointdraft;
	NVEC3	vshift;

	if(builder)
	{
		jointdraft.fRadius	= builder->fRadius;
		jointdraft.fMass	= builder->fMass;
		NVec3Copy(&jointdraft.vSpeed,&builder->vSpeed);
		NVec3Set(&jointdraft.vForcesAccumulator,0,0,0);
		NVec3Copy(&jointdraft.CurrentPosition,&builder->CurrentPosition);
		NVec3Copy(&jointdraft.OldPosition,&builder->OldPosition);
		NVec3Copy(&vshift,&builder->shift_Vector);
	}
	else
	{
		memset(&jointdraft,0,sizeof(NJOINT));
		jointdraft.fRadius	= JOINT_DEFAULTRADIUS;
		jointdraft.fMass	= JOINT_DEFAULTMASS;
		NVec3Set(&vshift,0,0,0);
	}

	for(i=0;i<number_of_joints;i++)
	{
		NArrayPushBack(&ps->JointArray,(NBYTE*)&jointdraft);

		NVec3AddTo(&jointdraft.CurrentPosition,&vshift);
		NVec3AddTo(&jointdraft.OldPosition,&vshift);
	}
}*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							PHYSIC CALLBACK Functions								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// =======================================================================================
// EULER
// =======================================================================================
/*
static inline void	_euler_applysprings_forces(NSTRUCTURE *ps,float fdelay)
{
	DWORD			i;
	NVEC3		v,va;
	NCONSTRAINT		*pspring;
	float			fABlength;
	float			fdiff;

	pspring = (NCONSTRAINT*)NGetFirstArrayPtr(&ps->SpringArray);
	for(i=0;i<NGetArraySize(&ps->SpringArray);i++)
	{
		// SPRING Force
		NVec3Sub(&pspring->AB,&pspring->pjB->CurrentPosition,&pspring->pjA->CurrentPosition);
		fABlength = NVec3FastNormalize(&pspring->AB);
		fdiff= fABlength - pspring->fRestLength;
		fdiff *= pspring->fK;
		NVec3Scale(&v,&pspring->AB,fdiff);

		// Resistance Force
		NVec3Sub(&va,&pspring->pjA->vSpeed,&pspring->pjB->vSpeed);
		NVec3Scale(&va,&va,-pspring->fFriction);
		NVec3AddTo(&v,&va);

		NAddForceToJoint(pspring->pjA,&v);
		NSubForceFromJoint(pspring->pjB,&v);

		pspring++;
	}
}
static inline void	_euler_applybasic_forces(NSTRUCTURE *ps,float fdelay)
{
	DWORD			i;
	NJOINT			*pj;
	NVEC3		v;

	pj=(NJOINT*)NGetFirstArrayPtr(&ps->JointArray);
	for( i=0;i<NGetArraySize(&ps->JointArray);i++ )
	{
		// Fixed Joint Trick
		if( ISFLAG_ON(pj->Flags,FLAG_JOINT_FIXED) )
			NVec3Set(&pj->vForcesAccumulator,0,0,0);
		else
		{
			// Weight
			NVec3Scale(&v,&ps->vGravity,pj->fMass);
			NAddForceToJoint(pj,&v);

			// Air Friction
			NVec3Scale(&v,&pj->vSpeed,-fAirFriction);
			NAddForceToJoint(pj,&v);
		}
		pj++;
	}
}

static inline void _euler_applyforcefields_forces(NSTRUCTURE *ps,float fdelay)
{
	Nu32					i;
	NJOINT					*pj;
	NVEC3				v;
	NSTRUCTUREFORCEFIELD_BASIC	**ppff, **ppffstart;

	ppffstart = ppff = (NSTRUCTUREFORCEFIELD_BASIC**)NGetFirstArrayPtr(NpActivePhysicStructureForceFieldsPtrArray);
	for(i=0;i<NGetArraySize(NpActivePhysicStructureForceFieldsPtrArray);i++,ppff++)
	{
		if( !(*ppff)->pApplyForceField(*ppff,fdelay) ) // !!! return value NSTRUCTUREFORCEFIELD_RESULT_STOP_AND_REMOVE = 0 !!!
		{
			// Remove ForceField pointer from array !
			// !!! Because Forcefield processing order is not important we are going to do a "quick" remove !!!
			*ppff = ppffstart[NpActivePhysicStructureForceFieldsPtrArray->Size - 1];
			ppff--;
			i--;
			NpActivePhysicStructureForceFieldsPtrArray->Size--;
		}
	}
}
*/

void NPhysicStructureDefault_EulerAccumulateForces(NSTRUCTURE *ps, const Nf32 fdelay)
{
	// 	_euler_applysprings_forces(ps,fdelay);
	// 	_euler_applybasic_forces(ps,fdelay);

	Nu32 i;
	NVEC3 v, va;
	NCONSTRAINT *pspring;
	Nf32 fABlength;
	Nf32 fdiff;
	NJOINT *pj;

	pspring = (NCONSTRAINT *)NGetFirstArrayPtr(&ps->SpringArray);
	for (i = NGetArraySize(&ps->SpringArray); i != 0; i--, pspring++)
	{
		// SPRING Force
		NVec3Sub(&pspring->AB, &pspring->pjB->CurrentPosition, &pspring->pjA->CurrentPosition);
		fABlength = NVec3FastNormalize(&pspring->AB);
		fdiff = fABlength - pspring->fRestLength;
		fdiff *= pspring->fK;
		NVec3Scale(&v, &pspring->AB, fdiff);

		// Resistance Force
		NVec3Sub(&va, &pspring->pjA->vSpeed, &pspring->pjB->vSpeed);
		NVec3Scale(&va, &va, -pspring->fFriction);
		NVec3AddTo(&v, &va);

		NAddForceToJoint(pspring->pjA, &v);
		NSubForceFromJoint(pspring->pjB, &v);
	}

	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = NGetArraySize(&ps->JointArray); i != 0; i--, pj++)
	{
		// Fixed Joint Trick
		if (ISFLAG_ON(pj->Flags, FLAG_JOINT_FIXED))
			NVec3Set(&pj->vForcesAccumulator, 0, 0, 0);
		else
		{
			// Weight
			NVec3Scale(&v, &ps->vGravity, pj->fMass);
			NAddForceToJoint(pj, &v);

			// Air Friction
			NVec3Scale(&v, &pj->vSpeed, -fAirFriction);
			NAddForceToJoint(pj, &v);
		}
	}
}

/*
void NPhysicStructureForceFieldSensitive_EulerAccumulateForces(NSTRUCTURE *ps,float fdelay)
{
	_euler_applysprings_forces(ps,fdelay);
	_euler_applybasic_forces(ps,fdelay);
	_euler_applyforcefields_forces(ps,fdelay);
}*/

void NPhysicStructureDefault_EulerIntegration(NSTRUCTURE *ps, const Nf32 fdelay)
{
	Nu32 i;
	NJOINT *pj;
	NVEC3 vtemp, va;
	Nf32 factor;

	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = NGetArraySize(&ps->JointArray); i != 0; i--, pj++)
	{
		pj->OldPosition = pj->CurrentPosition;

		factor = fdelay / pj->fMass;
		NVec3ScaleBy(&pj->vSpeed, ps->JointSpeedDamping);
		NVec3Scale(&va, &pj->vForcesAccumulator, factor);
		NVec3AddTo(&pj->vSpeed, &va);
		NVec3Scale(&vtemp, &pj->vSpeed, fdelay);
		NVec3AddTo(&pj->CurrentPosition, &vtemp);

		// Reset Forces Accumulator
		NVec3Set(&pj->vForcesAccumulator, 0.0f, 0.0f, 0.0f);
	}
}

void NPhysicStructureDefault_EulerSatisfyConstraints(NSTRUCTURE *ps, const Nf32 fdelay)
{
	Nu32 i;
	NCONSTRAINT *pc;
	NJOINT *pj;
	Nf32 fABlength;
	Nf32 fdiff;
	NVEC3 v;

	// for(iter=100;iter!=0;iter--)
	// {

	// ==================
	// Simple Constraints
	// ==================
	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&ps->SimpleConstraintArray);
	for (i = NGetArraySize(&ps->SimpleConstraintArray); i != 0; i--, pc++)
	{

		// Previous method
		NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
		fABlength = NVec3Normalize(&pc->AB);
		fdiff = fABlength - pc->fLength;

		fdiff *= 0.5f;
		NVec3Scale(&v, &pc->AB, fdiff);
		NVec3SubFrom(&pc->pjB->CurrentPosition, &v);
		NVec3AddTo(&pc->pjA->CurrentPosition, &v);

		/*
				NVec3Sub(&pc->AB,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
				fABlength = NVec3Normalize(&pc->AB);
				fdiff = fABlength - pc->fLength;

				if( ISFLAG_ON(pc->pjA->Flags,FLAG_JOINT_FIXED) )
				{
					if( ISFLAG_ON(pc->pjB->Flags,FLAG_JOINT_FIXED) )
					{
						// nothing to do
					}
					else
					{
						NVec3Scale(&v,&pc->AB,fdiff);
						NVec3SubFrom(&pc->pjB->CurrentPosition,&v);
					}
				}
				else
				{
					if( ISFLAG_ON(pc->pjB->Flags,FLAG_JOINT_FIXED) )
					{
						NVec3Scale(&v,&pc->AB,fdiff);
						NVec3AddTo(&pc->pjA->CurrentPosition,&v);
					}
					else
					{
						fdiff*=0.5f;
						NVec3Scale(&v,&pc->AB,fdiff);
						NVec3SubFrom(&pc->pjB->CurrentPosition,&v);
						NVec3AddTo(&pc->pjA->CurrentPosition,&v);
					}
				}
				*/
	}

	// Debug only
	/*
		pc = (NCONSTRAINT*)NGetFirstArrayPtr(&ps->SimpleConstraintArray);
		for(i=NGetArraySize(&ps->SimpleConstraintArray);i!=0;i--,pc++)
		{
			NVec3Sub(&v,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
			fdiff = NVec3Length(&v);
			printf("\n result: %f  Target: %f Error: %f",fdiff,pc->fLength, fdiff-pc->fLength);
		}
	*/
	// ==================
	// MinMax Constraints
	// ==================
	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&ps->MinMaxConstraintArray);
	for (i = NGetArraySize(&ps->MinMaxConstraintArray); i != 0; i--, pc++)
	{
		NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
		fABlength = NVec3Normalize(&pc->AB);

		if (fABlength > pc->fMaxLength)
		{
			fdiff = fABlength - pc->fMaxLength;
			fdiff *= 0.5f;

			NVec3Scale(&v, &pc->AB, fdiff);
			NVec3SubFrom(&pc->pjB->CurrentPosition, &v);
			NVec3AddTo(&pc->pjA->CurrentPosition, &v);
		}
		else if (fABlength < pc->fMinLength)
		{
			fdiff = fABlength - pc->fMaxLength;
			fdiff *= 0.5f;

			NVec3Scale(&v, &pc->AB, fdiff);
			NVec3SubFrom(&pc->pjB->CurrentPosition, &v);
			NVec3AddTo(&pc->pjA->CurrentPosition, &v);
		}
	}
	// ===================
	// Special Constraints
	// ===================
	pc = (NCONSTRAINT *)NGetFirstArrayPtr(&ps->SpecialConstraintArray);
	for (i = NGetArraySize(&ps->SpecialConstraintArray); i != 0; i--, pc++)
	{
		NVec3Sub(&pc->AB, &pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition);
		fABlength = NVec3Normalize(&pc->AB);
		if (fABlength > pc->fMaxLength)
		{
			NVec3Scale(&v, &pc->AB, pc->fMaxLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			//			NVec3Add(&pc->pjB->OldPosition,&pc->pjA->OldPosition,&v);
		}
		else if (fABlength < pc->fMinLength)
		{
			NVec3Scale(&v, &pc->AB, pc->fMinLength);
			NVec3Add(&pc->pjB->CurrentPosition, &pc->pjA->CurrentPosition, &v);
			//			NVec3Add(&pc->pjB->OldPosition,&pc->pjA->OldPosition,&v);
		}
		/*
				NVec3Sub(&pc->AB,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
				fABlength = NVec3FastNormalize(&pc->AB);
				//fdiff = fABlength - pc->fLength;

				NVec3Scale(&v,&pc->AB,pc->fLength);
				NVec3Add(&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition,&v);
		*/
	}

	// FIXED NJOINT
	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = NGetArraySize(&ps->JointArray); i != 0; i--, pj++)
	{
		// Fixed Joint Trick
		if (ISFLAG_ON(pj->Flags, FLAG_JOINT_FIXED))
			pj->CurrentPosition = pj->OldPosition;
	}

	// NVec3Copy( &ps->pJointList->CurrentPosition,&ps->pJointList->OldPosition);
	// NVec3Set(&ps->pJointList->vSpeed,0,0,0);

	// } // iter
}

// =======================================================================================
// VERLET
// =======================================================================================

void NPhysicStructureDefault_VerletAccumulateForces(NSTRUCTURE *ps, const Nf32 fdelay)
{
	NPhysicStructureDefault_EulerAccumulateForces(ps, fdelay);

	/*
	DWORD		i;
	NJOINT		*pj;
	NVEC3		vtemp,v,va,vb;
	NCONSTRAINT	*pc;
	float		fABlength;
	float		fdiff;

	pc=ps->pConstraintsList;
	for(i=0;i<ps->ConstraintNumber;i++)
	{
		// =======================
		// Constraint SPRING Force
		// =======================
		NVec3Sub(&pc->AB,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
		fABlength = NVec3FastNormalize(&pc->AB);
		fdiff= fABlength - pc->fRestLength;
		fdiff *= -pc->fK;
		NVec3Scale(&vtemp,&pc->AB,fdiff);

		// ================
		// Resistance Force
		// ================
		NVec3Sub(&v,&pc->pjA->vSpeed,&pc->pjB->vSpeed);
		NVec3Scale(&v,&v,-pc->fFriction);
		NVec3AddTo(&vtemp,&v);
		AddForceToJoint(pc->pjA,&vtemp);
		SubForceFromJoint(pc->pjB,&vtemp);

		pc++;
	}

	pj=ps->pJointList;
	for( i=1;i<ps->JointNumber;i++ )
	{
		// =======
		// Weight
		// =======
		NVec3Scale(&v,&vGravity,pj->fMass);
		AddForceToJoint(pj,&v);

		// ============
		// Air Friction
		// ============
		NVec3Scale(&v,&pj->vSpeed,-fAirFriction);
		AddForceToJoint(pj,&v);

		pj++;
	}
	*/
}

void NPhysicStructureDefault_VerletIntegration(NSTRUCTURE *ps, const Nf32 fdelay)
{
	Nu32 i;
	NJOINT *pj;
	NVEC3 vtemp, va, vnew;
	Nf32 factor;
	Nf32 fdelaysquare;

	fdelaysquare = fdelay * fdelay;

	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = NGetArraySize(&ps->JointArray); i != 0; i--, pj++)
	{
		vtemp = pj->CurrentPosition;

		// Verlet
		factor = fdelaysquare / pj->fMass;
		NVec3Scale(&va, &pj->vForcesAccumulator, factor);
		NVec3Sub(&pj->vSpeed, &pj->CurrentPosition, &pj->OldPosition); // Calculate and Store Joint Speed ( to still compatible with Euler approach)
		NVec3ScaleBy(&pj->vSpeed, ps->JointSpeedDamping);

		NVec3AddTo(&pj->vSpeed, &va);
		NVec3Add(&vnew, &pj->CurrentPosition, &pj->vSpeed);

		pj->OldPosition = vtemp;
		pj->CurrentPosition = vnew;

		// Reset Forces Accumulator
		NVec3Set(&pj->vForcesAccumulator, 0, 0, 0);
	}
}

void NPhysicStructureDefault_VerletSatisfyConstraints(NSTRUCTURE *ps, const Nf32 fdelay)
{
	NPhysicStructureDefault_EulerSatisfyConstraints(ps, fdelay);
	/*
	DWORD		i;
	NCONSTRAINT	*pc;
	NJOINT		*pj;
	float		fABlength;
	float		fdiff;
	NVEC3		v;

	pc=ps->pConstraintsList;
	for(i=0;i<ps->ConstraintNumber;i++)
	{
		NVec3Sub(&pc->AB,&pc->pjB->CurrentPosition,&pc->pjA->CurrentPosition);
		fABlength = NVec3FastNormalize(&pc->AB);
		fdiff = fABlength-pc->fMaxLength;

		// =================
		// Standard approach
		// =================
		fdiff*=0.5f;
		NVec3Scale(&v,&pc->AB,fdiff);
		NVec3SubFrom(&pc->pjB->CurrentPosition,&v);
		NVec3AddTo(&pc->pjA->CurrentPosition,&v);
		pc++;
	}
	*/
	// Fixed First Joint
	// NVec3Copy( &ps->pJointList->CurrentPosition,&ps->pJointList->OldPosition);
}

void NResetAllJointsDynamic(NSTRUCTURE *ps)
{
	Nu32 i;
	NJOINT *pj;

	pj = (NJOINT *)NGetFirstArrayPtr(&ps->JointArray);
	for (i = NGetArraySize(&ps->JointArray); i != 0; i--, pj++)
	{
		pj->OldPosition = pj->CurrentPosition;
		NVec3Set(&pj->vSpeed, 0.0f, 0.0f, 0.0f);
		NVec3Set(&pj->vForcesAccumulator, 0.0f, 0.0f, 0.0f);
	}
}

// ------------------------------------------------------------------------------------------
// void NDeletePhysicStructureGeometry
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
void NDeletePhysicStructureGeometry(NSTRUCTURE *pstructure, NGEOMETRY *pgeometry)
{
	NSKIN *pskin;
	Nu32 i;
	Nu32 geomid;
#ifdef _DEBUG
	Nu32 nb_linked_skin;
#endif
	// First of ALL analyze the SKINs and this geometry links
	// In Other words, is there any Skins linked with the geometry ?
	// Each time a Skin is linked: erase it !
#ifdef _DEBUG
	nb_linked_skin = 0;
#endif
	pskin = (NSKIN *)NGetFirstArrayPtr(&pstructure->SkinArray);
	for (i = 0; i < pstructure->SkinArray.Size; i++, pskin++)
	{
		if (pskin->common.pGeometry == pgeometry)
		{
			NEraseArrayElementPtr(&pstructure->SkinArray, (NBYTE *)pskin, NSkinDestructor_ArrayCallBack);
			i--;
			pskin--;
#ifdef _DEBUG
			nb_linked_skin++;
#endif
		}
		else
		{
			// "SKIN Pointer on Geometry" to Index
			pskin->common.pGeometry = (NGEOMETRY *)NGetIncludedGeometryIndex(pstructure->pRenderable, pskin->common.pGeometry);
		}
	}
	// Just in case, this check warn if an "empty" geometry still having SKIN(s) linked with.
	NErrorIf(!pgeometry->Mesh.VertexArray.Size && nb_linked_skin, NERROR_NON_NULL_VALUE); // It should never happen !
	// ... and an empty geometry has to be ... EMPTY !
	NErrorIf(!pgeometry->Mesh.VertexArray.Size && pgeometry->Mesh.PrimitiveArray.Size, NERROR_NON_NULL_VALUE); // It should never happen !

	// keeping deleted geometry index ... and delete
	geomid = NGetIncludedGeometryIndex(pstructure->pRenderable, pgeometry);
	NDeleteIncludedGeometry(pstructure->pRenderable, pgeometry);

	// SKIN Index to pointers
	pskin = (NSKIN *)pstructure->SkinArray.pFirst;
	for (i = pstructure->SkinArray.Size; i != 0; i--, pskin++)
	{
		if ((Nu32)pskin->common.pGeometry > geomid)
			pskin->common.pGeometry = NGetIncludedGeometryByIndex(pstructure->pRenderable, (Nu32)(pskin->common.pGeometry) - 1);
		else
			pskin->common.pGeometry = NGetIncludedGeometryByIndex(pstructure->pRenderable, (Nu32)pskin->common.pGeometry);
	}
}

void NDeleteEmptyPhysicStructureGeometries(NSTRUCTURE *pstructure)
{
	Nu32 i, j, g;
	NSKIN *pskin;
	NGEOMETRY *pgeometry;

	pgeometry = (NGEOMETRY *)pstructure->pRenderable->GeomArray.pFirst;
	for (i = 0; i < pstructure->pRenderable->GeomArray.Size; i++, pgeometry++)
	{
		if (!pgeometry->Mesh.VertexArray.Size)
		{
			NErrorIf(pgeometry->Mesh.PrimitiveArray.Size, NERROR_NON_NULL_VALUE); // Empty ... is Empty !!!
			pskin = (NSKIN *)NGetFirstArrayPtr(&pstructure->SkinArray);
			for (j = pstructure->SkinArray.Size; j != 0; j--, pskin++)
			{
				NErrorIf(pskin->common.pGeometry == pgeometry, NERROR_INCONSISTENT_VALUES); // pGeometry is an empty geometry, so if a skin keep a link with it ... it's BAD !
				// "SKIN Pointer on Geometry" to Index
				pskin->common.pGeometry = (NGEOMETRY *)NGetIncludedGeometryIndex(pstructure->pRenderable, pskin->common.pGeometry);
			}
			// keeping deleted geometry index ... and delete
			g = i;
			NDeleteIncludedGeometry(pstructure->pRenderable, pgeometry);
			i--;
			pgeometry--;
			// SKIN Index to pointers
			pskin = (NSKIN *)pstructure->SkinArray.pFirst;
			for (j = pstructure->SkinArray.Size; j != 0; j--, pskin++)
			{
				if ((Nu32)pskin->common.pGeometry > g)
					pskin->common.pGeometry = NGetIncludedGeometryByIndex(pstructure->pRenderable, (Nu32)(pskin->common.pGeometry) - 1);
				else
					pskin->common.pGeometry = NGetIncludedGeometryByIndex(pstructure->pRenderable, (Nu32)pskin->common.pGeometry);
			}
		}
	}
}
// TODO: Move this part of code into NUT area
/*
void Draw_StructureList(NXNODELIST *pxlist)
{
	// TODO remove directmode calls
#ifdef _NWINDOWS
	NSTRUCTURE	*pstructure;
	DWORD		i;
	NCONSTRAINT	*pspring;
	NCONSTRAINT	*pconst;
	NJOINT		*pj;

	NVEC3	*pv;
	NVEC3		v;

	NXNODE		*pxn;

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	pxn = XFIRST(pxlist);
	while( XVALID(pxn,pxlist) )
	{
		pstructure = (NSTRUCTURE*)NGetXNodeX(pxn);

		//Draw Springs:
		glBegin(GL_LINES);
		glColor4f(1,0,0,1);
		pspring = (NCONSTRAINT*)NGetFirstArrayPtr(&pstructure->SpringArray);
		for(i=0;i<NGetArraySize(&pstructure->SpringArray);i++)
		{
			pv=&pspring->pjA->CurrentPosition;
			glVertex3f(pv->x,pv->y,pv->z);
			pv=&pspring->pjB->CurrentPosition;
			glVertex3f(pv->x,pv->y,pv->z);

			pspring++;
		}
		glEnd();

		//Draw Simple Constraints:
		glBegin(GL_LINES);
		glColor4f(0,1,0,1);
		pconst = (NCONSTRAINT*)NGetFirstArrayPtr(&pstructure->SimpleConstraintArray);
		for(i=0;i<NGetArraySize(&pstructure->SimpleConstraintArray);i++)
		{
			pv=&pconst->pjA->CurrentPosition;
			glVertex3f(pv->x,pv->y,pv->z);
			pv=&pconst->pjB->CurrentPosition;
			glVertex3f(pv->x,pv->y,pv->z);

			pconst++;
		}
		glEnd();



		// Draw Joint
		glBegin(GL_LINES);
		glColor4f(1,1,1,1);
		pj = (NJOINT*)NGetFirstArrayPtr(&pstructure->JointArray);
		for(i=0;i<NGetArraySize(&pstructure->JointArray);i++)
		{
			pv=&pj->CurrentPosition;

			NVec3Copy(&v,pv);
			v.x-=2.0f;
			glVertex3f(v.x,v.y,v.z);

			NVec3Copy(&v,pv);
			v.x+=2.0f;
			glVertex3f(v.x,v.y,v.z);

			NVec3Copy(&v,pv);
			v.y-=2.0f;
			glVertex3f(v.x,v.y,v.z);

			NVec3Copy(&v,pv);
			v.y+=2.0f;
			glVertex3f(v.x,v.y,v.z);

			pj++;
		}
		glEnd();

		// Draw AABB
		glBegin(GL_LINE_STRIP);

		if(ISFLAG_ON(pstructure->Flags,BIT_0))
			glColor4f(1,1,1,1);
		else
			glColor4f(0.5f,0,0,0.5f);

		glVertex3f(pstructure->Min.x,pstructure->Min.y,0);
		glVertex3f(pstructure->Min.x,pstructure->Max.y,0);
		glVertex3f(pstructure->Max.x,pstructure->Max.y,0);
		glVertex3f(pstructure->Max.x,pstructure->Min.y,0);
		glVertex3f(pstructure->Min.x,pstructure->Min.y,0);
		glEnd();

		XNEXT(pxn);
	}
#endif
}*/