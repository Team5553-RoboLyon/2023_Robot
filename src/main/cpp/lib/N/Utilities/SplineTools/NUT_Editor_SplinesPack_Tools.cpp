#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "lib/N/NCStandard.h"
#include "lib/N/UI/NUI.h"
#include "../../NCamera.h"
#include "lib/N/Maths/NVec3f32.h"
#include "../../NCoordinateSystemConversions.h"
#include "lib/N/NEditor.h"
#include "../../Utilities/NUT_X.h"
#include "../../Utilities/NUT_3DMouse.h"
#include "../../Utilities/Maths/NUT_MathsMisc.h"
#include "../../Utilities/NUT_Spline.h"

#include "../../Event/NEventTouch.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_Editor_SplinesPack_Tools.h"

static inline void _lookfor_spline_element(const NARRAY *parrayofsplineptr, const NVEC2s16 *ppointer, NEDITOR_SELECTION *pselect,NVEC3 *phitpoint)
{
	NSPLINE					**ppspline;
	NSPLINE					*pspline;
	NSPLINEKNOT				*psk;
	Nu32					s,i,keep_index;
	
	NVEC3				mouselineA,mouselineB;
	NUT_3DLINEXLINE_RESULT	result,keep_result;
	Nf32					squared_dist,min_squared_dist;
	Nu16					typepoint;
	NVEC3				*keep_ptangent;
	NVEC3				v;
	Nf32					pick_ratio;
	NVEC3				AB;
// 	Nchar					debug_txt[16];

	// Full 3D research.
	// At this stage we just have a ray ( mouse 3D ray ) and spline elements to look for...
	// So we are are to determine the closest element to the mouse 3D ray and if this element is close enough
	// to the ray it will be the One !
	// 'Look for' order is important, all tangent elements first AND ONLY AFTER, Knot positions.
	// Lets go !
	min_squared_dist = NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE_SQUARED;
	NResetToolSelection(pselect);
	NUT_Get3DMouseSegment(&mouselineA, &mouselineB, NULL, ppointer );
	ppspline = (NSPLINE**)NGetFirstArrayPtr(parrayofsplineptr);
	for(s=0;s<NGetArraySize(parrayofsplineptr);s++,ppspline++)
	{
		pspline = *ppspline;
		// Test all the spline points
		psk = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
		for(i=0;i<NGetArraySize(&pspline->KnotArray);i++,psk++)
		{
			// looking only for psk position if Tangents are not editable !
			if( ISFLAG_ON(pspline->Flags,FLAG_SPLINE_EDITOR_UNEDITABLE_TANGENT) )
			{
				NVec3Sub(&AB,&mouselineB,&mouselineA);
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB,&mouselineA,&AB,&psk->Position);
				NVec3Sub(&v,&psk->Position,&result.PointOnAB);
				squared_dist = NVec3SquareLength(&v);
				if(squared_dist<min_squared_dist)
				{
					min_squared_dist		= squared_dist;
					pselect->pSelectedOwner	= (void*)pspline;
					pselect->pSelected		= (void*)psk;
					keep_index				= i;
					keep_result.ParamCoordCD= 0.0f;// to force function (at the end of...) to define selected psk as SPLINESPACK_SPLINEKNOT_POSITION
					keep_result.PointOnCD	= psk->Position;	// Because we have been used NUT_GetClosestPointOnLine the Second pass of this algo
					keep_ptangent			= NULL;							// is going to look for the hitpoint into  keep_result.PointOnCD
				}
			}
			// Looking for all elements
			else 
			{
				// First of ALL:
				// Get the shortest distance between 3D Mouse Ray and Tangent A segment and analyze it ! 
				NUT_LineXSeg(&result,&mouselineA,&mouselineB,&psk->Position,&psk->TA);
				squared_dist = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&result);
				if(squared_dist<min_squared_dist)
				{
					min_squared_dist		= squared_dist;
					pselect->pSelectedOwner	= (void*)pspline;
					pselect->pSelected		= (void*)psk;
					keep_index				= i;
					keep_result				= result;
					keep_ptangent			= &psk->TA; 
				}

				// Get the shortest distance between 3D Mouse Ray and Tangent B segment and analyze it ! 
				NUT_LineXSeg(&result,&mouselineA,&mouselineB,&psk->Position,&psk->TB);
				squared_dist = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&result);
				if(squared_dist<min_squared_dist)
				{
					min_squared_dist		= squared_dist;
					pselect->pSelectedOwner	= (void*)pspline;
					pselect->pSelected		= (void*)psk;
					keep_index				= i;
					keep_result				= result;
					keep_ptangent			= &psk->TB;
				}
			}
		}
	}
	
	// SECOND PASS
	// Deep Result Analysis
	if(!pselect->pSelected)
		return;
	
	psk = (NSPLINEKNOT *)pselect->pSelected;
	// Fine precision about selected knot
	if(keep_index==0)
		typepoint = _START_KNOT;
	else if( keep_index== NGetArraySize(&((NSPLINE*)pselect->pSelectedOwner)->KnotArray)-1 ) 
		typepoint = _END_KNOT;
	else
		typepoint = _INBETWEEN_KNOT;

	// Precise element selection according to the ParamCoord value
	if(keep_result.ParamCoordCD == 0.0f)		// Knot Position
	{
		pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_POSITION,typepoint);
		//pselect->ZfromCam		= ; we will define that latter
	}
	else if(keep_result.ParamCoordCD == 1.0f)	// Knot Tangent B Extremity Handle 
	{
		if(keep_ptangent == &psk->TA)
		{
			pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TA_ENDHANDLE,typepoint);
// 			printf("\nKnot Tangent A Confirmed");

		}
		else
		{
			pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TB_ENDHANDLE,typepoint);
// 			printf("\nKnot Tangent B Confirmed");

		}
	}
	else	// general case
	{
		psk = (NSPLINEKNOT *)pselect->pSelected;
		NVec3Sub(&v,keep_ptangent,&psk->Position);
		
		pick_ratio = NMIN( 0.125f,NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE/NVec3Length(&v) ); // 0.125f means 1/8 
		//				+-+ -- -- -- +-+
		//				P a          b E		>>> P:position	a:(pick_ratio)	b:(1-pick_ratio)	E:EndHandle
		
		if( keep_result.ParamCoordCD < pick_ratio )
		{
			// POSITION
			pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_POSITION,typepoint);
		}
		else if( keep_result.ParamCoordCD > (1.0f-pick_ratio) )
		{
			// TANGENT HANDLES
			if(keep_ptangent == &psk->TA)
			{
				pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TA_ENDHANDLE,typepoint);
			}
			else
			{
				pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TB_ENDHANDLE,typepoint);
			}
		}
		else
		{
			// TANGENT SEGMENTS
			if(keep_ptangent == &psk->TA)
				pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TA_SEGMENT,typepoint);
			else
				pselect->SelectParam	= NMAKELONG(NUT_SPACK_SPLINEKNOT_TB_SEGMENT,typepoint);
		}
	}

	// LAST PASS
	//Snks_GameData.LineXLine = keep_result;
	*phitpoint = keep_result.PointOnCD;	//NVec3Copy(phitpoint,&keep_result.PointOnCD);
}





// ------------------------------------------------------------------------------------------
// NUT_SplinesPackTool_MoveKnot
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//		
//		NTOUCH_MOVE:	"A SplinePoint is selected and moved (position or tangents)."
//			CallBack is going to be called  only if there is a selected SplinePoint and
//			after moving the SplinePosition or Tangent. 
//			The "NEDITOR_SELECTION"	structure contains the selected spline point informations.
//
//		NTOUCH_END:
//			CallBack is going to be called  ONLY if a real move occurred before.
//			( obviously if there is a selected SplinePoint ... )
//			Be aware that the selected point position is exactly the same than the last 'NTOUCH_MOVE' callback call.
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NUT_SplinesPackTool_MoveKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	static	Nbool				bmoved = NFALSE;
	static	NVEC3			hitpoint ={0,0,0};
	static	Nf32				additional_tangent_portion;
	static	NVEC3			relative_tangent_position;
	
	static	Nu32				touch_id = 0;

	//NEVENT_TOUCH				*ptouch;
	NUT_SPACK_TOOLDATA				*pdata;
	NSPLINE							*pspline;
	NSPLINEKNOT						*psp;
	NVEC3						mouseray_origin,mouseray_dirvect;
	NVEC3						planenormal,mouse_on_workplane;
	NVEC3						v,va,vb;
	/*NVEC3						vx,vy,vz;*/
	NMATRIX3x3						base;
	Nf32							la;

	switch(pevent->Type)
	{
		case NTOUCH_START:
			bmoved = NFALSE;
			//ptouch=(NEVENT_TOUCH*)pevent;
			pdata = (NUT_SPACK_TOOLDATA *)tool_param;
			
			_lookfor_spline_element(&pdata->ArrayOfSplinesPtr,NGetEventTouchPositionPtr(pevent,0),pselect,&hitpoint);

			if( pselect->pSelected )
			{
				psp = (NSPLINEKNOT *)pselect->pSelected;

				switch( NLOWORD(pselect->SelectParam) )
				{
					case NUT_SPACK_SPLINEKNOT_TA_SEGMENT:
						// Additional Tangent Portion 
						//											  			 additional part
						//					x----------------------------------|------------------>
						//			   knot position					   hitpoint
						//
						NVec3Sub(&v,&hitpoint,&psp->Position);
						NVec3Sub(&va,&psp->TA,&psp->Position);
						additional_tangent_portion = NVec3Length(&va) - NVec3Length(&v);

						// TB position relative to TA into an orthonormal basis with TA as forward axis
						NGetEditorWorkPlaneNormalVector(&planenormal);
						//NForwardVectorMatrix3x3Ex(&base,&psp->TA,&psp->Position,&planenormal);
						NUT_Build3DOrthonormal(&base,&psp->TA,&psp->Position,&planenormal);
						NVec3Sub(&v,&psp->TB,&psp->Position);
						relative_tangent_position.x = NVec3DotProduct(&v,&base.Side);
						relative_tangent_position.y = NVec3DotProduct(&v,&base.Up);
						relative_tangent_position.z = NVec3DotProduct(&v,&base.Forward);
						break;

					case NUT_SPACK_SPLINEKNOT_TB_SEGMENT:
						// Additional Tangent Portion 
						//											  			 additional part
						//					x----------------------------------|------------------>
						//			   knot position					   hitpoint
						//
						NVec3Sub(&v,&hitpoint,&psp->Position);
						NVec3Sub(&vb,&psp->TB,&psp->Position);
						additional_tangent_portion = NVec3Length(&vb) - NVec3Length(&v);
						
						// TA position relative to TB into an orthonormal basis with TB as forward axis
						NGetEditorWorkPlaneNormalVector(&planenormal);
						// TODO: We have a problem here with 'NUT_ForwardVectorMatrix3x3Ex'
						// tangent and Normal vector of the Work plane are collinear !
						// strange but it could happen, in 3D with a dynamic Normal Vector ( Face camera process ) ...
						// ... so ... we will to find a solution ... one day !
	//						NForwardVectorMatrix3x3Ex(&base,&psp->TB,&psp->Position,&planenormal);
						NUT_Build3DOrthonormal(&base,&psp->TB,&psp->Position,&planenormal);
						NVec3Sub(&v,&psp->TA,&psp->Position);
						relative_tangent_position.x = NVec3DotProduct(&v,&base.Side);
						relative_tangent_position.y = NVec3DotProduct(&v,&base.Up);
						relative_tangent_position.z = NVec3DotProduct(&v,&base.Forward);
						break;

					default:
						break;
				}
				//pdata->EnableMove_CallBack();
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;
		case NTOUCH_MOVE:
			pdata = (NUT_SPACK_TOOLDATA *)tool_param;
// 			ptouch=(NEVENT_TOUCH*)pevent;
			psp = (NSPLINEKNOT *)pselect->pSelected;
			pspline = (NSPLINE*)pselect->pSelectedOwner;

			//printf("/nx:%d,y:%d",ptouch->Position.x,ptouch->Position.y);
			
			if(psp)
			{
				NGetEditorWorkPlaneNormalVector(&planenormal);
				NUT_Get3DMouseRay(&mouseray_origin,&mouseray_dirvect,NULL,NGetEventTouchPositionPtr(pevent,0));
				
				if (NUT_LineXPLane(&mouseray_origin, &mouseray_dirvect, &hitpoint, &planenormal, &mouse_on_workplane))
				{
					// CALL BACK 'PRE' MOVE
					if (pdata->PreMove_CallBack)
						pdata->PreMove_CallBack((NSPLINE*)pselect->pSelectedOwner, psp, pselect->SelectParam);
					
					switch (NLOWORD(pselect->SelectParam))
					{
					case NUT_SPACK_SPLINEKNOT_POSITION:
						
							//NVec3Sub(&psp->TA,&psp->TA,&psp->Position);
							//NVec3Sub(&psp->TB,&psp->TB,&psp->Position);
							//psp->Position = mouse_on_workplane;
							NVec3Sub(&v, &mouse_on_workplane, &psp->Position);
							NVec3AddTo(&psp->TA, &v);
							NVec3AddTo(&psp->TB, &v);
							NVec3AddTo(&psp->Position, &v);

						NUT_ApplySplinesPackConstraints(pdata, (NSPLINE*)pselect->pSelectedOwner, psp, (NUT_SPACK_SPLINEPOINT_ENUM)NHIWORD(pselect->SelectParam));
						break;

					case NUT_SPACK_SPLINEKNOT_TA_SEGMENT:
						// Move TA 
						NVec3Sub(&v, &mouse_on_workplane, &psp->Position);
						la = NVec3Length(&v);
						NVec3Scale(&v, &v, (la + additional_tangent_portion) / la);
						
						NVec3Add(&psp->TA, &psp->Position, &v);

						// Move TB
						NGetEditorWorkPlaneNormalVector(&planenormal);

						// TODO: We have a problem here with 'NUT_ForwardVectorMatrix3x3Ex'
						// tangent and Normal vector of the Work plane are collinear !
						// strange but it could happen, in 3D with a dynamic Normal Vector ( Face camera process ) ...
						// ... so ... we will to find a solution ... one day !
//							NForwardVectorMatrix3x3Ex(&base,&psp->TA,&psp->Position,&planenormal);
						NUT_Build3DOrthonormal(&base, &psp->TA, &psp->Position, &planenormal);
						NVec3ScaleBy(&base.Side, relative_tangent_position.x);
						NVec3ScaleBy(&base.Up, relative_tangent_position.y);
						NVec3ScaleBy(&base.Forward, relative_tangent_position.z);
						psp->TB.x = psp->Position.x + base.Side.x + base.Up.x + base.Forward.x;
						psp->TB.y = psp->Position.y + base.Side.y + base.Up.y + base.Forward.y;
						psp->TB.z = psp->Position.z + base.Side.z + base.Up.z + base.Forward.z;

						NUT_ApplySplinesPackTangentsSnap(pdata, FLAG_SPACK_TANGENTSNAP_LENGTH_ON, &psp->TA, &psp->TB, &psp->Position);
						break;

					case NUT_SPACK_SPLINEKNOT_TA_ENDHANDLE:
						psp->TA = mouse_on_workplane;
						NUT_ApplySplinesPackTangentsSnap(pdata, FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON | FLAG_SPACK_TANGENTSNAP_LENGTH_ON, &psp->TA, &psp->TB, &psp->Position);
						break;


					case NUT_SPACK_SPLINEKNOT_TB_SEGMENT:
						// Move TB
						NVec3Sub(&v, &mouse_on_workplane, &psp->Position);
						la = NVec3Length(&v);
						NVec3Scale(&v, &v, (la + additional_tangent_portion) / la);
						NVec3Add(&psp->TB, &psp->Position, &v);

						// Move TA
						NGetEditorWorkPlaneNormalVector(&planenormal);
						// TODO: We have a problem here with 'NUT_ForwardVectorMatrix3x3Ex'
						// tangent and Normal vector of the Work plane are collinear !
						// strange but it could happen, in 3D with a dynamic Normal Vector ( Face camera process ) ...
						// ... so ... we will to find a solution ... one day !
						//NForwardVectorMatrix3x3Ex(&base,&psp->TB,&psp->Position,&planenormal);
						NUT_Build3DOrthonormal(&base, &psp->TB, &psp->Position, &planenormal);
						NVec3ScaleBy(&base.Side, relative_tangent_position.x);
						NVec3ScaleBy(&base.Up, relative_tangent_position.y);
						NVec3ScaleBy(&base.Forward, relative_tangent_position.z);
						psp->TA.x = psp->Position.x + base.Side.x + base.Up.x + base.Forward.x;
						psp->TA.y = psp->Position.y + base.Side.y + base.Up.y + base.Forward.y;
						psp->TA.z = psp->Position.z + base.Side.z + base.Up.z + base.Forward.z;

						NUT_ApplySplinesPackTangentsSnap(pdata, FLAG_SPACK_TANGENTSNAP_LENGTH_ON, &psp->TB, &psp->TA, &psp->Position);
						break;

					case NUT_SPACK_SPLINEKNOT_TB_ENDHANDLE:
						psp->TB = mouse_on_workplane;
						NUT_ApplySplinesPackTangentsSnap(pdata, FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON | FLAG_SPACK_TANGENTSNAP_LENGTH_ON, &psp->TB, &psp->TA, &psp->Position);
						break;
					}


					bmoved = NTRUE;
					NUT_ApplySplinesPackSymmetries(pdata, (NSPLINE*)pselect->pSelectedOwner);

					// CALL BACK 'POST' MOVE
					if (pdata->PostMove_CallBack)
						pdata->PostMove_CallBack((NSPLINE*)pselect->pSelectedOwner, psp, pselect->SelectParam);
					return NEDITOR_RESULT_TOOL_ON;
				}
			}
			break;
		
		case NTOUCH_END:
			if( pselect->pSelected && bmoved )
			{
	// 				NCallCurrentToolCallBack(pevent);
				NResetToolSelection(pselect);
				bmoved = NFALSE;
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				NResetToolSelection(pselect);
			}
			break;

		default:
			break;
	}


	return NEDITOR_RESULT_TOOL_OFF;
}

// ------------------------------------------------------------------------------------------
// NUT_SplinesPackTool_AddKnot
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//	
//		NTOUCH_START: 
//			CallBack is going to be called only if there is a new SplinePoint.
//			This call is performed after a successfully spline point insertion,
//			so the "NEDITOR_SELECTION" structure will be fill by the created spline point informations.
//		
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NUT_SplinesPackTool_AddKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
// 	NEVENT_TOUCH				*ptouch;
	NSPLINEKNOT					newsplinepoint;
	NSPLINEKNOT					*psp;

	Nu32						s;
	NUT_SPACK_TOOLDATA			*pdata;
	NSPLINE						*pspline;
	NSPLINE						**ppspline;

	NVEC3					L1,L2,L3,L4,L5,R;	// for by hand 'Casteljau' Get Point 
	NVEC3					mouselineA,mouselineB;
	NUT_3DLINEXSPLINE_RESULT	xresult;

	switch(pevent->Type)
	{
		case NTOUCH_START:
// 			ptouch=(NEVENT_TOUCH*)pevent;
			pdata = (NUT_SPACK_TOOLDATA *)tool_param;
			ppspline = (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);

			NUT_Get3DMouseSegment(&mouselineA, &mouselineB,NULL, NGetEventTouchPositionPtr(pevent,0) );
			//NUT_Get3DMouseRay(NULL,&ptouch->Position,&linep,&linedir);
			for(s=0;s<NGetArraySize(&pdata->ArrayOfSplinesPtr);s++)
			{
				pspline = *ppspline;
				
			//	NUT_GetClosestPointOnSpline( &v3mouseonplane, pspline,&closestpointresult );
				
				NUT_lineXSpline(  &xresult,&mouselineA, &mouselineB, pspline );

				if( xresult.SquareDistance <= NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE_SQUARED 
					&& xresult.LerpFactorEstimatedResult != 0.0f		// To avoid creating a new SplineKnot onto an existing one
					&& xresult.LerpFactorEstimatedResult != 1.0f )		// To avoid creating a new SplineKnot onto an existing one
				{
					// Re-compute reel "on spline" knot with estimated factor
					// Calculation 'by hand' because we need L1 and L3 result to modify previous and next SplineKnot tangents and keep 
					// ... the same curve !
					NVec3Lerp(&L1,	&xresult.pSplineKnotA->Position,&xresult.pSplineKnotA->TB,			xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L2,	&xresult.pSplineKnotA->TB,		&xresult.pSplineKnotB->TA,			xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L3,	&xresult.pSplineKnotB->TA,		&xresult.pSplineKnotB->Position,	xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L4,	&L1,&L2,xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L5,	&L2,&L3,xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&R,	&L4,&L5,xresult.LerpFactorEstimatedResult);

					// CALL BACK 'PRE' ADD
					if(pdata->PreAdd_CallBack)
						pdata->PreAdd_CallBack(pspline,xresult.pSplineKnotA,xresult.pSplineKnotB,xresult.LerpFactorEstimatedResult);

					// ... modify previous and next SplineKnot Tangents before inserting
					xresult.pSplineKnotA->TB = L1;
					xresult.pSplineKnotB->TA = L3;

					// ... And Insert.
					memset(&newsplinepoint,0,sizeof(NSPLINEKNOT));
					newsplinepoint.Flags=0;
					newsplinepoint.Position	= R;
					newsplinepoint.TA		= L4;
					newsplinepoint.TB		= L5;

				
					psp = NInsertSplineKnot(pspline, xresult.SplineKnotBIndex, &newsplinepoint);

					// Setup pselect ... but ... maybe it's useless ? (we do that because user can access to current selection inside post callback ...)
					pselect->pSelected = psp;
					pselect->pSelectedOwner = pspline;
					pselect->SelectParam = xresult.SplineKnotBIndex;// which is now the inserted node index.
					pselect->ZfromCam = 0.0f; // Unused ?

					// Apply constraint to the New Knot. It's a  _INBETWEEN_KNOT obviously !! Because it was created between two pre existing Knots.
					NUT_ApplySplinesPackConstraints(pdata,pspline,psp,_INBETWEEN_KNOT);
					NUT_ApplySplinesPackSymmetries(pdata,(NSPLINE*)pselect->pSelectedOwner);

					// CALL BACK 'POST' ADD
					if(pdata->PostAdd_CallBack)
					{
						pdata->PostAdd_CallBack(pspline,psp-1,psp+1,xresult.LerpFactorEstimatedResult,psp);
					}
					


					NResetToolSelection(pselect);
					return NEDITOR_RESULT_TOOL_ON;
				}

				ppspline++;
			}
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}


// ------------------------------------------------------------------------------------------
// SplinesPackTool_Add_ColorExtended
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Specific Tool is a copy of "SplinesPackTool_Add". In addition to Add Spline Knot it manage 
//		Knot values UserA and UserB as RGBA colors and calculate them for each new added knot.
//		TODO: in some future version it should be replaced by a smart use of a "SelectOwnerCopy" done before each modofication.
//		and usable with ToolCallback function.
//
//		As usual it uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//	
//		NTOUCH_START: 
//			CallBack is going to be called only if there is a new SplinePoint.
//			This call is performed after a successfully spline point insertion,
//			so the "NEDITOR_SELECTION" structure will be fill by the created spline point informations.
//		
// ------------------------------------------------------------------------------------------
/*

NEDITOR_RESULT SplinesPackTool_Add_ColorExtended(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	NEVENT_TOUCH				*ptouch;
	NSPLINEKNOT					newsplinepoint;
	Nu32						s;
	SNKS_SPLINESPACK_TOOLDATA	*pdata;
	NSPLINE						*pspline;
	NSPLINE						**ppspline;

	NVEC3					L1,L2,L3,L4,L5,R;	// for by hand 'NVec3QLerp' 
	NVEC3					mouselineA,mouselineB;
	NUT_3DLINEXSPLINE_RESULT	xresult;

		switch(pevent->Type)
		{
		case NTOUCH_START:
			ptouch=(NEVENT_TOUCH*)pevent;
			pdata = (SNKS_SPLINESPACK_TOOLDATA *)tool_param;
			ppspline = (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);

			NUT_Get3DMouseSegment(&mouselineA, &mouselineB,NULL, &ptouch->Position );
			//NUT_Get3DMouseRay(NULL,&ptouch->Position,&linep,&linedir);
			for(s=0;s<NGetArraySize(&pdata->ArrayOfSplinesPtr);s++)
			{
				pspline = *ppspline;

				//	NUT_GetClosestPointOnSpline( &v3mouseonplane, pspline,&closestpointresult );

				NUT_lineXSpline(  &xresult,&mouselineA, &mouselineB, pspline );

				if( xresult.Distance <= SPPACK_SPLINEKNOT_DETECTION_DISTANCE 
					&& xresult.LerpFactorEstimatedResult != 0.0f		// To avoid creating a new SplineKnot onto an existing one
					&& xresult.LerpFactorEstimatedResult != 1.0f )		// To avoid creating a new SplineKnot onto an existing one
				{
					// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// + "...ColorExtended" add on
					// + PART I
					NCOLOR cola,colb,col0,col1;
					RGBA8_To_NCOLOR(&col0, xresult.pSplineKnotA->A_User_32);
					RGBA8_To_NCOLOR(&col1, xresult.pSplineKnotB->A_User_32);
					NColorLerp(&cola,&col0,&col1,xresult.LerpFactorEstimatedResult);
					RGBA8_To_NCOLOR(&col0, xresult.pSplineKnotA->B_User_32);
					RGBA8_To_NCOLOR(&col1, xresult.pSplineKnotB->B_User_32);
					NColorLerp(&colb,&col0,&col1,xresult.LerpFactorEstimatedResult);
					// +
					// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

					// Re-compute reel "on spline" knot with estimated factor
					// Calculation 'by hand' because we need L1 and L3 result to modify previous and next SplineKnot tangents and keep 
					// ... the same curve !
					NVec3Lerp(&L1,	&xresult.pSplineKnotA->Position,&xresult.pSplineKnotA->TB,			xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L2,	&xresult.pSplineKnotA->TB,		&xresult.pSplineKnotB->TA,			xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L3,	&xresult.pSplineKnotB->TA,		&xresult.pSplineKnotB->Position,	xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L4,	&L1,&L2,xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&L5,	&L2,&L3,xresult.LerpFactorEstimatedResult);
					NVec3Lerp(&R,	&L4,&L5,xresult.LerpFactorEstimatedResult);

					// ... modify previous and next SplineKnot Tangents before inserting
					NVec3Copy(&xresult.pSplineKnotA->TB, &L1);
					NVec3Copy(&xresult.pSplineKnotB->TA, &L3);

					// ... And Insert.
					memset(&newsplinepoint,0,sizeof(NSPLINEKNOT));
					newsplinepoint.Flags=0;
					NVec3Copy(&newsplinepoint.Position,&R);
					NVec3Copy(&newsplinepoint.TA,&L4);
					NVec3Copy(&newsplinepoint.TB,&L5);
					pselect->pSelected		= NInsertSplineKnot(pspline,xresult.SplineKnotBIndex,&newsplinepoint);
					pselect->pSelectedOwner = pspline;
					pselect->SelectParam	= xresult.SplineKnotBIndex;// which is now the inserted node index.
					pselect->ZfromCam		= 0.0f; // Unused ?

					// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// + "...ColorExtended" add on
					// + PART II
					NCOLOR_To_RGBA8( &((NSPLINEKNOT*)pselect->pSelected)->A_User_32, &cola );
					NCOLOR_To_RGBA8( &((NSPLINEKNOT*)pselect->pSelected)->B_User_32, &colb );
					// +
					// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

					// ... and call the callback
// 					NCallCurrentToolCallBack(pevent);
					NResetToolSelection(pselect);
					return NEDITOR_RESULT_TOOL_ON;
				}

				ppspline++;
			}
			break;

		default:
			break;
		}
	return NEDITOR_RESULT_TOOL_OFF;
}

*/
/*
Nbool SplinesPackTool_Add(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	NEVENT_TOUCH				*ptouch;
	NSPLINEKNOT				newsplinepoint;
	Nu32						s;
	SNKS_SPLINESPACK_TOOLDATA	*pdata;
	NSPLINE						*pspline;
	NSPLINE						**ppspline;
	NVEC3					v3mouseonplane;
	NUT_CLOSESTPOINT_ON_SPLINE	closestpointresult;

	NVEC3					L1,L2,L3,L4,L5,R;	// for by hand 'NVec3QLerp' 
	NVEC3					linep,linedir;

		switch(pevent->Type)
		{
			case NTOUCH_START:
				ptouch=(NEVENT_TOUCH*)pevent;
				pdata = (SNKS_SPLINESPACK_TOOLDATA *)tool_param;
				ppspline = (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);

				NUT_Get3DMouseRay(NULL,&ptouch->Position,&linep,&linedir);
				if( NUT_LineXPLane(&linep,&linedir,&pdata->CreationPlane_Point,&pdata->CreationPlane_VDir,&v3mouseonplane) )
				{
					for(s=0;s<NGetArraySize(&pdata->ArrayOfSplinesPtr);s++)
					{
						pspline = *ppspline;
						NUT_GetClosestPointOnSpline( &v3mouseonplane, pspline,&closestpointresult );
						if( closestpointresult.Distance <= SPPACK_SPLINEKNOT_DETECTION_DISTANCE 
							&& closestpointresult.LerpFactorEstimatedResult != 0.0f		// To avoid creating a new SplineKnot onto an existing one
							&& closestpointresult.LerpFactorEstimatedResult != 1.0f )	// To avoid creating a new SplineKnot onto an existing one
						{
							// Re-compute reel "on spline" knot with estimated factor
							//NVec3QLerp(closestpointresult.FactorEstimatedResult,    &A->Position,  &A->TB,   &B->TA,  &B->Position,  R);
							NVec3Lerp(&L1,	&closestpointresult.pSplineKnotA->Position,	&closestpointresult.pSplineKnotA->TB,		closestpointresult.LerpFactorEstimatedResult);
							NVec3Lerp(&L2,	&closestpointresult.pSplineKnotA->TB,		&closestpointresult.pSplineKnotB->TA,		closestpointresult.LerpFactorEstimatedResult);
							NVec3Lerp(&L3,	&closestpointresult.pSplineKnotB->TA,		&closestpointresult.pSplineKnotB->Position,	closestpointresult.LerpFactorEstimatedResult);
							NVec3Lerp(&L4,	&L1,&L2,closestpointresult.LerpFactorEstimatedResult);
							NVec3Lerp(&L5,	&L2,&L3,closestpointresult.LerpFactorEstimatedResult);
							NVec3Lerp(&R,	&L4,&L5,closestpointresult.LerpFactorEstimatedResult);
							
							// ... modify previous and next SplineKnot Tangents before inserting
							NVec3Copy(&closestpointresult.pSplineKnotA->TB, &L1);
							NVec3Copy(&closestpointresult.pSplineKnotB->TA, &L3);

							// ... And Insert.
							memset(&newsplinepoint,0,sizeof(NSPLINEKNOT));
							newsplinepoint.Flags=0;
							NVec3Copy(&newsplinepoint.Position,&R);
							NVec3Copy(&newsplinepoint.TA,&L4);
							NVec3Copy(&newsplinepoint.TB,&L5);
							NInsertSplineKnot(pspline,closestpointresult.SplineKnotBIndex,&newsplinepoint);
							break;
						}

						ppspline++;
					}
				}
				break;

			default:
				break;
		}
	return NFALSE;
}*/

// ------------------------------------------------------------------------------------------
// NUT_SplinesPackTool_SuprKnot
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//	
//		NTOUCH_END:	"Selected SplinePoint was deleted. Remaining accessible selection informations are obsolete."
//			CallBack is going to be called  ONLY if a SplinePoint is really deleted, and it will be call
//			AFTER the deleting process, BUT before reseting "NEDITOR_SELECTION" data. So, deleted SplinePoint
//			informations remain accessible even if they are obsolete.
//			Take care ! the deleted spline point is really deleted !!!
//			Don't try to use the "NEDITOR_SELECTION" informations to access ...
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT NUT_SplinesPackTool_SuprKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
// 	NEVENT_TOUCH	*ptouch;
	NVEC3		vdir;

	NVEC3 OA,OP,PA;
	NVEC3 O,A,P;
	Nf32		fdot;
	Nf32		fdist;

	Nu32		i,s;
	NSPLINEKNOT	*psp;

	NUT_SPACK_TOOLDATA			*pdata;
	NSPLINE						*pspline;
	NSPLINE						**ppspline;
	
//	Nbool						berase;
	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
// 			ptouch=(NEVENT_TOUCH*)pevent;
			pdata = (NUT_SPACK_TOOLDATA *)tool_param;

			NUT_Get3DMouseRay(&O,&vdir,NULL,NGetEventTouchPositionPtr(pevent,0) );
			NVec3Normalize(&vdir);
			

			pselect->pSelectedOwner = NULL;
			pselect->pSelected = NULL;
			pselect->SelectParam = NVOID;
			pselect->ZfromCam = 0;

			ppspline = (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);
			for(s=0;s<NGetArraySize(&pdata->ArrayOfSplinesPtr);s++)
			{
				pspline = *ppspline;
				// Test all the spline points
				psp = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
				for(i=0;i<NGetArraySize(&pspline->KnotArray);i++)
				{
					if(ISFLAG_ON(pspline->Flags, FLAG_SPLINE_EDITOR_START_PERMANENT) && i == 0)
						continue;
					if (ISFLAG_ON(pspline->Flags, FLAG_SPLINE_EDITOR_END_PERMANENT) && i == ((NSPLINE*)pselect->pSelectedOwner)->KnotArray.Size - 1)
						continue;
					
					// Position:
					A = psp->Position;
					NVec3Sub(&OA,&A,&O);
					fdot = NVec3DotProduct(&vdir,&OA);
					NVec3Scale(&OP,&vdir,fdot);
					NVec3Add(&P,&OP,&O);
					NVec3Sub(&PA,&A,&P);
					fdist = NVec3FastNormalize(&PA); //fdist = NVec3Normalize(&u); 
					if( fdist <= NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE )
					{
						pselect->pSelectedOwner	= (void*)pspline;
						pselect->pSelected		= (void*)psp;
						pselect->SelectParam	= i;
						pselect->ZfromCam		= fdot;
						return NEDITOR_RESULT_TOOL_ON;
					}

					psp++;
				}
				ppspline++;
			}
			break;

		case NTOUCH_MOVE:
			// The tool didn't do something, except un-select the splinepoint if there was one !
			pselect->pSelectedOwner	= NULL;
			pselect->pSelected		= NULL;
			pselect->SelectParam	= 0;
			pselect->ZfromCam		= 0;
			break;

		case NTOUCH_END:
			if(pselect->pSelected)
			{
				pdata = (NUT_SPACK_TOOLDATA *)tool_param;
				pspline = (NSPLINE*)pselect->pSelectedOwner;

				// Check number of Spline Knots before erasing it.
				if (pspline->KnotArray.Size > 2)
				{

					// CALL BACK 'PRE' SUPR
					if (pdata->PreSupr_CallBack)
						pdata->PreSupr_CallBack(pspline, (NSPLINEKNOT*)pselect->pSelected, pselect->SelectParam);

					NEraseSplineKnot(pspline, pselect->SelectParam);

					// Here constraints are not applied !
					//NUT_ApplySplinesPackConstraints(pdata, (NSPLINE*)pselect->pSelectedOwner, psp, (NUT_SPACK_SPLINEPOINT_ENUM)NHIWORD(pselect->SelectParam));

					// APPLY SYMMETRIES !!!
					NUT_ApplySplinesPackSymmetries(pdata, pspline);

					// CALL BACK 'POST' SUPR
					if (pdata->PostSupr_CallBack)
						pdata->PostSupr_CallBack(pspline);
				}
				// Spline contains 2 or less knots
				else
				{
					// CALL BACK 'PRE' DELETE
					if (pdata->PreDelete_CallBack)
						pdata->PreDelete_CallBack(pspline);


					ppspline = (NSPLINE**)NGetFirstArrayPtr(&pdata->ArrayOfSplinesPtr);
					for (i = 0; i < pdata->ArrayOfSplinesPtr.Size; i++)
					{
						if (pspline == *ppspline)
						{
							NEraseArrayElement(&pdata->ArrayOfSplinesPtr, i, NULL);
							#ifdef _DEBUG
							bdel = 1;
							#endif
							break;
						}
						ppspline++;
					}
					NErrorIf(bdel == 0, NERROR_GENERIC);

					if (pdata->Delete_CallBack)
						pdata->Delete_CallBack(pspline);
					else
						NDeleteSpline(pspline);

					// CALL BACK 'PRE' DELETE
					if (pdata->PostDelete_CallBack)
						pdata->PostDelete_CallBack();

				}
				pselect->pSelectedOwner	= NULL;
				pselect->pSelected		= NULL;
				pselect->SelectParam	= 0;
				pselect->ZfromCam		= 0;
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT NUT_SplinesPackTool_CreateSpline(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool				bmoved = NFALSE;

	// 	NEVENT_TOUCH				*ptouch;
//	NSPLINEKNOT					newsplinepoint;
	NSPLINEKNOT					*psp;

//	Nu32						s;
	NUT_SPACK_TOOLDATA			*pdata;
	NSPLINE						*pspline;
//	NSPLINE						**ppspline;

	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	NVEC3					result;

	NSPLINEKNOTDESC				desc[2];

	switch (pevent->Type)
	{
	case NTOUCH_START:
		pdata = (NUT_SPACK_TOOLDATA *)tool_param;
	
		NUT_Get3DMouseRay(&mouseline_point,&mouseline_dir,NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point,&mouseline_dir,&plane_point,&plane_dir, &result);


		// CALL BACK 'PRE' CREATE
		if (pdata->PreCreate_CallBack)
			pdata->PreCreate_CallBack(&result);

		if (pdata->Create_CallBack)
			pdata->Create_CallBack(&result,&pspline,&psp); // user can fill up , or not the psp pointer with le knot he wants to move after create.
		else
		{
			desc[0].x = desc[1].x = result.x;
			desc[0].y = desc[1].y = result.y;
			desc[0].z = desc[1].z = result.z;
			desc[0].TBuildCode = desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_N1, 0, NSKT_P4, 0, NSKT_WORLD_N1, 0, NSKT_M4, 0);
			pspline = NCreateSpline(pdata->NewSplineAccuracy, 2, desc);
			if(pspline)
				psp = (NSPLINEKNOT*)NGetArrayPtr(&pspline->KnotArray, 1);
		}
		
		if(pspline)
		{
			NUT_SplinesPackToolData_BindSpline(pdata, pspline);

			pselect->pSelected = psp;
			pselect->pSelectedOwner = pspline;
			pselect->SelectParam = NMAKELONG(NUT_SPACK_SPLINEKNOT_POSITION, _END_KNOT);
			pselect->ZfromCam = 0.0f; // Unused ?
		}
		else
		{
			NResetToolSelection(pselect);
		}

		// CALL BACK 'POST' CREATE
		if (pdata->PostCreate_CallBack)
			pdata->PostCreate_CallBack(pspline);
		return NEDITOR_RESULT_TOOL_ON;
	
	case NTOUCH_MOVE:
		pdata = (NUT_SPACK_TOOLDATA *)tool_param;
		// 			ptouch=(NEVENT_TOUCH*)pevent;
		psp = (NSPLINEKNOT *)pselect->pSelected;
		if (psp)
		{
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);

		

			if (NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &result))
			{
				// CALL BACK 'PRE' MOVE
				if (pdata->PreMove_CallBack)
					pdata->PreMove_CallBack((NSPLINE*)pselect->pSelectedOwner, psp, pselect->SelectParam);
				
				NVec3Sub(&psp->TA, &psp->TA, &psp->Position);
				NVec3Sub(&psp->TB, &psp->TB, &psp->Position);
				NVec3AddTo(&psp->TA, &result);
				NVec3AddTo(&psp->TB, &result);
				psp->Position = result;

				NUT_ApplySplinesPackConstraints(pdata, (NSPLINE*)pselect->pSelectedOwner, psp, (NUT_SPACK_SPLINEPOINT_ENUM)NHIWORD(pselect->SelectParam));

				bmoved = NTRUE;
				NUT_ApplySplinesPackSymmetries(pdata, (NSPLINE*)pselect->pSelectedOwner);

				// CALL BACK 'POST' MOVE
				if (pdata->PostMove_CallBack)
					pdata->PostMove_CallBack((NSPLINE*)pselect->pSelectedOwner, psp, pselect->SelectParam);
				return NEDITOR_RESULT_TOOL_ON;
			}
		}
		break;

	case NTOUCH_END:
		if (pselect->pSelected && bmoved)
		{
			// 				NCallCurrentToolCallBack(pevent);
			NResetToolSelection(pselect);
			bmoved = NFALSE;
			return NEDITOR_RESULT_TOOL_ON;
		}
		else
		{
			NResetToolSelection(pselect);
		}
	default:
		break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT NUT_SplinesPackTool_DeleteSpline(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	return NEDITOR_RESULT_TOOL_OFF;
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR