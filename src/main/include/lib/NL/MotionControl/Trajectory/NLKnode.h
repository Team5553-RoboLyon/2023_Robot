#pragma once
#include "../../../N/NType.h"
#include "../../../N/Maths/NVec2f32.h"
#include "../../../N/Core/NLimits.h"

#include "../NLKinLimits.h"
#include "../../NL2DOrthogonalCoordinateSystem.h"
#include "../MotionProfiles/NLSmotionProfile.h"

#include "NLVstageX.h"


typedef struct NLKNODE NLKNODE;
struct NLKNODE
{
	// Membres:
	// -----------------------------------------------------------------------------------
	NLSMOTIONPROFILE	m_motionProfile;

	NLVSTAGEX	*m_pVfrom;
	NLVSTAGEX	*m_pVto;

	// Liens entre les différents NLKNODE associés
	NLKNODE		*m_pParent;		// NLKNODE parent
	NLKNODE		*m_pSibling;	// NLKNODE frere
	NLKNODE		*m_pLeftChild;	// NLKNODE fils |_ Un NLKNODE est splitable en 2 fils, un a droite et l'autre à gauche du NLVSTAGEX de split
	NLKNODE		*m_pRightChild;	// NLKNODE fils |

	// Méthodes:
	// -----------------------------------------------------------------------------------
	inline void	setup(NLVSTAGEX *pfrom, NLVSTAGEX *pto, NLKNODE *psibling, NLKNODE *pparent)
	{
		//m_motionProfile.
		m_motionProfile.initialize();

		m_pVfrom			= pfrom;
		m_pVto				= pto;
		
		m_pParent			= pparent;
		m_pSibling			= psibling;
		m_pLeftChild		= NULL;
		m_pRightChild		= NULL;

	}

	inline void	children(NLKNODE *pleft, NLKNODE *pright)
	{
		NErrorIf(m_pLeftChild || m_pRightChild, NERROR_NON_NULL_POINTER);
		m_pLeftChild = pleft;
		m_pRightChild = pright;
	}

	Nf32			getHighestVelocity();
	NLVSTAGEX*		getHighestCeilVStage();

	NLVSTAGEX*		getSplitVStage();
	NLVSTAGEX*		deepSolveAndGetSplitVStage();
	Nu32			promote(NLKIN * pkbuffer);
	inline Nf32		getFullTimeSpan() {	return m_motionProfile.getEffectiveTimeSpan() + m_pVfrom->getPrimeFlatShapeTimeSpan();}
	inline Nf32		getChildrenFullTimeSpan(){	return (m_pLeftChild) ? (m_pLeftChild->getFullTimeSpan() + m_pRightChild->getFullTimeSpan() ):NF32_MAX;}
};
#define NLClearKnodeInArrayCallBack		NULL
//inline void NLClearKnodeInArrayCallBack(void*p)