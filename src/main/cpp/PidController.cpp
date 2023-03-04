// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "PidController.h"
#define HABS(a) ((a) < 0.0 ? -(a) : (a))

Pid::Pid(double setpoint, double kp, double ki, double kd) : m_setpoint(0.0), m_kp(kp), m_ki(ki), m_kd(kd) {}

void Pid::SetGains(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    std::cout << m_kp << m_ki << m_kd << std::endl;
}

double Pid::Calculate(double measurement)
{
    m_error = m_setpoint - measurement;                                     // calcul de l'erreur entre la consigne et la mesure
    m_integrative += m_error;                                               // ajout de l'erreur
    m_derivative = m_error - m_lastError;                                   // calcul de la dérivée de l'erreur
    m_lastError = m_error;                                                  // sauvegarde de l'erreur précédente
    m_output = m_kp * m_error + m_ki * m_integrative + m_kd * m_derivative; // calcul de l'output
    if (HABS(m_error) < m_tolerance)
    {
        m_output = 0.0;
    } // si l'erreur est inférieure à la tolérance on arrête le moteur
    std::cout << m_output << std::endl;
    std::cout << m_setpoint << "m_setpoint" << std::endl;
    return m_output;
}

void Pid::Reset(double error, double lastError, double integrative, double derivative, double output)
{
    m_error = error;
    m_lastError = lastError;
    m_integrative = integrative;
    m_derivative = derivative;
    m_output = output;
}
