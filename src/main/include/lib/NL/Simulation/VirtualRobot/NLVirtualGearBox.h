#pragma once
#include "lib/N/NType.h"
#include "NLVirtualMotor.h"
#include "NLVirtualEncoder.h"
#include "NLVirtualOutPut.h"

#define CONSTANT_NLVIRTUAL_GEARBOX_MAX_NUMBER_OF_DRIVEN_INPUT		3		// min: 1 max: NU32MAX
																			// equivalant au nombre max de moteurs pr�sents dans la GearBox			
class NLVIRTUAL_GEARBOX
{
	friend class NLVIRTUAL_ROBOT;
	//NLVIRTUAL_GEARBOX() { Nmem0(this, NLVIRTUAL_GEARBOX); m_invGearRatio = 1.0f; }
	//NLVIRTUAL_GEARBOX(const Nf32 gear_ratio) { Nmem0(this, NLVIRTUAL_GEARBOX); if (gear_ratio) { m_invGearRatio = 1.0f / gear_ratio; } else { m_invGearRatio = 1.0f; } }

	inline void setGearBoxRatio(const Nf32 gear_ratio){ if (gear_ratio) { m_invGearRatio = 1.0f / gear_ratio; } else { m_invGearRatio = 1.0f; } }
	inline void clear() { Nmem0(this, NLVIRTUAL_GEARBOX); m_invGearRatio = 1.0f; }
	inline void reset() { m_output.reset();/*for (Nu8 i = 0; i < m_drivenInputCount; i++) { m_driveInput[i]->reset(); }*/ }
	void addDriveInput(const NLVIRTUAL_OUTPUT *pinput);
private:
	void update(const Nf32 ddt);

	NLVIRTUAL_OUTPUT	m_output;
	
	Nu8					m_flags;
	Nu8					m_drivenInputCount;
	Nu16				m_availableNu16;
	Nf32				m_invGearRatio;
	
	NLVIRTUAL_OUTPUT*	m_driveInput[CONSTANT_NLVIRTUAL_GEARBOX_MAX_NUMBER_OF_DRIVEN_INPUT];
};

/*
class NLVIRTUAL_GEARBOX
{
public:	
	void load(const Nchar *pfilename);

	inline void setVelocityAndAcceleration(const Nf32 velocity, const Nf32 accel)
	{
		
		// code generique
		//for(Nu32 i=0;i< m_characterization.m_numberOfMotors;i++)
		//	m_motor[i].setVoltage(m_characterization.m_motor[i].getVoltage(velocity, accel));

		m_motor[0].setVoltage(m_characterization.m_motor[0].getVoltage(velocity, accel) );
		m_motor[1].setVoltage(m_characterization.m_motor[1].getVoltage(velocity, accel) );
	}
	Nf32 update(const Nf32 dt);

	Nf32							m_w;

	NLVIRTUAL_MOTOR					m_motor[CONSTANT_MAX_NL_GEARBOX_NUMBER_OF_MOTORS];
	NLVIRTUAL_ENCODER				m_encoder;

	NL_GEARBOX_CHARACTERIZATION		m_characterization;
};
*/