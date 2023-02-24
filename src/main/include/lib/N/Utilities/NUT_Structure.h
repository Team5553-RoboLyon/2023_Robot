#ifndef __NUT_STRUCTURE_H
#define __NUT_STRUCTURE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Structure.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../NSpline.h"
#include "../NStructure.h"

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
#define DEFAULT_NUT_STRCT_2DGRID_DESC_GEOMETRY_FORMAT NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE
#define DEFAULT_NUT_STRCT_2DGRID_DESC_USERSTATESET 0
#define DEFAULT_NUT_STRCT_2DGRID_DESC_VGRAVITY_3f 0.0f, 10.0f, 0.0f
#define DEFAULT_NUT_STRCT_2DGRID_DESC_JOINTRADIUS 1.0f
#define DEFAULT_NUT_STRCT_2DGRID_DESC_JOINTMASS 1.0f

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + JOINT 'TYPE'
// +
// +-+	These types are only used by NUT_STRCT_... building tools to clearly identify all the joints,
// +	and to make things more clear and understandable for user.
// +	That all !
// +	There are no different list, or different specific properties for each specific 'TYPE'.
// +
#define NUT_STRCT_JOINT_CORNER 0	//|_2D Only	|
#define NUT_STRCT_JOINT_IJCENTER 1	//|			|
#define NUT_STRCT_JOINT_IKCENTER 2	//			|-3D
#define NUT_STRCT_JOINT_JKCENTER 3	//			|
#define NUT_STRCT_JOINT_IJKCENTER 4 //			|

#define NUT_STRCT_2D_JOINT_TYPE_NB 2 // Only the 2 first of the define in the list above list.
#define NUT_STRCT_3D_JOINT_TYPE_NB 5 // All of the define in the list above list
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + EDGE 'TYPE'
// +
// +-+	These types are only used by NUT_STRCT_... building tools to clearly identify all the different
// +	constraint Edges, and to make things more clear and understandable for user.
// +	That all!
// +	There are no different list, or different specific properties for each specific 'TYPE'.
// +
#define NUT_STRCT_I 0 //|_2D Only	|
#define NUT_STRCT_J 1 //|			|
#define NUT_STRCT_K 2 //			|-3D

#define NUT_STRCT_2D_AXIS_NB 2 // Only the 2 first of the define in the list above list.
#define NUT_STRCT_3D_AXIS_NB 3 // All of the define in the list above list

#define NUT_STRCT_DIAGONAL_IJ 0	 //|-2D Only	|
#define NUT_STRCT_DIAGONAL_IK 1	 //			|_3D
#define NUT_STRCT_DIAGONAL_JK 2	 //			|
#define NUT_STRCT_DIAGONAL_IJK 3 //			|

#define NUT_STRCT_2D_DIAGONAL_NB 1 // Only the first of the define in the list above list.
#define NUT_STRCT_3D_DIAGONAL_NB 4 // All of the define in the list above list
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + EDGE and DIAGONAL 'Nu8 Flags'
// +
// +-+	Used with all 'BuildFlags_Edge' and 'BuildFlags_Diagonal' of 'NUT_STRCT_2DGRID_DESC',
// +	or 'NUT_STRCT_3DGRID_DESC' to describe precisely things to build up.
// +
#define FLAG_NUT_STRCT_BUILD_SPRINGS BIT_0			  // NSTRUCTURE_ELEMENT_SPRING
#define FLAG_NUT_STRCT_BUILD_SIMPLE_CONSTRAINT BIT_1  // NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT
#define FLAG_NUT_STRCT_BUILD_MINMAX_CONSTRAINT BIT_2  // NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT
#define FLAG_NUT_STRCT_BUILD_SPECIAL_CONSTRAINT BIT_3 // NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT

#define FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT BIT_4 // Works only with diagonals ! Not with Edges ! Split Diagonals at middle and create center if is not already exist
												  // #define FLAG_NUT_STRCT_BUILD_xxx								BIT_5
												  // #define FLAG_NUT_STRCT_BUILD_xxx								BIT_6
												  // #define FLAG_NUT_STRCT_BUILD_xxx								BIT_7

#define MASK_NUT_STRCT_BUILD_CONSTRAINTS (0xF) // BIT_0 to BIT_3 included

// Nu32 FLAGS
#define FLAG_NUT_STRCT_2DGRID_DESC_CREATESKIN BIT_0
#define FLAG_NUT_STRCT_2DGRID_DESC_CREATESKIN_USING_CELLJOINT_CENTERS BIT_1
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_HIGHLEVEL_EXTRACTSET BIT_2
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_GEOMETRY_FORMAT BIT_3
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_USERSTATESET BIT_4
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_APPEARANCE BIT_5

	/*
	#define FLAG_NUT_STRCT_2DGRID_DESC_NTXCBUILD_USEPARAM_TXUSEDOAARECTf32			BIT_6	//|
	#define FLAG_NUT_STRCT_2DGRID_DESC_NTXCBUILD_USEPARAM_TXSUBAARECTf32			BIT_7	//|
	#define FLAG_NUT_STRCT_2DGRID_DESC_NTXCBUILD_FLIP_U								BIT_8	//|-To be transfered as FLAG_NTXCBUILD_... flags to be used in the appropriate functions.
	#define FLAG_NUT_STRCT_2DGRID_DESC_NTXCBUILD_FLIP_V								BIT_9	//|	!!! update Macro "CONVERT_FLAGS_NUT_STRCT_2DGRID_DESC_CREATESKIN_NTXCBUILD_TO_FLAGS_NTXCBUILD" in case of changes.
	#define FLAG_NUT_STRCT_2DGRID_DESC_NTXCBUILD_FLIP_W								BIT_10	//|

	#define FLAG_NUT_STRCT_2DGRID_DESC_NVCOLBUILD_USEPARAMS_COLORRANGE				BIT_11	//|_To be transfered as FLAG_NVCOLBUILD_... flags to be used in the appropriate functions.
	#define FLAG_NUT_STRCT_2DGRID_DESC_NVCOLBUILD_USEPARAM_COLORGRADIENTID			BIT_12	//| !!! update Macro "CONVERT_FLAGS_NUT_STRCT_2DGRID_DESC_CREATESKIN_NVCOLBUILD_TO_FLAGS_NVCOLBUILD" in case of changes.
	*/

#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTRADIUS BIT_13
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTMASS BIT_14
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTSPEEDDAMPING BIT_15
#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_VGRAVITY BIT_16

	/*
	todo: define these 4 flags ...
	#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_BUILDORIGIN							BIT_17
	#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAMS_BUILDAXIS							BIT_18
	#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAMS_CONSTRAINTNB						BIT_19
	#define FLAG_NUT_STRCT_2DGRID_DESC_USEPARAMS_CONSTRAINTDESC						BIT_20
	*/

	// Notes that members don't have associated USEPARAM flags, that means they are always used !!!
	// It seems normal because those members are flags too !!!
	//		BuildFlags_Edge (all of them)
	//		BuildFlags_Diagonal

#define FLAG_NUT_STRCT_2DGRID_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB BIT_21 // activate FLAG_NSTRUCTURE_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB
	// +
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	typedef struct NUT_STRCT_2DGRID_DESC NUT_STRCT_2DGRID_DESC;
	struct NUT_STRCT_2DGRID_DESC
	{
		Nu32 Flags;

		NEXTRACTSET HighLevelExtractSet;
		NGEOMETRY_FORMAT_ENUM GeometryFormat;
		NSTATESET UserStateSet;
		NAPPEARANCE_RAW Appearance;

		// Physics: global parameters
		Nf32 JointRadius; // |_Used to create process internal NJOIN_DESC
		Nf32 JointMass;	  // |

		NVEC3 vGravity;			// |_Of the created NSTRUCTURE
		Nf32 JointSpeedDamping; // |
		// Joints building
		union
		{
			NVEC3 BuildAxis[NUT_STRCT_2D_AXIS_NB];
			struct
			{
				NVEC3 BuildAxis_I;
				NVEC3 BuildAxis_J;
			};
		};
		NVEC3 BuildOrigin;
		// Constraints building
		union
		{
			Nu8 BuildFlags_Edge[NUT_STRCT_2D_JOINT_TYPE_NB][NUT_STRCT_2D_AXIS_NB];
			struct
			{
				Nu8 BuildFlags_Edge_Corner_I;
				Nu8 BuildFlags_Edge_Corner_J;
				Nu8 BuildFlags_Edge_Center_I;
				Nu8 BuildFlags_Edge_Center_J;
			};
		};
		Nu8 BuildFlags_Diagonal; // Only one 2D Type... Diagonal of the IJ PLANE of course.
		union
		{
			Nu32 ConstraintsNb[NUT_STRCT_2D_AXIS_NB];
			struct
			{
				Nu32 ConstraintsNb_I;
				Nu32 ConstraintsNb_J;
			};
		};
		union
		{
			NCONSTRAINT_DESC ConstraintDesc[NSTRUCTURE_ELEMENT_CONSTRAINT_TYPE_NUMBER]; // SPRING,SIMPLE,MINMAX,SPECIAL
			struct
			{
				NCONSTRAINT_DESC ConstraintDesc_Spring;
				NCONSTRAINT_DESC ConstraintDesc_Simple;
				NCONSTRAINT_DESC ConstraintDesc_MinMax;
				NCONSTRAINT_DESC ConstraintDesc_Special;
			};
		};

		// Vertex TexCoord Building
		NBUILDBASE_TEXCOORD TexCoordBuild;
		// 	NRECTf32			TexCoordUsedOAARectf32;		// Represent an oriented rectangular area of a texture.
		// 	NRECTf32			TexCoordSubAARectf32;		// Represent a NON-oriented rectangular sub area of oriented rectangular area.
		// Vertex Color Management
		NBUILDBASE_COLOR ColorBuild;
		// 	NCOLOR				*pColorRange;
		// 	Nu32				ColorRangeSize;
		// 	NCOLORGRADIENTID	ColorGradientID;
	};

	// Enum used only by the function "NUT_PhysicRopeAlign"
	typedef enum
	{
		NUT_PHYSIC_ROPE_ALIGN_USE_SPRING_RESTLENGTH = 0,
		NUT_PHYSIC_ROPE_ALIGN_USE_SIMPLECONSTRAINT_LENGTH,
		NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MINLENGTH,
		NUT_PHYSIC_ROPE_ALIGN_USE_MINMAXCONSTRAINT_MAXLENGTH,
		NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MINLENGTH,
		NUT_PHYSIC_ROPE_ALIGN_USE_SPECIALCONSTRAINT_MAXLENGTH
	} NUT_PHYSIC_ROPE_ALIGN;
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

	Nf32 NUT_PhysicRopeAlign(NSTRUCTURE *pstructure, const NVEC3 *axis, const NUT_PHYSIC_ROPE_ALIGN method);
	Nbool NUT_IterateUntilPhysicStructureStabilization(NSTRUCTURE *ps, const Nu32 time_max, const Nf32 error_tolerance);

	NSTRUCTURE *NUT_Setup2DGridStructure(NSTRUCTURE *pstructure, const NUT_STRCT_2DGRID_DESC *pgrid_desc);
	NSTRUCTURE *NUT_Create2DGridStructure(const NUT_STRCT_2DGRID_DESC *p2dgriddesc);
	inline void NUT_Clear2DGridStructure(NSTRUCTURE *ps) { NClearPhysicStructure(ps); }
	inline void NUT_Delete2DGridStructure(NSTRUCTURE *ps) { NDeletePhysicStructure(ps); }

	// LOW LEVEL Functions (used by 'NUT_SetUp2DGridStructure', and useful for expert ... )
	void NUT_Extract_Structure2D_Grid_Elements_Numbers(const NUT_STRCT_2DGRID_DESC *pstrct_desc, Nu32 *presult_table, const NSTRUCTURE_ELEMENT_TYPE_ENUM first_element, const Nu32 range_size);
	Nu32 NUT_Build_Structure_2DGrid_Joints(NSTRUCTURE *pstructure, const NUT_STRCT_2DGRID_DESC *pstrct_desc);
	void NUT_Build_Structure_2DGrid_Constraints(NSTRUCTURE *pstructure, const NUT_STRCT_2DGRID_DESC *pstrct_desc, const Nu32 first_joint, const Nu8 builded_constraints_mask);

/*
// LOW LEVEL MACROS ( Private use only )
//  To perform a right conversions from:	FLAGS_NUT_STRCT_2DGRID_... to FLAG_NVCOLBUILD_...
//											FLAGS_NUT_STRCT_2DGRID_... to FLAG_NTXCBUILD_...
// ... and use the result as flags into 'NVertexColorBuild' and 'NVertexTexCoordBuild' Low Level functions ...
#define CONVERT_FLAGS_NUT_STRCT_2DGRID_DESC_CREATESKIN_NTXCBUILD_TO_FLAGS_NSKIN_DESC_NTXCBUILD(flags)		( ((flags)>>6 )&0x1F )
#define	CONVERT_FLAGS_NUT_STRCT_2DGRID_DESC_CREATESKIN_NVCOLBUILD_TO_FLAGS_NSKIN_DESC_NVCOLBUILD(flags)		( ((flags)>>6)&0x60  )
*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_H
