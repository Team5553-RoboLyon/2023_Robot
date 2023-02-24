#include "lib/N/NCStandard.h"
#include "../Containers/NArray.h"
#include "lib/N/NErrorHandling.h"
#include "../Maths/NMatrix3x3.h"

#include "NUT_geometry.h"
#include "NUT_XMesh.h"
#include "./Maths/NUT_MathsMisc.h"

extern NVERTEX_SPECS NVertexSpecs[];

/*
static inline Nu32 _extract_texcoordsetid(const NUT_TEXCOORD_BUILDER *pbuilder)
{
	if( ISFLAG_ON(pbuilder->Common.Flags,FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TEXCOORDSET_ID) )
	{
		return pbuilder->Common.TexCoordSetId;
	}
	else
	{
		return DEFAULT_NUT_TEXTCOORD_BUILDER_TEXCOORDSET_ID;
	}
}
*/

// ------------------------------------------------------------------------------------------
// void NUT_BuildGeometryVertexTexCoords_Planar
// ------------------------------------------------------------------------------------------
// Description :
//	Build Vertex TexCoords for a range of geometry vertex. These TexCoords are computed
//	thanks to a planar projection.
// ------------------------------------------------------------------------------------------
// In :
//		NGEOMETRY					*pgeom					A valid Pointer on a Geometry
//
//		const Nu32					first_vertex_index		index of the first vertex of the build vertex TexCoord range
//		const Nu32					vertex_number			Number of vertex in the build vertex TexCoord range
//		const Nu32					first_triangle_index	index of the first triangle in the build triangle range
//		const Nu32					triangle_number			Number of triangle in the build triangle range
//															To be "in range" and has its texture coordinates calculated,
//															a vertex as to be inside the defined vertex range and has to be used
//															at least by one of the triangle of the defined triangles range.
//															These 4 values are restrictives to each others.
//
//															So if user doesn't care about triangle he should call this function with
//															"0" and "pgeom->PrimitiveArray.Size" for triangle first and range size parameters.
//															And define the vertex first and range size parameters in the right way.
//
//															Same strategy if user only care about triangle. User should call this function
//															with "0" and "pgeom->VertexArray.Size" for vertex first and range size parameters.
//															And define the Triangle first and range size parameters in the right way.
//
//		NUT_TEXCOORD_BUILDER_PLANAR	*pbuilder				A Valid Pointer on a NUT_TEXCOORD_BUILDER structure
//
// Out: Nothing
// ------------------------------------------------------------------------------------------
void NUT_BuildGeometryVertexTexCoords_Planar(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER_PLANAR *pbuilder)
{
	NVEC3 *pv;
	NBYTE *ptc;
	NVEC3 tc;
	NVEC3 vdir;
	Nu32 n;
	Nu32 vstride;
	Nu32 texcoordsetid;
	NVEC3 *pbuffer;
	NVEC3 *pvb;

	NMATRIX3x3 matrix;
	NMATRIX txc_bmat;
	Nu32 vertex_flags;
	Nu32 *pnu32bitfield;
	NTRIANGLE *ptri;

	NAABB geom_aabb;
	NVEC3 geom_center;
	NVEC3 geom_size;
	NVEC3 txc_volume_center, txc_volume_size;
	Nf32 r;
	Nu32 workingflags = 0;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CHECKS
	// +
	NErrorIf(pbuilder->Mode != NUT_TEXCOORD_BUILD_MODE_PLANAR, NERROR_INCONSISTENT_PARAMETERS);
	NErrorIf(first_vertex_index + vertex_number > pgeom->Mesh.VertexArray.Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	NErrorIf(first_triangle_index + triangle_number > pgeom->Mesh.PrimitiveArray.Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);

	// TxCAABB or (TxCSize and/or TxCCenter), use both of them at the same time is WRONG.
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCAABB) &&		 // Use TxC AABB
				 (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE) || // Use TxC Size
				  ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER)) // Use TxC Center
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	// Only One FIT FLAG at a time !!!
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X) &&												  // FIT X is set to ON
				 !FLAGS_TEST(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y | FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z, 0) // one or both FIT_Y and FIT_Z is/are set to ON
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y) &&												  // FIT Y is set to ON
				 !FLAGS_TEST(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X | FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z, 0) // one or both FIT_X and FIT_Z is/are set to ON
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z) &&												  // FIT X is set to ON
				 !FLAGS_TEST(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y | FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X, 0) // one or both FIT_Y and FIT_X is/are set to ON
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	// FIT FLAG need something to fit ... ( a TxC Volume description ! )
	NErrorIf(!FLAGS_TEST(pbuilder->Flags, MASK_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_XYZ, 0) &&	 // one of the FIT X,Y, Z is set to ON
				 ISFLAG_OFF(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE) && // TXCSize is not used
				 ISFLAG_OFF(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCAABB)	 // TXCAABB is not used
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	// ALIGN FLAG need something to (re)align ... ( a TxC Volume center description ! )
	NErrorIf(!FLAGS_TEST(pbuilder->Flags, MASK_NUT_TEXTCOORD_BUILDER_TXCSIZE_ALIGN_XYZ, 0) &&  // some of the ALIGN X,Y, Z is/are set to ON
				 ISFLAG_OFF(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER) && // TXCCenter is not used
				 ISFLAG_OFF(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCAABB)	   // TXCAABB is not used
			 ,
			 NERROR_INCONSISTENT_FLAGS);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TexCoord Set ID
	// +   ( Vertex may have several TexCoord Set - So, we need to know the one to use )
	texcoordsetid = NGetBuildBaseTexCoordSetID(&pbuilder->TexCoordBuild);
	vertex_flags = NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags;

	NErrorIf(texcoordsetid > 1, NERROR_GEOMETRY_INCONSISTENT_PARAMETER);
	NErrorIf(texcoordsetid == 0 && !NCHECK_VERTEX_SPECS_TEXCOORD0(vertex_flags), NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	NErrorIf(texcoordsetid == 1 && !NCHECK_VERTEX_SPECS_TEXCOORD0(vertex_flags), NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TexCoord 3f or 2f ? 3s or 2s ? ... CONSISTENCY CHECKS ONLY ...
	// +
	// Short not supported yet ... TODO : support them !
	NErrorIf(NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(vertex_flags), NERROR_SYSTEM_TODO);
	NErrorIf(NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(vertex_flags), NERROR_SYSTEM_TODO);

	// If User asks building W component. Incoming Geometry Vertex Format need to support 3 TexCoords components...
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W) && NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2f(vertex_flags), NERROR_INCONSISTENT_FLAGS);
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W) && NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(vertex_flags), NERROR_INCONSISTENT_FLAGS);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TexCoord Build Matrix ( 3D Matrix 4x4 )
	// +
	NGetBuildBaseTexCoordMatrix4x4(&txc_bmat, &pbuilder->TexCoordBuild);

	// +-+ Tiling into Build Matrix ? (Notes: Unexpected behavior may occur with tiling and sub texture area, like from atlas ... )
	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TILING))
	{
		// tiling = pbuilder->Tiling;
		NVec3ScaleBy((NVEC3 *)&txc_bmat.XAxis, pbuilder->Tiling.x);
		NVec3ScaleBy((NVEC3 *)&txc_bmat.YAxis, pbuilder->Tiling.y);
		NVec3ScaleBy((NVEC3 *)&txc_bmat.ZAxis, pbuilder->Tiling.z);
	}
	else
	{
		// NVec3Set(&tiling,DEFAULT_NUT_TEXTCOORD_BUILDER_TILING_3f);
	}
	// +-+ Offset into Build Matrix ?
	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_OFFSET))
	{
		// offset = pbuilder->Offset;
		NVec3AddTo((NVEC3 *)&txc_bmat.Translation, &pbuilder->Offset);
	}
	else
	{
		// NVec3Set(&offset,DEFAULT_NUT_TEXTCOORD_BUILDER_OFFSET_3f);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ Projection Matrix
	// +
	NIdentityMatrix3x3(&matrix);
	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_PROJECTIONPLANE_DIRECTORVECTOR))
	{
		vdir = pbuilder->ProjectionPlane_DirectorVector;
		NVec3Normalize(&vdir);
		// NUT_BuildHughesMoller3DOrthonormal(&matrix,&vdir);
		NUT_Build3DOrthonormal(&matrix, &vdir, NULL, NULL);
	}
	else if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_PROJECTIONPLANE))
	{
		switch (pbuilder->ProjectionPlane)
		{
		case _PLANE_XY:
			NVec3Set((NVEC3 *)&matrix.Side, 1.0f, 0.0f, 0.0f);
			NVec3Set((NVEC3 *)&matrix.Up, 0.0f, 1.0f, 0.0f);
			NVec3Set((NVEC3 *)&matrix.Forward, 0.0f, 0.0f, 1.0f);
			break;
		case _PLANE_XZ:
			NVec3Set((NVEC3 *)&matrix.Side, 1.0f, 0.0f, 0.0f);
			NVec3Set((NVEC3 *)&matrix.Up, 0.0f, 0.0f, 1.0f);
			NVec3Set((NVEC3 *)&matrix.Forward, 0.0f, 1.0f, 0.0f);
			break;
		case _PLANE_ZY:
			NVec3Set((NVEC3 *)&matrix.Side, 0.0f, 0.0f, 1.0f);
			NVec3Set((NVEC3 *)&matrix.Up, 0.0f, 1.0f, 0.0f);
			NVec3Set((NVEC3 *)&matrix.Forward, 1.0f, 0.0f, 0.0f);
			break;
		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			break;
		}
	}
	else
	{
		//  _PLANE_XY as default
		NVec3Set((NVEC3 *)&matrix.Side, 1.0f, 0.0f, 0.0f);
		NVec3Set((NVEC3 *)&matrix.Up, 0.0f, 1.0f, 0.0f);
		NVec3Set((NVEC3 *)&matrix.Forward, 0.0f, 0.0f, 1.0f);
	}
	NTransposeMatrix3x3(&matrix, &matrix);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ BitField to tag Triangle Used Vertex, the common ones between defined vertex range
	// +   and defined triangle range.
	// +
	pnu32bitfield = (Nu32 *)Nmalloc(Nu32FIELD_SIZE(vertex_number) * sizeof(Nu32));
	memset(pnu32bitfield, 0, Nu32FIELD_SIZE(vertex_number) * sizeof(Nu32));

	ptri = (NTRIANGLE *)NGetArrayPtr(&pgeom->Mesh.PrimitiveArray, first_triangle_index);
	for (n = triangle_number; n != 0; n--, ptri++)
	{
		if (ptri->i0 >= first_vertex_index && ptri->i0 < first_vertex_index + vertex_number)
		{
			Nu32FIELD_BITSET(pnu32bitfield, ptri->i0 - first_vertex_index);
		}

		if (ptri->i1 >= first_vertex_index && ptri->i1 < first_vertex_index + vertex_number)
		{
			Nu32FIELD_BITSET(pnu32bitfield, ptri->i1 - first_vertex_index);
		}

		if (ptri->i2 >= first_vertex_index && ptri->i2 < first_vertex_index + vertex_number)
		{
			Nu32FIELD_BITSET(pnu32bitfield, ptri->i2 - first_vertex_index);
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TMP Buffer
	// +	To store all the 'projected vertex' coordinates.
	pbuffer = (NVEC3 *)Nmalloc(sizeof(NVEC3) * vertex_number);
	pvb = pbuffer;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + COMPUTE PROJECTED COORDINATES
	// +
	// +-+ Compute coordinates into the desired base and evaluate AABB into that base.
	// +
	NSetMinMaxAABBWithMaxMinNf32(&geom_aabb);
	pv = (NVEC3 *)pgeom->Mesh.VertexArray.pFirst;
	vstride = pgeom->Mesh.VertexArray.ElementSize;
	for (n = 0; n < vertex_number; n++, pv = (NVEC3 *)((NBYTE *)pv + vstride), pvb++)
	{
		if (Nu32FIELD_BITGET(pnu32bitfield, n))
		{
			NMulVector3ByMatrix3x3O(pvb, &matrix, pv);
			NAABB_IncludeVector3Into(&geom_aabb, pvb);
		}
	}
	// Extract Geometry Volume Size and Geometry Volume Center from Geometry AABB.
	NGetAABBCenter(&geom_center, &geom_aabb);
	NGetAABBSize(&geom_size, &geom_aabb);
	// +
	// +-+ Take care about Texture Coordinates Reference Volume.
	// +	Purpose of this function is generating texture coordinates trough a planar projection.
	// +	The first step of this projection is done, so now, all vertex are now computed
	// +	into a new base with a z oriented like the director vector of the projection plane.
	// +	In this new base, x and y ( and z ) can be simply normalized to obtain texture coordinates.
	// +	In that case, that means the whole texture frame matches with the XY face of the geometry AABB bounding box.
	// +
	// +	Its cool but that means we don't care of the texture initial size. Texture will be stretched to perfectly fit the object
	// +	bounding volume, mapped/wrapped on its XY front face.
	// +	That's the default behavior, but there is a way to take in account the texture size.
	// +
	// +	Notes:	The NBUILDBASE_TEXCOORD embed structure has no size information. The UsedOAARectf32/SubAARectf32 included give information about which part of the texture is used.
	// +			These infos are relative to the texture itself, so a rectangular texture and a square texture plainly used 	will have the same UsedOAARectf32 infos, like (0,0,1,1).
	// +
	// +	To include the texture frame size in the equation we have to explicitly describe it to the algorithm.
	// +	Just to be perfectly clear. Imagine looking at the geometry trough a glass board ( the projection plane ). The geometry vertex from that point of view are going to be
	// +	the Texture coordinates (UVW). But to do this we have to 'draw' on the glass board the frame of the texture, this frame is going to represent the (0,0,1,1) Texcoord frame !
	// +	( by extension we draw a depth too ... for W ).
	// +	So, from that point Engine can transform transformed vertex positions ( the position they have from the glass board point of view )
	// +	to have them relatively to this TxC Volume ... (the frame of the texture + a depth )...
	// +
	// +	By default, without any user specification about TxC Volume, Engine suppose that TxC Volume fit perfectly the Geometry Volume.
	// +
	// + STEP A:	Analyze and extract incoming TXC Volume data.
	// +			We are sure about flags consistency because of earlier debug test.
	// +			At the end of this step, TxC Volume Data are going to be describe into a unique Way:
	// +						txc_volume_size
	// +						txc_volume_center
	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCAABB))
	{
		NGetAABBCenter(&txc_volume_center, &pbuilder->TxCAABB);
		NGetAABBSize(&txc_volume_size, &pbuilder->TxCAABB);

		workingflags |= FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE;
		workingflags |= FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER;
	}
	else
	{
		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE))
		{
			txc_volume_size = pbuilder->TxCSize;
			workingflags |= FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE;
		}

		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER))
		{
			txc_volume_center = pbuilder->TxCCenter;
			workingflags |= FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER;
		}
	}
	// + STEP B:	Analyze and extract incoming TXC Volume data.
	// +			We are sure about flags consistency because of earlier debug test.
	// +
	if (ISFLAG_ON(workingflags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCSIZE))
	{
		//
		// COMPUTE A RATIO r, according with the relationship between TxCSize and Geometry Dimensions.
		//
		r = 1.0f; // that the default value for r ... (if all tests failed )
		// TxCSize.x is suppose to FIT with object AABB X
		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_X))
		{
			// NErrorIf(!pbuilder->TxCSize.x, NERROR_NULL_VALUE);
			if (txc_volume_size.x)
				r = geom_size.x / txc_volume_size.x;
		}
		// TxCSize.y is suppose to FIT with object AABB Y
		else if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Y))
		{
			// NErrorIf(!pbuilder->TxCSize.y, NERROR_NULL_VALUE);
			if (txc_volume_size.y)
				r = geom_size.y / txc_volume_size.y;
		}
		// TxCSize.z is suppose to FIT with object AABB Z
		else if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCSIZE_FIT_Z))
		{
			// NErrorIf(!pbuilder->TxCSize.z, NERROR_NULL_VALUE);
			if (txc_volume_size.z)
				r = geom_size.z / txc_volume_size.z;
		}
		// none of the TxCSize dimensions are suppose to FIT with object dimensions
		// So keeping the default r value ( 1.0f )
		// 		else
		// 		{
		// 			r = 1.0f;
		// 		}

		// Adjust texture coordinates volume Size with the right ratio
		NVec3ScaleBy(&txc_volume_size, r);
	}
	else
	{
		txc_volume_size = geom_size;
	}

	/*
			// TxC virtual 3D Box and geometry AABB are suppose to be both aligned
			// and their respective center are suppose to be in the same place.
			// So it's easy to compute the 'ref_origin' which is the Min Corner of the "Ref" Box ( with Ref size dimensions )
			origin.x = center.x - size.x*0.5f;
			origin.y = center.y - size.y*0.5f;
			origin.z = center.z - size.z*0.5f;
		}
		else
		{
			// Size still the size of the geometry AABB previously calculated
			// Origin simply becomes AABB min
			origin = aabb.VMin;
		}
	*/
	if (ISFLAG_ON(workingflags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_TXCCENTER))
	{
		txc_volume_center = pbuilder->TxCCenter;

		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_X))
			txc_volume_center.x = geom_center.x;
		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Y))
			txc_volume_center.x = geom_center.y;
		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_TXCCENTER_ALIGN_Z))
			txc_volume_center.x = geom_center.z;
	}
	else
	{
		txc_volume_center = geom_center;
	}

	// +
	// +-+ PreCompute AABB size ratio to get normalized coordinates.
	// +	tc = ( pvb - origin ) / size;
	// +	In that way we will have all the 3D vertex coordinates ( in the desired base ) in to
	// +	a normalized form  0 < tc < 1 ( maybe not normalized if TxCSize is used but it's not a problem ... )
	// +	Its very close to Texcoord form isn't it ?
	if (txc_volume_size.x)
		txc_volume_size.x = 1.0f / txc_volume_size.x;
	if (txc_volume_size.y)
		txc_volume_size.y = 1.0f / txc_volume_size.y;
	if (txc_volume_size.z)
		txc_volume_size.z = 1.0f / txc_volume_size.z;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + COMPUTE UVW
	// +
	// +-+ 3 steps.	1/ Inflate
	// +			2/ Normalize
	// +			3/ Multiply normalized vector3 by the TexCoord Build Matrix
	// +
	pvb = pbuffer;
	ptc = pgeom->Mesh.VertexArray.pFirst + NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0 + texcoordsetid];
	for (n = 0; n < vertex_number; n++, ptc += vstride, pvb++)
	{
		if (!Nu32FIELD_BITGET(pnu32bitfield, n))
			continue;

		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_U))
		{
			// 1/Normalize x (against AABB size x)
			tc.x = (pvb->x - txc_volume_center.x) * txc_volume_size.x;
			// 2/Multiply normalized vector component by the TexCoord Build Matrix
			((NTEXCOORD_2f *)ptc)->x = txc_bmat.Translation.x + tc.x * txc_bmat.XAxis.x + tc.x * txc_bmat.XAxis.y + tc.x * txc_bmat.XAxis.z;
		}

		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_V))
		{
			// 1/Normalize y (against AABB size y) AND "Invert" value because conventional 'V' TexCoord Axis facing Down !
			tc.y = NTxCYFacingUp((pvb->y - txc_volume_center.y) * txc_volume_size.y); // 1.0f-(tc.y-aabb.fYMin)*size.y;
			// 2/Multiply normalized vector component by the TexCoord Build Matrix
			((NTEXCOORD_2f *)ptc)->y = txc_bmat.Translation.y + tc.y * txc_bmat.YAxis.x + tc.y * txc_bmat.YAxis.y + tc.y * txc_bmat.YAxis.z;
		}
		if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W))
		{
			// 1/Normalize z (against AABB size z)
			tc.z = (pvb->z - txc_volume_center.z) * txc_volume_size.z;
			// 2/Multiply normalized vector component by the TexCoord Build Matrix
			((NTEXCOORD_3f *)ptc)->z = txc_bmat.Translation.z + tc.z * txc_bmat.ZAxis.x + tc.z * txc_bmat.ZAxis.y + tc.z * txc_bmat.ZAxis.z;
		}
	}

	Nfree(pnu32bitfield);
	Nfree(pbuffer);
}

Ns32 _xrpoly_qsort_compare_less_first(const void *pa, const void *pb)
{
	// return(((NARRAY*)pa)->Size - ((NARRAY*)pb)->Size );

	// we want having the 'less' considered as 'stronger than' the 'larger', so tests are inverted to achieve this goal
	if (((NARRAY *)pa)->Size < ((NARRAY *)pb)->Size)
		return 1;
	else if (((NARRAY *)pa)->Size > ((NARRAY *)pb)->Size)
		return -1;
	else
		return 0;
}
// ------------------------------------------------------------------------------------------
// void NUT_BuildGeometryVertexTexCoords_FaceMap
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//		NGEOMETRY					*pgeom					A valid Pointer on a Geometry
//
//		const Nu32					first_vertex_index		index of the first vertex of the build vertex TexCoord range
//		const Nu32					vertex_number			Number of vertex in the build vertex TexCoord range
//		const Nu32					first_triangle_index	index of the first triangle in the build triangle range
//		const Nu32					triangle_number			Number of triangle in the build triangle range
//															To be "in range" and has its texture coordinates calculated,
//															a vertex as to be inside the defined vertex range and has to be used
//															at least by one of the triangle of the defined triangles range.
//															These 4 values are restrictives to each others.
//
//															So if user doesn't care about triangle he should call this function with
//															"0" and "pgeom->PrimitiveArray.Size" for triangle first and range size parameters.
//															And define the vertex first and range size parameters in the right way.
//
//															Same strategy if user only care about triangle. User should call this function
//															with "0" and "pgeom->VertexArray.Size" for vertex first and range size parameters.
//															And define the Triangle first and range size parameters in the right way.
//
//		NUT_TEXCOORD_BUILDER_PLANAR	*pbuilder				A Valid Pointer on a NUT_TEXCOORD_BUILDER structure
// // Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BuildGeometryVertexTexCoords_FaceMap(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER_FACEMAP *pbuilder)
{
	Nf32 quadquality;
	Nf32 cosphi_min;
	Nu32 flags = 0;
	NUT_XTRI_ARRAY xtriarray;
	NUT_XQUAD_ARRAY xquadarray;
	NUT_XAREA_ARRAY xarea_array;
	NUT_XRPOLY_ARRAY xrpoly_array;
	Nu8 offset_to_texcoords;
	Nu32 texcoordsetid;
	Nu32 vertex_flags;
	NMATRIX txc_bmat;
	NTEXCOORD_3f uvquadref[4];

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CHECKS
	// +
	NErrorIf(pbuilder->Mode != NUT_TEXCOORD_BUILD_MODE_FACEMAP, NERROR_INCONSISTENT_PARAMETERS);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TexCoord Set ID
	// +   ( Vertex may have several TexCoord Set - So, we need to know the one to use )
	texcoordsetid = NGetBuildBaseTexCoordSetID(&pbuilder->TexCoordBuild);
	vertex_flags = NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags;

	NErrorIf(texcoordsetid > 1, NERROR_GEOMETRY_INCONSISTENT_PARAMETER);
	NErrorIf(texcoordsetid == 0 && !NCHECK_VERTEX_SPECS_TEXCOORD0(vertex_flags), NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);
	NErrorIf(texcoordsetid == 1 && !NCHECK_VERTEX_SPECS_TEXCOORD0(vertex_flags), NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA);

	offset_to_texcoords = NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].TableAccess.Offset[NVERTEX_DATA_TEXCOORD0 + texcoordsetid];

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP
	// +
	// +-+ TexCoord 3f or 2f ? 3s or 2s ? ... CONSISTENCY CHECKS ONLY ...
	// +
	// Short not supported yet ... TODO : support them !
	NErrorIf(NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(vertex_flags), NERROR_SYSTEM_TODO);
	NErrorIf(NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(vertex_flags), NERROR_SYSTEM_TODO);

	// If User asks building W component. Incoming Geometry Vertex Format need to support 3 TexCoords components...
	// ------------------------------------------------------------------------------------------------------------
	// BUT, RIGHT KNOW TEXCOORD W is not supported in FACEMAPPING so just "CRASH" if it's requested.
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W), NERROR_INCONSISTENT_FLAGS);
	// BUT ... BUT, RIGHT KNOW TEXCOORD s is not supported in FACEMAPPING so just "CRASH" if it's requested.
	NErrorIf(NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_2s(vertex_flags) || NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(vertex_flags), NERROR_INCONSISTENT_FLAGS);

	// ... and just for the future (when this function will support W ... Vertex Texture coordinates have to support w coordinate too ...)
	NErrorIf(ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORD_W) && (!NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3s(vertex_flags)) && (!NCHECK_VERTEX_SPECS_TEXCOORDS_TYPE_3f(vertex_flags)), NERROR_INCONSISTENT_FLAGS);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP UV QUAD TEXTURE COORDINATES
	// +
	// +-+	TexCoord Build Matrix ( 3D Matrix 4x4 )
	// +	... And the 4 Texture Coordinates used as Quad Texture Coordinates for each Quad of the Geometry
	// +
	// +-+	Pre-Build each of the 4 TexCoords necessary for each Quad
	NGetBuildBaseTexCoordMatrix4x4(&txc_bmat, &pbuilder->TexCoordBuild);

	// _EAST EDGE P0 (1,1)
	uvquadref[0].x = txc_bmat.Translation.x + txc_bmat.XAxis.x + txc_bmat.YAxis.x;
	uvquadref[0].y = NTxCYFacingDown(txc_bmat.Translation.y + txc_bmat.XAxis.y + txc_bmat.YAxis.y);
	uvquadref[0].z = txc_bmat.Translation.z /* + txc_bmat.XAxis.z + txc_bmat.YAxis.z*/;

	// _NORTH EDGE P0 (1,0)
	uvquadref[1].x = txc_bmat.Translation.x + txc_bmat.XAxis.x;
	uvquadref[1].y = NTxCYFacingDown(txc_bmat.Translation.y + txc_bmat.XAxis.y);
	uvquadref[1].z = txc_bmat.Translation.z /* + txc_bmat.XAxis.z*/;

	// _WEST EDGE P0 (0,0)
	uvquadref[2].x = txc_bmat.Translation.x;
	uvquadref[2].y = NTxCYFacingDown(txc_bmat.Translation.y);
	uvquadref[2].z = txc_bmat.Translation.z;

	// _SOUTH EDGE P0 (0,1)
	uvquadref[3].x = txc_bmat.Translation.x + txc_bmat.YAxis.x;
	uvquadref[3].y = NTxCYFacingDown(txc_bmat.Translation.y + txc_bmat.YAxis.y);
	uvquadref[3].z = txc_bmat.Translation.z /* + txc_bmat.YAxis.z*/;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SETUP XMESH XRPOLY Building
	// +
	// +
	// +
	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_QUADQUALITY_THRESHOLD))
	{
		quadquality = pbuilder->QuadQualityThreshold;
	}
	else
	{
		quadquality = DEFAULT_NUT_TEXTCOORD_BUILDER_QUADQUALITY_THRESHOLD;
	}

	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_USEPARAM_COPLANARITY_THRESHOLD))
	{
		cosphi_min = pbuilder->CoplanarityThreshold;
	}
	else
	{
		cosphi_min = DEFAULT_NUT_TEXTCOORD_BUILDER_COPLANARITY_THRESHOLD;
	}

	if (ISFLAG_ON(pbuilder->Flags, FLAG_NUT_TEXTCOORD_BUILDER_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2))
	{
		FLAG_ON(flags, FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BUILD XRPOLY
	// +
	// +	... but checking at each stage if there is enough primitive to justify next building stage.
	// +
	Nmem0(&xtriarray, NUT_XTRI_ARRAY);
	Nmem0(&xquadarray, NUT_XQUAD_ARRAY);
	Nmem0(&xarea_array, NUT_XAREA_ARRAY);
	Nmem0(&xrpoly_array, NUT_XRPOLY_ARRAY);

	// Create XTRI Array
	if (!triangle_number)
		return;
	NUT_BuildXTriArray(&xtriarray, &pgeom->Mesh.PrimitiveArray, first_triangle_index, triangle_number, &pgeom->Mesh.VertexArray, first_vertex_index, vertex_number);
	if (xtriarray.XTriArray.Size == 1)
	{
		NUT_BuildXTriTexCoords_FaceMap((NUT_XTRI *)xtriarray.XTriArray.pFirst, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW);
		NUT_ClearXTriArray(&xtriarray);
		return;
	}
	// Create XQUAD Array and XArea
	NUT_BuildXAreaArray(&xarea_array, &xquadarray, &xtriarray, quadquality, cosphi_min, flags);

	// TO OPTIMIZE distinguish all specific cases ...

	// 1/ NO XQUADS ! ONLY XTRI ( and more than 1, ... see above )
	if (xquadarray.XQuadArray.Size == 0)
	{
		// That means there are only isolated XTRIS ... Map them
		NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap(&xtriarray, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW); // The mask only because we are just interested by the 3 FLAG_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_U, V and W. It's not an obligation , but to stay Cristal clear ...

		// ... and clear all build things
		NUT_ClearXAreaArray(&xarea_array);
		NUT_ClearXQuadArray(&xquadarray);
		NUT_ClearXTriArray(&xtriarray);
	}
	// 2/ Only 1 XQUAD (in only 1 XArea)
	else if (xquadarray.XQuadArray.Size == 1)
	{
		// There are potential isolated XTRIS ...
		NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap(&xtriarray, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW); // The mask only because we are just interested by the 3 FLAG_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_U, V and W. It's not an obligation , but to stay Cristal clear ...
		// ... and only 1 XQUAD
		NUT_BuildXQuadTexCoords_FaceMap((NUT_XQUAD *)xquadarray.XQuadArray.pFirst, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW);

		// clear all build things
		NUT_ClearXAreaArray(&xarea_array);
		NUT_ClearXQuadArray(&xquadarray);
		NUT_ClearXTriArray(&xtriarray);
	}
	// 3/ Several XTRI and XQUADS ...
	//	  ... build XRPOLY to have a smart view on them.
	else
	{
		NUT_BuildXRPolyArrayFromXAreaArray(&xrpoly_array, &xarea_array, &xquadarray, &xtriarray);

		// start mapping potential isolated XTRI
		NUT_BuildXTriArrayIsolatedXTriTexCoords_FaceMap(&xtriarray, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW); // The mask only because we are just interested by the 3 FLAG_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_U, V and W. It's not an obligation , but to stay Cristal clear ...
		// APPLY TEXTURE COORDINATES TO XRPOLY
		// First we are going to sort All XRPolys from the less number of XQuads inside to the max.
		// ...And after, we are going to apply texture coordinates to each of them.
		// ...starting with the XRPoly with the smaller number of XQuads inside.
		NQuickSortArray(&xrpoly_array.XRPolyArray, _xrpoly_qsort_compare_less_first);
		// Because of sorting, XRPOLY order may be changed, and XQuads of these XRPoly have to be Re-addressed ( their .pXRPoly pointer)
		NUT_ReAddressing_XRPoly_Array(&xrpoly_array);
		NUT_BuildXRPolyArrayTexCoords_FaceMap(&xrpoly_array, offset_to_texcoords, &uvquadref[0], pbuilder->Flags & MASK_NUT_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_UVW); // The mask only because we are just interested by the 3 FLAG_TEXTCOORD_BUILDER_BUILD_TEXCOORDS_U, V and W. It's not an obligation , but to stay Cristal clear ...

		// and clear
		NUT_ClearXRPolyArray(&xrpoly_array);
		NUT_ClearXAreaArray(&xarea_array);
		NUT_ClearXQuadArray(&xquadarray);
		NUT_ClearXTriArray(&xtriarray);
	}
	return;
}

void NUT_BuildGeometryVertexTexCoords(NGEOMETRY *pgeom, const Nu32 first_vertex_index, const Nu32 vertex_number, const Nu32 first_triangle_index, const Nu32 triangle_number, NUT_TEXCOORD_BUILDER *pbuilder)
{
	switch (pbuilder->Common.Mode)
	{
	case NUT_TEXCOORD_BUILD_MODE_FACEMAP:
		NUT_BuildGeometryVertexTexCoords_FaceMap(pgeom, first_vertex_index, vertex_number, first_triangle_index, triangle_number, &pbuilder->FaceMap);
		break;

	case NUT_TEXCOORD_BUILD_MODE_PLANAR:
		NUT_BuildGeometryVertexTexCoords_Planar(pgeom, first_vertex_index, vertex_number, first_triangle_index, triangle_number, &pbuilder->Planar);
		break;

	default:
		NErrorIf(1, NERROR_INVALID_CASE);
		break;
	}
}