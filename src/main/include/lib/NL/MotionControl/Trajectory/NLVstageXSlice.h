#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NMemory.h"
#include "lib/N/Containers/NArray.h"

#include "lib/NL/MotionControl/NLKinLimits.h"

#include "lib/NL/MotionControl/NLKinTweak.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryKey.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPointDesc.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageX.h"
#ifdef _NEDITOR
#include "lib/NL/MotionControl/NL2DOrthogonalCoordinateSystem.h"
#endif

#define CONSTANT_NLVSTAGEXSLICE_INITIAL_VSTAGEX_ARRAYCAPACITY		4
//typedef struct NLVSTAGE NLVSTAGE;	// forward declaration ( NLTrim.h sera d�clar� dans NLVstage.cpp)
typedef struct NLVSTAGEXSLICE NLVSTAGEXSLICE;
struct NLVSTAGEXSLICE
{
	NLVSTAGEXSLICE();
	~NLVSTAGEXSLICE();

	Nu32		prepare(const NARRAY* pvstage_array, const NLKINLIMITS* plocal_limits, const NLKINTWEAK* pktweak, const NLTRJKEY* pk0, const NLTRJKEY* pk1);
	void		BuildSolutionTree();
	NLTRJPOINT_DESC_CFG	Promote(const NLTRJPOINT_DESC_CFG cfg, const Nf32 t, NARRAY* pkin_out);

	#ifdef _NEDITOR
	void		drawVStages(NLTRAJECTORY_PACK* ppack, NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
	void		drawKnodes(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
	#endif

	NARRAY		m_vStageXArray;
	NARRAY		m_knodeArray;
	Nu32		m_RequiredKinArraySize;

	NLTRJKEY*	m_pKey0;
	NLTRJKEY*	m_pKey1;
};

// Clear call back pour NArray
inline void NLclearVStageXSliceInArrayCallBack(void* p)
{
	NLVSTAGEXSLICE* pvxs = (NLVSTAGEXSLICE*)p;
	pvxs->~NLVSTAGEXSLICE();
}