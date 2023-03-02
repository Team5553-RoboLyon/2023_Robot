#include "../../../N/NErrorHandling.h"
#include "../VirtualRobot/NLVirtualRobot.h"
#include "NLVGyro.h"
#ifdef _NEDITOR
#include "../../../N/Utilities/Draw/NUT_Draw.h"
#endif

void NLVGYRO::Init(/*const Nu8 channelid,*/ const Nu32 flags, const NLVIRTUAL_ROBOT *pvrobot)
{
	m_flags = flags & MASK_NLVGYRO_ALL_FLAGS;
	m_pVirtualGyro = (NLVIRTUAL_GYRO *)&pvrobot->m_gyro;
}

void NLVGYRO::reset()
{
	m_pVirtualGyro->reset();
}

Nf32 NLVGYRO::get()
{
	Nf32 ang = m_pVirtualGyro->getAngle();

	// inversion du sens trigo si besoin:
	if (ISFLAG_ON(m_flags, FLAG_NLVGYRO_IS_CLOCKWISE))
		ang = NF32_2PI - ang;
	// degr�s ?
	if (ISFLAG_ON(m_flags, FLAG_NLVGYRO_IS_DEGREE))
		ang = NRADtoDEG(ang);

	return ang;
}
#ifdef _NEDITOR
void NLVGYRO::draw(const NVEC3 *po)
{
	m_pVirtualGyro->draw(po);
}
#endif
