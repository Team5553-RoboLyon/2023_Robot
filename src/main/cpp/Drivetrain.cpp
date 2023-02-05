#define REDUC_V1 13.3
#define REDUC_V2 8.8
#define T_SWITCH 0.5
#define VOLTAGE_REF 12.0                                    // tension de référence
#define RESIST_TORQUE_NM 0.013                    // hop plus de frottements
#define MAXSWITCHTIMELOCK 0.5                              // temps max pour le switch de vitesse
#define AXLETRACK 0.5098 // distance entre les roues
#define HALF_TRACKWIDTH (AXLETRACK / 2.0f)
#define T_ACCEL 0.5 
#define T_SPEED  0.5 
#define WF_MOTOR_RPM 6380     // vitesse de rotation théorique du moteur
#define SIGMA 0.5          // sigma pour le rate limiter

#define AMAX 5.1 // Acceleration Max  au PIF .. à définir aux encodeurs
#define VMAX 3.4 // vitesse Max  théorique (3,395472 sur JVN-DT) .. à vérifier aux encodeurs
#define WMAX                       \
    (((2.0 * VMAX) / AXLETRACK) / \
     1.7) // vitesse angulaire Max theorique	.. à modifier avec Garice

#define NABS(a) (((a) < 0) ? -(a) : (a))     // VALEUR ABSOLUE
#define NMAX(a, b) (((a) > (b)) ? (a) : (b)) // Max
#define NMIN(a, b) (((a) < (b)) ? (a) : (b)) // Min

#include "Drivetrain.h"
#include "lib/MoveData.h"
#include "lib/utils.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

Drivetrain::Drivetrain()
{
    m_MotorGearboxLeft1.ConfigFactoryDefault(); // reset des paramètres du moteur
    m_MotorGearboxLeft2.ConfigFactoryDefault();
    m_MotorGearboxLeft3.ConfigFactoryDefault();

    m_MotorGearboxRight1.ConfigFactoryDefault();
    m_MotorGearboxRight2.ConfigFactoryDefault();
    m_MotorGearboxRight3.ConfigFactoryDefault();

    m_MotorGearboxLeft1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0)); // limite de courant   
    m_MotorGearboxLeft2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxLeft3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    
    m_MotorGearboxRight1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxRight2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorGearboxRight3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));

    m_MotorGearboxLeft1.SetInverted(true); // inversion des moteurs
    m_MotorGearboxLeft2.SetInverted(true);
    m_MotorGearboxLeft3.SetInverted(true);

    m_MotorGearboxRight1.SetInverted(false);
    m_MotorGearboxRight2.SetInverted(false);
    m_MotorGearboxRight3.SetInverted(false);

    m_MotorGearboxLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake); // init brake mode moteur
    m_MotorGearboxLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorGearboxRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorGearboxRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorGearboxRight1.ConfigVoltageCompSaturation(VOLTAGE_REF); // init tension de référence
    m_MotorGearboxRight2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxRight3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorGearboxLeft1.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxLeft2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorGearboxLeft3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorGearboxLeft2.Follow(m_MotorGearboxLeft1); // init follower moteurs
    m_MotorGearboxLeft3.Follow(m_MotorGearboxLeft1);

    m_MotorGearboxRight2.Follow(m_MotorGearboxRight1);
    m_MotorGearboxRight3.Follow(m_MotorGearboxRight1);
    

    m_EncoderRight.SetDistancePerPulse(1.0/2048.0); // 1 tour correspond à 2048 ticks
    m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);  // 1 tour correspond à 2048 ticks

    m_EncoderLeft.Reset(); // reset des encodeurs
    m_EncoderRight.Reset(); 

    m_rateLimiter_V_Fast.Reset(0.0,0.0,2.0); // reset des rate limiters
    m_rateLimiter_V_Slow.Reset(0.0,0.0,0.05);

    m_rateLimiter_W_Fast.Reset(0.0,0.0,2.0);
    m_rateLimiter_W_Slow.Reset(0.0,0.0,0.05);

    m_logCSV.setItem(0,"joystick_V",5,&m_Joystick_V_Pure);
    m_logCSV.setItem(1,"joystick_W",5,&m_Joystick_W_Pure);
    m_logCSV.setItem(2,"SpeedRobot",5,&m_Gearboxes_W_average_RPM);
    m_logCSV.setItem(3,"état",5,&m_Etat);
    m_logCSV.setItem(4,"rate_limiter_Fast_V",5,&m_rateLimiter_V_Fast.m_current);
    m_logCSV.setItem(5,"rate_limiter_Slow_V",5,&m_rateLimiter_V_Slow.m_current);
    m_logCSV.setItem(6,"rate_limiter_Fast_W",5,&m_rateLimiter_W_Fast.m_current);
    m_logCSV.setItem(7,"rate_limiter_Slow_W",5,&m_rateLimiter_W_Slow.m_current);

    ActiveBallShifterV1(); // init ball shifter
    m_State =State::lowGear;
}

void Drivetrain::Set(double v_motor) // set des moteurs
{
    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
}

void Drivetrain::ActiveBallShifterV1() // active ball shifter V1
{
    std::cout<<"ActiveBallShifterV1"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Drivetrain::ActiveBallShifterV2() // active ball shifter V2
{
    std::cout<<"ActiveBallShifterV2"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kForward);
}

void Drivetrain::InvertBallShifter() // inverse ball shifter
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

double Drivetrain::GetSwitchGearVoltage(double w_motor_rpm) // calcule la tension de référence en fonction de la vitesse du robot
{
    return (std::abs(w_motor_rpm) + WF_MOTOR_RPM * RESIST_TORQUE_NM) / WF_MOTOR_RPM;
}

void Drivetrain::EnableBrakeMode(bool change) // active/ désactive le breakMode 
{
    if (change)
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

double Drivetrain::Calcul_De_Notre_Brave_JM(double forward, double turn, bool wheelSide) // calcule la vitesse des roues
{
    // double m_forward = utils::Deadband(forward);
    // double m_turn = utils::Deadband(turn);

    // double v = m_forward * VMAX;
    // double w = m_turn * WMAX;

    // double left_wheel = v + (w * HALF_TRACKWIDTH);
    // double right_wheel = v - (w * HALF_TRACKWIDTH);

    // double k;
    // k = 1.0 / (NMAX(VMAX, NMAX(NABS(left_wheel), NABS(right_wheel))));
    // left_wheel *= k;
    // right_wheel *= k;

    // if (wheelSide == 0)
    //     return right_wheel;
    // else
    //     return left_wheel;

    double m_forward = utils::Deadband(forward);
    double m_turn = utils::Deadband(turn);

    double left_wheel = m_forward + m_turn * SIGMA;
    double right_wheel = m_forward - m_turn * SIGMA;

    double k;
    k = 1.0 / (NMAX(1, NMAX(NABS(left_wheel), NABS(right_wheel))));
    left_wheel *= k;
    right_wheel *= k;

    if (wheelSide == 0)
        return right_wheel;
    else
        return left_wheel;
}

bool Drivetrain::General(double switchTimeLock,double w) // mode général, détermine si on peut passer une vitesse
{
    return false;
    if (switchTimeLock <= 0 and std::abs(w) <300)//à définir
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Drivetrain::Up(double speedRobot, double accelerationRobot, double joystick,double deltaJoystick) // mode up, détermine si on peut passer en V2
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

bool Drivetrain::KickDown(double speedRobot, double accelerationRobot, double joystick) // mode kickdown, détermine si on peut passer en V1
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

bool Drivetrain::CoastDown(double speedRobot) // mode coastdown, détermine si on peut passer en V1
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

void Drivetrain::SetSwitchTimeLock(double switchTimeLock) // défini le temps de lock
{
    m_SwitchTimeLock = switchTimeLock;
}


void Drivetrain::SwitchUp(double w_motor_rpm) // passage de la vitesse en V2
{
    m_SwitchSpeed = GetSwitchGearVoltage(w_motor_rpm);
    m_Recul = ((m_rateLimiter_V_Slow.m_current - m_SwitchSpeed)/m_rateLimiter_V_Slow.m_speed )*m_rateLimiter_V_Fast.m_speed;
    m_rateLimiter_V_Fast.m_current=m_SwitchSpeed-m_Recul;
    ActiveBallShifterV2();
}

void Drivetrain::SwitchDown(double w_motor_rpm) // passage de la vitesse en V1
{
    m_SwitchSpeed = GetSwitchGearVoltage(w_motor_rpm);
    m_Recul = ((m_rateLimiter_V_Slow.m_current - m_SwitchSpeed)/m_rateLimiter_V_Slow.m_speed )*m_rateLimiter_V_Fast.m_speed;
    m_rateLimiter_V_Fast.m_current=m_SwitchSpeed-m_Recul;
    ActiveBallShifterV1();
}




void Drivetrain::Drive(double joystick_V, double joystick_W) // 
{
    // double signe=dif_Right>0 ? 1 : -1;
    double coef = m_State == State::lowGear ? REDUC_V1 : REDUC_V2;
    //distance sortie de boite en nombre de tour
    // double encoder_Gearbox_Right_Distance = m_EncoderRight.GetDistance(); // nombre de tours de l''axe de sortie de boite droite
    // double encoder_Gearbox_Left_Distance = m_EncoderLeft.GetDistance();  // nombre de tours de l'axe de sortie de boite gauche
    // vitesse angulaire en sortie de boite basé sur les encodeurs motors
    // double encoder_Motor_Right_Distance = m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef);// RPM axe du moteur 1 de la boite droite / par le coef de réduction
    // double encoder_Motor_Left_Distance = m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef);// RPM axe du moteur 1 de la boite de gauche / par le coef de réduction

    m_Gearbox_encoder_Right.setPos(m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef)); 
    m_Gearbox_encoder_Left.setPos(m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*coef));

    m_Motor_encoder_Left.setPos(m_EncoderLeft.GetDistance());
    m_Motor_encoder_Right.setPos(m_EncoderRight.GetDistance());

    // m_encoder_Gearbox_Left_W = (encoder_Gearbox_Left_Distance-m_Encoder_Gearbox_Left_Previous_Distance)/0.02*60; // vitesse de l'axe de sortie de boite gauche en RPM
    // m_encoder_Gearbox_Right_W = (encoder_Gearbox_Right_Distance-m_Encoder_Gearbox_Right_Previous_Distance)/0.02*60; // vitesse de l'axe de sortie de boite droite en RPM
    // m_encoder_Motor_Left_W = (encoder_Motor_Left_Distance-m_Encoder_Motor_Left_Previous_Distance)/0.02*60; // vitesse de l'axe du moteur 1 de la boite gauche en RPM
    // m_encoder_Motor_Right_W = (encoder_Motor_Right_Distance-m_Encoder_Motor_Right_Previous_Distance)/0.02*60; // vitesse de l'axe du moteur 1 de la boite droite en RPM

    // m_Encoder_Gearbox_Left_Previous_Distance = encoder_Gearbox_Left_Distance; // sauvegarde de la vitesse de l'axe de sortie de boite gauche
    // m_Encoder_Gearbox_Right_Previous_Distance = encoder_Gearbox_Right_Distance; // sauvegarde de la vitesse de l'axe de sortie de boite droite
    // m_Encoder_Motor_Left_Previous_Distance = encoder_Motor_Left_Distance;    // sauvegarde de la vitesse de l'axe du moteur 1 de la boite gauche
    // m_Encoder_Motor_Right_Previous_Distance = encoder_Motor_Right_Distance;  // sauvegarde de la vitesse de l'axe du moteur 1 de la boite droite

    m_Gearbox_Right_W_RPM = (m_Gearbox_encoder_Right.m_velocity*T_SPEED +m_Motor_encoder_Right.m_velocity*(1-T_SPEED )) ;  // vitesse du robot partie droite en RPM
    m_Gearbox_Left_W_RPM = (m_Gearbox_encoder_Left.m_velocity*T_SPEED +m_Motor_encoder_Left.m_velocity*(1-T_SPEED )) ; // vitesse du robot partie gauche en RPM

    m_Gearboxes_W_average_RPM = (m_Gearbox_Right_W_RPM + m_Gearbox_Left_W_RPM) / 2;   // vitesse angulaire moyenne des boites du robot en RPM


    // m_MotorAccelerationRight = encoder_Motor_Right_Distance - m_Encoder_Motor_Right_Previous_Distance; // accélération moteur partie droite
    // m_MotorAccelerationLeft = encoder_Motor_Left_Distance - m_Encoder_Motor_Left_Previous_Distance; // accélération moteur partie gauche
    // m_EncoderAccelerationRight = encoder_Gearbox_Right_Distance - m_Encoder_Gearbox_Right_Previous_Distance; // accélération axe de sortie de boite partie droite
    // m_EncoderAccelerationLeft = encoder_Gearbox_Left_Distance - m_Encoder_Gearbox_Left_Previous_Distance; // accélération axe de sortie de boite partie gauche

    m_RobotAccelerationLeft = (m_Gearbox_encoder_Left.m_acceleration*T_ACCEL+m_Motor_encoder_Left.m_acceleration*(1-T_ACCEL)) ; // accélération partie gauche
    m_RobotAccelerationRight = (m_Gearbox_encoder_Right.m_acceleration*T_ACCEL+m_Motor_encoder_Right.m_acceleration*(1-T_ACCEL)); // accélération partie droite

    m_Gearboxes_Acceleration = (m_RobotAccelerationLeft + m_RobotAccelerationRight) / 2; // accélération du robot

    m_Robot_W=(m_Gearbox_Right_W_RPM-m_Gearbox_Left_W_RPM)/AXLETRACK; // vitesse angulaire du robot en RPM

    m_Joystick_V_Pure = joystick_V; 
    m_Joystick_W_Pure = joystick_W;
    m_Joystick_V_Acceleration = (m_Joystick_V_Pure - m_Joystick_V_Last) / 0.02;//pourquoi par 0.02 ??
    m_Joystick_V_Last = m_Joystick_V_Pure;

    m_SwitchTimeLock = m_SwitchTimeLock - 0.02; // décrémentation du temps de verrouillage de la vitesse

    m_rateLimiter_V_Fast.Update(m_Joystick_V_Pure); 
    m_rateLimiter_V_Slow.Update(m_rateLimiter_V_Fast.m_current);

    m_rateLimiter_W_Fast.Update(m_Joystick_W_Pure);
    m_rateLimiter_W_Slow.Update(m_rateLimiter_W_Fast.m_current);

    m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
    m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));

    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Fast.m_current", m_rateLimiter_V_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Slow.m_current", m_rateLimiter_V_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Fast.m_current", m_rateLimiter_W_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Slow.m_current", m_rateLimiter_W_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_Joystick_V_Pure", m_Joystick_V_Pure);
    frc::SmartDashboard::PutNumber("m_Joystick_W_Pure", m_Joystick_W_Pure);

    frc::SmartDashboard::PutNumber("W", m_Robot_W);



    switch (m_State)
    {
        case State::lowGear:
        {
            m_Etat=1;
            // m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (General(m_SwitchTimeLock,m_Robot_W))
            {
                if (Up(m_Gearboxes_W_average_RPM,m_Gearboxes_Acceleration,m_Joystick_V_Pure,m_Joystick_V_Acceleration))
                {
                    SwitchUp(m_Gearboxes_W_average_RPM*REDUC_V1);
                    // m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    // m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_State = State::highGear;
                }
            }
        }
        break;

        case State::highGear:
        {
            m_Etat=2;
            // m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (CoastDown(m_Gearboxes_W_average_RPM))
            {
                SwitchDown(m_Gearboxes_W_average_RPM*REDUC_V2);
                // m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                // m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                SetSwitchTimeLock(0.5);
                m_State = State::lowGear;
            }
            
            if (General(m_SwitchTimeLock,m_Robot_W))
            {
                if (KickDown(m_Gearboxes_W_average_RPM,m_Gearboxes_Acceleration,m_Joystick_V_Pure))
                {
                    SwitchDown(m_Gearboxes_W_average_RPM*REDUC_V2);
                    // m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    // m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_State = State::lowGear;
                }
            }
        }
    }
}
