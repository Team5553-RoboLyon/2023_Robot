#ifdef _NEDITOR

#ifndef __NUT_EDITOR_NVEC3f32PACKTOOLS_H
#define __NUT_EDITOR_NVEC3f32PACKTOOLS_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NUT_Editor_NVec3f32Pack.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../../N/Event/NEvent.h"
#include "../../../N/Core/NTime.h"
#include "../../../N/UI/NUI.h"
#include "../../../N/NEditor.h"
#include "../../../N/NUsualCoordinates.h"
#include "../Draw/NUT_Draw.h"
//#include "../../Snks_GameData.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NUT_NV3PACK_DETECTION_DISTANCE							3.0f	//Used to test if cursor is close to a point.
#define NUT_NV3PACK_DETECTION_DISTANCE_SQUARED					(NUT_NV3PACK_DETECTION_DISTANCE*NUT_NV3PACK_DETECTION_DISTANCE)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef void(*NUT_V3PACK_SETPOSITION_CALLBACK)(const NARRAY *parrayofelement, const Nu32 ownerindex_inparrayofelements, NVEC3f32 *pv3, const NVEC3f32 *pnewposition );
typedef void(*NUT_V3PACK_INSERT_CALLBACK)(NARRAY *parrayofelement, Nu32 *pcreatedelementindex);
typedef void(*NUT_V3PACK_ERASE_CALLBACK)(NARRAY *parrayofelement, const Nu32 ownerindex_inparrayofelementsptr);

#define DEFAULT_NUT_V3PACK_TICK_XEXTEND			0.75f
#define DEFAULT_NUT_V3PACK_TICK_YEXTEND			0.75f
#define DEFAULT_NUT_V3PACK_DRAW_COLOR			NCOLOR_PRESET3F_GREEN_JADE
#define DEFAULT_NUT_V3PACK_SELECTION_COLOR		NCOLOR_PRESET3F_WHITE

typedef struct  
{
	Nu32							FlagsV3Pack;
	NARRAY							*pArrayOfElements;
	Nu32							OffsetToV3;

	NCOLOR							SelectionColor;
	NCOLOR							TickColor;
	NVEC2f32						TickExtend;

	// CallBacks
	NUT_V3PACK_INSERT_CALLBACK		Insert_CallBack;
	NUT_V3PACK_ERASE_CALLBACK		Erase_CallBack;
	NUT_V3PACK_SETPOSITION_CALLBACK	SetPosition_CallBack;

}NUT_V3PACK_TOOLDATA;


// ToolData
// -------------------------------------
NUT_V3PACK_TOOLDATA*	NUT_SetupV3PackToolData(NUT_V3PACK_TOOLDATA* pv3pack, NARRAY *parrayofelements, const Nu32 offsettov3);
NUT_V3PACK_TOOLDATA*	NUT_CreateV3PackToolData(NARRAY *parrayofelements, const Nu32 offsettov3);
void					NUT_ClearV3PackToolData(NUT_V3PACK_TOOLDATA *pv3pack);
void					NUT_DeleteV3PackToolData(NUT_V3PACK_TOOLDATA *pv3pack);

inline void				NUT_SetV3PackToolData_InsertCallBack(NUT_V3PACK_TOOLDATA *pv3pack, const NUT_V3PACK_INSERT_CALLBACK pinsert ){ pv3pack->Insert_CallBack = pinsert; }
inline void				NUT_SetV3PackToolData_EraseCallBack(NUT_V3PACK_TOOLDATA *pv3pack, const NUT_V3PACK_ERASE_CALLBACK perase) { pv3pack->Erase_CallBack = perase; }
inline void				NUT_SetV3PackToolData_SetPositionCallBack(NUT_V3PACK_TOOLDATA *pv3pack, const NUT_V3PACK_SETPOSITION_CALLBACK psetpos) { pv3pack->SetPosition_CallBack = psetpos; }

//TOOLS
// -----------------------------------------------------------------------------------------------------------------------------
NEDITOR_RESULT			NUT_NVec3f32PackTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT			NUT_NVec3f32PackTool_Erase(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT			NUT_NVec3f32PackTool_Move(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param);


void					NUT_NVec3f32PackTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_EDITOR_SPLINESPACKTOOLS_H 

#endif //_NEDITOR

  