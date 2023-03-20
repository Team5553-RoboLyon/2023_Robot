#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NMemory.h"
#include "lib/NL/MotionControl/NLKin.h"
#include "lib/NL/MotionControl/NLKinTweak.h"
#include "lib/NL/MotionControl/NLKinLimits.h"

#include "lib/NL/MotionControl/Path/Geometry/NLPathPoint.h"
#include "NLSovershoot.h"

#define MASK_NLVSTAGEX_TYPE_AND_ACCESSIBILITY (BIT_0 | BIT_1) // type: Floor ( plancher ) ou Ceil ( plafond ) et Accessibility: Accessible ou pas.
#define FLAG_NLVSTAGEX_IS_CEIL BIT_0
#define FLAG_NLVSTAGEX_INACCESSIBLE BIT_1

#define FLAG_NLVSTAGEX_AXIN_MAX BIT_2
#define FLAG_NLVSTAGEX_JXIN_MAX BIT_3

#define FLAG_NLVSTAGEX_AXOUT_MAX BIT_4
#define FLAG_NLVSTAGEX_JXOUT_MAX BIT_5

#define MASK_NLVSTAGEX_IN_MAX (BIT_2 | BIT_3)
#define MASK_NLVSTAGEX_OUT_MAX (BIT_4 | BIT_5)

typedef struct NLVSTAGE NLVSTAGE; // forward declaration ( NLVStage.h sera d�clar� dans NLVstage.cpp)
typedef struct NLVSTAGEX NLVSTAGEX;
struct NLVSTAGEX
{
	Nf32 m_v;
	Nf32 m_s0;
	Nf32 m_s1;

	Nu32 m_flags;

	NLSSTITCH m_stitch; // les delta de temps d'abscisse et de vitesse � ajouter au vstage pour partir/arriver lors d'un overshoot

	Nf32 m_s0Prime; // "distance � parcourir" � la vitesse plafonn�e par le VStage.
	Nf32 m_s1Prime; // Initialement cette distance vaut "m_s1 - m_s0" mais,
					//	En cas de plafonnement � gauche elle vaut "m_s1 - s_left_out"
					//	En cas de plafonnement � droite elle vaut 0
					//  En cas d'overshoot � droite ou � gauche elle vaut 0
	NLKIN m_kinXIn;
	NLKIN m_kinXOut;

	// Nf32			m_axIn;					// Acceleration (<0) conseill�e en entr�e de VStage ( depuis a = 0 ), calcul�e afin de ne pas "d�passer la limite de vitesse"
	//  fix�e par la pente V(s) de la clothoide entrante et "descendante".
	// Nf32			m_axOut;				// Acceleration conseill�e en sortie de VStage ( depuis a = 0 ), calcul�e afin de ne pas "d�passer la limite de vitesse"
	//  fix�e par la pente V(s) de la clothoide sortante et "montante".
	//	Nf32			m_jxIn;
	//	Nf32			m_jxOut;

	/*
	inline		void setPrimeFlatShapeOff() {FLAG_OFF(m_flags, FLAG_NLVSTAGE_PRIME_FLAT_SHAPE_IS_ON);}
	inline		void setPrimeFlatShapeOn() { FLAG_ON(m_flags, FLAG_NLVSTAGE_PRIME_FLAT_SHAPE_IS_ON);}
	inline		void setPrimeFlatShapeS0(const Nf32 s0) { NErrorIf(m_s1Prime - s0 < 0.0f, NERROR_SYSTEM_GURU_MEDITATION); m_s0Prime = s0;FLAG_ON(m_flags, FLAG_NLVSTAGE_PRIME_FLAT_SHAPE_IS_ON); }
	inline		void setPrimeFlatShapeS1(const Nf32 s1) { NErrorIf(s1 - m_s0Prime < 0.0f, NERROR_SYSTEM_GURU_MEDITATION); m_s1Prime = s1;FLAG_ON(m_flags, FLAG_NLVSTAGE_PRIME_FLAT_SHAPE_IS_ON); }
	*/
	inline void nullPrimeFlatShape() { m_s0Prime = m_s1Prime = 0.0f; }
	inline Nf32 getPrimeFlatShapeTimeSpan()
	{
		Nf32 s = m_s1Prime - m_s0Prime;
		if ((!s) || (!m_v))
			return 0.0f;
		else
			return s / m_v;
	};
	inline void setPrimeFlatShapeS0(const Nf32 s0)
	{ /*NErrorIf(m_s1Prime - s0 < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);*/
		m_s0Prime = s0;
	}
	inline void setPrimeFlatShapeS1(const Nf32 s1)
	{ /*NErrorIf(s1 - m_s0Prime < 0.0f, NERROR_SYSTEM_GURU_MEDITATION); */
		m_s1Prime = s1;
	}

	inline Nu32 isAccessible() { return ISFLAG_OFF(m_flags, FLAG_NLVSTAGEX_INACCESSIBLE); }
	inline Nu32 isAccessibleFloor() { return FLAGS_TEST(m_flags, MASK_NLVSTAGEX_TYPE_AND_ACCESSIBILITY, 0); }
	inline Nu32 isInaccessible() { return ISFLAG_ON(m_flags, FLAG_NLVSTAGEX_INACCESSIBLE); }
	inline Nu32 isInaccessibleFloor() { return FLAGS_TEST(m_flags, MASK_NLVSTAGEX_TYPE_AND_ACCESSIBILITY, FLAG_NLVSTAGEX_INACCESSIBLE); }

	inline void null() { Nmem0(this, NLVSTAGEX); };

	void set(const NLVSTAGE *ptrim);
	void set(const Nf32 s0, const Nf32 s1, const Nf32 vlimit, const Nbool bisceil);
	// void			 set(const NLPATH_POINT *pkp, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity);
	void set(const Nf32 s0, const Nf32 s1, const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity);
	void setButKinX(const Nf32 s0, const Nf32 s1, const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity);
	void setKinXOut(NLVSTAGEX *pvs_nxt, const NLKINTWEAK *pktwk, const NLKINLIMITS *pklim);

private:
	void compute_v(const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity);
};
#define NLClearVStageXInArrayCallBack NULL
/// inline void NLclearVStageXInArrayCallBack(void* p)