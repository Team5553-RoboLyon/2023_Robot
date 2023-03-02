// #include "lib/N/NCStandard.h"

#include "lib/N/NErrorHandling.h"
#include "NUT_LUT.h"

void NUT_LUTBuild_NColorLerp(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pcola, const NCOLOR *pcolb)
{
	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!element_nb, NERROR_NULL_VALUE);

	Nu32 i;
	NCOLOR *pc;
	NCOLOR c;
	Nf32 t;

	if (element_nb < 2)
	{
		t = 0.0f;
	}
	else
	{
		t = 1.0f / (Nf32)(element_nb - 1);
	}

	pc = plut;
	for (i = 0; i < element_nb; i++, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			NColorLerp(pc, pcola, pcolb, (Nf32)i * t);
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			NColorLerp(&c, pcola, pcolb, (Nf32)i * t);
			pc->red += c.red;
			pc->green += c.green;
			pc->blue += c.blue;
			pc->alpha += c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			NColorLerp(&c, pcola, pcolb, (Nf32)i * t);
			pc->red *= c.red;
			pc->green *= c.green;
			pc->blue *= c.blue;
			pc->alpha *= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			NColorLerp(&c, pcola, pcolb, (Nf32)i * t);
			pc->red -= c.red;
			pc->green -= c.green;
			pc->blue -= c.blue;
			pc->alpha -= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			NColorLerp(&c, pcola, pcolb, (Nf32)i * t);
			pc->red = (pc->red + c.red) * 0.5f;
			pc->green = (pc->green + c.green) * 0.5f;
			pc->blue = (pc->blue + c.blue) * 0.5f;
			pc->alpha = (pc->alpha + c.alpha) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromConstantf(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 red, const Nf32 green, const Nf32 blue, const Nf32 alpha)
{
	Nu32 i;
	NCOLOR *pc;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX(0.0f,NMIN(red,1.0f)	);
			// 				pc->green	= NMAX(0.0f,NMIN(green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(alpha,1.0f)	);

			pc->red = red;
			pc->green = green;
			pc->blue = blue;
			pc->alpha = alpha;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red + red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green + green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue + blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha + alpha,1.0f)	);

			pc->red += red;
			pc->green += green;
			pc->blue += blue;
			pc->alpha += alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red * red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green * green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue * blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha * alpha,1.0f)	);

			pc->red *= red;
			pc->green *= green;
			pc->blue *= blue;
			pc->alpha *= alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red - red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green - green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue - blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha - alpha,1.0f)	);

			pc->red -= red;
			pc->green -= green;
			pc->blue -= blue;
			pc->alpha -= alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		=NMAX(0.0f,NMIN((pc->red + red)*0.5f,1.0f)		);
			// 				pc->green	=NMAX(0.0f,NMIN((pc->green + green)*0.5f,1.0f)	);
			// 				pc->blue	=NMAX(0.0f,NMIN((pc->blue + blue)*0.5f,1.0f)		);
			// 				pc->alpha	=NMAX(0.0f,NMIN((pc->alpha + alpha)*0.5f,1.0f)	);

			pc->red = (pc->red + red) * 0.5f;
			pc->green = (pc->green + green) * 0.5f;
			pc->blue = (pc->blue + blue) * 0.5f;
			pc->alpha = (pc->alpha + alpha) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromConstant(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pcolor)
{
	Nu32 i;
	NCOLOR *pc;

	NErrorIf(!stride, NERROR_NULL_VALUE);

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX( 0.0f,NMIN(pcolor->red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pcolor->green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pcolor->blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pcolor->alpha,1.0f)	);

			pc->red = pcolor->red;
			pc->green = pcolor->green;
			pc->blue = pcolor->blue;
			pc->alpha = pcolor->alpha;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red + pcolor->red,1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green + pcolor->green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue + pcolor->blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha + pcolor->alpha,1.0f)	);

			pc->red += pcolor->red;
			pc->green += pcolor->green;
			pc->blue += pcolor->blue;
			pc->alpha += pcolor->alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red*pcolor->red,1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green*pcolor->green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue*pcolor->blue,1.0f)		);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha*pcolor->alpha,1.0f)	);

			pc->red *= pcolor->red;
			pc->green *= pcolor->green;
			pc->blue *= pcolor->blue;
			pc->alpha *= pcolor->alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red - pcolor->red,1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green - pcolor->green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue - pcolor->blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha - pcolor->alpha,1.0f)	);

			pc->red -= pcolor->red;
			pc->green -= pcolor->green;
			pc->blue -= pcolor->blue;
			pc->alpha -= pcolor->alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		= NMAX(0.0f, NMIN( (pc->red	+ pcolor->red)*0.5f		,1.0f)	);
			// 				pc->green	= NMAX(0.0f, NMIN( (pc->green + pcolor->green)*0.5f	,1.0f)	);
			// 				pc->blue	= NMAX(0.0f, NMIN( (pc->blue + pcolor->blue)*0.5f		,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f, NMIN( (pc->alpha + pcolor->alpha)*0.5f	,1.0f)	);

			pc->red = (pc->red + pcolor->red) * 0.5f;
			pc->green = (pc->green + pcolor->green) * 0.5f;
			pc->blue = (pc->blue + pcolor->blue) * 0.5f;
			pc->alpha = (pc->alpha + pcolor->alpha) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromFastRand(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pscale)
{
	Nu32 i;
	NCOLOR *pc;

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX( 0.0f,NMIN(pscale->red*NFastRand(),1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pscale->green*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pscale->blue*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pscale->alpha*NFastRand(),1.0f)	);

			pc->red = pscale->red * NFastRand();
			pc->green = pscale->green * NFastRand();
			pc->blue = pscale->blue * NFastRand();
			pc->alpha = pscale->alpha * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red + pscale->red*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green + pscale->green*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue + pscale->blue*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha + pscale->alpha*NFastRand(),1.0f)	);
			pc->red += pscale->red * NFastRand();
			pc->green += pscale->green * NFastRand();
			pc->blue += pscale->blue * NFastRand();
			pc->alpha += pscale->alpha * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red*pscale->red*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green*pscale->green*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue*pscale->blue*NFastRand(),1.0f)		);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha*pscale->alpha*NFastRand(),1.0f)	);
			pc->red *= pscale->red * NFastRand();
			pc->green *= pscale->green * NFastRand();
			pc->blue *= pscale->blue * NFastRand();
			pc->alpha *= pscale->alpha * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red - pscale->red*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green - pscale->green*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue - pscale->blue*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha - pscale->alpha*NFastRand(),1.0f)	);
			pc->red -= pscale->red * NFastRand();
			pc->green -= pscale->green * NFastRand();
			pc->blue -= pscale->blue * NFastRand();
			pc->alpha -= pscale->alpha * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		= NMAX(0.0f, NMIN( (pc->red	+ pscale->red*NFastRand())*0.5f		,1.0f)	);
			// 				pc->green	= NMAX(0.0f, NMIN( (pc->green + pscale->green*NFastRand())*0.5f	,1.0f)	);
			// 				pc->blue	= NMAX(0.0f, NMIN( (pc->blue + pscale->blue*NFastRand())*0.5f		,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f, NMIN( (pc->alpha + pscale->alpha*NFastRand())*0.5f	,1.0f)	);
			pc->red = (pc->red + pscale->red * NFastRand()) * 0.5f;
			pc->green = (pc->green + pscale->green * NFastRand()) * 0.5f;
			pc->blue = (pc->blue + pscale->blue * NFastRand()) * 0.5f;
			pc->alpha = (pc->alpha + pscale->alpha * NFastRand()) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromFastRandEx(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pscale, const Nu32 key_every)
{
	Nu32 i;
	NCOLOR *pc;

	NCOLOR c, ca, cb, cmore;
	Nu32 k;
	Nf32 step;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(key_every < 2, NERROR_UNAUTHORIZED_REQUEST); // Use the primary function 'NUT_LUTBuild_Nf32FromFastRand' to fill up a table !

	k = 0;
	cb.red = NFastRand() * pscale->red;
	cb.green = NFastRand() * pscale->green;
	cb.blue = NFastRand() * pscale->blue;
	cb.alpha = NFastRand() * pscale->alpha;

	step = 1.0f / (Nf32)key_every;

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (!k)
		{
			k = key_every - 1;
			c = ca = cb;
			cb.red = NFastRand() * pscale->red;
			cb.green = NFastRand() * pscale->green;
			cb.blue = NFastRand() * pscale->blue;
			cb.alpha = NFastRand() * pscale->alpha;

			cmore.red = (cb.red - ca.red) * step;
			cmore.green = (cb.green - ca.green) * step;
			cmore.blue = (cb.blue - ca.blue) * step;
			cmore.alpha = (cb.alpha - ca.alpha) * step;
		}
		else
		{
			c.red += cmore.red;
			c.green += cmore.green;
			c.blue += cmore.blue;
			c.alpha += cmore.alpha;

			k--;
		}

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX( 0.0f,NMIN(c.red,	1.0f) );
			// 				pc->green	= NMAX( 0.0f,NMIN(c.green,1.0f) );
			// 				pc->blue	= NMAX( 0.0f,NMIN(c.blue,	1.0f) );
			// 				pc->alpha	= NMAX( 0.0f,NMIN(c.alpha,1.0f) );
			pc->red = c.red;
			pc->green = c.green;
			pc->blue = c.blue;
			pc->alpha = c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		+ c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	+ c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	+ c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	+ c.alpha,1.0f)	);
			pc->red += c.red;
			pc->green += c.green;
			pc->blue += c.blue;
			pc->alpha += c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		* c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	* c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	* c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	* c.alpha,1.0f)	);
			pc->red *= c.red;
			pc->green *= c.green;
			pc->blue *= c.blue;
			pc->alpha *= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		- c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	- c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	- c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	- c.alpha,1.0f)	);
			pc->red -= c.red;
			pc->green -= c.green;
			pc->blue -= c.blue;
			pc->alpha -= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		= NMAX(0.0f, NMIN( (pc->red	+ c.red		)*0.5f	,1.0f)	);
			// 				pc->green	= NMAX(0.0f, NMIN( (pc->green + c.green	)*0.5f	,1.0f)	);
			// 				pc->blue	= NMAX(0.0f, NMIN( (pc->blue  + c.blue	)*0.5f	,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f, NMIN( (pc->alpha + c.alpha	)*0.5f	,1.0f)	);
			pc->red = (pc->red + c.red) * 0.5f;
			pc->green = (pc->green + c.green) * 0.5f;
			pc->blue = (pc->blue + c.blue) * 0.5f;
			pc->alpha = (pc->alpha + c.alpha) * 0.5f;
			break;
		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromFastRandf(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 redscale, const Nf32 greenscale, const Nf32 bluescale, const Nf32 alphascale)
{
	Nu32 i;
	NCOLOR *pc;

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX( 0.0f,NMIN(redscale*NFastRand(),1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(greenscale*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(bluescale*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(alphascale*NFastRand(),1.0f)	);
			pc->red = redscale * NFastRand();
			pc->green = greenscale * NFastRand();
			pc->blue = bluescale * NFastRand();
			pc->alpha = alphascale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red + redscale*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green + greenscale*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue + bluescale*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha + alphascale*NFastRand(),1.0f)	);
			pc->red += redscale * NFastRand();
			pc->green += greenscale * NFastRand();
			pc->blue += bluescale * NFastRand();
			pc->alpha += alphascale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red*redscale*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green*greenscale*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue*bluescale*NFastRand(),1.0f)		);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha*alphascale*NFastRand(),1.0f)	);
			pc->red *= redscale * NFastRand();
			pc->green *= greenscale * NFastRand();
			pc->blue *= bluescale * NFastRand();
			pc->alpha *= alphascale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red - redscale*NFastRand(),1.0f)		);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green - greenscale*NFastRand(),1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue - bluescale*NFastRand(),1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha - alphascale*NFastRand(),1.0f)	);
			pc->red -= redscale * NFastRand();
			pc->green -= greenscale * NFastRand();
			pc->blue -= bluescale * NFastRand();
			pc->alpha -= alphascale * NFastRand();
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		= NMAX(0.0f, NMIN( (pc->red	+ redscale*NFastRand())*0.5f		,1.0f)	);
			// 				pc->green	= NMAX(0.0f, NMIN( (pc->green + greenscale*NFastRand())*0.5f	,1.0f)	);
			// 				pc->blue	= NMAX(0.0f, NMIN( (pc->blue + bluescale*NFastRand())*0.5f		,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f, NMIN( (pc->alpha + alphascale*NFastRand())*0.5f	,1.0f)	);
			pc->red = (pc->red + redscale * NFastRand()) * 0.5f;
			pc->green = (pc->green + greenscale * NFastRand()) * 0.5f;
			pc->blue = (pc->blue + bluescale * NFastRand()) * 0.5f;
			pc->alpha = (pc->alpha + alphascale * NFastRand()) * 0.5f;
			break;

		default:
			break;
		}
	}
}
void NUT_LUTBuild_NColorFromFastRandExf(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 redscale, const Nf32 greenscale, const Nf32 bluescale, const Nf32 alphascale, const Nu32 key_every)
{
	Nu32 i;
	NCOLOR *pc;

	NCOLOR c, ca, cb, cmore;
	Nu32 k;
	Nf32 step;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(key_every < 2, NERROR_UNAUTHORIZED_REQUEST); // Use the primary function 'NUT_LUTBuild_Nf32FromFastRand' to fill up a table !

	k = 0;
	cb.red = NFastRand() * redscale;
	cb.green = NFastRand() * greenscale;
	cb.blue = NFastRand() * bluescale;
	cb.alpha = NFastRand() * alphascale;

	step = 1.0f / (Nf32)key_every;

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (!k)
		{
			k = key_every - 1;
			c = ca = cb;
			cb.red = NFastRand() * redscale;
			cb.green = NFastRand() * greenscale;
			cb.blue = NFastRand() * bluescale;
			cb.alpha = NFastRand() * alphascale;

			cmore.red = (cb.red - ca.red) * step;
			cmore.green = (cb.green - ca.green) * step;
			cmore.blue = (cb.blue - ca.blue) * step;
			cmore.alpha = (cb.alpha - ca.alpha) * step;
		}
		else
		{
			c.red += cmore.red;
			c.green += cmore.green;
			c.blue += cmore.blue;
			c.alpha += cmore.alpha;

			k--;
		}

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX( 0.0f,NMIN(c.red,	1.0f) );
			// 				pc->green	= NMAX( 0.0f,NMIN(c.green,1.0f) );
			// 				pc->blue	= NMAX( 0.0f,NMIN(c.blue,	1.0f) );
			// 				pc->alpha	= NMAX( 0.0f,NMIN(c.alpha,1.0f) );
			pc->red = c.red;
			pc->green = c.green;
			pc->blue = c.blue;
			pc->alpha = c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		+ c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	+ c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	+ c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	+ c.alpha,1.0f)	);
			pc->red += c.red;
			pc->green += c.green;
			pc->blue += c.blue;
			pc->alpha += c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		* c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	* c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	* c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	* c.alpha,1.0f)	);
			pc->red *= c.red;
			pc->green *= c.green;
			pc->blue *= c.blue;
			pc->alpha *= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX( 0.0f,NMIN(pc->red		- c.red,1.0f)	);
			// 				pc->green	= NMAX( 0.0f,NMIN(pc->green	- c.green,1.0f)	);
			// 				pc->blue	= NMAX( 0.0f,NMIN(pc->blue	- c.blue,1.0f)	);
			// 				pc->alpha	= NMAX( 0.0f,NMIN(pc->alpha	- c.alpha,1.0f)	);
			pc->red -= c.red;
			pc->green -= c.green;
			pc->blue -= c.blue;
			pc->alpha -= c.alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		= NMAX(0.0f, NMIN( (pc->red	+ c.red		)*0.5f	,1.0f)	);
			// 				pc->green	= NMAX(0.0f, NMIN( (pc->green + c.green	)*0.5f	,1.0f)	);
			// 				pc->blue	= NMAX(0.0f, NMIN( (pc->blue  + c.blue	)*0.5f	,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f, NMIN( (pc->alpha + c.alpha	)*0.5f	,1.0f)	);
			pc->red = (pc->red + c.red) * 0.5f;
			pc->green = (pc->green + c.green) * 0.5f;
			pc->blue = (pc->blue + c.blue) * 0.5f;
			pc->alpha = (pc->alpha + c.alpha) * 0.5f;
			break;

		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorFromSplines(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *pred_spline, const NSPLINE *pgreen_spline, const NSPLINE *pblue_spline, const NSPLINE *palpha_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistentred, persistentgreen, persistentblue, persistentalpha;
	Nf32 red, green, blue, alpha;
	NCOLOR *pc;

	NErrorIf(!pred_spline, NERROR_NULL_POINTER);
	NErrorIf(!pgreen_spline, NERROR_NULL_POINTER);
	NErrorIf(!pblue_spline, NERROR_NULL_POINTER);
	NErrorIf(!palpha_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistentred, pred_spline);
	NUT_Setup2DSplinePersistentXResult(&persistentgreen, pgreen_spline);
	NUT_Setup2DSplinePersistentXResult(&persistentblue, pblue_spline);
	NUT_Setup2DSplinePersistentXResult(&persistentalpha, palpha_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pc = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	red = green = blue = alpha = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentred))
			red = persistentred.XResult.I.y;
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentgreen))
			green = persistentgreen.XResult.I.y;
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentblue))
			blue = persistentblue.XResult.I.y;
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistentalpha))
			alpha = persistentalpha.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// 				pc->red		= NMAX(0.0f,NMIN(red,1.0f)	);
			// 				pc->green	= NMAX(0.0f,NMIN(green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(alpha,1.0f)	);

			pc->red = red;
			pc->green = green;
			pc->blue = blue;
			pc->alpha = alpha;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red + red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green + green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue + blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha + alpha,1.0f)	);

			pc->red += red;
			pc->green += green;
			pc->blue += blue;
			pc->alpha += alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red * red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green * green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue * blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha * alpha,1.0f)	);

			pc->red *= red;
			pc->green *= green;
			pc->blue *= blue;
			pc->alpha *= alpha;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// 				pc->red		= NMAX(0.0f,NMIN(pc->red - red,1.0f)		);
			// 				pc->green	= NMAX(0.0f,NMIN(pc->green - green,1.0f)	);
			// 				pc->blue	= NMAX(0.0f,NMIN(pc->blue - blue,1.0f)	);
			// 				pc->alpha	= NMAX(0.0f,NMIN(pc->alpha - alpha,1.0f)	);

			pc->red -= red;
			pc->green -= green;
			pc->blue -= blue;
			pc->alpha -= alpha;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// 				pc->red		=NMAX(0.0f,NMIN((pc->red + red)*0.5f,1.0f)		);
			// 				pc->green	=NMAX(0.0f,NMIN((pc->green + green)*0.5f,1.0f)	);
			// 				pc->blue	=NMAX(0.0f,NMIN((pc->blue + blue)*0.5f,1.0f)		);
			// 				pc->alpha	=NMAX(0.0f,NMIN((pc->alpha + alpha)*0.5f,1.0f)	);

			pc->red = (pc->red + red) * 0.5f;
			pc->green = (pc->green + green) * 0.5f;
			pc->blue = (pc->blue + blue) * 0.5f;
			pc->alpha = (pc->alpha + alpha) * 0.5f;
			break;
		default:
			break;
		}
	}
}

void NUT_LUTBuild_NColorClamp(NCOLOR *plut, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax)
{
	Nu32 i;
	NCOLOR *pc;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!pcnewmin, NERROR_NULL_VALUE);
	NErrorIf(!pcnewmax, NERROR_NULL_VALUE);

	pc = plut;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (pc->red < pcnewmin->red)
			pc->red = pcnewmin->red;
		else if (pc->red > pcnewmax->red)
			pc->red = pcnewmax->red;

		if (pc->green < pcnewmin->green)
			pc->green = pcnewmin->green;
		else if (pc->green > pcnewmax->green)
			pc->green = pcnewmax->green;

		if (pc->blue < pcnewmin->blue)
			pc->blue = pcnewmin->blue;
		else if (pc->blue > pcnewmax->blue)
			pc->blue = pcnewmax->blue;

		if (pc->alpha < pcnewmin->alpha)
			pc->alpha = pcnewmin->alpha;
		else if (pc->alpha > pcnewmax->alpha)
			pc->alpha = pcnewmax->alpha;
	}
}

void NUT_LUTBuild_NColorWarp(NCOLOR *plut, const Nu32 element_nb, const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax)
{
	Nu32 i;
	NCOLOR *pc;
	NCOLOR curmin, curmax;
	NCOLOR ratio;

	// NCOLOR		cnewmin,cnewmax;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!pcnewmin, NERROR_NULL_VALUE);
	NErrorIf(!pcnewmax, NERROR_NULL_VALUE);

	// Checks incoming limits !
	// 	cnewmin.red		 = NMAX( 0.0f,NMIN(pcnewmin->red,1.0f) );
	// 	cnewmin.green	 = NMAX( 0.0f,NMIN(pcnewmin->green,1.0f) );
	// 	cnewmin.blue	 = NMAX( 0.0f,NMIN(pcnewmin->blue,1.0f) );
	// 	cnewmin.alpha	 = NMAX( 0.0f,NMIN(pcnewmin->alpha,1.0f) );
	//
	// 	cnewmax.red		 = NMAX( 0.0f,NMIN(pcnewmax->red,1.0f) );
	// 	cnewmax.green	 = NMAX( 0.0f,NMIN(pcnewmax->green,1.0f) );
	// 	cnewmax.blue	 = NMAX( 0.0f,NMIN(pcnewmax->blue,1.0f) );
	// 	cnewmax.alpha	 = NMAX( 0.0f,NMIN(pcnewmax->alpha,1.0f) );

	// Find LUT current min and max
	pc = plut;
	curmin = curmax = *pc;
	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (pc->red < curmin.red)
			curmin.red = pc->red;
		else if (pc->red > curmax.red)
			curmax.red = pc->red;

		if (pc->green < curmin.green)
			curmin.green = pc->green;
		else if (pc->green > curmax.green)
			curmax.green = pc->green;

		if (pc->blue < curmin.blue)
			curmin.blue = pc->blue;
		else if (pc->blue > curmax.blue)
			curmax.blue = pc->blue;

		if (pc->alpha < curmin.alpha)
			curmin.alpha = pc->alpha;
		else if (pc->alpha > curmax.alpha)
			curmax.alpha = pc->alpha;
	}

	// incoming min/max values constraint
	pc = plut;
	ratio.red = (pcnewmax->red - pcnewmin->red) / (curmax.red - curmin.red);
	ratio.green = (pcnewmax->green - pcnewmin->green) / (curmax.green - curmin.green);
	ratio.blue = (pcnewmax->blue - pcnewmin->blue) / (curmax.blue - curmin.blue);
	ratio.alpha = (pcnewmax->alpha - pcnewmin->alpha) / (curmax.alpha - curmin.alpha);

	for (i = element_nb; i != 0; i--, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		// 		pc->red		= NMAX( 0.0f, NMIN( (pc->red+addmin.red)*ratio.red,		1.0f) );
		// 		pc->green	= NMAX( 0.0f, NMIN( (pc->green+addmin.green)*ratio.green, 1.0f) );
		// 		pc->blue	= NMAX( 0.0f, NMIN( (pc->blue+addmin.blue)*ratio.blue,	1.0f) );
		// 		pc->alpha	= NMAX( 0.0f, NMIN( (pc->alpha+addmin.alpha)*ratio.alpha, 1.0f) );

		pc->red = pcnewmin->red + (pc->red - curmin.red) * ratio.red;
		pc->green = pcnewmin->green + (pc->green - curmin.green) * ratio.green;
		pc->blue = pcnewmin->blue + (pc->blue - curmin.blue) * ratio.blue;
		pc->alpha = pcnewmin->alpha + (pc->alpha - curmin.alpha) * ratio.alpha;
	}
}

void NUT_LUTBuild_NColorSmooth(NCOLOR *plut, const Nu32 element_nb, const Nu32 stride, const Nu16 range, const Nu16 pass_nb)
{
	Nu32 i;
	Nf32 inv_coefsum;
	NCOLOR *pc, *pdata, *pd, *prevpd;
	NCOLOR tmp;
	Nu16 p;

	NErrorIf(!stride, NERROR_NULL_VALUE);
	NErrorIf(!range, NERROR_NULL_VALUE);
	NErrorIf(!pass_nb, NERROR_NULL_VALUE);

	pdata = (NCOLOR *)Nmalloc(sizeof(NCOLOR) * (element_nb + 2 * range));
	inv_coefsum = 1.0f / ((Nf32)range * 2.0f + 1.0f);

	for (p = 0; p < pass_nb; p++)
	{
		pc = plut;
		pd = pdata;
		// All divided in one pass !
		// 0) The first
		pd->red = tmp.red = pc->red * inv_coefsum;
		pd->green = tmp.green = pc->green * inv_coefsum;
		pd->blue = tmp.blue = pc->blue * inv_coefsum;
		pd->alpha = tmp.alpha = pc->alpha * inv_coefsum;

		prevpd = pd;
		pd++;
		// 1) The left "out of range" part !
		for (i = range - 1; i != 0; i--, pd++, prevpd++)
		{
			pd->red = prevpd->red + tmp.red;
			pd->green = prevpd->green + tmp.green;
			pd->blue = prevpd->blue + tmp.blue;
			pd->alpha = prevpd->alpha + tmp.alpha;
		}
		// 2) The main part !
		for (i = element_nb; i != 0; i--, pd++, prevpd++, pc = (NCOLOR *)((NBYTE *)pc + stride))
		{
			pd->red = prevpd->red + pc->red * inv_coefsum;
			pd->green = prevpd->green + pc->green * inv_coefsum;
			pd->blue = prevpd->blue + pc->blue * inv_coefsum;
			pd->alpha = prevpd->alpha + pc->alpha * inv_coefsum;
		}

		// 3) The right "out of range" part !
		tmp.red = prevpd->red - ((NCOLOR *)(prevpd - 1))->red;
		tmp.green = prevpd->green - ((NCOLOR *)(prevpd - 1))->green;
		tmp.blue = prevpd->blue - ((NCOLOR *)(prevpd - 1))->blue;
		tmp.alpha = prevpd->alpha - ((NCOLOR *)(prevpd - 1))->alpha;

		for (i = range; i != 0; i--, pd++, prevpd++)
		{
			pd->red = prevpd->red + tmp.red;
			pd->green = prevpd->green + tmp.green;
			pd->blue = prevpd->blue + tmp.blue;
			pd->alpha = prevpd->alpha + tmp.alpha;
		}
		// 4)Final pass: start !
		pc = plut;
		prevpd = pdata;
		pd = &pdata[2 * range];

		pc->red = pd->red;
		pc->green = pd->green;
		pc->blue = pd->blue;
		pc->alpha = pd->alpha;

		// 5)Final pass, loop !
		pc = (NCOLOR *)((NBYTE *)pc + stride);
		pd++;
		for (i = element_nb - 1; i != 0; i--, pd++, prevpd++, pc = (NCOLOR *)((NBYTE *)pc + stride))
		{
			pc->red = pd->red - prevpd->red;
			pc->green = pd->green - prevpd->green;
			pc->blue = pd->blue - prevpd->blue;
			pc->alpha = pd->alpha - prevpd->alpha;
		}
	}

	Nfree(pdata);
}

void NUT_LUTBuild_NColorSplineRed(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *pred_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistent;
	Nf32 component;
	NCOLOR *pc;

	NErrorIf(!pred_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistent, pred_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pc = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	component = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistent))
			component = persistent.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// pc->red		= NMAX(0.0f,NMIN(component,1.0f)	);
			pc->red = component;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// pc->red		= NMAX(0.0f,NMIN(pc->red + component,1.0f));
			pc->red += component;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// pc->red		= NMAX(0.0f,NMIN(pc->red * component,1.0f));
			pc->red *= component;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// pc->red		= NMAX(0.0f,NMIN(pc->red - component,1.0f));
			pc->red -= component;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// pc->red		=NMAX(0.0f,NMIN((pc->red + component)*0.5f,1.0f));
			pc->red = (pc->red + component) * 0.5f;
			break;

		default:
			break;
		}
	}
}
void NUT_LUTBuild_NColorSplineGreen(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *pgreen_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistent;
	Nf32 component;
	NCOLOR *pc;

	NErrorIf(!pgreen_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistent, pgreen_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pc = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	component = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistent))
			component = persistent.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// pc->green		= NMAX(0.0f,NMIN(component,1.0f)	);
			pc->green = component;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// pc->green		= NMAX(0.0f,NMIN(pc->green + component,1.0f));
			pc->green += component;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// pc->green		= NMAX(0.0f,NMIN(pc->green - component,1.0f));
			pc->green -= component;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// pc->green		= NMAX(0.0f,NMIN(pc->green * component,1.0f));
			pc->green *= component;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// pc->green		=NMAX(0.0f,NMIN((pc->green + component)*0.5f,1.0f));
			pc->green = (pc->green + component) * 0.5f;
			break;

		default:
			break;
		}
	}
}
void NUT_LUTBuild_NColorSplineBlue(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *pblue_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistent;
	Nf32 component;
	NCOLOR *pc;

	NErrorIf(!pblue_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistent, pblue_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pc = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	component = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistent))
			component = persistent.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// pc->blue		= NMAX(0.0f,NMIN(component,1.0f)	);
			pc->blue = component;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// pc->blue		= NMAX(0.0f,NMIN(pc->blue + component,1.0f));
			pc->blue += component;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// pc->blue		= NMAX(0.0f,NMIN(pc->blue - component,1.0f));
			pc->blue -= component;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// pc->blue		= NMAX(0.0f,NMIN(pc->blue * component,1.0f));
			pc->blue *= component;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// pc->blue		=NMAX(0.0f,NMIN((pc->blue + component)*0.5f,1.0f));
			pc->blue = (pc->blue + component) * 0.5f;
			break;

		default:
			break;
		}
	}
}
void NUT_LUTBuild_NColorSplineAlpha(NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const NSPLINE *palpha_spline)
{
	Nu32 i;
	Nf32 step;
	NVEC2 C;
	NUT_2DSPLINE_PERSISTENT_XRESULT persistent;
	Nf32 component;
	NCOLOR *pc;

	NErrorIf(!palpha_spline, NERROR_NULL_POINTER);

	// Computing values
	NUT_Setup2DSplinePersistentXResult(&persistent, palpha_spline);

	if (element_nb > 1)
		step = 1.0f / (Nf32)(element_nb - 1);
	else
		step = 0.0f;

	pc = plut;
	C.x = 0.0f;
	C.y = 0.0f;
	component = 0.0f;
	for (i = 0; i < element_nb; i++, C.x = (Nf32)i * step, pc = (NCOLOR *)((NBYTE *)pc + stride))
	{
		if (NUT_2DVerticalLineXSplineExtended(&C, &persistent))
			component = persistent.XResult.I.y;

		switch (operand)
		{
		case NUT_LUTBUILD_OPERAND_PUSH:
			// pc->alpha		= NMAX(0.0f,NMIN(component,1.0f)	);
			pc->alpha = component;
			break;

		case NUT_LUTBUILD_OPERAND_ADD:
			// pc->alpha		= NMAX(0.0f,NMIN(pc->alpha + component,1.0f));
			pc->alpha += component;
			break;

		case NUT_LUTBUILD_OPERAND_SUB:
			// pc->alpha		= NMAX(0.0f,NMIN(pc->alpha - component,1.0f));
			pc->alpha -= component;
			break;

		case NUT_LUTBUILD_OPERAND_SCALE:
			// pc->alpha		= NMAX(0.0f,NMIN(pc->alpha * component,1.0f));
			pc->alpha *= component;
			break;

		case NUT_LUTBUILD_OPERAND_AVERAGE:
			// pc->alpha		=NMAX(0.0f,NMIN((pc->alpha + component)*0.5f,1.0f));
			pc->alpha = (pc->alpha + component) * 0.5f;
			break;

		default:
			break;
		}
	}
}
