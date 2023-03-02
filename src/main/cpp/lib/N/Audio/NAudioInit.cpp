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
extern Nf32 ListenerGain;

void NAudio_Init()
{
	// Device Opening
	ALCdevice  *pdevice = alcOpenDevice(NULL);
	NErrorIf(!pdevice, NERROR_AUDIO_OPEN_DEVICE_FAIL);
	
	// Context creation
	ALCcontext *pcontext = alcCreateContext(pdevice, NULL);
	NErrorIf(!pcontext, NERROR_AUDIO_CONTEXT_CREATION_FAIL);

	// Context Activation
	ALCboolean b = alcMakeContextCurrent(pcontext);
	NErrorIf(!b, NERROR_AUDIO_CONTEXT_FAIL);

#ifdef _NWINDOWS	
	// Openal Extensions Queries
	// XRAM
	if(ALFWIsXRAMSupported() == AL_TRUE)
	{
		FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_AUDIO_XRAM);
#ifdef _DEBUG
		printf("\n Audio XRAM supported");
#endif
	}
#ifdef _DEBUG
	else
	{
		printf("\n Audio XRAM NOT supported");
	}
#endif
	// EFX	
	if(ALFWIsEFXSupported() == AL_TRUE)
	{
		FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_AUDIO_EFX);
#ifdef _DEBUG
		printf("\n Audio EFX supported");
#endif
	}
#ifdef _DEBUG
	else
	{
		printf("\n Audio EFX NOT supported");
	}
#endif
#endif

    // Initial value
    ListenerGain = 1.0f;
	FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_AUDIO_MUTED);
}

void NAudio_Quit()
{
	// Get device and context
	ALCcontext	*pcontext = alcGetCurrentContext();
	ALCdevice	*pdevice  = alcGetContextsDevice(pcontext);

	// Context deactivation and destruction
	alcMakeContextCurrent(NULL);
	alcDestroyContext(pcontext);

	// Device closing
	alcCloseDevice(pdevice);

	// XRAM
	FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_AUDIO_XRAM);

}
