// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NAudioExtended.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCore.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Audio/NAudio.h"


Nf32 ListenerGain = 1.0f; // By default

void	NSetAudioListenerGain(const Nf32 gain)
{
	ListenerGain = gain;
	if( ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_AUDIO_MUTED) )
	{
		alListenerf(AL_GAIN, 0.0f);NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	else
	{
		alListenerf(AL_GAIN, gain);NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
}
void 	NMuteAudio(const Nbool mute)
{
	if( mute )
	{
		FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_AUDIO_MUTED);
		alListenerf(AL_GAIN, 0.0f);NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
	else
	{
		FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_AUDIO_MUTED);
		alListenerf(AL_GAIN, ListenerGain);NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
	}
}

// ------------------------------------------------------------------------------------------
// void NPlayAudioSourceEx
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NPlayAudioSourceEx(NAUDIOSOURCE *pas,const NAUDIOBUFFER *pwbuffer)
{
	if(pwbuffer)
	{
		ALint al_state;
		alGetSourcei(pas->AL_Source,AL_SOURCE_STATE,&al_state);
		switch(al_state)
		{
			case AL_INITIAL:
			case AL_STOPPED:
				alSourcei(pas->AL_Source, AL_BUFFER, pwbuffer->AL_Buffer);
				alSourcePlay(pas->AL_Source);
				NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
				pas->Priority = pwbuffer->Priority;
				break;

			case AL_PLAYING:
			case AL_PAUSED:
				// Incoming priority is higher than current (0 for higher priority, 255 to lowest)
				// in case of priority conflict ( incoming buffer priority == current buffer priority ) check the FLAG_NAUDIOSOURCE_PRIORITY_CONFLICT_KEEP_CURRENT.
				if( pwbuffer->Priority < pas->Priority || 
					( ( pwbuffer->Priority == pas->Priority ) && ISFLAG_OFF(pas->Flags,FLAG_NAUDIOSOURCE_PRIORITY_CONFLICT_KEEP_CURRENT) ) )
				{
					alSourceStop(pas->AL_Source);
					alSourcei(pas->AL_Source, AL_BUFFER, pwbuffer->AL_Buffer);
					alSourcePlay(pas->AL_Source);
					NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
					pas->Priority = pwbuffer->Priority;
				}
				break;
		}
	}
	else
	{
		alSourceStop(pas->AL_Source);
		alSourcei(pas->AL_Source, AL_BUFFER, 0);
		NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
		pas->Priority = 0;
	}
}
