#pragma once

typedef struct Dynamic Dynamic;
struct Dynamic
{

    Dynamic() : m_current(0.0), m_delta(0.0) {};
    Dynamic(double initialvalue) : m_current(initialvalue), m_delta(0.0)  {};
    ~Dynamic() {};
    double m_current;
    double m_delta;
    
    void set(double value) 
    { 
        m_delta = (value - m_current);
        m_current   = value; 
    };
    void reset(double initialvalue ) 
    { 
        m_current   = initialvalue; 
        m_delta     = 0.0; 
    };


};
