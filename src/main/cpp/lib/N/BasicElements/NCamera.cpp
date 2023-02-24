#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../NMatrix.h"
#include "../NViewport.h"
#include "../NCamera.h"
#include "lib/N/NErrorHandling.h"

// Global Variable
NCAMERA NCamera;
NVIEWPORT NViewPort;

NCAMERA *NGetCamera() { return &NCamera; };

void NCameraSetUp(NCAMERA *cam_draft)
{
	Nu32 flagstmp;

	flagstmp = NCamera.Flags;

	memcpy(&NCamera, cam_draft, sizeof(NCAMERA));
	NCamera.Flags = flagstmp;

	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
}

void NMoveCameraPosition(const NVEC3 *v)
{
	NVec3AddTo(&NCamera.Pos, v);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NMoveCameraPositionf(const Nf32 x, const Nf32 y, const Nf32 z)
{
	NCamera.Pos.x += x;
	NCamera.Pos.y += y;
	NCamera.Pos.z += z;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NMoveCameraTargetPosition(const NVEC3 *v)
{
	NVec3AddTo(&NCamera.TargetPos, v);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NMoveCameraTargetPositionf(const Nf32 x, const Nf32 y, const Nf32 z)
{
	NCamera.TargetPos.x += x;
	NCamera.TargetPos.y += y;
	NCamera.TargetPos.z += z;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}

void NSetCameraPosition(const NVEC3 *v)
{
	NCamera.Pos = *v;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraPositionf(const Nf32 x, const Nf32 y, const Nf32 z)
{
	NVec3Set(&NCamera.Pos, x, y, z);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraPositionXf(const Nf32 x)
{
	NCamera.Pos.x = x;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraPositionYf(const Nf32 y)
{
	NCamera.Pos.y = y;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraPositionZf(const Nf32 z)
{
	NCamera.Pos.z = z;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}

void NSetCameraTargetPosition(const NVEC3 *v)
{
	NCamera.TargetPos = *v;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraTargetPositionf(const Nf32 x, const Nf32 y, const Nf32 z)
{
	NVec3Set(&NCamera.TargetPos, x, y, z);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraTargetPositionXf(const Nf32 x)
{
	NCamera.TargetPos.x = x;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraTargetPositionYf(const Nf32 y)
{
	NCamera.TargetPos.y = y;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NSetCameraTargetPositionZf(const Nf32 z)
{
	NCamera.TargetPos.z = z;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}

void NCameraSetVectorUp(const NVEC3 *v)
{
	NCamera.Up = *v;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NCameraSetVectorUpf(const Nf32 x, const Nf32 y, const Nf32 z)
{
	NVec3Set(&NCamera.Up, x, y, z);
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED | FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
}
void NCameraSetFov(const Nf32 fov)
{
	NCamera.fFovY = fov;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
}
void NCameraSetAspectRatio(const Nf32 fratio)
{
	NCamera.fAspectRatio = fratio;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
}
void NCameraSetZNear(const Nf32 fnear)
{
	NCamera.fZNear = fnear;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
}
void NCameraSetZFar(const Nf32 ffar)
{
	NCamera.fZFar = ffar;
	FLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
}

// GLint i;

/*
void NSetCameraRoll(const Nf32 angle_roll_rad)
{
	// "Roll"  represents the rotation around Z Axis
	// "Pitch"  represents the rotation around X Axis
	// "Yaw"  represents the rotation around Y Axis

	// "Roll" will change X and Y axis, but Z Axis is going to stay the same.

}
*/

NMATRIX *NGetPerspectiveMatrix(NMATRIX *pmatrix)
{

	Nf32 ymax = NCamera.fZNear * tan(NCamera.fFovY * NF32_PI / 360.0f);
	Nf32 ymin = -ymax;

	Nf32 xmin = ymin * NCamera.fAspectRatio;
	Nf32 xmax = ymax * NCamera.fAspectRatio;

	// Compute the projection matrix exactly like glFrustum() do ...
	pmatrix->f00 = (2.0f * NCamera.fZNear) / (xmax - xmin);
	pmatrix->f10 = 0.0f;
	pmatrix->f20 = (xmax + xmin) / (xmax - xmin);
	pmatrix->f30 = 0.0f;
	pmatrix->f01 = 0.0f;
	pmatrix->f11 = (2.0f * NCamera.fZNear) / (ymax - ymin);
	pmatrix->f21 = (ymax + ymin) / (ymax - ymin);
	pmatrix->f31 = 0.0f;
	pmatrix->f02 = 0.0f;
	pmatrix->f12 = 0.0f;
	pmatrix->f22 = (NCamera.fZFar + NCamera.fZNear) / (NCamera.fZFar - NCamera.fZNear);
	pmatrix->f32 = (2.0f * NCamera.fZFar * NCamera.fZNear) / (NCamera.fZFar - NCamera.fZNear);
	pmatrix->f03 = 0.0f;
	pmatrix->f13 = 0.0f;
	pmatrix->f23 = -1.0f;
	pmatrix->f33 = 1.0f;

	return pmatrix;
}

/*
inline void _LookAt(NCAMERA *pcamera)
{
	GLfloat m[4][4];

	_UpdateCameraAxis(pcamera);

	NIdentityMatrix((NMATRIX*)m);
	m[0][0] = pcamera->XAxis.x;
	m[1][0] = pcamera->XAxis.y;
	m[2][0] = pcamera->XAxis.z;
	m[0][1] = pcamera->YAxis.x;
	m[1][1] = pcamera->YAxis.y;
	m[2][1] = pcamera->YAxis.z;
	m[0][2] = -pcamera->ZAxis.x;
	m[1][2] = -pcamera->ZAxis.y;
	m[2][2] = -pcamera->ZAxis.z;
	glMultMatrixf(&m[0][0]);
	glTranslated(-pcamera->Pos.x, -pcamera->Pos.y, -pcamera->Pos.z);
}*/

static void MygluPerspective(const Nf32 fovy, const Nf32 aspect, const Nf32 znear, const Nf32 zfar)
{
#ifdef _NWINDOWS
	GLdouble xmin, xmax, ymin, ymax;

	ymax = (GLdouble)znear * tan((GLdouble)fovy * NF32_PI / 360.0);
	ymin = -ymax;

	xmin = ymin * (GLdouble)aspect;
	xmax = ymax * (GLdouble)aspect;

	glFrustum(xmin, xmax, ymin, ymax, (GLdouble)znear, (GLdouble)zfar);
#endif

#if defined _NIOS || defined _NANDROID
	GLfloat xmin, xmax, ymin, ymax;

	ymax = (GLfloat)znear * tan((GLfloat)fovy * NF32_PI / 360.0f);
	ymin = -ymax;

	xmin = ymin * (GLfloat)aspect;
	xmax = ymax * (GLfloat)aspect;

	glFrustum(xmin, xmax, ymin, ymax, (GLfloat)znear, (GLfloat)zfar);
#endif

	// "myglFrustum" function -----------------------------------------------------------
	//	NMATRIX mat;
	/*
		mat.f00 = (2.0f*znear)/(xmax-xmin); mat.f10 = 0.0f;						mat.f20 = (xmax+xmin)/(xmax-xmin);		mat.f30 = 0.0f;
		mat.f01 = 0.0f;						mat.f11 = (2.0f*znear)/(ymax-ymin);	mat.f21 = (ymax+ymin)/(ymax-ymin);		mat.f31 = 0.0f;
		mat.f02 = 0.0f;						mat.f12 = 0.0f;						mat.f22 = (zfar+znear)/(zfar-znear);	mat.f32 = (2.0f*zfar*znear)/(zfar-znear);
		mat.f03 = 0.0f;						mat.f13 = 0.0f;						mat.f23 = -1.0f;						mat.f33 = 1.0f;
	*/
	/*
		NGetPerspectiveMatrix(&mat);
		glMultMatrixf(&mat.f00);
	*/
	//-----------------------------------------------------------------------------------
}

void NCameraUpdatePerspective()
{

	if (ISFLAG_ON(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED))
	{
		Nf32 anglerady;
		Nf32 angleradx;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		MygluPerspective(NCamera.fFovY, NCamera.fAspectRatio, NCamera.fZNear, NCamera.fZFar);

		// PreComputing Data
		anglerady = NDEGtoRAD(NCamera.fFovY * 0.5f);
		NCamera.PreCompute_Tangente = (float)tan(anglerady);
		NCamera.PreCompute_InverseTangente = 1.0f / NCamera.PreCompute_Tangente;
		NCamera.PreCompute_SphereFactorY = (float)1.0f / cos(anglerady);
		angleradx = atan(NCamera.PreCompute_Tangente * NCamera.fAspectRatio);
		NCamera.PreCompute_SphereFactorX = (float)1.0f / cos(angleradx);

		FLAG_OFF(NCamera.Flags, FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED);
	}
}

void NCameraUpdateLookAt()
{
	if (ISFLAG_ON(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED))
	{
		// GLfloat m[4][4];
		NMATRIX mat0, mat1, mat2;

		NUpdateCameraAxis(&NCamera); // this function update Axis camera ONLY if it's necessary.

		mat0.f00 = NCamera.XAxis.x;
		mat0.f10 = NCamera.XAxis.y;
		mat0.f20 = NCamera.XAxis.z;
		mat0.f30 = 0.0f;

		mat0.f01 = NCamera.YAxis.x;
		mat0.f11 = NCamera.YAxis.y;
		mat0.f21 = NCamera.YAxis.z;
		mat0.f31 = 0.0f;

		mat0.f02 = -NCamera.ZAxis.x;
		mat0.f12 = -NCamera.ZAxis.y;
		mat0.f22 = -NCamera.ZAxis.z;
		mat0.f32 = 0.0f;

		mat0.f03 = 0.0f;
		mat0.f13 = 0.0f;
		mat0.f23 = 0.0f;
		mat0.f33 = 1.0f;

		NTranslationMatrixf(&mat1, -NCamera.Pos.x, -NCamera.Pos.y, -NCamera.Pos.z);
		NMulMatrix(&mat2, &mat1, &mat0);

		/*
		TRANSLATION MATRIX
				mat2->f00 = 1.0f;	mat2->f10 = 0.0f;	mat2->f20 = 0.0f;	mat2->f30 = tx;
				mat2->f01 = 0.0f;	mat2->f11 = 1.0f;	mat2->f21 = 0.0f;	mat2->f31 = ty;
				mat2->f02 = 0.0f;	mat2->f12 = 0.0f;	mat2->f22 = 1.0f;	mat2->f32 = tz;
				mat2->f03 = 0.0f;	mat2->f13 = 0.0f;	mat2->f23 = 0.0f;	mat2->f33 = 1.0f;

		MULMATRIX
				matresult->f00 = mat1->f00;
				matresult->f01 = mat1->f01;
				matresult->f02 = mat1->f02;
				matresult->f03 = 0.0f;

				matresult->f10 = mat1->f10;
				matresult->f11 = mat1->f11;
				matresult->f12 = mat1->f12;
				matresult->f13 = 0.0f;

				matresult->f20 = mat1->f20;
				matresult->f21 = mat1->f21;
				matresult->f22 = mat1->f22;
				matresult->f23 = 0.0f;

				matresult->f30 = mat2->f30;
				matresult->f31 = mat2->f31;
				matresult->f32 = mat2->f32;
				matresult->f33 = mat2->f33;
		*/

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat *)&mat2);
		// glTranslatef(-NCamera.Pos.x, -NCamera.Pos.y, -NCamera.Pos.z);

		FLAG_OFF(NCamera.Flags, FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED);
	}
}

NMATRIX *NGetCameraMatrix(NMATRIX *pcameramatrix)
{
	NErrorIf(!pcameramatrix, NERROR_NULL_POINTER);

	/*
		mat->f00 = 1.0f; 	mat->f10 = 0.0f;	mat->f20 = 0.0f;	 mat->f30 = 0.0f;
		mat->f01 = 0.0f;	mat->f11 = 1.0f;	mat->f21 = 0.0f;	 mat->f31 = 0.0f;
		mat->f02 = 0.0f;	mat->f12 = 0.0f;	mat->f22 = 1.0f;	 mat->f32 = 0.0f;
		mat->f03 = 0.0f;	mat->f13 = 0.0f;	mat->f23 = 0.0f;	 mat->f33 = 1.0f;
	*/

	NMATRIX mat0, mat1;
	mat0.f00 = NCamera.XAxis.x;
	mat0.f10 = NCamera.XAxis.y;
	mat0.f20 = NCamera.XAxis.z;
	mat0.f30 = 0.0f;
	mat0.f01 = NCamera.YAxis.x;
	mat0.f11 = NCamera.YAxis.y;
	mat0.f21 = NCamera.YAxis.z;
	mat0.f31 = 0.0f;
	mat0.f02 = -NCamera.ZAxis.x;
	mat0.f12 = -NCamera.ZAxis.y;
	mat0.f22 = -NCamera.ZAxis.z;
	mat0.f32 = 0.0f;
	mat0.f03 = 0.0f;
	mat0.f13 = 0.0f;
	mat0.f23 = 0.0f;
	mat0.f33 = 1.0f;

	NTranslationMatrixf(&mat1, -NCamera.Pos.x, -NCamera.Pos.y, -NCamera.Pos.z);
	NMulMatrix(pcameramatrix, &mat1, &mat0);

	return pcameramatrix;
}

/*
float NCameraCalcAspectRatioFromCurrentView()
{
	GLint	viewport[4];
	float	fratio;

	glGetIntegerv(GL_VIEWPORT, viewport);
	fratio = (float)viewport[2]/(float)viewport[3];

	// reminders
	// ---------
	// viewport[2] = ViewPort Width
	// viewport[3] = ViewPort Height

	return fratio;
}*/

Nf32 NGetZ_CameraBase(const NVEC3 *pworld_position)
{
	NVEC3 v;

	NVec3Sub(&v, pworld_position, &NCamera.Pos);
	return NVec3DotProduct(&v, &NCamera.ZAxis);
}

static inline void _UpdateViewPort_ReferenceConstants()
{
	Nf32 diagonal;
	Nf32 refdiagonal;

	switch (NViewPort.RefMode)
	{
	case NVIEWPORT_REFERENCEMODE_DIAGONAL:
		diagonal = sqrt((Nf32)(NViewPort.Width * NViewPort.Width + NViewPort.Height * NViewPort.Height));
		refdiagonal = sqrt((Nf32)(NViewPort.RefWidth * NViewPort.RefWidth + NViewPort.RefHeight * NViewPort.RefHeight));
		NViewPort.RefRatio = diagonal / refdiagonal;
		NViewPort.RefRatio_Height = diagonal / (refdiagonal * NViewPort.Height);
		NViewPort.RefRatio_Width = diagonal / (refdiagonal * NViewPort.Width);
		break;

	case NVIEWPORT_REFERENCEMODE_WIDTH:
		NViewPort.RefRatio = (Nf32)NViewPort.Width / (Nf32)NViewPort.RefWidth;
		NViewPort.RefRatio_Height = (Nf32)NViewPort.Width / ((Nf32)NViewPort.RefWidth * (Nf32)NViewPort.Height);
		NViewPort.RefRatio_Width = 1.0f / (Nf32)NViewPort.RefWidth;
		break;

	case NVIEWPORT_REFERENCEMODE_HEIGHT:
		NViewPort.RefRatio = (Nf32)NViewPort.Height / (Nf32)NViewPort.RefHeight;
		NViewPort.RefRatio_Height = 1.0f / (Nf32)NViewPort.RefHeight;
		NViewPort.RefRatio_Width = (Nf32)NViewPort.Height / ((Nf32)NViewPort.RefHeight * (Nf32)NViewPort.Width);
		break;

	case NVIEWPORT_REFERENCEMODE_AUTO:
		NErrorIf(1, NERROR_SYSTEM_TODO);
		break;

	default:
		// CRASH In debug mode
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		// ... but works like NVIEWPORT_REFERENCEMODE_DIAGONAL in release mode ...
		diagonal = sqrt((Nf32)(NViewPort.Width * NViewPort.Width + NViewPort.Height * NViewPort.Height));
		refdiagonal = sqrt((Nf32)(NViewPort.RefWidth * NViewPort.RefWidth + NViewPort.RefHeight * NViewPort.RefHeight));
		NViewPort.RefRatio = diagonal / refdiagonal;
		NViewPort.RefRatio_Height = diagonal / (refdiagonal * NViewPort.Height);
		NViewPort.RefRatio_Width = diagonal / (refdiagonal * NViewPort.Width);
		break;
	}
}

static inline void _UpdateViewPort_BasicConstants()
{
	// reminders
	// ---------
	// viewport[2] = ViewPort Width
	// viewport[3] = ViewPort Height

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

#ifdef _NWINDOWS
	NViewPort.Width = viewport[2];
	NViewPort.Height = viewport[3];
#endif

#ifdef _NANDROID
	NViewPort.Width = viewport[2];
	NViewPort.Height = viewport[3];
#endif

#ifdef _NIOS
	NViewPort.Width = viewport[2] * 2;
	!!!todo : check that it's WEIRD after all !!! take a look at CSS pixel size stackoverflow on the Web ... NViewPort.Height = viewport[3] * 2; !!!todo : check that it's WEIRD after all !!! ....
#endif

																																						   NErrorIf(!NViewPort.Width || !NViewPort.Height, NERROR_NULL_VALUE);

	NViewPort.InvWidth = 1.0f / (Nf32)NViewPort.Width;
	NViewPort.InvHeight = 1.0f / (Nf32)NViewPort.Height;

	NViewPort.WidthOutOf2 = (Nf32)NViewPort.Width / 2.0f;
	NViewPort.HeightOutOf2 = (Nf32)NViewPort.Height / 2.0f;

	NViewPort.AspectRatio = (Nf32)NViewPort.Width / (Nf32)NViewPort.Height;
	NViewPort.InvAspectRatio = (Nf32)NViewPort.Height / (Nf32)NViewPort.Width;
	// NViewPort.Diagonal			= sqrt((Nf32)(NViewPort.Width*NViewPort.Width + NViewPort.Height*NViewPort.Height));
}

void NSetViewPortReferenceMode(const Nu8 refmode)
{
	NViewPort.RefMode = refmode;
	_UpdateViewPort_ReferenceConstants();
}

void NSetViewPortReferenceScreenSize(const Nu32 ref_width, const Nu32 ref_height)
{
	NErrorIf(!ref_height || !ref_width, NERROR_NULL_VALUE);
	NViewPort.RefWidth = ref_width;
	NViewPort.RefHeight = ref_height;

	_UpdateViewPort_ReferenceConstants();
}

void NSetViewPortReferenceParams(const Nu32 ref_width, const Nu32 ref_height, const Nu8 refmode)
{
	NErrorIf(!ref_height || !ref_width, NERROR_NULL_VALUE);
	NViewPort.RefWidth = ref_width;
	NViewPort.RefHeight = ref_height;
	NViewPort.RefMode = refmode;
	_UpdateViewPort_ReferenceConstants();
}
void NInitializeViewPortConstants()
{
	_UpdateViewPort_BasicConstants();
	NSetViewPortReferenceParams(NViewPort.Width, NViewPort.Height, NVIEWPORT_REFERENCEMODE_DIAGONAL);
}
void NUpdateViewPortConstants()
{
	_UpdateViewPort_BasicConstants();
	_UpdateViewPort_ReferenceConstants();
}
