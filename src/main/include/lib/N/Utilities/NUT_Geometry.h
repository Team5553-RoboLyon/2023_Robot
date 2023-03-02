#ifndef __NUT_GEOMETRY_H
#define __NUT_GEOMETRY_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Geometry.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../Miscellaneous/NVertexTexCoordBuild.h"

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

#define DEFAULT_NUT_TEXTCOORD_BUILDER_TEXCOORDSET_ID 0

// Geometry Texture Coordinates Building
#define DEFAULT_NUT_TEXTCOORD_BUILDER_QUADQUALITY_THRESHOLD 0.5f // That means
#define DEFAULT_NUT_TEXTCOORD_BUILDER_COPLANARITY_THRESHOLD 0.7f // That means an angle close to 45� between normals

#define DEFAULT_NUT_TEXTCOORD_BUILDER_TILING_3f 1.0f, 1.0f, 1.0f
#define DEFAULT_NUT_TEXTCOORD_BUILDER_OFFSET_3f 0.0f, 0.0f, 0.0f
// #define DEFAULT_NUT_TEXTCOORD_BUILDER_INFLATE_3f								0.0f,0.0f,0.0f

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + FLAG_NUT_TEXTCOORD_BUILDER_** All the FLAGS from BIT_0 to BIT_7 are common to all the
// +
// +-+ To convert 'FLAG_NUT_TEXTCOORD_BUILDER_FLIP_TEXCOORD_x' into 'FLAG_FLIP_NTEXCOORD_xx'
// #define NEXTRACT_NUT_TEXCOORD_BUILDER_FLAGS_FLIP_NTEXCOORD(flags_nutgcd)		( ((flags_nutgcd)>>5)&0x7 )
// +-+ Common FLAGS for all MODEs
#define FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U BIT_0
#define FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V BIT_1
#define FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W BIT_2
// +-+ Specifics FLAGS for MODE FACE MAP
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_QUADQUALITY_THRESHOLD BIT_3		//|
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_COPLANARITY_THRESHOLD BIT_4		//|- These ones are going to be used to create XRPOLY which are necessary to perform Face Mapping in the right way.
#define FLAG_NUT_TEXTCOORD_BUILDER_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2 BIT_5 //|
// +-+ Specifics FLAGS for MODE PLANAR
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_PROJECTIONPLANE BIT_6
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_PROJECTIONPLANE_DIRECTORVECTOR BIT_7
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_OFFSET BIT_8
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TILING BIT_9

#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCAABB BIT_10	 //| !TXC AABB and (TXCSize + TXCCenter) are UNION.
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE BIT_11	 //| So you can have TXC AABB or (TXCSize + TXCCenter)
#define FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER BIT_12 //|

#define FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X BIT_13 //| FIT_X,FIT_Y and FIT_Z work only if TXCSize or TXCAABB is used.
#define FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y BIT_14 //| Only one of the 3 may be set to ON. Their purpose is to adjust the TxC volume
#define FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z BIT_15 //| to the geometry Volume (AABB in the projection Base) and keeping the TxC Volume proportion. So only one dimension of it may exactly FIT the geometry volume.

#define FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_X BIT_16 //| TxC Volume and Geometry Volume (AABB in the projection Base) are suppose to be align and their respective center are suppose to be at the same place.
#define FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Y BIT_17 //| BUT, using TxCAABB or TxCCenter, force engine to use a specific TxC Center. It's possible to keep the TxC Volume center position align with the geometry Volume center,
#define FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Z BIT_18 //| dimension by dimension by setting to ON  1, 2 or 3 of these FLAGS.

#define MASK_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_XYZ (FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X | FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y | FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z)
#define MASK_NUT_TEXTCOORD_BUILDER_TXCSIZE_ALIGN_XYZ (FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_X | FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Y | FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Z)

#define MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW (FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U | FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V | FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W)

	typedef enum
	{
		NUT_TEXCOORD_BUILD_MODE_FACEMAP = 0,
		NUT_TEXCOORD_BUILD_MODE_PLANAR,
		//-------------------------------
		NUT_TEXCOORD_BUILD_MODE_ENUM_SIZE
	} NUT_TEXCOORD_BUILD_MODE_ENUM;

	typedef struct
	{
		NUT_TEXCOORD_BUILD_MODE_ENUM Mode;
		Nu32 Flags;
		NBUILDBASE_TEXCOORD TexCoordBuild;
	} NUT_TEXCOORD_BUILDER_COMMON;

	typedef struct
	{
		NUT_TEXCOORD_BUILD_MODE_ENUM Mode;
		Nu32 Flags;
		NBUILDBASE_TEXCOORD TexCoordBuild;

		// Application Scope
		Nu32 FirstVertexIndex;
		Nu32 VertexNumber;
		Nu32 FirstTriangleIndex;
		Nu32 TriangleNumber;

		// Face Map: params for Quad Detection ...
		Nf32 QuadQualityThreshold;
		Nf32 CoplanarityThreshold;
	} NUT_TEXCOORD_BUILDER_FACEMAP;

	typedef struct
	{
		NUT_TEXCOORD_BUILD_MODE_ENUM Mode;
		Nu32 Flags;
		NBUILDBASE_TEXCOORD TexCoordBuild;

		// Application Scope
		Nu32 FirstVertexIndex;
		Nu32 VertexNumber;
		Nu32 FirstTriangleIndex;
		Nu32 TriangleNumber;

		// Planar Projection Params
		NAAPLANE_ENUM ProjectionPlane;
		NVEC3 ProjectionPlane_DirectorVector;
		NVEC3 Offset;
		NVEC3 Tiling;

		union
		{
			NAABB TxCAABB; // TxC Volume ( into the projected geometry space )
			struct
			{
				NVEC3 TxCSize;	 // Size of the TxC Volume
				NVEC3 TxCCenter; // Center Position of the TxC Volume
			};
		};
	} NUT_TEXCOORD_BUILDER_PLANAR;

	typedef struct
	{
		NUT_TEXCOORD_BUILD_MODE_ENUM Mode;
		Nu32 Flags;
		Nf32 BlendFactor; // 0 to 1
		NBUILDBASE_TEXCOORD TexCoordBuild;

		// Application Scope
		Nu32 FirstVertexIndex;
		Nu32 VertexNumber;
		Nu32 NextVertexIndexStride;

		// Range Params
		NTEXCOORD_3f FirstTxC;
		NTEXCOORD_3f NextTxCShifting;
	} NUT_TEXCOORD_BUILDER_RANGE;

	typedef union
	{
		NUT_TEXCOORD_BUILDER_COMMON Common;
		NUT_TEXCOORD_BUILDER_FACEMAP FaceMap;
		NUT_TEXCOORD_BUILDER_PLANAR Planar;
	} NUT_TEXCOORD_BUILDER;

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **																			FUNCTIONS																		**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	void NUT_CloneGeometryContent(NGEOMETRY *pgeom, const NVEC3 *pvspacing, const Nu32 along_xaxis, const Nu32 along_yaxis, const Nu32 along_zaxis);

	void NUT_BuildGeometryVertexTexCoords(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER *pbuilder);
	void NUT_BuildGeometryVertexTexCoords_Planar(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER_PLANAR *pbuilder);
	void NUT_BuildGeometryVertexTexCoords_FaceMap(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER_FACEMAP *pbuilder);

// void	NUT_SetGeometryVertexTexCoords(NGEOMETRY *pgeom, const Nu32 vertex_index, const NTEXCOORD_3f *ptxc,NBUILDBASE_TEXCOORD *pbbase );
// void	NUT_SetGeometryVertexTexCoords_Range(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number,const Nu32 stride_to_next_vertex_index, const NTEXCOORD_3f *pfirsttxc, const NTEXCOORD_3f *pnexttxc_shifting, NBUILDBASE_TEXCOORD *pbbase );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_H
