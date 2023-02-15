#include "lib/RblUtils.h"
#include "ChargeStationTiltTracker.h"
#include <iostream>

/// @brief 
TiltTracker::TiltTracker(): m_filteredRateAverage(),
                            /*m_dt(0.0),*/
                            m_tiltA(0.0),
                            m_tiltB(0.0),
                            m_pFrom(&m_tiltA),
                            m_pTo(&m_tiltB), 
                            m_threshold(0.0),
                            m_deltaTimeThreshold(0.0),
                            m_aParabolic(1.0/(0.5*0.5)),
                            m_bParabolic(0.0),
                            m_peakWidth(0.0),
                            m_tilted(0),
                            m_angle(0.0),
                            m_peakInfluence(0.0),
                            m_previousFilteredRate(0.0),
                            m_k(0.0){}

TiltTracker::TiltTracker(const int average_samples_nb, const double threshold, const double dt_threshold, const double kmin):   m_filteredRateAverage(average_samples_nb),
                                                                                                                                /*m_dt(0.0),*/
                                                                                                                                m_tiltA(0.0),
                                                                                                                                m_tiltB(0.0),
                                                                                                                                m_pFrom(&m_tiltA),
                                                                                                                                m_pTo(&m_tiltB),
                                                                                                                                m_threshold(threshold),
                                                                                                                                m_deltaTimeThreshold(dt_threshold),
                                                                                                                                m_aParabolic((1.0-kmin)/(0.5*0.5)),
                                                                                                                                m_bParabolic(kmin),
                                                                                                                                m_peakWidth(0.0),
                                                                                                                                m_tilted(0),
                                                                                                                                m_angle(0.0),
                                                                                                                                m_peakInfluence(0.0),
                                                                                                                                m_previousFilteredRate(0.0),
                                                                                                                                m_k(0.0){}

/// @brief 
TiltTracker::~TiltTracker(){}

/// @brief initialise le Tilttracker. A appeler au pied de la charge station.                                    
/// @param position             valeur renvoyée par le getDistance de l'encodeur gauche en mètres                
/// @param estimated_next_tilt_distance estimation de la distance à laquelle surviendra le prochain Tilt en mètre
void TiltTracker::initialize(const double angle,const double position,const double estimated_next_tilt_distance)
{
    m_tilted= 0;
    m_angle = angle;
    m_tiltA = position;
    m_tiltB = m_tiltA + estimated_next_tilt_distance;
    m_pFrom = &m_tiltA;
    m_pTo   = &m_tiltB;
    m_filteredRateAverage.reset(0,0.0);
    m_previousFilteredRate = 0.0;
}

bool TiltTracker::Update(const double dt, const double position, const double angle,const double angle_rate)
{
    bool r=false;

 /*
  * ------------------------------------------------------------------------------------
  * V0.0
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
*    
* ------------------------------------------------------------------------------------
*/

/*
 * ------------------------------------------------------------------------------------
 * V0.1
*/
// Dans cette nouvelle version, angle_rate est supposé être la valeur brute de Gyro rate le tiltTracker incluant cette moyenne !
double dv = angle_rate - m_filteredRateAverage.m_mean;
double filtered_rate;
bool btilt = false;
// ci-dessous, ... pour eviter la racine carrée du "vrai test" à effectuer à partir de l'écart type: if(NABS(dv) > m_angleThreshold*sqrt(m_rateAverage.m_variance) )
// on utilise la variance ( = ecart type au carré ), threshold devra donc être ajusté en conséquence ( élevé au carré pour conserver les mêmes résultats qu'avec la standard deviation)
if((dv*dv) > m_threshold*m_filteredRateAverage.m_variance)
{
    // à partir d'ici nous sommes sûrs que : dv != 0.0

    if(dv > 0.0) // angle_rate > m_rateAverage.m_mean
    {
        // pic positif
        if(m_peakWidth<0.0)
            m_peakWidth = 0.0;
        
        m_peakWidth += dt;
        if(m_peakWidth > m_deltaTimeThreshold)
        {
             if(m_tilted<=0)
             {
               m_tilted = 1;
               btilt    = true;
             } 
        }
    }
    else        // angle_rate < m_rateAverage.m_mean 
    {
        // pic negatif
        if(m_peakWidth>0.0)
            m_peakWidth = 0.0;
        
        m_peakWidth = -dt;
        if(m_peakWidth < -m_deltaTimeThreshold)
        {
             if(m_tilted>=0)
             {
               m_tilted = -1;
               btilt    = true;
             } 
        }
    }
    
    filtered_rate = m_peakInfluence*angle_rate + (1.0-m_peakInfluence)*m_previousFilteredRate;
}
else
{
    filtered_rate = angle_rate;
}
m_filteredRateAverage.addSample(filtered_rate);

// tilt ?
if(tilt != 0)
{
    *m_pTo = position;
    double *pd = m_pFrom;
    m_pFrom = m_pTo;
    m_pTo = pd;
}
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
