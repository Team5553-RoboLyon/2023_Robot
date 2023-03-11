#include "lib/N/NCStandard.h"
#include "lib/N/NUsualCoordinates.h"
#include "lib/N/NString.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/UI/NUICheckBox.h"
#include "lib/N/UI/NUIColorCircle.h"
#include "NUT_UIPresets.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Panel.h"

#include "NUT_Shape.h"

// + -------------------------------------------------------------------------------------------------------------------------
//	SCHEMES:
//
//		  With caption											 Without caption
//
//		 ---------------	+			+
//		|               |   |			|
//		|               |   +			|
//		|  CAPTION      |   			|
//		|               |   +			|						 ---------------	+			+
//		|               |   |			|						|               |   |			|
//      |     + + +     |   +			|					    |     + + +     |   +			|
//		|   +	    +   |				|			OR			|   +	    +   |				|
//		|  +         +  |				|						|  +         +  |				|
//		|  +    +    +  |   MarginY		|sizeY					|  +    +    +  |   MarginY		|sizeY
//		|  +         +  |				|						|  +         +  |				|
//		|   +       +   |				|						|   +       +   |				|
//      |     + + +     |   +			|					    |     + + +     |   +			|
//		|               |   |			|						|               |   |			|
//		 ---------------	+			+						 ---------------	+			+

//		<->          <->		MarginX							<->          <->		MarginX
//		<-------------->		sizeX							<-------------->		sizeX

static inline void _PrepareUIColorCircleBasicsDesc(NUI_COLORCIRCLE_DESC *pcdesc, const Nf32 circle_size, const NCOLOR *pcolor, Nchar *pcaption, const NUI_EVENT_HANDLE event_proc)
{
	NErrorIf((!circle_size), NERROR_INVALID_PARAMETER);

	NVEC3 anchor, parent_origin, relative_pos;
	NVEC3 text_size, bkg_size, xtra_size;

	NVec3Set(&text_size, 0, 0, 0);
	NVec3Set(&xtra_size, 0, 0, 0);
	NVec3Set(&bkg_size, 0, 0, 0);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EASY PART FIRST
	pcdesc->UIDesc.pStyle = NUT_pCurrentUIDeskSet->pStyle;
	pcdesc->UIDesc.Event_Proc = event_proc;
	pcdesc->UIDesc.GeometryPreAllocatedCapacity = 1; // its a minimum : ColorDisc and Arcs and Selectors are into the sema geometry.
													 // if there is a background it will be 2 !
													 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
													 // +
													 // + NUT_UICREATIONSET
													 // +
													 // + Easy part first, just some PROPERTIES FLAGS to test and transpose into Core(UIDC) ,Style(UIDS) or uigeometryDesc(UIGD) Flags.
													 // +
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_COLOR_UPDATE))
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLOR_UPDATE);

	/*
		...Useless, color circle doesn't manage PUSH tech.
		if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_PUSH))
		{
			FLAG_ON(pcdesc->UIDesc.Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE); // It's enough, for a push UI, to enable push animation ...
			FLAG_ON(pcdesc->UIDesc.Flags_Core,FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS ); // NUT PUSH controllers doing like this...
		}																					// instead of 	FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH, but its just a choice
	*/

	//	This one will be done later ... just few lines down.
	// 	if(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags,FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
	// 		FLAG_ON( pbuttondesc->UIDesc.UITextDesc.Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE );

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Some ColorCircle Specific Setup controlled by User
	// +
	if (pcolor)
	{
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_COLORCIRCLE_INITIALCOLOR);
		pcdesc->InitialColor = *pcolor;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CAPTION/TEXT
	// + ColorCircle may have a caption(text) but its not an obligation.
	// +
	if (pcaption || ISFLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING))
	{
		// We need an UI UI Text Display Device to print these infos ...
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_TEXT_DISPLAY_DEVICE);
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT))
			FLAG_ON(pcdesc->UIDesc.UITextDesc.Flags, FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE);

		if (pcaption)
		{
			FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_DISPLAY_NAME);
			pcdesc->UIDesc.pName = pcaption;
			if (ISFLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING))
				FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_COPY_NAME_STRING);
		}

		// Text size Estimation.
		Nchar str[CONSTANT_NUI_COLORCIRCLE_INFOS_STRING_SIZEMAX + CONSTANT_NUI_NAME_SIZEMAX];
		str[0] = 0;
		if (ISFLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING))
		{
			// Printing Color is always in Nu8 Format whatever the COMMAND format is (COMMAND FORMAT == Color Format send trough ...
			// NCOMMAND_COLORCIRCLE_RGBA8 or NCOMMAND_COLORCIRCLE_RGBA32 messages. )
			if (ISFLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_RGBA))
				NUIColorCircle_SetupDisplayableInfos_RGBAString(str, pcaption, 255, 255, 255, 255);
			else
				NUIColorCircle_SetupDisplayableInfos_RGBString(str, pcaption, 255, 255, 255);
		}
		else
		{
			NStrCopy(str, pcaption, CONSTANT_NUI_COLORCIRCLE_INFOS_STRING_SIZEMAX + CONSTANT_NUI_NAME_SIZEMAX);
		}

		NGetStringSize(&text_size, str, pcdesc->UIDesc.pStyle->pFont);
		NSetUITextDescSize(&pcdesc->UIDesc.UITextDesc, &text_size);

		NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NFALSE);
		NSetUITextDescAnchor(&pcdesc->UIDesc.UITextDesc, &anchor); // LEFT,MIDHEIGHT
		NSetUITextDescParent(&pcdesc->UIDesc.UITextDesc, &pcdesc->ColorCircleDesc.Placement, &parent_origin);
		NSetUITextDescRelativePosition(&pcdesc->UIDesc.UITextDesc, &relative_pos);

		NUT_UIDesk_Apply_TextAlignment(&pcdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pcdesc->UIDesc.UITextDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_TEXT, (NUIGEOMETRY_DESC *)&pcdesc->UIDesc.UITextDesc);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + COLORCIRCLE
	// + the specific Geometry with all the elements inside (circle + arcs + cursor )
	// +
	if (ISFLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_RGBA))
		NSetUIGeometryDescCustomGeometry(&pcdesc->ColorCircleDesc, NUICreateColorCircleCustomGeometry(circle_size, NTRUE));
	else
		NSetUIGeometryDescCustomGeometry(&pcdesc->ColorCircleDesc, NUICreateColorCircleCustomGeometry(circle_size, NFALSE));
	NGetUIGeometryDescCustomGeometrySize(&xtra_size, &pcdesc->ColorCircleDesc);
	NSetUIGeometryDescSize(&pcdesc->ColorCircleDesc, &xtra_size);

	NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(&relative_pos, &parent_origin, &anchor, NTRUE);
	NSetUIGeometryDescAnchor(&pcdesc->ColorCircleDesc, &anchor);
	NSetUIGeometryDescParent(&pcdesc->ColorCircleDesc, &pcdesc->UIDesc.UIBackgroundDesc.Placement, &parent_origin);
	NSetUIGeometryDescRelativePosition(&pcdesc->ColorCircleDesc, &relative_pos);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BACKGROUND GEOMETRY
	// + This Controller may have a BACKGROUND its not an obligation.
	// + But in all cases we have to figure out its size en position.
	// + NUI setup is going to create a background only if 'FLAG_NUIDS_BACKGROUND' is set to on.
	// + So if we don't want to build it, just set this flag to OFF, but keep using "backgroundesc.placement" it's easier !
	NSetUIGeometryDescAnchor(&pcdesc->UIDesc.UIBackgroundDesc, &NUT_pCurrentUIDeskSet->BuildAnchor);
	NSetUIGeometryDescParentf(&pcdesc->UIDesc.UIBackgroundDesc, NULL, 0, 0, 0);
	NSetUIGeometryDescRelativePositionf(&pcdesc->UIDesc.UIBackgroundDesc, 0, 0, 0);
	NUT_UIDesk_ApplyUI_Size(&bkg_size, &bkg_size, NULL, &text_size, NULL);
	NSetUIGeometryDescSize(&pcdesc->UIDesc.UIBackgroundDesc, &bkg_size);
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_DEFAULT_BKGD))
	{
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND);
		pcdesc->UIDesc.GeometryPreAllocatedCapacity += 1;
		NUT_UIDesk_Apply_AlternativeColorRange(NUT_UIDESK_BKGD, &pcdesc->UIDesc.UIBackgroundDesc);
		NUT_UIDesk_Apply_Blend_And_Material(NUT_UIDESK_BKGD, &pcdesc->UIDesc.UIBackgroundDesc);
	}

	// Background Size calculation
	NUT_UIDesk_ApplyUI_Size(&bkg_size, &bkg_size, NULL, &text_size, &xtra_size);
	NSetUIGeometryDescSize(&pcdesc->UIDesc.UIBackgroundDesc, &bkg_size);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + UIDC Flags.
	// + Setup the all the UIDC/UIDS Flags which are not under user control.
	// +
	if (ISFLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_BACKGROUND))
	{
		FLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND);
	}
	else
	{
		// In that special case ( Color Circle without background) the controller NUT_UIColorCircle will have a custom picking geometry
		// behind the color circle and its arc(s). This one will be a simple quad with the exact background shape
		// ( the one it would have had if it had been created ).
		// This quad it's like an invisible background... ( like but not the same because only vertex positions will be computed and
		// stored by UIframe creation process).
		FLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH);
		pcdesc->UIDesc.pCustomPickingMesh = NUT_CreateUIPickingQuad(&pcdesc->UIDesc.UIBackgroundDesc.Placement);
	}

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
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT); // Set to ON as default behavior. User may update it after creation
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);	// Set to ON as default behavior. User may update it after creation
		FLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);	// Set to ON as default behavior. User may update it after creation
		NUT_UIDesk_Apply_TouchMode(&pcdesc->UIDesc);
	}
	else
	{
		FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT);	 // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT);		 // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT);		 // Set to OFF as default behavior. User may update it after creation
		FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE); // Set to OFF as default behavior. User may update it after creation
	}

	FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT);	// Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT);	// Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT); // Set to OFF as default behavior. User may update it after creation
	FLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT);		// Set to OFF as default behavior. User may update it after creation

	// + --- + Conditional
	//	... There are no conditional Flags to set !
	// + --- + Always ON
	FLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED);
	FLAG_ON(pcdesc->UIDesc.Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP);
}

// ------------------------------------------------------------------------------------------
// NUI_COLORCIRCLE*	NUT_CreateUIColorCircleRGB
// ------------------------------------------------------------------------------------------
// Description :
//	Create a ColorCircle Control managing RGB color with NCOLOR output ( trough NCOMMAND event )
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
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. ( No text !) [ Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ) ]
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Used. There is no BKG texture. [Default BKG if no BKG Atlas Element definition ?]
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
//// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// --------------------------------------------------------------------------------------------
NUI_COLORCIRCLE *NUT_CreateUIColorCircleRGB(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nf32 circle_size, const NCOLOR *pcolor, Nchar *pcaption, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NUI_COLORCIRCLE *pframe_circle;
	NUI_COLORCIRCLE_DESC cdesc;

	// Setup UI UI description ...
	memset(&cdesc, 0, sizeof(NUI_COLORCIRCLE_DESC));
	_PrepareUIColorCircleBasicsDesc(&cdesc, circle_size, pcolor, pcaption, event_proc);
	pframe_circle = NCreateUIColorCircle(parent, &cdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_circle, &cdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// !!! Some building stuff do delete ???
	// A custom picking mesh was created and has to be destroyed !
	// Useful Mesh contents (vertex positions and triangle indexes actually ) has been copied by NUI creation process...
	// ... and doesn't need "buttondesc.UIDesc.pCustomPickingMesh" any more. So let's delete it right now !
	if (ISFLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH))
		NDeleteMesh(cdesc.UIDesc.pCustomPickingMesh);
	return pframe_circle;
}
// ------------------------------------------------------------------------------------------
// NUI_COLORCIRCLE*	NUT_CreateUIColorCircleRGBA
// ------------------------------------------------------------------------------------------
// Description :
//	Create a ColorCircle Control managing RGBA color with NCOLOR output ( trough NCOMMAND event )
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
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. ( No text !) [ Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ) ]
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Used. There is no BKG texture. [Default BKG if no BKG Atlas Element definition ?]
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
//// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// --------------------------------------------------------------------------------------------
NUI_COLORCIRCLE *NUT_CreateUIColorCircleRGBA(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nf32 circle_size, const NCOLOR *pcolor, Nchar *pcaption, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NUI_COLORCIRCLE *pframe_circle;
	NUI_COLORCIRCLE_DESC cdesc;

	// Setup UI UI description ...
	memset(&cdesc, 0, sizeof(NUI_COLORCIRCLE_DESC));
	FLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_RGBA);
	_PrepareUIColorCircleBasicsDesc(&cdesc, circle_size, pcolor, pcaption, event_proc);
	pframe_circle = NCreateUIColorCircle(parent, &cdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_circle, &cdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// !!! Some building stuff do delete ???
	// A custom picking mesh was created and has to be destroyed !
	// Useful Mesh contents (vertex positions and triangle indexes actually ) has been copied by NUI creation process...
	// ... and doesn't need "buttondesc.UIDesc.pCustomPickingMesh" any more. So let's delete it right now !
	if (ISFLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH))
		NDeleteMesh(cdesc.UIDesc.pCustomPickingMesh);
	return pframe_circle;
}
// ------------------------------------------------------------------------------------------
// NUI_COLORCIRCLE*	NUT_CreateUIColorCircle
// ------------------------------------------------------------------------------------------
// Description :
//	Create a ColorCircle Control managing RGB / RGBA colors  with NCOLOR or RGBA8 output ( trough NCOMMAND event )
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
//		FLAG_NUT_UIDESKSET_INDEPENDENT_TEXT					Not Used. ( No text !) [ Define the Text object linked THN. ( ON: it will be the UI Renderable THN. OFF: It will be the UI THN ) ]
//		FLAG_NUT_UIDESKSET_COLOR_UPDATE						Used. Color Updating ( according with UI main states - ENABLE/DISABLE/FOCUSED )
//		FLAG_NUT_UIDESKSET_PUSH								Not Used. Push ?
//		FLAG_NUT_UIDESKSET_DEFAULT_BKGD						Used. There is no BKG texture. [Default BKG if no BKG Atlas Element definition ?]
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
//// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// --------------------------------------------------------------------------------------------
NUI_COLORCIRCLE *NUT_CreateUIColorCircle(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nf32 circle_size, const NCOLOR *pcolor, Nchar *pcaption, Nbool use_rgba, Nbool rgba8_output, Nbool inprogress_command, Nbool print_info, const NUI_EVENT_HANDLE event_proc, const Nu32 user32)
{
	NErrorIf(!NUT_pCurrentUIDeskSet->pStyle, NERROR_UI_STYLE_MISSING);

	NUI_COLORCIRCLE *pframe_circle;
	NUI_COLORCIRCLE_DESC cdesc;

	// Setup UI UI description ...
	memset(&cdesc, 0, sizeof(NUI_COLORCIRCLE_DESC));

	// Notes: ActivateUICreationSet Properties DOESN'T HAVE ANY EFFECT FOR THIS CONTROLLER

	if (use_rgba)
	{
		FLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_RGBA);
	}

	if (rgba8_output)
	{
		FLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_NU8_COMMAND_FORMAT);
	}

	if (inprogress_command)
	{
		FLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_COLORCIRCLE_INPROGRESS_COMMAND);
	}

	if (print_info)
	{
		FLAG_ON(cdesc.UIDesc.Flags_Style, FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING);
	}
	_PrepareUIColorCircleBasicsDesc(&cdesc, circle_size, pcolor, pcaption, event_proc);
	pframe_circle = NCreateUIColorCircle(parent, &cdesc, user32);
	NUT_UIDesk_SetInPanelUIPositionf((NUI *)pframe_circle, &cdesc.UIDesc.UIBackgroundDesc.Placement.Size, x, y, z);
	// !!! Some building stuff do delete ???
	// A custom picking mesh was created and has to be destroyed !
	// Useful Mesh contents (vertex positions and triangle indexes actually ) has been copied by NUI creation process...
	// ... and doesn't need "buttondesc.UIDesc.pCustomPickingMesh" any more. So let's delete it right now !
	if (ISFLAG_ON(cdesc.UIDesc.Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH))
		NDeleteMesh(cdesc.UIDesc.pCustomPickingMesh);
	return pframe_circle;
}