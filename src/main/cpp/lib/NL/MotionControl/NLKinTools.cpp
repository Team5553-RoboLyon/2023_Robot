#include "NLKinTools.h"

void NLKinTools::Shift(NARRAY* pkinarray, const Nf32 shift_s, const Nf32 shift_t)
{
	NLKIN* pk = (NLKIN*)pkinarray->pFirst;
	for (Nu32 i = pkinarray->Size; i != 0; i--, pk++)
	{
		pk->m_s += shift_s;
		pk->m_t += shift_t;
	}
}

void NLKinTools::Shift(NARRAY* pkinarray, const Nu32 first, const Nu32 last, const Nf32 shift_s, const Nf32 shift_t)
{
	NLKIN* pk = (NLKIN*)NGetArrayPtr(pkinarray, first);
	NLKIN* pklast = (NLKIN*)NGetArrayPtr(pkinarray, last);
	do
	{
		pk->m_s += shift_s;
		pk->m_t += shift_t;
		pk++;
	} while (pk <= pklast);
}

void NLKinTools::AddBack(NARRAY* pkinarray0, const NARRAY* pkinarray1)
{
	NLKIN* pk0 = (NLKIN*)NGetLastArrayPtr(pkinarray0);
	NLKIN* pk1 = (NLKIN*)NGetFirstArrayPtr(pkinarray1);

	NErrorIf(pk1->m_s != 0.0f, NERROR_NON_NULL_VALUE);
	NErrorIf(pk1->m_v != 0.0f, NERROR_NON_NULL_VALUE);
	NErrorIf(pk1->m_a != 0.0f, NERROR_NON_NULL_VALUE);
	NErrorIf(pk1->m_t != 0.0f, NERROR_NON_NULL_VALUE);

	NErrorIf(pk0->m_v != 0.0f, NERROR_NON_NULL_VALUE);
	NErrorIf(pk0->m_a != 0.0f, NERROR_NON_NULL_VALUE);
	Nf32 s = pk0->m_s;
	Nf32 t = pk0->m_t;
	Nu32 first = pkinarray0->Size;										// index du premier élément à shifté une fois que celui-ci est inséré dans pkinarray0
	Nu32 last = first + pkinarray1->Size - 2;							// -2 car on parle ici de l'index du dernier élément ( on va en copier 'Size - 1').
	NArrayBufferPushBack(pkinarray0, pk1 + 1, pkinarray1->Size - 1);	// -1 car on ne copie pas le premier élément	
	Shift(pkinarray0, first,last,s, t);
}
