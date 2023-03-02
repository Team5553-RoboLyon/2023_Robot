// ==========================================================================================
// NColorGradient.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 31/12/2015
// Updated :
// ------------------------------------------------------------------------------------------
//
// ==========================================================================================
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
#include "../Maths/NVec2f32.h"
#include "../Maths/NVec3f32.h"
#include "NColor.h"
#include "NColorGradient.h"
#include "NColorGradient.h"

// ------------------------------------------------------------------------------------------
// NCOLOR* NExtractGradientColor
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is to consider a range of colors like a 1D,2D or 3D texture, and, trough
//	an incoming 'color position'(1D,2D or 3D according with the gradient type) pick the color at this
//	specific position in this virtual 'color texture'.
//	The color Gradient Type is used to know how to build this virtual color texture.
// ------------------------------------------------------------------------------------------
// In	:
//			NCOLOR *presult							A Valid pointer on a NCOLOR structure to store the evaluated picked color.
//			const NCOLOR *pcolorange				A Valid pointer on a list of NCOLOR structure.
//			const Nu32 colorangesize				Number of color into the list pointed by 'pcolorange'
//			const Nf32 *pcolorposition				A valid pointer on a range of Nf32 that represent the position of the color to evaluate.
//													According with the color gradient ID, range should have 1,2 or 3 Nf32.
//													into the virtual 2D color texture. This param is something like texture coordinate
//			const NCOLORGRADIENTID gradientid		A NCOLORGRADIENTID value to specify the way to use the color range.
//													( how to build the virtual 2D color texture )
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NCOLOR *NExtractGradientColor(NCOLOR *presult, const NCOLOR *pcolorange, const Nu32 colorangesize, const Nf32 *pcolorposition, const NCOLORGRADIENTID gradientid)
{
	NErrorIf(NGETCOLORGRADIENT_TYPE(gradientid) >= NCOLORGRADIENT_TYPE_ENUM_SIZE, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(NGETCOLORGRADIENT_LERP(gradientid) >= NCOLORRANGE_LERP_MODE_ENUM_SIZE, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(NGETCOLORGRADIENT_ANGLE(gradientid) >= NANGLELUT_SIZE, NERROR_VALUE_OUTOFRANGE);

	Nf32 f, r;
	Nf32 angrad;
	NVEC2 v, v1, vdir;
	NVEC3 v3;
	Nu32 a;
	NCOLOR cornercolor[8];
	NCOLOR color0, color1, colorback, colorfront;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NOTES about NORMALIZED POSITION
	// +
	// + ColorGradient is supposed to fit into a 1D,2D or 3D area, like a texture.
	// + "pcolorposition" is for Gradient what U,UV or UVW is for a texture.
	// +
	// + In order to harmonize UVs and GradientColor Coordinates UVs and Color Gradient Bases are the same
	// + with inverted Y (from top to bottom !)
	//	   0,0 ------- 1,0
	//		|           |
	//		|           |
	//		|     +     |
	//		|           |
	//		|           |
	//	   0,1 --------1,1
	//
	// + ...And W oriented to have left/top/back (0,0,0) and left/top/front(0,0,1)
	// +
	// + in certain cases we are going to keep this coordinates system, but ...
	// + in some others cases we are going to move origin to the center of the Area.
	// + (for internal calculation only, user doesn't have to deal with this.)
	// + ... And we use this opportunity to flip y coordinate from inverted "uv" way to the "conventional" way.
	// + So coordinates base becomes:
	//			   +0.5
	//	   ...................
	//	   .	    |	     .
	//	   .	    |	     .
	//	   .	    |	     .
	//	   .	    |0,0	 .
	//-0.5 .--------+--------. +0.5
	//	   .	    |	     .
	//	   .	    |	     .
	//	   .	    |	     .
	//	   .	    |	     .
	//	   ...................
	//			  -0.5

	switch (NGETCOLORGRADIENT_TYPE(gradientid))
	{
	case NCOLORGRADIENT_TYPE_1D_LINEAR:
		// Simplest way, pcolorposition is a 1D coordinate and is directly used as a Lerp factor.
		NLerpColorRange(presult, pcolorange, colorangesize, *pcolorposition, (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		return presult;

	case NCOLORGRADIENT_TYPE_2D_LINEAR:
		//	Notes: "NVec2DotProduct(&v,&vdir)/NF32_SQRT2 + 0.5" as factor for NLerpColorRange. Why ?
		//	1)  NVec2DotProduct(&v,&vdir)
		//		vdir is a normalized vector with a length of 1 : vdir(cos,-sin)
		//		v represent the normalized reference position from the center of the gradient normalized area: center(0.5,0.5)
		//		So  -sqrt(2)/2< NVec2DotProduct(&v,&vdir) < sqrt(2)/2
		//
		//	2)	NVec2DotProduct(&v,&vdir)+sqrt(2)/2
		//		sqrt(2)/2 is the radius of the external bounding circle of the square (0,0, 1,1)
		//		( 0.5  is the radius of the internal bounding circle of the same square )
		//		The dot product is the 1D coordinate of the point on the Gradient Axis. But from the center of the diameter segment.
		//		so we just perform a simple calculation to obtain this coordinate from the start point of the gradient:
		//			dot-(-sqrt(2)/2)
		//	2)	(NVec2DotProduct(&v,&vdir)+sqrt(2)/2)/sqrt(2)
		//		The "logical" way to obtain a normalized gradient Lerp ratio (0<=ratio<=1) is this one:
		//			ratio = (NVec2DotProduct(&v,&vdir)+sqrt(2)/2)/sqrt(2)
		//			which is exactly the same than
		//			ratio = NVec2DotProduct(&v,&vdir)/sqrt(2) + 0.5f
		//		In that way, all the 2D points inside the normalized square will have a normalized associated Lerp ratio
		//		like this: 0.0f <= ratio <= 1.0f

		// First of all, consider pcolorposition as a NVEC2* and change the origin ! Use the QUAD center as the new one !
		// And we take this opportunity to flip y coordinate from inverted "uv" way to the "conventional" way ...
		v.x = ((NVEC2 *)pcolorposition)->x - 0.5f;
		v.y = 0.5f - ((NVEC2 *)pcolorposition)->y; // = -(pnormposition->y - 0.5f);

		NFastVCosSin(NGETCOLORGRADIENT_ANGLE(gradientid), (Nf32 *)&vdir);
		NLerpColorRange(presult, pcolorange, colorangesize, (NVec2DotProduct(&v, &vdir) / NF32_SQRT2) + 0.5f, (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		return presult;

	case NCOLORGRADIENT_TYPE_2D_RADIAL:
		// First of all, consider pcolorposition as a NVEC2* and change the origin ! Use the QUAD center as the new one !
		// And we take this opportunity to flip y coordinate to the "conventional" way ...
		v.x = ((NVEC2 *)pcolorposition)->x - 0.5f;
		v.y = 0.5f - ((NVEC2 *)pcolorposition)->y; // = -(pnormposition->y - 0.5f);

		NLerpColorRange(presult, pcolorange, colorangesize, NVec2Length(&v) / (NF32_SQRT2_2), (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		return presult;

	case NCOLORGRADIENT_TYPE_2D_ANGULAR:
		// First of all, consider pcolorposition as a NVEC2* and change the origin ! Use the QUAD center as the new one !
		// And we take this opportunity to flip y coordinate to the "conventional" way ...
		v.x = ((NVEC2 *)pcolorposition)->x - 0.5f;
		v.y = 0.5f - ((NVEC2 *)pcolorposition)->y; // = -(pnormposition->y - 0.5f);

		if (!v.x && !v.y)
		{
			*presult = *pcolorange;
			return presult;
		}
		else
		{
			angrad = atan2(v.y, v.x) - NANGLEtoRAD(NGETCOLORGRADIENT_ANGLE(gradientid));
			while (angrad > NF32_2PI)
				angrad -= NF32_2PI;

			while (angrad < 0.0f)
				angrad += NF32_2PI;

			NLerpColorRange(presult, pcolorange, colorangesize, angrad / NF32_2PI, (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		}
		return presult;

	case NCOLORGRADIENT_TYPE_2D_DIAMOND:
		// First of all, consider pcolorposition as a NVEC2* and change the origin ! Use the QUAD center as the new one !
		// And we take this opportunity to flip y coordinate to the "conventional" way ...
		v.x = ((NVEC2 *)pcolorposition)->x - 0.5f;
		v.y = 0.5f - ((NVEC2 *)pcolorposition)->y; // = -(pnormposition->y - 0.5f);

		NLerpColorRange(presult, pcolorange, colorangesize, NMAX(NABS(v.x), NABS(v.y)) / NF32_SQRT2_2, (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		return presult;

	case NCOLORGRADIENT_TYPE_2D_REFLECTED:
		// First of all, consider pcolorposition as a NVEC2* and change the origin ! Use the QUAD center as the new one !
		// And we take this opportunity to flip y coordinate to the "conventional" way ...
		v.x = ((NVEC2 *)pcolorposition)->x - 0.5f;
		v.y = 0.5f - ((NVEC2 *)pcolorposition)->y; // = -(pnormposition->y - 0.5f);

		NFastVCosSin(NGETCOLORGRADIENT_ANGLE(gradientid), (Nf32 *)&vdir);
		f = NVec2DotProduct(&v, &vdir);
		v1.x = v.x - (f * vdir.x); // |_v becomes the vector from the incoming NormPosition to its projected point on vdir.
		v1.y = v.y - (f * vdir.y); // |
		r = NVec2Length(&v1);
		r /= NF32_SQRT2_2;
		NLerpColorRange(presult, pcolorange, colorangesize, r, (NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid));
		return presult;

	case NCOLORGRADIENT_TYPE_2D_4CORNERS:
		// First of all, consider pcolorposition as a NVEC2*.
		v = *(NVEC2 *)pcolorposition;
		// We are going to keep coordinates as they are, described in the "uv" way ...
		//
		//					    0
		//			  C...................B
		//			   .	    |	     .
		//			   .	    |	     .
		//			   .	    |	     .
		//			   .	    |0.5,0.5 .
		//  		 0 .--------+--------. +1
		//			   .	    |	     .
		//			   .	    |	     .
		//			   .	    |	     .
		//			   .	    |	     .
		//			  D...................A
		//					    +1
		//
		//		A(1,1)	B(1,0)	C(0,0)	D(0,1)
		//

		//	Extract the 4 used 'corner colors' from  'pColorrange'
		if (colorangesize == 4)
		{
			// Just to avoid useless 'NLerpColorRange' calculation in that specific case where user gives the exact number
			// of attended colors in the range.
			for (a = 0; a < 4; a++)
			{
				cornercolor[a] = pcolorange[a];
			}
		}
		else
		{
			for (a = 0; a < 4; a++)
			{
				// Use CLAMP here. And not the user choice.
				// It's because we are building CUBE COLORs here. So it's just normal !
				NLerpColorRange(&cornercolor[a], pcolorange, colorangesize, (Nf32)a / 3.0f, NCOLORRANGE_LERP_CLAMP);
			}
		}

		// SPECIFIC LERP, So Self application of LERP MODE ...
		switch ((NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid))
		{
		case NCOLORRANGE_LERP_CLAMP:
			v.x = NCLAMP(0.0f, v.x, 1.0f);
			v.y = NCLAMP(0.0f, v.y, 1.0f);
			break;

		case NCOLORRANGE_LERP_REPEAT:
			v.x = v.x - floor(v.x);
			v.y = v.y - floor(v.y);
			break;
		}

		// X
		NColorLerp(&color0, &cornercolor[2], &cornercolor[1], v.x); // CB
		NColorLerp(&color1, &cornercolor[3], &cornercolor[0], v.x); // DA
		// Y
		NColorLerp(presult, &color0, &color1, v.y); //[CB][DA]
		return presult;

	case NCOLORGRADIENT_TYPE_3D_CUBE:
		// First of all, consider pcolorposition as a NVEC3*.
		v3 = *(NVEC3 *)pcolorposition;
		// We are going to keep coordinates as they are, described in the "uvw" way ...
		//
		// THE CUBE:											BACK and FRONT Face.		    	BOTTOM and TOP Faces
		//
		//						B
		//		C_________------+------_____							    0								    0
		//		+----___	G	. _____-----+F					C/G...................B/F			D/C...................A/B
		//		|		----+-----			|					   .	    |	     .				   .	    |	     .
		//		|			|	.			|					   .	    |	     . 				   .	    |	     .
		//		|			|	.			|					   .	    |	     .				   .	    |	     .
		//		|			|	.			|					   .	    |0.5,0.5 .				   .	    |0.5,0.5 .
		//		|			|	.			|				     0 .--------+--------. +1			 0 .--------+--------. +1
		//		|			|	.			|					   .	    |	     .				   .	    |	     .
		//		|			|	.			|					   .	    |	     . 				   .	    |	     .
		//		|	     ...|...+A.....		|					   .	    |	     .				   .	    |	     .
		//		|........	|		   .....|					   .	    |	     . 				   .	    |	     .
		//	   D+----___	|	  _____-----+E					D/H...................A/E			H/G...................E/F
		//				----+-----										    +1								    +1
		//					H
		//
		//		Back:	A,B,C,D
		//		A(1,1,0)	B(1,0,0)	C(0,0,0)	D(0,1,0)
		//
		//		Front:	E,F,G,H
		//		E(1,1,1)	F(1,0,1)	G(0,0,1)	H(0,1,1)
		//
		//		Extract the 8 used 'corner colors' from  'pColorrange'
		if (colorangesize == 8)
		{
			// Just to avoid useless 'NLerpColorRange' calculation in that specific case where user gives the exact number
			// of attended colors in the range.
			for (a = 0; a < 8; a++)
			{
				cornercolor[a] = pcolorange[a];
			}
		}
		else
		{
			for (a = 0; a < 8; a++)
			{
				// Use CLAMP here. And not the user choice.
				// It's because we are building CUBE COLORs here. So it's just normal !
				NLerpColorRange(&cornercolor[a], pcolorange, colorangesize, (Nf32)a / 7.0f, NCOLORRANGE_LERP_CLAMP);
			}
		}

		// SPECIFIC LERP, So Self application of LERP MODE ...
		switch ((NCOLORRANGE_LERP_MODE_ENUM)NGETCOLORGRADIENT_LERP(gradientid))
		{
		case NCOLORRANGE_LERP_CLAMP:
			v3.x = NCLAMP(0.0f, v3.x, 1.0f);
			v3.y = NCLAMP(0.0f, v3.y, 1.0f);
			v3.z = NCLAMP(0.0f, v3.z, 1.0f);
			break;

		case NCOLORRANGE_LERP_REPEAT:
			v3.x = v3.x - floor(v3.x);
			v3.y = v3.y - floor(v3.y);
			v3.z = v3.z - floor(v3.z);
			break;
		}

		// BACK FACE
		// X
		NColorLerp(&color0, &cornercolor[2], &cornercolor[1], v3.x); // CB
		NColorLerp(&color1, &cornercolor[3], &cornercolor[0], v3.x); // DA
		// Y
		NColorLerp(&colorback, &color0, &color1, v3.y); //[CB][DA]
		// FRONT FACE
		// X
		NColorLerp(&color0, &cornercolor[6], &cornercolor[5], v3.x); // GF
		NColorLerp(&color1, &cornercolor[7], &cornercolor[4], v3.x); // HE
		// Y
		NColorLerp(&colorfront, &color0, &color1, v3.y); //[GF][HE]

		// FINAL RESULT including Z
		NColorLerp(presult, &colorback, &colorfront, v3.z);
		return presult;

	default:
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		*presult = *pcolorange;
		return presult;
	}
}
