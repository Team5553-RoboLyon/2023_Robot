#ifndef __NSCREENPOINT_H
#define __NSCREENPOINT_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
// a 2D point 16 bit version
// used to describe point in screen coord system (pixel)
typedef struct
{
	union
	{
		struct 
		{
			Ns16	x;
			Ns16	y;
		};

		Ns16 coord[2];
	};
}NVEC2s16;

// a 2D point 32 bit version
// used to describe point in screen coord system (pixel)
typedef struct
{
	union
	{
		struct 
		{
			Ns32	x;
			Ns32	y;
		};

		Ns32 coord[2];
	};
}NVEC2s32;


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSCREENPOINT_H 

