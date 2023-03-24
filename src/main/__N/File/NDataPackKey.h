#ifndef __NDPKEY_H
#define __NDPKEY_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NDataPackKey.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NDPKEY_RESERVED0		0		// The first valid key used by system ! 
#define NDPKEY_RESERVED1		1
#define NDPKEY_RESERVED2		2
#define NDPKEY_RESERVED3		3
#define NDPKEY_RESERVED4		4
#define NDPKEY_RESERVED5		5
#define NDPKEY_RESERVED6		6
#define NDPKEY_RESERVED7		7
#define NDPKEY_RESERVED8		8
#define NDPKEY_RESERVED9		9
#define NDPKEY_RESERVED10		10
#define NDPKEY_RESERVED11		11
#define NDPKEY_RESERVED12		12
#define NDPKEY_RESERVED13		13
#define NDPKEY_RESERVED14		14
#define NDPKEY_RESERVED15		15
#define NDPKEY_RESERVED16		16
#define NDPKEY_SYSTEM_COUNT		17 // --> Number of different DPKEY ( Data Pack reserved and used by N.

#define NDPKEY_INVALID			NVOID
#define NDPKEY_USER				NDPKEY_SYSTEM_COUNT	// --> the First Key index available for user 

#define NDPKEY_WATCHDOG			128						// --> Pure arbitrary value used in DEBUG only to check 'NInsertPack'
															// --> this value represents the "max" supposed value of the 'key' param ...
															// --> Of course is just an assumption. Actually is possible for a user to define more than ...
															// --> ... (NDPKEY_WATCHDOG - NDPKEY_SYSTEM_COUNT ) NDPKEYs ! But ... 
															// --> ... in case of WATCHDOG value overload, N prefer to warn user for a potential problem ( something like key = NVOID or some other mystake )
															// --> ... If it's not an error, ... increase this Watchdog value !
// All next user NDPKEY must be defined like this
//		#define NDPKEY_xxx				NDPKEY_USER + k		with   0 <= k < NU32_MAX-1
//
// So, according with these rules, the first NDPKEY defined by user will be : 
//		#define NDPKEY_xxx				NDPKEY_USER + 0		

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NDPKEY_H

