#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/File/NFile.h"
#include "lib/N/NString.h"
#include "lib/N/NErrorHandling.h"
#include "lib/NL/Simulation/VirtualRobot/NLVirtualRobot.h"
#include "lib/NL/Simulation/VControllers/NLVMotorController.h"

/*
NLVMOTOR_CONTROLLER::NLVMOTOR_CONTROLLER()
{
	Nmem0(this, NLVMOTOR_CONTROLLER);
}

NLVMOTOR_CONTROLLER::NLVMOTOR_CONTROLLER(Nu8 channel_id, NLVIRTUAL_ROBOT * pvrobot)
{
	Nmem0(this, NLVMOTOR_CONTROLLER);
	m_pVirtualMotor = &pvrobot->m_motor[channel_id];
}
*/
void NLVMOTOR_CONTROLLER::Init(const Nu8 channelid, const NLVIRTUAL_ROBOT *pvrobot)
{
	if (BITGET(pvrobot->m_isMotorActive, channelid))
	{
		m_pVirtualMotor = &(((NLVIRTUAL_ROBOT *)pvrobot)->m_motor[channelid]);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1, NERROR_INCONSISTENT_REQUEST); // moteur non connect�
	}
#endif
}
void NLVMOTOR_CONTROLLER::SetVoltage(const Nf32 signed_voltage)
{
	m_pVirtualMotor->m_appliedVoltage = signed_voltage;
}

void NLVMOTOR_CONTROLLER::SetVelocityAndAcceleration(const Nf32 velocity, const Nf32 acceleration)
{
	m_pVirtualMotor->m_output.m_angularVelocity = velocity;
}

void NLVMOTOR_CONTROLLER::Set(const Nf32 signed_speed)
{
	m_pVirtualMotor->m_appliedVoltage = signed_speed * GetBusVoltage();
}

void NLVMOTOR_CONTROLLER::SetInverted(Nbool is_inverted)
{
	if (is_inverted)
		FLAG_ON(m_flags, FLAG_NLVMOTOR_CONTROLLER_IS_INVERTED);
	else
		FLAG_OFF(m_flags, FLAG_NLVMOTOR_CONTROLLER_IS_INVERTED);
}

Nbool NLVMOTOR_CONTROLLER::GetInverted()
{
	return ISFLAG_ON(m_flags, FLAG_NLVMOTOR_CONTROLLER_IS_INVERTED) ? NTRUE : NFALSE;
}

Nf32 NLVMOTOR_CONTROLLER::GetBusVoltage()
{
	return 12.0f;
}

Nf32 NLVMOTOR_CONTROLLER::GetAppliedOutput()
{
	return 0.0f;
}

Nf32 NLVMOTOR_CONTROLLER::GetOutputCurrent()
{
	return 0.0f;
}
