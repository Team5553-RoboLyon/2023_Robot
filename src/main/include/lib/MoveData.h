#pragma once

typedef struct MoveData MoveData;
struct MoveData
{

    MoveData() : m_pos(0.0), m_prevPos(0.0), m_deltaPos(0.0) {};
    MoveData(double initialPos) : m_pos(initialPos), m_prevPos(initialPos), m_deltaPos(0.0)  {};
    ~MoveData() {};
    double m_pos;
    double m_prevPos;
    double m_deltaPos;
    //double m_prevVelocity;
    //double m_acceleration;
    
    void setPos(double pos) 
    { 
        m_prevPos = m_pos; 
        m_pos = pos; 
        //m_prevVelocity = m_velocity;
        m_deltaPos = (m_pos - m_prevPos);
        //m_acceleration = m_velocity - m_prevVelocity;
    };
    void reset(double initialPos ) 
    { 
        m_pos = initialPos; 
        m_prevPos = initialPos; 
        m_deltaPos = 0.0; 
        //m_prevVelocity = 0; 
        //m_acceleration = 0; 
    };


};
