#include "../NCStandard.h"
#include "../NType.h"
#include "NUIClearWatchStack.h"


extern NARRAY	ActiveUIRootsList;
extern NUI		*pFocusedUI;
extern Nu32		NEngineCoreFlags;



NARRAY UIClearWatchStack;

void NInitUIClearWatchStack()
{
	NSetupArray(&UIClearWatchStack,CONSTANT_UICLEARWATCHSTACK_INITIAL_CAPACITY,sizeof(NUI*));
}

void NQuitUIClearWatchStack()
{
	NClearArray(&UIClearWatchStack,NULL);
}

void NUIClearWatchStackPush(const NUI *pui)
{
	NArrayPushBack(&UIClearWatchStack,(NBYTE*)&pui);
//	printf("\n+NUIClearWatchStackPush. Stack Size = %d",UIClearWatchStack.Size);
}
void NUIClearWatchStackPoke(const NUI *pui)
{
	NErrorIf(UIClearWatchStack.Size==0,NERROR_ARRAY_IS_EMPTY);
	((NUI**)(UIClearWatchStack.pFirst))[UIClearWatchStack.Size-1] = (NUI*)pui; 
//	printf("\n+NUIClearWatchStackPush. Stack Size = %d",UIClearWatchStack.Size);
}

NUI* NUIClearWatchStackPop()
{
	NErrorIf(UIClearWatchStack.Size==0,NERROR_ARRAY_IS_EMPTY);
	NUI **ptr = (NUI**)NGetLastArrayPtr(&UIClearWatchStack); 
	UIClearWatchStack.Size--;
//	printf("\n+NUIClearWatchStackPop. Stack Size = %d",UIClearWatchStack.Size);
	return *ptr;
}

NUI* NUIClearWatchStackPeek()
{
	NErrorIf(UIClearWatchStack.Size==0,NERROR_ARRAY_IS_EMPTY);
//	printf("\n+NUIClearWatchStackPop. Stack Size = %d",UIClearWatchStack.Size);
	return *(NUI**)NGetLastArrayPtr(&UIClearWatchStack); 
}

void NUIClearWatchStackRemove()
{
	NErrorIf(UIClearWatchStack.Size==0,NERROR_ARRAY_IS_EMPTY);
	UIClearWatchStack.Size--;
//	printf("\n+NUIClearWatchStackRemove. Stack Size = %d",UIClearWatchStack.Size);
}


NUI* NUIClearWatchStackPeekI(const Nu32 index)
{
	return *((NUI**)NGetArrayPtr(&UIClearWatchStack,index));
}

void NDecreaseUIClearWatchStackSize(const Nu32 decrease)
{
	NErrorIf(UIClearWatchStack.Size < decrease ,NERROR_ARRAY_WRONG_SIZE);
	UIClearWatchStack.Size -= decrease;
// 	printf("\n+NDecreaseUIClearWatchStackSize. Stack Size = %d",UIClearWatchStack.Size);
}

Nu32 NGetUIClearWatchStackSize()
{
	return UIClearWatchStack.Size;
}

void NSetUIClearWatchStackSize(const Nu32 size)
{
	NErrorIf(size > UIClearWatchStack.Capacity ,NERROR_ARRAY_WRONG_SIZE);
	UIClearWatchStack.Size = size;
// 	printf("\n+NSetUIClearWatchStackSize. Stack Size = %d",UIClearWatchStack.Size);
}

void NUIClearWatchStack_ClearNotification(const NUI *pui)
{
	NUI **ptr = (NUI**)UIClearWatchStack.pFirst;
	for(Nu32 i=UIClearWatchStack.Size;i!=0;i--, ptr ++)
	{
		if(*ptr == pui)
			*ptr = NULL;
	}
}
