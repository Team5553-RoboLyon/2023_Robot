#ifndef __NUT_DRAW_H
#define __NUT_DRAW_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Draw.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NRectf32.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "lib/N/NSpline.h"
#include "lib/N/NStructure.h"
#include "lib/N/Utilities/NUT_Slider.h"
#include "lib/N/Utilities/NUT_GizmoMove.h"
#include "lib/N/Utilities/NUT_GizmoRectf32.h"
#include "lib/N/Utilities/NUT_2DAAColorCage.h"
#include "lib/N/Utilities/NUT_XMesh.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **														GLOBAL and GENERIC DEFINEs																			**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************

	// ***************************************************************************************
	// ***************************************************************************************
	// **																					**
	// **									 												**
	// **									Draw											**
	// **									---												**
	// **						enums, structures and define								**
	// **																					**
	// ***************************************************************************************
	// ***************************************************************************************
	// GLOBALS
	typedef enum
	{
		NUT_DRAW_RENDERING_MODE_3D,
		NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC,
		//-------------------------
		NUT_DRAW_RENDERING_MODE_ENUM_SIZE
	} NUT_DRAW_RENDERING_MODE_ENUM;
#define NUT_DRAW_DEFAULT_PRIMITIVE_CAPACITY 256

// SLIDER
#define STRING_NUT_SLIDER_PRINT_VALUE "%.6f"
#define CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT 0.125f // ... relative to the NUT_DRAW_SLIDER text size if there is one. To place the text from the handle tick extremity.
												   // 1.0f means 1 text character height, 0.125f means 1/4 character height
// ELLIPSE
#define CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_LOWEST 8
#define CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_LOW 16
#define CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_MEDIUM 32
#define CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_HIGH 64
#define CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_BEST 2048
#define CONSTANT_NUT_DRAW_AACIRCLE_ANGLESTEP 256

#define FLAG_NUT_DRAW_ELLIPSE_MARKINGS_ONLY BIT_0
#define FLAG_NUT_DRAW_ELLIPSE_MARKINGS BIT_1
#define FLAG_NUT_DRAW_ELLIPSE_DASHLINE BIT_2
#define FLAG_NUT_DRAW_ELLIPSE_OPEN BIT_3
	typedef struct
	{
		NVEC3 Center;
		NVEC2 Extents; // NVEC2 because its a FLAT POLYGON !
		NCOLOR Color;
		Nf32 FromAngle;
		Nf32 ToAngle;
		Nu16 SliceNb;		// Number of building Slices ( = number of segments along the perimeter )
		Nu16 MarkingNb;		// Number of Markings.
		Nu8 MarkingOutSize; // >>> float = MarkingOutSize/255
		Nu8 MarkingInSize;	// >>> float = MarkingInSize/255
		Nu16 Flags;
	} NUT_DRAW_ELLIPSE;

// A Spline description
#define CONSTANT_NUT_DRAW_SPLINE_ELLIPSE_HANDLE_SLICE_NB 32 // (=CONSTANT_NUT_DRAW_ELLIPSE_SLICENB_MEDIUM)
#define FLAG_NUT_DRAW_SPLINE_HIDESPLINE BIT_0
#define FLAG_NUT_DRAW_SPLINE_HIDESPLINEKNOTS BIT_1
#define FLAG_NUT_DRAW_SPLINE_HIDETANGENTES BIT_2
#define FLAG_NUT_DRAW_SPLINE_HIDE_XTRA BIT_3
	typedef struct
	{
		// NSPLINE					*pSpline;
		NCOLOR Color;
		Nf32 KnotSize;
		Nf32 Xtra_Threshold; // For Xtra calculation. Represents the smallest value against that we consider result equivalent to ZERO.
							 // Should be <= NF32_EPSILON_VECTOR_LENGTH For high Precision XTRA infos.
							 // Should be <= NF32_SMALL_VECTOR_LENGTH For medium ( and enough)  Precision XTRA infos.
		Nu32 Flags;
	} NUT_DRAW_SPLINE;

	// A Triangle Array description (with the vertex)
	// Helpful to easy draw a geometry
	typedef struct
	{
		NARRAY *pTrianglesArray;
		NCOLOR Color;
		NVEC3 *pFirstVertexPosition;
		Nu32 VertexStride;
	} NUT_DRAW_TRIANGLES_ARRAY;

#define NPRESET1F_DRAWTEXT_ORTHOGRAPHIC_SIZE_SMALL 0.01f
#define NPRESET1F_DRAWTEXT_ORTHOGRAPHIC_SIZE_MEDIUM 0.025f
#define NPRESET1F_DRAWTEXT_ORTHOGRAPHIC_SIZE_LARGE 0.05f
	typedef struct
	{
		Nf32 Size; // Relative to Screen Height. a Size of 1 means a draw text as height as the screen.
		NCOLOR Color;
	} NUT_DRAWTEXT;

// NUT_DRAW_ARROWS
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// A 2D Arrow(s) ( = a segment with/without arrows on each extremity)
#define CONSTANT_NUT_DRAW_ARROWS_CIRCLE_ANGLESTEP 256

// The DEFINE  "BITS_NUT_DRAW_ARROWS_SHAPE_xxx" must be used with the 2 inlines:
//							NUT_SetDrawArrows_ShapeA
//							NUT_SetDrawArrows_ShapeB
#define BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD 0xD
#define BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLESHIFTED 0xC
#define BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLE 0xB
#define BITS_NUT_DRAW_ARROWS_SHAPE_CROSS 0xA
#define BITS_NUT_DRAW_ARROWS_SHAPE_DIAMOND 0x9
#define BITS_NUT_DRAW_ARROWS_SHAPE_FASTBACKWARD 0x8
#define BITS_NUT_DRAW_ARROWS_SHAPE_FASTFORWARD 0x7
#define BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD 0x6
#define BITS_NUT_DRAW_ARROWS_SHAPE_HALFSQUARE 0x5
#define BITS_NUT_DRAW_ARROWS_SHAPE_PAUSE 0x4
#define BITS_NUT_DRAW_ARROWS_SHAPE_SQUARE 0x3
#define BITS_NUT_DRAW_ARROWS_SHAPE_STICK 0x2
#define BITS_NUT_DRAW_ARROWS_SHAPE_SPACE 0x1 // this is a "blank space"
#define BITS_NUT_DRAW_ARROWS_SHAPE_NONE 0x0	 // by default, BArrow is drawn as a line

#define MASK_NUT_DRAW_ARROWS_SHAPE 0xF
#define SHIFT_NUT_DRAW_ARROWS_SHAPE_A 4			  // SHAPE_A bits are shifted at storage ( with a 4 bits shifting )
#define SHIFT_NUT_DRAW_ARROWS_SHAPE_B 0x0		  // SHAPE_B bits are not shifted at storage ( 0 bit shifting )
#define MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A 0xF0 // = (0xF<<4) ... SHAPE_A bits are shifted at storage ( with a 4 bits shifting )
#define MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_B 0xF  // = (0xF<<0) ... SHAPE_B bits are not shifted at storage ( 0 bit shifting )
// These 2 MACROS allow user to "flag" directly NUT_DRAW_ARROWS.Flags like this:
//		FLAG_ON( NUT_DRAW_ARROWS.Flags, FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLE) ) to define A SHAPE
//		or
//		FLAG_ON( NUT_DRAW_ARROWS.Flags, FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLE) ) to define B SHAPE
#define FLAGS_NUT_DRAW_ARROWS_BSHAPE(bits_shape) ((bits_shape)&MASK_NUT_DRAW_ARROWS_SHAPE) // it should be ((id4bits)<< SHIFT_NUT_DRAW_ARROWS_SHAPE_B) but SHIFT_NUT_DRAW_ARROWS_SHAPE_B is ZERO so ...
#define FLAGS_NUT_DRAW_ARROWS_ASHAPE(bits_shape) (((bits_shape)&MASK_NUT_DRAW_ARROWS_SHAPE) << SHIFT_NUT_DRAW_ARROWS_SHAPE_A)

// The DEFINE  "ID2BITS_NUT_DRAW_ARROWS_xxx" must be used with the inline:
//							NUT_SetDrawArrows_TickSize
#define BITS_NUT_DRAW_ARROWS_TICK_SIZE_VERY_SMALL 0x3 // 1/2^3 = 1/8
#define BITS_NUT_DRAW_ARROWS_TICK_SIZE_SMALL 0x2	  // 1/2^2 = 1/4
#define BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON 0x1	  // 1/2^1 = 1/2
#define BITS_NUT_DRAW_ARROWS_TICK_SIZE_LARGE 0x0	  // 1/2^0 = 1/1

#define MASK_NUT_DRAW_ARROWS_TICK_SIZE 0x3
#define SHIFT_NUT_DRAW_ARROWS_TICK_SIZE 14			  // TICK_SIZE bits are shifted at storage ( with a 14 bits shifting )
#define MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_SIZE 0xC000 // = (0x3<<14) ... TICK_SIZE bits are shifted at storage ( with a 14 bits shifting )

#define BITS_NUT_DRAW_ARROWS_TICK_CENTERED 0x3 // right and left part of the tick . So it cross the AB segment.
#define BITS_NUT_DRAW_ARROWS_TICK_LEFT 0x2	   // Only the left part of the tick ( at the left of the AB segment )
#define BITS_NUT_DRAW_ARROWS_TICK_RIGHT 0x1	   // Only the right part of the tick ( at the right of the AB segment )
#define BITS_NUT_DRAW_ARROWS_TICK_NONE 0x0	   // No tick

#define MASK_NUT_DRAW_ARROWS_TICK 0x3
#define SHIFT_NUT_DRAW_ARROWS_TICK 12			 // TICK bits are shifted at storage ( with a 12 bits shifting )
#define MASK_SHIFTED_NUT_DRAW_ARROWS_TICK 0x3000 // = (0x3<<12) ... TICK bits are shifted at storage ( with a 12 bits shifting )
// These 3 MACROS allow user to "flag" directly NUT_DRAW_ARROWS.Flags like this:
//		FLAG_ON( NUT_DRAW_ARROWS.Flags, FLAGS_NUT_DRAW_ARROWS_TICK_SHAPE(BITS_NUT_DRAW_ARROWS_TICK_SHAPE_LEFT) ) to define TICK SHAPE
//		or
//		FLAG_ON( NUT_DRAW_ARROWS.Flags, FLAGS_NUT_DRAW_ARROWS_TICK(BITS_NUT_DRAW_ARROWS_TICK_SHAPE_LEFT,BITS_NUT_DRAW_ARROWS_TICK_SIZE_COMMON) ) to entirely arrow ticks
#define FLAGS_NUT_DRAW_ARROWS_TICK(bits_tick) (((bits_tick)&MASK_NUT_DRAW_ARROWS_TICK) << SHIFT_NUT_DRAW_ARROWS_TICK)
#define FLAGS_NUT_DRAW_ARROWS_TICK_SIZE(bits_tick_size) (((bits_tick_size)&MASK_NUT_DRAW_ARROWS_TICK_SIZE) << SHIFT_NUT_DRAW_ARROWS_TICK_SIZE)
#define FLAGS_NUT_DRAW_ARROWS_TICK_ALL(bits_tick, bits_tick_size) ((((bits_tick)&MASK_NUT_DRAW_ARROWS_TICK) << SHIFT_NUT_DRAW_ARROWS_TICK) | (((bits_tick_size)&MASK_NUT_DRAW_ARROWS_TICK_SIZE) << SHIFT_NUT_DRAW_ARROWS_TICK_SIZE))

// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_0	\
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_1	 |
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_2	 |
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_3	 |	To define Arrowheads Shapes
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_4	 |
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_5	 |
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_6	 |
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_7	/
#define FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE BIT_8 // by default AArrow Size is relative to segment length
#define FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE BIT_9 // by default BArrow Size is relative to segment length
// #define FLAG_NUT_DRAW_ARROWS_available					BIT_10
// #define FLAG_NUT_DRAW_ARROWS_available					BIT_11
//  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_TICK_xxx		BIT_12	\
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_TICK_xxx		BIT_13	 |	To define Ticks along Arrow...
//  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_TICK_SIZE_xxx	BIT_14	 |	... Tick Size and Tick Shape are customisable
//  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_TICK_SIZE_xxx	BIT_15	/
#define FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE BIT_16		   // by default Unit is relative to AB segment length
#define FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN BIT_17	   // by default ticks are spread along the whole segment AB, even if some of them aren't drawn ( no juxtaposition with Arrowheads )
#define FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_FROM_CENTER BIT_18   // by default ticks are spread from A to B. with this flag ON, ticks are going to be spread in both directions from the middle of the segment AB ( or from the inbetween segment ).
#define FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT BIT_19 // by default tick size is relative to B ArrowHead Size ( even if there is no B Arrowhead to draw ). With this flag to ON, Tick size is going to be calculated from Unit instead.
															   // #define FLAG_NUT_DRAW_ARROWS_available					BIT_20
															   // #define FLAG_NUT_DRAW_ARROWS_available					BIT_21
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_22	\
// BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_23	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_24	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_25	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_26	 |_ Reserved to store a 10 bits value [0,1024[ which represents the MaxTickDensity
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_27	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_28	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_29	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_30	 |
															   //  BIT RESERVED BY BITS_NUT_DRAW_ARROWS_SHAPE_xxx		BIT_31	/

#define MASK_NUT_DRAW_ARROWS_TICK_DENSITY_MAX 0x3FF
#define SHIFT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX 22				 // TICK bits are shifted at storage ( with a 12 bits shifting )
#define MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_DENSITY_MAX 0xFFC00000 // = (0x3<<12) ... TICK bits are shifted at storage ( with a 12 bits shifting )

#define CONSTANT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX_MAX 1023	 // la valeur maximum possible pour density max
#define CONSTANT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX_MIN 1		 // la valeur minimum pour density max
#define CONSTANT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX_HIGH 512	 //
#define CONSTANT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX_MEDIUM 128 //
#define CONSTANT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX_LOW 32	 //

#define GET_NUT_DRAW_ARROWS_TICK_DENSITY_MAX(flags) (((flags) >> SHIFT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX) & MASK_NUT_DRAW_ARROWS_TICK_DENSITY_MAX)

	typedef struct
	{
		NCOLOR Color;
		Nf32 AArrowSize; // relative size of A Arrow ( relative to AB size )
		Nf32 BArrowSize; // relative size of B Arrow ( relative to AB size )
		Nf32 Unit;		 // Size of segment ( = "space" between 2 ticks )
		Nu32 Flags;		 // constructions Flags to activate several options
	} NUT_DRAW_ARROWS;

	inline void NUT_SetDrawArrows_TickDensityMax(NUT_DRAW_ARROWS *p, const Nu32 densitymax)
	{
		NErrorIf((densitymax & MASK_NUT_DRAW_ARROWS_TICK_DENSITY_MAX) != densitymax, NERROR_VALUE_OUTOFRANGE);
		SET_FLAGS(p->Flags, MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_DENSITY_MAX, ((densitymax & MASK_NUT_DRAW_ARROWS_TICK_DENSITY_MAX) << SHIFT_NUT_DRAW_ARROWS_TICK_DENSITY_MAX));
	}

	inline void NUT_SetDrawArrows_ShapeA(NUT_DRAW_ARROWS *p, const Nu32 bits_shape_a)
	{
		NErrorIf((bits_shape_a & MASK_NUT_DRAW_ARROWS_SHAPE) != bits_shape_a, NERROR_VALUE_OUTOFRANGE);
		SET_FLAGS(p->Flags, MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A, ((bits_shape_a & MASK_NUT_DRAW_ARROWS_SHAPE) << SHIFT_NUT_DRAW_ARROWS_SHAPE_A));
	}

	inline void NUT_SetDrawArrows_ShapeB(NUT_DRAW_ARROWS *p, const Nu32 bits_shape_b)
	{
		NErrorIf((bits_shape_b & MASK_NUT_DRAW_ARROWS_SHAPE) != bits_shape_b, NERROR_VALUE_OUTOFRANGE);
		SET_FLAGS(p->Flags, MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_B, ((bits_shape_b & MASK_NUT_DRAW_ARROWS_SHAPE) << SHIFT_NUT_DRAW_ARROWS_SHAPE_B));
	}

	inline void NUT_SetDrawArrows_TickSize(NUT_DRAW_ARROWS *p, const Nu32 bits_tick_size)
	{
		NErrorIf((bits_tick_size & MASK_NUT_DRAW_ARROWS_TICK_SIZE) != bits_tick_size, NERROR_VALUE_OUTOFRANGE);
		SET_FLAGS(p->Flags, MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_SIZE, ((bits_tick_size & MASK_NUT_DRAW_ARROWS_TICK_SIZE) << SHIFT_NUT_DRAW_ARROWS_TICK_SIZE));
	}

	inline void NUT_SetDrawArrows_Tick(NUT_DRAW_ARROWS *p, const Nu32 bits_tick)
	{
		NErrorIf((bits_tick & MASK_NUT_DRAW_ARROWS_TICK) != bits_tick, NERROR_VALUE_OUTOFRANGE);
		SET_FLAGS(p->Flags, MASK_SHIFTED_NUT_DRAW_ARROWS_TICK, ((bits_tick & MASK_NUT_DRAW_ARROWS_TICK) << SHIFT_NUT_DRAW_ARROWS_TICK));
	}

	/*
	// An Oriented Axis
	#define DEFAUL_NUT_DRAW_AXIS_ARROW_SIZE				0.1f
	#define DEFAULT_NUT_DRAW_AXIS_GRADUATIONEXTEND		0.5f
	#define	FLAG_NUT_DRAW_AXIS_USEPARAM_ARROWPARAM		BIT_0
	#define	FLAG_NUT_DRAW_AXIS_USEPARAM_LABELPARAM		BIT_1
	typedef struct
	{
		NUT_DRAW_ARROWS		ArrowParam;			// Parameters of the "Axis" itself ( the oriented segment )
		NUT_DRAWTEXT		LabelParam;			// Parameters of the "Labels" ( texts written arround the Axis )
		Nf32				Unit;				// Size of 1 unit
		Nf32				GraduationExtend;	// Half length of graduation relative to 1 unit
		Nu32				Flags;
	}NUT_DRAW_AXIS;
	*/

	// A Physic Structure description (with the vertex)
	// Helpful to easy draw It !
	typedef struct
	{
		NSTRUCTURE *pStructure;
		NSTRUCTURE_ELEMENT_TYPE_ENUM ElementType;
		NCOLOR Color;
		Nf32 JointSize;
	} NUT_DRAW_PHYSICSTRUCTURE;

// A Spring description (notes: NUT_SPRING is NOT a representation of an  NSTRUCTURE NSPRING. It's 2 different things !
#define FLAG_NUT_DRAW_SPRING_USEPARAM_BUILDVECTOR BIT_0 // by default is not going to be used and the build vector will be estimated

	typedef struct
	{
		NVEC3 A;		   // first spring extremity	( segment like )
		NVEC3 B;		   // second spring extremity	( segment like )
		NVEC3 BuildVector; // To orient the Spring Draw 'vertical'..
		NCOLOR Color;

		//	NVEC3				Construction_Vector;// with AB, define a plan orthogonal with the Spring plan
		Nf32 Thickness;	 // NUT_DRAW_SPRING is a 2D planar shape, [AB] represents the total spring length and Thickness the second dimension of it
		Nu16 SpringCoil; // Number of SpringCoils ( visually a "coil" = a 'V')
		Nu16 Flags;
	} NUT_DRAW_SPRING;

	// A COLORCAGE description
	typedef struct
	{
		NUT_2DAACOLORCAGE *pColorCage;
		NCOLOR Color;
		NCOLOR ActiveKnotColor;
		Nu32 ActiveKnotID; // !!! Be careful: 0 is a valid ID so, force it to NVOID if you don't want to draw any knot with ActiveColor !!!
		Nf32 KnotSize;
	} NUT_DRAW_COLORCAGE;

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **																			FUNCTIONS																		**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	NUT_DRAW_RENDERING_MODE_ENUM NUT_GetDrawMode();
	void NUT_SetDrawMode(const NUT_DRAW_RENDERING_MODE_ENUM draw_mode);
	void NUT_SetDrawConstructionPlane(const NAAPLANE_ENUM axis_aligned_plane);
	void NUT_SetDrawConstructionPlaneUnitDirectorVector(const NVEC3 *punit_director_vector);
	void NUT_SetDrawConstructionPlaneMatrix(const NMATRIX *pmtx);
	void NUT_SetDrawConstructionPlaneMatrix3x3(const NMATRIX3x3 *pmtx);

	void NUT_Draw_2DAAColorCage(const NUT_DRAW_COLORCAGE *pdraw);
	void NUT_Draw_AABB(const NAABB *paabb, const NCOLOR *pcolor);
	void NUT_Draw_Angle(const NVEC3f32 *pO, const NVEC2f32 *pA, const Nf32 angle, const NCOLOR *pcolor);
	void NUT_Draw_FastAngle(const NVEC3f32 *pO, const NVEC2f32 *pA, const Nu32 fastangle, const NCOLOR *pcolor);
	void NUT_Draw_Arrows(const NVEC3 *pA, const NVEC3 *pB, const NUT_DRAW_ARROWS *parrows);
	void NUT_Draw_BoundingSphere(const NBOUNDINGSPHERE *psphere, const NCOLOR *pcolor);
	void NUT_Draw_Cross(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Ellipse(const NUT_DRAW_ELLIPSE *pellipse);
	void NUT_Draw_GizmoMove(const NUT_GIZMO_MOVE *pgm, const NCOLOR *pactivecolor, const NUT_DRAWTEXT *ptxtparam);
	void NUT_Draw_XYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NCOLOR *pcolor, NCOLOR *pselectioncolor);

	void NUT_Draw_Mark(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Left(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Right(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Up(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Down(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);

	void NUT_Draw_PhysicStructure(const NUT_DRAW_PHYSICSTRUCTURE *pdraw);

	void NUT_Draw_Quad(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_Rectangle(const NRECTf32 *prectangle, const NCOLOR *pcolor);

	void NUT_Draw_DoubleSegment(const NVEC3 *pA, const NVEC3 *pB, const Nf32 thickness, const NCOLOR *pcolor);
	void NUT_Draw_Segment(const NVEC3 *pA, const NVEC3 *pB, const NCOLOR *pcolor);
	void NUT_Draw_Stick(const NVEC3 *pA, const NVEC3 *pB, const Nf32 thickness, const NCOLOR *pcolor);
	// void NUT_Draw_Axis(const NVEC3 *pO, const NVEC3 *pB, const NUT_DRAW_AXIS *pparam);

	void NUT_Draw_Spline(const NSPLINE *pspline, const NUT_DRAW_SPLINE *pdrawspline);
	void NUT_Draw_Spring(const NUT_DRAW_SPRING *pspring);
	void NUT_Draw_Slider(const NUT_SLIDER *pslider, const NVEC3 *pposition, const NCOLOR *pcolor, const NUT_DRAWTEXT *ptxtparam);
	void NUT_Draw_Text(const Nchar *pstring, const NVEC3 *pposition, const NUT_DRAWTEXT *pparams);
	void NUT_Draw_TrianglesArray(const NUT_DRAW_TRIANGLES_ARRAY *ptriarray);
	void NUT_Draw_Vector3Array(const NARRAY *pvector3array, const NCOLOR *pcolor);
	void NUT_Draw_IsolatedXtri(const NUT_XTRI_ARRAY *pxtriarray, const NCOLOR *pcolor);
	void NUT_Draw_XArea(const NUT_XAREA *pxarea, const NCOLOR *pcolor);
	void NUT_Draw_XRPoly(const NUT_XRPOLY *pxrpoly, const NCOLOR *pcolor);
	void NUT_Draw_3DMatrix3x3(const NMATRIX3x3 *pm3x3, const NVEC3 *pposition, const Nf32 scale);

	void NUT_RenderDrawings();

	// Tools for World Axis Aligned Drawings.
	// These functions are not using the 'Draw Orthonormal' but simply used the World XYZ Orthonormal.
	// And draw their Shapes on the dimensions included in their names 'XY' means draw on the XY plane of the WORLD base.
	// This version of the draw function are faster than the global version which works with the customizable Draw Orthonormal.
	// for example 'NUT_Draw_XYSlider' and 'NUT_Draw_Slider' do the same JOB but 'NUT_Draw_XYSlider' is faster because it doesn't transform
	// coordinates into the Draw Orthonormal
	void NUT_Draw_XYSlider(const NUT_SLIDER *pslider, const NVEC3 *pposition, const NCOLOR *pcolor, const NUT_DRAWTEXT *ptxtparam);
#define NUT_Draw_SliderOnScreenPlane(pslider, pcolor, ptxtparam) NUT_Draw_XYSlider(pslider, NULL, pcolor, ptxtparam)

	void NUT_Draw_XYSquare(const NVEC3 *pcenter, const Nf32 side, const NCOLOR *pcolor);
	void NUT_Draw_XYQuad(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_XYMark(const NVEC3 *pcenter, const NVEC2 *pextents, const NCOLOR *pcolor);
	void NUT_Draw_XYCircle(const NVEC3 *pcenter, const Nf32 radius, const NCOLOR *pcolor);
	void NUT_Draw_XZCircle(const NVEC3 *pcenter, const Nf32 radius, const NCOLOR *pcolor);
	void NUT_Draw_ZYCircle(const NVEC3 *pcenter, const Nf32 radius, const NCOLOR *pcolor);

	void NUT_Draw_XYTriangleR(const NVEC3 *pcenter, const Nf32 side, const NCOLOR *pcolor);
	void NUT_Draw_XYTriangleL(const NVEC3 *pcenter, const Nf32 side, const NCOLOR *pcolor);

	// Draw Text tools
	Nf32 NUT_GetHighestDrawTextCharHeight(const NUT_DRAWTEXT *pparams);
	Nf32 NUT_GetLargestDrawTextCharWidth(const NUT_DRAWTEXT *pparams);
	Nf32 NUT_EstimateDrawTextCharLineLength(const Nu32 len, const NUT_DRAWTEXT *pparams);
	Nf32 NUT_EstimateDrawTextCharLinesHeight(const Nu32 line_nb, const NUT_DRAWTEXT *pparams);

	// PRIVATE USE (user should not use these functions )
	void NUT_Initialize_DrawTextCharTable();

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_H
