#pragma once
#include "lib/N/NType.h"
#include "lib/N/NMemory.h"

typedef struct NLVIRTUAL_OUTPUT	NLVIRTUAL_OUTPUT;
struct NLVIRTUAL_OUTPUT
{
	void reset() { Nmem0(this, NLVIRTUAL_OUTPUT); }
	Nf32	m_angularPosition;			// rad
	Nf32	m_angularVelocity;			// rad/sec
	Nf32	m_angularAcceleration;		// rad/sec�
};
