#include "lib/N/NCStandard.h"
#include "lib/N/UI/NUI.h"
#include "lib/N/UI/NUICustom.h"
#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

static inline void _PrepareBasicUICustomDesc(NUI_CUSTOM_DESC *pgvdesc, Nchar *pframe_caption, const Nbool copy_caption_as_uiname, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const NVEC3 *pxtrageom_shape_size, const Nu16 extra_geom_capacity, const NUI_EVENT_HANDLE event_proc)
{

	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);
	//	NErrorIf(!pframe_caption && !patlas_iconrefname && !patlas_backrefname && !pxtrageom_shape_size,NERROR_INCONSISTENT_PARAMETERS);

	NATLAS_ELEMENT *pel;
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
	// +
	pgvdesc->UIDesc.pStyle = NUT_pCurrentUIDeskSet->pStyle;
	pgvdesc->UIDesc.GeometryPreAllocatedCapacity += extra_geom_capacity;
	pgvdesc->UIDesc.Event_Proc = event_proc;

	if (!pgvdesc->UIDesc.Event_Proc)
	{
		FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_DISABLE);
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +
	// + ... Not relevant if FLAG_NUIDC_REFUSE_TOUCH_FOCUS is set to ON (no Event Proc) !

	// 	if( ISFLAG_OFF( pgvdesc->UIDesc.Flags_Core,FLAG_NUIDC_DISABLE ) ) ... Not a good idea after all
	// 	{
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_PUSH))
	{
		FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_UI_TRANSFORMHNODE);				 // It's enough, for a push UI, to enable push animation ...
		FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS); // NUT PUSH controllers doing like this...
	}																					 // instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice
																						 // 	} .................................................................... Not a good idea after all

	// ... This one excepted but it will be done .. just few lines down.
	// if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
	// 		FLAG_ON( pbuttondesc->UIDesc.UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + TOUCH MODE
	// +
	NUT_UIDesk_Apply_TouchMode(&pgvdesc->UIDesc);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + ICON GEOMETRY
	// + UICustom may have an ICON but its not an obligation.
	// +
	if (patlas_iconrefname)
	{
		pel = NLookupAtlasElement(patlas_iconrefname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON);
		pgvdesc->UIDesc.GeometryPreAllocatedCapacity += 1;

		FLAG_ON(pgvdesc->UIDesc.UIIconDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
		FLAG_ON(pgvdesc->UIDesc.UIIconDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		pgvdesc->UIDesc.UIIconDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
		NSetAppearance_Raw_Texture(&pgvdesc->UIDesc.UIIconDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_ICON_TEXTUREUNIT, pel->pTexture);
		NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pgvdesc->UIDesc.UIIconDesc, pel);
		// NGetScreenRectFromAtlasElement(pel,&pbuttondesc->UIDesc.UIIconDesc.ScrUVRect);

		icon_size.z = 0.0f;
		NGetAtlasElementHRelSize((NVEC2 *)&icon_size, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
		// NGetUIGeometryDescTextureSize( &icon_size, &pbuttondesc->UIDesc.UIIconDesc,CONSTANT_NUT_UIDESKSET_ICON_TEXTUREUNIT );
		NSetUIGeometryDescSize(&pgvdesc->UIDesc.UIIconDesc, &icon_size);

		//	Icon 3D placement relative to Background (real or virtual,we don't know yet).
		NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
		NSetUIGeometryDescAnchor(&pgvdesc->UIDesc.UIIconDesc, &anchor);
		NSetUIGeometryDescParent(&pgvdesc->UIDesc.UIIconDesc, &pgvdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		NSetUIGeometryDescRelativePosition(&pgvdesc->UIDesc.UIIconDesc, &relative_pos);

		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_ICON, &pgvdesc->UIDesc.UIIconDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_ICON, &pgvdesc->UIDesc.UIIconDesc);

		// for Size calculation
		picon_size = &icon_size;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CAPTION/TEXT
	// + Geometry Viewer may have a caption(text) but its not an obligation.
	// +
	if (pframe_caption)
	{
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON(pgvdesc->UIDesc.UITextDesc.Flags, FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE);

		// UI Text displayer created only if there is a caption string !
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME);
		pgvdesc->UIDesc.pName = pframe_caption;
		if (copy_caption_as_uiname)
			FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_COPY_NAME_STRING);

		NGetStringSize(&text_size, pframe_caption, pgvdesc->UIDesc.pStyle->pFont);
		NSetUITextDescSize(&pgvdesc->UIDesc.UITextDesc, &text_size);

		//	Text 3D placement relative to Icon or Background(real or virtual,we don't know yet).
		if (patlas_iconrefname)
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUITextDescParent(&pgvdesc->UIDesc.UITextDesc, &pgvdesc->UIDesc.UIIconDesc.Placement, &parent_origin);
		}
		else
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
			NSetUITextDescParent(&pgvdesc->UIDesc.UITextDesc, &pgvdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		}
		NSetUITextDescAnchor(&pgvdesc->UIDesc.UITextDesc, &anchor);
		NSetUITextDescRelativePosition(&pgvdesc->UIDesc.UITextDesc, &relative_pos);

		NUT_UIDesk_Apply_TextAlignment(&pgvdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pgvdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pgvdesc->UIDesc.UITextDesc);

		// for Size calculation
		ptext_size = &text_size;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BACKGROUND GEOMETRY
	// + UICustom may have a BACKGROUND its not an obligation.
	// + But in all cases we have to figure out its size en position.
	// + NUI setup is going to create a background only if 'FLAG_NUIDS_BACKGROUND' is set to on.
	// + So if we don't want to build it, just set this flag to OFF, but keep using "backgroundesc.placement" it's easier !
	NSetUIGeometryDescAnchor(&pgvdesc->UIDesc.UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pgvdesc->UIDesc.UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pgvdesc->UIDesc.UIBackgroundDesc, 0, 0, 0);

	if (patlas_backrefname || ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_DEFAULT_BKGD))
	{
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND);
		pgvdesc->UIDesc.GeometryPreAllocatedCapacity += 1;

		if (patlas_backrefname)
		{
			pel = NLookupAtlasElement(patlas_backrefname);
			NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);
			FLAG_ON(pgvdesc->UIDesc.UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
			FLAG_ON(pgvdesc->UIDesc.UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
			pgvdesc->UIDesc.UIBackgroundDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
			NSetAppearance_Raw_Texture(&pgvdesc->UIDesc.UIBackgroundDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_BACKGROUND_TEXTUREUNIT, pel->pTexture);
			NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pgvdesc->UIDesc.UIBackgroundDesc, pel);
			// NGetScreenRectFromAtlasElement(pel,&pbuttondesc->UIDesc.UIBackgroundDesc.ScrUVRect);

			bkg_size.z = 0.0f;
			NGetAtlasElementHRelSize((NVEC2 *)&bkg_size, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
			// NGetUIGeometryDescTextureSize(&bkg_size,&pbuttondesc->UIDesc.UIBackgroundDesc,CONSTANT_NUT_UIDESKSET_BACKGROUND_TEXTUREUNIT);
			//  for Size calculation
			pbkg_size = &bkg_size;
		}
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pgvdesc->UIDesc.UIBackgroundDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD, &pgvdesc->UIDesc.UIBackgroundDesc);
	}
	// Size calculation
	NUT_UIDesk_ApplyUI_Size(&bkg_size, pbkg_size, picon_size, ptext_size, pxtrageom_shape_size); // Re-using "bkg_size" to calculate and setup UIDesc.UIBackgroundDesc size. Even if there is no background ( it will be "VIRTUAL" in that case )
	NSetUIGeometryDescSize(&pgvdesc->UIDesc.UIBackgroundDesc, &bkg_size);

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
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	 // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	 // Set to ON as default behavior. User may update it after creation
		NUT_UIDesk_Apply_TouchMode(&pgvdesc->UIDesc);
	}
	else
	{
		FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);		  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); // Set to OFF as default behavior. User may update it after creation
	}

	FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);	 // Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND))
	{
		FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
	}
	else if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME))
	{
		if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON))
		{
			// In that special case ( Icon+Text without background) the controller NUT_UIPushButtonf will have a custom picking geometry.
			// This one will be a simple quad with the exact background shape ( the one it would have had if it had been created ).
			// This quad it's like an invisible background... ( like but not the same because only vertex positions will
			// copied and be store by UIframe at creation ).
			FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH);
			pgvdesc->UIDesc.pCustomPickingMesh = NUT_CreateUIPickingQuad(&pgvdesc->UIDesc.UIBackgroundDesc.Placement);
		}
		else // Text alone
		{
			FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT);
		}
	}
	else if (ISFLAG_ON(pgvdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON))
	{
		FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_ICON);
	}
	// 	else	|
	// 	{		|- Useless because its impossible and will generate an error right at the beginning of this function !
	// 	}		|

	// + --- + Always ON
	FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_PRECISE_PICKING);
	FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);
	FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);
	FLAG_ON(pgvdesc->UIDesc.Flags_Core, FLAG_NUIDC_PUSHBUTTONCOMMAND_UP);
}

/**
 *------------------------------------------------------------------------------------------
 * \fn	NUI_CUSTOM NUT_CreateNonFocusableUICustom
 *------------------------------------------------------------------------------------------
 *\brief
 *		Create a Simple UI UI Custom without Xtra geometries inside at creation.
 *		They will be inserted/removed after creation thanks 'NUT_InsertPictureInUICustom'...
 *		... and 'NEraseUICustomExtraGeometry' functions.
 *		It may have a Caption or Not (If not, UI TextDisplayer are not going to be created)
 *		It HAS NO Background, and NO ICON.
 *		(It will Only have inserted Xtra geometries)
 *------------------------------------------------------------------------------------------
 *\param
 *		NUI		*parent				Parent UI ( a menu, a layer, ... all NUI* are working )
 *		Nf32			x,y,z				UI local position. (ignored if a NUT_UIDesk_Panel is Open )
 *		NVEC3		*pbuild_size		Building Size. Crucial parameter for NUT_UIDesk_Panel insertion.
 *											This NUT Controller doesn't have a NGEOMETRYDESC at creation, so it doesn't have any
 *											Size information. But NUT_UIDesk Set positioning and Panel insertion need one clear SIZE
 *											to work. So, user has to figure out this size if he wants define UICustom position
 *											with NUT_UIDesk advanced tools (Position anchoring and/or Panel).
 *											'pbuild_size' may be NULL, in that case none of the NUT_UIDesk advanced positioning tools
 *											will work... and the Custom position will be (x,y,z).
 *		Nchar			*pcaption			Displayed UI name (string is not going to be stored as a copy into the uiframe)
 *		Nu16			extra_geom_capacity	Maximum number of UI included extra-geometries.
 *		Nu32			user32				User define value
 *\return
 *		NUI *		Ptr on the created UI.
 *------------------------------------------------------------------------------------------
 */
/*
NUI_CUSTOM* NUT_CreateNonFocusableUICustom(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const NVEC3 *pbuild_size, const Nchar *pcaption, const Nu16 extra_geom_capacity, const Nu32 user32 )
{
	NUI_CUSTOM_DESC	uicustomdesc;
	NUI_CUSTOM*		pui;

	memset(&uicustomdesc,0,sizeof(NUI_CUSTOM_DESC));

	_PrepareBasicUICustomDesc(&uicustomdesc,NULL,extra_geom_capacity,pcaption);
	pui = NCreateUICustom(parent,&uicustomdesc,user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI*)pui,pbuild_size,x,y,z);

	// !!! Some building stuff to delete ???
	// A custom picking mesh was created and has to be destroyed !
	// Useful Mesh contents (vertex positions and triangle indexes actually ) has been copied by NUI creation process...
	// ... and doesn't need "buttondesc.UIDesc.pCustomPickingMesh" any more. So let's delete it right now !
	if(ISFLAG_ON( uicustomdesc.UIDesc.Flags_Core,FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH ) )
		NDeleteMesh(uicustomdesc.UIDesc.pCustomPickingMesh);

	return pui;
}
*/

// ------------------------------------------------------------------------------------------
// NUI_CUSTOM* NUT_CreateUICustom
// ------------------------------------------------------------------------------------------
// Description :
//	Create an UI Custom Controller without any Custom geometry Inside. They will be add in a second pass thanks to the
//	function 'NUT_InsertPictureInUICustom'.
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Used.		|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Used. Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Used. Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ).
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Used. Default BKG if no BKG Atlas Element definition ?
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					Used.	|
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					Used.	|_DISTRIBUTION
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					Used.	|
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				Used.	|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Used. |
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Used. |- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Used. |
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Used. |
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
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
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUI_CUSTOM *NUT_CreateUICustom(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const NVEC3 *pxtrageom_shape_size, const Nu16 extra_geom_capacity, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_CUSTOM_DESC uicustomdesc;
	NUI_CUSTOM *pui;

	memset(&uicustomdesc, 0, sizeof(NUI_CUSTOM_DESC));
	_PrepareBasicUICustomDesc(&uicustomdesc, pframe_caption, NFALSE, patlas_backrefname, patlas_iconrefname, pxtrageom_shape_size, extra_geom_capacity, event_proc);
	pui = NCreateUICustom(parent, &uicustomdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pui, &uicustomdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);

	// !!! Some building stuff to delete ???
	// A custom picking mesh was created and has to be destroyed !
	// Useful Mesh contents (vertex positions and triangle indexes actually ) has been copied by NUI creation process...
	// ... and doesn't need "buttondesc.UIDesc.pCustomPickingMesh" any more. So let's delete it right now !
	if (ISFLAG_ON(uicustomdesc.UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH))
		NDeleteMesh(uicustomdesc.UIDesc.pCustomPickingMesh);

	return pui;
}

// ------------------------------------------------------------------------------------------
// void NUT_InsertPictureInUICustom
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a Picture into an UI Custom
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Not Used.	|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Not Used.	 Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used.	 Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ).
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Not Used.	 Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used.	 Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Not Used.	 Default BKG if no BKG Atlas Element definition ?
//		FLAG_NUT_UIDESKSET_X_DISTRIBUTION					Not Used.|
//		FLAG_NUT_UIDESKSET_Y_DISTRIBUTION					Not Used.|_DISTRIBUTION
//		FLAG_NUT_UIDESKSET_Z_DISTRIBUTION					Not Used.|
//		FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION				Not Used.|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE			Not Used.		|
//		FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS		Not Used.		|
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE			Not Used.		|- Size Constraints
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN		Not Used.		|
//		FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX		Not Used.		|
//
//	List of NUT_UiCreationSet Params effectively used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		Nu32					PropertiesFlags;			Used. List of used FLAGS just above.
//		NUITOUCH_MODE_ENUM		TouchMode;					Not Used. If associated FLAG_NUT_UIDESKSET_USE_TOUCH_MODE is SET to ON.
//		NVEC3				SizeConstraints;			Not Used. According with FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_xxx FLAGS.
//		NVEC3				OuterMargin;				Not Used.
//		NVEC3				InnerSpacing;				Not Used.
//		NUI_STYLE				*pStyle;					Not Used.
//		NVEC3				BuildAnchor;				Not Used.
//		NVEC3				PositionAnchor;				Not Used.
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Not Used.
//		NBLEND					*pBlend[];					Used. If associated FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Used. If associated FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Used. If associated FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR is SET to ON.
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_InsertPictureInUICustom(NUI_CUSTOM *pgv, const Nchar *patlas_refname, const NBUILDBASE_COLOR *pcolorbuild, const Nbool bdisplay)
{
	NErrorIf(!patlas_refname, NERROR_NULL_POINTER);
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	if (patlas_refname)
	{
		NATLAS_ELEMENT *pel;

		pel = NLookupAtlasElement(patlas_refname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		if (pel)
		{
			NUIGEOMETRY_DESC gdesc;
			Nmem0(&gdesc, NUIGEOMETRY_DESC);

			FLAG_ON(gdesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
			FLAG_ON(gdesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
			if (pcolorbuild)
			{
				gdesc.ColorBuild = *pcolorbuild;
			}
			gdesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;

			NSetAppearance_Raw_Texture(&gdesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_CUSTOM_PICTURE_TEXTUREUNIT, pel->pTexture);
			NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&gdesc, pel);
			// 			NGetScreenRectFromAtlasElement(pel,&gdesc.ScrUVRect);

			NSetUIGeometryDescAnchor(&gdesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
			NSetUIGeometryDescParentf(&gdesc, NULL, 0, 0, 0);
			NSetUIGeometryDescRelativePositionf(&gdesc, 0, 0, 0);

			//			NSetUIGeometryDescSizeFromTexture(&gdesc, CONSTANT_NUT_UIDESKSET_CUSTOM_PICTURE_TEXTUREUNIT );
			NSetUIGeometryDescSizeFromAtlasElementSize(&gdesc, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));

			NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_MISC, &gdesc);
			NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_MISC, &gdesc);

			Nu32 extrageomid = NInsertUICustomExtraGeometry(pgv, &gdesc);
			if (bdisplay)
			{
				NDisplayUICustomGeometry(pgv, extrageomid, FLAG_NUI_CUSTOM_EXTRA_GEOMETRY_INDEX_BASE);
			}
		}
	}
}
