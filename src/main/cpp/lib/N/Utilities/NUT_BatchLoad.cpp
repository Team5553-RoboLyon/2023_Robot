
// ------------------------------------------------------------------------------------------------------------------------------------------
// -
// -
// -
#include "../GL/Ngl.h"
#include "../Containers/NHashMap.h"
#include "../Image/NTga.h"
#include "../NErrorHandling.h"
#include "../NString.h"
#include "../File/NFile.h"


#ifdef _NLIBS
#include "../Libraries/NLibraries.h"
extern 	NOAHASHMAP	TexturesLibrary;
extern 	NOAHASHMAP	BlendsLibrary;
extern 	NOAHASHMAP	MaterialsLibrary;
#endif

#include "NUT_BatchLoad.h"

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

// ------------------------------------------------------------------------------------------
// void NBatchLoadTextures
// ------------------------------------------------------------------------------------------
// Description :
//	load Textures list. that means load image (.tga) as texture.
//	... And load texture parameters and Texture BankID if these informations are writen explicitely in the batch file.
// ------------------------------------------------------------------------------------------
// In	:
// 
//		Readable Texture Parameters are:
//
//		BANK or +BANK				= 0 to 255
//
//		WRAP_S or +WRAP_S		 	
//		WRAP_T or +WRAP_T			= GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER,GL_REPEAT,GL_MIRRORED_REPEAT	
//
//		MIN_FILTER or +MIN_FILTER	= GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,
//		MAG_FILTER or +MAG_FILTER	  GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR
// Out :
//
//
// ------------------------------------------------------------------------------------------
#define NBLTX_DICO_SIZE		10	// NBatcgLoadTextures dico Array size
void NBatchLoadTextures(const Nchar *pbatchfilename, const NBATCHLOADTEXTURES_CALLBACK callback, Nu32 param)
{
	//Check file description extension
	NErrorIf(!NCheckFileType(pbatchfilename,NBATCHLOAD_TEXTURES_FILE_EXTENSION),NERROR_FILE_INVALID_EXTENSION);
	
	Nchar		*dico[]={"+BANK","BANK","+WRAP_S","WRAP_S","+WRAP_T","WRAP_T","+MIN_FILTER","MIN_FILTER","+MAG_FILTER","MAG_FILTER"};

	NFILE		*pfile;
	Nchar		stra[256];
	Nchar		strb[256];
	Nchar		*pstr;
	Nchar		*pstrtga;

	NTEXTURE	*ptex;
	Nu32		l;
	Nu32		i;
	Nu32		wordid;
//	Ns16		s16;
	Nbool		bnewtx;
	GLint		glparam;
	//open and check
	pfile = NFileOpen(pbatchfilename,FLAG_NFILE_READ);
	if(!pfile)
		return;

	NFileSeek(pfile,0,NFILE_SEEK_SET);

	stra[0]=0;
	while( NFileGetString(stra, 256, pfile) ) 
	{
		// to remove the potential end line characters (one or two) (especially on IOS)
		if( ( l=strlen(stra) ) )
		{
			l--;
			while( (stra[l]=='\n')||(stra[l]=='\r'))
			{
				stra[l]=0; 
				l--;
			}
		}

		if( strlen(stra) )
		{
			if( strstr(stra,";") )
			{
				pstr = NStrGet_String_BeforeLabel(stra,";",strb);
				pstr = NStrMove_AfterLabel(pstr,";");
				pstrtga = strb;
			}
			else
			{
				pstrtga = stra;
				pstr	= strb;
				strb[0]=0;
			}

#ifdef _NLIBS	
			ptex = (NTEXTURE*)NLookupOAHashMapEntry(&TexturesLibrary,pstrtga);// To avoid replacing pre-existing texture with the same key
			if( !ptex ) 
			{
				ptex = NCreateTextureFromTga(pstrtga,pstrtga);
				NErrorIf(!ptex,NERROR_NULL_POINTER);
				NInsertOAHashMapEntry(&TexturesLibrary,ptex->pName,ptex);
				bnewtx = NTRUE;
			}
			else
			{
				bnewtx = NFALSE;
			}
#endif
#ifndef _NLIBS
			ptex = NCreateTextureFromTga(pstrtga,pstrtga);
			NErrorIf(!ptex,NERROR_NULL_POINTER);
			bnewtx = NTRUE;
#endif
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

				// +BANK and BANK
				if( wordid <= 1 )
				{
					NErrorIf(1,NERROR_SYSTEM_CHECK);
/*
					pstr = NStrMove_AfterLabel(pstr,"=");
					s16 = -1;
					pstr = NStrGet_Ns16(pstr,&s16);
					pstr = NStrMove_AfterLabel(pstr,";");
					NErrorIf(s16>255 || s16<0, NERROR_VALUE_OUTOFRANGE);
					
					if( (wordid == 0) || (wordid == 1 && bnewtx) )
						ptex->BankID = (Nu8)s16;
*/
				}
				// +WRAP_S,+WRAP_T and WRAP_S,WRAP_T
				else if( wordid < 6 )
				{
					pstr = NStrMove_AfterLabel(pstr,"=");
					pstr = NStrGet_String_BeforeLabel(pstr,";",strb);
					pstr = NStrMove_AfterLabel(pstr,";");
					if(_read_wrap_glint_define(strb,&glparam))
					{
						if( (wordid == 2) || (wordid == 3 && bnewtx) )
						{
							ptex->WrapS = glparam;
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glparam );
						}
						else if( (wordid == 4) || (wordid == 5 && bnewtx) )
						{
							ptex->WrapT = glparam;
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glparam );
						}
					}
				}
				// +MIN_FILTER,+MAG_FILTER and MIN_FILTER,MAG_FILTER
				else if( wordid < 10 )
				{
					pstr = NStrMove_AfterLabel(pstr,"=");
					pstr = NStrGet_String_BeforeLabel(pstr,";",strb);
					pstr = NStrMove_AfterLabel(pstr,";");
					if(_read_filter_glint_define(strb,&glparam))
					{
						if( (wordid == 6) || (wordid == 7 && bnewtx) )
						{
							ptex->MinFilter = glparam;
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glparam );
						}
						else if( (wordid == 8) || (wordid == 9 && bnewtx) )
						{
							ptex->MagFilter = glparam;
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glparam );
						}
					}
				}
				// NVOID or other unexpected value
				else
				{
					NErrorIf(1,NERROR_BATCHLOAD_TEXTURES_SYNTAX_ERROR); // Unknown key name readed inside the batch file.
				}
			}

			// Call back handle:
			if(callback)
				callback(ptex,param);
		}
		else
		{
			break;
		}
	}
	NFileClose(pfile);
}

void NBatchLoadBlends(const Nchar *pbatchfilename, const NBATCHLOADBLENDS_CALLBACK callback, Nu32 Nu32_param)
{
	//Check file description extension
	NErrorIf(!NCheckFileType(pbatchfilename,NBATCHLOAD_BLENDS_FILE_EXTENSION),NERROR_FILE_INVALID_EXTENSION);

	NFILE		*pfile;
	Nchar		tempstring[256];
	NBLEND		*pblend;
	Nu32		l;

	//open and check
	pfile = NFileOpen(pbatchfilename,FLAG_NFILE_READ);
	if(!pfile)
		return;

	NFileSeek(pfile,0,NFILE_SEEK_SET);

	tempstring[0]=0;
	while (NFileGetString(tempstring, 256, pfile)) 
	{
		if( ( l=strlen(tempstring) ) )
		{
			l--;
			while( (tempstring[l]=='\n')||(tempstring[l]=='\r'))
			{
				tempstring[l]=0; // to remove the potential end line characters (one or two)
				l--;
			}
		}

		if( strlen(tempstring) )
		{
#ifdef _NLIBS	
			pblend = (NBLEND*)NLookupOAHashMapEntry(&BlendsLibrary,tempstring);
			if(!pblend) // To avoid replacing pre-existing blend with the same key
			{
				//				NErrorIf(NIsRootRelativePathFileName(tempstring)!=NIsRootRelativeDefaultPathSystem(),NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY );
				pblend = NCreateBlendFromFile(tempstring,tempstring);
				NErrorIf(!pblend,NERROR_NULL_POINTER);
				NInsertOAHashMapEntry(&BlendsLibrary,pblend->pName,pblend);
			}
#endif
#ifndef _NLIBS	
			pblend = NCreateBlendFromFile(tempstring,tempstring);
			NErrorIf(!pblend,NERROR_NULL_POINTER);
#endif
			// Call back handle:
			if(callback)
				callback(pblend,Nu32_param);
		}
		else
		{
			break;
		}
	}
	NFileClose(pfile);
}

void NBatchLoadMaterials(const Nchar *pbatchfilename,const NBATCHLOADMATERIALS_CALLBACK callback, Nu32 Nu32_param)
{
	//Check file description extension
	NErrorIf(!NCheckFileType(pbatchfilename,NBATCHLOAD_MATERIALS_FILE_EXTENSION),NERROR_FILE_INVALID_EXTENSION);

	NFILE		*pfile;
	NMATERIAL	*pmat;
	Nchar		tempstring[256];
	Nu32		l;

	//open and check
	pfile = NFileOpen(pbatchfilename,FLAG_NFILE_READ);
	if(!pfile)
		return;

	NFileSeek(pfile,0,NFILE_SEEK_SET);

	tempstring[0]=0;
	while ( NFileGetString(tempstring, 256, pfile) )
	{
		if( ( l=strlen(tempstring) ) )
		{
			l--;
			while( (tempstring[l]=='\n')||(tempstring[l]=='\r'))
			{
				tempstring[l]=0; // to remove the potential end line characters (one or two)
				l--;
			}
		}

		if( strlen(tempstring) )
		{
#ifdef _NLIBS	
			pmat = (NMATERIAL*)NLookupOAHashMapEntry(&MaterialsLibrary,tempstring);
			if( !pmat ) // To avoid replacing pre-existing Material with the same key
			{
				// NErrorIf(NIsRootRelativePathFileName(tempstring)!=NIsRootRelativeDefaultPathSystem(),NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY );
				pmat = NCreateMaterialFromFile(tempstring,tempstring);
				NErrorIf(!pmat,NERROR_NULL_POINTER);
				NInsertOAHashMapEntry(&MaterialsLibrary,pmat->pName,pmat);
			}
#endif
#ifndef _NLIBS	
			pmat = NCreateMaterialFromFile(tempstring,tempstring);
			NErrorIf(!pmat,NERROR_NULL_POINTER);
#endif
			// Call back handle:
			if(callback)
				callback(pmat,Nu32_param);
		}
		else
		{
			break;
		}
	}

	NFileClose(pfile);
}