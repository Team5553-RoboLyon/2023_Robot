#include "lib/N/NCStandard.h"
#include "../Containers/NArray.h"
#include "../Geometry/NGeometryMisc.h"
#include "../Maths/NMatrix3x3.h"
#include "lib/N/NErrorHandling.h"
#include "NUT_XMesh.h"

// ------------------------------------------------------------------------------------------
// void _clear_area_element
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
static void _clear_area_element(void *ppxquad)
{
	NUT_XQUAD *pxquad = *(NUT_XQUAD **)ppxquad;
	NErrorIf(pxquad->pXArea == NULL, NERROR_SYSTEM_GURU_MEDITATION);  // ??? It's not suppose to happen ! Never ! pxquad->XAreaIndex is suppose to be equal to the Index of its XArea
	NErrorIf(pxquad->pXRPoly != NULL, NERROR_SYSTEM_GURU_MEDITATION); // ??? It's not suppose to happen ! Never ! pxquad->XRPolyIndex is suppose to be equal to NVOID, because XQUAD is suppose to be OUT OF XRPOLY !

	FLAG_OFF(pxquad->Flags, FLAG_NUT_XQUAD_IN_AREA);
	pxquad->pXArea = NULL;
}
// ------------------------------------------------------------------------------------------
// void NUT_ClearXArea
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
void NUT_ClearXArea(NUT_XAREA *pxarea)
{
	NClearArray((NARRAY *)pxarea, _clear_area_element);
}
// ------------------------------------------------------------------------------------------
// void NUT_DeleteXArea
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
void NUT_DeleteXArea(NUT_XAREA *pxarea)
{
	NDeleteArray((NARRAY *)pxarea, _clear_area_element);
}
// ------------------------------------------------------------------------------------------
// void _clear_xareaarray_element
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
static void _clear_xareaarray_element(void *pxarea)
{
	NUT_ClearXArea((NUT_XAREA *)pxarea);
}
// ------------------------------------------------------------------------------------------
// void NUT_ClearXAreaArray
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
void NUT_ClearXAreaArray(NUT_XAREA_ARRAY *pxquadareaarray)
{
	NClearArray((NARRAY *)pxquadareaarray, _clear_xareaarray_element);
}
// ------------------------------------------------------------------------------------------
// void NUT_DeleteXAreaArray
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
void NUT_DeleteXAreaArray(NUT_XAREA_ARRAY *pxquadareaarray)
{
	NDeleteArray((NARRAY *)pxquadareaarray, _clear_xareaarray_element);
}

// ------------------------------------------------------------------------------------------
// void NUT_ReAddressing_XArea_Array(NUT_XAREA_ARRAY* pxquadareaarray)
// ------------------------------------------------------------------------------------------
// Description :
//		Due to memory reallocation (create a new XAREA and add it into the NUT_XAREA_ARRAY )
//		pXArea stored into each XQUAD as to be re-addressed (potentially).
//		Notice that re-addressing is going to be done only if it seems necessary.
//		... Function is going to check the first XQUAD of each XArea and perform
//		re-addressing (or not) in accordance with the .pXArea of this first XQuad ( all the XQuads of one XArea
//		are suppose to be set in the same way ...)
//		... SO A TRICK to ENFORCE readdressing ALL XQUADS of a specific XAREA : Set it's first XQUAD.pXArea to NULL !!

// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_ReAddressing_XArea_Array(NUT_XAREA_ARRAY *pxquadareaarray)
{
	Nu32 n, p;
	NUT_XAREA *pxarea;
	NUT_XQUAD **ppxquad;

	pxarea = (NUT_XAREA *)pxquadareaarray->XAreaArray.pFirst;
	for (n = pxquadareaarray->XAreaArray.Size; n != 0; n--, pxarea++)
	{
		if (pxarea->XQuadPtrArray.Size)
		{
			ppxquad = (NUT_XQUAD **)pxarea->XQuadPtrArray.pFirst;
			// All the XQUAD included need to have a valid pXArea pointer.
			// If not, address it for all of them !
			if ((*ppxquad)->pXArea != pxarea)
			{
				for (p = pxarea->XQuadPtrArray.Size; p != 0; p--, ppxquad++)
				{
					(*ppxquad)->pXArea = pxarea;
				}
			}
		}
	}
}
// ------------------------------------------------------------------------------------------
// Nf32 _evaluate_xtricouple
// ------------------------------------------------------------------------------------------
// Description :
//	This function is going to calculate a quality coefficient (f) for the incoming triangle couple (a future potential XQUAD).
//	The second Xtri of the couple is the adjacent triangle trough the edge "edge_id" of the incoming Xtri.
// ------------------------------------------------------------------------------------------
// In	:
//			const NUT_XTRI *pxtri	A valid pointer on a XTriangle
//			const Nu32 edge_id		Id of the XTriangle Edge used to get the adjacent Xtri of the incoming one.
//			const Nf32 cosphi_min	Cosine of the maximum acceptable Angle  between the 2 triangles normal.
//									Less cosphi_min is, larger accepted angle between the 2 triangles is.
//
//									notice: Tested angle is the one between triangles NORMAL. SO ...
//											an angle close to 0 between normals		(=PI between triangles)		has a cosine close to  1
//											an angle close to PI/2 between normals	(=PI/2 between triangles)	has a cosine close to  0
//											an angle close to PI between normals	(=0 between triangles)		has a cosine close to -1
//
//									So, if user accepts all cases, worse ones included, he should use a value close to -1.
//										( With -1, opposite coplanar triangles are going to be selected too ...
//										  And it's not really recommended. )
//										Prefer using 0 as minimum. 0 means a pair of triangles with an angle close to PI/2 between them
//										is going to be accepted.
//										if user accepts only perfect coplanar triangles he must use 1
//										( but it should be better to accept a small tolerance, like 0.99f instead of 1 )
//										if user accepts "correct" cases he should use cosine(PI/6) or cosine(PI/4)
//										ADVICE: a cosphi_min
// Out :
//		A Nf32 value which represents the quality coefficient.
//
//		!!! IMPORTANT, even if this value is in the same range than 'cosphi_min' it's absolutely not the same things !!!
//
//		f = -1.0f a pair of triangles out of coplanarity limit. (Same Quality coefficient for isolated triangle.)
//
//		f =  0.0f a pair of triangles which is NOT a good XQUAD.
//		f >  0.0f a pair of triangles which is a XQUAD.
//		f =  1.0f a pair of triangles which is the best possible XQUAD.
// ------------------------------------------------------------------------------------------
static inline Nf32 _evaluate_xtricouple(const NUT_XTRI *pxtria, const Nu32 edge_ida, const NUT_XTRI *pxtrib, const Nu32 edge_idb, const Nf32 cosphi_min)
{
	NErrorIf(!pxtria, NERROR_NULL_POINTER);
	NErrorIf(edge_ida >= 3, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(!pxtrib, NERROR_NULL_POINTER);
	NErrorIf(edge_idb >= 3, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(cosphi_min > 1.0f || cosphi_min < -1.0f, NERROR_VALUE_OUTOFRANGE);
	// First of all let's define a co planarity  quality coefficient.
	// The best case is c = 1, that means the 2 triangles are perfectly coplanar.
	// The Worse case is -1, that means they are opposite. ( They are coplanar too, it's true but they're  facing the opposition direction )
	// or that means one of the 2 triangles is a degenerated one.
	if (NVec3DotProduct(&pxtria->Normal, &pxtrib->Normal) < cosphi_min) // -1 < f < 1
	{
		return -1.0f;
	}

	// Compute the 4 quad-corner angles
	// CCW quad edge winding order.
	//	Common Edge is [1,3]
	//	3 <---------------------2
	//	^ .		   Vs2			|
	//	|     .					|
	//	| vs3      .		Vs1	|
	//	|				.		|
	//	|		   Vs0		.	V
	//	0 --------------------->1
	NVEC3 vs[4];
	Nu32 a;
	Nf32 l;

	NVec3Sub(&vs[0], pxtria->pPoint[NGET_EDGE_P0(edge_ida)], pxtria->pPoint[NGET_EDGE_POUT(edge_ida)]);
	NVec3Sub(&vs[3], pxtria->pPoint[NGET_EDGE_P1(edge_ida)], pxtria->pPoint[NGET_EDGE_POUT(edge_ida)]);

	NVec3Sub(&vs[1], pxtria->pPoint[NGET_EDGE_P0(edge_ida)], pxtrib->pPoint[NGET_EDGE_POUT(edge_idb)]);
	NVec3Sub(&vs[2], pxtria->pPoint[NGET_EDGE_P1(edge_ida)], pxtrib->pPoint[NGET_EDGE_POUT(edge_idb)]);

	for (a = 0; a < 4; a++)
	{
		l = NVec3Length(&vs[a]);
		if (l <= NF32_EPSILON_VECTOR_LENGTH)
		{
			// It should never happen ! Because X Triangle are not degenerated (Xtri building take care of this and didn't add degenerated triangle in it's list).
			// So, all of these 4 NVEC3 are not suppose to be close to NULL vector ! If it doe's ... CRASH
			NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
			return -1.0f;
		}
		else
		{
			// Normalize ...
			NVec3ScaleBy(&vs[a], 1.0f / l);
		}
	}

	// Each dot product is suppose to be ( -1 < dot < 1 ) so because of NABS, we should have 0 <f <4 ( because we have 4 NABS(dot) )
	// So, f*0.25f should be 0<f<1 ...
	// So, Final return result should be 0 < return value < 1 !
	// ( Remember return value may be -1.0f due to a 'bad' coplanarity factor. Have a look on the top of this function. )
	Nf32 f30 = NABS(NVec3DotProduct(&vs[3], &vs[0]));
	Nf32 f01 = NABS(NVec3DotProduct(&vs[0], &vs[1]));
	Nf32 f12 = NABS(NVec3DotProduct(&vs[1], &vs[2]));
	Nf32 f23 = NABS(NVec3DotProduct(&vs[2], &vs[3]));

	return (1.0f - (f30 + f01 + f12 + f23) * 0.25f); // a simple average... it works but ...
													 // return ( 1.0f -(f30*f30+f01*f01+f12*f12+f23*f23)*0.25f ); // ... this one seems better
}

static inline NUT_XQUAD *_push_xquad_to_array(NUT_XQUAD_ARRAY *pxquad_array, const NUT_XTRI *pxtri, Nu32 edge)
{
	NErrorIf(!pxquad_array, NERROR_NULL_POINTER);
	NErrorIf(ISFLAG_ON(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD), NERROR_XTRI_ALREADY_USED);
	NErrorIf(!pxtri->pAdjXTri[edge], NERROR_XTRI_NO_ADJACENT_XTRI);
	NErrorIf(ISFLAG_ON(pxtri->pAdjXTri[edge]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD), NERROR_XTRI_ALREADY_USED);
	NUT_XQUAD *pxquad;
	NUT_XQUAD xquad;

	Nmem0(&xquad, NUT_XQUAD);
	xquad.Flags = 0;
	xquad.pXTri[0] = (NUT_XTRI *)pxtri;
	xquad.pXTri[1] = pxtri->pAdjXTri[edge];
	FLAG_ON(xquad.pXTri[0]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD);
	FLAG_ON(xquad.pXTri[1]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD);
	xquad.pXTri[0]->pAssociatedXtri = xquad.pXTri[1];
	xquad.pXTri[1]->pAssociatedXtri = xquad.pXTri[0];
	xquad.pXTri[0]->QuadDiagonal = (Nu8)edge;
	xquad.pXTri[1]->QuadDiagonal = (Nu8)pxtri->AdjEdge[edge];
	xquad.pXArea = NULL;
	xquad.pXRPoly = NULL;

	NErrorIf(NIsArrayFull((NARRAY *)pxquad_array), NERROR_MEMORY_UNEXPECTED_REALLOCATION_WILL_OCCUR);
	pxquad = (NUT_XQUAD *)NArrayPushBack((NARRAY *)pxquad_array, (NBYTE *)&xquad);
	pxquad->pXTri[0]->pXQuad = pxquad;
	pxquad->pXTri[1]->pXQuad = pxquad;

	return pxquad;
}

static inline NUT_XQUAD **_push_xquadptr_to_area(NUT_XAREA *pxquad_area, NUT_XQUAD *pxquad)
{
	NErrorIf(ISFLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_IN_AREA), NERROR_XQUAD_ALREADY_IN_AREA);
	NErrorIf(pxquad->pXArea, NERROR_XQUAD_ALREADY_IN_AREA);

	FLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_IN_AREA);
	pxquad->pXArea = pxquad_area;
	return (NUT_XQUAD **)NArrayPushBack((NARRAY *)pxquad_area, (NBYTE *)&pxquad);
}

static inline Nf32 _get_best_available_edge(const NUT_XTRI *pxtri, Nu32 *pedgeID)
{
	Nf32 xquad_qualitymax = -1.0f;
	for (Nu32 a = 0; a < 3; a++)
	{
		// Notice that it's not necessary to check (pxtri->pAdjXTri[a]!=NULL)...
		// ...because of the first test (pxtri->EdgeQuality[a] > xquad_qualitymax) !!!
		// Indeed, pxtri->EdgeQuality[a] is set to -1 if pxtri->pAdjXTri[a]==NULL so,
		// pxtri->EdgeQuality[a] > xquad_qualitymax do the job !
		if ((pxtri->EdgeQuality[a] > xquad_qualitymax) && ISFLAG_OFF(pxtri->pAdjXTri[a]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
		{
			xquad_qualitymax = pxtri->EdgeQuality[a];
			*pedgeID = a;
		}
	}
	return xquad_qualitymax; // Which is the quality of the XQUAD made with these 2 adjacent triangles .
}

// ------------------------------------------------------------------------------------------
// NUT_XTRI* _update_xtri_adjacents
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_XTRI		*pxtrilist		a valid pointer on the first NUT_XTRI of the list to update
//			const Nu32		xtrilist_size	number of Xtri in the list
//			const Nf32		cosphi_min		Cosine of the maximum acceptable Angle  between the 2 triangles normal.
//											(see description of '_evaluate_xtricouple' for more information
// Out :
//			A pointer to the Xtri with the best edge quality.
//
// ------------------------------------------------------------------------------------------
static inline NUT_XTRI *_update_xtri_adjacents(NUT_XTRI *pxtrilist, const Nu32 xtrilist_size, const Nf32 cosphi_min)
{
	Nu32 m, n, a, b;
	NUT_XTRI *pxtri, *pxtrib;
	NUT_XTRI *pbestxtri = NULL;
	Nf32 f;
	Nf32 bestquality = -1.0f;

	pxtri = (NUT_XTRI *)pxtrilist;

	for (m = xtrilist_size; m != 0; m--, pxtri++)
	{
		// This XTriangle was already evaluated ( all it's 3 edges were tested/computed) and a solution was found with one of it's edge.
		// This XTriangle is a part of an XQUAD.
		// It's 2 other edges (the ones that are not the XQUAD diagonal ) are connected with the best XTriangle as possible for them,
		// thanks to this evaluation ( which was already made ).
		// So this XTriangle has to stay as it is.
		if (ISFLAG_ON(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
			continue;

		pxtrib = (NUT_XTRI *)pxtrilist;
		for (n = xtrilist_size; n != 0; n--, pxtrib++)
		{
			if (ISFLAG_ON(pxtrib->Flags, FLAG_NUT_XTRI_USED_BY_QUAD) || pxtri == pxtrib)
				continue;

			for (a = 0; a < 3; a++)
			{
				for (b = 0; b < 3; b++)
				{
					// Common Edge ?
					if (pxtri->pPoint[NGET_EDGE_P0(a)] == pxtrib->pPoint[NGET_EDGE_P1(b)] && pxtri->pPoint[NGET_EDGE_P1(a)] == pxtrib->pPoint[NGET_EDGE_P0(b)])
					{
						// Because of several successive call sof this function, pxtri->pAdjXTri[a] may be already computed ...
						// So we have to deal with that and take the right decisions.
						if (pxtri->pAdjXTri[a] != pxtrib) // 'pxtri->pAdjXTri[a]' may be NULL but 'pxtrib' can not !!
						{
							// It's possible that 'pxtri->pAdjXTri[a]' was already computed with an another triangle !!!!
							// That means Edge a has a valence > 2 and is shared by more than 2 triangles...
							// If the already computed adjacent triangle is included into an XQUAD,... with an another XTriangle than pxtri, because
							// this one is not a part of an XQuad (we are sure of this thanks to a previous test at the top of this function )...
							// ... then... we are going to RESET 'pxtri->pAdjXTri[a]' and try to pick a new adjacent triangle for it !
							if ((pxtri->pAdjXTri[a] != NULL) && ISFLAG_ON(pxtri->pAdjXTri[a]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
							{
								NErrorIf(1, NERROR_SYSTEM_CHECK); // Check the case: a triangle with an edge with a valence > 2 and a previously build XQuad ...
								pxtri->pAdjXTri[a] = NULL;
								pxtri->EdgeQuality[a] = -1.0f;
							}

							f = _evaluate_xtricouple(pxtri, a, pxtrib, b, cosphi_min);
							// pxtri->EdgeQuality[a] is set to -1 at setup and pxtri->pAdjXTri[a] is set to NULL.
							// So if 'f' is -1, pxtri->pAdjXTri[a] is going to stay 'NULL'
							if (f > pxtri->EdgeQuality[a])
							{
								pxtri->AdjEdge[a] = (Nu8)b;
								pxtri->pAdjXTri[a] = pxtrib;
								pxtri->EdgeQuality[a] = f;

								if (pxtri->EdgeQuality[a] > bestquality)
								{
									pbestxtri = pxtri;
									bestquality = pxtri->EdgeQuality[a];
								}
							}
						}
						else
						{
							//	...	That means "pxtri->pAdjXTri[a] == pxtrib"
							//	So, calculus are already made, and it's not necessary to redo them ( results will be the same. )
							// Excepted best quality test
							if (pxtri->EdgeQuality[a] > bestquality)
							{
								pbestxtri = pxtri;
								bestquality = pxtri->EdgeQuality[a];
							}
						}
					}
					// Common Edge BUT strange ... The 2 Triangles have a different winding order !!!! IT'S NOT NORMAL
					else if (pxtri->pPoint[NGET_EDGE_P0(a)] == pxtrib->pPoint[NGET_EDGE_P0(b)] && pxtri->pPoint[NGET_EDGE_P1(a)] == pxtrib->pPoint[NGET_EDGE_P1(b)])
					{
						NErrorIf(1, NERROR_PRIMITIVE_INCONSISTENT_WINDING_ORDER);
					}
				}
			}
		}
	}

	return pbestxtri;
}

// ------------------------------------------------------------------------------------------
// NUT_XTRI* _update_xtri_adjacents_optimized
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
static inline NUT_XTRI *_update_xtri_adjacents_optimized(NUT_XTRI *pxtrilist, const Nu32 xtrilist_size, const Nf32 cosphi_min)
{
	Nu32 m, n, a, b;
	NUT_XTRI *pxtri, *pxtrib;
	NUT_XTRI *pbestxtri = NULL;
	Nf32 bestquality = -1.0f;
	Nf32 f;

	pxtri = pxtrilist;
	for (m = 0; m < xtrilist_size; m++, pxtri++)
	{
		if (ISFLAG_ON(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
			continue;

		pxtrib = pxtri + 1;
		for (n = m + 1; n < xtrilist_size; n++, pxtrib++)
		{
			if (ISFLAG_ON(pxtrib->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
				continue;

			// Check each Xtri Edge with each other Xtri edges
			for (a = 0; a < 3; a++)
			{
				for (b = 0; b < 3; b++)
				{
					// Common Edge ?
					if (pxtri->pPoint[NGET_EDGE_P0(a)] == pxtrib->pPoint[NGET_EDGE_P1(b)] && pxtri->pPoint[NGET_EDGE_P1(a)] == pxtrib->pPoint[NGET_EDGE_P0(b)])
					{
						f = _evaluate_xtricouple(pxtri, a, pxtrib, b, cosphi_min);
						if (f > pxtri->EdgeQuality[a]) // pxtri->EdgeQuality[a] is set to -1 at setup...
						{
							pxtri->AdjEdge[a] = (Nu8)b;
							pxtri->pAdjXTri[a] = pxtrib;
							pxtri->EdgeQuality[a] = f;

							pxtrib->AdjEdge[b] = (Nu8)a;
							pxtrib->pAdjXTri[b] = pxtri;
							pxtrib->EdgeQuality[b] = pxtri->EdgeQuality[a];

							if (pxtri->EdgeQuality[a] > bestquality)
							{
								pbestxtri = pxtri;
								bestquality = pxtri->EdgeQuality[a];
							}
						}
					}
					// Common Edge BUT strange ... The 2 Triangles have a different winding order !!!! IT'S NOT NORMAL
					else if (pxtri->pPoint[NGET_EDGE_P0(a)] == pxtrib->pPoint[NGET_EDGE_P0(b)] && pxtri->pPoint[NGET_EDGE_P1(a)] == pxtrib->pPoint[NGET_EDGE_P1(b)])
					{
						NErrorIf(1, NERROR_PRIMITIVE_INCONSISTENT_WINDING_ORDER);
					}
				}
			}
		}
	}

	return pbestxtri;
}
// ------------------------------------------------------------------------------------------
// NUT_XTRI* NUT_UpdateXtriAdjacents
// ------------------------------------------------------------------------------------------
// Description :
//		Evaluate All Adjacent Triangle Couples and store results into each Xtri structure.
//		For each of the 3 Xtri Edges( edge id = 0,1,2 )the following XTRI members are updated:
//
//				pxtri->AdjEdge[edge id]		... to identify the same edge but into adjacent
//				pxtri->pAdjXTri[edge id]
//				pxtri->EdgeQuality[edge id]
//
//		At the End of the process, function returns the BEST Xtri, that means the one with the BEST evaluation coef.
//
// ------------------------------------------------------------------------------------------
// In	:
//			NARRAY *pxtriarray		a valid pointer on an Array with XTriangles inside.
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_XTRI *NUT_UpdateXtriAdjacents(NUT_XTRI_ARRAY *pxtriarray, const Nf32 cosphi_min, const Nu32 flags)
{
	NErrorIf(!pxtriarray, NERROR_NULL_POINTER);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOOK for ADJACENT TRIANGLES
	// + ... Evaluate All Adjacent Triangle Couples
	// + ... Return the best
	// +

	// Use optimized way ... if all edges have a valence of 2 (an edge is shared by 0,1 or 2 triangles at maximum )
	if (ISFLAG_ON(flags, FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2))
	{
		return _update_xtri_adjacents_optimized((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
	}
	// Use Full exhaustive method ... each triangle is evaluated against all the others ...
	else
	{
		return _update_xtri_adjacents((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_BuildXAreaArray
// ------------------------------------------------------------------------------------------
// Description :
//	Build a XQUAD array AND a XAREA array ( with pointer on XQUAD of XQUAD array into )
// ------------------------------------------------------------------------------------------
// In	:
//		NUT_XAREA_ARRAY	*pout_xareaarray	A valid pointer on an array of XAREA to fill in with computed XArea.
//		NUT_XQUAD_ARRAY *pout_xquadarray	A valid pointer on an array of XQUADs to fill in with XQUADS.
//
//											Note that this function is going to create XQUADS and store them into pout_xquadarray
//											AND, is going to regroup these XQUADS (pointers on them) into XAREAs that its going to create too.
//
//		const Nf32		 xquad_quality		A threshold, 'potential xquad' to be valid need to have a quality at least equal to this value.
//											Absolutely different than cosphi_min. It is based on the quality estimation of the 4 xquad angles.
//												if an angle is close to 90�, the dot product of its 2 sides is close to 0.
//												So, if the 4 dot products is close to zero ... we have a pretty good xquad here.
//												IMPORTANT to be easy to work (and understand , because 1 seems better than 0)
//												the quad quality formula is:
//
//																	---> 1-(dot0+dot1+dot2+dot3)/4
//
//												range of usable values:  0<= xquad_quality <= 1
//
//												so, use the followings:
//															  1				if you want ONLY the absolutely perfect 4 right angles quad.
//															  0.7 to 0.9	if you want to keep good quads.
//															  0.5 to 0.7	to accept quad of medium quality
//															  0				to accept all angles quadruplet configuration.
//
//												... but, make your own tests and decide what is the best for you !
//
//		const Nf32		 cosphi_min			Cosine of the max angle accepted between 2 triangles of an XQUAD.
//											This coefficient is used to check coplanarity of 2 triangles.
//											If the cosine of the angle between 2 triangles is less than this minimum
//											then the 2 triangles are rejected.
//												cosine(0)	=  1			if you want ONLY consider PERFECT coplanar triangles.
//												cosine(PI/2)=  0			if you want consider a wide range of triangles. Actually all the ones
//																			which are not facing in an opposite direction.
//												cosine(PI)	= -1(or less)	if you want to accept everything.
// Out :
//		NUT_XAREA_ARRAY* pout_xareaarray  itself.
//
// ------------------------------------------------------------------------------------------
NUT_XAREA_ARRAY *NUT_BuildXAreaArray(NUT_XAREA_ARRAY *pout_xareaarray, NUT_XQUAD_ARRAY *pout_xquadarray, const NUT_XTRI_ARRAY *pxtriarray, const Nf32 xquad_quality, const Nf32 cosphi_min, const Nu32 flags)
{
	NErrorIf(!pout_xareaarray, NERROR_NULL_POINTER);
	NErrorIf(!pout_xquadarray, NERROR_NULL_POINTER);
	NErrorIf(!pxtriarray, NERROR_NULL_POINTER);
	NErrorIf(xquad_quality < 0.0f || xquad_quality > 1.0f, NERROR_WRONG_VALUE); // Not really a bug, just to take care... as you can see, function are going

	Nf32 quality_threshold = NCLAMP(0.0f, xquad_quality, 1.0f); // to clamp "xquad_quality" so ...
	Nf32 f;
	Nf32 quality0;
	Nf32 quality1;
	NUT_XTRI **ppxtri;
	NUT_XTRI *pxtri;
	NUT_XTRI *pxtri0, *pxtri1;
	NUT_XTRI *pbestxtri;
	NUT_XQUAD *pxquad;
	NUT_XQUAD **ppxquad;
	NUT_XAREA xarea; // tmp struct to create new x area
	NUT_XAREA *pxarea;
	NARRAY tmparray_of_pxtri;
	Nu32 bestedgeid, edgeid;
	Nu32 n, a;
	Nu32 builded_xquad_number = 0;
	// 	Nu32			alonepoint[3]={2,0,1};					// Index of the "alone point" for each "edge". ( the third point which is not used to define the edge )
	// 	Nu32			edgepoint[3][2]={ {0,1},{1,2},{2,0} };	// Index of the "2 used points" for each "edge". ( the first and second point which are used to define the edge )
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + WORKING BUFFERS
	// +
	// +
	// + - + Out Arrays Check and Setup
	//			pout_xquadareasarray is an ARRAY of ARRAY.
	//			Each array included into it is supposed to be an XQUADArea and contains XQUAD pointers.
	if (pout_xareaarray->XAreaArray.ElementSize == 0)
	{
		NUT_SetupXAreaArray(pout_xareaarray, DEFAULT_NUT_AREA_ARRAY_CAPACITY);
	}
	else if (pout_xareaarray->XAreaArray.ElementSize == sizeof(NARRAY))
	{
#ifdef _DEBUG
		// pout_xquadareasarray is suppose to be an array of array of XQuads Pointers...
		if (pout_xareaarray->XAreaArray.Size != 0)
		{
			NErrorIf(((NARRAY *)NGetFirstArrayPtr((NARRAY *)pout_xareaarray))->ElementSize != sizeof(NUT_XQUAD *), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
		}
#endif

		if (pout_xareaarray->XAreaArray.Size == pout_xareaarray->XAreaArray.Capacity)
			NSetArrayCapacity((NARRAY *)pout_xareaarray, pout_xareaarray->XAreaArray.Size + DEFAULT_NUT_AREA_ARRAY_CAPACITY, NULL);
	}
	else
	{
		NErrorIf(1, NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup XQuads Array ?
	// +	There are several possibilities for a maximum flexibility.
	// +	1/	"pout_xquadarray" is full of ZERO. It will be setup with a capacity equal to the maximum number of buildable XQuads based
	// +		on the incoming XTriangles Array Size.
	// +
	if (pout_xquadarray->XQuadArray.ElementSize == 0)
	{
		NUT_SetupXQuadArray(pout_xquadarray, pxtriarray->XTriArray.Size / 2);
	}
	else if (pout_xquadarray->XQuadArray.ElementSize == sizeof(NUT_XQUAD))
	{
		// If 'pout_xquadarray' is already setup, it has to have enough capacity to insert all the new XQUADS.
		// If not ... CRASH in debug Mode. Do nothing in Release and return.
		// It's like this because of memory reallocation. Indeed 'reallocation' may change addresses of XQUADS and it's BAD !!!
		// Because these address are used by others XMesh structures, like XAREA,XRPOLY ...
		// So if user wants using the same pout_xquadarray for several successive XQUADS building, he has to setup this array by himself
		// with enough capacity right at the beginning.
		NErrorIf(pout_xquadarray->XQuadArray.Size + pxtriarray->XTriArray.Size / 2 > pout_xquadarray->XQuadArray.Capacity, NERROR_ARRAY_WRONG_SIZE);
		return pout_xareaarray;
	}
	else
	{
		NErrorIf(1, NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LOOK for ADJACENT TRIANGLES
	// + ... and Evaluate All Adjacent Triangle Couple
	// +
	if (ISFLAG_ON(flags, FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2))
		pbestxtri = _update_xtri_adjacents_optimized((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
	else
		pbestxtri = _update_xtri_adjacents((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BUILD QUADS Areas
	// +	From the best XQuad ( best XTri actually, that means the one with the best Quality coefficient associated with one
	// +	of its 3 edges ) we are going to propagate an area by using the Quality coefficient calculated in the previous step.
	// +
	if (pbestxtri && _get_best_available_edge(pbestxtri, &bestedgeid) < quality_threshold)
	{
		return 0;
	}
	NSetupArray(&tmparray_of_pxtri, 1 + (pxtriarray->XTriArray.Size * 2) / 3, sizeof(NUT_XTRI *));

	while (pbestxtri) // To create a new area
	{
		// Create a New Area
		// Calculate the capacity of this xquad area.
		// At maximum is equal to the half of non used xTri number.
		Nmem0(&xarea, NUT_XAREA);
		NUT_SetupXArea(&xarea, (pxtriarray->XTriArray.Size / 2) - builded_xquad_number);

		pxarea = (NUT_XAREA *)NArrayPushBack((NARRAY *)pout_xareaarray, (NBYTE *)&xarea);
		NUT_ReAddressing_XArea_Array(pout_xareaarray);

		while (pbestxtri) // To fill up an Area with XQuads
		{
			_push_xquadptr_to_area(pxarea, _push_xquad_to_array(pout_xquadarray, pbestxtri, bestedgeid));
			builded_xquad_number += 1;

			// Retrieve all the best available xtri arround the current Xquads Area ( = the area outline )
			// ... and insert them into a tmp array of xtri.
			tmparray_of_pxtri.Size = 0;
			ppxquad = (NUT_XQUAD **)pxarea->XQuadPtrArray.pFirst;
			for (n = pxarea->XQuadPtrArray.Size; n != 0; n--, ppxquad++)
			{
				pxquad = *ppxquad;

				quality0 = quality1 = quality_threshold;
				pxtri0 = pxtri1 = NULL;

				for (a = 0; a < 3; a++)
				{
					// XTri 0
					if (a != pxquad->pXTri[0]->QuadDiagonal)
					{
						// Notes: ( pxquad->pXTri[0]->EdgeQuality[a] == -1.0f ) is the same as ( pxquad->pXTri[0]->pAdjXTri[a] == NULL )
						if ((pxquad->pXTri[0]->pAdjXTri[a]) && ISFLAG_OFF(pxquad->pXTri[0]->pAdjXTri[a]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
						{
							NArrayPushBack(&tmparray_of_pxtri, (NBYTE *)&pxquad->pXTri[0]->pAdjXTri[a]);
						}
					}
					// XTri 1
					if (a != pxquad->pXTri[1]->QuadDiagonal)
					{
						// Notes: ( pxquad->pXTri[1]->EdgeQuality[a] == -1.0f ) is the same as ( pxquad->pXTri[1]->pAdjXTri[a] == NULL )
						if ((pxquad->pXTri[1]->pAdjXTri[a]) && ISFLAG_OFF(pxquad->pXTri[1]->pAdjXTri[a]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
						{
							NArrayPushBack(&tmparray_of_pxtri, (NBYTE *)&pxquad->pXTri[1]->pAdjXTri[a]);
						}
					}
				}
			}
			// Parse All the Xtri inserted into the tmp array to find the One able to form
			// the best XQuad.
			pbestxtri = NULL;
			quality0 = quality_threshold;
			ppxtri = (NUT_XTRI **)tmparray_of_pxtri.pFirst;
			for (n = tmparray_of_pxtri.Size; n != 0; n--, ppxtri++)
			{
				f = _get_best_available_edge(*ppxtri, &edgeid);
				if (f >= quality0)
				{
					bestedgeid = edgeid;
					pbestxtri = *ppxtri;
					quality0 = f;
				}
			}
		} // while( pbestxtri )... To fill up an Area with XQuads
		// 'pbestxtri==NULL' That means, the Current Area is finalized.
		// Lets doing some checks in debug mode...
#ifdef _DEBUG
		ppxquad = (NUT_XQUAD **)pxarea->XQuadPtrArray.pFirst;
		for (n = pxarea->XQuadPtrArray.Size; n != 0; n--, ppxquad++)
		{
			NErrorIf((*ppxquad)->pXArea != pxarea, NERROR_INCONSISTENT_PARAMETERS);
		}
#endif

		// Is it necessary to initialized a new Area ?
		// To know that, try to find the best XTri ( the one with the best quality to make a Quad )
		// in the part of the triangulated mesh not use yet.
		// there are 2 ways to do this, according with the maximum valence of the Xtri edges.
		//
		// If user specify a max valence of, then just parse all the Xtris not use yet to find
		// the best candidate for a "new first Xquad" ( "first" into a new area ...)
		if (ISFLAG_ON(flags, FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2))
		{
			quality0 = quality_threshold;
			pxtri = (NUT_XTRI *)pxtriarray->XTriArray.pFirst;
			for (n = pxtriarray->XTriArray.Size; n != 0; n--, pxtri++)
			{
				f = _get_best_available_edge(pxtri, &edgeid);
				if (f >= quality0)
				{
					bestedgeid = edgeid;
					pbestxtri = pxtri;
					quality0 = f;
				}
			}
		}
		// Without any kind of hint about Xtri edges valence, we need to update all the adjacent for all unused xtri.
		// It's important to do that because one edge may have more than 2 xtris within.
		// But only 2 of them may be specified at a time, the 2 best ones.
		// So in the previous loop, XQuads were built and Xtri used. So now, it's possible that these used Xtri make room
		// for another unused Xtri, which were not as good as the used ones in the previous loop.
		// But because now these previous "best ones" are used ... place is free.
		else
		{
			pbestxtri = _update_xtri_adjacents((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
			if (pbestxtri && (_get_best_available_edge(pbestxtri, &bestedgeid) < quality_threshold))
			{
				pbestxtri = NULL;
			}
		}
	} // while(pbestxtri)... To create a new area

	NClearArray(&tmparray_of_pxtri, NULL);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + FINISHING
	// + ... Debug checking and buffer freeing
	// +
	// checking (debug only) ... all the isolated triangle has to be ... isolated. Let's check that.
	// ... If not ... This algorithm need to be refresh !!!!
#ifdef _DEBUG
/*
	pxtri = (NUT_XTRI*)pxtriarray->XTriArray.pFirst;
	for(n=pxtriarray->XTriArray.Size;n!=0;n--,pxtri++)
	{
		if( ISFLAG_OFF(pxtri->Flags,FLAG_NUT_XTRI_USED_BY_QUAD) )
		{
			for(a=0;a<3;a++)
			{
				NErrorIf(pxtri->pAdjXTri[a]!=NULL && ISFLAG_OFF(pxtri->pAdjXTri[a]->Flags,FLAG_NUT_XTRI_USED_BY_QUAD),NERROR_SYSTEM_GURU_MEDITATION);
			}
		}
	}
*/
#endif

	return pout_xareaarray;
}

// ------------------------------------------------------------------------------------------
// void NUT_BuildXQuadArray
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//		A Nu32 result code.
//
//
//		const Nf32		cosphi_min		Cosine of the max angle accepted between 2 triangles of an XQUAD.
//										This coefficient is used to check coplanarity of 2 triangles.
//										If the cosine of the angle between 2 triangles is less than this minimum
//										then the 2 triangles are rejected.
//											cosine(0)	=  1			if you want ONLY consider PERFECT coplanar triangles.
//											cosine(PI/2)=  0			if you want consider a wide range of triangles. Actually all the ones
//																		which are not facing in an opposite direction.
//											cosine(PI)	= -1(or less)	if you want to accept everything.
// ------------------------------------------------------------------------------------------
NUT_XQUAD_ARRAY *NUT_BuildXQuadArray(NUT_XQUAD_ARRAY *pout_xquadarray, const NUT_XTRI_ARRAY *pxtriarray, const Nf32 xquad_quality, const Nf32 cosphi_min, const Nu32 flags)
{
	NErrorIf(!pout_xquadarray, NERROR_NULL_POINTER);
	NErrorIf(!pxtriarray, NERROR_NULL_POINTER);
	NErrorIf(xquad_quality < 0.0f || xquad_quality > 1.0f, NERROR_WRONG_VALUE); // Not really a bug, just to take care... as you can see, function are going

	Nf32 quality_threshold = NCLAMP(0.0f, xquad_quality, 1.0f); // to clamp "xquad_quality" so ...
	// Nu32			builded_xquad_number;
	Nu32 build;
	Nu32 n;
	NUT_XTRI *pxtri, *pbestxtri;

	// 	Nu32			alonepoint[3]={2,0,1};					// Index of the "alone point" for each "edge". ( the third point which is not used to define the edge )
	// 	Nu32			edgepoint[3][2]={ {0,1},{1,2},{2,0} };	// Index of the "2 used points" for each "edge". ( the first and second point which are used to define the edge )
	Nu32 bestedgeid;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup XQuads Array ?
	// +	There are several possibilities for a maximum flexibility.
	// +	1/	"pout_xquadarray" is full of ZERO. It will be setup with a capacity equal to the maximum number of buildable quad based
	// +		on the incoming Xtris Array Size.
	// +
	if (pout_xquadarray->XQuadArray.ElementSize == 0)
	{
		NUT_SetupXQuadArray(pout_xquadarray, pxtriarray->XTriArray.Size / 2);
	}
	else if (pout_xquadarray->XQuadArray.ElementSize == sizeof(NUT_XQUAD))
	{
		if (pout_xquadarray->XQuadArray.Size == pout_xquadarray->XQuadArray.Capacity)
			NSetArrayCapacity((NARRAY *)pout_xquadarray, pout_xquadarray->XQuadArray.Size + pxtriarray->XTriArray.Size / 2, NULL);
	}
	else
	{
		NErrorIf(1, NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + BUILD QUADS ARRAY
	// +	Each XTri is associated with it's best adjacent neighbor (among 3 possibilities, one by edge)
	// +	The resulted XQuad is preserved only if it's Quality is equal or better
	// +	to the requested user quality.
	// +	This method doesn't guarantee to choose the best Xtri to Xquad distribution ...
	// +	But any way ... it's simple ...
	// +
	// builded_xquad_number = 0;
	if (ISFLAG_ON(flags, FLAG_NUT_BUILDXMESH_HINT_EDGE_VALENCE_MAX_2))
	{
		pbestxtri = _update_xtri_adjacents_optimized((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
		if (pbestxtri && _get_best_available_edge(pbestxtri, &bestedgeid) >= quality_threshold)
		{
			pxtri = (NUT_XTRI *)pxtriarray->XTriArray.pFirst;
			for (n = pxtriarray->XTriArray.Size; n != 0; n--, pxtri++)
			{
				if (ISFLAG_OFF(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
				{
					if (_get_best_available_edge(pxtri, &bestedgeid) >= quality_threshold)
					{
						// One more xquad.
						_push_xquad_to_array(pout_xquadarray, pxtri, bestedgeid);
						// builded_xquad_number += 1;
					}
				}
			}
		}
	}
	else
	{
		do
		{
			build = 0;
			pbestxtri = _update_xtri_adjacents((NUT_XTRI *)pxtriarray->XTriArray.pFirst, pxtriarray->XTriArray.Size, cosphi_min);
			if (pbestxtri && _get_best_available_edge(pbestxtri, &bestedgeid) >= quality_threshold)
			{
				pxtri = (NUT_XTRI *)pxtriarray->XTriArray.pFirst;
				for (n = pxtriarray->XTriArray.Size; n != 0; n--, pxtri++)
				{
					if (ISFLAG_OFF(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
					{
						if (_get_best_available_edge(pxtri, &bestedgeid) >= quality_threshold)
						{
							_push_xquad_to_array(pout_xquadarray, pxtri, bestedgeid);
							// One more xquad.
							// builded_xquad_number += 1;
							build = 1;
						}
					}
				}
			}
		} while (build != 0);
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + FINISHING
	// + ... Debug checking and buffer freeing
	// +
	// checking (debug only) ... all the isolated triangle has to be ... isolated. Let's check that.
	// ... If not ... This algorithm need to be refresh !!!!
#ifdef _DEBUG
	Nu32 a;
	pxtri = (NUT_XTRI *)pxtriarray->XTriArray.pFirst;
	for (n = pxtriarray->XTriArray.Size; n != 0; n--, pxtri++)
	{
		if (ISFLAG_OFF(pxtri->Flags, FLAG_NUT_XTRI_USED_BY_QUAD))
		{
			for (a = 0; a < 3; a++)
			{
				NErrorIf(pxtri->pAdjXTri[a] != NULL && ISFLAG_OFF(pxtri->pAdjXTri[a]->Flags, FLAG_NUT_XTRI_USED_BY_QUAD) && pxtri->EdgeQuality[a] >= quality_threshold, NERROR_SYSTEM_GURU_MEDITATION);
			}
		}
	}
#endif
	return pout_xquadarray;
}
