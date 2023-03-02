// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NAudio.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NErrorHandling.h"
#include "lib/N/File/NFile.h"
#include "lib/N/Core/NSafeConversion.h"
#include "lib/N/NString.h"
#include "lib/N/NCore.h"
#include "lib/N/Audio/NAudio.h"

void NSaveWavFile(NWAVE *pwave, const Nchar *pfilename)
{
	NErrorIf(!pwave || !pfilename, NERROR_NULL_POINTER);

	NFILE *pfile;
	NHEADER_WAV_CHUNK_RIFF riffheader;
	NHEADER_WAV_CHUNK_FMT fmtheader;
	NHEADER_WAV_CHUNK_DATA dataheader;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Open file
	// +
	pfile = NFileOpen(pfilename, FLAG_NFILE_WRITE | FLAG_NFILE_BINARY);
	if (!pfile)
	{
		NErrorIf(1, NERROR_FILE_OPENING_ERROR);
		return;
	}

	Nmem0(&riffheader, NHEADER_WAV_CHUNK_RIFF);
	Nmem0(&fmtheader, NHEADER_WAV_CHUNK_FMT);
	Nmem0(&dataheader, NHEADER_WAV_CHUNK_DATA);

	dataheader.SubChunk2ID = WAV_CHUNK_ID_DATA;
	dataheader.SubChunk2Size = NGetWaveDataSize(pwave);

	fmtheader.SubChunk1ID = WAV_CHUNK_ID_FMT;
	fmtheader.SubChunk1Size = WAV_CHUNK_SIZE_PCM;
	fmtheader.AudioFormat = WAV_PCM_FORMAT;
	fmtheader.NumChannels = NGetWaveChannelNb(pwave);
	fmtheader.SampleRate = NGetWaveSampleRate(pwave);
	fmtheader.ByteRate = NGetWaveByteRate(pwave);
	fmtheader.BlockAlign = NGetWaveBlockAlign(pwave);
	fmtheader.BitsPerSample = NGetWaveBitsPerSample(pwave);

	riffheader.ChunkID = WAV_CHUNK_ID_RIFF;
	riffheader.ChunkSize = WAV_CHUNK_SIZE_RIFF_FIX_PART + dataheader.SubChunk2Size;
	riffheader.Format = WAV_FORMAT;

	NFileWrite(pfile, &riffheader, sizeof(NHEADER_WAV_CHUNK_RIFF), 1);
	NFileWrite(pfile, &fmtheader, sizeof(NHEADER_WAV_CHUNK_FMT), 1);
	NFileWrite(pfile, &dataheader, sizeof(NHEADER_WAV_CHUNK_DATA), 1);
	NFileWrite(pfile, pwave->pData, pwave->WaveSize, 1);

	NFileClose(pfile);
}
NWAVE *NCreateWaveFromWavFile(const Nchar *pfilename)
{
	/*
		bool loadWavFile(const std::string filename, ALuint* buffer,
			ALsizei* size, ALsizei* frequency,
			ALenum* format) {
	*/
	// Local Declarations
	/*
				FILE* soundFile = NULL;
				WAVE_Format wave_format;
				RIFF_Header riff_header;
				WAVE_Data wave_data;
				unsigned char* data;
	*/

	NFILE *pfile;
	NHEADER_WAV_CHUNK_RIFF riffheader;
	NHEADER_WAV_CHUNK_FMT fmtheader;
	NHEADER_WAV_CHUNK_DATA dataheader;
	Nu16 extradatasize;
	NWAVE *pwave;
	NWAVEDESC wdesc;
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + First basic check (debug only)
	// +
	NErrorIf(!pfilename, NERROR_NULL_POINTER);
	NErrorIf(!NCheckFileType(pfilename, EXTENSION_WAV), NERROR_FILE_INVALID_EXTENSION);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Open file an read WAV Header
	// +
	pfile = NFileOpen(pfilename, FLAG_NFILE_READ | FLAG_NFILE_BINARY);
	if (!pfile)
	{
		NErrorIf(1, NERROR_NULL_POINTER);
		return NULL;
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Read and check in the first chunk (RIFF)
	// +
	memset(&riffheader, 0, sizeof(NHEADER_WAV_CHUNK_RIFF));
	NFileRead(pfile, &riffheader, sizeof(NHEADER_WAV_CHUNK_RIFF), 1);

	// +-+ Check for RIFF tag
	if (riffheader.ChunkID != WAV_CHUNK_ID_RIFF)
	{
		NErrorIf(1, NERROR_AUDIO_FILE_IS_NOT_A_RIFF_FILE);
		NFileClose(pfile);
		return NULL;
	}
	// +-+ Check for WAVE format
	if (riffheader.Format != WAV_FORMAT)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_FILE_IS_NOT_A_WAVE);
		NFileClose(pfile);
		return NULL;
	}

	// +-+ note: we are not going to use 'riffheader.ChunkSize' yet.

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Read and check in the second chunk (Wave Format Infos)
	// +
	memset(&fmtheader, 0, sizeof(NHEADER_WAV_CHUNK_FMT));
	NFileRead(pfile, &fmtheader, sizeof(NHEADER_WAV_CHUNK_FMT), 1);

	// +-+ Check for fmt tag
	if (fmtheader.SubChunk1ID != WAV_CHUNK_ID_FMT)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_FORMAT);
		NFileClose(pfile);
		return NULL;
	}

	// +-+ Check for chunk size
	if (fmtheader.SubChunk1Size < WAV_CHUNK_SIZE_PCM)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_WAVE_FILE_CORRUPTED);
		NFileClose(pfile);
		return NULL;
	}

	// +-+ Check for PCM
	if (fmtheader.AudioFormat != WAV_PCM_FORMAT)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_WAVE_FILE_IS_NOT_PCM);
		NFileClose(pfile);
		return NULL;
	}

	// +-+ Check for extra parameters ... (some application add extra data, we are going to avoid them )
	if (fmtheader.SubChunk1Size > WAV_CHUNK_SIZE_PCM)
	{
		NFileRead(pfile, &extradatasize, sizeof(Nu16), 1);

		// note:we just pass the extra data... but we assume they are just here ( as describe into documentation found on internet)
		//		... maybe it's not the case, maybe those data are stored "AFTER" wave data ... so, let's check precisely those cases when they will
		//		appear.
		// NErrorIf(1,NERROR_SYSTEM_TODO);

		NFileSeek(pfile, extradatasize, NFILE_SEEK_CUR);
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Read and check in the second chunk (Wave Format Infos)
	// +
	memset(&dataheader, 0, sizeof(NHEADER_WAV_CHUNK_DATA));
	NFileRead(pfile, &dataheader, sizeof(NHEADER_WAV_CHUNK_DATA), 1);

	// +-+ Check for data tag
	if (dataheader.SubChunk2ID != WAV_CHUNK_ID_DATA)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_DATA_ID);
		NFileClose(pfile);
		return NULL;
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Loading Data and WAVE creation
	// +
	if (dataheader.SubChunk2Size == 0)
	{
		NErrorIf(1, NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_DATA_SIZE);
		NFileClose(pfile);
		return NULL;
	}

	wdesc.Flags = 0;
	wdesc.Bps = _SafeNu32ToNu8((Nu32)fmtheader.BitsPerSample);
	wdesc.Channel = _SafeNu32ToNu8((Nu32)fmtheader.NumChannels);
	wdesc.SampleRate = fmtheader.SampleRate;
	wdesc.WaveSize = dataheader.SubChunk2Size;
	pwave = NCreateWave(&wdesc);

	NFileRead(pfile, pwave->pData, dataheader.SubChunk2Size, 1);

	NFileClose(pfile);

	/*
					soundFile = fopen(filename.c_str(), "rb");
					if (!soundFile)
						throw (filename);

					// Read in the first chunk into the struct
					fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);


					//check for RIFF and WAVE tag in memeory
					if ((riff_header.chunkID[0] != 'R' ||
						riff_header.chunkID[1] != 'I' ||
						riff_header.chunkID[2] != 'F' ||
						riff_header.chunkID[3] != 'F') ||
						(riff_header.format[0] != 'W' ||
						riff_header.format[1] != 'A' ||
						riff_header.format[2] != 'V' ||
						riff_header.format[3] != 'E'))
						throw ("Invalid RIFF or WAVE Header");

					//Read in the 2nd chunk for the wave info
					fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
					//check for fmt tag in memory
					if (wave_format.subChunkID[0] != 'f' ||
						wave_format.subChunkID[1] != 'm' ||
						wave_format.subChunkID[2] != 't' ||
						wave_format.subChunkID[3] != ' ')
						throw ("Invalid Wave Format");

					//check for extra parameters;
					if (wave_format.subChunkSize > 16)
						fseek(soundFile, sizeof(short), SEEK_CUR);

					//Read in the the last byte of data before the sound file
					fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
					//check for data tag in memory
					if (wave_data.subChunkID[0] != 'd' ||
						wave_data.subChunkID[1] != 'a' ||
						wave_data.subChunkID[2] != 't' ||
						wave_data.subChunkID[3] != 'a')
						throw ("Invalid data header");

					//Allocate memory for data
					data = new unsigned char[wave_data.subChunk2Size];

					// Read in the sound data into the soundData variable
					if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
						throw ("error loading WAVE data into struct!");

					//Now we set the variables that we passed in with the
					//data from the structs
					*size = wave_data.subChunk2Size;
					*frequency = wave_format.sampleRate;
					//The format is worked out by looking at the number of
					//channels and the bits per sample.
					if (wave_format.numChannels == 1) {
						if (wave_format.bitsPerSample == 8 )
							*format = AL_FORMAT_MONO8;
						else if (wave_format.bitsPerSample == 16)
							*format = AL_FORMAT_MONO16;
					} else if (wave_format.numChannels == 2) {
						if (wave_format.bitsPerSample == 8 )
							*format = AL_FORMAT_STEREO8;
						else if (wave_format.bitsPerSample == 16)
							*format = AL_FORMAT_STEREO16;
					}
					//create our openAL buffer and check for success
					alGenBuffers(1, buffer);
					errorCheck();
					//now we put our data into the openAL buffer and
					//check for success
					alBufferData(*buffer, *format, (void*)data,
						*size, *frequency);
					errorCheck();
					//clean up and return true if successful
					fclose(soundFile);
					return true;
	*/
	/*
		}
	*/

	return pwave;
}
