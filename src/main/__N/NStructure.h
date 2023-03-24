#ifndef __NSTRUCTURE_H
#define __NSTRUCTURE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NStructure.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"
#include "./Containers/NNode.h"
#include "./Containers/NXNode.h"
#include "./Containers/NArray.h"
#include "NFlags.h"
#include "NFastAnimatedValue.h"
#include "./Render/Renderable/NRenderable.h"
#include "./Render/Renderable/NRenderable_ExtractSet.h"
#include "./Geometry/NGeometry.h"
#include "./Miscellaneous/NColorGradient.h"
#include "./Miscellaneous/NVertexColorBuild.h"
#include "./Miscellaneous/NVertexTexCoordBuild.h"
#include "./Core/NTime.h"
#include "NBoundingVolumes.h"
#include "NPlane.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Globales											**
// ***************************************************************************************
//extern	NNODELIST	NActivePhysicStructureList;
extern	NARRAY		NActivePhysicStructuresPtr_Array;	// Private for internal use only

extern	NNODELIST	NActivePhysicStructureCollisionCoupleList;

extern	Nf32		fSimulationTimeStep;
extern	Nf32		fAirFriction;

//extern	NNODELIST	SkinList;

// ***************************************************************************************
// **									FLAGs Defines									**
// ***************************************************************************************
// NPHYSICSTRUCTURE Flags:
//#define FLAG_PHYSICSTRUCTURE_ACTIVE			BIT_0	// Means this structure is active == inserted into "NActiveStructureList"
// ***************************************************************************************
// **									VALUE Defines									**
// ***************************************************************************************
// Core features
#define DEFAULT_NSTRUCTURE_RENDERABLE_HIGHLEVEL_EXTRACTSET		FLAG_NEXTRACTSET_3D
#define DEFAULT_NSTRUCTURE_RENDERABLE_BOUNDINGSPHERE_RADIUS		1.0f
#define DEFAULT_PHYSICACCUMULATOR_CAPACITY						128		// How many physic Structures rendered (... and updated) at the same time ? This is it.
																		// ... more precisely, this is the initial array capacity. So if the reel number is bigger we will 
																		// have reallocation !
#define DEFAULT_PHYSICSTRUCTURE_FORCEFIELDS_ARRAY_CAPACITY		8

#define NDEFAULT_JOINTSPEEDDAMPING			1.0f
#define DEFAULT_AIRFRICTION					0.5f

// Default gravity vector
#define DEFAULT_NSTRUCTURE_VGRAVITY_3f		0.0f,150.0f,0.0f
// Constraint Default Values
#define	DEFAULT_CONSTRAINT_LENGTH			1.0f
#define DEFAULT_CONSTRAINT_MINILENGTH		NF32_EPSILON_VECTOR_LENGTH
#define DEFAULT_CONSTRAINT_MAXLENGTH		1.0f

// Spring Default Values
#define DEFAULT_SPRING_K					1.0f
#define DEFAULT_SPRING_FRICTION				0.1f
#define DEFAULT_SPRING_RESTLENGTH			1.0f
#define DEFAULT_SPRING_RESTLENGTH_RATIO		0.725f

// Joint Default Values	and Flags
#define DEFAULT_JOINT_MASS					1.0f
#define DEFAULT_JOINT_RADIUS				1.0f


// SKIN Default Values
#define DEFAULT_SKIN_VERTEBRA_LENGTH		3.0f

// Simulation Time                     
#define SIMULATION_TIMESTEP					0.005f	// 0.01f = 100 times per second -- 0.005f = 200 times per second -- 0.001f = 1000 times per second


// COLLISIONS Default Values
// #define REPULSION_IMPULSION					200.0f

// ***************************************************************************************
// **							Structures												**
// ***************************************************************************************
typedef enum
{
	NSTRUCTURE_RIGHTTOLEFT,
	NSTRUCTURE_TOPTOBOTTOM,
	NSTRUCTURE_BOTTOMTOTOP,
	NSTRUCTURE_POINT
}NSTRUCTURE_POSITION;

typedef enum										// To recognize which kind of element is used, for example into a skin
{													// by the "skin pointer" or by Constraint creation functions ...
	NSTRUCTURE_ELEMENT_SPRING = 0,			//|
	NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT,	//|_ All the constraints at the top of the enum, from 0 to n
	NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT,	//|
	NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT,	//|
	// ------------------------------------
	NSTRUCTURE_ELEMENT_JOINT,				//| 'Joint' The only one which is not a constraint, at the End of the enum
	// ------------------------------------
	NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE
}NSTRUCTURE_ELEMENT_TYPE_ENUM;
#define NSTRUCTURE_ELEMENT_CONSTRAINT_TYPE_NUMBER	(NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE-1) // All NSTRUCTURE_ELEMENTS are constraints JOINT excepted !


#define FLAG_JOINT_FIXED					BIT_0
typedef struct NJOINT NJOINT;
struct NJOINT 
{
	Nu16		Flags;
	Nu16		Available_Nu16;

	Nf32		fMass;
	Nf32		fRadius;

	NVEC3	vForcesAccumulator;

	NVEC3	vSpeed;				// For Euler Integration Only
	NVEC3	CurrentPosition;	// For Euler and Verlet Integration
	NVEC3	OldPosition;		// For Verlet Integration Only
};

typedef struct NCONSTRAINT NCONSTRAINT;
struct NCONSTRAINT 
{
	NJOINT		*pjA;
	NJOINT		*pjB;
	NVEC3	AB;

	union
	{
		struct  
		{
			Nf32	fLength;		// for SIMPLE NCONSTRAINT
		};

		struct  
		{
			Nf32	fMinLength;		// for NMIN-NMAX NCONSTRAINT and SPECIAL NCONSTRAINT
			Nf32	fMaxLength;		// for NMIN-NMAX NCONSTRAINT and SPECIAL NCONSTRAINT
		};

		struct  
		{
			Nf32	fRestLength;	// for SPRING
			Nf32	fK;				// for SPRING
			Nf32	fFriction;		// for SPRING
		};

		Nf32		f[3];			// for Indexed  Access ...
	};
};

typedef struct NSTRUCTURE NSTRUCTURE;
typedef void (*ACCUMULATEFORCE_FCT)(NSTRUCTURE* pstructure,Nf32 fdelay);
typedef void (*INTEGRATION_FCT)(NSTRUCTURE* pstructure,Nf32 fdelay);
typedef void (*SATISFYCONSTRAINT_FCT)(NSTRUCTURE* pstructure,Nf32 fdelay);


#define FLAG_NSTRUCTURE_FORCEFIELD_INSENSITIVE								BIT_0
#define FLAG_NSTRUCTURE_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB	BIT_1 // by default BONDINGVOLUME STILL the same (no update!). This Flag force BV update at each turn
#define FLAG_NSTRUCTURE_ACTIVE												BIT_2 
struct NSTRUCTURE
{
	NRENDERABLE				*pRenderable;

	Nu32					Flags;
	Nu32					CollisionReferencesCounter;

	ACCUMULATEFORCE_FCT		pAccumulateForce;
	INTEGRATION_FCT			pIntegration;
	SATISFYCONSTRAINT_FCT	pSatisfyConstraint;
	
	Nf32					JointSpeedDamping;

	NVEC3				vGravity;				// Structure Gravity ( each structure has its own Gravity vector )
	
	NAABB					JointsAABB;

	union
	{
		NARRAY	ElementArray[NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE];
		
		struct  
		{
			NARRAY			SpringArray;			// == ElementArray[NSTRUCTURE_ELEMENT_SPRING]				All the Structure Springs
			NARRAY			SimpleConstraintArray;	// == ElementArray[NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT]	All the Structure 'Simple' Constraints
			NARRAY			MinMaxConstraintArray;	// == ElementArray[NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT]	All the Structure 'MinMax' Constraints
			NARRAY			SpecialConstraintArray;	// == ElementArray[NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT]	All the Structure 'Special' Constraints
			NARRAY			JointArray;				// == ElementArray[NSTRUCTURE_ELEMENT_JOINT]				All the Structure Joints
		};											   				
	};
	
	NARRAY					SkinArray;						// All the Structure Skin.
};

// =============================================================================================
// TO CREATE A  PHYSIC STRUCTURE ...
// =============================================================================================
// To build the main physic structure:
#define FLAG_NSTRUCTURE_DESC_WITH_RENDERABLE										BIT_0
#define FLAG_NSTRUCTURE_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB	BIT_1 // link with FLAG_NSTRUCTURE_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB
#define FLAG_NSTRUCTURE_DESC_USEPARAM_HIGHLEVEL_EXTRACTSET							BIT_2

#define FLAG_NSTRUCTURE_DESC_USEPARAM_RENDERABLEGEOMETRY_CAPACITY					BIT_3

#define FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSARRAY_CAPACITY							BIT_4
#define FLAG_NSTRUCTURE_DESC_USEPARAM_SPRINGS_CAPACITY								BIT_5
#define FLAG_NSTRUCTURE_DESC_USEPARAM_SIMPLE_CONSTRAINTS_CAPACITY					BIT_6
#define FLAG_NSTRUCTURE_DESC_USEPARAM_MINMAX_CONSTRAINTS_CAPACITY					BIT_7
#define FLAG_NSTRUCTURE_DESC_USEPARAM_SPECIAL_CONSTRAINTS_CAPACITY					BIT_8
#define FLAG_NSTRUCTURE_DESC_USEPARAM_SKINARRAY_CAPACITY							BIT_9

#define FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSPEEDDAMPING								BIT_10
#define FLAG_NSTRUCTURE_DESC_USEPARAM_VGRAVITY										BIT_11

#define FLAG_NSTRUCTURE_DESC_BIND_EULER_PHYSIC_FUNCTIONS							BIT_12
#define FLAG_NSTRUCTURE_DESC_USEPARAM_ACCUMULATEFORCES_USERFUNCTION					BIT_13
#define FLAG_NSTRUCTURE_DESC_USEPARAM_INTEGRATION_USERFUNCTION						BIT_14
#define FLAG_NSTRUCTURE_DESC_USEPARAM_SATISFYCONSTRAINT_USERFUNCTION				BIT_15

#define FLAGS_NSTRUCTURE_DESC_USEPARAMS_CONSTRAINTS_CAPACITIES	(FLAG_NSTRUCTURE_DESC_USEPARAM_SPRINGS_CAPACITY|FLAG_NSTRUCTURE_DESC_USEPARAM_SIMPLE_CONSTRAINTS_CAPACITY|FLAG_NSTRUCTURE_DESC_USEPARAM_MINMAX_CONSTRAINTS_CAPACITY|FLAG_NSTRUCTURE_DESC_USEPARAM_SPECIAL_CONSTRAINTS_CAPACITY)
typedef struct NSTRUCTURE_DESC NSTRUCTURE_DESC;
struct NSTRUCTURE_DESC 
{
	Nu16		Flags;

	Nu16		RenderableGeometryCapacity;

	NEXTRACTSET	HighLevelExtractSet;

	Nu32		JointsArrayCapacity;
	Nu32		SpringsArrayCapacity;
	Nu32		SimpleConstraintsArrayCapacity;
	Nu32		MinMaxConstraintsArrayCapacity;
	Nu32		SpecialConstraintsArrayCapacity;
	Nu32		SkinArrayCapacity;
	Nf32		JointSpeedDamping;
	NVEC3	vGravity;
	
	ACCUMULATEFORCE_FCT		pAccumulateForce_UserFunction;
	INTEGRATION_FCT			pIntegration_UserFunction;
	SATISFYCONSTRAINT_FCT	pSatisfyConstraint_UserFunction;
};

// To build joint:
typedef struct 
{
	Nf32		fMass;
	Nf32		fRadius;
	NVEC3	vSpeed;						
	
	NVEC3	CurrentPosition;	
	NVEC3	OldPosition;		
}NJOINT_DESC;

// To build constraint:
#define FLAG_NCONSTRAINT_DESC_USE_LENGTHPARAMS_AS_RATIO_OF_CURRENT_JOINTS_DIST		BIT_0
//#define FLAG_NCONSTRAINT_BUILDER_RANGE_LOOP											BIT_1



/*
typedef enum 			
{																
	NCONSTRAINT_DESC_TYPE_SPRING=0,
	NCONSTRAINT_DESC_TYPE_SIMPLE,
	NCONSTRAINT_DESC_TYPE_MINMAX,
	NCONSTRAINT_DESC_TYPE_SPECIAL,
	// ------------------------------------
	NSTRUCTURE_BUILDER_CONSTRAINT_TYPE_ENUM_SIZE
}NCONSTRAINT_DESC_TYPE_ENUM;
*/
/*
typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;
}NCONSTRAINT_COMMON_DESC;

typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;
	Nf32							fRestLength;
	Nf32							fK;		
	Nf32							fFriction;
}NSPRING_DESC;

typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;
	Nf32							fLength;
}NCONSTRAINT_SIMPLE_DESC;

typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;
	Nf32							fMinLength;
	Nf32							fMaxLength;
}NCONSTRAINT_SPECIAL_DESC;

typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;

	Nf32							fMinLength;
	Nf32							fMaxLength;
}NCONSTRAINT_MINMAX_DESC;

typedef union
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	
	NCONSTRAINT_COMMON_DESC			Common;
	
	NCONSTRAINT_MINMAX_DESC			MinMax_Constraint;
	NCONSTRAINT_SIMPLE_DESC			Simple_Constraint;
	NCONSTRAINT_SPECIAL_DESC		Special_Constraint;
	NSPRING_DESC					Spring;
}NCONSTRAINT_DESC;
*/


// NEW FORM :
typedef struct  
{
	NSTRUCTURE_ELEMENT_TYPE_ENUM	Constraint_Type;
	Nu32							Flags;

	union
	{
		struct  
		{
			Nf32	fLength;		// for SIMPLE NCONSTRAINT
		};

		struct  
		{
			Nf32	fMinLength;		// for NMIN-NMAX NCONSTRAINT and SPECIAL NCONSTRAINT
			Nf32	fMaxLength;		// for NMIN-NMAX NCONSTRAINT and SPECIAL NCONSTRAINT
		};

		struct  
		{
			Nf32	fRestLength;	// for SPRING
			Nf32	fK;				// for SPRING
			Nf32	fFriction;		// for SPRING
		};

		Nf32		f[3];			// for Indexed  Access ...
	};
}NCONSTRAINT_DESC;

typedef struct  
{
	Nu32	JointAIndex;
	Nu32	JointBIndex;
}NCONSTRAINT_PATTERN;

// Joints Chunk
typedef enum
{
	NJOINTSCHUNK_TYPE_INSPHERE,
	NJOINTSCHUNK_TYPE_INAABB,
	NJOINTSCHUNK_TYPE_RIGHTSIDEOFPLANE,
	//--------------------------
	NJOINTSCHUNK_TYPE_ENUMSIZE
}NJOINTSCHUNK_TYPE_ENUM;

typedef struct
{
	NJOINTSCHUNK_TYPE_ENUM	Type;
	
	union
	{
		NBOUNDINGSPHERE	Sphere;
		NAABB			AABB;
		NPLANE			Plane;
	};
}NJOINTSCHUNK;


// ***************************************************************************************
// **																					**
// **								SKIN												**
// **																					**
// ***************************************************************************************
typedef enum NSKIN_TYPE							// Mainly used for saving/loading skin process			
{
	NSKIN_TYPE_BILLBOARD=0,
	NSKIN_TYPE_ROPE,
	NSKIN_TYPE_GRID,
	// -------------
	NSKIN_TYPE_ENUMSIZE
}NSKIN_TYPE;

typedef enum NSKIN_SCHEME
{
	NSKIN_SCHEME_ROPE_QUADS_DOUBLEROWS,
	NSKIN_SCHEME_ROPE_QUADS_ONEROW,
	NSKIN_SCHEME_ROPE_TRIANGLES_DOUBLEROW,
	NSKIN_SCHEME_ROPE_TRIANGLES_ONEROW,

	NSKIN_SCHEME_GRID_QUADS,
	NSKIN_SCHEME_GRID_FANQUADS,
	NSKIN_SCHEME_BILLBOARD_1QUAD,
	// ---------------------------
	NSKIN_SCHEME_VOID						// Means, no scheme
}NSKIN_SCHEME;


typedef union NSKIN NSKIN; // This early typedef to a right compilation of the structures bellow

typedef struct NSKIN_BILLBOARD NSKIN_BILLBOARD;
struct NSKIN_BILLBOARD 
{
	void			(*pUpdateSkinGeometry)(NSKIN*);					// The Update function used to update skin vertex position from structure element which
															// are referenced below.
	NSKIN_TYPE		SkinType;
	NSTRUCTURE_ELEMENT_TYPE_ENUM	StructureElementType; // To know which kind of reference is used. A joint ? a Spring ?
														// ... Important to remember that, mainly for saving/loading process.	
	NGEOMETRY		*pGeometry;					// A pointer on the geometry where all the "skin vertex" are referenced.
												// this geometry is one of the geometries stores into the structure Renderable.
	NSKINVERTEX		*pFirstVertex;				// Pointer on the first Vertex used by the Skin. Indeed, all the skin with the same
												// texture are into the same geometry !


	NJOINT			*pCenter;					// A valid pointer on an Structure Element (type specified by "StructureElementType"). Its position will be the center position of the Billboard 

	NJOINT			*pOrient_A;					// A valid pointer on an Structure Element (Same type for all the referenced Structure Element).
	NJOINT			*pOrient_B;					// A valid pointer on an Structure Element (Same type for all the referenced Structure Element).
												// The position of the 2 previous structure element pointed as pOrient_A and pOrient_B are used
												// to compute the vector AB which is used as Xaxis to orient the Billboard.
	Nf32			fScale;						// BillBoard scale
	Nf32			RatioWidth_Height;			// Width/height of the Billboard ( BB is a 2D rectangle, instead of keeping extents we just keep this ratio. We do that because there is Scale too)
	NVEC3		VShift;						// To shift (translate) Billboard from its ref_center point to its final position.
};

typedef struct 
{
	Nf32	SideA;
	Nf32	SideB;
	Nf32	Bulge;
}NSKIN_ROPE_VERTEBRA;

typedef struct NSKIN_ROPE NSKIN_ROPE;
struct NSKIN_ROPE 
{
	void		(*pUpdateSkinGeometry)(NSKIN*);	// The Update function used to update skin vertex position from structure element which
												// are referenced below.
	NSKIN_TYPE	SkinType;
	NSTRUCTURE_ELEMENT_TYPE_ENUM	StructureElementType;			// To know which kind of reference is used. A constraint ? a Spring ?
												// ... Important to remember that for saving/loading process.	
												// Notice in that specific case it cant be a joint because we need a vector( a direction)
												// to build up vertebras.
	NGEOMETRY	*pGeometry;						// A pointer on the geometry where all the "skin vertex" are referenced.
												// this geometry is one of the geometries stores into the structure Renderable.
	NSKINVERTEX	*pFirstVertex;					// Pointer on the first Vertex used by the Skin. Indeed, all the skin with the same
												// texture are into the same geometry !

	void		*pFirst;						// A valid pointer on an Structure Element (type specified by "StructureElementType"). Its position will be the center position of the Billboard 
	Nu32		RangeSize;						// Number of reference (constraint) used to build the rope. 

/*
	NARRAY		*pSideAVertebra;				// A valid pointer on a Nf32 array. Each Nf32 value represents a vertebra length.
	NARRAY		*pSideBVertebra;				// A valid pointer on a Nf32 array. Each Nf32 value represents a vertebra length.
												// There are two vertebras for each Reference, so 2 vertex build for each Reference.
												// So, each array must have a size of "ReferenceNumber" element.
	Nf32		TwistBulgeFactor;				// magnify the bulge created by the rope twist. A value of 0 cancel it, a negative one reverse it !
*/
	NARRAY		VertebrasArray;					// a NSKIN_ROPE_VERTEBRA array. 
												
};

typedef struct NSKIN_GRID NSKIN_GRID;
struct NSKIN_GRID 
{
	void		(*pUpdateSkinGeometry)(NSKIN*);	// The Update function used to update skin vertex position from structure element which
												// are referenced below.
	NSKIN_TYPE	SkinType;
	NSTRUCTURE_ELEMENT_TYPE_ENUM	StructureElementType;			// To know which kind of reference is used. 
												// ... Important to remember that for saving/loading process.	
												// Notice in that specific case its always a joint.
	NGEOMETRY	*pGeometry;						// A pointer on the geometry where all the "skin vertex" are referenced.
												// this geometry is one of the geometries stores into the structure Renderable.
	NSKINVERTEX	*pFirstVertex;					// Pointer on the first Vertex used by the Skin. Indeed, all the skin with the same
												// texture are into the same geometry !


	void		*pFirst;						// A valid pointer on an Structure Element (type specified by "StructureElementType"). Its position will be the center position of the Billboard 
	Nu32		RangeSize;						// Number of reference (joints) used to build the skin.
	Nu32		I_CellsNb;						// Number of joints along I ( main line, vertex are consecutive 0,1,2,3 ...)
	Nu32		J_CellsNb;						// Number of joints along J ( secondary line, vertex are not consecutive 0,MainlinesNb,2xMainLineNb,...)
};

typedef struct NSKIN_COMMON NSKIN_COMMON;
struct NSKIN_COMMON 
{
	void							(*pUpdateSkinGeometry)(NSKIN*);	// The Update function used to update skin vertex position from structure element.
	NSKIN_TYPE						SkinType;
	NSTRUCTURE_ELEMENT_TYPE_ENUM	StructureElementType;			// To know which kind of reference is used. 
																	// ... Important to remember that for saving/loading process.	
	NGEOMETRY						*pGeometry;						// A pointer on the geometry where all the "skin vertex" are referenced.
	NSKINVERTEX						*pFirstVertex;					// Pointer on the first Vertex used by the Skin. Indeed, all the skin with the same
																	// texture are into the same geometry !
};


union NSKIN 
{
	NSKIN_COMMON	common;
	NSKIN_BILLBOARD	BillBoard;
	NSKIN_ROPE		Rope;
	NSKIN_GRID		Grid;
};
typedef void (*NUPDATESKIN_FCT)(NSKIN* pskin);


#define FLAG_NSKIN_DESC_TRIANGLE_CW_BUILDING					BIT_0 // Default is CCW
#define FLAG_NSKIN_DESC_BUILD_TEXCOORDS							BIT_1
#define FLAG_NSKIN_DESC_BUILD_COLORS							BIT_2


typedef struct NSKIN_COMMON_DESC NSKIN_COMMON_DESC;
struct NSKIN_COMMON_DESC 
{
	Nu32				Flags;					// 32 FLAGS to setup different building method (uv for exemple)
	NSKIN_SCHEME		Structure_Scheme;		// - Type of the structure mesh created/managed by the skin
	// TexCoords building
	NBUILDBASE_TEXCOORD	TexCoordBuild;
	// Color building
	NBUILDBASE_COLOR	ColorBuild;
};

typedef struct NSKIN_BILLBOARD_DESC NSKIN_BILLBOARD_DESC;
struct NSKIN_BILLBOARD_DESC 
{	
	Nu32				Flags;						// 32 FLAGS to setup different building method (uv for exemple)
	NSKIN_SCHEME		Structure_Scheme;			// - Type of the structure mesh created/managed by the skin
	// TexCoords building
	NBUILDBASE_TEXCOORD	TexCoordBuild;
	// Color building
	NBUILDBASE_COLOR	ColorBuild;

	NSTRUCTURE_ELEMENT_TYPE_ENUM		Ref_StructureElementType;	// - Type of the structure elements used to create and update the skin.
	Nu32				Ref_Center;					// - Index of the structure element used to create/update Billboard Center
	Nu32				Ref_Orient_A;				// - Index of the structure element used to create/update Orient Vector AB
	Nu32				Ref_Orient_B;				// - Index of the structure element used to create/update Orient Vector AB
	Nf32				fScale;						// - Scale of the Billboard
	Nf32				RatioWidth_Height;			// - width/height of the Billboard ( BB is a 2D rectangle, instead of keeping extents we just keep this ratio. We do that because there is Scale too)
	NVEC3			VShift;						// - To shift the whole Billboard from it's ref_center point
};

typedef struct NSKIN_ROPE_DESC NSKIN_ROPE_DESC;
struct NSKIN_ROPE_DESC 
{	
	Nu32				Flags;						// 32 FLAGS to setup different building method (uv for example)
	NSKIN_SCHEME		Structure_Scheme;			// - Type of the structure mesh created/managed by the skin
	// TexCoords building
	NBUILDBASE_TEXCOORD	TexCoordBuild;
	// Color building
	NBUILDBASE_COLOR	ColorBuild;
	
	NSTRUCTURE_ELEMENT_TYPE_ENUM		Ref_StructureElementType;	// - Type of the structure elements used to create and update the skin.
	Nu32				Ref_First;					// - Index of the structure element used to create/update Billboard Center
	Nu32				Ref_RangeSize;				// - Size of the element range (=number of ALL elements) used first included.
	Nf32				VertebraSize;				// Size of one Vertebra. This value will be spread into the 2 Vertebras Arrays created and linked with the Skin Rope
	Nf32				TwistBulgeFactor;			// magnify the bulge created by the rope twist. A value of 0 cancel it, a negative one reverse it !
};

typedef struct NSKIN_GRID_DESC NSKIN_GRID_DESC;
struct NSKIN_GRID_DESC 
{	
	Nu32				Flags;						// 32 FLAGS to setup different building method (uv for exemple)
	NSKIN_SCHEME		Structure_Scheme;			// - Type of the structure mesh created/managed by the skin
	// TexCoords building
	NBUILDBASE_TEXCOORD	TexCoordBuild;
	// Color building
	NBUILDBASE_COLOR	ColorBuild;

	NSTRUCTURE_ELEMENT_TYPE_ENUM		Ref_StructureElementType;	// - Type of the structure elements used to create and update the skin.
	Nu32				Ref_First;						// - Index of the first structure element used to create/update Grid
	Nu32				Ref_RangeSize;					// - Size of the element range (= number of ALL elements) used first included.
	Nu32				I_CellsNb;						// Number of joints along I ( main line, vertex are consecutive 0,1,2,3 ...)
	Nu32				J_CellsNb;						// Number of joints along J ( secondary line, vertex are not consecutive 0,MainlinesNb,2xMainLineNb,...)

	//NCOLOR			ColorRange[NRANGE_4COLORS];		// Used for Quad building (Text/Caption).
};

typedef union NSKIN_DESC NSKIN_DESC;
union NSKIN_DESC 
{
	NSKIN_COMMON_DESC		Common;
	NSKIN_BILLBOARD_DESC	BillBoard;
	NSKIN_GRID_DESC			Grid;
	NSKIN_ROPE_DESC			Rope;
};

typedef struct  
{
	Nf32	SeparatingLimit;	// = JointA_Radius + JointB_Radius. Beyond this limit there is no collision ! 
	Nf32	SeparatingDistance;	// The Current Distance between JointA and Joint B. Because there is a collision, we are sure SeparatingDistance <= SeparatingLimit !
	NJOINT	*pjA;
	NJOINT	*pjB;
}NSTRUCTURE_COLLISION_RESULT;

typedef struct NSTRUCTURE_COLLISONCOUPLE NSTRUCTURE_COLLISONCOUPLE;
typedef void (*NSTRUCTURE_COLLISION_RESPONSE_FCT)(NSTRUCTURE_COLLISONCOUPLE* pc,NSTRUCTURE_COLLISION_RESULT *pcollisionresult);
struct NSTRUCTURE_COLLISONCOUPLE
{
	NSTRUCTURE							*pStructureA;
	NSTRUCTURE							*pStructureB;

	NSTRUCTURE_COLLISION_RESPONSE_FCT	CollisionResponse;
};

// 1/Springs setting up (All length recalculation, based on the new vertex position)
// ---------------------------------------------------------------------------------
// for i=0 to number of springs
//		update vector and length spring[i]
//		AB = joint B - joint A 
//		nAB = AB Normalized 
//		fLength = AB length
//		fStrength = fLength*fK;
//		Pass Strength to each of the  2 joints connected with 
//
// 2/ Joints positions Update
// --------------------------
// for i=0 to number of joints
//		transform the strength accumulator into acceleration
//		Integrate acceleration into speed
//		Calculate the new position
//		Set the strength accumulator to 0
//
// 3/ Vertebra positions Update
// ----------------------------
// for i=0 to number of vertebra
//		calculate vertebra extremity position based on the associated 'joint position' and 'spring vector'
//		
//		
//		

// PhysicStructures Function
// Private Functions for Internal use Only
void			NInitializePhysicStructures();
void			NDisablePhysicStructures();

inline void		NSetAirFriction(const Nf32 f){fAirFriction = f;}
inline Nf32		NGetAirFriction(){return fAirFriction;}

NSTRUCTURE*		NSetupPhysicStructure(NSTRUCTURE *pstructure, const NSTRUCTURE_DESC *builder);
NSTRUCTURE*		NCreatePhysicStructure( const NSTRUCTURE_DESC *builder);
void			NClearPhysicStructure(NSTRUCTURE *pstructure);
void			NDeletePhysicStructure(NSTRUCTURE *pstructure);

// void			NErasePhysicStructureAllContents(NSTRUCTURE *ps); Dangerous !
void			NErasePhysicStructureElementArray(NSTRUCTURE *ps, NSTRUCTURE_ELEMENT_TYPE_ENUM element_type );
inline void		NErasePhysicStructureJointArray(NSTRUCTURE *ps){NEraseArray(&ps->JointArray,NULL);}
inline void		NErasePhysicStructureSpringArray(NSTRUCTURE *ps){NEraseArray(&ps->SpringArray,NULL);}			
inline void		NErasePhysicStructureSimpleConstraintArray(NSTRUCTURE *ps){NEraseArray(&ps->SimpleConstraintArray,NULL);}			
inline void		NErasePhysicStructureMinMaxConstraintArray(NSTRUCTURE *ps){NEraseArray(&ps->MinMaxConstraintArray,NULL);}			
inline void		NErasePhysicStructureSpecialConstraintArray(NSTRUCTURE *ps){NEraseArray(&ps->SpecialConstraintArray,NULL);}			

void			NCopyPhysicStructure(NSTRUCTURE *pdst,const NSTRUCTURE *psrc);
NXNODELIST*		NBindStructureCollisionCoupleXNodeList(NXNODELIST *pxlist);

// Euler CallBack functions
void			NPhysicStructureDefault_EulerAccumulateForces(NSTRUCTURE *ps,const Nf32 fdelay);
void			NPhysicStructureDefault_EulerIntegration(NSTRUCTURE *ps,const Nf32 fdelay);
void			NPhysicStructureDefault_EulerSatisfyConstraints(NSTRUCTURE *ps,const Nf32 fdelay);

// Verlet CallBack functions
void			NPhysicStructureDefault_VerletAccumulateForces(NSTRUCTURE *ps,const Nf32 fdelay);
void			NPhysicStructureDefault_VerletIntegration(NSTRUCTURE *ps,const Nf32 fdelay);
void			NPhysicStructureDefault_VerletSatisfyConstraints(NSTRUCTURE *ps,const Nf32 fdelay);

// Joints and Constraint function
void			NCreateJoint(NSTRUCTURE *ps, const NJOINT_DESC *builder);
void			NCreateJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints, const NJOINT_DESC *builder,const NVEC3 *pshift);
NCONSTRAINT*	NCreateConstraint(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 indexA, const Nu32 indexB);
// void			NCreateConstraintsRange(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 firstjoint, const Nu32 stride );
void			NCreateConstraintStrip(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 firstjoint_index, const Nu32 offset_to_next_joint );
void			NCreateConstraintLoop(NSTRUCTURE *ps, const NCONSTRAINT_DESC *builder, const Nu32 number_of_constraints, const Nu32 firstjoint_index, const Nu32 offset_to_next_joint );
void			NCreateConstraintPattern(NSTRUCTURE *ps, const NCONSTRAINT_DESC *pcdesc, const NCONSTRAINT_PATTERN *ppattern, const Nu32 pattern_size,const Nu32 offset_to_next_pattern, const Nu32 repetition, const Nu32 joint_index_origin );
void			NCreateConstraintPatternSeries(NSTRUCTURE *ps, const NCONSTRAINT_DESC *pcdesc, const NCONSTRAINT_PATTERN *ppattern, const Nu32 pattern_size,const Nu32 offset_to_next_pattern, const Nu32 repetition, const Nu32 joint_index_origin, const Nu32 serie_offset_to_next_joint_index_origin, const Nu32 serie_number );



inline	Nbool	NArrayDeleteConstraint(NARRAY *pconstraintarray,const Nu32 id){return NEraseArrayElement(pconstraintarray,id,NULL);}

inline	void	NAddForceToJoint(NJOINT *pj,const NVEC3 *pf){ NVec3AddTo(&pj->vForcesAccumulator,pf); }
inline	void	NAddForceToStructureHead(NSTRUCTURE *pstructure,const NVEC3 *pf){NVec3AddTo(&((NJOINT*)(pstructure->JointArray.pFirst))->vForcesAccumulator,pf);}
inline	void	NSubForceFromJoint(NJOINT *pj,const NVEC3 *pf){ NVec3SubFrom(&pj->vForcesAccumulator,pf); }
inline	void	NSubForceFromStructureHead(NSTRUCTURE *pstructure,const NVEC3 *pf){NVec3SubFrom(&((NJOINT*)(pstructure->JointArray.pFirst))->vForcesAccumulator,pf);}

inline	void	NFreezeJoint(NJOINT *pj){FLAG_ON(pj->Flags,FLAG_JOINT_FIXED);}
inline	void	NFreezeAllJoints(NSTRUCTURE *ps){Nu32 i;NJOINT *pj;pj=(NJOINT*)NGetFirstArrayPtr(&ps->JointArray);for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++){FLAG_ON(pj->Flags,FLAG_JOINT_FIXED);}}
void			NFreezeJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints_first_included, const Nu32 firstjoint, const Ns32 stride );
Nu32			NFreezeJointsChunk(NSTRUCTURE *ps,const NJOINTSCHUNK *pchunk);

inline	void	NUnfreezeJoint(NJOINT *pj){FLAG_OFF(pj->Flags,FLAG_JOINT_FIXED);}
inline	void	NUnfreezeAllJoints(NSTRUCTURE *ps){Nu32 i;NJOINT *pj;pj=(NJOINT*)NGetFirstArrayPtr(&ps->JointArray);for(i=NGetArraySize(&ps->JointArray);i!=0;i--,pj++){FLAG_OFF(pj->Flags,FLAG_JOINT_FIXED);}}
void			NUnfreezeJointsRange(NSTRUCTURE *ps, const Nu32 number_of_joints_first_included, const Nu32 firstjoint, const Ns32 stride );
Nu32			NUnfreezeJointsChunk(NSTRUCTURE *ps,const NJOINTSCHUNK *pchunk);

inline	NJOINT*	NGetJointPtr(NSTRUCTURE *ps, const Nu32 id){return ((NJOINT*)NGetArrayPtr(&ps->JointArray,id));}
inline	NJOINT*	NGetFirstJointPtr(const NSTRUCTURE *ps){return ((NJOINT*)NGetFirstArrayPtr(&ps->JointArray));}
inline	NJOINT*	NGetLastJointPtr(const NSTRUCTURE *ps){return ((NJOINT*)NGetLastArrayPtr(&ps->JointArray));}
inline	Nu32	NGetNbJoints(const NSTRUCTURE *ps){return (NGetArraySize(&ps->JointArray));}

inline	void	NGetJointPosition(const NJOINT *pj,NVEC3 *v){*v = pj->CurrentPosition;}
inline	void	NMoveJointToPosition(NJOINT *pj,const NVEC3 *v){pj->CurrentPosition = *v;}
inline	void	NSetJointMass(NJOINT *pj,const Nf32 fmass){pj->fMass=fmass;}
inline	void	NSetJointRadius(NJOINT *pj, const Nf32 fradius){pj->fRadius=fradius;}
inline	void	NSetIndexedJointMass(NSTRUCTURE *ps,const Nu32 i,const Nf32 fmass){ ((NJOINT*)(ps->JointArray.pFirst))[i].fMass = fmass;}
inline	void	NSetJointPosition(NJOINT *pj,const NVEC3 *v){pj->CurrentPosition=*v;pj->OldPosition=*v;}
inline	void	NSetJointPositionf(NJOINT *pj,const Nf32 x, const Nf32 y, const Nf32 z){NVec3Set(&pj->CurrentPosition,x,y,z);pj->OldPosition=pj->CurrentPosition;}
// inline	void	NSetStructureHeadMass(NSTRUCTURE *ps,const Nf32 fmass){((NJOINT*)(ps->JointArray.pFirst))->fMass = fmass;}
// inline	void	NSetStructureTailMass(NSTRUCTURE *ps,const Nf32 fmass){((NJOINT*)(ps->JointArray.pFirst))[ps->JointArray.Size - 1].fMass = fmass; }
void			NSetStructureJointRangeRadius(NJOINT*pfirstjoint,const Nu32 range_size,const Nf32 scale_factor, const Nf32 minradius, const Nf32 maxradius, const NFASTANIMATEDVALUE*ptable);
void			NMovePhysicStructure(NSTRUCTURE *ps,const NVEC3 *pv);
void			NSetPhysicStructurePosition(NSTRUCTURE *ps, const Nu32 ref_jointid, const NVEC3 *pv);
void			NResetAllJointsDynamic(NSTRUCTURE *ps);
inline void		NResetJointDynamic(NJOINT* pj){ pj->OldPosition = pj->CurrentPosition; NVec3Set(&pj->vSpeed,0.0f,0.0f,0.0f); NVec3Set(&pj->vForcesAccumulator,0.0f,0.0f,0.0f); }; 

void 			NUseCurrentSpringsLengthAsRestLength(NSTRUCTURE *pstructure, const Nf32 length_ratio);
void 			NUseCurrentSimpleConstraintLengthAsLength(NSTRUCTURE *pstructure, const Nf32 length_ratio);
void			NUseCurrentSpecialConstraintLengthAsLengths(NSTRUCTURE *pstructure, const Nf32 length_minratio,const Nf32 length_maxratio);
void 			NUseCurrentMinMaxConstraintLengthAsLengths(NSTRUCTURE *pstructure, const Nf32 length_minratio,const Nf32 length_maxratio);


void			NDeletePhysicStructureGeometry(NSTRUCTURE *pstructure, NGEOMETRY *pgeometry);
void			NDeleteEmptyPhysicStructureGeometries(NSTRUCTURE *pstructure);

// Private use
void			NUpdatePhysicStructures(const NTIME *ptime);
inline void		NBind_Physic_EulerFunctions(NSTRUCTURE *pstructure)
{
	pstructure->pAccumulateForce		= NPhysicStructureDefault_EulerAccumulateForces;
	pstructure->pIntegration			= NPhysicStructureDefault_EulerIntegration;
	pstructure->pSatisfyConstraint		= NPhysicStructureDefault_EulerSatisfyConstraints;
}

inline void		NBind_Physic_VerletFunctions(NSTRUCTURE *pstructure)
{
	pstructure->pAccumulateForce		= NPhysicStructureDefault_VerletAccumulateForces;
	pstructure->pIntegration			= NPhysicStructureDefault_VerletIntegration;
	pstructure->pSatisfyConstraint		= NPhysicStructureDefault_VerletSatisfyConstraints;
}

void			NRenderableUpdate_PhysicStructure(NRENDERABLE *prenderable, void* powner, const NTIME *ptime);
// ***************************************************************************************
// **								SKIN												**
// **							  FUNCTIONS												**
// ***************************************************************************************
NSKIN*			NCreateSkin_Rope(NSTRUCTURE *pstruct, NGEOMETRY	*pgeom, const NSKIN_DESC *pbuilder );
NSKIN*			NCreateSkin_Grid(NSTRUCTURE *pstruct, NGEOMETRY	*pgeom, const NSKIN_DESC *pbuilder );
NSKIN*			NCreateSkin_Billboard(NSTRUCTURE *pstruct, NGEOMETRY *pgeom, const NSKIN_DESC *pbuilder );

NUPDATESKIN_FCT	NSetSkinUpdateFunction(NSKIN *pskin, const NSKIN_SCHEME sh);
NSKIN_SCHEME	NGetSkinSheme(const NUPDATESKIN_FCT fct);
void			NDeleteSkin(NSTRUCTURE *pstructure, NSKIN *pskin);

void			NErasePhysicStructureSkinArray(NSTRUCTURE *ps);

//void			NSetSkinUVs(NSKIN *pskin,const NTEXCOORD_2f *puvorigin,const NTEXCOORD_2f *puvtiling, const Nu16 flags); // Re-calculate Skin UVs with new incoming parameters
/*
void			DeleteSkin(SKIN *pskin);
void			Skins_Update();
*/

inline	void	NUpdateStructureSkins(NSTRUCTURE *ps){NSKIN *psk = (NSKIN*)NGetFirstArrayPtr(&ps->SkinArray); for( Nu32 i=NGetArraySize(&ps->SkinArray);i!=0;i--,psk++){psk->common.pUpdateSkinGeometry(psk);}}
void			NUpdateSkin_Rope_Quads_DoubleRows(NSKIN* pskin);
void			NUpdateSkin_Rope_Quads_OneRow(NSKIN* pskin);
void			NUpdateSkin_Rope_Triangles_OneRow(NSKIN* pskin);
void			NUpdateSkin_Rope_Triangles_DoubleRows(NSKIN* pskin);
void			NUpdateSkin_Grid_Quads(NSKIN* pskin);
void			NUpdateSkin_Grid_FanQuads(NSKIN* pskin);
void			NUpdateSkin_Billboard_Quad(NSKIN* pskin);

void			NSkinDestructor_ArrayCallBack(void* pel);

// NStructure Collision Couple
NSTRUCTURE_COLLISONCOUPLE*	NSetUpStructureCollisionCouple(NSTRUCTURE_COLLISONCOUPLE* pcol, NSTRUCTURE *p1,NSTRUCTURE *p2, NSTRUCTURE_COLLISION_RESPONSE_FCT collisionResponse_callback);
NSTRUCTURE_COLLISONCOUPLE*	NCreateStructureCollisionCouple(NSTRUCTURE *p1,NSTRUCTURE *p2, NSTRUCTURE_COLLISION_RESPONSE_FCT collisionResponse_callback);
void						NClearStructureCollisionCouple(void *pcol);
void						NDeleteStructureCollisionCouple(void *pcol);

Nu32						NDeleteAll_StructureCollisionCouplesXNode_LinkedWith_PhysicStructure(NXNODELIST *pcolcouplexlist,const NSTRUCTURE *ps, NNODE_X_DESTRUCTOR_CALLBACK  xclear);
inline void					NDeleteAll_StructureCollisionCouplesXNode(NXNODELIST* pxnlist,NNODE_X_DESTRUCTOR_CALLBACK nxn_X_destructor){NDeleteXNodeList(pxnlist, nxn_X_destructor);}

void						NCheckStructuresCollision();

inline	void				NStructureCollisionResult_Extract_CollisionPosition(NVEC3 *pposition, const NSTRUCTURE_COLLISION_RESULT *pr){NVec3Lerp(pposition, &pr->pjA->CurrentPosition, &pr->pjB->CurrentPosition,pr->SeparatingDistance/pr->SeparatingLimit);}
NAABB*						NGetStructureJointsAABB(NAABB *paabb, const NSTRUCTURE *pstruct);
void						NUpdateStructureAABB(NSTRUCTURE *pstruct);
Nbool						NStructureCollisionAABBvsAABB(const NSTRUCTURE *psA,const NSTRUCTURE *psB);




#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSTRUCTURE_H 