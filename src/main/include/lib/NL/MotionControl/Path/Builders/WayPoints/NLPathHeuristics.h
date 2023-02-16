#pragma once
#include "../../../../../N/NType.h"
#include "../../../DriveTrain/NLDriveTrainSpecs.h"

typedef struct	NLPATH_SECTION		NLPATH_SECTION;						// "NLPathHeuristics.h" est inclu dans "NLPathSection.h" cette ligne déclare 'NLPATH_SECTION' tout en évitant "l'inclusion cyclique"

typedef Nu32(*NLPATH_HEURISTIC)(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/);
Nu32 NLPATH_heuristic_shortest(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/);		// 0
Nu32 NLPATH_heuristic_quickest(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/);		// 1
Nu32 NLPATH_heuristic_lessCurvy(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/);		// 2	
