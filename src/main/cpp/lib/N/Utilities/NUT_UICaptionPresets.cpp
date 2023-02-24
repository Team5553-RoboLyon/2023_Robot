#include "lib/N/NCStandard.h"
#include "../UI/NUI.h"
#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

// ------------------------------------------------------------------------------------------
// NUT_CreateUICaptionf
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Simple Caption,
//				It doesn't capture any Touch, so no interactivity with it. It's just ... a caption.
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			NOT used.
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		NOT used.
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			NOT used.
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		NOT used.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Used.
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Used.
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			NOT used.
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		NOT used.
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					NOT used.
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					NOT used.
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						NOT used.
//		FLAG_NUT_UIDESKSET_PUSH								NOT used.
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						NOT used.
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					NOT used.
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					NOT used.
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					NOT used.
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				NOT used.
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			NOT used.
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		NOT used.
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			NOT used.
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		NOT used.
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		NOT used.
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					NOT used.
//		NVEC3				SizeConstraints;			NOT used.
//		NVEC3				OuterMargin;				NOT used.
//		NVEC3				InnerSpacing;				NOT used.
//		NUI_STYLE				*pStyle;					Used.
//		NVEC3				BuildAnchor;				Used.
//		NVEC3				PositionAnchor;				Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Used.
//		NBLEND					*pBlend[];					Used. If associated FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Used. If associated FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Used. If associated FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR is SET to ON.
//
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent				Parent UI ( a menu, a layer, ... all NUI* are working )
//		Nf32			x,y,z				UI position
//		Nchar			*pcaption			displayed UI name (string is not going to be stored as a copy into the uiframe)
//		Nu32			user32				User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI *NUT_CreateUICaptionf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Nu32 user32)
{
	NUI_DESC uidesc;
	NUI *pui;
	NVEC3 tsize;

	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);
	NErrorIf(!pcaption, NERROR_UI_FRAME_NAME_MISSING);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	// +
	memset(&uidesc, 0, sizeof(NUI_DESC));
	uidesc.pName = (Nchar *)pcaption;
	uidesc.pStyle = NUT_pCurrentUIDeskSet->pStyle;
	// uidesc.Event_Proc	= ???; Nothing ! because it's not an interactive controler !

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +
	// ... Not relevant here !
	/*
		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_COLOR_UPDATE))
			FLAG_ON( uidesc.Flags_Style,FLAG_NUIDS_FOCUS_COLORS );

		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_PUSH))
		{
			FLAG_ON(uidesc.UIDesc.Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE); // It's enough, for a push UI, to enable push animation ...
			FLAG_ON(uidesc.UIDesc.Flags_Core,FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS );	// NUT PUSH controllers doing like this...
		}																					// instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice

		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON( uidesc.UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );
	*/

	// UI Caption Text
	NGetStringSize(&tsize, pcaption, uidesc.pStyle->pFont);
	NSetUITextDescSize(&uidesc.UITextDesc, &tsize);
	NSetUITextDescAnchor(&uidesc.UITextDesc, &NUT_pCurrentUIDeskSet->BuildAnchor); // Caption Anchoring
	NSetUITextDescParentf(&uidesc.UITextDesc, NULL, 0, 0, 0);
	NSetUITextDescRelativePositionf(&uidesc.UITextDesc, 0, 0, 0);

	NUT_UIDesk_Apply_TextAlignment(&uidesc.UITextDesc);
	NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&uidesc.UITextDesc);
	NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&uidesc.UITextDesc);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC/UIDS Flags.
	// + Setup  UIDC & UIDS Flags.
	// +
	// + --- + Event Management Flags
	// NUT supposes user wants to receive all events if he defines an "event_proc", and in an opposite way ...
	// ... NUT supposes user doesn't want to receive any event if he doesn't define an "event_proc".
	// ---> Because it's not possible for user to define an event_proc for this kind of UI .... All flags are going to be set to OFF !
	// ---> Set all of them to OFF is useless because of previous "Nmem0"
	/*
		if(event_proc)
		{

			FLAG_ON( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	// Set to ON as default behavior. User may update it after creation
			FLAG_ON( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	// Set to ON as default behavior. User may update it after creation
			FLAG_ON( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UICORE_EVENT);		// Set to ON as default behavior. User may update it after creation
			NUT_UIDesk_Apply_TouchMode(&uidesc);
		}
		else
		{
			FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	// Set to OFF as default behavior. User may update it after creation
			FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	// Set to OFF as default behavior. User may update it after creation
			FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_LISTEN_UICORE_EVENT);	// Set to OFF as default behavior. User may update it after creation
			FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE);	// Set to OFF as default behavior. User may update it after creation
		}

		FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_INTERCEPT_UICORE_EVENT);		// Set to OFF as default behavior. User may update it after creation
		FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	// Set to OFF as default behavior. User may update it after creation
		FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT);	// Set to OFF as default behavior. User may update it after creation
		FLAG_OFF( pdesc->UIDesc.Flags_Style,FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);		// Set to OFF as default behavior. User may update it after creation
	*/

	// + --- + Conditional
	//	... There are no conditional Flags to set !
	// + --- + Always ON
	FLAG_ON(uidesc.Flags_Core, FLAG_NUIDC_DISABLE);
	FLAG_ON(uidesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
	FLAG_ON(uidesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME);

	// None of the Event Flags is set to ON so, no event listening and no event interception !
	pui = NCreateUI(parent, &uidesc, user32);
	NUT_UIDesk_ApplyUI_Size(&tsize, NULL, NULL, &tsize, NULL);
	NUT_UIDesk_SetInPanelUIPositionf(pui, &tsize, x, y, z);

	return pui;
}

/*
NUI* NUT_CreateUICaptionExf(NUI *parent, const Nf32 x, const Nf32 y, const Nchar *pcaption, const Nbool copy_caption_as_uiname, const Nchar *patlas_backrefname, const Nu32 user32 )
{
	NUI_DESC	uidesc;
	NUI*		pui;
	NVEC3		tsize;
	NATLAS_ELEMENT	*pel;

	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle,	NERROR_UI_STYLE_MISSING);
	NErrorIf(!pcaption, NERROR_UI_FRAME_NAME_MISSING);

	memset(&uidesc,0,sizeof(NUI_DESC));

	// Just a simple caption text, without any interaction
	FLAG_ON(uidesc.Flags_Core,FLAG_NUIDC_REFUSE_TOUCH_FOCUS);
	FLAG_ON(uidesc.Flags_Style,FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
	if(copy_caption_as_uiname)
		FLAG_ON(uidesc.Flags_Style,FLAG_NUIDS_COPY_NAME_STRING);
	uidesc.pName			= (Nchar*)pcaption;
	uidesc.pStyle		= NUT_pCurrentUIDeskSet->pStyle;
	// uidesc.Event_Proc	= ???; Nothing ! because it's not an interactive controler !

	// UI Caption Text
	NGetStringSize(pcaption,uidesc.pStyle->pFont,(NVEC2*)&tsize);
	tsize.z = 0.0f;
	NSetUITextDescSize(&uidesc.UITextDesc,&tsize);
	NSetUITextDescAnchorf(&uidesc.UITextDesc,ANCHOR_MIDWIDTH,ANCHOR_MIDHEIGHT,0); // Text Anchoring
	NSetUITextDescParentf(&uidesc.UITextDesc,&uidesc.UIBackgroundDesc.Placement,ANCHOR_MIDWIDTH,ANCHOR_MIDHEIGHT,0);
	NSetUITextDescRelativePositionf(&uidesc.UITextDesc,0,0,0);
	NUT_UIDesk_ApplyFrameText_Blend_And_Material(&uidesc.UITextDesc);

	// UI Background
	FLAG_ON( uidesc.Flags_Style,FLAG_NUIDS_BACKGROUND );
	uidesc.GeometryPreAllocatedCapacity = 1;
	NSetUIGeometryDescAnchor(&uidesc.UIBackgroundDesc,&NUT_pCurrentUIDeskSet->Anchor);
	NSetUIGeometryDescParentf(&uidesc.UIBackgroundDesc,NULL,0,0,0);
	NSetUIGeometryDescRelativePositionf(&uidesc.UIBackgroundDesc,0,0,0);
	tsize.x+=2.0f*uidesc.pStyle->Margin.x;
	tsize.y+=2.0f*uidesc.pStyle->Margin.y;
	//tsize.z+=2.0f*uidesc.pStyle->Margin.z;
	NSetUIGeometryDescSize(&uidesc.UIBackgroundDesc, &tsize );

	// Rule for this specific control:
	//		with or without texture, background size will be size caption string size + style margins!
	if( patlas_backrefname)
	{
		pel = NLookupAtlasElement(patlas_backrefname);
		if(pel)
		{
			FLAG_ON(uidesc.UIBackgroundDesc.Flags,FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
			uidesc.UIBackgroundDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
			FLAG_ON(uidesc.UIBackgroundDesc.Flags,FLAG_NUIGD_USEPARAM_APPEARANCE);
			NSetAppearance_Raw_Texture(&uidesc.UIBackgroundDesc.Appearance_Raw,0, pel->pTexture);
			FLAG_ON(uidesc.UIBackgroundDesc.Flags,FLAG_NUIGD_USEPARAM_SCRUV);
			NGetScreenRectFromAtlasElement(pel,&uidesc.UIBackgroundDesc.ScrUVRect);
		}
	}
	NUT_UIDesk_ApplyFrameBackground_Blend_And_Material(&uidesc.UIBackgroundDesc);

	// ...
	pui = NCreateUI(parent,&uidesc,user32);
	NSetUIPositionf(pui, x, y, 0.0f);
	return pui;
}
*/
