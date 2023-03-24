#include "NGeometryMesh.h"

// ------------------------------------------------------------------------------------------
// NSetupMesh
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NMESH* NSetupMesh(NMESH *pmesh,const Nu16 primitivestructsize, const Nu16 vertexstructsize, const NMESH_CAPACITIES *pcapacitiesdesc  )
{
	NErrorIf(pcapacitiesdesc->PrimitiveArray_Size > pcapacitiesdesc->PrimitiveArray_Capacity, NERROR_GEOMETRY_INCONSISTENT_PARAMETER);
	NErrorIf(pcapacitiesdesc->VertexArray_Size > pcapacitiesdesc->VertexArray_Capacity, NERROR_GEOMETRY_INCONSISTENT_PARAMETER);

	NSetupArray(&pmesh->PrimitiveArray,pcapacitiesdesc->PrimitiveArray_Capacity,primitivestructsize);
	NResizeArray(&pmesh->PrimitiveArray,pcapacitiesdesc->PrimitiveArray_Size,NULL,NULL);

	NSetupArray(&pmesh->VertexArray,pcapacitiesdesc->VertexArray_Capacity,vertexstructsize);
	NResizeArray(&pmesh->VertexArray,pcapacitiesdesc->VertexArray_Size,NULL,NULL);

	return pmesh;
}
// ------------------------------------------------------------------------------------------
// NCreateMesh
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NMESH* NCreateMesh( const Nu16 primitivestructsize, const Nu16 vertexstructsize, const NMESH_CAPACITIES *pcapacitiesdesc )
{
	return NSetupMesh(NEW(NMESH), primitivestructsize, vertexstructsize, pcapacitiesdesc);
}
// ------------------------------------------------------------------------------------------
// NClearMesh
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearMesh(void *pmesh)
{
	NClearArray(&((NMESH*)pmesh)->PrimitiveArray,NULL);
	NClearArray(&((NMESH*)pmesh)->VertexArray,NULL);
}

// ------------------------------------------------------------------------------------------
// NDeleteMesh
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteMesh(void *pmesh)
{
	NClearMesh(pmesh);
	Nfree(pmesh);
}
