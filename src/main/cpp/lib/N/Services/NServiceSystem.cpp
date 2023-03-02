// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NServiceSystem.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NString.h"

#ifdef _NANDROID
#include "../System/NAndroid.h"
#endif

#include "./NServiceSystem.h"

void NDisplayMessageBox(const Nchar *ptitle_str, const Nchar *pmessage_str)
{
#ifdef _NANDROID
	NBuildJNICall_2String("ShowDialog", ptitle_str, pmessage_str);
#endif
}
void NMinimizeApp()
{
#ifdef _NANDROID
	NBuildJNICall("mr_MinimizeApp");
#endif
}

/*
#ifdef _NANDROID
// To inform ANDROID/JAVA, that
void NServiceSystem_AppInitializationCompleted()
{
	NANDROIDTASK_SYSTEM_MESSAGE* ptask = (NANDROIDTASK_SYSTEM_MESSAGE*)NAndroidTaskAllocBack();
	ptask->Type			= _NANDROIDTASK_SYSTEM_INIT_COMPLETED;
	ptask->Param_Nu16	= 0;
}
#endif
*/
