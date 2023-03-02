#ifndef __NRENDERABLESCANNER_H
#define __NRENDERABLESCANNER_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NRenderableScanner.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Core/NTime.h"
#include "../../Containers/NNode.h"

#ifdef __cplusplus
extern "C"
{
#endif
/*
#define NRENDERABLESCANNER_UI_RESERVED		0
#define NRENDERABLESCANNER_PARTICLEEMITTER	1
#define NRENDERABLESCANNER_PHYSICSTRUCTURE	2 // or 1
#define NRENDERABLESCANNER_OBJECT			3 // or 1
#define NRENDERABLESCANNER_TEXTDISPLAYER	4 // or 1
#define NRENDERABLESCANNER_SIZE				5 // or 2
*/


#define NRENDERABLESCANNERS_ARRAY_DEFAULTSIZE	2


typedef void (*NRENDERABLESCANNER_FCT)(void* pscan_entry, const NTIME *ptime);
typedef struct NRENDERABLESCANNER NRENDERABLESCANNER;
//typedef void (*NRENDERABLESCANNER_CHECK_FCT)(NRENDERABLESCANNER *pscan,NRENDERABLE *prenderable);
struct NRENDERABLESCANNER
{
	void							*pEntry;
/*
	NRENDERABLESCANNER_CHECK_FCT	pRenderableCheckIn;
	NRENDERABLESCANNER_CHECK_FCT	pRenderableCheckOut;
*/

	NRENDERABLESCANNER_FCT			pScanner;
};

// extern NRENDERABLESCANNER	NRenderableScanner[NRENDERABLESCANNER_SIZE];
extern NARRAY	NRenderableScanner;

NRENDERABLESCANNER	*NCreateRenderableScanner(void *pentry, const NRENDERABLESCANNER_FCT scanner_function );
Ns32				NDeleteRenderableScanner(const Nu32 scanner_index);

// All RenderableScanner CallBack Functions
// ========================================
void NRenderableScannerCallBack_UIs(void* pentry,const NTIME *ptime);
/*
void NRenderableScannerCallBack_RenderableNode(void* pentry,NTIME *ptime);	// This Generic scanner working with all the structure
																			// with NNODE and NRENDERABLE at the top of it.
																			// So, its working well with 3DObjects, ParticleEmitter,
																			// PhysicStructure, Textdisplayer.
																			// USe this scanner especially if you want to put 
																			// all these different elements into the same and unique
																			// nodelist.
																			// !!!  IT DOESN'T WORK with UI !!!!
void NRenderableScannerCallBack_3DObjects(void *pentry,NTIME *ptime);
void NRenderableScannerCallBack_ParticlesEmitters(void *pentry,NTIME *ptime);
void NRenderableScannerCallBack_PhysicStructure(void *pentry,NTIME *ptime);
void NRenderableScannerCallBack_TextDisplayers(void *pentry,NTIME *ptime);
*/

// new ones ... better
void NRenderableScannerCallBack_RenderableAddress_Array(void *pentry,const NTIME *ptime);
void NRenderableScannerCallBack_RenderableAddress_XList(void *pentry,const NTIME *ptime);

// N Internal use Only ...

// Initialization & Clear
inline void NInitializeRenderableScannerArray(){NSetupArray(&NRenderableScanner,NRENDERABLESCANNERS_ARRAY_DEFAULTSIZE,sizeof(NRENDERABLESCANNER));};
inline void NClearRenderableScannerArray(){NClearArray(&NRenderableScanner,NULL);};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NRENDERABLESCANNER_H 

