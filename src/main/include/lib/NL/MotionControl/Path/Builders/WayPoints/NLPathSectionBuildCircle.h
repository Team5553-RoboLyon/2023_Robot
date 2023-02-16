#pragma once
#include "../../../../../N/NType.h"
#include "../../../../../N/Maths/NVec2f32.h"

typedef struct
{
	NVEC2			o;				// centre du cercle
	Nf32			r;				// rayon du cercle
	Ns32			sgn;			// Signe ou sens de parcour du cercle.
}NLPATH_SECTION_BCIRCLE;
