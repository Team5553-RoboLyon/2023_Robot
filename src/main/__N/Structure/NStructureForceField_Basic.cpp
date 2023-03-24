// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						   NStructureForceField_Basic.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"
#include "../NStructure.h"
#include "../NStructureForceField.h"
#include "../NErrorHandling.h"


extern NARRAY	NActivePhysicStructure_ForceFields_Array;

NSTRUCTUREFORCEFIELD* NSetupPhysicStructureForceField_Basic(NSTRUCTUREFORCEFIELD* pff, const NAPPLYFORCEFIELD_FCT applyforcefield_fct)
{
	Nmem0(pff,NSTRUCTUREFORCEFIELD);
	//pff->Basic.pDum = pff;
	pff->Basic.pUpdateAndApplyForceField = applyforcefield_fct;
	return pff;
}

Nbool	NActivatePhysicStructureForceField_Basic( const NAPPLYFORCEFIELD_FCT applyforcefield_fct )
{
	if( NActivePhysicStructure_ForceFields_Array.Size < NActivePhysicStructure_ForceFields_Array.Capacity )
	{
		// SetUp Data
		NSetupPhysicStructureForceField_Basic((NSTRUCTUREFORCEFIELD*)&NActivePhysicStructure_ForceFields_Array.pFirst[NActivePhysicStructure_ForceFields_Array.Size],applyforcefield_fct );
		NActivePhysicStructure_ForceFields_Array.Size += 1;
		return NTRUE;
	}
	return NFALSE;
}
