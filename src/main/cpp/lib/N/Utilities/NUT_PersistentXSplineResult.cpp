#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "lib/N/NErrorHandling.h"
#include "NUT_X.h"

// ------------------------------------------------------------------------------------------
// NUT_Setup2DSplinePersistentXResult
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_2DSPLINE_PERSISTENT_XRESULT *NUT_Setup2DSplinePersistentXResult(NUT_2DSPLINE_PERSISTENT_XRESULT *pprs, const NSPLINE *pspline)
{
	NErrorIf(!pspline, NERROR_NULL_POINTER);
	NErrorIf(NGetSplineKnotsNumber(pspline) < 2, NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);

	// ---------------------------------------------------------------------------------
	// Fix data (not suppose to change )
	pprs->pSpline = (NSPLINE *)pspline;

	// ---------------------------------------------------------------------------------
	// Updated data (suppose to be updated by the X functions that will store their intermediate result here.
	// So the values set up here are the initial ones.
	//	--> the first subspline ( with [firstsplineknot,firstsplineknot+1] )
	//	--> All the param to define the segment of the spline are set to define the first segment of the first subspline.
	//  --> The Result is set to the first Spline Knot Position. ( point A of segment AB )
	//
	// pprs->pSplineKnot0			= (NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);
	pprs->pSplineKnot1 = (NSPLINEKNOT *)pspline->KnotArray.pFirst + 1;
	pprs->SegB_BezierParamCoord = 1.0f / (Nf32)pspline->BezierAccuracy;
	pprs->SegA_BezierParamCoord = 1.0f - pprs->SegB_BezierParamCoord;
	pprs->SegA_Position = ((NSPLINEKNOT *)pspline->KnotArray.pFirst)->Position;
	pprs->SegB_Position = pprs->pSplineKnot1->Position;

	pprs->XResult.I.x = ((NSPLINEKNOT *)pspline->KnotArray.pFirst)->Position.x;
	pprs->XResult.I.y = ((NSPLINEKNOT *)pspline->KnotArray.pFirst)->Position.y;
	pprs->XResult.ParamCoordAB = 0.0f;

	return pprs;
}

// ------------------------------------------------------------------------------------------
// NUT_Create2DSplinePersistentXResult
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_2DSPLINE_PERSISTENT_XRESULT *NUT_Create2DSplinePersistentXResult(const NSPLINE *pspline)
{
	return NUT_Setup2DSplinePersistentXResult(NEW(NUT_2DSPLINE_PERSISTENT_XRESULT), pspline);
}

// ------------------------------------------------------------------------------------------
// NUT_Clear2DSplinePersistentXResult
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Clear2DSplinePersistentXResult(void *pprs)
{
	memset(pprs, 0, sizeof(NUT_2DSPLINE_PERSISTENT_XRESULT));
}

// ------------------------------------------------------------------------------------------
// NUT_Delete2DSplinePersistentXResult
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Delete2DSplinePersistentXResult(void *pprs)
{
	NUT_Clear2DSplinePersistentXResult(pprs);
	free(pprs);
}
