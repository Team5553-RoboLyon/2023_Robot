#ifndef __NUT_GRAPHICCOUNTER_H
#define __NUT_GRAPHICCOUNTER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_GraphicCounter.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	/*
	typedef struct
	{
		NGEOMETRY	*pGeometry;
		Nu32		InitialGeometryPrimitiveArraySize;
		Nu16		ValueMin;
		Nu16		ValueMax;
		Nu16		CurrentValue;
		Nu16		PrimitiveStep;	// how many geometry primitives for 1 ?
	}NUT_GRAPHICCOUNTER;

	typedef struct
	{
		NGEOMETRY	*pGeometry;
		Nu16		ValueMin;
		Nu16		ValueMax;
		Nu16		InitialValue;
		Nu16		PrimitiveStep;	// how many geometry primitives for 1 ?
	}NUT_GRAPHICCOUNTER_DESC;
	*/
	typedef struct
	{
		NGEOMETRY *pGeometry;
		Nu32 CurrentValue;
		Nu32 ValueMax;
		Nu32 PrimitiveStep;

		Nu32 PrimitiveArraySizeMin;
		Nu32 PrimitiveArraySizeMax;
		Nu32 PrimitiveArraySizeInitial;
	} NUT_GRAPHICCOUNTER;

#define FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_INITIALVALUE BIT_0
#define FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_STEP BIT_2
#define FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_ARRAY_SIZE_MIN BIT_3
#define FLAG_NUT_GRAPHICCOUNTER_DESC_USEPARAM_PRIMITIVE_ARRAY_SIZE_MAX BIT_4
	typedef struct
	{
		Nu32 Flags;
		NGEOMETRY *pGeometry;
		Nu32 InitialValue;
		Nu32 PrimitiveArraySizeMin;
		Nu32 PrimitiveArraySizeMax;
		Nu32 PrimitiveStep;
	} NUT_GRAPHICCOUNTER_DESC;

	NUT_GRAPHICCOUNTER *NUT_SetupGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const NUT_GRAPHICCOUNTER_DESC *pdesc);
	NUT_GRAPHICCOUNTER *NUT_CreateGraphicCounter(const NUT_GRAPHICCOUNTER_DESC *pdesc);
	void NUT_ClearGraphicCounter(void *pcounter);
	void NUT_DeleteGraphicCounter(void *pcounter);

	void NUT_IncreaseGraphicCounter(NUT_GRAPHICCOUNTER *pcounter);
	void NUT_DecreaseGraphicCounter(NUT_GRAPHICCOUNTER *pcounter);
	void NUT_SetGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const Nu32 countervalue);
	void NUT_AddToGraphicCounter(NUT_GRAPHICCOUNTER *pcounter, const Ns32 add_countervalue);
	inline Nu32 NUT_GetGraphicCounterValue(const NUT_GRAPHICCOUNTER *pcounter) { return pcounter->CurrentValue; }

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_GRAPHICCOUNTER_H
