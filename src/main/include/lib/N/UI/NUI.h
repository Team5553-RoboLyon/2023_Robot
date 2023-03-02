#ifndef __NUI_H
#define __NUI_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUI.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "lib/N/Containers/NNode.h"
#include "../Containers/NHNode.h"
#include "../Maths/NVec3f32.h"
#include "../NScreenPoint.h"
#include "../NRectf32.h"
#include "../NTexture.h"
#include "../NFont.h"
#include "../Ntext.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../Miscellaneous/NVertexTexCoordBuild.h"
#include "../Miscellaneous/NVertexColorBuild.h"

#include "../BasicElements/NTextureAtlas.h"
// #include "../Geometry/Components/NGeometryTextureCoordinate.h"
#include "../NFastAnimatedValue.h"
#include "../Event/NEvent.h"
#include "../UI/NUIPlacement.h"
#include "lib/N/NErrorHandling.h"
#include "../Render/Renderable/NRenderable_ExtractSet.h"
// #include "NUIDescription.h"
#include "../NCoordinateSystemConversions.h"
#include "../Touch/NTouchUIListener.h"
#include "NUIColorSet.h"
#include "NUIStyle.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************
// UI constants
#define CONSTANT_NUI_NAME_SIZEMAX 32 // Maximum number of characters for the UI name null terminating character included.
									 // At UIFRame creation NUI.pName member could be allocated with a size in accordance
									 // with the size of the incoming name. "Could be" means that if there is no incoming name,
									 // then no memory allocation happens.
									 // In case of memory allocation this one will be at maximum at the size of
									 // "CONSTANT_NUI_NAME_SIZEMAX*sizeof(Nchar)"

// UI Default values
// #define DEFAULT_NUI_LISTEN_TOUCH_MODE					NUI_LISTEN_TOUCH_MODE_BASIC
#define DEFAULT_NUI_RENDERABLE_HIGHLEVEL_EXTRACTSET FLAG_NEXTRACTSET_HUD
#define DEFAULT_NUI_RENDERABLE_BOUNDINGSPHERE_RADIUS 1.0f
#define DEFAULT_NUI_GEOMETRY_FORMAT NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE
#define DEFAULT_NUI_GEOMETRY_USERSTATESET 0

// UI Default values
#define DEFAULT_NUI_ACTIVEROOTS_NUMBER 2 // Only used at the creation of the list "ActiveUIRootsList" to initialize it with an initial number of elements
#define DEFAULT_NUI_PICKING_GEOMETRY_TRANSFORMED_VERTEX_ARRAY_CAPACITY 16
#define DEFAULT_NUI_SIZE_X 0.1f
#define DEFAULT_NUI_SIZE_Y 0.1f

#define DEFAULT_NUIPLACEMENT_SIZE_X 1.0f
#define DEFAULT_NUIPLACEMENT_SIZE_Y 1.0f
#define DEFAULT_NUIPLACEMENT_SIZE_Z 1.0f

#define DEFAULT_NUIPLACEMENT_RELATIVEPOSITION_X 0.0f
#define DEFAULT_NUIPLACEMENT_RELATIVEPOSITION_Y 0.0f
#define DEFAULT_NUIPLACEMENT_RELATIVEPOSITION_Z 0.0f

#define DEFAULT_NUIPLACEMENT_ANCHOR_X 0.0f
#define DEFAULT_NUIPLACEMENT_ANCHOR_Y 0.0f
#define DEFAULT_NUIPLACEMENT_ANCHOR_Z 0.0f

#define DEFAULT_NUIPLACEMENT_PARENTANCHOR_X 0.0f
#define DEFAULT_NUIPLACEMENT_PARENTANCHOR_Y 0.0f
#define DEFAULT_NUIPLACEMENT_PARENTANCHOR_Z 0.0f

// ----------------------------------------------------------------------------------------------------------
// UI FLAGS Debug
// ----------------------------------------------------------------------------------------------------------
// UI FLAGS Debug works with 'NUI.Flags_Debug' Only ! ... And, of course, only in debug mode !
#ifdef _DEBUG
#define FLAG_NUI_DEBUG_CLEAR_PROCESS_ENGAGED BIT_0
// #define	FLAG_NUI_DEBUG_ENABLE_DISABLE_PROCESS_ENGAGED		BIT_1
// #define FLAG_NUI_DEBUG_SHOW_HIDE_PROCESS_ENGAGED			BIT_2
#endif

// ----------------------------------------------------------------------------------------------------------
// UI FLAGS RunTime
// ----------------------------------------------------------------------------------------------------------
// UI FLAGS works with 'NUI.FlagsXtd' Only !
#define FLAG_NUI_XTD_BACKGROUND BIT_0
#define FLAG_NUI_XTD_ICON BIT_1
#define FLAG_NUI_XTD_CUSTOM_PICKING_MESH BIT_2
#define FLAG_NUI_XTD_TEXT_PICKING_MESH BIT_3
#define FLAG_NUI_XTD_PRECISE_PICKING BIT_4
#define FLAG_NUI_XTD_NAME_LOCKED BIT_5				   // not used ... may be removed
#define FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL BIT_6   // if ON  UI will be showed automatically when it's parent will be.
#define FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL BIT_7 // if ON, UI will be enable automatically when it's parent will be.
// #define	FLAG_NUI_XTD_xxx										BIT_8		//
// #define	FLAG_NUI_XTD_xxx										BIT_9		//
// #define	FLAG_NUI_XTD_xxx										BIT_10		//
// #define	FLAG_NUI_XTD_xxx										BIT_11		//
// #define	FLAG_NUI_XTD_xxx										BIT_12		//
// #define	FLAG_NUI_XTD_xxx										BIT_13		//
// #define	FLAG_NUI_XTD_TRANSFORM_HNODE_UPDATE_TAG					BIT_14		//
// #define	FLAG_NUI_XTD_BBOX_UPDATE_ONCE							BIT_15		//

// ----------------------------------------------------------------------------------------------------------
// UI FLAGS
// ----------------------------------------------------------------------------------------------------------
// UI FLAGS works with 'NUI.Flags' Only !
// Activation
#define FLAG_NUI_SHOW BIT_0	  //
#define FLAG_NUI_ENABLE BIT_1 //
// Specific
#define FLAG_NUI_TRANSFORM_HNODE_UPDATE_TAG BIT_2 // !!! HAS TO BE "BIT_2" LIKE 'FLAG_NTRANSFORM_HNODE_UPDATE_TAG' !!!
// Listen/Intercept EVENT
#define FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT BIT_3 // ON means update flags "FLAG_NTOUCH_IS_IN/FLAG_NTOUCH_WAS_IN" and "UIListener.LTouchCountIn" at move. ( old flag name was: FLAG_NUI_LISTEN_TOUCH_EVENT_UNDER_CONTROL )
#define FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START BIT_4		// ON means capture TOUCH at start ONLY.			OFF means doesn't capture TOUCH at start.
#define FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN BIT_5	// ON means capture TOUCH at move if TOUCH is IN.	OFF means doesn't capture TOUCH at move.
#define FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT BIT_6	// ON means release TOUCH at move if TOUCH is OUT.	OFF means doesn't release at TOUCH at move.

#define FLAG_NUI_INTERCEPT_TOUCH_EVENT BIT_7
#define FLAG_NUI_LISTEN_UICORE_EVENT BIT_8
#define FLAG_NUI_INTERCEPT_UICORE_EVENT BIT_9
#define FLAG_NUI_LISTEN_UICOMMAND_EVENT BIT_10
#define FLAG_NUI_INTERCEPT_UICOMMAND_EVENT BIT_11
#define FLAG_NUI_LISTEN_UINOTIFY_EVENT BIT_12
#define FLAG_NUI_INTERCEPT_UINOTIFY_EVENT BIT_13
#define FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN BIT_14

#define FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES BIT_15
#define FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN BIT_16
// BBox Update
#define FLAG_NUI_BBOX_UPDATE_IF_THN_CHANGED BIT_17
#define FLAG_NUI_BBOX_UPDATE_FROM_CHILDREN_BBOX BIT_18
#define FLAG_NUI_BBOX_UPDATE_DURING_PUSH BIT_19
#define FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS BIT_20
#define FLAG_NUI_BBOX_UPDATE_ONCE BIT_21
// Misc
#define FLAG_NUI_COLOR_UPDATE BIT_22 // Set to ON, and UI Colors (of all elements, bkg, icon, text, ...)  are going to be updated from UIStyle ColorSet, according with the state of the UI: ENABLE / DISABLE / FOCUSED
// #define	FLAG_NUI_xxx									BIT_23

// ... All flags until BIT_23 (included) are reserved for NUI use only.
// So, BIT_24 to BIT_31 included can be used by structures inherited from NUI

// MASKs
//
// All the Flags used to determine if an UI is active
#define MASK_NUI_ACTIVE (FLAG_NUI_SHOW | FLAG_NUI_ENABLE)
// All the Flags used to define Touch Event Listening ( 4 flags )
#define MASK_NUI_LISTEN_TOUCH_EVENT (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT | FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT)
// All the Flags used to define Event Listening ( 4 flags of touch event listening included )
#define MASK_NUI_LISTEN_EVENT (MASK_NUI_LISTEN_TOUCH_EVENT | FLAG_NUI_LISTEN_UICORE_EVENT | FLAG_NUI_LISTEN_UICOMMAND_EVENT | FLAG_NUI_LISTEN_UINOTIFY_EVENT)
// All the Flags used to define Event Intercepting
#define MASK_NUI_INTERCEPT_EVENT (FLAG_NUI_INTERCEPT_UICORE_EVENT | FLAG_NUI_INTERCEPT_UICOMMAND_EVENT | FLAG_NUI_INTERCEPT_TOUCH_EVENT | FLAG_NUI_INTERCEPT_UINOTIFY_EVENT)

/*
// All The Flags used by NSendUIEvent function to decide if an UI is going to listen, and receive, sent event.
// (notes: Touch and system events are not included because is not allowed to Send this kind of event trough NSendUIEvent )
#define MASK_NSendUIEvent_LISTEN_EVENT					(FLAG_NUI_LISTEN_UICORE_EVENT|FLAG_NUI_LISTEN_UICOMMAND_EVENT|FLAG_NUI_LISTEN_UINOTIFY_EVENT)
// All The Flags used by NSendUIEvent function to decide if an UI is going to intercept, and receive, sent event.
// (notes: Touch and system event are not included because is not allowed to Send this kind of event trough NSendUIEvent )
#define MASK_NSendUIEvent_INTERCEPT_EVENT				(FLAG_NUI_INTERCEPT_UICORE_EVENT|FLAG_NUI_INTERCEPT_UICOMMAND_EVENT|FLAG_NUI_INTERCEPT_UINOTIFY_EVENT)
*/

// -----------------------------------------------------------------------------------------------------------------------------------------
// All the valid and consistent Compositions of FLAG_NUI_xxx in relation with TOUCH_MODE
#define FLAGS_NUI_TOUCH_MODE_NULL (0)
#define FLAGS_NUI_TOUCH_MODE_CATCH (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)
#define FLAGS_NUI_TOUCH_MODE_CATCH_TRACK (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT)
#define FLAGS_NUI_TOUCH_MODE_CATCH_RELEASE (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT)
#define FLAGS_NUI_TOUCH_MODE_BASIC (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START)
#define FLAGS_NUI_TOUCH_MODE_BASIC_TRACK (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT)
#define FLAGS_NUI_TOUCH_MODE_DROP (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT)
#define FLAGS_NUI_TOUCH_MODE_GREED (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN)
#define FLAGS_NUI_TOUCH_MODE_GREED_TRACK (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT)
#define FLAGS_NUI_TOUCH_MODE_ON_THE_FLY (FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT)
// -----------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------
// UI CORE FUNCTIONNALITIES DESCRIPTION FLAGS works with 'NUI_DESC.Flags_Core'
// #define	FLAG_NUIDC_USEPARAM_LISTEN_TOUCH_MODE					BIT_0	// Used by DESC only
#define FLAG_NUIDC_USEPARAM_BOUNDINGBOX BIT_1 // Used by DESC only
#define FLAG_NUIDC_PRECISE_PICKING BIT_2	  // Equivalent to UI Flag:  FLAG_NUI_PRECISE_PICKING

#define FLAG_NUIDC_DISABLE BIT_3						// Opposite of UI Flag FLAG_NUI_ENABLE.
														// By default an UI is Enable and hidden at Start and will receive 2 events:
														//								NUI_SETUP
														//								NUI_ENABLE.
														//	... So after Setup UI Activation flags are like this:
														//								FLAG_NUI_ENABLE set to ON.
														//								FLAG_NUI_SHOW set to OFF.
														//	WITH FLAG_NUIDC_DISABLE set to ON, UI will not receive NUI_ENABLE event and ...
														//	after Setup  UI Activation flags are like this:
														//								FLAG_NUI_ENABLE set to OFF.
														//								FLAG_NUI_SHOW set to OFF.
#define FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED BIT_4		// Equivalent to UI Flag:  FLAG_NUI_BBOX_UPDATE_ASNECESSARY
#define FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX BIT_5 // Equivalent to UI Flag:  FLAG_NUI_BBOX_UPDATE_FROM_CHILDREN_BBOX
#define FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP BIT_6		// Equivalent to UI RunTime Flag:  FLAG_NUI_BBOX_UPDATE_ONCE (... AT_SETUP because it was set at setup ! )
#define FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH BIT_7
#define FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS BIT_8

#define FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND BIT_9  // Used by DESC only
#define FLAG_NUIDC_PICKING_MESH_FROM_ICON BIT_10	   // Used by DESC only
#define FLAG_NUIDC_PICKING_MESH_FROM_TEXT BIT_11	   // Used by DESC only
#define FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH BIT_12 // Used by DESC only maybe not only by desc after all

#define FLAG_NUIDC_PARENT_CHILDRENLIST_BEGINNING_INSERTION BIT_13 // Used by DESC only
#define FLAG_NUIDC_USEPARAM_HIGHLEVEL_EXTRACTSET BIT_14			  // Maybe remove it because UI are necessary HUD and HUD 2D or HUD 3D are just going to be 2 different ways to render the same HUD ACCUMULATOR !
#define FLAG_NUIDC_UI_TRANSFORMHNODE BIT_15						  // Create an additional THN stored as NUI.pUITransformHNode
																  // (which is usually  equal on NUI.pRenderable.pTransformHNode.)
#define FLAG_NUIDC_USE_PARENT_RENDERABLE_TRANSFORMHNODE BIT_16

#define FLAG_NUIDC_HIERARCHICAL_SHOW_CONTROL_OFF BIT_17	  // Opposite of UI Flag FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL
#define FLAG_NUIDC_HIERARCHICAL_ENABLE_CONTROL_OFF BIT_18 // Opposite of UI Flag FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL
														  // By default, an UI is activated when it's parent is activated.
														  // That means:	Showed/Hide when it's parent is Showed/Hidden
														  //				Enable/Disable when it's parent is Enable/Disable
														  // These 2 FLAGS turn OFF this automatism in a certain way.
														  // That means:	 Visible UI is Hidden when it's parent is Hidden. (ALWAYS TRUE, with or without FLAG_NUIDC_HIERARCHICAL_SHOW_CONTROL_OFF )
														  //				>Hidden UI still hidden when it's parent is SHOWED.
														  //				 Enable UI is Disable when it's parent is Disable.(ALWAYS TRUE, with or without FLAG_NUIDC_HIERARCHICAL_ENABLE_CONTROL_OFF )
														  //				>Disable UI still Disable when it's parent is Enable.

// #define	FLAG_NUIDC_xxx										BIT_17
// #define	FLAG_NUIDC_xxx										BIT_18
// #define	FLAG_NUIDC_xxx										BIT_19
// #define	FLAG_NUIDC_xxx										BIT_20
// #define	FLAG_NUIDC_xxx										BIT_21
// #define	FLAG_NUIDC_xxx										BIT_22
// #define	FLAG_NUIDC_xxx										BIT_23

// ... All flags until BIT_23 (included) are reserved for NUI_DESC use only.
// So, BIT_24 to BIT_31 included can be used by structures inherited from NUI_DESC

// ----------------------------------------------------------------------------------------------------------
// GENERIC STYLE/APPEARANCE DESCRIPTION FLAGS works with 'NUI_DESC.StyleFlags'
#define FLAG_NUIDS_BACKGROUND BIT_0			 //
#define FLAG_NUIDS_ICON BIT_1				 //
#define FLAG_NUIDS_TEXT_DISPLAY_DEVICE BIT_2 // Should be a UIDC Flag !!
#define FLAG_NUIDS_COLOR_UPDATE BIT_3		 // Equivalent to UI Flag:  FLAG_NUI_COLOR_UPDATE
#define FLAG_NUIDS_COPY_NAME_STRING BIT_4
#define FLAG_NUIDS_DISPLAY_NAME BIT_5
#define FLAG_NUIDS_NAME_LOCKED BIT_6			// Name is locked. It will be impossible to change it through "NSetUIName" function.
#define FLAG_NUIDS_NO_GEOMETRY_EXTRACTION BIT_7 // Used by DESC only

#define FLAG_NUIDS_INTERCEPT_TOUCH_EVENT BIT_8 // Equivalent to UI Flag:  FLAG_NUI_INTERCEPT_TOUCH_EVENT
#define FLAG_NUIDS_INTERCEPT_UICORE_EVENT BIT_9
#define FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT BIT_10
#define FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT BIT_11

#define FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE BIT_12
#define FLAG_NUIDS_LISTEN_UICORE_EVENT BIT_13	 // Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICORE_EVENT
#define FLAG_NUIDS_LISTEN_UICOMMAND_EVENT BIT_14 // Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICOMMAND_EVENT
#define FLAG_NUIDS_LISTEN_UINOTIFY_EVENT BIT_15	 // Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICOMMAND_EVENT

	// !!!	FLAG_NUIDS_DO_NOT_LISTEN_UICORE_EVENT						BIT_13	// Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICORE_EVENT
	// !!!	FLAG_NUIDS_DO_NOT_LISTEN_UICOMMAND_EVENT					BIT_14	// Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICOMMAND_EVENT
	// !!!	FLAG_NUIDS_DO_NOT_LISTEN_UINOTIFY_EVENT						BIT_15	// Equivalent to  UI Flag:  FLAG_NUI_LISTEN_UICOMMAND_EVENT
	// Replace by the positive Version ... Update of all ui creation function in progress ... TO be continued !!!

#define FLAG_NUIDS_DO_NOT_BLOCK_TOUCH_EVENT_FOR_GAMESTATES BIT_16 // Equivalent to the opposite of UI Flag:  FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES
#define FLAG_NUIDS_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN BIT_17	  // Equivalent to the UI Flag:  FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN

	// #define	FLAG_NUIDS_UNDER_TOUCH_CONTROL							BIT_18	// ON means UI without Release Touch Move OUT, behaves like UI with TOUCH MOVE OUT activated but Keep Touch.
	// #define FLAG_NUIDS_CAPTURE_TOUCH_START							BIT_19	// ON means capture TOUCH at start. OFF means doesn't capture TOUCH at START.
	// #define FLAG_NUIDS_CAPTURE_TOUCH_MOVE_IN						BIT_20	// ON means capture TOUCH at Move IN. OFF means doesn't capture TOUCH at MOVE IN.
	// #define FLAG_NUIDS_RELEASE_TOUCH_MOVE_OUT						BIT_21	// ON means capture Release TOUCH at MOVE OUT. OFF means doesn't Release TOUCH at MOVE OUT.
	// #define FLAG_NUIDS__											BIT_22
	// #define FLAG_NUIDS__											BIT_23
	// ... All flags until BIT_23 (included) are reserved for NUISEC use only.
	// So, BIT_24 to BIT_31 included can be used by structures inherited from NUI_DESC

#define MASK_NUIDS_LISTEN_EVENT (FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE | FLAG_NUIDS_LISTEN_UICORE_EVENT | FLAG_NUIDS_LISTEN_UICOMMAND_EVENT | FLAG_NUIDS_LISTEN_UINOTIFY_EVENT)
#define MASK_NUIDS_INTERCEPT_EVENT (FLAG_NUIDS_INTERCEPT_TOUCH_EVENT | FLAG_NUIDS_INTERCEPT_UICORE_EVENT | FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT | FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT)
	//
	// ---------------------- FLAGS_NUI_TOUCH_MODE ( FLAGS compositions Table )-------------------------------------------------------------------
	//
	// ---------------------------------------------------------------------------------------------------------------------------------------
	//		S			T	| ( TRACK is for TRACK MOVE IN OUT )
	//		T			R	|
	//		A		O	A	|
	//		R	I	U	C	|
	//		T	N	T	K	|
	// ---------------------------------------------------------------------------------------------------------------------------------------
	//		x	x	X	X	|	Inconsistent		- TRACK MOVE IN OUT  behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible.
	//		x	x	x	.	|	MODE ON_THE_FLY		- Capture Touch Start and Touch Move In And keep them  until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		x	X	.	X	|	MODE GREED_TRACK	- Capture Touch Start and Touch Move In And keep them  until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		x	x	.	.	|	MODE GREED			- Capture Touch Start and Touch Move In And keep them  until Touch End/Cancel, without any specific behavior.
	//		x	.	X	X	|	Inconsistent		- TRACK MOVE IN OUT  behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible.
	//		X	.	x	.	|	MODE DROP			- Capture Touch Start and keep it until Move Out (or End/Cancel) , without any specific behavior.
	//		x	.	.	X	|	MODE BASIC_TRACK	- Capture Touch Start and keep it until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		x	.	.	.	|	MODE BASIC			- Capture Touch Start and keep it until Touch End/Cancel, without any specific behavior.
	//		.	x	X	X	|	Inconsistent		- TRACK MOVE IN OUT  behavior and FLAGS settings are not compatible.
	//		.	x	x	.	|	MODE CATCH_RELEASE	- Capture Touch Move In Only ( not Touch Start )  and Release Touch at Move Out (or End/Cancel), with specific UNDER TOUCH CONTROL behavior.
	//		.	x	.	x	|	MODE CATCH_TRACK	- Capture Touch Move In Only ( not Touch Start )  and keep it until Touch End/Cancel, with specific TRACK MOVE IN OUT behavior.
	//		.	x	.	.	|	MODE CATCH			- Capture Touch Move In Only ( not Touch Start )  and keep it until Touch End/Cancel, without any specific behavior.
	//		.	.	X	X	|	Inconsistent		- An UI can not behave TRACK MOVE IN OUT  or Release Touch at Move Out (or End/Cancel) without any activated capture touch -- AND -- TRACK MOVE IN OUT behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible.
	//		.	.	X	.	|	Inconsistent		- An UI can not Release Touch at Move Out (or End/Cancel) without any activated capture touch
	//		.	.	.	X	|	Inconsistent		- An UI can not behave TRACK MOVE IN OUT  without any activated capture touch
	//		.	.	.	.	|	MODE NULL			- No Capture/ No Release ...
	typedef enum
	{
		NUI_LISTEN_TOUCH_MODE_NULL = 0,
		NUI_LISTEN_TOUCH_MODE_CATCH,
		NUI_LISTEN_TOUCH_MODE_CATCH_TRACK,
		NUI_LISTEN_TOUCH_MODE_CATCH_RELEASE,
		NUI_LISTEN_TOUCH_MODE_BASIC,
		NUI_LISTEN_TOUCH_MODE_BASIC_TRACK,
		NUI_LISTEN_TOUCH_MODE_DROP,
		NUI_LISTEN_TOUCH_MODE_GREED,
		NUI_LISTEN_TOUCH_MODE_GREED_TRACK,
		NUI_LISTEN_TOUCH_MODE_ON_THE_FLY,
		// ----------------------------
		NUI_LISTEN_TOUCH_MODE_ENUM_SIZE
	} NUI_LISTEN_TOUCH_MODE_ENUM;

// ----------------------------------------------------------------------------------------------------------
// UI GRAPHIC DESC used by NUIGEOMETRY_DESC.Flags and NUITEXT_DESC.Flags ...  32 FLAGS
//
// Specific to NUIGEOMETRY_DESC
#define FLAG_NUIGD_USEPARAM_CUSTOM_GEOMETRY BIT_0 //
// #define		FLAG_NUIGD_USEPARAM_TEXCOORDBUILD		 				BIT_1	// Force using TexCoordBuild member to build TxC

// Specific to NUITEXT_DESC
#define FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE BIT_2 // Force using The Renderable TransformHNode of the Host UI instead of using pUITransformedHNode.
#define FLAG_NUIGD_USEPARAM_TEXT_ALIGNMENT BIT_3			   //

// Common to NUIGEOMETRY_DESC and NUITEXT_DESC
#define FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT BIT_4 //
#define FLAG_NUIGD_USEPARAM_USERSTATESET BIT_5	  //
#define FLAG_NUIGD_USEPARAM_APPEARANCE BIT_6	  //
// #define		FLAG_NUIGD_USEPARAM_COLORBUILD							BIT_7	// Force using ColorBuild member to build vertex color

// Common to NUIGEOMETRY_DESC and NUITEXT_DESC Placements Flags
// ... Useful to be ABSOLUTLY sure that User setup all of these parameters !
#define FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE BIT_8			  // Placement.size				is setup
#define FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION BIT_9 // Placement.RelativePosition	is setup
#define FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR BIT_10			  // Placement.Anchor				is setup
#define FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT BIT_11		  // Placement.pParent AND Placement.ParentAnchor are setup

	// UISTATEs (Version 3)
	// There are 2 different main types (both are STATES. It's more an intellectual split to make things more readable...)
	//	NUI_STATE_xxx		-->	A state with a real duration.
	//	NUI_INSTANT_xxx		--> A transition state. It is set by "event_to_state" and set again to an another state by "state_handle"
	//								All this process inside the same "loop".
	typedef enum
	{
		//---------------------------------------
		NUI_STATE_NULL = 0,			 //
									 //---------------------------------------
		NUI_STATE_PUSH_UP_REST,		 //
		NUI_STATE_PUSH_TO_UP,		 //
		NUI_STATE_PUSH_TO_UP_CANCEL, //
		NUI_STATE_PUSH_TO_DOWN,		 //
		NUI_STATE_PUSH_DOWN_WAIT,	 //

		// A KEY_INSTANT is a "moment" between 2 states.
		NUI_KEY_INSTANT_PUSH_UP,		  // When UI arrives at UP position just before being into _UP_REST  State.
		NUI_KEY_INSTANT_PUSH_DOWN_CANCEL, // When UI leaves _DOWN_WAIT state, just before being into _TO_UP_CANCEL State for animated UI or just before being _UP_REST State for non animated one.
		NUI_KEY_INSTANT_PUSH_DOWN,		  // When UI arrives at DOWN position just before being into _DOWN_WAIT State.
		NUI_KEY_INSTANT_PUSH_RESET,		  // When UI is forced to _UP_REST State, if it's "Hidden", "Cleared" or "Disabled" without already being into _UP_REST State.
										  // (Pushed UI do not really used it yet, but it exists ... )
										  //---------------------------------------
										  // 	NUI_STATE_VALUPDATER_REST,
										  // 	NUI_STATE_VALUPDATER_START,
										  // 	NUI_STATE_VALUPDATER_WAIT,
										  // 	NUI_STATE_VALUPDATER_UPDATE,
										  // 	NUI_STATE_VALUPDATER_END,
										  //---------------------------------------
										  // 	NUI_STATE_CHECKBOX_REST,
										  // 	NUI_STATE_CHECKBOX_UPDATE,
										  // 	NUI_STATE_CHECKBOX_WAIT,
										  //---------------------------------------
		NUI_STATE_SLIDE_REST,
		NUI_STATE_SLIDE_DRAG,
		NUI_STATE_SLIDE_FREE,
		NUI_STATE_SLIDE_FREE_STOP,
		// NUI_INSTANT_SLIDE_DRAGING_TO_SLIDING,
		//---------------------------------------
		NUI_STATE_COLORCIRCLE_REST,
		NUI_STATE_COLORCIRCLE_HUE_SATURATION_UPDATE,
		NUI_STATE_COLORCIRCLE_BRIGHTNESS_UPDATE,
		NUI_STATE_COLORCIRCLE_ALPHA_UPDATE,
		NUI_STATE_COLORCIRCLE_HUE_SATURATION_WAIT,
		NUI_STATE_COLORCIRCLE_BRIGHTNESS_WAIT,
		NUI_STATE_COLORCIRCLE_ALPHA_WAIT,
		NUI_STATE_COLORCIRCLE_END,
		//---------------------------------------
		NUI_STATE_ENUM_SIZE // !!! MUST BE < 65535. N needs to be able packing 2 NUI_STATEs in the same 32 bits with MAKELONG(state0,state1)
							//						...It's for 'UIPush' with PREVIOUS_STATE and KEY_INSTANT
	} NUI_STATE_ENUM;

	// UI Pick Result
	typedef Nu32 NUI_PICK_RESULT;
#define FLAG_NUI_PICK_RESULT_IS_PICKED BIT_0
#define FLAG_NUI_PICK_RESULT_PICK_CHILDREN BIT_1

	// UI Extended, Default call back
	typedef struct NUI NUI;
	typedef struct NUI_DESC NUI_DESC;

	typedef NUI_EVENT_RESULT (*NUI_EVENT_HANDLE)(NUI *pui, const NEVENT *pevent);
	typedef void (*NUI_CLEAR_HANDLE)(NUI *pui);
	typedef void (*NUI_BBOX_UPDATING_HANDLE)(NUI *pui);
	typedef NUI_PICK_RESULT (*NUI_PICK_HANDLE)(const NUI *pui, const NVEC2 *pmouse_normalized);
	typedef void (*NUI_STATE_HANDLE)(NUI *pui, const NTIME *ptime);				   // Must return Previous State ( == current state at function entering )
	typedef void (*NUI_FRAME_EVENTTOSTATE_HANDLE)(NUI *pui, const NEVENT *pevent); // Must return Previous State ( == current state at function entering )

	// a UI of UI
	struct NUI
	{
		NHIERARCHY_NODEDEF // Should becomes DBL_LINKED_LIST_NODE(NUI) at the END of the NUI structure !

			// UI FLags:
			Nu32 Flags; // 32 Flags that describe UI properties.
		Nu16 FlagsXtd;	// 16 Flags that describe UI properties.

		// Interactive UI Part:
		Nu8 TouchCountMax;
		Nu8 TouchCompatibility;
		NTOUCH_UI_LISTENER *pTouchUIListener;

		Nchar *pName;
		NTRANSFORM_HNODE *pUITransformHNode;

		// Gfx Part:
		NRENDERABLE *pRenderable; // Should becomes Renderable and the first component of the UI Structure
		NTEXT *pText;			  // Should becomes pTextDisplayDevice
		NUI_STYLE *pStyle;

		NUI_STATE_ENUM CurrentState;
		Nf32 fPushUpDownRatio; // Current Ratio between 0(UP/REST) and 1(DOWN).
		// Picking
		NRECTf32 BoundingBox;
		NARRAY PickingGeometryTransformedVertexArray;
		NMESH *pPickingGeometryMesh;
		// Functions
		NUI_EVENT_HANDLE Event_Proc;
		// System functions
		NUI_BBOX_UPDATING_HANDLE BoundingBoxUpdating_Proc;
		NUI_PICK_HANDLE Pick_Proc;
		NUI_FRAME_EVENTTOSTATE_HANDLE UIEventToState_Proc;
		NUI_STATE_HANDLE UIState_Proc;
		NUI_CLEAR_HANDLE ClearUI_Proc;
		// User 32 value:
		Nu32 User_Nu32;

#ifdef _DEBUG
		Nu32 Flags_Debug;
#endif
	};

	/*
	typedef struct
	{
		Nu32					Creation_Mask;

		NFONT					*pFont;
		NVEC3				Margin;
	// 	Nf32					fTransitionDuration;
	// 	Nf32					fDownScale;

		// Push Button Animations Params
		NUI_EASING_FCT_ID_ENUM	PushButton_EasingFunctionID;
		Nf32					Push_UpSpeed;
		Nf32					Push_DownSpeed;
		Nf32					Push_CancelSpeed;
		NVEC3				Push_DownTranslation;		// To store local translations of the Node
		NVEC3				Push_DownScale;				// To Store local Scale of the Node
		NQUATERNION				Push_DownQuatRotation;		// To store local rotations of the Node

	/ *
		// UI use:	at FRAME Creation, created Text will keep its own Font ptr and Colors.
		NCOLOR					UIText_ColorRange[NUI_COLORSET_ENUM_SIZE][CONSTANT_NUI_STYLE_TEXT_COLORRANGE_SIZE];			// Used for Quad building (Text/Caption).
		NCOLOR					UIBackground_ColorRange[NUI_COLORSET_ENUM_SIZE][CONSTANT_NUI_STYLE_BACK_COLORRANGE_SIZE];	// Used for Quad building (Background UI).
		NCOLOR					UIIcon_ColorRange[NUI_COLORSET_ENUM_SIZE][CONSTANT_NUI_STYLE_ICON_COLORRANGE_SIZE];			// Used for Quad building (Icon).

		// CheckBox
		NCOLOR					CheckBoxCheck_ColorRange[NUI_COLORSET_ENUM_SIZE][CONSTANT_NUI_STYLE_CHECK_COLORRANGE_SIZE];		// Used for Quad building (CheckMesh).
		NCOLOR					CheckBoxUnCheck_ColorRange[NUI_COLORSET_ENUM_SIZE][CONSTANT_NUI_STYLE_UNCHECK_COLORRANGE_SIZE];	// Used for Quad building (UnCheckMesh).
	* /

		NCOLOR					ColorSet[NUI_COLORSET_TARGET_ENUM_SIZE][NUI_COLORSET_ENUM_SIZE][CONSTANT_COLORSET_COLORRANGE_SIZE];	// Used for Quad building (UnCheckMesh).

	}NUI_STYLE_DESC;
	*/

	typedef enum
	{
		NUI_CAPTION_STRING_MAXSIZE = 32,
		NUI_CHECKTEXTS_STRING_MAXSIZE = 8,
		NUI_STRING_MAX_MAXSIZE = 32 // Equal to the Max of those values ! ( here = NUI_CAPTION_STRING_MAXSIZE )
	} NUISTRING_MAXSIZE_ENUM;

	typedef struct
	{
		Nu32 Flags;							  // |
		NUIPLACEMENT Placement;				  // |
		NGEOMETRY_FORMAT_ENUM GeometryFormat; // |
		NSTATESET UserStateSet;				  // |_ NUITEXT_DESC/NUIGEOMETRY_DESC common part
		NAPPEARANCE_RAW Appearance_Raw;		  // |
		NBUILDBASE_COLOR ColorBuild;		  // | |-Color Building

		NTEXT_ALIGNMENT_ENUM Alignment;
	} NUITEXT_DESC;

	typedef struct
	{
		Nu32 Flags;							  // |
		NUIPLACEMENT Placement;				  // |
		NGEOMETRY_FORMAT_ENUM GeometryFormat; // |
		NSTATESET UserStateSet;				  // |_ NUITEXT_DESC/NUIGEOMETRY_DESC common part
		NAPPEARANCE_RAW Appearance_Raw;		  // |
		NBUILDBASE_COLOR ColorBuild;		  // | |-Color Building

		NBUILDBASE_TEXCOORD TexCoordBuild; // |-Texture Coordinates Building
		NGEOMETRY *pCustomGeometry;
	} NUIGEOMETRY_DESC;

	struct NUI_DESC
	{
		Nu32 Flags_Style;
		Nu32 Flags_Core;
		NEXTRACTSET HighLevelExtractSet;
		NUI_LISTEN_TOUCH_MODE_ENUM TouchMode; // Predefined Touch Mode used to setup NUI Touch Mode Flags (FLAG_NUI_CAPTURE_TOUCH_START/FLAG_NUI_CAPTURE_TOUCH_MOVE_IN/FLAG_NUI_RELEASE_TOUCH_MOVE_OUT/FLAG_NUI_UNDER_TOUCH_CONTROL).

		Nu16 GeometryPreAllocatedCapacity;
		// Nu16						available !!!!

		NUI_STYLE *pStyle;
		NRECTf32 BoundingBox;

		// Nchar						*pCaptionString;
		Nchar *pName;

		NUITEXT_DESC UITextDesc;
		NUIGEOMETRY_DESC UIBackgroundDesc;
		NUIGEOMETRY_DESC UIIconDesc;

		NMESH *pCustomPickingMesh;

		NUI_EVENT_HANDLE Event_Proc;

		// Private auto-update
		Nu32 Private_SetUpStack;
	};

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	//
	// BASIC FUNCTIONS
	NUI *NCreateUI(NUI *parent, NUI_DESC *uidesc, const Nu32 user32);
	NUI *NSetupUI(NUI *parent, NUI *pui, NUI_DESC *uidesc, const Nu32 user32);
	void NClearUI(NUI *pui);
	void NDeleteUI(NUI *pui);

	inline Nu32 NGetUIUserData(const NUI *pui) { return pui->User_Nu32; };
	inline void NSetUIUserData(NUI *pui, const Nu32 user) { pui->User_Nu32 = user; };
	void NSetUIName(NUI *pui, const Nchar *pname);

	Nbool NHideUI(NUI *pui);
	Nbool NShowUI(NUI *pui);
	Nbool NEnableUI(NUI *pui);
	Nbool NDisableUI(NUI *pui);

#define NIsUIVisible(pui) (ISFLAG_ON((pui)->Flags, FLAG_NUI_SHOW))
#define NIsUIHidden(pui) (ISFLAG_OFF((pui)->Flags, FLAG_NUI_SHOW))

#define NIsUIEnable(pui) (ISFLAG_ON((pui)->Flags, FLAG_NUI_ENABLE))
#define NIsUIDisable(pui) (ISFLAG_OFF((pui)->Flags, FLAG_NUI_ENABLE))

#define NIsUIActive(pui) (FLAGS_TEST((pui)->Flags, MASK_NUI_ACTIVE, MASK_NUI_ACTIVE))	 // Visible AND Enable
#define NIsUIInactive(pui) (!FLAGS_TEST((pui)->Flags, MASK_NUI_ACTIVE, MASK_NUI_ACTIVE)) // Invisible AND Disable

	inline void NEnableUIHierarchicalShowControl(NUI *pui)
	{
		FLAG_ON(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL);
	}
	inline void NDisableUIHierarchicalShowControl(NUI *pui) { FLAG_OFF(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL); }

	inline void NEnableUIHierarchicalEnableControl(NUI *pui) { FLAG_ON(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL); }
	inline void NDisableUIHierarchicalEnableControl(NUI *pui) { FLAG_OFF(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL); }

	// UI Color
	inline void NEnableUIColorUpdate(NUI *pui) { FLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE); }
	inline void NDisableUIColorUpdate(NUI *pui) { FLAG_OFF(pui->Flags, FLAG_NUI_COLOR_UPDATE); }

	inline NUI *NGetUIParent(const NUI *pui) { return (NUI *)((NHIERARCHY_NODE *)pui)->pParent; }
	NUI *NGetUIRoot(const NUI *pui);
	inline Nu32 NGetUINumberOfChildren(const NUI *pui) { return NGetHNodeChildrenCount((NHIERARCHY_NODE *)pui); }
	inline NUI *NGetFirstUIChild(const NUI *pui)
	{
		if (NIS_NODE_VALID(pui->ChildrenList.pFirst, &pui->ChildrenList))
		{
			return (NUI *)pui->ChildrenList.pFirst;
		}
		else
		{
			return NULL;
		}
	}
	inline NUI *NGetLastUIChild(const NUI *pui)
	{
		if (NIS_NODE_VALID(pui->ChildrenList.pLast, &pui->ChildrenList))
		{
			return (NUI *)pui->ChildrenList.pLast;
		}
		else
		{
			return NULL;
		}
	}

	// Advanced UI Touch Listening
	inline void NFullyOnScreenUITouchListeningRestriction(NUI *pui, const Nbool b)
	{
		if (b)
		{
			FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN);
		}
		else
		{
			FLAG_OFF(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN);
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Advanced Events Management.
	// +
	// + Use them carefully ! For touch event listening/intercepting modifications, "UI" should not be listening touch.
	// +
	// +
	// + ---> (UI) Touch events
	// 			+
	// 			+--+ Listen
	//				  +
	//				  +-+ Trough Listen Touch Mode
	void NSetUIListenTouchMode(NUI *pui, const NUI_LISTEN_TOUCH_MODE_ENUM touch_mode);
#define NDisableUITouchEvent_Listening(pui) NSetUIListenTouchMode(pui, NUI_LISTEN_TOUCH_MODE_NULL)
#define NIsUIListenTouchEvent(pui) (!FLAGS_TEST((pui)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN, 0)) // An UI is able to listen if it is able to "Start" listening, so at Touch Start or/and at Touch Move In !

	//				  +
	//				  +-+ ... or update the 4 UI Listen Touch Flags one by one (useful to change/update just some of them)
	void NEnableUIListenTouch_CaptureStart(NUI *pui);
	void NDisableUIListenTouch_CaptureStart(NUI *pui);

	void NEnableUIListenTouch_CaptureMoveIn(NUI *pui);
	void NDisableUIListenTouch_CaptureMoveIn(NUI *pui);

	void NEnableUIListenTouch_ReleaseMoveOut(NUI *pui);
	void NDisableUIListenTouch_ReleaseMoveOut(NUI *pui);

	void NEnableUIListenTouch_TrackMoveInOut(NUI *pui);
	void NDisableUIListenTouch_TrackMoveInOut(NUI *pui);
	//			+
	// 			+--+ Interception
	void NEnableUITouchEvent_Interception(NUI *pui);
	void NDisableUITouchEvent_Interception(NUI *pui);
#define NIsUIInterceptTouchEvent(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT)
	//			+
	// 			+--+ Specific for Touch Event ...
	void NEnableGameStatesTouchEventBlocking(NUI *pui);
	void NDisableGameStatesTouchEventBlocking(NUI *pui);
#define NIsUIBlockTouchEventForGameStates(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES)

	// +
	// + ---> UiCore events
	// 			+
	// 			+--+ Listen
	void NEnableUICoreEvent_Listening(NUI *pui);
	void NDisableUICoreEvent_Listening(NUI *pui);
#define NIsUIListenUICoreEvent(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_LISTEN_UICORE_EVENT)

	//			+
	// 			+--+ Interception
	void NEnableUICoreEvent_Interception(NUI *pui);
	void NDisableUICoreEvent_Interception(NUI *pui);
#define NIsUIInterceptUICoreEvent(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT)
	// +
	// + ---> UiCommand events
	// 			+
	// 			+--+ Listen
	void NEnableUICommandEvent_Listening(NUI *pui);
	void NDisableUICommandEvent_Listening(NUI *pui);
#define NIsUIListenUICommandEvent(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_LISTEN_UICOMMAND_EVENT)
	//			+
	// 			+--+ Interception
	void NEnableUICommandEvent_Interception(NUI *pui);
	void NDisableUICommandEvent_Interception(NUI *pui);
#define NIsUIInterceptUICommandEvent(pui) ISFLAG_ON((pui)->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT)

	// UI Touch Counts
	inline void NSetUITouchCountMax(NUI *pui, const Nu8 count_max)
	{
		NErrorIf(pui->pTouchUIListener, NERROR_NON_NULL_VALUE);
		NErrorIf(count_max > CONSTANT_NTOUCH_COUNT_MAX, NERROR_VALUE_OUTOFRANGE);
		pui->TouchCountMax = count_max;
	} // Note that not allowed to update Touch Count Max of an UI that is listening touch ( pui->pTouchUIListener must be NULL ! )

#define NGetUITouchCount(pui) NGetListenerTouchCount(&(pui)->pTouchUIListener->Listener)
#define NGetUIPreviousTouchCount(pui) NGetListenerPreviousTouchCount(&(pui)->pTouchUIListener->Listener)
#define NGetUITouchCountIn(pui) NGetListenerTouchCountIn(&(pui)->pTouchUIListener->Listener)
#define NGetUITouchCountOut(pui) NGetListenerTouchCountOut(&(pui)->pTouchUIListener->Listener)
#define NGetUIPreviousTouchCountIn(pui) NGetListenerPreviousTouchCountIn(&(pui)->pTouchUIListener->Listener)
#define NGetUIPreviousTouchCountOut(pui) NGetListenerPreviousTouchCountOut(&(pui)->pTouchUIListener->Listener)

#define NIsUIFirstTouchStarted(pui) NIsListenerFirstTouchStarted(&(pui)->pTouchUIListener->Listener)
#define NIsUILastTouchEnded(pui) NIsListenerLastTouchEnded(&(pui)->pTouchUIListener->Listener)
#define NIsUIFirstTouchMoveIn(pui) NIsListenerFirstTouchMoveIn(&(pui)->pTouchUIListener->Listener)
#define NIsUILastTouchMoveOut(pui) NIsListenerLastTouchMoveOut(&(pui)->pTouchUIListener->Listener)

	// NUIGEOMETRY_DESC functions
	inline NVEC3 *NGetUIGeometryDescCustomGeometrySize(NVEC3 *psize, const NUIGEOMETRY_DESC *pgd)
	{
		NErrorIf(ISFLAG_OFF(pgd->Flags, FLAG_NUIGD_USEPARAM_CUSTOM_GEOMETRY) || (!pgd->pCustomGeometry), NERROR_UIGEOMETRYDESC_CUSTOM_GEOMETRY_REQUESTED);
		NAABB aabb;
		NGetGeometryAABB(pgd->pCustomGeometry, &aabb);
		return NGetAABBSize(psize, &aabb);
	}
	inline NVEC2s16 *NGetUIGeometryDescTextureUnitSizes16(NVEC2s16 *psize, const NUIGEOMETRY_DESC *pgd, const Nu8 texture_unit)
	{
		NErrorIf(ISFLAG_OFF(pgd->Flags, FLAG_NUIGD_USEPARAM_APPEARANCE) || (!NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC *)pgd)->Appearance_Raw, texture_unit)), NERROR_UIGEOMETRYDESC_TEXTURE_REQUESTED);
		NTEXTURE *ptexture = NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC *)pgd)->Appearance_Raw, texture_unit);
		*psize = ptexture->size;
		return psize;
	}
	inline NVEC2 *NGetUIGeometryDescTextureUnitHRelSize(NVEC2 *psize, const NUIGEOMETRY_DESC *pgd, const Nu8 texture_unit)
	{
		NErrorIf(ISFLAG_OFF(pgd->Flags, FLAG_NUIGD_USEPARAM_APPEARANCE) || (!NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC *)pgd)->Appearance_Raw, texture_unit)), NERROR_UIGEOMETRYDESC_TEXTURE_REQUESTED);
		NTEXTURE *ptexture = NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC *)pgd)->Appearance_Raw, texture_unit);
		return NSize_PixToHRel(psize, &ptexture->size);
	}

	inline void NSetUIGeometryDescAnchor(NUIGEOMETRY_DESC *pgd, const NVEC3 *panchor)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR);
		NSetUIPlacementAnchor(&pgd->Placement, panchor);
	}
	inline void NSetUIGeometryDescAnchorf(NUIGEOMETRY_DESC *pgd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR);
		NSetUIPlacementAnchorf(&pgd->Placement, x, y, z);
	}
	inline void NSetUIGeometryDescCustomGeometry(NUIGEOMETRY_DESC *pgd, NGEOMETRY *pcustomgeom)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_CUSTOM_GEOMETRY);
		pgd->pCustomGeometry = pcustomgeom;
	}
	inline void NSetUIGeometryDescParent(NUIGEOMETRY_DESC *pgd, NUIPLACEMENT *pparentplac, const NVEC3 *panchor)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT);
		NSetUIPlacementParent(&pgd->Placement, pparentplac, panchor);
	}
	inline void NSetUIGeometryDescParentf(NUIGEOMETRY_DESC *pgd, NUIPLACEMENT *pparentplac, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT);
		NSetUIPlacementParentf(&pgd->Placement, pparentplac, x, y, z);
	}
	inline void NSetUIGeometryDescRelativePosition(NUIGEOMETRY_DESC *pgd, const NVEC3 *ppos)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION);
		NSetUIPlacementRelativePosition(&pgd->Placement, ppos);
	}
	inline void NSetUIGeometryDescRelativePositionf(NUIGEOMETRY_DESC *pgd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION);
		NSetUIPlacementRelativePositionf(&pgd->Placement, x, y, z);
	}
	inline void NSetUIGeometryDescSize(NUIGEOMETRY_DESC *pgd, const NVEC3 *psize)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		NSetUIPlacementSize(&pgd->Placement, psize);
	}
	inline void NSetUIGeometryDescSizef(NUIGEOMETRY_DESC *pgd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		NSetUIPlacementSizef(&pgd->Placement, x, y, z);
	}

	inline NVEC3 *NSetUIGeometryDescSizeFromCustomGeometry(NUIGEOMETRY_DESC *pgd)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		return NGetUIGeometryDescCustomGeometrySize(&pgd->Placement.Size, pgd);
	}
	inline NVEC3 *NSetUIGeometryDescSizeFromTextureUnitSize(NUIGEOMETRY_DESC *pgd, const Nu8 texture_unit)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		pgd->Placement.Size.z = 0.0f;
		return (NVEC3 *)NGetUIGeometryDescTextureUnitHRelSize((NVEC2 *)&pgd->Placement.Size, pgd, texture_unit);
	}
	inline NVEC3 *NSetUIGeometryDescSizeFromAtlasElementSize(NUIGEOMETRY_DESC *pgd, const NATLAS_ELEMENT *pae, const Nbool _refpix_src)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		pgd->Placement.Size.z = 0.0f;
		return (NVEC3 *)NGetAtlasElementHRelSize((NVEC2 *)&pgd->Placement.Size, pae, _refpix_src);
	}

	inline NRECTf32 *NSetUIGeometryDescTexCoordUsedOAARectf32FromAtlasElement(NUIGEOMETRY_DESC *pgd, const NATLAS_ELEMENT *pae)
	{
		FLAG_ON(pgd->TexCoordBuild.Flags, FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32);
		return NGetAtlasElementUsedOAARectf32(&pgd->TexCoordBuild.UsedOAARectf32, pae);
	}
	inline void NSetUIGeometryDescTexCoordUsedOAARectf32(NUIGEOMETRY_DESC *pgd, const NRECTf32 *ptextureusedOAArectf32)
	{
		FLAG_ON(pgd->TexCoordBuild.Flags, FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32);
		pgd->TexCoordBuild.UsedOAARectf32 = *ptextureusedOAArectf32;
	}
	inline void NSetUIGeometryDescTexCoordSubAARectf32(NUIGEOMETRY_DESC *pgd, const NRECTf32 *ptexsubAArectf32)
	{
		FLAG_ON(pgd->TexCoordBuild.Flags, FLAG_NBUILDBASE_TEXCOORD_USEPARAM_SUBAARECTf32);
		pgd->TexCoordBuild.SubAARectf32 = *ptexsubAArectf32;
	}
	inline void NSetUIGeometryDescTexCoordSetID(NUIGEOMETRY_DESC *pgd, const Nu8 texcoordsetid)
	{
		NErrorIf(texcoordsetid > CONSTANT_NBUILDBASE_TEXCOORD_TEXCOORDSETID_MAX, NERROR_VALUE_OUTOFRANGE);
		FLAG_ON(pgd->TexCoordBuild.Flags, FLAG_NBUILDBASE_TEXCOORD_USEPARAM_TEXCOORDSETID);
		pgd->TexCoordBuild.TexCoordSetID = texcoordsetid;
	}

	inline void NSetUIGeometryDescBlendAndMaterial(NUIGEOMETRY_DESC *pgd, const NBLEND *pblend, const NMATERIAL *pmat)
	{
		FLAG_ON(pgd->Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		pgd->Appearance_Raw.pBlend = (NBLEND *)pblend;
		pgd->Appearance_Raw.pMaterial = (NMATERIAL *)pmat;
	}
	inline void NSetUIGeometryDescColorRange(NUIGEOMETRY_DESC *pgd, const NCOLOR *pcolorrange, const Nu32 colorrangesize)
	{
		FLAG_ON(pgd->ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
		pgd->ColorBuild.pColorRange = (NCOLOR *)pcolorrange;
		pgd->ColorBuild.ColorRangeSize = colorrangesize;
	}

	// NUITEXT_DESC functions
	void NGetUITextDescStringSize(NVEC3 *psize, const NUITEXT_DESC *ptd, const NFONT *pfont, const Nchar *pstr, const NUISTRING_MAXSIZE_ENUM maxsize);

	inline void NSetUITextDescAnchor(NUITEXT_DESC *ptd, const NVEC3 *panchor)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR);
		NSetUIPlacementAnchor(&ptd->Placement, panchor);
	}
	inline void NSetUITextDescAnchorf(NUITEXT_DESC *ptd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR);
		NSetUIPlacementAnchorf(&ptd->Placement, x, y, z);
	}
	inline void NSetUITextDescParent(NUITEXT_DESC *ptd, NUIPLACEMENT *pplac, const NVEC3 *panchor)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT);
		NSetUIPlacementParent(&ptd->Placement, pplac, panchor);
	}
	inline void NSetUITextDescParentf(NUITEXT_DESC *ptd, NUIPLACEMENT *pplac, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT);
		NSetUIPlacementParentf(&ptd->Placement, pplac, x, y, z);
	}
	inline void NSetUITextDescRelativePosition(NUITEXT_DESC *ptd, const NVEC3 *ppos)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION);
		NSetUIPlacementRelativePosition(&ptd->Placement, ppos);
	}
	inline void NSetUITextDescRelativePositionf(NUITEXT_DESC *ptd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION);
		NSetUIPlacementRelativePositionf(&ptd->Placement, x, y, z);
	}
	inline void NSetUITextDescSize(NUITEXT_DESC *ptd, const NVEC3 *psize)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		NSetUIPlacementSize(&ptd->Placement, psize);
	}
	inline void NSetUITextDescSizef(NUITEXT_DESC *ptd, const Nf32 x, const Nf32 y, const Nf32 z)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
		NSetUIPlacementSizef(&ptd->Placement, x, y, z);
	}

	void NSetUITextDescSizeFromString(NUITEXT_DESC *ptd, const NFONT *pfont, const Nchar *pstr, const NUISTRING_MAXSIZE_ENUM maxsize);
	inline void NSetUITextDescBlendAndMaterial(NUITEXT_DESC *ptd, const NBLEND *pblend, const NMATERIAL *pmat)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_APPEARANCE);
		ptd->Appearance_Raw.pBlend = (NBLEND *)pblend;
		ptd->Appearance_Raw.pMaterial = (NMATERIAL *)pmat;
	}
	inline void NSetUITextDescColorRange(NUITEXT_DESC *ptd, const NCOLOR *pcolorrange, const Nu32 colorrangesize)
	{
		FLAG_ON(ptd->ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
		ptd->ColorBuild.pColorRange = (NCOLOR *)pcolorrange;
		ptd->ColorBuild.ColorRangeSize = colorrangesize;
	}
	inline void NSetUITextDescAligment(NUITEXT_DESC *ptd, const NTEXT_ALIGNMENT_ENUM align)
	{
		FLAG_ON(ptd->Flags, FLAG_NUIGD_USEPARAM_TEXT_ALIGNMENT);
		ptd->Alignment = align;
	}

// UI TRANSFORMATIONS (all of them are just "#define" that call  "TransformHNode" corresponding function )
#define NGetUIPositionPtr(pui) NGetTransformHNodePositionPtr((pui)->pRenderable->pTransformHNode)
#define NGetUIPosition(vm, pui) NGetTransformHNodePosition(vm, (pui)->pRenderable->pTransformHNode)
#define NGetUIPositionX(pui) NGetTransformHNodePositionX((pui)->pRenderable->pTransformHNode)
#define NGetUIPositionY(pui) NGetTransformHNodePositionY((pui)->pRenderable->pTransformHNode)
#define NGetUIPositionZ(pui) NGetTransformHNodePositionZ((pui)->pRenderable->pTransformHNode)

#define NMoveUI(pui, vm) NMoveTransformHNode((pui)->pRenderable->pTransformHNode, vm)
#define NMoveUIf(pui, x, y, z) NMoveTransformHNodef((pui)->pRenderable->pTransformHNode, x, y, z)
#define NMoveUIX(pui, x) NMoveTransformHNodeX((pui)->pRenderable->pTransformHNode, x)
#define NMoveUIY(pui, y) NMoveTransformHNodeY((pui)->pRenderable->pTransformHNode, y)
#define NMoveUIZ(pui, z) NMoveTransformHNodeZ((pui)->pRenderable->pTransformHNode, z)

#define NRotateUI(pui, rx, ry, rz) NRotateTransformHNode((pui)->pRenderable->pTransformHNode, rx, ry, rz)
#define NRotateUIQuat(pui, pnormalized_quat) NRotateTransformHNodeNormQuat((pui)->pRenderable->pTransformHNode, pnormalized_quat)

#define NScaleUI(puivs) NScaleTransformHNode((pui)->pRenderable->pTransformHNode, vs)

#define NSetUIPosition(pui, vm) NSetTransformHNodePosition((pui)->pRenderable->pTransformHNode, vm)
#define NSetUIPositionf(pui, x, y, z) NSetTransformHNodePositionf((pui)->pRenderable->pTransformHNode, x, y, z)
#define NSetUIPositionX(pui, x) NSetTransformHNodePositionX((pui)->pRenderable->pTransformHNode, x)
#define NSetUIPositionY(pui, y) NSetTransformHNodePositionY((pui)->pRenderable->pTransformHNode, y)
#define NSetUIPositionZ(pui, z) NSetTransformHNodePositionZ((pui)->pRenderable->pTransformHNode, z)

#define NSetUIRotation(pui, rx, ry, rz) NSetTransformHNodeRotation((pui)->pRenderable->pTransformHNode, rx, ry, rz)
#define NSetUIRotationQuat(pui, pquat) NSetTransformHNodeRotationQuat((pui)->pRenderable->pTransformHNode, pquat)

#define NSetUIScale(pui, vs) NSetTransformHNodeScale((pui)->pRenderable->pTransformHNode, vs)
#define NSetUIScalef(pui, sx, sy, sz) NSetTransformHNodeScalef((pui)->pRenderable->pTransformHNode, sx, sy, sz)

	// PICKING PROCESS
	void NResetUIPickingMesh(NUI *pui, const NRECTf32 *pboundingbox);
	void NSetUIPickingGeometryMesh(NUI *pui, const Nu32 internal_ui_geometry_id);
	void NSetUICustomPickingMesh(NUI *pui, const NMESH *pcustommesh);
	void NSetUITextAsPickingMesh(NUI *pui);

	// MISC MID LEVEL FUNCTIONS
	NGEOMETRY *NCreateUIGeometry(NUI *pui, const NUIGEOMETRY_DESC *pgdesc);
	NTEXT *NCreateUIText(const NUI *pui, const NEXTRACTSET highlevel_extractset, const NUITEXT_DESC *ptdesc);
	NGEOMETRY *NGetUIBackgroundGeometry(const NUI *pui);
	NGEOMETRY *NGetUIIconGeometry(const NUI *pui);
	NUI *NSetUIParent_EndInsert(NUI *pparent, NUI *pui, const Nbool bupdate_Renderable_transformnode_parent);
	NUI *NSetUIParent_BeginningInsert(NUI *pparent, NUI *pui, const Nbool bupdate_Renderable_transformnode_parent);
	void NBuildUITextMeshFromString(NUI *pui, const Nchar *pstring);
	NUI_COLORSET_STATE_ENUM NExtractUIDescColorState(const NUI_DESC *puidesc);
	// LOW LEVEL
	void NInitializeActiveUIRootsList();
	void NClearActiveUIRootsList();

	inline void NPushUISetUpProcess(NUI *pui, NUI_DESC *pframedesc) { pframedesc->Private_SetUpStack++; }
	NUI *NPopUISetUpProcess(NUI *pui, NUI_DESC *pframedesc);
	void NClearUI_Basics(NUI *pui);

	NUI *NPickUI(const NVEC2s16 *ppos);
	NUI *NPickUI_HRel(const NVEC2 *phreltouchpos);

// PRIVATE FUNCTIONS
// Transformations
#define NIsUISynchronizedWithTransformHNode(pui) (((pui)->pRenderable->pTransformHNode->Flags & FLAG_NTRANSFORM_HNODE_UPDATE_TAG) == ((pui)->Flags & FLAG_NUI_TRANSFORM_HNODE_UPDATE_TAG))
#define NSynchronizeUIWithTransformHNode(pui) (((pui)->pRenderable->pTransformHNode->Flags & FLAG_NTRANSFORM_HNODE_UPDATE_TAG) ? FLAG_ON((pui)->Flags, FLAG_NUI_TRANSFORM_HNODE_UPDATE_TAG) : FLAG_OFF((pui)->Flags, FLAG_NUI_TRANSFORM_HNODE_UPDATE_TAG))

	NUI *NPickUIRecursive(const NUI *pui, const NVEC2 *phreltouchpos);
	// The 3 "standard" picking functions
	NUI_PICK_RESULT NPickUIPickingMesh(const NUI *pui, const NVEC2 *pmouse_normalized);
	NUI_PICK_RESULT NPickUIPickingTextMesh(const NUI *pui, const NVEC2 *pmouse_normalized);
	NUI_PICK_RESULT NPickUIBBox(const NUI *pui, const NVEC2 *pmouse_normalized);

	// The 4 "standard" Update UIBBox functions
	void NUpdateUIBBox_From_PickingMesh(NUI *pui);
	void NUpdateUIBBox_From_TextMesh(NUI *pui);
	void NUpdateUIBBox_From_ChildrenBBox(NUI *pui);
	void NUpdateUIBBox_Void(NUI *pui);

	// Private
	NUI_EVENT_RESULT NDefaultUIEvent_Handle(NUI *pui, const NEVENT *pevent);
	void NDefaultUIEventToState_Handle(NUI *pui, const NEVENT *pevent);
	void NDefaultUIState_Handle(NUI *pui, const NTIME *ptime);
	void NUpdateUIState_Recursive(NUI *pui, const NTIME *ptime);

#define NIsUICaptureTouchMoveIn(pui) (ISFLAG_ON((pui)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN))
#define NIsUICaptureTouchMoveInDummy(pui) (FLAGS_TEST((pui)->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START | FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN | FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN))
	Nbool NAreUIChildrenInterestedByTouchMoveIn(const NUI *pui);
	Nbool NAreActiveUIRootsInterestedByTouchMoveIn();
	void NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(NUI *pui);
	void NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(NUI *pui);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUI_H
