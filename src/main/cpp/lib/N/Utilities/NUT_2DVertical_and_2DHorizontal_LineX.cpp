#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_X.h"

Nbool NUT_2DVerticalLineXBezierFastAABB(const NVEC2 *plineA, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb)
{
	if (plineA->x < NMIN(spb->TA.x, NMIN(spb->Position.x, NMIN(spa->Position.x, spa->TB.x))))
		return NFALSE;

	if (plineA->x > NMAX(spb->TA.x, NMAX(spb->Position.x, NMAX(spa->Position.x, spa->TB.x))))
		return NFALSE;

	return NTRUE;
}

Nbool NUT_2DHorizontalLineXBezierFastAABB(const NVEC2 *plineA, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb)
{
	if (plineA->y < NMIN(spb->TA.y, NMIN(spb->Position.y, NMIN(spa->Position.y, spa->TB.y))))
		return NFALSE;

	if (plineA->y > NMAX(spb->TA.y, NMAX(spb->Position.y, NMAX(spa->Position.y, spa->TB.y))))
		return NFALSE;

	return NTRUE;
}