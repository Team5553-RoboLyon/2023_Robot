// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.
#include "lib/NL/MotionControl/DriveTrain/Characterization/NLCharacterizationTable.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryActionMessagesEnum.h"

#include "Robot.h"
// cc
// cc
void Robot::AutoBalanceTout()
{
  switch (m_StateAutobalance1)
  {
  case StateAutobalance1::forward:
  {
    m_output = NLERP(0.6, 0.4, ((double)m_count / 50.0));
    m_robotContainer.m_drivetrain.DriveAuto(m_output, 0.0);
    // if (m_count > 300p or (m_ahrs.GetPitch() >= -1.0 and m_count > 50)) // 75
    if (m_count > 70)
    {
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::stop;
    }
  }
  break;
  case StateAutobalance1::stop:
  {
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
  }
  break;
  default:
    break;
  }
}
void Robot::AutoBalance1()
{
  switch (m_StateAutobalance1)
  {
  case StateAutobalance1::forward:
  {
    m_robotContainer.m_drivetrain.DriveAuto(0.4, 0.0);
    // if (m_count > 300p or (m_ahrs.GetPitch() >= -1.0 and m_count > 50)) // 75
    if (m_count > 75)
    {
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::finish;
    }
  }
  break;
  case StateAutobalance1::stop:
  {
    if (NABS(NRADtoDEG(m_gyro.GetPitch())) > 5 and m_count > 250)
      m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
    else
    {
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::recule;
    }
  }
  break;
  case StateAutobalance1::recule:
  {
    m_robotContainer.m_drivetrain.DriveAuto(-0.4, 0.0);
    if (m_count > 25)
    {
      m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::forwardV2;
    }
  }
  break;

  case StateAutobalance1::forwardV2:
  {
    m_robotContainer.m_drivetrain.DriveAuto(0.7, 0.0);
    if (m_count > 35)
    {
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::finish;
    }
  }
  break;

  case StateAutobalance1::finish:
  {
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
  }
  break;
  default:
    break;
  }
}

void Robot::AutoBalance2()
{
  switch (m_StateAutobalance2)
  {
  case StateAutobalance2::open:
    m_robotContainer.m_intake.Open();
    m_count = 0;
    m_StateAutobalance2 = StateAutobalance2::speed;
    break;
  case StateAutobalance2::speed:
    m_robotContainer.m_intake.SetSpeed(-0.7);
    m_robotContainer.m_conveyor.SetSpeed(-0.7);
    if (m_count > 100)
    {
      m_StateAutobalance2 = StateAutobalance2::close;
      m_count = 0;
    }
    break;

  case StateAutobalance2::close:
    m_robotContainer.m_intake.Close();
    m_robotContainer.m_intake.SetSpeed(0.0);
    m_robotContainer.m_conveyor.SetSpeed(0.0);
    m_count = 0;
    m_StateAutobalance2 = StateAutobalance2::tour;
    break;
  case StateAutobalance2::tour:
    m_pidGyro.SetSetpoint(180.0);
    m_output = m_pidGyro.Calculate(m_gyro.GetAngle());
    m_robotContainer.m_drivetrain.DriveAuto(m_output, -m_output);
    if (NABS(m_pidGyro.m_error) < 1.0)
    {
      m_count = 0;
      m_StateAutobalance2 = StateAutobalance2::forward;
    }
    break;
  case StateAutobalance2::forward:
    m_robotContainer.m_drivetrain.DriveAuto(-0.2, 0.0);
    if (m_count > 350)
    {
      m_count = 0;
      m_StateAutobalance2 = StateAutobalance2::finish;
    }
    break;
  case StateAutobalance2::finish:
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
    break;
  default:
    break;
  }
}

void Robot::AutoCube2()
{
  switch (m_StateAutoCube2)
  {
  case StateAutoCube2::open:
    m_robotContainer.m_intake.Open();
    m_StateAutoCube2 = StateAutoCube2::speed;
    m_count = 0;
    break;
  case StateAutoCube2::speed:
    m_robotContainer.m_intake.SetSpeed(-0.7);
    m_robotContainer.m_conveyor.SetSpeed(-0.7);
    if (m_count > 100)
    {
      m_StateAutoCube2 = StateAutoCube2::close;
      m_count = 0;
    }
    break;
  case StateAutoCube2::close:
    m_robotContainer.m_intake.Close();
    m_robotContainer.m_intake.SetSpeed(0.0);
    m_robotContainer.m_conveyor.SetSpeed(0.0);
    m_count = 0;
    m_StateAutoCube2 = StateAutoCube2::forward;
    break;
  case StateAutoCube2::forward:
    m_robotContainer.m_drivetrain.DriveAuto(-0.2, 0.0);
    if (m_count > 400)
    {
      m_count = 0;
      m_StateAutoCube2 = StateAutoCube2::open2;
    }
    break;
  case StateAutoCube2::open2:
    m_robotContainer.m_intake.Open();
    m_robotContainer.m_intake.SetSpeed(0.7);
    m_robotContainer.m_conveyor.SetSpeed(0.7);
    if (m_count > 100)
    {
      m_StateAutoCube2 = StateAutoCube2::close2;
      m_count = 0;
    }
    break;
  case StateAutoCube2::close2:
    m_robotContainer.m_intake.Close();
    m_robotContainer.m_intake.SetSpeed(0.0);
    m_robotContainer.m_conveyor.SetSpeed(0.0);
    m_count = 0;
    m_StateAutoCube2 = StateAutoCube2::backward;
    break;
  case StateAutoCube2::backward:
    m_robotContainer.m_drivetrain.DriveAuto(0.2, 0.0);
    if (m_count > 50)
    {
      m_count = 0;
      m_StateAutoCube2 = StateAutoCube2::finish;
    }
    break;
  case StateAutoCube2::finish:
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
    break;
  }
}

void Robot::AutoCube1()
{
  switch (m_StateAutoCube1)
  {
  case StateAutoCube1::open:
    m_robotContainer.m_intake.Open();
    m_StateAutoCube1 = StateAutoCube1::speed;
    m_count = 0;
    break;

  case StateAutoCube1::speed:
    m_robotContainer.m_intake.SetSpeed(-0.7);
    m_robotContainer.m_conveyor.SetSpeed(-0.7);
    if (m_count > 100)
    {
      m_StateAutoCube1 = StateAutoCube1::close;
      m_count = 0;
    }
    break;
  case StateAutoCube1::close:
    m_robotContainer.m_intake.Close();
    m_robotContainer.m_intake.SetSpeed(0.0);
    m_robotContainer.m_conveyor.SetSpeed(0.0);
    m_count = 0;
    m_StateAutoCube1 = StateAutoCube1::forward;
    break;
  case StateAutoCube1::forward:
    m_robotContainer.m_drivetrain.DriveAuto(-0.2, 0.0);
    if (m_count > 350)
    {
      m_count = 0;
      m_StateAutoCube1 = StateAutoCube1::finish;
    }
    break;
  case StateAutoCube1::finish:
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
    break;
  default:
    break;
  }
}

void Robot::AutoCubeHaut()
{

  switch (m_StateAutoCubeHaut)
  {
  case StateAutoCubeHaut::Init:
    m_robotContainer.m_elevator.SetSetpoint(0.80);
    if (m_count > 50)
    {
      m_count = 0;
      m_robotContainer.m_arm.SetSetpoint(NDEGtoRAD(-35.0)); // valeur théorique à vérifier
      m_StateAutoCubeHaut = StateAutoCubeHaut::High;
    }
    break;
  case StateAutoCubeHaut::High:
    if (m_count > 80) // temps à réduire
    {
      m_robotContainer.m_elevator.SetSetpoint(0.30);
      m_count = 0;
      m_StateAutoCubeHaut = StateAutoCubeHaut::Lowered;
    }
    break;
  case StateAutoCubeHaut::Lowered:
    if (m_count > 45) // temps à réduire
    {
      m_count = 0;
      m_StateAutoCubeHaut = StateAutoCubeHaut::Taken;
    }
    break;
  case StateAutoCubeHaut::Taken:
    if (m_count > 50) // temps à réduire
    {
      m_robotContainer.m_elevator.SetSetpoint(0.98);
      m_count = 0;
      m_StateAutoCubeHaut = StateAutoCubeHaut::GoDown;
    }
    break;
  case StateAutoCubeHaut::GoDown:
    if (m_count > 50) // temps à réduire
    {
      m_robotContainer.m_arm.SetSetpoint(NDEGtoRAD(105.0));
      m_count = 0;
      m_StateAutoCubeHaut = StateAutoCubeHaut::Forward;
    }
    break;
  case StateAutoCubeHaut::Forward:
  {
    m_robotContainer.m_drivetrain.DriveAuto(0.2, 0.0);
    if (m_count > 50)
    {
      m_count = 0;
      m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
      m_StateAutoCubeHaut = StateAutoCubeHaut::Open;
    }
  }
  break;

  case StateAutoCubeHaut::Open:
  {
    if (m_count > 50)
    {
      m_count = 0;
      m_StateAutoCubeHaut = StateAutoCubeHaut::Recule;
    }
  }
  break;
  case StateAutoCubeHaut::Recule:
  {
    m_robotContainer.m_drivetrain.DriveAuto(-0.4, 0.0);
    if (m_count > 100)
    {
      m_count = 0;
      m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
      m_StateAutoCubeHaut = StateAutoCubeHaut::Finish;
    }
  }
  break;
  case StateAutoCubeHaut::Finish:
  {
    m_robotContainer.m_arm.SetSetpoint(NDEGtoRAD(90.0));
    m_robotContainer.m_elevator.SetSetpoint(0.0);
  }

  break;
  default:
    break;
  };
}

void Robot::RobotInit()
{
  // m_ahrs.Reset();
  // m_ahrs.Calibrate();
}

void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
}
// on fait

void Robot::AutonomousInit()
{
  m_gyro.Reset();
  m_gyro.Calibrate();
  m_csv.open("/home/lvuser/", true); // ouverture du fichier de log

  m_csv.setItem(0, "encoderLeft", 5, &m_encoderLeft);
  m_csv.setItem(1, "encoderRight", 5, &m_encoderRight);
  m_csv.setItem(2, "gyro", 5, &m_gyroAngle);
  m_csv.setItem(3, "voltageLeft", 5, &m_VoltageLeft);
  m_csv.setItem(4, "voltageRight", 5, &m_VoltageRight);

  // m_StateAutobalance1 = StateAutobalance1::forward;
  // m_StateAutobalance2 = StateAutobalance2::open;
  // m_StateAutoCubeHaut = StateAutoCubeHaut::Init;
  m_StateAutobalanceTout = StateAutoBalanceTout::forward;

  // m_StateAutoCube1 = StateAutoCube1::open;
  // m_StateAutobalanceTout=StateAutoBalanceTout
  m_robotContainer.m_drivetrain.IsAuto = true;
  m_robotContainer.m_elevator.IsAuto = true;
  m_robotContainer.m_drivetrain.IsAuto = true;

  m_robotContainer.m_drivetrain.Reset();
  m_robotContainer.m_elevator.Reset();
  m_robotContainer.m_intake.Reset();
  m_robotContainer.m_arm.Reset();
  m_robotContainer.m_conveyor.Reset();
  m_robotContainer.m_gripper.Reset();

  // m_robotContainer.GetAutonomousCommand()->Schedule();

  // ######## NLMOTOR_CHARACTERIZATION ########
  // NLCHARACTERIZATION_TABLE characterization_table(4);
  // characterization_table.importTxt("/home/lvuser/auto/characterization_MultiVarLinearRegression.txt");

  m_CrtzL.m_forwardKv = 2.6412869101570307;
  m_CrtzL.m_backwardKv = 2.6248036368134255;        // = m_kv[1]
  m_CrtzL.m_forwardKa = 0.41029013114711876;        // = m_ka[0]
  m_CrtzL.m_backwardKa = 0.3713930975997702;        // = m_ka[1]
  m_CrtzL.m_forwardIntercept = 0.39281383839084655; // = m_intercept[0]
  m_CrtzL.m_backwardIntercept = -0.47477756289709444;

  m_CrtzR.m_forwardKv = 2.643306157356795;
  m_CrtzR.m_backwardKv = 2.632469106333122;        // = m_kv[1]
  m_CrtzR.m_forwardKa = 0.36456683427794917;       // = m_ka[0]
  m_CrtzR.m_backwardKa = 0.3257845553734638;       // = m_ka[1]
  m_CrtzR.m_forwardIntercept = 0.4218986448873328; // = m_intercept[0]
  m_CrtzR.m_backwardIntercept = -0.49001485320659466;

  // characterization_table.get(&m_CrtzL, "L1", NFALSE);
  // characterization_table.get(&m_CrtzR, "R1", NFALSE);

  m_TrajectoryPack.load("/home/lvuser/auto/loop.trk");
  m_follower.initialize(&m_TrajectoryPack);
  m_state = Robot::STATE::PATH_FOLLOWING;
}
void Robot::AutonomousPeriodic()
{

  // std::cout << "encoderL" << m_robotContainer.m_drivetrain.m_EncoderLeft.GetDistance() << std::endl;
  // std::cout << "encoderR" << m_robotContainer.m_drivetrain.m_EncoderRight.GetDistance() << std::endl;
  std::cout << "gyro" << m_gyro.GetAngle() << std::endl;

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

    m_follower.estimate(m_robotContainer.m_drivetrain.m_EncoderLeft.GetDistance(), m_robotContainer.m_drivetrain.m_EncoderRight.GetDistance(), NDEGtoRAD(m_gyro.GetAngle()));
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
      case ACTIVATE_INTAKE_WHEELS_TAKE:
        m_robotContainer.m_intake.SetSpeed(0.6);
        break;
      case ACTIVATE_INTAKE_WHEELS_DROPOFF:
        m_robotContainer.m_intake.SetSpeed(-0.6);
        break;
      case DEACTIVATE_INTAKE_WHEELS:
        m_robotContainer.m_intake.SetSpeed(0.0);
        break;
      case ACTIVATE_CONVEYOR_TAKE:
        m_robotContainer.m_conveyor.SetSpeed(0.6);
        break;
      case ACTIVATE_CONVEYOR_DROPOFF:
        m_robotContainer.m_conveyor.SetSpeed(-0.6);
        break;
      case DEACTIVATE_CONVEYOR:
        m_robotContainer.m_conveyor.SetSpeed(0.0);
        break;
      case TURRET_0:
        m_robotContainer.m_turret.SetSetpoint(0.0);
        break;
      case TURRET_L90:
        m_robotContainer.m_turret.SetSetpoint(90.0);
        break;
      case TURRET_R90:
        m_robotContainer.m_turret.SetSetpoint(-90.0);
        break;
      case TURRET_HOME:
        m_robotContainer.m_turret.SetSetpoint(0.0);
        break;
        /*case CONE_HIGH_BACK_DROPOFF:
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

  m_encoderLeft = m_robotContainer.m_drivetrain.m_EncoderLeft.GetDistance();
  m_encoderRight = m_robotContainer.m_drivetrain.m_EncoderRight.GetDistance();
  m_gyroAngle = m_gyro.GetAngle();
  m_VoltageLeft = m_CrtzR.getVoltage(pout->m_rightVelocity, pout->m_rightAcceleration);
  m_VoltageRight = m_CrtzL.getVoltage(pout->m_leftVelocity, pout->m_leftAcceleration);

  m_csv.write();
  m_robotContainer.m_elevator.Reset();
  m_robotContainer.m_turret.Reset();
  m_count = 0;
  // std::cout << "on passe en auito" << std::endl;

  m_count += 1;
  // AutoBalance1();
  // AutoBalance2();

  // AutoCube1();
  // AutoCubeHaut();
  AutoBalanceTout();
}

void Robot::TeleopInit()
{

  m_robotContainer.m_drivetrain.IsAuto = false;
  m_robotContainer.m_drivetrain.IsAuto = false;
  m_robotContainer.m_elevator.IsAuto = false;
}
void Robot::TeleopPeriodic()
{
  frc::SmartDashboard::PutNumber("angleGyro", NRADtoDEG(m_gyro.GetPitch()));
  std::cout << m_gyro.GetPitch() << std::endl;
  frc::SmartDashboard::PutNumber("elevator", m_robotContainer.m_elevator.GetEncoder());
  frc::SmartDashboard::PutNumber("turret", m_robotContainer.m_turret.GetEncoder());
  frc::SmartDashboard::PutNumber("arm", m_robotContainer.m_arm.GetEncoder());
  frc::SmartDashboard::PutNumber("armOutput", m_robotContainer.m_arm.m_armPid.m_output);
  frc::SmartDashboard::PutNumber("armerror", m_robotContainer.m_arm.m_armPid.m_error);

  std::cout << "navX" << m_gyro.GetAngle() << std::endl;
  frc::SmartDashboard::PutNumber("navX", m_gyro.GetAngle());
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
  if (m_robotContainer.m_joystickLeft.GetRawButtonPressed(1))
  {
    m_robotContainer.m_drivetrain.InvertBallShifter();
  }
}

void Robot::DisabledInit()
{
  m_csv.close(); // fermeture du fichier de log
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
