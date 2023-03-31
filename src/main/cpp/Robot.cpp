// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
// cc
// cc

void Robot::AutoBalance1()
{
  switch (m_StateAutobalance1)
  {
  case StateAutobalance1::forward:
    m_robotContainer.m_drivetrain.DriveAuto(0.4, 0.0);
    if (m_count > 300 and m_ahrs.GetPitch()) // 75
    {
      m_count = 0;
      m_StateAutobalance1 = StateAutobalance1::finish;
    }
    break;
  case StateAutobalance1::finish:
    m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
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
    m_output = m_pidGyro.Calculate(m_ahrs.GetAngle());
    m_robotContainer.m_drivetrain.DriveAuto(m_output, -m_output);
    if (m_count > 100 or NABS(m_pidGyro.m_error) < 1.0)
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

void Robot::RobotInit()
{
  m_ahrs.Reset();
  m_ahrs.Calibrate();
}

void Robot::RobotPeriodic()
{
  frc2::CommandScheduler::GetInstance().Run();
  frc::SmartDashboard::PutNumber("angleGyro", m_ahrs.GetAngle());
}
// on fait

void Robot::AutonomousInit()
{
  m_StateAutobalance1 = StateAutobalance1::forward;
  // m_StateAutoCube1 = StateAutoCube1::open;
  m_robotContainer.m_drivetrain.IsAuto = true;
  m_robotContainer.m_drivetrain.Reset();
  m_robotContainer.m_intake.Reset();
  m_robotContainer.m_gripper.Reset();
  m_robotContainer.m_arm.Reset();
  m_robotContainer.m_conveyor.Reset();
  m_robotContainer.m_elevator.Reset();
  m_robotContainer.m_turret.Reset();
  // m_robotContainer.GetAutonomousCommand()->Schedule();
  m_count = 0;
  // std::cout << "on passe en auito" << std::endl;
}
void Robot::AutonomousPeriodic()
{
  m_count += 1;
  // if (m_count < 100)
  // {
  //   m_robotContainer.m_intake.Open();
  //   m_robotContainer.m_intake.SetSpeed(-0.7);
  //   m_robotContainer.m_conveyor.SetSpeed(-0.7);
  // }
  // else
  // {
  //   m_robotContainer.m_intake.Close();
  //   m_robotContainer.m_intake.SetSpeed(0.0);
  //   m_robotContainer.m_conveyor.SetSpeed(0.0);
  // }

  // if (m_count > 100 and m_count < 500)
  // {
  //   m_robotContainer.m_drivetrain.DriveAuto(-0.2, 0.0);
  // }
  // else
  // {
  //   m_robotContainer.m_drivetrain.DriveAuto(0.0, 0.0);
  // }

  AutoBalance1();
  // AutoCube1();
}

void Robot::TeleopInit()
{
  m_robotContainer.m_drivetrain.IsAuto = false;
  // InAutonomous = false;

  // m_robotContainer.m_drivetrain.m_logCSV.open("/home/lvuser/", true); // ouverture du fichier de log
}
void Robot::TeleopPeriodic()
{

  // std::cout << "navX" << m_ahrs.GetAngle() << std::endl;
  frc::SmartDashboard::PutNumber("navX", m_ahrs.GetAngle());
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
