// ==========================================================================================
// NMatrix.cpp
// ------------------------------------------------------------------------------------------
// Author	: Jean-Marie Nazaret
// Create	: 28/01/2012
// Modified : 
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
//#include "../Utilities/Maths/NUT_MathsMisc.h"
#include "lib/N/Maths/NMatrix.h"


// Notes: OpenGL, use a column-major convention.

// --------------------------------------------------------------------------- ---------------
// NIdentityMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	reset a Matrix to the Identity form.
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: "THE" Matrix
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NIdentityMatrix(NMATRIX *mat)
{
	mat->f00 = 1.0f; 	mat->f10 = 0.0f;	mat->f20 = 0.0f;	 mat->f30 = 0.0f; 						
	mat->f01 = 0.0f;	mat->f11 = 1.0f;	mat->f21 = 0.0f;	 mat->f31 = 0.0f; 
	mat->f02 = 0.0f;	mat->f12 = 0.0f;	mat->f22 = 1.0f;	 mat->f32 = 0.0f;
	mat->f03 = 0.0f;	mat->f13 = 0.0f;	mat->f23 = 0.0f;	 mat->f33 = 1.0f;
}

void NComposeMatrix(NMATRIX* matresult, const NMATRIX3x3* mat3x3, const NVEC3f32* po)
{
	matresult->f00 = mat3x3->f00; 	matresult->f10 = mat3x3->f10;	matresult->f20 = mat3x3->f20;	 matresult->f30 = po->x;
	matresult->f01 = mat3x3->f01;	matresult->f11 = mat3x3->f11;	matresult->f21 = mat3x3->f21;	 matresult->f31 = po->y;
	matresult->f02 = mat3x3->f02;	matresult->f12 = mat3x3->f12;	matresult->f22 = mat3x3->f22;	 matresult->f32 = po->z;
	matresult->f03 = 0.0f;			matresult->f13 = 0.0f;			matresult->f23 = 0.0f;			 matresult->f33 = 1.0f;
}

// --------------------------------------------------------------------------- ---------------
// NOrthoMatrixf
// ------------------------------------------------------------------------------------------
// Description :
//	Create an orthographic Matrix
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: "THE" Matrix
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NOrthoMatrixf( NMATRIX *mat, const Nf32 left, const Nf32 right, const Nf32 bottom, const Nf32 top, const Nf32 znear, const Nf32 zfar )
{
	Nf32	r_l, t_b, f_n;

	NErrorIf(!(right-left),NERROR_MATRIX_INVALID_VALUE);
	NErrorIf(!(top-bottom),NERROR_MATRIX_INVALID_VALUE);
	NErrorIf(!(zfar-znear),NERROR_MATRIX_INVALID_VALUE);

	r_l = 1.0f / (right-left);
	t_b = 1.0f / (top-bottom);
	f_n = 1.0f / (zfar-znear);
	
	// TODO: Check and apply: indeed, it seems strange to have those values for f03,f13,f23 and f33 ...

	mat->f00 = 2.0f*r_l;			mat->f10 = 0.0f;				mat->f20 = 0.0f;				mat->f30 = 0.0f;		
	mat->f01 = 0.0f;				mat->f11 = 2.0f*t_b;			mat->f21 = 0.0f;				mat->f31 = 0.0f;
	mat->f02 = 0.0f;				mat->f12 = 0.0f;				mat->f22 = -2.0f*f_n;			mat->f32 = 0.0f;
	mat->f03 = -(right+left)*r_l;	mat->f13 = -(top+bottom)*t_b;	mat->f23 = -(zfar+znear)*f_n;	mat->f33 = 1.0f;

/*
	mat->f00 = 2.0f*r_l;			mat->f01 = 0.0f;					mat->f02 = 0.0f;					mat->f03 = 0.0f;
	mat->f10 = 0.0f;				mat->f11 = 2.0f*t_b;				mat->f12 = 0.0f;					mat->f13 = 0.0f;
	mat->f20 = 0.0f;				mat->f21 = 0.0f;					mat->f22 = -2.0f*f_n;				mat->f23 = 0.0f;
	mat->f30 = -(right+left)*r_l;	mat->f31 = -(top+bottom)*t_b;		mat->f32 = -(zfar+znear)*f_n;		mat->f33 = 1.0f;
*/
/*
	mat->f00 = 2.0f*r_l;			mat->f01 = 0.0f;					mat->f02 = 0.0f;					mat->f03 = 0.0f;
	mat->f10 = 0.0f;				mat->f11 = 2.0f*t_b;				mat->f12 = 0.0f;					mat->f13 = 0.0f;
	mat->f20 = 0.0f;				mat->f21 = 0.0f;					mat->f22 = -2.0f*f_n;				mat->f23 = 0.0f;
	mat->f30 = 0.0f;	mat->f31 = 0.0f;		mat->f32 = 0.0f;		mat->f33 = 1.0f;
*/
}

// ------------------------------------------------------------------------------------------
// NMulMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Multiplie 2 matrices : matd=mat1*mat2.
//	Les matrices sont des matrices 4x4.
// ------------------------------------------------------------------------------------------
// In  :
//		mat1	: La 1� matrice.
//		mat2	: La 2� matrice.
//		matd	: La matrice de destination.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NMulMatrix(NMATRIX *matresult, const NMATRIX *mat1, const NMATRIX *mat2 )
{
	matresult->f00 = mat1->f00 * mat2->f00	 +	 mat1->f01 * mat2->f10	 +	 mat1->f02 * mat2->f20	 +	 mat1->f03 * mat2->f30;
	matresult->f01 = mat1->f00 * mat2->f01	 +	 mat1->f01 * mat2->f11	 +	 mat1->f02 * mat2->f21	 +	 mat1->f03 * mat2->f31;
	matresult->f02 = mat1->f00 * mat2->f02	 +	 mat1->f01 * mat2->f12	 +	 mat1->f02 * mat2->f22	 +	 mat1->f03 * mat2->f32;
	matresult->f03 = mat1->f00 * mat2->f03	 +	 mat1->f01 * mat2->f13	 +	 mat1->f02 * mat2->f23	 +	 mat1->f03 * mat2->f33;

	matresult->f10 = mat1->f10 * mat2->f00	 +	 mat1->f11 * mat2->f10	 +	 mat1->f12 * mat2->f20	 +	 mat1->f13 * mat2->f30;
	matresult->f11 = mat1->f10 * mat2->f01	 +	 mat1->f11 * mat2->f11	 +	 mat1->f12 * mat2->f21	 +	 mat1->f13 * mat2->f31;
	matresult->f12 = mat1->f10 * mat2->f02	 +	 mat1->f11 * mat2->f12	 +	 mat1->f12 * mat2->f22	 +	 mat1->f13 * mat2->f32;
	matresult->f13 = mat1->f10 * mat2->f03	 +	 mat1->f11 * mat2->f13	 +	 mat1->f12 * mat2->f23	 +	 mat1->f13 * mat2->f33;

	matresult->f20 = mat1->f20 * mat2->f00	 +	 mat1->f21 * mat2->f10	 +	 mat1->f22 * mat2->f20	 +	 mat1->f23 * mat2->f30;
	matresult->f21 = mat1->f20 * mat2->f01	 +	 mat1->f21 * mat2->f11	 +	 mat1->f22 * mat2->f21	 +	 mat1->f23 * mat2->f31;
	matresult->f22 = mat1->f20 * mat2->f02	 +	 mat1->f21 * mat2->f12	 +	 mat1->f22 * mat2->f22	 +	 mat1->f23 * mat2->f32;
	matresult->f23 = mat1->f20 * mat2->f03	 +	 mat1->f21 * mat2->f13	 +	 mat1->f22 * mat2->f23	 +	 mat1->f23 * mat2->f33;

	matresult->f30 = mat1->f30 * mat2->f00	 +	 mat1->f31 * mat2->f10	 +	 mat1->f32 * mat2->f20	 +	 mat1->f33 * mat2->f30;
	matresult->f31 = mat1->f30 * mat2->f01	 +	 mat1->f31 * mat2->f11	 +	 mat1->f32 * mat2->f21	 +	 mat1->f33 * mat2->f31;
	matresult->f32 = mat1->f30 * mat2->f02	 +	 mat1->f31 * mat2->f12	 +	 mat1->f32 * mat2->f22	 +	 mat1->f33 * mat2->f32;
	matresult->f33 = mat1->f30 * mat2->f03	 +	 mat1->f31 * mat2->f13	 +	 mat1->f32 * mat2->f23	 +	 mat1->f33 * mat2->f33;
}
// ------------------------------------------------------------------------------------------
// NMulMatrix3
// ------------------------------------------------------------------------------------------
// Description :
//	Multiply 2 matrix : matresult=mat1*mat2.
//	All Matrix are 3x3 matrix
// ------------------------------------------------------------------------------------------
// In  :
//		matresult	: Result Matrix
//		mat1		: The 1� matrix
//		mat2		: The 2� matrix
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NMulMatrix3(NMATRIX *matresult, const NMATRIX *mat1, const NMATRIX *mat2 )
{
	// Use only the "Mat3x3" part of the NMATRIX structure (which is a 4x4 matrix)
	// The unused Matrix 4x4  elements still inline but under comments to exactly understand what's happening...
	matresult->f00 = mat1->f00 * mat2->f00 + mat1->f01 * mat2->f10 + mat1->f02 * mat2->f20 ;//+ mat1->f03 * mat2->f30;
	matresult->f01 = mat1->f00 * mat2->f01 + mat1->f01 * mat2->f11 + mat1->f02 * mat2->f21 ;//+ mat1->f03 * mat2->f31;
	matresult->f02 = mat1->f00 * mat2->f02 + mat1->f01 * mat2->f12 + mat1->f02 * mat2->f22 ;//+ mat1->f03 * mat2->f32;
//	matresult->f03 = mat1->f00 * mat2->f03 + mat1->f01 * mat2->f13 + mat1->f02 * mat2->f23    + mat1->f03 * mat2->f33;

	matresult->f10 = mat1->f10 * mat2->f00 + mat1->f11 * mat2->f10 + mat1->f12 * mat2->f20 ;//+ mat1->f13 * mat2->f30;
	matresult->f11 = mat1->f10 * mat2->f01 + mat1->f11 * mat2->f11 + mat1->f12 * mat2->f21 ;//+ mat1->f13 * mat2->f31;
	matresult->f12 = mat1->f10 * mat2->f02 + mat1->f11 * mat2->f12 + mat1->f12 * mat2->f22 ;//+ mat1->f13 * mat2->f32;
//	matresult->f13 = mat1->f10 * mat2->f03 + mat1->f11 * mat2->f13 + mat1->f12 * mat2->f23    + mat1->f13 * mat2->f33;

	matresult->f20 = mat1->f20 * mat2->f00 + mat1->f21 * mat2->f10 + mat1->f22 * mat2->f20 ;//+ mat1->f23 * mat2->f30;
	matresult->f21 = mat1->f20 * mat2->f01 + mat1->f21 * mat2->f11 + mat1->f22 * mat2->f21 ;//+ mat1->f23 * mat2->f31;
	matresult->f22 = mat1->f20 * mat2->f02 + mat1->f21 * mat2->f12 + mat1->f22 * mat2->f22 ;//+ mat1->f23 * mat2->f32;
//	matresult->f23 = mat1->f20 * mat2->f03 + mat1->f21 * mat2->f13 + mat1->f22 * mat2->f23	  + mat1->f23 * mat2->f33;

//	matresult->f30 = mat1->f30 * mat2->f00 + mat1->f31 * mat2->f10 + mat1->f32 * mat2->f20 + mat1->f33 * mat2->f30;
//	matresult->f31 = mat1->f30 * mat2->f01 + mat1->f31 * mat2->f11 + mat1->f32 * mat2->f21 + mat1->f33 * mat2->f31;
//	matresult->f32 = mat1->f30 * mat2->f02 + mat1->f31 * mat2->f12 + mat1->f32 * mat2->f22 + mat1->f33 * mat2->f32;
//	matresult->f33 = mat1->f30 * mat2->f03 + mat1->f31 * mat2->f13 + mat1->f32 * mat2->f23 + mat1->f33 * mat2->f33;

// -------------------------------------------------------------------------------------------------------------------
// THE VERSION WITHOUT UNUSED 4x4 Matrix elements...
/*
	matresult->f00 =	mat1->f00 * mat2->f00 +	mat1->f01 * mat2->f10 +	mat1->f02 * mat2->f20;
	matresult->f01 =	mat1->f00 * mat2->f01 +	mat1->f01 * mat2->f11 +	mat1->f02 * mat2->f21;
	matresult->f02 =	mat1->f00 * mat2->f02 +	mat1->f01 * mat2->f12 +	mat1->f02 * mat2->f22;

	matresult->f10 =	mat1->f10 * mat2->f00 +	mat1->f11 * mat2->f10 +	mat1->f12 * mat2->f20;
	matresult->f11 =	mat1->f10 * mat2->f01 +	mat1->f11 * mat2->f11 +	mat1->f12 * mat2->f21;
	matresult->f12 =	mat1->f10 * mat2->f02 +	mat1->f11 * mat2->f12 +	mat1->f12 * mat2->f22;

	matresult->f20 =	mat1->f20 * mat2->f00 +	mat1->f21 * mat2->f10 +	mat1->f22 * mat2->f20;
	matresult->f21 =	mat1->f20 * mat2->f01 +	mat1->f21 * mat2->f11 +	mat1->f22 * mat2->f21;
	matresult->f22 =	mat1->f20 * mat2->f02 +	mat1->f21 * mat2->f12 +	mat1->f22 * mat2->f22;
*/
}


// ------------------------------------------------------------------------------------------
// NTranslationMatrixf
// ------------------------------------------------------------------------------------------
// Description :
//	Build a 4x4 translation Matrix
// ------------------------------------------------------------------------------------------
// In  :
//		tx,ty,tz	: Translations.
//		matresult	: The translation Matrix
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NTranslationMatrixf(NMATRIX *matresult,const Nf32 tx,const Nf32 ty,const Nf32 tz )
{
/*
	matresult->f00 = 1.0f;	matresult->f01 = 0.0f;	matresult->f02 = 0.0f;	matresult->f03 = 0.0f;
	matresult->f10 = 0.0f;	matresult->f11 = 1.0f;	matresult->f12 = 0.0f;	matresult->f13 = 0.0f;
	matresult->f20 = 0.0f;	matresult->f21 = 0.0f;	matresult->f22 = 1.0f;	matresult->f23 = 0.0f;
	matresult->f30 = tx;	matresult->f31 = ty;	matresult->f32 = tz;	matresult->f33 = 1.0f;
*/
	matresult->f00 = 1.0f;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;	matresult->f30 = tx;
	matresult->f01 = 0.0f;	matresult->f11 = 1.0f;	matresult->f21 = 0.0f;	matresult->f31 = ty;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = 1.0f;	matresult->f32 = tz;
	matresult->f03 = 0.0f;	matresult->f13 = 0.0f;	matresult->f23 = 0.0f;	matresult->f33 = 1.0f;

}

void NTranslationMatrix(NMATRIX *matresult, const NVEC3 *v )
{
/*
	matresult->f00 = 1.0f;	matresult->f01 = 0.0f;	matresult->f02 = 0.0f;	matresult->f03 = 0.0f;
	matresult->f10 = 0.0f;	matresult->f11 = 1.0f;	matresult->f12 = 0.0f;	matresult->f13 = 0.0f;
	matresult->f20 = 0.0f;	matresult->f21 = 0.0f;	matresult->f22 = 1.0f;	matresult->f23 = 0.0f;
	matresult->f30 = v->x;	matresult->f31 = v->y;	matresult->f32 = v->z;	matresult->f33 = 1.0f;
*/
	matresult->f00 = 1.0f;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;	matresult->f30 = v->x;
	matresult->f01 = 0.0f;	matresult->f11 = 1.0f;	matresult->f21 = 0.0f;	matresult->f31 = v->y;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = 1.0f;	matresult->f32 = v->z;
	matresult->f03 = 0.0f;	matresult->f13 = 0.0f;	matresult->f23 = 0.0f;	matresult->f33 = 1.0f;
}
// ------------------------------------------------------------------------------------------
// NScaleMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Build a scale matrix.
// ------------------------------------------------------------------------------------------
// In  :
//		sx,sy,sz	: scale values according the 3 axis X,Y and Z.
//		matresult	: The resulting scale matrix.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NScaleMatrixf(NMATRIX *matresult,const Nf32 sx,const Nf32 sy,const Nf32 sz )
{
/*
	matresult->f00 = sx;	matresult->f01 = 0.0f;	matresult->f02 = 0.0f;	matresult->f03 = 0.0f;
	matresult->f10 = 0.0f;	matresult->f11 = sy;	matresult->f12 = 0.0f;	matresult->f13 = 0.0f;
	matresult->f20 = 0.0f;	matresult->f21 = 0.0f;	matresult->f22 = sz;	matresult->f23 = 0.0f;
	matresult->f30 = 0.0f;	matresult->f31 = 0.0f;	matresult->f32 = 0.0f;	matresult->f33 = 1.0f;
*/
	matresult->f00 = sx;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;	matresult->f30 = 0.0f;
	matresult->f01 = 0.0f;	matresult->f11 = sy;	matresult->f21 = 0.0f;	matresult->f31 = 0.0f;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = sz;	matresult->f32 = 0.0f;
	matresult->f03 = 0.0f;	matresult->f13 = 0.0f;	matresult->f23 = 0.0f;	matresult->f33 = 1.0f;

}
void NScaleMatrix(NMATRIX *matresult, const NVEC3 *s)
{
/*
	matresult->f00 = s->x;	matresult->f01 = 0.0f;	matresult->f02 = 0.0f;	matresult->f03 = 0.0f;
	matresult->f10 = 0.0f;	matresult->f11 = s->y;	matresult->f12 = 0.0f;	matresult->f13 = 0.0f;
	matresult->f20 = 0.0f;	matresult->f21 = 0.0f;	matresult->f22 = s->z;	matresult->f23 = 0.0f;
	matresult->f30 = 0.0f;	matresult->f31 = 0.0f;	matresult->f32 = 0.0f;	matresult->f33 = 1.0f;
*/
	matresult->f00 = s->x;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;	matresult->f30 = 0.0f;
	matresult->f01 = 0.0f;	matresult->f11 = s->y;	matresult->f21 = 0.0f;	matresult->f31 = 0.0f;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = s->z;	matresult->f32 = 0.0f;
	matresult->f03 = 0.0f;	matresult->f13 = 0.0f;	matresult->f23 = 0.0f;	matresult->f33 = 1.0f;

}


// ------------------------------------------------------------------------------------------
// NTransposeMatrixO
// ------------------------------------------------------------------------------------------
// Description :
//	Perform a matrix 4x4 transposition.
//	Optimized version ... ( one NMATRIX copy less )
//	! pmatresult and pmat need to be different ! 
//	Use NTransposeMatrixO ONLY if you are sure that "pmatresult" is different than "pmat"
//	Use NTransposeMatrix for all other cases.
// ------------------------------------------------------------------------------------------
// In  :
//		pmat			: ptr on initial matrix.
//		pmatresult		: ptr on result matrix. (can not be the same as initial)
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NTransposeMatrixO( NMATRIX *pmatresult, const NMATRIX *pmat )
{
	NErrorIf((NMATRIX*)pmat == pmatresult, NERROR_MATRIX_IN_OUT_MATRIX_POINTERS_HAS_TO_BE_DIFFERENT);

	pmatresult->f00 = pmat->f00;	pmatresult->f10 = pmat->f01;	pmatresult->f20 = pmat->f02;	pmatresult->f30 = pmat->f03;
	pmatresult->f01 = pmat->f10;	pmatresult->f11 = pmat->f11;	pmatresult->f21 = pmat->f12;	pmatresult->f31 = pmat->f13;
	pmatresult->f02 = pmat->f20;	pmatresult->f12 = pmat->f21;	pmatresult->f22 = pmat->f22;	pmatresult->f32 = pmat->f23;
	pmatresult->f03 = pmat->f30;	pmatresult->f13 = pmat->f31;	pmatresult->f23 = pmat->f32;	pmatresult->f33 = pmat->f33;
}

// ------------------------------------------------------------------------------------------
// NTransposeMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Perform a matrix 4x4 transposition.
// ------------------------------------------------------------------------------------------
// In  :
//		pmat			: ptr on initial matrix.
//		pmatresult		: ptr on result matrix. (may be the same as initial)
// Out :
//
// ------------------------------------------------------------------------------------------
void NTransposeMatrix( NMATRIX *pmatresult, const NMATRIX *pmat )
{
	NMATRIX		tmp;

	tmp.f00 = pmat->f00;	tmp.f10 = pmat->f01;	tmp.f20 = pmat->f02;	tmp.f30 = pmat->f03;
	tmp.f01 = pmat->f10;	tmp.f11 = pmat->f11;	tmp.f21 = pmat->f12;	tmp.f31 = pmat->f13;
	tmp.f02 = pmat->f20;	tmp.f12 = pmat->f21;	tmp.f22 = pmat->f22;	tmp.f32 = pmat->f23;
	tmp.f03 = pmat->f30;	tmp.f13 = pmat->f31;	tmp.f23 = pmat->f32;	tmp.f33 = pmat->f33;

	// or maybe
/*
	tmp.f00 = pmat->f00;	tmp.f10 = pmat->f01;	tmp.f20 = pmat->f02;	tmp.f30 = -pmat->f30;
	tmp.f01 = pmat->f10;	tmp.f11 = pmat->f11;	tmp.f21 = pmat->f12;	tmp.f31 = -pmat->f31;
	tmp.f02 = pmat->f20;	tmp.f12 = pmat->f21;	tmp.f22 = pmat->f22;	tmp.f32 = -pmat->f32;
	tmp.f03 = 0.0f;			tmp.f13 = 0.0f;			tmp.f23 = 0.0f;			tmp.f33 =  pmat->f33;
*/
	*pmatresult = tmp;
}

// -------------------------------------------------------------------------------------------
// NMulVector4ByMatrixO
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector4 by a Matrix.
//	Optimized version ... ( one NVEC4 copy less )
//	! vr and v need to be different ! 
//	Use NMulVector4ByMatrixO ONLY if you are sure that "vr" is different than "v"
//	Use NMulVector4ByMatrix for all other cases.
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC4
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector4ByMatrixO(NVEC4 *vr, const NMATRIX *mat, const NVEC4 *v)
{
	NErrorIf( vr == (NVEC4*)v, NERROR_MATRIX_IN_OUT_VECTOR4_POINTERS_HAS_TO_BE_DIFFERENT);
/*
	// AGL version
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/	

	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + v->w * mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + v->w * mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + v->w * mat->f32;
	vr->w = v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + v->w * mat->f33;

/*
	vr->x = v->x * mat->f00 + v->y * mat->f01 + v->z * mat->f02 + v->w * mat->f03;
	vr->y = v->x * mat->f10 + v->y * mat->f11 + v->z * mat->f12 + v->w * mat->f13;
	vr->z = v->x * mat->f20 + v->y * mat->f21 + v->z * mat->f22 + v->w * mat->f23;
	vr->w = v->x * mat->f30 + v->y * mat->f31 + v->z * mat->f32 + v->w * mat->f33;
*/

}
// -------------------------------------------------------------------------------------------
// NMulVector4ByMatrix
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector4 by a Matrix.
//	vr and v may be the same one 
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC4
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector4ByMatrix(NVEC4 *vr, const NMATRIX *mat, const NVEC4 *v)
{
	NVEC4	vtmp;
/*
	// AGL version
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/	

	vtmp.x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + v->w * mat->f30;
	vtmp.y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + v->w * mat->f31;
	vtmp.z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + v->w * mat->f32;
	vtmp.w = v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + v->w * mat->f33;

	*vr = vtmp;
/*
	vr.x = v->x * mat->f00 + v->y * mat->f01 + v->z * mat->f02 + v->w * mat->f03;
	vr.y = v->x * mat->f10 + v->y * mat->f11 + v->z * mat->f12 + v->w * mat->f13;
	vr.z = v->x * mat->f20 + v->y * mat->f21 + v->z * mat->f22 + v->w * mat->f23;
	vr.w = v->x * mat->f30 + v->y * mat->f31 + v->z * mat->f32 + v->w * mat->f33;
	*v = vr;
*/

}

// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrixO
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix.
//	Optimized version ... ( one NVEC3 copy less )
//	! vr and v need to be different ! 
//	Use NMulVector3ByMatrixO ONLY if you are sure that "vr" is different than "v"
//	Use NMulVector3ByMatrix for all other cases.
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC3
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector3ByMatrixO(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v)
{
	NErrorIf( vr == (NVEC3*)v, NERROR_MATRIX_IN_OUT_VECTOR3_POINTERS_HAS_TO_BE_DIFFERENT);

/*
	// AGL version
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/	
	vr->x	= v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30/* *v->w (== 1)*/;
	vr->y	= v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31/* *v->w (== 1)*/;
	vr->z	= v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32/* *v->w (== 1)*/;
	Nf32 w	= v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + mat->f33/* *v->w (== 1)*/;


/*
	vr->x	= v->x * mat->f00 + v->y * mat->f01 + v->z * mat->f02 + mat->f03/ * *v->w (== 1)* /;
	vr->y	= v->x * mat->f10 + v->y * mat->f11 + v->z * mat->f12 + mat->f13/ * *v->w (== 1)* /;
	vr->z	= v->x * mat->f20 + v->y * mat->f21 + v->z * mat->f22 + mat->f23/ * *v->w (== 1)* /;
	Nf32 w	= v->x * mat->f30 + v->y * mat->f31 + v->z * mat->f32 + mat->f33/ * *v->w (== 1)* /;
*/

	NErrorIf(w == 0.0f,NERROR_MATRIX_NULL_W);
	if(w != 1.0f)
	{
		vr->x /= w;
		vr->y /= w;
		vr->z /= w;
	}
}

// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix.
//	vr and v maybe the same one
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC3
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector3ByMatrix(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v)
{
	NVEC3	vtmp;
/*
	// AGL version
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/	
	vtmp.x	= v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30/* *v->w (== 1)*/;
	vtmp.y	= v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31/* *v->w (== 1)*/;
	vtmp.z	= v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32/* *v->w (== 1)*/;
	Nf32 w	= v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + mat->f33/* *v->w (== 1)*/;

/*
	v4.x = v->x * mat->f00 + v->y * mat->f01 + v->z * mat->f02 + mat->f03/ * *v->w (== 1)* /;
	v4.y = v->x * mat->f10 + v->y * mat->f11 + v->z * mat->f12 + mat->f13/ * *v->w (== 1)* /;
	v4.z = v->x * mat->f20 + v->y * mat->f21 + v->z * mat->f22 + mat->f23/ * *v->w (== 1)* /;
	v4.w = v->x * mat->f30 + v->y * mat->f31 + v->z * mat->f32 + mat->f33/ * *v->w (== 1)* /;
*/
	NErrorIf(w == 0.0f,NERROR_MATRIX_NULL_W);
	if(w != 1.0f)
	{
		vr->x = vtmp.x/w;
		vr->y = vtmp.y/w;
		vr->z = vtmp.z/w;
	}
	else
	{
		*vr = vtmp;
	}
}
// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix3O
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix.
//	Optimized version ... ( one NVEC3 copy less )
//	! vr and v need to be different ! 
//	Use NMulVector3ByMatrixO ONLY if you are sure that "vr" is different than "v"
//	Use NMulVector3ByMatrix for all other cases.
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC3
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector3ByMatrix3O(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v )
{
	NErrorIf( vr == (NVEC3*)v, NERROR_MATRIX_IN_OUT_VECTOR3_POINTERS_HAS_TO_BE_DIFFERENT);

	// Use only the "Mat3x3" part of the NMATRIX structure (which is a 4x4 matrix)
	// The unused Matrix 4x4  elements still inline but under comments to exactly understand what's happening...
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;// + mat->f30/* *v->w (== 1)*/;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;// + mat->f31/* *v->w (== 1)*/;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;// + mat->f32/* *v->w (== 1)*/;
	// vtmp.w = v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + mat->f33/* *v->w (== 1)*/;


// -------------------------------------------------------------------------------------------------------------------
// THE VERSION WITHOUT UNUSED 4x4 Matrix elements...
/*
	vtmp.x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;
	vtmp.y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;
	vtmp.z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;
*/

// -------------------------------------------------------------------------------------------------------------------
// AGL version
/*
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/

}
// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix3
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix.
//	vr and v may be the same one
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC3
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector3ByMatrix3(NVEC3 *vr, const NMATRIX *mat, const NVEC3 *v )
{
	// Use only the "Mat3x3" part of the NMATRIX structure (which is a 4x4 matrix)
	// The unused Matrix 4x4  elements still inline but under comments to exactly understand what's happening...
	NVEC3 vtmp;
	vtmp.x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;// + mat->f30/* *v->w (== 1)*/;
	vtmp.y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;// + mat->f31/* *v->w (== 1)*/;
	vtmp.z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;// + mat->f32/* *v->w (== 1)*/;
	// vtmp.w = v->x * mat->f03 + v->y * mat->f13 + v->z * mat->f23 + mat->f33/* *v->w (== 1)*/;
	
	*vr = vtmp;
// -------------------------------------------------------------------------------------------------------------------
// THE VERSION WITHOUT UNUSED 4x4 Matrix elements...
/*
	vtmp.x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;
	vtmp.y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;
	vtmp.z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;
*/

// -------------------------------------------------------------------------------------------------------------------
// AGL version
/*
	vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
	vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
	vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
*/

}

// -------------------------------------------------------------------------------------------
// NMulVector2ByMatrixO
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector2 ( asuming z = 0.0f )  by a Matrix the result is a VECTOR 3 !!!.
//	
//	! vr and v need to be different ! ..; And they are because one is a NVEC2 and the othe a NVEC3 !!!!
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC2 ( with assumption : z = 0.0f )
//		mat	: Matrix.
//		vr	: NVEC3 result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector2ByMatrixO(NVEC3* vr, const NMATRIX* mat, const NVEC2* v)
{
	NErrorIf((NVEC2*)vr == (NVEC2*)v, NERROR_MATRIX_IN_OUT_VECTOR2_POINTERS_HAS_TO_BE_DIFFERENT);

	/*
		// AGL version
		vr->x = v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20 + mat->f30;
		vr->y = v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21 + mat->f31;
		vr->z = v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22 + mat->f32;
	*/

	// Because we assume v->z = 0.0f ( because v is a NVEC2 and doesn't have a third coordinate )
	// we can simplify some operation...
	vr->x  = v->x * mat->f00 + v->y * mat->f10  /*+ v->z (== 0 )* mat->f20*/ + mat->f30/* *v->w (== 1)*/;
	vr->y  = v->x * mat->f01 + v->y * mat->f11  /*+ v->z (== 0 )* mat->f21*/ + mat->f31/* *v->w (== 1)*/;
	vr->z  = v->x * mat->f02 + v->y * mat->f12  /*+ v->z (== 0 )* mat->f22*/ + mat->f32/* *v->w (== 1)*/;
	Nf32 w = v->x * mat->f03 + v->y * mat->f13  /*+ v->z (== 0 )* mat->f23*/ + mat->f33/* *v->w (== 1)*/;


	/*
		vr->x	= v->x * mat->f00 + v->y * mat->f01 + v->z * mat->f02 + mat->f03/ * *v->w (== 1)* /;
		vr->y	= v->x * mat->f10 + v->y * mat->f11 + v->z * mat->f12 + mat->f13/ * *v->w (== 1)* /;
		vr->z	= v->x * mat->f20 + v->y * mat->f21 + v->z * mat->f22 + mat->f23/ * *v->w (== 1)* /;
		Nf32 w	= v->x * mat->f30 + v->y * mat->f31 + v->z * mat->f32 + mat->f33/ * *v->w (== 1)* /;
	*/

	NErrorIf(w == 0.0f, NERROR_MATRIX_NULL_W);
	if (w != 1.0f)
	{
		vr->x /= w;
		vr->y /= w;
		vr->z /= w;
	}
}
// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix3O
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector2 by a Matrix.
// Description :
//	Multiply a vector2 ( asuming z = 0.0f )  by a Matrix the result is a VECTOR 3 !!!.
//	
//	! vr and v need to be different ! ..; And they are because one is a NVEC2 and the other a NVEC3 !!!!
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC2
//		mat	: Matrix.
//		vr	: NVEC3 result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector2ByMatrix3O(NVEC3* vr, const NMATRIX* mat, const NVEC2* v)
{
	NErrorIf((NVEC2*)vr == (NVEC2*)v, NERROR_MATRIX_IN_OUT_VECTOR2_POINTERS_HAS_TO_BE_DIFFERENT);

	// Use only the "Mat3x3" part of the NMATRIX structure (which is a 4x4 matrix)
		// Because we assume v->z = 0.0f ( because v is a NVEC2 and doesn't have a third coordinate )
	// we can simplify some operation...

	// The unused Matrix 4x4  elements still inline but under comments to exactly understand what's happening...
	vr->x = v->x * mat->f00 + v->y * mat->f10  /*+ v->z (== 0 )* mat->f20 + mat->f30 *v->w (== 1)*/;
	vr->y = v->x * mat->f01 + v->y * mat->f11  /*+ v->z (== 0 )* mat->f21 + mat->f31 *v->w (== 1)*/;
	vr->z = v->x * mat->f02 + v->y * mat->f12  /*+ v->z (== 0 )* mat->f22 + mat->f32 *v->w (== 1)*/;
	// vtmp.w = v->x * mat->f03 + v->y * mat->f13 /*+ v->z (== 0 )* mat->f23 + v->z * mat->f23 + mat->f33/* *v->w (== 1)*/;
}

// ------------------------------------------------------------------------------------------
// NRotationMatrix_AxisX
// ------------------------------------------------------------------------------------------
// Description :
//	Build a rotation matrix "around" X axis.
// ------------------------------------------------------------------------------------------
// In  :
//		angx_rad	: Rotation Angle (radian) around X axis.
//		pmat		: Destination matrix. It will be initialized with the incoming rotation
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NRotationMatrix_AxisX(NMATRIX *pmat, Nf32 angx_rad )
{
	Nf32	co,si;
	NCosSin(angx_rad, &co, &si);
	pmat->f00=1.0f;		pmat->f10=0.0f;		pmat->f20=0.0f;		pmat->f30=0.0f;
	pmat->f01=0.0f;		pmat->f11=co;		pmat->f21=-si; 		pmat->f31=0.0f;
	pmat->f02=0.0f;		pmat->f12=si;		pmat->f22=co;  		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix_AxisX(NMATRIX *pmat, Nu32 nanglex )
{
	Nf32	co,si;
	NFastCosSin(nanglex, &co, &si);
	pmat->f00=1.0f;		pmat->f10=0.0f;		pmat->f20=0.0f;		pmat->f30=0.0f;
	pmat->f01=0.0f;		pmat->f11=co;		pmat->f21=-si; 		pmat->f31=0.0f;
	pmat->f02=0.0f;		pmat->f12=si;		pmat->f22=co;  		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
*/
// ------------------------------------------------------------------------------------------
// NRotationMatrix_AxisY
// ------------------------------------------------------------------------------------------
// Description :
//	Build a rotation matrix "around" Y axis.
// ------------------------------------------------------------------------------------------
// In  :
//		angy_rad	: Rotation Angle (radian) around Y axis.
//		pmat		: Destination matrix. It will be initialized with the incoming rotation
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NRotationMatrix_AxisY(NMATRIX *pmat, Nf32 angy_rad )
{
	Nf32		co,si;
	NCosSin(angy_rad,&co,&si);

	pmat->f00=co;		pmat->f10=0.0f;		pmat->f20=si;		pmat->f30=0.0f;
	pmat->f01=0.0f;		pmat->f11=1.0f;		pmat->f21=0.0f;		pmat->f31=0.0f;
	pmat->f02=-si;		pmat->f12=0.0f;		pmat->f22=co;		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix_AxisY(NMATRIX *pmat, Nu32 nangley )
{
	Nf32		co,si;
	NFastCosSin(nangley,&co,&si);

	pmat->f00=co;		pmat->f10=0.0f;		pmat->f20=si;		pmat->f30=0.0f;
	pmat->f01=0.0f;		pmat->f11=1.0f;		pmat->f21=0.0f;		pmat->f31=0.0f;
	pmat->f02=-si;		pmat->f12=0.0f;		pmat->f22=co;		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
*/
// ------------------------------------------------------------------------------------------
// NRotationMatrix_AxisZ
// ------------------------------------------------------------------------------------------
// Description :
//	Build a rotation matrix "around" Z axis.
// ------------------------------------------------------------------------------------------
// In  :
//		angz_rad	: Rotation Angle (radian) around Z axis.
//		pmat		: Destination matrix. It will be initialized with the incoming rotation
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NRotationMatrix_AxisZ(NMATRIX *pmat, Nf32 angz_rad )
{
	Nf32		co,si;
	NCosSin(angz_rad,&co,&si);

	pmat->f00=co;		pmat->f10=-si;		pmat->f20=0.0f;		pmat->f30=0.0f;
	pmat->f01=si;		pmat->f11=co;		pmat->f21=0.0f;		pmat->f31=0.0f;
	pmat->f02=0.0f;		pmat->f12=0.0f;		pmat->f22=1.0;		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix_AxisZ(NMATRIX *pmat, Nu32 nanglez )
{
	Nf32		co,si;
	NFastCosSin(nanglez,&co,&si);

	pmat->f00=co;		pmat->f10=-si;		pmat->f20=0.0f;		pmat->f30=0.0f;
	pmat->f01=si;		pmat->f11=co;		pmat->f21=0.0f;		pmat->f31=0.0f;
	pmat->f02=0.0f;		pmat->f12=0.0f;		pmat->f22=1.0;		pmat->f32=0.0f;
	pmat->f03=0.0f;		pmat->f13=0.0f;		pmat->f23=0.0f;		pmat->f33=1.0f;
}
*/
/*
// --------------------------------------------------------------------------- ---------------
// NForwardVectorMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a Matrix(4x4) from a Forward unit Vector (NVEC3).
// ------------------------------------------------------------------------------------------
// In  :
//		NMATRIX			*pmat				"THE" Matrix to setup
//		const NVEC3	*punit_forward		The unit vector used as forward vector to build the matrix with.
// Out :
//
// ------------------------------------------------------------------------------------------
void NForwardVectorMatrix(NMATRIX *pmat, const NVEC3 *punit_forward)
{
	NErrorIf(NABS(1.0f-NVec3Length(punit_forward))>NF32_EPSILON_VECTOR_LENGTH,NERROR_NON_UNIT_VECTOR);	 // Crash in Debug Mode	Only

	// Identity Matrix ( don't touch the 3x3 matrix which will be setup just right now ...)
	/ *pmat->f00 = 1.0f; 	pmat->f10 = 0.0f;	pmat->f20 = 0.0f;* /	 pmat->f30 = 0.0f; 						
	/ *pmat->f01 = 0.0f;		pmat->f11 = 1.0f;	pmat->f21 = 0.0f;* /	 pmat->f31 = 0.0f; 
	/ *pmat->f02 = 0.0f;		pmat->f12 = 0.0f;	pmat->f22 = 1.0f;* /	 pmat->f32 = 0.0f;
	pmat->f03 = 0.0f;		pmat->f13 = 0.0f;	pmat->f23 = 0.0f;	 pmat->f33 = 1.0f;

	NVEC3	up;
	NUT_HughesMollerChooseUp(&up,punit_forward);										// Choose/Use a temporary up vector to build side AXIS.
	NVec3CrossProduct((NVEC3*)&pmat->Side,punit_forward,&up);						// Build side AXIS.
	NVec3Normalize((NVEC3*)&pmat->Side);											// Normalize it.
	NVec3CrossProduct((NVEC3*)&pmat->Up,(NVEC3*)&pmat->Side,punit_forward);	// Build Up Axis we don't normalize it, not necessary 
	*(NVEC3*)&pmat->Forward = *punit_forward;										// (... forward and Side are normalized ones ... )
}

// --------------------------------------------------------------------------- ---------------
// NForwardVectorMatrixEx
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a Matrix(4x4) from 2 points which define a Forward Vector and a reference up vector.
//	Notice that vectors "from" and "up" are both optional.
//	If both are NULL, this function works exactly like 'NForwardVectorMatrix' but with some 
//	extra tests (due to these NULL vectors), so in that case prefer using 'NForwardVectorMatrix'.
// ------------------------------------------------------------------------------------------
// In  :
//		NMATRIX			*pmat				"THE" Matrix to setup
//		const NVEC3	*pto				"head point" of the forward vector ( targeted or look at point).
//		const NVEC3	*pfrom				"origin" of the forward vector.
//		const NVEC3	*pup				reference up to build the 2 other vectors. It could be NULL, in that case
//											function is going to choose an up vector with HughesMoeller algorithm.
// Out :
//
// ------------------------------------------------------------------------------------------
void NForwardVectorMatrixEx(NMATRIX *pmat, const NVEC3 *pto, const NVEC3 *pfrom, const NVEC3 *pup)
{
	NErrorIf(!pto||!pup,NERROR_NULL_VECTOR);

	Nf32		length;

	// forward = lookat - origin
	if(pfrom)
		NVec3Sub((NVEC3*)&pmat->Forward,pto,pfrom);
	else
		*(NVEC3*)&pmat->Forward=*pto;
	length = NVec3Length((NVEC3*)&pmat->Forward);

	if(length<NF32_EPSILON_VECTOR_LENGTH)		// "pfrom" and "pto" are close together !
	{
		NErrorIf(1,NERROR_NULL_VECTOR);			// Crash in Debug Mode	Only
	}
	NVec3ScaleBy((NVEC3*)&pmat->Forward,1.0f/length);		// normalize forward AXIS

	if(pup)
	{
		NVec3CrossProduct((NVEC3*)&pmat->Side,(NVEC3*)&pmat->Forward,pup);	// use pup vector to build side AXIS
		length = NVec3Length((NVEC3*)&pmat->Side);
		if(length < NF32_EPSILON_VECTOR_LENGTH)
		{
			NErrorIf(1,NERROR_COLLINEAR_VECTORS);
		}
		// too small length means pup and forward are almost collinear.
		// so its better to stop function here and return NFALSE. User should choose an another up vector !
		// notes:
		// forward and up have both a length of 1. so side = 1 x 1 x sin(up,forward)
		// so if length = 0.001f that means sin(up,forward)= 0.01f !
		// ... and that means the angle between up and forward = 0.057 deg 
		// ... close to be collinear isn't it ? So imagine for a length value < NF32_EPSILON_VECTOR_LENGTH !
		NVec3ScaleBy((NVEC3*)&pmat->Side,1.0f/length); // normalize 
	}
	else
	{
		NVEC3	up;
		NUT_HughesMollerChooseUp(&up,(NVEC3*)&pmat->Forward);					// Choose/Use a temporary up vector to build side AXIS.
		NVec3CrossProduct((NVEC3*)&pmat->Side,(NVEC3*)&pmat->Forward,&up);	// Build side AXIS.
		NVec3Normalize((NVEC3*)&pmat->Side);									// Normalize it.
	}

	// Compute Base Up = side x forward
	NVec3CrossProduct((NVEC3*)&pmat->Up,(NVEC3*)&pmat->Side,(NVEC3*)&pmat->Forward); // we don't normalize this one, not necessary

	// Identity Matrix ( don't touch the 3x3 matrix which was be setup just before ...)
	/ *pmat->f00 = 1.0f; 	pmat->f10 = 0.0f;	pmat->f20 = 0.0f;* /	 pmat->f30 = 0.0f; 						
	/ *pmat->f01 = 0.0f;		pmat->f11 = 1.0f;	pmat->f21 = 0.0f;* /	 pmat->f31 = 0.0f; 
	/ *pmat->f02 = 0.0f;		pmat->f12 = 0.0f;	pmat->f22 = 1.0f;* /	 pmat->f32 = 0.0f;
	pmat->f03 = 0.0f;		pmat->f13 = 0.0f;	pmat->f23 = 0.0f;	 pmat->f33 = 1.0f;
}
*/

/*
// ------------------------------------------------------------------------------------------
// AGLMulMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Multiplie 2 matrices : matd=mat1*mat2.
//	Les matrices sont des matrices 4x4.
// ------------------------------------------------------------------------------------------
// In  :
//		mat1	: La 1� matrice.
//		mat2	: La 2� matrice.
//		matd	: La matrice de destination.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLMulMatrix(PAGLMATRIX mat1, PAGLMATRIX mat2, PAGLMATRIX matd)
{
	matd->f00 = mat1->f00 * mat2->f00 + mat1->f01 * mat2->f10 +	mat1->f02 * mat2->f20 +	mat1->f03 * mat2->f30;
	matd->f01 = mat1->f00 * mat2->f01 +	mat1->f01 * mat2->f11 +	mat1->f02 * mat2->f21 +	mat1->f03 * mat2->f31;
	matd->f02 = mat1->f00 * mat2->f02 +	mat1->f01 * mat2->f12 +	mat1->f02 * mat2->f22 +	mat1->f03 * mat2->f32;
	matd->f03 = mat1->f00 * mat2->f03 +	mat1->f01 * mat2->f13 +	mat1->f02 * mat2->f23 +	mat1->f03 * mat2->f33;
	
	matd->f10 = mat1->f10 * mat2->f00 +	mat1->f11 * mat2->f10 +	mat1->f12 * mat2->f20 +	mat1->f13 * mat2->f30;
	matd->f11 = mat1->f10 * mat2->f01 +	mat1->f11 * mat2->f11 +	mat1->f12 * mat2->f21 +	mat1->f13 * mat2->f31;
	matd->f12 = mat1->f10 * mat2->f02 +	mat1->f11 * mat2->f12 +	mat1->f12 * mat2->f22 +	mat1->f13 * mat2->f32;
	matd->f13 = mat1->f10 * mat2->f03 +	mat1->f11 * mat2->f13 +	mat1->f12 * mat2->f23 +	mat1->f13 * mat2->f33;
	
	matd->f20 = mat1->f20 * mat2->f00 +	mat1->f21 * mat2->f10 +	mat1->f22 * mat2->f20 +	mat1->f23 * mat2->f30;
	matd->f21 = mat1->f20 * mat2->f01 +	mat1->f21 * mat2->f11 +	mat1->f22 * mat2->f21 +	mat1->f23 * mat2->f31;
	matd->f22 = mat1->f20 * mat2->f02 +	mat1->f21 * mat2->f12 +	mat1->f22 * mat2->f22 +	mat1->f23 * mat2->f32;
	matd->f23 = mat1->f20 * mat2->f03 +	mat1->f21 * mat2->f13 +	mat1->f22 * mat2->f23 +	mat1->f23 * mat2->f33;
	
	matd->f30 = mat1->f30 * mat2->f00 +	mat1->f31 * mat2->f10 +	mat1->f32 * mat2->f20 +	mat1->f33 * mat2->f30;
	matd->f31 = mat1->f30 * mat2->f01 +	mat1->f31 * mat2->f11 +	mat1->f32 * mat2->f21 +	mat1->f33 * mat2->f31;
	matd->f32 = mat1->f30 * mat2->f02 +	mat1->f31 * mat2->f12 +	mat1->f32 * mat2->f22 +	mat1->f33 * mat2->f32;
	matd->f33 = mat1->f30 * mat2->f03 +	mat1->f31 * mat2->f13 +	mat1->f32 * mat2->f23 +	mat1->f33 * mat2->f33;
}
*/

/*
// ------------------------------------------------------------------------------------------
// AGLMulMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
//	Multiplie 2 matrices : matd=mat1*mat2.
//	Les matrices sont des matrices 3x3.
// ------------------------------------------------------------------------------------------
// In  :
//		mat1	: La 1� matrice.
//		mat2	: La 2� matrice.
//		matd	: La matrice de destination.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLMulMatrix3x3(PAGLMATRIX mat1, PAGLMATRIX mat2, PAGLMATRIX matd)
{
	matd->f00 = mat1->f00 * mat2->f00 +
		mat1->f01 * mat2->f10 +
		mat1->f02 * mat2->f20;
	matd->f01 = mat1->f00 * mat2->f01 +
		mat1->f01 * mat2->f11 +
		mat1->f02 * mat2->f21;
	matd->f02 = mat1->f00 * mat2->f02 +
		mat1->f01 * mat2->f12 +
		mat1->f02 * mat2->f22;

	matd->f10 = mat1->f10 * mat2->f00 +
		mat1->f11 * mat2->f10 +
		mat1->f12 * mat2->f20;
	matd->f11 = mat1->f10 * mat2->f01 +
		mat1->f11 * mat2->f11 +
		mat1->f12 * mat2->f21;
	matd->f12 = mat1->f10 * mat2->f02 +
		mat1->f11 * mat2->f12 +
		mat1->f12 * mat2->f22;

	matd->f20 = mat1->f20 * mat2->f00 +
		mat1->f21 * mat2->f10 +
		mat1->f22 * mat2->f20;
	matd->f21 = mat1->f20 * mat2->f01 +
		mat1->f21 * mat2->f11 +
		mat1->f22 * mat2->f21;
	matd->f22 = mat1->f20 * mat2->f02 +
		mat1->f21 * mat2->f12 +
		mat1->f22 * mat2->f22;
}
*/

/*
// ------------------------------------------------------------------------------------------
// AGLRotationAxisMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Calcule la matrice de rotation autour d'un axe.
// ------------------------------------------------------------------------------------------
// In  :
//		v			: Vecteur d�crivant l'axe.
//		rot			: Angle de rotation.
//		mat			: La matrice.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLRotationAxisMatrix(PAGLVECTOR v, float rot, PAGLMATRIX mat)
{
	float		c,s,t;
	float		x,y,z;
	float		x2,y2,z2;

	x = v->x;
	y = v->y;
	z = v->z;
	// ----------------------
	// Normalise le vecteur
	// ----------------------
	x2 = x * x;
	y2 = y * y;
	z2 = z * z;

	c = 1.0f / AGLSqrt(x2 + y2 + z2);
	x *= c;
	y *= c;
	z *= c;

	AGLCosSin(rot, &c, &s);
	t = 1.0f - c;

	mat->f00=(float)((t*x2)+c);			mat->f01=(float)((t*x*y)+(s*z));	mat->f02=(float)((t*x*z)-(s*y));	mat->f03=0.0f;
	mat->f10=(float)((t*x*y)-(s*z));	mat->f11=(float)((t*y2)+c);			mat->f12=(float)((t*y*z)+(s*x));	mat->f13=0.0f;
	mat->f20=(float)((t*x*z)+(s*y));	mat->f21=(float)((t*y*z)-(s*x));	mat->f22=(float)((t*z2)+c);			mat->f23=0.0f;
	mat->f30=0.0f;						mat->f31=0.0f;						mat->f32=0.0f;						mat->f33=1.0f;
}

// ------------------------------------------------------------------------------------------
// AGLRotationXMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Cr�e une matrice de rotations selon l'axe des X.
// ------------------------------------------------------------------------------------------
// In  :
//		rx			: Angle (radian) de rotation selon l'axe des X.
//		matd		: La matrice de destination. Elle est compl�tement initialis�e !!!
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLRotationXMatrix(float rx, PAGLMATRIX matd)
{
	float		co,si;

	CosSin(rx, &co, &si);

	matd->f00=1.0f;	matd->f01=0.0f;	matd->f02=0.0f;	matd->f03=0.0f;
	matd->f10=0.0f;	matd->f11=co;	matd->f12=si;	matd->f13=0.0f;
	matd->f20=0.0f;	matd->f21=-si;	matd->f22=co;	matd->f23=0.0f;
	matd->f30=0.0f;	matd->f31=0.0f;	matd->f32=0.0f;	matd->f33=1.0f;
}

// ------------------------------------------------------------------------------------------
// AGLRotationYMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Cr�e une matrice de rotations selon l'axe des Y.
// ------------------------------------------------------------------------------------------
// In  :
//		ry			: Angle (radian) de rotation selon l'axe des Y.
//		matd		: La matrice de destination. Elle est compl�tement initialis�e !!!
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLRotationYMatrix(float ry, PAGLMATRIX matd)
{
	float		co,si;

	CosSin(ry,&co,&si);

	matd->f00=co;	matd->f01=0.0f;	matd->f02=-si;	matd->f03=0.0f;
	matd->f10=0.0f;	matd->f11=1.0f;	matd->f12=0.0f;	matd->f13=0.0f;
	matd->f20=si;	matd->f21=0.0f;	matd->f22=co;	matd->f23=0.0f;
	matd->f30=0.0f;	matd->f31=0.0f;	matd->f32=0.0f;	matd->f33=1.0f;
}

// ------------------------------------------------------------------------------------------
// AGLRotationZMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Cr�e une matrice de rotations selon l'axe des Z.
// ------------------------------------------------------------------------------------------
// In  :
//		rz			: Angle (radian) de rotation selon l'axe des Z.
//		matd		: La matrice de destination. Elle est compl�tement initialis�e !!!
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLRotationZMatrix(float rz, PAGLMATRIX matd)
{
	float	co,si;

	CosSin(rz,&co,&si);

	matd->f00=co;	matd->f01=si;	matd->f02=0.0f;	matd->f03=0.0f;
	matd->f10=-si;	matd->f11=co;	matd->f12=0.0f;	matd->f13=0.0f;
	matd->f20=0.0f;	matd->f21=0.0f;	matd->f22=1.0;	matd->f23=0.0f;
	matd->f30=0.0f;	matd->f31=0.0f;	matd->f32=0.0f;	matd->f33=1.0f;
}

// ------------------------------------------------------------------------------------------
// AGLRotationMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Cr�e une matrice de rotations selon 3 angles.
// ------------------------------------------------------------------------------------------
// In  :
//		rx,ry,rz	: Angles de rotation. (en radian).
//		matd		: La matrice de destination. Elle est compl�tement initialis�e !!!
//
// Out :
//
// ------------------------------------------------------------------------------------------
void AGLRotationMatrix(float rx, float ry, float rz, PAGLMATRIX matd)
{
	AGLMATRIX		ma,mb,mc;
	float			co,si;

	// ----------------
	// Rotation selon X
	// ----------------
	CosSin(rx, &co, &si);

	mb.f00 = 1.0f;	mb.f01 = 0.0f;	mb.f02 = 0.0f;
	mb.f10 = 0.0f;	mb.f11 = co;	mb.f12 = si;
	mb.f20 = 0.0f;	mb.f21 = -si;	mb.f22 = co;

	// ----------------
	// Rotation selon Y
	// ----------------
	CosSin(ry, &co, &si);

	ma.f00 = co;	ma.f01 = 0.0f;	ma.f02 = -si;
	ma.f10 = 0.0f;	ma.f11 = 1.0f;	ma.f12 = 0.0f;
	ma.f20 = si;	ma.f21 = 0.0f;	ma.f22 = co;

	AGLMulMatrix3x3(&mb, &ma, &mc);

	// ----------------
	// Rotation selon Z
	// ----------------
	CosSin(rz, &co, &si);

	ma.f00 = co;	ma.f01 = si;	ma.f02 = 0.0f;
	ma.f10 = -si;	ma.f11 = co;	ma.f12 = 0.0f;
	ma.f20 = 0.0f;	ma.f21 = 0.0f;	ma.f22 = 1.0;

	memset(matd, 0, sizeof(AGLMATRIX));
	AGLMulMatrix3x3(&mc, &ma, matd);
	matd->f33 = 1.0f;
}


// ------------------------------------------------------------------------------------------
// Det2x2
// ------------------------------------------------------------------------------------------
// Description :
//	Retrouve le d�terminant d'une matrice 2x2.
// ------------------------------------------------------------------------------------------
// In  :
//		matrice.
//
// Out :
//		D�terminant.
//
// ------------------------------------------------------------------------------------------
static float Det2x2(float a,float b,float c,float d)
{
	float	ans;

	ans = a * d - b * c;

	return ans;
}

// ------------------------------------------------------------------------------------------
// Det3x3
// ------------------------------------------------------------------------------------------
// Description :
//	Retrouve le d�terminant d'une matrice 3x3 de la forme :
//
//		| a1, b1, c1 |
//		| a2, b2, c2 |
//		| a3, b3, c3 |
//
// ------------------------------------------------------------------------------------------
// In  :
//		matrice.
//
// Out :
//		D�terminant.
//
// ------------------------------------------------------------------------------------------
static float Det3x3(float a1,float a2,float a3,float b1,float b2,float b3,float c1,float c2,float c3)
{
	float	ans;

	ans = a1 * Det2x2(b2,b3,c2,c3) -
		b1 * Det2x2(a2,a3,c2,c3) +
		c1 * Det2x2(a2,a3,b2,b3);

	return ans;
}

// ------------------------------------------------------------------------------------------
// Det4x4
// ------------------------------------------------------------------------------------------
// Description :
//	Retrouve le d�terminant d'une matrice 4x4.
// ------------------------------------------------------------------------------------------
// In  :
//		mat	: Matrice.
//
// Out :
//		D�terminant.
//
// ------------------------------------------------------------------------------------------
static float Det4x4(PAGLMATRIX mat)
{
	float	ans;
	float	a1,a2,a3,a4;
	float	b1,b2,b3,b4;
	float	c1,c2,c3,c4;
	float	d1,d2,d3,d4;

	a1 = mat->f00; b1 = mat->f01;
	c1 = mat->f02; d1 = mat->f03;

	a2 = mat->f10; b2 = mat->f11;
	c2 = mat->f12; d2 = mat->f13;

	a3 = mat->f20; b3 = mat->f21;
	c3 = mat->f22; d3 = mat->f23;

	a4 = mat->f30; b4 = mat->f31;
	c4 = mat->f32; d4 = mat->f33;

	ans = a1 * Det3x3(b2,b3,b4,c2,c3,c4,d2,d3,d4) -
		b1 * Det3x3(a2,a3,a4,c2,c3,c4,d2,d3,d4) +
		c1 * Det3x3(a2,a3,a4,b2,b3,b4,d2,d3,d4) -
		d1 * Det3x3(a2,a3,a4,b2,b3,b4,c2,c3,c4);

	return ans;
}

// ------------------------------------------------------------------------------------------
// AdjointMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Retrouve la matrice adjointe d'une matrice.
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: La matrice source � inverser.
//		adj		: La matrice de destination.
//
// Out :
//
// ------------------------------------------------------------------------------------------
static void AdjointMatrix(PAGLMATRIX mat, PAGLMATRIX adj)
{
	float	a1,a2,a3,a4;
	float	b1,b2,b3,b4;
	float	c1,c2,c3,c4;
	float	d1,d2,d3,d4;

	a1=mat->f00; b1=mat->f01;
	c1=mat->f02; d1=mat->f03;

	a2=mat->f10; b2=mat->f11;
	c2=mat->f12; d2=mat->f13;

	a3=mat->f20; b3=mat->f21;
	c3=mat->f22; d3=mat->f23;

	a4=mat->f30; b4=mat->f31;
	c4=mat->f32; d4=mat->f33;

	adj->f00 =  Det3x3(b2,b3,b4,c2,c3,c4,d2,d3,d4);
	adj->f10 = -Det3x3(a2,a3,a4,c2,c3,c4,d2,d3,d4);
	adj->f20 =  Det3x3(a2,a3,a4,b2,b3,b4,d2,d3,d4);
	adj->f30 = -Det3x3(a2,a3,a4,b2,b3,b4,c2,c3,c4);

	adj->f01 = -Det3x3(b1,b3,b4,c1,c3,c4,d1,d3,d4);
	adj->f11 =  Det3x3(a1,a3,a4,c1,c3,c4,d1,d3,d4);
	adj->f21 = -Det3x3(a1,a3,a4,b1,b3,b4,d1,d3,d4);
	adj->f31 =  Det3x3(a1,a3,a4,b1,b3,b4,c1,c3,c4);

	adj->f02 =  Det3x3(b1,b2,b4,c1,c2,c4,d1,d2,d4);
	adj->f12 = -Det3x3(a1,a2,a4,c1,c2,c4,d1,d2,d4);
	adj->f22 =  Det3x3(a1,a2,a4,b1,b2,b4,d1,d2,d4);
	adj->f32 = -Det3x3(a1,a2,a4,b1,b2,b4,c1,c2,c4);

	adj->f03 = -Det3x3(b1,b2,b3,c1,c2,c3,d1,d2,d3);
	adj->f13 =  Det3x3(a1,a2,a3,c1,c2,c3,d1,d2,d3);
	adj->f23 = -Det3x3(a1,a2,a3,b1,b2,b3,d1,d2,d3);
	adj->f33 =  Det3x3(a1,a2,a3,b1,b2,b3,c1,c2,c3);
}

// ------------------------------------------------------------------------------------------
// AGLInverseMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Calcule l'inverse d'une matrice.
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: La matrice source � inverser.
//		inv		: La matrice de destination.
//
// Out :
//		Code d'erreur.
//
// ------------------------------------------------------------------------------------------
long AGLInverseMatrix(PAGLMATRIX mat, PAGLMATRIX inv)
{
	DWORD	i,j;
	float	det;

	// ------------------
	// Matrice adjointe
	// ------------------
	AdjointMatrix(mat, inv);

	// -----------------
	// D�terminant 4x4
	// -----------------
	det = Det4x4(mat);
	if( det==0.0f )
		return AGLERROR(AGL_CANTDOTHAT);

	// --------------------------------------------------
	// Multiplie la matrice adjointe par le d�terminant
	// pour avoir la matrice inverse
	// --------------------------------------------------
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			inv->f[j][i] = inv->f[j][i] / det;
		}
	}

	return AGLERROR(AGL_OK);
}
*/