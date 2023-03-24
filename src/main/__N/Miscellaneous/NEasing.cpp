// ==========================================================================================
// N2DPointInside.cpp
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 05/04/2017
// Updated : 
// ------------------------------------------------------------------------------------------
// 
// ==========================================================================================
#include "../NMath.h"
#include "NEasing.h"

Nf32 NEasing_Linear(const Nf32 normalized_ratio)
{
	return normalized_ratio;
}
Nf32 NEasing_Out(const Nf32 normalized_ratio)
{
	return (1.0f + cos( NDEGtoRAD(180.0f)+normalized_ratio*NDEGtoRAD(90.0f) ));
}
Nf32 NEasing_In(const Nf32 normalized_ratio)
{
	return sin( normalized_ratio*NDEGtoRAD(90.0f) );
}
Nf32 NEasing_InOut(const Nf32 normalized_ratio)
{
	return (sin( NDEGtoRAD(270.0f)+normalized_ratio*NDEGtoRAD(180.0f) )+1.0f)*0.5f;
}
Nf32 NEasing_Over(const Nf32 normalized_ratio)
{
	return (normalized_ratio + sin( normalized_ratio*NDEGtoRAD(180.0f) ));
}
Nf32 NEasing_Under(const Nf32 normalized_ratio)
{
	return (normalized_ratio - sin( normalized_ratio*NDEGtoRAD(180.0f) ));
}
Nf32 NEasing_SmoothOver(const Nf32 normalized_ratio)
{
	return (normalized_ratio + (1.0f-cos( normalized_ratio*NDEGtoRAD(360.0f) ))*0.5f);
}
Nf32 NEasing_SmoothUnder(const Nf32 normalized_ratio)
{
	return (normalized_ratio + (cos( normalized_ratio*NDEGtoRAD(360.0f) )-1.0f)*0.5f);
}

/*
Nf32 NEasing_OverIn(const Nf32 normalized_ratio)
{
	return (cos( normalized_ratio*NDEGtoRAD(540.0f) )-1.0f);
}
Nf32 NEasing_OverOut(const Nf32 normalized_ratio)
{
	return (1.0f-cos( normalized_ratio*NDEGtoRAD(540.0f) ));
}*/