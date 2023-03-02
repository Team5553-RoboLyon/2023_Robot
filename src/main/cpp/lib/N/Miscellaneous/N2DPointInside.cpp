// ==========================================================================================
// N2DPointInside.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 06/08/2014
// Updated : 
// ------------------------------------------------------------------------------------------
// 
// ==========================================================================================

#include "N2DPointInside.h"


static inline Nf32 ComputeWhatSide(const NVEC2 *pA, const NVEC2 *pB, const NVEC2 *p)
{
	//x1 (y2 - y3) + x2 (y3 - y1) + x3 (y1 - y2)

	return pA->x * (pB->y - p->y) + pB->x * (p->y - pA->y) + p->x * (pA->y - pB->y);
}

// ------------------------------------------------------------------------------------------
// N2DPointInside2DTriangle
// ------------------------------------------------------------------------------------------
// Description :
//	check if a point is INSIDE a triangle or Not. (A,B,C are described in CCW order )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nbool N2DPointInside2DTriangle(const NVEC2 *pA,const NVEC2 *pB,const NVEC2 *pC,const NVEC2 *p)
{
	Nf32 ws1 = ComputeWhatSide(pA, pB, p);
	Nf32 ws2 = ComputeWhatSide(pB, pC, p);
	Nf32 ws3 = ComputeWhatSide(pC, pA, p);

	return (ws1 > 0 && ws2 > 0 && ws3 > 0) || (ws1 < 0 && ws2 < 0 && ws3 < 0);
}

// ------------------------------------------------------------------------------------------
// N2DPointInside2DRectangle
// ------------------------------------------------------------------------------------------
// Description :
//	check if a point is INSIDE a Rectangle ( rectangle limits included)
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nbool N2DPointInside2DRectangle(const NRECTf32 *prect, const NVEC2 *p)
{
	if(p->x > prect->right )
		return NFALSE;
	if(p->x < prect->left )
		return NFALSE;
	if(p->y > prect->top)
		return NFALSE;
	if(p->y < prect->bottom)
		return NFALSE;

	return NTRUE;
}

Nbool N2DPointInside2DRectangleEx(const NVEC2 *pcenter, const NVEC2 *pextent, const NVEC2 *p)
{
	NVEC2 v;

	v.x = p->x - pcenter->x;
	v.y = p->y - pcenter->y;

	if(v.x > pextent->x )
		return NFALSE;
	if(v.x < -pextent->x )
		return NFALSE;
	if(v.y > pextent->x)
		return NFALSE;
	if(v.y < -pextent->y)
		return NFALSE;

	return NTRUE;
}

