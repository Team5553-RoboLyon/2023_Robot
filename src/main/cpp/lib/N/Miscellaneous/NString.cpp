#include "lib/N/NCStandard.h"
//#include "../GL/Ngl.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Miscellaneous/NString.h"
#include "lib/N/Maths/NMath.h"

Nchar* NStrGet_Ns16_AfterLabel (Nchar *string, const Nchar* tag, Ns16 *pinteger)
{
	NErrorIf(!string, NERROR_STRING_NULL);
	NErrorIf(!tag, NERROR_STRING_NULL);
	NErrorIf(!pinteger,NERROR_NULL_POINTER);

	int numberlength;
	char numbertemp[32];
	char *pchar;

	
	pchar = strstr(string, tag);
	if(!pchar)
		return string;

	pchar += strlen(tag);
	for (numberlength = 0; strchr("0123456789-",*(pchar + numberlength)) ; numberlength ++);

	if(numberlength)
	{
		memset(numbertemp,0,32);
		memcpy (numbertemp, pchar, sizeof (char) * numberlength);
		*pinteger = (Ns16)atoi(numbertemp);
		return (pchar + numberlength);
	}
	else
		return string;
}

Nchar *NStrGet_Ns16(Nchar *psource,Ns16 *pinteger)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!pinteger,NERROR_NULL_POINTER);

	Nchar numbertemp[32];// avoiding initialization like this ={0}, because it will be performed later thanks to  memset only if its necessary 
	Nu16 numberlength = 0;

	while (strchr("0123456789-",*(psource + numberlength) ))
	{
		numberlength ++;
	}

	if(numberlength)
	{
		memset(numbertemp,0,32);
		memcpy(numbertemp, psource, sizeof(Nchar) * numberlength);
		numbertemp[numberlength] = 0;
		*pinteger = (Ns16)atoi(numbertemp);
	}

	return ( psource + numberlength );
}

Nchar* NStrGet_Ns32_AfterLabel (Nchar *string, const Nchar* tag, Ns32 *pint)
{
	NErrorIf(!string, NERROR_STRING_NULL);
	NErrorIf(!tag, NERROR_STRING_NULL);
	NErrorIf(!pint,NERROR_NULL_POINTER);

	int numberlength;
	char numbertemp[32];
	char *pchar;

	pchar = strstr(string, tag);
	if(!pchar)
		return string;

	pchar += strlen(tag);
	for (numberlength = 0; strchr("0123456789-",*(pchar + numberlength)) ; numberlength ++);

	if(numberlength)
	{
		memset(numbertemp,0,32);
		memcpy (numbertemp, pchar, sizeof (char) * numberlength);
		*pint = (Ns32)atoi(numbertemp);
		return (pchar + numberlength);
	}
	else
		return string;
}

Nchar* NStrGet_Ns32(Nchar *psource,Ns32 *pinteger)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!pinteger,NERROR_NULL_POINTER);

	Nchar numbertemp[32];// avoiding initialization like this ={0}, because it will be performed later thanks to  memset only if its necessary 
	Nu16 numberlength = 0;

	while (strchr("0123456789-",*(psource + numberlength) ))
	{
		numberlength ++;
	}

	if(numberlength)
	{
		memset(numbertemp,0,32);
		memcpy(numbertemp, psource, sizeof(Nchar) * numberlength);
		*pinteger = (Ns32)atoi(numbertemp);
	}
	
	return ( psource + numberlength );
}

Nchar *NStrGet_Nf32_AfterLabel (Nchar *string, const Nchar* tag, Nf32 *pf)
{
	NErrorIf(!string, NERROR_STRING_NULL);
	NErrorIf(!tag, NERROR_STRING_NULL);
	NErrorIf(!pf,NERROR_NULL_POINTER);

	Nu16 numberlength = 0;
	char numbertemp[32];
	char *pchar;

	pchar = strstr(string, tag);
	if(!pchar)
		return string;

	pchar += strlen(tag);
	//for (numberlength = 0; strchr("0123456789-.",*(pchar + numberlength)) ; numberlength ++);
	while (strchr("0123456789-.", *(pchar + numberlength))) { numberlength++; }

	if(numberlength)
	{
		NErrorIf(numberlength > 32, NERROR_VALUE_OUTOFRANGE);
		// number of charactere in the string is bigger than the temporaty buffer !!!! 
		// We should increase temporary buffer from 32 to 64 !!!

		memset(numbertemp,0,32);
		memcpy (numbertemp, pchar, sizeof (char) * numberlength);
		*pf = (Nf32)atof(numbertemp);
		return (pchar + numberlength);
	}
	else
		return string;
}

Nchar *NStrGet_Nf32(Nchar *psource,Nf32 *pfloat)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!pfloat,NERROR_NULL_POINTER);

	Nchar numbertemp[32];// avoiding initialization like this ={0}, because it will be performed later thanks to  memset only if its necessary 
	Nu16 numberlength = 0;

	while (strchr("0123456789-.",*(psource+numberlength) ) ){numberlength ++;}

	if (numberlength)
	{
		NErrorIf(numberlength > 32, NERROR_VALUE_OUTOFRANGE);
		// number of charactere in the string is bigger than the temporaty buffer !!!! 
		// We should increase temporary buffer from 32 to 64 !!!

		memset(numbertemp,0,32);
		memcpy(numbertemp, psource, sizeof(Nchar) * numberlength);
		*pfloat = (Nf32)atof(numbertemp);
	}
	
	return ( psource + numberlength );
}

Nchar *NStrGet_Nf64_AfterLabel(Nchar *string, const Nchar* tag, Nf64 *pf)
{
	NErrorIf(!string, NERROR_STRING_NULL);
	NErrorIf(!tag, NERROR_STRING_NULL);
	NErrorIf(!pf, NERROR_NULL_POINTER);

	Nu16 numberlength = 0;
	char numbertemp[64];
	char *pchar;

	pchar = strstr(string, tag);
	if (!pchar)
		return string;

	pchar += strlen(tag);
	//for (numberlength = 0; strchr("0123456789-.",*(pchar + numberlength)) ; numberlength ++);
	while (strchr("0123456789-.", *(pchar + numberlength))) { numberlength++; }

	if (numberlength)
	{
		NErrorIf(numberlength > 64, NERROR_VALUE_OUTOFRANGE);
		// number of charactere in the string is bigger than the temporaty buffer !!!! 
		// We should increase temporary buffer from 64 to 128 !!!

		memset(numbertemp, 0, 64);
		memcpy(numbertemp, pchar, sizeof(char) * numberlength);
		*pf = (Nf64)atof(numbertemp);
		return (pchar + numberlength);
	}
	else
		return string;
}

Nchar *NStrGet_Nf64(Nchar *psource, Nf64 *pdouble)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!pdouble, NERROR_NULL_POINTER);

	Nchar numbertemp[64];// avoiding initialization like this ={0}, because it will be performed later thanks to  memset only if its necessary 
	Nu16 numberlength = 0;

	while (strchr("0123456789-.", *(psource + numberlength)))
	{
		numberlength++;
	}
	
	if (numberlength)
	{
		NErrorIf(numberlength > 64, NERROR_VALUE_OUTOFRANGE); 
		// number of charactere in the string is bigger than the temporaty buffer !!!! 
		// We should increse temporary buffer from 64 to 128 !!!

		memset(numbertemp, 0, 64);
		memcpy(numbertemp, psource, sizeof(Nchar) * numberlength);
		*pdouble = (Nf64)atof(numbertemp);
	}

	return (psource + numberlength);
}

// ------------------------------------------------------------------------------------------
// Nchar* NStrGet_String_BetweenLabels
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			const Nchar *psource	a valid pointer on a string to look into
//			const Nchar *plabel0	a valid pointer on the first label, a string to find into "psource"
//			const Nchar *plabel1	a valid pointer on the second label, a string to find into "psource"
//				  Nchar *pdest		a valid pointer on a string where all the characters of "psource" 
//									which are after the last character of "plabel" are going to be copied into.	
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nchar *NStrGet_String_BetweenLabels(const Nchar *psource,const Nchar *plabel0, const Nchar *plabel1, Nchar *pdest)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!plabel0, NERROR_STRING_NULL);
	NErrorIf(!plabel1, NERROR_STRING_NULL);
	NErrorIf(!pdest, NERROR_STRING_NULL);

	Nu32	textlength = 0;
	Nchar	*pstr;
	Nchar	*pchar;

	pchar = strstr((Nchar*)psource,(Nchar*)plabel0);
	if(pchar)
	{
		pchar += strlen(plabel0);
		pstr = strstr(pchar, plabel1);
		if ( pstr )
		{
			textlength = pstr - pchar;
			memcpy(pdest, pchar, sizeof(Nchar) * textlength);
			pdest[textlength] = 0;
			return ( pchar + textlength );
		}
	}
	
	// By default:
	pdest[0] = 0;
	return (Nchar*)psource;
}
// ------------------------------------------------------------------------------------------
// Nchar* NStrGet_String_AfterLabel
// ------------------------------------------------------------------------------------------
// Description :
//	Find the first occurrence of "plabel" in "psource" and copy all the characters of psource that are after it
//	into "pdest". 
//	The function return a pointer on the first character of psource just after the last one copied into pdest.
//	Which is the terminating NULL-character of "psource" 
// ------------------------------------------------------------------------------------------
// In	:
//			const Nchar *psource	a valid pointer on a string to look into
//			const Nchar *plabel		a valid pointer on the string to find into "psource"
//				  Nchar *pdest		a valid pointer on a string where all the characters of "psource" 
//									which are after the last character of "plabel" are going to be copied into.	
// Out :
//		a pointer to the  terminating NULL-character of "psource" 
//
// ------------------------------------------------------------------------------------------
Nchar *NStrGet_String_AfterLabel(const Nchar *psource,const Nchar *plabel, Nchar *pdest)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!plabel, NERROR_STRING_NULL);
	NErrorIf(!pdest, NERROR_STRING_NULL);

	Nu32	textlength = 0;
	Nchar	*pstr;

	pstr = strstr((Nchar*)psource,(Nchar*)plabel);
	if(!pstr)
	{
		pdest[0] = 0;
		return (Nchar*)psource;
	}
	pstr += strlen(plabel);

	if ( pstr )
	{
		textlength = strlen(psource)-(pstr - psource);
		memcpy(pdest, pstr, sizeof(Nchar) * textlength);
		pdest[textlength] = 0;
		return ( pstr + textlength ); // which should be the NULL terminating character of psource
	}
	else
	{
		pdest[0] = 0;
		return (Nchar*)psource;
	}
}
// ------------------------------------------------------------------------------------------
// Nchar* NStrGet_String_BeforeLabel
// ------------------------------------------------------------------------------------------
// Description :
//	Find the first occurrence of "plabel" in "psource" and copy all the characters of psource that are before it
//	into "pdest". 
//	The function return a pointer on the first character of psource just after the last one copied into pdest.
//	Which is the first character of "plabel" 
// ------------------------------------------------------------------------------------------
// In	:
//			const Nchar *psource	a valid pointer on a string to look into
//			const Nchar *plabel		a valid pointer on the string to find into "psource"
//				  Nchar *pdest		a valid pointer on a string where all the character of "psource" 
//									which are before the first character of "plabel" are going to be copied into.	
// Out :
//		a pointer to the first character (in psource) of "plabel" 
//
// ------------------------------------------------------------------------------------------
Nchar* NStrGet_String_BeforeLabel(const Nchar *psource, const Nchar *plabel, Nchar *pdest)
{
	NErrorIf(!psource, NERROR_STRING_NULL);
	NErrorIf(!plabel, NERROR_STRING_NULL);
	NErrorIf(!pdest, NERROR_STRING_NULL);
	NErrorIf(pdest==psource, NERROR_INCONSISTENT_PARAMETERS);

	Nu32 textlength = 0;
	Nchar	*pstr;

	pstr = strstr((Nchar*)psource,(Nchar*)plabel);
	if ( pstr )
	{
		textlength = pstr - psource;
		if(textlength)
		{
			memcpy(pdest, psource, sizeof(Nchar) * textlength);
			pdest[textlength] = 0;
		}
		else
		{
			pdest[0] = 0;
		}
	}
	else
	{
		pdest[0] = 0;
	}
	return ( (Nchar*)psource + textlength );
}

Nchar *NStrCopy(Nchar *destination, const Nchar *source,const Nu32 sizemax_terminating_null_character_included )
{
	NErrorIf(!source, NERROR_STRING_NULL);
	NErrorIf(!destination, NERROR_STRING_NULL);
	NErrorIf(sizemax_terminating_null_character_included < 1,NERROR_STRING_SIZEMAX_HAS_TO_BE_EQUAL_OR_GREATER_THAN_1);
	
	if( strlen(source) < (sizemax_terminating_null_character_included-1) ) // -1 to include null terminated char.
		return strcpy(destination,source);
	else
	{
		memcpy( destination,source,sizeof(Nchar)*(sizemax_terminating_null_character_included-1) );
		destination[sizemax_terminating_null_character_included-1]=0;
	}
	return destination;
}



Nchar *NStrCat(Nchar *destination, const Nchar *source,const Nu32 sizemax_terminating_null_character_included )
{
	NErrorIf(!source, NERROR_STRING_NULL);
	NErrorIf(!destination, NERROR_STRING_NULL);
	NErrorIf(sizemax_terminating_null_character_included < 1,NERROR_STRING_SIZEMAX_HAS_TO_BE_EQUAL_OR_GREATER_THAN_1);
	NErrorIf( strlen(destination) > sizemax_terminating_null_character_included-1,NERROR_STRING_TOO_LONG);

	Nu32	len;

	if( ( strlen(destination) + strlen(source) ) < ( sizemax_terminating_null_character_included - 1 ) ) // -1 to include null terminated char.
		return strcat(destination,source);
	else
	{
		len = NMIN( strlen(source), sizemax_terminating_null_character_included - strlen(destination) - 1 );
		memcpy( destination+strlen(destination)+1,source,sizeof(Nchar)*len );
		destination[strlen(destination)+len] = 0;
	}

	return destination;
}

Nchar* NStrDuplicate(const Nchar *psource)
{
	if(psource && strlen(psource)>0)
	{
		Nchar *pdup = (Nchar*)Nmalloc((strlen(psource)+1)*sizeof(Nchar));
		return (Nchar*)strcpy(pdup,psource);
	}
	else
	{
		return NULL;
	}
}

Nu32 NMakeUIDFromString( const char *ename )
{
	NErrorIf(!ename, NERROR_STRING_NULL);

	// Optimized Method
	const	Nu16*	data = (Nu16*)ename;				 // Pointer to the data to be summed 
	Nu32				len  = (strlen(ename)+1) >> 1;   // Length in 16-bit words 
	Nu32				sum1 = 0xffff;
	Nu32				sum2 = 0xffff;

	while ( len-- )
	{
		sum1 += *data++;
		sum2 += sum1;

	}

	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// Second reduction step to reduce sums to 16 bits 
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	// return identifier
	return sum2 << 16 | sum1;
	//---

	//--- Original Method
	//const	u16*	data = (u16*)ename;				// Pointer to the data to be summed 
	//u32				len  = (strlen(ename)+1) >> 1;   // Length in 16-bit words 
	//u32				sum1 = 0xffff;
	//u32				sum2 = 0xffff;

	//while (len)
	//{
	//	unsigned tlen = len > 360 ? 360 : len;
	//	len -= tlen;
	//	do 
	//	{
	//		sum1 += *data++;
	//		sum2 += sum1;

	//	} while (--tlen);

	//	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	//	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	//}

	//// Second reduction step to reduce sums to 16 bits 
	//sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	//sum2 = (sum2 & 0xffff) + (sum2 >> 16);

	//// return identifier
	//return sum2 << 16 | sum1;
	//---


	//--- Methode au pif (fonctionne mais peut faire mieux)
	//u32 ecode = 0;
	//const char* buff = ename;
	//s32 i = strlen(ename);

	//while ( i )
	//{
	//	ecode += (i+1)*((*buff)-i)*((*buff)+i);
	//	//ecode += (i+1)*((*buff)-i)*(i+(*buff)*(i+1));

	//	buff++;
	//	i--;
	//}

	//return ecode;
	//---
}

Nu32 NStrGetLineCount(const Nchar *pstring)
{
	Nu32 len = (Nu32)strlen(pstring);
	if (!len)
		return 0;
	Nu32		line = 1;
	Nchar		*pstr = (Nchar*)pstring;
	Nu32		i;

	for (i = len; i != 0; i--, pstr++)
	{
		if (*pstr == '\n')
			line++;
	}
	return line;
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Check if a string is the end of an another one.
 *
 *	@param		pstr is a pointer on the string to check against ( for example :  a filename name ).
 *	@param		pend is a pointer on the string which is supposed to be "the end" of pstr.
  *	@return		a boolean value
 *					NTRUE		if 'ptail' is the end of pstr.
 *					NFALSE		if not.
 */
 // ------------------------------------------------------------------------------------------

Nbool NStrCheckEnd(const Nchar* pstr, const Nchar* pend)
{
	NErrorIf(!pstr || !pend, NERROR_NULL_POINTER);
	const Nchar* pstrx = pstr + strlen(pstr) - strlen(pend);
	if (strcmp(pstrx, pend))
		return NFALSE;

	return NTRUE;
}
