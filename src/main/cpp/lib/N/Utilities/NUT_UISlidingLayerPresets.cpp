#include "lib/N/NCStandard.h"
#include "lib/N/UI/NUI.h"

#include "NUT_UIPresets.h"

// ------------------------------------------------------------------------------------------
// NUT_CreateUISlidingLayer
// ------------------------------------------------------------------------------------------
// Description :
//
//				A Sliding layer,
//				Is not Visible
//				Has no graphic representation
//				Has no texture, no caption ...
//				... So, no geometry extraction !
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
NUI_SLIDE *NUT_CreateUISlidingLayer(NUI *parent, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_SLIDE_DESC slidedesc;

	memset(&slidedesc, 0, sizeof(NUI_SLIDE_DESC));
	FLAG_ON(slidedesc.UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_KILL_CHILD_TOUCH_FOCUS);
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT); // To allow Sliding Layer to slide when one of its child has the Touch Focus
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_NO_GEOMETRY_EXTRACTION);
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_SWIPE);
	slidedesc.UIDesc.Event_Proc = event_proc;

	// Notice:
	//	Layer is not going to be managed by NUT_UIdesk ... tools for placement. Obviously, its a layer !
	return NCreateUISlide(parent, &slidedesc, user32);
}

// ------------------------------------------------------------------------------------------
// NUT_CreateUISlidingFramedLayer
// ------------------------------------------------------------------------------------------
// Description :
//
//				A Framed Sliding layer,
//				Is exactly like a Sliding Layer, PLUS ...
//				Its BoundingBox surround all its children instead of being equal to the whole screen surface.
//				This Bounding Box will ALWAYS surround all its children and so on, will be updated each time
//				children BBox change.
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
// --------------------------------------------------- ---------------------------------------
NUI_SLIDE *NUT_CreateUISlidingFramedLayer(NUI *parent, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_SLIDE_DESC slidedesc;

	memset(&slidedesc, 0, sizeof(NUI_SLIDE_DESC));
	FLAG_ON(slidedesc.UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX);
	FLAG_ON(slidedesc.UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_KILL_CHILD_TOUCH_FOCUS);
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT); // To allow Sliding Layer to slide when one of its child has the Touch Focus
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_NO_GEOMETRY_EXTRACTION);
	FLAG_ON(slidedesc.UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_SWIPE);
	slidedesc.UIDesc.Event_Proc = event_proc;

	// Notice:
	//	Layer is not going to be managed by NUT_UIdesk ... tools for placement. Obviously, its a layer !
	return NCreateUISlide(parent, &slidedesc, user32);
}
