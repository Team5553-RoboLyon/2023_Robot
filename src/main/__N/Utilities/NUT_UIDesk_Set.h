#ifndef __NUT_UIDESK_SET_H
#define __NUT_UIDESK_SET_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_UIDesk_set.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../UI/NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_NUT_UIDESKSET_DEFAULT_TEXTUREUNIT				0 // Texture Unit used by default for all the NUT_UIs
#define CONSTANT_NUT_UIDESKSET_BACKGROUND_TEXTUREUNIT			0 // Texture Unit used by the background texture for all the NUT_UIs
#define CONSTANT_NUT_UIDESKSET_ICON_TEXTUREUNIT					0 // Texture Unit used by the icon texture for all the NUT_UIs
#define CONSTANT_NUT_UIDESKSET_CHECK_UNCHECK_TEXTUREUNIT		0 // Texture Unit used by the check/Uncheck texture of the NUT_UI CheckBoxes
#define CONSTANT_NUT_UIDESKSET_CUSTOM_PICTURE_TEXTUREUNIT	0 // Texture Unit used by the geomViewer Picture of the NUT_UI Custom
#define CONSTANT_NUT_UIDESKSET_PICTURE_TEXTUREUNIT				0 // Texture Unit used by the Picture of the NUT_UI Picture

#define CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE					4 // Size of all the alternative color range
																  // (used instead classic UIStyle color if the associated UIDESK FLAG is set to ON)
typedef enum
{
	NUT_UIDESK_BKGD=0,		// |
	NUT_UIDESK_ICON,		// |_ Important Order. If it changes, update "FLAG_NUT_UIDESKSET_USE_???"  in accordance with...
	NUT_UIDESK_TEXT,		// |
	NUT_UIDESK_MISC,		// |
	// --------------------
	NUT_UIDESKCOMPONENT_ENUM_SIZE
}NUT_UIDESKCOMPONENT_ENUM;

/*
typedef enum
{
	NUT_UIC_SIZECONTRAINTS_DISABLE,
	NUT_UIC_SIZECONTRAINTS_SIZE,
	NUT_UIC_SIZECONTRAINTS_SIZEMIN,
	NUT_UIC_SIZECONTRAINTS_SIZEMAX,
	// --------------------
	NUT_UIDESKSET_SIZECONTRAINTS_ENUM_SIZE
}NUT_UIDESKSET_SIZECONTRAINTS_ENUM;
*/
																	//	_ 
#define FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			BIT_0	// |
#define FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		BIT_1	// |_ These flags are connected to the 'NUT_UIDESKCOMPONENT_ENUM'
#define FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			BIT_2	// |  They follow the same order (BKGD,ICON,TEXT,MISC). 
#define FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		BIT_3	// |_ And the sequence (BLENDMATERIAL,ALTERNATIVECOLOR) is the same for 	
#define FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			BIT_4	// |  each group. !!It's very important!! This organization is necessary 
#define FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		BIT_5	// |_ and used to simplify the treatment algorithm !!!!
#define FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			BIT_6	// |  They have to use BIT_0 to BIT_7 in that exact order !
#define FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		BIT_7	// |_ 

#define FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE		BIT_17	
#define FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					BIT_18	
#define FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					BIT_19	// WORKS with: 
#define FLAG_NUT_UIDESKSET_COLOR_UPDATE						BIT_20	// WORKS with:
#define FLAG_NUT_UIDESKSET_PUSH								BIT_21	// WORKS with: PushButton/CheckBox/ValUpdater
#define FLAG_NUT_UIDESKSET_DEFAULT_BKGD						BIT_22
#define FLAG_NUT_UIDESKSET_X_DISTRIBUTION					BIT_23	// WORKS with:
#define FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					BIT_24	// WORKS with:
#define FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					BIT_25	// WORKS with:
#define FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				BIT_26	// 
#define FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			BIT_27	// WORKS with:
#define FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		BIT_28	// WORKS with:
#define FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			BIT_29	// | 
#define FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		BIT_30	// |- UI Building Size Constraints type ( UI Building Size = UI Building Background Size. )
#define FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		BIT_31	// |

#define MASK_NUT_UIDESKSET_SIZECONSTRAINTS					(FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE|FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN|FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NUT_UIDESK_SET
// +
// +-+	About Outer3DMargin and Inner3DSpacing.
//		First, they are both in 3D ! But controllers are in 2D, so what ?
//		In fact, UI is in 3D and absolutely must be like this because it will be possible
//		to create 3D HUD !
//		It's easy to understand how 3DMargin and 3DSpacing are working for 3D and 2D at the same time.
//		In 3D it's like nesting 3d boxes, in 2D like nesting 2d boxes.
//		More precisely, 3DMargin deal especially with background. It helps to determine size of it regarding all
//		the other components (text and geometries). Purpose is having a margin between the background edges and all
//		of the others. But, it's possible to have a Flat Background (size Z = 0) ... So what about 3DMargin.z in that very common case ?
//		... Actually it's Simple, and absolutely logical. Purpose of Margin is keep an empty space between others things and background edges.
//		That case is not so different than all the others. Other things have to respect this z margin between them and background, and because
//		It's a flat background they have just to be in front of them with a distance of 3Dmargin.z from their back to the flat background.
//		An image to better understood: A Flat background is like the back side of the 3D background cube.
//		Same logic for Inner3DSpacing...
typedef struct  
{
	Nu32						PropertiesFlags;
	NUI_LISTEN_TOUCH_MODE_ENUM	TouchMode;
	NVEC3					SizeConstraints;	// Size constraints of the Background (=size, = size min, = size max according properties flags)
	NVEC3					OuterMargin;		// Margin between "things" and external Background edges.
	NVEC3					InnerSpacing;		// Space between "things" inside the controller.
	NUI_STYLE					*pStyle;
	NVEC3					BuildAnchor;		// The whole controller is build from this own anchor ( like, build this button from it's own center ... that means the center of the button will be (0,0,0) into its local base )
	NVEC3					PositionAnchor;		// The controller is put in place anchored by this point ( like, put the center of this button at this world place )
	NTEXT_ALIGNMENT_ENUM		Alignment;
	NBLEND						*pBlend[NUT_UIDESKCOMPONENT_ENUM_SIZE];
	NMATERIAL					*pMaterial[NUT_UIDESKCOMPONENT_ENUM_SIZE];
	NCOLOR						AlternativeColorRange[NUT_UIDESKCOMPONENT_ENUM_SIZE][CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE];
}NUT_UIDESK_SET;

#define NUT_UIDESKSET_STACKSIZE		4
extern NUT_UIDESK_SET	NUT_UIDeskSet[NUT_UIDESKSET_STACKSIZE];
extern NUT_UIDESK_SET	*NUT_pCurrentUIDeskSet;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
inline void		NUT_UIDesk_ActivateProperties(Nu32 properties_to_activate){FLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,properties_to_activate);}
inline void		NUT_UIDesk_DeactivateProperties(Nu32 properties_to_deactivate){FLAG_OFF(NUT_pCurrentUIDeskSet->PropertiesFlags,properties_to_deactivate);}
inline void		NUT_UIDesk_DeactivateAllProperties(){NUT_pCurrentUIDeskSet->PropertiesFlags = 0;}

inline void		NUT_UIDesk_SetTouchMode(const NUI_LISTEN_TOUCH_MODE_ENUM touch_mode){NUT_pCurrentUIDeskSet->TouchMode = touch_mode;}

inline void		NUT_UIDesk_ActivateSizeConstraints_Min(){FLAG_OFF(NUT_pCurrentUIDeskSet->PropertiesFlags,MASK_NUT_UIDESKSET_SIZECONSTRAINTS);FLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN);}
inline void		NUT_UIDesk_ActivateSizeConstraints_Max(){FLAG_OFF(NUT_pCurrentUIDeskSet->PropertiesFlags,MASK_NUT_UIDESKSET_SIZECONSTRAINTS);FLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX);}
inline void		NUT_UIDesk_ActivateSizeConstraints_Size(){FLAG_OFF(NUT_pCurrentUIDeskSet->PropertiesFlags,MASK_NUT_UIDESKSET_SIZECONSTRAINTS);FLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE);}
inline void		NUT_UIDesk_DeactivateSizeConstraints(){FLAG_OFF(NUT_pCurrentUIDeskSet->PropertiesFlags,MASK_NUT_UIDESKSET_SIZECONSTRAINTS);}

inline void		NUT_UIDesk_SetUIBackground_SizeConstraintsf(const Nf32 x_size,const Nf32 y_size,const Nf32 z_size ){NUT_pCurrentUIDeskSet->SizeConstraints.x = x_size;NUT_pCurrentUIDeskSet->SizeConstraints.y = y_size;NUT_pCurrentUIDeskSet->SizeConstraints.z = z_size;}
inline void		NUT_UIDesk_SetUIBackground_SizeConstraints(const NVEC3 *psize ){NUT_pCurrentUIDeskSet->SizeConstraints = *psize;}
inline void		NUT_UIDesk_SetStyle(const NUI_STYLE *pstyle){NUT_pCurrentUIDeskSet->pStyle = (NUI_STYLE*)pstyle;}

inline void		NUT_UIDesk_SetTextAlignment(const NTEXT_ALIGNMENT_ENUM alignment){NUT_pCurrentUIDeskSet->Alignment = alignment;}

inline void		NUT_UIDesk_SetBuildAnchor(const NVEC3 *panchor){NUT_pCurrentUIDeskSet->BuildAnchor = *panchor;}
inline void		NUT_UIDesk_SetBuildAnchorf(const Nf32 anchor_x, const Nf32 anchor_y, const Nf32 anchor_z){NVec3Set(&NUT_pCurrentUIDeskSet->BuildAnchor,anchor_x,anchor_y,anchor_z);}
inline void		NUT_UIDesk_SetPositionAnchor(const NVEC3 *panchor){NUT_pCurrentUIDeskSet->PositionAnchor = *panchor;}
inline void		NUT_UIDesk_SetPositionAnchorf(const Nf32 anchor_x, const Nf32 anchor_y, const Nf32 anchor_z){NVec3Set(&NUT_pCurrentUIDeskSet->PositionAnchor,anchor_x,anchor_y,anchor_z);}
inline void		NUT_UIDesk_SetBuildAndPositionAnchor(const NVEC3 *panchor){NUT_pCurrentUIDeskSet->BuildAnchor = *panchor;NUT_pCurrentUIDeskSet->PositionAnchor = *panchor;}
inline void		NUT_UIDesk_SetBuildAndPositionAnchorf(const Nf32 anchor_x, const Nf32 anchor_y, const Nf32 anchor_z){NVec3Set(&NUT_pCurrentUIDeskSet->BuildAnchor,anchor_x,anchor_y,anchor_z);NUT_pCurrentUIDeskSet->PositionAnchor = NUT_pCurrentUIDeskSet->BuildAnchor;}

inline void		NUT_UIDesk_SetBlend(const NUT_UIDESKCOMPONENT_ENUM uicrs_id, const NBLEND *pblend){NUT_pCurrentUIDeskSet->pBlend[uicrs_id] = (NBLEND*)pblend;}
inline void		NUT_UIDesk_SetMaterial(const NUT_UIDESKCOMPONENT_ENUM uicrs_id, const NMATERIAL *pmat){NUT_pCurrentUIDeskSet->pMaterial[uicrs_id] = (NMATERIAL*)pmat;}
inline void		NUT_UIDesk_SetAlternativeColorRange(const NUT_UIDESKCOMPONENT_ENUM uielement_id, const NCOLOR *pcolor_range, const Nu32 range_size ){NCopyColorRange(NUT_pCurrentUIDeskSet->AlternativeColorRange[uielement_id], CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE,pcolor_range,range_size);}
inline void		NUT_UIDesk_SetAlternativeColorf(const NUT_UIDESKCOMPONENT_ENUM uielement_id, const Nf32 r, const Nf32 v, const Nf32 b, const Nf32 a ){NSetColorRangef(NUT_pCurrentUIDeskSet->AlternativeColorRange[uielement_id], CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE, r,v,b,a );}
																																					  
				

inline void		NUT_UIDesk_SetOuterMarginf(const Nf32 margin_x, const Nf32 margin_y, const Nf32 margin_z){NVec3Set(&NUT_pCurrentUIDeskSet->OuterMargin,margin_x,margin_y,margin_z);}
inline void		NUT_UIDesk_SetOuterMargin(const NVEC3 *pmargin){NUT_pCurrentUIDeskSet->OuterMargin=*pmargin;}
inline void		NUT_UIDesk_SetInnerSpacingf(const Nf32 space_x, const Nf32 space_y, const Nf32 space_z){NVec3Set(&NUT_pCurrentUIDeskSet->InnerSpacing,space_x,space_y,space_z);}
inline void		NUT_UIDesk_SetInnerSpacing(const NVEC3 *pspacing){NUT_pCurrentUIDeskSet->InnerSpacing=*pspacing;}

inline void		NUT_PushUIDeskSet()
{
	NErrorIf(NUT_pCurrentUIDeskSet == &NUT_UIDeskSet[NUT_UIDESKSET_STACKSIZE-1], NERROR_STACK_OVERFLOW);
	*(NUT_pCurrentUIDeskSet+1)=*NUT_pCurrentUIDeskSet;NUT_pCurrentUIDeskSet++;
	
	#ifdef _NOUT_SYS_UTIL
	printf("\n Push->UIStack LEVEL %d",((NBYTE*)NUT_pCurrentUIDeskSet-(NBYTE*)&NUT_UIDeskSet[0])/sizeof(NUT_UIDESK_SET));
	#endif
}
inline void		NUT_PopUIDeskSet()
{
	NErrorIf(NUT_pCurrentUIDeskSet == &NUT_UIDeskSet[0], NERROR_STACK_OVERFLOW);
	NUT_pCurrentUIDeskSet--;
	
	#ifdef _NOUT_SYS_UTIL
	printf("\n Pop<-UIStack LEVEL %d",((NBYTE*)NUT_pCurrentUIDeskSet-(NBYTE*)&NUT_UIDeskSet[0])/sizeof(NUT_UIDESK_SET));
	#endif
}
void			NUT_ClearUIDeskSet();
void			NUT_ClearUIDeskSetStack();

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// LOWLEVEL/PRIVATE FUNCTIONS
void			NUT_InitializeUIDeskSet();

void			NUT_UIDesk_Apply_TouchMode(  NUI_DESC *pdesc);
void			NUT_UIDesk_Apply_Blend_And_Material( const NUT_UIDESKCOMPONENT_ENUM ui_element_id, NUIGEOMETRY_DESC *pgdesc);
void			NUT_UIDesk_Apply_AlternativeColorRange( const NUT_UIDESKCOMPONENT_ENUM ui_element_id, NUIGEOMETRY_DESC *pgdesc);

void			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors( NVEC3 *prelativ_pos,NVEC3 *pparentanchor, NVEC3 *panchor,const Nbool parent_is_bkg );
void			NUT_UIDesk_ApplyUI_Size( NVEC3 *pdst_uisize, const NVEC3 *psrc_bkgtexture_size, const NVEC3 *psrc_icon_size, const NVEC3 *psrc_text_size, const NVEC3 *psrc_xtra_size );
void			NUT_UIDesk_Apply_TextAlignment(NUITEXT_DESC *ptdesc);

void			NUT_UIDesk_SetUIPositionf(NUI *puiframe, const NVEC3 *puiframesize, const Nf32 x, const Nf32 y, const Nf32 z);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_UIDESK_SET_H 

