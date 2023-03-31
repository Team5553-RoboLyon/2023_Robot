#pragma once
/*
#ifndef __NVEC3F32_H
#define __NVEC3F32_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NVec3f32.h										**
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
			Nf32 x;
			Nf32 y;
			Nf32 z;
		};
		Nf32	coord[3];
	};
}NVEC3;
typedef NVEC3		NVEC3f32;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
inline	void		NVec3CrossProduct(NVEC3* vr,const NVEC3* v1,const NVEC3* v2){vr->x=(v1->y*v2->z)-(v1->z*v2->y);vr->y=(v1->z*v2->x)-(v1->x*v2->z);vr->z=(v1->x*v2->y)-(v1->y*v2->x);}
inline	Nf32		NVec3DotProduct(const NVEC3* v1,const NVEC3* v2){	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z); }
inline	void		NVec3Add(NVEC3* vr, const NVEC3* v1, const NVEC3* v2){ vr->x = v1->x + v2->x;vr->y = v1->y + v2->y;vr->z = v1->z + v2->z; }
inline	void		NVec3AddTo(NVEC3* v1, const NVEC3* v2){ v1->x += v2->x; v1->y += v2->y; v1->z += v2->z; }
inline	Nf32		NVec3FastLength(const NVEC3* v){ return NFastSqrt(v->x*v->x + v->y*v->y + v->z*v->z); }
Nf32				NVec3FastNormalize(NVEC3* v);
inline	Nf32		NVec3Length(const NVEC3* v){ return sqrt(v->x*v->x + v->y*v->y + v->z*v->z); }
Nf32				NVec3Normalize(NVEC3* v);
inline	void		NVec3Set(NVEC3* v, const Nf32 x, const Nf32 y, const Nf32 z){ v->x = x; v->y = y; v->z = z; }
inline	void		NVec3Scale(NVEC3* dest, const NVEC3* src, const Nf32 factor){ dest->x = src->x*factor; dest->y = src->y*factor; dest->z = src->z*factor; }
inline	void		NVec3ScaleBy(NVEC3* v,const Nf32 factor){ v->x *= factor; v->y *= factor; v->z *= factor; }
inline	Nf32		NVec3SquareLength(const NVEC3* v){ return (v->x*v->x + v->y*v->y + v->z*v->z); }
inline	Nf32		NVec3ManhattanLength(const NVEC3* v) { return (NABS(v->x)+NABS(v->y) + NABS(v->z)); }
inline	void		NVec3Sub(NVEC3* vr, const NVEC3* v1, const NVEC3* v2){vr->x = v1->x - v2->x; vr->y = v1->y - v2->y; vr->z = v1->z - v2->z; }
inline	void		NVec3SubFrom(NVEC3* v1, const NVEC3* v2){v1->x -= v2->x; v1->y -= v2->y; v1->z -= v2->z; }
inline	void		NVec3Null(NVEC3*v){v->x = 0;v->y=0;v->z=0;}
inline	Nbool		NVec3IsUnit(const NVEC3 *v){ return (NABS(1.0f-NVec3Length(v)) < NF32_EPSILON_VECTOR_LENGTH ? 1:0); }
inline	void		NVec3Lerp(NVEC3* vr, const NVEC3* vorigin, const NVEC3* v,const Nf32 t ){vr->x = vorigin->x + (v->x - vorigin->x)*t;vr->y = vorigin->y + (v->y - vorigin->y)*t;vr->z = vorigin->z + (v->z - vorigin->z)*t;}

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NVEC3F32_H 
*/
