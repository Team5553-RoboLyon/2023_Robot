#ifndef __NUSUAL_COORDINATES_H
#define __NUSUAL_COORDINATES_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUsualCoordinates.h								**
// **																					**
// ***************************************************************************************

// Usual 3D coordinates names which are working with NVEC3.coord[]
typedef enum
{
	_I=0,
	_J,
	_K,
	_NV3ORTHONORMAL_ENUM_SIZE
}NV3ORTHONORMAL_ENUM;

typedef enum
{
	_X=0,
	_Y,
	_Z,
	NV3COORD_ENUM_SIZE
}NV3COORD_ENUM;

typedef enum
{
	_WIDTH = 0,
	_HEIGHT,
	_DEPTH,
	NV3SIZE_ENUM_SIZE
}NV3SIZE_ENUM;

typedef enum
{
	_PLANE_XY = 0,
	_PLANE_XZ,
	_PLANE_ZY,
	NAAPLANE_ENUM_SIZE
}NAAPLANE_ENUM;

// ORIENTATION
typedef enum
{
	_LEFT  = 0,
	_RIGHT,
	_FRONT,
	_BACK,
	_TOP,
	_BOTTOM,
	NORIENTATION_ENUM_SIZE
}NORIENTATION_ENUM;

// SPHERIC
typedef enum
{
	_RADIUS=0,
	_INCLINATION,
	_AZIMUTH,
	NSPHERIC_COORD_ENUM_SIZE
}NSPHERIC_COORD_ENUM;

// ORTHOGONAL AXIS NUMBER (for 1D, 2D and 3D)
#define _1DAXISNB	1	// _X		or	_I
#define _2DAXISNB	2	// _X,_Y	or	_I,_J
#define _3DAXISNB	3	// _X,_Y,_Z	or	_I,_J,_K

// ANCHORING
// Change ANCHORING to have ANCHOR_BOTTOM == 0.0f ??? and all positive ? maybe !!!!
// It makes sens in 3D ... lets do that a future day , but it will be a good idea to do !!!
#define ANCHOR_LEFT				0.0f
#define ANCHOR_MIDWIDTH			0.5f
#define ANCHOR_RIGHT			1.0f
#define ANCHOR_TOP				0.0f
#define ANCHOR_MIDHEIGHT		-0.5f
#define ANCHOR_BOTTOM			-1.0f
#define ANCHOR_BACK				0.0f
#define ANCHOR_MIDDEPTH			0.5f
#define ANCHOR_FRONT			1.0f

// WORLD RIGHT HAND ORTHONORMAL BASE
//
#define _WORLD_XAXIS_3f			1.0f,0.0f,0.0f	
#define _WORLD_YAXIS_3f			0.0f,1.0f,0.0f
#define _WORLD_ZAXIS_3f			0.0f,0.0f,1.0f

#define _WORLD_SIDE_3f			_WORLD_XAXIS_3f
#define _WORLD_UP_3f			_WORLD_YAXIS_3f
#define _WORLD_FORWARD_3f		_WORLD_ZAXIS_3f	

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUSUAL_COORDINATES_H 

