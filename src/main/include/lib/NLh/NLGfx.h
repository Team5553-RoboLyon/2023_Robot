#pragma once
#include "lib/N/NType.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "lib/N/Miscellaneous/NColorPalette.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#define NLGFX_COLOR_TABLE_SIZE		16
#define NLGFX_COLOR_TABLE_ID_MSK	0xF		// Mask associ� permettant de s�curis� un id acc�dant directement � la palette de couleur comme ceci:  
											// m_color[ id&NL2DOCS_COLOR_TABLE_ID_MSK ]
											// par exemple avec un masque de 0xF soit 1111 la plus grande valeur possible apr�s masquage sera 15 !
											// !!! DONC, EN CAS DE MODIFICATION DE LA TAILLE DE LA PALETTE IL CONVIENT DE MODIFIER AUSSI LE MASK ASSOCIE !!!	
											//	taille 8, mask 0x7 ... taille 16, mask 0xF ... taille 16, mask 0x1F ...
#define NLGFX_TXTSZ_SIZE		8																				
#define NLGFX_TXTSZ_ID_MSK		0x7																				
#define NLGFX_TXTSZ_2D_S		0
#define NLGFX_TXTSZ_2D_M		1
#define NLGFX_TXTSZ_2D_L		2
#define NLGFX_TXTSZ_2D_XL		3
#define NLGFX_TXTSZ_3D_S		4
#define NLGFX_TXTSZ_3D_M		5
#define NLGFX_TXTSZ_3D_L		6
#define NLGFX_TXTSZ_3D_XL		7

#define NLGFX_SZ_SIZE			8																				
#define NLGFX_SZ_ID_MSK			0x7																				
#define NLGFX_SZ_2D_S			0
#define NLGFX_SZ_2D_M			1
#define NLGFX_SZ_2D_L			2
#define NLGFX_SZ_2D_XL			3
#define NLGFX_SZ_3D_S			4
#define NLGFX_SZ_3D_M			5
#define NLGFX_SZ_3D_L			6
#define NLGFX_SZ_3D_XL			7


typedef struct NLGFX	NLGFX;
struct  NLGFX
{

	Nu32					m_Flags;

	Nf32					m_currentTextSize;
	Nf32					m_currentSize;
	NCOLORPICKPACK			m_currentPickPack;

	NCOLOR					m_color[NLGFX_COLOR_TABLE_SIZE];
	Nf32					m_textSize[NLGFX_TXTSZ_SIZE];
	Nf32					m_size[NLGFX_SZ_SIZE];

	NUT_DRAWTEXT			m_drawTextTitle;
	NUT_DRAWTEXT			m_drawTextMain;
	NUT_DRAWTEXT			m_drawTextSecondary;


	void setup();

	inline const NCOLOR* getModulatedColor(NCOLOR* pcolor, const Nu8 index)const { return NExtractModulatedColor(pcolor, m_color, NLGFX_COLOR_TABLE_SIZE, index, m_currentPickPack); }
	inline const NCOLOR* getColor(const Nu8 index)const { NErrorIf(NSTCPLT_GET_COLID_MSK4b(index, m_currentPickPack) >= NLGFX_COLOR_TABLE_SIZE, NERROR_SYSTEM_GURU_MEDITATION); return &m_color[NSTCPLT_GET_COLID_MSK4b(index, m_currentPickPack)]; }
	inline const NCOLOR* getPaletteColor(const Nu8 index)const { NErrorIf(index >= NLGFX_COLOR_TABLE_SIZE, NERROR_SYSTEM_GURU_MEDITATION);  return &m_color[index]; }

	inline	NLGFX* usePickPack(const NCOLORPICKPACK pickpack) { m_currentPickPack = pickpack; return this; }
	inline	NLGFX* useTextSize(const Nu8 txtsz) { m_currentTextSize = m_textSize[txtsz & NLGFX_TXTSZ_ID_MSK]; return this; }
	inline	NLGFX* useSize(const Nu8 sz) { m_currentSize = m_size[sz & NLGFX_SZ_ID_MSK]; return this; }
	inline	NLGFX* useGfx(const NCOLORPICKPACK pickpack,const Nu8 txtsz,const Nu8 sz) { m_currentPickPack = pickpack; m_currentTextSize = m_textSize[txtsz & NLGFX_TXTSZ_ID_MSK]; m_currentSize = m_size[sz & NLGFX_SZ_ID_MSK]; return this; }
};
