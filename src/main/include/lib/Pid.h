#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>

class Pid
{

public:
  Pid(double setpoint, double kp, double ki, double kd);
  void SetGains(double kp, double ki, double kd);                                                                                 // définit les gains
  void SetSetpoint(double setpoint) { m_setpoint = setpoint; };                                                                   // définit la consigne
  double Calculate(double measurement);                                                                                           // retourne l'output à appliquer measurement en degrés
  void Reset(double error = 0.0, double lastError = 0.0, double integrative = 0.0, double derivative = 0.0, double output = 0.0); // réinitialise le pid
  void SetTolerance(double tolerance) { m_tolerance = tolerance; };
  double m_error;    // erreur entre la consigne et la mesure
  double m_output;   // output du pid
  double m_setpoint; // consigne

private:
  double m_lastError;   // erreur précédente
  double m_integrative; // intégrale de l'erreur
  double m_derivative;  // dérivée de l'erreur
  double m_tolerance;   // tolérance de la mesure
  double m_kp;          // gain proportionnel
  double m_ki;          // gain intégral
  double m_kd;          // gain dérivé
};