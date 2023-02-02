

class RateLimiter {

 public:
  RateLimiter(): m_target(0.0), m_current(0.0), m_speed(0.0){};

  double Update(double target); // Update la valeur de current en fonction de la vitesse et du target
  double Update(); // Update la valeur de current en fonction de la vitesse 
  void SetTarget(double target); // Set un nouveau target
  void SetCurrent(double current); // Set un nouveau current
  void SetSpeed (double speed); // Set une nouvelle vitesse
  void Reset(double target, double current, double speed); // Reset tout avec des valeurs que l'on choisit je sais pas si c'est ce que tu pensais
//   void SetRecul(double current, double speed_1, double speed_2); // Set le recul

  double m_target; 
  double m_current;
  double m_speed;
 private:
  
//   double m_speedSwitch;
//   double m_recul;


};