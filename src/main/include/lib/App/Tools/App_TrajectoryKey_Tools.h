#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							App_TrajectoryKey_Tools.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../N/Event/NEvent.h"
#include "../../N/Core/NTime.h"
#include "../../N/UI/NUI.h"
#include "../../N/NEditor.h"
#include "../../N/NUsualCoordinates.h"
#include "../../N/Utilities/Draw/NUT_Draw.h"

#include "../../NL/MotionControl/Path/NLPathPersistentTrackingData.h"
#include "../../NL//MotionControl/NLKin.h"

#include "../../NL/MotionControl/NLPathWorkbench.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define APP_USERKEY_DETECTION_DISTANCE							0.3f
//#define APP_USERVZONE_DETECTION_DISTANCE_SQUARED					(APP_USERVZONE_DETECTION_DISTANCE*APP_USERVZONE_DETECTION_DISTANCE)

#define DEFAULT_APP_USERKEY_SNAP_S_THRESHOLD					0.05f
#define DEFAULT_APP_USERKEY_SPACING								0.1f		// distance minimum "par défaut" entre 2 clefs [ 0.1f = 0.1 mètre = 1 cm ]	
#define CONSTANT_APP_USERKEY_MIN_SPACING						0.001f		// 1 mm	

//#define FLAG_APP_USERKEY_TOOLDATA_POINT_REGISTRED			BIT_0
//#define MASK_APP_USERKEY_TOOLDATA_xxx						(BIT_0|BIT_1)

typedef struct APP_TRJKEY_TOOLDATA_KEY_CANDIDATE	APP_TRJKEY_TOOLDATA_KEY_CANDIDATE;
struct  APP_TRJKEY_TOOLDATA_KEY_CANDIDATE
{
	Nf32		m_s;
	Nu32		m_TrjK1Index;
	NLTRJKEY*	m_pTrjK1;
};


#define FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID					BIT_0		// La clef candidate est utilisable pour créer une clef selon les critères en cours (... et potentiellement ajustée ).
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED				BIT_1		// La clef candidate est ajustée pour prendre en compte les critères/contraintes en cours.
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_NEXT_KEY	BIT_2		// L'abscisse en entrée est trop proche de la clef suivante (et déjà en place).
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_PREV_KEY	BIT_3		// L'abscisse en entrée est trop proche de la clef précédente (et déjà en place).
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_MERGED					BIT_4		// L'abscisse en entrée est confondue(=superposée) avec celle d'une clef déjà en place.
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_BEFORE_START				BIT_5		// L'abscisse en entrée est située avant l'abscisse de la première clef ( donc s <= 0.0f ! )
#define	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_AFTER_END				BIT_6		// L'abscisse en entrée est située après l'abscisse de la dernière clef ( donc s >= NF32MAX ! )

/*
#define APP_TRJKEY_TOOLDATA_SUBMIT_REFUSE			0	// Refus !			Car pas assez de place entre les deux clefs préexistantes encadrant s ( l'abscisse soumise à validation )
#define APP_TRJKEY_TOOLDATA_SUBMIT_ACCEPT			1	// Acceptation !	Sans aucun changement.
#define APP_TRJKEY_TOOLDATA_SUBMIT_ADJUST_UP		2	// Acceptation !	Mais avec ajustement de s pour le rapprocher de pk1 car trop près de pk0
#define APP_TRJKEY_TOOLDATA_SUBMIT_ADJUST_DOWN		3	// Acceptation !	Mais avec ajustement de s pour le rapprocher de pk0 car trop près de pk1
#define APP_TRJKEY_TOOLDATA_SUBMIT_ADJUST_PRECISE	4	// Acceptation !	Mais avec ajustement de s pour le placer exactement entre pk0 et pk1, seule place disponible.
*/

#define FLAG_APP_TRJKEY_TOOLDATA_ALLOW_UNDERSPACING_KEY		BIT_0

typedef struct APP_TRJKEY_TOOLDATA	APP_TRJKEY_TOOLDATA;
struct  APP_TRJKEY_TOOLDATA
{
	APP_TRJKEY_TOOLDATA();
	APP_TRJKEY_TOOLDATA(NLPATH_WORKBENCH *pwb);
	~APP_TRJKEY_TOOLDATA();

	Nu32 submit(APP_TRJKEY_TOOLDATA_KEY_CANDIDATE* pkc, const Nf32 s, const NLTRJKEY* pkghost = NULL);
	NLTRJKEY* setSandSort(NLTRJKEY* pcursel, APP_TRJKEY_TOOLDATA_KEY_CANDIDATE *pcandidate);

	Nu32								m_flags;
	NLPATH_WORKBENCH					*m_pWorkbench;
	NLPATH_PERSISTENT_TRACKING_DATA		m_PersistentTrackingData;
	Nf32								m_SnapThreshold;
	Nf32								m_keySpacing;			// Distance minimum entre 2 clefs

	Nu32								m_keyCreationFlags;
};


//TOOLS
// -----------------------------------------------------------------------------------------------------------------------------
NEDITOR_RESULT			App_TrajectoryKeyTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT			App_TrajectoryKeyTool_Edit(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param);
NEDITOR_RESULT			App_TrajectoryKeyTool_Delete(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param);
void					App_TrajectoryKeyTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus

