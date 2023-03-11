#ifndef __NVERTEXTEXCOORD_BUILD_H
#define __NVERTEXTEXCOORD_BUILD_H
// ***************************************************************************************
// **																					**
// **								NVertexTexCoordBuild.h								**
// **																					**
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NMatrix.h"
#include "lib/N/NScreenRect.h"
#include "lib/N/NScreenPoint.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NRectf32.h"
#include "lib/N/Core/NSafeConversion.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "lib/N/Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_NBUILDBASE_TEXCOORD_TEXCOORDSETID_MAX 1
#define DEFAULT_NBUILDBASE_TEXCOORD_TEXCOORDSETID 0

// Texture Coordinates are common vertex components.
// To build them we need a building Matrix which is the description of the orthonormal (2D or 3D) used to compute Texture coordinates.
// To build this Matrix, all N Engine TexCoords building process use 3 specific things:
//		1/a Oriented Axis Aligned Used Rectangle of Texture
//		2/a Sub- Axis Aligned Rectangle of Texture
//		3/a range of 5 flags set to ON or OFF
// ... through these 2 functions:
//		NGetTexCoordBuilding3DMatrix4x4
//		NGetTexCoordBuilding2DMatrix3x3
//
// 3D Textures coordinates (with W) are not yet 'fully' supported.
// ( Notes: 3D UVW are supported into Vertex Format, and it's possible to work with them here.
// ... But, real 3D UVW are not computed here, in some future days they will ).
//
// FLAGS and MASKS
#define FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32 BIT_0
#define FLAG_NBUILDBASE_TEXCOORD_USEPARAM_SUBAARECTf32 BIT_1
#define FLAG_NBUILDBASE_TEXCOORD_USEPARAM_TEXCOORDSETID BIT_2
#define FLAG_NBUILDBASE_TEXCOORD_FLIP_U BIT_3
#define FLAG_NBUILDBASE_TEXCOORD_FLIP_V BIT_4
#define FLAG_NBUILDBASE_TEXCOORD_FLIP_W BIT_5

#define MASK_NBUILDBASE_TEXCOORD_ALL_FLAGS (0x3F) //( BIT_0 to BIT_5 included )
#define MASK_NBUILDBASE_TEXCOORD_FLIP_UVW (0x38)  //( BIT_3 to BIT_5 included )
#define MASK_NBUILDBASE_TEXCOORD_FLIP_UV (0x18)	  //( BIT_3 to BIT_4 included )

	typedef struct
	{
		Nu16 Flags;
		Nu8 TexCoordSetID;
		Nu8 Available_Nu8;

		NRECTf32 UsedOAARectf32; // Represent an oriented rectangular area of a texture.
		NRECTf32 SubAARectf32;	 // Represent a NON-oriented rectangular sub area of oriented rectangular area.
	} NBUILDBASE_TEXCOORD;
	// NOAARECT_CCW_ENUM contains the 4 possible OAARECT orientations id.
	// ( To work with  'NGetOAARectf32_CCWOrientation' and 'NGetOAARects16_CCWOrientation' functions. )
	typedef enum
	{
		NOAARECT_CCW0 = 0,
		NOAARECT_CCW90,
		NOAARECT_CCW180,
		NOAARECT_CCW270,
		// ----------------------
		NOAARECT_CCW_ENUM_SIZE
	} NOAARECT_CCW_ENUM;

	// ***************************************************************************************
	// **						N LOW LEVEL (Private Internal Use)							**
	// **								 Functions											**
	// ***************************************************************************************

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +-+ Basic NBUILDBASE_TEXCOORD functions
	NBUILDBASE_TEXCOORD *NSetupBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc, const Nu16 flags, const Nu8 txcset_id, const NRECTf32 *pusedoaarectf32, const NRECTf32 *psubaarectf32);
	NBUILDBASE_TEXCOORD *NCreateBuildBaseTexCoord(const Nu16 flags, const Nu8 txcset_id, const NRECTf32 *pusedoaarectf32, const NRECTf32 *psubaarectf32);
	void NClearBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc);
	void NDeleteBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +-+ TexCoords Building from NBUILDBASE_TEXCOORD:
	Nu8 NGetBuildBaseTexCoordSetID(const NBUILDBASE_TEXCOORD *pbbtexcoord);
	NMATRIX *NGetBuildBaseTexCoordMatrix4x4(NMATRIX *pmatrix, const NBUILDBASE_TEXCOORD *pbbtexcoord);
	NMATRIX3x3 *NGetBuildBaseTexCoordMatrix3x3(NMATRIX3x3 *pmatrix3x3, const NBUILDBASE_TEXCOORD *pbbtexcoord);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOW LEVEL FUNCTIONS
	// +
	// +-+ TexCoords Building Matrix3x3
	NMATRIX3x3 *NGetOAARectf32_TexCoords_Building2DMatrix3x3(NMATRIX3x3 *ptm, const NRECTf32 *poaarectf32, const Nu32 ntxcbuild_flip_uv_flags);
	NMATRIX3x3 *NGetSubAARectf32_TexCoords_Building2DMatrix3x3(NMATRIX3x3 *psubmatrix, const NMATRIX3x3 *pmatrix, const NRECTf32 *puvrect);
	// +-+ TexCoords Building Matrix4x4
	NMATRIX *NGetOAARectf32_TexCoords_Building3DMatrix4x4(NMATRIX *ptm, const NRECTf32 *poaarectf32, const Nu32 ntxcbuild_flip_uvw_flags);
	NMATRIX *NGetSubAARectf32_TexCoords_Building3DMatrix4x4(NMATRIX *psubmatrix, const NMATRIX *pmatrix, const NRECTf32 *puvrect);
	// +-+ Oriented Axis Aligned  NRECTf32
	NOAARECT_CCW_ENUM NGetOAARectf32_CCWOrientation(const NRECTf32 *poaarectf32);
	void NGetOAARectf32Orthogonal(NVEC2 *porigin, NVEC2 *piaxis, NVEC2 *pjaxis, const NRECTf32 *poaarectf32, const Nu32 ntxcbuild_flip_uv_flags);
	NVEC2 *NGetOAARectf32Size(NVEC2 *psize, const NRECTf32 *poaarectf32);
	// +-+ Oriented Axis Aligned  NRECTs16
	NOAARECT_CCW_ENUM NGetOAARects16_CCWOrientation(const NRECTs16 *poaarects16);
	void NGetOAARects16Orthogonal(NVEC2s16 *porigin, NVEC2s16 *puaxis, NVEC2s16 *pvaxis, const NRECTs16 *poaarects16, const Nu32 ntxcbuild_flip_uv_flags);
	NVEC2s16 *NGetOAARects16Size(NVEC2s16 *psize, const NRECTs16 *poaarects16);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NVERTEXTEXCOORD_BUILD_H

/*
OLD to be deleted ...
typedef struct
{
NRECTf32		TxUsedOAARectf32;		// Represent an oriented rectangular area of a texture.
NRECTf32		TxSubAARectf32;			// Represent a NON-oriented rectangular sub area of oriented rectangular area.
}NTEXCOORD_BUILD_CONTEXT;

NTEXCOORD_BUILD_CONTEXT*	NSetupTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext, const Nu8 target_texcoord_set_id,const NRECTf32 *ptextureusedOAArectf32,const Nu32 flags_24bits);
NTEXCOORD_BUILD_CONTEXT*	NCreateTexCoordBuildContext(const Nu8 target_texcoord_set_id,const NRECTf32 *ptextureusedOAArectf32,const Nu32 flags_24bits);
void						NClearTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext);
void						NDeleteTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext);
inline void NSetTexCoordBuildContext_Flags(NTEXCOORD_BUILD_CONTEXT *pcontext,const Nu32 flags_24bits){NErrorIf((flags_24bits&0xFF000000)!=0,NERROR_INCONSISTENT_FLAGS);pcontext->Flags = (flags_24bits&0xFFFFFF);}// Only BIT_0 to BIT_23 may to be used. It's not the case !!!
inline void NSetTexCoordBuildContext_TargetTexCoordSet(NTEXCOORD_BUILD_CONTEXT *pcontext, const NVERTEX_DATA_ENUM target_texcoord_set){NErrorIf(target_texcoord_set != NVERTEX_DATA_TEXCOORD0 && target_texcoord_set != NVERTEX_DATA_TEXCOORD1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA ); _SafeNu32ToNu8((Nu32)target_texcoord_set);}
inline void NSetTexCoordBuildContext_TxUsedOAARectf32(NTEXCOORD_BUILD_CONTEXT *pcontext,const NRECTf32 *ptexture_used_OAArectf32){if(ptexture_used_OAArectf32){pcontext->TxUsedOAARectf32 = *ptexture_used_OAArectf32;}else{pcontext->TxUsedOAARectf32.left = pcontext->TxUsedOAARectf32.top = 0.0f;pcontext->TxUsedOAARectf32.right = pcontext->TxUsedOAARectf32.bottom = 1.0f;}}
inline void NSetTexCoordBuildContext_FlipUVW(NTEXCOORD_BUILD_CONTEXT *pcontext,const Nu32 flags_flip_uvw){NErrorIf((flags_flip_uvw&MASK_NTEXCOORDS_BUILD_CONTEXT_FLIP_UVW)!=0,NERROR_INCONSISTENT_FLAGS);pcontext->Flags |= (flags_flip_uvw&MASK_NTEXCOORDS_BUILD_CONTEXT_FLIP_UVW);}// Only BIT_3,BIT_4,BIT_5 may be used. CRASH if it's not the case !!!
inline void NSetTexCoordBuildContext_TargetTexCoordSet(NTEXCOORD_BUILD_CONTEXT *pcontext, const NVERTEX_DATA_ENUM target_texcoord_set){NErrorIf(target_texcoord_set != NVERTEX_DATA_TEXCOORD0 && target_texcoord_set != NVERTEX_DATA_TEXCOORD1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA ); FLAG_ON(pcontext->Flags,FLAG_NTEXCOORDS_BUILD_CONTEXT_USEPARAM_TEXCOORDSETID);pcontext->TexCoordSetID = _SafeNu32ToNu8((Nu32)target_texcoord_set);}
Nbool		NCheckTexCoordBuildContext_TargetTexCoordSet(const NTEXCOORD_BUILD_CONTEXT *pcontext, const NGEOMETRY *pgeom );
inline void NSetTexCoordBuildContext_TxUsedOAARectf32(NTEXCOORD_BUILD_CONTEXT *pcontext,const NRECTf32 *ptexture_used_OAArectf32){NErrorIf(!ptexture_used_OAArectf32,NERROR_NULL_POINTER);pcontext->TxUsedOAARectf32 = *ptexture_used_OAArectf32;}
inline void NSetTexCoordBuildContext_TxSubAARectf32(NTEXCOORD_BUILD_CONTEXT *pcontext,const NRECTf32 *ptexture_SubAArectf32){NErrorIf(!ptexture_SubAArectf32,NERROR_NULL_POINTER);pcontext->TxSubAARectf32 = *ptexture_SubAArectf32;}
*/