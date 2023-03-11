#include "lib/N/NCStandard.h"
#include "lib/N/UI/NUI.h"
#include "NUT_UIDesk_Dummy.h"
#include "NUT_UIDesk_Set.h"

NUT_UIDESK_SET NUT_UIDeskSet[NUT_UIDESKSET_STACKSIZE];
NUT_UIDESK_SET *NUT_pCurrentUIDeskSet;

void NUT_ClearUIDeskSet()
{
	NErrorIf(!NUT_pCurrentUIDeskSet, NERROR_NULL_POINTER);
	Nmem0(NUT_pCurrentUIDeskSet, NUT_UIDESK_SET);
}

void NUT_ClearUIDeskSetStack()
{
	for (Nu32 i = 0; i < NUT_UIDESKSET_STACKSIZE; i++)
	{
		NUT_pCurrentUIDeskSet = &NUT_UIDeskSet[i];
		NUT_ClearUIDeskSet();
	}
	NUT_pCurrentUIDeskSet = &NUT_UIDeskSet[0];
}

void NUT_InitializeUIDeskSet()
{
	NUT_ClearUIDeskSetStack();
}
void NUT_UIDesk_Apply_TextAlignment(NUITEXT_DESC *ptdesc)
{
	FLAG_ON(ptdesc->Flags, FLAG_NUIGD_USEPARAM_TEXT_ALIGNMENT);
	ptdesc->Alignment = NUT_pCurrentUIDeskSet->Alignment;
}

void NUT_UIDesk_Apply_Blend_And_Material(const NUT_UIDESKCOMPONENT_ENUM ui_element_id, NUIGEOMETRY_DESC *pgdesc)
{
	//	Nu32	flags=0;

	// The Properties FLAG sequence Order is:
	//						0-BLEND_MATERIAL
	//						1-ALTERNATIVECOLOR
	//
	//	... and for components :
	//						0-NUT_UIDESK_BKGD
	//						1-NUT_UIDESK_ICON
	//						2-NUT_UIDESK_TEXT
	//						3-NUT_UIDESK_MISC
	// For example, to check if 'FLAG_NUT_UIDESKSET_USE_ICON_BLEND_MATERIAL' is ON we are going first to "rebuild"
	// the flag by calculating the bit id: "NUT_UIDESK_ICON*2" which is "1*2 = 2" that means BIT_2 ( done !)
	// Blend
	if (BITGET(NUT_pCurrentUIDeskSet->PropertiesFlags, ui_element_id * 2))
	{
		// Check for available Appearance
		NGEOMETRY_FORMAT_ENUM rsid;
		if (ISFLAG_ON(pgdesc->Flags, FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT))
		{
			rsid = pgdesc->GeometryFormat;
		}
		else // default
		{
			if (ui_element_id == NUT_UIDESK_TEXT)
				rsid = DEFAULT_NTEXT_GEOMETRIES_SPECS_ID;
			else
				rsid = DEFAULT_NUI_GEOMETRY_FORMAT;
		}
		if (NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(NGetGeometrySpecs(rsid)->IDS) == NAPPEARANCE_FORMAT_NULL)
		{
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
			return;
		}

		// Apply !
		/*
				FLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_APPEARANCE);
				pgdesc->Appearance_Raw.pBlend = NUT_pCurrentUIDeskSet->pBlend[ui_element_id];
				pgdesc->Appearance_Raw.pMaterial	= NUT_pCurrentUIDeskSet->pMaterial[ui_element_id];
		*/
		NSetUIGeometryDescBlendAndMaterial(pgdesc, NUT_pCurrentUIDeskSet->pBlend[ui_element_id],
										   NUT_pCurrentUIDeskSet->pMaterial[ui_element_id]);
	}
}

void NUT_UIDesk_Apply_AlternativeColorRange(const NUT_UIDESKCOMPONENT_ENUM ui_element_id, NUIGEOMETRY_DESC *pgdesc)
{
	// The Properties FLAG sequence Order is:
	//						0-BLEND_MATERIAL
	//						1-ALTERNATIVECOLOR
	//
	//	... and for components :
	//						0-NUT_UIDESK_BKGD
	//						1-NUT_UIDESK_ICON
	//						2-NUT_UIDESK_TEXT
	//						3-NUT_UIDESK_MISC
	// For example, to check if 'FLAG_NUT_UIDESKSET_USE_ICON_ALTERNATIVECOLOR' is ON we are going first to "rebuild"
	// the flag by calculating the bit id: "NUT_UIDESK_ICON*2 + 1" which is "1*2+1 = 3" BIT_3 ( done !)
	if (BITGET(NUT_pCurrentUIDeskSet->PropertiesFlags, ui_element_id * 2 + 1))
	{
		/*
				pgdesc->pColorRange		= NUT_pCurrentUIDeskSet->AlternativeColorRange[ui_element_id];
				pgdesc->ColorRangeSize	= CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE;
				FLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAMS_COLORRANGE);
		*/
		NSetUIGeometryDescColorRange(pgdesc, NUT_pCurrentUIDeskSet->AlternativeColorRange[ui_element_id], CONSTANT_NUT_UIDESKSET_COLOR_RANGE_SIZE);
	}
}

void NUT_UIDesk_ApplyRelativeBuildPosition_And_BuildAnchors(NVEC3 *prelativ_pos, NVEC3 *pparentanchor, NVEC3 *panchor, const Nbool parent_is_bkg)
{

	// Notes:
	// Distribution Order follows western read logic from left to right, from top to bottom, from back to front.
	// So reverse Order is the opposite.
	//
	// X Axis distribution
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_X_DISTRIBUTION))
	{
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION))
		{
			panchor->x = ANCHOR_RIGHT;
			if (parent_is_bkg)
			{
				pparentanchor->x = ANCHOR_RIGHT;
				prelativ_pos->x = -NUT_pCurrentUIDeskSet->OuterMargin.x;
			}
			else
			{
				pparentanchor->x = ANCHOR_LEFT;
				prelativ_pos->x = -NUT_pCurrentUIDeskSet->InnerSpacing.x;
			}
		}
		else
		{
			panchor->x = ANCHOR_LEFT;

			if (parent_is_bkg)
			{
				pparentanchor->x = ANCHOR_LEFT;
				prelativ_pos->x = NUT_pCurrentUIDeskSet->OuterMargin.x;
			}
			else
			{
				pparentanchor->x = ANCHOR_RIGHT;
				prelativ_pos->x = NUT_pCurrentUIDeskSet->InnerSpacing.x;
			}
		}
	}
	else
	{
		pparentanchor->x = ANCHOR_MIDWIDTH;
		panchor->x = ANCHOR_MIDWIDTH;
		prelativ_pos->x = 0.0f;
	}
	// Y Axis distribution
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_Y_DISTRIBUTION))
	{
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION))
		{
			panchor->y = ANCHOR_TOP;

			if (parent_is_bkg)
			{
				pparentanchor->y = ANCHOR_TOP;
				prelativ_pos->y = -NUT_pCurrentUIDeskSet->OuterMargin.y;
			}
			else
			{
				pparentanchor->y = ANCHOR_BOTTOM;
				prelativ_pos->y = -NUT_pCurrentUIDeskSet->InnerSpacing.y;
			}
		}
		else
		{
			panchor->y = ANCHOR_BOTTOM;

			if (parent_is_bkg)
			{
				pparentanchor->y = ANCHOR_BOTTOM;
				prelativ_pos->y = NUT_pCurrentUIDeskSet->OuterMargin.y;
			}
			else
			{
				pparentanchor->y = ANCHOR_TOP;
				prelativ_pos->y = NUT_pCurrentUIDeskSet->InnerSpacing.y;
			}
		}
	}
	else
	{
		pparentanchor->y = ANCHOR_MIDHEIGHT;
		panchor->y = ANCHOR_MIDHEIGHT;
		prelativ_pos->y = 0.0f;
	}
	// Z Axis distibution
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_X_DISTRIBUTION))
	{
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_REVERSE_DISTRIBUTION))
		{
			panchor->z = ANCHOR_FRONT;
			if (parent_is_bkg)
			{
				pparentanchor->z = ANCHOR_FRONT;
				prelativ_pos->z = -NUT_pCurrentUIDeskSet->OuterMargin.z;
			}
			else
			{
				pparentanchor->z = ANCHOR_BACK;
				prelativ_pos->z = -NUT_pCurrentUIDeskSet->InnerSpacing.z;
			}
		}
		else
		{
			panchor->z = ANCHOR_BACK;
			if (parent_is_bkg)
			{
				pparentanchor->z = ANCHOR_BACK;
				prelativ_pos->z = NUT_pCurrentUIDeskSet->OuterMargin.z;
			}
			else
			{
				pparentanchor->z = ANCHOR_FRONT;
				prelativ_pos->z = NUT_pCurrentUIDeskSet->InnerSpacing.z;
			}
		}
	}
	else
	{
		pparentanchor->z = ANCHOR_MIDDEPTH;
		panchor->z = ANCHOR_MIDDEPTH;
		prelativ_pos->z = 0.0f;
	}
}

void NUT_UIDesk_ApplyUI_Size(NVEC3 *pdst_uisize, const NVEC3 *psrc_bkgtexture_size, const NVEC3 *psrc_icon_size, const NVEC3 *psrc_text_size, const NVEC3 *psrc_xtra_size)
{
	Ns32 nb = -1;
	NVEC3 null_size;
	NVEC3 *ptext_size, *picon_size, *pxtra_size, *pbkg_size;
	NVEC3 bsize;
	NVEC3 innerspace;

	// HOW TO DO ?
	// First of all, check some special cases to avoid useless calculations and return back as quickly as possible.
	//		case 0: there is a texture and NUT_UIDeskSet Properties asks for keeping texture size... No choice !
	//		case 1: case 0 didn't appear and NUT_UIDeskSet Properties asks for forcing size!
	// Then, After these tests if we still here, calculate !

	// Consistency Checks
#ifdef _DEBUG
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE) &&
		!FLAGS_TEST(NUT_pCurrentUIDeskSet->PropertiesFlags, MASK_NUT_UIDESKSET_SIZECONSTRAINTS, 0))
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS) &&
		ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE))
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE) &&
		ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS))
		NErrorIf(1, NERROR_INCONSISTENT_FLAGS);

	if (!FLAGS_TEST(NUT_pCurrentUIDeskSet->PropertiesFlags, MASK_NUT_UIDESKSET_SIZECONSTRAINTS, 0))
	{
		if (!NUT_pCurrentUIDeskSet->SizeConstraints.x && !NUT_pCurrentUIDeskSet->SizeConstraints.y && !NUT_pCurrentUIDeskSet->SizeConstraints.z)
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		if (NUT_pCurrentUIDeskSet->SizeConstraints.x < 0 || NUT_pCurrentUIDeskSet->SizeConstraints.y < 0 || NUT_pCurrentUIDeskSet->SizeConstraints.z < 0)
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
	}

#endif

	// Some earlier setup
	NVec3Set(&null_size, 0, 0, 0);
	if (psrc_icon_size)
	{
		picon_size = (NVEC3 *)psrc_icon_size;
		// In case of "no sizes" element, we are not going to count this one
		// into the inner space counter. We are going to consider it as a NULL object.
		if (picon_size->x + picon_size->y + picon_size->z != 0.0f)
			nb++;
	}
	else
	{
		picon_size = &null_size;
	}
	if (psrc_text_size)
	{
		ptext_size = (NVEC3 *)psrc_text_size;
		// In case of "no sizes" element, we are not going to count this one
		// into the inner space counter. We are going to consider it as a NULL object.
		if (ptext_size->x + ptext_size->y + ptext_size->z != 0.0f)
			nb++;
	}
	else
	{
		ptext_size = &null_size;
	}

	if (psrc_xtra_size)
	{
		pxtra_size = (NVEC3 *)psrc_xtra_size;

		// In case of "no sizes" element, we are not going to count this one
		// into the inner space counter. We are going to consider it as a NULL object.
		if (pxtra_size->x + pxtra_size->y + pxtra_size->z != 0.0f)
			nb++;
	}
	else
	{
		pxtra_size = &null_size;
	}
	// Calculate (cumulate) total inner spaces
	// Note that we need at least 2
	if (nb > 0)
	{
		NVec3Scale(&innerspace, &NUT_pCurrentUIDeskSet->InnerSpacing, (Nf32)nb);
	}
	else
	{
		NVec3Set(&innerspace, 0, 0, 0);
	}
	// ... AND GO !
	// ------------------------------------------------------------------------------------------------------------------------
	// Background Size is not managed as the other sizes... because background size could be updated by the others ...
	// ... Or it could stays as it is depending the FLAGS setup by user (like FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE )
	// ... BUT HAVING A valid "psrc_bkgtexture_size" pointer is not enough to validate it, i mean, a real size is needed not a NULL pointer !
	// ... So, let's check that first, and work with the full validated "pkg_size" pointer instead of "psrc_bkgtexture_size"
	pbkg_size = NULL;
	if (psrc_bkgtexture_size)
	{
		if (psrc_bkgtexture_size->x + psrc_bkgtexture_size->y + psrc_bkgtexture_size->z != 0.0f)
			pbkg_size = (NVEC3 *)psrc_bkgtexture_size;
	}

	if (pbkg_size && ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE))
	{
		*pdst_uisize = *pbkg_size;
		return;
	}

	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE))
	{
		*pdst_uisize = NUT_pCurrentUIDeskSet->SizeConstraints;
		return;
	}

	// 1)Distribute elements on each of the 3 Axis and take in account margin and spacing to calculate
	//	 Each background size dimension by addition. Without distribution (distribution FLAG set to OFF),
	//	 background size dimension is equal to the biggest element size and margin.
	//	 (Spacing between element is not used in that case).
	//
	// X ----------------------------------------------------------------------------------------------------------------------------
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_X_DISTRIBUTION))
	{
		bsize.x = picon_size->x + ptext_size->x + pxtra_size->x + innerspace.x + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.x;
	}
	else
	{
		bsize.x = NMAX(pxtra_size->x, NMAX(ptext_size->x, picon_size->x)) + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.x;
	}
	// Y ----------------------------------------------------------------------------------------------------------------------------
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_Y_DISTRIBUTION))
	{
		bsize.y = picon_size->y + ptext_size->y + pxtra_size->y + innerspace.y + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.y;
	}
	else
	{
		bsize.y = NMAX(pxtra_size->y, NMAX(ptext_size->y, picon_size->y)) + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.y;
	}
	// Z ----------------------------------------------------------------------------------------------------------------------------
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_Z_DISTRIBUTION))
	{
		bsize.z = picon_size->z + ptext_size->z + pxtra_size->z + innerspace.z + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.z;
	}
	else
	{
		bsize.z = NMAX(pxtra_size->z, NMAX(ptext_size->z, picon_size->z)) + 2.0f * NUT_pCurrentUIDeskSet->OuterMargin.z;
	}

	// 2) Check background size calculation results against UICreationSet Size Constraints.
	//	  Case 	FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZE is already done (take a look a few lines above )
	//	  So, just two cases to deal with:
	//			FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN
	//			FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN))
	{
		bsize.x = NMAX(bsize.x, NUT_pCurrentUIDeskSet->SizeConstraints.x);
		bsize.y = NMAX(bsize.y, NUT_pCurrentUIDeskSet->SizeConstraints.y);
		bsize.z = NMAX(bsize.z, NUT_pCurrentUIDeskSet->SizeConstraints.z);
	}
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX))
	{
		bsize.x = NMIN(bsize.x, NUT_pCurrentUIDeskSet->SizeConstraints.x);
		bsize.y = NMIN(bsize.y, NUT_pCurrentUIDeskSet->SizeConstraints.y);
		bsize.z = NMIN(bsize.z, NUT_pCurrentUIDeskSet->SizeConstraints.z);
	}

	// 3) Check background size with constraints against UICreationSet Texture constraints.
	//	  Case FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_SIZE is already done (take a look a few lines above )
	//	  There is 1 other case to deal with:
	//			FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS
	if (pbkg_size &&
		ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_KEEP_BKG_TEXTURE_PROPORTIONS) &&
		(ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN) ||
		 ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX)))
	{
		NVEC3 r;

		// 'p3dsize->coord == 0' means DON'T USE size min for this dimension !!!
		// so, in that specific case, we will have 'r.coord = 1' (as scale ratio, so without any effect !)
		if (bsize.x)
			r.x = NUT_pCurrentUIDeskSet->SizeConstraints.x / bsize.x;
		else
			r.x = 1.0f;

		if (bsize.y)
			r.y = NUT_pCurrentUIDeskSet->SizeConstraints.y / bsize.y;
		else
			r.y = 1.0f;

		if (bsize.z)
			r.z = NUT_pCurrentUIDeskSet->SizeConstraints.z / bsize.z;
		else
			r.z = 1.0f;

		// 'r.coord' <= 1 means bsize.coord >= NUT_pCurrentUIDeskSet->Background3DSizeConstraints.coord'
		// 'r.coord' > 1 means bsize.coord < NUT_pCurrentUIDeskSet->Background3DSizeConstraints.coord'
		// So according with size constraints type (Min or Max or none ) we are going to find which ratio apply.
		Nf32 f;
		if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMIN))
		{
			// We have to find the worst ratio against what we are looking for.
			// And what are we looking for ? ... All ratios smaller than 1 are working, only the biggest against 1 makes problem.
			// So let's find the worst, that means the biggest and forget all the ones which are smaller than 1.
			f = NMAX(1.0f, NMAX(r.x, NMAX(r.y, r.z)));
		}
		else if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_SIZECONSTRAINTS_IS_SIZEMAX))
		{
			// We have to find the worst ratio against what we are looking for.
			// And what are we looking for ? ... All ratios Bigger than 1 are working, only the smallest than 1 makes problem.
			// So let's find the worst, that means the smallest and forget all the ones which are bigger than 1.
			f = NMIN(1.0f, NMIN(r.x, NMIN(r.y, r.z)));
		}
		NVec3Scale(&bsize, pbkg_size, f);
	}

	*pdst_uisize = bsize;
}

void NUT_UIDesk_SetUIPositionf(NUI *puiframe, const NVEC3 *puiframesize, const Nf32 x, const Nf32 y, const Nf32 z)
{
	if (puiframesize)
	{
		NVEC3 worldpos;
		worldpos.x = x + (NUT_pCurrentUIDeskSet->BuildAnchor.x - NUT_pCurrentUIDeskSet->PositionAnchor.x) * puiframesize->x;
		worldpos.y = y + (NUT_pCurrentUIDeskSet->BuildAnchor.y - NUT_pCurrentUIDeskSet->PositionAnchor.y) * puiframesize->y;
		worldpos.z = z + (NUT_pCurrentUIDeskSet->BuildAnchor.z - NUT_pCurrentUIDeskSet->PositionAnchor.z) * puiframesize->z;
		NSetUIPosition(puiframe, &worldpos);
		worldpos.x -= NUT_pCurrentUIDeskSet->BuildAnchor.x * puiframesize->x;
		worldpos.y -= NUT_pCurrentUIDeskSet->BuildAnchor.y * puiframesize->y;
		worldpos.z -= NUT_pCurrentUIDeskSet->BuildAnchor.z * puiframesize->z;
		NUT_UIDesk_StoreUIDummyData(&worldpos, puiframesize);
	}
	else
	{
		NSetUIPositionf(puiframe, x, y, z);
		NUT_UIDesk_StoreUIDummyDataf(x, y, z, 0, 0, 0);
	}
}

void NUT_UIDesk_Apply_TouchMode(NUI_DESC *pdesc)
{
	if (ISFLAG_ON(NUT_pCurrentUIDeskSet->PropertiesFlags, FLAG_NUT_UIDESKSET_USE_TOUCH_MODE))
	{
		FLAG_ON(pdesc->Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE);
		pdesc->TouchMode = NUT_pCurrentUIDeskSet->TouchMode;
	}
	else
	{
		FLAG_OFF(pdesc->Flags_Core, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE);
	}
}
