#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"

#include "../VirtualRobot/NLVirtualGyro.h"

#define FLAG_NLVGYRO_IS_CLOCKWISE				BIT_0 // sens anti-trigonometrique
#define FLAG_NLVGYRO_IS_DEGREE					BIT_1 // la fonction get() retournera une valeur exprimée en degrés. ( l'unité par défaut est le radian )
#define MASK_NLVGYRO_ALL_FLAGS					(BIT_0|BIT_1) 


class NLVIRTUAL_ROBOT;	// NLVIRTUAL_ROBOT 'possède' NLROBOT comme membre ... 
						// ...	et la classe NLROBOT peut contenir un (ou plusieurs?)  NLVGYRO comme membre(s).
						// Donc, pour éviter des "inclusions circulaires et/où des "error C2079" ( undefined class ... )
						// il convient ici de déclarer la class NVIRTUAL_ROBOT en forward déclaration ( sans inclure NLVirtualRobot.h ) 
						// ainsi le compilateur 'sait' que la class existe sans en connaitre encore le détail...
						// ( NLVirtualRobot.h étant bien sûr inclu dans NLVGyro.cpp ) 


class NLVGYRO
{
public:	
	void Init(/*const Nu8 channelid,*/ const Nu32 flags, const NLVIRTUAL_ROBOT* pvrobot);
	void reset();
	Nf32 get();

	Nu32				m_flags;			//Divers Flags
	NLVIRTUAL_GYRO		*m_pVirtualGyro;	
};
