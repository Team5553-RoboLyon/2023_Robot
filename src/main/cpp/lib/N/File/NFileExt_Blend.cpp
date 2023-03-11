#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// NFileWriteBlendValues
// ------------------------------------------------------------------------------------------
// Description :
// Write a string into a Binary file
// ------------------------------------------------------------------------------------------
// In  :
//		pfile	:	a Valid pointer on an open file
//		pblend	:	a valid pointer on a valid NBLEND Structure.
// Out :
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteBlendValues(NFILE *pfile, const NBLEND *pblend)
{
	NErrorIf(!pblend || !pfile, NERROR_NULL_POINTER);

	NHEADER_BLEND header;
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_BLEND) != 1)
		return 0;

	Nmem0(&header, NHEADER_BLEND);
	header.Flags = pblend->Flags;
	header.Blend0_Equation = pblend->Blend0_Equation;
	header.Blend0_Sfactor = pblend->Blend0_Sfactor;
	header.Blend0_Dfactor = pblend->Blend0_Dfactor;
	header.Blend1_Equation = pblend->Blend1_Equation;
	header.Blend1_Sfactor = pblend->Blend1_Sfactor;
	header.Blend1_Dfactor = pblend->Blend1_Dfactor;
	if (NFileWrite(pfile, &header, sizeof(NHEADER_BLEND), 1) != 1)
		return 0;

	return 1;
}

// ------------------------------------------------------------------------------------------
// NFileReadBlendValues
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			: a Valid pointer on an open file
//		pblend			: a valid pointer on a NBLEND structure
// Out :
//
// ------------------------------------------------------------------------------------------
Nu32 NFileReadBlendValues(NFILE *pfile, NBLEND *pblend)
{
	NErrorIf(!pblend || !pfile, NERROR_NULL_POINTER);

	NHEADER_BLEND header;
	Nu32 version;

	// Check Incoming version.
	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current main version (alls versions 0.0.x):
	case NGETVERSION_MAIN(VERSION_NHEADER_BLEND):
		Nmem0(&header, NHEADER_BLEND);
		if (NFileRead(pfile, &header, sizeof(NHEADER_BLEND), 1) != 1)
			return 0;

		pblend->Flags = header.Flags;
		pblend->Blend0_Equation = header.Blend0_Equation;
		pblend->Blend0_Sfactor = header.Blend0_Sfactor;
		pblend->Blend0_Dfactor = header.Blend0_Dfactor;
		pblend->Blend1_Equation = header.Blend1_Equation;
		pblend->Blend1_Sfactor = header.Blend1_Sfactor;
		pblend->Blend1_Dfactor = header.Blend1_Dfactor;
		return 1;

	// Unknown version:
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}
