#ifndef __NRECTF32_H
#define __NRECTF32_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NRectf32.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Maths/NVec2f32.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// a 2D rect float 32 bit version
	// used to describe a rectangle in floating coord system
	typedef struct
	{
		union
		{
			struct
			{
				Nf32 left;
				Nf32 top;
				Nf32 right;
				Nf32 bottom;
			};

			struct
			{
				NVEC2 upper_left;
				NVEC2 lower_right;
			};
			Nf32 coord[4];
		};
	} NRECTf32;

	inline void NSetRectf32(NRECTf32 *pr, const NVEC2 *pupper_left, const NVEC2 *plower_right)
	{
		pr->upper_left = *pupper_left;
		pr->lower_right = *plower_right;
	}
	inline void NSetRectf32f(NRECTf32 *pr, const Nf32 left, const Nf32 top, const Nf32 right, const Nf32 bottom)
	{
		pr->left = left;
		pr->top = top;
		pr->right = right;
		pr->bottom = bottom;
	}
	inline Nbool NRectf32Equality(const NRECTf32 *pr0, const NRECTf32 *pr1)
	{
		if (pr0->left != pr1->left)
			return NFALSE;
		if (pr0->right != pr1->right)
			return NFALSE;
		if (pr0->top != pr1->top)
			return NFALSE;
		if (pr0->bottom != pr1->bottom)
			return NFALSE;
		return NTRUE;
	}
	inline void NRectf32Reorder(NRECTf32 *pr)
	{
		Nf32 t;
		if (pr->left > pr->right)
		{
			t = pr->left;
			pr->left = pr->right;
			pr->right = t;
		}
		if (pr->bottom > pr->top)
		{
			t = pr->bottom;
			pr->bottom = pr->top;
			pr->top = t;
		}
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NRECTF32_H
