#include "../../NCStandard.h"
#include "../../NCoordinateSystemConversions.h"
#include "../../NErrorHandling.h"
#include "../../Geometry/Components/NGeometryVertex.h"
#include "../../Maths/NMatrix3x3.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

/*
extern NUT_DRAW_CALLBACK		NUT_DrawShapeCallBack_LUT[];
extern NARRAY					NUT_DrawingsList[];
extern NSTATESET				NCurrentStateSet;
extern NACCUMULATOR				NAccumulator[];
*/
extern NGEOMETRY				*NUT_pCurrentDrawableShapeGeometry;
extern NGEOMETRY				NUT_DrawableShapeGeometry[]; // 0 means 3D, 1 means ORTHO
extern NMATRIX3x3				NUT_DrawOrthonormal;

#define CONSTANT_NUT_DRAWTEXT_J_LENGTH				NCoord_NormToHRel_Y(0.25f)
#define CONSTANT_NUT_DRAWTEXT_I_LENGTH				NCoord_NormToHRel_Y(0.25f)
#define DEFAULT_NUT_DRAWTEXT_CONSTRUCTION_PLANE		_PLANE_XY
#define DEFAULT_NUT_DRAWTEXT_SIZE					0.02f

Nu32	NUT_DrawTextChar[255];

void NUT_Initialize_DrawTextCharTable()
{
	//	Pattern (Hexdecimal)
	//
	//	D	E	F
	//
	//	A	B	C	
	//
	//	7	8	9
	//
	//	4	5	6
	//
	//	1	2	3

	//							0x00000000
	NUT_DrawTextChar[32]	=	0x00000000;	// Space
	NUT_DrawTextChar[33]	=	0x0002508E;	// !
	NUT_DrawTextChar[34]	=	0x0008E07D;	// "
	//NUT_DrawTextChar[35]	=	0x00000000;	// #
	//NUT_DrawTextChar[36]	=	0x00000000;	// $
	NUT_DrawTextChar[37]	=	0x360AD01F;	// %
	NUT_DrawTextChar[38]	=	0x0003DF12;	// &
	NUT_DrawTextChar[39]	=	0x000000BE;	// '
	NUT_DrawTextChar[40]	=	0x000035BF;	// (
	NUT_DrawTextChar[41]	=	0x000015BD;	// )
	NUT_DrawTextChar[42]	=	0x6A04C097;	// *
	NUT_DrawTextChar[43]	=	0x0009705B;	// +
	NUT_DrawTextChar[44]	=	0x00000015;	// ,
	NUT_DrawTextChar[45]	=	0x00000097;	// -
	NUT_DrawTextChar[46]	=	0x00000025;	// .
	NUT_DrawTextChar[47]	=	0x0000001F;	// /
	NUT_DrawTextChar[48]	=	0x000D13FD;	// 0
	NUT_DrawTextChar[49]	=	0x0000002E;	// 1
	NUT_DrawTextChar[50]	=	0x003179FD;	// 2
	NUT_DrawTextChar[51]	=	0x079013FD;	// 3
	NUT_DrawTextChar[52]	=	0x0028064D;	// 4
	NUT_DrawTextChar[53]	=	0x001397DF;	// 5
	NUT_DrawTextChar[54]	=	0x007931DF;	// 6
	NUT_DrawTextChar[55]	=	0x000001FD;	// 7
	NUT_DrawTextChar[56]	=	0x09713FD1;	// 8
	NUT_DrawTextChar[57]	=	0x0013FD79;	// 9
	NUT_DrawTextChar[58]	=	0x0002508B;	// :
	NUT_DrawTextChar[59]	=	0x0001508B;	// ;
	NUT_DrawTextChar[60]	=	0x0000037F;	// <
	NUT_DrawTextChar[61]	=	0x00064097;	// =
	NUT_DrawTextChar[62]	=	0x0000019D;	// >
	NUT_DrawTextChar[63]	=	0x31058CEA;	// ?
	NUT_DrawTextChar[64]	=	0x31DF6589;	// @
	//														0x00000000
	NUT_DrawTextChar[65]	=	NUT_DrawTextChar[97]	=	0x00793FD1;	// A a
	NUT_DrawTextChar[66]	=	NUT_DrawTextChar[98]	=	0xDFC7931D;	// B b
	NUT_DrawTextChar[67]	=	NUT_DrawTextChar[99]	=	0x000031DF;	// C c
	NUT_DrawTextChar[68]	=	NUT_DrawTextChar[100]	=	0x000D13CD;	// D d
	NUT_DrawTextChar[69]	=	NUT_DrawTextChar[101]	=	0x097031DF;	// E e
	NUT_DrawTextChar[70]	=	NUT_DrawTextChar[102]	=	0x009701DF;	// F f
	NUT_DrawTextChar[71]	=	NUT_DrawTextChar[103]	=	0x00FD1398;	// G g
	NUT_DrawTextChar[72]	=	NUT_DrawTextChar[104]	=	0x003F971D;	// H h
	NUT_DrawTextChar[73]	=	NUT_DrawTextChar[105]	=	0x0000002E;	// I i
	NUT_DrawTextChar[74]	=	NUT_DrawTextChar[106]	=	0x0000F314;	// J j
	NUT_DrawTextChar[75]	=	NUT_DrawTextChar[107]	=	0x0037F01D;	// K k
	NUT_DrawTextChar[76]	=	NUT_DrawTextChar[108]	=	0x0000031D;	// L l
	NUT_DrawTextChar[77]	=	NUT_DrawTextChar[109]	=	0x0003FBD1;	// M m
	NUT_DrawTextChar[78]	=	NUT_DrawTextChar[110]	=	0x0000F3D1;	// N n
	NUT_DrawTextChar[79]	=	NUT_DrawTextChar[111]	=	0x000D13FD;	// O o
	NUT_DrawTextChar[80]	=	NUT_DrawTextChar[112]	=	0x0001DF97;	// P p
	NUT_DrawTextChar[81]	=	NUT_DrawTextChar[113]	=	0x35013FD1;	// Q q
	NUT_DrawTextChar[82]	=	NUT_DrawTextChar[114]	=	0x00379FD1;	// R r
	NUT_DrawTextChar[83]	=	NUT_DrawTextChar[115]	=	0x00426AEC;	// S s
	NUT_DrawTextChar[84]	=	NUT_DrawTextChar[116]	=	0x0002E0FD;	// T t
	NUT_DrawTextChar[85]	=	NUT_DrawTextChar[117]	=	0x0000F31D;	// U u
	NUT_DrawTextChar[86]	=	NUT_DrawTextChar[118]	=	0x00000F2D;	// V v
	NUT_DrawTextChar[87]	=	NUT_DrawTextChar[119]	=	0x000F351D;	// W w
	NUT_DrawTextChar[88]	=	NUT_DrawTextChar[120]	=	0x000F103D;	// X x
	NUT_DrawTextChar[89]	=	NUT_DrawTextChar[121]	=	0x0000D8F1;	// Y y
	NUT_DrawTextChar[90]	=	NUT_DrawTextChar[122]	=	0x000031FD;	// Z z


}

void _setup_vector_typo_pattern(NVEC3 *pattern, Nf32 size)
{
	// !! This function assume 'pattern' is a table with 17 NVEC3 included !!!
	//
	//	Pattern (DECIMAL)
	//
	//	13	14	15
	//
	//	10	11	12	
	//
	//	07	08	09
	//
	//	04	05	06
	//
	//	01	02	03		00
	//	.
	//  .
	//	.
	//	.
	//	16
	//
	//	00 exists too, it stores the relative position of the next character.
	//	16 exists too, it stores the relative position of the next line.
	NVEC3	i,j;	
	Nf32		mi,mj;

	NVec3Scale(&i,&NUT_DrawOrthonormal.Side,CONSTANT_NUT_DRAWTEXT_I_LENGTH*size);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,CONSTANT_NUT_DRAWTEXT_J_LENGTH*size);
	
/*
	switch(construction_plane)
	{
		case _PLANE_XY:
			NVec3Set(&i,CONSTANT_NUT_DRAWTEXT_I_LENGTH*size,0.0f,0.0f);
			NVec3Set(&j,0.0f,CONSTANT_NUT_DRAWTEXT_J_LENGTH*size,0.0f);
			break;

		case _PLANE_XZ:
			NVec3Set(&i,CONSTANT_NUT_DRAWTEXT_I_LENGTH*size,0.0f,0.0f);
			NVec3Set(&j,0.0f,0.0f,CONSTANT_NUT_DRAWTEXT_J_LENGTH*size);
			break;

		case _PLANE_ZY:
			NVec3Set(&i,0.0f,0.0f,CONSTANT_NUT_DRAWTEXT_I_LENGTH*size);
			NVec3Set(&j,0.0f,CONSTANT_NUT_DRAWTEXT_J_LENGTH*size,0.0f);
			break;

		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			break;
	}
*/
	// 00 ... Next character Position
	pattern[0].x = 3.0f*i.x;
	pattern[0].y = 3.0f*i.y;
	pattern[0].z = 3.0f*i.z;
	// 16 ... Next Line Position
	pattern[16].x = -5.0f*j.x;
	pattern[16].y = -5.0f*j.y;
	pattern[16].z = -5.0f*j.z;

	for(Nu32 a=0;a<15;a++)
	{
		mi = (Nf32)(a%3);
		mj = (Nf32)(a/3);
		pattern[a+1].x = mi*i.x + mj*j.x;
		pattern[a+1].y = mi*i.y + mj*j.y;
		pattern[a+1].z = mi*i.z + mj*j.z;
	}
}

		

void NUT_Draw_Text(const Nchar *pstring, const NVEC3 *pposition,const NUT_DRAWTEXT *pparams )
{
// One character		( each 'plot" is a fixed point. The function is going to connect some of them with line to draw a character )
// ---------------
//	13	14	15
//
//	10	11	12	
//
//	07	08	09
//
//	04	05	06
//
//	01	02	03	00
//	.
//  .
//	.
//	.
//	16

// Each gap between 2 'plots' as a size of drawtext.size * CONSTANT_NUT_DRAWTEXT_I_LENGTH for horizontal ones and drawtext.size * CONSTANT_NUT_DRAWTEXT_J_LENGTH for vertical ones
// So,	the char boundingbox as a size  of,
//											width:		2 * drawtext.size * CONSTANT_NUT_DRAWTEXT_I_LENGTH
//											height:		4 * drawtext.size * CONSTANT_NUT_DRAWTEXT_J_LENGTH
//		the separation dist. between 2 char is,
//											separation: 1 * drawtext.size * CONSTANT_NUT_DRAWTEXT_I_LENGTH
//		the separation dist. between 2 lines is,
//											interline:	5 * drawtext.size * CONSTANT_NUT_DRAWTEXT_J_LENGTH

	NErrorIf(!pposition,NERROR_NULL_POINTER);
	NErrorIf(!pstring,NERROR_NULL_POINTER);
	
	Nu32 len = (Nu32)strlen(pstring);
	if(!len)
		return;

	NVEC3		pattern[17];
	NUTDRAWVERTEX	v;

	// Setup
	if(pparams)
	{
		_setup_vector_typo_pattern(pattern,pparams->Size);
		v.Color0_4f = pparams->Color;
	}
	else // default value
	{
		_setup_vector_typo_pattern(pattern,DEFAULT_NUT_DRAWTEXT_SIZE);
		NSetColorf(&v.Color0_4f,NCOLOR_PRESET3F_WHITE,1.0f);
	}


	Nchar			*pstr	= (Nchar*)pstring;
	NVEC3		linestart = *pposition;
	NVEC3		cursor	= linestart;
	Nu32			charcode;
	Nu32			i,j;
	Nu32			get4bits,prevget4bits = 0;

//	NUT_pCurrentDrawableShapeGeometry = &NUT_DrawableShapeGeometry[NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC];

	for(i=len;i!=0;i--,pstr++)
	{
		// LINE FEED
		if( *pstr =='\n')
		{
			linestart.x += pattern[16].x;
			linestart.y += pattern[16].y;
			linestart.z += pattern[16].z;

			cursor = linestart;
		}
		else // CHAR BUILD
		{
			// Read and interpret the char code ( 8 X 4 bits )
			charcode = NUT_DrawTextChar[*pstr];
			// first
			prevget4bits = get4bits = charcode & 0xF;
			v.Position_3f.x = cursor.x + pattern[get4bits].x;
			v.Position_3f.y = cursor.y + pattern[get4bits].y;
			v.Position_3f.z = cursor.z + pattern[get4bits].z;

			// DRAW ONE CHARACTER:
			NUT_DrawPencil_From(&v);
			for(j=1;j<8;j++)
			{
				get4bits = (charcode >> (j*4)) & 0xF;
				if(get4bits)
				{
					v.Position_3f.x = cursor.x + pattern[get4bits].x;
					v.Position_3f.y = cursor.y + pattern[get4bits].y;
					v.Position_3f.z = cursor.z + pattern[get4bits].z;
					NUT_DrawPencil_LineTo(&v);
				}
				else
				{
					j+=1;
					get4bits = (charcode >> (j*4)) & 0xF;
					if(get4bits)
					{
						v.Position_3f.x = cursor.x + pattern[get4bits].x;
						v.Position_3f.y = cursor.y + pattern[get4bits].y;
						v.Position_3f.z = cursor.z + pattern[get4bits].z;
						NUT_DrawPencil_From(&v);
					}
					else
						break;
				}
			}

			// Move Cursor to next character position
			cursor.x += pattern[0].x;
			cursor.y += pattern[0].y;
			cursor.z += pattern[0].z;
		}
	}
}

Nf32 NUT_GetHighestDrawTextCharHeight(const NUT_DRAWTEXT *pparams)
{
	return pparams ? pparams->Size : DEFAULT_NUT_DRAWTEXT_SIZE;
	//with 1.0f = CONSTANT_NUT_DRAWTEXT_J_LENGTH*4.0f and CONSTANT_NUT_DRAWTEXT_J_LENGTH = 1/4 !
}
Nf32 NUT_GetLargestDrawTextCharWidth(const NUT_DRAWTEXT *pparams)
{
	return pparams ? pparams->Size*0.5f: DEFAULT_NUT_DRAWTEXT_SIZE * 0.5f;
	// with 0.5f = CONSTANT_NUT_DRAWTEXT_I_LENGTH*2.0f and CONSTANT_NUT_DRAWTEXT_I_LENGTH = 1/4 !
}

Nf32 NUT_EstimateDrawTextCharLineLength(const Nu32 len, const NUT_DRAWTEXT *pparams)
{
	if (!len)
		return 0.0f;
	else
		return pparams?(3.0f*CONSTANT_NUT_DRAWTEXT_I_LENGTH*pparams->Size)*(len - 1) + pparams->Size*0.5f: (3.0f*CONSTANT_NUT_DRAWTEXT_I_LENGTH*DEFAULT_NUT_DRAWTEXT_SIZE)*(len - 1) + DEFAULT_NUT_DRAWTEXT_SIZE *0.5f;
}

Nf32 NUT_EstimateDrawTextCharLinesHeight(const Nu32 line_nb, const NUT_DRAWTEXT *pparams)
{
	if (!line_nb)
		return 0.0f;
	else
		return pparams ? (5.0f*CONSTANT_NUT_DRAWTEXT_J_LENGTH*pparams->Size)*(line_nb - 1) + pparams->Size : (5.0f*CONSTANT_NUT_DRAWTEXT_J_LENGTH*DEFAULT_NUT_DRAWTEXT_SIZE)*(line_nb - 1) + DEFAULT_NUT_DRAWTEXT_SIZE;

}
