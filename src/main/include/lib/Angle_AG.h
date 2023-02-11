#include <iostream>
#include <cstdlib>
#include <cmath>

#define NCLAMP(mn,a,mx) ((a)<(mn)) ? (mn) :  ((a)>(mx)) ? (mx) :(a) 

class Angle_AG
{
  public :
  Angle_AG(double dt,double tau,double angle=0.0);
  double Update(double rate_gyro, double xaccel,double zaccel); 
  void Reset(double angle = 0.0);
  void SetTau(double tau){m_tau=tau; m_k=m_tau/(m_tau+m_dt);};

  double GetAngle(){return m_angle-m_bias;};
  double GetK(){return m_k;};
  double GetTau(){return m_tau;};
  double GetDelta(){return m_delta;};
  void SetBias(double bias){m_bias=bias;};
  void AutoSetBias(){m_bias=m_angle;};




  private :

  double m_angle; // angle en radian combinant accel et gyro
  double m_delta; // différence entre l'angle et l'angle précédent
  double m_dt;    // intervalle de temps en seconde entre 
  double m_tau;   // temp en seconde
  double m_k;     // coef calculé grace à m_tau et dt
  double m_bias; // 
  
};