#include "NGeometryTextureUnit.h"
#include "lib/N/NMatrix.h"
/*
// ------------------------------------------------------------------------------------------
// NSetupTextureUnitsContainer_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_T*	NSetupTextureUnitsContainer_T( NTEXTUREUNITS_CONTAINER_T *ptucontainer )
{
	Nmem0(ptucontainer,NTEXTUREUNITS_CONTAINER_T);
	return ptucontainer;
}
// ------------------------------------------------------------------------------------------
// NCreateTextureUnitsContainer_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_T* NCreateTextureUnitsContainer_T()
{
	return NSetupTextureUnitsContainer_T(NEW(NTEXTUREUNITS_CONTAINER_T));
}
// ------------------------------------------------------------------------------------------
// NClearTextureUnitsContainer_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearTextureUnitsContainer_T(void *ptr)
{
	Nmem0(ptr,NTEXTUREUNITS_CONTAINER_T);
}
// ------------------------------------------------------------------------------------------
// NDeleteTextureUnitsContainer_T
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteTextureUnitsContainer_T(void *ptr)
{
	NClearTextureUnitsContainer_T(ptr);
	Nfree(ptr);
}
// ------------------------------------------------------------------------------------------
// NSetupTextureUnitsContainer_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_TT*	NSetupTextureUnitsContainer_TT( NTEXTUREUNITS_CONTAINER_TT *ptucontainer )
{
	Nmem0(ptucontainer,NTEXTUREUNITS_CONTAINER_TT);
	return ptucontainer;
}
// ------------------------------------------------------------------------------------------
// NCreateTextureUnitsContainer_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_TT* NCreateTextureUnitsContainer_TT()
{
	return NSetupTextureUnitsContainer_TT(NEW(NTEXTUREUNITS_CONTAINER_TT));
}
// ------------------------------------------------------------------------------------------
// NClearTextureUnitsContainer_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearTextureUnitsContainer_TT(void *ptr)
{
	Nmem0(ptr,NTEXTUREUNITS_CONTAINER_TT);
}
// ------------------------------------------------------------------------------------------
// NDeleteTextureUnitsContainer_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteTextureUnitsContainer_TT(void *ptr)
{
	NClearTextureUnitsContainer_TT(ptr);
	Nfree(ptr);
}

// ------------------------------------------------------------------------------------------
// NSetupTextureUnitsContainer_TT
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_TM*	NSetupTextureUnitsContainer_TM( NTEXTUREUNITS_CONTAINER_TM *ptucontainer )
{
	Nmem0(ptucontainer,NTEXTUREUNITS_CONTAINER_TM);
	return ptucontainer;
}
// ------------------------------------------------------------------------------------------
// NCreateTextureUnitsContainer_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NTEXTUREUNITS_CONTAINER_TM* NCreateTextureUnitsContainer_TM()
{
	return NSetupTextureUnitsContainer_TM(NEW(NTEXTUREUNITS_CONTAINER_TM));
}
// ------------------------------------------------------------------------------------------
// NClearTextureUnitsContainer_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearTextureUnitsContainer_TM(void *ptr)
{
	Nmem0(ptr,NTEXTUREUNITS_CONTAINER_TM);
}
// ------------------------------------------------------------------------------------------
// NDeleteTextureUnitsContainer_TM
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteTextureUnitsContainer_TM(void *ptr)
{
	NClearTextureUnitsContainer_TM(ptr);
	Nfree(ptr);
}

NTEXTUREUNITS_CONTAINER_ULTD*	NSetupTextureUnitsContainer_ULTD( NTEXTUREUNITS_CONTAINER_ULTD *ptucontainer, const Nu8 textureunitformat, const Nu8 textureunitnb, const Nu16 textureunitstructsize)
{
	NErrorIf( !textureunitnb,NERROR_GEOMETRY_MAPS_NEEDS_ONE_TEXTUREUNIT_AT_LEAST);
	NErrorIf( !textureunitstructsize,NERROR_INCONSISTENT_PARAMETERS);

	ptucontainer->pTextureUnit			= Nmalloc(textureunitstructsize*textureunitnb);
	memset(ptucontainer->pTextureUnit,0,textureunitstructsize*textureunitnb);
	ptucontainer->TextureUnitNb			= textureunitnb;
	ptucontainer->TextureUnitStructSize	= textureunitstructsize;
	ptucontainer->TextureUnitFormat		= textureunitformat;

	return ptucontainer;
}

NTEXTUREUNITS_CONTAINER_ULTD*	NCreateTextureUnitsContainer_ULTD( const Nu8 textureunitformat, const Nu8 textureunitnb, const Nu16 textureunitstructsize )
{
	return NSetupTextureUnitsContainer_ULTD( NEW(NTEXTUREUNITS_CONTAINER_ULTD), textureunitformat, textureunitnb, textureunitstructsize );
}
void	NClearTextureUnitsContainer_ULTD( void *ptucontainer)
{
	Nfree(((NTEXTUREUNITS_CONTAINER_ULTD*)ptucontainer)->pTextureUnit);
	Nmem0(ptucontainer,NTEXTUREUNITS_CONTAINER_ULTD);
}

void NDeleteTextureUnitsContainer_ULTD(void *ptucontainer)
{
	NClearTextureUnitsContainer_ULTD(ptucontainer);
	Nfree(ptucontainer);
}

void NCopyTextureUnitsContainer_ULTD(NTEXTUREUNITS_CONTAINER_ULTD *pdest, const NTEXTUREUNITS_CONTAINER_ULTD *psrc)
{
// 	NErrorIf(pdest->TextureUnitFormat != psrc->TextureUnitFormat, NERROR_GEOMETRY_MAPS_INCOMPATIBLE_TYPE);
// 	NErrorIf(pdest->TextureUnitNb != psrc->TextureUnitNb, NERROR_GEOMETRY_MAPS_INCOMPATIBLE_TYPE);
// 	NErrorIf(pdest->TextureUnitStructSize != psrc->TextureUnitStructSize, NERROR_GEOMETRY_TEXTUREUNIT_INCOMPATIBLE_TYPE);
	
	// ! Source and Destination may have different data structure so let's check !
	if( (pdest->TextureUnitFormat != psrc->TextureUnitFormat)
		||(pdest->TextureUnitStructSize != psrc->TextureUnitStructSize) // Certainly useless due to the previous test 
		||(pdest->TextureUnitNb != psrc->TextureUnitNb) )
	{
		NClearTextureUnitsContainer_ULTD( pdest );
		NSetupTextureUnitsContainer_ULTD( pdest, psrc->TextureUnitFormat,psrc->TextureUnitNb, psrc->TextureUnitStructSize );
	}
	// ... And copy !!!	
	memcpy( pdest->pTextureUnit,psrc->pTextureUnit,psrc->TextureUnitNb*psrc->TextureUnitStructSize );
}
*/
