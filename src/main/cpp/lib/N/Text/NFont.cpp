#include "lib/N/NCStandard.h"
#include "lib/N/File/NFile.h"
#include "lib/N/NString.h"

#include "lib/N/NFont.h"
#include "lib/N/NCoordinateSystemConversions.h"

// Global
NNODELIST NFontsList;

void NInitializeFontsList()
{
	NSetupNodeList(&NFontsList);
}

void NDeleteFontsList()
{
	NNODE *pnode, *pnodeb;

	pnode = (NNODE *)NFontsList.pFirst;
	while ((NNODE *)pnode != (NNODE *)&NFontsList)
	{
		pnodeb = pnode;
		pnode = (NNODE *)pnode->pNext;
		NDeleteFont((NFONT *)pnodeb);
	}
}

NFONT *NGetDefaultFont()
{
	if ((NFONT *)NFontsList.pFirst != (NFONT *)&NFontsList)
		return (NFONT *)NFontsList.pFirst;
	else
		return NULL;
}

// ------------------------------------------------------------------------------------------
// NDeleteFont
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteFont(NFONT *pfont)
{
	// Remove NNODE from the Doubly-Linked List
	if (!NNodeIsOrphan((NNODE *)pfont))
		NNodeRemove((NNODE *)pfont);

	for (Ns32 i = 0; i < pfont->PageNb; i++)
	{
		NDeleteTexture(pfont->pNTextures[i]);
	}

	Nfree(pfont->pNCharTable);
	Nfree(pfont->pNTextures);
	Nfree(pfont);
}

// ------------------------------------------------------------------------------------------
// NCreateFontFromFile
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NFONT *NCreateFontFromFile(const char *filename, const Nbool _adaptative_size)
{
	NTEXTURE *ptexture;
	NFILE *pfile;
	NFONT *pfont;
	Nchar tempString[256] = {0};
	Nchar texturename[256] = {0};
	int i;
	Ns32 id;
	Ns32 read_value;

	// Check font file description extension
	NErrorIf(!NCheckFileType(filename, EXTENSION_FNT), NERROR_FILE_INVALID_EXTENSION);

	pfont = NEW(NFONT);
	pfont->pNCharTable = (NCHAR *)Nmalloc(sizeof(NCHAR) * NFONT_CHAR_TABLE_SIZE);

	// open "file.fnt" and check
	pfile = NFileOpen(filename, FLAG_NFILE_READ);
	if (!pfile)
		return NULL;

	NFileSeek(pfile, 0, NFILE_SEEK_SET);

	NFileGetString(tempString, 256, pfile); // Line Jump
	NFileGetString(tempString, 256, pfile); // Line get

	NStrGet_Ns32_AfterLabel(tempString, "common lineHeight=", &pfont->CommonLineHeight);
	NErrorIf(!pfont->HRelCommonLineHeight, NERROR_NULL_VALUE);

	NStrGet_Ns32_AfterLabel(tempString, "base=", &pfont->Base);
	NErrorIf(!pfont->HRelBase, NERROR_NULL_VALUE);

	NStrGet_Ns32_AfterLabel(tempString, "scaleW=", &pfont->PageWidth);
	NStrGet_Ns32_AfterLabel(tempString, "scaleH=", &pfont->PageHeight);
	NStrGet_Ns32_AfterLabel(tempString, "pages=", &pfont->PageNb);

	if (_adaptative_size)
	{
		FLAG_ON(pfont->Flags, FLAG_NFONT_ADAPTATIVE_SIZE);
		pfont->HRelCommonLineHeight = NSize_RefPixToHRel_H(pfont->CommonLineHeight);
		pfont->HRelBase = NSize_RefPixToHRel_H(pfont->Base);
	}
	else
	{
		FLAG_OFF(pfont->Flags, FLAG_NFONT_ADAPTATIVE_SIZE);
		pfont->HRelCommonLineHeight = NSize_PixToHRel_H(pfont->CommonLineHeight);
		pfont->HRelBase = NSize_PixToHRel_H(pfont->Base);
	}

	pfont->pNTextures = (NTEXTURE **)Nmalloc(sizeof(NTEXTURE *) * pfont->PageNb);

	memset(pfont->pNTextures, 0, sizeof(NTEXTURE *) * pfont->PageNb);
	for (i = 0; i < pfont->PageNb; i++)
	{
		NFileGetString(tempString, 256, pfile);
		NStrGet_String_BetweenLabels(tempString, "=\"/", "\"", texturename); // get the string between =" and "
		ptexture = NCreateTextureFromTga(texturename, texturename);
		pfont->pNTextures[i] = ptexture;
	}
	NFileGetString(tempString, 256, pfile);
	NStrGet_Ns32_AfterLabel(tempString, "chars count=", &pfont->NCharNb);

	for (i = 0; i < pfont->NCharNb; i++)
	{
		NFileGetString(tempString, 256, pfile);
		NStrGet_Ns32_AfterLabel(tempString, "id=", &id);

		// Relative to Page size data
		// These ones are Viewport independent. Changing Viewport has NO effects on them !
		// It's why they are PRE-calculated.
		NStrGet_Ns32_AfterLabel(tempString, "x=", &read_value);
		pfont->pNCharTable[id].U0 = (Nf32)read_value / (Nf32)pfont->PageWidth;
		// pfont->pNCharTable[id].X = read_value; // we don't keep this member anymore, it's not usefull
		NStrGet_Ns32_AfterLabel(tempString, "y=", &read_value);
		pfont->pNCharTable[id].V0 = (Nf32)read_value / (Nf32)pfont->PageHeight;
		// pfont->pNCharTable[id].Y = read_value; // we don't keep this member anymore, it's not usefull

		// if .USize and .VSize still viewport independent like .U0 and .V0
		// .Width and .Height have to stay in memory too, because we need them to pre-calculate some other viwport DEPENDENT data
		NStrGet_Ns32_AfterLabel(tempString, "width=", &pfont->pNCharTable[id].Width);
		pfont->pNCharTable[id].USize = (Nf32)pfont->pNCharTable[id].Width / (Nf32)pfont->PageWidth;
		NStrGet_Ns32_AfterLabel(tempString, "height=", &pfont->pNCharTable[id].Height);
		pfont->pNCharTable[id].VSize = (Nf32)pfont->pNCharTable[id].Height / (Nf32)pfont->PageHeight;
		NStrGet_Ns32_AfterLabel(tempString, "xoffset=", &pfont->pNCharTable[id].XOffset);
		NStrGet_Ns32_AfterLabel(tempString, "yoffset=", &pfont->pNCharTable[id].YOffset);
		NStrGet_Ns32_AfterLabel(tempString, "xadvance=", &pfont->pNCharTable[id].XAdvance);
		NStrGet_Ns32_AfterLabel(tempString, "page=", &pfont->pNCharTable[id].Page);

		if (_adaptative_size)
		{
			pfont->pNCharTable[id].HRelWidth = NSize_RefPixToHRel_W(pfont->pNCharTable[id].Width);
			pfont->pNCharTable[id].HRelHeight = NSize_RefPixToHRel_H(pfont->pNCharTable[id].Height);
			pfont->pNCharTable[id].HRelXOffset = NSize_RefPixToHRel_W(pfont->pNCharTable[id].XOffset);
			pfont->pNCharTable[id].HRelYOffset = NSize_RefPixToHRel_H(pfont->pNCharTable[id].YOffset);
			pfont->pNCharTable[id].HRelXAdvance = NSize_RefPixToHRel_W(pfont->pNCharTable[id].XAdvance);
		}
		else
		{
			pfont->pNCharTable[id].HRelWidth = NSize_PixToHRel_W(pfont->pNCharTable[id].Width);
			pfont->pNCharTable[id].HRelHeight = NSize_PixToHRel_H(pfont->pNCharTable[id].Height);
			pfont->pNCharTable[id].HRelXOffset = NSize_PixToHRel_W(pfont->pNCharTable[id].XOffset);
			pfont->pNCharTable[id].HRelYOffset = NSize_PixToHRel_H(pfont->pNCharTable[id].YOffset);
			pfont->pNCharTable[id].HRelXAdvance = NSize_PixToHRel_W(pfont->pNCharTable[id].XAdvance);
		}
	}
	// And the tabulation ( which is not saved but precomputed from blank space size)
	pfont->HRelTab = (Nf32)CONSTANT_NFONT_TAB_SPACE_COUNT * pfont->pNCharTable[' '].HRelXAdvance;
	NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);
	NFileClose(pfile);

	// ===========================================================
	// Insert NTFONT Into the Global Fonts Doubly-Linked List
	// ===========================================================
	NNodeInsertEnd((NNODE *)pfont, &NFontsList);

	return pfont;
}

// After a viewport updating all used fonts have to be updated by using this function
void NUpdateFontConstants(NFONT *pfont)
{
	Ns32 i;

	if (ISFLAG_ON(pfont->Flags, FLAG_NFONT_ADAPTATIVE_SIZE))
	{
		pfont->HRelCommonLineHeight = NSize_RefPixToHRel_H(pfont->CommonLineHeight);
		pfont->HRelBase = NSize_RefPixToHRel_H(pfont->Base);
		for (i = 0; i < NFONT_CHAR_TABLE_SIZE; i++)
		{
			pfont->pNCharTable[i].HRelWidth = NSize_RefPixToHRel_W(pfont->pNCharTable[i].Width);
			pfont->pNCharTable[i].HRelHeight = NSize_RefPixToHRel_H(pfont->pNCharTable[i].Height);
			pfont->pNCharTable[i].HRelXOffset = NCoord_RefPixToHRel_X(pfont->pNCharTable[i].XOffset);
			pfont->pNCharTable[i].HRelYOffset = NCoord_RefPixToHRel_Y(pfont->pNCharTable[i].YOffset);
			pfont->pNCharTable[i].HRelXAdvance = NCoord_RefPixToHRel_X(pfont->pNCharTable[i].XAdvance);
		}
	}
	else
	{
		pfont->HRelCommonLineHeight = NSize_PixToHRel_H(pfont->CommonLineHeight);
		pfont->HRelBase = NSize_PixToHRel_H(pfont->Base);
		for (i = 0; i < NFONT_CHAR_TABLE_SIZE; i++)
		{
			pfont->pNCharTable[i].HRelWidth = NSize_PixToHRel_W(pfont->pNCharTable[i].Width);
			pfont->pNCharTable[i].HRelHeight = NSize_PixToHRel_H(pfont->pNCharTable[i].Height);
			pfont->pNCharTable[i].HRelXOffset = NCoord_PixToHRel_X(pfont->pNCharTable[i].XOffset);
			pfont->pNCharTable[i].HRelYOffset = NCoord_PixToHRel_Y(pfont->pNCharTable[i].YOffset);
			pfont->pNCharTable[i].HRelXAdvance = NCoord_PixToHRel_X(pfont->pNCharTable[i].XAdvance);
		}
	}

	NErrorIf(!pfont->HRelCommonLineHeight, NERROR_NULL_VALUE);
	NErrorIf(!pfont->HRelBase, NERROR_NULL_VALUE);

	// And the tabulation ( after char update to use HRelXAdvance of Blank Space)
	pfont->HRelTab = (Nf32)CONSTANT_NFONT_TAB_SPACE_COUNT * pfont->pNCharTable[' '].HRelXAdvance;
	NErrorIf(!pfont->HRelTab, NERROR_NULL_VALUE);
}

// ------------------------------------------------------------------------------------------
// NGetAlphaEffectiveFontPageNumber
// ------------------------------------------------------------------------------------------
// Description :
//		Check the alpha level status of each font page (texture) to find out how many of them
//		have an effective Alpha. Helpful to decide if it's necessary to activate "alpha blending".
//		Reminder:	Effective Alpha ...
//					... There is an alpha level.
//					... Is not full of 0, or full of 1.
// ------------------------------------------------------------------------------------------
// In  :
//			const NFONT		*pfont	a valid pointer on a NFONT structure
// Out :
//			Nu32			Number of font pages (textures) with a effective Alpha.
// ------------------------------------------------------------------------------------------
Nu32 NGetAlphaEffectiveFontPageNumber(const NFONT *pfont)
{
	NErrorIf(!pfont, NERROR_NULL_POINTER);
	Nu32 nb = 0;
	for (Ns32 i = 0; i < pfont->PageNb; i++)
	{
		if (ISFLAG_ON(pfont->pNTextures[i]->Flags, FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE))
			nb++;
	}
	return nb;
}

// ------------------------------------------------------------------------------------------
// NGetFontInfo_CharacterSizeMax
// ------------------------------------------------------------------------------------------
// Description :
//			Find and return trough pvsize the highest and the largest dimension of all the
//			characters of the Font.
//			!! The highest dimension doesn't come from the same character of the largest !!
//			!! Understand : the highest of ALL the characters and the largest of ALL the characters !!
// ------------------------------------------------------------------------------------------
// In  :
//			NVEC2			*pvsize	a valid pointer on a NVEC2 structure to receive result
//			const NFONT			*pfont	a valid pointer on a NFONT structure
// Out :
// ------------------------------------------------------------------------------------------
/*
void NGetFontInfo_CharacterSizeMax(NVEC2 *pvsize, const NFONT *pfont)
{
	NCHAR		*pnc;
	NVEC2	size;
	size.x=size.y=0;
	for(Ns32 i=0;i<pfont->NCharNb;i++)
	{
			pnc = &pfont->pNCharTable[i];

			if(pnc->HRelWidth>size.x)
				size.x = pnc->HRelWidth;
			if(pnc->HRelWidth>size.y)
				size.y = pnc->HRelHeight;
	}
	*pvsize = size;
}
*/