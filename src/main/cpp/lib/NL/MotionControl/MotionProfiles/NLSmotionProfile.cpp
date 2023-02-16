
#include "../../../N/NMath.h"
#include "../NLKin.h"
#include "../NLKinLimits.h"
#include "NLSMotionProfile.h"
/*
void  NLSMOTIONPROFILE::fPrime(const Nf32 s0, const Nf32 s1, const Nf32 v)
{
	m_fPrime.forwardVs(s0, v, s1 - s0);

	m_id |= NLSMOTIONPROFILE_ID_BIT_FPRIM;
}
void  NLSMOTIONPROFILE::resetFprime()
{
	m_fPrime.null();

	m_id &= ~NLSMOTIONPROFILE_ID_BIT_FPRIM;
}
*/
void  NLSMOTIONPROFILE::flat(const Nf32 s0, const Nf32 s1, const Nf32 v)
{
	NErrorIf(s0 >= s1, NERROR_SYSTEM_CHECK);			// Non autorisé, la FShape doit 's'étendre' de gauche à droite

	//m_flags		=;

	//m_fPrime			=;
	m_fUp.forwardVs(s0, v, s1 - s0);
	m_sUp.m_v1 = v;						// La vitesse est stockée ici !!!
	
	//m_fUp.forwardVs(m_sUp.m_s0 + m_sUp.m_ds, m_sUp.m_v1, fshape_ds);
	//m_sDown		= *psshape_down;

	//m_t0Offset = t0_offset;
	//m_cutOutOffsets.null();

	m_dt = m_fUp.m_dt;
	m_ds = m_fUp.m_ds;

	m_id = NLSMOTIONPROFILE_ID_ENUM::FLAT;
	
	ppromote = promoteFlat;// promoteSextendedUpFLatMotionProfile;
#ifdef _NEDITOR
	pdraw = drawFlat;// drawSextendedMotionProfile;
#endif


}
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Setup d'un motion profile Simple de type S-Up.
 *			Cette fonction ne calcule rien ( ou presque, car elle prend en charge le calcul de la durée totale i.e 'm_dt').
 *			Elle a pour objectif de ranger correctement les paramètres entrant dans la structure NLSMOTIONPROFILE et d'y associer les fonctions de sortie appropriées.
 *
 *	@param	psshape_up				Décrit la S-Shape montante du motion profile.
 *	@param	t0_offset				Offset au démarrage du motion profile, pour le faire 'virtuellement' commencer plus tôt que le dernier Kin du motion profile précédent. 
 *									Cet offset est toujour positif ou null et sera soustrait à la date du Kin précédent pour fixé la date de démarrage du motion profile.
 *	@param	cut_out_overshoot		Regroupe les Offsets appliqués lors du calculs du dernier Kin du motion profile pour le faire terminer plus tôt.
 *	@return	void
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void NLSMOTIONPROFILE::sUp(const NLSMOTIONSHAPE * psshape_up, const  NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets)
{
	//m_flags			=;
	m_sUp			= *psshape_up;
	//m_fUp			=;
	//m_sDown		=;

	if (pcut_in_offsets)
		m_cutInOffsets = *pcut_in_offsets;
	else
		m_cutInOffsets.null();

	if (pcut_out_offsets)
		m_cutOutOffsets = *pcut_out_offsets;
	else
		m_cutOutOffsets.null();

	NErrorIf(m_cutInOffsets.m_dt > 0.0f, NERROR_SYSTEM_CHECK);
	NErrorIf(m_cutOutOffsets.m_dt	> 0.0f, NERROR_SYSTEM_CHECK);
	m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
	m_ds = m_sUp.m_ds;							// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;

	m_id = NLSMOTIONPROFILE_ID_ENUM::SUP;

	ppromote		= promoteSup;//promoteSsimpleMotionProfile;
#ifdef _NEDITOR
	pdraw			= drawSup;// drawSsimpleMotionProfile;
#endif

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *	@brief	Setup d'un motion profile Simple de type S-Down.
 *			Cette fonction ne calcule rien ( ou presque, car elle prend en charge le calcul de la durée totale i.e 'm_dt').
 *			Elle a pour objectif de ranger correctement les paramètres entrant dans la structure NLSMOTIONPROFILE et d'y associer les fonctions de sortie appropriées.
 *
 *	@param	psshape_down			Décrit la S-Shape descendante du motion profile.
 *	@param	t0_offset				Offset au démarrage du motion profile, pour le faire 'virtuellement' commencer plus tôt que le dernier Kin du motion profile précédent.
 *									Cet offset est toujour positif ou null et sera soustrait à la date du Kin précédent pour fixé la date de démarrage du motion profile.
 *	@param	cut_out_overshoot		Regroupe les Offsets appliqués lors du calculs du dernier Kin du motion profile pour le faire terminer plus tôt.
 *	@return	void
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void NLSMOTIONPROFILE::sDown(const NLSMOTIONSHAPE * psshape_down, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets)
{
	//m_flags			=;

	//m_sUp				=;
	//m_fUp				=;
	m_sDown			= *psshape_down;
	
	if (pcut_in_offsets)
		m_cutInOffsets = *pcut_in_offsets;
	else
		m_cutInOffsets.null();

	if (pcut_out_offsets)
		m_cutOutOffsets = *pcut_out_offsets;
	else
		m_cutOutOffsets.null();

	NErrorIf(m_cutInOffsets.m_dt > 0.0f, NERROR_SYSTEM_CHECK);
	NErrorIf(m_cutOutOffsets.m_dt > 0.0f, NERROR_SYSTEM_CHECK);
	m_dt = (m_sDown.m_tb + m_sDown.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
	m_ds = m_sDown.m_ds;							// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;
	// m_ds		= m_sDown.m_ds  + m_cutInOffsets.m_dt * (m_sDown.m_v0 + m_sDown.m_jx*NPOW2(m_cutInOffsets.m_dt) / 6.0f) + m_cutOutOffsets.m_ds; !!! remplacé par la ligne juste au dessus le 2021/10/01 !

	m_id = NLSMOTIONPROFILE_ID_ENUM::SDOWN;

	ppromote		= promoteSdown;// promoteSsimpleMotionProfile;
#ifdef _NEDITOR
	pdraw			= drawSdown;// drawSsimpleMotionProfile;
#endif
}

void NLSMOTIONPROFILE::sUpFlat(const NLSMOTIONSHAPE * psshape_up, const NLSCUTINOFFSETS * pcut_in_offsets, const Nf32 fshape_ds)
{
	NErrorIf(psshape_up->m_v1 <= psshape_up->m_v0, NERROR_SYSTEM_CHECK);	// Non autorisé, la SShape doit être "montante"!
	NErrorIf(fshape_ds <= 0.0f, NERROR_SYSTEM_CHECK);						// Non autorisé, la fShape doit exister et s'étirer vers la droite !
	
	//m_flags		=;
	m_sUp			= *psshape_up;
	m_fUp.forwardVs(m_sUp.m_s0 + m_sUp.m_ds, m_sUp.m_v1,fshape_ds);
	//m_sDown		= *psshape_down;

	if (pcut_in_offsets)
		m_cutInOffsets = *pcut_in_offsets;
	else
		m_cutInOffsets.null();

	m_cutOutOffsets.null();

	m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + m_fUp.m_dt;	// +m_cutInOffsets.m_dt; // !! m_cutOutOffsets.m_dt est tjrs null pour un sUpFlat !!
	m_ds = m_sUp.m_ds + m_fUp.m_ds;							// +m_cutInOffsets.m_ds; // !! m_cutOutOffsets.m_ds est tjrs null pour un sUpFlat !!
	
	m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT;

	ppromote	= promoteSupFlat;// promoteSextendedUpFLatMotionProfile;
#ifdef _NEDITOR
	pdraw		= drawSupFlat;// drawSextendedMotionProfile;
#endif

}

void NLSMOTIONPROFILE::flatSdown(const NLSMOTIONSHAPE * psshape_down, const NLSCUTOUTOFFSETS * pcut_out_offsets, const Nf32 fshape_ds)
{
	NErrorIf(psshape_down->m_v0 <= psshape_down->m_v1, NERROR_SYSTEM_CHECK);	// Non autorisé, la SShape doit être "descendante"!
	
	//NErrorIf(fshape_ds <= 0.0f, NERROR_SYSTEM_CHECK);							// Non autorisé, la fShape doit exister et s'étirer vers la droite !
	if (fshape_ds <= 0.0f)
		Nu32 a = 1;
	
	//m_flags		=;
	//m_sUp			= *psshape_up;
	m_sDown			= *psshape_down;
	m_fUp.backwardVs(m_sDown.m_s0, m_sDown.m_v0, fshape_ds);

	m_cutInOffsets.null();
	if (pcut_out_offsets)
		m_cutOutOffsets = *pcut_out_offsets;
	else
		m_cutOutOffsets.null();

	m_dt = (m_sDown.m_tb + m_sDown.m_tx * 2.0f) + m_fUp.m_dt;	// + m_cutOutOffsets.m_dt;  // !! m_cutInOffsets.m_dt est tjrs null pour un sUpFlat !!
	m_ds = m_fUp.m_ds + m_sDown.m_ds;							// + m_cutOutOffsets.m_ds;  // !! m_cutInOffsets.m_ds est tjrs null pour un sUpFlat !!
	m_id = NLSMOTIONPROFILE_ID_ENUM::FLAT_SDOWN;

	ppromote		= promoteFlatSdown;// promoteSextendedUpFLatMotionProfile;
#ifdef _NEDITOR
	pdraw			= drawFlatSdown;// drawSextendedMotionProfile;
#endif
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Setup de la class NLSMOTIONPROFILE à partir de deux SMOTIONSHAPE ( up et down )
 *
 *	@param		psshape_up pointeur vers la SMOTIONSHAPE Up ( d'entrée ). Son contenu sera copié dans le  motion profile.
 *	@param		psshape_down pointeur vers la SMOTIONSHAPE Down ( de sortie ). Son contenu sera copié dans le  motion profile.
 *	@param		pcut_in_offsets pointeur vers une structure NLSCUTINOFFSETS. Son contenu sera copié dans le motion profile. 
 *				Ce paramètre peut-être null. Dans ce cas le membre m_cutInOffsets est défini par m_cutInOffsets.null();
 *	@param		pcut_out_offsets pointeur vers une structure NLSCUTOUTOFFSETS. Son contenu sera copié dans le motion profile. 
 *				Ce paramètre peut-être null. Dans ce cas le membre m_cutOutOffsets est défini par m_cutOutOffsets.null();
 *	@return		Retourne l'Id du NLSMOTIONPROFILE. ( \b EMPTY ou \b SUP_SDOWN ou \b SUP_FLAT_SDOWN )	
 *
 */
 // ------------------------------------------------------------------------------------------
NLSMOTIONPROFILE_ID_ENUM NLSMOTIONPROFILE::sRamped(const NLSMOTIONSHAPE * psshape_up, const NLSMOTIONSHAPE * psshape_down, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets)
{
	//NErrorIf(psshape_down->m_v0 <= psshape_down->m_v1, NERROR_SYSTEM_CHECK);	// Non autorisé, la SShape doit être "descendante"!
	//NErrorIf(fshape_ds <= 0.0f, NERROR_SYSTEM_CHECK);							// Non autorisé, la fShape doit exister et s'étirer vers la droite !

	//m_flags			= 0;

	m_sUp			= *psshape_up;
	m_sDown			= *psshape_down;

	if (pcut_in_offsets)
		m_cutInOffsets = *pcut_in_offsets;
	else
		m_cutInOffsets.null();

	if (pcut_out_offsets)
		m_cutOutOffsets = *pcut_out_offsets;
	else
		m_cutOutOffsets.null();

	Nf32 fds = m_sDown.m_s0 - (m_sUp.m_s0 + m_sUp.m_ds);

	if (fds < 0.0f)
	{
		NErrorIf(1, NERROR_SYSTEM_CHECK); // ???  ( le cas est géré mais ... pas à ce niveau, il s'agit d'un croisement !!!)
		m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN;
	}
	else if (fds) // ici fds > 0
	{
		m_fUp.backwardVs(m_sDown.m_s0, m_sDown.m_v0, fds);

		m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + m_fUp.m_dt + (m_sDown.m_tb + m_sDown.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
		m_ds = m_sUp.m_ds + m_sDown.m_ds + m_fUp.m_ds;													// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;
		m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN;

		ppromote	= promoteSupFlatSdown;
		#ifdef _NEDITOR
		pdraw		= drawSupFlatSdown;
		#endif
	}
	else // fds = 0
	{
		m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + (m_sDown.m_tb + m_sDown.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
		m_ds = m_sUp.m_ds + m_sDown.m_ds;												// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;

		m_id		= NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN;

		ppromote = promoteSupSdown;
		#ifdef _NEDITOR
		pdraw = drawSupSdown;
		#endif
	}

	return (NLSMOTIONPROFILE_ID_ENUM)m_id;
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Setup de la class NLSMOTIONPROFILE à partir de deux NLSVSTAGE et d'une vitesse max.
 *
 *	@param		vmax vitesse maximum au delà de laquelle le motionprofile ne peut aller.
 *	@param		pv0 NLVSTAGEX ( palier de vitesse ) de départ.
 *	@param		pv1 NLVSTAGEX ( palier de vitesse ) d'arrivée.
 *
 *	@return		Retourne l'Id du NLSMOTIONPROFILE ainsi construit. ( \b EMPTY ou \b SUP_SDOWN ou \b SUP_FLAT_SDOWN )
 *
 */
 // ------------------------------------------------------------------------------------------
NLSMOTIONPROFILE_ID_ENUM NLSMOTIONPROFILE::sRamped(const Nf32 vmax, const NLVSTAGEX * pv0, const NLVSTAGEX * pv1)
{


#ifdef _DEBUG
	Nf32 s0 = pv0->m_s1 + pv0->m_stitch.m_start.m_s1Offsets.m_ds;
	Nf32 v0 = pv0->m_v	+ pv0->m_stitch.m_start.m_s1Offsets.m_dv;
	Nf32 s1 = pv1->m_s0 + pv1->m_stitch.m_end.m_s0Offsets.m_ds;
	Nf32 v1 = pv1->m_v	+ pv1->m_stitch.m_end.m_s0Offsets.m_dv;
	NErrorIf(v0 > vmax, NERROR_INCONSISTENT_VALUES);			// v0 doit être inférieure ou egale à la vitesse max !
	NErrorIf(v1 > vmax, NERROR_INCONSISTENT_VALUES);			// v1 doit être inférieure ou egale à la vitesse max !
	NErrorIf((s1 - s0) <= 0.0f, NERROR_INCONSISTENT_VALUES);	// ds0s1 (la distance à parcourir) ne peut être que positive ! Elle ne peut pas être négative ou nulle !
																// ... TODO, voir à prendre en compte le signe et gérer une distance algébrique ?
#endif	

	NLSMOTIONSHAPE sup, sdown;
	NLSMOTIONSHAPE::STYPE up_type	= sup.forwardBuildUp(pv0, vmax);
	NLSMOTIONSHAPE::STYPE down_type	= sdown.backwardBuildDown(vmax, pv1);
	NErrorIf(!up_type, NERROR_NULL_VALUE);			// Pas une erreur, juste pour voir si il ne faudrait pas gérer ce cas dans la construction du MP avec sRamped ... 
	NErrorIf(!down_type, NERROR_NULL_VALUE);		// Pas une erreur, juste pour voir si il ne faudrait pas gérer ce cas dans la construction du MP avec sRamped ... 	

	Nf32 dsflat = sdown.m_s0 - (sup.m_s0 + sup.m_ds);
	//						   ---------------------		
	//									|
	//									= sup.m_s1



	if ( (dsflat < 0.0f) /*|| !(up_type|down_type)*/ )
	{
		// Signifie qu'il y a croisement des 2 SSHAPES ( si dsflat == 0 )
		// ou
		// que les deux shapes sup et sdown sont NULL ( si combine == 0 )
		return NLSMOTIONPROFILE_ID_ENUM::EMPTY;
	}
	else
	{

		m_sUp	= sup;
		m_sDown = sdown;
		m_cutInOffsets = pv0->m_stitch.m_start.m_cutIn;
		m_cutOutOffsets = pv1->m_stitch.m_end.m_cutOut;

		/* On pourrait intégrer ici une optim... 
		 * Mais atention seulement si m_cutInOffsets et/ou m_cutOutOffsets sont NULL. ( ou alors ça risque de compliquer un peu les choses ... )
		 * Car cette optim modifiera légèrement le jerk d'une des deux shapes (Up ou Down).
		 * Or ce dernier ne peut-être modifié en cas de cutIn/cutOut car il est utilisé à l'identique par le motion profile frere de droite ou de gauche. 
		 * L'idée est simple, baisser le JERK d'une des 2 S SHAPEs  pour "absorber" la distance couverte par la flat shape.
		 * En effet faire progresser l'acceleration avec un taux moins élevé va prendre plus de temps pour atteindre l'acceleration max et revenir ensuite à ZERO...
		 * ... et in fine consommera plus de distance. On cherchera donc le jerk permettant de consommer exactement la distance couverte initialement par la FShape
		 * ... ainsi plus de FSHAPE !
		 *
		 *								Ce nouveau cherk est: j = a*(v0+v1)/(d-e)
		 *								
		 *								avec	a acceleration max utilisée par la shape 
		 *										d distance totale couverte par le motion profile
		 *										e distance couverte par la flat shape
		 *
		 * On pourrait imaginer appliquer cela au 2 S SHAPE		( Up et Down ) en répartissant 'e' à part égale mais le plus optimal ( celui qui au final prendra le moins de temps )	
		 * semble être intuitivement de choisir SUp si SUp.m_v0 => SDown.m_v1 Sinon SDown. En effet avec une vitesse plus élevée on consommera plus vite la distance "e"
		 *
		 * pour l'instant cette optimisation est mise de côté mais ... à voir !
		 *
		if (dsflat < NLKIN_EPSILON_S)
		{

		}
		*/

		if (dsflat > 0.0f)
		{
			m_fUp.forwardVs(m_sUp.m_s0 + m_sUp.m_ds, m_sUp.m_v1, dsflat);
			m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + (m_sDown.m_tb + m_sDown.m_tx * 2.0f) + m_fUp.m_dt;	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
			m_ds = m_sUp.m_ds + m_sDown.m_ds + m_fUp.m_ds;													// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;
			m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN;

			ppromote = promoteSupFlatSdown;// promoteSrampedMotionProfile;
			#ifdef _NEDITOR
			pdraw = drawSupFlatSdown;// drawSrampedMotionProfile;
			#endif
		}
		else // dsflat = 0.0f
		{
			m_fUp.null();
			m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + (m_sDown.m_tb + m_sDown.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
			m_ds = m_sUp.m_ds + m_sDown.m_ds;												// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;

			m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN;

			ppromote = promoteSupSdown;// promoteSrampedMotionProfile;
			#ifdef _NEDITOR
			pdraw = drawSupSdown;// drawSrampedMotionProfile;
			#endif
		}
	}
	return (NLSMOTIONPROFILE_ID_ENUM)m_id;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Setup de la class NLSMOTIONPROFILE à partir des vitesses et abscisses curv. initiales et finales, des limites cinématiques à utiliser et du cut_in et cut_out. 
 *			Cette version de sRamped ne prend pas de vitesse vmax en paramètre. Cette vitesse vmax qui, rappellons sera atteinte depuis v0 avant de revenir vers v1, est donc 
 *			évaluée à partir de la distance couverte par le motion profile ( = s1 -s0). Pour plus de détails sur ce calcul voir MOTION PROFILE C
 *			
 *	@param		pkmax pointeur vers une structure NLKINLIMITS contenant les limites à utiliser ( vitesse, accel, jerk )
 *	@param		v0 vitesse initiale.
 *	@param		v1 vitesse finale
 *	@param		s0 abscisse curviligne initiale
 *	@param		s1 abscisse curviligne finale
 *	@param		pcut_in_offsets pointeur vers une structure NLSCUTINOFFSETS. Son contenu sera copié dans le motion profile.
 *				Ce paramètre peut-être null. Dans ce cas le membre m_cutInOffsets est défini par m_cutInOffsets.null();
 *	@param		pcut_out_offsets pointeur vers une structure NLSCUTOUTOFFSETS. Son contenu sera copié dans le motion profile.
 *				Ce paramètre peut-être null. Dans ce cas le membre m_cutOutOffsets est défini par m_cutOutOffsets.null();
 *
 *	@return		Retourne l'Id du NLSMOTIONPROFILE ainsi construit. ( \b EMPTY ou \b SUP_SDOWN ou \b SUP_FLAT_SDOWN )
 *
 */
 // ------------------------------------------------------------------------------------------
NLSMOTIONPROFILE_ID_ENUM NLSMOTIONPROFILE::sRamped(const NLKINLIMITS * pkmax, const Nf32 v0, const Nf32 v1, const Nf32 s0, const Nf32 s1, const NLSCUTINOFFSETS * pcut_in_offsets, const NLSCUTOUTOFFSETS * pcut_out_offsets)
{
	NErrorIf(NABS(v0) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);		// v0 doit être inférieure ou egale à la vitesse max !
	NErrorIf(NABS(v1) > pkmax->m_v, NERROR_INCONSISTENT_VALUES);		// v1 doit être inférieure ou egale à la vitesse max !
	//NErrorIf((s1 - s0) <= 0.0f, NERROR_INCONSISTENT_VALUES);			// ds0s1 (la distance à parcourir) ne peut être que positive ! Elle ne peut pas être négative ou nulle !

	Nf32 s = s1 - s0;
	Nf32 disc = 2.0f*(v0*v0 + v1 * v1) - pkmax->m_2a2ooj*(v0 + v1) + pkmax->m_a4ooj2 + pkmax->m_4a*s;

	if (disc >= 0)
	{
		Nf32 vmax = (sqrt(disc) - pkmax->m_a2ooj) / 2.0f;
		vmax = NMIN(vmax, pkmax->m_v);

		// vmax doit être plus grand que v0 et v1
		if (vmax > NMAX(v0, v1))
		{
			if (pcut_in_offsets)
				m_cutInOffsets = *pcut_in_offsets;
			else
				m_cutInOffsets.null();

			if (pcut_out_offsets)
				m_cutOutOffsets = *pcut_out_offsets;
			else
				m_cutOutOffsets.null();

			m_sUp.forwardBuildUp(pkmax, s0, v0, vmax);
			m_sDown.backwardBuildDown(pkmax, s1,vmax, v1);


			Nf32 dsflat;
			dsflat	= m_sDown.m_s0 - (m_sUp.m_s0 + m_sUp.m_ds);

			if (dsflat < -NLKIN_EPSILON_S) 
			{
				m_id = NLSMOTIONPROFILE_ID_ENUM::EMPTY;
				return (NLSMOTIONPROFILE_ID_ENUM)m_id;
			}
			else if (dsflat < 0.0f) //c'est à dire : -NLKIN_EPSILON_S <= dsflat < 0
			{
				// Nous sommes ici dans un tout petit intervalle qui normalement devrait générer "NLSMOTIONPROFILE_ID_ENUM::EMPTY;" car "dsflat < 0.0f" mais
				// abs(dsflat) est si petit qu'on peut imputer ce résultat aux erreurs d'approximation des calculs "float".
				// En conséquence on va réduire "un poil" vmax pour que cela passe
				do
				{
					vmax *= (1.0f - NLKIN_EPSILON_S);
					m_sUp.forwardBuildUp(pkmax, s0, v0, vmax);
					m_sDown.backwardBuildDown(pkmax, s1, vmax, v1);
					dsflat = m_sDown.m_s0 - (m_sUp.m_s0 + m_sUp.m_ds);
				} while (dsflat < 0.0f);
			}

			if(dsflat > 0.0f)
			{
				m_fUp.forwardVs(m_sUp.m_s0 + m_sUp.m_ds, m_sUp.m_v1, dsflat);
				m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + (m_sDown.m_tb + m_sDown.m_tx * 2.0f) + m_fUp.m_dt;	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
				m_ds = m_sUp.m_ds + m_sDown.m_ds + m_fUp.m_ds;													// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;
				ppromote = promoteSupFlatSdown;
				#ifdef _NEDITOR
				pdraw = drawSupFlatSdown;
				#endif
				m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN;
				return (NLSMOTIONPROFILE_ID_ENUM)m_id;
			}
			else // dsflat = 0.0f
			{
				NErrorIf(dsflat != 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				m_fUp.null();
				m_dt = (m_sUp.m_tb + m_sUp.m_tx * 2.0f) + (m_sDown.m_tb + m_sDown.m_tx * 2.0f);	// +m_cutInOffsets.m_dt + m_cutOutOffsets.m_dt;
				m_ds = m_sUp.m_ds + m_sDown.m_ds;												// +m_cutInOffsets.m_ds + m_cutOutOffsets.m_ds;
				ppromote = promoteSupSdown;
				#ifdef _NEDITOR
				pdraw = drawSupSdown;
				#endif
				m_id = NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN;
				return (NLSMOTIONPROFILE_ID_ENUM)m_id;
			}
		}
	}

	m_id = NLSMOTIONPROFILE_ID_ENUM::EMPTY;
	return (NLSMOTIONPROFILE_ID_ENUM)m_id;
}


//MOCHE !!!!
extern NL2DOCS				MotionProfileOcs;
//MOCHE !!!!

static inline Nu32 collision_aabb(const NLVSTAGEX *pvs, const Nf32 phase_s0, const Nf32 phase_s1, const Nf32 phase_vmax)
{
	if (pvs->m_s0 > phase_s1)
		return 0;
	if (pvs->m_s1 < phase_s0)
		return 0;
	if (pvs->m_v < phase_vmax)
		return 1;
	return 0;
}

Nu32 NLSMOTIONPROFILE::detectStageVelocityOvershoot(NLVSTAGEX *pv0, NLVSTAGEX *pv1, const Nu32 no_collision_flags)
{
	Nu32 nbcol = 0;

	for (NLVSTAGEX *pvs = pv0 + 1; pvs < pv1; pvs++)
	{
		//if (ISFLAG_ON(pvs->m_flags, FLAG_NLVSTAGE_INACCESSIBLE))
		//	continue;
		if (!pvs->isAccessibleFloor())
			continue;



		// La vitesse max du motion profile ( = m_sShape.m_v1 = m_sShapeOut.m_v0 ) est inférieure ou égale à la limite de vitesse fixée par le vstage.
		// Aucune "collision" possible
		if (pvs->m_v >= m_sUp.m_v1)
		{
			FLAG_ON(pvs->m_flags, no_collision_flags); // no_collision_flags peut-être égal à FLAG_NLVSTAGE_INACCESSIBLE ou 0
			continue;
		}

		Nf32 phase_s0, phase_s1, phase_vmax;
		Nf32 t, s;
		// Phase 1/7
		phase_s0	= m_sUp.m_s0;
		phase_s1	= m_sUp.m_s0 + m_sUp.m_v0*m_sUp.m_tx + m_sUp.m_sx;
		phase_vmax	= m_sUp.m_v0 + m_sUp.m_vx;
		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// Si la vitesse max autorisée par le Vstage est supérieure au plancher de vitesse de la phase ...
			// ... alors il est possible le VStage soit "au-dessus de la courbe de la phase" tout en intersectant sa boite englobante.
			// ... dans ces conditions il n'y a pas d'intersection !
			if (pvs->m_v > m_sUp.m_v0)
			{
				// En phase 1, on a:
				//		v = v0 + a0*t + 1/2*j*t²
				//		de plus, on sait que a0 = 0 en phase 1, donc,
				//		v = v0 + 1/2*j*t²	[1]
				//
				// On va donc chercher à quel moment la phase 1 atteint le plancher de vitesse du VStage.
				// d'après [1]:
				//		t² = 2*(v-v0)/j
				//		t = sqrt(2*(v-v0)/j)
				//
				// dans le cas qui nous préoccupe, nous sommes sur que:
				//		v > v0 ( c'est le test 	if(pvs->m_v > m_sShape.m_v0), qui le garanti ), donc v-v0 est toujours positif.
				//		j est positif car il s'agit ici de m_sShape.m_jx, soit le jerk de la sShape "montante" du motion profile, donc toujours positif.
				//		En conséquence, 2*(v-v0)/j est toujours positif. La racine carrée existe donc toujours.
				//
				t = sqrt(2.0f*(pvs->m_v - m_sUp.m_v0) / m_sUp.m_jx);

				// On va maintenant chercher, avec "t", à quelle position la phase 1 atteind la vitesse max fixée par le VStage.
				// En phase 1, on a:
				//		s = s0 + v0*t + 1/2*a0*t² + 1/6*j*t^3
				//		de plus, on sait que a0 = 0 en phase 1, donc,
				//		s = s0 + v0*t + 1/6*j*t^3
				s = phase_s0 + m_sUp.m_v0*t + (m_sUp.m_jx*t*t*t) / 6.0f;

				// Il ne reste plus qu'à comparer "s", la position exacte où la phase 1 atteint la vitesse max autorisée par le Vstage avec la position de sortie du  VStage ( soit pvs->m_s1)
				// Si s est égale à Vstage.m_s1:	Il n'y a pas 'collision'.
				//									En effet, cela signifie que la phase 1 atteind la vitesse max autorisée par le VStage juste à la sortie de celui-ci.
				// Si s est plus grand que Vstage.m_s1:	Il n'y a pas 'collision'.
				//										En effet, cela signifie que la phase 1 atteind la vitesse max autorisée APRES la sortie m_s1 du Vstage. 
				//										Avant, la vitesse de la phase 1 est inférieure.
				// Si s est plus petit que Vstage.m_s1:	Il y a 'collision'.
				//										En effet, cela signifie que la phase 1 atteind la vitesse max autorisée AVANT la sortie m_s1 du Vstage... 
				//										... la vitesse continuant d'augmenter elle dépasse la limite juste après s.
				// En bref, si s est inférieur à VStage.m_s1 il y a collision, sinon il n'y a pas collision.
				if (s < pvs->m_s1)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}

				// on passe au stage suivant...
				// 2 possibilités si on arrive ici: 
				//		a)	le test précédent est validé et donc une collision a été détectée. Dans ce cas on 'continue' pour que ce Vstage ne génère qu'une collision 
				//			il peut en effet être aussi en collision avec la phase suivante, mais seule sa "premiere" collision nous interresse.
				//		b)	le test précédent n'est pas validé et donc aucune collision n'a été détectée. Dans ce cas on 'continue' car nous sommes sûr qu'il ne peut pas y avoir de collision
				//			avec une des prochaines phases ... ( en effet, pas de collision signifie que le Vstage est à 'gauche' de la courbe de la phase, et comme  les phases suivantes s'étendent
				//			à droite ... ;)
				continue;
			}
			else
				// Le plancher de vitesse du Vstage est inférieur ou égal au plancher de vitesse de la phase.
				// La phase intersecte obligatoirement le VStage !
			{
				// collision !
				nbcol++;
				continue; // on passe au stage suivant. Ainsi un stage ne peut générer qu'une seule collision.
			}
		}

		// Phase 2/7
		phase_s0 = phase_s1;
		phase_s1 += ((m_sUp.m_v0 + m_sUp.m_v1)*m_sUp.m_tb) / 2.0f; // ou s1 = m_sShape.m_s - m_sShape.m_v1*(m_sShape.m_tx*2.0f + m_sShape.m_tb) - m_sShape.m_sx;
		phase_vmax = m_sUp.m_v1 - m_sUp.m_vx;
		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// plancher de phase 2 = m_sShape.m_v0 + m_sShape.m_vx
			if (pvs->m_v > m_sUp.m_v0 + m_sUp.m_vx)
			{
				// En phase 2, on a:
				//		v = v0 + a0*t + 1/2*j*t²
				//		de plus, on sait que j = 0 en phase 2, donc,
				//		v = v0 + a0*t	[2]
				//
				// On va donc chercher à quel moment la phase 2 atteint le plancher de vitesse du VStage.
				// d'après [2]:
				//		t = (v-v0)/a0
				//
				// dans le cas qui nous préoccupe, nous sommes sur que:
				//		a0 > 0 car il s'agit ici de m_sShape.m_ax, soit l'acceleration max de la sShape "montante" du motion profile, donc toujours positive.
				//		En conséquence, 1/a0 existe donc toujours.
				t = (pvs->m_v - m_sUp.m_v0 - m_sUp.m_vx) / m_sUp.m_ax;

				// On va maintenant chercher, avec "t", à quelle position la phase 2 atteind la vitesse max locale fixée par le VStage.
				// En phase 2, on a:
				//		s = s0 + v0*t + 1/2*a0*t² + 1/6*j*t^3
				//		de plus, on sait que j = 0 en phase 2, donc,
				//		s = s0 + v0*t + 1/2*a0*t²
				s = phase_s0 + (m_sUp.m_v0 + m_sUp.m_vx)*t + m_sUp.m_ax*t*t / 2.0f;

				// Il ne reste plus qu'à effectuer les tests comme précédement ( cf Phase 1/7 )
				if (s < pvs->m_s1)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}

				continue; // on passe au stage suivant. ( cf Phase 1/7 )
			}
			else
			{
				// collision !
				nbcol++;
				continue;
			}
		}
		// Phase 3/7
		phase_s0 = phase_s1;
		phase_s1 = m_sUp.m_s0 + m_sUp.m_ds;
		phase_vmax = m_sUp.m_v1;
		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// plancher de phase 3 =  m_sShape.m_v1 - m_sShape.m_vx
			if (pvs->m_v > m_sUp.m_v1 - m_sUp.m_vx)
			{
				// En phase 3, on a:
				// [3a]	v = v0 + a0*t + 1/2*j*t²		 en partant de la fin de la phase 2 avec :
				//														v0 = m_sShape.m_v1 - m_sShape.m_vx	( vitesse en fin de phase 2/début de phase 3 )
				//														a0 = m_sShape.m_ax					( acceleration en fin de phase 2/début de phase 3 )
				//														 j = -m_sShape.m_jx					(  Jerk en début de phase 3 pour ramener l'acceleration à ZERO)		
				//		Trouver t à partir de [3a] oblige à en passer par la résolution d'une equation de degré 2. Simple, mais il y a ENCORE PLUS SIMPLE dans ce cas précis.
				//		En effet, on sait aussi que l'acceleration est nulle en fin de phase 3, or
				//		a = a0 + jt	
				//		donc,
				//		a0 + jt = 0 
				//		soit,
				//	
				// [3b]	a0 = -jt ( toujours avec j = -m_sShape.m_jx et a0 la valeur de l'acceleration associée à v0)
				//		
				//		Considérons donc maintenant que a0 est l'acceleration quand v0 vaut 'pvs->m_v'
				//		Il faudra un temps "t" pour amener cette acceleration à 0 et pendant ce même temps la vitesse devra passer de sa valeur v0 à la valeur de la vitesse en fin de phase 3.
				//		donc, d'après [3a] et [3b], en fin de phase 3 nous avons,
				//		v = v0 -jt*t + 1/2*j*t²
				//		v = v0 -jt² + 1/2*j*t²
				//		v = v0 -1/2*jt²
				//		Bien sûr, on connait la vitesse en fin de phase 3 ! ( c'est v1 ! )
				//		donc,
				//		m_sShape.m_v1									= pvs->m_v - 1/2 * (-m_sShape.m_jx)*t²
				//		m_sShape.m_v1 - pvs->m_v						= - 1/2 * (-m_sShape.m_jx)*t²
				//		-2*(m_sShape.m_v1 - pvs->m_v)/(-m_sShape.m_jx)	= t²
				//		 2*(m_sShape.m_v1 - pvs->m_v)/m_sShape.m_jx		= t²
				//
				// [3c]	t = sqrt(2*(m_sShape.m_v1 - pvs->m_v)/m_sShape.m_jx )	
				//
				//		dans le cas qui nous préoccupe, nous sommes surs que:
				//				m_sShape.m_v1 - pvs->m_v > 0 , en effet, grâce au test de collision aabb précédent nous sommes sûr que 'pvs->m_v < m_sShape.m_v1'
				//				m_sShape.m_jx est toujours positif  car c'est le jerk de la sShape "montante" du motion profile.
				//				En conséquence, 2*(m_sShape.m_v1 - pvs->m_v)/m_sShape.m_jx  est toujours positif. 
				//				La racine carrée existe donc toujours.
				//
				//		Attention 't' représente ici la durée nécéssaire pour amener l'acceleration à ZERO ( i.e fin de phase 3) depuis sa valeur quand v0 vaut 'pvs->m_v'
				//		Mais nous cherchons la durée séparant la fin de la phase 2/début de phase 3 du moment où la vitesse vaut 'pvs->m_v'.
				//		Dis autrement le point recherché P a comme abscisse 't' par rapport à la fin de la phase 3.
				//		... Si voulons connaitre ses coordonnées par rapport à la fin de la phase 2...
				//		... Il suffit juste de faire un changement d'origine. En effet on sait que la fin de la phase 2 et la fin de la phase 3 sont séparées par une durée 'm_sShape.m_tx' !
				//		... Donc,
				//		la valeur de t par rapport à la fin de la phase 2 vaut:
				//		t = m_sShape.m_tx - sqrt(2*(m_sShape.m_v1 - pvs->m_v)/m_sShape.m_jx )	
				//		
				//		"Est-il possible que sqrt(2*(m_sShape.m_v1 - pvs->m_v)/m_sShape.m_jx ) soit supérieur à m_sShape.m_tx ?"
				//			pour simplifier la notation on pose v1 = m_sShape.m_v1, vx = m_sShape.m_vx et jx = m_sShape.m_jx
				//			soit,
				//			t =  m_sShape.m_tx - sqrt(2*(v1 - pvs->m_v)/jx )
				//		Dans notre cas, nous sommes sûrs que:
				//			( v1 - pvs->m_v) < vx , et ce grâce au test précédent: 	if (pvs->m_v > m_sShape.m_v1 - m_sShape.m_vx)...
				//			En conséquence, la limite supérieure de sqrt(2*(v1 - pvs->m_v)/jx ) vaut: 	
				//
				// [3d] t2lim = sqrt( 2*(vx) / jx )
				//
				//		or on sait que,
				//		vx = 1/2*jx*tx² 
				//		En remplacant dans [3d] on obtient,
				//		t2lim = sqrt( 2*(1/2*jx*tx²) / jx )
				//		t2lim = sqrt( jx*tx² / jx ) 
				//		t2lim = sqrt( tx² ) 
				//		t2lim = tx
				//		
				//		DONC,
				//		Nous sommes sûrs que 
				//			sqrt(2*(v1 - pvs->m_v)/jx ) < m_sShape.m_tx			...			( rappel: car ( v1 - pvs->m_v) < vx ) 
				//
				//		
				//		Pour la suite des calculs et dans un soucis de simplicité nous continuons à mesurer t depuis la fin de la phase 3
				t = sqrt(2.0f*(m_sUp.m_v1 - pvs->m_v) / m_sUp.m_jx);

				// On va maintenant chercher, avec "t", à quelle position la phase 3 atteind la vitesse max locale fixée par le VStage.
				// En phase 3, et en partant de la fin,  on a:
				//		s = s1 - v1*t + 1/6*j*t^3 ( avec a = 0 en fin de phase 3 )
				s = m_sUp.m_s0 + m_sUp.m_ds - m_sUp.m_v1*t + m_sUp.m_jx*t*t*t / 6.0f;
				if (s < pvs->m_s1)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}

				continue; // on passe au stage suivant... ( voir phase 1/7 pour détails )
			}
			else
			{
				// collision !
				nbcol++;
				continue; // on passe au stage suivant. Ainsi un stage ne peut générer qu'une seule collision.
			}
		}

		// Phase 4/7
		if (m_fUp.m_dt)
		{
			//phase_s0  = phase_s1;
			//phase_s1 += m_flatShape.m_ds0s1;
			//phase_vmax = m_flatShape.m_v; est inutile car m_flatShape.m_v = m_sShape.m_v1 = phase_vmax; donc "phase_vmax" a déjà la bonne valeur

			NErrorIf(NLKIN_S_DISTINCT(m_fUp.m_s0, phase_s1), NERROR_SYSTEM_GURU_MEDITATION);
			phase_s0 = m_fUp.m_s0;
			phase_s1 = m_fUp.m_s0 + m_fUp.m_ds;
			// phase_vmax = m_sUp.m_v1; already défini à cette valeur ... voir plus haut !
			NErrorIf(NLKIN_V_DISTINCT(m_fUp.m_ds/m_fUp.m_dt, m_sUp.m_v1), NERROR_SYSTEM_GURU_MEDITATION);

			if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
			{
				// collision !
				nbcol++;
				continue; // on passe au stage suivant. Ainsi un stage ne peut générer qu'une seule collision.
			}
		}
		// Phase 5/7
		//phase_s0  = phase_s1;
		//phase_s1 += m_sShapeOut.m_v1*m_sShapeOut.m_tx - m_sShapeOut.m_sx;
		//phase_vmax = m_outShape.m_v0; "useless" car m_sShapeOut.m_v0 = m_flatShape.m_v = m_sShape.m_v1 = phase_vmax;

		NErrorIf(NLKIN_S_DISTINCT(m_sDown.m_s0, phase_s1), NERROR_SYSTEM_GURU_MEDITATION);
		phase_s0 = m_sDown.m_s0;
		phase_s1 = m_sDown.m_s0 + m_sDown.m_v0*m_sDown.m_tx + m_sDown.m_sx;
		NErrorIf(NLKIN_V_DISTINCT(phase_vmax, m_sDown.m_v0), NERROR_SYSTEM_GURU_MEDITATION);

		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// Rappel:
			// Si la vitesse max autorisée par le Vstage est supérieure au plancher de vitesse de la phase ...
			// ... alors il est possible le VStage soit "au-dessus de la courbe de la phase" tout en intersectant sa boite englobante.
			// ... dans ces conditions il n'y a pas d'intersection !
			if (pvs->m_v > m_sDown.m_v0 + m_sDown.m_vx)
			{
				// En phase 5, on a:
				//		v = v0 + a0*t + 1/2*j*t²
				//		de plus, on sait que a0 = 0 en début de phase 5,
				//
				// [5]	v = v0 + 1/2*j*t²	
				//
				// On va donc chercher à quel moment la phase 5 atteint la vitesse max fixée par le VStage.
				// d'après [5]:
				//		t² = 2*(v-v0)/j
				//		t = sqrt(2*(v-v0)/j)
				//
				// dans le cas qui nous préoccupe, nous sommes sur que:
				//		v < v0 ( c'est le test de collision aabb qui nous le garanti ! Donc v-v0 est toujours négatif.
				//		j est négatif car il s'agit ici de m_sShapeOut.m_jx, soit le jerk de la sShape "desendante" du motion profile, donc toujours negatif.
				//		En conséquence, 2*(v-v0)/j est toujours positif. La racine carrée existe donc toujours.
				//
				t = sqrt(2.0f*(pvs->m_v - m_sDown.m_v0) / m_sDown.m_jx);

				// On va maintenant chercher, avec "t", à quelle position la phase 5 atteind la vitesse max fixée par le VStage.
				// En phase 5, on a:
				//		s = s0 + v0*t + 1/2*a0*t² + 1/6*j*t^3
				//		avec a0 = 0 en phase 5, donc,
				//		s = s0 + v0*t + 1/6*j*t^3
				s = phase_s0 + m_sDown.m_v0*t + m_sDown.m_jx*t*t*t / 6.0f;

				// Il ne reste plus qu'à comparer "s", la position exacte où la phase 5 atteint la vitesse max autorisée par le Vstage avec la position d'entrée du  VStage ( soit pvs->m_s0)
				// Si s est égale à Vstage.m_s0:	Il n'y a pas 'collision'.
				//									En effet, cela signifie que la phase 5 atteind la vitesse max autorisée par le VStage juste à l'entrée de celui-ci, et comme la phase 5 "descend"...
				// Si s est plus petit que Vstage.m_s0:	Il n'y a pas 'collision'.
				//										En effet, cela signifie que la phase 1 atteind la vitesse max autorisée AVANT l'entrée m_s0 du Vstage. 
				//										Après, la vitesse de la phase 5 ( qui décroit ) est inférieure.
				// Si s est plus grand que Vstage.m_s0: Il y a 'collision'.
				//										En effet, cela signifie que la phase 5 atteind la vitesse max autorisée APRES l'entrée m_s0 du Vstage... 
				//										
				// En bref, si s est supérieur à VStage.m_s0 il y a collision, sinon il n'y a pas collision.

				if (s > pvs->m_s0)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}

				continue;
			}
			else
			{
				// collision !
				nbcol++;
				continue;
			}
		}
		// Phase 6/7
		phase_s0 = phase_s1;
		phase_s1 = m_sDown.m_s0 + m_sDown.m_ds - m_sDown.m_v1*m_sDown.m_tx - m_sDown.m_sx;
		phase_vmax = m_sDown.m_v0 + m_sDown.m_vx;
		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// Rappel:
			// Si la vitesse max autorisée par le Vstage est supérieure au plancher de vitesse de la phase ...
			// ... alors il est possible le VStage soit "au-dessus de la courbe de la phase" tout en intersectant sa boite englobante.
			// ... dans ces conditions il n'y a pas d'intersection !
			if (pvs->m_v > m_sDown.m_v1 - m_sDown.m_vx)
			{
				// En phase 6, on a:
				//		v = v0 + a0*t + 1/2*j*t²
				//		de plus, on sait que j = 0 en début de phase 6,
				//
				// [6]	v = v0 + a0*t
				//
				// On va donc chercher à quel moment la phase 6 atteint la vitesse max fixée par le VStage.
				// d'après [6]:
				//		t = (v-v0)/a0
				//
				// dans le cas qui nous préoccupe, nous sommes sur que:
				//		v < v0 ( c'est le test de collision aabb qui nous le garanti ! Donc v-v0 est toujours négatif.
				//		a0 est négatif car il s'agit ici de m_sShapeOut.m_ax, soit l'acceleration "max" de la sShape "desendante" du motion profile, donc toujours negative.
				//		En conséquence, t = (v-v0)/a0 est toujours défini et positif.
				//
				t = (pvs->m_v - (m_sDown.m_v0 + m_sDown.m_vx)) / m_sDown.m_ax;

				// On va maintenant chercher, avec "t", à quelle position la phase 6 atteind la vitesse max fixée par le VStage.
				// En phase 6, on a:
				//		s = s0 + v0*t + 1/2*a0*t² + 1/6*j*t^3
				//		avec j = 0donc,
				//		s = s0 + v0*t + 1/2*a0*t²
				s = phase_s0 + (m_sDown.m_v0 + m_sDown.m_vx)*t + m_sDown.m_ax*t*t / 2.0f;

				if (s > pvs->m_s0)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}

				continue;
			}
			else
			{
				// collision !
				nbcol++;
				continue;
			}
		}
		// Phase 7/7
		phase_s0 = phase_s1;
		phase_s1 = m_sDown.m_s0 + m_sDown.m_ds;
		phase_vmax = m_sDown.m_v1 + m_sDown.m_vx;
		if (collision_aabb(pvs, phase_s0, phase_s1, phase_vmax))
		{
			// Rappel:
			// Si la vitesse max autorisée par le Vstage est supérieure au plancher de vitesse de la phase ...
			// ... alors il est possible le VStage soit "au-dessus de la courbe de la phase" tout en intersectant sa boite englobante.
			// ... dans ces conditions il n'y a pas d'intersection !
			if (pvs->m_v > m_sDown.m_v1)
			{
				// En phase 7, on a:
				//
				// [7]	v = v0 + a0*t + 1/2*j*t²		en partant de la fin de la phase 6.
				// 		
				// Tout comme pour la phase 3, on peut faire plus simple en partant de la fin de la phase 7, là où on sait que l'acceleration est nulle !
				// La démarche est exactement la même que pour la phase 3
				//
				// Donc, d'après [3c]:
				//		t = sqrt(2*(m_sShapeOut.m_v1 - pvs->m_v)/m_sShapeOut.m_jx )
				//
				//		dans le cas qui nous préoccupe, nous sommes surs que:
				//				m_sShapeOut.m_v1 - pvs->m_v < 0 ,  grâce au test précédent... if (pvs->m_v > m_sShapeOut.m_v1)
				//				m_sShapeOut.m_jx est toujours negatif  car c'est le jerk de la sShape "descendante" du motion profile.
				//				En conséquence, 2*(m_sShapeOut.m_v1 - pvs->m_v)/m_sShapeOut.m_jx  est toujours positif. 
				//				La racine carrée existe donc toujours.
				//
				t = sqrt(2.0f*(m_sDown.m_v1 - pvs->m_v) / m_sDown.m_jx);

				// On va maintenant chercher, avec "t", à quelle position la phase 7 atteind la vitesse max locale fixée par le VStage.
				// En phase 7, et en partant de la fin,  on a:
				//		s = s1 - v1*t + 1/6*j*t^3 ( avec a = 0 en fin de phase 7 )
				s = phase_s1 - m_sDown.m_v1*t + (m_sDown.m_jx*t*t*t) / 6.0f;

				if (s > pvs->m_s0)
				{
					// collision !
					nbcol++;
				}
				else
				{
					FLAG_ON(pvs->m_flags, no_collision_flags);
				}
				// on passe au stage suivant...
				continue;
			}
			else
			{
				// collision !
				nbcol++;
				continue; // on passe au stage suivant. Ainsi un stage ne peut générer qu'une seule collision.
			}
		}

		// Si on passe ici c'est qu'aucune collision n'a été détectée ...
		FLAG_ON(pvs->m_flags, no_collision_flags);
	}

	return nbcol;
}
