#include "../../N/NMemory.h"
#include "../../N/NMath.h"

#include "../../N/Miscellaneous/NColor.h"
#include "../../N/NErrorHandling.h"
#include "../../N/Utilities/Draw/NUT_Draw.h"
#include "../../N/Utilities/Draw/NUT_DrawPencil.h"

#include "../MotionControl/DriveTrain/NLOdometry.h"

#include "./VirtualRobot/NLVirtualRobot.h"
#include "NLRobot.h"

/*
void NLROBOT::fromWorkbench(const NLPATH_WORKBENCH* pwb)
{
	m_pDriveTrainSpecs = (NLDRIVETRAINSPECS*)pwb->m_pDriveTrainSpecifications;
	if (m_pTrajectoryStateSPack != (NLTRAJECTORY_BASIC_PACK*)pwb->m_pTrajectoryStateSPack)
	{
		m_pTrajectoryStateSPack = (NLTRAJECTORY_BASIC_PACK*)pwb->m_pTrajectoryStateSPack;
		m_currrentSState.null();
	}
	if (m_pTrajectoryStatePack != (NLTRAJECTORY_PACK*)pwb->m_pTrajectoryStatePack)
	{
		m_pTrajectoryStatePack = (NLTRAJECTORY_PACK*)pwb->m_pTrajectoryStatePack;
		m_currrentState.null();
	}
}
*/

void NLROBOT::RobotInit(NLPATH_WORKBENCH *pwb)
{
	NErrorIf(!m_pVirtualRobot, NERROR_NULL_POINTER); // A virtual Robot has to be defined !!!
	m_gyro.Init(0, m_pVirtualRobot);

	// a reprendre :
	/*********************************************************************************************************************
	NLCHARACTERIZATION_TABLE	characterization_table(4);
	characterization_table.load("characterization_MultiVarLinearRegression.txt");
	characterization_table.get(&m_CrtzL1, "L1", NFALSE);
	characterization_table.get(&m_CrtzL2, "L2", NFALSE);
	characterization_table.get(&m_CrtzR1, "R1", NFALSE);
	characterization_table.get(&m_CrtzR2, "R2", NFALSE);

	m_moteurL1.Init(0, m_pVirtualRobot);
	m_moteurL2.Init(1, m_pVirtualRobot);
	m_moteurR1.Init(2, m_pVirtualRobot);
	m_moteurR2.Init(3, m_pVirtualRobot);

	m_leftGearboxEncoder.Init(0, NLVENCODER_ENCODING_K4X, m_pVirtualRobot);
	m_rightGearboxEncoder.Init(1, NLVENCODER_ENCODING_K4X, m_pVirtualRobot);



	// Read TrajectoryFollowing external data:
	// En mode simulation ces donn�es sont 'lues' ( copi�es en fait ) depuis le path workbench courant.
	// En mode "r�el" elles sont effectivement pr�sentes sous forme de fichiers charg�s � l'initialisation.
	Nu32 read_error_count = 0;

	read_error_count += m_DriveTrainSpecs.read(pwb)? 0:1;
	read_error_count += m_ramsete.read(pwb) ? 0:1;
	read_error_count += m_TrajectoryPack.read(pwb) ? 0:1;
	read_error_count += m_TrajectoryPack.m_chunksArray.Size ? 0:1;

	m_currrentPoint.null();
	m_TrajectoryPack.initializePersistentData();
	//read_error_count += m_TrajectoryStateSPack.read(pwb) ? 0:1;
	//m_currrentSState.null();


	if (read_error_count)
		m_state = NLROBOT::STATE::PATH_ERROR;
	else
		m_state = NLROBOT::STATE::PATH_FOLLOWING;

	m_dsLeftWheel	= 0.0f;
	m_dsRightWheel	= 0.0f;
	m_estimatedPose.reset();
	**********************************************************************************************************************/

	NLCHARACTERIZATION_TABLE characterization_table(4);
	characterization_table.importTxt("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/characterization_MultiVarLinearRegression.txt");
	characterization_table.get(&m_CrtzL1, "L1", NFALSE);
	characterization_table.get(&m_CrtzL2, "L2", NFALSE);
	characterization_table.get(&m_CrtzR1, "R1", NFALSE);
	characterization_table.get(&m_CrtzR2, "R2", NFALSE);

	m_moteurL1.Init(0, m_pVirtualRobot);
	m_moteurL2.Init(1, m_pVirtualRobot);
	m_moteurR1.Init(2, m_pVirtualRobot);
	m_moteurR2.Init(3, m_pVirtualRobot);

	m_leftGearboxEncoder.Init(0, NLVENCODER_ENCODING_K4X, m_pVirtualRobot);
	m_rightGearboxEncoder.Init(1, NLVENCODER_ENCODING_K4X, m_pVirtualRobot);

	m_TrajectoryPack.load(/*"trajectory1.pak"*/ pwb);
	m_follower.load(/*"scrumtrooper.ftk"*/ pwb);
	m_follower.initialize(&m_TrajectoryPack);
	m_state = NLROBOT::STATE::PATH_FOLLOWING;

	m_logCsv.setItem(0, "Vleft", 5, &m_follower.m_output.m_leftVelocity);
	m_logCsv.setItem(1, "Rleft", 5, &m_follower.m_output.m_rightVelocity);

	if (m_logCsv.isOpen())
		m_logCsv.close();

	m_logCsv.open(nullptr, true);
}

void NLROBOT::RobotPeriodic(const Nf32 dt)
{
	NLRAMSETEOUTPUT output;
	const NLFOLLOWER_TANK_OUTPUT *pout;

	switch (m_state)
	{
	case NLROBOT::STATE::PATH_ERROR:
		break;

	case NLROBOT::STATE::PATH_FOLLOWING:
		// *****************************************************    'THE' METHOD(e)
		// A) Feed back:
		// avec les encodeurs on estime la position du robot:
		//			l = distance parcourue par la roue gauche depuis le dernier reset encodeur.
		//			r = distance parcourue par la roue droite depuis le dernier reset encodeur.
		//
		//			dl et dr = distances parcourues par les roues gauche et droite depuis le dernier call.
		//			(note dl/dt = vitesse roue gauche et dr/dt = vitesse roue droite  )
		//
		/*
		l = (m_leftGearboxEncoder.getRaw() / 8192.0f) * NF32_2PI * m_DriveTrainSpecs.m_wheelRadius;
		r = (m_rightGearboxEncoder.getRaw() / 8192.0f) * NF32_2PI * m_DriveTrainSpecs.m_wheelRadius;
		dl = l - m_dsLeftWheel;
		dr = r - m_dsRightWheel;
		m_dsLeftWheel = l;
		m_dsRightWheel = r;
		// forward:
		m_estimatedPose.odometryUpdate(&m_DriveTrainSpecs, dl, dr, m_gyro.get());
		*/
		// backward:
		// m_estimatedPose.odometryUpdate(&m_DriveTrainSpecs, -dr, -dl, m_gyro.get());
		m_follower.estimate(m_leftGearboxEncoder.getRaw() / 8192.0f, m_rightGearboxEncoder.getRaw() / 8192.0f, m_gyro.get());

		/*
		// B) Feed forward : State ( full )
		m_currrentPoint.m_kin.m_t += dt;
		m_TrajectoryPack.getPoint(&m_currrentPoint, m_currrentPoint.m_kin.m_t);
		*/
		m_follower.updateTarget(&m_TrajectoryPack, dt);
		// C) Ramsete:
		// m_ramsete.update2(&output, &m_DriveTrainSpecs, &m_currrentPoint, &m_estimatedPose);
		// m_ramsete.update3(&output, &m_follower);

		pout = m_follower.compute();
		// forward:
		/*
		m_moteurL1.SetVoltage(m_CrtzL1.getVoltage(output.m_leftVelocity, output.m_leftAcceleration));
		m_moteurL2.SetVoltage(m_CrtzL2.getVoltage(output.m_leftVelocity, output.m_leftAcceleration));
		m_moteurR1.SetVoltage(m_CrtzR1.getVoltage(output.m_rightVelocity, output.m_rightAcceleration));
		m_moteurR2.SetVoltage(m_CrtzR2.getVoltage(output.m_rightVelocity, output.m_rightAcceleration));
		*/
		/*
		// backward:
		m_moteurR1.SetVoltage(m_CrtzL1.getVoltage(-output.m_leftVelocity, -output.m_leftAcceleration));
		m_moteurR2.SetVoltage(m_CrtzL2.getVoltage(-output.m_leftVelocity, -output.m_leftAcceleration));

		m_moteurL1.SetVoltage(m_CrtzR1.getVoltage(-output.m_rightVelocity,-output.m_rightAcceleration));
		m_moteurL2.SetVoltage(m_CrtzR2.getVoltage(-output.m_rightVelocity,-output.m_rightAcceleration));
		*/
		m_moteurL1.SetVoltage(m_CrtzL1.getVoltage(pout->m_leftVelocity, pout->m_leftAcceleration));
		m_moteurL2.SetVoltage(m_CrtzL2.getVoltage(pout->m_leftVelocity, pout->m_leftAcceleration));
		m_moteurR1.SetVoltage(m_CrtzR1.getVoltage(pout->m_rightVelocity, pout->m_rightAcceleration));
		m_moteurR2.SetVoltage(m_CrtzR2.getVoltage(pout->m_rightVelocity, pout->m_rightAcceleration));

		m_logCsv.write();

		break;

	case NLROBOT::STATE::PATH_END:
		break;
	default:
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		break;
	}
}

void NLROBOT::draw()
{
	// ***************************************************************************************
	// *
	// ROBOT REFERENCE
	// Repr�sentation de la position et de l'orientation de REFERENCE DU ROBOT.
	// Selon les donn�es du NLTRAJECTORY_PACK ( �tat courant )
	NVEC2f32 u;
	u.x = m_follower.getTargetPoint()->m_u.x;
	u.y = m_follower.getTargetPoint()->m_u.y;

	NVec2Normalize(&u);
	NMATRIX3x3 mx;
	mx.XAxis.x = u.x;
	mx.YAxis.x = -u.y;
	mx.ZAxis.x = 0.0f;
	mx.XAxis.y = u.y;
	mx.YAxis.y = u.x;
	mx.ZAxis.y = 0.0f;
	mx.XAxis.z = 0.0f;
	mx.YAxis.z = 0.0f;
	mx.ZAxis.z = 1.0f;
	NVEC3 origin;
	origin.x = m_follower.getTargetPoint()->m_p.x;
	origin.y = m_follower.getTargetPoint()->m_p.y;
	origin.z = 0.0f;
	NCOLOR color = {NCOLOR_PRESET3F_GREEN, 1.0f};
	m_pVirtualRobot->drawRobotShape(&m_TrajectoryPack.m_matrix, &mx, &origin, &color);

	NCOLOR red = {NCOLOR_PRESET3F_RED_AMARANTH, 1.0f};

	origin.x = m_follower.m_estimatedPose.m_position.x;
	origin.y = m_follower.m_estimatedPose.m_position.y;
	origin.z = 0.0f;

	mx.XAxis.x = cos(m_follower.m_estimatedPose.m_angle);
	mx.YAxis.x = -sin(m_follower.m_estimatedPose.m_angle);
	mx.ZAxis.x = 0.0f;
	mx.XAxis.y = sin(m_follower.m_estimatedPose.m_angle);
	mx.YAxis.y = cos(m_follower.m_estimatedPose.m_angle);
	mx.ZAxis.y = 0.0f;
	mx.XAxis.z = 0.0f;
	mx.YAxis.z = 0.0f;
	mx.ZAxis.z = 1.0f;

	m_pVirtualRobot->drawRobotShape(&m_TrajectoryPack.m_matrix, &mx, &origin, &red);

	// *
	// ***************************************************************************************

	// ***************************************************************************************
	// *
	// ROBOT RAMSETE DEBUG
	// Repr�sentation de la position et de l'orientation de REFERENCE DU ROBOT.
	// VUE DEPUIS la position estim�e du ROBOT.
	/*
	NMATRIX mx2, mxr;
	NVEC3f32 p;
	p.x = m_ramsete.dbg_currentposture.x;
	p.y = m_ramsete.dbg_currentposture.y;
	p.z = 0.0f;

	NComposeMatrix(&mx2, &m_ramsete.dbg_mx,&p);
	NMulMatrix(&mxr, &mx2, &m_TrajectoryPack.m_matrix);

	NMATRIX3x3	mx3;
	NVEC3f32	o;
	// extract
	mx3.f00 = mxr.f00;	mx3.f10 = mxr.f10;	mx3.f20 = mxr.f20;
	mx3.f01 = mxr.f01;	mx3.f11 = mxr.f11;	mx3.f21 = mxr.f21;
	mx3.f02 = mxr.f02;	mx3.f12 = mxr.f12;	mx3.f22 = mxr.f22;
	o.x = mxr.f30;
	o.y = mxr.f31;
	o.z = mxr.f32;

	NUT_Draw_3DMatrix3x3(&mx3, &o, 2.0f);


	o.x = m_ramsete.dbg_errp.x;
	o.y = m_ramsete.dbg_errp.y;
	o.z = 0.0f;
	NMulVector3ByMatrix(&o, &mxr, &o);
	NUT_SetDrawConstructionPlane(_PLANE_XY);
	NUT_DRAW_ELLIPSE	ellipse;
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	FLAG_ON(ellipse.Flags, FLAG_NUT_DRAW_ELLIPSE_DASHLINE);
	NSetColorf(&ellipse.Color,NCOLOR_PRESET3F_BLUE_AQUAMARINE,1);
	ellipse.SliceNb = 33;
	ellipse.Extents.x = 1.0f;
	ellipse.Extents.y = 1.0f;
	ellipse.Center = o;
	NUT_Draw_Ellipse(&ellipse);
	*/

	/*
	NVEC3		pos,B;
	NCOLOR		color = { NCOLOR_PRESET3F_GREEN,1.0f };


	// Preparation: texte
	Nchar			txt[64];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.1f;
	NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_ELECTRIC, 1);



	//NUT_Draw_Cross(&m_refPosition, &xtd, &color);


	// A: Position de Reference
	m_refMatrix.XAxis.x =  m_refTangent.y;
	m_refMatrix.XAxis.y = -m_refTangent.x;
	m_refMatrix.XAxis.z = 0.0f;

	m_refMatrix.YAxis.x = m_refTangent.x;
	m_refMatrix.YAxis.y = m_refTangent.y;
	m_refMatrix.YAxis.z = 0.0f;

	m_refMatrix.ZAxis.x = 0.0f;
	m_refMatrix.ZAxis.y = 0.0f;
	m_refMatrix.ZAxis.z = 1.0f;
	NUT_SetDrawConstructionPlaneMatrix3x3(&m_refMatrix);
	// position du centre g�om�trique du robot.
	pos.x = m_refPosition.x;//- m_pSpecifications->m_centerOfMass.x;
	pos.y = m_refPosition.y;//- m_pSpecifications->m_centerOfMass.y;
	pos.z = m_refPosition.z;//- m_pSpecifications->m_centerOfMass.z;
	m_pVirtualRobot->drawRobotShape(&pos,&m_refMatrix, &color);



	// B: Position estim�e par Odom�trie ( avec les encodeurs )
	NSetColorf(&color, NCOLOR_PRESET3F_RED, 1.0f);
	NRotationMatrix3x3_AxisZ(&m_estimatedMatrix, m_estimatedAngle - NF32_PI_2);
	NUT_SetDrawConstructionPlaneMatrix3x3(&m_estimatedMatrix);
	// position du centre g�om�trique du robot.
	pos.x = m_estimatedPosition.x;//- m_pSpecifications->m_centerOfMass.x;
	pos.y = m_estimatedPosition.y;//- m_pSpecifications->m_centerOfMass.y;
	pos.z = m_estimatedPosition.z;//- m_pSpecifications->m_centerOfMass.z;
	// tracage du cadre fantome 1( position et orientation estim�e par odom�trie )
	m_pVirtualRobot->drawRobotShape(&pos, &m_estimatedMatrix, &color);

//	sprintf(txt, "LU: %.4f    RU: %.4f", m_leftErrorVoltage, m_rightErrorVoltage);
//	NUT_Draw_Text(txt, &pos, &drawtxt);

	// ----------------------------------------------------------------------------------
	// Visualisation RAMSETE DATA sur la position estim�e
	NUT_SetDrawConstructionPlane(_PLANE_XY);

	NVEC2 xtd = { 0.2f,0.2f };
	NVEC3 posb;
	posb.x = m_estimatedPosition.x + m_ramseteErrPos.x * m_estimatedMatrix.XAxis.x + m_ramseteErrPos.y * m_estimatedMatrix.YAxis.x;
	posb.y = m_estimatedPosition.y + m_ramseteErrPos.x * m_estimatedMatrix.XAxis.y + m_ramseteErrPos.y * m_estimatedMatrix.YAxis.y;
	posb.z = 0.0f;

	NUT_Draw_Cross(&posb, &xtd, &color);
	NUT_Draw_Mark(&posb, &xtd, &color);
	NUT_Draw_Angle(&pos, (NVEC2*)&m_estimatedMatrix.YAxis, m_ramseteErrAngle, &color);

	posb.x = pos.x + m_refTangent.x * 2.5f;
	posb.y = pos.y + m_refTangent.y * 2.5f;
	posb.z = 0.0f;
	NSetColorf(&color, NCOLOR_PRESET3F_YELLOW, 1.0f);
	NUT_Draw_Segment(&pos, &posb, &color);

	sprintf(txt, "est: %.4f    Tgt: %.4f", m_estimatedAngle, atan2f(m_currrentState.m_tangent.y, m_currrentState.m_tangent.x));
	NUT_Draw_Text(txt, &posb, &drawtxt);
	// ----------------------------------------------------------------------------------


	// C: Position estim�e par Odom�trie ( avec des moteurs "MAGIQUES" d�livrant exactement la vitesse et l'acceleration demand�es )
	NRotationMatrix3x3_AxisZ(&m_trueVelAccMatrix, m_trueVelAccAngle - NF32_PI_2);
	NUT_SetDrawConstructionPlaneMatrix3x3(&m_trueVelAccMatrix);
	// position du centre g�om�trique du robot.
	pos.x = m_trueVelAccPosition.x;//- m_pSpecifications->m_centerOfMass.x;
	pos.y = m_trueVelAccPosition.y;//- m_pSpecifications->m_centerOfMass.y;
	pos.z = m_trueVelAccPosition.z;//- m_pSpecifications->m_centerOfMass.z;
	// tracage du cadre fantome 1( position et orientation estim�e par odom�trie )
	NSetColorf(&color, NCOLOR_PRESET3F_BLUE_AQUAMARINE,1.0f);
	//NUT_Draw_Quad(&pos, &extend, &col);
	m_pVirtualRobot->drawRobotShape(&pos, &m_trueVelAccMatrix, &color);
	*/
}

void NLROBOT::drawDashBoard(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	m_follower.drawDashBoard(p2docs, pickpack);
}
