#pragma once
/*
#ifndef __NVEC2F32_H
#define __NVEC2F32_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NVec2f32.h										**
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
			Nf32	x;
			Nf32	y;
		};
		Nf32 coord[2];
	};
}NVEC2;
typedef NVEC2		NVEC2f32;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
inline	Nf32		NVec2DotProduct(const NVEC2* v1,const NVEC2* v2){	return (v1->x * v2->x + v1->y * v2->y); }
inline	void		NVec2Add(NVEC2* vr, const NVEC2* v1, const NVEC2* v2){ vr->x = v1->x + v2->x;vr->y = v1->y + v2->y; }
inline	void		NVec2AddTo(NVEC2* v1, const NVEC2* v2){ v1->x += v2->x; v1->y += v2->y; }
inline	Nf32		NVec2FastLength(const NVEC2* v){ return NFastSqrt(v->x*v->x + v->y*v->y); }
Nf32				NVec2FastNormalize(NVEC2* v);
inline	Nf32		NVec2Length(const NVEC2* v){ return sqrt(v->x*v->x + v->y*v->y); }
Nf32				NVec2Normalize(NVEC2* v);
inline	void		NVec2Set(NVEC2* v, const Nf32 x, const Nf32 y){ v->x = x; v->y = y; }
inline	void		NVec2Scale(NVEC2* dest, const NVEC2* src, const Nf32 factor){ dest->x = src->x*factor; dest->y = src->y*factor; }
inline	void		NVec2ScaleBy(NVEC2* v, const Nf32 factor){ v->x *= factor; v->y *=factor; }
inline	Nf32		NVec2SquareLength(const NVEC2* v){ return (v->x*v->x + v->y*v->y); }
inline	Nf32		NVec2ManhattanLength(const NVEC2* v) { return (NABS(v->x) + NABS(v->y)); }
inline	void		NVec2Sub(NVEC2* vr, const NVEC2* v1, const NVEC2* v2){vr->x = v1->x - v2->x; vr->y = v1->y - v2->y; }
inline	void		NVec2SubFrom(NVEC2* v1, const NVEC2* v2){v1->x -= v2->x; v1->y -= v2->y; }
inline	void		NVec2Null(NVEC2*v){v->x = 0;v->y=0;}
inline	Nbool		NVec2IsUnit(const NVEC2 *v){ return (NABS(1.0f-NVec2Length(v)) < NF32_EPSILON_VECTOR_LENGTH ? 1:0); }
inline	void		NVec2Lerp(NVEC2* vr, const NVEC2* vorigin, const NVEC2* v,const Nf32 t ){vr->x = vorigin->x + (v->x - vorigin->x)*t;vr->y = vorigin->y + (v->y - vorigin->y)*t;}
inline	void		NVec2QLerp(NVEC2 *R, const Nf32 factor, const NVEC2 *A, const NVEC2 *B, const NVEC2 *C, const NVEC2 *D )
{
	NVEC2	L1,L2,L3,L4,L5;
	NVec2Lerp(&L1,A,B,factor);
	NVec2Lerp(&L2,B,C,factor);
	NVec2Lerp(&L3,C,D,factor);
	NVec2Lerp(&L4,&L1,&L2,factor);
	NVec2Lerp(&L5,&L2,&L3,factor);
	NVec2Lerp(R,&L4,&L5,factor);
}
/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NVEC2F32_H 
*/
