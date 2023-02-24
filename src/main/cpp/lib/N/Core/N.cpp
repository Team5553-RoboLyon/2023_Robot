#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NMath.h"
#include "../Event/NEvent.h"
#include "../NFlags.h"
#include "lib/N/NType.h"
#include "../NScreenRect.h"
#include "../NRectf32.h"
#include "../NCamera.h"
#include "../NViewport.h"
#include "../GameLoop/NGameStates.h"
#include "../NTransformationTree.h"
#include "lib/N/Containers/NNode.h"
#include "../Containers/NArray.h"
#include "../BasicElements/NTextureAtlas.h"
#include "../Geometry/NGeometryMisc.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Render/Accumulator/NAccumulator.h"
#include "../Render/Accumulator/NStateSet.h"
#include "../Render/NRenderCell.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

#ifdef _NLIBS
#include "../Libraries/NLibraries.h"
#endif

// #if defined _NIOS || defined _NANDROID
#include "../Event/NEventBuffer.h"
// #endif

#include "../UI/NUI.h"
#include "../UI/NUIClearWatchStack.h"

#include "../Particles/NParticles.h"
#include "../NFont.h"
#include "../NTexture.h"
#include "../NSpline.h"
#include "../Core/NTime.h"
#include "../NPostUpdate.h"
#include "../NStructure.h"
#include "../Render/Renderable/NRenderableScanner.h"
#include "../Render/NFrustum_Culling.h"
#include "lib/N/NErrorHandling.h"
#include "../Utilities/NUT_Shape.h"
#include "../Timer/NTimer.h"
#include "../NCore.h"
#include "../File/NFileExt_Appearance_RWCallBacks.h"
#include "../Touch/NTouch.h"
#include "../Touch/NTouchEmulation.h"

// TEMP
#include "../Collectibles/NCollectibles.h"
// TEMP

// ==========================================================================================
//
// VARIABLES GLOBALES
//
// ==========================================================================================
// static DWORD	VERSION=(6<<16 | 0<<8 | 0);
// static char		Copyright[]={"N V0.0.0 (c) TERAZAN 2012"};
// static char		Author[]={"Author : JJ.TERAZAN"};

// #define			MAINTIMER_ACCURACY	1.0f/1000.0f
//  N GLobals

// TEMP

NTIME MainTimer;
NSTATESET CurrentStateSet;
// NERROR						NLastError;
Nu32 NEngineCoreFlags;
extern NCOLOR NClearSurfaceColorModulation;
extern NARRAY NTimerPtrArray;

/*
#define FLAG_NTOUCH_ACTIVE								BIT_0
#define FLAG_NTOUCH_UI_BLOCK_EVENT_TO_GAMESTATES		BIT_1

typedef struct
{
	Nu32			Flags;
	Nu32			SystemID;
	NVEC2s16		Position;
	NUI				*pFocusedUI;
}NTOUCH;

NTOUCH						NTouch[CONSTANT_NTOUCH_MAX];
*/

// Extern Globals (only accessible by their 'owner.cpp' and N.cpp)
extern NARRAY NTimerArray;
extern NGAMESTATEMACHINE NGameStateMachine;	  // THE 'NGameState' machine
extern NTRANSFORM_HNODE *pTransformationRoot; // Root node of the transformation tree
extern NNODELIST NTransformationTree;		  // Nnodelist of all transformation trees
extern NARRAY ActiveUIRootsList;			  // Array with all the active UI. The one which can be modified during the loop.
extern NNODELIST PostUpdateProcessList;		  // Nnodelist of all postupdate process ... to be DELELETED
extern NPOSTUPDATE *NpPostUpdates;
extern NGEOMETRY *pNClearSurfaceGeometry;

// PRIVATE FUNCTIONS only used by NENGINE INIT and QUIT functions ...
void NInit_ClearSurface();
void NDisable_ClearSurface();

void N_AskToQuit() { FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_CONTINUE); }

// Nbool N_IsRendering(){return bRendering;}
Nbool N_IsRendering()
{
	if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_RENDERING))
		return NTRUE;
	else
		return NFALSE;
}
/*
void NInitializeMainTimer()
{
#ifdef _NWINDOWS
	timeBeginPeriod(1);
	MainTimer.Start = timeGetTime();
	MainTimer.Current = MainTimer.Start;
	MainTimer.Previous = MainTimer.Start;
	MainTimer.Delay = 0;

	MainTimer.fStart = (Nf32)MainTimer.Start * MAINTIMER_ACCURACY;
	MainTimer.fCurrent = MainTimer.fStart;
	MainTimer.fPrevious = MainTimer.fStart;
	MainTimer.Nf32_Delay = 0.0f;
#endif
#ifdef _NIOS
	MainTimer.fStart = (float)CACurrentMediaTime();
	MainTimer.fCurrent = MainTimer.fStart;
	MainTimer.fPrevious = MainTimer.fStart;
	MainTimer.Nf32_Delay = 0.0f;
#endif

#ifdef _NANDROID
TODO
#endif
}

void NResetMainTimer()
{
	#ifdef _NWINDOWS
		timeEndPeriod(1);
	#endif

	#ifdef _NIOS
	TODO
	#endif

	#ifdef _NANDROID
	TODO
	#endif
}
*/

// ------------------------------------------------------------------------------------------
// N_Init
// ------------------------------------------------------------------------------------------
// Description :
//	Initialize all the variables, lists, LUT, objects, used by N
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void N_Init()
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +					!!!	DEBUG ENGINE INIT TESTING SECTION !!!
	// NEngine uses a lot of CONSTANTS and DEFAULT values which are all "#define". Engine version
	// after engine version new CONSTANTS and DEFAULT appear or change, and because some of them could be
	// "connected" in some ways... it's important to test the validity of these values.
	// It's the purpose of this section ... but, for sure, only in debug mode.
#ifdef _DEBUG
	NErrorIf(CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER > CONSTANT_NAPPEARANCE_TEXTUREUNIT_NUMBER_MAX, NERROR_SYSTEM_INVALID_CONSTANT);
#endif
	// +
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// #if defined _NIOS || defined _NANDROID
	NInitEventBuffer();
	// #endif

	// Default FileSystem Path Mode:
	NSwitchToFullPathFileSystem();

	// UI
	NInitUIClearWatchStack();
	// Event
	NInitDelayedUIEventArray();

	NInitializeVertexSpecs();
	NInitializePrimitiveSpecs();
	NInitializeTextureUnitSpecs();
	NInitializeAppearanceSpecs();
	NInitializeGeometrySpecs();

	NInitializeRenderableScannerArray();
	NInitializeMathLUTs();
	// #ifdef _IOS
	//	NInitializeEventQueue();
	// #endif
	NInitializeGameStateMachine();
	NPostUpdatesInitialization();

	// Initialize libraries (if activated with the right compiler flags)
#ifdef _NLIBS
	NInitTexturesLibrary();
	NInitBlendsLibrary();
	NInitMaterialsLibrary();
#endif

	NInitializeAtlas();

	/*
		NInitializeParticleInfluencesList();
		NInitializeParticleObstaclesList();
	*/

	// Touch System
	NInitTouchCore();

	// UI
	//	NInitializeUIStylesList();
	NInitializeFontsList();
	NInitializeActiveUIRootsList();
	NCreateRenderableScanner((void *)&ActiveUIRootsList, NRenderableScannerCallBack_UIs);
	NInit_Accumulators();
	NSetupTime(&MainTimer);
	NInitializeTimerPtrArray();

	NInitialiazeRenderCellsBuffer();

	NTransformationTree_RootInit();

	// ENGINE CORE FLAGS
	// bUpdateTransformationtree = NFALSE;
	// pCurrentUIRoot = NULL;
	// bUpdateContinue = NTRUE;

	FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_UPDATE_TRANSFORMATION_TREE);
	FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_CONTINUE);

	// Opengl has been initialized (it should be)
	// So, we can Pre-calculate ViewPort Constants
	// NInitializeViewPortConstants();

	// and ...
	NInitializePhysicStructures();

	// Functions ptr LUT for setup/saving/loading:
	NInitializeRenderableUpdateFCT_LUT();
	NInitializeRenderableExtractFCT_LUT();

	// NCreateRenderableScanner((void*)&NActivePhysicStructureList,NRenderableScannerCallBack_PhysicStructure);

	NInit_ClearSurface();

	// File System
	NInitRWAppearanceCallBacks();

	// Collectible Set
	NInitCollectiblePicking(8, 3); // find a way to interface these param outside the function N_Init.
								   // a .h with DEFINE used for init MAYBE !

#ifdef _NWINDOWS
	NTouchEmulInit(8); // Preallocation of 8 Touch Emul Structs
#endif

	// END OF INITIALIZATION ...
	FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_INITIALIZED);
}

// ------------------------------------------------------------------------------------------
// N_Disable
// ------------------------------------------------------------------------------------------
// Description :
//	Free properly all the lists allocated and used by N
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void N_Disable()
{
	// Starting by setting the Flag to OFF...
	FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_INITIALIZED);

#ifdef _NWINDOWS
	NTouchEmulQuit();
#endif

	// Collectible Set
	NQuitCollectiblePicking();

	NDisable_ClearSurface();

	NClearTimerPtrArray();
	NClearTime(&MainTimer);

	NDisable_Accumulators();

	NDisableGameStateMachine();
	NPostUpdatesDisable();

	/*
		NDeleteParticleInfluencesList();
		NDeleteParticleObstaclesList();
	*/

	//	NDeleteUIStylesList();
	NDeleteFontsList();
	NClearActiveUIRootsList();
	NDeleteTransformationTree();
	NDeleteRenderCellsBuffer();

	// TODO >>> DELETE all UIframes of the UI list
	// TODO >>> DELETE all GameStates
	// NDeleteUITree();
	// NDeleteGameStates();
	NClearAtlas();
	// Initialize libraries (if activated with the right compiler flags)
#ifdef _NLIBS
	NDisableTexturesLibrary();
	NDisableBlendsLibrary();
	NDisableMaterialsLibrary();
#endif
	// #ifdef _IOS
	//	NClearEventQueue();
	// #endif
	NClearRenderableScannerArray();

	NDisableVertexSpecs();
	NDisablePrimitiveSpecs();
	NDisableTextureUnitSpecs();
	NDisableAppearanceSpecs();
	NDisableGeometrySpecs();

	NQuitDelayedUIEventArray();
	NQuitUIClearWatchStack();

	// Touch System
	NQuitTouchCore();

	// #if defined _NIOS || defined _NANDROID
	NQuitEventBuffer();
	// #endif
}

/*
static inline void _apply_ui_postponedactions()
{
	NUI_POSTPONEDACTION	*paction = NUIPostponedActions.pFirst;
	for(Nu32 i=NUIPostponedActions.Size;i!=0;i--,paction++)
	{
		switch(paction->Type)
		{
			case NUI_PPA_IN:
				break;
			case NUI_PPA_OUT:
				break;
			case NUI_PPA_OUTDEL:
				break;
		}
	}
}
*/

// ------------------------------------------------------------------------------------------
// N_Update
// ------------------------------------------------------------------------------------------
// Description :
//	Update all the N entities
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nu32 N_Update()
{
	Nu32 i;

	NGAMESTATE *pcurrentstate;
	NUI **ptr;

	NPOSTUPDATE *pup;

	// About Update Order:

	//	Driven by		Order) Type
	//  ------------------------------------
	//	EVENT-Delayed	1a) UIs				|
	//	EVENT			1a')UIs				|_User Handles. User can perform important modifications !!!
	//	EVENT			1b) GameStates		|				Everything can happen !!!
	//	TIME			2a) GameStates		|
	//  ------------------------------------
	//	TIME			2b) UIs				|
	//	TIME			2c) Timers			|-Internal System Handles Only.User doesn't have the hand to make modification !!!
	//	TIME			2d)	PostUpdates		|							   All potential generated Events are DELAYED !!!
	//  ------------------------------------							   Everything is under control !!!

	// ......................................................................................
	// EVENT DRIVEN

	// Dispatch all Delayed UI Events
	NDispatchDelayedUIEvent();

	// Dispatch All Queued Events: to UI and GameState

#ifdef _NWINDOWS
	/*
		NEVENT	nevent;
		while( NWindowsPollEvent(&nevent) )
		{
			NDispatchEvent( &nevent );
		}
	*/
	NEVENT *pevent;
	while (pevent = NPollEvent())
	{
		NDispatchEvent(pevent);
		NFreePolledEvent();
	}
#endif

#if defined _NIOS || defined _NANDROID
	NEVENT *pevent;
	while (pevent = NPollEvent())
	{
		NDispatchEvent(pevent);
		NFreePolledEvent();
	}
#endif

	// ......................................................................................
	// TIME DRIVEN
	// ......................................................................................
	// Update MainTimer
	NUpdateTime(&MainTimer);

	// All the TimeDriven Handles of the current GameState are called
	pcurrentstate = NGameStateMachine.pCurrentTimeRootGameState;
	while (pcurrentstate)
	{
		pcurrentstate->pTimeDrivenHandle(&MainTimer, pcurrentstate->User_Nu32);
		pcurrentstate = pcurrentstate->pCurrentTimeChild;
	}

	// Call of all the Current UIState Handles
	// We have to call them now because UiFrame States are very often updating NTransformTree ...
	// So its not possible to call the UIState handle inside the Renderable Update and Extract process which happens
	// AFTER the TransformTree Update.
	ptr = (NUI **)ActiveUIRootsList.pFirst;
	for (i = ActiveUIRootsList.Size; i != 0; i--, ptr++)
	{
		NUpdateUIState_Recursive((NUI *)*ptr, &MainTimer);
	}

	// All the Timer are called:
	//	1)Simple Timers
	//	2)Elapsed Timers
	//	3)TransformationHnodeAnimationPlayers
	if (NTimerPtrArray.Size)
	{
		NTIMER **pptimer = (NTIMER **)NTimerPtrArray.pFirst;
		for (i = NTimerPtrArray.Size; i != 0; i--, pptimer++) // By doing loop in that way, potential nested Timer creation are going to be ignored at this turn ... and that is exactly what we want !
		{
			if (*pptimer)
			{
				(*pptimer)->TimerHandle(*pptimer, &MainTimer);
			}
		}

		// If necessary, clean Timer Stack up ...
		if (ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_TIMERSTACK_TO_BE_CLEAN))
		{
			pptimer = (NTIMER **)NTimerPtrArray.pFirst;
			Nu32 outsize = 0;
			for (i = 0; i < NTimerPtrArray.Size; i++, pptimer++)
			{
				if (*pptimer != NULL) // was not deleted
				{
					(*pptimer)->Index = _SafeNu32ToNu16(outsize);
					((NTIMER **)NTimerPtrArray.pFirst)[outsize] = *pptimer;
					outsize++;
				}
			}
			NTimerPtrArray.Size = outsize;
			FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_TIMERSTACK_TO_BE_CLEAN);
		}
	}

	// ......................................................................................
	// POST UPDATE PROCESS
	// Todo: remove them !!! and replace them by a better use of NUpdateAndExtract_Process
	pup = NpPostUpdates;
	for (i = NPOSTUPDATE_ENUM_SIZE; i != 0; i--, pup++)
	{
		if (ISFLAG_ON(pup->Flags, FLAG_POSTUPDATE_PROCESS_ACTIVE))
			pup->pHandle(&MainTimer, pup->u32_DataA, pup->u32_DataB);
	}
	// ......................................................................................

	NUpdateCollectiblePicking();

	// End of Update.
	// return bUpdateContinue;
	return (NEngineCoreFlags & FLAG_NENGINE_CORE_CONTINUE);
}

// ------------------------------------------------------------------------------------------
// N_Draw
// ------------------------------------------------------------------------------------------
// Description :
//	Draw all the N entities they have to
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void N_Draw()
{
	//	static	NACCUMULATOR_SETUP_DATA		CurrentAccSetUp;
	Nu32 i;
	NRENDERABLESCANNER *pscan;

	// ---------------------------------- BEGINNING OF RENDERING
	// bRendering = NTRUE;
	FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_RENDERING);
	// ==================================================================
	// FIRST OF ALL
	// ==================================================================
	NUpdateTransformationTree();
	// Reset RenderCell Buffer
	pCurrentRenderCell = pRenderCellsBuffer;

	// ==================================================================
	// SCAN FOR FIND ALL RENDERABLE and 'UPDATE AND EXTRACT' ALL OF THEM
	// ==================================================================
	pscan = (NRENDERABLESCANNER *)NRenderableScanner.pFirst;
	for (i = NRenderableScanner.Size; i != 0; i--, pscan++)
	{
		pscan->pScanner(pscan->pEntry, &MainTimer);
	}

	// ==================================================================
	// PHYSIC ENGINE
	// ==================================================================
	// Physic Structures Array was filled up 'each turn' by the 'UpdateAndExtract'
	// callback function linked with each NStructure Renderable ...
	NUpdatePhysicStructures(&MainTimer);

	// ==================================================================
	// Rendering Accumulators Loop
	// ==================================================================
	NACCUMULATOR *pacc = NAccumulator;
	for (i = NACCUM_ID_ENUM_SIZE; i != 0; i--, pacc++)
	{
		if (pacc->AccumulatedRenderCellsArray.Size)
		{
			pacc->RenderAccumulator(pacc);
			pacc->AccumulatedRenderCellsArray.Size = 0; // Cash ! But here we know what we do !
		}
	}
	FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_RENDERING);
	return;
}

void NClear(const Nu32 clear_flags)
{
	GLbitfield gl_flags = 0;

	if (ISFLAG_ON(clear_flags, FLAG_CLEAR_DEPTH_BUFFER))
	{
		if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_ZWRITE))
		{
			FLAG_ON(NCurrentStateSet, FLAG_STATESET_ZWRITE);
			glDepthMask(GL_TRUE);
		}

		gl_flags |= GL_DEPTH_BUFFER_BIT;
	}

	if (ISFLAG_ON(clear_flags, FLAG_CLEAR_COLOR_BUFFER))
	{
		gl_flags |= GL_COLOR_BUFFER_BIT;
	}

	if (ISFLAG_ON(clear_flags, FLAG_CLEAR_STENCIL_BUFFER))
	{
		gl_flags |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(gl_flags);

	if (ISFLAG_ON(clear_flags, FLAG_CLEAR_SURFACE) /*&& pNClearSurfaceGeometry*/)
	{
		NErrorIf(!pNClearSurfaceGeometry, NERROR_CLEAR_SURFACE_NULL);

		// Modulate
		if (ISFLAG_ON(clear_flags, FLAG_CLEAR_SURFACE_MODULATE))
		{
			NModulateGeometryVertexRangeColor(pNClearSurfaceGeometry, NVERTEX_DATA_COLOR0, 0, pNClearSurfaceGeometry->Mesh.VertexArray.Size, &NClearSurfaceColorModulation);
		}

		NUpdateCurrentStateSet(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pNClearSurfaceGeometry->Core.IDS)].StateSet & MASK_STATESET_APPEARANCE_AND_RESERVED);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		// glOrthographicMatrix
		// ---------------------
		//	glOrtho( left,	right,								bottom,	top,	near,	far );
		glOrtho(0.0f, NGetViewPort_AspectRatio(), -1.0f, 0.0f, -1.0f, 1.0f);
		// NOrthoMatrixf(0.0f,NGetViewPort_AspectRatio(),-1.0f,0.0f,-1.0f,1.0f);
		pNClearSurfaceGeometry->Core.RenderFunction(pNClearSurfaceGeometry);
		// Restore Previous Matrix Context
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}
