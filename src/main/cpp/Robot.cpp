// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryActionMessagesEnum.h"

#include "Robot.h"
// cc
// cc
void Robot::RobotInit()
{
  m_ahrs.Reset();
  m_ahrs.Calibrate();
}
void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}
// on fait

void Robot::AutonomousInit()
{
  //
  m_robotContainer.m_drivetrain.IsAuto = true;
  // m_robotContainer.GetAutonomousCommand()->Schedule();

  // ######## NLMOTOR_CHARACTERIZATION ########
  // NLCHARACTERIZATION_TABLE characterization_table(4);
  // characterization_table.importTxt("/home/lvuser/auto/characterization_MultiVarLinearRegression.txt");

  m_CrtzL.m_forwardKv = 2.7768955535857174;
  m_CrtzL.m_backwardKv = 2.812021660337195;         // = m_kv[1]
  m_CrtzL.m_forwardKa = 0.43399905394521276;        // = m_ka[0]
  m_CrtzL.m_backwardKa = 0.3692375083754934;        // = m_ka[1]
  m_CrtzL.m_forwardIntercept = 0.17348065329638107; // = m_intercept[0]
  m_CrtzL.m_backwardIntercept = -0.1536673881482158;

  m_CrtzR.m_forwardKv = 2.779743232558036;
  m_CrtzR.m_backwardKv = 2.775883010820141;         // = m_kv[1]
  m_CrtzR.m_forwardKa = 0.4248692255119551;         // = m_ka[0]
  m_CrtzR.m_backwardKa = 0.39095781941614804;       // = m_ka[1]
  m_CrtzR.m_forwardIntercept = 0.18380021592124685; // = m_intercept[0]
  m_CrtzR.m_backwardIntercept = -0.1990197738348809;

  // characterization_table.get(&m_CrtzL, "L1", NFALSE);
  // characterization_table.get(&m_CrtzR, "R1", NFALSE);

  m_TrajectoryPack.load("/home/lvuser/auto/auto1.trk");
  m_follower.initialize(&m_TrajectoryPack);
  m_state = Robot::STATE::PATH_END;
}
void Robot::AutonomousPeriodic()
{
  std::cout << "encoderL" << m_robotContainer.m_drivetrain.m_EncoderLeft.GetDistance() << std::endl;
  std::cout << "encoderR" << m_robotContainer.m_drivetrain.m_EncoderRight.GetDistance() << std::endl;
  std::cout << "gyro" << m_ahrs.GetAngle() << std::endl;

  NLRAMSETEOUTPUT output;
  NLFOLLOWER_TANK_OUTPUT *pout = nullptr;

  NLTRJ_POSTED_MESSAGE message; // Posted Message

  switch (m_state)
  {
  case Robot::STATE::PATH_ERROR:
    break;

  case Robot::STATE::PATH_FOLLOWING:
    // *****************************************************    'THE' METHOD(e)
    // A) Feed back:
    // avec les encodeurs on estime la position du robot:
    //			l = distance parcourue par la roue gauche depuis le dernier reset encodeur.
    //			r = distance parcourue par la roue droite depuis le dernier reset encodeur.
    //
    //			dl et dr = distances parcourues par les roues gauche et droite depuis le dernier call.
    //			(note dl/dt = vitesse roue gauche et dr/dt = vitesse roue droite  )
    //

    m_follower.estimate(m_robotContainer.m_drivetrain.m_EncoderLeft.GetDistance(), m_robotContainer.m_drivetrain.m_EncoderRight.GetDistance(), NDEGtoRAD(m_ahrs.GetAngle()));
    m_follower.updateTarget(&m_TrajectoryPack, 0.02f);
    pout = m_follower.compute();
    m_robotContainer.m_drivetrain.SetVoltage(m_CrtzR.getVoltage(pout->m_rightVelocity, pout->m_rightAcceleration), m_CrtzL.getVoltage(pout->m_leftVelocity, pout->m_leftAcceleration));
    std::cout << m_CrtzR.m_forwardKv << "m_CrtzR.m_forwardKv " << std::endl;

    while (m_follower.getMessage(&message))
    {
      switch (message.m_id)
      {
      case OPEN_INTAKE:
        m_robotContainer.m_intake.Open();
        break;
      case CLOSE_INTAKE:
        m_robotContainer.m_intake.Close();
        break;
        /*case ACTIVATE_INTAKE_WHEELS:
          m_allMechanisms.Command("activate intake wheels");
          break;
        case DEACTIVATE_INTAKE_WHEELS:
          m_allMechanisms.Command("deactivate intake wheels");
          break;
        case ACTIVATE_CONVEYOR:
          m_allMechanisms.Command("activate conveyor");
          break;
        case DEACTIVATE_CONVEYOR:
          m_allMechanisms.Command("deactivate intake wheels");
          break;

        case ACTIVATE_AIM:
          break;
        case DEACTIVATE_AIM:
          break;
        case ACTIVATE_SHOOTER:
          break;
        case DEACTIVATE_SHOOTER:
          break;*/

      default:
        break;
      }
    }

    break;

  case Robot::STATE::PATH_END:
    break;
  default:
    NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
    break;
  }
}

void Robot::TeleopInit()
{
  m_robotContainer.m_drivetrain.IsAuto = false;

  // m_robotContainer.m_drivetrain.m_logCSV.open("/home/lvuser/", true); // ouverture du fichier de log
}
void Robot::TeleopPeriodic()
{
  // m_robotContainer.m_arm.m_speed = m_robotContainer.m_joystickRight.GetY();

  // if (m_robotContainer.m_joystickLeft.GetRawButtonPressed(1))
  // {
  //   m_robotContainer.m_gripper.ChangePosition();
  // }
  // frc::SmartDashboard::PutNumber("x", m_robotContainer.m_copiloter.m_x);
  // frc::SmartDashboard::PutNumber("h", m_robotContainer.m_copiloter.m_h);
  // frc::SmartDashboard::PutNumber("tetha", m_robotContainer.m_copiloter.m_theta);
  // frc::SmartDashboard::PutNumber("encoderElevator", m_robotContainer.m_copiloter.m_elevator.GetEncoder());
  // frc::SmartDashboard::PutNumber("encoderArm", m_robotContainer.m_copiloter.m_arm.GetEncoder());
  frc::SmartDashboard::PutNumber("encoderArm", m_robotContainer.m_arm.GetEncoder());
  frc::SmartDashboard::PutNumber("outputArm", m_robotContainer.m_arm.m_armPid.m_output);

  // frc::SmartDashboard::PutNumber("outputelevateur", m_robotContainer.m_copiloter.m_elevator.m_elevatorPid.m_output);
  // frc::SmartDashboard::PutNumber("setpoint", m_robotContainer.m_copiloter.m_elevator.m_elevatorPid.m_setpoint);
  // m_robotContainer.m_copiloter.m_elevator.SetGains(frc::SmartDashboard::GetNumber("Pelevator", 0),
  //                                                  frc::SmartDashboard::GetNumber("Ielevator", 0),
  //                                                  frc::SmartDashboard::GetNumber("Delevator", 0));

  // m_robotContainer.m_copiloter.m_arm.SetGains(frc::SmartDashboard::GetNumber("Parm", 0),
  //                                             frc::SmartDashboard::GetNumber("Iarm", 0),
  //                                             frc::SmartDashboard::GetNumber("Darm", 0));

  // m_robotContainer.m_turret.SetGains(frc::SmartDashboard::GetNumber("Pturret", 0),
  //                                    frc::SmartDashboard::GetNumber("Iturret", 0),
  //                                    frc::SmartDashboard::GetNumber("Dturret", 0));

  // m_robotContainer.m_copiloter.m_elevator.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointelevator", 0));
  // m_robotContainer.m_copiloter.m_arm.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointarm", 0));
  // m_robotContainer.m_turret.SetSetpoint(frc::SmartDashboard::GetNumber("Setpointturret", 0));

  // m_robotContainer.m_copiloter.m_elevator.Set(frc::SmartDashboard::GetNumber("voltage", 0.0));
}

void Robot::DisabledInit()
{
  // m_robotContainer.m_drivetrain.m_logCSV.close(); // fermeture du fichier de log
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::SimulationInit() {}
void Robot::SimulationPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
