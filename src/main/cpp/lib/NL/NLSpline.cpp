#include "NLSpline.h"

NLBEZIERf32::NLBEZIERf32()
{
}

NLBEZIERf32::~NLBEZIERf32()
{
}

NL2f32 NLBEZIERf32::getPoint(const nlf32 t)
{
	nlf32	u	= 1.0f - t;
	nlf32	u2	= u	* u;
	nlf32	u3	= u2 * u;
	nlf32	t2	= t * t;
	nlf32	t3	= t2 * t;

	NL2f32 p;

	p.x =	m_p0.x * u3 +
			m_p1.x * (3.0f * u2 * t) +
			m_p2.x * (3.0f * u * t2) +
			m_p3.x * t3;

	p.y =	m_p0.y * u3 +
			m_p1.y * (3.0f * u2 * t) +
			m_p2.y * (3.0f * u * t2) +
			m_p3.y * t3;


	return p;
}

NLSPLINEf32::NLSPLINEf32()
{
}

NLSPLINEf32::~NLSPLINEf32()
{
}
