#include "NGeometryAppearance.h"


// ------------------------------------------------------------------------------------------
// NSetupAppearance_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_TT*	NSetupAppearance_TT( NAPPEARANCE_TT *papp)
{
	Nmem0(papp,NAPPEARANCE_TT);
	return papp;
}
// ------------------------------------------------------------------------------------------
// NCreateAppearance_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_TT* NCreateAppearance_TT()
{
	return NSetupAppearance_TT(NEW(NAPPEARANCE_TT));
}
// ------------------------------------------------------------------------------------------
// NClearAppearance_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearAppearance_TT(NAPPEARANCE_TT *papp)
{
	if(papp->TextureUnitT0.pTexture)
		NUnregisterTextureReference(papp->TextureUnitT0.pTexture);
	
	if(papp->TextureUnitT1.pTexture)
		NUnregisterTextureReference(papp->TextureUnitT1.pTexture);

	Nmem0(papp,NAPPEARANCE_TT);
}
// ------------------------------------------------------------------------------------------
// NDeleteAppearance_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteAppearance_TT(NAPPEARANCE_TT *ptr)
{
	NClearAppearance_TT(ptr);
	Nfree(ptr);
}
// ------------------------------------------------------------------------------------------
// NCopyAppearance_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NCopyAppearance_TT(NAPPEARANCE_TT *pdst,const NAPPEARANCE_TT *psrc)
{
	if(pdst->TextureUnitT0.pTexture)
		NUnregisterTextureReference(pdst->TextureUnitT0.pTexture);

	if(pdst->TextureUnitT1.pTexture)
		NUnregisterTextureReference(pdst->TextureUnitT1.pTexture);

	*pdst = *psrc;

	if(pdst->TextureUnitT0.pTexture)
		NRegisterTextureReference(pdst->TextureUnitT0.pTexture);

	if(pdst->TextureUnitT1.pTexture)
		NRegisterTextureReference(pdst->TextureUnitT1.pTexture);
}
// ------------------------------------------------------------------------------------------
// NSetupAppearance_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_TM*	NSetupAppearance_TM( NAPPEARANCE_TM *papp)
{
	Nmem0(papp,NAPPEARANCE_TM);
	return papp;
}
// ------------------------------------------------------------------------------------------
// NCreateAppearance_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_TM* NCreateAppearance_TM()
{
	return NSetupAppearance_TM(NEW(NAPPEARANCE_TM));
}
// ------------------------------------------------------------------------------------------
// NClearAppearance_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearAppearance_TM(NAPPEARANCE_TM *papp)
{
	if(papp->TextureUnitTM.pTexture)
		NUnregisterTextureReference(papp->TextureUnitTM.pTexture);

	Nmem0(papp,NAPPEARANCE_TM);
}
// ------------------------------------------------------------------------------------------
// NDeleteAppearance_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteAppearance_TM(NAPPEARANCE_TM *ptr)
{
	NClearAppearance_TM(ptr);
	Nfree(ptr);
}
// ------------------------------------------------------------------------------------------
// NCopyAppearance_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NCopyAppearance_TM(NAPPEARANCE_TM *pdst,const NAPPEARANCE_TM *psrc)
{
	if(pdst->TextureUnitTM.pTexture)
		NUnregisterTextureReference(pdst->TextureUnitTM.pTexture);

	*pdst = *psrc;

	if(pdst->TextureUnitTM.pTexture)
		NRegisterTextureReference(pdst->TextureUnitTM.pTexture);
}
// ------------------------------------------------------------------------------------------
// NSetupAppearance_ULTD_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_ULTD_T*	NSetupAppearance_ULTD_T( NAPPEARANCE_ULTD_T *papp,const Nu8 textureunitnb)
{
	papp->pBlend			 = NULL;
	papp->pMaterial			 = NULL;
	papp->pTextureUnitT		 = (NTEXTUREUNIT_T*)Nmalloc_mem0(sizeof(NTEXTUREUNIT_T)*textureunitnb);
	papp->TextureUnitTNumber = textureunitnb;
	return papp;
}
// ------------------------------------------------------------------------------------------
// NCreateGeometryAppearance_ULTD_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_ULTD_T* NCreateAppearance_ULTD_T(const Nu8 textureunitnb)
{
	return NSetupAppearance_ULTD_T( NEW(NAPPEARANCE_ULTD_T), textureunitnb );
}
// ------------------------------------------------------------------------------------------
// NClearGeometryAppearance_ULTD_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearAppearance_ULTD_T(NAPPEARANCE_ULTD_T *ptr)
{
	for(Nu8 i=0;i<ptr->TextureUnitTNumber;i++)
	{
		if(ptr->pTextureUnitT[i].pTexture)
			NUnregisterTextureReference(ptr->pTextureUnitT[i].pTexture);
	}
	Nfree(ptr->pTextureUnitT);
	Nmem0(ptr,NAPPEARANCE_ULTD_T);
}

// ------------------------------------------------------------------------------------------
// NDeleteGeometryAppearance_ULTD_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteAppearance_ULTD_T(NAPPEARANCE_ULTD_T *ptr)
{
	NClearAppearance_ULTD_T(ptr);
	Nfree(ptr);
}
// ------------------------------------------------------------------------------------------
// NCopyGeometryAppearance_Full
// ------------------------------------------------------------------------------------------
// Description :
//	Copy a geometry Mesh into an another one !
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NCopyAppearance_ULTD_T(NAPPEARANCE_ULTD_T *pdst, const NAPPEARANCE_ULTD_T *psrc)
{
	Nu8 i;

	pdst->pBlend		= psrc->pBlend;
	pdst->pMaterial		= psrc->pMaterial;
	
	for(i=0;i<pdst->TextureUnitTNumber;i++)
	{
		if(pdst->pTextureUnitT[i].pTexture)
			NUnregisterTextureReference(pdst->pTextureUnitT[i].pTexture);
	}

	if(pdst->TextureUnitTNumber == psrc->TextureUnitTNumber)
	{
		memcpy(pdst->pTextureUnitT,psrc->pTextureUnitT,pdst->TextureUnitTNumber);
	}
	else
	{
		pdst->TextureUnitTNumber = psrc->TextureUnitTNumber;
		Nrealloc(pdst->pTextureUnitT,pdst->TextureUnitTNumber);
		memcpy(pdst->pTextureUnitT,psrc->pTextureUnitT,pdst->TextureUnitTNumber);
	}

	for(i=0;i<pdst->TextureUnitTNumber;i++)
	{
		if(pdst->pTextureUnitT[i].pTexture)
			NRegisterTextureReference(pdst->pTextureUnitT[i].pTexture);
	}
}
// ------------------------------------------------------------------------------------------
// NSetupAppearance_ULTD_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_ULTD_TM*	NSetupAppearance_ULTD_TM( NAPPEARANCE_ULTD_TM *papp,const Nu8 textureunitnb)
{
	papp->pBlend				= NULL;
	papp->pMaterial				= NULL;
	papp->pTextureUnitTM		= (NTEXTUREUNIT_TM*)Nmalloc_mem0(sizeof(NTEXTUREUNIT_TM)*textureunitnb);
	papp->TextureUnitTMNumber	= textureunitnb;
	return papp;
}
// ------------------------------------------------------------------------------------------
// NCreateAppearance_ULTD_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_ULTD_TM* NCreateAppearance_ULTD_TM(const Nu8 textureunitnb)
{
	return NSetupAppearance_ULTD_TM( NEW(NAPPEARANCE_ULTD_TM), textureunitnb );
}
// ------------------------------------------------------------------------------------------
// NClearAppearance_ULTD_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *ptr)
{
	for(Nu8 i=0;i<ptr->TextureUnitTMNumber;i++)
	{
		if(ptr->pTextureUnitTM[i].pTexture)
			NUnregisterTextureReference(ptr->pTextureUnitTM[i].pTexture);
	}

	Nfree(ptr->pTextureUnitTM);
	Nmem0(ptr,NAPPEARANCE_ULTD_TM);
}

// ------------------------------------------------------------------------------------------
// NDeleteAppearance_ULTD_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *ptr)
{
	NClearAppearance_ULTD_TM(ptr);
	Nfree(ptr);
}
// ------------------------------------------------------------------------------------------
// NCopyAppearance_ULTD_TM
// ------------------------------------------------------------------------------------------
// Description :
//	Copy an Appearance to an another one !
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NCopyAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *pdst, const NAPPEARANCE_ULTD_TM *psrc)
{
	Nu8 i;

	pdst->pBlend		= psrc->pBlend;
	pdst->pMaterial		= psrc->pMaterial;

	for(i=0;i<pdst->TextureUnitTMNumber;i++)
	{
		if(pdst->pTextureUnitTM[i].pTexture)
			NUnregisterTextureReference(pdst->pTextureUnitTM[i].pTexture);
	}

	if(pdst->TextureUnitTMNumber == psrc->TextureUnitTMNumber)
	{
		memcpy(pdst->pTextureUnitTM,psrc->pTextureUnitTM,pdst->TextureUnitTMNumber);
	}
	else
	{
		pdst->TextureUnitTMNumber = psrc->TextureUnitTMNumber;
		Nrealloc(pdst->pTextureUnitTM,pdst->TextureUnitTMNumber);
		memcpy(pdst->pTextureUnitTM,psrc->pTextureUnitTM,pdst->TextureUnitTMNumber);
	}

	for(i=0;i<pdst->TextureUnitTMNumber;i++)
	{
		if(pdst->pTextureUnitTM[i].pTexture)
			NRegisterTextureReference(pdst->pTextureUnitTM[i].pTexture);
	}
}

NAPPEARANCE_TT* NSetAppearance_TT( NAPPEARANCE_TT *papp, const NAPPEARANCE_RAW *praw)
{
	NErrorIf(!papp,NERROR_NULL_POINTER);

	if(papp->TextureUnitT0.pTexture)
		NUnregisterTextureReference(papp->TextureUnitT0.pTexture);

	if(papp->TextureUnitT1.pTexture)
		NUnregisterTextureReference(papp->TextureUnitT1.pTexture);

	if(!praw)
	{
		papp->TextureUnitT0.pTexture	= NULL;
		papp->TextureUnitT1.pTexture	= NULL;
		papp->pBlend					= NULL;
		papp->pMaterial					= NULL;
	}
	else
	{
		papp->TextureUnitT0.pTexture	= praw->TextureUnit[0].pTexture;
		papp->TextureUnitT1.pTexture	= praw->TextureUnit[1].pTexture;
		papp->pBlend					= praw->pBlend;
		papp->pMaterial					= praw->pMaterial;

		if(papp->TextureUnitT0.pTexture)
			NRegisterTextureReference(papp->TextureUnitT0.pTexture);

		if(papp->TextureUnitT1.pTexture)
			NRegisterTextureReference(papp->TextureUnitT1.pTexture);
	}
	return papp;
}
NAPPEARANCE_TM* NSetAppearance_TM( NAPPEARANCE_TM *papp, const NAPPEARANCE_RAW *praw)
{
	NErrorIf(!papp,NERROR_NULL_POINTER);

	if(papp->TextureUnitTM.pTexture)
		NUnregisterTextureReference(papp->TextureUnitTM.pTexture);

	if(!praw)
	{
		papp->TextureUnitTM.pTexture		= NULL;
		papp->TextureUnitTM.pUVMatrixAnim	= NULL;
		papp->pBlend						= NULL;
		papp->pMaterial						= NULL;
	}
	else
	{
		papp->TextureUnitTM.pTexture		= praw->TextureUnit[0].pTexture;
		papp->TextureUnitTM.pUVMatrixAnim	= praw->TextureUnit[0].pUVMatrixAnim;
		papp->pBlend						= praw->pBlend;
		papp->pMaterial						= praw->pMaterial;
		
		if(papp->TextureUnitTM.pTexture)
			NRegisterTextureReference(papp->TextureUnitTM.pTexture);
	}
	return papp;
}
NAPPEARANCE_ULTD_T* NSetAppearance_ULTD_T( NAPPEARANCE_ULTD_T *papp, const NAPPEARANCE_RAW *praw)
{
	NErrorIf(!papp,NERROR_NULL_POINTER);
	NErrorIf(papp->pTextureUnitT == NULL || papp->TextureUnitTNumber == 0, NERROR_INCONSISTENT_PARAMETERS);
	// Not really an Error, just a warning to SAY : "Hey ! CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER is too low... increase it !
	NErrorIf( papp->TextureUnitTNumber > CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER, NERROR_SYSTEM_GURU_MEDITATION ); 

	Nu8 i;
	NTEXTUREUNIT_T *ptu_t	= papp->pTextureUnitT;
	if(praw)
	{
		papp->pBlend	= praw->pBlend;
		papp->pMaterial	= praw->pMaterial;
		for(i=0;i<papp->TextureUnitTNumber;i++,ptu_t++)
		{
			if(ptu_t->pTexture)
				NUnregisterTextureReference(ptu_t->pTexture);
			
			ptu_t->pTexture = praw->TextureUnit[i].pTexture;
			
			if(ptu_t->pTexture)
				NRegisterTextureReference(ptu_t->pTexture);

			NErrorIf(praw->TextureUnit[i].pUVMatrixAnim != NULL, NERROR_SYSTEM_GURU_MEDITATION ); // Not really an Error but User shouldn't pass a Matrix ptr in that case !
		}
	}
	else
	{
		papp->pBlend	= NULL;
		papp->pMaterial	= NULL;
		for(i=0;i<papp->TextureUnitTNumber;i++,ptu_t++)
		{
			if(ptu_t->pTexture)
				NUnregisterTextureReference(ptu_t->pTexture);

			ptu_t->pTexture = NULL;
		}

	}

	return papp;
}
NAPPEARANCE_ULTD_TM* NSetAppearance_ULTD_TM( NAPPEARANCE_ULTD_TM *papp, const NAPPEARANCE_RAW *praw)
{
	NErrorIf(!papp,NERROR_NULL_POINTER);
	NErrorIf(papp->pTextureUnitTM == NULL || papp->TextureUnitTMNumber == 0, NERROR_INCONSISTENT_PARAMETERS);
	// Not really an Error, just a warning to SAY : "Hey ! CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER is too low... increase it !
	NErrorIf( papp->TextureUnitTMNumber > CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER, NERROR_SYSTEM_GURU_MEDITATION ); 

	Nu8 i;
	NTEXTUREUNIT_TM *ptu_tm	= papp->pTextureUnitTM;
	if(praw)
	{
		papp->pBlend	= praw->pBlend;
		papp->pMaterial	= praw->pMaterial;
		for(i=0;i<papp->TextureUnitTMNumber;i++,ptu_tm++)
		{
			if(ptu_tm->pTexture)
				NUnregisterTextureReference(ptu_tm->pTexture);

			ptu_tm->pTexture		= praw->TextureUnit[i].pTexture;
			
			if(ptu_tm->pTexture)
				NRegisterTextureReference(ptu_tm->pTexture);

			ptu_tm->pUVMatrixAnim	= praw->TextureUnit[i].pUVMatrixAnim;
		}
	}
	else
	{
		papp->pBlend	= NULL;
		papp->pMaterial	= NULL;
		for(i=0;i<papp->TextureUnitTMNumber;i++,ptu_tm++)
		{
			if(ptu_tm->pTexture)
				NUnregisterTextureReference(ptu_tm->pTexture);

			ptu_tm->pTexture		= NULL;
			ptu_tm->pUVMatrixAnim	= NULL;
		}

	}

	return papp;
}

NAPPEARANCE* NSetAppearance( NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE_RAW *praw)
{
	switch( app_format )
	{
		case NAPPEARANCE_FORMAT_NULL:
			NErrorIf(praw, NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED);
			break;
		case NAPPEARANCE_FORMAT_TT:
			NSetAppearance_TT(&papp->TT,praw);
			break;
		case NAPPEARANCE_FORMAT_TM:
			NSetAppearance_TM(&papp->TM,praw);
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NSetAppearance_ULTD_T(&papp->ULTD_T,praw);
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NSetAppearance_ULTD_TM(&papp->ULTD_TM,praw);
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			break;
	}
	return papp;
}

void	NSetAppearance_Texture(NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const Nu8 texture_unit,const NTEXTURE *ptexture)
{
	switch( app_format )
	{
		case NAPPEARANCE_FORMAT_NULL:
			NErrorIf(1, NERROR_GEOMETRY_TEXTURE_NOT_SUPPORTED);
			return;

		case NAPPEARANCE_FORMAT_TT:
			NErrorIf(texture_unit > 1,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
			
			if(papp->TT.TextureUnitT[texture_unit].pTexture)
				NUnregisterTextureReference(papp->TT.TextureUnitT[texture_unit].pTexture);

			papp->TT.TextureUnitT[texture_unit].pTexture = (NTEXTURE*)ptexture;
			
			if(papp->TT.TextureUnitT[texture_unit].pTexture)
				NRegisterTextureReference(papp->TT.TextureUnitT[texture_unit].pTexture);
			return;

		case NAPPEARANCE_FORMAT_TM:
			NErrorIf(texture_unit != 0,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);

			if(papp->TM.TextureUnitTM.pTexture)
				NUnregisterTextureReference(papp->TM.TextureUnitTM.pTexture);
			
			papp->TM.TextureUnitTM.pTexture = (NTEXTURE*)ptexture;

			if(papp->TM.TextureUnitTM.pTexture)
				NRegisterTextureReference(papp->TM.TextureUnitTM.pTexture);
			return;

		case NAPPEARANCE_FORMAT_ULTD_T:
			NErrorIf(texture_unit >= papp->ULTD_T.TextureUnitTNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
			
			if(papp->ULTD_T.pTextureUnitT[texture_unit].pTexture)
				NUnregisterTextureReference(papp->ULTD_T.pTextureUnitT[texture_unit].pTexture);
		
			papp->ULTD_T.pTextureUnitT[texture_unit].pTexture = (NTEXTURE*)ptexture;

			if(papp->ULTD_T.pTextureUnitT[texture_unit].pTexture)
				NRegisterTextureReference(papp->ULTD_T.pTextureUnitT[texture_unit].pTexture);
			return;

		case NAPPEARANCE_FORMAT_ULTD_TM:
			NErrorIf(texture_unit >= papp->ULTD_TM.TextureUnitTMNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);

			if(papp->ULTD_TM.pTextureUnitTM[texture_unit].pTexture)
				NUnregisterTextureReference(papp->ULTD_TM.pTextureUnitTM[texture_unit].pTexture);

			papp->ULTD_TM.pTextureUnitTM[texture_unit].pTexture = (NTEXTURE*)ptexture;
			
			if(papp->ULTD_TM.pTextureUnitTM[texture_unit].pTexture)
				NRegisterTextureReference(papp->ULTD_TM.pTextureUnitTM[texture_unit].pTexture);
			return;

		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			return;
	}
}
void	NSetAppearance_TT_Texture(NAPPEARANCE_TT *papp, const Nu8 texture_unit, const NTEXTURE *ptexture)
{
	NErrorIf(texture_unit > 1,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
	if( papp->TextureUnitT[texture_unit].pTexture )
		NUnregisterTextureReference(papp->TextureUnitT[texture_unit].pTexture);
	
	papp->TextureUnitT[texture_unit].pTexture = (NTEXTURE*)ptexture;
	
	if( papp->TextureUnitT[texture_unit].pTexture )
		NRegisterTextureReference(papp->TextureUnitT[texture_unit].pTexture);

}


void	NSetAppearance_TM_Texture(NAPPEARANCE_TM *papp, const NTEXTURE *ptexture)
{
	if( papp->TextureUnitTM.pTexture )
		NUnregisterTextureReference(papp->TextureUnitTM.pTexture);

	papp->TextureUnitTM.pTexture = (NTEXTURE*)ptexture;
	
	if( papp->TextureUnitTM.pTexture )
		NRegisterTextureReference(papp->TextureUnitTM.pTexture);
}
void	NSetAppearance_ULTD_T_Texture(NAPPEARANCE_ULTD_T *papp, const Nu8 texture_unit, const NTEXTURE *ptexture)
{
	NErrorIf(texture_unit >= papp->TextureUnitTNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);

	if( papp->pTextureUnitT[texture_unit].pTexture )
		NUnregisterTextureReference(papp->pTextureUnitT[texture_unit].pTexture);

	papp->pTextureUnitT[texture_unit].pTexture = (NTEXTURE*)ptexture;

	if( papp->pTextureUnitT[texture_unit].pTexture )
		NRegisterTextureReference(papp->pTextureUnitT[texture_unit].pTexture);
}
void	NSetAppearance_ULTD_TM_Texture(NAPPEARANCE_ULTD_TM *papp, const Nu8 texture_unit, const NTEXTURE *ptexture)
{
	NErrorIf(texture_unit >= papp->TextureUnitTMNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
	
	if( papp->pTextureUnitTM[texture_unit].pTexture )
		NUnregisterTextureReference(papp->pTextureUnitTM[texture_unit].pTexture);

	papp->pTextureUnitTM[texture_unit].pTexture = (NTEXTURE*)ptexture;

	if( papp->pTextureUnitTM[texture_unit].pTexture )
		NRegisterTextureReference(papp->pTextureUnitTM[texture_unit].pTexture);
}

NAPPEARANCE_RAW* NUnpackAppearance_TT(NAPPEARANCE_RAW *praw, const NAPPEARANCE_TT *ptt)
{
	NErrorIf(!praw || !ptt, NERROR_NULL_POINTER);
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<2,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);

	Nmem0(praw,NAPPEARANCE_RAW);
	praw->pBlend						= ptt->pBlend;
	praw->pMaterial						= ptt->pMaterial;
	praw->TextureUnit[0].pTexture		= ptt->TextureUnitT0.pTexture;
	praw->TextureUnit[0].pUVMatrixAnim	= NULL;
	praw->TextureUnit[1].pTexture		= ptt->TextureUnitT1.pTexture;
	praw->TextureUnit[1].pUVMatrixAnim	= NULL;

	return praw;
}
NAPPEARANCE_RAW* NUnpackAppearance_TM(NAPPEARANCE_RAW *praw, const NAPPEARANCE_TM *ptm)
{
	NErrorIf(!praw || !ptm, NERROR_NULL_POINTER);
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<1,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);

	Nmem0(praw,NAPPEARANCE_RAW);
	praw->pBlend						= ptm->pBlend;
	praw->pMaterial						= ptm->pMaterial;
	praw->TextureUnit[0].pTexture		= ptm->TextureUnitTM.pTexture;
	praw->TextureUnit[0].pUVMatrixAnim	= ptm->TextureUnitTM.pUVMatrixAnim;

	return praw;
}
NAPPEARANCE_RAW* NUnpackAppearance_ULTD_T(NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_T *pu)
{
	NErrorIf(!praw || !pu, NERROR_NULL_POINTER);
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<pu->TextureUnitTNumber,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);

	NTEXTUREUNIT_T	*ptut	= pu->pTextureUnitT;

	Nmem0(praw,NAPPEARANCE_RAW);
	praw->pBlend				= pu->pBlend;
	praw->pMaterial				= pu->pMaterial;
	
	for(Nu8 i=0;i<pu->TextureUnitTNumber;i++,ptut++)
	{
		praw->TextureUnit[i].pTexture		= ptut->pTexture;
		praw->TextureUnit[i].pUVMatrixAnim	= NULL;
	}
	return praw;
}
NAPPEARANCE_RAW* NUnpackAppearance_ULTD_TM(NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_TM *pu)
{
	NErrorIf(!praw || !pu, NERROR_NULL_POINTER);
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<pu->TextureUnitTMNumber,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);

	NTEXTUREUNIT_TM	*ptutm	= pu->pTextureUnitTM;

	Nmem0(praw,NAPPEARANCE_RAW);
	praw->pBlend				= pu->pBlend;
	praw->pMaterial				= pu->pMaterial;

	for(Nu8 i=0;i<pu->TextureUnitTMNumber;i++,ptutm++)
	{
		praw->TextureUnit[i].pTexture		= ptutm->pTexture;
		praw->TextureUnit[i].pUVMatrixAnim	= ptutm->pUVMatrixAnim;
	}
	return praw;
}
NAPPEARANCE_RAW* NUnpackAppearance( NAPPEARANCE_RAW *praw, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE *papp)
{
	switch( app_format )
	{
		case NAPPEARANCE_FORMAT_NULL:
			Nmem0(praw,NAPPEARANCE_RAW);
			break;
		case NAPPEARANCE_FORMAT_TT:
			NUnpackAppearance_TT(praw,&papp->TT);
			break;
		case NAPPEARANCE_FORMAT_TM:
			NUnpackAppearance_TM(praw,&papp->TM);
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NUnpackAppearance_ULTD_T(praw,&papp->ULTD_T);
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NUnpackAppearance_ULTD_TM(praw,&papp->ULTD_TM);
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			break;
	}
	return praw;
}
// ------------------------------------------------------------------------------------------
// NAPPEARANCE_CHECKMASK* NSetupAppearanceCheckMask
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of NAPPEARANCE_CHECKMASK is to setup manually member by member to make precise
//	checking between Appearances.
//	But, it could be easier to use this setup function in a first pass to roughly define the Mask before
//	before finalize it by changing the value of specific member.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_CHECKMASK* NSetupAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pappcheckmask, const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue, const Nu32 nbtexture_unit   )
{
	NErrorIf(nbtexture_unit>CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER, NERROR_VALUE_OUTOFRANGE);
	NErrorIf((blend_maskvalue!=NAPPEARANCE_CHECKMASK_MASKED) && (blend_maskvalue!=NAPPEARANCE_CHECKMASK_EQUAL) && (blend_maskvalue!=NAPPEARANCE_CHECKMASK_DIFFERENT) , NERROR_INCONSISTENT_VALUES);
	NErrorIf((material_maskvalue!=NAPPEARANCE_CHECKMASK_MASKED) && (material_maskvalue!=NAPPEARANCE_CHECKMASK_EQUAL) && (material_maskvalue!=NAPPEARANCE_CHECKMASK_DIFFERENT) , NERROR_INCONSISTENT_VALUES);
	NErrorIf((texture_maskvalue!=NAPPEARANCE_CHECKMASK_MASKED) && (texture_maskvalue!=NAPPEARANCE_CHECKMASK_EQUAL) && (texture_maskvalue!=NAPPEARANCE_CHECKMASK_DIFFERENT) , NERROR_INCONSISTENT_VALUES);
	NErrorIf((txcmatrixupdater_maskvalue!=NAPPEARANCE_CHECKMASK_MASKED) && (txcmatrixupdater_maskvalue!=NAPPEARANCE_CHECKMASK_EQUAL) && (txcmatrixupdater_maskvalue!=NAPPEARANCE_CHECKMASK_DIFFERENT) , NERROR_INCONSISTENT_VALUES);

	// Note: 
	// The lines bellow are working because NAPPEARANCE_CHECKMASK_MASKED = 0.
	// So, just a security here:
	NErrorIf(NAPPEARANCE_CHECKMASK_MASKED!=0,NERROR_SYSTEM_INVALID_CONSTANT); // 
	Nmem0(pappcheckmask,NAPPEARANCE_CHECKMASK);// ... so all masks are going to be setup up as NAPPEARANCE_CHECKMASK_MASKED !
	pappcheckmask->Blend	= blend_maskvalue;
	pappcheckmask->Material = material_maskvalue;
	
	for(Nu32 i=0;i<nbtexture_unit;i++)
	{
		pappcheckmask->TU_Texture[i]		= texture_maskvalue;
		pappcheckmask->TU_MatrixUpdater[i]	= txcmatrixupdater_maskvalue;
	}
	return pappcheckmask;
}
// ------------------------------------------------------------------------------------------
// NAPPEARANCE_CHECKMASK* NCreateAppearanceCheckMask
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
NAPPEARANCE_CHECKMASK* NCreateAppearanceCheckMask(const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue, const Nu32 nbtexture_unit   )
{
	return NSetupAppearanceCheckMask( NEW(NAPPEARANCE_CHECKMASK), blend_maskvalue, material_maskvalue, texture_maskvalue, txcmatrixupdater_maskvalue, nbtexture_unit );
}
// ------------------------------------------------------------------------------------------
// void NClearAppearanceCheckMask
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
void NClearAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask)
{
	Nmem0(pcheckmask,NAPPEARANCE_CHECKMASK);
}
// ------------------------------------------------------------------------------------------
// void NDeleteAppearanceCheckMask
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
void NDeleteAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask)
{
	NClearAppearanceCheckMask(pcheckmask);
	Nfree(pcheckmask);
}

static inline Nbool _checkpointervalue(const void *p0, const void *p1, const Nu8 checkmask)
{
	if( p0 != p1 )
	{
		// must be EQUAL
		if( checkmask == NAPPEARANCE_CHECKMASK_EQUAL )
			return NFALSE;
	}
	else
	{
		// must be DIFFERENT
		if( checkmask == NAPPEARANCE_CHECKMASK_DIFFERENT )
			return NFALSE;
	}
	
	return NTRUE;
}
Nbool NCheckAppearanceRaw_vs_Appearance_TT(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_TT *ptt, const NAPPEARANCE_CHECKMASK *pmask )
{
	// pmask may be a NULL pointer, that means NO MASK so the equality between praw and ptt has to be total !
	// its like all NAPPEARANCE_CHECKMASK are set to NAPPEARANCE_CHECKMASK_EQUAL
	NErrorIf(!praw || !ptt || !pmask, NERROR_NULL_POINTER);
	
	// Let's check
	if( !_checkpointervalue(praw->pBlend,ptt->pBlend,pmask->Blend ) )
		return NFALSE;
	if( !_checkpointervalue(praw->pMaterial,ptt->pMaterial,pmask->Material ) )
		return NFALSE;
	
	Nu8 i;
	// TU 0 and TU 1
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<2,NERROR_SYSTEM_NENGINEVERSION_CONFLICT); // Just in case !
	for(i=0;i<2;i++)
	{
		if( !_checkpointervalue(praw->TextureUnit[i].pTexture, ptt->TextureUnitT[i].pTexture,pmask->TU_Texture[i]) )
			return NFALSE;

		if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE; // Because NTEXTUREUNIT_T doesn't have a MATRIX ptr inside !!!
	}

	// Check all the other Appearance Masks ... They have to be MASKED, otherwise that means user asks to check a non-existent pointer ... 
	for(i=2;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)// just to always work, even if CONSTANT changes ... into a future next engine version
	{
		if( pmask->TU_Texture[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
		if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
	}
	// ... GOOD !!! Both Appearances match together !!!
	return NTRUE;
}
Nbool NCheckAppearanceRaw_vs_Appearance_TM(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_TM *ptm, const NAPPEARANCE_CHECKMASK *pmask )
{
	// pmask may be a NULL pointer, that means NO MASK so the equality between praw and ptm has to be total !
	// its like all NAPPEARANCE_CHECKMASK are set to NAPPEARANCE_CHECKMASK_EQUAL
	NErrorIf(!praw || !ptm || !pmask, NERROR_NULL_POINTER);

	// Let's check
	if( !_checkpointervalue(praw->pBlend,ptm->pBlend,pmask->Blend ) )
		return NFALSE;
	if( !_checkpointervalue(praw->pMaterial,ptm->pMaterial,pmask->Material ) )
		return NFALSE;
	// Texture Unit 0
	if( !_checkpointervalue(praw->TextureUnit[0].pTexture, ptm->TextureUnitTM.pTexture, pmask->TU_Texture[0]) )
		return NFALSE;
	if( !_checkpointervalue(praw->TextureUnit[0].pUVMatrixAnim, ptm->TextureUnitTM.pUVMatrixAnim, pmask->TU_MatrixUpdater[0]) )
		return NFALSE;

	// Check all the other Appearance Masks ... They have to be MASKED, otherwise that means user asks to check a non-existent pointer ... 
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER<1,NERROR_SYSTEM_NENGINEVERSION_CONFLICT); // Just in case !
	for(Nu8 i=1;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++)
	{
		if( pmask->TU_Texture[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
		if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
	}
	// ... GOOD !!! Both Appearances match together !!!
	return NTRUE;
}

Nbool NCheckAppearanceRaw_vs_Appearance_ULTD_T(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_T *ptultd_t, const NAPPEARANCE_CHECKMASK *pmask )
{
	// pmask may be a NULL pointer, that means NO MASK so the equality between praw and ptultd_t has to be total !
	// its like all NAPPEARANCE_CHECKMASK are set to NAPPEARANCE_CHECKMASK_EQUAL
	NErrorIf(!praw || !ptultd_t || !pmask, NERROR_NULL_POINTER);

	// Let's check
	if( !_checkpointervalue(praw->pBlend,ptultd_t->pBlend,pmask->Blend ) )
		return NFALSE;
	if( !_checkpointervalue(praw->pMaterial,ptultd_t->pMaterial,pmask->Material ) )
		return NFALSE;
	// All TUs Texture and Matrix
	NErrorIf(ptultd_t->TextureUnitTNumber<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);
	NTEXTUREUNIT_T	*ptut	= ptultd_t->pTextureUnitT;
	Nu8	i;
	for(i=0;i<ptultd_t->TextureUnitTNumber;i++, ptut++)
	{
		if( !_checkpointervalue(praw->TextureUnit[i].pTexture,	ptut->pTexture,	pmask->TU_Texture[i]) )
		{
			return NFALSE;
		}
	}

	// Check all the other Appearance Masks ... They have to be MASKED, otherwise that means user asks to check a non-existent pointer ... 
	for(i=ptultd_t->TextureUnitTNumber;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++ )
	{
		if( pmask->TU_Texture[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
		if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
	}
	// ... GOOD !!! Both Appearances match together !!!
	return NTRUE;
}
Nbool NCheckAppearanceRaw_vs_Appearance_ULTD_TM(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_TM *ptultd_tm, const NAPPEARANCE_CHECKMASK *pmask )
{
	// pmask may be a NULL pointer, that means NO MASK so the equality between praw and ptultd_tm has to be total !
	// its like all NAPPEARANCE_CHECKMASK are set to NAPPEARANCE_CHECKMASK_EQUAL
	NErrorIf(!praw || !ptultd_tm || !pmask, NERROR_NULL_POINTER);

	// Let's check
	if( !_checkpointervalue(praw->pBlend,ptultd_tm->pBlend,pmask->Blend ) )
		return NFALSE;
	if( !_checkpointervalue(praw->pMaterial,ptultd_tm->pMaterial,pmask->Material ) )
		return NFALSE;
	// All TUs Texture and Matrix
	NErrorIf(ptultd_tm->TextureUnitTMNumber<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_SYSTEM_NENGINEVERSION_CONFLICT);
	NTEXTUREUNIT_TM *ptutm	= ptultd_tm->pTextureUnitTM;
	Nu8	i;
	for(i=0;i<ptultd_tm->TextureUnitTMNumber;i++,ptutm++)
	{
		if( !_checkpointervalue(praw->TextureUnit[i].pTexture, ptutm->pTexture,	pmask->TU_Texture[i])||
			!_checkpointervalue(praw->TextureUnit[i].pUVMatrixAnim,	ptutm->pUVMatrixAnim, pmask->TU_MatrixUpdater[i]) )
		{
			return NFALSE;
		}
	}
	// Check all the other Appearance Masks ... They have to be MASKED, otherwise that means user asks to check a non-existent pointer ... 
	for(i=ptultd_tm->TextureUnitTMNumber;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++ )
	{
		if( pmask->TU_Texture[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
		if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
			return NFALSE;
	}
	// ... GOOD !!! Both Appearances match together !!!
	return NTRUE;
}

Nbool NCheckAppearanceRaw_vs_Appearance(const NAPPEARANCE_RAW *praw, const NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE_CHECKMASK *pmask )
{
	switch( app_format )
	{
		case NAPPEARANCE_FORMAT_NULL:
			// Check all the Appearance Masks ... They have to be MASKED, otherwise that means user asks to check a non-existent pointer ... 
			if( pmask->Blend != NAPPEARANCE_CHECKMASK_MASKED )
				return NFALSE;
			if( pmask->Material != NAPPEARANCE_CHECKMASK_MASKED )
				return NFALSE;

			for(Nu8 i=0;i<CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER;i++ )
			{
				if( pmask->TU_Texture[i] != NAPPEARANCE_CHECKMASK_MASKED )
					return NFALSE;
				if( pmask->TU_MatrixUpdater[i] != NAPPEARANCE_CHECKMASK_MASKED )
					return NFALSE;
			}
			break;
		case NAPPEARANCE_FORMAT_TT:
			if(!NCheckAppearanceRaw_vs_Appearance_TT(praw,&papp->TT,pmask))
				return NFALSE;
			break;
		case NAPPEARANCE_FORMAT_TM:
			if(!NCheckAppearanceRaw_vs_Appearance_TM(praw,&papp->TM,pmask))
				return NFALSE;
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			if(!NCheckAppearanceRaw_vs_Appearance_ULTD_T(praw,&papp->ULTD_T,pmask))
				return NFALSE;
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			if(!NCheckAppearanceRaw_vs_Appearance_ULTD_TM(praw,&papp->ULTD_TM,pmask))
				return NFALSE;
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			return NFALSE; // In Release mode, no Crash ... return NFALSE !
	}
	return NTRUE;
}
NTEXTURE* NGetAppearance_TT_Texture(const NAPPEARANCE_TT *papp,const Nu8 texture_unit)
{
	NErrorIf(texture_unit > 1,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
	return papp->TextureUnitT[texture_unit].pTexture;
}
NTEXTURE* NGetAppearance_TM_Texture(const NAPPEARANCE_TM *papp)
{
	return papp->TextureUnitTM.pTexture;
}

NTEXTURE* NGetAppearance_ULTD_T_Texture(const NAPPEARANCE_ULTD_T *papp,const Nu8 texture_unit)
{
	NErrorIf(texture_unit >= papp->TextureUnitTNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
	return (papp->pTextureUnitT + texture_unit)->pTexture;
}
NTEXTURE* NGetAppearance_ULTD_TM_Texture(const NAPPEARANCE_ULTD_TM *papp,const Nu8 texture_unit)
{
	NErrorIf(texture_unit >= papp->TextureUnitTMNumber,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
	return (papp->pTextureUnitTM + texture_unit)->pTexture;
}

NTEXTURE* NGetAppearance_Texture(const NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const Nu8 texture_unit)
{
	switch( app_format )
	{
		case NAPPEARANCE_FORMAT_NULL:
			return NULL;
		
		case NAPPEARANCE_FORMAT_TT:
			return NGetAppearance_TT_Texture(&papp->TT,texture_unit);

		case NAPPEARANCE_FORMAT_TM:
			if(texture_unit==0)
			{
				return NGetAppearance_TM_Texture(&papp->TM);
			}
			else
			{
				NErrorIf(1,NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE);
				return NULL;
			}

		case NAPPEARANCE_FORMAT_ULTD_T:
			return NGetAppearance_ULTD_T_Texture(&papp->ULTD_T,texture_unit);

		case NAPPEARANCE_FORMAT_ULTD_TM:
			return NGetAppearance_ULTD_TM_Texture(&papp->ULTD_TM,texture_unit);

		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			return NULL; // In Release mode, no Crash ... return NULL, thats all !
	}
}

// ------------------------------------------------------------------------------------------
// Nu32 NGetAppearance_StateSet
// ------------------------------------------------------------------------------------------
// Description :
//		!!! WARNING!!! IT DOESN'T compute a full usable StateSet !!!
//		It ONLY Compute the "stateset" part linked with the Appearance content.
//		This part is the one masked by "MASK_STATESET_Z_AND_BLENDLIGHT"
//		all the other bits remain OFF.	
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//		a Nu32 StateSet code with the "Z and Appearance Bits"  potentially set to 1.
//		all the others bits are going to still 0.
// ------------------------------------------------------------------------------------------
NSTATESET NGetAppearance_BlendAndMaterial_StateSet(const NAPPEARANCE *papp)
{
	Nu32 stateset = 0;

	if(papp->Common.pBlend)
	{
		FLAG_ON(stateset,FLAG_STATESET_ZREAD|FLAG_STATESET_BLEND);
		FLAG_OFF(stateset,FLAG_STATESET_ZWRITE);
	}
	else
	{
		FLAG_OFF(stateset,FLAG_STATESET_BLEND);
		FLAG_ON(stateset,FLAG_STATESET_ZREAD|FLAG_STATESET_ZWRITE);
	}

	if(papp->Common.pMaterial)
		FLAG_ON(stateset,FLAG_STATESET_LIGHT); 
	else
		FLAG_OFF(stateset,FLAG_STATESET_LIGHT);

	// return (stateset & MASK_STATESET_Z_AND_BLENDLIGHT); ---> useless because stateset was initialized to ZERO
	//															but it's the purpose of this function.
	return stateset;
}
