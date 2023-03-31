#pragma once
/*
#ifndef __NVEC2F64_H
#define __NVEC2F64_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NVec2f64.h										**
// **																					**
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Maths/NMath.h"
#include "lib/N/Maths/NVecLimits.h"
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// **								Structures											**
// ***************************************************************************************
typedef struct
{
	union
	{
		struct 
		{
			Nf64	x;
			Nf64	y;
		};
		Nf64 coord[2];
	};
}NVEC2f64;


// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
inline	Nf64		NVec2f64DotProduct(const NVEC2f64* v1,const NVEC2f64* v2){	return (v1->x * v2->x + v1->y * v2->y); }
inline	void		NVec2f64Add(NVEC2f64* vr, const NVEC2f64* v1, const NVEC2f64* v2){ vr->x = v1->x + v2->x;vr->y = v1->y + v2->y; }
inline	void		NVec2f64AddTo(NVEC2f64* v1, const NVEC2f64* v2){ v1->x += v2->x; v1->y += v2->y; }
//inline	Nf64		NVec2f64FastLength(const NVEC2f64* v){ return NFastSqrt(v->x*v->x + v->y*v->y); }
inline	Nf64		NVec2f64Length(const NVEC2f64* v) { return sqrt(v->x * v->x + v->y * v->y); }
Nf64				NVec2f64FastNormalize(NVEC2f64* v);
Nf64				NVec2f64Normalize(NVEC2f64* v);
inline	void		NVec2f64Set(NVEC2f64* v, const Nf32 x, const Nf32 y){ v->x = x; v->y = y; }
inline	void		NVec2f64Scale(NVEC2f64* dest, const NVEC2f64* src, const Nf64 factor){ dest->x = src->x*factor; dest->y = src->y*factor; }
inline	void		NVec2f64ScaleBy(NVEC2f64* v, const Nf32 factor){ v->x *= factor; v->y *=factor; }
inline	Nf64		NVec2f64SquareLength(const NVEC2f64* v){ return (v->x*v->x + v->y*v->y); }
inline	Nf64		NVec2f64ManhattanLength(const NVEC2f64* v) { return (NABS(v->x) + NABS(v->y)); }
inline	void		NVec2f64Sub(NVEC2f64* vr, const NVEC2f64* v1, const NVEC2f64* v2){vr->x = v1->x - v2->x; vr->y = v1->y - v2->y; }
inline	void		NVec2f64SubFrom(NVEC2f64* v1, const NVEC2f64* v2){v1->x -= v2->x; v1->y -= v2->y; }
inline	void		NVec2f64Null(NVEC2f64*v){v->x = 0;v->y=0;}
inline	Nbool		NVec2f64IsUnit(const NVEC2f64 *v){ return (NABS(1.0-NVec2f64Length(v)) < NF32_EPSILON_VECTOR_LENGTH ? 1:0); }
inline	void		NVec2f64Lerp(NVEC2f64* vr, const NVEC2f64* vorigin, const NVEC2f64* v,const Nf64 t ){vr->x = vorigin->x + (v->x - vorigin->x)*t;vr->y = vorigin->y + (v->y - vorigin->y)*t;}
inline	void		NVec2f64QLerp(NVEC2f64 *R, const Nf64 factor, const NVEC2f64 *A, const NVEC2f64 *B, const NVEC2f64 *C, const NVEC2f64 *D )
{
	NVEC2f64	L1,L2,L3,L4,L5;
	NVec2f64Lerp(&L1,A,B,factor);
	NVec2f64Lerp(&L2,B,C,factor);
	NVec2f64Lerp(&L3,C,D,factor);
	NVec2f64Lerp(&L4,&L1,&L2,factor);
	NVec2f64Lerp(&L5,&L2,&L3,factor);
	NVec2f64Lerp(R,&L4,&L5,factor);
}

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NVEC2F32_H 
*/
