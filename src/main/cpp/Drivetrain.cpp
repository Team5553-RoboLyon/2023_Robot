#define REDUC_V1 13.3
#define REDUC_V2 8.8
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
    (((2.0 * VMAX) / AXLETRACK) / \
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

    m_rateLimiter_V_Fast.Reset(0.0,0.0,0.0);
    m_rateLimiter_V_Slow.Reset(0.0,0.0,0.0);

    m_rateLimiter_W_Fast.Reset(0.0,0.0,0.0);
    m_rateLimiter_W_Slow.Reset(0.0,0.0,0.0);

    m_logCSV.setItem(0,"joystick_V",5,&m_Joystick_V_Pure);
    m_logCSV.setItem(1,"joystick_W",5,&m_Joystick_W_Pure);
    m_logCSV.setItem(2,"SpeedRobot",5,&m_Robot_W);
    m_logCSV.setItem(3,"état",5,&m_Vitesse);
    m_logCSV.setItem(4,"rate_limiter_Fast_V",5,&m_rateLimiter_V_Fast.m_current);
    m_logCSV.setItem(5,"rate_limiter_Slow_V",5,&m_rateLimiter_V_Slow.m_current);
    m_logCSV.setItem(6,"rate_limiter_Fast_W",5,&m_rateLimiter_W_Fast.m_current);
    m_logCSV.setItem(7,"rate_limiter_Slow_W",5,&m_rateLimiter_W_Slow.m_current);

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

double Drivetrain::GetSwitchGearVoltage(double w)
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
     
    if (switchTimeLock <= 0 and std::abs(w) <300)//à définir
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
    if (speedRobot > 125 and accelerationRobot > 0 and joystick > 0.4 and deltaJoystick >= 0)
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
    if (speedRobot < 50  and accelerationRobot < 0 and joystick > 0.2)
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
    if (speedRobot < 10)
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


void Drivetrain::SwitchUp(double w)
{
    m_SwitchSpeed = GetSwitchGearVoltage(w);
    m_Recul = ((m_rateLimiter_V_Slow.m_current - m_SwitchSpeed)/m_rateLimiter_V_Slow.m_speed )*m_rateLimiter_V_Fast.m_speed;
    m_rateLimiter_V_Fast.m_current=m_SwitchSpeed-m_Recul;
    ActiveBallShifterV2();
}

void Drivetrain::SwitchDown(double w)
{
    m_SwitchSpeed = GetSwitchGearVoltage(w);
    m_Recul = ((m_rateLimiter_V_Slow.m_current - m_SwitchSpeed)/m_rateLimiter_V_Slow.m_speed )*m_rateLimiter_V_Fast.m_speed;
    m_rateLimiter_V_Fast.m_current=m_SwitchSpeed-m_Recul;
    ActiveBallShifterV1();
}




void Drivetrain::Drive(double joystick_V, double joystick_W)
{
    // double signe=dif_Right>0 ? 1 : -1;
    double coef = m_State == State::lowGear ? REDUC_V1 : REDUC_V2;

    double encoder_Gear_Right_Distance = m_EncoderRight.GetDistance(); // nombre de tours de l''axe de sortie de boite droite
    double encoder_Gear_Left_Distance = m_EncoderLeft.GetDistance();  // nombre de tours de l'axe de sortie de boite gauche
    double encoder_Motor_Right_Distance = m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef);// RPM axe du moteur 1 de la boite droite / par le coef de réduction
    double encoder_Motor_Left_Distance = m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef);// RPM axe du moteur 1 de la boite de gauche / par le coef de réduction

    m_EncoderLeft_V = (encoder_Gear_Left_Distance-m_EncoderLeftLast_V)/0.02*60;
    m_EncoderRight_V = (encoder_Gear_Right_Distance-m_EncoderRightLast_V)/0.02*60;
    m_MotorLeft_V = (encoder_Motor_Left_Distance-m_MotorLeftLast_V)/0.02*60;
    m_MotorRight_V = (encoder_Motor_Right_Distance-m_MotorRightLast_V)/0.02*60;

    m_EncoderLeftLast_V = encoder_Gear_Left_Distance;
    m_EncoderRightLast_V = encoder_Gear_Right_Distance;
    m_MotorLeftLast_V = encoder_Motor_Left_Distance;
    m_MotorRightLast_V = encoder_Motor_Right_Distance;

    m_RobotRight_W = (m_EncoderRight_V*T_SPEED +m_MotorRight_V*(1-T_SPEED )) / 2;
    m_RobotLeft_W = (m_EncoderLeft_V*T_SPEED +m_MotorLeft_V*(1-T_SPEED )) / 2;

    m_Robot_W = (m_RobotRight_W + m_RobotLeft_W) / 2;


    m_MotorAccelerationRight = m_MotorRight_V - m_MotorRightLast_V;
    m_MotorAccelerationLeft = m_MotorLeft_V - m_MotorLeftLast_V;
    m_EncoderAccelerationRight = m_EncoderRight_V - m_EncoderRightLast_V;
    m_EncoderAccelerationLeft = m_EncoderLeft_V - m_EncoderLeftLast_V;

    m_RobotAccelerationLeft = (m_EncoderAccelerationLeft*T_ACCEL+m_MotorAccelerationLeft*(1-T_ACCEL)) /2;
    m_RobotAccelerationRight = (m_EncoderAccelerationRight*T_ACCEL+m_MotorAccelerationRight*(1-T_ACCEL)) /2;

    m_RobotAcceleration = (m_RobotAccelerationLeft + m_RobotAccelerationRight) / 2;

    m_Robot_Angular_Rotate=(m_RobotRight_W-m_RobotLeft_W)/AXLETRACK;

    m_Joystick_V_Pure = joystick_V;
    m_Joystick_W_Pure = joystick_W;
    m_Joystick_V_Acceleration = (m_Joystick_V_Pure - m_Joystick_V_Last) / 0.02;
    m_Joystick_V_Last = m_Joystick_V_Pure;

    m_SwitchTimeLock = m_SwitchTimeLock - 0.02;

    m_rateLimiter_V_Fast.Update(m_Joystick_V_Pure);
    m_rateLimiter_V_Slow.Update(m_rateLimiter_V_Fast.m_current);

    m_rateLimiter_W_Fast.Update(m_Joystick_W_Pure);
    m_rateLimiter_W_Slow.Update(m_rateLimiter_W_Fast.m_current);

    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Fast.m_current", m_rateLimiter_V_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Slow.m_current", m_rateLimiter_V_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Fast.m_current", m_rateLimiter_W_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Slow.m_current", m_rateLimiter_W_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_Joystick_V_Pure", m_Joystick_V_Pure);
    frc::SmartDashboard::PutNumber("m_Joystick_W_Pure", m_Joystick_W_Pure);



    switch (m_State)
    {
        case State::lowGear:
        {
            m_Vitesse=1;
            m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (General(m_SwitchTimeLock,m_Robot_Angular_Rotate))
            {
                if (Up(m_Robot_W,m_RobotAcceleration,m_Joystick_V_Pure,m_Joystick_V_Acceleration))
                {
                    SwitchUp(m_Robot_W);
                    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_State = State::highGear;
                }
            }
        }
        break;

        case State::highGear:
        {
            m_Vitesse=2;
            m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (CoastDown(m_Robot_W))
            {
                SwitchDown(m_Robot_W);
                m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                SetSwitchTimeLock(0.5);
                m_State = State::lowGear;
            }
            
            if (General(m_SwitchTimeLock,m_Robot_Angular_Rotate))
            {
                if (KickDown(m_Robot_W,m_RobotAcceleration,m_Joystick_V_Pure))
                {
                    SwitchDown(m_Robot_W);
                    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_State = State::lowGear;
                }
            }
        }
    }
}
