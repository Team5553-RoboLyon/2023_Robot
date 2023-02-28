#ifndef __NAUDIO_H
#define __NAUDIO_H

#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Core/NSafeConversion.h"
#include "lib/N/AL/Nal.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define EXTENSION_WAV ".wav"
#define WAV_FORMAT 0x45564157		 // "WAVE" in ASCII form
#define WAV_CHUNK_ID_RIFF 0x46464952 // "RIFF" in ASCII form
#define WAV_CHUNK_ID_FMT 0x20746d66	 // "fmt " in ASCII form
#define WAV_CHUNK_ID_DATA 0x61746164 // "data" in ASCII form
#define WAV_CHUNK_SIZE_PCM 16
#define WAV_CHUNK_SIZE_RIFF_FIX_PART 36 //
#define WAV_PCM_FORMAT 1				// (linear quantization). Values other than 1 indicate some form of compression.

	typedef struct
	{
		Nu8 BankID;		   // 0 to 255. The bank which the WaveBuffer belongs to.
		Nu8 Priority;	   // 0 to 255. (0 for higher priority, 255 to lowest)
		Nu8 Flags;		   // 8 flags
		Nu8 Available_Nu8; // Available for future use

		Nchar *pName; // Should be unique, and should be filename without the "datapath" which is different by machine
		ALuint AL_Buffer;
	} NAUDIOBUFFER;

#define FLAG_NAUDIOSOURCE_PRIORITY_CONFLICT_KEEP_CURRENT BIT_0 // In case of priority conflict between current and incoming NAUDIOBUFFER with the "NPlayAudioSourceEx" function, N choose to keep the current if this FLAG is set to ON or the new one by default.
	typedef struct
	{
		Nu8 BankID;		   // 0 to 255. The bank which the Source belongs to.
		Nu8 Flags;		   // 8 flags
		Nu8 Priority;	   // 0 to 255. Priority of current bound Audio Buffer. (0 for higher priority, 255 to lowest)
		Nu8 Available_Nu8; // Available for future use

		Nchar *pName; // Should be unique
		ALuint AL_Source;
	} NAUDIOSOURCE;

	typedef struct
	{
		Nu16 Flags;		 // ?? Unused yet
		Nu8 Channel;	 // Mono or Stereo (1 or 2)
		Nu8 Bps;		 // Bits per Sample (8 or 16)
		Nu32 SampleRate; // Hz Samples per Second. 22000,44100,48000
		Nu32 WaveSize;
		Nu8 *pData;
	} NWAVE;

	typedef struct
	{
		Nu16 Flags;		 // ?? Unused yet
		Nu8 Channel;	 // Mono or Stereo (1 or 2)
		Nu8 Bps;		 // Bits per Sample (8 or 16)
		Nu32 SampleRate; // Hz Samples per Second. 22000,44100,48000
		Nu32 WaveSize;

	} NWAVEDESC;

	typedef struct
	{
		ALenum Format;
		ALsizei Size;
		ALsizei Frequency;
	} NWAVEALINFO;

	typedef struct
	{
		Nu32 ChunkID;	// "RIFF"
		Nu32 ChunkSize; //	Size not including ChunkSize and ChunkID
		Nu32 Format;	// "WAVE"
	} NHEADER_WAV_CHUNK_RIFF;

	typedef struct
	{
		Nu32 SubChunk1ID; // "fmt "
		Nu32 SubChunk1Size;
		Nu16 AudioFormat;
		Nu16 NumChannels;
		Nu32 SampleRate;
		Nu32 ByteRate;
		Nu16 BlockAlign;
		Nu16 BitsPerSample;
	} NHEADER_WAV_CHUNK_FMT;

	typedef struct
	{
		Nu32 SubChunk2ID;	// should contain the word "data"
		Nu32 SubChunk2Size; // Stores the size of the data block
	} NHEADER_WAV_CHUNK_DATA;

	inline Nu8 NGetWaveChannelNb(const NWAVE *pwave) { return pwave->Channel; }
	inline Nu32 NGetWaveSampleRate(const NWAVE *pwave) { return pwave->SampleRate; }
	inline Nu8 NGetWaveBitsPerSample(const NWAVE *pwave) { return pwave->Bps; }
	inline Nu32 NGetWaveDataSize(const NWAVE *pwave) { return pwave->WaveSize; }
	inline Nu32 NGetWaveByteRate(const NWAVE *pwave) { return (Nu32)(pwave->SampleRate * pwave->Channel * pwave->Bps / 8); }
	inline Nu16 NGetWaveBlockAlign(const NWAVE *pwave) { return _SafeNu32ToNu16(pwave->Channel * pwave->Bps / 8); }

	void NAudio_Init();
	void NAudio_Quit();
	void NMuteAudio(const Nbool mute);

	NWAVE *NSetupWave(NWAVE *pwave, const NWAVEDESC *pwavedesc);
	NWAVE *NCreateWave(const NWAVEDESC *pwavedesc);
	void NClearWave(NWAVE *pwave);
	void NDeleteWave(NWAVE *pwave);
	Nbool NGetWaveALInfo(NWAVEALINFO *pwave_al_info, const NWAVE *pwave);

	NWAVE *NCreateWaveFromWavFile(const Nchar *pfilename);
	void NSaveWavFile(NWAVE *pwave, const Nchar *pfilename);

	// AUDIO LISTENER
	void NSetAudioListenerGain(const Nf32 gain);
	inline void NSetAudioListenerPosition(const NVEC3 *ppos)
	{
		alListenerfv(AL_POSITION, (ALfloat *)ppos);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NSetAudioListenerPosition3f(const Nf32 x, const Nf32 y, const Nf32 z)
	{
		alListener3f(AL_POSITION, x, y, z);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NSetAudioListenerVelocity(const NVEC3 *pv)
	{
		alListenerfv(AL_VELOCITY, (ALfloat *)pv);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NSetAudioListenerVelocity3f(const Nf32 vx, const Nf32 vy, const Nf32 vz)
	{
		alListener3f(AL_VELOCITY, vx, vy, vz);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NSetAudioListenerOrientation(const NVEC3 *at, const NVEC3 *up)
	{
		NVEC3 v[2];
		v[0] = *at;
		v[1] = *up;
		alListenerfv(AL_ORIENTATION, (ALfloat *)v);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}

	// AUDIO BUFFER
	NAUDIOBUFFER *NSetupAudioBuffer(NAUDIOBUFFER *pwbuffer, const Nchar *paudiobuffer_name, const NWAVE *pwave, const Nu8 bankid, const Nu8 priority);
	NAUDIOBUFFER *NCreateAudioBuffer(const Nchar *paudiobuffer_name, const NWAVE *pwave, const Nu8 bankid, const Nu8 priority);
	void NClearAudioBuffer(NAUDIOBUFFER *pabuf);
	void NDeleteAudioBuffer(NAUDIOBUFFER *pabuf);
	NAUDIOBUFFER *NCreateAudioBufferFromWavFile(const Nchar *paudiobuffer_name, const Nchar *pwav_filename, const Nu8 bankid, const Nu8 priority);

	inline void NSetAudioBufferPriority(NAUDIOBUFFER *pwbuffer, const Nu8 priority) { pwbuffer->Priority = priority; }
	inline Nu8 NGetAudioBufferPriority(NAUDIOBUFFER *pwbuffer) { return pwbuffer->Priority; }

	// AUDIO SOURCE
	NAUDIOSOURCE *NSetupAudioSource(NAUDIOSOURCE *pasource, const Nchar *paudiosource_name);
	NAUDIOSOURCE *NCreateAudioSource(const Nchar *paudiosource_name);
	void NClearAudioSource(NAUDIOSOURCE *pas);
	void NDeleteAudioSource(NAUDIOSOURCE *pas);

	void NBindAudioBufferToSource(NAUDIOSOURCE *pas, const NAUDIOBUFFER *pab);

	inline void NPlayAudioSource(NAUDIOSOURCE *pas)
	{
		alSourcePlay(pas->AL_Source);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	void NPlayAudioSourceEx(NAUDIOSOURCE *pas, const NAUDIOBUFFER *pwbuffer);
	inline void NPauseAudioSource(NAUDIOSOURCE *pas)
	{
		alSourcePause(pas->AL_Source);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NStopAudioSource(NAUDIOSOURCE *pas)
	{
		alSourceStop(pas->AL_Source);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NRewindAudioSource(NAUDIOSOURCE *pas)
	{
		alSourceRewind(pas->AL_Source);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	/*
	inline	void	NResumeAudioSource(NAUDIOSOURCE *pas)
	{
		ALint	val;
		alGetSourcei(pas->AL_Source, AL_SOURCE_STATE, &val);
		if(val == AL_PAUSED)
			alSourcei(pas->AL_Source, AL_SOURCE_STATE, AL_PLAYING);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}*/

	inline void NSetAudioSourcePitch(NAUDIOSOURCE *pas, const Nf32 pitch)
	{
		alSourcef(pas->AL_Source, AL_PITCH, pitch);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_PITCH
	inline void NSetAudioSourceGain(NAUDIOSOURCE *pas, const Nf32 gain)
	{
		alSourcef(pas->AL_Source, AL_GAIN, gain);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_GAIN
	inline void NSetAudioSourceMinGain(NAUDIOSOURCE *pas, const Nf32 mingain)
	{
		alSourcef(pas->AL_Source, AL_MIN_GAIN, mingain);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_MIN_GAIN
	inline void NSetAudioSourceMaxGain(NAUDIOSOURCE *pas, const Nf32 maxgain)
	{
		alSourcef(pas->AL_Source, AL_MAX_GAIN, maxgain);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_MAX_GAIN
	inline void NSetAudioSourceMaxDistance(NAUDIOSOURCE *pas, const Nf32 maxdist)
	{
		alSourcef(pas->AL_Source, AL_MAX_DISTANCE, maxdist);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_MAX_DISTANCE
	inline void NSetAudioSourceRollOffFactor(NAUDIOSOURCE *pas, const Nf32 rolloff)
	{
		alSourcef(pas->AL_Source, AL_ROLLOFF_FACTOR, rolloff);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_ROLLOFF_FACTOR
	inline void NSetAudioSourceConeOuterGain(NAUDIOSOURCE *pas, const Nf32 gain)
	{
		alSourcef(pas->AL_Source, AL_CONE_OUTER_GAIN, gain);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_CONE_OUTER_GAIN
	inline void NSetAudioSourceConeOuterAngle(NAUDIOSOURCE *pas, const Nf32 angle)
	{
		alSourcef(pas->AL_Source, AL_CONE_OUTER_ANGLE, angle);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_CONE_INNER_ANGLE
	inline void NSetAudioSourceConeInnerAngle(NAUDIOSOURCE *pas, const Nf32 angle)
	{
		alSourcef(pas->AL_Source, AL_CONE_INNER_ANGLE, angle);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_CONE_OUTER_ANGLE
	inline void NSetAudioSourceReferenceDistance(NAUDIOSOURCE *pas, const Nf32 rdist)
	{
		alSourcef(pas->AL_Source, AL_REFERENCE_DISTANCE, rdist);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_REFERENCE_DISTANCE

	inline void NSetAudioSourcePosition(NAUDIOSOURCE *pas, const NVEC3 *ppos)
	{
		alSourcefv(pas->AL_Source, AL_POSITION, (ALfloat *)ppos);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_POSITION
	inline void NSetAudioSourcePosition3f(NAUDIOSOURCE *pas, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		alSource3f(pas->AL_Source, AL_POSITION, x, y, z);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_POSITION
	inline void NSetAudioSourceVelocity(NAUDIOSOURCE *pas, const NVEC3 *pv)
	{
		alSourcefv(pas->AL_Source, AL_VELOCITY, (ALfloat *)pv);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_VELOCITY
	inline void NSetAudioSourceVelocity3f(NAUDIOSOURCE *pas, const Nf32 vx, const Nf32 vy, const Nf32 vz)
	{
		alSource3f(pas->AL_Source, AL_VELOCITY, vx, vy, vz);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_VELOCITY
	inline void NSetAudioSourceDirection(NAUDIOSOURCE *pas, const NVEC3 *pd)
	{
		alSourcefv(pas->AL_Source, AL_DIRECTION, (ALfloat *)pd);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_DIRECTION
	inline void NSetAudioSourceDirection3f(NAUDIOSOURCE *pas, const Nf32 dx, const Nf32 dy, const Nf32 dz)
	{
		alSource3f(pas->AL_Source, AL_DIRECTION, dx, dy, dz);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	} // AL_DIRECTION

	inline void NEnableAudioSourceRelativePosition(NAUDIOSOURCE *pas)
	{
		alSourcei(pas->AL_Source, AL_SOURCE_RELATIVE, AL_TRUE);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NDisableAudioSourceRelativePosition(NAUDIOSOURCE *pas)
	{
		alSourcei(pas->AL_Source, AL_SOURCE_RELATIVE, AL_FALSE);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NEnableAudioSourceLooping(NAUDIOSOURCE *pas)
	{
		alSourcei(pas->AL_Source, AL_LOOPING, AL_TRUE);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	inline void NDisableAudioSourceLooping(NAUDIOSOURCE *pas)
	{
		alSourcei(pas->AL_Source, AL_LOOPING, AL_FALSE);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}

	// AL_PITCH			NSetAudioSourcePitch(NAUDIOSOURCE *pas, const Nf32 pitch)
	//					pitch multiplier always positive.

	// AL_GAIN			NSetAudioSourceGain(NAUDIOSOURCE *pas, const Nf32 gain)
	//					source gain value should be positive.

	// AL_MAX_DISTANCE	used with the Inverse Clamped Distance Model to set the distance where there will no longer be any attenuation of the source.
	/*
	AL_ROLLOFF_FACTOR
	f, fv, i, iv
	the rolloff rate for the source
	default is 1.0
	AL_REFERENCE_DISTANCE
	f, fv, i, iv
	the distance under which the volume for the source would normally drop by half (before being influenced by rolloff factor or AL_MAX_DISTANCE)
	AL_MIN_GAIN
	f, fv
	the minimum gain for this source
	AL_MAX_GAIN
	f, fv
	the maximum gain for this source
	AL_CONE_OUTER_GAIN
	f, fv
	the gain when outside the oriented cone
	AL_CONE_INNER_ANGLE
	f, fv, i, iv
	the gain when inside the oriented cone
	AL_CONE_OUTER_ANGLE
	f, fv, i, iv
	outer angle of the sound cone, in degrees
	default is 360
	AL_POSITION
	fv, 3f
	X, Y, Z position
	AL_VELOCITY
	fv, 3f
	velocity vector
	AL_DIRECTION
	fv, 3f, iv, 3i
	direction vector
	AL_SOURCE_RELATIVE
	i, iv
	determines if the positions are relative to the listener
	default is AL_FALSE
	AL_SOURCE_TYPE
	i, iv
	the soruce type ? AL_UNDETERMINED, AL_STATIC, or AL_STREAMING
	AL_LOOPING
	i, iv
	turns looping on (AL_TRUE) or off (AL_FALSE)
	AL_BUFFER
	i, iv
	the ID of the attached buffer
	AL_SOURCE_STATE
	i, iv
	the state of the source (AL_STOPPED, AL_PLAYING, ? )
	AL_BUFFERS_QUEUED*
	i, iv
	the number of buffers queued on this source
	AL_BUFFERS_PROCESSED*
	i, iv
	the number of buffers in the queue that have been processed
	AL_SEC_OFFSET
	f, fv, i, iv
	the playback position, expressed in seconds
	AL_SAMPLE_OFFSET
	f, fv, i, iv
	the playback position, expressed in samples
	AL_BYTE_OFFSET
	f, fv, i, iv
	the playback position, expressed in bytes
	*/

	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NAUDIO_H
