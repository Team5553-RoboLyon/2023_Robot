#define T_SWITCH 0.5
#define VOLTAGE_REF 12.0                                    // tension de référence
#define ANNULE_FROTTEMENTS 0.013                    // hop plus de frottements
#define MAXSWITCHTIMELOCK 0.5                              // temps max pour le switch de vitesse
#define AXLETRACK 0.5098 // distance entre les roues
#define HALF_TRACKWIDTH (AXLETRACK / 2.0f)
#define T_ACCEL 0.5 
#define T_SPEED  0.5 
#define WF_REF 6380     // vitesse de rotation théorique du moteur

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


    m_MotorGearboxRight1.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxRight2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxRight3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorGearboxLeft1.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxLeft2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxLeft3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorGearboxLeft2.Follow(m_MotorGearboxLeft1);
    m_MotorGearboxLeft3.Follow(m_MotorGearboxLeft1);

    m_MotorGearboxRight2.Follow(m_MotorGearboxRight1);
    m_MotorGearboxRight3.Follow(m_MotorGearboxRight1);
    

    m_EncoderRight.SetDistancePerPulse(1.0/2048.0); // 1 tour correspond à 2048 ticks
    m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);  // 1 tour correspond à 2048 ticks

    m_EncoderLeft.Reset();
    m_EncoderRight.Reset();

    m_rateLimiterFast.Reset(0.0,0.0,0.0);
    m_rateLimiterSlow.Reset(0.0,0.0,0.0);

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
    return (std::abs(w) + WF_REF * ANNULE_FROTTEMENTS) / WF_REF;

}

double Drivetrain::GetSwitchGearVoltageLeft(double w)
{
    return (std::abs(w) + WF_REF * ANNULE_FROTTEMENTS) / WF_REF;
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
    if (speedRobot > 0 and accelerationRobot > 0 and joystick > 0 and deltaJoystick >= 0)
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
    if (speedRobot < 100  and accelerationRobot < 0 and joystick > 111110)
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
    if (speedRobot < 0.0000001)
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

double Drivetrain::SwitchUpRight(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageRight(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV2();
    return U;
}

double Drivetrain::SwitchUpLeft(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageLeft(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV2();
    return U;
}

double Drivetrain::SwitchDownRight(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageRight(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV1();
    return U;
}

double Drivetrain::SwitchDownLeft(double coeff, double target,double w)
{
    double Usn = GetSwitchGearVoltageLeft(w);
    double U=target*coeff +(1-coeff)*Usn;
    ActiveBallShifterV1();
    return U;
}


void Drivetrain::Drive(double joystick_V, double joystick_W)
{
    // double signe=dif_Right>0 ? 1 : -1;


    double encoder_Gear_Right_Distance = m_EncoderRight.GetDistance(); // nombre de tours de l''axe de sortie de boite droite
    double encoder_Gear_Left_Distance = m_EncoderLeft.GetDistance();  // nombre de tours de l'axe de sortie de boite gauche
    double encoder_Motor_Right_Distance = m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / 2048;// RPM axe du moteur 1 de la boite droite
    double encoder_Motor_Left_Distance = m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / 2048;// RPM axe du moteur 1 de la boite de gauche

    m_SpeedEncoderLeft = (encoder_Gear_Left_Distance-m_SpeedEncoderLeftLast)/0.02*60;
    m_SpeedEncoderRight = (encoder_Gear_Right_Distance-m_SpeedEncoderRightLast)/0.02*60;
    m_SpeedMotorLeft = (encoder_Motor_Left_Distance-m_SpeedMotorLeftLast)/0.02*60;
    m_SpeedMotorRight = (encoder_Motor_Right_Distance-m_SpeedMotorRightLast)/0.02*60;

    m_SpeedEncoderLeftLast = encoder_Gear_Left_Distance;
    m_SpeedEncoderRightLast = encoder_Gear_Right_Distance;
    m_SpeedMotorLeftLast = encoder_Motor_Left_Distance;
    m_SpeedMotorRightLast = encoder_Motor_Right_Distance;

    m_SpeedRobotRight = (m_SpeedEncoderRight*T_SPEED +m_SpeedMotorRight*(1-T_SPEED )) / 2;
    m_SpeedRobotLeft = (m_SpeedEncoderLeft*T_SPEED +m_SpeedMotorLeft*(1-T_SPEED )) / 2;

    m_Robot_W = (m_RobotRight_W + m_RobotLeft_W) / 2;


    m_AccelerationMotorRight = m_SpeedMotorRight - m_SpeedMotorRightLast;
    m_AccelerationMotorLeft = m_SpeedMotorLeft - m_SpeedMotorLeftLast;
    m_AccelerationEncoderRight = m_SpeedEncoderRight - m_SpeedEncoderRightLast;
    m_AccelerationEncoderLeft = m_SpeedEncoderLeft - m_SpeedEncoderLeftLast;

    m_RobotAccelerationLeft = (m_EncoderAccelerationLeft*T_ACCEL+m_MotorAccelerationLeft*(1-T_ACCEL)) /2;
    m_RobotAccelerationRight = (m_EncoderAccelerationRight*T_ACCEL+m_MotorAccelerationRight*(1-T_ACCEL)) /2;

    m_RobotAcceleration = (m_RobotAccelerationLeft + m_RobotAccelerationRight) / 2;

    m_Robot_W=(m_SpeedRobotRight-m_SpeedRobotLeft)/AXLETRACK;

    m_Joystick_V_Pure = joystick_V;
    m_Joystick_W_Pure = joystick_W;
    m_joyAcceleration = (m_joystickRight - m_joystickRightLast) / 0.02;
    m_joystickRightLast = m_joystickRight;

    m_SwitchTimeLock = m_SwitchTimeLock - 0.02;

    m_rateLimiterFast.Update(m_joystickRight);
    m_rateLimiterSlow.Update(m_rateLimiterFast.m_current);




    switch (m_State)
    {
        case State::lowGear:
        {
            m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiterSlow.m_current, m_joystickLeft, 0));
            m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiterSlow.m_current, m_joystickLeft, 1));
            if (General(m_SwitchTimeLock,m_W))
            {
                if (Up(m_SpeedRobot,m_AccelerationRobot,m_joystickRight,m_joyAcceleration))
                {
                    m_SwitchGearVoltageRight = SwitchUpRight(T_SWITCH, m_joystickRight,m_SpeedRobotRight);
                    m_SwitchGearVoltageLeft = SwitchUpLeft(T_SWITCH, m_joystickRight,m_SpeedRobotLeft);
                    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageLeft);
                    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageRight);
                    SetSwitchTimeLock(0);
                    m_State = State::highGear;
                }
            }
        }
        break;

        case State::highGear:
        {
            m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiterSlow.m_current, m_joystickLeft, 0));
            m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiterSlow.m_current, m_joystickLeft, 1));
            if (CoastDown(m_SpeedRobot))
            {
                m_SwitchGearVoltageRight = SwitchDownRight(T_SWITCH, m_joystickRight,m_SpeedRobotRight);
                m_SwitchGearVoltageLeft = SwitchDownLeft(T_SWITCH, m_joystickRight,m_SpeedRobotLeft);
                m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageLeft);
                m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageRight);
                SetSwitchTimeLock(0);
                m_State = State::lowGear;
            }
            
            if (General(m_SwitchTimeLock,m_W))
            {
                if (KickDown(m_SpeedRobot,m_AccelerationRobot,m_joystickRight))
                {
                    m_SwitchGearVoltageRight = SwitchDownRight(T_SWITCH, m_joystickRight,m_SpeedRobotRight);
                    m_SwitchGearVoltageLeft = SwitchDownLeft(T_SWITCH, m_joystickRight,m_SpeedRobotLeft);
                    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageLeft);
                    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, m_SwitchGearVoltageRight);
                    SetSwitchTimeLock(0);
                    m_State = State::lowGear;
                }
            }
        }
    }
}
