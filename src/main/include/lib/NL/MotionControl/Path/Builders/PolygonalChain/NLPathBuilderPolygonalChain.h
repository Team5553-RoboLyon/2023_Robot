#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Core/NVersion.h"

#include "lib/NL/MotionControl/Path/Builders/NLPathBuilder.h"
#include "lib/NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalVertex.h"

#ifdef _NEDITOR
#include "../../../../NL2DOrthogonalCoordinateSystem.h"

// Pour dessiner les Polygonal Paths...
/*
#define DEFAULT_NL_CLOTHOID_DRAW_ACCURACY					256
#define DEFAULT_NL_ARC_DRAW_ACCURACY						64

#define CONSTANT_NL_WAYPOINT_TANGENT_ARROWHEAD_SIZE_RATIO	0.1f				// la tete de fleche de la tangente
#define CONSTANT_NL_WAYPOINT_TANGENT_ARROWTAIL_SIZE_RATIO	0.075f				// à la naissance de la fleche, longueur du 'petit espace'

#define DEFAULT_NL_KEYPOINT_XEXTEND							0.035f
#define DEFAULT_NL_KEYPOINT_YEXTEND							0.035f
#define DEFAULT_NL_WAYPOINT_XEXTEND							0.05f
#define DEFAULT_NL_WAYPOINT_YEXTEND							0.05f
#define DEFAULT_NL_WAYPOINT_TGT_SIZE						2.0f

#define DEFAULT_NL_WAYPOINT_P_COLOR							NCOLOR_PRESET3F_BLUE_ELECTRIC,1
#define DEFAULT_NL_KEYPOINTID_COLOR							NCOLOR_PRESET3F_ORANGE_GOLD,1
#define DEFAULT_NL_WAYPOINT_COLOR							NCOLOR_PRESET3F_GREEN_JADE,1
#define DEFAULT_NL_HANDLE_COLOR								NCOLOR_PRESET3F_GREEN_HARLEQUIN,1
#define DEFAULT_NL_HELPERS_COLOR							NCOLOR_PRESET3F_BLUE_CERULEAN,1
#define DEFAULT_NL_SEGMENT_COLOR							NCOLOR_PRESET3F_YELLOW_LEMON,1
#define DEFAULT_NL_CLOTHOID_COLORA							NCOLOR_PRESET3F_BRONZE,1
#define DEFAULT_NL_CLOTHOID_COLORB							NCOLOR_PRESET3F_BRONZE_GOLD,1
#define DEFAULT_NL_ARC_COLOR								NCOLOR_PRESET3F_GREEN_SPRING,1
#define DEFAULT_NL_KEYPOINT_COLOR							NCOLOR_PRESET3F_GREEN_EMERALD,1
#define DEFAULT_NL_KEYPOINTID_COLOR							NCOLOR_PRESET3F_ORANGE_GOLD,1
#define DEFAULT_NL_WAYPOINT_P_COLOR							NCOLOR_PRESET3F_BLUE_ELECTRIC,1
#define DEFAULT_NL_WAYPOINT_U_COLOR							NCOLOR_PRESET3F_BLUE_AQUAMARINE,1

*/
#define DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_XEXTEND 0.05f
#define DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_YEXTEND 0.05f
#define DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_LENGTH 0.25f

#define DEFAULT_NL_PATH_BUILDER_PLG_HANDLE_COLOR NCOLOR_PRESET3F_BLUE_AZURE, 1	  // NCOLOR_PRESET3F_GREEN_HARLEQUIN,1
#define DEFAULT_NL_PATH_BUILDER_PLG_VERTEX_COLOR NCOLOR_PRESET3F_BLUE_CERULEAN, 1 // NCOLOR_PRESET3F_GREEN_JADE,1
#define DEFAULT_NL_PATH_BUILDER_PLG_SEGMENT_COLOR NCOLOR_PRESET3F_BLUE_TEAL, 1	  // NCOLOR_PRESET3F_YELLOW_LEMON,1
#define DEFAULT_NL_WARNING_COLOR NCOLOR_PRESET3F_RED_SCARLET, 1
#endif

#define DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ARRAY_CAPACITY 4

#ifdef _NEDITOR
// FLAGS Spécifiques à ce builder:
//																	.
//																	.
//																	.
#define FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_VERTEX BIT_2  // Under _NEDITOR only: draw Vertex
#define FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_SEGMENT BIT_1 // Under _NEDITOR only: draw Segment between vertex
#define FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_HANDLE BIT_0  // Under _NEDITOR only: draw vertex handle
#endif

class NLPATH_BUILDER_POLYGONAL_CHAIN : NLPATH_BUILDER
{
public:
	NLPATH_BUILDER_POLYGONAL_CHAIN();
	//	NLPATH_BUILDER_POLYGONAL_CHAIN(NLPATH* plinkedpath);
	virtual ~NLPATH_BUILDER_POLYGONAL_CHAIN();

	virtual void build(NLPATH *ppath);
	virtual Nu32 read(FILE *pfile);
	virtual Nu32 write(FILE *pfile);
	// new
	NLPATH_BUILDER_PLG_VERTEX *pushBackVertex(const NLPATH_BUILDER_PLG_VERTEX *pv);
	NLPATH_BUILDER_PLG_VERTEX *pushBackVertex(const NVEC2f32 *ppos);

	NLPATH_BUILDER_PLG_VERTEX *insertVertex(const Nu32 insertbefore_index, const NLPATH_BUILDER_PLG_VERTEX *pv);
	NLPATH_BUILDER_PLG_VERTEX *insertVertex(const Nu32 insertbefore_index, const NVEC2f32 *ppos);

	// void						setVertexPosition(NLPATH_BUILDER_PLG_VERTEX* pv, const NVEC2f32* ppos);
	void eraseVertex(const Nu32 index);

	inline const Nu32 getVertexNumber() { return m_vertexArray.Size; }
	inline NARRAY *getArrayOfVertex() { return &m_vertexArray; }

#ifdef _NEDITOR
	virtual void draw(); // trace le "polygone" à l'écran avec ses segments et ses vertex.
	virtual void draw(NL2DOCS *pocs);

	// Sizes
	NVEC2f32 m_handleExtend;
	Nf32 m_handleLength;

	// Colors
	NCOLOR m_HandleColor;
	NCOLOR m_SegmentColor;
	NCOLOR m_vertexColor;
	NCOLOR m_WarningColor;
#endif
	Nu32 m_flags;
	NARRAY m_vertexArray;	   // Array of plg vertex
	NARRAY m_buildingArcArray; // Array of arc

private:
	void updatePolygonalChainVertexFlagsIn(NLPATH_BUILDER_PLG_VERTEX *pv);
	void updatePolygonalChainVertexFlagsOut(NLPATH_BUILDER_PLG_VERTEX *pv);
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER NMAKEVERSION(0, 0, 0) // >>> 2021/04/03
typedef struct
{
	Nu32 m_flags;
	//	Nu32				m_vertexArraySize;
	//	Nu32				m_buildingArcArraySize;
	NARRAYBOUNDS m_vertexArrayBounds;
	NARRAYBOUNDS m_buildingArcArrayBounds;
} NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER;
