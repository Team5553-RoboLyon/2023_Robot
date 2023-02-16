#include "../NCStandard.h"
#include "../NType.h"
#include "../NString.h"
#include "../Containers/NArray.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../NErrorHandling.h"
#include "../Render/Renderable/NRenderable.h"
//#include "../Render/Renderable/NRenderable_UpdateAndExtract.h"
#include "../Render/NFrustum_Culling.h"

// ------------------------------------------------------------------------------------------
// Nu32 NFileWritePoolTagList
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NFileWritePoolTagList(NFILE *pfile, const NPOOL *ppool)
{
	Nu32						poolsize;
	NPOOLTAG_FILEBLOCK_V000		htag;
	NPOOLTAG					*ptag;

	if(NFileWriteVersionTag( pfile,VERSION_NPOOL_FILEBLOCK_V000) != 1)
		return 0;

	poolsize = 0;
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while( (NNODE*)ptag != (NNODE*)&ppool->TagList)
	{
		poolsize ++;

		// the next PoolTag.
		ptag = (NPOOLTAG*)ptag->pNext;
	}
	if( NFileWrite(pfile,&poolsize,sizeof(Nu32),1) != 1)
		return 0;
	
	ptag = (NPOOLTAG *)ppool->TagList.pFirst;
	while( (NNODE*)ptag != (NNODE*)&ppool->TagList)
	{
		memset(&htag,0,sizeof(NPOOLTAG_FILEBLOCK_V000));
		htag.Type		= ptag->Type;
		htag.User_8		= ptag->User_8;
		htag.User_16	= ptag->User_16;
		htag.A_User_32	= ptag->A_User_32;
		htag.B_User_32	= ptag->B_User_32;
		htag.Pos = ptag->Pos;
		if( NFileWrite(pfile,&htag,sizeof(NPOOLTAG_FILEBLOCK_V000),1) != 1)
			return 0;

		// the next PoolTag.
		ptag = (NPOOLTAG*)ptag->pNext;
	}
	return 1;
}

// ------------------------------------------------------------------------------------------
// Nu32 NFileReadPoolTagList
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu32 NFileReadPoolTagList(NFILE *pfile,NPOOL *ppool)
{
	// First: the current Pooltag list has to be empty !
	NErrorIf( (NNODE *)ppool->TagList.pFirst != (NNODE*)&ppool->TagList, NERROR_POOLTAG_LIST_HAS_TO_BE_EMPTY );

	Nu32					poolsize = 0;
	NPOOLTAG_FILEBLOCK_V000	htag;
	NPOOLTAG				*ptag;
	Nu32					i;
	Nu32					version;

	if( NFileReadVersionTag(pfile,&version) != 1)
		return 0;

	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (all versions 0.0.x)
		case NGETVERSION_MAIN(VERSION_NPOOL_FILEBLOCK_V000):
			if( NFileRead(pfile,&poolsize,sizeof(Nu32),1 ) != 1)
				return 0;
			
			for(i=0;i<poolsize;i++)
			{
				memset(&htag,0,sizeof(NPOOLTAG_FILEBLOCK_V000));
				if( NFileRead(pfile,&htag,sizeof(NPOOLTAG_FILEBLOCK_V000),1 ) != 1)
					return 0;

				ptag = NCreatePoolTag(ppool,htag.Type,&htag.Pos);
				ptag->Type			= htag.Type;
				ptag->User_8		= htag.User_8;
				ptag->User_16		= htag.User_16;
				ptag->A_User_32		= htag.A_User_32;
				ptag->B_User_32		= htag.B_User_32;
			}
			return 1;

		default:
			NErrorIf(1,NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}
}
