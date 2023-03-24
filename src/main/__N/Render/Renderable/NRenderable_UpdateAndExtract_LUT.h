#ifndef __NRENDERABLE_UPDATEANDEXTRACT_LUT_H
#define __NRENDERABLE_UPDATEANDEXTRACT_LUT_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NRenderable_UpdateAndExtract_LUT.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NRenderable.h"
#include "NRenderable_Update.h"
#include "NRenderable_Extract.h"
#include "NRenderable_ExtractSet.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Structures											**
// ***************************************************************************************

typedef struct  
{
	NRENDERABLE_EXTRACT_FCT_ID_ENUM	Extract_FCTID;
	NEXTRACTSET						ExtractSet;
}NEXTRACTSET_KEY;
// ***************************************************************************************
// **								Functions											**
// ***************************************************************************************
NRENDERABLE_UPDATE_FCT				NGetRenderableUpdateFunctionByID( NRENDERABLE_UPDATE_FCT_ID_ENUM id );
NRENDERABLE_UPDATE_FCT_ID_ENUM		NGetRenderableUpdateFunctionID( NRENDERABLE_UPDATE_FCT pfct );
void								NBindRenderableUpdateUserFunction( NRENDERABLE_UPDATE_FCT_ID_ENUM id, NRENDERABLE_UPDATE_FCT pfct );

NRENDERABLE_EXTRACT_FCT				NGetRenderableExtractFunctionByID( NRENDERABLE_EXTRACT_FCT_ID_ENUM id );
NRENDERABLE_EXTRACT_FCT_ID_ENUM		NGetRenderableExtractFunctionID( NRENDERABLE_EXTRACT_FCT pfct );
NRENDERABLE_EXTRACT_FCT_ID_ENUM		NGetRenderableExtractFunctionIDByExtractSet( const NEXTRACTSET extractset );
NRENDERABLE_EXTRACT_FCT				NGetRenderableExtractFunctionByExtractSet( const NEXTRACTSET extractset );

void								NBindRenderableExtractUserFunction( NRENDERABLE_EXTRACT_FCT_ID_ENUM id, NRENDERABLE_EXTRACT_FCT pfct, const NEXTRACTSET extractset  );

// Private
void NInitializeRenderableUpdateFCT_LUT();	// Used by NInit() function.
void NInitializeRenderableExtractFCT_LUT();	// Used by NInit() function.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NRENDERABLE_UPDATEANDEXTRACT_LUT_H 

