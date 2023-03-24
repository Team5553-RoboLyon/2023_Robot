#ifndef __NUT_STATISTIC_H
#define __NUT_STATISTIC_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Statistic.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct  
{
	Nu32	Minimum;
	Nu32	Maximum;
	Nu32	Sum;
	Nu32	Samples;
}NUT_STATISTIC_Nu32;

typedef struct  
{
	Nf32	Minimum;
	Nf32	Maximum;
	Nf32	Sum;
	Nu32	Samples;
}NUT_STATISTIC_Nf32;

// STATISTICS Nf32
NUT_STATISTIC_Nf32*	NUT_SetupStatisticNf32(NUT_STATISTIC_Nf32 *pfstat);
void				NUT_ClearStatisticNf32(NUT_STATISTIC_Nf32 *pfstat);

void				NUT_AddStatisticSample_Nf32(NUT_STATISTIC_Nf32 *pfstat, Nf32 fsample);
inline Nf32			NUT_GetStatistic_Nf32_Average(const NUT_STATISTIC_Nf32 *pfstat){if(pfstat->Samples){return pfstat->Sum / pfstat->Samples;}else{return 0.0f;}}
inline Nf32			NUT_GetStatistic_Nf32_Maximum(const NUT_STATISTIC_Nf32 *pfstat){return pfstat->Maximum;}
inline Nf32			NUT_GetStatistic_Nf32_Minimum(const NUT_STATISTIC_Nf32 *pfstat){return pfstat->Minimum;}
inline Nf32			NUT_GetStatistic_Nf32_Sum(const NUT_STATISTIC_Nf32 *pfstat){return pfstat->Sum;}
inline Nu32			NUT_GetStatistic_Nf32_Samples(const NUT_STATISTIC_Nf32 *pfstat){return pfstat->Samples;}

// STATISTICS Nu32
NUT_STATISTIC_Nu32*	NUT_SetupStatisticNu32(NUT_STATISTIC_Nu32 *pstat);
void				NUT_ClearStatisticNu32(NUT_STATISTIC_Nu32 *pstat);

void				NUT_AddStatisticSample_Nu32(NUT_STATISTIC_Nu32 *pstat, Nu32 sample);
inline Nu32			NUT_GetStatistic_Nu32_Average(const NUT_STATISTIC_Nu32 *pstat){if(pstat->Samples){return pstat->Sum / pstat->Samples;}else{return 0;}}
inline Nu32			NUT_GetStatistic_Nu32_Minimum(const NUT_STATISTIC_Nu32 *pstat){return pstat->Minimum;}
inline Nu32			NUT_GetStatistic_Nu32_Maximum(const NUT_STATISTIC_Nu32 *pstat){return pstat->Maximum;}
inline Nu32			NUT_GetStatistic_Nu32_Sum(const NUT_STATISTIC_Nu32 *pstat){return pstat->Sum;}
inline Nu32			NUT_GetStatistic_Nu32_Samples(const NUT_STATISTIC_Nu32 *pstat){return pstat->Samples;}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	//	__cpluplus
#endif //	__NUT_STATISTIC_H

