#ifndef __NCOLOR_PALETTE_H
#define __NCOLOR_PALETTE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NColorPalette.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NErrorHandling.h"
#include "NColor.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // ***************************************************************************************
    // **					PICKPACK value													**
    // ***************************************************************************************
    typedef Nu32 NCOLORPICKPACK;

//
//	A static color palette picking is a packed 32 bits value with all the picking infos inside.
//
//		|	- BITS per COLOR Id				2 Bits	number of bits used to define one embed color id. The exact number of bit used is done by: ( 2^x ) where x is the 2bits value.
//	BIT	|	- MOD.ALPHA						1 Bit	TRUE :Alpha Channel will be modulated as the R,G,B components FALSE : No Modulation for Alpha channel
//	 0	|	- + 8 HEIGHTH					1 Bit	TRUE : add 8 heights to the number of heigths given by the 3 bits modulation factor. FALSE: nothing added.
//	 to	|	- MODULATION FACTOR				3 Bits	used modulation factor value will be computed like this: (float)[ 8 - 3bitsvalue ) / 8 ]
//	 7	|										The complete value computed by the NSTCPLT_GET_HEIGHTH macro is equal to : '+8 HEIGHTH' + 'MODULATION FACTOR'
//		|	- VALID ? 						1 Bit	TRUE : 32 Bits description is Valid.	FLASE : 32 bits description is invalid, all default values will be used instead.
//
//	BIT	|	- INDEX of COLOR#0 				1 or 2 or 4 or 8 Bits
//	 8	|	- INDEX of COLOR#1 				1 or 2 or 4 or 8 Bits
//	 to	|	- ...
//	 31	|	- INDEX of COLOR#n 				1 or 2 or 4 or 8 Bits
//
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	Number of colors index packed inside 24 bits according to the number of bit allowed to describe ONE index:
//	1 bit							24 color index 		( = 24 / 1 )	with a value from 0 to 1	-> should work with a 2   color palette. ( but it's not an obligation )
//	2 bits							12 color index		( = 24 / 2 )	with a value from 0 to 3	-> should work with a 4   color palette. ( but it's not an obligation )
//	4 bits							 6 color index		( = 24 / 4 )	with a value from 0 to 15	-> should work with a 16  color palette. ( but it's not an obligation )
//	8 bits							 3 color index		( = 24 / 8 )	with a value from 0 to 255	-> should work with a 256 color palette. ( but it's not an obligation )
//
// Example: with 4 Bits packed index.
// -----------------------------------
// |___________|___________|___________|___________|___________|___________|___________|___________|
// |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
// |		   |		   |		   |		   |		   |		   |  |		   |  |	 |	   |
// |		   |		   |		   |		   |		   |		   |  |		   |+ |	 |	   |
// |		   |		   |		   |		   |		   |		   |  |Heighth |8 |	 |	   |
// |		   |		   |		   |		   |		   |		   |  |  for   |H |	 | 2^x |
// | COLOR #5  | COLOR #4  | COLOR #3  | COLOR #2  | COLOR #1  | COLOR #0  |V |	R G B  |e |M | BITS|
// |  index	   |  index	   |  index	   |  index	   |  index	   |  index	   |A |	MODUL. |i |O | per |
// |		   |		   |	       |		   |		   |	       |L |	FACTOR |g |D |COLOR|
// |		   |		   |		   |		   |		   |		   |I |		   |h |. |	ID |
// |		   |		   |		   |		   |		   |		   |D |		   |t |A |	   |
// |           |		   |		   |		   |		   |		   |? |		   |h |L |     |
// |           |		   |		   |		   |		   |		   |  |		   |  |P |	   |
// |           |		   |		   |		   |		   |		   |  |		   |  |H |	   |
// |           |		   |		   |		   |		   |		   |  |		   |  |A |	   |
// |           |		   |		   |		   |		   |		   |  |		   |  |? |	   |
// -------------------------------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	About Modulation factor:
//							used_factor = [ 8 - 3bitsvalue + 8*1bitvalue ) / 8 ]
//												r = r*used_factor
//												g = g*used_factor
//												b = b*used_factor
//												a = a*used_factor ( ! only if MOD.ALPHA is set to TRUE else a = a )
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Some MACROS to define a packed 32 bits value with 2, 4, and 8 bits per colorid.
// These MACROS set 'MOD.W', 'MOD.V' and 'MOD.ALPHA' bits ALL to zero. that means:
//											MOD.ALPHA set to FALSE means The modulation of the Alpha component is OFF.
//											the 3 bits of MOD.V (RGB MODULATOR) all set to FALSE ...
//											... AND
//											the bit of MID.W ( RGB MODULATOR +1) means the modulation factor is 1.
//																				= [ 8 - 3bitsvalue + 8*1bitvalue ) / 8 ]
//																				= [ 8 - 0 ) + 8*0 / 8 ]
//																				= 1
// example of use:
//											NSTCPLT_8B_PCK(2,201,127)		The first picked color has index 2 in the palette, the second one has index 201 and the third one has index 127.
//																			Of course the associated STATIC PALETTE must handle all the 8 bits index ( = 255 colors index ) !
//											NSTCPLT_4B_PCK(1,15,8,0,0,0)	The first picked color has index 1 in the palette, the second one has index 15 and the third one has index 8...
//																			Of course the associated STATIC PALETTE must handle all the 3 bits index ( = 15 colors index ) !
//
//															BIT/
//													 Valid?| id	 | Color#0		| Color#1		 | Color#2			| Color#3		 | Color#4		 | Color#5		 | Color#6		 | Color#7		 | Color#8		 | Color#9		 | Color#10		 | Color#11		 |
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NSTCPLT_8b_PCK(a, b, c) (0x80 | 0x3 | ((a)&0xFF) << 8 | ((b)&0xFF) << 16 | ((c)&0xFF) << 24)
#define NSTCPLT_4b_PCK(a, b, c, d, e, f) (0x80 | 0x2 | ((a)&0xF) << 8 | ((b)&0xF) << 12 | ((c)&0xF) << 16 | ((d)&0xF) << 20 | ((e)&0xF) << 24 | ((f)&0xF) << 28)
#define NSTCPLT_2b_PCK(a, b, c, d, e, f, g, h, i, j, k, l) (0x80 | 0x1 | ((a)&0x3) << 8 | ((b)&0x3) << 10 | ((c)&0x3) << 12 | ((d)&0x3) << 14 | ((e)&0x3) << 16 | ((f)&0x7) << 18 | ((g)&0x3) << 20 | ((h)&0x3) << 22 | ((i)&0x3) << 24 | ((j)&0x3) << 26 | ((k)&0x3) << 28 | ((l)&0x3) << 30)

// Some MACROS to define a packed 32 bits value with 2, 4 and 8 bits per colorid.
// These MACROS set 'MOD.W', 'MOD.V' and 'MOD.ALPHA' bits respectively to 'w', 'v' and 'p' value.
//
// example of use:
//											NSTCPLT_8B_MPCK(1,5,1,201,127)		MOD.W = 1, that means modulation factor will be, at least equal
//																				Of course the associated STATIC PALETTE must handle all the 8 bits index ( = 255 colors index ) !
//											NSTCPLT_4B_MPCK(0,2,0,15,8,0,0,0)	The first picked color has index 1 in the palette, the second one has index 15 and the third one has index 8...
//																				Of course the associated STATIC PALETTE must handle all the 3 bits index ( = 15 colors index ) !
//																BIT/
//														 Valid?| id	 | MOD.ALPHA ?	| MOD.W			| MOD.V			| Color#0		| Color#1		 | Color#2			| Color#3		 | Color#4		 | Color#5		 | Color#6		 | Color#7		 | Color#8		 | Color#9		 | Color#10		 | Color#11		 |
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NSTCPLT_8b_MPCK(w, v, p, a, b, c) (0x80 | 0x3 | ((p)&0x1) << 2 | ((w)&0x1) << 3 | ((v)&0x7) << 4 | ((a)&0xFF) << 8 | ((b)&0xFF) << 16 | ((c)&0xFF) << 24)
#define NSTCPLT_4b_MPCK(w, v, p, a, b, c, d, e, f) (0x80 | 0x2 | ((p)&0x1) << 2 | ((w)&0x1) << 3 | ((v)&0x7) << 4 | ((a)&0xF) << 8 | ((b)&0xF) << 12 | ((c)&0xF) << 16 | ((d)&0xF) << 20 | ((e)&0xF) << 24 | ((f)&0xF) << 28)
#define NSTCPLT_2b_MPCK(w, v, p, a, b, c, d, e, f, g, h, i, j, k, l) (0x80 | 0x1 | ((p)&0x1) << 2 | ((w)&0x1) << 3 | ((v)&0x7) << 4 | ((a)&0x3) << 8 | ((b)&0x3) << 10 | ((c)&0x3) << 12 | ((d)&0x3) << 14 | ((e)&0x3) << 16 | ((f)&0x7) << 18 | ((g)&0x3) << 20 | ((h)&0x3) << 22 | ((i)&0x3) << 24 | ((j)&0x3) << 26 | ((k)&0x3) << 28 | ((l)&0x3) << 30)

// TO GET COLOR ID ( in static color palette ) and MODULATION FACTOR from a 32 bits PICKING DESCRIPTION:
#define NSTCPLT_GET_BPI(k) (1 << ((k)&0x2))
#define NSTCPLT_GET_COLID(i, k) ((k)&0x80 ? ((k) >> (8 + (i) * (1 << ((k)&0x2)))) & (0xFF >> (1 << ((k)&0x2))) : (i))

#define NSTCPLT_GET_COLID_MSK2b(i, k) (((k)&0x80 ? ((k) >> (8 + (i) * (1 << ((k)&0x2)))) & (0xFF >> (1 << ((k)&0x2))) : (i)) & 0x3)
#define NSTCPLT_GET_COLID_MSK4b(i, k) (((k)&0x80 ? ((k) >> (8 + (i) * (1 << ((k)&0x2)))) & (0xFF >> (1 << ((k)&0x2))) : (i)) & 0xF)
#define NSTCPLT_GET_COLID_MSK8b(i, k) (((k)&0x80 ? ((k) >> (8 + (i) * (1 << ((k)&0x2)))) & (0xFF >> (1 << ((k)&0x2))) : (i)) & 0xFF)
#define NSTCPLT_GET_COLID_MSK(i, k, msk) (((k)&0x80 ? ((k) >> (8 + (i) * (1 << ((k)&0x2)))) & (0xFF >> (1 << ((k)&0x2))) : (i)) & (msk))

#define NSTCPLT_GET_HEIGHTH(k) (8 - (((k) >> 4) & 0x7) + ((k)&0x8))
#define NSTCPLT_IS_ALPHAMOD_ON(k) ((k)&0x4)

// MACROS Details:
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NSTCPLT_GET_BPI(k)						... to retry the number of bits used to encode one packed color index
//												pbi itself is encoded with only 2 bits ( possible values : 0,1,2,3 )
//												This 2 bits value is x and pbi = 2^x
//												so bpi must be 1,2,4 or 8.
//
//											( 1<<( (k)&0x2 ) )
//											 ----  -------
//											  |		  |
//						   shift to the left  |		  | retrieve the 2 first bits  of k
//				with a number of steps equal  |		  | where the dpi exposant is encoded ( possible values are : 0,1,2,3 )
//							 to the exposant  |
//			after shifting possible value are:|
//									  1,2,4,8 |

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NSTCPLT_GET_COLID(i,k)					... To extract a nested static palette colorid.
//
//											( (k)&0x80 ? ( (k)>>(8 +(i)*(1<<(k)&0x2)) )&( 0xFF >> (1<<(k)&0x2) ) : (i) )
//											  --------	   --------------------------	  --------------------	   ---
//				  check validity of the 32 bits	 |						|							|				|
//									PACKING data |						|							|				| If the pack is not valid.
//																		|							|				| MACRO simply gives us (i).
//										To shift bits with the right	|							|
//										step number. These one is equal |							|
// 				    to 8 plus the size of one packed index multiply by  |							|
//					the value of the requested packid					|							|
//																									|
//															Building the mask with the right number |
//															of Bits ( set to TRUE ), according with |
//									the embed description. (1<<(k)&0x2) returns this number of bits.|
//

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NSTCPLT_GET_COLID_MSK(i,k,msk)			... exactly like like NSTCPLT_GET_COLID(i,k) but with a safe additionnal mask apply at the whole end to ensure
//												output value max. Why that ? Because user may use a 8 bits NCOLORPICKPACK to describe what he wants, and, in that
//												case, with the use of  NSTCPLT_GET_COLID(i,k), output value will be a 8 bits value and so, may be ... 255 for instance.
//												and maybe, behind the scene, programm is waiting for a a bits value at maximum. ( max = 15 ).
//												To handle those cases it's possible to test the NCOLORPICKPACK configuration and retry the number of bits per index with NSTCPLT_GET_BPI(k)
//												and behave in consequence. An other and elegant way is to use NSTCPLT_GET_COLID_MSK(i,k,msk) with msk = 0xF to keep output value <= 15
//
//											(( (k)&0x80 ? ( (k)>>(8 +(i)*(1<<(k)&0x2)) )&( 0xFF >> (1<<(k)&0x2) ) : (i) )	& (msk) )
//																															  -----
//																																|
//																																|
//																								Acceptable value of mask are:	|
//														2 bits mask		0x3	: to keep output value v like this	 0 <= v <= 3	|
//														3 bits mask		0x7	: to keep output value v like this	 0 <= v <= 7	|
//														4 bits mask		0xF	: to keep output value v like this	 0 <= v <= 15	|
//
//														5 bits mask		0x1F: to keep output value v like this	 0 <= v <= 31	|
//														6 bits mask		0x3F: to keep output value v like this	 0 <= v <= 63	|
//														7 bits mask		0x7F: to keep output value v like this	 0 <= v <= 127	|
//
//														8 bits mask		0xFF: to keep output value v like this	 0 <= v <= 255  |
//																		using 0xFF as a mask may be a waste of time if we are   |
//													  sure that i ( icomming index ) is inside the used palette range of index.	|
//													  because if pickpack is = 0 NSTCPLT_GET_COLID_MSK(i,k,msk) returns i itself|
//

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NSTCPLT_GET_HEIGHTH(k)					... To get the number of heigths to build the scale factor with. [ color scale factor = (float)heighth / 8.0f ]
//											This MACRO returns a value between 1 to 16. In that way, color scale factor may be one of these 16 values:
//
//																										1/8,	1/4,	3/8,	1/2,	5/8,	3/4,	7/8,	1
//																										1+1/8,	1+1/4,	1+3/8,	1+1/2,	1+5/8,	1+3/4,	1+7/8,	2
//
//											(8-(((k)>>4)&0x7) + ((k)&0x8))
//											-----------------	----------
//													|				|
//								a value from 8 to 1	|				| 8 or 0
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NSTCPLT_IS_ALPHAMOD_ON(k)			... To apply or not color scale factor to the Alpha channel ( TRUE: apply	FALSE: not apply )
//
//											((k) & 0x4)
//											-----------
//												 | Not equal to 1 but to 0x4 in case of success !!! The correct form to have TRUE/FALSE result :  ( (k)>>2 ) & 0x1
//												 | But it's enough to perform the tests N needs inside its functions.
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Some additionnal predefined value to use  with all the NSTCPLT_?B_MPCK MACROS.
//
// Example:
//			NSTCPLT_8B_MPCK(_DARK_7, a,b,c)	is equivalant to  NSTCPLT_8B_MPCK(0,7,0, a,b,c)
//
#define _DARK_7 0, 7, 0 // scale factor = 1/8	alpha will not be affected --> the DARKEST
#define _DARK_6 0, 6, 0 // scale factor = 2/8	alpha will not be affected
#define _DARK_5 0, 5, 0 // scale factor = 3/8	alpha will not be affected
#define _DARK_4 0, 4, 0 // scale factor = 4/8	alpha will not be affected
#define _DARK_3 0, 3, 0 // scale factor = 5/8	alpha will not be affected
#define _DARK_2 0, 2, 0 // scale factor = 6/8	alpha will not be affected
#define _DARK_1 0, 1, 0 // scale factor = 7/8	alpha will not be affected --> the softest DARK

#define _LIGHT_1 1, 7, 0 // scale factor = 1+1/8	alpha will not be affected --> the softest LIGHT
#define _LIGHT_2 1, 6, 0 // scale factor = 1+2/8	alpha will not be affected
#define _LIGHT_3 1, 5, 0 // scale factor = 1+3/8	alpha will not be affected
#define _LIGHT_4 1, 4, 0 // scale factor = 1+4/8	alpha will not be affected
#define _LIGHT_5 1, 3, 0 // scale factor = 1+5/8	alpha will not be affected
#define _LIGHT_6 1, 2, 0 // scale factor = 1+6/8	alpha will not be affected
#define _LIGHT_7 1, 1, 0 // scale factor = 1+7/8	alpha will not be affected --> the LIGHTEST

#define _A_DARK_7 0, 7, 1 // scale factor = 1/8	alpha will be affected
#define _A_DARK_6 0, 6, 1 // scale factor = 2/8	alpha will be affected
#define _A_DARK_5 0, 5, 1 // scale factor = 3/8	alpha will be affected
#define _A_DARK_4 0, 4, 1 // scale factor = 4/8	alpha will be affected
#define _A_DARK_3 0, 3, 1 // scale factor = 5/8	alpha will be affected
#define _A_DARK_2 0, 2, 1 // scale factor = 6/8	alpha will be affected
#define _A_DARK_1 0, 1, 1 // scale factor = 7/8	alpha will be affected

#define _A_LIGHT_1 1, 7, 1 // scale factor = 1+1/8	alpha will be affected
#define _A_LIGHT_2 1, 6, 1 // scale factor = 1+2/8	alpha will be affected
#define _A_LIGHT_3 1, 5, 1 // scale factor = 1+3/8	alpha will be affected
#define _A_LIGHT_4 1, 4, 1 // scale factor = 1+4/8	alpha will be affected
#define _A_LIGHT_5 1, 3, 1 // scale factor = 1+5/8	alpha will be affected
#define _A_LIGHT_6 1, 2, 1 // scale factor = 1+6/8	alpha will be affected
#define _A_LIGHT_7 1, 1, 1 // scale factor = 1+7/8	alpha will be affected

    // +-------------------------------------------------------------------------------------------------------------------------------------------------------------
    // |
    // |
    // |	NCOLOR PALETTE functions:
    // |
    // |
    NCOLOR *NBuildColorPaletteFromColor(NCOLOR *pcolor_plt, const Nu32 ncolor_plt_size, const NCOLOR *pcol_src, const Nu32 colsrc_src_size, const Nu32 ncolor_id0);
    NCOLOR *NExtractColor(NCOLOR *pcol, const NCOLOR *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);
    NCOLOR *NExtractModulatedColor(NCOLOR *pcol, const NCOLOR *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);

    // RGBA PALETTE functions:
    Nu32 *NBuildRGBAPaletteFromRGBA(Nu32 *prgba_plt, const Nu32 rgba_plt_size, const Nu32 *prgba_src, const Nu32 rgba_src_size, const Nu32 rgba_id0);
    Nu32 NExtractRGBA(const Nu32 *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);
    Nu32 NExtractModulatedRGBA(const Nu32 *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);

    // CROSS
    NCOLOR *NBuildColorPaletteFromRGBA(NCOLOR *pcolor_plt, const Nu32 ncolor_plt_size, const Nu32 *prgba_src, const Nu32 rgba_src_size, const Nu32 ncolor_id0);
    NCOLOR *NExtractColorFromRGBA(NCOLOR *pcol, const Nu32 *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);
    NCOLOR *NExtractModulatedColorFromRGBA(NCOLOR *pcol, const Nu32 *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);

    Nu32 *NBuildRGBAPaletteFromColor(Nu32 *prgba_plt, const Nu32 rgba_plt_size, const NCOLOR *pcol_src, const Nu32 colsrc_src_size, const Nu32 rgba_id0);
    Nu32 NExtractRGBAFromColor(const NCOLOR *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);
    Nu32 NExtractModulatedRGBAFromColor(const NCOLOR *pcp, const Nu8 pcp_size, const Nu8 pck_id, const NCOLORPICKPACK pickpack);
// |
// +-------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NCOLOR_GRADIENT_H
