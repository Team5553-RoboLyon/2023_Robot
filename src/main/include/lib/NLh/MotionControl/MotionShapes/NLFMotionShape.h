#pragma once
#include "lib/N/NType.h"
#include "lib/N/NMemory.h"
#include "lib/N/NErrorHandling.h"
#include "lib/NL/MotionControl/NLKin.h"

#ifdef _NEDITOR
#include "lib/NL/MotionControl/NL2DOrthogonalCoordinateSystem.h"
#endif

// FLAT MOTION SHAPE ( = vitesse constante sur une periode de temps dt )
typedef struct NLFMOTIONSHAPE NLFMOTIONSHAPE;
struct NLFMOTIONSHAPE
{
	Nf32 m_s0; // Abscisse initiale de la FShape
	Nf32 m_ds; // distance couverte par la FShape
	Nf32 m_dt; // Dur�e de maintient de la vitesse.

	inline void null() { Nmem0(this, NLFMOTIONSHAPE); }
	inline void forwardVt(const Nf32 s0, const Nf32 v, const Nf32 dt)
	{
		NErrorIf(dt < 0.0f, NERROR_INCONSISTENT_VALUES);
		m_s0 = s0;
		m_dt = dt;
		m_ds = v * dt;
	}
	inline void forwardVs(const Nf32 s0, const Nf32 v, const Nf32 ds)
	{
		NErrorIf(v == 0.0f, NERROR_INCONSISTENT_VALUES);
		m_s0 = s0;
		m_ds = ds;
		m_dt = ds / v;
	}
	inline void forwardSt(const Nf32 s0, const Nf32 ds, const Nf32 dt)
	{
		NErrorIf(dt <= 0.0f, NERROR_INCONSISTENT_VALUES);
		m_s0 = s0;
		m_ds = ds;
		m_dt = dt;
	}

	inline void backwardVt(const Nf32 s1, const Nf32 v, const Nf32 dt)
	{
		NErrorIf(dt < 0.0f, NERROR_INCONSISTENT_VALUES);
		m_dt = dt;
		m_ds = v * dt;
		m_s0 = s1 - m_ds;
	}
	inline void backwardVs(const Nf32 s1, const Nf32 v, const Nf32 ds)
	{
		NErrorIf(v == 0.0f, NERROR_INCONSISTENT_VALUES);
		m_ds = ds;
		m_dt = ds / v;
		m_s0 = s1 - m_ds;
	}
	inline void backwardSt(const Nf32 s1, const Nf32 ds, const Nf32 dt)
	{
		NErrorIf(dt <= 0.0f, NERROR_INCONSISTENT_VALUES);
		m_ds = ds;
		m_dt = dt;
		m_s0 = s1 - m_ds;
	}

	Nu32 promote(NLKIN *pkbuff);
	Nu32 promote(NLKIN *pkbuff, const Nf32 v);

	void drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);
	void drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 v);
};

typedef struct NLFMOTIONSHAPEold NLFMOTIONSHAPEold;
struct NLFMOTIONSHAPEold
{
	Nf32 m_v;	  // Vitesse (constante)
	Nf32 m_ds0s1; // Distance totale couverte pas la F Motion Shape
	Nf32 m_dt0t1; // Dur�e totale couverte pas la F Motion Shape

	inline void null() { Nmem0(this, NLFMOTIONSHAPE); }
	inline void buildFromVelocityAndTime(const Nf32 v, const Nf32 t)
	{
		NErrorIf(t < 0.0f, NERROR_INCONSISTENT_VALUES);
		m_v = v;
		m_dt0t1 = t;
		m_ds0s1 = v * t;
	}
	inline void buildFromVelocityAndDist(const Nf32 v, const Nf32 s)
	{
		NErrorIf(v <= 0.0f, NERROR_INCONSISTENT_VALUES);
		m_v = v;
		m_ds0s1 = s;
		m_dt0t1 = s / v;
	}
	inline void buildFromTimeAndDist(const Nf32 t, const Nf32 s)
	{
		NErrorIf(t <= 0.0f, NERROR_INCONSISTENT_VALUES);
		m_v = s / t;
		m_ds0s1 = s;
		m_dt0t1 = t;
	}

#ifdef _NEDITOR
	void drawSv(NL2DOCS *p2docs, const Nf32 s0, const NCOLOR *pcol);
#endif
	inline Nu32 extendIntoKinBuffer(NLKIN *pkbuff)
	{
		NErrorIf((pkbuff->m_a) || (pkbuff->m_v != m_v), NERROR_INCONSISTENT_VALUES);
		NLKIN *pk = pkbuff + 1;
		pk->m_j = 0.0f;
		pk->m_a = 0.0f;
		pk->m_v = m_v;
		pk->m_s = pkbuff->m_s + m_ds0s1;
		pk->m_t = pkbuff->m_t + m_dt0t1;
		return 1;
	}
};
