#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/NString.h"
#include "lib/N/NTexture.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NErrorHandling.h"

static Nu32 _get_treelocationmap(NARRAY *pmap_array, const NTRANSFORM_HNODE *pthn)
{
	Nu32 location_hint;
	NHIERARCHY_NODE *pa = (NHIERARCHY_NODE *)pthn;

	NErrorIf(NGetArraySize(pmap_array), NERROR_ARRAY_NOT_EMPTY);
	NErrorIf(!pthn->pParent, NERROR_NNODE_ORPHAN);

	while (pa->pParent != NULL)
	{
		location_hint = NNodeGetIndex(&((NHIERARCHY_NODE *)pa->pParent)->ChildrenList, (NNODE *)pa);
		NArrayPushBack(pmap_array, (NBYTE *)&location_hint);
		pa = (NHIERARCHY_NODE *)pa->pParent;
	}

	return NGetArraySize(pmap_array);
}
static NTRANSFORM_HNODE *_get_transformhnode_from_treelocationmap(NARRAY *pmap_array, const NTRANSFORM_HNODE *proot)
{
	Nu32 *phint;
	Nu32 i;
	NTRANSFORM_HNODE *pa = (NTRANSFORM_HNODE *)proot;

	NErrorIf(!NGetArraySize(pmap_array), NERROR_ARRAY_WRONG_SIZE);

	// Read hints backward
	phint = (Nu32 *)NGetLastArrayPtr(pmap_array);
	for (i = NGetArraySize(pmap_array); i != 0; i--, phint--)
	{
		pa = (NTRANSFORM_HNODE *)NNodeGetIndexedNode(&pa->ChildrenList, *phint);
	}

	return pa;
}

// ------------------------------------------------------------------------------------------
// NFileWriteTransformHNodeReference
// ------------------------------------------------------------------------------------------
// Description :
// Write a TransformHNode Pointer reference
// !! This function doesn't write a TransformHNode into a file to reload it.
// But a reference on it. So at reading the "NFileReadTransformHNodeReference" should be able
// to find back the right TransformHNode in the Transformation Tree.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			:	a Valid pointer on an open file
//		pthn			:	a valid pointer on a valid NTRANSFORM_HNODE
// Out :
//		0 on Error
//		1 on success
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteTransformHNodeReference(NFILE *pfile, const NTRANSFORM_HNODE *pthn)
{
	NErrorIf(!pfile, NERROR_NULL_POINTER);

	NHEADER_TRANSFORM_HNODE_REFERENCE header;
	NARRAY location_hints_array;
	Nu32 deep;

	// Header
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_TRANSFORM_HNODE_REFERENCE) != 1)
		return 0;

	memset(&header, 0, sizeof(NHEADER_TRANSFORM_HNODE_REFERENCE));
	header.TransformationTreeID = 0; // it will be set with a NTRANSFORM_HNODE member

	if (!pthn)
	{
		FLAG_ON(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_NULL);
	}
	else
	{
		deep = NGetHNodeDeep((NHIERARCHY_NODE *)pthn);
		if (deep)
		{
			NSetupArray(&location_hints_array, deep, sizeof(Nu32));
			_get_treelocationmap(&location_hints_array, pthn);
		}
		else
		{
			FLAG_ON(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_ROOT);
		}
	}

	if (NFileWrite(pfile, &header, sizeof(NHEADER_TRANSFORM_HNODE_REFERENCE), 1) != 1)
	{
		if (ISFLAG_OFF(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_ROOT) && ISFLAG_OFF(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_NULL))
		{
			NClearArray(&location_hints_array, NULL);
		}
		return 0;
	}

	// Data
	if (ISFLAG_OFF(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_ROOT) && ISFLAG_OFF(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_NULL))
	{
		if (!NFileWriteArray(pfile, &location_hints_array, NULL, 0))
		{
			NClearArray(&location_hints_array, NULL);
		}
		return 0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
// NFileReadTransformHNodeReference
// ------------------------------------------------------------------------------------------
// Description :
//		Read a TransformHNode Pointer reference written with "NFileWriteTransformHNodeReference"
//		!! This function doesn't load a TransformHNode with its content (MATRIX, position, rotation) but a reference on
//		an already created TransformHNode.
//		There is no guarantee that the return pthn is exactly the same than the one at writing.
//		In the best case the returned one has the same hierarchic position than the written one.
//		In the worse, it could be the root node or NULL.
// ------------------------------------------------------------------------------------------
// In  :
//		pfile			:	a Valid pointer on an open file
// Out :
//		0 on Error ( ppthn remains unchanged )
//		1 On success
//
// ------------------------------------------------------------------------------------------
Nu32 NFileReadTransformHNodeReference(NFILE *pfile, NTRANSFORM_HNODE **ppthn)
{
	NErrorIf(!pfile || !ppthn, NERROR_NULL_POINTER);

	NHEADER_TRANSFORM_HNODE_REFERENCE header;
	Nu32 version;
	NARRAY location_hints_array;

	// Check Incoming version.
	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current main version (alls versions 0.0.x):
	case NGETVERSION_MAIN(VERSION_NHEADER_TRANSFORM_HNODE_REFERENCE):
		memset(&header, 0, sizeof(NHEADER_TRANSFORM_HNODE_REFERENCE));
		if (NFileRead(pfile, &header, sizeof(NHEADER_TRANSFORM_HNODE_REFERENCE), 1) != 1)
			return 0;

		if (ISFLAG_ON(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_NULL))
		{
			*ppthn = NULL;
			return 1;
		}
		else if (ISFLAG_ON(header.Flags, FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_ROOT))
		{
			// It should be : return &NTransformationTreeRoot[header.TransformarionTreeID];
			*ppthn = NGetTransformHNodeRoot();
			return 1;
		}
		else
		{
			NSetupArray(&location_hints_array, 0, sizeof(Nu32));
			if (!NFileReadArray(pfile, &location_hints_array, NULL, 0))
			{
				NClearArray(&location_hints_array, NULL);
				return 0;
			}

			// Use location_hints to get the node from the root
			// It should be : return _get_transformhnode_from_treelocationmap(&location_hints_array, &NTransformationTreeRoot[header.TransformarionTreeID]);

			*ppthn = _get_transformhnode_from_treelocationmap(&location_hints_array, NGetTransformHNodeRoot());
			return 1;
		}
		break;

	// Unknown version:
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
	//	return 0;
}
