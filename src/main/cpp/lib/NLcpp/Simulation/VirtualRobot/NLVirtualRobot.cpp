#include "lib/N/NMemory.h"
#include "lib/N/NFlags.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"

#include "lib/NL/MotionControl/DriveTrain/NLOdometry.h"
#include "lib/NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"

#include "lib/NL/Simulation/VirtualRobot/NLVirtualRobot.h"
NLVIRTUAL_ROBOT::NLVIRTUAL_ROBOT()
{
	Nmem0(this, NLVIRTUAL_ROBOT);
	m_robot.setVirtualRobot(this);
}
NLVIRTUAL_ROBOT::NLVIRTUAL_ROBOT(const NLDRIVETRAINSPECS *pspecs, const NCOLOR *pframecolor, const NCOLOR *pwheelcolor)
{
	m_pSpecifications = (NLDRIVETRAINSPECS *)pspecs;
	m_robot.setVirtualRobot(this);

	if (pframecolor)
		m_frameColor = *pframecolor;
	else
		NSetColorf(&m_frameColor, NCOLOR_PRESET3F_WHITE, 1);

	if (pwheelcolor)
		m_wheelColor = *pwheelcolor;
	else
		NSetColorf(&m_wheelColor, NCOLOR_PRESET3F_WHITE, 1);

	m_prevLeft = 0.0f;
	m_prevRight = 0.0f;

	NIdentityMatrix3x3(&m_matrix);
	// NRotationMatrix3x3_AxisZ(&m_matrix, - NF32_PI_2);

	m_angle = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_centerPos = m_position;
}
void NLVIRTUAL_ROBOT::Init()
{
	NLCHARACTERIZATION_TABLE characterization_table(4);
	NLMOTOR_CHARACTERIZATION chz;
	characterization_table.importTxt("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/characterization_MultiVarLinearRegression.txt");
	// Definir ici toutes les parties MECHANICS du robot
	// DT Gearbox
	RegisterGearBox(0, 10.8f);
	RegisterGearBox(1, 10.8f);
	// DT Motors
	RegisterMotor(0, characterization_table.get(&chz, "L1", NTRUE), PRESET_NLMOTOR_SPECS_NEO, 0); // moteur NEO L1 dans gearbox 0
	RegisterMotor(1, characterization_table.get(&chz, "L2", NTRUE), PRESET_NLMOTOR_SPECS_NEO, 0); // moteur NEO L2 dans gearbox 0
	RegisterMotor(2, characterization_table.get(&chz, "R1", NTRUE), PRESET_NLMOTOR_SPECS_NEO, 1); // moteur NEO R1 dans gearbox 1
	RegisterMotor(3, characterization_table.get(&chz, "R2", NTRUE), PRESET_NLMOTOR_SPECS_NEO, 1); // moteur NEO R2 dans gearbox 1
	// Encoders
	RegisterEncoder(0, 2048, 0, NFALSE); // encodeur 0 d'une resolution de 2048 ( x 4 = 8192 en k4x !) branch� sur la sortie de la gearbox 0
	RegisterEncoder(1, 2048, 1, NFALSE); // encodeur 1 d'une resolution de 2048 ( x 4 = 8192 en k4x !) branch� sur la sortie de la gearbox 1
	// Gyro
	RegisterGyro(/*NLVIRTUAL_GYRO_DRIFT_5_DEG_PER_MIN*/ 0.0f);
}

void NLVIRTUAL_ROBOT::reset()
{
	m_prevLeft = 0.0f;
	m_prevRight = 0.0f;

	m_angle = 0.0f; // NF32_PI
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_centerPos = m_position;

	// !!!! WARNING !!!! La matrice du robot DOIT ABSOLUMENT �tre reset AVANT le reset du gyro !!! car le reset du gyro fait une copie
	// de l'axe Y de la matrice ...
	// Notes:
	// "NLVIRTUAL_ROBOT.m_angle" correspond � l'angle que fait le robot par rapport � l'axe X monde.
	// Cette valeur est ABSOLUE !!! ( c'est � dire pas relative � une position de d�part, comme peut l'�tre le m_angle du Gyro )
	// AINSI, ...
	//		  ... si le robot d�marre en mode forward,  alors m_angle = 0
	//		  ... si le robot d�marre en mode backward, alors m_angle = NF32_PI
	//
	NRotationMatrix3x3_AxisZ(&m_matrix, m_angle);

	// m_battery.reset();
	Nu8 i;
	// reset des ensembles de syst�mes
	for (i = 0; i < m_motorCount; i++)
	{
		if (BITGET(m_isMotorActive, i))
		{
			m_motor[i].reset();
		}
	} // reset moteurs
	for (i = 0; i < m_gearboxCount; i++)
	{
		if (BITGET(m_isGearBoxActive, i))
		{
			m_gearbox[i].reset();
		}
	} // reset gearboxes pour le moment inutile car NLVIRTUAL_GEARBOX.reset() reset uniquement tous ses driveinput... soit les output des moteurs, d�j� reset par les moteurs eux-m�mes
	for (i = 0; i < m_encoderCount; i++)
	{
		if (BITGET(m_isEncoderActive, i))
		{
			m_encoder[i].reset();
		}
	} // reset encoders
	if (BITGET(m_isGyroActive, 0))
	{
		m_gyro.reset();
	}
}

void NLVIRTUAL_ROBOT::RegisterMotor(const Nu8 channelid, const NLMOTOR_CHARACTERIZATION *pcharacterization, const Nf32 nominal_voltage, const Nf32 free_speed, const Nf32 free_current, const Nf32 stall_current, const Nf32 stall_torque, const Ns32 gearbox)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_MOTOR, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(BITGET(m_isMotorActive, channelid), NERROR_INCONSISTENT_REQUEST);					  // channelid d�j� occup� !

	BITSET(m_isMotorActive, channelid);
	m_motor[channelid].SetSpecs(nominal_voltage, free_speed, free_current, stall_current, stall_torque);
	m_motor[channelid].SetInternalCharacterization(pcharacterization);

	if ((gearbox >= 0) && (gearbox < CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GEARBOX))
	{
		m_gearbox[gearbox].addDriveInput(&m_motor[channelid].m_output);
	}
	m_motorCount++;
}

void NLVIRTUAL_ROBOT::UnregisterMotor(const Nu8 channelid)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_MOTOR, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(!BITGET(m_isMotorActive, channelid), NERROR_INCONSISTENT_REQUEST);					  // channelid non occup� !

	BITCLEAR(m_isMotorActive, channelid);
	m_motor[channelid].Clear();
	m_motorCount--;
}

void NLVIRTUAL_ROBOT::RegisterGearBox(const Nu8 channelid, const Nf32 gearbox_ratio)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GEARBOX, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(BITGET(m_isGearBoxActive, channelid), NERROR_INCONSISTENT_REQUEST);					// channelid d�j� occup� !

	BITSET(m_isGearBoxActive, channelid);
	m_gearbox[channelid].setGearBoxRatio(gearbox_ratio);
	m_gearboxCount++;
}

void NLVIRTUAL_ROBOT::UnregisterGearBox(const Nu8 channelid)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GEARBOX, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(!BITGET(m_isGearBoxActive, channelid), NERROR_INCONSISTENT_REQUEST);					// channelid non occup� !

	BITCLEAR(m_isGearBoxActive, channelid);
	m_gearbox[channelid].clear();
	m_gearboxCount--;
}

void NLVIRTUAL_ROBOT::RegisterEncoder(const Nu8 channelid, const Nu32 pulse_per_revolution, const Nu32 trackedchannel, const Nbool isconnectedtoamotor)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_ENCODER, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(BITGET(m_isEncoderActive, channelid), NERROR_INCONSISTENT_REQUEST);					// channelid d�j� occup� !

	BITSET(m_isEncoderActive, channelid);
	m_encoder[channelid].setup(pulse_per_revolution);
	if (isconnectedtoamotor)
	{
		if (BITGET(m_isMotorActive, trackedchannel))
		{
			m_encoder[channelid].SetDriveInput(&m_motor[trackedchannel].m_output);
		}
#ifdef _DEBUG
		else
		{
			NErrorIf(1, NERROR_INCONSISTENT_REQUEST); // moteur non connect�
		}
#endif
	}
	else
	{
		if (BITGET(m_isGearBoxActive, trackedchannel))
		{
			m_encoder[channelid].SetDriveInput(&m_gearbox[trackedchannel].m_output);
		}
#ifdef _DEBUG
		else
		{
			NErrorIf(1, NERROR_INCONSISTENT_REQUEST); // moteur non connect�
		}
#endif
	}
	m_encoderCount++;
}

void NLVIRTUAL_ROBOT::UnregisterEncoder(const Nu8 channelid)
{
	NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_ENCODER, NERROR_VALUE_OUTOFRANGE); // channelid trop grand
	NErrorIf(!BITGET(m_isEncoderActive, channelid), NERROR_INCONSISTENT_REQUEST);					// channelid non occup� !

	BITCLEAR(m_isEncoderActive, channelid);
	m_encoder[channelid].Clear();
	m_encoderCount--;
}

void NLVIRTUAL_ROBOT::RegisterGyro(/*const Nu8 channelid,*/ const Nf32 drift)
{
	// NErrorIf(channelid >= CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_GYRO, NERROR_VALUE_OUTOFRANGE);	// channelid trop grand
	NErrorIf(BITGET(m_isGyroActive, /*channelid*/ 0), NERROR_INCONSISTENT_REQUEST); // channelid d�j� occup� !

	BITSET(m_isGyroActive, /*channelid*/ 0);
	// m_gyro[channelid].setup(...);
	m_gyro.setup((NVEC2f32 *)&m_matrix.XAxis, drift);
	// m_gyroCount++;
}

void NLVIRTUAL_ROBOT::UnregisterGyro(/*const Nu8 channelid*/)
{
	BITCLEAR(m_isGyroActive, 0); // il n'y a qu'un seul Gyro pour le moment doncc channelid = 0
	m_gyro.Clear();
	// m_gyroCount--;			//... Un seul gyro donc compteur inutile pour l'instant
}
/*
void NLVIRTUAL_ROBOT::Stop()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
	NIdentityMatrix3x3(&m_matrix);
	m_prevAngle = 0.0f;
	m_prevLeft = 0.0f;
	m_prevRight = 0.0f;

	for (Nu32 i = 0; i < CONSTANT_NLVIRTUAL_ROBOT_MAX_NUMBER_OF_MOTOR; i++)
		m_motor[i].Stop();
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Mise sous tension du Robot Virtuel. Correspond � la mise sous tension du 'VRAI' robot.
 */
// ------------------------------------------------------------------------------------------
void NLVIRTUAL_ROBOT::switchOn(NLPATH_WORKBENCH *pwb, const Nf32 time)
{
	NErrorIf(ISFLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON), NERROR_FLAG_INCONSISTENCY);		  // Le robot est d�j� sous tension !!!
	NErrorIf(ISFLAG_ON(m_robot.m_flags, FLAG_NLROBOT_IS_INITIALIZED), NERROR_FLAG_INCONSISTENCY); // Le robot d�j� initializ� !!

	if (time > 0.0f)
	{
		m_SimulationLeftTime = time;
		// Reset "mat�riel" � effectuer � l'allumage du robot:
		reset();
		FLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON);

		// Initialisation de la classe Robot:
		m_robot.RobotInit(pwb);
		FLAG_ON(m_robot.m_flags, FLAG_NLROBOT_IS_INITIALIZED);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
#endif
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Arr�t du Robot Virtuel ( hors tension ). Correspond � l'arr�t du 'VRAI' robot.
 */
// ------------------------------------------------------------------------------------------
void NLVIRTUAL_ROBOT::switchOff()
{
	NErrorIf(ISFLAG_OFF(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON), NERROR_FLAG_INCONSISTENCY);		   // Le robot est d�j� hors tension !!!
	NErrorIf(ISFLAG_OFF(m_robot.m_flags, FLAG_NLROBOT_IS_INITIALIZED), NERROR_FLAG_INCONSISTENCY); // Le robot est d�j� off !!

	FLAG_OFF(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON);
	FLAG_OFF(m_robot.m_flags, FLAG_NLROBOT_IS_INITIALIZED);
	m_SimulationLeftTime = 0.0f;
}

Nu32 NLVIRTUAL_ROBOT::update(const Nf32 dt)
{
	// if (m_SimulationLeftTime)
	if (ISFLAG_ON(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON))
	{
		NErrorIf(ISFLAG_OFF(m_flags, FLAG_NLVIRTUAL_ROBOT_IS_ON), NERROR_FLAG_INCONSISTENCY);		   // Le robot est  hors tension !!!
		NErrorIf(ISFLAG_OFF(m_robot.m_flags, FLAG_NLROBOT_IS_INITIALIZED), NERROR_FLAG_INCONSISTENCY); // Le robot est  hors tension !!!

		Nf32 udt = NMIN(m_SimulationLeftTime, dt);
		NErrorIf(udt <= 0.0f, NERROR_WRONG_VALUE); // udt doit �tre non null positif , c'est un intervalle de temps !

		m_robot.RobotPeriodic(udt);

		// update batteries, moteurs, encodeurs, position ... etc
		// le tout � une frequence "�lev�e" :
		//	Pour se faire on d�coupe l'intervalle dt en sous intervalles de "CONSTANT_NLVIRTUAL_DRIVE_TRAIN_INTEGRATION_DT secondes"
		Nu32 iter = (Nu32)(udt / CONSTANT_NLVIRTUAL_ROBOT_INTEGRATION_DT) + 1;
		Nf32 ddt = udt / (Nf32)iter;
		Nf32 left = 0.0f;
		Nf32 right = 0.0f;
		Nf32 l, r;

		Nf32 v, w;
		Nu8 m, g, e;

		for (Nu32 i = iter; i > 0; i--)
		{
			// m_battery.update(ddt);

			// Update des ensembles de syst�mes
			for (m = 0; m < m_motorCount; m++)
			{
				if (BITGET(m_isMotorActive, m))
				{
					m_motor[m].update(ddt);
				}
			} // update moteurs
			for (g = 0; g < m_gearboxCount; g++)
			{
				if (BITGET(m_isGearBoxActive, g))
				{
					m_gearbox[g].update(ddt);
				}
			} // update gearboxes
			for (e = 0; e < m_encoderCount; e++)
			{
				if (BITGET(m_isEncoderActive, e))
				{
					m_encoder[e].update(ddt);
				}
			} // update encoders

			//
			l = m_gearbox[0].m_output.m_angularPosition * m_pSpecifications->m_wheelRadius; // l est la distance parcourue par la roue
			r = m_gearbox[1].m_output.m_angularPosition * m_pSpecifications->m_wheelRadius;

			left = l - m_prevLeft;
			right = r - m_prevRight;

			m_prevLeft = l;
			m_prevRight = r;

			// M�thode simplifi�e:
			v = NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(left, right);
			w = NLODOMETRY_DRIVETRAIN_W_FROM_WHEELS(left, right, m_pSpecifications->m_axleTrack);
			m_angle += w;

			m_position.x += cos(m_angle) * v;
			m_position.y += sin(m_angle) * v;
			m_position.z = 0.0f;

			m_centerPos = m_position;

			/*
			// M�thode compl�te:
			v = NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(left, right);
			w = NLODOMETRY_DRIVETRAIN_W_FROM_WHEELS(left, right, m_pSpecifications->m_axleTrack);
			if (w == 0.0f)
			{
				m_position.x += cos(m_angle)*v*ddt;
				m_position.y += sin(m_angle)*v*ddt;
				m_position.z = 0.0f;
				m_centerPos = m_position;
			}
			else
			{
				rcenter = v / w;
				m_centerPos.x = m_position.x - rcenter * sin(m_angle);//  ==m_position.x - rcenter *cos(m_angle - NF32_PI_2);
				m_centerPos.y = m_position.y + rcenter * cos(m_angle);//  ==m_position.y - rcenter *sin(m_angle - NF32_PI_2);
				m_centerPos.z = 0.0f;

				m_angle += w*ddt;
				m_position.x = m_centerPos.x + rcenter * cos(m_angle - NF32_PI_2);
				m_position.y = m_centerPos.y + rcenter * sin(m_angle - NF32_PI_2);
				m_position.z = 0.0f;
			}
			*/
		}
		// Mise � jour de la matrice ( orientation "r�elle" du robot )
		NRotationMatrix3x3_AxisZ(&m_matrix, m_angle);
		if (BITGET(m_isGyroActive, 0))
			m_gyro.update(udt);

		m_SimulationLeftTime -= udt;
		if (m_SimulationLeftTime <= 0.0f)
		{
			switchOff();
		}
		return 1;
	}
	else
		return 0;
}

void NLVIRTUAL_ROBOT::draw()
{

	NVEC3 pos;

	// on trace la DT � sa position "R�elle" ( en opposition � la position estim�e par odometrie )
	pos.x = m_position.x; //- m_pSpecifications->m_centerOfMass.x;
	pos.y = m_position.y; //- m_pSpecifications->m_centerOfMass.y;
	pos.z = m_position.z; //- m_pSpecifications->m_centerOfMass.z;
	// tracage du cadre
	drawRobotShape(&m_robot.m_TrajectoryPack.m_matrix, &m_matrix, &pos, &m_frameColor);

	m_robot.draw();
}
void NLVIRTUAL_ROBOT::drawDashBoard(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	m_robot.drawDashBoard(p2docs, pickpack);

	NVEC3 o;
	o.x = p2docs->m_Origin.x;
	o.y = p2docs->m_Origin.y;
	o.z = 0.0f;
	m_gyro.draw(&o);
}

/*
void NLVIRTUAL_ROBOT::drawRobotShape( const NMATRIX3x3 *prot, const NVEC3* ppos, const NCOLOR *pcolor)
{

	NUT_Draw_3DMatrix3x3(prot, ppos, 1.0f);
	NUT_SetDrawConstructionPlaneMatrix3x3(prot);

	// Tracage des 6 roues

	NVEC3f32	pos;
	NVEC2f32	xtd;

	pos.z = 0.0f;
	// Xtend d'une traction wheel
	xtd.x = m_pSpecifications->m_wheelRadius;
	xtd.y = m_pSpecifications->m_wheelWidth;

	// c�t� droit
	// ------------------------------
	pos.x = ppos->x + prot->XAxis.x * m_pSpecifications->m_wheelBase * 0.5f + prot->YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = ppos->y + prot->XAxis.y * m_pSpecifications->m_wheelBase * 0.5f + prot->YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = ppos->x + prot->YAxis.x *m_pSpecifications->m_axleTrack*0.5f;
	pos.y = ppos->y + prot->YAxis.y *m_pSpecifications->m_axleTrack*0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = ppos->x + prot->XAxis.x * m_pSpecifications->m_wheelBase * 0.5f - prot->YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = ppos->y + prot->XAxis.y * m_pSpecifications->m_wheelBase * 0.5f - prot->YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	// c�t� gauche
	// ------------------------------
	pos.x = ppos->x - prot->XAxis.x *m_pSpecifications->m_axleTrack*0.5f + prot->YAxis.x*m_pSpecifications->m_wheelBase*0.5f;
	pos.y = ppos->y - prot->XAxis.y *m_pSpecifications->m_axleTrack*0.5f + prot->YAxis.y*m_pSpecifications->m_wheelBase*0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = ppos->x - prot->XAxis.x *m_pSpecifications->m_axleTrack*0.5f;
	pos.y = ppos->y - prot->XAxis.y *m_pSpecifications->m_axleTrack*0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = ppos->x - prot->XAxis.x *m_pSpecifications->m_axleTrack*0.5f - prot->YAxis.x*m_pSpecifications->m_wheelBase*0.5f;
	pos.y = ppos->y - prot->XAxis.y *m_pSpecifications->m_axleTrack*0.5f - prot->YAxis.y*m_pSpecifications->m_wheelBase*0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	// cadre int�rieur
	xtd.x = m_pSpecifications->m_size.x*0.5f;
	xtd.y = m_pSpecifications->m_size.y*0.5f;
	NUT_Draw_Quad(ppos, &xtd, pcolor);

	// cadre ext�rieur
	xtd.x = m_pSpecifications->m_size.x*0.5f + 0.1f;
	xtd.y = m_pSpecifications->m_size.y*0.5f + 0.1f;
	NUT_Draw_Quad(ppos, &xtd, pcolor);
	/ *
	// Vecteur Y+
	pos.x = ppos->x + prot->YAxis.x*m_pSpecifications->m_wheelBase*1.25f;
	pos.y = ppos->y + prot->YAxis.y*m_pSpecifications->m_wheelBase*1.25f;
	NUT_Draw_Segment(ppos, &pos, pcolor);
	* /
}
*/
void NLVIRTUAL_ROBOT::drawRobotShape(const NMATRIX *pbase, const NMATRIX3x3 *prot, const NVEC3 *ppos, const NCOLOR *pcolor)
{
	NMATRIX mx, mxr;
	NComposeMatrix(&mx, prot, ppos);
	NMulMatrix(&mxr, &mx, pbase);

	NMATRIX3x3 mx3;
	NVEC3f32 o;
	// extract
	mx3.f00 = mxr.f00;
	mx3.f10 = mxr.f10;
	mx3.f20 = mxr.f20;
	mx3.f01 = mxr.f01;
	mx3.f11 = mxr.f11;
	mx3.f21 = mxr.f21;
	mx3.f02 = mxr.f02;
	mx3.f12 = mxr.f12;
	mx3.f22 = mxr.f22;
	o.x = mxr.f30;
	o.y = mxr.f31;
	o.z = mxr.f32;

	NUT_Draw_3DMatrix3x3(&mx3, &o, 1.0f);
	NUT_SetDrawConstructionPlaneMatrix3x3(&mx3);

	// Tracage des 6 roues

	NVEC3f32 pos;
	NVEC2f32 xtd;

	pos.z = 0.0f;
	// Xtend d'une traction wheel
	xtd.x = m_pSpecifications->m_wheelRadius;
	xtd.y = m_pSpecifications->m_wheelWidth;

	// c�t� droit
	// ------------------------------
	pos.x = o.x + mx3.XAxis.x * m_pSpecifications->m_wheelBase * 0.5f + mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y + mx3.XAxis.y * m_pSpecifications->m_wheelBase * 0.5f + mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = o.x + mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y + mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = o.x - mx3.XAxis.x * m_pSpecifications->m_wheelBase * 0.5f + mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y - mx3.XAxis.y * m_pSpecifications->m_wheelBase * 0.5f + mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	// c�t� gauche
	// ------------------------------
	pos.x = o.x - mx3.XAxis.x * m_pSpecifications->m_wheelBase * 0.5f - mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y - mx3.XAxis.y * m_pSpecifications->m_wheelBase * 0.5f - mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = o.x - mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y - mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = o.x + mx3.XAxis.x * m_pSpecifications->m_wheelBase * 0.5f - mx3.YAxis.x * m_pSpecifications->m_axleTrack * 0.5f;
	pos.y = o.y + mx3.XAxis.y * m_pSpecifications->m_wheelBase * 0.5f - mx3.YAxis.y * m_pSpecifications->m_axleTrack * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	pos.x = o.x;
	pos.y = o.y;
	// cadre int�rieur
	xtd.x = m_pSpecifications->m_size.x * 0.5f;
	xtd.y = m_pSpecifications->m_size.y * 0.5f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	// cadre ext�rieur
	xtd.x = m_pSpecifications->m_size.x * 0.5f + 0.1f;
	xtd.y = m_pSpecifications->m_size.y * 0.5f + 0.1f;
	NUT_Draw_Quad(&pos, &xtd, pcolor);

	/*
	// Vecteur Y+
	pos.x = o.x + pmx->YAxis.x * m_pSpecifications->m_wheelBase * 1.25f;
	pos.y = o.y + pmx->YAxis.y * m_pSpecifications->m_wheelBase * 1.25f;
	NUT_Draw_Segment(ppos, &pos, pcolor);
	*/
}

/*


NLDifferentialDriveBase::NLDifferentialDriveBase()
{
	Nmem0(this, NLDifferentialDriveBase);
	m_rightWheelRadius	= DEFAULT_NLDIFFERENTIALDRIVEBASE_WHEELRADIUS;
	m_leftWheelRadius	= DEFAULT_NLDIFFERENTIALDRIVEBASE_WHEELRADIUS;
	m_rightEncoder = NLEncoder(DEFAULT_NLDIFFERENTIALDRIVEBASE_ENCODER_TICKS_PER_REVOLUTION);
	m_leftEncoder = NLEncoder(DEFAULT_NLDIFFERENTIALDRIVEBASE_ENCODER_TICKS_PER_REVOLUTION);
	m_axleTrack = DEFAULT_NLDIFFERENTIALDRIVEBASE_AXLETRACK;
	m_centerOfMassY = DEFAULT_NLDIFFERENTIALDRIVEBASE_CENTEROFMASSY;

#ifdef _NEDITOR
	m_frameExtend.x = DEFAULT_NLDIFFERENTIALDRIVEBASE_FRAMEWIDTH;
	m_frameExtend.y = DEFAULT_NLDIFFERENTIALDRIVEBASE_FRAMELENGTH;
#endif
}

NLDifferentialDriveBase::NLDifferentialDriveBase(const Nf32 rightwheelradius, const Nf32 lefttwheelradius, const Nf32 rightencoder_revolticks, const Nf32 leftencoder_revolticks)
{
	m_rightWheelRadius		= rightwheelradius;
	m_leftWheelRadius		= lefttwheelradius;
	m_rightEncoder			= NLEncoder(rightencoder_revolticks);
	m_leftEncoder			= NLEncoder(leftencoder_revolticks);
	m_leftMotor				= NLVirtualMotor(0.0f);
	m_rightMotor			= NLVirtualMotor(0.0f);
}

NLDifferentialDriveBase::~NLDifferentialDriveBase()
{

}

void NLDifferentialDriveBase::update(const Nf32 dt)
{
	Nf32	ll, lr;

	// Update des moteurs virtuels
	Nf32 ml = m_leftMotor.m_power	*m_leftMotor.m_rps	* dt;
	Nf32 mr = m_rightMotor.m_power * m_rightMotor.m_rps	* dt;

	// update des encodeurs.
	m_leftEncoder.update( ml );
	m_rightEncoder.update( mr );

	// Update des valeurs "REELLES" de la Drivebase Virtuelle
	// Distances (arcs) parcourues par la roue gauche d'une part (ll) et la roue droite d'autre part (lr)
	ll = NF32_2PI * ml	* m_leftWheelRadius;	//m_leftEncoder.m_deltaRad	* m_leftWheelRadius;
	lr = NF32_2PI * mr	* m_rightWheelRadius;	//m_rightEncoder.m_deltaRad	* m_rightWheelRadius;
	// Rotation de la base
	m_dang = NLODOMETRY_WBASE_FROM_WHEELS(ll, lr, m_axleTrack);// (lr - ll) / m_frameExtend.x;
	// D�placement de la base
	m_dd = NLODOMETRY_VBASE_FROM_WHEELS(ll, lr); ;//  (lr + ll) / 2.0f;
	// D�placement total et Orientation de la base
	m_s		+= m_dd;
	m_ang	+= m_dang;
	// vitesse de la base
	m_v.x = cos(m_ang)*m_dd;
	m_v.y = sin(m_ang)*m_dd;
	// Position de la base
	m_pos.x += m_v.x;
	m_pos.y += m_v.y;

	// Update des valeurs "estim�es" par encodeur de la Drivebase Virtuelle
	// Distances (arcs) parcourues par la roue gauche d'une part (ll) et la roue droite d'autre part (lr)
	ll = m_leftEncoder.m_deltaRad	* m_leftWheelRadius;
	lr = m_rightEncoder.m_deltaRad	* m_rightWheelRadius;
	// Rotation de la base
	m_encod_dang = NLODOMETRY_WBASE_FROM_WHEELS(ll, lr, m_axleTrack);// (lr - ll) / m_frameExtend.x;
	// D�placement de la base
	m_encod_dd = NLODOMETRY_VBASE_FROM_WHEELS(ll, lr); ;//  (lr + ll) / 2.0f;
	// D�placement total et Orientation de la base
	m_encod_d += m_encod_dd;
	m_encod_ang += m_encod_dang;
	// vitesse de la base
	m_encod_v.x = cos(m_encod_ang)*m_encod_dd;
	m_encod_v.y = sin(m_encod_ang)*m_encod_dd;
	// Position de la base
	m_encod_pos.x += m_encod_v.x;
	m_encod_pos.y += m_encod_v.y;
}

void NLDifferentialDriveBase::initialPose(const Nf32 orient, const Nf32 x, const Nf32 y)
{
	m_v.x	= 0.0f;
	m_v.y	= 0.0f;

	m_dd	= 0.0f;
	m_s		= 0.0f;
	m_dang	= 0.0f;
	m_pos.x = x;
	m_pos.y = y;
	m_ang	= orient;

	// encodeurs
	m_encod_v.x = 0.0f;
	m_encod_v.y = 0.0f;

	m_encod_dd = 0.0f;
	m_encod_d = 0.0f;
	m_encod_dang = 0.0f;
	m_encod_pos.x = x;
	m_encod_pos.y = y;
	m_encod_ang = orient;
}

void NLDifferentialDriveBase::draw(const NCOLOR *pcolor1, const NCOLOR *pcolor2, const NVEC2f32 *porigin, const Nf32 scale, const Nbool real)
{
	NUTDRAWVERTEX	R;
	NVEC2f32		axe_x, axe_y;
	Nf32			_cos,_sin;
	NVEC3f32		c;
	NVEC3f32		g,d;			// centre roue gauche et droit
	NVEC3f32		rg,rd;			// rayon roue gauche et droite
	NVEC3f32		l, w;			// demi longueur et demi largeur
	NVEC3f32		fd,fg,bd,bg;	// les 4 angles de la base ( avant(front) droit, avant gauche, arriere(back) droit, arriere gauche )
	Nf32			axe_scale;

	if (real) // use "real" position of the "virtual" robot
	{
		_cos = cos(m_ang);
		_sin = sin(m_ang);
	}
	else // use "estimated" position of the "virtual" robot ( estim�e avec les valeurs mesur�es aux encodeurs )
	{
		_cos = cos(m_encod_ang);
		_sin = sin(m_encod_ang);
	}

	axe_x.x = _cos*scale;		axe_y.x = -_sin*scale;
	axe_x.y = _sin*scale;		axe_y.y =  _cos*scale;

	if(pcolor1)
		R.Color0_4f = *pcolor1;
	else
	{
		if(real)
			NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_WHITE, 1);
		else
			NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_RED, 1);
	}

	// Centre de la base
	if (porigin)
	{
		if (real)
		{
			c.x = porigin->x + m_pos.x * scale;
			c.y = porigin->y + m_pos.y * scale;
			c.z = 0.0f;
		}
		else
		{
			c.x = porigin->x + m_encod_pos.x * scale;
			c.y = porigin->y + m_encod_pos.y * scale;
			c.z = 0.0f;
		}
	}
	else
	{
		if (real)
		{
			c.x = m_pos.x * scale;
			c.y = m_pos.y * scale;
			c.z = 0.0f;
		}
		else
		{
			c.x = m_encod_pos.x * scale;
			c.y = m_encod_pos.y * scale;
			c.z = 0.0f;
		}
	}


	// longueur ( l'axe x est align� sur la direction du robot )
	l.x = axe_x.x*m_frameExtend.y*0.5f;
	l.y = axe_x.y*m_frameExtend.y*0.5f;
	l.z = 0.0f;
	// largeur ( l'axe y est perpendiculaire � la direction du robot )
	w.x = axe_y.x*m_frameExtend.x*0.5f;
	w.y = axe_y.y*m_frameExtend.x*0.5f;
	w.z = 0.0f;

	// centre roue gauche
	g = c;
	g.x += w.x;
	g.y += w.y;

	// centre roue droite
	d = c;
	d.x -= w.x;
	d.y -= w.y;
	//rayon roue gauche
	rg.x = axe_x.x*m_leftWheelRadius;
	rg.y = axe_x.y*m_leftWheelRadius;
	rg.z = 0.0f;
	//rayon roue droite
	rd.x = axe_x.x*m_rightWheelRadius;
	rd.y = axe_x.y*m_rightWheelRadius;
	rd.z = 0.0f;

	// Draw Entraxe
	R.Position_3f = g;
	NUT_DrawPencil_From(&R);
	R.Position_3f = d;
	NUT_DrawPencil_LineTo(&R);

	// Roue Gauche
	R.Position_3f = g;
	R.Position_3f.x -= rg.x;
	R.Position_3f.y -= rg.y;
	NUT_DrawPencil_From(&R);
	R.Position_3f = g;
	R.Position_3f.x += rg.x;
	R.Position_3f.y += rg.y;
	NUT_DrawPencil_LineTo(&R);

	// Roue Droite
	R.Position_3f = d;
	R.Position_3f.x -= rd.x;
	R.Position_3f.y -= rd.y;
	NUT_DrawPencil_From(&R);
	R.Position_3f = d;
	R.Position_3f.x += rd.x;
	R.Position_3f.y += rd.y;
	NUT_DrawPencil_LineTo(&R);

	// Angles
	fg = g;					fd = d;
	fg.x += l.x;			fd.x += l.x;
	fg.y += l.y;			fd.y += l.y;

	bg = g;					bd = d;
	bg.x -= l.x;			bd.x -= l.x;
	bg.y -= l.y;			bd.y -= l.y;

	// Frame
	if (pcolor2)
		R.Color0_4f = *pcolor1;
	else
	{
	if (real)
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_GREEN, 1);
	else
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_PINK_ORCHID, 1);
	}
	R.Position_3f = fg;
	NUT_DrawPencil_From(&R);
	R.Position_3f = fd;
	NUT_DrawPencil_LineTo(&R);
	R.Position_3f = bd;
	NUT_DrawPencil_LineTo(&R);
	R.Position_3f = bg;
	NUT_DrawPencil_LineTo(&R);
	R.Position_3f = fg;
	NUT_DrawPencil_LineTo(&R);

	// Tracage repere XY
	axe_scale = NMAX(m_frameExtend.x, m_frameExtend.y)*0.75f; // plus grand que la demi largeur/longueur ...

	// Axe X
	if(real)
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_RED, 1);
	else
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_ORANGE, 1);

	R.Position_3f = c;
	NUT_DrawPencil_From(&R);
	R.Position_3f.x += axe_x.x*axe_scale;
	R.Position_3f.y += axe_x.y*axe_scale;
	NUT_DrawPencil_LineTo(&R);

	// Axe Y
	if(real)
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_GREEN, 1);
	else
		NSetColorf(&R.Color0_4f, NCOLOR_PRESET3F_GREEN_EMERALD, 1);

	R.Position_3f = c;
	NUT_DrawPencil_From(&R);
	R.Position_3f.x += axe_y.x*axe_scale;
	R.Position_3f.y += axe_y.y*axe_scale;
	NUT_DrawPencil_LineTo(&R);

}
*/