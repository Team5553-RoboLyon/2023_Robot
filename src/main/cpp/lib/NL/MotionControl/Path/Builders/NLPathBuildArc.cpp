#include "../../../../N/Utilities/NUT_X.h"

#include "NLPathBuildArc.h"

// Refs:
// D:\_PROJETS\FIRST\_Docs\_State-Of-the-Art\_PathBuilding\1_these-clothoide(page 49 et suivantes).pdf

Nu32 NLPATH_BARC::extractPrimitives(NLPATH_GEOMETRY *pgeom)
{
	NErrorIf(pgeom->m_pathPointsArray.ElementSize != sizeof(NLPATH_POINT), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(pgeom->m_primitivesArray.ElementSize != sizeof(NLPATH_PRIMITIVE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

		// Primitive#1: ROOT.PRIMITIVE: Arc de cercle de waypoint0 au point de sortie sur le premier cercle.
		//					|
		//					+----> Primitive#1.1: Clothoide
		//					|
		//					+----> Primitive#1.2: Arc de cercle "inséré"
		//					|
		//					+----> Primitive#1.3: Clothoide
		//
		// Calculs des paramètres de clothoides
	NUT_2DLINE_XRESULT		xres;
	Nf32					fa;
	NVEC2f32				f0;
	Nf32					lp0f0;					// longueur du vecteur [p0,f0]
	Nf32					alpha, sigma, theta;
	Nf32					km;						// courbure au point d'abscisse curviligne maximum.
	Nf32					rac1, rac2;				// racines intermédiaires utilisées dans le calcul du ryon du cercle (arc de cercle) inséré entre les deux clothoides.
	Nf32					costheta, sintheta;		// respectivement cosinus et sinus de l'angle theta.
	Nf32					icr;					// Inserted Cercle Radius ( rayon du cercle, ou plutôt arc de cercle, inséré entre les deux clothoides )	
	NVEC2f32				icc;					// Inserted Cercle Center ( centre du cercle, ou plutôt arc de cercle, inséré entre les deux clothoides )
	Nf32					sm;						// Abscisse curviligne su sommet de la clothoide ( = abscisse curviligne max )
	Nf32					param;					//
	Nf32					cf, sf;					// Résultats des calculs des intégrales de Fresnel.
	Nf32					x, y;					// variables de calculs intermédiaires

	NLPATH_PRIMITIVE* pprim;
	NLPATH_POINT* pkey;

	NUT_2DLineXLine_VDir(&m_B, &m_BTgt, &m_D, &m_DTgt, &xres);
	f0 = xres.I;
	lp0f0 = NABS(xres.ParamCoordAB); // comme 'u' est unitaire, 'xres.ParamCoordAB' represente ici la longueur algébrique du vecteur [p0,f0].
	alpha = acosf(NVec2DotProduct(&m_BTgt, &m_DTgt));

	if (m_Icr <= 0.0f)
	{
		// ARC DE CERCLE SEUL SANS CLOTHOIDES !!!
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW);
		if (m_Sgn == -1.0f)
			FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT);

		pprim->m_arc.m_core.m_id	= NLPATH_PRIMITIVE_ID_ARC;
		pprim->m_arc.m_core.m_l		= m_BBeta * m_BRadius;
		pprim->m_arc.m_phi			= -NF32_PI_2;			// theta - NF32_PI_2;				|_ Dans ce cas specifique on a "theta = 0.0f" donc ...
		pprim->m_arc.m_omega		= m_BBeta;		// NF32_PI - alpha -2.0f*theta;		|
		pprim->m_arc.m_i			= m_BTgt;
		pprim->m_arc.m_j.x			= -m_BTgt.y * m_Sgn;
		pprim->m_arc.m_j.y			= m_BTgt.x * m_Sgn;

		//pprim->m_arc.m_center.x = pbarc->B.x + pprim->m_arc.m_i.x*icc.x + pprim->m_arc.m_j.x*icc.y;
		//pprim->m_arc.m_center.y = pbarc->B.y + pprim->m_arc.m_i.y*icc.x + pprim->m_arc.m_j.y*icc.y;
		//pprim->m_arc.m_radius = icr;
		pprim->m_arc.m_center = m_BCenter;
		pprim->m_arc.m_radius = m_BRadius;

		// ARC:extraction keypoint  ( de fin d'arc de cercle )... 
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		NErrorIf(!NIsValidArrayElementPtr(&pgeom->m_pathPointsArray, (NBYTE*)(NLPATH_POINT*)(pkey - 1)), NERROR_ARRAY_PTR_BEYOND_LIMITS);

		pkey->s = (pkey - 1)->s + pprim->m_arc.m_omega * pprim->m_arc.m_radius;
		pkey->k = m_Sgn / pprim->m_arc.m_radius;
		pkey->p = m_D;
		pkey->u.x = -m_DTgt.x;
		pkey->u.y = -m_DTgt.y;
		return 1;
	}
	else if (m_Icr >= 1.0f)
	{
		// Sans cercle inséré entre les deux clothoides symetriques à sommet.
		// Les deux clothoides forment alors une paire liée et sont flaggées en conséquence.
		NFresnelIntegralsf32(sqrt(1.0f - alpha / NF32_PI), &cf, &sf);		// |	
		fa = cf + sf / (tanf(alpha / 2.0f));								// |-- Ref. [2.4] page 61
		sigma = (NF32_PI / (lp0f0 * lp0f0)) * fa * fa;						// |

		sm = sqrt((NF32_PI - alpha) / sigma);								// |-- Ref. page 62

		km = (sqrt(NF32_PI * (NF32_PI - alpha)) / lp0f0) * fa;				// |-- Ref. page 62
		param = sqrt(NF32_PI / sigma);										// |-- Ref. [1.6] page 42
		theta = ((NF32_PI - alpha) / 2.0f);									// |-- Ref. page 61

		costheta = cosf(theta);
		sintheta = sinf(theta);



		// CLOTHOIDE #1
		// extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_NXTLINK);
		pprim->m_clothoid.m_core.m_id	= NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km			= km;
		pprim->m_clothoid.m_core.m_l	= sm;
		pprim->m_clothoid.m_sigma		= sigma;
		pprim->m_clothoid.m_param		= param;
		pprim->m_clothoid.m_i			= m_BTgt;
		pprim->m_clothoid.m_j.x			= -m_BTgt.y * m_Sgn;
		pprim->m_clothoid.m_j.y			= m_BTgt.x * m_Sgn;
		// extraction keypoint
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		pkey->s = (pkey - 1)->s + sm;
		pkey->k = km * m_Sgn;
		NFresnelIntegralsf32(sm / param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x = m_B.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y;
		pkey->p.y = m_B.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y;
		pkey->u.x = pprim->m_clothoid.m_i.x * costheta + pprim->m_clothoid.m_j.x * sintheta;
		pkey->u.y = pprim->m_clothoid.m_i.y * costheta + pprim->m_clothoid.m_j.y * sintheta;

		// CLOTHOIDE #2
		// extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_PRVLINK | FLAG_NLPATH_CLOTHOID_SECOND);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km			= km;
		pprim->m_clothoid.m_core.m_l	= sm;
		pprim->m_clothoid.m_sigma		= sigma;
		pprim->m_clothoid.m_param		= param;
		pprim->m_clothoid.m_i			= m_DTgt;
		pprim->m_clothoid.m_j.x			= m_DTgt.y * m_Sgn;
		pprim->m_clothoid.m_j.y			= -m_DTgt.x * m_Sgn;

		// extraction keypoint
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		pkey->s		= (pkey - 1)->s + sm;
		pkey->p		= m_D;
		pkey->k		= 0.0f;
		pkey->u.x	= -m_DTgt.x;
		pkey->u.y	= -m_DTgt.y;
		return 2;
	}
	else
	{
		// Avec un arc de cercle inséré entre les deux clothoides symetriques à sommet.
		theta = ((NF32_PI - alpha) / 2.0f) * m_Icr;	// |-- Ref. [2.2.5] page 63 AVEC " 0.0f < icratio < 1.0f " permettant de faire evoluer tetha0 dans l'intervalle ]0, (PI-alpha0)/2 [

		rac1		= sqrt(2.0f * theta * NF32_PI);													// |
		rac2		= sqrt((2.0f * theta) / NF32_PI);												// |
		costheta	= cosf(theta);																	// |__ Ref. [2.9] page 64
		sintheta	= sinf(theta);																	// |
		NFresnelIntegralsf32(rac2, &cf, &sf);														// |
		icr = lp0f0 / (rac1 * cf - sintheta + (rac1 * sf + costheta) / tanf(alpha / 2.0f));			// |

		icc.x		= icr * (rac1 * cf - sintheta);													// |__ Ref. [2.7] page 64
		icc.y		= icr * (rac1 * sf + costheta);													// |

		sigma		= 1.0f / (2.0f * theta * icr * icr);											// |__ Ref. page 65
		sm			= 2.0f * theta * icr;															// |

		km			= 1.0f / icr;																	// |-- Ref. [1.3] page 39 --> km =  sm * sigma;
																									// |						  km = 	2*theta*icr  *  1 / (2*theta*icr*icr)
																									// |						  km =  1/icr

		param = sqrt(NF32_PI / sigma);																// |-- Ref. [1.6] page 42															

	//	Nf32 tmp_reverseback_theta = sigma * sm*sm / 2.0f;											// |-- Ref. page 61	

		// CLOTHOIDE #1
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		pprim->m_clothoid.m_core.m_id	= NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km			= km;
		pprim->m_clothoid.m_core.m_l	= sm;
		pprim->m_clothoid.m_sigma		= sigma;
		pprim->m_clothoid.m_param		= param;
		pprim->m_clothoid.m_i			= m_BTgt;
		pprim->m_clothoid.m_j.x			= -m_BTgt.y * m_Sgn;
		pprim->m_clothoid.m_j.y			= m_BTgt.x * m_Sgn;
		// extraction keypoint ( de fin de clothoide )
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		pkey->s		= (pkey - 1)->s + sm;
		NFresnelIntegralsf32(sm / param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x	= m_B.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y;
		pkey->p.y	= m_B.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y;
		pkey->k		= km * m_Sgn;
		pkey->u.x	= pprim->m_clothoid.m_i.x * costheta + pprim->m_clothoid.m_j.x * sintheta;
		pkey->u.y	= pprim->m_clothoid.m_i.y * costheta + pprim->m_clothoid.m_j.y * sintheta;

		// ARC DE CERCLE INSERE
		// ARC:extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		pprim->m_arc.m_core.m_id = NLPATH_PRIMITIVE_ID_ARC;
		if (m_Sgn == -1.0f)
			FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT);
		//else
		//	FLAG_OFF(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT);
		pprim->m_arc.m_phi		= theta - NF32_PI_2;
		pprim->m_arc.m_omega	= NF32_PI - alpha - 2.0f * theta;
		pprim->m_arc.m_i		= m_BTgt;
		pprim->m_arc.m_j.x		= -m_BTgt.y * m_Sgn;
		pprim->m_arc.m_j.y		= m_BTgt.x * m_Sgn;
		//pprim->m_arc.m_center		= icc;
		pprim->m_arc.m_center.x = m_B.x + pprim->m_arc.m_i.x * icc.x + pprim->m_arc.m_j.x * icc.y;
		pprim->m_arc.m_center.y = m_B.y + pprim->m_arc.m_i.y * icc.x + pprim->m_arc.m_j.y * icc.y;
		pprim->m_arc.m_radius	= icr;
		pprim->m_arc.m_core.m_l = icr * pprim->m_arc.m_omega;

		// ARC:extraction keypoint  ( de fin d'arcc de cercle )... part I
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		pkey->s					= (pkey - 1)->s + pprim->m_arc.m_omega * icr;
		pkey->k					= km * m_Sgn;

		// CLOTHOIDE #2:extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&pgeom->m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND);
		pprim->m_clothoid.m_core.m_id	= NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km			= km;
		pprim->m_clothoid.m_core.m_l	= sm;
		pprim->m_clothoid.m_sigma		= sigma;
		pprim->m_clothoid.m_param		= param;
		pprim->m_clothoid.m_i			= m_DTgt;
		pprim->m_clothoid.m_j.x			= m_DTgt.y * m_Sgn;
		pprim->m_clothoid.m_j.y			= -m_DTgt.x * m_Sgn;

		// ARC:extraction keypoint ( de fin d'arc de cercle ) ... part II ( utilise les parametres et la base de la clothoide ) 
		NFresnelIntegralsf32(sm / param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x = m_D.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y;
		pkey->p.y = m_D.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y;
		pkey->u.x = -m_DTgt.x * costheta - m_DTgt.y * sintheta * m_Sgn;
		pkey->u.y = -m_DTgt.y * costheta + m_DTgt.x * sintheta * m_Sgn;

		// CLOTHOIDE #2: extraction keypoint
		pprim->m_core.m_keyPoint0 = _SafeNu32ToNu16(pgeom->m_pathPointsArray.Size - 1); // '-1' car on veut l'index du keypoint précédent ( déjà inséré !) 
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&pgeom->m_pathPointsArray);
		pkey->s = (pkey - 1)->s + sm;
		pkey->p = m_D;
		pkey->k = 0.0f;
		pkey->u.x = -m_DTgt.x;
		pkey->u.y = -m_DTgt.y;
		return 3;
	}
}
#ifdef _NEDITOR
void NLPATH_BARC::draw(const NCOLOR * pcolor)
{

}
#endif
