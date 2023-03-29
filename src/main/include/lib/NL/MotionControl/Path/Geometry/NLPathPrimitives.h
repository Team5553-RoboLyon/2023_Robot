#pragma once
#include "../../../../N/NType.h"
#include "../../../../N/Maths/NVec2f32.h"
#include "../../../../N/Maths/NMatrix.h"

#include "../../../../N/Containers/NArray.h"
#include "NLPathPoint.h"
/**
 * \def	NLPATH_PRIMITIVE_ID_xxx
 * \brief	
 *
 * Il y a 3 types de primitives \b Path differentes.
 * chaque type possede son propre identifiant accessible 
 * 
 * 
 * 
*/

#define NLPATH_PRIMITIVE_ID_NULL		0				/**<  ID de primitive indefinie. Une primitive ne devrait jamais avoir cet ID.*/
#define NLPATH_PRIMITIVE_ID_SEGMENT		1				/**<  ID de primitive Segment.*/
#define NLPATH_PRIMITIVE_ID_ARC			2				/**<  ID de primitive Arc.*/
#define NLPATH_PRIMITIVE_ID_CLOTHOID	3				/**<  ID de primitive Clothoide.*/

#define FLAG_NLPATH_CLOTHOID_NXTLINK			BIT_0	/**<Une primitive "clothoide" flaggee de la sorte est consideree comme formant une "paire" avec la primitive suivante,
														*	qui est alors obligatoirement une clothoide. ! Cela arrive quand un Arc de cercle de base est remplace par
														*	une paire de clothoides sans insertion d'arc de cercle entre les deux clothoides.
														*	Il est important de remarquer que deux clothoides peuvent se suivre sans forcement former une paire ( remplacement de 2 
														*	arcs de cercles successifs par deux paires de clothoides, la premiere clothoide de la deuxieme paire "suit" bien la deuxieme
														*	clothoide de la premiere paire, sans pour autant former une paire avec elle.) 
														*/

#define FLAG_NLPATH_CLOTHOID_PRVLINK			BIT_1	// Une primitive "clothoide" flaggee de la sorte est consideree comme formant une "paire" avec la primitive precedente ...
														// ( voir plus haut pour l'explication )
#define FLAG_NLPATH_CLOTHOID_SECOND				BIT_2	// Une primitive "clothoide" flaggee de la sorte est consideree comme etant "la deuxieme". Une clothoide peut-être la "deuxieme" sans former une paire avec la premiere.
														// ... si il y a un cercle insere.

#define FLAG_NLPATH_PRIMITIVE_RAW				BIT_3	// Une primitive "Arc" flaggee de la sorte est consideree comme 'BRUT'(RAW), c'est a dire qu'il s'agit de l'arc de cercle 'build' non remplace par une paire de clothoides. 
														// Cela signifie que le coefficient ICRA ou ICRC est a ZERO. 
														// Une primitive "Segment" flaggee de la sorte est consideree comme 'BRUT'(RAW), c'est a dire qu'il relie directement 2 waypoints sans aucune construction de section.
														// Il s'agit de la plus simple representation d'une section: le segment direct.
														// Cela signifie que le coefficient BCR de la section est a ZERO. ( du coup aucune paires de cercles n'est construite ... )

#define FLAG_NLPATH_ARC_INDIRECT				BIT_4	// Une primitive "Arc" flaggee de la sorte est parcourue dans le sens indirect. Par defaut,un Arc est parcouru dans le sens direct et ce flag a OFF.
														// Le sens de parcours de l'Arc decoule bien evidement du sens de parcours du cercle de construction dont il est issu.
														// NLPATH_SECTION_BCIRCLE.sgn =  1 <---> FLAG_NLPATH_ARC_INDIRECT est OFF ... le sens est "Direct"
														// NLPATH_SECTION_BCIRCLE.sgn = -1 <---> FLAG_NLPATH_ARC_INDIRECT est ON  ... le sens est "Indirect"



//#define FLAG_NLPATH_PRIMITIVE_				BIT_5	// dispo.
//#define FLAG_NLPATH_PRIMITIVE_				BIT_6	// dispo.
//#define FLAG_NLPATH_PRIMITIVE_				BIT_7	// dispo.


/**
 * \struct	NLPATH_PRIMITIVE_CORE
 * \brief	Coeur commun a toute les primitives.
 *
 * Les primitives de \b Path sont au nombre de 3.
 * NLPATH_SEGMENT
 * NLPATH_ARC
 * NLPATH_CLOTHOID
 * Le premier membre de chacune de ces primtives est NLPATH_PRIMITIVE_CORE
*/
typedef struct
{
	Nu8			m_id;						// id de la primitive ( = type )
	Nu8			m_flags;			
	Nu16		m_keyPoint0;				// Index du premier keypoint associe a cette primitive dand l'array des keypoints. Le deuxieme keypoint associe ayant pour index [m_keyPoint0+1]
	Nf32		m_l;						// 'Longueur' de la primitive
}NLPATH_PRIMITIVE_CORE;

typedef struct
{
	NLPATH_PRIMITIVE_CORE	m_core;
}NLPATH_SEGMENT;

typedef struct
{
	NLPATH_PRIMITIVE_CORE	m_core;
	Nf32					m_phi;			// Angle de demarrage de l'arc de cercle
	Nf32					m_omega;		// Longueur angulaire de l'arc de cercle
	Nf32					m_radius;
	NVEC2f32				m_center;
	NVEC2f32				m_i;			// vecteur i unitaire formant avec "m_j" une base orthonormee utilisee pour definir les coordonnees des points de l'arc
	NVEC2f32				m_j;			// vecteur j unitaire formant avec "m_i" une base orthonormee utilisee pour definir les coordonnees des points de l'arc
}NLPATH_ARC;

typedef struct
{
	NLPATH_PRIMITIVE_CORE	m_core;
	Nf32					m_km;			// courbure de la clothoide a son extremite ( fin, l'autre extremite a toujours une courbure nulle )
	Nf32					m_sigma;		// parametre de clothoide ... -- Ref. [2.4] page 61. [K(s) = m_sigma*s] ... la courbure k en fonction de l'abscisse curviligne s.
	Nf32					m_param;		// parametre de clothoide ... -- Ref. [1.6] page 42 precalcule permettant avec "sm" de calculer les coordonnees de tous les points de la clothoide.
	NVEC2f32				m_i;			// vecteur i unitaire formant avec "m_j" une base orthonormee utilisee pour definir les coordonnees des points de la clothoide
	NVEC2f32				m_j;			// vecteur j unitaire formant avec "m_i" une base orthonormee utilisee pour definir les coordonnees des points de la clothoide
	//NLPPATH_CLOTHOID_DATA	*m_pData;		
}NLPATH_CLOTHOID;

typedef union
{
	NLPATH_PRIMITIVE_CORE	m_core;
	NLPATH_SEGMENT			m_segment;
	NLPATH_ARC				m_arc;
	NLPATH_CLOTHOID			m_clothoid;
}NLPATH_PRIMITIVE;


#define NLclearNLPathPrimitiveInArrayCallBack		NULL
#define NLclearNLPathPrimitivePtrInArrayCallBack	NULL
//inline void NLclearNLPathPrimitiveInArrayCallBack(void* p) { ((NLPATH_PRIMITIVE*)p)->...; };
//inline void NLclearNLPathPrimitivePtrInArrayCallBack(void* p) { (*(NLPATH_PRIMITIVE**)p)->...; };

inline NLPATH_PRIMITIVE* setupPathPrimitiveSegment(NLPATH_PRIMITIVE* p, const Nu8 flags, const Nu16 kp0idx, const Nf32 l) 
{
	p->m_segment.m_core.m_id		= NLPATH_PRIMITIVE_ID_SEGMENT;
	p->m_segment.m_core.m_flags		= flags;
	p->m_segment.m_core.m_keyPoint0 = kp0idx;
	p->m_segment.m_core.m_l			= l;
	return p;
}

inline NLPATH_PRIMITIVE* setupPathPrimitiveArc(NLPATH_PRIMITIVE* p, const Nu8 flags, const Nu16 kp0idx, const Nf32 l, const Nf32 phi, const Nf32 omega, const Nf32 r, const NVEC2f32* pc, const NVEC2f32* pi, const NVEC2f32*pj)
{
	p->m_arc.m_core.m_id			= NLPATH_PRIMITIVE_ID_ARC;
	p->m_arc.m_core.m_flags			= flags;
	p->m_arc.m_core.m_keyPoint0		= kp0idx;
	p->m_arc.m_core.m_l				= l;
	p->m_arc.m_phi					= phi;
	p->m_arc.m_omega				= omega;
	p->m_arc.m_radius				= r;
	p->m_arc.m_center				= *pc;
	p->m_arc.m_i					= *pi;
	p->m_arc.m_j					= *pj;
	return p;
}
inline NLPATH_PRIMITIVE* setupPathPrimitiveClothoid(NLPATH_PRIMITIVE* p, const Nu8 flags, const Nu16 kp0idx, const Nf32 l,const Nf32 km, const Nf32 sigma, const Nf32 param, const NVEC2f32* pi, const NVEC2f32* pj)
{
	p->m_clothoid.m_core.m_id			= NLPATH_PRIMITIVE_ID_SEGMENT;
	p->m_clothoid.m_core.m_flags		= flags;
	p->m_clothoid.m_core.m_keyPoint0	= kp0idx;
	p->m_clothoid.m_core.m_l			= l;
	p->m_clothoid.m_km					= km;
	p->m_clothoid.m_sigma				= sigma;
	p->m_clothoid.m_param				= param;
	p->m_clothoid.m_i					= *pi;
	p->m_clothoid.m_j					= *pj;
	return p;
}

inline void NL_ExtractArcHandle(NVEC2f32 *pv2, const NMATRIX *pmx, const NLPATH_PRIMITIVE *pprim)
{
	NErrorIf(pprim->m_core.m_id != NLPATH_PRIMITIVE_ID_ARC, NERROR_UNAUTHORIZED_REQUEST); // cette fonction attend une primitive "arc de cercle" uniquement !!!

	Nf32 ang = pprim->m_arc.m_omega / 2.0f + pprim->m_arc.m_phi;
	Nf32 x = cos(ang)*pprim->m_arc.m_radius;
	Nf32 y = sin(ang)*pprim->m_arc.m_radius;

	NVEC2f32 local;
	local.x = pprim->m_arc.m_center.x + pprim->m_arc.m_i.x * x + pprim->m_arc.m_j.x * y;
	local.y = pprim->m_arc.m_center.y + pprim->m_arc.m_i.y * x + pprim->m_arc.m_j.y * y;

	NVEC3f32 v0;
	NMulVector2ByMatrixO(&v0, pmx, &local);
	pv2->x = v0.x;
	pv2->y = v0.y;
}
//Nu32 NL_GetClosestPointOnClothoid(NLPATH_POINT *presult, const NVEC2 *ppos, const NLPATH_POINT *pkp0, const NLPATH_POINT *pkp1, const NLPATH_CLOTHOID *pcl, const Nf32 error_threshold);

// A propos de la courbure K des primitives et des keypoints:
//		La courbure K est stockee dans les keypoints.
//		
//		pour une PRIMITIVE SEGMENT: k est constante et nulle ! Elle est stockee dans le keypoint 1 du segment.
//					PRIMITIVE SEGMENT->kp0.k	= courbure de la primitive precedente.
//					PRIMITIVE SEGMENT->kp1.k	= courbure de cette primitive segment, ( vaut 0 )
//
//		pour une PRIMITIVE ARC: k est constante ! Elle est stockee dans le keypoint 1 de l'Arc.
//					PRIMITIVE ARC->kp0.k	= courbure de la primitive precedente.
//					PRIMITIVE ARC->kp1.k	= courbure de cette primitive Arc
//
//		pour une PRIMITIVE CLOTHOIDE: k varie de k0 a k1 !
//					PRIMITIVE CLOTHOIDE->kp0.k		= courbure en debut de clothoide ( = courbure de la primitive precedente ).
//					PRIMITIVE ARC->kp1.k			= courbure en fin de clothoide	 ( = courbure de la primitive suivante ).
