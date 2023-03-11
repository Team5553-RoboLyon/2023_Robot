#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageX.h"

#ifdef _NEDITOR
#include "lib/NL/MotionControl/NL2DOrthogonalCoordinateSystem.h"
#include "lib/NL/MotionControl/Path/NLPath.h"
#endif

/*
#define FLAG_NLVSTAGE_FORCE_CUT_OUT		BIT_0	// La fonction trim n'utilise un NLVSTAGE en tant que 'cutters' que si celui-ci � une vitesse m_v inf�rieure � celle du NLVSTAGE � 'trimer'
												// Pour forcer la d�coupe m�me si m_v est sup�rieure ou �gale il suffit de flagger le NLVSTAGE cutters ou le NLVSTAGE � couper
												// Si c'est le cutters qui est flagg� alors il coupera TOUJOURS et TOUT ... ce qu'il "touche"
												// Si c'est le NLVSTAGE � trimer/couper, qui est flagg�, alors seul sa d�coupe sera forc�e ( bien s�r si il "touche" le cutters )
												// Notez que le CUTTER doit �tre munis de cutters verticaux !!! sinon CRASH volontaire ...
*/

#define FLAG_CUTOUT_LEFT BIT_0	 // repr�sente tout ou partie du Vstage situ� � GAUCHE de l'emporte piece de decoupe
#define FLAG_CUTOUT_COOKIE BIT_1 // repr�sente tout ou partie du Vstage situ� '� l'int�rieur' l'emporte piece de decoupe
#define FLAG_CUTOUT_RIGHT BIT_2	 // repr�sente tout ou partie du Vstage situ� � DROITE de l'emporte piece de decoupe
#define FLAG_CUTOUT_CUT BIT_3	 // indique si une coupe � r�ellement eut lieu ( RESULT ONLY )
#define FLAG_CUTOUT_FORCE BIT_4	 // indique si une coupe � r�ellement eut lieu ( CONFIG ONLY )

#define MAKE_CUTOUT_CFG(nocut_default_result, mode) (((mode)&0x17) | (((nocut_default_result)&0x7) << 5))
#define GET_CUTOUT_CFG_EXPECTED_DEFAULT_RESULT(cfg) (((cfg) >> 5) & 0x7)
#define GET_CUTOUT_CFG_MODE(cfg)						( (cfg)&0x17))

#define CUTOUT_CFG_REMOVE_COOKIE MAKE_CUTOUT_CFG(FLAG_CUTOUT_RIGHT, FLAG_CUTOUT_LEFT | FLAG_CUTOUT_RIGHT)
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	FLAGS de configuration de la fonction cutOut
// |	La configuration doit �tre construite avec 'MAKE_CUTOUT_CFG'
//																		+---- BIT_3 reste volontairement inutilis� par CUTOUT_CFG
//																		|
//																		.
//									|	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	|
//					   				+-----------------------+-------+-------+-----------------------+
//									|  LEFT	|COOKIE	| RIGHT	| FORCE	|	X	|  LEFT	|COOKIE	| RIGHT |
//									\______________________/ \_____/ \_____/ \_____________________/
//												|				|							|
//												| 				|							|
//							'nocut_default_result'				'Force'						'push mask'
//					   Que faire et que retourner				Faut-il forcer la			Parties � 'pusher' sur l'array
//					      En cas de "non-coupe" ?				d�coupe ?					(si elles existent apr�s d�coupe)
//	   Ces 3 bits sont syst�matiquement retourn�s				Normalement celle-ci n'est
//	  En cas de non coupe. Si au moins un d'entre				valid�e que si le membre 'm_v'
//	 eux est TRUE alors la totalit� du VStage est				du cookiecutter est inf�rieur au membre 'm_v'
//	"push�e" sur l'array, sinon rien.( tous � FALSE )			du VStage. Si 'Force' est � TRUE, la d�coupe sera
//																toujours valid�e et tent�e !
//
//								   \______________________/ \_____________________________________/
//												|								 |
//									 'nocut_default_result' 	              'mode'
// |
// +-------------------------------------------------------------------------------------------------------------------

#define IS_CUTOUT_RESULT_LEFT(cutout_result) ((cutout_result)&FLAG_CUTOUT_LEFT)
#define IS_CUTOUT_RESULT_RIGHT(cutout_result) ((cutout_result)&FLAG_CUTOUT_RIGHT)
#define IS_CUTOUT_RESULT_COOKIE(cutout_result) ((cutout_result)&FLAG_CUTOUT_COOKIE)
#define IS_CUTOUT_RESULT_CUTTED(cutout_result) ((cutout_result)&FLAG_CUTOUT_CUT)

#define IS_CUTOUT_RESULT_CUTTED_LEFT(cutout_result) ((cutout_result) & (FLAG_CUTOUT_CUT | FLAG_CUTOUT_LEFT) == (FLAG_CUTOUT_CUT | FLAG_CUTOUT_LEFT))
#define IS_CUTOUT_RESULT_CUTTED_RIGHT(cutout_result) ((cutout_result) & (FLAG_CUTOUT_CUT | FLAG_CUTOUT_RIGHT) == (FLAG_CUTOUT_CUT | FLAG_CUTOUT_RIGHT))
#define IS_CUTOUT_RESULT_CUTTED_COOKIE(cutout_result) ((cutout_result) & (FLAG_CUTOUT_CUT | FLAG_CUTOUT_COOKIE) == (FLAG_CUTOUT_CUT | FLAG_CUTOUT_COOKIE))
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	Valeur de retour de la fonction cutOut apr�s d�coupe
// |	Cette valeur peut-�tre analys�e directement via les FLAG_CUTOUT_xx ou � l'aide des macros IS_CUTOUT_RESULT_xx
//
//
//												|	3	|	2	|	1	|	0	|
//												+-------------------------------+
//												| CUT ?	|  LEFT	|COOKIE	| RIGHT |
//												 \_____/ \_____________________/
//													|				|
//													|	 			|
//												cut ?				Part(s) push�e(s) sur l'output array.
//	Si TRUE une d�coupe a bien �t� valid�e et tent�e.				Notez que dans le cas o� CUT ( BIT_3 ) est FALSE :
//																	Soit la valeur des BITs 0 � 2 est FALSE �galement et alors
//		   Si FALSE il n'y a pas eut de validation de				rien n'est push� sur l'array.
//  d�coupe. Le comportement par d�faut (push or not)				Soit au moins 1 des BIT est � TRUE et LA TOTALITE de Vstage est
// � �t� appliqu� et la valeur de retour initialement				alors push�e sur Array.
//				 pass�e en parametre a �t� retourn�e.
//
// |
// +-------------------------------------------------------------------------------------------------------------------
// |
// |	Quelques exemples de valeur de retour:
// |
//						|	3	|	2	|	1	|	0	|
//						+----------------------------------+
//	NOTHING				|	1	|	0	|	0	|	0	|	--> Il y a eut tentative de d�coupe, mais rien n'est 'push�' dans l'array de sortie.
//																Cela est du aux flags de restitution ne demandant pas � restituer les parties
//																effectivement coup�es.
//
//	RIGHT				|	1	|	0	|	0	|	1	|	--> Il y a eut tentative de d�coupe, seule la partie d�passant � droite de l'emporte piece
//																a �t� 'push�e' dans l'array de sortie.
//																Les autres parties d�coup�es, si elles existent n'ont pas �t� push�es soit parceque la
//																configuration de d�coupe ne le demandait pas, soit parceque la totalit� du VStage se trouve
//																� droite du 'cookie cutter'
//
//	COOKIE				|	1	|	0	|	1	|	0	|	--> Il y a eut tentative de d�coupe, seule la partie d�coup�e par l'emporte piece a �t� 'push�e'
//																dans l'array de sortie.
//																Les autres parties d�coup�es, si elles existent n'ont pas �t� push�es soit parceque la
//																configuration de d�coupe ne le demandait pas, soit parceque la totalit� du VStage se trouve
//																� l'int�rieur du 'cookie cutter'

//	LEFT_COOKIE_RIGHT	|	1	|	1	|	1	|	1	|	--> Il y a eut tentative de d�coupe, toutes les parties d�coup�es ont �t� 'push�es'
//																dans l'array de sortie. La configuration a demand� � ce que cela soit ainsi fait et on
//																se retrouve donc bien avec 3 parties cons�cutives et distinctes 'push�es' dans l'array.
//
//	UNCUT				|	0	|	1	|	0	|	0	|	--> Il n'y a eut aucune d�coupe car les tests de validit� de coupe ont �chou�s.
//																La valeur de retour par d�faut pass�e en configuration est retourn�e ( BIT_2 ). Celle-ci
//																�tant non nulle ( au moins 1 des 3 BITS est TRUE ) la totalit� du Vstage a �t� 'push�e'
//																sur l'array de sortie.
//
//	UNCUT_BUT_NOTHING	|	0	|	0	|	0	|	0	|	--> Il n'y a eut aucune d�coupe car les tests de validit� de coupe ont �chou�s.
//																La valeur de retour par d�faut pass�e en configuration est retourn�e ( 0 ). Celle-ci
//																�tant nulle ( tous les BITS sont FALSE ) rien n'a �t� 'push�' sur l'array de sortie.
typedef struct NLVSTAGE NLVSTAGE;
struct NLVSTAGE
{
	Nf32 m_v;  // Vitesse limite associ�e
	Nf32 m_s0; // abscisse de 'd�part' de la clef ( position � partir de laquelle la clef � de l'effet )
	Nf32 m_s1; // abscisse de 'fin' de la clef ( position � partir de laquelle la clef n'a plus d'effet )

	Nf32 m_value; //
	Nu16 m_type;  // type pour d�finir comment interpr�ter 'm_value' et, en fonction,  calculer 'm_v'.
	Nu16 m_flags;

	enum TYPE
	{
		FORCED = 0,		// 'm_value' est 'bypass�e' par un 'force', ou simplement pas encore sett�e
		RELATIVE_VALUE, // 'm_value' repr�sente le % [0,1] de la vitesse max utilis�e sur la courbe ( = % de la vitesse de croisi�re )
		ABSOLUTE_VALUE	// 'm_value' repr�sente la vitesse maximum autoris�e � partir de cette clef.
	};
	NLVSTAGE() : m_type(FORCED), m_value(0), m_s0(0), m_s1(0), m_v(0.0f), m_flags(0) { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.f; };
	NLVSTAGE(const Nf32 s0, const Nf32 s1, const Nf32 val = 0.0f, const TYPE type = FORCED) : m_type(type), m_value(val), m_s0(s0), m_s1(s1), m_v(type == FORCED ? val : 0.0f), m_flags(0)
	{
		m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.f;
		NErrorIf(val < 0.0f, NERROR_VALUE_OUTOFRANGE);
		NErrorIf((type == RELATIVE_VALUE) && (val > 1.0f), NERROR_VALUE_OUTOFRANGE);
	};

	void set(const Nf32 s0, const Nf32 s1, const Nf32 val = 1.0f, const TYPE type = RELATIVE_VALUE);

	// Pour les 2 m�thodes qui suivent, on remarquera:
	//				m_v = -NF32_MAX
	//				... Cela pour g�n�rer des erreurs/aberrations plus loin et obliger � d�finir les NLVSTAGE correctement avec appel au moment appropri� � la m�thode "computeAdjustedVelocity"
	void setRelativeValue(Nf32 val)
	{
		m_v = -NF32_MAX;
		m_value = val;
		m_type = (Nu16)RELATIVE_VALUE;
		NErrorIf(val <= 0.0f, NERROR_VALUE_OUTOFRANGE);
		NErrorIf(val > 1.0f, NERROR_VALUE_OUTOFRANGE);
	};
	void setAbsoluteValue(Nf32 val)
	{
		m_v = -NF32_MAX;
		m_value = val;
		m_type = (Nu16)ABSOLUTE_VALUE;
		NErrorIf(val <= 0.0f, NERROR_VALUE_OUTOFRANGE);
	};

	void computeAdjustedVelocity(const Nf32 velocity_max);

#ifdef _NEDITOR
	void draw(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);
	void draw(NLPATH *ppath, const Nf32 width, const NCOLORPICKPACK pickpack);
#endif
	friend class NLPATH;
	friend class NLTRAJECTORY;

	enum AGGREGATE_FLAGS
	{
		NONE = 0,							   // Aucune action particuli�re, le NLVSTAGE est ajout� � la liste, si sa vitesse est identique � celle du dernier de la liste avant insertion, alors il le prolonge plut�t que de s'ajouter.
		PRESERVE_GAP = BIT_0,				   // Il n'y aura pas de prolongement(=fusion) si il existe un "vide" entre ce Vstage et le dernier de la liste d'insertion
		PRESERVE_JOINT = BIT_1,				   // Il n'y aura pas de prolongement(=fusion) si il y a un "joint" entre ce Vstage et le dernier de la liste d'insertion ( joint veut dire " last.m_s1 = inserted.m_s0 )
		PRESERVE_GAP_AND_JOINT = BIT_0 | BIT_1 // Il n'y aura pas de prolongement(=fusion) si il y a un "joint" entre ce Vstage et le dernier de la liste d'insertion ( joint veut dire " last.m_s1 = inserted.m_s0 )

	};
	void aggregate(NARRAY *poutput_array, const AGGREGATE_FLAGS flags = NONE);
	Nu32 cutOut(NARRAY *poutput_array, const NLVSTAGE *pcookiecutter, const Nu32 cfg = CUTOUT_CFG_REMOVE_COOKIE);
	// inline void trimCutForced(const Nbool bforce) { if (bforce)FLAG_ON(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); else FLAG_OFF(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); }
	void setCutters(const NLVSTAGE *p0, const NLVSTAGE *p1);
	void setCutters(const Nf32 cutter0_s, const Nf32 cutter0_v, const Nf32 cutter1_s, const Nf32 cutter1_v);
	void resetCutters() { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.0f; };
	// void resetCutters(const Nbool bforce = NFALSE) { m_cutter0.x = m_cutter0.y = m_cutter1.x = m_cutter1.y = 0.0f; if (bforce)FLAG_ON(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT); else FLAG_OFF(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT);};
	void force(const Nf32 s0, const Nf32 s1, const Nf32 val)
	{
		m_s0 = s0;
		m_s1 = s1;
		m_value = val;
		m_v = val;
		m_type = (Nu16)FORCED;
		NErrorIf(val < 0.0f, NERROR_VALUE_OUTOFRANGE);
	};

private:
	NVEC2 m_cutter0;
	NVEC2 m_cutter1;
};
#define NLclearVStageInArrayCallBack NULL
// inline void NLclearVStageInArrayCallBack(void* p){}