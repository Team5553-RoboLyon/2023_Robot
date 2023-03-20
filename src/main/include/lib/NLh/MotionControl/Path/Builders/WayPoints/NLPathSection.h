#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Containers/NArray.h"

// #include "lib/NL/MotionControl/Drivetrain/NLDriveTrainSpecs.h"
#include "lib/NL/MotionControl/Path/NLPath.h"

#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathWayPoint.h"
#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathHeuristics.h"
#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathSectionBuildCircle.h"
#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathSectionBuildPair.h"

// ***************************************************************************************
// **					Path Section UID												**
// ***************************************************************************************
//
//	Section UID is a packed 8 bits value with all the associated primitives number
//	packed inside.
//
//	Les primitives issues d'une section sont toujours construites à partir de 3 parties:
//					Le premier arc de cercle de construction
//					Le segment ( tangent ) connectant les deux arcs de cercles de construction.
//					Le deuxième arc de cercle de construction
//
//		- IS COMPUTED					1 Bit	true si la section ( et l'UID) est valide
//		- SECTION PART A				3 Bits  Nombre de primitives ( 0 à 6) composant la premiere partie de la section ( primitives issue du premier arc de cercle de construction )
//		- SECTION PART B				1 Bit	Nombre de primitives ( 0 ou 1) composant la seconde partie de la section ( soit un segment soit "rien", c'est à dire un segment de longueur nulle )
//		- SECTION PART C				3 Bits	Nombre de primitives ( 0 à 6) composant la troisieme partie de la section ( primitives issue du second arc de cercle de construction )
//
// |____|______________|____|______________|
// | 07 | 06 | 05 | 04 | 03 | 02 | 01 | 00 |
// | 	|			   |	|			   |
// | V	|			   | P	|			   |
// | A	|	SECTION	   | A	|	SECTION	   |
// | L	|	PART  A	   | R	|	PART  C	   |
// | I	|			   | T 	|			   |
// | D	|			   | 	|			   |
// | 	|			   | B	|			   |
// |____|______________|____|______________|
// |	|			   |	|			   |
// |____|______________|____|______________|
typedef struct NLPATH_SECTION NLPATH_SECTION;
struct NLPATH_SECTION
{
	friend class NLPATH_BUILDER_WAYPOINTS;

#define FLAG_NLPATH_SECTION_IS_ALIGNED BIT_0
#define NLPATH_SECTION_WAYPOINTS_ALIGNMENT_THRESOLD 0.9999984769f // = cos(0.1 deg). More accurate value of cos(0.1 deg) is 0.99999847691328769880290124792571
#define NLPATH_SECTION_BETA_THRESHOLD (NF32_PI * 179.9f / 180.0f) // = 0.1 deg en radian

// MAKE UID MACROS
#define MAKE_NLPATH_SECTION_UID(valid, A, B, C) (Nu8)((((valid)&0x1) << 7) | (((A)&0x7) << 4) | (((B)&0x1) << 3) | ((C)&0x7))
#define RESET_NLPATH_SECTION_UID(uid) ((uid) = 0)
#define SET_NLPATH_SECTION_VALIDITY(uid, valid) ((((uid)&0x7F)) | (((valid)&0x1) << 7))
#define SET_NLPATH_SECTION_PRIMITIVES_NB_A(uid, A) ((((uid)&0x8F)) | (((A)&0x7) << 4))
#define SET_NLPATH_SECTION_PRIMITIVES_NB_B(uid, B) ((((uid)&0xF7)) | (((B)&0x1) << 3))
#define SET_NLPATH_SECTION_PRIMITIVES_NB_C(uid, C) ((((uid)&0xF8)) | (((C)&0x7)))

#define GET_NLPATH_SECTION_VALIDITY(uid) (((uid)&0x80) >> 7)
#define GET_NLPATH_SECTION_PRIMITIVES_NB_A(uid) (((uid)&0x70) >> 4)
#define GET_NLPATH_SECTION_PRIMITIVES_NB_B(uid) (((uid)&0x8) >> 3)
#define GET_NLPATH_SECTION_PRIMITIVES_NB_C(uid) ((uid)&0x7)
#define GET_NLPATH_SECTION_PRIMITIVES_NB(uid) (GET_NLPATH_SECTION_PRIMITIVES_NB_A(uid) + GET_NLPATH_SECTION_PRIMITIVES_NB_B(uid) + GET_NLPATH_SECTION_PRIMITIVES_NB_C(uid))

	Nu8 m_flags; // 8 flags
	Nu8 m_uid;	 //
	Nu16 m_availableNu16;

	NLPATH_SECTION_BCIRCLE m_circle[4];
	NLPATH_SECTION_BPAIR m_pair[4];
	NLPATH_SECTION_BPAIR *m_pBestPair;

	Nu32 computeAndExtract(NLPATH_WAYPOINT *pwp0, NLPATH_WAYPOINT *pwp1, /*const NLDRIVETRAINSPECS *pdts, */ NLPATH_HEURISTIC heuristic, NLPATH_GEOMETRY *pgeom, const Nf32 arc_length_threshold);

private:
	void drawBuildingCircles(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR *pcol);
	void drawBuildingPairs(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR *pcolcircles, const NCOLOR *pcolpair, const NCOLOR *pcolalpha, const NCOLOR *pcolbeta, const Nu32 what);
	// inline void drawBestBuildingPair(const NLPATH_WAYPOINT * pwp0, const NLPATH_WAYPOINT * pwp1, const NCOLOR * pcol) { m_pBestPair->draw(pwp0, pwp1, pcol); }
};
