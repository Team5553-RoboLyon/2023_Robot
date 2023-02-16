#ifndef __NAL_H
#define __NAL_H


#ifdef _NWINDOWS
// AL libraries for WINDOWS
// --------------------------------------------------------------------------------------------
#include <al.h>
#include <alc.h>
#include <xram.h>
#include <efx.h>
#include <efx-creative.h>

// AL Extensions (EFX and XRAM)
// notes: 
//			The lines bellow are based on "Framework.h" created by Creative Labs in 2006
//			Original File is saved as "_PROJET_C\_LIBS\OpenAL\src\Audio\OpenAL\Framework.h"
// --------------------------------------------------------------------------------------------
ALboolean ALFWIsXRAMSupported();
ALboolean ALFWIsEFXSupported();

// EFX Extension function pointer variables
// ----------------------------------------------------------
// Effect objects
extern LPALGENEFFECTS		alGenEffects;
extern LPALDELETEEFFECTS	alDeleteEffects;
extern LPALISEFFECT			alIsEffect;
extern LPALEFFECTI			alEffecti;
extern LPALEFFECTIV			alEffectiv;
extern LPALEFFECTF			alEffectf;
extern LPALEFFECTFV			alEffectfv;
extern LPALGETEFFECTI		alGetEffecti;
extern LPALGETEFFECTIV		alGetEffectiv;
extern LPALGETEFFECTF		alGetEffectf;
extern LPALGETEFFECTFV		alGetEffectfv;

// Filter objects
extern LPALGENFILTERS		alGenFilters;
extern LPALDELETEFILTERS	alDeleteFilters;
extern LPALISFILTER			alIsFilter;
extern LPALFILTERI			alFilteri;
extern LPALFILTERIV			alFilteriv;
extern LPALFILTERF			alFilterf;
extern LPALFILTERFV			alFilterfv;
extern LPALGETFILTERI		alGetFilteri;
extern LPALGETFILTERIV		alGetFilteriv;
extern LPALGETFILTERF		alGetFilterf;
extern LPALGETFILTERFV		alGetFilterfv;

// Auxiliary slot object
extern LPALGENAUXILIARYEFFECTSLOTS		alGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS	alDeleteAuxiliaryEffectSlots;
extern LPALISAUXILIARYEFFECTSLOT		alIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI			alAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV		alAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF			alAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV		alAuxiliaryEffectSlotfv;
extern LPALGETAUXILIARYEFFECTSLOTI		alGetAuxiliaryEffectSloti;
extern LPALGETAUXILIARYEFFECTSLOTIV		alGetAuxiliaryEffectSlotiv;
extern LPALGETAUXILIARYEFFECTSLOTF		alGetAuxiliaryEffectSlotf;
extern LPALGETAUXILIARYEFFECTSLOTFV		alGetAuxiliaryEffectSlotfv;

// XRAM Extension function pointer variables and enum values
// ----------------------------------------------------------
typedef ALboolean (__cdecl *LPEAXSETBUFFERMODE)(ALsizei n, ALuint *buffers, ALint value);
typedef ALenum    (__cdecl *LPEAXGETBUFFERMODE)(ALuint buffer, ALint *value);

extern LPEAXSETBUFFERMODE eaxSetBufferMode;
extern LPEAXGETBUFFERMODE eaxGetBufferMode;

// X-RAM Enum values
extern ALenum eXRAMSize, eXRAMFree;
extern ALenum eXRAMAuto, eXRAMHardware, eXRAMAccessible;
#endif // _NWINDOWS

#ifdef _NIOS
// AL libraries for IOS
todo
#endif

#ifdef _NANDROID
// AL libraries for ANDROID
// --------------------------------------------------------------------------------------------
#include <al.h>
#include <alc.h>
#include <xram.h>
#include <efx.h>
#include <efx-creative.h>

// AL Extensions (EFX and XRAM)
// notes: 
//			The lines bellow are based on "Framework.h" created by Creative Labs in 2006
//			Original File is saved as "_PROJET_C\_LIBS\OpenAL\src\Audio\OpenAL\Framework.h"
// --------------------------------------------------------------------------------------------
ALboolean ALFWIsXRAMSupported();
ALboolean ALFWIsEFXSupported();

// EFX Extension function pointer variables
// ----------------------------------------------------------
// Effect objects
extern LPALGENEFFECTS		alGenEffects;
extern LPALDELETEEFFECTS	alDeleteEffects;
extern LPALISEFFECT			alIsEffect;
extern LPALEFFECTI			alEffecti;
extern LPALEFFECTIV			alEffectiv;
extern LPALEFFECTF			alEffectf;
extern LPALEFFECTFV			alEffectfv;
extern LPALGETEFFECTI		alGetEffecti;
extern LPALGETEFFECTIV		alGetEffectiv;
extern LPALGETEFFECTF		alGetEffectf;
extern LPALGETEFFECTFV		alGetEffectfv;

// Filter objects
extern LPALGENFILTERS		alGenFilters;
extern LPALDELETEFILTERS	alDeleteFilters;
extern LPALISFILTER			alIsFilter;
extern LPALFILTERI			alFilteri;
extern LPALFILTERIV			alFilteriv;
extern LPALFILTERF			alFilterf;
extern LPALFILTERFV			alFilterfv;
extern LPALGETFILTERI		alGetFilteri;
extern LPALGETFILTERIV		alGetFilteriv;
extern LPALGETFILTERF		alGetFilterf;
extern LPALGETFILTERFV		alGetFilterfv;

// Auxiliary slot object
extern LPALGENAUXILIARYEFFECTSLOTS		alGenAuxiliaryEffectSlots;
extern LPALDELETEAUXILIARYEFFECTSLOTS	alDeleteAuxiliaryEffectSlots;
extern LPALISAUXILIARYEFFECTSLOT		alIsAuxiliaryEffectSlot;
extern LPALAUXILIARYEFFECTSLOTI			alAuxiliaryEffectSloti;
extern LPALAUXILIARYEFFECTSLOTIV		alAuxiliaryEffectSlotiv;
extern LPALAUXILIARYEFFECTSLOTF			alAuxiliaryEffectSlotf;
extern LPALAUXILIARYEFFECTSLOTFV		alAuxiliaryEffectSlotfv;
extern LPALGETAUXILIARYEFFECTSLOTI		alGetAuxiliaryEffectSloti;
extern LPALGETAUXILIARYEFFECTSLOTIV		alGetAuxiliaryEffectSlotiv;
extern LPALGETAUXILIARYEFFECTSLOTF		alGetAuxiliaryEffectSlotf;
extern LPALGETAUXILIARYEFFECTSLOTFV		alGetAuxiliaryEffectSlotfv;

// XRAM Extension function pointer variables and enum values
// ----------------------------------------------------------
typedef ALboolean (__cdecl *LPEAXSETBUFFERMODE)(ALsizei n, ALuint *buffers, ALint value);
typedef ALenum    (__cdecl *LPEAXGETBUFFERMODE)(ALuint buffer, ALint *value);

extern LPEAXSETBUFFERMODE eaxSetBufferMode;
extern LPEAXGETBUFFERMODE eaxGetBufferMode;

// X-RAM Enum values
extern ALenum eXRAMSize, eXRAMFree;
extern ALenum eXRAMAuto, eXRAMHardware, eXRAMAccessible;
#endif // _NANDROID


/*
#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
*/

#endif  // __NGL_H 
