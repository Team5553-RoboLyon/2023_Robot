#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Core/NVersion.h"

#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathHeuristics.h"
#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathWayPoint.h"
#include "lib/NL/MotionControl/Path/Builders/NLPathBuilder.h"

#ifdef _NEDITOR
#include "lib/NL/NL2DOrthogonalCoordinateSystem.h"
#endif

#define DEFAULT_NLPATH_WAYPOINTS_ARRAYCAPACITY 4

#define DEFAULT_NLPATH_WAYPOINTS_ARC_LENGTH_THRESHOLD 0.25f // metres ( = 25 cm )

#define CONSTANT_NL_WAYPOINT_TANGENT_ARROWHEAD_SIZE_RATIO 0.1f	 // la tete de fleche de la tangente
#define CONSTANT_NL_WAYPOINT_TANGENT_ARROWTAIL_SIZE_RATIO 0.075f // à la naissance de la fleche, longueur du 'petit espace'

#define DEFAULT_NL_WAYPOINT_XEXTEND 0.05f
#define DEFAULT_NL_WAYPOINT_YEXTEND 0.05f
#define DEFAULT_NL_WAYPOINT_TGT_SIZE 2.0f

#define DEFAULT_NL_WAYPOINT_P_COLOR NCOLOR_PRESET3F_BLUE_AZURE, 1	 // NCOLOR_PRESET3F_BLUE_ELECTRIC,1
#define DEFAULT_NL_WAYPOINT_U_COLOR NCOLOR_PRESET3F_BLUE_CERULEAN, 1 // NCOLOR_PRESET3F_BLUE_AQUAMARINE,1
#define DEFAULT_NL_HANDLE_COLOR NCOLOR_PRESET3F_BLUE_TEAL, 1		 // NCOLOR_PRESET3F_GREEN_HARLEQUIN,1

#ifdef _NEDITOR
// FLAGS Spécifiques à ce builder:
//																		.
//																		.
//																		.
// #define FLAG_NLPATH_EDITING_DRAW_PATH									BIT_23		// Under _NEDITOR only: draw the path
// #define FLAG_NLPATH_EDITING_DRAW_WHEEL_TRACKS							BIT_19		// Under _NEDITOR only: draw left and right wheel tracks
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_HANDLES BIT_9				 // Under _NEDITOR only: draw all the handles used to edit the path ( tangent, circle handles ... )
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_CIRCLES BIT_8			 // Under _NEDITOR only: draw all the buildCircles used to build the path ( 4 by section )
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_BASES BIT_7			 // Under _NEDITOR only: draw all the orthogonal bases builded and used to create the path
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_LINES BIT_6		 // Under _NEDITOR only: draw all the lines (and points ) defined and used when building a pair of circles.
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_BETA_ANGLES BIT_5	 // Under _NEDITOR only: draw
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_ALPHA_ANGLES BIT_4 // Under _NEDITOR only: draw
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_3 BIT_3			 // Under _NEDITOR only: draw
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_2 BIT_2			 // Under _NEDITOR only: draw
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_1 BIT_1			 // Under _NEDITOR only: draw
#define FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_PAIR_0 BIT_0			 // Under _NEDITOR only: draw
#endif

class NLPATH_BUILDER_WAYPOINTS : NLPATH_BUILDER
{
public:
	NLPATH_BUILDER_WAYPOINTS();
	//	NLPATH_BUILDER_WAYPOINTS(NLPATH* plinkedpath);
	virtual ~NLPATH_BUILDER_WAYPOINTS();
	virtual void build(NLPATH *ppath);
	virtual Nu32 read(FILE *pfile);
	virtual Nu32 write(FILE *pfile);

	NLPATH_WAYPOINT *pushBackWayPoint(const NLPATH_WAYPOINT *pwp);
	NLPATH_WAYPOINT *pushBackWayPoint(const NVEC2f32 *ppos);
	NLPATH_WAYPOINT *pushBackWayPoint(const NVEC2f32 *ppos, const NVEC2f32 *pdirection);
	NLPATH_WAYPOINT *pushBackWayPoint(const NVEC2f32 *ppos, const NVEC2f32 *pdirection, const Nf32 radiusratio, const Nf32 icr_A, const Nf32 icr_C);

	NLPATH_WAYPOINT *insertWayPoint(const Nu32 insertbefore_index, const NLPATH_WAYPOINT *pwp);
	NLPATH_WAYPOINT *insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32 *ppos);
	NLPATH_WAYPOINT *insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32 *ppos, const NVEC2f32 *pdirection);
	NLPATH_WAYPOINT *insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32 *ppos, const NVEC2f32 *pdirection, const Nf32 radiusratio, const Nf32 icr_A, const Nf32 icr_C);

	void eraseWaypoint(const Nu32 index);
	void eraseWaypoint(NLPATH_WAYPOINT *pwp);

	inline const Nu32 getWayPointsNumber() { return m_wayPointsArray.Size; }
	inline NARRAY *getArrayOfWayPoints() { return &m_wayPointsArray; }

	inline const Nu32 getSectionsNumber() { return m_sectionsArray.Size; }
	inline NARRAY *getArrayOfSections() { return &m_sectionsArray; }

	void setHeuristic(NLPATH_HEURISTIC heuristic) { m_heuristic = heuristic; }

#ifdef _NEDITOR
	virtual void draw(); // trace les "WayPoints" à l'écran avec leur tangente.
	virtual void draw(NL2DOCS *pocs);

	void drawSection(const Nu32 section_index, const NCOLOR *pcolcircles, const NCOLOR *pcolpair, const NCOLOR *pcolalpha, const NCOLOR *pcolbeta); // trace une section ( surtout ses données de construction en utilisant les flags de NLPATH )
	Nu32 getSectionIndex(const Nu32 primitive_index);
	Nu32 getSectionIndexXtd(const Nu32 primitive_index);

	Nf32 m_TgtSize;
	// NVEC2f32			m_ICircleHandleExtend;
	NVEC2f32 m_WayPointExtend;

	NCOLOR m_HandleColor;
	NCOLOR m_WayPointPColor;
	NCOLOR m_WayPointUColor;
	// NCOLOR				m_WarningColor;
#endif
	Nu32 m_flags;
	NARRAY m_wayPointsArray;
	NARRAY m_sectionsArray;
	Nf32 m_arcLengthThreshold;
	NLPATH_HEURISTIC m_heuristic;
	// private:
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLPATH_BUILDER_WAYPOINTS_HEADER NMAKEVERSION(0, 0, 0) // >>> 2021/04/03
typedef struct
{
	Nu32 m_flags;
	NARRAYBOUNDS m_wayPointsArrayBounds;
	Nf32 m_arcLengthThreshold;
	Nu8 m_heuristicId; // 0 = shortest, 1 = lesscurvy

	Nu8 m_availableNu8;
	Nu16 m_availableNu16;
} NLPATH_BUILDER_WAYPOINTS_HEADER;
