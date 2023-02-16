#ifndef __NGEOMETRY_TEXTUREMATRIXANIMATION_H
#define __NGEOMETRY_TEXTUREMATRIXANIMATION_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NGeometryTextureMatrixAnimation.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NUsualCoordinates.h"
#include "../../NMatrix.h"
#include "../../NQuaternion.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------
typedef  union NUVMATRIXANIM		NUVMATRIXANIM;
typedef void (*NUVMATRIX_UPDATE_FCT)(NUVMATRIXANIM *ptexmat);

struct NUVMATRIXANIM_COMMON
{
	Nu32					ID;
	NMATRIX					Matrix;
	NUVMATRIX_UPDATE_FCT	Update;
};

struct NUVMATRIXANIM_LINEAR
{
	Nu32					ID;		//|
	NMATRIX					Matrix; //|-Common Part
	NUVMATRIX_UPDATE_FCT	Update; //|
	
	// TODO ... just some members to see ...
	Nu32					RotationSpeed[_NV3ORTHONORMAL_ENUM_SIZE];
	NVEC3				TranslationSpeed;
	NVEC3				ScaleSpeed;
};

typedef struct NUVMATRIXANIM_FULLKEY	NUVMATRIXANIM_FULLKEY;
struct  NUVMATRIXANIM_FULLKEY
{
	Nu32					Duration;			// Time to reach this key
	NVEC3				vTranslation;		// To store translation of the Key
	NVEC3				vScale;				// To Store Scale of the key
	NQUATERNION				QuatRotation;		// To store rotation of the Node
	NUVMATRIXANIM_FULLKEY	*pNext;				// Next KeyFrame
};
typedef struct 
{
	Nu32					ID;		//|
	NMATRIX					Matrix;	//|-Common Part
	NUVMATRIX_UPDATE_FCT	Update;	//|

	// TODO ... just some members to see ...
	Nu32					InternalStep;	// tick instead of timer 
	NUVMATRIXANIM_FULLKEY	*pfirstKey;
}NUVMATRIXANIM_FULLKEYFRAMED; // means full 4x4 matrix animation, with Rotation + translation + scale

union NUVMATRIXANIM
{
	NUVMATRIXANIM_COMMON			Common;
	NUVMATRIXANIM_FULLKEYFRAMED		FullKeyFramed;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRY_TEXTUREMATRIXUPDATER_H 

