#include "../../../Core/NSafeConversion.h"
#include "../NGeometryTextureUnit.h"
#include "NGeometryTextureUnit_Flags.h"



NTEXTUREUNIT_SPECS NTextureUnitSpecs[NTEXTUREUNIT_FORMAT_ENUM_SIZE];
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
void NInitializeTextureUnitSpecs()
{
	// All to ZERO ...
	memset(NTextureUnitSpecs,0,sizeof(NTEXTUREUNIT_SPECS)*NTEXTUREUNIT_FORMAT_ENUM_SIZE);
	// Setup each of them, One by One
	// NTEXTUREUNIT_FORMAT_NULL
	// NTEXTUREUNIT_FORMAT_T
	NTextureUnitSpecs[NTEXTUREUNIT_FORMAT_T].Flags				= FLAGS_NTEXTURE_UNIT_SPECS_T;
	NTextureUnitSpecs[NTEXTUREUNIT_FORMAT_T].SizeOfTextureUnit	= _SafeNu32ToNu8(sizeof(NTEXTUREUNIT_T));
	// NTEXTUREUNIT_FORMAT_TM
	NTextureUnitSpecs[NTEXTUREUNIT_FORMAT_TM].Flags				= FLAGS_NTEXTURE_UNIT_SPECS_TM;
	NTextureUnitSpecs[NTEXTUREUNIT_FORMAT_TM].SizeOfTextureUnit	= _SafeNu32ToNu8(sizeof(NTEXTUREUNIT_TM));
	//NTextureUnitSizeOf[NTEXTUREUNIT_FORMAT_AVAILABLE]	= _SafeNu32ToNu8(sizeof(???));
}
void NDisableTextureUnitSpecs()
{
	// All to ZERO ...
	memset(NTextureUnitSpecs,0,sizeof(NTEXTUREUNIT_SPECS)*NTEXTUREUNIT_FORMAT_ENUM_SIZE);
}

NTEXTUREUNIT_SPECS* NGetTextureUnitSpecs(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format)
{
	NErrorIf(ntextureunit_format>=NTEXTUREUNIT_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return &NTextureUnitSpecs[ntextureunit_format];
}
Nu32 NGetTextureUnitSpecsFlags(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format)
{
	NErrorIf(ntextureunit_format>=NTEXTUREUNIT_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NTextureUnitSpecs[ntextureunit_format].Flags;
}

Nu8 NGetSizeOfTextureUnit(const NTEXTUREUNIT_FORMAT_ENUM ntextureunit_format)
{
	NErrorIf(ntextureunit_format>=NTEXTUREUNIT_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NTextureUnitSpecs[ntextureunit_format].SizeOfTextureUnit;
}

