#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

#include "lib/N/NMemory.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageX.h"#include "lib/NL/MotionControl/Trajectory/NLVstage.h"


void NLVSTAGEX::compute_v(const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	Nf32 r, sqr;
	// k negatif signifie qu'on tourne � droite.
	if (k < 0.0f)
	{
		r = -1.0f / k;
		m_v = max_cruise_velocity * (1.0f + (r - half_axle_track) / (r + half_axle_track)) * 0.5f;
		sqr = sqrt(turn_right_inertia_coef / (-k));

		m_v = NMIN(m_v, sqr);
		FLAG_OFF(m_flags, FLAG_NLVSTAGEX_IS_CEIL);
	}
	// k positif signifie qu'on tourne � gauche.
	else if (k > 0.0f)
	{
		r = 1.0f / k;
		m_v = max_cruise_velocity * (1.0f + (r - half_axle_track) / (r + half_axle_track)) * 0.5f;
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

void NLVSTAGEX::set(const NLVSTAGE *pvsdesc)
{
	m_s0 = pvsdesc->m_s0;
	m_s1 = pvsdesc->m_s1;

	m_s0Prime = m_s0;
	m_s1Prime = m_s1;

	m_flags = 0;
	m_v = pvsdesc->m_v;

	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);
	m_stitch.null();
}

void NLVSTAGEX::set(const Nf32 s0, const Nf32 s1, const Nf32 vlimit, const Nbool bisceil)
{
	NErrorIf(s1 < s0, NERROR_INCONSISTENT_VALUES);		 // s1 doit �tre plus grand que s0
	NErrorIf(vlimit < 0.0f, NERROR_INCONSISTENT_VALUES); // vlimit doit �tre positif
	m_v = vlimit;

	m_s0 = s0;
	m_s1 = s1;

	m_s0Prime = s0;
	m_s1Prime = s1;

	m_flags = 0;

	m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);
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
	compute_v(k, half_axle_track, turn_left_inertia_coef, turn_right_inertia_coef, max_cruise_velocity);
}
void NLVSTAGEX::setButKinX(const Nf32 s0, const Nf32 s1, const Nf32 k, const Nf32 half_axle_track, const Nf32 turn_left_inertia_coef, const Nf32 turn_right_inertia_coef, const Nf32 max_cruise_velocity)
{
	m_s0 = s0;
	m_s1 = s1;

	m_s0Prime = m_s0;
	m_s1Prime = m_s1;

	m_flags = 0;

	// !!! bien fond� de cette fonction !!! NE PAS MODIFIER LES VALEURS m_axIn et m_axOut
	// m_axIn = NF32_MAX;
	// m_axOut = NF32_MAX;
	// m_kinXIn.tjavs_set(0.0f, -NF32_MAX, -NF32_MAX, 0.0f, 0.0f);
	// m_kinXOut.tjavs_set(0.0f, NF32_MAX, NF32_MAX, 0.0f, 0.0f);

	m_stitch.null();
	compute_v(k, half_axle_track, turn_left_inertia_coef, turn_right_inertia_coef, max_cruise_velocity);
}

// Il faudra ici int�grer la notion de courbure influencant l'acceleration max possible du robot de telle sorte que l'acceleration max de la roue exterieure au virage
// reste inferieure ou �gale � l'acceleration max limite !
// on montre, en se basant sur les fonction d'odometrie pour calculer a_left et a_right, que a doit �tre:
//																											a <= (2 * pklim->m_a ) / ( 2 + e*k )
//																											a : accel max local du robot
//																											e : entraxe
//																											k : courbure
// donc il faut ajouter cette contrainte ici, c'est � dire prendre la plus contraignante des contraintes d'accel.
//																											pklim->m_a
//																											a_max_local_trapezoidale
//																											a ( = a_max_local_en_fonction_de_courbure )
void NLVSTAGEX::setKinXOut(NLVSTAGEX *pvs_nxt, const NLKINTWEAK *pktwk, const NLKINLIMITS *pklim)
{
	Nf32 f;
	Nf32 d = pvs_nxt->m_s0 - m_s1;

	if (m_v > pvs_nxt->m_v) // signifie v0 > v1
	{
		// "f" represente l'acceleration locale max dans un modele trapezoidal.
		// Demonstration:
		// posons:	d	distance parcourue
		//			v0	vitesse au d�part
		//			v1	vitesse � l'arriv�e
		//			t	dur�e �coul�e entre le d�part et l'arriv�e
		//			a	acc�l�ration (constante, le mod�le est trap�zoidal) permettant de passer de v0 � v1
		//
		//			on connait d,v0 et v1. On ne connait ni t, ni a
		//			on cherche � trouver a
		//
		//			Dans un mod�le trapezoidal on peut �crire:
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
		//					a	= (v1� - v0�)/(2*d)				[3]
		//
		// Concr�tement la longueur de la clothoide correspond � d, v0 � la vitesse du VSTAGE pr�c�dent, et v1 � celle du VSTAGE courant:
		//
		f = (d > 0.0) ? (NPOW2(pvs_nxt->m_v) - NPOW2(m_v)) / (2.0f * d) : -pklim->m_a;
		//
		// si f est plus petite ( en val.absolue) que le max alors f devient le nouveau max possible. on utilise pktweak pour permettre � l'utilisateur de moduler et
		// ainsi choisir une accel. max comprise entre f et le max possible autoris� par les specs de la drive train.

		m_kinXOut.m_a = (f > -pklim->m_a) ? -pklim->m_a + pktwk->m_aHoldLocalLimitRatio * (f + pklim->m_a) : -pklim->m_a;
		m_kinXOut.m_j = -(pklim->m_j + pktwk->m_jHoldLocalLimitRatio * pklim->m_j * (-m_kinXOut.m_a / pklim->m_a - 1.0f));
		// NErrorIf(NSIGN(m_kinXOut.m_j) >= 0, NERROR_SYSTEM_GURU_MEDITATION);
		if ((m_kinXOut.m_a == -pklim->m_a) && (m_kinXOut.m_j == -pklim->m_j))
		{
			m_kinXOut.m_t = pklim->m_tx;
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
