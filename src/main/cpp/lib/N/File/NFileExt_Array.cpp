#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../Containers/NArray.h"
#include "lib/N/File/NFile.h"
#include "../File/NFileExt.h"
#include "lib/N/NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// NFileWriteArray
// ------------------------------------------------------------------------------------------
// Description :
//	Write an Array
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			:	a Valid pointer on an open file
//		parray			:	a valid pointer on an array
//		write_element	:	an optional call back function called to save each array element
//		userdata		:	a user value pass as the third parameter of the write_element function
// Out :
//		1 on success
//		0 on Error.	"parray" still available.
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteArray(NFILE *pfile, const NARRAY *parray, NARRAY_RWELEMENT_CALLBACK write_element, Nu32 userdata)
{
	NErrorIf(!pfile || !parray, NERROR_NULL_POINTER);
	NErrorIf(parray->Capacity < parray->Size, NERROR_ARRAY_CORRUPTED);

	NHEADER_ARRAY arrayheader;
	NBYTE *ptr;
	Nu32 i;
	//	Nu32			written = 0;

	// Write Current version Tag
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_ARRAY) != 1)
		return 0;

	memset(&arrayheader, 0, sizeof(NHEADER_ARRAY));
	arrayheader.Capacity = parray->Capacity;
	arrayheader.ElementSize = parray->ElementSize;
	arrayheader.Size = parray->Size;
	if (NFileWrite(pfile, &arrayheader, sizeof(NHEADER_ARRAY), 1) != 1)
		return 0;

	if (parray->Size * parray->ElementSize)
	{
		if (!write_element)
		{
			if (NFileWrite(pfile, parray->pFirst, parray->ElementSize, parray->Size) != parray->Size)
				return 0;
		}
		else
		{
			ptr = parray->pFirst;
			for (i = parray->Size; i != 0; i--, ptr += parray->ElementSize)
			{
				if (!write_element(ptr, pfile, userdata))
				{
					NErrorIf(1, NERROR_FILE_WRITE_ERROR);
					return 0;
				}
			}
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NFileReadArray
// ------------------------------------------------------------------------------------------
// Description :
//	Read an Array
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			:	a Valid pointer on an open file
//		parray			:	a valid pointer on an "empty" array
//		read_element	:	an optional call back function called to load content of each array element
//							Notice that the array element is already created/allocated and full of ZERO when calling 'read_element'
//		userdata		:	a user value pass as the third parameter of the read_element function
// Out :
//		1 on success
//		0 on Error.		"parray" still available and, even if its content is uncertain regarding what it was suppose to read,
//						it still AVAILABLE with the number of element effectively read inside. ( parray->Size = number of read element)
//						IT CAN ALWAYS BE CLEARED WITH "NClearArray".
// ------------------------------------------------------------------------------------------
Nu32 NFileReadArray(NFILE *pfile, NARRAY *parray, NARRAY_RWELEMENT_CALLBACK read_element, Nu32 userdata)
{
	NErrorIf(!pfile || !parray, NERROR_NULL_POINTER);
	// The incoming Array as to be previously made empty by user.
	// But doing that only in DEBUG mode because this potential Error is a logical error caused by programming mistakes ...
	NErrorIf(parray->Size, NERROR_ARRAY_NOT_EMPTY);

	NHEADER_ARRAY arrayheader;
	NBYTE *ptr;
	Nu32 i;
	Nu32 read = 0;
	Nu32 version;
	Nu32 bloc_size;

	// Check Array version.
	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current main version (alls versions 0.0.x):
	case NGETVERSION_MAIN(VERSION_NHEADER_ARRAY):
		memset(&arrayheader, 0, sizeof(NHEADER_ARRAY));
		if (NFileRead(pfile, &arrayheader, sizeof(NHEADER_ARRAY), 1) != 1)
			return 0;

		NErrorIf(arrayheader.Capacity < arrayheader.Size, NERROR_ARRAY_CORRUPTED);

		bloc_size = arrayheader.ElementSize * arrayheader.Capacity;
		// "Re-init" the array by hand:
		// Actually, we are sure that  incoming array is empty but that doesn't mean 'cleared' without any memory allocation.
		// So we have to re-initialize the array to fit with loading data in.
		if (bloc_size == 0)
		{
			if (parray->ElementSize * parray->Capacity)
			{
				NErrorIf(!parray->pFirst, NERROR_ARRAY_CORRUPTED);
				Nfree(parray->pFirst);
				parray->pFirst = NULL;
			}
			else
			{
				NErrorIf(parray->pFirst, NERROR_ARRAY_CORRUPTED);
			}
			parray->Size = arrayheader.Size;
			parray->ElementSize = arrayheader.ElementSize;
			parray->Capacity = arrayheader.Capacity;
		}
		else
		{
			if (parray->ElementSize * parray->Capacity != bloc_size)
				parray->pFirst = (NBYTE *)Nrealloc(parray->pFirst, bloc_size);

			// Fill up all the array with ZEROS
			memset(parray->pFirst, 0, bloc_size);

			parray->Size = arrayheader.Size;
			parray->ElementSize = arrayheader.ElementSize;
			parray->Capacity = arrayheader.Capacity;

			// If array contains some element ... Read them !
			if (parray->Size)
			{
				if (!read_element)
				{
					read = NFileRead(pfile, parray->pFirst, parray->ElementSize, parray->Size);
					if (read != parray->Size)
					{
						parray->Size = read; // force size of the array to the real read element number
						return 0;			 // return 0 to inform user of an error
					}
				}
				else
				{
					ptr = parray->pFirst;
					for (i = parray->Size; i != 0; i--, ptr += parray->ElementSize)
					{
						if (read_element(ptr, pfile, userdata) == 1)
						{
							read++;
						}
						else
						{
							parray->Size = read; // force size of the array to the real read element number
							return 0;			 // return 0 to inform user of an error
						}
					}
				}
			}
		}
		break;

	// Unknown version:
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}

	return 1;
}
