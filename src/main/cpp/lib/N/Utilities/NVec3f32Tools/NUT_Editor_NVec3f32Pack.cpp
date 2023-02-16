#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../NCStandard.h"
#include "NUT_Editor_NVec3f32Pack_Tools.h"

NUT_V3PACK_TOOLDATA*	NUT_SetupV3PackToolData(NUT_V3PACK_TOOLDATA* pv3pack, NARRAY *parrayofelements, const Nu32 offsettov3)
{
	memset(pv3pack,0,sizeof(NUT_V3PACK_TOOLDATA));
	pv3pack->pArrayOfElements	= parrayofelements;
	pv3pack->OffsetToV3			= offsettov3;
	
	// Default Drawings setup:
	pv3pack->TickExtend.x = DEFAULT_NUT_V3PACK_TICK_XEXTEND;
	pv3pack->TickExtend.y = DEFAULT_NUT_V3PACK_TICK_YEXTEND;
	NSetColorf(&pv3pack->TickColor, DEFAULT_NUT_V3PACK_DRAW_COLOR, 1);
	NSetColorf(&pv3pack->SelectionColor, DEFAULT_NUT_V3PACK_SELECTION_COLOR, 1);

	return pv3pack;
}

NUT_V3PACK_TOOLDATA*	NUT_CreateV3PackToolData(NARRAY *parrayofelements, const Nu32 offsettov3)
{
	return NUT_SetupV3PackToolData(NEW(NUT_V3PACK_TOOLDATA), parrayofelements,offsettov3);
}

void NUT_ClearV3PackToolData(NUT_V3PACK_TOOLDATA *pv3pack)
{
	Nmem0(pv3pack,NUT_V3PACK_TOOLDATA);
}

void NUT_DeleteV3PackToolData(NUT_V3PACK_TOOLDATA *pv3pack)
{
	NUT_ClearV3PackToolData(pv3pack);
	Nfree(pv3pack);
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR