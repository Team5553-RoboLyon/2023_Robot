#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"


#include "../VirtualRobot/NLVirtualEncoder.h"
class NLVIRTUAL_ROBOT;	// NLVIRTUAL_ROBOT 'possède' NLROBOT comme membre ... 
						// ...	et la classe NLROBOT peut contenir un ou plusieurs  NLVENCODER comme membre(s).
						// Donc, pour éviter des "inclusions circulaires et/où des "error C2079" ( undefined class ... )
						// il convient ici de déclarer la class NVIRTUAL_ROBOT en forward déclaration ( sans inclure NLVirtualRobot.h ) 
						// ainsi le compilateur 'sait' que la class existe sans en connaitre encore le détail...
						// ( NLVirtualRobot.h étant bien sûr inclu dans NLVencoder.cpp ) 

typedef enum NLVENCODER_ENCODING_TYPE	NLVENCODER_ENCODING_TYPE;
enum NLVENCODER_ENCODING_TYPE
{
	NLVENCODER_ENCODING_K1X = 4, // car en k1x on divise la valeur m_count du virtual encodeur ( qui est en k4x )  par 4
	NLVENCODER_ENCODING_K2X = 2, // car en k2x on divise la valeur m_count du virtual encodeur ( qui est en k4x )  par 2
	NLVENCODER_ENCODING_K4X = 1, // car en k4x on divise la valeur m_count du virtual encodeur ( qui est en k4x )  par 1
};

class NLVENCODER
{
public:	
	void Init(const Nu8 channelid, const NLVENCODER_ENCODING_TYPE encoding_type, const NLVIRTUAL_ROBOT * pvrobot);
	void reset();
	//Ns32 get();
	Ns32 getRaw();

	Nf32					m_kx;
	NLVIRTUAL_ENCODER		*m_pVirtualEncoder;
};
