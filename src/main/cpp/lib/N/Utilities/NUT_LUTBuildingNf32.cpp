//#include "../NCStandard.h"

#include "../NErrorHandling.h"
#include "NUT_LUT.h"

/*
NUT_Serief_Constant_Push( Nf32 *pfirst, const Nu32 f_nb,const Nu32 byte_stride_to_next, const Nf32 constant );
NUT_Serief_Constant_Add( Nf32 *pfirst, const Nu32 f_nb,const Nu32 byte_stride_to_next, const Nf32 constant );
NUT_Serief_Constant_scale( Nf32 *pfirst, const Nu32 f_nb,const Nu32 byte_stride_to_next, const Nf32 constant );
NUT_Serief_Constant_Blend( Nf32 *pfirst, const Nu32 f_nb,const Nu32 byte_stride_to_next, const Nf32 constant );
*/

void NUT_LUTBuild_Nf32FromConstant(	Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 constantvalue )
{
	Nu32					i;
	Nf32					*pf;

	NErrorIf(!stride,NERROR_NULL_VALUE);

	pf = plut;
	for( i=element_nb;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		switch( operand )
		{
			case NUT_LUTBUILD_OPERAND_PUSH:
				*pf = constantvalue;
				break;

			case NUT_LUTBUILD_OPERAND_ADD:
				*pf += constantvalue;
				break;

			case NUT_LUTBUILD_OPERAND_SCALE:
				*pf *= constantvalue;
				break;

			case NUT_LUTBUILD_OPERAND_SUB:
				*pf -= constantvalue;
				break;

			case NUT_LUTBUILD_OPERAND_AVERAGE:
				*pf = (*pf+constantvalue)*0.5f;
				break;

			default:
				break;
		}
	}
}

void NUT_LUTBuild_Nf32FromFastRand(	Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 scale )
{
	Nu32					i;
	Nf32					*pf;
	Nf32					val;

	NErrorIf(!stride,NERROR_NULL_VALUE);

	pf = plut;
	for( i=element_nb;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		val = NFastRand()*scale;
		
		switch( operand )
		{
			case NUT_LUTBUILD_OPERAND_PUSH:
				*pf = val;
				break;

			case NUT_LUTBUILD_OPERAND_ADD:
				*pf += val;
				break;

			case NUT_LUTBUILD_OPERAND_SCALE:
				*pf *= val;
				break;

			case NUT_LUTBUILD_OPERAND_SUB:
				*pf -= val;
				break;

			case NUT_LUTBUILD_OPERAND_AVERAGE:
				*pf = (*pf+val)*0.5f;
				break;

			default:
				break;
		}
	}
}

void NUT_LUTBuild_Nf32FromFastRandEx(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 scale,const Nu32 key_every )
{
	Nu32		i;
	Nf32		*pf;
	Nf32		val,vala,valb,valmore,valstep;
	Nu32		k;	
	
	NErrorIf(!stride,NERROR_NULL_VALUE);
	NErrorIf(key_every<2,NERROR_UNAUTHORIZED_REQUEST); // Use the primary function 'NUT_LUTBuild_Nf32FromFastRand' to fill up a table !

	k = 0;
	valb	= NFastRand()*scale;
	valstep = 1.0f/(Nf32)key_every;

	pf = plut;
	for( i=element_nb;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		if(!k)
		{
			k=key_every-1;
			val = vala	= valb;
			valb	= NFastRand()*scale;
			valmore = (valb-vala)*valstep;
		}
		else
		{
			val += valmore;
			k--;
		}

		switch( operand )
		{
			case NUT_LUTBUILD_OPERAND_PUSH:
				*pf = val;
				break;

			case NUT_LUTBUILD_OPERAND_ADD:
				*pf += val;
				break;

			case NUT_LUTBUILD_OPERAND_SCALE:
				*pf *= val;
				break;

			case NUT_LUTBUILD_OPERAND_SUB:
				*pf -= val;
				break;

			case NUT_LUTBUILD_OPERAND_AVERAGE:
				*pf = (*pf+val)*0.5f;
				break;

			default:
				break;
		}
	}
}


// ------------------------------------------------------------------------------------------
// void NUT_LUTComputing_Nf32FromSpline
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate and store Nf32 values From a Spline.
// ------------------------------------------------------------------------------------------
// In	:
//
//			operand				How to  introduce each value into the LUT.
//								It could be a simple push, or an addition, or a scale , etc ...
//			plut				a valid pointer on a Nf32 table. 
//			element_nb			Number of values to calculate.
//								( notice that's the LUT contains at least 'element_nb' elements,
//								  and, in that case, each element will have a size of one Nf32 )
//			stride				number of BYTES between 2 Nf32 values. 
//								It's the size of one element of the LUT ( for example 'sizeof(NVEC3)')
//								if stride==0 function considers that there is sizeof(Nf32) BYTES between 2 calculated values.
//								Thanks to 'stride' value, it's possible to create a LUT of NVEC3 for example ( 1 NVECTOR = 3 Nf32 )
//								... and calculate values for X only. "stride" value helps us to pass from a value to th next one. 
//			valuemin/valuemax	Nf32 values are going to be calculated from the incoming spline.
//								First value will be the Y coordinate of the first point of the spline.
//								Last value will be the Y coordinate of the last point of the spline.
//								All the other values are going to be calculated along the spline, between first and last point.
//								Thanks to the 'mode' selected, all these values can be "restricted" into limits (valuemin and valuemax)  
//			mode				3 possible modes
//									NUT_LUTCOMPUTING_MODE_CLAMP			Calculated values are going to be "limited" by valuemin and valuemax.
//																		if(value<valuemin)value = valuemin or if(value>valuemax)value = valuemax 
//													
//									NUT_LUTCOMPUTING_MODE_WRAP			Calculated values are going to be "stretched" between the 2 limits.
//																		after processing, the smallest value will be valuemin and the biggest will be value max
//
//									NUT_LUTCOMPUTING_MODE_NOLIMITS		No process applied. So values are going to be y coordinates of spline points. 
//
//			pspline				A valid pointer on a spline. 
//
// Out : -
//
//
// ------------------------------------------------------------------------------------------
void NUT_LUTBuild_Nf32FromSpline(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, NSPLINE *pspline, const Nf32 scale )
{
	Nu32							i;
	Nf32							*pf;
	Nf32							step;
	NVEC2						C;
	NUT_2DSPLINE_PERSISTENT_XRESULT	persistent;
	Nf32							val;

	NErrorIf(!stride,NERROR_NULL_VALUE);
	NErrorIf(!pspline,NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistent,pspline);

	if(element_nb > 1)
		step = 1.0f/(Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pf	= plut;
	C.x	= 0.0f;
	C.y = 0.0f;
	val = 0.0f;
	for( i=0; i<element_nb; i++, C.x = (Nf32)i*step, pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		if( NUT_2DVerticalLineXSplineExtended( &C, &persistent ) )
			val = scale * persistent.XResult.I.y;

		switch( operand )
		{
			case NUT_LUTBUILD_OPERAND_PUSH:
				*pf = val;
				break;

			case NUT_LUTBUILD_OPERAND_ADD:
				*pf += val;
				break;

			case NUT_LUTBUILD_OPERAND_SCALE:
				*pf *= val;
				break;

			case NUT_LUTBUILD_OPERAND_SUB:
				*pf -= val;
				break;

			case NUT_LUTBUILD_OPERAND_AVERAGE:
				*pf = (*pf+val)*0.5f;
				break;

			default:
				break;
		}
	}
}


void NUT_LUTBuild_Nf32Clamp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax )
{
	Nu32	i;
	Nf32	*pf;
	
	NErrorIf(!stride,NERROR_NULL_VALUE);

	pf = plut;
	for( i=element_nb; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		if(*pf<newmin)
			*pf=newmin;
		else if(*pf>newmax)
			*pf=newmax;
	}
}

void NUT_LUTBuild_Nf32Warp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax )
{
	Nu32	i;
	Nf32	*pf;
	Nf32	curmin,curmax;
	Nf32	ratio;

	NErrorIf(!stride,NERROR_NULL_VALUE);

	// Find LUT current min and max
	pf	= plut;
	curmin = curmax = *pf;
	for( i=element_nb;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		if(*pf<curmin)
			curmin = *pf;
		else if(*pf>curmax)
			curmax = *pf;
	}

	// incoming min/max values constraint
	pf	= plut;
	ratio = (newmax-newmin)/(curmax-curmin);
	for( i=element_nb;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		*pf = newmin + (*pf-curmin)*ratio;
	}
}

void NUT_LUTBuild_Nf32Smooth( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb )
{
	Nu32	i;
	Nf32	*pf;
	Nf32	inv_coefsum;
	Nf32	*pdata,*pd,*prevpd;
	Nu16	p;
	Nf32	tmp;

	NErrorIf(!stride,NERROR_NULL_VALUE);

	pdata = (Nf32*)Nmalloc( sizeof(Nf32)*(element_nb+2*range) );
	inv_coefsum = 1.0f/( (Nf32)range*2.0f+1.0f );
	
	for(p=0;p<pass_nb;p++)
	{
		pf = plut;
		pd = pdata;
		// All divided in one pass !
		// 0) The first
		*pd = tmp = *pf*inv_coefsum; 
		prevpd = pd;
		pd ++;
		// 1) The left "out of range" part ! 
		for(i=range-1;i!=0;i--,pd++,prevpd++)
		{
			*pd = *prevpd + tmp;	
		}
		// 2) The main part ! 
		for( i=element_nb; i!=0; i--, pd++, prevpd++,pf=(Nf32*)( (NBYTE*)pf+stride ) )
		{
			*pd = *prevpd + *pf*inv_coefsum;
		}
		// 3) The right "out of range" part ! 
		tmp = *prevpd - *(prevpd-1);
		for(i=range;i!=0;i--,pd++,prevpd++)
		{
			*pd = *prevpd + tmp;	
		}
		// 4)Final pass: start !
		pf		= plut;
		prevpd  = pdata;
		pd		= &pdata[2*range];

		*pf		= *pd;

		// 5)Final pass, loop !
		pf=(Nf32*)( (NBYTE*)pf+stride );
		pd++;
		for( i=element_nb-1;i!=0;i--, pd++, prevpd++, pf=(Nf32*)( (NBYTE*)pf+stride ) )
		{
			*pf = *pd - *prevpd;
		}

	}

	Nfree(pdata);
}

void NUT_LUTBuild_Nf32Lerp(	Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 valuea, const Nf32 valueb )
{
	NErrorIf(!stride,NERROR_NULL_VALUE);
	NErrorIf(!element_nb,NERROR_NULL_VALUE);

	Nu32					i;
	Nf32					*pf;
	Nf32					t;

	if( element_nb < 2 )
	{
		t = 0.0f;
	}
	else
	{
		t = 1.0f/(Nf32)(element_nb-1);
	}

	pf = plut;
	for( i=0;i<element_nb;i++,pf=(Nf32*)( (NBYTE*)pf+stride ) )
	{
		switch( operand )
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			*pf = valuea + (valueb-valuea)*((Nf32)i*t);
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			*pf += valuea + (valueb-valuea)*((Nf32)i*t);
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			*pf *= valuea + (valueb-valuea)*((Nf32)i*t);
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			*pf -= valuea + (valueb-valuea)*((Nf32)i*t);
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			*pf = (*pf + valuea+(valueb-valuea)*((Nf32)i*t) )*0.5f;
			break;

		default:
			break;
		}
	}
}
