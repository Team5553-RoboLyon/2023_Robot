#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "lib/N/NCStandard.h"
#include "../../Libraries/NLibraries.h"
#include "lib/N/Geometry/Components/NGeometryAppearance.h"
#include "NUT_Editor_ATxC_Gizmo_Drag_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Texture_InLine.h"

#include "NUT_Editor_ATxC_Tool.h"

// ------------------------------------------------------------------------------------------
// void NUT_ATxC_TextureRingActivation
// ------------------------------------------------------------------------------------------
// Description :
//	GIZMO Activation calls this function to activate or not the texture Ring Selector
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_ATxC_TextureRingActivation( NUT_EDITOR_ATXC_TOOLDATA *ptooldata )
{
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE) )
	{
		Nu8 available_tu = NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(ptooldata->Elements.GeometryIDS))->UsedTextureUnitNumber;
		if( !available_tu )
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON);
		}
		else if( !NUT_ATxC_GetTextureRingSectorNb(ptooldata) )
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON);
		}
		else
		{
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON);
		}
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON);
	}

	// ... Yes ! Texture ! So ... what about the Selector Handle ...
	if(	ISFLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON) )
	{
		Nu32 index = NUT_ATxC_GetTextureRingSectorIndex(ptooldata);
		if(index != NVOID)
		{
			NUT_ATxC_SetTextureHandlePosition(ptooldata,index);
		}
		else
		{
			// If we are here that means current (and initial ) texture is not referenced into the associated Texture List ...
			// It's not a Bug, it could happen, but in that case it's impossible to place the texture handle in a valid position
			// on the texture selector. So, just put it in the center of the GIZMO.
			NUT_ATxC_SetUndefinedTextureHandlePosition(ptooldata);
		}
	}
}

void NUT_ATxC_TextureParametersActivation( NUT_EDITOR_ATXC_TOOLDATA *ptooldata )
{
	NATLAS_ELEMENT	*patlas_element;
	NTEXTURE		*ptexture;

	if( NUT_ATxC_GetCurrentMetaTexturePtrs(&patlas_element, &ptexture, ptooldata)!= NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL )
	{
		// WRAP
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE_WRAP_PARAMETERS) ) 
		{
			FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON);
			glBindTexture(GL_TEXTURE_2D,ptexture->GL_Texture);
			glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		&ptooldata->Elements.TextureWrapS );
			glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		&ptooldata->Elements.TextureWrapT );
		}
		else
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON);
		}

		// FILTER
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE_FILTER_PARAMETERS) ) 
		{
			FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON);
			glBindTexture(GL_TEXTURE_2D,ptexture->GL_Texture);
			glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	&ptooldata->Elements.TextureMinFilter );
			glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	&ptooldata->Elements.TextureMagFilter );
		}
		else
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON);
		}
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON);
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON);
	}
}

Nbool NUT_ATxC_PickTextureRing(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2 *pnorm_mouse)
{
	NVEC2	v0,vm,vex;
	Nf32		l,halfthickness;

	// Distance mouse from texture ring center:
	NUT_ATxC_GetDrag_HandleCenter( &v0, &ptooldata->HUDGizmo );
	NVec2Sub(&vm,pnorm_mouse,&v0);
	l = NVec2Length(&vm);

	// Compare this distance with the texture circle radius:
	NUT_ATxC_GetTextureRingSelector_Extents(&vex, &ptooldata->HUDGizmo );
	halfthickness = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize)*0.5f;

	if( l > vex.x + halfthickness ) // We know vex.x = vex.y, its a circle !
		return NFALSE;
	if( l < vex.x - halfthickness ) // We know vex.x = vex.y, its a circle !
		return NFALSE;

	return NTRUE;
}

static inline Nu32 _ATxC_get_texturenb(const NARRAY *parray_of_texture)
{
	if( parray_of_texture == CONSTANT_NUT_ATXC_TEXTURELIBRARY )
	{
		return NGetTexturesLibraryCount();
	}
	else if( parray_of_texture )
	{
		return parray_of_texture->Size;
	}
	else// parray_texture == NULL
	{
	 	return 0;
	}
}
static inline Nu32 _ATxC_get_atlaselementnb(const NARRAY *parray_of_atlas_el)
{
	if( parray_of_atlas_el == CONSTANT_NUT_ATXC_ATLAS )
	{
		return NGetAtlasCount();
	}
	else if( parray_of_atlas_el )
	{
		return parray_of_atlas_el->Size;
	}
	else // parray_of_atlas_el == NULL
	{
		return 0;
	}
}
Nu32 NUT_ATxC_GetTextureRingSectorNb(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	// Number of Texture Ring Sectors is the sum of:
	//		Number of Textures
	//		Number of Atlas Element
	//		+1 if NULL Texture pointer is allowed by user.
	// ... may be 0 or 1 without any texture/Atlas element (due to the potential allowed NULL texture)
	
	Nu32	extended_size;
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED ) )
	{
		extended_size = 1;
	}
	else
	{
		extended_size = 0;
	}
	
	return	(	extended_size +
				_ATxC_get_texturenb(ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit]) +
				_ATxC_get_atlaselementnb(ptooldata->pCurrentEntity->pArrayOfAtlasElementPtr[ptooldata->HUDGizmo.EditedTextureUnit])
			);
}

// ------------------------------------------------------------------------------------------
// Nu32 NUT_ATxC_GetTextureRingSectorIndex
// ------------------------------------------------------------------------------------------
// Description :
//	Try to deduce Ring Sector Index from a NTEXTURE* pointer.
//	Texture Ring can contain Texture and Atlas Element, so it could
//	be possible to find several potential sector indexes for a single 'ptexture' pointer.
//	for example:
//		Several Atlas Element (of the same texture) with 1 sector for each of them ...
//		1 Texture (full, i mean not an Atlas Element) and 1 Atlas Element ...
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//		return NVOID if texture/Atlas element is not included into the current entity arrays!
//		return 0 if ptexture == NULL and FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED is set to ON
//		return 0 also if ptexture != NULL and FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED is set to OFF
// ------------------------------------------------------------------------------------------
Nu32 NUT_ATxC_GetTextureRingSectorIndex(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	NTEXTURE		**ptexptr;
	NATLAS_ELEMENT	**ppatlas_el_ptr;
	Nu32			index;
	Nu32			indexshifting;
	Nu32			i;
	NARRAY			*parray;
	//NRECTf32		oaarectf32={0};
	NATLAS_ELEMENT	*patlas_el	= NULL;
	NTEXTURE		*ptexture	= NULL;
//	Nu8				type		= NU8_MAX;	// 

	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED ) )
		indexshifting = 1;
	else
		indexshifting = 0;

	// Extract "Edited Texture Unit" data
	switch( ptooldata->Elements.MetaTextureType[ptooldata->HUDGizmo.EditedTextureUnit] )
	{
		// "Current" MetaTextureType is TYPE_NULL that means "current" MetaTexture pointer is NULL...
		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL:
			NErrorIf(ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NON_NULL_POINTER);
			
			if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED ) )
				return 0;
			else
				return NVOID;
			break;

		// "Current" MetaTextureType is TYPE_PLAIN that means "current" MetaTexture pointer is non-NULL and is a NTEXTURE pointer
		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN:
			ptexture	= (NTEXTURE*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			NErrorIf(!ptexture, NERROR_NULL_POINTER);

			parray = ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit];
			if( parray == CONSTANT_NUT_ATXC_TEXTURELIBRARY )
			{
				if( NGetTexturesLibraryCount() )
				{
					// look for the current texture into the Texture Library
					i = NGetLibraryTextureValidIndex(ptexture->pName);
					NErrorIf(i==NVOID,NERROR_HASHMAP_INVALID_KEY );
					if(i==NVOID)
						return NVOID;
					else
						return (i+indexshifting);
				}
			}
			else if(parray)  // parray != NULL && parray != CONSTANT_NUT_ATXC_TEXTURELIBRARY
			{
				// look for the current texture into the Array of Preloaded Texture Ptr
				index = NVOID;
				ptexptr = (NTEXTURE**)NGetFirstArrayPtr(parray);
				for (i=0;i< NGetArraySize(parray);i++,ptexptr++)
				{
					if(*ptexptr == ptexture)
					{
						return i+indexshifting;
					}
				}
				return NVOID;
			}
			else // parray == NULL
			{
				// ATxC Tool doesn't have any texture list to look for into.
				return NVOID;
			}
			break;

		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT:
			patlas_el	= (NATLAS_ELEMENT*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			NErrorIf(!patlas_el, NERROR_NULL_POINTER);
			
			indexshifting += _ATxC_get_texturenb(ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit]);

			parray = ptooldata->pCurrentEntity->pArrayOfAtlasElementPtr[ptooldata->HUDGizmo.EditedTextureUnit];
			if( parray == CONSTANT_NUT_ATXC_ATLAS )
			{
				if( NGetAtlasCount() )
				{
					// look for the current Atlas Element into the Atlas
					i = NGetAtlasElementValidIndex(patlas_el->pName);
					NErrorIf(i==NVOID,NERROR_HASHMAP_INVALID_KEY );
					
					if(i==NVOID)
						return NVOID;
					else
						return (i+indexshifting);
				}
			}
			else if(parray)// parray != NULL && parray != CONSTANT_NUT_ATXC_ATLAS
			{
				// look for the atlas element into the Array of Atlas element Ptr
				index = NVOID;
				ppatlas_el_ptr = (NATLAS_ELEMENT **)NGetFirstArrayPtr(parray);
				for (i=0;i< NGetArraySize(parray);i++,ppatlas_el_ptr++)
				{
					if(*ppatlas_el_ptr == patlas_el)
					{
						return i+indexshifting;
					}
				}
				return NVOID;
			}
			else  // parray == NULL
			{
				// ATxC Tool doesn't have any Atlas element array to look for into.
				return NVOID;
			}
			break;

		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			break;
	}

	return NVOID;
}

void* NUT_ATxC_GetTextureRingSectorMetaTexture(Nu8 *pmetatexturetype, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 sectorindex)
{
	Nu32			used_index;
	NTEXTURE		*ptexture;
//	NATLAS_ELEMENT	*patlas_element;

	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED ) )
	{
		if( sectorindex == 0 )
		{
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL;
			return NULL;
		}
		else
		{
			used_index = sectorindex - 1;
		}
	}
	else
	{
		used_index = sectorindex;
	}
	
	// USedIndex Check: It has to be into the right range
	NErrorIf(used_index>=_ATxC_get_texturenb(ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit]) + _ATxC_get_atlaselementnb(ptooldata->pCurrentEntity->pArrayOfAtlasElementPtr[ptooldata->HUDGizmo.EditedTextureUnit]), NERROR_VALUE_OUTOFRANGE );

	// Figure out Sector data type, PLAIN TEXTURE or ATLAS ELEMENT
	NARRAY *parray;
	// 1/ PLAIN TEXTURE !
	if( used_index < _ATxC_get_texturenb(ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit]) )
	{
		parray = ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit];
		if( parray == CONSTANT_NUT_ATXC_TEXTURELIBRARY )
		{
			ptexture = NLookupLibraryTextureByValidIndex(used_index);
			NErrorIf(!ptexture,NERROR_NULL_POINTER);//It shouldn't happen !
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN;
			return (void*)ptexture;
		}
		else if(parray)
		{
			ptexture = *(NTEXTURE**)NGetArrayPtr(parray,used_index);
			NErrorIf(!ptexture,NERROR_NULL_POINTER);//It shouldn't happen !
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN;
			return (void*)ptexture;
		}
		else // parray == NULL
		{
			// It shouldn't never happen ! 
			NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL;
			return NULL;
		}
	}
	else
	{
		NATLAS_ELEMENT	*patlas_el;
		used_index -= _ATxC_get_texturenb(ptooldata->pCurrentEntity->pArrayOfTexturePtr[ptooldata->HUDGizmo.EditedTextureUnit]);
		
		parray = ptooldata->pCurrentEntity->pArrayOfAtlasElementPtr[ptooldata->HUDGizmo.EditedTextureUnit];
		if( parray == CONSTANT_NUT_ATXC_ATLAS )
		{
			patlas_el = NLookupAtlasElementByValidIndex(used_index);
			NErrorIf(!patlas_el,NERROR_NULL_POINTER);//It shouldn't happen !
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT;
			return (void*)patlas_el;
		}
		else if(parray)
		{
			patlas_el = *(NATLAS_ELEMENT**)NGetArrayPtr(parray,used_index);
			NErrorIf(!patlas_el,NERROR_NULL_POINTER);//It shouldn't happen !
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT;
			return (void*)patlas_el;
		}
		else // parray == NULL
		{
			// It shouldn't never happen ! 
			NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL;
			return NULL;
		}
	}
}

void* NUT_ATxC_GetCurrentMetaTexture(Nu8 *pmetatexturetype, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata )
{
	NTEXTURE		*ptexture;
	NATLAS_ELEMENT	*patlas_el;

	switch( ptooldata->Elements.MetaTextureType[ptooldata->HUDGizmo.EditedTextureUnit] )
	{
		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL:
			NErrorIf(ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NON_NULL_POINTER);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL;
			return NULL;

		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN:
			ptexture	= (NTEXTURE*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			NErrorIf(!ptexture, NERROR_NULL_POINTER);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN;
			return (void*)ptexture;

		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT:
			patlas_el	= (NATLAS_ELEMENT*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			NErrorIf(!patlas_el, NERROR_NULL_POINTER);
			NErrorIf(!patlas_el->pTexture, NERROR_NULL_POINTER);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT;
			return (void*)patlas_el;

		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			*pmetatexturetype = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL;
			return NULL;
	}
}

Nu8 NUT_ATxC_GetCurrentMetaTexturePtrs(NATLAS_ELEMENT **ppmetatexture_atlaselement, NTEXTURE **ppmetatexture_texture, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata )
{
	NATLAS_ELEMENT	*patlas_el;
	Nu8				type = ptooldata->Elements.MetaTextureType[ptooldata->HUDGizmo.EditedTextureUnit];
	switch( type )
	{
		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL:
			NErrorIf(ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NON_NULL_POINTER);
			*ppmetatexture_atlaselement = NULL;
			*ppmetatexture_texture		= NULL;
			return type;

		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN:
			NErrorIf(!ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NULL_POINTER);
			*ppmetatexture_atlaselement = NULL;
			*ppmetatexture_texture		= (NTEXTURE*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			return type;

		case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT:
			patlas_el	= (NATLAS_ELEMENT*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
			NErrorIf(!patlas_el, NERROR_NULL_POINTER);
			NErrorIf(!patlas_el->pTexture, NERROR_NULL_POINTER);
			*ppmetatexture_atlaselement = patlas_el;
			*ppmetatexture_texture		= patlas_el->pTexture;
			return type;

		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			*ppmetatexture_atlaselement = NULL;
			*ppmetatexture_texture		= NULL;
			return type;
	}
}


void NUT_ATxC_SetTextureHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 sectorindex)
{
	NVEC2	vex;
	Nf32		_cos,_sin;
	Nu32		sectornb = NUT_ATxC_GetTextureRingSectorNb(ptooldata);
	
	NErrorIf(!sectornb,NERROR_NULL_VALUE);
	NErrorIf(sectorindex>=sectornb,NERROR_VALUE_OUTOFRANGE);

	Nf32 angle=((Nf32)sectorindex+0.5f)*360.0f/(Nf32)sectornb;
	NFastCosSin( NDEGtoANGLE(angle), &_cos,&_sin );
	NUT_ATxC_GetTextureRingSelector_Extents( &vex, &ptooldata->HUDGizmo );
	ptooldata->HUDGizmo.TextureHandlePosition.x = vex.x * _cos;
	ptooldata->HUDGizmo.TextureHandlePosition.y = vex.y * _sin;
}

void NUT_ATxC_SetUndefinedTextureHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	ptooldata->HUDGizmo.TextureHandlePosition.x = 0.0f;
	ptooldata->HUDGizmo.TextureHandlePosition.y = 0.0f;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
