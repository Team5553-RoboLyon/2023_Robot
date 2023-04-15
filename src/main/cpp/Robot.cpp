// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

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
    if (NABS(NRADtoDEG(m_ahrs.GetPitch())) > 5 and m_count > 250)
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
    m_output = m_pidGyro.Calculate(m_ahrs.GetAngle());
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

  // m_StateAutobalance1 = StateAutobalance1::forward;
  // m_StateAutobalance2 = StateAutobalance2::open;
  // m_StateAutoCubeHaut = StateAutoCubeHaut::Init;
  m_StateAutobalanceTout = StateAutoBalanceTout::forward;

  // m_StateAutoCube1 = StateAutoCube1::open;
  // m_StateAutobalanceTout=StateAutoBalanceTout
  m_robotContainer.m_drivetrain.IsAuto = true;
  m_robotContainer.m_elevator.IsAuto = true;

  m_robotContainer.m_drivetrain.Reset();
  m_robotContainer.m_intake.Reset();
  m_robotContainer.m_gripper.Reset();
  m_robotContainer.m_arm.Reset();
  m_robotContainer.m_conveyor.Reset();
  m_robotContainer.m_elevator.Reset();
  m_robotContainer.m_turret.Reset();
  m_count = 0;
  // std::cout << "on passe en auito" << std::endl;
}
void Robot::AutonomousPeriodic()
{
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
  m_robotContainer.m_elevator.IsAuto = false;
}
void Robot::TeleopPeriodic()
{
  frc::SmartDashboard::PutNumber("angleGyro", NRADtoDEG(m_ahrs.GetPitch()));
  // std::cout << m_ahrs.GetPitch() << std::endl;
  frc::SmartDashboard::PutNumber("elevator", m_robotContainer.m_elevator.GetEncoder());
  frc::SmartDashboard::PutNumber("turret", m_robotContainer.m_turret.GetEncoder());
  frc::SmartDashboard::PutNumber("arm", m_robotContainer.m_arm.GetEncoder());
  frc::SmartDashboard::PutNumber("armOutput", m_robotContainer.m_arm.m_armPid.m_output);
  frc::SmartDashboard::PutNumber("armerror", m_robotContainer.m_arm.m_armPid.m_error);

  if (m_robotContainer.m_joystickLeft.GetRawButtonPressed(2))
  {
    m_robotContainer.m_drivetrain.InvertBallShifter();
  }
}

void Robot::DisabledInit()
{
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
