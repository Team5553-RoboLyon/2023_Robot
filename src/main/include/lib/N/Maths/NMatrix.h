#pragma once
/*
#ifndef __NMATRIX_H
#define __NMATRIX_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NMatrix.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Maths/NVec4f32.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
// A matrix 
/*
typedef struct
{
	union
	{
		// Floats table(s)...
		Nf32		f[16];				// Simple List of Floats ... like OpenGl.
		Nf32		Tf[4][4];			// 2 entries Table of Floats
		NVEC4	V4[4];				// 4 Vectors (NVEC4)
		
		// Explicit Names ...
		struct							// Simple List of floats but with explicit Names linked with the 2 entries table description.
		{								//					for example: f12 and ft[1][2] point to the value.
			Nf32	f00, f01, f02, f03; // -> X Axis
			Nf32	f10, f11, f12, f13; // -> Y Axis
			Nf32	f20, f21, f22, f23; // -> Z Axis
			Nf32	f30, f31, f32, f33; // -> Translations
		};
		
		struct							// Side,Up,Forward, Translation Nomenclature...
		{
			NVEC4	Side;
			NVEC4	Up;
			NVEC4	Forward;
			NVEC4	Translation;
		};

		struct							// Axis names Nomenclature ...
		{
			NVEC4	XAxis;
			NVEC4	YAxis;
			NVEC4	ZAxis;
			NVEC4	Origin;
		};
	};
}NMATRIX;
*/

// A matrix 
typedef union
{
	// Floats table(s)...
	Nf32		f[16];				// Simple List of Floats ... like OpenGl.
	Nf32		Tf[4][4];			// 2 entries Table of Floats
	NVEC4		V4[4];				// 4 Vectors (NVEC4)

	// Explicit Names ...
	struct							// Simple List of floats but with explicit Names linked with the 2 entries table description.
	{								//					for example: f12 and ft[1][2] point to the value.
		Nf32	f00, f01, f02, f03; // -> X Axis
		Nf32	f10, f11, f12, f13; // -> Y Axis
		Nf32	f20, f21, f22, f23; // -> Z Axis
		Nf32	f30, f31, f32, f33; // -> Translations
	};

	struct							// Side,Up,Forward, Translation Nomenclature...
	{
		NVEC4	Side;
		NVEC4	Up;
		NVEC4	Forward;
		NVEC4	Translation;
	};

	struct							// Axis names Nomenclature ...
	{
		NVEC4	XAxis;
		NVEC4	YAxis;
		NVEC4	ZAxis;
		NVEC4	Origin;
	};
}NMATRIX;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Matrix 4x4
void				NIdentityMatrix(NMATRIX *mat);
void				NComposeMatrix(NMATRIX* matresult, const NMATRIX3x3* mat3x3, const NVEC3f32* po);
void				NMulMatrix(NMATRIX *matresult, const NMATRIX *mat1, const NMATRIX *mat2 );
void				NTransposeMatrix( NMATRIX *pmatresult, const NMATRIX *pmat );
void				NTransposeMatrixO( NMATRIX *pmatresult, const NMATRIX *pmat );
void				NOrthoMatrixf( NMATRIX *mat, const Nf32 left, const Nf32 right, const Nf32 bottom, const Nf32 top, const Nf32 znear, const Nf32 zfar );

void				NFastRotationMatrix_AxisX(NMATRIX *pmat, Nu32 nanglex );
void				NFastRotationMatrix_AxisY(NMATRIX *pmat, Nu32 nangley );
void				NFastRotationMatrix_AxisZ(NMATRIX *pmat, Nu32 nanglez );
void				NRotationMatrix_AxisX(NMATRIX *pmat, Nf32 angx_rad );
void				NRotationMatrix_AxisY(NMATRIX *pmat, Nf32 angy_rad );
void				NRotationMatrix_AxisZ(NMATRIX *pmat, Nf32 angz_rad );
void				NScaleMatrixf(NMATRIX *matresult, const Nf32 sx, const Nf32 sy, const Nf32 sz );
void				NScaleMatrix(NMATRIX *matresult, const NVEC3 *s );
void				NTranslationMatrixf(NMATRIX *matresult,const Nf32 sx,const Nf32 sy,const Nf32 sz );
void				NTranslationMatrix(NMATRIX *matresult, const NVEC3 *v );

void				NMulVector4ByMatrixO(NVEC4 *vr, const NMATRIX *mat, const NVEC4 *v);
void				NMulVector4ByMatrix(NVEC4 *vr, const NMATRIX *mat, const NVEC4 *v);
void				NMulVector3ByMatrixO(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v);
void				NMulVector3ByMatrix(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v);
void				NMulVector2ByMatrixO(NVEC3* vr, const NMATRIX* mat, const NVEC2* v);

//					These functions use only the "Mat3x3" part of the NMATRIX structure (which is a 4x4 matrix)
//					Be aware that NMATRIX is a 4x4 matrix not a 3x3 Matrix ... ( functions working with 3x3 Matrix use 3x3 suffix )
void				NMulMatrix3(NMATRIX *matresult, const NMATRIX *mat1, const NMATRIX *mat2 );
void				NMulVector3ByMatrix3O(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v);
void				NMulVector3ByMatrix3(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v);
void				NMulVector2ByMatrix3O(NVEC3* vr, const NMATRIX* mat, const NVEC2 *v);

// void				NForwardVectorMatrix(NMATRIX *pmat, const NVEC3 *punit_forward);
// void				NForwardVectorMatrixEx(NMATRIX *pmat, const NVEC3 *pto, const NVEC3 *pfrom, const NVEC3 *pup);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NMATRIX_H 
*/
