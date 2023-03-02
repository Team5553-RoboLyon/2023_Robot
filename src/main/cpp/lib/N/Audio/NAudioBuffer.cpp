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
extern Nu32 NEngineCoreFlags;

NAUDIOBUFFER *NSetupAudioBuffer(NAUDIOBUFFER *pwbuffer, const Nchar *paudiobuffer_name, const NWAVE *pwave, const Nu8 bankid, const Nu8 priority)
{
	NErrorIf(!pwbuffer || !pwave, NERROR_NULL_POINTER);

	NWAVEALINFO alinfo;

	Nmem0(pwbuffer, NAUDIOBUFFER);
	NGetWaveALInfo(&alinfo, pwave);

	alGenBuffers(1, &pwbuffer->AL_Buffer);
	if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_AUDIO_XRAM))
	{
		// EAXSetBufferMode eaxSetBufferMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
		//  EAXGetBufferMode eaxGetBufferMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");
		eaxSetBufferMode(1, &pwbuffer->AL_Buffer, alGetEnumValue("AL_STORAGE_AUTOMATIC"));
	}

	alBufferData(pwbuffer->AL_Buffer, alinfo.Format, pwave->pData, alinfo.Size, alinfo.Frequency);

	pwbuffer->pName = NStrDuplicate(paudiobuffer_name);
	pwbuffer->BankID = bankid;
	pwbuffer->Priority = priority;
	return pwbuffer;
}

NAUDIOBUFFER *NCreateAudioBuffer(const Nchar *paudiobuffer_name, const NWAVE *pwave, const Nu8 bankid, const Nu8 priority)
{
	return NSetupAudioBuffer(NEW(NAUDIOBUFFER), paudiobuffer_name, pwave, bankid, priority);
}

void NClearAudioBuffer(NAUDIOBUFFER *pabuf)
{
	NErrorIf(!pabuf, NERROR_NULL_POINTER);

	alDeleteBuffers(1, &pabuf->AL_Buffer);
	if (alGetError() == AL_NO_ERROR)
	{
		Nmem0(pabuf, NAUDIOBUFFER);
	}
#ifdef _DEBUG
	else
	{
		if (alGetError() == AL_INVALID_OPERATION)
		{
			NErrorIf(1, NERROR_AUDIOBUFFER_STILL_IN_USE); // Probably attached to one or more sources.
		}
		else if (alGetError() == AL_INVALID_NAME) // 'pabuf->AL_Buffer' is invalid !
		{
			NErrorIf(1, NERROR_AUDIOBUFFER_INVALID_NAME); // ??? strange because here it's '1'
		}
		else if (alGetError() == AL_INVALID_VALUE)
		{
			NErrorIf(1, NERROR_AUDIOBUFFER_INVALID_REQUESTED_BUFFER_NB);
		}
		else
		{
			NErrorIf(1, NERROR_AUDIOBUFFER_UNEXPECTED_CASE);
		}
	}
#endif
}

void NDeleteAudioBuffer(NAUDIOBUFFER *pabuf)
{
	NClearAudioBuffer(pabuf);
	Nfree(pabuf);
}

NAUDIOBUFFER *NCreateAudioBufferFromWavFile(const Nchar *paudiobuffer_name, const Nchar *pwav_filename, const Nu8 bankid, const Nu8 priority)
{
	NAUDIOBUFFER *pabuf = NULL;
	NWAVE *pwave = NCreateWaveFromWavFile(pwav_filename);
	if (pwave)
	{
		pabuf = NCreateAudioBuffer(paudiobuffer_name, pwave, bankid, priority);
		NDeleteWave(pwave);
	}
	return pabuf;
}
