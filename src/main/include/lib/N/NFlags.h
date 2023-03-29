
#pragma once

/*
#ifndef __NFLAGS_H
#define __NFLAGS_H
*/
#include "lib/N/NType.h"
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// BIT Manipulation
#define BITSET(val,bit_id)			((val) |= (1 << (bit_id))) 
#define BITCLEAR(val,bit_id)		((val) &=~(1 << (bit_id)))  
//#define BITGET(val,bit_id)		((val) &  (1 << (bit_id)))
#define BITGET(val,bit_id)			(((val) >> (bit_id)) &  1)	// 0 or 1



#define BITTOGLE(val,bit_id)		((val) ^= (1 << (bit_id)))

// BYTE BIT FIELD
// pbytefield is a valid pointer on Nu8 list. [ Nu8 *pbytefield = (Nu8*)Nmalloc(number_of_wishes_Nu8*sizeof(Nu8); ]
#define BYTEFIELD_SIZE(bit_nb)					( ((bit_nb)>>3) + 1 )	// Number of BYTE(Nu8) for a BITS FIELD with a bits number of  "bit_nb" 
#define BYTEFIELD_BITSET(pbytefield,bit_id)		( (pbytefield[(bit_id)>>3]) |=  (1 << ((bit_id)&0x7)) )
#define BYTEFIELD_BITCLEAR(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) &=~ (1 << ((bit_id)&0x7)) )
//#define BYTEFIELD_BITGET(pbytefield,bit_id)		( (pbytefield[(bit_id)>>3]) &   (1 << ((bit_id)&0x7)) )
#define BYTEFIELD_BITGET(pbytefield,bit_id)		(( (pbytefield[(bit_id)>>3]) >>  ((bit_id)&0x7)) & 1 )		// 0 or 1
#define BYTEFIELD_BITTOGLE(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) ^=  (1 << ((bit_id)&0x7)) )

// Nu32 BIT FIELD
// pNu32field is a valid pointer on Nu32 list. [ Nu32 *pNu32field = (Nu32*)Nmalloc(number_of_wishes_Nu32*sizeof(Nu32); ]
#define Nu32FIELD_SIZE(bit_nb)					( ((bit_nb)>>5) + 1 )	// Number of Nu32 for a BITS FIELD with a bits number of  "bit_nb" 
#define Nu32FIELD_BITSET(pnu32field,bit_id)		( (pnu32field[(bit_id)>>5]) |=  (1 << ((bit_id)&0x1F)) )
#define Nu32FIELD_BITCLEAR(pnu32field,bit_id)	( (pnu32field[(bit_id)>>5]) &=~ (1 << ((bit_id)&0x1F)) )
//#define Nu32FIELD_BITGET(pnu32field,bit_id)		( (pnu32field[(bit_id)>>5]) &   (1 << ((bit_id)&0x1F)) )
#define Nu32FIELD_BITGET(pnu32field,bit_id)		( ((pnu32field[(bit_id)>>5]) >> ((bit_id)&0x1F)) & 1   )

#define Nu32FIELD_BITTOGLE(pnu32field,bit_id)	( (pnu32field[(bit_id)>>5]) ^=  (1 << ((bit_id)&0x1F)) )

// To define a bit mask with 's' bits TRUE:
#define MSK8(s)		((0xFF)>>(8-s))			// !!! WARNING !!  0 <= s  <= 8
#define MSK16(s)	((0xFFFF)>>(16-s))		// !!! WARNING !!  0 <= s  <= 16
#define MSK32(s)	((0xFFFFFFFF)>>(32-s))	// !!! WARNING !!  0 <= s  <= 32


// Flags Manipulation
#define FLAG_TOGGLE(val,flag)		((val) ^= (flag))
#define FLAG_ON(val,flag)			((val) |= (flag))
#define FLAG_OFF(val,flag)			((val) &= ~(flag))
#define ISFLAG_ON(val,flag)			((val) & (flag))						// !! ZERO or NON ZERO value !!! BE AWARE THAT NON ZERO DOESN'T MEAN 1 !!!
#define ISFLAG_OFF(val,flag)		(!((val) & (flag)))						// !! ZERO or NON ZERO value !!! BE AWARE THAT NON ZERO DOESN'T MEAN 1 !!!
#define FLAGS_TEST(val,bmask,flags)	(((val)&(bmask))==(flags))				// NFALSE or NTRUE
#define SET_FLAGS(val,bmask,flags)	((val)=(((val)&(~(bmask)))|(flags)))	// RESET FLAGS BITS and Set them all like flags.
#define RESET_FLAGS(val,bmask)		((val)&=~(bmask)))						// Set all FLAGS BITS to ZERO

inline	Nu32 NGetNu32SetBITCount(Nu32 val){Nu32 nb=0;for(Nu32 i=0;i<32;i++){if(BITGET(val,i))nb++;}return nb;}
inline	Nu32 NGetNu16SetBITCount(Nu16 val){Nu32 nb=0;for(Nu32 i=0;i<16;i++){if(BITGET(val,i))nb++;}return nb;}
inline	Nu32 NGetNu8SetBITCount(Nu8 val){Nu32 nb=0;for(Nu32 i=0;i<8;i++){if(BITGET(val,i))nb++;}return nb;}

//	|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|
//	|31|30|29|28|.|27|26|25|24|.|23|22|21|20|.|19|18|17|16|.|15|14|13|12|.|11|10|09|08|.|07|06|05|04|.|03|02|01|00|
//	--------------------------------------------------------------- -----------------------------------------------
//	|								   				  	  |.|										              |
//	--------------------------------------------------------------- -----------------------------------------------
//	|  |  |  |	|.|  |  |  |  |.|  |  |  |  |.|  |  |  |  |.|  |  |  |  |.|  |  |  |  |.|  |  |  |  |.|  |  |  |  |
//	|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |.|0 |0 |0 |0 |

// BIT definitions
#define BIT_0			1
#define BIT_1			2
#define BIT_2			4
#define BIT_3			8
#define BIT_4			16
#define BIT_5			32
#define BIT_6			64
#define BIT_7			128
#define BIT_8			256
#define BIT_9			512
#define BIT_10			1024
#define BIT_11			2048
#define BIT_12			4096
#define BIT_13			8192
#define BIT_14			16384
#define BIT_15			32768
#define	BIT_16			65536
#define	BIT_17			131072
#define	BIT_18			262144
#define	BIT_19			524288
#define	BIT_20			1048576
#define	BIT_21			2097152
#define	BIT_22			4194304
#define	BIT_23			8388608	
#define	BIT_24			16777216
#define	BIT_25			33554432
#define	BIT_26			67108864
#define	BIT_27			134217728
#define	BIT_28			268435456
#define	BIT_29			536870912
#define	BIT_30			1073741824
#define	BIT_31			2147483648
/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __N_H 
*/
