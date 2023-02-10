#include <lib/Pid.h>

Pid::Pid(double setpoint, double kp, double ki, double kd) : m_setpoint(0.0), m_kp(0.0), m_ki(0.0), m_kd(0.0){}

void Pid::SetGains(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void Pid::SetSetpoint(double setpoint)
{
    m_setpoint = setpoint;
}

double Pid::Update(double measurement)
{
    m_error = m_setpoint - measurement; // calcul de l'erreur entre la consigne et la mesure
    m_integrative += m_error; // ajout de l'erreur
    m_derivative = m_error - m_lastError; // calcul de la dérivée de l'erreur 
    m_lastError = m_error; // sauvegarde de l'erreur précédente
    m_output = m_kp * m_error + m_ki * m_integrative + m_kd * m_derivative; // calcul de l'output
    return m_output;
}