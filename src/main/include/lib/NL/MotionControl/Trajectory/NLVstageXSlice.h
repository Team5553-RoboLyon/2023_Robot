#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "../../../N/NMemory.h"
#include "../../../N/Containers/NArray.h"
#include "../NLKinLimits.h"
#include "../NLKinTweak.h"
#include "NLTrajectoryPack.h"
#include "NLTrajectoryKey.h"
#include "NLTrajectoryPointDesc.h"
#include "NLVStageX.h"

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
#endif

#define CONSTANT_NLVSTAGEXSLICE_INITIAL_VSTAGEX_ARRAYCAPACITY		4
//typedef struct NLVSTAGE NLVSTAGE;	// forward declaration ( NLTrim.h sera déclaré dans NLVstage.cpp)
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