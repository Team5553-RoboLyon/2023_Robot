#ifndef __NUT_SPLINE_H
#define __NUT_SPLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Spline.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NSpline.h"

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

	//
	//
	//	  ......						.						\					    	
//	     	...						.						 \							
//		 	   ..					.						  \							
//		 	     .					 .						   \						
//		 	      .					  ..						\						
//				   .				     ....					 \						
//			       .						.......				  \						
//
	//		DECREASE ACCELERATED 	  DECREASE DECELERATED 			DECREASE LINEAR
	//
	//		.....					.							  ..........
	//	    	 ...				.									    .
	//				.				.										.
	//				.				.										 .
	//				.				 .										 .
	//				 ...			 . 										 .
	//					.....		  ...............						 .
	//
	//		  DECREASE SMOOTH			DECREASE EARLY				DECREASE LATE
	//
	//
	//						  .				   ......				  /
	//						  .			    ...						 /
	//						  .			  ..						/
	//						 .			 .						   /
	//					   ..			.						  /
	//			       ....				.						 /
	//			.......					.						/
	//
	//		 INCREASE ACCELERATED 	 INCREASE DECELERATED	INCREASE LINEAR
	//
	//					   .....	  ...........						.
	//				    ...			 .									.
	//				   .			 .									.
	//				   .			.									.
	//				   .			.						           .
	//				...				.								   .
	//		   .....				.					...............
	//
	//		INCREASE SMOOTH 		INCREASE EARLY			INCREASE LATE
	//
	//					.....							   ......
	//				  ..	 ..							...      ...
	//			     .		   .					  ..		    ..
	//			    .		    .    				 .				  .
	//				.			.					 .				  .
	//			   .			 .					.				   .
	//			...				  ...				.				   .
	//		 ...					 ....			.				   .    ............................
	//
	//					BELL								DOME		  			 FLAT
	//
	//			.							..									   ..
	//			.							.  .								 .   .
	//			.							.	 .								.	  .
	//		   . .						    .     .							    .     .
	//		   . .						   .       .						   .       .
	//		  .	  .						  .        .						  .        .
	//		..	   ..				   ...          ...				      ....         .
	//	....	     ....		  .....                ...		    ...... 	           .
	//
	//		   PIC							SHARK FIN						FORWARD WAVE
	//
	// 	   ..
	//    .	  .
	//   .	   .
	//   .	   .
	//  .	    .
	//  .	     .
	//  .	      ....
	//  .			  ......
	//
	//  	BACKWARD WAVE
	//
	typedef enum
	{
		NUT_SPLINEMODEL_DECREASE_ACCELERATED = 0,
		NUT_SPLINEMODEL_DECREASE_DECELERATED,
		NUT_SPLINEMODEL_DECREASE_LINEAR,
		NUT_SPLINEMODEL_DECREASE_SMOOTH,
		NUT_SPLINEMODEL_DECREASE_EARLY,
		NUT_SPLINEMODEL_DECREASE_LATE,

		NUT_SPLINEMODEL_INCREASE_ACCELERATED,
		NUT_SPLINEMODEL_INCREASE_DECELERATED,
		NUT_SPLINEMODEL_INCREASE_LINEAR,
		NUT_SPLINEMODEL_INCREASE_SMOOTH,
		NUT_SPLINEMODEL_INCREASE_EARLY,
		NUT_SPLINEMODEL_INCREASE_LATE,

		NUT_SPLINEMODEL_BELL,
		NUT_SPLINEMODEL_SHARPBELL,
		NUT_SPLINEMODEL_DOME,
		NUT_SPLINEMODEL_FLAT,
		NUT_SPLINEMODEL_PIC,
		NUT_SPLINEMODEL_SHARKFIN,
		NUT_SPLINEMODEL_FORWARD_WAVE,
		NUT_SPLINEMODEL_BACKWARD_WAVE,

		//--------------------------------
		NUT_SPLINEMODEL_ENUMSIZE
	} NUT_SPLINEMODEL;

	// ***************************************************************************************
	// ***************************************************************************************
	// **																					**
	// **									 												**
	// **						PRE-DEFINED SPLINE MODEL CREATION							**
	// **									---												**
	// **							     Functions											**
	// **																					**
	// ***************************************************************************************
	// ***************************************************************************************

	NSPLINE *NUT_CreatePlanarSpline(const NUT_SPLINEMODEL splinemodel, const Nu32 accuracy, const Nf32 scalex, const Nf32 scaley);

	Nu32 NUT_ExtractRegularlySpaced3DPointFromSpline(NVEC3 *ppoint, const Nu32 pointnumber, const Nu32 pointstride, const Nf32 pointdistance, const Nf32 error, const NSPLINE *pspline, const NVEC3 *porigin);
	Nf32 NUT_ExtractForwardPositionFromSpline(NVEC3 *pposition, const Nf32 t0, const Nf32 distanceforward, const Nf32 error, const NSPLINE *pspline, const NVEC3 *porigin);
	Ns32 NUT_WrapOnSpline(NVEC3 *ppoint, Nf32 *pxtd_factor, Nf32 *pfactor_step, const NSPLINE *pspline);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_SPLINE_H
