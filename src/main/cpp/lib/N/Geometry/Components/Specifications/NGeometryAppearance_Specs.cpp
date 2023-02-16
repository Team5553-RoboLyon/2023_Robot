#include "../../../Core/NSafeConversion.h"
#include "../NGeometryTextureUnit.h"

#include "../NGeometryAppearance.h"


NAPPEARANCE_SPECS NAppearanceSpecs[NAPPEARANCE_FORMAT_ENUM_SIZE];
// ------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NInitializeAppearanceSpecs()
{
	// All to ZERO ...
	memset(NAppearanceSpecs,0,sizeof(NAPPEARANCE_SPECS)*NAPPEARANCE_FORMAT_ENUM_SIZE);
	// Setup each of them, One by One
	// NAPPEARANCE_FORMAT_NULL
	// Nothing to do ...
	// NAPPEARANCE_FORMAT_TT
	NAppearanceSpecs[NAPPEARANCE_FORMAT_TT].Flags = FLAGS_NAPPEARANCE_SPECS_TT;

	// NAPPEARANCE_FORMAT_TM
	NAppearanceSpecs[NAPPEARANCE_FORMAT_TM].Flags = FLAGS_NAPPEARANCE_SPECS_TM;

	// NAPPEARANCE_FORMAT_ULTD_T
	NAppearanceSpecs[NAPPEARANCE_FORMAT_ULTD_T].Flags = FLAGS_NAPPEARANCE_SPECS_ULTD_T;

	// NAPPEARANCE_FORMAT_ULTD_TM
	NAppearanceSpecs[NAPPEARANCE_FORMAT_ULTD_TM].Flags = FLAGS_NAPPEARANCE_SPECS_ULTD_TM;

	// NAPPEARANCE_FORMAT_xxx
	// NAPPEARANCE_FORMAT_xxx
	// NAPPEARANCE_FORMAT_xxx
}
void NDisableAppearanceSpecs()
{
	// All to ZERO ...
	memset(NAppearanceSpecs,0,sizeof(NAPPEARANCE_SPECS)*NAPPEARANCE_FORMAT_ENUM_SIZE);
}

NAPPEARANCE_SPECS* NGetAppearanceSpecs(const NAPPEARANCE_FORMAT_ENUM nappearance_format)
{
	NErrorIf(nappearance_format>=NAPPEARANCE_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return &NAppearanceSpecs[nappearance_format];
}

Nu32 NGetAppearanceSpecsFlags(const NAPPEARANCE_FORMAT_ENUM nappearance_format)
{
	NErrorIf(nappearance_format>=NAPPEARANCE_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NAppearanceSpecs[nappearance_format].Flags;
}

