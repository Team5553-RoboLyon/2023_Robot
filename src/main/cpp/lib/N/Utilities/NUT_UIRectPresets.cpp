#include "lib/N/NCStandard.h"
#include "../NUsualCoordinates.h"
#include "../UI/NUI.h"

#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

static inline void _PrepareUIRectDesc(NUI_DESC *pdesc, Nchar *pframe_caption, const Nbool copy_caption_as_uiname, const NVEC2 *psize, const NUI_EVENT_HANDLE event_proc)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NVEC3 text_size, icon_size, bkg_size;
	NVEC3 *ptext_size = NULL;
	NVEC3 *picon_size = NULL;
	NVEC3 *pbkg_size = NULL;
	NVEC3 anchor, parent_origin, relative_pos;
	//	NGEOMETRY			*ptmpgeometry = NULL;

	NVec3Set(&text_size, 0, 0, 0);
	NVec3Set(&icon_size, 0, 0, 0);
	NVec3Set(&bkg_size, 0, 0, 0);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	pdesc->GeometryPreAllocatedCapacity = 0;
	pdesc->pStyle = NUT_pCurrentUIDeskSet->pStyle;
	pdesc->Event_Proc = event_proc;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	/*

		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_PUSH))
		{
			FLAG_ON(pdesc->UIDesc.Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE);					// It's enough, for a push UI, to enable push animation ...
			FLAG_ON(pdesc->UIDesc.Flags_Core,FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS );	// NUT PUSH controllers doing like this...
		}																							// instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice

	*/
	//	This one will be done later ... just few lines down.
	// 	if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
	// 		FLAG_ON( pdesc->UIDesc.UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + TOUCH MODE
	// +
	NUT_UIDesk_Apply_TouchMode(pdesc);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CAPTION/TEXT
	// + PusButton may have a caption(text) but its not an obligation.
	// +
	if (pframe_caption)
	{
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON(pdesc->UITextDesc.Flags, FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE);

		// UI Text displayer created only if there is a caption string !
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_DISPLAY_NAME);
		pdesc->pName = pframe_caption;
		if (copy_caption_as_uiname)
			FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_COPY_NAME_STRING);

		NGetStringSize(&text_size, pframe_caption, pdesc->pStyle->pFont);
		NSetUITextDescSize(&pdesc->UITextDesc, &text_size);

		NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
		NSetUITextDescParent(&pdesc->UITextDesc, &pdesc->UIBackgroundDesc.Placement, &parent_origin);

		NSetUITextDescAnchor(&pdesc->UITextDesc, &anchor);
		NSetUITextDescRelativePosition(&pdesc->UITextDesc, &relative_pos);

		NUT_UIDesk_Apply_TextAlignment(&pdesc->UITextDesc);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pdesc->UITextDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pdesc->UITextDesc);

		// for Size calculation
		ptext_size = &text_size;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BACKGROUND GEOMETRY
	// + PusButton may have a BACKGROUND its not an obligation.
	// + But in all cases we have to figure out its size en position.
	// + NUI setup is going to create a background only if 'FLAG_NUIDS_BACKGROUND' is set to on.
	// + So if we don't want to build it, just set this flag to OFF, but keep using "backgroundesc.placement" it's easier !
	NSetUIGeometryDescAnchor(&pdesc->UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pdesc->UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pdesc->UIBackgroundDesc, 0, 0, 0);

	// 	if( ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_DEFAULT_BKGD) )
	// 	{
	FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_BACKGROUND);
	pdesc->GeometryPreAllocatedCapacity += 1;

	if (psize)
	{
		bkg_size.z = 0.0f;
		bkg_size.y = psize->y;
		bkg_size.x = psize->x;

		pbkg_size = &bkg_size;
	}
	NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pdesc->UIBackgroundDesc);
	// NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD,&pdesc->UIBackgroundDesc);
	// 	}
	// Size calculation
	NUT_UIDesk_ApplyUI_Size(&bkg_size, pbkg_size, picon_size, ptext_size, NULL); // Re-using "bkg_size" to calculate and setup UIDesc.UIBackgroundDesc size. Even if there is no background ( it will be "VIRTUAL" in that case )
	NSetUIGeometryDescSize(&pdesc->UIBackgroundDesc, &bkg_size);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC/UIDS Flags.
	// + Setup  UIDC & UIDS Flags.
	// +
	// + --- + Event Management Flags
	// NUT supposes user wants to receive all events if he defines an "event_proc", and in an opposite way ...
	// ... NUT supposes user doesn't want to receive any event if he doesn't define an "event_proc".
	if (event_proc)
	{
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	// Set to ON as default behavior. User may update it after creation
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	// Set to ON as default behavior. User may update it after creation
																		// "LISTEN_TOUCH or NOT ?" This is define by Touch Mode, according with "NUT_UIDesk_Apply_TouchMode" done earlier.
	}
	else
	{
		FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	 // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	 // Set to OFF as default behavior. User may update it after creation
																		 // "LISTEN_TOUCH or NOT ?" This is define by Touch Mode, according with "NUT_UIDesk_Apply_TouchMode" done earlier.
	}

	FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	// Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	// Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pdesc->Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);		// Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	if (ISFLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_BACKGROUND))
	{
		FLAG_ON(pdesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
	}
	else if (ISFLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_DISPLAY_NAME))
	{
		FLAG_ON(pdesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT);
	}
	// 	else	|
	// 	{		|- Useless because its impossible and will generate an error right at the beginning of this function !
	// 	}		|

	// + --- + Always ON
	FLAG_ON(pdesc->Flags_Core, FLAG_NUIDC_PRECISE_PICKING);
	FLAG_ON(pdesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);
	FLAG_ON(pdesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);
}

// ------------------------------------------------------------------------------------------
// NUT_CreateUIRectf
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Simple Flat UI Rectangle, with/without a title
//				It may have a Caption or Not (If not, UI TextDisplayer are not going to be created)
//				It may have an Icon or Not.
//				It may have a Flat Background or Not.
//				(A flat Background is going to be created if a NON NULL 'psize' is specified as Background Size. )
//
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Not Used.	|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Used. Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Used. Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ).
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Not Used. Default BKG if no BKG Atlas Element definition ?
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					Used.	|
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					Used.	|_DISTRIBUTION
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					Used.	|
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				Used.	|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Not Used.	|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Not Used.	|
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Used.		|- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Used.		|
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Used.		|
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Not Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
//		NVEC3				SizeConstraints;			Used. According with FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_xxx FLAGS.
//		NVEC3				OuterMargin;				Used.
//		NVEC3				InnerSpacing;				Used.
//		NUI_STYLE				*pStyle;					Used.
//		NVEC3				BuildAnchor;				Used.
//		NVEC3				PositionAnchor;				Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Used.
//		NBLEND					*pBlend[];					Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_ALTERNATIVECOLOR is SET to ON.
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent				Parent UI ( a menu, a layer, ... all NUI* are working )
//		Nf32			x,y,z				UI position
//		Nchar			*pcaption			displayed UI name (string is not going to be stored as a copy into the uiframe)
//		Nchar			*patlas_backrefname	Background Texture Atlas element name.
//		Nchar			*patlas_iconrefname	Icon Texture Atlas element name.
//		NUI_EVENT_HANDLE	event_proc			A pointer on an event handle to allow use managing events received by the Layer.
//		Nu32			user32				User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI *NUT_CreateUIRectf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const NVEC2 *psize, const Nu32 user32)
{
	NUI_DESC uidesc;
	NUI *pframe;

	memset(&uidesc, 0, sizeof(NUI_DESC));
	_PrepareUIRectDesc(&uidesc, pframe_caption, NTRUE, psize, NULL);
	pframe = NCreateUI(parent, &uidesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe, &uidesc.UIBackgroundDesc.Placement.Size, x, y, z);

	return pframe;
}

NUI *NUT_CreateUIRectXtdf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const NVEC2 *psize, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_DESC uidesc;
	NUI *pframe;

	memset(&uidesc, 0, sizeof(NUI_DESC));
	_PrepareUIRectDesc(&uidesc, pframe_caption, NTRUE, psize, event_proc);
	pframe = NCreateUI(parent, &uidesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe, &uidesc.UIBackgroundDesc.Placement.Size, x, y, z);

	return pframe;
}
