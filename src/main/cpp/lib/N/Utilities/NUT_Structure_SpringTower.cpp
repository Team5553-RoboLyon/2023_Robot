#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"

#include "lib/N/NStructure.h"
#include "NUT_Structure.h"
/*

NSTRUCTURE* NUTSetUpSpringyTowerStructure(NSTRUCTURE *pstructure,const NVEC3 *pos,const NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder)
{
	Nu32				a,j;
	NSTRUCTURE_DESC	spbuilder;
	NJOINT_DESC		jointbuilder;
	NCONSTRAINT_BUILDER	constraintbuilder;
	NGEOMETRY			geom;
	NGEOMETRY			*pgeom;
	NGEOMETRY_DESC		gdesc;
	NSKIN_DESC		skinbuilder;

	// ==========================
	// SetUp the PhysicStructure
	// ==========================
	memset(&spbuilder,0,sizeof(NSTRUCTURE_DESC));
	if( ISFLAG_ON(pbuilder->Flags,FLAG_SPRINGYSTRUCTURE_CREATESKIN) )
	{
		FLAG_ON(spbuilder.Flags,FLAG_NSTRUCTUREBUILDER_WITH_RENDERABLE);
		spbuilder.RenderableGeometryCapacity	= 1; // else 0 thanks to memset ...
		spbuilder.SkinArrayCapacity				= 1; // else 0 thanks to memset ...
	}
	spbuilder.JointsArrayCapacity				= 2*( pbuilder->StagesNb + 1 );
	spbuilder.SpringsArrayCapacity				= 5 * pbuilder->StagesNb + 1;
	spbuilder.MinMaxConstraintsArrayCapacity	= 0;
	spbuilder.SimpleConstraintsArrayCapacity	= 0;
	spbuilder.SpecialConstraintsArrayCapacity	= 0;
	NVec3Copy(&spbuilder.vGravity,&pbuilder->vGravity);
	NSetupPhysicStructure(pstructure,&spbuilder);

	// ==========================
	// Create the Joints
	// ==========================
	// First Vertical Row
	NVec3Copy( &jointbuilder.CurrentPosition,pos );
	NVec3Copy( &jointbuilder.OldPosition,&jointbuilder.CurrentPosition );
	NVec3Copy( &jointbuilder.shift_Vector,&pbuilder->ToNextLineShiftingVector );
	NVec3Set( &jointbuilder.vSpeed,0,0,0 );
	jointbuilder.fMass = pbuilder->JointMass;
	jointbuilder.fRadius = pbuilder->JointRadius;
	NCreateJointsRange( pstructure,pbuilder->StagesNb +1,&jointbuilder );
	NFreezeJoint((NJOINT*)NGetFirstArrayPtr( &pstructure->JointArray ) );

	// second Vertical Row
	NVec3Copy( &jointbuilder.CurrentPosition,pos );
	NVec3AddTo(&jointbuilder.CurrentPosition,&pbuilder->ToNextColumnShiftingVector);
	NVec3Copy(&jointbuilder.OldPosition,&jointbuilder.CurrentPosition);
	NCreateJointsRange(pstructure,pbuilder->StagesNb+1,&jointbuilder);
	NFreezeJoint((NJOINT*)NGetArrayPtr(&pstructure->JointArray,pbuilder->StagesNb+1));

	// ==========================
	// Create the Springs
	// ==========================
	constraintbuilder.Constraint_Type	= NSTRUCTURE_ELEMENT_SPRING;
	constraintbuilder.fFriction			= pbuilder->Spring_Friction;
	constraintbuilder.fK				= pbuilder->Spring_K;
	constraintbuilder.fLengthRatio		= pbuilder->Spring_RestLengthRatio;
	FLAG_ON( constraintbuilder.wFlags,FLAG_CONSTRAINTBUILDER_USE_LENGTH_RATIO );
	FLAG_ON( constraintbuilder.wFlags,FLAG_CONSTRAINTBUILDER_USE_LENGTH_BETWEEN_JOINTS_AS_CONSTRAINTLENGTH );

	// Springs, First vertical row
	NCreateConstraintsRange(pstructure,&constraintbuilder,0,pbuilder->StagesNb,NSTRUCTURE_SPIN);
	// Springs, Second vertical row
	NCreateConstraintsRange(pstructure,&constraintbuilder,pbuilder->StagesNb+1,pbuilder->StagesNb,NSTRUCTURE_SPIN);

	// Springs, Horizontal ladder rung
	a = pbuilder->StagesNb+1;
	for(j=0;j<a;j++)
	{
		NCreateConstraint(pstructure, &constraintbuilder, j, j+a);
	}

	// Springs, X
	for(j=0;j<(int)pbuilder->StagesNb;j++)
	{
		NCreateConstraint(pstructure, &constraintbuilder, j, j+pbuilder->StagesNb+2);
	}

	for(j=1;j<a;j++)
	{
		NCreateConstraint(pstructure, &constraintbuilder, j, j+a-1);
	}

	// ==========================
	// Create the SKIN (if asked )
	// ==========================
	if( ISFLAG_ON(pbuilder->Flags,FLAG_SPRINGYSTRUCTURE_CREATESKIN) )
	{
		NSetUpGeometryDesc_Triangle(&gdesc,NGetMostSuitableStateSet(pbuilder->pSkinTexture,pbuilder->SkinAccumulatorSpecFlags), pbuilder->StagesNb*2, spbuilder.JointsArrayCapacity, pbuilder->pSkinTexture,NULL );
		pgeom = NRenderableGeometryPushBack(pstructure->pRenderable,NSetUpGeometry(&geom,&gdesc),NTRUE);

		memset(&skinbuilder,0,sizeof(NSKIN_DESC));
		skinbuilder.Grid.Structure_Scheme			= NSKIN_SCHEME_GRID_QUADS;
		skinbuilder.Grid.Ref_First					= 0;
		skinbuilder.Grid.Ref_RangeSize				= spbuilder.JointsArrayCapacity;
		skinbuilder.Grid.Ref_ColumnSize				= spbuilder.JointsArrayCapacity/2;
		skinbuilder.Grid.Ref_StructureElementType	= NSTRUCTURE_ELEMENT_JOINT;
		skinbuilder.Grid.UVOrigin.x					= pbuilder->SkinUVOrigin.x;
		skinbuilder.Grid.UVOrigin.y					= pbuilder->SkinUVOrigin.y;
		skinbuilder.Grid.UVTiling.x					= pbuilder->SkinUVTiling.x;
		skinbuilder.Grid.UVTiling.y					= pbuilder->SkinUVTiling.y;
		NCreateSkin_Grid(pstructure,pgeom,&skinbuilder);
	}
	return pstructure;

}
NSTRUCTURE* NUTCreateSpringyTowerStructure(const NVEC3 *pos,const NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder )
{
	return NUTSetUpSpringyTowerStructure(NEW(NSTRUCTURE),pos,pbuilder);
}



// ------------------------------------------------------------------------------------------
// NUT_EstimateSpringyTowerRestingState
// ------------------------------------------------------------------------------------------
// Description :
//				Resting situation calculation ONLY for a Springy tower.
//				WARNING this version (16/07/2013) is just a draft.
//				TODO: Create the final function ...
//
//
// ------------------------------------------------------------------------------------------
// In	:
//				ptower:			a valid pointer on a structure created with the function
//								'NUT_CreateSpringyTowerStructure'
//
//				updatedata:		NTRUE to update all structure data (joints position, speed,..)
//								to match with the estimated rest state calculation.
//								After processing the structure should be entirely into rest state.
//
//								NFALSE to only return the global structure length without any
//								structure data updating
// Out :
//				(Nf32) Global structure rest length.
//
// ------------------------------------------------------------------------------------------
Nf32 NUT_EstimateSpringyTowerRestingState(NSTRUCTURE *ptower, Nbool updatedata )
{
	Nf32	mass;
	Nf32	f,fl;
	Nf32	g;
	Nu32	springstages;
	Nu32	i;
	Nu32	j;
	NCONSTRAINT	*pspringA,*pspringB,*pspringXA,*pspringXB,*pspringH;
	NJOINT		*pjA,*pjB;
	NJOINT		*pjAm,*pjBm;
	NJOINT		*pjAprev,*pjBprev;
	Nf32		s,a,h1,h2,k;
	NVEC3	vx,vy,vz;
	NVEC3	v;

	fl = 0.0f;
	springstages= NGetArraySize(&ptower->JointArray)/2 - 1;				// Because a Springy Tower has 2 columns of joints, AND two vertical column of springs.

	// Pointers initialization
	pspringA	= (NCONSTRAINT*)NGetFirstArrayPtr(&ptower->SpringArray);		// Into a springy tower, all the springs of  the first springs vertical column are in the beginning of the springs list ...
	pspringB	= pspringA + springstages;//(NCONSTRAINT*)NGetArrayPtr(&ptower->SpringArray, springstages );		// pspringA + springstages;
	pspringH	= pspringB + springstages;//(NCONSTRAINT*)NGetArrayPtr(&ptower->SpringArray, springstages*2 );	// pspringB + springstages;
	pspringXA	= pspringH + springstages + 1;//(NCONSTRAINT*)NGetArrayPtr(&ptower->SpringArray, springstages*3+1 );	// pspringH + springstages + 1;
	pspringXB	= pspringXA+ springstages;//(NCONSTRAINT*)NGetArrayPtr(&ptower->SpringArray, springstages*4+1 );		// pspringXA+ springstages;

	pjAprev = pjA = (NJOINT*)NGetFirstArrayPtr(&ptower->JointArray);					// Into a springy tower, all the joints of  the first joints vertical column are in the beginning of the joints list ...
	pjBprev = pjB = pjA + springstages + 1;

	// Create an orthonormal based on the gravity vector
	// vy:
	NVec3Copy(&vy,&ptower->vGravity);
	g = NVec3Normalize(&vy);

	if(updatedata)
	{
		// vz:
		NVec3Sub(&vx,&pjB->CurrentPosition,&pjA->CurrentPosition);
		NVec3CrossProduct(&vz,&vx,&vy);
		f = NVec3Length(&vz);

		if(f<NF32EPSILON)
			updatedata = NFALSE;
		else
		{
			NVec3Normalize(&vz);
			// vx:
			NVec3CrossProduct(&vx,&vy,&vz);
		}

		if(updatedata)
		{
			NResetJointDynamic(pjA);
			NResetJointDynamic(pjB);
		}
	}


	pjA ++; pjB ++;	// the first joint of the column is locked.
	pspringH ++; // because we dont use the top edge of each "trapeze"

	for(i=0;i<springstages;i++,pspringA++,pspringB++,pspringH++,pspringXA++,pspringXB++,pjAprev=pjA,pjBprev=pjB,pjA++,pjB++)
	{
		// We are going to replace (just for this calculation)
		// the 4 stage's springs by 1 unique spring, with:
		//	K = k1+k2+k3+k4.
		//  notice that we ignore horizontal spring !  (pspringH)
		k = pspringA->fK + pspringB->fK;// + pspringXA->fK + pspringXB->fK;

		//	R = ... we can only use the 4 springs restlength to estimate the rest length of this spring.
		//		without using the joint position. BECAUSE we don't know when this function is called
		//		i mean, the current joint positions can be "exotic" because of a hard collision or something else ...
		//		... And the only sure thing we know that a springy tower was made in a way where all the springs restlength were calculated
		//	so to calculate that we are going to use the "heron formula" and the "standard formula" to calculate the trapeze surface...

		// TRI 1:
		s	= 0.5f*( pspringA->fRestLength + pspringXA->fRestLength + pspringH->fRestLength );
		a	= sqrt( s*(s-pspringA->fRestLength)*(s-pspringXA->fRestLength)*(s-pspringH->fRestLength) );
		h1	= 2.0f*a/pspringH->fRestLength;

		// TRI 2:
		s	= 0.5f*( pspringB->fRestLength + pspringXB->fRestLength + pspringH->fRestLength );
		a	= sqrt( s*(s-pspringB->fRestLength)*(s-pspringXB->fRestLength)*(s-pspringH->fRestLength) );
		h2	= 2.0f*a/pspringH->fRestLength;

		// if we have a symmetric Tower, h1 == h2. But... we would have a lot of unsymmetric Tower ...
		// so ... just the simplest way to estimate ( maybe not the best one...)
		if( (h1-h2)>NF32EPSILON )
		{
			h1=h1*h2*0.5f;
		}

		// well done, right now we are going to use h1 as the unique spring rest length !
		// now we need to know the global weight bellow this unique spring !
		// All the joint below act on it with the sum of their mass

		pjAm=pjA;
		pjBm=pjB;

		mass = 0.0f;
		for(j=i;j<springstages;j++,pjAm++,pjBm++)
		{
			mass += (pjAm->fMass + pjBm->fMass);
		}

		f = ( h1 + (mass*g)/k);
		fl += f;

		if(updatedata)
		{
			NVec3Scale(&v,&vy,f);
			NVec3Add(&pjA->CurrentPosition,&pjAprev->CurrentPosition,&v);
			NVec3Add(&pjB->CurrentPosition,&pjBprev->CurrentPosition,&v);

			f = sqrt( pspringA->fRestLength*pspringA->fRestLength - h1*h1 );
			NVec3Scale(&v,&vx,f);
			NVec3AddTo(&pjA->CurrentPosition,&v);

			f = sqrt( pspringB->fRestLength*pspringB->fRestLength - h1*h1 ); // we are not using h2, because we assume that h1 == h2 !
			NVec3Scale(&v,&vx,-f);
			NVec3AddTo(&pjB->CurrentPosition,&v);

			NResetJointDynamic(pjA);
			NResetJointDynamic(pjB);
		}

	}

	return fl;
}

*/
