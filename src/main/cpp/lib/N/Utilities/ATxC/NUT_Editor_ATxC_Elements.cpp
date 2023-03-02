#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "../../NType.h"
#include "../../NCStandard.h"
#include "NUT_Editor_ATxC_Tool.h"

void NUT_Editor_ATxC_SetupElements(NUT_EDITOR_ATXC_ELEMENTS *pelements)
{
	// This Setup Function is going to setup NUT_EDITOR_ATXC_ELEMENTS at its default state.
	Nmem0(pelements, NUT_EDITOR_ATXC_ELEMENTS);
	
	// Nu32			Flags;				... Full of ZEROs is fine !
	// Nu32			GeometryIDS;		... Full of ZEROs, mean = 0, mean NGEOMETRY_FORMAT_POINT_TU0_V3_POINT 
	
	// Textures Coordinates Controllers
	// NTEXCOORD_2f	TexCoordOffset[];	... Full of ZEROs is not enough specific ! So let's be more specific ...	
	// NTEXCOORD_2f	TexCoordScale[];	... Full of ZEROs is not enough specific ! So let's be more specific ...	
	for(Nu32 i=0;i<CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB;i++)
	{
		// Offset
		pelements->TexCoordOffset[i].x		= 0.0f;
		pelements->TexCoordOffset[i].y		= NTxCYFacingDown(0.0f);

		// Scale
		pelements->TexCoordScale[i].x		= 1.0f;
		pelements->TexCoordScale[i].y		= NTxCYFacingDown(1.0f);
	}

	// Appearance Controllers
	// NBLEND		*pBlend;			... Full of ZEROs is fine !
	// NMATERIAL	*pMaterial;			... Full of ZEROs is fine !
	// void			*pTexture[];		... Full of ZEROs is fine !
	// Nu8			MetaTextureType[];		... Full of ZEROs is fine ! But, to be crystal clear, let's be more specific ...
	for(Nu32 i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
	{
		pelements->MetaTextureType[i]	= NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL; // which is 0... but any way ... Cristal Clear !!!!
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_SetEditorATxC_Elements_FromAppearance
// ------------------------------------------------------------------------------------------
// Description :
//	Setup all the "Appearance Controllers" members of "NUT_EDITOR_ATXC_ELEMENTS" from an NAPPEARANCE_RAW structure
//		NUT_EDITOR_ATXC_ELEMENTS.pBlend;
//		NUT_EDITOR_ATXC_ELEMENTS.pMaterial;
//		NUT_EDITOR_ATXC_ELEMENTS.pTexture[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];
//		NUT_EDITOR_ATXC_ELEMENTS.TextureType[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];
//
//		Notes:	All the NUT_EDITOR_ATXC_ELEMENTS.TextureType are going to be setup as ...TEXTURE_TYPE_PLAIN
//				Because NAPPEARANCE_RAW doesn't store any ATLAS information, texture are just... texture.
//
//				So, prefer using other NUT_SetEditorATxC_Elements_... functions if you want to be more specific.
//				( more specific about texture ... )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_SetEditorATxC_Elements_FromAppearance(NUT_EDITOR_ATXC_ELEMENTS *pelements,  const NAPPEARANCE_RAW *pappearance)
{
	pelements->pBlend		= pappearance->pBlend;
	pelements->pMaterial	= pappearance->pMaterial;
	
	for(Nu32 i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
	{
		pelements->pMetaTexture[i]		= pappearance->TextureUnit[i].pTexture;
		pelements->MetaTextureType[i]	= NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN;
	}
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
