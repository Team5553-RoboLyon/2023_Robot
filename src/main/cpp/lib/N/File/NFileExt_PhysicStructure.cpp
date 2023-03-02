#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "../Containers/NArray.h"
#include "lib/N/File/NFile.h"
#include "../File/NFileExt.h"
#include "lib/N/NErrorHandling.h"
#include "../Render/Renderable/NRenderable.h"
// #include "../Render/Renderable/NRenderable_UpdateAndExtract.h"
#include "../Render/NFrustum_Culling.h"

static Nu32 _writeconstraint(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	NHEADER_CONSTRAINT header;
	NCONSTRAINT *pc;
	NARRAY *pjointarray;

	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_CONSTRAINT) != 1)
		return 0;

	pjointarray = (NARRAY *)userdata;
	pc = (NCONSTRAINT *)ptr;

	memset(&header, 0, sizeof(NHEADER_CONSTRAINT));
	header.Ref_jA = NGetArrayIndex(pjointarray, (NBYTE *)pc->pjA);
	header.Ref_jB = NGetArrayIndex(pjointarray, (NBYTE *)pc->pjB);
	header.AB = pc->AB;
	header.f[0] = pc->f[0];
	header.f[1] = pc->f[1];
	header.f[2] = pc->f[2];
	if (NFileWrite(pfile, &header, sizeof(NHEADER_CONSTRAINT), 1) != 1)
		return 0;

	return 1;
}
static Nu32 _readconstraint(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	NHEADER_CONSTRAINT header;
	NCONSTRAINT *pc;
	NARRAY *pjointarray;
	Nu32 version;

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current Main Version (all versions 0.0.x)
	case NGETVERSION_MAIN(VERSION_NHEADER_CONSTRAINT):
		pjointarray = (NARRAY *)userdata;
		pc = (NCONSTRAINT *)ptr;

		memset(&header, 0, sizeof(NHEADER_CONSTRAINT));
		if (NFileRead(pfile, &header, sizeof(NHEADER_CONSTRAINT), 1) != 1)
			return 0;

		pc->pjA = (NJOINT *)NGetArrayPtr(pjointarray, header.Ref_jA);
		pc->pjB = (NJOINT *)NGetArrayPtr(pjointarray, header.Ref_jB);
		pc->AB = header.AB;
		pc->f[0] = header.f[0];
		pc->f[1] = header.f[1];
		pc->f[2] = header.f[2];
		return 1;

	// Unknown version:
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}

/*

void _write_skin_rop_vertebra(NBYTE *ptr,NFILE *pfile,Nu32 userdata)
{
/ *
	typedef struct
	{
		Nf32	SideA;
		Nf32	SideB;
		Nf32	Bulge;
		Nf32	BulgeShifting;	// on the fly calculated data
	}NSKIN_ROPE_VERTEBRA;
* /
	// Purpose of this callback is to only write the first 3 Nf32 of the structure ...
	// The last one ( the 4th Nf32 is not supposed to be saved/loaded )
	// So, let's do this in the simplest way
	NFileWrite(pfile,ptr,sizeof(Nf32),3);
}

void _read_skin_rop_vertebra(NBYTE *ptr,NFILE *pfile,Nu32 userdata)
{
/ *
	typedef struct
	{
		Nf32	SideA;
		Nf32	SideB;
		Nf32	Bulge;
		Nf32	BulgeShifting;	// on the fly calculated data
	}NSKIN_ROPE_VERTEBRA;
* /
	// Purpose of this callback is to only read the first 3 Nf32 of the structure ...
	// The last one ( the 4th Nf32 is not supposed to be saved/loaded )
	// So, let's do this in the simplest way
	NFileRead(pfile,ptr,sizeof(Nf32),3);
	((NSKIN_ROPE_VERTEBRA*)ptr)->BulgeShifting = 0.0f; // And setup the 4th Nf32 to ZERO.
}
*/

// static Nu32	_skinversionbytype[NSKIN_TYPE_ENUMSIZE] = {VERSION_NNHEADER_SKINBILLBOARD,VERSION_NNHEADER_SKINROPE,VERSION_NNHEADER_SKINGRID};
static Nu32 _writeskin(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	NARRAY *pref_array;
	NHEADER_SKIN header;
	NSKIN *pskin;
	NSTRUCTURE *ps;

	// Write Current version Tag
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_SKIN) != 1)
		return 0;

	ps = (NSTRUCTURE *)userdata;
	pskin = (NSKIN *)ptr;

	memset(&header, 0, sizeof(NHEADER_SKIN));
	header.Common.SkinType = pskin->common.SkinType;
	header.Common.Ref_StructureElementType = pskin->common.StructureElementType;
	header.Common.Geometry = NGetArrayIndex(&ps->pRenderable->GeomArray, (NBYTE *)pskin->common.pGeometry);
	header.Common.Ref_FirstVertex = NGetArrayIndex(&pskin->common.pGeometry->Mesh.VertexArray, (NBYTE *)pskin->common.pFirstVertex);
	header.Common.SkinScheme = NGetSkinSheme(pskin->common.pUpdateSkinGeometry);

	switch (header.Common.Ref_StructureElementType)
	{
	case NSTRUCTURE_ELEMENT_JOINT:
		pref_array = &ps->JointArray;
		break;
	case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
		pref_array = &ps->MinMaxConstraintArray;
		break;
	case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
		pref_array = &ps->SimpleConstraintArray;
		break;
	case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
		pref_array = &ps->SpecialConstraintArray;
		break;
	case NSTRUCTURE_ELEMENT_SPRING:
		pref_array = &ps->SpringArray;
		break;
	default:
		NErrorIf(1, NERROR_INVALID_CASE);
		break;
	}

	switch (header.Common.SkinType)
	{
	case NSKIN_TYPE_BILLBOARD:
		header.BillBoard.fScale = pskin->BillBoard.fScale;
		header.BillBoard.RatioWidth_Height = pskin->BillBoard.RatioWidth_Height;
		header.BillBoard.Ref_Center = NGetArrayIndex(pref_array, (NBYTE *)pskin->BillBoard.pCenter);
		header.BillBoard.Ref_Orient_A = NGetArrayIndex(pref_array, (NBYTE *)pskin->BillBoard.pOrient_A);
		header.BillBoard.Ref_Orient_B = NGetArrayIndex(pref_array, (NBYTE *)pskin->BillBoard.pOrient_B);
		header.BillBoard.VShift = pskin->BillBoard.VShift;
		break;

	case NSKIN_TYPE_ROPE:
		header.Rope.Ref_First = NGetArrayIndex(pref_array, (NBYTE *)pskin->Rope.pFirst);
		header.Rope.Ref_RangeSize = pskin->Rope.RangeSize;
		//			header.Rope.TwistBulgeFactor= pskin->Rope.TwistBulgeFactor;
		break;

	case NSKIN_TYPE_GRID:
		header.Grid.Ref_First = NGetArrayIndex(pref_array, (NBYTE *)pskin->Grid.pFirst);
		header.Grid.Ref_RangeSize = pskin->Grid.RangeSize;
		header.Grid.I_CellsNb = pskin->Grid.I_CellsNb;
		header.Grid.J_CellsNb = pskin->Grid.J_CellsNb;
		break;

	default:
		NErrorIf(1, NERROR_INVALID_CASE);
		return 0;
	}

	if (NFileWrite(pfile, &header, sizeof(NHEADER_SKIN), 1) != 1)
		return 0;

	// ... And After All, save the Vertebras Array, Only  if NSKIN is a Rope.
	if (header.Common.SkinType == NSKIN_TYPE_ROPE)
	{
		if (!NFileWriteArray(pfile, &pskin->Rope.VertebrasArray, NULL, 0))
			return 0;
	}

	return 1;
}

static Nu32 _readskin(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	NARRAY *pref_array;
	NHEADER_SKIN header;
	NSKIN *pskin;
	NSTRUCTURE *ps;
	Nu32 version;

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current Main Version (all versions 0.0.x )
	case NGETVERSION_MAIN(VERSION_NHEADER_SKIN):
		ps = (NSTRUCTURE *)userdata;
		pskin = (NSKIN *)ptr;

		memset(&header, 0, sizeof(NHEADER_SKIN));
		if (NFileRead(pfile, &header, sizeof(NHEADER_SKIN), 1) != 1)
			return 0;

		pskin->common.SkinType = header.Common.SkinType;
		NSetSkinUpdateFunction(pskin, header.Common.SkinScheme);
		pskin->common.StructureElementType = header.Common.Ref_StructureElementType;
		pskin->common.pGeometry = (NGEOMETRY *)NGetArrayPtr(&ps->pRenderable->GeomArray, header.Common.Geometry);
		pskin->common.pFirstVertex = (NSKINVERTEX *)NGetArrayPtr(&pskin->common.pGeometry->Mesh.VertexArray, header.Common.Ref_FirstVertex);

		switch (pskin->common.StructureElementType)
		{
		case NSTRUCTURE_ELEMENT_JOINT:
			pref_array = &ps->JointArray;
			break;
		case NSTRUCTURE_ELEMENT_MINMAX_CONSTRAINT:
			pref_array = &ps->MinMaxConstraintArray;
			break;
		case NSTRUCTURE_ELEMENT_SIMPLE_CONSTRAINT:
			pref_array = &ps->SimpleConstraintArray;
			break;
		case NSTRUCTURE_ELEMENT_SPECIAL_CONSTRAINT:
			pref_array = &ps->SpecialConstraintArray;
			break;
		case NSTRUCTURE_ELEMENT_SPRING:
			pref_array = &ps->SpringArray;
			break;
		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			break;
		}

		switch (pskin->common.SkinType)
		{
		case NSKIN_TYPE_BILLBOARD:
			pskin->BillBoard.fScale = header.BillBoard.fScale;
			pskin->BillBoard.RatioWidth_Height = header.BillBoard.RatioWidth_Height;
			pskin->BillBoard.pCenter = (NJOINT *)NGetArrayPtr(pref_array, header.BillBoard.Ref_Center);
			pskin->BillBoard.pOrient_A = (NJOINT *)NGetArrayPtr(pref_array, header.BillBoard.Ref_Orient_A);
			pskin->BillBoard.pOrient_B = (NJOINT *)NGetArrayPtr(pref_array, header.BillBoard.Ref_Orient_B);
			pskin->BillBoard.VShift = header.BillBoard.VShift;
			break;

		case NSKIN_TYPE_ROPE:
			pskin->Rope.pFirst = NGetArrayPtr(pref_array, header.Rope.Ref_First);
			pskin->Rope.RangeSize = header.Rope.Ref_RangeSize;
			// pskin->Rope.TwistBulgeFactor	= header.Rope.TwistBulgeFactor;

			// ... And After All, create and load the 2 Vertebras LUTs, Only  if NSKIN is a Rope.
			// 					pskin->Rope.pSideAVertebra = NCreateArray(0,sizeof(Nf32));
			// 					pskin->Rope.pSideBVertebra = NCreateArray(0,sizeof(Nf32));
			// 					NFileReadArray(pfile,pskin->Rope.pSideAVertebra,NULL,0);
			// 					NFileReadArray(pfile,pskin->Rope.pSideBVertebra,NULL,0);
			NSetupArray(&pskin->Rope.VertebrasArray, header.Rope.Ref_RangeSize + 1, sizeof(NSKIN_ROPE_VERTEBRA));

			if (!NFileReadArray(pfile, &pskin->Rope.VertebrasArray, NULL, 0))
				return 0;

			break;

		case NSKIN_TYPE_GRID:
			pskin->Grid.pFirst = NGetArrayPtr(pref_array, header.Grid.Ref_First);
			pskin->Grid.RangeSize = header.Grid.Ref_RangeSize;
			// pskin->Grid.ColumnSize		= header.Grid.Ref_ColumnSize;
			pskin->Grid.I_CellsNb = header.Grid.I_CellsNb;
			pskin->Grid.J_CellsNb = header.Grid.J_CellsNb;
			break;

		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			return 0;
		}
		break;

	// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// void NFileWritePhysicStructure
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//		Incoming ps pointer on success
//		NULL on Error.	"ps" still available.
// ------------------------------------------------------------------------------------------
Nu32 NFileWritePhysicStructure(NFILE *pfile, const NSTRUCTURE *ps)
{

	NSTRUCTURE_FILEBLOCK_MAIN_V100 fileblock;

	if (NFileWriteVersionTag(pfile, VERSION_NSTRUCTURE_FILEBLOCK_MAIN_V100) != 1)
		return 0;

	memset(&fileblock, 0, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V100));
	fileblock.Flags = ps->Flags;
	fileblock.JointSpeedDamping = ps->JointSpeedDamping;
	fileblock.vGravity = ps->vGravity;
	fileblock.JointsAABB = ps->JointsAABB;

	// ----------------------------------------------------------------------------------
	//
	// Structure Pointers descriptions
	//
	// Renderable or Not
	if (ps->pRenderable)
		fileblock.WithRenderable = 1;
	else
		fileblock.WithRenderable = 0;
	//
	// Physic Functions
	//		Notice there is no functions LUT for Physic structures, but only 3 possibles ID:
	//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER	--> function is in the EULER range of functions  ( 3 functions by range:pAccumulateForce,pIntegration,pSatisfyConstraint)
	//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET	--> function is in the VERLET range of functions ( 3 functions by range:pAccumulateForce,pIntegration,pSatisfyConstraint)
	//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN	--> function is unknown.
	//																That mean user bind a specific function to this Physic Structure.
	//																In that case N set the pointer to NULL, AND user has to retrieve
	//																the function by itself and rebind it after this reading process.
	//

	// Accumulate Force
	if (ps->pAccumulateForce == NPhysicStructureDefault_EulerAccumulateForces)
	{
		fileblock.FctID_AccumulateForce = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER;
	}
	else if (ps->pAccumulateForce == NPhysicStructureDefault_VerletAccumulateForces)
	{
		fileblock.FctID_AccumulateForce = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET;
	}
	else
	{
		NErrorIf(1, NERROR_SYSTEM_CHECK); // not an error but check
		fileblock.FctID_AccumulateForce = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN;
	}

	// Integration
	if (ps->pIntegration == NPhysicStructureDefault_EulerIntegration)
	{
		fileblock.FctID_Integration = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER;
	}
	else if (ps->pIntegration == NPhysicStructureDefault_VerletIntegration)
	{
		fileblock.FctID_Integration = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET;
	}
	else
	{
		NErrorIf(1, NERROR_SYSTEM_CHECK); // not an error but check
		fileblock.FctID_Integration = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN;
	}

	// Satisfy Constraint
	if (ps->pSatisfyConstraint == NPhysicStructureDefault_EulerSatisfyConstraints)
	{
		fileblock.FctID_SatisfyConstraint = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER;
	}
	else if (ps->pSatisfyConstraint == NPhysicStructureDefault_VerletSatisfyConstraints)
	{
		fileblock.FctID_SatisfyConstraint = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET;
	}
	else
	{
		NErrorIf(1, NERROR_SYSTEM_CHECK); // not an error but check
		fileblock.FctID_SatisfyConstraint = NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN;
	}
	// ----------------------------------------------------------------------------------

	if (NFileWrite(pfile, &fileblock, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V100), 1) != 1)
		return 0;

	if (fileblock.WithRenderable)
		if (!NFileWriteRenderable(pfile, ps->pRenderable))
			return 0;

	if (!NFileWriteArray(pfile, &ps->JointArray, NULL, 0))
		return 0;
	if (!NFileWriteArray(pfile, &ps->SpringArray, _writeconstraint, (Nu32)&ps->JointArray))
		return 0;
	if (!NFileWriteArray(pfile, &ps->SimpleConstraintArray, _writeconstraint, (Nu32)&ps->JointArray))
		return 0;
	if (!NFileWriteArray(pfile, &ps->MinMaxConstraintArray, _writeconstraint, (Nu32)&ps->JointArray))
		return 0;
	if (!NFileWriteArray(pfile, &ps->SpecialConstraintArray, _writeconstraint, (Nu32)&ps->JointArray))
		return 0;
	if (!NFileWriteArray(pfile, &ps->SkinArray, _writeskin, (Nu32)ps))
		return 0;

	return 1;
}

// ------------------------------------------------------------------------------------------
// NSTRUCTURE* NFileReadPhysicStructure
// ------------------------------------------------------------------------------------------
// Description :
//		Read a Physic Structure.
//
//		Incoming NSTRUCTURE "ps" should be 'empty'. That means with all its array with a size of ZEROs.
//		If Not, A crash will occur in DEBUG MODE when the function will try to read an Array with a non-null size.
//		All the other structure members should be "cleared" too, but it's not a restriction.

// ------------------------------------------------------------------------------------------
// In	:
//		pfile			:	a Valid pointer on an open file
//		ps				:	a valid pointer on an "empty" physic structure
// Out :
//		Incoming "ps" pointer on success
//		NULL on Error.	"ps" still available and, even if its content is uncertain regarding what it was suppose to read,
//						it still AVAILABLE with the number of element effectively read inside.
//						And, thanks to the read order:
//								Renderable with geometries,
//								physic functions,
//								some parameters,
//								Joints ,
//								then constraint that using joints,
//								then skin that using geometries and joints and constraints ...
//
//						... even if read process stop before the end, the loaded things should still work together in a certain way.
//
//						AND NSTRUCTURE MAY BE CLEARED WITH "NClearPhysicStructure".
// ------------------------------------------------------------------------------------------
Nu32 NFileReadPhysicStructure(NFILE *pfile, NSTRUCTURE *ps)
{
	NSTRUCTURE_FILEBLOCK_MAIN_V000 fileblock_v000;
	NSTRUCTURE_FILEBLOCK_MAIN_V100 fileblock_v100;

	NRENDERABLE_DESC rdesc;
	Nu32 version;

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Main Version (0.0.0)
	case NGETVERSION_MAIN(VERSION_NSTRUCTURE_FILEBLOCK_MAIN_V000):
		memset(&fileblock_v000, 0, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V000));
		if (NFileRead(pfile, &fileblock_v000, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V000), 1) != 1)
			return 0;

		if (fileblock_v000.WithRenderable)
		{
			// Create an Empty Renderable
			if (!ps->pRenderable)
			{
				memset(&rdesc, 0, sizeof(NRENDERABLE_DESC));
				ps->pRenderable = NCreateRenderable(&rdesc);
			}

			if (!NFileReadRenderable(pfile, ps->pRenderable))
				return 0;
		}

		// In that Version 000 Bind VERLET Physic functions
		NBind_Physic_VerletFunctions(ps);

		ps->Flags = fileblock_v000.Flags;
		ps->JointSpeedDamping = fileblock_v000.JointSpeedDamping;
		ps->vGravity = fileblock_v000.vGravity;
		ps->JointsAABB = fileblock_v000.JointsAABB;

		if (!NFileReadArray(pfile, &ps->JointArray, NULL, 0))
			return 0;
		if (!NFileReadArray(pfile, &ps->SpringArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SimpleConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->MinMaxConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SpecialConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SkinArray, _readskin, (Nu32)ps))
			return 0;

		return 1;

		// Main Version (1.0.0)
	case NGETVERSION_MAIN(VERSION_NSTRUCTURE_FILEBLOCK_MAIN_V100):
		memset(&fileblock_v100, 0, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V100));
		if (NFileRead(pfile, &fileblock_v100, sizeof(NSTRUCTURE_FILEBLOCK_MAIN_V100), 1) != 1)
			return 0;

		// ----------------------------------------------------------------------------------
		//
		// Structure Pointers descriptions
		//
		// Renderable or Not
		if (fileblock_v100.WithRenderable)
		{
			// Create an Empty Renderable
			if (!ps->pRenderable)
			{
				memset(&rdesc, 0, sizeof(NRENDERABLE_DESC));
				ps->pRenderable = NCreateRenderable(&rdesc);
			}

			if (!NFileReadRenderable(pfile, ps->pRenderable))
				return 0;
		}

		//
		// Physic Functions
		//		Notice there is no functions LUT for Physic structures, but only 3 possibles ID:
		//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER	--> function is in the EULER range of functions  ( 3 functions by range:pAccumulateForce,pIntegration,pSatisfyConstraint)
		//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET	--> function is in the VERLET range of functions ( 3 functions by range:pAccumulateForce,pIntegration,pSatisfyConstraint)
		//					NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN	--> function is unknown.
		//																That mean user bind a specific function to this Physic Structure.
		//																In that case N set the pointer to NULL, AND user has to retrieve
		//																the function by itself and rebind it after this reading process.
		//

		// Accumulate Force
		switch (fileblock_v100.FctID_AccumulateForce)
		{
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER:
			ps->pAccumulateForce = NPhysicStructureDefault_EulerAccumulateForces;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET:
			ps->pAccumulateForce = NPhysicStructureDefault_VerletAccumulateForces;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN:
			NErrorIf(1, NERROR_SYSTEM_CHECK); // not an error but check
			ps->pAccumulateForce = NULL;
			break;
		default:
			NErrorIf(1, NERROR_FILE_UNEXPECTED_READ_VALUE);
			return 0;
		}

		// Integration
		switch (fileblock_v100.FctID_Integration)
		{
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER:
			ps->pIntegration = NPhysicStructureDefault_EulerIntegration;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET:
			ps->pIntegration = NPhysicStructureDefault_VerletIntegration;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN:
			NErrorIf(1, NERROR_SYSTEM_CHECK); // not an error but check
			ps->pIntegration = NULL;
			break;
		default:
			NErrorIf(1, NERROR_FILE_UNEXPECTED_READ_VALUE);
			return 0;
		}

		// Satisfy Constraint
		switch (fileblock_v100.FctID_SatisfyConstraint)
		{
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER:
			ps->pSatisfyConstraint = NPhysicStructureDefault_EulerSatisfyConstraints;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET:
			ps->pSatisfyConstraint = NPhysicStructureDefault_VerletSatisfyConstraints;
			break;
		case NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN:
			// 					NErrorIf(1,NERROR_SYSTEM_CHECK); // not an error but check
			ps->pSatisfyConstraint = NULL;
			break;
		default:
			NErrorIf(1, NERROR_FILE_UNEXPECTED_READ_VALUE);
			return 0;
		}
		// ----------------------------------------------------------------------------------

		ps->Flags = fileblock_v100.Flags;
		ps->JointSpeedDamping = fileblock_v100.JointSpeedDamping;
		ps->vGravity = fileblock_v100.vGravity;
		ps->JointsAABB = fileblock_v100.JointsAABB;

		if (!NFileReadArray(pfile, &ps->JointArray, NULL, 0))
			return 0;
		if (!NFileReadArray(pfile, &ps->SpringArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SimpleConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->MinMaxConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SpecialConstraintArray, _readconstraint, (Nu32)&ps->JointArray))
			return 0;
		if (!NFileReadArray(pfile, &ps->SkinArray, _readskin, (Nu32)ps))
			return 0;

		return 1;

	// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}
