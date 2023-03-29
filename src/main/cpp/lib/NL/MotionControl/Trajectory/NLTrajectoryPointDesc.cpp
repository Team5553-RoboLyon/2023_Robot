#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPointDesc.h"


#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#endif


#ifdef _NEDITOR
void NLTRJPOINT_DESC::draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack, const NLTRJPOINT_DESC* p0)
{
	NVEC2 o;

	if (!p0)
	{
		NLKIN k0;
		if (ISFLAG_ON(m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING) || ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT) )
			m_kin.draw(p2docs, pickpack, &k0);
		else
		{
			o.x = m_pPathPoint1->s;
			o.y = 0.0f;
			m_kin.verticalDraw(p2docs, &o, pickpack, &k0);
		}
	}
	else
	{
		if (ISFLAG_ON(m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING) || ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
			m_kin.draw(p2docs, pickpack, &p0->m_kin);
		else
		{
			o.x = m_pPathPoint1->s;
			o.y = 0.0f;
			m_kin.verticalDraw(p2docs, &o, pickpack, &p0->m_kin);
		}
	}
}
#endif
