#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "../../NCore.h"

#include "lib/N/NStructure.h"
//#include "../../NObjects.h"
#include "../../Particles/NParticles.h"
#include "../../Ntext.h"
#include "../NFrustum_Culling.h"

#include "lib/N/UI/NUI.h"

// TEMPORARY
#include"../../Collectibles/NCollectibles.h"
// TEMPORARY


// External Global 

#include "NRenderableScanner.h"

NARRAY NRenderableScanner;

//NRENDERABLESCANNER	NRenderableScanner[NRENDERABLESCANNER_ENUMSIZE];


NRENDERABLESCANNER	*NCreateRenderableScanner(void *pentry, const NRENDERABLESCANNER_FCT scanner_function )
{
	NRENDERABLESCANNER	rscan;

	rscan.pEntry	= pentry;
	rscan.pScanner	= scanner_function;
	return (NRENDERABLESCANNER*)NArrayPushBack(&NRenderableScanner,(NBYTE*)&rscan );
}

Ns32 NDeleteRenderableScanner(const Nu32 scanner_index)
{

//	if(bRendering)
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_RENDERING) )
		return -1;
	else
		return (Ns32)NEraseArrayElement(&NRenderableScanner,scanner_index,NULL);
}

void NRenderableScannerCallBack_UIs(void* pentry,const NTIME *ptime)
{
	NUI	**ptr;
	Nu32		i;
	NUI	*pui;

	// Call of all the Active Root UI
	ptr = (NUI**)NGetFirstArrayPtr((NARRAY*)pentry);
	for(i=NGetArraySize((NARRAY*)pentry);i!=0;i--,ptr ++)
	{
		pui = *ptr;
		pui->pRenderable->Update_FCT(pui->pRenderable,pui,ptime);
	}
}

// TEMPORARY IMPLEMENTATION !!!! WILL BE MUCH BETTER WITH NRNODE !!!
extern NCOLLECTIBLE_SET		NCollectibleSet;
void NRenderableScannerCallBack_Collectibles(void* pentry,const NTIME *ptime)
{
	NCOLLECTIBLE	**ptr;
	Nu32			i;
	
	NUpdateCollectiblePicking();

	// Call of all the Active Collectible for Update and extract
	ptr = (NCOLLECTIBLE**)NCollectibleSet.CollectiblePtrArray.pFirst;
	for(i=NCollectibleSet.CollectiblePtrArray.Size;i!=0;i--,ptr ++)
	{
		(*ptr)->pRenderable->Update_FCT((*ptr)->pRenderable,*ptr,ptime);
	}
}


void NRenderableScannerCallBack_RenderableAddress_Array(void *pentry,const NTIME *ptime)
{
	NRENDERABLE		**ppr;
	NRENDERABLE		*pr;
	Nu32			i;

	ppr		= (NRENDERABLE**)NGetFirstArrayPtr((NARRAY*)pentry);
	for(i=0;i<NGetArraySize((NARRAY*)pentry);i++,ppr++)
	{
		pr = *ppr;
		pr->Update_FCT(pr,ppr,ptime);
	}
}

void NRenderableScannerCallBack_RenderableAddress_XList(void *pentry,const NTIME *ptime)
{
	NXNODE		*pxn;
	NRENDERABLE	*pr;
	NRENDERABLE	**ppr;

	pxn = XFIRST((NXNODELIST*)pentry);
	while(XVALID(pxn,(NXNODELIST*)pentry))
	{
		ppr = (NRENDERABLE**)XX(pxn);
		pr = *ppr;
		pr->Update_FCT(pr,ppr,ptime);
		XNEXT(pxn);
	}
}
