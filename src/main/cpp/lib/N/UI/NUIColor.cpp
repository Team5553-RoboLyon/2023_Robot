#include "lib/N/NErrorHandling.h"
#include "lib/N/Ntext.h"
#include "lib/N/Geometry/NGeometryMisc.h"

#include "NUIColor.h"

void NSetUIColor(NUI *pui, const NUI_COLORSET_STATE_ENUM state)
{
	NErrorIf(ISFLAG_OFF(pui->Flags, FLAG_NUI_COLOR_UPDATE), NERROR_UI_COLOR_UPDATE_IS_OFF);

	NGEOMETRY *pgeom;

	// BKG
	if ((pgeom = NGetUIBackgroundGeometry(pui)))
		NSetGeometryVertexRangeColorRange(pgeom, NVERTEX_DATA_COLOR0, 0, pgeom->Mesh.VertexArray.Size, NGetUIColorSetPtr(&pui->pStyle->ColorSet, NUI_COLORSET_BKG, state), CONSTANT_NUI_COLORSET_COLORRANGE_SIZE);

	// ICON
	if ((pgeom = NGetUIIconGeometry(pui)))
		NSetGeometryVertexRangeColorRange(pgeom, NVERTEX_DATA_COLOR0, 0, pgeom->Mesh.VertexArray.Size, NGetUIColorSetPtr(&pui->pStyle->ColorSet, NUI_COLORSET_ICON, state), CONSTANT_NUI_COLORSET_COLORRANGE_SIZE);

	// TEXT
	if (pui->pText)
	{
		NSetTextColorRange(pui->pText, NGetUIColorSetPtr(&pui->pStyle->ColorSet, NUI_COLORSET_TEXT, state), CONSTANT_NUI_COLORSET_COLORRANGE_SIZE);
		NUpdateTextColor(pui->pText);
	}
}

void NSetUICheckUnCheckColor(NUI_CHECKBOX *puicb, const NUI_COLORSET_STATE_ENUM state)
{
	NErrorIf(ISFLAG_OFF(puicb->UI.Flags, FLAG_NUI_COLOR_UPDATE), NERROR_UI_COLOR_UPDATE_IS_OFF);
	// CHECK
	if (puicb->pCheckTag_Geometry)
		NSetGeometryVertexRangeColorRange(puicb->pCheckTag_Geometry, NVERTEX_DATA_COLOR0, 0, puicb->pCheckTag_Geometry->Mesh.VertexArray.Size, NGetUIColorSetPtr(&puicb->UI.pStyle->ColorSet, NUI_COLORSET_CHECK, state), CONSTANT_NUI_COLORSET_COLORRANGE_SIZE);
	// UNCHECK
	if (puicb->pUnCheckTag_Geometry)
		NSetGeometryVertexRangeColorRange(puicb->pUnCheckTag_Geometry, NVERTEX_DATA_COLOR0, 0, puicb->pUnCheckTag_Geometry->Mesh.VertexArray.Size, NGetUIColorSetPtr(&puicb->UI.pStyle->ColorSet, NUI_COLORSET_UNCHECK, state), CONSTANT_NUI_COLORSET_COLORRANGE_SIZE);
}