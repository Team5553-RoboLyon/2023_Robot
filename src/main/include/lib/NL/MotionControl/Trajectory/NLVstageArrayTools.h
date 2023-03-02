#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "../../../N/Containers/NArray.h"
#include "NLVstage.h"

namespace NLVstageArrayTools
{
	#define COMBINE_RESET_CUTTERS	0
	#define COMBINE_CHAINED_CUTTERS	1

	Nu32 insertVelocityStage(NARRAY* parray, NLVSTAGE* pnewvstage, const Nu32 mode);
	Nu32 slice(NARRAY* pout, const NARRAY* pin, Nf32 s0, Nf32 s1, const Nbool binsert_start_end);
	Nu32 combine(NARRAY* pout, const NARRAY* pin0, const NARRAY* pin1, const Nu32 in0_cutters_desc, const Nu32 in1_cutters_desc);
}