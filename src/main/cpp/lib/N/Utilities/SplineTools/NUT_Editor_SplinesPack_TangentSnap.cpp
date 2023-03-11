#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "lib/N/NCStandard.h"
#include "lib/N/UI/NUI.h"
#include "../../NCamera.h"
#include "../../NCoordinateSystemConversions.h"
#include "lib/N/NEditor.h"
#include "../../Utilities/NUT_X.h"
#include "../../Utilities/Draw/NUT_Draw.h"

#include "NUT_Editor_SplinesPack_Tools.h"


// ------------------------------------------------------------------------------------------
// 	NUT_ApplySplinesPackTangentsSnap
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
// Out :
//
// ------------------------------------------------------------------------------------------
Nbool NUT_ApplySplinesPackTangentsSnap(NUT_SPACK_TOOLDATA *pdata, const Nu32 snapflags_toolrequest, NVEC3 *ptangent,const NVEC3 *ptargettangent,const NVEC3 *pknotposition )
{
	NVEC3	vtang,vtarget;	// "vtang" means "tangent vector" and "vtarget" means "target tangent vector"
	NVEC3	P,vdist;
	NVEC3	vsnaptangent;
	Nf32		lvtarget,lvtargetsquared;
	Nf32		lvtang;
	Nf32		t;
	Nf32		fdist;
	Nbool		job;
	Nf32		numsize,densize;
	Nf32		snapdist;
	NVEC3	vbase;


	NUT_DRAWTEXT	drawtext_param;
	char		txt[128];

	NVec3Sub(&vtarget,ptargettangent,pknotposition);
	lvtargetsquared =  NVec3SquareLength(&vtarget);
	job = NFALSE;

	// To perform Tangent Snap, tangent target has to be long enough !
	if(lvtargetsquared > XEPSILON_SQUARED)
	{
		lvtarget = sqrt(lvtargetsquared);

		NVec3Sub(&vtang,ptangent,pknotposition);
		lvtang		= NVec3Length(&vtang);
		
		
		//snapdist = NMIN(lvtang*NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_DISTANCERATIO,NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_DISTANCEMAX);
		snapdist = lvtang * NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_DISTANCERATIO;

		//printf("\n SnapDist=%6f",snapdist);
		
		// Init of elements use to compute full snap result
		numsize	= lvtang; // "numerator"	size
		densize	= lvtang; // "denominator"	size
		vbase	= vtang;
		
		// First: Length Snap
		// To be applied, a snap has to be requested by tool and allowed by user ( right FlagsSplinesPack set to ON ) 
		if( ISFLAG_ON(snapflags_toolrequest,FLAG_SPACK_TANGENTSNAP_LENGTH_ON) && ISFLAG_ON(pdata->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_LENGTH_ON) )
		{
			if( NABS(lvtarget-lvtang) < snapdist )
			{
				numsize	= lvtarget;
				job		= NTRUE;
			}
		}

		// Second: Orientation Snap (align0ment )
		if( ISFLAG_ON(snapflags_toolrequest,FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON) && ISFLAG_ON(pdata->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON) )
		{
			t = NVec3DotProduct(&vtang,&vtarget)/ lvtargetsquared; 
			// ... / lvtargetsquaredsquared ??? 
			// YES !!! because 't' should be the dot product with vtang AND the normalized vector n from vtarget ( so vtarget / lvtarget)
			// AND, in the top of that, to obtain the projected vector of vtang ON vtarget we have to multiply the normaliezd vector n by 't'
			// So instead calculating 'n' we are using "vtarget" and we intergate the 2 divisions by lvtarget 'INTO' t... It's a trick to avoid the 3 divisons used by the vtarget normalization... :) 
			//NVEC3 n;
			//n.x = vtarget.x / lvtarget;
			//n.y = vtarget.y / lvtarget;
			//n.z = vtarget.z / lvtarget;

			if (t < 0.0f) // Be sure the 2 tangents are not on the "same side"
			{
				// Compute 'P', projection of the 'head' of vtang 'ON' vtarget
				// with n it should be: 
				//P.x = t * n.x; 
				//P.y = t * n.y;
				//P.z = t * n.z;
				// but it's like this, with t divided par "lvtargetsquared".
				P.x = t * vtarget.x;
				P.y = t * vtarget.y;
				P.z = t * vtarget.z;
				// compute 'vdist' the separating vector between the "head" of vtang and the "head" of it's projection on vtarget 		
				NVec3Sub(&vdist, &P, &vtang);
				fdist = NVec3Length(&vdist);
				if (fdist < snapdist )
				{
					vbase = vtarget;
					densize = -lvtarget;
					job = NTRUE;
				}

				sprintf(txt, "     %.2f,%.2f,%2f", pknotposition->z,ptargettangent->z,ptangent->z);
				NSetColorf(&drawtext_param.Color, NCOLOR_PRESET3F_BLUE_AQUAMARINE, 1);
				drawtext_param.Size =2.0f;
				NUT_Draw_Text(txt, ptangent, &drawtext_param);

				NVEC3 A, B;
				A = *pknotposition;
				NVec3Add(&B, &A, &P);
				NUT_Draw_Segment(&A, &B, &drawtext_param.Color);

				NVec3Add(&B, &A, &vtang);
				NUT_Draw_Segment(&A, &B, &drawtext_param.Color);

				NVec3Add(&B, &A, &vtarget);
				NUT_Draw_Segment(&A, &B, &drawtext_param.Color);
			}
			/*
			t = NVec3DotProduct(&vtang, &vtarget) / (lvtarget*lvtang);
			if ( (t < -NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_COSANGLE) && ( t > -1.0f) ) // '-' because the 2 tangents must be on the "opposite side" to snap
			{
					vbase = vtarget;
					densize = -lvtarget;
					job = NTRUE;
			}
			*/
		}

		// End : Apply snap transformations if necessary
		if(job)
		{
			// some examples to explain:
			//		We use "vbase" which can be "vtarget" or "vtang". It's easy to understand. If the Orientation snap is not effective, vbase still equal to vtang ( the snapped tangent before snap ).
			//		In that way the calculous bellow may affect only its size.
			//		But, if the orientation snap is effective, that means the snapped tangent must be aligned with the target tangent ( but opposite direction ). To perform that we are going to set vbase as vtarget ( the target tangent ).
			//		In that way the calculous bellow may affect the orientation to.
			//
			//		So, 
			//			The 'vbase' purpose is to start the snaped tangent computing in the right direction ( 2 possible ones: current direction of the snapped tangent or direction of the target tangent ).
			//			The 'numsize/densize' ratio purpose is making the right size of the snaped tangent.
			//			If vbase is equal to the current snapped tangent ( Orientation SNAP OFF) , then, densize will be equal to the current length of snap tangent. In that way, vbase/densize will be an unitary vector oriented 
			//			in the current direction of the snaped tangent.
			//			If vbase is equal to the target tangent ( Orientation SNAP OFF) , then, densize will be equal to the current length of target tangent * -1. In that way, vbase/densize will be an unitary vector oriented 
			//			in the oposite direction of the target tangent.
			//			And,
			//			we will set "numsize"  to the length we want for the snaped tangent. There are 2 possible values:  current length of snapped tangent ( LENGTH SNAP OFF ) or length of the target tangent ( LENGTH SNAP ON ).
			NVec3Scale(&vsnaptangent,&vbase,numsize/densize);
			NVec3Add(ptangent,pknotposition,&vsnaptangent);
		}
	}
	else
	{
		lvtarget = 0;
	}

	return job;
}

// -----------------------------------------------------------------------
#endif	//_NEDITOR