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

Drivetrain::Drivetrain() : m_GearboxRightAveragedRpt(AVERAGE_SAMPLES_NUMBER), m_GearboxLeftAveragedRpt(AVERAGE_SAMPLES_NUMBER), m_SuperMotorLeftAveragedRpm(AVERAGE_SAMPLES_NUMBER), m_SuperMotorRightAveragedRpm(AVERAGE_SAMPLES_NUMBER)
{
    m_MotorLeft1.ConfigFactoryDefault(); // reset des paramètres du moteur
    m_MotorLeft2.ConfigFactoryDefault();
    m_MotorLeft3.ConfigFactoryDefault();

    m_MotorRight1.ConfigFactoryDefault();
    m_MotorRight2.ConfigFactoryDefault();
    m_MotorRight3.ConfigFactoryDefault();

    m_MotorLeft1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0)); // limite de courant   
    m_MotorLeft2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorLeft3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    
    m_MotorRight1.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorRight2.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));
    m_MotorRight3.ConfigSupplyCurrentLimit(ctre::phoenix::motorcontrol::SupplyCurrentLimitConfiguration(true, 40, 40, 0));

    m_MotorLeft1.SetInverted(true); // inversion des moteurs
    m_MotorLeft2.SetInverted(true);
    m_MotorLeft3.SetInverted(true);

    m_MotorRight1.SetInverted(false);
    m_MotorRight2.SetInverted(false);
    m_MotorRight3.SetInverted(false);

    m_MotorLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake); // init brake mode moteur
    m_MotorLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_MotorRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

    m_MotorRight1.ConfigVoltageCompSaturation(VOLTAGE_REF); // init tension de référence
    m_MotorRight2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorRight3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorLeft1.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorLeft2.ConfigVoltageCompSaturation(VOLTAGE_REF);
    m_MotorLeft3.ConfigVoltageCompSaturation(VOLTAGE_REF);

    m_MotorLeft2.Follow(m_MotorLeft1); // init follower moteurs
    m_MotorLeft3.Follow(m_MotorLeft1);

    m_MotorRight2.Follow(m_MotorRight1);
    m_MotorRight3.Follow(m_MotorRight1);
    

    m_EncoderRight.SetDistancePerPulse(1.0/2048.0); // 1 tour correspond à 2048 ticks
    m_EncoderLeft.SetDistancePerPulse(1.0/2048.0);  // 1 tour correspond à 2048 ticks

    m_EncoderLeft.Reset(); // reset des encodeurs
    m_EncoderRight.Reset(); 

    m_rateLimiter_V_Fast.Reset(0.0,0.0,2.0); // reset des rate limiters
    m_rateLimiter_V_Slow.Reset(0.0,0.0,0.05);

    m_rateLimiter_W_Fast.Reset(0.0,0.0,2.0);
    m_rateLimiter_W_Slow.Reset(0.0,0.0,0.05);

    // m_logCSV.setItem(0,"joystick_V",5,&m_Joystick_V_Pure);
    // m_logCSV.setItem(1,"joystick_W",5,&m_Joystick_W_Pure);
    // m_logCSV.setItem(2,"SpeedRobot",5,&m_Gearboxes_W_average_RPM);
    // m_logCSV.setItem(3,"état",5,&m_Etat);
    // m_logCSV.setItem(4,"rate_limiter_Fast_V",5,&m_rateLimiter_V_Fast.m_current);
    // m_logCSV.setItem(5,"rate_limiter_Slow_V",5,&m_rateLimiter_V_Slow.m_current);
    // m_logCSV.setItem(6,"rate_limiter_Fast_W",5,&m_rateLimiter_W_Fast.m_current);
    // m_logCSV.setItem(7,"rate_limiter_Slow_W",5,&m_rateLimiter_W_Slow.m_current);

    // Règle le ball shifter, le State et la Reduction en V1 lors de l'initialisation du robot
    ActiveBallShifterV1(); 
    m_State =State::lowGear;
    m_CurrentGearboxReductionFactor = REDUC_V1; 


}

void Drivetrain::Set(double v_motor) // set des moteurs
{
    m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
    m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
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
        m_MotorLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);

        m_MotorRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_MotorRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    }
    else
    {
        m_MotorLeft1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorLeft2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorLeft3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);

        m_MotorRight1.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorRight2.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_MotorRight3.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
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

bool Drivetrain::General(double switchTimeLock, double GearboxRightRpm, double GearboxLeftRpm) // mode général, détermine si on peut passer une vitesse
{
    return false;
    if (switchTimeLock <= 0 and (GearboxLeftRpm/GearboxRightRpm) > 0.95//à définir
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
    //distance sortie de boite en nombre de tour
    // double encoder_Gearbox_Right_Distance = m_EncoderRight.GetDistance(); // nombre de tours de l''axe de sortie de boite droite
    // double encoder_Gearbox_Left_Distance = m_EncoderLeft.GetDistance();  // nombre de tours de l'axe de sortie de boite gauche
    // vitesse angulaire en sortie de boite basé sur les encodeurs motors
    // double encoder_Motor_Right_Distance = m_MotorRight1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*reduction_factor);// RPM axe du moteur 1 de la boite droite / par le reduction_factor de réduction
    // double encoder_Motor_Left_Distance = m_MotorLeft1.GetSensorCollection().GetIntegratedSensorVelocity()* 600 / (2048*reduction_factor);// RPM axe du moteur 1 de la boite de gauche / par le reduction_factor de réduction

    // calcul de la vitesse moyenne des deux encodeurs de sortie de boite. (GetDistance renvoie un nombre de tours car setup fait dans le constructeur)(.SetDistancePerPulse(1.0/2048.0)
    // les valeurs sont en tours/tick
    m_GearboxRightOutRpt.setPos(m_EncoderRight.GetDistance() ); 
    m_GearboxLeftOutRpt.setPos(m_EncoderLeft.GetDistance() );
    m_GearboxRightAveragedRpt.add(m_GearboxRightOutRpt.m_deltaPos);
    m_GearboxLeftAveragedRpt.add(m_GearboxLeftOutRpt.m_deltaPos);

    // calcul de la vitesse moyenne des 3 moteurs de la boite gauche et droite avec les encodeurs des talon srx qui retourne des valeurs en ticks/100ms
    // les moyennes sont converties ( * 600 / 2048 ) et stockées en RPM
    double average_left = (m_MotorLeft1.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorLeft2.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorLeft3.GetSensorCollection().GetIntegratedSensorVelocity())/3;
    double average_right = (m_MotorRight1.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorRight2.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorRight3.GetSensorCollection().GetIntegratedSensorVelocity())/3;
    m_SuperMotorLeftRpm.setPos(average_left * 600 / 2048);    
    m_SuperMotorRightRpm.setPos(average_right * 600 / 2048); 
    m_SuperMotorLeftAveragedRpm.add(m_SuperMotorLeftRpm.m_deltaPos);
    m_SuperMotorRightAveragedRpm.add(m_SuperMotorRightRpm.m_deltaPos);

    // Vitesses des boites en RPM construitent en combinant les valeurs encodeurs moteurs et through bore 
    // TRUST_GEARBOX_OUT_ENCODER représente le coeff de confiance qu'on a dans les encodeurs de sortie de boite et (1-TRUST_GEARBOX_OUT_ENCODER) représente la confiance des encodeurs moteurs
    // m_SuperMotorLeftRpm et m_SuperMotorRightRpm sont déjà exprimé en RPM et m_GearboxRightOutRpt et m_GearboxLeftOutRpt sont en tours/tick (RPT), 
    // il faut donc les convertir en RPM ( * (60/TICK_DT) ) 
    // Les m_SuperMotorLeftRpm et m_SuperMotorRightRpm sont les valeurs avant réduction, il faut appliquer le facteur de réduction de boite enclenché pour obtenir une valeur RPM "sortie de boite" (m_CurrentGearboxReductionFactor)
    m_GearboxOutRightRpm = (m_GearboxRightAveragedRpt.get() * (60 / TICK_DT) * TRUST_GEARBOX_OUT_ENCODER + m_SuperMotorRightAveragedRpm.get() * m_CurrentGearboxReductionFactor *(1-TRUST_GEARBOX_OUT_ENCODER )) ;  
    m_GearboxOutLeftRpm = (m_GearboxLeftAveragedRpt.get() * (60 / TICK_DT) * TRUST_GEARBOX_OUT_ENCODER + m_SuperMotorLeftAveragedRpm.get() * m_CurrentGearboxReductionFactor *(1-TRUST_GEARBOX_OUT_ENCODER )) ; 


    m_RobotAccelerationRight = (m_GearboxRightOutRpt.m_acceleration*TRUST_GEARBOX_OUT_ENCODER+m_SuperMotorRightRpm.m_acceleration*(1-TRUST_GEARBOX_OUT_ENCODER)); // accélération partie droite
    m_RobotAccelerationLeft = (m_GearboxLeftOutRpt.m_acceleration*TRUST_GEARBOX_OUT_ENCODER+m_SuperMotorLeftRpm.m_acceleration*(1-TRUST_GEARBOX_OUT_ENCODER)) ; // accélération partie gauche
   

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

    m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
    m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));

    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Fast.m_current", m_rateLimiter_V_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_V_Slow.m_current", m_rateLimiter_V_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Fast.m_current", m_rateLimiter_W_Fast.m_current);
    frc::SmartDashboard::PutNumber("m_rateLimiter_W_Slow.m_current", m_rateLimiter_W_Slow.m_current);

    frc::SmartDashboard::PutNumber("m_Joystick_V_Pure", m_Joystick_V_Pure);
    frc::SmartDashboard::PutNumber("m_Joystick_W_Pure", m_Joystick_W_Pure);

    frc::SmartDashboard::PutNumber("W", );



    switch (m_State)
    {
        case State::lowGear:
        {
            
            // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (General(m_SwitchTimeLock,m_GearboxOutRightRpm, m_GearboxOutLeftRpm))
            {
                if (Up(m_Gearboxes_W_average_RPM,m_Gearboxes_Acceleration,m_Joystick_V_Pure,m_Joystick_V_Acceleration))
                {
                    SwitchUp(m_Gearboxes_W_average_RPM*REDUC_V1);
                    // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_CurrentGearboxReductionFactor = REDUC_V2;
                    m_State = State::highGear;
                }
            }
        }
        break;

        case State::highGear:
        {
            // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (CoastDown(m_Gearboxes_W_average_RPM))
            {
                SwitchDown(m_Gearboxes_W_average_RPM*REDUC_V2);
                // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                SetSwitchTimeLock(0.5);
                m_CurrentGearboxReductionFactor = REDUC_V1;
                m_State = State::lowGear;
            }
            
            if (General(m_SwitchTimeLock,m_Robot_W))
            {
                if (KickDown(m_Gearboxes_W_average_RPM,m_Gearboxes_Acceleration,m_Joystick_V_Pure))
                {
                    SwitchDown(m_Gearboxes_W_average_RPM*REDUC_V2);
                    // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
                    // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
                    SetSwitchTimeLock(0.5);
                    m_CurrentGearboxReductionFactor = REDUC_V1;
                    m_State = State::lowGear;
                }
            }
        }
    }
}
