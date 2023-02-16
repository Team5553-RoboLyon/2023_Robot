#ifdef _NANDROID 
#ifndef __NANDROID_H
#define __NANDROID_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NAndroid.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void NBuildJNICall( const char *fnct_name );
void NBuildJNICall_Ns32( const char *fnct_name, const Ns32 param_Ns32 );
void NBuildJNICall_Nf32( const char *fnct_name, const Nf32 param_Nf32 );
void NBuildJNICall_4Nf32( const char *fnct_name, const Nf32 parama_Nf32, const Nf32 paramb_Nf32, const Nf32 paramc_Nf32, const Nf32 paramd_Nf32 );
void NBuildJNICall_String( const char *fnct_name, const Nchar *param_str );
void NBuildJNICall_2String ( const char *fnct_name, const Nchar *parama_str, const Nchar *paramb_str );
void NBuildJNICall_String_Nf32( const char *fnct_name, const Nchar *param_str, const Nf32 param_Nf32 );
#ifdef __cplusplus
}
#endif	//  __cpluplus
#endif	//  __NANDROID_H 
#endif	//  _NANDROID
