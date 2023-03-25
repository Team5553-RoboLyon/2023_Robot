#pragma once
/*
#ifndef __NSTRING_H
#define __NSTRING_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NString.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************

Nchar*			NStrCopy(Nchar *destination, const Nchar *source, const Nu32 sizemax_terminating_null_character_included );
Nchar*			NStrCat(Nchar *destination, const Nchar *source, const Nu32 sizemax_terminating_null_character_included );
Nchar*			NStrDuplicate(const Nchar *psource);

Nu32			NMakeUIDFromString( const char *ename );

inline	 Nchar*	NStrMove_AfterLabel(Nchar* string, const Nchar* tag){return strstr(string, tag) + strlen(tag);};

Nchar*			NStrGet_Nf32(Nchar *psource,Nf32 *pfloat);
Nchar*			NStrGet_Nf32_AfterLabel(Nchar *string, const Nchar *tag, Nf32 *pf);

Nchar*			NStrGet_Nf64(Nchar *psource, Nf64 *pfloat);
Nchar*			NStrGet_Nf64_AfterLabel(Nchar *string, const Nchar* tag, Nf64 *pf);

Nchar*			NStrGet_Ns16(Nchar *psource,Ns16 *pinteger);
Nchar*			NStrGet_Ns16_AfterLabel (const Nchar *string, const Nchar *tag, Ns16 *pinteger);

Nchar*			NStrGet_Ns32(Nchar *psource,Ns32 *pinteger);
Nchar*			NStrGet_Ns32_AfterLabel (Nchar *string, const Nchar* tag, Ns32 *pint);

Nchar*			NStrGet_String_BeforeLabel(const Nchar *psource, const Nchar *plabel, Nchar *pdest);
Nchar*			NStrGet_String_AfterLabel(const Nchar *psource,const Nchar *plabel, Nchar *pdest);
Nchar*			NStrGet_String_BetweenLabels(const Nchar *psource,const Nchar *plabel0, const Nchar *plabel1, Nchar *pdest);

Nu32			NStrGetLineCount(const Nchar *pstring);
Nbool			NStrCheckEnd(const Nchar* pstr, const Nchar* pend);

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSTRING_H 
*/