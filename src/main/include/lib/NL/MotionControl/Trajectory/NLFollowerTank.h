#pragma once
#include "../../../N/NType.h"
#include "NLTrajectoryChunk.h"
#include "../NLRobotPose.h"

#include "NLTrajectoryPack.h"
#include "NLTrajectoryPackGetPointPersistentResult.h"

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#endif

typedef struct NLFOLLOWER_TANK_OUTPUT	NLFOLLOWER_TANK_OUTPUT;
struct NLFOLLOWER_TANK_OUTPUT
{
	void null() { m_leftVelocity = 0.0f; m_leftAcceleration = 0.0f; m_rightVelocity = 0.0f; m_rightAcceleration = 0.0f; }
	Nf32	m_leftVelocity;		// Vitesse à appliquer à gauche
	Nf32	m_leftAcceleration;	// Acceleration à appliquer à gauche
	Nf32	m_rightVelocity;	// Vitesse à appliquer à droite
	Nf32	m_rightAcceleration;// Acceleration à appliquer à droite
};


class NLFOLLOWER_TANK
{
#define FLAG_REVERSE_MODE	BIT_0

public:
	enum GEARBOX
	{
		LEFT = 0,
		RIGHT = 1,
		NB = 2
	};
	NLFOLLOWER_TANK() { Nmem0(this, NLFOLLOWER_TANK); };

	~NLFOLLOWER_TANK(){};

#ifdef _NEDITOR
	Nu32	read(NLPATH_WORKBENCH* pwb);				// fonction read "spéciale" qui copie les données du path workbench passé en paramètre plutot
	Nu32	load(NLPATH_WORKBENCH* pwb) { return read(pwb);};	// ... que de les lire dans un fichier. Cela permet au code du simulateur de rester très proche du code "réel" du robot	
#endif													// ... tout en permettant de recupérer directement les données du path workbench. 	
	Nu32	read(FILE* pfile);
	Nu32	write(FILE* pfile);

	Nu32	save(const Nchar* pfilename);
	Nu32	load(const Nchar* pfilename);

	Nu32	importTxt(const Nchar* pfilename);

	void							initialize(const NLTRAJECTORY_PACK* ppack);
	void							estimate(const Nf32 left_encoder_rev_count, const Nf32 right_encoder_rev_count, const Nf32 gyro_angle);
	void							updateTarget(NLTRAJECTORY_PACK* ppack, const Nf32 dt);
	const NLFOLLOWER_TANK_OUTPUT*	compute();
	
	const NLROBOTPOSE*				getEstimatedPose() const{ return &m_estimatedPose; };
	const NLTRAJECTORY_POINT*		getTargetPoint() const{ return &m_targetPoint; };

#ifdef _NEDITOR
	void draw(NL2DOCS * p2docs);
#endif

private:
	Nu32					m_flags;

	Nu16					m_message_id_from;
	Nu16					m_message_id_to;

	//NLTRAJECTORY_PACK*	m_pPack;
	
	//Nf32					m_driveTrainAxleTrack;
	//Nf32					m_tractionWheelCircumference;
	//NLDRIVETRAIN_TANK_PACK	m_dtPack;
	
	NLDRIVETRAINSPECS		m_driveTrainSpecifications;
	NLRAMSETE				m_ramsete;

	NLROBOTPOSE				m_estimatedPose;
	NLTRAJECTORY_POINT		m_targetPoint;

	Nf32					m_currentTime;
	Nf32					m_leftDistance;
	Nf32					m_rightDistance;

	NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT	m_persistent;
	NLFOLLOWER_TANK_OUTPUT							m_output;
};



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define EXTENSION_NLFOLLOWER_TANK_BIN		".ftk"
#define EXTENSION_NLFOLLOWER_TANK_TXT		".txt"
#define SIGNATURE_NLFOLLOWER_TANK_BIN		NMAKESIGNATURE('T','A','N','K')
#define SIGNATURE_NLFOLLOWER_TANK_TXT		"tank"
#define VERSION_NLFOLLOWER_TANK_HEADER		NMAKEVERSION(0,0,0)				// >>> 2022/05/25
/*
typedef struct
{
}NLFOLLOWER_TANK_HEADER;
*/
