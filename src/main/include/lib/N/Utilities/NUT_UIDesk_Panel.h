#ifndef __NUT_UIDESK_PANEL_H
#define __NUT_UIDESK_PANEL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NUT_UIDesk_Panel.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/UI/NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum
{
	NUT_UIDESK_LINE_PREVIOUS_UI,		// for the current/next insertion Parent is the previous inserted UI
	NUT_UIDESK_LINE_FULL_LINE,			// for the current/next insertion Parent is the whole Line with it whole size.
	// --------------------------------
	NUT_UIDESK_LINE_REFTYPE_ENUM_SIZE
}NUT_UIDESK_LINE_REFTYPE_ENUM;

#define FLAG_NUT_UIDESK_PANEL_IS_OPEN				BIT_0
#define FLAG_NUT_UIDESK_PANEL_HAS_UI				BIT_1	// set to ON when the first UI is inserted.
#define FLAG_NUT_UIDESK_PANEL_IS_PARENT				BIT_2	// set to ON when the first UI is inserted.

typedef struct  
{
/*
	NUIPLACEMENT				PlacementDesc[NUT_UIDESK_LINE_REFTYPE_ENUM_SIZE];
	NUIPLACEMENT				CurrentPlacement;
	NUT_UIDESK_LINE_REFTYPE_ENUM	CurrentParent;
*/
	Nu32				Flags;
	NTRANSFORM_HNODE	*pParentThn;

	NVEC3			Panel_WorldPosition;
	NVEC3			Panel_Size;
	
	NVEC3			PreviousUI_WorldPosition;
	NVEC3			PreviousUI_Size;
	
	NVEC3			UI_ParentAnchor;		// To place Origin relatively from Previous UI or from the whole line
	NVEC3			UI_RelativePosition;

}NUT_UIDESK_PANEL;

void		NUT_UIDesk_OpenPanel(const NVEC3 *poriginposition);
inline void NUT_UIDesk_OpenPanelf(const Nf32 origin_x,const Nf32 origin_y,const Nf32 origin_z){NVEC3 origin;NVec3Set(&origin,origin_x,origin_y,origin_z);NUT_UIDesk_OpenPanel(&origin);} 
void		NUT_UIDesk_ClosePanel();
void		NUT_UIDesk_InsertPanelBlankSpace(const NVEC3 *pblank_size);
inline void NUT_UIDesk_InsertPanelBlankSpacef(const Nf32 x,const Nf32 y,const Nf32 z){NVEC3 blank_size;NVec3Set(&blank_size,x,y,z);NUT_UIDesk_InsertPanelBlankSpace(&blank_size);} 

void		NUT_UIDesk_SetPanelUIParentAnchor(const NVEC3 *panchor);
void		NUT_UIDesk_SetPanelUIParentAnchorf(const Nf32 parent_anchor_x,const Nf32 parent_anchor_y,const Nf32 parent_anchor_z);
void		NUT_UIDesk_SetPanelUIRelativePosition(const NVEC3 *prelativepos);
void		NUT_UIDesk_SetPanelUIRelativePositionf(const Nf32 x,const Nf32 y,const Nf32 z);
void		NUT_UIDesk_PanelIsParent(const Nbool bfrom_panel);

void		NUT_UIDesk_SetInPanelUIPositionf(NUI *puicontroller, const NVEC3 *puicontrollersize, const Nf32 default_posx, const Nf32 default_posy, const Nf32 default_posz);

// PRIVATE : LOWLEVEL use only
void		NUT_UIDesk_InitializePanel();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_UIDESK_PANEL_H 

