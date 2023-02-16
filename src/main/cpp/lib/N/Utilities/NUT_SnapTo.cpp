#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../Containers/NArray.h"
#include "../NViewport.h"

#include "NUT_Snap.h"

// ------------------------------------------------------------------------------------------
// NUT_GetSnapValuef
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
Nf32 NUT_GetSnapValuef(const NUT_SNAP*psnap, const Nf32 a)
{
	Nf32	b,c,d;
	Nf32	out;

	if(!psnap->Grid)
		return (Nf32)a;

	b = a/psnap->Grid;
	c = (Nf32)floor(b);
	b-= c;
	b-= 0.5f; // from the exact middle point between previous and next vertical snap line.

	// snap threshold computing
	d= 0.5f- psnap->Distance/psnap->Grid;

	// snap X:
	if(b>d)
		out = (c+1.0f)*psnap->Grid;		
	else if(b<-d)
		out = c*psnap->Grid;		
	else
		out = (Nf32)a;

	return out;
}
