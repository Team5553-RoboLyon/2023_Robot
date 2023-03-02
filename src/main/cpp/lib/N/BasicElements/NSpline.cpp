#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NNode.h"
#include "lib/N/NSpline.h"

// N Used this LUT to build in SplineKnot tangents there are 15 useful values of 16. Actually, the last one (value 16) is not used
// but to stay coherent with the number of values usable with 4 bits we keep a LUT of 16 values ...
static Nf32 NSplineKnotTangentCoordLUT[16] = {0.0f, 1.0f / 4.0f, 1.0f / 3.0f, 1.0f / 2.0f, 1.0f, 2.0f, 3.0f, 4.0f,
											  -1.0f / 4.0f, -1.0f / 3.0f, -1.0f / 2.0f, -1.0f, -2.0f, -3.0f, -4.0f, 0.0f};

static inline Nbool _build_localsystem(NVEC3 *plocalx, NVEC3 *plocaly, NVEC3 *plocalz, Nf32 *plength_p1p0, const NVEC3 *p0, const NVEC3 *p1)
{
	Nf32 base, lx;
	NVEC3 up;

	NVec3Sub(plocalz, p1, p0);
	base = NVec3Length(plocalz);
	if (base <= NF32_EPSILON_VECTOR_LENGTH) // instead of 'base==NF32EPSILON'
	{
		return NFALSE;
	}
	else
	{
		NVec3Scale(plocalz, plocalz, 1.0f / base); // normalize
	}

	/* Side = forward x up */
	// cross(forward, up, side);
	NVec3Set(&up, 0, 1, 0);
	NVec3CrossProduct(plocalx, plocalz, &up);
	lx = NVec3Length(plocalx);

	// if too small lx that means up and locale are almost collinear.
	// so its better to choose an another up vector
	// notes:
	// localz and up have both a length of 1. so lx = 1 x 1 x sin(up,plocalz)
	// so if lx = 0.001f that means sin(up,plocalz)= 0.01f !
	// ... and that means the angle between up and plocalz = 0.057 deg
	// ... so, close to be collinear isn't it ?
	if (lx < NF32_EPSILON_VECTOR_LENGTH)
	{
		NVec3Set(&up, 1, 0, 0);
		NVec3CrossProduct(plocalx, plocalz, &up);
		lx = NVec3Length(plocalx);
	}
	NVec3Scale(plocalx, plocalx, 1.0f / lx); // normalize

	/* Recompute up as: up = side x forward */
	// cross(side, forward, up);
	NVec3CrossProduct(plocaly, plocalx, plocalz);

	// setup base length
	if (plength_p1p0)
	{
		*plength_p1p0 = base;
	}

	return NTRUE;
}

static inline void _extract_splineknot0_TB(NSPLINEKNOT *psk0, NSPLINEKNOT *psk1, const Nu32 tangents_code)
{
	Nu8 coord_system, code_tx, code_ty, code_tz;
	Nf32 base_length;
	NVEC3 world;
	NVEC3 localx, localy, localz;
	NVEC3 base;

	coord_system = (Nu8)((tangents_code >> 12) & 0xF);
	code_tx = (Nu8)((tangents_code >> 8) & 0xF);
	code_ty = (Nu8)((tangents_code >> 4) & 0xF);
	code_tz = (Nu8)(tangents_code & 0xF);

	switch (coord_system)
	{
		// Direct Orthonormal base with Z axis align with vector (p1,p0).
		// i,j,k have a length of 1
	case NSKT_LOCAL_N1:
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx]);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty]);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz]);
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// Direct Orthonormal base with Z axis align with vector (p1,p0).
		// i,j,k have a length of "L" which is the length of vector (p1,p0)
	case NSKT_LOCAL_NRL:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, &base_length, &psk0->Position, &psk1->Position))
		{
			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// Direct Orthonormal base with Z axis align with vector (p1,p0).
		// i,j,k have a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
	case NSKT_LOCAL_NRX:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.x - psk0->Position.x);

			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// Direct Orthonormal base with Z axis align with vector (p1,p0).
		// i,j,k have a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
	case NSKT_LOCAL_NRY:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.y - psk0->Position.y);

			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// Direct Orthonormal base with Z axis align with vector (p1,p0).
		// i,j,k have a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
	case NSKT_LOCAL_NRZ:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.z - psk0->Position.z);

			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// Direct Orthogonal base with Z axis align with vector (p1,p0).
		// i,j,k have their own length which are the absolute value of the vector (p1,p0) X,Y,Z World components.
	case NSKT_LOCAL_RXYZ:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			NVec3Sub(&base, &psk1->Position, &psk0->Position);

			psk0->TB = psk0->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * NABS(base.x));
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * NABS(base.y));
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * NABS(base.z));
			NVec3AddTo(&psk0->TB, &localx);
			NVec3AddTo(&psk0->TB, &localy);
			NVec3AddTo(&psk0->TB, &localz);
		}
		else
			NVec3Set(&psk0->TB, 0, 0, 0);
		break;

		// WORLD ORTHORMAL BASE.
		// i,j,k have a length of 1
	case NSKT_WORLD_N1:
		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx];
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty];
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz];
		break;

		// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
		// i,j,k have a length of "L" which is the length of vector (p1,p0)
	case NSKT_WORLD_NRL:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		base_length = NVec3Length(&world);
		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx] * base_length;
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty] * base_length;
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz] * base_length;
		break;

		// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
		// i,j,k have a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
	case NSKT_WORLD_NRX:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);

		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.x);
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.x);
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.x);
		break;

		// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
		// i,j,k have a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
	case NSKT_WORLD_NRY:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.y);
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.y);
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.y);
		break;

		// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
		// i,j,k have a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
	case NSKT_WORLD_NRZ:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.z);
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.z);
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.z);
		break;

		// Direct Orthogonal base align with the WORLD ORTHORMAL BASE.
		// i,j,k have their own length which are the absolute value of the vector (p1,p0) X,Y,Z World components.
	case NSKT_WORLD_RXYZ:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk0->TB.x = psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.x);
		psk0->TB.y = psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.y);
		psk0->TB.z = psk0->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.z);
		break;

	default:
		NVec3Set(&psk0->TB, 0, 0, 0);
		break;
	}

	/*
		switch( coord_system )
		{
		case NSKT_3DLOCAL_RELATIVE:
			// Build local system
			if( _build_localsystem(&localx,&localy,&localz,&psk0->Position,&psk1->Position,NTRUE) )
			{
				psk0->TB = psk0->Position;
				NVec3Scale(&localx,&localx,NSplineKnotTangentCoordLUT[code_tx] );
				NVec3Scale(&localy,&localy,NSplineKnotTangentCoordLUT[code_ty] );
				NVec3Scale(&localz,&localz,NSplineKnotTangentCoordLUT[code_tz] );
				NVec3AddTo(&psk0->TB,&localx);
				NVec3AddTo(&psk0->TB,&localy);
				NVec3AddTo(&psk0->TB,&localz);
			}
			else
				NVec3Set(&psk0->TB,0,0,0);
			break;

		case NSKT_3DLOCAL_ABSOLUTE:
			if( _build_localsystem(&localx,&localy,&localz,&psk0->Position,&psk1->Position,NFALSE) )
			{
				psk0->TB = psk0->Position;
				NVec3Scale(&localx,&localx,NSplineKnotTangentCoordLUT[code_tx] );
				NVec3Scale(&localy,&localy,NSplineKnotTangentCoordLUT[code_ty] );
				NVec3Scale(&localz,&localz,NSplineKnotTangentCoordLUT[code_tz] );
				NVec3AddTo(&psk0->TB,&localx);
				NVec3AddTo(&psk0->TB,&localy);
				NVec3AddTo(&psk0->TB,&localz);
			}
			else
				NVec3Set(&psk0->TB,0,0,0);
			break;

		case NSKT_3DWORLD_RELATIVE:
			NVec3Sub(&world,&psk1->Position,&psk0->Position);
			base = NVec3Length(&world);
			psk0->TB.x =  psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx]*NABS(world.x);
			psk0->TB.y =  psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty]*NABS(world.y);
			psk0->TB.z =  psk0->Position.z + NSplineKnotTangentCoordLUT[code_ty]*NABS(world.z);
			break;

		case NSKT_3DWORLD_ABSOLUTE:
			psk0->TB.x =  psk0->Position.x + NSplineKnotTangentCoordLUT[code_tx];
			psk0->TB.y =  psk0->Position.y + NSplineKnotTangentCoordLUT[code_ty];
			psk0->TB.z =  psk0->Position.z + NSplineKnotTangentCoordLUT[code_ty];
			break;

		default:
			NVec3Set(&psk0->TB,0,0,0);
			break;
		}
		*/
}

static inline void _extract_splineknot1_TA(NSPLINEKNOT *psk0, NSPLINEKNOT *psk1, const Nu32 tangents_code)
{
	NVEC3 world;
	Nu8 coord_system, code_tx, code_ty, code_tz;
	Nf32 base_length;
	NVEC3 localx, localy, localz;
	NVEC3 base;

	// Crack the code !
	coord_system = (Nu8)((tangents_code >> 28) & 0xF);
	code_tx = (Nu8)((tangents_code >> 24) & 0xF);
	code_ty = (Nu8)((tangents_code >> 20) & 0xF);
	code_tz = (Nu8)((tangents_code >> 16) & 0xF);

	switch (coord_system)
	{
	// Direct Orthonormal base with Z axis align with vector (p1,p0).
	// i,j,k have a length of 1
	case NSKT_LOCAL_N1:
		if (_build_localsystem(&localx, &localy, &localz, &base_length, &psk0->Position, &psk1->Position))
		{
			psk1->TA = psk1->Position;
			NVec3Scale(&localx, &localx, NSplineKnotTangentCoordLUT[code_tx]);
			NVec3Scale(&localy, &localy, NSplineKnotTangentCoordLUT[code_ty]);
			NVec3Scale(&localz, &localz, NSplineKnotTangentCoordLUT[code_tz]);
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// Direct Orthonormal base with Z axis align with vector (p1,p0).
	// i,j,k have a length of "L" which is the length of vector (p1,p0)
	case NSKT_LOCAL_NRL:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, &base_length, &psk0->Position, &psk1->Position))
		{
			psk1->TA = psk1->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// Direct Orthonormal base with Z axis align with vector (p1,p0).
	// i,j,k have a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
	case NSKT_LOCAL_NRX:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.x - psk0->Position.x);

			psk1->TA = psk1->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// Direct Orthonormal base with Z axis align with vector (p1,p0).
	// i,j,k have a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
	case NSKT_LOCAL_NRY:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.y - psk0->Position.y);

			psk1->TA = psk1->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// Direct Orthonormal base with Z axis align with vector (p1,p0).
	// i,j,k have a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
	case NSKT_LOCAL_NRZ:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			base_length = NABS(psk1->Position.z - psk0->Position.z);

			psk1->TA = psk1->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * base_length);
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * base_length);
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * base_length);
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// Direct Orthogonal base with Z axis align with vector (p1,p0).
	// i,j,k have their own length which are the absolute value of the vector (p1,p0) X,Y,Z World components.
	case NSKT_LOCAL_RXYZ:
		// Build local system
		if (_build_localsystem(&localx, &localy, &localz, NULL, &psk0->Position, &psk1->Position))
		{
			NVec3Sub(&base, &psk1->Position, &psk0->Position);

			psk1->TA = psk1->Position;
			NVec3ScaleBy(&localx, NSplineKnotTangentCoordLUT[code_tx] * NABS(base.x));
			NVec3ScaleBy(&localy, NSplineKnotTangentCoordLUT[code_ty] * NABS(base.y));
			NVec3ScaleBy(&localz, NSplineKnotTangentCoordLUT[code_tz] * NABS(base.z));
			NVec3AddTo(&psk1->TA, &localx);
			NVec3AddTo(&psk1->TA, &localy);
			NVec3AddTo(&psk1->TA, &localz);
		}
		else
			NVec3Set(&psk1->TA, 0, 0, 0);
		break;

	// WORLD ORTHORMAL BASE.
	// i,j,k have a length of 1
	case NSKT_WORLD_N1:
		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx];
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty];
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz];
		break;

	// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
	// i,j,k have a length of "L" which is the length of vector (p1,p0)
	case NSKT_WORLD_NRL:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		base_length = NVec3Length(&world);
		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx] * base_length;
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty] * base_length;
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz] * base_length;
		break;

	// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
	// i,j,k have a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
	case NSKT_WORLD_NRX:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);

		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.x);
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.x);
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.x);
		break;

	// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
	// i,j,k have a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
	case NSKT_WORLD_NRY:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.y);
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.y);
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.y);
		break;

	// Direct Orthonormal base align with the WORLD ORTHORMAL BASE.
	// i,j,k have a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
	case NSKT_WORLD_NRZ:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.z);
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.z);
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.z);
		break;

	// Direct Orthogonal base align with the WORLD ORTHORMAL BASE.
	// i,j,k have their own length which are the absolute value of the vector (p1,p0) X,Y,Z World components.
	case NSKT_WORLD_RXYZ:
		NVec3Sub(&world, &psk1->Position, &psk0->Position);
		psk1->TA.x = psk1->Position.x + NSplineKnotTangentCoordLUT[code_tx] * NABS(world.x);
		psk1->TA.y = psk1->Position.y + NSplineKnotTangentCoordLUT[code_ty] * NABS(world.y);
		psk1->TA.z = psk1->Position.z + NSplineKnotTangentCoordLUT[code_tz] * NABS(world.z);
		break;

	default:
		NVec3Set(&psk1->TA, 0, 0, 0);
		break;
	}
}

// WARNING !!! Take care of tangents_code !!!
// Here we have 2 splineknots and this function will set Tangent B of Knot 0 and Tangent A of Knot 1
// So tangents_code needs to contain "KNOT-1 TANGENT A code" and "KNOT-0 TANGENT B code"
// if you have to deal with 2 complete 32 bits packs, do like this:
//	(Nu32_Knot1Tangentscode & 0xFFFF0000 )|(Nu32_Knot0Tangentscode & 0x0000FFFF )
//	So, like this the new tangents code generated will contain the right data.
//	or use predefined MACRO (see NSpline.h) to perform this for you:
//		NSKTANGENTSPACK( NSKTANGENTA_EXTRACT(Nu32_Knot1Tangentscode),NSKTANGENTB_EXTRACT(Nu32_Knot0Tangentscode) )
void NSetBezierTangents(NSPLINEKNOT *psk0, NSPLINEKNOT *psk1, const Nu32 tangents_code)
{
	_extract_splineknot1_TA(psk0, psk1, tangents_code);
	_extract_splineknot0_TB(psk0, psk1, tangents_code);
}

void NSetAllSplineKnotTangents(NSPLINE *pspline, const Nu32 tangents_code)
{
	NSPLINEKNOT *psk0, *psk1;
	Nu32 i;

	// Initialize SplineKnots Tangent A
	psk0 = psk1 = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
	for (i = 0; i < NGetSplineKnotsNumber(pspline); i++, psk0 = psk1, psk1++)
	{
		_extract_splineknot1_TA(psk0, psk1, tangents_code);
	}

	// Initialize SplineKnots Tangent B
	psk0 = psk1 = (NSPLINEKNOT *)NGetLastArrayPtr(&pspline->KnotArray);
	for (i = 0; i < NGetSplineKnotsNumber(pspline); i++, psk1 = psk0, psk0--)
	{
		_extract_splineknot0_TB(psk0, psk1, tangents_code);
	}
}
// ------------------------------------------------------------------------------------------
// NSetupSpline
// ------------------------------------------------------------------------------------------
// Description :
//		Here you have first to initialize a list of SPLINEPOINTDESC
//		this function will use the description to create the spline and all its SplinePoints.
//
//		To right initialize the NSPLINEKNOTDESC list do like this:
//
//		NSPLINEKNOTDESC	description[] = {
//												{0.0f,	17.0f,	10.0f,	FLAG_SPLINEPOINTDESC_TANG_X,	FLAG_SPLINEPOINTDESC_TANG_X|FLAG_SPLINEPOINTDESC_TANG_Y},
//												{0.1f,	1.0f,	0.0f,	FLAG_SPLINEPOINTDESC_TANG_X,	FLAG_SPLINEPOINTDESC_TANG_X|FLAG_SPLINEPOINTDESC_TANG_Y},
//											};
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
NSPLINE *NSetupSpline(NSPLINE *pspline, const Nu32 accuracy, const Nu32 splinepoint_nb, const NSPLINEKNOTDESC *pdesclist)
{
	NSPLINEKNOTDESC *pdesc;
	NSPLINEKNOT *psp0, *psp1;
	Nu32 i;

	// Accuracy: (has to be >= 1)
	NErrorIf(accuracy < 1, NERROR_SPLINE_WRONG_ACCURACY);

	pspline->Flags = 0;
	pspline->BezierAccuracy = accuracy;
	// 	if(pspline->BezierAccuracy < 1)
	// 		pspline->BezierAccuracy = 1;

	NSetupArray(&pspline->KnotArray, splinepoint_nb, sizeof(NSPLINEKNOT));

	// Stop and return.
	// if splinepoint_nb <>0 but pdesclist is NULL, then, 'SplinePointlist' will have a capacity of 'splinepoint_nb' splinepoint
	// but without any setup of them.
	// if splinepoint_nb == 0, then there is no memory allocation for spline points, and, even if 'SplinePointlist' <> NULL there is no
	// "splinepoint" setting up
	if (!splinepoint_nb || !pdesclist)
		return pspline;

	// ===============================================================
	// Initializations according to the incoming NSPLINEKNOTDESC list
	// ===============================================================
	NResizeArray(&pspline->KnotArray, splinepoint_nb, NULL, NULL);
	psp0 = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
	pdesc = (NSPLINEKNOTDESC *)pdesclist; // pdesclist is 'const' and its normal because we are not going to
										  // modify any pdesclist content ...
										  // but we need to recast it as (NSPLINEKNOTDESC*) to read all the pdesclist element
										  // with this pdesc incremental ptr...

	// Initialize SplinePoints position ans SplinePoints Flags
	for (i = 0; i < splinepoint_nb; i++, psp0++, pdesc++)
	{
		NVec3Set(&psp0->Position, pdesc->x, pdesc->y, pdesc->z);
		psp0->Flags = 0;
	}

	// Initialize SplinePoints Tangent A
	psp0 = psp1 = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
	pdesc = (NSPLINEKNOTDESC *)pdesclist; // pdesclist is 'const' and its normal because we are not going to
										  // modify any pdesclist content ...
										  // but we need to recast it as (NSPLINEKNOTDESC*) to read all the pdesclist element
										  // with this pdesc incremental ptr...
	for (i = 0; i < splinepoint_nb; i++, psp0 = psp1, psp1++, pdesc++)
	{
		_extract_splineknot1_TA(psp0, psp1, pdesc->TBuildCode); // Here it is TBuildCode of psp1
	}

	// Initialize SplinePoints Tangent B
	psp0 = psp1 = (NSPLINEKNOT *)NGetLastArrayPtr(&pspline->KnotArray);
	pdesc = (NSPLINEKNOTDESC *)&pdesclist[splinepoint_nb - 1]; // pdesclist is 'const' and its normal because we are not going to
															   // modify any pdesclist content ...
															   // but we need to recast it as (NSPLINEKNOTDESC*) to read all the pdesclist element
															   // with this pdesc incremental ptr...
	for (i = 0; i < splinepoint_nb; i++, psp1 = psp0, psp0--, pdesc--)
	{
		_extract_splineknot0_TB(psp0, psp1, pdesc->TBuildCode); // Here it is TBuildCode of psp0
	}
	/*
	// ------------------------------------------------------------------------------
	// TEST
		NSPLINEKNOT *psk,*psk_prev,*psk_next;
		pdesc = pdesclist;
		psk = (NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);
		psk_prev = NULL;
		psk_next = psk+1;
		for(i=1;i<splinepoint_nb;i++,psk_prev=psk, psk=psk_next,psk_next++ ,pdesc++)
		{
			_extract_splineknot_TATB(psk_prev,psk,psk_next,pdesc->TBuildCode);
		}
	// ------------------------------------------------------------------------------
	*/
	return pspline;
}

NSPLINE *NCreateSpline(Nu32 accuracy, const Nu32 splinepoint_nb, const NSPLINEKNOTDESC *pdesclist)
{
	return NSetupSpline(NEW(NSPLINE), accuracy, splinepoint_nb, pdesclist);
}

void NClearSpline(NSPLINE *pspline)
{
	pspline->Flags = 0;
	pspline->BezierAccuracy = 0;
	NClearArray(&pspline->KnotArray, NULL);
}
void NDeleteSpline(NSPLINE *pspline)
{
	NClearSpline(pspline);
	Nfree(pspline);
}

void NSplinePointD3f32(NVEC3 *R, const Nf32 extended_factor, const NSPLINE *pspline)
{
	NErrorIf(NGetSplineKnotsNumber(pspline) < 2, NERROR_WRONG_VALUE);
	NErrorIf(extended_factor > (Nf32)(pspline->KnotArray.Size - 1), NERROR_VALUE_OUTOFRANGE);

	NSPLINEKNOT *pfirst = NGetFirstSplineKnot(pspline);
	Nu32 id0 = (Nu32)extended_factor;
	Nf32 t = extended_factor - (Nf32)(id0);
	NCasteljauPoint3f32(R, t, pfirst + id0, pfirst + id0 + 1);
	// Note:	There is an ERROR here but this ERROR doesn't affect the result actually...
	//			If extended_factor == (Nf32)(pspline->KnotArray.Size-1)
	//			Then, t will be 0, which is perfectly fine and absolutely not an error.
	//			BUT... NCasteljauPoint3f32 parameters are wrongs ... the last one actually.
	//			pfirst+id0 will be target the last valid SplineKnot
	//			... and pfirst+id0+1 will be OUT OF RANGE and target ... nothing relevant.
	//			So, calculation are going to be made with an absolutely WRONG second Spline Knot.
	//			BUT !! BUT !! ... t=0 SO, at the end of the day, the result of the LERP calculations
	//			will be ABSOLUTELY RIGHT because 0 multiply by a VALUE even if this value is WRONG will be equal to 0.
	//			And the final result will be equal to the last valid SplineKnot Position.
}

void NCopySpline(NSPLINE *pdst, const NSPLINE *psrc)
{
	pdst->Flags = psrc->Flags;
	pdst->BezierAccuracy = psrc->BezierAccuracy;
	NCopyArray(&pdst->KnotArray, &psrc->KnotArray);
}

Nf32 NCasteljauLength(const NSPLINEKNOT *A, const NSPLINEKNOT *B, const Nu32 accuracy)
{
	Nf32 length;
	Nu32 j;
	NVEC3 u0, u1, v;

	length = 0.0f;
	u0 = A->Position;
	for (j = 1; j <= accuracy; j++)
	{
		NCasteljauPoint3f32(&u1, (Nf32)j / (Nf32)accuracy, A, B);
		NVec3Sub(&v, &u1, &u0);
		length += NVec3Length(&v);

		u0 = u1;
	}
	return length;
}

Nf32 NBezierLength(const NSPLINEKNOT *A, const NSPLINEKNOT *B, const Nu32 accuracy)
{
	Nf32 length;
	Nu32 j;
	NVEC3 u0, u1, v;

	length = 0.0f;
	u0 = A->Position;
	for (j = 1; j <= accuracy; j++)
	{
		NBezierPoint3f32(&u1, (Nf32)j / (Nf32)accuracy, A, B);
		NVec3Sub(&v, &u1, &u0);
		length += NVec3Length(&v);

		u0 = u1;
	}
	return length;
}

Nf32 NSplineLength(const NSPLINE *pspline)
{
	Nf32 length;
	Nu32 i;
	NSPLINEKNOT *A, *B;

	if (!pspline->KnotArray.Size)
		return 0.0f;

	// There are 2 Splinepoints at minimum
	length = 0.0f;
	A = (NSPLINEKNOT *)pspline->KnotArray.pFirst;
	i = 0;
	do
	{
		B = A + 1;
		length += NBezierLength(A, B, pspline->BezierAccuracy);
		i++;
		A++;
	} while (i < pspline->KnotArray.Size);

	return length;
}
/*
NARRAY* NSplineLengthLUT(NARRAY *plengthlut, const NSPLINE *pspline)
{
	NErrorIf(!plengthlut, NERROR_NULL_POINTER);
	NErrorIf(!pspline, NERROR_NULL_POINTER);
	NErrorIf(pspline->KnotArray.Size < 2 , NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	NErrorIf(plengthlut->ElementSize != sizeof(Nf32), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	Nf32			length;
	Nf32			*plut;
	Nu32			i,j;
	NVEC3		u0,u1,v;
	NSPLINEKNOT		*A,*B;

	NResizeArray( plengthlut,1 + (pspline->BezierAccuracy*pspline->KnotArray.Size - 1), NULL, NULL );

	plut	= (Nf32*)plengthlut->pFirst;
	A		= (NSPLINEKNOT *)pspline->KnotArray.pFirst;
	B		= A + 1;

	length	= 0.0f;
	*plut	= 0.0f;
	plut++;

	for( i=1; i < pspline->KnotArray.Size; i++, A = B, B++ )
	{
		u0 = A->Position;

		for(j=1;j<=pspline->BezierAccuracy;j++,plut++)
		{
			NErrorIf( NIsValidArrayElementPtr(plengthlut,(NBYTE*)plut), NERROR_ARRAY_PTR_BEYOND_LIMITS );

			NCasteljauPoint3f32(&u1,(Nf32)j/(Nf32)pspline->BezierAccuracy,A,B);
			NVec3Sub(&v,&u1,&u0);
			length+=NVec3Length(&v);

			*plut += length;

			u0 = u1;
		}
	}

	return plengthlut;
}
*/
void NScaleSplineKnot(NSPLINEKNOT *psp, const NVEC3 *pscale, const NVEC3 *porigin)
{
	if (porigin)
	{
		psp->Position.x = (psp->Position.x - porigin->x) * pscale->x + porigin->x;
		psp->Position.y = (psp->Position.y - porigin->y) * pscale->y + porigin->y;
		psp->Position.z = (psp->Position.z - porigin->z) * pscale->z + porigin->z;

		psp->TA.x = (psp->TA.x - porigin->x) * pscale->x + porigin->x;
		psp->TA.y = (psp->TA.y - porigin->y) * pscale->y + porigin->y;
		psp->TA.z = (psp->TA.z - porigin->z) * pscale->z + porigin->z;

		psp->TB.x = (psp->TB.x - porigin->x) * pscale->x + porigin->x;
		psp->TB.y = (psp->TB.y - porigin->y) * pscale->y + porigin->y;
		psp->TB.z = (psp->TB.z - porigin->z) * pscale->z + porigin->z;
	}
	else
	{
		psp->Position.x *= pscale->x;
		psp->Position.y *= pscale->y;
		psp->Position.z *= pscale->z;

		psp->TA.x *= pscale->x;
		psp->TA.y *= pscale->y;
		psp->TA.z *= pscale->z;

		psp->TB.x *= pscale->x;
		psp->TB.y *= pscale->y;
		psp->TB.z *= pscale->z;
	}
}
void NScaleSpline(NSPLINE *ps, const NVEC3 *pscale, const NVEC3 *porigin)
{
	NSPLINEKNOT *psp = (NSPLINEKNOT *)ps->KnotArray.pFirst;
	if (porigin)
	{
		for (Nu32 i = 0; i < ps->KnotArray.Size; i++, psp++)
		{
			psp->Position.x = (psp->Position.x - porigin->x) * pscale->x + porigin->x;
			psp->Position.y = (psp->Position.y - porigin->y) * pscale->y + porigin->y;
			psp->Position.z = (psp->Position.z - porigin->z) * pscale->z + porigin->z;

			psp->TA.x = (psp->TA.x - porigin->x) * pscale->x + porigin->x;
			psp->TA.y = (psp->TA.y - porigin->y) * pscale->y + porigin->y;
			psp->TA.z = (psp->TA.z - porigin->z) * pscale->z + porigin->z;

			psp->TB.x = (psp->TB.x - porigin->x) * pscale->x + porigin->x;
			psp->TB.y = (psp->TB.y - porigin->y) * pscale->y + porigin->y;
			psp->TB.z = (psp->TB.z - porigin->z) * pscale->z + porigin->z;
		}
	}
	else
	{
		for (Nu32 i = 0; i < ps->KnotArray.Size; i++, psp++)
		{
			psp->Position.x *= pscale->x;
			psp->Position.y *= pscale->y;
			psp->Position.z *= pscale->z;

			psp->TA.x *= pscale->x;
			psp->TA.y *= pscale->y;
			psp->TA.z *= pscale->z;

			psp->TB.x *= pscale->x;
			psp->TB.y *= pscale->y;
			psp->TB.z *= pscale->z;
		}
	}
}
