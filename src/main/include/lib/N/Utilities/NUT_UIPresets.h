#ifndef __NUT_UI_PRESETS_H
#define __NUT_UI_PRESETS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_UIPresets.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/UI/NUI.h"
#include "lib/N/UI/NUIPushButton.h"
#include "lib/N/UI/NUICheckBox.h"
#include "lib/N/UI/NUIValUpdater.h"
#include "lib/N/UI/NUISlide.h"
#include "lib/N/UI/NUIColorCircle.h"
#include "lib/N/UI/NUICustom.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

NUI_COLORCIRCLE*	NUT_CreateUIColorCircleRGB(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 sizex,const Nf32 sizey,const NCOLOR *pcolor, Nchar *pcaption, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );
NUI_COLORCIRCLE*	NUT_CreateUIColorCircleRGBA(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 sizex,const Nf32 sizey,const NCOLOR *pcolor, Nchar *pcaption, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );
NUI_COLORCIRCLE*	NUT_CreateUIColorCircle(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 sizex,const Nf32 sizey,const NCOLOR *pcolor, Nchar *pcaption, Nbool use_rgba, Nbool rgba8_output, Nbool inprogress_command, Nbool print_info, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );

NUI*				NUT_CreateUICaptionf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Nu32 user32 );

NUI*				NUT_CreateUIPicturef(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *patlas_refname, const Nu32 user32 );
NUI*				NUT_CreateUIPictureXtdf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *patlas_refname, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );
inline NGEOMETRY*	NUT_GetUIPictureGeometry(NUI *puipic){return (NGEOMETRY*)puipic->pRenderable->GeomArray.pFirst;}

NUI_SLIDE*			NUT_CreateUISlidingLayer(NUI *parent, const NUI_EVENT_HANDLE event_proc, const Nu32 user32);
NUI_SLIDE*			NUT_CreateUISlidingFramedLayer(NUI *parent, const NUI_EVENT_HANDLE event_proc, const Nu32 user32);

NUI_PUSHBUTTON*		NUT_CreateUIPushButtonf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );

NUI_VALUPDATER*		NUT_CreateUIFlatValUpdateri(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Ns32 minvalue,const Ns32 maxvalue, const Ns32 initialvalue, const NUI_EVENT_HANDLE event_proc,  const Nu32 user32 );
NUI_VALUPDATER*		NUT_CreateUIFlatValUpdaterf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, const Nchar *pcaption, const Nf32 minvalue,const Nf32 maxvalue, const Nf32 initialvalue, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );

NUI_CHECKBOX*		NUT_CreateUIFlatCheckBox(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pcaption, Nchar *pchecktext,Nchar *punchecktext, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );
NUI_CHECKBOX*		NUT_CreateUIGraphicCheckBox(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z,const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname,const Nchar *patlas_checkrefname, const Nchar *patlas_uncheckrefname, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );

NUI_CUSTOM*			NUT_CreateUICustom(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const Nchar *patlas_backrefname, const Nchar *patlas_iconrefname, const NVEC3 *pxtrageom_shape_size, const Nu16 extra_geom_capacity, const NUI_EVENT_HANDLE event_proc, const Nu32 user32 );
void				NUT_InsertPictureInUICustom(NUI_CUSTOM *pgv, const Nchar *patlas_refname, const NBUILDBASE_COLOR *pcolorbuild, const Nbool bdisplay );

NUI*				NUT_CreateUIRectf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const NVEC2 *psize, const Nu32 user32 );
NUI*				NUT_CreateUIRectXtdf(NUI *parent, const Nf32 x, const Nf32 y, const Nf32 z, Nchar *pframe_caption, const NVEC2 *psize, const NUI_EVENT_HANDLE event_proc,  const Nu32 user32 );

// NUI*				NUT_CreateUILayer(NUI *parent, const NUI_EVENT_HANDLE event_handle, const Nu32 listen_intercept_flags_style, NUI_LISTEN_TOUCH_MODE_ENUM touch_mode, const Nu32 user32 );
// NUI*				NUT_CreateUIFramedLayer(NUI *parent, const NUI_EVENT_HANDLE event_handle, const Nu32 listen_intercept_flags_style, NUI_LISTEN_TOUCH_MODE_ENUM touch_mode, const NRECTf32 *pbbox, const Nu32 user32 );

// LOW LEVEL function
NMESH*				NUT_CreateUIPickingQuad(const NUIPLACEMENT *place_desc);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_UI_PRESETS_H 



