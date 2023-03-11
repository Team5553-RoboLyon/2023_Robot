// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						   NStructureForceField_Basic.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/NStructure.h"
#include "lib/N/NStructureForceField.h"
#include "lib/N/NErrorHandling.h"

extern NARRAY NActivePhysicStructure_ForceFields_Array;

NSTRUCTUREFORCEFIELD *NSetupPhysicStructureForceField_Basic(NSTRUCTUREFORCEFIELD *pff, const NAPPLYFORCEFIELD_FCT applyforcefield_fct)
{
	Nmem0(pff, NSTRUCTUREFORCEFIELD);
	// pff->Basic.pDum = pff;
	pff->Basic.pUpdateAndApplyForceField = applyforcefield_fct;
	return pff;
}

Nbool NActivatePhysicStructureForceField_Basic(const NAPPLYFORCEFIELD_FCT applyforcefield_fct)
{
	if (NActivePhysicStructure_ForceFields_Array.Size < NActivePhysicStructure_ForceFields_Array.Capacity)
	{
		// SetUp Data
		NSetupPhysicStructureForceField_Basic((NSTRUCTUREFORCEFIELD *)&NActivePhysicStructure_ForceFields_Array.pFirst[NActivePhysicStructure_ForceFields_Array.Size], applyforcefield_fct);
		NActivePhysicStructure_ForceFields_Array.Size += 1;
		return NTRUE;
	}
	return NFALSE;
}
