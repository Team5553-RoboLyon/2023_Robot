/*
#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVecLimits.h"
#include "lib/N/NErrorHandling.h"
*/
#include "lib/NL/MotionControl/Trajectory/NLVstage.h"
#include "lib/NL/MotionControl/Path/NLPath.h"

#ifdef _NEDITOR
#endif

NLPATH::NLPATH()
{
	NIdentityMatrix(&m_matrix);
}
NLPATH::NLPATH(const Nu32 pathpoints_capacity, const Nu32 primitives_capacity) : m_geometry(pathpoints_capacity, primitives_capacity)
{
	NIdentityMatrix(&m_matrix);
}
NLPATH::~NLPATH()
{
	NIdentityMatrix(&m_matrix);
}

Nu32 NLPATH::write(FILE *pfile)
{
	// 1) �criture Version
	Nu32 version_u32 = VERSION_NLPATH_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) �criture Header
	NLPATH_HEADER header;
	header.m_matrix = m_matrix;

	if (fwrite(&header, sizeof(NLPATH_HEADER), 1, pfile) != 1)
		return 0;

	return m_geometry.write(pfile);
}

Nu32 NLPATH::read(FILE *pfile)
{
	// 1) lecture Version
	Nu32 version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLPATH_HEADER header;

	switch (NGETVERSION_MAIN(version_u32))
	{
	// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLPATH_HEADER):
		if (fread(&header, sizeof(NLPATH_HEADER), 1, pfile) != 1)
			return 0;
		m_matrix = header.m_matrix;

		if (!m_geometry.read(pfile))
			return 0;

		return 1;

	default:
		break;
	}
	return 0;
}

Nf32 NLPATH::calculateMaxCruiseVelocity(const NLKINLIMITS *pklim) const
{
	NErrorIf(!pklim->isConsistent(), NERROR_INCONSISTENT_VALUES);

	//	Si le chemin est jug� trop court, on ne calcule aucune vitesse de croisiere et on renvoie ZERO.
	if (m_geometry.m_ds < pklim->m_2ss)
		return 0.0f;
	else
	{
		Nf32 vc = NMIN(pklim->m_v, (-pklim->m_a2 + sqrt(pklim->m_a4 + pklim->m_4j2a * m_geometry.m_ds)) / pklim->m_2j);
		NErrorIf(vc < pklim->m_2vx, NERROR_INCONSISTENT_VALUES); // vu que m_geometry.m_ds >= pklim->m_2ss, alors la racine trouv�e
																 // ne saurait �tre hors limite et donc vc ne saurait l'�tre en cons�quence.
		return vc;
		//	Si la vitesse de croisiere est jug�e trop faible, on renvoie ZERO.
		// return (vc < pklim->m_2vx) ? 0.0f:vc;
	}
}

Nu32 NLPATH::buildVStages(NARRAY *pout_vstagedesc_array, const NLDRIVETRAINSPECS *pdtspecs, const NLKINLIMITS *pklim) const
{
	// Remarques sur les "VStageDescs":
	// Les VstageDescs issus d'un Path se succ�dent syst�matiquement selon un m�me et unique pattern.
	// En effet, si on nomme VSceil les vstage dont la courbure associ�e est 0 ( i.e vitesse associ�e max ) et VSFloor les vstage dont la courbure est non nulle alors nous voyons apparaitre
	// une alternance .... VSCeil, VSFloor, VSCeil, VSFloor, VSCeil, VSFloor, VSCeil ...
	// On comprend vite pourquoi, la m�thode utilisis�e pour g�n�rer un chemin commence syst�matiquement soit par un segment ( k=0 ) soit par une clothoide ( dont le k initial est 0 )
	//
	//	+------+				   +		 +------------+   +--+				  +
	//			.				  . .		.			   . .	  .				 .
	//			 .				 .	 .	   .				+	   .			.
	//			  .			    .	  .   .						    +----------+
	//			   +-----------+	   . .
	//									+
	NErrorIf(!pout_vstagedesc_array, NERROR_NULL_POINTER);
	NErrorIf(pout_vstagedesc_array->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	NLPATH_POINT *pkp0 = ((NLPATH_POINT *)m_geometry.m_pathPointsArray.pFirst);
	NLPATH_POINT *pkp1 = pkp0 + 1;
	NLPATH_PRIMITIVE *pprim = (NLPATH_PRIMITIVE *)m_geometry.m_primitivesArray.pFirst;
	Nf32 prev_k = NF32_MAX;

	NLVSTAGE vstage_desc;
	NLVSTAGE *pvstage_desc;

	Nu32 initial_array_size = pout_vstagedesc_array->Size;

	for (Nu32 i = 0; i < m_geometry.m_primitivesArray.Size; i++, pprim++, pkp0 = pkp1, pkp1++)
	{
		switch (pprim->m_core.m_id)
		{
		case NLPATH_PRIMITIVE_ID_SEGMENT:
			// Si la courbure pr�c�dente est diff�rente de celle du path point de sortie de la primitive ...
			if (pkp1->k != prev_k)
			{
				// ... alors on cr�e un nouveau VSTAGE
				prev_k = pkp1->k;
				pvstage_desc = (NLVSTAGE *)NArrayAllocBack(pout_vstagedesc_array);
				pvstage_desc->force(pkp0->s, pkp1->s, pdtspecs->getVelocity(pkp1->k, pklim->m_v));
			}
			// ... sinon, dans le cas ou la courbure r�f�renc�e dans le VSTAGE courant et dans le pathpoint de sortie de primitive sont �gaux ...
			else
			{
				pvstage_desc->m_s1 = pkp1->s;
				// ... Alors on "�tire le VSTAGE courant jusqu'au path point de sortie de primitive.
			}
			break;

		case NLPATH_PRIMITIVE_ID_ARC:
			// La gestion de la primitive Arc est exactement la m�me que pour la primitive segment.
			if (pkp1->k != prev_k)
			{
				prev_k = pkp1->k;
				pvstage_desc = (NLVSTAGE *)NArrayAllocBack(pout_vstagedesc_array);
				pvstage_desc->force(pkp0->s, pkp1->s, pdtspecs->getVelocity(pkp1->k, pklim->m_v));
			}
			else
			{
				pvstage_desc->m_s1 = pkp1->s;
			}
			break;

		case NLPATH_PRIMITIVE_ID_CLOTHOID:
			// La clothoide joue le role de transition de courbure entre deux VStages.
			// Le Vstage pr�c�dent est donc cens� avoir un K identique au K initial de la Clothoide. Si cela n'est pas le cas, cela revient � dire qu'il y a une rupture de continuit� de courbure.
			// Cela est possible si le path contient des cercles de constructions non remplac�s par des paires de clothoides (+potentiel arc ins�r�).
			//
			// 1)La clothoide est 1/2 et a donc un k0 = 0.
			//		Dans ce cas cela revient � dire que le Vstage pr�c�dent � un K different de 0 ... d'o� la rupture de continuit� !
			//		a) Cela est possible au tout d�but avec la premi�re clothoide. Le vstage precedent � un k valant NF32_MAX.
			//		( du coup cette premiere clothoide cr��e systematiquement un VStage de longueur nulle et de courbure nulle)
			//
			//		b)Cela est possible dans le cas de l'enchainement initial de deux arcs de cercle de construction avec le second remplac� par 2 clothoides (+ potentiel cercle ins�r�).
			//		Dans ce cas, le premier cercle de construction g�n�re un premier VStage et la premi�re clothoide g�n�re un VStage de longeur et courbure nulle situ� � la m�me abscisse
			//		que la sortie du VStage pr�c�dent ( celui g�n�r� par le premier cercle de construction ).
			//
			// 2)La clothoide est 2/2 et a donc un k0 > 0. On voit vite que ce cas est impossible. Si pkp0->k != pvs->m_k, la clothoide est forcement 1/2 !
			//
			if (pkp0->k != prev_k)
			{
				NErrorIf(ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND), NERROR_SYSTEM_GURU_MEDITATION);
				NErrorIf(pkp0->k != 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				pvstage_desc = (NLVSTAGE *)NArrayAllocBack(pout_vstagedesc_array);
				pvstage_desc->force(pkp0->s, pkp0->s, pdtspecs->getVelocity(pkp0->k, pklim->m_v));
			}
			// ... ensuite la clothoide g�n�re un Vstage de "sortie" cens� �tre utilis� par la primitive suivante si celle ci est bien dans la continuit� de la courbure de la clothoide.
			prev_k = pkp1->k;
			pvstage_desc = (NLVSTAGE *)NArrayAllocBack(pout_vstagedesc_array);
			pvstage_desc->force(pkp1->s, pkp1->s, pdtspecs->getVelocity(pkp1->k, pklim->m_v));
			break;

		default:
			NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
			break;
		}
	}
	return (pout_vstagedesc_array->Size - initial_array_size);
}

NVEC2f32 *NLPATH::convertGlobalPositionToLocal(NVEC2f32 *pres, const NVEC2f32 *pglobal_position) const
{
	NVEC2f32 p, r;

	p.x = pglobal_position->x - m_matrix.Origin.x;
	p.y = pglobal_position->y - m_matrix.Origin.y;

	r.x = p.x * m_matrix.XAxis.x + p.y * m_matrix.XAxis.y; // �quivalant �:  NVec2DotProduct(&p, (NVEC2f32*)&m_matrix.XAxis);
	r.y = p.x * m_matrix.YAxis.x + p.y * m_matrix.YAxis.y; // �quivalant �:  NVec2DotProduct(&p, (NVEC2f32*)&m_matrix.YAxis);

	*pres = r;
	return pres;
}

NVEC2f32 *NLPATH::convertGlobalDirectionToLocal(NVEC2f32 *pres, const NVEC2f32 *pglobal_direction) const
{
	NVEC2f32 r;

	r.x = pglobal_direction->x * m_matrix.XAxis.x + pglobal_direction->y * m_matrix.XAxis.y; // �quivalant �:  NVec2DotProduct(pglobal_direction, (NVEC2f32*)&m_matrix.XAxis);
	r.y = pglobal_direction->x * m_matrix.YAxis.x + pglobal_direction->y * m_matrix.YAxis.y; // �quivalant �:  NVec2DotProduct(pglobal_direction, (NVEC2f32*)&m_matrix.YAxis);

	*pres = r;
	return pres;
}

NVEC2f32 *NLPATH::convertLocalPositionToGlobal(NVEC2f32 *pres, const NVEC2f32 *plocal_position) const
{
	NVEC3f32 v;
	NMulVector2ByMatrixO(&v, &m_matrix, plocal_position);
	pres->x = v.x;
	pres->y = v.y;

	return pres;
}

NVEC2f32 *NLPATH::convertLocalDirectionToGlobal(NVEC2f32 *pres, const NVEC2f32 *plocal_direction) const
{
	NVEC3f32 v;
	NMulVector2ByMatrix3O(&v, &m_matrix, plocal_direction);
	pres->x = v.x;
	pres->y = v.y;

	return pres;
}

/*
LEGACY TO BE DELETED !!
void NLPATH::segmentPrimitive(const NVEC2f32 *pA, const NVEC2f32 *pB)
{
	NVEC2f32	 v;
	Nf32		lv;
	v.x = pB->x - pA->x;
	v.y = pB->y - pA->y;
	lv = NVec2Length(&v);

	// extraction primitive
	NLPATH_PRIMITIVE *pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
	pprim->m_segment.m_core.m_id = NLPATH_PRIMITIVE_ID_SEGMENT;
	pprim->m_segment.m_length = lv;

	// extraction keypoint
	NLPATH_POINT *pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
	pkey->s = (pkey - 1)->s + lv;
	pkey->p = *pB;
	pkey->k = 0.0f;
}
*/
/*
// ------------------------------------------------------------------------------------------
// NLPATH::arcPrimitives
// ------------------------------------------------------------------------------------------
// In  :
//		pB			ptr sur le point d'entr�e sur l'arc de cercle 'source'
//		pBtgt		ptr sur le vecteur tangent � l'arc de cercle en B.
//		pD			ptr sur le point de sortie sur l'arc de cercle 'source'
//		pDtgt		ptr sur le vecteur tangent � l'arc de cercle en D.
//		csgn		"Circle Sign" signe du cercle source ( 1 ou -1 selon son sens de parcours )
//		icratio		"Inserted Circle ratio"	d�fini la pr�sence ( ou pas) d'un arc de cercle ins�r� entre les deux clothoides.
// Out :
//		void
// ------------------------------------------------------------------------------------------
void NLPATH::arcPrimitives(const NVEC2f32 *pB, const NVEC2f32 *pBtgt, const NVEC2f32 *pD, const NVEC2f32 *pDtgt, const Nf32 csgn, const Nf32 icratio )
{
//	NErrorIf(!NVec2IsUnit(pBtgt), NERROR_WRONG_VALUE);
//	NErrorIf(!NVec2IsUnit(pDtgt), NERROR_WRONG_VALUE);

	// Primitive#1: ROOT.PRIMITIVE: Arc de cercle de waypoint0 au point de sortie sur le premier cercle.
	//					|
	//					+----> Primitive#1.1: Clothoide
	//					|
	//					+----> Primitive#1.2: Arc de cercle "ins�r�"
	//					|
	//					+----> Primitive#1.3: Clothoide
	//
	// Calculs des param�tres de clothoides
	NUT_2DLINE_XRESULT	xres;
	Nf32					fa;
	NVEC2f32				f0;
	Nf32					lp0f0;					// longueur du vecteur [p0,f0]
	Nf32					alpha, sigma, theta;
	Nf32					km;						// courbure au point d'abscisse curviligne maximum.
	Nf32					rac1, rac2;				// racines interm�diaires utilis�es dans le calcul du ryon du cercle (arc de cercle) ins�r� entre les deux clothoides.
	Nf32					costheta, sintheta;		// respectivement cosinus et sinus de l'angle theta.
	Nf32					icr;					// Inserted Cercle Radius ( rayon du cercle, ou plut�t arc de cercle, ins�r� entre les deux clothoides )
	NVEC2f32				icc;					// Inserted Cercle Center ( centre du cercle, ou plut�t arc de cercle, ins�r� entre les deux clothoides )
	Nf32					sm;						// Abscisse curviligne su sommet de la clothoide ( = abscisse curviligne max )
	Nf32					param;					//
	Nf32					cf, sf;					// R�sultats des calculs des int�grales de Fresnel.
	Nf32					x, y;					// variables de calculs interm�diaires

	NLPATH_PRIMITIVE*		pprim;
	NLPATH_POINT*		pkey;

	NUT_2DLineXLine_VDir(pB,pBtgt,pD,pDtgt, &xres);
	f0 = xres.I;
	lp0f0 = NABS(xres.ParamCoordAB); // comme 'u' est unitaire, 'xres.ParamCoordAB' represente ici la longueur alg�brique du vecteur [p0,f0].
	alpha = acosf( NVec2DotProduct(pBtgt,pDtgt) );

	// Avec un arc de cercle ins�r� entre les deux clothoides symetriques � sommet.
	if (icratio != 0.0f)
	{
		theta	= ((NF32_PI - alpha) / 2.0f) *icratio; // Ref [2.2.5] page 63 AVEC " 0.0f < icratio < 1.0f " permettant de faire evoluer tetha0 dans l'intervalle ]0, (PI-alpha0)/2 [
		rac1	= sqrt(2.0f*theta*NF32_PI);
		rac2	= sqrt((2.0f*theta) / NF32_PI);
		costheta= cosf(theta);
		sintheta= sinf(theta);
		NFresnelIntegralsf64(rac2, &cf, &sf);

		icr		= lp0f0 / (rac1*cf - sintheta + (rac1*sf + costheta) / tanf(alpha / 2.0f));
		icc.x	= icr * (rac1 * cf - sintheta);
		icc.y	= icr * (rac1 * sf + costheta);
		sigma	= 1.0f / (2.0f*theta*icr*icr);
		sm		= 2.0f*theta*icr;
		km		= 1.0f / icr;	// ou ... km = sm * sigma; qui normalement doit donner le m�me r�sultat � epsilon pr�s...
		param	= sqrt(NF32_PI / sigma);

		// CLOTHOIDE #1
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km		= km;
		pprim->m_clothoid.m_sm		= sm;
		pprim->m_clothoid.m_param	= param;
		pprim->m_clothoid.m_i		= *pBtgt;
		pprim->m_clothoid.m_j.x		= -pBtgt->y * csgn;
		pprim->m_clothoid.m_j.y		=  pBtgt->x * csgn;
		// extraction keypoint ( de fin de clothoide )
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
		pkey->s		= (pkey - 1)->s + sm;
		NFresnelIntegralsf64(sm / param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x = pB->x + pprim->m_clothoid.m_i.x*x + pprim->m_clothoid.m_j.x*y;
		pkey->p.y = pB->y + pprim->m_clothoid.m_i.y*x + pprim->m_clothoid.m_j.y*y;
		pkey->k = km * csgn;

		// ARC DE CERCLE INSERE
		// ARC:extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_ARC;
		pprim->m_arc.m_phi			= theta - NF32_PI_2;
		pprim->m_arc.m_omega		= NF32_PI - alpha - 2.0f*theta;
		pprim->m_arc.m_i			= *pBtgt;
		pprim->m_arc.m_j.x			= -pBtgt->y * csgn;
		pprim->m_arc.m_j.y			=  pBtgt->x * csgn;

		//pprim->m_arc.m_center		= icc;
		pprim->m_arc.m_center.x = pB->x + pprim->m_arc.m_i.x*icc.x + pprim->m_arc.m_j.x*icc.y;
		pprim->m_arc.m_center.y = pB->y + pprim->m_arc.m_i.y*icc.x + pprim->m_arc.m_j.y*icc.y;


		pprim->m_arc.m_radius		= icr;

		// ARC:extraction keypoint  ( de fin d'arcc de cercle )... part I
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
		pkey->s = (pkey - 1)->s + pprim->m_arc.m_omega*icr;
		pkey->k = km * csgn;
		// CLOTHOIDE #2:extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km = km;
		pprim->m_clothoid.m_sm = -sm;
		pprim->m_clothoid.m_param = param;
		pprim->m_clothoid.m_i	= *pDtgt;
		pprim->m_clothoid.m_j.x = pDtgt->y * csgn;
		pprim->m_clothoid.m_j.y = -pDtgt->x * csgn;

		// ARC:extraction keypoint ( de fin d'arc de cercle ) ... part II ( utilise les parametres et la base de la clothoide )
		NFresnelIntegralsf64(sm / param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x = pD->x + pprim->m_clothoid.m_i.x*x + pprim->m_clothoid.m_j.x*y;
		pkey->p.y = pD->y + pprim->m_clothoid.m_i.y*x + pprim->m_clothoid.m_j.y*y;

		// CLOTHOIDE #2: extraction keypoint
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
		pkey->s = (pkey - 1)->s + sm;
		pkey->p = *pD;
		pkey->k = 0.0f;
	}
	else
	// Sans cercle ins�r� entre les deux clothoides symetriques � sommet.
	{
		NFresnelIntegralsf64(sqrt(1.0f - alpha / NF32_PI), &cf, &sf);				// |
		fa = cf + sf / (tanf(alpha / 2.0f));								// |-- Ref. [2.4] page 61
		sigma = (NF32_PI / (lp0f0*lp0f0)) * fa*fa;							// |
		sm = sqrt((NF32_PI - alpha) / sigma);
		km = (sqrt(NF32_PI * (NF32_PI - alpha)) / lp0f0) * fa;				// |-- Ref. page 62
		param = sqrt(NF32_PI / sigma);

		// CLOTHOIDE #1
		// extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_NXTLINK);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km		= km;
		pprim->m_clothoid.m_sm		= sm;
		pprim->m_clothoid.m_param	= param;
		pprim->m_clothoid.m_i		= *pBtgt;
		pprim->m_clothoid.m_j.x		= -pBtgt->y * csgn;
		pprim->m_clothoid.m_j.y		=  pBtgt->x * csgn;
		// extraction keypoint
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
		pkey->s = (pkey - 1)->s + sm;
		pkey->k = km * csgn;
		NFresnelIntegralsf64(sm/param, &cf, &sf);
		x = param * cf;
		y = param * sf;
		pkey->p.x = pB->x + pprim->m_clothoid.m_i.x*x + pprim->m_clothoid.m_j.x*y;
		pkey->p.y = pB->y + pprim->m_clothoid.m_i.y*x + pprim->m_clothoid.m_j.y*y;

		// CLOTHOIDE #2
		// extraction primitive
		pprim = (NLPATH_PRIMITIVE*)NArrayAllocBack0(&m_primitivesArray);
		FLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_PRVLINK);
		pprim->m_clothoid.m_core.m_id = NLPATH_PRIMITIVE_ID_CLOTHOID;
		pprim->m_clothoid.m_km		= km;
		pprim->m_clothoid.m_sm		= -sm;
		pprim->m_clothoid.m_param	= param;
		pprim->m_clothoid.m_i		= *pDtgt;
		pprim->m_clothoid.m_j.x		=  pDtgt->y * csgn;
		pprim->m_clothoid.m_j.y		= -pDtgt->x * csgn;
		// extraction keypoint
		pkey = (NLPATH_POINT*)NArrayAllocBack0(&m_pathPointsArray);
		pkey->s = (pkey - 1)->s + sm;
		pkey->p = *pD;
		pkey->k = 0.0f;
	}
}
*/

/*
void NLPATH::build()
{
	// Rien � faire // Nothing to do !
//	if (ISFLAG_OFF(m_flags, FLAG_NLPATH_WAYPOINTS_UPDATED) || (m_wayPointsArray.Size < 2) )
//		return;

	// 	Clear et (re)Construction des sections � partir des Waypoints
	NEraseArray(&m_pathPointsArray,	NULL);
	NEraseArray(&m_sectionsArray,	NULL);
	NEraseArray(&m_primitivesArray, NULL);

	Nu32			i;
	NLPATH_WAYPOINT	*pwp0 = (NLPATH_WAYPOINT*)m_wayPointsArray.pFirst;
	NLPATH_WAYPOINT	*pwp1 = pwp0+1;
	NLPATH_SECTION	*psection;
	NLPATH_SECTION_BPAIR	*ppair;
	NVEC2f32		 v, vp, C,es,mu;

	// insertion du premier keyPoint ( les suivants seront ins�r�s 1 par 1 par les methodes d'extraction de  primitives )
	NLPATH_POINT *pkp = (NLPATH_POINT*)NArrayAllocBack(&m_pathPointsArray);
	pkp->p = pwp0->p;		// position dans le plan XY
	pkp->s = 0.0f;			// abscisse curviligne nulle ( ... c'est le point de depart )
	pkp->k = 0.0f ;			// courbure � ce point.  TODO: ajouter la possibilit� d'avoir une courbure non-nulle c'est � dire d�part non-arr�t� et que la premiere primitive est un arc de cercle !!!

	for (i = 1; i < m_wayPointsArray.Size; i++, pwp0 = pwp1, pwp1++)
	{
		psection = (NLPATH_SECTION*)NArrayAllocBack(&m_sectionsArray);
		psection->setup(pwp0, pwp1);

		// EXTRACTION DES PRIMITIVES
		// -------------------------
		if (ISFLAG_ON(psection->flags, FLAG_NLPATH_SECTION_IS_ALIGNED))
		{
			segmentPrimitive(&psection->pwayPoint_from->p, &psection->pwayPoint_to->p);
		}
		else
		{
			ppair = (NLPATH_SECTION_BPAIR*)&psection->pair[m_heuristic(psection, m_pdrivetrainspecs)];

			es.x = -ppair->se.x;
			es.y = -ppair->se.y;


			// Remplacement du premier arc de cercle:
			if (ppair->beta0 >= NF32_PI)
			{
				// l'arc de cercle est trop "ouvert". Il doit �tre scind� en deux.
				NUT_2DLINE_XRESULT	xres;
				NUT_2DLineXLine_VDir(&psection->pwayPoint_from->p, &psection->pwayPoint_from->u, &ppair->s, &ppair->se, &xres);
				v.x = (ppair->pcircle0->o.x - xres.I.x);
				v.y = (ppair->pcircle0->o.y - xres.I.y);
				NVec2Normalize(&v);
				vp.x =  v.y*ppair->pcircle0->sgn;
				vp.y = -v.x*ppair->pcircle0->sgn;
				C.x = ppair->pcircle0->o.x + v.x*ppair->pcircle0->r;
				C.y = ppair->pcircle0->o.y + v.y*ppair->pcircle0->r;
				arcPrimitives(&psection->pwayPoint_from->p, &psection->pwayPoint_from->u,&C ,&vp ,(Nf32)ppair->pcircle0->sgn, psection->pwayPoint_from->icr_A);
				vp.x = -vp.x;
				vp.y = -vp.y;
				arcPrimitives(&C, &vp, &ppair->s, &es, (Nf32)ppair->pcircle0->sgn, psection->pwayPoint_from->icr_A);
			}
			else
			{
				arcPrimitives(&psection->pwayPoint_from->p, &psection->pwayPoint_from->u, &ppair->s, &es, (Nf32)ppair->pcircle0->sgn, psection->pwayPoint_from->icr_A);
			}

			// Remplacement du segment tangent
			if(ppair->lse != 0.0f)
				segmentPrimitive(&ppair->s,&ppair->e);

			// Remplacement du second arc de cercle:
			if (ppair->beta1 >= NF32_PI)
			{
				// l'arc de cercle est trop "ouvert". Il doit �tre scind� en deux.
				NUT_2DLINE_XRESULT	xres;
				NUT_2DLineXLine_VDir(&psection->pwayPoint_to->p, &psection->pwayPoint_to->u, &ppair->e, &es, &xres);
				v.x = (ppair->pcircle1->o.x - xres.I.x);
				v.y = (ppair->pcircle1->o.y - xres.I.y);
				NVec2Normalize(&v);
				vp.x =  v.y*ppair->pcircle1->sgn;
				vp.y = -v.x*ppair->pcircle1->sgn;
				C.x  = ppair->pcircle1->o.x + v.x*ppair->pcircle1->r;
				C.y  = ppair->pcircle1->o.y + v.y*ppair->pcircle1->r;
				arcPrimitives(&ppair->e,&ppair->se, &C, &vp, (Nf32)ppair->pcircle1->sgn, psection->pwayPoint_to->icr_C);
				vp.x = -vp.x;
				vp.y = -vp.y;
				mu.x = -psection->pwayPoint_to->u.x;
				mu.y = -psection->pwayPoint_to->u.y;
				arcPrimitives(&C, &vp, &psection->pwayPoint_to->p, &mu, (Nf32)ppair->pcircle1->sgn, psection->pwayPoint_to->icr_C);
			}
			else
			{
				mu.x = -psection->pwayPoint_to->u.x;
				mu.y = -psection->pwayPoint_to->u.y;
				arcPrimitives(&ppair->e, &ppair->se, &psection->pwayPoint_to->p, &mu,(Nf32)ppair->pcircle1->sgn, psection->pwayPoint_to->icr_C);
			}

		}

	}
}
*/
