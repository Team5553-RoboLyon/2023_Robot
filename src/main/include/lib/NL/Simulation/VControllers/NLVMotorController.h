#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"


#include "../VirtualRobot/NLVirtualMotor.h"


#define FLAG_NLVMOTOR_CONTROLLER_IS_INVERTED		BIT_0
class NLVIRTUAL_ROBOT;	// NLVIRTUAL_ROBOT 'possède' NLROBOT comme membre ... 
						// ...	et la classe NLROBOT peut contenir un ou plusieurs  NLVMOTOR_CONTROLLER comme membre(s).
						// Donc, pour éviter des "inclusions circulaires et/où des "error C2079" ( undefined class ... )
						// il convient ici de déclarer la class NVIRTUAL_ROBOT en forward déclaration ( sans inclure NLVirtualRobot.h ) 
						// ainsi le compilateur 'sait' que la class existe sans en connaitre encore le détail...
						// ( NLVirtualRobot.h étant bien sûr inclu dans NLVMotorController.cpp ) 

class NLVMOTOR_CONTROLLER
{
public:
	//NLVMOTOR_CONTROLLER();
	//NLVMOTOR_CONTROLLER(Nu8 channel_id, NLVIRTUAL_ROBOT *pvrobot);
	void	Init(const Nu8 channelid, const NLVIRTUAL_ROBOT *pvrobot);
	void	SetVoltage(const Nf32 signed_voltage);
	void	SetVelocityAndAcceleration(const Nf32 velocity, const Nf32 acceleration);

	void	Set(const Nf32 signed_speed);
	void	SetInverted(Nbool is_inverted);
	Nbool	GetInverted();
	Nf32	GetBusVoltage();
	Nf32	GetAppliedOutput();
	Nf32	GetOutputCurrent();
	//Nf32 GetMotorTemperature();



private:
	Nu32				m_flags;

	NLVIRTUAL_MOTOR		*m_pVirtualMotor;
	
	//NLVIRTUAL_ROBOT		*m_pVirtualRobot;
};
