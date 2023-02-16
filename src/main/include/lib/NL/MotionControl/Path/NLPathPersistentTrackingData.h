#pragma once
#include "../../../N/NType.h"
#include "../../../N/Maths/NVec2f32.h"

#include "../../../N/Miscellaneous/NColor.h"
#include "./Geometry/NLPathGeometry.h"


#define DEFAULT_NLPATH_PERSISTENT_TRACKING_DATA_CLOTHOID_ERROR_THRESHOLD		0.01f		// Tolérance par défaut sur l'estimation de la position de l'image de la position recherchée sur la clothoide.
#define FLAG_NLPATH_PERSISTENT_TRACKING_DATA_BACKWARD							BIT_0		// Si ce flag est ON, alors cela signifie que le chemin est parcouru 'à l'envers'. Sinon, à l'endroit...
typedef struct NLPATH_PERSISTENT_TRACKING_DATA NLPATH_PERSISTENT_TRACKING_DATA;
struct NLPATH_PERSISTENT_TRACKING_DATA
{
	enum TRCK_RESULT
	{
		NOT_DETECTED = 0,			// Aucun point "le plus proche sur le chemin" détecté	
		ON_PATH_SEGMENT_START,		// Le point le + proche détecté correspond au début d'un segment ( i.e le point A d'un segment [AB] )
		ON_PATH_SEGMENT_END,		// Le point le + proche détecté correspond à la fin d'un segment ( i.e le point B d'un segment [AB] )	
		ON_PATH_SEGMENT,			// Le point le + proche détecté  est sur un segment mais n'est confondu ni avec le début ni avec la fin ( i.e un point I de ]AB[ )	
		ON_PATH_ARC_START,
		ON_PATH_ARC_END,
		ON_PATH_ARC,
		ON_PATH_ARC_BISSECTOR,
		ON_PATH_ARC_CENTER,
		ON_PATH_CLOTHOID_START,
		ON_PATH_CLOTHOID_END,
		ON_PATH_CLOTHOID
	};
	/*
	NLPATH_PERSISTENT_TRACKING_DATA();
	NLPATH_PERSISTENT_TRACKING_DATA(NLPATH* ppath, const Nbool bforward = NTRUE);
	*/
	void				reset();
	void				forwardSetup(const NLPATH_GEOMETRY* ppath_geometry, const Nf32 point_to_path_max_dist, const Nf32 cloth_err_threshold = DEFAULT_NLPATH_PERSISTENT_TRACKING_DATA_CLOTHOID_ERROR_THRESHOLD);
	void				backwardSetup(const NLPATH_GEOMETRY* ppath_geometry, const Nf32 point_to_path_max_dist, const Nf32 cloth_err_threshold = DEFAULT_NLPATH_PERSISTENT_TRACKING_DATA_CLOTHOID_ERROR_THRESHOLD);

	TRCK_RESULT			trackForward(NLPATH_POINT* pout, const NVEC2f32 *ppos);
	TRCK_RESULT			trackBackward(NLPATH_POINT* pout, const NVEC2f32 *ppos);


	Nu32				m_flags;
	NLPATH_GEOMETRY*	m_pPathGeometry;			// On conserve un pointeur sur la geometrie du Path tracké

	Nf32				m_pointToPathMaxDist;		// Distance maximum 'autorisée' entre le point recherché et sa projection la plus proche sur le chemin.
													// ( Si la projection du point sur le Path est trop éloignée du point, alors les fonction de recherche ne retiennent pas cette solution. )
	Nf32				m_clothoidErrorThreshold;	// Tolérance sur l'estimation de la position de l'image de la position recherchée sur la clothoide.
	NLPATH_POINT*		m_pTrackedKeyPoint;			// 'keypoint courant' (associé à la primitive courante: il s'agit du SECOND KEYPOINT )
	NLPATH_PRIMITIVE*	m_pTrackedPrimitive;		// 'Primitive courante' où la précédente "on Path" projection a été trouvée et donc celle avec laquelle on commence les recherches. 
	Nu32				m_trackedPrimitiveIndex;	// 'Index de la primitive courante dans NLPATH.m_primitivesArray
	

#ifdef _NEDITOR
	Nu32				m_iteration;				// Nombre total d'itérations de l'algo de recherche sur les clothoides ( = somme de toutes les itérations effectuées sur toutes les clothoides traitées )
#endif

};


