#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../NViewport.h"
#include "../NErrorHandling.h"
#include "../Utilities/NUT_Shape.h"
#include "../NCore.h"
#include "../Geometry/NGeometryMisc.h"

extern NGEOMETRY_SPECS	NGeometrySpecs[];
// N GLobals
NGEOMETRY					*pNClearSurfaceGeometry;	// Single Geometry used as Background (should be a 1x1 XY surface).
NCOLOR						NClearSurfaceColorModulation;

Nu32 NGetClearSurfaceVertexNumber()
{
	if(pNClearSurfaceGeometry)
	{
		return pNClearSurfaceGeometry->Mesh.VertexArray.Size;
	}
	else
	{
		return 0;
	}
}

void NSetClearSurfaceVertexColor(const Nu32 index, const NCOLOR *pcolor )
{
	NErrorIf(!pNClearSurfaceGeometry, NERROR_NULL_POINTER);
	NSetVertexColor0_4f(	NGetArrayPtr(&pNClearSurfaceGeometry->Mesh.VertexArray,index),
							NGET_GEOMETRY_IDS_VERTEX_FORMAT(pNClearSurfaceGeometry->Core.IDS),
							pcolor );
}

void NSetClearSurfaceAllVertexColor(const NCOLOR *pcolor )
{
	NSetGeometryVertexRangeColor(pNClearSurfaceGeometry,NVERTEX_DATA_COLOR0,0,pNClearSurfaceGeometry->Mesh.VertexArray.Size,pcolor);
}

void NSetClearSurfaceAllVertexColorf(const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a )
{
	NSetGeometryVertexRangeColorf(pNClearSurfaceGeometry,NVERTEX_DATA_COLOR0,0,pNClearSurfaceGeometry->Mesh.VertexArray.Size,r,g,b,a);
}

void NGetClearSurfaceVertexColor(NCOLOR *pcolor, const Nu32 index )
{
	NErrorIf(!pNClearSurfaceGeometry, NERROR_NULL_POINTER);
	NGetVertexColor0_4f(	pcolor,
							NGetArrayPtr(&pNClearSurfaceGeometry->Mesh.VertexArray,index),
							NGET_GEOMETRY_IDS_VERTEX_FORMAT(pNClearSurfaceGeometry->Core.IDS) );
}

void NSetCLearSurfaceColorModulationf(const Nf32 red,const Nf32 green,const Nf32 blue,const Nf32 alpha)
{
	NClearSurfaceColorModulation.red	= red;
	NClearSurfaceColorModulation.green	= green;
	NClearSurfaceColorModulation.blue	= blue;
	NClearSurfaceColorModulation.alpha	= alpha;
}

void NSetCLearSurfaceColorModulation(const NCOLOR *pcolormod)
{
	NClearSurfaceColorModulation = *pcolormod;
}

void NGetCLearSurfaceColorModulation(NCOLOR *pcolormod)
{
	 *pcolormod = NClearSurfaceColorModulation;
}


NGEOMETRY* NGetBoundClearSurface()
{
	return pNClearSurfaceGeometry;
}

void NBindClearSurface(NGEOMETRY *pgeometry)
{
	pNClearSurfaceGeometry = pgeometry;
}



// PRIVATE FUNCTIONS only used by NENGINE INIT and QUIT functions ...
void NInit_ClearSurface()
{
	pNClearSurfaceGeometry = NULL; 
}

void NDisable_ClearSurface()
{
	NErrorIf(pNClearSurfaceGeometry, NERROR_NON_NULL_POINTER);
}
