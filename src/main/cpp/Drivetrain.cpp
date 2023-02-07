#define MULTIPLICATEUR_PM 50 * 60                          // pour passer des pulses à une minute (SetDistancePerPulse)

#define TRACKWIDTH 0.61f
#define HALF_TRACKWIDTH (TRACKWIDTH / 2.0f)

#define AMAX 5.1 // Acceleration Max  au PIF .. à définir aux encodeurs
#define VMAX 3.4 // vitesse Max  théorique (3,395472 sur JVN-DT) .. à vérifier aux encodeurs
#define WMAX                       \
    (((2.0 * VMAX) / TRACKWIDTH) / \
     1.7) // vitesse angulaire Max theorique	.. à modifier avec Garice

#define NABS(a) (((a) < 0) ? -(a) : (a))     // VALEUR ABSOLUE
#define NMAX(a, b) (((a) > (b)) ? (a) : (b)) // Max
#define NMIN(a, b) (((a) < (b)) ? (a) : (b)) // Min

#include "Drivetrain.h"
#include "lib/utils.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

Drivetrain::Drivetrain()
{
    m_MotorGearboxLeft1.ConfigFactoryDefault();
    m_MotorGearboxLeft2.ConfigFactoryDefault();
    m_MotorGearboxLeft3.ConfigFactoryDefault();

    m_MotorGearboxRight1.ConfigFactoryDefault();
    m_MotorGearboxRight2.ConfigFactoryDefault();
    m_MotorGearboxRight3.ConfigFactoryDefault();

    // m_MotorGearboxLeft1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration 40);

    m_MotorGearboxLeft1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));      
    m_MotorGearboxLeft2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxLeft3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    
    m_MotorGearboxRight1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxRight2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxRight3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));

    m_MotorGearboxLeft1.SetInverted(true);
    m_MotorGearboxLeft2.SetInverted(true);
    m_MotorGearboxLeft3.SetInverted(true);

    m_MotorGearboxRight1.SetInverted(false);
    m_MotorGearboxRight2.SetInverted(false);
    m_MotorGearboxRight3.SetInverted(false);

    m_MotorGearboxLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorGearboxRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorGearboxLeft1.ConfigOpenloopRamp(0.5);
    m_MotorGearboxLeft2.ConfigOpenloopRamp(0.5);
    m_MotorGearboxLeft3.ConfigOpenloopRamp(0.5);

    m_MotorGearboxRight1.ConfigOpenloopRamp(0.5);
    m_MotorGearboxRight2.ConfigOpenloopRamp(0.5);
    m_MotorGearboxRight3.ConfigOpenloopRamp(0.5);

    m_MotorGearboxLeft1.EnableVoltageCompensation(true);
    m_MotorGearboxLeft2.EnableVoltageCompensation(true);
    m_MotorGearboxLeft3.EnableVoltageCompensation(true);

    m_MotorGearboxRight1.EnableVoltageCompensation(true);
    m_MotorGearboxRight2.EnableVoltageCompensation(true);
    m_MotorGearboxRight3.EnableVoltageCompensation(true);

    m_MotorGearboxRight1.ConfigVoltageCompSaturation(12.0);
    m_MotorGearboxRight2.ConfigVoltageCompSaturation(12.0);
    m_MotorGearboxRight3.ConfigVoltageCompSaturation(12.0);

    m_MotorGearboxLeft1.ConfigVoltageCompSaturation(12.0);
    m_MotorGearboxLeft2.ConfigVoltageCompSaturation(12.0);
    m_MotorGearboxLeft3.ConfigVoltageCompSaturation(12.0);

    m_MotorGearboxLeft2.Follow(m_MotorGearboxLeft1);
    m_MotorGearboxLeft3.Follow(m_MotorGearboxLeft1);

    m_MotorGearboxRight2.Follow(m_MotorGearboxRight1);
    m_MotorGearboxRight3.Follow(m_MotorGearboxRight1);
    

    m_EncoderRight.SetDistancePerPulse(1.0/2048.0);
    m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);    

    m_EncoderLeft.Reset();
    m_EncoderRight.Reset();
    


}

void Drivetrain::Set(double speed)
{
    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
}

double Drivetrain::Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide)
{
    double m_forward = utils::Deadband(forward);
    double m_turn = utils::Deadband(turn);

    double v = m_forward * VMAX;
    double w = m_turn * WMAX;

    double left_wheel = v + (w * HALF_TRACKWIDTH);
    double right_wheel = v - (w * HALF_TRACKWIDTH);

    double k;
    k = 1.0 / (NMAX(VMAX, NMAX(NABS(left_wheel), NABS(right_wheel))));
    left_wheel *= k;
    right_wheel *= k;

    if (wheelSide == 0)
        return right_wheel;
    else
        return left_wheel;
}
void Drivetrain::SetBallShifterV1()
{
    m_BallShifter.Set(frc::DoubleSolenoid::Value::kForward);
}

void Drivetrain::Drive(double joystickRight, double joystickLeft, double buttonpressed)
{
    m_EncoderRightCurrent = m_EncoderRight.GetDistance();
    m_EncoderLeftCurrent  = m_EncoderLeft.GetDistance();
    double rpmright = (m_EncoderRightCurrent - m_EncoderRightLast) * 50 * 60;
    double rpmleft  = (m_EncoderLeftCurrent - m_EncoderLeftLast) * 50 * 60;
    
    m_EncoderRightLast = m_EncoderRightCurrent;
    m_EncoderLeftLast  = m_EncoderLeftCurrent;

    double rpmrobot = (rpmright + rpmleft) / 2;

    double voltage = frc::SmartDashboard::GetNumber("voltage moteurs", 0.0 );
    frc::SmartDashboard::PutNumber("RPM Right", rpmright);
    frc::SmartDashboard::PutNumber("RPM Left", rpmleft);
    frc::SmartDashboard::PutNumber("RPM Robot", rpmrobot);
    frc::SmartDashboard::PutNumber("voltage moteurs", voltage);
    if (buttonpressed)
    {
        Drivetrain::Set(voltage/12.0);
    }
    else 
    {
        Drivetrain::Set(0.0);
    }
    }