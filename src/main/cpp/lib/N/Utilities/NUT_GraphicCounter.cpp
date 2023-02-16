#include "../NCStandard.h"
#include "../Containers/NArray.h"
#include "../Geometry/NGeometry.h"
#include "../NErrorHandling.h"
#include "NUT_GraphicCounter.h"

// ------------------------------------------------------------------------------------------
// NSetupGraphicCounter
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a GraphicCounter. Be aware that setup doesn't create the graphic object (geometries)
//	It setup the manager that will be used to update those geometries according with the 
//	graphic counter value(s).
//	In other words it will, increase/decrease the number of used vertex/primitive ( without really removed them)
//	in relation with the current value of the "counter".
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NUT_GRAPHICCOUNTER* NUT_SetupGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const NUT_GRAPHICCOUNTER_DESC *pdesc)
{
	NErrorIf(!pdesc,NERROR_NULL_POINTER);
	NErrorIf(!pdesc->pGeometry, NERROR_NULL_POINTER);

	memset(pcounter,0,sizeof(NUT_GRAPHICCOUNTER));
	// CORE
	pcounter->pGeometry					= pdesc->pGeometry;
	pcounter->PrimitiveArraySizeInitial = pdesc->pGeometry->Mesh.PrimitiveArray.Size;
	
	// OPTIONAL
	if(ISFLAG_ON(pdesc->Flags,FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_STEP))
	{
		pcounter->PrimitiveStep = pdesc->PrimitiveStep;
	}
	else
	{
		pcounter->PrimitiveStep = 1;
	}
	if(ISFLAG_ON(pdesc->Flags,FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_ARRAY_SIZE_MIN))
	{
		pcounter->PrimitiveArraySizeMin = pdesc->PrimitiveArraySizeMin;
	}
	else
	{
		pcounter->PrimitiveArraySizeMin = 0;
	}
	if(ISFLAG_ON(pdesc->Flags,FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_ARRAY_SIZE_MAX))
	{
		pcounter->PrimitiveArraySizeMax = pdesc->PrimitiveArraySizeMax;
	}
	else
	{
		pcounter->PrimitiveArraySizeMax = pdesc->pGeometry->Mesh.PrimitiveArray.Size;
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + CHECK CONSISTENCY
	// + Some debug checks
	//		Most important thing: we need to have enough primitives already setup into the geometry
	//		To manage all the value range until max.
	NErrorIf(pcounter->PrimitiveArraySizeMin > pcounter->PrimitiveArraySizeInitial,NERROR_WRONG_VALUE);
	NErrorIf(pcounter->PrimitiveArraySizeMax > pcounter->PrimitiveArraySizeInitial,NERROR_WRONG_VALUE);
	NErrorIf(pcounter->PrimitiveArraySizeMax <= pcounter->PrimitiveArraySizeMin,NERROR_WRONG_VALUE);

#ifdef _DEBUG
	Ns32 diff = pcounter->PrimitiveArraySizeMax - pcounter->PrimitiveArraySizeMin;
	Ns32 a = diff/pcounter->PrimitiveStep;
	// The number of primitives (Difference between sizemax and sizemin) used by the GFX counter
	// has to be a perfect multiple of PrimitiveStep !!!  
	NErrorIf((Ns32)(a*pcounter->PrimitiveStep) != diff, NERROR_INCONSISTENT_PARAMETERS); 
#endif
	// COMPUTED 
	pcounter->ValueMax = (pcounter->PrimitiveArraySizeMax-pcounter->PrimitiveArraySizeMin)/pcounter->PrimitiveStep;
	NErrorIf( pdesc->pGeometry->Mesh.PrimitiveArray.Size < (Nu32)(pcounter->ValueMax*pdesc->PrimitiveStep), NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE );
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + INITIAL VALUE
	// +
	if(ISFLAG_ON(pdesc->Flags,FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_INITIALVALUE))
	{
		pcounter->CurrentValue = NMIN(pdesc->InitialValue,pcounter->ValueMax);

	}
	else
	{
		pcounter->CurrentValue = 0;
	}
	pcounter->pGeometry->Mesh.PrimitiveArray.Size = pcounter->PrimitiveArraySizeMin + (pcounter->CurrentValue * pcounter->PrimitiveStep); 
	return pcounter;
}

// ------------------------------------------------------------------------------------------
// NCreateGraphicCounter
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NUT_GRAPHICCOUNTER* NUT_CreateGraphicCounter(const NUT_GRAPHICCOUNTER_DESC *pdesc)
{
	return NUT_SetupGraphicCounter(NEW(NUT_GRAPHICCOUNTER),pdesc);
}

void NUT_ClearGraphicCounter(void *pcounter)
{
	// Before set all values to 0
	// ... restore geometry initial primitive array size ...
	NResizeArray(&((NUT_GRAPHICCOUNTER*)pcounter)->pGeometry->Mesh.PrimitiveArray,((NUT_GRAPHICCOUNTER*)pcounter)->PrimitiveArraySizeInitial,NULL,NULL);
	memset(pcounter,0,sizeof(NUT_GRAPHICCOUNTER));
}

void NUT_DeleteGraphicCounter(void *pcounter)
{
	NUT_ClearGraphicCounter(pcounter);
	Nfree(pcounter);
}

void NUT_IncreaseGraphicCounter(NUT_GRAPHICCOUNTER *pcounter)
{
	if(pcounter->CurrentValue < pcounter->ValueMax )
	{
		pcounter->CurrentValue += 1;
		pcounter->pGeometry->Mesh.PrimitiveArray.Size = pcounter->PrimitiveArraySizeMin + (pcounter->CurrentValue * pcounter->PrimitiveStep); 
	}
}

void NUT_DecreaseGraphicCounter(NUT_GRAPHICCOUNTER *pcounter)
{
	if(pcounter->CurrentValue > 0 )
	{
		pcounter->CurrentValue -= 1;
		pcounter->pGeometry->Mesh.PrimitiveArray.Size = pcounter->PrimitiveArraySizeMin + (pcounter->CurrentValue * pcounter->PrimitiveStep); 
	}
}

void NUT_SetGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const Nu32 countervalue)
{
	pcounter->CurrentValue = NMIN(countervalue,pcounter->ValueMax);
	pcounter->pGeometry->Mesh.PrimitiveArray.Size = pcounter->PrimitiveArraySizeMin + (pcounter->CurrentValue * pcounter->PrimitiveStep); 
}

void NUT_AddToGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const Ns32 add_countervalue)
{
	if(add_countervalue < 0)
	{
		if((Nu32)NABS(add_countervalue)>= pcounter->CurrentValue)
			pcounter->CurrentValue = 0;
		else
			pcounter->CurrentValue += add_countervalue;
	}
	else
	{
		pcounter->CurrentValue = NMIN(pcounter->CurrentValue+add_countervalue,pcounter->ValueMax);
	}
	pcounter->pGeometry->Mesh.PrimitiveArray.Size = pcounter->PrimitiveArraySizeMin + (pcounter->CurrentValue * pcounter->PrimitiveStep); 
}
