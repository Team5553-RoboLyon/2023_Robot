#pragma once
#include "lib/N/NType.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "lib/N/Miscellaneous/NColorPalette.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/NObject.h"

#define NLPLAYGROUND_TEXTURE_NAME_SIZE			256

typedef struct NLPLAYGROUND	NLPLAYGROUND;
struct  NLPLAYGROUND
{
	NLPLAYGROUND* setup(const Nf32 ppm, const Nchar* ptxname = NULL);
	void clear();
	void drawFieldTrueEdges(const NCOLOR* pcol);
	void addInRenderPipeline(NXNODELIST *pxdisplaylist);
	void removeFromRenderPipeline(NXNODELIST* pxdisplaylist);

	void setTexture(const NTEXTURE* ptx, const Nbool brebuild);
	void setTextureFilename(const Nchar *pfilename);

	void setPpM(const Nf32 ppm, const Nbool brebuild);
	void setFieldTrueWidth(const Nf32 f) { m_fieldTrueSize.x = f; };
	void setFieldTrueLength(const Nf32 f) { m_fieldTrueSize.y = f; };
	Nf32 getFieldTrueWidth() { return m_fieldTrueSize.x; };
	Nf32 getFieldTrueLength() { return m_fieldTrueSize.y; };

	void updatePpcFromImageWidth(const Nf32 texture_width_in_meter, const Nbool brebuild);
	void updatePpcFromImageHeight(const Nf32 texture_height_in_meter, const Nbool brebuild);

	Nu32 write(FILE* pfile);
	Nu32 read(FILE* pfile);

#ifdef _NEDITOR
	void draw();
#endif

	/*
	void setSize(const NVEC2f32* psize);
	void setWidth(const Nf32 w);
	void setLength(const Nf32 l);
	*/
	Nf32			m_fieldImagePixelPerMeter;
	Nchar*			m_pfieldTextureName;

private:
	void rebuildFieldObject();
	NVEC2f32		m_fieldTrueSize;
	NVEC2f32		m_fieldObjectSize;
	NOBJECT			m_fieldObject;
};
