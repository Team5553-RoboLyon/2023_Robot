#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"
#include "lib/NL/MotionControl/MotionShapes/NLFMotionShape.h"

Nu32 NLFMOTIONSHAPE::promote(NLKIN *pkbuff, const Nf32 v)
{
	NErrorIf(!m_dt || !m_ds, NERROR_INCONSISTENT_VALUES);
	NErrorIf(NLKIN_V_DISTINCT(v, m_ds / m_dt), NERROR_INCONSISTENT_VALUES);

	NLKIN *pk = pkbuff + 1;
	// Promote point/kin de fin de phase 1/1
	pk->m_j = 0.0f;
	pk->m_a = 0.0f;
	pk->m_v = v;
	pk->m_s = m_s0 + m_ds;
	pk->m_t = pkbuff->m_t + m_dt;
	return 1;
}

Nu32 NLFMOTIONSHAPE::promote(NLKIN *pkbuff)
{
	NErrorIf(!m_dt || !m_ds, NERROR_INCONSISTENT_VALUES);

	NLKIN *pk = pkbuff + 1;
	// Promote point/kin de fin de phase 1/1
	pk->m_j = 0.0f;
	pk->m_a = 0.0f;
	pk->m_v = m_ds / m_dt;
	pk->m_s = m_s0 + m_ds;
	pk->m_t = pkbuff->m_t + m_dt;
	return 1;
}

#ifdef _NEDITOR
// SMOTION PROFILE NCOLORPICKPACK index ventilation (4 bits / index):
// NCOLORPICKPACK.#0....... S Motion Shape color phase 1			NCOLORPICKPACK.#3....... S Motion Shape color 'Out'
// NCOLORPICKPACK.#1....... S Motion Shape color phase 2			NCOLORPICKPACK.#4....... Flat Motion Shape color
// NCOLORPICKPACK.#2....... S Motion Shape color phase 3			NCOLORPICKPACK.#5....... F et S Motion Shape Text color
void NLFMOTIONSHAPE::drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NErrorIf(!m_dt || !m_ds, NERROR_INCONSISTENT_VALUES);

	NUTDRAWVERTEX va;
	// NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_BLUE_OXFORD, 1.0f);
	va.Color0_4f = *p2docs->getColor(4, pickpack);
	va.Position_3f.z = 0.0f;
	va.Position_3f.x = p2docs->transformX(m_s0);
	va.Position_3f.y = p2docs->transformY(m_ds / m_dt);
	NUT_DrawPencil_From(&va);
	va.Position_3f.x = p2docs->transformX(m_s0 + m_ds);
	NUT_DrawPencil_LineTo(&va);
}
void NLFMOTIONSHAPE::drawVs(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack, const Nf32 v)
{
	NErrorIf(!m_dt || !m_ds, NERROR_INCONSISTENT_VALUES);
	NUTDRAWVERTEX va;
	// NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_BLUE_OXFORD, 1.0f);
	va.Color0_4f = *p2docs->getColor(4, pickpack);
	va.Position_3f.z = 0.0f;
	va.Position_3f.x = p2docs->transformX(m_s0);
	va.Position_3f.y = p2docs->transformY(v);
	NUT_DrawPencil_From(&va);
	va.Position_3f.x = p2docs->transformX(m_s0 + m_ds);
	NUT_DrawPencil_LineTo(&va);
}
#endif
