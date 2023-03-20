#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"

#include "lib/NL/Simulation/VirtualRobot/NLVirtualGyro.h"

#define FLAG_NLVGYRO_IS_CLOCKWISE BIT_0 // sens anti-trigonometrique
#define FLAG_NLVGYRO_IS_DEGREE BIT_1	// la fonction get() retournera une valeur exprim�e en degr�s. ( l'unit� par d�faut est le radian )
#define MASK_NLVGYRO_ALL_FLAGS (BIT_0 | BIT_1)

class NLVIRTUAL_ROBOT; // NLVIRTUAL_ROBOT 'poss�de' NLROBOT comme membre ...
					   // ...	et la classe NLROBOT peut contenir un (ou plusieurs?)  NLVGYRO comme membre(s).
					   // Donc, pour �viter des "inclusions circulaires et/o� des "error C2079" ( undefined class ... )
					   // il convient ici de d�clarer la class NVIRTUAL_ROBOT en forward d�claration ( sans inclure NLVirtualRobot.h )
					   // ainsi le compilateur 'sait' que la class existe sans en connaitre encore le d�tail...
					   // ( NLVirtualRobot.h �tant bien s�r inclu dans NLVGyro.cpp )

class NLVGYRO
{
public:
	void Init(/*const Nu8 channelid,*/ const Nu32 flags, const NLVIRTUAL_ROBOT *pvrobot);
	void reset();
	Nf32 get();

#ifdef _NEDITOR
	void draw(const NVEC3 *po);
#endif

	Nu32 m_flags; // Divers Flags
	NLVIRTUAL_GYRO *m_pVirtualGyro;
};
