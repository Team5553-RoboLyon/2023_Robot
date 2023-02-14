#pragma once
#include <cstdlib>
#include <math.h>
#include "lib/RblUtils.h"
#define K_MIN 0.2 
#define K_MAX 0.8

class TiltTracker
{
public:
	TiltTracker();
    TiltTracker(const double angle_rate_threshold, const double dt_threshold, const double m_kmin);
	~TiltTracker();
    
    void initialize(const double angle,const double position,const double estimated_next_tilt_distance);
 	bool Update(const double dt, const double position, const double angle,const double angle_rate);
    double getDistanceBetweenTilts(){return NABS(m_tiltB - m_tiltA);}
    double getAlgebricDistanceBetweenTilts(){return m_tiltB - m_tiltA;}
    double getNormalizeDistance();
    
    double	m_tiltA;
	double	m_tiltB;
    double* m_pFrom;
    double* m_pTo;

    double  m_dt;
    double  m_angleThreshold;
    double  m_deltaTimeThreshold;
    
    
    double m_aParabolic;
    double m_bParabolic;    // = m_k minimum
    double m_k;
    double m_angle;
    
    
};
