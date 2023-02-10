#include <iostream>
#include <cstdlib>
#include <cmath>

#define NCLAMP(mn,a,mx) ((a)<(mn)) ? (mn) :  ((a)>(mx)) ? (mx) :(a) 

class Angle_AG
{
 public :
    Angle_AG(double dt,double tau,double angle=0.0);
    double Update(double rate_gyro, double xaccel,double zaccel); // x correspond à la verticale vers le haut et y à l'horizontale vers l'avant et z l'ordonée
    double GetAngle(){return m_angle;};
    void Reset(double angle =0.0);
    double m_k; // coef calculé grace à m_tau et dt
    double m_tau; // temp en seconde
    double m_dt; // dela t en seconde
    double m_a;
    double m_b;
    double m_angleAccel;




 private :
    double m_angle; // angle en radian combinant accel et gyro
   

};