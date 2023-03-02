#ifndef __NGEOMETRYPRIMITIVE_H
#define __NGEOMETRYPRIMITIVE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NGeometryPrimitive.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/Components/specifications/NGeometryPrimitive_Flags.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	typedef enum
	{
		NPRIMITIVE_FORMAT_NULL = 0,
		NPRIMITIVE_FORMAT_POINT,
		NPRIMITIVE_FORMAT_LINE,
		NPRIMITIVE_FORMAT_LINESTRIP,
		NPRIMITIVE_FORMAT_TRIANGLE,
		NPRIMITIVE_FORMAT_TRIANGLESTRIP,
		NPRIMITIVE_FORMAT_TRIANGLEFAN,
		NPRIMITIVE_FORMAT_LINELOOP, // TODO: move up this ID (added later) just below NPRIMITIVE_FORMAT_LINESTRIP.
		// -----------------------
		NPRIMITIVE_FORMAT_ENUM_SIZE // MAX size is 8 !!! (and max ID is 7) This ID is going to be stored on a 3 bits value !!!
	} NPRIMITIVE_FORMAT_ENUM;

#ifdef _NWINDOWS
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Point
	typedef struct
	{
		Nu32 i0;
	} NPOINT;

	// a Line
	typedef struct
	{
		union
		{
			struct
			{
				Nu32 i0;
				Nu32 i1;
			};
			Nu32 i[2];
		};
	} NLINE;

	// NLINE_STRIP structure ... doesn't really exists.
	// a LineStrip ( a line strip is simply a list of vertex index so the entire primitive array of the geometry IS the line strip !)

	// NLINE_LOOP structure ... doesn't really exists.
	// ( same thing than NLINE_STRIP )

	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Triangle
	typedef struct
	{
		union
		{
			struct
			{
				Nu32 i0;
				Nu32 i1;
				Nu32 i2;
			};
			Nu32 i[3];
		};
	} NTRIANGLE;
// ------------------------------------------------------------------------------------------------------------------------------------------
// a Quad
/*
typedef struct
{
	union
	{
		struct
		{
			Nu32 i0;
			Nu32 i1;
			Nu32 i2;
			Nu32 i3;
		};
		Nu32 i[4];
	};
}NQUAD;
*/
#endif // _NWINDOWS
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _NIOS
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Point
	typedef struct
	{
		Nu16 i0;
	} NPOINT;

	// a Line
	typedef struct
	{
		union
		{
			struct
			{
				Nu16 i0;
				Nu16 i1;
			};
			Nu16 i[2];
		};
	} NLINE;
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Triangle
	typedef struct
	{
		union
		{
			struct
			{
				Nu16 i0;
				Nu16 i1;
				Nu16 i2;
			};
			Nu16 i[3];
		};
	} NTRIANGLE;
// ------------------------------------------------------------------------------------------------------------------------------------------
// a Quad
/*
typedef struct
{
	union
	{
		struct
		{
			Nu16 i0;
			Nu16 i1;
			Nu16 i2;
			Nu16 i3;
		};
		Nu16 i[4];
	};
}NQUAD;
*/
#endif //_NIOS

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _NANDROID
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Point
	typedef struct
	{
		Nu16 i0;
	} NPOINT;

	// a Line
	typedef struct
	{
		union
		{
			struct
			{
				Nu16 i0;
				Nu16 i1;
			};
			Nu16 i[2];
		};
	} NLINE;
	// ------------------------------------------------------------------------------------------------------------------------------------------
	// a Triangle
	typedef struct
	{
		union
		{
			struct
			{
				Nu16 i0;
				Nu16 i1;
				Nu16 i2;
			};
			Nu16 i[3];
		};
	} NTRIANGLE;
// ------------------------------------------------------------------------------------------------------------------------------------------
// a Quad
/*
typedef struct
{
	union
	{
		struct
		{
			Nu16 i0;
			Nu16 i1;
			Nu16 i2;
			Nu16 i3;
		};
		Nu16 i[4];
	};
}NQUAD;
*/
#endif //_NANDROID

	typedef struct
	{
		Nu32 Flags;
		Nu8 SizeOfPrimitive;
		// 	Nu8		Available_Nu8;
		// 	Nu16	Available_Nu16;
	} NPRIMITIVE_SPECS;
	// ***************************************************************************************
	// **					Primitive Functions												**
	// ***************************************************************************************
	//
	// ***************************************************************************************
	// **					Primitive "Specs" Functions										**
	// ***************************************************************************************
	NPRIMITIVE_SPECS *NGetPrimitiveSpecs(const NPRIMITIVE_FORMAT_ENUM nprimitive_format);
	Nu32 NGetPrimitiveSpecsFlags(const NPRIMITIVE_FORMAT_ENUM nprimitive_format);
	Nu8 NGetSizeOfPrimitive(const NPRIMITIVE_FORMAT_ENUM nprimitive_format);

	// ***************************************************************************************
	// **					Primitive "PRIVATE" Functions									**
	// ***************************************************************************************
	void NInitializePrimitiveSpecs();
	void NDisablePrimitiveSpecs();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NPRIMITIVE_H
