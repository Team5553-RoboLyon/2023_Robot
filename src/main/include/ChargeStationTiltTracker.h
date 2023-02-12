#pragma once
#include <cstdlib>
#include "lib/RblUtils.h"
#define K_MIN 0.2 
#define K_MAX 0.8

class TiltTracker
{
public:
	TiltTracker();
    TiltTracker(double angle_threshold);
	~TiltTracker();
    
    void initialize(double left_encoder_dist, double right_encoder_dist, double m_angle, double estimated_next_tilt_distance );
 	bool DetectTiltPoint(double left_encoder_dist, double right_encoder_dist,double angle);
    double getDistanceBetweenTilts(){return NABS(m_tiltB - m_tiltA);}
    double getAlgebricDistanceBetweenTilts(){return m_tiltB - m_tiltA;}
    double getNormalizeDistance();
    double	m_tiltA;
	double	m_tiltB;
    double* m_pTilt;
    double  m_angle;
    double  m_angleThreshold;
    double m_kAnticipation;
    double m_a;
    
    
};
