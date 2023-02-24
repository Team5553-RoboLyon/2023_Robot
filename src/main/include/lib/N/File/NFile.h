#ifndef __NFILE_H
#define __NFILE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NFile.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "../Containers/NArray.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*
	* MOVE IN N/Core/NVersion.h"
	// ---------------------------------------------------------------------------------------------------------------------
	// VERSIONNING RULES
	// Given a version number MAJOR.MINOR.MICRO, increment the:
	//	.MAJOR version when you make incompatible API changes,
	//	.MINOR version when you add functionality in a backwards-compatible manner, and
	//	.MICRO version when you make backwards-compatible bug fixes.
	// ( Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.MICRO format.)
	// ---------------------------------------------------------------------------------------------------------------------
	#define NMAKEVERSION(_maj,_min,_mic)	( (Nu32)(((Nu32)_maj)<<16 | ((Nu32)_min)<<8 | ((Nu32)_mic)) )
	#define	NGETVERSION_MAJOR(a)	( (((Nu32)a) >> 16) & 0xff )
	#define	NGETVERSION_MINOR(a)	( (((Nu32)a) >> 8) & 0xff )
	#define	NGETVERSION_MICRO(a)	( ((Nu32)a) & 0xff )
	#define	NGETVERSION_MAIN(a)		( (((Nu32)a) >> 8) & 0xffff )						// 'Main version' contains 'Major' and 'Minor' without 'Micro'

	#define NMAKESIGNATURE(a,b,c,d) ( (((Nu32)(d)) << 24) | (((Nu32)(c)) << 16) | (((Nu32)(b)) << 8) | ((Nu32)a) )
	#define NSIGNATURECMP(a,b)		(((a) == (b)) ? 1 : 0)
	*/

#define CONSTANT_NFILE_FULLDATAPATH_MAXSIZE 256 // Used by the function 'NFileOpen' to manage relative or absolute file pathnames
												// computed by N with the name of the file and the pathname of the data file ( different from a machine to an another one )
#define NFILENAME_DEFAULTSIZE 64				// Advice: When N has to create a filename to load or save a file it has to reserve
												// a string with enough characters. "NFILENAME_DEFAULTSIZE" is this number of characters.
												// this is not the Fulldatapath size. this is just the path size 'behind' the data file path !

	typedef enum
	{
		NFILE_SEEK_SET = 0,
		NFILE_SEEK_CUR,
		NFILE_SEEK_END
	} NFILE_SEEKORIGIN;

// -----------------------------
// BIT_0 to BIT_2 included need to set as they are because they are linked with NEngineCore default File system flags !!!
#define FLAG_NFILE_APP_ROOT_RELATIVE_PATH BIT_0 //|If Booth are OFF, using NENGINE statement.
#define FLAG_NFILE_FULL_PATH BIT_1				//|!NEED TO STAY BIT_0 and BIT_1 Like FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH and FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH

#define FLAG_NFILE_READ BIT_2
#define FLAG_NFILE_WRITE BIT_3
#define FLAG_NFILE_UPDATE BIT_4 // in combination with READ or WRITE ( equivalent to r+/rb+ or w+/wb+ )

#define FLAG_NFILE_MEMORY BIT_5
#define FLAG_NFILE_BINARY BIT_6 // Default is "Text"

// -----------------------------
// BIT_0 to BIT_1 included need to set as they are because they are linked with NEngineCore default File system flags !!!
#define MASK_NFILE_PATH_FILESYSTEM (FLAG_NFILE_APP_ROOT_RELATIVE_PATH | FLAG_NFILE_FULL_PATH)

#define CONSTANT_NROOT_RELATIVEPATH_COUNT 2
#define NROOTRELATIVEPATH_APPDATA 0
#define NROOTRELATIVEPATH_USERDATA 1

	typedef struct
	{
		FILE *pFile; // Disc file
					 // 	NBYTE	*pMemFile;	// Memory file
					 // 	Nu32	RWPos;		// Memory file only ... Read/Write position in Memory file

		Nu32 Flags; // ...
	} NFILE;
	// New implementation (2017/08/14)
	typedef struct
	{
		Nu32 Signature; // Unique Signature Tag to clearly identify the following data
		Nu32 Version;	// Version Tag of the following data
		Nu32 Size;		// Size (in Byte) of the following data that this header is related with
	} NFILE_HEADER;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************

	void NFileClearError(const NFILE *pfile);
	Ns32 NFileClose(NFILE *pfile);
	Ns32 NFileError(const NFILE *pfile);
	Ns32 NFileEOF(const NFILE *pfile);
	Ns32 NFileGetSize(NFILE *pfile);
	Nchar *NFileGetString(Nchar *pstring, Nu32 size_null_end_included, NFILE *pfile);
	NFILE *NFileOpen(const Nchar *filename, const Nu32 flags);
	Nu32 NFileRead(NFILE *pfile, void *ptr, const Nu32 element_size_in_byte, const Nu32 element_count);
	void NFileRewind(NFILE *pfile);
	Ns32 NFileTell(NFILE *pfile);
	Ns32 NFileSeek(NFILE *pfile, const Nu32 offset, const NFILE_SEEKORIGIN from);
	Nu32 NFileWrite(NFILE *pfile, const void *ptr, const Nu32 element_size_in_byte, const Nu32 element_count);

	void NPrintErrorMessage(const Nchar *str);

	// Advanced File functions
	Nbool NCheckFileType(const Nchar *filename, const Nchar *ext);

	Nu32 NFileWriteVersionTag(NFILE *pfile, const Nu32 version);
	Nu32 NFileReadVersionTag(NFILE *pfile, Nu32 *pversion);

	Nu32 NFileWriteSignatureTag(NFILE *pfile, const Nu32 sign);
	Nu32 NFileReadSignatureTag(NFILE *pfile, Nu32 *psign);

	Ns32 NFileWriteHeader_Emplace(NFILE *pfile, NFILE_HEADER *pfile_header, const Nu32 sign, const Nu32 version);
	Ns32 NFileWriteHeader_CompleteBlockSize(NFILE *pfile, const NFILE_HEADER *pfile_header_filled_by_write_header_emplace);
	Nu32 NFileReadHeader(NFILE *pfile, NFILE_HEADER *pheader);
	inline Nu32 NFileGetHeaderBlockSize(const NFILE_HEADER *pheader) { return pheader->Size; }

	Ns32 NVersionTagCmp(const Nu32 a, const Nu32 b);

	// Nbool			NIsRootRelativePathFileName(const Nchar *pfilename);

	void NSetupRootRelativePath(const Nu32 id, const Nchar *prootpath);
	void NClearRootRelativePath(const Nu32 id);
	void NClearAllRootRelativePath();
	Nu32 NSetCurrentRootRelativePath(const Nu32 id);

	Nchar *NGetFullFileName(Nchar *pbuildfullfilename, const Nchar *pfilename, const Nu32 flags_filesystem_path);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NFILE_H
