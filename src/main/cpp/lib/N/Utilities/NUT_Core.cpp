#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/NViewport.h"

#include "lib/N/Utilities/NUT_UIDesk_Set.h"
#include "lib/N/Utilities/NUT_UIDesk_Panel.h"

#include "lib/N/Utilities/NUT_UIPresets.h"
#include "lib/N/Utilities/NUT_Core.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
// #include "lib/N/Utilities/Draw/NUT_DrawText.h"

// Globales
Nu32 NUtilitiesCoreFlags = 0;

/*

NGEOMETRY		*NUT_pCurrentDrawableShapeGeometry;
NORTHONORMALBASIS	NUT_DrawOrthonormal;
*/
extern NGEOMETRY NUT_DrawableShapeGeometry[];

void NUT_Init()
{
	Nu32 i;
	NGEOMETRY_DESC gdesc;

	// Initialize Global Values
	for (i = 0; i < NUT_DRAW_RENDERING_MODE_ENUM_SIZE; i++)
	{
		// Initialize ALL THE NUT Geometry
		NFillupGeometryDesc(&gdesc, NGEOMETRY_FORMAT_COLORLINE_TU0_V3C4_LINE, 0,
							NUT_DRAW_DEFAULT_PRIMITIVE_CAPACITY,
							NUT_DRAW_DEFAULT_PRIMITIVE_CAPACITY * 2);
		NSetupGeometry(&NUT_DrawableShapeGeometry[i], &gdesc);
	}

	// Initialize UI Creation
	NUT_InitializeUIDeskSet();
	NUT_UIDesk_InitializePanel();

	NUT_Initialize_DrawTextCharTable();
}

void NUT_Disable()
{
	Nu16 i = NUT_DRAW_RENDERING_MODE_ENUM_SIZE;

	// Clear Global Lists
	while (i--)
	{
		NClearGeometry(&NUT_DrawableShapeGeometry[i]);
	}
}
