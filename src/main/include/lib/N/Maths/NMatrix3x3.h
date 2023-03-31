#pragma once
/*
#ifndef __NMATRIX3x3_H
#define __NMATRIX3x3_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NMatrix3x3.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Maths/NMath.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// A matrix3x3 
typedef struct
{
	union
	{
		// Floats table(s)...
		Nf32		f[9];				// Simple List of Floats ... DON'T MATCH WITH OPENGL
		Nf32		ft[3][3];			// 2 entries Table of Floats
		NVEC3		V3[3];				// 3 Vectors (NVEC3)

		// Explicit Names ...
		struct							// Simple List of floats but with explicit Names linked with the 2 entries table description.
		{								//					for example: f12 and ft[1][2] point to the value.
			Nf32	f00, f01, f02;		// -> X Axis
			Nf32	f10, f11, f12;		// -> Y Axis
			Nf32	f20, f21, f22;		// -> Z Axis
		};

		struct							// Side,Up,Forward Nomenclature...
		{
			NVEC3	Side;
			NVEC3	Up;
			NVEC3	Forward;
		};

		struct							// Axis names Nomenclature ...
		{
			NVEC3	XAxis;
			NVEC3	YAxis;
			NVEC3	ZAxis;
		};

		struct							// Axis names Nomenclature when using Matrix3x3 as 2D homogeneous Matrix...
		{
			NVEC3	_XAxis;
			NVEC3	_YAxis;
			NVEC3	Origin;
		};

	};
}NMATRIX3x3;

void NIdentityMatrix3x3(NMATRIX3x3 *mat);
void NMulMatrix3x3(NMATRIX3x3 *matresult, const NMATRIX3x3 *mat1, const NMATRIX3x3 *mat2 );
void NScaleMatrix3x3f(NMATRIX3x3 *matresult,const Nf32 sx,const Nf32 sy,const Nf32 sz );
void NScaleMatrix3x3(NMATRIX3x3 *matresult, const NVEC3 *s);
void NTransposeMatrix3x3O( NMATRIX3x3 *pmatresult, const NMATRIX3x3 *pmat );
void NTransposeMatrix3x3( NMATRIX3x3 *pmatresult, const NMATRIX3x3 *pmat );
void NMulVector2ByMatrix3x3O(NVEC2 *vr, const NMATRIX3x3 *mat, const NVEC2 *v);
void NMulVector2ByMatrix3x3(NVEC2 *vr, const NMATRIX3x3 *mat, const NVEC2 *v);
void NMulVector3ByMatrix3x3O(NVEC3 *vr, const NMATRIX3x3 *mat, const NVEC3 *v);
void NMulVector3ByMatrix3x3(NVEC3 *vr, const NMATRIX3x3 *mat, const NVEC3 *v);
void NRotationMatrix3x3_AxisX(NMATRIX3x3 *pmat, Nf32 angx_rad );
//void NFastRotationMatrix3x3_AxisX(NMATRIX3x3 *pmat, Nu32 nanglex );
void NRotationMatrix3x3_AxisY(NMATRIX3x3 *pmat, Nf32 angy_rad );
//void NFastRotationMatrix3x3_AxisY(NMATRIX3x3 *pmat, Nu32 nangley );
void NRotationMatrix3x3_AxisZ(NMATRIX3x3 *pmat, Nf32 angz_rad );
//void NFastRotationMatrix3x3_AxisZ(NMATRIX3x3 *pmat, Nu32 nanglez );

// void NForwardVectorMatrix3x3(NMATRIX3x3 *pbase, const NVEC3 *punit_forward);
// void NForwardVectorMatrix3x3Ex(NMATRIX3x3 *pbase, const NVEC3 *pto, const NVEC3 *pfrom, const NVEC3 *pup);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#ifdef __cplusplus
}
#endif	// __cpluplus

#endif	// __NMATRIX3x3_H 
*/
