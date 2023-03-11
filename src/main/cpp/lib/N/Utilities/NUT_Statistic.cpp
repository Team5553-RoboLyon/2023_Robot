#include "lib/N/NCStandard.h"
#include "lib/N/Core/NLimits.h"
#include "lib/N/NMath.h"
#include "NUT_Statistic.h"

NUT_STATISTIC_Nf32 *NUT_SetupStatisticNf32(NUT_STATISTIC_Nf32 *pfstat)
{
	pfstat->Maximum = 0.0f;
	pfstat->Minimum = NF32_MAX;
	pfstat->Sum = 0.0f;
	pfstat->Samples = 0;

	return pfstat;
}

void NUT_ClearStatisticNf32(NUT_STATISTIC_Nf32 *pfstat)
{
	Nmem0(pfstat, NUT_STATISTIC_Nf32);
}

void NUT_AddStatisticSample_Nf32(NUT_STATISTIC_Nf32 *pfstat, Nf32 fsample)
{
	pfstat->Minimum = NMIN(fsample, pfstat->Minimum);
	pfstat->Maximum = NMAX(fsample, pfstat->Maximum);
	pfstat->Sum += fsample;
	pfstat->Samples++;
}

NUT_STATISTIC_Nu32 *NUT_SetupStatisticNu32(NUT_STATISTIC_Nu32 *pstat)
{
	pstat->Maximum = 0;
	pstat->Minimum = NU32_MAX;
	pstat->Sum = 0;
	pstat->Samples = 0;

	return pstat;
}
void NUT_ClearStatisticNu32(NUT_STATISTIC_Nu32 *pstat)
{
	Nmem0(pstat, NUT_STATISTIC_Nu32);
}
void NUT_AddStatisticSample_Nu32(NUT_STATISTIC_Nu32 *pstat, Nu32 sample)
{
	pstat->Minimum = NMIN(sample, pstat->Minimum);
	pstat->Maximum = NMAX(sample, pstat->Maximum);
	pstat->Sum += sample;
	pstat->Samples++;
}
