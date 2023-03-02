#ifndef __NTEXT_H
#define __NTEXT_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NText.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "./Containers/NNode.h"
#include "./Geometry/NGeometryBoundingVolume.h"
#include "./Render/Renderable/NRenderable.h"
#include "./Render/Renderable/NRenderable_ExtractSet.h"
#include "./Miscellaneous/NVertexColorBuild.h"
#include "NType.h"
#include "./Maths/NVec3f32.h"
#include "NFont.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Structures & Define									**
// ***************************************************************************************
#define CONSTANT_NTEXT_SINGLEPAGE_FONT_LOWLEVEL_EXTRACTSET		NEXTRACTSET_SECTION_STANDARD
#define CONSTANT_NTEXT_MULTIPAGES_FONT_LOWLEVEL_EXTRACTSET		(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES)

#define DEFAULT_NTEXT_RENDERABLE_BOUNDINGSPHERE_RADIUS			1.0f
#define DEFAULT_NTEXT_GEOMETRY_PRIMITIVE_CAPACITY				8	// = 4 characters * 2 triangles
#define DEFAULT_NTEXT_GEOMETRY_VERTEX_CAPACITY					16	// = 4 characters * 4 vertex

#define DEFAULT_NTEXT_GEOMETRIES_SPECS_ID						NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE
#define DEFAULT_NTEXT_USERSTATESET								0
#define DEFAULT_NTEXT_HIGHLEVEL_EXTRACTSET						FLAG_NEXTRACTSET_3D

#define DEFAULT_NTEXT_COLOR										NCOLOR_PRESET3F_WHITE
#define DEFAULT_NTEXT_ANCHOR_3F									0.0f,0.0f,0.0f
#define DEFAULT_NTEXT_CONSTRUCTION_ORIGIN_3F					0.0f,0.0f,0.0f
#define DEFAULT_NTEXT_ALIGNMENT									NTEXT_ALIGNMENT_LEFT
//#define NTEXT_COLOR_RANGE_SIZE									4


/*
// Predefined Anchor values
#define NANCHOR_X_LEFT			0.0f
#define NANCHOR_X_CENTER		0.5f
#define NANCHOR_X_RIGHT			1.0f
#define NANCHOR_Y_BOTTOM		0.0f
#define NANCHOR_Y_CENTER		0.5f
#define NANCHOR_Y_TOP			1.0f
*/

/*
typedef enum
{
	NTEXT_XLEFT_ALIGNED=0,
	NTEXT_XRIGHT_ALIGNED,
	NTEXT_XCENTER_ALIGNED
}NTEXT_XALIGN_ENUM;
typedef enum
{
	NTEXT_YTOP_ALIGNED=0,
	NTEXT_YBOTTOM_ALIGNED,
	NTEXT_YCENTER_ALIGNED,
	NTEXT_YBASE_ALIGNED
}NTEXT_YALIGN_ENUM;
*/
#define FLAG_NTEXT_DESC_USEPARAM_GEOMETRY_FORMAT		BIT_0
#define FLAG_NTEXT_DESC_USEPARAM_USERSTATESET			BIT_1
#define FLAG_NTEXT_DESC_USEPARAM_USEREXTRACTSET			BIT_2
#define FLAG_NTEXT_DESC_USEPARAM_APPEARANCE				BIT_3
#define FLAG_NTEXT_DESC_USEPARAM_FONT_TEXTUREUNIT		BIT_4

#define FLAG_NTEXT_DESC_USEPARAM_FONT					BIT_5
#define FLAG_NTEXT_DESC_USEPARAM_ANCHOR					BIT_6
#define FLAG_NTEXT_DESC_USEPARAM_CONSTRUCTION_ORIGIN	BIT_7
#define FLAG_NTEXT_DESC_USEPARAM_TRANSFORMHNODE			BIT_8
#define FLAG_NTEXT_DESC_USEPARAM_ALIGNMENT				BIT_9

// NTEXT_ALIGNMENT_ENUM.
// !!! WARNING !!!	Text Alignment has nothing to see with Anchoring.
//					These 2 concepts are different.
//					Text Alignment represents the way used to build and locate each text line
//					( and character for NTEXT_ALIGMENT_JUSTIFY ).
//					Text Anchoring represents the relative to text size hot spot. This notion
//					is relative to the whole text Box and the ConstructionOrigin.
typedef enum
{
	NTEXT_ALIGNMENT_LEFT = 0,
	NTEXT_ALIGNMENT_RIGHT,
	NTEXT_ALIGNMENT_CENTER,
	//NTEXT_ALIGNMENT_JUSTIFY, ToDO !
	//---------------------
	NTEXT_ALIGNMENT_ENUM_SIZE		// ( will be stored as a Nu8 into NTEXT structure, so NTEXT_ALIGNMENT_ENUM_SIZE should not > 255 )
}NTEXT_ALIGNMENT_ENUM;
typedef struct
{
	Nu16								Flags;
	Nu8									Font_TextureUnit;
	//Nu8								Available_Nu8;
	NTEXT_ALIGNMENT_ENUM				Alignment;
	NGEOMETRY_FORMAT_ENUM				GeometryFormat;
	NSTATESET							UserStateSet;
	NEXTRACTSET							HighLevelExtractSet;
	NAPPEARANCE_RAW						Appearance_Raw;
	
	NFONT								*pFont;
	
	NBUILDBASE_COLOR					ColorBuild;
	NVEC3							Anchor;
	NVEC3							ConstructionOrigin;
	NTRANSFORM_HNODE					*pThn;
}NTEXT_DESC;

// a "NText" (used to create and display a textured text mesh from a string)
// #define FLAG_NTEXT_MESHBUILT											BIT_0 // There is a real mesh, created by "NBuildTextMeshFromString(NTEXT *ptext, const Nchar *pstring)"
// #define FLAG_NTEXT_MESH_UPDATED_BOUNDINGVOLUME_HAS_TO_BE_UPDATED		BIT_1 
typedef struct
{
	NRENDERABLE			*pRenderable;
	NVEC3			ConstructionOrigin;
	NVEC3			Anchor;
	NVEC3			Cursor;				
	NFONT				*pFont;
	NCOLOR				ColorRange4[NRANGE_4COLORS];	// 4 colors for the 4 vertex of each character quad
	Nu8					Alignment;	// it's a NTEXT_ALIGNMENT_ENUM stored into a Nu8 to save memory...
	//Nu8				Available_Nu8;
	//Nu16				Available_Nu16;
}NTEXT;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NTEXT*				NSetupText(NTEXT *ptext, const NTEXT_DESC *ptextdesc);
NTEXT*				NCreateText(const NTEXT_DESC *ptextdesc);
void				NClearText(void *ptext);
void				NDeleteText(void *ptext);

inline	void		NSetTextColorRange(NTEXT *ptext, const NCOLOR *pcolorrange, const Nu32 colorangesize){NCopyColorRange(ptext->ColorRange4,NRANGE_4COLORS,pcolorrange,colorangesize);}
inline	void		NSetTextColor(NTEXT *ptext, const NCOLOR *pcolor){NSetColorRange(ptext->ColorRange4,NRANGE_4COLORS,pcolor);}
inline	void		NSetTextColorf(NTEXT *ptext, const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a){NSetColorRangef(ptext->ColorRange4,NRANGE_4COLORS,r,g,b,a);}
/*
inline	void		NSetTextColorRangeGradient(NTEXT *ptext,const NCOLOR *pcolorA,const NCOLOR *pcolorB){NSetColorRangeGradient(ptext->ColorRange4,NRANGE_4COLORS,2,pcolorA,pcolorB);}
inline	void		NSetTextColorRangeGradientf(NTEXT *ptext, const Nf32 r0, const Nf32 v0, const Nf32 b0, const Nf32 a0, const Nf32 r1, const Nf32 v1, const Nf32 b1, const Nf32 a1 ){NSetColorRangeGradientf(ptext->ColorRange4,NRANGE_4COLORS,2,r0,v0,b0,a0,r1,v1,b1,a1 );}
*/

void				NScaleTextColorRange(NTEXT *ptext, const NCOLOR *c);
void				NScaleTextColorRangef(NTEXT *ptext, const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a);
void				NScaleTextColorRangeComponent(NTEXT *ptext, const Nu32 component_id, const Nf32 s);

inline	void		NSetTextAnchor(NTEXT *ptext, const NVEC3 *pvanc ){ptext->Anchor = *pvanc;}
inline	void		NSetTextAnchorf(NTEXT *ptext, const Nf32 x, const Nf32 y, const Nf32 z){NVec3Set(&ptext->Anchor,x,y,z);}
inline	void		NSetTextConstructionOrigin(NTEXT *ptext, const NVEC3 *pvsp ){ptext->ConstructionOrigin =*pvsp;}
inline	void		NSetTextConstructionOriginf(NTEXT *ptext, const Nf32 x, const Nf32 y, const Nf32 z){NVec3Set(&ptext->ConstructionOrigin,x,y,z);}
inline	void		NSetTextAlignment(NTEXT *ptext, NTEXT_ALIGNMENT_ENUM alignment ){ptext->Alignment = (Nu8)alignment;}

void				NSetTextMaterial(NTEXT *ptext, const NMATERIAL *pmaterial);
void				NSetTextBlend(NTEXT *ptext, const NBLEND *pblend);

void				NUpdateTextColor(NTEXT *ptext);

void				NBuildTextMeshFromString(NTEXT *ptext, const Nchar *pstring);

Nu32				NGetStringSize(NVEC3 *pvsize, const Nchar *pstr, const NFONT *pfont);
Nu32				NGetStringLineSize( NVEC3 *pvsize,const Nchar *pstr, const NFONT *pfont);
Nf32				NGetStringLineLength(const Nchar *pstr, const NFONT *pfont);

void				NRenderableUpdate_Text(NRENDERABLE *prenderable, void* powner, const NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NTEXT_H 

