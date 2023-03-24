#include "../../../Core/NSafeConversion.h"
#include "../NGeometryPrimitive.h"
#include "NGeometryPrimitive_Flags.h"



NPRIMITIVE_SPECS NPrimitiveSpecs[NPRIMITIVE_FORMAT_ENUM_SIZE];
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
void NInitializePrimitiveSpecs()
{
	// All to ZERO ...
	memset(NPrimitiveSpecs,0,sizeof(NPRIMITIVE_SPECS)*NPRIMITIVE_FORMAT_ENUM_SIZE);
	// Setup each of them, One by One
	//NPRIMITIVE_FORMAT_NULL
	 //... Nothing to do
	
	//NPRIMITIVE_FORMAT_POINT
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].Flags					= FLAGS_NPRIMITIVE_SPECS_POINT;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].SizeOfPrimitive		= _SafeNu32ToNu8(sizeof(NPOINT));

	//NPRIMITIVE_FORMAT_LINE
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINE].Flags					= FLAGS_NPRIMITIVE_SPECS_LINE;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINE].SizeOfPrimitive			= _SafeNu32ToNu8(sizeof(NLINE));// ! = 2 * sizeof(NPOINT)
	
	//NPRIMITIVE_FORMAT_LINESTRIP
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINESTRIP].Flags				= FLAGS_NPRIMITIVE_SPECS_LINESTRIP;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINESTRIP].SizeOfPrimitive	= _SafeNu32ToNu8(sizeof(NPOINT));	// ! NPOINT as a size of Nu32 on Windows and a size of Nu16 on IOS/Android
																										// ! NLINE_STRIP doesn't exist as a structure with a fixed number of index inside ... All the index in the PrimitiveArray belong to the same LineStrip
	//NPRIMITIVE_FORMAT_LINELOOP
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINELOOP].Flags				= FLAGS_NPRIMITIVE_SPECS_LINELOOP;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_LINELOOP].SizeOfPrimitive		= _SafeNu32ToNu8(sizeof(NPOINT));	// ! NPOINT as a size of Nu32 on Windows and a size of Nu16 on IOS/Android		
																										// ! NLINE_LOOP doesn't exist as a structure with a fixed number of index inside ... All the index in the PrimitiveArray belong to the same LineLoop	
	//NPRIMITIVE_FORMAT_TRIANGLE
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLE].Flags				= FLAGS_NPRIMITIVE_SPECS_TRIANGLE;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLE].SizeOfPrimitive		= _SafeNu32ToNu8(sizeof(NTRIANGLE));// ! = 3 * sizeof(NPOINT)
	
	//NPRIMITIVE_FORMAT_TRIANGLESTRIP
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLESTRIP].Flags			= FLAGS_NPRIMITIVE_SPECS_TRIANGLESTRIP;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLESTRIP].SizeOfPrimitive= _SafeNu32ToNu8(sizeof(NPOINT));	// ! NPOINT as a size of Nu32 on Windows and a size of Nu16 on IOS/Android
																										// ! NTRIANGLE_STRIP doesn't exist as a structure with a fixed number of index inside ... All the index in the PrimitiveArray belong to the same NTRIANGLE_STRIP	
	//NPRIMITIVE_FORMAT_TRIANGLEFAN
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLEFAN].Flags			= FLAGS_NPRIMITIVE_SPECS_TRIANGLEFAN;
	NPrimitiveSpecs[NPRIMITIVE_FORMAT_TRIANGLEFAN].SizeOfPrimitive	= _SafeNu32ToNu8(sizeof(NPOINT));	// ! NPOINT as a size of Nu32 on Windows and a size of Nu16 on IOS/Android
																										// ! NTRIANGLE_LOOP doesn't exist as a structure with a fixed number of index inside ... All the index in the PrimitiveArray belong to the same NTRIANGLE_LOOP	
}
void NDisablePrimitiveSpecs()
{
	// All to ZERO ...
	memset(NPrimitiveSpecs,0,sizeof(NPRIMITIVE_SPECS)*NPRIMITIVE_FORMAT_ENUM_SIZE);
}
NPRIMITIVE_SPECS* NGetPrimitiveSpecs(const NPRIMITIVE_FORMAT_ENUM nprimitive_format)
{
	NErrorIf(nprimitive_format>=NPRIMITIVE_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return &NPrimitiveSpecs[nprimitive_format];
}

Nu32 NGetPrimitiveSpecsFlags(const NPRIMITIVE_FORMAT_ENUM nprimitive_format)
{
	NErrorIf(nprimitive_format>=NPRIMITIVE_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NPrimitiveSpecs[nprimitive_format].Flags;
}

Nu8 NGetSizeOfPrimitive(const NPRIMITIVE_FORMAT_ENUM nprimitive_format)
{
	NErrorIf(nprimitive_format>=NPRIMITIVE_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NPrimitiveSpecs[nprimitive_format].SizeOfPrimitive;
}