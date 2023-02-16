#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "../../../N/Maths/NVec2f32.h"
#include "../../../N/Containers/NArray.h"
#include "../../../N/File/NDataPacker.h"
#include "../../../N/Core/NVersion.h"

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#include "../../NLGfx.h"
#endif

#include "../NLPhysics.h"
#include "../DriveTrain/NLDriveTrainSpecs.h"
#include "../NLKin.h"
#include "../NLKinLimits.h"
#include "../NLKinTweak.h"
#include "../Path/NLPath.h"

#include "NLTrajectoryKey.h"
#include "NLVstageX.h"
#include "NLTrajectoryPack.h"


//#define DEFAULT_NLTRAJECTORY_KEYPOINT_ARRAY_CAPACITY							8
//#define DEFAULT_NLTRAJECTORY_VSTAGES_ARRAYCAPACITY								8
#define NLTRAJECTORY_SIMPLEBUILD_FLAG_MAX_VELOCITY_OVERSHOOT_AUTHORIZED			BIT_0
#define NLTRAJECTORY_SIMPLEBUILD_FLAG_MAX_ACCELERATION_OVERSHOOT_AUTHORIZED		BIT_1

#define NLSKIDDINGVELOCITYLIMIT(static_friction_coef, gravity, curvature_radius)											( sqrt(static_friction_coef * gravity * curvature_radius) )
#define NLTILTINGVELOCITYLIMIT(dist_center_of_mass_x_to_external_wheel, center_of_mass_y, gravity, curvature_radius)		( sqrt( (dist_center_of_mass_x_to_external_wheel/center_of_mass_y) * gravity * curvature_radius) )



// GESTION DES 
//
// NCOLORPICKPACK index ventilation (4 bits / index):
// 

#define NLTRAJECTORY_COLORPICKPACK_drawKinsArray					NSTCPLT_4b_MPCK( 0,0,0,6,5,4,0,0,15 ) // equivalant à NSTCPLT_4B_PCK(6,5,4,0,0,15)
#define NLTRAJECTORY_COLORPICKPACK_drawKinsArray_dark				NSTCPLT_4b_MPCK( 0,3,0,6,5,4,0,0,15 )
#define NLTRAJECTORY_COLORPICKPACK_drawKinsArray_light				NSTCPLT_4b_MPCK( 1,1,0,6,5,4,0,0,15 )
// NCOLORPICKPACK.#0....... j > 0									NCOLORPICKPACK.#3....... x
// NCOLORPICKPACK.#1....... ==  0									NCOLORPICKPACK.#4....... x
// NCOLORPICKPACK.#2....... j < 0									NCOLORPICKPACK.#5....... Text informations

#define NLTRAJECTORY_COLORPICKPACK_drawVStages						NSTCPLT_4b_MPCK( 0,0,0,15,14,3,0,0,15)
#define NLTRAJECTORY_COLORPICKPACK_drawVStages_dark					NSTCPLT_4b_MPCK( 0,3,0,15,14,3,0,0,15)
#define NLTRAJECTORY_COLORPICKPACK_drawVStages_light				NSTCPLT_4b_MPCK( 1,1,0,15,14,3,0,0,15)
// NCOLORPICKPACK.#0....... ceil									NCOLORPICKPACK.#3....... x
// NCOLORPICKPACK.#1....... floor									NCOLORPICKPACK.#4....... x
// NCOLORPICKPACK.#2....... x										NCOLORPICKPACK.#5....... Text informations

#define NLTRAJECTORY_COLORPICKPACK_drawReverseVelocityCheck			NSTCPLT_4b_MPCK( 0,0,0,0,1,3,0,0,0)
#define NLTRAJECTORY_COLORPICKPACK_drawReverseVelocityCheck_dark	NSTCPLT_4b_MPCK( 0,3,0,0,1,3,0,0,0)
#define NLTRAJECTORY_COLORPICKPACK_drawReverseVelocityCheck_light	NSTCPLT_4b_MPCK( 1,1,0,0,1,3,0,0,0)
// NCOLORPICKPACK.#0....... j > 0									NCOLORPICKPACK.#3....... x
// NCOLORPICKPACK.#1....... ==  0									NCOLORPICKPACK.#4....... x
// NCOLORPICKPACK.#2....... j < 0									NCOLORPICKPACK.#5....... x

#define NLTRAJECTORY_COLORPICKPACK_drawKnodes						NSTCPLT_4b_MPCK( 0,0,0,0,1,3,0,0,15)
#define NLTRAJECTORY_COLORPICKPACK_drawKnodes_dark					NSTCPLT_4b_MPCK( 0,3,0,0,1,3,0,0,15)
#define NLTRAJECTORY_COLORPICKPACK_drawKnodes_light					NSTCPLT_4b_MPCK( 1,1,0,0,1,3,0,0,15)
// NCOLORPICKPACK.#0....... S Motion Shape color phase 1			NCOLORPICKPACK.#3....... S Motion Shape color 'Out'
// NCOLORPICKPACK.#1....... S Motion Shape color phase 2			NCOLORPICKPACK.#4....... Flat Motion Shape color
// NCOLORPICKPACK.#2....... S Motion Shape color phase 3			NCOLORPICKPACK.#5....... F et S Motion Shape Text color

#define NLTRAJECTORY_COLORPICKPACK_drawKeys							NSTCPLT_4b_MPCK( 0,0,0,6,0,0,0,0,5)
#define NLTRAJECTORY_COLORPICKPACK_drawKeys_dark					NSTCPLT_4b_MPCK( 0,3,0,6,0,0,0,0,5)
#define NLTRAJECTORY_COLORPICKPACK_drawKeys_light					NSTCPLT_4b_MPCK( 1,1,0,6,0,0,0,0,5)
// NCOLORPICKPACK.#0....... Enabled Key								NCOLORPICKPACK.#3....... x
// NCOLORPICKPACK.#1....... Disabled key							NCOLORPICKPACK.#4....... x
// NCOLORPICKPACK.#2....... x										NCOLORPICKPACK.#5....... Text color

class NLTRAJECTORY
{
	friend class NLPATH_WORKBENCH;

public:
	#define MASK_NTRJ_ENFORCE					(BIT_0|BIT_1|BIT_2)
	#define FLAGS_NLTRJ_ENFORCE_ALL				MASK_NTRJ_ENFORCE
	#define FLAG_NLTRJ_ENFORCE_VEL				BIT_0		// Force l'utilisation de 'pklim->m_v' par la fonction 'build' et ce même si elle est hors limite des capacités du Robot
	#define FLAG_NLTRJ_ENFORCE_ACC				BIT_1		// Force l'utilisation de 'pklim->m_a' par la fonction 'build' et ce même si elle est hors limite des capacités du Robot
	#define FLAG_NLTRJ_ENFORCE_JRK				BIT_2		// Force l'utilisation de 'pklim->m_j' par la fonction 'build' et ce même si il est hors limite des capacités du Robot
	#define FLAG_NLTRJ_SPECIFIC_KINLIMITS		BIT_3		// Le membre m_specificKinLimits est valide et doit être pris en compte avec les flags 'FLAG_NLTRJ_ENFORCE_xxx'
	//#define FLAG_xxx					BIT_3
	//											.
	//											.
	//											.
	//#define FLAG_NLTRJ_xxx					BIT_7	-->	MAX ! Car ces flags sont stockés dans le Nu8 "NLPATH_WORKBENCH.m_trajectoryBuildFlags" 

	NLTRAJECTORY();
	NLTRAJECTORY(NLPATH *ppath, const NLKINLIMITS *pklim, const NLKINTWEAK *pktwk, Nu32 enforceflags);
	~NLTRAJECTORY();
	
	Nu32	write(FILE* pfile, NDATAPACKER *pdpacker);
	Nu32	read(FILE* pfile, NDATAPACKER* pdpacker);

	void	build(const NLDRIVETRAINSPECS *pdtspecs);
//	void	buildTrajectoryPack(NLTRAJECTORY_BASIC_PACK *ppack);
//	void	buildTrajectoryPack(NLTRAJECTORY_PACK *ppack);
	//Nf32	getMinPathLength(const NLDRIVETRAINSPECS* pdtspecs);

	/*
	Nf32	getTime(const Nf32 s);
	void	getKinAtS(NLKIN *pkin, const NLKIN *pk0, const NLKIN *pk1, const Nf32 s_from_trajectory_start);
	void	getKinAtS(NLKIN *pkin, const Nf32 s_from_trajectory_start);
	void	getKinAtT(NLKIN *pkin, const Nf32 t);
	*/

	Nu32		insertUserVelocityStage(NLVSTAGE* pnewvs);										// Insert un NLVSTAGE ( = velocity stage défini manuellement par l'utilisateur )
	NLTRJKEY*	insertUserKey(const Nf32 s, const Nu32 insert_before, const Nu32 keyflags = 0);	// Insert une Trajectory Key (NLTRJKEY) définie par l'utilisateur.
//NLTRJKEY*	insertUserKeyStop(const Nf32 s, const Nu32 insert_before);							// Insert une Trajectory Key (NLTRJKEY) qui est un Stop, définie par l'utilisateur.
	NLTRJKEY*	addUserKey(const Nf32 s, const Nu32 keyflags = 0);								// Insert une Trajectory Key (NLTRJKEY) définie par l'utilisateur.
//	NLTRJKEY*	addUserKeyStop(const Nf32 s);													// Insert une Trajectory Key (NLTRJKEY) qui est un Stop, définie par l'utilisateur.
//	NLTRJKEY*	MoveUserKey(const Nf32 s);														// déplace une Trajectory Key (NLTRJKEY) à une nouvelle position s et potentiellement la change de place dans l'array des keys !

	inline void	setKinematicsTweaks(const NLKINTWEAK* pktwk) { if (pktwk) { m_kinTweak = *pktwk; } else { m_kinTweak.m_aHoldLocalLimitRatio = 0.0f; m_kinTweak.m_jHoldLocalLimitRatio = 0.0f; } }
	void		setPath(NLPATH* ppath);
	inline void	setSpecificKinematicLimits(NLKINLIMITS* pklim, Nu32 enforceflags = 0) { if (pklim) { FLAG_ON(m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS | (enforceflags & MASK_NTRJ_ENFORCE)); m_specificKinLimits = *pklim; } else { FLAG_OFF(m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS); } }


	Nf32	getDt() { return m_pack.m_dt; };
	Nf32	getDs() { return m_pack.m_ds; };

	NLTRAJECTORY_PACK* getPack() { return &m_pack; };
	NLPATH* getPath() { return m_pPath; };
	NARRAY* getKeysArray() { return &m_keysArray; };
	
/*
	inline void	setKinematicsTweaksAccelRatio(const Nf32 accel_ratio) { m_kinTweak.m_aHoldLocalLimitRatio = accel_ratio; }
	inline void	setKinematicsTweaksJerkRatio(const Nf32 jerk_ratio) { m_kinTweak.m_jHoldLocalLimitRatio = jerk_ratio;}
	inline Nf32	getKinematicsTweaksAccelRatio() const { return m_kinTweak.m_aHoldLocalLimitRatio; }
	inline Nf32	getKinematicsTweaksJerkRatio() const { return m_kinTweak.m_jHoldLocalLimitRatio; }
*/
	//inline Nf32	getDt() {return m_dt;}
	//inline Nf32	getDs() {return m_ds; }

	//void	getPoseAtT(NLDIFFERENTIAL_DRIVETRAIN_POSE *ppose, const Nf32 t);
	//void	getPoseAtT(NLDIFFERENTIAL_DRIVETRAIN_POSE_XTD *ppose, const Nf32 t);

#ifdef _NEDITOR
	NARRAY* getLimitedpKeysArray() { return &m_limitedpKeysArray; }

	void	drawKnodes(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);					// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawKnodes
	void	drawVStages(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);					// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawVStages
	void	drawMotionProfile(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);				// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawKinsArray
	
	void	drawKinTArray(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);					// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawKinsArray
	//void	drawKinRArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);					// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawKinsArray
	void	drawReverseVelocityCheck(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);		// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawReverseVelocityCheck
	//void	drawChunksArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack) { m_pack.drawChunksArray(p2docs, pickpack); };	// NCOLORPICKPACK associé : NLTRAJECTORY_COLORPICKPACK_drawKinsArray
	void	drawMotionProfilePathKeyPointId(NL2DOCS* p2docs);								// pas besoin de color pickpack car utilise le system d'affichage de coordonnées ( avec style ) de NL2DOCS.

	void	drawUserVStagesArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
	void	drawLimitedUserVStagesArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
	void	drawLimitedUserVStagesArray(const Nf32 width, const NCOLORPICKPACK pickpack);

	void	drawKeysArray(NL2DOCS* p2docs);
	void	drawLimitedKeysArray(NL2DOCS* p2docs);
	void	drawLimitedKeysArray(const NLGFX *pgfx);

#endif

	Nu32					m_flags;

	NLKINTWEAK				m_kinTweak;				// Ratios d'ajustement de l'acceleration et du jerk appliqués à l'entrée et sortie de chaque palier de vitesse.
	NLKINLIMITS				m_specificKinLimits;	// limites kinematics associées à la trajectoire ! ATTENTION ! Les limites effectivements utilisées sont différentes ...
													// ... elles sont construites localement en prenant egalement en compte celles propres au robot ( DTspecs et des flags FLAG_NLTRJ_ENFORCE_xxx )
													// ... et en prenant en compte egalement la vitesse de croisiere max possible sur le chemin associé.

private:
	//Nu32	buildVelocityStages(const NLDRIVETRAINSPECS* pdtspecs, const NLKINLIMITS * pklim);
	//Nu32	adjustVelocityStages(const NLKINLIMITS* pklim);
	//	NLKINLIMITS	m_limits;
	NLPATH					*m_pPath;					// Path associé à la Trajectoire.
	NARRAY					m_keysArray;				// Array des 'Keys' définis par l'utilisateur le long du chemin.
	NARRAY					m_userVStagesArray;			// Array des Vstages ( NLVSTAGE) définis manuellement par l'utilisateur
	
	NLTRAJECTORY_PACK		m_pack;						

#ifdef _NEDITOR
	NARRAY					m_kinTArray;				// Array de promotion des Kin T ( T pour Traveling )
	NARRAY					m_kinSArray;				// Array de promotion des Kin R ( R pour Spot - sur place )	

	NARRAY					m_limitedpKeysArray;		// Array des (pointeurs sur) 'Keys' définis par l'utilisateur le long du chemin, limitées à l'intervalle couvert par le chemin. 
	NARRAY					m_pathVStagesArray;			// Array des Vstages computés à partir du chemin.
	NARRAY					m_limitedUserVStagesArray;	// Array des Vstages ( NLVSTAGE) définis manuellement par l'utilisateur, limités à l'intervalle couvert par le chemin.
	NARRAY					m_combinedVStagesArray;		// Array de la combinaison des Vstages définis manuellement par l'utilisateur ( et limités) et des VStages.
	NARRAY					m_vStageXSlicesArray;

//	NARRAY					m_sMotionProfilesArray;		// Array des motionprofiles en external data. (utilisé pour le calcul des kins de SpotChunk )

#endif

};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLTRAJECTORY_HEADER			NMAKEVERSION(0,0,0)				// >>> 2021/04/03
typedef struct
{
	Nu32				m_flags;
	// NKINLIMITS:
	Nf32			m_limits_v;
	Nf32			m_limits_a;
	Nf32			m_limits_j;
	NLKINTWEAK		m_kinTweak;
	Nu32			m_pathIndex;

	NARRAYBOUNDS	m_keysArrayBounds;
	NARRAYBOUNDS	m_userVStagesArrayBounds;
}NLTRAJECTORY_HEADER;
