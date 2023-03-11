#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// NFileWriteMaterialValues
// ------------------------------------------------------------------------------------------
// Description :
//	Write all material values ... without writing the "pName" Member
// ------------------------------------------------------------------------------------------
// In  :
//		pfile		:	a Valid pointer on an open file
//		pmaterial	:	a valid pointer on a valid NMATERIAL Structure.
// Out :
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteMaterialValues(NFILE *pfile, const NMATERIAL *pmaterial)
{
	NErrorIf(!pmaterial || !pfile, NERROR_NULL_POINTER);

	NHEADER_MATERIAL header;
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_MATERIAL) != 1)
		return 0;
	Nmem0(&header, NHEADER_MATERIAL);
	header.AmbientColor = pmaterial->AmbientColor;
	header.DiffuseColor = pmaterial->DiffuseColor;
	header.EmissionColor = pmaterial->EmissionColor;
	header.SpecularColor = pmaterial->SpecularColor;
	header.Shininess = pmaterial->Shininess;

	if (NFileWrite(pfile, &header, sizeof(NHEADER_MATERIAL), 1) != 1)
		return 0;

	return 1;
}

// ------------------------------------------------------------------------------------------
// NFileReadMaterial
// ------------------------------------------------------------------------------------------
// Description :
//	read all material values ... without reading the "pName" Member
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			: a Valid pointer on an open file
//		pmaterial		: a valid pointer on a NMATERIAL structure
// Out :
//
// ------------------------------------------------------------------------------------------
Nu32 NFileReadMaterialValues(NFILE *pfile, NMATERIAL *pmaterial)
{
	NErrorIf(!pmaterial || !pfile, NERROR_NULL_POINTER);

	NHEADER_MATERIAL header;
	Nu32 version;
	// Check Incoming version.
	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current main version (alls versions 0.0.x):
	case NGETVERSION_MAIN(VERSION_NHEADER_MATERIAL):
		Nmem0(&header, NHEADER_MATERIAL);
		if (NFileRead(pfile, &header, sizeof(NHEADER_MATERIAL), 1) != 1)
			return 0;

		pmaterial->AmbientColor = header.AmbientColor;
		pmaterial->DiffuseColor = header.DiffuseColor;
		pmaterial->EmissionColor = header.EmissionColor;
		pmaterial->SpecularColor = header.SpecularColor;
		pmaterial->Shininess = header.Shininess;
		return 1;

		// Unknown version:
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}
