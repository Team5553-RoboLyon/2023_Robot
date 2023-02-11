#include "lib/RblUtils.h"
#include "ChargeStationTiltTracker.h"

/// @brief 
TiltTracker::TiltTracker():m_tiltA(0.0),m_tiltB(0.0),m_pTilt(&m_tiltB), m_angle(0.0),m_angleThreshold(0.0){}

TiltTracker::TiltTracker(double angle_threshold):m_tiltA(0.0),m_tiltB(0.0),m_angle(0.0),m_angleThreshold(angle_threshold){}

/// @brief 
TiltTracker::~TiltTracker(){}

/// @brief initialise le Tilttracker. A appeler au pied de la charge station.                                    
/// @param left_encoder_dist    valeur renvoyée par le getDistance de l'encodeur gauche en mètres                
/// @param right_encoder_dist   valeur renvoyée par le getDistance de l'encodeur droit en mètres                
/// @param angle                angle actuel du robot ( sera considéré comme premier angle de tilt )
/// @param estimated_next_tilt_distance estimation de la distance à laquelle surviendra le prochain Tilt en mètre
void TiltTracker::initialize(double left_encoder_dist, double right_encoder_dist, double angle, double estimated_next_tilt_distance)
{
    m_tiltA = (left_encoder_dist + right_encoder_dist)/2.0;
    m_tiltB = m_tiltA + estimated_next_tilt_distance;
    m_angle = angle;
}

void TiltTracker::DetectTiltPoint(double left_encoder_dist, double right_encoder_dist, double angle)
{
    if( NSIGN(angle) != NSIGN(m_angle) )
    {
        double dif = angle - m_angle;
        dif = NABS(dif);
        if(dif > m_angleThreshold)
        {
            m_angle = angle;
            *m_pTilt = (left_encoder_dist + right_encoder_dist)/2.0;
            m_pTilt = (m_pTilt == &m_tiltA) ? &m_tiltB:&m_tiltA;
        }
    }
}
