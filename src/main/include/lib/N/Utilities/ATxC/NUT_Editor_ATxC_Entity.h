#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_ENTITY_H
#define __NUT_EDITOR_ATXC_ENTITY_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Entity.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NEditor.h"
#include "NUT_Editor_ATxC_Updata.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NUT_EDITOR_ATXC_ENTITY_DESC.Flags
// +
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_ENTRYPTR BIT_0
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_TEXTUREPTR BIT_1
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_ATLASELEMENTPTR BIT_2
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_BLENDPTR BIT_3
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_PARRAY_OF_MATERIALPTR BIT_4
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_OFFSET_TOUCHSCALE BIT_5
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_OFFSET_MINMAX BIT_6
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_TOUCHSCALE BIT_7
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_BASESCALE BIT_8
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_SPEED_SWITCH_THRESHOLD BIT_9
#define FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_EDITING_TXC_SCALE_SPEED_MAX BIT_10

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + NUT_EDITOR_ATXC_ENTITY_DESC
	// +	Flags						several 'UseParam' flags ( FLAG_NUT_EDITOR_ATXC_ENTITY_DESC_USEPARAM_... )
	// +	Pick_And_Start_CallBack		Function called each time user touch screen, to look for an entity.
	// +	GetElements_CallBack		Function called to get data (elements) from selected entity or from default entity.
	// +	Update_CallBack				If an entity is selected and if user works on it. Engine calls this function with the appropriate updatedata structure, each time an update occurs.
	// +
	// +	pEntry						For this entity, what is the entry point to scan and pick an entity. ( often an array or a node list )
	// +	pArrayOfTexturePtr[]		|_The Textures Ring Selector helps user to choose 1 texture or/and 1 Atlas Element. This element is going to be choose from a specific array of textures/AtlasElement, specifics for each texture unit of each entity. If one is NVOID, engine will use the whole NUT_TextureLibrary or the Whole Atlas. If one is NULL, NEngine is going to consider there is No available Texture for this texture unit of this entity.
	// +	pArrayOfAtlasElementPtr[]	| Textures and Atlas Elements are going to be all together in the Texture Ring Selector.
	// +	pArrayOfBlendPtr
	// +	pArrayOfMaterialPtr
	typedef Nbool NUT_EDITOR_ATXC_RESULT;

	typedef NUT_EDITOR_ATXC_RESULT (*NUT_EDITOR_ATXC_UPDATE_CALLBACK)(const NEDITOR_SELECTION *pselect, const NUT_EDITOR_ATXC_UPDATA *pupdated_data);
	typedef NUT_EDITOR_ATXC_RESULT (*NUT_EDITOR_ATXC_PICK_CALLBACK)(NEDITOR_SELECTION *pselect, const NVEC2s16 *pmouse2d, const void *pentry);
	typedef NUT_EDITOR_ATXC_RESULT (*NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK)(NUT_EDITOR_ATXC_ELEMENTS *pelement_data, const NEDITOR_SELECTION *pselect);

	typedef struct
	{
		Nu32 Flags;
		NUT_EDITOR_ATXC_PICK_CALLBACK Pick_CallBack;								  //
		NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK GetElements_CallBack;					  //
		NUT_EDITOR_ATXC_UPDATE_CALLBACK Update_CallBack;							  //
		void *pEntry;																  // Pointer on the "entry point" pass to the Pick callback. It's where the pick callback is supposed to look for ...
		NARRAY *pArrayOfTexturePtr[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];	  // Arrays of all the possible textures for each possible Texture unit for this entity. They are pointers on arrays of texture pointers or NVOID or NULL
		NARRAY *pArrayOfAtlasElementPtr[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER]; // Arrays of all the possible textures for each possible Texture unit for this entity. They are pointers on arrays of texture pointers or NVOID or NULL
		NARRAY *pArrayOfBlendPtr;													  // Array of all the possible Blends for this entity.
		NARRAY *pArrayOfMaterialPtr;												  // Array of all the possible Materials for this entity.

		NVEC2 EditingTxCOffset_TouchScale; //|_ To scale the 'Touch' (pointer) position input, used to be more precise
		NVEC2 EditingTxCScale_TouchScale;  //|

		// Representation of TxCScale is suppose to be relative to the GIZMO DragHandleSize.
		// Basically, '1 TxCScale = DragHandleSize' (for X and Y because DragHandle is always a SQUARE).
		// But for practical reasons, it's possible to change that, thanks to 'EditingTxCScale_BaseScale'.
		// For example with  EditingTxCScale_BaseScale = 2, '1 TxCScaleUV' will have a graphical representation (Arrows)
		//	2 times bigger than 'DragHandleSize'.
		NVEC2 EditingTxCScale_BaseScale;

		// Representation of TxCOffset is suppose to be relative to the GIZMO DragHandleSize, like TxCScale.
		// Basically, '1 TxCOffset = DragHandleSize' (for X and Y because DragHandle is always a SQUARE).
		// But, it could be necessary to manage a wider range of TxCOffset value.
		// For TxCScale user give explicitly a Scale to do this... and it's because all TxCScale values are always >= 0 and have no limits ( thanks to TxC Speed Editing system ).
		// For TxCOffset it's easier to specify min/max values and lets Engine calculate the associated scale.
		Nf32 EditingTxCOffset_Min; // Minimum values for TxCOffset. (TxCOffset Min X = TxCOffset Min Y = EditingTxCOffset_Min)
		Nf32 EditingTxCOffset_Max; // Maximum values for TxCOffset. (TxCOffset Max X = TxCOffset Max Y = EditingTxCOffset_Max)

		NTEXCOORD_2f EditingTxCScale_SpeedSwitchThreshold; // EditingTxCScale system switch to 'Speed edition mode' if current Edited TxC value is bigger than EditingTxCScale_SpeedSwitchThreshold,
		NTEXCOORD_2f EditingTxCScale_SpeedMax;			   // The Max TxC speed values for TexCoord.x and TexCoord.y speed
	} NUT_EDITOR_ATXC_ENTITY_DESC;
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
//
#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_MIN_1F -1.0f // Used to limit "TxCOffset X and Y" values.
#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_MAX_1F 1.0f  // Minimum values will match with the upper left cornet of the Central Gizmo Rect, and Max with the lower right corner of the same rect

#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_OFFSET_TOUCHSCALE_2F 1.0f, 1.0f // Used to scale "Touch move" differential coordinates during Texcoord offset editing.
#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_TOUCHSCALE_2F 1.0f, 1.0f  // Used to scale "Touch move" differential coordinates during Texcoord Scale editing.
#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_BASESCALE_2F 1.0f, 1.0f   // Used to modify Scale Base (which is initially like Offset Base rule is 'Scale base = Offset Base * scaleBaseFactor)

#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_SPEED_SWITCH_THRESHOLD_2F 2.0f, 2.0f // Until it UVScale system switch from absolute to speed integration. (UV coord. system)
#define DEFAULT_NUT_EDITOR_ATXC_ENTITY_EDITING_TXC_SCALE_SPEED_MAX_2F 4.0f, 4.0f			  // UV coord system. number of UV/second in UV speed edition mode

	typedef struct
	{
		Nu32 Flags;
		NUT_EDITOR_ATXC_PICK_CALLBACK Pick_CallBack;				//
		NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK GetElements_CallBack; //
		NUT_EDITOR_ATXC_UPDATE_CALLBACK Update_CallBack;			//

		void *pEntry; // Pointer on the "entry point" pass to the Pick callback. It's where the pick callback is supposed to look for ...

		// Limits
		Nf32 EditingTxCOffset_Min; // Minimum values for TxCOffset. (TxCOffset Min X = TxCOffset Min Y = EditingTxCOffset_Min)
		Nf32 EditingTxCOffset_Max; // Maximum values for TxCOffset. (TxCOffset Max X = TxCOffset Max Y = EditingTxCOffset_Max)

		// Editing
		NVEC2 EditingTxCOffset_TouchScale;
		NVEC2 EditingTxCScale_TouchScale;

		NVEC2 EditingTxCScale_BaseScale;

		NTEXCOORD_2f EditingTxCScale_SpeedSwitchThreshold;
		NTEXCOORD_2f EditingTxCScale_SpeedMax; // The Max speed values for TexCoord.x and TexCoord.y speed

		NARRAY *pArrayOfBlendPtr;													  // Array of all the possible Blends for this entity.
		NARRAY *pArrayOfMaterialPtr;												  // Array of all the possible Materials for this entity.
		NARRAY *pArrayOfTexturePtr[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];	  // Array of all the possible textures for each possible Texture unit for this entity.
		NARRAY *pArrayOfAtlasElementPtr[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER]; // Array of all the possible textures for each possible Texture unit for this entity.
	} NUT_EDITOR_ATXC_ENTITY;

	// +
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ***************************************************************************************
	// ***************************************************************************************
	// **																					**
	// **									 												**
	// **									---												**
	// **							     Functions											**
	// **																					**
	// ***************************************************************************************
	// ***************************************************************************************
	NUT_EDITOR_ATXC_ENTITY *NUT_Editor_ATxC_SetupEntity(NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_ENTITY_DESC *pdesc);
	void NUT_Editor_ATxC_ClearEntity(NUT_EDITOR_ATXC_ENTITY *pentity);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_EDITOR_ATXC_ENTITY_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
