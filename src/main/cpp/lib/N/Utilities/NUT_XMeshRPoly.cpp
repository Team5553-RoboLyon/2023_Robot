#include "../NCStandard.h"
#include "../Containers/NArray.h"
#include "../Geometry/NGeometryMisc.h"
#include "../Maths/NMatrix3x3.h"
#include "../NErrorHandling.h"
#include "NUT_XMesh.h"

// ------------------------------------------------------------------------------------------
// static void _clear_patch_element
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
static void _clear_rpoly_element(void* ppxquad)
{
	NErrorIf((*((NUT_XQUAD**)ppxquad))->pXRPoly == NULL,NERROR_SYSTEM_GURU_MEDITATION); // ??? It's not suppose to happen ! Never ! pxquad->pXRPoly is suppose to be equal to the Index of its XRpoly
	FLAG_OFF((*((NUT_XQUAD**)ppxquad))->Flags,FLAG_NUT_XQUAD_IN_RPOLY);
	(*((NUT_XQUAD**)ppxquad))->pXRPoly = NULL;
/*
	not necessary to this, a proper reset of these values is done before each queue boot
	pxquad->XBuildGridCoords.x	= 0;
	pxquad->XBuildGridCoords.y	= 0;
	pxquad->GridCoordsInfos = 0;
*/
}
// ------------------------------------------------------------------------------------------
// void	NUT_ClearXRPoly
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
void	NUT_ClearXRPoly(NUT_XRPOLY* pxrpoly)
{
	NClearArray((NARRAY*)pxrpoly,_clear_rpoly_element);
}
// ------------------------------------------------------------------------------------------
// void	NUT_DeleteXRPoly
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
void	NUT_DeleteXRPoly(NUT_XRPOLY* pxrpoly)
{
	NDeleteArray((NARRAY*)pxrpoly,_clear_rpoly_element);
}
// ------------------------------------------------------------------------------------------
// static void _clear_patcharray_element
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
static void _clear_rpolyarray_element(void* pxrpoly)
{
	NUT_ClearXRPoly((NUT_XRPOLY*)pxrpoly);
}
// ------------------------------------------------------------------------------------------
// void NUT_ClearXRPolyArray
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
void NUT_ClearXRPolyArray(NUT_XRPOLY_ARRAY* pxrpolyarray)
{
	NClearArray((NARRAY*)pxrpolyarray,_clear_rpolyarray_element);
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
void NUT_DeleteXAreaArray(NUT_XRPOLY_ARRAY* pxrpolyarray)
{
	NDeleteArray((NARRAY*)pxrpolyarray,_clear_rpolyarray_element);
}
// ------------------------------------------------------------------------------------------
// NUT_XQUAD*	_getfirstavailable_xarea_xquad
// ------------------------------------------------------------------------------------------
// Description :
//	Available Only inside this .c file
//	This inline function parse a NUT_XAREA and return the first available XQUAD.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
inline NUT_XQUAD*	_getfirstavailable_xarea_xquad(const NUT_XAREA *pxarea)
{
	NUT_XQUAD	*pxquad;	
	NUT_XQUAD	**ppxquad = (NUT_XQUAD**)pxarea->XQuadPtrArray.pFirst;
	for(Nu32 i=pxarea->XQuadPtrArray.Size;i--;i!=0,ppxquad++)
	{
		pxquad = *ppxquad;
		NErrorIf(pxquad->pXArea != pxarea, NERROR_XQUAD_NOT_IN_AREA );
		
		// To be valid an XQuad has to be:	1)Inside the XArea (it is!)
		//									2)Its Flag "FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK" has to be set to ON (it should be, because is in the pXArea))
		//									3)Its Flag "FLAG_NUT_XQUAD_IN_RPOLY" has to be set to OFF ( that means, not already used by an XRPOLY) 
		if(  pxquad && FLAGS_TEST(pxquad->Flags,FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK|FLAG_NUT_XQUAD_IN_RPOLY,FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK) )
		{
			NErrorIf(pxquad->pXRPoly, NERROR_XQUAD_ALREADY_IN_RPOLY );// ->pXRPoly should be NULL because 'FLAG_NUT_XQUAD_IN_RPOLY' is OFF
			
			return pxquad;
		}
	}
	return NULL;
}
// ------------------------------------------------------------------------------------------
// void _clean_xrpoly
// ------------------------------------------------------------------------------------------
// Description :
//	Available Only inside this .c file
//	This inline function works like NEraseArrayElementsBatch.
//	... instead removing XQUAD one by one from xrpoly.
//	It seems quicker to tag XQUAD and remove them all together at the same time.
//	By this way it's possible to reduce the number of "memcpy" calls... and save time.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _clean_xrpoly(NUT_XRPOLY *pxrpoly)
{
	NUT_XQUAD	**ppxquad	= (NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst;
	Nu32		patchsize	= pxrpoly->XQuadPtrArray.Size;
	Nu32		n			= 0;	
	NUT_XQUAD	**pdst,**psrc;
	Nu32		size = 0;

	// Look for the first "to delete pXQuad" element. 
	while((*ppxquad)->pXRPoly == pxrpoly || n<patchsize ){ppxquad++;n++;};
	pdst = ppxquad; 

	while(n<patchsize)
	{
		// Look for the first "to keep" element  
		while((*ppxquad)->pXRPoly != pxrpoly || n<patchsize ){ppxquad++;n++;};
		psrc = ppxquad;size = n;
		// Look for the Next "to delete" element and Size to copy 
		while((*ppxquad)->pXRPoly == pxrpoly || n<patchsize ){ppxquad++;n++;};
		size = n-size;

		memcpy(pdst,psrc,size*sizeof(NUT_XQUAD*));
		pdst += size;
		// add size to pdst at the end ! Like this,  at the loop end
		// 'pdst' is going to be the next address after the last valid One !!!
		// It seems weird but it's not ! 
		// We are going to use it to calculate the final size of the Patch... 
	}
	// Calculate new XRPoly size...
	pxrpoly->XQuadPtrArray.Size = (Nu32)(pdst - (NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst);
}

// ------------------------------------------------------------------------------------------
// NUT_XQUADNODE * _setup_xqseed
// ------------------------------------------------------------------------------------------
// Description :
//	Initialize an XQUADNODE to insert the first XQUAD into an XRPOLY.
//	This XQUADNODE is suppose to be the first one of the QUEUE where it will be inserted in.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline NUT_XQUADNODE* _setup_xqseed(NUT_XQUADNODE *pxqnode, NUT_XQUAD *pxquad)
{
	Nmem0(pxqnode,NUT_XQUADNODE);
	pxqnode->pXTri			= pxquad->pXTri[0];										//|
	pxqnode->XTriEdge		= NGET_NEXT_XTRI_EDGE(pxqnode->pXTri->QuadDiagonal);	//|- To Orient the XQuadNode (...and associate XquadEdge and XtriEdge )
	pxqnode->XQuadEdge		= 0; // ( _XEAST = 0 )									//|
	
	pxqnode->XGridCoords.x	= 0;
	pxqnode->XGridCoords.y	= 0;

	return pxqnode;
}
// ------------------------------------------------------------------------------------------
// void _boot_xquadnode_queue_with_populated_xrpoly
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is creating all the XQuadNodes that surrounded the pxrpoly 'border'
//	and push all of them into the queue.
//	After processing several XQuadNodes may be in the queue. Note it's possible to have no XQuadNode
//	pushed into the Queue if there are no available XQUAD arrount the XRPoly Border.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _boot_xquadnode_queue_with_populated_xrpoly( NARRAY *pqueue_of_xquadnode,NUT_XRPOLY *pxrpoly )
{
	NErrorIf(!pxrpoly||!pqueue_of_xquadnode,NERROR_NULL_POINTER);
	NErrorIf(pxrpoly->XQuadPtrArray.Size == 0, NERROR_XMESH_RPOLY_PROCESSING_RPOLY_EMPTY);
	NErrorIf(pqueue_of_xquadnode->Size != 0, NERROR_XMESH_RPOLY_PROCESSING_QUEUE_NOT_EMPTY);

	NUT_XTRI		*pxtri;
	NUT_XQUADNODE	*pxqnode;
	NUT_XQUADNODE	xqnode;
	Nu32			i;
	NUT_XQUAD		*pxquad;
	NARRAY			subqueue;
	Nu32			xquadedge,xtriedge;
	NVEC2s32		parentrelativegridcoords[4]={{1,0},{0,1},{-1,0},{0,-1}};

	
	NUT_ResetXRPolyXQuadXBuildData(pxrpoly);
	
	pxquad = *(NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst;
	_setup_xqseed(&xqnode,pxquad);
	NSetupArray(&subqueue,pxrpoly->XQuadPtrArray.Size,sizeof(NUT_XQUADNODE));
	NArrayPushBack(&subqueue,(NBYTE*)&xqnode);
	
	while(subqueue.Size)
	{
		pxqnode = (NUT_XQUADNODE*)subqueue.pFirst;
		
		// No XQuad Push BAck into an XRPoly here, XQuad is already at the right place !
		// ...
		// Propagate and dispatch xqnode in the appropriate Queue (sub or main )
		xquadedge	= pxqnode->XQuadEdge;
		xtriedge	= pxqnode->XTriEdge;
		pxtri		= pxqnode->pXTri;

		for(i=4;i!=0;i--)
		{
			if(!NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(pxtri->pXQuad->XBuildInfos,xquadedge))
			{
				xqnode.pXTri	= pxtri->pAdjXTri[xtriedge];
				
				if( !xqnode.pXTri )
				{
					NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
				}
				else
				{
					if( ISFLAG_OFF(xqnode.pXTri->Flags,FLAG_NUT_XTRI_PATCH_PROCESS_LOCK) ) // means "xqnode.pXTri->pXQuad = NULL" or "ISFLAG_OFF(xqnode.pXTri->pXQuad->Flags,FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK)" have a lock at the init stage of this function )
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
					}
					else if( xqnode.pXTri->pXQuad->pXRPoly == pxrpoly )
					{
						xqnode.XTriEdge			= xqnode.pXTri->AdjEdge[xtriedge];
						xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
						xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
						xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);

						NArrayPushBack(&subqueue,(NBYTE*)&xqnode);
					}
					else if( xqnode.pXTri->pXQuad->pXRPoly == NULL )
					{
						xqnode.XTriEdge			= xqnode.pXTri->AdjEdge[xtriedge];
						xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
						xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
						xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);

						NArrayPushBack(pqueue_of_xquadnode,(NBYTE*)&xqnode);
					}
					else
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
					}

				}
			}

			// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
			xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
			if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
			{
				pxtri	= pxtri->pAssociatedXtri;
				xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
			}
			else
			{
				xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
			}
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++
		// + Remove pxqnode from the Queue ( it is the first one of the list... )
		// + Because of the possible memory reallocation we prefer using it's index (0) instead of 'pxqnode' pointer !
		// +
		NEraseArrayElement(&subqueue,0,NULL);
	}

	NClearArray(&subqueue,NULL);
}

// ------------------------------------------------------------------------------------------
// void _boot_xquadnode_queue_with_seed
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
static inline void _boot_xquadnode_queue_with_seed( NARRAY *pqueue_of_xquadnode,NUT_XQUADNODE *pseed )
{
	NErrorIf(!pseed||!pqueue_of_xquadnode,NERROR_NULL_POINTER);
	NErrorIf(pqueue_of_xquadnode->Size != 0, NERROR_XMESH_RPOLY_PROCESSING_QUEUE_NOT_EMPTY);

	NArrayPushBack(pqueue_of_xquadnode,(NBYTE*)pseed);
}
// ------------------------------------------------------------------------------------------
// void _push_xquad_to_xrpoly 
// ------------------------------------------------------------------------------------------
// Description :
//	Perform all the XQuad members update (Grid Coordinates, pXRPoly pointer, Flags) AND ...
//	... Push an XQUAD into a Patch ( push a pointer on the XQuad actually, not the XQuad itself).
//	!IMPORTANT ! This function doesn't remove XQuad from it's previous potential XRPoly.
//	( Using the function "_clean_xrpoly" in the right way will take care of this. )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _push_xquad_to_xrpoly(NUT_XRPOLY *pxrpoly, const NUT_XQUADNODE *pxqnode ) 
{
	NErrorIf(!pxrpoly||!pxqnode,NERROR_NULL_POINTER);
	NErrorIf(pxqnode->pXTri->pXQuad->pXRPoly == pxrpoly,NERROR_XMESH_XQUAD_ALREADY_INSERTED_IN);

	pxqnode->pXTri->pXQuad->XBuildGridCoords	= pxqnode->XGridCoords;
	pxqnode->pXTri->pXQuad->pXRPoly		= pxrpoly;
	FLAG_ON(pxqnode->pXTri->pXQuad->Flags,FLAG_NUT_XQUAD_IN_RPOLY);
	NUT_XQUAD_XBUILDINFOS_GRIDCOORDS_COMPUTED(pxqnode->pXTri->pXQuad->XBuildInfos); // Notes: this flag and FLAG_NUT_XQUAD_IN_RPOLY are not redundant. Because of patch split ...
	NArrayPushBack(&pxrpoly->XQuadPtrArray,(NBYTE*)&pxqnode->pXTri->pXQuad);
}



// ------------------------------------------------------------------------------------------
// Nu32 _get_direction_mask_from_xquadnode_error
// ------------------------------------------------------------------------------------------
// Description :
//	When an xrpoly building error occurs (which is not a bug, but a normal situation we have to deal with),
//	N split the current xrpoly in two ones, with a simple rule: build the second patch from the current
//	one by extracting its XQuads in all directions BUT the one where initial error came from.
//	Doing this suppose to identify the forbidden direction in the initial "seed" base where 
//	XQuadEdge 0 represents the next diagonal edge of the first quad triangle.
//			Reminders about XQuadNode Seed building:
//						XQuadNode.XQuadEdge		= 0; ( _XEAST = 0 )
//						XQuadNode.pXTri			= pxquad->pXTri[0];
//						XQuadNode.XTriEdge		= NGET_NEXT_XTRI_EDGE(XQuadNode.pXTri->QuadDiagonal);
//
// IMPORTANT NOTE:
//	The XQuadNode which generates an error may have a non-consistent .XQuadEdge regarding the whole patch.
//	(like around a vertex with a valence of 3, where directions between the quads sharing this vertex are not consistent).
//	BUT XQuadNode.pXTri AND XQuadNode.XTriEdge members are always consistent.
//	So this function is going to work from these 2 consistent members to recover the right .XQuadEdge into "Seed" Base.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline Nu32 _get_direction_mask_from_xquadnode_error(NUT_XQUADNODE *pxqnode_error)
{
	NUT_XTRI	*pxtri		= pxqnode_error->pXTri->pXQuad->pXTri[0];
	Nu8			xtriedge	= NGET_NEXT_XTRI_EDGE(pxtri->QuadDiagonal);

	for(Nu32 xquadedge=0;xquadedge<4;xquadedge++)
	{
		if( (pxtri == pxqnode_error->pXTri) && (xtriedge == pxqnode_error->XTriEdge) )
		{
			return (Nu32)(1<<xquadedge);
		}

		// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
		if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
		{
			pxtri	= pxtri->pAssociatedXtri;
			xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
		}
		else
		{
			xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
		}
	}

	// We are not suppose to be here !
	// That means, 'pxqnode_error->pXTri' AND 'pxqnode_error->XTriEdge' don't match together with one of the XQUAD Edge ???
	// ... It's a BUG !
	NErrorIf(1,NERROR_XMESH_INCONSISTENCY); 
	return 0xF; // In release mode it will mask all directions !!!
}

// ------------------------------------------------------------------------------------------
// void _split_xrpoly_and_boot_xquadnode_queue 
// ------------------------------------------------------------------------------------------
// Description :
//	Extract a new patch (pxrpoly_dst) from a source xrpoly (pxrpoly_src) and boot xquad node queue
//	with the pxrpoly_dst at the same time, exactly like "_boot_xquadnode_queue_with_populated_xrpoly"
//	would do it, but in the same loop so... faster. 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void _split_xrpoly_and_boot_xquadnode_queue( NARRAY *pqueue_of_xquadnode,NUT_XRPOLY *pxrpoly_dst,NUT_XRPOLY *pxrpoly_src,NUT_XQUAD *pfirstextracted_xquad, const Nu32 mask_direction_seed_base )
{
	NErrorIf(!pqueue_of_xquadnode||!pxrpoly_dst||!pxrpoly_src||!pfirstextracted_xquad,NERROR_NULL_POINTER);
	NErrorIf(pxrpoly_dst->XQuadPtrArray.Size != 0, NERROR_XMESH_RPOLY_PROCESSING_RPOLY_NOT_EMPTY);
	NErrorIf(pxrpoly_src->XQuadPtrArray.Size == 0, NERROR_XMESH_RPOLY_PROCESSING_RPOLY_EMPTY);
	NErrorIf(pqueue_of_xquadnode->Size != 0, NERROR_XMESH_RPOLY_PROCESSING_QUEUE_NOT_EMPTY);
	NErrorIf(pfirstextracted_xquad->pXRPoly != pxrpoly_src, NERROR_XMESH_INCONSISTENCY);

	NUT_XTRI		*pxtri;
	NUT_XQUADNODE	*pxqnode;
	NUT_XQUADNODE	xqnode;
	Nu32			i;
	NUT_XQUAD		*pxquad;
	NARRAY			subqueue;
	Nu32			xquadedge,xtriedge;
	NVEC2s32		parentrelativegridcoords[4]={{1,0},{0,1},{-1,0},{0,-1}};

	#ifdef _DEBUG
	Nu32 dbg_pxrpoly_src_size = pxrpoly_src->XQuadPtrArray.Size;
	#endif

	NUT_ResetXRPolyXQuadXBuildData(pxrpoly_src);

	pxquad = pfirstextracted_xquad;
	_setup_xqseed(&xqnode,pxquad);
	NSetupArray(&subqueue,pxrpoly_src->XQuadPtrArray.Size,sizeof(NUT_XQUADNODE));
	NArrayPushBack(&subqueue,(NBYTE*)&xqnode);

	while(subqueue.Size)
	{
		pxqnode = (NUT_XQUADNODE*)subqueue.pFirst;

		if( NUT_XQUAD_XBUILDINFOS_IS_GRIDCOORDS_COMPUTED(pxqnode->pXTri->pXQuad->XBuildInfos) )
		{
			// ! GRID COORDS ERROR !
			// Incoming grid coordinates don't match with the ones that are already calculated and store into this Xquad...
			// This is the result of a non uniform XQuads repartition. 
			// Here we are inside SPLIT PROCESS , dealing with patch already made ... So ... it's not suppose to happen !
			// In that case ... generate an error in debug mode.
			if( (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) || (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) )
			{
				NErrorIf(1,NERROR_XMESH_INCONSISTENCY);
				NClearArray(&subqueue,NULL);
				return;
			}
			// else
			//{
			//	Nothing to do, it happens what it suppose to happen, which is: Grid Coordinates are consistent	
			//}	
		}
		else
		{
			_push_xquad_to_xrpoly(pxrpoly_dst,pxqnode);
			// Note:
			// N is not going to remove XQUAD from pxrpoly_src right now.
			// All XQuads removals are going to be done at the end of this process by a call to "_clean_xrpoly" function.
		}
		
		// Propagate and dispatch xqnode in the appropriate Queue (sub or main )
		xquadedge	= pxqnode->XQuadEdge;
		xtriedge	= pxqnode->XTriEdge;
		pxtri		= pxqnode->pXTri;

		for(i=4;i!=0;i--)
		{
			if( !NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(pxtri->pXQuad->XBuildInfos,xquadedge) )
			{
				xqnode.pXTri	= pxtri->pAdjXTri[xtriedge];
				if( !xqnode.pXTri )
				{
					NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
				}
				else
				{
					if( ISFLAG_OFF(xqnode.pXTri->Flags,FLAG_NUT_XTRI_PATCH_PROCESS_LOCK) ) // means "xqnode.pXTri->pXQuad = NULL" or "ISFLAG_OFF(xqnode.pXTri->pXQuad->Flags,FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK)" have a lock at the init stage of this function )
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
					}
					// ... mask_direction_seed_base applies only to XQUADs from pspatch_src (+ pspatch_dst, but these ones are the ones from pspatch_src ... so )
					else if( (!(mask_direction_seed_base &(1<<xquadedge))) && (xqnode.pXTri->pXQuad->pXRPoly == pxrpoly_src || xqnode.pXTri->pXQuad->pXRPoly == pxrpoly_dst) )
					{
						xqnode.XTriEdge			= xqnode.pXTri->AdjEdge[xtriedge];
						xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
						xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
						xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);

						NArrayPushBack(&subqueue,(NBYTE*)&xqnode);
					}
					// ... mask_direction_seed_base is not apply to XQUADS out of pxrpoly_src
					else if( xqnode.pXTri->pXQuad->pXRPoly == NULL )
					{
						xqnode.XTriEdge			= xqnode.pXTri->AdjEdge[xtriedge];
						xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
						xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
						xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);

						NArrayPushBack(pqueue_of_xquadnode,(NBYTE*)&xqnode);
					}
					// ... All the others cases are XQUADS from another XRPoly or Another XArea
					else
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
					}
				}
			}

			// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
			xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
			if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
			{
				pxtri	= pxtri->pAssociatedXtri;
				xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
			}
			else
			{
				xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
			}
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++
		// + Remove pxqnode from the Queue ( it is the first one of the list... )
		// + Because of the possible memory reallocation we prefer using it's index (0) instead of 'pxqnode' pointer !
		// +
		NEraseArrayElement(&subqueue,0,NULL);
	}

	// Clean Source Patch, that means remove all the XQUADS which are not belongs to it any more ...
	_clean_xrpoly(pxrpoly_src);

	NErrorIf(pxrpoly_src->XQuadPtrArray.Size + pxrpoly_dst->XQuadPtrArray.Size != dbg_pxrpoly_src_size, NERROR_XMESH_INCONSISTENCY );
	NClearArray(&subqueue,NULL);
}

// ------------------------------------------------------------------------------------------
// Ns32 _xrpoly_qsort_compare
// ------------------------------------------------------------------------------------------
// Description :
//	callback function used to sort XRPolyes inside the out coming array of xrpoly
//	used by the function 'NUT_BuildXRPolyArrayFromXArea'
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//			pa	<	pb return -1
//			pa	>	pb return  1
//			pa	==	pb return  0
//
// ------------------------------------------------------------------------------------------
Ns32 _xrpoly_qsort_compare(const void *pa, const void *pb)
{
	//return(((NARRAY*)pa)->Size - ((NARRAY*)pb)->Size );
	
	if(((NARRAY*)pa)->Size > ((NARRAY*)pb)->Size )
		return 1;
	else if(((NARRAY*)pa)->Size < ((NARRAY*)pb)->Size )
		return -1;
	else
		return 0;
}

// ------------------------------------------------------------------------------------------
// Nu32 _getxrpoly_commonedges_count
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
Nu32 _getxrpoly_commonedges_count(const NUT_XRPOLY *pxrpoly1,const NUT_XRPOLY *pxrpoly2,NUT_XQUAD **ppneighbor_xquad)
{
	NErrorIf(!pxrpoly1||!pxrpoly2,NERROR_NULL_POINTER);
	Nu32		i,j;
	Nu32		nb = 0;
	NUT_XQUAD	**ppxquad;
	NUT_XQUAD	*pxquad;
	NUT_XRPOLY	*pxrpoly,*pxrpoly_adj;

	// +-+ Identify the Patch with the big number of XQuads 
	if(pxrpoly2->XQuadPtrArray.Size > pxrpoly1->XQuadPtrArray.Size)
	{
		pxrpoly		= (NUT_XRPOLY*)pxrpoly1;
		pxrpoly_adj = (NUT_XRPOLY*)pxrpoly2;
	}
	else
	{
		pxrpoly		= (NUT_XRPOLY*)pxrpoly2;
		pxrpoly_adj = (NUT_XRPOLY*)pxrpoly1;
	}

	// +-+ Flag all the XTris which belong to the XRPoly with the less number of XQuads
	ppxquad = (NUT_XQUAD**)pxrpoly_adj->XQuadPtrArray.pFirst;
	for(i=pxrpoly_adj->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;

		NErrorIf(ISFLAG_ON(pxquad->pXTri[0]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK),NERROR_XTRI_ALREADY_LOCKED);
		NErrorIf(ISFLAG_ON(pxquad->pXTri[1]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK),NERROR_XTRI_ALREADY_LOCKED);

		FLAG_ON(pxquad->pXTri[0]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK);
		FLAG_ON(pxquad->pXTri[1]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK);
	}

	// +-+ From the 2 XTris of each XQuads of the XRPoly check the flag of adjacent Xtri if there are
	ppxquad = (NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst;
	for(i=pxrpoly->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;

		for(j=0;j<3;j++)
		{
			if(pxquad->pXTri[0]->pAdjXTri[j] && ISFLAG_ON(pxquad->pXTri[0]->pAdjXTri[j]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK))
			{
				*ppneighbor_xquad = pxquad;
				nb++;
			}
			if(pxquad->pXTri[1]->pAdjXTri[j] && ISFLAG_ON(pxquad->pXTri[1]->pAdjXTri[j]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK))
			{
				*ppneighbor_xquad = pxquad;
				nb++;
			}
		}
	}

	// +-+ UnFlag all the XTris which belong to the XRPoly with the less number of XQuads
	ppxquad = (NUT_XQUAD**)pxrpoly_adj->XQuadPtrArray.pFirst;
	for(i=pxrpoly_adj->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;
		NErrorIf(ISFLAG_OFF(pxquad->pXTri[0]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK),NERROR_XTRI_UNLOCKED);
		NErrorIf(ISFLAG_OFF(pxquad->pXTri[1]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK),NERROR_XTRI_UNLOCKED);

		FLAG_OFF(pxquad->pXTri[0]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK);
		FLAG_OFF(pxquad->pXTri[1]->Flags,FLAG_NUT_XTRI_RPOLY_COMMON_EDGE_COUNT_PROCESS_LOCK);
	}

	return nb;
}

// ------------------------------------------------------------------------------------------
// void NUT_ReAddressing_XRPoly_Array
// ------------------------------------------------------------------------------------------
// Description :
//		Due to memory reallocation (create a new XRPOLY and add it into the NUT_XRPOLY_ARRAY )
//		OR Due to XRPoly sorting ...
//		.pXRPoly stored into each XQUAD as to be re-addressed (potentially).
//		Notice that re-addressing is going to be done only if it seems necessary.
//		... Function is going to check the first XQUAD of each XRPoly and perform
//		re-addressing (or not) in accordance with the .pXRPoly of this first XQuad ( all the XQuads of one XRPoly
//		are suppose to be set in the same way ...)
//		... SO A TRICK to ENFORCE readdressing ALL XQUADS of a specific XRPOLY : Set it's first XQUAD.pXRPoly to NULL !!
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_ReAddressing_XRPoly_Array(NUT_XRPOLY_ARRAY* pxrpolyarray)
{
	Nu32		n,p;
	NUT_XRPOLY	*pxrpoly;
	NUT_XQUAD	**ppxquad;

	pxrpoly = (NUT_XRPOLY*)pxrpolyarray->XRPolyArray.pFirst;
	for(n=pxrpolyarray->XRPolyArray.Size;n!=0;n--,pxrpoly++)
	{
		if(pxrpoly->XQuadPtrArray.Size)
		{
			ppxquad = (NUT_XQUAD**)pxrpoly->XQuadPtrArray.pFirst;
			// All the XQUAD included need to have a valid pXRPoly pointer.
			// If not, address it for all of them !
			if( (*ppxquad)->pXRPoly != pxrpoly )
			{
				for(p=pxrpoly->XQuadPtrArray.Size;p!=0;p--,ppxquad++)
				{
					(*ppxquad)->pXRPoly = pxrpoly;
				}
			}
		}
	}
}



// ------------------------------------------------------------------------------------------
// Nu32 NUT_BuildXRPolyArrayFromXArea
// ------------------------------------------------------------------------------------------
// Description :
//	Build Rectilinear Polygons of XQuads from One single NUT_XAREA (an XQuads AREA).
//	Notice that during this process there is NO XQUAD CREATION. But just storage of a pointer 
//	on already build XQuad ( already build during the XArea building process ). 
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_XRPOLY_ARRAY		*pout_xrpolyarray	The array where all the built XRPolys are going to be store
//			const NUT_XAREA			*pxarea				The Area of XQuads. Previously built with 'NUT_BuildXAreaArray'
//			const NUT_XQUAD_ARRAY	*pxquadarray		The Array of XQuads linked and built with the XArea ( 'NUT_BuildXAreaArray' build XAreas which are arrays of XQuads Pointers, AND XQuads Array which is an array of XQuads)
//			const NUT_XTRI_ARRAY	*pxtriarray 		The Array of XTris built with 'NUT_BuildXTriArray' and also used to build XQuads.
// 
// Out :
// ------------------------------------------------------------------------------------------
void NUT_BuildXRPolyArrayFromXArea( NUT_XRPOLY_ARRAY *pout_xrpolyarray, const NUT_XAREA *pxarea, const NUT_XQUAD_ARRAY *pxquadarray, const NUT_XTRI_ARRAY *pxtriarray )
{
	NErrorIf(!pout_xrpolyarray,NERROR_NULL_POINTER);
	NErrorIf(!pxarea,NERROR_NULL_POINTER);
	NErrorIf(!pxquadarray,NERROR_NULL_POINTER);
	NErrorIf(!pxtriarray,NERROR_NULL_POINTER);

	NUT_XTRI		*pxtri;
	NUT_XQUAD		**ppxquad;
	NUT_XQUAD		*pxquad;
	
	NUT_XQUADNODE	*pxqnode;
	NUT_XQUADNODE	xqnode;

	NUT_XRPOLY		xrpoly;
	NUT_XRPOLY		*pcurrentopenxrpoly		= NULL;
	NUT_XRPOLY		*ppreviousxrpoly		= NULL;
	NUT_XRPOLY		*pxrpolya,*pxrpolyb,*pxrpoly;
	Nu32			currentopenxrpolyindex	= 0;	
	Nu32			firstopenxrpolyindex	= 0;	
	Nu32			first_inserted_xrpolyindex;
	Nu32			i,j,k;
	Nu32			xquadedge,xtriedge;
	NVEC2s32		parentrelativegridcoords[4]={{1,0},{0,1},{-1,0},{0,-1}};
	Nu32			remaining_xarea_xquad;
	Nu32			forbidden_direction_mask;
	Nu32			merging_success;

	// Temporary buffers
	NARRAY			xquadnode_queue;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + XQUAD and XTRI check and lock
// +
// + Propagation needs to stay 'inside' the 'XArea' which is an XQUAD pointers Array. 
// + But we are going to propagate  trough XTriangle Edges. To be sure to use only XTriangles of this Xarea Xquads,
// + We are going to flag them just for this process.
// +
// +-+ Flag XQUAD and XTRI as ..._PATCH_PROCESS_LOCK
// +
	ppxquad = (NUT_XQUAD**)pxarea->XQuadPtrArray.pFirst;
	for(i=pxarea->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;

		// Check some flags first
		NErrorIf( ISFLAG_OFF(pxquad->Flags, FLAG_NUT_XQUAD_IN_AREA),NERROR_XQUAD_NOT_IN_AREA );
		NErrorIf( ISFLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_IN_RPOLY),NERROR_XQUAD_ALREADY_IN_RPOLY );
		NErrorIf( pxquad->pXRPoly,NERROR_XQUAD_ALREADY_IN_RPOLY );
		NErrorIf( pxquad->pXArea != pxarea,NERROR_XMESH_INCONSISTENCY );

		NErrorIf( ISFLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK),NERROR_XQUAD_ALREADY_LOCKED );
		NErrorIf( ISFLAG_ON(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK),NERROR_XTRI_ALREADY_LOCKED );
		NErrorIf( ISFLAG_ON(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK),NERROR_XTRI_ALREADY_LOCKED );
		
		// Flag XQuadq and XTris as locked by the process.
		// Later in the process it will be easy to identify them !!!
		FLAG_ON(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK);
		FLAG_ON(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);
		FLAG_ON(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);

		// Check the 4 XQuad Edges and Close the ones that are on the Xarea 
	}
	// Get the number of XArea XQuads
	remaining_xarea_xquad = pxarea->XQuadPtrArray.Size;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + OUT PATCH ARRAY SETUP
// +
// +
// +-+	The Patch Array where all patches are going to be push into is the first incoming parameter of this function.
// +
// +	User may already setup it. For example if he wants to store into the same array all the patches from several XAreas ...
// +	( It has to be done in the right way ... we are going to check that some lines below ).
// +	OR,
// +	User may just pass a pointer on a valid, but cleared, NArray (cleared means FULL OF ZEROS).
// +
	if(pout_xrpolyarray->XRPolyArray.ElementSize == 0)
	{
		NUT_SetupXRPolyArray( pout_xrpolyarray,DEFAULT_NUT_RPOLY_ARRAY_CAPACITY );
	}
	else if(pout_xrpolyarray->XRPolyArray.ElementSize == sizeof(NUT_XRPOLY) )
	{
		#ifdef _DEBUG
		// pout_xrpolyarray is suppose to be an array of NUT_XRPOLY (== array of XQuads Pointers...)
		if(pout_xrpolyarray->XRPolyArray.Size !=  0 )
		{
			NErrorIf(((NARRAY*)NGetFirstArrayPtr((NARRAY*)pout_xrpolyarray))->ElementSize != sizeof(NUT_XQUAD*),NERROR_ARRAY_WRONG_ELEMENT_SIZE );
		}
		#endif

		if( pout_xrpolyarray->XRPolyArray.Size == pout_xrpolyarray->XRPolyArray.Capacity )
			NSetArrayCapacity((NARRAY*)pout_xrpolyarray,pout_xrpolyarray->XRPolyArray.Size + DEFAULT_NUT_RPOLY_ARRAY_CAPACITY, NULL);
	}
	else
	{
		NErrorIf(1,NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + TEMPORARY BUFFER(s) SETUP
// +
// +
// +-+	This function is going to use a temporary NArray for an explicitly queue-based implementation of the propagation 
// +	recursive process.
// +
	NSetupArray(&xquadnode_queue,pxarea->XQuadPtrArray.Size, sizeof(NUT_XQUADNODE));

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RPOLY DETECTION PROCESS
// +
// +-+	To define each Rectilinear Polygon N is going to start from a first Quad and progress neighbor by neighbor,  each time with a check
// +	about XQuad Grid coordinates.
// +	For more information have a look on this article: "D:\_PROJET_C\Divers Doc\Triangles-to-Quads.pdf"
// +
// +
// +-+ Keep in mind Index of the first inserted XRPoly ( because  "pout_xrpolyarray" may already contains previously made XRPolyes )
	first_inserted_xrpolyindex = pout_xrpolyarray->XRPolyArray.Size;

	while( remaining_xarea_xquad )
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + Define the Current XRPOLY to fill in
		// +
		// +-+	Create a new patch or use a previous one ?
		// +
		if( currentopenxrpolyindex > firstopenxrpolyindex )
		{
			// Use a previous XRPoly
			currentopenxrpolyindex --;
			pcurrentopenxrpoly = *(NUT_XRPOLY**)NGetArrayPtr((NARRAY*)pout_xrpolyarray,currentopenxrpolyindex);
			_boot_xquadnode_queue_with_populated_xrpoly(&xquadnode_queue,pcurrentopenxrpoly);
		}
		else
		{
			// Create a new XRPoly ...
			currentopenxrpolyindex = firstopenxrpolyindex = pout_xrpolyarray->XRPolyArray.Size;
			pcurrentopenxrpoly = (NUT_XRPOLY*)NArrayPushBack( (NARRAY*)pout_xrpolyarray,(NBYTE*)NUT_SetupXRPoly(&xrpoly,(remaining_xarea_xquad/2)+1) );
			
			
			// ... And boot the Queue with the First XArea Available XQuad...
			pxquad = _getfirstavailable_xarea_xquad(pxarea);
			NErrorIf(!pxquad,NERROR_SYSTEM_GURU_MEDITATION);
			_boot_xquadnode_queue_with_seed( &xquadnode_queue, _setup_xqseed(&xqnode,pxquad) );
		}

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +
		// + BUILD XRPOLY RECURSIVE PROCESS With EXPLICIT QUEUE
		// +
		// + Principle is to "ENLARGE" (or diffuse) the Current Patch
		// + as much as possible trough 4 neighbors of each XQuad.
		// + Its obviously a recursive process.
		// + But to avoid overloading processor pile, N is going to use an explicit
		// + Queuing system.
		// +
		// +-+	Fill Up the Current XRPoly until Queue becomes empty.
		// +
		while( xquadnode_queue.Size )
		{
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// +
			// + GET THE FIRST XQUADNODE OF THE QUEUE AND TRY INSERT
			// + XQUAD INTO THE CURRENT OPEN PATCH 
			// +
			// +-+ Get a pointer of the first element of the Queue
			// +
			pxqnode = (NUT_XQUADNODE*)xquadnode_queue.pFirst;
			// +
			// +-+ XQuad already inserted in RPoly ...
			if(NUT_XQUAD_XBUILDINFOS_IS_GRIDCOORDS_COMPUTED(pxqnode->pXTri->pXQuad->XBuildInfos) )
			{
				if( (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) || (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) )
				{
					// ! GRID COORDS ERROR !
					// Incoming grid coordinates don't match with the ones that are already calculated and store into this XQuad...
					// This is the result of a non uniform XQuads repartition.
					// To deal with this we have to split current patch in 2 patches.
					// 
					// Split setup
					ppreviousxrpoly			= pcurrentopenxrpoly;
					currentopenxrpolyindex	= pout_xrpolyarray->XRPolyArray.Size;
					pcurrentopenxrpoly		= (NUT_XRPOLY*)NArrayPushBack( (NARRAY*)pout_xrpolyarray,(NBYTE*)NUT_SetupXRPoly(&xrpoly,(ppreviousxrpoly->XQuadPtrArray.Size/2)+1) );
					NUT_ReAddressing_XRPoly_Array(pout_xrpolyarray);
					pxquad					= pxqnode->pXTri->pXQuad;
					forbidden_direction_mask= _get_direction_mask_from_xquadnode_error(pxqnode);
					// Make the queue empty, '_split_xrpoly_and_boot_xquadnode_queue' is going to try to boot it again after splitting.
					xquadnode_queue.Size = 0;
					_split_xrpoly_and_boot_xquadnode_queue(&xquadnode_queue,pcurrentopenxrpoly,ppreviousxrpoly,pxquad,forbidden_direction_mask);
					// In case of successful RE-boot (xquadnode_queue filled up with new XQuadNodes by '_split_xrpoly_and_boot_xquadnode_queue'),
					// main patch building loop is going to continue, otherwise is going to stop.
					// (which means SPLIT works but without any possibilities to continue to extend the extracted XRPoly.)
					//  In that last case, PROCESS continues with an another patch. 
					if(xquadnode_queue.Size)
					{
						pxqnode = (NUT_XQUADNODE*)xquadnode_queue.pFirst;
					}
					else
					{
						// It may happen, close to the end of process ... just quit the queue...
						break;
					}
				}
				// else
				// {
				//		No Error: just continue... Nothing more to do
				// }
			}
			else
			{
				// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// + INSERT NEW XQUAD INTO THE CURRENT OPEN PATCH 
				// +
				// +-+ XQuad is not already inserted into the Patch, so, insert it !
				// +
				_push_xquad_to_xrpoly(pcurrentopenxrpoly,pxqnode);
				remaining_xarea_xquad --;
			}

			// ++++++++++++++++++++++++++++++++++++++++++++++++
			// + Propagate XQuad to its 4 neighbors
			// +
			// +-+	Propagation will start from XQuad Edge 0 and progress to the 3 others, one after one :0,1,2 and 3. 
			//		Notice that a new xqnode is created an push into the Queue only if the XQuad edge to pass trough is OPEN ...
			//		... That means only if this edge is not flagged as a CLOSE Edge.
			//		Why ? : Because this flag says that the current XQUAD was already evaluated ( or asked for evaluation ) 
			//				from the Neighbor XQuad in that direction (trough this edge) so, it will be useless to do the backward evaluation
			//				by asking evaluation from current XQuad to this neighbor.
			//
			xquadedge	= pxqnode->XQuadEdge;
			xtriedge	= pxqnode->XTriEdge;
			pxtri		= pxqnode->pXTri;

			for(i=4;i!=0;i--)
			{
				// If Edge of the Current XQUADNODE is not CLOSED then Propagate!
				// That means try to create a NEW XQUADNODE.
				if(!NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(pxtri->pXQuad->XBuildInfos,xquadedge))
				{
					// 1a) First, get the adjacent triangle trough this OPEN EDGE.
					xqnode.pXTri	= pxtri->pAdjXTri[xtriedge];
					// 1b) Check if this adjacent triangle exists and if it is concerned by the current process.
					//	   ( be aware that's absolutely possible having an adjacent triangle but OUT of the process. For example:
					//		 This triangle belongs to a quad which is already inserted into an another XAREA, so not concerned ...)
					if( xqnode.pXTri && ISFLAG_ON(xqnode.pXTri->Flags,FLAG_NUT_XTRI_PATCH_PROCESS_LOCK) )
					{
						// 2) At the end, check if the XQUAD of the XTriangle is not already inserted into an another RPOLY of this process.
						//	  Only the Ones which belong to the current XRPOLY or the ones which are free are available
						if(xqnode.pXTri->pXQuad->pXRPoly == pcurrentopenxrpoly || xqnode.pXTri->pXQuad->pXRPoly == NULL)
						{
							xqnode.XTriEdge			= pxtri->AdjEdge[xtriedge];
							xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
							xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
							xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
							
							// Close the Edge of the NEW XQUADNODE to avoid going back ! (smart !!!)
							NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);
							NArrayPushBack(&xquadnode_queue,(NBYTE*)&xqnode);
						}
						else
						{
							NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
						}
					}
					else
					{
						NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
					}
				}

				// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
				xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
				if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
				{
					pxtri	= pxtri->pAssociatedXtri;
					xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
				}
				else
				{
					xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
				}
			}
			// ++++++++++++++++++++++++++++++++++++++++++++++++
			// + Remove pxquadNode from the Queue ( it is the first one of the list... )
			// + Because of the possible memory reallocation we prefer using it's index (0) instead of 'pxqnode' pointer !
			// +
			NEraseArrayElement( &xquadnode_queue,0,NULL );

		}// while( xquadnode_queue.Size )

	}// while( remaining_xarea_xquad )

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + XRPOLYS MERGING
// +
// +
// +	XRPOLY Building process may over slice the source XArea to achieve its goal.
// +	So after the building process, N will try to merge XRPolyes together when its possible.
// +				
// +
// +-+ XRPolyes sorting: 
// +		First position... the xrpoly with the larger number of XQuads Inside.
	NQuickSortArrayEx(&pout_xrpolyarray->XRPolyArray,first_inserted_xrpolyindex, pout_xrpolyarray->XRPolyArray.Size-1, _xrpoly_qsort_compare );
// +-+ Because of XRPoly rearrangement, a re addressing may be necessary 
	NUT_ReAddressing_XRPoly_Array(pout_xrpolyarray);

// +
// +-+ Try to pack XRPolys with all the others 
// +
	pxrpolya =  (NUT_XRPOLY*)NGetArrayPtr(&pout_xrpolyarray->XRPolyArray,first_inserted_xrpolyindex);
	for(i=first_inserted_xrpolyindex;i<pout_xrpolyarray->XRPolyArray.Size-1;i++,pxrpolya++) //'< pout_xrpolyarray->XRPolyArray.Size-1' because we don't want to have pxrpolya = last xrpoly of the array
	{
		NUT_ResetXRPolyXQuadXBuildData(pxrpolya);
		pxrpolyb = pxrpolya+1;
		for(j=i+1;j<pout_xrpolyarray->XRPolyArray.Size;j++,pxrpolyb++) //'< pout_xrpolyarray->XRPolyArray.Size' because we want to have pxrpolyb = last xrpoly of the array
		{
			if( _getxrpoly_commonedges_count(pxrpolya,pxrpolyb,&pxquad) )
			{
				// Setup a new XRPoly ( a temporary one ). It will be built as the mix of pxrpolya and pxrpolyb and
				// in case of successful build will replace XRPolya.
				NUT_SetupXRPoly(&xrpoly,pxrpolya->XQuadPtrArray.Size+pxrpolyb->XQuadPtrArray.Size);
				// Boot xquadnode_queue with pxquad
				_boot_xquadnode_queue_with_seed( &xquadnode_queue, _setup_xqseed(&xqnode,pxquad) );
				merging_success = 1;
				NUT_ResetXRPolyXQuadXBuildData(pxrpolyb);

				while( xquadnode_queue.Size )
				{
					// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// +
					// + GET THE FIRST XQUADNODE OF THE QUEUE AND TRY INSERT
					// + XQUAD INTO THE CURRENT OPEN PATCH 
					// +
					// +-+ Get a pointer of the first element of the Queue
					// +
					pxqnode = (NUT_XQUADNODE*)xquadnode_queue.pFirst;
					// +
					// +-+ XQuad already inserted in Patch ...
					if( NUT_XQUAD_XBUILDINFOS_IS_GRIDCOORDS_COMPUTED(pxqnode->pXTri->pXQuad->XBuildInfos) )
					{
						if( (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) || (pxqnode->XGridCoords.x != pxqnode->pXTri->pXQuad->XBuildGridCoords.x) )
						{
							// ! GRID COORDS ERROR !
							// Incoming grid coordinates don't match with the ones that are already calculated and store into this XQuad...
							// This is the result of a non uniform XQuads repartition.
							// In that stage it just means the 2 XRPolyes are non compatible so... just stop 
							merging_success = 0;
							xquadnode_queue.Size = 0;
							break;
						}
						// else
						// {
						//		No Error: just continue... Nothing more to do
						// }
					}
					else
					{
						// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						// + INSERT NEW XQUAD INTO THE TEMPORARY PATCH 
						// +
						// +-+	XQuad is not already inserted into the Patch, so, insert it !
						// +	But, it's a temporary action maybe useless so keep the original 
						// +	"pxqnode->pXTri->pXQuad->pXRPoly" member for a while ...
						pxrpoly = pxqnode->pXTri->pXQuad->pXRPoly;
						_push_xquad_to_xrpoly(&xrpoly,pxqnode);
						pxqnode->pXTri->pXQuad->pXRPoly = pxrpoly;
					}

					// ++++++++++++++++++++++++++++++++++++++++++++++++
					// + Propagate XQuad to its 4 neighbors
					// +
					// +-+	Propagation will start from XQuad Edge 0 and progress to the 3 others, one after one :0,1,2 and 3. 
					//		Notice that a new xqnode is created an push into the Queue only if the XQuad edge to pass trough is OPEN ...
					//		... That means only if this edge is not flagged as a CLOSE Edge.
					//		Why ? : Because this flag says that the current XQUAD was already evaluated ( or asked for evaluation ) 
					//				from the Neighbor XQuad in that direction (trough this edge) so, it will be useless to do the backward evaluation
					//				by asking evaluation from current XQuad to this neighbor.
					//
					xquadedge	= pxqnode->XQuadEdge;
					xtriedge	= pxqnode->XTriEdge;
					pxtri		= pxqnode->pXTri;

					for(k=4;k!=0;k--)
					{
						if(!NUT_XQUAD_XBUILDINFOS_IS_EDGE_CLOSED(pxtri->pXQuad->XBuildInfos,xquadedge))
						{
							xqnode.pXTri	= pxtri->pAdjXTri[xtriedge];
							
							if(!xqnode.pXTri)
							{
								NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
							}
							else
							{
								if( ISFLAG_ON(xqnode.pXTri->Flags,FLAG_NUT_XTRI_PATCH_PROCESS_LOCK) && (xqnode.pXTri->pXQuad->pXRPoly == pxrpolya || xqnode.pXTri->pXQuad->pXRPoly == pxrpolyb) )
								{
									xqnode.XTriEdge			= pxtri->AdjEdge[xtriedge];
									xqnode.XQuadEdge		= NGET_OPPOSITE_XQUAD_EDGE(xquadedge);
									xqnode.XGridCoords.x	= pxqnode->XGridCoords.x + parentrelativegridcoords[xquadedge].x;
									xqnode.XGridCoords.y	= pxqnode->XGridCoords.y + parentrelativegridcoords[xquadedge].y;
									NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(xqnode.pXTri->pXQuad->XBuildInfos,xqnode.XQuadEdge);
									NArrayPushBack(&xquadnode_queue,(NBYTE*)&xqnode);
								}
								else // if( ISFLAG_OFF(xqnode.pXTri->Flags,FLAG_NUT_XTRI_PATCH_PROCESS_LOCK) || (pxquad->pXRPoly != pcurrentopenxrpoly && pxquad->pXRPoly != NULL) )
								{
									NUT_XQUAD_XBUILDINFOS_CLOSE_EDGE(pxtri->pXQuad->XBuildInfos,xquadedge);
								}
							}
						}

						// NEXT EDGE of THE XQUAD (and of the associated XTRIs)
						xquadedge = NGET_NEXT_XQUAD_EDGE(xquadedge);
						if( NGET_NEXT_XTRI_EDGE( xtriedge ) == pxtri->QuadDiagonal )
						{
							pxtri	= pxtri->pAssociatedXtri;
							xtriedge = NGET_NEXT_XTRI_EDGE( pxtri->QuadDiagonal );
						}
						else
						{
							xtriedge = NGET_NEXT_XTRI_EDGE( xtriedge );
						}
					}
					// ++++++++++++++++++++++++++++++++++++++++++++++++
					// + Remove pxqnode from the Queue ( it is the first one of the list... )
					// + Because of the possible memory reallocation we prefer using it's index (0) instead of 'pxqnode' pointer !
					// +
					NEraseArrayElement( &xquadnode_queue,0,NULL );

				}// while( xquadnode_queue.Size )
				
				// At the End, check merging final result
				if(merging_success == 1)
				{
					// XRPolya:
					//		content is going to be cleared
					//		XRPoly content is going to be 'transfered' to XRPolya
					NClearArray(&pxrpolya->XQuadPtrArray,NULL);
					memcpy(&pxrpolya->XQuadPtrArray,&xrpoly.XQuadPtrArray,sizeof(NARRAY));
					NClearArray(&pxrpolyb->XQuadPtrArray,NULL);
					NEraseArrayElementPtr(&pout_xrpolyarray->XRPolyArray,(NBYTE*)pxrpolyb,NULL);
					pxrpolyb--;
					// We have to readdress all the XQUAD.pXRPoly member of 'pxrpolya' (some of them come from 'pxrpolyb' and still having 'pxrpolyb' has '.pXRPoly' member. )
					// So, to enforce re-addressing of this XRPOLY we have to make the pXRPoly member of its first XQUAD different than 'pxrpolya'...
					(*(NUT_XQUAD**)pxrpolya->XQuadPtrArray.pFirst)->pXRPoly = NULL; 
					NUT_ReAddressing_XRPoly_Array(pout_xrpolyarray);
				}
				else
				{
					// xrpoly content remains unused, so clear it:
					NClearArray(&xrpoly.XQuadPtrArray,NULL);
				}
			}//if( _getxrpoly_commonedges_count(pxrpolya,pxrpolyb) )
		}
	}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + BUFFERS CLEAR
// +
// +
// +-+	Clear all the temporary buffers:
// +			xquadnode_queue
// +				
	NClearArray(&xquadnode_queue,NULL);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + XQUAD and XTRI check and unlock
// +
// + Process is over so, remove all the flags set to ON at the start of this process.
// +
// +-+ Flag XQUAD and XTRI  ..._PATCH_PROCESS_LOCK set to OFF
// +
	// Unlocked XQUAD and XTRI
	ppxquad = (NUT_XQUAD**)pxarea->XQuadPtrArray.pFirst;
	for(i=pxarea->XQuadPtrArray.Size;i!=0;i--,ppxquad++)
	{
		pxquad = *ppxquad;

		NErrorIf( ISFLAG_OFF(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK),NERROR_XQUAD_UNLOCKED );
		NErrorIf( ISFLAG_OFF(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK),NERROR_XTRI_UNLOCKED );
		NErrorIf( ISFLAG_OFF(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK),NERROR_XTRI_UNLOCKED );

		FLAG_OFF(pxquad->Flags, FLAG_NUT_XQUAD_RPOLY_PROCESS_LOCK);
		FLAG_OFF(pxquad->pXTri[0]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);
		FLAG_OFF(pxquad->pXTri[1]->Flags, FLAG_NUT_XTRI_PATCH_PROCESS_LOCK);
	}
}
// ------------------------------------------------------------------------------------------
// void NUT_BuildXRPolyArrayFromXAreaArray
// ------------------------------------------------------------------------------------------
// Description :
// Description :
//	Build Rectilinear Polygons of XQuads from several NUT_XAREA (XQuads AREAs) stored into a NUT_XAREA_ARRAY.
//	Notice that during this process there is NO XQUAD CREATION. But just storage of a pointer 
//	on already build XQuad ( already build during the XAreas building process ). 
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_XRPOLY_ARRAY				*pout_xrpolyarray	The array where all the built XRPolys are going to be store
//			const NUT_XARNUT_XAREA_ARRAY	*pxarea_array		The array of Areas of XQuads. Previously built with 'NUT_BuildXAreaArray'
//			const NUT_XQUAD_ARRAY			*pxquadarray		The Array of XQuads linked and built with the XArea ( 'NUT_BuildXAreaArray' build XAreas which are arrays of XQuads Pointers, AND XQuads Array which is an array of XQuads)
//			const NUT_XTRI_ARRAY			*pxtriarray 		The Array of XTris built with 'NUT_BuildXTriArray' and also used to build XQuads.
// 
// ------------------------------------------------------------------------------------------
void NUT_BuildXRPolyArrayFromXAreaArray( NUT_XRPOLY_ARRAY *pout_xrpolyarray, const NUT_XAREA_ARRAY *pxarea_array, const NUT_XQUAD_ARRAY *pxquadarray, const NUT_XTRI_ARRAY *pxtriarray )
{
	NUT_XAREA	*pxarea = (NUT_XAREA*)pxarea_array->XAreaArray.pFirst;
	for(Nu32 i=pxarea_array->XAreaArray.Size;i!=0;i--,pxarea++)
	{
		NUT_BuildXRPolyArrayFromXArea( pout_xrpolyarray, pxarea, pxquadarray, pxtriarray );
	}
}
