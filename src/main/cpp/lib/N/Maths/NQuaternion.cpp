// ==========================================================================================
// Quaternion.cpp
// ------------------------------------------------------------------------------------------
// Author  : Jean-Marie Nazaret
// Created : 23/01/2012
// revision:
// ------------------------------------------------------------------------------------------
// Quaternions fonctions.
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NMath.h"

#include "../NQuaternion.h"

// ------------------------------------------------------------------------------------------
// NIdentityQuat
// ------------------------------------------------------------------------------------------
// Description :
//	Write the right values into a Quaternion to obtained an "Identity Quaternion".
// ------------------------------------------------------------------------------------------
// In  :
//		pquat	: Pointer on the quaternion.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NIdentityQuaternion(NQUATERNION *pquat)
{
	pquat->x = 0.0f;
	pquat->y = 0.0f;
	pquat->z = 0.0f;
	pquat->w = 1.0f;
}

// ------------------------------------------------------------------------------------------
// NQuatRotation
// ------------------------------------------------------------------------------------------
// Description :
//	compute a quaternion to describe a rotation around any rotation axis.
//	Radian for the angle.
// ------------------------------------------------------------------------------------------
// In  :
//		v		: Vector's pointer which represents the rotation axis.
//		ang		: Rotation angle in radian
//		quat	: Quaternion's pointer to fill in.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NQuaternionAxisRotation(NQUATERNION *pquat, const NVEC3 *v, const Nf32 ang)
{
	Nf32 si;

	NCosSin((ang * 0.5f), &pquat->w, &si); // Optimize  NCosSin function ASAP !!!!!
	pquat->x = si * v->x;
	pquat->y = si * v->y;
	pquat->z = si * v->z;
}
void NFastQuaternionAxisRotation(NQUATERNION *pquat, const NVEC3 *v, const Nu32 ang)
{
	Nf32 si;

	NFastCosSin((ang / 2), &pquat->w, &si);
	pquat->x = si * v->x;
	pquat->y = si * v->y;
	pquat->z = si * v->z;
}

// ------------------------------------------------------------------------------------------
// NQuaternionRotation
// ------------------------------------------------------------------------------------------
// Description :
//	Create a quaternion form x,y,z rotations.
//	Radians for the angles.
// ------------------------------------------------------------------------------------------
// In  :
//		rx,ry,rz	: rotations angles (rad).
//		quat		: pointer on the quaternion to fill in.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NQuaternionRotation(NQUATERNION *pquat, const Nf32 rx, const Nf32 ry, const Nf32 rz)
{
	Nf32 cx, cy, cz, sx, sy, sz, cycz, sysz;

	NCosSin((rx * 0.5f), &cx, &sx);
	NCosSin((ry * 0.5f), &cy, &sy);
	NCosSin((rz * 0.5f), &cz, &sz);

	cycz = cy * cz;
	sysz = sy * sz;

	pquat->w = cx * cycz + sx * sysz;
	pquat->x = sx * cycz - cx * sysz;
	pquat->y = cx * sy * cz + sx * cy * sz;
	pquat->z = cx * cy * sz - sx * sy * cz;
}

// ------------------------------------------------------------------------------------------
// NMatrixToQuaternion
// ------------------------------------------------------------------------------------------
// Description :
//	Create a quaternion from a Matrix
// ------------------------------------------------------------------------------------------
// In  :
//		mat		: The Matrix
//		quat	: The quaternion to fill in
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NMatrixToQuaternion(NQUATERNION *quat, const NMATRIX *mat)
{
	Nf32 tr, s, q[4];
	int i, j, k;
	int nxt[3] = {1, 2, 0};

	tr = mat->f00 + mat->f11 + mat->f22;

	if (tr > 0.0f)
	{
		// ------------------
		// Positive  Diagonal
		// ------------------
		s = sqrt(tr + 1.0f);
		quat->w = s / 2.0f;
		s = 0.5f / s;
		quat->x = (mat->f12 - mat->f21) * s;
		quat->y = (mat->f20 - mat->f02) * s;
		quat->z = (mat->f01 - mat->f10) * s;
	}
	else
	{
		// -----------------
		// Negative Diagonal
		// -----------------
		i = 0;
		if (mat->f11 > mat->f00)
			i = 1;
		if (mat->f22 > mat->f11)
			i = 2;

		j = nxt[i];
		k = nxt[j];
		s = (Nf32)sqrt((mat->Tf[i][i] - (mat->Tf[j][j] + mat->Tf[k][k])) + 1.0f);

		q[i] = s * 0.5f;
		if (s != 0.0f)
			s = 0.5f / s;
		q[3] = (mat->Tf[j][k] - mat->Tf[k][j]) * s;
		q[j] = (mat->Tf[i][j] + mat->Tf[j][i]) * s;
		q[k] = (mat->Tf[i][k] + mat->Tf[k][i]) * s;

		quat->x = q[0];
		quat->y = q[1];
		quat->z = q[2];
		quat->w = q[3];
	}
}

// ------------------------------------------------------------------------------------------
// NQuaternionToMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Create a Matrix from a quaternion.
// ------------------------------------------------------------------------------------------
// In  :
//		quat	:THE quaternion.
//		mat		: The Matrix to fill in.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NQuaternionToMatrix(NMATRIX *mat, const NQUATERNION *quat)
{
	Nf32 wx, wy, wz;
	Nf32 xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = quat->x + quat->x;
	y2 = quat->y + quat->y;
	z2 = quat->z + quat->z;

	xx = quat->x * x2;
	xy = quat->x * y2;
	xz = quat->x * z2;
	yy = quat->y * y2;
	yz = quat->y * z2;
	zz = quat->z * z2;
	wx = quat->w * x2;
	wy = quat->w * y2;
	wz = quat->w * z2;

	mat->f00 = 1.0f - (yy + zz);
	mat->f01 = xy + wz;
	mat->f02 = xz - wy;
	mat->f03 = 0.0f;

	mat->f10 = xy - wz;
	mat->f11 = 1.0f - (xx + zz);
	mat->f12 = yz + wx;
	mat->f13 = 0.0f;

	mat->f20 = xz + wy;
	mat->f21 = yz - wx;
	mat->f22 = 1.0f - (xx + yy);
	mat->f23 = 0.0f;

	mat->f30 = 0.0f;
	mat->f31 = 0.0f;
	mat->f32 = 0.0f;
	mat->f33 = 1.0f;
}

// ------------------------------------------------------------------------------------------
// NMulQuaternion
// ------------------------------------------------------------------------------------------
// Description :
//	Multiply 2 quaternions.
//	!: Quaternions have to be multiply in the same order as the rotations ( not like the Matrix)
//	EXEMPLE:
//		q1q2 means : q2 rotation and q1 rotation.
//
// ------------------------------------------------------------------------------------------
// In  :
//		q1	: First quaternion.
//		q2	: second quaternion.
//		res	: Result.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NMulQuaternion(NQUATERNION *res, const NQUATERNION *q1, const NQUATERNION *q2)
{
	NQUATERNION calc;

	calc.x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
	calc.y = q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z;
	calc.z = q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x;
	calc.w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;

	res->x = calc.x;
	res->y = calc.y;
	res->z = calc.z;
	res->w = calc.w;
	/*
		float A = (q1->w + q1->x) * (q2->w + q2->x);
		float B = (q1->z - q1->y) * (q2->y - q2->z);
		float C = (q1->x - q1->w) * (q2->y + q2->z);
		float D = (q1->y + q1->z) * (q2->x - q2->w);
		float E = (q1->x + q1->z) * (q2->x + q2->y);
		float F = (q1->x - q1->z) * (q2->x - q2->y);
		float G = (q1->w + q1->y) * (q2->w - q2->z);
		float H = (q1->w - q1->y) * (q2->w + q2->z);

		float EpF = E + F, EmF = E - F;
		float GpH = G + H, GmH = G - H;

		res->w = B + 0.5f * (GpH - EpF);
		res->x = A - 0.5f * (GpH + EpF);
		res->y = -C + 0.5f * (EmF + GmH);
		res->z = -D + 0.5f * (EmF - GmH);
	*/
}
Nf32 NQuaternionLength(const NQUATERNION *q)
{
	return sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
}

Nf32 NQuaternionNormalize(NQUATERNION *q)
{
	Nf32 l;
	Nf32 ls;
	l = sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
	ls = 1.0f / l;

	q->w *= ls;
	q->x *= ls;
	q->y *= ls;
	q->z *= ls;

	return l;
}
// ------------------------------------------------------------------------------------------
// NQuaternionSlerp
// ------------------------------------------------------------------------------------------
// Description :
//	Perform a SLERP (Spherical Linear intERPolation) between 2 quaternions.
// ------------------------------------------------------------------------------------------
// In  :
//		from	: Start Quaternion.
//		to		: End Quaternion.
//		t		: between 0.0 (0%) et 1.0 (100%).
//		res		: result Quaternion.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NQuaternionSlerp(NQUATERNION *res, const NQUATERNION *from, const NQUATERNION *to, const Nf32 t)
{
	Nf32 to1[4];
	Nf32 omega, cosomega, sinomega, scale0, scale1;

	// ------------------------------------------------------
	// Scalar product (to know the cosine of the angle
	// between "from" and "to")
	// ------------------------------------------------------
	cosomega = from->x * to->x + from->y * to->y + from->z * to->z + from->w * to->w;

	// -----------------------------------------------------------------
	// Sign adjustment if it's necessary to use the shortest path
	// -----------------------------------------------------------------
	if (cosomega < 0.0f)
	{
		cosomega = -cosomega;
		to1[0] = -to->x;
		to1[1] = -to->y;
		to1[2] = -to->z;
		to1[3] = -to->w;
	}
	else
	{
		to1[0] = to->x;
		to1[1] = to->y;
		to1[2] = to->z;
		to1[3] = to->w;
	}

	// ------------------------
	// coefficients calculation
	// ------------------------
	if (cosomega < 0.9999f) // if( (1.0f - cosom) > 0.0001f )
	{
		// ---------------------
		// Standard Case (Slerp)
		// ---------------------
		omega = (Nf32)acos(cosomega); // Angle between "from" & "to"
		sinomega = (Nf32)sin(omega);  // Angle Sine
		scale0 = (Nf32)sin((1.0f - t) * omega) / sinomega;
		scale1 = (Nf32)sin(t * omega) / sinomega;
	}
	else
	{
		// -------------------------------------------------
		// "from" & "to" quaternions are close :
		// We can perform a linear interpolation
		// -------------------------------------------------
		scale0 = 1.0f - t;
		scale1 = t;
	}

	// ------------
	// Final Values
	// ------------
	res->x = (Nf32)(scale0 * from->x + scale1 * to1[0]);
	res->y = (Nf32)(scale0 * from->y + scale1 * to1[1]);
	res->z = (Nf32)(scale0 * from->z + scale1 * to1[2]);
	res->w = (Nf32)(scale0 * from->w + scale1 * to1[3]);
}

// ------------------------------------------------------------------------------------------
// NMulVectorByQuat
// ------------------------------------------------------------------------------------------
// Description :
//	Multiply a Nvector3  by a quaternion.
//	Thanks using 'vtmp' both incoming VECTOR3 ptr (vr and v) can be the same
// ------------------------------------------------------------------------------------------
// In  :
//		v		: Vector
//		quat	: the quaternion.
//		vr		: result vector
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NMulVectorByQuat(NVEC3 *vr, const NVEC3 *v, const NQUATERNION *quat)
{
	Nf32 wx, wy, wz;
	Nf32 xx, yy, yz, xy, xz, zz, x2, y2, z2;
	NVEC3 vtmp;

	x2 = quat->x + quat->x;
	y2 = quat->y + quat->y;
	z2 = quat->z + quat->z;

	xx = quat->x * x2;
	xy = quat->x * y2;
	xz = quat->x * z2;
	yy = quat->y * y2;
	yz = quat->y * z2;
	zz = quat->z * z2;
	wx = quat->w * x2;
	wy = quat->w * y2;
	wz = quat->w * z2;

	vtmp.x = (v->x * (1.0f - (yy + zz))) + (v->y * (xy - wz)) + (v->z * (xz + wy));
	vtmp.y = (v->x * (xy + wz)) + (v->y * (1.0f - (xx + zz))) + (v->z * (yz - wx));
	vtmp.z = (v->x * (xz - wy)) + (v->y * (yz + wx)) + (v->z * (1.0f - (xx + yy)));

	*vr = vtmp;
}