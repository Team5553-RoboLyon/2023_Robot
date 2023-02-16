#pragma once
#include "../../../N/NType.h"
#include "NLVirtualOutPut.h"

// https://www.cuidevices.com/blog/what-is-encoder-ppr-cpr-and-lpr


class NLVIRTUAL_ENCODER
{
	friend class NLVIRTUAL_ROBOT;
	friend class NLVENCODER;

	void SetDriveInput(const NLVIRTUAL_OUTPUT *pinput) { m_pDriveInput = (NLVIRTUAL_OUTPUT*)pinput; }

private:
	void update(const Nf32 dt);// { m_revolution += omega; m_count = (Ns32)(m_revolution*m_pulse); }

	inline void Clear() { Nmem0(this, NLVIRTUAL_ENCODER);  }

	inline void setup(const Nu32 ppr) { m_pulsePerRevolution = ppr;  }
	inline void reset() { m_count = 0; m_input0 = *m_pDriveInput; }
	inline Ns32 getK4xCount() { return (Ns32)floor(m_count); }
	inline Nf32 getRevolution() { return (floor(m_count) * NF32_PI) / (2.0f * m_pulsePerRevolution); }


	Nu32				m_pulsePerRevolution;	// resolution de l'encodeur ( cpr = ppr * 4 ) k1x = ppr / k2x = ppr*2 / k4x = ppr * 4 
	Nf32				m_count;				// nombre de 'ticks' actuels
	NLVIRTUAL_OUTPUT	m_input0;				// 'snapshot' enregistré au reset de l'output tracké par l'encodeur
	NLVIRTUAL_OUTPUT	*m_pDriveInput;
};
