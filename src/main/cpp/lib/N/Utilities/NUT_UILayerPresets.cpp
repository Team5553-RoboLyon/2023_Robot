#include "lib/N/NCStandard.h"
#include "../UI/NUI.h"
#include "../UI/NUINull.h"

#include "NUT_UIPresets.h"

/*

Obsolete ...
	Prefer using directly 'NCreateUINullXtd'
// ------------------------------------------------------------------------------------------
// NUT_CreateUILayer
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Simple layer,
//				Is not Visible
//				Has no graphic representation
//				Has no texture, no caption ...
//				... So, no geometry extraction !
//				It can get and keep the focus ONLY if one of its children has the focus too...
//
//	None of the NUT_UiCreationSet Properties Flags and Params are used !
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Not Used.	|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Not Used. Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ).
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Not Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Not Used. Default BKG if no BKG Atlas Element definition ?
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					Not Used.	|
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					Not Used.	|_DISTRIBUTION
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					Not Used.	|
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				Not Used.	|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Not Used. |
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Not Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Not Used. |- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Not Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Not Used. |
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Not Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Not Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
//		NVEC3				SizeConstraints;			Not Used. According with FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_xxx FLAGS.
//		NVEC3				OuterMargin;				Not Used.
//		NVEC3				InnerSpacing;				Not Used.
//		NUI_STYLE				*pStyle;					Not Used.
//		NVEC3				BuildAnchor;				Not Used.
//		NVEC3				PositionAnchor;				Not Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Not Used.
//		NBLEND					*pBlend[];					Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_ALTERNATIVECOLOR is SET to ON.
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent			Parent UI ( a menu, a layer, ... all NUI* are working )
//		NUI_EVENT_HANDLE	event_proc		A pointer on an event handle to allow use managing events received by the Layer.
//		Nu32			user32			User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI* NUT_CreateUILayer(NUI *parent, const NUI_EVENT_HANDLE event_handle, const Nu32 listen_intercept_flags_style, NUI_LISTEN_TOUCH_MODE_ENUM touch_mode, const Nu32 user32 )
{
	return NCreateUINullXtd( parent,event_handle,listen_intercept_flags_style,touch_mode,user32 );
}



// ------------------------------------------------------------------------------------------
// NUT_CreateUIFramedLayer
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Simple Framed Layer,
//				Is exactly like a Simple Layer PLUS,
//				Its BoundingBox surround its children Bounding Boxes and is updated each time one (or more) children bounding changes.
//
//
//	None of the NUT_UiCreationSet Properties Flags and Params are used !
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Not Used.	|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Not Used. Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ).
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Not Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Not Used. Default BKG if no BKG Atlas Element definition ?
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					Not Used.	|
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					Not Used.	|_DISTRIBUTION
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					Not Used.	|
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				Not Used.	|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Not Used. |
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Not Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Not Used. |- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Not Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Not Used. |
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Not Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Not Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
//		NVEC3				SizeConstraints;			Not Used. According with FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_xxx FLAGS.
//		NVEC3				OuterMargin;				Not Used.
//		NVEC3				InnerSpacing;				Not Used.
//		NUI_STYLE				*pStyle;					Not Used.
//		NVEC3				BuildAnchor;				Not Used.
//		NVEC3				PositionAnchor;				Not Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Not Used.
//		NBLEND					*pBlend[];					Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Not Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_ALTERNATIVECOLOR is SET to ON.
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent			Parent UI ( a menu, a layer, ... all NUI* are working )
//		NUI_EVENT_HANDLE	event_proc		A pointer on an event handle to allow use managing events received by the Layer.
//		Nu32			user32			User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI* NUT_CreateUIFramedLayer(NUI *parent, const NUI_EVENT_HANDLE event_handle, const Nu32 listen_intercept_flags_style, NUI_LISTEN_TOUCH_MODE_ENUM touch_mode, const NRECTf32 *pbbox, const Nu32 user32 )
{
	// Notice:
	//	Layer is not going to be managed by NUT_UIdesk ... tools for placement. Obviously, its a layer !

	// Check that only listen and/or intercept uidesc_style flags are used ...
	NErrorIf(!FLAGS_TEST(listen_intercept_flags_style,MASK_NUIDS_LISTEN_EVENT|MASK_NUIDS_INTERCEPT_EVENT,listen_intercept_flags_style), NERROR_INCONSISTENT_PARAMETERS);
	// UINull needs an event_handle to listen and/or intercept events ...
	NErrorIf(listen_intercept_flags_style && !event_handle, NERROR_INCONSISTENT_PARAMETERS);
	// If user define "touch_mode", he has to set FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE to ON !
	NErrorIf(ISFLAG_OFF(listen_intercept_flags_style,FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE) && touch_mode, NERROR_INCONSISTENT_PARAMETERS);

	NUI_DESC	uidesc;

	memset(&uidesc,0,sizeof(NUI_DESC));
	uidesc.Event_Proc = event_handle;
	FLAG_ON(uidesc.Flags_Style,FLAG_NUIDS_NO_GEOMETRY_EXTRACTION);
	FLAG_ON(uidesc.Flags_Style,listen_intercept_flags_style);

	if(pbbox)
	{
		uidesc.BoundingBox = *pbbox;
		FLAG_ON(uidesc.Flags_Core,FLAG_NUIDC_USEPARAM_BOUNDINGBOX);
	}
	else
	{
		FLAG_ON(uidesc.Flags_Core,FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX);
	}

	//FLAG_ON(uidesc.Flags_Style,FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); not necessary thx to the previous 'NErrorIf' check
	uidesc.TouchMode = touch_mode;
	return  NCreateUI(parent,&uidesc,user32);
}


*/
