#ifndef __NUI_STYLE_H
#define __NUI_STYLE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUI_Style.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "lib/N/NType.h"
#include "../NFlags.h"
#include "lib/N/NMath.h"
#include "../NFont.h"
#include "../NQuaternion.h"
#include "lib/N/Containers/NNode.h"
#include "../Miscellaneous/NEasing.h"
#include "NUIColorSet.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************
// UI Style Constants

// UI Style Defaults
#define DEFAULT_NUI_STYLE_EASING_FCT_ID NEASING_INOUT
#define DEFAULT_NUI_STYLE_PUSH_UPSPEED 10.0f
#define DEFAULT_NUI_STYLE_PUSH_DOWNSPEED 10.0f
#define DEFAULT_NUI_STYLE_PUSH_CANCELSPEED 15.0f

#define DEFAULT_NUI_STYLE_PUSH_DOWN_TRANSLATION_3F 0.0f, 0.0f, 0.0f
#define DEFAULT_NUI_STYLE_PUSH_DOWN_SCALE_3F 0.92f, 0.92f, 0.0f
#define DEFAULT_NUI_STYLE_PUSH_DOWN_ROTATION_3F NDEGtoRAD(0.0f), NDEGtoRAD(0.0f), NDEGtoRAD(0.0f)

	typedef struct
	{
		NNODEDEF
		// Basic UI
		NFONT *pFont;

		// Color Set
		NUI_COLORSET ColorSet;

		// UI Animations
		NEASING_HANDLE pPushUIEasingHandle; // To ease/smooth the animation normalized ratio.
		Nf32 Push_UpSpeed;
		Nf32 Push_DownSpeed;
		Nf32 Push_CancelSpeed;

		NVEC3 Push_DownTranslation;		   // To store local translations of the Node
		NVEC3 Push_DownScale;			   // To Store local Scale of the Node
		NQUATERNION Push_DownQuatRotation; // To store local rotations of the Node

	} NUI_STYLE;

#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_UPSPEED BIT_0			  // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWNSPEED BIT_1		  // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_CANCELSPEED BIT_2		  // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_TRANSLATION BIT_3  // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_QUATROTATION BIT_4 // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_SCALE BIT_5		  // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_EASING_FUNCTION_ID BIT_6
#define FLAG_NUI_STYLE_DESC_USEPARAM_MARGIN BIT_7		 // Used by DESC only
#define FLAG_NUI_STYLE_DESC_USEPARAM_COLORSET_DESC BIT_8 // Used by DESC only

	typedef struct
	{
		Nu32 Creation_Mask;

		NFONT *pFont;
		NVEC3 Margin;
		// 	Nf32					fTransitionDuration;
		// 	Nf32					fDownScale;

		// Push Button Animations Params
		NEASING_HANDLE_ID_ENUM Push_EasingFunctionID;
		Nf32 Push_UpSpeed;
		Nf32 Push_DownSpeed;
		Nf32 Push_CancelSpeed;
		NVEC3 Push_DownTranslation;		   // To store local translations of the Node
		NVEC3 Push_DownScale;			   // To Store local Scale of the Node
		NQUATERNION Push_DownQuatRotation; // To store local rotations of the Node

		// Color Set
		NUI_COLORSET_DESC ColorSetDesc;

	} NUI_STYLE_DESC;

	NUI_STYLE *NSetUpUIStyle(NUI_STYLE *pstyle, const NUI_STYLE_DESC *desc);
	NUI_STYLE *NCreateUIStyle(const NUI_STYLE_DESC *desc);
	void NClearUIStyle(NUI_STYLE *pstyle);
	void NDeleteUIStyle(NUI_STYLE *pstyle);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUI_H
