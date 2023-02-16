#ifndef __NGEOMETRY_TEXTUREUNIT_H
#define __NGEOMETRY_TEXTUREUNIT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NGeometryTextureUnit.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NTexture.h"
#include "../../NMatrix.h"
#include "NGeometryTextureMatrixAnimation.h"
#include "./Specifications/NGeometryTextureUnit_Flags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------
typedef enum  
{
	NTEXTUREUNIT_FORMAT_NULL = 0,
	NTEXTUREUNIT_FORMAT_T,
	NTEXTUREUNIT_FORMAT_TM,
	//NTEXTUREUNIT_FORMAT_AVAILABLE,
	// -----------------------
	NTEXTUREUNIT_FORMAT_ENUM_SIZE	// MAX size is 4 !!! (and max ID is 3) This ID is going to be stored on a 2 bits value !!!
}NTEXTUREUNIT_FORMAT_ENUM;

/*
typedef  struct NUVMATRIXANIM		NUVMATRIXANIM;
typedef void (*NTEXTUREMATRIX_UPDATE_FCT)(NUVMATRIXANIM *pupdater);
struct NUVMATRIXANIM
{
	NTEXTUREMATRIX_UPDATE_FCT	Update;
	NMATRIX						Matrix;
};
*/
typedef struct  
{
	NTEXTURE *pTexture;
}NTEXTUREUNIT_T;

typedef struct  
{
	NTEXTURE		*pTexture;
	NUVMATRIXANIM	*pUVMatrixAnim;
}NTEXTUREUNIT_TM;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct  
{
	Nu32	Flags;
	Nu8		SizeOfTextureUnit;
	// 	Nu8		Available_Nu8;
	// 	Nu16	Available_Nu16;
}NTEXTUREUNIT_SPECS;

// ***************************************************************************************
// **					TextureUnit Functions											**
// ***************************************************************************************
inline NTEXTUREUNIT_T*				NSetupTextureUnit_T( NTEXTUREUNIT_T *ptut, const NTEXTURE *ptexture){ptut->pTexture = (NTEXTURE*)ptexture;return ptut;}
inline NTEXTUREUNIT_T*				NCreateTextureUnit_T( const NTEXTURE *ptexture){return NSetupTextureUnit_T(NEW(NTEXTUREUNIT_T),ptexture);}
inline void							NClearTextureUnit_T( void *ptut){((NTEXTUREUNIT_T*)ptut)->pTexture = NULL;}
inline void							NDeleteTextureUnit_T( void *ptut){NClearTextureUnit_T(ptut);Nfree(ptut);}

inline NTEXTUREUNIT_TM*				NSetupTextureUnit_TM( NTEXTUREUNIT_TM *ptu, const NTEXTURE *ptexture,const NUVMATRIXANIM *pm){ptu->pTexture = (NTEXTURE*)ptexture;ptu->pUVMatrixAnim = (NUVMATRIXANIM*)pm;return ptu;}
inline NTEXTUREUNIT_TM*				NCreateTextureUnit_TM( const NTEXTURE *ptexture, const NUVMATRIXANIM *pm){return NSetupTextureUnit_TM(NEW(NTEXTUREUNIT_TM),ptexture,pm);}
inline void							NClearTextureUnit_TM( void *ptu){Nmem0(ptu,NTEXTUREUNIT_TM);}
inline void							NDeleteTextureUnit_TM( void *ptu){NClearTextureUnit_TM(ptu);Nfree(ptu);}

// ***************************************************************************************
// **					TextureUnit "Specs" Functions									**
// ***************************************************************************************
NTEXTUREUNIT_SPECS*					NGetTextureUnitSpecs(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format);
Nu32								NGetTextureUnitSpecsFlags(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format);
Nu8									NGetSizeOfTextureUnit(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format);

// ***************************************************************************************
// **					TextureUnit "PRIVATE" Functions									**
// ***************************************************************************************
void								NInitializeTextureUnitSpecs();
void								NDisableTextureUnitSpecs();

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYTEXTUREUNIT_H 

