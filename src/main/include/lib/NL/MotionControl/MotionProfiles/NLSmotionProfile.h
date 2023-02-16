#pragma once
#include "../../../N/NMemory.h"
#include "../../../N/NFlags.h"
#include "../../../N/NErrorHandling.h"
#include "../../../N/Miscellaneous/NColorPalette.h"

#include "../NLKin.h"
#include "../NLKinLimits.h"
#include "../Trajectory/NLVstageX.h"
#include "../MotionShapes/NLFMotionShape.h"
#include "../MotionShapes/NLSMotionShape.h"

//														+-------------------------+
//												   ----	.						  .	----	
//											   ---		.						  .		 ---
//											 -			.						  .			 -	
//											.			.						  .			   .	
//										   .			.						  .				.
//										  .				.						  .				 .
//										 .				.						  .				  .		
//										-				.						  .				   -
//									---					.						  .					 ---
//							   ----						.						  .						 ----
//	+------------------------+							.						  .							  +
//	|			 			 |							|						  |							  |
//	| FPRIM = FLAT-PRIME	 |		SUP = S-UP			|		FUP = FLAT-UP	  |		SDOWN = S-DOWN		  |

/** Liste de l'ensemble des Id de NLSMOTIONPROFILE.
*	Ces valeurs sont une asssociation des BIT 0 à 3.
*	Par exemple, ACCELERATION_OVERSHOOT|VELOCITY_WAS_OVERSHOOTED.
*/
typedef enum
{											///<	|	3	|	2	|	1	|	0	| 
											///<	+----------------------------------+
	EMPTY						= 0,		///<	|	0	|	0	|	0	|	0	|	--> no shape !
	SDOWN						= 0x1,		///<	|	0	|	0	|	0	|	1	|	--> utilise "m_sDown" 
	FLAT						= 0x2,		///<	|	0	|	0	|	1	|	0	|	--> utilise "m_fUp"
	FLAT_SDOWN					= 0x3,		///<	|	0	|	0	|	1	|	1	|	--> utilise "m_fUp"	et "m_sDown" 
	SUP							= 0x4,		///<	|	0	|	1	|	0	|	0	|	--> utilise "m_sUp"
	SUP_SDOWN					= 0x5,		///<	|	0	|	1	|	0	|	1	|	--> utilise "m_sUp"	et "m_sDown" 
	SUP_FLAT					= 0x6,		///<	|	0	|	1	|	1	|	0	|	--> utilise "m_sUp" et "m_fUp" 
	SUP_FLAT_SDOWN				= 0x7,		///<	|	0	|	1	|	1	|	1	|	--> utilise "m_sUp", "m_fUp" et "m_sDown" 
	//FLAT_SUP_SDOWN				= 0xD,	///<	|	1	|	1	|	0	|	1	|	--> utilise "m_fPrim", "m_sUp" et "m_sDown" 
	//FLAT_SUP_FLAT_SDOWN			= 0xF,	///<	|	1	|	1	|	1	|	1	|	--> utilise "m_fPrim", "m_sUp","m_fUp" et "m_sDown" 
											//		+----------------------------------+
											//		| FPRIM	|  SUP	| FUP	|SDOWN  |	
											//		\_______/
											//			|	 \_____________________/
											//		Extra shape			|
											//							|
											//						Basic Shapes 
}NLSMOTIONPROFILE_ID_ENUM;

#define MASK_NLSMOTIONPROFILE_ID_ALL_SHAPES			0xF
#define MASK_NLSMOTIONPROFILE_ID_BASIC_SHAPES		0x7
#define NLSMOTIONPROFILE_ID_BIT_SDOWN				BIT_0		// !!! DOIT ETRE EGAL A " NLSMOTIONSHAPE_ID_BIT_SDOWN "	 
#define NLSMOTIONPROFILE_ID_BIT_FUP					BIT_1			   	
#define NLSMOTIONPROFILE_ID_BIT_SUP					BIT_2		// !!! DOIT ETRE EGAL A " NLSMOTIONSHAPE_ID_BIT_SUP "
#define NLSMOTIONPROFILE_ID_BIT_FPRIM				BIT_3



#define FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION				BIT_0

typedef struct NLSMOTIONPROFILE	NLSMOTIONPROFILE;

// Rappel ( extrait de NLVStageXSlice.cpp line 543 et suivantes ... )
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
//		Nombre de Kins:	Pour rappel, la promotion d'un Motion profile en Kin ne créée pas le kin0 du motion profile... celui - ci étant considéré comme pré-existant !
//						Ainsi un motion profile de type SUP se décrit avec 4 kins :	
//						( mais seul 3 seront réellement créés )
//																			kin0 (			t0 ) ... La promotion ne crée pas ce premier Kin mais assume qu'il existe déjà.
//																			kin1 (kin0	+	tx )
//																			kin2 (kin1	+	tb ) ... n'existe que si tb > 0
//																			kin3 (kin2	+	tx )
//		
//											|		Nombre MINimum de Kin créés à la promotion		|		Nombre MAXimum de kin créés à la promotion		|
//											+	   ( ... dans le cas ou Tb(s) est/sont Null(s) )	+														+
//		Motion Profile	EMPTY				|							0							|							0							|
//		Motion Profile	SDOWN				|							2	...si Tb null			|							3							|
//		Motion Profile	FLAT				|							1							|							1							|
//		Motion Profile	FLAT_SDOWN			|							3	...si Tb null			|							4							|
//		Motion Profile	SUP					|							2	...si Tb null			|							3							|
//		Motion Profile	SUP_SDOWN			|							4	...si Tb(s) null(s)		|							6							|
//		Motion Profile	SUP_FLAT			|							3	...si Tb null			|							4							|
//		Motion Profile	SUP_FLAT_SDOWN		|							5	...si Tb(s) null(s)		|							7							|
#define  NLSMOTIONPROFILE_EMPTY__MAX_PROMOTED_KINS			0
#define  NLSMOTIONPROFILE_SDOWN__MAX_PROMOTED_KINS			3
#define  NLSMOTIONPROFILE_FLAT__MAX_PROMOTED_KINS			1
#define  NLSMOTIONPROFILE_FLAT_SDOWN__MAX_PROMOTED_KINS		4
#define  NLSMOTIONPROFILE_SUP__MAX_PROMOTED_KINS			3
#define  NLSMOTIONPROFILE_SUP_SDOWN__MAX_PROMOTED_KINS		6
#define  NLSMOTIONPROFILE_SUP_FLAT__MAX_PROMOTED_KINS		4
#define  NLSMOTIONPROFILE_SUP_FLAT_SDOWN__MAX_PROMOTED_KINS	7



// Fonctions callback de promotion en KIN
typedef Nu32(*NLSPROMOTEHANDLE)(NLSMOTIONPROFILE *psmp, NLKIN *pkinbuffer);
Nu32 promoteNull(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteFlat(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteSup(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteSupFlat(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteFlatSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteSupSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);
Nu32 promoteSupFlatSdown(NLSMOTIONPROFILE * psmp, NLKIN * pkbuff);

#ifdef _NEDITOR
// Fonctions callback de tracage direct ( n'utilise pas la promotion en Kin )
// NCOLORPICKPACK sera sollicité par les fonctions de tracage pour fournir 6 couleurs ( 0 à 5 ), il convient donc d'utiliser le format 4Bits (.. ou 2Bits )
// SMOTION PROFILE NCOLORPICKPACK index ventilation:
// NCOLORPICKPACK.#0....... Flat Motion Shape color					NCOLORPICKPACK.#3....... S Motion Shape color phase 3
// NCOLORPICKPACK.#1....... S Motion Shape color phase 1			NCOLORPICKPACK.#4....... S Motion Shape color 'Out'
// NCOLORPICKPACK.#2....... S Motion Shape color phase 2			NCOLORPICKPACK.#5....... F et S Motion Shape Text color
//
#define	DEFAULT_NLSMOTIONPROFILE_PICKPACK		NSTCPLT_4B_PCK(a,b,c,d,e,f)	// assume que la palette de NL2DOCS contient 16 couleurs.
typedef void(*NLSDRAWHANDLE)(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawNull(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawFlat(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawSup(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawSupFlat(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawFlatSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawSupSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
void drawSupFlatSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
#endif


struct NLSMOTIONPROFILE
{
	NLSPROMOTEHANDLE	ppromote;
#ifdef _NEDITOR
	NLSDRAWHANDLE		pdraw;
#endif

	Nu8					m_id;
	Nu8					m_flags;
	Nu16				m_availableNu16;

	Nf32				m_dt;// Durée totale du motion profile 'BRUT'.					|__ ! C'est à dire sans prise en compte de CutIn et CutOut
	Nf32				m_ds;// Distance totale couverte par le motion profile 'BRUT'.  | 

//	NLFMOTIONSHAPE		m_fPrime;
	NLSMOTIONSHAPE		m_sUp;
	NLFMOTIONSHAPE		m_fUp;
	NLSMOTIONSHAPE		m_sDown;

	//Nf32				m_t0Offset;
	NLSCUTINOFFSETS		m_cutInOffsets;
	NLSCUTOUTOFFSETS	m_cutOutOffsets;

	inline void initialize()
	{
		Nmem0(this, NLSMOTIONPROFILE); m_dt = NF32_MAX; ppromote = promoteNull;
#ifdef _NEDITOR 
		pdraw = drawNull;
#endif
	};

	inline Nf32 getEffectiveTimeSpan() const { return m_dt + m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt; NErrorIf(m_cutInOffsets.m_dt > 0.0, NERROR_SYSTEM_CHECK); NErrorIf(m_cutOutOffsets.m_dt > 0.0, NERROR_SYSTEM_CHECK);}; // = m_dt auquel on enlève les cutin et cutout ( les valeurs de cutin et cutout sont tjrs négatives) 
	inline Nf32 getEffectiveLength() const { return m_ds + m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds; };   // = m_ds auquel on enlève les cutin et cutout ( les valeurs de cutin et cutout sont tjrs négatives) 


	void flat(const Nf32 s0, const Nf32 s1, const Nf32 v);
	void sUp(const NLSMOTIONSHAPE * psshape_up, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets);
	void sDown(const NLSMOTIONSHAPE * psshape_down, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets);
	void sUpFlat(const NLSMOTIONSHAPE * psshape_up, const NLSCUTINOFFSETS * pcut_in_offsets, const Nf32 fshape_ds);
	void flatSdown(const NLSMOTIONSHAPE * psshape_down, const NLSCUTOUTOFFSETS * pcut_out_offsets, const Nf32 fshape_ds);
	
	NLSMOTIONPROFILE_ID_ENUM sRamped(const NLSMOTIONSHAPE * psshape_up, const NLSMOTIONSHAPE * psshape_down, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets);
	NLSMOTIONPROFILE_ID_ENUM sRamped(const NLKINLIMITS * pkmax, const Nf32 v0, const Nf32 v1, const Nf32 s0, const Nf32 s1, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets);
	NLSMOTIONPROFILE_ID_ENUM sRamped(const Nf32 vmax, const NLVSTAGEX *pv0, const NLVSTAGEX *pv1);
	
	Nu32 detectStageVelocityOvershoot(NLVSTAGEX *pv0, NLVSTAGEX *pv1, const Nu32 no_collision_flags);

//inline void setId(const NLSMOTIONPROFILE_ID_ENUM id) { m_id = (m_id&(~MASK_NLSMOTIONPROFILE_ID_BASIC_SHAPES)) | id; }
//inline void setFullId(const NLSMOTIONPROFILE_ID_ENUM id) { m_id =  id; }

};
#define NLclearSmotionProfileInArrayCallBack			NULL	
//inline void NLclearSmotionProfileInArrayCallBack(void* p){}

