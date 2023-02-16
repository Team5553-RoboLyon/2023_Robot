#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NArray.h"
#include "../NErrorHandling.h"
#include "NGeometry.h"
#include "NGeometryMisc.h"


// ------------------------------------------------------------------------------------------
// Nu32 NComputeTriangleNormal
// ------------------------------------------------------------------------------------------
// Description :
//	Compute Triangle normal vector 3.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//			1 if a normal was calculated in a right way
//			0 if calculated normal is the NULL vector.
//
// ------------------------------------------------------------------------------------------
Nu32 NComputeTriangleNormal(NVEC3 *pnormal, const NVEC3 *p0,const NVEC3 *p1,const NVEC3 *p2 )
{
	NErrorIf(!pnormal ||!p0||!p1||!p2,NERROR_NULL_POINTER);
	NVEC3 v0,v1;

	NVec3Sub(&v0,p1,p0);
	NVec3Sub(&v1,p2,p0);
	NVec3CrossProduct(pnormal,&v0,&v1);
	Nf32 l = NVec3Length(pnormal);
	if( l<= NF32_EPSILON_VECTOR_LENGTH )
	{
		NVec3Set(pnormal,0,0,0);
		return 0;
	//	NErrorIf(1,NERROR_PRIMITIVE_DEGENERATED_TRIANGLE); // NOT REALLY A BUG IF IT HAPPENS ... FIND A WAY TO FIX THE PROBLEM AND REMOVE THIS ERROR TEST.
	}
	else
	{
		// Normalize ...
		NVec3ScaleBy(pnormal,1.0f/l);
		return 1;
	}
}

// ------------------------------------------------------------------------------------------
// NSetGeometryQuadUVs
// ------------------------------------------------------------------------------------------
// Description :
//		Convert an NRECTf32 uvrect description (for instance, generated from an ATLAS element.)
//		into 4 successive Vertex UVs. It's perfect to get Atlas Element UV Rect and set the 4 UV's
//		of the geometry Quad associated with.
// ------------------------------------------------------------------------------------------
// In	:
//			NGEOMETRY					*pgeom					The geometry where the 4 UVs to update are in.
//			const Nu32					first_quadvertex_index	ID of the first of the 4 Vertex to update uvs.
//			const NVERTEX_DATA_ENUM		texcoord_addr			should be	NVERTEX_ADDRESS_TEXCOORD0
//																		or	NVERTEX_ADDRESS_TEXCOORD1 
//			const NRECTf32				*pUVrect				The NRECTf32 struct where the Rectangular UV area is described.
//																(... often from ATLAS ELEMENT). !!! could be 'AA' or 'OAA' rect !!!
// Out :
//
// ------------------------------------------------------------------------------------------
void NSetGeometryQuadUVs( NGEOMETRY *pgeom, const Nu32 first_quadvertex_index,const NVERTEX_DATA_ENUM vertex_data_texcoordset, const NRECTf32 *pUVrect, const Nbool cw_building )
{
#ifdef _DEBUG
	NErrorIf(vertex_data_texcoordset != NVERTEX_DATA_TEXCOORD0 && vertex_data_texcoordset != NVERTEX_DATA_TEXCOORD1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	NErrorIf(first_quadvertex_index		>= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_quadvertex_index + 3 >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID); // 4th vertex Index
	if( vertex_data_texcoordset == NVERTEX_DATA_TEXCOORD0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_TEXCOORD0_2f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_TEXCOORD0_2f_NOT_SUPPORTED );
	if( vertex_data_texcoordset == NVERTEX_DATA_TEXCOORD1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_TEXCOORD1_2f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_TEXCOORD1_2f_NOT_SUPPORTED );
#endif
	
	// Get First UV Address
	NTEXCOORD_2f *puv = (NTEXCOORD_2f*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_quadvertex_index) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_texcoordset]);

	// CLOCKWISE
	if(cw_building)
	{
		puv->x = pUVrect->upper_left.x;
		puv->y = pUVrect->upper_left.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->lower_right.x;
		puv->y = pUVrect->upper_left.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->lower_right.x;
		puv->y = pUVrect->lower_right.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->upper_left.x;
		puv->y = pUVrect->lower_right.y;
	}
	// COUNTERCLOCKWISE (default)
	else
	{
		puv->x = pUVrect->upper_left.x;
		puv->y = pUVrect->upper_left.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->upper_left.x;
		puv->y = pUVrect->lower_right.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->lower_right.x;
		puv->y = pUVrect->lower_right.y;

		puv = (NTEXCOORD_2f*)((NBYTE*)puv+pgeom->Mesh.VertexArray.ElementSize);
		puv->x = pUVrect->lower_right.x;
		puv->y = pUVrect->upper_left.y;
	}
}

/*
void NSetGeometryVertexColor(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 vertex_id,const NCOLOR *pcolor)
{
	switch(vertex_data_colorset)
	{
		case NVERTEX_DATA_COLOR0:
			NSetVertexColor0_4f(pgeom->Mesh.VertexArray.pFirst + vertex_id*pgeom->Mesh.VertexArray.ElementSize,NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),pcolor);
			break;

		case NVERTEX_DATA_COLOR1:
			NSetVertexColor1_4f(pgeom->Mesh.VertexArray.pFirst + vertex_id*pgeom->Mesh.VertexArray.ElementSize, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),pcolor);
			break;
	}
}*/
void NModulateGeometryVertexRangeColor(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pmodcolor)
{
#ifdef _DEBUG
	NErrorIf(!pmodcolor, NERROR_NULL_POINTER);
	NErrorIf(vertex_data_colorset != NVERTEX_DATA_COLOR0 && vertex_data_colorset != NVERTEX_DATA_COLOR1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	if( vertex_data_colorset == NVERTEX_DATA_COLOR0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED );
	if( vertex_data_colorset == NVERTEX_DATA_COLOR1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR1_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED );
	NErrorIf(first_vertex >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_vertex + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
#endif
	// Get First Color Address
	NCOLOR *pvertex_col = (NCOLOR*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_vertex) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_colorset]);
	Nu32 stride			= pgeom->Mesh.VertexArray.ElementSize;
	for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
	{
		pvertex_col->red	= NMIN(1.0f,pvertex_col->red*pmodcolor->red);
		pvertex_col->green	= NMIN(1.0f,pvertex_col->green*pmodcolor->green);
		pvertex_col->blue	= NMIN(1.0f,pvertex_col->blue*pmodcolor->blue);
		pvertex_col->alpha	= NMIN(1.0f,pvertex_col->alpha*pmodcolor->alpha);
	}
}

void NModulateGeometryVertexRangeAlpha(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const Nf32 modalpha)
{
#ifdef _DEBUG
	NErrorIf(vertex_data_colorset != NVERTEX_DATA_COLOR0 && vertex_data_colorset != NVERTEX_DATA_COLOR1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	if( vertex_data_colorset == NVERTEX_DATA_COLOR0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED );
	if( vertex_data_colorset == NVERTEX_DATA_COLOR1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR1_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED );
	NErrorIf(first_vertex >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_vertex + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
#endif
	// Get First Color Address
	NCOLOR *pvertex_col = (NCOLOR*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_vertex) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_colorset]);
	Nu32 stride			= pgeom->Mesh.VertexArray.ElementSize;
	for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
	{
		pvertex_col->alpha	= NMIN(1.0f,pvertex_col->alpha*modalpha);
	}
}

void NSetGeometryVertexRangeColor(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pcolor)
{
#ifdef _DEBUG
	NErrorIf(!pcolor, NERROR_NULL_POINTER);
	NErrorIf(vertex_data_colorset != NVERTEX_DATA_COLOR0 && vertex_data_colorset != NVERTEX_DATA_COLOR1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	if( vertex_data_colorset == NVERTEX_DATA_COLOR0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED );
	if( vertex_data_colorset == NVERTEX_DATA_COLOR1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR1_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED );
	NErrorIf(first_vertex >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_vertex + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
#endif
	// Get First Color Address
	NCOLOR *pvertex_col = (NCOLOR*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_vertex) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_colorset]);
	Nu32 stride			= pgeom->Mesh.VertexArray.ElementSize;
	for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
	{
		*pvertex_col = *pcolor;
	}
}
void NSetGeometryVertexRangeColorf(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a)
{
#ifdef _DEBUG
	NErrorIf(vertex_data_colorset != NVERTEX_DATA_COLOR0 && vertex_data_colorset != NVERTEX_DATA_COLOR1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	if( vertex_data_colorset == NVERTEX_DATA_COLOR0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED );
	if( vertex_data_colorset == NVERTEX_DATA_COLOR1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR1_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED );
	NErrorIf(first_vertex >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_vertex + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
#endif

	// Get First Color Address
	NCOLOR *pvertex_col = (NCOLOR*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_vertex) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_colorset]);
	Nu32 stride			= pgeom->Mesh.VertexArray.ElementSize;
	for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
	{
		pvertex_col->red	= r;
		pvertex_col->green	= g;
		pvertex_col->blue	= b;
		pvertex_col->alpha	= a;
	}
}
void NSetGeometryVertexRangeColorRange(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pcolorange, const Nu32 colorange_size )
{
	#ifdef _DEBUG
	NErrorIf(!pcolorange, NERROR_NULL_POINTER);
	NErrorIf(!colorange_size, NERROR_NULL_VALUE);
	NErrorIf(vertex_data_colorset != NVERTEX_DATA_COLOR0 && vertex_data_colorset != NVERTEX_DATA_COLOR1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	if( vertex_data_colorset == NVERTEX_DATA_COLOR0 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED );
	if( vertex_data_colorset == NVERTEX_DATA_COLOR1 )
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR1_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->DirectAccess.Flags),NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED );
	NErrorIf(first_vertex >= pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	NErrorIf(first_vertex + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_GEOMETRY_OUTOFRANGE_VERTEXID);
	#endif

	// Get First Color Address
	NCOLOR *pvertex_col = (NCOLOR*)(NGetArrayPtr(&pgeom->Mesh.VertexArray,first_vertex) + NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->TableAccess.Offset[vertex_data_colorset]);
	Nu32 stride			= pgeom->Mesh.VertexArray.ElementSize;

	if(colorange_size == 1)
	{
		for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
		{
			*pvertex_col = *(NCOLOR*)pcolorange;
		}
	}
	else
	{
		NCOLOR *psrccol_afterlast	= ((NCOLOR*)pcolorange) + colorange_size;  // The last addr is: psrccol + color_number -1 
		NCOLOR *psrccol				= psrccol_afterlast;					// Like this it will be set to pfirstcolor at the first loop
		for( Nu32 i=vertex_number;i!=0;i--, pvertex_col = (NCOLOR *)(((NBYTE*)pvertex_col)+stride ) )
		{
			if(psrccol == psrccol_afterlast)
			{
				psrccol = (NCOLOR*)pcolorange;
			}
			else
			{
				psrccol ++;
			}
			*pvertex_col = *psrccol;
		}
	}
}

