#ifndef __NCAMERA_H
#define __NCAMERA_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NCamera.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "NFlags.h"
#include "./Maths/NVec3f32.h"
#include "NMatrix.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
#define FLAG_NCAMERA_LOOKAT_HASTOBEUPDATED			BIT_0
#define FLAG_NCAMERA_AXIS_HASTOBEUPDATED			BIT_1
#define FLAG_NCAMERA_PERSPECTIVE_HASTOBEUPDATED		BIT_2

typedef struct 
{
	Nu32		Flags;						// Camera Flags. Mainly used to know if recalculations are necessary
	NVEC3	Pos;						// Camera "EYE" position
	NVEC3	TargetPos;					// Position of the point targeted by the camera
	NVEC3	Up;							// Vertical axis Direction Given to the camera

	NVEC3	XAxis;
	NVEC3	YAxis;
	NVEC3	ZAxis;

	Nf32		fFovY;						// Camera FOVY (DEG). Specifies the field of view angle, in degrees, in the y direction.
	Nf32		fAspectRatio;				// Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).	
	Nf32		fZNear;						// Specifies the distance from the viewer to the near clipping plane (always positive).
	Nf32		fZFar;						// Specifies the distance from the viewer to the far clipping plane (always positive).

	Nf32		PreCompute_Tangente;		// Pre-computed Data used for precise FRUSTRUM Culling and other things ...
	Nf32		PreCompute_InverseTangente;	// Pre-computed Data 
	Nf32		PreCompute_SphereFactorX;	// Pre-computed Data used for precise FRUSTRUM Culling and other things ...
	Nf32		PreCompute_SphereFactorY;	// Pre-computed Data used for precise FRUSTRUM Culling and other things ...

}NCAMERA;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************

void		NCameraUpdatePerspective();
void		NCameraUpdateLookAt();

NCAMERA*	NGetCamera();
void		NCameraSetUp(const NCAMERA* cam_draft);

void		NMoveCameraPosition(const NVEC3 *v);
void		NMoveCameraPositionf(const Nf32 x,const Nf32 y, const Nf32 z);
void		NMoveCameraTargetPosition(const NVEC3 *v);
void		NMoveCameraTargetPositionf(const Nf32 x,const Nf32 y, const Nf32 z);

void		NSetCameraPosition(const NVEC3 *v);
void		NSetCameraPositionf(const Nf32 x,const Nf32 y, const Nf32 z);
void		NSetCameraPositionXf(const Nf32 x);
void		NSetCameraPositionYf(const Nf32 y);
void		NSetCameraPositionZf(const Nf32 z);

void		NSetCameraTargetPosition(const NVEC3 *v);
void		NSetCameraTargetPositionf(const Nf32 x,const Nf32 y,const  Nf32 z);
void		NSetCameraTargetPositionXf(const Nf32 x);
void		NSetCameraTargetPositionYf(const Nf32 y);
void		NSetCameraTargetPositionZf(const Nf32 z);

void		NCameraSetVectorUp(const NVEC3 *v);
void		NCameraSetVectorUpf(const Nf32 x,const Nf32 y, const Nf32 z);
void		NCameraSetFov(const Nf32 fov);
void		NCameraSetAspectRatio(const Nf32 fratio);
void		NCameraSetZNear(const Nf32 fnear);
void		NCameraSetZFar(const Nf32 far);

// LOW LEVEL
NMATRIX*	NGetCameraMatrix(NMATRIX *pcameramatrix);
NMATRIX*	NGetPerspectiveMatrix(NMATRIX *pmatrix);
Nf32		NGetZ_CameraBase(const NVEC3 *pworld_position);

// Private functions
inline void NUpdateCameraAxis(NCAMERA *pcamera)
{
	if(ISFLAG_ON(pcamera->Flags,FLAG_NCAMERA_AXIS_HASTOBEUPDATED))
	{
		pcamera->ZAxis.x = pcamera->TargetPos.x - pcamera->Pos.x;
		pcamera->ZAxis.y = pcamera->TargetPos.y - pcamera->Pos.y;
		pcamera->ZAxis.z = pcamera->TargetPos.z - pcamera->Pos.z;
		NVec3Normalize(&pcamera->ZAxis);
		// Side = forward x up
		// cross(forward, up, side);
		NVec3CrossProduct(&pcamera->XAxis,&pcamera->ZAxis,&pcamera->Up);
		NVec3Normalize(&pcamera->XAxis);
		// Recompute up as: up = side x forward
		//cross(side, forward, up);
		NVec3CrossProduct(&pcamera->YAxis,&pcamera->XAxis,&pcamera->ZAxis);


		FLAG_OFF(pcamera->Flags,FLAG_NCAMERA_AXIS_HASTOBEUPDATED);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NCAMERA_H 

