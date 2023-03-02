#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../NCStandard.h"
#include "../../UI/NUI.h"
#include "../../NCamera.h"
#include "../../NCoordinateSystemConversions.h"
#include "../../NEditor.h"
#include "../../NErrorHandling.h"
#include "../../NSpline.h"
#include "NUT_Editor_SplinesPack_Tools.h"

// ------------------------------------------------------------------------------------------
// void NUT_CreateSplinesPackSymmetry
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			const Nu32			Aspline_id		ID of the 'source Spline' this one is not going to be modified
//			const Nu32			Bspline_id		ID of the 'Target Spline' this one is going to be update from the "Aspline_id"	
//			SNKS_SPS_AXIS_ENUM	sym_axis		Symmetry Axis used to compute BSpline_id from Aspline_id
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_CreateSplinesPackSymmetry(NUT_SPACK_TOOLDATA *pt, const Nu32 Aspline_id,const Nu32 Bspline_id,const NUT_SPACK_SYM_AXIS_ENUM sym_axis)
{
	NUT_SPACK_SYMMETRY	sps;


	// Error check:
	NErrorIf(Aspline_id == Bspline_id,NERROR_VALUES_HAVE_TO_BE_DIFFERENT);				// Operand A as to be different than Operand B.
														
	// PreSet:
	memset(&sps,0,sizeof(NUT_SPACK_SYMMETRY));
	sps.OperandA_SplineID		= Aspline_id;
	sps.OperandB_SplineID		= Bspline_id;
	sps.Symmetry_Axis			= _SafeNu32ToNu8(sym_axis);

	NArrayPushBack(&pt->Symmetries,(NBYTE*)&sps);
}

// ------------------------------------------------------------------------------------------
// NUT_ApplySplinesPackSymmetries
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
Nbool NUT_ApplySplinesPackSymmetries(NUT_SPACK_TOOLDATA *pt, const NSPLINE *pmodified_spline)
{
	NUT_SPACK_SYMMETRY			*psps;
	Nu32						i,j;
	NSPLINE						*psplineA,*psplineB;
	NSPLINEKNOT					*pspA,*pspB;
	NVEC3					vsym[6] = { {1,-1,1},	//_2D_XAXIS	
											{-1,1,1},	//_2D_YAXIS	
											{-1,-1,1},	//_2D_ZAXIS	
											{1,-1,-1},	//_3D_XAXIS		
											{-1,1,-1},	//_3D_YAXIS	
											{-1,-1,1}	//_3D_ZAXIS	
														};

	// Symmetry unactivated !
	if(ISFLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_SYMMETRY_ON))
		return NFALSE;

	psps = (NUT_SPACK_SYMMETRY *)NGetFirstArrayPtr(&pt->Symmetries);
	for(i=0;i<NGetArraySize(&pt->Symmetries);i++,psps++)
	{
		psplineA = *( (NSPLINE**)NGetArrayPtr(&pt->ArrayOfSplinesPtr,psps->OperandA_SplineID) );
		if(pmodified_spline == psplineA )
		{
			psplineB = *( (NSPLINE**)NGetArrayPtr(&pt->ArrayOfSplinesPtr,psps->OperandB_SplineID) );
		
			// CALL BACK 'PRE' SYMMETRY APPLY
			if(pt->PreApplySymmetry_CallBack)
				pt->PreApplySymmetry_CallBack(psplineB,psplineA,(NUT_SPACK_SYM_AXIS_ENUM)psps->Symmetry_Axis);

			// first: the 2 splines need to have the same number of spline points.
			if( NGetSplineKnotsNumber(psplineA)!=NGetSplineKnotsNumber(psplineB) )
			{
				NResizeArray(&psplineB->KnotArray,NGetArraySize(&psplineA->KnotArray),NULL,NULL);
			}
			
			// second: Symmetry ! 
			// Processing all the Spline Knots each time because we don't exactly know which Spline Knots
			// was modified (it could be possible to have several modified Spline Knots thanks to the constraint process previously applied)
			pspA = NGetFirstSplineKnot(psplineA);
			pspB = NGetFirstSplineKnot(psplineB);
			for(j=0;j<NGetSplineKnotsNumber(psplineA);j++,pspA++,pspB++)
			{
				// copy
				pspB->Position	= pspA->Position;
				pspB->TA		= pspA->TA;
				pspB->TB		= pspA->TB;
				
				// Symmetry
				NVec3SubFrom(&pspB->TA,&pspB->Position);
				NVec3SubFrom(&pspB->TB,&pspB->Position);

				pspB->TA.x*=vsym[psps->Symmetry_Axis].x;
				pspB->TA.y*=vsym[psps->Symmetry_Axis].y;
				pspB->TA.z*=vsym[psps->Symmetry_Axis].z;
				
				pspB->TB.x*=vsym[psps->Symmetry_Axis].x;
				pspB->TB.y*=vsym[psps->Symmetry_Axis].y;
				pspB->TB.z*=vsym[psps->Symmetry_Axis].z;

				pspB->Position.x*=vsym[psps->Symmetry_Axis].x;
				pspB->Position.y*=vsym[psps->Symmetry_Axis].y;
				pspB->Position.z*=vsym[psps->Symmetry_Axis].z;
				
				NVec3AddTo(&pspB->TA,&pspB->Position);
				NVec3AddTo(&pspB->TB,&pspB->Position);
			}

			// CALL BACK 'POST' SYMMETRY APPLY
			if(pt->PostApplySymmetry_CallBack)
				pt->PostApplySymmetry_CallBack(psplineB,psplineA,(NUT_SPACK_SYM_AXIS_ENUM)psps->Symmetry_Axis);

		}
	}

	return NTRUE;
}

// -----------------------------------------------------------------------
#endif	//_NEDITOR