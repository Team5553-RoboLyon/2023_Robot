#include "../../../N/NErrorHandling.h"
#include "../../../N/NMath.h"
#include "../../../N/NFlags.h"


#include "NLKNode.h"

NLVSTAGEX* NLKNODE::getSplitVStage()
{
	NLVSTAGEX *psplit	= NULL;
	Nf32		vmin	= NF32_MAX;

	for (NLVSTAGEX *pvs = m_pVfrom + 1; pvs < m_pVto; pvs++)
	{
		if ( pvs->isAccessibleFloor() && (pvs->m_v < vmin) )
		{
			vmin	= pvs->m_v;
			psplit	= pvs;
		}
	}
	return psplit;
}

NLVSTAGEX* NLKNODE::getHighestCeilVStage()
{
	NLVSTAGEX*	pvsx = NULL;
	Nf32		vmax = -NF32_MAX;

	for (NLVSTAGEX* pvs = m_pVfrom + 1; pvs < m_pVto; pvs++)
	{
		if ( ISFLAG_OFF(pvs->m_flags, FLAG_NLVSTAGEX_IS_CEIL) && (pvs->m_v > vmax) )
		{
			vmax = pvs->m_v;
			pvsx = pvs;
		}
	}
	return pvsx;
}

Nf32 NLKNODE::getHighestVelocity()
{
	Nf32		vmax = -NF32_MAX;

	for (NLVSTAGEX* pvs = m_pVfrom + 1; pvs < m_pVto; pvs++)
	{
		if (ISFLAG_ON(pvs->m_flags, FLAG_NLVSTAGEX_IS_CEIL) && (pvs->m_v > vmax))
			vmax = pvs->m_v;
	}
	return vmax;
}



NLVSTAGEX* NLKNODE::deepSolveAndGetSplitVStage()
{
	// Le noeud est déjà flaggé "solution optimum" il ne sera donc pas possible de faire mieux !
	// En cas de dépassement ou de plafonnement lors de la "primo-recherche" de solution directe. 
	if( ISFLAG_ON(m_motionProfile.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION ) )
		return NULL;

	//return getSplitVStage();


	NLSMOTIONPROFILE mp,mpo;
	mp.initialize();

	// On cherche d'abord si il existe un motion profile sur l'intervalle du noeud
	Nf32 s0 = m_pVfrom->m_s1	+	m_pVfrom->m_stitch.m_start.m_s1Offsets.m_ds;
	Nf32 v0 = m_pVfrom->m_v		+	m_pVfrom->m_stitch.m_start.m_s1Offsets.m_dv;
	Nf32 s1 = m_pVto->m_s0		+	m_pVto->m_stitch.m_end.m_s0Offsets.m_ds;
	Nf32 v1 = m_pVto->m_v		+	m_pVto->m_stitch.m_end.m_s0Offsets.m_dv;

	Nf32 vhigh	= getHighestVelocity();
	Nf32 vmax	= vhigh;// plocal_limits->m_v;
	Nf32 vbase	= NMAX(v0, v1);
	Nf32 vdelta = (vmax-vbase)/2.0f;	// Il est important de commencer avec la moitié de la diff. car au premier tour on fait vmax -= vdelta soit
										// si, vdelta = vmax - vbase , on aura vmax = vmax - vdelta  = vmax - (vmax - vbase ) = vbase ...
										// et avec les imprécisions du au virgules flottantes vmax = vbase peut devenir vmax < vbase et là ... AÎE
										// en divisant d'entrée par 2 on évite l'effet de bord de l'imprécision float. Par contre on prend 1 tour d'avance 
										// sur la valeur de vdelta. En soit cela ne pose aucun pb sauf par rapport à la valeur de sortie de while
										// en effet la valeur comparée est celle QUI SERA UTILISEE et pas celle qui vient de l'être... du coup si on 
										// souhaite inclure la valeur de threshold dans les valeurs utilisées par vdelta, il faut comparer avec (threshold/2).

	Nf32 vdelta_threshold = 0.0005f;	// "= threshold demandée / 2 " ( voir juste au dessus pour xeplication ) (1e-3)/2
	Nf32 upds_threshold = 0.01f;	

	Nu32 found = 0;

	// TODO:	la méthode employée ici est itérative.  On cherche le motion profile SUP_FLAT_SDOWN optimal en ajustant la vitesse max par petites touches...
	//			.. mais il est possible de trouver le profil IDEAL/OPTIMAL directement ! ( et savoir si il existe ): SUP_SDOWN 
	//			.. peut-être commencer par cela ici ( je l'ai  fait puis, mis de côté pour cette solution plus simple ... )
	//			.. Mais quand tout sera OK cela vaut le coup de faire cela ici en proposant un mode "PRECIS" par exemple pour que cela ne soit pas forcement actif
	//			.. si on ne veut pas. Et vu qu'on va devoir rendre certaines constantes/threshold parametrables, on pourrait en profiter pour rajouter ce param'
	//			.. pour se faire revoir la pochette verte avec les profils A,B,C,D,E, etc ...

	do
	{
		switch (mp.sRamped(vmax, m_pVfrom, m_pVto))
		{
			case NLSMOTIONPROFILE_ID_ENUM::EMPTY:
				// mp.sRamped n'a pas trouvé de solution car vmax est trop grande par rapport à la distance disponible.
				// sUp et sDown se croisent et en conséquence fUp.m_ds est négatif, ce qui est négatif !!! Aîe ... 
				vmax -= vdelta;
				break;

			case NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN:
				vdelta = 0.0f;
				mpo = mp;
				found = 1;
				break;

			case NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN:
				mpo				= mp;
				found	= 1;
				// (vmax == plocal_limits->m_v) : signifie que le motion profile mp a été construit par mp.sRamped avec la vitesse max ( = plocal_limits->m_v).
				//								  il possède potentiellement une phase flat ( fUp ).
				// (mp.m_fUp.m_ds < 0.01f):		  signifie qu'une phase flat (fUp) existe et qu'elle s'étire sur une distance inférieure à xx m
				//								  si ce test entre effectivement en jeu c'est que vmax est inférieure à plocal_limits->m_v et donc que l'algo cherche
				//								  la vitesse la plus grande possible permettant, sur la distance disponible, de construire un mp. On sait que quand
				//								  cette plus grande vitesse possible est utilisée alors mp.fUp.m_ds = 0.0f, c'est à dire qu'elle ne peut être maintenue
				//								  ... Attention cette plus grande vitesse possible n'est pas la vitesse max donnée par plocal_limits->m_v mais la vitesse la plus grande
				//								  qu'on peut atteindre sur la distance séparant les deux VSTAGE. Ce qui veut dire que l'algo, par dichotomie va essayer de s'en approcher le plus possible.
				//								  ... sans jamais vraiment l'atteindre précisement ( ou alors, coup de chance mais dans ce cas nous ne passons pas ici mais 
				//								  plus haut dans "case NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN:". Du coup on fixe un seuil mini pour 'fUp.m_ds' seuil à partir duquel
				//								  on va considérer que même si le motion profile n'est pas de type SUP_SDOWN  il peut être considérer comme "presque" SUP_SDOWN
				//								  et donc être optimal dans ce cas. Bien sûr, et pour être parfaitement clair, un mp SUP_FLAT_SDOWN avec vmax = plocal_limits->m_v
				//								  ( voir un peu plus haut ) est evidement optimal !
				if ((vmax == vhigh/*plocal_limits->m_v*/) || (mp.m_fUp.m_ds < upds_threshold)) // TODO: 0.01f à rendre parametrable !!! 0.01f m = 1 cm !!!
				{
					vdelta = 0.0f;		// permet de sortir de la boucle " while(vdelta > 0.001f) "
				}
				// vmax est inférieur à plocal_limits->m_v et fUp.m_ds est plus grand que le seuil minimum.
				// on considère donc qu'on peut potentiellement trouver mieux, c'est à dire augmenter vmax ce qui aura pour effet, si la distance disponible le permet
				// de réduire fUp.m_ds.
				else
				{
					vmax += vdelta;
				}
				break;
		}

		vdelta /= 2.0f;
	}while (vdelta > vdelta_threshold); // TODO: 0.001f à rendre parametrable !!! 0.001 m/s  =  1 mm par seconde !!!

	if (found)
	{
		if (!mpo.detectStageVelocityOvershoot(m_pVfrom, m_pVto, FLAG_NLVSTAGEX_INACCESSIBLE))
		{
			// FLAG_ON(mp.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION); ???? corrigé le 6/9/2021 c'est weird de flagger mp ???!!!
			FLAG_ON(mpo.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION);
			m_motionProfile = mpo;
			return NULL;
		}
		else
		{
			return getSplitVStage();
		}
	}
	else
	{
		return NULL;
	}
}

Nu32 NLKNODE::promote(NLKIN * pkbuffer)
{
	Nu32 r;
	if (getFullTimeSpan() <= getChildrenFullTimeSpan())
	//if(!m_pLeftChild)
	{
		// Y a t'il une flat shape d'amorcage ?
		Nf32 ds = m_pVfrom->m_s1Prime - m_pVfrom->m_s0Prime;
		//NErrorIf(ds < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
		if (ds > 0.0f)
		{
			NLFMOTIONSHAPE fprime;
			fprime.forwardVs(m_pVfrom->m_s0Prime, m_pVfrom->m_v, ds);
			r = fprime.promote(pkbuffer);
		}
		else
			r = 0;
		return (r + m_motionProfile.ppromote(&m_motionProfile, pkbuffer+r) );
	}
	else
	{
		r = m_pLeftChild->promote(pkbuffer);
		return ( r + m_pRightChild->promote(pkbuffer + r));
	}

}
