#ifndef __NTRANSFORMATIONTREE_H
#define __NTRANSFORMATIONTREE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NTransformationTree.h							**
// **																					**
// ***************************************************************************************
#include "NType.h"
#include "NUsualCoordinates.h"
#include "NFlags.h"
#include "./Maths/NVec3f32.h"
#include "NMatrix.h"
#include "NQuaternion.h"
#include "./Containers/NHNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Structures & Define									**
// ***************************************************************************************
// Flags for Transformations TREE
#define FLAG_NTRANSFORM_HNODE_UPDATE_LOCAL_TRANSFORMATIONS		BIT_0
#define FLAG_NTRANSFORM_HNODE_UPDATE_WORLD_TRANSFORMATIONS		BIT_1
#define FLAG_NTRANSFORM_HNODE_UPDATE_TAG						BIT_2	// !!! HAS TO BE "BIT_2" LIKE 'FLAG_NUI_TRANSFORM_HNODE_UPDATE_TAG' !!!
// A Transform HNode
typedef struct
{
	NHIERARCHY_NODEDEF
	
	Nu8			Flags;
	Nu8			Available_Nu8;
	Nu16		ReferenceCount;
	NVEC3		vTranslation;		// To store local translations of the Node
	NVEC3		vScale;				// To Store local Scale of the Node
	NQUATERNION	QuatRotation;		// To store local rotations of the Node

	NMATRIX		LocalMatrix;		// The Local Matrix of the node Computed from vTanslatio, vScale and QuatRotation datas of the NTRANSFORM_HNODE
	NMATRIX		WorldMatrix;		// The World Matrix of the node. (all transformations computed into from the node and his parent hierarchy)
}NTRANSFORM_HNODE;


// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
void				NTransformationTree_RootInit();
void				NDeleteTransformationTree();
void				NPurgeTransformationTree();


NTRANSFORM_HNODE*	NGetTransformHNodeRoot();


NTRANSFORM_HNODE*	NSetupTransformHNode( NTRANSFORM_HNODE* parent, NTRANSFORM_HNODE* ptr );
NTRANSFORM_HNODE*	NCreateTransformHNode( NTRANSFORM_HNODE* parent );
void				NClearTransformHNode(NTRANSFORM_HNODE *pthn);
void				NDeleteTransformHNode(NTRANSFORM_HNODE* pthn);

void				NPurgeTransformHNode(NTRANSFORM_HNODE	*pthn);

inline const NVEC3*	NGetTransformHNodePositionPtr(const NTRANSFORM_HNODE* pn){return &pn->vTranslation;}
inline void				NGetTransformHNodePosition(NVEC3* vm, const NTRANSFORM_HNODE* pn ){*vm=pn->vTranslation;}
inline Nf32				NGetTransformHNodePositionX( const NTRANSFORM_HNODE* pn ){return pn->vTranslation.x;}
inline Nf32				NGetTransformHNodePositionY( const NTRANSFORM_HNODE* pn ){return pn->vTranslation.y;}
inline Nf32				NGetTransformHNodePositionZ( const NTRANSFORM_HNODE* pn ){return pn->vTranslation.z;}

inline const NVEC3*	NGetTransformHNodeScalePtr(const NTRANSFORM_HNODE* pn){return &pn->vScale;}
inline void				NGetTransformHNodeScale(NVEC3* vs, const NTRANSFORM_HNODE* pn ){*vs=pn->vScale;}

// Move
void				NMoveTransformHNode(NTRANSFORM_HNODE* pn, const NVEC3* vm);
void				NMoveTransformHNodef(NTRANSFORM_HNODE* pn, const Nf32 x, const Nf32 y, const Nf32 z);
void				NMoveTransformHNodeX(NTRANSFORM_HNODE* pn, const Nf32 x);
void				NMoveTransformHNodeY(NTRANSFORM_HNODE* pn, const Nf32 y);
void				NMoveTransformHNodeZ(NTRANSFORM_HNODE* pn, const Nf32 z);
void				NMoveTransformHNodeCoord(NTRANSFORM_HNODE* pn, const NV3COORD_ENUM coord, const Nf32 f);
// Rotate
void				NRotateTransformHNode(NTRANSFORM_HNODE* pn, const Nf32 rx, const Nf32 ry, const Nf32 rz);
void				NRotateTransformHNodeNormQuat(NTRANSFORM_HNODE* pn, const NQUATERNION *pnormalized_quat);
// SCale
void				NScaleTransformHNode(NTRANSFORM_HNODE* pn, const NVEC3* vs);
void				NScaleTransformHNodef(NTRANSFORM_HNODE* pn, const Nf32 scale_x, const Nf32 scale_y, const Nf32 scale_z );


// Set Position
void				NSetTransformHNodePosition(NTRANSFORM_HNODE* pn, const NVEC3* vm);
void				NSetTransformHNodePositionf(NTRANSFORM_HNODE* pn, const Nf32 x, const Nf32 y, const Nf32 z);
void				NSetTransformHNodePositionX( NTRANSFORM_HNODE* pn, const Nf32 x );
void				NSetTransformHNodePositionY( NTRANSFORM_HNODE* pn, const Nf32 y );
void				NSetTransformHNodePositionZ( NTRANSFORM_HNODE* pn, const Nf32 z );
void				NSetTransformHNodePositionCoord( NTRANSFORM_HNODE* pn, const NV3COORD_ENUM coord, const Nf32 f );
// Set Rotation
void				NSetTransformHNodeRotation(NTRANSFORM_HNODE* pn, const Nf32 rx, const Nf32 ry, const Nf32 rz);
void				NSetTransformHNodeRotationQuat(NTRANSFORM_HNODE* pn, const NQUATERNION *pquat);

// Set Scale
void				NSetTransformHNodeScale(NTRANSFORM_HNODE* pn, const NVEC3* vs);
void				NSetTransformHNodeScalef(NTRANSFORM_HNODE* pn, const Nf32 x, const Nf32 y, const Nf32 z);



NTRANSFORM_HNODE*	NSetTransformHNodeParent_EndInsert( NTRANSFORM_HNODE * pparent, NTRANSFORM_HNODE * ptr );
NTRANSFORM_HNODE*	NSetTransformHNodeParent_BeginningInsert( NTRANSFORM_HNODE * pparent, NTRANSFORM_HNODE * ptr );
#define				NSetTransformHNodeParent( a,b ) NSetTransformHNodeParent_EndInsert( a,b )

Nbool				NUpdateTransformationHNode(NTRANSFORM_HNODE*pnode);
void				NUpdateTransformationTree();

// Counting

inline void			NRegisterTransformHNodeReference(NTRANSFORM_HNODE*pnode){NErrorIf(pnode->ReferenceCount==NU16_MAX,NERROR_VALUE_OUTOFRANGE);pnode->ReferenceCount++;}
inline void			NUnregisterTransformHNodeReference(NTRANSFORM_HNODE*pnode){NErrorIf(pnode->ReferenceCount==0,NERROR_VALUE_OUTOFRANGE);pnode->ReferenceCount--;}
inline Nu16			NGetTransformHNodeReferenceCount(const NTRANSFORM_HNODE*pnode){return pnode->ReferenceCount;}



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NTRANSFORMATIONTREE_H

