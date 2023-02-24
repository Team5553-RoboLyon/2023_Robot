#ifndef __NSERVICE_SYSTEM_H
#define __NSERVICE_SYSTEM_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceSystem.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------
    void NDisplayMessageBox(const Nchar *ptitle_str, const Nchar *pmessage_str);
    void NMinimizeApp();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif //  __cpluplus
#endif //  __NSERVICE_SYSTEM_H
