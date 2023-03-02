#include "lib/N/NCStandard.h"
#include "../UI/NUI.h"
#include "../BasicElements/NTextureAtlas.h"
#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

static inline void _PrepareBasicUIPictureDesc(NUI_DESC *pframedesc, const Nchar *patlas_refname, const NUI_EVENT_HANDLE event_proc)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NATLAS_ELEMENT *pel;
	NVEC3 size;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	// +
	pframedesc->pStyle = NUT_pCurrentUIDeskSet->pStyle;
	pframedesc->Event_Proc = event_proc;
	pframedesc->GeometryPreAllocatedCapacity = 1;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pframedesc->Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	// ... Not relevant here !
	/*
		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_PUSH))
		{
			FLAG_ON(pframedesc->UIDesc.Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE); // It's enough, for a push UI, to enable push animation ...
			FLAG_ON(pframedesc->UIDesc.Flags_Core,FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS ); // NUT PUSH controllers doing like this...
		}																							// instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice

		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON( pframedesc->UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );
	*/
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Picture GEOMETRY ( == BACKGROUND GEOMETRY actually)
	// + UI Picture have a BACKGROUND its an obligation.
	// + Actually UIFRame Picture is 'just' that ... a BACKGROUND with a texture !
	// + ( without a valid texture, it's going to be a FLAT quad )
	// +
	NVec3Set(&size, 1.0f, 1.0f, 0.0f); // Default: a 1x1 quad background
	if (patlas_refname)
	{
		pel = NLookupAtlasElement(patlas_refname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		if (pel)
		{
			FLAG_ON(pframedesc->UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
			FLAG_ON(pframedesc->UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
			pframedesc->UIBackgroundDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
			NSetAppearance_Raw_Texture(&pframedesc->UIBackgroundDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_PICTURE_TEXTUREUNIT, pel->pTexture);
			// NGetScreenRectFromAtlasElement(pel,&pframedesc->UIBackgroundDesc.ScrUVRect);
			NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pframedesc->UIBackgroundDesc, pel);

			// NGetUIGeometryDescTextureSize(&size,&pframedesc->UIBackgroundDesc, CONSTANT_NUT_UIDESKSET_PICTURE_TEXTUREUNIT );
			NGetAtlasElementHRelSize((NVEC2 *)&size, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
			size.z = 0.0f;
		}
	}
	NSetUIGeometryDescAnchor(&pframedesc->UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pframedesc->UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pframedesc->UIBackgroundDesc, 0, 0, 0);

	NUT_UIDesk_ApplyUI_Size(&size, &size, NULL, NULL, NULL);
	NSetUIGeometryDescSize(&pframedesc->UIBackgroundDesc, &size);

	NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pframedesc->UIBackgroundDesc);
	NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD, &pframedesc->UIBackgroundDesc);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC and UIDS Flags.
	// + Setup the all the UIDC/UIDS Flags.
	// +
	// + --- + Event Management Flags
	// NUT supposes user wants to receive all events if he defines an "event_proc", and in an opposite way ...
	// ... NUT supposes user doesn't want to receive any event if he doesn't define an "event_proc".
	if (event_proc)
	{
		FLAG_ON(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	 // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	 // Set to ON as default behavior. User may update it after creation
		NUT_UIDesk_Apply_TouchMode(pframedesc);
		// Activate Automatically BBox if necessary
		if (pframedesc->TouchMode != NUI_LISTEN_TOUCH_MODE_NULL)
		{
			FLAG_ON(pframedesc->Flags_Core, FLAG_NUIDC_PRECISE_PICKING);
			FLAG_ON(pframedesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);
			FLAG_ON(pframedesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);
			FLAG_ON(pframedesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
		}
	}
	else
	{
		FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);		  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); // Set to OFF as default behavior. User may update it after creation
	}

	FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pframedesc->Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);	 // Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	//
	// + --- + Always ON
	FLAG_ON(pframedesc->Flags_Style, FLAG_NUIDS_BACKGROUND);
}

// ------------------------------------------------------------------------------------------
// NUT_CreateUIPicturef
// ------------------------------------------------------------------------------------------
// Description :
//
//				A Simple Picture (quad + texture),
//
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Used.		|
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
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Used. |
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Used. |- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Used. |
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Not Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
//		NVEC3				SizeConstraints;			Used. According with FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_xxx FLAGS.
//		NVEC3				OuterMargin;				Not Used.
//		NVEC3				InnerSpacing;				Not Used.
//		NUI_STYLE				*pStyle;					Used.
//		NVEC3				BuildAnchor;				Used.
//		NVEC3				PositionAnchor;				Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Not Used.
//		NBLEND					*pBlend[];					Used. If associated FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Used. If associated FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Used. If associated FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR is SET to ON.
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent				Parent UI ( a menu, a layer, ... all NUI* are working )
//		Nf32			x,y,z				UI position
//		Nchar			*patlas_refname		Picture (texture) Atlas element name
//		Nu32			user32				User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI *NUT_CreateUIPicturef(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *patlas_refname, const Nu32 user32)
{
	NUI_DESC uidesc;
	NUI *pui;

	memset(&uidesc, 0, sizeof(NUI_DESC));
	_PrepareBasicUIPictureDesc(&uidesc, patlas_refname, NULL);
	pui = NCreateUI(parent, &uidesc, user32);

	NUT_UIDesk_SetInPanelUIPositionf(pui, &uidesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// NSetUIPositionf(pui, x, y, 0.0f);

	return pui;
}

// ------------------------------------------------------------------------------------------
// NUI* NUT_CreateUIPictureXtdf
// ------------------------------------------------------------------------------------------
// Description :
//		A Simple Picture (quad + texture) AND a specific Event_Handle.
//		!!! Be aware that the default behavior of the PictureXtd about EVENT listen and interception is:
//				UICORE		--> NO LISTENING and NO INTERCEPTING !!! User may change it by hand after creation
//				UINOTIFY	--> NO LISTENING and NO INTERCEPTING !!! User may change it by hand after creation
//				UICOMMAND	--> NO LISTENING and NO INTERCEPTING !!! User may change it by hand after creation
//				UITOUCH		--> NO LISTENING and NO INTERCEPTING !!! User may change it by hand after creation
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUI *NUT_CreateUIPictureXtdf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *patlas_refname, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_DESC uidesc;
	NUI *pui;

	memset(&uidesc, 0, sizeof(NUI_DESC));
	_PrepareBasicUIPictureDesc(&uidesc, patlas_refname, event_proc);
	pui = NCreateUI(parent, &uidesc, user32);

	NUT_UIDesk_SetInPanelUIPositionf(pui, &uidesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// NSetUIPositionf(pui, x, y, 0.0f);

	return pui;
}
