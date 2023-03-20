#include "lib/N/NErrorHandling.h"
#include "lib/NL/Simulation/VirtualRobot/NLVirtualRobot.h"
#include "lib/NL/Simulation/VControllers/NLVEncoder.h"

void NLVENCODER::Init(const Nu8 channelid, const NLVENCODER_ENCODING_TYPE encoding_type, const NLVIRTUAL_ROBOT *pvrobot)
{
	if (BITGET(pvrobot->m_isEncoderActive, channelid))
	{
		m_pVirtualEncoder = &(((NLVIRTUAL_ROBOT *)pvrobot)->m_encoder[channelid]);
		m_kx = 1.0f / (Nf32)encoding_type;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1, NERROR_INCONSISTENT_REQUEST); // moteur non connect�
	}
#endif
}

void NLVENCODER::reset()
{
	m_pVirtualEncoder->reset();
}

Ns32 NLVENCODER::getRaw()
{
	return (Ns32)(m_pVirtualEncoder->getK4xCount() * m_kx);
}
