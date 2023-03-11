#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NViewport.h"

#include "NUT_3DMouse.h"

// ------------------------------------------------------------------------------------------
// void NUT_Get3DMouse
// ------------------------------------------------------------------------------------------
// Description :
// Compute 3D World coordinates from a 2D Screen coord point
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouse(NVEC3 *pmouse3d, NCAMERA *pcam, const NVEC2s16 *pmouse2d)
{
	NCAMERA *pcamera;
	//	NVEC3				vx,vy,vz;
	Nf32 xl, yl, zl;
	Nf32 x, y;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUpdateCameraAxis(pcamera); // this function update Axis camera ONLY if it's necessary.

	zl = pcamera->fZNear;
	yl = zl * pcamera->PreCompute_Tangente;
	xl = yl * NViewPort.AspectRatio; // Aspect Ratio is Width/Height

	/*
	 * Old version
	x = ( (Nf32)pmouse2d->x - NViewPort.WidthOutOf2 ) / NViewPort.WidthOutOf2;
	y = ( -(Nf32)pmouse2d->y + NViewPort.HeightOutOf2 ) / NViewPort.HeightOutOf2; // '-' due to the 2D screen base is inverted ( origin is LEFT/TOP corner )
	// 3D Mouse Camera Coordinates system
	NVec3Scale(&vx,&pcamera->XAxis,x*xl);
	NVec3Scale(&vy,&pcamera->YAxis,y*yl);
	NVec3Scale(&vz,&pcamera->ZAxis,zl);
	// 3D Mouse World Coordinates system
	*pmouse3d = pcamera->Pos;
	NVec3AddTo(pmouse3d,&vx);
	NVec3AddTo(pmouse3d,&vy);
	NVec3AddTo(pmouse3d,&vz);
	*/
	// New version ( same thing but no 'NVecAddTo' and no 'NVec3Scale' calls )
	x = (((Nf32)pmouse2d->x - NViewPort.WidthOutOf2) / NViewPort.WidthOutOf2) * xl;
	y = ((-(Nf32)pmouse2d->y + NViewPort.HeightOutOf2) / NViewPort.HeightOutOf2) * yl; // '-' due to the 2D screen base is inverted ( origin is LEFT/TOP corner )
	pmouse3d->x = pcamera->Pos.x + pcamera->XAxis.x * x + pcamera->YAxis.x * y + pcamera->ZAxis.x * zl;
	pmouse3d->y = pcamera->Pos.y + pcamera->XAxis.y * x + pcamera->YAxis.y * y + pcamera->ZAxis.y * zl;
	pmouse3d->z = pcamera->Pos.z + pcamera->XAxis.z * x + pcamera->YAxis.z * y + pcamera->ZAxis.z * zl;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Compute a 3DWorld position of a 2D screen coordinates point
 *			(mouse pointer coordinates for example).
 *
 *			There are 2 versions of this function:
 *				NUT_Get3DMouseEx (this one)
 *				NUT_Get3DMouse
 *			There is one simple difference between the 2 versions.
 *			Actually, a 2D point on screen can be an infinity of 3D points, all the ones which are
 *			on the 3Dline pass through the cameraPos and this 2D point !
 *			To calculate 3DWorld coordinates we need to choose a from camera distance (to this "2Dpoint").
 *			This distance will be the Z component of the 3D point into the camera coordinates system ...
 *			So, that's the point! with the "NUT_Get3DMouse" function, this Z is set for you to "Cam ZNear"
 *			...and in this version "NUT_Get3DMouseEx", you choose which Z (camera coord. system) you want.
 *
 *	@param	pmouse3d			a pointer on a NVEC3f32 struct where the function is going to write  the result.
 *	@param	pcam				a pointer on a NVEC3f32 struct where the function is going to write  the result.
 *	@param	pmouse2d			a pointer on a NVEC3f32 struct where the function is going to write  the result.
 *	@param	z_camcoordsystem	this function is going to use this parameter instead of "pcamera->fZNear" as the z position of the "screenplane" where the 2D mouse is supposed to be.
 *								( function \e NUT_Get3DMouse is using "pcamera->fZNear" for that purpose )
 */
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouseEx(NVEC3 *pmouse3d, NCAMERA *pcam, const NVEC2s16 *pmouse2d, const Nf32 z_camcoordsystem)
{
	NCAMERA *pcamera;
	//	NVEC3	vx,vy,vz;
	Nf32 xl, yl /*,zl*/;
	Nf32 x, y;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUpdateCameraAxis(pcamera); // this function update Axis camera ONLY if it's necessary.

	// zl = z_camcoordsystem;
	yl = /*zl*/ z_camcoordsystem * pcamera->PreCompute_Tangente;
	xl = yl * NViewPort.AspectRatio; // Aspect Ratio is Width/Height

	/*
	 * Old version
	x = ( (Nf32)pmouse2d->x - NViewPort.WidthOutOf2 ) / NViewPort.WidthOutOf2;
	y = ( -(Nf32)pmouse2d->y + NViewPort.HeightOutOf2 ) / NViewPort.HeightOutOf2;

	// 3D Mouse Camera Coordinates system
	NVec3Scale(&vx,&pcamera->XAxis,x*xl);
	NVec3Scale(&vy,&pcamera->YAxis,y*yl);
	NVec3Scale(&vz,&pcamera->ZAxis,z_camcoordsystem);

	// 3D Mouse World Coordinates system
	*pmouse3d = pcamera->Pos;
	NVec3AddTo(pmouse3d,&vx);
	NVec3AddTo(pmouse3d,&vy);
	NVec3AddTo(pmouse3d,&vz);
	*/
	// New version ( same thing but no 'NVecAddTo' and no 'NVec3Scale' calls )
	x = (((Nf32)pmouse2d->x - NViewPort.WidthOutOf2) / NViewPort.WidthOutOf2) * xl;
	y = ((-(Nf32)pmouse2d->y + NViewPort.HeightOutOf2) / NViewPort.HeightOutOf2) * yl; // '-' due to the 2D screen base is inverted ( origin is LEFT/TOP corner )
	pmouse3d->x = pcamera->Pos.x + pcamera->XAxis.x * x + pcamera->YAxis.x * y + pcamera->ZAxis.x * z_camcoordsystem;
	pmouse3d->y = pcamera->Pos.y + pcamera->XAxis.y * x + pcamera->YAxis.y * y + pcamera->ZAxis.y * z_camcoordsystem;
	pmouse3d->z = pcamera->Pos.z + pcamera->XAxis.z * x + pcamera->YAxis.z * y + pcamera->ZAxis.z * z_camcoordsystem;
}

// ------------------------------------------------------------------------------------------
// void NUT_Get3DMouseRay
// ------------------------------------------------------------------------------------------
// Description :
// Compute a ray that start from camera world position and pass trough 3dmouse world position.
// Be aware that the resulting ray vector (pdirector) is not normalized !!!
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouseRay(NVEC3 *porigin, NVEC3 *pdirector, NCAMERA *pcam, const NVEC2s16 *pmouse2d)
{
	NCAMERA *pcamera;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUT_Get3DMouseEx(pdirector, pcamera, pmouse2d, 1.0f); // 1.0f for a better precision (i guess) ...
	// NVec3SubFrom(pdirector,&pcamera->Pos);
	// NVec3Normalize(pdirector);
	pdirector->x -= pcamera->Pos.x;
	pdirector->y -= pcamera->Pos.y;
	pdirector->z -= pcamera->Pos.z;
	*porigin = pcamera->Pos;
}
// ------------------------------------------------------------------------------------------
// void NUT_Get3DMouseRayEx
// ------------------------------------------------------------------------------------------
// Description :
// Compute a ray that start from camera world position and pass trough 3dmouse world position.
// Be aware that the resulting ray vector (pdirector) is not normalized !!!
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouseRayEx(NVEC3 *porigin, NVEC3 *pdirector, NCAMERA *pcam, const NVEC2s16 *pmouse2d, const Nf32 z_camcoordsystem)
{
	NCAMERA *pcamera;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUT_Get3DMouseEx(pdirector, pcamera, pmouse2d, z_camcoordsystem);
	// NVec3SubFrom(pdirector,&pcamera->Pos);
	// NVec3Normalize(pdirector);
	pdirector->x -= pcamera->Pos.x;
	pdirector->y -= pcamera->Pos.y;
	pdirector->z -= pcamera->Pos.z;
	*porigin = pcamera->Pos;
}
// ------------------------------------------------------------------------------------------
// void NUT_Get3DMouseSegment
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouseSegment(NVEC3 *psegA, NVEC3 *psegB, NCAMERA *pcam, const NVEC2s16 *pmouse2d)
{
	NCAMERA *pcamera;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUT_Get3DMouseEx(psegB, pcamera, pmouse2d, 1.0f); // 1.0f for a better precision (i guess) ...
	*psegA = pcamera->Pos;
}
// ------------------------------------------------------------------------------------------
// void NUT_Get3DMouseSegmentEx
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Get3DMouseSegmentEx(NVEC3 *psegA, NVEC3 *psegB, NCAMERA *pcam, const NVEC2s16 *pmouse2d, const Nf32 z_camcoordsystem)
{
	NCAMERA *pcamera;

	if (pcam)
		pcamera = pcam;
	else
		pcamera = NGetCamera();

	NUT_Get3DMouseEx(psegB, pcamera, pmouse2d, z_camcoordsystem);
	*psegA = pcamera->Pos;
}

//===============================================================================================================================================================================================
/*

void MultiplyMatrices4by4OpenGL_Nf32(Nf32 *result, const Nf32 *matrix1, const Nf32 *matrix2)
{
	result[0] = matrix1[0]*matrix2[0]	+	matrix1[4]*matrix2[1]	+	matrix1[8]*matrix2[2]	+	matrix1[12]*matrix2[3];
	result[4] = matrix1[0]*matrix2[4]	+	matrix1[4]*matrix2[5]	+	matrix1[8]*matrix2[6]	+	matrix1[12]*matrix2[7];
	result[8]=matrix1[0]*matrix2[8]+
		matrix1[4]*matrix2[9]+
		matrix1[8]*matrix2[10]+
		matrix1[12]*matrix2[11];
	result[12]=matrix1[0]*matrix2[12]+
		matrix1[4]*matrix2[13]+
		matrix1[8]*matrix2[14]+
		matrix1[12]*matrix2[15];
	result[1]=matrix1[1]*matrix2[0]+
		matrix1[5]*matrix2[1]+
		matrix1[9]*matrix2[2]+
		matrix1[13]*matrix2[3];
	result[5]=matrix1[1]*matrix2[4]+
		matrix1[5]*matrix2[5]+
		matrix1[9]*matrix2[6]+
		matrix1[13]*matrix2[7];
	result[9]=matrix1[1]*matrix2[8]+
		matrix1[5]*matrix2[9]+
		matrix1[9]*matrix2[10]+
		matrix1[13]*matrix2[11];
	result[13]=matrix1[1]*matrix2[12]+
		matrix1[5]*matrix2[13]+
		matrix1[9]*matrix2[14]+
		matrix1[13]*matrix2[15];
	result[2]=matrix1[2]*matrix2[0]+
		matrix1[6]*matrix2[1]+
		matrix1[10]*matrix2[2]+
		matrix1[14]*matrix2[3];
	result[6]=matrix1[2]*matrix2[4]+
		matrix1[6]*matrix2[5]+
		matrix1[10]*matrix2[6]+
		matrix1[14]*matrix2[7];
	result[10]=matrix1[2]*matrix2[8]+
		matrix1[6]*matrix2[9]+
		matrix1[10]*matrix2[10]+
		matrix1[14]*matrix2[11];
	result[14]=matrix1[2]*matrix2[12]+
		matrix1[6]*matrix2[13]+
		matrix1[10]*matrix2[14]+
		matrix1[14]*matrix2[15];
	result[3]=matrix1[3]*matrix2[0]+
		matrix1[7]*matrix2[1]+
		matrix1[11]*matrix2[2]+
		matrix1[15]*matrix2[3];
	result[7]=matrix1[3]*matrix2[4]+
		matrix1[7]*matrix2[5]+
		matrix1[11]*matrix2[6]+
		matrix1[15]*matrix2[7];
	result[11]=matrix1[3]*matrix2[8]+
		matrix1[7]*matrix2[9]+
		matrix1[11]*matrix2[10]+
		matrix1[15]*matrix2[11];
	result[15]=matrix1[3]*matrix2[12]+
		matrix1[7]*matrix2[13]+
		matrix1[11]*matrix2[14]+
		matrix1[15]*matrix2[15];
}

void MultiplyMatrixByVector4by4OpenGL_Nf32(Nf32 *resultvector, const Nf32 *matrix, const Nf32 *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
	resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
	resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
	resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

//This code comes directly from GLU except that it is for float
int glhInvertMatrixf2(Nf32 *out, const Nf32 *m )
{
	Nf32 wtmp[4][8];
	Nf32 m0, m1, m2, m3, s;
	Nf32 *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	/ * choose pivot - or die * /
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS_FLOAT(r1, r0);
	if (0.0 == r0[0])
		return 0;
	/ * eliminate first variable     * /
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/ * choose pivot - or die * /
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (0.0 == r1[1])
		return 0;
	/ * eliminate second variable * /
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/ * choose pivot - or die * /
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (0.0 == r2[2])
		return 0;
	/ * eliminate third variable * /
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	/ * last check * /
	if (0.0 == r3[3])
		return 0;
	s = 1.0f / r3[3];		/ * now back substitute row 3 * /
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3];			/ * now back substitute row 2 * /
	s = 1.0f / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2];			/ * now back substitute row 1 * /
	s = 1.0f / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1];			/ * now back substitute row 0 * /
	s = 1.0f / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	return 1;
}

int _glhProjectf(Nf32 *result_windowCoordinate, const Nf32 objx,const Nf32 objy,const Nf32 objz,const Nf32 *modelview,const Nf32 *projection,const int *viewport )
{
	//Transformation vectors
	Nf32 fTempo[8];

	//Modelview transform
	fTempo[0]=modelview[0]*objx+modelview[4]*objy+modelview[8]*objz+modelview[12];  //w is always 1
	fTempo[1]=modelview[1]*objx+modelview[5]*objy+modelview[9]*objz+modelview[13];
	fTempo[2]=modelview[2]*objx+modelview[6]*objy+modelview[10]*objz+modelview[14];
	fTempo[3]=modelview[3]*objx+modelview[7]*objy+modelview[11]*objz+modelview[15];
	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
	fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
	fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
	fTempo[7]=-fTempo[2];
	//The result normalizes between -1 and 1
	if(fTempo[7]==0.0)	//The w value
		return 0;
	fTempo[7]=1.0f/fTempo[7];
	//Perspective division
	fTempo[4]*=fTempo[7];
	fTempo[5]*=fTempo[7];
	fTempo[6]*=fTempo[7];
	//Window coordinates
	//Map x, y to range 0-1
	result_windowCoordinate[0]=(fTempo[4]*0.5f+0.5f)*viewport[2]+viewport[0];
	result_windowCoordinate[1]=(fTempo[5]*0.5f+0.5f)*viewport[3]+viewport[1];
	//This is only correct when glDepthRange(0.0, 1.0)
	result_windowCoordinate[2]=(1.0f+fTempo[6])*0.5f;	//Between 0 and 1
	return 1;
}

int _glhUnProjectf(Nf32 *result_object3Dcoord, const Nf32 winx,const Nf32 winy,const Nf32 winz,const Nf32 *modelview,const Nf32 *projection,const int *viewport )
{
	//Transformation matrices
	Nf32 m[16], A[16];
	Nf32 in[4], out[4];

	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	MultiplyMatrices4by4OpenGL_Nf32(A, projection, modelview);
	//Now compute the inverse of matrix A
	if(glhInvertMatrixf2(m,A)==0)
		return 0;
	//Transformation of normalized coordinates between -1 and 1
	in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0f-1.0f;
	in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0f-1.0f;
	in[2]=2.0f*winz-1.0f;
	in[3]=1.0f;
	//Objects coordinates
	MultiplyMatrixByVector4by4OpenGL_Nf32(out, m, in);
	if(out[3]==0.0)
		return 0;
	out[3]=1.0f/out[3];
	result_object3Dcoord[0]=out[0]*out[3];
	result_object3Dcoord[1]=out[1]*out[3];
	result_object3Dcoord[2]=out[2]*out[3];
	return 1;
}

void NUT_UnProject2DMouse(NVEC3 *pmouse3d, const NVEC2s16 *pmouse2d )
{
	NCAMERA	*pcamera;
	pcamera = NGetCamera();

// ---------------------
// Without GLU
// ---------------------
	Nf32	mwinx,mwiny,mwinz;
	float	mmodelview[16];
	float	mprojection[16];
	GLint	mviewport[4];

	glGetFloatv(GL_MODELVIEW_MATRIX, mmodelview);
	glGetFloatv(GL_PROJECTION_MATRIX, mprojection);
	glGetIntegerv( GL_VIEWPORT, mviewport );

	mwinx = (Nf32)pmouse2d->x;
	mwiny = (Nf32)mviewport[3] - (Nf32)pmouse2d->y; // Hauteur ecran - y ( sinon y invers� )
	mwinz = pcamera->fZNear;
	_glhUnProjectf((Nf32*)pmouse3d,mwinx,mwiny,mwinz, mmodelview, mprojection, mviewport );
	//printf("ByMat: x:%f Y:%f Z:%f\n",pmouse3d->x,pmouse3d->y,pmouse3d->z);

/ *
// ---------------------
// With GLU
// ---------------------
	GLdouble	winx,winy,winz;
	GLdouble	outx,outy,outz;
	GLdouble	modelview[16];
	GLdouble	projection[16];
	GLint		viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv( GL_VIEWPORT, viewport );

	winx = (GLdouble)pmouse2d->x;
	winy = (GLdouble)viewport[3] - (GLdouble)pmouse2d->y; // Hauteur ecran - y ( sinon y invers� )
	winz = pcamera->fZNear;
	gluUnProject( winx, winy, winz, modelview, projection, viewport, &outx,&outy,&outz);

	// DEBUG CONSOLE OUTPUT
	// compare with and without GLU
	printf("ByMat: x:%f Y:%f Z:%f\n",pmouse3d->x,pmouse3d->y,pmouse3d->z);
	pmouse3d->x = (Nf32)outx;
	pmouse3d->y = (Nf32)outy;
	pmouse3d->z = (Nf32)outz;
	printf("ByGlu: x:%f Y:%f Z:%f\n",outx,outy,outz);
	printf("-----\n");
* /

/ *
// ---------------------
// MOUSE RAY CALCULATION
// ---------------------
	vect.x = (float)posX[0] - Camera.Pos.x;
	vect.y = (float)posY[0] - Camera.Pos.y;
	vect.z = (float)posZ[0] - Camera.Pos.z;

	f = NVec3FastNormalize(&vect);//f = NVec3Normalize(&vect);
	NSetParticleInfluence_RayDirectorVector(pdeflector, &vect, &Camera.Pos);
	NVec3Scale(&vect,&vect,0.01f);
* /
}
*/