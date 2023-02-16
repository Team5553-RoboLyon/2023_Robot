#pragma once

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					App_GameState_Main_HandleFunctions.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/NStructure.h"
#include "lib/N/GameLoop/NGameStates.h"
#include "lib/N/Core/NTime.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // ************************************************************************************
    // **		 					 Functions											 **
    // ************************************************************************************

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // + STATE_MAIN
    void App_GameState_Main_StartHandle(const NSTATEUID_ENUM previous_uid, const Nu32 UserNu32);
    void App_GameState_Main_EndHandle(const NSTATEUID_ENUM next_uid, const Nu32 UserNu32);
    void App_GameState_Main_TimeHandle(const NTIME *ptime, const Nu32 user_Nu32);
    void App_GameState_Main_EventHandle(const NEVENT *pevent, const Nu32 user_Nu32);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
