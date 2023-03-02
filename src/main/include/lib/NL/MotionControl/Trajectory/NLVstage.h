#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "NLVstageX.h"


#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#include "../Path/NLPath.h"
#endif

/*
#define FLAG_NLVSTAGE_FORCE_CUT_OUT		BIT_0	// La fonction trim n'utilise un NLVSTAGE en tant que 'cutters' que si celui-ci à une vitesse m_v inférieure à celle du NLVSTAGE à 'trimer'
												// Pour forcer la découpe même si m_v est supérieure ou égale il suffit de flagger le NLVSTAGE cutters ou le NLVSTAGE à couper
												// Si c'est le cutters qui est flaggé alors il coupera TOUJOURS et TOUT ... ce qu'il "touche"
												// Si c'est le NLVSTAGE à trimer/couper, qui est flaggé, alors seul sa découpe sera forcée ( bien sûr si il "touche" le cutters )
												// Notez que le CUTTER doit être munis de cutters verticaux !!! sinon CRASH volontaire ...
*/

#define FLAG_CUTOUT_LEFT			BIT_0	// représente tout ou partie du Vstage situé à GAUCHE de l'emporte piece de decoupe
#define FLAG_CUTOUT_COOKIE			BIT_1	// représente tout ou partie du Vstage situé 'à l'intérieur' l'emporte piece de decoupe
#define FLAG_CUTOUT_RIGHT			BIT_2	// représente tout ou partie du Vstage situé à DROITE de l'emporte piece de decoupe
#define FLAG_CUTOUT_CUT				BIT_3	// indique si une coupe à réellement eut lieu ( RESULT ONLY )
#define FLAG_CUTOUT_FORCE			BIT_4	// indique si une coupe à réellement eut lieu ( CONFIG ONLY )

#define MAKE_CUTOUT_CFG(nocut_default_result,mode)		( ((mode)&0x17)|(((nocut_default_result)&0x7)<<5))
#define GET_CUTOUT_CFG_EXPECTED_DEFAULT_RESULT(cfg)		(((cfg)>>5)&0x7)
#define GET_CUTOUT_CFG_MODE(cfg)						( (cfg)&0x17))

#define CUTOUT_CFG_REMOVE_COOKIE					MAKE_CUTOUT_CFG(FLAG_CUTOUT_RIGHT,FLAG_CUTOUT_LEFT|FLAG_CUTOUT_RIGHT)	
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	FLAGS de configuration de la fonction cutOut 
// |	La configuration doit être construite avec 'MAKE_CUTOUT_CFG'
//																		+---- BIT_3 reste volontairement inutilisé par CUTOUT_CFG
//																		|	
//																		.
//									|	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	| 
//					   				+-----------------------+-------+-------+-----------------------+
//									|  LEFT	|COOKIE	| RIGHT	| FORCE	|	X	|  LEFT	|COOKIE	| RIGHT |	
//									\______________________/ \_____/ \_____/ \_____________________/
//												|				|							|
//												| 				|							|
//							'nocut_default_result'				'Force'						'push mask'
//					   Que faire et que retourner				Faut-il forcer la			Parties à 'pusher' sur l'array 
//					      En cas de "non-coupe" ?				découpe ?					(si elles existent après découpe)
//	   Ces 3 bits sont systématiquement retournés				Normalement celle-ci n'est
//	  En cas de non coupe. Si au moins un d'entre				validée que si le membre 'm_v'
//	 eux est TRUE alors la totalité du VStage est				du cookiecutter est inférieur au membre 'm_v'
//	"pushée" sur l'array, sinon rien.( tous à FALSE )			du VStage. Si 'Force' est à TRUE, la découpe sera
//																toujours validée et tentée !
//
//								   \______________________/ \_____________________________________/
//												|								 |			
//									 'nocut_default_result' 	              'mode'	 			
// |
// +-------------------------------------------------------------------------------------------------------------------

#define IS_CUTOUT_RESULT_LEFT(cutout_result)	( (cutout_result)&FLAG_CUTOUT_LEFT )
#define IS_CUTOUT_RESULT_RIGHT(cutout_result)	( (cutout_result)&FLAG_CUTOUT_RIGHT )
#define IS_CUTOUT_RESULT_COOKIE(cutout_result)	( (cutout_result)&FLAG_CUTOUT_COOKIE )
#define IS_CUTOUT_RESULT_CUTTED(cutout_result)	( (cutout_result)&FLAG_CUTOUT_CUT )

#define IS_CUTOUT_RESULT_CUTTED_LEFT(cutout_result)		( (cutout_result)&(FLAG_CUTOUT_CUT|FLAG_CUTOUT_LEFT)==(FLAG_CUTOUT_CUT|FLAG_CUTOUT_LEFT) )
#define IS_CUTOUT_RESULT_CUTTED_RIGHT(cutout_result)	( (cutout_result)&(FLAG_CUTOUT_CUT|FLAG_CUTOUT_RIGHT)==(FLAG_CUTOUT_CUT|FLAG_CUTOUT_RIGHT) )
#define IS_CUTOUT_RESULT_CUTTED_COOKIE(cutout_result)	( (cutout_result)&(FLAG_CUTOUT_CUT|FLAG_CUTOUT_COOKIE)==(FLAG_CUTOUT_CUT|FLAG_CUTOUT_COOKIE) )
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	Valeur de retour de la fonction cutOut après découpe
// |	Cette valeur peut-être analysée directement via les FLAG_CUTOUT_xx ou à l'aide des macros IS_CUTOUT_RESULT_xx
//	
//	
//												|	3	|	2	|	1	|	0	| 
//												+-------------------------------+
//												| CUT ?	|  LEFT	|COOKIE	| RIGHT |	
//												 \_____/ \_____________________/
//													|				|			
//													|	 			|
//												cut ?				Part(s) pushée(s) sur l'output array.
//	Si TRUE une découpe a bien été validée et tentée.				Notez que dans le cas où CUT ( BIT_3 ) est FALSE : 
//																	Soit la valeur des BITs 0 à 2 est FALSE également et alors
//		   Si FALSE il n'y a pas eut de validation de				rien n'est pushé sur l'array.
//  découpe. Le comportement par défaut (push or not)				Soit au moins 1 des BIT est à TRUE et LA TOTALITE de Vstage est
// à été appliqué et la valeur de retour initialement				alors pushée sur Array.
//				 passée en parametre a été retournée.
//
// |
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	Quelques exemples de valeur de retour:
// |
//						|	3	|	2	|	1	|	0	| 
//						+----------------------------------+
//	NOTHING				|	1	|	0	|	0	|	0	|	--> Il y a eut tentative de découpe, mais rien n'est 'pushé' dans l'array de sortie.
//																Cela est du aux flags de restitution ne demandant pas à restituer les parties
//																effectivement coupées.	
//
//	RIGHT				|	1	|	0	|	0	|	1	|	--> Il y a eut tentative de découpe, seule la partie dépassant à droite de l'emporte piece
//																a été 'pushée' dans l'array de sortie. 
//																Les autres parties découpées, si elles existent n'ont pas été pushées soit parceque la
//																configuration de découpe ne le demandait pas, soit parceque la totalité du VStage se trouve
//																à droite du 'cookie cutter'
//
//	COOKIE				|	1	|	0	|	1	|	0	|	--> Il y a eut tentative de découpe, seule la partie découpée par l'emporte piece a été 'pushée'
//																dans l'array de sortie.
//																Les autres parties découpées, si elles existent n'ont pas été pushées soit parceque la
//																configuration de découpe ne le demandait pas, soit parceque la totalité du VStage se trouve
//																à l'intérieur du 'cookie cutter'

//	LEFT_COOKIE_RIGHT	|	1	|	1	|	1	|	1	|	--> Il y a eut tentative de découpe, toutes les parties découpées ont été 'pushées' 
//																dans l'array de sortie. La configuration a demandé à ce que cela soit ainsi fait et on 
//																se retrouve donc bien avec 3 parties consécutives et distinctes 'pushées' dans l'array.
//
//	UNCUT				|	0	|	1	|	0	|	0	|	--> Il n'y a eut aucune découpe car les tests de validité de coupe ont échoués.
//																La valeur de retour par défaut passée en configuration est retournée ( BIT_2 ). Celle-ci
//																étant non nulle ( au moins 1 des 3 BITS est TRUE ) la totalité du Vstage a été 'pushée' 
//																sur l'array de sortie. 
//
//	UNCUT_BUT_NOTHING	|	0	|	0	|	0	|	0	|	--> Il n'y a eut aucune découpe car les tests de validité de coupe ont échoués.
//																La valeur de retour par défaut passée en configuration est retournée ( 0 ). Celle-ci
//																étant nulle ( tous les BITS sont FALSE ) rien n'a été 'pushé' sur l'array de sortie. 
typedef struct NLVSTAGE NLVSTAGE;
struct NLVSTAGE
{
	Nf32			m_v;		// Vitesse limite associée
	Nf32			m_s0;		// abscisse de 'départ' de la clef ( position à partir de laquelle la clef à de l'effet )
	Nf32			m_s1;		// abscisse de 'fin' de la clef ( position à partir de laquelle la clef n'a plus d'effet )

	Nf32			m_value;	// 
	Nu16			m_type;		// type pour définir comment interpréter 'm_value' et, en fonction,  calculer 'm_v'.
	Nu16			m_flags;

	enum TYPE
	{
		FORCED = 0,		// 'm_value' est 'bypassée' par un 'force', ou simplement pas encore settée
		RELATIVE_VALUE,	// 'm_value' représente le % [0,1] de la vitesse max utilisée sur la courbe ( = % de la vitesse de croisière )
		ABSOLUTE_VALUE	// 'm_value' représente la vitesse maximum autorisée à partir de cette clef.
	};
	NLVSTAGE() :m_type(FORCED), m_value(0), m_s0(0), m_s1(0), m_v(0.0f), m_flags(0) { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.f; };
	NLVSTAGE( const Nf32 s0, const Nf32 s1, const Nf32 val = 0.0f, const TYPE type = FORCED):m_type(type), m_value(val), m_s0(s0), m_s1(s1),m_v(type==FORCED ? val:0.0f),m_flags(0) { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.f; NErrorIf(val < 0.0f, NERROR_VALUE_OUTOFRANGE); NErrorIf((type == RELATIVE_VALUE) && (val > 1.0f), NERROR_VALUE_OUTOFRANGE);};

	void set(const Nf32 s0, const Nf32 s1, const Nf32 val = 1.0f, const TYPE type = RELATIVE_VALUE);

	// Pour les 2 méthodes qui suivent, on remarquera:
	//				m_v = -NF32_MAX
	//				... Cela pour générer des erreurs/aberrations plus loin et obliger à définir les NLVSTAGE correctement avec appel au moment approprié à la méthode "computeAdjustedVelocity" 
	void setRelativeValue(Nf32 val) { m_v = -NF32_MAX;  m_value = val; m_type = (Nu16)RELATIVE_VALUE; NErrorIf(val <= 0.0f, NERROR_VALUE_OUTOFRANGE); NErrorIf(val > 1.0f, NERROR_VALUE_OUTOFRANGE); };
	void setAbsoluteValue(Nf32 val) { m_v = -NF32_MAX;  m_value = val; m_type = (Nu16)ABSOLUTE_VALUE; NErrorIf(val <= 0.0f, NERROR_VALUE_OUTOFRANGE); };

	void computeAdjustedVelocity(const Nf32 velocity_max);


#ifdef _NEDITOR
	void draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
	void draw(NLPATH* ppath, const Nf32 width, const NCOLORPICKPACK pickpack);
#endif
	friend class NLPATH;
	friend class NLTRAJECTORY;

	enum AGGREGATE_FLAGS
	{
		NONE = 0,			// Aucune action particulière, le NLVSTAGE est ajouté à la liste, si sa vitesse est identique à celle du dernier de la liste avant insertion, alors il le prolonge plutôt que de s'ajouter.
		PRESERVE_GAP = BIT_0,		// Il n'y aura pas de prolongement(=fusion) si il existe un "vide" entre ce Vstage et le dernier de la liste d'insertion
		PRESERVE_JOINT = BIT_1,		// Il n'y aura pas de prolongement(=fusion) si il y a un "joint" entre ce Vstage et le dernier de la liste d'insertion ( joint veut dire " last.m_s1 = inserted.m_s0 )
		PRESERVE_GAP_AND_JOINT = BIT_0 | BIT_1	// Il n'y aura pas de prolongement(=fusion) si il y a un "joint" entre ce Vstage et le dernier de la liste d'insertion ( joint veut dire " last.m_s1 = inserted.m_s0 )

	};
	void aggregate(NARRAY* poutput_array, const AGGREGATE_FLAGS flags = NONE);
	Nu32 cutOut(NARRAY* poutput_array, const NLVSTAGE* pcookiecutter, const Nu32 cfg = CUTOUT_CFG_REMOVE_COOKIE);
	//inline void trimCutForced(const Nbool bforce) { if (bforce)FLAG_ON(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); else FLAG_OFF(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); }
	void setCutters(const NLVSTAGE* p0, const NLVSTAGE* p1);
	void setCutters(const Nf32 cutter0_s, const Nf32 cutter0_v, const Nf32 cutter1_s, const Nf32 cutter1_v);
	void resetCutters() { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.0f; };
	//void resetCutters(const Nbool bforce = NFALSE) { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.0f; if (bforce)FLAG_ON(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); else FLAG_OFF(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT);};
	void force(const Nf32 s0, const Nf32 s1, const Nf32 val) { m_s0 = s0; m_s1 = s1; m_value = val; m_v = val;  m_type = (Nu16)FORCED; NErrorIf(val < 0.0f, NERROR_VALUE_OUTOFRANGE); };

private:
	NVEC2			m_cutter0;
	NVEC2			m_cutter1;
};
#define NLclearVStageInArrayCallBack			NULL	
//inline void NLclearVStageInArrayCallBack(void* p){}