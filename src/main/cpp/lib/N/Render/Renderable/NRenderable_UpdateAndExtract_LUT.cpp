#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NFlags.h"
#include "../NFrustum_Culling.h"
#include "../Accumulator/NAccumulator.h"

//#include "../../NObjects.h"
#include "../../Particles/NParticles.h"
#include "../../Particles/NHomingParticles.h"
#include "../../Particles/NAdvancedParticles.h"
#include "../../Particles/NAdvancedHomingParticles.h"

#include "../../Ntext.h"
#include "../../UI/NUI.h"
#include "../../UI/NUIRenderableUpdate.h"
#include "../../NStructure.h"
#include "../NRenderCell.h"

// #include "../../Collectibles/NCollectibles.h"
#include "../../UI/NUICheckBox.h"
#include "NRenderable_Extract.h"
#include "NRenderable_Update.h"

#include "NRenderable_UpdateAndExtract_LUT.h"

const NEXTRACTSET_KEY	NExtractSet_LUT[]=
{

// + NEXTRACTSET_SECTION_VOID
//	 (used by NENGINE to avoid some tests at creation. If there is nothing to extract,
//	  but user set ON HIGHLEVEL flags the 'NGetRenderableExtractFunctionByExtractSet' function will return  'NRENDERABLE_EXTRACT_FCT_VOID' )
{NRENDERABLE_EXTRACT_FCT_VOID,			NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,0)}, // ! its not NULL but VOID !
{NRENDERABLE_EXTRACT_FCT_VOID,			NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_VOID,			NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_VOID,			NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_HUD)},

// + NEXTRACTSET_SECTION_STANDARD
//{NRENDERABLE_EXTRACT_FCT_VOID,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,0)}, it could be usefull but not used yet ...
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},

{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMORDER)}, // GEOMORDER for single geometry is a nonsense so use the NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY function 
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMORDER)},// GEOMORDER for single geometry is a nonsense so use the NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING function

{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMZ)}, // GEOMZ for single geometry is a nonsense so use the NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY function 
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMZ)},// GEOMZ for single geometry is a nonsense so use the NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING function

{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},

{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMORDER,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMORDER)},
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMORDER,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMORDER)},

{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMZ,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMZ)},
{NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMZ,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING|FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMZ)},

{NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD,FLAG_NEXTRACTSET_HUD)},
{NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_STANDARD|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HUD)},
// + NEXTRACTSET_SECTION_UI
//{NRENDERABLE_EXTRACT_FCT_VOID,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,0)}, it could be usefull but not used yet ...
{NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,FLAG_NEXTRACTSET_HUD)},
{NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HUD)},
// + NEXTRACTSET_SECTION_UICHECKBOX
//{NRENDERABLE_EXTRACT_FCT_VOID,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX,0)}, it could be usefull but not used yet ...
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX,FLAG_NEXTRACTSET_HUD)},
{NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICHECKBOX|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HUD)},
// + NEXTRACTSET_SECTION_UICUSTOM
//{NRENDERABLE_EXTRACT_FCT_VOID,NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,0)}, it could be usefull but not used yet ...
// No FLASH
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_HUD)},
{NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,				NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HUD)},
// FLASH
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_3D)},
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING,	NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING)},
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_HUD)},
{NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES,					NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UICUSTOM|FLAG_NEXTRACTSET_MULTIGEOMETRIES,FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH|FLAG_NEXTRACTSET_HUD)},
// LAST ENTRY (degenerated)
{NRENDERABLE_EXTRACT_FCT_NULL,0}
};


NRENDERABLE_UPDATE_FCT		NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_FCT_ID_ENUM_SIZE];
NRENDERABLE_EXTRACT_FCT		NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE];

//NRENDERABLE_EXTRACT_FCT_DESC	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE];

void NRenderableUpdate_Void(NRENDERABLE *prenderable, void* powner, const NTIME *ptime){}
void NRenderableUpdate_User_Void(NRENDERABLE *prenderable, void* powner, const NTIME *ptime){}

Nu32 NRenderableExtract_Void(const NRENDERABLE *prenderable, const void *powner){return 0;}
Nu32 NRenderableExtract_User_Void(const NRENDERABLE *prenderable, const void *powner){return 0;}

void NInitializeRenderableUpdateFCT_LUT()
{
	// NRENDERABLE UPDATE FUNCTIONS
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_NULL]								= NULL;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_VOID]								= NRenderableUpdate_Void;
	
	for(Nu32 i = NRENDERABLE_UPDATE_USER0;i<=NRENDERABLE_UPDATE_USER31;i++)
	{
		NRenderableUpdateFCT_LUT[i]	= NRenderableUpdate_User_Void;
	}

	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_PARTICLESEMITTER]					= NRenderableUpdate_ParticlesEmitter;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_ADVANCED_HOMING_PARTICLESEMITTER]	= NRenderableUpdate_AdvancedHomingParticlesEmitter;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_ADVANCED_PARTICLESEMITTER]			= NRenderableUpdate_AdvancedParticlesEmitter;			
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_HOMING_PARTICLESEMITTER]			= NRenderableUpdate_HomingParticlesEmitter;

	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_PHYSIC_STRUCTURE]					= NRenderableUpdate_PhysicStructure;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_TEXT]								= NRenderableUpdate_Text;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_UI]									= NRenderableUpdate_UI;
	NRenderableUpdateFCT_LUT[NRENDERABLE_UPDATE_UI_WITHOUT_EXTRACTION]				= NRenderableUpdateWithoutExtract_UI;
}
void NInitializeRenderableExtractFCT_LUT()
{


	// NRENDERABLE EXTRACT FUNCTIONS
/*
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_NULL].Extract_FCT		= NULL;
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_NULL].ExtractSet		= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_NULL,0);

	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_VOID].Extract_FCT		= NRenderableExtract_Void;
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_VOID].ExtractSet		= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,0);

	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_3D_VOID].Extract_FCT	= NRenderableExtract_3D_Void;
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_3D_VOID].ExtractSet	= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_3D);

	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_3D_VOID_FRUSTUMCULLING].Extract_FCT		= NRenderableExtract_3D_Void_FrustumCulling;
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_3D_VOID_FRUSTUMCULLING].ExtractSet		= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_FRUSTUMCULLING);

	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_HUD_VOID].Extract_FCT		= NRenderableExtract_HUD_Void;
	NRenderableExtractFCTDesc_LUT[NRENDERABLE_EXTRACT_FCT_HUD_VOID].ExtractSet		= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_VOID,FLAG_NEXTRACTSET_HUD);

	for(Nu32 j=NRENDERABLE_EXTRACT_FCT_USER0;j<=NRENDERABLE_EXTRACT_FCT_USER31;j++)
	{
		NRenderableExtractFCTDesc_LUT[j].Extract_FCT	= NRenderableExtract_User_Void;
		NRenderableExtractFCTDesc_LUT[j].ExtractSet		= NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_USERVOID,0);
	}
*/
	// + NEXTRACT FUNCTIONs NULL & VOID
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_NULL]												= NULL;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_VOID]												= NRenderableExtract_Void;

	// + NEXTRACT FUNCTIONs USER
	for(Nu32 j=NRENDERABLE_EXTRACT_FCT_USER0;j<=NRENDERABLE_EXTRACT_FCT_USER31;j++)
	{
		NRenderableExtractFCT_LUT[j] = NRenderableExtract_User_Void;
	}

	// + NEXTRACT FUNCTIONs STANDARD
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY]					= NRenderableExtract_3D_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES]				= NRenderableExtract_3D_SingleInstance_MultiGeometries;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMORDER]		= NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomOrder;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_GEOMZ]			= NRenderableExtract_3D_SingleInstance_MultiGeometries_GeomZ;

	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING]				= NRenderableExtract_3D_SingleInstance_SingleGeometry_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING]				= NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMORDER]	= NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomOrder;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING_GEOMZ]		= NRenderableExtract_3D_SingleInstance_MultiGeometries_FrustumCulling_GeomZ;
	
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_SINGLEGEOMETRY]							= NRenderableExtract_HUD_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_HUD_SINGLEINSTANCE_MULTIGEOMETRIES]							= NRenderableExtract_HUD_SingleInstance_MultiGeometries;
	// + NEXTRACT FUNCTIONs UI
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY]					= NRenderableExtract_UI_3D_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES]					= NRenderableExtract_UI_3D_SingleInstance_MultiGeometries;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING]	= NRenderableExtract_UI_3D_SingleInstance_SingleGeometry_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING]	= NRenderableExtract_UI_3D_SingleInstance_MultiGeometries_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_SINGLEGEOMETRY]					= NRenderableExtract_UI_HUD_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UI_HUD_SINGLEINSTANCE_MULTIGEOMETRIES]				= NRenderableExtract_UI_HUD_SingleInstance_MultiGeometries;
	// + NEXTRACT FUNCTIONs UICHECKBOX
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY]			= NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES]			= NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING]	= NRenderableExtract_UICheckBox_3D_SingleInstance_SingleGeometry_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING]	= NRenderableExtract_UICheckBox_3D_SingleInstance_MultiGeometries_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_SINGLEGEOMETRY]			= NRenderableExtract_UICheckBox_HUD_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICHECKBOX_HUD_SINGLEINSTANCE_MULTIGEOMETRIES]		= NRenderableExtract_UICheckBox_HUD_SingleInstance_MultiGeometries;
	// + NEXTRACT FUNCTIONs UICUSTOM
	// No FLASH
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY]			= NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES]			= NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING]	= NRenderableExtract_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING]= NRenderableExtract_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY]				= NRenderableExtract_UICustom_HUD_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES]				= NRenderableExtract_UICustom_HUD_SingleInstance_MultiGeometries;
	// FLASH
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY]			= NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES]			= NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_SINGLEGEOMETRY_FRUSTUMCULLING] = NRenderableExtract_Flash_UICustom_3D_SingleInstance_SingleGeometry_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_3D_SINGLEINSTANCE_MULTIGEOMETRIES_FRUSTUMCULLING]= NRenderableExtract_Flash_UICustom_3D_SingleInstance_MultiGeometries_FrustumCulling;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_SINGLEGEOMETRY]				= NRenderableExtract_Flash_UICustom_HUD_SingleInstance_SingleGeometry;
	NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_FLASH_UICUSTOM_HUD_SINGLEINSTANCE_MULTIGEOMETRIES]			= NRenderableExtract_Flash_UICustom_HUD_SingleInstance_MultiGeometries;

	// There are 2 LUTs actually, 
	// CHECK 'NExtractSet_LUT' consistency in DEBUG MODE.
#ifdef _DEBUG
	// FIRST LUT:
	// Size is unknown (due to the direct by hand setup at the top of this file) but the last entry is the only one full of ZEROs.
	// Rule #0: ALL NEXTRACTSET_KEY.ExtractSet in the LUT are NON NULL except for the last entry which is degenerated ( to recognize the end of the LUT )
	// Rule #1: ALL NEXTRACTSET_KEY.ExtractFCTID in the LUT are NON NULL except for the last entry which is degenerated ( to recognize the end of the LUT )
	// Rule #2: ALL NEXTRACTSET_KEY.ExtractSet in the LUT are different !
	// Rule #3: several NEXTRACTSET_KEY.ExtractFCTID in the LUT may be identical, only  NEXTRACTSET_KEY.ExtractSet have to be all different !
	
	Nu32 dbg_i=0; Nu32 dbg_j=0;
	while( NExtractSet_LUT[dbg_i].ExtractSet && NExtractSet_LUT[dbg_i].Extract_FCTID) 
	{
		// Rule #0 check...
		NErrorIf(!NExtractSet_LUT[dbg_i].ExtractSet, NERROR_SYSTEM_GURU_MEDITATION);
		// Rule #1 check...
		NErrorIf(!NExtractSet_LUT[dbg_i].Extract_FCTID, NERROR_SYSTEM_GURU_MEDITATION);

		dbg_j=dbg_i+1;
		while( NExtractSet_LUT[dbg_j].ExtractSet ) 
		{
			// Rule #2 check...
			NErrorIf(NExtractSet_LUT[dbg_i].ExtractSet == NExtractSet_LUT[dbg_j].ExtractSet,NERROR_SYSTEM_GURU_MEDITATION);
			dbg_j++;
		}

		// Other checks about NEXTRACTSET FLAGS inconsistency :
		NErrorIf( (NExtractSet_LUT[dbg_i].ExtractSet&(FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HUD))==(FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HUD),NERROR_RENDERABLE_EXTRACTSET_INVALID );

		dbg_i++;
	}

	// SECOND LUT
	// Size is known its NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE.
	// Each entry of this ENUM corresponds with a n Extract Function.
	// Rule #0: ALL LUT entries are NON NULL the first one  excepted (NRENDERABLE_EXTRACT_FCT_NULL).
	// Rule #1: ALL LUT entries are different (Excepted for 'NRenderableExtract_User_Void').
	
	// Rule #0 check ( we check the first is NULL, so after, in the loops no other ones can be NULL too )
	NErrorIf(NRenderableExtractFCT_LUT[NRENDERABLE_EXTRACT_FCT_NULL]!=NULL,NERROR_SYSTEM_GURU_MEDITATION);
	for(dbg_i=0;dbg_i<NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE;dbg_i++)
	{
		for(dbg_j=dbg_i+1;dbg_j<NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE;dbg_j++)
		{
			// Rule #1 check
			if(NRenderableExtractFCT_LUT[dbg_i]==NRenderableExtract_User_Void)
			{
				NErrorIf(dbg_i<NRENDERABLE_EXTRACT_FCT_USER0 || dbg_i>NRENDERABLE_EXTRACT_FCT_USER31,NERROR_SYSTEM_GURU_MEDITATION);
			}
			else
			{
				NErrorIf(NRenderableExtractFCT_LUT[dbg_i] == NRenderableExtractFCT_LUT[dbg_j], NERROR_SYSTEM_GURU_MEDITATION);
			}
		}
	}
#endif
}



NRENDERABLE_UPDATE_FCT	NGetRenderableUpdateFunctionByID(NRENDERABLE_UPDATE_FCT_ID_ENUM id)
{
	// No need of special tests, there is always a function linked with a right ID.
	return NRenderableUpdateFCT_LUT[id];
}

NRENDERABLE_UPDATE_FCT_ID_ENUM	NGetRenderableUpdateFunctionID( NRENDERABLE_UPDATE_FCT pfct )
{
	// Notes:
	// All the RenderableUpdate functions should be unique and stored only once into the LUT.
	// !Excepted! the "NRenderable_Update_User_Void" function which can be stored several times by and for lowlevel engine use ONLY !
	// User is not allowed to use this function directly...
	// So if "pfct" == "NRenderable_Update_User_Void" it will make an ERROR in debug mode.
	// In release mode, the first user ID which stored has this function into will be returned
	// But it should NEVER happening !
	// In Addition to stay as safer as possible, "Debug Mode" check if the function is unique
	// for sure it should be the case, and its not the better place to do this. 
	// User Function Bind method is the most important place to perform this test.
	// ... but any way. Just in case of weird things.
#ifdef _DEBUG
	NErrorIf(pfct == NRenderableUpdate_User_Void, NERROR_RENDERABLE_UPDATE_FCT_INVALID);
	NRENDERABLE_UPDATE_FCT_ID_ENUM id = NRENDERABLE_UPDATE_NULL;
	Nu32 found = 0;
	for(Nu32 i=0;i<NRENDERABLE_UPDATE_FCT_ID_ENUM_SIZE;i++)
	{
		if(NRenderableUpdateFCT_LUT[i]==pfct)
		{
			id = (NRENDERABLE_UPDATE_FCT_ID_ENUM)i;
			found ++;
		}
	}
	if( found == 0 )
	{
		NErrorIf(1,NERROR_RENDERABLE_UPDATE_FCT_INVALID); 
	}
	else if( found > 1)
	{
		NErrorIf(1,NERROR_RENDERABLE_UPDATE_FCT_LUT_CORRUPTED); 
	}
	return id; 
#endif
#ifndef _DEBUG
	// In Release mode the first one stored will be pick up
	// ... And it's ID will be returned.
	for(Nu32 i=0;i<NRENDERABLE_UPDATE_FCT_ID_ENUM_SIZE;i++)
	{
		if(NRenderableUpdateFCT_LUT[i]==pfct)
			return (NRENDERABLE_UPDATE_FCT_ID_ENUM)i;
	}
	// Beyond this point ?... That means no match ! incoming function is not referenced ...
	return NRENDERABLE_UPDATE_NULL; // By default return 'NRENDERABLE_UPDATE_NULL'. Maybe it will be better to return 'NRENDERABLE_UPDATE_VOID' ??? 
#endif
}

void	NBindRenderableUpdateUserFunction( NRENDERABLE_UPDATE_FCT_ID_ENUM id, NRENDERABLE_UPDATE_FCT pfct )
{
	// Notes:
	// All the RenderableUpdate functions should be unique and stored only once into the LUT.
	// !Excepted! the "NRenderable_Update_User_Void" function which can be stored several times by and for low level engine use ONLY !
	// User is not allowed to use this function directly...
	// So if "pfct" == "NRenderable_Update_User_Void" it will make an ERROR in debug mode.
	// Same thing for "NRenderable_Update_Void" and NULL pointer.
	// There are only 32 available slots to reference user Renderable update function. If User try to set up a reserved one ... Crash !
	// In Addition to stay as safer as possible, "Debug Mode" check if the function will be unique after bind
	// It should be the case, but if not ... CRASH !
#ifdef _DEBUG
	NErrorIf(id<NRENDERABLE_UPDATE_USER0,NERROR_RENDERABLE_UPDATE_FCT_ID_INVALID);
	NErrorIf(id>NRENDERABLE_UPDATE_USER31,NERROR_RENDERABLE_UPDATE_FCT_ID_INVALID);

	NErrorIf(!pfct,NERROR_RENDERABLE_UPDATE_FCT_INVALID);								// User is not allowed to use this function directly !	
	NErrorIf(pfct==NRenderableUpdate_User_Void,NERROR_RENDERABLE_UPDATE_FCT_INVALID);	// User is not allowed to use this function directly !
	NErrorIf(pfct==NRenderableUpdate_Void,NERROR_RENDERABLE_UPDATE_FCT_INVALID);		// User is not allowed to use this function directly !
	
	for(Nu32 i=0;i<NRENDERABLE_UPDATE_FCT_ID_ENUM_SIZE;i++)
	{
		NErrorIf(i!=id && NRenderableUpdateFCT_LUT[i] == pfct, NERROR_RENDERABLE_UPDATE_FCT_ALREADY_EXISTS);
	}
#endif
	
	NRenderableUpdateFCT_LUT[id] = pfct;
}

NRENDERABLE_EXTRACT_FCT	NGetRenderableExtractFunctionByID( NRENDERABLE_EXTRACT_FCT_ID_ENUM id )
{
	// No need of special tests, there is always a function linked with a right ID.
	return NRenderableExtractFCT_LUT[id];
}
NRENDERABLE_EXTRACT_FCT_ID_ENUM	NGetRenderableExtractFunctionID( NRENDERABLE_EXTRACT_FCT pfct )
{
	// Notes:
	// All the RenderableExtract functions are unique and stored only once into the LUT.
	// !Excepted! the "NRenderable_Extract_User_Void" function which can be stored several times 
	// (with the same "ExtractSet" each time, of course).
	// And ... User is not allowed to use this function directly.
	// So if "pfct" == "NRenderable_Extract_User_Void" it will make an ERROR in debug mode.
	// In release mode, the first user ID which stored has this function into will be returned
	// But it should NEVER happening !
	// Understand that this "NRenderable_Extract_User_Void" is here to say ... UNUSED SLOT !

	// In Addition to stay as safer as possible, "Debug Mode" check if the function is unique
	// for sure it should be the case, and its not the better place to do this. 
	// User Function Bind method is the most important place to perform this test.
	// ... but any way. Just in case of weird things.
#ifdef _DEBUG
	NErrorIf(pfct == NRenderableExtract_User_Void, NERROR_RENDERABLE_EXTRACT_FCT_INVALID);
	
	NRENDERABLE_EXTRACT_FCT_ID_ENUM id = NRENDERABLE_EXTRACT_FCT_NULL;
	Nu32 found = 0;
	for(Nu32 i=0;i<NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE;i++)
	{
		// Note: pfct may be NULL !!! 
		if(NRenderableExtractFCT_LUT[i] == pfct)
		{
			id = (NRENDERABLE_EXTRACT_FCT_ID_ENUM)i;
			found ++;
		}
	}
	if( found == 0 )
	{
		NErrorIf(1,NERROR_RENDERABLE_UPDATE_FCT_INVALID); 
	}
	else if( found > 1)
	{
		NErrorIf(1,NERROR_RENDERABLE_UPDATE_FCT_LUT_CORRUPTED); 
	}
	return id; 
#endif
#ifndef _DEBUG
	// In Release mode the first one stored will be pick up
	// ... And it's ID will be returned.
	for(Nu32 i=0;i<NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE;i++)
	{
		if(NRenderableExtractFCT_LUT[i]==pfct)
			return (NRENDERABLE_EXTRACT_FCT_ID_ENUM)i;
	}
	return NRENDERABLE_EXTRACT_FCT_NULL; // By default return 'NRENDERABLE_EXTRACT_FCT_NULL'. Maybe it will be better to return 'NRENDERABLE_EXTRACT_VOID' ??? 
#endif
}

void	NBindRenderableExtractUserFunction( NRENDERABLE_EXTRACT_FCT_ID_ENUM id, NRENDERABLE_EXTRACT_FCT pfct )
{
	// Notes:
	// All the RenderableExtract functions should be unique and stored only once into the LUT.
	// There are only 32 available slots to reference user Renderable extract function.
	// If User try to set up a reserved one ... ERROR !
	// In Addition to stay as safer as possible, "Debug Mode" check if the function will be unique after bind
	// It should be the case, but if not ... ERROR !
#ifdef _DEBUG
	NErrorIf(id<NRENDERABLE_EXTRACT_FCT_USER0,NERROR_RENDERABLE_EXTRACT_FCT_ID_INVALID);
	NErrorIf(id>NRENDERABLE_EXTRACT_FCT_USER31,NERROR_RENDERABLE_EXTRACT_FCT_ID_INVALID);
	for(Nu32 i = 0; i<NRENDERABLE_EXTRACT_FCT_ID_ENUM_SIZE; i++)
	{
		NErrorIf((i!=id)&&(NRenderableExtractFCT_LUT[i] == pfct),NERROR_RENDERABLE_EXTRACT_FCT_ALREADY_EXISTS );
	}

#endif	
	NRenderableExtractFCT_LUT[id] = pfct;
}

NRENDERABLE_EXTRACT_FCT_ID_ENUM NGetRenderableExtractFunctionIDByExtractSet( const NEXTRACTSET extractset )
{
	// Notes:
	// All the RenderableExtract functions are unique and stored only once into the LUT.
	// !Excepted! the "NRenderable_Extract_User_Void" function which can be stored several times 
	// (with the same "ExtractSet" each time, of course).
	// And ... User is not allowed to use this function directly.
	// So if "NEXTRACTSET_ENGINE_LOWLEVEL(extractset)" == "FLAGS_NEXTRACTSET_USER_VOID" it will make an ERROR in debug mode.
	// User has to define the type of accumulator spreading (HUD or 3D) if none or both are defined  ... ERROR in debug mode.
	// The right way is to choose only one of these two flags.
	//
	// In release mode, the first user ID which stored  this extractset into will be returned
	// But it should NEVER happening !

	// In Addition to stay as safer as possible, "Debug Mode" check if the extractset is unique
	// for sure it should be the case, and its not the better place to do this. 
	// User Function Bind method is the most important place to perform this test.
	// ... but any way. Just in case of weird things.
#ifdef _DEBUG
	NErrorIf( !extractset, NERROR_RENDERABLE_EXTRACTSET_INVALID);

	NRENDERABLE_EXTRACT_FCT_ID_ENUM id = NRENDERABLE_EXTRACT_FCT_NULL;
	Nu32 found = 0;
	Nu32 i=0;
	while( NExtractSet_LUT[i].ExtractSet )
	{
		if( NExtractSet_LUT[i].ExtractSet == extractset )
		{
			if(found == 0)
				id = NExtractSet_LUT[i].Extract_FCTID;	

			found ++;
		}
		i++;
	}

	if( found == 0 ) // No matching !
	{
		NErrorIf(1,NERROR_RENDERABLE_EXTRACTSET_INVALID); 
	}
	else if( found > 1 ) // if we are here, that means SECTION != NEXTRACTSET_SECTION_USERVOID ... ( due to previous test ) so ...
	{
		NErrorIf(1,NERROR_RENDERABLE_EXTRACT_FCT_LUT_CORRUPTED); 
	}
	return id; 
#endif
#ifndef _DEBUG
	Nu32 i=0;
	while( NExtractSet_LUT[i].ExtractSet )
	{
		if(  NExtractSet_LUT[i].ExtractSet == extractset )
			return NExtractSet_LUT[i].Extract_FCTID;
		i++;
	}
	// We shouldn't pass here ... but just in case
	return NRENDERABLE_EXTRACT_FCT_NULL;
#endif
}

NRENDERABLE_EXTRACT_FCT NGetRenderableExtractFunctionByExtractSet( const NEXTRACTSET extractset )
{
	return NRenderableExtractFCT_LUT[NGetRenderableExtractFunctionIDByExtractSet(extractset)];
}
