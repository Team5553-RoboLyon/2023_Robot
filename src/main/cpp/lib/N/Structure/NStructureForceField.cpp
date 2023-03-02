// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						   NStructureForceFiled.cpp									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NStructureForceField.h"

NARRAY NActivePhysicStructure_ForceFields_Array;

// Infos: The function "NInitializePhysicStructures()" initialize "NActivePhysicStructure_ForceFields_Array"

void NClearPhysicStructureForceField(NSTRUCTUREFORCEFIELD *pff)
{
	Nmem0(pff, NSTRUCTUREFORCEFIELD);
}
