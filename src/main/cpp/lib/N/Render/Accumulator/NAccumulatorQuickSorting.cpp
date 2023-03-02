#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../Containers/NArray.h"
#include "../../NErrorHandling.h"
#include "../NRenderCell.h"
#include "NAccumulator.h"
#include "NAccumulatorQuickSorting.h"

// Accumulator Sorting:
// What is the sorting Key Here ?

void NQuickSortAccumulatorBTF(NRENDERCELL **pprcell, const Ns32 start, const Ns32 end)
{
	NRENDERCELL	*ptemp;
    Ns32 left = start - 1;
    Ns32 right = end + 1;
    const Nf32 pivot = ( (NRENDERCELL*)pprcell[start] )->SortingKey;

    // Nothing to do if array size is null
    if(start >= end)
        return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
    while(1)
    {
        do right--; while( ( (NRENDERCELL*)pprcell[right] )->SortingKey < pivot);
        do left++; while( ( (NRENDERCELL*)pprcell[left] )->SortingKey > pivot);

        if(left < right)
		{
			ptemp = pprcell[left];
			pprcell[left] = pprcell[right];
			pprcell[right] = ptemp;
		}
        else break;
    }

	// Now all the element lower than the pivot are before the ones greater than the pivot.
	// So, we have 2 groups to sort again. To do that, we are going to use quicksort itself !
    NQuickSortAccumulatorBTF(pprcell, start, right);
    NQuickSortAccumulatorBTF(pprcell, right+1, end);
}

void NQuickSortAccumulatorFTB(NRENDERCELL **pprcell, const Ns32 start, const Ns32 end)
{
	NRENDERCELL	*ptemp;
    Ns32		left	= start - 1;
    Ns32		right	= end	+ 1;
    const Nf32	pivot	= ( (NRENDERCELL*)pprcell[start] )->SortingKey;

	// Nothing to do if array size is null
    if(start >= end)
        return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
    while(1)
    {
        do right--; while( ( (NRENDERCELL*)pprcell[right] )->SortingKey > pivot);
        do left++; while( ( (NRENDERCELL*)pprcell[left] )->SortingKey < pivot);

        if(left < right)
		{
			ptemp			= pprcell[left];
			pprcell[left]	= pprcell[right];
			pprcell[right]	= ptemp;
		}
        else break;
    }

	// Now all the element lower than the pivot are before the ones greater than the pivot.
	// So, we have 2 groups to sort again. To do that, we are going to use quicksort itself !
    NQuickSortAccumulatorFTB(pprcell, start, right);
    NQuickSortAccumulatorFTB(pprcell, right+1, end);
}
