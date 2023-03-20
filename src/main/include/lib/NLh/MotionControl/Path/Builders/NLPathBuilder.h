#pragma once
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/NL/MotionControl/Path/NLPath.h"

#ifdef _NEDITOR
#include "lib/NL/MotionControl/NL2DOrthogonalCoordinateSystem.h"
#endif

class NLPATH_BUILDER
{
public:
	NLPATH_BUILDER() {}
	virtual ~NLPATH_BUILDER() {}
	virtual void build(NLPATH *ppath){};
	virtual Nu32 read(FILE *pfile) { return 0; };
	virtual Nu32 write(FILE *pfile) { return 0; };

#ifdef _NEDITOR
	virtual void draw(){};
	virtual void draw(NL2DOCS *pocs){};
#endif
};

/*
#define FLAG_NLPATH_BUILDER_TARGET_BUILD_REQUEST				BIT_31		// RESERVED by NLPATH_BUILDER
//#define FLAG_NLPATH_BUILDER_xxx								BIT_30		// Available for derived builder
//																.
//																.
//																.
//#define FLAG_NLPATH_BUILDER_xxx								BIT_0		// Available for derived builder

class NLPATH_BUILDER
{

public:
	NLPATH_BUILDER() { m_pPathTarget = NULL; }
	NLPATH_BUILDER(NLPATH* ppathtarget) { m_pPathTarget = ppathtarget; }
	virtual ~NLPATH_BUILDER() {}
	virtual void build(NLPATH* ppath) {};

	inline void buildTarget() { if (m_pPathTarget)build(m_pPathTarget); }
	inline void setTarget(NLPATH* ppathtarget) { m_pPathTarget = ppathtarget; if (!ppathtarget)FLAG_OFF(m_flags, FLAG_NLPATH_BUILDER_TARGET_BUILD_REQUEST); }	// On annule une potentielle demande de build si le path target devient nulle

	inline void requestTargetBuild() { if (m_pPathTarget)FLAG_ON(m_flags, FLAG_NLPATH_BUILDER_TARGET_BUILD_REQUEST); }											// On ne prend en compte la demande que si un path est actuellement referencé comme cible.
	inline Nu32 buildTargetOnRequest() { if (ISFLAG_ON(m_flags, FLAG_NLPATH_BUILDER_TARGET_BUILD_REQUEST)) { build(m_pPathTarget); return 1; } else { return 0; } }
#ifdef _NEDITOR
	virtual void draw() {};
	virtual void draw(NL2DOCS* pocs) {};
#endif
protected:
	Nu32	m_flags;
	NLPATH* m_pPathTarget;
};
*/