#pragma once
#include "../../N/NType.h"
#include "../../N/NFlags.h"

class NLKINLIMITS
{
	friend class NLMOTIONCONTROLER;
	friend class NLKIN;
	//friend class NLMOTIONPROFILE;
	friend class NLTRAJECTORY;
	friend class NLPATH;

	friend struct NLKNODE;
	friend struct NLSMOTIONSHAPE;
	friend struct NLFMOTIONSHAPE;
	friend struct NLSMOTIONPROFILE;
	friend struct NLVSTAGEX;

	#define DEFAULT_NLKINLIMITS_VELOCITY_MAX				3.0f		// Valeurs "par défaut".
	#define DEFAULT_NLKINLIMITS_ACCELERATION_MAX			5.0f		// 
	#define DEFAULT_NLKINLIMITS_JERK_MAX					30.0f		// 
																		// Valeurs "par défaut" minimum.
	#define DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX			1e-3f		// 
	#define DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX		1e-3f		// 
	#define DEFAULT_MIN_NLKINLIMITS_JERK_MAX				1e-3f		//

	#define FLAG_VELOCITY									BIT_0		// |
	#define FLAG_ACCELERATION								BIT_1		// |-	Flags utilisés par la fonction build en paramètres d'entrée ( preservation) et de sortie ( indication valeurs ajustées )  
	#define FLAG_JERK										BIT_2		// |
	#define FLAG_MINIMIZE_CHANGE							BIT_3
	#define	FLAG_FALLBACK_MIN								BIT_4		//		Flag utilisé par la fonction build en entrée pour définir les valeurs MINIMUM comme valeur par défaut plutôt que les valeurs ... par défaut.
																		//		(  cf DEFAULT_NLKINLIMITS_... et DEFAULT_MIN_NLKINLIMITS_... )
	#define FLAG_AUTO										BIT_5		// si 'build_flags = 0' alors 'build' n'est pas appelé. 
																		// 'FLAG_AUTO' set to ON permet que build soit appelé sans aucune directives particulières.
	
	#define MASK_FALLBACK									(BIT_4)
	#define MASK_PRESERVE									(BIT_0|BIT_1|BIT_2)

public:
	NLKINLIMITS();
	NLKINLIMITS(const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max, const Nu32 build_flags= FLAG_AUTO);
	NLKINLIMITS(const NLKINLIMITS *pklim, const Nu32 build_flags= FLAG_AUTO);
	NLKINLIMITS(NLKINLIMITS const& tocopy );
	//~NLKINLIMITS();
	void null();

	

	Nu32 setVelocityMax(const Nf32 velocity_max, const Nu32 build_flags);
	void setVelocityMax(const Nf32 velocity_max);

	Nu32 setAccelerationMax(const Nf32 accel_max, const Nu32 build_flags);
	void setAccelerationMax(const Nf32 accel_max);

	Nu32 setJerkMax(const Nf32 jerk_max, const Nu32 build_flags);
	void setJerkMax(const Nf32 jerk_max);

	Nu32 build(const Nu32 build_flags);

	Nbool isConsistent()const;

	inline	Nf32  getVelocityMax() const { return m_v; }
	inline	Nf32  getAccelerationMax() const { return m_a; }
	inline	Nf32  getJerkMax() const { return m_j; }

	Nf32		m_v;			// Velocité Maximum
	Nf32		m_a;			// Accéleration Maximum
	Nf32		m_j;			// Jerk Maximum
private:

	// TODO !!!! Ajouter un membre "m_txMin" permettant de gérer les chemins/trajectoires court(e)s et aussi de proposé un JERK calculé automatiquement: j=a/txmin pour le cas "normal" et ensuite si la longueur du chemin est plus petite que m_2ss alors on recalcul un jerk prenant en compte la longueur du chemin et txmin j = length/(2*txmin^3) ... à étudier !!!



	// Precalcul de certaines "constantes" étroitement liées au limites:
	
	union
	{
		Nf32		m_aooj;			// = a/j	 (UNION) = 'm_tx' 	Temps (durée) nécéssaire pour atteindre l'acceleration max ( = NLKINLIMITS.m_a ) 
		Nf32		m_tx;			// = a/j	 (UNION) = 'm_aooj' Temps (durée) nécéssaire pour atteindre l'acceleration max ( = NLKINLIMITS.m_a ) 
	};

	union
	{
		Nf32		m_a2oo2j;		// = a²/2j	 (UNION) = 'm_vx'		Vitesse atteinte quand a est maximum ( = NLKINLIMITS.m_a )
		Nf32		m_vx;			// = a²/2j	 (UNION) = 'm_a2oo2j'	Vitesse atteinte quand a est maximum ( = NLKINLIMITS.m_a )
	};
	
	union
	{
		Nf32		m_a2ooj;		// = a²/j	 (UNION) = 'm_2vx'		2 X la Vitesse atteinte quand a est maximum ( = NLKINLIMITS.m_a )
		Nf32		m_2vx;			// = a²/j	 (UNION) = 'm_a2ooj'	2 X la Vitesse atteinte quand a est maximum ( = NLKINLIMITS.m_a )
	};
	
	
	union
	{
		Nf32		m_a3oo6j2;		// = a^3/(6j²)	(UNION) = 'm_sx'		la distance parcourue ( (jt^3)/6 ) depuis a=0,v=0 quand a atteind son maximum. ( avec t = m_tx = a/j )
		Nf32		m_sx;			// = a^3/(6j²)	(UNION) = 'm_a3oo6j2'	la distance parcourue ( (jt^3)/6 ) depuis a=0,v=0 quand a atteind son maximum. ( avec t = m_tx = a/j )
	};
	

	union
	{
		Nf32		m_2a3ooj2;		// = 2a^3/j²	(UNION) = 'm_2ss'		|la distance parcourue depuis a=0,v=0 jusqu'à a=0 , v=0 en passant par v=vx( avec a=0 )
									//										|
									//										| 2 x ( portions L0+L1+L2 )
		Nf32		m_2ss;			// = 2a^3/j²	(UNION) = 'm_2a3ooj2'	| 
	};
	

	Nf32			m_2j;			// = 2j
	Nf32			m_a2;			// = a²
	Nf32			m_vmvx;			// = v - vx			la vitesse max (m_v)  moins la vitesse atteinte (m_vx) quand a atteind son maximum.

	// "Composantes" présentes dans le calcul du discriminant d'un Motion Profile de type C
	Nf32			m_2a2ooj;		// = 2a²/j		|
	Nf32			m_a4ooj2;		// = a^4/j²		|-- DISC = 2*( V0² + V1² ) - (2*a²)/j * (v0+v1) + a^4/j² + 4*a*d  
	Nf32			m_4a;			// = 4a			|						

	Nf32			m_a4;			// = a^4		|__ Calcul vitesse de croisiere: Vc = ( -a² + sqrt( a^4 + 4*j²*a*d) ) / (2*j)
	Nf32			m_4j2a;			// = 4j²a		|	


/*
	Valeurs abandonnées : 
	****************************************************************************************************************************************************************************
	union
	{
		Nf32		m_2aooj;		// = 2a/j	 (UNION) = 'm_2tx' 	 2 x Temps (durée) nécéssaire pour atteindre l'acceleration max ( = NLKINLIMITS.m_a ) 
		Nf32		m_2tx;			// = 2a/j	 (UNION) = 'm_2aooj' 2 x Temps (durée) nécéssaire pour atteindre l'acceleration max ( = NLKINLIMITS.m_a ) 
	};

	union
	{
		Nf32		m_a3oo2j2;		// = a^3/(2j²)	(UNION) = 'm_sxx'		| La portion de distance acquise avec une vitesse constante vx sur une durée tx
									//										| Attention, il ne s'agit pas de la distance parcourue, seulement de la partie générée par vx sur tx.
									//										| ( portion L1 )
		Nf32		m_sxx;			// = a^3/(2j²)	(UNION) = 'm_a2oo2j2'	| ( at²/2 - (jt^3)/6 = (jt^3)/2 - (jt^3)/6 = (jt^3)/3 )  ( avec t = m_tx = a/j ) ...
	};

	union
	{
		Nf32		m_a3oo3j2;		// = a^3/(3j²)	(UNION) = 'm_sxxx'		| La portion de distance acquise avec une vitesse constante vx sur une durée tx
									//										| Attention, il ne s'agit pas de la distance parcourue, seulement de la partie générée par vx sur tx.
									//										| ( portion L1 )
		Nf32		m_sxxx;			// = a^3/(3j²)	(UNION) = 'm_a3oo3j2'	| ( at²/2 - (jt^3)/6 = (jt^3)/2 - (jt^3)/6 = (jt^3)/3 )  ( avec t = m_tx = a/j ) ...
	};

	union
	{
		Nf32		m_a3ooj2;		// = a^3/j²	(UNION) = 'm_ss'			|la distance parcourue depuis a=0,v=0 quand v atteind la valeur de vx ( avec a=0 )
									//										|
									//										| ( portions L0+L1+L2 )
		Nf32		m_ss;			// = a^3/j²	(UNION) = 'm_a3ooj2'		|
	};

	Nf32			m_3a2ooj;		// = 3a²/j		|_	Racine 1 = ( -2*v0 - (3*a²)/j + sqrt( DISC ) )/(2a)
	Nf32			m_2a;			// = 2a			|
*/
};
