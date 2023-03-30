#pragma once

#include "lib/N/NType.h"
// #include "../../../N/Containers/NRingBuffer.h"
#include "lib/NL/MotionControl/NLRobotPose.h"

#include "NLTrajectoryPack.h"
#include "NLTrajectoryPackGetPointPersistentResult.h"
#include "NLTrajectoryPostedMessage.h"

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#endif

typedef struct NLFOLLOWER_TANK_OUTPUT NLFOLLOWER_TANK_OUTPUT;
struct NLFOLLOWER_TANK_OUTPUT
{
	void null()
	{
		m_leftVelocity = 0.0f;
		m_leftAcceleration = 0.0f;
		m_rightVelocity = 0.0f;
		m_rightAcceleration = 0.0f;
	}
	Nf32 m_leftVelocity;	  // Vitesse � appliquer � gauche
	Nf32 m_leftAcceleration;  // Acceleration � appliquer � gauche
	Nf32 m_rightVelocity;	  // Vitesse � appliquer � droite
	Nf32 m_rightAcceleration; // Acceleration � appliquer � droite
};

#ifdef _NEDITOR
#define NLFTDD_PARAM_BIT_ID_SHIFTING 15
#define NLFTDD_PARAM_ID_V 0
#define NLFTDD_PARAM_ID_A 1
#define NLFTDD_PARAM_ID_W 2
#define NLFTDD_PARAM_ID_ALPHA 3
#define NLFTDD_PARAM_ID_ERR_X 4
#define NLFTDD_PARAM_ID_ERR_Y 5
#define NLFTDD_PARAM_ID_ERR_ANG 6
#define NLFTDD_PARAM_ID_ENCOD_LEFT 7
#define NLFTDD_PARAM_ID_ENCOD_RIGHT 8
#define NLFTDD_PARAM_ID_ENCOD_LEFT_VAR 9
#define NLFTDD_PARAM_ID_ENCOD_RIGHT_VAR 10
#define NLFTDD_PARAM_ID_OUTPUT_LEFT_V 11
#define NLFTDD_PARAM_ID_OUTPUT_LEFT_A 12
#define NLFTDD_PARAM_ID_OUTPUT_RIGHT_V 13
#define NLFTDD_PARAM_ID_OUTPUT_RIGHT_A 14

typedef struct NLFOLLOWER_TANK_DRAWDATA NLFOLLOWER_TANK_DRAWDATA;
struct NLFOLLOWER_TANK_DRAWDATA
{
	union
	{
		struct
		{
			Nf32 m_v;	  // PARAM ID = 0  >>> FLAG = (8 + 0 )	BIT_8
			Nf32 m_a;	  // PARAM ID = 1  >>> FLAG = (8 + 1 )	BIT_9
			Nf32 m_w;	  // PARAM ID = 2  >>> FLAG = (8 + 2 )	BIT_10
			Nf32 m_alpha; // PARAM ID = 3  >>> FLAG = (8 + 3 )	BIT_11

			NVEC2 m_errorPosition; // PARAM ID = 4  >>> FLAG = (8 + 4 )	BIT_12
								   // PARAM ID = 5  >>> FLAG = (8 + 5 )	BIT_13
			Nf32 m_errorAngle;	   // PARAM ID = 6  >>> FLAG = (8 + 6 )	BIT_14

			Nf32 m_encoderLeft;			  // PARAM ID = 7  >>> FLAG = (8 + 7 )	BIT_15
			Nf32 m_encoderRight;		  // PARAM ID = 8  >>> FLAG = (8 + 8 )	BIT_16
			Nf32 m_encoderLeftVariation;  // PARAM ID = 9  >>> FLAG = (8 + 9 )	BIT_17
			Nf32 m_encoderRightVariation; // PARAM ID = 10 >>> FLAG = (8 + 10 )	BIT_18

			NLFOLLOWER_TANK_OUTPUT m_output; // PARAM ID = 11 >>> FLAG = (8 + 11 )	BIT_19
											 // PARAM ID = 12 >>> FLAG = (8 + 12)	BIT_20
											 // PARAM ID = 13 >>> FLAG = (8 + 13)	BIT_21
											 // PARAM ID = 14 >>> FLAG = (8 + 14)	BIT_22
			Nf32 m_time;					 // PARAM ID = 15 >>> FLAG = (8 + 15)	BIT_23
		};

		Nf32 m_params[16];
	};
};
#endif

class NLFOLLOWER_TANK
{
#define FLAG_REVERSE_MODE BIT_0
	friend class NLROBOT;

public:
	enum GEARBOX
	{
		LEFT = 0,
		RIGHT = 1,
		NB = 2
	};
	NLFOLLOWER_TANK()
	{
		Nmem0(this, NLFOLLOWER_TANK);
#ifdef _NEDITOR
		NSetupRingBuffer(&m_errorBuffer, 800, sizeof(NLFOLLOWER_TANK_DRAWDATA));
#endif
	};

	~NLFOLLOWER_TANK()
	{
#ifdef _NEDITOR
		NClearRingBuffer(&m_errorBuffer, NULL);
#endif
	};

#ifdef _NEDITOR
	Nu32 read(NLPATH_WORKBENCH *pwb);						// fonction read "sp�ciale" qui copie les donn�es du path workbench pass� en param�tre plutot
	Nu32 load(NLPATH_WORKBENCH *pwb) { return read(pwb); }; // ... que de les lire dans un fichier. Cela permet au code du simulateur de rester tr�s proche du code "r�el" du robot
#endif														// ... tout en permettant de recup�rer directement les donn�es du path workbench.

	Nu32 read(FILE *pfile);
	Nu32 write(FILE *pfile);

	Nu32 save(const Nchar *pfilename);
	Nu32 load(const Nchar *pfilename);

	Nu32 importTxt(const Nchar *pfilename);

	void initialize(const NLTRAJECTORY_PACK *ppack);
	void estimate(const Nf32 left_encoder_rev_count, const Nf32 right_encoder_rev_count, const Nf32 gyro_angle);
	void updateTarget(NLTRAJECTORY_PACK *ppack, const Nf32 dt);
	NLFOLLOWER_TANK_OUTPUT *compute();
	Nu32 getMessage(NLTRJ_POSTED_MESSAGE *msg);

	const NLROBOTPOSE *getEstimatedPose() const { return &m_estimatedPose; };
	const NLTRAJECTORY_POINT *getTargetPoint() const { return &m_persistent.m_result; };

#ifdef _NEDITOR
	void drawDashBoard(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);
#endif

private:
	Nu32 m_flags;

	Nu32 m_message_id_from;
	Nu32 m_message_id_to;

	NLTRJ_POSTED_MESSAGE *m_pFromMessage;
	NLTRJ_POSTED_MESSAGE *m_pToMessage;

	// NLTRAJECTORY_PACK*	m_pPack;

	// Nf32					m_driveTrainAxleTrack;
	// Nf32					m_tractionWheelCircumference;
	// NLDRIVETRAIN_TANK_PACK	m_dtPack;

	NLDRIVETRAINSPECS m_driveTrainSpecifications;
	NLRAMSETE m_ramsete;

	NLROBOTPOSE m_estimatedPose;
	//	NLTRAJECTORY_POINT		m_targetPoint;

	Nf32 m_currentTime;
	Nf32 m_leftDistance;
	Nf32 m_rightDistance;

	NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT m_persistent;
	NLFOLLOWER_TANK_OUTPUT m_output;

#ifdef _NEDITOR
	Nf32 m_leftMove;
	Nf32 m_rightMove;
	NRINGBUFFER m_errorBuffer;
	void pushError(const NLFOLLOWER_TANK_DRAWDATA *pdd);
#endif
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define EXTENSION_NLFOLLOWER_TANK_BIN ".ftk"
#define EXTENSION_NLFOLLOWER_TANK_TXT ".txt"
#define SIGNATURE_NLFOLLOWER_TANK_BIN NMAKESIGNATURE('T', 'A', 'N', 'K')
#define SIGNATURE_NLFOLLOWER_TANK_TXT "tank"
#define VERSION_NLFOLLOWER_TANK_HEADER NMAKEVERSION(0, 0, 0) // >>> 2022/05/25
/*
typedef struct
{
}NLFOLLOWER_TANK_HEADER;
*/
