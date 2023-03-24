#ifndef __NUT_H
#define __NUT_H

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NCamera.h"
#include "NType.h"
#include "NMath.h"
#include "NVector.h"
#include "NRectf32.h"
#include "NGeometry.h"
#include "NArray.h"
#include "NFile.h"
#include "NSpline.h"
#include "NStructure.h"
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
/*
typedef enum NUT_RENDERING_MODE
{
	NUT_STANDARD3D_RENDERING,
	NUT_ORTHOGRAPHIC_RENDERING,
	//-------------------------
	NUT_RENDERING_MODE_ENUM_SIZE
}NUT_RENDERING_MODE;

#define NUT_LOWER_SHAPEACCURACY					512
#define NUT_LOW_SHAPEACCURACY					128
#define NUT_MEDIUM_SHAPEACCURACY				64
#define NUT_HIGH_SHAPEACCURACY					32
#define NUT_BEST_SHAPEACCURACY					2

#define NUT_DEFAULT_SHAPE_PRIMITIVE_CAPACITY	256

extern NARRAY	NUT_DrawableShapesList[NUT_RENDERING_MODE_ENUM_SIZE];
extern Nu32		NUT_ShapeAccuracy;
*/
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							 Geometries Creation									**
// **									---												**
// **						enums, structures and define								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// construction Planes used for all planar geometry building like quad, circle, etc ...
/*
typedef enum NUT_CONSTRUCTION_PLANE
{
	NUT_PLANE_XY,
	NUT_PLANE_XZ,
	NUT_PLANE_YZ
}NUT_CONSTRUCTION_PLANE;

#define FLAG_QUAD_DESC_USEPARAM_SCRUV									BIT_0
#define FLAG_QUAD_DESC_USEPARAM_UV										BIT_1

//#define FLAG_QUAD_DESC_USEPARAM_SHIFTING								BIT_2
#define FLAG_QUAD_DESC_USEPARAM_CONSTRUCTION_ORIGIN						BIT_2
#define FLAG_QUAD_DESC_USEPARAM_ANCHOR									BIT_3

#define FLAG_QUAD_DESC_USEPARAM_SCALE									BIT_4

#define FLAG_QUAD_DESC_QUADSIZE_FROM_TEXTURESIZE						BIT_5
#define FLAG_QUAD_DESC_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR		BIT_6
#define FLAG_QUAD_DESC_USEPARAM_CONSTRUCTIONPLANE						BIT_7

// A Quad Description.
// Used to insert a Quad with Vertex,UVs,Colors and triangles into a geometry
typedef struct
{
	Nu32						flags;
	NUT_CONSTRUCTION_PLANE		construction_plane;
	NVEC3f32					ConstructionPlane_DirectorVector;
// 	Nf32						Width;
// 	Nf32						Height;
	NVEC2f32					Size;

	NSCREENRECT16				scrUV;					// UV rect in screen coord system (pixel). Used to get texture rect into ATLAS texture
	NRECTf32					UV;						// UV Rect in normalized format (0.0f < uv < 1.0f ). BUT Can be greater than 1.0f for Tiling.

	//NVEC3f32					Shifting;
	NVEC3f32					ConstructionOrigin;
	NVEC3f32					Anchor;
	NVEC2f32					Scale;

	NCOLOR						ColorRange[RANGE_4COLORS];
}NUT_QUAD_DESC;

// A SplineStrip Description.
// Used to insert a Ribbon (spline strip) with Vertex,UVs,Colors and triangles into a geometry
// All SHAPE TYPE are built with the function 'NUT_DrawShape'
typedef enum
{
	NUT_SPLINESTRIP_ONESPLINE_ONEVSHIFT,
	NUT_SPLINESTRIP_ONESPLINE_TWOVSHIFTS,
	NUT_SPLINESTRIP_TWOSPLINES,
	NUT_SPLINESTRIP_TWOSPLINES_TWOVSHIFTS,
	//---------------
	NUT_SPLINESTRIP_ENUMSIZE
}NUT_SPLINESTRIP_TYPE;

typedef struct
{
	Nu32						flags;

	NUT_SPLINESTRIP_TYPE		Type;
	NSPLINE						*psplineA;
	NSPLINE						*psplineB;
	NVEC3f32					VShift1;
	NVEC3f32					VShift2;
	Nu32						NbSubSplineSlice;
	NTEXTURE					*pTexture;
	Nf32						UTilingOrigin;
	Nf32						UTilingRatio;
	Nf32						VTiling;
	Nf32						GeometrySortingKey;
}NUT_SPLINESTRIP_DESC;
*/

/*
// All SHAPE TYPE are built with the function 'NUT_DrawShape'
typedef enum
{
	NUT_SHAPE_SEGMENT,
	NUT_SHAPE_QUAD,
	NUT_SHAPE_ELLIPSE,
	NUT_SHAPE_BOUNDINGBOX,
	NUT_SHAPE_SPLINE,
	NUT_SHAPE_V3ARRAY,
	NUT_SHAPE_TRIARRAY,
	NUT_SHAPE_ARROW,
	//---------------
	NUT_SHAPE_SIZE
}NUT_SHAPE_TYPE;


typedef union NUT_SHAPE NUT_SHAPE;
typedef void (*NUT_DRAWSHAPE_CALLBACK)(NUT_SHAPE* pshape);

// A 2D boundingBox ( = a rectangle)
typedef struct
{
//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;

	NVEC3f32					ConstructionPlane_DirectorVector;
	NRECTf32					BoundingBox;
}NUT_2DBOUNDINGBOX_POLYGON;

// A Convex polygon.
// The number of segments created is in relationship with the value of 'NUT_ShapeAccuracy'
typedef struct
{
//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;

	NVEC3f32					ConstructionPlane_DirectorVector;
	NVEC3f32					CenterShifting;
	Nf32						Width;
	Nf32						Height;
}NUT_CONVEX_POLYGON;

// A Segment description
typedef struct
{
//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;

	NVEC3f32						A;
	NVEC3f32						B;
}NUT_SEGMENT;

// A Spline description
#define FLAG_NUTSHAPE_SPLINE_HIDESPLINE				BIT_0
#define FLAG_NUTSHAPE_SPLINE_HIDESPLINEPOINTS		BIT_1
#define FLAG_NUTSHAPE_SPLINE_HIDETANGENTES			BIT_2
typedef struct
{
//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;
	Nf32						PointSize;
	NSPLINE						*pspline;
	Nu32						Flags;
}NUT_SPLINE;

// An Array description
typedef struct
{
	//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;
	NARRAY						*pArray;
}NUT_V3ARRAY;

// A Triangle Array description (with the vertex)
// Helpful to easy draw a geometry
typedef struct
{
	//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;
	NARRAY						*pTrianglesArray;
	NARRAY						*pVertexArray;
}NUT_TRIANGLES_ARRAY;

// A 2D Arrow(s) ( = a segment with/without arrows on each extremity)
typedef struct
{
	//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;
	NCOLOR						Color;

	NVEC3f32					Construction_Vector; // with AB, define a plan orthogonal with the arrows plan
	NVEC3f32					A;					 // Extremity of A arrow
	NVEC3f32					B;					 // Extremity of B arrow
	Nf32						AArrowSize;			 // relative size of A Arrow ( relative to AB size )
	Nf32						BArrowSize;			 // relative size of B Arrow ( relative to AB size )
}NUT_2DARROW;

// General shape structure
union NUT_SHAPE
{
//	NUT_SHAPE_TYPE				Type;
	NUT_DRAWSHAPE_CALLBACK		pDrawSphapeCallBack;

	NUT_2DBOUNDINGBOX_POLYGON	BBox;
	NUT_SEGMENT					Segment;
	NUT_CONVEX_POLYGON			Polygon;
	NUT_SPLINE					Spline;
	NUT_V3ARRAY					V3Array;
	NUT_TRIANGLES_ARRAY			TriArray;
	NUT_2DARROW					Arrow;
};

*/

// PICK RESULT STRUCTURES
// Spline
//faire une union pickspline point (comme ici) avec pickspline splinepoint
/*
typedef struct
{
	NVEC3f32	PickedPosition;
	NVEC3f32	EstimatedTA;
	NVEC3f32	EstimatedTB;
	Nu32		BSplinePointIndex;	//Nu32	ASplinePointIndex; ... is  not necessary because it's obvious: A = B-1. Always !
	Nf32		LerpFactor;
	Nf32		fDot;				// ZfromCam indeed ...
}NUT_PICKSPLINE;

typedef struct
{
	NVEC3f32	*pPickedV3Element;
	Nu32		PickedElementIndex;
	Nf32		PickingDistance;
}NUT_PICKV3ARRAY;
*/

/*

// Segment X Result. Used by all the '2DSegX' functions
typedef struct
{
	//NVEC2f32		XPos;		// Intersection Point 2D coordinates
	Nf32			x;
	Nf32			y;
	Nf32			t;			// Normalized ratio to place I (intersection point) like this: I = A+t*AB
}NUT_2DSEGMENTX_RESULT;

typedef struct
{
	//NVEC2f32		XPos;		// Intersection Point 2D coordinates
	Nf32			x;
	Nf32			y;
	Nf32			t;			// Normalized ratio to place I (intersection point) like this: I = A+t*AB
	// -----------------------------------------------------------------------------------------------------------------------------
	Nf32			t2;			// Normalized ratio to place I (intersection point) like this: I = C+t2*CD, only used by "Seg X Seg"
}NUT_2DSEGMENTX_RESULT_EXTEND;

// Used by advanced Crossing functions like:
//						NUT_2DVerticalLineXSplineExtended
typedef struct
{
	NUT_2DSEGMENTX_RESULT		XResult;		// Intersection Point 2D coordinates
	// --------------------------------------
	NSPLINE			*pSpline;
	NSPLINEKNOT		*pSplinePointB;
	Nf32			SubSplineRatio_0;
	Nf32			SubSplineRatio_1;
	// --------------------------------------
	NVEC3f32		ReferencePosition_0;
	NVEC3f32		ReferencePosition_1;
	// --------------------------------------
}NUT_XSPLINE_PERSISTENTDATA;
*/
/*

#define FLAG_SPRINGYSTRUCTURE_CREATESKIN				BIT_0

typedef struct NUT_SPRINGYSTRUCTURE_BUILDER NUT_SPRINGYSTRUCTURE_BUILDER;
struct NUT_SPRINGYSTRUCTURE_BUILDER
{
	Nu32				Flags;

	Nu32				StagesNb;					// Number of "Stages".	1 stage  = 4 joints = 6 springs ( 2 Horizontals Springs + 2 Vertical Springs + 2 X springs )
													//						2 stages = 6 joints = 11 springs( 3 Horizontals Springs + 4 Vertical Springs + 4 X springs )
//	Nf32				Segment_VLength;			// Length of All the Vertical springs.
//	Nf32				Segment_HLength;			// Length of All the Horizontal springs.

	NVEC3f32			ToNextLineShiftingVector;	// Shifting Apply from the current joints Line to create the next joints line
	NVEC3f32			ToNextColumnShiftingVector; // Shifting Apply from the current joints column to create the next joints column

	Nf32				JointMass;					// Mass for all Joints in the structure
	Nf32				JointRadius;				// collision radius for all joints

	Nf32				Spring_Friction;			// Friction for all springs
	Nf32				Spring_K;					// K for all springs
	Nf32				Spring_RestLengthRatio;		// Ratio used to calculate the final RestLength stored in each Spring (restlengthstored = length*ratio)
	NVEC3f32			vGravity;

	Nu32				SkinAccumulatorSpecFlags;	// Flags to give to the skin builder ...
	NTEXTURE			*pSkinTexture;				// Texture used by the skin ....................Use only if FLAG_SPRINGYSTRUCTURE_CREATESKIN is set to ON

	NUV					SkinUVOrigin;
	NUV					SkinUVTiling;
};
*/

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **																			FUNCTIONS																		**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
//void NUT_Init();
//void NUT_Draw(Nbool reset_list);
//void NUT_Quit();

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							   'DIRECT' DRAW										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
inline void				NUT_DrawShape(NUT_SHAPE *desc,NUT_RENDERING_MODE mode){	NArrayPush_Back(&NUT_DrawableShapesList[mode],(BYTE*)desc);}

void					NUT_Draw3DSegment(NVEC3f32 *A, NVEC3f32 *B, NCOLOR *color);
void					NUT_DrawOrthographicSegment(NVEC3f32 *A, NVEC3f32 *B, NCOLOR *color);

//
NUT_DRAWSHAPE_CALLBACK	NUT_GetDrawShapeCallBack(NUT_SHAPE_TYPE shapetype);
inline Nu32				NUT_GetShapeAccuracy(){return NUT_ShapeAccuracy;};
inline void				NUT_SetShapeAccuracy(Nu32 acc){NUT_ShapeAccuracy = acc;if(!NUT_ShapeAccuracy){NUT_ShapeAccuracy = 1;}if(NUT_ShapeAccuracy>COSSINLUT_SIZE/3){NUT_ShapeAccuracy = COSSINLUT_SIZE/3;}};



// Specific DrawShape CallBack Functions
void NUT_DrawShapeCallBack_Segment(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_Quad(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_Ellipse(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_BoundingBox(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_Spline(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_V3Array(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_TrianglesArray(NUT_SHAPE *pshape);
void NUT_DrawShapeCallBack_Arrow(NUT_SHAPE *pshape);
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							 Geometries Creation									**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
void	NUT_InsertQuad(NGEOMETRY *pgeom,NUT_QUAD_DESC *desc); // build with 2 triangles.
Nu32	NUT_CreateSplineStripObjects( NNODELIST *pownerlist, NUT_SPLINESTRIP_DESC *pdesc);
void	NUT_CloneGeometryContent(NGEOMETRY *pgeom, NVEC3f32 *pvspacing, Nu32 along_xaxis, Nu32 along_yaxis, Nu32 along_zaxis );
*/



// Result Data: used by 'NUT_GetClosestPointOnSpline'
/*
typedef struct
{
	NSPLINEKNOT		*pSplineKnotA;
	NSPLINEKNOT		*pSplineKnotB;
	Nu32			SplineKnotBIndex;	//... is  not necessary to store KnotA because it's obvious: A = B-1. Always !

	Nf32			LerpFactorEstimatedResult;
	Nf32			Distance;
	NVEC3f32		VResult;
}NUT_CLOSESTPOINT_ON_SPLINE;
*/
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							Crossing/Intersection									**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
NUT_XSPLINE_PERSISTENTDATA*	NUT_SetUpXSplinePersistentData(NUT_XSPLINE_PERSISTENTDATA* pprs, NSPLINE *pspline);
NUT_XSPLINE_PERSISTENTDATA*	NUT_CreateXSplinePersistentData(NSPLINE *pspline);
void						NUT_ClearXSplinePersistentData(void* pobj);
void						NUT_DeleteXSplinePersistentData(void* pobj);

Nbool	NUT_2DSegXSubSplineFastAABB(NVEC2f32 *psegA,NVEC2f32 *psegB, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb);
Nbool	NUT_2DSegXSeg(NVEC2f32 *A,NVEC2f32 *B, NVEC2f32 *C, NVEC2f32 *D, NUT_2DSEGMENTX_RESULT_EXTEND *pxrx);
Nbool	NUT_2DSegXLine(NVEC2f32 *psegA, NVEC2f32 *psegB, NVEC2f32 *plineC,NVEC2f32 *plineD,  NUT_2DSEGMENTX_RESULT *pxr);
Nbool	NUT_2DSegXSpline(NVEC2f32 *psegA,NVEC2f32 *psegB, NSPLINE *pspline, NUT_2DSEGMENTX_RESULT_EXTEND *pxrx);
Nbool	NUT_2DSegXVerticalLine( NVEC2f32 *psegA, NVEC2f32 *psegB, NVEC2f32 *plineC, NUT_2DSEGMENTX_RESULT *pxr);
Nu8		NUT_2DSegX2DCircle(NVEC2f32 *pA, NVEC2f32 *pB,NVEC2f32 *pC,Nf32 radius, NUT_2DSEGMENTX_RESULT *pR1, NUT_2DSEGMENTX_RESULT *pR2);

Nu32	NUT_2DSegV2ArrayXLine(NARRAY *pv2array,NVEC2f32 *plineC,NVEC2f32 *plineD,  NUT_2DSEGMENTX_RESULT *pxr);
Nu32	NUT_2DSegV2ArrayXVerticalLine(NARRAY *pv2array, NVEC2f32 *plineC,  NUT_2DSEGMENTX_RESULT *pxr );
Nu32	NUT_2DSegV3ArrayXLine(NARRAY *pv3array, NVEC2f32 *plineC,NVEC2f32 *plineD,  NUT_2DSEGMENTX_RESULT *pxr);
Nu32	NUT_2DSegV3ArrayXVerticalLine( NARRAY *pv3array, NVEC2f32 *plineC, NUT_2DSEGMENTX_RESULT *pxr );

Nbool	NUT_2DLineXLine(NVEC2f32 *A,NVEC2f32 *B, NVEC2f32 *C, NVEC2f32 *D, NUT_2DSEGMENTX_RESULT *pxr);

Nbool	NUT_2DSplineXLine(NSPLINE *pspline, NVEC2f32 *plineA,NVEC2f32 *plineB, NUT_2DSEGMENTX_RESULT *pxr);
Nbool	NUT_2DSplineXVerticalLine( NSPLINE *pspline, NVEC2f32 *plineA,  NUT_2DSEGMENTX_RESULT *pxr);
Nbool	NUT_2DSplineXVerticalLineExtended( NVEC2f32 *plineA, NUT_XSPLINE_PERSISTENTDATA *ppersistent);

Nbool	NUT_2DVerticalLineXSubSplineFastAABB(const NVEC2f32 *plineA, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb);

Nbool	NUT_LineXPLane(NVEC3f32 *plineA,NVEC3f32 *plinedir, NVEC3f32 *pplaneP, NVEC3f32 *pplanedir,NVEC3f32 *presult);
Nf32	NUT_LinePointShortestDistance(NVEC3f32 *pline_o,NVEC3f32 *pline_dir,NVEC3f32 *ppoint);


void	NUT_GetClosestPointOnSpline( NVEC3f32 *pC, NSPLINE *pspline,NUT_CLOSESTPOINT_ON_SPLINE *presult );


#define	XEPSILON				NF32EPSILON			// (X for Cross !) tolerance value used to check some result against it (if value < XEPSILON means value = 0 !) used to check for degenerate lines, or parallel lines
#define	XEPSILON_SQUARED		XEPSILON*XEPSILON	// Cristal clear
#define OUT_OF_SEGMENTRANGE(a) ((a) < 0.0f || (a) > 1.f)

// Result Data: used by 'NUT_GetClosestPointOnLine' 'NUT_GetClosestPointOnSegment'
typedef struct
{
	NVEC3f32	PointOnLine;
	Nf32		ParamCoord;
}NUT_CLOSESTPOINTONLINE_RESULT;
// Result Data: used by 'NUT_3DLineXLine' 'NUT_3DLineXSeg' 'NUT_3DSegXSeg'
typedef struct
{
	// ----------------------
	NVEC3f32	PointOnAB;		// |__ NUT_CLOSESTPOINTONLINE_RESULT;
	Nf32		ParamCoordAB;	// |
	// ----------------------
	NVEC3f32	PointOnCD;		// |__ NUT_CLOSESTPOINTONLINE_RESULT;
	Nf32		ParamCoordCD;	// |
	// ----------------------
	//NVEC3f32	NearestPoint;
}NUT_3DLINEXLINE_RESULT;

// Result Data: used by 'NUT_GetClosestPointOnSpline'
typedef struct
{
	NSPLINEKNOT					*pSplineKnotA;
	NSPLINEKNOT					*pSplineKnotB;
	Nu32						SplineKnotBIndex;	//... is  not necessary to store KnotA because it's obvious: A = B-1. Always !

	Nf32						LerpFactorEstimatedResult;
	Nf32						Distance;
	NUT_3DLINEXLINE_RESULT		XResult;
}NUT_3DLINEXSPLINE;

Nbool	NUT_3DLineXLineResult_Extract_EstimatedIntersectionPoint(NVEC3f32* pintersectionpoint,NUT_3DLINEXLINE_RESULT *pprecalculatedresult);
Nf32	NUT_3DLineXLineResult_Extract_EstimateShortestDistance(NUT_3DLINEXLINE_RESULT *pprecalculatedresult);
Nf32	NUT_3DLineXLineResult_Extract_EstimateShortestSquaredDistance(NUT_3DLINEXLINE_RESULT *pprecalculatedresult);

void	NUT_3DLineXLine(NVEC3f32 *plineA, NVEC3f32 *plineB, NVEC3f32 *plineC, NVEC3f32 *plineD, NUT_3DLINEXLINE_RESULT *presult );
void	NUT_3DLineXSeg(NVEC3f32 *plineA, NVEC3f32 *plineB, NVEC3f32 *psegC, NVEC3f32 *psegD, NUT_3DLINEXLINE_RESULT *presult );
void	NUT_3DSegXSeg(NVEC3f32 *plineA, NVEC3f32 *plineB, NVEC3f32 *plineC, NVEC3f32 *plineD, NUT_3DLINEXLINE_RESULT *presult );

void	NUT_GetClosestPointOnLine( NVEC3f32 *pA, NVEC3f32 *pdirvect_lineAB, NVEC3f32 *pC,NUT_CLOSESTPOINTONLINE_RESULT *presult);
void	NUT_GetClosestPointOnSegment( NVEC3f32 *pA, NVEC3f32 *pdirvect_segAB, NVEC3f32 *pC,NUT_CLOSESTPOINTONLINE_RESULT *presult);

void	NUT_GetClosestPointsOfParallelLines(NVEC3f32 *plineA,NVEC3f32* plineB, NVEC3f32* pdirvect_lineAB, NVEC3f32 *plineC,NVEC3f32* plineD, NVEC3f32* pdirvect_lineCD, NUT_3DLINEXLINE_RESULT *presult );
void	NUT_GetClosestPointsOfParallelSegments(NVEC3f32 *psegA,NVEC3f32* psegB, NVEC3f32* pdirvect_segAB, NVEC3f32 *psegC,NVEC3f32* psegD, NVEC3f32* pdirvect_segCD, NUT_3DLINEXLINE_RESULT *presult );
#define NUT_GetClosestPointsOfParallelLineSegment(plineA,plineB,pdirvect_lineAB,psegC,psegD,pdirvect_segCD,presult )	NUT_GetClosestPointsOfParallelLines(plineA,plineB,pdirvect_lineAB,psegC,psegD,pdirvect_segCD,presult )

void	NUT_3DlineXSpline( NVEC3f32 *plineA, NVEC3f32 *plineB, NSPLINE *pspline,NUT_3DLINEXSPLINE *presult );
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **							 Pick and Detection										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
Nf32	NUT_Pick3DPoint(NSCREENPOINT16 *pmouse2d,Nf32 pickradius, NVEC3f32 *pv);
Nf32	NUT_Pick3DPointEx(NVEC3f32 *prayorigin, NVEC3f32 *prayvdir,Nf32 pickradius, NVEC3f32 *pv);
Nbool	NUT_Pick3DSpline(NSCREENPOINT16 *pmouse2d,Nf32 pickradius, NSPLINE *pspline,NUT_PICKSPLINE *presult);
Nu32	NUT_PickVector3Array(NSCREENPOINT16 *pmouse2d,Nf32 pickradius, NARRAY *pv3array,NUT_PICKV3ARRAY *presult);


Nbool	NUT_2DPointInside2DTriangle(const NVEC2f32 *pA,const NVEC2f32 *pB,const NVEC2f32 *pC,const NVEC2f32 *p);
*/


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								   Files											**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								 NSTRUCTURE											**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// Enum used only by the function "NUT_PhysicRopeAlign"
/*
typedef enum
{
	NUT_PHYSIC_ROPE_ALIGN_USE_SPRING_RESTLENGTH=0,
	NUT_PHYSIC_ROPE_ALIGN_USE_SIMPLECONSTRAINT_LENGTH,
	NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MINLENGTH,
	NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MAXLENGTH,
	NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MINLENGTH,
	NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MAXLENGTH
}NUT_PHYSIC_ROPE_ALIGN;
Nf32			NUT_PhysicRopeAlign(NSTRUCTURE *pstructure,NVEC3f32 *axis,NUT_PHYSIC_ROPE_ALIGN method);

void			NUT_UseCurrentSpringsSizeAsRestSize(NSTRUCTURE *pstructure);

NSTRUCTURE*		NUTSetUpSpringyTowerStructure(NSTRUCTURE *pstructure,NVEC3f32 *pos,NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder);
NSTRUCTURE*		NUTCreateSpringyTowerStructure(NVEC3f32 *pos,  NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder );
inline void		NUTClearSpringyTowerStructure(NSTRUCTURE *ps){NClearPhysicStructure(ps);}
inline void		NUTDeleteSpringyTowerStructure(NSTRUCTURE *ps){NDeletePhysicStructure(ps);}

//Nf32		NUT_EstimateSpringyTowerRestLength(NSTRUCTURE *ptower);
Nf32			NUT_EstimateSpringyTowerRestingState(NSTRUCTURE *ptower,Nbool updatedata );

Nbool			NUT_IterateUntilPhysicStructureStabilization(NSTRUCTURE *ps, Nu32 time_max,Nf32 error_tolerance);*/

//NSTRUCTURE*	NUT_CreateRigidTowerStructure( NNODELIST *powner_list, NVEC3f32 *pos,  NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder );
//NSTRUCTURE* NUT_CreateSpringyRopeStructure( NNODELIST *powner_list, NVEC3f32 *pos,  NUT_SPRINGYSTRUCTURE_BUILDER *pbuilder );

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								Miscellaneous										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//Nu32 NUT_MakeUIDFromString( const char *ename );

/*
void NUT_UnProject2DMouse(NSCREENPOINT16 *pmouse2d, NVEC3f32 *pmouse3d);

void NUT_Get3DMouse(NCAMERA *pcam,NSCREENPOINT16 *pmouse2d, NVEC3f32 *pmouse3d);
void NUT_Get3DMouseEx(NCAMERA *pcam,NSCREENPOINT16 *pmouse2d, Nf32 z_camcoordsystem, NVEC3f32 *pmouse3d);
void NUT_Get3DMouseRay(NCAMERA *pcam,NSCREENPOINT16 *pmouse2d, NVEC3f32 *porigin, NVEC3f32 *pdirector);
void NUT_Get3DMouseRayEx(NCAMERA *pcam, NSCREENPOINT16 *pmouse2d, Nf32 z_camcoordsystem, NVEC3f32 *porigin, NVEC3f32 *pdirector);
void NUT_Get3DMouseRayPoints(NCAMERA *pcam, NSCREENPOINT16 *pmouse2d, NVEC3f32 *pmouserayA, NVEC3f32 *pmouserayB);
void NUT_Get3DMouseRayPointsEx(NCAMERA *pcam, NSCREENPOINT16 *pmouse2d, Nf32 z_camcoordsystem, NVEC3f32 *pmouserayA, NVEC3f32 *pmouserayB);
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								LUTComputing										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
/*
typedef enum
{
	NUT_LUTBUILD_MODE_CLAMP = 0,
	NUT_LUTBUILD_MODE_WRAP,
	NUT_LUTBUILD_MODE_NOLIMITS,
	// ------------------------
	NUT_LUTBUILD_MODE_ENUM_SIZE
}NUT_LUTBUILD_MODE_ENUM;

typedef enum
{
	NUT_LUTBUILD_OPERAND_PUSH = 0,
	NUT_LUTBUILD_OPERAND_ADD,
	NUT_LUTBUILD_OPERAND_SUB,
	NUT_LUTBUILD_OPERAND_SCALE,
	NUT_LUTBUILD_OPERAND_AVERAGE,
	// ------------------------
	NUT_LUTBUILD_OPERAND_ENUM_SIZE
}NUT_LUTBUILD_OPERAND_ENUM;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Nf32 LUT
void			NUT_LUTBuild_Nf32FromConstant(	NUT_LUTBUILD_OPERAND_ENUM operand, Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 constantvalue );

void			NUT_LUTBuild_Nf32FromFastRand(	NUT_LUTBUILD_OPERAND_ENUM operand, Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 scale );
void			NUT_LUTBuild_Nf32FromFastRandEx(NUT_LUTBUILD_OPERAND_ENUM operand, Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 scale,const Nu32 key_every );

void			NUT_LUTBuild_Nf32FromSpline(	NUT_LUTBUILD_OPERAND_ENUM operand, Nf32 *plut,const Nu32 element_nb,const Nu32 stride, NSPLINE *pspline, const Nf32 scale );

void			NUT_LUTBuild_Nf32Clamp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax );
void			NUT_LUTBuild_Nf32Warp( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nf32 newmin, const Nf32 newmax );
void			NUT_LUTBuild_Nf32Smooth( Nf32 *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NVEC3f32 LUT
void			NUT_LUTBuild_NVector3FromConstant( NUT_LUTBUILD_OPERAND_ENUM operand, NVEC3f32 *plut, Nu32 element_nb, Nu32 stride, NVEC3f32 *pvector );
void			NUT_LUTBuild_NVector3FromConstantf( NUT_LUTBUILD_OPERAND_ENUM operand,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, Nf32 x,Nf32 y, Nf32 z );

void			NUT_LUTBuild_NVector3FromFastRand( NUT_LUTBUILD_OPERAND_ENUM operand,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, NVEC3f32 *pscale );
void			NUT_LUTBuild_NVector3FromFastRandEx( NUT_LUTBUILD_OPERAND_ENUM operand,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, NVEC3f32 *pscale, const Nu32 key_every  );
void			NUT_LUTBuild_NVector3FromFastRandf( NUT_LUTBUILD_OPERAND_ENUM operand_fct,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, Nf32 xscale, Nf32 yscale, Nf32 zscale );
void			NUT_LUTBuild_NVector3FromFastRandExf( NUT_LUTBUILD_OPERAND_ENUM operand,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, Nf32 xscale, Nf32 yscale, Nf32 zscale, const Nu32 key_every  );

void			NUT_LUTBuild_NVector3FromSplines( NUT_LUTBUILD_OPERAND_ENUM operand,NVEC3f32 *plut,Nu32 element_nb, Nu32 stride, NSPLINE *px_spline, NSPLINE *py_spline, NSPLINE *pz_spline );

void			NUT_LUTBuild_NVector3Clamp( NVEC3f32 *plut,const Nu32 element_nb,const Nu32 stride, const NVEC3f32 *pvnewmin, const NVEC3f32 *pvnewmax );
void			NUT_LUTBuild_NVector3Warp( NVEC3f32 *plut,const Nu32 element_nb,const Nu32 stride, const NVEC3f32 *pvnewmin, const NVEC3f32 *pvnewmax );
void			NUT_LUTBuild_NVector3Smooth( NVEC3f32 *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NCOLOR LUT
void			NUT_LUTBuild_NColorFromConstant( NUT_LUTBUILD_OPERAND_ENUM operand, NCOLOR *plut, Nu32 element_nb, Nu32 stride, NCOLOR *pcolor );
void			NUT_LUTBuild_NColorFromConstantf( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 red,Nf32 green, Nf32 blue, Nf32  alpha );

void			NUT_LUTBuild_NColorFromFastRand( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, NCOLOR *pscale );
void			NUT_LUTBuild_NColorFromFastRandEx( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, NCOLOR *pscale, const Nu32 key_every  );
void			NUT_LUTBuild_NColorFromFastRandf( NUT_LUTBUILD_OPERAND_ENUM operand_fct,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 redscale, Nf32 greenscale, Nf32 bluescale, Nf32 alphascale );
void			NUT_LUTBuild_NColorFromFastRandExf( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 redscale, Nf32 greenscale, Nf32 bluescale, Nf32 alphascale, const Nu32 key_every  );

void			NUT_LUTBuild_NColorFromSplines( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, NSPLINE *pred_spline, NSPLINE *pgreen_spline, NSPLINE *pblue_spline, NSPLINE *palpha_spline );

void			NUT_LUTBuild_NColorClamp( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax );
void			NUT_LUTBuild_NColorWarp( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const NCOLOR *pcnewmin, const NCOLOR *pcnewmax );
void			NUT_LUTBuild_NColorSmooth( NCOLOR *plut,const Nu32 element_nb,const Nu32 stride, const Nu16 range, const Nu16 pass_nb );

// separate component
inline	void	NUT_LUTBuild_NColorConstantRed( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 red ){ NUT_LUTBuild_Nf32FromConstant( operand,&plut->red,element_nb,stride, MAX(0.0f,MIN(red,1.0f)) );}
inline	void	NUT_LUTBuild_NColorConstantGreen( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 green ){ NUT_LUTBuild_Nf32FromConstant( operand,&plut->green,element_nb,stride, MAX(0.0f,MIN(green,1.0f)) );}
inline	void	NUT_LUTBuild_NColorConstantBlue( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 blue ){ NUT_LUTBuild_Nf32FromConstant( operand,&plut->blue,element_nb,stride, MAX(0.0f,MIN(blue,1.0f)) );}
inline	void	NUT_LUTBuild_NColorConstantAlpha( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 alpha ){ NUT_LUTBuild_Nf32FromConstant( operand,&plut->alpha,element_nb,stride, MAX(0.0f,MIN(alpha,1.0f)) );}

inline	void	NUT_LUTBuild_NColorFastRandRed( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 redscale ){ NUT_LUTBuild_Nf32FromFastRand( operand,&plut->red,element_nb,stride, MAX(0.0f,MIN(redscale,1.0f)) );}
inline	void	NUT_LUTBuild_NColorFastRandRedEx( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 redscale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( operand,&plut->red,element_nb,stride, MAX(0.0f,MIN(redscale,1.0f)),key_every );}
inline	void	NUT_LUTBuild_NColorFastRandGreen( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 greenscale ){ NUT_LUTBuild_Nf32FromFastRand( operand,&plut->green,element_nb,stride, MAX(0.0f,MIN(greenscale,1.0f)) );}
inline	void	NUT_LUTBuild_NColorFastRandGreenEx( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 greenscale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( operand,&plut->green,element_nb,stride, MAX(0.0f,MIN(greenscale,1.0f)),key_every );}
inline	void	NUT_LUTBuild_NColorFastRandBlue( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 bluescale ){ NUT_LUTBuild_Nf32FromFastRand( operand,&plut->blue,element_nb,stride, MAX(0.0f,MIN(bluescale,1.0f)) );}
inline	void	NUT_LUTBuild_NColorFastRandBlueEx( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 bluescale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( operand,&plut->blue,element_nb,stride, MAX(0.0f,MIN(bluescale,1.0f)),key_every );}
inline	void	NUT_LUTBuild_NColorFastRandAlpha( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 alphascale ){ NUT_LUTBuild_Nf32FromFastRand( operand,&plut->alpha,element_nb,stride, MAX(0.0f,MIN(alphascale,1.0f)) );}
inline	void	NUT_LUTBuild_NColorFastRandAlphaEx( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride, Nf32 alphascale, const Nu32 key_every ){ NUT_LUTBuild_Nf32FromFastRandEx( operand,&plut->alpha,element_nb,stride, MAX(0.0f,MIN(alphascale,1.0f)),key_every );}

void			NUT_LUTBuild_NColorSplineRed( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride,  NSPLINE *pred_spline );
void			NUT_LUTBuild_NColorSplineGreen( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride,  NSPLINE *pgreen_spline );
void			NUT_LUTBuild_NColorSplineBlue( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride,  NSPLINE *pblue_spline );
void			NUT_LUTBuild_NColorSplineAlpha( NUT_LUTBUILD_OPERAND_ENUM operand,NCOLOR *plut,Nu32 element_nb, Nu32 stride,  NSPLINE *palpha_spline );

*/


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **								Snapping Value										**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// Notes:
// NUT_SNAP.Grid <=0.0f, means no SNAP !
/*

#define NUT_DEFAULT_SNAP_GRID			50.0f	// a snap point every xxx unity
#define NUT_DEFAULT_SNAP_DISTANCE		2.5f	//

typedef struct
{
	Nf32	Grid;
	Nf32	Distance;
}NUT_SNAP;

inline	Nf32	NUT_GetSnapDistance(NUT_SNAP*psnap){return psnap->Distance;}
inline	Nf32	NUT_GetSnapGrid(NUT_SNAP*psnap){return psnap->Grid; }
Nf32			NUT_GetSnapValuef(NUT_SNAP*psnap,Nf32 a);
inline	void	NUT_ResetSnapToDefault(NUT_SNAP*psnap){psnap->Grid = NUT_DEFAULT_SNAP_GRID;psnap->Distance = NUT_DEFAULT_SNAP_DISTANCE;}
inline	Nf32	NUT_SetSnapDistance(NUT_SNAP*psnap,Nf32 a){Nf32 old;old=psnap->Distance;psnap->Distance = a;return old; }
inline	Nf32	NUT_SetSnapGrid(NUT_SNAP*psnap,Nf32 a){Nf32 old;old=psnap->Grid;psnap->Grid = a;return old; }
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **						PRE-DEFINED SPLINE MODEL CREATION							**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
//
//	    .....				   ......						    .						\
//			 ...				     ...						.						 \
//				.					    ..						.						  \
//				.					      .						 .						   \
//				.					       .					  ..						\
//				 ...					   .				        ....					 \
//					.....				   .							.......				  \
//
//		  DECREASE SMOOTH 		DECREASE ACCELERATED 	  DECREASE DECELERATED 			DECREASE LINEAR
//
//					   .....			   ......						  .					  /				  ............
//				    ...				    ...								  .					 /				 .
//				   .				  ..								  .					/				 .
//				   .				 .									 .				   /				.
//				   .				.								   ..				  /					.
//				...					.						       ....					 /					.
//		   .....					.						.......						/					.
//
//		INCREASE SMOOTH 		INCREASE DECELERATED 	   INCREASE ACCELERATED 	INCREASE LINEAR			INCREASE EARLY
//
//
//
//					.....							.							...						   ......
//				  ..	 ..							.							.  ..					...      ...
//			     .		   .						.							.	 .				  ..		    ..
//			    .		    .    				   . .						    .     .				 .				  .
//				.			.					   . .						   .       .			 .				  .
//			   .			 .					  .	  .						  .        .			.				   .
//			...				  ...				..	   ..				   ...          ...			.				   .
//		 ...					 ....		....	     ....		  .....                ...		.				   .
//
//					BELL						   PIC							SHARK FIN					DOME
//
//
//
//
//
//
//
//
//
//
//		 ............................
//
//					FLAT


/*


typedef enum
{
	NUT_SPLINEMODEL_DECREASE_ACCELERATED=0,
	NUT_SPLINEMODEL_DECREASE_DECELERATED,
	NUT_SPLINEMODEL_DECREASE_LINEAR,
	NUT_SPLINEMODEL_DECREASE_SMOOTH,
	NUT_SPLINEMODEL_DECREASE_EARLY,
	NUT_SPLINEMODEL_DECREASE_LATE,

	NUT_SPLINEMODEL_INCREASE_ACCELERATED,
	NUT_SPLINEMODEL_INCREASE_DECELERATED,
	NUT_SPLINEMODEL_INCREASE_LINEAR,
	NUT_SPLINEMODEL_INCREASE_SMOOTH,
	NUT_SPLINEMODEL_INCREASE_EARLY,
	NUT_SPLINEMODEL_INCREASE_LATE,

	NUT_SPLINEMODEL_BELL,
	NUT_SPLINEMODEL_DOME,
	NUT_SPLINEMODEL_FLAT,
	NUT_SPLINEMODEL_PIC,
	NUT_SPLINEMODEL_SHARKFIN,

	//--------------------------------
	NUT_SPLINEMODEL_ENUMSIZE
}NUT_SPLINEMODEL;

NSPLINE *NUT_CreatePlanarSpline(NUT_SPLINEMODEL splinemodel, Nu32 accuracy, Nf32 scalex, Nf32 scaley);


*/



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_H

