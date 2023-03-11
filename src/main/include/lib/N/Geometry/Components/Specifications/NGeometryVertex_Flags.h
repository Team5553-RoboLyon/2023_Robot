#ifndef __NGEOMETRY_VERTEX_FLAGS_H_
#define __NGEOMETRY_VERTEX_FLAGS_H_


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NGeometryVertex_Flags.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NFlags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FLAG_NVERTEX_SPECS_xx are simple flags to describe NVERTEX_FORMAT content. 
// These FLAGS are combined with some others to create NVERTEX_DESC_xx DESCRIPTION values
// BIT_0 to BIT_9 to describe which VERTEX DATA is present.
#define FLAG_NVERTEX_SPECS_POSITION				BIT_0	// ON: Vertex Position	 OFF: No vertex Position	( BIT_id = NVERTEX_DATA_POSITION	)
#define FLAG_NVERTEX_SPECS_NORMAL				BIT_1	// ON: Vertex Normal	 OFF: No vertex Normal		( BIT_id = NVERTEX_DATA_NORMAL		)	
#define FLAG_NVERTEX_SPECS_BINORMAL				BIT_2	// ON: Vertex BiNormal	 OFF: No vertex BiNormal	( BIT_id = NVERTEX_DATA_BINORMAL	)
#define FLAG_NVERTEX_SPECS_TANGENT				BIT_3	// ON: Vertex Tangent	 OFF: No vertex Tangent		( BIT_id = NVERTEX_DATA_TANGENT		)
#define FLAG_NVERTEX_SPECS_COLOR0				BIT_4	// ON: Vertex Color0	 OFF: No vertex Color0		( BIT_id = NVERTEX_DATA_COLOR0		)
#define FLAG_NVERTEX_SPECS_COLOR1				BIT_5	// ON: Vertex Color1	 OFF: No vertex Color1		( BIT_id = NVERTEX_DATA_COLOR1		)
#define FLAG_NVERTEX_SPECS_TEXCOORD0			BIT_6	// ON: Vertex TexCoord0	 OFF: No vertex TexCoord0	( BIT_id = NVERTEX_DATA_TEXCOORD0	)
#define FLAG_NVERTEX_SPECS_TEXCOORD1			BIT_7	// ON: Vertex TexCoord1	 OFF: No vertex TexCoord1	( BIT_id = NVERTEX_DATA_TEXCOORD1	)
#define FLAG_NVERTEX_SPECS_BONE0				BIT_8	// ON: Vertex Bone0		 OFF: No vertex Bone0		( BIT_id = NVERTEX_DATA_BONE0		)
#define FLAG_NVERTEX_SPECS_BONE1				BIT_9	// ON: Vertex Bone1		 OFF: No vertex Bone1		( BIT_id = NVERTEX_DATA_BONE1		)
//
// BIT_10 to BIT_22 Available but for private use of NVERTEX_SPECS ONLY ! (To add some new vertex components/infos)
//
// BIT_23 to BIT_31 to describe each data types (included data, of course ...)
#define FLAG_NVERTEX_SPECS_POSITION_3f			BIT_23	// ON: Vertex Position has 3 NF32.			OFF (default): 4 Nf32
#define FLAG_NVERTEX_SPECS_TBN_3f				BIT_24	// ON: Vertex TBN have 3 NF32.				OFF (default): 3 Ns16 (signed short)
#define FLAG_NVERTEX_SPECS_COLORS_3				BIT_25	// ON: Vertex Colors have 3 components.		OFF (default): 4 components
#define FLAG_NVERTEX_SPECS_COLORS_f				BIT_26	// ON: Vertex Colors components are Nf32.	OFF (default): NByte
#define FLAG_NVERTEX_SPECS_TEXCOORDS_2			BIT_27	// ON: Vertex TexCoords have 2 components.	OFF (default): 3 components  
#define FLAG_NVERTEX_SPECS_TEXCOORDS_f			BIT_28	// ON: Vertex Colors components are Nf32.	OFF (default): short (Ns16 ? or Nu 16 ? ... TBD) 
#define FLAG_NVERTEX_SPECS_BONE_1b				BIT_29	// ON: unweighted Bone(s), with only a bone index coded with 1 Byte		OFF (default): |
#define FLAG_NVERTEX_SPECS_BONE_2b				BIT_30	// ON: for weighted Bone(s), with 1 NBbyte index + 1 NByte weight		OFF (default): |- 1d1f Weighted Bone(s) with 1 Nu32 index + 1 Nf32 Weight 
#define FLAG_NVERTEX_SPECS_BONE_2s				BIT_31	// ON: weighted Bone(s), with 1 Short index + 1 Short weight			OFF (default): |  ( with all the 3 BITs to OFF )	


// NVERTEX_DESCMASK_xx to isolate descriptions and perform right tests (are compositions from FLAG_NVERTEX_DESC_xx too ... ).
#define MASK_NVERTEX_SPECS_DATA					(0x3FF)			// BIT_0 to BIT_9 included ..... 
#define MASK_NVERTEX_SPECS_DATA_TYPES			(0xFF800000)	// BIT_23 to BIT_31 included ... 

#define MASK_NVERTEX_SPECS_POSITION				(FLAG_NVERTEX_SPECS_POSITION|FLAG_NVERTEX_SPECS_POSITION_3f)
#define MASK_NVERTEX_SPECS_POSITION_TYPES		(FLAG_NVERTEX_SPECS_POSITION_3f)

#define MASK_NVERTEX_SPECS_NORMAL				(FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_TBN_3f)
#define MASK_NVERTEX_SPECS_BINORMAL				(FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TBN_3f)
#define MASK_NVERTEX_SPECS_TANGENT				(FLAG_NVERTEX_SPECS_TANGENT|FLAG_NVERTEX_SPECS_TBN_3f)
#define MASK_NVERTEX_SPECS_TBN					(FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TANGENT|FLAG_NVERTEX_SPECS_TBN_3f)
#define MASK_NVERTEX_SPECS_TBN_TYPES			(FLAG_NVERTEX_SPECS_TBN_3f)

#define MASK_NVERTEX_SPECS_COLOR0				(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define MASK_NVERTEX_SPECS_COLOR1				(FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define MASK_NVERTEX_SPECS_COLORS				(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define MASK_NVERTEX_SPECS_COLORS_TYPES			(FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)

#define MASK_NVERTEX_SPECS_TEXCOORD0			(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define MASK_NVERTEX_SPECS_TEXCOORD1			(FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define MASK_NVERTEX_SPECS_TEXCOORDS			(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define MASK_NVERTEX_SPECS_TEXCOORDS_TYPES		(FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)

#define MASK_NVERTEX_SPECS_BONE0				(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE_1b|FLAG_NVERTEX_SPECS_BONE_2b|FLAG_NVERTEX_SPECS_BONE_2s)
#define MASK_NVERTEX_SPECS_BONE1				(FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_1b|FLAG_NVERTEX_SPECS_BONE_2b|FLAG_NVERTEX_SPECS_BONE_2s)
#define MASK_NVERTEX_SPECS_BONES				(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_1b|FLAG_NVERTEX_SPECS_BONE_2b|FLAG_NVERTEX_SPECS_BONE_2s)
#define MASK_NVERTEX_SPECS_BONES_TYPES			(FLAG_NVERTEX_SPECS_BONE_1b|FLAG_NVERTEX_SPECS_BONE_2b|FLAG_NVERTEX_SPECS_BONE_2s)


// FLAGS_NVERTEX_SPECS_xx are compositions from FLAG_NVERTEX_SPECS_xx.
// This first list contains the all the possibilities for each Vertex component (Position, Normal, color, texture coordinates ... )
#define FLAGS_NVERTEX_SPECS_NULL		0		
#define FLAGS_NVERTEX_SPECS_V3			(FLAG_NVERTEX_SPECS_POSITION|FLAG_NVERTEX_SPECS_POSITION_3f)
#define FLAGS_NVERTEX_SPECS_V4			(FLAG_NVERTEX_SPECS_POSITION)

#define FLAGS_NVERTEX_SPECS_N3			(FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_TBN_3f)
#define FLAGS_NVERTEX_SPECS_N3s			(FLAG_NVERTEX_SPECS_NORMAL)	
#define FLAGS_NVERTEX_SPECS_BI3			(FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TBN_3f)
#define FLAGS_NVERTEX_SPECS_BI3s		(FLAG_NVERTEX_SPECS_BINORMAL)
#define FLAGS_NVERTEX_SPECS_TG3			(FLAG_NVERTEX_SPECS_TANGENT|FLAG_NVERTEX_SPECS_TBN_3f)
#define FLAGS_NVERTEX_SPECS_TG3s		(FLAG_NVERTEX_SPECS_TANGENT)
#define FLAGS_NVERTEX_SPECS_TBN3		(FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TANGENT|FLAG_NVERTEX_SPECS_TBN_3f)
#define FLAGS_NVERTEX_SPECS_TBN3s		(FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TANGENT)

#define FLAGS_NVERTEX_SPECS_C4			(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLORS_f)
#define FLAGS_NVERTEX_SPECS_C3			(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define FLAGS_NVERTEX_SPECS_C4b			(FLAG_NVERTEX_SPECS_COLOR0)
#define FLAGS_NVERTEX_SPECS_C3b			(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLORS_3)
#define FLAGS_NVERTEX_SPECS_C4C4		(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_f)
#define FLAGS_NVERTEX_SPECS_C3C3		(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define FLAGS_NVERTEX_SPECS_C4bC4b		(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1)	
#define FLAGS_NVERTEX_SPECS_C3bC3b		(FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3)	

#define FLAGS_NVERTEX_SPECS_T2			(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define FLAGS_NVERTEX_SPECS_T3			(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define FLAGS_NVERTEX_SPECS_T2s			(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORDS_2)
#define FLAGS_NVERTEX_SPECS_T3s			(FLAG_NVERTEX_SPECS_TEXCOORD0)
#define FLAGS_NVERTEX_SPECS_T2T2		(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define FLAGS_NVERTEX_SPECS_T3T3		(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define FLAGS_NVERTEX_SPECS_T2sT2s		(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2)
#define FLAGS_NVERTEX_SPECS_T3sT3s		(FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1)

#define FLAGS_NVERTEX_SPECS_B1d1f		(FLAG_NVERTEX_SPECS_BONE0)
#define FLAGS_NVERTEX_SPECS_B1b			(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE_1b)
#define FLAGS_NVERTEX_SPECS_B2b			(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE_2b)
#define FLAGS_NVERTEX_SPECS_B2s			(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE_2s)
#define FLAGS_NVERTEX_SPECS_B1d1fB1d1f	(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1)
#define FLAGS_NVERTEX_SPECS_B1bB1b		(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_1b)
#define FLAGS_NVERTEX_SPECS_B2bB2b		(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_2b)
#define FLAGS_NVERTEX_SPECS_B2sB2s		(FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_2s)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + VERTEX SPECS
// +
// +---+ NOMENCLATURE
// +
// +		V ... Vertex Position
// +		N ... Normal
// +		BN... BiNormal
// +		TN... Tangent
// +		B ... Bone
// +		T ... Texture Coordinate
// +		C ... Color
// +
// +		f (or nothing) ... Nf32 (default)
// +		b	............. Byte
// +		s	............. Short
// +		h	............. Half Float (not yet)
// +
#define FLAGS_NVERTEX_SPECS_V3T2			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3)
#define FLAGS_NVERTEX_SPECS_V3C4			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C4)
#define FLAGS_NVERTEX_SPECS_V3N3C4			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4)
#define FLAGS_NVERTEX_SPECS_V3C4T2			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C4|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4T2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4T2T2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4|FLAGS_NVERTEX_SPECS_T2T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4C4T2T2	(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4C4|FLAGS_NVERTEX_SPECS_T2T2)

#define FLAGS_NVERTEX_SPECS_V3C3			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C3)
#define FLAGS_NVERTEX_SPECS_V3N3C3			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C3)
#define FLAGS_NVERTEX_SPECS_V3C3T2			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C3|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C3T2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C3|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C3T2T2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C3|FLAGS_NVERTEX_SPECS_T2T2)
#define FLAGS_NVERTEX_SPECS_V3N3C3C3T2T2	(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C3C3|FLAGS_NVERTEX_SPECS_T2T2)
		
#define FLAGS_NVERTEX_SPECS_V3C4b			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C4b)
#define FLAGS_NVERTEX_SPECS_V3N3C4b			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4b)
#define FLAGS_NVERTEX_SPECS_V3C4bT2			(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C4b|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4bT2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4b|FLAGS_NVERTEX_SPECS_T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4bT2T2		(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4b|FLAGS_NVERTEX_SPECS_T2T2)
#define FLAGS_NVERTEX_SPECS_V3N3C4bC4bT2T2	(FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_N3|FLAGS_NVERTEX_SPECS_C4bC4b|FLAGS_NVERTEX_SPECS_T2T2)

//	NCHECK_VERTEX_SPECS_xx 
//	... to easy evaluate a Nu32 VertexSpecs_Flags  !
//  These MACROS are evaluate to ZERO if NO and NON-ZERO if YES for the true/false ones

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + POSITION
// +
// + Is there a position ?
// +	0 if Not. Non Zero if there is a position ... but we don't have any specifications about position format (3f or 4f).
#define NCHECK_VERTEX_SPECS_POSITION(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_POSITION)
// + Type(s) of the position.
// +	Check the position type only, so just one flag. if the flag is ON, position is 3f, if is OFF position is 4f.
// +	Notice that this check doesn't test if there is a position, and if result is 4f (FLAG_NVERTEX_SPECS_POSITION_3f is OFF) 
// +	Result may be corrupted if FLAG_NVERTEX_SPECS_POSITION ( is there a position or not) is OFF too ( it would be a Bug but ...)
// +	Uses NCHECK_VERTEX_SPECS_POSITION before using NCHECK_VERTEX_SPECS_POSITION_TYPE_3f and NCHECK_VERTEX_SPECS_POSITION_TYPE_4f
// +	to be sure there is a position !
#define NCHECK_VERTEX_SPECS_POSITION_TYPE_3f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_POSITION_TYPES,FLAG_NVERTEX_SPECS_POSITION_3f)
#define NCHECK_VERTEX_SPECS_POSITION_TYPE_4f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_POSITION_TYPES,0)
// + Is there a Position of type.
// +	A good way to test because with only one macro we test if there is a position and if this position has a type of...
// +	But a result of 0 with NCHECK_VERTEX_SPECS_POSITION_3f doesn't mean that there is no position, maybe position is 4f, maybe there is no one...
#define NCHECK_VERTEX_SPECS_POSITION_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_POSITION,FLAGS_NVERTEX_SPECS_V3)
#define NCHECK_VERTEX_SPECS_POSITION_4f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_POSITION,FLAGS_NVERTEX_SPECS_V4)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + TBN (Tangent,Normal,BiNormal)
// +
// + Is there a Normal ? A BiNormal ? A Tangent ? the 3  ?
#define NCHECK_VERTEX_SPECS_NORMAL(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_NORMAL)
#define NCHECK_VERTEX_SPECS_BINORMAL(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_BINORMAL)
#define NCHECK_VERTEX_SPECS_TANGENT(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_TANGENT)
#define NCHECK_VERTEX_SPECS_TBN(vertexspecs_Nu32flags)				FLAGS_TEST(vertexspecs_Nu32flags,FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TANGENT,FLAG_NVERTEX_SPECS_NORMAL|FLAG_NVERTEX_SPECS_BINORMAL|FLAG_NVERTEX_SPECS_TANGENT)
// + Type(s) of all the TBN 
#define NCHECK_VERTEX_SPECS_TBN_TYPE_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TBN_TYPES,FLAG_NVERTEX_SPECS_TBN_3f)
#define NCHECK_VERTEX_SPECS_TBN_TYPE_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TBN_TYPES,0)
// + Is there a TBN of type.
#define NCHECK_VERTEX_SPECS_NORMAL_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_NORMAL,FLAGS_NVERTEX_SPECS_N3)
#define NCHECK_VERTEX_SPECS_BINORMAL_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BINORMAL,FLAGS_NVERTEX_SPECS_BI3)
#define NCHECK_VERTEX_SPECS_TANGENT_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TANGENT,FLAGS_NVERTEX_SPECS_TG3)
#define NCHECK_VERTEX_SPECS_NORMAL_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_NORMAL,FLAGS_NVERTEX_SPECS_N3s)
#define NCHECK_VERTEX_SPECS_BINORMAL_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BINORMAL,FLAGS_NVERTEX_SPECS_BI3s)
#define NCHECK_VERTEX_SPECS_TANGENT_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TANGENT,FLAGS_NVERTEX_SPECS_TG3s)
#define	NCHECK_VERTEX_SPECS_TBN_3f(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TBN,FLAGS_NVERTEX_SPECS_TBN3)
#define NCHECK_VERTEX_SPECS_TBN_3s(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_TBN3s)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + COLOR(s)
// +
// + Is there Color0 ? Color1 ? Both of them ( notice that if Color1 is present, Color0 is here !!!)
#define NCHECK_VERTEX_SPECS_COLOR0(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_COLOR0)
#define NCHECK_VERTEX_SPECS_COLOR1(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_COLOR1)
#define NCHECK_VERTEX_SPECS_COLORS(vertexspecs_Nu32flags)			(!FLAGS_TEST(vertexspecs_Nu32flags,FLAG_NVERTEX_SPECS_COLOR0|FLAG_NVERTEX_SPECS_COLOR1,0)) // return 0 if there is No Color, 1 if there is 1 or 2 colors
// + Type(s) of all the Color
#define NCHECK_VERTEX_SPECS_COLORS_TYPE_3f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS_TYPES,FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define NCHECK_VERTEX_SPECS_COLORS_TYPE_4f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS_TYPES,FLAG_NVERTEX_SPECS_COLORS_f)
#define NCHECK_VERTEX_SPECS_COLORS_TYPE_3b(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS_TYPES,FLAG_NVERTEX_SPECS_COLORS_3)
#define NCHECK_VERTEX_SPECS_COLORS_TYPE_4b(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS_TYPES,0)
// + Is there a Color of type.
#define NCHECK_VERTEX_SPECS_COLOR0_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_C3)
#define NCHECK_VERTEX_SPECS_COLOR0_4f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_C4)
#define NCHECK_VERTEX_SPECS_COLOR0_3b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_C3b)
#define NCHECK_VERTEX_SPECS_COLOR0_4b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_C4b)

#define NCHECK_VERTEX_SPECS_COLOR1_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR1,FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3|FLAG_NVERTEX_SPECS_COLORS_f)
#define NCHECK_VERTEX_SPECS_COLOR1_4f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR1,FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_f)
#define NCHECK_VERTEX_SPECS_COLOR1_3b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR1,FLAG_NVERTEX_SPECS_COLOR1|FLAG_NVERTEX_SPECS_COLORS_3)
#define NCHECK_VERTEX_SPECS_COLOR1_4b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLOR1,FLAG_NVERTEX_SPECS_COLOR1)

#define NCHECK_VERTEX_SPECS_COLORS_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS,FLAGS_NVERTEX_SPECS_C3C3)
#define NCHECK_VERTEX_SPECS_COLORS_4f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS,FLAGS_NVERTEX_SPECS_C4C4)
#define NCHECK_VERTEX_SPECS_COLORS_3b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS,FLAGS_NVERTEX_SPECS_C3bC3b)
#define NCHECK_VERTEX_SPECS_COLORS_4b(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_COLORS,FLAGS_NVERTEX_SPECS_C4bC4b)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + TEXCOORD(s)
// +
// + Is there TexCoord0 ? TexCoord1 ? Both of them ( notice that if TexCoord1 is present, TexCoord0 is here !!!)
#define NCHECK_VERTEX_SPECS_TEXCOORD0(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_TEXCOORD0)
#define NCHECK_VERTEX_SPECS_TEXCOORD1(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_TEXCOORD1)
#define NCHECK_VERTEX_SPECS_TEXCOORDS(vertexspecs_Nu32flags)			(!FLAGS_TEST(vertexspecs_Nu32flags,FLAG_NVERTEX_SPECS_TEXCOORD0|FLAG_NVERTEX_SPECS_TEXCOORD1,0)) // return 0 if there is No TexCoord, 1 if there is 1 or 2 TexCoords
// + Type(s) of all the TexCoords
#define NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS_TYPES,FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS_TYPES,FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS_TYPES,FLAG_NVERTEX_SPECS_TEXCOORDS_2)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS_TYPES,0)
// + Is there a TexCoord of type.
#define NCHECK_VERTEX_SPECS_TEXCOORD0_2f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD0,FLAGS_NVERTEX_SPECS_T2)
#define NCHECK_VERTEX_SPECS_TEXCOORD0_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD0,FLAGS_NVERTEX_SPECS_T3)
#define NCHECK_VERTEX_SPECS_TEXCOORD1_2f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD1,FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define NCHECK_VERTEX_SPECS_TEXCOORD1_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD1,FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_f)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_2f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS,FLAGS_NVERTEX_SPECS_T2T2)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_3f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS,FLAGS_NVERTEX_SPECS_T3T3)
#define NCHECK_VERTEX_SPECS_TEXCOORD0_2s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD0,FLAGS_NVERTEX_SPECS_T2s)
#define NCHECK_VERTEX_SPECS_TEXCOORD0_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD0,FLAGS_NVERTEX_SPECS_T3s)
#define NCHECK_VERTEX_SPECS_TEXCOORD1_2s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD1,FLAG_NVERTEX_SPECS_TEXCOORD1|FLAG_NVERTEX_SPECS_TEXCOORDS_2)
#define NCHECK_VERTEX_SPECS_TEXCOORD1_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORD1,FLAG_NVERTEX_SPECS_TEXCOORD1)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_2s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS,FLAGS_NVERTEX_SPECS_T2sT2s)
#define NCHECK_VERTEX_SPECS_TEXCOORDS_3s(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_TEXCOORDS,FLAGS_NVERTEX_SPECS_T3sT3s)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + BONE(s)
// +
// + Is there Bone0 ? Bone1 ? Both of them ( notice that if Bone1 is present, Bone0 is here !!!)
#define NCHECK_VERTEX_SPECS_BONE0(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_BONE0)
#define NCHECK_VERTEX_SPECS_BONE1(vertexspecs_Nu32flags)			(vertexspecs_Nu32flags&FLAG_NVERTEX_SPECS_BONE1)
#define NCHECK_VERTEX_SPECS_BONES(vertexspecs_Nu32flags)			(!FLAGS_TEST(vertexspecs_Nu32flags,FLAG_NVERTEX_SPECS_BONE0|FLAG_NVERTEX_SPECS_BONE1,0)) // return 0 if there is No Bone, 1 if there is 1 or 2 Bones
// + Type(s) of all the Bones
#define NCHECK_VERTEX_SPECS_BONES_TYPE_1d1f(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES_TYPES,0)
#define NCHECK_VERTEX_SPECS_BONES_TYPE_1b(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES_TYPES,FLAG_NVERTEX_SPECS_BONE_1b)
#define NCHECK_VERTEX_SPECS_BONES_TYPE_2b(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES_TYPES,FLAG_NVERTEX_SPECS_BONE_2b)
#define NCHECK_VERTEX_SPECS_BONES_TYPE_2s(vertexspecs_Nu32flags)	FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES_TYPES,FLAG_NVERTEX_SPECS_BONE_2s)
// + Is there a Bone of type.
#define NCHECK_VERTEX_SPECS_BONE0_1d1f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE0,FLAGS_NVERTEX_SPECS_B1d1f)
#define NCHECK_VERTEX_SPECS_BONE0_1b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE0,FLAGS_NVERTEX_SPECS_B1b)
#define NCHECK_VERTEX_SPECS_BONE0_2b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE0,FLAGS_NVERTEX_SPECS_B2b)
#define NCHECK_VERTEX_SPECS_BONE0_2s(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE0,FLAGS_NVERTEX_SPECS_B2s)
#define NCHECK_VERTEX_SPECS_BONE1_1d1f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE1,FLAG_NVERTEX_SPECS_BONE1)
#define NCHECK_VERTEX_SPECS_BONE1_1b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE1,FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_1b)
#define NCHECK_VERTEX_SPECS_BONE1_2b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE1,FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_2b)
#define NCHECK_VERTEX_SPECS_BONE1_2s(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONE1,FLAG_NVERTEX_SPECS_BONE1|FLAG_NVERTEX_SPECS_BONE_2s)
#define NCHECK_VERTEX_SPECS_BONES_1d1f(vertexspecs_Nu32flags)		FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES,FLAGS_NVERTEX_SPECS_B1d1fB1d1f)
#define NCHECK_VERTEX_SPECS_BONES_1b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES,FLAGS_NVERTEX_SPECS_B1bB1b)
#define NCHECK_VERTEX_SPECS_BONES_2b(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES,FLAGS_NVERTEX_SPECS_B2bB2b)
#define NCHECK_VERTEX_SPECS_BONES_2s(vertexspecs_Nu32flags)			FLAGS_TEST(vertexspecs_Nu32flags,MASK_NVERTEX_SPECS_BONES,FLAGS_NVERTEX_SPECS_B2sB2s)

//	NGET_VERTEX_SPECS_xx 
//	... to easy get a value from a Nu32 NVERTEX_SPECS !
//  These MACROS extract the requested value.
#define NGET_VERTEX_SPECS_TEXCOORD_NUMBER(vertexspecs_Nu32flags)	( (((vertexspecs_Nu32flags)>>6)&0x1)+(((vertexspecs_Nu32flags)>>7)&0x1) )
#define NGET_VERTEX_SPECS_COLOR_NUMBER(vertexspecs_Nu32flags)		( (((vertexspecs_Nu32flags)>>4)&0x1)+(((vertexspecs_Nu32flags)>>5)&0x1) )
#define NGET_VERTEX_SPECS_BONE_NUMBER(vertexspecs_Nu32flags)		( (((vertexspecs_Nu32flags)>>8)&0x1)+(((vertexspecs_Nu32flags)>>9)&0x1) )




// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}


#endif	// __cpluplus
#endif	// __NGEOMETRY_VERTEX_FLAGS_H_

