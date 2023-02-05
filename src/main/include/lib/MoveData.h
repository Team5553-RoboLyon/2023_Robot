#pragma once

typedef struct MoveData MoveData;
struct MoveData
{

    MoveData() : m_pos(0), m_prevPos(0), m_velocity(0), m_prevVelocity(0), m_acceleration(0) {};
    MoveData(double initialPos) : m_pos(initialPos), m_prevPos(initialPos), m_velocity(0), m_prevVelocity(0), m_acceleration(0) {};
    ~MoveData() {};
    double m_pos;
    double m_prevPos;
    double m_velocity;
    double m_prevVelocity;
    double m_acceleration;
    
    void setPos(double pos) 
    { 
        m_prevPos = m_pos; 
        m_pos = pos; 
        m_prevVelocity = m_velocity;
        m_velocity = (m_pos - m_prevPos)/0.02*60;
        m_acceleration = m_velocity - m_prevVelocity;
    };
    void reset(double initialPos ) 
    { 
        m_pos = initialPos; 
        m_prevPos = initialPos; 
        m_velocity = 0; 
        m_prevVelocity = 0; 
        m_acceleration = 0; 
    };


};
