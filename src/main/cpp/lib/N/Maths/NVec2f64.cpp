// ==========================================================================================
// Vector.cpp
// ------------------------------------------------------------------------------------------
// Author  :
// Created : 03/01/2012
// Updated :
// ------------------------------------------------------------------------------------------
// Vectors management.
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/Maths/NVec2f64.h"
#include "lib/N/NType.h"
// -------------------------------------------------------------------------------------------
// NVec2Normalize
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
Nf64 NVec2f64Normalize(NVEC2f64 *v)
{
	Nf64 n, sn;

	n = sqrt(v->x * v->x + v->y * v->y);
	if (n >= NF32_EPSILON_VECTOR_LENGTH)
	{
		sn = 1.0 / n;
		v->x *= sn;
		v->y *= sn;
	}

	return n;
}

// -------------------------------------------------------------------------------------------
// NVec2FastNormalize
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
Nf64 NVec2f64FastNormalize(NVEC2f64 *v)
{
	Nf64 n, sn;

	n = NFastSqrt64(v->x * v->x + v->y * v->y);
	if (n >= NF32_EPSILON_VECTOR_LENGTH)
	{
		sn = 1.0 / n;
		v->x *= sn;
		v->y *= sn;
	}

	return n;
}
