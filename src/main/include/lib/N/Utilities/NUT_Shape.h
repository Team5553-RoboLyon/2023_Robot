#ifndef __NUT_SHAPE_H
#define __NUT_SHAPE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 NUT_Shape.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NRectf32.h"
#include "lib/N/NSpline.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/Geometry/Components/NGeometryTextureCoordinate.h"
#include "lib/N/Miscellaneous/NColorGradient.h"
#include "lib/N/Miscellaneous/NVertexTexCoordBuild.h"
#include "lib/N/Miscellaneous/NVertexColorBuild.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************

// ... _NUTSCD_ ... means 'NUT SHAPE CORE DESC'

//	NUT Geometry Prime default values.
#define DEFAULT_NUTSCD_SIZE_WIDTH 0.1f
#define DEFAULT_NUTSCD_SIZE_HEIGHT 0.1f

//	NUT SHAPE Core Flags.
//		From BIT_0 to BIT_15 included. Reserved for NUT_SHAPE_DESC it self
//		From BIT_16 to BIT_31 included. Used by each specific NUT_SHAPE to build. ( NUT_QUAD_DESC, NUT_HOLLOW_QUAD_DESC, ... )
#define FLAG_NUTSCD_TRIANGLE_CW_BUILDING BIT_0
#define FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE BIT_1
#define FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR BIT_2
#define FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN BIT_3
#define FLAG_NUTSCD_USEPARAM_SIZE BIT_4
#define FLAG_NUTSCD_USEPARAM_ANCHOR BIT_5
#define FLAG_NUTSCD_USEPARAM_SCALE BIT_6
	// #define FLAG_NUTSCD_xxx												BIT_7
	// #define FLAG_NUTSCD_xxx												BIT_8
	// #define FLAG_NUTSCD_xxx												BIT_9
	// #define FLAG_NUTSCD_xxx												BIT_10
	// #define FLAG_NUTSCD_xxx												BIT_11
	// #define FLAG_NUTSCD_xxx												BIT_12
	// #define FLAG_NUTSCD_xxx												BIT_13
	// #define FLAG_NUTSCD_xxx												BIT_14
	// #define FLAG_NUTSCD_xxx												BIT_15
	//  Next flags (BIT_16 to BIT_31 included) )reserved for specific use.
	//  --------------------------------------------------------------------------------
	typedef struct
	{
		Nu32 Flags;
		// Mesh Management
		NAAPLANE_ENUM Construction_Plane;
		NVEC3 ConstructionPlane_DirectorVector;
		NVEC3 ConstructionOrigin;
		NVEC2 Size;
		NVEC3 Anchor;
		NVEC2 Scale;

		// Texture Coordinates and Colors Building
		NBUILDBASE_TEXCOORD TexCoordBuild;
		NBUILDBASE_COLOR ColorBuild;
	} NUT_SHAPE_DESC;

	// A Quad Description.
	// Used to insert a Quad with Vertex position,UVs,Colors,Normals ... and triangles into a geometry
	typedef struct
	{
		NUT_SHAPE_DESC CoreDesc;
	} NUT_QUAD_DESC;

	// An Hollow Quad Description.
	// Used to insert an Hollow Quad ( A quad with an hole ) with Vertex position,UVs,Colors,Normals ... and triangles into a geometry
	typedef struct
	{
		NUT_SHAPE_DESC CoreDesc;
		NVEC2 SmallSizeRatio; // SmallSize = Size*SmallSizeRatio;
	} NUT_HOLLOW_QUAD_DESC;
// Hollow Quad Default values
#define DEFAULT_HOLLOW_QUAD_DESC_SMALLSIZERATIO_WIDTH 0.5f
#define DEFAULT_HOLLOW_QUAD_DESC_SMALLSIZERATIO_HEIGHT 0.5f
// Specific Hollow Quad Description FLAGS
#define FLAG_HOLLOW_QUAD_DESC_FACEMAP BIT_16				 // "face" means "quad" and the entire texture will be
#define FLAG_HOLLOW_QUAD_DESC_USEPARAM_SMALLSIZERATIO BIT_17 // if not, DEFAULT_HOLLOW_QUAD_DESC_SMALLSIZERATIO... will be used

// A SURFACE Description.
// Used to insert a Plane with Vertex position,UVs,Colors,Normals ... and triangles into a geometry
// Specific Plane Desc Default Values
#define DEFAULT_SURFACE_DESC_XSEGMENTS 1
#define DEFAULT_SURFACE_DESC_YSEGMENTS 1
// Specific SURFACE Description FLAGS
#define FLAG_SURFACE_DESC_USEPARAMS_SEGMENTS BIT_16
	typedef struct
	{
		NUT_SHAPE_DESC CoreDesc;
		Nu16 XSegments;
		Nu16 YSegments;
	} NUT_SURFACE_DESC;

// A Disc Description. ( Used to insert a Disc with Vertex,UVs,Colors and triangles into a geometry )
// Specific Disc Description FLAGS
#define FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES BIT_16
#define FLAG_DISC_DESC_VERTEX_TRIGO_INVERSE_BUILDING BIT_17 // By default vertex are created into trigonometric direction
#define FLAG_DISC_DESC_MIRRORING_U BIT_18
#define FLAG_DISC_DESC_MIRRORING_V BIT_19
#define FLAG_DISC_DESC_USEPARAM_COLORCENTER BIT_20
	typedef struct
	{
		NUT_SHAPE_DESC CoreDesc;

		Nf32 OpenDisc_FromAngle;
		Nf32 OpenDisc_ToAngle;
		Nu32 SliceNb;
		NCOLOR *pColorCenter;

	} NUT_DISC_DESC;

// A Disc Description.
// Used to insert a Disc with Vertex,UVs,Colors and triangles into a geometry
// Ring Default values
#define DEFAULT_RING_DESC_SMALLSIZERATIO_WIDTH 0.5f
#define DEFAULT_RING_DESC_SMALLSIZERATIO_HEIGHT 0.5f
#define DEFAULT_RING_DESC_FACEMAP_SUBSLICENB 1
// Specific Ring Description FLAGS
#define FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES BIT_16
#define FLAG_RING_DESC_VERTEX_TRIGO_INVERSE_BUILDING BIT_17 // By default vertex are created into trigonometric direction
#define FLAG_RING_DESC_MIRRORING_U BIT_18
#define FLAG_RING_DESC_MIRRORING_V BIT_19
#define FLAG_RING_DESC_FACEMAP BIT_20 // "face" means "quad" and the entire texture will be
#define FLAG_RING_DESC_USEPARAM_FACEMAP_SUBSLICENB BIT_21
#define FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO BIT_22 // if not, DEFAULT_RINGDESC_SMALLSIZERATIO... will be used

	typedef struct
	{
		NUT_SHAPE_DESC CoreDesc;

		Nu32 SliceNb;			// equal to the circumference number of vertex
		Nu32 FaceMapSubSliceNb; // Only if FaceMap is enable. Number of ring slices mapped with the same texture width.
		Nf32 OpenRing_FromAngle;
		Nf32 OpenRing_ToAngle;
		NVEC2 SmallSizeRatio; // SmallSize = Size*SmallSizeRatio;
	} NUT_RING_DESC;

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **																			FUNCTIONS																		**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	inline Nu32 NUT_GetDiscVertexNumber(const Nu32 disc_desc_slicenb, const Nbool disc_is_open) { return (1 + disc_desc_slicenb + (Nu32)disc_is_open); }
	inline Nu32 NUT_GetDiscTriangleNumber(const Nu32 disc_desc_slicenb) { return disc_desc_slicenb; }
	inline Nu32 NUT_GetRingVertexNumber(const Nu32 ring_desc_slicenb, const Nbool ring_is_open) { return (2 * (ring_desc_slicenb + (Nu32)ring_is_open)); }
	inline Nu32 NUT_GetRingTriangleNumber(const Nu32 ring_desc_slicenb) { return (2 * ring_desc_slicenb); }

	// Insert a shape into a geometry
	void NUT_InsertDisc(NGEOMETRY *pgeom, const NUT_DISC_DESC *desc);
	void NUT_InsertQuad(NGEOMETRY *pgeom, const NUT_QUAD_DESC *desc);			   // build with 2 triangles.
	void NUT_InsertHollowQuad(NGEOMETRY *pgeom, const NUT_HOLLOW_QUAD_DESC *desc); // build with 8 triangles.
	void NUT_InsertRing(NGEOMETRY *pgeom, const NUT_RING_DESC *desc);
	void NUT_InsertSurface(NGEOMETRY *pgeom, const NUT_SURFACE_DESC *desc);

/*
// LOW LEVEL MACROS ( Private use only )
//  To perform a right conversions from:	FLAG_NUTSCD_... to FLAG_NVCOLBUILD_...
//											FLAG_NUTSCD_... to FLAG_NTXCBUILD_...
// ... and use the result as flags into 'NVertexColorBuild' and 'NVertexTexCoordBuild' Low Level functions ...
#define	CONVERT_FLAGS_NUTSCD_NTXCBUILD_TO_FLAGS_NTXCBUILD(flags_nuigd)		((flags_nuigd)&0x1F)
#define	CONVERT_FLAGS_NUTSCD_NVCOLBUILD_TO_FLAGS_NVCOLBUILD(flags_nuigd)	(((flags_nuigd)>>5)&0x3)
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_SHAPE_H
