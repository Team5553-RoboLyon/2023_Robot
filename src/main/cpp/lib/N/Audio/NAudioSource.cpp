// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NAudio.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NErrorHandling.h"
#include "../File/NFile.h"
#include "../Core/NSafeConversion.h"
#include "../NString.h"
#include "../NCore.h"
#include "NAudio.h"

NAUDIOSOURCE*	NSetupAudioSource( NAUDIOSOURCE *pasource, const Nchar *paudiosource_name )
{
	NErrorIf(!pasource, NERROR_NULL_POINTER);


	Nmem0(pasource,NAUDIOSOURCE);
	alGenSources(1,&pasource->AL_Source);
	if(alGetError() == AL_NO_ERROR)
	{
		pasource->pName = NStrDuplicate(paudiosource_name);
	}
#ifdef _DEBUG
	else
	{
		if(alGetError() == AL_INVALID_OPERATION)
		{
			// There is no context to create sources in..
			NErrorIf(1,NERROR_AUDIOSOURCE_NO_CONTEXT);		
		}
		else if(alGetError() == AL_OUT_OF_MEMORY)			
		{
			// There is not enough memory to generate all the requested sources !
			NErrorIf(1,NERROR_AUDIOSOURCE_OUT_OF_MEMORY);	
		}
		else if(alGetError() == AL_INVALID_VALUE)
		{
			// There are not enough non-memory resources to create all the requested sources, 
			// or the array pointer (pasource->AL_Source)is not valid (which is not really possible actually).
			NErrorIf(1,NERROR_AUDIOSOURCE_OUT_OF_RESOURCES);
		}
		else
		{
			NErrorIf(1,NERROR_AUDIOSOURCE_UNEXPECTED_CASE);
		}
	}
#endif
	return pasource;
}

NAUDIOSOURCE*	NCreateAudioSource(const Nchar *paudiosource_name )
{
	return NSetupAudioSource(NEW(NAUDIOSOURCE),paudiosource_name);
}

void NClearAudioSource(NAUDIOSOURCE *pas)
{
	NErrorIf(!pas,NERROR_NULL_POINTER);

	alDeleteSources(1,&pas->AL_Source);
	if(alGetError() == AL_NO_ERROR)
	{
		Nmem0(pas,NAUDIOSOURCE);
	}
#ifdef _DEBUG
	else
	{
		if(alGetError() == AL_INVALID_OPERATION)
		{
			// There is no current context.
			NErrorIf(1,NERROR_AUDIOSOURCE_NO_CONTEXT);
		}
		else if(alGetError() == AL_INVALID_NAME)
		{
			// At least one specified source is not valid,
			// or an attempt is being made to delete more sources than exist.
			NErrorIf(1,NERROR_AUDIOSOURCE_INVALID_NAME);
		}
		else
		{
			NErrorIf(1,NERROR_AUDIOSOURCE_UNEXPECTED_CASE);
		}
	}
#endif
}

void NDeleteAudioSource(NAUDIOSOURCE *pas)
{
	NClearAudioSource(pas);
	Nfree(pas);
}

void NBindAudioBufferToSource(NAUDIOSOURCE *pas, const NAUDIOBUFFER *pab)
{
	if(pab)
	{
		pas->Priority = pab->Priority;
		alSourcei(pas->AL_Source, AL_BUFFER, pab->AL_Buffer);
	}
	else
	{
		alSourcei(pas->AL_Source, AL_BUFFER, 0);
		pas->Priority = 0;
	}
	NErrorIf(alGetError() != AL_NO_ERROR, NERROR_AUDIO);
}
