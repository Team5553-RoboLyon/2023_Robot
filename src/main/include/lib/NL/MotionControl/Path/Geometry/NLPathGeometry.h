#pragma once
#include "../../../../N/NType.h"
#include "../../../../N/Maths/NMatrix.h"
//#include "../../../../N/File/NFile.h"
#include "../../../../N/Containers/NArray.h"
#include "../../../../N/Core/NVersion.h"

#include "NLPathPoint.h"
#include "NLPathPrimitives.h"

#ifdef _NEDITOR
#include "../../../NL2DOrthogonalCoordinateSystem.h"
#define DEFAULT_NLPATH_GEOMETRY_PATHPOINT_XEXTEND			0.035f
#define DEFAULT_NLPATH_GEOMETRY_PATHPOINT_YEXTEND			0.035f
#define DEFAULT_NLPATH_GEOMETRY_CLOTHOID_DRAW_ACCURACY		256
#define DEFAULT_NLPATH_GEOMETRY_ARC_DRAW_ACCURACY			64

#define DEFAULT_NLPATH_GEOMETRY_SEGMENT_COLOR				NCOLOR_PRESET3F_YELLOW_LEMON,1
#define DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORA				NCOLOR_PRESET3F_BRONZE,1
#define DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORB				NCOLOR_PRESET3F_BRONZE_GOLD,1
#define DEFAULT_NLPATH_GEOMETRY_ARC_COLOR					NCOLOR_PRESET3F_GREEN_SPRING,1
#define DEFAULT_NLPATH_GEOMETRY_PATHPOINT_COLOR				NCOLOR_PRESET3F_GREEN_EMERALD,1
#define DEFAULT_NLPATH_GEOMETRY_PATHPOINTID_COLOR			NCOLOR_PRESET3F_ORANGE_GOLD,1
#define DEFAULT_NLPATH_GEOMETRY_WARNING_COLOR				NCOLOR_PRESET3F_RED_SCARLET,1

//#define FLAG_NLPATH_GEOMETRY_DRAW_TRACKS					BIT_0


#endif // _NEDITOR

#define DEFAULT_NLPATH_PATHPOINTS_ARRAYCAPACITY		8
#define DEFAULT_NLPATH_PRIMITIVES_ARRAYCAPACITY		8


typedef struct NLPATH_GEOMETRY NLPATH_GEOMETRY;
struct NLPATH_GEOMETRY
{
	NLPATH_GEOMETRY();
	NLPATH_GEOMETRY(const Nu32 pathpoints_capacity, const Nu32 primitives_capacity);
	~NLPATH_GEOMETRY();
	NLPATH_GEOMETRY(NLPATH_GEOMETRY const &tocopy);
	NLPATH_GEOMETRY& operator=(const NLPATH_GEOMETRY& from);
	Nu32	write(FILE* pfile);
	Nu32	read(FILE* pfile);

	inline void erase() { NEraseArray(&m_pathPointsArray, NLclearNLPathPrimitivePtrInArrayCallBack); NEraseArray(&m_primitivesArray, NLclearNLPathPrimitiveInArrayCallBack); m_ds = 0.0f; /*m_flags = 0;*/ }

	//void getPathPoint(NLPATH_POINT_S *pres, const Nf32 s)const;
	//void getPathPoint(NLPATH_POINT_S *pres, const NLPATH_POINT *pkp1, const NLPATH_PRIMITIVE *pprim, const Nf32 s)const;
	void getPathPoint(NLPATH_POINT *pres, const Nf32 s)const;
	void getPathPoint(NLPATH_POINT *pres, const NLPATH_POINT *pkp1, const NLPATH_PRIMITIVE *pprim, const Nf32 s)const;
	const NLPATH_POINT* getPathPoint(const Nu32 index);
#ifdef _NEDITOR
	NCOLOR				m_KeyPointColor;
	NCOLOR				m_KeyPointIDColor;
	NCOLOR				m_SegmentColor;
	NCOLOR				m_ArcColor;
	NCOLOR				m_ClothoidColorA;
	NCOLOR				m_ClothoidColorB;
	NCOLOR				m_WarningColor;

	NVEC2f32			m_KeyPointExtend;
	Nu32				m_ClothoidDrawAccuracy;
	Nu32				m_ArcDrawAccuracy;

	//Nu32				m_flags;
	void draw(const NMATRIX *pmx, const Nf32 path_width);
	void draw(const NMATRIX* pmx, const Nf32 path_width, const Nf32 s0, const Nf32 s1);
	void drawCurvature(NL2DOCS* pocs,const NCOLORPICKPACK pickpack);
#endif

	NARRAY				m_pathPointsArray;		// liste des points clefs du chemin
	NARRAY				m_primitivesArray;		// liste des primitives du chemin
	Nf32				m_ds;					// longueur du chemin
};


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLPATH_GEOMETRY_HEADER			NMAKEVERSION(0,0,0)				// >>> 2021/04/05
typedef struct
{
	Nu32				m_flags;
	Nf32				m_ds;
//	Nu32				m_keyPointsArraySize;
//	Nu32				m_primitivesArraySize;
	NARRAYBOUNDS		m_keyPointsArrayBounds;		
	NARRAYBOUNDS		m_primitivesArrayBounds;	

}NLPATH_GEOMETRY_HEADER;
