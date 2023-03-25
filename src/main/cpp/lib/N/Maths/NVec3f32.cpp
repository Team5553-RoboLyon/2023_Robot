// ==========================================================================================
// Vector.cpp
// ------------------------------------------------------------------------------------------
// Author  : Nzt
// Created : 03/01/2012
// Updated :
// ------------------------------------------------------------------------------------------
// Vectors management.
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NType.h"
#include "lib/N/Maths/NMath.h"
// -------------------------------------------------------------------------------------------
// NVec3Normalize
// -------------------------------------------------------------------------------------------
//	Normalize a vector
// -------------------------------------------------------------------------------------------
// In  :
//		v: Vector pointer.
//
// Out :
//		Vector length before normalization.
//
// -------------------------------------------------------------------------------------------
Nf32 NVec3Normalize(NVEC3 *v)
{
	Nf32 n, sn;

	n = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	if (n >= NF32_EPSILON_VECTOR_LENGTH)
	{
		sn = 1.0f / n;
		v->x *= sn;
		v->y *= sn;
		v->z *= sn;
	}

	return n;
}

// -------------------------------------------------------------------------------------------
// NVec3FastNormalize
// -------------------------------------------------------------------------------------------
//	Normalize a vector
// -------------------------------------------------------------------------------------------
// In  :
//		v: Vector pointer.
//
// Out :
//		Vector length before normalization.
//
// -------------------------------------------------------------------------------------------
Nf32 NVec3FastNormalize(NVEC3 *v)
{
	Nf32 n, sn;

	n = NFastSqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	if (n >= NF32_EPSILON_VECTOR_LENGTH)
	{
		sn = 1.0f / n;
		v->x *= sn;
		v->y *= sn;
		v->z *= sn;
	}

	return n;
}
