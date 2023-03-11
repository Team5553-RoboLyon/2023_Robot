#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NErrorHandling.h"

extern NPRIMITIVE_SPECS NPrimitiveSpecs[];

// A primitive array is, at the end of the day, an array of index.
// These index may be 32 bits index or 16 bits index, according with the opengl version ( standard or ES )
// So, it's necessary to convert

// In the other hand, primitive array size and primitive array element size are based on the primitive type
// which could be an alone index or a group of index. 3 index for a NTRIANGLE, 2 index for a NLINE, 1 index for a NPOINT ...
//
// So, there are 2 functions to convert 32 bits index to 16 bits index and vice versa.

static void _convert_primitive_array_16_to_32(NGEOMETRY *pgeom)
{
	// retrieve number of index into the primitive

	Nu32 point_per_primitive = NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive / NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].SizeOfPrimitive;
	NErrorIf(!point_per_primitive, NERROR_SYSTEM_CHECK);
	Nu32 nbindex = pgeom->Mesh.PrimitiveArray.Size * point_per_primitive;

	// 1) first of all, create a tmp array
	NARRAY _array;
	NSetupArray(&_array, pgeom->Mesh.PrimitiveArray.Capacity, pgeom->Mesh.PrimitiveArray.ElementSize * 2);
	_array.Size = pgeom->Mesh.PrimitiveArray.Size;

	Nu32 *p32 = (Nu32 *)_array.pFirst;
	Nu16 *p16 = (Nu16 *)pgeom->Mesh.PrimitiveArray.pFirst;
	for (Nu32 i = nbindex; i--; p32++, p16++)
	{
		*p32 = (Nu32)(*p16);
	}

	NSwapArrayContent(&_array, &pgeom->Mesh.PrimitiveArray);
	NClearArray(&_array, NULL); // clear the content of tmp array ( which was the content of primitive array before the swap ! )
}

static void _convert_primitive_array_32_to_16(NGEOMETRY *pgeom)
{
	Nu32 point_per_primitive = NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive / NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].SizeOfPrimitive;
	NErrorIf(!point_per_primitive, NERROR_SYSTEM_CHECK);
	Nu32 nbindex = pgeom->Mesh.PrimitiveArray.Size * point_per_primitive;

	// 1) first of all, create a tmp array
	NARRAY _array;
	NSetupArray(&_array, pgeom->Mesh.PrimitiveArray.Capacity, pgeom->Mesh.PrimitiveArray.ElementSize / 2);
	_array.Size = pgeom->Mesh.PrimitiveArray.Size;

	Nu32 *p32 = (Nu32 *)pgeom->Mesh.PrimitiveArray.pFirst;
	Nu16 *p16 = (Nu16 *)_array.pFirst;
	for (Nu32 i = nbindex; i--; p32++, p16++)
	{
		*p16 = (Nu16)(*p32);
	}

	NSwapArrayContent(&_array, &pgeom->Mesh.PrimitiveArray);
	NClearArray(&_array, NULL); // clear the content of tmp array ( which was the content of primitive array before the swap ! )
}

Nu32 NFileWriteGeometry(NFILE *pfile, const NGEOMETRY *pgeom)
{
	NHEADER_GEOMETRY header;

	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_GEOMETRY) != 1)
		return 0;

	memset(&header, 0, sizeof(NHEADER_GEOMETRY));
	header.IDS = pgeom->Core.IDS;
	if (NFileWrite(pfile, &header, sizeof(NHEADER_GEOMETRY), 1) != 1)
		return 0;

	// Mesh
	if (!NFileWriteArray(pfile, &pgeom->Mesh.VertexArray, NULL, 0))
		return 0;

	if (!NFileWriteArray(pfile, &pgeom->Mesh.PrimitiveArray, NULL, 0))
		return 0;

	// Appearance
	if (!NFileWriteAppearance(pfile, &pgeom->Appearance, NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)))
		return 0;

	return 1;
}

Nu32 NFileReadGeometry(NFILE *pfile, NGEOMETRY *pgeom)
{
	NHEADER_GEOMETRY header;
	NGEOMETRY_SPECS *prs;
	Nu32 version;
	NAPPEARANCE_FORMAT_ENUM read_format;

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NHEADER_GEOMETRY):
		if (NFileRead(pfile, &header, sizeof(NHEADER_GEOMETRY), 1) != 1)
			return 0;

		// FillUp By Hand
		prs = NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(header.IDS));
		NSetupGeometryComponent_Core(&pgeom->Core, prs->RenderFunction, header.IDS);

		// Mesh
		if (!NFileReadArray(pfile, &pgeom->Mesh.VertexArray, NULL, 0))
			return 0;
		if (!NFileReadArray(pfile, &pgeom->Mesh.PrimitiveArray, NULL, 0))
			return 0;
		// Appearance
		if (!NFileReadAppearance(pfile, &pgeom->Appearance, &read_format))
			return 0;
		// a final check, just for consistency
		NErrorIf(read_format != NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS), NERROR_FILE_READ_ERROR);

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Check for potential conversion of incoming Primitive Array
		// Primitive Array element size has to match with predefine "NPRIMITIVE_SPECS.SizeOfPrimitive" which is different
		// from opengl  to openglES.
		// The difference comes from the different vertex index type used on Opengl and OpenglES.
		//			Opengl :	32 bits Index	(Nu32)
		//			OpenglES:	16 bis Index	(Nu16)
		// ... If incoming and expected primitive size are different ... we need a conversion !
		//
		// But there are only 2 possible index type, 16 bits or 32 bits !
		// And, on the top of that, operating system oblige the type:
		//	32 bits for opengl		(WINDOWS)
		//	16 bits for openglES	(IOS and ANDROID)
		//
		if (pgeom->Mesh.PrimitiveArray.ElementSize > NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive)
		{
#ifdef _NWINDOWS
			NErrorIf(1, NERROR_SYSTEM_CHECK); // Strange that means incoming vertex index > 32 bits.
#endif

			// OPERATING SYSTEM IS _IOS or ANDROID
			// The only possible way is incoming Size is the 32 bits version of the primitive and expected size the 16 bits version.
			NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize / 2 != NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive, NERROR_SYSTEM_CHECK);
			// Check Point size is "sizeof(Nu16)" !
			NErrorIf(NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].SizeOfPrimitive != sizeof(Nu16), NERROR_SYSTEM_CHECK);
			_convert_primitive_array_32_to_16(pgeom);
		}
		else if (pgeom->Mesh.PrimitiveArray.ElementSize < NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive)
		{
#if defined _NIOS || defined _NANDROID
			NErrorIf(1, NERROR_SYSTEM_CHECK); // Strange that means incoming vertex index > 32 bits.
#endif

			// OPERATING SYSTEM IS WINDOWS
			// The only possible way is incoming Size is the 16 bits version of the primitive and expected size the 32 bits version.
			NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize * 2 != NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)].SizeOfPrimitive, NERROR_SYSTEM_CHECK);
			// Check Point size is "sizeof(Nu32)" !
			NErrorIf(NPrimitiveSpecs[NPRIMITIVE_FORMAT_POINT].SizeOfPrimitive != sizeof(Nu32), NERROR_SYSTEM_CHECK);
			_convert_primitive_array_16_to_32(pgeom);
		}
		return 1;

	// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}
