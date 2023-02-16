#include "../../../N/NMemory.h"
#include "../../../N/NMath.h"
#include "../../../N/NErrorHandling.h"
#include "NLVirtualMotor.h"

/*
Nf32 NLVIRTUAL_MOTOR::getVirtualVelocity(const Nf32 velocity, const Nf32 accel, const Nf32 dt)
{
	return velocity + accel*dt + NFastCos((Nu32)(NFastRand()*NANGLEMAX))*m_error;
}
*/
void NLVIRTUAL_MOTOR::update(const Nf32 dt)
{

	Nf32 uv, ua,a;
	// Le moteur va "réagir" en fonction de sa vitesse courante:
	// Rappel de la fonction de characterization:
	//		U = kv*v + ka*a + intercept
	//
	if (m_output.m_angularVelocity < 0.0f)
	{
		// voltage nécéssaire pour maintenir la vitesse courante:
		uv = m_output.m_angularVelocity * m_characterization.m_backwardKv + m_characterization.m_backwardIntercept;
		// la différence avec le voltage entrant va engendrer une variation de vitesse ( acceleration )
		ua = m_appliedVoltage - uv;
		a = ua/ m_characterization.m_backwardKa;
		

		m_output.m_angularPosition		+= m_output.m_angularVelocity * dt + (m_output.m_angularAcceleration*dt*dt)/2.0f;
		m_output.m_angularVelocity		+= a * dt;
		m_output.m_angularAcceleration = a;
	}
	else
	{
		// voltage nécéssaire pour maintenir la vitesse courante:
		uv = m_output.m_angularVelocity * m_characterization.m_forwardKv + m_characterization.m_forwardIntercept;
		// la différence avec le volatage entrant va engendrer une variation de vitesse ( acceleration )
		ua = m_appliedVoltage - uv;
		a = ua / m_characterization.m_forwardKa;

		m_output.m_angularPosition += m_output.m_angularVelocity * dt + (m_output.m_angularAcceleration*dt*dt) / 2.0f;
		m_output.m_angularVelocity += a * dt;
		m_output.m_angularAcceleration = a;
	}
}
