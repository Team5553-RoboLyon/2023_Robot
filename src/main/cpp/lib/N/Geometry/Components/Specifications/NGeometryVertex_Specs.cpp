#include "../../../Core/NSafeConversion.h"
#include "../NGeometryVertex.h"
#include "NGeometryVertex_Flags.h"



NVERTEX_SPECS NVertexSpecs[NVERTEX_FORMAT_ENUM_SIZE];


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
void NInitializeVertexSpecs()
{
// 	NVERTEX_SPECS is an union of 2 structures:
// 		NVERTEX_SPECS_DIRECT_ACCESS and NVERTEX_SPECS_TABLE_ACCESS
// 		purpose of these 2 structures is accessing  to the same data in memory in two different ways.
// 		So they need to stay consistent and to reflect the exact same memory area. 
// 		So their sizes are exactly the same ... the Test check that ...
	NErrorIf(sizeof(NVERTEX_SPECS_DIRECT_ACCESS)!=sizeof(NVERTEX_SPECS_TABLE_ACCESS), NERROR_SYSTEM_CHECK);
	

	// All to ZERO ...
	memset(NVertexSpecs,0,sizeof(NVERTEX_SPECS)*NVERTEX_FORMAT_ENUM_SIZE);
	
	// Setup each of them, One by One
	// NVERTEX_FORMAT_NULL
	// Nothing to do ... everything still ZERO

	// NVERTEX_FORMAT_V3
	NVertexSpecs[NVERTEX_FORMAT_V3].DirectAccess.Flags					= FLAGS_NVERTEX_SPECS_V3;
	NVertexSpecs[NVERTEX_FORMAT_V3].DirectAccess.SizeOfVertex			= _SafeNu32ToNu8(sizeof(NVERTEX_V3));
	NVertexSpecs[NVERTEX_FORMAT_V3].DirectAccess.SizeOfPosition			= _SafeNu32ToNu8(sizeof(Nf32)*3);
	
	// NVERTEX_FORMAT_V3C4
	NVertexSpecs[NVERTEX_FORMAT_V3C4].DirectAccess.Flags				= FLAGS_NVERTEX_SPECS_V3C4;
	NVertexSpecs[NVERTEX_FORMAT_V3C4].DirectAccess.SizeOfVertex			= _SafeNu32ToNu8(sizeof(NVERTEX_V3C4));
	NVertexSpecs[NVERTEX_FORMAT_V3C4].DirectAccess.SizeOfPosition		= _SafeNu32ToNu8(sizeof(Nf32)*3);
	NVertexSpecs[NVERTEX_FORMAT_V3C4].DirectAccess.SizeOfColor0			= _SafeNu32ToNu8(sizeof(Nf32)*4);

	// NVERTEX_FORMAT_V3C4T2
	NVertexSpecs[NVERTEX_FORMAT_V3C4T2].DirectAccess.Flags				= FLAGS_NVERTEX_SPECS_V3C4T2;
	NVertexSpecs[NVERTEX_FORMAT_V3C4T2].DirectAccess.SizeOfVertex		= _SafeNu32ToNu8(sizeof(NVERTEX_V3C4T2));
	NVertexSpecs[NVERTEX_FORMAT_V3C4T2].DirectAccess.SizeOfPosition		= _SafeNu32ToNu8(sizeof(Nf32)*3);
	NVertexSpecs[NVERTEX_FORMAT_V3C4T2].DirectAccess.SizeOfColor0		= _SafeNu32ToNu8(sizeof(Nf32)*4);
	NVertexSpecs[NVERTEX_FORMAT_V3C4T2].DirectAccess.SizeOfTexCoord0	= _SafeNu32ToNu8(sizeof(Nf32)*2);

	// NVERTEX_FORMAT_V3N3C4T2
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.Flags			= FLAGS_NVERTEX_SPECS_V3N3C4T2;
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.SizeOfVertex		= _SafeNu32ToNu8(sizeof(NVERTEX_V3N3C4T2));
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.SizeOfPosition	= _SafeNu32ToNu8(sizeof(Nf32)*3);
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.SizeOfNormal		= _SafeNu32ToNu8(sizeof(Nf32)*3);
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.SizeOfColor0		= _SafeNu32ToNu8(sizeof(Nf32)*4);
	NVertexSpecs[NVERTEX_FORMAT_V3N3C4T2].DirectAccess.SizeOfTexCoord0	= _SafeNu32ToNu8(sizeof(Nf32)*2);
	
	Nu32 i,j;
	Nu32 offset = 0;
	for(i=0;i<NVERTEX_FORMAT_ENUM_SIZE;i++,offset = 0)
	{
		for(j=0;j<NVERTEX_DATA_ENUM_SIZE;j++)
		{
			NVertexSpecs[i].TableAccess.Offset[j] = _SafeNu32ToNu8(offset);
			offset += NVertexSpecs[i].TableAccess.SizeOf[j];
		}
	}

	// Checks
#ifdef _DEBUG
	// To help finding the forgotten vertex format/ offsets updates
	// Notice than we start from NVERTEX_FORMAT_V3 and not from NVERTEX_FORMAT_NULL ...
	// ... because NVERTEX_FORMAT_NULL.SizeOfVertex = 0 ... and it's normal.
	for(Nu32 i_dbg=NVERTEX_FORMAT_V3;i_dbg<NVERTEX_FORMAT_ENUM_SIZE;i_dbg++)
		NErrorIf(NVertexSpecs[i_dbg].DirectAccess.SizeOfVertex == 0,NERROR_SYSTEM_GURU_MEDITATION);
#endif
}
void NDisableVertexSpecs()
{
	// All to ZERO ...
	memset(NVertexSpecs,0,sizeof(NVERTEX_SPECS)*NVERTEX_FORMAT_ENUM_SIZE);
}

NVERTEX_SPECS* NGetVertexSpecs(const NVERTEX_FORMAT_ENUM nvertex_format)
{
	NErrorIf(nvertex_format>=NVERTEX_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return &NVertexSpecs[nvertex_format];
}

Nu32 NGetVertexSpecsFlags(const NVERTEX_FORMAT_ENUM nvertex_format)
{
	NErrorIf(nvertex_format>=NVERTEX_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE);
	return NVertexSpecs[nvertex_format].DirectAccess.Flags;
}

Nu8 NGetSizeOfVertexData(const NVERTEX_FORMAT_ENUM nvertex_format, const NVERTEX_DATA_ENUM nvertex_data)
{
	NErrorIf( nvertex_format>=NVERTEX_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE );
	NErrorIf( nvertex_data>=NVERTEX_DATA_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE );
	return NVertexSpecs[nvertex_format].TableAccess.SizeOf[nvertex_data];
}

Nu8 NGetSizeOfVertex(const NVERTEX_FORMAT_ENUM nvertex_format)
{
	NErrorIf( nvertex_format>=NVERTEX_FORMAT_ENUM_SIZE,NERROR_VALUE_OUTOFRANGE );
	return NVertexSpecs[nvertex_format].DirectAccess.SizeOfVertex;
}
