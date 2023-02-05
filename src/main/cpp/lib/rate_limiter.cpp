#include <lib/rate_limiter.h>


double RateLimiter::Update(double target) { // on update le current
   m_target = target; 

   if(m_current < m_target - m_speed) // si le current est plus petit que le target - la vitesse
   {
        m_current += m_speed; // on ajoute la vitesse au current
   }
   else if (m_current > m_target + m_speed) // si le current est plus grand que le target + la vitesse
   {
        m_current -= m_speed; // on enleve la vitesse au current
   }
   else
   {
        m_current = m_target; // sinon le current est egal au target
   }      
    return m_current; // on retourne le current
}

double RateLimiter::Update() {
   if(m_current < m_target - m_speed) // si le current est plus petit que le target - la vitesse
   {
        m_current += m_speed; // on ajoute la vitesse au current
   }
   else if (m_current > m_target + m_speed) // si le current est plus grand que le target + la vitesse
   {
        m_current -= m_speed; // on enleve la vitesse au current
   }
   else
   {
        m_current = m_target; // sinon le current est egal au target
   }      
    return m_current; // on retourne le current
}
void RateLimiter::SetTarget(double target) { // on set le target
  m_target = target; 
}

void RateLimiter::SetCurrent(double current) { // on set le current
  m_current = current;
}

void RateLimiter::SetSpeed (double speed) { // on set la vitesse
  m_speed = speed;
}

void RateLimiter::Reset(double target, double current, double speed) { // on reset tout
  m_target = target;
  m_current = current;
  m_speed = speed;
}

// void RateLimiter::SetRecul(double current, double speed_1, double speed_2) { // on set le recul
//   m_current = current;
//   m_recul = ((m_current - m_speedSwitch)/speed_2 )*speed_1; // on calcule le current du recul qui sera soustrait à une switch définie ?
// }