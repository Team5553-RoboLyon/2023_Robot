#ifndef __NCOLOR_H
#define __NCOLOR_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NColor.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// COLOR ID
typedef enum
{
	_COLOR_FORMAT_NCOLOR = 0,
	_COLOR_FORMAT_NHSV,
	_COLOR_FORMAT_RGBA8,
	_COLOR_FORMAT_RGB8,
	_COLOR_FORMAT_RGB5,
	_COLOR_FORMAT_GRAYA8,
	//---------------------
	NCOLOR_FORMAT_ENUM_SIZE
}NCOLOR_FORMAT_ENUM;

// COLOR LERP WRAP MODE( to work with the function "NLerpColorRange" )
typedef enum
{
	NCOLORRANGE_LERP_CLAMP=0,
	NCOLORRANGE_LERP_REPEAT,
	// ---------------
	NCOLORRANGE_LERP_MODE_ENUM_SIZE // Max size = 4. And Max NCOLORRANGE_LERP_MODE ID = 3. Because it may be packed into a NCOLORGRADIENTID as a 2 Bits value.
}NCOLORRANGE_LERP_MODE_ENUM;

// COLOR PRESETS
#define	NRGBA8_PRESET_WHITE					0xFFFFFFFF
#define	NRGBA8_PRESET_BLACK					0x000000FF
#define	NRGBA8_PRESET_GRAY					0x808080FF
#define NRGBA8_PRESET_DARKGRAY				0x404040FF
#define	NRGBA8_PRESET_RED					0xFF0000FF
#define	NRGBA8_PRESET_GREEN					0x00FF00FF
#define	NRGBA8_PRESET_BLUE					0x0000FFFF
#define	NRGBA8_PRESET_YELLOW				0xFFFF80FF
#define	NRGBA8_PRESET_ORANGE				0xFFB240FF

// NCOLOR BASIC PRESETS
#define NCOLOR_PRESET3F_WHITE				1.000f,	1.000f,	1.000f
#define NCOLOR_PRESET3F_BLACK				0.000f,	0.000f,	0.000f
#define NCOLOR_PRESET3F_LIGHTGRAY			0.750f,	0.750f,	0.750f
#define NCOLOR_PRESET3F_GRAY				0.500f,	0.500f,	0.500f
#define NCOLOR_PRESET3F_DARKGRAY			0.250f,	0.250f,	0.250f
#define NCOLOR_PRESET3F_RED					1.000f,	0.000f,	0.000f
#define NCOLOR_PRESET3F_GREEN				0.000f,	1.000f,	0.000f
#define NCOLOR_PRESET3F_BLUE				0.000f,	0.000f,	1.000f
#define NCOLOR_PRESET3F_YELLOW				1.000f,	1.000f,	0.500f
#define NCOLOR_PRESET3F_ORANGE				1.000f,	0.647f,	0.000f
#define NCOLOR_PRESET3F_PALEPLUM			0.867f, 0.627f, 0.867f
#define NCOLOR_PRESET3F_PLUM				0.557f, 0.271f, 0.522f
#define NCOLOR_PRESET3F_PURPLE				0.500f,	0.000f,	0.500f

// NCOLOR COMMON SUBTLE PRESETS
#define NCOLOR_PRESET3F_BLUE_MONASTRAL		0.000f, 0.06f,	0.54f
#define NCOLOR_PRESET3F_BLUE_OXFORD			0.000f, 0.13f,	0.28f
#define NCOLOR_PRESET3F_BLUE_NAVY			0.000f, 0.000f, 0.502f
#define NCOLOR_PRESET3F_BLUE_PRUSSIAN		0.000f, 0.192f, 0.325f
#define NCOLOR_PRESET3F_BLUE_PERSIAN		0.110f, 0.224f, 0.733f
#define NCOLOR_PRESET3F_BLUE_COBALT			0.000f, 0.278f, 0.621f
#define NCOLOR_PRESET3F_BLUE_SAPPHIRE		0.059f, 0.322f, 0.729f
#define NCOLOR_PRESET3F_BLUE_CYAN			0.000f, 1.000f, 1.000f
#define NCOLOR_PRESET3F_BLUE_ELECTRIC		0.490f, 0.976f, 1.000f
#define NCOLOR_PRESET3F_BLUE_CERULEAN		0.000f, 0.482f, 0.655f
#define NCOLOR_PRESET3F_BLUE_BABY			0.537f, 0.812f, 0.941f
#define NCOLOR_PRESET3F_BLUE_AZURE			0.000f, 0.498f, 1.000f
#define NCOLOR_PRESET3F_BLUE_AQUAMARINE		0.498f, 1.000f, 0.831f
#define NCOLOR_PRESET3F_BLUE_TEAL			0.000f, 0.502f, 0.502f
#define NCOLOR_PRESET3F_BLUE_TURQUOISE		0.251f, 0.878f, 0.816f
#define NCOLOR_PRESET3F_BLUE_VIOLET			0.541f, 0.169f, 0.886f

#define NCOLOR_PRESET3F_GREEN_CHARTREUSE	0.500f,	1.000f,	0.000f
#define NCOLOR_PRESET3F_GREEN_HARLEQUIN		0.250f,	1.000f,	0.000f
#define NCOLOR_PRESET3F_GREEN_SPRING		0.000f,	1.000f,	0.498f
#define NCOLOR_PRESET3F_GREEN_EMERALD		0.314f,	0.784f,	0.471f
#define NCOLOR_PRESET3F_GREEN_NAPIER		0.231f,	0.494f,	0.000f
#define NCOLOR_PRESET3F_GREEN_ERIN			0.000f,	1.000f,	0.247f
#define NCOLOR_PRESET3F_GREEN_JADE			0.000f,	0.659f,	0.420f
#define NCOLOR_PRESET3F_GREEN_JUNGLE		0.161f,	0.671f,	0.529f
#define NCOLOR_PRESET3F_GREEN_PEAR			0.820f,	0.886f,	0.192f
#define NCOLOR_PRESET3F_GREEN_PERSIAN		0.165f,	0.616f,	0.561f



#define NCOLOR_PRESET3F_ORANGE_AMBER		1.000f,	0.749f, 0.000f
#define NCOLOR_PRESET3F_ORANGE_GOLD			1.000f,	0.843f, 0.000f
#define NCOLOR_PRESET3F_ORANGE_RED			1.000f,	0.271f, 0.000f
#define NCOLOR_PRESET3F_ORANGE_MANGO		0.976f,	0.518f, 0.290f

#define NCOLOR_PRESET3F_RED_BLOOD			0.4f,	0.0f,	0.0f
#define NCOLOR_PRESET3F_RED_CRAYOLA			0.910f,	0.282f, 0.333f
#define NCOLOR_PRESET3F_RED_CRIMSON			0.863f,	0.078f, 0.235f
#define NCOLOR_PRESET3F_RED_SCARLET			1.000f, 0.141f, 0.000f
#define NCOLOR_PRESET3F_RED_AMARANTH		0.898f, 0.169f, 0.314f
#define NCOLOR_PRESET3F_RED_MADDER			0.647f, 0.0f,	0.129f


#define NCOLOR_PRESET3F_BRONZE				0.804f,	0.498f, 0.196f
#define NCOLOR_PRESET3F_BRONZE_GOLD			0.788f,	0.682f, 0.365f
#define NCOLOR_PRESET3F_BRONZE_DEEP			0.549f,	0.471f, 0.325f

#define NCOLOR_PRESET3F_YELLOW_CANARI		1.000f,	0.992f, 0.510f
#define NCOLOR_PRESET3F_YELLOW_LEMON		1.000f,	0.969f, 0.000f
#define NCOLOR_PRESET3F_YELLOW_LASER_LEMON	0.996f,	0.996f, 0.133f

#define NCOLOR_PRESET3F_PINK_BARBIE			0.880f,	0.130f, 0.540f
#define NCOLOR_PRESET3F_PINK_BRILLIANT		1.000f,	0.330f, 0.640f
#define NCOLOR_PRESET3F_PINK_BRINK			0.980f,	0.380f, 0.500f
#define NCOLOR_PRESET3F_PINK_CARNATION		1.000f,	0.650f, 0.790f
#define NCOLOR_PRESET3F_PINK_CERISE			0.930f,	0.230f, 0.510f
#define NCOLOR_PRESET3F_PINK_CHINESE		0.870f,	0.440f, 0.630f
#define NCOLOR_PRESET3F_PINK_ORCHID			0.855f,	0.439f, 0.839f
#define NCOLOR_PRESET3F_PINK_PARADISE		0.937f,	0.278f, 0.435f

#define NCOLOR_PRESET3F_MAGENTA				1.000f,	0.000f, 1.000f
#define NCOLOR_PRESET3F_MAGENTA_ROSE		1.000f,	0.000f, 0.686f

#define NCOLOR_PRESET3F_LAVANDER			0.710f, 0.130f, 0.540f
#define NCOLOR_PRESET3F_VIOLET				0.933f, 0.510f, 0.933f
#define NCOLOR_PRESET3F_PEACH				1.000f, 0.898f, 0.706f
#define NCOLOR_PRESET3F_IVORY				1.000f, 1.000f, 0.941f

// NCOLOR SYSTEM PRESETS
#define NCOLOR_PRESET3F_SYSTEM				NCOLOR_PRESET3F_BLUE_CERULEAN
#define NCOLOR_PRESET3F_SYSTEM_LIGHT		NCOLOR_PRESET3F_YELLOW_LEMON
#define NCOLOR_PRESET3F_SYSTEM_SHADOW		NCOLOR_PRESET3F_GREEN_EMERALD
#define NCOLOR_PRESET3F_SYSTEM_BACKGROUND	NCOLOR_PRESET3F_BLACK
#define NCOLOR_PRESET3F_SYSTEM_WARNING		NCOLOR_PRESET3F_RED_SCARLET


// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// a 'Color' for NENGINE
typedef struct
{
	union
	{
		struct  
		{
			Nf32 red;
			Nf32 green;
			Nf32 blue;
			Nf32 alpha;
		};
		Nf32	comp[4];
	};
}NCOLOR;

typedef struct  
{
	Nf32	Hue;
	Nf32	Saturation;
	Nf32	Brightness; // or Value ...
}NHSV;

// RGBA 32 bits, 4 components, 8 bits per component
#define RGBA8(r,g,b,a)  ((( (Nu32)(r) << 24 )&0xFF000000)|( ((Nu32)(g) << 16)&0xFF0000 )|( ((Nu32)(b) << 8)&0xFF00  )|(((Nu32)(a))&0xFF ))
#define RGBA8_R(c)		( (Nu8)( ((Nu32)(c))>>24 )&0xFF )
#define RGBA8_G(c)		( (Nu8)( ((Nu32)(c))>>16 )&0xFF )
#define RGBA8_B(c)		( (Nu8)( ((Nu32)(c))>>8  )&0xFF )
#define RGBA8_A(c)		( (Nu8)( ((Nu32)(c))&0xFF) )

// RGB 24 bits (no alpha), 3 components, 8 bits per component.
#define RGB8(r,g,b)		(( ((Nu32)(r) << 16)&0xFF0000 )|( ((Nu32)(g) << 8)&0xFF00  )|(((Nu32)(b))&0xFF ))
#define RGB8_R(c)		( (Nu8)( ((Nu32)(c))>>16 )&0xFF )
#define RGB8_G(c)		( (Nu8)( ((Nu32)(c))>>8  )&0xFF )
#define RGB8_B(c)		( (Nu8)( ((Nu32)(c))&0xFF) )

// RGB 16 bits (no alpha), 3 components, 5 bits per component.
#define RGB5(r,g,b)	( ( ((r)&0x001F)<<10 )|( ((g)&0x001F)<<5 )|((b)&0x001F) )
#define RGB5_R(c)		(((Nu16)(c)&0x7C00)>>10)	
#define RGB5_G(c)		(((Nu16)(c)&0x03E0)>>5)	
#define RGB5_B(c)		((Nu16)(c)&0x001F)	

// GRAY 16 bits, 2 components, 8 bits per component.
#define GRAYA8(g,a)	( ( ( (Nu16)(g) << 8 )&0xFF00 )|( (Nu8)(a) )&0xFF )
#define GRAYA8_G(c)	( (Nu8)(((c)&0xFF00)>>8) )
#define GRAYA8_A(c)	( (Nu8)((c)&0xFF) )

// useful color range sizes
#define	NRANGE_2COLORS	2			// for  ring building
#define	NRANGE_3COLORS	3			// for triangle building
#define	NRANGE_4COLORS	4			// for quad building

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Color
NCOLOR*				NLerpColorRange(NCOLOR *presult, const NCOLOR *prange, const Nu32 range_size, const Nf32 ratio, const NCOLORRANGE_LERP_MODE_ENUM mode );
NCOLOR*				NCopyColorRange( NCOLOR *pdst_range, const Nu32 dst_rangesize, const NCOLOR *psrc_range, const Nu32 src_rangesize );
inline NCOLOR*		NScaleColor(NCOLOR* pc, const Nf32 scalecomp, const Nf32 scalealpha) { pc->red *= scalecomp; pc->green *= scalecomp; pc->blue *= scalecomp; pc->alpha *= scalealpha; return pc; }

inline NCOLOR*		NSetColorf(NCOLOR* pc,const Nf32 r,const Nf32 g, const Nf32 b, const Nf32 a){pc->red=r;pc->green=g;pc->blue=b;pc->alpha=a;return pc;}
// inline NCOLOR*		NColorCopy(NCOLOR*dest,const NCOLOR*src){/*memcpy(dest,src,sizeof(NCOLOR))*/*dest = *src;}
inline NCOLOR*		NSetColorRangef( NCOLOR *pfirst, const Nu32 size, const Nf32 r, const Nf32 v, const Nf32 b, const Nf32 a ){Nu32 i;for(i=0;i<size;i++){NSetColorf(&pfirst[i],r,v,b,a);}return pfirst;}
inline NCOLOR*		NSetColorRange( NCOLOR *pfirst, const Nu32 size, const NCOLOR *pcolor ){Nu32 i;for(i=0;i<size;i++){pfirst[i]=*pcolor;}return pfirst;}
//inline void		NColorRangeCopy( NCOLOR *pdest, const NCOLOR *psource, const Nu32 size ){memcpy(pdest,psource,size*sizeof(NCOLOR));}
inline NCOLOR*		NColorLerp(NCOLOR* cr, const NCOLOR* corigin, const NCOLOR* c,const Nf32 t ){cr->red = corigin->red + (c->red - corigin->red)*t;cr->green = corigin->green + (c->green - corigin->green)*t;cr->blue = corigin->blue + (c->blue - corigin->blue)*t;cr->alpha = corigin->alpha + (c->alpha - corigin->alpha)*t;return cr;}

NCOLOR*				NSetColorRangeGradient( NCOLOR *pcolor_range,const Nu32 size, const Nu32 colornbmax,const NCOLOR *pcolorA,const NCOLOR *pcolorB);
inline NCOLOR*		NSetColorRangeGradientf(NCOLOR *pcolor_range,const Nu32 size, const Nu32 colornbmax, const Nf32 r0, const Nf32 v0, const Nf32 b0, const Nf32 a0,const Nf32 r1, const Nf32 v1, const Nf32 b1, const Nf32 a1 )
{
	NCOLOR	colorA,colorB;

	NSetColorf(&colorA,r0,v0,b0,a0);
	NSetColorf(&colorB,r1,v1,b1,a1);
	return NSetColorRangeGradient(pcolor_range,size,colornbmax,&colorA,&colorB);
}

// Conversion Methods
NCOLOR*			NHSV_To_NCOLOR( NCOLOR *pcolor, const NHSV *phsv, const Nf32 alpha );
NHSV*			NCOLOR_To_NHSV( NHSV *phsv, const NCOLOR *pcolor );

inline Nu32*	NCOLOR_To_RGBA8(Nu32 *rgba, const NCOLOR *pcolor){*rgba = RGBA8( (Nu8)(pcolor->red*255.0f),(Nu8)(pcolor->green*255.0f),(Nu8)(pcolor->blue*255.0f),(Nu8)(pcolor->alpha*255.0f) );return rgba;}
inline NCOLOR*	RGBA8_To_NCOLOR(NCOLOR *pcolor, const Nu32 rgba){pcolor->red = (Nf32)RGBA8_R(rgba)/255.0f; pcolor->green = (Nf32)RGBA8_G(rgba)/255.0f; pcolor->blue = (Nf32)RGBA8_B(rgba)/255.0f; pcolor->alpha = (Nf32)RGBA8_A(rgba)/255.0f;return pcolor;}

inline Nu32*	NCOLOR_To_RGB8(Nu32 *rgb8, const NCOLOR *pcolor){*rgb8 = RGB8( (Nu8)(pcolor->red*255.0f),(Nu8)(pcolor->green*255.0f),(Nu8)(pcolor->blue*255.0f) );return rgb8;}
inline NCOLOR*	RGB8_To_NCOLOR(NCOLOR *pcolor, const Nu32 rgb, const Nf32 alpha){pcolor->red = (Nf32)RGB8_R(rgb)/255.0f; pcolor->green = (Nf32)RGB8_G(rgb)/255.0f; pcolor->blue = (Nf32)RGB8_B(rgb)/255.0f; if(alpha>=0.0f){pcolor->alpha = alpha;}return pcolor;}

inline Nu32*	NCOLOR_To_RGB5(Nu32 *rgb5, const NCOLOR *pcolor){*rgb5 = RGB5( (Nu8)(pcolor->red*31.0f),(Nu8)(pcolor->green*31.0f),(Nu8)(pcolor->blue*31.0f) );return rgb5;}
inline NCOLOR*	RGB5_To_NCOLOR(NCOLOR *pcolor, const Nu32 rgb5, const Nf32 alpha){pcolor->red = (Nf32)RGB5_R(rgb5)/31.0f; pcolor->green = (Nf32)RGB5_G(rgb5)/31.0f; pcolor->blue = (Nf32)RGB5_B(rgb5)/31.0f; if(alpha>=0.0f){pcolor->alpha = alpha;}return pcolor;}

inline Nu32*	NCOLOR_To_GRAYA8(Nu32 *graya, const NCOLOR *pcolor){*graya = GRAYA8( (Nu8)((pcolor->red+pcolor->green+pcolor->blue)*(255.0f/3.0f)), (Nu8)(pcolor->alpha*255.0f) );return graya;}
inline NCOLOR*	GRAYA8_To_NCOLOR(NCOLOR *pcolor, const Nu32 graya8){pcolor->red = pcolor->green = pcolor->blue = (Nf32)GRAYA8_G(graya8)/255.0f, (Nf32)GRAYA8_A(graya8)/255.0f;return pcolor;}

NCOLOR*			NColorConverter(NCOLOR *pcolor, const void *psrc,const NCOLOR_FORMAT_ENUM src_format);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NCOLOR_H 

