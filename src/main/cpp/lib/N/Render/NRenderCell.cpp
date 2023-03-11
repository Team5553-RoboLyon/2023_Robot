#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Render/Accumulator/NAccumulator.h"
#include "lib/N/NErrorHandling.h"

#include "NRenderCell.h"

// + GLOBALES VALUES
NRENDERCELL *pRenderCellsBuffer;
NRENDERCELL *pCurrentRenderCell;
NRENDERCELL *pLastRenderCell;

extern NGEOMETRY_SPECS NGeometrySpecs[];

static inline Nu32 _ptoi(const NRENDERCELL *prendercell)
{
	NErrorIf(prendercell < pRenderCellsBuffer, NERROR_ADDRESS_OUTOFRANGE);
	NErrorIf(prendercell > pLastRenderCell, NERROR_ADDRESS_OUTOFRANGE);
	return (Nu32)(prendercell - pRenderCellsBuffer);
}
static inline NRENDERCELL *_itop(const Nu32 id)
{
	NErrorIf((NRENDERCELL *)(pRenderCellsBuffer + id) < pRenderCellsBuffer, NERROR_ADDRESS_OUTOFRANGE);
	NErrorIf((NRENDERCELL *)(pRenderCellsBuffer + id) > pLastRenderCell, NERROR_ADDRESS_OUTOFRANGE);
	return (NRENDERCELL *)(pRenderCellsBuffer + id);
}

static void NEnlargeRenderCellsBuffer()
{
	Nu32 capacity;
	Nu32 i, j;
	NRENDERCELL **pptr;
	NACCUMULATOR *pacc;

	NErrorIf(!pRenderCellsBuffer, NERROR_NULL_POINTER);

	// I: Convert all current accumulator rendercell pointers into index
	pacc = NAccumulator;
	for (i = NACCUM_ID_ENUM_SIZE; i != 0; i--, pacc++)
	{
		if (!pacc->AccumulatedRenderCellsArray.Size)
			continue;

		pptr = (NRENDERCELL **)pacc->AccumulatedRenderCellsArray.pFirst;
		for (j = pacc->AccumulatedRenderCellsArray.Size; j != 0; j--, pptr++)
		{
			*pptr = (NRENDERCELL *)_ptoi(*pptr);
		}
	}

	// II: Realloc
	capacity = (pLastRenderCell - pRenderCellsBuffer) + 1;
	capacity += NRENDERCELL_ADDITIONNAL_BUFFERSIZE;
	pRenderCellsBuffer = (NRENDERCELL *)Nrealloc(pRenderCellsBuffer, capacity * sizeof(NRENDERCELL));
	pLastRenderCell = pRenderCellsBuffer + capacity - 1;

	// III: Convert back to pointers
	pacc = NAccumulator;
	for (i = NACCUM_ID_ENUM_SIZE; i != 0; i--, pacc++)
	{
		if (!pacc->AccumulatedRenderCellsArray.Size)
			continue;

		pptr = (NRENDERCELL **)pacc->AccumulatedRenderCellsArray.pFirst;
		for (j = pacc->AccumulatedRenderCellsArray.Size; j != 0; j--, pptr++)
		{
			*pptr = _itop((Nu32)*pptr);
		}
	}
}

void NInitialiazeRenderCellsBuffer()
{
	NErrorIf(pRenderCellsBuffer, NERROR_NON_NULL_POINTER);

	pRenderCellsBuffer = (NRENDERCELL *)Nmalloc(DEFAULT_NRENDERCELL_BUFFERSIZE * sizeof(NRENDERCELL));
	pCurrentRenderCell = pRenderCellsBuffer;
	pLastRenderCell = pCurrentRenderCell + (DEFAULT_NRENDERCELL_BUFFERSIZE - 1);
}

void NDeleteRenderCellsBuffer()
{
	NErrorIf(!pRenderCellsBuffer, NERROR_NULL_POINTER);
	Nfree(pRenderCellsBuffer);
	pRenderCellsBuffer = NULL;
	pCurrentRenderCell = NULL;
	pLastRenderCell = NULL;
}

void NExtract3DRenderCell(const NGEOMETRY *pgeom, const NTRANSFORM_HNODE *ptransformhnode, const Nf32 fsortingkey)
{
// ---------- DEBUG ONLY ----------
#ifdef _DEBUG
	NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS)].Debug_CheckFunction(pgeom);
#endif
	// ---------- DEBUG ONLY ----------

	pCurrentRenderCell->SortingKey = fsortingkey;
	pCurrentRenderCell->pGeometry = (NGEOMETRY *)pgeom;
	pCurrentRenderCell->pTransformHNode = (NTRANSFORM_HNODE *)ptransformhnode;

	// Push this RenderCell address into the right Accumulator
	NArrayPushBack(&NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].AccumulatedRenderCellsArray, (NBYTE *)&pCurrentRenderCell);
	pCurrentRenderCell++;

	// Overloading Test
	if (pCurrentRenderCell > pLastRenderCell)
	{
		pCurrentRenderCell = (NRENDERCELL *)(_ptoi(pCurrentRenderCell - 1));
		NEnlargeRenderCellsBuffer();
		pCurrentRenderCell = (NRENDERCELL *)(_itop((Nu32)pCurrentRenderCell + 1));
	}
}

void NExtractHUDRenderCell(const NGEOMETRY *pgeom, const NTRANSFORM_HNODE *ptransformhnode)
{
// ---------- DEBUG ONLY ----------
#ifdef _DEBUG
	NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS)].Debug_CheckFunction(pgeom);
#endif
	// ---------- DEBUG ONLY ----------

	// pCurrentRenderCell->SortingKey	= 0.0f; Not necessarry to set it. It will still unused !
	pCurrentRenderCell->pGeometry = (NGEOMETRY *)pgeom;
	pCurrentRenderCell->pTransformHNode = (NTRANSFORM_HNODE *)ptransformhnode;

	// Push this RenderCell address into the HUD Accumulator
	NArrayPushBack(&NAccumulator[NACCUM_HUD].AccumulatedRenderCellsArray, (NBYTE *)&pCurrentRenderCell);
	pCurrentRenderCell++;

	// Overloading Test
	if (pCurrentRenderCell > pLastRenderCell)
	{
		pCurrentRenderCell = (NRENDERCELL *)(_ptoi(pCurrentRenderCell - 1));
		NEnlargeRenderCellsBuffer();
		pCurrentRenderCell = (NRENDERCELL *)(_itop((Nu32)pCurrentRenderCell + 1));
	}
}
