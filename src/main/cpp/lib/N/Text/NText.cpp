#include "lib/N/NCStandard.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/NViewport.h"
#include "lib/N/Ntext.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/Geometry/Components/NGeometryAppearance.h"
#include "lib/N/Render/NFrustum_Culling.h"
#include "lib/N/Render/Renderable/NRenderable.h"
#include "lib/N/Render/Renderable/NRenderable_ExtractSet.h"
#include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

NTEXT *NSetupText(NTEXT *ptext, const NTEXT_DESC *ptextdesc)
{
	NGEOMETRY_FORMAT_ENUM rsid;
	NSTATESET user_stateset_flags;
	NRENDERABLE_DESC rdesc;
	NGEOMETRY_DESC geomdesc;
	Ns32 i;
	NGEOMETRY *pgeom;
	Nu8 font_tu_id;

	NErrorIf(ptextdesc->pFont->PageNb == 0, NERROR_NULL_VALUE); // We nned at least one page of character to create a text in the right way !

	NVec3Set(&ptext->Cursor, 0, 0, 0);

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_GEOMETRY_FORMAT))
	{
		rsid = ptextdesc->GeometryFormat;
	}
	else
	{
		rsid = DEFAULT_NTEXT_GEOMETRIES_SPECS_ID;
	}
	// NSTATESET						UserStateSet;
	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_USERSTATESET))
	{
		user_stateset_flags = ptextdesc->UserStateSet;
	}
	else
	{
		user_stateset_flags = DEFAULT_NTEXT_USERSTATESET;
	}
	// NEXTRACTSET						HighLevelExtractSet;
	NEXTRACTSET highlevel_extractset;
	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_USEREXTRACTSET))
	{
		highlevel_extractset = ptextdesc->HighLevelExtractSet;
	}
	else
	{
		highlevel_extractset = DEFAULT_NTEXT_HIGHLEVEL_EXTRACTSET;
	}
	NErrorIf(NGET_EXTRACTSET_LOWLEVEL(highlevel_extractset), NERROR_RENDERABLE_EXTRACTSET_UNAUTHORIZED_FLAGS_USING);

	// TEXTURE UNIT
	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_FONT_TEXTUREUNIT))
	{
		font_tu_id = ptextdesc->Font_TextureUnit;
	}
	else
	{
		font_tu_id = 0;
	}

	// Geometry Checks:
	//		MESH:			NVERTEX_V3C4T2 and NPRIMITIVE_TRIANGLE  only.
	//		APPEARANCE:		1 TextureUnit ONLY !
	//						No other constraints ...
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(NGetGeometrySpecs(rsid)->IDS) != NPRIMITIVE_FORMAT_TRIANGLE, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(NGetGeometrySpecs(rsid)->IDS) != NVERTEX_FORMAT_V3C4T2, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT);
	NErrorIf(NGetGeometrySpecs(rsid)->UsedTextureUnitNumber < 1, NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_FONT))
	{
		ptext->pFont = ptextdesc->pFont;
	}
	else
	{
		ptext->pFont = NGetDefaultFont();
	}
	NErrorIf(!ptext->pFont, NERROR_TEXT_FONT_MISSING);

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_ANCHOR))
	{
		ptext->Anchor = ptextdesc->Anchor;
	}
	else
	{
		NVec3Set(&ptext->Anchor, DEFAULT_NTEXT_ANCHOR_3F);
	}

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_CONSTRUCTION_ORIGIN))
	{
		ptext->ConstructionOrigin = ptextdesc->ConstructionOrigin;
	}
	else
	{
		NVec3Set(&ptext->ConstructionOrigin, DEFAULT_NTEXT_CONSTRUCTION_ORIGIN_3F);
	}
	ptext->Cursor = ptext->ConstructionOrigin;

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_ALIGNMENT))
	{
		ptext->Alignment = (Nu8)ptextdesc->Alignment;
	}
	else
	{
		ptext->Alignment = (Nu8)DEFAULT_NTEXT_ALIGNMENT;
	}

	// Setup the 4 COLORS range used to build up each character
	NVEC3 vgrad;
	vgrad.x = 0.0f;
	vgrad.y = 0.0f;
	vgrad.z = 0.0f;
	NGetBuildBaseColorVertexColor(&ptext->ColorRange4[0], 0, 4, (Nf32 *)&vgrad, &ptextdesc->ColorBuild);
	vgrad.x = 1.0f;
	vgrad.y = 0.0f;
	NGetBuildBaseColorVertexColor(&ptext->ColorRange4[1], 1, 4, (Nf32 *)&vgrad, &ptextdesc->ColorBuild);
	vgrad.x = 1.0f;
	vgrad.y = 1.0f;
	NGetBuildBaseColorVertexColor(&ptext->ColorRange4[2], 2, 4, (Nf32 *)&vgrad, &ptextdesc->ColorBuild);
	vgrad.x = 0.0f;
	vgrad.y = 1.0f;
	NGetBuildBaseColorVertexColor(&ptext->ColorRange4[3], 3, 4, (Nf32 *)&vgrad, &ptextdesc->ColorBuild);

	// potentially the "Renderable" could have to display characters from all the different font textures
	// It's the reason why we create this "Renderable" with this Pre-allocated number of geometries.
	memset(&rdesc, 0, sizeof(NRENDERABLE_DESC));
	rdesc.IncludedGeometryPreallocatedCapacity = (Nu16)ptext->pFont->PageNb;
	rdesc.Update_FCT = NRenderableUpdate_Text;
	if (ptext->pFont->PageNb == 1)
	{
		rdesc.Extract_FCT = NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(CONSTANT_NTEXT_SINGLEPAGE_FONT_LOWLEVEL_EXTRACTSET, highlevel_extractset));
	}
	else if (ptext->pFont->PageNb > 1)
	{
		rdesc.Extract_FCT = NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(CONSTANT_NTEXT_MULTIPAGES_FONT_LOWLEVEL_EXTRACTSET, highlevel_extractset));
	}

	if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_TRANSFORMHNODE))
	{
		rdesc.pTransformHNode = ptextdesc->pThn;
	}
	else
	{
		rdesc.pTransformHNode = NULL; // No Default pThn !
	}
	ptext->pRenderable = NCreateRenderable(&rdesc);

	// Create and Store One Geometry for each Page (= Texture within several characters )
	for (i = 0; i < ptext->pFont->PageNb; i++)
	{
		// original version
		// NSetUpGeometryDesc_Triangle(&geomdesc,NGetMostSuitableStateSet(pfont->pNTextures[i],ptextdesc->StateSet_Specflags),0,0,pfont->pNTextures[i]);
		// NRenderableGeometryPushBack(ptext->pRenderable,NSetUpGeometry(&geom,&geomdesc));

		// version +
		NFillupGeometryDesc(&geomdesc, rsid, user_stateset_flags, DEFAULT_NTEXT_GEOMETRY_PRIMITIVE_CAPACITY, DEFAULT_NTEXT_GEOMETRY_VERTEX_CAPACITY);
		pgeom = NGeometryEmplaceBack(ptext->pRenderable, &geomdesc);
		// version + Appearance
		if (ISFLAG_ON(ptextdesc->Flags, FLAG_NTEXT_DESC_USEPARAM_APPEARANCE))
		{
			// Check : Incoming appearance can't use the Font texture unit ( where font  texture is going to be )!!
			NErrorIf(ptextdesc->Appearance_Raw.TextureUnit[font_tu_id].pTexture != NULL, NERROR_INCONSISTENT_PARAMETERS);
			NSetGeometryAppearance(pgeom, &ptextdesc->Appearance_Raw);
		}
		// version + Font Texture into the right TextureUnit
		NSetGeometryTexture(pgeom, font_tu_id, ptext->pFont->pNTextures[i]);
	}

	return ptext;
}

NTEXT *NCreateText(const NTEXT_DESC *ptextdesc)
{
	return NSetupText(NEW(NTEXT), ptextdesc);
}

void NClearText(void *ptext)
{
	NDeleteRenderable(((NTEXT *)ptext)->pRenderable);
	memset(ptext, 0, sizeof(NTEXT));
}

void NDeleteText(void *ptext)
{
	NClearText(ptext);
	Nfree(ptext);
}

void NSetTextMaterial(NTEXT *ptext, const NMATERIAL *pmaterial)
{
	NGEOMETRY *pgeom = (NGEOMETRY *)ptext->pRenderable->GeomArray.pFirst;
	for (Nu32 i = ptext->pRenderable->GeomArray.Size; i != 0; i--, pgeom++)
	{
		NSetGeometryMaterial(pgeom, pmaterial);
	}
}

void NSetTextBlend(NTEXT *ptext, const NBLEND *pblend)
{
	NGEOMETRY *pgeom = (NGEOMETRY *)ptext->pRenderable->GeomArray.pFirst;
	for (Nu32 i = ptext->pRenderable->GeomArray.Size; i != 0; i--, pgeom++)
	{
		NSetGeometryBlend(pgeom, pblend);
	}
}

void NRenderableUpdate_Text(NRENDERABLE *prenderable, void *powner, const NTIME *ptime)
{
	// =========================================================================
	// EXTRACT (and return if no extraction ... )
	if (!prenderable->Extract_FCT(prenderable, powner))
	{
		return;
	}
	// =========================================================================
}
static inline void _charpushback(NTEXT *ptext, NAABB *paabb, const Nu32 nchar_code)
{
	// Character Shape
	//
	//	line0 ---------------------------------------------------
	//										-		-			-
	//										|		|			|
	//										|		| YOffset	|
	//										|		|			|
	//		 0 ------- 1 -----	Top			|		-			|
	//		 |         |					| BASE				|
	//		 |         |					|					| LineHeight
	//		 |         |					|					|
	//		 |         |					|					|
	//		 |         |					|					|
	//		 3 ------- 2 -----	Bottom		-					|
	//															|
	//															-
	//	line1 ---------------------------------------------------
	//
	Nu16 index0;
	NCHAR *pnc;
	NGEOMETRY *pgeom;

	NTEXTVERTEX v0, vertex;
	NTRIANGLE triangle;

	// INSERT CHARACTER
	pnc = &ptext->pFont->pNCharTable[nchar_code];

	// Choose the right geometry (...and page/texture)
	pgeom = (NGEOMETRY *)NGetArrayPtr(&ptext->pRenderable->GeomArray, pnc->Page);
	index0 = (Nu16)NGetArraySize(&pgeom->Mesh.VertexArray);

	// ========
	// Vertex 0
	// ========
	v0.Position_3f.x = ptext->Cursor.x + pnc->HRelXOffset;
	v0.Position_3f.y = ptext->Cursor.y - pnc->HRelYOffset;
	v0.Position_3f.z = ptext->Cursor.z;
	v0.Color0_4f = ptext->ColorRange4[0];
	v0.TexCoord0_2f.x = pnc->U0;
	v0.TexCoord0_2f.y = pnc->V0;
	NArrayPushBack(&pgeom->Mesh.VertexArray, (NBYTE *)&v0);

	// update Text AABB (only X and Y, assuming Z still the same...)
	if (v0.Position_3f.x < paabb->fXMin)
		paabb->fXMin = v0.Position_3f.x;
	if (v0.Position_3f.y > paabb->fYMax)
		paabb->fYMax = v0.Position_3f.y;

	// ========
	// Vertex 1
	// ========
	vertex.Position_3f.x = v0.Position_3f.x + pnc->HRelWidth;
	vertex.Position_3f.y = v0.Position_3f.y;
	vertex.Position_3f.z = v0.Position_3f.z;
	vertex.Color0_4f = ptext->ColorRange4[1];
	vertex.TexCoord0_2f.x = pnc->U0 + pnc->USize;
	vertex.TexCoord0_2f.y = pnc->V0;
	NArrayPushBack(&pgeom->Mesh.VertexArray, (NBYTE *)&vertex);

	// ========
	// Vertex 2
	// ========
	vertex.Position_3f.x = v0.Position_3f.x + pnc->HRelWidth;
	vertex.Position_3f.y = v0.Position_3f.y - pnc->HRelHeight;
	vertex.Position_3f.z = v0.Position_3f.z;
	vertex.Color0_4f = ptext->ColorRange4[2];
	vertex.TexCoord0_2f.x = pnc->U0 + pnc->USize;
	vertex.TexCoord0_2f.y = pnc->V0 + pnc->VSize;
	NArrayPushBack(&pgeom->Mesh.VertexArray, (NBYTE *)&vertex);

	// update Text AABB (only X and Y, assuming Z still the same...)
	if (vertex.Position_3f.x > paabb->fXMax)
		paabb->fXMax = vertex.Position_3f.x;
	if (vertex.Position_3f.y < paabb->fYMin)
		paabb->fYMin = vertex.Position_3f.y;

	// ========
	// Vertex 3
	// ========
	vertex.Position_3f.x = v0.Position_3f.x;
	vertex.Position_3f.y = v0.Position_3f.y - pnc->HRelHeight;
	vertex.Position_3f.z = v0.Position_3f.z;
	vertex.Color0_4f = ptext->ColorRange4[3];
	vertex.TexCoord0_2f.x = pnc->U0;
	vertex.TexCoord0_2f.y = pnc->V0 + pnc->VSize;
	NArrayPushBack(&pgeom->Mesh.VertexArray, (NBYTE *)&vertex);

	// ==========
	// Primitives
	// ==========
	triangle.i0 = index0;
	triangle.i1 = index0 + 1;
	triangle.i2 = index0 + 2;
	NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
	triangle.i0 = index0;
	triangle.i1 = index0 + 2;
	triangle.i2 = index0 + 3;
	NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);

	ptext->Cursor.x += pnc->HRelXAdvance; // for space character too .... obviously !!!
}

static inline Nf32 _get_next_tabstop(const NTEXT *ptext)
{
	// A little bit special to avoid Nf32 approximation error ...
	Nf32 f0 = (ptext->Cursor.x - ptext->ConstructionOrigin.x);
	Nf32 f1 = ceil(f0 / ptext->pFont->HRelTab) * ptext->pFont->HRelTab;
	// We are sure of that:  f1 >= f0
	// But because of the floating point error we have to integrate an error margin in our test
	// ... so if we detect cursor.x too close from f1 "tab stop", we jump to the next one. ( too close means less than 1/2 space )
	if ((f1 - f0) < 0.5f * ptext->pFont->pNCharTable[' '].HRelXAdvance)
		f1 += ptext->pFont->HRelTab;

	return ptext->ConstructionOrigin.x + f1;
}

static inline Nf32 _get_next_tabstop_xtd(const NFONT *pfont, const Nf32 cursor_x)
{
	// A little bit special to avoid Nf32 approximation error ...
	Nf32 f0 = cursor_x / pfont->HRelTab;
	Nf32 f1 = ceil(f0) * pfont->HRelTab;
	// We are sure of that:  f1 >= cursor_x
	// But because of the floating point error we have to integrate an error margin in our test
	// ... so if we detect cursor.x too close from f1 "tab stop", we jump to the next one. ( too close means less than 1/2 space )
	if ((f1 - cursor_x) < 0.5f * pfont->pNCharTable[' '].HRelXAdvance)
		f1 += pfont->HRelTab;

	return f1;
}

#define FLAG_BUILDTEXTMESH_ALIGNMENT_CENTER_CALCULATED BIT_0
void NBuildTextMeshFromString(NTEXT *ptext, const Nchar *pstring)
{
	NGEOMETRY *pgeom;
	NTEXTVERTEX *pv;
	NVEC3 vshift;
	Nu32 len;
	Nu32 i, j;
	NAABB aabb;
	Nf32 center;
	Nu32 flags = 0;

	// FIRST OF ALL
	// Reset all the Renderable Geometries
	pgeom = (NGEOMETRY *)ptext->pRenderable->GeomArray.pFirst;
	for (i = ptext->pRenderable->GeomArray.Size; i != 0; i--, pgeom++)
	{
		NEraseGeometryMesh(pgeom);
	}

	// Important to reset Cursor to Initial position (ConstructionOrigin) each time before building a new mesh.
	ptext->Cursor = ptext->ConstructionOrigin;
	// ... so reset AABB
	aabb.fXMin = aabb.fXMax = ptext->Cursor.x;
	aabb.fYMin = aabb.fYMax = ptext->Cursor.y;
	aabb.fZMin = aabb.fZMax = ptext->Cursor.z;

	// No text to print
	if (!pstring)
	{
		// 		if( ISFLAG_ON(ptext->Flags,FLAG_NTEXT_MESHBUILT) )
		// 		{
		// 			FLAG_OFF(ptext->Flags,FLAG_NTEXT_MESHBUILT);
		// 			FLAG_ON(ptext->Flags,FLAG_NTEXT_MESH_UPDATED_BOUNDINGVOLUME_HAS_TO_BE_UPDATED);
		// 		}
		// Reset Boundding Sphere (here it's the best place... Like this we avoid reset it when it will be update some lines after ...)
		ptext->pRenderable->BoundingSphere.Center = ptext->Cursor;
		ptext->pRenderable->BoundingSphere.fRadius = 0.0f;
		return;
	}

	// Create text
	len = (long)strlen(pstring);
	if (!len)
	{
		// 		if( ISFLAG_ON(ptext->Flags,FLAG_NTEXT_MESHBUILT) )
		// 		{
		// 			FLAG_OFF(ptext->Flags,FLAG_NTEXT_MESHBUILT);
		// 			FLAG_ON(ptext->Flags,FLAG_NTEXT_MESH_UPDATED_BOUNDINGVOLUME_HAS_TO_BE_UPDATED);
		// 		}
		// Reset Boundding Sphere (here it's the second best best place... Like this we avoid reset it when it will be update some lines after ...)
		ptext->pRenderable->BoundingSphere.Center = ptext->Cursor;
		ptext->pRenderable->BoundingSphere.fRadius = 0.0f;
		return;
	}

	Nchar *pstr = (Nchar *)pstring;
	for (i = 0; i < len; i++, pstr++)
	{
		// LINE FEED
		switch (*pstr)
		{
		// New Line
		case '\n':
			switch (ptext->Alignment)
			{
			case NTEXT_ALIGNMENT_LEFT:
				ptext->Cursor.x = ptext->ConstructionOrigin.x;
				break;
			case NTEXT_ALIGNMENT_RIGHT:
				ptext->Cursor.x -= NGetStringLineLength(pstr + 1, ptext->pFont);
				break;
			case NTEXT_ALIGNMENT_CENTER:
				if (ISFLAG_OFF(flags, FLAG_BUILDTEXTMESH_ALIGNMENT_CENTER_CALCULATED))
				{
					center = ptext->ConstructionOrigin.x + (ptext->Cursor.x - ptext->ConstructionOrigin.x) * 0.5f;
					FLAG_ON(flags, FLAG_BUILDTEXTMESH_ALIGNMENT_CENTER_CALCULATED);
				}
				ptext->Cursor.x = center - NGetStringLineLength(pstr + 1, ptext->pFont) * 0.5f;
				break;
			}
			ptext->Cursor.y -= ptext->pFont->HRelCommonLineHeight;
			break;

		// Horizontal Tabulation
		case '\t':
			ptext->Cursor.x = _get_next_tabstop(ptext);
			break;

		// Space
		case ' ':
			ptext->Cursor.x += ptext->pFont->pNCharTable[' '].HRelXAdvance; // for space character too .... obviously !!!
			break;

		// Print char
		default:
			_charpushback(ptext, &aabb, *pstr);
			break;
		}
	}

	// Update Renderable BoundingSphere from AAB
	NGetAABBBoundingSphere(&ptext->pRenderable->BoundingSphere, &aabb);

	// Take care of text anchoring. ( which text box 'point' IS ON the construction Origin...)
	// And take care for ConstructionOrigin (especially x) which needs to be WHERE Text Box TOP/LEFT/BACK corner
	// has to be before anchoring, and, due to NTEXT_ALIGNMENT or just due to the text mesh building way ...
	// ... AABB boundaries do not fit and the whole text has to be shifted.
	vshift.x = -ptext->Anchor.x * (aabb.fXMax - aabb.fXMin) - (aabb.fXMin - ptext->ConstructionOrigin.x); // 'LEFT'... to do because of justification !!!
	vshift.y = -ptext->Anchor.y * (aabb.fYMax - aabb.fYMin);											  // - (aabb.fYMax-ptext->ConstructionOrigin.y); // 'TOP' ... not to do to preserve Line Height !
	vshift.z = -ptext->Anchor.z * (aabb.fZMax - aabb.fZMin);											  // - (aabb.fZMin-ptext->ConstructionOrigin.z); // 'BACK' ... useless now because text is flat

	if (vshift.x || vshift.y || vshift.z)
	{
		// Update all vertex position
		pgeom = (NGEOMETRY *)ptext->pRenderable->GeomArray.pFirst;
		for (i = ptext->pRenderable->GeomArray.Size; i != 0; i--, pgeom++)
		{
			pv = (NTEXTVERTEX *)pgeom->Mesh.VertexArray.pFirst;
			for (j = pgeom->Mesh.VertexArray.Size; j != 0; j--, pv++)
			{
				NVec3AddTo((NVEC3 *)pv, &vshift);
			}
		}
		// Update BoundSphere Center (Radius still the same...)
		NVec3AddTo(&ptext->pRenderable->BoundingSphere.Center, &vshift);
	}
}

// ------------------------------------------------------------------------------------------
// Nu32 NGetStringLineSize
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NGetStringLineSize(NVEC3 *pvsize, const Nchar *pstr, const NFONT *pfont)
{
	Nu32 i;
	Nu32 nbchar;
	Nu32 nchar_code;
	NVEC3 cursor, pos;
	NAABB aabb;
	NCHAR *pnc;

	cursor.x = cursor.y = cursor.z = 0.0f;
	pos.x = pos.y = pos.z = 0.0f;
	aabb.fXMin = aabb.fXMax = pos.x;
	aabb.fYMin = aabb.fYMax = pos.y;
	aabb.fZMin = aabb.fZMax = pos.z;

	nbchar = (Nu32)strlen(pstr);
	for (i = 0; i < nbchar; i++)
	{
		nchar_code = (Nu32)pstr[i];

		if (nchar_code == '\n')
		{
			NVec3Sub(pvsize, &aabb.VMax, &aabb.VMin);
			return (i + 1); // to stay coherent, return value doesn't include the terminated char, here it's '\n'
		}					// ... but it's a 'number of char' not an index, so ... +1
		else if (nchar_code == '\t')
		{
			cursor.x = _get_next_tabstop_xtd(pfont, cursor.x);
		}
		else
		{
			pnc = &pfont->pNCharTable[nchar_code];
			if (nchar_code != ' ') // Space
			{
				// Like charpushback vertex 0
				pos.x = cursor.x + pnc->HRelXOffset;
				pos.y = cursor.y - pnc->HRelYOffset;
				// pos.z = cursor.z; ... useless, but maybe not for a long...
				if (pos.x < aabb.fXMin)
					aabb.fXMin = pos.x;
				if (pos.y > aabb.fYMax)
					aabb.fYMax = pos.y;

				// Like charpushback vertex 2
				pos.x += pnc->HRelWidth;
				pos.y -= pnc->HRelHeight;
				// pos.z = cursor.z; ... useless, but maybe not for a long...
				if (pos.x > aabb.fXMax)
					aabb.fXMax = pos.x;
				if (pos.y < aabb.fYMin)
					aabb.fYMin = pos.y;
			}
			cursor.x += pnc->HRelXAdvance;
		}
	}

	NVec3Sub(pvsize, &aabb.VMax, &aabb.VMin);
	return nbchar;
}
// ------------------------------------------------------------------------------------------
// Nu32 NGetStringSize
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NGetStringSize(NVEC3 *pvsize, const Nchar *pstr, const NFONT *pfont)
{
	Nu32 i;
	Nu32 nbchar;
	Nu32 nchar_code;
	NVEC3 cursor, pos;
	NAABB aabb;
	NCHAR *pnc;

	cursor.x = cursor.y = cursor.z = 0.0f;
	pos.x = pos.y = pos.z = 0.0f;
	aabb.fXMin = aabb.fXMax = pos.x;
	aabb.fYMin = aabb.fYMax = pos.y;
	aabb.fZMin = aabb.fZMax = pos.z;

	nbchar = (Nu32)strlen(pstr);
	for (i = 0; i < nbchar; i++)
	{
		nchar_code = (Nu32)pstr[i];

		if (nchar_code == '\n')
		{
			cursor.x = 0.0f;
			cursor.y -= pfont->HRelCommonLineHeight;
		}
		else if (nchar_code == '\t')
		{
			cursor.x = _get_next_tabstop_xtd(pfont, cursor.x);
		}
		else
		{
			pnc = &pfont->pNCharTable[nchar_code];
			if (nchar_code != ' ')
			{
				// Like charpushback vertex 0
				pos.x = cursor.x + pnc->HRelXOffset;
				pos.y = cursor.y - pnc->HRelYOffset;
				// pos.z = cursor.z; ... useless, but maybe not for a long...
				if (pos.x < aabb.fXMin)
					aabb.fXMin = pos.x;
				if (pos.y > aabb.fYMax)
					aabb.fYMax = pos.y;

				// Like charpushback vertex 2
				pos.x += pnc->HRelWidth;
				pos.y -= pnc->HRelHeight;
				// pos.z = cursor.z; ... useless, but maybe not for a long...
				if (pos.x > aabb.fXMax)
					aabb.fXMax = pos.x;
				if (pos.y < aabb.fYMin)
					aabb.fYMin = pos.y;
			}
			cursor.x += pnc->HRelXAdvance;
		}
	}

	NVec3Sub(pvsize, &aabb.VMax, &aabb.VMin);
	return nbchar;
}
// ------------------------------------------------------------------------------------------
// Nf32 NGetStringLineLength
// ------------------------------------------------------------------------------------------
// Description :
//		Calculate and return the "justification" of a full line of text.
//		Justification is the distance between the first and the position of the x_cursor.
//		It's not the same thing as the length of the line, which is supposed to integrate
//		all characters size. Justification is "always <= length" due to character typo offset.
//		This function is very useful to calculate text justification along text building process.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nf32 NGetStringLineLength(const Nchar *pstr, const NFONT *pfont)
{
	Nf32 cursorx = 0.0f;
	Nu32 nbchar = (Nu32)strlen(pstr);
	for (Nu32 i = 0; i < nbchar; i++)
	{
		if (pstr[i] == '\n')
		{
			return cursorx;
		}
		else if (pstr[i] == '\t')
		{
			cursorx = _get_next_tabstop_xtd(pfont, cursorx);
		}
		else
		{
			cursorx += pfont->pNCharTable[pstr[i]].HRelXAdvance;
		}
	}
	return cursorx;
}

void NScaleTextColorRange(NTEXT *ptext, const NCOLOR *c)
{
	NCOLOR *pcol = ptext->ColorRange4;
	for (Nu32 i = 4; i != 0; i--, pcol++)
	{
		pcol->red *= c->red;
		pcol->green *= c->green;
		pcol->blue *= c->blue;
		pcol->alpha *= c->alpha;
	}
}

void NScaleTextColorRangef(NTEXT *ptext, const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a)
{
	NCOLOR *pcol = ptext->ColorRange4;
	for (Nu32 i = 4; i != 0; i--, pcol++)
	{
		pcol->red *= r;
		pcol->green *= g;
		pcol->blue *= b;
		pcol->alpha *= a;
	}
}

void NScaleTextColorRangeComponent(NTEXT *ptext, const Nu32 component_id, const Nf32 s)
{
	NErrorIf(component_id > 3, NERROR_VALUE_OUTOFRANGE); // 4 Components ... so ID 0 to 3 max !!!
	NCOLOR *pcol = ptext->ColorRange4;
	for (Nu32 i = 4; i != 0; i--, pcol++)
	{
		pcol->comp[component_id] *= s;
	}
}

void NUpdateTextColor(NTEXT *ptext)
{
	NTEXTVERTEX *pv;
	NCOLOR *psrccol = ptext->ColorRange4;
	NGEOMETRY *pgeom = (NGEOMETRY *)ptext->pRenderable->GeomArray.pFirst;

	for (Nu32 i = ptext->pRenderable->GeomArray.Size; i != 0; i--, pgeom++)
	{
		if (pgeom->Mesh.VertexArray.Size)
		{
			pv = (NTEXTVERTEX *)pgeom->Mesh.VertexArray.pFirst;
			// 4 by 4 because text manages quads, 4 vertex for each quad ( for each char )!
			for (Nu32 j = pgeom->Mesh.VertexArray.Size; j != 0; j -= 4, pv += 4)
			{
				pv[0].Color0_4f = psrccol[0];
				pv[1].Color0_4f = psrccol[1];
				pv[2].Color0_4f = psrccol[2];
				pv[3].Color0_4f = psrccol[3];
			}
		}
	}
}
