#include "../../NErrorHandling.h"
#include "lib/N/Geometry/Components/Specifications/NGeometryVertex_Flags.h"

#include "NGeometryVertex.h"


extern NVERTEX_SPECS NVertexSpecs[];

void NGetVertexPosition_3f(NVEC3 *ppos, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_POSITION_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *ppos = *(NVEC3*)pvertex;
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexPosition_4f(NVEC4 *ppos, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_POSITION_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*ppos = *(NVEC4*)pvertex;
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexNormal_3f(NVEC3 *pnormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*pnormal = *(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToNormal);
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
void NGetVertexNormal_3s( ??? *pnormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_NORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*pnormal = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToNormal);
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
void NGetVertexBiNormal_3f( NVEC3 *pbinormal ,const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*pbinormal = *(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBiNormal);
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
void NGetVertexBiNormal_3s(??? *pbinormal, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_BINORMAL_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*pbinormal = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBiNormal);
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}
*/
void NGetVertexTangent_3f( NVEC3 *ptangent, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*ptangent = *(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTangent);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
/*
void NGetVertexTangent_3s(??? *ptangent, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_TANGENT_3s(NVertexSpecs[vertex_format].Flags) )
	{
		*ptangent = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToTangent);
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
void NGetVertexColor0_3f(??? *pcolor_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_3f(NVertexSpecs[vertex_format].Flags) )
	{
		*pcolor_3f = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0);
	}
#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
#endif
}

void NGetVertexColor0_3b(??? *pcolor_3b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_3b(NVertexSpecs[vertex_format].Flags) )
	{
		*pcolor_3b = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor0);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
*/
void NGetVertexColor0_4f(NCOLOR *pcolor4f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*pcolor4f = *(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexColor0_4b(Nu32 *pcolor4b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR0_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *pcolor4b = *(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor0);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
/*
void NGetVertexColor1_3f(??? *pcolor_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_3f(NVertexSpecs[vertex_format].Flags) )
	{
		*pcolor_3f = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor1);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}

void NGetVertexColor1_3b(??? *pcolor_3b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_3b(NVertexSpecs[vertex_format].Flags) )
	{
		*pcolor_3b = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToColor1);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
*/
void NGetVertexColor1_4f(NCOLOR *pcolor4f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *pcolor4f = *(NCOLOR*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexColor1_4b(Nu32 *pcolor4b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_COLOR1_4b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *pcolor4b = *(Nu32*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToColor1);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexTexCoord0_2f(NVEC2 *ptexcoord_2f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *ptexcoord_2f = *(NVEC2*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexTexCoord0_3f(NVEC3 *ptexcoord_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD0_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *ptexcoord_3f = *(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord0);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexTexCoord1_2f(NVEC2 *ptexcoord_2f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_2f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *ptexcoord_2f = *(NVEC2*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexTexCoord1_3f(NVEC3 *ptexcoord_3f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_TEXCOORD1_3f(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *ptexcoord_3f = *(NVEC3*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToTexCoord1);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
/*
TODO
void NGetVertexBone0_1d1f(??? *pbone_1d1f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE0_1d1f(NVertexSpecs[vertex_format].Flags) )
	{
		*pbone_1d1f = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexBone1_1d1f(??? *pbone_1d1f, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE1_1d1f(NVertexSpecs[vertex_format].Flags) )
	{
		 *pbone_1d1f = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
*/
void NGetVertexBone0_1b(NBYTE *pbone_1b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE0_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		*pbone_1b = *(NBYTE*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone0);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexBone1_1b(NBYTE *pbone_1b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE1_1b(NVertexSpecs[vertex_format].DirectAccess.Flags) )
	{
		 *pbone_1b = *(NBYTE*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].DirectAccess.OffsetToBone1);
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
/*
void NGetVertexBone0_2b(??? *pbone_2b, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE0_2b(NVertexSpecs[vertex_format].Flags) )
	{
		*pbone_2b = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexBone1_2b( ??? *pbone_2b, void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format )
{
	if( NCHECK_VERTEX_SPECS_BONE1_2b(NVertexSpecs[vertex_format].Flags) )
	{
		 *pbone_2b = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexBone0_2s(??? *pbone_2s, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE0_2s(NVertexSpecs[vertex_format].Flags) )
	{
		 *pbone_2s = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone0);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}
void NGetVertexBone1_2s(??? *pbone_2s, const void *pvertex, const NVERTEX_FORMAT_ENUM vertex_format)
{
	if( NCHECK_VERTEX_SPECS_BONE1_2s(NVertexSpecs[vertex_format].Flags) )
	{
		 *pbone_2s = *(???*)((NBYTE*)pvertex+NVertexSpecs[vertex_format].OffsetToBone1);
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	}
	#endif
}*/
