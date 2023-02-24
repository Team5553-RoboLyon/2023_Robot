#ifndef __NCOLLECTIBLES_H
#define __NCOLLECTIBLES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NCollectibles.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "../Containers/NArray.h"
#include "../Containers/NXNode.h"
#include "../Miscellaneous/NColor.h"
#include "../Miscellaneous/NColorGradient.h"
#include "../Miscellaneous/NVertexColorBuild.h"
#include "../Miscellaneous/NVertexTexCoordBuild.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../BasicElements/NTextureAtlas.h"
#include "../Render/Renderable/NRenderable.h"
#include "../Render/Renderable/NRenderable_ExtractSet.h"
#include "../BasicElements/NTextureAtlas.h"
#include "../NTexture.h"
#include "../NBoundingVolumes.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures and enum										**
// ***************************************************************************************
// Collectible default values
#define CONSTANT_NCOLLECTIBLE_LOWLEVEL_EXTRACTSET NEXTRACTSET_SECTION_STANDARD

#define DEFAULT_NCOLLECTIBLE_HIGHLEVEL_EXTRACTSET FLAG_NEXTRACTSET_3D
#define DEFAULT_NCOLLECTIBLE_RENDERABLE_BOUNDINGSPHERE_RADIUS 1.0f									  // used by NCollectible Setup function
#define DEFAULT_NCOLLECTIBLE_DESC_GEOMETRY_SPECS_ID NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE // used by NCollectible Setup function
#define DEFAULT_NCOLLECTIBLE_USERSTATESET 0

#define DEFAULT_NCOLLECTIBLE_ELEMENT_RADIUS 0.1f
#define DEFAULT_NCOLLECTIBLE_ELEMENT_COUNT 1

	// Collectible Description for building
	typedef struct NCOLLECTIBLE NCOLLECTIBLE;
	typedef struct NCOLLECTIBLE_PICKER NCOLLECTIBLE_PICKER;
	typedef void (*NCOLLECTIBLE_GETPOSITION_CALLBACK)(NVEC3 *pvpos, const NCOLLECTIBLE *pcollectible, const Nu32 i);
	typedef Nbool (*NCOLLECTIBLE_PICKING_CALLBACK)(const NCOLLECTIBLE_PICKER *ppicker, const NCOLLECTIBLE *pcollectible, const Nu32 i, NVEC3 *pvposition);

#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_GEOMETRY_FORMAT BIT_0
#define FLAG_NCOLLECTIBLE_DESC_USE_USERSTATESET BIT_1
#define FLAG_NCOLLECTIBLE_DESC_USE_HIGHLEVEL_EXTRACTSET BIT_2
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR BIT_3 // if not, BillBoard are going to be created with "XY" default plane
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_SCALE BIT_4					   // to create BB, are we going to use Scale ?
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_GET_ELEMENT_POSITION BIT_5			   // if not, BillBoard collectible are going to use default one (place all billboard along X axis )
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_PICKING_CALLBACK BIT_6				   // if not, BillBoard collectible are going to use default one (does nothing, return NTRUE )
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_SIZE BIT_7					   // to create BB, are we going to use pTexture ?
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_APPEARANCE BIT_8					   // to create BB, are we going to use pTexture ?
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_RADIUS BIT_9				   // to create BB, are we going to use Radius ?
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_ELEMENT_COUNT BIT_10				   // to create BB, are we going to use element count ?
#define FLAG_NCOLLECTIBLE_DESC_USEPARAM_TRANSFORMHNODE BIT_11				   // to create BB, are we going to use a transformHNode ?

	typedef struct
	{
		Nu16 Flags;
		Nu8 ElementCount;
		Nu8 Available_Nu8;
		NCOLLECTIBLE_GETPOSITION_CALLBACK pGetElementPosition;
		NCOLLECTIBLE_PICKING_CALLBACK pPicking_CallBack;

		// Parameters for Element(s) Creation:
		NEXTRACTSET HighLevelExtractSet;
		NGEOMETRY_FORMAT_ENUM GeometryFormat;
		NSTATESET UserStateSet;

		NVEC3 ConstructionPlane_DirectorVector;
		NVEC2 ElementScale;
		NVEC2 ElementSize;
		Nf32 ElementRadius; // Picking Radius of one Element ( a collectible could have several elements, 32 at maximum )
		NAPPEARANCE_RAW Appearance_Raw;
		NBUILDBASE_TEXCOORD TexCoordBuild;
		NBUILDBASE_COLOR ColorBuild;
		NTRANSFORM_HNODE *pTransformHNode;
	} NCOLLECTIBLE_DESC;

	// A Collectible
	struct NCOLLECTIBLE
	{
		NRENDERABLE *pRenderable;							   // Ptr on a Renderable
		NAABB AABB;											   // Global Bounding volume (for the 32 billboard collectibles)
		Nf32 ElementRadius;									   // Picking Radius of one Element ( a collectible could have several elements, 32 at maximum )
		NCOLLECTIBLE_GETPOSITION_CALLBACK pGetElementPosition; // To get position of each collectible element (BillBoard)
		NCOLLECTIBLE_PICKING_CALLBACK pPicking_CallBack;	   // In case of collectible picking ... this callback will be call. must return NTRUE to validate the picking
		NCOLLECTIBLEVERTEX VertexShape[4];					   // To Store one Billboard description
		Nu32 BitShape;										   // 32 bits to describe 32 collectible Elements status (active or not), so 1 bit by element.
		Nu8 ElementCount;									   // Number of collectible elements used. MAX is 32
		Nu8 User8;											   // User value 8 bits !
		Nu16 BindID;										   // Index into NCOLLECTIBLE_SET (16 bits only !) if the collectible is Bound, of course

		Nu32 User32; // User value 32 bits !
	};

	struct NCOLLECTIBLE_PICKER
	{
		Nf32 Radius;	  // Detection Radius of the Picker.
		NVEC3 *pPosition; // a valid pointer on the Picker position
	};

	// TEMPORARY
	typedef struct
	{
		NARRAY PickerArray;
		NARRAY CollectiblePtrArray;
	} NCOLLECTIBLE_SET;
	/*
	// THIS Struct will be :
	typedef struct
	{
		NRNODE_CONTAINER	RNodeContainer;
		NARRAY				CollectiblePtrArray;
		NARRAY				PickerArray;
	}NCOLLECTIBLE_SET;

	typedef struct
	{
		NRNODE				RNode;
		NRNODE				pRNode_First;
		NRNODE				pRNode_Last;
	}NRNODE_CONTAINER;

	*/

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************

	// Collectible Picker
	NCOLLECTIBLE_PICKER *NSetupCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker, const NVEC3 *ppickerposition, const Nf32 radius);
	void NClearCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker);
	NCOLLECTIBLE_PICKER *NCreateCollectiblePicker(const NVEC3 *ppickerposition, const Nf32 radius);
	void NDeleteCollectiblePicker(NCOLLECTIBLE_PICKER *ppicker);

	inline void NSetCollectiblePickerRadius(NCOLLECTIBLE_PICKER *ppicker, const Nf32 radius) { ppicker->Radius = radius; }
	inline void NSetCollectiblePickerPositionPtr(NCOLLECTIBLE_PICKER *ppicker, const NVEC3 *ptr) { ppicker->pPosition = (NVEC3 *)ptr; }

	// Collectible
	NCOLLECTIBLE *NSetupCollectible(NCOLLECTIBLE *pcollec, const NCOLLECTIBLE_DESC *pdesc);
	void NClearCollectible(NCOLLECTIBLE *pcollec);
	NCOLLECTIBLE *NCreateCollectible(const NCOLLECTIBLE_DESC *pdesc);
	void NDeleteCollectible(NCOLLECTIBLE *pcollec);

	inline void NSetCollectibleUser32(NCOLLECTIBLE *pcollec, const Nu32 user32) { pcollec->User32 = user32; }
	inline void NSetCollectibleELementRadius(NCOLLECTIBLE *pcollec, const Nf32 radius) { pcollec->ElementRadius = radius; }

	void NSetCollectibleBitShape(NCOLLECTIBLE *pcollec, const Nu32 bitshape);
	void NUpdateCollectibleAABB(NCOLLECTIBLE *pcollec);

	inline Nu32 NGetCollectibleElementCount(NCOLLECTIBLE *pcollec) { return pcollec->ElementCount; }
	Nu32 NGetEnableCollectibleElementCount(NCOLLECTIBLE *pcollec);

	// Collectible Set
	NCOLLECTIBLE_SET *NSetupCollectibleSet(NCOLLECTIBLE_SET *pcset, const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity);
	NCOLLECTIBLE_SET *NCreateCollectibleSet(const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity);
	void NClearCollectibleSet(NCOLLECTIBLE_SET *pcset);
	void NDeleteCollectibleSet(NCOLLECTIBLE_SET *pcset);

	// Collectible Picking
	void NInitCollectiblePicking(const Nu16 collectible_ptr_capacity, const Nu32 picker_capacity);
	void NQuitCollectiblePicking();

	void NEnableCollectibleDetection(NCOLLECTIBLE *pcollectible);
	void NDisableCollectibleDetection(NCOLLECTIBLE *pcollectible);
	void NDisableAllCollectibleDetection();
#define NIsDetectableCollectible(pcol) ((pcol)->BindID != 0xFFFF)

	Nu32 NInsertCollectiblePicker(const NVEC3 *ppickerposition, const Nf32 radius);
	void NEraseCollectiblePicker(const Nu32 index);
	void NEraseAllCollectiblePicker();
	Nu32 NGetCollectiblePickerCount();

	void NUpdateCollectiblePicking();

// Updates
// .......
// 2 ways to do the same (Direct Update or PostUpdate processing)...
// ... if you want to use the Postupdate processing, you have to Bind it to a Valid POSTUPDATE UID.
// ... And for each of them, 2 possible storage (Array or XNodeList)
/*
void							NUpdateCollectiblesPickingArray(NARRAY *pcollectiblepickingcouple_addressarray);
void							NCollectiblesPickingArray_PostUpdateHandle(const NTIME *ptime, Nu32 dataA,Nu32 dataB);
// ... And with an XNodeList
void							NUpdateCollectiblesPickingXNodeList(NXNODELIST *pcollectiblepickingcouple_addressXNodeList);
void							NCollectiblesPickingXNodeList_PostUpdateHandle(const NTIME *ptime, Nu32 dataA,Nu32 dataB);
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NCOLLECTIBLES_H
