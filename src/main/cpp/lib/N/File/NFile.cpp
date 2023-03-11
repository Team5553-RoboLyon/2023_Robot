#include "lib/N/NCStandard.h"
#include "lib/N/NCore.h"
#include "lib/N/GL/Ngl.h"

#include "lib/N/NType.h"

#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NString.h"
#include "lib/N/NErrorHandling.h"

char *pNRootRelativePath[CONSTANT_NROOT_RELATIVEPATH_COUNT];
Nu32 NRootRelativePathID;

void NSetupRootRelativePath(const Nu32 id, const Nchar *prootpath)
{
	NErrorIf(id >= CONSTANT_NROOT_RELATIVEPATH_COUNT, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(pNRootRelativePath[id], NERROR_NON_NULL_POINTER);
	if (!pNRootRelativePath[id])
		pNRootRelativePath[id] = NStrDuplicate(prootpath);
}

void NClearRootRelativePath(const Nu32 id)
{
	NErrorIf(id >= CONSTANT_NROOT_RELATIVEPATH_COUNT, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(!pNRootRelativePath[id], NERROR_NULL_POINTER);
	Nfree(pNRootRelativePath[id]);
	pNRootRelativePath[id] = NULL;
}

void NClearAllRootRelativePath()
{
	for (Nu32 i = 0; i < CONSTANT_NROOT_RELATIVEPATH_COUNT; i++)
	{
		Nfree(pNRootRelativePath[i]);
		pNRootRelativePath[i] = NULL;
	}
}

Nu32 NSetCurrentRootRelativePath(const Nu32 id)
{
	NErrorIf(id >= CONSTANT_NROOT_RELATIVEPATH_COUNT, NERROR_VALUE_OUTOFRANGE);
	Nu32 previousid = id;
	NRootRelativePathID = id;

	return previousid;
}

// ------------------------------------------------------------------------------------------
// NCheckFileType
// ------------------------------------------------------------------------------------------
// Description :
//	check the file extension
//	... just the file extension, there is no garantie the file is what it pretends to be !
// ------------------------------------------------------------------------------------------
// In  :
//		filename:	ptr on the data filename name
//		ext:		ptr on the extension string form like this: ".ext" with dot.
//
// Out :
//		NTRUE		if 'ext' is the right extension of 'filename'
//		NFALSE		if not
// ------------------------------------------------------------------------------------------
Nbool NCheckFileType(const Nchar *filename, const Nchar *ext)
{
	NErrorIf(ext[0] != '.', NERROR_FILE_INVALID_EXTENSION);
	// Check file description extension
	if (strlen(filename) < strlen(ext) + 1) // at minimum one letter + extension ...
		return NFALSE;
	else
		return NStrCheckEnd(filename, ext);

	/* previous version ( updated 23/05/2022 )
	NErrorIf(!filename || !ext,NERROR_NULL_POINTER);
	NErrorIf(ext[0]!='.',NERROR_FILE_INVALID_EXTENSION);

	const Nchar *pfilext;
	//Check file description extension
	if (strlen(filename) < strlen(ext) + 1) // at minimum one letter + extension ...
		return NFALSE;
	else
	{
		pfilext = filename + strlen(filename) - strlen(ext);
		if (strcmp(pfilext, ext))
			return NFALSE;
	}

	return NTRUE;
	*/
}

// ------------------------------------------------------------------------------------------
// Nchar* NGetFullFileName
// ------------------------------------------------------------------------------------------
// Description :
//	check and build (or not) the full file name that is going to be used by file open or what ever
//	as full file path to access a file.
//	function, returns a pointer on "pbuilfullfilename" if it build it or on pfilename is it was not necessary to complete it...
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nchar *NGetFullFileName(Nchar *pbuildfullfilename, const Nchar *pfilename, const Nu32 flags_filesystem_path)
{
	// Only the filesystem PATH flags are going to be used here, so its better if only these ones are set ... ( it's not ab error just a safe rule )
	NErrorIf((flags_filesystem_path & MASK_NFILE_PATH_FILESYSTEM) != flags_filesystem_path, NERROR_SYSTEM_CHECK);

	// Just in case of NENGINE inconsistency evolutions ...
	NErrorIf(MASK_NENGINE_CORE_PATH_FILESYSTEM != MASK_NFILE_PATH_FILESYSTEM, NERROR_SYSTEM_NENGINEVERSION_CONFLICT);

	Nu32 filesystem_path;
	Nchar *pusedfilename = NULL;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + FILENAME BUILDING
	// +
	// Flags checking
	// Rules:	if incoming flags contain File system path directive, these ones are taking in account.
	//			Otherwise, default NEngine File system Path directive are used.
	if (!(flags_filesystem_path & MASK_NFILE_PATH_FILESYSTEM))
	{
		filesystem_path = (NEngineCoreFlags & MASK_NENGINE_CORE_PATH_FILESYSTEM);
	}
	else
	{
		filesystem_path = (flags_filesystem_path & MASK_NFILE_PATH_FILESYSTEM);
	}

	// Name building accordingly with the current 'filesystem_path'
	switch (filesystem_path)
	{
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +-+ Relative Path

	case (FLAG_NFILE_APP_ROOT_RELATIVE_PATH):
		NErrorIf(pfilename[1] == ':', NERROR_UNAUTHORIZED_DRIVE_ROOT_FILENAME); // filename is something like "c:..." or "d:..." and it's totally unauthorized here

#if defined _NWINDOWS || defined _NANDROID
		NErrorIf(pfilename[0] == '/', NERROR_FILE_INCONSISTENT_FILENAME); // filename is something like "/blabla... "and it's totally unauthorized here
		NErrorIf(strlen(pfilename) + strlen(pNRootRelativePath[NRootRelativePathID]) >= CONSTANT_NFILE_FULLDATAPATH_MAXSIZE, NERROR_STRING_TOO_LONG);
		NErrorIf(strstr(pfilename, pNRootRelativePath[NRootRelativePathID]) == pfilename, NERROR_FILE_INCONSISTENT_FILENAME); // Root Relative Path filename can not contain Root Path !

		pusedfilename = pbuildfullfilename;
		sprintf(pusedfilename, "%s%s", pNRootRelativePath[NRootRelativePathID], pfilename);
#endif
#ifdef _NIOS
		NErrorIf(pfilename[0] == '/', NERROR_FILE_INCONSISTENT_FILENAME); // filename is something like "/blabla... "and it's totally unauthorized here
		NErrorIf(strlen(pfilename) + strlen(DatasPathFolder) >= CONSTANT_NFILE_FULLDATAPATH_MAXSIZE, NERROR_STRING_TOO_LONG);
		NErrorIf(strstr(pfilename, DatasPathFolder) == pfilename, NERROR_FILE_INCONSISTENT_FILENAME); // Root Relative Path filename can not contain Root Path !
		// Due to all the IOS files are stored without any folder ...

		// we have to keep the filename without any folder references ...
		Nu32 ios_i = strlen(pfilename);
		while (ios_i--)
		{
			if (pfilename[ios_i] == '/')
			{
				ios_i++;
				break;
			}
		}
		pusedfilename = pbuildfullfilename;
		sprintf(pusedfilename, "%s%s%s", DatasPathFolder, "/", &pfilename[ios_i]);
#endif
		break;

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +-+ Absolute Path
	case (FLAG_NFILE_FULL_PATH):
#ifdef _NWINDOWS
		NErrorIf(pfilename[1] != ':', NERROR_FILE_INCONSISTENT_FILENAME); // filename has to be like "c:..." or "d:..." if not ... it's not a windows absolute PATH !
#endif
#ifdef _NANDROID						  // == Windows version ... ???
		NErrorIf(1, NERROR_SYSTEM_CHECK); // Check exactly what an ANDROID absolute PATH truly is and if it's authorized after all !
#endif
#ifdef _NIOS
		NErrorIf(1, NERROR_SYSTEM_CHECK); // Check exactly what an IOS absolute PATH truly is and if it's authorized after all !
#endif
		pusedfilename = (Nchar *)pfilename;
		break;

		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +-+ Automatic Path mode ( evaluate the incoming filename and decide ... )
	case (FLAG_NFILE_APP_ROOT_RELATIVE_PATH | FLAG_NFILE_FULL_PATH):
#ifdef _NWINDOWS
		// Check for Absolute Path first
		if (pfilename[1] == ':') // filename is something like "c:..." or "d:..." that's the only possible way for an absolute path to be
		{
			pusedfilename = (Nchar *)pfilename;
		}
		// Consider pfilename as an app root relative path filename
		else
		{
			NErrorIf(pfilename[0] == '/', NERROR_FILE_INCONSISTENT_FILENAME); // filename is something like "/blabla... "and it's totally unauthorized here
			NErrorIf(strlen(pfilename) + strlen(pNRootRelativePath[NRootRelativePathID]) >= CONSTANT_NFILE_FULLDATAPATH_MAXSIZE, NERROR_STRING_TOO_LONG);
			NErrorIf(strstr(pfilename, pNRootRelativePath[NRootRelativePathID]) == pfilename, NERROR_FILE_INCONSISTENT_FILENAME); // Root Relative Path filename can not contain Root Path !

			pusedfilename = pbuildfullfilename;
			sprintf(pusedfilename, "%s%s", pNRootRelativePath[NRootRelativePathID], pfilename);
		}
#endif
#ifdef _NANDROID						  // == Windows version ... ???
		NErrorIf(1, NERROR_SYSTEM_CHECK); // Check exactly what an ANDROID absolute PATH truly is and if it's authorized after all !
#endif
#ifdef _NIOS
		NErrorIf(1, NERROR_SYSTEM_CHECK); // Check exactly what an IOS absolute PATH truly is and if it's authorized after all !
#endif
		break;

	default:
		NErrorIf(1, NERROR_NFILESYSTEM_UNDEFINED_PATH_MODE);
		break;
	}

	return pusedfilename;
}

// ------------------------------------------------------------------------------------------
// NFileOpen
// ------------------------------------------------------------------------------------------
// Description :
//	open a file. Depending the flags set, the file will be ready for:
//		reading or writing,
//		on HDD or in Memory,
//		Binary or Text
//		with or without taglist management included
// ------------------------------------------------------------------------------------------
// In  :
//		filename:	ptr on the data filename name
//		flags:		All the flag set to specify the desired "open way"
//					FLAG_NFILE_MEMORY		file is not a file but a memory bloc (Not implemented yet !)
//					FLAG_NFILE_BINARY		file will be open as a Binary stream ("b"with 'fopen'). If OFF, file will be open as a formated text stream
//					FLAG_NFILE_READ			file will be open for reading
//					FLAG_NFILE_WRITE		file will be open for writing
//											(necessary for a right use of all the "Tag" functions)
//
//		!!!			Some flags are not compatible so, it's impossible to set at the same time:
//					FLAG_NFILE_WRITE and FLAG_NFILE_READ
//					FLAG_NFILE_ROOT_RELATIVE_PATHS and FLAG_NFILE_ABSOLUTE_PATHS
//					This will generate an error and NFileOpen function will return a NULL pointer.
// Out :
//		A valid pointer on a "NFILE" structure used by all the other NFILE functions
//		to manipulate the file.
//		or
//		NULL in case of Error.
// ------------------------------------------------------------------------------------------
NFILE *NFileOpen(const Nchar *pfilename, const Nu32 flags)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + FLAGS INCONSISTENCY CHECKING
	// +
	//	 FLAGS inconsistency checking : In debug mode ... CRASH !
	NErrorIf(ISFLAG_ON(flags, FLAG_NFILE_WRITE) && ISFLAG_ON(flags, FLAG_NFILE_READ), NERROR_FILE_INCONSISTENT_FLAGS);
	Nchar fullpathname[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];

	Nchar *pusedfilename = NGetFullFileName(fullpathname, pfilename, flags & MASK_NFILE_PATH_FILESYSTEM);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + FILE CREATION
	// +
	NFILE *pfile = NEW(NFILE);
	memset(pfile, 0, sizeof(NFILE));

	FLAG_ON(pfile->Flags, flags);
	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
	}
	else
	{
		if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_WRITE))
		{
			if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_BINARY))
			{
				if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_UPDATE))
					pfile->pFile = fopen(pusedfilename, "wb+"); // Binary File. Write/Update . The file is created or discarded if it exists and is treated as a new empty file!
				else
					pfile->pFile = fopen(pusedfilename, "wb"); // Binary File. Write. The file is created or discarded if it exists and is treated as a new empty file!
			}
			else // if(ISFLAG_OFF(pfile->Flags,FLAG_NFILE_BINARY))  That means Text Stream !
			{
				if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_UPDATE))
					pfile->pFile = fopen(pusedfilename, "w+"); // Text file. Write/Update. The file is created or discarded if it exists and is treated as a new empty file!
				else
					pfile->pFile = fopen(pusedfilename, "w"); // Text file. Write only. The file is created or discarded if it exists and is treated as a new empty file!
			}
		}
		else // ISFLAG_ON( pfile->Flags,FLAG_NFILE_READ )... We are sure of that !
		{
			if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_BINARY))
			{
				if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_UPDATE))
					pfile->pFile = fopen(pusedfilename, "rb+"); // BINARY file. Read/Update. The file must exists !
				else
					pfile->pFile = fopen(pusedfilename, "rb"); // BINARY file. Read only. The file must exists !
			}
			else
			{
				if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_UPDATE))
					pfile->pFile = fopen(pusedfilename, "r+"); // text file. Read/Update. The file must exists !
				else
					pfile->pFile = fopen(pusedfilename, "r"); // text file. Read only. The file must exists !
			}
		}
	}

	if (!pfile->pFile)
	{
		Nfree(pfile);
		return NULL;
	}
	else
	{
		return pfile;
	}
}
// ------------------------------------------------------------------------------------------
// NFileClose
// ------------------------------------------------------------------------------------------
// Description :
//	Close a file, previously open with NFileOpen.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile:		A valid pointer to a NFILE previously created by NFileOpen function
// Out :
//		0			if the file successfully closed.
//		non-zero 	if not. (it returns EOF, that it should be -1 )
//
//					Even if the call fails, the stream passed as parameter will no longer be associated with the file nor its buffers.
//					... AND NFILE *pfile is free.
// ------------------------------------------------------------------------------------------
Ns32 NFileClose(NFILE *pfile)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		Ns32 ret = _SafeNintToNs32(fclose(pfile->pFile));
		NErrorIf(ret != 0, NERROR_FILE_FCLOSE);
		Nfree(pfile); // it 's not going to be effectively free but ... that's the idea...
		return ret;	  // it 's not going to be effectively returned but ... that's the idea...
#endif
#ifndef _DEBUG
		Ns32 ret = fclose(pfile->pFile);
		Nfree(pfile);
		return ret;
#endif
	}
}

// ------------------------------------------------------------------------------------------
// void NFileTell
// ------------------------------------------------------------------------------------------
// Description :
//	Get current position indicator associated with the stream.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//	On success, the current value of the position indicator is returned.
//	On failure, -1 is returned.
// ------------------------------------------------------------------------------------------
Ns32 NFileTell(NFILE *pfile)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		Ns32 ret = (Ns32)ftell(pfile->pFile);
		NErrorIf(ret == -1, NERROR_FILE_FTELL);
		return ret; // it 's not going to be effectively returned but ... that's the idea...
#endif
#ifndef _DEBUG
		return (Ns32)ftell(pfile->pFile);
#endif
	}
}

// ------------------------------------------------------------------------------------------
// NFileSeek
// ------------------------------------------------------------------------------------------
// Description :
//	Sets the position indicator associated with the stream to a new position.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile:		A valid pointer to a NFILE previously created by NFileOpen function
//
//		offset:		Binary files: Number of bytes to offset from origin.
//					Text files: Either zero, or a value returned by ftell.
//
//		origin:		Position used as reference for the offset.
//					It is specified by one of the following constants
// Out :
//		If successful, the function returns zero.
//		Otherwise, it returns non-zero value.
//
// ------------------------------------------------------------------------------------------
Ns32 NFileSeek(NFILE *pfile, const Nu32 offset, const NFILE_SEEKORIGIN origin)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
		// correspondances:
		// NFILE_SEEK_SET	->  SEEK_SET
		// NFILE_SEEK_CUR	->  SEEK_CUR
		// NFILE_SEEK_END	->	SEEK_END

		Nu32 seekvalue[3] = {SEEK_SET, SEEK_CUR, SEEK_END};

#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		Ns32 ret = _SafeNintToNs32(fseek(pfile->pFile, offset, seekvalue[origin]));
		NErrorIf(ret != 0, NERROR_FILE_FSEEK);
		return ret; // it 's not going to be effectively returned but ... that's the idea...
#endif

#ifndef _DEBUG
		return (Ns32)fseek(pfile->pFile, offset, seekvalue[origin]);
#endif
	}
}

void NFileRewind(NFILE *pfile)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		rewind(pfile->pFile);
		NErrorIf(ferror(pfile->pFile), NERROR_FILE); // An Error occurred with rewind !
#endif
#ifndef _DEBUG
		rewind(pfile->pFile);
#endif
	}
}

// ------------------------------------------------------------------------------------------
// Nchar *NFileGetString
// ------------------------------------------------------------------------------------------
// Description :
//	Reads characters from stream and stores them as a C string into str
//	until (num-1) characters have been read or either a newline or the end-of-file is reached, whichever happens first.
//	A terminating null character is automatically appended after the characters copied to str.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nchar *NFileGetString(Nchar *pstring, Nu32 size_null_end_included, NFILE *pfile)
{
	NErrorIf(!pfile || !pstring, NERROR_NULL_POINTER);
	NErrorIf(size_null_end_included < 2, NERROR_VALUE_UNEXPECTED); // Because null character is included a value of 1 ( or less ) would mean : "just the end character" ( or nothing if  0 ) !!!

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return NULL; // for the moment
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ISFLAG_OFF(pfile->Flags, FLAG_NFILE_READ), NERROR_FILE_INCONSISTENT_FLAGS);
		NErrorIf(ISFLAG_ON(pfile->Flags, FLAG_NFILE_BINARY), NERROR_FILE_INCONSISTENT_FLAGS);
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		return fgets(pstring, size_null_end_included, pfile->pFile);
#endif

#ifndef _DEBUG
		return fgets(pstring, size_null_end_included, pfile->pFile);
#endif
	}
}

// ------------------------------------------------------------------------------------------
// NFileRead
// ------------------------------------------------------------------------------------------
// Description :
//	Read block of data from stream
// ------------------------------------------------------------------------------------------
// In  :
//		pfile:		A valid pointer to a NFILE previously created by NFileOpen function
//
//		ptr:		Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
//
//		elementsize:Size, in bytes, of each element to be read.
//
//		elementnb:	Number of elements, each one with a size of size bytes.
//
// Out :
//		The total number of elements successfully read is returned.
//
// ------------------------------------------------------------------------------------------
Nu32 NFileRead(NFILE *pfile, void *ptr, const Nu32 element_size_in_byte, const Nu32 element_count)
{
	NErrorIf(!pfile || !ptr, NERROR_NULL_POINTER);
	NErrorIf(!element_size_in_byte, NERROR_FILE_ELEMENT_SIZE_IS_NULL);
	NErrorIf(!element_count, NERROR_FILE_ELEMENT_COUNT_IS_NULL);
	NErrorIf(ISFLAG_OFF(pfile->Flags, FLAG_NFILE_READ), NERROR_FILE_READ_ERROR);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		// pfile->RWPos += readelement * elementsize;
		return 0;
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		Nu32 readelement = fread(ptr, element_size_in_byte, element_count, pfile->pFile);
		NErrorIf(readelement != element_count, NERROR_FILE_FREAD_ERROR);
		return readelement; // it 's not going to be effectively returned but ... that's the idea...
#endif

#ifndef _DEBUG
		return (Nu32)fread(ptr, element_size_in_byte, element_count, pfile->pFile);
#endif
	}
}

// ------------------------------------------------------------------------------------------
// NFileWrite
// ------------------------------------------------------------------------------------------
// Description :
//	Write block of data to stream
// ------------------------------------------------------------------------------------------
// In  :
//		pfile:		A valid pointer to a NFILE previously created by NFileOpen function
//
//		ptr:		Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
//
//		elementsize:Size, in bytes, of each element to be written.
//
//		elementnb:	Number of elements, each one with a size of size bytes.
//
// Out :
//		The total number of elements successfully written is returned.
//
// ------------------------------------------------------------------------------------------
Nu32 NFileWrite(NFILE *pfile, const void *ptr, const Nu32 element_size_in_byte, const Nu32 element_count)
{
	NErrorIf(!pfile || !ptr, NERROR_NULL_POINTER);
	NErrorIf(!element_size_in_byte, NERROR_FILE_ELEMENT_SIZE_IS_NULL);
	NErrorIf(!element_count, NERROR_FILE_ELEMENT_COUNT_IS_NULL);
	NErrorIf(ISFLAG_OFF(pfile->Flags, FLAG_NFILE_WRITE) && ISFLAG_OFF(pfile->Flags, FLAG_NFILE_UPDATE), NERROR_FILE_WRITE_ERROR);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
#ifdef _DEBUG
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.
		Nu32 writtenelement = fwrite(ptr, element_size_in_byte, element_count, pfile->pFile);
		NErrorIf(writtenelement != element_count, NERROR_FILE_WRITE_ERROR);
		return writtenelement; // it 's not going to be effectively returned but ... that's the idea...
#endif

#ifndef _DEBUG
		return (Nu32)fwrite(ptr, element_size_in_byte, element_count, pfile->pFile);
#endif
	}
}

Ns32 NFileError(const NFILE *pfile)
{
	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
		return _SafeNintToNs32(ferror(pfile->pFile));
	}
}
Ns32 NFileEOF(const NFILE *pfile)
{
	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		return 0;
	}
	else
	{
		return _SafeNintToNs32(feof(pfile->pFile));
	}
}
void NFileClearError(const NFILE *pfile)
{
	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
	}
	else
	{
		clearerr(pfile->pFile);
	}
}

void NPrintErrorMessage(const Nchar *str)
{
	perror(str);
}
// ------------------------------------------------------------------------------------------
// NFileGetSize
// ------------------------------------------------------------------------------------------
// Description :
//	return the current file size.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile:		A valid pointer to a NFILE previously created by NFileOpen function
//
// Out :
//		size of file in BYTES.
//		or
//		-1 if an error occurred.
// ------------------------------------------------------------------------------------------
Ns32 NFileGetSize(NFILE *pfile)
{

	NErrorIf(!pfile, NERROR_NULL_POINTER);

	Ns32 size, offset;

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		// pfile->RWPos += writtenelement * elementsize;
	}
	else
	{
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.

		offset = ftell(pfile->pFile);
		fseek(pfile->pFile, 0, SEEK_END); // TODO: check if it's working with IOS, it's not sure ...
		size = ftell(pfile->pFile);
		fseek(pfile->pFile, offset, SEEK_SET);

		NErrorIf(ferror(pfile->pFile), NERROR_FILE); // ftell or fseek previous call ( in the block up to it ) generate an error ...
	}

	if (ferror(pfile->pFile))
	{
		return -1;
	}
	else
	{
		return size;
	}
}

Nu32 NFileWriteVersionTag(NFILE *pfile, const Nu32 version)
{
	return NFileWrite(pfile, &version, sizeof(Nu32), 1);
}

Nu32 NFileReadVersionTag(NFILE *pfile, Nu32 *pversion)
{
	return NFileRead(pfile, pversion, sizeof(Nu32), 1);
}

Nu32 NFileWriteSignatureTag(NFILE *pfile, const Nu32 sign)
{
	return NFileWrite(pfile, &sign, sizeof(Nu32), 1);
}

Nu32 NFileReadSignatureTag(NFILE *pfile, Nu32 *psign)
{
	return NFileRead(pfile, psign, sizeof(Nu32), 1);
}

Nu32 NFileReadHeader(NFILE *pfile, NFILE_HEADER *pheader)
{
	return NFileRead(pfile, pheader, sizeof(NFILE_HEADER), 1);
}

// ------------------------------------------------------------------------------------------
// Nu32 NFileWriteHeader_Emplace
// ------------------------------------------------------------------------------------------
// Description :
//	Write Header into position in the file with signature and version data inside.
//	At the same time, the position in file is store into the NFILE_HEADER.Size member.
//	This value is going to be used by NFileWriteHeader_CompleteBlockSize to retrieve the
//	Header position in the file and write down the block size.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//			 0 if OK
//			-1 on Error
// ------------------------------------------------------------------------------------------
Ns32 NFileWriteHeader_Emplace(NFILE *pfile, NFILE_HEADER *pfile_header, const Nu32 sign, const Nu32 version)
{
	NErrorIf(!pfile || !pfile_header, NERROR_NULL_POINTER);
	NErrorIf(ISFLAG_OFF(pfile->Flags, FLAG_NFILE_WRITE), NERROR_FILE_WRITE_ERROR);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		// pfile->RWPos += writtenelement * elementsize;
	}
	else
	{
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.

		pfile_header->Signature = sign;
		pfile_header->Version = version;
		pfile_header->Size = NVOID;

		Ns32 tell_value = ftell(pfile->pFile);
		Nu32 offset;
		if (tell_value == -1)
		{
			NErrorIf(1, NERROR_FILE_FTELL);
			return -1;
		}
		else
		{
			offset = _SafeNs32ToNu32(tell_value) + sizeof(Nu32) * 2;
		}

		if (fwrite(pfile_header, sizeof(NFILE_HEADER), 1, pfile->pFile) != 1) // write "pfile_header" with its Size member equal to NVOID
		{
			NErrorIf(1, NERROR_FILE_FWRITE);
			return -1;
		}

		pfile_header->Size = offset; // Store the offset of "pfile_header->Size" member in the file into "pfile_header->Size" itself !!!
	}
	return 0;
}

// ------------------------------------------------------------------------------------------
// void NFileWriteHeader_CompleteBlockSize
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//			 0 if OK
//			-1 on Error
// ------------------------------------------------------------------------------------------
Ns32 NFileWriteHeader_CompleteBlockSize(NFILE *pfile, const NFILE_HEADER *pfile_header_filled_by_write_header_emplace)
{
	NErrorIf(!pfile || !pfile_header_filled_by_write_header_emplace, NERROR_NULL_POINTER);
	NErrorIf(ISFLAG_OFF(pfile->Flags, FLAG_NFILE_WRITE), NERROR_FILE_WRITE_ERROR);

	if (ISFLAG_ON(pfile->Flags, FLAG_NFILE_MEMORY))
	{
		// TODO
		// pfile->RWPos += writtenelement * elementsize;
	}
	else
	{
		NErrorIf(ferror(pfile->pFile), NERROR_FILE_ALREADY_DETECTED_ERROR); // There is an already detected Error with the file.

		Ns32 offset = ftell(pfile->pFile);
		if (offset == -1)
		{
			NErrorIf(1, NERROR_FILE_FTELL);
			return -1;
		}
		else
		{
			Nu32 size = _SafeNs32ToNu32(offset) - pfile_header_filled_by_write_header_emplace->Size;

			if (fseek(pfile->pFile, pfile_header_filled_by_write_header_emplace->Size, SEEK_SET))
			{
				NErrorIf(1, NERROR_FILE_FSEEK);
				return -1;
			}

			if (fwrite(&size, sizeof(Nu32), 1, pfile->pFile) != 1)
			{
				NErrorIf(1, NERROR_FILE_FWRITE);
				return -1;
			}

			if (fseek(pfile->pFile, offset, SEEK_SET))
			{
				NErrorIf(1, NERROR_FILE_FSEEK);
				return -1;
			}
		}
	}

	return 0;
}

// -------------------------------------------------------------------------------------------
// NVersionTagCmp
// -------------------------------------------------------------------------------------------
// Description :
//	Compare 2 Version Tags (Nu32 numbers)
// -------------------------------------------------------------------------------------------
// In  :
//		a	: Version a.
//		b	: Version b.
//
// Out :
//		 > 0	: (a) version is greater than (b) version. It means (a) more recent than (b).
//		 0		: Both versions are the same.
//		 < 0	: (a) version is lower than (b) version. It means (a) older than (b).
//
//		Details:
//			Return value can be: 1,2,3,0,-1,-2,-3
//		-3		: (a) version is lower than (b) version (<0). Precision: (a)Major version < (b)Major version
//		-2		: (a) version is lower than (b) version (<0). Precision: Same Major versions but (a)Minor version < (b)Minor version
//		-1		: (a) version is lower than (b) version (<0). Precision:  Same Major versions, same Minor versions but (a)Micro version < (b)Micro version
//		 0		: Both versions are the same.
//		 3		: (a) version is greater than (b) version (>0). Precision: (a)Major version > (b)Major version
//		 2		: (a) version is greater than (b) version (>0). Precision: Same Major versions but (a)Minor version > (b)Minor version
//		 1		: (a) version is greater than (b) version (>0). Precision:  Same Major versions, same Minor versions but (a)Micro version > (b)Micro version
// -------------------------------------------------------------------------------------------
Ns32 NVersionTagCmp(const Nu32 a, const Nu32 b)
{
	Nu8 maja, mina, ra;
	Nu8 majb, minb, rb;

	maja = (Nu8)(a >> 16);
	mina = (Nu8)(a >> 8);
	ra = (Nu8)(a);

	majb = (Nu8)(b >> 16);
	minb = (Nu8)(b >> 8);
	rb = (Nu8)(b);

	// ----------------------
	// Compare major versions
	// ----------------------
	if (maja > majb)
		return 3;

	if (maja < majb)
		return -3;

	// ----------------------
	// Compare minor versions
	// ----------------------
	if (mina > minb)
		return 2;
	if (mina < minb)
		return -2;

	// ----------------------
	// Compare micro versions
	// ----------------------
	if (ra > rb)
		return 1;
	if (ra < rb)
		return -1;

	return 0;
}

/*
Nbool NIsRootRelativePathFileName(const Nchar *pfilename)
{
	NErrorIf(pfilename[1] == ':',NERROR_UNAUTHORIZED_DRIVE_ROOT_FILENAME);// filename is something like "c:..." or "d:..." and it's totally unauthorized

	#ifdef _NWINDOWS
	if(pfilename[1] == ':')
		return NFALSE;

	if(strstr(pfilename,"data/")==pfilename)
	{
		return NFALSE;
	}
	else
	{
		return NTRUE;
	}
	#endif

	#ifdef _NIOS
	if(strstr(pfilename,DatasPathFolder)==pfilename)
	{
		return NFALSE;
	}
	else
	{
		return NTRUE;
	}
	#endif
}
*/