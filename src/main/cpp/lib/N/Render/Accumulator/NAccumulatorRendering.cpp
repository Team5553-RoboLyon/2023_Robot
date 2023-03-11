#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/NTransformationTree.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NViewport.h"
#include "../../NErrorHandling.h"
#include "lib/N/NRenderCell.h"
#include "NAccumulator.h"
#include "NAccumulatorQuickSorting.h"
#include "NAccumulatorRendering.h"

void NRenderAccumulator_NoBlendNoLight(NACCUMULATOR *pacc)
{
	NRENDERCELL	*prcell;
	NRENDERCELL	**pptr =(NRENDERCELL**)pacc->AccumulatedRenderCellsArray.pFirst;
	
	// Sorting Accumulator
	NQuickSortAccumulatorFTB(pptr,0,pacc->AccumulatedRenderCellsArray.Size-1);
	
	// Applying Accumulator StateSets
	NUpdateCurrentStateSet(pacc->StateSet);
	
	// Rendering Accumulator
	glMatrixMode(GL_MODELVIEW);
	for( Nu32 j=pacc->AccumulatedRenderCellsArray.Size;j!=0;j--, pptr++)
	{
		prcell = (NRENDERCELL*)(*pptr);
		
		// RENDERING GEOMETRY WITH or WITHOUT MATRIX
		if(prcell->pTransformHNode)
		{
			glPushMatrix();
			glMultMatrixf((GLfloat*)&prcell->pTransformHNode->WorldMatrix);
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
			glPopMatrix();
		}
		else
		{
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
		}
	}
}

void NRenderAccumulator_BlendNoLight(NACCUMULATOR *pacc)
{
	NRENDERCELL	*prcell;
	NRENDERCELL	**pptr =(NRENDERCELL**)pacc->AccumulatedRenderCellsArray.pFirst;

	// Sorting Accumulator
	NQuickSortAccumulatorBTF(pptr,0,pacc->AccumulatedRenderCellsArray.Size-1);
	
	// Applying Accumulator StateSets
	NUpdateCurrentStateSet(pacc->StateSet);
	NErrorIf(ISFLAG_OFF(NCurrentStateSet,FLAG_STATESET_BLEND),NERROR_INCONSISTENT_PARAMETERS);
	// Rendering Accumulator
	glMatrixMode(GL_MODELVIEW);
	for( Nu32 j=pacc->AccumulatedRenderCellsArray.Size;j!=0;j--, pptr++)
	{
		prcell = (NRENDERCELL*)(*pptr);
		// Blend
		// glBlendEquation is not available on opengles where the default 'GL_FUNC_ADD' equation is always used !
		#ifdef _NWINDOWS
		glBlendEquation(prcell->pGeometry->Appearance.TT.pBlend->Blend0_Equation);
		#endif
		glBlendFunc( prcell->pGeometry->Appearance.TT.pBlend->Blend0_Sfactor, prcell->pGeometry->Appearance.TT.pBlend->Blend0_Dfactor );
		
		// RENDERING GEOMETRY WITH or WITHOUT MATRIX
		if(prcell->pTransformHNode)
		{
			glPushMatrix();
			glMultMatrixf((GLfloat*)&prcell->pTransformHNode->WorldMatrix);
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
			glPopMatrix();
		}
		else
		{
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
		}
	}
}
void NRenderAccumulator_NoBlendLight(NACCUMULATOR *pacc)
{
	NRENDERCELL	*prcell;
	NRENDERCELL	**pptr =(NRENDERCELL**)pacc->AccumulatedRenderCellsArray.pFirst;

	// Sorting Accumulator
	NQuickSortAccumulatorFTB(pptr,0,pacc->AccumulatedRenderCellsArray.Size-1);

	// Applying Accumulator StateSets
	NUpdateCurrentStateSet(pacc->StateSet);
	NErrorIf(ISFLAG_OFF(NCurrentStateSet,FLAG_STATESET_LIGHT),NERROR_INCONSISTENT_PARAMETERS);
	// Rendering Accumulator
	glMatrixMode(GL_MODELVIEW);
	for( Nu32 j=pacc->AccumulatedRenderCellsArray.Size;j!=0;j--, pptr++)
	{
		prcell = (NRENDERCELL*)(*pptr);
		// Light material
		// TODO ....
		// RENDERING GEOMETRY WITH or WITHOUT MATRIX
		if(prcell->pTransformHNode)
		{
			glPushMatrix();
			glMultMatrixf((GLfloat*)&prcell->pTransformHNode->WorldMatrix);
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
			glPopMatrix();
		}
		else
		{
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
		}
	}
}
void NRenderAccumulator_BlendLight(NACCUMULATOR *pacc)
{
	NRENDERCELL	*prcell;
	NRENDERCELL	**pptr =(NRENDERCELL**)pacc->AccumulatedRenderCellsArray.pFirst;

	// Sorting Accumulator
	NQuickSortAccumulatorBTF(pptr,0,pacc->AccumulatedRenderCellsArray.Size-1);

	// Applying Accumulator StateSets
	NUpdateCurrentStateSet(pacc->StateSet);
	NErrorIf(ISFLAG_OFF(NCurrentStateSet,FLAG_STATESET_BLEND),NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(ISFLAG_OFF(NCurrentStateSet,FLAG_STATESET_LIGHT),NERROR_INCONSISTENT_PARAMETERS);
	// Rendering Accumulator
	glMatrixMode(GL_MODELVIEW);
	for( Nu32 j=pacc->AccumulatedRenderCellsArray.Size;j!=0;j--, pptr++)
	{
		prcell = (NRENDERCELL*)(*pptr);
		// Blend
		// glBlendEquation is not available on opengles where the default 'GL_FUNC_ADD' equation is always used !
		#ifdef _NWINDOWS
		glBlendEquation(prcell->pGeometry->Appearance.TT.pBlend->Blend0_Equation);
		#endif
		glBlendFunc( prcell->pGeometry->Appearance.TT.pBlend->Blend0_Sfactor, prcell->pGeometry->Appearance.TT.pBlend->Blend0_Dfactor );
		// Light material
		// TODO ....
		// RENDERING GEOMETRY WITH or WITHOUT MATRIX
		if(prcell->pTransformHNode)
		{
			glPushMatrix();
			glMultMatrixf((GLfloat*)&prcell->pTransformHNode->WorldMatrix);
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
			glPopMatrix();
		}
		else
		{
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
		}
	}
}
// =====================================================================
// Rendering HUD
//		Just one Accumulator with all the 'HUD'  geometries inside !
//		Geometries are NOT sorted as usual ( No Sorting key Needs ... )
//		Geometries still have a specific Accumulator ID store into (which is not HUD ID !).
//		But HERE, in the special HUD Case, we are going to call 'NUpdateCurrentStateSet' function
//		before rendering each geometry !
//		Obviously, all the UI UI extract and push their geometries into this specific accumulator
//		... Directly in the RIGHT RENDERING ORDER which is a HIERARCHICAL ORDER
// =====================================================================
void NRenderAccumulator_HUD(NACCUMULATOR *pacc)
{
	NRENDERCELL	*prcell;
	NRENDERCELL	**pptr =(NRENDERCELL**)pacc->AccumulatedRenderCellsArray.pFirst;
	Nu32		stateset;

	// Matrix Context Setting Up
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// glOrthographicMatrix
	// ---------------------
	glOrtho(0.0f,NGetViewPort_AspectRatio(),-1.0f,0.0f,-1.0f,1.0f);
	//NOrthoMatrixf(0.0f,NGetViewPort_AspectRatio(),-1.0f,0.0f,-1.0f,1.0f);

	// TODO: replace calling glOrtho by using glMulMatrixf(&NOrthographicMatrix)
	// Rendering Accumulator
	
	glMatrixMode(GL_MODELVIEW);
	for( Nu32 j=pacc->AccumulatedRenderCellsArray.Size;j!=0;j--, pptr++)
	{
		prcell = (NRENDERCELL*)(*pptr);

		// Applying StateSets by geometry !
		// Zbuffer unactivation forced ... (by using MASK_STATESET_APPEARANCE_AND_RESERVED we 'mask' statesets Z buffer flags )
		stateset = NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(prcell->pGeometry->Core.IDS)].StateSet;
		NUpdateCurrentStateSet( stateset&MASK_STATESET_APPEARANCE_AND_RESERVED);

		// Blend
		if(ISFLAG_ON(stateset,FLAG_STATESET_BLEND))
		{
			// glBlendEquation is not available on opengles where the default 'GL_FUNC_ADD' equation is always used !
			#ifdef _NWINDOWS
			glBlendEquation(prcell->pGeometry->Appearance.TT.pBlend->Blend0_Equation);
			#endif
			glBlendFunc( prcell->pGeometry->Appearance.TT.pBlend->Blend0_Sfactor, prcell->pGeometry->Appearance.TT.pBlend->Blend0_Dfactor );
		}
		// Light material
		if(ISFLAG_ON(stateset,FLAG_STATESET_LIGHT))
		{
		// TODO ....
		}
		// RENDERING GEOMETRY WITH or WITHOUT MATRIX
		if(prcell->pTransformHNode)
		{
			glPushMatrix();
			glMultMatrixf((GLfloat*)&prcell->pTransformHNode->WorldMatrix);
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
			glPopMatrix();
		}
		else
		{
			prcell->pGeometry->Core.RenderFunction(prcell->pGeometry);
		}
	}

	// Restore Previous Matrix Context 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}
