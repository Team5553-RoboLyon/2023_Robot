#ifndef __NSCREENRECT_H
#define __NSCREENRECT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NScreenRect.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NScreenPoint.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// a 2D rect 16 bit version
	// used to describe a rectangle in screen coord system (pixel)
	typedef struct
	{
		union
		{
			struct
			{
				Ns16 left;
				Ns16 top;
				Ns16 right;
				Ns16 bottom;
			};

			struct
			{
				NVEC2s16 upper_left;
				NVEC2s16 lower_right;
			};
			Ns16 coord[4];
		};
	} NRECTs16;

	// a 2D rect 32 bit version
	// used to describe a rectangle in screen coord system (pixel)
	typedef struct
	{
		union
		{
			struct
			{
				Ns32 left;
				Ns32 top;
				Ns32 right;
				Ns32 bottom;
			};

			struct
			{
				NVEC2s32 upper_left;
				NVEC2s32 lower_right;
			};
			Ns32 coord[4];
		};
	} NRECTs32;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NSCREENRECT_H
