#ifndef __NRENDERCELL_H
#define __NRENDERCELL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NRenderCell.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../NTransformationTree.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/NErrorHandling.h"
#include "./Renderable/NRenderable.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + IMPORTANT NOTES:
// + Render Engine needs a static buffer to store NRENDERCELLs. Each turn, Engine fill up
// + this buffer with all rendering valid NRENDERCELLs.
// + The buffer capacity has to be sufficient to store the biggest amount of RENDERCELLs
// + in a UI. In case of overloading, the buffer is going to be reallocated and all
// + the accumulator RenderCell pointers re-indexed !!!! (which will cause a UI lag ...
// + ... for sure ! ).
// + SO DON'T HESITATE TO INCREASE THE "DEFAULT_NRENDERCELL_BUFFERSIZE" IF THIS FRAME LAG
// + HAPPENS
#define DEFAULT_NRENDERCELL_BUFFERSIZE 256
#define NRENDERCELL_ADDITIONNAL_BUFFERSIZE 128
	// +
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	typedef struct
	{
		Nf32 SortingKey; // Used for Accumulator sorting
		NGEOMETRY *pGeometry;
		NTRANSFORM_HNODE *pTransformHNode;
	} NRENDERCELL;

	extern NRENDERCELL *pRenderCellsBuffer;
	extern NRENDERCELL *pCurrentRenderCell;

	inline void NEraseRenderCellsBuffer() { pCurrentRenderCell = pRenderCellsBuffer; }

	void NInitialiazeRenderCellsBuffer();
	void NDeleteRenderCellsBuffer();

	void NExtract3DRenderCell(const NGEOMETRY *pgeom, const NTRANSFORM_HNODE *ptransformhnode, const Nf32 fsortingkey);
	void NExtractHUDRenderCell(const NGEOMETRY *pgeom, const NTRANSFORM_HNODE *ptransformhnode);

	inline void NExtractAll3DRenderCell(const NRENDERABLE *prenderable, const Nf32 fsortingkey)
	{
		NGEOMETRY *pgeom = (NGEOMETRY *)prenderable->GeomArray.pFirst;
		for (Nu32 i = prenderable->GeomArray.Size; i != 0; i--, pgeom++)
		{
			// NErrorIf(!pgeom->Mesh.PrimitiveArray.Size, NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE );
			NExtract3DRenderCell(pgeom, prenderable->pTransformHNode, fsortingkey);
		}
	}

	inline void NExtractAllOrdered3DRenderCell(const NRENDERABLE *prenderable, const Nf32 fsortingkey)
	{
		Nf32 spread_intervall = prenderable->BoundingSphere.fRadius * 0.1f;
		Nf32 fsubsortingkey = fsortingkey + spread_intervall * 0.5f;
		spread_intervall /= (Nf32)prenderable->GeomArray.Size;
		NGEOMETRY *pgeom = (NGEOMETRY *)prenderable->GeomArray.pFirst;
		for (Nu32 i = prenderable->GeomArray.Size; i != 0; i--, pgeom++, fsubsortingkey -= spread_intervall)
		{
			// NErrorIf(!pgeom->Mesh.PrimitiveArray.Size, NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE );
			NExtract3DRenderCell(pgeom, prenderable->pTransformHNode, fsubsortingkey);
		}
	}
	inline void NExtractAllHUDRenderCell(const NRENDERABLE *prenderable)
	{
		NGEOMETRY *pgeom = (NGEOMETRY *)prenderable->GeomArray.pFirst;
		for (Nu32 i = prenderable->GeomArray.Size; i != 0; i--, pgeom++)
		{
			if (pgeom->Mesh.PrimitiveArray.Size)
				NExtractHUDRenderCell(pgeom, prenderable->pTransformHNode);
		}
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NRENDERCELL_H
