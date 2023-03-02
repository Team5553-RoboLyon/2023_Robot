#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Miscellaneous/NColor.h"

#include "../Geometry/NLPathGeometry.h"

// Arc de cercle de construction ( "Build Arc" )
typedef struct NLPATH_BARC NLPATH_BARC;
struct NLPATH_BARC
{
	NVEC2f32 m_B;	 // Point 'B' de début de l'arc
	NVEC2f32 m_BTgt; // Tangente à l'arc en 'B'
	NVEC2f32 m_D;	 // Point 'D' de fin de l'arc
	NVEC2f32 m_DTgt; // Tangente à l'arc en 'D'
	Nf32 m_Sgn;		 // Signe ou sens de parcour de l'arc
	Nf32 m_Icr;		 // Coefficient d'insertion d'un arc de cercle entre les deux clothoides à sommet issues de l'arc de cercle de construction.
					 // Icr =  0 signifie pas de clothoide, l'arc de  cercle inséré EST l'arc de cercle de construction.
					 // Icr =  1 signifie paire de clothoides de longueur maximale et PAS d'ARC de CERCLE inséré entre les deux.
	//
	// -----------------------------		Les valeurs ci-dessous sont utilisées uniquement Dans le cas où Icr = 0. ( la primitive arc de cercle qui sera créée étant donc identique à l'arc de cercle de construction )
	//
	Nf32 m_BBeta;		// Longueur angulaire de l'arc de cercle de construction
	Nf32 m_BRadius;		// Rayon du cercle de construction
	NVEC2f32 m_BCenter; // Centre du cercle de construction

	Nu32 extractPrimitives(NLPATH_GEOMETRY *pgeom);
#ifdef _NEDITOR
	void draw(const NCOLOR *pcolor);
#endif
};
