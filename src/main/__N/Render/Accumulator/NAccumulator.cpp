#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../GL/Ngl.h"
#include "../../Containers/NArray.h"
#include "../../NTransformationTree.h"
#include "../../Geometry/NGeometry.h"
#include "../../NViewport.h"
#include "../../NErrorHandling.h"
#include "../NRenderCell.h"

#include "NAccumulator.h"
#include "NAccumulatorRendering.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Globals 
NACCUMULATOR			NAccumulator[NACCUM_ID_ENUM_SIZE];

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NACCUMULATOR*	NSetupAccumulator(NACCUMULATOR *pacc, const ACCUMULATOR_RENDER_FCT	render_function, NSTATESET stateset )
{
	pacc->RenderAccumulator	= (ACCUMULATOR_RENDER_FCT)render_function;
	NSetupArray(&pacc->AccumulatedRenderCellsArray,DEFAULT_NACCUMULATOR_CAPACITY,sizeof(NRENDERCELL*));
	pacc->StateSet = stateset;

	return pacc;
}
void NClearAccumulator(NACCUMULATOR *pacc)
{
	pacc->RenderAccumulator	= NULL;
	NClearArray(&pacc->AccumulatedRenderCellsArray, NULL );
	pacc->StateSet = 0;
}

void NInit_Accumulators()
{
	NSetupAccumulator( &NAccumulator[NACCUM_FLAT], NRenderAccumulator_NoBlendNoLight, FLAG_STATESET_ZWRITE|FLAG_STATESET_ZREAD );
	NSetupAccumulator( &NAccumulator[NACCUM_TEXTURES], NRenderAccumulator_NoBlendNoLight, FLAG_STATESET_ZWRITE|FLAG_STATESET_ZREAD|FLAG_STATESET_TEXTURES );
	NSetupAccumulator( &NAccumulator[NACCUM_TEXTURES_LIGHT], NRenderAccumulator_NoBlendLight, FLAG_STATESET_ZWRITE|FLAG_STATESET_ZREAD|FLAG_STATESET_TEXTURES|FLAG_STATESET_LIGHT );
	NSetupAccumulator( &NAccumulator[NACCUM_TEXTURES_BLEND_LIGHT], NRenderAccumulator_BlendLight,FLAG_STATESET_ZREAD|FLAG_STATESET_TEXTURES|FLAG_STATESET_BLEND|FLAG_STATESET_LIGHT );
	NSetupAccumulator( &NAccumulator[NACCUM_TEXTURES_BLEND], NRenderAccumulator_BlendNoLight, FLAG_STATESET_ZREAD|FLAG_STATESET_TEXTURES|FLAG_STATESET_BLEND);
	NSetupAccumulator( &NAccumulator[NACCUM_HUD], NRenderAccumulator_HUD, 0 );
}

void NDisable_Accumulators()
{
	NClearAccumulator(&NAccumulator[NACCUM_FLAT]);
	NClearAccumulator(&NAccumulator[NACCUM_TEXTURES]);
	NClearAccumulator(&NAccumulator[NACCUM_TEXTURES_LIGHT]);
	NClearAccumulator(&NAccumulator[NACCUM_TEXTURES_BLEND_LIGHT]);
	NClearAccumulator(&NAccumulator[NACCUM_TEXTURES_BLEND]);
	NClearAccumulator(&NAccumulator[NACCUM_HUD]);
}

NACCUM_ID_ENUM NGetAccumulatorID( const NSTATESET stateset_flags )
{
	// the debug version check if the "looking for" stateset exists and is unique and make Errors if not.
#ifdef _DEBUG
	NACCUM_ID_ENUM id = NACCUM_ID_ENUM_SIZE;
	Nu32 found = 0;
	for(Nu32 i=0;i<NACCUM_ID_ENUM_SIZE;i++)
	{
		if( NAccumulator[i].StateSet == stateset_flags )
		{
			if(found == 0)
				id = (NACCUM_ID_ENUM)i;	

			found ++;
		}
	}
	if( found == 0 )
	{
		NErrorIf(1,NERROR_ACCUMULATOR_NOT_FOUND); 
	}
	if( found > 1 )
	{
		NErrorIf(1,NERROR_ACCUMULATOR_MULTIPLE_RESULTS); 
	}
	return id; // return NSTATESET_ID_ENUM_SIZE will cause a crash ...
#endif
#ifndef _DEBUG
	for(Nu32 i=0;i<NACCUM_ID_ENUM_SIZE;i++)
	{
		if( NAccumulator[i].StateSet == stateset_flags )
			return (NACCUM_ID_ENUM)i;
	}
	// We shouldn't pass here ... but just in case return an out of range value which will cause a crash !!!
	return NACCUM_ID_ENUM_SIZE;
#endif
}
