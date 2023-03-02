#include "../../N/NMemory.h"
#include "../../N/NMath.h"
#include "../../N/NErrorHandling.h"
#include "../../N/Core/NLimits.h"

#include "NLKinLimits.h"

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Constructeur par defaut de la classe. 
 */
 // ------------------------------------------------------------------------------------------
NLKINLIMITS::NLKINLIMITS()
{
	null();
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Utilise les valeurs 'par defaut' de vitesse max, acceleration max et jerk max pour
 *			calculer toutes les autres constantes de la classe.
 */
 // ------------------------------------------------------------------------------------------
/*
void NLKINLIMITS::default()
{
	m_v = DEFAULT_NLKINLIMITS_VELOCITY_MAX;
	m_a = DEFAULT_NLKINLIMITS_ACCELERATION_MAX;
	m_j = DEFAULT_NLKINLIMITS_JERK_MAX;
	
	compute();
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Constructeur specifique de la classe. 
 *
 *	A partir des valeurs de velocite, acceleration et jerk maximum, plusieurs constantes, utilisees regulierement, sont precalculees et stockees.
 *  Le nom de ces constantes se compose de la maniere suivante: \n 
 *	D'abord le prefixe "m_" suivit d'un ensemble de lettres et de chiffres. \n
 *	@li "2v" se lit "2 * v"		( 2 fois la vitesse max )
 *  @li "v2" se lit "v^2 "		( la vitesse max au carre )
 *  @li "2v2" se lit "2*v^2 "	( 2 fois la vitesse max au carre )
 *  @li "aooj" se lit "a/j "	( l'acceleration max divise par  (out of) le jerk max )
 *  @li "vmvx" se lit "v-vx "	( la vitesse max moins la vitesse 'vx' ) \n
 *	\n
 *	Certaines de ces constantes sont accessibles par 2 noms differents. Le premier nom est le nom 'standard' qui décrit le calcul dont la constante est le resultat.
 *	Le second nom est plus "explicite" quand à ce que represente la constante ( tx, vx ... ).\n
 *	\n
 *	@li \e a, \e v et \e j represente respectivement l'acceleration max, la vitesse max et le jerk max. \n
 *	@li \e tx est le temps necessaire pour passe d'une acceleration nulle a l'acceleration max, ( tx = a/j ). \n
 *	@li \e 2tx est simplement le double de tx, ( 2tx = 2a/j ). \n
 *  @li \e vx est la vitesse acquise au temps tx, depuis v0 = 0, a0 = 0, s=0 et t=0 ( vx = a^2/2j ). \n
 *  @li \e 2vx est simplement le double de vx. C'est la vitesse maximum depuis a=0,v=0 en passant par l'acceleration maximum (sans la maintenir!) et en revenant a une acceleration nulle.\n
 *  @li \e sx est la distance parcourue au temps \e tx, depuis v0 = 0, a0 = 0, s=0 et t=0 ( sx =  a^3/(6j^2) ). Dans ces conditions, la vitesse en \e sx vaut \e vx.\n
 *	@li \e sxx  est la portion de distance générée par \e vx pendant une durée \e tx. \n
 *	@li \e sxxx  est la portion de distance generee par la variation d'acceleration quand elle evolue de l'acceleration max à une valeur nulle. \n
 *	@li \e ss est la distance parcourue depuis v0 = 0, a0 = 0, s0=0 et t0=0 et jusqu'à v = 2vx, a = 0, s=ss et t=2tx .( Avec a = acceleration max et v = vx au temps tx ). \n
 *	@li \e 2ss est le double de ss et represente la distance parcourue depuis v0 = 0, a0 = 0 et jusqu'à v = 0, a = 0  en passant par une vitesse max de 2vx au temps 2tx. \n
 *
 *	\n
 *  <img src="kinlimits_detail.png" align="left"/>
 *  <div style="clear: both"></div>
 *	@param	velocity_max	est la velocite ( vitesse ) maximum possible.
 *	@param	accel_max		est l'acceleration maximum possible. 
 *	@param	jerk_max		est le jerk maximum appliquable.
 */
 // ------------------------------------------------------------------------------------------
NLKINLIMITS::NLKINLIMITS(const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max, const Nu32 build_flags)
{
	m_v = velocity_max;
	m_a = accel_max;
	m_j = jerk_max;

	if (build_flags)
		build(build_flags);
}

NLKINLIMITS::NLKINLIMITS(NLKINLIMITS const& tobecopied)
{
	NErrorIf(!tobecopied.isConsistent(), NERROR_INCONSISTENT_VALUES);
	*this = tobecopied;
}

NLKINLIMITS::NLKINLIMITS(const NLKINLIMITS* pklim,const Nu32 build_flags)
{
	if (pklim)
		*this = *pklim;
	else
	{
		m_v = 0.0f;
		m_a = 0.0f;
		m_j = 0.0f;
	}

	if (build_flags)
		build(build_flags);
}

/*
NLKINLIMITS::~NLKINLIMITS()
{
}
*/
void NLKINLIMITS::setVelocityMax(const Nf32 velocity_max)
{
	NErrorIf(!isConsistent(), NERROR_INCONSISTENT_VALUES);

	if (velocity_max < m_2vx)
	{
		m_v		= m_2vx;
		m_vmvx	= m_vx;
	}
	else
	{
		m_v		= velocity_max;
		m_vmvx	= m_v - m_vx;
	}
}

void NLKINLIMITS::setAccelerationMax(const Nf32 accel_max)
{
	NErrorIf(!isConsistent(), NERROR_INCONSISTENT_VALUES);

	m_a = accel_max;
	build(FLAG_FALLBACK_MIN|FLAG_MINIMIZE_CHANGE|FLAG_VELOCITY|FLAG_JERK);
}
void NLKINLIMITS::setJerkMax(const Nf32 jerk_max)
{
	NErrorIf(!isConsistent(), NERROR_INCONSISTENT_VALUES);

	m_j = jerk_max;
	build(FLAG_FALLBACK_MIN|FLAG_MINIMIZE_CHANGE|FLAG_VELOCITY|FLAG_ACCELERATION);
}

Nu32 NLKINLIMITS::setVelocityMax(const Nf32 velocity_max, const Nu32 build_flags)
{
	m_v = velocity_max;
	if (build_flags)
		return build(build_flags);
	else
		return 0; // 0 pour : aucun ajustement n'a été effectué
}

Nu32 NLKINLIMITS::setAccelerationMax(const Nf32 accel_max, const Nu32 build_flags)
{
	m_a = accel_max;
	if (build_flags)
		return build(build_flags);
	else
		return 0; // 0 pour : aucun ajustement n'a été effectué
}

Nu32 NLKINLIMITS::setJerkMax(const Nf32 jerk_max, const Nu32 build_flags)
{
	m_j = jerk_max;
	if (build_flags)
		return build(build_flags);
	else
		return 0; // 0 pour : aucun ajustement n'a été effectué
}

Nu32 NLKINLIMITS::build(const Nu32 build_flags)
{
	NErrorIf(!build_flags, NERROR_NULL_VALUE);
	Nu32 r = 0;

	// A)	Use Default values ?
	if (m_v <= 0.0f)
	{
		m_v = ISFLAG_ON(build_flags, FLAG_FALLBACK_MIN) ? DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX : DEFAULT_NLKINLIMITS_VELOCITY_MAX;
		r |= FLAG_VELOCITY;
	}

	if (m_a <= 0.0f)
	{
		m_a = ISFLAG_ON(build_flags, FLAG_FALLBACK_MIN) ? DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX : DEFAULT_NLKINLIMITS_ACCELERATION_MAX;
		r |= FLAG_ACCELERATION;
	}
	if (m_j <= 0.0f)
	{
		m_j = ISFLAG_ON(build_flags, FLAG_FALLBACK_MIN) ? DEFAULT_MIN_NLKINLIMITS_JERK_MAX : DEFAULT_NLKINLIMITS_JERK_MAX;
		r |= FLAG_JERK;
	}

	// B)	Check consistency ( cohérence des valeurs entre elles ):
	//		Pour que les valeurs soient jugées cohérentes, elles doivent vérifier :
	//									
	//							m_v >= m_a²/m_j
	//		
	//		Dans le cas contraire la fonction build recalcule une des 3 valeurs pour rétablir la cohérence.
	//		Afin d'orienter l'algorythme dans le choix de la valeur à recalculer en fonction des 2 autres, l'utilisateur
	//		a 3 flags à sa disposition :	
	//											FLAG_VELOCITY						+	Le plus prioritaire											
	// 											FLAG_ACCELERATION					+/-
	// 											FLAG_JERK							-	Le moins prioritaire
	// 
	//		Il peut, à sa guise set ON 1,2 ou 3 flags pour préciser quelle valeur il souhaite préserver.
	//		!ATTENTION! Si l'utilisateur demande de préserver 1 ou 2 valeurs, cela devrait tjrs être possible, sauf bien sûr
	//		si l'une d'elle est initialement negative ou nulle ( dans ce cas elle aura été déjà modifiée par la passe (A) )
	//		Par contre dans le cas d'une demande de préservation totale ( les 3 valeurs ) alors le JERK ne pourra pas être préserver.
	//		Ci-dessous la table de préservation et les valeurs qui seront ajustées en cas de besoin:
	// 
	//	Preservations TABLE												Valeur ajustée si besoin	
	//
	//		V	A	J															V	A	J
	//		.	.	.		Aucune requête de préservation		>>>				.	.	X
	//		.	.	X		Préserver le jerk					>>>				.	X	.
	//		.	X	.		Préserver l'Acceleration			>>>				.	.	X
	//		.	X	X		Préserver Acceleration & Jerk		>>>				X	.	.
	//		X	.	.		Préserver la vitesse				>>>				.	.	X
	//		X	.	X		Préserver Vitesse & Jerk			>>>				.	X	.		
	//		X	X	.		Préserver Vitesse & Acceleration	>>>				.	.	X		
	//		X	X	X		Tout Préserver						>>>				.	.	X		
	// 
	m_2vx = (m_a * m_a) / m_j;								// = a²/j
	if (m_v < m_2vx)
	{	
		// L'utilisateur demande à minimiser les changements 
		// et des changements ( valeurs négatives ou nulles ajustées ) ont déjà eut lieu
		// On va donc continuer à modifier une valeur déjà modifiée, même si cela ne correspond pas 
		// zux flags de preservation passés par l'utilisateur: 
		if (r & ISFLAG_ON(build_flags, FLAG_MINIMIZE_CHANGE))
		{
			if (ISFLAG_ON(r, FLAG_JERK))				// Le jerk a déjà subit un ajustement, on continue avec lui ...
				m_j = (m_a * m_a) / m_v;				//		Vitesse et acceleration sont préservés, le jerk est ajusté.				
			else if (ISFLAG_ON(r, FLAG_ACCELERATION))	// L'acceleration a déjà subit un ajustement, on continue avec elle ...
				m_a = sqrt(m_j * m_v);					//		Vitesse et jerk sont préservés, l'acceleration est ajustée.
			else										// La vitesse a déjà subit un ajustement, on continue avec elle ...
			{
				NErrorIf(ISFLAG_OFF(r, FLAG_VELOCITY), NERROR_INCONSISTENT_FLAGS);
				m_v = (m_a * m_a) / m_j;				//		Acceleration et Jerk sont préservés, la vitesse est ajustée.				
			}
		}
		// L'utilisateur ne précise pas de minimiser les changements du coup on ne tient pas compte des ajustements précédents potentiels
		// On prend par contre en considération les flags de preservation passés par l'utilisateur.
		else
		{
			if (FLAGS_TEST(build_flags, MASK_PRESERVE, FLAG_ACCELERATION | FLAG_JERK))
			{
				m_v = (m_a * m_a) / m_j;	// Acceleration et Jerk sont préservés, la vitesse est ajustée.				
				r |= FLAG_VELOCITY;
			}
			else if (FLAGS_TEST(build_flags, FLAG_ACCELERATION | FLAG_JERK, FLAG_JERK))
			{
				m_a = sqrt(m_j * m_v);		// Vitesse et jerk sont préservés, l'acceleration est ajustée.
				r |= FLAG_ACCELERATION;
			}
			else
			{
				m_j = (m_a * m_a) / m_v;	// Vitesse et acceleration sont préservés, le jerk est ajusté.				
				r |= FLAG_JERK;
			}
		}
		m_2vx = (m_a * m_a) / m_j;		// = a²/j
	}

	// Précalcul constantes
	// ------------------------------------------------------------------------------------------------
	// groupe 'j only':
	//
	m_2j = 2.0f * m_j;									// = 2j

	// ------------------------------------------------------------------------------------------------
	//	groupe 'a only':
	//
	m_a2 = m_a * m_a;									// = a²
	m_4a = 4.0f * m_a;									// = 4a	
	m_a4 = m_a * m_a * m_a * m_a;						// = a^4

	// groupe 'v only':
	
	// ------------------------------------------------------------------------------------------------
	//	groupe 'a et j':
	//
	m_tx = m_a / m_j;									// = a/j
	m_sx = (m_a * m_a * m_a) / (6.0f * m_j * m_j);		// = a^3/6j²
	m_2ss = 2.0f * (m_a * m_a * m_a) / (m_j * m_j);		// = 2a^3/j²
	m_vx = (m_a * m_a) / (2.0f * m_j);					// = a²/2j
	//m_2vx = (m_a * m_a) / m_j;						// = a²/j	..... calculé un peu plus haut pour vérifier la cohérence m_v, m_a et m_j

	m_2a2ooj = (2.0f * m_a * m_a) / m_j;				// = 2a²/j
	m_a4ooj2 = (m_a * m_a * m_a * m_a) / (m_j * m_j);	// = a^4/j²
	m_4j2a = 4.0f * m_j * m_j * m_a;					// = 4j²a

	// ------------------------------------------------------------------------------------------------
	//	groupe 'v et a et j':
	//
	m_vmvx = m_v - m_vx;								// = v - a²/2j

	// ------------------------------------------------------------------------------------------------
	//	groupe 'obsolet':
	//
	//m_2tx = 2.0f * m_a / m_j;							// = 2a/j
	//m_sxx = (m_a * m_a * m_a) / (2.0f * m_j * m_j);	// = a^3/2j²
	//m_sxxx = (m_a * m_a * m_a) / (3.0f * m_j * m_j);	// = a^3/3j²
	//m_ss = (m_a * m_a * m_a) / (m_j * m_j);			// = a^3/j²
	return r;
}

Nbool NLKINLIMITS::isConsistent() const
{
	if (m_v < m_2vx)
		return NFALSE;
	else if (m_2vx != (m_a * m_a) / m_j)
		return NFALSE;
	else
		return NTRUE;
}

void NLKINLIMITS::null()
{
	m_v = 0.0f;
	m_a = 0.0f;
	m_j = 0.0f;

	// setup constantes
	m_tx		= NF32_MAX;		// = a/j		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_vx		= NF32_MAX;		// = a²/2j		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_2vx		= NF32_MAX;		// = a²/j		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_sx		= NF32_MAX;		// = a^3/6j²	>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_2ss		= NF32_MAX;		// = 2a^3/j²	>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.

	m_2j		= 0.0f;			// = 2j			>>> = j vaut 0 donc ...
	m_vmvx		=-NF32_MAX;		// = v - a²/2j	>>> = -NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.

	m_2a2ooj	= NF32_MAX;		// = 2a²/j		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_a4ooj2	= NF32_MAX;		// = a^4/j²		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	m_4j2a		= 0.0f;			// = 4j²a		>>> = j et a valent 0 donc ...

	m_a2		= 0.0f;			// = a²			>>> = a vaut 0 donc ...
	m_4a		= 0.0f;			// = 4a			>>> = a vaut 0 donc ...
	m_a4		= 0.0f;			// = a^4		>>> = a vaut 0 donc ...

	//m_2tx		= NF32_MAX;			// = 2a/j		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	//m_sxx		= NF32_MAX;			// = a^3/2j²	>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	//m_sxxx	= NF32_MAX;		// = a^3/3j²	>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
	//m_ss		= NF32_MAX;			// = a^3/j²		>>> = NF32_MAX mais n'est pas défini car le résultat d'une division par ZERO n'est pas défini.
}


