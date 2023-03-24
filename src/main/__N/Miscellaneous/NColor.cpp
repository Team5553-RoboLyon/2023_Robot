// ==========================================================================================
// NColor.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 06/10/2012
// Updated : 
// ------------------------------------------------------------------------------------------
// 
// ==========================================================================================
#include "../NCStandard.h"
#include "../NType.h"

#include "../Maths/NVec4f32.h"
#include "../Miscellaneous/NColor.h"
#include "../NErrorHandling.h"


NCOLOR* NLerpColorRange(NCOLOR *presult, const NCOLOR *prange, const Nu32 range_size, const Nf32 ratio, const NCOLORRANGE_LERP_MODE_ENUM mode )
{
	NErrorIf(!range_size, NERROR_NULL_VALUE);
	
	if(range_size == 1)
	{
		*presult = *prange;
		return presult;
	}

	Nf32 f;
	switch(mode)
	{
		case NCOLORRANGE_LERP_CLAMP:
			f = NCLAMP(0.0f,ratio,1.0f);
			break;
		case NCOLORRANGE_LERP_REPEAT:
			f = ratio-floor(ratio);
			break;
	}
	Nf32	fid	= f*(Nf32)(range_size-1);
	Nu32	id	= (Nu32)fid;
	fid		   -= (Nf32)id;
	NColorLerp(presult,&prange[id],&prange[id+1],fid);
	// Note about the case: f == 1.0f
	// When f == 1.0f we will have fid == (Nf32)(range_size-1)
	// ... and so, id == range_size-1
	// ... and so, id+1 == range_size !!! which is OUT OF RANGE !!!
	// so, why don't use a kind of test like this :
	//			NErrorIf(id+1>=range_size,NERROR_INDEX_OUTOFRANGE);
	// ... and why are we not going to manage the specific case of f == 1.0f in a specific way ???
	// ...
	// Because it's not necessary. When f == 1.0f, sure id = range_size-1 and id+1 == range_size
	// but we also have fid = 0.0f ( "id" is the rational part and "fid" is the fractional part of "f*(Nf32)(range_size-1)" )
	// so, if prange[id+1] is going to read "nowhere", the LERP algorithm is going to 
	// multiply these uninitialized numbers from nowhere by ZERO !!!
	// ... at the end of the day, the result will be correct:
	//
	//							presult = prange[range_size-1] when f ==1.0f !!! 
	//
	return presult;
}
// ------------------------------------------------------------------------------------------
// NCopyColorRange
// ------------------------------------------------------------------------------------------
// Description :
//		Copy a ColorRange to an another one. Source and destination can have a different size.
//		Function adapts the destination range as better as possible to keep the same visual aspect
//		despite the possible different number of colors.
// ------------------------------------------------------------------------------------------
// In  :
//			pdst_range		a valid pointer on a simple range of NCOLOR elements. This range is the destination one.
//			dst_rangesize	size of the color range ( == number of NCOLOR elements )
//			psrc_range		a valid pointer on a simple range of NCOLOR elements. This Range is the source one.
//			src_rangesize	size of the color range ( == number of NCOLOR elements )
// Out :
//			NCOLOR*			incoming "pdst_range" pointer
// ------------------------------------------------------------------------------------------
NCOLOR* NCopyColorRange( NCOLOR *pdst_range, const Nu32 dst_rangesize, const NCOLOR *psrc_range, const Nu32 src_rangesize )
{
	NErrorIf( !pdst_range		, NERROR_NULL_VALUE );
	NErrorIf( !psrc_range		, NERROR_NULL_VALUE );
	NErrorIf( dst_rangesize	== 0, NERROR_NULL_VALUE );
	NErrorIf( src_rangesize	== 0, NERROR_NULL_VALUE );

	// Special cases
	if(dst_rangesize==1)
	{
		*pdst_range = *psrc_range;
		return pdst_range;
	}
	else if(dst_rangesize == src_rangesize)
	{
		memcpy(pdst_range,psrc_range,sizeof(NCOLOR)*dst_rangesize);
		return pdst_range;
	}
	else
	{
		// Ratio
		Nf32 r = (Nf32)(src_rangesize-1)/(Nf32)(dst_rangesize-1);
		Nf32 fid;
		Nu32 id;
		for(Nu32 i=0;i<dst_rangesize;i++)
		{
			fid	= (Nf32)i*r;
			id	= (Nu32)fid;
			if(id==src_rangesize-1)
			{
				pdst_range[i] = psrc_range[id]; 
			}
			else
			{
				NErrorIf(id>src_rangesize-1, NERROR_INDEX_OUTOFRANGE); // id should be strictly less than (src_rangesize-1)

				fid-= (Nf32)id;
				NColorLerp(&pdst_range[i],&psrc_range[id],&psrc_range[id+1],fid);
			}
		}
		return pdst_range;
	}
}
// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In  :
//			pcolor_range	a valid pointer on a simple range of NCOLOR éléments
//			size			size of the color range ( == number of NCOLOR elements )
//			colornbmax		Maximum number of color created by the Gradient process, in the range at the end of the day ( colorA and colorB included )
//			pcolorA			a valid pointer on the first color of the Gradient.
//			pcolorB			a valid pointer on the last color of the Gradient.
// Out :
//			NCOLOR*			incoming "pcolor_range" pointer
// ------------------------------------------------------------------------------------------
NCOLOR* NSetColorRangeGradient(NCOLOR *pcolor_range,const Nu32 size, const Nu32 colornbmax,const NCOLOR *pcolorA,const NCOLOR *pcolorB)
{
	Nu32	i,j,slot;
	NCOLOR	*pr;

	if(colornbmax>=size)
	{
		pr = pcolor_range;
		for(i=0;i<size;i++)
		{
			NVec4Lerp( (NVEC4*)pr,(const NVEC4*)pcolorA,(const NVEC4*)pcolorB,(Nf32)i/(Nf32)(size-1) );
			pr++;
		}
	}
	else
	{
		slot = size/colornbmax;

		pr = pcolor_range;
		for(i=0;i<colornbmax;i++)
		{
			for(j=0;j<slot;j++)
			{
				NVec4Lerp( (NVEC4*)pr,(const NVEC4*)pcolorA,(const NVEC4*)pcolorB,(Nf32)i/(Nf32)(colornbmax-1) );
				pr++;
			}
		}
		
		// rest ?
		i = size - slot*colornbmax;
		if(i)
        {
			NSetColorRange( pr,i,pr-1 );
        }
	}
	return pcolor_range;
}

// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In  :
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
NHSV* NCOLOR_To_NHSV( NHSV *phsv, const NCOLOR *pcolor )
{
	Nf32 var_min = NMIN( NMIN( pcolor->red, pcolor->green ), pcolor->blue );    //Min. value of RGB
	Nf32 var_max = NMAX( NMAX( pcolor->red, pcolor->green ), pcolor->blue );    //Max. value of RGB
	Nf32 del_max = var_max - var_min;             //Delta RGB value 

	phsv->Brightness = var_max;

	if ( del_max == 0.0f )      //This is a gray, no chroma...
	{
		phsv->Hue		= 0.0f;	//HSV results from 0 to 1
		phsv->Saturation= 0.0f;
	}
	else //Chromatic data...
	{
		phsv->Saturation = del_max / var_max;

		Nf32 del_red	= ( ( ( var_max - pcolor->red		) / 6.0f ) + ( del_max * 0.5f ) ) / del_max;
		Nf32 del_green	= ( ( ( var_max - pcolor->green		) / 6.0f ) + ( del_max * 0.5f ) ) / del_max;
		Nf32 del_blue	= ( ( ( var_max - pcolor->blue		) / 6.0f ) + ( del_max * 0.5f ) ) / del_max;

		if( pcolor->red == var_max )
		{
			phsv->Hue = del_blue - del_green;
		}
		else if( pcolor->green == var_max )
		{
			phsv->Hue = ( 1.0f / 3.0f ) + del_red - del_blue;
		}
		else// if ( prgb->blue == var_Max )
		{
			phsv->Hue = ( 2.0f / 3.0f ) + del_green - del_red;
		}
		if ( phsv->Hue < 0.0f )
		{
			phsv->Hue += 1.0f;
		}

		if ( phsv->Hue > 1.0f )
		{
			phsv->Hue -= 1.0f;
		}
	}
	return phsv;
}

NCOLOR* NHSV_To_NCOLOR( NCOLOR *pcolor, const NHSV *phsv, const Nf32 alpha )
{
	if( phsv->Saturation == 0.0f )
	{
		pcolor->red		= phsv->Brightness;
		pcolor->green	= phsv->Brightness;
		pcolor->blue	= phsv->Brightness;
	}
	else
	{
		Nf32 var_h = phsv->Hue * 6.0f;
		
		if ( var_h >= 6.0f ) 
		{
			var_h = 0.0f;     //H must be < 1
		}

		Nu32 var_i = (Nu32)floor( var_h );
		Nf32 var_1 = phsv->Brightness * ( 1.0f - phsv->Saturation );
		Nf32 var_2 = phsv->Brightness * ( 1.0f - phsv->Saturation * ( var_h - (Nf32)var_i ) );
		Nf32 var_3 = phsv->Brightness * ( 1.0f - phsv->Saturation * ( 1.0f - ( var_h - (Nf32)var_i ) ) );

		switch(var_i)
		{
			case 0:
				pcolor->red		= phsv->Brightness;
				pcolor->green	= var_3;
				pcolor->blue	= var_1; 
				break;

			case 1:
				pcolor->red		= var_2;
				pcolor->green	= phsv->Brightness;
				pcolor->blue	= var_1; 
				break;
		
			case 2:
				pcolor->red		= var_1;
				pcolor->green	= phsv->Brightness;
				pcolor->blue	= var_3; 
				break;

			case 3:
				pcolor->red		= var_1;
				pcolor->green	= var_2;
				pcolor->blue	= phsv->Brightness; 
				break;

			case 4:
				pcolor->red		= var_3;
				pcolor->green	= var_1;
				pcolor->blue	= phsv->Brightness; 
				break;

			case 5:
				pcolor->red		= phsv->Brightness;
				pcolor->green	= var_1;
				pcolor->blue	= var_2; 
				break;
		}
	}

	// alpha
	 if(alpha>=0.0f)
	 {
		 pcolor->alpha = alpha;
	 }

	 return pcolor;
}

// ------------------------------------------------------------------------------------------
// NColorConverter
// ------------------------------------------------------------------------------------------
// Description :
//	
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NCOLOR* NColorConverter(NCOLOR *pcolor, const void *psrc,const NCOLOR_FORMAT_ENUM src_format)
{
	switch(src_format)
	{
		case _COLOR_FORMAT_NCOLOR:
			*pcolor = *((NCOLOR*)psrc);
			return pcolor;

		case _COLOR_FORMAT_RGBA8:
			return RGBA8_To_NCOLOR( pcolor,*((Nu32*)psrc) );

		case _COLOR_FORMAT_RGB8:
			return RGB8_To_NCOLOR( pcolor,*((Nu32*)psrc),1.0f );

		case _COLOR_FORMAT_RGB5:
			return RGB5_To_NCOLOR( pcolor,*((Nu32*)psrc),1.0f );

		case _COLOR_FORMAT_GRAYA8:
			return GRAYA8_To_NCOLOR( pcolor,*((Nu32*)psrc) );

		case _COLOR_FORMAT_NHSV:
			return NHSV_To_NCOLOR( pcolor,(NHSV*)psrc,1.0f );

		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			return pcolor;
	}
}