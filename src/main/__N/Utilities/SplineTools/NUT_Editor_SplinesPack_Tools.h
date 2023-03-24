#ifdef _NEDITOR

#ifndef __NUT_EDITOR_SPLINESPACKTOOLS_H
#define __NUT_EDITOR_SPLINESPACKTOOLS_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NUT_Editor_SplinesPack.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../../N/Event/NEvent.h"
#include "../../../N/Core/NTime.h"
#include "../../../N/UI/NUI.h"
#include "../../../N/NEditor.h"
#include "../../../N/NSpline.h"
#include "../../../N/NUsualCoordinates.h"
#include "../Draw/NUT_Draw.h"
//#include "../../Snks_GameData.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE					3.0f	//Used to test if cursor is close to a spline knot or its tangents.
#define NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE_SQUARED			NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE*NUT_SPACK_SPLINEKNOT_DETECTION_DISTANCE
#define NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_DISTANCERATIO			0.125f
#define NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_COSANGLE				0.70710678118654752440084436210485f//0.866025f // = cos(30°)
//#define NUT_SPACK_SPLINEKNOT_TANGENT_SNAP_DISTANCEMAX			15.0f

// User defined SPLINE FLAGS
// -------------------------
// for each NSPLINE, BIT_8 to BIT_31 are user available.
// We are going to use them to specify some editing properties
#define FLAG_SPLINE_EDITOR_START_PERMANENT						BIT_8
#define FLAG_SPLINE_EDITOR_END_PERMANENT						BIT_9
#define FLAG_SPLINE_EDITOR_UNEDITABLE_TANGENT					BIT_10

typedef enum
{
	_START_KNOT=0,			// -->represents the first Knot of a spline
	_END_KNOT,				// -->represents the last Knot of a spline
	_INBETWEEN_KNOT,		// -->represents a spline knot which is neither start knot or neither end knot. 
	_KNOT					// -->represents a spline knot without any precision ( so ALL of them or One among all of them)
}NUT_SPACK_SPLINEPOINT_ENUM;

/*
typedef enum
{
	_X=0,
	_Y,
	_Z
}SNKS_SPC_V3COORD_ENUM;
*/

typedef enum
{
	_GREATER=0,
	_SMALLER,
	_DIFFERENT
}NUT_SPACK_TEST_ENUM;
typedef enum
{
	_THEN_OpA_EQUAL_OpB=0,
	_THEN_OpB_EQUAL_OpA
}NUT_SPACK_THEN_ENUM;

#define NUT_SPACK_INTERNAL_VALUE	NU32_MAX

#define _A							_START_KNOT		// =0 --> _A represents the First internal value
#define _B							_END_KNOT		// =1 --> _B represents the First internal value
#define _C							_INBETWEEN_KNOT	// =2 --> _C represents the First internal value
#define _VALUE						_KNOT			// =3 --> _VALUE represents "ALL" or "ONE VALUE AMONG THE 3 VALUES".  
#define NUT_SPACK_INTERNAL_VALUE_NB _C+1			// =4

typedef struct
{
	Nu32	OperandA_SplineID;
	Nu32	OperandB_SplineID;
	Nu8		OperandB_SplinePointID;
	Nu8		OperandA_SplinePointID;
	Nu8		OperandA_V3_Coord;
	Nu8		OperandB_V3_Coord;
	Nu8		Test;
}NUT_SPACK_CONSTRAINT;

typedef enum
{
	_2D_XAXIS=0,
	_2D_YAXIS,
	_2D_ZAXIS,
	_3D_XAXIS,
	_3D_YAXIS,
	_3D_ZAXIS
}NUT_SPACK_SYM_AXIS_ENUM;


typedef struct
{
	Nu32	OperandA_SplineID;
	Nu32	OperandB_SplineID;
	Nu8		Symmetry_Axis;
}NUT_SPACK_SYMMETRY;

#define DEFAULT_SPACK_NEW_SPLINE_ACCURACY				1000		
#define DEFAULT_NUT_SPACK_DRAW_SPLINE_COLOR				NCOLOR_PRESET3F_GREEN_JADE	// Value used by default at setup
#define DEFAULT_NUT_SPACK_SELECTION_COLOR				NCOLOR_PRESET3F_WHITE		// Value used by default at setup
#define DEFAULT_NUT_SPACK_DRAW_SPLINEKNOT_SIZE			2.0f						// Value used by default at setup
#define DEFAULT_NUT_SPACK_DRAW_XTRA_THRESHOLD			NF32_EPSILON_VECTOR_LENGTH	// Value used by default at setup ( High precision )

#define NUT_SPACK_INITIAL_CONSTRAINTSCAPACITY			8							// for each of the 2 arrays
#define NUT_SPACK_INITIAL_SYMMETRIECAPACITY				2							// for the symmetries array

#define FLAG_SPACK_SYMMETRY_ON							BIT_0	// Symmetry System activated (or not)
#define FLAG_SPACK_CONSTRAINT_ON						BIT_1	// constraints System activated (or not)
#define FLAG_SPACK_TANGENTSNAP_LENGTH_ON				BIT_2	// Tangent Snap: moving tangent snap its length onto the opposite tangent length
#define FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON				BIT_3	// Tangent Snap: moving tangent align itself to opposite tangent

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + CALLBACKS
// + 
// +	Callbacks take a very important place in SplinesPack Tool. They allow user to "map" or "branch" 
// +	it's own process deeply inside each SplinesPack tool.
// +	General principle is pretty easy to understand. They are 2 callbacks for each actions:
// +		PRE-Action callback			
// +		POST-Action callback
// +	In that way user knows intuitively when Engine calls the callbacks. PRE is 'before' and POST is 'after'.
// +	So the PRE-Move Callback is called just before performing move action and POST-Move callback just after ...
// +	
// +	All the callbacks are working like this excepted one: 'SPLINESPACK_CONSTRAINTS_INTERNVALUES_CALLBACK'
// +	There are no PRE or POST for this one. Engine call it when a constraint needs an internal value to work with.
// +	Those internal values are InternalSPlineKnots position actually. And it's possible for user to setup them by calling one of the 
// +	functions: 'Snks_SetSplinesPackValue???'
// +	..OR update them trough this callback just before Engine needs them.
// +
// + DETAILS:
// + 
// + SPLINESPACK_CONSTRAINTS_INTERNVALUES_CALLBACK
// +		pvA,pvB,pvC				Are pointers to Internal spline knots positions known as "internal values". User may update all of them trough this call back.
// +		psplineA,pspA,spA_Ref	Define the spline knot concerned by the constraint application. This this the one that will be updated against constraints. (spA_ref are _START_KNOT, _INBETWEEN_KNOT, END_KNOT or _KNOT )	
// +
// + SPLINESPACK_CONSTRAINTS_PRE_APPLY_CALLBACK & SPLINESPACK_CONSTRAINTS_POST_APPLY_CALLBACK
// +		pspline_dst,psp_dst,dst_V3_Coord	Define the spline knot Coordinates that will be tested/updated.
// +		pspline_src,psp_src,src_V3_Coord	Define the spline knot Coordinates that will be used to test against. ( it could be an internal value, in that case "pspline_src" is NULL )
// +
// + SPLINESPACK_SYMMETRY_PRE_APPLY_CALLBACK & SPLINESPACK_SYMMETRY_POST_APPLY_CALLBACK
// +							 pspline_dst	Define the spline that will be updated.
// +							 pspline_src	Define the spline source spline.
// +							    sym_axis	The symmetry axis to make 'pspline_dst' from 'pspline_src'
// +
// + SNKS_SPS_PRE_ADD_CALLBACK
// +					   pspline,pspA,pspB	Define the 2 Spline Knots used to create the new one in between.
// +							 spnew_ratio	the LERP ratio used between the 2 spline knots pspA and pspB to obtain the new spline knot position
// +
// + SNKS_SPS_POST_ADD_CALLBACK
// +					   pspline,pspA,pspB	Define the 2 Spline Knots used to create the new one in between.
// +							 spnew_ratio	the LERP ratio used between the 2 spline knots pspA and pspB to obtain the new spline knot position
// +					      pspline,pspnew	Define the new spline knot
// +
// + SNKS_SPS_PRE_SUPR_CALLBACK
// +						  pspline,pspA		Define the spline knot that will be deleted
// + SNKS_SPS_POST_SUPR_CALLBACK
// +						  pspline			The spline after knot delete
// +
// + SNKS_SPS_PRE_MOVE_CALLBACK & SNKS_SPS_POST_MOVE_CALLBACK
// +				 pspline,psp,knot_infos		Define the spline knot concerned by the MOVE. 
// +											knot_infos, is made with NMAKELONG(SplineKnot Element, SplineKnot Type);
// +											with:
// +												SplineKnot Element	->	SPLINESPACK_SPLINEKNOT_POSITION		
// +																		SPLINESPACK_SPLINEKNOT_TA_SEGMENT		
// +																		SPLINESPACK_SPLINEKNOT_TA_ENDHANDLE			
// +																		SPLINESPACK_SPLINEKNOT_TB_SEGMENT			
// +																		SPLINESPACK_SPLINEKNOT_TB_ENDHANDLE			
// +																		
// +												SplineKnot Type		->	_START_KNOT
// +																		_END_KNOT
// +																		_INBETWEEN_KNOT
// +																		(_KNOT is never used here because TOOL MOVE knows the type of the node it moves )
// +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef void (*NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK)(NVEC3 *pvA, NVEC3* pvB, NVEC3* pvC, const NSPLINE *psplineA, const NSPLINEKNOT *pspA, const NUT_SPACK_SPLINEPOINT_ENUM spA_ref );

typedef void (*NUT_SPACK_CONSTRAINTS_PRE_APPLY_CALLBACK)( const NSPLINE *pspline_dst, const NSPLINEKNOT *psp_dst, const Nu8 dst_V3_Coord, const NSPLINE *pspline_src, const NSPLINEKNOT *psp_src, const Nu8 src_V3_Coord );
typedef void (*NUT_SPACK_CONSTRAINTS_POST_APPLY_CALLBACK)( const NSPLINE *pspline_dst, const NSPLINEKNOT *psp_dst, const Nu8 dst_V3_Coord, const NSPLINE *pspline_src, const NSPLINEKNOT *psp_src, const Nu8 src_V3_Coord  );

typedef void (*NUT_SPACK_SYMMETRY_PRE_APPLY_CALLBACK)(const NSPLINE *pspline_dst,const NSPLINE *pspline_src,const NUT_SPACK_SYM_AXIS_ENUM sym_axis);
typedef void (*NUT_SPACK_SYMMETRY_POST_APPLY_CALLBACK)(const NSPLINE *pspline_dst,const NSPLINE *pspline_src,const NUT_SPACK_SYM_AXIS_ENUM sym_axis);

typedef void (*NUT_SPACK_PRE_ADD_CALLBACK)(const NSPLINE *pspline, const NSPLINEKNOT *pspA, const NSPLINEKNOT *pspB, const Nf32 spnew_ratio);
typedef void (*NUT_SPACK_POST_ADD_CALLBACK)(const NSPLINE *pspline, const NSPLINEKNOT *pspA, const NSPLINEKNOT *pspB, const Nf32 spnew_ratio, NSPLINEKNOT *pspnew);

typedef void (*NUT_SPACK_PRE_SUPR_CALLBACK)(const NSPLINE *pspline, const NSPLINEKNOT *psp, const Nu32 index);
typedef void (*NUT_SPACK_POST_SUPR_CALLBACK)(const NSPLINE *pspline);

typedef void (*NUT_SPACK_PRE_MOVE_CALLBACK)(const NSPLINE *pspline, const NSPLINEKNOT *psp,const Nu32 knot_infos);
typedef void (*NUT_SPACK_POST_MOVE_CALLBACK)(const NSPLINE *pspline, const NSPLINEKNOT *psp,const Nu32 knot_infos);

typedef void(*NUT_SPACK_PRE_CREATE_CALLBACK)(const NVEC3 *pcreatepos);
typedef void(*NUT_SPACK_POST_CREATE_CALLBACK)(const NSPLINE *pspline);
typedef void(*NUT_SPACK_CREATE_CALLBACK)(const NVEC3 *pcreatepos, NSPLINE **ppnewspline, NSPLINEKNOT **ppknot_to_move);

typedef void(*NUT_SPACK_PRE_DELETE_CALLBACK)(const NSPLINE *pspline);
typedef void(*NUT_SPACK_POST_DELETE_CALLBACK)();
typedef void(*NUT_SPACK_DELETE_CALLBACK)(NSPLINE *pspline);

typedef struct  
{
	Nu32						FlagsSplinesPack;
	NCOLOR						SelectionColor;
	Nu32						NewSplineAccuracy;
	NARRAY						ArrayOfSplinesPtr;
	NUT_DRAW_SPLINE				DrawSpline_Param;		// Parameters to draw the spline passed to the function "NUT_Draw_Spline" for each spline.

	// Constraints
	NARRAY						Constraints_OpA_Equal_OpB;
	NARRAY						Constraints_OpB_Equal_OpA;
	NSPLINEKNOT					InternalSplinePoint[NUT_SPACK_INTERNAL_VALUE_NB];
	NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK		InternalValues_CallBack;
	NUT_SPACK_CONSTRAINTS_PRE_APPLY_CALLBACK		PreApplyConstraints_CallBack;
	NUT_SPACK_CONSTRAINTS_POST_APPLY_CALLBACK		PostApplyConstraints_CallBack;

	// Symmetry
	NARRAY						Symmetries;
	NUT_SPACK_SYMMETRY_PRE_APPLY_CALLBACK		PreApplySymmetry_CallBack;
	NUT_SPACK_SYMMETRY_POST_APPLY_CALLBACK		PostApplySymmetry_CallBack;
	
	// CallBacks
	// Add
	NUT_SPACK_PRE_ADD_CALLBACK		PreAdd_CallBack;
	NUT_SPACK_POST_ADD_CALLBACK		PostAdd_CallBack;
	
	// Supr
	NUT_SPACK_PRE_SUPR_CALLBACK		PreSupr_CallBack;
	NUT_SPACK_POST_SUPR_CALLBACK	PostSupr_CallBack;
	
	// Move
	NUT_SPACK_PRE_MOVE_CALLBACK		PreMove_CallBack;
	NUT_SPACK_POST_MOVE_CALLBACK	PostMove_CallBack;
	
	// Create
	NUT_SPACK_PRE_CREATE_CALLBACK	PreCreate_CallBack;
	NUT_SPACK_POST_CREATE_CALLBACK	PostCreate_CallBack;
	NUT_SPACK_CREATE_CALLBACK		Create_CallBack;

	// Delete
	NUT_SPACK_PRE_DELETE_CALLBACK	PreDelete_CallBack;
	NUT_SPACK_POST_DELETE_CALLBACK	PostDelete_CallBack;
	NUT_SPACK_DELETE_CALLBACK		Delete_CallBack;

}NUT_SPACK_TOOLDATA;



// Tools
// -------------------------------------
// Spline Tools
#define			NUT_SPACK_SPLINEKNOT_POSITION		1
#define			NUT_SPACK_SPLINEKNOT_TA_SEGMENT		2
#define			NUT_SPACK_SPLINEKNOT_TA_ENDHANDLE	3

#define			NUT_SPACK_SPLINEKNOT_TB_SEGMENT		4
#define			NUT_SPACK_SPLINEKNOT_TB_ENDHANDLE	5



NUT_SPACK_TOOLDATA*		NUT_SetupSplinesPackToolData(NUT_SPACK_TOOLDATA* ps, const Nu32 splinesptr_array_capacity );
NUT_SPACK_TOOLDATA*		NUT_CreateSplinesPackToolData(const Nu32 splinesptr_array_capacity );
void					NUT_ClearSplinesPackToolData(NUT_SPACK_TOOLDATA *ps);
void					NUT_DeleteSplinesPackToolData(NUT_SPACK_TOOLDATA *ps);


//inline	void			NUT_SetSplinesPackToolData_DrawColor(NUT_SPACK_TOOLDATA *ps, const NCOLOR *pcol) { ps->DrawSpline_Param.Color = *pcol; }
//inline	void			NUT_SetSplinesPackToolData_DrawKnotSize(NUT_SPACK_TOOLDATA *ps, const Nf32 fsize) { ps->DrawSpline_Param.KnotSize = fsize; }
//inline	void			NUT_SetSplinesPackToolData_DrawXtraThreshold(NUT_SPACK_TOOLDATA *ps, const Nf32 xthres) { ps->DrawSpline_Param.Xtra_Threshold; }


inline NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK	NUT_SetSplinesPackConstraints_InternalValuesCallBack( NUT_SPACK_TOOLDATA* ps, NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK internalvalue_callback){NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK old = ps->InternalValues_CallBack; ps->InternalValues_CallBack = internalvalue_callback; return old; }
inline NUT_SPACK_CONSTRAINTS_INTERNVALUES_CALLBACK	NUT_GetSplinesPackConstraints_InternalValuesCallBack( NUT_SPACK_TOOLDATA* ps){return ps->InternalValues_CallBack;}

inline void NUT_SetSplinesPackToolData_MoveCallBacks(NUT_SPACK_TOOLDATA *ps, const NUT_SPACK_PRE_MOVE_CALLBACK premove, const NUT_SPACK_POST_MOVE_CALLBACK postmove ){ps->PreMove_CallBack = premove;ps->PostMove_CallBack = postmove;}
inline void NUT_SetSplinesPackToolData_AddCallBacks(NUT_SPACK_TOOLDATA *ps, const NUT_SPACK_PRE_ADD_CALLBACK preadd, const NUT_SPACK_POST_ADD_CALLBACK postadd ){ps->PreAdd_CallBack = preadd;ps->PostAdd_CallBack = postadd;}
inline void NUT_SetSplinesPackToolData_SuprCallBacks(NUT_SPACK_TOOLDATA *ps, const NUT_SPACK_PRE_SUPR_CALLBACK presupr, const NUT_SPACK_POST_SUPR_CALLBACK postsupr ){ps->PreSupr_CallBack = presupr;ps->PostSupr_CallBack = postsupr;}
inline void NUT_SetSplinesPackToolData_CreateSplineCallBacks(NUT_SPACK_TOOLDATA *ps, const NUT_SPACK_PRE_CREATE_CALLBACK precrea, const NUT_SPACK_POST_CREATE_CALLBACK postcrea, const NUT_SPACK_CREATE_CALLBACK crea) { ps->PreCreate_CallBack = precrea; ps->PostCreate_CallBack = postcrea; ps->Create_CallBack = crea;}
inline void NUT_SetSplinesPackToolData_DeleteSplineCallBacks(NUT_SPACK_TOOLDATA *ps, const NUT_SPACK_PRE_DELETE_CALLBACK predel, const NUT_SPACK_POST_DELETE_CALLBACK postdel, const NUT_SPACK_DELETE_CALLBACK del) { ps->PreDelete_CallBack = predel; ps->PostDelete_CallBack = postdel; ps->Delete_CallBack = del;}

inline void						NUT_SetSplinesPackToolData_NewSplineAccuracy(NUT_SPACK_TOOLDATA *ps, const Nu32 accuracy) { ps->NewSplineAccuracy = accuracy; }
	
inline Nu32						NUT_SplinesPackToolData_BindSpline(NUT_SPACK_TOOLDATA *ptd, const NSPLINE *ps) {NArrayPushBack(&ptd->ArrayOfSplinesPtr, (NBYTE*)&ps); return ptd->ArrayOfSplinesPtr.Size - 1;}
inline void						NUT_SplinesPackToolData_ForgetAllBoundSplines(NUT_SPACK_TOOLDATA *ptd ){NResizeArray(&ptd->ArrayOfSplinesPtr,0,NULL,NULL);}

Nbool							NUT_CreateSplinesPackConstraint(NUT_SPACK_TOOLDATA *pt, Nu32 Aspline_id,NUT_SPACK_SPLINEPOINT_ENUM Asplinepoint_ref,NV3COORD_ENUM Av3_coord_ref,NUT_SPACK_TEST_ENUM test_ref,Nu32 Bspline_id_or_value_tag,NUT_SPACK_SPLINEPOINT_ENUM Bsplinepoint_ref_or_value_ref,NV3COORD_ENUM Bv3_coord_ref,NUT_SPACK_THEN_ENUM then_ref);
inline void						NUT_EraseAllSplinesPackConstraints(NUT_SPACK_TOOLDATA *pt){NEraseArray(&pt->Constraints_OpA_Equal_OpB,NULL);NEraseArray(&pt->Constraints_OpB_Equal_OpA,NULL);}

void							NUT_CreateSplinesPackSymmetry(NUT_SPACK_TOOLDATA *pt, Nu32 Aspline_id,Nu32 Bspline_id,NUT_SPACK_SYM_AXIS_ENUM sym_axis);
inline void						NUT_EraseAllSplinesPackSymmetry(NUT_SPACK_TOOLDATA *pt){NEraseArray(&pt->Symmetries,NULL);}

inline void						NUT_ActivateSplinesPackConstraints(NUT_SPACK_TOOLDATA *pt){FLAG_ON(pt->FlagsSplinesPack,FLAG_SPACK_CONSTRAINT_ON);}
inline void						NUT_ActivateSplinesPackSymmetries(NUT_SPACK_TOOLDATA *pt){FLAG_ON(pt->FlagsSplinesPack,FLAG_SPACK_SYMMETRY_ON);}
inline void						NUT_ActivateSplinesPackTangentSnap_Alignment(NUT_SPACK_TOOLDATA *pt){FLAG_ON(pt->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON);}
inline void						NUT_ActivateSplinesPackTangentSnap_Length(NUT_SPACK_TOOLDATA *pt){FLAG_ON(pt->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_LENGTH_ON);}

inline void						NUT_UnactivateSplinesPackConstraints(NUT_SPACK_TOOLDATA *pt){FLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_CONSTRAINT_ON);}
inline void						NUT_UnactivateSplinesPackSymmetries(NUT_SPACK_TOOLDATA *pt){FLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_SYMMETRY_ON);}
inline void						NUT_UnactivateSplinesPackTangentSnap_Alignment(NUT_SPACK_TOOLDATA *pt){FLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_ALIGNMENT_ON);}
inline void						NUT_UnactivateSplinesPackTangentSnap_Length(NUT_SPACK_TOOLDATA *pt){FLAG_OFF(pt->FlagsSplinesPack,FLAG_SPACK_TANGENTSNAP_LENGTH_ON);}

inline Nbool					NUT_SetSplinesPackValuef(NUT_SPACK_TOOLDATA *pt,const NUT_SPACK_SPLINEPOINT_ENUM internal_value_id,const Nf32 x,const Nf32 y,const Nf32 z){if(internal_value_id<NUT_SPACK_INTERNAL_VALUE_NB){NVec3Set(&pt->InternalSplinePoint[internal_value_id].Position,x,y,z);return NTRUE;}else return NFALSE; }
inline Nbool					NUT_SetSplinesPackValueXf(NUT_SPACK_TOOLDATA *pt,const NUT_SPACK_SPLINEPOINT_ENUM internal_value_id,const Nf32 val){if(internal_value_id<NUT_SPACK_INTERNAL_VALUE_NB){pt->InternalSplinePoint[internal_value_id].Position.x = val;return NTRUE;}else return NFALSE; }
inline Nbool					NUT_SetSplinesPackValueYf(NUT_SPACK_TOOLDATA *pt,const NUT_SPACK_SPLINEPOINT_ENUM internal_value_id,const Nf32 val){if(internal_value_id<NUT_SPACK_INTERNAL_VALUE_NB){pt->InternalSplinePoint[internal_value_id].Position.y = val;return NTRUE;}else return NFALSE; }
inline Nbool					NUT_SetSplinesPackValueZf(NUT_SPACK_TOOLDATA *pt,const NUT_SPACK_SPLINEPOINT_ENUM internal_value_id,const Nf32 val){if(internal_value_id<NUT_SPACK_INTERNAL_VALUE_NB){pt->InternalSplinePoint[internal_value_id].Position.z = val;return NTRUE;}else return NFALSE; }

// SPLINES PACK TOOLS
// -----------------------------------------------------------------------------------------------------------------------------
NEDITOR_RESULT					NUT_SplinesPackTool_CreateSpline(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT					NUT_SplinesPackTool_DeleteSpline(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT					NUT_SplinesPackTool_MoveKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param);
NEDITOR_RESULT					NUT_SplinesPackTool_AddKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param);
NEDITOR_RESULT					NUT_SplinesPackTool_SuprKnot(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param);

void							NUT_SplinesPackTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);

// internal use
Nbool							NUT_ApplySplinesPackConstraints(NUT_SPACK_TOOLDATA *pt, NSPLINE *pspline, NSPLINEKNOT *psp, const NUT_SPACK_SPLINEPOINT_ENUM Asplinepoint_ref );
Nbool							NUT_ApplySplinesPackSymmetries(NUT_SPACK_TOOLDATA *pt, const NSPLINE *pmodified_spline);

Nbool							NUT_ApplySplinesPackTangentsSnap(NUT_SPACK_TOOLDATA *pdata, const Nu32 snapflags_toolrequest, NVEC3 *ptangent,const NVEC3 *ptargettangent,const NVEC3 *pknotposition );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_EDITOR_SPLINESPACKTOOLS_H 

#endif //_NEDITOR

  