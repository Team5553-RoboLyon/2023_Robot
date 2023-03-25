#include "lib/N/NMemory.h"
#include "lib/N/Maths/NMath.h"
#include "lib/N/NErrorHandling.h"
// #include "lib/N/Utilities/Draw/NUT_Draw.h"
// #include "lib/N/Utilities/Draw/NUT_DrawPencil.h"

// #include "../NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "lib/NL/MotionControl/NLKin.h"



/*
NLKIN::~NLKIN()
{
}
*/



// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de KIN suppos� avoir une abscisse curviligne s situ�s entre l'abscisse curv. de pk0 et celle de pk1.
 *
 *	@param	pk0 est le	Kin suppos� �tre situ� "avant"
 * 	@param	pk1 est le	Kin suppos� �tre situ� "apr�s"
 *	@param	s est une abscisse curviligne suppos�e se trouver � l'int�rieur de l'intervalle [pk0->s, pk1->s]
 *			Dans le cas o� s serait inf�rieure � pk0->s, alors Kin sera une copie de pk0.
 *			Dans le cas o� s serait sup�rieure � pk1->s, alors Kin sera une copie de pk1. 
 *			Dans tous les autres cas, c'est � dire pk0->s < s < pk1->s, les valeur de kin seront calcul�es en fonction des donn�es contenues dans pk0 et pk1.
 */
 // ------------------------------------------------------------------------------------------
void NLKIN::atS(const NLKIN* pk0, const NLKIN* pk1, const Nf32 s)
{
	NErrorIf(pk0->m_t > pk1->m_t, NERROR_INCONSISTENT_VALUES);
	// le code est tr�s proche de celui de la fonction getTime.
	// C'est normal, puisqu'en fait on commence par trouver "t" localement � partir de "s" pour calculer ensuite les autres param�tres du kin.
	if (s <= pk0->m_s)
	{
		*this = *pk0;
		return;
	}
	else if (s < pk1->m_s)
	{
		Nf32	p, q, sqrt_dlt;

		if (!pk1->m_j)
		{
			if (!pk0->m_a)
			{
				//	NErrorIf(pk->m_a, NERROR_SYSTEM_CHECK);				// impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				//	NErrorIf(pk0->m_v != pk->m_v, NERROR_SYSTEM_CHECK); // impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				NErrorIf(!pk1->m_v, NERROR_SYSTEM_CHECK);			// Dans ce cas, la vitesse courante et donc constante ne saurait �tre nulle !
				from(pk0, (s - pk0->m_s) / pk0->m_v);
				return;
			}
			else
			{
				// Quelque soit le signe de "pk0->m_a" seule la racine " R2 = (-B + sqrt(delta) )/2A " est applicable � notre situation ( cf etude et tableau de signe )
				from(pk0, (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f * pk0->m_a * (pk0->m_s - s))) / pk0->m_a);
				return;
			}

			// C'est une phase 2 qui m�ne � pk !
			// On recherche donc une valeur de t solution de l'�quation:
			//	s = pk0->s + pk0->m_v*t + pk0->m_a*t*t/2.0f
			//	
			//	On a donc delta = NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) discriminant de l'�quation.
			//  Pour avoir au moins une soluiton, il faut que delta soit >=0
			//						delta	>=	0
			//	[1]					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0
			//						NPOW2(pk0->m_v) >=	2.0f*pk0->m_a*(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) >=	(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) - pk0->m_s >=	- s
			//	[2]				   -NPOW2(pk0->m_v)/(2.0f*pk0->m_a) + pk0->m_s <=	s
			//			
			/*
			if (pk0->m_a > 0.0f)
			{
				// Il y a TOUJOURS deux racines mais une seule positive !
				// ------------------------------------------------------
				// Dans [1] on voit bien que si pk0->m_a > 0 alors, - 2.0f*pk0->m_a*(pk0->m_s - s)	> 0, car (pk0->m_s - s) < 0 !
				// on a aussi NPOW2(pk0->m_v) >= 0, Donc
				//					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0 est TOUJOURS VRAI
				//
				//						delta  =	NPOW2(pk0->m_v) + qqchose ( avec qqchose = + 2.0f*NABS( pk0->m_a*(pk0->m_s - s) ) )
				//						donc,
				//						delta  >	NPOW2(pk0->m_v)
				// on aura donc TOUJOURS
				//						sqrt(delta) > pk0->m_v
				// donc,
				//						sqrt(delta) - pk0->m_v  > 0
				//
				// Donc,
				//						R2 =  (-pk0->m_v + sqrt(delta) ) / pk0->m_a		SERA TOUJOURS POSITIVE ( avec pk0->m_a positive )
				//
				// R2 = (-pk0->m_v + sqrt(delta) ) / pk0->m_a
				//
				// Quand � R1 = (-pk0->m_v - sqrt(delta) ) / pk0->m_a
				// On voit bien qu'elle sera TOUJOURS n�gative pour les m�mes raisons ,
				// c'est � dire
				//						-pk0->m_v - sqrt(delta) < 0, TOUJOURS ! donc ...
				//
				// Donc on a bien une seule racine positive et donc possible: R+ !
				return (-pk0->m_v + sqrt( NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) ) / pk0->m_a );
			}
			else // pk0->m_a < 0.0f
			{
				// Il est possible que delta soit negatif ou null !
				// ------------------------------------------------
				// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
				// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
				return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)) / pk0->m_a);
			}
			*/
		}
		else if (pk1->m_j > 0.0f)
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk1->m_j;
				q = (6.0f * (pk0->m_s - s)) / pk1->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				from(pk0, pk1->m_j, (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)));
				return;
			}
			else if (!pk1->m_a)
			{
				p = (6.0f * pk1->m_v) / pk1->m_j;
				q = (6.0f * (s - pk1->m_s)) / pk1->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				from(pk0, pk1->m_j, pk1->m_t - (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)) - pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk1->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f * pk0->m_v) / pk1->m_j - (3.0f * a02) / j2;
				q = (2.0f * a02 * pk0->m_a) / (j2 * pk1->m_j) - (6.0f * pk0->m_a * pk0->m_v) / j2 + (6.0f * (pk0->m_s - s)) / pk1->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				Nf32 r = (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
				from(pk0, pk1->m_j, r - pk0->m_a / pk1->m_j);
				return;
			}
		}
		else // pk->m_j < 0.0f
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk1->m_j;
				q = (6.0f * (pk0->m_s - s)) / pk1->m_j;
				from(pk0, pk1->m_j, 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3));
				return;
			}
			else if (!pk1->m_a)
			{
				p = (6.0f * pk1->m_v) / pk1->m_j;
				q = (6.0f * (s - pk1->m_s)) / pk1->m_j;
				from(pk0, pk1->m_j, pk1->m_t - 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3) - pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk1->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f * pk0->m_v) / pk1->m_j - (3.0f * a02) / j2;
				q = (2.0f * a02 * pk0->m_a) / (j2 * pk1->m_j) - (6.0f * pk0->m_a * pk0->m_v) / j2 + (6.0f * (pk0->m_s - s)) / pk1->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				Nf32 r = 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3);
				from(pk0, pk1->m_j, r - pk0->m_a / pk1->m_j);
				return;
			}
		}
	}
	else // s >= pk1->m_s
	{
		*this = *pk1;
		return;
	}
}

void NLKIN::atT(const NLKIN* pk0, const NLKIN* pk1, const Nf32 t)
{
	NErrorIf(pk0->m_t > pk1->m_t, NERROR_INCONSISTENT_VALUES);
	// le code est beaucoup plus simple que pour atS. Ici, pas besoin de trouver 't' il est pass� en param�tre !
	// il s'agit donc juste de calculer les valeurs du Kin en fonction de 't'!
	if (t <= pk0->m_t)
	{
		*this = *pk0;
		return;
	}
	else if (t < pk1->m_t)
	{
		from(pk0, t - pk0->m_t);
		return;
	}
	else // t >= pk1->m_t
	{
		*this = *pk1;
		return;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de KIN depuis le KIN pk0 en appliquant 'jerk' sur une duree "dt"
 *
 *	@param	pk0 est le	Kin "origine"
 *	@param	jerk est le	Jerk a appliquer ( m/s/s/s )
 *	@param	dt est la Duree separant pk0 et KIN
 */
 // ------------------------------------------------------------------------------------------
void NLKIN::from(const NLKIN* pk0, const Nf32 jerk, const Nf32 dt)
{
	Nf32 dt2 = dt * dt;

	m_t = pk0->m_t + dt;
	m_a = pk0->m_a + jerk * dt;
	m_v = pk0->m_v + pk0->m_a * dt + (jerk * dt2) / 2.0f;
	m_s = pk0->m_s + pk0->m_v * dt + (pk0->m_a * dt2) / 2.0f + (jerk * dt2 * dt) / 6.0f;
	m_j = jerk;
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de KIN depuis le KIN pk0 en appliquant un \b Jerk nul sur une duree "dt"
 *			Cette fonction fait la meme chose que NLKIN::from(pk0, 0, dt) de maniere plus optimisee.
 *	@param	pk0 est le	Kin "origine"
 *	@param	dt est la Duree separant pk0 et KIN
 */
 // ------------------------------------------------------------------------------------------
void NLKIN::from(const NLKIN *pk0, const Nf32 dt)
{
	Nf32 dt2 = dt * dt;

	m_t = pk0->m_t + dt;
	m_a = pk0->m_a;
	m_v = pk0->m_v + pk0->m_a*dt;
	m_s = pk0->m_s + pk0->m_v*dt + (pk0->m_a*dt2) / 2.0f;
	m_j = 0.0f;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de pk1 depuis KIN en appliquant 'jerk' sur une duree "dt"
 *
 *	@param		pk1 est le	Kin "cible". Son contenu ( position,vitesse, acceleration,et date ) seront calcules et mis a jour a partir de KIN.
 *	@param		jerk est le	Jerk a appliquer ( m/s/s/s )
 *	@param		dt est la Duree separant KIN et pk1
 *	@return		le pointeur pk1 passe en argument est simplement retourne en sortie de fonction
 */
 // ------------------------------------------------------------------------------------------
NLKIN* NLKIN::to(NLKIN *pk1, const Nf32 jerk, const Nf32 dt)
{
	Nf32 dt2 = dt * dt;

	pk1->m_t = m_t + dt;
	pk1->m_a = m_a + jerk * dt;
	pk1->m_v = m_v + m_a * dt + (jerk*dt2) / 2.0f;
	pk1->m_s = m_s + m_v * dt + (m_a*dt2) / 2.0f + (jerk*dt2*dt) / 6.0f;
	pk1->m_j = jerk;

	return pk1;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de pk1 depuis KIN en appliquant un \b Jerk nul sur une duree "dt"
 *			Cette fonction fait la meme chose que NLKIN::to(pk1, 0, dt) de maniere plus optimis�e.
 *	@param	pk1 est le	Kin "cible"
 *	@param	dt est la Duree separant KIN et pk1
 *	@return	le pointeur pk1 passe en argument est simplement retourne en sortie de fonction
 */
 // ------------------------------------------------------------------------------------------
NLKIN* NLKIN::to(NLKIN *pk1, const Nf32 dt)
{
	Nf32 dt2 = dt * dt;

	pk1->m_t = m_t + dt;
	pk1->m_a = m_a;
	pk1->m_v = m_v + m_a * dt;
	pk1->m_s = m_s + m_v * dt + (m_a*dt2) / 2.0f;
	pk1->m_j = 0.0f;

	return pk1;
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie que les valeurs d'Acceleration et de Vitesse contenues dans KIN respectent les limites fixees par NLKINLIMITS passee en argument.
 *			\b ATTENTION! Cette fonction ne fait rien de plus que tester si l'acceleration  et potentiellement la vitesse actuelle(s) du KIN sont superieure(s), ou pas, aux limites fix�es.
 *			L'acceleration est testee en premier. La vitesse est testee dans un second temps si et seulement si l'acceleration est OK.
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return	Une des valeurs de l'enum NLKIN::CHECKS.
 *			VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites. 
 *			ACCELERATION_OVERSHOOT		Signifie que l'acceleration actuelle est hors limite. La velocite n'a pas ete testee car le resultat du test de l'acceleration est negatif.
 *			VELOCITY_OVERSHOOT			Signifie que la vitesse actuelle est hors limite. L'acceleration est alors obligatoirement OK car testee avant la vitesse.
 */
 // ------------------------------------------------------------------------------------------
const Nu32 NLKIN::accVelfastCheck(const NLKINLIMITS * pkmax)
{
	if (NABS(m_a) > pkmax->m_a)
		return NLKIN::ACCELERATION_OVERSHOOT;

	if (NABS(m_v) > pkmax->m_v)
		return NLKIN::VELOCITY_OVERSHOOT;

	return NLKIN::VELOCITY_ACCELERATION_OK;
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie que les valeurs d'Acceleration et de Vitesse contenues dans KIN respectent les limites fixees par NLKINLIMITS passee en argument.
 *			\b ATTENTION! Cette fonction ne fait rien de plus que tester si l'acceleration  et la vitesse actuelle(s) du KIN sont superieure(s), ou pas, aux limites fixees.
 *			L'acceleration et la vitesse sont systematiquement testees toutes les deux. 
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return	Une \b combinaison des valeurs de l'enum NLKIN::CHECKS.
 *			+ \b VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites.
 *			+ \b ACCELERATION_OVERSHOOT		Signifie que l'acceleration actuelle est hors limite, la velocite est OK.
 *			+ \b VELOCITY_OVERSHOOT			Signifie que la vitesse actuelle est hors limite. L'acceleration est OK.
 *			+ \b ACCELERATION_OVERSHOOT|VELOCITY_OVERSHOOT	Signifie que l'acceleration et la vitesse actuelles sont hors limite.
 */
 // ------------------------------------------------------------------------------------------
const Nu32 NLKIN::accVelCheck(const NLKINLIMITS * pkmax)
{
	Nu32 ret = 0;
	if (NABS(m_a) > pkmax->m_a)
		ret |= NLKIN::ACCELERATION_OVERSHOOT;

	if (NABS(m_v) > pkmax->m_v)
		ret |= NLKIN::VELOCITY_OVERSHOOT;

	return ret;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie que les valeurs d'Acceleration et de Vitesse contenues dans KIN respectent les limites fixees par NLKINLIMITS passee en argument.
 *			Cette fonction, en plus de  tester si l'acceleration  et la vitesse actuelle(s) du KIN sont superieure(s), ou pas, aux limites fixees, va verifier la coh�rence de l'acceleration et de la vitesse au regard de ces limites.
 *			L'acceleration est testee en premier. La vitesse est testee dans un second temps si et seulement si l'acceleration est OK.
 *
 * ## Precision sur les tests effectues
 * Si la vitesse de pk0 est proche de la limite sup�rieure de vitesse, plus precisement dans la zone ou l'acceleration evolue de amax � -amax en passant par 0...
 * Il est important de verifier la coh�rence acceleration / vitesse de pk0. En effet,
 *					+ si, pk0->m_a > 0	il faut veiller a ce que la reduction de m_a � 0 n'entraine pas du meme coup un d�passement de vitesse.
 *										Ce qui revient a dire que les valeurs de m_a et de m_v sont incoherentes.
 *					+ si, pk0->m_a < 0	il faut verifier la valeur de la vitesse quand a etait nulle ! Elle doit etre inferieure a la vitesse max autorisee.
 *										En effet, imaginons une valeur de vitesse egale a la vitesse max ou presque ( donc pas de depassement ) et une valeur d'acceleration negative.( pas de depassement non plus )
 *										ce cas de figure est des plus etrange, et signifie une chose: la vitesse 'fut' en d�passement.
 *										Pour verifier ce cas de figure, on recherche la valeur de la vitesse d'origine "dans le passe" quand l'acceleration "etait" nulle.
 * ## Dans un premier temps, la fonction verifie:	
 *					+ -vmax <= v <= vmax
 *					+ -amax <= a <= amax
 *
 * Plus precisement, si v est comprise dans l'intervalle [ (-vmax + vx ),  (vmax-vx) ] il n'y a aucun probl�me possible. Car,l'acceleration,respectant ses valeurs limites ne pourra jamais amener la vitesse en dehors de ses propres limites.
 *				car, vx = amax*amax/2j
 * Par contre, si v est proche de vmax ou de -vmax et plus exactement si v est dans ] vmax-vx, vmax] ou dans ]-vmax + vx, -vmax ], alors il est possible qu'une valeur incoherente d'acceleration entraine la vitesse vers une valeur hors limite.
 *				
 *				dvatozero = | a*a/2j |
 *
 *				SI v > vmax - vx,
 *				on veut donc s'assurer que, 
 *					v + dvatozero	<= vmax	quand 'a'  est positive ( sinon OVERSHOOT dans le futur  ! )
 *					v + dvatozero	<= vmax	quand 'a'  est negative ( sinon OVERSHOOTED dans le pass� ! )
 *											v + a*a/2j		<= vmax
 *												a*a/2j		<= vmax-v
 *												a*a			<= (vmax-v)*2j
 *
 *				Donc la vitesse sera hors limite si,
 *												a*a > (vmax-v)*2j
 *
 *				SI v < -vmax + vx,
 *				on veut donc s'assurer que, v - dvatozero	>= -vmax
 *											v - a*a/2j		>= -vmax
 *											  -	a*a/2j		>= -vmax-v
 *											  -	a*a/2j		>= -( vmax+v )
 *												a*a			<=  ( vmax+v )*2j
 *
 *				Donc la vitesse sera hors limite si,
 *												a*a > (vmax+v)*2j
 *
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return		Une des valeurs de l'enum NLKIN::CHECKS.
 *			+ \b VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites.
 *			+ \b ACCELERATION_OVERSHOOT		Signifie que l'acceleration actuelle est hors limite, la velocite est OK.
 *			+ \b VELOCITY_OVERSHOOT			Signifie que la vitesse actuelle est hors limite. L'acceleration est OK.
 *			+ \b VELOCITY_WAS_OVERSHOOTED	Signifie que la vitesse et l'acceleration actuelle "semblent" OK. Mais qu'il y a une incoh�rence entre les 2 qui ne s'explique que si la vitesse "etait" hors limite.
 *			+ \b VELOCITY_WILL_OVERSHOOT	Signifie que la vitesse et l'acceleration actuelle "semblent" OK. Mais qu'il y a une incoh�rence entre les 2 qui entrainera forcement la vitesse hors des limites fixees.
 */
 // ------------------------------------------------------------------------------------------
const Nu32 NLKIN::accVelFastFullCheck(const NLKINLIMITS *pkmax)
{
	if (NABS(m_a) > pkmax->m_a)
		return NLKIN::ACCELERATION_OVERSHOOT;	// retourne immediatement le code d'erreur approprie si l'Acceleration actuelle est hors limite. Les tests plus avanc�s et les tests sur la vitesse ne sont pas effectu�s.

	if (NABS(m_v) > pkmax->m_v)
		return NLKIN::VELOCITY_OVERSHOOT;		// retourne immediatement le code d'erreur approprie si la vitesse actuelle est hors limite. Les tests plus avanc�s ne sont pas effectu�s.


	if (m_v > pkmax->m_vmvx)
	{
		if ((m_a*m_a) > ((pkmax->m_v - m_v)*pkmax->m_2j))
			return ((m_a > 0.0f) ? NLKIN::VELOCITY_WILL_OVERSHOOT : NLKIN::VELOCITY_WAS_OVERSHOOTED);
	}
	else if (m_v < -pkmax->m_vmvx )
	{
		if ((m_a*m_a) > ((pkmax->m_v + m_v)*pkmax->m_2j))
			return ((m_a > 0.0f) ? NLKIN::VELOCITY_WAS_OVERSHOOTED : NLKIN::VELOCITY_WILL_OVERSHOOT);
	}

	return  NLKIN::VELOCITY_ACCELERATION_OK;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie que les valeurs d'Acceleration et de Vitesse contenues dans KIN respectent les limites fixees par NLKINLIMITS passee en argument.
 *			\b ATTENTION! Cette fonction ne fait rien de plus que tester si l'acceleration  et la vitesse actuelle(s) du KIN sont superieure(s), ou pas, aux limites fixees.
 *			L'acceleration et la vitesse sont systematiquement testees toutes les deux.
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return	Une \b combinaison des valeurs de l'enum NLKIN::CHECKS.
 *			+ \b VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites.
 *			+ \b ACCELERATION_OVERSHOOT		Signifie que l'acceleration actuelle est hors limite, la velocite est OK.
 *			+ \b VELOCITY_OVERSHOOT			Signifie que la vitesse actuelle est hors limite. L'acceleration est OK.
 *			+ \b VELOCITY_WAS_OVERSHOOTED	Signifie que la vitesse et l'acceleration actuelle "semblent" OK. Mais qu'il y a une incoh�rence entre les 2 qui ne s'explique que si la vitesse "etait" hors limite.
 *			+ \b VELOCITY_WILL_OVERSHOOT	Signifie que la vitesse et l'acceleration actuelle "semblent" OK. Mais qu'il y a une incoh�rence entre les 2 qui entrainera forcement la vitesse hors des limites fixees..
*/
// ------------------------------------------------------------------------------------------
const Nu32 NLKIN::accVelFullCheck(const NLKINLIMITS *pkmax)
{
	Nu32 ret = 0;

	if (NABS(m_a) > pkmax->m_a)
		ret |= NLKIN::ACCELERATION_OVERSHOOT;

	if (NABS(m_v) > pkmax->m_v)
		ret |= NLKIN::VELOCITY_OVERSHOOT;

	if (m_v > pkmax->m_vmvx)
	{
		if ((m_a*m_a) > ((pkmax->m_v - m_v)*pkmax->m_2j))
			ret |= ((m_a > 0.0f) ? NLKIN::VELOCITY_WILL_OVERSHOOT : NLKIN::VELOCITY_WAS_OVERSHOOTED);
	}
	else if (m_v < (-pkmax->m_vmvx))
	{
		if ((m_a*m_a) > ((pkmax->m_v + m_v)*pkmax->m_2j))
			ret |= ((m_a > 0.0f) ? NLKIN::VELOCITY_WAS_OVERSHOOTED : NLKIN::VELOCITY_WILL_OVERSHOOT);
	}
	return ret;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie l'evolution de la vitesse du KIN.
 *			Cette fonction ne teste ni l'acceleration ni la vitesse actuelles.	
 *
 * Plus exactement la fonction verifie la valeur de la vitesse apres avoir ramene l'acceleration a 0 depuis sa valeur actuelle.
 *		Si la vitesse est alors hors limite alors la fonction renvoie un code d'erreur \b VELOCITY_WILL_OVERSHOOT
 *		Sinon la valeur VELOCITY_ACCELERATION_OK est retournee ( = 0 )
 *
 * Le temps necessaire pour amener l'acceleration a 0 est calcule \n
 * \li					t = pk0->m_a / jmax ( jmax etant la valeur de jerk maximum et constante. jmax = pkmax->m_j )
 *
 * La quantite de vitesse generee pendant cette duree \e t vaut:
 * \li					dvatozero = a * t = pk0->m_a * pk0->m_a / jmax
 *
 * Si \e a est positive alors cette quantite de vitesse sera ajoutee a la vitesse initiale pour obtenir la valeur de la vitesse quand \e a sera nulle.
 * A l'inverse, si \e a est negative alors cette quantite de vitesse sera soustraite a la vitesse initiale pour obtenir la valeur de la vitesse quand \e a sera nulle.
 * \li				a < 0,	v_azero = v - dvatozero
 * \li				a > 0,	v_azero = v + dvatozero
 * \li				a = 0,	v_azero = v
 *
 * La fonction teste alors la vitesse obtenue qui, pour etre valide, doit rester comprise entre -vmax et vmax.
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return	Une des valeurs de l'enum NLKIN::CHECKS.
 *			+ \b VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites.
 *			+ \b VELOCITY_WILL_OVERSHOOT	Signifie que la vitesse sera forcement entrainee hors des limites fixees par l'acceleration actuelle qui est donc incoherente.
*/
// ------------------------------------------------------------------------------------------
const Nu32 NLKIN::velForwardCheck(const NLKINLIMITS *pkmax)
{
	// Calcul de "dvatozero", c'est � dire du differentiel ( gain ou perte) de vitesse acquise/perdue lors de la transition de la valeur courante de l'acceleration a ZERO.
	// ( le signe de "a" permet de signer dvatozero ) 
	Nf32 dvatozero = (m_a < 0.0f) ? -(m_a*m_a) / pkmax->m_2j : (m_a*m_a) / pkmax->m_2j;

	if (NABS(m_v + dvatozero) > pkmax->m_v)
		return  NLKIN::VELOCITY_WILL_OVERSHOOT;	

	return  NLKIN::VELOCITY_ACCELERATION_OK;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Verifie l'evolution de la vitesse du KIN.
 *			Cette fonction ne teste ni l'acceleration ni la vitesse actuelles.
 *
 * Plus exactement la fonction va verifier la valeur que la vitesse "a eut" quand l'acceleration "valait" 0.
 *		Si la vitesse �tait alors hors limite alors la fonction "backward check" renvoie un code d'erreur "VELOCITY_WAS_OVERSHOOTED"
 *		Sinon la valeur ZERO est retourn�e ( 0 = OK )
 *
 * ## Details
 * le temps n�c�ssaire pour revenir en arriere ( dans le temps) � l'instant o� l'acceleration �tait nulle est d'abord calcule \n
 * \li					t = pk0->m_a / jmax ( jmax �tant la valeur de jerk maximum et constante. jmax = pkmax->m_j )
 *
 * La quantit� de vitesse g�n�r�e pendant cette dur�e \e t vaut:
 * \li					dvatozero = a * t = pk0->m_a * pk0->m_a / jmax
 *
 * Si \e a est positive alors cette quantite de vitesse sera enlevee a la vitesse initiale pour obtenir la valeur de la vitesse quand \e a \b etait nulle.
 * A l'inverse, si \e a est negative alors cette quantite de vitesse sera ajoutee a la vitesse initiale pour obtenir la valeur de la vitesse quand \e a \b etait nulle.
 * \li				a < 0,	v_azero = v + dvatozero
 * \li				a > 0,	v_azero = v - dvatozero
 * \li				a = 0,	v_azero = v
 *
 * Reste alors � tester la vitesse obtenue qui, pour �tre valide doit rester comprise entre -vmax et vmax.
 *	@param	pkmax regroupe les valeurs maximales d'Acceleration, de Vitesse ( ou Velocite ) et de Jerk d'un systeme.
 *	@return	Une des valeurs de l'enum NLKIN::CHECKS.
 *			+ \b VELOCITY_ACCELERATION_OK	(= 0) Signifie que la vitesse et l'acceleration respectent toutes les deux les limites.
 *			+ \b VELOCITY_WAS_OVERSHOOTED	Signifie que la vitesse etait forcement hors des limites fixees au regard de l'acceleration.
*/
// ------------------------------------------------------------------------------------------
const Nu32 NLKIN::velBackwardCheck(const NLKINLIMITS *pkmax)
{
	// Calcul de "dvafromzero", c'est � dire le diff�rentiel ( gain ou perte) de vitesse acquise/perdue lors de la transition de l'acceleration nulle � la valeur courante de l'acceleration.
	// ( le signe de "a" permet de signer dvafromzero ) 
	Nf32 dvafromzero = (m_a < 0.0f) ? (m_a*m_a) / pkmax->m_2j : -(m_a*m_a) / pkmax->m_2j;

	if (NABS(m_v + dvafromzero) > pkmax->m_v)
		return NLKIN::VELOCITY_WAS_OVERSHOOTED;

	return  NLKIN::VELOCITY_ACCELERATION_OK;
}

#ifdef _NEDITOR
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine un Kin.
 *
 *	@param	p2docs
 *	@param	pk0
 *	@param	pickack	couleur id0	: index de la couleur j > 0 
 *					couleur id1	: index de la couleur ==  0 
 *					couleur id2	: index de la couleur j < 0 
 *	@return	
 */
// ------------------------------------------------------------------------------------------
void NLKIN::draw(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack, const NLKIN* pk0)
{
	Nf32 dt = m_t - pk0->m_t;
	if (dt)
	{
		Nf32			a0, v0, s0, t0, t;
		Nu32			i;
		Nu32			iter = 32;
		NUTDRAWVERTEX	p, o;

		NVEC2 xt = { 0.005f,0.005f };

		o.Position_3f.z = p.Position_3f.z = 0.0f;

		// Couleurs
		if (m_j > 0.0f)
			p2docs->getModulatedColor(&p.Color0_4f,0, pickpack);
		else if (m_j == 0.0f)
			p2docs->getModulatedColor(&p.Color0_4f, 1, pickpack);
		else
			p2docs->getModulatedColor(&p.Color0_4f, 2, pickpack);
		o.Color0_4f = p.Color0_4f;

		// On dessine par rapport � T
		if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
		{
			// t:
			o.Position_3f.x = p2docs->transformX(m_t, NL2DOCS_COORDS_STYLE_0);

			// V(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_VEL))
			{
				// M�thode B:
				o.Position_3f.y = p2docs->transformY(m_v, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					v0 = m_v - m_a * t + 0.5f*m_j*t*t;
					t0 = m_t - t;
					p.Position_3f.x = p2docs->transformX(t0);
					p.Position_3f.y = p2docs->transformY(v0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// A(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_ACCEL))
			{
				// M�thode B:
				o.Position_3f.y = p2docs->transformY(m_a, NL2DOCS_SUBSCALE_0, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					a0 = m_a - m_j * t;
					t0 = m_t - t;
					p.Position_3f.x = p2docs->transformX(t0);
					p.Position_3f.y = p2docs->transformY(a0, NL2DOCS_SUBSCALE_0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// J(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_JERK))
			{
				// M�thode B:
				o.Position_3f.y = p2docs->transformY(m_j, NL2DOCS_SUBSCALE_1, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					t0 = m_t - t;
					p.Position_3f.x = p2docs->transformX(t0);
					p.Position_3f.y = p2docs->transformY(m_j, NL2DOCS_SUBSCALE_1);
					NUT_DrawPencil_LineTo(&p);
				}
			}
		}
		else // On dessine par rapport � S (par defaut) 
		{
			// s:
			o.Position_3f.x = p2docs->transformX(m_s, NL2DOCS_COORDS_STYLE_0);

			// V(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_VEL))
			{
				// M�thode B:
				o.Position_3f.y = p2docs->transformY(m_v, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					v0 = m_v - m_a * t + 0.5f*m_j*t*t;
					s0 = m_s - m_v * t + 0.5f*m_a*t*t - m_j * t*t*t / 6.0f;
					p.Position_3f.x = p2docs->transformX(s0);
					p.Position_3f.y = p2docs->transformY(v0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// A(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_ACCEL))
			{
				o.Position_3f.y = p2docs->transformY(m_a, NL2DOCS_SUBSCALE_0, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					a0 = m_a - m_j * t;
					s0 = m_s - m_v * t + 0.5f*m_a*t*t - m_j * t*t*t / 6.0f;
					p.Position_3f.x = p2docs->transformX(s0);
					p.Position_3f.y = p2docs->transformY(a0, NL2DOCS_SUBSCALE_0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// J(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_JERK))
			{
				// M�thode B:
				o.Position_3f.y = p2docs->transformY(m_j, NL2DOCS_SUBSCALE_1, NL2DOCS_COORDS_STYLE_1);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					//a0 = m_a - m_j * t;
					s0 = m_s - m_v * t + 0.5f*m_a*t*t - m_j * t*t*t / 6.0f;
					p.Position_3f.x = p2docs->transformX(s0);
					p.Position_3f.y = p2docs->transformY(m_j, NL2DOCS_SUBSCALE_1);
					NUT_DrawPencil_LineTo(&p);
				}
			}
		}
	}
}

void NLKIN::verticalDraw(NL2DOCS* p2docs, const NVEC2* po, const NCOLORPICKPACK pickpack, const NLKIN* pk0)
{
	Nf32 dt = m_t - pk0->m_t;
	if (dt)
	{
		Nf32			a0, v0, s0, t0, t;
		Nu32			i;
		Nu32			iter = 32;
		NUTDRAWVERTEX	p, o;
		NVEC2			xt = { 0.005f,0.005f };

		o.Position_3f.z = p.Position_3f.z = 0.0f;

		//	-----------------------------------------------------
		// 
		//	Couleurs
		//
		if (m_j > 0.0f)
			p2docs->getModulatedColor(&p.Color0_4f, 0, pickpack);
		else if (m_j == 0.0f)
			p2docs->getModulatedColor(&p.Color0_4f, 1, pickpack);
		else
			p2docs->getModulatedColor(&p.Color0_4f, 2, pickpack);
		o.Color0_4f = p.Color0_4f;

		//	-----------------------------------------------------
		// 
		// Shift d'origine
		//
		NVEC2 shiftorgn;
		shiftorgn.x = p2docs->transformX(po->x);
		shiftorgn.y = p2docs->transformY(po->y);

		//	-----------------------------------------------------
		// 
		// On dessine par rapport � T
		//
		if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
		{
			// t:
			o.Position_3f.y = shiftorgn.y + p2docs->transformVy(-m_t);

			// V(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_VEL))
			{
				// M�thode B:
				o.Position_3f.x = shiftorgn.x + p2docs->transformVx(m_v);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					v0 = m_v - m_a * t + 0.5f*m_j*t*t;
					t0 = m_t - t;
					p.Position_3f.x = shiftorgn.x+p2docs->transformVx(v0);
					p.Position_3f.y = shiftorgn.y+p2docs->transformVy(-t0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// A(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_ACCEL))
			{
				// M�thode B:
				o.Position_3f.x = shiftorgn.x+p2docs->transformVx(m_a);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					a0 = m_a - m_j * t;
					t0 = m_t - t;
					p.Position_3f.x = shiftorgn.x+p2docs->transformVx(a0);
					p.Position_3f.y = shiftorgn.y+p2docs->transformVy(-t0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// J(t):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_JERK))
			{
				// M�thode B:
				o.Position_3f.x = shiftorgn.x+p2docs->transformVx(m_j);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					t0 = m_t - t;
					p.Position_3f.x = shiftorgn.x+p2docs->transformVx(m_j);
					p.Position_3f.y = shiftorgn.y+p2docs->transformVy(-t0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
		}
		else  // On dessine par rapport � S (par defaut) 
		{
			// s:
			o.Position_3f.y = shiftorgn.y+p2docs->transformVy(-m_s);
			
			// V(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_VEL))
			{
				// M�thode B:
				o.Position_3f.x = shiftorgn.x + p2docs->transformVx(m_v);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					v0 = m_v - m_a * t + 0.5f * m_j * t * t;
					s0 = m_s - m_v * t + 0.5f * m_a * t * t - m_j * t * t * t / 6.0f;
					p.Position_3f.x = shiftorgn.x+p2docs->transformVx(v0);
					p.Position_3f.y = shiftorgn.y+p2docs->transformVy(-s0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// A(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_ACCEL))
			{
				o.Position_3f.x = shiftorgn.x + p2docs->transformVx(m_a);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					a0 = m_a - m_j * t;
					s0 = m_s - m_v * t + 0.5f * m_a * t * t - m_j * t * t * t / 6.0f;
					p.Position_3f.x = shiftorgn.x + p2docs->transformX(a0);
					p.Position_3f.y = shiftorgn.y + p2docs->transformY(-s0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
			// J(s):
			if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_JERK))
			{
				// M�thode B:
				o.Position_3f.x = shiftorgn.x + p2docs->transformY(m_j);
				NUT_Draw_Mark(&o.Position_3f, &xt, &o.Color0_4f);
				NUT_DrawPencil_From(&o);

				for (i = 0; i <= iter; i++)
				{
					t = dt * (Nf32)i / (Nf32)iter;
					//a0 = m_a - m_j * t;
					s0 = m_s - m_v * t + 0.5f * m_a * t * t - m_j * t * t * t / 6.0f;
					p.Position_3f.x = shiftorgn.x + p2docs->transformVx(m_j);
					p.Position_3f.y = shiftorgn.y + p2docs->transformVy(-s0);
					NUT_DrawPencil_LineTo(&p);
				}
			}
		}
	}
}

Nchar* NLKIN::print(Nchar* pstr)
{
	sprintf(pstr, "Kin[%X].t = %.4f\t.s = %.4f\t.v = %.4f\t.a = %.4f\t.j = %.4f", this, m_t, m_s, m_v, m_a, m_j);
	return pstr;
}

#endif