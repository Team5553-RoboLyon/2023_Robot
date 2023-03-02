#ifndef __NWINCOORDSYSTEMCONVERSION_H
#define __NWINCOORDSYSTEMCONVERSION_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NCoordinateSystemConversions.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVec3f32.h"

#include "lib/N/NErrorHandling.h"
#include "lib/N/NViewport.h"
#include "lib/N/NScreenPoint.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	//
	//		HRel
	//
	//	Relative to Screen Height.			HRel is what N is waiting for
	//	Origin is at left top screen		interface building
	//	corner. With Y Axis oriented
	//	to top ! ( so to bottom Y values
	//	are negative )
	//
	//	0----------> + Width/Height
	//	|
	//	|
	//	|
	//	|
	//	|
	//	V
	//	- 1.0f
	//
	//
	//		Pix									fPix									Norm
	//
	//	In pixel. Origin is at				In (Nf32)pixel. Origin is at				Normalized coordinates. from 0 to 1 for X and 0 to -1 for Y
	//	the left top screen corner			the left top screen corner					Origin is at left top screen
	//	With Y axis oriented to				With Y axis oriented to						corner. With Y Axis oriented
	//	bottom !!!							TOP !!!										to top ! ( so to bottom Y values
	//										(fPix are float Pixel)						are negative )
	//
	//	0----------> + screen Width			0----------> + screen Width					0----------> + 1.0f
	//	|			  ( in pixel )			|			  ( in (Nf32)pixel )			|
	//	|									|											|
	//	|									|											|
	//	|									|											|
	//	|									|											|
	//	V									V											V
	//	+	screen height					-	screen height							- 1.0f
	//		( in pixel )						( in (Nf32)pixel )
	//
	inline Nf32 NCoord_PixToHRel_X(const Ns32 w) { return (Nf32)w * NViewPort.InvHeight; }
	inline Nf32 NCoord_PixToHRel_Y(const Ns32 h) { return (Nf32)(-h) * NViewPort.InvHeight; }
	inline NVEC2 *NCoord_PixToHRel(NVEC2 *phrel, const NVEC2s16 *pscr)
	{
		phrel->x = ((Nf32)pscr->x) * NViewPort.InvHeight;
		phrel->y = ((Nf32)-pscr->y) * NViewPort.InvHeight;
		return phrel;
	}

	inline Ns32 NCoord_HRelToPix_X(const Nf32 w) { return (Ns32)(w * (Nf32)NViewPort.Height); }
	inline Ns32 NCoord_HRelToPix_Y(const Nf32 h) { return (Ns32)(-h * (Nf32)NViewPort.Height); }
	inline NVEC2s16 *NCoord_HRelToPix(NVEC2s16 *pscr, const NVEC2 *phrel)
	{
		pscr->x = (Ns16)(phrel->x * (Nf32)NViewPort.Height);
		pscr->y = (Ns16)(-phrel->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NCoord_NormToHRel_X(const Nf32 w) { return w * NViewPort.AspectRatio; }
	inline Nf32 NCoord_NormToHRel_Y(const Nf32 h) { return h; }
	inline NVEC2 *NCoord_NormToHRel(NVEC2 *phrel, const NVEC2 *pnorm)
	{
		phrel->x = pnorm->x * NViewPort.AspectRatio;
		phrel->y = pnorm->y;
		return phrel;
	}

	inline Nf32 NCoord_HRelToNorm_X(const Nf32 w) { return w * NViewPort.InvAspectRatio; }
	inline Nf32 NCoord_HRelToNorm_Y(const Nf32 h) { return h; }
	inline NVEC2 *NCoord_HRelToNorm(NVEC2 *pnorm, const NVEC2 *phrel)
	{
		pnorm->x = phrel->x * NViewPort.InvAspectRatio;
		pnorm->y = phrel->y;
		return pnorm;
	}

	inline Ns32 NCoord_NormToPix_X(const Nf32 w) { return (Ns32)(w * (Nf32)NViewPort.Width); }
	inline Ns32 NCoord_NormToPix_Y(const Nf32 h) { return (Ns32)(-h * (Nf32)NViewPort.Height); }
	inline NVEC2s16 *NCoord_NormToPix(NVEC2s16 *pscr, const NVEC2 *pnorm)
	{
		pscr->x = (Ns16)(pnorm->x * (Nf32)NViewPort.Width);
		pscr->y = (Ns16)(-pnorm->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NCoord_PixToNorm_X(const Ns32 w) { return (Nf32)w * NViewPort.InvWidth; }
	inline Nf32 NCoord_PixToNorm_Y(const Ns32 h) { return (Nf32)-h * NViewPort.InvHeight; }
	inline NVEC2 *NCoord_PixToNorm(NVEC2 *pnorm, const NVEC2s16 *pscr)
	{
		pnorm->x = ((Nf32)pscr->x) * NViewPort.InvWidth;
		pnorm->y = ((Nf32)-pscr->y) * NViewPort.InvHeight;
		return pnorm;
	}

	inline Nf32 NCoord_fPixToHRel_X(const Nf32 w) { return w * NViewPort.InvHeight; }
	inline Nf32 NCoord_fPixToHRel_Y(const Nf32 h) { return h * NViewPort.InvHeight; }
	inline NVEC2 *NCoord_fPixToHRel(NVEC2 *phrel, const NVEC2 *pscr)
	{
		phrel->x = pscr->x * NViewPort.InvHeight;
		phrel->y = pscr->y * NViewPort.InvHeight;
		return phrel;
	}

	inline Nf32 NCoord_HRelTofPix_X(const Nf32 w) { return (w * (Nf32)NViewPort.Height); }
	inline Nf32 NCoord_HRelTofPix_Y(const Nf32 h) { return (h * (Nf32)NViewPort.Height); }
	inline NVEC2 *NCoord_HRelTofPix(NVEC2 *pscr, const NVEC2 *phrel)
	{
		pscr->x = (phrel->x * (Nf32)NViewPort.Height);
		pscr->y = (phrel->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NCoord_NormTofPix_X(const Nf32 w) { return (w * (Nf32)NViewPort.Width); }
	inline Nf32 NCoord_NormTofPix_Y(const Nf32 h) { return (h * (Nf32)NViewPort.Height); }
	inline NVEC2 *NCoord_NormTofPix(NVEC2 *pscr, const NVEC2 *pnorm)
	{
		pscr->x = (pnorm->x * (Nf32)NViewPort.Width);
		pscr->y = (pnorm->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NCoord_fPixToNorm_X(const Nf32 w) { return w * NViewPort.InvWidth; }
	inline Nf32 NCoord_fPixToNorm_Y(const Nf32 h) { return h * NViewPort.InvHeight; }
	inline NVEC2 *NCoord_fPixToNorm(NVEC2 *pnorm, const NVEC2 *pscr)
	{
		pnorm->x = (pscr->x) * NViewPort.InvWidth;
		pnorm->y = (pscr->y) * NViewPort.InvHeight;
		return pnorm;
	}

	inline Ns32 NCoord_fPixToPix_X(const Nf32 w) { return (Ns32)w; }
	inline Ns32 NCoord_fPixToPix_Y(const Nf32 h) { return -(Ns32)h; }
	inline NVEC2s16 *NCoord_fPixToPix(NVEC2s16 *pscr, const NVEC2 *pfscr)
	{
		pscr->x = (Ns16)pfscr->x;
		pscr->y = -(Ns16)(pfscr->y);
		return pscr;
	}

	inline Nf32 NCoord_PixTofPix_X(const Ns32 w) { return (Nf32)w; }
	inline Nf32 NCoord_PixTofPix_Y(const Ns32 h) { return -(Nf32)h; }
	inline NVEC2 *NCoord_PixTofPix(NVEC2 *pfscr, const NVEC2s16 *pscr)
	{
		pfscr->x = (Nf32)pscr->x;
		pfscr->y = -(Nf32)pscr->y;
		return pfscr;
	}

	// ++
	inline Nf32 NSize_PixToHRel_W(const Ns32 w) { return (Nf32)w * NViewPort.InvHeight; }
	inline Nf32 NSize_PixToHRel_H(const Ns32 h) { return (Nf32)h * NViewPort.InvHeight; }
	inline NVEC2 *NSize_PixToHRel(NVEC2 *phrel, const NVEC2s16 *pscr)
	{
		phrel->x = ((Nf32)pscr->x) * NViewPort.InvHeight;
		phrel->y = ((Nf32)pscr->y) * NViewPort.InvHeight;
		return phrel;
	}

	inline Ns32 NSize_HRelToPix_W(const Nf32 w) { return (Ns32)(w * (Nf32)NViewPort.Height); }
	inline Ns32 NSize_HRelToPix_H(const Nf32 h) { return (Ns32)(h * (Nf32)NViewPort.Height); }
	inline NVEC2s16 *NSize_HRelToPix(NVEC2s16 *pscr, const NVEC2 *phrel)
	{
		pscr->x = (Ns16)(phrel->x * (Nf32)NViewPort.Height);
		pscr->y = (Ns16)(phrel->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NSize_NormToHRel_W(const Nf32 w) { return w * NViewPort.AspectRatio; }
	inline Nf32 NSize_NormToHRel_H(const Nf32 h) { return h; }
	inline NVEC2 *NSize_NormToHRel(NVEC2 *phrel, const NVEC2 *pnorm)
	{
		phrel->x = pnorm->x * NViewPort.AspectRatio;
		phrel->y = pnorm->y;
		return phrel;
	}

	inline Nf32 NSize_HRelToNorm_W(const Nf32 w) { return w * NViewPort.InvAspectRatio; }
	inline Nf32 NSize_HRelToNorm_H(const Nf32 h) { return h; }
	inline NVEC2 *NSize_HRelToNorm(NVEC2 *pnorm, const NVEC2 *phrel)
	{
		pnorm->x = phrel->x * NViewPort.InvAspectRatio;
		pnorm->y = phrel->y;
		return pnorm;
	}

	inline Ns32 NSize_NormToPix_W(const Nf32 w) { return (Ns32)(w * (Nf32)NViewPort.Width); }
	inline Ns32 NSize_NormToPix_H(const Nf32 h) { return (Ns32)(h * (Nf32)NViewPort.Height); }
	inline NVEC2s16 *NSize_NormToPix(NVEC2s16 *pscr, const NVEC2 *pnorm)
	{
		pscr->x = (Ns16)(pnorm->x * (Nf32)NViewPort.Width);
		pscr->y = (Ns16)(pnorm->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NSize_PixToNorm_W(const Ns32 w) { return (Nf32)w * NViewPort.InvWidth; }
	inline Nf32 NSize_PixToNorm_H(const Ns32 h) { return (Nf32)h * NViewPort.InvHeight; }
	inline NVEC2 *NSize_PixToNorm(NVEC2 *pnorm, const NVEC2s16 *pscr)
	{
		pnorm->x = ((Nf32)pscr->x) * NViewPort.InvWidth;
		pnorm->y = ((Nf32)pscr->y) * NViewPort.InvHeight;
		return pnorm;
	}

	inline Nf32 NSize_fPixToHRel_W(const Nf32 w) { return w * NViewPort.InvHeight; }
	inline Nf32 NSize_fPixToHRel_H(const Nf32 h) { return h * NViewPort.InvHeight; }
	inline NVEC2 *NSize_fPixToHRel(NVEC2 *phrel, const NVEC2 *pscr)
	{
		phrel->x = (pscr->x) * NViewPort.InvHeight;
		phrel->y = (pscr->y) * NViewPort.InvHeight;
		return phrel;
	}

	inline Nf32 NSize_HRelTofPix_W(const Nf32 w) { return (w * (Nf32)NViewPort.Height); }
	inline Nf32 NSize_HRelTofPix_H(const Nf32 h) { return (h * (Nf32)NViewPort.Height); }
	inline NVEC2 *NSize_HRelTofPix(NVEC2 *pscr, const NVEC2 *phrel)
	{
		pscr->x = (phrel->x * (Nf32)NViewPort.Height);
		pscr->y = (phrel->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NSize_NormTofPix_W(const Nf32 w) { return (w * (Nf32)NViewPort.Width); }
	inline Nf32 NSize_NormTofPix_H(const Nf32 h) { return (h * (Nf32)NViewPort.Height); }
	inline NVEC2 *NSize_NormTofPix(NVEC2 *pscr, const NVEC2 *pnorm)
	{
		pscr->x = (pnorm->x * (Nf32)NViewPort.Width);
		pscr->y = (pnorm->y * (Nf32)NViewPort.Height);
		return pscr;
	}

	inline Nf32 NSize_fPixToNorm_W(const Nf32 w) { return w * NViewPort.InvWidth; }
	inline Nf32 NSize_fPixToNorm_H(const Nf32 h) { return h * NViewPort.InvHeight; }
	inline NVEC2 *NSize_fPixToNorm(NVEC2 *pnorm, const NVEC2 *pscr)
	{
		pnorm->x = (pscr->x) * NViewPort.InvWidth;
		pnorm->y = (pscr->y) * NViewPort.InvHeight;
		return pnorm;
	}

	inline Ns32 NSize_fPixToPix_W(const Nf32 w) { return (Ns32)w; }
	inline Ns32 NSize_fPixToPix_H(const Nf32 h) { return (Ns32)h; }
	inline NVEC2s16 *NSize_fPixToPix(NVEC2s16 *pscr, const NVEC2 *pfscr)
	{
		pscr->x = (Ns16)pfscr->x;
		pscr->y = (Ns16)(pfscr->y);
		return pscr;
	}

	inline Nf32 NSize_PixTofPix_W(const Ns32 w) { return (Nf32)w; }
	inline Nf32 NSize_PixTofPix_H(const Ns32 h) { return (Nf32)h; }
	inline NVEC2 *NSize_PixTofPix(NVEC2 *pfscr, const NVEC2s16 *pscr)
	{
		pfscr->x = (Nf32)pscr->x;
		pfscr->y = (Nf32)pscr->y;
		return pfscr;
	}

	// + -- +
	// RefPix -> HRel
	// from "Pixel" relative to reference screen size to current Viewport Height Relative coordinate system.
	inline Nf32 NCoord_RefPixToHRel_X(const Ns32 x) { return (Nf32)x * NViewPort.RefRatio_Height; }
	inline Nf32 NCoord_RefPixToHRel_Y(const Ns32 y) { return (Nf32)-y * NViewPort.RefRatio_Height; }
	inline NVEC2 *NCoord_RefPixToHRel(NVEC2 *phrel, const NVEC2s16 *pscr)
	{
		phrel->x = ((Nf32)pscr->x) * NViewPort.RefRatio_Height;
		phrel->y = ((Nf32)-pscr->y) * NViewPort.RefRatio_Height;
		return phrel;
	}

	inline Nf32 NCoord_RefNormToHRel_X(const Nf32 x) { return x * (Nf32)NViewPort.RefWidth * NViewPort.RefRatio_Height; }
	inline Nf32 NCoord_RefNormToHRel_Y(const Nf32 y) { return y * (Nf32)NViewPort.RefHeight * NViewPort.RefRatio_Height; }
	inline NVEC2 *NCoord_RefNormToHRel(NVEC2 *phrel, const NVEC2 *pnorm)
	{
		phrel->x = pnorm->x * (Nf32)NViewPort.RefWidth * NViewPort.RefRatio_Height;
		phrel->y = pnorm->y * (Nf32)NViewPort.RefHeight * NViewPort.RefRatio_Height;
		return phrel;
	}

	inline Nf32 NSize_RefPixTofPix_W(const Ns32 w) { return (Nf32)w * NViewPort.RefRatio; }
	inline Nf32 NSize_RefPixTofPix_H(const Ns32 h) { return (Nf32)h * NViewPort.RefRatio; }
	inline NVEC2 *NSize_RefPixTofPix(NVEC2 *pfpix, const NVEC2s16 *pscr)
	{
		pfpix->x = ((Nf32)pscr->x) * NViewPort.RefRatio;
		pfpix->y = ((Nf32)pscr->y) * NViewPort.RefRatio;
		return pfpix;
	}

	inline Nf32 NSize_RefPixToHRel_W(const Ns32 w) { return (Nf32)w * NViewPort.RefRatio_Height; }
	inline Nf32 NSize_RefPixToHRel_H(const Ns32 h) { return (Nf32)h * NViewPort.RefRatio_Height; }
	inline NVEC2 *NSize_RefPixToHRel(NVEC2 *phrel, const NVEC2s16 *pscr)
	{
		phrel->x = ((Nf32)pscr->x) * NViewPort.RefRatio_Height;
		phrel->y = ((Nf32)pscr->y) * NViewPort.RefRatio_Height;
		return phrel;
	}

	inline Nf32 NSize_RefNormToHRel_W(const Nf32 w) { return w * (Nf32)NViewPort.RefWidth * NViewPort.RefRatio_Height; }
	inline Nf32 NSize_RefNormToHRel_H(const Nf32 h) { return h * (Nf32)NViewPort.RefHeight * NViewPort.RefRatio_Height; }
	inline NVEC2 *NSize_RefNormToHRel(NVEC2 *phrel, const NVEC2 *pnorm)
	{
		phrel->x = pnorm->x * (Nf32)NViewPort.RefWidth * NViewPort.RefRatio_Height;
		phrel->y = pnorm->y * (Nf32)NViewPort.RefHeight * NViewPort.RefRatio_Height;
		return phrel;
	}

	inline Nf32 NSize_RefPixToNorm_W(const Ns32 w) { return (Nf32)w * NViewPort.RefRatio_Width; }
	inline Nf32 NSize_RefPixToNorm_H(const Ns32 h) { return (Nf32)h * NViewPort.RefRatio_Height; }
	inline NVEC2 *NSize_RefPixToNorm(NVEC2 *pnorm, const NVEC2s16 *pscr)
	{
		pnorm->x = ((Nf32)pscr->x) * NViewPort.RefRatio_Width;
		pnorm->y = ((Nf32)pscr->y) * NViewPort.RefRatio_Height;
		return pnorm;
	}

/*
inline void NCoord_fPixFit(NVEC2 *pfpix, const NVEC2 *pfpixsrc,const Nf32 reference_diagonal)
{
	// The reference Aspect Ratio must be defined !
	NErrorIf(reference_diagonale== 0.0f, NERROR_NULL_VALUE);

	Nf32 f = reference_diagonal/NViewPort.Diagonal;

	pfpix->x = pfpixsrc->x
}*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NWINCOORDSYSTEMCONVERSION_H
