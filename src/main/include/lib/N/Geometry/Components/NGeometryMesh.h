#ifndef __NGEOMETRYCOMPONENT_MESH_H_
#define __NGEOMETRYCOMPONENT_MESH_H_


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NGeometryMesh.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Containers/NArray.h"
#include "../../NTexture.h"
#include "../../Miscellaneous/NColor.h"
#include "NGeometryVertex.h"
#include "NGeometryPrimitive.h"
#include "./Specifications/NGeometryMesh_Flags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum
{
	NMESH_FORMAT_NULL = 0,		
	NMESH_FORMAT_ARRAYS,			// 2 ARRAYS of Data: PRIMITIVE ARRAY and VERTEX ARRAY. These 2 ARRAYS are on "Client Side".(regarding OpenGL way of speaking) 
//	NMESH_FORMAT_ARRAYS_VBO,
//	...
//	or, maybe something more specific ... i don't know yet. Let see after VBO implementation ...
//	...
//	NMESH_FORMAT_NULL = 0,		
//	NMESH_FORMAT_ARRAYS,
// 	NMESH_FORMAT_VBO_STATIC,		// OpenGL VBO STATIC Names and some other data to use these VBO in the right way. (TODO)
// 	NMESH_FORMAT_VBO_DYNAMIC,		// OpenGL VBO DYNAMIC Names and some other data to use these VBO in the right way. (TODO)
// 	NMESH_FORMAT_VBO_MIXTE,			// OpenGL VBOs STATIC & DYNAMIC Names and some other data to use these VBO in the right way. (TODO)
// 	NMESH_FORMAT_ARRAYS_VBO_STATIC,	// ARRAYS & OpenGL VBO STATIC Names and some other data to use these VBO in the right way. (TODO)
// 	NMESH_FORMAT_ARRAYS_VBO_DYNAMIC,// ARRAYS & OpenGL VBO DYNAMIC Names and some other data to use these VBO in the right way. (TODO)
// 	NMESH_FORMAT_ARRAYS_VBO_MIXTE,	// ARRAYS & OpenGL VBO STATIC & DYNAMIC Names and some other data to use these VBO in the right way. (TODO)
	//----------------------	
	NMESH_FORMAT_ENUM_SIZE	// MAX size is 8 !!! (and max ID is 7) This ID is going to be stored on a 3 bits value !!!
}NMESH_FORMAT_ENUM;

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// ------------------------------------------------------------------------------------------------------------------------------------------
// Basic Mesh data which are Vertex (with their color for each of them) and Primitives ( triangle or line )
typedef struct 
{
	NARRAY			PrimitiveArray;		
	NARRAY			VertexArray;		
}NMESH;
// ------------------------------------------------------------------------------------------------------------------------------------------
// Description Structure to Create NMESH
// Notes:	This structure doesn't have infos about vertex and primitive sizes, only Array sizes and capacities.
//			 	
typedef struct
{
	Nu32			PrimitiveArray_Capacity;
	Nu32			PrimitiveArray_Size;

	Nu32			VertexArray_Capacity;
	Nu32			VertexArray_Size;
}NMESH_CAPACITIES; 

typedef struct  
{
	Nu32	Flags;
}NMESH_SPECS;

// ***************************************************************************************
// **					Mesh Functions													**
// ***************************************************************************************
NMESH*			NSetupMesh(NMESH *pmesh,const Nu16 primitivestructsize, const Nu16 vertexstructsize, const NMESH_CAPACITIES *pcapacitiesdesc  );
NMESH*			NCreateMesh( const Nu16 primitivestructsize, const Nu16 vertexstructsize , const NMESH_CAPACITIES *pcapacitiesdesc);
void			NClearMesh(void *pmesh);
void			NDeleteMesh(void *pmesh);
inline void		NCopyMesh(NMESH *pmeshdst, const NMESH *pmeshsrc){NCopyArray(&pmeshdst->VertexArray,&pmeshsrc->VertexArray);NCopyArray(&pmeshdst->PrimitiveArray,&pmeshsrc->PrimitiveArray);}
inline void		NEraseMesh(NMESH *pmesh){NEraseArray(&pmesh->PrimitiveArray,NULL);NEraseArray(&pmesh->VertexArray,NULL);}

// ------------------------------------------------------------------------------------------------------------------------------------------
// Fill up Geometry description
inline void		NFillupMeshCapacitiesEx(NMESH_CAPACITIES *pcapacytiesdesc, const Nu32 primitivearray_capacity, const Nu32 primitivearray_size, const Nu32 vertexarray_capacity, const Nu32 vertexarray_size )
{
	pcapacytiesdesc->PrimitiveArray_Capacity	= primitivearray_capacity;
	pcapacytiesdesc->PrimitiveArray_Size		= primitivearray_size;
	
	pcapacytiesdesc->VertexArray_Capacity		= vertexarray_capacity;
	pcapacytiesdesc->VertexArray_Size			= vertexarray_size;
}
inline void		NFillupMeshCapacities(NMESH_CAPACITIES *pcapacytiesdesc,  const Nu32 primitivearray_capacity, const Nu32 vertexarray_capacity )
{
	pcapacytiesdesc->PrimitiveArray_Capacity	= primitivearray_capacity;
	pcapacytiesdesc->PrimitiveArray_Size		= 0;
	
	pcapacytiesdesc->VertexArray_Capacity		= vertexarray_capacity;
	pcapacytiesdesc->VertexArray_Size			= 0;
}
// ------------------------------------------------------------------------------------------------------------------------------------------
// Misc...
inline	Nu32			NGetMeshVertexArraySize(const NMESH *pmesh){return pmesh->VertexArray.Size;}
inline	Nu32			NGetMeshPrimitiveArraySize(const NMESH *pmesh){return pmesh->PrimitiveArray.Size;}

inline	NBYTE*			NGetFirstMeshVertexPtr(const NMESH *pmesh){return pmesh->VertexArray.pFirst;}
inline	NLINE*			NGetFirstMeshLinePtr(const NMESH *pmesh){NErrorIf(pmesh->PrimitiveArray.ElementSize != sizeof(NLINE), NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE);return (NLINE*)pmesh->PrimitiveArray.pFirst;}
inline	NTRIANGLE*		NGetFirstMeshTrianglePtr(const NMESH *pmesh){NErrorIf(pmesh->PrimitiveArray.ElementSize != sizeof(NTRIANGLE), NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE);return (NTRIANGLE*)pmesh->PrimitiveArray.pFirst;}

#ifdef _DEBUG
#define					NPushBackMeshVertex(pmesh,pvertex,vertexstructsize)				NPushBackMeshVertex_debug(pmesh,pvertex,vertexstructsize)
#define					NPushBackMeshPrimitive(pmesh,pprimitive,primitivestructsize)	NPushBackMeshPrimitive_debug(pmesh,pprimitive,primitivestructsize)
// Don't use it directly... USE THE DEFINE !
inline	NBYTE*			NPushBackMeshVertex_debug(NMESH *pmesh,const NBYTE *pvertex, const Nu32 vertexstructsize){NErrorIf(NGetArrayElementSize(&pmesh->VertexArray)!=vertexstructsize,NERROR_GEOMETRY_WRONG_VERTEX_STRUCTURESIZE);return (NBYTE*)NArrayPushBack(&pmesh->VertexArray,(NBYTE*)pvertex);}
inline	NBYTE*			NPushBackMeshPrimitive_debug(NMESH *pmesh,const NBYTE *pprimitive, const Nu32 primitivestructsize){NErrorIf(pmesh->PrimitiveArray.ElementSize != primitivestructsize, NERROR_GEOMETRY_WRONG_PRIMITIVE_STRUCTURESIZE);return (NBYTE*)NArrayPushBack(&pmesh->PrimitiveArray,(NBYTE*)pprimitive);}
#endif
#ifndef _DEBUG
#define					NPushBackMeshVertex(pmesh,pvertex,vertexstructsize)				NArrayPushBack(&pmesh->VertexArray,(NBYTE*)pvertex)
#define					NPushBackMeshPrimitive(pmesh,pprimitive,primitivestructsize)	NArrayPushBack(&pmesh->PrimitiveArray,(NBYTE*)pprimitive)
#endif

// ***************************************************************************************
// **					Mesh "Specs" Functions											**
// ***************************************************************************************
//
// ***************************************************************************************
// **					Mesh "PRIVATE" Functions										**
// ***************************************************************************************

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRYCOMPONENT_MESH_H_

