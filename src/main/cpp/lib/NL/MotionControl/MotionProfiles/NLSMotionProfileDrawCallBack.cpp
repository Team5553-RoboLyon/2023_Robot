
#include "lib/NL/MotionControl/NLKin.h"
#include "lib/NL/MotionControl/MotionProfiles/NLSMotionProfile.h"

#ifdef _NEDITOR
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"
#endif

// SMOTION PROFILE NCOLORPICKPACK index ventilation:
// NCOLORPICKPACK.#0....... Flat Motion Shape color					NCOLORPICKPACK.#3....... S Motion Shape color phase 3
// NCOLORPICKPACK.#1....... S Motion Shape color phase 1			NCOLORPICKPACK.#4....... S Motion Shape color 'Out'
// NCOLORPICKPACK.#2....... S Motion Shape color phase 2			NCOLORPICKPACK.#5....... F et S Motion Shape Text color


#ifdef _NEDITOR
void drawNull(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	const NRECTf32 *pr = p2docs->getBoundingBox();
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size  = 0.02f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
//	NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_RED, 1);

	v.z = 0.0f;
	v.x = (pr->right + pr->left )/2.0f;
	v.y = (pr->top + pr->bottom)/2.0f;
	NUT_Draw_Text("! NULL MOTION PROFILE !", &v, &drawtxt);
}

void drawFlat(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	// Nom Motion profile
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);
	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_fUp.m_s0 + psmp->m_fUp.m_ds*0.5f);
	v.y = p2docs->transformY(psmp->m_sUp.m_v1);
	sprintf(txt, "Flat ( %.4f s)", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);
	
	psmp->m_fUp.drawVs(p2docs,pickpack);
}
void drawSup(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_sUp.m_s0 + psmp->m_sUp.m_ds*0.5f);
	v.y = p2docs->transformY((psmp->m_sUp.m_v0 + psmp->m_sUp.m_v1) / 2.0f);
	sprintf(txt, "Up ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	psmp->m_sUp.drawVs(p2docs, pickpack, psmp->m_cutInOffsets.m_dt, &psmp->m_cutOutOffsets);
}
void drawSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_sDown.m_s0 + psmp->m_sDown.m_ds*0.5f);
	v.y = p2docs->transformY((psmp->m_sDown.m_v0 + psmp->m_sDown.m_v1)/2.0f);
	sprintf(txt, "Down ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	psmp->m_sDown.drawVs(p2docs, pickpack, psmp->m_cutInOffsets.m_dt, &psmp->m_cutOutOffsets);
}

void drawSupFlat(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_fUp.m_s0 + psmp->m_fUp.m_ds*0.5f);
	v.y = p2docs->transformY(psmp->m_sUp.m_v1 + 0.025f);
	sprintf(txt, "UpFlat ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	NLSCUTOUTOFFSETS	out;
	out.null();
	psmp->m_sUp.drawVs(p2docs, pickpack, psmp->m_cutInOffsets.m_dt,&out);
	psmp->m_fUp.drawVs(p2docs, pickpack,psmp->m_sUp.m_v1 );
}

void drawFlatSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_fUp.m_s0 + psmp->m_fUp.m_ds*0.5f);
	v.y = p2docs->transformY(psmp->m_sDown.m_v0 + 0.025f);
	sprintf(txt, "Flat Down ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	psmp->m_fUp.drawVs(p2docs, pickpack,  psmp->m_sDown.m_v0);
	psmp->m_sDown.drawVs(p2docs, pickpack, 0.0f, &psmp->m_cutOutOffsets);
}

void drawSupSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_sDown.m_s0);
	v.y = p2docs->transformY(psmp->m_sDown.m_v0 + 0.025f);
	sprintf(txt, "UpDown ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	NLSCUTOUTOFFSETS	out;
	out.null();
	psmp->m_sUp.drawVs(p2docs, pickpack, psmp->m_cutInOffsets.m_dt,&out);
	psmp->m_sDown.drawVs(p2docs, pickpack,0.0f, &psmp->m_cutOutOffsets);
}

void drawSupFlatSdown(NLSMOTIONPROFILE *psmp, NL2DOCS * p2docs, const NCOLORPICKPACK pickpack)
{
	NVEC3 v;
	Nchar txt[32];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = *p2docs->getColor(5, pickpack);
	//NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_MONASTRAL,1);

	v.z = 0.0f;
	v.x = p2docs->transformX(psmp->m_fUp.m_s0 + psmp->m_fUp.m_ds*0.5f);
	v.y = p2docs->transformY(psmp->m_sUp.m_v1 + 0.025f);
	sprintf(txt, "UpFlatDown ( %.4f )", psmp->m_dt);
	NUT_Draw_Text(txt, &v, &drawtxt);

	NLSCUTOUTOFFSETS	out;
	out.null();

	psmp->m_sUp.drawVs(p2docs, pickpack,psmp->m_cutInOffsets.m_dt,&out);
	psmp->m_fUp.drawVs(p2docs, pickpack,psmp->m_sUp.m_v1);
	psmp->m_sDown.drawVs(p2docs, pickpack,0.0f,&psmp->m_cutOutOffsets);
}
#endif
