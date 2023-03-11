#ifndef __NSERVICE_ADVERTISING_H
#define __NSERVICE_ADVERTISING_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NAvertising.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NFlags.h"
#include "lib/N/NCore.h"
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NSHOW_AD_BANNER_TOP 0
#define NSHOW_AD_BANNER_BOTTOM 1

    void NShowADBanner(const Ns32 banner_position);
    void NHideADBanner();

    inline Nbool NIsADInterstitielLoaded()
    {
        if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_AD_INTERSTITIAL_LOADED))
        {
            return NTRUE;
        }
        else
        {
            return NFALSE;
        }
    }
    void NRequestADInterstitiel();
    void NShowADInterstitiel();
    // void NHideADInterstitiel() doesn'exist !

    inline Nbool NIsADRewardedLoaded()
    {
        if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_AD_REWARDED_LOADED))
        {
            return NTRUE;
        }
        else
        {
            return NFALSE;
        }
    }
    void NRequestADRewarded();
    void NShowADRewarded();
    // void NHideADRewarded() doesn'exist !

    void NPrepareADNative();
    void NShowADNative();
// void NHideADNative() doesn'exist !
//  ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif //  __cpluplus
#endif //  __NSERVICE_ADVERTISING_H
