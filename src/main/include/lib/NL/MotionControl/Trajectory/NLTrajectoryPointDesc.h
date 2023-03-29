#pragma once
#include "../../../N/NType.h"
#include "../../../N/NMemory.h"

#include "../NLKIN.h"
#include "../Path/Geometry/NLPathGeometry.h"

// *****************************************************************************************************************************************************
// *	
// * Flags de configuration ( Attention ces 4 premiers flags sont communs avec ceux de NLTRJACTION ) 
// *
#define FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING					BIT_0																
#define FLAG_NLTRJPOINT_DESC_KTYPE_SPOT							BIT_1														
#define FLAG_NLTRJPOINT_DESC_SET_DTMODE							BIT_2														
#define FLAG_NLTRJPOINT_DESC_DTMODE_REVERSE						BIT_3														

// * Combinaisons de flags
#define FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING					FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING																
#define FLAGS_NLTRJPOINT_DESC_KTYPE_SPOT						FLAG_NLTRJPOINT_DESC_KTYPE_SPOT														
#define FLAGS_NLTRJPOINT_DESC_KTYPE_BACK_AND_FORTH				(FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING|FLAG_NLTRJPOINT_DESC_KTYPE_SPOT)														

#define FLAGS_NLTRJPOINT_DESC_SET_DTMODE_FORWARD				FLAG_NLTRJPOINT_DESC_SET_DTMODE												//| ..._SET_DTMODE OFF + ..._DTMODE_REVERSE OFF ( signifie DT_MODE = 0) n'est pas possible 
#define FLAGS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE				(FLAG_NLTRJPOINT_DESC_SET_DTMODE|FLAG_NLTRJPOINT_DESC_DTMODE_REVERSE)		//| ..._SET_DTMODE


// * Mask de configuration
#define MASK_NLTRJPOINT_DESC_KTYPE								(BIT_0|BIT_1)												
#define MASK_NLTRJPOINT_DESC_DTMODE								(BIT_2|BIT_3)												
#define MASK_NLTRJPOINT_DESC_CONFIG								(MASK_NLTRJPOINT_DESC_KTYPE|MASK_NLTRJPOINT_DESC_DTMODE)	

#define IS_NLTRJPOINT_DESC_NULL_DTMODE(flags)					(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_DTMODE,0))
#define IS_NLTRJPOINT_DESC_SET_DTMODE(flags)					ISFLAG_ON(flags,FLAG_NLTRJPOINT_DESC_SET_DTMODE)
#define IS_NLTRJPOINT_DESC_SET_DTMODE_FORWARD(flags)			(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_DTMODE,FLAG_NLTRJPOINT_DESC_SET_DTMODE))
#define IS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE(flags)			(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_DTMODE,FLAG_NLTRJPOINT_DESC_SET_DTMODE|FLAG_NLTRJPOINT_DESC_DTMODE_REVERSE))

#define IS_NLTRJPOINT_DESC_KTYPE_NULL(flags)					(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,0))
#define IS_NLTRJPOINT_DESC_KTYPE_TRAVELING(flags)				(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING))
#define IS_NLTRJPOINT_DESC_KTYPE_SPOT(flags)					(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
#define IS_NLTRJPOINT_DESC_KTYPE_BACK_AND_FORTH(flags)			(FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_SPOT|FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING))

#define GET_NLTRJPOINT_DESC_DT_MODE(flags)						((flags)&MASK_NLTRJPOINT_DESC_DTMODE)
#define GET_NLTRJPOINT_DESC_KTYPE(flags)						((flags)&MASK_NLTRJPOINT_DESC_KTYPE)
#define GET_NLTRJPOINT_DESC_CONFIG(flags)						((flags)&MASK_NLTRJPOINT_DESC_CONFIG)

#define SET_AS_NLTRAJECTORY_POINT_DESC_TRAVELING(flags)			SET_FLAGS(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING)
#define SET_AS_NLTRAJECTORY_POINT_DESC_SPOT(flags)				SET_FLAGS(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_SPOT)
#define SET_AS_NLTRAJECTORY_POINT_DESC_BACK_AND_FORTH(flags)	SET_FLAGS(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING|FLAG_NLTRJPOINT_DESC_KTYPE_SPOT)

#define IS_NLTRAJECTORY_POINT_DESC_TRAVELING(flags)				FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING)
#define IS_NLTRAJECTORY_POINT_DESC_SPOT(flags)					FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_SPOT)
#define IS_NLTRAJECTORY_POINT_DESC_BACK_AND_FORTH(flags)		FLAGS_TEST(flags,MASK_NLTRJPOINT_DESC_KTYPE,FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING|FLAG_NLTRJPOINT_DESC_KTYPE_SPOT)

#define DEFAULT_KTYPE											FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING		// soit le KTYPE traveling
#define DEFAULT_DTMODE											FLAGS_NLTRJPOINT_DESC_SET_DTMODE_FORWARD	// soit le DT Mode FORWARD
// *
// *****************************************************************************************************************************************************
/*
 *  ________________________________________________________________________________________________
 * |																							    |
 * |									NLTRJPOINT_DESC_CFG										    |
 * |________________________________________________________________________________________________|
 * |___________|___________|___________|___________||___________|___________|___________|___________|
 * |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16||15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 * |											   ||								 |i |     |		|
 * |											   ||								 |s | DT  |KType|
 * |			First pushed KIN Index			   ||		AVAILABLE				 |K |Mode |     |
 * |											   ||								 |i |	  |     |
 * |											   ||								 |n |	  |     |
 * |_______________________________________________||_______________________________ |__|_____|_____|
 * |											   ||								 |  |	  |		|
 * |			16 Bits value. [0,65535]		   ||	11 Bits value. [0,2047]		 |1b|2bits|2bits|
 * |_______________________________________________||________________________________|__|_____|_____|
 */
typedef Nu32	NLTRJPOINT_DESC_CFG;
//#define CONSTANT_MAX_PUSHED_KIN_INDEX			65535	// car valeur codée sur 16 bits dans NLTRJPOINT_DESC_CFG ( cette constante est utilisée dans les tests de débordement )

// MAKE_NLTRJPOINT_DESC_CFG(kidx,iskin,cfg)
// ------------------------------------------------------------------------------------------
/**
 *	@brief	MAKE_NLTRJPOINT_DESC_CFG construit la valeur 32 bits de retour des fonctions "promoteAction" et "NLTRJKEY::promote"
 *			Cette valeur 32 bits permet au processus de promotion de passer des informations entre les différents "étages" en jeu:
 *
 *									TRAJECTORY / PACK
 *										 |
 *										 |_____KEY(s)
 *												|
 *												|___ACTION(s)
 *
 *  @param	kidx	[Kin Index]........	est l'index du premier KIN créé par la promotion et "pushé" dans l'array de construction des kins.
 *	@param	iskin	[Is Kin Left]......	est un boleen indiquant si la valeur kidx est à prendre en compte.
 *										TRUE signifie que kidx est valide. FALSE signifie qu'il n'y a pas de KIN à traiter.
 *	@param	actioncfg [Action Config]..	est une valeur dont seuls les 4 premiers BITS seront pris en compte. (BIT_0 to BIT_3 inclus)
 *										Sur ces 4 BITS sont codés le KTYPE (BIT_0 et BIT_1) et le DT_MODE ( BIT_2 et BIT_3)
 */

#define MAKE_NLTRJPOINT_DESC_CFG(kidx,iskin,cfg)		((((kidx)&0xFFFF)<<16)|(((iskin)&0x1)<<4)|((cfg)&0xF))	
#define IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(r)			(((r)>>4)&0x1)			
#define GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(r)		(((r)>>16)&0xFFFF)
#define GET_NLTRJPOINT_DESC_CFG_CONFIG(r)				((cfg)&0xF)


typedef struct NLTRJPOINT_DESC NLTRJPOINT_DESC;
struct NLTRJPOINT_DESC
{
	void set(Nu32 flags, NLKIN* pkin, NLPATH_POINT* pkp1, NLPATH_PRIMITIVE* ppr) { m_flags = flags;  m_kin = *pkin; m_pPathPoint1 = pkp1; m_pPrimitive = ppr; }
	inline void	null() { Nmem0(this, NLTRJPOINT_DESC); }
#ifdef _NEDITOR
	void draw(NL2DOCS* p2docs,  const NCOLORPICKPACK pickpack, const NLTRJPOINT_DESC* p0);
#endif
	NLKIN				m_kin;			// kin représentant le robot ( son centre d'inertie )
	
	Nu32				m_flags;
	NLPATH_POINT	   *m_pPathPoint1;	// |_ portion du path où se trouve ce "Trajectory Point Desc"
	NLPATH_PRIMITIVE   *m_pPrimitive;	// |
};

#define NLclearNLTrajectoryPointDescInArrayCallBack		NULL
//inline void NLclearNLTrajectoryPointDescInArrayCallBack(void* p){};

typedef struct NLTRJPOINT_DESC_HEADER NLTRJPOINT_DESC_HEADER;
struct NLTRJPOINT_DESC_HEADER
{
	NLKIN	m_kin;
	Nu32	m_flags;
	Nu32	m_KeyPoint1Idx;
	Nu32	m_PrimitiveIdx;
};
