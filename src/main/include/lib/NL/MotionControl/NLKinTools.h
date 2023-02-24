#pragma once
#include "lib/N/Containers/NArray.h"
#include "NLKin.h"

namespace NLKinTools
{

	void Shift(NARRAY *pkinarray, const Nf32 shift_s, const Nf32 shift_t);
	void Shift(NARRAY *pkinarray, const Nu32 first, const Nu32 last, const Nf32 shift_s, const Nf32 shift_t);
	void AddBack(NARRAY *pkinarray0, const NARRAY *pkinarray1);
}
