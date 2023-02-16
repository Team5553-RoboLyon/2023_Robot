#pragma once
#include "../../../../../N/NType.h"
#include "../../../../../N/Miscellaneous/NColor.h"
#include "../../../../../N/Maths/NVec2f32.h"
#include "../../../../../N/Containers/NArray.h"

#include "NLPathSectionBuildCircle.h"



typedef struct NLPATH_SECTION_BPAIR		NLPATH_SECTION_BPAIR;
struct NLPATH_SECTION_BPAIR
{
	friend struct NLPATH_SECTION;

	enum NLPATH_BPAIRTYPE
	{
		TYPE_UNDEFINED = 0,
		TYPE_C = 1,
		TYPE_S = 2
	};

	Nu16					type;
	Nu16					flags;

	NLPATH_SECTION_BCIRCLE	*pcircle0;
	NLPATH_SECTION_BCIRCLE	*pcircle1;

	Nf32					alpha;  // Transition en C et S. Les points de contact des tangentes [sur les cercles] sont situés à un angle alpha de part et d'autre de l'axe o0o1 [ reliant les centres des deux cercles]. ( alpha = PI/2 pour TYPE_C )
	Nf32					beta0;	// Distance angulaire entre le point P0 et le point de sortie S sur le cercle C0 et en respectant le sens du cercle. 
	Nf32					beta1;  // Distance angulaire entre le point E d'entrée sur le cercle 1 et le point P1 et en respectant le sens du cercle. 

	NVEC2					s;		// point de sortie situé sur le cercle 0
	NVEC2					e;		// point d'entrée situé sur le cercle 1
	NVEC2					se;		// vecteur 'sortie-entrée' ( dont la longueur avant normalisation est LA longueur du segment "tangente" se)
	Nf32					lse;	// longueur du vecteur 'sortie-entrée' calculé avant normalisation.

	//NLPATH_SECTION_BCIRCLE	*pc0;
	//NLPATH_SECTION_BCIRCLE	*pc1;

	void setup(const NLPATH_WAYPOINT *pwp0, const NLPATH_SECTION_BCIRCLE *pc0, const NLPATH_WAYPOINT *pwp1, const NLPATH_SECTION_BCIRCLE *pc1);
#ifdef _NEDITOR
private:
	void draw(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR *pcol);
	void drawAlphaAngles(const NCOLOR * pcolalpha);
	void drawBetaAngles(const NLPATH_WAYPOINT *pwp0, const NCOLOR *pcolbeta);
	void drawPointsAndLines(const NCOLOR * pcolpoints, const NCOLOR *pcollines);
#endif
};
