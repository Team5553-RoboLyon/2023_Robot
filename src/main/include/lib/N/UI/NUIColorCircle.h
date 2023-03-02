#ifndef __NUI_COLORCIRCLE_H
#define __NUI_COLORCIRCLE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUIColorCircle.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_NUI_COLORCIRCLE_INFOS_STRING_SIZEMAX				128

#define DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE							0.85f	// |
#define DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE						0.025f	// |-> Total = 1.0f !
#define DEFAULT_NUI_COLORCIRCLE_ARCS_RSIZE							0.125f	// |

#define DEFAULT_NUI_COLORCIRCLE_ADDITIONNAL_SEXTANT_SUBDIVISION		6
#define	DEFAULT_NUI_COLORCIRCLE_SLICENUMBER							( 6*(DEFAULT_NUI_COLORCIRCLE_ADDITIONNAL_SEXTANT_SUBDIVISION+1) )

#define DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB					16
#define DEFAULT_NUI_COLORCIRCLE_SELECTOR_SMALLSIZERATIO				0.6f	// against selector full size
#define DEFAULT_NUI_COLORCIRCLE_SELECTOR_SIZERATIO					0.08f	// against full color circle size 

// Define
#define DEFAULT_NUI_COLORCIRCLE_COLOR_RED						0.5f
#define DEFAULT_NUI_COLORCIRCLE_COLOR_GREEN						0.5f
#define DEFAULT_NUI_COLORCIRCLE_COLOR_BLUE						0.5f
#define DEFAULT_NUI_COLORCIRCLE_COLOR_ALPHA						1.0f

// COLORCIRCLE ALPHA ARC "BACK" COLOR
#define CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_RED		0.25f
#define CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_GREEN		0.25f
#define CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_BLUE		0.25f
#define CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_ALPHA		1.0f

// COLORCIRCLE BUILDING ANGLE
#define CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG		10.0f 

#define CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_ANGLEDEG						(270.0f + CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)
#define CONSTANT_NUI_COLORCIRCLE_ALPHAMAX_ANGLEDEG						(90.0f - CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)
#define CONSTANT_NUI_COLORCIRCLE_ALPHARANGE_ANGLEDEG						(180.0f - 2.0f*CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)

#define CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG					(270.0f - CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)
#define CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_ANGLEDEG					(90.0f + CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)
#define CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSRANGE_ANGLEDEG				(180.0f - 2.0f*CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG)

// used only to pick Arcs ...
#define CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_PLUS_MARGIN_ANGLEDEG			(270.0f + CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG*0.25f)
#define CONSTANT_NUI_COLORCIRCLE_ALPHAMAX_PLUS_MARGIN_ANGLEDEG			(90.0f - CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG*0.25f)
#define CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_PLUS_MARGIN_ANGLEDEG		(270.0f - CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG*0.25f)
#define CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_PLUS_MARGIN_ANGLEDEG		(90.0f + CONSTANT_NUI_COLORCIRCLE_ARC_SHIFTING_FROM_VERTICAL_ANGLEDEG*0.25f)

// Miscellaneous
#ifdef _DEBUG
#define CONSTANT_NUI_COLORCIRCLE_SIZEMIN								0.05f // Generate an error in debug mode if colorcircle size is less than this value
#endif
// ***************************************************************************************
// *								FLAG_NUI_COLORCIRCLE									 *
// ***************************************************************************************
//	NUI_COLORCIRCLE.UI.Flags (starts at BIT_24)
#define FLAG_NUI_COLORCIRCLE_INPROGRESS_COMMAND								BIT_24
#define	FLAG_NUI_COLORCIRCLE_RGBA											BIT_25
#define	FLAG_NUI_COLORCIRCLE_COLOR_INFOS_PRINTING							BIT_26
#define	FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT								BIT_27
//#define FLAG_NUI_COLORCIRCLE_												BIT_28
//#define FLAG_NUI_COLORCIRCLE_												BIT_29
//#define FLAG_NUI_COLORCIRCLE_												BIT_30
//#define FLAG_NUI_COLORCIRCLE_												BIT_31

//	NUI_COLORCIRCLE_DESC.UI.Flags_Style (starts at BIT_24)
#define	FLAG_NUIDS_USEPARAM_COLORCIRCLE_INITIALCOLOR						BIT_24
/*
#define	FLAG_NUIDS_USEPARAMS_COLORCIRCLE_RELATIVE_SIZE_RATIO				BIT_25
#define	FLAG_NUIDS_USEPARAM_COLORCIRCLE_ADDITIONAL_SEXTANT_SUBDIVISION		BIT_26
#define	FLAG_NUIDS_USEPARAMS_COLORCIRCLE_SELECTORS_SIZE_RATIO				BIT_27
#define	FLAG_NUIDS_USEPARAM_COLORCIRCLE_SELECTORS_SLICENB					BIT_28
*/
#define	FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING							BIT_29

//	NUI_COLORCIRCLE_DESC.UI.Flags_Core (starts at BIT_24)
#define FLAG_NUIDC_COLORCIRCLE_INPROGRESS_COMMAND							BIT_24
#define	FLAG_NUIDC_COLORCIRCLE_RGBA											BIT_25 // means updatable Alpha !
#define	FLAG_NUIDC_COLORCIRCLE_NU8_COMMAND_FORMAT							BIT_26

	// ***************************************************************************************
	// *								Structures											 *
	// ***************************************************************************************
	// A ColorCircle
	typedef struct  
	{
		NUI	UI;
		
		NCOLOR		Color;
		NHSV		HSV;

		Nf32		ColorRadius;			// Color Circle Radius.
		Nf32		SpacingRadius;			// From color circle center to Small Arcs Circumference - spacing included.
		Nf32		ArcsRadius;				// Arcs Radius, from color circle center to LA arcs edge.
		
		NGEOMETRY	*pCircle_Geometry;		// The geometry which contains ColorCircle, the 2 Arcs and the 3(or 2) selectors (Rings).
		
		NVEC3	SelectorColor_Position;
		NVEC3	SelectorBright_Position;
		NVEC3	SelectorAlpha_Position;

		Nu16		Circle_AdditionalSextantSubdivision;	// At least one circle = 6 vertex (center excluded, just the 6 around ).
															// So we have 6 sextants ... for each of them we may have additional vertex.
															// VertexSize = 6*(1+additionalSextantSubdivision)
		//Nu16		available ...
		
		Nu16		Circle_FirstVertex;						// ...of circumference  (center excluded)
		Nu16		Circle_VertexSize;						// .. of circumference (center excluded). 
															// Total Vertex size ( circumference + center ) = Circle_VertexSize+1

		Nu16		Circle_CenterVertex;					// the center of the circle !
		
		Nu16		ArcAlpha_FirstVertex;
		Nu16		ArcAlpha_VertexSize;
		
		Nu16		SelectorColor_FirstVertex;
		Nu16		SelectorColor_VertexSize;
		
		Nu16		SelectorBright_FirstVertex;
		Nu16		SelectorBright_VertexSize;
		
		Nu16		SelectorAlpha_FirstVertex;
		Nu16		SelectorAlpha_VertexSize;

		NVEC3	TransformedCenter_Position;

	}NUI_COLORCIRCLE;

	// a ColorCircle Description
	typedef struct
	{
		NUI_DESC		UIDesc;
		NUIGEOMETRY_DESC		ColorCircleDesc;

		NCOLOR				InitialColor;
		
/*
		Nf32				ColorCircle_RelativeSizeRatio;	// Color Circle Radius.
		Nf32				Spacing_RelativeSizeRatio;	// From color circle center to Small Arcs Circumference - spacing included.
		Nf32				Arcs_RelativeSizeRatio;		// Arcs Radius, from color circle center to Extern arcs edge.
		
		Nu16				Circle_AdditionalSextantSubdivision;	// At least one circle = 6 vertex (center excluded, just the 6 around ).
		Nu16				Selectors_Ring_SliceNb;	// At least one circle = 6 vertex (center excluded, just the 6 around ).

		Nf32				Selectors_Ring_SizeRatio;
		Nf32				Selectors_Ring_SmallSizeRatio;
*/
		
	}NUI_COLORCIRCLE_DESC;

	// ***************************************************************************************
	// *								Functions											 *
	// ***************************************************************************************
	NUI_COLORCIRCLE*		NCreateUIColorCircle(NUI *parent, NUI_COLORCIRCLE_DESC *pcdesc, const Nu32 user32);
	NUI_COLORCIRCLE*		NSetupUIColorCircle(NUI *parent, NUI_COLORCIRCLE *pc, NUI_COLORCIRCLE_DESC *pcdesc, const Nu32 user32);
	void					NClearUIColorCircle(NUI *pui);
	//void					NDeleteUIColorCircle(void *pfr); !!! Doesn't Exist !!! Use 'NDeleteUI'

	void					NUIColorCircle_SetColor(NUI_COLORCIRCLE *pc, const NCOLOR *pcolor, const Nbool send_command_message, const Nbool _if_color_updated_only);

	inline	Nu32			NUIGetColorCircleSliceNumber( const Nu16 circle_additionalsextantsubdivision ){return ( 6*(circle_additionalsextantsubdivision+1) );}
	
	inline	void			NUIGetColorCircleColor(NCOLOR *pcolor, const NUI_COLORCIRCLE *pc){*pcolor = pc->Color;}
	inline	void			NUIGetColorCircleHSV(NHSV *phsv, const NUI_COLORCIRCLE *pc){*phsv = pc->HSV;}

// To always generate ColorCircle displayable Infos in the same way ...
// These 2 functions are the ones used internally:
	Nchar*					NUIColorCircle_SetupDisplayableInfos_RGBAString(Nchar *pdest, const Nchar *pcaption, const Nu8 red, const Nu8 green, const Nu8 blue, const Nu8 alpha );
	Nchar*					NUIColorCircle_SetupDisplayableInfos_RGBString(Nchar *pdest, const Nchar *pcaption, const Nu8 red, const Nu8 green, const Nu8 blue );

// LOW LEVEL
	NGEOMETRY*				NUICreateColorCircleCustomGeometry(const Nf32 main_radius,const Nbool updatable_alpha);

// PRIVATE functions ( never use them !! )
// (because they don't send message through event proc. They just change internal values )
	inline	void			NUISetColorCircleColorHSV(NUI_COLORCIRCLE *pc,const NHSV *phsv){pc->HSV = *phsv;NHSV_To_NCOLOR(&pc->Color,&pc->HSV,-1);}
	inline	void			NUISetColorCircleColorHSVf(NUI_COLORCIRCLE *pc,const Nf32 h, const Nf32 s, const Nf32 v){pc->HSV.Hue=h;pc->HSV.Saturation=s;pc->HSV.Brightness=v;NHSV_To_NCOLOR(&pc->Color,&pc->HSV,-1);}
	inline	void			NUISetColorCircleColorHSV_H(NUI_COLORCIRCLE *pc,const Nf32 h){pc->HSV.Hue=h;NHSV_To_NCOLOR(&pc->Color,&pc->HSV,-1);}
	inline	void			NUISetColorCircleColorHSV_S(NUI_COLORCIRCLE *pc,const Nf32 s){pc->HSV.Saturation=s;NHSV_To_NCOLOR(&pc->Color,&pc->HSV,-1);}
	inline	void			NUISetColorCircleColorHSV_V(NUI_COLORCIRCLE *pc,const Nf32 v){pc->HSV.Brightness=v;NHSV_To_NCOLOR(&pc->Color,&pc->HSV,-1);}
	
	inline	void			NUISetColorCircleColor(NUI_COLORCIRCLE *pc,const NCOLOR *pcolor){pc->Color = *pcolor;NCOLOR_To_NHSV(&pc->HSV,&pc->Color);}
	inline	void			NUISetColorCircleColorf(NUI_COLORCIRCLE *pc,const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a){NSetColorf(&pc->Color,r,g,b,a);NCOLOR_To_NHSV(&pc->HSV,&pc->Color);}
	inline	void			NUISetColorCircleColorRed(NUI_COLORCIRCLE *pc,const Nf32 r){pc->Color.red=r;NCOLOR_To_NHSV(&pc->HSV,&pc->Color);}
	inline	void			NUISetColorCircleColorGreen(NUI_COLORCIRCLE *pc,const Nf32 g){pc->Color.green=g;NCOLOR_To_NHSV(&pc->HSV,&pc->Color);}
	inline	void			NUISetColorCircleColorBlue(NUI_COLORCIRCLE *pc,const Nf32 b){pc->Color.blue=b;NCOLOR_To_NHSV(&pc->HSV,&pc->Color);}
	inline	void			NUISetColorCircleColorAlpha(NUI_COLORCIRCLE *pc,const Nf32 a){pc->Color.alpha=a;}

	

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUI_FRAME_COLORCIRCLE_H 

