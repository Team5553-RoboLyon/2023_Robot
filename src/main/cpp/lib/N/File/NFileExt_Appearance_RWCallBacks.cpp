#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NCore.h"
#include "../NTexture.h"
#include "lib/N/NErrorHandling.h"
#include "../Core/NVersion.h"
#include "lib/N/File/NFile.h"
#include "../File/NFileExt.h"
#include "../Libraries/NLibraries.h"

#include "../Geometry/Components/NGeometryTextureMatrixAnimation.h"
#include "../Geometry/Components/NGeometryBlend.h"
#include "../Geometry/Components/NGeometryMaterial.h"
#include "NFileExt_Appearance_RWCallBacks.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + RWAppearance void functions
// These functions are not suppose to do anything !
Nu32 NWriteAppearanceTexture_Void(NFILE *pfile, const NTEXTURE *ptr) { return 1; }
Nu32 NReadAppearanceTexture_Void(NFILE *pfile, NTEXTURE **pptr) { return 1; }

Nu32 NWriteAppearanceBlend_Void(NFILE *pfile, const NBLEND *ptr) { return 1; }
Nu32 NReadAppearanceBlend_Void(NFILE *pfile, NBLEND **pptr) { return 1; }

Nu32 NWriteAppearanceMaterial_Void(NFILE *pfile, const NMATERIAL *ptr) { return 1; }
Nu32 NReadAppearanceMaterial_Void(NFILE *pfile, NMATERIAL **pptr) { return 1; }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + RWAppearance_...Data  functions
// +
// These functions are suppose to write/read all appearance component data !
// So, no reference here but all data included !
Nu32 NWriteAppearanceTexture_ImageData(NFILE *pfile, const NTEXTURE *ptr)
{
	return 0;
}
Nu32 NReadAppearanceTexture_ImageData(NFILE *pfile, NTEXTURE **pptr)
{
	return 0;
}

Nu32 NWriteAppearanceBlend_Data(NFILE *pfile, const NBLEND *ptr)
{
	return 0;
}

Nu32 NReadAppearanceBlend_Data(NFILE *pfile, NBLEND **ppblend)
{
	return 0;
}

Nu32 NWriteAppearanceMaterial_Data(NFILE *pfile, const NMATERIAL *ptr)
{
	return 0;
}

Nu32 NReadAppearanceMaterial_Data(NFILE *pfile, NMATERIAL **pptr)
{
	return 0;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + RWAppearance_...Reference  functions
// +
// These functions are suppose to write/read a reference on each appearance component !
// So, These component need to be save/load or created somewhere else in the code !
// Notes: the 2 RW UVMatrixAnim functions below don't exist. It's perfectly normal ...
//		1/--> void				NWriteAppearanceUVMatrixAnim_NUTLibrary_Reference(NFILE *pfile,const NUVMATRIXANIM *ptr)
//		2/--> NUVMATRIXANIM*	NReadAppearanceUVMatrixAnim_NUTLibrary_Reference(NFILE *pfile)
//		... because NUVMATRIXANIM structures aren't stored into library like textures but specific at each NAPPEARANCE Texture Unit.

Nu32 NWriteAppearanceTexture_NUTLibrary_Reference(NFILE *pfile, const NTEXTURE *ptr)
{
	NHEADER_LIBRARY_ENTRY header;

	Nmem0(&header, NHEADER_LIBRARY_ENTRY);
	if (ptr)
	{
		header.KeySize = strlen(ptr->pName);
	}
	else
	{
		header.KeySize = 0;
	}

	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_LIBRARY_ENTRY) != 1)
		return 0;

	if (NFileWrite(pfile, &header, sizeof(NHEADER_LIBRARY_ENTRY), 1) != 1)
		return 0;

	if (header.KeySize)
	{
		if (NFileWrite(pfile, ptr->pName, sizeof(Nchar), header.KeySize) != header.KeySize)
			return 0;
	}

	// Actually, we are going to Write more than just a reference.
	// An other way could be an integration of these parameters into the TGA File.
	// But, write now it's like this ...
	// ! Maybe it will change in the future !
	if (!NFileWriteTextureParameters(pfile, ptr))
		return 0;

	return 1;
}
Nu32 NReadAppearanceTexture_NUTLibrary_Reference(NFILE *pfile, NTEXTURE **pptr)
{
	NErrorIf(!pfile | !pptr, NERROR_NULL_POINTER);

	NHEADER_LIBRARY_ENTRY header;
	Nu32 version;
	Nchar key[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current Main Version (all versions 0.0.x )
	case NGETVERSION_MAIN(VERSION_NHEADER_LIBRARY_ENTRY):
		if (NFileRead(pfile, &header, sizeof(NHEADER_LIBRARY_ENTRY), 1) != 1)
			return 0;

		if (header.KeySize)
		{
			NErrorIf(header.KeySize + 1 > CONSTANT_NFILE_FULLDATAPATH_MAXSIZE, NERROR_STRING_TOO_LONG); // Avoiding writing data out of 'key[]'

			if (NFileRead(pfile, key, sizeof(Nchar), header.KeySize) != header.KeySize)
				return 0;

			key[header.KeySize] = 0; // Null terminated character was not saved, so, add it !
			*pptr = NGetLibraryTexture(key);
			// Read Texture FILTERS and Texture WRAP parameters
			// Actually, we are going to Read more than just a reference.
			// An other way could be an integration of these parameters into the TGA File.
			// But, now it's like this ...
			// ! Maybe it will change in the future !
			if (*pptr && !NFileReadTextureParameters(pfile, *pptr))
				return 0;
		}
		else
		{
			*pptr = NULL;
		}
		return 1;

		// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}

Nu32 NWriteAppearanceBlend_NUTLibrary_Reference(NFILE *pfile, const NBLEND *ptr)
{
	NErrorIf(!pfile | !ptr, NERROR_NULL_POINTER);

	NHEADER_LIBRARY_ENTRY header;

	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_LIBRARY_ENTRY) != 1)
		return 0;

	Nmem0(&header, NHEADER_LIBRARY_ENTRY);

	if (ptr)
	{
		//		NErrorIf(!NIsRootRelativePathFileName(ptr->pName),NERROR_FILE_FILENAME_MUST_BE_ROOT_RELATIVE);
		header.KeySize = strlen(ptr->pName);
	}
	else
	{
		header.KeySize = 0;
	}

	if (NFileWrite(pfile, &header, sizeof(NHEADER_LIBRARY_ENTRY), 1) != 1)
		return 0;

	if (header.KeySize && (NFileWrite(pfile, ptr->pName, sizeof(Nchar), header.KeySize) != header.KeySize))
	{
		return 0;
	}
	return 1;
}

Nu32 NReadAppearanceBlend_NUTLibrary_Reference(NFILE *pfile, NBLEND **pptr)
{

	NErrorIf(!pfile | !pptr, NERROR_NULL_POINTER);

	NHEADER_LIBRARY_ENTRY header;
	Nu32 version;
	//	NBLEND					*pblend = NULL;
	Nchar key[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current Main Version (all versions 0.0.x )
	case NGETVERSION_MAIN(VERSION_NHEADER_LIBRARY_ENTRY):
		if (NFileRead(pfile, &header, sizeof(NHEADER_LIBRARY_ENTRY), 1) != 1)
			return 0;

		if (header.KeySize)
		{
			NErrorIf(header.KeySize + 1 > CONSTANT_NFILE_FULLDATAPATH_MAXSIZE, NERROR_STRING_TOO_LONG); // Avoiding writing data out of 'key[]'

			if (NFileRead(pfile, key, sizeof(Nchar), header.KeySize) != header.KeySize)
				return 0;

			key[header.KeySize] = 0; // Null terminated character was not saved, so, add it !

			*pptr = NGetLibraryBlend(key);
		}
		else
		{
			*pptr = NULL;
		}
		return 1;

	// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
}

Nu32 NWriteAppearanceMaterial_NUTLibrary_Reference(NFILE *pfile, const NMATERIAL *ptr)
{
	// TODO
	return 0;
}
Nu32 NReadAppearanceMaterial_NUTLibrary_Reference(NFILE *pfile, NMATERIAL **pptr)
{
	// TODO
	return 0;
}
