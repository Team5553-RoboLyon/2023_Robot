#define REDUC_V1 13.3                                      // valeur de réduction de la vitesse 1
#define REDUC_V2 8.4                                       // valeur de réduction de la vitesse 2
#define MAX_RPM_MOTEUR 6380                                // a définir
#define COEFF_JVN 0.80                                     // tiré de JVN/a définir
#define MAX_RPM_V1 (MAX_RPM_MOTEUR / REDUC_V1) * COEFF_JVN // rendement de 80% sortie de boite avec reduc 13.3
#define MAX_RPM_V2 (MAX_RPM_MOTEUR / REDUC_V2) * COEFF_JVN // rendement de 80% sortie de boite avec reduc de 8.8
#define MULTIPLICATEUR_PM 50 * 60                          // pour passer des pulses à une minute (SetDistancePerPulse)
#define voltageRef 12.0                                    // tension de référence
#define hop_plus_de_frottements 0.013                    // hop plus de frottements

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
    m_logCSV.setItem(0,"encoderLeft",5,&m_difLeft);
    m_logCSV.setItem(1,"encoderRight",5,&m_difRight);
    m_logCSV.setItem(2,"encoderLeftMotor",5,&m_encoderLeftMotor);
    m_logCSV.setItem(3,"encoderRightMotor",5,&m_encoderRightMotor);
    m_logCSV.setItem(4,"GetswitchgearvoltageLeft",5,&m_getSwitchGearVoltageLeft);
    m_logCSV.setItem(5,"GetswitchgearvoltageRight",5,&m_getSwitchGearVoltageRight);
    m_logCSV.setItem(6,"voltageCourant",5,&m_speed);
    m_logCSV.setItem(7,"état",5,&m_vitesse);
    


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

double Drivetrain::GetSwitchGearVoltageRight(double w, double voltageReference, double reduction, double finalReduction)
{
    double switchGearVoltage = (std::abs(w) + wfRef * hop_plus_de_frottements) * (voltageReference / wfRef);//TODO changer le 0.8
    double percentVoltage = switchGearVoltage / voltageReference ;
    return percentVoltage;

}

double Drivetrain::GetSwitchGearVoltageLeft(double w, double voltageReference, double reduction, double finalReduction)
{
    double switchGearVoltage = (std::abs(w) + wfRef * hop_plus_de_frottements) * (voltageReference / wfRef);//TODO changer le 0.8
    double percentVoltage = switchGearVoltage / voltageReference;
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


void Drivetrain::Drive(double joystickRight, double joystickLeft)
{
    double encodeLeft_tick_per_100ms = m_MotorGearboxLeft1.GetSensorCollection().GetIntegratedSensorVelocity();
    double encodeRight_tick_per_100ms = m_MotorGearboxRight1.GetSensorCollection().GetIntegratedSensorVelocity();
    m_encoderRightMotor = encodeRight_tick_per_100ms * 600 / 2048;
    m_encoderLeftMotor = encodeLeft_tick_per_100ms * 600 / 2048;
    double encoderRight_RPM = std::abs(m_encoderRightMotor);

    double encoder_Right=m_EncoderRight.GetDistance();
    double encoder_Left=m_EncoderLeft.GetDistance();
    m_difRight=(encoder_Right-encoder_RightLast)/0.02*60;
    m_difLeft=(encoder_Left-encoder_LeftLast)/0.02*60;
    double dif_Rightvrai=(encoder_Right-encoder_RightLast)/0.02*60;

    encoder_RightLast=encoder_Right;
    encoder_LeftLast=encoder_Left;

    double signe=m_difRight>0 ? 1 : -1;

    frc::SmartDashboard::PutNumber("rapport",dif_Rightvrai);
    frc::SmartDashboard::PutNumber("encodeur right", dif_Rightvrai);
    frc::SmartDashboard::PutNumber("encodeur left", m_difLeft);
    frc::SmartDashboard::PutNumber("ratio" , m_difLeft/dif_Rightvrai);

    m_getSwitchGearVoltageLeft = signe*GetSwitchGearVoltageLeft(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2);
    m_getSwitchGearVoltageRight = signe*GetSwitchGearVoltageRight(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2);


    switch (m_state)
    {

    case State::Pilote_V1:
        m_vitesse = 1;
        std::cout << "Pilot V1" << std::endl;
        EnableBrakeMode(true);
        m_speed=Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 1);
        m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 1));
        m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 0));
        std::cout << "getswitch " << signe*GetSwitchGearVoltageRight(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2) << std::endl;
        if (std::abs(m_difRight)>=wfRef/REDUC_V1 *0.15) // avant 0.9       odométrie && encodeLeft >= MAX_RPM_V1 -50 encoderRight_RPM>=5500
        {
            m_state = State::Auto_V1toV2_Inter;
            std::cout << "on passe vers V2" << std::endl;
        }
        break;

    case State::Auto_V1toV2_Inter:
        std::cout << "Auto V1 to V2 Inter" << std::endl;
        EnableBrakeMode(false);
        ActiveBallShifterV2();
        m_state = State::Auto_V1toV2;
        break;

    case State::Auto_V1toV2:
        std::cout << "Auto V1 to V2" << std::endl;
        std::cout << "getswitch " << signe*GetSwitchGearVoltageRight(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2) << std::endl;
        m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, signe*GetSwitchGearVoltageLeft(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2));
        m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, signe*GetSwitchGearVoltageRight(m_difRight*REDUC_V1, voltageRef, REDUC_V1, REDUC_V2));
        m_count++;

        if (m_count >= 5)
        {
            m_count = 0;
            m_state = State::Pilote_V2;
            std::cout << "on passe vers V2" << std::endl;
        }
        break;

    case State::Pilote_V2:
        m_vitesse = 2;
        m_speed=Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 1);
        std::cout << "Pilot V2" << std::endl;
        std::cout << "getswitch " << signe*GetSwitchGearVoltageRight(m_difRight, voltageRef, REDUC_V1, REDUC_V2) << std::endl;
        if (std::abs(m_difRight)>=wfRef/REDUC_V2*0.05) // avant 0.7 or encodeLeft >= MAX_RPM_V1 - 50 encoderRight_RPM>=4000
        {
            std::cout << "on passe ds V2" << std::endl;
            EnableBrakeMode(true);
            m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 1));
            m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Calcul_De_Notre_Brave_JM(joystickRight, joystickLeft, 0));
        }
        else
        {
             m_state = State::Auto_V2toV1_Inter;
             std::cout << "cc" << std::endl;
        }
        break;
    case State::Auto_V2toV1_Inter:
        std::cout << "Auto V2 to V1 Inter" << std::endl;
        EnableBrakeMode(false);
        ActiveBallShifterV1();
        m_state = State::Auto_V2toV1;
        break;

    case State::Auto_V2toV1:
        std::cout << "Auto V2 to V1" << std::endl;
        std::cout << "getswitch " << signe*GetSwitchGearVoltageRight(m_difRight*REDUC_V2, voltageRef, REDUC_V2, REDUC_V1) << std::endl;
        m_MotorGearboxLeft1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, signe *GetSwitchGearVoltageRight(m_difRight*REDUC_V2, voltageRef, REDUC_V1, REDUC_V2));
        m_MotorGearboxRight1.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, signe * GetSwitchGearVoltageRight(m_difRight*REDUC_V2, voltageRef, REDUC_V1, REDUC_V2) );
        m_count++;
        if (m_count >= 5)
        {
            m_count = 0;
            m_state = State::Pilote_V1;
            std::cout << "on passe vers V1" << std::endl;
        }
        break;
    }
}
