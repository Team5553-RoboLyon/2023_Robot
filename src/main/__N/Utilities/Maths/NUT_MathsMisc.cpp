// ==========================================================================================
// NUT_MathsMix.cpp
// ------------------------------------------------------------------------------------------
// Author	: Jean-Marie Nazaret
// Create	: 07/05/2012
// Modified : 
// ==========================================================================================
#include "../../GL/Ngl.h"
#include "../../NErrorHandling.h"
#include "../../NUsualCoordinates.h"

#include "NUT_MathsMisc.h"

// ------------------------------------------------------------------------------------------
// void NUT_Build3DOrthonormal
// ------------------------------------------------------------------------------------------
// Description :
//	Build an Orthonormal into a 3x3 matrix from 1,2 or 3 vectors.
//	if a problem occurs, like collinear vectors, function returns without changing the incoming orthonormal.
// ------------------------------------------------------------------------------------------
// In	:
//
//	NMATRIX3x3 *porthonormal	Orthonormal to update( a Matri3x3 ). It will be update at the end only if process succeds.
//								and will remain unchanged in case of failure.
//								So a good practice could be set it up to identity or something acceptable before calling this function
//								if there are inconsistency possibility for incoming vector (pto,pfrom or pup).
//
//		const NVEC3 *pto		'pto' has to be defined (it can not be a NULL pointer)
//								--> NEngine is going to considers 'pto' in 2 different ways according with 'pfrom' ( see b)'pfrom' )
//
//		const NVEC3 *pfrom	'pfrom' may be defined or not (NULL or NON NULL)
//								--> if 'pfrom is NULL' NEngine understand 'pto' IS a direction vector, THE FORWARD vector itself.
//									In that case 'pto' needs to have enough magnitude (it has not to be a unit vector ).
//								-->	if 'pfrom is non NULL' NEngine Understand 'pfrom' and 'pto' as 2 positions in space. The FORWARD vector will be FORWARD(pfrom,pto)
//								In that case 'pto' and 'pfrom' has to be 2 distinct positions.
//
//		const NVEC3 *pup		'pup' may be defined or not (NULL or NON NULL)
//								--> if 'pup is NULL' NEngine try first using WORL UP (0,1,0) as intermediate build up vector,
//									to build Side with forward. If it fails because WORLD UP and forward are close together (collinear)
//									NEngine will use WORLD FORWARD(0,0,1) as intermediate build up vector.
//								--> if 'pup is NON NULL' NEngine is going to use it as a direction vector (up reference) to build Side with forward.
//									If it fails because up magnitude is close to zero, or because up and FORWARD are close together (collinear)
//									function will stop and returns without updating 'porthonormal' (and will CRASH in debug mode)
//									
// Out :
//								The incoming porthonormal 'updated' ( or not in case of trouble - RELEASE only because it will crash inside the function in DEBUG )
//
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NUT_Build3DOrthonormal(NMATRIX3x3 *porthonormal, const NVEC3 *pto, const NVEC3 *pfrom, const NVEC3 *pup)
{
	// 'pto' has to be valid NVEC3 pointer the 2 others may be NULL pointer. 
	NErrorIf(!pto,NERROR_NULL_VECTOR);

	Nf32		length;
	NMATRIX3x3	mat;

	// forward = to - from
	if(pfrom)
		NVec3Sub((NVEC3*)&mat.Forward,pto,pfrom);
	else
		*(NVEC3*)&mat.Forward=*pto;
	length = NVec3Length(&mat.Forward);
	if(length<NF32_EPSILON_VECTOR_LENGTH)// "pto" and "pfrom" are close together ! if they are both define
	{									 // or "pto" is close to be a NULL vector (or it is) if it is alone. (pfrom = NULL)	
		NErrorIf(1,NERROR_NULL_VECTOR);	 // Crash in Debug Mode	Only
		return porthonormal;
	}
	NVec3ScaleBy(&mat.Forward,1.0f/length);		// normalize forward AXIS
	
	// up
	if(pup)
	{
		NVec3CrossProduct(&mat.Side,pup,&mat.Forward);	// use pup vector to build side AXIS
		length = NVec3Length(&mat.Side);
		if(length < NF32_EPSILON_VECTOR_LENGTH)
		{
		// too small length means pup and forward are almost collinear.
		// so its better to stop function here and return NFALSE. 
		// User should choose an another up vector ! A one which is not collinear with forward 
		// notes:
		// forward and up have both a length of 1. so side = 1 x 1 x sin(up,forward)
		// so if length = 0.001f that means sin(up,forward)= 0.01f !
		// ... and that means the angle between up and forward = 0.057 deg 
		// ... close to be collinear isn't it ? So imagine for a length value < NF32_EPSILON_VECTOR_LENGTH !
			NErrorIf(1,NERROR_COLLINEAR_VECTORS);
			return porthonormal;
		}
		NVec3ScaleBy(&mat.Side,1.0f/length); // normalize 
	}
	else
	{
		NVEC3	up = {_WORLD_UP_3f};
		
		NVec3CrossProduct(&mat.Side,&up,&mat.Forward);	// use up vector to build side AXIS
		length = NVec3Length(&mat.Side);
		if(length < NF32_EPSILON_VECTOR_LENGTH)
		{
			// too small length means pup and forward are almost collinear.
			// So here, because user let the function choose the UP we are going to choose Z axis as a new UP ...
			NVec3Set(&up,_WORLD_FORWARD_3f);
			NVec3CrossProduct(&mat.Side,&up,&mat.Forward);	// use up vector to build side AXIS
			length = NVec3Length(&mat.Side);
			if(length < NF32_EPSILON_VECTOR_LENGTH)
			{
				NErrorIf(1,NERROR_COLLINEAR_VECTORS);
				return porthonormal;
			}
			NVec3ScaleBy(&mat.Side,1.0f/length); // normalize 
		}
	}
	// Compute Base Up = forward x side
	NVec3CrossProduct(&mat.Up,&mat.Forward,&mat.Side);
	NVec3Normalize(&mat.Up);

	// copy result
	*porthonormal = mat;

	return porthonormal;
}

// ------------------------------------------------------------------------------------------
// NMATRIX3x3* NUT_BuildHughesMoller3DOrthonormal
// ------------------------------------------------------------------------------------------
// Description :
//	Based on the paper from Tomas Möller and John F.Hughes
//			"Extending a Unit Vector to an Orthonormal Basis of 3-Space"
//
//	This function build in a quick way a Right Hand 3D Orthonormal from a simple NVEC3.
//	ZAxis is the normalized version of this incoming NVEC3.
//
//	! WARNING ! Function works without taking care about WOLRD UP Vector !!
//	... So in several cases chosen UP ( and choosen RIGHT ) may be not the ones user expected to have,
//	even if the Orthonormal still a Right Hand One ( ALWAYS).
// ------------------------------------------------------------------------------------------
// In	:
//			NMATRIX3x3	*porthonormal
//			NVEC3	*pforward
// Out :
//
//
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NUT_BuildHughesMoller3DOrthonormal(NMATRIX3x3 *porthonormal, const NVEC3 *pforward)
{
	// if pforward is almost NULL vector
	NErrorIf(NVec3SquareLength(pforward) < NF32_EPSILON_VECTOR_LENGTH,NERROR_NULL_VECTOR);
	
	porthonormal->ZAxis = *pforward;
	NVec3Normalize(&porthonormal->ZAxis);

	Nf32 mini = NMIN(NABS(pforward->x),NMIN(NABS(pforward->y),NABS(pforward->z)));
	if(mini == NABS(pforward->x))
	{
		porthonormal->YAxis.x =  0.0f;
		porthonormal->YAxis.y = -pforward->z;
		porthonormal->YAxis.z =  pforward->y;
	}
	else if(mini == NABS(pforward->y))
	{
		porthonormal->YAxis.x = -pforward->z;
		porthonormal->YAxis.y =  0.0f;
		porthonormal->YAxis.z =  pforward->x;
	}
	else
	{
		porthonormal->YAxis.x = -pforward->y;
		porthonormal->YAxis.y =  pforward->x;
		porthonormal->YAxis.z =  0.0f;
	}
	NVec3Normalize(&porthonormal->YAxis);
	
	NVec3CrossProduct(&porthonormal->XAxis,&porthonormal->YAxis,&porthonormal->ZAxis);
	NVec3Normalize(&porthonormal->XAxis);
	return porthonormal;
}

void NUT_ExtractUpSideFromModelViewMatrix(NVEC3* up,NVEC3* side)
{
	Nf32 modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	side->x		= modelview[0];
	side->y		= modelview[4];
	side->z		= modelview[8];

	up->x		= modelview[1];
	up->y		= modelview[5];
	up->z		= modelview[9];
}


/*

OLD 


// --------------------------------------------------------------------------- ---------------
// NUT_HughesMollerChooseUp
// ------------------------------------------------------------------------------------------
// Description :
//	Choose a Up vector regarding a forward vector, to build Matrix or orthogonal. 
//	This chosen up vector is not the matrix/orthogonal Up but the one used to get side 
//
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC3		*pup				The up vector to fill in.
//		const NVEC3	*pforward			The forward vector.
// Out :
//
// ------------------------------------------------------------------------------------------
void NUT_HughesMollerChooseUp(NVEC3 *pup, const NVEC3 *pforward)
{
	// if pforward is almost NULL vector
	NErrorIf(NVec3SquareLength(pforward) < NF32_EPSILON_VECTOR_LENGTH,NERROR_NULL_VECTOR);

	// Up choose 3: “Take the smallest entry (in absolute value) of u and set it to zero,
	// swap the other two entries and negate the first of them.” ... from Hughes-Möller
	Nf32 mini = NMIN(NABS(pforward->x),NMIN(NABS(pforward->y),NABS(pforward->z)));
	if(mini == NABS(pforward->x))
	{
		pup->x = 0.0f;
		pup->y = -pforward->z;
		pup->z = pforward->y;
	}
	else if(mini == NABS(pforward->y))
	{
		pup->x = -pforward->z;
		pup->y = 0.0f;
		pup->z = pforward->x;
	}
	else
	{
		pup->x = -pforward->y;
		pup->y = pforward->x;
		pup->z = 0.0f;
	}
}
*/

void NUT_LinearRegression()
{
	/*
	int i;
	double xsomme, ysomme, xysomme, xxsomme;
	double ai, bi;

	xsomme = 0.0; ysomme = 0.0;
	xysomme = 0.0; xxsomme = 0.0;
	for (i = 0; i < n; i++)
	{
		xsomme = xsomme + x[i];
		ysomme = ysomme + y[i];
		xysomme = xysomme + x[i] * y[i];
		xxsomme = xxsomme + x[i] * x[i];
	}
	ai = (n*xysomme - xsomme * ysomme) / (n*xxsomme - xsomme * xsomme);
	bi = (ysomme - ai * xsomme) / n;
	*a = ai;
	*b = bi;
	return;
	*/
}