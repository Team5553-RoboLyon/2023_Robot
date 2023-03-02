#include "../N/NString.h"
#include "NL2DOrthogonalCoordinateSystem.h"


#ifdef _NEDITOR
// -----------------------------------------------------------------------
#endif	
/*
#define _updateOrigin(o,bb,flgs)		Nf32 a = (Nf32)_GET_NL2DOCS_ORIGIN_RIGHT(flgs);		\
										Nf32 b = (Nf32)_GET_NL2DOCS_ORIGIN_LEFT(flgs);			\
										o.x = (bb.right + bb.left)  * (1.0f + a-b) / 2.0f;		\
										a = (Nf32)_GET_NL2DOCS_ORIGIN_BOTTOM(flgs);			\
										b = (Nf32)_GET_NL2DOCS_ORIGIN_TOP(flgs);				\
										o.y = (bb.top + bb.bottom)  * (1.0f + a-b) / 2.0f;		
*/
/*
NL2DOCS::NL2DOCS()
{
	m_Flags = BITS_NL2DOCS_ORIGIN_CENTER;

	m_BoundingBox.left		=  0.0f;
	m_BoundingBox.bottom	= -1.0f;
	m_BoundingBox.right		=  1.0f;
	m_BoundingBox.top		=  0.0f;

	updateOrigin();

	m_Size.x = m_BoundingBox.right - m_BoundingBox.left;
	m_Size.y = m_BoundingBox.top - m_BoundingBox.bottom;

	m_Unit.x = 1.0f;
	m_Unit.y = 1.0f;

	m_UnitCount.x = m_Size.x / m_Unit.x;
	m_UnitCount.y = m_Size.y / m_Unit.y;

	m_pCaption = NULL;

	NSetColorf(&m_CaptionParam.Color, NCOLOR_PRESET3F_SYSTEM,1);
	m_CaptionParam.Size = NPRESET1F_DRAWTEXT_ORTHOGRAPHIC_SIZE_SMALL;
}
*/

void NL2DOCS::setup(const Nchar *pcaption, const Nu32 bits_nl_2docs_origin, const NRECTf32 * pbb, const NVEC2f32 *punit)
{
	m_Flags = bits_nl_2docs_origin & MASK_SHIFTED_2DOCS_ORIGIN;

	if (pbb)
	{
		NErrorIf(pbb->left > pbb->right, NERROR_INCONSISTENT_VALUE);
		NErrorIf(pbb->bottom > pbb->top, NERROR_INCONSISTENT_VALUE);
		m_BoundingBox = *pbb;
	}
	else
	{
		m_BoundingBox.left		= 0.0f;
		m_BoundingBox.bottom	= -1.0f;
		m_BoundingBox.right		= 1.0f;
		m_BoundingBox.top		= 0.0f;
	}


	updateOrigin();

	m_Size.x		= m_BoundingBox.right - m_BoundingBox.left;
	m_Size.y		= m_BoundingBox.top - m_BoundingBox.bottom;

	if (punit)
	{
		NErrorIf(punit->x <= 0.0f, NERROR_INCONSISTENT_VALUE);
		NErrorIf(punit->y <= 0.0f, NERROR_INCONSISTENT_VALUE);
		m_Unit = *punit;
	}
	else
	{
		m_Unit.x	= 1.0f;
		m_Unit.y	= 1.0f;
	}

	m_UnitCount.x	= m_Size.x / m_Unit.x;
	m_UnitCount.y	= m_Size.y / m_Unit.y;

	if (pcaption)
		m_pCaption = NStrDuplicate(pcaption);

	NSetColorf(&m_CaptionParam.Color, NCOLOR_PRESET3F_SYSTEM, 1);
	m_CaptionParam.Size = NPRESET1F_DRAWTEXT_ORTHOGRAPHIC_SIZE_SMALL;

	Nu32 i;
	for (i = 0; i < NL2DOCS_SUBSCALE_ENUM::NL2DOCS_SUBSCALE_ENUM_SIZE; i++)
	{
		m_subScale[i].x = m_subScale[i].y = 1.0f;
	}
	//																							| should be used by						| will be default color for ... ( null PICKPACK case )	
	// Default color setup															------------|---------------------------------------|-------------------------------------------------------			
	NSetColorf(&m_color[0],		NCOLOR_PRESET3F_RED_CRIMSON, 1);				//	RED			|	axis,								|	axis,	
	NSetColorf(&m_color[1],		NCOLOR_PRESET3F_GREEN_SPRING, 1);				//	GREEN		|										|
	NSetColorf(&m_color[2],		NCOLOR_PRESET3F_BLUE_NAVY, 1);					//	BLUE		|										|
	NSetColorf(&m_color[3],		NCOLOR_PRESET3F_YELLOW, 1);						//	YELLOW		|										|
	NSetColorf(&m_color[4],		NCOLOR_PRESET3F_PINK_PARADISE, 1);				//	PINK		| kin.j<0										|
	NSetColorf(&m_color[5],		NCOLOR_PRESET3F_ORANGE_GOLD, 1);				//	ORANGE		| kin.j=0,								|
	NSetColorf(&m_color[6],		NCOLOR_PRESET3F_GREEN_PERSIAN, 1);				//	GREEN#2		| kin.j>0										|
	NSetColorf(&m_color[7],		NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[8],		NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[9],		NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[10],	NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[11],	NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[12],	NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[13],	NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[14],	NCOLOR_PRESET3F_BLUE_PERSIAN, 1);				// BLUE PERSIAN	|	floor,
	NSetColorf(&m_color[15],	NCOLOR_PRESET3F_BLUE_TURQUOISE, 1);				// TURQUOISE	|	Infos, Ceil,

	/*
		for (i = 0; i < NL2DOCS_COLOR_ENUM::NL2DOCS_COLOR_TABLE_SIZE; i++)
		{
			NSetColorf(&m_color[i], NCOLOR_PRESET3F_PINK_ORCHID, 1.0f);
		}
	*/
	
	// Default coord styles setup
	for (i = NL2DOCS_COORDS_STYLE_0; i < NL2DOCS_COORDS_STYLE_ENUM::NL2DOCS_COORDS_STYLE_ENUM_SIZE; i++)
	{
		m_coordsStyle[i] = 0;
		/*
		setCoordsDigits((NL2DOCS_COORDS_STYLE_ENUM)i, 2);
		showCoordsLine((NL2DOCS_COORDS_STYLE_ENUM)i);
		showCoordsValue((NL2DOCS_COORDS_STYLE_ENUM)i);
		setCoordsColor((NL2DOCS_COORDS_STYLE_ENUM)i, 3);
		*/
	}
	NSetupArray(&m_coordsArray, 8, sizeof(NL2DOCS_COORD));
}

NL2DOCS::~NL2DOCS()
{
	if (m_pCaption)
	{
		Nfree(m_pCaption);
		m_pCaption = NULL;
	}
	NClearArray(&m_coordsArray, NULL);
}

void NL2DOCS::setBoundingBoxKeepUnitCount(const NRECTf32 * pbb)
{
	if (pbb)
	{
		NErrorIf(pbb->left		> pbb->right, NERROR_INCONSISTENT_VALUE);
		NErrorIf(pbb->bottom	> pbb->top, NERROR_INCONSISTENT_VALUE);
		m_BoundingBox = *pbb;
	}
	else
	{
		m_BoundingBox.left = 0.0f;
		m_BoundingBox.bottom = -1.0f;
		m_BoundingBox.right = 1.0f;
		m_BoundingBox.top = 0.0f;
	}
	
	updateOrigin();

	m_Size.x	= m_BoundingBox.right	- m_BoundingBox.left;
	m_Size.y	= m_BoundingBox.top	- m_BoundingBox.bottom;

	NErrorIf(m_UnitCount.x <= 0.0f, NERROR_INCONSISTENT_VALUE);
	NErrorIf(m_UnitCount.y <= 0.0f, NERROR_INCONSISTENT_VALUE);

	m_Unit.x = m_Size.x / m_UnitCount.x;
	m_Unit.y = m_Size.y / m_UnitCount.y;
}

void NL2DOCS::setBoundingBoxKeepUnitCount(const Nf32 left, const Nf32 bottom, const Nf32 right, const Nf32 top)
{
	NErrorIf(left > right, NERROR_INCONSISTENT_VALUE);
	NErrorIf(bottom > top, NERROR_INCONSISTENT_VALUE);

	m_BoundingBox.left		= left;
	m_BoundingBox.bottom	= bottom;
	m_BoundingBox.right		= right;
	m_BoundingBox.top		= top;

	updateOrigin();

	m_Size.x = m_BoundingBox.right - m_BoundingBox.left;
	m_Size.y = m_BoundingBox.top - m_BoundingBox.bottom;

	NErrorIf(m_UnitCount.x <= 0.0f, NERROR_INCONSISTENT_VALUE);
	NErrorIf(m_UnitCount.y <= 0.0f, NERROR_INCONSISTENT_VALUE);

	m_Unit.x = m_Size.x / m_UnitCount.x;
	m_Unit.y = m_Size.y / m_UnitCount.y;
}

void NL2DOCS::setBoundingBoxKeepUnitSize(const NRECTf32 * pbb)
{
	if (pbb)
	{
		NErrorIf(pbb->left > pbb->right, NERROR_INCONSISTENT_VALUE);
		NErrorIf(pbb->bottom > pbb->top, NERROR_INCONSISTENT_VALUE);
		m_BoundingBox = *pbb;
	}
	else
	{
		m_BoundingBox.left = 0.0f;
		m_BoundingBox.bottom = -1.0f;
		m_BoundingBox.right = 1.0f;
		m_BoundingBox.top = 0.0f;
	}

	updateOrigin();

	m_Size.x = m_BoundingBox.right - m_BoundingBox.left;
	m_Size.y = m_BoundingBox.top - m_BoundingBox.bottom;

	m_UnitCount.x = m_Size.x / m_Unit.x;
	m_UnitCount.y = m_Size.y / m_Unit.y;
}

void NL2DOCS::setBoundingBoxKeepUnitSize(const Nf32 left, const Nf32 bottom, const Nf32 right, const Nf32 top)
{
	NErrorIf(left > right, NERROR_INCONSISTENT_VALUE);
	NErrorIf(bottom > top, NERROR_INCONSISTENT_VALUE);

	m_BoundingBox.left = left;
	m_BoundingBox.bottom = bottom;
	m_BoundingBox.right = right;
	m_BoundingBox.top = top;

	updateOrigin();

	m_Size.x = m_BoundingBox.right - m_BoundingBox.left;
	m_Size.y = m_BoundingBox.top - m_BoundingBox.bottom;

	m_UnitCount.x = m_Size.x / m_Unit.x;
	m_UnitCount.y = m_Size.y / m_Unit.y;
}


void NL2DOCS::setOrigin(const Nu32 bits_nl_2docs_origin)
{
	m_Flags = (m_Flags&(~(MASK_SHIFTED_2DOCS_ORIGIN))) | (bits_nl_2docs_origin&MASK_SHIFTED_2DOCS_ORIGIN);

	updateOrigin();
}

Nf32 NL2DOCS::transformX(const Nf32 x, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = 0;
		pcoord->m_source = x;
		pcoord->m_transformed = m_Origin.x + x * m_Unit.x;

		pcoord->m_flags = 0; // car il s'agit d'une coord X et pas Y !  FLAG_NL2DOCS_COORD_IS_Y doit être OFF;
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return  m_Origin.x + x * m_Unit.x;
}

Nf32 NL2DOCS::transformY(const Nf32 y, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = 0;
		pcoord->m_source = y;
		pcoord->m_transformed = m_Origin.y + y * m_Unit.y;
		FLAG_ON(pcoord->m_flags, FLAG_NL2DOCS_COORD_IS_Y); // FLAG_NL2DOCS_COORD_IS_Y doit être ON ...
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.y + y * m_Unit.y;

}
Nf32 NL2DOCS::transformX(const Nf32 x, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id )
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = 0;
		pcoord->m_source = x;
		pcoord->m_transformed = m_Origin.x + x * m_subScale[sub].x * m_Unit.x;
		pcoord->m_flags = 0; // car il s'agit d'une coord X et pas Y !  FLAG_NL2DOCS_COORD_IS_Y doit être OFF;
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.x + x * m_subScale[sub].x * m_Unit.x;
}

Nf32 NL2DOCS::transformY(const Nf32 y, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = 0;
		pcoord->m_source = y;
		pcoord->m_transformed = m_Origin.y + y * m_subScale[sub].y * m_Unit.y;
		pcoord->m_flags = FLAG_NL2DOCS_COORD_IS_Y; // FLAG_NL2DOCS_COORD_IS_Y doit être ON et comme pour le moment c'est le seul flag existant pour NL2DOCS_COORD.m_flags ...
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.y + y * m_subScale[sub].y * m_Unit.y;
}

Nf32 NL2DOCS::transformX(const Nu8 index, const Nf32 x, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = index;
		pcoord->m_source = x;
		pcoord->m_transformed = m_Origin.x + x * m_Unit.x;
		pcoord->m_flags = 0; // car il s'agit d'une coord X et pas Y !  FLAG_NL2DOCS_COORD_IS_Y doit être OFF;
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.x + x * m_Unit.x;
}

Nf32 NL2DOCS::transformY(const Nu8 index, const Nf32 y, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{

		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = index;
		pcoord->m_source = y;
		pcoord->m_transformed = m_Origin.y + y * m_Unit.y;
		FLAG_ON(pcoord->m_flags, FLAG_NL2DOCS_COORD_IS_Y); // FLAG_NL2DOCS_COORD_IS_Y doit être ON ...
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.y + y * m_Unit.y;
}

Nf32 NL2DOCS::transformX(const Nu8 index, const Nf32 x, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = index;
		pcoord->m_source = x;
		pcoord->m_transformed = m_Origin.x + x * m_subScale[sub].x * m_Unit.x;
		pcoord->m_flags = 0; // car il s'agit d'une coord X et pas Y !  FLAG_NL2DOCS_COORD_IS_Y doit être OFF;
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.x + x * m_subScale[sub].x * m_Unit.x;
}

Nf32 NL2DOCS::transformY(const Nu8 index, const Nf32 y, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id)
{
	if (ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
	{
		NL2DOCS_COORD* pcoord = (NL2DOCS_COORD*)NArrayAllocBack(&m_coordsArray);
		pcoord->m_index = index;
		pcoord->m_source = y;
		pcoord->m_transformed = m_Origin.y + y * m_subScale[sub].y * m_Unit.y;
		pcoord->m_flags = FLAG_NL2DOCS_COORD_IS_Y; // FLAG_NL2DOCS_COORD_IS_Y doit être ON et comme pour le moment c'est le seul flag existant pour NL2DOCS_COORD.m_flags ...
		pcoord->m_coordsStyle = m_coordsStyle[coord_style_id];
		return pcoord->m_transformed;
	}
	else
		return m_Origin.y + y * m_subScale[sub].y * m_Unit.y;
}

void NL2DOCS::draw(const NCOLORPICKPACK pickpack)
{
	NVEC3f32		A, B;
	A.z = B.z = 0.0f;
	//
	// COORDS ?
	//
	// Si au moins un groupe est visible alors on trace sinon, on entre même pas dans la boucle
	if (m_coordsArray.Size)
	{
		Nchar format[16];
		Nchar val_txt[32];
		Nchar all_txt[48];

		Nf32  tick_size = defaultAxisArrowSize()*0.25f;
		NUT_DRAWTEXT	drawtxtparam = m_CaptionParam;
		//NUTDRAWVERTEX	va;
		//va.Position_3f.z = 0.0f;

		Nf32	prev_text_x		= -NF32_MAX;
		Nf32	prev_line_len	=  0.0f;
		Nf32	line_len		= 0;

		Nu32	digit_nb = NVOID; // pour forcer l'initialisation au premier passage 

		NL2DOCS_COORD *pcoord = (NL2DOCS_COORD*)m_coordsArray.pFirst;
		for (Nu32 i = 0; i < m_coordsArray.Size; i++, pcoord++)
		{
			drawtxtparam.Color = m_color[GET_NL2DOCS_COORDS_STYLE_COLOR(pcoord->m_coordsStyle)];


			// AFFICHAGE TEXTES
			// ----------------
			//
			// 1) PREPARATION
			//
			all_txt[0]		= 0;
			val_txt[0]		= 0;
			prev_line_len	= line_len;
			line_len		= 0;

			// 1.1) Preparation index
			if (ISFLAG_ON(pcoord->m_coordsStyle, FLAG_NL2DOCS_COORDS_STYLE_INDEX_DISPLAY))
			{
				sprintf(all_txt,"%d ", pcoord->m_index );
			}
			// 1.1) Preparation valeurs
			if (ISFLAG_ON(pcoord->m_coordsStyle, FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY))
			{
				if (GET_NL2DOCS_COORDS_STYLE_DIGITS(pcoord->m_coordsStyle) != digit_nb)
				{
					digit_nb = GET_NL2DOCS_COORDS_STYLE_DIGITS(pcoord->m_coordsStyle);
					sprintf(format, "%s%d%s", "%.", digit_nb , "f");
				}
				sprintf(val_txt, format, pcoord->m_source);
				strcat(all_txt, val_txt);
			}
			//
			// 2) Affichage
			//
			// On affiche seulement si il y a quelquechose à afficher ...
			if (line_len = (Nf32)strlen(all_txt))
			{
				line_len = NUT_EstimateDrawTextCharLineLength(strlen(all_txt) + 1, &drawtxtparam);

				// On affiche le long de l'axe vertical ?
				if (ISFLAG_ON(pcoord->m_flags, FLAG_NL2DOCS_COORD_IS_Y))
				{
					A.x = m_Origin.x - line_len;
					A.y = pcoord->m_transformed - tick_size;
				}
				// On affiche le long de l'axe horizontal
				else
				{
					A.x = pcoord->m_transformed;
					if ((prev_text_x + prev_line_len) > A.x)
						A.y = m_Origin.y - 2.0f * (m_CaptionParam.Size + tick_size);
					else
					{
						prev_text_x = A.x;
						A.y = m_Origin.y - m_CaptionParam.Size - tick_size;
					}
				}
				NUT_Draw_Text(all_txt, &A, &drawtxtparam);
			}

			// AFFICHAGE LIGNES
			// ----------------
			//
			if (ISFLAG_ON(pcoord->m_coordsStyle, FLAG_NL2DOCS_COORDS_STYLE_LINE_DISPLAY))
			{
				if (ISFLAG_ON(pcoord->m_flags, FLAG_NL2DOCS_COORD_IS_Y))
				{
					A.x = m_BoundingBox.left;
					B.x = m_BoundingBox.right;
					A.y = B.y = pcoord->m_transformed;
				}
				else
				{
					A.y = m_BoundingBox.bottom - tick_size*0.5f;
					B.y = m_BoundingBox.top;
					A.x = B.x = pcoord->m_transformed;
				}
			}
			else
			{
				if (ISFLAG_ON(pcoord->m_flags, FLAG_NL2DOCS_COORD_IS_Y))
				{
					A.x = m_Origin.x -tick_size;
					B.x = m_Origin.x + tick_size;
					A.y = B.y = pcoord->m_transformed;
				}
				else
				{
					A.y = m_Origin.y - tick_size * 0.5f;
					B.y = m_Origin.y + tick_size * 0.5f;
					A.x = B.x = pcoord->m_transformed;
				}
			}
			NUT_Draw_Segment(&A, &B, &drawtxtparam.Color);
		}
		// On vide le buffer !
		NResizeArray(&m_coordsArray, 0, NULL, NULL);
	}
	//
	// BASE
	//
	NUT_DRAW_ARROWS arrows;
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	arrows.Color = m_color[ NSTCPLT_GET_COLID_MSK(0, pickpack, NL2DOCS_COLOR_TABLE_ID_MSK) ];
	NUT_SetDrawArrows_TickSize(&arrows, BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE | FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE| FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE);
	arrows.AArrowSize = arrows.BArrowSize = defaultAxisArrowSize();
	NUT_SetDrawArrows_TickDensityMax(&arrows, 100);

	//	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT);

	// X Axis 
	if (_GET_NL2DOCS_ORIGIN_RIGHT(m_Flags) ^ _GET_NL2DOCS_ORIGIN_LEFT(m_Flags))
	{
		// "0 and 1" or "1 and 0"
		if (_GET_NL2DOCS_ORIGIN_RIGHT(m_Flags))
		{
			NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
		}
		else
		{
			NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
		}
	}
	else  
	{
		// "0 and 0" or "1 and 1"
		NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
		NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	}

	arrows.Unit = m_Unit.x;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_RIGHT);

	A.x = m_BoundingBox.left;
	A.y = m_Origin.y;
	B.x = m_BoundingBox.right;
	B.y = m_Origin.y;
	NUT_Draw_Arrows(&A, &B, &arrows);

	// Y Axis 
	/*
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	arrows.Color = m_color[NL2DOCS_COLOR_0];
	NUT_SetDrawArrows_TickSize(&arrows, BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE | FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE | FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE);
	arrows.AArrowSize = arrows.BArrowSize = defaultAxisArrowSize();
	NUT_SetDrawArrows_TickDensityMax(&arrows, 100);
	*/
	if (_GET_NL2DOCS_ORIGIN_TOP(m_Flags) ^ _GET_NL2DOCS_ORIGIN_BOTTOM(m_Flags))
	{
		// "0 and 1" or "1 and 0"
		if (_GET_NL2DOCS_ORIGIN_TOP(m_Flags))
		{
			NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
		}
		else
		{
			NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
		}
	}
	else
	{
		// "0 and 0" or "1 and 1"
		NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
		NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	}

	arrows.Unit = m_Unit.y;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_LEFT);

	A.x = m_Origin.x;
	A.y = m_BoundingBox.bottom;
	B.x = m_Origin.x;
	B.y = m_BoundingBox.top;
	NUT_Draw_Arrows(&A, &B, &arrows);

	//
	// CAPTION (Main)
	//
	if (m_pCaption)
	{
		A.x = m_BoundingBox.left	+ m_CaptionParam.Size *2.0f;
		A.y = m_BoundingBox.bottom	+ (m_BoundingBox.top - m_BoundingBox.bottom)*0.9f;
		A.z = 0.0f;

		NUT_Draw_Text(m_pCaption, &A, &m_CaptionParam);
	}

}

void NL2DOCS::drawBaseLine(const Nchar * pbaselinetxt)
{
	if (pbaselinetxt)
	{
		NVEC3f32		A;
		A.x = m_BoundingBox.left + m_CaptionParam.Size *2.0f;
		A.y = m_BoundingBox.bottom + (m_BoundingBox.top - m_BoundingBox.bottom)*0.9f - NUT_EstimateDrawTextCharLinesHeight(NStrGetLineCount(m_pCaption)+1, &m_CaptionParam);
		A.z = 0.0f;
		NUT_Draw_Text(pbaselinetxt, &A, &m_CaptionParam);
	}
}

/*
void NL2DOCS::drawAxis()
{
	NVEC3f32 A, B;

	NUT_DRAW_ARROWS arrows;
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	NSetColorf(&arrows.Color, DEFAULT_NL2DOCS_AXIS_COLOR, 1);
	NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
	NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE);
	arrows.AArrowSize = arrows.BArrowSize = defaultAxisArrowSize();
	NUT_SetDrawArrows_TickSize(&arrows, BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_FROM_CENTER);
//	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT);

	// X Axis 
	arrows.Unit = m_unit.x;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_RIGHT);

	A.x = m_origin.x - m_size.x * 0.5f;
	A.y = m_origin.y;
	A.z = 0.0f;
	B.x = m_origin.x + m_size.x * 0.5f;
	B.y = m_origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);

	// Y Axis 
	arrows.Unit = m_unit.y;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_LEFT);

	A.x = m_origin.x;
	A.y = m_origin.y - m_size.y * 0.5f;
	A.z = 0.0f;
	B.x = m_origin.x;
	B.y = m_origin.y + m_size.y * 0.5f;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);
}
void NL2DOCS::drawAxis(const NUT_DRAW_ARROWS *px, const NUT_DRAW_ARROWS *py)
{
	NVEC3f32 A, B;

	// X Axis 
	A.x = m_origin.x - m_size.x * 0.5f;
	A.y = m_origin.y;
	A.z = 0.0f;

	B.x = m_origin.x + m_size.x * 0.5f;
	B.y = m_origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, px);

	// Y Axis 
	A.x = m_origin.x;
	A.y = m_origin.y - m_size.y * 0.5f;
	A.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, py);

	B.x = m_origin.x;
	B.y = m_origin.y + m_size.y * 0.5f;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, py);
}

void NL2DOCS::drawPositiveAxis()
{
	NVEC3f32 A, B;

	NUT_DRAW_ARROWS arrows;
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	NSetColorf(&arrows.Color, DEFAULT_NL2DOCS_AXIS_COLOR, 1);
	NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
	NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE);
	arrows.BArrowSize = defaultAxisArrowSize();
	NUT_SetDrawArrows_TickSize(&arrows, BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON);
	// FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_FROM_CENTER);
	// FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT);

	A.x = m_Origin.x;
	A.y = m_Origin.y;
	A.z = 0.0f;

	// X+ Axis 
	arrows.Unit = m_Unit.x;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_RIGHT);

	B.x = m_Origin.x + m_Size.x;
	B.y = m_Origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);

	// Y+ Axis 
	arrows.Unit = m_Unit.y;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_LEFT);

	B.x = m_Origin.x;
	B.y = m_Origin.y + m_Size.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);
}

void NL2DOCS::drawPositiveAxis(const NUT_DRAW_ARROWS *px, const NUT_DRAW_ARROWS *py)
{
	NVEC3f32 A, B;
	A.x = m_Origin.x;
	A.y = m_Origin.y;
	A.z = 0.0f;

	// X+ Axis 
	B.x = m_Origin.x + m_Size.x;
	B.y = m_Origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, px);

	// Y+ Axis 
	B.x = m_Origin.x;
	B.y = m_Origin.y + m_Size.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, py);
}

void NL2DOCS::drawNegativeAxis()
{
	NVEC3f32 A, B;
	NUT_DRAW_ARROWS arrows;
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	NSetColorf(&arrows.Color, DEFAULT_NL2DOCS_AXIS_COLOR, 1);
	NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
	NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE);
	arrows.BArrowSize = defaultAxisArrowSize();
	NUT_SetDrawArrows_TickSize(&arrows, BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON);
	// FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_FROM_CENTER);
	// FLAG_ON(arrows.Flags, FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT);

	A.x = m_Origin.x;
	A.y = m_Origin.y;
	A.z = 0.0f;

	// X- Axis 
	arrows.Unit = m_Unit.x;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_RIGHT);

	B.x = m_Origin.x - m_Size.x;
	B.y = m_Origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);

	// Y- Axis 
	arrows.Unit = m_Unit.y;
	NUT_SetDrawArrows_Tick(&arrows, BITS_NUT_DRAW_ARROWS_TICK_LEFT);

	B.x = m_Origin.x;
	B.y = m_Origin.y - m_Size.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, &arrows);
}


void NL2DOCS::drawNegativeAxis(const NUT_DRAW_ARROWS *px, const NUT_DRAW_ARROWS *py)
{
	NVEC3f32 A, B;
	A.x = m_Origin.x;
	A.y = m_Origin.y;
	A.z = 0.0f;

	// X- Axis 
	B.x = m_Origin.x - m_Size.x;
	B.y = m_Origin.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, px);

	// Y- Axis 
	B.x = m_Origin.x;
	B.y = m_Origin.y - m_Size.y;
	B.z = 0.0f;
	NUT_Draw_Arrows(&A, &B, py);
}
*/
