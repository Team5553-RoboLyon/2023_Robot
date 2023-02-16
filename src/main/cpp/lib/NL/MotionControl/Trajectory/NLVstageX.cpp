#include "../../../N/NErrorHandling.h"
#include "../../../N/Core/NLimits.h"
#include "../../../N/NMemory.h"
#include "NLVstageX.h"
#include "NLVstage.h"

void NLVSTAGEX::compute_v(const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	Nf32 r, sqr;
	// k negatif signifie qu'on tourne à droite.
	if (k < 0.0f)
	{
		r = -1.0f / k;
		m_v = max_cruise_velocity * (1.0f + (r - half_axle_track) / (r + half_axle_track))*0.5f;
		sqr = sqrt(turn_right_inertia_coef / (-k));

		m_v = NMIN(m_v, sqr);
		FLAG_OFF(m_flags, FLAG_NLVSTAGEX_IS_CEIL);
	}
	// k positif signifie qu'on tourne à gauche.
	else if (k > 0.0f)
	{
		r = 1.0f / k;
		m_v = max_cruise_velocity * (1.0f + (r - half_axle_track) / (r + half_axle_track))*0.5f;
		sqr = sqrt(turn_left_inertia_coef / k);

		m_v = NMIN(m_v, sqr);
		FLAG_OFF(m_flags, FLAG_NLVSTAGEX_IS_CEIL);
	}
	// k null, siginifie qu'on va "tout droit"
	else
	{
		m_v = max_cruise_velocity;
		FLAG_ON(m_flags, FLAG_NLVSTAGEX_IS_CEIL);
	}
}

void NLVSTAGEX::set(const NLVSTAGE* pvsdesc)
{
	m_s0		= pvsdesc->m_s0;
	m_s1		= pvsdesc->m_s1;

	m_s0Prime	= m_s0;
	m_s1Prime	= m_s1;

	m_flags		= 0;
	m_v			= pvsdesc->m_v;
	
	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);
	m_stitch.null();
}

void NLVSTAGEX::set(const Nf32 s0, const Nf32 s1, const Nf32 vlimit, const Nbool bisceil)
{
	NErrorIf(s1 < s0, NERROR_INCONSISTENT_VALUES);			//s1 doit être plus grand que s0
	NErrorIf(vlimit < 0.0f , NERROR_INCONSISTENT_VALUES);	//vlimit doit être positif
	m_v			= vlimit;

	m_s0		= s0;
	m_s1		= s1;

	m_s0Prime	= s0;
	m_s1Prime	= s1;

	m_flags		= 0;

	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f,0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX,  NF32_MAX, 0.0f, 0.0f);
	m_stitch.null();

	if (bisceil)
		FLAG_ON(m_flags, FLAG_NLVSTAGEX_IS_CEIL);

}

/*
void NLVSTAGEX::set(const NLPATH_POINT *pkp, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	m_s0		= m_s1 = pkp->s;

	m_s0Prime	= m_s0;
	m_s1Prime	= m_s1;
	
	m_flags		= 0;

	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);

//	m_axIn		=  -NF32_MAX;
//	m_axOut		=   NF32_MAX;
//	m_jxIn		=  -NF32_MAX;
//	m_jxOut		=	NF32_MAX;

	m_stitch.null();

	compute_v(half_axle_track,turn_left_inertia_coef,turn_right_inertia_coef,max_cruise_velocity);
}
*/
void NLVSTAGEX::set(const Nf32 s0, const Nf32 s1, const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	m_s0 = s0;
	m_s1 = s1;

	m_s0Prime = m_s0;
	m_s1Prime = m_s1;

	m_flags = 0;

	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);
	m_stitch.null();
	compute_v(k,half_axle_track, turn_left_inertia_coef, turn_right_inertia_coef, max_cruise_velocity);
}
void NLVSTAGEX::setButKinX(const Nf32 s0, const Nf32 s1, const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	m_s0 = s0;
	m_s1 = s1;

	m_s0Prime = m_s0;
	m_s1Prime = m_s1;

	m_flags = 0;

	// !!! bien fondé de cette fonction !!! NE PAS MODIFIER LES VALEURS m_axIn et m_axOut
	//m_axIn = NF32_MAX;
	//m_axOut = NF32_MAX;
	//m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	//m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);

	m_stitch.null();
	compute_v(k,half_axle_track, turn_left_inertia_coef, turn_right_inertia_coef, max_cruise_velocity);
}

void NLVSTAGEX::setKinXOut(NLVSTAGEX* pvs_nxt, const NLKINTWEAK* pktwk, const NLKINLIMITS* pklim)
{
	Nf32 f;
	Nf32 d = pvs_nxt->m_s0 - m_s1;

	if (m_v > pvs_nxt->m_v) // signifie v0 > v1
	{
		// "f" represente l'acceleration locale max dans un modele trapezoidal. 
		// Demonstration:
		// posons:	d	distance parcourue
		//			v0	vitesse au départ
		//			v1	vitesse à l'arrivée
		//			t	durée écoulée entre le départ et l'arrivée
		//			a	accélération (constante, le modèle est trapézoidal) permettant de passer de v0 à v1
		//
		//			on connait d,v0 et v1. On ne connait ni t, ni a
		//			on cherche à trouver a
		//
		//			Dans un modèle trapezoidal on peut écrire:
		//			
		//					d	= v0*t + (v1-v0)*t/2
		//						= v0*t + v1*t/2 -v0*t/2
		//						= (v0 + v1)*t/2					[1]
		//
		//					v1	= v0 + a*t
		//					t	= (v1 - v0 ) / a				[2]
		//
		//			En remplacant t de [1] par t de [2]
		//					d	= (v0 + v1)*(v1 - v0 ) / (2*a)
		//					a	= (v1 + v0)*(v1 - v0 ) / (2*d)
		//
		//					a	= (v1² - v0²)/(2*d)				[3]
		// 
		// Concrêtement la longueur de la clothoide correspond à d, v0 à la vitesse du VSTAGE précédent, et v1 à celle du VSTAGE courant:
		//
		f = (d > 0.0) ? (NPOW2(pvs_nxt->m_v)- NPOW2(m_v)) / (2.0f * d) : -pklim->m_a;
		//
		// si f est plus petite ( en val.absolue) que le max alors f devient le nouveau max possible. on utilise pktweak pour permettre à l'utilisateur de moduler et
		// ainsi choisir une accel. max comprise entre f et le max possible autorisé par les specs de la drive train.

		m_kinXOut.m_a = (f > -pklim->m_a) ? -pklim->m_a + pktwk->m_aHoldLocalLimitRatio * (f + pklim->m_a) : -pklim->m_a;
		m_kinXOut.m_j = -(pklim->m_j + pktwk->m_jHoldLocalLimitRatio * pklim->m_j * (-m_kinXOut.m_a / pklim->m_a - 1.0f));
		//NErrorIf(NSIGN(m_kinXOut.m_j) >= 0, NERROR_SYSTEM_GURU_MEDITATION);
		if ((m_kinXOut.m_a == -pklim->m_a) && (m_kinXOut.m_j == -pklim->m_j))
		{
			m_kinXOut.m_t =  pklim->m_tx;
			m_kinXOut.m_v = -pklim->m_vx;
			m_kinXOut.m_s = -pklim->m_sx;
		}
		else
		{
			m_kinXOut.m_t = m_kinXOut.m_a / m_kinXOut.m_j;
			m_kinXOut.m_v = NPOW2(m_kinXOut.m_a) / (2.0f * m_kinXOut.m_j);
			m_kinXOut.m_s = m_kinXOut.m_j * NPOW3(m_kinXOut.m_t) / 6.0f;
		}
	}
	else // (m_v <= pvs_nxt->m_v) // signifie v0 <= v1
	{
		f = (d > 0.0) ? (NPOW2(pvs_nxt->m_v) - NPOW2(m_v)) / (2.0f * d) : pklim->m_a;
		m_kinXOut.m_a = (f < pklim->m_a) ? pklim->m_a + pktwk->m_aHoldLocalLimitRatio * (f - pklim->m_a) : pklim->m_a;
		m_kinXOut.m_j = pklim->m_j + pktwk->m_jHoldLocalLimitRatio * pklim->m_j * (m_kinXOut.m_a / pklim->m_a - 1.0f);

		if ((m_kinXOut.m_a == pklim->m_a) && (m_kinXOut.m_j == pklim->m_j))
		{
			m_kinXOut.m_t = pklim->m_tx;
			m_kinXOut.m_v = pklim->m_vx;
			m_kinXOut.m_s = pklim->m_sx;
		}
		else
		{
			m_kinXOut.m_t = m_kinXOut.m_a / m_kinXOut.m_j;
			m_kinXOut.m_v = NPOW2(m_kinXOut.m_a) / (2.0f * m_kinXOut.m_j);
			m_kinXOut.m_s = m_kinXOut.m_j * NPOW3(m_kinXOut.m_t) / 6.0f;
		}
	}
	pvs_nxt->m_kinXIn = m_kinXOut;
}
