#pragma once

#include "nlType.h"

// BIT ID
//	|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|.|___________|
//	|31|30|29|28|.|27|26|25|24|.|23|22|21|20|.|19|18|17|16|.|15|14|13|12|.|11|10|09|08|.|07|06|05|04|.|03|02|01|00|
//	--------------------------------------------------------------- -----------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// BIT Manipulation ( using the BIT ID )
#define nlBitSet(val,bit_id)		((val) |= (1 << (bit_id))) 
#define nlBitClear(val,bit_id)		((val) &=~(1 << (bit_id)))  
#define nlBitGet(val,bit_id)		((val) &  (1 << (bit_id)))
#define nlBitToggle(val,bit_id)		((val) ^= (1 << (bit_id)))

inline	nlu32 nlu32GetValidBitCount(nlu32 val) { nlu32 nb = 0; for (nlu32 i = 0; i < 32; i++) { if (nlBitGet(val, i))nb++; }return nb; }
inline	nlu32 nlu16GetValidBitCount(nlu16 val) { nlu32 nb = 0; for (nlu32 i = 0; i < 16; i++) { if (nlBitGet(val, i))nb++; }return nb; }
inline	nlu32 nlu8GetValidBitCount(nlu8 val) { nlu32 nb = 0; for (nlu32 i = 0; i < 8; i++) { if (nlBitGet(val, i))nb++; }return nb; }


//BIT FIELD
// pbytefield is a valid pointer on nlu8 list. [ nlu8 *pbytefield = (Nu8*)Nmalloc(number_of_wishes_bytes*sizeof(nlu8); ]
#define nlbyteFieldSize(bit_nb)				( ((bit_nb)>>3) + 1 )	// Number of BYTE for a BITS FIELD with a bits number of  "bit_nb" 
#define nlbyteFieldBitSet(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) |=  (1 << ((bit_id)&0x7)) )
#define nlbyteFieldBitClear(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) &=~ (1 << ((bit_id)&0x7)) )
#define nlbyteFieldBitGet(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) &   (1 << ((bit_id)&0x7)) )
#define nlbyteFieldBitToggle(pbytefield,bit_id)	( (pbytefield[(bit_id)>>3]) ^=  (1 << ((bit_id)&0x7)) )

// nlu32 BIT FIELD
// pnlu32field is a valid pointer on nlu32 list. [ nlu32 *pnlu32field = (Nu32*)Nmalloc(number_of_wishes_Nu32*sizeof(nlu32); ]
#define nlu32FieldSize(bit_nb)					( ((bit_nb)>>5) + 1 )	// Number of Nu32 for a BITS FIELD with a bits number of  "bit_nb" 
#define nlu32FieldBitSet(pnlu32field,bit_id)	( (pnlu32field[(bit_id)>>5]) |=  (1 << ((bit_id)&0x1F)) )
#define nlu32FieldBitClear(pnlu32field,bit_id)	( (pnlu32field[(bit_id)>>5]) &=~ (1 << ((bit_id)&0x1F)) )
#define nlu32FieldBitGet(pnlu32field,bit_id)	( (pnlu32field[(bit_id)>>5]) &   (1 << ((bit_id)&0x1F)) )
#define nlu32FieldBitToggle(pnlu32field,bit_id)	( (pnlu32field[(bit_id)>>5]) ^=  (1 << ((bit_id)&0x1F)) )

// FLAG definitions (FLAG_x represents the decimal value of a 32 bits value with the bit n°x set to 1 and all the 31 other bits set to 0 )
#define FLAG_0			1			
#define FLAG_1			2
#define FLAG_2			4
#define FLAG_3			8
#define FLAG_4			16
#define FLAG_5			32
#define FLAG_6			64
#define FLAG_7			128
#define FLAG_8			256
#define FLAG_9			512
#define FLAG_10			1024
#define FLAG_11			2048
#define FLAG_12			4096
#define FLAG_13			8192
#define FLAG_14			16384
#define FLAG_15			32768
#define	FLAG_16			65536
#define	FLAG_17			131072
#define	FLAG_18			262144
#define	FLAG_19			524288
#define	FLAG_20			1048576
#define	FLAG_21			2097152
#define	FLAG_22			4194304
#define	FLAG_23			8388608	
#define	FLAG_24			16777216
#define	FLAG_25			33554432
#define	FLAG_26			67108864
#define	FLAG_27			134217728
#define	FLAG_28			268435456
#define	FLAG_29			536870912
#define	FLAG_30			1073741824
#define	FLAG_31			2147483648

// FLAG Manipulation ( using the FLAG definition, see below )
#define nlFlagToggle(val,flag)		((val) ^= (flag))
#define nlFlagOn(val,flag)			((val) |= (flag))
#define ntFlagOff(val,flag)			((val) &= ~(flag))
#define nlIsFlagOn(val,flag)		((val) & (flag))			// !! ZERO or NON ZERO value
#define nlIsFlagOff(val,flag)		(!((val) & (flag)))			// !! ZERO or NON ZERO value
#define nlTestFlag(val,bmask,flags)	(((val)&(bmask))==(flags))	// false or true
