#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_H
#define __NUT_EDITOR_ATXC_GIZMO_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Gizmo.h									**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../NFlags.h"
#include "../../Maths/NVec3f32.h"
#include "../../Miscellaneous/NColor.h"
#include "../../Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_ ...
// +
// + Used by user to describe the ATxC Gizmo he wants
// +
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START		BIT_0	// Usually the NUT_EDITOR_ATXC_ENTITY.GetElements_CallBack is called after a valid entity was picked. But, by setting this flag to ON, "GetElements_CallBack" of the default Entity is going to be called right at the tool start with a NULL pointer for SELECTION. 
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_KEEP_SELECTION									BIT_1	// Each time User Touch_Down, Gizmo Tool try to pick a new Entity if selection fails then Gizmo is deactivated and draw itself as a simple Rect with the Passive color. But with FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_KEEP_SELECTION set to ON, previous valid selection remains valid( it's like there was no selection fail ).
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_MOVE_TO_PICK_POSITION							BIT_3	// GIZMO Takes place to the mouse pick position only if a valid selection occurs !

#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DRAG_HANDLE_SIZE						BIT_4	// GIZMO Main Rect Size. Used to Drag Gizmo and used as the representation of the current texture USedOAARect
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_GIZMO_HANDLE_SIZE						BIT_5	// Size used to define all the "Handles" which are in the Gizmo (texture,blend,material, arrows ...)
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_PASSIVECOLOR							BIT_6	// Color used to draw asleep Gizmo. 
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_ACTIVECOLOR							BIT_7	// Color used to draw Active but not Used Gizmo parts. 
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_INUSECOLOR								BIT_8	// Color used to draw Active and Used Gizmo parts. 
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DEFAULT_POSITION						BIT_9	// Gizmo default position.
#define FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_USEPARAM_DEFAULT_ENTITY_ID						BIT_10	// Gizmo Default Entity ID is going to be used if FLAG_NUT_EDITOR_ATXC_GIZMO_DESC_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START is set to ON. At Gizmo setup this Default ID is set to NVOID by NEngine (and will make a crash because it's not a valid ID !). SO USER HAS TO SETUP a valid Default Entity ID if he wants ATxC to use it !

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + FLAG_NUT_EDITOR_ATXC_GIZMO_ ...
// +
// + Used by Engine to manage ATxC Gizmo 
// +
//#define FLAG_NUT_EDITOR_ATXC_GIZMO_xxx											BIT_0	// available !
#define FLAG_NUT_EDITOR_ATXC_GIZMO_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START		BIT_1	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
#define FLAG_NUT_EDITOR_ATXC_GIZMO_KEEP_SELECTION									BIT_2	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
#define FLAG_NUT_EDITOR_ATXC_GIZMO_MOVE_TO_PICK_POSITION							BIT_3	// GIZMO Takes place to the mouse pick position only if a valid selection occurs !



#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING								BIT_4
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVE_AXIS_RESTRICTION				BIT_5
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_X_OFF								BIT_6
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_Y_OFF								BIT_7
// -------------------------------------------------------------------------------------------------------------------------------------------
// GIZMO FLAGS used in real time by engine to describe GIZMO edition capabilities at a specific moment.
// this capabilities is connected with the User askings (Authorizations), and the current TXUV element member settings.
// for example: User asked for UVs and Textures Edition, but right now, user using Gizmo to setup texture to NULL (No Texture) so ...
// at the same time Gizmo UVS are automatically set to OFF (non editable) because of the texture lack of, even if user asked before for UVs Edition.
#define FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE											BIT_8
#define FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON										BIT_9	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON									BIT_10	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON										BIT_11
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON									BIT_12
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON								BIT_13

#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON									BIT_14
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON								BIT_15
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON								BIT_16
#define FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON								BIT_17
#define FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON											BIT_18	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
#define FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON										BIT_19	// USER CAN CHANGE CURRENT TEXTURE USING GIZMO TEXTURES RING
// GIZMO FLAGS used in reel time by engine to describe GIZMO status in a different way
// than the State system.
// These flags mean " USER is UPDATING ... "
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO									BIT_20
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GEOMSPECS								BIT_21
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT								BIT_22
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE									BIT_23
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_WRAP							BIT_24
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_FILTER							BIT_25
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORDSET								BIT_26
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET							BIT_27
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE							BIT_28
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET							BIT_29
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND									BIT_30
#define FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_MATERIAL								BIT_31

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + CONSTANT and DEFAULT VALUES ...
// +

	// + CONSTANT
#define CONSTANT_NUT_EDITOR_ATXC_SUBSIZEFACTOR				0.4f	// Ratio From Gizmo Handle Size to calculate "UVReset Rect size"
#define CONSTANT_NUT_ATXC_ARC_START							5.0f	// Angle (degree). There are 4 Arc, each of them starts at this angle from the previous multiple of PI/2 angle.
#define CONSTANT_NUT_ATXC_ARC_LENGTH						(90.0f-2.0f*CONSTANT_NUT_ATXC_ARC_START)

	// + CONSTANT STRINGS
#define CONSTANT_NUT_EDITOR_ATXC__GIZMO_OFF							"OFF LINE"
#define CONSTANT_NUT_EDITOR_ATXC__NOBLEND							"NO BLEND"
#define CONSTANT_NUT_EDITOR_ATXC__NOTEXTURE							"NO TEXTURE"
#define CONSTANT_NUT_EDITOR_ATXC__NOMATERIAL						"NO MATERIAL"

#define CONSTANT_NUT_EDITOR_ATXC__WRAP_GL_CLAMP_TO_BORDER			"GL_CLAMP_TO_BORDER"
#define CONSTANT_NUT_EDITOR_ATXC__WRAP_GL_CLAMP_TO_EDGE				"GL_CLAMP_TO_EDGE"
#define CONSTANT_NUT_EDITOR_ATXC__WRAP_GL_MIRRORED_REPEAT			"GL_MIRRORED_REPEAT"
#define CONSTANT_NUT_EDITOR_ATXC__WRAP_GL_REPEAT					"GL_REPEAT"

#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_NEAREST					"GL_NEAREST"
#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_LINEAR					"GL_LINEAR"
#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_NEAREST_MIPMAP_NEAREST	"GL_NEAREST_MIPMAP_NEAREST"
#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_LINEAR_MIPMAP_NEAREST	"GL_LINEAR_MIPMAP_NEAREST"
#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_NEAREST_MIPMAP_LINEAR	"GL_NEAREST_MIPMAP_LINEAR"
#define CONSTANT_NUT_EDITOR_ATXC__FILTER_GL_LINEAR_MIPMAP_LINEAR	"GL_LINEAR_MIPMAP_LINEAR"

// + DEFAULT VALUES
// +++ GIZMO DEFAULT POSITION
#define DEFAULT_NUT_EDITOR_ATXC_POSITION2F					0.5f,-0.5f

// +++ GIZMO ELEMENT SIZES ( different rectangles, arrow size and length, etc ... )
#define DEFAULT_NUT_EDITOR_ATXC_DRAG_HANDLE_SIZE_2F			0.125f,0.125f	// Size of "one uv" for Origin placement, in screen coord. system.
#define DEFAULT_NUT_EDITOR_ATXC_HANDLE_SIZE_2F				0.025f,0.025f	// Size of all Gizmo handles ( origin rect, uv scale arrows) in screen coord sytem.

// +++ GIZMO COLORS
#define DEFAULT_NUT_EDITOR_ATXC_PASSIVE_COLOR4F				NCOLOR_PRESET3F_ORANGE,1
#define DEFAULT_NUT_EDITOR_ATXC_ACTIVE_COLOR4F				NCOLOR_PRESET3F_YELLOW,1
#define DEFAULT_NUT_EDITOR_ATXC_INUSE_COLOR4F				NCOLOR_PRESET3F_WHITE,1
// All the different GIZMO states depending on user activity ...
// if user press/use a specific Gizmo element, state changes ...
#define NUT_EDITOR_ATXC_GIZMO_STATE_VOID					0
#define NUT_EDITOR_ATXC_GIZMO_STATE_GIZMO					1
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_OFFSET			2
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_U		3
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_V		4
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_U		5
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_V		6
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED	7
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED	8
//#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SET			9 TODO !!!
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE					10
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_S			11
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_T			12
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MIN_FILTER		13
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MAG_FILTER		14
#define NUT_EDITOR_ATXC_GIZMO_STATE_TEXTUREUNIT				15
#define NUT_EDITOR_ATXC_GIZMO_STATE_BLEND					16
#define NUT_EDITOR_ATXC_GIZMO_STATE_MATERIAL				17
#define NUT_EDITOR_ATXC_GIZMO_STATE_GEOMSPECS				18

// ARC SELECTORS ID
#define NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID				0	
#define NUT_EDITOR_ATXC_ARC_SELECTOR_ID_TEXTUREUNIT			1 // |
#define NUT_EDITOR_ATXC_ARC_SELECTOR_ID_GEOMSPECS			2 // |_Trigonometric ordered
#define NUT_EDITOR_ATXC_ARC_SELECTOR_ID_BLEND				3 // |
#define NUT_EDITOR_ATXC_ARC_SELECTOR_ID_MATERIAL			4 // |

typedef struct  
{
	Nu32					Flags;

 	NVEC2				DragHandleSize;				// Screen coord system: Size of one UV (u AND v) in screen coord. Used only by UV Origin edition.
	NVEC2				HandleSize;					// Screen coord system: Size of all the Gizmo handle
	NCOLOR					PassiveColor;
	NCOLOR					ActiveColor;				// Color of the whole Gizmo
	NCOLOR					InUseColor;					// Color of the active Gizmo Element when... it's active !

	NVEC2				SingleTexCoordExtents;
	NVEC2				DefaultPosition;
	Nu32					DefaultEntityID;
}NUT_EDITOR_ATXC_GIZMO_DESC;


typedef struct  
{
	Nu32					Flags;							// |
	Nu32					State;							// |

	// GIZMO
	NVEC2				Position;						// Gizmo Bottom Left Corner Screen Position
	NCOLOR					PassiveColor;					// |
	NCOLOR					ActiveColor;					// | The 3 color used to draw the Gizmo 
	NCOLOR					InUseColor;						// |

	// TexCoord
	NTEXCOORD_2f			TexCoordScaleSpeed;				// The current TexCoord Scale  speed controlled by Player	
	//NVEC2				SingleTexCoordExtents;			// To Draw Vertex Ticks

	// HANDLES
	NVEC2				DragHandleSize;					// Size of the Drag Handle (the central rect)
	NVEC2				HandleSize;						// Size of all the Handles, Drag Handle excepted !
	NVEC2				TextureHandlePosition;			// Position of the Texture Selector Handle (Position means Relative position from Gizmo Position !)
	NVEC2				TextureUnitHandlePosition;		// Position of the Texture Unit Selector Handle (Position means Relative position from Gizmo Position !)
	NVEC2				GeometryFormatHandlePosition;	// Position of the Geometry Format Selector Handle (Position means Relative position from Gizmo Position !)
	NVEC2				BlendHandlePosition;			// Position of the Blend Selector Handle (Position means Relative position from Gizmo Position !)
	NVEC2				MaterialHandlePosition;			// Position of the Material Selector Handle (Position means Relative position from Gizmo Position !)

	// MISC
	Nu8						EditedTextureUnit;
	Nu8						EditedTexCoordSetID;
	// 	Nu16				Available_Nu16;
	NVEC3				*SingleTexCoord_pEditedVertexPosition;
	NTEXCOORD_2f			*SingleTexCoord_pEditedVertexTexCoord;
	Nu32					SingleTexCoord_EditedVertexIndex;
	// Default:
	NVEC2				DefaultPosition;
	Nu32					DefaultEntityID;
}NUT_EDITOR_ATXC_GIZMO;

NUT_EDITOR_ATXC_GIZMO*	NUT_ATxC_SetUp_Gizmo(NUT_EDITOR_ATXC_GIZMO *pgizmo, const NUT_EDITOR_ATXC_GIZMO_DESC *pdesc );
void					NUT_ATxC_Clear_Gizmo(void *pgizmo);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR

