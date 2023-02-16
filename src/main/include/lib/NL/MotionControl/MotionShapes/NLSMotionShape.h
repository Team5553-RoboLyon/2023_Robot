#pragma once
#include "../../../N/NType.h"
#include "../../../N/NMemory.h"
#include "../../../N/NErrorHandling.h"
#include "../NLKinLimits.h"
#include "../NLKinTweak.h"

#include "../Trajectory/NLSovershoot.h"
//#include "../NLScut.h"
#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#define CONSTANT_NLSMOTIONSHAPE_DT_ACCURACY	0.001f		// pas de calcul pour tracage des courbes en secondes.
#endif

#define NLSMOTIONSHAPE_ID_BIT_SDOWN				BIT_0	// !!! DOIT ETRE EGAL A " NLSMOTIONPROFILE_ID_BIT_SDOWN	"
#define NLSMOTIONSHAPE_ID_BIT_SUP				BIT_2	// !!! DOIT ETRE EGAL A " NLSMOTIONPROFILE_ID_BIT_SUP "

typedef struct NLSMOTIONSHAPE	NNLSMOTIONSHAPE;
struct NLSMOTIONSHAPE
{
	enum STYPE
	{
		EMPTY		= 0,		
		SUP			= NLSMOTIONSHAPE_ID_BIT_SDOWN,	//0x1
		SDOWN		= NLSMOTIONSHAPE_ID_BIT_SUP		//0x4
	};

	Nf32 m_v0;		// vitesse initiale
	Nf32 m_v1;		// vitesse finale

	Nf32 m_s0;		// Abscisse initiale
	Nf32 m_ds;		// Distance totale couverte par la SShape. ( abscisse finale de la SShape S1 = m_s0 + m_ds )

	Nf32 m_jx;		// Jerk initial (et max).
	Nf32 m_ax;		// Acceleration Max.	
	Nf32 m_vx;		// Quantité de vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur maximum ( soit m_ax ).
	Nf32 m_tx;		// Durée nécéssaire pour que l'acceleration (initialement nulle) atteigne sa valeur maximum ( soit m_ax ).
	Nf32 m_sx;		// PORTION fixe  de la distance indépendante de v0 et v1 couverte pas la SShape sur une durée de m_tx
	
	Nf32 m_tb;		// Durée de maintient de l'acceleration maximum.

	inline void	null() { Nmem0(this, NLSMOTIONSHAPE); }

	STYPE forwardBuildUp(const NLVSTAGEX * pv0, const Nf32 v1);
	STYPE forwardBuildUp(const NLKINLIMITS *pkmax, const Nf32 s0, const Nf32 v0, const Nf32 v1);					// setup à utiliser si on sait que v0 < v1.		Up car on "monte" de v0 vers v1.
	STYPE backwardBuildDown(const NLKINLIMITS *pkmax, const Nf32 s1, const Nf32 v0, const Nf32 v1);				// setup à utiliser si on sait que v0 > v1.		Down car on "descend" de v0 vers v1.
	STYPE backwardBuildDown(const Nf32 v0, const NLVSTAGEX * pv1);
	Nu32 forwardOvershoot( NLSCUTOUTOFFSETS * pcutout, NLSTARTSTITCH *pimmediate_start, const NLVSTAGEX *pv0, NLVSTAGEX *pv1);
	Nu32 backwardOvershoot(NLSCUTINOFFSETS *pcutin, NLENDSTITCH *pimmediate_end, NLVSTAGEX * pv0, const NLVSTAGEX *pv1);

	Nu32 promote(NLKIN *pkbuff);
	Nu32 promote(NLKIN *pkbuff, const Nf32 cutin_dt);
	Nu32 promote(NLKIN *pkbuff, const NLSCUTOUTOFFSETS *pcut_out);
	Nu32 promote(NLKIN *pkbuff, const Nf32 cutin_dt, const NLSCUTOUTOFFSETS *pcut_out);


#ifdef _NEDITOR
	/*
	void drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 cutin_dt);
	void drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const NLSCUTOUTOFFSETS *pcut_out);
	*/
	void drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 cutin_dt, const NLSCUTOUTOFFSETS *pcut_out);

	void drawTangents(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack, const Nf32 pnt,const Nf32 axout);
#endif

};



