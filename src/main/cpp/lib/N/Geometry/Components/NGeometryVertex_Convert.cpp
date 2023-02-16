#include "../../NErrorHandling.h"
#include "./Specifications/NGeometryVertex_Flags.h"

#include "NGeometryVertex.h"


extern NVERTEX_SPECS NVertexSpecs[];

void NConvertVertexPosition_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC4 *ppos)
{
	if( NCHECK_VERTEX_SPECS_POSITION_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC4*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition) = *ppos;
	}
	else if( NCHECK_VERTEX_SPECS_POSITION_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(!ppos->w,NERROR_NULL_VALUE); // It's a position so w should be different than 0! ( for a direction vector w = 0 but here it's a position not a direction ! )
		if(ppos->w == 1.0f)
		{
			*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition) = *(NVEC3*)ppos;
		}
		else
		{
			((NVEC4*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition))->x = ppos->x/ppos->w;
			((NVEC4*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition))->y = ppos->y/ppos->w;
			((NVEC4*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition))->z = ppos->z/ppos->w;
		}
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexPosition_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ppos)
{
	if( NCHECK_VERTEX_SPECS_POSITION_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition)		= *ppos;
	}
	else if( NCHECK_VERTEX_SPECS_POSITION_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition)		= *ppos;
		((NVEC4*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToPosition))->w	= 1.0f; // it's a position so, w = 1.0f
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}

void NConvertVertexNormal_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pnormal)
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToNormal) = *pnormal;
	}
	else if( NCHECK_VERTEX_SPECS_NORMAL_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexBiNormal_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *pbinormal)
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBiNormal) = *pbinormal;
	}
	else if( NCHECK_VERTEX_SPECS_BINORMAL_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexTangent_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NVEC3 *ptangent)
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTangent) = *ptangent;
	}
	else if( NCHECK_VERTEX_SPECS_TANGENT_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexNormal_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pnormal)
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_NORMAL_3f(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexBiNormal_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pbinormal)
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_BINORMAL_3f(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexTangent_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *ptangent)
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3s(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_TANGENT_3f(NVertexSpecs[vertex_format].Flags) )
	{
		NError(NERROR_SYSTEM_TODO);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
*/

// -----------------------------------------------------------------------------------------------------------------------------------------
// void NConvertVertexColor...
// -----------------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set a vertex color with incoming color. 
//	Whatever the vertex data format is, N is going to convert incoming data to the vertex data format.
// -----------------------------------------------------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor0_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0) = *pcolor4f;
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NCOLOR_To_RGBA8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0),pcolor4f);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NCOLOR_To_RGB8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0),pcolor4f);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor0_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 *pcolor4b)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0) = *pcolor4b;
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		RGBA8_To_NCOLOR((NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0),*pcolor4b);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR0_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO); // don't do Nu32_3b = Nu32_4b, and protect the 4th NBYTE of Nu32_3b
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor0_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor0_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor1_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NCOLOR *pcolor4f)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1) = *pcolor4f;
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NCOLOR_To_RGBA8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1),pcolor4f);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NCOLOR_To_RGB8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1),pcolor4f);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor1_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu32 *pcolor4b)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1) = *pcolor4b;
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		RGBA8_To_NCOLOR((NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1),*pcolor4b);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_COLOR1_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO); // don't do Nu32_3b = Nu32_4b, and protect the 4th NBYTE of Nu32_3b
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor1_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor1_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? *pcolor3b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor_4f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const NCOLOR *pcolor4f)
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
		else if(NCHECK_VERTEX_SPECS_COLORS_TYPE_3f(NVertexSpecs[vertex_format].DirectAccess.Flags))
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
			return;
		}
		else if( NCHECK_VERTEX_SPECS_COLORS_TYPE_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NCOLOR_To_RGBA8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]),pcolor4f);
			return;
		}
		else if( NCHECK_VERTEX_SPECS_COLORS_TYPE_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NCOLOR_To_RGB8((Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]),pcolor4f);
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor_4b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const Nu32 *pcolor4b)
{
	NErrorIf(colorset_id >= NVERTEX_DATA_COLOR_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// Colorset_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_COLOR0+colorset_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{

		if( NCHECK_VERTEX_SPECS_COLORS_TYPE_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]) = *pcolor4b;
			return;
		}
		else if( NCHECK_VERTEX_SPECS_COLORS_TYPE_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			RGBA8_To_NCOLOR((NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_COLOR0+colorset_id]),*pcolor4b);
			return;
		}
		else if( NCHECK_VERTEX_SPECS_COLORS_TYPE_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
			return;
		}
		else if( NCHECK_VERTEX_SPECS_COLORS_TYPE_3b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO); // don't do Nu32_3b = Nu32_4b, and protect the 4th NBYTE of Nu32_3b
			return;
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor3f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexColor_3b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 colorset_id, const ??? *pcolor3b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/
// -----------------------------------------------------------------------------------------------------------------------------------------
// void NConvertVertexTexCoord...
// -----------------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set a vertex Texture Coordinate with incoming Tex Coord. 
//	Whatever the vertex data format is, N is going to convert incoming data to the vertex data format.
// -----------------------------------------------------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexTexCoord_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2f *ptexcoord_2f)
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
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id])		= *ptexcoord_2f;
			((NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]))->z = 1.0f; // Texture coordinate is a position
			return;
		}
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexTexCoord_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3f *ptexcoord_3f)
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
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(!ptexcoord_3f->z,NERROR_NULL_VALUE); // It's a 2D position so z should be different than 0! ( for a 2D direction vector z = 0 but here it's a position not a direction ! )
			if(ptexcoord_3f->z == 1.0f)
			{
				*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]) = *(NTEXCOORD_2f*)ptexcoord_3f;
			}
			else
			{
				((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]))->x = ptexcoord_3f->x/ptexcoord_3f->z;
				((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0+texcoordset_id]))->y = ptexcoord_3f->y/ptexcoord_3f->z;
			}
			return;
		}
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
		else if( NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
void NConvertVertexTexCoord_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_2s *ptexcoord_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexTexCoord_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 texcoordset_id, const NTEXCOORD_3s *ptexcoord_3s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/
void NConvertVertexTexCoord0_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0) = *ptexcoord_2f;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0)		= *ptexcoord_2f;
		((NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0))->z	= 1.0f; // Texture coordinate is a position
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NConvertVertexTexCoord0_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0) = *ptexcoord_3f;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(!ptexcoord_3f->z,NERROR_NULL_VALUE); // It's a 2D position so z should be different than 0! ( for a 2D direction vector z = 0 but here it's a position not a direction ! )
		if(ptexcoord_3f->z == 1.0f)
		{
			*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0) = *(NTEXCOORD_2f*)ptexcoord_3f;
		}
		else
		{
			((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0))->x = ptexcoord_3f->x/ptexcoord_3f->z;
			((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0))->y = ptexcoord_3f->y/ptexcoord_3f->z;
		}
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD0_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
void NConvertVertexTexCoord0_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexTexCoord0_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/
void NConvertVertexTexCoord1_2f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2f *ptexcoord_2f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1) = *ptexcoord_2f;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1)		= *ptexcoord_2f;
		((NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1))->z	= 1.0f; // Texture coordinate is a position
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
void NConvertVertexTexCoord1_3f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3f *ptexcoord_3f)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*(NTEXCOORD_3f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1) = *ptexcoord_3f;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(!ptexcoord_3f->z,NERROR_NULL_VALUE); // It's a 2D position so z should be different than 0! ( for a 2D direction vector z = 0 but here it's a position not a direction ! )
		if(ptexcoord_3f->z == 1.0f)
		{
			*(NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1) = *(NTEXCOORD_2f*)ptexcoord_3f;
		}
		else
		{
			((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1))->x = ptexcoord_3f->x/ptexcoord_3f->z;
			((NTEXCOORD_2f*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1))->y = ptexcoord_3f->y/ptexcoord_3f->z;
		}
		return;
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_3s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_TEXCOORD1_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
/*
void NConvertVertexTexCoord1_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_2s *ptexcoord_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexTexCoord1_3s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NTEXCOORD_3s *ptexcoord_3s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
*/

// -----------------------------------------------------------------------------------------------------------------------------------------
// void NConvertVertexBone...
// -----------------------------------------------------------------------------------------------------------------------------------------
// Description :
//	Set a vertex Bone Id with incoming Bone Id. 
//	Whatever the vertex data format is, N is going to convert incoming data to the vertex data format.
// -----------------------------------------------------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// -----------------------------------------------------------------------------------------------------------------------------------------
void NConvertVertexBone_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const NBYTE bone_1b)
{
	NErrorIf(bone_id >= NVERTEX_DATA_BONE_NUMBER_MAX-1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	// bone_id is 0 or 1 that's it !
	// Check if the associated flag is set to ON in the vertex format... if not, CRASH in debug mode, just leave in Release.
	if( (NVertexSpecs[vertex_format].DirectAccess.Flags)&(1<<(NVERTEX_DATA_BONE0+bone_id)) ) // FLAG and Data position follows the same order -> NVERTEX_DATA_COLOR0 = 4 AND FLAG_NVERTEX_SPECS_COLOR0 = BIT_4 !
	{
		if( NCHECK_VERTEX_SPECS_BONES_TYPE_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			*((NBYTE*)pvertex+NVertexSpecs[vertex_format].TableAccess.Offset[NVERTEX_DATA_BONE0+bone_id]) = bone_1b;
			return;
		}
		else if( NCHECK_VERTEX_SPECS_BONES_TYPE_2b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
		else if( NCHECK_VERTEX_SPECS_BONES_TYPE_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
		else if( NCHECK_VERTEX_SPECS_BONES_TYPE_1d1f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
		{
			NErrorIf(1,NERROR_SYSTEM_TODO);
		}
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
void NConvertVertexBone_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const Nu8 bone_id, const ??? bone_1d1f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}*/
void NConvertVertexBone0_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b)
{
	if( NCHECK_VERTEX_SPECS_BONE0_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone0) = bone_1b;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_BONE0_2b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_BONE0_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_BONE0_1d1f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
void NConvertVertexBone0_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone0_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone0_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_1d1f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}*/
void NConvertVertexBone1_1b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const NBYTE bone_1b)
{
	if( NCHECK_VERTEX_SPECS_BONE1_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone1) = bone_1b;
		return;
	}
	else if( NCHECK_VERTEX_SPECS_BONE1_2b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_BONE1_2s(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	else if( NCHECK_VERTEX_SPECS_BONE1_1d1f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
	}
	NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
}
/*
void NConvertVertexBone0_2b_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2b)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone0_2s_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_2s)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}
void NConvertVertexBone0_1d1f_ToVertexDataFormat(void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format, const ??? bone_1d1f)
{
	NErrorIf(1,NERROR_SYSTEM_TODO);
}*/