#pragma once

typedef struct DynamicData DynamicData;
struct DynamicData
{

    DynamicData() : m_current(0.0), m_previous(0.0), m_delta(0.0) {};
    DynamicData(double initialvalue) : m_current(initialvalue), m_previous(initialvalue), m_delta(0.0)  {};
    ~DynamicData() {};
    double m_current;
    double m_previous;
    double m_delta;
    
    void set(double value) 
    { 
        m_previous  = m_current; 
        m_current   = value; 
        m_delta = (m_current - m_previous);
    };
    void reset(double initialvalue ) 
    { 
        m_current   = initialvalue; 
        m_previous  = initialvalue; 
        m_delta     = 0.0; 
    };


};
