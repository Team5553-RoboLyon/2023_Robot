#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NL2DOrthogonalCoordinateSystem.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
// Un outil simple pour dessiner des graphs 2D

#include "lib/N/NType.h"
#include "lib/N/Miscellaneous/NColorPalette.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/NRectf32.h"
#include "lib/N/Miscellaneous/NColorPalette.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"

#define DEFAULT_NL2DOCS_COLOR_PICKPACK		NSTCPLT_4b_PCK(0,1,0,0,0,0)

#define DEFAULT_NL2DOCS_AXIS_ARROWHEAD_SIZE_MIN	NSize_fPixToHRel_W(16.0f)			// a minimum size of 16 PIXELS 
#define DEFAULT_NL2DOCS_AXIS_ARROWHEAD_SIZE_RATIO	0.1f							// ratio de la plus petite des 2 dimensions du vecteur unit� 'm_unit'
																
// Position Relative de l'origine dans le Rectangle englobant												
// Inspir� de la gestion de la gestion des "poign�es" de "NUT_GizmoRectf32"
//
															//	| T | B | L | R | 		
															//	+---+---+---+---+
#define BITS_NL2DOCS_ORIGIN_LEFT_BOTTOM				0x6		//	| 0 | 1	| 1	| 0	|	
#define BITS_NL2DOCS_ORIGIN_LEFT_MID				0x2		//	| 0	| 0	| 1	| 0	|	
#define BITS_NL2DOCS_ORIGIN_LEFT_TOP				0xA		//	| 1	| 0	| 1	| 0	|	
#define BITS_NL2DOCS_ORIGIN_MID_TOP					0x8		//	| 1	| 0	| 0	| 0	|	
#define BITS_NL2DOCS_ORIGIN_RIGHT_TOP				0x9		//	| 1	| 0	| 0	| 1	|	
#define BITS_NL2DOCS_ORIGIN_RIGHT_MID				0x1		//	| 0	| 0	| 0	| 1	|	
#define BITS_NL2DOCS_ORIGIN_RIGHT_BOTTOM			0x5		//	| 0	| 1	| 0	| 1	|	
#define BITS_NL2DOCS_ORIGIN_MID_BOTTOM				0x4		//	| 0	| 1	| 0	| 0	|	
#define BITS_NL2DOCS_ORIGIN_CENTER					0xF		//	| 1	| 1	| 1	| 1	|	
//#define BITS_NL2DOCS_ORIGIN_CENTER				0x0		//	| 0	| 0	| 0	| 0	|	

#define MASK_2DOCS_USER_AVAILABLE					0xFFFFFF00	// available for external user ( = ~MASK_2DOCS_SYSTEM_RESERVED )
																// ( = ~MASK_2DOCS_SYSTEM_RESERVED )
#define MASK_2DOCS_SYSTEM_RESERVED					0xFF		// reserved by NL to manage NL2DOCS class 

#define MASK_2DOCS_ORIGIN							0xF
#define MASK_SHIFTED_2DOCS_ORIGIN					0xF
#define SHIFT_2DOCS_ORIGIN							0

#define _GET_NL2DOCS_ORIGIN_RIGHT(id)				((id)&1)	
#define _GET_NL2DOCS_ORIGIN_LEFT(id)				(((id)>>1)&1)
#define _GET_NL2DOCS_ORIGIN_BOTTOM(id)				(((id)>>2)&1)
#define _GET_NL2DOCS_ORIGIN_TOP(id)					(((id)>>3)&1)


//#define FLAG_NL2DOCS_ORIGIN_Reserved				BIT_0	\
//#define FLAG_NL2DOCS_ORIGIN_Reserved				BIT_1	 |_Used by the BITS_NL2DOCS_ORIGIN_xxx values to describe position of Origin in the OCS Bounding Box	
//#define FLAG_NL2DOCS_ORIGIN_Reserved				BIT_2	 | Utilis� par les 	valeurs BITS_NL2DOCS_ORIGIN_xxx pour d�crire la position del'Origine dans la boite englobante de l'OCS. 
//#define FLAG_NL2DOCS_ORIGIN_Reserved				BIT_3	/

//#define FLAG_NL2DOCS_available_but_RESERVED ...	BIT_4	
//#define FLAG_NL2DOCS_available_but_RESERVED ...	BIT_5	
//#define FLAG_NL2DOCS_available_but_RESERVED ...	BIT_6	
//#define FLAG_NL2DOCS_available_but_RESERVED ...	BIT_7	

//#define FLAG_NL2DOCS_available_for_user ...		BIT_8 until BIT_31	



typedef enum
{
	NL2DOCS_SUBSCALE_0 = 0,
	NL2DOCS_SUBSCALE_1,
	NL2DOCS_SUBSCALE_2,
	//---------------------------
	NL2DOCS_SUBSCALE_ENUM_SIZE
}NL2DOCS_SUBSCALE_ENUM;

typedef enum
{
	NL2DOCS_COORDS_STYLE_0 = 0,
	NL2DOCS_COORDS_STYLE_1,
	NL2DOCS_COORDS_STYLE_2,
	NL2DOCS_COORDS_STYLE_3,
	//---------------------------
	NL2DOCS_COORDS_STYLE_ENUM_SIZE
}NL2DOCS_COORDS_STYLE_ENUM;

typedef enum
{
	NL2DOCS_ORIENT_HORIZONTAL = 0,
	NL2DOCS_ORIENT_45,
	NL2DOCS_ORIENT_135,
	NL2DOCS_ORIENT_VERTICAL,
	//---------------------------
	NL2DOCS_ORIENT_ENUM_SIZE
}NL2DOCS_ORIENT_ENUM;

/*
enum class NL2DOCS_SUBSCALE_ENUM : unsigned int
{
	NL2DOCS_SUBSCALE_0 = 0,
	NL2DOCS_SUBSCALE_1 = 1,
	NL2DOCS_SUBSCALE_2 = 2,
	//---------------------------
	NL2DOCS_SUBSCALE_ENUM_SIZE = 3
};

enum class NL2DOCS_COORDS_STYLE_ENUM : unsigned int
{
	NL2DOCS_COORDS_STYLE_0 = 0,
	NL2DOCS_COORDS_STYLE_1 = 1,
	NL2DOCS_COORDS_STYLE_2 = 2,
	NL2DOCS_COORDS_STYLE_3 = 3,
	//---------------------------
	NL2DOCS_COORDS_STYLE_ENUM_SIZE = 4
};

enum class NL2DOCS_ORIENT_ENUM : unsigned int
{
	NL2DOCS_ORIENT_HORIZONTAL = 0,
	NL2DOCS_ORIENT_45 = 1,
	NL2DOCS_ORIENT_135 = 2,
	NL2DOCS_ORIENT_VERTICAL = 3,
	//---------------------------
	NL2DOCS_ORIENT_ENUM_SIZE = 4
};
*/
#define NL2DOCS_COLOR_TABLE_SIZE		16		// Nombre de couleurs dans la palette de couleurs int�gr�e. ( doit �tre puissance de 2 )
#define NL2DOCS_COLOR_TABLE_ID_MSK		0xF		// Mask associ� permettant de s�curis� un id acc�dant directement � la palette de couleur comme ceci:  palette[id&NL2DOCS_COLOR_TABLE_ID_MSK]
												// par exemple avec un masque de 0xF soit 1111 la plus grande valeur possible apr�s masquage sera 15 !
												// !!! DONC, EN CAS DE MODIFICATION DE LA TAILLE DE LA PALETTE IL CONVIENT DE MODIFIER AUSSI LE MASK ASSOCIE !!!	
												//	taille 8, mask 0x7 ... taille 16, mask 0xF ... taille 16, mask 0x1F ...
/*				
typedef enum
{
	NL2DOCS_COLOR_0 = 0,
	NL2DOCS_COLOR_1,
	NL2DOCS_COLOR_2,
	NL2DOCS_COLOR_3,
	NL2DOCS_COLOR_4,
	NL2DOCS_COLOR_5,
	NL2DOCS_COLOR_6,
	NL2DOCS_COLOR_7,
	//---------------------------
	NL2DOCS_COLOR_ENUM_SIZE
}NL2DOCS_COLOR_ENUM;
*/
/*
typedef enum
{
	NL2DOCS_DIGITS_NONE = 0,
	NL2DOCS_DIGITS_1,
	NL2DOCS_DIGITS_2,
	NL2DOCS_DIGITS_3,
	NL2DOCS_DIGITS_4,
	NL2DOCS_DIGITS_5,
	NL2DOCS_DIGITS_6,
	NL2DOCS_DIGITS_7,
	//---------------------------
	NL2DOCS_DIGIT_ENUM_SIZE
}NL2DOCS_DIGITS_ENUM;
*/

// Coords Style Flags
//	|_______________|_______________|_______________|_______________|
//	|15 |14 |13 |12 |11 |10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//	|				|				|		|			|	|	|	|
//	|				|	 COLOR Id	|ORIENT | DIGITS Nb	|	| L	| V |
//	|				|				|		|			|	| I	| A |
//	|				|	  4 bits	|2 bits |  3 bits	|	| N	| L |
//	|				|	   de 0		| de 0  |	de 0	|	| E	| U |
//	|				|	   � 15 	|  � 3  |	 � 7 	|	|  	| E |
//	|				|	  inclus	| inclus|  inclus	|	|	|   |
//	|				|				|		|			|	|	|   |
//	|---------------+-------.-------+-------+-----------+---+---+---|
//	|					Nu16	m_coordsStyle						|
//	|---------------------------------------------------------------|

#define MASK_NL2DOCS_COORDS_STYLE_COLOR						0xF
#define MASK_NL2DOCS_COORDS_STYLE_ORIENT					0x3
#define MASK_NL2DOCS_COORDS_STYLE_DIGITS					0x7

#define SHIFT_NL2DOCS_COORDS_STYLE_COLOR					8
#define SHIFT_NL2DOCS_COORDS_STYLE_ORIENT					6
#define SHIFT_NL2DOCS_COORDS_STYLE_DIGITS					3

#define FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY				BIT_0		// 
#define FLAG_NL2DOCS_COORDS_STYLE_LINE_DISPLAY				BIT_1		// 
#define FLAG_NL2DOCS_COORDS_STYLE_INDEX_DISPLAY				BIT_2		// 

//#define RESERVED TO STORE 3 bits DIGITS VALUE				BIT_3		 
//#define RESERVED TO STORE 3 bits DIGITS VALUE				BIT_4		 
//#define RESERVED TO STORE 3 bits DIGITS VALUE				BIT_5

//#define RESERVED TO STORE 2 bits ORIENT					BIT_6		 
//#define RESERVED TO STORE 2 bits ORIENT					BIT_7		 

//#define RESERVED TO STORE 4 bits COLOR ID					BIT_8		 
//#define RESERVED TO STORE 4 bits COLOR ID					BIT_9		 
//#define RESERVED TO STORE 4 bits COLOR ID					BIT_10		 
//#define RESERVED TO STORE 4 bits COLOR ID					BIT_11		 

//#define FLAG_NL2DOCS_COORDS_STYLE_availableflag			BIT_12		 
//#define FLAG_NL2DOCS_COORDS_STYLE_availableflag			BIT_13		 
//#define FLAG_NL2DOCS_COORDS_STYLE_availableflag			BIT_14		 
//#define FLAG_NL2DOCS_COORDS_STYLE_availableflag			BIT_15		 

#define GET_NL2DOCS_COORDS_STYLE_COLOR(coord_style_nu16)		(((coord_style_nu16)>>SHIFT_NL2DOCS_COORDS_STYLE_COLOR)&MASK_NL2DOCS_COORDS_STYLE_COLOR)
#define GET_NL2DOCS_COORDS_STYLE_ORIENT(coord_style_nu16)		(((coord_style_nu16)>>SHIFT_NL2DOCS_COORDS_STYLE_ORIENT)&MASK_NL2DOCS_COORDS_STYLE_ORIENT)
#define GET_NL2DOCS_COORDS_STYLE_DIGITS(coord_style_nu16)		(((coord_style_nu16)>>SHIFT_NL2DOCS_COORDS_STYLE_DIGITS)&MASK_NL2DOCS_COORDS_STYLE_DIGITS)

#define SET_NL2DOCS_COORDS_STYLE_COLOR(coord_style_nu16,c )		SET_FLAGS(coord_style_nu16, MASK_NL2DOCS_COORDS_STYLE_COLOR << SHIFT_NL2DOCS_COORDS_STYLE_COLOR, (c)&MASK_NL2DOCS_COORDS_STYLE_COLOR << SHIFT_NL2DOCS_COORDS_STYLE_COLOR)
#define SET_NL2DOCS_COORDS_STYLE_ORIENT(coord_style_nu16,o )	SET_FLAGS(coord_style_nu16, MASK_NL2DOCS_COORDS_STYLE_ORIENT << SHIFT_NL2DOCS_COORDS_STYLE_ORIENT, (o)&MASK_NL2DOCS_COORDS_STYLE_ORIENT << SHIFT_NL2DOCS_COORDS_STYLE_ORIENT)
#define SET_NL2DOCS_COORDS_STYLE_DIGITS(coord_style_nu16,d )	SET_FLAGS(coord_style_nu16, MASK_NL2DOCS_COORDS_STYLE_DIGITS << SHIFT_NL2DOCS_COORDS_STYLE_DIGITS, (d)&MASK_NL2DOCS_COORDS_STYLE_DIGITS << SHIFT_NL2DOCS_COORDS_STYLE_DIGITS)



#define FLAG_NL2DOCS_COORD_IS_Y								BIT_0		// Si ce flag est � ZERO ( valeur par d�faut) alors la coordonn�e est consid�r�e comme une abscisse (X) sinon comme une ordonn�e (Y)

typedef struct NL2DOCS_COORD	NL2DOCS_COORD;
struct NL2DOCS_COORD
{
	Nf32		m_source;
	Nf32		m_transformed;
	
	Nu8			m_index;
	Nu8			m_flags;
	Nu16		m_coordsStyle;
};

typedef struct NL2DOCS	NL2DOCS;									
struct NL2DOCS														
{	
public:
	//NL2DOCS();
	void setup(const Nchar *pcaption, const Nu32 bits_nl_2docs_origin, const NRECTf32 *pbb, const NVEC2f32 *punit);
	~NL2DOCS();
	Nu32					m_Flags;
	NRECTf32				m_BoundingBox;
	NVEC2f32				m_subScale[NL2DOCS_SUBSCALE_ENUM::NL2DOCS_SUBSCALE_ENUM_SIZE];
	NCOLOR					m_color[NL2DOCS_COLOR_TABLE_SIZE];

	Nchar				   *m_pCaption;			// Pointeur sur la "legende" du graphique
	NUT_DRAWTEXT			m_CaptionParam;		// Param�tres d'affichage de la l�gende du Graphique ( Taille et Couleur du texte, etc ... )
	NVEC2f32				m_Origin;			// "Origine" en coordonn�es "MONDE"
	NVEC2f32				m_Size;				// "Dimensions" du rep�re en coordonn�es "MONDE"	|
	NVEC2f32				m_Unit;				// "Longeur" d'une unit� en coordonn�es "MONDE"		|_le "nombre d'unit�s" repr�sentable est "m_size/m_unit"
	NVEC2f32				m_UnitCount;		// "nombre d'unit�s" repr�sentables.

	NARRAY					m_coordsArray;		// Array de "coordonn�es" stock�es pour affichage.
	Nu16					m_coordsStyle[NL2DOCS_COORDS_STYLE_ENUM::NL2DOCS_COORDS_STYLE_ENUM_SIZE];





	inline void				setUserFlagOn(const Nu32 userflags) { m_Flags |= (userflags&MASK_2DOCS_USER_AVAILABLE);  }
	inline void				setUserFlagOff(const Nu32 userflags) { m_Flags &= ~(userflags&MASK_2DOCS_USER_AVAILABLE); }


	void					setBoundingBoxKeepUnitCount(const NRECTf32 *pbb);
	void					setBoundingBoxKeepUnitCount(const Nf32 left, const Nf32 bottom, const Nf32 right, const Nf32 top );
	void					setBoundingBoxKeepUnitSize(const NRECTf32 * pbb);
	void					setBoundingBoxKeepUnitSize(const Nf32 left, const Nf32 bottom, const Nf32 right, const Nf32 top);

	inline void				setCaptionColor(const NCOLOR *pcolor) { m_CaptionParam.Color = *pcolor; }
	inline void				setCaptionColor(const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a) { NSetColorf(&m_CaptionParam.Color,r,g,b,a); }
	inline void				setCaptionSize(const Nf32 size) { m_CaptionParam.Size = size; }
	void					setOrigin(const Nu32 bits_nl_2docs_origin);
	inline void				setUnitSize(const NVEC2f32 *pu) { NErrorIf(!pu->x || !pu->y, NERROR_NULL_VALUE); m_Unit = *(NVEC2f32*)pu; m_UnitCount.x = m_Size.x / m_Unit.x; m_UnitCount.y = m_Size.y / m_Unit.y; }
	inline void				setUnitSize(const Nf32 ux, const Nf32 uy) { NErrorIf(!ux || !uy, NERROR_NULL_VALUE); m_Unit.x = ux; m_Unit.y = uy; m_UnitCount.x = m_Size.x / m_Unit.x; m_UnitCount.y = m_Size.y / m_Unit.y; }
	inline void				setUnitCount(const Nu32 nb_xunit, Nu32 nb_yunit)
	{
		NErrorIf(!nb_xunit || !nb_yunit, NERROR_NULL_VALUE);
		m_Unit.x = m_Size.x / (Nf32)nb_xunit; m_Unit.y = m_Size.y / (Nf32)nb_yunit;
		m_UnitCount.x = (Nf32)nb_xunit; m_UnitCount.y = (Nf32)nb_yunit;
	}

	inline const NRECTf32*	getBoundingBox() { return &m_BoundingBox; }
	inline const NUT_DRAWTEXT *getCaptionParam() { return &m_CaptionParam; }
	inline const NVEC2f32*	getOrigin() { return &m_Origin; }

	//inline	void			setColor(const NL2DOCS_COLOR_ENUM color_id, const NCOLOR *pcolor) { m_color[color_id] = *pcolor; }
	inline NCOLOR*			getModulatedColor(NCOLOR* pcolor, const Nu8 index, const NCOLORPICKPACK pickpack) {return NExtractModulatedColor(pcolor, m_color, NL2DOCS_COLOR_TABLE_SIZE, index, pickpack);}
	inline NCOLOR*			getColor(const Nu8 index, const NCOLORPICKPACK pickpack) { NErrorIf(NSTCPLT_GET_COLID_MSK4b(index, pickpack) >= NL2DOCS_COLOR_TABLE_SIZE, NERROR_SYSTEM_GURU_MEDITATION); return &m_color[NSTCPLT_GET_COLID_MSK4b(index, pickpack)]; }
	inline NCOLOR*			getColor(const Nu8 index) { NErrorIf(index >= NL2DOCS_COLOR_TABLE_SIZE, NERROR_SYSTEM_GURU_MEDITATION);  return &m_color[index]; }

	// Coords style
	inline void				hideCoordsIndex(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_OFF(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_INDEX_DISPLAY); }
	inline void				showCoordsIndex(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_INDEX_DISPLAY); }
	inline Nbool			isCoordsIndexVisible(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { return ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_INDEX_DISPLAY); }

	inline void				hideCoordsValue(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_OFF(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY); }
	inline void				showCoordsValue(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY); }
	inline Nbool			isCoordsValueVisible(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { return ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_VALUE_DISPLAY); }

	inline void				hideCoordsLine(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_OFF(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_LINE_DISPLAY); }
	inline void				showCoordsLine(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { FLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_LINE_DISPLAY ); }
	inline Nbool			isCoordsLineVisible(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id) { return ISFLAG_ON(m_coordsStyle[coord_style_id], FLAG_NL2DOCS_COORDS_STYLE_LINE_DISPLAY); }

	inline void				setCoordsColor(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id, const Nu8 color_id) { SET_NL2DOCS_COORDS_STYLE_COLOR(m_coordsStyle[coord_style_id], color_id); };
	inline void				setCoordsDigits(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id, const Nu8 digit_id) { SET_NL2DOCS_COORDS_STYLE_DIGITS(m_coordsStyle[coord_style_id], digit_id); };
	inline void				setCoordsOrientation(const NL2DOCS_COORDS_STYLE_ENUM coord_style_id, const NL2DOCS_ORIENT_ENUM orient_id) { SET_NL2DOCS_COORDS_STYLE_ORIENT(m_coordsStyle[coord_style_id], orient_id); };


	inline void				setSubScale(const NL2DOCS_SUBSCALE_ENUM sub, const Nf32 x, const Nf32 y) { m_subScale[sub].x = x; m_subScale[sub].y = y; }
	inline void				setSubScale(const NL2DOCS_SUBSCALE_ENUM sub, const NVEC2f32 *pv) { m_subScale[sub] = *pv; }
	inline void				setSubScaleX(const NL2DOCS_SUBSCALE_ENUM sub, const Nf32 x) { m_subScale[sub].x = x; }
	inline void				setSubScaleY(const NL2DOCS_SUBSCALE_ENUM sub, const Nf32 y) { m_subScale[sub].y = y; }

	//	Transformation simple sans et avec application d'une �chelle sp�cifique (subscale)
	inline Nf32				transformVx(const Nf32 x) { return  x * m_Unit.x; }
	inline Nf32				transformVy(const Nf32 y) { return  y * m_Unit.y; }
	inline void				transformV(NVEC2f32 *pv) { pv->x =  pv->x * m_Unit.x; pv->y = pv->y * m_Unit.y; }

	inline Nf32				transformX(const Nf32 x) { return m_Origin.x + x * m_Unit.x; }
	inline Nf32				transformY(const Nf32 y) { return m_Origin.y + y * m_Unit.y; }
	inline void				transform(NVEC2f32* pv) { pv->x = m_Origin.x + pv->x * m_Unit.x; pv->y = m_Origin.y + pv->y * m_Unit.y; }

	inline Nf32				transformX(const Nf32 x, const NL2DOCS_SUBSCALE_ENUM sub) { return m_Origin.x + x * m_subScale[sub].x*m_Unit.x; }
	inline Nf32				transformY(const Nf32 y, const NL2DOCS_SUBSCALE_ENUM sub) { return m_Origin.y + y * m_subScale[sub].y*m_Unit.y; }
	inline void				transform(NVEC2f32 *pv, const NL2DOCS_SUBSCALE_ENUM sub) { pv->x = m_Origin.x + pv->x * m_subScale[sub].x* m_Unit.x; pv->y = m_Origin.y + pv->y * m_subScale[sub].y* m_Unit.y; }
	
	//	Transformation stock�e pour restitution ( draw ) sans et avec application d'une �chelle sp�cifique (subscale)
	Nf32					transformX(const Nf32 x, const NL2DOCS_COORDS_STYLE_ENUM coord_group_id);
	Nf32					transformY(const Nf32 y, const NL2DOCS_COORDS_STYLE_ENUM coord_group_id);

	Nf32					transformX(const Nf32 x, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);
	Nf32					transformY(const Nf32 y, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);

	//	Transformation stock�e pour restitution ( draw ) sans et avec application d'une �chelle sp�cifique (subscale) + m�morisation d'un index [0,255] affichable.
	Nf32					transformX(const Nu8 index, const Nf32 x, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);
	Nf32					transformY(const Nu8 index, const Nf32 y, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);

	Nf32					transformX(const Nu8 index, const Nf32 x, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);
	Nf32					transformY(const Nu8 index, const Nf32 y, const NL2DOCS_SUBSCALE_ENUM sub, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);

	void					drawTransformedXaxisLabel(const Nf32  transformed_x, const Nchar * plabel, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);
	void					drawTransformedYaxisLabel(const Nf32  transformed_y, const Nchar * plabel, const NL2DOCS_COORDS_STYLE_ENUM coord_style_id);


//	void					getRectf32(NRECTf32 *prect);
//	void					setRectf32(const NRECTf32 *prect);

	void					draw(const NCOLORPICKPACK k);
	void					drawBaseLine(const Nchar *pbaselinetxt);

//	void					drawPositiveAxis();
//	void					drawPositiveAxis(const NUT_DRAW_ARROWS *px, const  NUT_DRAW_ARROWS *py);
//	void					drawNegativeAxis();
//	void					drawNegativeAxis(const NUT_DRAW_ARROWS *px, const  NUT_DRAW_ARROWS *py);
//	void					drawAxis(const NUT_DRAW_ARROWS *px, const  NUT_DRAW_ARROWS *py);
//	void					drawAxis();

	// helpers:
	Nf32					defaultAxisArrowSize() { return NMAX(NMIN(m_Unit.x, m_Unit.y)*DEFAULT_NL2DOCS_AXIS_ARROWHEAD_SIZE_RATIO, DEFAULT_NL2DOCS_AXIS_ARROWHEAD_SIZE_MIN); }	//  un ratio de la plus petite des 2 dimensions du vecteur unit�
																																													// ... avec un minimum de DEFAULT_NL2DOCS_AXIS_MIN_ARROWHEAD_SIZE
//private:
	inline void				updateOrigin()
	{
		Nu32 a = (Nu32)_GET_NL2DOCS_ORIGIN_RIGHT(m_Flags);
		Nu32 b = (Nu32)_GET_NL2DOCS_ORIGIN_LEFT(m_Flags);
		//m_Origin.x = (m_BoundingBox.right + m_BoundingBox.left)  * (1.0f + a - b) / 2.0f;
		m_Origin.x = ( m_BoundingBox.left*(Nf32)(1 - a + b ) + m_BoundingBox.right * (Nf32)(1 + a - b) )/ 2.0f;
		a = (Nu32)_GET_NL2DOCS_ORIGIN_BOTTOM(m_Flags);
		b = (Nu32)_GET_NL2DOCS_ORIGIN_TOP(m_Flags);
		//m_Origin.y = (m_BoundingBox.top + m_BoundingBox.bottom)	* (1.0f + a - b) / 2.0f;
		m_Origin.y = (m_BoundingBox.top*(Nf32)(1 - a + b) + m_BoundingBox.bottom * (Nf32)(1 + a - b)) / 2.0f;
	}
	
};

#ifdef _NEDITOR
// -----------------------------------------------------------------------
// NEDITOR Tool to easy manage and manipulate a 2D Orthogonal Coordinate System (NL2DOCS)
// outil "NEDITOR" pour manager et manipuler facilement un syst�me de coordonn�es orthogonal 2D ( NL2DOCS )

#endif	// _NEDITOR
