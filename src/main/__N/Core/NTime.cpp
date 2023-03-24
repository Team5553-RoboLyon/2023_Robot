// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTime.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NTime.h"

NTIME* NSetupTime(NTIME *ptime)
{
	Nmem0(ptime,NTIME);

#ifdef _NWINDOWS
// 	timeBeginPeriod(1);
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&ptime->Nu64_Frequency);
	ptime->Nf64_InvFrequency = (Nf32)((Nf64)1.0/(Nf64)ptime->Nu64_Frequency);
	
	QueryPerformanceCounter((LARGE_INTEGER*)&ptime->Nu64_Start);
	ptime->Nu64_Current	 = ptime->Nu64_Start;
	ptime->Nu64_Previous = ptime->Nu64_Start;
	ptime->Nu64_Delay	 = 0;

	ptime->Nf64_Start	= (Nf64)((Nf64)ptime->Nu64_Start * ptime->Nf64_InvFrequency); 
	ptime->Nf64_Current = ptime->Nf64_Start;
	ptime->Nf64_Previous= ptime->Nf64_Start;
	ptime->Nf32_Delay	= 0.0f;
#endif

#ifdef _NIOS
	ptime->Nf64_Start	= (Nf32)CACurrentMediaTime();
	ptime->Nf64_Current = ptime->Nf64_Start;
	ptime->Nf64_Previous= ptime->Nf64_Start;
	ptime->Nf32_Delay	= 0.0f;
#endif

#ifdef _NANDROID
	ptime->Nf64_Start	= 0.0f;
	ptime->Nf64_Current = ptime->Nf64_Start;
	ptime->Nf64_Previous= ptime->Nf64_Start;
	ptime->Nf32_Delay	= 0.0f;
#endif

	return ptime;
}
NTIME* NCreateTime()
{
	return NSetupTime(NEW(NTIME));
}
void NClearTime(NTIME *ptime)
{
	Nmem0(ptime,NTIME);
}
void NDeleteTime(NTIME *ptime)
{
	NClearTime(ptime);
	Nfree(ptime);
}

void NUpdateTime(NTIME *ptime)
{
	#ifdef _NWINDOWS
		ptime->Nu64_Previous	= ptime->Nu64_Current;
		QueryPerformanceCounter((LARGE_INTEGER*)&ptime->Nu64_Current);
		ptime->Nu64_Delay		= ptime->Nu64_Current - ptime->Nu64_Previous;

		ptime->Nf64_Previous = ptime->Nf64_Current;
		ptime->Nf64_Current	= (Nf64)((Nf64)ptime->Nu64_Current * ptime->Nf64_InvFrequency); 
		ptime->Nf32_Delay	= (Nf32)((Nf64)ptime->Nu64_Delay * ptime->Nf64_InvFrequency);
		
 		if(ptime->Nf32_Delay > 1.0f/60.0f)
 			ptime->Nf32_Delay = 1.0f/60.0f;

		//ptime->Nu32_Tick ++; maybe its not the right place to update it. Better to do at the Start or at the End of "Render"

		//printf("\n Nf32_Delay:%.6f",ptime->Nf32_Delay);
	#endif
	#ifdef _NIOS
		//CFTimeInterval startTime = CACurrentMediaTime();	
		ptime->Nf64_Previous	= ptime->Nf64_fCurrent;
		ptime->Nf64_Current		= (Nf64)CACurrentMediaTime();
		ptime->Nf32_Delay		= (Nf32)(ptime->Nf64_Current - ptime->Nf64_Previous);

		//TODO : timegettime ios equivalence 

		//MainTimer.fPrevious = MainTimer.fCurrent;
		//MainTimer.Nf32_Delay = 1.0f/60.0f;
		//MainTimer.fCurrent += MainTimer.Nf32_Delay;
	#endif

	#ifdef _NANDROID
			ptime->Nf32_Delay = 1.0f/60.0f;
	#endif
}