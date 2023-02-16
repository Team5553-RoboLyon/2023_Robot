#include "../NCStandard.h"
#include "../NType.h"
#include "../NErrorHandling.h"
#include "../Core/NSafeConversion.h"
#include "../Core/NVersion.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../File/NFileExt_Appearance_RWCallBacks.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + ABOUT WRITE/READ APPEARANCE
// +
// +	Appearance is a part of 'Ngeometry' (mostly). It contains several pointers:
// +		-1 pointer on Blend parameters
// +		-1 pointer on Material parameters
// +		-n pointer(s) on Texture(s) 
// +		-n pointer(s) on UVMatrixAnim(s)
// +	There is always 1 Blend and 1 Material pointer into an appearance, BUT the number of texture units
// +	is variable (1 texture unit contains one texture pointer and possibly one uvmatrix pointer ).
// +
// +	UVMatrix Anim is a little bit special... even if is a referenced element it is not considered like an external element
// +	usable by several different appearance. So it is fully managed by appearance itself. So if you delete an appearance,
// +	all the  UVMatrix Anim linked with are going to be delete too. If you write/read an appearance structure, all the UVMatrix linked with
// +	are going to be write/read like any other INTERNAL parts of the appearance structure.
// +	It's not the case for the 3 others referenced element.	They are EXTERNAL, can be share between several different appearances.
// +
// +	These 3 other referenced elements (blend, material, texture) can be store and managed in different ways,
// +	trough 'NUT libraries' or in some other user ways. For that reason, writing an appearance structure
// +	imply using several callbacks to write/read these referenced element.
// +	By default, 'void' call backs are bind with the write/read system, that means these elements are not going to be write/read,
// +	but, if the _NLIBS compiler tag is define, then "_NLIBS" write/read reference callbacks become the default ones.
// +	To force a specific set of write/read callbacks use:
// +			void NSetCurrentRWAppearanceIDS(const Nu32 ids) where ids is made with 'NMAKE_RWAPPEARANCE_IDS(t,m,b)'  with textureID, materialID, blendID
// +			void NSetCurrentRWAppearance_Texture_ID(const Nu8 id);
// +			void NSetCurrentRWAppearance_Material_ID(const Nu8 id);
// +			void NSetCurrentRWAppearance_Blend_ID(const Nu8 id);
// +	( Understand IDs as "CallBack IDs" into the "Read/Write Appearance LUTS" ) 			
// +	There are some predefined ID for each of the 3 external element and it's possible to bind user callbacks and active them by using
// +	some specific range of user IDs. (check 'NFileExt_Appearance_RWCallBacks.h')
// +		
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Internal call back functions ptr used to save/load appearances components
static Nu32								RWAppearance_IDs;
static NRW_APPEARANCE_TEXTURE			RWAppearance_Texture[CONSTANT_RWAPPEARANCE_LUT_SIZE];
static NRW_APPEARANCE_BLEND				RWAppearance_Blend[CONSTANT_RWAPPEARANCE_LUT_SIZE];
static NRW_APPEARANCE_MATERIAL			RWAppearance_Material[CONSTANT_RWAPPEARANCE_LUT_SIZE];
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void NInitRWAppearanceCallBacks()
{
#ifdef _NLIBS
	RWAppearance_IDs = DEFAULT_NUTLIBS_RWAPPEARANCE_IDS;
#endif	
#ifndef _NLIBS
	RWAppearance_IDs = DEFAULT_RWAPPEARANCE_IDS;
#endif
	// Void
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_VOID].pWrite			= NWriteAppearanceTexture_Void;
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_VOID].pRead			= NReadAppearanceTexture_Void;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_VOID].pWrite				= NWriteAppearanceBlend_Void;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_VOID].pRead				= NReadAppearanceBlend_Void;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_VOID].pWrite		= NWriteAppearanceMaterial_Void;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_VOID].pRead			= NReadAppearanceMaterial_Void;

	// Data
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_DATA].pWrite			= NWriteAppearanceTexture_ImageData;
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_DATA].pRead			= NReadAppearanceTexture_ImageData;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_DATA].pWrite				= NWriteAppearanceBlend_Data;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_DATA].pRead				= NReadAppearanceBlend_Data;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_DATA].pWrite		= NWriteAppearanceMaterial_Data;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_DATA].pRead			= NReadAppearanceMaterial_Data;

	// Ref
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_NUTLIBS_REF].pWrite	= NWriteAppearanceTexture_NUTLibrary_Reference;
	RWAppearance_Texture[RWAPPEARANCE_TEXTURE_NUTLIBS_REF].pRead	= NReadAppearanceTexture_NUTLibrary_Reference;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_NUTLIBS_REF].pWrite		= NWriteAppearanceBlend_NUTLibrary_Reference;
	RWAppearance_Blend[RWAPPEARANCE_BLEND_NUTLIBS_REF].pRead		= NReadAppearanceBlend_NUTLibrary_Reference;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_NUTLIBS_REF].pWrite	= NWriteAppearanceMaterial_NUTLibrary_Reference;
	RWAppearance_Material[RWAPPEARANCE_MATERIAL_NUTLIBS_REF].pRead	= NReadAppearanceMaterial_NUTLibrary_Reference;
}


void NSetCurrentRWAppearanceIDS(const Nu32 ids)
{
	NErrorIf( NGET_RWAPPEARANCE_IDS_TEXTURE(ids)		>= CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( NGET_RWAPPEARANCE_IDS_MATERIAL(ids)		>= CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( NGET_RWAPPEARANCE_IDS_BLEND(ids)			>= CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );

	RWAppearance_IDs = ids;
}

void NSetCurrentRWAppearance_Texture_ID(const Nu8 id)
{
	NErrorIf( id >=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	RWAppearance_IDs = (RWAppearance_IDs&0x00FFFFFF)|((id&0xFF)<<24);	
}

void NSetCurrentRWAppearance_Material_ID(const Nu8 id)
{
	NErrorIf( id >=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	RWAppearance_IDs = (RWAppearance_IDs&0xFFFF00FF)|((id&0xFF)<<8);	
}
void NSetCurrentRWAppearance_Blend_ID(const Nu8 id)
{
	NErrorIf( id >=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	RWAppearance_IDs = (RWAppearance_IDs&0xFFFFFF00)|(id&0xFF);	
}

void NBindRWAppearanceTextureCallBacks(const Nu8 id, const NAPPEARANCE_WRITETEXTURE_CALLBACK pfctwrite,const NAPPEARANCE_READTEXTURE_CALLBACK pfctread)
{
	NErrorIf( !pfctwrite || !pfctread,NERROR_NULL_POINTER );
	NErrorIf( id>=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( id<CONSTANT_RWAPPEARANCE_FIRST_USERID,NERROR_VALUE_OUTOFRANGE );

	RWAppearance_Texture[id].pWrite	= (NAPPEARANCE_WRITETEXTURE_CALLBACK)pfctwrite;
	RWAppearance_Texture[id].pRead	= (NAPPEARANCE_READTEXTURE_CALLBACK)pfctread;
}

void NBindRWAppearanceBlendCallBacks(const Nu8 id, const NAPPEARANCE_WRITEBLEND_CALLBACK pfctwrite, const NAPPEARANCE_READBLEND_CALLBACK pfctread)
{
	NErrorIf( !pfctwrite || !pfctread,NERROR_NULL_POINTER );
	NErrorIf( id>=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( id<CONSTANT_RWAPPEARANCE_FIRST_USERID,NERROR_VALUE_OUTOFRANGE );

	RWAppearance_Blend[id].pWrite	= (NAPPEARANCE_WRITEBLEND_CALLBACK)pfctwrite;
	RWAppearance_Blend[id].pRead	= (NAPPEARANCE_READBLEND_CALLBACK)pfctread;
}
void NBindRWAppearanceMaterialCallBacks(const Nu8 id, const NAPPEARANCE_WRITEMATERIAL_CALLBACK pfctwrite, const NAPPEARANCE_READMATERIAL_CALLBACK pfctread)
{
	NErrorIf( !pfctwrite || !pfctread,NERROR_NULL_POINTER );
	NErrorIf( id>=CONSTANT_RWAPPEARANCE_LUT_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( id<CONSTANT_RWAPPEARANCE_FIRST_USERID,NERROR_VALUE_OUTOFRANGE );

	RWAppearance_Material[id].pWrite	= (NAPPEARANCE_WRITEMATERIAL_CALLBACK)pfctwrite;
	RWAppearance_Material[id].pRead		= (NAPPEARANCE_READMATERIAL_CALLBACK)pfctread;
}

Nu32 NFileWriteAppearanceUVMatrixAnim(NFILE *pfile,const NUVMATRIXANIM *ptr)
{
	// To do or TO REDO into an another way .... 
	return 0;
}
Nu32	NFileReadAppearanceUVMatrixAnim(NFILE *pfile, NUVMATRIXANIM** pptr)
{

	return 0;
}


static inline void _set_header_appearance_NULL(NHEADER_APPEARANCE *pheader)
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	pheader->Flags						= 0;
	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_NULL;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_NULL;
	pheader->TextureUnitNb				= 0;	
	pheader->LastUsedTextureUnitID		= 0;
	pheader->TextureUnitStructureSize	= 0;
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
}

static inline void _set_header_appearance_TT(NHEADER_APPEARANCE *pheader, const NAPPEARANCE_TT *papp )
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	if(papp->pMaterial)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL);
	if(papp->pBlend)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_BLEND);

	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_TT;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_T;
	pheader->TextureUnitStructureSize	= _SafeNu32ToNu16(sizeof(NTEXTUREUNIT_T));
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
	
	pheader->TextureUnitNb				= 2;	
	for(Nu8 i=0;i<pheader->TextureUnitNb;i++)
	{
		if( papp->TextureUnitT[i].pTexture )
		{
			pheader->LastUsedTextureUnitID = i;
		}
	}
}
static inline void _set_header_appearance_TM(NHEADER_APPEARANCE *pheader, const NAPPEARANCE_TM *papp )
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	if(papp->pMaterial)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL);
	if(papp->pBlend)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_BLEND);
	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_TM;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_TM;
	pheader->TextureUnitStructureSize	= _SafeNu32ToNu16(sizeof(NTEXTUREUNIT_TM));
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
	pheader->TextureUnitNb				= 1;
	pheader->LastUsedTextureUnitID		= 0;
}
static inline void _set_header_appearance_ULTD_T(NHEADER_APPEARANCE *pheader, const NAPPEARANCE_ULTD_T *papp )
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	if(papp->pMaterial)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL);
	if(papp->pBlend)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_BLEND);
	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_ULTD_T;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_T;
	pheader->TextureUnitStructureSize	= _SafeNu32ToNu16(sizeof(NTEXTUREUNIT_T));
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
	pheader->TextureUnitNb				= papp->TextureUnitTNumber;
	
	// Parsing all the available Texture Unit until the last one to be sure catching the last one with something inside.
	NTEXTUREUNIT_T *ptut = papp->pTextureUnitT;
	for(Nu8 i=0;i<pheader->TextureUnitNb;i++,ptut++)
	{
		if( ptut->pTexture )
		{
			pheader->LastUsedTextureUnitID = i;
		}
	}
}
static inline void _set_header_appearance_ULTD_TM(NHEADER_APPEARANCE *pheader, const NAPPEARANCE_ULTD_TM *papp )
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	if(papp->pMaterial)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL);
	if(papp->pBlend)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_BLEND);
	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_ULTD_TM;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_TM;
	pheader->TextureUnitStructureSize	= _SafeNu32ToNu16(sizeof(NTEXTUREUNIT_TM));
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
	pheader->TextureUnitNb				= papp->TextureUnitTMNumber;
	
	// Parsing all the available Texture Unit until the last one to be sure catching the last one with something inside.
	NTEXTUREUNIT_TM *ptutm = papp->pTextureUnitTM;
	for(Nu8 i=0;i<pheader->TextureUnitNb;i++,ptutm++)
	{
		if( ptutm->pTexture || ptutm->pUVMatrixAnim )
		{
			pheader->LastUsedTextureUnitID = i;
		}
	}
}
static inline void _set_header_appearance_Raw(NHEADER_APPEARANCE *pheader, const NAPPEARANCE_RAW *papp )
{
	Nmem0(pheader,NHEADER_APPEARANCE);
	if(papp->pMaterial)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL);
	if(papp->pBlend)
		FLAG_ON(pheader->Flags,FLAG_NHEADER_APPEARANCE_BLEND);
	pheader->RWAppearanceFormat			= NHEADER_APPEARANCE_RW_FORMAT_RAW;
	pheader->TextureUnitFormat			= (Nu8)NTEXTUREUNIT_FORMAT_TM;
	pheader->TextureUnitStructureSize	= sizeof(NTEXTUREUNIT_TM);
	pheader->RWAppearance_IDs			= RWAppearance_IDs;
	pheader->TextureUnitNb				= CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;
	
	NTEXTUREUNIT_TM *ptutm = (NTEXTUREUNIT_TM*)&papp->TextureUnit[0];
	for(Nu8 i=0;i<pheader->TextureUnitNb;i++,ptutm++)
	{
		if( ptutm->pTexture || ptutm->pUVMatrixAnim )
		{
			pheader->LastUsedTextureUnitID = i;
		}
	}
}
static inline Nu32 _write_textureunits_t(NFILE *pfile,const NTEXTUREUNIT_T *pfirst, const Nu8 tu_nb, const Nu8 tu_lastid)
{
	NErrorIf(tu_nb==0,NERROR_INCONSISTENT_PARAMETERS);	// problem ... tu_nb has to be >=1
	NErrorIf(tu_nb!=0 && tu_lastid>=tu_nb,NERROR_INCONSISTENT_PARAMETERS);
	
	NHEADER_TEXTUREUNIT	head;
	if( NFileWriteVersionTag(pfile,VERSION_NHEADER_TEXTUREUNIT) != 1 ) // of All the Texture Units of this container
		return 0;

	NTEXTUREUNIT_T *ptut = (NTEXTUREUNIT_T*)pfirst;
	for(Nu8 i=0; i<=tu_lastid;i++,ptut++)
	{
		Nmem0(&head,NHEADER_TEXTUREUNIT);
		// "ptut" can be empty but not being the last full one  !
		if(ptut->pTexture)
		{
			FLAG_ON(head.Flags,FLAG_NHEADER_TEXTUREUNIT_TEXTURE);
		}
		
		if( NFileWrite(pfile,&head,sizeof(NHEADER_TEXTUREUNIT),1) != 1 )
			return 0;

		if(ptut->pTexture)
		{
			if( !RWAppearance_Texture[NGET_RWAPPEARANCE_IDS_TEXTURE(RWAppearance_IDs)].pWrite(pfile,ptut->pTexture) )
				return 0;
		}
	}
	return 1;
}

static inline Nu32 _read_textureunits_t(NFILE *pfile,NTEXTUREUNIT_T *pfirst, const Nu8 tu_nb, const Nu8 tu_lastid) 
{
	NErrorIf(tu_nb==0,NERROR_INCONSISTENT_PARAMETERS);	// problem ... tu_nb has to be >=1
	NErrorIf(tu_nb!=0 && tu_lastid>=tu_nb,NERROR_INCONSISTENT_PARAMETERS);

	// Check Incoming version.
	Nu32				version;
	NHEADER_TEXTUREUNIT	header;
	NTEXTUREUNIT_T		*ptut;

	if( NFileReadVersionTag(pfile,&version) != 1)
		return 0;

	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
		case NGETVERSION_MAIN( VERSION_NHEADER_TEXTUREUNIT ):
			ptut = pfirst;
			for(Nu8 i=0; i<tu_nb; i++,ptut++)
			{
				NErrorIf(ptut->pTexture,NERROR_NON_NULL_POINTER);// Targeted texture unit has to be empty !!!
				if(i<=tu_lastid)
				{
					Nmem0(&header,NHEADER_TEXTUREUNIT);
					if(NFileRead(pfile,&header,sizeof(NHEADER_TEXTUREUNIT),1) != 1)
						return 0;

					if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTUREUNIT_TEXTURE))
					{
						if( !RWAppearance_Texture[NGET_RWAPPEARANCE_IDS_TEXTURE(RWAppearance_IDs)].pRead(pfile,&ptut->pTexture) )
							return 0;

						if(ptut->pTexture)
						{
							NRegisterTextureReference(ptut->pTexture);
						}
						#ifdef _DEBUG
						else
						{
							NErrorIf(1,NERROR_SYSTEM_CHECK); // It's not suppose to happen ! That could means texture is missing on storage device !
						}
						#endif
					}
					else
					{
						ptut->pTexture = NULL;
					}
				}
				else
				{
					ptut->pTexture = NULL;
				}
			}
			return 1;

		// Unknown version:
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}
static inline Nu32 _write_textureunits_tm(NFILE *pfile,const NTEXTUREUNIT_TM *pfirst, const Nu8 tu_nb, const Nu8 tu_lastid)
{
	NErrorIf(tu_nb==0,NERROR_INCONSISTENT_PARAMETERS);	// problem ... tu_nb has to be >=1
	NErrorIf(tu_nb!=0 && tu_lastid>=tu_nb,NERROR_INCONSISTENT_PARAMETERS);
	
	if( NFileWriteVersionTag(pfile,VERSION_NHEADER_TEXTUREUNIT) != 1 ) // of All the Texture Units of this container
		return 0;

	NHEADER_TEXTUREUNIT	head;
	NTEXTUREUNIT_TM *ptutm = (NTEXTUREUNIT_TM*)pfirst;
	for(Nu8 i=0; i<=tu_lastid;i++,ptutm++)
	{
		Nmem0(&head,NHEADER_TEXTUREUNIT);
		if(ptutm->pTexture)
		{
			FLAG_ON(head.Flags,FLAG_NHEADER_TEXTUREUNIT_TEXTURE);
		}
		if(ptutm->pUVMatrixAnim)
		{
			FLAG_ON(head.Flags,FLAG_NHEADER_TEXTUREUNIT_UVMATRIXANIM);
		}
		
		if( NFileWrite(pfile,&head,sizeof(NHEADER_TEXTUREUNIT),1) != 1)
			return 0;

		if(ptutm->pTexture)
		{
			if( !RWAppearance_Texture[NGET_RWAPPEARANCE_IDS_TEXTURE(RWAppearance_IDs)].pWrite(pfile,ptutm->pTexture) )
				return 0;
		}

		if(ptutm->pUVMatrixAnim)
		{
			if( ! NFileWriteAppearanceUVMatrixAnim(pfile,ptutm->pUVMatrixAnim) )
				return 0;
		}
	}
	return 1;
}
static inline Nu32 _read_textureunits_tm(NFILE *pfile,NTEXTUREUNIT_TM *pfirst, const Nu8 tu_nb, const Nu8 tu_lastid, const Nbool texture_register) // because NAPPEARANCE_RAW read doesn't register TEXTURE !!!
{
	NErrorIf(tu_nb==0,NERROR_INCONSISTENT_PARAMETERS);	// problem ... tu_nb has to be >=1
	NErrorIf(tu_nb!=0 && tu_lastid>=tu_nb,NERROR_INCONSISTENT_PARAMETERS);

	// Check Incoming version.
	Nu32				version;
	NHEADER_TEXTUREUNIT	header;
	NTEXTUREUNIT_TM		*ptutm;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
		case NGETVERSION_MAIN( VERSION_NHEADER_TEXTUREUNIT ):
			ptutm = pfirst;
			for(Nu8 i=0; i<tu_nb;i++,ptutm++)
			{
				NErrorIf(ptutm->pTexture,NERROR_NON_NULL_POINTER);
				NErrorIf(ptutm->pUVMatrixAnim,NERROR_NON_NULL_POINTER);

				if(i<=tu_lastid)
				{
					Nmem0(&header,NHEADER_TEXTUREUNIT);
					if( NFileRead(pfile,&header,sizeof(NHEADER_TEXTUREUNIT),1) != 1 )
						return 0;

					if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTUREUNIT_TEXTURE))
					{
						if( !RWAppearance_Texture[NGET_RWAPPEARANCE_IDS_TEXTURE(RWAppearance_IDs)].pRead(pfile,&ptutm->pTexture) )
							return 0;

						if(texture_register)
						{
							if(ptutm->pTexture)
							{
								NRegisterTextureReference(ptutm->pTexture);
							}
							#ifdef _DEBUG
							else
							{
								NErrorIf(1,NERROR_SYSTEM_CHECK); // It's not suppose to happen ! That could means texture is missing on storage device !
							}
							#endif
						}
					}
					else
					{
						ptutm->pTexture = NULL;
					}
					if(ISFLAG_ON(header.Flags,FLAG_NHEADER_TEXTUREUNIT_UVMATRIXANIM))
					{
						 if( !NFileReadAppearanceUVMatrixAnim(pfile,&ptutm->pUVMatrixAnim) )
							 return 0;
					}
					else
					{
						ptutm->pUVMatrixAnim = NULL;
					}
				}
				else
				{
					ptutm->pTexture			= NULL;
					ptutm->pUVMatrixAnim	= NULL;
				}
			}
			return 1;

		// Unknown version:
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}
static inline Nu32 _write_blend_and_material(NFILE *pfile, const NHEADER_APPEARANCE *phead, const NAPPEARANCE_COMMON *papp)
{
	if(ISFLAG_ON(phead->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL))
		if( !RWAppearance_Material[NGET_RWAPPEARANCE_IDS_MATERIAL(RWAppearance_IDs)].pWrite(pfile,papp->pMaterial) )
			return 0;

	if(ISFLAG_ON(phead->Flags,FLAG_NHEADER_APPEARANCE_BLEND))
		if( !RWAppearance_Blend[NGET_RWAPPEARANCE_IDS_BLEND(RWAppearance_IDs)].pWrite(pfile,papp->pBlend) )
			return 0;
	
	return 1;
}
static inline Nu32 _read_blend_and_material(NFILE *pfile, const NHEADER_APPEARANCE *phead, NAPPEARANCE_COMMON *papp)
{
	if(ISFLAG_ON(phead->Flags,FLAG_NHEADER_APPEARANCE_MATERIAL))
		 if( !RWAppearance_Material[NGET_RWAPPEARANCE_IDS_MATERIAL(RWAppearance_IDs)].pRead(pfile,&papp->pMaterial) )
			 return 0;

	if(ISFLAG_ON(phead->Flags,FLAG_NHEADER_APPEARANCE_BLEND))
		 if( !RWAppearance_Blend[NGET_RWAPPEARANCE_IDS_BLEND(RWAppearance_IDs)].pRead(pfile,&papp->pBlend) )
			 return 0;

	return 1;
}

Nu32 NFileWriteAppearance_TT(NFILE *pfile,const NAPPEARANCE_TT *papp)
{
	NHEADER_APPEARANCE		header;
	
	_set_header_appearance_TT(&header,papp);

	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1 )
		return 0;

	if(NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
		return 0;

	if( !_write_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )//write if there are ones ("header" keep those infos into its flags )
		return 0;

	if( !_write_textureunits_t(pfile,papp->TextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
		return 0;

	return 1;
}

Nu32 NFileWriteAppearance_TM(NFILE *pfile,const NAPPEARANCE_TM *papp)
{
	NHEADER_APPEARANCE		header;
	
	_set_header_appearance_TM(&header,papp);

	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1 )
		return 0;

	if( NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
		return 0;

	if( !_write_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )//write if there are ones ("header" keep those infos into its flags )
		return 0;

	if( !_write_textureunits_tm(pfile,&papp->TextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
		return 0;
	
	return 1;
}
Nu32 NFileWriteAppearance_ULTD_T(NFILE *pfile,const NAPPEARANCE_ULTD_T *papp)
{
	NHEADER_APPEARANCE		header;
	
	_set_header_appearance_ULTD_T(&header,papp);

	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1 )
		return 0;

	if( NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
		return 0;

	if( !_write_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )//write if there are ones ("header" keep those infos into its flags )
		return 0;

	if( !_write_textureunits_t(pfile,papp->pTextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
		return 0;

	return 1;
}

Nu32 NFileWriteAppearance_ULTD_TM(NFILE *pfile,const NAPPEARANCE_ULTD_TM *papp)
{
	NHEADER_APPEARANCE		header;
	
	_set_header_appearance_ULTD_TM(&header,papp);

	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1)
		return 0;

	if( NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
		return 0;

	if( !_write_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )//write if there are ones ("header" keep those infos into its flags )
		return 0;
	
	if( !_write_textureunits_tm(pfile,papp->pTextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
		return 0;

	return 1;
}

static inline Nu32 NFileWriteAppearance_NULL(NFILE *pfile)
{
	NHEADER_APPEARANCE		header;
	
	_set_header_appearance_NULL(&header);
	
	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1)
		return 0;
	
	if( NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1)
		return 0;

	return 1;
}

Nu32 NFileWriteAppearance(NFILE *pfile,const NAPPEARANCE *papp,const NAPPEARANCE_FORMAT_ENUM app_format)
{
	NErrorIf(!pfile || !papp, NERROR_NULL_POINTER);
	NErrorIf(app_format >= NAPPEARANCE_FORMAT_ENUM_SIZE,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
	
	switch(app_format)
	{
		case NAPPEARANCE_FORMAT_NULL:
			return NFileWriteAppearance_NULL(pfile);

		case NAPPEARANCE_FORMAT_TT:
			return NFileWriteAppearance_TT(pfile,(NAPPEARANCE_TT*)papp);

		case NAPPEARANCE_FORMAT_TM:
			return NFileWriteAppearance_TM(pfile,(NAPPEARANCE_TM*)papp);

		case NAPPEARANCE_FORMAT_ULTD_T:
			return NFileWriteAppearance_ULTD_T(pfile,(NAPPEARANCE_ULTD_T*)papp);
		
		case NAPPEARANCE_FORMAT_ULTD_TM:
			return NFileWriteAppearance_ULTD_TM(pfile,(NAPPEARANCE_ULTD_TM*)papp);
		
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			return 0;
	}
}

Nu32 NFileReadAppearance_TT(NFILE *pfile,NAPPEARANCE_TT *papp)
{
	NMemoryCheck(papp,sizeof(NAPPEARANCE_TT),0);

	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
				return 0;

			NErrorIf(header.RWAppearanceFormat!=NHEADER_APPEARANCE_RW_FORMAT_TT,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			NErrorIf(header.TextureUnitNb!=2,NERROR_WRONG_VALUE);

			if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
				return 0;

			if( !_read_textureunits_t(pfile,papp->TextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
				return 0;

			return 1;
			
		// Unknown version
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}

Nu32 NFileReadAppearance_TM(NFILE *pfile,NAPPEARANCE_TM *papp)
{
	NMemoryCheck(papp,sizeof(NAPPEARANCE_TM),0);

	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 0 )
				return 0;

			NErrorIf(header.RWAppearanceFormat!=NHEADER_APPEARANCE_RW_FORMAT_TM,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			NErrorIf(header.TextureUnitNb!=1,NERROR_WRONG_VALUE);
			
			if(!_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp))
				return 0;
			if(!_read_textureunits_tm(pfile,&papp->TextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID,NTRUE ))
				return 0;

			return 1;
		
		// Unknown version
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}

Nu32 NFileReadAppearance_ULTD_T(NFILE *pfile,NAPPEARANCE_ULTD_T *papp)
{
	NMemoryCheck(papp,sizeof(NAPPEARANCE_ULTD_T),0);

	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
				return 0;

			NErrorIf(header.RWAppearanceFormat!=NHEADER_APPEARANCE_RW_FORMAT_ULTD_T,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			NErrorIf(header.TextureUnitFormat !=(Nu8)NTEXTUREUNIT_FORMAT_T,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_FORMAT);

			NSetupAppearance_ULTD_T(papp,header.TextureUnitNb);
			
			if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
			{
				// NClearAppearance_ULTD_T(papp);
				return 0;
			}
			if( !_read_textureunits_t(pfile,papp->pTextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
			{
				// NClearAppearance_ULTD_T(papp);
				return 0;
			}
			return 1;

		// Unknown version
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}
Nu32 NFileReadAppearance_ULTD_TM(NFILE *pfile,NAPPEARANCE_ULTD_TM *papp)
{
	NMemoryCheck(papp,sizeof(NAPPEARANCE_ULTD_TM),0);

	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
				return 0;

			NErrorIf(header.RWAppearanceFormat!=NHEADER_APPEARANCE_RW_FORMAT_ULTD_TM,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			NErrorIf(header.TextureUnitFormat !=(Nu8)NTEXTUREUNIT_FORMAT_TM,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_FORMAT);
			
			NSetupAppearance_ULTD_TM(papp,header.TextureUnitNb);
			
			if(!_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
			{
				// NClearAppearance_ULTD_TM(papp);
				return 0;
			}
			if(!_read_textureunits_tm(pfile,papp->pTextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID,NTRUE ))
			{
				// NClearAppearance_ULTD_TM(papp);
				return 0;
			}
			return 1;

		// Unknown version
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}
Nu32 NFileReadAppearance(NFILE *pfile,NAPPEARANCE *papp, NAPPEARANCE_FORMAT_ENUM *papp_format)
{
	// Incoming "papp" must be clear (full of ZERO). What ever its specific type ( TT,TM, ULTD ... )
	// Because, NAPPEARANCE is an union, all the different appearance types use the same memory space of 4 32 bits.
	// So, lets check those 4 32 bits are effectively ZEROs before loading. But doing that only in DEBUG mode
	// because this potential Error is a logical error caused by programming mistakes ...
	NMemoryCheck(papp,sizeof(NAPPEARANCE_ULTD_TM),0);
	
	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return NULL;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
				return 0;

			switch( header.RWAppearanceFormat )
			{
				case NHEADER_APPEARANCE_RW_FORMAT_NULL:
					// Its possible ! And its not an error ...
					//NErrorIf( 1,NERROR_FILE_UNEXPECTED_READ_VALUE ); 
					*papp_format = NAPPEARANCE_FORMAT_NULL;
					return 1;

				case NHEADER_APPEARANCE_RW_FORMAT_TT:
					NErrorIf(header.TextureUnitNb!=2,NERROR_WRONG_VALUE);
					if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
						return 0;
					if( !_read_textureunits_t(pfile,papp->TT.TextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ))
						return 0;
					*papp_format = NAPPEARANCE_FORMAT_TT;
					return 1;

				case NHEADER_APPEARANCE_RW_FORMAT_TM:
					NErrorIf(header.TextureUnitNb!=1,NERROR_WRONG_VALUE);
					if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
						return 0;
					if( !_read_textureunits_tm(pfile,&papp->TM.TextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID,NTRUE ) )
						return 0;
					*papp_format = NAPPEARANCE_FORMAT_TM;
					return 1;

				case NHEADER_APPEARANCE_RW_FORMAT_ULTD_T:
					NErrorIf(header.TextureUnitFormat !=(Nu8)NTEXTUREUNIT_FORMAT_T,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_FORMAT);
					NSetupAppearance_ULTD_T((NAPPEARANCE_ULTD_T*)papp,header.TextureUnitNb);
					if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
					{
						//NClearAppearance_ULTD_T((NAPPEARANCE_ULTD_T*)papp);
						return 0;
					}
					if( !_read_textureunits_t(pfile,papp->ULTD_T.pTextureUnitT,header.TextureUnitNb,header.LastUsedTextureUnitID ) )
					{
						//NClearAppearance_ULTD_T((NAPPEARANCE_ULTD_T*)papp);
						return 0;
					}

					*papp_format = NAPPEARANCE_FORMAT_ULTD_T;
					return 1;

				case NHEADER_APPEARANCE_RW_FORMAT_ULTD_TM:
					NErrorIf(header.TextureUnitFormat !=(Nu8)NTEXTUREUNIT_FORMAT_TM,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_FORMAT);
					NSetupAppearance_ULTD_TM(&papp->ULTD_TM,header.TextureUnitNb);
					if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
					{
						//NClearAppearance_ULTD_TM((NAPPEARANCE_ULTD_TM*)papp);
						return 0;
					}
					if( !_read_textureunits_tm(pfile,papp->ULTD_TM.pTextureUnitTM,header.TextureUnitNb,header.LastUsedTextureUnitID,NTRUE ) )
					{
						//NClearAppearance_ULTD_TM((NAPPEARANCE_ULTD_TM*)papp);
						return 0;
					}

					*papp_format = NAPPEARANCE_FORMAT_ULTD_TM;
					return 1;

				case NHEADER_APPEARANCE_RW_FORMAT_RAW:
					// !!! USE "NFileReadAppearanceRaw" TO READ A NAPPEARANCE_RAW !!!
					NErrorIf(1,NERROR_FILE_UNEXPECTED_READ_VALUE);
					return 0;

				default:
					NErrorIf(1,NERROR_FILE_UNEXPECTED_READ_VALUE);
					return 0;

			}//switch( header.RWAppearanceFormat )
			break;

			// Unknown version
			default:
				NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
				return 0;
	}//	switch( NGETVERSION_MAIN(version))

//	return 0;
}

Nu32 NFileWriteAppearance_Raw( NFILE *pfile,const NAPPEARANCE_RAW *papp )
{
	NHEADER_APPEARANCE			header;
	_set_header_appearance_Raw(&header,papp );

	// Write Current version Tag
	if( NFileWriteVersionTag( pfile, VERSION_NHEADER_APPEARANCE ) != 1 )
		return 0;

	if( NFileWrite(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
		return 0;

	if( !_write_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp))
		return 0;

	if( !_write_textureunits_tm(pfile,&papp->TextureUnit[0],header.TextureUnitNb,header.LastUsedTextureUnitID))
		return 0;

	return 1;
}
Nu32 NFileReadAppearance_Raw( NFILE *pfile,NAPPEARANCE_RAW *papp )
{
	NMemoryCheck(papp,sizeof(NAPPEARANCE_RAW),0);

	NHEADER_APPEARANCE		header;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version))
	{
		// Current Main Version (all versions 0.0.x )
		case NGETVERSION_MAIN( VERSION_NHEADER_APPEARANCE ):
			if( NFileRead(pfile,&header,sizeof(NHEADER_APPEARANCE),1) != 1 )
				return 0;
			
			// Appearance Raw TextureUnits Number is fixed by engine at 'CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER'
			// So we can imagine a future version of the engine with a different value for this constant.
			// To avoid loosing some loading data due to a new smaller constant, it's better to perform some tests and make a CRASH.
			NErrorIf(header.RWAppearanceFormat!=NHEADER_APPEARANCE_RW_FORMAT_RAW,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			NErrorIf(header.TextureUnitNb > CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER, NERROR_SYSTEM_NENGINEVERSION_CONFLICT); 
			NErrorIf(header.LastUsedTextureUnitID >= CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER, NERROR_SYSTEM_NENGINEVERSION_CONFLICT); 

		
			if( !_read_blend_and_material(pfile,&header,(NAPPEARANCE_COMMON*)papp) )
				return 0;

			if( !_read_textureunits_tm(pfile,&papp->TextureUnit[0],header.TextureUnitNb,header.LastUsedTextureUnitID,NFALSE) ) // NFALSE, because there is no Texture registration for Appearance RAW !!!
				return 0;
			
			return 1;

		// Unknown version
		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}

