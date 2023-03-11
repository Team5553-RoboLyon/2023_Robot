#ifndef __NFILEEXT_APPEARANCE_RWCALLBACKS_H
#define __NFILEEXT_APPEARANCE_RWCALLBACKS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NFileExt_Appearance_RWCallBacks.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NTexture.h"
#include "lib/N/Geometry/Components/NGeometryBlend.h"
#include "lib/N/Geometry/Components/NGeometryMaterial.h"
#include "NFile.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_RWAPPEARANCE_LUT_SIZE 6     // .Possible different callbacks number for each type (from 0 to 5)!
#define CONSTANT_RWAPPEARANCE_FIRST_USERID 3 // |_Ids 3,4,5 are user friendly... but 0,1,2 are ENGINE RESERVED !!!
#define CONSTANT_RWAPPEARANCE_LAST_USERID 5  // |

#define DEFAULT_RWAPPEARANCE_IDS NMAKE_RWAPPEARANCE_IDS(0, 0, 0)         // if the Compiler define _NLIBS is set to OFF, this default IDs is used
#define DEFAULT_NUTLIBS_RWAPPEARANCE_IDS NMAKE_RWAPPEARANCE_IDS(2, 2, 2) // if the Compiler define _NLIBS is set to ON, this default IDs is used

#define RWAPPEARANCE_TEXTURE_VOID 0  // Do nothing, no Write/no Read
#define RWAPPEARANCE_BLEND_VOID 0    // Do nothing, no Write/no Read
#define RWAPPEARANCE_MATERIAL_VOID 0 // Do nothing, no Write/no Read

#define RWAPPEARANCE_TEXTURE_DATA 1  // Write/Read full data
#define RWAPPEARANCE_BLEND_DATA 1    // Write/Read full data
#define RWAPPEARANCE_MATERIAL_DATA 1 // Write/Read full data

#define RWAPPEARANCE_TEXTURE_NUTLIBS_REF 2  // Write/Read library key reference
#define RWAPPEARANCE_BLEND_NUTLIBS_REF 2    // Write/Read library key reference
#define RWAPPEARANCE_MATERIAL_NUTLIBS_REF 2 // Write/Read library key reference

#define NMAKE_RWAPPEARANCE_IDS(t, m, b) (((t & 0xFF) << 16) | ((m & 0xFF) << 8) | (b & 0xFF))
#define NGET_RWAPPEARANCE_IDS_TEXTURE(id) ((id & 0x00FF0000) >> 16)
#define NGET_RWAPPEARANCE_IDS_MATERIAL(id) ((id & 0x0000FF00) >> 8)
#define NGET_RWAPPEARANCE_IDS_BLEND(id) (id & 0x000000FF)

    void NInitRWAppearanceCallBacks(); // used by "N_Init" function only !

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // + RWAppearance void functions
    // +
    // These functions are not suppose to do anything !
    Nu32 NWriteAppearanceTexture_Void(NFILE *pfile, const NTEXTURE *ptr);
    Nu32 NReadAppearanceTexture_Void(NFILE *pfile, NTEXTURE **pptr);
    Nu32 NWriteAppearanceUVMatrixAnim_Void(NFILE *pfile, const NUVMATRIXANIM *ptr);
    Nu32 NReadAppearanceUVMatrixAnim_Void(NFILE *pfile, NUVMATRIXANIM **pptr);
    Nu32 NWriteAppearanceBlend_Void(NFILE *pfile, const NBLEND *ptr);
    Nu32 NReadAppearanceBlend_Void(NFILE *pfile, NBLEND **pptr);
    Nu32 NWriteAppearanceMaterial_Void(NFILE *pfile, const NMATERIAL *ptr);
    Nu32 NReadAppearanceMaterial_Void(NFILE *pfile, NMATERIAL **pptr);
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // + RWAppearance_...Data  functions
    // +
    // These functions are suppose to write/read all appearance component data !
    Nu32 NWriteAppearanceTexture_ImageData(NFILE *pfile, const NTEXTURE *ptr);
    Nu32 NReadAppearanceTexture_ImageData(NFILE *pfile, NTEXTURE **pptr);
    Nu32 NWriteAppearanceUVMatrixAnim_Data(NFILE *pfile, const NUVMATRIXANIM *ptr);
    Nu32 NReadAppearanceUVMatrixAnim_Data(NFILE *pfile);
    Nu32 NWriteAppearanceBlend_Data(NFILE *pfile, const NBLEND *ptr);
    Nu32 NReadAppearanceBlend_Data(NFILE *pfile, NBLEND **pptr);
    Nu32 NWriteAppearanceMaterial_Data(NFILE *pfile, const NMATERIAL *ptr);
    Nu32 NReadAppearanceMaterial_Data(NFILE *pfile, NMATERIAL **pptr);
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // + RWAppearance_...Reference  functions
    // +
    // These functions are suppose to write/read a reference on each appearance component !
    Nu32 NWriteAppearanceTexture_NUTLibrary_Reference(NFILE *pfile, const NTEXTURE *ptr);
    Nu32 NReadAppearanceTexture_NUTLibrary_Reference(NFILE *pfile, NTEXTURE **pptr);
    Nu32 NWriteAppearanceBlend_NUTLibrary_Reference(NFILE *pfile, const NBLEND *ptr);
    Nu32 NReadAppearanceBlend_NUTLibrary_Reference(NFILE *pfile, NBLEND **pptr);
    Nu32 NWriteAppearanceMaterial_NUTLibrary_Reference(NFILE *pfile, const NMATERIAL *ptr);
    Nu32 NReadAppearanceMaterial_NUTLibrary_Reference(NFILE *pfile, NMATERIAL **pptr);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NFILEEXT_H
