#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NL2DOrthogonalCoordinateSystemToolDatah							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "../N/NType.h"
#include "../N/Event/NEvent.h"
#include "../N/Core/NTime.h"
#include "../N/NEditor.h"
#include "../N/Maths/NVec2f32.h"
#include "../N/Utilities/NUT_GizmoRectf32.h"
#include "NL2DOrthogonalCoordinateSystem.h"

#ifdef _NEDITOR
// -----------------------------------------------------------------------
#define DEFAULT_NL2DOCS_TOOLDATA_ARRAY_OF_OCS_PTR_CAPACITY	4		// capacité de l'array de pointeur sur OCS à la création.

typedef struct NL2DOCS_TOOLDATA	NL2DOCS_TOOLDATA;
struct NL2DOCS_TOOLDATA
{
	NL2DOCS_TOOLDATA(const NVEC2f32 *phandle_extend);
	~NL2DOCS_TOOLDATA();

	inline Nu32			bindOcs(const NL2DOCS *pocs) { NArrayPushBack(&m_ArrayOfOcsPtr, (NBYTE*)&pocs); return m_ArrayOfOcsPtr.Size - 1; }
	inline void			unbindOcs(const NL2DOCS *pocs)
	{
		NL2DOCS **ppocs = (NL2DOCS **)m_ArrayOfOcsPtr.pFirst;
		for (Nu32 i = 0; i < m_ArrayOfOcsPtr.Size;ppocs++)
		{
			if (*ppocs == pocs)
			{
				NQuickEraseArrayElementPtr(&m_ArrayOfOcsPtr, (NBYTE*)ppocs, NULL);
				return;
			}
		}
		NErrorIf(1, NERROR_SYSTEM_CHECK); // si on passe ici , on a pas trouvé l'OCS à sortir de la liste....pas grave mais bon, si il n'est pas dans la liste pourquoi vouloir l'en sortir ?
	}
	inline void			unbindAllOcs() { NResizeArray(&m_ArrayOfOcsPtr, 0, NULL, NULL); }

//	Nu32				m_Flags;
	NUT_GIZMO_RECTf32	m_Gizmo;
	NARRAY				m_ArrayOfOcsPtr;				// ToolData manage an array of pointers on OCS
};

#endif	// _NEDITOR
