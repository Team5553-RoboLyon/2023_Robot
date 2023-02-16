#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../NRectf32.h"
#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "../Miscellaneous/N2DPointInside.h"
#include "../NCoordinateSystemConversions.h"

//#include "NUT_Draw.h"
#include "NUT_3DMouse.h"
#include "NUT_Pick.h"
#include "NUT_X.h"
#include "NUT_2DAAColorCage.h"

extern NVERTEX_SPECS NVertexSpecs[];

// ------------------------------------------------------------------------------------------
// NUT_SetUp2DAAColorCage
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
NUT_2DAACOLORCAGE *	NUT_SetUp_2DAAColorCage(NUT_2DAACOLORCAGE *pcc, const NUT_2DAACOLORCAGE_DESC *paacc_desc)
{
	Nu32		a;
	Nu32		capacity;
//	Nu32		idcol = 0;
	NBYTE		*ptr;
	NCOLOR		col;

	NErrorIf(!paacc_desc->SegmentNb[_I] || !paacc_desc->SegmentNb[_J], NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf( ISFLAG_ON(paacc_desc->Flags,FLAG_NUT_2DAACOLORCAGE_DESC_USEPARAMS_INITIAL_COLOR4F) &&
			  ( paacc_desc->InitialColorNumber == 0 || paacc_desc->InitialColorStride < sizeof(Nu32) || paacc_desc->pInitialColor == NULL ),
			  NERROR_INCONSISTENT_PARAMETERS );

	Nmem0(pcc,NUT_2DAACOLORCAGE);
	switch(paacc_desc->ConstructionPlane)
	{
		case _PLANE_XY:
			NVec3Set(&pcc->PlaneDir,0,0,1);
			pcc->Is_X = _X;
			pcc->Is_Y = _Y;
			break;
		case _PLANE_XZ:
			NVec3Set(&pcc->PlaneDir,0,1,0);
			pcc->Is_X = _X;
			pcc->Is_Y = _Z;
			break;
		case _PLANE_ZY:
			NVec3Set(&pcc->PlaneDir,1,0,0);
			pcc->Is_X = _Z;
			pcc->Is_Y = _Y;
			break;
		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			break;
	}

	pcc->Origin				= paacc_desc->Origin;
	pcc->Size				= paacc_desc->Size;
	pcc->SegmentNb[_I]		= paacc_desc->SegmentNb[_I];
	pcc->SegmentNb[_J]		= paacc_desc->SegmentNb[_J];

	// Allocate the right Color capacity no more no less
	capacity = (pcc->SegmentNb[_I]+1)*(pcc->SegmentNb[_J]+1);
	NSetupArray(&pcc->ColorArray,capacity,sizeof(NCOLOR));



	// Color Initialization Loop
	NSetColorf(&col, DEFAULT_NUT_2DAACOLORCAGE_INITIAL_COLOR4F);
	if( ISFLAG_OFF(paacc_desc->Flags,FLAG_NUT_2DAACOLORCAGE_DESC_USEPARAMS_INITIAL_COLOR4F) )
	{
		NResizeArray(&pcc->ColorArray,capacity,(NBYTE*)&col,NULL);
	}
	else
	{
		for(a=0;a<capacity;a++)
		{
			ptr	= (NBYTE*)paacc_desc->pInitialColor + (a%paacc_desc->InitialColorNumber)*paacc_desc->InitialColorStride;
			NColorConverter(&col,ptr,paacc_desc->InitialColorFormat);
			NArrayPushBack(&pcc->ColorArray,(NBYTE*)&col);				
		}
	}


	return pcc;
}

// ------------------------------------------------------------------------------------------
// NUT_Create_2DAAColorCage
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
NUT_2DAACOLORCAGE *	NUT_Create_2DAAColorCage(const NUT_2DAACOLORCAGE_DESC *paacc_desc)
{
	return NUT_SetUp_2DAAColorCage(NEW(NUT_2DAACOLORCAGE),paacc_desc);
}

// ------------------------------------------------------------------------------------------
// NUT_CLear_2DAAColorCage
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
void NUT_CLear_2DAAColorCage(void *ptr)
{
	NUT_2DAACOLORCAGE * pcc = (NUT_2DAACOLORCAGE*)ptr;

	NClearArray(&pcc->ColorArray,NULL);
	Nmem0(pcc,NUT_2DAACOLORCAGE);
}

// ------------------------------------------------------------------------------------------
// NUT_Delete_2DAAColorCage
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
void NUT_Delete_2DAAColorCage(NUT_2DAACOLORCAGE * pcc)
{
	NUT_CLear_2DAAColorCage(pcc);
	Nfree(pcc);
}

// ------------------------------------------------------------------------------------------
// NUT_Pick_2DAAColorCage_Knot
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
Nu32	NUT_Pick_2DAAColorCage_Knot(const NUT_2DAACOLORCAGE *pcc, const Nf32 pickradius, const NVEC2s16 *ptouchpos)
{
	NVEC3		vorigin;
	NVEC3		vdir;
	NVEC3		v0,v1;
	NVEC3		vsubi,vsubj;		
	Nu32			i,j;
	Nf32			squarepickradius = pickradius*pickradius;
	Nu32			idk;

	NUT_Get3DMouseRay(&vorigin,&vdir,NULL,ptouchpos);
	NVec3Normalize(&vdir);

	NVec3Set(&vsubi,0,0,0);
	NVec3Set(&vsubj,0,0,0);
	vsubi.coord[pcc->Is_X] = pcc->Size.x/(Nf32)pcc->SegmentNb[_I];
	vsubj.coord[pcc->Is_Y] = pcc->Size.y/(Nf32)pcc->SegmentNb[_J];

	v1 = v0 = pcc->Origin;
	for(j=0;j<(Nu32)pcc->SegmentNb[_J]+1;j++,NVec3AddTo(&v0,&vsubj),v1=v0)
	{
		for(i=0;i<(Nu32)pcc->SegmentNb[_I]+1;i++,NVec3AddTo(&v1,&vsubi))
		{
			//if( NUT_Pick3DPointEx_SquareDist(&vorigin,&vdir,squarepickradius,&v1) >= 0)
			if( NUT_LinePointShortestSquareDistance(&vorigin,&vdir,&v1) <= squarepickradius)
			{
				idk = (j*((Nu32)pcc->SegmentNb[_I]+1)+i);
				//NCOLOR *pcol = (NCOLOR*)NGetFirstArrayPtr(&pcc->ColorArray); 
				//printf("\n COLOR CAGE I = %d, J = %d, IDK = %d-- R:%.3f V:%.3f B:%.3f A:%.3f",i,j,idk,pcol[idk].red,pcol[idk].green,pcol[idk].blue,pcol[idk].alpha);
				return idk;
			}
		}
	}
	return NVOID;

/*
	NUT_2DAACOLORCAGE	*pcc	= pshape->ColorCage.pColorCage;
	NCOLOR				*pcol	= (NCOLOR*)&pshape->ColorCage.Color;
	Nf32				ksize	= pshape->ColorCage.KnotSize;

	Nu32				i,j;
	NVEC3			v0,v1;
	NVEC3			vi,vj;
	NVEC3			vsubi,vsubj;


	NVec3Set(&vi,0,0,0);
	NVec3Set(&vj,0,0,0);
	vi.coord[pcc->Is_X] = pcc->Size.x;
	vj.coord[pcc->Is_Y] = pcc->Size.y;

	NVec3Set(&vsubi,0,0,0);
	NVec3Set(&vsubj,0,0,0);
	vsubi.coord[pcc->Is_X] = pcc->Size.x/(Nf32)pcc->SegmentNb[_I];
	vsubj.coord[pcc->Is_Y] = pcc->Size.y/(Nf32)pcc->SegmentNb[_J];

	// DRAW LINES (I and J)
	v0 = pcc->Origin;
	NVec3Add(&v1,&v0,&vj);
	for(i=pcc->SegmentNb[_I]+1;i!=0;i--,NVec3AddTo(&v0,&vsubi),NVec3AddTo(&v1,&vsubi))
	{
		_from(&v0,pcol);
		_lineto(&v1,pcol);
	}

	v0 = pcc->Origin;
	NVec3Add(&v1,&v0,&vi);
	for(j=pcc->SegmentNb[_J]+1;j!=0;j--,NVec3AddTo(&v0,&vsubj),NVec3AddTo(&v1,&vsubj))
	{
		_from(&v0,pcol);
		_lineto(&v1,pcol);
	}

	// DRAW KNOTS
	v1 = v0 = pcc->Origin;
	for(j=pcc->SegmentNb[_J]+1;j!=0;j--,NVec3AddTo(&v0,&vsubj),v1=v0)
	{
		for(i=pcc->SegmentNb[_I]+1;i!=0;i--,NVec3AddTo(&v1,&vsubi))
		{
			_DrawXYSquare(&v1,ksize,pcol);
		}
	}
*/

	

/*
	
	NUT_Get3DMouseRay(&vorigin,&vdir,NULL,ptouchpos);
	if(!NUT_LineXPLane(&vorigin,&vdir,&pcc->Origin,&pcc->PlaneDir,&v3m) )
		return NVOID;
	
	NVec3SubFrom(&v3m,&pcc->Origin);

	// Inside ColorCage Edges ?
	if( v3m.coord[pcc->Is_X] < 0 || v3m.coord[pcc->Is_X] > pcc->Size.x  )
		return NVOID;
	
	if( v3m.coord[pcc->Is_Y] < 0 || v3m.coord[pcc->Is_Y] > pcc->Size.y  )
		return NVOID;

	// ... From this point we are sure v3m is "inside" the cage limits 
	v.x = (v3m.coord[pcc->Is_X]*(Nf32)pcc->SegmentNb[_X])/pcc->Size.x;
	v.y = (v3m.coord[pcc->Is_Y]*(Nf32)pcc->SegmentNb[_Y])/pcc->Size.y;
	
	Nu32 celli		= (Nu32)floorf(v.x);
	Nu32 cellj		= (Nu32)floorf(v.y);
	Nu32 xknotnb	= pcc->SegmentNb[_X]+1;
	v.x -= (Nf32)celli;
	v.y -= (Nf32)cellj;
	
	// IDs Pattern
	//		2 ------- 3
	//		|         |
	//		|         |
	//		|         |
	//		0 ------- 1
	
	if( v.x < pickradius )
	{
		// id0
		if( v.y < pickradius )
		{
			return ( celli + cellj*xknotnb );
		}

		// id2
		if( v.y > 1.0f - pickradius )
		{
			return ( celli + (cellj+1)*xknotnb );
		}
	}

	if( v.x > 1.0f - pickradius )
	{
		// id1
		if( v.y < pickradius )
		{
			return ( celli + cellj*xknotnb +1 );
		}

		// id3
		if( v.y > 1.0f - pickradius )
		{
			return ( celli + (cellj+1)*xknotnb + 1 );
		}
	}

	return NVOID;
*/
}

// ------------------------------------------------------------------------------------------
// NUT_Set_ColorCage_KnotColor_Range
// ------------------------------------------------------------------------------------------
// Description :
//	Change ColorcageKnot Color of a range . 
// ------------------------------------------------------------------------------------------
// In	:
//		NUT_COLORCAGE								*pcc				A valid pointer on a ColorCage
//		const Nu32									first_knotindex		Index of the first Knot to update
//		const void									*pcol				A Valid pointer on a memory zone containing incomming color data.
//		const Nu32									incolor_stride		How many bytes between 2 incoming colors ?
//		const Nu32									incolor_nb			Number of incoming colors. depending on the number of ColorCage knots, some of these incoming colors could be ignored.
//		const NUT_COLORCAGE_COLOR_FORMAT_ENUM		incolor_format		Format of the incoming color data.
// Out :
//
// ------------------------------------------------------------------------------------------
void NUT_Set_2DAAColorCage_KnotColorRange(NUT_2DAACOLORCAGE *pcc, const Nu32 first_knotindex, const void *pcol, const Nu32 incolor_stride, const Nu32 incolor_nb, const NCOLOR_FORMAT_ENUM incolor_format)
{
	NErrorIf( first_knotindex >= NGetArraySize(&pcc->ColorArray), NERROR_ARRAY_INDEX_BEYOND_LIMITS );
	
	Nu32				i;
	Nu32				size = NGetArraySize(&pcc->ColorArray) - first_knotindex;
	NBYTE				*ptr;

	// Size is already set as the number of available Knots between the first to update and the last one of the array.
	// So here we check "incolor_nb" to be sure it's lower than "size", in that case "size" becomes "incolor_nb" because it will be used
	// as the number of color to update some lines below. If not, ( Size< incolor_nb) then size stay the same and the number of updated 
	// is going to stay inside the limit of the array !
	if( size > incolor_nb )
	{
		size = incolor_nb;
	}
	
	NCOLOR				*pdestcolor = &((NCOLOR*)NGetFirstArrayPtr(&pcc->ColorArray))[first_knotindex];
	ptr = (NBYTE*)pcol; 
	for(i=size;i!=0;i--,pdestcolor++, ptr+=incolor_stride)
	{
		NColorConverter(pdestcolor, ptr, incolor_format);
	}
}

// ------------------------------------------------------------------------------------------
// NUT_Apply_ColorCage
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
/*
inline Nu32 _isinsidecolorcagecell( NVEC2 *pr, const NVEC3 *pv, const NUT_2DAACOLORCAGE *pcc )
{
	Nu32				i,j;
	Nu32				id;
	NVEC2			v0,v1;
	NVEC2			v;
	NVEC2			ext;
	Nf32				fi	= pcc->Size.x/(Nf32)pcc->SegmentNb[_I];
	Nf32				fj	= pcc->Size.y/(Nf32)pcc->SegmentNb[_J];

	ext.x	= fi*0.5f;
	ext.y	= fj*0.5f;

	v0.x	= pcc->Origin.coord[pcc->Is_X] + ext.x;
	v0.y	= pcc->Origin.coord[pcc->Is_Y] + ext.y;
	v1		= v0;
	v.x = pv->coord[pcc->Is_X];
	v.y = pv->coord[pcc->Is_Y];

	for(j=0;j<pcc->SegmentNb[_J];j++,v0.y+=fj,v1=v0)
	{
		for(i=0;i<pcc->SegmentNb[_I];i++,v1.x+=fi)
		{
			if( N2DPointInside2DRectangleEx(&v1,&ext,&v) )
			{
				pr->x = ( v.x - (v1.x-ext.x) ) / 2.0f*ext.x;
				pr->y = ( v.y - (v1.y-ext.y) ) / 2.0f*ext.y;
				return ( i+j*(pcc->SegmentNb[_I]+1) );
			}
		}
	}
	return NVOID;
}
*/

//void NUT_ApplyColorCage(NCOLOR *pcolor, const Nu32 colorstride, const NVEC3 *ppoint, , const Nu32 pointstride, const Nu32 number, const NUT_COLORCAGE *pcc)
//void NUT_Apply_2DAAColorCage(NARRAY *pcolorarray, const NARRAY *pv3array, const NUT_2DAACOLORCAGE *pcc)
void NUT_Apply_2DAAColorCage(NCOLOR *pfirstcolor, const Nu32 colorstride, const NVEC3 *pfirstpoint, const Nu32 pointstride, const Nu32 size, const NUT_2DAACOLORCAGE *pcc)
{

	NVEC3		*pv		= (NVEC3*)pfirstpoint;
	NCOLOR			*pcolor	= (NCOLOR*)pfirstcolor;	
	NCOLOR			*psrcc	= (NCOLOR*)pcc->ColorArray.pFirst;
	Nu32			a;
	Nu32			celli,cellj;	
	Nu32			id0,id1,id2,id3;
	
	NVEC2		v2,vr;
	
	Nf32			inv_il	= (Nf32)pcc->SegmentNb[_X]/pcc->Size.x;
	Nf32			inv_jl	= (Nf32)pcc->SegmentNb[_Y]/pcc->Size.y;
	Nu32			xknotnb	= pcc->SegmentNb[_X]+1;
	NCOLOR			c01,c23;

	for(a=size;a!=0;a--,pcolor = (NCOLOR*)((NBYTE*)pcolor+colorstride),pv=(NVEC3*)((NBYTE*)pv+pointstride))
	{
		// pv localization
		v2.x = pv->coord[pcc->Is_X] - pcc->Origin.coord[pcc->Is_X];
		v2.y = pv->coord[pcc->Is_Y] - pcc->Origin.coord[pcc->Is_Y];
		v2.x = NCLAMP(0.0f,v2.x,pcc->Size.x);
		v2.y = NCLAMP(0.0f,v2.y,pcc->Size.y);
		v2.x *=inv_il;
		v2.y *=inv_jl;
		celli = (Nu32)NMIN((Nu16)floor(v2.x),pcc->SegmentNb[_X]-1);
		cellj = (Nu32)NMIN((Nu16)floor(v2.y),pcc->SegmentNb[_Y]-1);
		
		vr.x = v2.x - (Nf32)celli;
		vr.y = v2.y - (Nf32)cellj;

		// Retrieve IDs
		//		2 ------- 3
		//		| .       |
		//		|    +    |
		//		|       . |
		//		0 ------- 1
		id0 = celli + cellj*xknotnb;
		id1 = id0 + 1;
		id2 = id0 + xknotnb;
		id3 = id2 + 1;

		NErrorIf(id0 >= NGetArraySize(&pcc->ColorArray), NERROR_ARRAY_INDEX_BEYOND_LIMITS);
		NErrorIf(id1 >= NGetArraySize(&pcc->ColorArray), NERROR_ARRAY_INDEX_BEYOND_LIMITS);
		NErrorIf(id2 >= NGetArraySize(&pcc->ColorArray), NERROR_ARRAY_INDEX_BEYOND_LIMITS);
		NErrorIf(id3 >= NGetArraySize(&pcc->ColorArray), NERROR_ARRAY_INDEX_BEYOND_LIMITS);
		
		// pcolor Update
		NColorLerp(&c01,&psrcc[id0],&psrcc[id1],vr.x);
		NColorLerp(&c23,&psrcc[id2],&psrcc[id3],vr.x);
		NColorLerp(pcolor,&c01,&c23,vr.y);

	}
}

void	NUT_Apply_2DAAColorCage_To_RenderableGeometries(NRENDERABLE *prenderable, const NUT_2DAACOLORCAGE *pcc)
{
	NGEOMETRY					*pgeom = (NGEOMETRY*)prenderable->GeomArray.pFirst;
	NVEC3					*pv;
	NCOLOR						*pc;
	Nu32						stride;

	for(Nu32 a=prenderable->GeomArray.Size;a!=0;a--,pgeom++)
	{
		NErrorIf(!NCHECK_VERTEX_SPECS_POSITION_3f(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags), NERROR_GEOMETRY_INCOMPATIBLE_TYPE );
		NErrorIf(!NCHECK_VERTEX_SPECS_COLOR0_4f(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags), NERROR_GEOMETRY_INCOMPATIBLE_TYPE );
		if( FLAGS_TEST(NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags,MASK_NVERTEX_SPECS_POSITION|MASK_NVERTEX_SPECS_COLOR0,FLAGS_NVERTEX_SPECS_V3|FLAGS_NVERTEX_SPECS_C4) )
		{
			pv		= (NVEC3*)pgeom->Mesh.VertexArray.pFirst; // because Position is always the first Vertex member !
			pc		= (NCOLOR*)( pgeom->Mesh.VertexArray.pFirst + NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.OffsetToColor0 ); 
			stride	= pgeom->Mesh.VertexArray.ElementSize;
			NUT_Apply_2DAAColorCage(pc,
									stride,
									pv,
									stride,
									pgeom->Mesh.VertexArray.Size,
									pcc );
		}
	}
}
