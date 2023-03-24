#ifndef __NCORE_H
#define __NCORE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NCore.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Miscellaneous/NColor.h"
#include "./Miscellaneous/NColorGradient.h"
#include "./Geometry/NGeometry.h"
#include "./GL/Ngl.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Core Flags											**
// ***************************************************************************************
#define DEFAULT_CLEAR_GRADIENT__NCOLOR0_4F			0.161f,0.039f,0.349f,1.0f				
#define DEFAULT_CLEAR_GRADIENT__NCOLOR1_4F			1.000f,0.486f,0.000f,1.0f				
#define DEFAULT_CLEAR_GRADIENT__NCOLOR2_4F			0.161f,0.039f,0.349f,1.0f								
#define DEFAULT_CLEAR_GRADIENT__NCOLOR3_4F			1.000f,0.486f,0.000f,1.0f					
//#define DEFAULT_CLEAR_GRADIENT__NCOLOR4_4F							

// Clear Flags
#define FLAG_CLEAR_COLOR_BUFFER						BIT_0
#define FLAG_CLEAR_DEPTH_BUFFER						BIT_1
#define FLAG_CLEAR_STENCIL_BUFFER					BIT_2
#define FLAG_CLEAR_SURFACE							BIT_3
#define FLAG_CLEAR_SURFACE_MODULATE					BIT_4

// Core Flags
extern	Nu32								NEngineCoreFlags;


// PRIVATE Engine Flags
// BIT_0 to BIT_1 included need to set as they are because they are linked with File system flags !!!
#define FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH			BIT_0//|! At least one of the two is set to ON. If both are set to ON that means AUTO EVALUATE THE BEST SOLUTION PATH by PATH
#define FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH						BIT_1//|! NEED TO STAY BIT_0 and BIT_1 Like FLAG_NFILE_APP_ROOT_RELATIVE_PATH and FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH !


#define FLAG_NENGINE_CORE_CONTINUE									BIT_2
#define FLAG_NENGINE_CORE_RENDERING									BIT_3
#define FLAG_NENGINE_CORE_UPDATE_TRANSFORMATION_TREE				BIT_4

#define FLAG_NENGINE_CORE_PAUSE_PARTICLES							BIT_5
#define FLAG_NENGINE_CORE_PAUSE_PHYSIC								BIT_6
#define FLAG_NENGINE_CORE_AUDIO_XRAM								BIT_7 // ON if Audio XRAM extension is supported by hardware ("NAudio_Init" has to be called)
#define FLAG_NENGINE_CORE_AUDIO_EFX									BIT_8 // ON if Audio EFX extension is supported by hardware ("NAudio_Init" has to be called)	
#define FLAG_NENGINE_CORE_AUDIO_MUTED								BIT_9 // ON NAudio system is muted OFF is Not

#define FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES					BIT_10 // Set to ON. Event To GameStates Dispatch is blocked.
#define FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI							BIT_11 // Set to ON. Event To UI Dispatch is blocked.
#define FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN			BIT_12
#define FLAG_NENGINE_CORE_GAMESATE_TOUCH_MOVE_ADVANCED_DISPATCH		BIT_13
#define FLAG_NENGINE_CORE_TIMERSTACK_TO_BE_CLEAN					BIT_14

#ifdef _NEDITOR
#define FLAG_NENGINE_CORE_EDITOR_INITIALIZED						BIT_15
#endif
#define FLAG_NENGINE_CORE_GL_INITIALIZED                            BIT_16
#define FLAG_NENGINE_CORE_INITIALIZED								BIT_17

// -------------------------------------------------------------------------------------------------
#define FLAG_NENGINE_CORE_AD_REWARDED_LOADED		                BIT_30
#define FLAG_NENGINE_CORE_AD_INTERSTITIAL_LOADED		            BIT_31


// -----------------------------
// BIT_0 to BIT_1 included need to set as they are because they are linked with File system flags !!!
#define MASK_NENGINE_CORE_PATH_FILESYSTEM							(FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH|FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH)
// -----------------------------
#define MASK_NENGINE_CORE_EVENT_TO_GAMESTATES_BLOCKING			(FLAG_NENGINE_CORE_BLOCK_EVENT_TO_GAMESTATES)
#define MASK_NENGINE_CORE_EVENT_TO_UI_BLOCKING					(FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI)


// Notes: 
// !!! All these flags are PRIVATE. Never use them directly !!!!
// FLAG_NENGINE_CORE_CONTINUE
//	Engine Loop Continue ? Tested after each Engine Loop.
// FLAG_NENGINE_CORE_RENDERING
//	Rendering is in progress. Set to ON at the beginning of DRAW and to OFF at the end. 
// FLAG_NENGINE_CORE_ROOT_RELATIVE_PATHS_AS_DEFAULT
//	open/close file with NFileOpen. "By default", name pass trough the NFileOpen function is considered as Absolute path filename.
//	With this flag set to ON names pass trough the NFileOpen function are going to be considered, "by default", like root relative
//	path filenames. "By default" means if none of the NFileOpen flags "FLAG_NFILE_ROOT_RELATIVE_PATHS or FLAG_NFILE_ABSOLUTE_PATHS"
//	is set to ON. Because these 2 NFILE flags will always have the priority on the default engine statement... (obviously)
// FLAG_NENGINE_CORE_PAUSE_PARTICLES
//	Pause the particles ? set to ON to do it
// FLAG_NENGINE_CORE_PAUSE_PHYSIC	
//	Pause all the physic ? set to ON to do it
// FLAG_NENGINE_CORE_EDITOR_INITIALIZED	
//	Set to ON by the "NInitializeEditor" function and to OFF by the "NQuitEditor" function.




// ***************************************************************************************
// **							  Core Functions										**
// ***************************************************************************************
void				N_Init();
Nbool				N_IsRendering();
void				N_AskToQuit();
void				N_Disable();
Nu32				N_Update();
void				N_Draw();


// Private variable
inline	void		NPauseParticles(){FLAG_ON( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PARTICLES );}
inline	void		NResumeParticles(){FLAG_OFF( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PARTICLES );}

inline	void		NPausePhysic(){FLAG_ON( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PHYSIC );}
inline	void		NResumePhysic(){FLAG_OFF( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PHYSIC );}

inline	void		NPause(){FLAG_ON( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PHYSIC|FLAG_NENGINE_CORE_PAUSE_PARTICLES );}
inline	void		NResume(){FLAG_OFF( NEngineCoreFlags, FLAG_NENGINE_CORE_PAUSE_PHYSIC|FLAG_NENGINE_CORE_PAUSE_PARTICLES );}


// Clear
void				NClear(const Nu32 clear_flags);
inline void			NClearColor(const NCOLOR *pcolor){glClearColor(pcolor->red,pcolor->green,pcolor->blue,pcolor->alpha);}
inline void			NClearColorf(const Nf32 red, const Nf32 green, const Nf32 blue, const Nf32 alpha){glClearColor(red,green,blue,alpha);}

void				NBindClearSurface(NGEOMETRY *pgeometry);
Nu32				NGetClearSurfaceVertexNumber();
void				NGetClearSurfaceVertexColor(NCOLOR *pcolor, const Nu32 index );
NGEOMETRY*			NGetBoundClearSurface();
void				NSetClearSurfaceVertexColor(const Nu32 index, const NCOLOR *pcolor );

void				NSetClearSurfaceAllVertexColor(const NCOLOR *pcolor );
void				NSetClearSurfaceAllVertexColorf(const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a );

void				NSetCLearSurfaceColorModulationf(const Nf32 red,const Nf32 green,const Nf32 blue,const Nf32 alpha );
void				NSetCLearSurfaceColorModulation(const NCOLOR *pcolormod);
void				NGetCLearSurfaceColorModulation(NCOLOR *pcolormod);

// Timer
// void				NInitializeMainTimer();

// File System
inline	void		NSwitchToFullPathFileSystem(){FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH);FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH);}
inline	void		NSwitchToRootRelativePathFileSystem(){FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH);FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH);}
inline	void		NSwitchToAutoPathFileSystem(){FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_FULL_PATH);FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_FILESYSTEM_APP_ROOT_RELATIVE_PATH);}

// FullPathFileSystem: 
//		FileOpen will only accepts FULL PATH FILENAMEas filename. CRASH in debug mode if incoming file name is not.
//		This kind of filename is going to be used as it is without any string manipulation. Filename is suppose to be the 
//		full and absolute one.
//		This approach may cause some problems due to some basic differences between Platforms. WINDOWS accepts FULL PATH from the Root ('c:...')
//		BUT mobile system don't ... 
//
// Application Root relative PathFileSystem:
//		FileOpen will only accepts App.Root Relative FILENAME PATH as filename. CRASH in debug mode if incoming file name is not.
//		This kind of filename is going to be 'augmented' with some extra at the beginning of the string. But inside FileOpen of course.
//		filename itself remains unchanged.
//
// Auto PathFileSystem: 
//		FileOpen will try to figure out which kind of path incoming filename is (Absolute or App.Root Relative) by analyzing the filename.
//		According with the analyze result, filename is going to be considered as an Absolute or as a relative PathName...
//		( Avoid it !!! excepted to get back files from unknown origin that include filenames inside, like texture references for example ... )
//
// inline	void		NSetRootRelativePathSystemAsDefault(){FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_ROOT_RELATIVE_PATHS_AS_DEFAULT);}
// inline	void		NSetAbsolutePathSystemAsDefault(){FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_ROOT_RELATIVE_PATHS_AS_DEFAULT);}
// inline	Nbool		NIsRootRelativeDefaultPathSystem(){return ( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_ROOT_RELATIVE_PATHS_AS_DEFAULT) ? 1:0 );}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NCORE_H 

