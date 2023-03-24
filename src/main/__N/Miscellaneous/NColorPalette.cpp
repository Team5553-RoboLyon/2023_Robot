// ==========================================================================================
// NColorPalette.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 01/04/2021
// Updated : 
// ------------------------------------------------------------------------------------------
// 
// ==========================================================================================
#include "../NMath.h"
#include "NColorPalette.h"

NCOLOR* NBuildColorPaletteFromRGBA(NCOLOR* pcolor_plt, const Nu32 ncolor_plt_size, const Nu32* prgba_src, const Nu32 rgba_src_size, const Nu32 ncolor_id0)
{
	NErrorIf(ncolor_id0 >= ncolor_plt_size, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(ncolor_id0 + rgba_src_size > ncolor_plt_size, NERROR_VALUE_OUTOFRANGE);

	if (!prgba_src)
		memset(&pcolor_plt[ncolor_id0], 0, sizeof(NCOLOR) * rgba_src_size);
	else
	{
		Nu32* pc = (Nu32*)prgba_src;
		for (Nu32 c = 0; c < rgba_src_size; c++, pc++)
		{
			RGBA8_To_NCOLOR(&pcolor_plt[ncolor_id0 + c], *pc);
		}
	}
	return pcolor_plt;
}

NCOLOR* NBuildColorPaletteFromColor(NCOLOR* pcolor_plt, const Nu32 ncolor_plt_size, const NCOLOR* pcol_src, const Nu32 colsrc_src_size, const Nu32 ncolor_id0)
{
	NErrorIf(ncolor_id0 >= ncolor_plt_size, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(ncolor_id0 + colsrc_src_size > ncolor_plt_size, NERROR_VALUE_OUTOFRANGE);

	if (!pcol_src)
		memset(&pcolor_plt[ncolor_id0], 0, sizeof(NCOLOR) * colsrc_src_size);
	else
		memcpy(&pcolor_plt[ncolor_id0], pcol_src, colsrc_src_size * sizeof(NCOLOR));

	return pcolor_plt;
}

Nu32* NBuildRGBAPaletteFromRGBA(Nu32* prgba_plt, const Nu32 rgba_plt_size, const Nu32* prgba_src, const Nu32 rgba_src_size, const Nu32 rgba_id0)
{
	NErrorIf(rgba_id0 >= rgba_plt_size, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(rgba_id0 + rgba_src_size > rgba_plt_size, NERROR_VALUE_OUTOFRANGE);

	if (!prgba_src)
		memset(&prgba_plt[rgba_id0], 0, sizeof(Nu32) * rgba_src_size);
	else
		memcpy(&prgba_plt[rgba_id0], prgba_src, rgba_src_size * sizeof(Nu32));
	
	return prgba_plt;
}

Nu32* NBuildRGBAPaletteFromColor(Nu32* prgba_plt, const Nu32 rgba_plt_size, const NCOLOR* pcol_src, const Nu32 colsrc_src_size, const Nu32 rgba_id0)
{
	NErrorIf(rgba_id0 >= rgba_plt_size, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(rgba_id0 + colsrc_src_size > rgba_plt_size, NERROR_VALUE_OUTOFRANGE);

	if (!pcol_src)
		memset(&prgba_plt[rgba_id0], 0, sizeof(Nu32) * colsrc_src_size);
	else
	{
		NCOLOR* pc = (NCOLOR*)pcol_src;
		for (Nu32 c = 0; c < colsrc_src_size; c++, pc++)
		{
			prgba_plt[rgba_id0 + c] = RGBA8((Nu8)(pc->red * 255.0f), (Nu8)(pc->green * 255.0f), (Nu8)(pc->blue * 255.0f), (Nu8)(pc->alpha * 255.0f));
		}
	}

	return prgba_plt;
}
// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (NCOLOR format) from a static palette
*			This function uses a PickPack set description.
*
*	@param	pcol		output color .
*	@param	pcp			src palette ( an array of NCOLOR )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*/
// ------------------------------------------------------------------------------------------
NCOLOR* NExtractModulatedColor(NCOLOR* pcol, const NCOLOR* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	NErrorIf(!pcp_size, NERROR_WRONG_SIZE);
	Nf32 scale		= (Nf32)NSTCPLT_GET_HEIGHTH(pickpack) / 8.0f;
	Nu32 id			= NSTCPLT_GET_COLID(pck_id, pickpack);
	NCOLOR* pc		= (NCOLOR*)&pcp[NMIN(id,(Nu32)(pcp_size-1))];
	pcol->red		= pc->red	* scale;
	pcol->green		= pc->green * scale;
	pcol->blue		= pc->blue	* scale;
	pcol->alpha		= NSTCPLT_IS_ALPHAMOD_ON(pickpack) ? pc->alpha * scale: pc->alpha;
	return pcol;
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (NCOLOR format) from a static palette
*			This function uses a PickPack set description.
*
*	@param	pcol		output color .
*	@param	pcp			src palette ( an array of NCOLOR )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*/
// ------------------------------------------------------------------------------------------
NCOLOR* NExtractColor(NCOLOR* pcol, const NCOLOR* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nu32 id = NSTCPLT_GET_COLID(pck_id, pickpack);
	*pcol = pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	return pcol;
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (NCOLOR format) from a static palette
*			This function uses a PickPack set description.
*
*	@param	pcol		output color .
*	@param	pcp			src palette ( an array of Nu32 )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*/
// ------------------------------------------------------------------------------------------
NCOLOR* NExtractModulatedColorFromRGBA(NCOLOR* pcol, const Nu32* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nf32 scale	= (Nf32)NSTCPLT_GET_HEIGHTH(pickpack) / (255.0f*8.0f);
	Nu32 id		= NSTCPLT_GET_COLID(pck_id, pickpack);
	Nu32 rgba	= pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	pcol->red	= (Nf32)RGBA8_R(rgba) * scale;
	pcol->green = (Nf32)RGBA8_G(rgba) * scale;
	pcol->blue	= (Nf32)RGBA8_B(rgba) * scale;
	pcol->alpha = NSTCPLT_IS_ALPHAMOD_ON(pickpack) ? (Nf32)RGBA8_A(rgba) * scale : (Nf32)RGBA8_A(rgba)/255.0f;
	return pcol;
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (NCOLOR format) from a static palette
*			This function uses a PickPack set description.
*
*	@param	pcol		output color .
*	@param	pcp			src palette ( an array of Nu32 )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*/
// ------------------------------------------------------------------------------------------
NCOLOR* NExtractColorFromRGBA(NCOLOR* pcol, const Nu32* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nu32 id		= NSTCPLT_GET_COLID(pck_id, pickpack);
	Nu32 rgba	= pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	pcol->red	= (Nf32)RGBA8_R(rgba);
	pcol->green = (Nf32)RGBA8_G(rgba);
	pcol->blue	= (Nf32)RGBA8_B(rgba);
	pcol->alpha = (Nf32)RGBA8_A(rgba);
	return pcol;
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (RGBA format) from an RGBA palette
*			This function uses a description.
*
*	@param	pcp			src palette ( an array of Nu32 )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*	@return	output RGBA (32 bits value)
*/
// ------------------------------------------------------------------------------------------
Nu32 NExtractModulatedRGBA(const Nu32* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nu32 eighth	= NSTCPLT_GET_HEIGHTH(pickpack);
	Nu32 id		= NSTCPLT_GET_COLID(pck_id, pickpack);
	Nu32 rgba	= pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	Nu8 a		= NSTCPLT_IS_ALPHAMOD_ON(pickpack) ? (Nu8)((RGBA8_A(rgba) * eighth) >> 3):RGBA8_A(rgba);
	return RGBA8((RGBA8_R(rgba) * eighth) >> 3, (RGBA8_G(rgba) * eighth) >> 3, (RGBA8_B(rgba) * eighth) >> 3, a);
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (RGBA format) from an RGBA palette
*			This function uses a description.
*
*	@param	pcp			src palette ( an array of Nu32 )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*	@return	output RGBA (32 bits value)
*/
// ------------------------------------------------------------------------------------------
Nu32 NExtractRGBA(const Nu32* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nu32 id = NSTCPLT_GET_COLID(pck_id, pickpack);
	return  pcp[NMIN(id, (Nu32)(pcp_size - 1))];
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (RGBA format) from a NCOLOR palette
*			This function uses a description.
*
*	@param	pcp			src palette ( an array of NCOLOR )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*	@return	output RGBA (32 bits value)
*/
// ------------------------------------------------------------------------------------------
Nu32 NExtractModulatedRGBAFromColor(const NCOLOR* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nf32 scale = ((Nf32)NSTCPLT_GET_HEIGHTH(pickpack)*255.0f) / 8.0f;
	Nu32 id = NSTCPLT_GET_COLID(pck_id, pickpack);
	NCOLOR* pc = (NCOLOR*)&pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	Nu8 a = NSTCPLT_IS_ALPHAMOD_ON(pickpack) ? (Nu8)(pc->alpha * scale) : (Nu8)(pc->alpha*255.0f);
	return RGBA8((Nu8)(pc->red * scale), (Nu8)(pc->green * scale), (Nu8)(pc->blue * scale), a);
}

// ------------------------------------------------------------------------------------------
/**
*	@brief	Extract a color (RGBA format) from a NCOLOR palette
*			This function uses a description.
*
*	@param	pcp			src palette ( an array of NCOLOR )
*	@param	pcp_size	src palette size
*	@param	pck_id		of the pickpack slot which contains the palette color id if desc is defined, or the color id in the palette if desc is not defined.
*	@param	pickpack	complete description of a color picking set
*	@return	output RGBA (32 bits value)
*/
// ------------------------------------------------------------------------------------------
Nu32 NExtractRGBAFromColor(const NCOLOR* pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack)
{
	Nu32 id = NSTCPLT_GET_COLID(pck_id, pickpack);
	NCOLOR* pc = (NCOLOR*)&pcp[NMIN(id, (Nu32)(pcp_size - 1))];
	return RGBA8((Nu8)(pc->red * 255.0f), (Nu8)(pc->green * 255.0f), (Nu8)(pc->blue * 255.0f), (Nu8)(pc->alpha * 255.0f));
}
