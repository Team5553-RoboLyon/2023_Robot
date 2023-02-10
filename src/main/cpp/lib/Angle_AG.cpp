#include <lib/Angle_AG.h>

Angle_AG::Angle_AG(double dt,double tau,double angle) : m_angle(angle), m_dt(dt),m_tau(tau) {
    m_k=tau/(tau+dt);
}

//rate_gyro -> rate du gyro en radian/sec
//xaccel et zaccel -> base ortonormée directe avec z pointant vers le haut et x axe horizontale 
double Angle_AG::Update(double rate_gyro, double xaccel, double zaccel)
{
    double a = NCLAMP(-1.0,zaccel,1.0);
    double b = NCLAMP(-1.0,xaccel,1.0);
    // m_angleAccel = acos(a);
    double angleaccel = atan2(b,a);
    m_angle = m_k *(m_angle+rate_gyro*m_dt)+(1.0-m_k)*(angleaccel);
    return m_angle;


//doc : 
// a=tau / (tau + loop time)
// newAngle = angle measured with atan2 using the accelerometer
// newRate = angle measured using the gyro
// looptime = loop time in millis()
// float tau=0.075;
// a=0.0;
// float Complementary(float newAngle, float newRate,int looptime) {
// float dtC = float(looptime)/1000.0; 
// a=tau/(tau+dtC);
// x_angleC= a* (x_angleC + newRate * dtC) + (1-a) * (newAngle);
// return x_angleC;

}

void Angle_AG::Reset(double angle)
{
    m_angle=angle;
}