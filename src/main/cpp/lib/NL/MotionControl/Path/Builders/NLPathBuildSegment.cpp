#include "lib/N/NFlags.h"
#include "lib/N/Core/NSafeConversion.h"
#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/NErrorHandling.h"

#include "LIB/NL/MotionControl/Path/Builders/NLPathBuildSegment.h"

Nu32 NL_ExtractSegmentPrimitives(const NVEC2f32 *pA, const NVEC2f32 *pB, const Nu8 primflags, NLPATH_GEOMETRY *pgeom)
{
	NVEC2f32 v;
	Nf32 lv;
	v.x = pB->x - pA->x;
	v.y = pB->y - pA->y;
	lv = NVec2Length(&v);
	NErrorIf(lv <= 0.0f, NERROR_NULL_VECTOR);
	// extraction primitive
	NLPATH_PRIMITIVE *pprim = (NLPATH_PRIMITIVE *)NArrayAllocBack0(&pgeom->m_primitivesArray);
	FLAG_ON(pprim->m_core.m_flags, primflags);
	pprim->m_segment.m_core.m_id = NLPATH_PRIMITIVE_ID_SEGMENT;
	pprim->m_segment.m_core.m_l = lv;

	// extraction keypoint
	pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint pr�c�dent ( d�j� ins�r� !)
	NLPATH_POINT *pkey = (NLPATH_POINT *)NArrayAllocBack0(&pgeom->m_pathPointsArray);
	pkey->s = (pkey - 1)->s + lv;
	pkey->p = *pB;
	pkey->u.x = v.x / lv;
	pkey->u.y = v.y / lv;
	pkey->k = 0.0f;

	return 1;
}
