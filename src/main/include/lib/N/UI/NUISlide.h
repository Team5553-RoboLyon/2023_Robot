#ifndef __NUI_SLIDE_H
#define __NUI_SLIDE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUISlide.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"
#include "../NSpline.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define		CONSTANT_NUI_SLIDE_EPSILON_SNAPPOINT_DISTANCE			0.0001f
#define		CONSTANT_NUI_SLIDE_EPSILON_SPEED						0.001f
#define		CONSTANT_NUI_SLIDE_KILL_TOUCH_LISTENING_SLIDING_DISTANCE			0.075f	// Maximum distance from the first touch. Beyond it, all the focused children of the Sliding layer loose the focus (if they have)
#define		CONSTANT_NUI_SLIDE_DRAGPOINT_SNAP_THRESHOLD				(0.005f*0.005f)
// Default Values
#define		DEFAULT_NUI_SLIDE_SWIPE_THRESHOLD						0.012f
#define		DEFAULT_NUI_SLIDE_SWIPE_IDIRECTION_2F					1.0f,0.0f
#define		DEFAULT_NUI_SLIDE_SWIPE_JUMP							1
//#define	DEFAULT_NUI_SLIDE_SWIPE_ASIDE_JUMP						Doesn't exist, because user has to define it, if he wants use it.
#define		DEFAULT_NUI_SLIDE_SWIPE_ONFIELD_SEEKING_ACCURACY		0.125f // distance to the seeking swipe ray (in norm screen unity, so here it represents 1/8 of screen height all around the swipe ray)

#define		DEFAULT_NUI_SLIDE_SNAP_CAPTURE_RADIUS					0.005f
#define		DEFAULT_NUI_SLIDE_VERLETDAMPING							0.9f				
#define		DEFAULT_NUI_SLIDE_VERLETFRICTION						1000.0f
#define		DEFAULT_NUI_SLIDE_DRAGDAMPING							0.92f
#define		DEFAULT_NUI_SLIDE_SNAP_SPRINGK							125.0f
#define		DEFAULT_NUI_SLIDE_DRAG_SPRINGK							125.0f
#define		DEFAULT_NUI_SLIDE_SAFE_SPRINGK							250.0f

#define		DEFAULT_NUI_SLIDE_SAFE_SPRINGFRICTION					0.0f
#define		DEFAULT_NUI_SLIDE_TOUCH_END_SPEED_MULTIPLIER			2.0f

#define		DEFAULT_NUI_SLIDE_PIN_CAPTURE_RADIUS					0.005f
#define		DEFAULT_NUI_SLIDE_PIN_SPEED_THRESHOLD					0.002f

//	NUI_SLIDE.Flags (starts at BIT_24)
#define		FLAG_NUI_SLIDE_SNAP_THRESHOLD							BIT_24
#define		FLAG_NUI_SLIDE_LIMITS_KILL_STATE_DRAG					BIT_25
#define		FLAG_NUI_SLIDE_LIMITS_REPULSE							BIT_26
#define		FLAG_NUI_SLIDE_KILL_CHILDREN_TOUCH_LISTENING					BIT_27
//#define		FLAG_NUI_SLIDE_xxx									BIT_28
#define		FLAG_NUI_SLIDE_TOUCH_END_SPEED_MULTIPLIER				BIT_29
#define		FLAG_NUI_SLIDE_DRAG_LOCK								BIT_30
#define		FLAG_NUI_SLIDE_REPULSE									BIT_31


//	NUI_SLIDE_DESC.Flags_Style (starts at BIT_24)
#define		FLAG_NUIDS_SLIDE_SWIPE									BIT_24 //|
#define		FLAG_NUIDS_SLIDE_SWIPE_BIDIRECTIONNAL					BIT_25 //|- To define which type of SWIPE Sliding layer is going to use.
#define		FLAG_NUIDS_SLIDE_SWIPE_ONFIELD_SEEKING					BIT_26 //|	These flags help Setup choosing the right Swipe_Handle.

#define		FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_THRESHOLD				BIT_27
#define		FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_IDIRECTION				BIT_28
#define		FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_JUMP					BIT_29
#define		FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_ASIDE_JUMP				BIT_30
#define		FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_ONFIELD_SEEKING_ACCURACY BIT_31

//	NUI_SLIDE_DESC.Flags_Core (starts at BIT_24)
#define		FLAG_NUIDC_SLIDE_SNAP_THRESHOLD							BIT_24
#define		FLAG_NUIDC_SLIDE_LIMITS_KILL_STATE_DRAG					BIT_25
#define		FLAG_NUIDC_SLIDE_LIMITS_REPULSE							BIT_26
#define		FLAG_NUIDC_SLIDE_USEPARAM_SNAPPOINT_ARRAY_CAPACITY		BIT_27
#define		FLAG_NUIDC_SLIDE_USEPARAM_PIN_ARRAY_CAPACITY			BIT_28
#define		FLAG_NUIDC_SLIDE_TOUCH_END_SPEED_MULTIPLIER				BIT_29
#define		FLAG_NUIDC_SLIDE_KILL_CHILD_TOUCH_FOCUS					BIT_30 // Equivalent to UI Slide Flag: FLAG_NUI_SLIDE_KILL_CHILDREN_TOUCH_LISTENING

// NUI_SLIDE_DESC.XFlags (starts at BIT_0, to BIT_31)
#define		FLAG_NUIX_SLIDE_USEPARAM_VERLETDAMPING					BIT_0
#define		FLAG_NUIX_SLIDE_USEPARAM_VERLETFRICTION					BIT_1
#define		FLAG_NUIX_SLIDE_USEPARAM_DRAGDAMPING					BIT_2
#define		FLAG_NUIX_SLIDE_USEPARAM_SNAP_CAPTURE_RADIUS			BIT_3
#define		FLAG_NUIX_SLIDE_USEPARAM_DRAG_SPRINGK					BIT_4
#define		FLAG_NUIX_SLIDE_USEPARAM_SNAP_SPRINGK					BIT_5
#define		FLAG_NUIX_SLIDE_USEPARAM_SAFE_SPRINGK					BIT_6
#define		FLAG_NUIX_SLIDE_USEPARAM_SAFE_SPRINGFRICTION			BIT_7
#define		FLAG_NUIX_SLIDE_USEPARAM_TOUCH_END_SPEED_MULTIPLIER		BIT_8


typedef struct	NUI_SLIDE		NUI_SLIDE;
typedef	Nu32	(*NUI_SLIDE_MOVE_UNDER_CONSTRAINT)(NUI_SLIDE* psl);
typedef	void	(*NUI_SLIDE_SWIPE)(NUI_SLIDE* psl);
typedef struct  
{
	NUI_SLIDE_MOVE_UNDER_CONSTRAINT	ApplyConstraint_Handle;	// |-COMMON PART OF ALL THE NSLIDINGLAYER_CONSTRAINT_xx
	Nf32							SafeSegmentRatio;
	NVEC3						SegmentPoint0;
	NVEC3						SegmentPoint1;
}NUI_SLIDE_CONSTRAINT_SEGMENT;

typedef struct  
{
	NUI_SLIDE_MOVE_UNDER_CONSTRAINT	ApplyConstraint_Handle;	// |-COMMON PART OF ALL THE NSLIDINGLAYER_CONSTRAINT_xx
	NVEC3						SafeAABBDistance;

	NAABB							*pAABB;					// The Constraint Sliding Layer Spline
}NUI_SLIDE_CONSTRAINT_AABB;

typedef struct  
{
	NUI_SLIDE_MOVE_UNDER_CONSTRAINT	ApplyConstraint_Handle;	// |-COMMON PART OF ALL THE NSLIDINGLAYER_CONSTRAINT_xx
	Nf32							SafeSplineRatioStart;
	Nf32							SafeSplineRatioEnd;
	NSPLINE							*pSpline;				// The Constraint Sliding Layer Spline
	Nf32							TCurrentXtdFactor;		// 'ExtendedLerpRatio'. 0<=TCurrent<=(Nf32)(NGetSplineKnotsNumber(pSpline)-1) used by 'NSplinePointD3f32'
	Nf32							TStep;					// + or - (1/SplineAccurracy) to parse spline segments and apply Sliding Layer Speed along the spline.
}NUI_SLIDE_CONSTRAINT_SPLINE;

typedef union  
{
	NUI_SLIDE_MOVE_UNDER_CONSTRAINT		ApplyConstraint_Handle;
	NUI_SLIDE_CONSTRAINT_SEGMENT		Segment;	
	NUI_SLIDE_CONSTRAINT_AABB			AABB;	
	NUI_SLIDE_CONSTRAINT_SPLINE			Spline;	
}NUI_SLIDE_CONSTRAINT;

typedef struct  
{
	NVEC3							Position;				// The SnapPoint 3D Position
}NUI_SLIDE_SNAPPOINT;

#define FLAG_NUI_SLIDE_PIN_USE_SPEED_THRESHOLD_DIRECTION	BIT_0
typedef struct  
{
	NVEC3							Position;				// The SnapPoint 3D Position
	NVEC3							SpeedThresholdDirection;
	Nf32								SquareSpeedThreshold;
	Nf32								SquareCaptureRadius;
	Nu32								Flags;
}NUI_SLIDE_PIN;

struct NUI_SLIDE
{
	NUI						UI;
	
	// Additionnal Picking Params
//	NVEC2				PickChildrenBBoxMargin; // (0 <= x <= 1 ) Size in % of x and y margins used to refuse children picking when touch position is out of these margins...
													// Like this N avoid picking Slide children that they are close to screen edge.
	// TOUCH
// 	NVEC2s16				InitialTouchPosition;
// 	NVEC2s16				TouchPosition;
	
	// TOUCH MOVE and children 
	NVEC2				NormTouchMoveAccumulator;

	// SWIPE
	NVEC2				NormTouchMove;	// SWIPE survey real touch move to decide what it has to do.
	NVEC2				SwipeI;			// To orient Swipe detection. This is the I Vector. J Vector will be calculated from this one if necessary.
												// By default, SwipeI is (1,0)
	Nf32					SwipeThreshold;	// DotProduct(TouchMove,SwipeI) has to be greater than "SwipeThreshold" to SWIPE
												// or DotProduct(TouchMove,SwipeJ) has to be greater than "SwipeThreshold" to SWIPE ASIDE (if its allowed of course)
	Nu16					SwipeJump;		// Index distance into the snap point array from current snap point to choose the next one in case of SWIPE. 
	Nu16					SwipeAsideJump; // Index distance into the snap point array from current snap point to choose the next one in case of SWIPE ASIDE. 
	Nf32					SwipeOnFieldSeekingAccuracy;	
	NUI_SLIDE_SWIPE			Swipe_Handle;

	// PHYSICS
	NVEC3				SlidePos;		
	NVEC3				OldSlidePos;
	NVEC3				DragPos;
	
	Nf32					SnapSpringK;
	Nf32					DragSpringK;

	Nf32					SafeSpringK;
	Nf32					SafeSpringFriction;

	NVEC3				SafeRepulseForce;

	NVEC3				ForceAccumulator;

	Nf32					VerletDamping;			// Damping value used by VERLET Integration Sliding system to damp on fly extracted speed.
	Nf32					VerletFriction;			// Sliding Friction. The only one force applied to the slider when it slide.
	
	Nf32					DragDamping;			// Damping value used to "kill" initial sliding speed if user still static between TOUCH_START and TOUCH_END for a while...
													// ...Initial sliding speed is the speed the slider has when user TOUCH_END the slider. Because there is no speed but old and current position
													// ...this DragDamping value changes old position and push it closer and closer to the current position, each UI. 
	Nf32					TouchEndSpeedMultiplier;
	// SNAP POINTS
	NUI_SLIDE_SNAPPOINT		*pCurrentSnapPoint;		// The current SnapPoint ... Active when Layer Slide 
	NARRAY					SnapPointArray;			// All The Snap Points of the Layer
	Nf32					SquareSnapCaptureRadius;// radius x radius, just because its faster to test !
	
	// PIN
	NARRAY					PinArray;				// All The Pin of the Layer
	
	// CONSTRAINT
	NUI_SLIDE_CONSTRAINT	Constraint;
	
	// Children listening Management
// 	NUI						*pListenUI;			// UI Tracked for Touch Event processing ( UISLide itself or one of it's child )
// 	NUI						*pTouchKillUI;		// Slide ask to kill the focus of this one ... ( used to manage the kill focus in the right way )
// TEMP DEBUG
// 	NSPLINE						dbg_spline;
// 	NVEC3					dbg_shift;
};

typedef struct
{
	NUI_DESC		UIDesc;

	Nu32			XFlags;					// Slide Extended Build Flags

	// PHYSICS
	Nf32			SnapSpringK;
	Nf32			SafeSpringFriction;

	Nf32			DragSpringK;
	Nf32			SafeSpringK;
	
	Nf32			VerletDamping;			// Damping value used by VERLET Integration Sliding system to damp on fly extracted speed to loose energy system.
	Nf32			VerletFriction;			// Sliding Friction. The only one force applied to the slider when it slide.
	Nf32			DragDamping;			// Damping value used to "kill" initial sliding speed if user still static between TOUCH_START and TOUCH_END for a while...
	Nf32			TouchEndSpeedMultiplier;
	// SNAP POINTS
	Nu32			SnapPointArrayCapacity;	//
	Nf32			SnapCaptureRadius;		// 
	// PIN
	Nu32			PinArrayCapacity;
	// SWIPE
	Nf32			SwipeThreshold;			// DotProduct(TouchMove,SwipeI) has to be greater than "SwipeThreshold" to SWIPE
	NVEC2		SwipeIDirection;		// To orient Swipe detection. This is the I Vector. J Vector will be calculated from this one if necessary.
	Nu16			SwipeJump;				// Index distance into the snap point array from current snap point to choose the next one in case of SWIPE. 
	Nu16			SwipeAsideJump;			// Index distance into the snap point array from current snap point to choose the next one in case of SWIPE ASIDE. 
	Nf32			SwipeOnFieldSeekingAccuracy;	
}NUI_SLIDE_DESC;

NUI_SLIDE*				NCreateUISlide(NUI *parent,NUI_SLIDE_DESC *pslayerdesc, const Nu32 user32);
NUI_SLIDE*				NSetupUISlide(NUI *parent, NUI_SLIDE* pslayer, NUI_SLIDE_DESC *pslayerdesc, const Nu32 user32 );
void					NClearUISlide(NUI *pui);
//void					NDeleteUISlide(void *pfr); !!! Doesn't Exist !!! Use 'NDeleteUI'

// Sliding Layer SnapPoints
void					NInsertUISlideSnapPoint(NUI_SLIDE *pslayer, const NVEC3 *pposition,const Nbool conform_to_constraint_context);
inline void				NEraseAllUISlideSnapPoints(NUI_SLIDE*pslayer){pslayer->pCurrentSnapPoint = NULL; NEraseArray(&pslayer->SnapPointArray,NULL);}

void					NInsertUISlidePin(NUI_SLIDE *pslayer, const NVEC3 *pposition,const NVEC3 *pspeed_threshold_dir, const Nf32 speed_threshold, const Nf32 captureradius, const Nbool conform_to_constraint_context);
inline void				NEraseAllUISlidePins(NUI_SLIDE*pslayer){NEraseArray(&pslayer->PinArray,NULL);}

// Sliding Layer Constraints
void					NSetUISlide_Constraint_None(NUI_SLIDE* pslayer);
void					NSetUISlide_Constraint_Segment(NUI_SLIDE* pslayer, const NVEC3 *psegment0, const NVEC3 *psegment1, const Nf32 Nf32_safedistance,const Nbool _safe_dist_is_relative);
void					NSetUISlide_Constraint_Path(NUI_SLIDE* pslayer, const NSPLINE *ppath, const Nf32 Nf32_safedistance);
void					NSetUISlide_Constraint_AABB(NUI_SLIDE* pslayer, const NAABB *paabb, const Nf32 Nf32_safedistance_x, const Nf32 Nf32_safedistance_y, const Nf32 Nf32_safedistance_z);

NVEC3*				NConformPositionToConstraintContext(NVEC3 *ppos, const NUI_SLIDE *pslayer );

inline void				NActivateUISlide_Constraint_LimitRepulse(NUI_SLIDE * pslayer){FLAG_ON(pslayer->UI.Flags,FLAG_NUI_SLIDE_LIMITS_REPULSE);}
inline void				NDeactivateUISlide_Constraint_LimitRepulse(NUI_SLIDE * pslayer){FLAG_OFF(pslayer->UI.Flags,FLAG_NUI_SLIDE_LIMITS_REPULSE);}

// Sliding Layer Swipe
void					NSetUISlide_Swipe(NUI_SLIDE* pslayer, const Nu32 flags_swipe);// use with FLAG_NUIDS_SLIDE_SWIPE,FLAG_NUIDS_SLIDE_SWIPE_BIDIRECTIONNAL,FLAG_NUIDS_SLIDE_SWIPE_ONFIELD_SEEKING. 0 means NO SWIPE
inline void				NSetUpSlide_SwipeThresHold(NUI_SLIDE* pslayer,const Nf32 swipethreshold){NErrorIf(!swipethreshold,NERROR_INCONSISTENT_PARAMETERS);pslayer->SwipeThreshold = swipethreshold;}
inline void				NSetUpSlide_SwipeIDirection(NUI_SLIDE* pslayer,const NVEC2 *pswipe_idirection){NErrorIf(NVec2Length(pswipe_idirection)<= NF32_EPSILON_VECTOR_LENGTH,NERROR_INCONSISTENT_PARAMETERS);pslayer->SwipeI = *pswipe_idirection;}
inline void				NSetUpSlide_SwipeJump(NUI_SLIDE* pslayer,const Nu16 jump){NErrorIf(!jump,NERROR_INCONSISTENT_PARAMETERS);pslayer->SwipeJump = jump;}
inline void				NSetUpSlide_SwipeAsideJump(NUI_SLIDE* pslayer,const Nu16 aside_jump){NErrorIf(!aside_jump,NERROR_INCONSISTENT_PARAMETERS);pslayer->SwipeAsideJump = aside_jump;}
inline void				NSetUpSlide_SwipeOnFieldSeekingAccuracy(NUI_SLIDE* pslayer,const Nf32 seeking_accuracy){NErrorIf(seeking_accuracy<0.0f,NERROR_INCONSISTENT_PARAMETERS);pslayer->SwipeOnFieldSeekingAccuracy = seeking_accuracy;}

// LOW LEVEL PRIVATE FUNCTIONS
Nu32 NApplyUISlideConstraints_Basic(NUI_SLIDE* psl);
Nu32 NApplyUISlideConstraints_Segment(NUI_SLIDE* psl);
Nu32 NApplyUISlideConstraints_AABB(NUI_SLIDE* psl);
Nu32 NApplyUISlideConstraints_Spline(NUI_SLIDE* psl);	

void NUpdateCurrentUISlideSnapPoint_CaptureClosest(NUI_SLIDE *pslayer);
void NApplyCurrentUISlideSnapPointConstraint(NUI_SLIDE *pslayer);

void NSwipeUISlide_Basic(NUI_SLIDE *pslayer);
void NSwipeUISlide_Bidirectionnal(NUI_SLIDE *pslayer);	
void NSwipeUISlide_OnFieldSeeking(NUI_SLIDE *pslayer);
	
	
	
	
	
	
	
	

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NUI_SLIDE_H 

