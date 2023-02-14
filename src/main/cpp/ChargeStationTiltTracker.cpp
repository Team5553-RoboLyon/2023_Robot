#include "lib/RblUtils.h"
#include "ChargeStationTiltTracker.h"
#include <iostream>

/// @brief 
TiltTracker::TiltTracker():m_dt(0.0),m_tiltA(0.0),m_tiltB(0.0),m_pFrom(&m_tiltA),m_pTo(&m_tiltB), m_angleThreshold(0.0),m_deltaTimeThreshold(0.0),m_aParabolic(1.0/(0.5*0.5)),m_bParabolic(0.0),m_angle(0.0){}

TiltTracker::TiltTracker(const double angle_rate_threshold, const double dt_threshold, const double kmin):m_dt(0.0),m_tiltA(0.0),m_tiltB(0.0),m_pFrom(&m_tiltA),m_pTo(&m_tiltB),m_angleThreshold(angle_rate_threshold),m_deltaTimeThreshold(dt_threshold),m_aParabolic((1.0-kmin)/(0.5*0.5)),m_bParabolic(kmin),m_angle(0.0){}

/// @brief 
TiltTracker::~TiltTracker(){}

/// @brief initialise le Tilttracker. A appeler au pied de la charge station.                                    
/// @param position             valeur renvoyée par le getDistance de l'encodeur gauche en mètres                
/// @param estimated_next_tilt_distance estimation de la distance à laquelle surviendra le prochain Tilt en mètre
void TiltTracker::initialize(const double angle,const double position,const double estimated_next_tilt_distance)
{
    m_angle=angle;
    m_tiltA = position;
    m_tiltB = m_tiltA + estimated_next_tilt_distance;
    m_pFrom = &m_tiltA;
    m_pTo   = &m_tiltB;
}

bool TiltTracker::Update(const double dt, const double position, const double angle,const double angle_rate)
{
    bool r=false;
    m_dt += dt;
    if(m_dt >= m_deltaTimeThreshold)
    {
        std::cout<<"(m_dt >= m_deltaTimeThreshold)"<<std::endl;
        double dif = m_angle-angle;
        // if( NSIGN(angle) != NSIGN(angle_rate) )
        if (abs(dif)>m_angleThreshold)
        {
            std::cout<<"(abs(dif)>m_angleThreshold"<<dif<<std::endl;
            // if(angle_rate > m_angleThreshold)
            // {
                r=true;
                *m_pTo = position;
                double *pd = m_pFrom;
                m_pFrom = m_pTo;
                m_pTo = pd;
                m_dt = 0.0;

            // }
        }
    }
    m_angle=angle;

//-----------------------parabolic---------------------------
//    double nrmx =  (position - *m_pFrom) / (*m_pTo - *m_pFrom);
//    nrmx = NCLAMP(0,nrmx,1.0) - 0.5;

//    m_k = m_aParabolic*nrmx*nrmx + m_bParabolic;

//----------------------------cos-----------------------------
   double nrmx =  (position - *m_pFrom) / (*m_pTo - *m_pFrom);
   nrmx = NCLAMP(0,nrmx,1.0) *NF64_PI;
   nrmx=(cos(nrmx)+1.0)/2.0;
   m_k =  m_bParabolic + nrmx*(1-m_bParabolic); // m_bparabole = m_cosinus min

//----------------------------cos2pi--------------------------
//    double nrmx =  (position - *m_pFrom) / (*m_pTo - *m_pFrom);
//    nrmx = NCLAMP(0,nrmx,1.0) *NF64_2PI;
//    nrmx=(cos(nrmx)+1.0)/2.0;

//    m_k =  m_bParabolic + nrmx*(1-m_bParabolic); // m_bparabole = m_cosinus min

   return r; 
}

double TiltTracker::getNormalizeDistance()
{
    return 0.0;
}
