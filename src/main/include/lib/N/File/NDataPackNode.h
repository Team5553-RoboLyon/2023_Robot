#ifndef __NDATAPACKNODE_H
#define __NDATAPACKNODE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NDataPackNode.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "../Containers/NArray.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------
	typedef struct NDATAPACKNODE NDATAPACKNODE;
	struct NDATAPACKNODE
	{
		Nu32 Config;

		void *pAdress;	   // could be: Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
		Nu32 ObjectsCount; // if IsWhat =  NSRLCFG_MEMBLOC this member represent the number of objects "storable" in this membloc if  IsWhat =  NSRLCFG_ARRAY , this member is equal to Array.Size.
	};

	typedef struct NDATAPACKNODEHEADER NDATAPACKNODEHEADER;
	struct NDATAPACKNODEHEADER
	{
		Nu32 Config;
		Nu32 ObjectsCount;
	};

// ------------------------------------------------------------------------------------
// DATAPACKNODE CONFIGURATIONS:
// ------------------------------------------------------------------------------------
// Configuration is the way to describe DATAPACKER behavior with a specific DATAPACK
//											|			|						|			|Array/Mbloc| Obj.Malloc|
//											|			|		 Is What		|	  Of	|by User	| by User	|
//											|			|						|	 PTRs	|			|			|
// -----------------------------------------+-----------+-----------+-----------+-----------+-----------+-----------+
//											|	  5 	|	  4 	|	  3 	|	BIT_2	|	BIT_1 	|	BIT_0 	|
// -----------------------------------------+-----------+-----------+-----------+-----------+-----------+-----------+
//											|	 		|						|	NTRUE	|	NTRUE	|	NTRUE	|
//											|	  u		|	NDPCFG_VOID			|	  or	|	  or	|	  or	|
//											|	  n		|		  or			|	NFALSE	|	NFALSE	|	NFALSE	|
//											|	  u		|	NDPCFG_LONELY_PTR	|			|			|			|
//											|	  s		|		  or			|True:		|True:		|True:		|
//											|	  e		|	NDPCFG_ARRAY		|	of Ptr	|	by user	|	by user	|
//											|	  d		|		  or			|			|			|			|
//											|			|	NDPCFG_MEMBLOC		|False:		|False:		|False:		|
//											|			|						|	Of Obj.	|	by DC	|	by DC	|

// ---------------------------------------------------
#define NDPCFG_VOID 0
#define NDPCFG_LONELY_PTR 1
#define NDPCFG_ARRAY 2
#define NDPCFG_MEMBLOC 3
// ---------------------------------------------------
#define NMAKE_DPCFG(isWhat, ofPtrs, arrayByUser, objByUser) (Nu32)((((isWhat)&0x3) << 4) | (((ofPtrs)&0x1) << 2) | (((arrayByUser)&0x1) << 1) | ((objByUser)&0x1))

#define NGET_DPCFG_WHAT(cfg) (((cfg) >> 4) & 0x3)
#define NIS_DPCFG_LONELY_PTR(cfg) (NGET_DPCFG_WHAT(cfg) == NDPCFG_LONELY_PTR)
#define NIS_DPCFG_ARRAY(cfg) (NGET_DPCFG_WHAT(cfg) == NDPCFG_ARRAY)
#define NIS_DPCFG_MEMBLOC(cfg) (NGET_DPCFG_WHAT(cfg) == NDPCFG_MEMBLOC)
#define NIS_DPCFG_ARRAY_OR_MEMBLOC(cfg) (NGET_DPCFG_WHAT(cfg) >= NDPCFG_ARRAY)

#define NIS_DPCFG_OBJ_ALLOC_BY_USER(cfg) ISFLAG_ON(cfg, BIT_0)
#define NIS_DPCFG_OBJ_ALLOC_BY_DP(cfg) ISFLAG_OFF(cfg, BIT_0)

#define NIS_DPCFG_ARRAY_ALLOC_BY_USER(cfg) ISFLAG_ON(cfg, BIT_1) // |
#define NIS_DPCFG_ARRAY_ALLOC_BY_DP(cfg) ISFLAG_OFF(cfg, BIT_1)	 // |_ MACROS for ARRAY and MEMBLOC are exactly the same !
#define NIS_DPCFG_MBLOC_ALLOC_BY_USER(cfg) ISFLAG_ON(cfg, BIT_1) // |  ... of Course !
#define NIS_DPCFG_MBLOC_ALLOC_BY_DP(cfg) ISFLAG_OFF(cfg, BIT_1)	 // |

#define NIS_DPCFG_OF_PTRS(cfg) ISFLAG_ON(cfg, BIT_2)
#define NIS_DPCFG_OF_OBJECTS(cfg) ISFLAG_OFF(cfg, BIT_2)

// 0) A Lonely Pointer
#define USR_OBJ NMAKE_DPCFG(NDPCFG_LONELY_PTR, NFALSE, NFALSE, NTRUE) // 0.1)
#define DP_OBJ NMAKE_DPCFG(NDPCFG_LONELY_PTR, NFALSE, NFALSE, NFALSE) // 0.2)
// 1) Array Of ...
// Array of Objects
#define USR_ARRAY_USR_OBJ NMAKE_DPCFG(NDPCFG_ARRAY, NFALSE, NTRUE, NTRUE) // 1.1)
#define DP_ARRAY_DP_OBJ NMAKE_DPCFG(NDPCFG_ARRAY, NFALSE, NFALSE, NFALSE) // 1.2)
#define USR_ARRAY_DP_OBJ NMAKE_DPCFG(NDPCFG_ARRAY, NFALSE, NTRUE, NFALSE) // 1.3)
// Array of Pointers
#define USR_ARRAY_USR_PTR NMAKE_DPCFG(NDPCFG_ARRAY, NTRUE, NTRUE, NTRUE)					  // 1.4)
#define SERIAL_ALLOC_ARRAY_AND_PTR NMAKE_DPCFG(NDPCFG_ARRAY, NTRUE, NFALSE, NFALSE)			  // 1.5)
#define USER_PREALLOC_ARRAY__SERIAL_ALLOC_PTR NMAKE_DPCFG(NDPCFG_ARRAY, NTRUE, NTRUE, NFALSE) // 1.6)
// 2) MemBloc of ...
// Membloc of Objects
#define USER_PREALLOC_MBLOC_AND_OBJ NMAKE_DPCFG(NDPCFG_MEMBLOC, NFALSE, NTRUE, NTRUE)			 // 2.1)
#define SERIAL_ALLOC_MBLOC_AND_OBJ NMAKE_DPCFG(NDPCFG_MEMBLOC, NFALSE, NFALSE, NFALSE)			 // 2.2)
#define USER_PREALLOC_MBLOC__SERIAL_ALLOC_OBJ NMAKE_DPCFG(NDPCFG_MEMBLOC, NFALSE, NTRUE, NFALSE) // 2.3)

// Membloc of Pointers
#define USER_PREALLOC_MBLOC_AND_PTR NMAKE_DPCFG(NDPCFG_MEMBLOC, NTRUE, NTRUE, NTRUE)			// 2.4)
#define SERIAL_ALLOC_MBLOC_AND_PTR NMAKE_DPCFG(NDPCFG_MEMBLOC, NTRUE, NFALSE, NFALSE)			// 2.5)
#define USER_PREALLOC_MBLOC__SERIAL_ALLOC_PTR NMAKE_DPCFG(NDPCFG_MEMBLOC, NTRUE, NTRUE, NFALSE) // 2.6)

	NDATAPACKNODE *NSetupDataPackNode(NDATAPACKNODE *pdpn, const Nu32 cfg, const void *padress, const Nu32 membloc_object_count);
	NDATAPACKNODE *NCreateDataPackNode(const Nu32 cfg, const void *padress, const Nu32 membloc_object_count);
	void NClearDataPackNode(NDATAPACKNODE *pdpn);
	void NDeleteDataPackNode(NDATAPACKNODE *pdpn);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NDATAPACKNODE_H
