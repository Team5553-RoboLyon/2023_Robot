#include "lib/RblUtils.h"
#include "ChargeStationTiltTracker.h"

/// @brief 
TiltTracker::TiltTracker():m_dt(0.0),m_tiltA(0.0),m_tiltB(0.0),m_pFrom(&m_tiltA),m_pTo(&m_tiltB), m_angleThreshold(0.0),m_deltaTimeThreshold(0.0),m_aParabolic(1.0/(0.5*0.5)),m_bParabolic(0.0){}

TiltTracker::TiltTracker(const double angle_rate_threshold, const double dt_threshold, const double kmin):m_dt(0.0),m_tiltA(0.0),m_tiltB(0.0),m_pFrom(&m_tiltA),m_pTo(&m_tiltB),m_angleThreshold(angle_rate_threshold),m_deltaTimeThreshold(dt_threshold),m_aParabolic((1.0-kmin)/(0.5*0.5)),m_bParabolic(kmin){}

/// @brief 
TiltTracker::~TiltTracker(){}

/// @brief initialise le Tilttracker. A appeler au pied de la charge station.                                    
/// @param position             valeur renvoyée par le getDistance de l'encodeur gauche en mètres                
/// @param estimated_next_tilt_distance estimation de la distance à laquelle surviendra le prochain Tilt en mètre
void TiltTracker::initialize(const double position,const double estimated_next_tilt_distance)
{
    m_tiltA = position;
    m_tiltB = m_tiltA + estimated_next_tilt_distance;
    m_pFrom = &m_tiltA;
    m_pTo   = &m_tiltB;
}

double TiltTracker::Update(const double dt, const double position, const double angle,const double angle_rate)
{

    m_dt += dt;
    if(m_dt >= m_deltaTimeThreshold)
    {
        m_dt = 0.0;
        if( NSIGN(angle) != NSIGN(angle_rate) )
        {
            if(angle_rate > m_angleThreshold)
            {
                *m_pTo = position;
                double *pd = m_pFrom;
                m_pFrom = m_pTo;
                m_pTo = pd;
            }
        }
    }
   double nrmx =  (position - *m_pFrom) / (*m_pTo - *m_pFrom);
   nrmx = NCLAMP(0,nrmx,1.0) - 0.5;

   m_k = m_aParabolic*nrmx*nrmx + m_bParabolic;
   return m_k; 
}

double TiltTracker::getNormalizeDistance()
{
    return 0.0;
}
