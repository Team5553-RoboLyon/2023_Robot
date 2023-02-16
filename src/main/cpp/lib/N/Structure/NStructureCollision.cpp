#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"

#include "../NStructure.h"

NXNODELIST	*NpActivePhysicStructureCollisionCoupleList;

NXNODELIST* NBindStructureCollisionCoupleXNodeList(NXNODELIST *pxlist)
{
	NXNODELIST *poldxlist;

	poldxlist = NpActivePhysicStructureCollisionCoupleList;
	NpActivePhysicStructureCollisionCoupleList = pxlist;

	return poldxlist;
}

NSTRUCTURE_COLLISONCOUPLE* NSetUpStructureCollisionCouple(NSTRUCTURE_COLLISONCOUPLE* pcol, NSTRUCTURE *p1,NSTRUCTURE *p2, NSTRUCTURE_COLLISION_RESPONSE_FCT collisionResponse_callback)
{
	memset(pcol,0,sizeof(NSTRUCTURE_COLLISONCOUPLE));

	pcol->pStructureA		= p1;
	pcol->pStructureB		= p2;
	pcol->CollisionResponse = collisionResponse_callback;

	p1->CollisionReferencesCounter ++;
	p2->CollisionReferencesCounter ++;
	
	return pcol;
}

NSTRUCTURE_COLLISONCOUPLE* NCreateStructureCollisionCouple(NSTRUCTURE *p1,NSTRUCTURE *p2, NSTRUCTURE_COLLISION_RESPONSE_FCT collisionResponse_callback)
{
	return  NSetUpStructureCollisionCouple(NEW(NSTRUCTURE_COLLISONCOUPLE),p1,p2,collisionResponse_callback);
}

void NClearStructureCollisionCouple(void *pcol)
{
	((NSTRUCTURE_COLLISONCOUPLE*)pcol)->pStructureA->CollisionReferencesCounter --;
	((NSTRUCTURE_COLLISONCOUPLE*)pcol)->pStructureB->CollisionReferencesCounter --;
	memset(pcol,0,sizeof(NSTRUCTURE_COLLISONCOUPLE));
}
void NDeleteStructureCollisionCouple(void *pcol)
{
	NClearStructureCollisionCouple(pcol);
	Nfree(pcol);
}

Nu32 NDeleteAll_StructureCollisionCouplesXNode_LinkedWith_PhysicStructure(NXNODELIST *pcolcouplexlist, const NSTRUCTURE *ps, NNODE_X_DESTRUCTOR_CALLBACK  xclear)
{
	Nu32							destroyed = 0;
	NSTRUCTURE_COLLISONCOUPLE		*pcol;
	NXNODE							*pxn,*pxnb;

	pxn = XFIRST(pcolcouplexlist);
	while( XVALID(pxn,pcolcouplexlist) )
	{
		pcol = (NSTRUCTURE_COLLISONCOUPLE*)pxn->pX;
		if( (pcol->pStructureA == ps)||(pcol->pStructureB == ps) )
		{
			pxnb = pxn->pNext;
			NDeleteXNode(pxn,xclear); 
			destroyed ++;
			pxn = pxnb;
		}
		else
			pxn=pxn->pNext;
	}

	return destroyed;
}

// ------------------------------------------------------------------------------------------
// NGetStructureJointsAABB
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the AABB of all joints for one Physic Structure.
//	Be aware that the Min/Max value automatically calculated for each structure during physic process
//	include joint radius.
//	This function doesn't integrate joint radius in its calculation. It uses joints position only.
// ------------------------------------------------------------------------------------------
// In	:
//			pstruct			a valid pointer on a structure
//			paabb			a valid pointer on a AABB
// Out :
//			return the pointer AABB pointer passed as param 1. 
//			( in that way its possible to pass this function as parameter of an another one ...) 
// ------------------------------------------------------------------------------------------
NAABB* NGetStructureJointsAABB(NAABB *paabb,const NSTRUCTURE *pstruct)
{
	NJOINT		*pj;
	NVEC3	*pv,*pmin,*pmax;
	Nu32		nb = NGetArraySize(&pstruct->JointArray);
	Nu32		i;

	pj=(NJOINT*)NGetFirstArrayPtr(&pstruct->JointArray);
	pv = &pj->CurrentPosition;
	pmin = &paabb->VMin;
	pmax = &paabb->VMax;
	*pmin=*pv;
	*pmax=*pv;
	pj++;
	
	// Without Joint Radius
	for(i=1;i<nb;i+=1,pj++)
	{
		pv = &pj->CurrentPosition;

		if( pv->x < pmin->x)
			pmin->x = pv->x;
		
		if( pv->x > pmax->x)
			pmax->x = pv->x;

		if( pv->y < pmin->y)
			pmin->y = pv->y;
		
		if( pv->y > pmax->y)
			pmax->y = pv->y;

		// We don't need this for Snakes !
		if( pv->z < pmin->z)
			pmin->z = pv->z;
		
		if( pv->z > pmax->z)
			pmax->z = pv->z;
	}

	return paabb;
}

void NUpdateStructureAABB(NSTRUCTURE *pstruct)
{
	Nu32		i;
	NVEC3	*pv;
	NJOINT		*pj=(NJOINT*)NGetFirstArrayPtr(&pstruct->JointArray);
	NVEC3	*pmin = &pstruct->JointsAABB.VMin;
	NVEC3	*pmax = &pstruct->JointsAABB.VMax;

	*pmin = *pmax = pj->CurrentPosition;
	pj++;

	// With Joint Radius
	for(i=NGetArraySize(&pstruct->JointArray)-1;i!=0;i--,pj++)
	{
		pv = &pj->CurrentPosition;

		if( (pv->x - pj->fRadius) < pmin->x)
			pmin->x = pv->x-pj->fRadius;
		
		if( (pv->x + pj->fRadius) > pmax->x)
			pmax->x = pv->x + pj->fRadius;

		if( (pv->y - pj->fRadius) < pmin->y)
			pmin->y = pv->y - pj->fRadius;
		
		if( (pv->y + pj->fRadius) > pmax->y)
			pmax->y = pv->y + pj->fRadius;
		
		// We don't need this for Snakes !
		if( (pv->z - pj->fRadius) < pmin->z)
			pmin->z = pv->z - pj->fRadius;
		
		if( (pv->z + pj->fRadius) > pmax->z)
			pmax->z = pv->z + pj->fRadius;
	}
}

Nbool NStructureCollisionAABBvsAABB(const NSTRUCTURE *psA,const NSTRUCTURE *psB)
{
	if(psA->JointsAABB.fXMax < psB->JointsAABB.fXMin)
		return NFALSE;

	if(psA->JointsAABB.fXMin > psB->JointsAABB.fXMax)
		return NFALSE;

	if(psA->JointsAABB.fYMax < psB->JointsAABB.fYMin)
		return NFALSE;

	if(psA->JointsAABB.fYMin > psB->JointsAABB.fYMax)
		return NFALSE;

	if(psA->JointsAABB.fZMax < psB->JointsAABB.fZMin)
		return NFALSE;

	if(psA->JointsAABB.fZMin > psB->JointsAABB.fZMax)
		return NFALSE;
	
	return NTRUE;
}



