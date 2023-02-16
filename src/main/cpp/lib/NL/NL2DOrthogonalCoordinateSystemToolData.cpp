
#include "../N/UI/NUI.h"
#include "../N/Event/NEventTouch.h"
#include "NL2DOrthogonalCoordinateSystemToolData.h"

NL2DOCS_TOOLDATA::NL2DOCS_TOOLDATA(const NVEC2f32 *phandle_extend)
{
	Nmem0(this, NL2DOCS_TOOLDATA);
	NUT_SetupGizmoRectf32(&m_Gizmo, phandle_extend);
	NSetupArray(&m_ArrayOfOcsPtr, DEFAULT_NL2DOCS_TOOLDATA_ARRAY_OF_OCS_PTR_CAPACITY, sizeof(NL2DOCS*));
}

NL2DOCS_TOOLDATA::~NL2DOCS_TOOLDATA()
{
	NClearArray(&m_ArrayOfOcsPtr, NULL);
	Nmem0(this, NL2DOCS_TOOLDATA);
}

/*
// OLD WAY ( Nzt 'c style' )
NL2DOCS_TOOLDATA*	NL_SetupOrthogonalCoordinateSystemToolData(NL2DOCS_TOOLDATA* ptooldata, const NVEC2f32 *phandle_extend)
{
	Nmem0(ptooldata, NL2DOCS_TOOLDATA);
	NUT_SetupGizmoRectf32(&ptooldata->Gizmo, phandle_extend);
	NSetupArray(&ptooldata->m_ArrayOfOcsPtr, DEFAULT_NL2DOCS_TOOLDATA_ARRAY_OF_OCS_PTR_CAPACITY, sizeof(NL2DOCS*));
	return ptooldata;
}
NL2DOCS_TOOLDATA*	NL_CreateOrthogonalCoordinateSystemToolData(const NVEC2f32 *phandle_extend)
{
	return NL_SetupOrthogonalCoordinateSystemToolData(NEW(NL2DOCS_TOOLDATA),phandle_extend);
}
void NL_ClearOrthogonalCoordinateSystemToolData(NL2DOCS_TOOLDATA *ptooldata)
{
	NClearArray(&ptooldata->m_ArrayOfOcsPtr, NULL);
	Nmem0(ptooldata, NL2DOCS_TOOLDATA);
}
void NL_DeleteOrthogonalCoordinateSystemToolData(NL2DOCS_TOOLDATA *ptooldata)
{
	NL_ClearOrthogonalCoordinateSystemToolData(ptooldata);
	Nfree(ptooldata);
}
*/
