#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_TEXTUREANDUV_TOOL_ELEMENTS_H
#define __NUT_EDITOR_TEXTUREANDUV_TOOL_ELEMENTS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Elements.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/Geometry/Components/NGeometryAppearance.h"
#include "lib/N/BasicElements/NTextureAtlas.h"

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

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + FLAGS NUT_EDITOR_ATXC_ELEMENTS
// +
// + What do you want to edit ? ...
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_COMPATIBLE_GEOMETRY_RENDER_FCT BIT_0 // USER WANTS UPDATE/CHANGE Geometry Render Function with a compatible one ( a one which uses the same Geometry Data Format )
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE BIT_1						// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE_WRAP_PARAMETERS BIT_2		// USER WANTS UPDATE TEXTURE WRAP_S and WRAP_T parameters
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE_FILTER_PARAMETERS BIT_3		// USER WANTS UPDATE TEXTURE MIN_FILTER and MAG_FILTER parameters
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_SET BIT_4					// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_OFFSET BIT_5				// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_SCALE BIT_6					// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_OFFSET BIT_7			// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_SCALE BIT_8			// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_BLEND BIT_9							// USER WANTS UPDATE
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_MATERIAL BIT_10						// USER WANTS UPDATE
// Notice that FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTUREUNIT doesn't exist. And it's perfectly normal.
// Texture Unit Arc selector is automatically activated if user asks to Edit Texture (FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE set to ON)
// with 1 or more Texture Units available (which is describe by the GeometryIDS given by user)

// ... Some additionnal informations to give to Engine to describe the way the ATxC will work.
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_TEXTURE_ALLOWED BIT_11  // USER is OK to include a NULL Texture as a valid choice in the TEXTURE RING SELECTOR.
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_BLEND_ALLOWED BIT_12	   // USER is OK to include a NULL Blend as a valid choice in the BLEND RING SELECTOR.
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_NULL_MATERIAL_ALLOWED BIT_13 // USER is OK to include a NULL Material as a valid choice in the MATERIAL RING SELECTOR.
#define FLAG_NUT_EDITOR_ATXC_ELEMENTS_CLAMP_TEXCOORD_SCALE BIT_14  // TxC Offset are always CLAMP between 0 and 1, but not TxC Scale. To Force TxC Scale Clamping (between 0,1) set this flag to ON.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + CONSTANTS & DEFAULT VALUES
// +
#define CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB 2 // This is the maximum possible number. In line with the current maximum Numver of UV Set, possible inside a Vertex Format.

#define NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL 0
#define NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN 1
#define NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT 2
	// #define NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_COMPOSITE				3

	typedef struct
	{
		Nu32 Flags;
		Nu32 GeometryIDS; // GeometryIDS used as reference to setup some part of the GIZMO ( should be the same same the geometry which owns the updated data ).
						  // 	NAPPEARANCE_RAW	Appearance;			// Raw Copy of Parent Geometry Appearance. Thanks to the NGEOMETRY_FORMAT_ENUM we knows exactly how many Texture Unit are really available into it.

		// Textures Coordinates Controllers
		NTEXCOORD_2f TexCoordOffset[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB]; // |_UVs Data... To update UVs. There are 2 possible sets, according with the GeometrySpecs Vertex Format.
		NTEXCOORD_2f TexCoordScale[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];	// |
		// Appearance Controllers
		NBLEND *pBlend;
		NMATERIAL *pMaterial;
		void *pMetaTexture[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER];  // 'Meta' because it's not a texture but a data that describe a texture ( ... and it's used oaa rect)
		Nu8 MetaTextureType[CONSTANT_NAPPEARANCE_RAW_TEXTUREUNIT_NUMBER]; // the 'MetaTexture Type". To know how casting the MetaTexture pointer.

		GLint TextureWrapS;
		GLint TextureWrapT;
		GLint TextureMinFilter;
		GLint TextureMagFilter;

		// 	void						*pFirstVertex;
		// 	Nu32						VertexNumber;
	} NUT_EDITOR_ATXC_ELEMENTS;

	inline void NUT_SetEditorATxC_Elements_GeometryIDS(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu32 geometry_ids) { pelements->GeometryIDS = geometry_ids; }
	inline void NUT_SetEditorATxC_Elements_Texture(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu8 texture_unit, const NTEXTURE *ptexture)
	{
		pelements->MetaTextureType[texture_unit] = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN;
		pelements->pMetaTexture[texture_unit] = (void *)ptexture;
	}
	inline void NUT_SetEditorATxC_Elements_AtlasElement(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu8 texture_unit, const NATLAS_ELEMENT *patlas_el)
	{
		pelements->MetaTextureType[texture_unit] = NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT;
		pelements->pMetaTexture[texture_unit] = (void *)patlas_el;
	}
	inline void NUT_SetEditorATxC_Elements_Blend(NUT_EDITOR_ATXC_ELEMENTS *pelements, const NBLEND *pblend) { pelements->pBlend = (NBLEND *)pblend; }
	inline void NUT_SetEditorATxC_Elements_Material(NUT_EDITOR_ATXC_ELEMENTS *pelements, const NMATERIAL *pmaterial) { pelements->pMaterial = (NMATERIAL *)pmaterial; }
	void NUT_SetEditorATxC_Elements_FromAppearance(NUT_EDITOR_ATXC_ELEMENTS *pelements, const NAPPEARANCE_RAW *pappearance);

	inline void NUT_SetEditorATxC_Elements_TexCoord_Controller(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu32 TxCsetid, const NTEXCOORD_2f *poffset, const NTEXCOORD_2f *pscale)
	{
		NErrorIf(TxCsetid >= CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB, NERROR_VALUE_OUTOFRANGE);
		pelements->TexCoordOffset[TxCsetid] = *poffset;
		pelements->TexCoordScale[TxCsetid] = *pscale;
	}
	inline void NUT_SetEditorATxC_Elements_TexCoord_Controllerf(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu32 TxCsetid, const Nf32 offset_u, const Nf32 offset_v, const Nf32 scale_u, const Nf32 scale_v)
	{
		NErrorIf(TxCsetid >= CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB, NERROR_VALUE_OUTOFRANGE);
		pelements->TexCoordOffset[TxCsetid].x = offset_u;
		pelements->TexCoordOffset[TxCsetid].y = offset_v;
		pelements->TexCoordScale[TxCsetid].x = scale_u;
		pelements->TexCoordScale[TxCsetid].y = scale_v;
	}
	inline void NUT_SetEditorATxC_Elements_TexCoord0_Controller(NUT_EDITOR_ATXC_ELEMENTS *pelements, const NTEXCOORD_2f *poffset, const NTEXCOORD_2f *pscale)
	{
		pelements->TexCoordOffset[0] = *poffset;
		pelements->TexCoordScale[0] = *pscale;
	}
	inline void NUT_SetEditorATxC_Elements_TexCoord1_Controller(NUT_EDITOR_ATXC_ELEMENTS *pelements, const NTEXCOORD_2f *poffset, const NTEXCOORD_2f *pscale)
	{
		pelements->TexCoordOffset[1] = *poffset;
		pelements->TexCoordScale[1] = *pscale;
	}
	inline void NUT_SetEditorATxC_Elements_TexCoord0_Controllerf(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nf32 offset_u, const Nf32 offset_v, const Nf32 scale_u, const Nf32 scale_v)
	{
		pelements->TexCoordOffset[0].x = offset_u;
		pelements->TexCoordOffset[0].y = offset_v;
		pelements->TexCoordScale[0].x = scale_u;
		pelements->TexCoordScale[0].y = scale_v;
	}
	inline void NUT_SetEditorATxC_Elements_TexCoord1_Controllerf(NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nf32 offset_u, const Nf32 offset_v, const Nf32 scale_u, const Nf32 scale_v)
	{
		pelements->TexCoordOffset[1].x = offset_u;
		pelements->TexCoordOffset[1].y = offset_v;
		pelements->TexCoordScale[1].x = scale_u;
		pelements->TexCoordScale[1].y = scale_v;
	}

	// inline void NUT_SetEditorATxC_Elements_VertexList(NUT_EDITOR_ATXC_ELEMENTS *pelements, void* pfirstvertex, const Nu32 vertex_number ){pelements->pFirstVertex = pfirstvertex; pelements->VertexNumber = vertex_number;}

	// Notice: After Setup Elements, we will have ...
	// NUT_EDITOR_ATXC_ELEMENTS.GeometrySpecs = 0 = NGEOMETRY_FORMAT_POINT_TU0_V3_POINT
	// !! Which is perfect for us, because that means NO texture, NO UVs ...
	void NUT_Editor_ATxC_SetupElements(NUT_EDITOR_ATXC_ELEMENTS *pelements);
	inline void NUT_Editor_ATxC_ClearElements(NUT_EDITOR_ATXC_ELEMENTS *pelements) { Nmem0(pelements, NUT_EDITOR_ATXC_ELEMENTS); }
// Nbool		NUT_Editor_ATxC_ElementsUsedOAARectf32IsDefault(const NUT_EDITOR_ATXC_ELEMENTS *pelements, const Nu32 texcoordsetid);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_EDITOR_TEXTUREANDUV_TOOL_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR
