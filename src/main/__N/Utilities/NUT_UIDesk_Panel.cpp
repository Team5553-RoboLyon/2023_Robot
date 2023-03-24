#include "../NCStandard.h"
#include "../UI/NUI.h"
#include "../UI/NUIPlacement.h"
#include "NUT_UIDesk_Set.h"
#include "NUT_UIDesk_Dummy.h"
#include "NUT_UIDesk_Panel.h"

extern NUT_UIDESK_SET	*NUT_pCurrentUIDeskSet; // ... from NUT_UIDeskSet.cpp
NUT_UIDESK_PANEL			UIDeskPanel;

void NUT_UIDesk_InitializePanel()
{
	Nmem0(&UIDeskPanel,NUT_UIDESK_PANEL);
	// By default we assume line purpose is stacking UI from left to right ... 
	// NVec3Set(&UIDeskPanel.UI_PositionAnchor,ANCHOR_LEFT,ANCHOR_TOP,ANCHOR_BACK);
	NVec3Set(&UIDeskPanel.UI_ParentAnchor,ANCHOR_RIGHT,ANCHOR_TOP,ANCHOR_BACK);
}

void NUT_UIDesk_OpenPanel(const NVEC3 *pline_worldposition)
{
	NErrorIf(ISFLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN),NERROR_NUT_UIDESK_PANEL_IS_OPEN );
	FLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN);
	FLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_HAS_UI);
	UIDeskPanel.pParentThn = NULL;

	// Reset Line 
	UIDeskPanel.Panel_WorldPosition = *pline_worldposition;
	NVec3Set(&UIDeskPanel.Panel_Size,0,0,0);

	UIDeskPanel.PreviousUI_WorldPosition = *pline_worldposition;
	NVec3Set(&UIDeskPanel.PreviousUI_Size,0,0,0);
}

void NUT_UIDesk_ClosePanel()
{
	NErrorIf(ISFLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN),NERROR_NUT_UIDESK_PANEL_IS_CLOSE );
	FLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN);
	FLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_HAS_UI);
}

void NUT_UIDesk_InsertPanelBlankSpace(const NVEC3 *pblank_size)
{
	NErrorIf(!pblank_size,NERROR_NULL_POINTER);
	if( ISFLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN) && pblank_size )
	{
		NVEC3 worldpos; // world ... of the line.
		// 1/ "Raw" position without taking care of UI Position Anchoring  
		if(ISFLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_PARENT))
		{
			worldpos.x = UIDeskPanel.Panel_WorldPosition.x + UIDeskPanel.Panel_Size.x*UIDeskPanel.UI_ParentAnchor.x + UIDeskPanel.UI_RelativePosition.x;
			worldpos.y = UIDeskPanel.Panel_WorldPosition.y + UIDeskPanel.Panel_Size.y*UIDeskPanel.UI_ParentAnchor.y + UIDeskPanel.UI_RelativePosition.y;
			worldpos.z = UIDeskPanel.Panel_WorldPosition.z + UIDeskPanel.Panel_Size.z*UIDeskPanel.UI_ParentAnchor.z + UIDeskPanel.UI_RelativePosition.z;
		}
		else
		{
			worldpos.x = UIDeskPanel.PreviousUI_WorldPosition.x + UIDeskPanel.PreviousUI_Size.x*UIDeskPanel.UI_ParentAnchor.x + UIDeskPanel.UI_RelativePosition.x;
			worldpos.y = UIDeskPanel.PreviousUI_WorldPosition.y + UIDeskPanel.PreviousUI_Size.y*UIDeskPanel.UI_ParentAnchor.y + UIDeskPanel.UI_RelativePosition.y;
			worldpos.z = UIDeskPanel.PreviousUI_WorldPosition.z + UIDeskPanel.PreviousUI_Size.z*UIDeskPanel.UI_ParentAnchor.z + UIDeskPanel.UI_RelativePosition.z;
		}
		// 2/ For a Blank space there is no integration of "position anchoring".
		// 3/... we need to store the Left/Top/Back corner World Position of the UI to work ... 
		//	 ... So, we have to remove Build Anchor for that...
		UIDeskPanel.PreviousUI_WorldPosition.x = worldpos.x;
		UIDeskPanel.PreviousUI_WorldPosition.y = worldpos.y;
		UIDeskPanel.PreviousUI_WorldPosition.z = worldpos.z;
		UIDeskPanel.PreviousUI_Size = *pblank_size;

		NUT_UIDesk_StoreUIDummyData(&worldpos,pblank_size);

		// ... And we need to update Panel Size and maybe Panel World Position !!
		// 'Panel World Position' represents WHERE the LEFT/TOP/BACK corner of the AABB Panel is in the WORLD. 
		// and obviously Panel Size represents Panel AABB size...
		NAABB	panel_aabb,aabb;
		panel_aabb.VMin = UIDeskPanel.Panel_WorldPosition;
		panel_aabb.VMin.y -= UIDeskPanel.Panel_Size.y;
		NVec3Add(&panel_aabb.VMax,&panel_aabb.VMin,&UIDeskPanel.Panel_Size);

		aabb.VMin = UIDeskPanel.PreviousUI_WorldPosition;
		aabb.VMin.y -= UIDeskPanel.PreviousUI_Size.y;
		NVec3Add(&aabb.VMax,&aabb.VMin,&UIDeskPanel.PreviousUI_Size);

		NAABB_IncludeAABBInto(&panel_aabb,&aabb);
		NGetAABBSize(&UIDeskPanel.Panel_Size,&panel_aabb);
		UIDeskPanel.Panel_WorldPosition = panel_aabb.VMin;
		UIDeskPanel.Panel_WorldPosition.y += UIDeskPanel.Panel_Size.y;
	}
}

void NUT_UIDesk_SetInPanelUIPositionf(NUI *puiframe, const NVEC3 *puiframesize, const Nf32 default_posx, const Nf32 default_posy, const Nf32 default_posz)
{
	NErrorIf(!puiframe,NERROR_NULL_POINTER);

	if( ISFLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN) && puiframesize )
	{
// 		NErrorIf( NVec3Length(puiframesize)<= NF32_EPSILON_VECTOR_LENGTH,NERROR_NUT_UIDESK_PANEL_REQUIRES_CONSISTENT_SIZE )
		if( ISFLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_HAS_UI) )
		{
			UIDeskPanel.pParentThn = (NTRANSFORM_HNODE *)puiframe->pRenderable->pTransformHNode->pParent;
			FLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_HAS_UI);
		}

		// All the UIs inserted into the same open line must share the same THN parent hierarchy...
		// If not ... CRASH
		NErrorIf(UIDeskPanel.pParentThn != puiframe->pRenderable->pTransformHNode->pParent,NERROR_NUT_UIDESK_PANEL_UI_MUST_HAVE_THE_SAME_THN_HIERARCHY);
		
		// Calculate World Position of the new incoming UI.
		// Be aware that position is the one useful by the uiframe itself and doesn't integrate yet local Anchor shifting.
		// Because this one will be done by the UI itself at creation (Obviously!)...
		// ...
		NVEC3 worldpos; // world ... of the line.
		// 1/ "Raw" position without taking care of UI Position Anchoring  
		if(ISFLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_PARENT))
		{
			worldpos.x = UIDeskPanel.Panel_WorldPosition.x + UIDeskPanel.Panel_Size.x*UIDeskPanel.UI_ParentAnchor.x + UIDeskPanel.UI_RelativePosition.x;
			worldpos.y = UIDeskPanel.Panel_WorldPosition.y + UIDeskPanel.Panel_Size.y*UIDeskPanel.UI_ParentAnchor.y + UIDeskPanel.UI_RelativePosition.y;
			worldpos.z = UIDeskPanel.Panel_WorldPosition.z + UIDeskPanel.Panel_Size.z*UIDeskPanel.UI_ParentAnchor.z + UIDeskPanel.UI_RelativePosition.z;
		}
		else
		{
			worldpos.x = UIDeskPanel.PreviousUI_WorldPosition.x + UIDeskPanel.PreviousUI_Size.x*UIDeskPanel.UI_ParentAnchor.x + UIDeskPanel.UI_RelativePosition.x;
			worldpos.y = UIDeskPanel.PreviousUI_WorldPosition.y + UIDeskPanel.PreviousUI_Size.y*UIDeskPanel.UI_ParentAnchor.y + UIDeskPanel.UI_RelativePosition.y;
			worldpos.z = UIDeskPanel.PreviousUI_WorldPosition.z + UIDeskPanel.PreviousUI_Size.z*UIDeskPanel.UI_ParentAnchor.z + UIDeskPanel.UI_RelativePosition.z;
		}
		// 2/ ... and integrate position anchoring into after "removing" UI build Anchor.
		// we are doing here exactly what the function "NUT_UIDesk_SetUIPositionf" does.
		// But, due to the next Move (see "3/") its better to transform "worldpos" here.
		worldpos.x += (NUT_pCurrentUIDeskSet->BuildAnchor.x - NUT_pCurrentUIDeskSet->PositionAnchor.x)*puiframesize->x;
		worldpos.y += (NUT_pCurrentUIDeskSet->BuildAnchor.y - NUT_pCurrentUIDeskSet->PositionAnchor.y)*puiframesize->y;
		worldpos.z += (NUT_pCurrentUIDeskSet->BuildAnchor.z - NUT_pCurrentUIDeskSet->PositionAnchor.z)*puiframesize->z;
		NSetUIPosition(puiframe,&worldpos);

		// 3/... we need to store the Left/Top/Back corner World Position of the UI to work ... 
		//	 ... So, we have to remove Build Anchor for that...
		UIDeskPanel.PreviousUI_WorldPosition.x = worldpos.x - NUT_pCurrentUIDeskSet->BuildAnchor.x*puiframesize->x;
		UIDeskPanel.PreviousUI_WorldPosition.y = worldpos.y - NUT_pCurrentUIDeskSet->BuildAnchor.y*puiframesize->y;
		UIDeskPanel.PreviousUI_WorldPosition.z = worldpos.z - NUT_pCurrentUIDeskSet->BuildAnchor.z*puiframesize->z;
		UIDeskPanel.PreviousUI_Size = *puiframesize;

		NUT_UIDesk_StoreUIDummyData(&UIDeskPanel.PreviousUI_WorldPosition,puiframesize);

		// ... And we need to update Panel Size and maybe Panel World Position !!
		// 'Panel World Position' represents WHERE the LEFT/TOP/BACK corner of the AABB Panel is in the WORLD. 
		// and obviously Panel Size represents Panel AABB size...
		NAABB	panel_aabb,aabb;
		panel_aabb.VMin = UIDeskPanel.Panel_WorldPosition;
		panel_aabb.VMin.y -= UIDeskPanel.Panel_Size.y;
		NVec3Add(&panel_aabb.VMax,&panel_aabb.VMin,&UIDeskPanel.Panel_Size);

		aabb.VMin = UIDeskPanel.PreviousUI_WorldPosition;
		aabb.VMin.y -= UIDeskPanel.PreviousUI_Size.y;
		NVec3Add(&aabb.VMax,&aabb.VMin,&UIDeskPanel.PreviousUI_Size);

		NAABB_IncludeAABBInto(&panel_aabb,&aabb);
		NGetAABBSize(&UIDeskPanel.Panel_Size,&panel_aabb);
		UIDeskPanel.Panel_WorldPosition = panel_aabb.VMin;
		UIDeskPanel.Panel_WorldPosition.y += UIDeskPanel.Panel_Size.y;
	}
	else // Panel is Closed : ISFLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_OPEN) or/and puiframe == NULL 
	{
		NUT_UIDesk_SetUIPositionf(puiframe,puiframesize, default_posx,default_posy,default_posz);
		//>>>'NUT_UIDesk_StoreUIDummyData' is going to be done inside the previous function.
	}
}

void NUT_UIDesk_SetPanelUIParentAnchor(const NVEC3 *panchor)
{
	UIDeskPanel.UI_ParentAnchor = *panchor;
}
void NUT_UIDesk_SetPanelUIParentAnchorf(const Nf32 parent_anchor_x,const Nf32 parent_anchor_y,const Nf32 parent_anchor_z)
{
	NVec3Set(&UIDeskPanel.UI_ParentAnchor,parent_anchor_x,parent_anchor_y,parent_anchor_z);
}
void NUT_UIDesk_SetPanelUIRelativePosition(const NVEC3 *prelativepos)
{
	UIDeskPanel.UI_RelativePosition = *prelativepos;
}
void NUT_UIDesk_SetPanelUIRelativePositionf(const Nf32 x,const Nf32 y,const Nf32 z)
{
	NVec3Set(&UIDeskPanel.UI_RelativePosition,x,y,z); 
}

void NUT_UIDesk_PanelIsParent(const Nbool bfrom_panel)
{
	if(bfrom_panel)
	{
		FLAG_ON(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_PARENT);
	}
	else
	{
		FLAG_OFF(UIDeskPanel.Flags,FLAG_NUT_UIDESK_PANEL_IS_PARENT);
	}
}