#ifndef __NPARTICLECOMPONENT_LUT_H
#define __NPARTICLECOMPONENT_LUT_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NParticleComponent_LUT.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../Miscellaneous/NColor.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + CONSTANT and DEFAULT VALUES
																									// Std	 : NPARTICLE
																									// Adv	 : NADVANCEDPARTICLE
																									// H.Std : NHOMINGPARTICLE
																									// H.Adv : NADVANCEDHOMINGPARTICLE
#define CONSTANT_NPARTICLE_EMITTERCOMPONENT_LUT_SIZEMAX					NU16_MAX					//
// 4 vertex by particle and AT maximum NU16_MAX indexed vertex (OPENGL ES limitation)				//  ----- ----- ----- ----- ----- -----			
																									// | Std | Adv |H.Std|H.Adv|     |     |
																									//  ----- ----- ----- ----- ----- -----			
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_SIZE					64							// |  O  |  O  |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_SCALE				0.5f						// |  O  |  O  |  O	 |  O  |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_COLOR_4F				0.5f,0.5f,0.5f,1.0f			// |  O  |  O  |  O	 |  O  |     |     |

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + DESCRIPTION FLAGS
//																						// | Std | Adv |H.Std|H.Adv|     |     |
//																						//  ----- ----- ----- ----- ----- -----	
#define FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_SIZE			BIT_0			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_INITIAL		BIT_1			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_ALL				0x3
// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// A Particle LUT Element
typedef struct
{
	Nf32		Scale;		// Particle Scale LUT element
	NCOLOR		Color;		// Particle Color LUT element
}NPARTICLELUT;

typedef	struct 
{
	Nu32			Flags;
	Nu16			LUT_Size;
	NPARTICLELUT	LUT_Initial;
}NPARTICLE_EMITTERCOMPONENT_LUT_DESC;

typedef	struct 
{
	NPARTICLELUT	*pLUT;
	Nu16			LUT_Size;
}NPARTICLE_EMITTERCOMPONENT_LUT;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
inline void	NSetupParticleEmitterComponent_LUT_Desc_LUTSize(NPARTICLE_EMITTERCOMPONENT_LUT_DESC *pdesc,const Nu16 lutsize ){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_SIZE);pdesc->LUT_Size = lutsize;}
inline void	NSetupParticleEmitterComponent_LUT_Desc_LUTInitial(NPARTICLE_EMITTERCOMPONENT_LUT_DESC *pdesc,const Nf32 scale,const NCOLOR *pc ){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_INITIAL);pdesc->LUT_Initial.Scale=scale;pdesc->LUT_Initial.Color = *pc;}
inline void	NSetupParticleEmitterComponent_LUT_Desc_LUTInitialf(NPARTICLE_EMITTERCOMPONENT_LUT_DESC *pdesc,const Nf32 scale,const Nf32 red,const Nf32 green,const Nf32 blue,const Nf32 alpha  ){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_INITIAL);pdesc->LUT_Initial.Scale=scale;pdesc->LUT_Initial.Color.red=red;pdesc->LUT_Initial.Color.green=green;pdesc->LUT_Initial.Color.blue=blue;pdesc->LUT_Initial.Color.alpha=alpha;}

// ***************************************************************************************
// **				LOW LEVEL:PRIVATE Inline Setup & Clear Functions					**
// ***************************************************************************************
inline void NSetupParticlesEmitterComponent_LUT( NPARTICLE_EMITTERCOMPONENT_LUT *pemittercomponent,const NPARTICLE_EMITTERCOMPONENT_LUT_DESC *pdesc )
{
	NPARTICLELUT	*plut;
	NPARTICLELUT	lut0;
	Nu16			i;

	// Lut_Size
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_SIZE) )
	{
		pemittercomponent->LUT_Size = NCLAMP(1,pdesc->LUT_Size,CONSTANT_NPARTICLE_EMITTERCOMPONENT_LUT_SIZEMAX);
	}
	else
	{
		pemittercomponent->LUT_Size = NCLAMP(1,DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_SIZE,CONSTANT_NPARTICLE_EMITTERCOMPONENT_LUT_SIZEMAX);
	}

	// LUT initial element
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_LUT_DESC_USE_LUT_INITIAL) )
	{
		lut0 = pdesc->LUT_Initial;
	}
	else
	{
		lut0.Scale = DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_SCALE;
		NSetColorf(&lut0.Color,DEFAULT_NPARTICLE_EMITTERCOMPONENT_LUT_COLOR_4F);
	}
	// LUT
	plut = pemittercomponent->pLUT = (NPARTICLELUT*)Nmalloc( pemittercomponent->LUT_Size * sizeof(NPARTICLELUT) );
	for(i=pemittercomponent->LUT_Size;i!=0;i--,plut++)
	{
		*plut = lut0;
	}
}
inline void NClearParticlesEmitterComponent_LUT( NPARTICLE_EMITTERCOMPONENT_LUT *pemittercomponent )
{
	Nfree(pemittercomponent->pLUT);
	Nmem0(pemittercomponent,NPARTICLE_EMITTERCOMPONENT_LUT);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NPARTICLECOMPONENT_LUT_H 

