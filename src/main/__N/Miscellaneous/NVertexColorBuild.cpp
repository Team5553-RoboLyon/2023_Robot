#include "../Core/NSafeConversion.h"
#include "NVertexColorBuild.h"

NBUILDBASE_COLOR* NSetupBuildBaseColor(NBUILDBASE_COLOR *pbbcolor, const Nu16 flags, const Nu8 colorsetid, const NCOLOR *pcolorrange, const Nu32 colorangesize, const NCOLORGRADIENTID colorgradientid )
{
	NErrorIf( ( flags& (MASK_NBUILDBASE_COLOR_ALL_FLAGS) )!=flags, NERROR_INCONSISTENT_FLAGS );

	Nmem0(pbbcolor,NBUILDBASE_COLOR);
	pbbcolor->Flags = flags;

	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_COLOR_USEPARAM_COLORSETID) )
	{
		NErrorIf(colorsetid > CONSTANT_NBUILDBASE_COLOR_COLORSETID_MAX, NERROR_VALUE_OUTOFRANGE);
		pbbcolor->ColorSetID = colorsetid;
	}
	
	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE) )
	{
		NErrorIf(!pcolorrange, NERROR_NULL_POINTER);
		NErrorIf(!colorangesize, NERROR_NULL_VALUE);

		pbbcolor->pColorRange		= (NCOLOR*)pcolorrange;
		pbbcolor->ColorRangeSize	= colorangesize;
	}

	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_COLOR_USEPARAM_COLORGRADIENTID) )
	{
		NErrorIf(NGETCOLORGRADIENT_TYPE(colorgradientid)>= NCOLORGRADIENT_TYPE_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
		NErrorIf(NGETCOLORGRADIENT_LERP(colorgradientid)>= NCOLORRANGE_LERP_MODE_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
		NErrorIf(NGETCOLORGRADIENT_ANGLE(colorgradientid)>= NANGLELUT_SIZE,NERROR_VALUE_OUTOFRANGE);
		pbbcolor->ColorGradientID	= colorgradientid;
	}
	return pbbcolor;
}

NBUILDBASE_COLOR* NCreateBuildBaseColor(const Nu16 flags, const Nu8 colorsetid, const NCOLOR *pcolorrange, const Nu32 colorangesize, const NCOLORGRADIENTID colorgradientid )
{
	return NSetupBuildBaseColor(NEW(NBUILDBASE_COLOR),flags,colorsetid,pcolorrange,colorangesize,colorgradientid);
}

void NClearBuildBaseColor(NBUILDBASE_COLOR *pbbcolor)
{
	Nmem0(pbbcolor,NBUILDBASE_COLOR);
}

void NDeleteBuildBaseColor(NBUILDBASE_COLOR *pbbcolor)
{
	NClearBuildBaseColor(pbbcolor);
	Nfree(pbbcolor);
}

Nu8	NGetBuildBaseColorSetID(const NBUILDBASE_COLOR *pbbcolor )
{
	if( ISFLAG_ON(pbbcolor->Flags,FLAG_NBUILDBASE_COLOR_USEPARAM_COLORSETID) )
	{
		NErrorIf(pbbcolor->ColorSetID>CONSTANT_NBUILDBASE_COLOR_COLORSETID_MAX, NERROR_VALUE_OUTOFRANGE);
		return pbbcolor->ColorSetID;
	}
	else
	{
		return DEFAULT_NBUILDBASE_COLOR_COLORSETID;
	}
}

// ------------------------------------------------------------------------------------------
// NCOLOR* NBuildVertexColor
// ------------------------------------------------------------------------------------------
// Description :
//	Extract a color from common building parameters.
// ------------------------------------------------------------------------------------------
// In	:
//			NCOLOR *pcolor_result						A valid pointer on a NCOLOR structure which will receive result
//			const Nu32 vertexrange_index				Index of the vertex in it's range base. It will be used to define Lerp ratio if the flag 
//														FLAG_NVCOLBUILD_USEPARAM_COLORGRADIENTID is set to OFF by user. In that case, the function is going to 
//														evaluate a result color trough a LERP call with a lerp ratio of (vertexindex)/(vertexrange_size-1).
//			const Nu32 vertexrange_size					Size of the Vertex range.
//			NCOLOR *pcolor_result						A Valid pointer on a NCOLOR structure to store the evaluated picked color.
//			const NCOLOR *pcolorange					A Valid pointer on a list of NCOLOR structure.
//			const Nu32 colorangesize					Number of color into the list pointed by 'pcolorange'
//			const Nf32 *pcolorposition					A valid pointer on a range of Nf32 that represent the position of the color to evaluate.
//														According with the color gradient ID, range should have 1,2 or 3 Nf32.
//														into the virtual 2D color texture. This param is something like texture coordinate
//			const NCOLORGRADIENTID colorgradientid		A NCOLORGRADIENTID value to specify the way to use the color range.
//														( how to build the virtual 2D color texture )

// 
// Out :
//			return the incoming NCOLOR* pcolor_result parameter.
// ------------------------------------------------------------------------------------------
NCOLOR* NGetBuildBaseColorVertexColor(NCOLOR *pcolor_result, const Nu32 vertexrange_index, const Nu32 vertexrange_size, const Nf32 *pcolorposition, const NBUILDBASE_COLOR *pvcolbuild )
{
	NErrorIf(!pvcolbuild, NERROR_NULL_POINTER);
	NErrorIf( (pvcolbuild->Flags & MASK_NBUILDBASE_COLOR_ALL_FLAGS) != pvcolbuild->Flags, NERROR_INCONSISTENT_FLAGS);
	NErrorIf( (pvcolbuild->Flags == 0)  && (pvcolbuild->pColorRange || pvcolbuild->ColorRangeSize), NERROR_INCONSISTENT_PARAMETERS);

	// Used Color range as a Color Gradient.
	if( ISFLAG_ON(pvcolbuild->Flags,FLAG_NBUILDBASE_COLOR_USEPARAM_COLORGRADIENTID) )
	{
		return NExtractGradientColor(pcolor_result,pvcolbuild->pColorRange,pvcolbuild->ColorRangeSize,pcolorposition,pvcolbuild->ColorGradientID);
	}
	else if( ISFLAG_ON(pvcolbuild->Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE) ) // Used Color range as .. a simple color range.
	{
		NErrorIf(!pvcolbuild->pColorRange,NERROR_NULL_POINTER);
		NErrorIf(!pvcolbuild->ColorRangeSize,NERROR_NULL_VALUE);
		NErrorIf(vertexrange_index>=vertexrange_size,NERROR_INDEX_OUTOFRANGE);
		
		if(vertexrange_size == 1 )
		{
			// In that Specific Case, evaluate the "middle" point of the Color range as the result color.
			NLerpColorRange(pcolor_result,pvcolbuild->pColorRange,pvcolbuild->ColorRangeSize,0.5f,NCOLORRANGE_LERP_CLAMP);
		}
		else
		{
			// In the case of a Simple color range using. Extracting the result with a ratio based on the vertex index in its range.
			NLerpColorRange(pcolor_result,pvcolbuild->pColorRange,pvcolbuild->ColorRangeSize,(Nf32)vertexrange_index/(vertexrange_size-1),NCOLORRANGE_LERP_CLAMP);
		}
		return pcolor_result;
	}
	else
	{
		// Default case: Using default color ... a simple WHITE
		NSetColorf(pcolor_result,NCOLOR_PRESET3F_WHITE,1.0f);
		return pcolor_result;
	}
}


