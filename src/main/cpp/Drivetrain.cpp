#define AMAX 5.1 // Acceleration Max  au PIF .. à définir aux encodeurs
#define VMAX 3.4 // vitesse Max  théorique (3,395472 sur JVN-DT) .. à vérifier aux encodeurs
#define WMAX                       \
    (((2.0 * VMAX) / AXLETRACK) / \
     1.7) // vitesse angulaire Max theorique	.. à modifier avec Garice

#define NABS(a)         (((a) < 0) ? -(a) : (a))     // VALEUR ABSOLUE
#define NMAX(a, b)      (((a) > (b)) ? (a) : (b)) // Max
#define NMIN(a, b)      (((a) < (b)) ? (a) : (b)) // Min
#define NROUND(fval)	( ( (fval) >= 0.0f ) ? ((Ns32)((fval) + 0.5f)) : ((Ns32)((fval) - 0.5f)) )
#define NSIGN(a)        (((a)<0) ? -1:1)	
#define NCLAMP(mn,a,mx)	( ((a)<(mn)) ? (mn) : ((a)>(mx)) ? (mx) : (a) )
#define NLERP(a,b,t)	( a + (b - a)*t )

#include "Drivetrain.h"
#include "lib/Dynamic.h"
#include "lib/utils.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

Drivetrain::Drivetrain() :  m_GearboxLeftOutAveragedRpt(AVERAGE_SAMPLES_NUMBER), 
                            m_GearboxRightOutAveragedRpt(AVERAGE_SAMPLES_NUMBER),
                            m_SuperMotorLeftAveragedRpm(AVERAGE_SAMPLES_NUMBER), 
                            m_SuperMotorRightAveragedRpm(AVERAGE_SAMPLES_NUMBER),
                            m_GearboxesOutAveragedAccelerationRpm2(AVERAGE_SAMPLES_NUMBER)

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
    
    // Les valeurs sont inversée pour que l'avant soit à l'avant et l'arrière à l'arrière
    m_EncoderRight.SetDistancePerPulse(-1.0/2048.0); // 1 tour correspond à 2048 ticks
    m_EncoderLeft.SetDistancePerPulse(-1.0/2048.0);  // 1 tour correspond à 2048 ticks

    m_EncoderLeft.Reset(); // reset des encodeurs
    m_EncoderRight.Reset(); 

    m_JoystickPrelimited_V.Reset(0.0,0.0,2.0); // reset des rate limiters
    m_JoystickLimited_V.Reset(0.0,0.0,0.05);

    m_JoystickPrelimited_W.Reset(0.0,0.0,2.0);
    m_JoystickLimited_W.Reset(0.0,0.0,0.05);

    m_logCSV.setItem(0,"joystick_V",5,&m_JoystickRaw_V.m_current);
    m_logCSV.setItem(1,"joystick_W",5,&m_JoystickRaw_W.m_current);
    m_logCSV.setItem(2,"SpeedRobot",5,&m_GearboxesOutAdjustedRpm.m_current);
    m_logCSV.setItem(3,"état",5,&m_CurrentGearboxRatio);
    m_logCSV.setItem(4,"rate_limiter_Fast_V",5,&m_JoystickLimited_V.m_current);
    m_logCSV.setItem(5,"rate_limiter_Slow_V",5,&m_JoystickPrelimited_V.m_current);
    m_logCSV.setItem(6,"rate_limiter_Fast_W",5,&m_JoystickLimited_W.m_current);
    m_logCSV.setItem(7,"rate_limiter_Slow_W",5,&m_JoystickPrelimited_W.m_current);

    // Règle le ball shifter, le State et la Reduction en V1 lors de l'initialisation du robot
    ActiveBallShifterV1(); 
    m_State =State::lowGear;
    m_CurrentGearboxRatio = REDUC_V1; 


}

void Drivetrain::Set(double v_motor) // set des moteurs
{
    m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
    m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, v_motor);
}

void Drivetrain::ActiveBallShifterV1() // active ball shifter V1
{
    std::cout<<"ActiveBallShifterV1"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kForward);
}

void Drivetrain::ActiveBallShifterV2() // active ball shifter V2
{
    std::cout<<"ActiveBallShifterV2"<<std::endl;
    m_BallShifterSolenoidLeft.Set(frc::DoubleSolenoid::Value::kReverse);
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

int Drivetrain::GetSpeedSign() // retourne le signe de la vitesse du robot
{
    return m_GearboxesOutAdjustedRpm.m_current > 0.0 ? 1 : -1;
}

double Drivetrain::GetGearShiftingVoltage() // calcule la tension de référence en fonction de la vitesse du robot
{

    double u = NABS(m_GearboxesOutAdjustedRpm.m_current*m_CurrentGearboxRatio)/MOTOR_WF_RPM + RESIST_TORQUE_NM/MOTOR_TS_NM;
    return m_GearboxesOutAdjustedRpm.m_current > 0.0 ? NCLAMP(0.0,u,1.0):-NCLAMP(0.0,u,1.0);
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

bool Drivetrain::isUpshiftingAllowed() // mode up, détermine si on peut passer en V2
{
    // Le Gear shifting précédent est-il suffisamment "ancien" ?  (m_GearShiftingTimeLock doit être  null )
    // Le robot est-il en train de rouler tout droit, sans tourner ?  (m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm doit être proche de 1 )
    // La vérification se fait pour un robot allant en marche avant (m_GearboxesOutAdjustedRpm.m_current > 0.0)
    if (m_GearboxesOutAdjustedRpm.m_current > 0.0)
    {
        if ((m_GearShiftingTimeLock == 0.0) and (m_GearboxRightOutAdjustedRpm) and (m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm < (1 + TURNING_TOLERANCE)) and ((1 - TURNING_TOLERANCE) < m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm))
        {
            if (    m_GearboxesOutAdjustedRpm.m_current           > UP_SHIFTING_POINT_GEARBOXES_OUT_RPM  and 
                    m_GearboxesOutAveragedAccelerationRpm2.get()  > UP_SHIFTING_POINT_GEARBOXES_OUT_RPM2 and 
                    m_JoystickRaw_V.m_current                     > UP_SHIFTING_POINT_JOYSTICK_V and 
                    m_JoystickRaw_V.m_delta                       >=UP_SHIFTING_POINT_JOYSTICK_V_VARIATION )
                    return true;
            else
                    return false;
        }
    }

    // La vérification se fait pour un robot allant en marche arrière (m_GearboxesOutAdjustedRpm.m_current < 0.0) 
    else 
    {
        if ((m_GearShiftingTimeLock == 0.0) and (m_GearboxRightOutAdjustedRpm) and (m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm < (1 + TURNING_TOLERANCE)) and ((1 - TURNING_TOLERANCE) < m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm))
        {
            if (    -m_GearboxesOutAdjustedRpm.m_current           > UP_SHIFTING_POINT_GEARBOXES_OUT_RPM  and 
                    -m_GearboxesOutAveragedAccelerationRpm2.get()  > UP_SHIFTING_POINT_GEARBOXES_OUT_RPM2 and 
                    -m_JoystickRaw_V.m_current                     > UP_SHIFTING_POINT_JOYSTICK_V and 
                    -m_JoystickRaw_V.m_delta                       >=UP_SHIFTING_POINT_JOYSTICK_V_VARIATION )
                    return true;
            else
                    return false;
        }
    }
}

bool Drivetrain::isKickdownShiftingAllowed() // mode kickdown, détermine si on peut passer en V1
{
    //double speedRobot, double accelerationRobot, double joystick
    return false;
    if (m_GearboxesOutAdjustedRpm.m_current > 0.0) 
    {   
        if (    m_GearboxesOutAdjustedRpm.m_current            < KICKDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM  and
                m_GearboxesOutAveragedAccelerationRpm2.get()   < 0.0 and
                m_JoystickRaw_V.m_current                      > KICKDOWN_SHIFTING_POINT_JOYSTICK_V and 
                m_JoystickRaw_V.m_delta                        >=KICKDOWN_SHIFTING_POINT_JOYSTICK_V_VARIATION )
            return true;
        else
            return false;
    }
    else 
    {
        if (    -m_GearboxesOutAdjustedRpm.m_current            < KICKDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM  and
                -m_GearboxesOutAveragedAccelerationRpm2.get()   < 0.0 and
                -m_JoystickRaw_V.m_current                      > KICKDOWN_SHIFTING_POINT_JOYSTICK_V and 
                -m_JoystickRaw_V.m_delta                        >=KICKDOWN_SHIFTING_POINT_JOYSTICK_V_VARIATION )
            return true;
        else
            return false;

    }

}

bool Drivetrain::isCoastdownShiftingAllowed() // mode coastdown, détermine si on peut passer en V1
{
    if (m_GearboxesOutAdjustedRpm.m_current > 0.0)
    {   
        if (m_GearboxesOutAdjustedRpm.m_current < COASTDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM)
            return true;
        else
            return false;
    }
    else
    {   
        if (-m_GearboxesOutAdjustedRpm.m_current < COASTDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM)
            return true;
        else
            return false;
    }
}


void Drivetrain::ShiftGearUp() // passage de la vitesse en V2
{
    double u = GetGearShiftingVoltage();
    m_JoystickPrelimited_V.m_current = u - ((m_JoystickLimited_V.m_current - u)/m_JoystickLimited_V.m_speed )*m_JoystickPrelimited_V.m_speed;
    m_JoystickLimited_V.Update(m_JoystickPrelimited_V.m_current);
    ActiveBallShifterV2();
}

void Drivetrain::ShiftGearDown() // passage de la vitesse en V1
{
   double u = GetGearShiftingVoltage();
    m_JoystickPrelimited_V.m_current = u - ((m_JoystickLimited_V.m_current - u)/m_JoystickLimited_V.m_speed )*m_JoystickPrelimited_V.m_speed;
    m_JoystickLimited_V.Update(m_JoystickPrelimited_V.m_current);
    ActiveBallShifterV1();
}




void Drivetrain::Drive(double joystick_V, double joystick_W) // 
{   
    // calcul de la vitesse moyenne des deux encodeurs de sortie de boite. (GetDistance renvoie un nombre de tours car setup fait dans le constructeur)(.SetDistancePerPulse(1.0/2048.0)
    // les valeurs sont en tours/tick
    m_GearboxRightOutRawRpt.set(m_EncoderRight.GetDistance() ); 
    m_GearboxRightOutAveragedRpt.add(m_GearboxRightOutRawRpt.m_delta);
    m_GearboxLeftOutRawRpt.set(m_EncoderLeft.GetDistance() );
    m_GearboxLeftOutAveragedRpt.add(m_GearboxLeftOutRawRpt.m_delta);

    // calcul de la vitesse moyenne des 3 moteurs de la boite gauche et droite avec les encodeurs des talon srx qui retourne des valeurs en ticks/100ms
    // les moyennes sont converties ( * 600 / 2048 ) et stockées en RPM
    // La valeur de m_SuperMotorLeftRawRpm est inversée 
    m_SuperMotorLeftRawRpm = -(m_MotorLeft1.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorLeft2.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorLeft3.GetSensorCollection().GetIntegratedSensorVelocity())*600/(3*2048);
    m_SuperMotorLeftAveragedRpm.add(m_SuperMotorLeftRawRpm);
    m_SuperMotorRightRawRpm = (m_MotorRight1.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorRight2.GetSensorCollection().GetIntegratedSensorVelocity() + m_MotorRight3.GetSensorCollection().GetIntegratedSensorVelocity())*600/(3*2048);
    m_SuperMotorRightAveragedRpm.add(m_SuperMotorRightRawRpm);

    // Vitesses des boites en RPM construitent en combinant les valeurs encodeurs moteurs et through bore 
    // TRUST_GEARBOX_OUT_ENCODER représente le coeff de confiance qu'on a dans les encodeurs de sortie de boite et (1-TRUST_GEARBOX_OUT_ENCODER) représente la confiance des encodeurs moteurs
    // m_SuperMotorLeftRpm et m_SuperMotorRightRpm sont déjà exprimé en RPM et m_GearboxRightOutRpt et m_GearboxLeftOutRpt sont en tours/tick (RPT), 
    // Il faut donc les convertir en RPM ( * (60/TICK_DT) ). 
    // Les m_SuperMotorLeftRpm et m_SuperMotorRightRpm sont les valeurs avant réduction, il faut appliquer le facteur de réduction de boite enclenché 
    // pour obtenir une valeur RPM "sortie de boite" (m_CurrentGearboxReductionFactor)
    m_GearboxRightOutAdjustedRpm = (m_GearboxRightOutAveragedRpt.get() * (60 / TICK_DT) * TRUST_GEARBOX_OUT_ENCODER + (m_SuperMotorRightAveragedRpm.get() / m_CurrentGearboxRatio) *(1-TRUST_GEARBOX_OUT_ENCODER )) ;  
    m_GearboxLeftOutAdjustedRpm = (m_GearboxLeftOutAveragedRpt.get() * (60 / TICK_DT) * TRUST_GEARBOX_OUT_ENCODER + (m_SuperMotorLeftAveragedRpm.get() / m_CurrentGearboxRatio) *(1-TRUST_GEARBOX_OUT_ENCODER )) ; 

    m_GearboxesOutAdjustedRpm.set(( m_GearboxRightOutAdjustedRpm + m_GearboxLeftOutAdjustedRpm )/2.0);
    m_GearboxesOutAveragedAccelerationRpm2.add(m_GearboxesOutAdjustedRpm.m_delta);

    // 
    m_JoystickRaw_V.set(joystick_V); 
    m_JoystickLimited_V.Update( m_JoystickPrelimited_V.Update(joystick_V) );

    m_JoystickRaw_W.set(joystick_W);
    m_JoystickLimited_W.Update( m_JoystickPrelimited_W.Update(joystick_W) );


    // décrémentation du temps de verrouillage de la vitesse    
    if(m_GearShiftingTimeLock >= TICK_DT)
        m_GearShiftingTimeLock -= TICK_DT; 
    else
        m_GearShiftingTimeLock = 0.0;
 
    switch (m_State)
    {
        case State::lowGear:
        {
            
            // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (isUpshiftingAllowed())
            {
                ShiftGearUp();
                m_GearShiftingTimeLock    = GEARSHIFTING_TIMELOCK;
                m_CurrentGearboxRatio     = REDUC_V2;
                m_State                   = State::highGear;
            }
        }
        break;

        case State::highGear:
        {
            // m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 0));
            // m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_rateLimiter_V_Slow.m_current, m_rateLimiter_W_Slow.m_current, 1));
            if (isCoastdownShiftingAllowed() or isKickdownShiftingAllowed())
            {
                ShiftGearDown();
                m_GearShiftingTimeLock  = GEARSHIFTING_TIMELOCK;
                m_CurrentGearboxRatio   = REDUC_V1;
                m_State                 = State::lowGear;
            }
        }
    }

    m_MotorLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_JoystickLimited_V.m_current, m_JoystickLimited_W.m_current, 0));
    m_MotorRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(m_JoystickLimited_V.m_current, m_JoystickLimited_W.m_current, 1));

    frc::SmartDashboard::PutNumber("m_JoystickPrelimited_V",    m_JoystickPrelimited_V.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickLimited_V",       m_JoystickLimited_V.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickPrelimited_W",    m_JoystickPrelimited_W.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickLimited_W",       m_JoystickLimited_W.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickRaw_V", m_JoystickRaw_V.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickRaw_W", m_JoystickRaw_W.m_current);
    // frc::SmartDashboard::PutString("m_State", m_State == State::lowGear ? "lowGear" : "highGear");
    frc::SmartDashboard::PutNumber("m_State", (double) m_State);
    frc::SmartDashboard::PutNumber("m_GearShiftingTimeLock", m_GearShiftingTimeLock);
    frc::SmartDashboard::PutNumber("m_CurrentGearboxRatio", m_CurrentGearboxRatio);

    frc::SmartDashboard::PutNumber("m_GearShiftingTimeLock", m_GearShiftingTimeLock);
    frc::SmartDashboard::PutNumber("m_GearboxesOutAdjustedRpm", m_GearboxesOutAdjustedRpm.m_current);
    frc::SmartDashboard::PutNumber("m_GearboxesOutAccelerationRpm2", m_GearboxesOutAveragedAccelerationRpm2.get());
    frc::SmartDashboard::PutNumber("m_JoystickRaw_V", m_JoystickRaw_V.m_current);
    frc::SmartDashboard::PutNumber("m_JoystickRaw_V_Acceleration", m_JoystickRaw_V.m_delta);
    frc::SmartDashboard::PutNumber("Gearboxes_Ration", m_GearboxLeftOutAdjustedRpm/m_GearboxRightOutAdjustedRpm);
    // frc::SmartDashboard::PutNumber("GetEncoderRight", m_EncoderRight.GetDistance());
    // frc::SmartDashboard::PutNumber("GetEncoderLeft", m_EncoderLeft.GetDistance());
    // frc::SmartDashboard::PutNumber("GetEncoderMotorRight", m_SuperMotorRightRawRpm);
    // frc::SmartDashboard::PutNumber("GetEncoderMotorLeft", m_SuperMotorLeftRawRpm);
    frc::SmartDashboard::PutNumber("m_GearboxLeftOutAdjustedRpm", m_GearboxLeftOutAdjustedRpm);  
    frc::SmartDashboard::PutNumber("m_GearboxRightOutAdjustedRpm", m_GearboxRightOutAdjustedRpm);
    frc::SmartDashboard::PutNumber("m_SuperMotorRightAveragedRpm", (m_SuperMotorRightAveragedRpm.get() / m_CurrentGearboxRatio));
    frc::SmartDashboard::PutNumber("m_SuperMotorLeftAveragedRpm", (m_SuperMotorLeftAveragedRpm.get() / m_CurrentGearboxRatio));
    frc::SmartDashboard::PutNumber("UP_SHIFTING_POINT_GEARBOXES_OUT_RPM", UP_SHIFTING_POINT_GEARBOXES_OUT_RPM);
    frc::SmartDashboard::PutNumber("KICKDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM", KICKDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM);
    frc::SmartDashboard::PutNumber("COASTDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM", COASTDOWN_SHIFTING_POINT_GEARBOXES_OUT_RPM);
    frc::SmartDashboard::PutNumber("m_GearboxesOutAveragedAccelerationRpm2", m_GearboxesOutAveragedAccelerationRpm2.get());
    frc::SmartDashboard::PutNumber("m_GearboxesOutAdjustedRpm.m_delta", m_GearboxesOutAdjustedRpm.m_delta);
   // frc::SmartDashBoard::PutNumber("Total_current", m_PDP.GetTotalCurrent());


    





}
