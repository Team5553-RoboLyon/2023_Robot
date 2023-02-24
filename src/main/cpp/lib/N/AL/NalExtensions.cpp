// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NalExtensions.cpp								**
// **																					**
// **		This File is based on "Framework.cpp" created by Creative Labs in 2006.		**
// ** Original File is saved as "_PROJET_C\_LIBS\OpenAL\src\Audio\OpenAL\Framework.cpp"	**
// **																					**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "Nal.h"

#ifdef _NWINDOWS
// Imported EFX functions
// ---------------------------------------------------------------------------------------
// Effect objects
LPALGENEFFECTS		alGenEffects = NULL;
LPALDELETEEFFECTS	alDeleteEffects = NULL;
LPALISEFFECT		alIsEffect = NULL;
LPALEFFECTI			alEffecti = NULL;
LPALEFFECTIV		alEffectiv = NULL;
LPALEFFECTF			alEffectf = NULL;
LPALEFFECTFV		alEffectfv = NULL;
LPALGETEFFECTI		alGetEffecti = NULL;
LPALGETEFFECTIV		alGetEffectiv = NULL;
LPALGETEFFECTF		alGetEffectf = NULL;
LPALGETEFFECTFV		alGetEffectfv = NULL;

//Filter objects
LPALGENFILTERS		alGenFilters = NULL;
LPALDELETEFILTERS	alDeleteFilters = NULL;
LPALISFILTER		alIsFilter = NULL;
LPALFILTERI			alFilteri = NULL;
LPALFILTERIV		alFilteriv = NULL;
LPALFILTERF			alFilterf = NULL;
LPALFILTERFV		alFilterfv = NULL;
LPALGETFILTERI		alGetFilteri = NULL;
LPALGETFILTERIV		alGetFilteriv = NULL;
LPALGETFILTERF		alGetFilterf = NULL;
LPALGETFILTERFV		alGetFilterfv = NULL;

// Auxiliary slot object
LPALGENAUXILIARYEFFECTSLOTS		alGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS	alDeleteAuxiliaryEffectSlots = NULL;
LPALISAUXILIARYEFFECTSLOT		alIsAuxiliaryEffectSlot = NULL;
LPALAUXILIARYEFFECTSLOTI		alAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTIV		alAuxiliaryEffectSlotiv = NULL;
LPALAUXILIARYEFFECTSLOTF		alAuxiliaryEffectSlotf = NULL;
LPALAUXILIARYEFFECTSLOTFV		alAuxiliaryEffectSlotfv = NULL;
LPALGETAUXILIARYEFFECTSLOTI		alGetAuxiliaryEffectSloti = NULL;
LPALGETAUXILIARYEFFECTSLOTIV	alGetAuxiliaryEffectSlotiv = NULL;
LPALGETAUXILIARYEFFECTSLOTF		alGetAuxiliaryEffectSlotf = NULL;
LPALGETAUXILIARYEFFECTSLOTFV	alGetAuxiliaryEffectSlotfv = NULL;
#endif

// XRAM functions and enum values
// ---------------------------------------------------------------------------------------
LPEAXSETBUFFERMODE eaxSetBufferMode = NULL;
LPEAXGETBUFFERMODE eaxGetBufferMode = NULL;
ALenum eXRAMSize = 0;
ALenum eXRAMFree = 0;
ALenum eXRAMAuto = 0;
ALenum eXRAMHardware = 0;
ALenum eXRAMAccessible = 0;

// Extension Queries
// ---------------------------------------------------------------------------------------
ALboolean ALFWIsXRAMSupported()
{
	ALboolean bXRAM = AL_FALSE;

	if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
	{
		// Get X-RAM Function pointers
		eaxSetBufferMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
		eaxGetBufferMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");

		if (eaxSetBufferMode && eaxGetBufferMode)
		{
			eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
			eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
			eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
			eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
			eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

			if (eXRAMSize && eXRAMFree && eXRAMAuto && eXRAMHardware && eXRAMAccessible)
				bXRAM = AL_TRUE;
		}
	}
	return bXRAM;
}

ALboolean ALFWIsEFXSupported()
{
	ALCdevice *pDevice = NULL;
	ALCcontext *pContext = NULL;
	ALboolean bEFXSupport = AL_FALSE;

	pContext = alcGetCurrentContext();
	pDevice = alcGetContextsDevice(pContext);	

	if (alcIsExtensionPresent(pDevice, (ALCchar*)ALC_EXT_EFX_NAME))
	{
		// Get function pointers
		alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
		alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
		alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
		alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
		alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
		alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
		alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
		alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
		alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
		alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
		alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
		alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
		alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
		alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
		alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
		alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
		alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
		alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
		alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
		alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
		alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
		alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
		alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
		alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
		alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
		alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
		alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
		alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
		alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
		alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
		alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
		alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
		alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

		if (alGenEffects &&	alDeleteEffects && alIsEffect && alEffecti && alEffectiv &&	alEffectf &&
			alEffectfv && alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&	alGenFilters &&
			alDeleteFilters && alIsFilter && alFilteri && alFilteriv &&	alFilterf && alFilterfv &&
			alGetFilteri &&	alGetFilteriv && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots &&
			alDeleteAuxiliaryEffectSlots &&	alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
			alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
			alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf &&
			alGetAuxiliaryEffectSlotfv)
			bEFXSupport = AL_TRUE;
	}

	return bEFXSupport;
}
