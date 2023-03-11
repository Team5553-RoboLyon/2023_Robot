#pragma once
#include "lib/N/NType.h"
#include "lib/N/NMemory.h"

// ------------------------------------------------------------------------------------------
/**
 *	@brief	offsets applicables en d�but de S.Shape pour promouvoir le premier KIN.
 *			Si ces offsets sont nulls, alors le premier Kin d'une S-shape correspond bien au dernier Kin de la phase 1.
 *			Sinon, les offsets placent ce dernier KIN avant la fin de la phase 1
 */
 // ------------------------------------------------------------------------------------------
typedef struct NLSCUTINOFFSETS NLSCUTINOFFSETS;
struct NLSCUTINOFFSETS
{
	Nf32			m_dt;		///< Offset de temps.		
	//Nf32			m_da;		///< Offset d'accel.		
	//Nf32			m_dv;		///< Offset de Vitesse.	
	Nf32			m_ds;		///< Offset d'Abscisse.	
	inline void null() { Nmem0(this, NLSCUTINOFFSETS); }
};

// ------------------------------------------------------------------------------------------
/**
 *	@brief	offsets applicables en fin de S.Shape pour promouvoir le dernier KIN.
 *			Si ces offsets sont nulls, alors le dernier Kin d'une S-shape correspond bien au dernier Kin de la phase 3. 
 *			Sinon, les offsets placent ce dernier KIN avant la fin de la phase 3
 */
 // ------------------------------------------------------------------------------------------
typedef struct NLSCUTOUTOFFSETS NLSCUTOUTOFFSETS;
struct NLSCUTOUTOFFSETS
{
	Nf32			m_dt;		///< Offset de temps.	( <= 0.0f )		
	Nf32			m_da;		///< Offset d'accel.	( >= 0.0f )	
	Nf32			m_dv;		///< Offset de Vitesse.	( <= 0.0f )	
	Nf32			m_ds;		///< Offset d'Abscisse.	( <= 0.0f )	
	inline void null() { Nmem0(this, NLSCUTOUTOFFSETS); }
};

// ------------------------------------------------------------------------------------------
/**
 *	@brief	offsets applicables au point NLVSTAGEX.m_s0 ou NLVSTAGEX.m_s1, lors de la construction d'une S.Shape. 
 *			Ces offsets permettent de cr�er des "stitchs" ( i-e points de sutures ) entre deux S-Shapes.
 */
 // ------------------------------------------------------------------------------------------
typedef struct NLSANCHOROFFSETS NLSANCHOROFFSETS;
struct NLSANCHOROFFSETS
{
	Nf32			m_dv;		// Offset de Vitesse.	applicable au point s0 ou s1, lors de la construction d'une S.Shape ( s0 en forwardbuild, s1 en backwardbuild )
	Nf32			m_ds;		// Offset d'Abscisse.	applicable au point s0 ou s1, lors de la construction d'une S.Shape ( s0 en forwardbuild, s1 en backwardbuild )
	inline void null() { Nmem0(this, NLSANCHOROFFSETS); }
};


// ------------------------------------------------------------------------------------------
/**
 *	@brief	R�sultats des calculs effectu�s par les fonctions NLSMOTIONSHAPE::forwardOvershoot et NLSMOTIONSHAPE::backwardOvershoot.
 *
 */
// ------------------------------------------------------------------------------------------
/*
typedef struct NLSOVERSHOOT NLSOVERSHOOT;
struct NLSOVERSHOOT							//		forwardOvershoot							|				backwardOvershoot				
{											//	------------------------------------------------+--------------------------------------------------
	NLSCUTOUTOFFSETS	m_cutOut;			//	� appliquer � la S-shape en cours				|		� appliquer � la S-Shape soeur "de Gauche"
	NLSANCHOROFFSETS	m_sOffsets;			//	� apppliquer � la S-Shape soeur "de Droite". 	|		� appliquer � la S-Shape soeur "de Gauche"
	Nf32				m_t0Offset;			//  � apppliquer � la S-Shape soeur "de Droite".	|		� appliquer � la S-shape en cours
	inline void null() { Nmem0(this, NLSOVERSHOOT); }
};
*/


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Donn�es d'Overshoot stock�es dans un VStage de type "From" et donc utilis�es par les S.Shapes situ�es � droite
 *			de la S.shape Overshoot�e � partir de laquelle elles* ont �t� calcul�es (*ces donn�es ) par la fonction forwardOvershoot.
 */
// ------------------------------------------------------------------------------------------
typedef struct NLSTARTSTITCH NLSTARTSTITCH;
struct NLSTARTSTITCH
{
	NLSANCHOROFFSETS	m_s1Offsets;
	NLSCUTINOFFSETS		m_cutIn;
	//Nf32				m_t0Offset;
	inline void null() { Nmem0(this, NLSTARTSTITCH); }
};

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Donn�es d'Overshoot stock�es dans un VStage de type "To" et donc utilis�es par les S.Shapes situ�es � gauche
 *			de la S.shape Overshoot�e � partir de laquelle elles* ont �t� calcul�es (*ces donn�es ) par la fonction backwardOvershoot.
 */
 // ------------------------------------------------------------------------------------------
typedef struct NLENDSTITCH NLENDSTITCH;
struct NLENDSTITCH
{
	NLSANCHOROFFSETS	m_s0Offsets;
	NLSCUTOUTOFFSETS	m_cutOut;
	inline void null() { Nmem0(this, NLENDSTITCH); }
};

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Format de stockage des donn�es d'Overshoot dans un NLVSTAGEX
 *			Les fonctions utilis�es par l'algorithme de construction de trajectoire "savent" comment interpr�ter et donc choisir
 *			entre le format NLSTARTSTITCH et NLENDSTITCH
 */			
 // ------------------------------------------------------------------------------------------
typedef union NLSSTITCH NLSSTITCH;
union NLSSTITCH
{
	NLSTARTSTITCH	m_start;		// formatage des donn�es "pour repartir" en cas de forward overshoot 
	NLENDSTITCH		m_end;		// formatage des donn�es "pour repartir en arriere" en cas de backward overshoot  
	inline void null() { Nmem0(this, NLSSTITCH); }
};

// Rappels:
		//		D d�but du premier intervalle, soit le d�but de la S_SHAPE
		//		P d�but du troisi�me intervalle
		//		O point d'Overshoot approch� par dichotomie
		//		F fin de la S_SHAPE et donc du troisi�me intervalle
		//		et	
		//		G le point de redemarrage "fant�me".
		//
		//		Chacun de ces 5 points est d�fini par un quadruplet (t,a,v,s)

//	OVERSHOOT	|	Forward Overshoot												|		Backward Overshoot										|
//				|		( CutOut )													|			( CutIn )											|
//				|																	|																|
//	m_dt		|	Temps		de F vers O				[0 <=]						|		Temps		de D vers O				[>= 0]				|		
//	m_dv		|	Vitesse		de F vers O				[signe oppos� � m_jx]		|		Vitesse		de D vers O				[signe de m_jx]		|
//	m_ds		|	Distance	de F vers O				[0 <=]						|		Distance	de D vers O				[>= 0]				|	
//				|																	|																|		
//	m_dsx		|	Distance	de Vstage.m_s1 vers G	[0 <=]						|		Distance	de Vstage.m_s0 vers G	[>= 0]				|	
//				|																	|																|
//				|	On identifie facilement un Forward Overshoot:					|	On identifie facilement un Backward Overshoot:				|
//				|				m_dt et m_ds et m_dsx sont tous les 3 NEGATIFS.		|				m_dt et m_ds et m_dsx sont tous les 3 POSITIFS.	|
//				|																	|																|
