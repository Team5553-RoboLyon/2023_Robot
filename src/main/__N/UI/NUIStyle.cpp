#include "../NErrorHandling.h"
#include "NUIStyle.h"


NUI_STYLE *NSetUpUIStyle(NUI_STYLE *pstyle, const NUI_STYLE_DESC *desc)
{
	memset(pstyle,0,sizeof(NUI_STYLE));
	
	// Font
	// .............................................................
	pstyle->pFont = desc->pFont;
	if(!pstyle->pFont)
	{
		NErrorIf(1,NERROR_TEXT_FONT_MISSING);
		return NULL;
	}

/*
	// Margin Parameters
	// .............................................................
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_USEPARAM_MARGIN))
		pstyle->Margin = desc->Margin;
	else
		NVec3Set(&pstyle->Margin,DEFAULT_NUI_STYLE_MARGIN_X,DEFAULT_NUI_STYLE_MARGIN_Y,DEFAULT_NUI_STYLE_MARGIN_Z);
*/

	// Color Ranges Parameters
	// .............................................................
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_COLORSET_DESC) )
	{
		NSetupUIColorSet(&pstyle->ColorSet,&desc->ColorSetDesc);
	}
	else
	{
		NSetupUIColorSet(&pstyle->ColorSet,NULL);
	}

	// PUSH Animation Parameters
	// .............................................................
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_UPSPEED))
	{
		pstyle->Push_UpSpeed = desc->Push_UpSpeed;
	}
	else
	{
		pstyle->Push_UpSpeed = DEFAULT_NUI_STYLE_PUSH_UPSPEED;
	}

	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWNSPEED))
	{
		pstyle->Push_DownSpeed = desc->Push_DownSpeed;
	}
	else
	{
		pstyle->Push_DownSpeed = DEFAULT_NUI_STYLE_PUSH_DOWNSPEED;
	}
	
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_CANCELSPEED))
	{
		pstyle->Push_CancelSpeed = desc->Push_CancelSpeed;
	}
	else
	{
		pstyle->Push_CancelSpeed = DEFAULT_NUI_STYLE_PUSH_CANCELSPEED;
	}
	
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_TRANSLATION))
	{
		pstyle->Push_DownTranslation = desc->Push_DownTranslation;
	}
	else
	{
		NVec3Set(&pstyle->Push_DownTranslation,DEFAULT_NUI_STYLE_PUSH_DOWN_TRANSLATION_3F);
	}

	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_SCALE))
	{
		pstyle->Push_DownScale = desc->Push_DownScale;
	}
	else
	{
		NVec3Set(&pstyle->Push_DownScale,DEFAULT_NUI_STYLE_PUSH_DOWN_SCALE_3F);
	}
	
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_DOWN_QUATROTATION))
	{
		pstyle->Push_DownQuatRotation = desc->Push_DownQuatRotation;
	}
	else
	{
		NQuaternionRotation(&pstyle->Push_DownQuatRotation,DEFAULT_NUI_STYLE_PUSH_DOWN_ROTATION_3F);
	}
	
	if(ISFLAG_ON(desc->Creation_Mask,FLAG_NUI_STYLE_DESC_USEPARAM_PUSH_EASING_FUNCTION_ID))
	{
		pstyle->pPushUIEasingHandle = NGetEasingHandle(desc->Push_EasingFunctionID);
	}
	else
	{
		pstyle->pPushUIEasingHandle = NGetEasingHandle(DEFAULT_NUI_STYLE_EASING_FCT_ID);
	}

	return pstyle;
}

NUI_STYLE *NCreateUIStyle(const NUI_STYLE_DESC *desc)
{
	return NSetUpUIStyle(NEW(NUI_STYLE),desc);
}

void NClearUIStyle( NUI_STYLE *pstyle)
{
	memset(pstyle,0,sizeof(NUI_STYLE));
}

void NDeleteUIStyle( NUI_STYLE *pstyle)
{
	NClearUIStyle(pstyle);
	Nfree(pstyle);
}
