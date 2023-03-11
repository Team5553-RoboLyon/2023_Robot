// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceMedia.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "lib/N/File/NFile.h"

#ifdef _NANDROID
#include "lib/N/System/NAndroid.h"
#endif

#include "./NServiceMedia.h"

void NPlayVideo(const Nchar *file_name)
{
#ifdef _NANDROID
	Nchar fullpathname[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];
	NBuildJNICall_String("mr_PlayVideo", NGetFullFileName(fullpathname, file_name, 0));
#endif
}
void NStopVideo()
{
#ifdef _NANDROID
	NBuildJNICall("mr_StopVideo");
#endif
}

void NPlayMusic(const char *file_name)
{
#ifdef _NANDROID
	Nchar fullpathname[CONSTANT_NFILE_FULLDATAPATH_MAXSIZE];
	NBuildJNICall_String("mr_PlayMusic", NGetFullFileName(fullpathname, file_name, 0));
#endif
}

void NStopMusic()
{
#ifdef _NANDROID
	NBuildJNICall("mr_StopMusic");
#endif
}
void NPauseMusic()
{
#ifdef _NANDROID
	NBuildJNICall("mr_PauseMusic");
#endif
}
void NStartMusic()
{
#ifdef _NANDROID
	NBuildJNICall("mr_StartMusic");
#endif
}
void NSetMusicVolume(const Nf32 vol)
{
#ifdef _NANDROID
	NBuildJNICall_Nf32("mr_SetMusicVolume", vol);
#endif
}

//===============================================================================================================================================================================================
/*
NztBool	AndroidIsVideoEndCallBack( void )
{
	return ( callbackFVideo ) ? true : false;
}
NztBool	AndroidIsMusicEndCallBack( void )
{
	return ( callbackFMusic ) ? true : false;
}
void	AndroidSetVideoEndCallBack( void (NztDecl*callback_funct)(void) )
{
	callbackFVideo = callback_funct;
}
void	AndroidSetMusicEndCallBack( void (NztDecl*callback_funct)(void) )
{
	callbackFMusic = callback_funct;
}
*/
//===============================================================================================================================================================================================
/*
float	AndroidGetMusicVolume( void )
{
	return AndroidMusicVolume;
}
*/
//===============================================================================================================================================================================================
/*
NztBool	AndroidIsVideoPlaying( void )
{
	return bIsVideoPlaying;
}
NztBool	AndroidIsMusicPlaying( void )
{
	return bIsMusicPlaying;
}
*/
