#include "lib/N/NCStandard.h"
#include "lib/N/NUsualCoordinates.h"
#include "lib/N/NString.h"
#include "lib/N/UI/NUICheckBox.h"
#include "lib/N/BasicElements/NTextureAtlas.h"

#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

static inline void _PrepareUICheckBoxDesc(NUI_CHECKBOX_DESC *pcbdesc, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const Nchar *pcaption, const Nchar *pcheck_text, const Nchar *patlas_checkrefname, const Nchar *puncheck_text, const Nchar *patlas_uncheckrefname, const NUI_EVENT_HANDLE event_proc)
{
	Nchar str[NUI_CAPTION_STRING_MAXSIZE + NUI_CHECKTEXTS_STRING_MAXSIZE];
	NVEC3 tsizea, tsizeb;
	NVEC3 text_size, icon_size, bkg_size, xtra_size;
	NVEC3 *ptext_size = NULL;
	NVEC3 *picon_size = NULL;
	NVEC3 *pbkg_size = NULL;
	NVEC3 *pxtra_size = NULL;

	NVEC3 anchor, parent_origin, relative_pos;
	NATLAS_ELEMENT *pel;

	NVec3Set(&text_size, 0, 0, 0);
	NVec3Set(&bkg_size, 0, 0, 0);
	NVec3Set(&icon_size, 0, 0, 0);
	NVec3Set(&xtra_size, 0, 0, 0);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	// +
	memset(pcbdesc, 0, sizeof(NUI_CHECKBOX_DESC));
	pcbdesc->UIDesc.pStyle = NUT_pCurrentUIDeskSet->pStyle;
	pcbdesc->UIDesc.Event_Proc = event_proc;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_UICREATIONSET
	// +
	// + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
	// +
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_PUSH))
	{
		FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_UI_TRANSFORMHNODE);				 // It's enough, for a push UI, to enable push animation ...
		FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS); // NUT PUSH controllers doing like this...
	}																					 // instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice

	//	This one will be done later ... just few lines down.
	// 	if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
	// 		FLAG_ON( pcbdesc->UIDesc.UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + ICON GEOMETRY
	// + CheckBox may have an ICON but its not an obligation.
	// +
	if (patlas_iconrefname)
	{
		pel = NLookupAtlasElement(patlas_iconrefname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON);
		pcbdesc->UIDesc.GeometryPreAllocatedCapacity += 1;

		FLAG_ON(pcbdesc->UIDesc.UIIconDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
		FLAG_ON(pcbdesc->UIDesc.UIIconDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		pcbdesc->UIDesc.UIIconDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
		NSetAppearance_Raw_Texture(&pcbdesc->UIDesc.UIIconDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_ICON_TEXTUREUNIT, pel->pTexture);
		NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pcbdesc->UIDesc.UIIconDesc, pel);

		NGetAtlasElementHRelSize((NVEC2 *)&icon_size, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
		icon_size.z = 0.0f;
		NSetUIGeometryDescSize(&pcbdesc->UIDesc.UIIconDesc, &icon_size);

		//	Icon 3D placement relative to Background.( This NUT_UI controller has always a background ! )
		NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
		NSetUIGeometryDescAnchor(&pcbdesc->UIDesc.UIIconDesc, &anchor);
		NSetUIGeometryDescParent(&pcbdesc->UIDesc.UIIconDesc, &pcbdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		NSetUIGeometryDescRelativePosition(&pcbdesc->UIDesc.UIIconDesc, &relative_pos);

		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_ICON, &pcbdesc->UIDesc.UIIconDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_ICON, &pcbdesc->UIDesc.UIIconDesc);

		// for Size calculation
		picon_size = &icon_size;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CAPTION/TEXT
	// + CheckBox may have a text displayer, its not an obligation. It will have one if at least 1 of the 3 possible texts to display
	// + truly exist. ( the 3 possibilities are: caption, check text, uncheck text )
	// + Notice there is no use of 'NuiFrameCheckBox' default check/uncheck texts here. "pcheck_text == NULL" means NO check_text, same thing
	// + for uncheck_text obviously.
	// +
	NVec3Set(&tsizea, 0, 0, 0);
	NVec3Set(&tsizeb, 0, 0, 0);
	if (pcaption || pcheck_text || puncheck_text)
	{
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON(pcbdesc->UIDesc.UITextDesc.Flags, FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE);

		// Text size (...used only at creation for placement).
		// We will get the highest and longest string size.x and string size.y the controller will print...
		// which will be: 'caption + CheckText' or 'Caption+UnCheckText'.

		// A right initialized empty string:
		str[0] = 0;

		// Get Size of 'caption + CheckText'
		if (pcaption)
			NStrCopy(str, pcaption, NUI_CAPTION_STRING_MAXSIZE + NUI_CHECKTEXTS_STRING_MAXSIZE);
		if (pcheck_text)
			NStrCat(str, pcheck_text, NUI_CAPTION_STRING_MAXSIZE + NUI_CHECKTEXTS_STRING_MAXSIZE);
		// 		else
		// 			NStrCat(str,DEFAULT_NUI_CHECKBOX_CHECK_TEXT,NUI_CAPTION_STRING_MAXSIZE+NUI_CHECKTEXTS_STRING_MAXSIZE);
		NGetStringSize(&tsizea, str, pcbdesc->UIDesc.pStyle->pFont);

		// Get Size of 'Caption + UnCheckText'
		if (pcaption)
			NStrCopy(str, pcaption, NUI_CAPTION_STRING_MAXSIZE + NUI_CHECKTEXTS_STRING_MAXSIZE);
		if (puncheck_text)
			NStrCat(str, puncheck_text, NUI_CAPTION_STRING_MAXSIZE + NUI_CHECKTEXTS_STRING_MAXSIZE);
		// 		else
		// 			NStrCat(str,DEFAULT_NUI_CHECKBOX_UNCHECK_TEXT,NUI_CAPTION_STRING_MAXSIZE+NUI_CHECKTEXTS_STRING_MAXSIZE);
		NGetStringSize(&tsizeb, str, pcbdesc->UIDesc.pStyle->pFont);

		// Keep the longest and the highest.
		text_size.x = NMAX(tsizea.x, tsizeb.x);
		text_size.y = NMAX(tsizea.y, tsizeb.y);
		text_size.z = NMAX(tsizea.z, tsizeb.z);
		NSetUITextDescSize(&pcbdesc->UIDesc.UITextDesc, &text_size);

		// for Size calculation
		ptext_size = &text_size;

		// Text placement from Icon if there is one, otherwise from background
		if (patlas_iconrefname)
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUITextDescParent(&pcbdesc->UIDesc.UITextDesc, &pcbdesc->UIDesc.UIIconDesc.Placement, &parent_origin);
		}
		else
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
			NSetUITextDescParent(&pcbdesc->UIDesc.UITextDesc, &pcbdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		}
		NSetUITextDescAnchor(&pcbdesc->UIDesc.UITextDesc, &anchor);
		NSetUITextDescRelativePosition(&pcbdesc->UIDesc.UITextDesc, &relative_pos);

		// Finalize setup for associated caption
		if (pcaption)
		{
			// For a CheckBox, having a caption means keep UI Name string ...
			// ... Because we need to rebuild a text from name and check status each time it changes.
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_COPY_NAME_STRING);
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME); // Useless if there are check_text and/or uncheck_text
			pcbdesc->UIDesc.pName = (Nchar *)pcaption;					   // But useful to build caption at uiframe setup if there are none of them
		}
		// ... and for associated check/uncheck texts
		if (pcheck_text)
		{
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_TEXT);
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_CHECK_TEXT);
			pcbdesc->pCheck_Text = (Nchar *)pcheck_text;
		}
		else // useless but just to be crystal clear
		{
			FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_TEXT);
			FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_CHECK_TEXT);
			pcbdesc->pCheck_Text = NULL;
		}

		if (puncheck_text)
		{
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_TEXT);
			FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_UNCHECK_TEXT);
			pcbdesc->pUnCheck_Text = (Nchar *)puncheck_text;
		}
		else // useless but just to be crystal clear
		{
			FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_TEXT);
			FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_CHECKBOX_UNCHECK_TEXT);
			pcbdesc->pUnCheck_Text = NULL;
		}
		NUT_UIDesk_Apply_TextAlignment(&pcbdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pcbdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pcbdesc->UIDesc.UITextDesc);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CHECK / UNCHECK GEOMETRY
	// + CheckBox may have a check and an uncheck geometry but its not an obligation.
	// + It may have just the check geometry or just the uncheck or both or none !
	// +
	NVec3Set(&tsizea, 0, 0, 0);
	NVec3Set(&tsizeb, 0, 0, 0);
	if (patlas_checkrefname)
	{
		pel = NLookupAtlasElement(patlas_checkrefname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_CHECK_MESH);
		pcbdesc->UIDesc.GeometryPreAllocatedCapacity += 1;

		FLAG_ON(pcbdesc->CheckDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
		FLAG_ON(pcbdesc->CheckDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		pcbdesc->CheckDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
		NSetAppearance_Raw_Texture(&pcbdesc->CheckDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_CHECK_UNCHECK_TEXTUREUNIT, pel->pTexture);
		NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pcbdesc->CheckDesc, pel);

		NGetAtlasElementHRelSize((NVEC2 *)&tsizea, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
		tsizea.z = 0.0f;
		NSetUIGeometryDescSize(&pcbdesc->CheckDesc, &tsizea);

		//	Check geometry placement relative to Text. If no text then Icon. If no icon then Background.( This NUT_UI controller has always a background ! )
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE))
		{
			// 1/ from Text
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUIGeometryDescParent(&pcbdesc->CheckDesc, &pcbdesc->UIDesc.UITextDesc.Placement, &parent_origin);
		}
		else if (patlas_iconrefname)
		{
			// 2/ from Icon
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUIGeometryDescParent(&pcbdesc->CheckDesc, &pcbdesc->UIDesc.UIIconDesc.Placement, &parent_origin);
		}
		else // 3/ from Background
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
			NSetUIGeometryDescParent(&pcbdesc->CheckDesc, &pcbdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		}
		NSetUIGeometryDescAnchor(&pcbdesc->CheckDesc, &anchor);
		NSetUIGeometryDescRelativePosition(&pcbdesc->CheckDesc, &relative_pos);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_MISC, &pcbdesc->CheckDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_MISC, &pcbdesc->CheckDesc);

		// for Size calculation
		pxtra_size = &xtra_size;
	}

	if (patlas_uncheckrefname)
	{
		pel = NLookupAtlasElement(patlas_uncheckrefname);
		NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_UNCHECK_MESH);
		pcbdesc->UIDesc.GeometryPreAllocatedCapacity += 1;

		FLAG_ON(pcbdesc->UnCheckDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
		FLAG_ON(pcbdesc->UnCheckDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		pcbdesc->UnCheckDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
		NSetAppearance_Raw_Texture(&pcbdesc->UnCheckDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_CHECK_UNCHECK_TEXTUREUNIT, pel->pTexture);
		NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pcbdesc->UnCheckDesc, pel);

		NGetAtlasElementHRelSize((NVEC2 *)&tsizeb, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
		tsizeb.z = 0.0f;
		NSetUIGeometryDescSize(&pcbdesc->UnCheckDesc, &tsizeb);

		//	UnCheck geometry placement relative to Text. If no text then Icon. If no icon then Background.( This NUT_UI controller has always a background ! )
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE))
		{
			// 1/ from Text
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUIGeometryDescParent(&pcbdesc->UnCheckDesc, &pcbdesc->UIDesc.UITextDesc.Placement, &parent_origin);
		}
		else if (patlas_iconrefname)
		{
			// 2/ from Icon
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
			NSetUIGeometryDescParent(&pcbdesc->UnCheckDesc, &pcbdesc->UIDesc.UIIconDesc.Placement, &parent_origin);
		}
		else // 3/ from Background
		{
			NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
			NSetUIGeometryDescParent(&pcbdesc->UnCheckDesc, &pcbdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
		}
		NSetUIGeometryDescAnchor(&pcbdesc->UnCheckDesc, &anchor);
		NSetUIGeometryDescRelativePosition(&pcbdesc->UnCheckDesc, &relative_pos);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_MISC, &pcbdesc->UnCheckDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_MISC, &pcbdesc->UnCheckDesc);

		// for Size calculation
		pxtra_size = &xtra_size;
	}

	// Xtra size update from check/uncheck meshes ( usefull to define Background size)
	// Keep the longest and the highest.
	xtra_size.x = NMAX(tsizea.x, tsizeb.x);
	xtra_size.y = NMAX(tsizea.y, tsizeb.y);
	xtra_size.z = NMAX(tsizea.z, tsizeb.z);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BACKGROUND GEOMETRY
	// + CheckBox may have a BACKGROUND its not an obligation.
	// + But in all cases we have to figure out its size en position.
	// + NUI setup is going to create a background only if 'FLAG_NUIDS_BACKGROUND' is set to on.
	// + So if we don't want to build it, just set this flag to OFF, but keep using "backgroundesc.placement" it's easier !
	// +
	NSetUIGeometryDescAnchor(&pcbdesc->UIDesc.UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pcbdesc->UIDesc.UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pcbdesc->UIDesc.UIBackgroundDesc, 0, 0, 0);
	if (patlas_backrefname || ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_DEFAULT_BKGD))
	{
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND);
		pcbdesc->UIDesc.GeometryPreAllocatedCapacity += 1;
		if (patlas_backrefname)
		{
			pel = NLookupAtlasElement(patlas_backrefname);
			NErrorIf(!pel, NERROR_UI_ATLAS_ELEMENT_MISSING);

			FLAG_ON(pcbdesc->UIDesc.UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT);
			FLAG_ON(pcbdesc->UIDesc.UIBackgroundDesc.Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
			pcbdesc->UIDesc.UIBackgroundDesc.GeometryFormat = NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE;
			NSetAppearance_Raw_Texture(&pcbdesc->UIDesc.UIBackgroundDesc.Appearance_Raw, CONSTANT_NUT_UIDESKSET_BACKGROUND_TEXTUREUNIT, pel->pTexture);

			NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(&pcbdesc->UIDesc.UIBackgroundDesc, pel);

			NGetAtlasElementHRelSize((NVEC2 *)&bkg_size, pel, NBOOL(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_VIEWPORT_ADAPTATIVE_SIZE)));
			bkg_size.z = 0.0f;

			// for Size calculation
			pbkg_size = &bkg_size;
		}
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pcbdesc->UIDesc.UIBackgroundDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD, &pcbdesc->UIDesc.UIBackgroundDesc);
	}
	// Size calculation
	NUT_UIDesk_ApplyUI_Size(&bkg_size, pbkg_size, picon_size, ptext_size, pxtra_size); // Re-using "bkg_size" to calculate and setup UIDesc.UIBackgroundDesc size. Even if there is no background ( it will be "VIRTUAL" in that case )
	NSetUIGeometryDescSize(&pcbdesc->UIDesc.UIBackgroundDesc, &bkg_size);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC and UIDS Flags.
	// + Setup the all the UIDC/UIDS Flags which are not under user control.
	// +
	// + --- + Picking Mesh
	if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND))
	{
		FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
	}
	else if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE))
	{
		if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON))
		{
			// In that special case ( Icon+Text without background) the controller  will have a custom picking geometry.
			// This one will be a simple quad with the exact background shape ( the one it would have had if it had been created ).
			// This quad it's like an invisible background... ( like but not the same because only vertex positions will
			// copied and be store by UIframe at creation ).
			FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH);
			pcbdesc->UIDesc.pCustomPickingMesh = NUT_CreateUIPickingQuad(&pcbdesc->UIDesc.UIBackgroundDesc.Placement);
		}
		else // Text alone
		{
			FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT);
		}
	}
	else if (ISFLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_ICON))
	{
		FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_ICON);
	}
	// 	else	|
	// 	{		|- Useless because its impossible and will generate an error right at the beginning of this function !
	// 	}		|

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
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	 // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	 // Set to ON as default behavior. User may update it after creation
		NUT_UIDesk_Apply_TouchMode(&pcbdesc->UIDesc);
	}
	else
	{
		FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);		  // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); // Set to OFF as default behavior. User may update it after creation
	}

	FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	 // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);	 // Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	//	... There are no conditional Flags to set !
	// + --- + Always ON
	FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_PRECISE_PICKING);
	FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);
	FLAG_ON(pcbdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);

	FLAG_ON(pcbdesc->UIDesc.Flags_Style, FLAG_NUIDS_CHECKBOX_STATUS_PRINTING);
}

// ------------------------------------------------------------------------------------------
// NUT_CreateUIFlatCheckBox
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Simple Flat Check Box,
//				With a flat background, and text ( including check/uncheck texts)
//				No icon. No check/uncheck geometries.
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
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Used. Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN )
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Used. Default BKG if no BKG Atlas Element definition ?
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
//
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent			Parent UI ( a menu, a layer, ... all NUI* are working )
//		Nf32			x,y,z			UI position
//		Nchar			*pcaption		displayed UI name (string will be stored as a copy into the uiframe)
//		Nchar			*pchecktext		displayed check text (if NULL, default will be used)
//		Nchar			*punchecktext	displayed check text (if NULL, default will be used)
//		NUI_EVENT_HANDLE	event_proc		A pointer on an event handle to allow use managing events received by the Layer.
//		Nu32			user32			User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI_CHECKBOX *NUT_CreateUIFlatCheckBox(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pcaption, Nchar *pcheck_text, Nchar *puncheck_text, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_CHECKBOX *pframe_checkbox;
	NUI_CHECKBOX_DESC cbdesc;
	Nchar *pcheck, *puncheck;

	// For the "_PrepareUICheckBoxDesc" function pcheck_text == NULL means NO check text.( same thing for UnCheck Text).
	// For this controller we don't want that.
	// No incoming "check text" or "uncheck text" should mean "use default".
	if (pcheck_text)
		pcheck = pcheck_text;
	else
		pcheck = DEFAULT_NUI_CHECKBOX_CHECK_TEXT;

	if (puncheck_text)
		puncheck = puncheck_text;
	else
		puncheck = DEFAULT_NUI_CHECKBOX_UNCHECK_TEXT;

	_PrepareUICheckBoxDesc(&cbdesc, NULL, NULL, pcaption, pcheck, NULL, puncheck, NULL, event_proc);
	pframe_checkbox = NCreateUICheckBox(parent, &cbdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_checkbox, &cbdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// NSetUIPositionf((NUI*)pframe_checkbox, x, y, z);
	return pframe_checkbox;
}
// ------------------------------------------------------------------------------------------
// NUT_CreateUIGraphicCheckBox
// ------------------------------------------------------------------------------------------
//	Description :
//
//				A Check Box,
//				With a background, an icon and check/uncheck geometries.
//				without any text.
//
//	List of NUT_UiCreationSet Properties Flags used by this controller creation process.
//	-----------------------------------------------------------------------------------------
//		FLAG_NUT_UIDESKSET_USE_BKGD_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_BKGD_ALTERNATIVECOLOR		Used.		|
//		FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL			Used.		|
//		FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR		Used.		|_ To use BLEND/MATERIAL and ALTERNATIVE COLOR, element by element.
//		FLAG_NUT_UIDESKSET_USE_TEXT_BLEND_MATERIAL			Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_TEXT_ALTERNATIVECOLOR		Not Used.	|
//		FLAG_NUT_UIDESKSET_USE_MISC_BLEND_MATERIAL			Used.		|.... for Check/UnCheck Geometry.
//		FLAG_NUT_UIDESKSET_USE_MISC_ALTERNATIVECOLOR		Used.		|.... for Check/UnCheck Geometry.
//		FLAG_NUT_UIDESKSET_USE_TOUCH_MODE					Used. Use Specified Touch Mode ? Or let UI Using it's own default ...
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. ( No text !) [ Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ) ]
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
//		NTEXT_ALIGNMENT_ENUM	Alignment;					Not Used. ( No text !)
//		NBLEND					*pBlend[];					Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NMATERIAL				*pMaterial[];				Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_BLEND_MATERIAL is SET to ON.
//		NCOLOR					AlternativeColorRange[][];	Used. If associated FLAG_NUT_UIDESKSET_USE_xxx_ALTERNATIVECOLOR is SET to ON.
//
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent			Parent UI ( a menu, a layer, ... all NUI* are working )
//		Nf32			x,y,z			UI position
//		Nchar			*pcaption		displayed UI name (string will be stored as a copy into the uiframe)
//		Nchar			*pchecktext		displayed check text (if NULL, default will be used)
//		Nchar			*punchecktext	displayed check text (if NULL, default will be used)
//		NUI_EVENT_HANDLE	event_proc		A pointer on an event handle to allow use managing events received by the Layer.
//		Nu32			user32			User define value
// Out :
//		NUI *		Ptr on the created UI.
// ------------------------------------------------------------------------------------------
NUI_CHECKBOX *NUT_CreateUIGraphicCheckBox(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const Nchar *patlas_checkrefname, const Nchar *patlas_uncheckrefname, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NUI_CHECKBOX *pframe_checkbox;
	NUI_CHECKBOX_DESC cbdesc;

	_PrepareUICheckBoxDesc(&cbdesc, patlas_backrefname, patlas_iconrefname, NULL, NULL, patlas_checkrefname, NULL, patlas_uncheckrefname, event_proc);
	pframe_checkbox = NCreateUICheckBox(parent, &cbdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_checkbox, &cbdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// NSetUIPositionf((NUI*)pframe_checkbox, x, y, z);
	return pframe_checkbox;
}
