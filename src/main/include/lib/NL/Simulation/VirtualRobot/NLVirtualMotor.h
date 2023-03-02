#pragma once
#include "lib/N/NType.h"
#include "../../MotionControl/DriveTrain/Characterization/NLMotorCharacterization.h"
#include "NLVirtualOutPut.h"

//										nominal_voltage		free_speed		free_current		stall_current		stall_torque
#define PRESET_NLMOTOR_SPECS_NEO		12.0f,				5676.0f,		1.8f,				105.0f,				2.6f
#define PRESET_NLMOTOR_SPECS_NEO550		12.0f,				11000.0f,		1.4f,				100.0f,				0.97f
#define PRESET_NLMOTOR_SPECS_775PRO		12.0f,				18730.0f,		0.7f,				134.0f,				0.71f

typedef struct NLMOTOR_SPECS	NLMOTOR_SPECS;
struct NLMOTOR_SPECS
{
	inline void	Set(const Nf32 nominal_voltage, const Nf32 free_speed, const Nf32 free_current, const Nf32 stall_current, const Nf32 stall_torque)
	{
		m_nominalVoltage	= nominal_voltage;
		m_freeSpeed			= free_speed;
		m_freeCurrent		= free_current;
		m_stallTorque		= stall_current;
		m_stallCurrent		= stall_torque;
	}

	Nf32	m_nominalVoltage;
	Nf32	m_freeSpeed;
	Nf32	m_freeCurrent;
	Nf32	m_stallTorque;
	Nf32	m_stallCurrent;
};

class NLVIRTUAL_MOTOR
{
	friend class NLVIRTUAL_ROBOT;
	friend class NLVMOTOR_CONTROLLER;

private:
	NLVIRTUAL_OUTPUT			m_output;

	inline void SetSpecs(const Nf32 nominal_voltage, const Nf32 free_speed, const Nf32 free_current, const Nf32 stall_current, const Nf32 stall_torque){m_specs.Set(nominal_voltage, free_speed, free_current, stall_current, stall_torque);}
	inline void SetInternalCharacterization(const NLMOTOR_CHARACTERIZATION *pmc) {m_characterization.setFrom(pmc);}
	inline void SetInternalCharacterization(const NLMOTOR_CHARACTERIZATION *pmc, const Nf32 scale_factor) { m_characterization.setFrom(pmc, scale_factor); }

	inline void Clear() { Nmem0(this, NLVIRTUAL_MOTOR); }
	inline void setVoltage(const Nf32 u) { m_appliedVoltage = u; }

	inline Nf32 getVelocity() { return m_output.m_angularVelocity; }

	inline void reset() { m_output.reset(); }

	void update(const Nf32 dt);



	NLMOTOR_CHARACTERIZATION	m_characterization;
	NLMOTOR_SPECS				m_specs;

	Nf32						m_appliedVoltage;					// Current Voltage 
	//Nf32						m_currentVelocity;					// pour le moment en vitesse lineaire de sortie de boite !

	//Nf32						m_rps;
	//Nf32						m_error;

};
