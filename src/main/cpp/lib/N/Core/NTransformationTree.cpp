#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../NTransformationTree.h"
#include "lib/N/NErrorHandling.h"
#include "../Timer/NThnAnimationPlayer.h"

// Globales
NTRANSFORM_HNODE *pTransformationRoot;
NNODELIST NTransformationTree;
Nbool bUpdateTransformationtree;

/*
// ----------------------------------------------
// Private local function, used only in this .cpp
Nu8 NTransformHNodeCreation(NHIERARCHY_NODE*pnode)
{
	NTRANSFORM_HNODE *ptr;

	ptr = (NTRANSFORM_HNODE*)pnode;

	NVec3Set(&ptr->vTranslation,0.0f,0.0f,0.0f);
	NVec3Set(&ptr->vScale,1.0f,1.0f,1.0f);

	NIdentityQuaternion(&ptr->QuatRotation);
	NIdentityMatrix(&ptr->LocalMatrix);
	NIdentityMatrix(&ptr->WorldMatrix);

	return 1;
}
// Private local function, used only in this .cpp
// ----------------------------------------------
*/

void NTransformationTree_RootInit()
{
	NSetupNodeList(&NTransformationTree); // TODO suppress this list Just keep the Rootnode as start point.
	pTransformationRoot = NCreateTransformHNode(NULL);
	NRegisterTransformHNodeReference(pTransformationRoot);

	// Insert the Root Node into the Transformation Tree Node List (it will be the only one in this list)
	NNodeInsertBeginning((NNODE *)pTransformationRoot, &NTransformationTree);
}

void NDeleteTransformationTree()
{
	NUnregisterTransformHNodeReference(pTransformationRoot);
	NDeleteTransformHNode(pTransformationRoot);
	pTransformationRoot = NULL;
}

Nbool NUpdateTransformationHNode(NTRANSFORM_HNODE *pnode)
{
	Nu8 force_children_update;
	NTRANSFORM_HNODE *pa;
	NMATRIX rot, scale, trans, res;

	// ===========================
	// Compute Node transformation
	// ===========================
	force_children_update = 0;

	// LOCAL:
	if (ISFLAG_ON(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS))
	{
		NQuaternionToMatrix(&rot, &pnode->QuatRotation);
		NTranslationMatrix(&trans, &pnode->vTranslation);
		NScaleMatrix(&scale, &pnode->vScale);

		NMulMatrix(&res, &scale, &rot);
		NMulMatrix(&pnode->LocalMatrix, &res, &trans);

		FLAG_OFF(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);

		FLAG_ON(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS);
	}

	// WORLD:
	if (ISFLAG_ON(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS))
	{
		if (pnode->pParent)
		{
			NMulMatrix(&pnode->WorldMatrix, &pnode->LocalMatrix, &((NTRANSFORM_HNODE *)(pnode->pParent))->WorldMatrix);
			// NMulMatrix(&((NTRANSFORM_HNODE*)(pnode->pParent))->WorldMatrix,&pnode->LocalMatrix,&pnode->WorldMatrix);
		}
		else
		{
			pnode->WorldMatrix = pnode->LocalMatrix;
		}

		FLAG_OFF(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS);
		force_children_update = 1;

		// Toggled Tag to let outer objects knowing changes happen !!! ( Simple but efficient !)
		FLAG_TOGGLE(pnode->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_TAG);
	}

	// ========================
	// ... And all its children
	// ========================
	pa = (NTRANSFORM_HNODE *)(pnode->ChildrenList.pFirst);
	if (force_children_update)
	{
		while (NNodeIsValid((NNODE *)pa, &pnode->ChildrenList))
		{
			FLAG_ON(pa->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS); // Force to update world transformations for all the children

			NUpdateTransformationHNode(pa);
			pa = (NTRANSFORM_HNODE *)(pa->pNext);
		}
	}
	else
	{
		while (NNodeIsValid((NNODE *)pa, &pnode->ChildrenList))
		{
			NUpdateTransformationHNode(pa);
			pa = (NTRANSFORM_HNODE *)(pa->pNext);
		}
	}

	//	pnode->Flags = 0; // set All Flags to OFF
	return NTRUE;
}

void NUpdateTransformationTree()
{
	// TODO: Hudge optimization:
	// avoir plusieurs arbres s�par�s plut�t qu'une racine commune et unique !
	// ainsi on parcourt la liste des racines de chaques arbres et on s'enfonce dans l'arbre
	// si son bupdate continue est � ON.
	// il faudra veiller � ajouter un pointeur dans chaque htransformnode sur la racine de son arbre
	if (bUpdateTransformationtree)
	{
		NUpdateTransformationHNode(pTransformationRoot);
		bUpdateTransformationtree = NFALSE;
	}
}

NTRANSFORM_HNODE *NGetTransformHNodeRoot()
{
	return pTransformationRoot;
}

NTRANSFORM_HNODE *NSetupTransformHNode(NTRANSFORM_HNODE *parent, NTRANSFORM_HNODE *ptr)
{
	// Is it a primary HNode ?
	if (!parent)
		parent = pTransformationRoot;

	memset(ptr, 0, sizeof(NTRANSFORM_HNODE));
	NHNodeChildrenListInitialization((NHIERARCHY_NODE *)ptr);
	NHNodeSetParent_EndInsert((NHIERARCHY_NODE *)parent, (NHIERARCHY_NODE *)ptr);

	NVec3Set(&ptr->vTranslation, 0.0f, 0.0f, 0.0f);
	NVec3Set(&ptr->vScale, 1.0f, 1.0f, 1.0f);

	NIdentityQuaternion(&ptr->QuatRotation);
	NIdentityMatrix(&ptr->LocalMatrix);
	NIdentityMatrix(&ptr->WorldMatrix);

	// The transformation node World Matrix  has to be updated to take into account its parents transformations
	// (only if there is a valid parent, of course)
	if (parent)
	{
		FLAG_ON(ptr->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS); // Force to update world transformations for this new transformation Node
		bUpdateTransformationtree = NTRUE;
	}
	return ptr;
}

NTRANSFORM_HNODE *NCreateTransformHNode(NTRANSFORM_HNODE *parent)
{
	return NSetupTransformHNode(parent, NEW(NTRANSFORM_HNODE));
}

// ------------------------------------------------------------------------------------------
// void NDeleteTransformHNode
// ------------------------------------------------------------------------------------------
// Description :
//	Delete A transformHnode. That means:
//		1) Remove it from it's potential owner list ( parent children list )
//		2) Then, clear it's content.
//		3) Then, free the memory it used.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDeleteTransformHNode(NTRANSFORM_HNODE *pthn)
{
	NErrorIf(pthn->pParent && !pthn->pNext, NERROR_NODE_POINTER_INCONSISTENCY);
	NErrorIf(pthn->pParent && !pthn->pPrevious, NERROR_NODE_POINTER_INCONSISTENCY);
	NErrorIf(pthn->pNext && !pthn->pPrevious, NERROR_NODE_POINTER_INCONSISTENCY);
	NErrorIf(!pthn->pNext && pthn->pPrevious, NERROR_NODE_POINTER_INCONSISTENCY);
	// Remove the NNode from the list it belongs to... ( parent children list or other ... )
	if (pthn->pNext) // || pthn->pPrevious
		NNodeRemove((NNODE *)pthn);

	NClearTransformHNode(pthn);
	Nfree(pthn);
}

void NClearTransformHNode(NTRANSFORM_HNODE *pthn)
{
	// In debug mode, do this test here before
	NErrorIf(pthn->ReferenceCount, NERROR_NON_NULL_VALUE); // It's not cool !!! that means USer try to clear a Thn which is referenced by some other objects ...
#ifdef _DEBUG
	if (pthn->ReferenceCount)
		printf("\n CLEAR THN ---> [%X] . RefCount = %d", (unsigned int)pthn, pthn->ReferenceCount);
#endif
	// It's not the purpose of clear function to make it orphan !
	NErrorIf(pthn->pNext || pthn->pPrevious, NERROR_NODE_IS_NOT_ORPHAN);

	// Check if Node is referenced by some ThnAnimation Player and delete them...
	NDeleteAllThnAnimationPlayer(pthn);

	// Deleting All children (and All Children of each child, etc ... recursive method man !)
	NHIERARCHY_NODE *pa, *pb;
	pa = (NHIERARCHY_NODE *)pthn->ChildrenList.pFirst;
	while (NNodeIsValid((NNODE *)pa, &pthn->ChildrenList))
	{
		pb = (NHIERARCHY_NODE *)pa->pNext;
		NDeleteTransformHNode((NTRANSFORM_HNODE *)pa);
		pa = pb;
	}
	// Clear content, NHIERARCHY_NODE excepted because after clear  NTRANSFORM_HNODE
	pthn->Flags = 0;
	NVec3Set(&pthn->vTranslation, 0, 0, 0);
	NVec3Set(&pthn->vScale, 0, 0, 0);
	Nmem0(&pthn->QuatRotation, NQUATERNION);
	Nmem0(&pthn->LocalMatrix, NMATRIX);
	Nmem0(&pthn->WorldMatrix, NMATRIX);
}

void NPurgeTransformHNode(NTRANSFORM_HNODE *pthn)
{
	NTRANSFORM_HNODE *pb;
	NTRANSFORM_HNODE *pa = (NTRANSFORM_HNODE *)pthn->ChildrenList.pFirst;

	while (NIS_NODE_VALID(pa, &pthn->ChildrenList))
	{
		pb = (NTRANSFORM_HNODE *)pa->pNext;
		NPurgeTransformHNode(pa);
		pa = pb;
	}

	// Only the Non referenced THN without any children are going to be deleted !
	if (!pthn->ReferenceCount && NIS_NODELIST_EMPTY(&pthn->ChildrenList))
	{
		NErrorIf(pthn == pTransformationRoot, NERROR_SYSTEM_CHECK); // It's not suppose to happen !
		NDeleteTransformHNode(pthn);
	}
}

void NPurgeTransformationTree()
{
	NPurgeTransformHNode(pTransformationRoot);
}

NTRANSFORM_HNODE *NSetTransformHNodeParent_EndInsert(NTRANSFORM_HNODE *pparent, NTRANSFORM_HNODE *ptr)
{
	NHNodeSetParent_EndInsert((NHIERARCHY_NODE *)pparent, (NHIERARCHY_NODE *)ptr);

	// The transformation tree has to be updated
	FLAG_ON(ptr->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS); // Force to update world transformations for this new transformation Node ...
	bUpdateTransformationtree = NTRUE;										 // ... to take care of its parents transformation
	return ptr;
}

NTRANSFORM_HNODE *NSetTransformHNodeParent_BeginningInsert(NTRANSFORM_HNODE *pparent, NTRANSFORM_HNODE *ptr)
{
	NHNodeSetParent_BeginningInsert((NHIERARCHY_NODE *)pparent, (NHIERARCHY_NODE *)ptr);

	// The transformation tree has to be updated
	FLAG_ON(ptr->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS); // Force to update world transformations for this new transformation Node ...
	bUpdateTransformationtree = NTRUE;										 // ... to take care of its parents transformation
	return ptr;
}

// =============================================================
// Function to manipulate TransformHnode (Rotate,Move and Scale)
// =============================================================
void NRotateTransformHNode(NTRANSFORM_HNODE *pn, const Nf32 rx, const Nf32 ry, const Nf32 rz)
{
	NQUATERNION qt;

	NQuaternionRotation(&qt, rx, ry, rz);
	NQuaternionNormalize(&qt);
	NMulQuaternion(&pn->QuatRotation, &qt, &pn->QuatRotation);

	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NRotateTransformHNodeNormQuat(NTRANSFORM_HNODE *pn, const NQUATERNION *pnormalized_quat)
{
	NMulQuaternion(&pn->QuatRotation, pnormalized_quat, &pn->QuatRotation);
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodeRotation(NTRANSFORM_HNODE *pn, const Nf32 rx, const Nf32 ry, const Nf32 rz)
{
	NQuaternionRotation(&pn->QuatRotation, rx, ry, rz);
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NSetTransformHNodeRotationQuat(NTRANSFORM_HNODE *pn, const NQUATERNION *pquat)
{
	pn->QuatRotation = *pquat;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NMoveTransformHNode(NTRANSFORM_HNODE *pn, const NVEC3 *vm)
{
	NVec3AddTo(&pn->vTranslation, vm);
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NMoveTransformHNodef(NTRANSFORM_HNODE *pn, const Nf32 x, const Nf32 y, const Nf32 z)
{
	pn->vTranslation.x += x;
	pn->vTranslation.y += y;
	pn->vTranslation.z += z;

	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NMoveTransformHNodeX(NTRANSFORM_HNODE *pn, const Nf32 x)
{
	pn->vTranslation.x += x;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NMoveTransformHNodeY(NTRANSFORM_HNODE *pn, const Nf32 y)
{
	pn->vTranslation.y += y;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NMoveTransformHNodeZ(NTRANSFORM_HNODE *pn, const Nf32 z)
{
	pn->vTranslation.z += z;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NMoveTransformHNodeCoord(NTRANSFORM_HNODE *pn, const NV3COORD_ENUM coord, const Nf32 f)
{
	pn->vTranslation.coord[coord] += f;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodePosition(NTRANSFORM_HNODE *pn, const NVEC3 *vm)
{
	pn->vTranslation = *vm;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodePositionf(NTRANSFORM_HNODE *pn, const Nf32 x, const Nf32 y, const Nf32 z)
{
	NVec3Set(&pn->vTranslation, x, y, z);
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
void NSetTransformHNodePositionX(NTRANSFORM_HNODE *pn, const Nf32 x)
{
	pn->vTranslation.x = x;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodePositionY(NTRANSFORM_HNODE *pn, const Nf32 y)
{
	pn->vTranslation.y = y;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodePositionZ(NTRANSFORM_HNODE *pn, const Nf32 z)
{
	pn->vTranslation.z = z;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodePositionCoord(NTRANSFORM_HNODE *pn, const NV3COORD_ENUM coord, const Nf32 f)
{
	pn->vTranslation.coord[coord] = f;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NScaleTransformHNode(NTRANSFORM_HNODE *pn, const NVEC3 *vs)
{
	pn->vScale.x *= vs->x;
	pn->vScale.y *= vs->y;
	pn->vScale.z *= vs->z;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NScaleTransformHNodef(NTRANSFORM_HNODE *pn, const Nf32 scale_x, const Nf32 scale_y, const Nf32 scale_z)
{
	pn->vScale.x *= scale_x;
	pn->vScale.y *= scale_y;
	pn->vScale.z *= scale_z;

	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodeScale(NTRANSFORM_HNODE *pn, const NVEC3 *vs)
{
	pn->vScale = *vs;
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}

void NSetTransformHNodeScalef(NTRANSFORM_HNODE *pn, const Nf32 x, const Nf32 y, const Nf32 z)
{
	NVec3Set(&pn->vScale, x, y, z);
	FLAG_ON(pn->Flags, FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS | FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS);
	bUpdateTransformationtree = NTRUE;
}
