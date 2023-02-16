#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h				**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#ifdef _NEDITOR
#include "../N/NFlags.h"

// LES FLAGS BIT_8 à BIT_31 de la struct NL2DOCS sont utilisables :
// -----------------------------------------------------------------------------------------------------------------------

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT			BIT_8	// default is FS ( en fonction de S )

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JERK			BIT_9
#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_ACCEL			BIT_10
#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL			BIT_11
#define MASK_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JAV			(BIT_9|BIT_10|BIT_11)

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK		BIT_12

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE		BIT_13
#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID	BIT_14

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN		BIT_15
#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_SUB		BIT_16

#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_CHUNK_T		BIT_17
#define FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_CHUNK_R		BIT_18

#endif