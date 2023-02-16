#include "../../NErrorHandling.h"
#include "./Specifications/NGeometryVertex_Flags.h"

#include "NGeometryVertex.h"


extern NVERTEX_SPECS NVertexSpecs[];


// ------------------------------------------------------------------------------------------
// NCopyVertex
// ------------------------------------------------------------------------------------------
// Description :
//	Copy vertex data from a vertex to an another one. 
//	Especially made to 'copy' vertex data from a vertex format to an another one.
//	For 2 vertex with the same format, prefer using 'dst_vertex = src_vertex'
//	Notice that only the vertex data with the same format are going to be copied.
//	for example
//	NCopyVertex(&destination,NVERTEX_FORMAT_V3fC4b,&source,NVERTEX_FORMAT_V3fC4fT2f )
//		Position  is going to be copied from source to destination (3f at both side)
//		Color0	  is going to stay uncopied because color0 source and color0 destination doesn't have the same format !
//		TexCoord0 is going to stay uncopied because TexCoord0 destination doesn't exist !
//	To convert vertex data use 'NConvertVertex' function ... TODO
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//		Nu32	vertex_flags data which describe exactly what data were really copied.
//				0 means no data copied.
// ------------------------------------------------------------------------------------------
void NCopyVertex(void *pdst_vertex, const NVERTEX_FORMAT_ENUM dst_vertex_format, const void *psrc_vertex, const NVERTEX_FORMAT_ENUM src_vertex_format)
{
	// To obtain all the common component between the two vertex
	Nu32 src_flags = NVertexSpecs[src_vertex_format].DirectAccess.Flags;
	Nu32 dst_flags = NVertexSpecs[dst_vertex_format].DirectAccess.Flags;
	
	// Vertex Source and destination Formats are completely different ! ... 
	// So ... Nothing to do !!!
	if(!(src_flags&dst_flags))
	{
		return;
	}
	else if( (dst_vertex_format == src_vertex_format) )
	{
		memcpy(pdst_vertex,psrc_vertex,NVertexSpecs[src_vertex_format].DirectAccess.SizeOfVertex);
	}
	else
	{
		// POSITION
		if( (src_flags&MASK_NVERTEX_SPECS_POSITION) == (dst_flags&MASK_NVERTEX_SPECS_POSITION) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToPosition,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToPosition,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfPosition	);
		}
		// NORMAL
		if( (src_flags&MASK_NVERTEX_SPECS_NORMAL) == (dst_flags&MASK_NVERTEX_SPECS_NORMAL) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToNormal,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToNormal,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfNormal	);
		}
		// BINORMAL
		if( (src_flags&MASK_NVERTEX_SPECS_BINORMAL) == (dst_flags&MASK_NVERTEX_SPECS_BINORMAL) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBiNormal,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBiNormal,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBiNormal	);
		}
		// TANGENT
		if( (src_flags&MASK_NVERTEX_SPECS_TANGENT) == (dst_flags&MASK_NVERTEX_SPECS_TANGENT) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTangent,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTangent,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTangent	);
		}
		// COLOR 0
		if( (src_flags&MASK_NVERTEX_SPECS_COLOR0) == (dst_flags&MASK_NVERTEX_SPECS_COLOR0) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToColor0,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToColor0,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfColor0	);
		}
		// COLOR 1
		if( (src_flags&MASK_NVERTEX_SPECS_COLOR1) == (dst_flags&MASK_NVERTEX_SPECS_COLOR1) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToColor1,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToColor1,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfColor1	);
		}
		// TEXCOORD 0
		if( (src_flags&MASK_NVERTEX_SPECS_TEXCOORD0) == (dst_flags&MASK_NVERTEX_SPECS_TEXCOORD0) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTexCoord0,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTexCoord0,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTexCoord0	);
		}
		// TEXCOORD 1
		if( (src_flags&MASK_NVERTEX_SPECS_TEXCOORD1) == (dst_flags&MASK_NVERTEX_SPECS_TEXCOORD1) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTexCoord1,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTexCoord1,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTexCoord1	);
		}
		// BONE 0
		if( (src_flags&MASK_NVERTEX_SPECS_BONE0) == (dst_flags&MASK_NVERTEX_SPECS_BONE0) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBone0,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBone0,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBone0	);
		}
		// BONE 1
		if( (src_flags&MASK_NVERTEX_SPECS_BONE1) == (dst_flags&MASK_NVERTEX_SPECS_BONE1) )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBone1,
					(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBone1,
											NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBone1	);
		}
	}
}

void NCopyVertexEx(void *pdst_vertex, const NVERTEX_FORMAT_ENUM dst_vertex_format, const void *psrc_vertex, const NVERTEX_FORMAT_ENUM src_vertex_format,const Nu32 vertex_data_mask)
{
	NErrorIf((vertex_data_mask&MASK_NVERTEX_SPECS_DATA)!=vertex_data_mask,NERROR_INCONSISTENT_FLAGS);
	// To obtain all the common component between the two vertex
	Nu32 src_flags = NVertexSpecs[src_vertex_format].DirectAccess.Flags;
	Nu32 dst_flags = NVertexSpecs[dst_vertex_format].DirectAccess.Flags;

	// Vertex Source and destination Formats are completely different ! ... 
	// So ... Nothing to do !!!
	if(!(src_flags&dst_flags))
	{
		return;
	}
	else if( (dst_vertex_format == src_vertex_format) )
	{
		memcpy(pdst_vertex,psrc_vertex,NVertexSpecs[src_vertex_format].DirectAccess.SizeOfVertex);
	}
	else
	{
		// POSITION
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_POSITION) && (src_flags&MASK_NVERTEX_SPECS_POSITION) == (dst_flags&MASK_NVERTEX_SPECS_POSITION)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToPosition,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToPosition,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfPosition	);
		}
		// NORMAL
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_NORMAL) && (src_flags&MASK_NVERTEX_SPECS_NORMAL) == (dst_flags&MASK_NVERTEX_SPECS_NORMAL)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToNormal,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToNormal,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfNormal	);
		}
		// BINORMAL
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_BINORMAL) && (src_flags&MASK_NVERTEX_SPECS_BINORMAL) == (dst_flags&MASK_NVERTEX_SPECS_BINORMAL)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBiNormal,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBiNormal,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBiNormal	);
		}
		// TANGENT
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_TANGENT) && (src_flags&MASK_NVERTEX_SPECS_TANGENT) == (dst_flags&MASK_NVERTEX_SPECS_TANGENT)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTangent,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTangent,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTangent	);
		}
		// COLOR 0
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_COLOR0) && (src_flags&MASK_NVERTEX_SPECS_COLOR0) == (dst_flags&MASK_NVERTEX_SPECS_COLOR0)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToColor0,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToColor0,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfColor0	);
		}
		// COLOR 1
		if((vertex_data_mask&FLAG_NVERTEX_SPECS_COLOR1) && (src_flags&MASK_NVERTEX_SPECS_COLOR1) == (dst_flags&MASK_NVERTEX_SPECS_COLOR1)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToColor1,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToColor1,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfColor1	);
		}
		// TEXCOORD 0
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_TEXCOORD0) && (src_flags&MASK_NVERTEX_SPECS_TEXCOORD0) == (dst_flags&MASK_NVERTEX_SPECS_TEXCOORD0)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTexCoord0,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTexCoord0,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTexCoord0	);
		}
		// TEXCOORD 1
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_TEXCOORD1) && (src_flags&MASK_NVERTEX_SPECS_TEXCOORD1) == (dst_flags&MASK_NVERTEX_SPECS_TEXCOORD1)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToTexCoord1,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToTexCoord1,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfTexCoord1	);
		}
		// BONE 0
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_BONE0) && (src_flags&MASK_NVERTEX_SPECS_BONE0) == (dst_flags&MASK_NVERTEX_SPECS_BONE0)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBone0,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBone0,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBone0	);
		}
		// BONE 1
		if( (vertex_data_mask&FLAG_NVERTEX_SPECS_BONE1) && (src_flags&MASK_NVERTEX_SPECS_BONE1) == (dst_flags&MASK_NVERTEX_SPECS_BONE1)  )
		{
			memcpy(	(NBYTE*)pdst_vertex +	NVertexSpecs[dst_vertex_format].DirectAccess.OffsetToBone1,
				(NBYTE*)psrc_vertex +	NVertexSpecs[src_vertex_format].DirectAccess.OffsetToBone1,
				NVertexSpecs[dst_vertex_format].DirectAccess.SizeOfBone1	);
		}
	}
}