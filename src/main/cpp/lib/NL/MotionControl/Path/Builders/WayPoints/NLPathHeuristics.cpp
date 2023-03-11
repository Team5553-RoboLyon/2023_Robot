#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

#include "lib/NL/MotionControl/Path/Builders/Waypoints/NLPathSection.h"


Nu32 NLPATH_heuristic_shortest(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/)
{
	Nu32 j;
	Nu32 choosen = NU32_MAX;
	Nf32 distmin = NF32_MAX;
	Nf32 dist;

	// la plus courte
	for (j = 0; j < 4; j++)
	{
		dist = psection->m_pair[j].lse + NABS(psection->m_pair[j].beta0)*psection->m_pair[j].pcircle0->r + NABS(psection->m_pair[j].beta1*psection->m_pair[j].pcircle1->r);
		if (dist < distmin)
		{
			distmin = dist;
			choosen = j;
		}
	}
	return choosen;
}

Nu32 NLPATH_heuristic_lessCurvy(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/)
{
	Nu32 j;
	Nu32 choosen = NU32_MAX;
	Nf32 angmin	 = NF32_MAX;
	Nf32 ang;

	// la moins sinueuse
	for (j = 0; j < 4; j++)
	{
		ang = NABS(psection->m_pair[j].beta0) + NABS(psection->m_pair[j].beta1);
		if (ang < angmin)
		{
			angmin = ang;
			choosen = j;
		}
	}

	return choosen;
}


Nu32 NLPATH_heuristic_quickest(const NLPATH_SECTION *psection/*, const NLDRIVETRAINSPECS *pdts*/)
{
	// Pas sur que cela soit une bonne id�e...
	// la fonction est remplac�e par NLPATH_heuristic_shortest si pdts est null
	//if (!pdts)
		return NLPATH_heuristic_shortest(psection/*, 0*/);
/*
	Nu32 j;
	Nu32 choosen = NU32_MAX;
	Nf32 tmin = NF32_MAX;
	Nf32 t;
	Nf32 v0, v1;

	// Celle qui prendra le moins de temps ( pour le robot... n�c�ssite des infos sur le robot, ici on utilise juste l'entraxe,  la position du CG serait bienvenue )
	for (j = 0; j < 4; j++)
	{
		v0 = 0.5f*(1.0f + (psection->m_pair[j].pcircle0->r - pdts->m_axleTrack) / (psection->m_pair[j].pcircle0->r + pdts->m_axleTrack));
		v1 = 0.5f*(1.0f + (psection->m_pair[j].pcircle1->r - pdts->m_axleTrack) / (psection->m_pair[j].pcircle1->r + pdts->m_axleTrack));

		t = (NABS(psection->m_pair[j].beta0)*psection->m_pair[j].pcircle0->r) / v0 + (NABS(psection->m_pair[j].beta1)*psection->m_pair[j].pcircle1->r) / v1 + psection->m_pair[j].lse;
		if (t < tmin)
		{
			tmin = t;
			choosen = j;
		}
	}
	return choosen;
*/
}
