#pragma once

#include "../../../N/NType.h"
#include "../../../N/Maths/NVec2f32.h"
#include "../NLKIN.h"


// FULL STATE : 

typedef struct NLTRAJECTORY_POINT NLTRAJECTORY_POINT;
struct NLTRAJECTORY_POINT
{
	void set(NLKIN *pkin, const Nf32 curvature, NVEC2f32 *ppos, NVEC2f32 *ptgt) { m_k = curvature; m_kin = *pkin; m_p = *ppos; m_u = *ptgt; }
	inline void	null() { Nmem0(this, NLTRAJECTORY_POINT); }

	NVEC2f32	m_p;		// position (x,y) sur la trajectoire.
	Nf32		m_k;		// courbure � l'abscisse curviligne m_kin.m_s
	NVEC2f32	m_u;		// tangente � la trajectoire en cette position ( pour l'instant n'est pas garanti unitaire � cause de l'appel � la fonction NLTRAJECTORY_PACK::getState )
	NLKIN		m_kin;		// kin repr�sentant le robot ( son centre d'inertie )
};
/* Trick:
 *	Il est possible de caster un NLTRAJECTORY_POINT en NLPATH_POINT !
 * 
 *	NLPATH_POINT	p			NLTRAJECTORY_POINT	p				|
 *					k								k				|_
 *					u								u				|
 *					s								NLKIN	m_s		|
 *															m_v
 *															m_a
 *															m_j
 *															m_t
 */

/*

typedef struct NLTRAJECTORY_POINT_XTD NLTRAJECTORY_POINT_XTD;
struct NLTRAJECTORY_POINT_XTD
{
	void set(NLKIN* pkin, const Nf32 curvature, NVEC2f32* ppos, NVEC2f32* ptgt) { m_localCurvature = curvature; m_kin = *pkin; m_position = *ppos; m_tangent = *ptgt; }
	inline void	null() { Nmem0(this, NLTRAJECTORY_POINT); }

	NLPATH_POINT		m_Point;			// Point de chemin.
	NLKIN				m_kin;				// kin repr�sentant le robot ( son centre d'inertie )
};
*/