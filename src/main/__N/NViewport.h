#ifndef __NVIEWPORT_H
#define __NVIEWPORT_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									Niewport.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Structures & Defines									**
// ***************************************************************************************
#define NVIEWPORT_REFERENCEMODE_DIAGONAL		0
#define NVIEWPORT_REFERENCEMODE_WIDTH			1
#define NVIEWPORT_REFERENCEMODE_HEIGHT			2
#define NVIEWPORT_REFERENCEMODE_AUTO			3

typedef struct  
{
	Nu32	Width;
	Nu32	Height;

	Nf32	InvWidth;
	Nf32	InvHeight;

	Nf32	WidthOutOf2;
	Nf32	HeightOutOf2;
	
//	Nf32	Diagonal;
	Nf32	AspectRatio;//RatioWidth_Height;
	Nf32	InvAspectRatio;//InvRatioWidth_Height;

	// Reference ViewPort ( especially for UI building on different screen size )
	Nu8		RefMode;
	Nu8		Available_Nu8;
	Nu16	Available_Nu16;
	Nu32	RefWidth;
	Nu32	RefHeight;
//	Nf32	RefDiagonal;

	Nf32	RefRatio;			// RefRatio			= RefDiagonal/Diagonal								
	Nf32	RefRatio_Height;	// RefRatio_Height	= RefRatio / Height = RefDiagonal/(Diagonal*Height) 
	Nf32	RefRatio_Width;		// RefRatio_Width	= RefRatio / Width	= RefDiagonal/(Diagonal*Width)	


/*
	Nf32	RefNRatioX;			// RefNRatioX	= Width*RefDiagonal/Diagonal										"fPixX	= RefNormX x RefNRatioX"
	Nf32	RefNRatioY;			// RefNRatioY	= Height*RefDiagonal/Diagonal										"fPixY	= RefNormY x RefNRatioY"									
	Nf32	RefNRatioX_Height;	// RefNRatioX_Height = Width*RefDiagonal/(Diagonal*Height)							"HRelX	= RefNormX x RefNRatioX_Height"
	Nf32	RefNRatioY_Height;	// RefNRatioX_Height = Height*RefDiagonal/(Diagonal*Height) = RefDiagonal/Diagonal 	"HRelY	= RefNormY x RefNRatioY_Height"
*/
}NVIEWPORT;

// Global variable
extern NVIEWPORT	NViewPort;
// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
void		NInitializeViewPortConstants();
void		NUpdateViewPortConstants();
void		NSetViewPortReferenceScreenSize(const Nu32 ref_width, const Nu32 ref_height);
void		NSetViewPortReferenceMode(const Nu8 refmode);
void		NSetViewPortReferenceParams(const Nu32 ref_width, const Nu32 ref_height,const Nu8 refmode);

inline Nu32	NGetViewPort_Width(){return NViewPort.Width; };
inline Nu32 NGetViewPort_Height(){return NViewPort.Height;};

inline Nf32 NGetViewPort_InvWidth(){return NViewPort.InvWidth;};
inline Nf32 NGetViewPort_InvHeight(){return NViewPort.InvHeight;};

inline Nf32 NGetViewPort_WidthOutOf2(){return NViewPort.WidthOutOf2;};
inline Nf32 NGetViewPort_HeightOutOf2(){return NViewPort.HeightOutOf2;};

/*inline Nf32 NGetViewPort_Diagonal(){return NViewPort.Diagonal;};*/
inline Nf32 NGetViewPort_AspectRatio(){return NViewPort.AspectRatio;};
inline Nf32 NGetViewPort_InvAspectRatio(){return NViewPort.InvAspectRatio;};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NVIEWPORT_H 

