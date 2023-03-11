#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/NL/SimulatioN/VirtualRobot/NLVirtualEncoder.h"



void NLVIRTUAL_ENCODER::update(const Nf32 dt)
{
	// pour une rotation donn�e exprim�e en radian on a :
	//			m_count = (rotation * 4 * ppr )/ (2*PI) soit (rotation * 2 * ppr )/ PI
	//			avec
	//					rotation / (2*PI) le nombre de revolution effectu�s ( par exemple 3.2 'tours' )
	//					4*ppr le nombre de 'count' pour une revolution compl�te
	//
	// Et donc on a, dans 'l'autre sens'			
	//					rotation = (m_count * 2 * PI)/(4 * ppr) soit (m_count * PI)/(2 * ppr)
	m_count		 = ( (m_pDriveInput->m_angularPosition - m_input0.m_angularPosition) * 2.0f * m_pulsePerRevolution)/ NF32_PI;
}
