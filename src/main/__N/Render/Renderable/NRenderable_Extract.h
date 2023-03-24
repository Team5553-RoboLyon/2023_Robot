#ifndef __NRENDERABLE_EXTRACT_H
#define __NRENDERABLE_EXTRACT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NRenderable_Extract.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "NRenderable.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum
{
	NRENDERABLE_EXTRACT_FCT_NULL	= 0,					// NULL pointer
	NRENDERABLE_EXTRACT_FCT_VOID,							// NRenderable_Extract_Void
// 	NRENDERABLE_EXTRACT_FCT_3D_VOID,						// NRenderable_Extract_3D_Void
// 	NRENDERABLE_EXTRACT_FCT_3D_VOID_FRUSTUMCULLING,			// NRenderable_Extract_3D_Void_FrustumCulling
// 	NRENDERABLE_EXTRACT_FCT_HUD_VOID,						// NRenderable_Extract_3D_Void_FrustumCulling

	NRENDERABLE_EXTRACT_FCT_USER0,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER1,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER2,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER3,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER4,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER5,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER6,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER7,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER8,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER9,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER10,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER11,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER12,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER13,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER14,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER15,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER16,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER17,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER18,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER19,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER20,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER21,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER22,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER23,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER24,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER25,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER26,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER27,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER28,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER29,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER30,							// User Function or "NRenderableExtract_void" (default value)
	NRENDERABLE_EXTRACT_FCT_USER31,							// User Function or "NRenderableExtract_void" (default value)

	// NEXTRACT_SECTION_STANDARD
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY,
// 	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_GEOMORDER,					// nonsense (no needs to ordered a single geometry !)but useful to simplify extract function identification process
// 	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_GEOMZ,						// nonsense (no needs to Z sort a single geometry, renderable Z sort is enough ) but useful to simplify extract function identification process
	
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES,
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMORDER,
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMZ,
	
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,
// 	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING_GEOMORDER,	// nonsense but useful (see above ^)
// 	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING_GEOMZ,		// nonsense but useful (see above ^)
	
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMORDER,	
	NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMZ,		

	NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,
	
	// NEXTRACT_SECTION_UI
	NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES,
	NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,
	NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,

	NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,

	// NEXTRACT_SECTION_UICHECKBOX
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES,
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,
	
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,

	// NEXTRACT_SECTION_UIGEOMETRYVIEWER
	//		Geometry VIEWER without flash 
	NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES,
	NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,
	NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,

	NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,

	//		Geometry VIEWER with flash
	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES,
	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,
	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,

	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,
	NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,

	//----------------------
	NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE
}NRENDERABLE_EXTRACT_FCT_ID_ENUM;

// NEXTRACT_SECTION_STANDARD
Nu32 NRenderableExtract_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomOrder(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomZ(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomOrder(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomZ(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

// NEXTRACT_SECTION_UI
Nu32 NRenderableExtract_UI_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UI_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UI_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UI_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_UI_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UI_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

// NEXTRACT_SECTION_UICHECKBOX
Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_UICheckBox_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICheckBox_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

// NEXTRACT_SECTION_UIGEOMETRYVIEWER
// No Flash
Nu32 NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_UICustom_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_UICustom_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

// Flash
Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);

Nu32 NRenderableExtract_Flash_UICustom_HUD_SingleInstance_SingleGeometry(const NRENDERABLE *prenderable, const void *powner);
Nu32 NRenderableExtract_Flash_UICustom_HUD_SingleInstance_MultiGeometries(const NRENDERABLE *prenderable, const void *powner);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NRENDERABLEUPDATEANDEXTRACT_H 

