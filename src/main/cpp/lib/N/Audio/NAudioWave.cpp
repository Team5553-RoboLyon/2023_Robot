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



NWAVE *NSetupWave(NWAVE *pwave, const NWAVEDESC *pwavedesc)
{
	NErrorIf(pwavedesc->Channel != 1 && pwavedesc->Channel != 2, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pwavedesc->Bps != 8 && pwavedesc->Bps != 16, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pwavedesc->SampleRate == 0, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(pwavedesc->WaveSize < (Nu32)(pwavedesc->Channel*(pwavedesc->Bps/8)), NERROR_INCONSISTENT_PARAMETERS);

	pwave->Flags		= 0; // This member is unused Yet
	pwave->Channel		= pwavedesc->Channel;
	pwave->Bps			= pwavedesc->Bps;
	pwave->SampleRate	= pwavedesc->SampleRate;
	pwave->WaveSize		= pwavedesc->WaveSize;
	pwave->pData		= (Nu8*)Nmalloc(sizeof(Nu8)*pwave->WaveSize);
	memset(pwave->pData,0,sizeof(Nu8)*pwave->WaveSize);
	return pwave;
}

NWAVE *NCreateWave(const NWAVEDESC *pwavedesc)
{
	return NSetupWave(NEW(NWAVE),pwavedesc);
}

void NClearWave(NWAVE *pwave)
{
	Nfree(pwave->pData);
	Nmem0(pwave,NWAVE);
}

void NDeleteWave(NWAVE *pwave)
{
	NClearWave(pwave);
	Nfree(pwave);
	pwave = NULL;
}

Nbool 	NGetWaveALInfo( NWAVEALINFO *pwave_al_info, const NWAVE *pwave  )
{
	NErrorIf(!pwave_al_info || !pwave, NERROR_NULL_POINTER);
	
	pwave_al_info->Size			= (ALsizei)pwave->WaveSize;
	pwave_al_info->Frequency	= (ALsizei)pwave->SampleRate;
	// AL FORMAT
	switch(pwave->Channel)
	{
		case 1:
			switch(pwave->Bps)
			{
				case 8:
					pwave_al_info->Format = AL_FORMAT_MONO8;
					break;
				case 16:
					pwave_al_info->Format = AL_FORMAT_MONO16;
					break;
				default:
					NErrorIf(1,NERROR_AUDIO_WAVE_UNEXPECTED_BPS);
					return 0;
			}
			break;

		case 2:
			switch(pwave->Bps)
			{
				case 8:
					pwave_al_info->Format = AL_FORMAT_STEREO8;
					break;
				case 16:
					pwave_al_info->Format = AL_FORMAT_STEREO16;
					break;
				default:
					NErrorIf(1,NERROR_AUDIO_WAVE_UNEXPECTED_BPS);
					return 0;
			}
			break;

		default:
			NErrorIf(1,NERROR_AUDIO_WAVE_UNEXPECTED_CHANNEL_NUMBER);
			return NFALSE;
	}
	return NTRUE;
}
