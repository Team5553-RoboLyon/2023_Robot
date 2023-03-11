
#include "lib/N/NString.h"
#include "lib/N/File/NFile.h"
#include "lib/N/Utilities/NUT_Shape.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/NCore.h"
#include "lib/NL/NLPlayground.h"
/*
NLPLAYGROUND::~NLPLAYGROUND()
{
	NTRANSFORM_HNODE*	pthn = m_fieldObject.pRenderable->pTransformHNode;
	NTEXTURE* ptexture = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);
	NErrorIf(!ptexture, NERROR_SYSTEM_CHECK);
	NErrorIf(!pthn, NERROR_SYSTEM_CHECK);

	NPurgeTransformHNode(pthn);
	NDeleteTexture(ptexture);
	NClear3DObject(&m_fieldObject);

	if (m_pfieldTextureName)
		Nfree(m_pfieldTextureName);
}

NLPLAYGROUND::NLPLAYGROUND()
{
	Nmem0(this, NLPLAYGROUND);
}
*/

NLPLAYGROUND *NLPLAYGROUND::setup(const Nf32 ppm, const Nchar *ptxname)
{
	NOBJECT_DESC objdesc;
	NGEOMETRY_DESC geomdesc;
	NGEOMETRY *pgeom;
	NIMAGE image;
	NIMAGEDESC imagedesc;

	m_fieldImagePixelPerMeter = ppm;

	// Objet terrain
	Nmem0(&objdesc, NOBJECT_DESC);
	FLAG_ON(objdesc.Flags, FLAG_NOBJECT_DESC_USEPARAM_INCLUDED_GEOMETRY_PREALLOCATED_CAPACITY);
	FLAG_ON(objdesc.Flags, FLAG_NOBJECT_DESC_USEPARAM_TRANSFORMHNODE);
	objdesc.IncludedGeometryPreallocatedCapacity = 1;
	objdesc.pTransformHNode = NCreateTransformHNode(NULL);
	NSetup3DObject(&m_fieldObject, &objdesc);
	NSetTransformHNodePositionf(m_fieldObject.pRenderable->pTransformHNode, 0, 0, 0);

	// Insert a simple quad Geometry ( with a default texture)
	NFillupGeometryDesc(&geomdesc, NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE, 0, 2, 4);
	pgeom = NGeometryEmplaceBack(m_fieldObject.pRenderable, &geomdesc);

	if (ptxname)
	{
		NSetGeometryTexture(pgeom, 0, NCreateTextureFromTga("field", ptxname));
		setTextureFilename(ptxname);
	}
	else
	{
		// default image
		imagedesc.ColorMapSize = 0;
		imagedesc.ImageID = NIMAGEID_RGBA8;
		imagedesc.Height = 32;
		imagedesc.Width = 32;
		NSetGeometryTexture(pgeom, 0, NCreateTexture("field", NSetUpImage(&image, &imagedesc)));
		NClearImage(&image);
		m_pfieldTextureName = NULL;
	}

	// sizes:
	m_fieldTrueSize.x = 16.46f; // Width
	m_fieldTrueSize.y = 8.23f;	// Length

	return this;
}
void NLPLAYGROUND::clear()
{
	NTRANSFORM_HNODE *pthn = m_fieldObject.pRenderable->pTransformHNode;
	NTEXTURE *ptexture = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);
	NErrorIf(!ptexture, NERROR_SYSTEM_CHECK);
	NErrorIf(!pthn, NERROR_SYSTEM_CHECK);

	NPurgeTransformHNode(pthn);
	NDeleteTexture(ptexture);
	NClear3DObject(&m_fieldObject);

	if (m_pfieldTextureName)
		Nfree(m_pfieldTextureName);
}

void NLPLAYGROUND::drawFieldTrueEdges(const NCOLOR *pcol)
{
	NVEC3f32 v0;
	NVEC2f32 xtd;

	v0.x = v0.y = v0.z = 0.0f;
	xtd.x = m_fieldTrueSize.x / 2.0f;
	xtd.y = m_fieldTrueSize.y / 2.0f;
	NUT_Draw_Quad(&v0, &xtd, pcol);
}

void NLPLAYGROUND::addInRenderPipeline(NXNODELIST *pxdisplaylist)
{
	NErrorIf(NGetXNodeByX(pxdisplaylist, &m_fieldObject), NERROR_ALREADY_DONE);
	NXNodeInsertEnd(NCreateXNode(&m_fieldObject), pxdisplaylist);
}

void NLPLAYGROUND::removeFromRenderPipeline(NXNODELIST *pxdisplaylist)
{
	NErrorIf(!NGetXNodeByX(pxdisplaylist, &m_fieldObject), NERROR_ALREADY_DONE);
	NDeleteXNode(NGetXNodeByX(pxdisplaylist, &m_fieldObject), NULL);
}

void NLPLAYGROUND::setTexture(const NTEXTURE *ptx, const Nbool brebuild)
{
	NTEXTURE *ptexture = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);
	NSetGeometryTexture((NGEOMETRY *)m_fieldObject.pRenderable->GeomArray.pFirst, 0, ptx);
	if (ptexture)
	{
		NDeleteTexture(ptexture);
	}

	if (brebuild)
		rebuildFieldObject();
}
void NLPLAYGROUND::setTextureFilename(const Nchar *pfilename)
{
	if (m_pfieldTextureName)
		Nfree(m_pfieldTextureName);
	m_pfieldTextureName = NStrDuplicate(pfilename);
}
void NLPLAYGROUND::setPpM(const Nf32 ppm, const Nbool brebuild)
{
	m_fieldImagePixelPerMeter = ppm;
	if (brebuild)
		rebuildFieldObject();
}
void NLPLAYGROUND::updatePpcFromImageWidth(const Nf32 texture_width_in_meter, const Nbool brebuild)
{
	NErrorIf(!texture_width_in_meter, NERROR_NULL_VALUE);
	NTEXTURE *ptx = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);
	if (ptx)
	{
		m_fieldImagePixelPerMeter = (Nf32)ptx->size.x / texture_width_in_meter;
	}
	else
	{
		m_fieldImagePixelPerMeter = 0.0f;
	}

	if (brebuild)
		rebuildFieldObject();
}

void NLPLAYGROUND::updatePpcFromImageHeight(const Nf32 texture_height_in_meter, const Nbool brebuild)
{
	NErrorIf(!texture_height_in_meter, NERROR_NULL_VALUE);
	NTEXTURE *ptx = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);
	if (ptx)
	{
		m_fieldImagePixelPerMeter = (Nf32)ptx->size.y / texture_height_in_meter;
	}
	else
	{
		m_fieldImagePixelPerMeter = 0.0f;
	}

	if (brebuild)
		rebuildFieldObject();
}
Nu32 NLPLAYGROUND::write(FILE *pfile)
{
	if (fwrite(&m_fieldImagePixelPerMeter, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	if (fwrite(&m_fieldTrueSize, sizeof(NVEC2f32), 1, pfile) != 1)
		return 0;
	if (fwrite(&m_fieldObjectSize, sizeof(NVEC2f32), 1, pfile) != 1)
		return 0;

	Nu32 l = 0;
	if (m_pfieldTextureName)
		l = strlen(m_pfieldTextureName) + 1;

	if (fwrite(&l, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	if (l)
	{
		if (fwrite(m_pfieldTextureName, l * sizeof(Nchar), 1, pfile) != 1)
			return 0;
	}
	return 1;
}
Nu32 NLPLAYGROUND::read(FILE *pfile)
{
	if (fread(&m_fieldImagePixelPerMeter, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	if (fread(&m_fieldTrueSize, sizeof(NVEC2f32), 1, pfile) != 1)
		return 0;
	if (fread(&m_fieldObjectSize, sizeof(NVEC2f32), 1, pfile) != 1)
		return 0;

	Nu32 l = 0;
	if (fread(&l, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	if (l)
	{
		m_pfieldTextureName = (Nchar *)Nrealloc(m_pfieldTextureName, l * sizeof(Nchar));
		if (fread(m_pfieldTextureName, l * sizeof(Nchar), 1, pfile) != 1)
			return 0;

		NSwitchToFullPathFileSystem();
		setTexture(NCreateTextureFromTga("playground", m_pfieldTextureName), NTRUE);
		NSwitchToRootRelativePathFileSystem();
	}

	return 1;
}
#ifdef _NEDITOR
void NLPLAYGROUND::draw()
{
}
#endif

/*
void NLPLAYGROUND::setSize(const NVEC2f32* psize)
{
	m_fieldObjectSize = *psize;
	rebuildFieldObject();
}

void NLPLAYGROUND::setWidth(const Nf32 w)
{
	m_fieldObjectSize.x = w;
	rebuildFieldObject();
}

void NLPLAYGROUND::setLength(const Nf32 l)
{
	m_fieldObjectSize.y = l;
	rebuildFieldObject();
}
*/
void NLPLAYGROUND::rebuildFieldObject()
{
	if (m_fieldImagePixelPerMeter)
	{
		NTEXTURE *ptx = NGetGeometryTexture(NGetFirstIncludedGeometry(m_fieldObject.pRenderable), 0);

		if (ptx)
		{
			m_fieldObjectSize.x = (Nf32)ptx->size.x / m_fieldImagePixelPerMeter;
			m_fieldObjectSize.y = (Nf32)ptx->size.y / m_fieldImagePixelPerMeter;
		}
		else
		{
			m_fieldObjectSize.x = 0.0f;
			m_fieldObjectSize.y = 0.0f;
		}
	}
	else
	{
		m_fieldObjectSize.x = 0.0f;
		m_fieldObjectSize.y = 0.0f;
	}
	// The Quad
	NUT_QUAD_DESC quad_desc;
	NCOLOR color0 = {NCOLOR_PRESET3F_WHITE, 1.0f};
	memset(&quad_desc, 0, sizeof(NUT_QUAD_DESC));
	// FLAG_ON(quad_desc.CoreDesc.TexCoordBuild.Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32); // The simple default UV will be used automatically if the format vertex includes UV
	FLAG_ON(quad_desc.CoreDesc.ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
	FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);

	//	FLAG_ON(quad_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN);
	// 	quad_desc.CoreDesc.ConstructionOrigin.x = ;
	// 	quad_desc.CoreDesc.ConstructionOrigin.y = ;
	// 	quad_desc.CoreDesc.ConstructionOrigin.z = 0.0f;

	quad_desc.CoreDesc.Construction_Plane = _PLANE_XY;

	// DIMENSION TERRAIN INFINITE RECHARGE 2020
	// Dimension Terrain 2020 Infinite Recharge: 1598 cm
	// Dimension Terrain 2020 Infinite Recharge: 821 cm

	// DIMENSION TERRAIN INFINITE RECHARGE AT HOME 2021
	// Dimension Terrain 2021 Infinite Recharge at Home: 914.4 cm
	// Dimension Terrain 2021 Infinite Recharge at Home: 457.2 cm

	quad_desc.CoreDesc.Size = m_fieldObjectSize;

	FLAG_ON(quad_desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_ANCHOR);
	quad_desc.CoreDesc.Anchor.x = ANCHOR_MIDWIDTH;
	quad_desc.CoreDesc.Anchor.y = ANCHOR_MIDHEIGHT;
	quad_desc.CoreDesc.Anchor.z = 0.0f;

	quad_desc.CoreDesc.ColorBuild.pColorRange = &color0;
	quad_desc.CoreDesc.ColorBuild.ColorRangeSize = 1;

	NGEOMETRY *pgeom = NGetFirstIncludedGeometry(m_fieldObject.pRenderable);
	NEraseGeometryMesh(pgeom);
	NUT_InsertQuad(pgeom, &quad_desc);
}
