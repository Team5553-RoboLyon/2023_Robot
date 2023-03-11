#ifndef __NGEOMETRYVERTEX_H
#define __NGEOMETRYVERTEX_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							GeometryVertex.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec4f32.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "NGeometryTextureCoordinate.h"
#include "lib/N/Geometry/Components/Specifications/NGeometryVertex_Flags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Nomenclature
// V ... Vertex Position
// N ... Normal
// BN... BiNormal
// TN... Tangent
// B ... Bone
// T ... Texture Coordinate
// C ... Color
//
// f (or nothing) ... Nf32 (default)
// b	............. Byte
// s	............. Short
// h	............. Half Float (not yet)

typedef enum
{
	NVERTEX_FORMAT_NULL=0,				
	
	// No Color
	NVERTEX_FORMAT_V3,				//
	NVERTEX_FORMAT_V3C4,			// N Use it for: NUT_DRAW & NUT_COLORCIRCLE
	NVERTEX_FORMAT_V3C4T2,			// N Use it for: NPARTICLEVERTEX_FORMAT, NTEXTVERTEX_FORMAT, NSKINVERTEX_FORMAT, NCOLLECTIBLEVERTEX_FORMAT
	NVERTEX_FORMAT_V3N3C4T2,		//
//	NVERTEX_FORMAT_V3N3C4T2T2,			
//	NVERTEX_FORMAT_V3N3C4C4T2T2,	// N Use it for: NUT_SHAPE as build vertex structure. To be able building Texcoord0 or texcoord1 and color0 or color1.	
//	NVERTEX_FORMAT_V3T2,				
//	NVERTEX_FORMAT_V3N3,				
//	NVERTEX_FORMAT_V3N3C4,			
//	NVERTEX_FORMAT_V3C4T2T2,			
// 	NVERTEX_FORMAT_V3C3,				
// 	NVERTEX_FORMAT_V3N3C3,				
// 	NVERTEX_FORMAT_V3C3T2,					
// 	NVERTEX_FORMAT_V3C3T2T2,			
// 	NVERTEX_FORMAT_V3N3C3T2,				
// 	NVERTEX_FORMAT_V3N3C3T2T2,			
// 	NVERTEX_FORMAT_V3N3C3C3T2T2,		
// 	NVERTEX_FORMAT_V3C4b,				
// 	NVERTEX_FORMAT_V3N3C4b,					
// 	NVERTEX_FORMAT_V3C4bT2,					
// 	NVERTEX_FORMAT_V3C4bT2T2,			
// 	NVERTEX_FORMAT_V3N3C4bT2,				
// 	NVERTEX_FORMAT_V3N3C4bT2T2,				
// 	NVERTEX_FORMAT_V3N3C4bC4bT2T2,		
	// -----------------------
	NVERTEX_FORMAT_ENUM_SIZE		// MAX size is 64!!! (and max FORMATid is 63) This FORMATid is going to be stored on a 6 bits value !!!
}NVERTEX_FORMAT_ENUM;

typedef struct  
{
	NVEC3	Position_3f;
}NVERTEX_V3;
typedef struct  
{
	NVEC3	Position_3f;
	NCOLOR		Color0_4f;
}NVERTEX_V3C4;
typedef struct  
{
	NVEC3		Position_3f;
	NCOLOR			Color0_4f;
	NTEXCOORD_2f	TexCoord0_2f;
}NVERTEX_V3C4T2;
typedef struct  
{
	NVEC3		Position_3f;
	NVEC3		Normal_3f;
	NCOLOR			Color0_4f;
	NTEXCOORD_2f	TexCoord0_2f;
}NVERTEX_V3N3C4T2;

/*
typedef struct  
{
	NVEC3		Position_3f;
	NVEC3		Normal_3f;
	NCOLOR			Color0_4f;
	NCOLOR			Color1_4f;
	NTEXCOORD_2f	TexCoord0_2f;
	NTEXCOORD_2f	TexCoord1_2f;
}NVERTEX_V3N3C4C4T2T2;
*/

/*
typedef struct  
{
	NVEC3	Position_3f;
	NUV			TexCoord0_2f;
}NVERTEX_V3T2;

typedef struct  
{
	NVEC3	Position_3f;
	NVEC3	Normal_3f;
}NVERTEX_V3N3;
typedef struct  
{
	NVEC3	Position_3f;
	NVEC3	Normal_3f;
	NCOLOR		Color0_4f;
}NVERTEX_V3N3C4;


typedef struct  
{
	NVEC3	Position_3f;
	NCOLOR		Color0_4f;
	NUV			TexCoord0_2f;
}NVERTEX_V3C4T2T2;
typedef struct  
{
	NVEC3	Position_3f;
	NVEC3	Normal_3f;
	NCOLOR		Color0_4f;
	NUV			TexCoord0_2f;
	NUV			UV1_2f;
}NVERTEX_V3N3C4T2T2;

... and ...
NVERTEX_V3C3,				
NVERTEX_V3N3C3,				
NVERTEX_V3C3T2,				
NVERTEX_V3C3T2T2,			
NVERTEX_V3N3C3T2,			
NVERTEX_V3N3C3T2T2,			
NVERTEX_V3N3C3C3T2T2,		
NVERTEX_FORMAT_V3C4b,		
NVERTEX_V3N3C4b,			
NVERTEX_V3C4bT2,			
NVERTEX_V3C4bT2T2,			
NVERTEX_V3N3C4bT2,			
NVERTEX_V3N3C4bT2T2,		
NVERTEX_V3N3C4bC4bT2T2,		
*/

// + Fixed Used Vertex Formats
// ( !!! if one changes, don't forget to update its Geometry RenderSettings too !!! )

typedef NVERTEX_V3C4				NUTDRAWVERTEX;		// update NUT_Init() in case of change ! 
#define NUTDRAWVERTEX_FORMAT		NVERTEX_FORMAT_V3C4

typedef NVERTEX_V3C4T2				NPARTICLEVERTEX;
#define NPARTICLEVERTEX_FORMAT		NVERTEX_FORMAT_V3C4T2

typedef NVERTEX_V3C4T2				NTEXTVERTEX;
#define NTEXTVERTEX_FORMAT			NVERTEX_FORMAT_V3C4T2

typedef NVERTEX_V3C4T2				NSKINVERTEX;
#define NSKINVERTEX_FORMAT			NVERTEX_FORMAT_V3C4T2

typedef NVERTEX_V3C4T2				NCOLLECTIBLEVERTEX;
#define NCOLLECTIBLEVERTEX_FORMAT	NVERTEX_FORMAT_V3C4T2

typedef enum
{
	NVERTEX_DATA_POSITION=0,
	NVERTEX_DATA_NORMAL,
	NVERTEX_DATA_BINORMAL,
	NVERTEX_DATA_TANGENT,
	NVERTEX_DATA_COLOR0,
	NVERTEX_DATA_COLOR1,
	NVERTEX_DATA_TEXCOORD0,
	NVERTEX_DATA_TEXCOORD1,
	NVERTEX_DATA_BONE0,
	NVERTEX_DATA_BONE1,
	//---------------------------
	NVERTEX_DATA_ENUM_SIZE
}NVERTEX_DATA_ENUM;

#define NVERTEX_DATA_COLOR_NUMBER_MAX		2	// 2 Colors per Vertex at maximum
#define NVERTEX_DATA_TEXCOORD_NUMBER_MAX	2	// 2 Texture Coordinates per Vertex at maximum
#define NVERTEX_DATA_BONE_NUMBER_MAX		2	// 2 Bones per Vertex at maximum


// MASK_NVERTEX_DATA:	!! Important they are following the same order than the NVERTEX_DATA_ENUM
//						!! And there is one MASK for each NVERTEX_DATA !!!
//						Use these MASKS with function(s): - NCopyVertex
//														  - 	

// 	NVERTEX_SPECS is an union of 2 structures:
// 		NVERTEX_SPECS_DIRECT_ACCESS and NVERTEX_SPECS_TABLE_ACCESS
// 		purpose of these 2 structures is accessing  to the same data in memory in two different ways.
// 		So they need to stay consistent and to reflect the exact same memory area. 
// 		So their sizes are exactly the same ... the Test check that ...
// The 2 structures below represent 2 ways to access to the same Data
typedef struct 
{
	Nu32	Flags;

	// Offsets
	Nu8		OffsetToPosition;
	Nu8		OffsetToNormal;	
	Nu8		OffsetToBiNormal;	
	Nu8		OffsetToTangent;	

	Nu8		OffsetToColor0;		
	Nu8		OffsetToColor1;
	Nu8		OffsetToTexCoord0;
	Nu8		OffsetToTexCoord1;

	Nu8		OffsetToBone0;
	Nu8		OffsetToBone1;
	// SizeOf
	Nu8		SizeOfPosition;
	Nu8		SizeOfNormal;	
	Nu8		SizeOfBiNormal;	
	Nu8		SizeOfTangent;	

	Nu8		SizeOfColor0;		
	Nu8		SizeOfColor1;
	Nu8		SizeOfTexCoord0;
	Nu8		SizeOfTexCoord1;

	Nu8		SizeOfBone0;
	Nu8		SizeOfBone1;

	Nu8		SizeOfVertex; 
	// Nu8		Padding
	// Nu16		Padding;
}NVERTEX_SPECS_DIRECT_ACCESS;

typedef struct 
{
	Nu32	Flags;
	Nu8		Offset[NVERTEX_DATA_ENUM_SIZE];
	Nu8		SizeOf[NVERTEX_DATA_ENUM_SIZE];
	Nu8		SizeOfVertex; 
	// Nu8			Padding
	// Nu16			Padding;
}NVERTEX_SPECS_TABLE_ACCESS;

typedef union
{
	NVERTEX_SPECS_DIRECT_ACCESS		DirectAccess;
	NVERTEX_SPECS_TABLE_ACCESS		TableAccess;
}NVERTEX_SPECS;




// *******************************************************************************************************************************************
// **													Vertex Functions																	**
// *******************************************************************************************************************************************
void NCopyVertex(void *pdst_vertex, const NVERTEX_FORMAT_ENUM dst_vertex_format, const void *psrc_vertex, const NVERTEX_FORMAT_ENUM src_vertex_format);
void NCopyVertexEx(void *pdst_vertex, const NVERTEX_FORMAT_ENUM dst_vertex_format, const void *psrc_vertex, const NVERTEX_FORMAT_ENUM src_vertex_format,const Nu32 vertex_data_mask);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NSetVertex... Functions.
// +
// +	When user knows the vertex data format he may use these functions to set up vertex data.
// +	No conversion will be made !
// +	But there is a check before setup to confirm vertex data format matches with incoming data format.
// +	If not, there is a crash in debug mode. Nothing happen in release mode ( no crash and no setup ).
// +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Set Vertex Position
// +
void NSetVertexPosition_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ppos);
void NSetVertexPosition_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC4 *ppos);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Set Vertex TBN (Tangent, BiTangent,Normal)
// +
void NSetVertexNormal_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pnormal);
//void NSetVertexNormal_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pnormal );

void NSetVertexBiNormal_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pbinormal);
//void NSetVertexBiNormal_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const??? *pbinormal);

void NSetVertexTangent_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ptangent);
//void NSetVertexTangent_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *ptangent);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Set Vertex Colors
// +
// +-+ Parametric ColorSet
void NSetVertexColor_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const NCOLOR *pcolor4f);
void NSetVertexColor_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const Nu32 color4b);
//void NSetVertexColor_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor_3f);
//void NSetVertexColor_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor_3b);
// +
// +-+ VertexColor 0
void NSetVertexColor0_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f);
void NSetVertexColor0_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 color4b);
//void NSetVertexColor0_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3f);
//void NSetVertexColor0_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3b);
// +
// +-+ VertexColor 1
void NSetVertexColor1_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f);
void NSetVertexColor1_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 color4b);
//void NSetVertexColor1_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3f)
//void NSetVertexColor1_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3b)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Set Vertex Texture Coordinates
// +
// +-+ Parametric TexCoordSet
void NSetVertexTexCoord_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2f *ptexcoord_2f);
void NSetVertexTexCoord_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3f *ptexcoord_3f);
// void NSetVertexTexCoord_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2s *ptexcoord_2s);
// void NSetVertexTexCoord_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3s *ptexcoord_3s);

// +
// +-+ TexCoord 0
void NSetVertexTexCoord0_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f);
void NSetVertexTexCoord0_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f);
// void NSetVertexTexCoord0_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s);
// void NSetVertexTexCoord0_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s);

// +
// +-+ TexCoord 1
void NSetVertexTexCoord1_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f);
void NSetVertexTexCoord1_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f);
// void NSetVertexTexCoord1_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s);
// void NSetVertexTexCoord1_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Set Vertex Bones
// +
// +-+ Parametric Bone Id
void NSetVertexBone_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const NBYTE bone_1b);
// void NSetVertexBone_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2b);
// void NSetVertexBone_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2s);
// void NSetVertexBone_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_1d1f);
// +
// +-+ Bone 0
void NSetVertexBone0_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b);
// void NSetVertexBone0_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b);
// void NSetVertexBone0_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s);
// void NSetVertexBone0_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbone_1d1f);
// +
// +-+ Bone 1
void NSetVertexBone1_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b);
// void NSetVertexBone1_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b);
// void NSetVertexBone1_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s);
// void NSetVertexBone1_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbone_1d1f);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NConvertVertex... Functions.
// +
// +	When user doesn't know the vertex data format he may use these functions to convert incoming data
// +	into vertex data format.
// +	Purpose of these functions is to make a CONVERSION from a specific data format to the vertex data format.
// +
// + Convert Vertex Position
// +
void NConvertVertexPosition_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC4 *ppos);
void NConvertVertexPosition_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ppos);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Convert Vertex TBN (Tangent, BiTangent,Normal)
// +
void NConvertVertexNormal_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pnormal);
void NConvertVertexBiNormal_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pbinormal);
void NConvertVertexTangent_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ptangent);
// void NConvertVertexNormal_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pnormal);
// void NConvertVertexBiNormal_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbinormal);
// void NConvertVertexTangent_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *ptangent);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Convert Vertex Colors
// +
// +-+ Parametric ColorSet
void NConvertVertexColor_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const NCOLOR *pcolor4f);
void NConvertVertexColor_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const Nu32 *pcolor4b);
// void NConvertVertexColor_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor3f);
// void NConvertVertexColor_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor3b);
// +
// +-+ VertexColor 0
void NConvertVertexColor0_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f);
void NConvertVertexColor0_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 *pcolor4b);
// void NConvertVertexColor0_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3f);
// void NConvertVertexColor0_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3b);
// +
// +-+ VertexColor 1
void NConvertVertexColor1_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f);
void NConvertVertexColor1_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 *pcolor4b);
// void NConvertVertexColor1_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3f);
// void NConvertVertexColor1_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3b);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Convert Vertex Texture Coordinates
// +
// +-+ Parametric TexCoordSet
void NConvertVertexTexCoord_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2f *ptexcoord_2f);
void NConvertVertexTexCoord_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3f *ptexcoord_3f);
// void NConvertVertexTexCoord_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2s *ptexcoord_2s);
// void NConvertVertexTexCoord_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3s *ptexcoord_3s);
// +
// +-+ TexCoord 0
void NConvertVertexTexCoord0_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f);
void NConvertVertexTexCoord0_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f);
// void NConvertVertexTexCoord0_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s);
// void NConvertVertexTexCoord0_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s);
// +
// +-+ TexCoord 1
void NConvertVertexTexCoord1_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f);
void NConvertVertexTexCoord1_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f);
// void NConvertVertexTexCoord1_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s);
// void NConvertVertexTexCoord1_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Convert Vertex Bones
// +
// +-+ Parametric Bone Id
void NConvertVertexBone_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const NBYTE bone_1b);
// void NConvertVertexBone_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2b);
// void NConvertVertexBone_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2s);
// void NConvertVertexBone_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_1d1f);
// +
// +-+ Bone 0
void NConvertVertexBone0_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b);
// void NConvertVertexBone0_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b);
// void NConvertVertexBone0_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s);
// void NConvertVertexBone0_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_1d1f);
// +
// +-+ Bone 1
void NConvertVertexBone1_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b);
// void NConvertVertexBone1_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b);
// void NConvertVertexBone1_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s);
// void NConvertVertexBone1_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_1d1f);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NGetVertex... Functions.
// +
// +	When user knows the vertex data format he may use these functions to retrieve vertex data.
// +	No conversion will be made !
// +	But there is a check before get, to confirm vertex data format matches with outcoming data format.
// +	If not, there is a crash in debug mode. Nothing happen in release mode ( no crash and no Get ).
// +
void NGetVertexPosition_3f(NVEC3 *ppos, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
void NGetVertexPosition_4f(NVEC4 *ppos, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
void NGetVertexNormal_3f(NVEC3 *pnormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
// void NGetVertexNormal_3s( ??? *pnormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
void NGetVertexBiNormal_3f( NVEC3 *pbinormal ,const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
// void NGetVertexBiNormal_3s(??? *pbinormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
void NGetVertexTangent_3f( NVEC3 *ptangent, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
// void NGetVertexTangent_3s(??? *ptangent, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
// void NGetVertexColor0_3f(??? *pcolor_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexColor0_3b(??? *pcolor_3b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexColor0_4f(NCOLOR *pcolor4f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexColor0_4b(Nu32 *pcolor4b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
//void NGetVertexColor1_3f(??? *pcolor_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
//void NGetVertexColor1_3b(??? *pcolor_3b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexColor1_4f(NCOLOR *pcolor4f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexColor1_4b(Nu32 *pcolor4b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexTexCoord0_2f(NVEC2 *ptexcoord_2f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexTexCoord0_3f(NVEC3 *ptexcoord_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexTexCoord1_2f(NVEC2 *ptexcoord_2f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexTexCoord1_3f(NVEC3 *ptexcoord_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexBone0_1d1f(??? *pbone_1d1f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexBone1_1d1f(??? *pbone_1d1f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexBone0_1b(NBYTE *pbone_1b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
void NGetVertexBone1_1b(NBYTE *pbone_1b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexBone0_2b(??? *pbone_2b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexBone1_2b( ??? *pbone_2b, void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format );
// void NGetVertexBone0_2s(??? *pbone_2s, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);
// void NGetVertexBone1_2s(??? *pbone_2s, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format);


// ***************************************************************************************
// **					Vertex "Specs" Functions										**
// ***************************************************************************************
Nu8				NGetSizeOfVertex(const NVERTEX_FORMAT_ENUM nvertex_format);
Nu8				NGetSizeOfVertexData(const NVERTEX_FORMAT_ENUM nvertex_format, const NVERTEX_DATA_ENUM nvertex_data);
NVERTEX_SPECS*	NGetVertexSpecs(const NVERTEX_FORMAT_ENUM nvertex_format);
Nu32			NGetVertexSpecsFlags(const NVERTEX_FORMAT_ENUM nvertex_format);

// ***************************************************************************************
// **					Vertex "PRIVATE" Functions										**
// ***************************************************************************************
void			NInitializeVertexSpecs();
void			NDisableVertexSpecs();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYVERTEX_H 

