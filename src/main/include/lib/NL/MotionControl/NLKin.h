// 
//	'KIN' for 'KINEMATICS'
//
#pragma once
#include "../../N/NMath.h"
#include "../../N/NType.h"
#include "../../N/NFlags.h"
#include "../../N/NMemory.h"

#include "../NL2DOrthogonalCoordinateSystem.h"
#include "NLKinLimits.h"

// EPSILON(s)
// Les calculs en Nf32 engendre des erreurs d'arrondi qui rendent les tests d'égalité "délicats".
// On considérera que deux valeurs sont distinctes si leur différence est supérieure à un certain seuil ( EPSILON )
// Dans le cas contraire ( différence inférieure ou égale au seuil ) on considèrera les deux valeurs comme égales.
// Chaque grandeur possède son propre seuil (EPSILON)
#define NLKIN_EPSILON_S			1e-5f		///< 10E-5 m		1 centième de mm soit 10 micromètres
#define NLKIN_EPSILON_V			1e-5f		///< 10E-5 m/s		1 centième de mm/s
#define NLKIN_EPSILON_A			1e-5f		///< 10E-5 m/s/s	1 centième de mm/s/s
#define NLKIN_EPSILON_J			1e-5f		///< 10E-5 ms/s/s	1 centième de mm/s/s/s
#define NLKIN_EPSILON_T			1e-4f		///< 10E-4 s		1 dix millième de seconde 

// MACROS à utiliser pour tester l'égalité de deux valeurs: 
//							"if( !NLKIN_?_DISTINCT(valeur0,valeur1) )" à utiliser à la place de "if( valeur0 == valeur1 )"
#define NLKIN_S_DISTINCT(s0,s1)	( NABS((s0)-(s1)) > NLKIN_EPSILON_S )		
#define NLKIN_V_DISTINCT(v0,v1)	( NABS((v0)-(v1)) > NLKIN_EPSILON_V )		
#define NLKIN_A_DISTINCT(a0,a1)	( NABS((a0)-(a1)) > NLKIN_EPSILON_A )
#define NLKIN_J_DISTINCT(j0,j1)	( NABS((j0)-(j1)) > NLKIN_EPSILON_J )
#define NLKIN_T_DISTINCT(t0,t1)	( NABS((t0)-(t1)) > NLKIN_EPSILON_T )

//  Durée minimum d'intégration d'un KIN. ( à ne pas confondre avec le seuil NLKIN_EPSILON_T ! ) 
#define	NLKIN_MIN_T				0.002f	///< s.durée minimum d'un bloc kinematic

// MACROS à utiliser pour calculer/intégrer manuellement et séparement les composantes d'un NLKIN : 
#define NLKIN_S(s0,v0,a0,j0,dt,dt2)	( (s0) + (v0)*(dt) + ((a0)*(dt2))/2.0f + ((j0)*(dt2)*(dt))/6.0f )	///< S, l'abscisse s0 en t0	+ la distance parcourue  par integration des composantes j0,a0,v0 pendant un laps de temps dt 
#define NLKIN_V(v0,a0,j0,dt,dt2)	( (v0) + (a0)*(dt) + ((j0)*(dt2))/2.0f )							///< V, la vitesse v0 en t0	+ la vitesse additionnelle obtenue par integration des composantes j0,a0 pendant un laps de temps dt 
#define NLKIN_A(a0,j0,dt)			( (a0) + (j0)*(dt) )												///< A, l'accel. a0 en t0	+ l'accel. additionnelle obtenue par integration de la composante j0 pendant un laps de temps dt 

#define NLKIN_DS(v0,a0,j0,dt,dt2)		   ( (v0)*(dt) + ((a0)*(dt2))/2.0f + ((j0)*(dt2)*(dt))/6.0f )	///< DS la distance parcourue  par integration des composantes j0,a0,v0 pendant un laps de temps dt 
#define NLKIN_DV(a0,j0,dt,dt2)			   ( (a0)*(dt) + ((j0)*(dt2))/2.0f )							///< DV la vitesse additionnelle obtenue par integration des composantes j0,a0 pendant un laps de temps dt 
#define NLKIN_DA(j0,dt)					   ( (j0)*(dt) )												///< DA l'accel. additionnelle obtenue par integration de la composante j0 pendant un laps de temps dt 

	// Membres
enum KIN_PARAM_ID
{
	ABSCISSE = 0,		///< index of \e abscisse member in "m_param[]" Kin static table
	VELOCITY,			///< index of \e velocity member in "m_param[]" Kin static table
	ACCELERATION,		///< index of \e acceleration member in "m_param[]" Kin static table
	JERK,				///< index of \e jerk member in "m_param[]" Kin static table
	TIME				///< index of \e time stamp member in "m_param[]" Kin static table
};

class NLKIN3
{
public:
	union
	{
		struct
		{
			Nf32	m_s;	 ///< abscisse curviligne.
			Nf32	m_v;	 ///< Velocité.
			Nf32	m_a;	 ///< Accélération.
		};

		Nf32 m_param[3];	 ///< an another way to access kin values with \b KIN_PARAM_ID @see KIN_PARAM_ID			
	};
};

class NLKIN
{
public:
	/** Liste des differentes valeurs retournées  par les differentes fonctions Check ( AccVelFastCheck, AccVelCheck, ... ).
	 *	Ces valeurs sont potentiellement combinees entre elles avec l'operateur |.
	 *	Par exemple, ACCELERATION_OVERSHOOT|VELOCITY_WAS_OVERSHOOTED.
	 */
	enum CHECKS
	{
		VELOCITY_ACCELERATION_OK	= 0,		 ///< Vitesse  et Acceleration sont ok.
		VELOCITY_WAS_OVERSHOOTED	= BIT_0,	 ///< La Vitesse "était" en dépassement.
		VELOCITY_OVERSHOOT			= BIT_1,	 ///< La Vitesse  est en dépassement.
		VELOCITY_WILL_OVERSHOOT		= BIT_2,	 ///< La Vitesse  sera en dépassement.
		ACCELERATION_OVERSHOOT		= BIT_3		 ///< L'acceleration est en dépassement.
	};

	inline		NLKIN() { Nmem0(this, NLKIN);}
	inline		NLKIN(const Nf32 s, const Nf32 v, const Nf32 a, const Nf32 j, const Nf32 t){m_s = s;m_v = v;m_a = a;m_j = j;m_t = t;}
	inline void	null() { Nmem0(this, NLKIN); }
	inline void	tjavs_set(const Nf32 t, const Nf32 j, const Nf32 a, const Nf32 v, const Nf32 s) { m_t = t; m_a = a; m_v = v; m_s = s; m_j = j; }
	inline void setAsKinX(const Nf32 j, const Nf32 a) { m_j = j; m_a = a; m_t = a / j; m_v = (a*a) / (2 * j); m_s = m_j*NPOW3(m_t) / 6.0f; }
	inline void reverse() {	m_s = -m_s; m_v = -m_v; m_a = -m_a; m_j = -m_j;	}
	void		atS(const NLKIN* pk0, const NLKIN* pk1, const Nf32 s );
	void		atT(const NLKIN* pk0, const NLKIN* pk1, const Nf32 t);
	void		from(const NLKIN *pk0, const Nf32 dt);
	void		from(const NLKIN *pk0, const Nf32 jerk, const Nf32 dt);
	NLKIN*		to(NLKIN *pk1, const Nf32 dt);
	NLKIN*		to(NLKIN *pk1, const Nf32 jerk, const Nf32 dt);
	
	const Nu32	accVelfastCheck(const NLKINLIMITS * pkmax);
	const Nu32	accVelCheck(const NLKINLIMITS * pkmax);
	const Nu32	accVelFastFullCheck(const NLKINLIMITS *pkmax);
	const Nu32	accVelFullCheck(const NLKINLIMITS *pkmax);
	const Nu32	velForwardCheck(const NLKINLIMITS *pkmax);
	const Nu32	velBackwardCheck(const NLKINLIMITS *pkmax);

#ifdef _NEDITOR
void draw(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack, const NLKIN* pk0);
void verticalDraw(NL2DOCS* p2docs, const NVEC2* po, const NCOLORPICKPACK pickpack, const NLKIN* pk0);
Nchar* print(Nchar* pstr);
#endif


	union
	{
		struct
		{
			Nf32	m_s;	 ///< abscisse curviligne, a la date m_t, mesuree depuis le debut du chemin.( m_t = 0, m_s = 0 ).
			Nf32	m_v;	 ///< Velocite a la date m_t.
			Nf32	m_a;	 ///< Acceleration a la date m_t.
			Nf32	m_j;	 ///< Jerk ( qu'il a fallu appliquer depuis le  NLKIN precedent pour obtenir ce KIN.) this = Kin0 "+"  this.Jerk  ( ici le "+" ne représente pas l'addition ).
			Nf32	m_t;	 ///< temps/date, mesuree depuis le temps t0=0 ( debut du chemin). La duree ecoulee entre deux KIN vaut dt = (kin[n].m_t - kin[n-1].m_t).
		};

		Nf32 m_param[5];	 ///< an another way to access kin values with \b KIN_PARAM_ID @see KIN_PARAM_ID			
	};
};

#define NLclearKinInArrayCallBack			NULL	
//inline void NLclearKinInArrayCallBack(void* p){}