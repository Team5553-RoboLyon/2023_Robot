#include "lib/N/NCStandard.h"
#include "../NCoordinateSystemConversions.h"
#include "../Containers/NArray.h"
#include "../UI/NUI.h"
#include "../UI/NUIClearWatchStack.h"
#include "../GameLoop/NGameStates.h"
#include "../Containers/NRingBuffer.h"
#include "../NCore.h"
#include "lib/N/NErrorHandling.h"
#include "NTouchStream.h"
#include "NTouch.h"

extern Nu32 NEngineCoreFlags;
extern NGAMESTATEMACHINE NGameStateMachine;
// extern NRINGBUFFER			NTouchStream;
extern NARRAY ActiveUIRootsList;
extern NNODELIST UIListener_List;
extern NNODELIST AvailableUIListener_List;
extern Nu8 NTouchUICompatibility;

#ifdef _NOUT_SYS_EVENT
extern Nu32 debug_id;
Nu32 recursive_count;
#endif

inline void start_newline(Nu32 nb)
{
	printf("\n");
	for (Nu32 i = nb; i != 0; i--)
	{
		printf("\t");
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +
// +						STATIC INLINE FUNCTIONS for GAME STATE
// +						Used Only in that file.
// +
// +
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ------------------------------------------------------------------------------------------
// static inline Nu8 _gamestate_specific__NCleanTouchListenerUp
// ------------------------------------------------------------------------------------------
// Description :
//	There is no major differences between specific version (not written here) and the
//  generic function 'NCleanTouchListenerUp'
//	So, this 'gamestate specific version' is just a "define" not really a specific function.
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
#define _gamestate_specific__NCleanTouchListenerUp(pstate) NCleanTouchListenerUp(&(pstate)->Listener)

// ------------------------------------------------------------------------------------------
// static inline Nu32  _gamestate_specific__NListenTouchCancelSequence
// ------------------------------------------------------------------------------------------
// Description :
//		A specific version of the function 'NListenTouchCancelSequence'.
//		This version is different than the original but very close to the "NListenTouchEndSequence" function.
//		The NListenTouchCancelSequence function manage CANCEL and CANCEL ALL but not this specific function !
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _gamestate_specific__NListenTouchCancelSequence(NGAMESTATE *pgamestate, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!pgamestate || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(pgamestate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);												 // Source sequence must have, at least one Touch ! This listen CANCEL function doesn't manage CANCEL ALL !!!
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);											 // listened sequence must be empty !
	NErrorIf(pgamestate->Listener.LTouchCount != pgamestate->Listener.LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!pgamestate->Listener.LTouchCount)
		return 0;

	Nu32 streamed_systemid;
	NLISTENED_TOUCH *plistened;
	Nu8 s, l;
	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase CANCEL !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_CANCEL, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
		plistened = pgamestate->Listener.pLTouchBuffer;
		for (l = pgamestate->Listener.LTouchBufferSize; l != 0; l--, plistened++)
		{
			// Match !
			if (streamed_systemid == plistened->SystemID)
			{
				// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
				NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_TOUCH_INCONSISTENT_PHASE);

				// Build up the Listened Touch Sequence
				plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
				plistened_touchsequence->TouchCount++;

				// Update Listened Touch Data into the Listener Buffer !
				plistened->Phase = NTOUCH_PHASE_CANCEL;
				//		plistened->SystemID		= not updated;
				//		plistened->Flags		= not updated;
				//		plistened->User_Nu16	= not updated;
				break;
			}
		}

		// Are all the current Listened Touch in Buffer updated ?
		if (plistened_touchsequence->TouchCount == pgamestate->Listener.LTouchCount)
			break;
	}

	/*
		... Done just after the function call , to save the test here !
		if(plistened_touchsequence->TouchCount)
		{
			NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
			pgamestate->Listener.PreviousLTouchCount	= pgamestate->Listener.LTouchCount;
			pgamestate->Listener.LTouchCount		   -= plistened_touchsequence->TouchCount;

			// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
			FLAG_ON(pgamestate->Listener.Flags,FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			// Return the exact number of listened touch(es) updated in the buffer
			return plistened_touchsequence->TouchCount;
		}
		else
		{
			return 0;
		}
	*/
	return plistened_touchsequence->TouchCount;
}
// ------------------------------------------------------------------------------------------
// static inline Nu32  _gamestate_specific__NListenTouchEndSequence
// ------------------------------------------------------------------------------------------
// Description :
//		A specific version of the function 'NListenTouchEndSequence'.
//		This version is very close to the original one, but it exists because it's a critical point
//		in the engine with several calls at each engine turn ! So some tiny optimizations are made ...
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _gamestate_specific__NListenTouchEndSequence(NGAMESTATE *pgamestate, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!pgamestate || !plistened_touchsequence || !psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(pgamestate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);												 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);											 // listened sequence must be empty !
	NErrorIf(pgamestate->Listener.LTouchCount != pgamestate->Listener.LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!pgamestate->Listener.LTouchCount)
		return 0;

	Nu32 streamed_systemid;
	NLISTENED_TOUCH *plistened;
	Nu8 s, l;
	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase END !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_END, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
		plistened = pgamestate->Listener.pLTouchBuffer;
		for (l = pgamestate->Listener.LTouchBufferSize; l != 0; l--, plistened++)
		{
			// Match !
			if (streamed_systemid == plistened->SystemID)
			{
				// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
				NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_TOUCH_INCONSISTENT_PHASE);

				// Build up the Listened Touch Sequence
				plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
				plistened_touchsequence->TouchCount++;

				// Update Listened Touch Data into the Listener Buffer !
				plistened->Phase = NTOUCH_PHASE_END;
				//		plistened->SystemID		= not updated;
				//		plistened->Flags		= not updated;
				//		plistened->User_Nu16	= not updated;
				break;
			}
		}

		// Are all the current Listened Touch in Buffer updated ?
		if (plistened_touchsequence->TouchCount == pgamestate->Listener.LTouchCount)
			break;
	}

	/*
		... Done just after the function call , to save the test here !
		if(plistened_touchsequence->TouchCount)
		{
			NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
			pgamestate->Listener.PreviousLTouchCount	= pgamestate->Listener.LTouchCount;
			pgamestate->Listener.LTouchCount		   -= plistened_touchsequence->TouchCount;

			// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
			FLAG_ON(pgamestate->Listener.Flags,FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			// Return the exact number of listened touch(es) updated in the buffer
			return plistened_touchsequence->TouchCount;
		}
		else
		{
			return 0;
		}
	*/
	return plistened_touchsequence->TouchCount;
}

// ------------------------------------------------------------------------------------------
// static inline Nu8 _gamestate_specific__NListenTouchMoveSequence
// ------------------------------------------------------------------------------------------
// Description :
//	There is no major differences between specific version (in comments) and the
//  generic function 'NListenTouchMoveSequence'
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _gamestate_specific__NListenTouchMoveSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *pstart_touchsequence, NTOUCH_SEQUENCE *pmove_touchsequence, NTOUCH_SEQUENCE *pcanceled_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || !psource_touchsequence || !pstart_touchsequence || !pmove_touchsequence || !pcanceled_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);	  // Source sequence must have, at least one Touch !
	NErrorIf(pstart_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);	  // listened sequence must be empty !
	NErrorIf(pmove_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);	  // listened sequence must be empty !
	NErrorIf(pcanceled_touchsequence->TouchCount, NERROR_NON_NULL_VALUE); // canceled sequence must be empty !

	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount && ISFLAG_OFF(plistener->Flags, FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI))
		return;

	Nu32 streamed_systemid;
	NLISTENED_TOUCH *plistened;
	Nu8 s, l;
	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	Nu8 touchbuffersize = plistener->LTouchBufferSize;
	NErrorIf(ptindex != (Nu8 *)&psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase MOVE !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_MOVE, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
		plistened = plistener->pLTouchBuffer;
		for (l = touchbuffersize; l != 0; l--, plistened++)
		{
			// Match !
			if (streamed_systemid == plistened->SystemID)
			{
				// Does UI ( an UI that capture the touch on the fly ) Block the Touch for Game state ?
				if (ISFLAG_ON(_STREAMTOUCH(*ptindex).Flags, FLAG_NTOUCH_LOCK_GAMESTATE))
				{
					// Build up the canceled Touch Sequence
					pcanceled_touchsequence->TouchIndex[pcanceled_touchsequence->TouchCount] = *ptindex;
					pcanceled_touchsequence->TouchCount++;

					// Update Listened Touch Data into the Listener Buffer !
					plistened->Phase = NTOUCH_PHASE_CANCEL;
					//		plistened->SystemID		= not updated;
					//		plistened->Flags		= not updated;
					//		plistened->User_Nu16	= not updated;
				}
				else
				{
					// Build up the Listened Touch Sequence
					pmove_touchsequence->TouchIndex[pmove_touchsequence->TouchCount] = *ptindex;
					pmove_touchsequence->TouchCount++;

					// Update Listened Touch Data into the Listener Buffer !
					plistened->Phase = NTOUCH_PHASE_MOVE;
					//		plistened->SystemID		= not updated;
					//		plistened->Flags		= not updated;
					//		plistened->User_Nu16	= not updated;
				}
				break;
			}
			// incoming moving touch is not captured yet ... but ... if gamestate listener is able to capture at MOVE
			else if (ISFLAG_ON(plistener->Flags, FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI) && ISFLAG_OFF(_STREAMTOUCH(*ptindex).Flags, FLAG_NTOUCH_LOCK_GAMESTATE) && plistener->LTouchBufferSize < plistener->LTouchCountMax)
			{
				// Store the touch Index into the UITouchSequence
				pstart_touchsequence->TouchIndex[pstart_touchsequence->TouchCount] = *ptindex;
				pstart_touchsequence->TouchCount++;

				// Store the new Listened Touch Data into the Listener Buffer !
				plistener->pLTouchBuffer[plistener->LTouchBufferSize].SystemID = _STREAMTOUCH(*ptindex).SystemID;
				plistener->pLTouchBuffer[plistener->LTouchBufferSize].Phase = NTOUCH_PHASE_START;
				plistener->pLTouchBuffer[plistener->LTouchBufferSize].Flags = 0;
				plistener->pLTouchBuffer[plistener->LTouchBufferSize].User_Nu16 = 0;
				plistener->LTouchBufferSize++;
			}
		}
	}
}
/*
static inline Nu32 _gamestate_specific__NListenTouchMoveSequence(NGAMESTATE *pgamestate, NTOUCH_SEQUENCE *pout_listened_touchseq, const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	// Streamed
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER );														// It has to be Defined !
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE );											// It has to be Defined !
	// Listened
	NErrorIf(!pout_listened_touchseq, NERROR_INCONSISTENT_VALUES );												// It has to be Defined !
	NErrorIf(pout_listened_touchseq->TouchCount, NERROR_NON_NULL_VALUE);										// It has to be NULL !
	// GameState
	NErrorIf(!pgamestate, NERROR_NULL_POINTER);																	// It has to be Defined !
	NErrorIf(pgamestate->Listener.LTouchCount != pgamestate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);	// They have to be Equal !

	if(!pgamestate->Listener.LTouchCount)
		return 0;

	Nu32			i;
	Nu32			streamed_systemid;
	NLISTENED_TOUCH	*plistened;
	Nu8				*pt = (Nu8*)pin_streamed_touchseq->TouchIndex;
	for( i=pin_streamed_touchseq->TouchCount;i!=0;i--,pt++ )
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase MOVE !
		NErrorIf(_STREAMTOUCH(*pt).Phase != NTOUCH_PHASE_MOVE, NERROR_TOUCH_INCONSISTENT_PHASE );
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid	= _STREAMTOUCH(*pt).SystemID;
		plistened = pgamestate->Listener.pLTouchBuffer
		for( Nu8 j=pgamestate->Listener.LTouchBufferSize;j!=0;j--, plistened ++ )
		{
			// Match !
			if( streamed_systemid == plistened->SystemID )
			{
				// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
				NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_SYSTEM_CHECK);

				// Build up the Listened Touch Sequence
				pout_listened_touchseq->TouchIndex[pout_listened_touchseq->TouchCount] = *pt;
				pout_listened_touchseq->TouchCount ++;

				// Update Listened Touch Data into the Listener Buffer !
				plistened->Phase		= NTOUCH_PHASE_MOVE;
				//		plistened->SystemID		= not updated;
				//		plistened->Flags		= not updated;
				//		plistened->User_Nu16	= not updated;

				// not necessary to manage TRACK MOVE IN OUT flags. They mean something only for UI ...
				// FLAG_OFF( pstreamed->Flags, MASK_NTOUCH_TRACK_MOVE_IN_OUT );

				break;
			}
		}

		// Are all the current Listened Touch in Buffer updated ?
		if( pout_listened_touchseq->TouchCount == pgamestate->Listener.LTouchCount )
			break;

	}
	return pout_listened_touchseq->TouchCount;
}
*/

// ------------------------------------------------------------------------------------------
// static inline Nu32 _gamestate_specific__NListenTouchStartSequenceMsk
// ------------------------------------------------------------------------------------------
// Description :
//		A specific version of the function 'NListenTouchStartSequenceMsk'.
//		This version is very close to the original one, but it exists because it's a critical point
//		in the engine with several calls at each engine turn ! So some tiny optimizations are made ...
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _gamestate_specific__NListenTouchStartSequenceMsk(NGAMESTATE *pgamestate, NTOUCH_SEQUENCE *pout_listened_touchseq, const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	// Streamed
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);			 // It has to be Defined !
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE); // It has to be Defined !
	// Listened
	NErrorIf(!pout_listened_touchseq, NERROR_INCONSISTENT_VALUES);		 // It has to be Defined !
	NErrorIf(pout_listened_touchseq->TouchCount, NERROR_NON_NULL_VALUE); // It has to be NULL !
	// GameState
	NErrorIf(!pgamestate, NERROR_NULL_POINTER);																  // It has to be Defined !
	NErrorIf(pgamestate->Listener.LTouchCount != pgamestate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK); // They have to be Equal !

	// Maximum Touch number already reached ...
	if (pgamestate->Listener.LTouchCount == pgamestate->Listener.LTouchCountMax)
		return 0;

	Nu8 *pt = (Nu8 *)pin_streamed_touchseq->TouchIndex;
	for (Nu8 i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
	{
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(*pt).Phase != NTOUCH_PHASE_START, NERROR_INCONSISTENT_VALUES);
		// + In debug Mode, check if incoming Touch is not already registered !!
		NLISTENED_TOUCH *plt = pgamestate->Listener.pLTouchBuffer;
		for (Nu8 i_dbg = pgamestate->Listener.LTouchBufferSize; i_dbg != 0; i_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(*pt).SystemID, NERROR_SYSTEM_CHECK);
		}
#endif
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// Does UI Block the Touch for Game state ?
		if (ISFLAG_OFF(_STREAMTOUCH(*pt).Flags, FLAG_NTOUCH_LOCK_GAMESTATE))
		{
			// Store the touch Index into the UITouchSequence
			pout_listened_touchseq->TouchIndex[pout_listened_touchseq->TouchCount] = *pt;
			pout_listened_touchseq->TouchCount++;

			// Store the new Listened Touch Data into the Listener Buffer !
			pgamestate->Listener.pLTouchBuffer[pgamestate->Listener.LTouchBufferSize].SystemID = _STREAMTOUCH(*pt).SystemID;
			pgamestate->Listener.pLTouchBuffer[pgamestate->Listener.LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			pgamestate->Listener.pLTouchBuffer[pgamestate->Listener.LTouchBufferSize].Flags = 0;
			pgamestate->Listener.pLTouchBuffer[pgamestate->Listener.LTouchBufferSize].User_Nu16 = 0;

			if (pgamestate->Listener.LTouchCountMax == pgamestate->Listener.LTouchBufferSize++)
				break;
		}
	}

	/*
		It's done outside to save this test !!!
		if(pgamestate->Listener.LTouchCount != pgamestate->Listener.LTouchBufferSize)
		{
			pgamestate->Listener.PreviousLTouchCount	= pgamestate->Listener.LTouchCount;
			pgamestate->Listener.LTouchCount			= pgamestate->Listener.LTouchBufferSize;
		}
	*/

	return pout_listened_touchseq->TouchCount;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +
// +						STATIC INLINE FUNCTIONS for UI
// +						Used Only in that file.
// +
// +
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ------------------------------------------------------------------------------------------
// static inline void _ui_listen_touch_start_sequence
// ------------------------------------------------------------------------------------------
// Description :
//	Add incoming streamed touch sequence into a listener. The incoming NTOUCH(s) have been already tested for "capture"
//	and all of them were validated to become effective Listened touch !!!
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ui_listen_touch_start_sequence(NUI *pui, const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	// Streamed
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);			 // It has to be Defined !
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE); // It has to be Defined !

	NTOUCH_UI_LISTENER *puilistener;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + A/ CREATE ( and BIND with UI ) or GET ( the already bound with UI ) UI LISTENER:
	// +

	// Use already UI linked Listener
	if (pui->pTouchUIListener)
	{
		puilistener = pui->pTouchUIListener;
	}
	// ... or get a NEW Listener
	else
	{
		puilistener = NBindTouchUIListener(pui);
	}
	// UI Listener CHECK
	NErrorIf(!puilistener, NERROR_NULL_POINTER);			// It has to be Defined ! |_These 2 tests seems useless but anyway ... !
	NErrorIf(puilistener->pUI != pui, NERROR_SYSTEM_CHECK); // ... And well defined ! |

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + B/ ADD BY HAND NEW Incoming Touch ID into the LISTENER BUFFER
// +
#ifdef _DEBUG
	NErrorIf(puilistener->Listener.LTouchCount != puilistener->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK); // They have to be Equal !
	NLISTENED_TOUCH *plt;
	for (Nu32 i_dbg = 0; i_dbg < pin_streamed_touchseq->TouchCount; i_dbg++)
	{
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(pin_streamed_touchseq->TouchIndex[i_dbg]).Phase != NTOUCH_PHASE_START, NERROR_INCONSISTENT_VALUES);
		// + In debug mode, Check that none of the new incoming touch is already inserted into the Listener Buffer
		plt = puilistener->Listener.pLTouchBuffer;
		for (Nu8 j_dbg = puilistener->Listener.LTouchBufferSize; j_dbg != 0; j_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(pin_streamed_touchseq->TouchIndex[i_dbg]).SystemID, NERROR_SYSTEM_CHECK);
		}
	}
#endif

	// At Start we don't care about "FLAG_NUI_UNDER_TOUCH_CONTROL" to update "PreviousTouchCountOut" and "TouchCountOut"
	// These 2 members still available even if 'pui' is not Under Touch Control, BUT in that case,
	//			'PreviousTouchCountOut' and 'TouchCountOut' still always equal to  0
	//
	// In other words, Touch moves going to change these values ONLY if 'pui' IS Under Touch Control.
	//
	Nu32 i;
	Nu8 *pt = (Nu8 *)pin_streamed_touchseq->TouchIndex;
	if (ISFLAG_ON(pui->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES) /* && ISFLAG_OFF(_STREAMTOUCH(*pt).Flags,FLAG_NTOUCH_LOCK_GAMESTATE)*/) // we are going to flag it on several times but it's not a problem
	{
		for (i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
		{
			FLAG_ON(_STREAMTOUCH(*pt).Flags, FLAG_NTOUCH_LOCK_GAMESTATE);
			// Insert Listened Touch by Hand to Listener
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].SystemID = _STREAMTOUCH(*pt).SystemID;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].User_Nu16 = 0;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Flags = FLAG_NTOUCH_IS_IN;

			// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
			_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | FLAG_NTOUCH_IS_IN;
			// Notice that 'FLAG_NTOUCH_IS_IN' is set to ON without any test about UNDER TOUCH CONTROL ... It's normal !!!
			// Because with or without UNDER TOUCH Control, counters TouchCountIn is updated at start / end.
			// UNDER TOUCH Control update it too at MOVE ...
			// Some safety check
			NErrorIf(puilistener->Listener.LTouchBufferSize == puilistener->Listener.LTouchBufferCapacity, NERROR_SYSTEM_CHECK);
			puilistener->Listener.LTouchBufferSize++;
		}
	}
	else
	{
		for (i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
		{
			// Insert Listened Touch by Hand to Listener
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].SystemID = _STREAMTOUCH(*pt).SystemID;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].User_Nu16 = 0;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Flags = FLAG_NTOUCH_IS_IN;

			// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
			_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | FLAG_NTOUCH_IS_IN;
			// Notice that 'FLAG_NTOUCH_IS_IN' is set to ON without any test about UNDER TOUCH CONTROL ... It's normal !!!
			// Because with or without UNDER TOUCH Control, counters TouchCountIn is updated at start / end.
			// UNDER TOUCH Control update it too at MOVE ...
			// Some safety check
			NErrorIf(puilistener->Listener.LTouchBufferSize == puilistener->Listener.LTouchBufferCapacity, NERROR_SYSTEM_CHECK);

			puilistener->Listener.LTouchBufferSize++;
		}
	}

	// UPDATE LISTERNER TOUCH COUNTS
	puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
	puilistener->Listener.LTouchCount = puilistener->Listener.LTouchBufferSize;
	puilistener->Listener.PreviousLTouchCountIn = puilistener->Listener.LTouchCountIn;
	puilistener->Listener.LTouchCountIn += pin_streamed_touchseq->TouchCount;

	NErrorIf(puilistener->Listener.LTouchCount > puilistener->Listener.LTouchCountMax, NERROR_SYSTEM_CHECK);
}

// ------------------------------------------------------------------------------------------
// static inline Nu32 _is_ui_accept_touch
// ------------------------------------------------------------------------------------------
// Description :
//	About 'NTouchUICompatibility'
//	if User wants all UI exclusive regarding each other ... he should set all 'pui->TouchCompatibility' to 255
//	if User wants some UI compatibles					... he should set compatible UI with the same "TouchCompatibility", different from 0 and 255
//	if User wants some UI exclusive regarding each other + UI compatible with all ... he should set exclusive 'pui->TouchCompatibility' with a unique value different than 0 and 255 and all the compatible with all with 0.
//	example:
//		UI#0->TouchCompatibility = 255		|
//		UI#1->TouchCompatibility = 255		|If user touch one of these 3 UIs all the others are going to refuse the touch
//		UI#2->TouchCompatibility = 255		|
//
//		UI#0->TouchCompatibility = 0		|If user touch UI#1 or UI#2, UI#2 or UI#1 is going to refuse the touch
//		UI#1->TouchCompatibility = 255		|If user touch UI#0, UI#1 or UI#2 is going to accept the touch
//		UI#2->TouchCompatibility = 255		|UI#0 will always accepts the touch with or without UI#1 or UI#2 touched.
//
//		UI#0->TouchCompatibility = 1		|If user touch UI#0, UI#1 is going to accept the touch, and UI#2 will refuse it !
//		UI#1->TouchCompatibility = 1		|If user touch UI#1, UI#0 is going to accept the touch, and UI#2 will refuse it !
//		UI#2->TouchCompatibility = 255		|If user touch UI#2, UI#0 and UI#1 will refuse the touch
//
//		UI#0->TouchCompatibility = 1		|If user touch UI#0, UI#1 is going to accept the touch. UI#2 and UI#3 will refuse it !
//		UI#1->TouchCompatibility = 1		|If user touch UI#1, UI#0 is going to accept the touch. UI#2 and UI#3 will refuse it !
//		UI#2->TouchCompatibility = 2		|If user touch UI#2, UI#3 is going to accept the touch. UI#0 and UI#1 will refuse it !
//		UI#3->TouchCompatibility = 2		|If user touch UI#3, UI#2 is going to accept the touch. UI#0 and UI#1 will refuse it !
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _is_ui_accept_touch(const NUI *pui)
{
	/*
		if(!FLAGS_TEST(pui->Flags,MASK_NUI_ACTIVE,MASK_NUI_ACTIVE)  )
		{
			return 0;
		}
	*/

	if (ISFLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN))
	{
		// Entirely on screen ?
		if (pui->BoundingBox.left < 0.0f || pui->BoundingBox.right > NViewPort.AspectRatio)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Is not fully on screen and Refuse Touch(es).");
#endif
			return 0;
		}
		if (pui->BoundingBox.top > 0.0f || pui->BoundingBox.bottom < -1.0f)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Is not fully on screen and Refuse Touch(es).");
#endif
			return 0;
		}
	}

	if (pui->pTouchUIListener && pui->pTouchUIListener->Listener.LTouchCount == pui->pTouchUIListener->Listener.LTouchCountMax)
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Reaches its MAX Touches count and Refuse new Touch(es).");
#endif
		return 0;
	}
	else if (pui->TouchCompatibility)
	{
		// Reminder: !!! pui->TouchCompatibility != 0 HERE !!! ( have a look some lines up from here ... )
		// A 'pui' with '255' as TouchCompatibility value was already registered. '255' means EXCLUSIVE ... but 0!!!
		if (NTouchUICompatibility == 255)
		{
			// ... Maybe this already registered 'ui' is ... 'pui' itself ( with some previous touch ! ) !!!
			// ... In that case, of course, ui continues to accept touch !
			if (pui->pTouchUIListener)
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Is Exclusive, already has touches captured and Accept new one(s).");
#endif
				return 1;
			}
			else
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Can not Accept Touch(es) Because an EXCLUSIVE UI already have captured some Touch(es).");
#endif
				return 0;
			}
		}
		// There are registered UI with a TouchCompatibility of "n" ( 0<n<255).
		// Reminder: !!! pui->TouchCompatibility != 0 HERE !!! ( have a look some lines up from here ... )
		// That means a Touch Compatibility Group is Active, and 'pui' needs to have the same ...
		//
		else if (NTouchUICompatibility == pui->TouchCompatibility)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Is compatible with the Active Compatibility Group [%d] and Accept Touch(es).", NTouchUICompatibility);
#endif
			return 1;
		}
		// ... So, from here, pui->TouchCompatibility and NTouchUICompatibility are different one from each other !!!
		// There still Only One possibility to check :
		//		 NTouchUICompatibility == 0
		// In that case, we can say, there are no UI registered yet or only UI with a TouchCompatibility of 0.
		// '0' means COMPATIBLE with ALL !
		// AND THAT'S HERE WHERE 'NTouchUICompatibility' get the value of 'pui->TouchCompatibility'
		else if (NTouchUICompatibility == 0)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Current Active Compatibility Group is [0]. That means compatible with ALL !");
#endif

			NTouchUICompatibility = pui->TouchCompatibility;

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Accept Touch(es) and Re-define the Active Compatibility Group to %d.", NTouchUICompatibility);
#endif
			return 1;
		}
		// NTouchUICompatibility != pui->TouchCompatibility AND NTouchUICompatibility != 0
		else
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Refuse Touch(es) Because is not compatible with the Active Compatibility Group.");
			start_newline(recursive_count);
			printf(" (UI:%d Active Compatibility Group:%d )", pui->TouchCompatibility, NTouchUICompatibility);
#endif
			return 0;
		}
	}
	// pui->TouchCompatibility == 0, that means pui is compatible with every one.
	else
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Accept Touch(es). It has a Compatibility Group of 0.");
#endif
		return 1;
	}
}

// ------------------------------------------------------------------------------------------
// static inline Nu32 _ui_dispatch_touch_start_recursive
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _ui_dispatch_touch_start_recursive(NUI *pui, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + PASS A: Pass trough UI that Doesn't listen START or MOVE IN
	// +
	//
	// Here, UI may be one of these 3 Types ( about touch event listening ):
	//		1) Doesn't listen START or MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is OFF
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is OFF
	//		2) Listen START Only:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is ON
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is OFF
	//		3) Listen START and MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is ON
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON
	//
	// In the specific case 1), UI must let EVENT pass trough it to feed children which capture touch start
	// or which are also in the same case 1).
	// Because the case where FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON alone can not be here ...
	// testing  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START is OFF is enough to be sure that we are in case 1) !
	// In debug mode, we are going to check this assumption !
	NErrorIf(FLAGS_TEST(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN), NERROR_SYSTEM_CHECK);

	if (ISFLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START))
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Doesn't Listen Touch Event ( Neither at Start nor at Move In )");
		start_newline(recursive_count);
		printf("+ It's like a ghost.");
#endif

		if (NIS_NODE_VALID(pui->ChildrenList.pFirst, &pui->ChildrenList))
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Dispatch All Touch(es) to its Children. Without any BBOX filter.");
#endif

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
			Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

			Nu32 child_capture_return = 0;
			Nu32 stacksize = 0;
			NUI *puichild = (NUI *)pui->ChildrenList.pFirst;
			while (NIS_NODE_VALID(puichild, &pui->ChildrenList))
			{
				if (NIsUIActive(puichild) && (!FLAGS_TEST(puichild->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)))
				{
					NUIClearWatchStackPush(puichild);
					stacksize++;
				}
				puichild = (NUI *)NGET_NEXT_NODE(puichild);
			}

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ %d/%d UI Children Pushed to NUIClearWatchStack.", stacksize, NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui));
			recursive_count++;
#endif

			// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
			// ( because it could be deleted during the process ... )
			// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
			// Coying the 'pui' listened touch sequence to dispatch it (the copy ) between 'pui' children.
			while (stacksize)
			{
				if ((puichild = NUIClearWatchStackPop()))
				{
// ------------------------------------
// - Debug OutPut
#ifdef _NOUT_SYS_EVENT

					start_newline(recursive_count);
					printf("+");
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
					recursive_count++;
					start_newline(recursive_count);
					printf("+");
					start_newline(recursive_count);
					printf("|");

#endif
					// ------------------------------------

					if (_ui_dispatch_touch_start_recursive(puichild, pin_streamed_touchseq))
					{
						child_capture_return = 1; // To remember, one child, at least, captured some touches and remove them from 'touch_sequence_in'

						if (!pin_streamed_touchseq->TouchCount)
						{
							NDecreaseUIClearWatchStackSize(stacksize - 1);
							break;
						}
					}

#ifdef _NOUT_SYS_EVENT
					recursive_count--;
#endif
				}

				stacksize--;
			}

			NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);

#ifdef _NOUT_SYS_EVENT
			recursive_count--;
#endif

			// Clear the 'Clear Watch Stack' range of UI children pointers
			// !!! if child_capture_return = 1 pin_streamed_touchseq was modified  It contains all the non captured Touches !!!
			return child_capture_return;
		}
		else
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Doesn't have Children.STOP.", pui);
#endif

			// !!! pin_streamed_touchseq still the same !!!
			return 0;
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + PASS B: Dispatch to UI that listen START
	// +
	// Here, UI may be one of the 2 Types ( about touch event listening ) 2) or 3) (see above for details )
	// That means UI is listening touch start !!!
	// In debug mode, we are going to check this assumption !
	NErrorIf(ISFLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START), NERROR_SYSTEM_CHECK);

	if (_is_ui_accept_touch(pui))
	{
		NUI_EVENT_RESULT uiresult;
		NEVENT nevent;
		NVEC2 hrel;
		Nu32 t;
		Nu8 uitouchcount;

		//		NTOUCH_SEQUENCE		streamed_in_uncaptured;
		Nu8 uncaptured_touchcount = 0;
		Nu8 *pt = pin_streamed_touchseq->TouchIndex;
		//		Nu8					out_count = pin_streamed_touchseq->TouchCount;
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + Setup incoming 'pui' specific Touch Sequence.
		// + That means, get each touch from the incoming Touch Sequence 'pin' and decide if 'pui' gets it or not.
		// +		All the touches index "captured" by 'pui' are inserted into a new Touch Sequence and at the same time,
		// +		removed from Touch Sequence "pin"
		// +

		// Set the extracted streamed touch sequence to ZERO ( setup TouchCount is enough ...)
		// ( This extracted touch sequence is the Event touch sequence itself )
		nevent.Touch.TouchSequence.TouchCount = 0;

		// Retrieve current number of Touch listen by 'pui'
		if (pui->pTouchUIListener)
			uitouchcount = pui->pTouchUIListener->Listener.LTouchCount;
		else
			uitouchcount = 0;

		for (t = pin_streamed_touchseq->TouchCount; t != 0; t--, pt++)
		{
			// UI get the Touch !
			if (uitouchcount < pui->TouchCountMax && pui->Pick_Proc(pui, NCoord_PixToHRel(&hrel, &_STREAMTOUCH(*pt).Position)))
			{
				// Store the touch Index into the Extracted Touch Sequence
				nevent.Touch.TouchSequence.TouchIndex[nevent.Touch.TouchSequence.TouchCount] = *pt;
				nevent.Touch.TouchSequence.TouchCount++;
				uitouchcount++;
			}
			// UI doesn't get the Touch !
			// Write the touch ID inside 'streamed_in_uncaptured'
			else
			{
				pin_streamed_touchseq->TouchIndex[uncaptured_touchcount] = *pt;
				uncaptured_touchcount++;
			}
		}
		// +
		// +
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + If 'pui' doesn't captured any NTOUCH ...
		// + ... return 0 !
		if (!nevent.Touch.TouchSequence.TouchCount)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Doesn't Capture any Touch Start.");
#endif

			// !!! pin_streamed_touchseq still the same !!!
			return 0;
		}
		// + ... else, continue deeper.
		else
		{
			// Update pin_streamed_touchseq Touch Count.
			pin_streamed_touchseq->TouchCount = uncaptured_touchcount;

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Captured %d Touch(es). %d Touch(es) still non captured.", nevent.Touch.TouchSequence.TouchCount, uncaptured_touchcount);
#endif

			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + IF UI Intercepts TOUCH Event
			// +	Register all the Touch as Listened Touches by UI.
			// +
			// + AND
			// +
			// +	Call the Event Handles.
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT))
			{
				_ui_listen_touch_start_sequence(pui, &nevent.Touch.TouchSequence);
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Register the Touch it captured.", pui);
#endif

				// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
				NFillupEventHead(nevent, NTOUCH_START, 0, NEVENT_RECIPIENT_CAST_UI, pui);

				NUIClearWatchStackPush(pui);

				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				// ------------------------------------
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				if (pui->pName)
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
				}
				else
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p].UIEventToState_Proc()", pui);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p].Event_Proc()-->", pui);
				}
#endif
				// ------------------------------------

				pui->UIEventToState_Proc(pui, &nevent);
				uiresult = pui->Event_Proc(pui, &nevent);

#ifdef _NOUT_SYS_EVENT
				if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
					printf(" Return NUI_RESULT_DISPATCH.");
				else
					printf(" Return NUI_RESULT_PROCESSED.");
				printf("\n");
#endif

				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				// "pui" was deleted during 'pui->Event_Proc' event processing ... or ...
				// "pui" stills exist but simply refuse to dispatch further to it's forward hierarchy.
				if (NUIClearWatchStackPop() != pui || (!NIS_UI_EVENT_RESULT_DISPATCH(uiresult)))
				{
#ifdef _NOUT_SYS_EVENT
					start_newline(recursive_count);
					printf("+ UI Dispatch interrupted.(UI doesn't returns DISPATCH or was deleted.)");
#endif

					// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!
					return 1;
				}
			}

			// FROM Here we are sure :
			//
			//					1/ 'pui' still exists !
			//
			//					2/ if 'pui' is a LISTENER 'uiresult' return by 'pui->Event_Proc(pui,&nevent);' is DISPATCH !!!
			//
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + Call 'NDispatchTouch_Start' for each 'pui' Children with "touch_sequence".
			// +	! Notes: if 'pui' was registered as a Touch Listener, N inserted 'pui' in the Listener chain
			// +			... thanks to the '_ui_listen_touch_start_sequence' function call.
			// +			... It's important to do it in that order, to preserve the right Event dispatch Order. Parent_Listener -> Children.
			// +
			Nu32 child_capture_return = 0;
			if (NIS_NODE_VALID(pui->ChildrenList.pFirst, &pui->ChildrenList))
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ UI Dispatch the touches it captured to its Children.");
#endif

				// Keep an eye on 'pui' itself ...
				NUIClearWatchStackPush(pui);

#ifdef _DEBUG
				Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

				Nu32 stacksize = 0;
				NUI *puichild = (NUI *)pui->ChildrenList.pFirst;
				while (NIS_NODE_VALID(puichild, &pui->ChildrenList))
				{
					if (NIsUIActive(puichild) && (!FLAGS_TEST(puichild->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)))
					{
						NUIClearWatchStackPush(puichild);
						stacksize++;
					}

					puichild = (NUI *)NGET_NEXT_NODE(puichild);
				}
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ %d/%d UI Children Pushed to NUIClearWatchStack.", stacksize, NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui));
				recursive_count++;
#endif

				// Dispatch to children !
				// Copying the 'pui' listened touch sequence to dispatch it (the copy ) between 'pui' children.
				NTOUCH_SEQUENCE touch_sequence_in = nevent.Touch.TouchSequence;
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
// ------------------------------------
// - Debug OutPut
#ifdef _NOUT_SYS_EVENT

						start_newline(recursive_count);
						printf("+");
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
						recursive_count++;
						start_newline(recursive_count);
						printf("+");
						start_newline(recursive_count);
						printf("|");

#endif
						// ------------------------------------

						if (_ui_dispatch_touch_start_recursive(puichild, &touch_sequence_in))
						{
							child_capture_return = 1; // To remember, one child, at least, captured some touches and remove them from 'touch_sequence_in'

							if (!touch_sequence_in.TouchCount)
							{
								NDecreaseUIClearWatchStackSize(stacksize - 1);
								break;
							}
						}
#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif;
					}
					stacksize--;
				}

				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif

				// Maybe 'pui' WAS DELETED during children dispatch processing ...
				if (NUIClearWatchStackPop() != pui)
				{
					// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!
					return 1;
				}
				// else 'pui' stills exist !!! So, just continue ...
			}
			// +
			// +
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + Register 'pui' as a listener when it's the "Touch Hierarchy Leaf" !
			// + ( it captured some touches but not it's children )
			// +
			// + Note:	if 'pui' is flagged as Intercept touch Event , there is nothing to do because everything was already done just before children dispatch try.
			// +		BUT
			// +		if 'pui' doesn't intercept touch event, it has to become a leaf listener.
			// +		Of course, thanks to all the previous filters ( flags tests) we are sure here that "pui" listen touch event
			// +		...especially we are sure FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START is set to ON.
			// +
			if (ISFLAG_OFF(pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT) && (!child_capture_return))
			{
				_ui_listen_touch_start_sequence(pui, &nevent.Touch.TouchSequence);
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ UI Register the Touch it captured as a Leaf Listener.");
#endif

				// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
				NFillupEventHead(nevent, NTOUCH_START, 0, NEVENT_RECIPIENT_CAST_UI, pui);

// No need to survey pui trough 'ClearWatchStack' for this last Event_Proc call !
// ------------------------------------
// - Debug OutPut
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				if (pui->pName)
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
				}
				else
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p].UIEventToState_Proc()", pui);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START to UI[0x%p].Event_Proc()-->", pui);
				}
#endif
				// ------------------------------------

				pui->UIEventToState_Proc(pui, &nevent);
				pui->Event_Proc(pui, &nevent);
				// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!

#ifdef _NOUT_SYS_EVENT
				printf(" Return ... Don't Care... UI is the terminated leaf !");
				printf("\n");
#endif
			}
			/*
						else
						{
							_ui_listen_touch_start_sequence(pui,&nevent.Touch.TouchSequence);
							#ifdef _DEBUG
							start_newline(recursive_count);
							printf("+ UI Register the Touch it captured as HIDDEN Listener.");
							start_newline(recursive_count);
							printf("  (No Event Sent.)");
							#endif

							FLAG_ON(pui->pTouchUIListener->Listener.Flags, FLAG_NTOUCH_UI_LISTENER_NTOUCH_LISTENER_HIDDEN);
						}
			*/

			return 1;
		}
	}
	// UI LISTEN TOUCH EVENT but doesn't accept these TOUCH ( because it reaches it's touch count Max or something else checked before testing UI BBOX ... )
	else
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ UI doesn't accept these touches.");
#endif

		// !!! pin_streamed_touchseq still the same !!!
		return 0;
	}
	// +
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ------------------------------------------------------------------------------------------
// static inline void _ui_dispatch_touch_cancel_all_recursive
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ui_dispatch_touch_cancel_all_recursive(NUI *pui)
{
	NErrorIf(!pui->pTouchUIListener, NERROR_SYSTEM_CHECK);

	NTOUCH_UI_LISTENER *puilistener = pui->pTouchUIListener;
	NUI_EVENT_RESULT uiresult = NUI_EVENT_RESULT_DISPATCH;

	if (puilistener->Listener.LTouchCount)
	{
		NEVENT nevent;

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + Setup incoming 'pui' specific Touch Sequence.
		// + That means, get each touch from the incoming Touch Sequence 'pin' and check if 'pui' manages it or not ( it manages it if it get it at the Touch start ).
		// +		Thanks to the function 'NUIExtractListenedTouch', all the touches index "managed" by 'pui' are inserted
		// +		into a new Touch Sequence and at the same time,	removed from Touch Sequence "pin".
		// +
		Nmem0(&nevent, NEVENT);
		// Note:

		// Remove all Listener Listened Touches !!! BUT KEEP Listened Touch available in Listener.ListenedTouchArray !!!
		puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
		puilistener->Listener.PreviousLTouchCountIn = puilistener->Listener.LTouchCountIn;
		puilistener->Listener.LTouchCount = 0;
		puilistener->Listener.LTouchCountIn = 0;
		FLAG_ON(puilistener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Building the Event Touch sequence
		NFillupEventHead(nevent, NTOUCH_CANCEL, NEVENT_PARAM_TOUCH_CANCEL__ALL, NEVENT_RECIPIENT_CAST_UI, pui);
		nevent.Touch.TouchSequence.TouchCount = 0; // Specific for "CANCEL ALL"

		// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
		NUIClearWatchStackPush(pui);

		// SEND EVENT
		// ------------------------------------------------------------------------------------------------
		pui->UIEventToState_Proc(pui, &nevent);
		uiresult = pui->Event_Proc(pui, &nevent);
		// ------------------------------------------------------------------------------------------------
		// SEND EVENT

		// "pui" was deleted during 'pui->Event_Proc' event processing ...
		// a Nested CANCEL_ALL is always possible ... even if we are into CANCEL ALL and even if Listener.LTouchCount is already equal to ZERO !
		// So, a nested call to Cancel All would unbind the puilistener and it will not still available here !
		if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
		{
			return;
		}
	}

	// CleanUp the UI Listener !
	NCleanTouchListenerUp(&puilistener->Listener);
	NUIClearWatchStackPush(pui);

	//  ... Continue Deeper if there are some children to dispatch to (obviously ! )
	if (NIS_NODE_VALID(puilistener->ChildrenList.pLast, &puilistener->ChildrenList))
	{
		// NTOUCH_CANCEL event need to be dispatched !!! There is no choice.
		// In Release mode, Engine is going to dispatch them automatically,
		// even if user doesn't returns NUI_EVENT_RESULT_DISPATCH, but in Debug Mode a CRASH will occur.
		NErrorIf(!NIS_UI_EVENT_RESULT_DISPATCH(uiresult), NERROR_EVENT_TOUCH_CANCEL_MUST_BE_DISPATCHED);

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
		Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

		Nu32 stacksize = 0;
		NTOUCH_UI_LISTENER *puilistenerchild = (NTOUCH_UI_LISTENER *)puilistener->ChildrenList.pLast;
		while (NIS_NODE_VALID(puilistenerchild, &puilistener->ChildrenList))
		{
			NErrorIf(!puilistenerchild->pUI, NERROR_SYSTEM_CHECK);
			NUIClearWatchStackPush(puilistenerchild->pUI);
			stacksize++;

			puilistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(puilistenerchild);
		}

		// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui'linked with listenerchild if it still exist.
		// ( because it could be deleted during the process ... )
		// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
		//
		// According with uiresult, send CANCEL EVENT
		NUI *puichild;
		while (stacksize)
		{
			if (puichild = NUIClearWatchStackPop())
			{
				_ui_dispatch_touch_cancel_all_recursive(puichild);
			}
			stacksize--;
		}

#ifdef _DEBUG
		NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#endif
	}

	// After Children management ( if plistener had children ) we can manage 'plistener' removal, it doesn't have any Listened Touch !!
	// But, first be sure 'pui' and 'pui->pTouchUIListener' still both exist. Because if not, plistener doesn't exist anymore !!!
	// ( 'pui->pTouchUIListener'  could be deleted by a nested call to cancel all ! )
	// ... and remember 'puilistener' IS 'pui->pTouchUIListener' so ...

	if (NUIClearWatchStackPop() == pui && pui->pTouchUIListener && (!puilistener->Listener.LTouchBufferSize))
	{
		NUnbindTouchUIListener(puilistener);
	}
}

// ------------------------------------------------------------------------------------------
// static inline Nu32 _ui_listen_touch_move_sequence
// ------------------------------------------------------------------------------------------
// Description :
//		Extract all plistener listened touches from a touch sequence.
//		the listened touches are removed from 'pin_' and inserted into 'pextracted_'
//		So, at process end...
//				'pextracted_'	will contains all the listened touches
//				'pin'			will be modified !!! And will contains all the NON-listened touches
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//		return the number of touches extracted from 'pin' and add into 'pextracted_'
//
// ------------------------------------------------------------------------------------------
static inline void _ui_listen_touch_move_sequence(NTOUCH_UI_LISTENER *plistener, NTOUCH_SEQUENCE *pextracted_streamed_touchseq, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!plistener || !pextracted_streamed_touchseq || !pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);		   // 'pin'	must contains at least one touch
	NErrorIf(pextracted_streamed_touchseq->TouchCount, NERROR_NON_NULL_VALUE); // 'pextracted_streamed_touchseq'	! must be empty !

	// Extract loop
	Nu32 k = 0;
	Nu8 tin = plistener->Listener.LTouchCountIn;
	NLISTENED_TOUCH *plistenedtouch;
	Nu8 *pt = pin_streamed_touchseq->TouchIndex;
	NTOUCH *pstreamtouch;
	NVEC2 hrel;

	for (Nu32 i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
	{
		// Incoming streamed touch is not MOVE ??!!??
		NErrorIf(_STREAMTOUCH(*pt).Phase != NTOUCH_PHASE_MOVE, NERROR_SYSTEM_CHECK);

		// UI manage the Touch
		pstreamtouch = &_STREAMTOUCH(*pt);
		if (plistenedtouch = NGetListenedTouch(&plistener->Listener, pstreamtouch->SystemID))
		{
			// Listened touch is INACTIVE ?!?
			NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistenedtouch->Phase), NERROR_SYSTEM_CHECK);

			// Update Listened Touch By Hand
			plistenedtouch->Phase = NTOUCH_PHASE_MOVE;

			// TRACK MOVE IN OUT control Update ?
			if (ISFLAG_ON(plistener->pUI->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT))
			{

				// UI DETECT TOUCH as a TOUCH IN !
				if (plistener->pUI->Pick_Proc(plistener->pUI, NCoord_PixToHRel(&hrel, &pstreamtouch->Position)))
				{
					if (ISFLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN)) // Is  OUT
					{
						FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was OUT
						FLAG_ON(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN);	 // Is  IN

						tin++;
					}
					else // Is IN
					{
						FLAG_ON(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was IN
																			// FLAG_ON(plistenedtouch->Flags,FLAG_NTOUCH_IS_IN);			// Is  IN	useless to SET it ON because its already the case !
					}
				}
				// UI DETECT TOUCH as a TOUCH OUT !
				else
				{
					if (ISFLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN)) // Is  OUT
					{
						FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was OUT
						FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN);	 // Is  OUT
					}
					else // Is IN
					{
						FLAG_ON(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was IN
						FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN); // Is  OUT

						tin--;
					}
				}

				// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
				_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | (plistenedtouch->Flags & MASK_NTOUCH_TRACK_MOVE_IN_OUT);
				// FLAG_ON ( FLAG_OFF( _STREAMTOUCH(*pt).Flags, MASK_NTOUCH_TRACK_MOVE_IN_OUT ), (plistenedtouch->Flags & MASK_NTOUCH_TRACK_MOVE_IN_OUT) );
			}
			// Not TRACK MOVE IN OUT ?
			// 			else
			// 			{
			// 				not necessary if user read these flags after verifying 'ui' is TRACK MOVE IN OUT control !!!
			// 				FLAG_OFF( _STREAMTOUCH(*pt).Flags, MASK_NTOUCH_TRACK_MOVE_IN_OUT );
			// 			}

			// Store the touch Index into the 'pout_streamed_touchseq' UITouchSequence
			pextracted_streamed_touchseq->TouchIndex[pextracted_streamed_touchseq->TouchCount] = *pt;
			pextracted_streamed_touchseq->TouchCount++;
		}
		// UI doesn't Recognize the Touch !
		// Rewrite the touch ID inside 'pin_streamed_touchseq' (maybe it will be shifted to the left if some previous touches were got by UI )
		else
		{
			pin_streamed_touchseq->TouchIndex[k] = *pt;
			k++;
		}
	}

	// ... If there is some touch managed by the UI ...
	if (pextracted_streamed_touchseq->TouchCount)
		plistener->Listener.PreviousLTouchCountIn = plistener->Listener.LTouchCountIn;

	// 'tin' was updated ONLY if pui has TRACK MOVE IN OUT behavior ... otherwise it stills equal to its initialization value (= plistener->Listener.LTouchCountIn)
	if (tin != plistener->Listener.LTouchCountIn)
	{
		plistener->Listener.LTouchCountIn = tin;
	}

	pin_streamed_touchseq->TouchCount -= pextracted_streamed_touchseq->TouchCount;
}
static inline void _ui_listen_touch_move_sequence__released_move_out(NTOUCH_UI_LISTENER *plistener, NTOUCH_SEQUENCE *pextracted_streamed_touchseq, NTOUCH_SEQUENCE *pcancel_streamed_touchseq, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!plistener || !pextracted_streamed_touchseq || !pcancel_streamed_touchseq || !pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);		   // 'pin'	must contains at least one touch
	NErrorIf(pextracted_streamed_touchseq->TouchCount, NERROR_NON_NULL_VALUE); // 'pextracted_streamed_touchseq'	! must be empty !
	NErrorIf(pcancel_streamed_touchseq->TouchCount, NERROR_NON_NULL_VALUE);	   // 'pcancel_streamed_touchseq'	! must be empty !

	NErrorIf(ISFLAG_ON(plistener->pUI->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT), NERROR_INCONSISTENT_FLAGS); // A non capture UI can not be Under Touch Control at the same time. This propertie is reserved for UI that capture the touch !
	// Extract loop
	Nu32 k = 0;
	NLISTENED_TOUCH *plistenedtouch;
	Nu8 *pt = pin_streamed_touchseq->TouchIndex;
	NTOUCH *pstreamtouch;
	NVEC2 hrel;

	for (Nu32 i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
	{
		// Incoming streamed touch is not MOVE ??!!??
		NErrorIf(_STREAMTOUCH(*pt).Phase != NTOUCH_PHASE_MOVE, NERROR_SYSTEM_CHECK);

		// UI manage the Touch
		pstreamtouch = &_STREAMTOUCH(*pt);
		if (plistenedtouch = NGetListenedTouch(&plistener->Listener, pstreamtouch->SystemID))
		{
			// Listened touch is INACTIVE ?!?
			NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistenedtouch->Phase), NERROR_SYSTEM_CHECK);

			// Is TOUCH still IN UI ?
			if (plistener->pUI->Pick_Proc(plistener->pUI, NCoord_PixToHRel(&hrel, &pstreamtouch->Position)))
			{
				// Update Listened Touch By Hand
				plistenedtouch->Phase = NTOUCH_PHASE_MOVE;

				// Store the touch Index into the 'pextracted_streamed_touchseq' UITouchSequence
				pextracted_streamed_touchseq->TouchIndex[pextracted_streamed_touchseq->TouchCount] = *pt;
				pextracted_streamed_touchseq->TouchCount++;

				// ...
				// And that's it ... the code as it is will remove the touch from the 'pin_streamed_touchseq'.
				// ... to be Cristal clear, the code as it is, will not REWRITE the touch in the 'pin_streamed_touchseq' updated version...
			}
			// THE TOUCH MOVE OUT of the UI !!!!
			else
			{
				// Update Listened Touch By Hand
				plistenedtouch->Phase = NTOUCH_PHASE_CANCEL; // !!!! Set it to CANCEL evenif _STREAMTOUCH(*pt).Phase =  NTOUCH_PHASE_MOVE !!!
															 // !!!! This special case is handled by ENGINE !!!

				// Store the touch Index into the 'pcancel_streamed_touchseq' UITouchSequence
				pcancel_streamed_touchseq->TouchIndex[pcancel_streamed_touchseq->TouchCount] = *pt;
				pcancel_streamed_touchseq->TouchCount++;

				// ... And ...
				// Rewrite the touch ID inside 'pin_streamed_touchseq' (maybe it will be shifted to the left if some previous touches were got by UI )
				pin_streamed_touchseq->TouchIndex[k] = *pt;
				k++;
			}
		}
		// UI doesn't Recognize the Touch !
		// Rewrite the touch ID inside 'pin_streamed_touchseq' (maybe it will be shifted to the left if some previous touches were got by UI )
		else
		{
			pin_streamed_touchseq->TouchIndex[k] = *pt;
			k++;
		}
	}

	pin_streamed_touchseq->TouchCount -= pextracted_streamed_touchseq->TouchCount;
}

// ------------------------------------------------------------------------------------------
// static inline Nu32 _ui_extract_and_unregister_listened_touch
// ------------------------------------------------------------------------------------------
// Description :
//		doing exactly the same thing than 'NUIExtractListenedTouch'
//		PLUS
//		the listened touches are removed from the Listener Table !
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//		return the number of touches extracted from 'pin', Removed from plistener Table, and add into 'pout'
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _ui_listen_touch_endcancel_sequence(NTOUCH_UI_LISTENER *plistener, const Nu8 touch_phase, NTOUCH_SEQUENCE *pextracted_streamed_touchseq, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!plistener || !pextracted_streamed_touchseq || !pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);		   // 'pin'	must contains at least one touch
	NErrorIf(pextracted_streamed_touchseq->TouchCount, NERROR_NON_NULL_VALUE); // 'pextracted_streamed_touchseq'	! must be empty !
	NErrorIf(plistener->Listener.LTouchCount != plistener->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);

	// Extract loop
	Nu8 out = 0;
	Nu8 k = 0;
	NLISTENED_TOUCH *plistenedtouch;
	Nu8 *pt = pin_streamed_touchseq->TouchIndex;
	for (Nu32 i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
	{
		// Incoming streamed touch has to have ..._END or ..._CANCEL phase.
		NErrorIf(_STREAMTOUCH(*pt).Phase != touch_phase, NERROR_SYSTEM_CHECK);

		// UI manage the Touch
		if (plistenedtouch = NGetListenedTouch(&plistener->Listener, _STREAMTOUCH(*pt).SystemID))
		{
			// Listened touch should not already ENDED or CANCELED !
			NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistenedtouch->Phase), NERROR_SYSTEM_CHECK);

			// Update Listened Touch By Hand
			plistenedtouch->Phase = touch_phase;

			// TRACK MOVE IN OUT behavior ?
			if (ISFLAG_ON(plistener->pUI->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT))
			{
				if (ISFLAG_ON(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN)) // Is  IN
				{
					FLAG_ON(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was IN
					FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_IS_IN); // Is  OUT

					// Touch is IN ( and now Was IN ) so it must be Out !!!
					out++;
				}
				else // ISFLAG_ON(plistenedtouch->Flags,FLAG_NTOUCH_IS_OUT) // Is  OUT
				{
					FLAG_OFF(plistenedtouch->Flags, FLAG_NTOUCH_WAS_IN); // Was OUT
																		 // FLAG_OFF(plistenedtouch->Flags,FLAG_NTOUCH_IS_IN );	// Is  OUT	it's already the case !
				}

				// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
				_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | (plistenedtouch->Flags & MASK_NTOUCH_TRACK_MOVE_IN_OUT);
				// FLAG_ON ( FLAG_OFF( _STREAMTOUCH(*pt).Flags, MASK_NTOUCH_TRACK_MOVE_IN_OUT ), (plistenedtouch->Flags & MASK_NTOUCH_TRACK_MOVE_IN_OUT) );
			}
			// Not TRACK MOVE IN OUT ? All Touch are considered as In and must be Out !
			else
			{
				// FLAG_OFF( _STREAMTOUCH(*pt).Flags, MASK_NTOUCH_TRACK_MOVE_IN_OUT ); not necessary if user read these flags after verifing 'ui' has TRACK MOVE IN OUT behavior !!!
				out++;
			}

			// Store the touch Index into the UITouchSequence
			pextracted_streamed_touchseq->TouchIndex[pextracted_streamed_touchseq->TouchCount] = *pt;
			pextracted_streamed_touchseq->TouchCount++;
		}
		// UI doesn't Recognize the Touch !
		// Rewrite the touch ID inside 'pin' (maybe it will be shifted to the left if some previous touches were got by UI )
		else
		{
			pin_streamed_touchseq->TouchIndex[k] = *pt;
			k++;
		}
	}

	if (out)
	{
		plistener->Listener.PreviousLTouchCountIn = plistener->Listener.LTouchCountIn;
		plistener->Listener.LTouchCountIn -= out;
	}

	return pextracted_streamed_touchseq->TouchCount;
}

// ------------------------------------------------------------------------------------------
// static inline void _ui_dispatch_touch_cancel_recursive
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ui_dispatch_touch_cancel_recursive(NUI *pui, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pui->pTouchUIListener, NERROR_SYSTEM_CHECK);
	NErrorIf(!pui->pTouchUIListener->Listener.LTouchCount, NERROR_SYSTEM_CHECK);
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

	NTOUCH_UI_LISTENER *puilistener = pui->pTouchUIListener;
	NEVENT nevent;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup incoming 'pui' specific Touch Sequence.
	// + That means, get each touch from the incoming Touch Sequence 'pin' and check if 'pui' manages it or not ( it manages it if it get it at the Touch start ).
	// +		Thanks to the function 'NUIExtractListenedTouch', all the touches index "managed" by 'pui' are inserted
	// +		into a new Touch Sequence and at the same time,	removed from Touch Sequence "pin".
	// +

	// Set extracted touch sequence to ZERO ( just touch count is enough ...)
	nevent.Touch.TouchSequence.TouchCount = 0;
	// Continue deeper ONLY if UI listen some Touches into the 'pin' touch sequence.
	if (_ui_listen_touch_endcancel_sequence(puilistener, NTOUCH_PHASE_CANCEL, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Listen %d Touch(es) that CANCELED.", nevent.Touch.TouchSequence.TouchCount);
#endif

		pin_streamed_touchseq->TouchCount -= nevent.Touch.TouchSequence.TouchCount;

		// Finalize Listener update
		puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
		puilistener->Listener.LTouchCount -= nevent.Touch.TouchSequence.TouchCount;
		FLAG_ON(puilistener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
		NFillupEventHead(nevent, NTOUCH_CANCEL, 0, NEVENT_RECIPIENT_CAST_UI, pui);
		NUIClearWatchStackPush(pui);

		// SEND EVENT
		// ------------------------------------------------------------------------------------------------
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+");
		start_newline(recursive_count);
		printf("|");
		if (pui->pName)
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL to UI[0x%p][%s].UIEventToState_Proc()", pui->pName, pui);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL to UI[0x%p][%s].Event_Proc()-->", pui->pName, pui);
		}
		else
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL to UI[0x%p].UIEventToState_Proc()", pui);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL to UI[0x%p].Event_Proc()-->", pui);
		}
#endif

		pui->UIEventToState_Proc(pui, &nevent);
		NUI_EVENT_RESULT uiresult = pui->Event_Proc(pui, &nevent);

#ifdef _DEBUG
		if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			printf(" Return NUI_RESULT_DISPATCH.");
		else
			printf(" Return NUI_RESULT_PROCESSED...  Will probably crashed in debug mode!");
		printf("\n");
#endif

		// ------------------------------------------------------------------------------------------------
		// SEND EVENT

		// "pui" was deleted during 'pui->Event_Proc' event processing ...
		// or pTouchUIListener was deleted by a nested Call to "Cancel All"
		if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Dispatch interrupted.(UI Touches were all canceled or UI was deleted.)");
#endif

			return;
		}
		// "pui" stills here, "plistener" (which is 'pui->pTouchUIListener') stills here too ! So ...
		else
		{
			// Definitively erase Listened Touch !
			NCleanTouchListenerUp(&puilistener->Listener);

			NUIClearWatchStackPush(pui);

			//  ... Continue Deeper if there are some children to dispatch to (obviously ! )
			if (NIS_NODE_VALID(puilistener->ChildrenList.pLast, &puilistener->ChildrenList))
			{
				// NTOUCH_CANCEL event need to be dispatched !!! There is no choice.
				// In Release mode, Engine is going to dispatch them automatically,
				// even if user doesn't returns NUI_EVENT_RESULT_DISPATCH, but in Debug Mode a CRASH will occur.
				NErrorIf(!NIS_UI_EVENT_RESULT_DISPATCH(uiresult), NERROR_EVENT_TOUCH_CANCEL_MUST_BE_DISPATCHED);

#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ UI(Listener) Dispatch these NTOUCH_CANCEL to its Listener Children.");
#endif

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
				Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

				Nu32 stacksize = 0;
				NTOUCH_UI_LISTENER *puilistenerchild = (NTOUCH_UI_LISTENER *)puilistener->ChildrenList.pLast;
				while (NIS_NODE_VALID(puilistenerchild, &puilistener->ChildrenList))
				{
					NErrorIf(!puilistenerchild->pUI, NERROR_SYSTEM_CHECK);
					NUIClearWatchStackPush(puilistenerchild->pUI);
					stacksize++;

					puilistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(puilistenerchild);
				}

#ifdef _NOUT_SYS_EVENT
				if (stacksize)
				{
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->%d/%d UI Listeners Pushed from UIListener ChildrenList to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->START UI Pop and Recursive NTOUCH_CANCEL dispatching.");
				}
				else
				{

					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->There is NO UI Listeners (UIListener ChildrenList is empty).");
					start_newline(recursive_count);
					printf("	NO dispatching to UI Children.");
				}
				recursive_count++;
#endif

				// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui'linked with listenerchild if it still exist.
				// ( because it could be deleted during the process ... )
				// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
				//
				// According with uiresult, send CANCEL EVENT
				NUI *puichild;
				NTOUCH_SEQUENCE ts_in = nevent.Touch.TouchSequence;
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
#ifdef _NOUT_SYS_EVENT
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
						recursive_count++;
#endif

						_ui_dispatch_touch_cancel_recursive(puichild, &ts_in);

#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif

						// Stop dispatching if there is no more touches to dispatch !
						if (!ts_in.TouchCount)
						{
							NDecreaseUIClearWatchStackSize(stacksize - 1);
							break;
						}
					}
					stacksize--;
				}
				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}

			// After Children management ( if plistener had children ) we can manage potential 'plistener' removal
			// But, first be sure 'pui' still exist because if not, plistener doesn't exist anymore !!!
			// notice we have to test 'pui->pTouchUIListener' because it could be deleted by a nested call to cancel all !
			// ... and remember 'puilistener' IS 'pui->pTouchUIListener' so ...
			if ((NUIClearWatchStackPop() == pui) && pui->pTouchUIListener && (!puilistener->Listener.LTouchBufferSize))
			{
				NUnbindTouchUIListener(puilistener);
			}
		}
	}
}
// ------------------------------------------------------------------------------------------
// static inline void _ui_dispatch_touch_end_recursive
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ui_dispatch_touch_end_recursive(NUI *pui, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pui->pTouchUIListener, NERROR_SYSTEM_CHECK);
	NErrorIf(!pui->pTouchUIListener->Listener.LTouchCount, NERROR_SYSTEM_CHECK);
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

	NTOUCH_UI_LISTENER *puilistener = pui->pTouchUIListener;
	NEVENT nevent;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup incoming 'pui' specific Touch Sequence.
	// + That means, get each touch from the incoming Touch Sequence 'pin' and check if 'pui' manages it or not ( it manages it if it get it at the Touch start ).
	// +		Thanks to the function 'NUIExtractListenedTouch', all the touches index "managed" by 'pui' are inserted
	// +		into a new Touch Sequence and at the same time,	removed from Touch Sequence "pin".
	// +

	// Set extracted touch sequence to ZERO ( just touch count is enough ...)
	nevent.Touch.TouchSequence.TouchCount = 0;

	// Continue deeper ONLY if UI listen some Touches into the 'pin' touch sequence.
	if (_ui_listen_touch_endcancel_sequence(puilistener, NTOUCH_PHASE_END, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Listen %d Touch(es) that ENDED.", nevent.Touch.TouchSequence.TouchCount);
#endif

		pin_streamed_touchseq->TouchCount -= nevent.Touch.TouchSequence.TouchCount;

		puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
		puilistener->Listener.LTouchCount -= nevent.Touch.TouchSequence.TouchCount;
		FLAG_ON(puilistener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
		NFillupEventHead(nevent, NTOUCH_END, 0, NEVENT_RECIPIENT_CAST_UI, pui);
		NUIClearWatchStackPush(pui);

		// SEND EVENT
		// ------------------------------------------------------------------------------------------------
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+");
		start_newline(recursive_count);
		printf("|");
		if (pui->pName)
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_END to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_END to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
		}
		else
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_END to UI[0x%p].UIEventToState_Proc()", pui);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_END to UI[0x%p].Event_Proc()-->", pui);
		}
#endif

		pui->UIEventToState_Proc(pui, &nevent);
		NUI_EVENT_RESULT uiresult = pui->Event_Proc(pui, &nevent);

#ifdef _NOUT_SYS_EVENT
		if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			printf(" Return NUI_RESULT_DISPATCH.");
		else
			printf(" Return NUI_RESULT_PROCESSED.");
		printf("\n");
#endif
		// ------------------------------------------------------------------------------------------------
		// SEND EVENT

		// "pui" was deleted during 'pui->Event_Proc' event processing ...
		// or pTouchUIListener was deleted by a nested Call to "Cancel All"
		if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Dispatch interrupted.(UI Touches were all ended or UI was deleted.)");
#endif

			return;
		}
		// "pui" stills here, "plistener" (which is 'pui->pTouchUIListener') stills here too ! So ...
		else
		{
			// Definitively erase Listened Touch !
			NCleanTouchListenerUp(&puilistener->Listener);

			NUIClearWatchStackPush(pui); // Remember pui = puilistener->pUI AND puilistener = pui->pTouchUIListener

			// ...  Continue deeper ... if there are some children to dispatch to (obviously ! )
			if (NIS_NODE_VALID(puilistener->ChildrenList.pLast, &puilistener->ChildrenList))
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ UI(Listener) Dispatch these NTOUCH_END to its Listener Children.");
#endif

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
				Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

				Nu32 stacksize = 0;
				NTOUCH_UI_LISTENER *puilistenerchild = (NTOUCH_UI_LISTENER *)puilistener->ChildrenList.pLast;
				while (NIS_NODE_VALID(puilistenerchild, &puilistener->ChildrenList))
				{
					NErrorIf(!puilistenerchild->pUI, NERROR_SYSTEM_CHECK);
					NUIClearWatchStackPush(puilistenerchild->pUI);
					stacksize++;

					puilistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(puilistenerchild);
				}

#ifdef _NOUT_SYS_EVENT
				if (stacksize)
				{
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->%d/%d UI Listeners Pushed from UIListener ChildrenList to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->START UI Pop and Recursive NTOUCH_END dispatching.");
				}
				else
				{

					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->There is NO UI Listeners (UIListener ChildrenList is empty).");
					start_newline(recursive_count);
					printf("	NO dispatching to UI Children.");
				}
				recursive_count++;
#endif

				// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui'linked with listenerchild if it still exist.
				// ( because it could be deleted during the process ... )
				// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
				//
				// According with uiresult, send END EVENT
				NUI *puichild;
				NTOUCH_SEQUENCE ts_in = nevent.Touch.TouchSequence;

				if (uiresult == NUI_EVENT_RESULT_DISPATCH)
				{
					while (stacksize)
					{
						if (puichild = NUIClearWatchStackPop())
						{
#ifdef _NOUT_SYS_EVENT
							start_newline(recursive_count);
							printf("|");
							start_newline(recursive_count);
							printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
							recursive_count++;
#endif

							_ui_dispatch_touch_end_recursive(puichild, &ts_in);

#ifdef _NOUT_SYS_EVENT
							recursive_count--;
#endif

							// Stop dispatching if there is no more touches to dispatch !
							if (!ts_in.TouchCount)
							{
								NDecreaseUIClearWatchStackSize(stacksize - 1);
								break;
							}
						}
						stacksize--;
					}
				}
				else
				{
					//  ... or simply refuse to dispatch
					//	In that case we have to KILL Touches that UI children potentially listen !!!
					while (stacksize)
					{
						if (puichild = NUIClearWatchStackPop())
						{
#ifdef _NOUT_SYS_EVENT
							start_newline(recursive_count);
							printf("|");
							start_newline(recursive_count);
							printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
							recursive_count++;
#endif

							_ui_dispatch_touch_cancel_recursive(puichild, &ts_in);

#ifdef _NOUT_SYS_EVENT
							recursive_count--;
#endif

							// Stop dispatching if there is no more touches to dispatch !
							if (!ts_in.TouchCount)
							{
								NDecreaseUIClearWatchStackSize(stacksize - 1);
								break;
							}
						}
						stacksize--;
					}
				}

				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}

			// After Children management ( if plistener had children ) we can manage potential 'plistener' removal
			// But, first be sure 'pui' still exist because if not, plistener doesn't exist anymore !!!
			// notice we have to test 'pui->pTouchUIListener' because it could be deleted by a nested call to cancel all !
			// ... and remember 'puilistener' IS 'pui->pTouchUIListener' so ...
			if ((NUIClearWatchStackPop() == pui) && pui->pTouchUIListener && (!puilistener->Listener.LTouchBufferSize))
			{
				NUnbindTouchUIListener(puilistener);
			}
		}
	}
}

static inline void _ui_listen_touch_move_in_sequence(NUI *pui, const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	// Streamed
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);			 // It has to be Defined !
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE); // It has to be Defined !

	NTOUCH_UI_LISTENER *puilistener;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + A/ CREATE ( and BIND with UI ) or GET ( the already bound with UI ) UI LISTENER:
	// +

	// Use already UI linked Listener
	if (pui->pTouchUIListener)
	{
		puilistener = pui->pTouchUIListener;
	}
	// ... or get a NEW Listener
	else
	{
		puilistener = NBindTouchUIListener(pui);
	}
	// UI Listener CHECK
	NErrorIf(!puilistener, NERROR_NULL_POINTER);			// It has to be Defined ! |_These 2 tests seems useless but anyway ... !
	NErrorIf(puilistener->pUI != pui, NERROR_SYSTEM_CHECK); // ... And well defined ! |

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + B/ ADD BY HAND NEW Incoming Touch ID into the LISTENER BUFFER
// +
#ifdef _DEBUG
	// They have to be Equal !
	NErrorIf(puilistener->Listener.LTouchCount != puilistener->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
	NLISTENED_TOUCH *plt;
	for (Nu32 i_dbg = 0; i_dbg < pin_streamed_touchseq->TouchCount; i_dbg++)
	{
		// + In Debug Mode, check if incoming Touch is really in Phase MOVE !
		NErrorIf(_STREAMTOUCH(pin_streamed_touchseq->TouchIndex[i_dbg]).Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_VALUES);
		// + In debug mode, Check that none of the new incoming touch is already inserted into the Listener Buffer
		plt = puilistener->Listener.pLTouchBuffer;
		for (Nu8 j_dbg = puilistener->Listener.LTouchBufferSize; j_dbg != 0; j_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(pin_streamed_touchseq->TouchIndex[i_dbg]).SystemID, NERROR_SYSTEM_CHECK);
		}
	}
#endif

	// At Start we don't care about "FLAG_NUI_UNDER_TOUCH_CONTROL" to update "PreviousTouchCountOut" and "TouchCountOut"
	// These 2 members still available even if 'pui' is not Under Touch Control, BUT in that case,
	//			'PreviousTouchCountOut' and 'TouchCountOut' still always equal to  0
	//
	// In other words, Touch moves going to change these values ONLY if 'pui' IS Under Touch Control.
	//
	Nu32 i;
	Nu8 *pt = (Nu8 *)pin_streamed_touchseq->TouchIndex;
	if (ISFLAG_ON(pui->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES) /* && ISFLAG_OFF(_STREAMTOUCH(*pt).Flags,FLAG_NTOUCH_LOCK_GAMESTATE)*/) // we are going to flag it on several times but it's not a problem
	{
		// Because 'pui' will block this touch move event for game state, game state is going to receive a touch cancel ...
		// So to optimize algorithm N has to warn gamestate touch dispatch to switch to advanced mode ...
		// Notice it's not necessary to switch into a special touch move dispatch move if UI doesn't block the touch for game state, in that second case,
		// whatever happening to 'pui' ( move in / move out ) it's not going to have any impact on gamestate ...
		FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_GAMESATE_TOUCH_MOVE_ADVANCED_DISPATCH);

		for (i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
		{
			FLAG_ON(_STREAMTOUCH(*pt).Flags, FLAG_NTOUCH_LOCK_GAMESTATE);
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf(". UI LOCK GAMESTATE");
			printf(". TOUCH[index:%d][StreamedIndex:%d] will be ignored by GameState", i, *pt);
#endif

			// Insert Listened Touch by Hand to Listener
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].SystemID = _STREAMTOUCH(*pt).SystemID;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].User_Nu16 = 0;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Flags = FLAG_NTOUCH_IS_IN;

			// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
			_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | FLAG_NTOUCH_IS_IN;
			// Notice that 'FLAG_NTOUCH_IS_IN' is set to ON without any test about UNDER TOUCH CONTROL ... It's normal !!!
			// Because with or without UNDER TOUCH Control, counters TouchCountIn is updated at start / end.
			// UNDER TOUCH Control update it too at MOVE ...
			// Some safety check
			NErrorIf(puilistener->Listener.LTouchBufferSize == puilistener->Listener.LTouchBufferCapacity, NERROR_SYSTEM_CHECK);
			puilistener->Listener.LTouchBufferSize++;
		}
	}
	else
	{
		for (i = pin_streamed_touchseq->TouchCount; i != 0; i--, pt++)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf(". UI DOESN'T LOCK GAMESTATE");
			printf(". TOUCH[index:%d][StreamedIndex:%d] will be received by GameState", i, *pt);
#endif

			// Insert Listened Touch by Hand to Listener
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].SystemID = _STREAMTOUCH(*pt).SystemID;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].User_Nu16 = 0;
			puilistener->Listener.pLTouchBuffer[puilistener->Listener.LTouchBufferSize].Flags = FLAG_NTOUCH_IS_IN;

			// Copy Under touch Flags into Streamed NTOUCH.FLAGS !!! ( it will be temporary just for potential user access from Event_handle )
			_STREAMTOUCH(*pt).Flags = (_STREAMTOUCH(*pt).Flags & ~MASK_NTOUCH_TRACK_MOVE_IN_OUT) | FLAG_NTOUCH_IS_IN;
			// Notice that 'FLAG_NTOUCH_IS_IN' is set to ON without any test about UNDER TOUCH CONTROL ... It's normal !!!
			// Because with or without UNDER TOUCH Control, counters TouchCountIn is updated at start / end.
			// UNDER TOUCH Control update it too at MOVE ...
			// Some safety check
			NErrorIf(puilistener->Listener.LTouchBufferSize == puilistener->Listener.LTouchBufferCapacity, NERROR_SYSTEM_CHECK);
			puilistener->Listener.LTouchBufferSize++;
		}
	}
	// UPDATE LISTERNER TOUCH COUNTS
	puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
	puilistener->Listener.LTouchCount = puilistener->Listener.LTouchBufferSize;
	puilistener->Listener.PreviousLTouchCountIn = puilistener->Listener.LTouchCountIn;
	puilistener->Listener.LTouchCountIn += pin_streamed_touchseq->TouchCount;

	NErrorIf(puilistener->Listener.LTouchCount > puilistener->Listener.LTouchCountMax, NERROR_SYSTEM_CHECK);
}

// !!! This function is closer to touch start dispatch than touch move dispatch ...
static inline Nu32 _ui_dispatch_touch_move_recursive__capture_move_in(NUI *pui, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pui, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

	Nu32 child_get_return = 0;
	Nu32 stacksize;
	NUI *puichild;

#ifdef _DEBUG
	Nu32 dbg_initial_clear_watch_stack_size;
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + PASS A: Pass trough UI that Doesn't listen MOVE IN
	// +
	//
	// Here, UI may be one of these 6 Types ( about touch event listening ):
	//		1) Listen Start, Doesn't Listen Move IN but have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is ON
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is OFF
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is ON
	//		2) Listen Start, Listen MOVE IN and doesn't have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is ON
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is OFF
	//		3) Listen Start, Listen MOVE IN and have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is ON
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is ON
	//		4) Doesn't Listen Start, Doesn't Listen Move IN but have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is OFF
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is OFF
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is ON
	//		5) Doesn't Listen Start, Listen MOVE IN and doesn't have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is OFF
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is OFF
	//		6) Doesn't Listen Start, Listen MOVE IN and have children that Listen MOVE IN:
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START	is OFF
	//				FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN	is ON
	//				FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN	is ON

	//
	// In the specific case 4), UI must let EVENT pass trough it to feed children which capture touch Move In
	// or which are also in the same case 1).
	// Because the case where both the 3 flags are OFF can not be here ...
	// testing  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN is OFF and FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START is OFF
	// is enough to be sure that we are in case 4) !
	// In debug mode, we are going to check these assumptions !
	// ...Both the 3 flags can not be OFF.
	NErrorIf(FLAGS_TEST(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, 0), NERROR_SYSTEM_CHECK);
	// ...Both the 2 flags MOVE IN relative ( capture and have children interested )can not be OFF.
	NErrorIf(FLAGS_TEST(pui->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, 0), NERROR_SYSTEM_CHECK);

	// So...	If FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN is OFF
	//			That means FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN is ON
	//
	if (ISFLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN))
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Doesn't Listen Touch Event ( Neither at Start nor at Move In )");
		start_newline(recursive_count);
		printf("+ It's like a ghost.");
#endif

		// UI Capture Touch at start ... so it has to filter 'pin_streamed_touchseq' to only keep
		// the ones it should capture itself at a 'usual' touch start ...
		/*
					if( ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START) )
					{
						if( _is_ui_accept_touch(pui) )
						{
							NUI_EVENT_RESULT	uiresult;
							NEVENT				nevent;
							NVEC2			hrel;
							Nu32				t;
							Nu8					uitouchcount;

							//		NTOUCH_SEQUENCE		streamed_in_uncaptured;
							Nu8					uncaptured_touchcount = 0;
							Nu8					*pt = pin_streamed_touchseq->TouchIndex;
							Nu8					out_count = pin_streamed_touchseq->TouchCount;
							// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
							// +
							// + Setup incoming 'pui' specific Touch Sequence.
							// + That means, get each touch from the incoming Touch Sequence 'pin' and decide if 'pui' gets it or not.
							// +		All the touches index "captured" by 'pui' are inserted into a new Touch Sequence and at the same time,
							// +		removed from Touch Sequence "pin"
							// +

							// Set the extracted streamed touch sequence to ZERO ( setup TouchCount is enough ...)
							// ( This extracted touch sequence is the Event touch sequence itself )
							nevent.Touch.TouchSequence.TouchCount	= 0;

							// Retrieve current number of Touch listen by 'pui'
							if(pui->pTouchUIListener)
								uitouchcount = pui->pTouchUIListener->Listener.LTouchCount;
							else
								uitouchcount = 0;

							for(t=pin_streamed_touchseq->TouchCount;t!=0;t--,pt++)
							{
								// UI get the Touch !
								if( uitouchcount < pui->TouchCountMax && pui->Pick_Proc( pui, NCoord_PixToHRel(&hrel ,&_STREAMTOUCH(*pt).Position) ))
								{
									// Store the touch Index into the Extracted Touch Sequence
									nevent.Touch.TouchSequence.TouchIndex[nevent.Touch.TouchSequence.TouchCount] = *pt;
									nevent.Touch.TouchSequence.TouchCount ++;
									uitouchcount ++;
								}
								// UI doesn't get the Touch !
								// Write the touch ID inside 'streamed_in_uncaptured'
								else
								{
									pin_streamed_touchseq->TouchIndex[uncaptured_touchcount] = *pt;
									uncaptured_touchcount++;
								}
							}
						}
					}
		*/

		// As explained, if we are here and thanks to previous tests:
		// 1- UI must have children that listen touch move in ...
		NErrorIf(ISFLAG_OFF(pui->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);
		// 2- UI must be a total Ghost ( non listen touch )
		NErrorIf(ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START), NERROR_SYSTEM_CHECK);
		NErrorIf(!NIS_NODE_VALID(pui->ChildrenList.pFirst, &pui->ChildrenList), NERROR_SYSTEM_CHECK);

#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Dispatch All Touch(es) to its Children. Without any BBOX filter.");
#endif

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
#ifdef _DEBUG
		dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

		child_get_return = 0;
		stacksize = 0;
		puichild = (NUI *)pui->ChildrenList.pFirst;
		while (NIS_NODE_VALID(puichild, &pui->ChildrenList))
		{
			// 				if( NIsUIActive(puichild)
			// 					&& (ISFLAG_ON(puichild->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)||FLAGS_TEST(puichild->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) ) )
			if (NIsUIActive(puichild) && (NIsUICaptureTouchMoveIn(puichild) || NIsUICaptureTouchMoveInDummy(puichild)))
			{
				NUIClearWatchStackPush(puichild);
				stacksize++;
			}
			puichild = (NUI *)NGET_NEXT_NODE(puichild);
		}

#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ %d/%d UI Children Pushed to NUIClearWatchStack.", stacksize, NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui));
		recursive_count++;

#endif

		// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui child' if it still exist.
		// ( because it could be deleted during the process ... )
		// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
		// Coying the 'pui' listened touch sequence to dispatch it (the copy ) between 'pui' children.
		while (stacksize)
		{
			if (puichild = NUIClearWatchStackPop())
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				start_newline(recursive_count);
				printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
				recursive_count++;
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");

#endif

				if (_ui_dispatch_touch_move_recursive__capture_move_in(puichild, pin_streamed_touchseq))
				{
					child_get_return = 1; // To remember, one child, at least, get some touches and remove them from 'touch_sequence_in'

					if (!pin_streamed_touchseq->TouchCount)
					{
						NDecreaseUIClearWatchStackSize(stacksize - 1);
						break;
					}
				}

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}

			stacksize--;
		}
		NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
		recursive_count--;
#endif

		return child_get_return;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + PASS B: Dispatch to UI that listen MOVE IN
	// +
	// Here, UI may be one of the 2 Types ( about touch event listening ) 2) or 3) (see above for details )
	// That means UI is listening touch move In !!!
	// In debug mode, we are going to check this assumption !
	NErrorIf(ISFLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN), NERROR_SYSTEM_CHECK);

	// try dispatching the incoming Touch Sequence 'pin' to 'pui'
	if (_is_ui_accept_touch(pui))
	{
		NUI_EVENT_RESULT uiresult;
		NEVENT nevent;
		NVEC2 hrel;
		Nu32 t;
		Nu8 uitouchcount;

		//		NTOUCH_SEQUENCE		streamed_in_uncaptured;
		Nu8 uncaptured_touchcount = 0;
		Nu8 *pt = pin_streamed_touchseq->TouchIndex;
		//		Nu8					out_count = pin_streamed_touchseq->TouchCount;
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + Setup incoming 'pui' specific Touch Sequence.
		// + That means, get each touch from the incoming Touch Sequence 'pin' and decide if 'pui' gets it or not.
		// +		All the touches index "captured" by 'pui' are inserted into a new Touch Sequence and at the same time,
		// +		removed from Touch Sequence "pin"
		// +

		// Set the extracted streamed touch sequence to ZERO ( setup TouchCount is enough ...)
		// ( This extracted touch sequence is the Event touch sequence itself )
		nevent.Touch.TouchSequence.TouchCount = 0;

		// Retrieve current number of Touch listen by 'pui'
		if (pui->pTouchUIListener)
			uitouchcount = pui->pTouchUIListener->Listener.LTouchCount;
		else
			uitouchcount = 0;

		for (t = pin_streamed_touchseq->TouchCount; t != 0; t--, pt++)
		{
			// UI get the Touch !
			if (uitouchcount < pui->TouchCountMax && pui->Pick_Proc(pui, NCoord_PixToHRel(&hrel, &_STREAMTOUCH(*pt).Position)))
			{
				// Store the touch Index into the Extracted Touch Sequence
				nevent.Touch.TouchSequence.TouchIndex[nevent.Touch.TouchSequence.TouchCount] = *pt;
				nevent.Touch.TouchSequence.TouchCount++;
				uitouchcount++;
			}
			// UI doesn't get the Touch !
			// Write the touch ID inside 'streamed_in_uncaptured'
			else
			{
				pin_streamed_touchseq->TouchIndex[uncaptured_touchcount] = *pt;
				uncaptured_touchcount++;
			}
		}
		// +
		// +
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + If 'pui' doesn't captured any NTOUCH ...
		// + ... return 0 !
		if (!nevent.Touch.TouchSequence.TouchCount)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Doesn't Capture any Touch Move In.");
#endif

			// !!! pin_streamed_touchseq still the same !!!
			return 0;
		}
		// + ... else, continue deeper.
		else
		{
			// Update pin_streamed_touchseq Touch Count.
			pin_streamed_touchseq->TouchCount = uncaptured_touchcount;

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Captured %d Touch(es). %d Touch(es) still non captured.", nevent.Touch.TouchSequence.TouchCount, uncaptured_touchcount);
#endif

			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + IF UI intercepts TOUCH EVENT
			// +	Register all the Touch as Listened Touches by UI.
			// +
			// + AND
			// +
			// +	Call the Event Handles.
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT))
			{
				_ui_listen_touch_move_in_sequence(pui, &nevent.Touch.TouchSequence);
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Register the Touch it captured.");
#endif

				// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
				NFillupEventHead(nevent, NTOUCH_START, NEVENT_PARAM_TOUCH_START__TOUCH_MOVE_IN, NEVENT_RECIPIENT_CAST_UI, pui);
				NUIClearWatchStackPush(pui);

				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				// ------------------------------------
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				if (pui->pName)
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
				}
				else
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p].UIEventToState_Proc()", pui);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p].Event_Proc()-->", pui);
				}
#endif
				// ------------------------------------
				pui->UIEventToState_Proc(pui, &nevent);
				uiresult = pui->Event_Proc(pui, &nevent);

#ifdef _NOUT_SYS_EVENT
				if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
					printf(" Return NUI_RESULT_DISPATCH.");
				else
					printf(" Return NUI_RESULT_PROCESSED.");
				printf("\n");
#endif

				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				// "pui" was deleted during 'pui->Event_Proc' event processing ... or ...
				// "pui" stills exist but simply refuse to dispatch further to it's forward hierarchy.
				if (NUIClearWatchStackPop() != pui || (!NIS_UI_EVENT_RESULT_DISPATCH(uiresult)))
				{
#ifdef _NOUT_SYS_EVENT
					start_newline(recursive_count);
					printf("+ Dispatch interrupted.(UI doesn't returns DISPATCH or was deleted.)");
#endif

					// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!
					return 1;
				}
			}

			// !!!!!!!!
			// FROM THAT POINT we are sure :
			//
			//					1/ 'pui' still exists !
			//
			//					2/ if 'pui' is a LISTENER 'uiresult' return by 'pui->Event_Proc(pui,&nevent);' is DISPATCH !!!
			//
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + Call '_ui_dispatch_touch_move_recursive__capture_move_in' for each 'pui' Children with "touch_sequence".
			// +	! Notes: if 'pui' was registered as a Touch Listener, N inserted 'pui' in the Listener chain
			// +			... thanks to the '_ui_listen_touch_start_sequence' function call.
			// +			... It's important to do it in that order, to preserve the right Event dispatch Order. Parent_Listener -> Children.
			// +

			Nu32 child_capture_return = 0;
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN))
			{
				// Thanks to earlier test "ISFLAG_ON(pui->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN)" ui must have children !!!
				NErrorIf(!NIS_NODE_VALID(pui->ChildrenList.pFirst, &pui->ChildrenList), NERROR_SYSTEM_CHECK);

#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Dispatch the touches it captured to its Children.");
#endif

				// Keep an eye on 'pui' itself ...
				NUIClearWatchStackPush(pui);

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
				dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

				stacksize = 0;
				puichild = (NUI *)pui->ChildrenList.pFirst;
				while (NIS_NODE_VALID(puichild, &pui->ChildrenList))
				{
					// 					if( NIsUIActive(puichild)
					// 						&& (ISFLAG_ON(puichild->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)||FLAGS_TEST(puichild->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) ) )
					if (NIsUIActive(puichild) && (NIsUICaptureTouchMoveIn(puichild) || NIsUICaptureTouchMoveInDummy(puichild)))
					{
						NUIClearWatchStackPush(puichild);
						stacksize++;
					}
					puichild = (NUI *)NGET_NEXT_NODE(puichild);
				}
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ %d/%d UI Children Pushed to NUIClearWatchStack.", stacksize, NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui));
				recursive_count++;
#endif

				// Dispatch to children !
				// Copying the 'pui' listened touch sequence to dispatch it (the copy ) between 'pui' children.
				NTOUCH_SEQUENCE touch_sequence_in = nevent.Touch.TouchSequence;
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
#ifdef _NOUT_SYS_EVENT
						start_newline(recursive_count);
						printf("+");
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
						recursive_count++;
						start_newline(recursive_count);
						printf("+");
						start_newline(recursive_count);
						printf("|");
#endif

						if (_ui_dispatch_touch_move_recursive__capture_move_in(puichild, &touch_sequence_in))
						{
							child_capture_return = 1; // To remember, one child, at least, captured some touches and remove them from 'touch_sequence_in'

							if (!touch_sequence_in.TouchCount)
							{
								NDecreaseUIClearWatchStackSize(stacksize - 1);
								break;
							}
						}
#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif;
					}
					stacksize--;
				}
				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif

				// Maybe 'pui' WAS DELETED during children dispatch processing ...
				if (NUIClearWatchStackPop() != pui)
				{
					// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!
					return 1;
				}
				// else 'pui' stills exist !!! So, just continue ...
			}

			// +
			// +
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + Register 'pui' as a listener when it's the "Touch Hierarchy Leaf" !
			// + ( it captured some touches but not it's children )
			// +
			// + Note:	if 'pui' is flagged as Intercept touch Event , there is nothing to do because everything was already done just before children dispatch try.
			// +		BUT
			// +		if 'pui' doesn't intercept touch event, it has to become a leaf listener.
			// +		Of course, thanks to all the previous filters ( flags tests) we are sure here that "pui" listen touch event
			// +		...especially we are sure FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START is set to ON.
			//
			if (ISFLAG_OFF(pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT) && (!child_capture_return))
			{
				_ui_listen_touch_move_in_sequence(pui, &nevent.Touch.TouchSequence);
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Register the Touch it captured.");
#endif

				// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
				NFillupEventHead(nevent, NTOUCH_START, NEVENT_PARAM_TOUCH_START__TOUCH_MOVE_IN, NEVENT_RECIPIENT_CAST_UI, pui);

#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				if (pui->pName)
				{
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p][%s].UIEventToState_Proc()", pui->pName, pui);
					start_newline(recursive_count);
					printf("+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p][%s].Event_Proc()-->", pui->pName, pui);
				}
				else
				{
					start_newline(recursive_count);
					printf("\t+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p].UIEventToState_Proc()", pui);
					start_newline(recursive_count);
					printf("\t+-->>> SEND EVENT NTOUCH_START (Move In) to UI[0x%p].Event_Proc()-->", pui);
				}
#endif
				// No need to survey pui trough 'ClearWatchStack' for this last Event_Proc call !
				pui->UIEventToState_Proc(pui, &nevent);
				pui->Event_Proc(pui, &nevent);
// !!! pin_streamed_touchseq was modified. It contains all the non captured Touches !!!
#ifdef _NOUT_SYS_EVENT
				printf(" Return ... Don't Care... UI is the terminated leaf !");
#endif
			}

			return 1;
		}
	}
	// UI LISTEN TOUCH EVENT but doesn't accept these TOUCH ( because it reaches it's touch count Max or something else checked before testing UI BBOX ... )
	else
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Doesn't accept these touches.");
#endif

		// !!! pin_streamed_touchseq still the same !!!
		return 0;
	}
}

// ------------------------------------------------------------------------------------------
// static inline void _ui_dispatch_touch_move_recursive
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _ui_dispatch_touch_move_recursive(NUI *pui, NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pui->pTouchUIListener, NERROR_SYSTEM_CHECK);
	NErrorIf(!pui->pTouchUIListener->Listener.LTouchCount, NERROR_SYSTEM_CHECK);
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

	NTOUCH_UI_LISTENER *puilistener = pui->pTouchUIListener;
	//	Nu32				k	= 0;
	NEVENT nevent_move, nevent_released;
	//	Nu8					*pt = pin_streamed_touchseq->TouchIndex;
	//	Nu8					out_count = pin_streamed_touchseq->TouchCount;
	NUI_EVENT_RESULT uiresult;
	NTOUCH_UI_LISTENER *puilistenerchild;
	NUI *puichild;
	Nu32 stacksize;

#ifdef _DEBUG
	Nu32 dbg_initial_clear_watch_stack_size;
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup incoming 'pui' specific Touch Sequence.
	// + That means, get each touch from the incoming Touch Sequence 'pin' and check if 'pui' manages it or not ( it manages it if it get it at the Touch start ).
	// +		Thanks to the function 'NUIExtractListenedTouch', all the touches index "managed" by 'pui' are inserted
	// +		into a new Touch Sequence and at the same time,	removed from Touch Sequence "pin".
	// +
	nevent_move.Touch.TouchSequence.TouchCount = 0;		//|It's enough to setup the 2 events structures !
	nevent_released.Touch.TouchSequence.TouchCount = 0; //|

	if (ISFLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT))
		_ui_listen_touch_move_sequence(puilistener, &nevent_move.Touch.TouchSequence, pin_streamed_touchseq);
	else
		_ui_listen_touch_move_sequence__released_move_out(puilistener, &nevent_move.Touch.TouchSequence, &nevent_released.Touch.TouchSequence, pin_streamed_touchseq);

	// 1) CANCEL for UI that release some touches at MOVE OUT !
	if (nevent_released.Touch.TouchSequence.TouchCount)
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Release %d Touch(es) at MOVE <<OUT>>.", nevent_released.Touch.TouchSequence.TouchCount);
#endif

		if (ISFLAG_ON(pui->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES))
			FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_GAMESATE_TOUCH_MOVE_ADVANCED_DISPATCH);

		// Finalize Listener update
		puilistener->Listener.PreviousLTouchCount = puilistener->Listener.LTouchCount;
		puilistener->Listener.PreviousLTouchCountIn = puilistener->Listener.LTouchCountIn;
		puilistener->Listener.LTouchCount -= nevent_released.Touch.TouchSequence.TouchCount;
		puilistener->Listener.LTouchCountIn = puilistener->Listener.LTouchCount;

		FLAG_ON(puilistener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
		NFillupEventHead(nevent_released, NTOUCH_CANCEL, NEVENT_PARAM_TOUCH_CANCEL__TOUCH_MOVE_OUT, NEVENT_RECIPIENT_CAST_UI, pui);
		NUIClearWatchStackPush(pui);

// SEND EVENT
// ------------------------------------------------------------------------------------------------
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+");
		start_newline(recursive_count);
		printf("|");
		if (pui->pName)
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL (Move Out) to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL (Move Out) to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
		}
		else
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL (Move Out) to UI[0x%p].UIEventToState_Proc()", pui);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_CANCEL (Move Out) to UI[0x%p].Event_Proc()-->", pui);
		}
#endif

		pui->UIEventToState_Proc(pui, &nevent_released);
		uiresult = pui->Event_Proc(pui, &nevent_released);

#ifdef _NOUT_SYS_EVENT
		if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			printf(" Return NUI_RESULT_DISPATCH.");
		else
			printf(" Return NUI_RESULT_PROCESSED.... probably CRASH ! ( excepted if UI is deleted ) ");
		printf("\n");
#endif

		// ------------------------------------------------------------------------------------------------
		// SEND EVENT

		// "pui" was deleted during 'pui->Event_Proc' event processing ...
		// or pTouchUIListener was deleted by a nested Call to "Cancel All"
		if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Dispatch interrupted.(UI Touches were all canceled or UI was deleted.)");
#endif

			return;
		}
		// "pui" stills here, "plistener" (which is 'pui->pTouchUIListener') stills here too ! So ...
		else
		{
			// Definitively erase canceled Touch !
			NCleanTouchListenerUp(&puilistener->Listener);

			NUIClearWatchStackPush(pui);

			//  ... Continue Deeper if there are some children to dispatch to (obviously ! )
			if (NIS_NODE_VALID(puilistener->ChildrenList.pLast, &puilistener->ChildrenList))
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ UI(Listener) Dispatch these NTOUCH_CANCEL to its Listener Children.");
#endif

				// NTOUCH_CANCEL event need to be dispatched !!! There is no choice.
				// In Release mode, Engine is going to dispatch them automatically,
				// even if user doesn't returns NUI_EVENT_RESULT_DISPATCH, but in Debug Mode a CRASH will occur.
				NErrorIf(!NIS_UI_EVENT_RESULT_DISPATCH(uiresult), NERROR_EVENT_TOUCH_CANCEL_MUST_BE_DISPATCHED);

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
				dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

				stacksize = 0;
				puilistenerchild = (NTOUCH_UI_LISTENER *)puilistener->ChildrenList.pLast;
				while (NIS_NODE_VALID(puilistenerchild, &puilistener->ChildrenList))
				{
					NErrorIf(!puilistenerchild->pUI, NERROR_SYSTEM_CHECK);
					NUIClearWatchStackPush(puilistenerchild->pUI);
					stacksize++;

					puilistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(puilistenerchild);
				}
#ifdef _NOUT_SYS_EVENT
				if (stacksize)
				{
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->%d/%d UI Listeners Pushed from UIListener ChildrenList to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->START UI Pop and Recursive NTOUCH_CANCEL (Move Out) dispatching.");
					recursive_count++;
				}
				else
				{
					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->There is NO UI Listeners (UIListener ChildrenList is empty).");
					start_newline(recursive_count);
					printf("	NO NTOUCH_CANCEL dispatching to UI Children.");
				}
#endif

				// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui'linked with listenerchild if it still exist.
				// ( because it could be deleted during the process ... )
				// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
				//
				// According with uiresult, send CANCEL EVENT
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
#ifdef _NOUT_SYS_EVENT
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
						recursive_count++;
#endif

						_ui_dispatch_touch_cancel_recursive(puichild, &nevent_released.Touch.TouchSequence);

#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif

						// Stop dispatching if there is no more touches to dispatch !
						if (!nevent_released.Touch.TouchSequence.TouchCount)
						{
							NDecreaseUIClearWatchStackSize(stacksize - 1);
							break;
						}
					}
					stacksize--;
				}

				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}

			// After Children management ( if plistener had children ) we have to manage all potential critical cases:
			//	1) 'pui' was delete during touch cancel dispatch.
			//	2) 'pui' stills here but doesn't listen any touches due to a nested 'cancel all' during touch cancel dispatch.
			//	3) 'pui' dtills here with its bound listener but this one is empty. It's possible if all listener touches were canceled.
			//	   	( all of them in 'nevent_cancel.Touch.TouchSequence'
			//
			//	For all of the 3, there is no more things to do, just RETURN !
			//
			if (NUIClearWatchStackPop() != pui) // 1
			{
				return;
			}
			else if (!pui->pTouchUIListener) // 2
			{
				return;
			}
			else if (!puilistener->Listener.LTouchBufferSize) // 3
			{
				NUnbindTouchUIListener(puilistener);
				return;
			}
		}
	}

	// 2) MOVE ( first for UI and it's children in the usual Way, then as MOVE IN if some touches still unlistened )
	if (nevent_move.Touch.TouchSequence.TouchCount)
	{
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+ Listen %d Touch(es) that MOVE.", nevent_move.Touch.TouchSequence.TouchCount);
#endif

		// Finalize Listener update
		// ... nothing to do.

		// Calling the 2 UI Event_Handles. First, the Engine One, and after, the User One.
		NFillupEventHead(nevent_move, NTOUCH_MOVE, 0, NEVENT_RECIPIENT_CAST_UI, pui);
		NUIClearWatchStackPush(pui);

// SEND EVENT
// ------------------------------------------------------------------------------------------------
#ifdef _NOUT_SYS_EVENT
		start_newline(recursive_count);
		printf("+");
		start_newline(recursive_count);
		printf("|");
		if (pui->pName)
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_MOVE to UI[0x%p][%s].UIEventToState_Proc()", pui, pui->pName);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_MOVE to UI[0x%p][%s].Event_Proc()-->", pui, pui->pName);
		}
		else
		{
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_MOVE to UI[0x%p].UIEventToState_Proc()", pui);
			start_newline(recursive_count);
			printf("+-->>> SEND EVENT NTOUCH_MOVE to UI[0x%p].Event_Proc()-->", pui);
		}
#endif

		pui->UIEventToState_Proc(pui, &nevent_move);
		uiresult = pui->Event_Proc(pui, &nevent_move);

#ifdef _NOUT_SYS_EVENT
		if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			printf(" Return NUI_RESULT_DISPATCH.");
		else
			printf(" Return NUI_RESULT_PROCESSED.");
		printf("\n");
#endif
		// ------------------------------------------------------------------------------------------------
		// SEND EVENT

		// "pui" was deleted during 'pui->Event_Proc' event processing ...
		// or pTouchUIListener was deleted by a nested Call to "Cancel All"
		if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
		{
#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Dispatch interrupted.(UI Touches were all canceled or UI was deleted.)");
#endif

			return;
		}

		// "pui" stills here, "plistener" (which is 'pui->pTouchUIListener') stills here too ! So ...
		// ...  Continue deeper ... if there are some children to dispatch to (obviously ! )
		if (NIS_NODE_VALID(puilistener->ChildrenList.pLast, &puilistener->ChildrenList))
		{
			NUIClearWatchStackPush(pui);

#ifdef _NOUT_SYS_EVENT
			if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			{
				start_newline(recursive_count);
				printf("+ UI(Listener) Dispatch these NTOUCH_MOVE to its Listener Children.");
			}
			else
			{
				start_newline(recursive_count);
				printf("+ UI(Listener) Dispatch NTOUCH_CANCEL to its Listener Children.");
				printf(" (Because it returns NUI_RESULT_PROCESSED).");
			}
#endif

// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
			dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

			stacksize = 0;
			puilistenerchild = (NTOUCH_UI_LISTENER *)puilistener->ChildrenList.pLast;
			while (NIS_NODE_VALID(puilistenerchild, &puilistener->ChildrenList))
			{
				NErrorIf(!puilistenerchild->pUI, NERROR_SYSTEM_CHECK);
				NUIClearWatchStackPush(puilistenerchild->pUI);
				stacksize++;

				puilistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(puilistenerchild);
			}

#ifdef _NOUT_SYS_EVENT
			if (stacksize)
			{
				start_newline(recursive_count);
				printf("|");
				start_newline(recursive_count);
				printf("+-->%d/%d UI Listeners Pushed from UIListener ChildrenList to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
				start_newline(recursive_count);
				printf("|");
				if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
				{
					start_newline(recursive_count);
					printf("+-->START UI Pop and Recursive NTOUCH_MOVE dispatching.");
				}
				else
				{
					start_newline(recursive_count);
					printf("+-->START UI Pop and Recursive NTOUCH_CANCEL dispatching.");
				}
			}
			else
			{
				start_newline(recursive_count);
				printf("|");
				start_newline(recursive_count);
				printf("+-->There is NO UI Listeners (UIListener ChildrenList is empty).");
				start_newline(recursive_count);
				printf("	NO dispatching to UI Children.");
			}
#endif

			// According with uiresult, send MOVE or CANCEL EVENT
			if (NIS_UI_EVENT_RESULT_DISPATCH(uiresult))
			{
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
#ifdef _NOUT_SYS_EVENT
						recursive_count++;
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->\tUI#%d[0x%p] pops from Stack.", stacksize, puichild);
#endif

						_ui_dispatch_touch_move_recursive(puichild, &nevent_move.Touch.TouchSequence);

#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif

						// Stop dispatching if there is no more touches to dispatch !
						if (!&nevent_move.Touch.TouchSequence.TouchCount)
						{
							NDecreaseUIClearWatchStackSize(stacksize - 1);
							break;
						}
					}
					stacksize--;
				}
#ifdef _DEBUG
				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#endif
				// -----------------------
			}
			else
			{
#ifdef _NOUT_SYS_EVENT
				recursive_count++;
#endif

				//  ... or simply refuse to dispatch
				//	In that case we have to CANCEL Touches that UI children potentially listen !!!
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
#ifdef _NOUT_SYS_EVENT
						start_newline(recursive_count);
						printf("|");
						start_newline(recursive_count);
						printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
						recursive_count++;
#endif

						_ui_dispatch_touch_cancel_recursive(puichild, &nevent_move.Touch.TouchSequence);

#ifdef _NOUT_SYS_EVENT
						recursive_count--;
#endif

						// Stop dispatching if there is no more touches to dispatch !
						if (!nevent_move.Touch.TouchSequence.TouchCount)
						{
							NDecreaseUIClearWatchStackSize(stacksize - 1);
							break;
						}
					}
					stacksize--;
				}

				NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}
			// If a nested delete call delete 'pui' or canceled all touches ... just return
			if (NUIClearWatchStackPop() != pui || !pui->pTouchUIListener)
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+ Dispatch interrupted.(UI Touches were all canceled or UI was deleted.)");
#endif
				return;
			}
		}

		// 3)MOVE IN for UI with children which are going to capture Touch Move In !
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) && nevent_move.Touch.TouchSequence.TouchCount && ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI))
		{
			// Here 'nevent_move.Touch.TouchSequence' contains all the touches not listen/captured by children of 'pui',  including all the touch
			// canceled by 'non-capture children ui' due to a move outside of their pickable shape !
			//
			// Here we are going to try to dispatch those touches to the 'pui' children which are 'non-capture' because they are the only ones
			// capable of starting listening them by a MOVE IN !
			//
			// The High Level flag 'FLAG_NUI_SOME_CHILDREN_DONT_CAPTURE_TOUCH' help us to know if 'pui' has some children that are non-capture touch ui.
			// This flag is updated by critical function like Create/delete etc ... and allow N to avoiding entering in that secondary process
			// if there are no 'non-capture children'
			//

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("+ Get back %d Touch(es) not listen by children.", nevent_move.Touch.TouchSequence.TouchCount);
			start_newline(recursive_count);
			printf("+ It will try to dispatch them to its children that are able to capture Touch Move IN.");
			/*
						start_newline(recursive_count);
						printf("|" );
						printf("+--> First, check these touch(es) against UI Bou." );
			*/
#endif

#ifdef _DEBUG
			dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

			stacksize = 0;
			puichild = (NUI *)pui->ChildrenList.pFirst;
			while (NIS_NODE_VALID(puichild, &pui->ChildrenList))
			{
				// 				if( NIsUIActive(puichild)
				// 					&& (ISFLAG_ON(puichild->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)||FLAGS_TEST(puichild->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) ) )
				if (NIsUIActive(puichild) && (NIsUICaptureTouchMoveIn(puichild) || NIsUICaptureTouchMoveInDummy(puichild)))
				{
					NUIClearWatchStackPush(puichild);
					stacksize++;
				}
				puichild = (NUI *)NGET_NEXT_NODE(puichild);
			}

			NErrorIf(!stacksize, NERROR_SYSTEM_CHECK); // Not possible !!! that means pui->Flags FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN is not properly updated !

#ifdef _NOUT_SYS_EVENT
			start_newline(recursive_count);
			printf("|");
			start_newline(recursive_count);
			printf("+-->%d/%d UIs Pushed from UIChildrenList to NUIClearWatchStack.", stacksize, NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui));
			start_newline(recursive_count);
			printf("|");
			start_newline(recursive_count);
			printf("+-->START UI Pop and Recursive NTOUCH_MOVE (Move In) dispatching for capture.");
			recursive_count++;
#endif

			while (stacksize)
			{
				if (puichild = NUIClearWatchStackPop())
				{

#ifdef _NOUT_SYS_EVENT

					start_newline(recursive_count);
					printf("|");
					start_newline(recursive_count);
					printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, puichild, puichild->pName);
					recursive_count++;
#endif

					_ui_dispatch_touch_move_recursive__capture_move_in(puichild, &nevent_move.Touch.TouchSequence);

#ifdef _NOUT_SYS_EVENT
					recursive_count--;
#endif

					// Stop dispatching if there is no more touches to dispatch !
					if (!nevent_move.Touch.TouchSequence.TouchCount)
					{
						NDecreaseUIClearWatchStackSize(stacksize - 1);
						break;
					}
				}

				stacksize--;
			}

			NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#ifdef _NOUT_SYS_EVENT
			recursive_count--;
#endif
		}
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +
// +						PRIVATE FUNCTIONS
// +
// +
// +
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ------------------------------------------------------------------------------------------
// void NDispatchTouch_Start
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDispatchTouch_Start(const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

	if (ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI))
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + UI FIRST
		// +
		// Pile up all the 'Active UI' Pointers into the 'Clear Watch Stack' !
		// Do this from the last to the first ...

#ifdef _NOUT_SYS_EVENT
		Nu32 local_debug_id;
		debug_id++;
		local_debug_id = debug_id;
		printf("\n |");
		printf("\n +");
		printf("\n[%d] + NDispatchTouchEvent: NTOUCH_START", local_debug_id);
#endif

#ifdef _DEBUG
		Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

		// Pile up all UIs that are:
		//		1/ In the ActiveUIRoot List ( that means root and visible ) !
		//		2/ Enable !
		//		3/ Listen Touch Start AND Not Listen Touch at all !
		//			... that's exactly what the FLAG_TEST is going to check ...
		//			... and at the end of the day, all the FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START/FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
		//			... are going to be ok, excepted FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN set to ON alone.
		Nu32 stacksize = 0;
		Nu32 i;
		NUI **ptr = (NUI **)ActiveUIRootsList.pFirst;
		for (i = ActiveUIRootsList.Size; i != 0; i--, ptr++)
		{
			if (NIsUIEnable(*ptr) /*we are using NIsUIEnable instead of NIsActive because all the ui root are visible !!! */
				&& (!FLAGS_TEST((*ptr)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)))
			{
				NUIClearWatchStackPush(*ptr);
				stacksize++;
			}
		}
// ------------------------------------
// - Debug OutPut
#ifdef _NOUT_SYS_EVENT
		printf("\n\t|");
		printf("\n\t+-->%d/%d Active UIs Pushed from ActiveUIRootsList to NUIClearWatchStack.", stacksize, ActiveUIRootsList.Size);
		printf("\n\t|");
		printf("\n\t+-->START UI Pop and Recursive NTOUCH_START dispatching.");
		printf("\n\t\t+");
		recursive_count = 2;
#endif
		// ------------------------------------

		// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui' (and it's potential linked listener)  if it still exist.
		// ( because it could be deleted during the process ... )
		//		Nu32				child_dispatch_start_return = 0;
		NTOUCH_SEQUENCE ts_in = *pin_streamed_touchseq;
		NUI *pui;
		Nu32 j;

		while (stacksize)
		{
			if ((pui = NUIClearWatchStackPeek())) // Maybe 'pui' was deleted before its own touch dispatching. So check that's not the case !
			{
// ------------------------------------
// - Debug OutPut
#ifdef _NOUT_SYS_EVENT
				printf("\n\t\t|");
				printf("\n\t\t+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
				recursive_count++;
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
#endif
				// ------------------------------------
				if (_ui_dispatch_touch_start_recursive(pui, &ts_in))
				{
					if (((pui = NUIClearWatchStackPop()) && ISFLAG_ON(pui->Flags, FLAG_NUI_SHOW))) // Maybe 'pui' was deleted or hide during its own touch dispatching. So check that's not the case !
					{
						// So, at least one member of the 'pui' forward hierarchy capture some Touch.
						// In that case, UI as to be 'Moved' at the End of the "ActiveUIRootsList"
						// Because of potential previous UI delete, hide, show ... we have to look for it again into the ActiveUIRootList ...
						ptr = (NUI **)ActiveUIRootsList.pFirst;
						for (j = ActiveUIRootsList.Size; j != 0; j--, ptr++)
						{
							if (*ptr == pui)
							{
								NMoveArrayElementBack(&ActiveUIRootsList, ActiveUIRootsList.Size - j);
								break;
							}
						}
					}
				}
				else
				{
					NUIClearWatchStackRemove();
				}

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif

				// Stop dispatching if there is no more touches to dispatch !
				if (!ts_in.TouchCount)
				{
					NDecreaseUIClearWatchStackSize(stacksize - 1); // To remove all the Active UI from the Stack !
					break;
				}
			}
			else
			{
				NUIClearWatchStackRemove();
			}
			stacksize--;
		}

#ifdef _DEBUG
		NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif
#ifdef _NOUT_SYS_EVENT
		printf("\n \n -------------------------------------------------------------------------------------------------- ");
#endif
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THEN GAME STATES
	// +
	//
	//		Send Event to the Current GameState Event Handle hierarchy
	//		Starting from the Event Root GameState and 'descending' along
	//		the current active hierarchy.
	// First, checking GameState Event Dispatch non Blocking to continue.
	if (FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES))
	{
		NEVENT nevent;
		nevent.Head.Type = NTOUCH_START;
		nevent.Head.Param = 0;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		// nevent.Head.pRecipient	= ... setup later for each gamestate
		nevent.Touch.TouchSequence.TouchCount = 0;

		// And Send Event trough the pEventDrivenHandle.
		// Note that is more simple than for UI because GameStates are 'Static'.
		// It's not possible for user to delete one of them at run time...

		// Send Event to Current GameState
		NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
		while (pcurrentstate)
		{
			if (_gamestate_specific__NListenTouchStartSequenceMsk(pcurrentstate, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
			{
				pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
				pcurrentstate->Listener.LTouchCount += nevent.Touch.TouchSequence.TouchCount;
				NErrorIf(pcurrentstate->Listener.LTouchCount != pcurrentstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);

				nevent.Head.pRecipient = pcurrentstate;
				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				nevent.Touch.TouchSequence.TouchCount = 0;
			}

			pcurrentstate = pcurrentstate->pCurrentEventChild;
		}
	}
}

// ------------------------------------------------------------------------------------------
// void NDispatchTouch_Move
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDispatchTouch_Move(const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

#ifdef _DEBUG
	Nu32 dbg_initial_clear_watch_stack_size;
#endif
	Nu32 stacksize;

#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id++;
	local_debug_id = debug_id;
	printf("\n |");
	printf("\n +");
	printf("\n[%d] + NDispatchTouchEvent: NTOUCH_MOVE", local_debug_id);
#endif

// 1) DISPATCH TOUCH MOVE
//
// Pile up all the 'UI linked with Listener' Pointers into the 'Clear Watch Stack' !
#ifdef _DEBUG
	dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

	stacksize = 0;
	NTOUCH_UI_LISTENER *plistener = (NTOUCH_UI_LISTENER *)UIListener_List.pLast;
	while (NIS_NODE_VALID(plistener, &UIListener_List))
	{
		NUIClearWatchStackPush(plistener->pUI);
		stacksize++;
		plistener = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(plistener);
	}

#ifdef _NOUT_SYS_EVENT
	if (stacksize)
	{
		printf("\n\t|");
		printf("\n\t+-->%d/%d UI Listeners Pushed from UIListener_List to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
		printf("\n\t|");
		printf("\n\t+-->START UI Pop and Recursive NTOUCH_MOVE dispatching.");
		printf("\n\t\t+");
	}
	else
	{
		printf("\n\t|");
		printf("\n\t+-->There is NO UI Listeners (UIListener_List is empty).");
		printf("\n\t	NO NTOUCH_MOVE dispatching to UI that already started to listen.");
		printf("\n\t\t+");
	}
	recursive_count = 2;
#endif

	// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui' (and it's linked listener)  if it still exist.
	// ( because it could be deleted during the process ... )
	NUI *pui;
	NTOUCH_SEQUENCE ts_in = *pin_streamed_touchseq;
	//	Nu32	child_dispatch_end_return = 0;
	// Nu32 i;
	while (stacksize)
	{
		if (pui = NUIClearWatchStackPop())
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n\t\t|");
			printf("\n\t\t+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
			recursive_count++;
			start_newline(recursive_count);
			printf("+");
			start_newline(recursive_count);
			printf("|");
#endif

			_ui_dispatch_touch_move_recursive(pui, &ts_in);

#ifdef _NOUT_SYS_EVENT
			recursive_count--;
#endif

			// Stop dispatching because there is no more touches to dispatch !
			if (!ts_in.TouchCount)
			{
				NDecreaseUIClearWatchStackSize(stacksize - 1); // To remove all the Active UI from the Stack !
				break;
			}
		}
		stacksize--;
	}
#ifdef _DEBUG
	NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif

	// 2) DISPATCH TOUCH MOVE as TOUCH START for UIs able to capture touch move In  !
	//
	if (FLAGS_TEST(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN | FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) && ts_in.TouchCount)
	{
#ifdef _NOUT_SYS_EVENT
		printf("\n\t|");
		printf("\n\t+-->Some NTOUCH_MOVE Touch(es) were not captured/Listen by Listened UIs.");
		printf("\n\t	There are some UIs Into the ActiveUIRootsList potentially able to Capture NTOUCH_MOVE IN !");
#endif

// Here 'nevent_move.Touch.TouchSequence' contains all the touches not listen/captured by children of 'pui',  including all the touch
// canceled by 'non-capture children ui' due to a move outside of their pickable shape !
//
// Here we are going to try to dispatch those touches to the 'pui' children which are 'capture touch move in' because they are the only ones
// capable of starting listening them by a MOVE IN !
//
// The High Level flag 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' help us to know if 'pui' has some children that are capture touch move in.
// This flag is updated by critical function like Create/delete etc ... and allow N to avoiding entering in that secondary process
// if there are no 'capture touch move in' children !!!
//
#ifdef _DEBUG
		dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif

		// !!! IMPORTANT TO UNDERSTAND !!!
		// Here we are going to pile up the Active Enabled UI which are "interested" by capturing NTOUCH_MOVE.
		// These kind of UI are:
		//		+ Group 1: The UIs that capture NTOUCH_MOVE ( flag "FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN" set to ON )
		//				... All of these ones, i mean with or without the capabilities to capture NTOUCH_START. ( flag "FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START" set to ON )
		//
		//		+ Group 2: The UIs that have children "interested" by capturing NTOUCH_MOVE.
		//				... BUT ONLY if these UIs ( ... not the children, to be crystal clear ) doesn't capture TOUCH at START !
		//				Why ? Because these ones, are not interested to capture NTOUCH_MOVE ( if they are: they are in Group 1 ...)
		//				So, these kind of UI are not listen TOUCH ( neither at Start or at Move In ) but have children "interested".
		//				In that special UNIQUE case NTOUCH event are going to "pass trough" these UIs which are behave here like "ghosts" or "dummy"
		//				An UI like this is a "CaptureTouchMoveInDummy" !!!
		//				So, to be perfectly clear, a CaptureTouchMoveInDummy is an UI which doesn't capture any TOUCH neither at start and neither at MOVE IN.
		//				... but with children "interested" by capturing NTOUCH_MOVE.
		//				The test below is composed like this:
		//					Is UI Enable ?
		//					and
		//					[
		//						Is UI capture Touch Move In for itself ?
		//						or
		//						Is UI a CaptureTouchMoveInDummy ? ( an UI that doesn't capture Touch start or move but with children "interested" in ).
		//					]
		//				NIsUICaptureTouchMoveIn		 = ISFLAG_ON((*ptr)->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)
		//				NIsUICaptureTouchMoveInDummy = FLAGS_TEST((*ptr)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
		//
		stacksize = 0;
		Nu32 i;
		NUI **ptr = (NUI **)ActiveUIRootsList.pFirst;
		for (i = ActiveUIRootsList.Size; i != 0; i--, ptr++)
		{
			if (NIsUIEnable(*ptr) && (ISFLAG_ON((*ptr)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) || FLAGS_TEST((*ptr)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN)))
			{
				NUIClearWatchStackPush(*ptr);
				stacksize++;
			}
		}

#ifdef _NOUT_SYS_EVENT
		if (stacksize)
		{
			printf("\n\t|");
			printf("\n\t+-->%d/%d Active UI Pushed from ActiveUIRootsList to NUIClearWatchStack.", stacksize, ActiveUIRootsList.Size);
			printf("\n\t	These UI(s) may capture NTOUCH_MOVE (IN) or their children do.");
			printf("\n\t|");
			printf("\n\t+-->START UI Pop and Recursive Capture NTOUCH_MOVE (IN)  dispatching.");
			printf("\n\t\t+");
			recursive_count = 2;
		}
		else
		{
			printf("\n\t|");
			printf("\n\t+-->No Active UI were pushed to NUIClearWatchStack.");
			printf("\n\t	(The ones that potentially may capture NTOUCH_MOVE (IN) should be disable ).");
			printf("\n\t\t+");
		}
#endif

		while (stacksize)
		{
			if (pui = NUIClearWatchStackPeek()) // Maybe 'pui' was deleted before its own touch dispatching. So check that's not the case !
			{
#ifdef _NOUT_SYS_EVENT
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
				start_newline(recursive_count);
				printf("+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
				recursive_count++;
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");

#endif

				if (_ui_dispatch_touch_move_recursive__capture_move_in(pui, &ts_in))
				{
					if ((pui = NUIClearWatchStackPop()) && ISFLAG_ON(pui->Flags, FLAG_NUI_SHOW)) // Maybe 'pui' was deleted or hide during its own touch dispatching. So check that's not the case !
					{
						// So, at least one member of the 'pui' forward hierarchy capture some Touch.
						// In that case, UI as to be 'Moved' at the End of the "ActiveUIRootsList"
						// Because of potential previous UI delete, hide, show ... we have to look for it again into the ActiveUIRootList ...
						ptr = (NUI **)ActiveUIRootsList.pFirst;
						for (i = ActiveUIRootsList.Size; i != 0; i--, ptr++)
						{
							if (*ptr == pui)
							{
								NMoveArrayElementBack(&ActiveUIRootsList, ActiveUIRootsList.Size - i);
								break;
							}
						}
					}
				}
				else
				{
					NUIClearWatchStackRemove();
				}

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif

				// Stop dispatching if there is no more touches to dispatch !
				if (!ts_in.TouchCount)
				{
					NDecreaseUIClearWatchStackSize(stacksize - 1); // To remove all the Active UI from the Stack !
					break;
				}
			}
			else
			{
				NUIClearWatchStackRemove();
			}

			stacksize--;
		}

#ifdef _DEBUG
		NErrorIf(dbg_initial_clear_watch_stack_size != NGetUIClearWatchStackSize(), NERROR_SYSTEM_CHECK);
#endif

#ifdef _NOUT_SYS_EVENT
		printf("\n \n -------------------------------------------------------------------------------------------------- ");
#endif
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THEN GAME STATES
	// +
	//
	//		Send Event to the Current GameState Event Handle hierarchy
	//		Starting from the Event Root GameState and 'descending' along
	//		the current active hierarchy.
	// When FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES is set to ON, all current Listened Touch should be canceled, so ...
	// "NGameStateListenedTouchIDArray.Size" should be NULL

	// And Send Event trough the pEventDrivenHandle.
	// Note that is more simple than for UI because GameStates are 'Static'.
	// It's not possible for user to delete one of them at run time...
	// ... Note that only the Listened Touch started previously with a NTOUCH_START EVENT are going to be listen.
	// ( UI capability to BLOCK Event dispatching is not checked here because this CAPABILITY is ONLY blocking START not interrupting
	// an already STARTED touch ).

	if (ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_GAMESATE_TOUCH_MOVE_ADVANCED_DISPATCH))
	{
		NEVENT nevent;
		nevent.Head.Type = NTOUCH_MOVE;
		nevent.Head.Param = 0;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		nevent.Touch.TouchSequence.TouchCount = 0;
		// Send Event to Current GameState
		NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
		while (pcurrentstate)
		{
			if (NListenTouchMoveSequence(&pcurrentstate->Listener, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
			{
				nevent.Head.pRecipient = pcurrentstate;

				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT

				nevent.Touch.TouchSequence.TouchCount = 0;
			}
			pcurrentstate = pcurrentstate->pCurrentEventChild;
		}
	}
	else
	{
		NEVENT nevent_start, nevent_move, nevent_cancel;
		nevent_start.Head.Type = NTOUCH_START;
		nevent_start.Head.Param = NEVENT_PARAM_TOUCH_START__TOUCH_MOVE_IN;
		nevent_start.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		nevent_start.Touch.TouchSequence.TouchCount = 0;

		nevent_move.Head.Type = NTOUCH_MOVE;
		nevent_move.Head.Param = 0;
		nevent_move.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		nevent_move.Touch.TouchSequence.TouchCount = 0;

		nevent_cancel.Head.Type = NTOUCH_CANCEL;
		nevent_cancel.Head.Param = NEVENT_PARAM_TOUCH_CANCEL__TOUCH_MOVE_OUT;
		nevent_cancel.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		nevent_cancel.Touch.TouchSequence.TouchCount = 0;

		// nevent.Touch.pRecipient	= NULL; useless due to 'Nmem0'
		//  Send Event to Current GameState
		NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
		while (pcurrentstate)
		{
			_gamestate_specific__NListenTouchMoveSequence(&pcurrentstate->Listener, &nevent_start.Touch.TouchSequence, &nevent_move.Touch.TouchSequence, &nevent_cancel.Touch.TouchSequence, pin_streamed_touchseq);

			if (nevent_cancel.Touch.TouchSequence.TouchCount)
			{
				pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
				pcurrentstate->Listener.LTouchCount -= nevent_cancel.Touch.TouchSequence.TouchCount;
				// A call to 'NCleanTouchListenerUp' is requested
				FLAG_ON(pcurrentstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

				nevent_cancel.Head.pRecipient = pcurrentstate;
				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent_cancel, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				nevent_cancel.Touch.TouchSequence.TouchCount = 0;
			}

			if (nevent_move.Touch.TouchSequence.TouchCount)
			{
				nevent_move.Head.pRecipient = pcurrentstate;
				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent_move, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				nevent_move.Touch.TouchSequence.TouchCount = 0;
			}

			if (nevent_start.Touch.TouchSequence.TouchCount)
			{
				pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
				pcurrentstate->Listener.LTouchCount += nevent_start.Touch.TouchSequence.TouchCount;

				nevent_start.Head.pRecipient = pcurrentstate;
				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent_start, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT
				nevent_start.Touch.TouchSequence.TouchCount = 0;
			}

			if (ISFLAG_ON(pcurrentstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN))
				_gamestate_specific__NCleanTouchListenerUp(pcurrentstate);

			pcurrentstate = pcurrentstate->pCurrentEventChild;
		}

		FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_GAMESATE_TOUCH_MOVE_ADVANCED_DISPATCH);
	}
}

// ------------------------------------------------------------------------------------------
// void NDispatchTouch_End
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDispatchTouch_End(const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);
	NErrorIf(!pin_streamed_touchseq->TouchCount, NERROR_NULL_VALUE);

#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id++;
	local_debug_id = debug_id;
	printf("\n |");
	printf("\n +");
	printf("\n[%d] + NDispatchTouchEvent: NTOUCH_END", local_debug_id);
#endif

// Pile up all the 'UI linked with Listener' Pointers into the 'Clear Watch Stack' !
#ifdef _DEBUG
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif
	Nu32 stacksize = 0;
	NTOUCH_UI_LISTENER *plistener = (NTOUCH_UI_LISTENER *)UIListener_List.pLast;
	while (NIS_NODE_VALID(plistener, &UIListener_List))
	{
		NUIClearWatchStackPush(plistener->pUI);
		stacksize++;
		plistener = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(plistener);
	}

#ifdef _NOUT_SYS_EVENT
	if (stacksize)
	{
		printf("\n\t|");
		printf("\n\t+-->%d/%d UI Listeners Pushed from UIListener_List to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
		printf("\n\t|");
		printf("\n\t+-->START UI Pop and Recursive NTOUCH_END dispatching.");
		printf("\n\t\t+");
	}
	else
	{
		printf("\n\t|");
		printf("\n\t+-->There is NO UI Listeners (UIListener_List is empty).");
		printf("\n\t	NO NTOUCH_END dispatching to UI that already started to listen.");
		printf("\n\t\t+");
	}
	recursive_count = 2;
#endif

	// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui' (and it's linked listener)  if it still exist.
	// ( because it could be deleted during the process ... )
	NUI *pui;
	//	Nu32	child_dispatch_end_return = 0;
	NTOUCH_SEQUENCE ts_in = *pin_streamed_touchseq;
	while (stacksize)
	{
		if (pui = NUIClearWatchStackPop())
		{
#ifdef _NOUT_SYS_EVENT
			printf("\n\t\t|");
			printf("\n\t\t+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
			recursive_count++;
			start_newline(recursive_count);
			printf("+");
			start_newline(recursive_count);
			printf("|");
#endif

			_ui_dispatch_touch_end_recursive(pui, &ts_in);

#ifdef _NOUT_SYS_EVENT
			recursive_count--;
#endif

			// Stop dispatching because there is no more touches to dispatch !
			if (!ts_in.TouchCount)
			{
				NDecreaseUIClearWatchStackSize(stacksize - 1); // To remove all the Active UI from the Stack !
				break;
			}
		}
		stacksize--;
	}
#ifdef _DEBUG
	NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THEN GAME STATES
	// +
	//
	//		Send Event to the Current GameState Event Handle hierarchy
	//		Starting from the Event Root GameState and 'descending' along
	//		the current active hierarchy.

	// When FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES is set to ON, all current Listened Touch should be canceled, so ...
	// "NGameStateListenedTouchIDArray.Size" should be NULL
	// First, checking there are some listened touch before continuing.
	// And Send Event trough the pEventDrivenHandle.
	// Note that is more simple than for UI because GameStates are 'Static'.
	// It's not possible for user to delete one of them at run time...
	// ... Note that only the Listened Touch started previously with a NTOUCH_START EVENT are going to be listen.
	// ( UI capability to BLOCK Event dispatching is not checked here because this CAPABILITY is ONLY blocking START not interrupting
	// an already STARTED touch ).
	NEVENT nevent;
	Nmem0(&nevent, NEVENT);
	nevent.Head.Type = NTOUCH_END;
	nevent.Head.Param = 0;
	nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
	// nevent.Touch.pUIRecipient	= NULL; useless due to 'Nmem0'
	//  Send Event to Current GameState
	NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
	while (pcurrentstate)
	{
		if (_gamestate_specific__NListenTouchEndSequence(pcurrentstate, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
		{
			NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK); //'_gamestate_update_and_extract_listened_touchseq_endcancel' should return 0 in that case !!!
			pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
			pcurrentstate->Listener.LTouchCount -= nevent.Touch.TouchSequence.TouchCount;
			// A call to 'NCleanTouchListenerUp' is requested
			FLAG_ON(pcurrentstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			nevent.Head.pRecipient = pcurrentstate;
			// SEND EVENT
			// ------------------------------------------------------------------------------------------------
			pcurrentstate->pEventDrivenHandle(&nevent, pcurrentstate->User_Nu32);
			// ------------------------------------------------------------------------------------------------
			// SEND EVENT

			_gamestate_specific__NCleanTouchListenerUp(pcurrentstate);
			nevent.Touch.TouchSequence.TouchCount = 0;
		}
		pcurrentstate = pcurrentstate->pCurrentEventChild;
	}
}
// ------------------------------------------------------------------------------------------
// void NDispatchTouch_Cancel
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDispatchTouch_Cancel(const NTOUCH_SEQUENCE *pin_streamed_touchseq)
{
	NErrorIf(!pin_streamed_touchseq, NERROR_NULL_POINTER);

#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id++;
	local_debug_id = debug_id;
	printf("\n |");
	printf("\n +");
	printf("\n[%d] + NDispatchTouchEvent: NTOUCH_CANCEL", local_debug_id);
#endif

// Pile up all the 'UI linked with Listener' Pointers into the 'Clear Watch Stack' !
#ifdef _DEBUG
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif
	Nu32 stacksize = 0;
	NTOUCH_UI_LISTENER *plistener = (NTOUCH_UI_LISTENER *)UIListener_List.pLast;
	while (NIS_NODE_VALID(plistener, &UIListener_List))
	{
		NUIClearWatchStackPush(plistener->pUI);
		stacksize++;
		plistener = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(plistener);
	}

	NUI *pui;
	// CANCEL ALL without anymore details
	// ( Its possible from the Queue, when a buffer overflow occurs ... )
	if (!pin_streamed_touchseq->TouchCount)
	{
#ifdef _NOUT_SYS_EVENT
		if (stacksize)
		{
			printf("\n\t|");
			printf("\n\t+-->%d/%d UI Listeners Pushed from UIListener_List to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
			printf("\n\t|");
			printf("\n\t+-->START UI Pop and Recursive NTOUCH_CANCEL ALL dispatching.");
			printf("\n\t\t+");
		}
		else
		{
			printf("\n\t|");
			printf("\n\t+-->There is NO UI Listeners (UIListener_List is empty).");
			printf("\n\t	NO NTOUCH_CANCEL ALL dispatching to UI that already started to listen.");
			printf("\n\t\t+");
		}
		recursive_count = 2;
#endif

		// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui' (and it's linked listener)  if it still exist.
		// ( because it could be deleted during the process ... )
		while (stacksize)
		{
			if (pui = NUIClearWatchStackPop())
			{
#ifdef _NOUT_SYS_EVENT
				printf("\n\t\t|");
				printf("\n\t\t+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
				recursive_count++;
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
#endif

				_ui_dispatch_touch_cancel_all_recursive(pui);

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif
			}
			stacksize--;
		}
	}
	// "NORMAL" CANCEL of with some touches description
	else
	{

#ifdef _NOUT_SYS_EVENT
		if (stacksize)
		{
			printf("\n\t|");
			printf("\n\t+-->%d/%d UI Listeners Pushed from UIListener_List to NUIClearWatchStack.", stacksize, NGetNodeListCount(&UIListener_List));
			printf("\n\t|");
			printf("\n\t+-->START UI Pop and Recursive NTOUCH_CANCEL dispatching.");
			printf("\n\t\t+");
		}
		else
		{
			printf("\n\t|");
			printf("\n\t+-->There is NO UI Listeners (UIListener_List is empty).");
			printf("\n\t	NO NTOUCH_CANCEL dispatching to UI that already started to listen.");
			printf("\n\t\t+");
		}
		recursive_count = 2;
#endif

		NTOUCH_SEQUENCE ts_in = *pin_streamed_touchseq;
		// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui' (and it's linked listener)  if it still exist.
		// ( because it could be deleted during the process ... )
		while (stacksize)
		{
			if (pui = NUIClearWatchStackPop())
			{
#ifdef _NOUT_SYS_EVENT
				printf("\n\t\t|");
				printf("\n\t\t+-->UI#%d[0x%p][%s] pops from Stack.", stacksize, pui, pui->pName);
				recursive_count++;
				start_newline(recursive_count);
				printf("+");
				start_newline(recursive_count);
				printf("|");
#endif

				_ui_dispatch_touch_cancel_recursive(pui, &ts_in);

#ifdef _NOUT_SYS_EVENT
				recursive_count--;
#endif

				// Stop dispatching because there is no more touches to dispatch !
				if (!ts_in.TouchCount)
				{
					NDecreaseUIClearWatchStackSize(stacksize - 1); // To remove all the Active UI from the Stack !
					break;
				}
			}
		}
	}
#ifdef _DEBUG
	NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THEN GAME STATES
	// +
	//
	//		Send Event to the Current GameState Event Handle hierarchy
	//		Starting from the Event Root GameState and 'descending' along
	//		the current active hierarchy.

	// When FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES is set to ON, all current Listened Touch should be canceled, so ...
	// "NGameStateListenedTouchIDArray.Size" should be NULL
	// First, checking there are some listened touch before continuing.
	NEVENT nevent;
	Nmem0(&nevent, NEVENT);
	nevent.Head.Type = NTOUCH_CANCEL;
	// nevent.Head.Param			= 0; To be define later
	nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
	// nevent.Touch.pUIRecipient	= NULL; To be define later

	// And Send Event trough the pEventDrivenHandle.
	// Note that is more simple than for UI because GameStates are 'Static'.
	// It's not possible for user to delete one of them at run time...

	// ... Note that only the Listened Touch started previously with a NTOUCH_START EVENT are going to be listen.
	// ( UI capability to BLOCK Event dispatching is not checked here because this CAPABILITY is ONLY blocking START not interrupting
	// an already STARTED touch ).
	NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
	// CANCEL ALL !!!
	if (!pin_streamed_touchseq->TouchCount)
	{
		while (pcurrentstate)
		{
			// Kill all GameState Listened Touch
			// Building the Event Touch sequence
			NErrorIf(pcurrentstate->Listener.LTouchCount != pcurrentstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
			if (pcurrentstate->Listener.LTouchCount)
			{
				NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);
				pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
				pcurrentstate->Listener.LTouchCount = 0;
				// A call to 'NCleanTouchListenerUp' is requested
				FLAG_ON(pcurrentstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

				nevent.Touch.TouchSequence.TouchCount = 0; // CANCEL ALL !
				nevent.Head.Param = NEVENT_PARAM_TOUCH_CANCEL__ALL;
				nevent.Head.pRecipient = pcurrentstate;

				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT

				_gamestate_specific__NCleanTouchListenerUp(pcurrentstate);
			}

			pcurrentstate = pcurrentstate->pCurrentEventChild;
		}
	}
	// CANCEL ... Some
	else
	{
		nevent.Touch.TouchSequence.TouchCount = 0;

		// Send Event to Current GameState
		while (pcurrentstate)
		{
			if (_gamestate_specific__NListenTouchCancelSequence(pcurrentstate, &nevent.Touch.TouchSequence, pin_streamed_touchseq))
			{
				NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);
				pcurrentstate->Listener.PreviousLTouchCount = pcurrentstate->Listener.LTouchCount;
				pcurrentstate->Listener.LTouchCount -= nevent.Touch.TouchSequence.TouchCount;
				// A call to 'NCleanTouchListenerUp' is requested
				FLAG_ON(pcurrentstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

				nevent.Head.pRecipient = pcurrentstate;
				// SEND EVENT
				// ------------------------------------------------------------------------------------------------
				pcurrentstate->pEventDrivenHandle(&nevent, pcurrentstate->User_Nu32);
				// ------------------------------------------------------------------------------------------------
				// SEND EVENT

				_gamestate_specific__NCleanTouchListenerUp(pcurrentstate);

				nevent.Touch.TouchSequence.TouchCount = 0;
			}
			pcurrentstate = pcurrentstate->pCurrentEventChild;
		}
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +
// +						FUNCTIONS
// +
// +
// +
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void _ui_push_root_ui_listeners_to_clear_watch_stack(const NUI *proot)
{
	NUI *pui = (NUI *)proot->ChildrenList.pLast;
	while (NIS_NODE_VALID(pui, &proot->ChildrenList))
	{
		if (pui->pTouchUIListener)
			NUIClearWatchStackPush(pui);
		else
			_ui_push_root_ui_listeners_to_clear_watch_stack(pui);

		pui = (NUI *)NGET_PREV_NODE(pui);
	}
}
// ------------------------------------------------------------------------------------------
// void NUITouchCancelAll
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUITouchCancelAll(NUI *pui, const Nbool bthis_ui_included)
{

	// !!!! IMPORTANT !!!!
	// UIListener hierarchy is an optimized version of UI hierarchy !!!
	// Remember UIListener Hierarchy is build from Last to First UI Hierarchy parsing.
	// Example:
	//
	//		 UI HIERARCHY 							UILISTENER HIERARCHY(s)
	//  (with linked Listener)				( optimized for touch event dispatch )
	//
	//		   UI#2	(ActiveRootList Last UI)
	//			|
	//			o--UI#20								Listener#0
	//			|											|
	//			o--UI#21 + Listener#2						o--Listener#1
	//			|	|
	//			|	o--UI#210 + Listener#5
	//			|	|									Listener#2
	//			|	o--UI#211 + Listener#4					|
	//			|	|										o--Listener#3
	//			|	o--UI#212								|
	//			|		|									o--Listener#4
	//			|		o--UI#2120							|
	//			|		|									o--Listener#5
	//			|		o--UI#2121 + Listener#3
	//			|
	//			o--UI#22 + Listener#0
	//				|
	//				O--UI#220 + Listener#1
	//
	//
	//		   UI#1 + Listener#6						Listener#6
	//			|											|
	//			o--UI#10									o--Listener#7
	//			|
	//			o--UI#11 + Listener#7
	//
	//
	//		   UI#0	(ActiveRootList First UI)
	//			|
	//			o--UI#00
	//			|
	//			o--UI#01
	//
	if (pui->pTouchUIListener)
	{
		if (bthis_ui_included)
		{
			_ui_dispatch_touch_cancel_all_recursive(pui);
		}
		else
		{
			NTOUCH_UI_LISTENER *plistener = pui->pTouchUIListener;

			//  ... Continue Deeper if there are some children to dispatch to (obviously ! )
			if (NIS_NODE_VALID(plistener->ChildrenList.pLast, &plistener->ChildrenList))
			{
// Pile up all the 'UI CHildren' Pointers into the 'Clear Watch Stack' !
// Do this from the last to the first ...
#ifdef _DEBUG
				Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif
				Nu32 stacksize = 0;
				NTOUCH_UI_LISTENER *plistenerchild = (NTOUCH_UI_LISTENER *)plistener->ChildrenList.pLast;
				while (NIS_NODE_VALID(plistenerchild, &plistener->ChildrenList))
				{
					NErrorIf(!plistenerchild->pUI, NERROR_SYSTEM_CHECK);
					NUIClearWatchStackPush(plistenerchild->pUI);
					stacksize++;

					plistenerchild = (NTOUCH_UI_LISTENER *)NGET_PREV_NODE(plistenerchild);
				}

				// Parse the appropriate 'Clear Watch Stack' range of pointers to call each 'ui'linked with listenerchild if it still exist.
				// ( because it could be deleted during the process ... )
				// Call them from the first to the last in that order ..; that means from the Last UI Children to the First ( due to the way they wera pushed into the array)
				//
				// According with uiresult, send CANCEL EVENT
				NUI *puichild;
				while (stacksize)
				{
					if (puichild = NUIClearWatchStackPop())
					{
						_ui_dispatch_touch_cancel_all_recursive(puichild);
					}
					stacksize--;
				}
#ifdef _DEBUG
				NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif
			}
		}
	}
	else
	{
		// Push to ClearWatchStack all the "Root Listeners" concerned
		// They are all linked with children of UI so _ui_call_touch_cancel_all(pui);

#ifdef _DEBUG
		Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
#endif
		Nu32 stacksize = NGetUIClearWatchStackSize();
		_ui_push_root_ui_listeners_to_clear_watch_stack(pui);

		stacksize = NGetUIClearWatchStackSize() - stacksize;
		NUI *puichild_with_root_listener;
		while (stacksize)
		{
			if (puichild_with_root_listener = NUIClearWatchStackPop())
			{
				_ui_dispatch_touch_cancel_all_recursive(puichild_with_root_listener);
			}
			stacksize--;
		}

#ifdef _DEBUG
		NErrorIf(NGetUIClearWatchStackSize() != dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
#endif
	}
}

void NGameStateTouchCancelAll(const NSTATEUID_ENUM stateid)
{
	NGAMESTATE *pstate = &NGameStateMachine.pGameStatesList[stateid];

	// NErrorIf(pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK); // not a good idea because of potential nested cancel all

	if (pstate->Listener.LTouchCount && pstate->pEventDrivenHandle)
	{
		NErrorIf(pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
		NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);

		NEVENT nevent;
		nevent.Type = NTOUCH_CANCEL;
		nevent.Head.Param = NEVENT_PARAM_TOUCH_CANCEL__ALL;
		nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		nevent.Touch.TouchSequence.TouchCount = 0; // CANCEL ALL !

		pstate->Listener.PreviousLTouchCount = pstate->Listener.LTouchCount;
		pstate->Listener.LTouchCount = 0;
		// A call to 'NCleanTouchListenerUp' is requested
		FLAG_ON(pstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		nevent.Head.pRecipient = pstate;

		// SEND EVENT
		// ------------------------------------------------------------------------------------------------
		pstate->pEventDrivenHandle(&nevent, pstate->User_Nu32);
		// ------------------------------------------------------------------------------------------------
		// SEND EVENT
		_gamestate_specific__NCleanTouchListenerUp(pstate);
	}
}

void NGameStateMachineTouchCancelAll()
{

	NEVENT nevent;
	nevent.Type = NTOUCH_CANCEL;
	nevent.Head.Param = NEVENT_PARAM_TOUCH_CANCEL__ALL;
	nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
	nevent.Touch.TouchSequence.TouchCount = 0; // CANCEL ALL !

	NGAMESTATE *pstate = NGameStateMachine.pCurrentEventRootGameState;
	while (pstate)
	{
		// Kill all GameState Listened Touch
		// Building the Event Touch sequence
		if (pstate->Listener.LTouchCount)
		{
			NErrorIf(pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
			NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);
			pstate->Listener.PreviousLTouchCount = pstate->Listener.LTouchCount;
			pstate->Listener.LTouchCount = 0;
			// A call to 'NCleanTouchListenerUp' is requested
			FLAG_ON(pstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			nevent.Head.pRecipient = pstate;

			// SEND EVENT
			// ------------------------------------------------------------------------------------------------
			pstate->pEventDrivenHandle(&nevent, pstate->User_Nu32);
			// ------------------------------------------------------------------------------------------------
			// SEND EVENT

			_gamestate_specific__NCleanTouchListenerUp(pstate);
		}

		pstate = pstate->pCurrentEventChild;
	}
}

void NGameStateTouchCancelAllForwardHierarchy(const NSTATEUID_ENUM stateid, const Nbool bthis_state_included)
{
	NEVENT nevent;
	nevent.Type = NTOUCH_CANCEL;
	nevent.Head.Param = NEVENT_PARAM_TOUCH_CANCEL__ALL;
	nevent.Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
	nevent.Touch.TouchSequence.TouchCount = 0; // CANCEL ALL !

	// The First State
	NGAMESTATE *pstate = &NGameStateMachine.pGameStatesList[stateid];
	if (bthis_state_included)
	{
		if (pstate->Listener.LTouchCount && pstate->pEventDrivenHandle)
		{
			NErrorIf(pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
			NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);
			pstate->Listener.PreviousLTouchCount = pstate->Listener.LTouchCount;
			pstate->Listener.LTouchCount = 0;
			// A call to 'NCleanTouchListenerUp' is requested
			FLAG_ON(pstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			nevent.Head.pRecipient = pstate;

			// SEND EVENT
			// ------------------------------------------------------------------------------------------------
			pstate->pEventDrivenHandle(&nevent, pstate->User_Nu32);
			// ------------------------------------------------------------------------------------------------
			// SEND EVENT
			_gamestate_specific__NCleanTouchListenerUp(pstate);
		}
	}

	// The Forward hierarchy Ones ...
	pstate = pstate->pCurrentEventChild;
	while (pstate)
	{
		// Kill all GameState Listened Touch
		// Building the Event Touch sequence
		NErrorIf(pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
		if (pstate->Listener.LTouchCount)
		{
			NErrorIf(ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES), NERROR_SYSTEM_CHECK);
			pstate->Listener.PreviousLTouchCount = pstate->Listener.LTouchCount;
			pstate->Listener.LTouchCount = 0;
			// A call to 'NCleanTouchListenerUp' is requested
			FLAG_ON(pstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			nevent.Head.pRecipient = pstate;

			// SEND EVENT
			// ------------------------------------------------------------------------------------------------
			pstate->pEventDrivenHandle(&nevent, pstate->User_Nu32);
			// ------------------------------------------------------------------------------------------------
			// SEND EVENT

			_gamestate_specific__NCleanTouchListenerUp(pstate);
		}

		pstate = pstate->pCurrentEventChild;
	}
}
