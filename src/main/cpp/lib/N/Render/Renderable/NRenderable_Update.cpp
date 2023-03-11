#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "NRenderable.h"
#include "NRenderable_Update.h"

// ------------------------------------------------------------------------------------------
// void NRenderableUpdate_Basic
// ------------------------------------------------------------------------------------------
// Description :
//	The Basic Update which only performs Extraction, and just that.
//
//	Actually purpose of Renderable Update function is 1)Update data 2)Extract
//	This is the function to use if there is not data to update.
//
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out : nothing
// ------------------------------------------------------------------------------------------
void NRenderableUpdate_Basic(NRENDERABLE *prenderable, void* powner, const NTIME *ptime)
{
	prenderable->Extract_FCT( prenderable, powner );
}
