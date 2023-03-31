#pragma once

#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"

#ifdef _NEDITOR
#endif

typedef struct NLMOTOR_CHARACTERIZATION NLMOTOR_CHARACTERIZATION;
struct NLMOTOR_CHARACTERIZATION
{
	NLMOTOR_CHARACTERIZATION() { Nmem0(this, NLMOTOR_CHARACTERIZATION); }

	Nchar *read(Nchar *pstr);
	inline void setFrom(const NLMOTOR_CHARACTERIZATION *psrc) { *this = *psrc; };
	void setFrom(const NLMOTOR_CHARACTERIZATION *psrc, const Nf32 scalefactor);
	inline void setForwardConst(const Nf32 kv, const Nf32 ka, const Nf32 intercept)
	{
		m_forwardKv = kv;
		m_forwardKa = ka;
		m_forwardIntercept = intercept;
	}
	inline void setBackwardConst(const Nf32 kv, const Nf32 ka, const Nf32 intercept)
	{
		m_backwardKv = kv;
		m_backwardKa = ka;
		m_backwardIntercept = intercept;
	}

	inline Nf32 getVoltage(const Nf32 velocity, const Nf32 acceleration) { return (velocity < 0.0f) ? (m_backwardKv * velocity + m_backwardKa * acceleration + m_backwardIntercept) : (m_forwardKv * velocity + m_forwardKa * acceleration + m_forwardIntercept); }

	union
	{
		struct
		{
			Nf32 m_kv[2];		 // m_kv[0]			= "kv pour un voltage POSITIF" // m_kv[1] = "kv pour un voltage NEGATIF"
			Nf32 m_ka[2];		 // m_ka[0]			= "ka pour un voltage POSITIF" // m_ka[1] = "ka pour un voltage NEGATIF"
			Nf32 m_intercept[2]; // m_intercept[0]	= "intercept pour un voltage POSITIF" // m_intercept[1] = "intercept pour un voltage NEGATIF"
		};
		struct
		{
			Nf32 m_forwardKv;		  // = m_kv[0]
			Nf32 m_backwardKv;		  // = m_kv[1]
			Nf32 m_forwardKa;		  // = m_ka[0]
			Nf32 m_backwardKa;		  // = m_ka[1]
			Nf32 m_forwardIntercept;  // = m_intercept[0]
			Nf32 m_backwardIntercept; // = m_intercept[1]
		};
	};
};
