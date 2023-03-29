// ==========================================================================================
// NMatrix.cpp
// ------------------------------------------------------------------------------------------
// Author	: Jean-Marie Nazaret
// Create	: 06/05/2016
// Modified : 
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
//#include "../Utilities/Maths/NUT_MathsMisc.h"

#include "lib/N/Maths/NMatrix3x3.h"

// --------------------------------------------------------------------------- ---------------
// NIdentityMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
//	reset a Matrix3x3 to the Identity form.
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: "THE" 3x3 Matrix
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NIdentityMatrix3x3(NMATRIX3x3 *mat)
{
	//	XAxis				YAxis				ZAxis
		mat->f00 = 1.0f; 	mat->f10 = 0.0f;	mat->f20 = 0.0f;				
		mat->f01 = 0.0f;	mat->f11 = 1.0f;	mat->f21 = 0.0f;	
		mat->f02 = 0.0f;	mat->f12 = 0.0f;	mat->f22 = 1.0f;	
}

// ------------------------------------------------------------------------------------------
// NMulMatrix3x3
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
void NMulMatrix3x3(NMATRIX3x3 *matresult, const NMATRIX3x3 *mat1, const NMATRIX3x3 *mat2 )
{
	matresult->f00 = mat1->f00 * mat2->f00 + mat1->f01 * mat2->f10 + mat1->f02 * mat2->f20;
	matresult->f01 = mat1->f00 * mat2->f01 + mat1->f01 * mat2->f11 + mat1->f02 * mat2->f21;
	matresult->f02 = mat1->f00 * mat2->f02 + mat1->f01 * mat2->f12 + mat1->f02 * mat2->f22;

	matresult->f10 = mat1->f10 * mat2->f00 + mat1->f11 * mat2->f10 + mat1->f12 * mat2->f20;
	matresult->f11 = mat1->f10 * mat2->f01 + mat1->f11 * mat2->f11 + mat1->f12 * mat2->f21;
	matresult->f12 = mat1->f10 * mat2->f02 + mat1->f11 * mat2->f12 + mat1->f12 * mat2->f22;

	matresult->f20 = mat1->f20 * mat2->f00 + mat1->f21 * mat2->f10 + mat1->f22 * mat2->f20;
	matresult->f21 = mat1->f20 * mat2->f01 + mat1->f21 * mat2->f11 + mat1->f22 * mat2->f21;
	matresult->f22 = mat1->f20 * mat2->f02 + mat1->f21 * mat2->f12 + mat1->f22 * mat2->f22;
}


// ------------------------------------------------------------------------------------------
// NScaleMatrix3x3f
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
void NScaleMatrix3x3f(NMATRIX3x3 *matresult,const Nf32 sx,const Nf32 sy,const Nf32 sz )
{
	matresult->f00 = sx;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;
	matresult->f01 = 0.0f;	matresult->f11 = sy;	matresult->f21 = 0.0f;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = sz;

}
void NScaleMatrix3x3(NMATRIX3x3 *matresult, const NVEC3 *s)
{
	matresult->f00 = s->x;	matresult->f10 = 0.0f;	matresult->f20 = 0.0f;
	matresult->f01 = 0.0f;	matresult->f11 = s->y;	matresult->f21 = 0.0f;
	matresult->f02 = 0.0f;	matresult->f12 = 0.0f;	matresult->f22 = s->z;
}


// ------------------------------------------------------------------------------------------
// NTransposeMatrixO
// ------------------------------------------------------------------------------------------
// Description :
//	Perform a matrix 3x3 transposition.
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
void NTransposeMatrix3x3O( NMATRIX3x3 *pmatresult, const NMATRIX3x3 *pmat )
{
	NErrorIf((NMATRIX3x3*)pmat == pmatresult, NERROR_MATRIX_IN_OUT_MATRIX_POINTERS_HAS_TO_BE_DIFFERENT);

	pmatresult->f00 = pmat->f00;	pmatresult->f10 = pmat->f01;	pmatresult->f20 = pmat->f02;
	pmatresult->f01 = pmat->f10;	pmatresult->f11 = pmat->f11;	pmatresult->f21 = pmat->f12;
	pmatresult->f02 = pmat->f20;	pmatresult->f12 = pmat->f21;	pmatresult->f22 = pmat->f22;
}

// ------------------------------------------------------------------------------------------
// NTransposeMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
//	Perform a matrix 3x3 transposition.
// ------------------------------------------------------------------------------------------
// In  :
//		pmat			: ptr on initial matrix.
//		pmatresult		: ptr on result matrix. (may be the same as initial)
// Out :
//
// ------------------------------------------------------------------------------------------
void NTransposeMatrix3x3( NMATRIX3x3 *pmatresult, const NMATRIX3x3 *pmat )
{
	NMATRIX3x3		tmp;

	tmp.f00 = pmat->f00;	tmp.f10 = pmat->f01;	tmp.f20 = pmat->f02;
	tmp.f01 = pmat->f10;	tmp.f11 = pmat->f11;	tmp.f21 = pmat->f12;
	tmp.f02 = pmat->f20;	tmp.f12 = pmat->f21;	tmp.f22 = pmat->f22;

	*pmatresult = tmp;
}


// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix3x3O
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix3x3.
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
void NMulVector3ByMatrix3x3O(NVEC3 *vr, const NMATRIX3x3 *mat, const NVEC3 *v)
{
	NErrorIf( vr == (NVEC3*)v, NERROR_MATRIX_IN_OUT_VECTOR3_POINTERS_HAS_TO_BE_DIFFERENT);

	vr->x	= v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;
	vr->y	= v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;
	vr->z	= v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;
}

// -------------------------------------------------------------------------------------------
// NMulVector3ByMatrix3x3
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector3 by a Matrix3x3.
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
void NMulVector3ByMatrix3x3(NVEC3 *vr, const NMATRIX3x3 *mat, const NVEC3 *v)
{
	NVEC3	vtmp;
	
	vtmp.x	= v->x * mat->f00 + v->y * mat->f10 + v->z * mat->f20;
	vtmp.y	= v->x * mat->f01 + v->y * mat->f11 + v->z * mat->f21;
	vtmp.z	= v->x * mat->f02 + v->y * mat->f12 + v->z * mat->f22;

	*vr = vtmp;
}
// -------------------------------------------------------------------------------------------
// NMulVector2ByMatrix3x3O
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector2 by a Matrix3x3.
//	Optimized version ... ( one NVEC2 copy less )
//	! vr and v need to be different ! 
//	Use NMulVector2ByMatrixO ONLY if you are sure that "vr" is different than "v"
//	Use NMulVector2ByMatrix for all other cases.
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC2
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector2ByMatrix3x3O(NVEC2 *vr, const NMATRIX3x3 *mat, const NVEC2 *v)
{
	NErrorIf( vr == (NVEC2*)v, NERROR_MATRIX_IN_OUT_VECTOR2_POINTERS_HAS_TO_BE_DIFFERENT);

	vr->x	= v->x * mat->f00 + v->y * mat->f10 + mat->f20;// v->z = 1 ! (Homogeneous 2D coordinates ...)
	vr->y	= v->x * mat->f01 + v->y * mat->f11 + mat->f21;// v->z = 1 ! (Homogeneous 2D coordinates ...) 
	Nf32 z	= v->x * mat->f02 + v->y * mat->f12 + mat->f22;// v->z = 1 !  (Homogeneous 2D coordinates ...)

	NErrorIf(z == 0.0f,NERROR_MATRIX_NULL_Z);
	if(z != 1.0f)
	{
		vr->x /= z;
		vr->y /= z;
	}

}

// -------------------------------------------------------------------------------------------
// NMulVector2ByMatrix3x3
// -------------------------------------------------------------------------------------------
// Description :
//	Multiply a vector2 by a Matrix3x3.
//	vr and v maybe the same one
// -------------------------------------------------------------------------------------------
// In  :
//		v	: NVEC2
//		mat	: Matrix.
//		vr	: result vector
//
// Out :
//
// -------------------------------------------------------------------------------------------
void NMulVector2ByMatrix3x3(NVEC2 *vr, const NMATRIX3x3 *mat, const NVEC2 *v)
{
	NVEC2	vtmp;

	vtmp.x	= v->x * mat->f00 + v->y * mat->f10 + mat->f20;// v->z = 1 ! (Homogeneous 2D coordinates ...)
	vtmp.y	= v->x * mat->f01 + v->y * mat->f11 + mat->f21;// v->z = 1 ! (Homogeneous 2D coordinates ...) 
	Nf32 z	= v->x * mat->f02 + v->y * mat->f12 + mat->f22;// v->z = 1 !  (Homogeneous 2D coordinates ...)

	NErrorIf(z == 0.0f,NERROR_MATRIX_NULL_Z);
	if(z != 1.0f)
	{
		vr->x = vtmp.x/z;
		vr->y = vtmp.y/z;
	}
	else
	{
		*vr = vtmp;
	}
}

// ------------------------------------------------------------------------------------------
// NRotationMatrix3x3_AxisX
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
void NRotationMatrix3x3_AxisX(NMATRIX3x3 *pmat, Nf32 angx_rad )
{
	Nf32	co,si;
	NCosSin(angx_rad, &co, &si);
	pmat->f00=1.0f;		pmat->f10=0.0f;		pmat->f20=0.0f;	
	pmat->f01=0.0f;		pmat->f11=co;		pmat->f21=-si; 	
	pmat->f02=0.0f;		pmat->f12=si;		pmat->f22=co;  	
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix3x3_AxisX(NMATRIX3x3 *pmat, Nu32 nanglex )
{
	Nf32	co,si;
	NFastCosSin(nanglex, &co, &si);
	pmat->f00=1.0f;		pmat->f10=0.0f;		pmat->f20=0.0f;		
	pmat->f01=0.0f;		pmat->f11=co;		pmat->f21=-si; 		
	pmat->f02=0.0f;		pmat->f12=si;		pmat->f22=co;  		
}
*/

// ------------------------------------------------------------------------------------------
// NRotationMatrix3x3_AxisY
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
void NRotationMatrix3x3_AxisY(NMATRIX3x3 *pmat, Nf32 angy_rad )
{
	Nf32		co,si;
	NCosSin(angy_rad,&co,&si);

	pmat->f00=co;		pmat->f10=0.0f;		pmat->f20=si;	
	pmat->f01=0.0f;		pmat->f11=1.0f;		pmat->f21=0.0f;	
	pmat->f02=-si;		pmat->f12=0.0f;		pmat->f22=co;	
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix3x3_AxisY(NMATRIX3x3 *pmat, Nu32 nangley )
{
	Nf32		co,si;
	NFastCosSin(nangley,&co,&si);

	pmat->f00=co;		pmat->f10=0.0f;		pmat->f20=si;	
	pmat->f01=0.0f;		pmat->f11=1.0f;		pmat->f21=0.0f;	
	pmat->f02=-si;		pmat->f12=0.0f;		pmat->f22=co;	
}
*/
// ------------------------------------------------------------------------------------------
// NRotationMatrix3x3_AxisZ
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
void NRotationMatrix3x3_AxisZ(NMATRIX3x3 *pmat, Nf32 angz_rad )
{
	Nf32		co,si;
	NCosSin(angz_rad,&co,&si);

	pmat->f00=co;		pmat->f10=-si;		pmat->f20=0.0f;	
	pmat->f01=si;		pmat->f11=co;		pmat->f21=0.0f;	
	pmat->f02=0.0f;		pmat->f12=0.0f;		pmat->f22=1.0;	
}
/*
// Same function but with NFast Angle System
void NFastRotationMatrix3x3_AxisZ(NMATRIX3x3 *pmat, Nu32 nanglez )
{
	Nf32		co,si;
	NFastCosSin(nanglez,&co,&si);

	pmat->f00=co;		pmat->f10=-si;		pmat->f20=0.0f;	
	pmat->f01=si;		pmat->f11=co;		pmat->f21=0.0f;	
	pmat->f02=0.0f;		pmat->f12=0.0f;		pmat->f22=1.0;	
}
*/
/*
void NForwardVectorMatrix3x3Ex(NMATRIX3x3 *pbase, const NVEC3 *pto, const NVEC3 *pfrom, const NVEC3 *pup)
{
	NErrorIf(!pto||!pup,NERROR_NULL_VECTOR);

	Nf32		length;

	// forward = lookat - origin
	if(pfrom)
		NVec3Sub((NVEC3*)&pbase->Forward,pto,pfrom);
	else
		*(NVEC3*)&pbase->Forward=*pto;
	length = NVec3Length(&pbase->Forward);

	if(length<NF32_EPSILON_VECTOR_LENGTH)// "porigin" and "LookAt point" are close together !
	{
		NErrorIf(1,NERROR_NULL_VECTOR);	 // Crash in Debug Mode	Only
	}
	NVec3ScaleBy(&pbase->Forward,1.0f/length);		// normalize forward AXIS

	if(pup)
	{
		NVec3CrossProduct(&pbase->Side,&pbase->Forward,pup);	// use pup vector to build side AXIS
		length = NVec3Length(&pbase->Side);
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
		NVec3ScaleBy(&pbase->Side,1.0f/length); // normalize 
	}
	else
	{
		NVEC3	up;
		NUT_HughesMollerChooseUp(&up,&pbase->Forward);				// Choose/Use a temporary up vector to build side AXIS.
		NVec3CrossProduct(&pbase->Side,&pbase->Forward,&up);		// Build side AXIS.
		NVec3Normalize(&pbase->Side);							// Normalize it.
	}
	// Compute Base Up = side x forward
	NVec3CrossProduct(&pbase->Up,&pbase->Side,&pbase->Forward);	// we don't normalize this one, not necessary
}


void NForwardVectorMatrix3x3(NMATRIX3x3 *pbase, const NVEC3 *punit_forward)
{
	// 'punit_forward' has to be a unit vector
	NErrorIf(NABS(1.0f-NVec3Length(punit_forward))>NF32_EPSILON_VECTOR_LENGTH,NERROR_NON_UNIT_VECTOR);	 // Crash in Debug Mode	Only

	NVEC3	up;
	NUT_HughesMollerChooseUp(&up,punit_forward);
	NVec3CrossProduct(&pbase->Side,&up,punit_forward);		// use pup vector to build side AXIS
	NVec3Normalize(&pbase->Side);
	NVec3CrossProduct(&pbase->Up,punit_forward,&pbase->Side); // we don't normalize this one, not necessary
	NErrorIf(NABS(1.0f-NVec3Length(&pbase->Up))>NF32_EPSILON_VECTOR_LENGTH,NERROR_NON_UNIT_VECTOR);	 // Crash in Debug Mode	Only

	pbase->Forward = *punit_forward;
	printf("\n----------------------------------------");
	printf("\nForward Vector (%.3f,%.3f,%.3f)",punit_forward->x,punit_forward->y,punit_forward->z);
	printf("\nNUT_HughesMollerChooseUp returns (%.3f,%.3f,%.3f)",up.x,up.y,up.z);
	printf("\nXAXIS: (%.3f,%.3f,%.3f)",pbase->Side.x,pbase->Side.y,pbase->Side.z);
	printf("\nNUT_YAXIS: (%.3f,%.3f,%.3f)",pbase->Up.x,pbase->Up.y,pbase->Up.z);
	printf("\nNUT_ZAXIS: (%.3f,%.3f,%.3f)",pbase->Forward.x,pbase->Forward.y,pbase->Forward.z);

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