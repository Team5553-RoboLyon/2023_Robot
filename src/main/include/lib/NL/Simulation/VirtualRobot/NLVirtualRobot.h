#pragma once

#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "lib/N/Miscellaneous/NColor.h"

#include "lib/NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "NLVirtualMotor.h"
#include "NLVirtualGearBox.h"
#include "lib/NL/SimulatioN/VirtualRobot/NLVirtualEncoder.h"
#include "NLVirtualGyro.h"
#include "lib/NL/Simulation/NLRobot.h"

// Le robot virtuel ( VIRTUAL ROBOT ) est pour le simulateur l'�quivalant du robot r�el ( celui qui roule pour de vrai sur le terrain ).
// Ainsi le robot virtuel est compos� de boites de vitesse virtuelles, moteurs virtuels, encodeurs virtuel, gyroscope virtuel ... qui repr�sentent
// � chaque fois leur �quivalant "r�els".

#ifdef _NEDITOR

#define CONSTANT_NLVIRTUAL_ROBOT_INTEGRATION_DT 0.001f			// 1 milli�me de seconde
#define CONSTANT_NLVIRTUAL_ROBOT_DEFAULT_SIMULATION_TIME 150.0f // 2 mn 30 sec

// nombre MAXIMUM de systemes inclus dans le robot virtuel.
// Ces valeurs, fixes, peuvent �tre chang�es ici, mais doivent rest�es comprises entre 1 et 32 !
// ... 32 est le max car � chaque ensemble de syst�mes est associ� une valeur 32 bits ( ex: m_isMotorActive pour les moteurs ) ou chaque bit represente l'etat
// d'un des syst�mes de l'ensemble.
#define CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_MOTOR 32	  // min: 1 max: 32
#define CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GEARBOX 32 // min: 1 max: 32
#define CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_ENCODER 32 // min: 1 max: 32

#define FLAG_NLVIRTUAL_ROBOT_IS_ON BIT_0

#define FLAG_NLVIRTUAL_ROBOT_DRAW_TARGET BIT_1
#define FLAG_NLVIRTUAL_ROBOT_DRAW_PERFECT BIT_2
#define FLAG_NLVIRTUAL_ROBOT_DRAW_SIM BIT_3

// class NLROBOT;
class NLVIRTUAL_ROBOT
{
	friend class NLVMOTOR_CONTROLLER;
	friend class NLVENCODER;
	friend class NLVGYRO;

	//	friend class NLROBOT;
public:
	void Init(); // initialise le robot virtuel ( appel� une seule fois � l'initialisation du programme )
	void reset();

	/*
	inline Nbool IsOn() { return (ISFLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON) ? NTRUE : FALSE); }
	inline void TurnedOn() { FLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON); }
	inline void TurnedOff(){ FLAG_OFF(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON); }
	*/
	void RegisterMotor(const Nu8 channelid, const NLMOTOR_CHARACTERIZATION *pcharacterization, const Nf32 nominal_voltage, const Nf32 free_speed, const Nf32 free_current, const Nf32 stall_current, const Nf32 stall_torque, const Ns32 gearbox = -1);
	void UnregisterMotor(const Nu8 channelid);
	void RegisterGearBox(const Nu8 channelid, const Nf32 gearbox_ratio);
	void UnregisterGearBox(const Nu8 channelid);
	void RegisterEncoder(const Nu8 channelid, const Nu32 pulse_per_revolution, const Nu32 trackedchannel, const Nbool isconnectedtoamotor = NFALSE);
	void UnregisterEncoder(const Nu8 channelid);
	void RegisterGyro(/*const Nu8 channelid,*/ const Nf32 drift);
	void UnregisterGyro(/*const Nu8 channelid*/);

	// void Stop();

	NLVIRTUAL_ROBOT();
	NLVIRTUAL_ROBOT(const NLDRIVETRAINSPECS *pspecs, const NCOLOR *framecolor, const NCOLOR *wheelcolor);

	inline Nbool IsOn() { return (ISFLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON) ? NTRUE : FALSE); }
	void switchOn(NLPATH_WORKBENCH *pwb, const Nf32 time = CONSTANT_NLVIRTUAL_ROBOT_DEFAULT_SIMULATION_TIME);
	void switchOff();
	void setFlagsOn(const Nu8 flags) { FLAG_ON(m_flags, flags); }
	void setFlagsOff(const Nu8 flags) { FLAG_OFF(m_flags, flags); }

	Nu32 update(const Nf32 dt);

	void draw();
	void drawDashBoard(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);
	// void drawRobotShape(const NMATRIX3x3* prot, const NVEC3* ppos, const NCOLOR* pcolor);
	void drawRobotShape(const NMATRIX *pbase, const NMATRIX3x3 *prot, const NVEC3 *ppos, const NCOLOR *pcolor);
	Nf32 getSimulationLeftTime() const { return m_SimulationLeftTime; }

private:
	Nf32 m_SimulationLeftTime; // Temps de simulation restant ( pour arr�t auto )

	// Mechanics:
	NLDRIVETRAINSPECS *m_pSpecifications;

	Nu8 m_flags;
	Nu8 m_motorCount;
	Nu8 m_gearboxCount;
	Nu8 m_encoderCount;

	Nu32 m_isMotorActive;	// champ de 32 bits. BIT_x est ON: le moteur x est actif !
	Nu32 m_isGearBoxActive; // champ de 32 bits. BIT_x est ON: la gearbox x est active !
	Nu32 m_isEncoderActive; // champ de 32 bits. BIT_x est ON: l'encodeur x est actif !
	Nu32 m_isGyroActive;	// champ de 32 bits. BIT_x est ON: le gyro x est actif ! ( actuellement 1 seul Gyro possible !! )

	NLVIRTUAL_MOTOR m_motor[CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_MOTOR];
	NLVIRTUAL_GEARBOX m_gearbox[CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GEARBOX];
	NLVIRTUAL_ENCODER m_encoder[CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_ENCODER];
	NLVIRTUAL_GYRO m_gyro;

	// Simulation of Real World:
	NVEC3 m_centerPos; // Position du centre du cercle 'virage' sur lequel est en train de se d�placer le robot
	NVEC3 m_position;  // Position "r�elle" du centre de masse de la base

	Nf32 m_prevLeft;  // Odometrie. pour garder trace de la distance parcourue par la roue gauche du robot
	Nf32 m_prevRight; // Odometrie. pour garder trace de la distance parcourue par la roue droite du robot

	Nf32 m_angle; // Angle "r�el" de la base
	// Nf32				m_prevAngle;
	NMATRIX3x3 m_matrix; // matrice de rotation du robot

	NCOLOR m_frameColor;
	NCOLOR m_wheelColor;

	// Int�gration de la classe Robot "Dans" le virtualRobot.
	NLROBOT m_robot;
};

#endif // _NEDITOR
