#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "lib/N/NCStandard.h"
#include "NUT_Editor_ATxC_Tool.h"
#include "NUT_Editor_ATxC_Gizmo_TextureUnit_InLine.h"

void NUT_ATxC_SetTextureUnitHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	NVEC2	vex;
	Nf32		_cos,_sin;
	Nu32		sectornb = NUT_ATxC_GetTextureUnitNumber(&ptooldata->Elements);

	NErrorIf( !sectornb,NERROR_NULL_VALUE );
	NErrorIf( ptooldata->HUDGizmo.EditedTextureUnit >= sectornb, NERROR_VALUE_OUTOFRANGE );

	Nf32 angle=(NUT_EDITOR_ATXC_ARC_SELECTOR_ID_TEXTUREUNIT-1.0f)*90.0f + CONSTANT_NUT_ATXC_ARC_START + ((Nf32)ptooldata->HUDGizmo.EditedTextureUnit+0.5f)*CONSTANT_NUT_ATXC_ARC_LENGTH/(Nf32)sectornb;
	NFastCosSin( NDEGtoANGLE(angle), &_cos,&_sin );
	NUT_ATxC_GetArcSelector_Extents( &vex, &ptooldata->HUDGizmo );
	ptooldata->HUDGizmo.TextureUnitHandlePosition.x = vex.x * _cos;
	ptooldata->HUDGizmo.TextureUnitHandlePosition.y = vex.y * _sin;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
