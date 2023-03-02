#include "lib/N/NCStandard.h"
#include "../NFlags.h"
#include "../Containers/NArray.h"
#include "../Geometry/Components/NGeometryTextureCoordinate.h"
#include "../NCoordinateSystemConversions.h"
#include "../Miscellaneous/NVertexColorBuild.h"
#include "../Miscellaneous/NVertexTexCoordBuild.h"

#include "lib/N/NErrorHandling.h"
#include "NUT_Shape.h"

extern NVERTEX_SPECS NVertexSpecs[];

/*
static inline void extract_color_range(NCOLOR **ppcolor_range, Nu32 *pcolor_rangesize,const NUT_SHAPE_DESC *pcoredesc, const Nu32 coredesc_flags, NCOLOR *pdefault_color)
{
	// ? ColorRange
	if( ISFLAG_ON(coredesc_flags,FLAG_NUTSCD_USEPARAMS_COLORRANGE) )
	{
		*ppcolor_range		= (NCOLOR*)pcoredesc->pColorRange;
		*pcolor_rangesize	= (Nu32)pcoredesc->ColorRangeSize;
	}
	else
	{
		*ppcolor_range		= pdefault_color;
		*pcolor_rangesize	= 1;
		NSetColorf( pdefault_color,DEFAULT_NUTSCD_COLOR_4F );
	}
}
*/

/*
static inline void extract_colorgradient(NCOLOR_GRADIENT_TYPE_ENUM *pgradtype, NCOLOR_RANGE_LERP_MODE_ENUM *pgradlerpmode, Nu32 *pgradangle,const NUT_SHAPE_DESC *pcoredesc, const Nu32 coredesc_flags)
{
	// ? ColorGradient
	if( ISFLAG_ON(coredesc_flags,FLAG_NUTSCD_USEPARAMS_COLORGRADIENT) )
	{
		*pgradtype		= pcoredesc->ColorGradientType;
		*pgradlerpmode	= pcoredesc->ColorGradientLerpMode;
		*pgradangle		= pcoredesc->ColorGradientAngle;
	}
	else
	{
		*pgradtype		= NCOLOR_GRADIENT_TYPE_LINEAR;
		*pgradlerpmode	= NCOLOR_RANGE_LERP_CLAMP;
		*pgradangle		= NANGLE0;
	}
}
*/
static inline void extract_size(NVEC2 *psize, const NUT_SHAPE_DESC *pcoredesc, const Nu32 coredesc_flags /*, const NTEXTURE *ptexture, const NUT_GEOM_UVBASE *puvbase*/)
{
	// Quad Geometry Size computing
	/*
		NErrorIf(ISFLAG_ON(coredesc_flags,FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE) && !ptexture,NERROR_GEOMETRY_MISSING_TEXTURE);
		if( ptexture && ISFLAG_ON(coredesc_flags,FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE ))
		{
			psize->x = NABS(puvbase->U.x + puvbase->U.y)*NCoord_PixToHRel_X(ptexture->size.x); // special case of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
			psize->y = NABS(puvbase->V.x + puvbase->V.y)*NCoord_PixToHRel_Y(ptexture->size.y); // special case of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
		}
		else*/
	if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_SIZE))
	{
		psize->x = pcoredesc->Size.x;
		psize->y = pcoredesc->Size.y;
	}
	else
	{
		psize->x = DEFAULT_NUTSCD_SIZE_WIDTH;
		psize->y = DEFAULT_NUTSCD_SIZE_HEIGHT;
	}
}
static inline void scale_size(NVEC2 *psize, const NUT_SHAPE_DESC *pcoredesc, const Nu32 coredesc_flags)
{
	// Scale Size ?
	if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_SCALE))
	{
		psize->x *= pcoredesc->Scale.x;
		psize->y *= pcoredesc->Scale.y;
	}
}
static inline void extract_build_matrix(NMATRIX *pmatrix, const NVEC2 *psize, const NUT_SHAPE_DESC *pcoredesc, const Nu32 coredesc_flags)
{
	if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE))
	{
		switch (pcoredesc->Construction_Plane)
		{
		case _PLANE_XY:
			NVec4Set(&pmatrix->V4[0], psize->x, 0, 0, 0); // ---> X Axis (i)
			NVec4Set(&pmatrix->V4[1], 0, psize->y, 0, 0); // ---> Y Axis (j)
			NVec4Set(&pmatrix->V4[2], 0, 0, 1, 0);		  // ---> Z Axis (k)
			break;

		case _PLANE_XZ:
			NVec4Set(&pmatrix->V4[0], psize->x, 0, 0, 0); // ---> X Axis (i)
			NVec4Set(&pmatrix->V4[1], 0, 0, psize->y, 0); // ---> Y Axis (j)
			NVec4Set(&pmatrix->V4[2], 0, 1, 0, 0);		  // ---> Z Axis (k)
			break;

		case _PLANE_ZY:
			NVec4Set(&pmatrix->V4[0], 0, psize->x, 0, 0); // ---> X Axis (i)
			NVec4Set(&pmatrix->V4[1], 0, 0, psize->y, 0); // ---> Y Axis (j)
			NVec4Set(&pmatrix->V4[2], 1, 0, 0, 0);		  // ---> Z Axis (k)
			break;
		}
	}
	else if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR))
	{
		NVEC3 vect;
		// Plane construction
		NVec3Set(&vect, 0, -1, 0);
		NVec3CrossProduct((NVEC3 *)&pmatrix->V4[0], &pcoredesc->ConstructionPlane_DirectorVector, &vect);
		NVec3CrossProduct((NVEC3 *)&pmatrix->V4[1], &pcoredesc->ConstructionPlane_DirectorVector, (NVEC3 *)&pmatrix->V4[0]);
		Nf32 w = psize->x / NVec3Length((NVEC3 *)&pmatrix->V4[0]); // means psize->x / Length of X Axis
		Nf32 h = psize->y / NVec3Length((NVEC3 *)&pmatrix->V4[1]); // means psize->y / Length of Y Axis

		NVec3ScaleBy((NVEC3 *)&pmatrix->V4[0], w); // means multiply X Axis by w
		NVec3ScaleBy((NVEC3 *)&pmatrix->V4[1], h); // means multiply Y Axis by h
		*((NVEC3 *)&pmatrix->V4[2]) = pcoredesc->ConstructionPlane_DirectorVector;
		NVec3Normalize((NVEC3 *)&pmatrix->V4[2]); // means take care Z Axis is 1

		// Finalize by setting all the .w to ZERO
		pmatrix->V4[0].w = 0.0f;
		pmatrix->V4[1].w = 0.0f;
		pmatrix->V4[2].w = 0.0f;
	}
	else
	{
		// By default using NUT_PLANE_XY
		NVec4Set(&pmatrix->V4[0], psize->x, 0, 0, 0); // ---> X Axis (i)
		NVec4Set(&pmatrix->V4[1], 0, psize->y, 0, 0); // ---> Y Axis (j)
		NVec4Set(&pmatrix->V4[2], 0, 0, 1, 0);		  // ---> Z Axis (k)
	}

	// Origin and Anchor
	if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN))
	{
		pmatrix->V4[3].x = pcoredesc->ConstructionOrigin.x; // |
		pmatrix->V4[3].y = pcoredesc->ConstructionOrigin.y; // |- Construction Base Origin
		pmatrix->V4[3].z = pcoredesc->ConstructionOrigin.z; // |
		pmatrix->V4[3].w = 1.0f;
	}
	else
	{
		pmatrix->V4[3].x = 0.0f; // |
		pmatrix->V4[3].y = 0.0f; // |- Construction Base Origin
		pmatrix->V4[3].z = 0.0f; // |
		pmatrix->V4[3].w = 1.0f;
	}

	if (ISFLAG_ON(coredesc_flags, FLAG_NUTSCD_USEPARAM_ANCHOR))
	{
		// Translate Construction Origin regarding Anchor point
		pmatrix->V4[3].x -= pmatrix->V4[0].x * pcoredesc->Anchor.x + pmatrix->V4[1].x * pcoredesc->Anchor.y + pmatrix->V4[2].x * pcoredesc->Anchor.z;
		pmatrix->V4[3].y -= pmatrix->V4[0].y * pcoredesc->Anchor.x + pmatrix->V4[1].y * pcoredesc->Anchor.y + pmatrix->V4[2].y * pcoredesc->Anchor.z;
		pmatrix->V4[3].z -= pmatrix->V4[0].z * pcoredesc->Anchor.x + pmatrix->V4[1].z * pcoredesc->Anchor.y + pmatrix->V4[2].z * pcoredesc->Anchor.z; // A QUAD doesn't have a depth ... but HERE (only) we assume there is one (=1) like this we can Anchor Quad in its own depth dimension ! Cool
		pmatrix->V4[3].w = 1.0f;
	}
}

static inline void extract_figurecenter(NVEC3 *pcenter, const NMATRIX *pmatrix)
{
	// !!! Figure Size was already incorporated into the Matrix !!!
	// So, in our base we have figure large = 1*i, figure height = 1*j and figure depth = 1*k ...
	// So center of the figure, is just Half of that stuff ...
	// ( but -j due to ANCHORING system which declare ANCHOR_TOP = 0 ANCHOR_BOTTOM = -1 ANCHOR_LEFT = 0 ANCHOR_RIGHT = 1)
	pcenter->x = pmatrix->V4[3].x + 0.5f * (pmatrix->V4[0].x - pmatrix->V4[1].x + pmatrix->V4[2].x);
	pcenter->y = pmatrix->V4[3].y + 0.5f * (pmatrix->V4[0].y - pmatrix->V4[1].y + pmatrix->V4[2].y);
	pcenter->z = pmatrix->V4[3].z; // Because it's a FLAT figure
}

static inline Nf32 _angular_sector(const Nf32 deg_from, const Nf32 deg_to, const Nbool trigo_sens)
{
	Nf32 sect;

	if (trigo_sens)
	{
		sect = deg_to - deg_from;
		if (sect <= 0.0f) // that means FromAngle > ToAngle !
			sect += 360.0f;
	}
	else
	{
		sect = deg_to - deg_from;
		if (sect >= 0.0f) // that means FromAngle > ToAngle !
			sect -= 360.0f;
	}
	return sect;
}

static inline Nf32 get_colrangelerp_index_ratio(const Nu32 vertex_rangeid, const Nu32 vertex_rangesize)
{
	NErrorIf(vertex_rangeid >= vertex_rangesize, NERROR_INDEX_OUTOFRANGE);
	if (vertex_rangeid == vertex_rangesize - 1) // fix the problem of vertex_rangesize == 1 in the devide operation some lines bellow !!!
		return 1.0f;
	else
		return (Nf32)vertex_rangeid / (Nf32)(vertex_rangesize - 1);
}

static inline void _set_nut_shape_vertex_data(NBYTE *pdstvertex, const NVERTEX_FORMAT_ENUM dst_vertex_format, const Nu8 colorsetid, const Nu8 texcoordsetid, const NVERTEX_V3N3C4T2 *pvertex_V3N3C4T2)
{
	if (dst_vertex_format == NVERTEX_FORMAT_V3N3C4T2)
	{
		*(NVERTEX_V3N3C4T2 *)pdstvertex = *pvertex_V3N3C4T2;
	}
	// Setup vertex data one by one
	else
	{
		// POSITION
		if (NCHECK_VERTEX_SPECS_POSITION(NVertexSpecs[dst_vertex_format].DirectAccess.Flags))
		{
			NConvertVertexPosition_3f_ToVertexDataFormat(pdstvertex, dst_vertex_format, &pvertex_V3N3C4T2->Position_3f);
		}
		// NORMAL
		if (NCHECK_VERTEX_SPECS_NORMAL(NVertexSpecs[dst_vertex_format].DirectAccess.Flags))
		{
			NConvertVertexNormal_3f_ToVertexDataFormat(pdstvertex, dst_vertex_format, &pvertex_V3N3C4T2->Normal_3f);
		}
		// COLOR
		if (NCHECK_VERTEX_SPECS_COLORS(NVertexSpecs[dst_vertex_format].DirectAccess.Flags))
		{
			NConvertVertexColor_4f_ToVertexDataFormat(pdstvertex, dst_vertex_format, colorsetid, &pvertex_V3N3C4T2->Color0_4f);
		}
		// TEXCOORD
		if (NCHECK_VERTEX_SPECS_TEXCOORDS(NVertexSpecs[dst_vertex_format].DirectAccess.Flags))
		{
			NConvertVertexTexCoord_2f_ToVertexDataFormat(pdstvertex, dst_vertex_format, texcoordsetid, &pvertex_V3N3C4T2->TexCoord0_2f);
		}
	}
}
// ------------------------------------------------------------------------------------------
// NUT_InsertRing
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a Ring (2D) into an already created geometry
// ------------------------------------------------------------------------------------------
// In	:
//			pgeom:	a valid pointer on a geometry
//			dec:	a valid pointer on a disc description structure
// Out :
//
// ------------------------------------------------------------------------------------------
// Notes about NUT_RING_DESC parameters:
//		Nu32						flags
//										FLAG_RING_DESC_USEPARAM_SCRUV
//														Set it if you want to use the 'scrUV' Param.
//
//										FLAG_RING_DESC_USEPARAM_UV
//														Set it if you want to use the 'UV' Param.
//														Note that FLAG_RING_DESC_USEPARAM_SCRUV and FLAG_RING_DESC_USEPARAM_UV can be used together.
//														In that case, uvs work inside the scrUV rect.
//
//										FLAG_RING_DESC_USEPARAM_CONSTRUCTIONORIGIN
//														Set it if you want to use the 'Construction Origin' Param.
//														If not. Figure is going to be build around Default Origin( 0,0,0 ).
//
//										FLAG_NUTSCD_USEPARAM_ANCHOR
//														Set it if you want to use the 'Anchor' Param.
//														If not. Figure anchor point is going to be the default one (TOP,LEFT,BACK).
//
//										FLAG_NUTSCD_USEPARAM_SCALE
//														Set it if you want to use the 'Scale' Param.
//														There is 2 ways to define figure size, "from texture" or "from Size"
//														In both cases, final figure size is going to be multiply by this value before figure creation.
//
//										FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE
//														Set it if you want to compute figure size from linked texture size.
//														if not, by default,Size (x and y)  parameters are going to be used to define Figure Size.
//
//										FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE
//														Set it if you want to use the 'construction_plane' Param.
//
//										FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR
//														Set it if you want to use the 'ConstructionPlane_DirectorVector' Param.
//														If the 2 flags 'FLAG_USEPARAM_CONSTRUCTIONPLANE and 'FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR'
//														are set to ON, only 'FLAG_USEPARAM_CONSTRUCTIONPLANE' is used.
//
//														If none of them is set to ON, function is going to take the 'NUT_PLANE_XY' as a default construction_plane
//														to work.
//
//										FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES
//														Set it if you want to use the 'FromAngle' and 'ToAngle' Params.
//														In that case an open ring is going to be created ( something like the PACMAN 'figure' )
//														All the ring vertex will be created from FromAngle to ToAngle ...
//														AND the last created vertex is going to stay UNWELD from the first one even if
//														FromAngle = 0� and ToAngle=360�.
//														If the flag is set to OFF a full 360� closed ring is going to be created and first
//														and last circumference vertex are going to be weld !
//
//										FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO
//														Set it if you want to use the 'SmallSizeRatio' Param. (x and y)
//														If the flag is set to OFF the default SmallSizeRatio values (DEFAULT_RINGDESC_SMALLSIZERATIO_WIDTH and DEFAULT_RINGDESC_SMALLSIZERATIO_HEIGHT) will be used.
//
//		NUT_CONSTRUCTION_PLANE		construction_plane	To use it, set the flag FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE.
//										NUT_PLANE_XY	Cristal clear ...
//										NUT_PLANE_XZ	Cristal clear ...
//										NUT_PLANE_YZ	Cristal clear ...
//
//		NVEC3					ConstructionPlane_DirectorVector;
//														Cristal clear ...
//														To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR.
//
//		NVEC2					Size				Figure Size obviously (Width and Height). Used only if FLAG_QUADSIZE_FROM_TEXTURESIZE is set to OFF
//
//		NRECTs16				scrUV				UV rect in screen coord system (pixel). Used to get texture rect into ATLAS texture.
//														To compute final UVs coordinates a valid texture has to be already bind with the geometry.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_SCRUV.
//
//		NRECTf32					UV					UV Rect in normalized format (0.0f < uv < 1.0f ). BUT Can be greater than 1.0f for Tiling.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_UV.
//
//														Because user can ask for using 'scrUV' (FLAG_USEPARAM_SCRUV) and 'UV' (FLAG_USEPARAM_UV) at the same time
//														final uvs are the result of the right combination of both parameters.
//
//
//		NVEC3					ConstructionOrigin	Absolute position of the building reference point. 0,0,0 by default.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_CONSTRUCTIONORIGIN.
//
//		NVEC3					Anchor				Relative Anchor point... Which point of the Figure is going to be place at the "Construction Origin" ...
//														0,0,0 by default, that means the top left corner of the figure (Ring). To use it, set the flag FLAG_USEPARAM_ANCHOR.
//
//
//		NVEC2					Scale				There is 2 ways to define figure size, "from texture" or "from Width & Height definition"
//														In both cases, final quad size is going to be multiply by this value before quad creation.
//														To use it, set the flag FLAG_NUTSCD_USEPARAM_SCALE.
//
//		Nf32						OpenRing_FromAngle	To create an open disc (like a PACMAN figure) and define the starting circumference vertex position.
//														To use it, set the flag FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES.
//		Nf32						OpenRing_ToAngle	To create an open disc (like a PACMAN figure) and define the starting circumference vertex position.
//														To use it, set the flag FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES.

//		NVEC2					SmallSizeRatio;		To define a ring we need 2 circles od vertex. Size is the size of the largest.
//														Smallest circle size is equal to: SmallSize = Size*SmallSizeRatio;

//		Nu32						SliceNb;			Number of slices created.

//		NCOLOR						ColorRange[4]		Color associated with each Vertex.
//														Small circumference vertex are going to have a color between ColorRange[0] and ColorRange[2],
//														All the large circumference vertex are going to have a color between ColorRange[1] and ColorRange[3].
//
//		Description examples
//		--------------------
/*
			NUT_RING_DESC	ring_desc;
			Nmem0(&ring_desc,NUT_RING_DESC );

			// Define Ring Construction plane
			ring_desc.construction_plane	= NUT_PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
			FLAG_ON(ring_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
			// ... or like this (to use a plane director vector)
			NVec3Set(&ring_desc.ConstructionPlane_DirectorVector,0,0,1);
			FLAG_ON(ring_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR);

			// Define Disc Size
			ring_desc.Size.x	= my_width;
			ring_desc.Size.y	= my_height;
			// ... or like this (Size is not going to be used ... )
			FLAG_ON(ring_desc.flags,FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE);

			// Define Scale
			ring_desc.Scale.x	= my_scaleXvalue;
			ring_desc.Scale.y	= my_scaleYvalue;
			FLAG_ON(ring_desc.flags,FLAG_NUTSCD_USEPARAM_SCALE);

			// Define UVs
			memcpy(&ring_desc.scrUV,&my_srcuvs,sizeof(NRECTs16));
			FLAG_ON(ring_desc.flags,FLAG_RING_DESC_USEPARAM_SCRUV);
			// ... or like this, or Both together !
			memcpy(&ring_desc.UV,&my_uvs,sizeof(NRECTf32));
			FLAG_ON(ring_desc.flags,FLAG_RING_DESC_USEPARAM_UV);

			// Define figure building position( "Origin" )
			NVec3Copy(&ring_desc.ConstructionOrigin,&my_vector3_position);
			FLAG_ON(ring_desc.flags,FLAG_RING_DESC_USEPARAM_CONSTRUCTIONORIGIN);

			// Define figure Anchor point ( TOP, LEFT, BACK by default )
			NVec3Copy(&ring_desc.Anchor,&my_vector3_anchor);
			FLAG_ON(ring_desc.flags,FLAG_NUTSCD_USEPARAM_ANCHOR);

			// Define ring slices number
			ring_desc.SliceNb = my_number_of_slice;

			// Define Open Ring 'From' and 'To' angles
			ring_desc.OpenRing_FromAngle	= my_fromangle;
			ring_desc.OpenRing_ToAngle		= my_toangle;
			FLAG_ON(ring_desc.flags,FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES);

			// Define Ring small size
			ring_desc.SmallSizeRatio.x = my_small_size_ratio_x;
			ring_desc.SmallSizeRatio.y = my_small_size_ratio_y;
			FLAG_ON(ring_desc.flags,FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO);

			// Define Colors
			NColorRangeCopy( &ring_desc.ColorRange,&my_4_colors_range,4);
*/
// ------------------------------------------------------------------------------------------
void NUT_InsertRing(NGEOMETRY *pgeom, const NUT_RING_DESC *desc)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + INFOS : VERTEX
	// +
	//		   	   + +
	//                 9
	//         + 8 6    7
	//		 +		 4   5
	//		+         2  3
	//		+    +    0  1
	//		+         +
	//		 +       +
	//         + + +

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + INFOS : UVs
	// + Ring UVs are going to be calculated 'inside' the Texture Rectangle using the whole surface.
	// +
	// +-+ STANDARD:
	//
	//	1) Without Mirroring	2) U Mirroring Only		3) V Mirroring Only		4) U and V Mirroring
	//	   0,0 --------1,0		   0,0 +++++---1,0		   0,0 ---+----1,0		0,0 +---------1,0
	//      |   + + +   |		    |       ++  |		    +     Or    +		    |Or   	 	+
	//		| +		  + |			| 		  + |		   	+ 		    +		   	|	  	 	+
	//		|+         +|			|          +|		   	+           +		   	|		   +|
	//		|+    +Or  +|			+Or		   +|		   	|+		   +|		   	|		  +	|
	//		|+         +|			|          +|		   	| +       + |		   	|      ++   |
	//		| +       + |			|         + |		   	|  ++	++  |		   	|	+++	    |
	//      |   + + +   |		    |       ++  |		    |	 +++	|		    |+++		|
	//	   0,1 --------1,1		   0,1 +++++---1,1		   0,1 --------1,1	   (0,1)+----------1,1
	//		   DEFAULT
	// +---------------------------------------------------------------------------------------
	// +-+ FACE MAP:
	//
	//	5) Face map
	//	   0,0 --------1,0		U Mirroring is automatically enable when using Face Map
	//      |           |		Its the only way to map repeatedly all the quads of the ring strip.
	//		|  		    |
	//		|           |		V Mirroring is not effective for Face Map, trying to use it will
	//		|           |		generate an Error in debug mode.
	//		|           |
	//		|           |
	//      |           |
	//	   0,1 --------1,1
	//
	//	   0,0--+---+--1,0		"FaceMapSubSliceNb" defines the number of successive quads mapped
	//      |   +   +   |		before mirroring.
	//		|  	+	+   |
	//		|   +   +   |		example of UVs repartition with "FaceMapSubSliceNb = 3"
	//		|   +   +   |
	//		|   +   +   |
	//		|   +   +   |
	//      |   +   +   |
	//	   0,1--+---+--1,1
	// Nu32					meshdesc	=  NGetGeometrySpecs( NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS) )->MeshDescription;
	// NUT_GEOM_UVBASE			uvbase;
	NMATRIX3x3 txc_matrix;
	NVEC2 size;
	NMATRIX bmx;
	Nf32 ang;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Texture Coordinates Build
	// +
	NGetBuildBaseTexCoordMatrix3x3(&txc_matrix, &desc->CoreDesc.TexCoordBuild);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + FACE MAP U MIRRORING (only) EVERY ...
	// +	When U Mirroring is activated, it's possible to "stretch"
	// +	the whole texture on several slices before Mirroring.
	NErrorIf(ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_FACEMAP_SUBSLICENB) && ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_RING_DESC_FACEMAP), NERROR_INCONSISTENT_FLAGS);
	Nf32 facemap_subslicenb;
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_FACEMAP_SUBSLICENB))
	{
		facemap_subslicenb = (Nf32)desc->FaceMapSubSliceNb;
	}
	else
	{
		facemap_subslicenb = (Nf32)DEFAULT_RING_DESC_FACEMAP_SUBSLICENB;
	}
	NErrorIf(facemap_subslicenb == 0.0f, NERROR_NULL_VALUE);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Color Range
	// +

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Size
	// +
	// Basic incoming size extraction:
	extract_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	scale_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Build Matrix
	// +
	extract_build_matrix(&bmx, &size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + SMALL SIZE RATIO
	// +	There are 2 circles of vertex to build the ring.
	// +	Size of the smallest one is calculated from Main Size and the Small Size Ratio
	NVEC2 smallsizeratio;
	Nf32 a;

	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO))
	{
		smallsizeratio = desc->SmallSizeRatio;
	}
	else
	{
		// We have to figure out "smallsizeratio" value.
		//		There are 2 cases, without or with FLAG_RING_DESC_FACEMAP set to ON.
		//		A)In the first case, we are going to try keeping as much as possible the txc_matrix AxisX/AxisY ratio.
		//		  ( is suppose to be th texture width/height ratio )
		//		  To doing this, first get the 3D width of a full mapped texture along the ring external perimeter.
		//		  ... Apply to it the texture UV height/width ratio to obtain the corresponding 3D height and from it,
		//		  obtain smallsizeratio ... easy !!!
		//		B)In the second case (without) we are going to use the default value...
		// A)FIRST CASE
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_FACEMAP))
		{
			// 1) Retrieve the ring external perimeter from Ring Size:
			Nf32 perimeter;
			if (size.x == size.y)
			{
				perimeter = NCirclePerimeter(size.x * 0.5f);
			}
			else
			{
				perimeter = NEllipsePerimeterRamanujanApprox(size.x * 0.5f, size.y * 0.5f);
			}

			// 2) Keep only the used sector perimeter
			if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES))
			{
				if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_VERTEX_TRIGO_INVERSE_BUILDING))
					ang = _angular_sector(desc->OpenRing_FromAngle, desc->OpenRing_ToAngle, NFALSE);
				else
					ang = _angular_sector(desc->OpenRing_FromAngle, desc->OpenRing_ToAngle, NTRUE);

				perimeter *= ang / 360.0f;
			}
			// 3) How many times the texture Width is repeated along this perimeter ?
			//	 ---> (Nf32)desc->SliceNb /(Nf32)desc->FaceMapSubSliceNb
			//   So 3D Width will be:
			//	 ---> perimeter * (Nf32)desc->FaceMapSubSliceNb/(Nf32)desc->SliceNb
			//
			//	So, 3D Height should be:
			//	 ---> previous result * (NVec2FastLength((NVEC2*)&uv_u)/NVec2FastLength((NVEC2*)&uv_v))
			//		Where the ratio of UV vector lengths is apply to the calculated 3D width to obtain the corresponding 3D Height.
			a = perimeter * ((Nf32)desc->FaceMapSubSliceNb / (Nf32)desc->SliceNb);
			a *= (NVec2FastLength((NVEC2 *)&txc_matrix.XAxis) / NVec2FastLength((NVEC2 *)&txc_matrix.YAxis));
			// 4) But at the end of the day we need to calculate the "smallsizeratio", in other words
			//	 We have to find the value which multiply by the radius (x en y) will give the 3D  Height ...
			//	 Let just divide 3D Height by the radius to obtain it ...
			smallsizeratio.x = 1.0f - NCLAMP(0.01f, a / (size.x * 0.5f), 0.99f); // Clamp to be sure having a ring with a minimum thickness.
			smallsizeratio.y = 1.0f - NCLAMP(0.01f, a / (size.y * 0.5f), 0.99f); // Clamp to be sure having a ring with a minimum thickness.
		}
		// B)SECOND CASE
		// Without FACE MAP Activation, just use the default values
		else
		{
			smallsizeratio.x = DEFAULT_RING_DESC_SMALLSIZERATIO_WIDTH;
			smallsizeratio.y = DEFAULT_RING_DESC_SMALLSIZERATIO_HEIGHT;
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + PRE-BUILDING VERTEX BUILDING PARAMETERS
	// +
	// +	Calculating some parameters accordingly with the user demands used to create vertex.
	// +
	// +--+ Because we are going to build the Ring from its center it will be easier from now to use
	//		half width and half height instead of width, height !
	//		... !!!!!!! SO FROM HERE: vwidth is HALF vwidth !
	//		... !!!!!!! SO FROM HERE: vheight is HALF vheight !
	// NVERTEX_STRIDES			*pstrides	= &NGetGeometrySpecs( NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS) )->VertexStrides;
	NBYTE *pbyte;
	NVEC3 vcenter;
	NVEC3 vsmallwidth, vsmallheight;
	NVEC2 smalluv_u, smalluv_v;
	NVEC3 vi, vj;
	NTRIANGLE triangle;
	Nu32 firstvertex, idvertex;
	Nu32 i;
	Nf32 fcos, fsin;
	Nf32 angledeg, anglestep;
	Nu32 crea_vertexpair;

	// 	Nf32					tcol,tcolstep;
	// NVERTEX_DESC			vdesc;
	NVERTEX_V3N3C4T2 vertex_V3N3C4T2;
	NVEC3 *pvwidth = (NVEC3 *)&bmx.V4[0];
	NVEC3 *pvheight = (NVEC3 *)&bmx.V4[1];
	NVEC3 *pvdepth = (NVEC3 *)&bmx.V4[2];
	//	NVEC3				*pv0		= (NVEC3*)&bmx.V4[3];

	// +--+ Center point ( no vertex here ! Its a ring , not a disc, but we need a Center to build)
	extract_figurecenter(&vcenter, &bmx);

	// +--+ Scale Width and height to use them as Radius ...
	//		!! because pvwidth and pvheight are part of the bmx matrix, change them has to be
	//		... done at the right time !!! Absolutely after extracting figure center for example ...
	NVec3ScaleBy(pvwidth, 0.5f);
	NVec3ScaleBy(pvheight, 0.5f);
	// +--+ Smallwidth and Smallheight vectors to build internal circle of vertex.
	NVec3Scale(&vsmallwidth, pvwidth, smallsizeratio.x);
	NVec3Scale(&vsmallheight, pvheight, smallsizeratio.y);
	/*
		// +--+ Center point ( no vertex here ! Its a ring , not a disc, but we need a Center to build)
		vcenter.x = pv0->x + pvwidth->x - pvheight->x + pvdepth->x;
		vcenter.y = pv0->y + pvwidth->y - pvheight->y + pvdepth->y;
		vcenter.z = pv0->z;
	*/

	// +--+ Angle, Angle Step and building direction (trigonometric or not)
	NErrorIf((desc->OpenRing_FromAngle < 0.0f) || (desc->OpenRing_FromAngle > 360.0f), NERROR_VALUE_OUTOFRANGE);
	NErrorIf((desc->OpenRing_ToAngle < 0.0f) || (desc->OpenRing_ToAngle > 360.0f), NERROR_VALUE_OUTOFRANGE);
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES))
	{
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_VERTEX_TRIGO_INVERSE_BUILDING))
		{
			angledeg = desc->OpenRing_FromAngle;
			anglestep = _angular_sector(desc->OpenRing_FromAngle, desc->OpenRing_ToAngle, NFALSE) / (Nf32)desc->SliceNb;
		}
		else // Creation in the trigonometric direction ...
		{
			angledeg = desc->OpenRing_FromAngle;
			anglestep = _angular_sector(desc->OpenRing_FromAngle, desc->OpenRing_ToAngle, NTRUE) / (Nf32)desc->SliceNb;
		}

		crea_vertexpair = desc->SliceNb + 1;
		// 		tcol			= 0.0f;
		// 		tcolstep		= 1.0f/(Nf32)(crea_vertexpair-1);
	}
	else
	{
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_VERTEX_TRIGO_INVERSE_BUILDING))
		{
			angledeg = 360.0f;
			anglestep = -360.0f / (Nf32)desc->SliceNb;
		}
		else
		{
			angledeg = 0.0f;
			anglestep = 360.0f / (Nf32)desc->SliceNb;
		}
		crea_vertexpair = desc->SliceNb;
		// 		tcol				= 0.0f;
		// 		tcolstep			= 1.0f/(Nf32)(crea_vertexpair-1);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + PRE-BUILDING UV BUILDING PARAMETERS
	// +
	// +	Calculating some parameters accordingly with the user demands used to create uvs.
	// +
	// +--+ UV vectors scale to be used from Circle Center.
	// Special case of face map
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_FACEMAP))
	{
		// +--+ UV Origin.
		// It still the same as it was from "NGetTexCoordOrthogonal"
		// ... Excepted in the case of user asking for MIRRORING_V. Here we will have a specific interpretation
		// of V Mirroring which is just flip the texture from top to bottom, because in face map there is only one
		// repeat along V axis ...
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_V))
		{
			// Move UV Origin.
			txc_matrix.Origin.x += txc_matrix.YAxis.x;
			txc_matrix.Origin.y += txc_matrix.YAxis.y;
			// ... AND reverse "uv_v"
			txc_matrix.YAxis.x *= -1.0f;
			txc_matrix.YAxis.y *= -1.0f;
		}

		// +--+ UV Base vectors.
		// They still as they are from "NGetTexCoordOrthogonal"
		// +--+ "Small" UV vectors.
		// Used to build small circumference UVs vertex when FACE MAP is not activated. Remain unused.
	}
	else
	{
		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_U))
		{
			// UV Base Vectors.
			NVec2ScaleBy((NVEC2 *)&txc_matrix.XAxis, 0.5f);
			// UV Origin.
			txc_matrix.Origin.x += txc_matrix.XAxis.x;
			txc_matrix.Origin.y += txc_matrix.XAxis.y;
		}

		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_V))
		{
			// UV Base Vectors.
			NVec2ScaleBy((NVEC2 *)&txc_matrix.YAxis, 0.5f);
			// UV Origin.
			txc_matrix.Origin.x += txc_matrix.YAxis.x;
			txc_matrix.Origin.y += txc_matrix.YAxis.y;
		}

		// "Small" UV vectors to build Internal circle of UVs.
		NVec2Scale(&smalluv_u, (NVEC2 *)&txc_matrix.XAxis, smallsizeratio.x);
		NVec2Scale(&smalluv_v, (NVEC2 *)&txc_matrix.YAxis, smallsizeratio.y);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + VERTEX ATTRIBUTES INSERTION
	// +
	// +--+Get Vertex 0 index (it will be the index of the first inserted vertex )
	firstvertex = pgeom->Mesh.VertexArray.Size;
	NUpSizeArray(&pgeom->Mesh.VertexArray, crea_vertexpair * 2, NULL);
	pbyte = NGetArrayPtr(&pgeom->Mesh.VertexArray, firstvertex);
	Nu32 nextvertex = pgeom->Mesh.VertexArray.ElementSize;
	NVEC2 uvradius_u, uvradius_v;
	NVEC3 colorpos;
	Nf32 step = 0.0f;
	Nf32 nxt = 1.0f;
	Nf32 fa;
	NTEXCOORD_2f uvb;
	Nu8 colorsetid = NGetBuildBaseColorSetID(&desc->CoreDesc.ColorBuild);
	Nu8 texcoordsetid = NGetBuildBaseTexCoordSetID(&desc->CoreDesc.TexCoordBuild);

	vertex_V3N3C4T2.Normal_3f = *pvdepth; // Because all normals are exactly the same ... Yet

	colorpos.z = 0.0f; // for all vertex.
	for (i = 0; i < crea_vertexpair; i++, angledeg += anglestep /*, tcol+=tcolstep*/)
	{
		NFastCosSin(NDEGtoANGLE(angledeg), &fcos, &fsin);

		// Small circumference
		// Vertex
		NVec3Scale(&vi, &vsmallwidth, fcos);
		NVec3Scale(&vj, &vsmallheight, fsin);
		vertex_V3N3C4T2.Position_3f.x = vcenter.x + vi.x + vj.x;
		vertex_V3N3C4T2.Position_3f.y = vcenter.y + vi.y + vj.y;
		vertex_V3N3C4T2.Position_3f.z = vcenter.z + vi.z + vj.z;

		colorpos.x = 0.5f + (0.5f * fcos * smallsizeratio.x) / size.x;
		colorpos.y = 0.5f - (0.5f * fsin * smallsizeratio.y) / size.y;
		// colorpos.z = 0.0f; done only once time for all vertex just before the loop.
		NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
									  2 * i, 2 * crea_vertexpair,
									  (Nf32 *)&colorpos,
									  &desc->CoreDesc.ColorBuild);
		/*
				// COLOR
				if( ISFLAG_ON(desc->Flags,FLAG_NUTSCD_USEPARAM_COLORGRADIENTID) )
				{
					NExtractGradientColor(&vertex_V3N3C4T2.Color0_4f,pcolrange,colrangesize,&vg,desc->CoreDesc.ColorGradientID);
				}
				else
				{
					NLerpColorRange(&vertex_V3N3C4T2.Color0_4f,pcolrange,colrangesize,((Nf32)i*2.0f)/(crea_vertexpair*2.0f-1.0f),NCOLORRANGE_LERP_CLAMP);
				}
		*/

		// UV
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_FACEMAP))
		{
			vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x + (step / facemap_subslicenb) * txc_matrix.XAxis.x;
			vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y + (step / facemap_subslicenb) * txc_matrix.XAxis.y;

			// precalculate large circumefrence uv
			uvb.x = vertex_V3N3C4T2.TexCoord0_2f.x + txc_matrix.YAxis.x;
			uvb.y = vertex_V3N3C4T2.TexCoord0_2f.y + txc_matrix.YAxis.y;

			step += nxt;
			if (step == facemap_subslicenb || step == 0.0f)
				nxt *= -1.0f;
		}
		else
		{
			if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_U))
				fa = NABS(fcos);
			else
				fa = fcos;
			NVec2Scale(&uvradius_u, &smalluv_u, fa);

			if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_V))
				fa = NABS(fsin);
			else
				fa = fsin;
			NVec2Scale(&uvradius_v, &smalluv_v, fa);
			vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x + uvradius_u.x + uvradius_v.x;
			vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y + uvradius_u.y + uvradius_v.y;
		}

		// NCopyVertex( pbyte,NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS) ,&vertex_V3N3C4T2,NVERTEX_FORMAT_V3N3C4T2 );
		_set_nut_shape_vertex_data(pbyte, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
		pbyte += nextvertex;

		// Large circumference
		// Vertex
		NVec3Scale(&vi, pvwidth, fcos);
		NVec3Scale(&vj, pvheight, fsin);
		vertex_V3N3C4T2.Position_3f.x = vcenter.x + vi.x + vj.x;
		vertex_V3N3C4T2.Position_3f.y = vcenter.y + vi.y + vj.y;
		vertex_V3N3C4T2.Position_3f.z = vcenter.z + vi.z + vj.z;
		// COLOR
		colorpos.x = 0.5f + 0.5f * fcos;
		colorpos.y = 0.5f - 0.5f * fsin;
		// colorpos.z = 0.0f; done only once time for all vertex just before the loop.
		NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
									  2 * i + 1, 2 * crea_vertexpair,
									  (Nf32 *)&colorpos,
									  &desc->CoreDesc.ColorBuild);
		// UV
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_FACEMAP))
		{
			vertex_V3N3C4T2.TexCoord0_2f = uvb;
		}
		else
		{
			if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_U))
				fa = NABS(fcos);
			else
				fa = fcos;
			NVec2Scale(&uvradius_u, (NVEC2 *)&txc_matrix.XAxis, fa);

			if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_RING_DESC_MIRRORING_V))
				fa = NABS(fsin);
			else
				fa = fsin;
			NVec2Scale(&uvradius_v, (NVEC2 *)&txc_matrix.YAxis, fa);
			vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x + uvradius_u.x + uvradius_v.x;
			vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y + uvradius_u.y + uvradius_v.y;
		}

		// NCopyVertex( pbyte,NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS) ,&vertex_V3N3C4T2,NVERTEX_FORMAT_V3N3C4T2);
		_set_nut_shape_vertex_data(pbyte, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
		pbyte += nextvertex;
	}

	// All the Ring Primitives
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + VERTEX Winding ORDER			VERTEX Winding ORDER
	// +	CCW								CW
	//		   	   + +						   	   + +
	//                 9				               9
	//         + 8 6    7					   + 8 6    7
	//		 +		 4   5					 +		 4   5
	//		+         2  3					+         2  3
	//		+    +    0  1					+    +    0  1
	//		+         +						+         +
	//		 +       +						 +       +
	//         + + +					       + + +
	//
	//		T0(0,1,3)						T0(0,3,1)
	//		T1(0,3,2)						T1(0,2,3)
	//
	idvertex = firstvertex;
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_NUTSCD_TRIANGLE_CW_BUILDING))
	{
		for (i = crea_vertexpair - 1; i != 0; i--, idvertex += 2)
		{
			triangle.i0 = idvertex;
			triangle.i1 = idvertex + 3;
			triangle.i2 = idvertex + 1;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);

			triangle.i0 = idvertex;
			triangle.i1 = idvertex + 2;
			triangle.i2 = idvertex + 3;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
		// close or Open Ring ?
		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES))
		{
			triangle.i0 = idvertex;
			triangle.i1 = firstvertex + 1;
			triangle.i2 = idvertex + 1;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);

			triangle.i0 = idvertex;
			triangle.i1 = firstvertex;
			triangle.i2 = firstvertex + 1;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
	}
	else // CCW (default)
	{
		for (i = crea_vertexpair - 1; i != 0; i--, idvertex += 2)
		{
			triangle.i0 = idvertex;
			triangle.i1 = idvertex + 1;
			triangle.i2 = idvertex + 3;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);

			triangle.i0 = idvertex;
			triangle.i1 = idvertex + 3;
			triangle.i2 = idvertex + 2;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
		// close or Open Ring ?
		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES))
		{
			triangle.i0 = idvertex;
			triangle.i1 = idvertex + 1;
			triangle.i2 = firstvertex + 1;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);

			triangle.i0 = idvertex;
			triangle.i1 = firstvertex + 1;
			triangle.i2 = firstvertex;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
	}
}

// ------------------------------------------------------------------------------------------
// NUT_InsertDisc
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a Disc (2D) into an already created geometry
// ------------------------------------------------------------------------------------------
// In	:
//			pgeom:	a valid pointer on a geometry
//			dec:	a valid pointer on a disc description structure
// Out :
//
// ------------------------------------------------------------------------------------------
// Notes about NUT_DISC_DESC parameters:
//		Nu32						flags
//										FLAG_DISC_DESC_USEPARAM_SCRUV
//														Set it if you want to use the 'scrUV' Param.
//
//										FLAG_DISC_DESC_USEPARAM_UV
//														Set it if you want to use the 'UV' Param.
//														Note that FLAG_USEPARAM_SCRUV and FLAG_USEPARAM_UV can be used together.
//														In that case, uvs work inside the scrUV rect.
//
//										FLAG_DISC_DESC_USEPARAM_CONSTRUCTIONORIGIN
//														Set it if you want to use the 'Construction Origin' Param.
//														If not. figure is going to be build around Default Origin( 0,0,0 ).
//
//										FLAG_NUTSCD_USEPARAM_ANCHOR
//														Set it if you want to use the 'Anchor' Param.
//														If not. Figure anchor point is going to be the default one (TOP,LEFT,BACK).
//
//										FLAG_NUTSCD_USEPARAM_SCALE
//														Set it if you want to use the 'Scale' Param.
//														There is 2 ways to define figure size, "from texture" or "from Size"
//														In both cases, final quad size is going to be multiply by this value before figure creation.
//
//										FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE
//														Set it if you want to compute figure size from linked texture size.
//														if not, by default,Size (x and y)  parameters are going to be used to define Figure Size.
//
//										FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE
//														Set it if you want to use the 'construction_plane' Param.
//
//										FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR
//														Set it if you want to use the 'ConstructionPlane_DirectorVector' Param.
//														If the 2 flags 'FLAG_USEPARAM_CONSTRUCTIONPLANE and 'FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR'
//														are set to ON, only 'FLAG_USEPARAM_CONSTRUCTIONPLANE' is used.
//
//														If none of them is set to ON, function is going to take the 'NUT_PLANE_XY' as a default construction_plane
//														to work.
//
//										FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES
//														Set it if you want to use the 'OpenDisc_FromAngle' and 'OpenDisc_ToAngle' Param.
//														In that case an open disc is going to be created ( like the PACMAN 'figure' )
//														All the disc vertex will be created from FromAngle to ToAngle ...
//														AND the last created vertex is going to stay UNWELD from the first one even if
//														FromAngle = 0� and ToAngle=360�.
//														If the flag is set to OFF a full 360� closed disc is going to be created and first
//														and last circumference vertex are going to be weld !
//
//		NUT_CONSTRUCTION_PLANE		construction_plane	To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONPLANE.
//										NUT_PLANE_XY	Cristal clear ...
//										NUT_PLANE_XZ	Cristal clear ...
//										NUT_PLANE_YZ	Cristal clear ...
//
//		NVEC3					ConstructionPlane_DirectorVector;
//														Cristal clear ...
//														To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR.
//
//		NVEC2					Size				Figure Size obviously (Width and Height). Used only if FLAG_QUADSIZE_FROM_TEXTURESIZE is set to OFF
//
//		NRECTs16				scrUV				UV rect in screen coord system (pixel). Used to get texture rect into ATLAS texture.
//														To compute final UVs coordinates a valid texture has to be already bind with the geometry.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_SCRUV.
//
//		NRECTf32					UV					UV Rect in normalized format (0.0f < uv < 1.0f ). BUT Can be greater than 1.0f for Tiling.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_UV.
//
//														Because user can ask for using 'scrUV' (FLAG_USEPARAM_SCRUV) and 'UV' (FLAG_USEPARAM_UV) at the same time
//														final uvs are the result of the right combination of both parameters.
//
//
//		NVEC3					ConstructionOrigin	Absolute position of the building reference point. 0,0,0 by default.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_CONSTRUCTIONORIGIN.
//
//		NVEC3					Anchor				Relative Anchor point... Which point of the Figure is going to be place at the "Construction Origin" ...
//														0,0,0 by default, that means the top left corner of the figure (Ring). To use it, set the flag FLAG_USEPARAM_ANCHOR.
//
//
//		NVEC2					Scale				There is 2 ways to define figure size, "from texture" or "from Width & Height definition"
//														In both cases, final quad size is going to be multiply by this value before quad creation.
//														To use it, set the flag FLAG_NUTSCD_USEPARAM_SCALE.
//
//		Nf32						OpenDisc_FromAngle	To create an open disc (like a PACMAN figure) and define the starting circumference vertex position.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES.
//		Nf32						OpenDisc_ToAngle	To create an open disc (like a PACMAN figure) and define the starting circumference vertex position.
//														To use it, set the flag FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES.

//		Nu32						SliceNb;			Equal to the circumference number of vertex.

//		NCOLOR						ColorRange[3]		Color associated with each Vertex.
//														ColorRange[0] with disc center vertex, only this one !
//														All the disc circumference vertex are going to use a color between ColorRange[1] and ColorRange[2].
//
//		Description examples
//		--------------------
/*
			NUT_DISC_DESC	disc_desc;
			memset(&disc_desc,0,sizeof(NUT_DISC_DESC));

			// Define Disc Construction plane
			disc_desc.construction_plane	= NUT_PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
			FLAG_ON(disc_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
			// ... or like this (to use a plane director vector)
			NVec3Set(&disc_desc.ConstructionPlane_DirectorVector,0,0,1);
			FLAG_ON(disc_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR);

			// Define Disc Size
			disc_desc.Size.x	= my_width;
			disc_desc.Size.y	= my_height;
			// ... or like this (Size is not going to be used ... )
			FLAG_ON(disc_desc.flags,FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE);

			// Define Scale
			disc_desc.Scale.x	= my_scaleXvalue;
			disc_desc.Scale.y	= my_scaleYvalue;
			FLAG_ON(disc_desc.flags,FLAG_NUTSCD_USEPARAM_SCALE);

			// Define UVs
			memcpy(&disc_desc.scrUV,&my_srcuvs,sizeof(NRECTs16));
			FLAG_ON(disc_desc.flags,FLAG_DISC_DESC_USEPARAM_SCRUV);
			// ... or like this, or Both together !
			memcpy(&disc_desc.UV,&my_uvs,sizeof(NRECTf32));
			FLAG_ON(disc_desc.flags,FLAG_DISC_DESC_USEPARAM_UV);

			// Define figure building position( "Origin" )
			NVec3Copy(&disc_desc.ConstructionOrigin,&my_vector3_position);
			FLAG_ON(disc_desc.flags,FLAG_DISC_DESC_USEPARAM_CONSTRUCTIONORIGIN);

			// Define figure Anchor point ( TOP, LEFT, BACK by default )
			NVec3Copy(&disc_desc.Anchor,&my_vector3_anchor);
			FLAG_ON(disc_desc.flags,FLAG_NUTSCD_USEPARAM_ANCHOR);

			// Define Disc slices number ( equal to the number of circumference vertex )
			disc_desc.OpenDisc_FromAngle = my_number_of_disc_slice;

			// Define Open Disc 'From' and 'To' angles
			disc_desc.OpenDisc_FromAngle	= my_fromangle;
			disc_desc.OpenDisc_ToAngle		= my_toangle;
			FLAG_ON(disc_desc.flags,FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES);

			// Define Colors
			NColorRangeCopy( &disc_desc.ColorRange,&my_3_colors_range,3);
*/
// ------------------------------------------------------------------------------------------
void NUT_InsertDisc(NGEOMETRY *pgeom, const NUT_DISC_DESC *desc)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + INFOS : VERTEX Index Distribution
	// +
	//         6 5 4
	//		 7		 3
	//		8         2
	//		9    0    1
	//		+         +
	//		 +       +
	//         + + +
	//
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + INFOS : UVs
	// + Ring UVs are going to be calculated 'inside' the Texture Rectangle using the whole surface.
	// +
	// +-+ STANDARD:
	//
	//	1) Without Mirroring	2) U Mirroring Only		3) V Mirroring Only		4) U and V Mirroring
	//	   0,0 --------1,0		   0,0 +++++---1,0		   0,0 ---+----1,0		0,0 +---------1,0
	//      |   + + +   |		    |       ++  |		    +     Or    +		    |Or   	 	+
	//		| +		  + |			| 		  + |		   	+ 		    +		   	|	  	 	+
	//		|+         +|			|          +|		   	+           +		   	|		   +|
	//		|+    +Or  +|			+Or		   +|		   	|+		   +|		   	|		  +	|
	//		|+         +|			|          +|		   	| +       + |		   	|      ++   |
	//		| +       + |			|         + |		   	|  ++	++  |		   	|	+++	    |
	//      |   + + +   |		    |       ++  |		    |	 +++	|		    |+++		|
	//	   0,1 --------1,1		   0,1 +++++---1,1		   0,1 --------1,1	   (0,1)+----------1,1
	//		   DEFAULT
	//	Nu32							meshdesc	=  NGetGeometrySpecs( NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS) )->MeshDescription;
	// NUT_GEOM_UVBASE					uvbase;
	NMATRIX3x3 txc_matrix;
	NVEC2 size;
	NMATRIX bmx;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Texture Coordinates Context
	// +
	NGetBuildBaseTexCoordMatrix3x3(&txc_matrix, &desc->CoreDesc.TexCoordBuild);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Size
	// +
	// Basic incoming size extraction:
	extract_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	scale_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Build Matrix
	// +
	extract_build_matrix(&bmx, &size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + PRE-BUILDING VERTEX BUILDING PARAMETERS
	// +
	// +	Calculating some parameters accordingly with the user demands used to create vertex.
	// +
	// +--+ Because we are going to build the DISC from its center it will be easier from now to use
	//		half width and half height instead of width, height !
	//		... !!!!!!! SO FROM HERE: vwidth is HALF vwidth !
	//		... !!!!!!! SO FROM HERE: vheight is HALF vheight !
	// NVERTEX_STRIDES		*pstrides	= &NGetGeometrySpecs( NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS) )->VertexStrides;
	NVEC3 vcenter;
	NVEC3 vi, vj;
	NVEC2 uvradius_u, uvradius_v;
	NTRIANGLE triangle;
	Nu32 firstvertex, idvertex;
	Nu32 i;
	Nf32 fcos, fsin;
	Nf32 angledeg, anglestep;
	Nu32 crea_vertex;
	// 	Nf32				tcol,tcolstep;
	Nf32 fa;
	NVERTEX_V3N3C4T2 vertex_V3N3C4T2;
	NVEC3 *pvwidth = (NVEC3 *)&bmx.V4[0];
	NVEC3 *pvheight = (NVEC3 *)&bmx.V4[1];
	NVEC3 *pvdepth = (NVEC3 *)&bmx.V4[2];
	//	NVEC3			*pv0		= (NVEC3*)&bmx.V4[3];
	NVEC3 colorpos;
	// +--+ Center point
	extract_figurecenter(&vcenter, &bmx);

	// +--+ Scale Width and height to use them as Radius ...
	//		!! because pvwidth and pvheight are part of the bmx matrix, change them has to be
	//		... done at the right time !!! Absolutly after extracting figure center for example ...
	NVec3ScaleBy(pvwidth, 0.5f);
	NVec3ScaleBy(pvheight, 0.5f);

	// +--+ Angle, Angle Step and building direction (trigonometric or not)
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES))
	{
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_DISC_DESC_VERTEX_TRIGO_INVERSE_BUILDING))
		{
			angledeg = desc->OpenDisc_FromAngle;
			anglestep = _angular_sector(desc->OpenDisc_FromAngle, desc->OpenDisc_ToAngle, NFALSE) / (Nf32)desc->SliceNb;
		}
		else // Creation in the trigonometric direction ...
		{
			angledeg = desc->OpenDisc_FromAngle;
			anglestep = _angular_sector(desc->OpenDisc_FromAngle, desc->OpenDisc_ToAngle, NTRUE) / (Nf32)desc->SliceNb;
		}
		crea_vertex = desc->SliceNb + 1;
		// 		tcol		= 0.0f;
		// 		tcolstep	= 1.0f/(crea_vertex-1);
	}
	else
	{
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_DISC_DESC_VERTEX_TRIGO_INVERSE_BUILDING))
		{
			angledeg = 360.0f;
			anglestep = -360.0f / (Nf32)desc->SliceNb;
		}
		else
		{
			angledeg = 0.0f;
			anglestep = 360.0f / (Nf32)desc->SliceNb;
		}
		crea_vertex = desc->SliceNb;
		// 		tcol		= 0.0f;
		// 		tcolstep	= 1.0f/(crea_vertex-1);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + PRE-BUILDING UV BUILDING PARAMETERS
	// +
	// +	Calculating some parameters accordingly with the user demands used to create uvs.
	// +
	// +--+ UV vectors scale to be used from Circle Center.
	if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_DISC_DESC_MIRRORING_U))
	{
		// UV Base Vectors.
		NVec2ScaleBy((NVEC2 *)&txc_matrix.XAxis, 0.5f);
		// UV Origin.
		txc_matrix.Origin.x += txc_matrix.XAxis.x;
		txc_matrix.Origin.y += txc_matrix.XAxis.y;
	}

	if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_DISC_DESC_MIRRORING_V))
	{
		// UV Base Vectors.
		NVec2ScaleBy((NVEC2 *)&txc_matrix.YAxis, 0.5f);
		// UV Origin.
		txc_matrix.Origin.x += txc_matrix.YAxis.x;
		txc_matrix.Origin.y += txc_matrix.YAxis.y;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + VERTEX ATTRIBUTES INSERTION
	// +
	// +--+Get Vertex 0 index (it will be the index of the center vertex )
	firstvertex = pgeom->Mesh.VertexArray.Size;
	NUpSizeArray(&pgeom->Mesh.VertexArray, crea_vertex + 1, NULL); // crea_vertex = circumference vertex, and there is the center, ... so + 1!!!

	NBYTE *pvertex_addr = NGetArrayPtr(&pgeom->Mesh.VertexArray, firstvertex);
	Nu32 nextvertex = pgeom->Mesh.VertexArray.ElementSize;
	Nu8 colorsetid = NGetBuildBaseColorSetID(&desc->CoreDesc.ColorBuild);
	Nu8 texcoordsetid = NGetBuildBaseTexCoordSetID(&desc->CoreDesc.TexCoordBuild);

	// NVEC2	vg;
	vertex_V3N3C4T2.Normal_3f = *pvdepth; // For all inserted vertex !

	// +--+ Build Center
	// Vertex Position
	vertex_V3N3C4T2.Position_3f = vcenter;
	// COLOR
	colorpos.x = 0.5f, colorpos.y = 0.5f;
	colorpos.z = 0.0f;

	NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
								  0, crea_vertex + 1,
								  (Nf32 *)&colorpos,
								  &desc->CoreDesc.ColorBuild);
	// Vertex UV
	// (Due to previous setup U and V Mirroring are "integrated" into "uv_origin" )
	vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x;
	vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y;

	// NCopyVertex(pvertex_addr, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),&vertex_V3N3C4T2, NVERTEX_FORMAT_V3N3C4T2);
	_set_nut_shape_vertex_data(pvertex_addr, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
	pvertex_addr += nextvertex;

	// All the circumference vertex

	for (i = 0; i < crea_vertex; i++, angledeg += anglestep, pvertex_addr += nextvertex)
	{
		NFastCosSin(NDEGtoANGLE(angledeg), &fcos, &fsin);

		NVec3Scale(&vi, pvwidth, fcos);
		NVec3Scale(&vj, pvheight, fsin);
		NVec2Scale(&uvradius_u, (NVEC2 *)&txc_matrix.XAxis, fcos);
		NVec2Scale(&uvradius_v, (NVEC2 *)&txc_matrix.YAxis, fsin);
		// Vertex Position
		vertex_V3N3C4T2.Position_3f.x = vcenter.x + vi.x + vj.x;
		vertex_V3N3C4T2.Position_3f.y = vcenter.y + vi.y + vj.y;
		vertex_V3N3C4T2.Position_3f.z = vcenter.z + vi.z + vj.z;

		// COLOR
		colorpos.x = fcos * 0.5f + 0.5f;
		colorpos.y = 0.5f - fsin * 0.5f;
		// colorpos.z = 0.0f; allready done once before loop
		NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
									  i, crea_vertex,
									  (Nf32 *)&colorpos,
									  &desc->CoreDesc.ColorBuild);

		/*
				if( ISFLAG_ON(desc->Flags,FLAG_NUTSCD_USEPARAM_COLORGRADIENTID) )
				{
					vg.x = fcos*0.5f+0.5f; vg.y = 0.5f-fsin*0.5f;
					NExtractGradientColor(&vertex_V3N3C4T2.Color0_4f,pcolrange,colrangesize,&vg,desc->CoreDesc.ColorGradientID);
				}
				else
				{
					NLerpColorRange(&vertex_V3N3C4T2.Color0_4f,pcolrange,colrangesize,(Nf32)i/(crea_vertex-1),NCOLORRANGE_LERP_CLAMP);
				}

		*/
		// Vertex UV
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_DISC_DESC_MIRRORING_U))
			fa = NABS(fcos);
		else
			fa = fcos;
		NVec2Scale(&uvradius_u, (NVEC2 *)&txc_matrix.XAxis, fa);

		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_DISC_DESC_MIRRORING_V))
			fa = NABS(fsin);
		else
			fa = fsin;
		NVec2Scale(&uvradius_v, (NVEC2 *)&txc_matrix.YAxis, fa);

		vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x + uvradius_u.x + uvradius_v.x;
		vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y + uvradius_u.y + uvradius_v.y;

		// NCopyVertex(pvertex_addr, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),&vertex_V3N3C4T2, NVERTEX_FORMAT_V3N3C4T2);
		_set_nut_shape_vertex_data(pvertex_addr, NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
	}

	// All the Disc Primitives
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + VERTEX winding Order						 VERTEX winding Order
	// +		CCW											CW
	// + (follow index distribution order)
	// +
	//         6 5 4								       6 5 4
	//		 7		 3									 7		 3
	//		8         2									8         2
	//		9    0    1									9    0    1
	//		+         +									+         +
	//		 +       +									 +       +
	//         + + +								       + + +
	//
	//		T0(0,1,2)									T0(0,2,1)
	//		T1(0,2,3)									T1(0,3,2)
	//
	idvertex = firstvertex + 1; // due to center ( which is firstvertex)
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_NUTSCD_TRIANGLE_CW_BUILDING))
	{
		for (i = crea_vertex - 1; i != 0; i--, idvertex++)
		{
			triangle.i0 = firstvertex; // disc center
			triangle.i1 = idvertex + 1;
			triangle.i2 = idvertex;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}

		// Open or Close Disc ?
		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES))
		{
			triangle.i0 = firstvertex;	   // disc center
			triangle.i1 = firstvertex + 1; // circumference vertex 0
			triangle.i2 = idvertex;		   // ... the last circumference vertex
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
	}
	else // CCW (default)
	{
		for (i = crea_vertex - 1; i != 0; i--, idvertex++)
		{
			triangle.i0 = firstvertex; // disc center
			triangle.i1 = idvertex;
			triangle.i2 = idvertex + 1;
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}

		// Open or Close Disc ?
		if (ISFLAG_OFF(desc->CoreDesc.Flags, FLAG_DISC_DESC_USEPARAM_OPENDISC_ANGLES))
		{
			triangle.i0 = firstvertex;	   // disc center
			triangle.i1 = idvertex;		   // ... the last circumference vertex
			triangle.i2 = firstvertex + 1; // circumference vertex 0
			NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		}
	}
}

// ------------------------------------------------------------------------------------------
// NUT_InsertQuad
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a Quad into an already created geometry
// ------------------------------------------------------------------------------------------
// In	:
//			pgeom:	a valid pointer on a geometry
//			dec:	a valid pointer on a quad description structure
// Out :
//
// ------------------------------------------------------------------------------------------
// Notes about NUT_DESC_QUAD parameters:
//		Nu32						flags
//										FLAG_USEPARAM_SCRUV
//														Set it if you want to use the 'scrUV' Param.
//
//										FLAG_USEPARAM_UV
//														Set it if you want to use the 'UV' Param.
//														Note that FLAG_USEPARAM_SCRUV and FLAG_USEPARAM_UV can be used together.
//														In that case, uvs work inside the scrUV rect.
//
//										FLAG_USEPARAM_CONSTRUCTIONORIGIN
//														Set it if you want to use the 'Construction Origin' Param.
//														If not. figure is going to be build around Default Origin( 0,0,0 ).
//
//										FLAG_USEPARAM_ANCHOR
//														Set it if you want to use the 'Anchor' Param.
//														If not. Figure anchor point is going to be the default one (TOP,LEFT,BACK).
//
//										FLAG_USEPARAM_SCALE
//														Set it if you want to use the 'Scale' Param.
//														There is 2 ways to define figure size, "from texture" or "from Size"
//														In both cases, final quad size is going to be multiply by this value before figure creation.
//
//										FLAG_FIGURESIZE_FROM_TEXTURESIZE
//														Set it if you want to compute figure size from linked texture size.
//														if not, by default,Size (x and y)  parameters are going to be used to define Figure Size.
//
//										FLAG_USEPARAM_CONSTRUCTIONPLANE
//														Set it if you want to use the 'construction_plane' Param.
//
//										FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR
//														Set it if you want to use the 'ConstructionPlane_DirectorVector' Param.
//														If the 2 flags 'FLAG_USEPARAM_CONSTRUCTIONPLANE and 'FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR'
//														are set to ON, only 'FLAG_USEPARAM_CONSTRUCTIONPLANE' is used.
//
//														If none of them is set to ON, function is going to take the 'NUT_PLANE_XY' as a default construction_plane
//														to work.
//
//		NUT_CONSTRUCTION_PLANE		construction_plane	To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONPLANE.
//										NUT_PLANE_XY	Cristal clear ...
//										NUT_PLANE_XZ	Cristal clear ...
//										NUT_PLANE_YZ	Cristal clear ...
//
//		NVEC3					ConstructionPlane_DirectorVector;
//														Cristal clear ...
//														To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR.
//
//		NVEC2					Size				Figure Size obviously (Width and Height). Used only if FLAG_QUADSIZE_FROM_TEXTURESIZE is set to OFF
//
//		NRECTs16				scrUV				UV rect in screen coord system (pixel). Used to get texture rect into ATLAS texture.
//														To compute final UVs coordinates a valid texture has to be already bind with the geometry.
//														To use it, set the flag FLAG_USEPARAM_SCRUV.
//
//		NRECTf32					UV					UV Rect in normalized format (0.0f < uv < 1.0f ). BUT Can be greater than 1.0f for Tiling.
//														To use it, set the flag FLAG_USEPARAM_UV.
//
//														Because user can ask for using 'scrUV' (FLAG_USEPARAM_SCRUV) and 'UV' (FLAG_USEPARAM_UV) at the same time
//														final uvs are the result of the right combination of both parameters.
//
//
//		NVEC3					ConstructionOrigin	Absolute position of the building reference point. 0,0,0 by default.
//														To use it, set the flag FLAG_USEPARAM_CONSTRUCTIONORIGIN.
//
//		NVEC3					Anchor				Relative Anchor point... Which point of the Figure is going to be place at the "Construction Origin" ...
//														0,0,0 by default, that means the top left corner of the figure (Ring). To use it, set the flag FLAG_USEPARAM_ANCHOR.
//
//
//		NVEC2					Scale				There is 2 ways to define figure size, "from texture" or "from Width & Height definition"
//														In both cases, final quad size is going to be multiply by this value before quad creation.
//														To use it, set the flag FLAG_USEPARAM_SCALE.
//
//		NCOLOR						ColorRange[4]		Color associated with each Vertex. ColorRange[0] with Vertex 0, etc ...
//														Top-Left Vertex is Vertex 0, the other are build in the clockwise order.
//
//		Description examples
//		--------------------
/*
			NUT_QUAD_DESC	quad_desc;
			memset(&quad_desc,0,sizeof(NUT_QUAD_DESC));

			// Define quad orientation
			quad_desc.construction_plane	= NUT_PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
			// ... or like this (to use a plane director vector)
			NVec3Set(&quad_desc.ConstructionPlane_DirectorVector,0,0,1);
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR);

			// Define quad Size
			quad_desc.Size.x	= my_width;
			quad_desc.Size.y	= my_height;
			// ... or like this (Size is not going to be used ... )
			FLAG_ON(quad_desc.flags,FLAG_QUAD_DESC_QUADSIZE_FROM_TEXTURESIZE);

			// Define Scale
			quad_desc.Scale.x	= my_scaleXvalue;
			quad_desc.Scale.y	= my_scaleYvalue;
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_SCALE);

			// Define UVs
			memcpy(&quad_desc.scrUV,&my_srcuvs,sizeof(NRECTs16));
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_SCRUV_RECT);
			// ... or like this, or Both together !
			memcpy(&quad_desc.UV,&my_uvs,sizeof(NRECTf32));
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_UV_RECT);

			// Define quad building position( "Origin" )
			NVec3Copy(&quad_desc.ConstructionOrigin,&my_vector3_position);
			FLAG_ON(quad_desc.flags,FLAG_QUAD_DESC_USEPARAM_CONSTRUCTIONORIGIN);

			// Define quad Anchor point ( TOP, LEFT by default )
			NVec3Copy(&quad_desc.Anchor,&my_vector3_anchor);
			FLAG_ON(quad_desc.flags,FLAG_NUTSCD_USEPARAM_ANCHOR);

			// Define Colors
			NColorRangeCopy( &quad_desc.ColorRange,&my_4_colors_range,4);
*/
// ------------------------------------------------------------------------------------------
void NUT_InsertQuad(NGEOMETRY *pgeom, const NUT_QUAD_DESC *desc)
{
	//	Vertex index distribution CCW
	//		0 ------- 3
	//		| .       |
	//		|    +    |
	//		|       . |
	//		1 ------- 2
	//	UV
	//  	0,0 ------1,0
	//		 | .       |
	//		 |    +    |
	//		 |       . |
	//  	0,1 ------1,1

	// ----------------------------------------------------------------------------------------
	NMATRIX3x3 txc_matrix;
	NVEC2 size;
	NMATRIX bmx;
	Nu32 i;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Texture Coordinates Context
	// +
	NGetBuildBaseTexCoordMatrix3x3(&txc_matrix, &desc->CoreDesc.TexCoordBuild);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Size
	// +
	extract_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	scale_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Build Matrix
	// +
	extract_build_matrix(&bmx, &size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ----------------------------------------------------------------------------------------
	// VERTEX ATTRIBUTES INSERTION
	// ----------------------------------------------------------------------------------------
	//	NVERTEX_STRIDES	*pstrides	= &NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS))->VertexStrides;
	NVEC3 *pvwidth = (NVEC3 *)&bmx.V4[0];
	NVEC3 *pvheight = (NVEC3 *)&bmx.V4[1];
	NVEC3 *pvdepth = (NVEC3 *)&bmx.V4[2];
	NVEC3 *pv0 = (NVEC3 *)&bmx.V4[3];
	NVEC2 *puv0 = (NVEC2 *)&txc_matrix.Origin;

	// NVERTEX_DESC	vdata;
	NVERTEX_V3N3C4T2 vertex_V3N3C4T2;

	NTRIANGLE triangle;
	Nu32 firstvertex;
	NVEC3 colorposition;
	Nu8 colorsetid = NGetBuildBaseColorSetID(&desc->CoreDesc.ColorBuild);
	Nu8 texcoordsetid = NGetBuildBaseTexCoordSetID(&desc->CoreDesc.TexCoordBuild);

	// Setup shifting table
	NVEC2 shifting[4] = {{0.0f, 0.0f},
						 {0.0f, -1.0f},
						 {1.0f, -1.0f},
						 {1.0f, 0.0f}};

	// Get Vertex 0 index
	firstvertex = NGetArraySize(&pgeom->Mesh.VertexArray);
	// Add 4 vertex
	NUpSizeArray(&pgeom->Mesh.VertexArray, 4, NULL);

	// --------------------------------------------------------------------------------------------------------------------------------------
	for (i = 0; i < 4; i++)
	{
		// Position
		vertex_V3N3C4T2.Position_3f.x = pv0->x + shifting[i].x * pvwidth->x + shifting[i].y * pvheight->x;
		vertex_V3N3C4T2.Position_3f.y = pv0->y + shifting[i].x * pvwidth->y + shifting[i].y * pvheight->y;
		vertex_V3N3C4T2.Position_3f.z = pv0->z + shifting[i].x * pvwidth->z + shifting[i].y * pvheight->z;
		// Normal
		vertex_V3N3C4T2.Normal_3f = *pvdepth; // or use the opposite, ... lets check !
		// UV
		vertex_V3N3C4T2.TexCoord0_2f.x = puv0->x + shifting[i].x * txc_matrix.XAxis.x - shifting[i].y * txc_matrix.YAxis.x; // Notice "- shifting[i].y" and not "+" it's because we are using the same shifting data for position
		vertex_V3N3C4T2.TexCoord0_2f.y = puv0->y + shifting[i].x * txc_matrix.XAxis.y - shifting[i].y * txc_matrix.YAxis.y; // and for UV, and UV bas is WEIRD inverted about Y ...
		// COLOR
		colorposition.x = shifting[i].x;
		colorposition.y = -shifting[i].y; // Because color position is working like UV position into the WEIRD base
		colorposition.z = 0.0f;

		NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
									  i, 4,
									  (Nf32 *)&colorposition,
									  &desc->CoreDesc.ColorBuild);

		// DONE
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray, firstvertex + i), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
	}
	/*
		// Vertex 0 (Top Left)
		vertex_V3N3C4T2.Position_3f		= *pv0;
		vertex_V3N3C4T2.Normal_3f		= *pvdepth;	// or use the opposite, ... lets check !
		vertex_V3N3C4T2.TexCoord0_2f.x	= txc_matrix.Translation.x;
		vertex_V3N3C4T2.TexCoord0_2f.y	= txc_matrix.Translation.y;
		// COLOR
		colorposition.x = 0.0f;colorposition.y = 0.0f;colorposition.z = 0.0f; // Color position base follows the same rules than texcoords base ... invertex YAxis.
		NGetBuildBaseColorVertexColor(	&vertex_V3N3C4T2.Color0_4f,
										0,4,
										(Nf32*)&colorposition,
										&desc->CoreDesc.ColorBuild );

	// 	NCopyVertex(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),
	// 				&vertex_V3N3C4T2, NVERTEX_FORMAT_V3N3C4T2 );
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex),NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2 );

		// Vertex 1 (Bottom Left)
		NVec3SubFrom(&vertex_V3N3C4T2.Position_3f,pvheight);
		//vdata.Normal_3f		= vdepth;	// or the opposite lets check !
		vertex_V3N3C4T2.TexCoord0_2f.x		+= txc_matrix.YAxis.x;
		vertex_V3N3C4T2.TexCoord0_2f.y		+= txc_matrix.YAxis.y;
		// COLOR
		colorposition.x = 0.0f;colorposition.y = 1.0f;colorposition.z = 0.0f; // Color position base follows the same rules than texcoords base ... invertex YAxis.
		NGetBuildBaseColorVertexColor(	&vertex_V3N3C4T2.Color0_4f,
										1,4,
										(Nf32*)&colorposition,
										&desc->CoreDesc.ColorBuild );

		//NCopyVertex(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex+1), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),
		//			  &vertex_V3N3C4T2, NVERTEX_FORMAT_V3N3C4T2 );
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex+1),NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2 );

		// Vertex 2
		NVec3AddTo(&vertex_V3N3C4T2.Position_3f,pvwidth);
		//vdata.Normal_3f		= vdepth;	// or the opposite lets check !
		vertex_V3N3C4T2.TexCoord0_2f.x		+= txc_matrix.XAxis.x;
		vertex_V3N3C4T2.TexCoord0_2f.y		+= txc_matrix.XAxis.y;
		// COLOR
		colorposition.x = 1.0f;colorposition.y = 1.0f;colorposition.z = 0.0f; // Color position base follows the same rules than texcoords base ... invertex YAxis.
		NGetBuildBaseColorVertexColor(	&vertex_V3N3C4T2.Color0_4f,
										2,4,
										(Nf32*)&colorposition,
										&desc->CoreDesc.ColorBuild );
	// 	NCopyVertex(	NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex+2), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),
	// 					&vertex_V3N3C4T2, NVERTEX_FORMAT_V3N3C4T2 );
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex+2),NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2 );

		// Vertex 3
		NVec3AddTo(&vertex_V3N3C4T2.Position_3f,pvheight);
		//vdata.Normal_3f		= vdepth;	// or the opposite lets check !
		vertex_V3N3C4T2.TexCoord0_2f.x		-= txc_matrix.YAxis.x;
		vertex_V3N3C4T2.TexCoord0_2f.y		-= txc_matrix.YAxis.y;
		// COLOR
		colorposition.x = 1.0f;colorposition.y = 0.0f;colorposition.z = 0.0f; // Color position base follows the same rules than texcoords base ... invertex YAxis.
		NGetBuildBaseColorVertexColor(	&vertex_V3N3C4T2.Color0_4f,
										3,4,
										(Nf32*)&colorposition,
										&desc->CoreDesc.ColorBuild );
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray,firstvertex+3),NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2 );
	*/

	// Primitives
	//	CCW Winding Order			CW Winding Order
	//		0 ----- 3					0 ----- 3
	//		| .  T0 |					| .  T0 |
	//		|   +   |					|   +   |
	//		| T1  . |					| T1  . |
	//		1 ----- 2					1 ----- 2
	//	T0(2,3,0)					T0(2,0,3)
	//	T1(2,0,1)					T1(2,1,0)
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_NUTSCD_TRIANGLE_CW_BUILDING))
	{
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex;
		triangle.i2 = firstvertex + 3;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex + 1;
		triangle.i2 = firstvertex;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
	}
	else // CCW (default)
	{
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex + 3;
		triangle.i2 = firstvertex;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex;
		triangle.i2 = firstvertex + 1;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
	}
}

void NUT_InsertSurface(NGEOMETRY *pgeom, const NUT_SURFACE_DESC *desc)
{
	//	Vertex					UV
	//		0 ------- 1			   0,0 ------1,0
	//		| .       |				| .       |
	//		|    +    |				|    +    |
	//		|       . |				|       . |
	//		3 ------- 2			   0,1 ------1,1
	// ----------------------------------------------------------------------------------------
	NMATRIX3x3 txc_matrix;
	NVEC2 size;
	NMATRIX bmx;
	Nu32 xseg, yseg;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Texture Coordinates Context
	// +
	NGetBuildBaseTexCoordMatrix3x3(&txc_matrix, &desc->CoreDesc.TexCoordBuild);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Size
	// +
	extract_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	scale_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Build Matrix
	// +
	extract_build_matrix(&bmx, &size, &desc->CoreDesc, desc->CoreDesc.Flags);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Surface Specific
	// +
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_SURFACE_DESC_USEPARAMS_SEGMENTS))
	{
		xseg = desc->XSegments;
		yseg = desc->YSegments;
	}
	else
	{
		xseg = DEFAULT_SURFACE_DESC_XSEGMENTS;
		yseg = DEFAULT_SURFACE_DESC_XSEGMENTS;
	}
	NErrorIf(!xseg || !yseg, NERROR_NULL_VALUE);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + VERTEX ATTRIBUTES INSERTION
	// +
	//	NVERTEX_STRIDES	*pstrides	= &NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS))->VertexStrides;
	NVEC3 *pvwidth = (NVEC3 *)&bmx.V4[0];
	NVEC3 *pvheight = (NVEC3 *)&bmx.V4[1];
	NVEC3 *pvdepth = (NVEC3 *)&bmx.V4[2];
	NVEC3 *pv0 = (NVEC3 *)&bmx.V4[3];
	// NVERTEX_DESC	vdata;
	NVERTEX_V3N3C4T2 vertex_V3N3C4T2;

	NTRIANGLE triangle;
	Nu32 firstvertex, vertexid;
	Nu32 vertexnb = ((xseg + 1) * (yseg + 1));
	//	Nf32			invertexnb	= 1.0f/(Nf32)vertexnb;
	Nf32 invxseg = 1.0f / (Nf32)xseg;
	Nf32 invyseg = 1.0f / (Nf32)yseg;
	Nu16 i, j;
	NVEC3 vi, vj;
	NVEC2 uv_u, uv_v;
	NVEC3 colorposition;
	Nu8 colorsetid = NGetBuildBaseColorSetID(&desc->CoreDesc.ColorBuild);
	Nu8 texcoordsetid = NGetBuildBaseTexCoordSetID(&desc->CoreDesc.TexCoordBuild);

	// + Get Vertex 0 index
	vertexid = firstvertex = NGetArraySize(&pgeom->Mesh.VertexArray);

	// + Add all vertex
	// +
	// + Vertex Distribution ...
	// +
	//		0 ------- 1 ------  2 ------- 3 ------ n-1 -----> I
	//		| .       |			|		  |         |
	//		|    .    |			|         |         |
	//		|       . |			|         |         |
	//	    n ------ n+1 ----- n+2------ n+3----- 2n-1
	//		|         |			|         |         |
	//		|         |			|         |         |
	//		|         |			|         |         |
	//	   2n ----- 2n+1 -----2n+2------2n+3----- 3n-1
	//		|         |			|         |         |
	//		|         |			|         |         |
	//		|         |			|         |         |
	//	   3n ----- 3n+1 -----3n+3------3n+4----- 4n-1
	//		.		  .			.		  .			.
	//		.		  .			.		  .			.
	//		.		  .			.		  .			.
	//	    .
	//		.
	//		J
	NUpSizeArray(&pgeom->Mesh.VertexArray, vertexnb, NULL);
	colorposition.z = 0.0f;
	for (j = 0; j <= yseg; j++)
	{
		NVec3Scale(&vj, pvheight, (Nf32)j * invyseg);
		NVec2Scale(&uv_v, (NVEC2 *)&txc_matrix.YAxis, (Nf32)j * invyseg);
		for (i = 0; i <= xseg; i++, vertexid++)
		{
			NVec3Scale(&vi, pvwidth, (Nf32)i * invxseg);
			NVec2Scale(&uv_u, (NVEC2 *)&txc_matrix.XAxis, (Nf32)i * invxseg);
			vertex_V3N3C4T2.Position_3f.x = pv0->x + vi.x - vj.x;
			vertex_V3N3C4T2.Position_3f.y = pv0->y + vi.y - vj.y;
			vertex_V3N3C4T2.Position_3f.z = pv0->z + vi.z - vj.z;

			vertex_V3N3C4T2.Normal_3f = *pvdepth; // or use the opposite, ... lets check !

			vertex_V3N3C4T2.TexCoord0_2f.x = txc_matrix.Origin.x + uv_u.x + uv_v.x;
			vertex_V3N3C4T2.TexCoord0_2f.y = txc_matrix.Origin.y + uv_u.y + uv_v.y;
			// COLOR
			colorposition.x = vertex_V3N3C4T2.TexCoord0_2f.x;
			colorposition.y = vertex_V3N3C4T2.TexCoord0_2f.y;
			// colorposition.z = 0.0f; allready done just before the loops.
			NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
										  vertexid, vertexnb,
										  (Nf32 *)&colorposition,
										  &desc->CoreDesc.ColorBuild);
			// 			NCopyVertex(NGetArrayPtr(&pgeom->Mesh.VertexArray,vertexid), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS),
			// 						&vertex_V3N3C4T2,NVERTEX_FORMAT_V3N3C4T2);
			_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray, vertexid), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + PRIMITIVES INSERTION
	// +
	//	CCW Winding Order			CW Winding Order
	//		0 ----- 1					0 ----- 1
	//		| .  T0 |					| .  T0 |
	//		|   +   |					|   +   |
	//		| T1  . |					| T1  . |
	//		K ----- K+1					K ----- K+1
	//
	//		K = 0+(nb horizontal vertex)*(line_index+1)
	//
	//		T0(K+1,1,0)					T0(K+1,0,1)
	//		T1(K+1,0,K)					T1(K+1,K,0)
	//
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_NUTSCD_TRIANGLE_CW_BUILDING))
	{
		for (j = 0; j < yseg; j++)
		{
			for (i = 0; i < xseg; i++)
			{
				triangle.i0 = firstvertex + i + (j + 1) * (xseg + 1) + 1;
				triangle.i1 = firstvertex + i;
				triangle.i2 = firstvertex + i + 1;
				NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
				// triangle.i0 = firstvertex + i + (j+1)*(xseg+1)+1; !!!  already set to that value
				triangle.i1 = firstvertex + i + (j + 1) * (xseg + 1);
				triangle.i2 = firstvertex + i;
				NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
			}
		}
	}
	else // CCW (default)
	{
		for (j = 0; j < yseg; j++)
		{
			for (i = 0; i < xseg; i++)
			{
				triangle.i0 = firstvertex + i + (j + 1) * (xseg + 1) + 1;
				triangle.i1 = firstvertex + i + 1;
				triangle.i2 = firstvertex + i;
				NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
				// triangle.i0 = firstvertex + i + (j+1)*(xseg+1)+1; !!!  already set to that value
				triangle.i1 = firstvertex + i;
				triangle.i2 = firstvertex + i + (j + 1) * (xseg + 1);
				NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
			}
		}
	}
}

void NUT_InsertHollowQuad(NGEOMETRY *pgeom, const NUT_HOLLOW_QUAD_DESC *desc)
{
	//	Vertex index distribution CCW
	//		0 ----------------- 3
	//		| .				  .	|
	//		|	4 --------- 7	|
	//		|	|			|	|
	//		|	|			|	|
	//		|	|			|	|
	//		|	|			|	|
	//		|   5 --------- 6	|
	//		| .				  .	|
	//		1 ----------------- 2
	// ----------------------------------------------------------------------------------------
	NMATRIX3x3 txc_matrix;
	NVEC2 size;
	NMATRIX bmx;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Texture Coordinates Context
	// +
	NGetBuildBaseTexCoordMatrix3x3(&txc_matrix, &desc->CoreDesc.TexCoordBuild);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Size
	// +
	extract_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	scale_size(&size, &desc->CoreDesc, desc->CoreDesc.Flags);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + EXTRACTION DATA: Build Matrix
	// +
	extract_build_matrix(&bmx, &size, &desc->CoreDesc, desc->CoreDesc.Flags);
	// ----------------------------------------------------------------------------------------
	// VERTEX ATTRIBUTES INSERTION
	// ----------------------------------------------------------------------------------------
	//	NVERTEX_STRIDES	*pstrides	= &NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS))->VertexStrides;
	NVEC3 *pvwidth = (NVEC3 *)&bmx.V4[0];
	NVEC3 *pvheight = (NVEC3 *)&bmx.V4[1];
	NVEC3 *pvdepth = (NVEC3 *)&bmx.V4[2];
	NVEC3 *pv0 = (NVEC3 *)&bmx.V4[3];
	NVEC2 *puv0 = (NVEC2 *)&txc_matrix.Origin;
	// NVERTEX_DESC	vdata;
	NVERTEX_V3N3C4T2 vertex_V3N3C4T2;

	NTRIANGLE triangle;
	Nu32 firstvertex;
	NVEC3 colorposition;
	Nu8 colorsetid = NGetBuildBaseColorSetID(&desc->CoreDesc.ColorBuild);
	Nu8 texcoordsetid = NGetBuildBaseTexCoordSetID(&desc->CoreDesc.TexCoordBuild);
	Nu32 i;
	// Get Vertex 0 index
	firstvertex = NGetArraySize(&pgeom->Mesh.VertexArray);
	// Add 8 vertex
	NUpSizeArray(&pgeom->Mesh.VertexArray, 8, NULL);

	// Setup shifting table
	Nf32 rw = (1.0f - desc->SmallSizeRatio.x) * 0.5f;
	Nf32 rh = (1.0f - desc->SmallSizeRatio.y) * 0.5f;

	NVEC2 shifting[8] = {{0.0f, 0.0f},
						 {0.0f, -1.0f},
						 {1.0f, -1.0f},
						 {1.0f, 0.0f},
						 {rw, -rh},
						 {rw, -1.0f + rh},
						 {1.0f - rw, -1.0f + rh},
						 {1.0f - rw, -rh}};

	// --------------------------------------------------------------------------------------------------------------------------------------
	for (i = 0; i < 8; i++)
	{
		// Position
		vertex_V3N3C4T2.Position_3f.x = pv0->x + shifting[i].x * pvwidth->x + shifting[i].y * pvheight->x;
		vertex_V3N3C4T2.Position_3f.y = pv0->y + shifting[i].x * pvwidth->y + shifting[i].y * pvheight->y;
		vertex_V3N3C4T2.Position_3f.z = pv0->z + shifting[i].x * pvwidth->z + shifting[i].y * pvheight->z;
		// Normal
		vertex_V3N3C4T2.Normal_3f = *pvdepth; // or use the opposite, ... lets check !
		// UV
		if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_HOLLOW_QUAD_DESC_FACEMAP))
		{
			// Is is odd ( not even, not a multiple of 2 ... )
			if (i & 1)
			{
				vertex_V3N3C4T2.TexCoord0_2f.x = puv0->x + txc_matrix.XAxis.x;
				vertex_V3N3C4T2.TexCoord0_2f.y = puv0->x + txc_matrix.XAxis.y;
			}
			else
			{
				vertex_V3N3C4T2.TexCoord0_2f = *puv0;
			}

			if (i > 3)
			{
				vertex_V3N3C4T2.TexCoord0_2f.x += txc_matrix.YAxis.x;
				vertex_V3N3C4T2.TexCoord0_2f.y += txc_matrix.YAxis.y;
			}
		}
		else
		{
			vertex_V3N3C4T2.TexCoord0_2f.x = puv0->x + shifting[i].x * txc_matrix.XAxis.x - shifting[i].y * txc_matrix.YAxis.x; // Notice "- shifting[i].y" and not "+" it's because we are using the same shifting data for position
			vertex_V3N3C4T2.TexCoord0_2f.y = puv0->y + shifting[i].x * txc_matrix.XAxis.y - shifting[i].y * txc_matrix.YAxis.y; // and for UV, and UV bas is WEIRD inverted about Y ...
		}
		// COLOR
		colorposition.x = shifting[i].x;
		colorposition.y = -shifting[i].y; // Because color position is working like UV position into the WEIRD base
		colorposition.z = 0.0f;

		NGetBuildBaseColorVertexColor(&vertex_V3N3C4T2.Color0_4f,
									  i, 8,
									  (Nf32 *)&colorposition,
									  &desc->CoreDesc.ColorBuild);

		// DONE
		_set_nut_shape_vertex_data(NGetArrayPtr(&pgeom->Mesh.VertexArray, firstvertex + i), NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS), colorsetid, texcoordsetid, &vertex_V3N3C4T2);
	}

	// Primitives
	//	CCW Winding Order			CW Winding Order
	//		0 ----- 3					0 ----- 3
	//		| .  T0 |					| .  T0 |
	//		|   +   |					|   +   |
	//		| T1  . |					| T1  . |
	//		1 ----- 2					1 ----- 2
	//	T0(2,3,0)					T0(2,0,3)
	//	T1(2,0,1)					T1(2,1,0)
	if (ISFLAG_ON(desc->CoreDesc.Flags, FLAG_NUTSCD_TRIANGLE_CW_BUILDING))
	{
		triangle.i0 = firstvertex;
		triangle.i1 = firstvertex + 4;
		triangle.i2 = firstvertex + 1;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 4;
		triangle.i1 = firstvertex + 5;
		triangle.i2 = firstvertex + 1;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 1;
		triangle.i1 = firstvertex + 5;
		triangle.i2 = firstvertex + 2;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 5;
		triangle.i1 = firstvertex + 6;
		triangle.i2 = firstvertex + 2;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex + 6;
		triangle.i2 = firstvertex + 3;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 6;
		triangle.i1 = firstvertex + 7;
		triangle.i2 = firstvertex + 3;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 3;
		triangle.i1 = firstvertex + 7;
		triangle.i2 = firstvertex + 4;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 4;
		triangle.i1 = firstvertex;
		triangle.i2 = firstvertex + 3;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
	}
	else // CCW (default)
	{
		triangle.i0 = firstvertex;
		triangle.i1 = firstvertex + 1;
		triangle.i2 = firstvertex + 4;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 4;
		triangle.i1 = firstvertex + 1;
		triangle.i2 = firstvertex + 5;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 1;
		triangle.i1 = firstvertex + 2;
		triangle.i2 = firstvertex + 5;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 5;
		triangle.i1 = firstvertex + 2;
		triangle.i2 = firstvertex + 6;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 2;
		triangle.i1 = firstvertex + 3;
		triangle.i2 = firstvertex + 6;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 6;
		triangle.i1 = firstvertex + 3;
		triangle.i2 = firstvertex + 7;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 3;
		triangle.i1 = firstvertex + 4;
		triangle.i2 = firstvertex + 7;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
		triangle.i0 = firstvertex + 4;
		triangle.i1 = firstvertex + 3;
		triangle.i2 = firstvertex;
		NArrayPushBack(&pgeom->Mesh.PrimitiveArray, (NBYTE *)&triangle);
	}
}
