#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "../Geometry/NLPathGeometry.h"

Nu32 NL_ExtractSegmentPrimitives(const NVEC2f32 *pA, const NVEC2f32 *pB, const Nu8 primflags, NLPATH_GEOMETRY *pgeom);