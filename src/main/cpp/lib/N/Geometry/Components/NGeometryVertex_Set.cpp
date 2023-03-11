#include "../../NErrorHandling.h"
#include "lib/N/Geometry/Components/Specifications/NGeometryVertex_Flags.h"

#include "NGeometryVertex.h"


extern NVERTEX_SPECS NVertexSpecs[];


// ------------------------------------------------------------------------------------------
// void NSetVertexPosition_3f
// ------------------------------------------------------------------------------------------
// Description : set a Vertex position of an non-casted vertex pointer.
// ------------------------------------------------------------------------------------------
// In	:
//			void							*pvertex			pointer on the vertex structure to set.
//			const NVERTEX_FORMAT_ENUM		vertex_format		Format of the vertex structure. 
//			const NVEC3					*ppos				position data to copy into the vertex structure.
//			
// Out :
// ------------------------------------------------------------------------------------------
void NSetVertexPosition_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ppos)
{
	if( NCHECK_VERTEX_SPECS_POSITION_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)pvertex = *ppos;
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexPosition_4f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexPosition_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC4 *ppos)
{
	if( NCHECK_VERTEX_SPECS_POSITION_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC4*)pvertex = *ppos;
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexNormal_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexNormal_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pnormal)
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToNormal) = *pnormal;
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
/*
... TODO
void NSetVertexNormal_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pnormal )
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToNormal) = *pnormal;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexBiNormal_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexBiNormal_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pbinormal)
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBiNormal) = *pbinormal;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
... TODO
void NSetVertexBiNormal_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const??? *pbinormal)
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBiNormal) = *pbinormal;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexTangent_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTangent_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ptangent)
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTangent) = *ptangent;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
void NSetVertexTangent_3s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *ptangent)
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToTangent) = *ptangent;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
/*
void NSetVertexColor0_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3f)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_3f(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0) = *pcolor_3f;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}

void NSetVertexColor0_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3b)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_3b(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0) = *pcolor_3b;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexColor0_4f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor0_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0) = *pcolor4f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexColor0_4b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor0_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 color4b)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0) = color4b;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
void NSetVertexColor1_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3f)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_3f(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor1) = *pcolor_3f;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}

void NSetVertexColor1_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor_3b)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_3b(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor1) = *pcolor_3b;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexColor1_4f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor1_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1) = *pcolor4f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexColor1_4b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor1_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 color4b)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1) = color4b;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}

/*
void NSetVertexColor_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor_3f)
{
	NErrorIf(colorset_id >= NVERTEX_DATA_COLOR_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// Colorset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].Flags)&(1<<(NVERTEX_DATA_COLOR0+colorset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_COLORS_TYPE_3f(NVertexSpecs[vertex_format].Flags) )
		{
			*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0) = *pcolor_3f;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}

void NSetVertexColor_3b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor_3b)
{
	NErrorIf(colorset_id >= NVERTEX_DATA_COLOR_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// Colorset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].Flags)&(1<<(NVERTEX_DATA_COLOR0+colorset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_COLORS_TYPE_3b(NVertexSpecs[vertex_format].Flags) )
		{
			*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0) = *pcolor_3b;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexColor_4f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor_4f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const NCOLOR *pcolor4f)
{
	NErrorIf(colorset_id >= NVERTEX_DATA_COLOR_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// Colorset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_COLOR0+colorset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if(NCHECK_VERTEX_SPECS_COLORS_TYPE_4f(NVertexSpecs[vertex_format].DirectAccess.Flags))
		{
			*(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]) = *pcolor4f;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
// ------------------------------------------------------------------------------------------
// void NSetVertexColor_4b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexColor_4b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const Nu32 color4b)
{
	NErrorIf(colorset_id >= NVERTEX_DATA_COLOR_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// Colorset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_COLOR0+colorset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_COLORS_TYPE_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]) = color4b;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord0_2f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord0_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC2*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0) = *ptexcoord_2f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord0_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord0_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0) = *ptexcoord_3f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord1_2f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord1_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC2*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1) = *ptexcoord_2f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord1_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord1_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1) = *ptexcoord_3f;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord_2f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord_2f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2f *ptexcoord_2f)
{
	NErrorIf(texcoordset_id >= NVERTEX_DATA_TEXCOORD_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// texcoordset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_TEXCOORD0+texcoordset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]) = *ptexcoord_2f;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
// ------------------------------------------------------------------------------------------
// void NSetVertexTexCoord_3f
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexTexCoord_3f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3f *ptexcoord_3f)
{
	NErrorIf(texcoordset_id >= NVERTEX_DATA_TEXCOORD_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// texcoordset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_TEXCOORD0+texcoordset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]) = *ptexcoord_3f;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}



/*
TODO
void NSetVertexBone0_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbone_1d1f)
{
	if( NCHECK_VERTEX_SPECS_BONE0_1d1f(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0) = pbone_1d1f;
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NSetVertexBone1_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbone_1d1f)
{
	if( NCHECK_VERTEX_SPECS_BONE1_1d1f(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1) = pbone_1d1f;
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NSetVertexBone0_1b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexBone0_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b)
{
	if( NCHECK_VERTEX_SPECS_BONE0_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NBYTE*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone0) = bone_1b;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexBone1_1b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexBone1_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b)
{
	if( NCHECK_VERTEX_SPECS_BONE1_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NBYTE*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone1) = bone_1b;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// ------------------------------------------------------------------------------------------
// void NSetVertexBone_1b
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetVertexBone_1b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const NBYTE bone_1b)
{
	NErrorIf(bone_id >= NVERTEX_DATA_BONE_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// bone_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_BONE0+bone_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_BONES_TYPE_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(NBYTE*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_BONE0+bone_id]) = bone_1b;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
void NSetVertexBone0_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b)
{
	if( NCHECK_VERTEX_SPECS_BONE0_2b(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0) = bone_2b;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NSetVertexBone1_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b)
{
	if( NCHECK_VERTEX_SPECS_BONE1_2b(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1) = bone_2b;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NSetVertexBone0_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s)
{
	if( NCHECK_VERTEX_SPECS_BONE0_2s(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0) = bone_2s;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NSetVertexBone1_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s)
{
	if( NCHECK_VERTEX_SPECS_BONE1_2s(NVertexSpecs[vertex_format].Flags) )
	{
		*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1) = bone_2s;
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NSetVertexBone_1d1f(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_1d1f)
{
	NErrorIf(bone_id >= NVERTEX_DATA_BONE_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// bone_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].Flags)&(1<<(NVERTEX_DATA_BONE0+bone_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_BONES_TYPE_1d1f(NVertexSpecs[vertex_format].Flags) )
		{
			*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].Offset[NVERTEX_DATA_BONE0+bone_id]) = bone_1d1f;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}

void NSetVertexBone_2s(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2s)
{
	NErrorIf(bone_id >= NVERTEX_DATA_BONE_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// bone_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].Flags)&(1<<(NVERTEX_DATA_BONE0+bone_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_BONES_TYPE_2s(NVertexSpecs[vertex_format].Flags) )
		{
			*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].Offset[NVERTEX_DATA_BONE0+bone_id]) = bone_2s;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}

void NSetVertexBone_2b(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2b)
{
	NErrorIf(bone_id >= NVERTEX_DATA_BONE_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// bone_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].Flags)&(1<<(NVERTEX_DATA_BONE0+bone_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_BONES_TYPE_2b(NVertexSpecs[vertex_format].Flags) )
		{
			*(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].Offset[NVERTEX_DATA_BONE0+bone_id]) = bone_2b;
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
*/