#ifndef __GAMESTATEUID_H
#define __GAMESTATEUID_H

// IMPORTANT:
// THIS ENUM has to be defined for each game.
// Sure, it's "inside" the Engine, but by this way we avoid using GameStateCreating and deleting function.
// All the  states we need will be created at the beginning of the game by using the 'STATE_ENUM_SIZE' member.
//

// for SNEAKLE
typedef enum 
{
#ifdef _NEDITOR
//	STATE_ONLY_IF_NEDITOR,			// Only in EDITING MODE
#endif
	STATE_MAIN,					// 
	STATE_QUIT,					// When User quit the game. It does nothing, but thanks to it, there is no more 'real' STATE active
//  -------------------
	NSTATEUID_NULL,				// The NSTATE enum need to have this 'NULL' game state defined, just for use it with "NSetGameStateParent" to set State.pParent to NULL.
	NSTATEUID_ENUM_SIZE			// The NSTATE enum need to finish by this specific value: STATE_ENUM_SIZE. 
								// It's VERY IMPORTANT. It is used to allocate the right amount of memory.
}NSTATEUID_ENUM;

#endif // __GAMESTATEUID_H