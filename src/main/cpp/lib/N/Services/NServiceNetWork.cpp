// ==========================================================================================
// NServiceNetWork.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 11/03/2016
// Updated :
// ------------------------------------------------------------------------------------------
//
// ==========================================================================================
#include "NServiceNetWork.h"
#include "lib/N/NErrorHandling.h"

#ifdef _NANDROID
#include "../System/NAndroid.h"
#endif

// ------------------------------------------------------------------------------------------
// void NOpenURL
// ------------------------------------------------------------------------------------------
// Description :
//	Open an URL inside the default device Web browser
// ------------------------------------------------------------------------------------------
// In	:
//			const Nchar* str_url	URL to open.
//
// Out :
// ------------------------------------------------------------------------------------------
void NOpenURL(const Nchar *purl_str)
{
	NErrorIf(!purl_str, NERROR_STRING_NULL);
#ifdef _NWINDOWS
// a v�rifier... genere une erreur de compilation >>> ShellExecute( NULL, "open", purl_str, NULL, NULL, SW_SHOWNORMAL );
#endif

#ifdef _NIOS
	TODO
#endif

#ifdef _NANDROID
		NBuildJNICall_String("mr_OpenURL", purl_str);
#endif
}

void NStartURL(const Nchar *purl_str, const Nf32 time_out)
{
#ifdef _NANDROID
	NBuildJNICall_String_Nf32("mr_StartURL", purl_str, time_out);
#endif
}