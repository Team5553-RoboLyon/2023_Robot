#include "../NCStandard.h"
#include "../NType.h"
#include "../Maths/NVec3f32.h"
#include "../GL/Ngl.h"
#include "../NString.h"
#include "../File/NFile.h"
#include "../NCoordinateSystemConversions.h"
#include "../NErrorHandling.h"
#include "../Image/NImage.h"
#include "../Image/NTga.h"
#include "../Ntexture.h"
#include "../NString.h"

/*
// Globale
NNODELIST			NTexturesList;

void NInitializeTexturesList()
{
	NSetupNodeList(&NTexturesList);
}


void NDeleteTexturesList()
{
	NNODE	*pnode,*pnodeb;

	pnode = (NNODE*)NTexturesList.pFirst;
	while( (NNODE*)pnode != (NNODE*)&NTexturesList)
	{
		pnodeb = pnode;
		pnode=(NNODE*)pnode->pNext;
		NDeleteTexture((NTEXTURE*)pnodeb);
	}
}

NTEXTURE* NGetTextureByUID(Nu32 uid)
{
	NTEXTURE *ptext;

	ptext = (NTEXTURE*)NTexturesList.pFirst;
	while(ptext != (NTEXTURE*)&NTexturesList)
	{
		if(ptext->UID == uid)
			return ptext;

		ptext = (NTEXTURE*)ptext->pNext;
	}
	
	return NULL;
}
*/
Nbool 	NGetTextureGLInfo( NTEXTUREGLINFO *ptexture_glinfo, const NIMAGE *pimg )
{
	NErrorIf( !pimg,NERROR_NULL_POINTER );
	NErrorIf( !NIsValidImageID(pimg->ImageID),	NERROR_IMAGE_INVALID_IMAGEID );

	switch(pimg->ImageID)
	{
		case NIMAGEID_GRAY8:
			#ifdef _NWINDOWS
				ptexture_glinfo->InternalFormat = GL_LUMINANCE8;
			#endif
			#if defined _NIOS || defined _NANDROID
				ptexture_glinfo->InternalFormat = GL_LUMINANCE;
			#endif
			ptexture_glinfo->Format			= GL_LUMINANCE;
			ptexture_glinfo->Type			= GL_UNSIGNED_BYTE;
			break;

		case NIMAGEID_GRAYA8:
			#ifdef _NWINDOWS
						ptexture_glinfo->InternalFormat = GL_LUMINANCE8_ALPHA8;
			#endif
			#if defined _NIOS || defined _NANDROID
						ptexture_glinfo->InternalFormat = GL_LUMINANCE_ALPHA;
			#endif
			ptexture_glinfo->Format			= GL_LUMINANCE_ALPHA;
			ptexture_glinfo->Type			= GL_UNSIGNED_BYTE;
			break;

/*
		case NIMAGEID_RGB5: 
			ptexture_glinfo->InternalFormat = GL_RGB5;
			ptexture_glinfo->Format			= GL_RGB;
			ptexture_glinfo->Type			= ???;
			break;
*/

		case NIMAGEID_RGB8:
			#ifdef _NWINDOWS
						ptexture_glinfo->InternalFormat = GL_RGB8;
			#endif
			#if defined _NIOS || defined _NANDROID
						ptexture_glinfo->InternalFormat = GL_RGB;
			#endif
			ptexture_glinfo->Format			= GL_RGB;
			ptexture_glinfo->Type			= GL_UNSIGNED_BYTE;
			break;

		case NIMAGEID_RGBA8:
			#ifdef _NWINDOWS
						ptexture_glinfo->InternalFormat = GL_RGBA8;
			#endif
			#if defined _NIOS || defined _NANDROID
						ptexture_glinfo->InternalFormat = GL_RGBA;
			#endif
			ptexture_glinfo->Format			= GL_RGBA;
			ptexture_glinfo->Type			= GL_UNSIGNED_BYTE;
			break;

		default:
			memset(ptexture_glinfo,0,sizeof(NTEXTUREGLINFO));
			return NFALSE;
	}

	ptexture_glinfo->Width		= pimg->Width;
	ptexture_glinfo->Height		= pimg->Height;
	ptexture_glinfo->pImageData = (GLvoid*)pimg->pSurface;
	return NTRUE;
}

// ------------------------------------------------------------------------------------------
// NSetupTexture
// ------------------------------------------------------------------------------------------
// Description :
//	SetUp a Texture from an image
//  That means create a GL Texture and store its unique index + some other infos into
//	a NTEXTURE structure.
// ------------------------------------------------------------------------------------------
// In  :
//		*ptexture	: a valid ptr on a texture to Setup
//		*pimg		: a valid ptr on an NImage which be used to Setup the texture
//		
// Out :
//		== to incoming texture ptr
// ------------------------------------------------------------------------------------------
NTEXTURE*	NSetupTexture( NTEXTURE *ptexture, const Nchar *ptexture_name, const NIMAGE *pimg )
{
	GLuint			gltexture;			// This is a handle to our texture object
	Nu8				ntextureflags;
	NTEXTUREGLINFO	texture_glinfo;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + DEBUG TESTS 
	//		(in debug mode only)
	// 
	// a) We need to have a real image !
	NErrorIf( !pimg,NERROR_NULL_POINTER );
	NErrorIf( !NIsValidImageID(pimg->ImageID), NERROR_IMAGE_INVALID_IMAGEID );
	// b) We need to have a TRUE COLOR image coded with 8bits per component
	NErrorIf( pimg->ImageID == NIMAGEID_VOID,	NERROR_TEXTURE_UNSUPPORTED_IMAGEID );	// Only True Color image
	NErrorIf( NIMAGE_COLORMAP(pimg->ImageID),	NERROR_TEXTURE_UNSUPPORTED_IMAGEID );	// Only True Color image
	NErrorIf( pimg->ImageID == NIMAGEID_RGB5,	NERROR_TEXTURE_UNSUPPORTED_IMAGEID );	// Only GL_UNSIGNED_BYTE Type !
	// c) We need to have width and height power of two !
	NErrorIf( !NIsPowerOfTwo(pimg->Width),NERROR_IMAGE_INVALID_WIDTH );
	NErrorIf( !NIsPowerOfTwo(pimg->Height),NERROR_IMAGE_INVALID_HEIGHT );
	// d) We don't need to have a name for the texture ... not necessarily

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + COLLECT MORE INFORMATIONS ABOUT ALPHA CHANNEL
	//		...for future use, by the function "NGetMostSuitableAccumulator" for instance
	//		Store these informations on the texture flags parameter
	ntextureflags	= NGetImageAlphaChannelInfo(pimg);
	// Is AlphaChannel effective ?
	if( ISFLAG_ON(ntextureflags,FLAG_NTEXTURE_HAS_ALPHA) )
	{
		// Alpha is flat, full of the same intermediate value... is effective
		if( ISFLAG_ON(ntextureflags,FLAG_NTEXTURE_ALPHA_IS_FLAT) )
		{
			if( ISFLAG_ON(ntextureflags,FLAG_NTEXTURE_ALPHA_CONTAINS_INTERMEDIATE_VALUE) )
				FLAG_ON(ntextureflags,FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE);
			else // full of 0 or full of 255 ...
				FLAG_OFF(ntextureflags,FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE);
		}
		else // Alpha is not FLAT ... that means it contains different values
		{
			FLAG_ON(ntextureflags,FLAG_NTEXTURE_ALPHA_IS_EFFECTIVE);
		}
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + COLLECT OPEN GL TEXTURE INFORMATIONS FROM IMAGE
	//			These informations will help to create the texture	
	//
	NGetTextureGLInfo( &texture_glinfo,pimg );

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + CREATE OPEN GL TEXTURE
	//			
	glGenTextures( 1, &gltexture );
	glBindTexture( GL_TEXTURE_2D, gltexture );
	
	// DEFAULT: Set the texture's FILTER & Wrapping properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_CLAMP );
	// Store those default parameters in ptexture too ...
	ptexture->MinFilter = GL_LINEAR;
	ptexture->MagFilter = GL_LINEAR;
	ptexture->WrapS		= GL_CLAMP;
	ptexture->WrapT		= GL_CLAMP;

	// Edit the texture object's image data using the information 'NGetTextureGLInfo' gives us
	glTexImage2D(	GL_TEXTURE_2D, 0,
					texture_glinfo.InternalFormat,
					texture_glinfo.Width , texture_glinfo.Height ,
					0,
					texture_glinfo.Format,
					texture_glinfo.Type,
					(GLvoid*)texture_glinfo.pImageData );

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SETUP NTEXTURE
	//
	memset(ptexture,0,sizeof(NTEXTURE));
	// Some initializations
	ptexture->Flags					= ntextureflags;
	ptexture->GL_Texture			= gltexture;
	ptexture->size.x				= (Ns16)texture_glinfo.Width;
	ptexture->size.y 				= (Ns16)texture_glinfo.Height;
	ptexture->pName					= NStrDuplicate(ptexture_name);
	return ptexture;
}


NTEXTURE* NCreateTexture( const Nchar *pname, const NIMAGE *pimg )
{
	return NSetupTexture( NEW(NTEXTURE), pname, pimg );
}

// ------------------------------------------------------------------------------------------
// NClearTexture
// ------------------------------------------------------------------------------------------
// Description :
//	Delete a texture 
// ------------------------------------------------------------------------------------------
// In  :
//		pt	: NTexture pointer
//		
// Out :
//		int	: Number of texture deleted 1 or 0
//			  so, 1 is a success and 0 a fail caused by a non-zero number of instances 
//			  associated with the texture.
// ------------------------------------------------------------------------------------------
void NClearTexture(NTEXTURE* pt)
{
	NTEXTURE	*ptexture = (NTEXTURE*)pt;

	//NErrorIf(ptexture->ReferenceCount, NERROR_NON_NULL_VALUE); // It's not cool !!! that means USer try to clear/delete a texture which is referenced by some other objetcts ...
	#ifdef _DEBUG
	if(ptexture->ReferenceCount)
		printf("\n CLEAR TEXTURE ---> %s . RefCount = %d", ptexture->pName, ptexture->ReferenceCount);
	#endif
	// Delete OpenGL texture associated
	glDeleteTextures(1,&ptexture->GL_Texture);
	
	// Clear parameters
	ptexture->Flags					= 0;
	ptexture->GL_Texture			= 0;
	ptexture->size.x				= 0;
	ptexture->size.y 				= 0;
	if(ptexture->pName)
	{
		Nfree(ptexture->pName);
		ptexture->pName = NULL;
	}
}


// ------------------------------------------------------------------------------------------
// NDeleteTexture
// ------------------------------------------------------------------------------------------
// Description :
//	Delete a texture 
// ------------------------------------------------------------------------------------------
// In  :
//		pt	: NTexture pointer
//		
// Out :
//		int	: Number of texture deleted 1 or 0
//			  so, 1 is a success and 0 a fail caused by a non-zero number of instances 
//			  associated with the texture.
// ------------------------------------------------------------------------------------------
void NDeleteTexture(NTEXTURE* pt)
{
	NClearTexture(pt);
	Nfree(pt);
}


// ------------------------------------------------------------------------------------------
// NCreateTextureFromTga
// ------------------------------------------------------------------------------------------
// Description :
//	Create a Texture from an external Tga file
//  All usual formats are accepted but at the end of the day, image is going to be converted
//	as a RGB24 or a RGBA32 image.
// ------------------------------------------------------------------------------------------
// In  :
//		*file	: full name of the image file to load
//		
// Out :
//		a ptr on a new NTEXTURE structure
// ------------------------------------------------------------------------------------------
NTEXTURE*	NCreateTextureFromTga(const Nchar* ptexture_name, const Nchar* ptga_filename)
{
	NIMAGE *pimage = NCreateImageFromTga( ptga_filename);
	if( pimage )
	{
		NTEXTURE *ptexture = NCreateTexture( ptexture_name, NConformImageFormatToTextureCreationConstraint(pimage) );
		NDeleteImage(pimage);
		return ptexture;
	}
	else
	{
		return NULL;
	}
}

// ------------------------------------------------------------------------------------------
// NTEXTURE* NChangeTextureImageFromTga
// ------------------------------------------------------------------------------------------
// Description :
//	Especially for Android ... (even if this function stills available for all systems )
//	To fix/patch the OpenGL context lost on Android Systems ...
//	(used by the function NReloadAllLibraryTextures() )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NTEXTURE* NChangeTextureImageFromTga(NTEXTURE *ptexture, const Nchar* ptga_filename)
{
	// Store current texture name and current texture Reference Count ( they will be cleared )
	Nchar* ptexture_name	= NStrDuplicate(ptexture->pName);
	Nu16	ref_count		= ptexture->ReferenceCount;
	// Store current texture WrapS/T and Min/Mag filters
	GLint wraps	= ptexture->WrapS;
	GLint wrapt	= ptexture->WrapT;
	GLint minf	= ptexture->MinFilter;
	GLint magf	= ptexture->MagFilter;

	// Trick !!!
	// In debug mode, clear function check (and crash) if user try to delete 'in use' texture ( ReferenceCount must be = 0 )
#ifdef _DEBUG
	ptexture->ReferenceCount = 0;
#endif

	NClearTexture(ptexture);

	NIMAGE *pimage = NCreateImageFromTga( ptga_filename);
	if( pimage )
	{
		NSetupTexture( ptexture,ptexture_name, NConformImageFormatToTextureCreationConstraint(pimage) );
		
		// RESTORE texture Reference Count 
		ptexture->ReferenceCount = ref_count;
		
		// Restore WrapS/T and Min/Mag filters ( do it by hand )
		//			NSetTextureGLWrap(ptexture,wraps,wrapt);
		//			NSetTextureGLFilters(ptexture,minf,magf);
		glBindTexture( GL_TEXTURE_2D, ptexture->GL_Texture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minf );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magf );
		ptexture->WrapS		= wraps;
		ptexture->WrapT		= wrapt;
		ptexture->MinFilter = minf;
		ptexture->MagFilter = magf;

		// Delete temporary texture name copy
		Nfree(ptexture_name);
		
		// Delete Image
		NDeleteImage(pimage);
	}
	return ptexture;
}


/*
#define NBLTX_DICOWRAP_SIZE	4	// NBatcgLoadTextures dicow(wrap) Array size
static inline Nbool _read_wrap_glint_define(const Nchar *pstr,GLint *pwrap_param)
{
	Nchar		*dico[]={"GL_REPEAT","GL_MIRRORED_REPEAT","GL_CLAMP_TO_EDGE","GL_CLAMP_TO_BORDER"};
	GLint		trad[]	={GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER};
	for(Nu32 j=0;j<NBLTX_DICOWRAP_SIZE;j++)
	{
		if( strstr(pstr,dico[j]) )
		{ 
			*pwrap_param = trad[j];
			return NTRUE;
		}
	}
	return NFALSE;
}

#define NBLTX_DICOFILTER_SIZE	6	// NBatcgLoadTextures dicof(filter) Array size
static inline Nbool _read_filter_glint_define(const Nchar *pstr,GLint *pfilter_param)
{
	Nchar		*dico[]={"GL_NEAREST","GL_LINEAR","GL_NEAREST_MIPMAP_NEAREST","GL_LINEAR_MIPMAP_NEAREST","GL_NEAREST_MIPMAP_LINEAR","GL_LINEAR_MIPMAP_LINEAR"};
	GLint		trad[]	={GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR};
	for(Nu32 j=0;j<NBLTX_DICOFILTER_SIZE;j++)
	{
		if( strstr(pstr,dico[j]) )
		{ 
			*pfilter_param = trad[j];
			return NTRUE;
		}
	}
	return NFALSE;
}
#define NBLTX_DICO_SIZE		5	// NBatcgLoadTextures dico Array size
// ------------------------------------------------------------------------------------------
// void NSetTextureParametersFromStringDescription
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
//		"Readable" Texture Parameters are:
//
//		BANK 					=	0 to 255
//
//		WRAP_S / WRAP_T			=	GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER,GL_REPEAT,GL_MIRRORED_REPEAT	
//
//		MIN_FILTER / MAG_FILTER	=	GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,
//										GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetTextureParametersFromStringDescription(NTEXTURE *ptex, const Nchar *pstring_description)
{
	NErrorIf(strlen(pstr)>255, NERROR_STRING_TOO_LONG);
	Nchar	*dico[]={"BANK","WRAP_S","WRAP_T","MIN_FILTER","MAG_FILTER"};
	Nchar	strb[256];
	Nchar	*pstr = (Nchar*)pstring_description;
	Nu32	wordid;	
	Nu32	i;
	Ns16	s16;
	GLint	glparam;

	glBindTexture( GL_TEXTURE_2D, ptex->GL_Texture );
	// string 'stra' contains at least one ";"

	while( strlen(pstr) )
	{
		pstr = NStrGet_String_BeforeLabel(pstr,"=",strb);
		wordid = NVOID;
		for(i=0;i<NBLTX_DICO_SIZE;i++)
		{
			if( strstr(strb,dico[i]) )
			{
				wordid = i;
				break;
			}
		}

		// BANK
		if( wordid == 0 )
		{
			pstr = NStrMove_AfterLabel(pstr,"=");
			s16 = -1;
			pstr = NStrGet_Ns16(pstr,&s16);
			pstr = NStrMove_AfterLabel(pstr,";");
			NErrorIf(s16>255 || s16<0, NERROR_VALUE_OUTOFRANGE);

			if( wordid == 0 )
				ptex->BankID = (Nu8)s16;
		}
		// WRAP_S,WRAP_T
		else if( wordid < 3 )
		{
			pstr = NStrMove_AfterLabel(pstr,"=");
			pstr = NStrGet_String_BeforeLabel(pstr,";",strb);
			pstr = NStrMove_AfterLabel(pstr,";");
			if(_read_wrap_glint_define(strb,&glparam))
			{
				if( wordid == 1 )
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glparam );
				else // worldid == 2
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glparam );
			}
		}
		// MIN_FILTER,MAG_FILTER
		else if( wordid < 5 )
		{
			pstr = NStrMove_AfterLabel(pstr,"=");
			pstr = NStrGet_String_BeforeLabel(pstr,";",strb);
			pstr = NStrMove_AfterLabel(pstr,";");
			if(_read_filter_glint_define(strb,&glparam))
			{
				if( wordid == 3 )
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glparam );
				else //wordid == 4
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glparam );
			}
		}
		// NVOID or other unexpected value
		else
		{
			NErrorIf(1,NERROR_BATCHLOAD_TEXTURES_SYNTAX_ERROR); // Unknown key name readed inside the batch file.
		}
	}
}
*/
