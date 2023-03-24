#ifndef __NSPLINE_H
#define __NSPLINE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NSpline.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "./Containers/NArray.h"
#include "./Containers/NNode.h"
#include "./Maths/NVec2f32.h"
#include "./Maths/NVec3f32.h"
#include "./Miscellaneous/NColor.h"
#include "NFlags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define NSPLINE_KAPPA							0.55228474983f // = (4/3)*tan(pi/(2n)) with n = number of Bezier curve used to approximate all the circle arround. So our kappa is to create a circle with 4 successive bezier curves. 

#define NSPLINE_DEFAULT_SUBSPLINE_ACCURACY		50

// Flags SplineKnot
// BIT_0 to BIT_7 are private
// #define FLAG_SPLINEKNOT_UNLINKED_TANGENT			BIT_0
// #define FLAG_SPLINEKNOT_TANGENT_LINKEDSIZE		BIT_1
// #define FLAG_SPLINEKNOT_TANGENT_LINKEDANGLE		BIT_2
// #define FLAG_SPLINEKNOT_TANGENT_ALIGNED			BIT_3

// BIT_8 to BIT_15 are user available

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
// TANGENTS BUILDING
// N provides a way to 'auto' create SplineKnot tangents TA and TB during spline setup/creation process.
// N does this with only one 32 bits code by knot. 
// This 32 bits code number is build like this:
//
//					----	----	----	----	----	----	----	---- 
//					 TA 	TA X	TA Y	TA Z	 TB	 	TB X	TB Y	TB Z
//					Syst.	code	code	code	Syst.	code	code	code
//
//
// Each Tangent uses one WORD of the 32 bits value
// Each Tangent WORD is split into 4 nibbles (4 bits each)
// The first nibble codes the coordinates system used by the tangent coordinates coded into the 3 next nibbles.
//		There are 4 different available systems:
//
//		NSKT_3DWORLD_ABSOLUTE	Means coordinates X,Y,Z will be understood has ABSOLUTE 3DWORLD coordinates 
//								So, TAx = 1 means TAx = 1 (World Unit). That's it !
//
//		NSKT_3DWORLD_RELATIVE	Means coordinates X,Y,Z will be understood has 3DWORLD coordinates, Relative to length of the Knots couple, which describe the subspline where Tangent are used in.   
//								So, TAx = 1 means TAx = 1 x length( Previous Knot, Knot of TA ). 
//																	Previous Knot ? YES, because for a subspline build with 2 knots, knot0 and knot1...
//																	... spline calculation uses Knot0.TB and Knot1.TA
//								So, TBx = 1 means TBx = 1 x length( Knot of TB, Next Knot). 
//																	Next Knot ? YES, because for a subspline build with 2 knots, knot0 and knot1...
//																	... spline calculation uses Knot0.TB and Knot1.TA
//		NSKT_3DLOCAL_ABSOLUTE	Means coordinates X,Y,Z will be understood has ABSOLUTE 3DLOCAL coordinates. 
//								So, TAx = 1 means TAx = 1 (World Unit). But not into the 3D WORLD system, into the LOCAL system ...
//																	LOCAL coordinates system uses the Knots couple where the Tangent is used in.
//																	So for Knot1.TA, local X Axis is aligned with  (Knot0,Knot1)3D vector
//																	So for Knot1.TB, local X Axis is aligned with  (Knot1,Knot2)3D vector
//
//		NSKT_3DLOCAL_RELATIVE	Means coordinates X,Y,Z will be understood has 3DLOCAL coordinates Relative to length of the Knots couple, which describe the subspline where Tangent are used in.    
//								So, TAx = 1 means TAx = 1 x length( Previous Knot, Knot of TA ). 
//																	LOCAL coordinates system uses the Knots couple where the Tangent is used in.
//																	So for Knot1.TA, local X Axis is aligned with  (Knot0,Knot1)3D vector
//																	So for Knot1.TB, local X Axis is aligned with  (Knot1,Knot2)3D vector
//
// ... and the 3 others nibbles code the Tangent coordinates X,Y,Z themselves.
//		The second Nibble codes X.
//		The Third codes Y
//		The fourth codes Z.
//
//		Because we have only 4 bits for each coordinates we only have 16 different possible values (from 0 to 15 included).
//		So, we are going to use a LUT to be more useful.
//		LUT values (15 values actually) are like this:
//			4,3,2,1,1/2,1/3,1/4,0,-1/4,-1/3,-1/2,-1,-2,-3,-4
//
//		Some useful and common Tangent code are already defined

// NSPLINEKNOT TANGENTS MACROS to pack/unpack knot tangents descriptions
#define NSKTANGENTI(Ts,Tx,Ty,Tz)							( ((((Nu32)Ts)&0xF)<<12)|((((Nu32)Tx)&0xF)<<8)|((((Nu32)Ty)&0xF)<<4)|(((Nu32)Tz)&0xF) )
#define NSKTANGENTSPACKI(TAs,TAx,TAy,TAz,TBs,TBx,TBy,TBz)	( ((((Nu32)TAs)&0xF)<<28)|((((Nu32)TAx)&0xF)<<24)|((((Nu32)TAy)&0xF)<<20)|((((Nu32)TAz)&0xF)<<16)|((((Nu32)TBs)&0xF)<<12)|((((Nu32)TBx)&0xF)<<8)|((((Nu32)TBy)&0xF)<<4)|(((Nu32)TBz)&0xF) )
#define NSKTANGENTSPACK(TA,TB)								( ((((Nu32)TA)&0xFFFF)<<16)|(((Nu32)TB)&0xFFFF))
#define NSKTANGENTA_EXTRACT(Tpack)							(((Nu32)Tpack)>>16)
#define NSKTANGENTB_EXTRACT(Tpack)							(((Nu32)Tpack)&0xFFFF)

// NSPLINEKNOT TANGENT Builder Coordinates System
// #define NSKT_3DWORLD_ABSOLUTE	0
// #define NSKT_3DWORLD_RELATIVE	1
// #define NSKT_3DLOCAL_ABSOLUTE	2
// #define NSKT_3DLOCAL_RELATIVE	3
#define NSKT_WORLD_N1	0	//NSKT_3DWORLD_ABSOLUTE		>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k with a length of 1
#define NSKT_WORLD_NRL	1	//NSKT_3DWORLD_RELATIVEL	>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k with a length of "L" which is the length of vector (p1,p0)
#define NSKT_WORLD_NRX	2	//NSKT_3DWORLD_RELATIVEX	>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k with a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
#define NSKT_WORLD_NRY	3	//NSKT_3DWORLD_RELATIVEY	>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k with a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
#define NSKT_WORLD_NRZ	4	//NSKT_3DWORLD_RELATIVEZ	>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k with a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
#define NSKT_WORLD_RXYZ	5	//NSKT_3DWORLD_RELATIVEXYZ	>>> Tangent will be build into a Direct Orthonormal base aligned with the WORLD ORTHORMAL BASE with i,j,k their own length which are respectively the absolute value of the vector (p1,p0) X,Y,Z World components.

#define NSKT_LOCAL_N1	6	//NSKT_3DLOCAL_ABSOLUTE		>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k will have a length of 1.
#define NSKT_LOCAL_NRL	7	//NSKT_3DLOCAL_RELATIVEL	>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k will have a length of "L" which is the length of vector (p1,p0).
#define NSKT_LOCAL_NRX	8	//NSKT_3DLOCAL_RELATIVEX	>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k will have a length of "X" which is the absolute value of the vector (p1,p0) X WORLD component.
#define NSKT_LOCAL_NRY	9	//NSKT_3DLOCAL_RELATIVEY	>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k will have a length of "Y" which is the absolute value of the vector (p1,p0) Y WORLD component.
#define NSKT_LOCAL_NRZ	0xA	//NSKT_3DLOCAL_RELATIVEZ	>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k will have a length of "Z" which is the absolute value of the vector (p1,p0) Z WORLD component.
#define NSKT_LOCAL_RXYZ	0xB	//NSKT_3DLOCAL_RELATIVEXYZ	>>> Tangent will be build into a Direct Orthonormal base with Z axis align with vector (p1,p0). i,j,k have their own length which are the absolute value of the vector (p1,p0) X,Y,Z World components.


// NSPLINEKNOT TANGENT Builder Coordinates Value Codes
#define NSKT_P4					7		// means 4
#define NSKT_P3					6		// means 3
#define NSKT_P2					5		// means 2
#define NSKT_P1					4		// means 1
#define NSKT_P1o2				3		// means 1/2
#define NSKT_P1o3				2		// means 1/3
#define NSKT_P1o4				1		// means 1/4
#define NSKT0					0		// means 0
#define NSKT_M1o4				8		// means -1/4
#define NSKT_M1o3				9		// means -1/3
#define NSKT_M1o2				0xA		// means -1/2
#define NSKT_M1					0xB		// means -1
#define NSKT_M2					0xC		// means -2
#define NSKT_M3					0xD		// means -3
#define NSKT_M4					0xE		// means -4

// NSPLINEKNOT TANGENT Builder Predefined Tangent code
// ( each code below describes only one Tangent, use Macro NSKTANGENTSPACK to combine TA and TB)
// ---------------------------------------------------
/*
#define NSKT_NULL						0x0000

#define NSKT_LOCALR_ZALIGN_P4			0x3007
#define NSKT_LOCALR_ZALIGN_P3			0x3006
#define NSKT_LOCALR_ZALIGN_P2			0x3005
#define NSKT_LOCALR_ZALIGN_P1			0x3004
#define NSKT_LOCALR_ZALIGN_P1of2		0x3003
#define NSKT_LOCALR_ZALIGN_P1of3		0x3002
#define NSKT_LOCALR_ZALIGN_P1of4		0x3001
//#define NSKT_NULL						0x0000
#define NSKT_LOCALR_ZALIGN_M1of4		0x3008
#define NSKT_LOCALR_ZALIGN_M1of3		0x3009
#define NSKT_LOCALR_ZALIGN_M1of2		0x300A
#define NSKT_LOCALR_ZALIGN_M1			0x300B
#define NSKT_LOCALR_ZALIGN_M2			0x300C
#define NSKT_LOCALR_ZALIGN_M3			0x300D
#define NSKT_LOCALR_ZALIGN_M4			0x300E

#define NSKT_LOCALR_YALIGN_P4			0x3070
#define NSKT_LOCALR_YALIGN_P3			0x3060
#define NSKT_LOCALR_YALIGN_P2			0x3050
#define NSKT_LOCALR_YALIGN_P1			0x3040
#define NSKT_LOCALR_YALIGN_P1of2		0x3030
#define NSKT_LOCALR_YALIGN_P1of3		0x3020
#define NSKT_LOCALR_YALIGN_P1of4		0x3010
//#define NSKTANGENT_NULL				0x0000
#define NSKT_LOCALR_YALIGN_M1of4		0x3080
#define NSKT_LOCALR_YALIGN_M1of3		0x3090
#define NSKT_LOCALR_YALIGN_M1of2		0x30A0
#define NSKT_LOCALR_YALIGN_M1			0x30B0
#define NSKT_LOCALR_YALIGN_M2			0x30C0
#define NSKT_LOCALR_YALIGN_M3			0x30D0
#define NSKT_LOCALR_YALIGN_M4			0x30E0

#define NSKT_LOCALR_XALIGN_P4			0x3700
#define NSKT_LOCALR_XALIGN_P3			0x3600
#define NSKT_LOCALR_XALIGN_P2			0x3500
#define NSKT_LOCALR_XALIGN_P1			0x3400
#define NSKT_LOCALR_XALIGN_P1of2		0x3300
#define NSKT_LOCALR_XALIGN_P1of3		0x3200
#define NSKT_LOCALR_XALIGN_P1of4		0x3100
//#define NSKTANGENT_NULL				0x0000
#define NSKT_LOCALR_XALIGN_M1of4		0x3800
#define NSKT_LOCALR_XALIGN_M1of3		0x3900
#define NSKT_LOCALR_XALIGN_M1of2		0x3A00
#define NSKT_LOCALR_XALIGN_M1			0x3B00
#define NSKT_LOCALR_XALIGN_M2			0x3C00
#define NSKT_LOCALR_XALIGN_M3			0x3D00
#define NSKT_LOCALR_XALIGN_M4			0x3E00

#define NSKT_WORLDR_XALIGN_P4			0x1700
#define NSKT_WORLDR_XALIGN_P3			0x1600
#define NSKT_WORLDR_XALIGN_P2			0x1500
#define NSKT_WORLDR_XALIGN_P1			0x1400
#define NSKT_WORLDR_XALIGN_P1of2		0x1300
#define NSKT_WORLDR_XALIGN_P1of3		0x1200
#define NSKT_WORLDR_XALIGN_P1of4		0x1100
//#define NSKT_NULL						0x0000
#define NSKT_WORLDR_XALIGN_M1of4		0x1800
#define NSKT_WORLDR_XALIGN_M1of3		0x1900
#define NSKT_WORLDR_XALIGN_M1of2		0x1A00
#define NSKT_WORLDR_XALIGN_M1			0x1B00
#define NSKT_WORLDR_XALIGN_M2			0x1C00
#define NSKT_WORLDR_XALIGN_M3			0x1D00
#define NSKT_WORLDR_XALIGN_M4			0x1E00

#define NSKT_WORLDR_YALIGN_P4			0x1070
#define NSKT_WORLDR_YALIGN_P3			0x1060
#define NSKT_WORLDR_YALIGN_P2			0x1050
#define NSKT_WORLDR_YALIGN_P1			0x1040
#define NSKT_WORLDR_YALIGN_P1of2		0x1030
#define NSKT_WORLDR_YALIGN_P1of3		0x1020
#define NSKT_WORLDR_YALIGN_P1of4		0x1010
//#define NSKT_NULL						0x0000
#define NSKT_WORLDR_YALIGN_M1of4		0x1080
#define NSKT_WORLDR_YALIGN_M1of3		0x1090
#define NSKT_WORLDR_YALIGN_M1of2		0x10A0
#define NSKT_WORLDR_YALIGN_M1			0x10B0
#define NSKT_WORLDR_YALIGN_M2			0x10C0
#define NSKT_WORLDR_YALIGN_M3			0x10D0
#define NSKT_WORLDR_YALIGN_M4			0x10E0

#define NSKT_WORLDR_ZALIGN_P4			0x1007
#define NSKT_WORLDR_ZALIGN_P3			0x1006
#define NSKT_WORLDR_ZALIGN_P2			0x1005
#define NSKT_WORLDR_ZALIGN_P1			0x1004
#define NSKT_WORLDR_ZALIGN_P1of2		0x1003
#define NSKT_WORLDR_ZALIGN_P1of3		0x1002
#define NSKT_WORLDR_ZALIGN_P1of4		0x1001
//#define NSKT_NULL						0x0000
#define NSKT_WORLDR_ZALIGN_M1of4		0x1008
#define NSKT_WORLDR_ZALIGN_M1of3		0x1009
#define NSKT_WORLDR_ZALIGN_M1of2		0x100A
#define NSKT_WORLDR_ZALIGN_M1			0x100B
#define NSKT_WORLDR_ZALIGN_M2			0x100C
#define NSKT_WORLDR_ZALIGN_M3			0x100D
#define NSKT_WORLDR_ZALIGN_M4			0x100E
*/



typedef struct  
{
	Nf32		x;
	Nf32		y;
	Nf32		z;
	Nu32		TBuildCode;
}NSPLINEKNOTDESC;
// ... To be used like this:
// NSPLINEKNOTDESC	description_list[]	=	{
// 												{x,y,z,tangents_buildcode},
// 												{1,4,7,NSKTANGENTSPACKI(NSKT_3DWORLD_RELATIVE,NSKT0,NSKT0,NSKT0,NSKT_3DWORLD_RELATIVE,NSKT_P1,NSKT0,NSKT0)},
// 												{1,4,7,NSKTANGENTSPACK(NSKT_NULL,NSKT_WORLDR_XALIGN_P1of3)},
// 												{1,4,7,0x02340AAB},
// 												...
// 											};

// for each NSPLINEKNOT, BIT_0 to BIT_7 are are system Reserved !
// BIT_8 to BIT_15 are user available.
#define FLAG_NSPLINEKNOT_NUT_SPACK_CONSTRAINT_LOCK			BIT_0 // Used by NUT_SPLINESPACK_TOOL ...  ! 
		
typedef struct  
{
	NVEC3	Position;
	NVEC3	TA;
	NVEC3	TB;

	Nu16		Flags;
	// --------------------------
	Nu16		User_16;
	Nu32		A_User_32;	// |_ TODO: Remove one of the 2 and rename it as User_32 ! 
	Nu32		B_User_32;	// |
}NSPLINEKNOT;

typedef struct  
{
	Nu32		Flags;
	Nu32		BezierAccuracy;	// Accuracy of each subSpline,... for this spline. Spline Accuracy represents the number of segments between each SplineKnot
	NARRAY		KnotArray;
}NSPLINE;
// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NSPLINE*			NSetupSpline(NSPLINE *pspline,const Nu32 accuracy,const Nu32 splinepoint_nb,const NSPLINEKNOTDESC *pdesclist);
NSPLINE*			NCreateSpline(const Nu32 accuracy,const Nu32 splinepoint_nb,const NSPLINEKNOTDESC *pdesclist);
void				NClearSpline(NSPLINE* pspline);
void				NDeleteSpline(NSPLINE *pspline);

void				NCopySpline(NSPLINE *pdst, const NSPLINE*psrc);

inline Nu32			NGetBezierAccuracy(const NSPLINE *pspline){return pspline->BezierAccuracy;}
inline void			NSetBezierAccuracy(NSPLINE *pspline,const Nu32 accuracy){pspline->BezierAccuracy = accuracy;}
inline NSPLINEKNOT*	NInsertSplineKnot(NSPLINE *pspline, const Nu32 refindex, const NSPLINEKNOT *ppoint){ return (NSPLINEKNOT*)NInsertArrayElement(&pspline->KnotArray,refindex,(const NBYTE *)ppoint);}
inline Nbool		NIsValidSplineKnot(const NSPLINE *pspline, const NSPLINEKNOT *psp){return NIsValidArrayElementPtr(&pspline->KnotArray,(const NBYTE*)psp);}
inline void			NEraseSplineKnot(NSPLINE *pspline, const Nu32 index){NEraseArrayElement(&pspline->KnotArray,index,NULL);}
inline NSPLINEKNOT*	NGetSplineKnot(NSPLINE *pspline, const Nu32 index){return (NSPLINEKNOT*)NGetArrayPtr(&pspline->KnotArray,index);}
inline NSPLINEKNOT*	NGetFirstSplineKnot(const NSPLINE *pspline){return (NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);}
inline NSPLINEKNOT*	NGetLastSplineKnot(const NSPLINE *pspline){return (NSPLINEKNOT*)NGetLastArrayPtr(&pspline->KnotArray);}
inline Nu32			NGetSplineKnotsNumber(const NSPLINE *pspline){return NGetArraySize(&pspline->KnotArray);}
void				NSetAllSplineKnotTangents(NSPLINE *pspline,const Nu32 tangents_code);
void				NSetBezierTangents(NSPLINEKNOT	*psk0,NSPLINEKNOT	*psk1, const Nu32 tangents_code);

Nf32				NSplineLength( const NSPLINE *pspline);
Nf32				NBezierLength(const NSPLINEKNOT *A, const NSPLINEKNOT*B, const Nu32 accuracy);
Nf32				NCasteljauLength(const NSPLINEKNOT *A, const NSPLINEKNOT*B, const Nu32 accuracy);

void				NSplinePointD3f32(NVEC3 *R, const Nf32 extended_factor, const NSPLINE *pspline );

//inline void			NCasteljauPoint3f32(NVEC3 *R, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B ){NVec3QLerp(R,factor, &A->Position,&A->TB,&B->TA,&B->Position);}
//inline void			N2DSplineQLerp(NVEC2 *R, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B ){NVec2QLerp(R,factor,(NVEC2*)&A->Position,(NVEC2*)&A->TB,(NVEC2*)&B->TA,(NVEC2*)&B->Position);}
//void				NSplineQLerpX(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B );

// GET POINT FROM A SPLINE ( from a bezier curve )
//------------------------------------------------- 
// De Casteljau algorythms
// Give same results as Bezier functions but in a different way, more stable with a less Error level. 
// BUT THEY ARE 2 TIMES SLOWER THAN THE BEZIER FUNCTION IMPLEMENTATION...  
//inline void			NCasteljauPoint3f32(NVEC3 *R, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NCasteljauTangent3f32(NVEC3 *T, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NCasteljauXtdPoint3f32(NVEC3 *R, const Nf32 factor, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline void			NCasteljauXtdTangent3f32(NVEC3 *T, const Nf32 factor, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)

// Bezier standart Cubic bezier cubic curve algorythms 
//inline void			NBezierPoint3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NBezierTangent3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NBezierDerivative3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NBezierSecondDerivative3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline Nf32			NBezierCurvaturef32( const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline Nf32			NBezierCurvatureRadiusf32(const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
//inline void			NBezierCurvatureRadius2f32(NVEC2f32 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)

//inline void			NBezierXtdPoint3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline void			NBezierXtdTangent3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline void			NBezierXtdDerivative3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline void			NBezierXtdSecondDerivative3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline Nf32			NBezierXtdCurvaturef32(const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline Nf32			NBezierXtdCurvatureRadiusf32(const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
//inline void			NBezierXtdCurvatureRadius2f32(NVEC2f32 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)	

// De Casteljau algorythms
inline void			NCasteljauPoint3f32(NVEC3 *R, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B )
{
	NVEC3	L1,L2,L3,L4,L5;
	NVec3Lerp(&L1,&A->Position,&A->TB,factor); NVec3Lerp(&L2,&A->TB,&B->TA,factor); NVec3Lerp(&L3,&B->TA,&B->Position,factor);	
	NVec3Lerp(&L4,&L1,&L2,factor); NVec3Lerp(&L5,&L2,&L3,factor); NVec3Lerp(R,&L4,&L5,factor);
}

inline void			NCasteljauTangent3f32(NVEC3 *T, const Nf32 factor, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	NVEC3	L1, L2, L3, L4, L5;

	NVec3Lerp(&L1, &A->Position, &A->TB, factor); NVec3Lerp(&L2, &A->TB, &B->TA, factor); NVec3Lerp(&L3, &B->TA, &B->Position, factor);
	NVec3Lerp(&L4, &L1, &L2, factor); NVec3Lerp(&L5, &L2, &L3, factor);
	NVec3Sub(T, &L5, &L4);
}

inline	void		NCasteljauXtdPoint3f32(NVEC3 *R, const Nf32 factor, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D )
{
	NVEC3	L1,L2,L3,L4,L5;
	NVec3Lerp(&L1,A,B,factor); NVec3Lerp(&L2,B,C,factor); NVec3Lerp(&L3,C,D,factor);
	NVec3Lerp(&L4,&L1,&L2,factor); NVec3Lerp(&L5,&L2,&L3,factor); NVec3Lerp(R,&L4,&L5,factor);
}

inline void			NCasteljauXtdTangent3f32(NVEC3 *T, const Nf32 factor, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	NVEC3	L1, L2, L3, L4, L5;

	NVec3Lerp(&L1, A, B, factor); NVec3Lerp(&L2, B, C, factor); NVec3Lerp(&L3, C, D, factor);
	NVec3Lerp(&L4, &L1, &L2, factor); NVec3Lerp(&L5, &L2, &L3, factor);
	NVec3Sub(T, &L5, &L4);
}
// Standard Bezier cubic curve optimized algorythm

inline void			NBezierPoint3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	Nf32	omt = 1.0f - t;	Nf32	omt2 = omt * omt; Nf32	omt3 = omt2 * omt;
	Nf32	t2 = t * t; Nf32	t3 = t2 * t;

	//	R->x =	A->Position.x * omt3 + A->TB.x * (3.0f * omt2 * t) + B->TA.x * (3.0f * omt * t2) + B->Position.x * t3;
	//	R->y =	A->Position.y * omt3 + A->TB.y * (3.0f * omt2 * t) + B->TA.y * (3.0f * omt * t2) + B->Position.y * t3;
	//	R->z =	A->Position.z * omt3 + A->TB.z * (3.0f * omt2 * t) + B->TA.z * (3.0f * omt * t2) + B->Position.z * t3;

	Nf32	omt2_3t = 3.0f * omt2 * t;
	Nf32	omt_3t2 = 3.0f * omt * t2;

	R->x = A->Position.x * omt3 + A->TB.x * omt2_3t + B->TA.x * omt_3t2 + B->Position.x * t3;
	R->y = A->Position.y * omt3 + A->TB.y * omt2_3t + B->TA.y * omt_3t2 + B->Position.y * t3;
	R->z = A->Position.z * omt3 + A->TB.z * omt2_3t + B->TA.z * omt_3t2 + B->Position.z * t3;

}
inline	void		NBezierXtdPoint3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D )
{
	Nf32	omt	= 1.0f - t;	Nf32	omt2	= omt	* omt; Nf32	omt3	= omt2 * omt;
	Nf32	t2	= t * t; Nf32	t3	= t2 * t;

//	R->x =	A->x * omt3 + B->x * (3.0f * omt2 * t) + C->x * (3.0f * omt * t2) + D->x * t3;
//	R->y =	A->y * omt3 + B->y * (3.0f * omt2 * t) + C->y * (3.0f * omt * t2) + D->y * t3;
//	R->z =	A->z * omt3 + B->z * (3.0f * omt2 * t) + C->z * (3.0f * omt * t2) + D->z * t3;

	Nf32	omt2_3t = 3.0f * omt2 * t;
	Nf32	omt_3t2 = 3.0f * omt * t2;

	R->x =	A->x * omt3 + B->x * omt2_3t + C->x * omt_3t2 + D->x * t3;
	R->y =	A->y * omt3 + B->y * omt2_3t + C->y * omt_3t2 + D->y * t3;
	R->z =	A->z * omt3 + B->z * omt2_3t + C->z * omt_3t2 + D->z * t3;

}

inline void			NBezierTangent3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	Nf32	omt		= 1.0f - t;	
	Nf32	omt2	= omt * omt;
	Nf32	t2		= t * t;
	
//	R->x = A->Position.x * (-omt2) + A->TB.x * (3.0f * omt2 - 2.0f * omt) + B->TA.x * (-3.0f * t2 + 2 * t) + B->Position.x * t2;
//	R->y = A->Position.y * (-omt2) + A->TB.y * (3.0f * omt2 - 2.0f * omt) + B->TA.y * (-3.0f * t2 + 2 * t) + B->Position.y * t2;
//	R->z = A->Position.z * (-omt2) + A->TB.z * (3.0f * omt2 - 2.0f * omt) + B->TA.z * (-3.0f * t2 + 2 * t) + B->Position.z * t2;

	Nf32	u		= 3.0f * omt2 - 2.0f * omt;
	Nf32	v		= -3.0f * t2 + 2 * t;
	R->x = A->Position.x * (-omt2) + A->TB.x * u + B->TA.x * v + B->Position.x * t2;
	R->y = A->Position.y * (-omt2) + A->TB.y * u + B->TA.y * v + B->Position.y * t2;
	R->z = A->Position.z * (-omt2) + A->TB.z * u + B->TA.z * v + B->Position.z * t2;

}
inline void			NBezierXtdTangent3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	Nf32	omt		= 1.0f - t;	
	Nf32	omt2	= omt * omt;
	Nf32	t2		= t * t;

	//	R->x = A->x * (-omt2) + B->x * (3.0f * omt2 - 2.0f * omt) + C->x * (-3.0f * t2 + 2 * t) + D->x * t2;
	//	R->y = A->y * (-omt2) + B->y * (3.0f * omt2 - 2.0f * omt) + C->y * (-3.0f * t2 + 2 * t) + D->y * t2;
	//	R->z = A->z * (-omt2) + B->z * (3.0f * omt2 - 2.0f * omt) + C->z * (-3.0f * t2 + 2 * t) + D->z * t2;

	Nf32 u			= 3.0f * omt2 - 2.0f * omt;
	Nf32 v			= -3.0f * t2 + 2 * t;

	R->x = A->x * (-omt2) + B->x * u + C->x * v + D->x * t2;
	R->y = A->y * (-omt2) + B->y * u + C->y * v + D->y * t2;
	R->z = A->z * (-omt2) + B->z * u + C->z * v + D->z * t2;

}

inline void			NBezierDerivative3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{

	Nf32	_omt	= 1.0f - t;
	Nf32	_3omt2	= 3.0f * _omt * _omt;
	Nf32	_6omtt	= 6.0f * _omt * t;
	Nf32	_3t2	= 3.0f* t * t;

	R->x = ( A->TB.x - A->Position.x ) * _3omt2 + ( B->TA.x - A->TB.x ) * _6omtt + ( B->Position.x - B->TA.x ) * _3t2;
	R->y = ( A->TB.y - A->Position.y ) * _3omt2 + ( B->TA.y - A->TB.y ) * _6omtt + ( B->Position.y - B->TA.y ) * _3t2;
	R->z = ( A->TB.z - A->Position.z ) * _3omt2 + ( B->TA.z - A->TB.z ) * _6omtt + ( B->Position.z - B->TA.z ) * _3t2;
}
inline void			NBezierXtdDerivative3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{

	Nf32	_omt = 1.0f - t;
	Nf32	_3omt2 = 3.0f * _omt * _omt;
	Nf32	_6omtt = 6.0f * _omt * t;
	Nf32	_3t2 = 3.0f* t * t;

	R->y = (B->y - A->y) * _3omt2 + (C->y - B->y) * _6omtt + (D->y - C->y) * _3t2;
	R->x = (B->x - A->x) * _3omt2 + (C->x - B->x) * _6omtt + (D->x - C->x) * _3t2;
	R->z = (B->z - A->z) * _3omt2 + (C->z - B->z) * _6omtt + (D->z - C->z) * _3t2;
}

inline void			NBezierSecondDerivative3f32(NVEC3 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	/*
	// Native version:
	Nf32	a = 6.0f*(1.0f - t);
	Nf32	b = 6.0f * t;

	R->x = (B->TA.x - 2.0f * A->TB.x + A->Position.x) * a + (B->Position.x - 2.0f * B->TA.x + A->TB.x) * b;
	R->y = (B->TA.y - 2.0f * A->TB.y + A->Position.y) * a + (B->Position.y - 2.0f * B->TA.y + A->TB.y) * b;
	R->z = (B->TA.z - 2.0f * A->TB.z + A->Position.z) * a + (B->Position.z - 2.0f * B->TA.z + A->TB.z) * b;
	*/

	// Optimized version
	Nf32	a = 1.0f - t;
	Nf32	b = 3.0f * t - 2.0f;
	Nf32	c = 1.0f - 3.0f * t;

	R->x = 6.0f * (A->Position.x * a + A->TB.x * b + B->TA.x * c + B->Position.x * t);
	R->y = 6.0f * (A->Position.y * a + A->TB.y * b + B->TA.y * c + B->Position.y * t);
	R->z = 6.0f * (A->Position.z * a + A->TB.z * b + B->TA.z * c + B->Position.z * t);
}
inline void			NBezierXtdSecondDerivative3f32(NVEC3 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	/*
	// Native version:
	Nf32	a = 6.0f*(1.0f - t);
	Nf32	b = 6.0f * t;

	R->x = (B->TA.x - 2.0f * A->TB.x + A->Position.x) * a + (B->Position.x - 2.0f * B->TA.x + A->TB.x) * b;
	R->y = (B->TA.y - 2.0f * A->TB.y + A->Position.y) * a + (B->Position.y - 2.0f * B->TA.y + A->TB.y) * b;
	R->z = (B->TA.z - 2.0f * A->TB.z + A->Position.z) * a + (B->Position.z - 2.0f * B->TA.z + A->TB.z) * b;
	*/

	// Optimized version
	Nf32	a = 1.0f - t;
	Nf32	b = 3.0f * t - 2.0f;
	Nf32	c = 1.0f - 3.0f * t;

	R->x = 6.0f * (A->x * a + B->x * b + C->x * c + D->x * t);
	R->y = 6.0f * (A->y * a + B->y * b + C->y * c + D->y * t);
	R->z = 6.0f * (A->z * a + B->z * b + C->z * c + D->z * t);
}

inline Nf32	NBezierCurvaturef32( const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	NVEC3f32	d, dd;

	NBezierDerivative3f32(&d, t, A, B);
	NBezierSecondDerivative3f32(&dd, t, A, B);

	Nf32 ds = d.x * d.x + d.y * d.y;

	return ( (d.x * dd.y - dd.x * d.y) / ( ds * sqrt(ds) ) );
}
inline Nf32	NBezierXtdCurvaturef32(const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	NVEC3f32	d, dd;

	NBezierXtdDerivative3f32(&d, t, A, B, C, D);
	NBezierXtdSecondDerivative3f32(&dd, t, A, B, C, D);

	Nf32 ds = d.x * d.x + d.y * d.y;

	return ((d.x * dd.y - dd.x * d.y) / (ds * sqrt(ds)));
}
inline Nf32	NBezierCurvatureRadiusf32(const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	NVEC3f32	d, dd;

	NBezierDerivative3f32(&d, t, A, B);
	NBezierSecondDerivative3f32(&dd, t, A, B);

	Nf32 ds = d.x * d.x + d.y * d.y;

	return ( (ds * sqrt(ds)) / (d.x * dd.y - dd.x * d.y) );
}
inline Nf32	NBezierXtdCurvatureRadiusf32(const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	NVEC3f32	d, dd;

	NBezierXtdDerivative3f32(&d, t, A, B, C, D);
	NBezierXtdSecondDerivative3f32(&dd, t, A, B, C, D);

	Nf32 ds = d.x * d.x + d.y * d.y;

	return ((ds * sqrt(ds)) / (d.x * dd.y - dd.x * d.y));
}

inline void NBezierCurvatureRadius2f32(NVEC2f32 *R, const Nf32 t, const NSPLINEKNOT *A, const NSPLINEKNOT *B)
{
	NVEC3f32	d, dd;

	NBezierDerivative3f32(&d, t, A, B);
	NBezierSecondDerivative3f32(&dd, t, A, B);


	//  Le "vecteur Rayon" est le vecteur Normal à la tangente ( connue grace à la derivée  d ) de longeur R.
	//	Pour obtenir N on part du vecteur (d.x,d.y) qu'on normalize, donc qu'on divise par sqrt( ds ) avec ds =  dx*dx + dy*dy
	//	soit: 
	//			N.x = -d.y / sqrt( ds )
	//			N.y = d.x / sqrt( ds )
	//	ensuite on multiplie N par la valeur algebrique du rayon de courbure.
	//
	//			N.x *= ((ds * sqrt(ds)) / (d.x * dd.y - dd.x * d.y));
	//			N.y *= ((ds * sqrt(ds)) / (d.x * dd.y - dd.x * d.y));
	//
	// On remarque qu'on divise N par sqrt(ds) pour ensuite le multiplier par sqrt(ds)....
	// On peut optimiser en supprimant la racine carrée au numerateur et la racine carrée au denominateur !
	// Le calcul  devient donc:	

	Nf32 ds = d.x * d.x + d.y * d.y;
	Nf32 f = (ds  / (d.x * dd.y - dd.x * d.y)); // f = r / sqrt( ds )  
												// avec The algebric radius r = ((ds * sqrt(ds)) / (d.x * dd.y - dd.x * d.y)); 
	R->x = -d.y * f;
	R->y =  d.x * f;
}
inline void NBezierXtdCurvatureRadius2f32(NVEC2f32 *R, const Nf32 t, const NVEC3 *A, const NVEC3 *B, const NVEC3 *C, const NVEC3 *D)
{
	NVEC3f32	d, dd;

	NBezierXtdDerivative3f32(&d, t, A, B, C, D);
	NBezierXtdSecondDerivative3f32(&dd, t, A, B, C, D);

	Nf32 ds = d.x * d.x + d.y * d.y;
	Nf32 f = (ds / (d.x * dd.y - dd.x * d.y));
	R->x = -d.y * f;
	R->y = d.x * f;
}


inline	void		NShiftSplineKnot(NSPLINEKNOT *psp,const NVEC3 *pv){NVec3AddTo(&psp->Position,pv);NVec3AddTo(&psp->TA,pv);NVec3AddTo(&psp->TB,pv);}
inline	void		NShiftSplineKnotf(NSPLINEKNOT *psp, const Nf32 x, const Nf32 y, const Nf32 z){NVEC3 v; NVec3Set(&v,x,y,z);NShiftSplineKnot(psp,&v);}
inline	void		NShiftSpline(NSPLINE *pspline,const NVEC3 *pv){Nu32 i;NSPLINEKNOT *psk; psk=(NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray); for(i=0;i<NGetArraySize(&pspline->KnotArray);i++,psk++){NShiftSplineKnot(psk,pv);} }
inline	void		NShiftSplinef(NSPLINE *pspline,const Nf32 x,const Nf32 y,const Nf32 z){Nu32 i;NSPLINEKNOT *psk;NVEC3 v; NVec3Set(&v,x,y,z); psk=(NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray); for(i=0;i<NGetArraySize(&pspline->KnotArray);i++,psk++){NShiftSplineKnot(psk,&v);} }

void				NScaleSplineKnot(NSPLINEKNOT *psp, const NVEC3 *pscale, const NVEC3 *porigin);
void				NScaleSpline(NSPLINE *ps, const NVEC3 *pscale, const NVEC3 *porigin);

// The different Spline Factors.
// All of them represent the same thing: a Lerp factor between 2 SplineKnots. But, because a spline has more than only 2 Bezier knots ...
// ... the extended versions of the initial simple "Factor" propose a way to integrate, and recover, the 2 spline knots too, 
// all packed together into the same Nf32.
//
// Factor			Works with 2 Knots Bezier ONLY. This is the basis.			
//					0 <= Factor	<= 1	
//					This is the base, needs the 2 knots.
//
// Factor_Xtd		Works with n Knots Spline. id of the first SplineKnot is the integer part.
//					0 <= Factor_Xtd <= KnotArray.Size-1
//					High Level Factor.It's easy to retrieve basic Factor and the 2 knots from it.
//
// Factor_NormXtd	Works with n Knots Spline. Its a Factor_Xtd divide by (KnotArray.Size-1).
//					0 <= Factor_NormXtd <= 1
//					High level factor.It's easy to retrieve basic Factor and the 2 knots from it.
//					It takes one more step than the Factor_Xtd but it's not dependent to the spline knots number.
inline Nf32			NSplineFactor_FactorToXtd(const NSPLINEKNOT *pfirstlerpknot,const Nf32 factor,const NSPLINE *pspline)
{
	// NErrorIf(!NIsValidArrayElementPtr(&pspline->KnotArray,(NBYTE*)pfirstlerpknot),NERROR_SPLINE_INVALID_SPLINEKNOT);
	// ... its not necessary to perform this debug test because it will be done inside the function 'NGetArrayIndex' called just
	// few lines later.
	NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	NErrorIf(factor<0.0f||factor>1.0f,NERROR_VALUE_OUTOFRANGE);
	return (Nf32)NGetArrayIndex(&pspline->KnotArray,(NBYTE*)pfirstlerpknot)+factor;
}
inline Nf32			NSplineFactor_FactorToNormXtd(const NSPLINEKNOT *pfirstlerpknot,const Nf32 factor,const NSPLINE *pspline)
{
	// NErrorIf(!NIsValidArrayElementPtr(&pspline->KnotArray,(NBYTE*)pfirstlerpknot),NERROR_SPLINE_INVALID_SPLINEKNOT);
	// ... its not necessary to perform this debug test because it will be done inside the function 'NGetArrayIndex' called just
	// few lines later.
	NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	NErrorIf(factor<0.0f||factor>1.0f,NERROR_VALUE_OUTOFRANGE);
	return ((Nf32)NGetArrayIndex(&pspline->KnotArray,(NBYTE*)pfirstlerpknot)+factor)/(Nf32)(pspline->KnotArray.Size-1);
}

inline Nf32			NSplineFactor_XtdToNormXtd(const Nf32 xtd_factor,const NSPLINE *pspline){NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);NErrorIf(xtd_factor<0.0f||xtd_factor>(Nf32)(pspline->KnotArray.Size-1),NERROR_VALUE_OUTOFRANGE);return xtd_factor/(Nf32)(pspline->KnotArray.Size-1);}
inline Nf32			NSplineFactor_XtdToFactor(NSPLINEKNOT *pfirstlerpknot, const Nf32 xtd_factor,const NSPLINE *pspline)
{
	NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	NErrorIf(xtd_factor<0.0f||xtd_factor>(Nf32)(pspline->KnotArray.Size-1),NERROR_VALUE_OUTOFRANGE);
	pfirstlerpknot = NGetFirstSplineKnot(pspline) + (Nu32)xtd_factor;
	return floor(xtd_factor);
}

inline Nf32			NSplineFactor_NormXtdToXtd(const Nf32 normxtd_factor,const NSPLINE *pspline){NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);NErrorIf(normxtd_factor<0.0f||normxtd_factor>1.0f,NERROR_VALUE_OUTOFRANGE);return normxtd_factor*(Nf32)(pspline->KnotArray.Size-1);}
inline Nf32			NSplineFactor_NormXtdToFactor(NSPLINEKNOT *pfirstlerpknot, const Nf32 normxtd_factor,const NSPLINE *pspline)
{
	NErrorIf(!pspline->KnotArray.Size,NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	NErrorIf(normxtd_factor<0.0f||normxtd_factor>1.0f,NERROR_VALUE_OUTOFRANGE);
	Nf32 xtd_factor = normxtd_factor*(Nf32)(pspline->KnotArray.Size-1);
	pfirstlerpknot = (NSPLINEKNOT*)pspline->KnotArray.pFirst + (Nu32)xtd_factor;
	return floor(xtd_factor);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSPLINE_H 

