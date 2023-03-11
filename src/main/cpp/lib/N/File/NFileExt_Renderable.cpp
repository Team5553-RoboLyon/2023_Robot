#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Render/Renderable/NRenderable.h"
// #include "lib/N/Render/Renderable/NRenderable_UpdateAndExtract.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Render/NFrustum_Culling.h"

static Nu32 _writegeometry(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	return NFileWriteGeometry(pfile, (NGEOMETRY *)ptr);
}

static Nu32 _readgeometry(NBYTE *ptr, NFILE *pfile, Nu32 userdata)
{
	return NFileReadGeometry(pfile, (NGEOMETRY *)ptr);
}

Nu32 NFileWriteRenderableGeometryArray(NFILE *pfile, const NRENDERABLE *pr)
{
	if (!NFileWriteArray(pfile, &pr->GeomArray, _writegeometry, 0))
		return 0;
	else
		return 1;
}

Nu32 NFileReadRenderableGeometryArray(NFILE *pfile, NRENDERABLE *pr)
{
	// The incoming Geometry Array size has to be 0 !...  Else, NFileReadArray will generate an Error !
	if (!NFileReadArray(pfile, &pr->GeomArray, _readgeometry, 0))
		return 0;
	else
		return 1;
}

Nu32 NFileWriteRenderable(NFILE *pfile, const NRENDERABLE *pr)
{
	NHEADER_RENDERABLE header;

	// Write Current version Tag
	if (NFileWriteVersionTag(pfile, VERSION_NHEADER_RENDERABLE) != 1)
		return 0;

	memset(&header, 0, sizeof(NHEADER_RENDERABLE));
	header.BoundingSphere = pr->BoundingSphere;
	header.Update_FCT_ID = NGetRenderableUpdateFunctionID(pr->Update_FCT);
	header.Extract_FCT_ID = NGetRenderableExtractFunctionID(pr->Extract_FCT);

	if (NFileWrite(pfile, &header, sizeof(NHEADER_RENDERABLE), 1) != 1)
		return 0;
	if (!NFileWriteTransformHNodeReference(pfile, pr->pTransformHNode))
		return 0;
	if (!NFileWriteRenderableGeometryArray(pfile, pr))
		return 0;

	return 1;
}

Nu32 NFileReadRenderable(NFILE *pfile, NRENDERABLE *pr)
{
	NHEADER_RENDERABLE header;
	Nu32 version;

	if (NFileReadVersionTag(pfile, &version) != 1)
		return 0;

	switch (NGETVERSION_MAIN(version))
	{
	// Current Main Version (all versions 0.0.x )
	case NGETVERSION_MAIN(VERSION_NHEADER_RENDERABLE):
		if (NFileRead(pfile, &header, sizeof(NHEADER_RENDERABLE), 1) != 1)
			return 0;

		pr->BoundingSphere = header.BoundingSphere;
		pr->Update_FCT = NGetRenderableUpdateFunctionByID(header.Update_FCT_ID);
		pr->Extract_FCT = NGetRenderableExtractFunctionByID(header.Extract_FCT_ID);

		if (!NFileReadTransformHNodeReference(pfile, &pr->pTransformHNode))
			return 0;

		if (!NFileReadRenderableGeometryArray(pfile, pr))
			return 0;

		break;

		// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}

	return 1;
}
