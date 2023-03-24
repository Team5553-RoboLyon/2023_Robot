#ifndef __NPOSTUPDATEUID_H
#define __NPOSTUPDATEUID_H

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NPostUpdateUID.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// IMPORTANT:
// THIS ENUM has to be defined for each game.
// Sure, it's "inside" the Engine, but by this way we avoid using PostUpdate creating and deleting functions.
// All the  PostUpdates we need will be created at the beginning of the game by using the 'NPOSTUPDATE_ENUM_SIZE' member.
// !!! Changing Enum elements Order will change post-update calling  order !!!
// for SNAKE
typedef enum 
{
	NPOSTUPDATE_POOL_COLLECTIBLES,
	NPOSTUPDATE_POOL_STRUCTURES,
	//NPOSTUPDATE_COLLECTIBLES_PICKING,
	//	..................................................................................................
	NPOSTUPDATE_ENUM_SIZE		// The NSTATE enum need to finish by this specific value: STATE_ENUM_SIZE. 
								// It's VERY IMPORTANT to allocate the right amount of memory
}NPOSTUPDATEUID;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NPOSTUPDATEUID_H