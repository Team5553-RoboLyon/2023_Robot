// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceAdvertising.cpp							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "../NCStandard.h"
#include "../NType.h"
#ifdef _NANDROID
#include "../System/NAndroid.h"
#endif

#include "./NServiceAdvertising.h"

/*
void NServiceAdvertising_PrepareBanner()
{
	#ifdef _NANDROID
	NBuildJNICall( "mr_PrepareADBanner" );
	#endif
}
*/

void NShowADBanner(const Ns32 banner_position)
{
	#ifdef _NANDROID
	NBuildJNICall_Ns32( "mr_ShowADBanner", banner_position );
	#endif
}

void NHideADBanner()
{
	#ifdef _NANDROID
	NBuildJNICall( "mr_HideADBanner");
	#endif
}

void NRequestADInterstitiel()
{
    #ifdef _NANDROID
    NBuildJNICall( "mr_RequestADInterstitial");
    #endif
}
void NShowADInterstitiel()
{
	#ifdef _NANDROID
	NBuildJNICall( "mr_ShowADInterstitial" );
	#endif
}
//void NHideADInterstitiel() doesn'exist !

void NRequestADRewarded()
{
	#ifdef _NANDROID
	NBuildJNICall( "mr_RequestADRewarded" );
	#endif
}
void NShowADRewarded()
{
	#ifdef _NANDROID
	NBuildJNICall( "mr_ShowADRewarded" );
	#endif
}
//void NHideADRewarded() doesn'exist !

void NPrepareADNative()
{
#ifdef _NANDROID
	//Todo
#endif
}

void NShowADNative()
{
#ifdef _NANDROID
	NBuildJNICall( "InitNativedAD" );
#endif
}
//void NHideADNative() doesn'exist !

