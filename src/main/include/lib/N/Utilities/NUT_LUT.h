#ifndef __NUT_LUT_H
#define __NUT_LUT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_LUT.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "lib/N/NSpline.h"
#include "NUT_X.h" // To spline understanding

#ifdef __cplusplus
extern "C"
{
#endif
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **														GLOBAL and GENERIC DEFINEs																			**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	typedef enum
	{
		NUT_LUTBUILD_OPERAND_PUSH = 0,
		NUT_LUTBUILD_OPERAND_ADD,
		NUT_LUTBUILD_OPERAND_SUB,
		NUT_LUTBUILD_OPERAND_SCALE,
		NUT_LUTBUILD_OPERAND_AVERAGE,
		// ------------------------
		NUT_LUTBUILD_OPERAND_ENUM_SIZE
	} NUT_LUTBUILD_OPERAND_ENUM;

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								LUTComputing										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Nf32 LUT

// BLEND

// BUILD
/*
void			NUT_LUTBuild_Nf32FromConstant(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 constantvalue );
void			NUT_LUTBuild_Nf32FromFastRand(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 scale );
void			NUT_LUTBuild_Nf32FromFastRandEx(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 scale,const Nu32 key_every );
void			NUT_LUTBuild_Nf32FromSpline(Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, NSPLINE *pspline, const Nf32 scale );
// NUT_LUTBuild_Nf32Lerp should be called NUT_LUTBuild_Nf32Smear (etaler)
void			NUT_LUTBuild_Nf32Lerp(	Nf32 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const Nf32 valuea, const Nf32 valueb );

// PROCESS
void			NUT_LUTBuild_Nf32Clamp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax );
void			NUT_LUTBuild_Nf32Warp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax );
void			NUT_LUTBuild_Nf32Smooth( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NVEC3 LUT
void			NUT_LUTBuild_NVec3FromConstant( NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride,const NVEC3 *pvector );
void			NUT_LUTBuild_NVec3FromConstantf( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 x,const Nf32 y,const Nf32 z );


void			NUT_LUTBuild_NVec3FromFastRand( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NVEC3 *pscale );
void			NUT_LUTBuild_NVec3FromFastRandEx( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NVEC3 *pscale, const Nu32 key_every  );
void			NUT_LUTBuild_NVec3FromFastRandf( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand_fct,const Nu32 element_nb,const Nu32 stride,const Nf32 xscale,const Nf32 yscale,const Nf32 zscale );
void			NUT_LUTBuild_NVec3FromFastRandExf( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const Nf32 xscale,const Nf32 yscale,const Nf32 zscale, const Nu32 key_every  );

void			NUT_LUTBuild_NVec3FromSplines( NVEC3 *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NSPLINE *px_spline,const NSPLINE *py_spline,const NSPLINE *pz_spline );

void			NUT_LUTBuild_Vector3Lerp(	NVEC3 *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const NVEC3 * pva, const NVEC3 *pvb );
void			NUT_LUTBuild_NVec3Clamp( NVEC3 *plut,const Nu32 element_nb,const Nu32 stride, const NVEC3 *pvnewmin, const NVEC3 *pvnewmax );
void			NUT_LUTBuild_NVec3Warp( NVEC3 *plut,const Nu32 element_nb,const Nu32 stride, const NVEC3 *pvnewmin, const NVEC3 *pvnewmax );
void			NUT_LUTBuild_NVec3Smooth( NVEC3 *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NCOLOR LUT
void			NUT_LUTBuild_NColorFromConstant( NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand,  const Nu32 element_nb, const Nu32 stride, const NCOLOR *pcolor );
void			NUT_LUTBuild_NColorFromConstantf( NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb, const Nu32 stride, const Nf32 red, const Nf32 green, const Nf32 blue, const Nf32  alpha );

void			NUT_LUTBuild_NColorFromFastRand( NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NCOLOR *pscale );
void			NUT_LUTBuild_NColorFromFastRandEx( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NCOLOR *pscale, const Nu32 key_every  );
void			NUT_LUTBuild_NColorFromFastRandf( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand_fct,const Nu32 element_nb,const Nu32 stride,const Nf32 redscale,const Nf32 greenscale,const Nf32 bluescale,const Nf32 alphascale );
void			NUT_LUTBuild_NColorFromFastRandExf( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const Nf32 redscale,const Nf32 greenscale,const Nf32 bluescale,const Nf32 alphascale, const Nu32 key_every  );

void			NUT_LUTBuild_NColorFromSplines( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb,const Nu32 stride,const NSPLINE *pred_spline,const NSPLINE *pgreen_spline,const NSPLINE *pblue_spline,const NSPLINE *palpha_spline );

void			NUT_LUTBuild_NColorLerp(	NCOLOR *plut, const NUT_LUTBUILD_OPERAND_ENUM operand, const Nu32 element_nb,const Nu32 stride, const NCOLOR *pcola, const NCOLOR *pcolb );
void			NUT_LUTBuild_NColorClamp( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax );
void			NUT_LUTBuild_NColorWarp( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax );
void			NUT_LUTBuild_NColorSmooth( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );

// separate component
inline	void	NUT_LUTBuild_NColorConstantRed( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 red ){ NUT_LUTBuild_Nf32FromConstant( &plut->red,operand,element_nb,stride, red );}
inline	void	NUT_LUTBuild_NColorConstantGreen( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 green ){ NUT_LUTBuild_Nf32FromConstant( &plut->green,operand,element_nb,stride, green );}
inline	void	NUT_LUTBuild_NColorConstantBlue( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 blue ){ NUT_LUTBuild_Nf32FromConstant( &plut->blue,operand,element_nb,stride, blue );}
inline	void	NUT_LUTBuild_NColorConstantAlpha( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 alpha ){ NUT_LUTBuild_Nf32FromConstant( &plut->alpha,operand,element_nb,stride, alpha );}

inline	void	NUT_LUTBuild_NColorFastRandRed( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 redscale ){ NUT_LUTBuild_Nf32FromFastRand( &plut->red,operand,element_nb,stride, redscale );}
inline	void	NUT_LUTBuild_NColorFastRandRedEx( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 redscale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( &plut->red,operand,element_nb,stride, redscale,key_every );}
inline	void	NUT_LUTBuild_NColorFastRandGreen( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 greenscale ){ NUT_LUTBuild_Nf32FromFastRand( &plut->green,operand,element_nb,stride, greenscale );}
inline	void	NUT_LUTBuild_NColorFastRandGreenEx( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 greenscale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( &plut->green,operand,element_nb,stride, greenscale,key_every );}
inline	void	NUT_LUTBuild_NColorFastRandBlue( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 bluescale ){ NUT_LUTBuild_Nf32FromFastRand( &plut->blue,operand,element_nb,stride, bluescale);}
inline	void	NUT_LUTBuild_NColorFastRandBlueEx( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 bluescale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( &plut->blue,operand,element_nb,stride, bluescale,key_every );}
inline	void	NUT_LUTBuild_NColorFastRandAlpha( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 alphascale ){ NUT_LUTBuild_Nf32FromFastRand( &plut->alpha,operand,element_nb,stride, alphascale );}
inline	void	NUT_LUTBuild_NColorFastRandAlphaEx( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 alphascale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( &plut->alpha,operand,element_nb,stride, alphascale,key_every );}

void			NUT_LUTBuild_NColorSplineRed( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride,  const NSPLINE *pred_spline );
void			NUT_LUTBuild_NColorSplineGreen( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride,  const NSPLINE *pgreen_spline );
void			NUT_LUTBuild_NColorSplineBlue( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride,  const NSPLINE *pblue_spline );
void			NUT_LUTBuild_NColorSplineAlpha( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride,  const NSPLINE *palpha_spline );

inline	void	NUT_LUTBuild_NColorLerpRed( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 reda, const Nf32 redb ){ NUT_LUTBuild_Nf32Lerp( &plut->red,operand,element_nb,stride, reda,redb );}
inline	void	NUT_LUTBuild_NColorLerpGreen( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 greena, const Nf32 greenb ){ NUT_LUTBuild_Nf32Lerp( &plut->red,operand,element_nb,stride, greena,greenb );}
inline	void	NUT_LUTBuild_NColorLerpBlue( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 bluea, const Nf32 blueb ){ NUT_LUTBuild_Nf32Lerp( &plut->red,operand,element_nb,stride, bluea,blueb );}
inline	void	NUT_LUTBuild_NColorLerpAlpha( NCOLOR *plut,const NUT_LUTBUILD_OPERAND_ENUM operand,const Nu32 element_nb, const Nu32 stride, const Nf32 alphaa, const Nf32 alphab ){ NUT_LUTBuild_Nf32Lerp( &plut->red,operand,element_nb,stride, alphaa,alphab );}

*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_LUT_H
