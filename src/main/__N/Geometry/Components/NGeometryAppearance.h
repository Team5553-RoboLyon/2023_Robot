#ifndef __NGEOMETRYCOMPONENT_APPEARANCE_H
#define __NGEOMETRYCOMPONENT_APPEARANCE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryAppearance.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Render/Accumulator/NStateSet.h"
#include "NGeometrytextureUnit.h"
#include "NGeometryMaterial.h"
#include "NGeometryBlend.h"
#include "./Specifications/NGeometryAppearance_Flags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **					Appearance Format Enum											**
// ***************************************************************************************
typedef enum
{
	NAPPEARANCE_FORMAT_NULL = 0,	// Has to be 0 ( used as return value in appaearance read function )
	NAPPEARANCE_FORMAT_TT,			// NAPPEARANCE "LIMITED" format TT: contains 1 or 2 simple TextureUnits (NTEXTUREUNIT_T) 
	NAPPEARANCE_FORMAT_TM,			// NAPPEARANCE "LIMITED" format TM: contains 1 simple TextureUnit (NTEXTUREUNIT_TM) 
	NAPPEARANCE_FORMAT_ULTD_T,		// NAPPEARANCE "UNLIMITED" format: contains 0 to 255 TextureUnits (all NTEXTUREUNIT_T)
	NAPPEARANCE_FORMAT_ULTD_TM,		// NAPPEARANCE "UNLIMITED" format: contains 0 to 255 TextureUnits (all NTEXTUREUNIT_TM)
	//NAPPEARANCE_FORMAT_xxx,		// 
	//NAPPEARANCE_FORMAT_xxx,		// 
	//NAPPEARANCE_FORMAT_xxx,		// 
	//----------------------	
	NAPPEARANCE_FORMAT_ENUM_SIZE	// MAX size is 8 !!! (and max ID is 7) This ID is going to be stored on a 3 bits value !!!
}NAPPEARANCE_FORMAT_ENUM;

// ***************************************************************************************
// **					Appearance "Sub-Type" Structures								**
// ***************************************************************************************
typedef struct 
{
	NMATERIAL						*pMaterial;		//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !  
	NBLEND							*pBlend;		//|
}NAPPEARANCE_COMMON;

typedef struct 
{
	NMATERIAL			*pMaterial;		//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !  
	NBLEND				*pBlend;		//|
	// TextureUnits
	union
	{
		NTEXTUREUNIT_T	TextureUnitT[2];

		struct  
		{
			NTEXTUREUNIT_T			TextureUnitT0;
			NTEXTUREUNIT_T			TextureUnitT1;
		};
	};
}NAPPEARANCE_TT;
typedef struct 
{
	NMATERIAL			*pMaterial;		//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !
	NBLEND				*pBlend;		//|
	// TextureUnits
	NTEXTUREUNIT_TM		TextureUnitTM;
}NAPPEARANCE_TM;

typedef struct 
{
	NMATERIAL			*pMaterial;		//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !
	NBLEND				*pBlend;		//|
	// TextureUnits
	NTEXTUREUNIT_T		*pTextureUnitT;
	Nu8					TextureUnitTNumber;
	
	Nu8					Available_Nu8;
	Nu16				Available_Nu16;
}NAPPEARANCE_ULTD_T;

typedef struct 
{
	NMATERIAL			*pMaterial;		//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !
	NBLEND				*pBlend;		//|
	// TextureUnits
	NTEXTUREUNIT_TM		*pTextureUnitTM;
	Nu8					TextureUnitTMNumber;
	
	Nu8					Available_Nu8;
	Nu16				Available_Nu16;
}NAPPEARANCE_ULTD_TM;

// ***************************************************************************************
// **					Appearance Structures											**
// ***************************************************************************************
typedef union								
{											//|This union is a smart one ! Because NAPPEARANCE_T,NAPPEARANCE_TT,NAPPEARANCE_TM and NAPPEARANCE_ULTD
	NAPPEARANCE_COMMON	Common;				//|have exactly the same size. So, in the case of using only 1 or 2 texture units 
	NAPPEARANCE_TT		TT;					//|Same thing for using only one texture unit with one TM format.
	NAPPEARANCE_TM		TM;					//|But for larger number of textures NAPPEARANCE_ULTD (unlimited)
	NAPPEARANCE_ULTD_T	ULTD_T;				//|will be used with an extra malloc to store them ...
	NAPPEARANCE_ULTD_TM	ULTD_TM;			//|will be used with an extra malloc to store them ...
}NAPPEARANCE;								//|Note1: This union doesn't know itself its own specific type !
											//|Note2: NAPPEARANCE_T is not so different than NAPPEARANCE_TT but the seconde texture unit is clearly named UnusedTextureUnit.
											//|		  So, it's just for a better readability.	 

// NAPPEARANCE manage the TextureUnits number into a smart optimized way.
// Optimized ? What kind of optimization ? Actually, a lot of geometries are going to have 
// only 1 texture, sometimes 2. But, because some of them are going to have more than 2 we
// have to manage "n textures" inside geometries ( ... and THIS is the future isn't it !).
// And, in the top of that, sometimes we want to animate UVs coordinates, so using UV Matrix to do that.
// The matrix pointer is going to be stored into the texture unit, if its necessary ...
// Using a simple NARRAY costs at least 16 NBYTES for a geometry with 0 texture unit, 20 NBYTES
// with 1 texture unit, 24 BYTES with 2, ...(16 + nbtextureunit*4) NBYTES... And more for textureunit with MATRIX included !
// NAPPEARANCE management use less ...
// 8 NBYTES for 0,1 or 2 simple texture units ! ... And for more (8+nbtextureunit*4) NBYTES !
// The maximum number of Texture unit managed by one appearance is ... (but it could be NU32_MAX if necessary ... )
#define CONSTANT_NAPPEARANCE_TEXTUREUNIT_NUMBER_MAX			NU8_MAX

// ------------------------------------------------------------------------------------------------------------------------------------------
// NAPPEARANCE_RAW is an appearance structure with a fixed Size.  
// There is a fixed number of texture units and all texture units are the biggest ones ( NTEXTUREUNIT_TM ) 
// This structure is not the one included into the NGEOMETRY structure, but due to its fixed/static size is 
// the perfect candidate to describe Appearance inside "???_DESC structures". 
// CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER represents the number of texture units allocated into NAPPEARANCE_RAW structure.
// Right now is fixed to 8 but it's possible to change it for a bigger value. ( MAX is 256)
#define CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER		8	// That means NAPPEARANCE_RAW manages 8 TextureUnits
															// from ID 0 to ID 7 included. MAX textureUnits number is 256 !
typedef struct 
{
	NMATERIAL			*pMaterial;			//|_At the top, and identical for all LIMITED and UNLIMITED formats ... and RAW !
	NBLEND				*pBlend;			//|
	NTEXTUREUNIT_TM		TextureUnit[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];
}NAPPEARANCE_RAW;

// NAPPEARANCE_CHECKMASK is use full to determine exactly what you are looking for when using "check vs" functions.
#define		NAPPEARANCE_CHECKMASK_MASKED			0	// Appearance Raw member and corresponding checked Appearance member are not going to be compare together.
#define		NAPPEARANCE_CHECKMASK_EQUAL				1	// Appearance Raw member and corresponding checked Appearance member are going to be compare together and must be EQUAL.
#define		NAPPEARANCE_CHECKMASK_DIFFERENT			2	// Appearance Raw member and corresponding checked Appearance member are going to be compare together and must be DIFFERENT.
typedef struct 
{
	Nu8	TU_Texture[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];
	Nu8	TU_MatrixUpdater[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];
	Nu8	Material;			
	Nu8	Blend;
}NAPPEARANCE_CHECKMASK;

// ***************************************************************************************
// **					Appearance "Specs" Structure									**
// ***************************************************************************************
typedef struct  
{
	Nu32	Flags;
}NAPPEARANCE_SPECS;

// ***************************************************************************************
// **					Appearance Functions											**
// ***************************************************************************************
NAPPEARANCE*					NSetAppearance( NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE_RAW *praw);
NAPPEARANCE_RAW*				NUnpackAppearance( NAPPEARANCE_RAW *praw, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE *papp);
NSTATESET						NGetAppearance_BlendAndMaterial_StateSet(const NAPPEARANCE *papp);
NTEXTURE*						NGetAppearance_Texture(const NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const Nu8 texture_unit);
void							NSetAppearance_Texture(NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const Nu8 texture_unit,const NTEXTURE *ptexture);

// + -------------------------+ Notes:
//								These 4 inline functions don't take care about Geometry capabilities (which may refuse Blend or/and Material..)
//								Prefer using 'NSetGeometryBLend/NGetGeometryBLend' and 'NSetGeometryMaterial/NGetGeometryMaterial'
//								to take care of them.	
inline NBLEND*					NGetAppearance_Blend(NAPPEARANCE *papp){return papp->Common.pBlend;}
inline NMATERIAL*				NGetAppearance_Material(NAPPEARANCE *papp){return papp->Common.pMaterial;}
inline void						NSetAppearance_Blend(NAPPEARANCE *papp, const NBLEND *pblend){papp->Common.pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_Material(NAPPEARANCE *papp, const NMATERIAL *pmat){papp->Common.pMaterial = (NMATERIAL*)pmat;}

// ***************************************************************************************
// **					Appearance "Raw" Functions										**
// ***************************************************************************************
inline void						NSetAppearance_Raw_Texture(NAPPEARANCE_RAW *pdesc,const Nu8 tu, const NTEXTURE *ptexture){NErrorIf(tu>=CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);pdesc->TextureUnit[tu].pTexture = (NTEXTURE*)ptexture;}
inline void						NSetAppearance_Raw_TextureMatrixAnim(NAPPEARANCE_RAW *pdesc,const Nu8 tu, const NUVMATRIXANIM *pmu){NErrorIf(tu>=CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);pdesc->TextureUnit[tu].pUVMatrixAnim = (NUVMATRIXANIM*)pmu;}
inline void						NSetAppearance_Raw_BlendAndMaterial(NAPPEARANCE_RAW *pdesc,const NBLEND *pblend, const NMATERIAL *pmaterial){pdesc->pBlend = (NBLEND*)pblend;pdesc->pMaterial = (NMATERIAL*)pmaterial;}
inline void						NSetAppearance_Raw_Blend(NAPPEARANCE_RAW *pdesc,const NBLEND *pblend){pdesc->pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_Raw_Material(NAPPEARANCE_RAW *pdesc,const NMATERIAL *pmaterial){pdesc->pMaterial = (NMATERIAL*)pmaterial;}

inline NTEXTURE*				NGetAppearance_Raw_Texture(NAPPEARANCE_RAW *pdesc,const Nu8 tu){NErrorIf(tu>=CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);return pdesc->TextureUnit[tu].pTexture;}
inline NUVMATRIXANIM*			NGetAppearance_Raw_TextureMatrixAnim(NAPPEARANCE_RAW *pdesc,const Nu8 tu){NErrorIf(tu>=CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER,NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER);return pdesc->TextureUnit[tu].pUVMatrixAnim;}
inline NBLEND*					NGetAppearance_Raw_Blend(NAPPEARANCE_RAW *pdesc){return pdesc->pBlend;}
inline NMATERIAL*				NGetAppearance_Raw_Material(NAPPEARANCE_RAW *pdesc){return pdesc->pMaterial;}

Nbool							NCheckAppearanceRaw_vs_Appearance(const NAPPEARANCE_RAW *praw, const NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format, const NAPPEARANCE_CHECKMASK *pmask );

// ***************************************************************************************
// **					Appearance "Sub-Type" Functions									**
// ***************************************************************************************
NAPPEARANCE_TT*					NSetupAppearance_TT( NAPPEARANCE_TT *papp);
NAPPEARANCE_TT*					NCreateAppearance_TT();
void							NClearAppearance_TT(NAPPEARANCE_TT *ptr);
void							NDeleteAppearance_TT(NAPPEARANCE_TT *ptr);
void							NCopyAppearance_TT(NAPPEARANCE_TT *pdst, const NAPPEARANCE_TT *psrc);

NAPPEARANCE_TM*					NSetupAppearance_TM( NAPPEARANCE_TM *papp);
NAPPEARANCE_TM*					NCreateAppearance_TM();
void							NClearAppearance_TM(NAPPEARANCE_TM *ptr);
void							NDeleteAppearance_TM(NAPPEARANCE_TM *ptr);
void							NCopyAppearance_TM(NAPPEARANCE_TM *pdst, const NAPPEARANCE_TM *psrc);

NAPPEARANCE_ULTD_T*				NSetupAppearance_ULTD_T( NAPPEARANCE_ULTD_T *papp, const Nu8 textureunitnb);
NAPPEARANCE_ULTD_T*				NCreateAppearance_ULTD_T(const Nu8 textureunitnb);
void							NClearAppearance_ULTD_T(NAPPEARANCE_ULTD_T *ptr);
void							NDeleteAppearance_ULTD_T(NAPPEARANCE_ULTD_T *ptr);
void							NCopyAppearance_ULTD_T(NAPPEARANCE_ULTD_T *pdst, const NAPPEARANCE_ULTD_T *psrc);

NAPPEARANCE_ULTD_TM*			NSetupAppearance_ULTD_TM( NAPPEARANCE_ULTD_TM *papp, const Nu8 textureunitnb);
NAPPEARANCE_ULTD_TM*			NCreateAppearance_ULTD_TM(const Nu8 textureunitnb);
void							NClearAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *ptr);
void							NDeleteAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *ptr);
void							NCopyAppearance_ULTD_TM(NAPPEARANCE_ULTD_TM *pdst, const NAPPEARANCE_ULTD_TM *psrc);

NAPPEARANCE_TT*					NSetAppearance_TT( NAPPEARANCE_TT *papp, const NAPPEARANCE_RAW *praw);
NAPPEARANCE_TM*					NSetAppearance_TM( NAPPEARANCE_TM *papp, const NAPPEARANCE_RAW *praw);
NAPPEARANCE_ULTD_T*				NSetAppearance_ULTD_T( NAPPEARANCE_ULTD_T *papp, const NAPPEARANCE_RAW *praw);
NAPPEARANCE_ULTD_TM*			NSetAppearance_ULTD_TM( NAPPEARANCE_ULTD_TM *papp, const NAPPEARANCE_RAW *praw);

NAPPEARANCE_RAW*				NUnpackAppearance_TT(NAPPEARANCE_RAW *praw, const NAPPEARANCE_TT *ptt);
NAPPEARANCE_RAW*				NUnpackAppearance_TM(NAPPEARANCE_RAW *praw, const NAPPEARANCE_TM *ptm);
NAPPEARANCE_RAW*				NUnpackAppearance_ULTD_T(NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_T *pu);
NAPPEARANCE_RAW*				NUnpackAppearance_ULTD_TM(NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_TM *pu);

NTEXTURE*						NGetAppearance_TM_Texture(const NAPPEARANCE_TM *papp);
NTEXTURE*						NGetAppearance_TT_Texture(const NAPPEARANCE_TM *papp,const Nu8 texture_unit);
NTEXTURE*						NGetAppearance_ULTD_T_Texture(const NAPPEARANCE_ULTD_T *papp,const Nu8 texture_unit);
NTEXTURE*						NGetAppearance_ULTD_TM_Texture(const NAPPEARANCE_ULTD_TM *papp,const Nu8 texture_unit);

void							NSetAppearance_TT_Texture(NAPPEARANCE_TT *papp, const Nu8 texture_unit, const NTEXTURE *ptexture);
void							NSetAppearance_TM_Texture(NAPPEARANCE_TM *papp, const NTEXTURE *ptexture);
void							NSetAppearance_ULTD_T_Texture(NAPPEARANCE_ULTD_T *papp, const Nu8 texture_unit, const NTEXTURE *ptexture);
void							NSetAppearance_ULTD_TM_Texture(NAPPEARANCE_ULTD_TM *papp, const Nu8 texture_unit, const NTEXTURE *ptexture);

// + -------------------------+ Notes:
//								These 2x4 inline functions don't take care about Geometry capabilities (which may refuse Blend or/and Material..)
//								Prefer using 'NSetGeometryBLend/NGetGeometryBLend' and 'NSetGeometryMaterial/NGetGeometryMaterial'
//								to take care of them.	
inline void						NSetAppearance_TT_Blend(NAPPEARANCE_TT *papp, const NBLEND *pblend){papp->pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_TM_Blend(NAPPEARANCE_TM *papp, const NBLEND *pblend){papp->pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_ULTD_T_Blend(NAPPEARANCE_ULTD_T *papp, const NBLEND *pblend){papp->pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_ULTD_TM_Blend(NAPPEARANCE_ULTD_TM *papp, const NBLEND *pblend){papp->pBlend = (NBLEND*)pblend;}
inline void						NSetAppearance_TT_Material(NAPPEARANCE_TT *papp, const NMATERIAL *pmat){papp->pMaterial = (NMATERIAL*)pmat;}
inline void						NSetAppearance_TM_Material(NAPPEARANCE_TM *papp, const NMATERIAL *pmat){papp->pMaterial = (NMATERIAL*)pmat;}
inline void						NSetAppearance_ULTD_T_Material(NAPPEARANCE_ULTD_T *papp, const NMATERIAL *pmat){papp->pMaterial = (NMATERIAL*)pmat;}
inline void						NSetAppearance_ULTD_TM_Material(NAPPEARANCE_ULTD_TM *papp, const NMATERIAL *pmat){papp->pMaterial = (NMATERIAL*)pmat;}

// ***************************************************************************************
// **					Appearance "Raw & Sub-Type" Functions							**
// ***************************************************************************************
Nbool							NCheckAppearanceRaw_vs_Appearance_TT(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_TT *ptt, const NAPPEARANCE_CHECKMASK *pmask );
Nbool							NCheckAppearanceRaw_vs_Appearance_TM(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_TM *ptm, const NAPPEARANCE_CHECKMASK *pmask );
Nbool							NCheckAppearanceRaw_vs_Appearance_ULTD_T(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_T *ptultd_t, const NAPPEARANCE_CHECKMASK *pmask );
Nbool							NCheckAppearanceRaw_vs_Appearance_ULTD_TM(const NAPPEARANCE_RAW *praw, const NAPPEARANCE_ULTD_TM *ptultd_tm, const NAPPEARANCE_CHECKMASK *pmask );

// ***************************************************************************************
// **					Appearance "Specs" Functions									**
// ***************************************************************************************
NAPPEARANCE_SPECS*				NGetAppearanceSpecs(const NAPPEARANCE_FORMAT_ENUM nappearance_format);
Nu32							NGetAppearanceSpecsFlags(const NAPPEARANCE_FORMAT_ENUM nappearance_format);

// ***************************************************************************************
// **					Appearance "PRIVATE" Functions									**
// ***************************************************************************************
void							NInitializeAppearanceSpecs();
void							NDisableAppearanceSpecs();

// ***************************************************************************************
// **					Appearance Mask Functions										**
// ***************************************************************************************
NAPPEARANCE_CHECKMASK*			NSetupAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pappcheckmask, const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue, const Nu32 nbtexture_unit);
NAPPEARANCE_CHECKMASK*			NCreateAppearanceCheckMask(const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue, const Nu32 nbtexture_unit);
void							NClearAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask);
void							NDeleteAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask);


#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYCOMPONENT_APPEARANCE_H 

