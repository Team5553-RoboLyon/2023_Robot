#ifndef __NFILEEXT_H
#define __NFILEEXT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NFileExt.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NType.h"
#include "lib/N/NSpline.h"
#include "lib/N/NStructure.h"
#include "lib/N/NPool.h"
#include "lib/N/Core/NVersion.h"
#include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"
#include "NFile.h"

// #include "NFlags.h"
#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Structures/Define/Enum								**
// ***************************************************************************************

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NUT Library Entry
// +

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NUT Library Entry
// +
#define VERSION_NHEADER_LIBRARY_ENTRY NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 KeySize; // current size of the key String, null terminated character excluded
	} NHEADER_LIBRARY_ENTRY;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NARRAY
// +
#define VERSION_NHEADER_ARRAY NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 Size;		  // current size of the Array ( = current number of elements in the Array )
		Nu32 Capacity;	  // Max possible size of the Array ( pre-allocated and bigger than Size )
		Nu32 ElementSize; // size of one element of the Array
	} NHEADER_ARRAY;
	typedef Nu32 (*NARRAY_RWELEMENT_CALLBACK)(NBYTE *ptr, NFILE *pfile, Nu32 userdata);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NSPLINE
// +
#define VERSION_NHEADER_SPLINE NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 Flags;
		Nu32 BezierAccuracy;
	} NHEADER_SPLINE;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NHASHMAP
// +
#define VERSION_NHEADER_HASHKEY_REFERENCE NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 HashKeySize;
	} NHEADER_HASHKEY_REFERENCE;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTRANSFORM_HNODE
// +
#define VERSION_NHEADER_TRANSFORM_HNODE_REFERENCE NMAKEVERSION(0, 0, 0)
#define FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_NULL BIT_0
#define FLAG_NHEADER_TRANSFORM_HNODE_REFERENCE_IS_ROOT BIT_1
	typedef struct
	{
		Nu16 Flags;
		Nu16 TransformationTreeID; // For the potential Future NEngine TransformationTree update Optimization
	} NHEADER_TRANSFORM_HNODE_REFERENCE;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NRENDERABLE
// +
#define VERSION_NHEADER_RENDERABLE NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		NBOUNDINGSPHERE BoundingSphere;
		NRENDERABLE_UPDATE_FCT_ID_ENUM Update_FCT_ID;
		NRENDERABLE_EXTRACT_FCT_ID_ENUM Extract_FCT_ID;

	} NHEADER_RENDERABLE;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NSKIN
	// +
	typedef struct
	{
		// ........................................................................................................................
		// Common Part
		NSKIN_TYPE SkinType;								   // BILLBOARD, ROPE or GRID ?
		NSKIN_SCHEME SkinScheme;							   // What update function ?
		NSTRUCTURE_ELEMENT_TYPE_ENUM Ref_StructureElementType; // - Type of the structure elements used to create and update the skin.
		Nu32 Geometry;										   // - Index of the geometry where the skin GFX data are stored
		Nu32 Ref_FirstVertex;								   // - Index of the first Vertex Geometry used by the skin.
															   // ........................................................................................................................

	} NHEADER_SKINCOMMON;

	typedef struct
	{
		// ........................................................................................................................
		// Common Part
		NSKIN_TYPE SkinType;								   // BILLBOARD, ROPE or GRID ?
		NSKIN_SCHEME SkinScheme;							   // What update function ?
		NSTRUCTURE_ELEMENT_TYPE_ENUM Ref_StructureElementType; // - Type of the structure elements used to create and update the skin.
		Nu32 Geometry;										   // - Index of the geometry where the skin GFX data are stored
		Nu32 Ref_FirstVertex;								   // - Index of the first Vertex Geometry used by the skin.
		// ........................................................................................................................

		Nu32 Ref_Center;		// - Index of the structure element used to create/update Billboard Center
		Nu32 Ref_Orient_A;		// - Index of the structure element used to create/update Orient Vector AB
		Nu32 Ref_Orient_B;		// - Index of the structure element used to create/update Orient Vector AB
		Nf32 fScale;			// - Scale of the Billboard
		Nf32 RatioWidth_Height; // Width/height of the Billboard ( BB is a 2D rectangle, instead of keeping extents we just keep this ratio. We do that because there is Scale too)
		NVEC3 VShift;			// - To shift (translate) Billboard from its ref_center point to its final position.
	} NHEADER_SKINBILLBOARD;

	typedef struct
	{
		// ........................................................................................................................
		// Common Part
		NSKIN_TYPE SkinType;								   // BILLBOARD, ROPE or GRID ?
		NSKIN_SCHEME SkinScheme;							   // What update function ?
		NSTRUCTURE_ELEMENT_TYPE_ENUM Ref_StructureElementType; // - Type of the structure elements used to create and update the skin.
		Nu32 Geometry;										   // - Index of the geometry where the skin GFX data are stored
		Nu32 Ref_FirstVertex;								   // - Index of the first Vertex Geometry used by the skin.
		// ........................................................................................................................

		Nu32 Ref_First;		// - Index of the structure element used to create/update Billboard Center
		Nu32 Ref_RangeSize; // - Size of the element range (=number of ALL elements) used first included.
							// Nf32					TwistBulgeFactor;			// - A factor to magnify the bulge created by the rope twist.
							//    A value of 0 cancel the bulge, a negative one reverse it !

		// Note: the 2 NARRAY (SideAVertebra and SideBVertebra ) are going to be saved just after this header.
		//...So, it's not necessary to reference them here.

	} NHEADER_SKINROPE;

	typedef struct
	{
		// ........................................................................................................................
		// Common Part
		NSKIN_TYPE SkinType;								   // BILLBOARD, ROPE or GRID ?
		NSKIN_SCHEME SkinScheme;							   // What update function ?
		NSTRUCTURE_ELEMENT_TYPE_ENUM Ref_StructureElementType; // - Type of the structure elements used to create and update the skin.
		Nu32 Geometry;										   // - Index of the geometry where the skin GFX data are stored
		Nu32 Ref_FirstVertex;								   // - Index of the first Vertex Geometry used by the skin.
		// ........................................................................................................................

		Nu32 Ref_First;		// - Index of the structure element used to create/update Billboard Center
		Nu32 Ref_RangeSize; // - Size of the element range (= number of ALL elements) used first included.
		// Nu32					Ref_ColumnSize;				// - Size of one element column(= number of joints in one joints column)
		Nu32 I_CellsNb; // - Size of one element column(= number of joints in one joints column)
		Nu32 J_CellsNb; // - Size of one element column(= number of joints in one joints column)

	} NHEADER_SKINGRID;

#define VERSION_NHEADER_SKIN NMAKEVERSION(0, 0, 0)
	typedef union
	{
		NHEADER_SKINCOMMON Common;
		NHEADER_SKINBILLBOARD BillBoard;
		NHEADER_SKINGRID Grid;
		NHEADER_SKINROPE Rope;
	} NHEADER_SKIN;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NSTRUCTURE
// +
#define NSTRUCTURE_FILEBLOCK_MAIN_FCTID_UNKNOWN 0
#define NSTRUCTURE_FILEBLOCK_MAIN_FCTID_EULER 1
#define NSTRUCTURE_FILEBLOCK_MAIN_FCTID_VERLET 2

#define VERSION_NSTRUCTURE_FILEBLOCK_MAIN_V000 NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 Flags;
		Nbool WithRenderable;
		Nf32 JointSpeedDamping;
		NVEC3 vGravity;
		NAABB JointsAABB;
	} NSTRUCTURE_FILEBLOCK_MAIN_V000; // NHEADER_STRUCTURE;

#define VERSION_NSTRUCTURE_FILEBLOCK_MAIN_V100 NMAKEVERSION(1, 0, 0)
	typedef struct
	{
		Nu8 WithRenderable;
		Nu8 FctID_AccumulateForce;
		Nu8 FctID_Integration;
		Nu8 FctID_SatisfyConstraint;

		Nu32 Flags;
		Nf32 JointSpeedDamping;
		NVEC3 vGravity;
		NAABB JointsAABB;
	} NSTRUCTURE_FILEBLOCK_MAIN_V100;

#define VERSION_NHEADER_CONSTRAINT NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 Ref_jA;
		Nu32 Ref_jB;
		NVEC3 AB;
		Nf32 f[3];
	} NHEADER_CONSTRAINT; // Works with all the different constraint (spring, minmax, special, ..., all of them )

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NAPPEARANCE
	// +
	typedef Nu32 (*NAPPEARANCE_WRITETEXTURE_CALLBACK)(NFILE *pfile, const NTEXTURE *ptr);
	typedef Nu32 (*NAPPEARANCE_READTEXTURE_CALLBACK)(NFILE *pfile, NTEXTURE **pptr);
	typedef struct
	{
		NAPPEARANCE_WRITETEXTURE_CALLBACK pWrite;
		NAPPEARANCE_READTEXTURE_CALLBACK pRead;
	} NRW_APPEARANCE_TEXTURE;

	typedef Nu32 (*NAPPEARANCE_WRITEBLEND_CALLBACK)(NFILE *pfile, const NBLEND *ptr);
	typedef Nu32 (*NAPPEARANCE_READBLEND_CALLBACK)(NFILE *pfile, NBLEND **pptr);
	typedef struct
	{
		NAPPEARANCE_WRITEBLEND_CALLBACK pWrite;
		NAPPEARANCE_READBLEND_CALLBACK pRead;
	} NRW_APPEARANCE_BLEND;

	typedef Nu32 (*NAPPEARANCE_WRITEMATERIAL_CALLBACK)(NFILE *pfile, const NMATERIAL *ptr);
	typedef Nu32 (*NAPPEARANCE_READMATERIAL_CALLBACK)(NFILE *pfile, NMATERIAL **pptr);
	typedef struct
	{
		NAPPEARANCE_WRITEMATERIAL_CALLBACK pWrite;
		NAPPEARANCE_READMATERIAL_CALLBACK pRead;
	} NRW_APPEARANCE_MATERIAL;

#define FLAG_NHEADER_TEXTUREUNIT_TEXTURE BIT_0		// Is there a Texture linked ?
#define FLAG_NHEADER_TEXTUREUNIT_UVMATRIXANIM BIT_1 // Is there a UVMatrixAnim linked ?
#define VERSION_NHEADER_TEXTUREUNIT NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu8 Flags;
		Nu8 Available_Nu8;
		Nu16 Available_Nu16;
	} NHEADER_TEXTUREUNIT;

#define FLAG_NHEADER_APPEARANCE_BLEND BIT_0	   // Is there BLEND data linked ?
#define FLAG_NHEADER_APPEARANCE_MATERIAL BIT_1 // Is there MATERIAL data linked ?

#define NHEADER_APPEARANCE_RW_FORMAT_NULL ((Nu8)NAPPEARANCE_FORMAT_NULL)
#define NHEADER_APPEARANCE_RW_FORMAT_TT ((Nu8)NAPPEARANCE_FORMAT_TT)
#define NHEADER_APPEARANCE_RW_FORMAT_TM ((Nu8)NAPPEARANCE_FORMAT_TM)
#define NHEADER_APPEARANCE_RW_FORMAT_ULTD_T ((Nu8)NAPPEARANCE_FORMAT_ULTD_T)
#define NHEADER_APPEARANCE_RW_FORMAT_ULTD_TM ((Nu8)NAPPEARANCE_FORMAT_ULTD_TM)

#define NHEADER_APPEARANCE_RW_FORMAT_RAW 0xFF // NAPPEARANCE_FORMAT_RAW doesn't exist !!! because NAPPEARANCE_RAW is not
											  // a part of the NAPPEARANCE union... it's something else which is never geometry included into !!!
#define VERSION_NHEADER_APPEARANCE NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu16 Flags;
		Nu8 RWAppearanceFormat;
		Nu8 LastUsedTextureUnitID; // To avoid writing/reading too much NULL references of empty TextureUnit.

		Nu8 TextureUnitFormat;
		Nu8 TextureUnitNb;
		Nu16 TextureUnitStructureSize;

		Nu32 RWAppearance_IDs;
	} NHEADER_APPEARANCE;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NBLEND
// +
#define VERSION_NHEADER_BLEND NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 Flags;				// == NBLEND.Flags
		GLenum Blend0_Equation; // == NBLEND.Blend0_Equation
		GLenum Blend0_Sfactor;	// == NBLEND.Blend0_Sfactor
		GLenum Blend0_Dfactor;	// == NBLEND.Blend0_Dfactor

		GLenum Blend1_Equation; // == NBLEND.Blend1_Equation
		GLenum Blend1_Sfactor;	// == NBLEND.Blend1_Sfactor
		GLenum Blend1_Dfactor;	// == NBLEND.Blend1_Dfactor
	} NHEADER_BLEND;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NMATERIAL
// +
#define VERSION_NHEADER_MATERIAL NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		NCOLOR AmbientColor;
		NCOLOR DiffuseColor;
		NCOLOR SpecularColor;
		NCOLOR EmissionColor;
		Nf32 Shininess;
	} NHEADER_MATERIAL;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTEXTURE PARAMETERS
// +
#define VERSION_NHEADER_TEXTURE_PARAMETERS NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		GLint Min_Filter;
		GLint Mag_Filter;
		GLint Wrap_s;
		GLint Wrap_t;
	} NHEADER_TEXTURE_PARAMETERS;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTEXTURE PARAMETERS
// +
#define VERSION_NHEADER_TEXTURE_BANK NMAKEVERSION(0, 0, 0)
#define FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED BIT_0
#define FLAG_NHEADER_TEXTURE_BANK_TEXTURE_IMAGEDATA_INCLUDED BIT_1
#define MASK_NHEADER_TEXTURE_BANK_ELEMENTS_INCLUDED (FLAG_NHEADER_TEXTURE_BANK_TEXTURE_PARAMETERS_INCLUDED | FLAG_NHEADER_TEXTURE_BANK_TEXTURE_IMAGEDATA_INCLUDED)
	typedef struct
	{
		Nu32 Flags;
		Nu32 BankSize; // Number of Textures which belong to the bank
	} NHEADER_TEXTURE_BANK;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NGEOMETRY
// +
#define VERSION_NHEADER_GEOMETRY NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu32 IDS; // The Geometry IDS with all the formats pack into
	} NHEADER_GEOMETRY;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NPOOL
// +
#define VERSION_NPOOL_FILEBLOCK_V000 NMAKEVERSION(0, 0, 0)
	typedef struct
	{
		Nu8 Type; // 0 to 255 = NPOOLTAG_TYPE_VALUEMAX
		//--------------------------------------
		Nu8 User_8;	  // 8  bits User value
		Nu16 User_16; // 16 bits User value
		//--------------------------------------
		Nu32 A_User_32; // 32 bits User value
		Nu32 B_User_32; // 32 bits User value
		//--------------------------------------
		NVEC3 Pos;
	} NPOOLTAG_FILEBLOCK_V000;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + Null terminated String of Nchar
	// +
	/*
	#define VERSION_NHEADER_STRING				NMAKEVERSION(0,0,0)
	typedef struct
	{
		Nu32		Size;			// current size of the String, null terminated character excluded
	}NHEADER_STRING;
	*/

	// ***************************************************************************************
	// **										Functions									**
	// ***************************************************************************************
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NARRAY
	// +
	Nu32 NFileWriteArray(NFILE *pfile, const NARRAY *parray, NARRAY_RWELEMENT_CALLBACK write_element, Nu32 userdata);
	Nu32 NFileReadArray(NFILE *pfile, NARRAY *parray, NARRAY_RWELEMENT_CALLBACK read_element, Nu32 userdata);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NSPLINE
	// +
	Nu32 NFileWriteSpline(NFILE *pfile, const NSPLINE *pspline, NARRAY_RWELEMENT_CALLBACK write_splineknot, Nu32 userdata);
	Nu32 NFileReadSpline(NFILE *pfile, NSPLINE *pspline, NARRAY_RWELEMENT_CALLBACK read_splineknot, Nu32 userdata);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NTRANSFORM_HNODE
	// +
	Nu32 NFileWriteTransformHNodeReference(NFILE *pfile, const NTRANSFORM_HNODE *pthn);
	Nu32 NFileReadTransformHNodeReference(NFILE *pfile, NTRANSFORM_HNODE **ppthn);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NSTRUCTURE
	// +
	Nu32 NFileWritePhysicStructure(NFILE *pfile, const NSTRUCTURE *ps);
	Nu32 NFileReadPhysicStructure(NFILE *pfile, NSTRUCTURE *ps);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NPOOL
	// +
	Nu32 NFileWritePoolTagList(NFILE *pfile, const NPOOL *ppool);
	Nu32 NFileReadPoolTagList(NFILE *pfile, NPOOL *ppool);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NGEOMETRY
	// +
	Nu32 NFileWriteGeometry(NFILE *pfile, const NGEOMETRY *pgeom);
	Nu32 NFileReadGeometry(NFILE *pfile, NGEOMETRY *pgeom);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NRENDERABLE
	// +
	Nu32 NFileWriteRenderableGeometryArray(NFILE *pfile, const NRENDERABLE *pr);
	Nu32 NFileReadRenderableGeometryArray(NFILE *pfile, NRENDERABLE *pr);
	Nu32 NFileWriteRenderable(NFILE *pfile, const NRENDERABLE *pr);
	Nu32 NFileReadRenderable(NFILE *pfile, NRENDERABLE *pr);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + BLEND
	// +
	Nu32 NFileWriteBlendValues(NFILE *pfile, const NBLEND *pblend);
	Nu32 NFileReadBlendValues(NFILE *pfile, NBLEND *pblend);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + MATERIAL
	// +
	Nu32 NFileWriteMaterialValues(NFILE *pfile, const NMATERIAL *pmaterial);
	Nu32 NFileReadMaterialValues(NFILE *pfile, NMATERIAL *pmaterial);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + TEXTURE PARAMETERS (Min/Mag filters,  s/t Wraps)
	// +
	Nu32 NFileWriteTextureParameters(NFILE *pfile, const NTEXTURE *ptexture);
	Nu32 NFileReadTextureParameters(NFILE *pfile, NTEXTURE *ptexture);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + APPEARANCE
	// +
	Nu32 NFileWriteAppearance(NFILE *pfile, const NAPPEARANCE *papp, const NAPPEARANCE_FORMAT_ENUM app_format);
	Nu32 NFileReadAppearance(NFILE *pfile, NAPPEARANCE *papp, NAPPEARANCE_FORMAT_ENUM *papp_format);

	Nu32 NFileWriteAppearance_TT(NFILE *pfile, const NAPPEARANCE_TT *papp);
	Nu32 NFileReadAppearance_TT(NFILE *pfile, NAPPEARANCE_TT *papp);

	Nu32 NFileWriteAppearance_TM(NFILE *pfile, const NAPPEARANCE_TM *papp);
	Nu32 NFileReadAppearance_TM(NFILE *pfile, NAPPEARANCE_TM *papp);

	Nu32 NFileWriteAppearance_ULTD_T(NFILE *pfile, const NAPPEARANCE_ULTD_T *papp);
	Nu32 NFileReadAppearance_ULTD_T(NFILE *pfile, NAPPEARANCE_ULTD_T *papp);

	Nu32 NFileWriteAppearance_ULTD_TM(NFILE *pfile, const NAPPEARANCE_ULTD_TM *papp);
	Nu32 NFileReadAppearance_ULTD_TM(NFILE *pfile, NAPPEARANCE_ULTD_TM *papp);

	Nu32 NFileWriteAppearance_Raw(NFILE *pfile, const NAPPEARANCE_RAW *papp);
	Nu32 NFileReadAppearance_Raw(NFILE *pfile, NAPPEARANCE_RAW *papp);

	void NSetCurrentRWAppearanceIDS(const Nu32 ids);
	void NSetCurrentRWAppearance_Texture_ID(const Nu8 id);
	void NSetCurrentRWAppearance_Material_ID(const Nu8 id);
	void NSetCurrentRWAppearance_Blend_ID(const Nu8 id);

	void NBindRWAppearanceTextureCallBacks(const Nu8 id, const NAPPEARANCE_WRITETEXTURE_CALLBACK pfctwrite, const NAPPEARANCE_READTEXTURE_CALLBACK pfctread);
	void NBindRWAppearanceBlendCallBacks(const Nu8 id, const NAPPEARANCE_WRITEBLEND_CALLBACK pfctwrite, const NAPPEARANCE_READBLEND_CALLBACK pfctread);
	void NBindRWAppearanceMaterialCallBacks(const Nu8 id, const NAPPEARANCE_WRITEMATERIAL_CALLBACK pfctwrite, const NAPPEARANCE_READMATERIAL_CALLBACK pfctread);

	// MID LEVEL
	Nu32 NFileWriteAppearanceUVMatrixAnim(NFILE *pfile, const NUVMATRIXANIM *ptr);
	Nu32 NFileReadAppearanceUVMatrixAnim(NFILE *pfile, NUVMATRIXANIM **pptr);

// LOW LEVEL
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NFILEEXT_H
