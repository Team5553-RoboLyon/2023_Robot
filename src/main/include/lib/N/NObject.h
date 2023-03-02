#ifndef __NOBJECTS_H
#define __NOBJECTS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NObjects.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Render/Renderable/NRenderable.h"
#include "lib/N/Render/Renderable/NRenderable_ExtractSet.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define DEFAULT_NOBJECT_RENDERABLE_BOUNDINGSPHERE_RADIUS 1.0f
#define DEFAULT_NOBJECT_RENDERABLE_BOUNDINGSPHERE_CENTER_3F 0.0f, 0.0f, 0.0f
#define DEFAULT_NOBJECT_HIGHLEVEL_EXTRACTSET FLAG_NEXTRACTSET_3D

// ***************************************************************************************
// **							Structures and enum										**
// ***************************************************************************************
// Loading object
#define ASE_EXTENSION ".ase"

	// An Object
	typedef struct NOBJECT NOBJECT;
	struct NOBJECT
	{
		NRENDERABLE *pRenderable; // Ptr on a Renderable
	};

// An Object Description
#define FLAG_NOBJECT_DESC_USEPARAM_HIGHLEVELEXTRACTSET BIT_0
#define FLAG_NOBJECT_DESC_USEPARAM_INCLUDED_GEOMETRY_PREALLOCATED_CAPACITY BIT_1
#define FLAG_NOBJECT_DESC_USEPARAM_BOUNDINGSPHERE BIT_2
#define FLAG_NOBJECT_DESC_USEPARAM_TRANSFORMHNODE BIT_3

	typedef struct
	{
		Nu32 Flags;
		NEXTRACTSET HighLevelExtractSet;
		Nu32 IncludedGeometryPreallocatedCapacity;
		NBOUNDINGSPHERE BoundingSphere;
		NTRANSFORM_HNODE *pTransformHNode;
	} NOBJECT_DESC;
	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	NOBJECT *NSetup3DObject(NOBJECT *pobj, const NOBJECT_DESC *pdesc);
	NOBJECT *NCreate3DObject(const NOBJECT_DESC *pdesc);
	void NClear3DObject(void *pobj);
	void NDelete3DObject(void *pobj);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NOBJECTS_H
