#ifndef __NUT_2DAACOLORCAGE_H
#define __NUT_2DAACOLORCAGE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_2DAAColorCage.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NUsualCoordinates.h"
#include "../Render/Renderable/NRenderable.h"

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

// +++ DEFAULT COLOR at SetUp.
#define DEFAULT_NUT_2DAACOLORCAGE_INITIAL_COLOR4F				NCOLOR_PRESET3F_GRAY,1
typedef struct 
{
	Nu16				SegmentNb[2];
	NVEC3			Origin;			// Bottom,Left corner of the Cage (Grid)			
	NVEC3			PlaneDir;
	NVEC2			Size;
	//NVEC2			InvSubSize;
	NV3COORD_ENUM		Is_X;
	NV3COORD_ENUM		Is_Y;
	
	NARRAY				ColorArray;
}NUT_2DAACOLORCAGE;

#define FLAG_NUT_2DAACOLORCAGE_DESC_USEPARAMS_INITIAL_COLOR4F		BIT_0
// #define FLAG_NUT_COLORCAGE_DESC_								BIT_1
// #define FLAG_NUT_COLORCAGE_DESC_								BIT_2
// #define FLAG_NUT_COLORCAGE_DESC_								BIT_3
//#define FLAG_NUT_COLORCAGE_DESC_								BIT_4
//#define FLAG_NUT_COLORCAGE_DESC_								BIT_5
//#define FLAG_NUT_COLORCAGE_DESC_								BIT_6
//#define FLAG_NUT_COLORCAGE_DESC_								BIT_7

typedef struct  
{
	Nu16			Flags;
	Nu16			SegmentNb[2];	// One segment connect 2 ColorKnots. So for a specific Dimension: "ColorKnotNb[dimension] = SegmentNb[dimension]+1"

	NAAPLANE_ENUM	ConstructionPlane;
	NVEC3		Origin;
	NVEC2		Size;	
	
	// ColorCage Color Initialization (Used Only if FLAG_NUT_COLORCAGE_DESC_USEPARAMS_INITIALCOLOR is set to ON )
	void				*pInitialColor;
	Nu32				InitialColorStride;
	Nu32				InitialColorNumber;
	NCOLOR_FORMAT_ENUM	InitialColorFormat;
}NUT_2DAACOLORCAGE_DESC;

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
NUT_2DAACOLORCAGE*			NUT_SetUp_2DAAColorCage(NUT_2DAACOLORCAGE *pcc, const NUT_2DAACOLORCAGE_DESC *paacc_desc);
NUT_2DAACOLORCAGE*			NUT_Create_2DAAColorCage(const NUT_2DAACOLORCAGE_DESC *paacc_desc);
void						NUT_CLear_2DAAColorCage(void *ptr);
void						NUT_Delete_2DAAColorCage(NUT_2DAACOLORCAGE * pcc);

inline void					NUT_Set2DAAColorCage_Origin(NUT_2DAACOLORCAGE *pcc,const NVEC3 *porigin){pcc->Origin = *porigin;}
inline void					NUT_Set2DAAColorCage_Size(NUT_2DAACOLORCAGE *pcc,const NVEC2 *psize){pcc->Size = *psize;}

void						NUT_Set_2DAAColorCage_KnotColorRange(NUT_2DAACOLORCAGE *pcc, const Nu32 first_knotindex, const void *pcol, const Nu32 incolor_stride, const Nu32 incolor_nb, const NCOLOR_FORMAT_ENUM incolor_format);

Nu32						NUT_Pick_2DAAColorCage_Knot(const NUT_2DAACOLORCAGE *pcc, const Nf32 pickradius, const NVEC2s16 *ptouchpos);
inline void					NUT_Get2DAAColorCage_KnotColor(NCOLOR *pcolor, const NUT_2DAACOLORCAGE *pcc,const Nu32 id){*pcolor = *(NCOLOR*)NGetArrayPtr(&pcc->ColorArray,id);}
inline void					NUT_Set2DAAColorCage_KnotColor(NUT_2DAACOLORCAGE *pcc,const Nu32 id,const NCOLOR *pcolor){*(NCOLOR*)NGetArrayPtr(&pcc->ColorArray,id) = *pcolor;}

void						NUT_Apply_2DAAColorCage(NCOLOR *pfirstcolor, const Nu32 colorstride, const NVEC3 *pfirstpoint, const Nu32 pointstride, const Nu32 size, const NUT_2DAACOLORCAGE *pcc);
void						NUT_Apply_2DAAColorCage_To_RenderableGeometries(NRENDERABLE *prenderable, const NUT_2DAACOLORCAGE *pcc);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_2DAACOLORCAGE_H
