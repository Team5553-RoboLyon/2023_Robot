class Pid 
{
    Pid(double setpoint, double kp, double ki, double kd);

  public:

    void SetGains(double kp, double ki, double kd); // définit les gains
    void SetSetpoint(double setpoint); // définit la consigne
    double Update(double measurement); // retourne l'output à appliquer measurement en degrés

  private:

  double m_output; // output du pid
  double m_error; // erreur entre la consigne et la mesure
  double m_lastError; // erreur précédente
  double m_setpoint; // consigne
  double m_integrative; // intégrale de l'erreur
  double m_derivative; // dérivée de l'erreur
  double m_kp; // gain proportionnel
  double m_ki; // gain intégral
  double m_kd; // gain dérivé
};