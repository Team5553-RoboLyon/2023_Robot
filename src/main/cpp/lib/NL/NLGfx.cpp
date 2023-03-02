
#include "NLGfx.h"

void NLGFX::setup()
{
	// Default color setup														------------|---------------------------------------|-------------------------------------------------------			
	NSetColorf(&m_color[0], NCOLOR_PRESET3F_RED_CRAYOLA, 1);				//	RED			|	axis,								|	axis,	
	NSetColorf(&m_color[1], NCOLOR_PRESET3F_GREEN_SPRING, 1);				//	GREEN		|										|
	NSetColorf(&m_color[2], NCOLOR_PRESET3F_BLUE_NAVY, 1);					//	BLUE		|										|
	NSetColorf(&m_color[3], NCOLOR_PRESET3F_YELLOW_CANARI, 1);				//	YELLOW		|										|
	NSetColorf(&m_color[4], NCOLOR_PRESET3F_PINK_PARADISE, 1);				//	PINK		| kin.j<0								|
	NSetColorf(&m_color[5], NCOLOR_PRESET3F_ORANGE_GOLD, 1);				//	ORANGE		| kin.j=0,								|
	NSetColorf(&m_color[6], NCOLOR_PRESET3F_GREEN_PERSIAN, 1);				//	GREEN#2		| kin.j>0								|
	NSetColorf(&m_color[7], NCOLOR_PRESET3F_ORANGE_MANGO, 1);				//  ORANGE#2
	NSetColorf(&m_color[8], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[9], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[10], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[11], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[12], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[13], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&m_color[14], NCOLOR_PRESET3F_BLUE_PERSIAN, 1);				// BLUE PERSIAN	|	floor,
	NSetColorf(&m_color[15], NCOLOR_PRESET3F_BLUE_TURQUOISE, 1);			// TURQUOISE	|	Infos, Ceil,

	// Default Sizes
	m_size[NLGFX_SZ_2D_S]			= 0.01f;
	m_size[NLGFX_SZ_2D_M]			= 0.025f;
	m_size[NLGFX_SZ_2D_L]			= 0.05f;
	m_size[NLGFX_SZ_2D_XL]			= 0.075f;
	m_size[NLGFX_SZ_3D_S]			= 0.05f;
	m_size[NLGFX_SZ_3D_M]			= 0.1f;
	m_size[NLGFX_SZ_3D_L]			= 0.15f;
	m_size[NLGFX_SZ_3D_XL]			= 0.3f;

	// Default text Sizes
	m_textSize[NLGFX_TXTSZ_2D_S] = 0.01f;
	m_textSize[NLGFX_TXTSZ_2D_M] = 0.025f;
	m_textSize[NLGFX_TXTSZ_2D_L] = 0.05f;
	m_textSize[NLGFX_TXTSZ_2D_XL] = 0.075f;

	m_textSize[NLGFX_TXTSZ_3D_S]	= 0.075f;
	m_textSize[NLGFX_TXTSZ_3D_M]	= 0.1f;
	m_textSize[NLGFX_TXTSZ_3D_L]	= 0.175f;
	m_textSize[NLGFX_TXTSZ_3D_XL]	= 0.25f;
	/*
	for (Nu32 i = 0; i < NLGFX_TXTSZ_SIZE; i++)
		m_textSize[i] = m_size[i & NLGFX_SZ_ID_MSK];
	*/
	NSetColorf(&m_drawTextTitle.Color, NCOLOR_PRESET3F_YELLOW, 1);
	m_drawTextTitle.Size = 0.05f;

	NSetColorf(&m_drawTextMain.Color, NCOLOR_PRESET3F_WHITE,1);
	m_drawTextMain.Size = 0.025f;

	NSetColorf(&m_drawTextSecondary.Color, NCOLOR_PRESET3F_BLUE_ELECTRIC, 1);
	m_drawTextSecondary.Size = 0.015f;

	// Default Basics
	m_Flags = 0;
	m_currentTextSize	= m_textSize[0];
	m_currentSize		= m_size[0];
	m_currentPickPack	= 0;
}
