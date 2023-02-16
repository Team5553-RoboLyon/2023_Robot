#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"

#include "../NStructure.h"
#include "NUT_Structure.h"


Nbool NUT_IterateUntilPhysicStructureStabilization(NSTRUCTURE *ps, const Nu32 time_max,const Nf32 error_tolerance)
{
	Nu32		i,j,iteration;
	NVEC3	min,max;

	// Iteration for 1 second
	iteration=(Nu32)(1.0f/fSimulationTimeStep)+1;
	NUpdateStructureAABB(ps);

	for(i=0;i<time_max;i++)
	{
		for(j=0;j<iteration;j++)
		{
			ps->pAccumulateForce(ps,fSimulationTimeStep);
			ps->pIntegration(ps,fSimulationTimeStep);
			ps->pSatisfyConstraint(ps,fSimulationTimeStep);
		}

		min = ps->JointsAABB.VMin;
		max = ps->JointsAABB.VMax;
		
		NUpdateStructureAABB(ps);

		// Stabilization estimation
		if( NABS( (ps->JointsAABB.fXMax - ps->JointsAABB.fXMin)-(max.x - min.x) )<= error_tolerance )
			if( NABS( (ps->JointsAABB.fYMax - ps->JointsAABB.fYMin)-(max.y - min.y) )<= error_tolerance )
				if( NABS( (ps->JointsAABB.fZMax - ps->JointsAABB.fZMin)-(max.z - min.z) )<= error_tolerance )
					return NTRUE;
		
	}

	return NFALSE;
}