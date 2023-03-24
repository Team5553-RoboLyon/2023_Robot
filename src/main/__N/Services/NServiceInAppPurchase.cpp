// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceInAppPurchase.cpp						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "../NCStandard.h"
#include "../NType.h"
#ifdef _NANDROID
#include "../System/NAndroid.h"
#endif

#include "./NServiceInAppPurchase.h"

void NRequestInAppPurchaseProductData( const char* str_prod_id )
{
	#ifdef _NANDROID
	NBuildJNICall_String ( "InAppProductData", str_prod_id );
	#endif
}

void NRequestInAppPurchasePayment( const char* str_prod_id )
{
	#ifdef _NANDROID
	NBuildJNICall_String ( "InAppPayment", str_prod_id );
	#endif
}

void NRequestInAppPurchaseRestore( const char* str_prod_id )
{
	#ifdef _NANDROID
	NBuildJNICall_String ( "InAppPayment", str_prod_id );
	#endif
}

