#define REDUC_V1 13.8                                      // valeur de réduction de la vitesse 1
#define REDUC_V2 8.4                                       // valeur de réduction de la vitesse 2
#define MAX_RPM_MOTEUR 6380                                // a définir
#define COEFF_JVN 0.80                                     // tiré de JVN/a définir
#define MAX_RPM_V1 (MAX_RPM_MOTEUR / REDUC_V1) * COEFF_JVN // rendement de 80% sortie de boite avec reduc 13.3
#define MAX_RPM_V2 (MAX_RPM_MOTEUR / REDUC_V2) * COEFF_JVN // rendement de 80% sortie de boite avec reduc de 8.8
#define MULTIPLICATEUR_PM 50 * 60                          // pour passer des pulses à une minute (SetDistancePerPulse)
#define voltageRef 12.0                                    // tension de référence
#define hop_plus_de_frottements 0.013                    // hop plus de frottements
#define MAXSWITCHTIMELOCK 0.5                              // temps max pour le switch de vitesse
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


    m_MotorGearboxRight1.ConfigVoltageCompSaturation(voltageRef);
    m_MotorGearboxRight2.ConfigVoltageCompSaturation(voltageRef);
    m_MotorGearboxRight3.ConfigVoltageCompSaturation(voltageRef);

    m_MotorGearboxLeft1.ConfigVoltageCompSaturation(voltageRef);
    m_MotorGearboxLeft2.ConfigVoltageCompSaturation(voltageRef);
    m_MotorGearboxLeft3.ConfigVoltageCompSaturation(voltageRef);

    m_MotorGearboxLeft2.Follow(m_MotorGearboxLeft1);
    m_MotorGearboxLeft3.Follow(m_MotorGearboxLeft1);

    m_MotorGearboxRight2.Follow(m_MotorGearboxRight1);
    m_MotorGearboxRight3.Follow(m_MotorGearboxRight1);
    

    m_EncoderRight.SetDistancePerPulse(1.0/2048.0);
    m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);    

    m_EncoderLeft.Reset();
    m_EncoderRight.Reset();

    ActiveBallShifterV1();
}

void Drivetrain::Set(double speed)
{
    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, speed);
}

void Drivetrain::ActiveBallShifterV1()
{
    std::cout<<"ActiveBallShifterV1"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Drivetrain::ActiveBallShifterV2()
{
    std::cout<<"ActiveBallShifterV2"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kForward);
}

void Drivetrain::InvertBallShifter()
{
    if (m_BallShifterSolenoidLeft.Get() == frc::DoubleSolenoid::Value::kForward)
    {
        ActiveBallShifterV1();
    }
    else
    {
        ActiveBallShifterV2();
    }
}

double Drivetrain::GetSwitchGearVoltageRight(double w)
{
    double switchGearVoltage = (std::abs(w) + wfRef * hop_plus_de_frottements) * (voltageRef / wfRef);//TODO changer le 0.8
    double percentVoltage = switchGearVoltage / voltageRef ;
    return percentVoltage;

}

double Drivetrain::GetSwitchGearVoltageLeft(double w)
{
    double switchGearVoltage = (std::abs(w) + wfRef * hop_plus_de_frottements) * (voltageRef / wfRef);//TODO changer le 0.8
    double percentVoltage = switchGearVoltage / voltageRef;
    return percentVoltage;
}

void Drivetrain::EnableBrakeMode(bool Change)
{
    if (Change)
    {
        m_MotorGearboxLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorGearboxLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorGearboxLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

        m_MotorGearboxRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorGearboxRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorGearboxRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    }
    else
    {
        m_MotorGearboxLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorGearboxLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorGearboxLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);

        m_MotorGearboxRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorGearboxRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorGearboxRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    }
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

bool Drivetrain::General(double switchTimeLock,double w)
{
     
    if (switchTimeLock > MAXSWITCHTIMELOCK and std::abs(w) >30)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Drivetrain::Up(double speedRobot, double accelerationRobot, double joystick,double deltaJoystick)
{
    if (speedRobot > 0 and accelerationRobot > 0 and joystick > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Drivetrain::KickDown(double speedRobot, double accelerationRobot, double joystick)
{
    if (speedRobot < 0 and accelerationRobot < 0 and joystick < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Drivetrain::CoastDown(double speedRobot)
{
    if (speedRobot < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Drivetrain::SetSwitchTimeLock(double switchTimeLock)
{
    m_SwitchTimeLock = switchTimeLock;
}

double Drivetrain::SwitchUp(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageRight(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV2();
    return U;
}

double Drivetrain::SwitchDown(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageLeft(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV1();
    return U;
}


void Drivetrain::Drive(double joystickRight, double joystickLeft)
{
    double encodeLeft = m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity();
    double encodeRight_tick_per_100ms = m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity();
    double encoderRight_RPM_Reel = encodeRight_tick_per_100ms * 600 / 2048;
    double encoderRight_RPM = std::abs(encoderRight_RPM_Reel);

    double encoder_Right=m_EncoderRight.GetDistance();
    double encoder_Left=m_EncoderLeft.GetDistance();
    double dif_Right=((encoder_Right-0)/0.02*60)+((encoder_Left-0)/0.02*60)/2;
    double dif_Left=(encoder_Left-0)/0.02*60;
    double dif_Rightvrai=(encoder_Right-0)/0.02*60;

    double encoder_RightLast=encoder_Right;
    double encoder_LeftLast=encoder_Left;

    double signe=dif_Right>0 ? 1 : -1;

    switch (m_State)
    {
        case State::lowGear:
        {
            if (General(m_SwitchTimeLock,m_W))
            {
                if (Up(m_SpeedRobot,m_AccelerationRobot,joystickRight,m_joyAcceleration))
                {
                    m_SwitchGearVoltageRight = SwitchUp(0.5, joystickRight,m_SpeedRobot);
                    SetSwitchTimeLock(0);
                    m_State = State::highGear;
                }
            }
        }
        break;

        case State::highGear:
        {
            if (CoastDown(m_SpeedRobot))
            {
                m_SwitchGearVoltageRight = SwitchDown(0.5, joystickRight,m_SpeedRobot);
                SetSwitchTimeLock(0);
                m_State = State::lowGear;
            }
            
            if (General(m_SwitchTimeLock,m_W))
            {
                if (KickDown(m_SpeedRobot,m_AccelerationRobot,joystickRight))
                {
                    m_SwitchGearVoltageRight = SwitchDown(0.5, joystickRight,m_SpeedRobot);
                    SetSwitchTimeLock(0);
                    m_State = State::lowGear;
                }
            }
        }
    }
}
