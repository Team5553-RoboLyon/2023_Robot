#include "../NCStandard.h"
#include "../NType.h"
#include "../NFlags.h"
#include "../Containers/NNode.h"
#include "../NErrorHandling.h"
#include "../NStructure.h"
#include "NUT_Structure.h"

static inline Nu32 _is_IJ_center( const NUT_STRCT_2DGRID_DESC *pstrct_desc )
{
	// Is there any Center joints to build ? Check that deeply into each of the flags descriptions.
	// The rules are actually really simple: 
	// Center joints are IMPLICITLY requested in two ways...
	//	-From Diagonal : If the flag FLAG_NUT_STRCT_DIAGONAL_SPLIT is set to ON. A Center joint is going to be created !
	//					 Note, Only one center is going to be created, so if a center already exist because asked by some other ways (NUT_STRCT_JOINT_IJCENTER Edges)
	//					 Is not going to be created twice.
	//
	//	-From EdgeFlags[NUT_STRCT_JOINT_IJCENTER][...]: if some constraint flags are set to ON, that means user asks to create constraint between center joints, so
	//													that means, implicitly, user wants Center joints !!!

	// if flag FLAG_NUT_STRCT_DIAGONAL_SPLIT is set to ON, Check if there are diagonal to split if not, CRASH ...
	NErrorIf(ISFLAG_ON(pstrct_desc->BuildFlags_Diagonal,FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT) && ((pstrct_desc->BuildFlags_Diagonal&MASK_NUT_STRCT_BUILD_CONSTRAINTS) == 0),NERROR_GEOMETRY_INCONSISTENT_PARAMETER);
	
	if( ISFLAG_ON(pstrct_desc->BuildFlags_Diagonal,FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT) )
		return 1;

	if(	(pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_I]&MASK_NUT_STRCT_BUILD_CONSTRAINTS) != 0 ||
		(pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_J]&MASK_NUT_STRCT_BUILD_CONSTRAINTS) != 0 )
		return 1;

	return 0;
}
static inline Nu32 _get_corner_joint_number(const NUT_STRCT_2DGRID_DESC *pstrct_desc)
{
	return (pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1)*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]+1);
}
static inline Nu32 _get_ijcenter_joint_number(const NUT_STRCT_2DGRID_DESC *pstrct_desc)
{
	if( _is_IJ_center(pstrct_desc) )
	{
		return (pstrct_desc->ConstraintsNb[NUT_STRCT_I])*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]);
	}
	else
	{
		return 0;
	}
}
// ------------------------------------------------------------------------------------------
// void NUT_Extract_Structure2D_Grid_Elements_Numbers
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Extract_Structure2D_Grid_Elements_Numbers(const NUT_STRCT_2DGRID_DESC *pstrct_desc, Nu32 *presult_table, const NSTRUCTURE_ELEMENT_TYPE_ENUM first_element, const Nu32 range_size )
{
	Nu32	a;
	Nu32	element[NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE];

	NErrorIf(!presult_table, NERROR_NULL_POINTER);
	NErrorIf( (first_element+range_size) > NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE, NERROR_INVALID_PARAMETER);
	NErrorIf(ISFLAG_ON(pstrct_desc->BuildFlags_Diagonal,FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT) && ((pstrct_desc->BuildFlags_Diagonal&MASK_NUT_STRCT_BUILD_CONSTRAINTS) == 0),NERROR_GEOMETRY_INCONSISTENT_PARAMETER);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NUMBER OF JOINTS
	// Base
	element[NSTRUCTURE_ELEMENT_JOINT]	= _get_corner_joint_number(pstrct_desc);
	// Additional joints (Center of Cell Joints)
	element[NSTRUCTURE_ELEMENT_JOINT]  += _get_ijcenter_joint_number(pstrct_desc);
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NUMBER OF CONSTRAINTS
	// simple reset 
	element[NSTRUCTURE_ELEMENT_SPRING]				= 0;
	element[NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT]	= 0;
	element[NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT]	= 0;
	element[NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT]	= 0;

	for(a=0;a<4;a++)
	{
		// I and J Edges
		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_CORNER][NUT_STRCT_I]&(1<<a) )
			element[a]+= (pstrct_desc->ConstraintsNb[NUT_STRCT_I])*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]+1);
		
		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_CORNER][NUT_STRCT_J]&(1<<a) )
			element[a]+= (pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1)*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]);

		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_I]&(1<<a) )
			element[a]+= (pstrct_desc->ConstraintsNb[NUT_STRCT_I]-1)*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]);

		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_J]&(1<<a) )
			element[a]+= (pstrct_desc->ConstraintsNb[NUT_STRCT_I])*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]-1);

		// 2D Diagonals (2 Diagonals build for each cell )
		if( pstrct_desc->BuildFlags_Diagonal & (1<<a) )
		{
			if( ISFLAG_ON(pstrct_desc->BuildFlags_Diagonal,FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT) )
			{
				// 2 split diagonals = 4 constraints to build !
				element[a]+= 4*(pstrct_desc->ConstraintsNb[NUT_STRCT_I])*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]);
			}
			else
			{
				// 2 diagonals = 2 constraints to build !
				element[a]+= 2*(pstrct_desc->ConstraintsNb[NUT_STRCT_I])*(pstrct_desc->ConstraintsNb[NUT_STRCT_J]);
			}
		}
	}

	// copy requested results
	memcpy(presult_table,&element[first_element],range_size*sizeof(Nu32));
}
// ------------------------------------------------------------------------------------------
// void NUT_Build_Structure_2DGrid
// ------------------------------------------------------------------------------------------
// Description :
//	Create the Joints of a 2D Grid Structure.
//	Joints creation order is very important.
//	It will be done always like this:
//
//		Create a range of n+1 JOINTS along I AXIS ---> Joint(0) > Joint(1) > Joint(2) ... with n = number of constraint along I AXIS 
//			Then 
//		shift one step along J AXIS
//			Then 
//		Repeat k+1 times (k =  number of constraint along J AXIS )
//			
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NUT_Build_Structure_2DGrid_Joints( NSTRUCTURE *pstructure,const NUT_STRCT_2DGRID_DESC *pstrct_desc )
{
	Nu32				a;
	NVEC3			v;
	Nu32				first_corner,first_ijcenter;
	NJOINT_DESC			joint_desc;

	// Joint DESC global parameters
	Nmem0(&joint_desc,NJOINT_DESC);
	// NVec3Set(&joint_desc.vSpeed,0,0,0); useless ! because of 'NMem0'
	if( ISFLAG_ON(pstrct_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTRADIUS) )
	{
		joint_desc.fRadius	= pstrct_desc->JointRadius;
	}
	else
	{
		joint_desc.fRadius	= DEFAULT_NUT_STRCT_2DGRID_DESC_JOINTRADIUS;
	}
	if( ISFLAG_ON(pstrct_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTMASS) )
	{
		joint_desc.fMass	= pstrct_desc->JointMass;
	}
	else
	{
		joint_desc.fMass	= DEFAULT_NUT_STRCT_2DGRID_DESC_JOINTMASS;
	}

	// Keep first joint index in mind (as first corner)
	first_corner = pstructure->JointArray.Size;

	// I and J
	// Create Joints by 'I range' and repeat that range creation along J Axis 
	v = pstrct_desc->BuildOrigin;
	for( a=pstrct_desc->ConstraintsNb[NUT_STRCT_J]+1;a!=0;a--, NVec3AddTo(&v,&pstrct_desc->BuildAxis[NUT_STRCT_J]) )
	{
		joint_desc.CurrentPosition	= v;
		joint_desc.OldPosition		= v;
		// Generate an error if capacity is not already setup ! ( it would work, thanks to the array capacity management, but we want thinks in that way ...  
		NErrorIf(NGetNbJoints(pstructure)+pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1 > NGetArrayCapacity(&pstructure->JointArray),NERROR_ARRAY_WRONG_CAPACITY);
		NCreateJointsRange( pstructure,pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1,&joint_desc,&pstrct_desc->BuildAxis[NUT_STRCT_I] );
	}

	// IJ Cell Center
	// These 'center joints' are going to be inserted at the end of joints array
	if( _is_IJ_center(pstrct_desc) )
	{
		// Keep first joint index in mind (as first IJ Center)
		first_ijcenter = pstructure->JointArray.Size;

		v.x = pstrct_desc->BuildOrigin.x + 0.5f*(pstrct_desc->BuildAxis[NUT_STRCT_I].x + pstrct_desc->BuildAxis[NUT_STRCT_J].x);
		v.y = pstrct_desc->BuildOrigin.y + 0.5f*(pstrct_desc->BuildAxis[NUT_STRCT_I].y + pstrct_desc->BuildAxis[NUT_STRCT_J].y);
		v.z = pstrct_desc->BuildOrigin.z + 0.5f*(pstrct_desc->BuildAxis[NUT_STRCT_I].z + pstrct_desc->BuildAxis[NUT_STRCT_J].z);

		// Create Joints by 'I range' and repeat that range creation along J Axis 
		for( a=pstrct_desc->ConstraintsNb[NUT_STRCT_J];a!=0;a--,NVec3AddTo(&v,&pstrct_desc->BuildAxis[NUT_STRCT_J]) )
		{
			joint_desc.CurrentPosition	= v;
			joint_desc.OldPosition		= v;
			// Generate an error if capacity is not already setup ! ( it would work, thanks to the array capacity management, but we want thinks in that way ...  
			NErrorIf(NGetNbJoints(pstructure)+pstrct_desc->ConstraintsNb[NUT_STRCT_I] > NGetArrayCapacity(&pstructure->JointArray),NERROR_ARRAY_WRONG_CAPACITY);
			NCreateJointsRange( pstructure,pstrct_desc->ConstraintsNb[NUT_STRCT_I],&joint_desc,&pstrct_desc->BuildAxis[NUT_STRCT_I] );
		}
	}

	return first_corner;
}

// ------------------------------------------------------------------------------------------
// void NUT_Build_Structure_2DGrid_Constraints
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Build_Structure_2DGrid_Constraints( NSTRUCTURE *pstructure,const NUT_STRCT_2DGRID_DESC *pstrct_desc, const Nu32 first_joint, const Nu8 builded_constraints_mask )
{
	Nu32				a,b;
	Nu32				strip_first_joint;
	Nu32				first_corner,first_ijcenter;
	NCONSTRAINT_PATTERN constraint_pattern[4];

	// User is suppose to use flags 'FLAG_NUT_STRCT_BUILD_(constraint)' only 
	NErrorIf((builded_constraints_mask&MASK_NUT_STRCT_BUILD_CONSTRAINTS) != builded_constraints_mask, NERROR_INCONSISTENT_FLAGS);
	// Purpose of this function is not joint creation. All of them have to be previously inserted into the Physic Structure. 
	// So, let's check there is enough joints to work ...
	
	NErrorIf( (first_joint + _get_corner_joint_number(pstrct_desc) + _get_ijcenter_joint_number(pstrct_desc) - 1 ) >= pstructure->JointArray.Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	//NErrorIf( (first_joint + _get_corner_joint_number(pstrct_desc) + _get_ijcenter_joint_number(pstrct_desc) ) >= pstructure->JointArray.Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);

	// Keep first joint index in mind (as first corner)
	first_corner	= first_joint;
	first_ijcenter	= first_joint + _get_corner_joint_number(pstrct_desc); 

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + CONSTRAINTS CREATION
// +
	for(a=0;a<4;a++)
	{
		// If building is not explicitly requested by user ... specified constraints are not builded.
		 if( !(builded_constraints_mask&(1<<a)) )
			 continue;
		
		//We have to ...  "xxx.Constraint_Type" has to be consistent with the ConstraintDesc Order ... !!!
		// And at the same time ... pstrct_desc is const .... but, any way, in that case, do it like this !
		((NUT_STRCT_2DGRID_DESC*)pstrct_desc)->ConstraintDesc[a].Constraint_Type = (NSTRUCTURE_ELEMENT_TYPE_ENUM)a;
		
		// I Edges
		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_CORNER][NUT_STRCT_I]&(1<<a) )
		{
			strip_first_joint = first_corner;
			for(b=pstrct_desc->ConstraintsNb[NUT_STRCT_J]+1;b!=0;b--,strip_first_joint+=pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1)
			{
				NCreateConstraintStrip(pstructure,&pstrct_desc->ConstraintDesc[a],pstrct_desc->ConstraintsNb[NUT_STRCT_I],strip_first_joint,1);
			}
		}
		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_I]&(1<<a) )
		{
			strip_first_joint = first_ijcenter;
			for(b=pstrct_desc->ConstraintsNb[NUT_STRCT_J];b!=0;b--,strip_first_joint+=pstrct_desc->ConstraintsNb[NUT_STRCT_I])
			{
				NCreateConstraintStrip(pstructure,&pstrct_desc->ConstraintDesc[a],pstrct_desc->ConstraintsNb[NUT_STRCT_I]-1,strip_first_joint,1);
			}
		}
		// J Edges
		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_CORNER][NUT_STRCT_J]&(1<<a) )
		{
			strip_first_joint = first_corner;
			for(b=pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1;b!=0;b--,strip_first_joint++)
			{
				NCreateConstraintStrip(pstructure,&pstrct_desc->ConstraintDesc[a],pstrct_desc->ConstraintsNb[NUT_STRCT_J], strip_first_joint, pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1);
			}
		}

		if( pstrct_desc->BuildFlags_Edge[NUT_STRCT_JOINT_IJCENTER][NUT_STRCT_J]&(1<<a) )
		{
			strip_first_joint = first_ijcenter;
			for(b=pstrct_desc->ConstraintsNb[NUT_STRCT_I];b!=0;b--,strip_first_joint++)
			{
				NCreateConstraintStrip(pstructure,&pstrct_desc->ConstraintDesc[a],pstrct_desc->ConstraintsNb[NUT_STRCT_J]-1, strip_first_joint, pstrct_desc->ConstraintsNb[NUT_STRCT_I]);
			}
		}

		// 2D Diagonals (2 Diagonals build for each cell )
		if( pstrct_desc->BuildFlags_Diagonal & (1<<a) )
		{
			if( ISFLAG_ON(pstrct_desc->BuildFlags_Diagonal,FLAG_NUT_STRCT_BUILD_DIAGONAL_SPLIT) )
			{
				// 2 split diagonals = 4 constraints to build !
				constraint_pattern[0].JointAIndex = first_ijcenter;
				constraint_pattern[0].JointBIndex = first_corner+1;
				constraint_pattern[1].JointAIndex = first_ijcenter;
				constraint_pattern[1].JointBIndex = first_corner;
				constraint_pattern[2].JointAIndex = first_ijcenter;
				constraint_pattern[2].JointBIndex = first_corner+pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1;
				constraint_pattern[3].JointAIndex = first_ijcenter;
				constraint_pattern[3].JointBIndex = first_corner+pstrct_desc->ConstraintsNb[NUT_STRCT_I]+2;
				NCreateConstraintPatternSeries(pstructure,&pstrct_desc->ConstraintDesc[a],constraint_pattern,4,1,pstrct_desc->ConstraintsNb[NUT_STRCT_I],0,pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1,pstrct_desc->ConstraintsNb[NUT_STRCT_J]);
			}
			else
			{
				// 2 diagonals = 2 constraints to build !
				constraint_pattern[0].JointAIndex = first_corner;
				constraint_pattern[0].JointBIndex = first_corner+pstrct_desc->ConstraintsNb[NUT_STRCT_I]+2;
				constraint_pattern[1].JointAIndex = first_corner+1;
				constraint_pattern[1].JointBIndex = first_corner+pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1;
				NCreateConstraintPatternSeries(pstructure,&pstrct_desc->ConstraintDesc[a],constraint_pattern,2,1,pstrct_desc->ConstraintsNb[NUT_STRCT_I],0,pstrct_desc->ConstraintsNb[NUT_STRCT_I]+1,pstrct_desc->ConstraintsNb[NUT_STRCT_J]);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// NSTRUCTURE* NUT_SetUp2DGridStructure
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NSTRUCTURE* NUT_Setup2DGridStructure(NSTRUCTURE *pstructure,const NUT_STRCT_2DGRID_DESC *pgrid_desc)
{
	NGEOMETRY_FORMAT_ENUM				rsid;
	NSTATESET							userstatesetflags;
	NSTRUCTURE_DESC						stdesc;
	NGEOMETRY							*pgeom;
	NGEOMETRY_DESC						gdesc;
	NSKIN_DESC							skindesc;
	Nu32								capacity[NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE];

	//	0-----------3-----------6-----------9  
	//	. +		  +	. +		  +	. +		  +	.	
	//	.   +	+	.   +	+	.   +	+	.	
	//	.	 12		.	 14		.	 16		.	
	//	.	+	+	.	+	+	.	+	+	.	
	//	. +		  +	. +		  +	. +		  +	.	
	//	1-----------4-----------7-----------10
	//	. +		  +	. +		  +	. +		  +	.	
	//	.   +	+	.   +	+	.   +	+	.	
	//	.	 13		.	 15		.	 17		.	
	//	.	+	+	.	+	+	.	+	+	.	
	//	. +		  +	. +		  +	. +		  +	.	
	//	2-----------5-----------8-----------11
	//
	//		a cell		a cell	    a cell

	// Check for Flags consistency
	NErrorIf(pgrid_desc->ConstraintsNb[NUT_STRCT_I]==0 || pgrid_desc->ConstraintsNb[NUT_STRCT_J] == 0, NERROR_INVALID_PARAMETER );

	// ==========================
	// SetUp the PhysicStructure
	// ==========================
	memset(&stdesc,0,sizeof(NSTRUCTURE_DESC));
	if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_CREATESKIN) )
	{
		FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_WITH_RENDERABLE);
		FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_RENDERABLEGEOMETRY_CAPACITY);
		stdesc.RenderableGeometryCapacity	= 8;
		stdesc.SkinArrayCapacity			= 8;
		
		if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_HIGHLEVEL_EXTRACTSET) )
		{
			FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_HIGHLEVEL_EXTRACTSET);
			stdesc.HighLevelExtractSet = pgrid_desc->HighLevelExtractSet;
		}
	}

	if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_VGRAVITY) )
	{
		FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_VGRAVITY);
		stdesc.vGravity  = pgrid_desc->vGravity;
	}

	if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_JOINTSPEEDDAMPING) )
	{
		FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSPEEDDAMPING);
		stdesc.JointSpeedDamping = pgrid_desc->JointSpeedDamping;	
	}
	// It seems we need to update Bounding Sphere only for culling BUT for some other case it could be necessary to do this ...
	// So with or without culling it will be possible to update BV.
	if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB) )
	{
		FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_UPDATE_RENDERABLE_BOUNDINGVOLUME_FROM_STRUCTUREAABB);
	}
	
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SETUP STRUCTURE  (With the right capacities, no less no more ... )
	// + AND STRUCTURE ELEMENTS CREATION
	NUT_Extract_Structure2D_Grid_Elements_Numbers(pgrid_desc,capacity,NSTRUCTURE_ELEMENT_SPRING,NSTRUCTURE_ELEMENT_TYPE_ENUM_SIZE );
	stdesc.JointsArrayCapacity				= capacity[NSTRUCTURE_ELEMENT_JOINT];
	stdesc.SpringsArrayCapacity				= capacity[NSTRUCTURE_ELEMENT_SPRING];
	stdesc.SimpleConstraintsArrayCapacity	= capacity[NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT];
	stdesc.MinMaxConstraintsArrayCapacity	= capacity[NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT];
	stdesc.SpecialConstraintsArrayCapacity	= capacity[NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT];
	FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_JOINTSARRAY_CAPACITY);
	FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_SPRINGS_CAPACITY);
	FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_SIMPLE_CONSTRAINTS_CAPACITY);
	FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_MINMAX_CONSTRAINTS_CAPACITY);
	FLAG_ON(stdesc.Flags,FLAG_NSTRUCTURE_DESC_USEPARAM_SPECIAL_CONSTRAINTS_CAPACITY);
	NSetupPhysicStructure(pstructure,&stdesc);
	NUT_Build_Structure_2DGrid_Constraints( pstructure, pgrid_desc,
											NUT_Build_Structure_2DGrid_Joints( pstructure,pgrid_desc ),
											MASK_NUT_STRCT_BUILD_CONSTRAINTS );
	
	// Post Checking
	NErrorIf(pstructure->JointArray.Size != pstructure->JointArray.Capacity, NERROR_ARRAY_WRONG_SIZE );
	NErrorIf(pstructure->SpringArray.Size != pstructure->SpringArray.Capacity, NERROR_ARRAY_WRONG_SIZE );
	NErrorIf(pstructure->SimpleConstraintArray.Size != pstructure->SimpleConstraintArray.Capacity, NERROR_ARRAY_WRONG_SIZE );
	NErrorIf(pstructure->SpecialConstraintArray.Size != pstructure->SpecialConstraintArray.Capacity, NERROR_ARRAY_WRONG_SIZE );
	NErrorIf(pstructure->MinMaxConstraintArray.Size != pstructure->MinMaxConstraintArray.Capacity, NERROR_ARRAY_WRONG_SIZE );

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SKIN CREATION (if asked )
	if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_CREATESKIN) )
	{
		memset(&skindesc,0,sizeof(NSKIN_DESC));

		if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_GEOMETRY_FORMAT) )
		{
			rsid = pgrid_desc->GeometryFormat;
		}
		else
		{
			rsid = DEFAULT_NUT_STRCT_2DGRID_DESC_GEOMETRY_FORMAT;
		}
		if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_USERSTATESET) )
		{
			userstatesetflags = pgrid_desc->UserStateSet;
		}
		else
		{
			userstatesetflags = DEFAULT_NUT_STRCT_2DGRID_DESC_USERSTATESET;
		}

		if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_CREATESKIN_USING_CELLJOINT_CENTERS) )
		{

			NErrorIf( !_is_IJ_center(pgrid_desc),NERROR_INCONSISTENT_PARAMETERS );
			if( _is_IJ_center(pgrid_desc) )
			{
				NFillupGeometryDesc(&gdesc,rsid, userstatesetflags,
									pgrid_desc->ConstraintsNb[NUT_STRCT_I]*pgrid_desc->ConstraintsNb[NUT_STRCT_J]*4,	// Nb Triangles
									capacity[NSTRUCTURE_ELEMENT_JOINT]);												// Nb Vertex
				skindesc.Grid.Structure_Scheme = NSKIN_SCHEME_GRID_FANQUADS;
			}
		}
		else
		{
			NFillupGeometryDesc(&gdesc,rsid,userstatesetflags,
								pgrid_desc->ConstraintsNb[NUT_STRCT_I]*pgrid_desc->ConstraintsNb[NUT_STRCT_J]*2,	// Nb Triangles
								capacity[NSTRUCTURE_ELEMENT_JOINT]);														// Nb Vertex
			skindesc.Grid.Structure_Scheme = NSKIN_SCHEME_GRID_QUADS;
		}

		skindesc.Grid.Ref_First					= 0;
		skindesc.Grid.Ref_RangeSize				= capacity[NSTRUCTURE_ELEMENT_JOINT];
		skindesc.Grid.I_CellsNb					= pgrid_desc->ConstraintsNb[NUT_STRCT_I];
		skindesc.Grid.J_CellsNb					= pgrid_desc->ConstraintsNb[NUT_STRCT_J];
		skindesc.Grid.Ref_StructureElementType	= NSTRUCTURE_ELEMENT_JOINT;

		skindesc.Grid.TexCoordBuild				= pgrid_desc->TexCoordBuild;
		skindesc.Grid.ColorBuild				= pgrid_desc->ColorBuild;

		pgeom = NGeometryEmplaceBack(pstructure->pRenderable,&gdesc);
		if( ISFLAG_ON(pgrid_desc->Flags,FLAG_NUT_STRCT_2DGRID_DESC_USEPARAM_APPEARANCE) )
			NSetGeometryAppearance(pgeom,&pgrid_desc->Appearance);

		NCreateSkin_Grid(pstructure,pgeom,&skindesc);
	}
	return pstructure;
}
// ------------------------------------------------------------------------------------------
// NSTRUCTURE* NUT_Create2DGridStructure
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NSTRUCTURE* NUT_Create2DGridStructure(const NUT_STRCT_2DGRID_DESC *p2dgriddesc )
{
	return NUT_Setup2DGridStructure(NEW(NSTRUCTURE),p2dgriddesc);
}


