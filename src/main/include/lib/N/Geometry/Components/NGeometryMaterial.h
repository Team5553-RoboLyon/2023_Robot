#ifndef __NGEOMETRYMATERIAL_H
#define __NGEOMETRYMATERIAL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							  NGeometryMaterial.h									**
// **																					**
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Miscellaneous/NColor.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							  Structures & Defines									**
// ***************************************************************************************
// File Extensions
#define EXTENSION_NMATERIAL ".nmt"
// File Signatures
#define SIGNATURE_NMATERIAL NMAKESIGNATURE('N', 'M', 'A', 'T')

	typedef enum
	{
		NMATERIAL_FORMAT_NULL = 0,
		NMATERIAL_FORMAT_STANDARD,
		// NMATERIAL_FORMAT_AVAILABLE,
		// NMATERIAL_FORMAT_AVAILABLE,
		//----------------------
		NMATERIAL_FORMAT_ENUM_SIZE // MAX size is 4 !!! (and max ID is 3) This ID is going to be stored on a 2 bits value !!!
	} NMATERIAL_FORMAT_ENUM;

	// Notes: a Material store all the lightning parameters
	typedef struct
	{
		Nchar *pName;

		NCOLOR AmbientColor;
		NCOLOR DiffuseColor;
		NCOLOR SpecularColor;
		NCOLOR EmissionColor;
		Nf32 Shininess;
	} NMATERIAL;

	typedef struct
	{
		NCOLOR AmbientColor;
		NCOLOR DiffuseColor;
		NCOLOR SpecularColor;
		NCOLOR EmissionColor;
		Nf32 Shininess;
	} NMATERIAL_DESC;
	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	NMATERIAL *NSetupMaterial(NMATERIAL *pmat, const Nchar *pname, const NMATERIAL_DESC *pdesc);
	NMATERIAL *NCreateMaterial(const Nchar *pname, const NMATERIAL_DESC *pdesc);
	void NClearMaterial(NMATERIAL *pmaterial);
	void NDeleteMaterial(NMATERIAL *pmaterial);
	NMATERIAL *NCreateMaterialFromFile(const Nchar *pmaterialname, const Nchar *pfilename);
	NMATERIAL *NSaveMaterial(const NMATERIAL *pmaterial, const Nchar *pfilename);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGEOMETRYMATERIAL_H
