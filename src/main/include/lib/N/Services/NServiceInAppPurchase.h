#ifndef __NSERVICE_IN_APP_PURCHASE_H
#define __NSERVICE_IN_APP_PURCHASE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceInAppPurchase.h							**
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
    void NRequestInAppPurchaseProductData(const char *str_prod_id);
    void NRequestInAppPurchasePayment(const char *str_prod_id);
    void NRequestInAppPurchaseRestore(const char *str_prod_id);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif //  __cpluplus
#endif //  __NSERVICE_IN_APP_PURCHASE_H
