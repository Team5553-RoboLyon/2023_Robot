#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectory.h"
#include "lib/NL/MotionControl/Path/NLPathPersistentTrackingData.h"
#include "lib/NL/Simulation/VControllers/NLVMotorController.h"
#include "lib/NL/Simulation/VControllers/NLVEncoder.h"
#include "lib/NL/Simulation/VControllers/NLVGyro.h"

#include "lib/NL/MotionControl/NLPid.h"
#include "lib/NL/MotionControl/NLRobotPose.h"
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"

#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"

// #include "./VirtualRobot/NLVirtualRobot.h"
// #define FLAG_NLROBOT_IS_ON	BIT_0
#define FLAG_NLROBOT_IS_INITIALIZED BIT_0 // la classe Robot est initialis�e., c'est � dire que la fonction RobotInit � �t� appel�e et que les appels � la fonction RobotPeriodic() sont possibles.
class NLVIRTUAL_ROBOT;
class NLROBOT
{
public:
	/** Liste des differentes valeurs retourn�es  par les differentes fonctions Check ( AccVelFastCheck, AccVelCheck, ... ).
	 *	Ces valeurs sont potentiellement combinees entre elles avec l'operateur |.
	 *	Par exemple, ACCELERATION_OVERSHOOT|VELOCITY_WAS_OVERSHOOTED.
	 */
	enum STATE
	{
		PATH_ERROR = 0, ///< L'initialisation du path following a rencontr� un probl�me ( erreur au chargement tr�s probablement ). Le Robot ne peut-�tre en �tat PATH_FOLLOWING.
		PATH_FOLLOWING, ///< Le robot est en �tat de suivit de chemin.
		PATH_END		///< La Vitesse  est en d�passement.
	};
	NLROBOT() : m_flags(0) {}
	NLROBOT(NLVIRTUAL_ROBOT *pvr) : m_flags(0), m_pVirtualRobot(pvr) {}
	~NLROBOT() {}

	void RobotInit(NLPATH_WORKBENCH *pwb);
	void RobotPeriodic(const Nf32 dt);
	// void TeleopInit();
	// void TeleopPeriodic();
	// void TestInit();
	// void TestPeriodic();

	void inline setVirtualRobot(NLVIRTUAL_ROBOT *pvr) { m_pVirtualRobot = pvr; }
	inline Nbool IsInitialized() { return (ISFLAG_ON(m_flags, FLAG_NLROBOT_IS_INITIALIZED) ? NTRUE : false); }

	void draw();
	STATE m_state;

	NLVMOTOR_CONTROLLER m_moteurL1;
	NLVMOTOR_CONTROLLER m_moteurL2;
	NLVMOTOR_CONTROLLER m_moteurR1;
	NLVMOTOR_CONTROLLER m_moteurR2;

	NLVENCODER m_leftGearboxEncoder;
	NLVENCODER m_rightGearboxEncoder;

	NLMOTOR_CHARACTERIZATION m_CrtzL1;
	NLMOTOR_CHARACTERIZATION m_CrtzL2;
	NLMOTOR_CHARACTERIZATION m_CrtzR1;
	NLMOTOR_CHARACTERIZATION m_CrtzR2;

	NLVGYRO m_gyro;

	Nu32 m_flags;
	// NLTRAJECTORY				*m_pCurrentTrajectory;
	// NLPATH					*m_pPath;

	/*
	Nf32						m_dsLeftWheel;
	Nf32						m_dsRightWheel;
	NLROBOTPOSE					m_estimatedPose;
	*/
	// NLRAMSETE			m_ramsete;
	/*
	// ---------------------------------------------
	// RAMSETE TEST
	Nf32		m_ramseteZeta;
	Nf32		m_ramseteB;
	NVEC2		m_ramseteErrPos;
	Nf32		m_ramseteErrAngle;
	Nf32		m_ramseteVelocityRef;
	Nf32		m_ramseteAngularVelocityRef;
	// ---------------------------------------------


	// position et angle du robot estim�e via les encodeurs
	NVEC3				m_estimatedPosition;	// Position "estim�e" du centre de masse de la base gr�ce aux encodeurs
	NVEC3f32			m_estimatedArcCenterPos;
	Nf32				m_estimatedAngle;		// Orientation "estim�e" de la base gr�ce aux encodeurs + gyro
	NMATRIX3x3			m_estimatedMatrix;

	// position et angle du robot estim�e en utilisant les vraies vitesses et accelerations dict�es par motion profiling
	NVEC3				m_trueVelAccCenterPos;	// Position du centre du cercle virage
	NVEC3				m_trueVelAccPosition;	// Position "estim�e" du centre de masse de la base gr�ce
	Nf32				m_trueVelAccAngle;		// Orientation "estim�e" de la base gr�ce
	NMATRIX3x3			m_trueVelAccMatrix;

	// position et angle du robot pr�cis, sans passer par les vitesses mais en utilisant directement l'abscisse curviligne d�crite dans la trajectoire
	// Il s'agit donc de la position et de l'angle de reference.
	NVEC3				m_refCenterPos;	// Position du centre du cercle virage
	NVEC3				m_refPosition;	// Position "estim�e" du centre de masse de la base gr�ce
	NVEC2				m_refTangent;
	Nf32				m_refAngle;		// Orientation "estim�e" de la base gr�ce
	NMATRIX3x3			m_refMatrix;

	// gestion d'erreur sur les abscisses curvilignes gauche et droite
	Nf32				m_refLeftS;
	Nf32				m_refRightS;
	Nf32				m_prevK;
	Nf32				m_prevS;
	// pure pursuit data
	//NMATRIX3x3			m_purePursuitMatrix;  !!! m_purePursuitMatrix repr�sente la base "locale" du robot soit  "m_estimatedMatrix" du coup pas besoin de dupliquer.
	NVEC2f32			m_purePursuitTarget;
	Nf32				m_purePursuitTheta;
	Nf32				m_purePursuitArcRadius;
	Nf32				m_purePursuitArcLength;
	Nf32				m_purePursuitLookaheadCoefficient;

	NLPID				m_pid;
	NLPID_ERROR			m_errorLeft;
	NLPID_ERROR			m_errorRight;

	Nf32				m_leftErrorVoltage;
	Nf32				m_rightErrorVoltage;
	*/

	// ------------------------------------------------
	//
	// Path and trajectory Datas:
	//
	// ------------------------------------------------
	// All drive train specifications:
	// NLDRIVETRAINSPECS			m_DriveTrainSpecs;
	// Simplified States pack data:
	// NLTRAJECTORY_BASIC_PACK	m_TrajectoryStateSPack;
	// NLTRAJECTORY_BASIC_STATE		m_currrentSState;
	// Full States pack data:
	NLTRAJECTORY_PACK m_TrajectoryPack;
	NLFOLLOWER_TANK m_follower;

	// NLTRAJECTORY_POINT		m_currrentPoint;

private:
	NLVIRTUAL_ROBOT *m_pVirtualRobot;
};
