#ifndef __NQUATERNION_H
#define __NQUATERNION_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NQuaternion.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Maths/NVec4f32.h"

#include "NMatrix.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
// A Quaternion
typedef NVEC4 NQUATERNION;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
void				NFastQuaternionAxisRotation(NQUATERNION *pquat, const NVEC3 *v, const Nu32 ang );
void				NIdentityQuaternion(NQUATERNION *pquat);
void				NMatrixToQuaternion(NQUATERNION *quat, const NMATRIX *mat );
void				NMulQuaternion( NQUATERNION *res, const NQUATERNION *q1, const NQUATERNION *q2 );
void				NQuaternionAxisRotation(NQUATERNION *pquat, const NVEC3 *v, const Nf32 ang );
void				NQuaternionRotation(NQUATERNION *pquat, const Nf32 rx, const Nf32 ry, const Nf32 rz );
void				NQuaternionSlerp(NQUATERNION *res, const NQUATERNION *from, const NQUATERNION *to, const Nf32 t );
void				NQuaternionToMatrix( NMATRIX *mat, const NQUATERNION *quat );

Nf32				NQuaternionLength(const NQUATERNION *q);
Nf32				NQuaternionNormalize(NQUATERNION *q);

void				NMulVectorByQuat( NVEC3 *vr, const NVEC3 *v, const NQUATERNION *quat );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NQUATERNION_H 

