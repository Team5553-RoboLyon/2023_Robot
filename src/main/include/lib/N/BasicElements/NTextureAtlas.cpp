#include "../NCStandard.h"
#include "../File/NFile.h"
#include "../NString.h"
#include "../NErrorHandling.h"

#include "NtextureAtlas.h"

// Global 
NOAHASHMAP	NAtlas;

// Internal functions used only into this .cpp
static void NAtlasElement_destructor_callback(void*pelement)
{
	NErrorIf(!((NATLAS_ELEMENT*)pelement)->pTexture, NERROR_NULL_POINTER);
	NUnregisterTextureReference(((NATLAS_ELEMENT*)pelement)->pTexture);

	Nfree(((NATLAS_ELEMENT*)pelement)->pName);
	Nfree(pelement);
}

// ------------------------------------------------------------------------------------------
// void NInitializeAtlas
// ------------------------------------------------------------------------------------------
// Description :
//	THE Atlas is a list of rectangular areas of texture(s).
//	It is unique. This function initialize the OAHashMap behind it ...
// ------------------------------------------------------------------------------------------
// In	:
// Out :
// ------------------------------------------------------------------------------------------
void NInitializeAtlas()
{
	NHASHMAP_DESC	desc;
	Nmem0(&desc,NHASHMAP_DESC);
	FLAG_ON(desc.Flags,FLAG_NHASHMAP_DESC_USE_ALLPARAMS);
	desc.Capacity_PowOfTwo	= CONSTANT_NATLAS_INITIALIZING_SIZE;
	desc.Hash_FCT			= NHashMap_optimizedFletcher32_SmallString_Hash_FCT;
	desc.KeyCompare_FCT		= NHashMap_String_Compare_FCT;
	NSetupOAHashMap(&NAtlas,&desc);
}
// ------------------------------------------------------------------------------------------
// void NClearAtlas
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
void NClearAtlas()
{
	NClearOAHashMap(&NAtlas,NAtlasElement_destructor_callback);
}

Nu32 NGetAtlasCount()
{
	return NAtlas.BucketCount;
}

NATLAS_ELEMENT* NLookupAtlasElementByValidIndex(const Nu32 validentry_index)
{
	return (NATLAS_ELEMENT*)NLookupValidOAHashMapIndexEntry(&NAtlas,validentry_index);
}

Nu32 NGetAtlasElementValidIndex(const Nchar *pkeyname)
{
	return NLookupValidOAHashMapEntryIndex(&NAtlas,pkeyname);
}



NATLAS_ELEMENT *NCreateAtlasElement(const Nchar *pname, NTEXTURE *ptexture, const NRECTs16 *prect )
{
	NErrorIf(!pname,NERROR_NULL_POINTER);// An Atlas Element needs a valid pname pointer to Work
	NErrorIf(!ptexture,NERROR_NULL_POINTER);// An Atlas Element needs a valid texture pointer to Work

	NATLAS_ELEMENT	*pel = (NATLAS_ELEMENT*)Nmalloc(sizeof(NATLAS_ELEMENT));
	Nmem0(pel,NATLAS_ELEMENT);
	pel->pName		= NStrDuplicate(pname);
	pel->pTexture	= ptexture;

	if(prect)
	{
		pel->UsedOAARects16 = *prect;
	}
	else
	{
		pel->UsedOAARects16.upper_left.x	= pel->UsedOAARects16.upper_left.y = 0;
		pel->UsedOAARects16.lower_right.x	= ptexture->size.x;
		pel->UsedOAARects16.lower_right.y	= ptexture->size.y;
	}

	NATLAS_ELEMENT *pret = (NATLAS_ELEMENT *)NInsertOAHashMapEntry(&NAtlas,pel->pName,pel);
	NErrorIf(pret != pel,NERROR_NLIBRARY_NON_UNIQUE_ENTRY);	// That means an already inserted texture Atlas element with the same Name was replaced by incoming "pel"
															// pel->pName should be unique ! So in DEBUG ... CRASH !
															
	NRegisterTextureReference(pret->pTexture);				// In RELEASE consider the already build in Atlas Element as the good one. 
															// and return it !
	return pret;											
}

void NDeleteAtlasElement(const Nchar *pname )
{
	NClearOAHashMapEntry(&NAtlas,pname,NAtlasElement_destructor_callback);
}

void NLoadAtlasElements(const char* filename, NTEXTURE *ptexture, const NLOADATLASELEMENT_CALLBACK callback, Nu32 Nu32_param)
{
	NErrorIf( !NCheckFileType(filename,EXTENSION_ATLAS),NERROR_FILE_INVALID_EXTENSION );
	NErrorIf(!ptexture,NERROR_NULL_POINTER);

	NATLAS_ELEMENT	*pel;
	Nchar			tempstringA[256];
	Nchar			atlas_element_name[256];
	NRECTs16		rects16;
	Nchar			*preader;
	Ns16			ox,oy,width,height;
	NFILE			*pfile;

	pfile = NFileOpen(filename, FLAG_NFILE_READ);
	NFileSeek(pfile, 0, NFILE_SEEK_SET);

	while( NFileGetString(tempstringA, 256, pfile) ) //Line get
	{
		preader =	tempstringA;
		preader =	NStrGet_String_BeforeLabel(preader," = ",atlas_element_name);
		preader =	NStrMove_AfterLabel(preader," = ");
		preader =	NStrGet_Ns16(preader,&ox);
		preader =	NStrMove_AfterLabel(preader," ");
		preader =	NStrGet_Ns16(preader,&oy);
		preader =	NStrMove_AfterLabel(preader," ");
		preader =	NStrGet_Ns16(preader,&width);
		preader =	NStrMove_AfterLabel(preader," ");
		preader =	NStrGet_Ns16(preader,&height);
		
		// Checks against texture
		NErrorIf( width > ptexture->size.x, NERROR_INCONSISTENT_PARAMETERS);
		NErrorIf( height > ptexture->size.y, NERROR_INCONSISTENT_PARAMETERS);

		// ROTATED ATLAS ELEMENT ???
		// Very often, atlas element has been rotated into their "mother" texture to fit in.
		// This information is written inside the file as CCW90,CCW180 or CCW270. CCW0 is useless but accepted ( it does nothing)
		if( strstr(preader,"ccw") )
		{
			Ns32 ccw ;
			preader =	NStrMove_AfterLabel(preader,"ccw");
			NStrGet_Ns32(preader, &ccw);

			switch(ccw)
			{
				case 0: // accepted : it will exactly like NO CCW !
					rects16.left	= ox;
					rects16.top		= oy;
					rects16.right	= ox  + width;
					rects16.bottom	= oy  + height;
					break;

				case 90:
					rects16.left	= ox + width;
					rects16.top		= oy;
					rects16.right	= ox;
					rects16.bottom	= oy + height;
					break;

				case 180:
					rects16.left	= ox + width;
					rects16.top		= oy + height;
					rects16.right	= ox;
					rects16.bottom	= oy;
					break;

				case 270:
					rects16.left	= ox;
					rects16.top		= oy + height;
					rects16.right	= ox + width;
					rects16.bottom	= oy ;
					break;
				
				default:
					NErrorIf(1,NERROR_INVALID_CASE);
					break;
			}
		}
		else // NO CCW ( most common case actually )
		{
			rects16.left	= ox;
			rects16.top		= oy;
			rects16.right	= ox + width;
			rects16.bottom	= oy + height;
		}

		pel = NCreateAtlasElement(atlas_element_name,ptexture,&rects16);
		if(callback)
			callback(pel,Nu32_param);

	}
	NFileClose(pfile);
}

NATLAS_ELEMENT *NLookupAtlasElement(const Nchar *pelement_name)
{
	return (NATLAS_ELEMENT*)NLookupOAHashMapEntry(&NAtlas,(const void*)pelement_name);
}
// ------------------------------------------------------------------------------------------
// NRECTf32* NGetAtlasElementUsedOAARectf32
// ------------------------------------------------------------------------------------------
// Description :
//		Compute and return the rectangle used by the Atlas element.
//		This is an OAA rectangle !
//		Coordinates are relative to the texture size, so width and height of this rectangle
//		are Normalized. So, for each of the four coordinates we have:
//							0 < coordinate < 1
//		In case of an Atlas Element without a valid Texture Inside, return Rectf32 will be fill
//		up with ZEROs.
// ------------------------------------------------------------------------------------------
// In	:
//			NRECTf32		*poaarect	The OAA Rectangle structure to fill in.
//			NATLAS_ELEMENT	*pel		The Atlas Element to get in.
// Out :
//			return the incoming "NRECTf32 poaarect" pointer.
//
// ------------------------------------------------------------------------------------------
NRECTf32* NGetAtlasElementUsedOAARectf32(NRECTf32 *poaarectf32, const NATLAS_ELEMENT *pel)
{
	NErrorIf(!poaarectf32 || !pel || !pel->pTexture, NERROR_NULL_POINTER);
	NErrorIf(!pel->pTexture->size.x || !pel->pTexture->size.y , NERROR_NULL_VALUE);
	if(pel->pTexture)
	{
		poaarectf32->upper_left.x	= (Nf32)pel->UsedOAARects16.upper_left.x	/ (Nf32)pel->pTexture->size.x; 
		poaarectf32->upper_left.y	= (Nf32)pel->UsedOAARects16.upper_left.y	/ (Nf32)pel->pTexture->size.y;
		poaarectf32->lower_right.x	= (Nf32)pel->UsedOAARects16.lower_right.x	/ (Nf32)pel->pTexture->size.x; 
		poaarectf32->lower_right.y	= (Nf32)pel->UsedOAARects16.lower_right.y	/ (Nf32)pel->pTexture->size.y;
	}
	else
	{
		Nmem0(poaarectf32,NRECTf32);
	}
	return poaarectf32;
}

