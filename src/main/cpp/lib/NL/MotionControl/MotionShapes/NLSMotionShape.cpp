#include "lib/N/NErrorHandling.h"
#include "lib/N/NMath.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"

#include "lib/NL/MotionControl/NLKin.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageX.h"
#include "lib/NL/MotionControl/Trajectory/NLSovershoot.h"
// #include "../NLScut.h"

#include "lib/NL/MotionControl/MotionShapes/NLSMotionShape.h"

Nu32 NLSMOTIONSHAPE::promote(NLKIN *pkbuff)
{
	NLKIN *pk = pkbuff + 1;
	Nu32 inserted = 0;

	// Promote point/kin de fin de phase 1/3
	pk->m_j = m_jx;
	pk->m_a = m_ax;
	pk->m_v = m_v0 + m_vx;
	pk->m_s = m_s0 + m_sx + m_v0 * m_tx;
	pk->m_t = pkbuff->m_t + m_tx;
	pk++;
	inserted++;

	// Promote point/kin de fin de phase 2/3 + Promote point/kin de fin de phase 3/3
	if (m_tb)
	{
		pk->m_j = 0.0f;
		pk->m_a = m_ax;
		pk->m_v = m_v1 - m_vx;
		pk->m_s = m_s0 + m_ds + m_sx - m_v1 * m_tx;
		pk->m_t = pkbuff->m_t + m_tx + m_tb;
		pk++;
		inserted++;
	}
	// Promote point/kin de fin de phase 3/3 sans phase 2/3 ( m_tb est null )
	pk->m_j = -m_jx;
	pk->m_a = 0.0f;
	pk->m_v = m_v1;
	pk->m_s = m_s0 + m_ds;
	pk->m_t = pkbuff->m_t + (m_tx * 2.0f + m_tb);
	inserted++;
	return inserted;
}
Nu32 NLSMOTIONSHAPE::promote(NLKIN *pkbuff, const Nf32 cutin_dt)
{
	NLKIN *pk = pkbuff + 1;
	Nf32 t = pkbuff->m_t + cutin_dt;
	Nu32 inserted = 0;

	// Promote point/kin de fin de phase 1/3
	pk->m_j = m_jx;
	pk->m_a = m_ax;
	pk->m_v = m_v0 + m_vx;
	pk->m_s = m_s0 + m_sx + m_v0 * m_tx;
	pk->m_t = t + m_tx;
	pk++;
	inserted++;

	// Promote point/kin de fin de phase 2/3 + Promote point/kin de fin de phase 3/3
	if (m_tb)
	{
		pk->m_j = 0.0f;
		pk->m_a = m_ax;
		pk->m_v = m_v1 - m_vx;
		pk->m_s = m_s0 + m_ds + m_sx - m_v1 * m_tx;
		pk->m_t = t + m_tx + m_tb;
		pk++;
		inserted++;
	}
	// Promote point/kin de fin de phase 3/3 sans phase 2/3 ( m_tb est null )
	pk->m_j = -m_jx;
	pk->m_a = 0.0f;
	pk->m_v = m_v1;
	pk->m_s = m_s0 + m_ds;
	pk->m_t = t + (m_tx * 2.0f + m_tb);
	inserted++;
	return inserted;
}

Nu32 NLSMOTIONSHAPE::promote(NLKIN *pkbuff, const NLSCUTOUTOFFSETS *pcut_out)
{
	NLKIN *pk = pkbuff + 1;
	Nu32 inserted = 0;

	// Promote point/kin de fin de phase 1/3
	pk->m_j = m_jx;
	pk->m_a = m_ax;
	pk->m_v = m_v0 + m_vx;
	pk->m_s = m_s0 + m_sx + m_v0 * m_tx;
	pk->m_t = pkbuff->m_t + m_tx;
	pk++;
	inserted++;

	// Promote point/kin de fin de phase 2/3 + Promote point/kin de fin de phase 3/3
	if (m_tb)
	{
		pk->m_j = 0.0f;
		pk->m_a = m_ax;
		pk->m_v = m_v1 - m_vx;
		pk->m_s = m_s0 + m_ds + m_sx - m_v1 * m_tx;
		pk->m_t = pkbuff->m_t + m_tx + m_tb;
		pk++;
		inserted++;
	}
	// Promote point/kin de fin de phase 3/3 sans phase 2/3 ( m_tb est null )
	pk->m_j = -m_jx;
	pk->m_a = pcut_out->m_da;
	pk->m_v = m_v1 + pcut_out->m_dv; // *NSIGN(m_jx);
	pk->m_s = m_s0 + m_ds + pcut_out->m_ds;
	pk->m_t = pkbuff->m_t + (m_tx * 2.0f + m_tb) + pcut_out->m_dt;
	inserted++;
	return inserted;
}

Nu32 NLSMOTIONSHAPE::promote(NLKIN *pkbuff, const Nf32 cutin_dt, const NLSCUTOUTOFFSETS *pcut_out)
{
	NLKIN *pk = pkbuff + 1;
	Nf32 t = pkbuff->m_t + cutin_dt;
	Nu32 inserted = 0;

	// Promote point/kin de fin de phase 1/3
	pk->m_j = m_jx;
	pk->m_a = m_ax;
	pk->m_v = m_v0 + m_vx;
	pk->m_s = m_s0 + m_sx + m_v0 * m_tx;
	pk->m_t = t + m_tx;
	pk++;
	inserted++;

	// Promote point/kin de fin de phase 2/3 + Promote point/kin de fin de phase 3/3
	if (m_tb)
	{
		pk->m_j = 0.0f;
		pk->m_a = m_ax;
		pk->m_v = m_v1 - m_vx;
		pk->m_s = m_s0 + m_ds + m_sx - m_v1 * m_tx;
		pk->m_t = t + m_tx + m_tb;
		pk++;
		inserted++;
	}
	// Promote point/kin de fin de phase 3/3 sans phase 2/3 ( m_tb est null )
	pk->m_j = -m_jx;
	pk->m_a = pcut_out->m_da;
	pk->m_v = m_v1 + pcut_out->m_dv; // *NSIGN(m_jx);
	pk->m_s = m_s0 + m_ds + pcut_out->m_ds;
	pk->m_t = t + (m_tx * 2.0f + m_tb) + pcut_out->m_dt;
	inserted++;
	return inserted;
}
/*
void NLSMOTIONSHAPE::forwardBuildUp(const NLKINLIMITS * pkmax, const NLVSTAGEX * pv0, const NLVSTAGEX * pv1)
{
	NErrorIf(NABS(pv0->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(pv1->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = pv1->m_v - pv0->m_v;
	Nf32 _2vx;
	NErrorIf(dv0v1 < 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre sup�rieure � v0 !

	m_s0 = pv0->m_s1	+ pv0->m_overshootOffsets.m_dsx;
	m_v0 = pv0->m_v		+ pv0->m_overshootOffsets.m_dv*2.0f;
	m_v1 = pv1->m_v;

	Nf32 conserv_local_ax_ratio = 0.0f;
	Nf32 conserv_local_jx_ratio = 0.0f;
	if (dv0v1)
	{
		// Choix du Jerk ( positif ) qui sera utilis�
		m_jx = pkmax->m_j;

		if (pv0->m_axOut < pkmax->m_a)
		{
			m_ax	= pkmax->m_a + conserv_local_ax_ratio*(pv0->m_axOut - pkmax->m_a);
			//r		= m_ax / pkmax->m_a;
			//m_jx	= pkmax->m_j + conserv_local_jx_ratio * (pkmax->m_j*r - pkmax->m_j);
			//m_jx = pkmax->m_j * m_ax / pkmax->m_a;
			m_tx = m_ax / m_jx;
			_2vx = m_ax * m_tx;

			if (dv0v1 < _2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;		 // m_ax positive !
				m_vx = dv0v1 / 2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*m_tx;
			}
			else // (dv0v1 >= pkmax->m_2vx)
			{
				//m_tx = m_tx;
				m_tb = (dv0v1 - _2vx) / m_ax; // ( avec vb = dv0v1 - _2vx )
				//m_ax = m_ax;			 // m_ax positive !
				m_vx = _2vx/2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*(m_tx + m_tb / 2.0f);
			}
		}
		else // (pv0->m_axOut >= pkmax->m_a)
		{
			m_jx = pkmax->m_j;				 // m_jx positif !
			if (dv0v1 < pkmax->m_2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;		 // m_ax positive !
				m_vx = dv0v1 / 2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*m_tx;
			}
			else // (dv0v1 >= pkmax->m_2vx)
			{
				m_tx = pkmax->m_tx;
				m_tb = (dv0v1 - pkmax->m_2vx) / pkmax->m_a; // ( avec vb = dv0v1 - pkmax->m_2vx )
				m_ax = pkmax->m_a;			 // m_ax positive !
				m_vx = pkmax->m_vx;			 // m_vx positive !
				m_sx = pkmax->m_sx;			// m_sx est toujours du signe de m_jx qui ici est positif.
				m_ds = (pv0->m_v + pv1->m_v)*(m_tx + m_tb / 2.0f);
			}
		}
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;

		m_ax = 0.0f;
		m_vx = 0.0f;

		m_sx = 0.0f;

		m_ds = 0.0f;
	}
}
*/

NLSMOTIONSHAPE::STYPE NLSMOTIONSHAPE::forwardBuildUp(const NLVSTAGEX *pv0, const Nf32 v1)
{

	m_s0 = pv0->m_s1 + pv0->m_stitch.m_start.m_s1Offsets.m_ds; // L'abscisse de d�part de la S Shape est �gale � l'abscisse de la sortie de VSTAGE ( + l'offset de raccordement si il est non null )
	m_v0 = pv0->m_v + pv0->m_stitch.m_start.m_s1Offsets.m_dv;  // La vitesse de d�part de la S Shape est �gale � la vitesse fix�e par le VSTAGE ( + l'offset de raccordement si il est non null )
	m_v1 = v1;												   // La vitesse d'arriv�e d ela S Shape est �gale � la vitesse cible.

	Nf32 dv0v1 = m_v1 - m_v0;							// Calcul de dv0v1, diff�rence entre vitesse initiale et finale de la S Shape.
	NErrorIf(dv0v1 < 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre sup�rieure � v0 ! Car nous construisont une S SHAPE montante de gauche � droite sur la ligne de temps. ( = forwardBuildUp)

	if (dv0v1)
	{
		Nf32 dv0v1oo2 = dv0v1 / 2.0f;

		if (dv0v1oo2 < pv0->m_kinXOut.m_v)
		{
			m_jx = pv0->m_kinXOut.m_j;
			m_tx = sqrt(dv0v1 / m_jx);
			m_ax = m_tx * m_jx;					// m_ax positive !
			m_vx = dv0v1oo2;					// m_vx positive !
			m_sx = (m_jx * NPOW3(m_tx)) / 6.0f; //(m_ax*NPOW2(m_tx)) / 6.0f;	// (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...

			m_tb = 0.0f;
			m_ds = (m_v0 + m_v1) * m_tx;
		}
		else // (dv0v1oo2 > pv0->m_kinXOut.m_v)
		{
			m_tx = pv0->m_kinXOut.m_t;
			m_jx = pv0->m_kinXOut.m_j;
			m_ax = pv0->m_kinXOut.m_a; // m_vx positive !
			m_vx = pv0->m_kinXOut.m_v; // m_vx positive !
			m_sx = pv0->m_kinXOut.m_s; // (m_ax*m_tx*m_tx) / 6.0f;		// (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...

			m_tb = (dv0v1 - 2.0f * m_vx) / m_ax; // ( avec vb = dv0v1 - _2vx )
			m_ds = (m_v0 + m_v1) * (m_tx + m_tb / 2.0f);
		}

		return STYPE::SUP;
	}
	else //(dv0v1 == 0.0f)
	{
		m_tx = 0.0f;
		m_jx = 0.0f;
		m_ax = 0.0f;
		m_vx = 0.0f;
		m_sx = 0.0f;

		m_tb = 0.0f;
		m_ds = 0.0f;

		return STYPE::EMPTY;
	}
}

/*
void NLSMOTIONSHAPE::forwardBuildUp(const NLKINLIMITS * pkmax, const NLVSTAGEX * pv0, const NLVSTAGEX * pv1)
{
	NErrorIf(NABS(pv0->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(pv1->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = pv1->m_v - pv0->m_v;
	Nf32 _2vx;
	NErrorIf(dv0v1 < 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre sup�rieure � v0 !

	m_s0 = pv0->m_s1+ pv0->m_stitch.m_start.m_s1Offsets.m_ds;
	m_v0 = pv0->m_v + pv0->m_stitch.m_start.m_s1Offsets.m_dv;
	m_v1 = pv1->m_v;

	if (dv0v1)
	{
		if (pv0->m_flags&MASK_NLVSTAGE_OUT_MAX == MASK_NLVSTAGE_OUT_MAX) // Si pv0 utilise accel max et jerk max tel que definis par pkmax.
		{
			m_jx = pkmax->m_j;				 // m_jx positif !
			if (dv0v1 < pkmax->m_2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;		 // m_ax positive !
				m_vx = dv0v1 / 2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*m_tx;
			}
			else // (dv0v1 >= pkmax->m_2vx)
			{
				m_tx = pkmax->m_tx;
				m_tb = (dv0v1 - pkmax->m_2vx) / pkmax->m_a; // ( avec vb = dv0v1 - pkmax->m_2vx )
				m_ax = pkmax->m_a;			 // m_ax positive !
				m_vx = pkmax->m_vx;			 // m_vx positive !
				m_sx = pkmax->m_sx;			// m_sx est toujours du signe de m_jx qui ici est positif.
				m_ds = (pv0->m_v + pv1->m_v)*(m_tx + m_tb / 2.0f);
			}
		}
		else
		{
			m_ax = pv0->m_axOut;
			m_jx = pkmax->m_j;// pv0->m_jxOut;
			m_tx = m_ax / m_jx;
			_2vx = m_ax * m_tx;

			if (dv0v1 < _2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;		 // m_ax positive !
				m_vx = dv0v1 / 2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*m_tx;
			}
			else // (dv0v1 >= pkmax->m_2vx)
			{
				//m_tx = m_tx;
				m_tb = (dv0v1 - _2vx) / m_ax; // ( avec vb = dv0v1 - _2vx )
				//m_ax = m_ax;			 // m_ax positive !
				m_vx = _2vx / 2.0f;		 // m_vx positive !
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...
				m_ds = (pv0->m_v + pv1->m_v)*(m_tx + m_tb / 2.0f);
			}
		}
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;

		m_ax = 0.0f;
		m_vx = 0.0f;

		m_sx = 0.0f;

		m_ds = 0.0f;
	}
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Construit une S-SHAPE entre 2 vitesses v0, vitesse de d�part, et v1, vitesse d'arriv�e, dans le respect des limites fix�es.
 *			La vitesse v0 est suppos�e �tre inf�rieure ou �gale � v1.
 *			Le point de d�part de la sShape est fix� � s0
 *	@param	pkmax d�crit l'ensemble des limites � respecter.
 *	@param	s0 est l'abscisse de d�pard
 *	@param	v0 est la vitesse initiale ( m/s )	[ v0 <= v1 ]
 *	@param	v1 est la vitesse finale ( m/s )	[ v0 <= v1 ]
 */
// ------------------------------------------------------------------------------------------
NLSMOTIONSHAPE::STYPE NLSMOTIONSHAPE::forwardBuildUp(const NLKINLIMITS *pkmax, const Nf32 s0, const Nf32 v0, const Nf32 v1)
{
	NErrorIf(NABS(v0) > pkmax->m_v, NERROR_INCONSISTENT_VALUES); // v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(v1) > pkmax->m_v, NERROR_INCONSISTENT_VALUES); // v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )

	m_s0 = s0;
	m_v0 = v0;
	m_v1 = v1;

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = m_v1 - m_v0;
	NErrorIf(dv0v1 < 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre sup�rieure � v0 !

	// dv0v1 > 0 signifie que v1 > v0. En cons�quence les valeurs axx,jxx et vxx seront positives
	if (dv0v1)
	{
		m_jx = pkmax->m_j; // m_jx positif !
		if (dv0v1 < pkmax->m_2vx)
		{
			// Calcul de m_tx dans ce cas pr�cis ( dv0v1 < pkmax->m_2vx )
			//		m_vx = (m_ax*m_tx)/2
			//		or,		m_ax = m_jx*m_tx
			//		donc,	m_vx = (m_jx*m_tx�)/2
			//				2*m_vx = m_jx*m_tx�
			//
			//		or,		2*m_vx = dv0v1
			//		donc,	dv0v1 = m_jx*m_tx�
			//				m_tx� = dv0v1/m_jx
			//				m_tx = sqrt(dv0v1/m_jx)

			m_tx = sqrt(dv0v1 / m_jx);
			m_tb = 0.0f;

			m_ax = m_tx * m_jx;	 // m_ax positive !
			m_vx = dv0v1 / 2.0f; // m_vx positive !

			m_sx = (m_ax * m_tx * m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...

			// Calcul de m_ds0s1 dans ce cas pr�cis ( dv0v1 < pkmax->m_2vx )
			// La distance parcourue lors du passage de v0 � v1 sur une dur�e de temps T vaut:
			//		ds	=	( (v0+v1)/2 )*T
			//		or,		T = 2*m_tx
			//		donc,	ds	=	( (v0+v1)/2 )*2*m_tx
			//		donc	ds  =	(v0 + v1)*m_tx

			m_ds = (v0 + v1) * m_tx;
			// m_dt0t1 = 2.0f*m_tx;
		}
		else // (dv0v1 >= pkmax->m_2vx)
		{
			m_tx = pkmax->m_tx;
			m_tb = (dv0v1 - pkmax->m_2vx) / pkmax->m_a; // ( avec vb = dv0v1 - pkmax->m_2vx )

			m_ax = pkmax->m_a;	// m_ax positive !
			m_vx = pkmax->m_vx; // m_vx positive !

			m_sx = pkmax->m_sx; // m_sx est toujours du signe de m_jx qui ici est positif.

			// Calcul de m_ds0s1 dans ce cas pr�cis ( dv0v1 > pkmax->m_2vx )
			// La distance parcourue lors du passage de v0 � v1 sur une dur�e de temps T vaut:
			//		ds	=	( (v0+v1)/2 )*T
			//		or,		T = 2*m_tx + m_tb
			//		donc,	ds	=	( (v0+v1)/2 )*(2*m_tx + m_tb)
			//		donc,	ds	=	( (v0+v1)/2 )*2*(m_tx + m_tb/2)
			//		donc	ds  =	(v0 + v1)*(m_tx + m_tb/2)

			m_ds = (v0 + v1) * (pkmax->m_tx + m_tb / 2.0f);
			// m_dt0t1 = m_tb + 2.0f*m_tx;
		}
		return STYPE::SUP;
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;

		m_ax = 0.0f;
		m_vx = 0.0f;

		m_sx = 0.0f;

		m_ds = 0.0f;
		// m_dt0t1 = 0.0f;
		return STYPE::EMPTY;
	}
}

NLSMOTIONSHAPE::STYPE NLSMOTIONSHAPE::backwardBuildDown(const Nf32 v0, const NLVSTAGEX *pv1)
{

	m_v0 = v0;
	m_v1 = pv1->m_v + pv1->m_stitch.m_end.m_s0Offsets.m_dv;

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = m_v1 - m_v0;
	NErrorIf(dv0v1 > 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre inf�rieure � v0 !

	if (dv0v1)
	{
		Nf32 dv0v1oo2 = dv0v1 / 2.0f;

		if (dv0v1oo2 > pv1->m_kinXIn.m_v)
		{
			m_jx = pv1->m_kinXIn.m_j;
			m_tx = sqrt(dv0v1 / m_jx);
			m_ax = m_tx * m_jx; // m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
			m_vx = dv0v1oo2;	// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif. ( et v1<v0 ! donc dv0v1 toujours negatif ! )
			m_sx = (m_jx * NPOW3(m_tx)) / 6.0f;
			; // (m_ax*NPOW2(m_tx)) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.

			m_tb = 0.0f;
			m_ds = (m_v0 + m_v1) * m_tx;
		}
		else //  (dv0v1oo2 <= pv1->m_kinXIn.m_v )
		{
			m_tx = pv1->m_kinXIn.m_t;
			m_jx = pv1->m_kinXIn.m_j;
			m_ax = pv1->m_kinXIn.m_a; // m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
			m_vx = pv1->m_kinXIn.m_v; // m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
			m_sx = pv1->m_kinXIn.m_s; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.

			m_tb = (dv0v1 - 2.0f * m_vx) / m_ax;
			m_ds = (m_v0 + m_v1) * (m_tx + m_tb / 2.0f);
		}
		m_s0 = pv1->m_s0 + pv1->m_stitch.m_end.m_s0Offsets.m_ds - m_ds;
		return STYPE::SDOWN;
	}
	else //(dv0v1 == 0.0f)
	{
		m_tx = 0.0f;
		m_jx = 0.0f;
		m_ax = 0.0f;
		m_vx = 0.0f;
		m_sx = 0.0f;

		m_tb = 0.0f;
		m_ds = 0.0f;

		m_s0 = pv1->m_s0 + pv1->m_stitch.m_end.m_s0Offsets.m_ds;
		return STYPE::EMPTY;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Construit une S-SHAPE entre 2 vitesses v0, vitesse de d�part, et v1, vitesse d'arriv�e, dans le respect des limites fix�es.
 *			La vitesse v0 est suppos�e �tre sup�rieure ou �gale � v1.
 *			Le point de d�part de la sShape est calcul� � partir du point d'arriv�e s1. s0 = s1 - ds
 *	@param	pkmax d�crit l'ensemble des limites � respecter.
 *	@param	pv0 est le VStage de "d�part". Plus exactement contient "la vitesse de d�part".
 *	@param	pv1 est le Vstage d'arriv�e. Plus exactement, il contient la vitesse et le point d'arriv�e ( qui ne peut �tre que pv1->m_s0 car la SSHAPE descends sur pv1)
 */
// ------------------------------------------------------------------------------------------
/*
void NLSMOTIONSHAPE::backwardBuildDown(const NLKINLIMITS * pkmax, const NLVSTAGEX * pv0, const NLVSTAGEX * pv1)
{
	NErrorIf(NABS(pv0->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(pv1->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = pv1->m_v - pv0->m_v;
	Nf32 _2vx;
	NErrorIf(dv0v1 > 0.0f, NERROR_INCONSISTENT_VALUES);		// v1 doit �tre inf�rieure � v0 !

	m_v0 = pv0->m_v;
	m_v1 = pv1->m_v + pv1->m_stitch.m_end.m_s0Offsets.m_dv;

	if (dv0v1)
	{
		NErrorIf(pv1->m_axIn >= 0.0f, NERROR_INCONSISTENT_VALUES);
		if (pv1->m_flags&MASK_NLVSTAGE_IN_MAX == MASK_NLVSTAGE_IN_MAX) // Si pv1 utilise accel max et jerk max tel que definis par pkmax.
		{
			m_jx = -pkmax->m_j;
			if (dv0v1 > -pkmax->m_2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;				// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = dv0v1 / 2.0f;			// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif. ( et v1<v0 ! donc dv0v1 toujours negatif ! )
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*m_tx;
			}
			else // (dv0v1 <= -pkmax->m_2vx)
			{
				m_tx = pkmax->m_tx;
				m_tb = (-dv0v1 - pkmax->m_2vx) / pkmax->m_a;
				m_ax = -pkmax->m_a;		// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = -pkmax->m_vx;	// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_sx = -pkmax->m_sx;	// m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*(pkmax->m_tx + m_tb / 2.0f);
			}
		}
		else
		{
			m_ax = pv1->m_axIn;
			m_jx = -pkmax->m_j;// pv1->m_jxIn;
			m_tx = m_ax / m_jx;														// m_tx > 0 ! (�videment )
			_2vx = m_ax * m_tx;														// _2vx < 0 !

			if (dv0v1 > _2vx) //_2vx < dv0v1 < 0
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;				// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = dv0v1 / 2.0f;			// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif. ( et v1<v0 ! donc dv0v1 toujours negatif ! )
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*m_tx;
			}
			else // (dv0v1 <= _2vx)
			{
				// m_tx = m_tx;
				m_tb = (-dv0v1 - pkmax->m_2vx) / pkmax->m_a;
				//m_ax = m_ax;					// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = _2vx / 2.0f;				// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*(pkmax->m_tx + m_tb / 2.0f);
			}
		}
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;
		m_ax = 0.0f;
		m_vx = 0.0f;
		m_sx = 0.0f;
		m_ds = 0.0f;
	}

	m_s0 = pv1->m_s0 + pv1->m_stitch.m_end.m_s0Offsets.m_ds - m_ds;
}
*/
/*
void NLSMOTIONSHAPE::backwardBuildDown(const NLKINLIMITS * pkmax, const NLVSTAGEX * pv0, const NLVSTAGEX * pv1)
{
	NErrorIf(NABS(pv0->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(pv1->m_v) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);	// v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )

	// Calcul de dv0v1, dsv0v1 et ds0s1:
	Nf32 dv0v1 = pv1->m_v - pv0->m_v;
	Nf32 _2vx;
	NErrorIf(dv0v1 > 0.0f, NERROR_INCONSISTENT_VALUES);		// v1 doit �tre inf�rieure � v0 !

	m_v0 = pv0->m_v;
	m_v1 = pv1->m_v + pv1->m_overshootOffsets.m_dv*2.0f;

	Nf32 conserv_ax_ratio = 0.0f;
	if (dv0v1)
	{
		// Choix du Jerk ( negatif ) qui sera utilis�
		m_jx = -pkmax->m_j;

		NErrorIf(pv1->m_axIn >= 0.0f, NERROR_INCONSISTENT_VALUES);
		if (pv1->m_axIn > -pkmax->m_a)
		{
			m_ax = pv0->m_axIn + conserv_ax_ratio * (-pkmax->m_a - pv0->m_axIn);	// m_ax < 0 !
			//m_jx = pkmax->m_j * m_ax / pkmax->m_a;
			m_tx = m_ax / m_jx;														// m_tx > 0 ! (�videment )
			_2vx = m_ax * m_tx;														// _2vx < 0 !

			if (dv0v1 > _2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;				// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = dv0v1 / 2.0f;			// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif. ( et v1<v0 ! donc dv0v1 toujours negatif ! )
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*m_tx;
			}
			else // (dv0v1 <= _2vx)
			{
				// m_tx = m_tx;
				m_tb = (-dv0v1 - pkmax->m_2vx) / pkmax->m_a;
				//m_ax = m_ax;					// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = _2vx/2.0f;				// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*m_tx;
			}
		}
		else // (pv1->m_axIn <= -pkmax->m_a)
		{
			if (dv0v1 > -pkmax->m_2vx)
			{
				m_tx = sqrt(dv0v1 / m_jx);
				m_tb = 0.0f;
				m_ax = m_tx * m_jx;				// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = dv0v1 / 2.0f;			// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif. ( et v1<v0 ! donc dv0v1 toujours negatif ! )
				m_sx = (m_ax*m_tx*m_tx) / 6.0f; // m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*m_tx;
			}
			else // (dv0v1 <= -pkmax->m_2vx)
			{
				m_tx = pkmax->m_tx;
				m_tb = (-dv0v1 - pkmax->m_2vx) / pkmax->m_a;
				m_ax = -pkmax->m_a;		// m_ax n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_vx = -pkmax->m_vx;	// m_vx n�gative ! ... est toujours du signe de m_jx qui ici est negatif.
				m_sx = -pkmax->m_sx;	// m_sx n�gatif  ! ... est toujours du signe de m_jx qui ici est negatif.
				m_ds = (m_v0 + m_v1)*(pkmax->m_tx + m_tb / 2.0f);
			}
		}
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;
		m_ax = 0.0f;
		m_vx = 0.0f;
		m_sx = 0.0f;
		m_ds = 0.0f;
	}

	m_s0 = pv1->m_s0 + pv1->m_overshootOffsets.m_dsx - m_ds;
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Construit une S-SHAPE entre 2 vitesses v0, vitesse de d�part, et v1, vitesse d'arriv�e, dans le respect des limites fix�es.
 *			La vitesse v0 est suppos�e �tre sup�rieure ou �gale � v1.
 *			Le point de d�part de la sShape est calcul� � partir du point d'arriv�e s1. s0 = s1 - ds
 *	@param	pkmax d�crit l'ensemble des limites � respecter.
 *	@param	s1 est l'abscisse d'arriv�e
 *	@param	v0 est la vitesse initiale ( m/s )	[ v0 >= v1 ]
 *	@param	v1 est la vitesse finale ( m/s )	[ v0 >= v1 ]
 */
// ------------------------------------------------------------------------------------------
NLSMOTIONSHAPE::STYPE NLSMOTIONSHAPE::backwardBuildDown(const NLKINLIMITS *pkmax, const Nf32 s1, const Nf32 v0, const Nf32 v1)
{
	NErrorIf(NABS(v0) > pkmax->m_v, NERROR_INCONSISTENT_VALUES); // v0 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
	NErrorIf(NABS(v1) > pkmax->m_v, NERROR_INCONSISTENT_VALUES); // v1 doit �tre inf�rieure ou egale � la "vitesse max" ! ( ... et sup�rieure � "- vitesse max" )
																 // Calcul de dv0v1, dsv0v1 et ds0s1:
	m_v0 = v0;
	m_v1 = v1;
	Nf32 dv0v1 = m_v1 - m_v0;
	NErrorIf(dv0v1 > 0.0f, NERROR_INCONSISTENT_VALUES); // v1 doit �tre inf�rieure � v0 !

	// dv0v1 < 0 signifie que v1 < v0. En cons�quence les valeurs axx,jxx et vxx seront positives
	if (dv0v1)
	{
		m_jx = -pkmax->m_j; // m_jx n�gatif !

		if (dv0v1 > -pkmax->m_2vx)
		{
			// Calcul de m_tx dans ce cas pr�cis ( dv0v1(n�gatif) > -pkmax->m_2vx )
			//	cf plus haut ( NLSMOTIONSHAPE::build) le calcul de m_tx dans le cas sym�trique ( dv0v1(positif) < pkmax->m_2vx )
			m_tx = sqrt(dv0v1 / m_jx);
			m_tb = 0.0f;

			m_ax = m_tx * m_jx;	 // m_ax n�gative !
			m_vx = dv0v1 / 2.0f; // m_vx n�gative !

			m_sx = (m_ax * m_tx * m_tx) / 6.0f; // (m_sx = (m_jx*m_tx^3)/6 comme m_ax = m_jx*m_tx ...

			m_ds = (v0 + v1) * m_tx;
			// m_dt0t1 = 2.0f*m_tx;
		}
		else // (dv0v1 <= -pkmax->m_2vx)
		{
			m_tx = pkmax->m_tx;
			m_tb = (-dv0v1 - pkmax->m_2vx) / pkmax->m_a;

			m_ax = -pkmax->m_a;	 // m_ax n�gative !
			m_vx = -pkmax->m_vx; // m_vx n�gative !

			m_sx = -pkmax->m_sx; // m_sx est toujours du signe de m_jx qui ici est negatif.

			m_ds = (v0 + v1) * (pkmax->m_tx + m_tb / 2.0f);
			// m_dt0t1 = m_tb + 2.0f*m_tx;
		}

		m_s0 = s1 - m_ds;

		return STYPE::SDOWN;
	}
	else //(dv0v1 == 0.0f)
	{
		m_jx = 0.0f;
		m_tx = 0.0f;
		m_tb = 0.0f;

		m_ax = 0.0f;
		m_vx = 0.0f;
		m_sx = 0.0f;

		m_ds = 0.0f;
		// m_dt0t1 = 0.0f;
		m_s0 = s1;

		return STYPE::EMPTY;
	}
}
/*
Nu32 NLSMOTIONSHAPE::forwardOvershoot(NLSOVERSHOOT * povs, const Nf32 s1)
{

	// Soit 'smax' une abscisse de "psrc" un S-MotionProfile simple ( i.e une S-MotionShape ) exprim�e par rapport � (s0,t0)
	// On cherche � identifier si 'smax' est situ�e avant la fin de la S-Motion shape.
	// Si tel est le cas on cherche alors � trouver la valeur de 't', temps/date � laquelle l'abscisse 'smax' est atteinte par "psrc".
	// ! Remarque importante:
	// une S-MotionShape se d�compose en 3 phases distinctes durant lesquelles a,v et s �volue en fonction de t
	//
	//											t						a					v					s
	//
	//						Phase 1:		[0,tx[						[0,ax[				[v0,v0+vx[			[0,s[
	//						Phase 2:		[tx,tx+tb]					[ax,ax]				[v0+vx,v1-vx]		[s,ss]
	//						Phase 3:		]tx+tb,tx+tb+tx]			]ax,0]				]v1-vx,v1]			]ss,sss]
	// Cette fonction est exclusivement utilis�e dans le cadre de la construction d'une trajectoire � partie d'un NLPATH au sein de l'algo "NLTRAJECTORY::build()".
	// Tel qu'est concu l'algorithme, il est impossible qu'un "forwardOvershoot" se produise pendant les phases de progression/maintient de l'acceleration (phase 1 et 2),
	// en effet,  si une telle situation se pr�sente, ( et cela est evidement possible ) elle aura �t� d�tect�e avant et consid�r�e comme "inaccessibility" ( et pas comme Overshoot)
	// et le NLVSTAGEX concern� sera tout simplement retir� de la liste des NLVSTAGEs � traiter.
	Nf32	ds0s1 = s1 - m_s0;
	if (ds0s1 >= m_ds) // ... Il n'y a pas d'overshoot
	{
		return 0;
	}
	//  --> A partir d'ici on a donc :	ds0s1 < m_ds0s1

	NErrorIf(m_jx <= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le forwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "montante" avec un jerk initial positif !
	Nf32	s;
	Nf32	ss;
	Nf32	dt, tok, sok;
	Nf32	t;
	Nf32	sdichotom;
	Nf32	dsss0s1;

	s	= m_v0*m_tx + m_sx;
	ss	= s + ((m_v0 + m_v1)* m_tb) / 2.0f;
	//	On remarquera que ss = s dans le cas ou tb est null. Cela ne d�range pas, l'enchainement des tests reste valide.

	if (ds0s1 > ss) //Gr�ce au test effectu� en d�but de fonction,  on sait que ( ds0s1 < m_ds) on a donc ici:	ss < ds0s1 < m_ds
	{
		// Signifie que ds0s1 est plus grande que ss, la distance n�c�ssaire pour atteindre le moment pr�cis o� l'acc�l�ration commence � diminuer � nouveau
		// La difference des deux (ds0s1 - ss), repr�sente la portion du troisi�me intervalle de la s-shape qui ne d�passe pas ( la partie qui reste � gauche de s1 )
		dsss0s1 = ds0s1 - ss;

		// on compte le temps et la distance � partir du debut de ce troisi�me intervalle ]tx+tb,tx+tb+tx] / ]ss,sss].
		// on approchera le point d'Overshoot par la droite ( <----+ ), en veillant � ce que notre solution approch�e soit sup�rieure ou �gale � la solution pr�cise.
		dt	= m_tx;
		t	= m_tx;
		tok = m_tx;			// soit la totalit� de la dur�e de l'intervalle 3.
		sok = m_ds - ss;	// soit la totalit� de la distance couverte pas l'intervalle 3.

		do
		{
			sdichotom = (m_v1 - m_vx)*t + m_ax*t*t / 2.0f - (m_jx*t*t*t) / 6.0f; // quand t = m_tx, sdichotom repr�sente la totalit� de la distance couverte pas l'intervalle 3.
			if (sdichotom >= dsss0s1)
			{
				tok = t;
				sok = sdichotom;
			}
			dt /= 2.0f;
			t = tok - dt;
		} while ((sok - dsss0s1) > 0.005f);
		// On se retrouve ici avec:
		//					'tok'	temps �coul�		depuis le d�but du troisi�me intervalle		( soit par rapport � t0 = tx + tb)
		//					'sok'	distance parcourue	depuis le d�but du troisi�me intervalle		( soit par rapport � s0 = ss )

		// Consid�rons les points suivants:
		//		D d�but du premier intervalle, soit le d�but de la S_SHAPE
		//		P d�but du troisi�me intervalle
		//		O point d'Overshoot approch� par dichotomie
		//		F fin de la S_SHAPE et donc du troisi�me intervalle
		//		et
		//		G le point de redemarrage "fant�me" situ� dans le pass� de O.
		//
		//		Chacun de ces 4 points est d�fini par un quadruplet (t,a,v,s)
		//

		// CUT OUT
		//	 Le CutOut est le vecteur FO, soit la description du point d'Overshoot O par rapport � la fin F de la S_SHAPE.
		//
		//	 Soit O( tok,?,?,sok) le point d'Overshoot et F(m_tx,0,m_vx,?) la fin de la S_SHAPE, exprim�s tous les deux par rapport � P, d�but du troisi�me intervalle.
		//
		//
		//1)'povs_out->m_dt' repr�sente la dur�e s�parant le point d'Overshoot O de F, la fin de la S_SHAPE (= la fin du troisi�me intervalle).
		//	 On sait que la dur�e du troisi�me intervalle est de 'm_tx'
		//	 En cons�quence,
		//					OF = PF - PO
		//					avec PF = m_tx et pO = tok
		//					povs_out->m_dt = FO.t = -OF.t = -( m_tx - tok ) = tok - m_tx
		//
		//
		//2)'povs_out->m_ds' repr�sente la distance alg�brique du point d'Overshoot O(sok,tok) par rapport � F la fin de la S_SHAPE (= la fin du troisi�me intervalle.)
		//					povs_out->m_ds = FO.s
		//	 La distance couverte par le troisi�me intervalle vaut:
		//					S = m_ds0s1 - ss	avec m_ds0s1 la distance couverte par l'ensemble des intervalles repr�sentant la S_SHAPE
		//										et ss  la distance couverte par tous les intervalles repr�sentant la S_SHAPE sauf le troisi�me
		//					( S est donc la distance alg�brique de F par rapport au d�but du troisi�me intervalle S = PF )
		//	 En cons�quence,
		//					povs_out->m_ds = FO.s
		//					povs_out->m_ds = O.s - F.s	( O et F exprim�s dans la m�me base, ici par rapport � P )
		//					povs_out->m_ds = sok - S
		//					povs_out->m_ds	= sok - (m_ds0s1 - ss)
		//					povs_out->m_ds	= sok - m_ds0s1 + ss
		//
		//
		//3)'povs_out->m_dv' repr�sente la vitesse alg�brique au point d'Overshoot O par rapport � F, la fin de la S_SHAPE (= la fin du troisi�me intervalle.)
		//					povs_out->m_dv = FO.v
		//					povs_out->m_dv = O.v - F.v	( O et F exprim�s dans la m�me base, ici par rapport � P )
		//					O.v, la quantit� de vitesse acquise en O depuis le d�but du troisi�me intervalle vaut:
		//					O.v	= m_ax*tok - m_jx*tok�/2
		//					                    ( note: m_vx = m_ax*m_tx - m_jx*m_tx�/2 = m_jx*m_tx/2) ??? c'est vrai mais est-ce � gard� ici ???
		//	 En cons�quence,
		//					povs_out->m_dv = FO.v = O.v - F.v
		//					povs_out->m_dv = O.v - m_vx
		//					povs_out->m_dv = m_ax*tok - m_jx*tok�/2 - m_vx
		povs_out->m_dt = m_tx - tok;									//NErrorIf(povs_out->m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre <= 0
		povs_out->m_dv = m_ax * tok - m_jx * tok*tok / 2.0f  - m_vx;	//NErrorIf(povs_out->m_dv > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre <= 0    ...  pcout->m_dv = 0 si tok = m_tx
		povs_out->m_ds = ss + sok - m_ds;								//NErrorIf(povs_out->m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0

		// OVERSHOOT OFFSET (Forward)
		// Nous sommes dans la derni�re phase de ce Motion Profile, celle avec un Jerk oppos� au Jerk initial.
		// Le temps n�c�ssaire pour amen� l'acceleration � 0 depuis le temps 'tok' vaut:
		//					povs_out->m_dt = m_tx - tok.
		// C'est �galement le temps qu'il "aurait fallu" pour atteindre la valeur de l'acc�l�ration au point d'Overshoot O( sok,tok,vok,aok) depuis un "point fant�me" ayant une acceleration nulle.
		// Ce "point fant�me" est un point fictif situ� "avant" O. Nous l'appelerons G(t,a,v,s).
		//
		// En forwardOvershoot, on recherche m_ds, m_dv et m_dt, les offsets � ajouter aux donn�es de NLVSTAGEX pour "bien repartir" � droite de l'Overshoot.
		//			 Soit depuis G, le point fant�me.
		//
		//1) Pour continuer au-del� de l'overshoot, on red�marre du point fant�me situ� dans le pass� proche de l'overshoot et nous permettant
		//	 d'atteindre et de d�passer l'overshoot. En d�marrant du point fantome et en appliquant m_jx sur une dur�e de |povs->m_dt| on se retrouve exactement en O(?,a,v,s) !
		//	 L'offset de temps � consid�rer vaut:  povs_out->m_dt ( et est donc n�gative car il s'agit ici de "remonter le temps" )
		//
		//2) La quantit� de vitesse � ajouter � la vitesse du VSTAGE pour obtenir la vitesse au 'point fant�me'
		//	 est la vitesse alg�brique en G exprim�e par rapport � VSTAGE.v
		//	 Cette valeur est obligatoirement n�gative ( car G.v < VSTAGE.v )
		//					F.v = m_vx
		//					G.v = m_vx + 2*pcout->m_dv ( '+' car 'pcout->m_dv' est alg�brique et exprim�e par rapport � F )
		//					v = G.v - F.v
		//					v = 2*povs_out->m_dv
		//
		//3)'povs_out->m_dsx' repr�sente la distance � ajouter � la position S1 du VSTAGE pour obtenir l'abscisse du 'point fant�me' de red�marrage.
		//			povs_out->m_dsx est obligatoirement n�gatif
		//			Il est important de prendre en compte l'approche par dichotomie du point d'Overshoot dans le calcul de 'povs_out->m_dsx'.
		//			En effet, l'abscisse de ce point d'Overshoot devrait th�oriquement �tre �gale � VSTAGE.m_s1, mais elle est en fait l�g�rement plus grande, car approch�e  par la droite...
		//			Il convient donc de calculer l'abscisse de ce point fant�me par rapport � la fin de la S.SHAPE dont la position est connue et fiable, puis de calculer la distance entre ce point
		//			et VSTAGE.m_s1.
		//			La distance s�parant ce point fant�me de la fin de la S_SHAPE vaut:
		//					GF = ( (v0 + v1)/2 ) * T
		//					avec, v0 la vitesse au point fant�me et v1 la vitesse � la fin de la S_SHAPE.
		//					or,		v0	=	v1 + 2*povs_out->m_dv ( povs_out->m_dv < 0 )
		//							T	=	2*povs_out->m_dt
		//					donc,	GF	=	( ( v1 + 2*povs_out->m_dv + v1 )/2 ) * 2*povs_out->m_dt
		//							GF	=	( ( 2*v1 + 2*povs_out->m_dv )/2 ) * 2*povs_out->m_dt
		//							GF	=	( 2*( v1 + povs_out->m_dv )/2 ) * 2*povs_out->m_dt
		//							GF	=	( v1 + povs_out->m_dv ) * 2*povs_out->m_dt
		//
		//			On obtient donc G.s l'abscisse du point fant�me de red�marrage:
		//					G.s	= s0 + SSHAPE.m_ds0s1 - GF	 avec 's0 + SSHAPE.m_ds0s1' correspondant � l'abscisse de la fin de la S_SHAPE.
		//			Pour obtenir la distance alg�brique de G par rapport � s1 il suffit de faire:
		//					povs_out->m_dsx	=	G.s - s1
		//

		povs_out->m_dsx = m_s0 + m_ds - (m_v1 + povs_out->m_dv)*2.0f*povs_out->m_dt - s1;		//NErrorIf(povs_out->m_ds >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0

		// old
		// povs->m_dt = pcout->m_dt;													NErrorIf(povs->m_dt >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
		// povs->m_dv = 2.0f*pcout->m_dv;												NErrorIf(povs->m_dv >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
		// old

		return 1;
	}
	else if (ds0s1 == ss)
	{
		// Pour rappel on consid�re D,P,O,F,G les points suivants:
		//		D d�but du premier intervalle, soit le d�bu de la S_SHAPE
		//		P d�but du troisi�me intervalle
		//		O point d'Overshoot approch� par dichotomie
		//		F fin de la S_SHAPE et donc du troisi�me intervalle
		//		et
		//		G le point de redemarrage "fant�me"
		//
		// Ici, nous sommes dans le cas particulier o� le point d'Overshoot O coincide avec le point P et donc o� le point G coincide avec le point D !
		// Donc ...
		//
		// CUTOUT
		povs_out->m_dt = -m_tx;			// = FO		= FP = -m_tx
		povs_out->m_ds = -m_ds + ss;	// = FO		= FP = -(DF - DP) = -(m_ds0s1 - ss)
		povs_out->m_dv = -m_vx;			// = FO		= FP = -PF = -m_vx
		// OVERSHOOT OFFSET ( dans ce cas, l'abscisse du Point O d'overshoot est exactement �gale � s1 )
		//povs->m_dt	= -m_tx;
		//povs->m_dv	= -2.0f*m_vx;
		povs_out->m_dsx	= m_s0 + m_ds - 2.0f*(m_v1 - povs_out->m_dv)*2.0f*m_tx - s1;
		return 1;
	}
#ifdef _DEBUG
	else
	{
		// Cela ne devrait JAMAIS arriver !!!
		// Le forward overshoot ne peut normalement apparaitre que sur la phase de diminution de l'acceleration de sa valeur max � 0.
		//NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
#endif

	return 0;
}
*/
Nu32 NLSMOTIONSHAPE::forwardOvershoot(NLSCUTOUTOFFSETS *pcutout, NLSTARTSTITCH *pimmediate_start, const NLVSTAGEX *pv0, NLVSTAGEX *pv1)
{
	// V�rification de la coh�rence de la SShape et des 2 Vstages pass�s en param�tres.
	// NErrorIf(pv0->m_v != m_v0, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv1->m_v != m_v1, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv0->m_jxOut != m_jx, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv0->m_axOut != m_ax, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(m_jx <= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);		// le forwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "montante" avec un jerk initial positif !

	// Soit 'smax' l'abscisse maximum que la SShape ne doit pas franchir.
	// Le dernier "point" (K3) de la SShape doit avoir une abscisse inf�rieure ou �gale � cette abscissr 'smax'.
	//
	// On cherche donc � identifier si 'smax' est situ�e avant la fin de la S-Motion shape.
	// Si tel est le cas on cherche alors � trouver la valeur de 't', temps/date � laquelle l'abscisse 'smax' est "franchie" par la SShape.
	//
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|					UNE S SHAPE "montante" ou 'S.UP' avec ces 4 'points' remarquables: K0, K1, K2 et K3
	//	|
	//	|												   -----+ K3
	//	|											   ----
	//	|									    K2 +---
	//	|										  -
	//	|										 .
	//	|									    .
	//	|									   .
	//	|									  .
	//	|									 .
	//	|									-
	//	|								---+ K1
	//	|							 ----
	//	|					K0 +-----
	//	|
	//	|					   .		   .	   .			.
	//	|					   .		   .	   .			.
	//	|					   .  PHASE 1  .PHASE 2.   PHASE 3	.
	//	|					   .		   .	   .			.
	//	|
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	! Remarque importante:
	//	|	Une S-MotionShape se d�compose en 3 phases distinctes durant lesquelles a,v et s �volue en fonction de t
	//	|
	//	|									t						a					v					s			portion	de S.Shape
	//	|
	//	|				Phase 1:		[0,tx[						[0,ax[				[v0,v0+vx[			[0,s[			[K0,K1[
	//	|				Phase 2:		[tx,tx+tb]					[ax,ax]				[v0+vx,v1-vx]		[s,ss]			[K1,K2]
	//	|				Phase 3:		]tx+tb,tx+tb+tx]			]ax,0]				]v1-vx,v1]			]ss,sss]		]K2,K3]
	//	|
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	Cette fonction est exclusivement utilis�e dans le cadre de la construction d'une trajectoire � partie d'un NLPATH au sein de l'algo "NLTRAJECTORY::build()".
	//	|	Tel qu'est concu l'algorithme, il est impossible qu'un "forwardOvershoot" se produise pendant les phases de progression/maintient de l'acceleration (phase 1 et 2),
	//	|	en effet,  si une telle situation se pr�sente, ( et cela est evidement possible ) elle aura �t� d�tect�e avant et consid�r�e comme "inaccessible" ( et pas comme Overshoot)...
	//	|	... et le NLVSTAGEX concern� tout simplement retir� de la liste des NLVSTAGEs � traiter.
	//	|
	//	|	Ainsi, un Forward Overshoot ne peut se produire qu'en phase 3 : ]K2,K3] ou en K2 soit sur l'intervalle [K2,K3].
	//	|
	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//
	//	ds0s1 repr�sente la Distance disponible entre le point de d�part de la SMOTIONSHAPE et la sortie du VSTAGE d'arriv�e.
	//	La distance couverte par la SSHAPE doit imp�rativement �tre inf�rieure ou �gale � ds0s1.
	//	En cas d'Overshoot, ds0s1 repr�sente l'abscisse EXACTE du point d'overshoot mesur�e depuis le d�but de la SShape (m_s0)
	Nf32 ds0s1 = pv1->m_s1 - m_s0;

	if (ds0s1 >= m_ds)
	{
		// Il n'y a pas d'overshoot !
		return 0;
	}

	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//  |	--> A partir d'ici on a donc :	m_ds > ds0s1
	//	|
	//	|	Il y a donc OVERSHOOT !!!
	//	|
	Nf32 s;
	Nf32 ss;
	Nf32 dt, tok, sok;
	Nf32 t;
	Nf32 sdichotom;
	Nf32 dsss0s1;
	Nf32 vx;

	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	's' repr�sente la distance couverte au moment ou l'acceleration atteind son max, soit sur une dur�e de m_tx.
	//	|	Cette distance est celle couverte par l'intervalle [k0,K1].
	//	|
	s = m_v0 * m_tx + m_sx;

	//	+-------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	'ss' repr�sente la distance couverte au moment ou l'acceleration max va commencer � diminuer, soit sur une dur�e de m_tx + m_tb.
	//	|	Cette distance est celle couverte par l'intervalle [k0,K2].
	//	|	On remarquera que ss = s dans le cas ou tb est null.
	//	|	... Cela ne d�range pas, l'enchainement des tests reste valide.
	//	|
	ss = s + ((m_v0 + m_v1) * m_tb) / 2.0f;

	NErrorIf(ds0s1 < ss, NERROR_SYSTEM_GURU_MEDITATION); // !!! Cela signifie que le Vstage pv1 est inaccessible ! ( car on d�passe la sortie de pv1 alors qu'on est toujours en phase d'accel. max maintenue )
														 // !!! Normalement le test de "collision" pr�alable aurait du le detecter.

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	A/	RECHERCHE DE 'O' le point d'Overshoot sur la phase 3 soit sur ]K2,K3]
	//	|
	if (ds0s1 > ss)
	{
		//	Gr�ce au test effectu� en d�but de fonction, on sait que(ds0s1 < m_ds) on a donc ici : ss < ds0s1 < m_ds
		//	La difference des deux (ds0s1 - ss), repr�sente la portion du troisi�me intervalle de la s-shape qui ne d�passe pas ( la partie qui reste � gauche de s1 )
		//	Rappel:
		//			ds0s1	repr�sente l'abscisse EXACTE du point d'overshoot mesur�e depuis le d�but de la SShape (m_s0)
		//			ss 		repr�sente l'abscisse du point K2 mesur�e depuis le d�but de la SShape (m_s0)
		//
		//	On calcule donc dsss0s1 l'abscisse EXACTE du point d'overshoot mesur�e depuis K2
		dsss0s1 = ds0s1 - ss;

		//	Il nous faut maintenant trouver � quelle moment/date la SSHAPE atteind cette abscisse.
		//	Dans cette version de la fonction 'forwardOvershoot' on approchera la valeur de cette 'date' par it�rations successives ( dichotommie ) jusqu'� ce que l'erreur
		//	sur l'abscisse recalcul�e � partir de cette valeur approch�e de date soit inf�rieure � un certain seuil.

		//	On compte le temps et la distance � partir du debut de ce troisi�me intervalle	]K2,K3] ->	]tx+tb,tx+tb+tx]
		//																							->	]ss,sss]
		//	on approchera le point d'Overshoot par la droite ( <----+ ), en veillant � ce que notre solution approch�e soit sup�rieure ou �gale � la solution pr�cise.
		//	Ainsi, l'abscisse de notre solution approch�e sera sup�rieure � pv1->m_s1
		//
		//	On effectue la recherche par rapport � K2(  ... dans cette base, K3.m_dt = m_tx ) et  en partant du point de la SShape le plus �loign� possible
		//	sur la droite, soit K3.
		dt = m_tx;		 //	soit K3.m_dt par rapport � K2 ( = la totalit� de la dur�e de la phase 3 )
		t = m_tx;		 //	soit K3.m_dt par rapport � K2 ( = la totalit� de la dur�e de la phase 3 )
		tok = m_tx;		 //	soit K3.m_dt par rapport � K2 ( = la totalit� de la dur�e de la phase 3 ).
		sok = m_ds - ss; //	soit K3.m_ds par rapport � K2 ( = la totalit� de la distance couverte pas la phase 3 ).

		//
		do
		{
			//	'sdichotom' repr�sente la distance parcourue depuis K2 au temps 't' (mesur� depuis K2 �galement)
			//	( quand t = m_tx, sdichotom repr�sente donc la totalit� de la distance couverte par la phase 3, soit l'intervalle [K2,K3] ).
			sdichotom = (m_v1 - m_vx) * t + m_ax * NPOW2(t) / 2.0f - (m_jx * NPOW3(t)) / 6.0f;

			//	On ne conserve les valeurs de 'tok' et 'sok' que si elles sont sup�rieures o� �gales aux valeurs 'r�elles' du point d'Overshoot.
			//	Ainsi comme souhait�, les valeurs 'sok' et 'tok' de notre solution approch�e seront toujours sup�rieures ou �gale aux valeurs de la solution pr�cise.
			if (sdichotom >= dsss0s1)
			{
				tok = t;
				sok = sdichotom;
			}
			// A chaque it�ration on divise par 2 le delta de temps que l'on retranche au 'tok' courant pour se rapprocher de la solution
			// Cela revient � rapprocher de K2 depuis k3 ( ... et donc de trouver 'O' sur le chemin )
			dt /= 2.0f;
			t = tok - dt;
		} while ((sok - dsss0s1) > 0.005f); //	On continue tant que l'erreur est sup�rieure � un certain seuil.
											//	Rappel:
											//			dsss0s1 repr�sente l'abscisse EXACTE du point d'overshoot mesur�e depuis K2
											//			sok		approche cette abscisse exacte par la droite, sans jamais lui �tre inf�rieure
											//			Nous sommes donc s�rs que (sok - dsss0s1) sera toujours sup�rieur ou �gal � 0 !

		//	+--------------------------------------------------------------------------------------------------------------------------
		//	|
		//	|	A.1/	CUT OUT
		//	|
		//	|			Le CutOut 'positionne' les donn�es du point d'Overshoot par rapport � la fin de la SShape ( K3 )
		//	|

		pcutout->m_dt = tok - m_tx; //	pcutout->m_dt repr�sente la 'date' de l'Overshoot mesur�e par rapport � K3.
									//	tok repr�sente la date  de l'Overshoot mesur�e par rapport � K2.
									//	m_tx  repr�sente la date  de K3 mesur�e par rapport � K2.
									//	'O' pr�c�de K3 dans le temps, nous avons �videment 'tok < mtx' ... C'est pourquoi 'tok - m_tx' est TOUJOURS n�gatif !

		pcutout->m_da = -m_jx * pcutout->m_dt; //	pcutout->m_da repr�sente l'accel. de l'Overshoot mesur�e par rapport � K3.
											   //	L'accel. de K2 vaut m_ax
											   //	L'accel. de l'overshoot  vaut m_ax - m_jx*tok ( en effet l'accel en K2 vaut 'm_ax' par d�finition et d�croit jusqu'� atteindre 0 en K3 )
											   //	L'accel. de K3 vaut 0
											   //
											   //	On a donc l'accel. de l'overshoot mesur�e par rapport � K3 vaut (m_ax - m_jx*tok) - 0 = m_ax - m_jx*tok
											   //	On sait que		m_ax = m_jx * m_tx
											   //	Donc,			m_ax - m_jx*tok = m_jx*m_tx - m_jx*tok
											   //									= m_jx*(m_tx - tok )
											   //									= -m_jx * pcutout->m_dt
											   //

		NErrorIf(pv1->m_kinXOut.m_a <= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre > 0

		//	|
		//	|			Il est possible que la valeur de l'accel. de l'Overshoot soit plus grande que l'accel. autoris�e � droite de VStage.S1 ( soit la valeur de l'accel. de KinXOut )
		//	|			Comme la strat�gie sera ensuite de 'repartir' depuis l'overshoot pour continuer, il n'est pas envisageable de le faire avec une accel. sup�rieure � celle autoris�e.
		//	|			En cons�quence, si le cas se produit on remplace pcutout->m_da par la valeur (plus petite) de KinXOut.m_da. Se faisant on modifie par la m�me occasion tous les autres param�tres
		//	|			de l'Overshoot, y compris son abscisse qui sera in�vitablement plus grande dans le cas de ce forward overshoot.( car l'accel. decroit depuis K2.m_da = m_ax vers k3.m_da = 0,
		//	|			et on veut une accel. plus petite, dont un Overshoot plus pr�t de K3 ... )
		//	|

		if (pcutout->m_da > pv1->m_kinXOut.m_a)
		{
			pcutout->m_da = pv1->m_kinXOut.m_a; //	On remplace la valeur initialement calcul�e par la limite d'acceleration locale fx�e par KinXOut

			pcutout->m_dt = -pcutout->m_da / m_jx; //	La nouvelle valeur de m_dt correspond au temps  n�c�ssaire pour acqu�rir m_da avec m_jx.
												   //	Rappel: m_dt, comme toutes les valeurs de pcutout, est mesur� depuis K3 ( d'o� le signe - ).

			pcutout->m_dv = -NPOW2(pcutout->m_da) / (2.0f * m_jx); //	La valeur de m_dv se calcule classiquement.
																   //	Rappel:		Avec une accel. initiale (en K3) 'nulle' et finale (en 'O') valant 'a', le delta de vitesse acquis sur une dur�e t vaut:
																   //				dv	= a*t/2
																   //					= a*(a/j)/2	avec a=jt ou t=a/j ( sur la m�me p�riode t, a est pass�e de 0 � sa valeur finale, par int�gration de j )
																   //					= a�/(2*j)
																   //	Comme pour m_dt, le signe - est n�c�ssaire car pcutout->m_dv est mesur� depuis K3 ...
																   //	C'est en K3 que l'accel est nulle et en 'O', situ� avant, qu'elle vaut 'a'.

			pcutout->m_ds = m_v1 * pcutout->m_dt - m_jx * NPOW3(pcutout->m_dt) / 6.0f; //	La valeur de m_ds se calcule classiquement.
																					   //	Rappel:		l'�quation du mouvement pour la position est S = S0 + V0*t + (1/2)*a0*t^2 + (1/6)*j*t^3
																					   //				En se placant sur K3 et en "remontant le temps" en direction de 'O' on a,
																					   //				S0 = 0		... on consid�re  K3 comme point de d�part.
																					   //				V0 = m_v1	... la vitesse en K3.
																					   //				a0 = 0		... l'accel. en K3.
																					   //				j  = m_jx	... de 'O' vers k3, j = -m_jx, donc de K3 vers 'O' c'est l'inverse.
																					   //				t  = pcutout->m_dt soit le temps s�parant 'O' de K3
																					   //					 ! Attention ce temps est n�gatif !
																					   //
																					   //				S = S0 + V0*t				+	(1/2)*a0*t^2				+	(1/6)*j*t^3
																					   //				S = 0  + m_v1*pcutout->m_dt	+	(1/2)*0*pcutout->m_dt^2     +	(1/6)*(- m_jx)* NPOW3(pcutout->m_dt)
																					   //				S = 0  + m_v1*pcutout->m_dt	+	0						    +	(1/6)*(- m_jx)* NPOW3(pcutout->m_dt)
																					   //				S =		 m_v1*pcutout->m_dt	- m_jx * NPOW3(pcutout->m_dt) / 6.0f
																					   //
																					   //	Etude des signes ( car le signe de m_dt est NEGATIF )
																					   //	pcutout->m_ds =		 m_v1*pcutout->m_dt		-	m_jx * NPOW3(pcutout->m_dt) / 6.0f
																					   //						 (+) *(-)				-	(+)  * (-)^3                / (+)
																					   //						...........			   ...  .......................................
																					   //							 |					|						|
																					   //						    (-)					-					   (-)
																					   //							 |				   ............................................
																					   //							 |								|
																					   //							(-)							   (+)
																					   //
																					   //	La quantit� |m_v1*pcutout->m_dt| �tant sup�rieure � |m_jx * NPOW3(pcutout->m_dt) / 6.0f| ...
																					   //	... on voit bien que 'pcutout->m_ds' est n�gatif. Ce qui est normal car 'O' pr�c�de K3.
		}
		//	|
		//	|			Dans le cas o� la valeur de l'accel. de l'Overshoot est inf�rieure ou �gale � celle du KinXOut, on conserve donc les valeurs de pcutout->m_da et pcutout->m_dt calcul�es pr�c�dement
		//	|			Et on calcule les valeurs des 2 autres membres de cutout.
		//	|
		else //(pcutout->m_da <= pv1->m_axOut)
		{
			pcutout->m_dv = m_ax * tok - m_jx * NPOW2(tok) / 2.0f - m_vx; //	Rappel:		l'�quation du mouvement pour la vitesse est V = V0 + a0*t + (1/2)*j*t^2
																		  //				On cherche 'pcutout->m_dv', la difference de vitesse entre K3 et 'O' mesur�e depuis K3.
																		  //				On calcule d'abord simplement la vitesse en 'O'
																		  //						  O.v	= K2.v + m_ax*tok - m_jx*tok^2
																		  //				Puis on l'exprime par rapport � K3
																		  //				pcutout->m_dv	= K2.v + m_ax*tok - m_jx*tok^2 - K3.v
																		  //
																		  //				On sait que m_vx = K3.v - K2.v	, donc ...
																		  //
																		  //				pcutout->m_dv	= m_ax*tok - m_jx*tok^2 + (K2 v - K3.v)
																		  //				pcutout->m_dv	= m_ax*tok - m_jx*tok^2 - m_vx

			pcutout->m_ds = ss + sok - m_ds; //	L'abscisse de 'O' par rapport au d�but de la SShape vaut ss + sok avec SS abscisse de K2 par rapport au d�but
											 //	et sok abscisse de 'O' par rapport � K2.
											 //	L'abscisse de K3 par rapport au d�but de la SShape vaut m_ds ( la dist. totale couverte par la SShape )
											 //	On a donc l'abscisse de 'O' par rapport � k3 :
											 //	pcutout->m_ds = (ss + sok) - m_ds
		}
	}
	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	B/	'O' le point d'Overshoot coincide avec le point K2 de la SShape
	//	|
	else if (ds0s1 == ss)
	{
		//	+--------------------------------------------------------------------------------------------------------------------------
		//	|
		//	|	B.1/	CUT OUT
		//	|
		//	|			Le CutOut 'positionne' les donn�es du point d'Overshoot par rapport � la fin de la SShape ( K3 )
		//	|			... Et comme ici, 'O' est K2 ...
		//	|			( voir plus haut en A.1 pour les explications sur les calculs )
		//	|
		if (m_ax > pv1->m_kinXOut.m_a)
		{
			pcutout->m_da = pv1->m_kinXOut.m_a;
			pcutout->m_dt = -pcutout->m_da / m_jx;
			pcutout->m_dv = -NPOW2(pcutout->m_da) / (2.0f * m_jx);
			pcutout->m_ds = m_v1 * pcutout->m_dt - m_jx * NPOW3(pcutout->m_dt) / 6.0f;
		}
		else //(pcutout->m_da <= pv1->m_axOut)
		{
			pcutout->m_dt = -m_tx;
			pcutout->m_da = m_ax;
			pcutout->m_ds = -m_ds + ss;
			pcutout->m_dv = -m_vx;
		}
	}
	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	C/	'O' le point d'Overshoot se situe avant le point K2 de la SShape ??????? !!!!!
	//	|		!!! Cela ne devrait JAMAIS arriver !!!
	//	|
	/*else // signifie (ds0s1 < ss)
	{
		// !!! Cela signifie que le Vstage pv1 est inaccessible !!! Normalement le test de "collision" pr�alable aurait du le detecter ....
		// En debug le test NErrorIf(ds0s1 < ss, NERROR_SYSTEM_GURU_MEDITATION) est effectu� en d�but de fonction ... au cas o�
		// Le forward overshoot ne peut normalement apparaitre que sur la phase de diminution de l'acceleration de sa valeur max � 0.
	}*/

	NErrorIf(pcutout->m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre <= 0
	NErrorIf(pcutout->m_da < 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre >= 0
	NErrorIf(pcutout->m_dv > 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre <= 0
	NErrorIf(pcutout->m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre <= 0

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	D/	Immediate Start
	//	|
	//	|		La detection d'un Overshoot 'O' entraine donc un 'CutOut'. L'objectif est d'arr�ter de faire baisser l'accel. pour la faire
	//	|		croitre � nouveau juste apr�s 'O'. Les informations de 'CutOut' permettent de conserver la SShape jusqu'� 'O'.
	//	|		On essaiera ensuite de construire une nouvelle shape d�marrant avant 'O' pour pouvoir passer par 'O' mais avec une acceleration en croissance et non plus en d�croissance...
	//	|		Cependant au cas o� cela s'av�re finalement impossible il est important de construire la S.SHAPE passant par 'O' par d�faut, c'est � dire celle qui red�marre uniquement pour "finir" la SShape exactement comme elle �tait
	//	|		sans le cutout, en red�marrant avant le point d'overshoot, depuis le point o� l'acceleration "�tait" nulle. !
	//	|		Cet immediate start se base sur le JERK de la SSHAPE alors que le start enregistr� dans le stage pv1 se base sur le JERK "pv1->m_kinXOut.m_j"
	//	|
	//	|
	//	|		m_s1Offsets
	//	|		La sshape de redemarrage commence, comme toute sshape avec une acceleration nulle, une vitesse v0 et une abscisse s0.
	//	|		La question est la suivante, � quelle position (abscisse) et avec quelle vitesse initiale dois-je d�marrer si je veux passer exactement par 'O' c'est � dire avec
	//	|		la bonne abscisse, la bonne vitesse et la bonne acceleration ?
	//	|		m_s1offset permet de d�finir cette position et cette vitesse de red�marrage par rapport au vstage d'arriv�e de la sshape (pv1), plus exactement par rapport � l'abscisse de sortie du Vstage (m_s1)
	//	|		et bien s�r par rapport � la vitesse impos�e par le vstage (m_v)
	//	|
	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	pimmediate_start->m_s1Offsets.m_ds = m_s0 + m_ds + (m_v1 + pcutout->m_dv) * 2.0f * pcutout->m_dt - pv1->m_s1;
	//
	//			m_s0 + m_ds ......................................	repr�sente l'abscisse 'monde' de la fin de la SHAPE. ( rappel: abscisse 'monde' de la fin de la SShape, S1 = m_s0 + m_ds )
	//			(m_v1 + pcutout->m_dv)............................	repr�sente la vitesse 'monde' � laquelle l'overshoot se produit. Elle est 'sous' m_v1 ( car pcutout->m_dv est n�gatif )
	//			(m_v1 + pcutout->m_dv) * 2.0f * pcutout->m_dt.....	repr�sente la distance parcourue depuis le point de depart fantome ou accel aurait �t� nulle jusqu'au point o� elle
	//																est � nouveau nulle c'est � dire en fin de SSHAPE de red�marrage.
	//																Cette distance est n�gative ( pcutout->m_dt < 0 ) elle est donc ici exprim�e par rapport � la fin de la sshape de redemarrage..
	//																Rappel:
	//																La SShape de red�marrage est une SShape sans maintient de  l'accel. max. ( tb = 0 ). D�s que l'accel.max est atteinte
	//																le jerk s'inverse et l'accel. diminue jusqu� atteindre 0 ( fin de la sshape ).
	//																On dit aussi qu'elle n'a pas de phase 2, que les points K1 et K2 sont confondus.
	//																La distance couverte par ce type de sshape se calcule comme ceci:
	//																	a) distance couverte en phase 1 :	m_v0*m_tx + m_sx
	//																	b) distance couverte en phase 2 :	0
	//																	c) distance couverte en phase 3 :	m_v1*m_tx - m_sx
	//
	//																	or, nous savons que m_v0 = m_v1 + 2*pcutout->m_dv	(! pcutout->m_dv est n�gatif !)
	//																		nous savons �galement que, pour cette SShape, m_tx = pcutout->m_dt ( ! attention pcut_out->m_dt est n�gatif ! )
	//																	donc,
	//																	a) distance couverte en phase 1 :	( m_v1 + 2*pcutout->m_dv)*pcutout->m_dt + msx
	//																	b) distance couverte en phase 2 :	0
	//																	c) distance couverte en phase 3 :	m_v1*pcutout->m_dt - m_sx
	//
	//																	Distance totale couverte pas la sshape de redemarrage:	D = (a) + (b) + (c)
	//																	D	= ( m_v1 + 2*pcutout->m_dv)*pcutout->m_dt + m_sx + 0 + m_v1*pcutout->m_dt - m_sx
	//																		= ( m_v1 + 2*pcutout->m_dv + m_v1)*pcutout->m_dt
	//																		= (2*m_v1 + 2*pcutout->m_dv)*pcutout->m_dt
	//																		= (m_v1 + pcutout->m_dv) * 2 * pcutout->m_dt
	//																	On precisera ici que les signes n�gatifs de pcutout->m_dv et de pcutout->m_dt sont bien pris en compte.
	//																	Le r�sultat est au final n�gatif ce qui correspond � ce que nous attendons.
	//			pv1->m_s1 .......................................	l'abscisse de fin de Vstage, par rapport � laquelle nous voulons placer le point de d�part de la SShape de redemarrage.
	//
	//			Au final, la somme [m_s0 + m_ds] + [ (m_v1 + pcutout->m_dv) * 2.0f * pcutout->m_dt ] repr�sente la position 'monde' du point de d�part "fant�me" de la SShape de redemarrage.
	//			et donc, en terminant avec - pv1->m_s1 on exprime cette position par rapport � l'abscisse de sortie du Vstage (m_s1).
	//	|
	//	+--------------------------------------------------------------------------------------------------------------------------

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	pimmediate_start->m_s1Offsets.m_dv = pcutout->m_dv * 2.0f;
	//
	//			La SShape de red�marrage est une SShape sans maintient de  l'accel. max. ( tb = 0 ).
	//			Le point 'O' correspond, pour cette SShape au point ou le jerk change de signe et |pcutout->m_dv| correspond � m_vx, le gain de vitesse quand l'accel. atteind son max.
	//			Le gain de vitesse totale d'une SShape sans maintient d'accel. max vaut 2 * m_vx, soit ici |2*pcutout->m_dv|
	//			La SShape de redemarrage rejoint en fin de course, la vitesse du vstage pv1. 'pimmediate_start->m_s1Offsets.m_dv' a pour objectif de placer la vitesse de d�marrage par rapport � celle du vstage.
	//			"pimmediate_start->m_s1Offsets.m_dv" est donc n�gatif, et donc �gal �   pcutout->m_dv * 2.0f;
	//	|
	//	+--------------------------------------------------------------------------------------------------------------------------

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	pimmediate_start->m_cutIn.m_dt = pcutout->m_dt;
	//
	//			La SShape de red�marrage a pour but de compl�ter la SSHAPE en forwardOvershoot coup�e en sortie avec les donn�es CutOut.
	//			La SShape de redemarrage sera, quand � elle, coup�e en entr�e avec des donn�es cutin de telle sorte que sa partie "restante" ( comprendre la partie non "coup�e")
	//			coincide parfaitement avec la partie "retir�e" ( comprendre la partie "coup�e" ) de la SSHAPE en forwardOvershoot.
	//			Il a d�j� �t� expliqu� que cette sshape de redemarrage �tait sans maintient de l'accel. max ( tb = 0 ).
	//			La partie de la Sshape situ�e avant 'O' ( = la partie � couper avec cutin ) et la partie de la sshape situ�e apr�s 'O' ( =la partie � conserver avec cutin)
	//			poss�dent toutes les 2 la m�me dur�e : m_tx
	//			Comme notre souhait est que la partie � gauche de 'O' corresponde � la partie "coup�e" de la SShape en overshoot, les deux doivent avoir la m�me dur�e :
	//
	//			pimmediate_start->m_cutIn.m_dt = pcutout->m_dt
	//
	//			On conservera bien le signe n�gatif de pcutout->m_dt, qui se justifie car pcutout->m_dt repr�sente la date de 'O' mesur�e depuis K3
	//			m_cutIn.m_dt repr�sente la date de K0 mesur�e depuis 'O' , cette valeur est donc n�gative �galement !
	//	|
	//	+--------------------------------------------------------------------------------------------------------------------------

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	pimmediate_start->m_cutIn.m_ds = (m_v1 + pcutout->m_dv) * pimmediate_start->m_cutIn.m_dt - m_jx * NPOW3(pimmediate_start->m_cutIn.m_dt) / 3.0f;
	//	|
	//	|		m_cutIn.m_ds repr�sente l'abscisse de K0 ( de la SShape de redemarrage ) mesur�e depuis 'O' , cette valeur est donc n�gative !
	//	|		(m_v1 + pcutout->m_dv)...................................	repr�sente la vitesse en 'O' ( point d'Overshoot )
	//	|																	pcutout->m_dv repr�sente la vitesse de 'O' par rapport � K3	et est donc <= 0
	//	|		(m_v1 + pcutout->m_dv)* pimmediate_start->m_cutIn.m_dt...	repr�sente la distance parcourue depuis 'O' en direction de K0 � vitesse constante
	//	|																	et pendant une dur�e de cutin.m-dt. Cette dist. est bien sur n�gative et sa valeur absolue trop grande car,
	//	|																	la vitesse �volue entre K0 et 'O' ...
	//	|		m_jx * NPOW3(pimmediate_start->m_cutIn.m_dt) / 3.0f;.....	repr�sente la distance 'en trop' qu'il faudra retirer � la quantit� pr�c�dente pour obtenir precisement
	//	|																	la distance parcourue entre K0 et 'O'. Cette distance est n�gative car exprim�e par rapport � 'O'
	//	+--------------------------------------------------------------------------------------------------------------------------

	//	+--------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	E/	STITCH
	//	|
	//	|		Les donn�es de STITCH fonctionnent dans l'id�e comme les donn�es de immediate_start. Au d�tail pret que le STITCH est stock� dans le VSTAGE ET
	//	|		que ces donn�es se basent sur le jerk du KinXOut et non pas sur celui de la SSHAPE en overshoot.
	//	|		En effet, le jerk utilis� par la sshape en overshoot peut-�tre diff�rent de celui pr�conis� par le kinXout de sortie de Vstage.
	//	|		Dans ce cas, si la sshape de redemarrage par defaut ( immediate start )	conserve le m�me jerk que la sshape overshoot toutes les autres SShape qui seront test�es
	//	|		par l'algo. de recherche utiliseront le jerk du KinXOut...
	//	|
	if (pv1->m_kinXOut.m_j == m_jx)
	{
		//		Dans le cas ou les deux jerks sont identiques... RAF. Le STITCH stock� dans le VStage est identique en tout point � l'immediate start
		pv1->m_stitch.m_start = *pimmediate_start;
	}
	else
	{
		//		En cas de difference, on recalcule les donn�es sp�cifiques du STITCH stock� dans le vstage
		//
		vx = -NPOW2(pcutout->m_da) / (2.0f * pv1->m_kinXOut.m_j);
		//		vx................	repr�sente la quantit� de vitesse acquise pendant que l'acceleration passe de ZERO � la valeur de pcutout->m_da ( valeur de l'accel. en 'O')
		//							par integration d'un jerk valant pv1->m_kinXOut.m_j.
		//							vx est n�gative car , il 'positionne' K0 par rapport � 'O'

		pv1->m_stitch.m_start.m_cutIn.m_dt = -pcutout->m_da / pv1->m_kinXOut.m_j;
		//		m_cutIn.m_dt......	repr�sente le temps n�c�ssaire pour que l'int�gration du jerk porte l'acceleration � sa valeur de pcutout->m_da en partant de 0.
		//							Dans le cas de l'immediate start la valeur de cutin.m_dt est �gale � celle de cutout.m_dt car le jerk de la SShape cutout est le m�me que celui de la SShape cutIn.
		//							mais ici, cela n'est pas le cas.
		//							comme pr�c�dement, cutIn.m_dt repr�sente la date de K0 mesur�e depuis 'O'. Cette valeur est donc n�gative.
		//

		pv1->m_stitch.m_start.m_cutIn.m_ds = (m_v1 + pcutout->m_dv) * pv1->m_stitch.m_start.m_cutIn.m_dt - pv1->m_kinXOut.m_j * NPOW3(pv1->m_stitch.m_start.m_cutIn.m_dt) / 3.0f;
		//		Voir les explication du calcul de 'pimmediate_start->m_cutIn.m_ds' quelques lignes plus haut.
		//		Nous faisons ici exactement la m�me chose avec une valeur de Jerk diff�rente.
		//

		pv1->m_stitch.m_start.m_s1Offsets.m_dv = pcutout->m_dv + vx;
		//		pcutout->m_dv......	represente la difference de vitesse entre K3 et 'O' mesur�e depuis K3. La vitesse en K3 �tant �gale � la vitesse limite du VStage,
		//							on peut donc dire que 'pcutout->m_dv' repr�sente aussi la vitesse de 'O' par rapport � la vitesse limite de VStage.
		//		vx................. repr�sente la vitesse de K0 par rapport � celle de 'O'
		//
		//		pcutout->m_dv + vx. repr�sente la vitesse du K0 de toute SShape de redemarrage contrainte par pv1->KinXOut, mesur�e par rapport � pv1->m_v.
		//

		pv1->m_stitch.m_start.m_s1Offsets.m_ds = m_s0 + m_ds + (pv1->m_stitch.m_start.m_cutIn.m_ds + pcutout->m_ds) - pv1->m_s1;
		//	(a)	m_s0 + m_ds....................................................	repr�sente la position 'monde' de la fin de la SShape en overshoot
		//	(b)	(pv1->m_stitch.m_start.m_cutIn.m_ds + pcutout->m_ds)...........	repr�sente la position du point d'overshoot 'O' par rapport  � K3, la fin de la sshape en overshoot ( soit pcutout->m_ds) ...
		//																		+ la position du K0 de la SShape de redemarrage mesur�e depuis 'O'. ( rappel : cutin.m_ds et cutout.m_ds sont tous 2 negatifs.)
		//																		Ainsi la somme de (a) et de (b) repr�sente la position 'monde' du K0 de la SShape de redemarrage.
		//																		En soustrayant pv1->m_s1 on obtient donc l'abscisse ( la position ) du K0 de la SShape de redemarrage par rapport � pv1->m_s1
	}

	return 1;
}

Nu32 NLSMOTIONSHAPE::backwardOvershoot(NLSCUTINOFFSETS *pcutin, NLENDSTITCH *pimmediate_end, NLVSTAGEX *pv0, const NLVSTAGEX *pv1)
{
	// V�rification de la coh�rence de la SShape et des 2 Vstages pass�s en param�tres.
	// NErrorIf(pv0->m_v != m_v0, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv1->m_v != m_v1, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv1->m_jxIn != m_jx, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(pv1->m_axIn != m_ax, NERROR_INCONSISTENT_VALUES);
	// NErrorIf(m_jx >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le backwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "descendante" avec un jerk initial negatif !
	// NErrorIf(m_ax >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le backwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "descendante" avec une accel initiale negative !

	//	Nf32 ds0s1 = (m_s0 + m_ds) - s0;
	if (pv0->m_s0 <= m_s0) // Il n'y a pas d'overshoot !
		return 0;
	//  --> A partir d'ici on a donc :	m_s0 < pv0->m_s0

	Nf32 s;
	Nf32 overshoot_acc; // acceleration au point d'overshoot
	Nf32 stitch_s;		//  - distance totale parcourue par la "stitch s-shape" complete ( est n�gative )
	Nf32 cutin_dv;		// vitesse  acquise( ou perdue ) pendant la phase de cut in
	Nf32 dt, tok, sok;
	Nf32 t;
	Nf32 sdichotom;

	Nf32 ds = pv0->m_s0 - m_s0; // pour approcher le point d'Overshoot depuis le debut de la S_SHAPE

	s = m_v0 * m_tx + m_sx;

	NErrorIf(ds > s, NERROR_SYSTEM_GURU_MEDITATION); // !!! Cela signifie que le Vstage pv0 est inaccessible depuis pv1 ( en bckwd) !!! Normalement le test de "collision" pr�alable aurait du le detecter ....

	//  --> Recherche de ds sur le premier Intervalle ]0,tx] / ]0,s].
	//  En effet, gr�ce au test pr�c�dent (voir plus haut) on sait d�j� que [ ds0s1 < m_ds0s1 ] DONC  ds > 0
	//	ds0s1 ( la distance entre le point de d�part et le point d'arriv�e ) est strictement inf�rieur � m_ds0s1 ( la distance totale couverte par la S_SHAPE ),
	//	Pour une raison algorithmique (�limination des "VStages" inaccessibles ) ds ne peut-�tre plus grande que s, car cel� signifierait qu'on peut "passer" le Vstage sans le toucher ...
	//	Donc � partir d'ici, on consid�re que
	//				0 < ds <= s
	if (ds < s)
	{
		dt = m_tx;
		tok = 0.0f; // on compte le temps � partir du debut de ce premier Intervalle ]0,tx] / ]0,s]
		t = 0.0f;
		sok = 0.0f; // on compte la distance parcourue entre v0 et v1 depuis le d�but du PREMIER intervalle

		do
		{
			sdichotom = m_v0 * t + m_jx * NPOW3(t) / 6.0f;
			if (sdichotom <= ds)
			{
				tok = t;
				sok = sdichotom;
			}
			dt /= 2.0f;
			t = tok + dt;
		} while ((ds - sok) > 0.005f);

		// CutIn
		overshoot_acc = m_jx * tok; // < 0 [-*+] ( car m_jx < 0 et tok > 0 )
		if (overshoot_acc < pv0->m_kinXIn.m_a)
		{
			// L'acceleration ( n�gative ) est trop importante !!! Il y a un risque de 'toucher' la pente de la clothoide en entr�e sur pv0.
			overshoot_acc = pv0->m_kinXIn.m_a;	  // < 0
			pcutin->m_dt = -overshoot_acc / m_jx; // < 0 [-(-)/-]
			pcutin->m_ds = m_v0 * pcutin->m_dt + m_jx * NPOW3(pcutin->m_dt) / 6.0f;
			NErrorIf(pcutin->m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre <= 0
																		  // +    * -			   + -    * - = -a + b
		}
		else
		{
			// a = m_jx * tok; ... 'a' conserve la valeur initialement calcul�e.
			pcutin->m_dt = -tok; // < 0
			pcutin->m_ds = -sok; // < 0
		}
	}
	// --> Recherche de ds sur le Second Intervalle [tx,tx+tb] / [s,ss]. En effet on sait d�j� que ds <= ss ( gr�ce au test pr�c�dent ... )
	else if (ds == s)
	{
		// ... alors on a : pimmediate_end->m_cutOut.m_da = m_ax !!
		if (m_ax < pv0->m_kinXIn.m_a) // m_ax est n�gatif
		{
			//			pv0->m_stitch.m_end.m_cutOut.m_da = pv0->m_kinXIn.m_a;
			//			pv0->m_stitch.m_end.m_cutOut.m_dt = -pv0->m_stitch.m_end.m_cutOut.m_da / m_jx;
			// L'acceleration ( n�gative ) est trop importante !!! Il y a un risque de 'toucher' la pente de la clothoide en entr�e sur pv0.
			overshoot_acc = pv0->m_kinXIn.m_a;	  // < 0 !
			pcutin->m_dt = -overshoot_acc / m_jx; // < 0 !
			pcutin->m_ds = m_v0 * pcutin->m_dt + m_jx * NPOW3(pcutin->m_dt) / 6.0f;
			NErrorIf(pcutin->m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // devrait �tre <= 0
		}
		else
		{
			overshoot_acc = m_ax;					   // < 0 !
			pcutin->m_dt = -m_tx;					   // < 0 !
			pcutin->m_ds = m_v0 * pcutin->m_dt - m_sx; // < 0 ...  !!!  pcutin->m_dt et m_sx sont negatifs ...
													   //				   +    * -            - - = -a + b
													   // 1.3) m_da et m_dt
													   //			pv0->m_stitch.m_end.m_cutOut.m_da =  m_ax;
													   //			pv0->m_stitch.m_end.m_cutOut.m_dt = -m_tx;

			//			pv0->m_stitch.m_end.m_cutOut.m_dv = -m_vx;
			//			pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + m_vx) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_jxIn*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;;
		}
		//		pv0->m_stitch.m_end.m_cutOut.m_dt = -pv0->m_stitch.m_end.m_cutOut.m_da / pv0->m_kinXIn.m_j;
	}
	/*else // signifie (ds < ss)
	{
		// Cela ne devrait JAMAIS arriver !!!
		// !!! Cela signifie que le Vstage pv1 est inaccessible !!! Normalement le test de "collision" pr�alable aurait du le detecter ....
		// En debug le test	NErrorIf(ds > s, NERROR_SYSTEM_GURU_MEDITATION);  est effectu� en d�but de fonction ... au cas o�
		// Le backwardovershoot ne peut normalement apparaitre que sur la phase de progression de l'acceleration d'une valeur initiale nulle � sa valeur minimum ( - accel max ).
	}*/
	// --- immediate Stitch End
	pimmediate_end->m_cutOut.m_da = overshoot_acc;
	pimmediate_end->m_cutOut.m_dt = pcutin->m_dt;
	pimmediate_end->m_cutOut.m_dv = -NPOW2(overshoot_acc) / (2.0f * m_jx);

	// stitch_s repr�sente la distance totale couverte par la SShape complete de redemarrage immediat
	stitch_s = (m_v0 - pimmediate_end->m_cutOut.m_dv) * 2.0f * pimmediate_end->m_cutOut.m_dt; //  < 0 car pimmediate_end->m_cutOut.m_dt < 0
	pimmediate_end->m_cutOut.m_ds = stitch_s - pcutin->m_ds;								  //  < 0 [ - - - = - + ]
	//								-		 - -

	pimmediate_end->m_s0Offsets.m_dv = -pimmediate_end->m_cutOut.m_dv * 2.0f;
	pimmediate_end->m_s0Offsets.m_ds = m_s0 - stitch_s - pv0->m_s0;

	// --- stored Stitch End
	if (m_jx == pv0->m_kinXIn.m_j)
	{
		pv0->m_stitch.m_end = *pimmediate_end;
	}
	else
	{
		pv0->m_stitch.m_end.m_cutOut.m_da = overshoot_acc;
		pv0->m_stitch.m_end.m_cutOut.m_dt = -overshoot_acc / pv0->m_kinXIn.m_j;
		pv0->m_stitch.m_end.m_cutOut.m_dv = -NPOW2(overshoot_acc) / (2.0f * pv0->m_kinXIn.m_j);

		// stitch_s = (m_v0 + pimmediate_end->m_cutOut.m_dv)*2.0f*pimmediate_end->m_cutOut.m_dt;
		cutin_dv = -NPOW2(overshoot_acc) / (2.0f * m_jx);
		pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 - cutin_dv - pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_kinXIn.m_j * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 6.0f;

		pv0->m_stitch.m_end.m_s0Offsets.m_dv = -cutin_dv - pv0->m_stitch.m_end.m_cutOut.m_dv;
		pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 - pcutin->m_ds - pv0->m_stitch.m_end.m_cutOut.m_ds - pv0->m_s0;
	}

	//
	// TO BE DELETED ....
	// TO BE DELETED ....
	// TO BE DELETED ....
	// TO BE DELETED ....
	// TO BE DELETED ....
	//

	/*
Nu32 NLSMOTIONSHAPE::backwardOvershoot(NLSOVERSHOOT * povs_in, const Nf32 s0)
{

//	Nf32 ds0s1 = (m_s0 + m_ds) - s0;
	if (s0 <= m_s0) // Il n'y a pas d'overshoot !
	{
		return 0;
	}
	//  --> A partir d'ici on a donc :	s0 > m_s0  .....................ds0s1 < m_ds0s1

	NErrorIf(m_jx >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le backwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "descendante" avec un jerk initial negatif !

	Nf32	s;
	Nf32	dt, tok, sok;
	Nf32	t;
	Nf32	v;
	Nf32	tmp;
	Nf32	sdichotom;

	Nf32	ds = s0 - m_s0; // pour approcher le point d'Overshoot depuis D ( le debut de la S_SHAPE)

	s = m_v0*m_tx + m_sx;

	//  --> Recherche de ds sur le premier Intervalle ]0,tx] / ]0,s].
	//  En effet, gr�ce au test pr�c�dent (voir plus haut) on sait d�j� que [ ds0s1 < m_ds0s1 ] DONC  ds > 0
	//	ds0s1 ( la distance entre le point de d�part et le point d'arriv�e ) est strictement inf�rieur � m_ds0s1 ( la distance totale couverte par la S_SHAPE ),
	//	Pour une raison algorithmique (�limination des "VStages" inaccessibles ) ds ne peut-�tre plus grande que s, car cel� signifierait qu'on peut "passer" le Vstage sans le toucher ...
	//	Donc � partir d'ici, on consid�re que
	//				0 < ds <= s
	if (ds < s)
	{
		dt	= m_tx;
		tok = 0.0f;	// on compte le temps � partir du debut de ce premier Intervalle ]0,tx] / ]0,s]
		t	= 0.0f;
		sok = 0.0f;	// on compte la distance parcourue entre v0 et v1 depuis le d�but du PREMIER intervalle

		do
		{
			sdichotom = m_v0*t + (m_jx*t*t*t) / 6.0f;
			if (sdichotom <= ds)
			{
				tok = t;
				sok = sdichotom;
			}
			dt /= 2.0f;
			t = tok + dt;
		} while ((ds - sok) > 0.005f);

		// On se retrouve ici avec:
		//					'tok'	temps �coul�		depuis le d�but du premier intervalle		( soit par rapport � t0 = 0)
		//					'sok'	distance parcourue	depuis le d�but du premier intervalle		( soit par rapport � s0 = 0 )

		// Consid�rons les points suivants:
		//		D d�but du premier intervalle, soit le d�but de la S_SHAPE
		//		P d�but du troisi�me intervalle
		//		O point d'Overshoot approch� par dichotomie
		//		F fin de la S_SHAPE et donc du troisi�me intervalle
		//		et
		//		G le point de redemarrage "fant�me" situ� dans le pass� de O.
		//
		//		Chacun de ces 4 points est d�fini par un quadruplet (t,a,v,s)

		// CUT IN
		//	 Le CutIn est le vecteur DO, soit la description du point d'Overshoot O par rapport au d�but D de la S_SHAPE.
		//
		//	 Soit O( tok,?,?,sok) le point d'Overshoot et D(0,0,v0,0) le d�but de la S_SHAPE, exprim�s tous les deux par rapport � D ( d�but de la S_SHAPE et donc du premier intervalle. )
		//
		//
		//1)'pcin->m_dt' repr�sente la dur�e s�parant le point d'Overshoot O de D, le d�but de la S_SHAPE (= le d�but du premier intervalle).
		//	 En cons�quence,
		//					pcout->m_dt = DO.t = tok
		//
		//
		//2)'pcin->m_ds' repr�sente la distance alg�brique du point d'Overshoot O par rapport � D le d�but de la S_SHAPE (= le d�but du premier intervalle).
		//	 En cons�quence,
		//					pcout->m_ds = DO.s = sok
		//
		//
		//3)'pcout->m_dv' repr�sente la vitesse alg�brique au point d'Overshoot O par rapport � D le d�but de la S_SHAPE (= le d�but du premier intervalle).
		//					pcout->m_dv = DO.v
		//					pcout->m_ds = O.v - D.v	( O et D exprim�s dans la m�me base, ici par rapport � D )
		//					D.v = 0 ( = v0 dans la base WORLD mais 0 par rapport � D)
		//					O.v, la quantit� de vitesse acquise en O depuis le d�but du premier intervalle vaut:
		//					O.v	= m_jx*tok�/2
		//
		//	 En cons�quence,
		//					pcout->m_dv = DO.v = O.v - D.v
		//					pcout->m_dv = O.v - 0
		//					pcout->m_dv = m_jx*tok�/2

		povs_in->m_dt = tok;
		povs_in->m_dv = m_jx * tok*tok / 2.0f;
		tmp =	(m_v0 + povs_in->m_dv) * 2.0f * povs_in->m_dt;
		povs_in->m_ds = sok - tmp;//sok;
																			// OVERSHOOT OFFSET (Backward)
		// Nous sommes dans la premiere phase de ce Motion Profile, celle avec un Jerk �gal au Jerk initial.
		// Le temps n�c�ssaire pour amen� l'acceleration � son maximum ( +ax ou -ax selon le signe du Jerk ) depuis le point d'overshoot O ( et donc depuis le temps 'tok') vaut:
		//						m_tx - tok.
		// C'est �galement le temps qu'il "faudrait" pour atteindre � nouveau  une acc�l�ration nulle depuis le point d'Overshoot O, en un "point fant�me" situ� "apr�s" O.
		// Ce "point fant�me" est un point fictif, nous l'appelerons G(t,a,v,s).
		//
		// En backwardOvershoot, on recherche m_ds, m_dv, et m_dt  les offsets � ajouter aux donn�es de NLVSTAGEX pour "bien repartir en arri�re" � gauche de l'Overshoot.
		//
		//1)'povs->m_dv' repr�sente la quantit� de vitesse � ajouter � la vitesse du VSTAGE pour obtenir la vitesse au 'point fant�me'.
		//			povs->m_dv repr�sente donc la vitesse alg�brique en G exprim�e par rapport � D ( avec D.v = VSTAGE.v )
		//					povs->m_dv = DG.v
		//					D.v = v0
		//					G.v = v0 + 2*pcin->m_dv
		//					povs->m_dv = G.v - D.v
		//					povs->m_dv = 2*pcin->m_dv
		//
		//
		//2)'povs->m_ds' repr�sente la distance � ajouter � la position S0 du VSTAGE pour obtenir l'abscisse du 'point fant�me' de red�marrage.
		//			povs->m_ds' est obligatoirement positif
		//			Il est important de prendre en compte l'approche par dichotomie du point d'Overshoot dans le calcul de 'povs->m_ds'.
		//			En effet, l'abscisse de ce point d'Overshoot devrait th�oriquement �tre �gale � VSTAGE.m_s0, mais elle est en fait l�g�rement plus petite, car approch�e  par la gauche...
		//			Il convient donc de calculer l'abscisse de ce point fant�me par rapport au d�but de la S_SHAPE dont la position est connue et fiable, puis de calculer la distance entre ce point
		//			et VSTAGE.m_s0.
		//			On notera, que le d�but de la S_Shape est obtenu depuis S1 ! En effet en backward Overshoot, la S_SHAPE est fix�e par sa fin et on remonte le temps ...
		//			La distance s�parant ce point fant�me du d�but de la S_SHAPE vaut:
		//					DG = ( (v0 + v1)/2 ) * T
		//					avec, v0 la vitesse au d�but de la S_SHAPE et v1 la vitesse au point fant�me G.
		//					or,		v1	=	v0 + 2*pcin->m_dv
		//							T	=	2*pcin->m_dt
		//					donc,	DG	=	( ( v0 + 2*pcin->m_dv + v0 )/2 ) * 2*pcin->m_dt
		//							DG	=	( ( 2*v0 + 2*pcin->m_dv )/2 ) * 2*pcin->m_dt
		//							DG	=	( 2*( v0 + pcin->m_dv )/2 ) * 2*pcin->m_dt
		//							DG	=	( v0 + pcin->m_dv ) * 2*pcin->m_dt
		//
		//			On obtient donc G.s l'abscisse en base WORLD du point fant�me de red�marrage:
		//					G.s	= (s1 - m_ds0s1) + DG.
		//			Pour obtenir la distance alg�brique de G par rapport � s0 il suffit de faire:
		//					povs->m_ds	=	G.s - s0
		//					povs->m_ds	=	(s1 - m_ds0s1) + ( v0 + pcin->m_dv ) * 2*pcin->m_dt - s0
		//
		//3)'povs->m_dt' repr�sente la dur�e ( positive!) � ajouter � la "date" du red�marrage depuis le 'point fant�me'.
		//			En effet, pour continuer au-del� de l'overshoot, on red�marre du point fant�me situ� dans le futur proche de l'overshoot et nous permettant
		//			d'atteindre et de d�passer l'overshoot � gauche. En d�marrant du point fantome et en appliquant m_jx sur une dur�e de |povs->m_dt| on se retrouve exactement en O(?,a,v,s) !
		//					povs->m_dt = pcin->m_dt ( et est donc n�gative car il s'agit ici de "remonter le temps" )
		//
		//povs->m_dt = pcin->m_dt;
		//povs->m_dv = 2.0f*pcin->m_dv;						NErrorIf(povs->m_dv > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
		povs_in->m_dsx = m_s0 + tmp - s0;//povs_in->m_dsx = s1 - m_ds + tmp - s0;			//NErrorIf(povs_in->m_dsx < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre >= 0



		return 1;
	}
	// --> Recherche de ds sur le Second Intervalle [tx,tx+tb] / [s,ss]. En effet on sait d�j� que ds <= ss ( gr�ce au test pr�c�dent ... )
	else if (ds == s)
	{
		// Pour rappel on consid�re D,P,O,F,G les points suivants:
		//		D d�but du premier intervalle, soit le d�bu de la S_SHAPE
		//		P d�but du troisi�me intervalle
		//		O point d'Overshoot approch� par dichotomie
		//		F fin de la S_SHAPE et donc du troisi�me intervalle
		//		et
		//		G le point de redemarrage "fant�me"
		//
		// Ici, nous sommes dans le cas particulier o� le point d'Overshoot O coincide avec le point P et donc o� le point G coincide avec le point F !
		// Donc ...
		//
		povs_in->m_dt = m_tx;
		povs_in->m_dv = m_vx;
		tmp = (m_v0 + m_vx) * 2.0f * m_tx;
		povs_in->m_ds = s - tmp;
		// OVERSHOOT OFFSET ( dans ce cas, l'abscisse du Point O d'overshoot est exactement �gale � s0 )
		//povs->m_dt = m_tx;
		//povs->m_dv = 2.0f*m_vx;
		povs_in->m_dsx = m_s0 + tmp - s0;		// NErrorIf(povs_in->m_dsx < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre >= 0
		return 1;
	}
#ifdef _DEBUG
	else
	{
		// Cela ne devrait JAMAIS arriver !!!
		// Le backwardovershoot ne peut normalement apparaitre que sur la phase de progression de l'acceleration d'une valeur initiale nulle � sa valeur minimum ( - accel max ).
	//NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
#endif
	return 0;
}
*/
	/*
	Nu32 NLSMOTIONSHAPE::backwardOvershoot(Nf32 *pt0offset, NLVSTAGEX * pv0, const NLVSTAGEX *pv1)
	{
		// V�rification de la coh�rence de la SShape et des 2 Vstages pass�s en param�tres.
		NErrorIf(pv0->m_v != m_v0, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pv1->m_v != m_v1, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pv1->m_jxIn != m_jx, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pv1->m_axIn != m_ax, NERROR_INCONSISTENT_VALUES);

		NErrorIf(m_jx >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le backwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "descendante" avec un jerk initial negatif !
		NErrorIf(m_ax >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION); // le backwardOvershoot ne peut �tre appliqu� QUE � une SSHAPE "descendante" avec une accel initiale negative !

		//	Nf32 ds0s1 = (m_s0 + m_ds) - s0;
		if (pv0->m_s0 <= m_s0) // Il n'y a pas d'overshoot !
			return 0;
		//  --> A partir d'ici on a donc :	m_s0 < pv0->m_s0

		Nf32	s;
		Nf32	dt, tok, sok;
		Nf32	t;
		Nf32	dv;
		Nf32	tmp;
		Nf32	sdichotom;

		Nf32	ds = pv0->m_s0 - m_s0; // pour approcher le point d'Overshoot depuis le debut de la S_SHAPE

		s = m_v0 * m_tx + m_sx;

		NErrorIf(ds > s, NERROR_SYSTEM_GURU_MEDITATION); // !!! Cela signifie que le Vstage pv0 est inaccessible depuis pv1 ( en bckwd) !!! Normalement le test de "collision" pr�alable aurait du le detecter ....

		//  --> Recherche de ds sur le premier Intervalle ]0,tx] / ]0,s].
		//  En effet, gr�ce au test pr�c�dent (voir plus haut) on sait d�j� que [ ds0s1 < m_ds0s1 ] DONC  ds > 0
		//	ds0s1 ( la distance entre le point de d�part et le point d'arriv�e ) est strictement inf�rieur � m_ds0s1 ( la distance totale couverte par la S_SHAPE ),
		//	Pour une raison algorithmique (�limination des "VStages" inaccessibles ) ds ne peut-�tre plus grande que s, car cel� signifierait qu'on peut "passer" le Vstage sans le toucher ...
		//	Donc � partir d'ici, on consid�re que
		//				0 < ds <= s
		if (ds < s)
		{
			dt = m_tx;
			tok = 0.0f;	// on compte le temps � partir du debut de ce premier Intervalle ]0,tx] / ]0,s]
			t = 0.0f;
			sok = 0.0f;	// on compte la distance parcourue entre v0 et v1 depuis le d�but du PREMIER intervalle

			do
			{
				sdichotom = m_v0 * t + (m_jx*t*t*t) / 6.0f;
				if (sdichotom <= ds)
				{
					tok = t;
					sok = sdichotom;
				}
				dt /= 2.0f;
				t = tok + dt;
			} while ((ds - sok) > 0.005f);

			pv0->m_stitch.m_end.m_cutOut.m_da = m_jx * tok;	NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_da >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0


			NErrorIf(pv0->m_axIn >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
			if (pv0->m_stitch.m_end.m_cutOut.m_da < pv0->m_axIn)// signifie que NABS(...->m_da) > NABS(...->m_axIn) !!! ce qui est MAL !!!
			{
				// 1.0) m_da et t0Offset
				NErrorIf(1, NERROR_SYSTEM_CHECK); // juste pour voir quand cela arrive  !!!
				pv0->m_stitch.m_end.m_cutOut.m_da =  pv0->m_axIn;
				NErrorIf(pv0->m_axIn >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
				t = pv0->m_stitch.m_end.m_cutOut.m_da / m_jx;
				*pt0offset = -t;
				NErrorIf(*pt0offset > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre <= 0

				if (m_jx == pv0->m_jxIn)
				{
					// 2.0) m_dt, m_dv et m_ds
					pv0->m_stitch.m_end.m_cutOut.m_dt = -t;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre <= 0

					pv0->m_stitch.m_end.m_cutOut.m_dv = -m_jx *NPOW2(pv0->m_stitch.m_end.m_cutOut.m_dt) / 2.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dv < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre >= 0

					// dv = m_jx * NPOW2(t) / 2.0f; // v vitesse au point d'overshoot = m_v0 + dv

					s = -(m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv)*2.0f*pv0->m_stitch.m_end.m_cutOut.m_dt;
					NErrorIf(s < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

					/ * note:
						* v  vitesse au point d'overshoot ( = m_v0 + dv)
						* vg vitesse au point de redemarrage
						* on a		v  = vg - jt�/2	|__ avec j negatif
						*			vg = v  + jt�/2 |
						* s distance parcourue du point de redemarrage au point d'overshoot
						* on a		s  = vgt - (jt^3)/6
						*			s  = (v  + jt�/2)t - (jt^3)/6
						*			s  = vt  + (jt^3)/2  - (jt^3)/6
						*			s  = vt  + (jt^3)(1/2 - 1/6)
						*			s  = vt  + (jt^3)(3/6 - 1/6)
						*			s  = vt  + (jt^3)/3
					* /
					pv0->m_stitch.m_end.m_cutOut.m_ds = - s + (m_v0*pv0->m_stitch.m_end.m_cutOut.m_dt + m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 6.0f);
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
					// ou ...
					//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
					//pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
					//NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds != (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f, NERROR_SYSTEM_GURU_MEDITATION);

					// 3.0) m_s0Offsets.m_dv, m_s0Offsets.m_ds
					pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + s - pv0->m_s0;
					pv0->m_stitch.m_end.m_s0Offsets.m_dv = pv0->m_stitch.m_end.m_cutOut.m_dv*2.0f;

					// ou ...
					//pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * t + m_jx * NPOW3(t) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
					//pv0->m_stitch.m_end.m_s0Offsets.m_dv = 2.0f*pv0->m_stitch.m_end.m_cutOut.m_dv;

				}
				else //(m_jx != pv0->m_jxIn)
				{
					// 2.1) m_dt, m_dv et m_ds
					pv0->m_stitch.m_end.m_cutOut.m_dt = -pv0->m_stitch.m_end.m_cutOut.m_da / pv0->m_jxIn;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					pv0->m_stitch.m_end.m_cutOut.m_dv = -pv0->m_jxIn*NPOW2(pv0->m_stitch.m_end.m_cutOut.m_dt) / 2.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dv < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

					dv = m_jx * NPOW2(t) / 2.0f; // v vitesse au point d'overshoot = m_v0 + v

					/ * note:
					 * v  vitesse au point d'overshoot ( = m_v0 + dv)
					 * vg vitesse au point de redemarrage
					 * on a		v  = vg - jt�/2	|__ avec j negatif
					 *			vg = v  + jt�/2 |
					 * s distance parcourue du point de redemarrage au point d'overshoot
					 * on a		s  = vgt - (jt^3)/6
					 *			s  = (v  + jt�/2)t - (jt^3)/6
					 *			s  = vt  + (jt^3)/2  - (jt^3)/6
					 *			s  = vt  + (jt^3)(1/2 - 1/6)
					 *			s  = vt  + (jt^3)(3/6 - 1/6)
					 *			s  = vt  + (jt^3)/3
					* /
					//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
					pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0+dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_jxIn*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					// 3.1) m_s0Offsets.m_dv, m_s0Offsets.m_ds
					pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * t + m_jx * NPOW3(t) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
					pv0->m_stitch.m_end.m_s0Offsets.m_dv = dv + pv0->m_stitch.m_end.m_cutOut.m_dv;
					NErrorIf(pv0->m_stitch.m_end.m_s0Offsets.m_ds < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

				}
			}
			else //(pv0->m_stitch.m_end.m_cutOut.m_da >= pv0->m_axIn) ... attention les 2 sont n�gatifs ...
			{
				// 1.1) m_da et t0Offset
				//			pv0->m_stitch.m_backwardStitch.m_cutOut.m_da ... est valide car  >= pv1->m_axIn
				*pt0offset = -tok;
				NErrorIf(*pt0offset > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre <= 0

				if (m_jx == pv0->m_jxIn)
				{
					// 2.2) m_dt, m_dv et m_ds
					pv0->m_stitch.m_end.m_cutOut.m_dt = -tok;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					pv0->m_stitch.m_end.m_cutOut.m_dv = -m_jx * NPOW2(pv0->m_stitch.m_end.m_cutOut.m_dt) / 2.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dv < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

					s = -(m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv)*2.0f*pv0->m_stitch.m_end.m_cutOut.m_dt;
					NErrorIf(s < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

					pv0->m_stitch.m_end.m_cutOut.m_ds = -( s - sok );
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					// 3.2) m_s0Offsets.m_dv, m_s0Offsets.m_ds
					pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + s - pv0->m_s0;
					pv0->m_stitch.m_end.m_s0Offsets.m_dv = pv0->m_stitch.m_end.m_cutOut.m_dv*2.0f;
				}
				else
				{
					// 2.3) m_dt, m_dv et m_ds
					pv0->m_stitch.m_end.m_cutOut.m_dt = -pv0->m_stitch.m_end.m_cutOut.m_da / pv0->m_jxIn;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dt > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					pv0->m_stitch.m_end.m_cutOut.m_dv = -pv0->m_jxIn*NPOW2(pv0->m_stitch.m_end.m_cutOut.m_dt) / 2.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dv < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0

					dv = m_jx * NPOW2(t) / 2.0f;
					//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
					pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0+dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_jxIn*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
					NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

					// 3.?) m_s0Offsets.m_dv, m_s0Offsets.m_ds
					pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * tok + m_jx * NPOW3(tok) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
					pv0->m_stitch.m_end.m_s0Offsets.m_dv = dv + pv0->m_stitch.m_end.m_cutOut.m_dv;
				}
			}
			return 1;
		}
		// --> Recherche de ds sur le Second Intervalle [tx,tx+tb] / [s,ss]. En effet on sait d�j� que ds <= ss ( gr�ce au test pr�c�dent ... )
		else if (ds == s)
		{

			if (m_ax < pv0->m_axIn) // m_ax est n�gatif
			{
				pv0->m_stitch.m_end.m_cutOut.m_da = m_jx * tok;	NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_da >= 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre < 0
				pv0->m_stitch.m_end.m_s0Offsets.m_ds =
				pv0->m_stitch.m_end.m_s0Offsets.m_dv =

			}
			else
			{
				pv0->m_stitch.m_end.m_cutOut.m_dt = -m_tx;
				pv0->m_stitch.m_end.m_cutOut.m_da =  m_ax;
				pv0->m_stitch.m_end.m_cutOut.m_dv = -m_vx;
				pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + m_vx) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_jxIn*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;;

				pv0->m_stitch.m_end.m_s0Offsets.m_ds =
				pv0->m_stitch.m_end.m_s0Offsets.m_dv =
			}







			// 1.2) m_da et t0Offset
			// 2.4) m_dt, m_dv et m_ds
			// 3.1) m_s0Offsets.m_dv, m_s0Offsets.m_ds


			// Pour rappel on consid�re D,P,O,F,G les points suivants:
			//		D d�but du premier intervalle, soit le d�bu de la S_SHAPE
			//		P d�but du troisi�me intervalle
			//		O point d'Overshoot approch� par dichotomie
			//		F fin de la S_SHAPE et donc du troisi�me intervalle
			//		et
			//		G le point de redemarrage "fant�me"
			//
			// Ici, nous sommes dans le cas particulier o� le point d'Overshoot O coincide avec le point P et donc o� le point G coincide avec le point F !
			// Donc ...
			//
			povs_in->m_dt = m_tx;
			povs_in->m_dv = m_vx;
			tmp = (m_v0 + m_vx) * 2.0f * m_tx;
			povs_in->m_ds = s - tmp;
			// OVERSHOOT OFFSET ( dans ce cas, l'abscisse du Point O d'overshoot est exactement �gale � s0 )
			//povs->m_dt = m_tx;
			//povs->m_dv = 2.0f*m_vx;
			povs_in->m_dsx = m_s0 + tmp - s0;		// NErrorIf(povs_in->m_dsx < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);// devrait �tre >= 0



			return 1;
		}
	#ifdef _DEBUG
		else
		{
			// Cela ne devrait JAMAIS arriver !!!
			// Le backwardovershoot ne peut normalement apparaitre que sur la phase de progression de l'acceleration d'une valeur initiale nulle � sa valeur minimum ( - accel max ).
		//NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
		}
	#endif
		return 0;
	}
	*/

	/*
	// 2.0) m_dv et m_ds
	pv0->m_stitch.m_end.m_cutOut.m_dv = -NPOW2(pv0->m_stitch.m_end.m_cutOut.m_da) / (2.0f*pv0->m_kinXIn.m_j);
	NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_dv < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0
	// s distance parcourue lors de la variation de a � 0 pour trouver le point de Re-demarrage ...
	// ! m_dv est positif et m_dt est n�gatif !
	s = -(m_v0 - pv0->m_stitch.m_end.m_cutOut.m_dv)*2.0f*pv0->m_stitch.m_end.m_cutOut.m_dt;
	NErrorIf(s < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0
	pv0->m_stitch.m_end.m_cutOut.m_ds = -s - (m_v0 *pv0->m_stitch.m_end.m_cutOut.m_dt + m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 6.0f);
	NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
	// ou ...
	//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
	//pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
	//NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
	//NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds != (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f, NERROR_SYSTEM_GURU_MEDITATION);
	// 4.0) m_s0Offsets.m_dv, m_s0Offsets.m_ds
	pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + s - pv0->m_s0;
	pv0->m_stitch.m_end.m_s0Offsets.m_dv = -pv0->m_stitch.m_end.m_cutOut.m_dv*2.0f;

	// ou ...
	//pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * t + m_jx * NPOW3(t) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
	//pv0->m_stitch.m_end.m_s0Offsets.m_dv = 2.0f*pv0->m_stitch.m_end.m_cutOut.m_dv;
	*/

	/*
	// old version avec Jerk variable
	if (m_jx == pv0->m_jxIn)
	{

		s = -(m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv)*2.0f*pv0->m_stitch.m_end.m_cutOut.m_dt;
		NErrorIf(s < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0
		pv0->m_stitch.m_end.m_cutOut.m_ds = -s + (m_v0*pv0->m_stitch.m_end.m_cutOut.m_dt + m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 6.0f);
		NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
		// ou ...
		//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
		//pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
		//NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0
		NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds != (m_v0 + pv0->m_stitch.m_end.m_cutOut.m_dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - m_jx * NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f, NERROR_SYSTEM_GURU_MEDITATION);

		// 3.0) m_s0Offsets.m_dv, m_s0Offsets.m_ds
		pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + s - pv0->m_s0;
		pv0->m_stitch.m_end.m_s0Offsets.m_dv = pv0->m_stitch.m_end.m_cutOut.m_dv*2.0f;

		// ou ...
		//pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * t + m_jx * NPOW3(t) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
		//pv0->m_stitch.m_end.m_s0Offsets.m_dv = 2.0f*pv0->m_stitch.m_end.m_cutOut.m_dv;

	}
	else //(m_jx != pv0->m_jxIn)
	{
		dv = m_jx * NPOW2(t) / 2.0f; // v vitesse au point d'overshoot = m_v0 + v
		/ *note:
		 *v  vitesse au point d'overshoot ( = m_v0 + dv)
		 * vg vitesse au point de redemarrage
		 * on a		v = vg - jt� / 2 | __ avec j negatif
		 *			vg = v + jt� / 2 |
		 *s distance parcourue du point de redemarrage au point d'overshoot
		 * on a		s = vgt - (jt ^ 3) / 6
		 * s = (v + jt� / 2)t - (jt ^ 3) / 6
		 * s = vt + (jt ^ 3) / 2 - (jt ^ 3) / 6
		 * s = vt + (jt ^ 3)(1 / 2 - 1 / 6)
		 *			s = vt + (jt ^ 3)(3 / 6 - 1 / 6)
		 *			s = vt + (jt ^ 3) / 3
		 * /
		//[				NEGATIF			  ] = [		NEGATIF car ...m_dt n�gatif		] -	[	 POSITIF car m_jxIn negatif		et ... m_dt negatif	 	]
		pv0->m_stitch.m_end.m_cutOut.m_ds = (m_v0 + dv) * pv0->m_stitch.m_end.m_cutOut.m_dt - pv0->m_jxIn*NPOW3(pv0->m_stitch.m_end.m_cutOut.m_dt) / 3.0f;
		NErrorIf(pv0->m_stitch.m_end.m_cutOut.m_ds > 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre <= 0

		// 3.1) m_s0Offsets.m_dv, m_s0Offsets.m_ds
		pv0->m_stitch.m_end.m_s0Offsets.m_ds = m_s0 + (m_v0 * t + m_jx * NPOW3(t) / 6.0f - pv0->m_stitch.m_end.m_cutOut.m_ds) - pv0->m_s0;
		pv0->m_stitch.m_end.m_s0Offsets.m_dv = dv + pv0->m_stitch.m_end.m_cutOut.m_dv;
		NErrorIf(pv0->m_stitch.m_end.m_s0Offsets.m_ds < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);	// devrait �tre >= 0
	}
	*/
	return 1;
}

#ifdef _NEDITOR
// SMOTION PROFILE NCOLORPICKPACK index ventilation:
// NCOLORPICKPACK.#0....... Flat Motion Shape color					NCOLORPICKPACK.#3....... S Motion Shape color phase 3
// NCOLORPICKPACK.#1....... S Motion Shape color phase 1			NCOLORPICKPACK.#4....... S Motion Shape color 'Out'
// NCOLORPICKPACK.#2....... S Motion Shape color phase 2			NCOLORPICKPACK.#5....... F et S Motion Shape Text color

// temporaire !!!!
// temporaire !!!!
/*
void NLSMOTIONSHAPE::drawVs(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack, const Nf32 t0_offset)
{
	NLSCUTOUTOFFSETS ovs;
	ovs.null();
	drawVs(p2docs, pickpack, t0_offset, &ovs);
}
void NLSMOTIONSHAPE::drawVs(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack, const NLSCUTOUTOFFSETS * pcut_out_overshoot)
{

	drawVs(p2docs, pickpack, 0.0f, pcut_out_overshoot);
}
*/
// temporaire !!!!
// temporaire !!!!

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Trace une S motion shape dans un 2D Ocs.
 *			Utilise un NCOLORPICKPACK pour d�finir les couleurs � utiliser parmis celles de la palette
 *			du NL2DOCS.
 *
 *	@param	p2docs
 *	@param	t0_offset
 *	@param	pcut_out_overshoot
 *	@param	pickpack				complete description des couleurs (6) utilis�es pour dessinner.
 *									comme d'habitude, la valeur de PickPack peut-�tre 0.
 *									dans ce cas, les couleurs 0 � 6 de la palette de l'OCS seront
 *									utilis�es par d�faut.
 *
 *									NCOLORPICKPACK.#0....... S Motion Shape color phase 1			NCOLORPICKPACK.#3....... S Motion Shape color 'Out'
 *									NCOLORPICKPACK.#1....... S Motion Shape color phase 2			NCOLORPICKPACK.#4....... Flat Motion Shape color
 *									NCOLORPICKPACK.#2....... S Motion Shape color phase 3			NCOLORPICKPACK.#5....... F et S Motion Shape Text color
 *
 */
// ------------------------------------------------------------------------------------------
void NLSMOTIONSHAPE::drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 t0_offset, const NLSCUTOUTOFFSETS *pcut_out_overshoot)
{
	Nu32 accuracy;
	Nf32 dt = CONSTANT_NLSMOTIONSHAPE_DT_ACCURACY;
	Nf32 t, t2, v, s;

	NUTDRAWVERTEX va;
	NUTDRAWVERTEX vb;
	NUTDRAWVERTEX vc;
	NUTDRAWVERTEX vout;

	va.Position_3f.z = 0.0f;
	vb.Position_3f.z = 0.0f;
	vc.Position_3f.z = 0.0f;
	vout.Position_3f.z = 0.0f;

	va.Color0_4f = *p2docs->getColor(0, pickpack);
	vb.Color0_4f = *p2docs->getColor(1, pickpack);
	vc.Color0_4f = *p2docs->getColor(2, pickpack);
	vout.Color0_4f = *p2docs->getColor(3, pickpack);

	Nu32 i;

	NVEC2 xt = {0.005f, 0.005f};
	/*
	NVEC3			v3;
	v3.z = 0.0f;
	*/

	// 1/3)
	if (t0_offset)
	{
		vout.Position_3f.x = p2docs->transformX(m_s0);
		vout.Position_3f.y = p2docs->transformY(m_v0);
		NUT_DrawPencil_From(&vout);
	}
	else
	{
		va.Position_3f.x = p2docs->transformX(m_s0);
		va.Position_3f.y = p2docs->transformY(m_v0);
		NUT_DrawPencil_From(&va);
	}

	accuracy = (Nu32)ceil(m_tx / dt);
	// NErrorIf(accuracy == 0, NERROR_NULL_VALUE);

	for (i = 0; i <= accuracy; i++)
	{
		// time
		t = m_tx * (Nf32)i / (Nf32)accuracy;
		t2 = t * t;

		if (t < t0_offset)
		{
			vout.Position_3f.x = p2docs->transformX(m_s0 + m_v0 * t + m_jx * t2 * t / 6.0f); // x = x0 + v0 * t + 1/2 * a0 * t^2 + 1/6 * j * t^3
			vout.Position_3f.y = p2docs->transformY(m_v0 + m_jx * t2 / 2.0f);				 // v = v0 + a0 * t + 1/2 * j * t^2
			NUT_DrawPencil_LineTo(&vout);
		}
		else
		{
			va.Position_3f.x = p2docs->transformX(m_s0 + m_v0 * t + m_jx * t2 * t / 6.0f); // x = x0 + v0 * t + 1/2 * a0 * t^2 + 1/6 * j * t^3
			va.Position_3f.y = p2docs->transformY(m_v0 + m_jx * t2 / 2.0f);				   // v = v0 + a0 * t + 1/2 * j * t^2
			NUT_DrawPencil_LineTo(&va);
		}

		/*
		v3.x = p2docs->transformX(m_s0 + m_v0 * t + m_jx * t2*t / 6.0f);
		v3.y = p2docs->transformY(m_v0 + m_jx * t2 / 2.0f);
		NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);
		*/
	}
	// 2/3)
	if (m_tb)
	{
		accuracy = (Nu32)ceil(m_tb / dt);
		NErrorIf(accuracy == 0, NERROR_NULL_VALUE);

		s = m_s0 + m_v0 * m_tx + m_sx;
		v = m_v0 + m_vx;

		for (i = 0; i <= accuracy; i++)
		{
			// time
			t = m_tb * (Nf32)i / (Nf32)accuracy;
			t2 = t * t;

			vb.Position_3f.x = p2docs->transformX(s + v * t + m_ax * t2 / 2.0f); // x = x0 + v0 * t + 1/2 * a0 * t^2 + 1/6 * j * t^3
			vb.Position_3f.y = p2docs->transformY(v + m_ax * t);				 // v = v0 + a0 * t + 1/2 * j * t^2
			NUT_DrawPencil_LineTo(&vb);

			/*
			v3.x = p2docs->transformX(s + v * t + m_ax * t2 / 2.0f);
			v3.y = p2docs->transformY(v + m_ax * t);
			NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);
			*/
		}
	}

	// 3/3)
	accuracy = (Nu32)ceil(m_tx / dt);
	// NErrorIf(accuracy == 0, NERROR_NULL_VALUE);

	s = m_s0 + m_ds - m_v1 * m_tx + m_sx;
	v = m_v1 - m_vx;

	for (i = 0; i <= accuracy; i++)
	{
		// time
		t = m_tx * (Nf32)i / (Nf32)accuracy;
		t2 = t * t;
		if (t > m_tx - pcut_out_overshoot->m_dt)
		{
			vout.Position_3f.x = p2docs->transformX(s + v * t + m_ax * t2 / 2.0f - m_jx * t2 * t / 6.0f); // x = x0 + v0 * t + 1/2 * a0 * t^2 + 1/6 * j * t^3
			vout.Position_3f.y = p2docs->transformY(v + m_ax * t - m_jx * t2 / 2.0f);					  // v = v0 + a0 * t + 1/2 * j * t^2
			NUT_DrawPencil_LineTo(&vout);
		}
		else
		{
			vc.Position_3f.x = p2docs->transformX(s + v * t + m_ax * t2 / 2.0f - m_jx * t2 * t / 6.0f); // x = x0 + v0 * t + 1/2 * a0 * t^2 + 1/6 * j * t^3
			vc.Position_3f.y = p2docs->transformY(v + m_ax * t - m_jx * t2 / 2.0f);						// v = v0 + a0 * t + 1/2 * j * t^2
			NUT_DrawPencil_LineTo(&vc);
		}

		/*
		v3.x = p2docs->transformX(s + v * t + m_ax * t2 / 2.0f - m_jx * t2*t / 6.0f);
		v3.y = p2docs->transformY(v + m_ax * t - m_jx * t2 / 2.0f);
		NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);
		*/
	}
}

// ----------------------------------------- TO BE DELETED
// ----------------------------------------- TO BE DELETED
void NLSMOTIONSHAPE::drawTangents(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 pnt, const Nf32 axout)
{

	NUTDRAWVERTEX va;
	va.Position_3f.z = 0.0f;
	NVEC2 o;
	Nf32 p0, p1, vt, st, tgt_s0;
	Nf32 delta, A, B, C, t1, t2;
	Nf32 tgt_halflength = 0.25f;
	NVEC2 xt = {0.005f, 0.005f};
	NVEC3 v3;
	v3.z = 0.0f;
	Nchar txt[32];
	NUT_DRAWTEXT drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_ELECTRIC, 1);

	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	if (m_v0 > m_v1)
	{
		// az acceleration constante dans un mod�le trapezoidale
		// az = (m_v1*m_v1 - m_v0 * m_v0) / (2.0f*ds);

		if (NABS(axout) < NF32_MAX)
		{
			NLSMOTIONSHAPE adjusted_shape;
			NLKINLIMITS adjusted_limits(m_v0, NABS(axout), NABS(m_jx));
			NLSCUTOUTOFFSETS ovs;
			ovs.null();

			adjusted_shape.backwardBuildDown(&adjusted_limits, m_s0 + m_ds, m_v0, m_v1);
			adjusted_shape.drawVs(p2docs, pickpack, 0.0f, &ovs);
			v3.x = p2docs->transformX(adjusted_shape.m_s0 + adjusted_shape.m_ds * 0.5f);
			v3.y = p2docs->transformY((adjusted_shape.m_v0 + adjusted_shape.m_v1) / 2.0f);

			sprintf(txt, "adj.az ( %.4f )", axout);
			NUT_Draw_Text(txt, &v3, &drawtxt);
		}
	}
	if (m_v0 < m_v1)
	{
		Nf32 a1, a2;
		// az acceleration constante dans un mod�le trapezoidale
		// az = (m_v1*m_v1 - m_v0 * m_v0) / (2.0f*ds);

		NLSMOTIONSHAPE adjusted_shape;
		NLKINLIMITS adjusted_limits(m_v1, axout, m_jx);
		NLSCUTOUTOFFSETS ovs;
		ovs.null();

		adjusted_shape.forwardBuildUp(&adjusted_limits, m_s0, m_v0, m_v1);
		adjusted_shape.drawVs(p2docs, pickpack, 0.0f, &ovs);
		v3.x = p2docs->transformX(adjusted_shape.m_s0 + adjusted_shape.m_ds * 0.5f);
		v3.y = p2docs->transformY((adjusted_shape.m_v0 + adjusted_shape.m_v1) / 2.0f);

		sprintf(txt, "adj.az ( %.4f )", axout);
		NUT_Draw_Text(txt, &v3, &drawtxt);

		/*
		// Tests "Ajusted S-SHAPE"
		// pnt en tant que P1
		delta = 1 + (2.0f*m_v1*pnt*pnt) / m_jx;
		a1 = -(1 - sqrt(delta))*m_jx / pnt;
		a2 = -(1 + sqrt(delta))*m_jx / pnt;
		// on choisi a1 car a2 sera tjrs negatif !
		if (a1 > 0.0f)
		{
			adjusted_limits.setAccelerationMax(a1);

			adjusted_shape.forwardBuildUp(&adjusted_limits, m_s0, m_v0, m_v1);
			adjusted_shape.drawVs(p2docs, 0.0f);

			v3.x = p2docs->transformX(adjusted_shape.m_s0 + adjusted_shape.m_ds*0.5f);
			v3.y = p2docs->transformY((adjusted_shape.m_v0 + adjusted_shape.m_v1)/2.0f);

			sprintf(txt, "adj.a ( %.4f )( %.4f )", a1,az);
			NUT_Draw_Text(txt, &v3, &drawtxt);

		}
		*/

		// pnt en tant que P0
		delta = 1 - (2.0f * m_v0 * pnt * pnt) / m_jx;
		if (delta >= 0)
		{
			a1 = -(1 - sqrt(delta)) * m_jx / pnt;
			a2 = -(1 + sqrt(delta)) * m_jx / pnt;
			// on choisi a1 car a2 sera tjrs negatif !
			if (a1 > 0.0f)
			{
				NLSMOTIONSHAPE adjusted_shape;
				NLKINLIMITS adjusted_limits(m_v1, a1, m_jx);
				NLSCUTOUTOFFSETS ovs;
				ovs.null();

				adjusted_shape.forwardBuildUp(&adjusted_limits, m_s0, m_v0, m_v1);
				adjusted_shape.drawVs(p2docs, pickpack, 0.0f, &ovs);
			}
		}
	}
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	// Tangentes:
	// T0:
	p0 = m_ax / (m_v0 + m_vx);
	// T1:
	p1 = m_ax / (m_v1 - m_vx);

	// Specific tangente avec une pente pnt
	// Recherche de l'intervalle ou pourrait se trouver cette tangente:
	// Notes:
	//
	// Pente de la sshape v(s):		0  <   p0    >	 p1			  >	  0
	//
	// Temps:						0 ---> m_tx ---> m_tx + m_tb ---> 2*m_tx + m_tb
	// du coup:						0 < p1 < p0

	if (pnt < p0)
	{
		// T0
		o.x = m_s0 + m_v0 * m_tx + m_sx;
		o.y = m_v0 + m_vx;

		NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_WHITE, 1.0f);

		v3.x = p2docs->transformX(o.x);
		v3.y = p2docs->transformY(o.y);
		NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

		va.Position_3f.x = p2docs->transformX(o.x + tgt_halflength);
		va.Position_3f.y = p2docs->transformY(o.y + p0 * tgt_halflength);
		NUT_DrawPencil_From(&va);
		va.Position_3f.x = p2docs->transformX(o.x - tgt_halflength);
		va.Position_3f.y = p2docs->transformY(o.y - p0 * tgt_halflength);
		NUT_DrawPencil_LineTo(&va);
		// T1
		o.x = m_s0 + m_ds - m_v1 * m_tx + m_sx;
		o.y = m_v1 - m_vx;

		NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_WHITE, 1.0f);

		v3.x = p2docs->transformX(o.x);
		v3.y = p2docs->transformY(o.y);
		NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

		va.Position_3f.x = p2docs->transformX(o.x + tgt_halflength);
		va.Position_3f.y = p2docs->transformY(o.y + p1 * tgt_halflength);
		NUT_DrawPencil_From(&va);
		va.Position_3f.x = p2docs->transformX(o.x - tgt_halflength);
		va.Position_3f.y = p2docs->transformY(o.y - p1 * tgt_halflength);
		NUT_DrawPencil_LineTo(&va);
		if (pnt >= p1)
		{
			// recherche sur la phase 2
			t1 = (m_ax / pnt - m_v0 - m_vx) / m_ax;
			vt = m_v0 + m_vx + m_ax * t1;
			st = m_s0 + m_v0 * m_tx + m_sx + (m_v0 + m_vx) * t1 + m_ax * t1 * t1 / 2.0f;

			tgt_s0 = (m_v0 - vt + pnt * st) / pnt;

			if ((tgt_s0 - m_s0) < 0.0f)
			{
				NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_RED, 1.0f);
			}
			else
			{
				NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_GREEN, 1.0f);
			}
			v3.x = p2docs->transformX(tgt_s0);
			v3.y = p2docs->transformY(m_v0);
			NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

			v3.x = p2docs->transformX(st);
			v3.y = p2docs->transformY(vt);
			NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

			va.Position_3f.x = p2docs->transformX(st + tgt_halflength);
			va.Position_3f.y = p2docs->transformY(vt + pnt * tgt_halflength);
			NUT_DrawPencil_From(&va);
			va.Position_3f.x = p2docs->transformX(st - tgt_halflength);
			va.Position_3f.y = p2docs->transformY(vt - pnt * tgt_halflength);
			NUT_DrawPencil_LineTo(&va);
		}
		else //(pnt < p1)
		{
			// recherche sur phase 3:
			A = -pnt * m_jx / 2.0f;
			B = pnt * m_ax + m_jx;
			C = pnt * (m_v1 - m_vx) - m_ax;
			delta = B * B - 4.0f * A * C;
			if (delta >= 0.0f)
			{
				t2 = (-B + sqrt(delta)) / (-pnt * m_jx); // >> the good one
				t1 = (-B - sqrt(delta)) / (-pnt * m_jx);

				if (t1 >= 0.0f)
				{
					st = m_s0 + m_ds - m_v1 * m_tx + m_sx;
					vt = m_v1 - m_vx;
					st += (m_v1 - m_vx) * t1 + m_ax * t1 * t1 / 2.0f - m_jx * t1 * t1 * t1 / 6.0f;
					vt += m_ax * t1 - m_jx * t1 * t1 / 2.0f;

					tgt_s0 = (m_v0 - vt + pnt * st) / pnt;

					if ((tgt_s0 - m_s0) < 0.0f)
					{
						NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_RED, 1.0f);
					}
					else
					{
						NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_GREEN, 1.0f);
					}
					v3.x = p2docs->transformX(tgt_s0);
					v3.y = p2docs->transformY(m_v0);
					NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

					v3.x = p2docs->transformX(st);
					v3.y = p2docs->transformY(vt);
					NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

					va.Position_3f.x = p2docs->transformX(st + tgt_halflength);
					va.Position_3f.y = p2docs->transformY(vt + pnt * tgt_halflength);
					NUT_DrawPencil_From(&va);
					va.Position_3f.x = p2docs->transformX(st - tgt_halflength);
					va.Position_3f.y = p2docs->transformY(vt - pnt * tgt_halflength);
					NUT_DrawPencil_LineTo(&va);
				}

				if (t2 >= 0.0f)
				{
					st = m_s0 + m_ds - m_v1 * m_tx + m_sx;
					vt = m_v1 - m_vx;
					st += (m_v1 - m_vx) * t2 + m_ax * t2 * t2 / 2.0f - m_jx * t2 * t2 * t2 / 6.0f;
					vt += m_ax * t2 - m_jx * t2 * t2 / 2.0f;

					tgt_s0 = (m_v0 - vt + pnt * st) / pnt;

					if ((tgt_s0 - m_s0) < 0.0f)
					{
						NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_ORANGE, 1.0f);
					}
					else
					{
						NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_BLUE_BABY, 1.0f);
					}
					v3.x = p2docs->transformX(tgt_s0);
					v3.y = p2docs->transformY(m_v0);
					NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

					v3.x = p2docs->transformX(st);
					v3.y = p2docs->transformY(vt);
					NUT_Draw_Mark(&v3, &xt, &va.Color0_4f);

					va.Position_3f.x = p2docs->transformX(st + tgt_halflength);
					va.Position_3f.y = p2docs->transformY(vt + pnt * tgt_halflength);
					NUT_DrawPencil_From(&va);
					va.Position_3f.x = p2docs->transformX(st - tgt_halflength);
					va.Position_3f.y = p2docs->transformY(vt - pnt * tgt_halflength);
					NUT_DrawPencil_LineTo(&va);
				}
			}
		}
		//	else if (pnt <= p0)
		{
		}
	}
	//	else if (pnt < 0.0f)
	{
		if (pnt <= p1)
		{
		}
		else if (pnt <= p0)
		{
		}
	}
	//	else // pnt == 0.0f
	{
	}

	/*	//
		NVEC3f32 vt;
		Nchar txt[32];
		NUT_DRAWTEXT	drawtxt;
		Nmem0(&drawtxt, NUT_DRAWTEXT);
		drawtxt.Size = 0.01f;
		NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_WHITE, 1);

		vt.z = 0.0f;
		vt.x = p2docs->transformX(o.x);
		vt.y = p2docs->transformY(o.y);
		sprintf(txt, " Tgt:( %.4f )", p);
		NUT_Draw_Text(txt, &vt, &drawtxt);
	*/
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
}
// ----------------------------------------- TO BE DELETED
// ----------------------------------------- TO BE DELETED

#endif

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Initialise le kin courant du buffer � ZERO et ins�re les Kins associ�s � S-SHAPE.
 *			Le premier Kin
 *	@param	pkbuff pointeur sur le buffer de Kins.
 *	@return	Nombre de Kin ins�r�s dans le buffer
 */
// ------------------------------------------------------------------------------------------
/*
Nu32 NLSMOTIONSHAPE::developInKinBuffer(NLKIN *pkbuff)
{
	NLKIN *pka = pkbuff + 1;
	NLKIN *pkb;

	pkbuff->m_j = 0.0f;			pkbuff->m_a = 0.0f;		pkbuff->m_v = m_v0;				pkbuff->m_s = 0.0f;										pkbuff->m_t = 0.0f;
	pka->m_j = m_jx;			pka->m_a = m_ax;		pka->m_v = m_v0 + m_vx;			pka->m_s = m_sx + m_v0 * m_tx;							pka->m_t = m_tx;		pkb = pka + 1;
	if (m_tb)
	{
		pkb->m_j = 0.0f;		pkb->m_a = m_ax;		pkb->m_v = m_v1 - m_vx;			pkb->m_s = pka->m_s + (m_v0 + m_v1)*m_tb / 2.0f;		pkb->m_t = pka->m_t + m_tb;	pka = pkb + 1;
		pka->m_j = -m_jx;		pka->m_a = 0.0f;		pka->m_v = m_v1;				pka->m_s = / *pkbuff->m_s +* / m_s;						pka->m_t = pkb->m_t + m_tx;
		return 4;
	}
	else
	{
		pkb->m_j = -m_jx;		pkb->m_a = 0.0f;		pkb->m_v = m_v1;				pkb->m_s = / *pkbuff->m_s +* / m_s;						pkb->m_t = pka->m_t + m_tx;
		return 3;
	}
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Prolonge le KinBuffer en y ins�rant les Kins associ�s � la S-SHAPE.
 *			Le premier Kin ins�r� est cr�� � partir du dernier Kin du buffer. Ce qui suppose que le buffer contient au moins un premier KIN initialis� correctement.
 *	@param	pkbuff pointeur sur le buffer de Kins.
 *	@return	Nombre de Kin ins�r�s dans le buffer
 */
// ------------------------------------------------------------------------------------------
/*
Nu32 NLSMOTIONSHAPE::extendIntoKinBuffer(NLKIN *pkbuff)
{
	NErrorIf((pkbuff->m_a) || (pkbuff->m_v != m_v0), NERROR_INCONSISTENT_VALUES);
	NLKIN *pka = pkbuff + 1;
	NLKIN *pkb;

	pka->m_j = m_jx;			pka->m_a = m_ax;		pka->m_v = m_v0 + m_vx;		pka->m_s = pkbuff->m_s + m_sx + m_v0 * m_tx;				pka->m_t = pkbuff->m_t + m_tx;		pkb = pka + 1;
	if (m_tb)
	{
		pkb->m_j = 0.0f;		pkb->m_a = m_ax;		pkb->m_v = m_v1 - m_vx;		pkb->m_s = pka->m_s + (m_v0 + m_v1)*m_tb / 2.0f;			pkb->m_t = pka->m_t + m_tb;	pka = pkb + 1;
		pka->m_j = -m_jx;		pka->m_a = 0.0f;		pka->m_v = m_v1;			pka->m_s = pkbuff->m_s + m_s;								pka->m_t = pkb->m_t + m_tx;
		return 3;
	}
	else
	{
		pkb->m_j = -m_jx;		pkb->m_a = 0.0f;		pkb->m_v = m_v1;			pkb->m_s = pkbuff->m_s + m_s;								pkb->m_t = pka->m_t + m_tx;
		return 2;
	}
}
*/
/*
Nu32 NLSMOTIONSHAPE::cutInAndExtendIntoKinBuffer(NLKIN *pkbuff, NLSOVERSHOOT *povs)
{
	NErrorIf((pkbuff->m_a != povs->m_a) || (pkbuff->m_v != m_v0 + povs->m_v), NERROR_INCONSISTENT_VALUES);
	NLKIN *pka;
	NLKIN *pkb;

	if (povs->m_t == m_tx)
	{
		pka = pkbuff;
		pkb = pka + 1;
	}
	else
	{
		pka = pkbuff + 1;
		pka->m_j = m_jx;	pka->m_a = m_ax;	pka->m_v = m_v0 + m_vx - povs->m_v;	pka->m_s = pkbuff->m_s + m_sx + m_v0 * m_tx - povs->m_s;	pka->m_t = pkbuff->m_t + m_tx - povs->m_t;	pkb = pka + 1;
	}

	if (m_tb)
	{
		pkb->m_j = 0.0f;	pkb->m_a = m_ax;	pkb->m_v = m_v1 - m_vx;				pkb->m_s = pka->m_s + (m_v0 + m_v1)*m_tb / 2.0f;			pkb->m_t = pka->m_t + m_tb;					pka = pkb + 1;
		pka->m_j = -m_jx;	pka->m_a = 0.0f;	pka->m_v = m_v1;					pka->m_s = pkbuff->m_s + m_s;								pka->m_t = pkb->m_t + m_tx;
		return 3;
	}
	else
	{
		pkb->m_j = -m_jx;	pkb->m_a = 0.0f;	pkb->m_v = m_v1;					pkb->m_s = pkbuff->m_s + m_s;								pkb->m_t = pka->m_t + m_tx;
		return 2;
	}
}

Nu32 NLSMOTIONSHAPE::cutOutAndExtendIntoKinBuffer(NLKIN *pkbuff, NLSOVERSHOOT *povs)
{
	NErrorIf((pkbuff->m_a) || (pkbuff->m_v != m_v0), NERROR_INCONSISTENT_VALUES);
	NLKIN *pka;
	NLKIN *pkb;

			pka->m_j = m_jx;	pka->m_a = m_ax;		pka->m_v = m_v0 + m_vx;			pka->m_s = pkbuff->m_s + m_sx + m_v0 * m_tx;		pka->m_t = pkbuff->m_t + m_tx;		pkb = pka + 1;


	if (m_tb)
	{
			pkb->m_j = 0.0f;	pkb->m_a = m_ax;		pkb->m_v = m_v1 - m_vx;			pkb->m_s = pka->m_s + (m_v0 + m_v1)*m_tb / 2.0f;	pkb->m_t = pka->m_t + m_tb;			pka = pkb + 1;

			if (povs->m_t + m_tx) // povs->m_t <= 0 car il est exprim� par rapport � la fin de la S.Shape ( t1 ). Donc si povs->m_t = -m_tx cela revient � dire qu'il n'y a pas de derni�re phase !
			{
				pka->m_j = -m_jx;	pka->m_a = 0.0f;		pka->m_v = m_v1;				pka->m_s = pkbuff->m_s + m_s;						pka->m_t = pkb->m_t + m_tx;
				return 3;
			}
			else
				return 2;
	}
	else
	{
		if (povs->m_t + m_tx) // povs->m_t <= 0 car il est exprim� par rapport � la fin de la S.Shape ( t1 ). Donc si povs->m_t = -m_tx cela revient � dire qu'il n'y a pas de derni�re phase !
		{
			pkb->m_j = -m_jx;	pkb->m_a = -povs->m_a;	pkb->m_v = m_v1 - povs->m_v;	pkb->m_s = pkbuff->m_s + m_s - povs->m_s;			pkb->m_t = pka->m_t + m_tx;
			return 2;
		}
		else
			return 1;
	}
}
*/
