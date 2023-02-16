#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../NCStandard.h"
#include "NUT_Editor_SplinesPack_Tools.h"

NUT_SPACK_TOOLDATA*	NUT_SetupSplinesPackToolData(NUT_SPACK_TOOLDATA* ps, const Nu32 splinesptr_array_capacity )
{
	memset(ps,0,sizeof(NUT_SPACK_TOOLDATA));
	NSetupArray(&ps->ArrayOfSplinesPtr,splinesptr_array_capacity,sizeof(NSPLINE*));
	NSetupArray(&ps->Constraints_OpA_Equal_OpB,NUT_SPACK_INITIAL_CONSTRAINTSCAPACITY,sizeof(NUT_SPACK_CONSTRAINT));
	NSetupArray(&ps->Constraints_OpB_Equal_OpA,NUT_SPACK_INITIAL_CONSTRAINTSCAPACITY,sizeof(NUT_SPACK_CONSTRAINT));
	NSetupArray(&ps->Symmetries,NUT_SPACK_INITIAL_SYMMETRIECAPACITY,sizeof(NUT_SPACK_SYMMETRY));

	ps->NewSplineAccuracy				= DEFAULT_SPACK_NEW_SPLINE_ACCURACY;
	// Default DrawSpline setup:
	NSetColorf(&ps->DrawSpline_Param.Color, DEFAULT_NUT_SPACK_DRAW_SPLINE_COLOR, 1);
	NSetColorf(&ps->SelectionColor, DEFAULT_NUT_SPACK_SELECTION_COLOR, 1);

	ps->DrawSpline_Param.KnotSize		= DEFAULT_NUT_SPACK_DRAW_SPLINEKNOT_SIZE;
	ps->DrawSpline_Param.Xtra_Threshold = DEFAULT_NUT_SPACK_DRAW_XTRA_THRESHOLD;
	return ps;
}

NUT_SPACK_TOOLDATA*	NUT_CreateSplinesPackToolData(const Nu32 splinesptr_array_capacity )
{
	return NUT_SetupSplinesPackToolData(NEW(NUT_SPACK_TOOLDATA),splinesptr_array_capacity );
}

void NUT_ClearSplinesPackToolData(NUT_SPACK_TOOLDATA *ps)
{
	NClearArray( &ps->ArrayOfSplinesPtr,NULL );
	NClearArray( &ps->Constraints_OpA_Equal_OpB,NULL );
	NClearArray( &ps->Constraints_OpB_Equal_OpA,NULL );
	NClearArray( &ps->Symmetries,NULL );
	Nmem0(ps,NUT_SPACK_TOOLDATA);
}

void NUT_DeleteSplinesPackToolData(NUT_SPACK_TOOLDATA *ps)
{
	NUT_ClearSplinesPackToolData(ps);
	Nfree(ps);
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR