#include "lib/N/NFlags.h"
#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVecLimits.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Utilities/NUT_X.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"

#include "lib/NL/MotionControl/Path/Geometry/NLPathGeometry.h"

#include "lib/NL/MotionControl/Path/Builders/NLPathBuildArc.h"
#include "lib/NL/MotionControl/Path/Builders/NLPathBuildSegment.h"

#include "lib/NL/MotionControl/Path/Builders/Waypoints/NLPathSection.h"

Nu32 NLPATH_SECTION::computeAndExtract(NLPATH_WAYPOINT *pwp0, NLPATH_WAYPOINT *pwp1, NLPATH_HEURISTIC heuristic, NLPATH_GEOMETRY *pgeom, const Nf32 arc_length_threshold)
{
	NErrorIf((pwp0->rr < 0.0f) || (pwp0->rr > 1.0f), NERROR_VALUE_OUTOFRANGE);
	// Reset m_uid
	// RESET_NLPATH_SECTION_UID(m_uid);
	Nmem0(this, NLPATH_SECTION);

	NVEC2f32 ortho0, ortho1;
	NVEC2f32 p0p1;
	Nf32 lp0p1;
	Nf32 r;
	Nu32 nb;
	Nu32 adjusted = 0;

	// Vecteur p0p1
	p0p1.x = pwp1->p.x - pwp0->p.x;
	p0p1.y = pwp1->p.y - pwp0->p.y;
	lp0p1 = NVec2Length(&p0p1);

	// TODO: Recherche pr�alable d'une section de type P ( Point ) on extrait alors directement une primitive de type POINT !!!
	/*
	if (!lp0p1) ou (lp0p1 < longueur mini )
	{

		return 0;
	}
	*/

	// Recherche pr�alable d'une section de type A ( Aligned )
	Nf32 dot = NVec2DotProduct(&pwp0->u, &pwp1->u);
	if (dot > NLPATH_SECTION_WAYPOINTS_ALIGNMENT_THRESOLD)
	{
		NVEC2f32 vs;
		vs.x = pwp0->u.x + pwp1->u.x;
		vs.y = pwp0->u.y + pwp1->u.y;

		dot = NVec2DotProduct(&p0p1, &vs) / (lp0p1 * NVec2Length(&vs));
		if (dot > NLPATH_SECTION_WAYPOINTS_ALIGNMENT_THRESOLD)
		{
			m_uid = MAKE_NLPATH_SECTION_UID(1, 0, NL_ExtractSegmentPrimitives(&pwp0->p, &pwp1->p, 0, pgeom), 0);
			return 0;
		}
	}

	// Identification pr�alable d'un BuildCircleRatio NULL ( ou n�gatif)
	// Se fait apr�s la recherche du TYPE A, car dans un cas ( TYPE A) la liaison est consid�r�e comme "bonne" et dans l'autre ( ratio rayon <= 0 ) comme Mauvaise
	if (pwp0->rr <= 0.0f)
	{
		// TODO: En cas de BuildCircleRatio NULL alors il faut ici ins�rer une(2?) primitive(s) de type POINT !!!!!
		m_uid = MAKE_NLPATH_SECTION_UID(1, 0, NL_ExtractSegmentPrimitives(&pwp0->p, &pwp1->p, FLAG_NLPATH_PRIMITIVE_RAW, pgeom), 0);
		return 0;
	}

	// Si on arrive ici, c'est que la section n'est pas de type A ...
	// Nous allons donc construire les 4 cercles et les 4 paires de cercles n�c�ssaires � la d�termination ult�rieure du type ( C ou S. )

	// Rayon commun des cercles de construction
	r = (lp0p1 / 4.0f) * pwp0->rr;

	// Mise en place des 4 cercles de contruction
	// -------------------------------------------
	// Le vecteur ortho0 est le resultat du produit vectoriel du vecteur "Z",perpendiculaire au plan de l'�cran et pointant "vers nous",  avec le vecteur 'pfrom->u'.
	//		NVEC3 vz = { 0.0f,0.0f,1.0f };
	//		ortho0.x = vz.y*u0.z - vz.z*u0.y;							 ortho0.x = -u0.y;
	//		ortho0.y = vz.z*u0.x - vz.x*u0.z;			soit			 ortho0.y =  u0.x;
	//		ortho0.z = vz.x*u0.y - vz.y*u0.x;							 ortho0.z =  0;
	ortho0.x = -pwp0->u.y;
	ortho1.x = -pwp1->u.y;
	ortho0.y = pwp0->u.x;
	ortho1.y = pwp1->u.x;

	m_circle[0].r = r;
	m_circle[0].o.x = pwp0->p.x + ortho0.x * r;
	m_circle[0].o.y = pwp0->p.y + ortho0.y * r;
	m_circle[0].sgn = NSIGN((pwp0->u.x) * (ortho0.y) - (pwp0->u.y) * (ortho0.x)); // = signe de la composante z du produit vectoriel Ui et OiPi = (Ui->x*OiPi->y) - (Ui->y*OiPi->x)

	m_circle[1].r = r;
	m_circle[1].o.x = pwp0->p.x - ortho0.x * r;
	m_circle[1].o.y = pwp0->p.y - ortho0.y * r;
	m_circle[1].sgn = NSIGN((pwp0->u.x) * (-ortho0.y) - (pwp0->u.y) * (-ortho0.x)); // = signe de la composante z du produit vectoriel Ui et OiPi = (Ui->x*OiPi->y) - (Ui->y*OiPi->x)

	m_circle[2].r = r;
	m_circle[2].o.x = pwp1->p.x + ortho1.x * r;
	m_circle[2].o.y = pwp1->p.y + ortho1.y * r;
	m_circle[2].sgn = NSIGN((pwp1->u.x) * (ortho1.y) - (pwp1->u.y) * (ortho1.x)); // = signe de la composante z du produit vectoriel Ui et OiPi = (Ui->x*OiPi->y) - (Ui->y*OiPi->x)

	m_circle[3].r = r;
	m_circle[3].o.x = pwp1->p.x - ortho1.x * r;
	m_circle[3].o.y = pwp1->p.y - ortho1.y * r;
	m_circle[3].sgn = NSIGN((pwp1->u.x) * (-ortho1.y) - (pwp1->u.y) * (-ortho1.x)); // = signe de la composante z du produit vectoriel Ui et OiPi = (Ui->x*OiPi->y) - (Ui->y*OiPi->x)

	// Setup des 4 paires de cercles possibles
	m_pair[0].setup(pwp0, &m_circle[0], pwp1, &m_circle[2]);
	m_pair[1].setup(pwp0, &m_circle[0], pwp1, &m_circle[3]);
	m_pair[2].setup(pwp0, &m_circle[1], pwp1, &m_circle[2]);
	m_pair[3].setup(pwp0, &m_circle[1], pwp1, &m_circle[3]);

	// Selection de la "meilleure paire"
	m_pBestPair = &m_pair[heuristic(this /*, pdts*/)];

	// V�rification et Ajustement dans le cas de longueurs d'arc � parcourir inf�rieures au seuil fix� par "arc_length_threshold"
	if (arc_length_threshold)
	{
		if (NABS(m_pBestPair->beta0) * m_pBestPair->pcircle0->r < arc_length_threshold)
		{
			// on ajuste LA SORTIE DU CERCLE 0 pour qu'elle coincide avec pwp0 et, en cons�quence, pwp1 se retrouve d�plac�!
			//
			/*
			MISE A JOUR DES PARAMETRES DE LA PAIRE ET DE PWP1:
			Nu16					type;		---> ne change pas !
			Nu16					flags;		---> ne change pas !

			NLPATH_SECTION_BCIRCLE	*pcircle0;	---> ne change pas !
			NLPATH_SECTION_BCIRCLE	*pcircle1;	---> CHANGE : LA POSITION DU CERCLE 1 CHANGE

			Nf32					alpha;		---> ne change pas !
			Nf32					beta0;		---> CHANGE : DEVIENT ZERO
			Nf32					beta1;		---> ne change pas !

			NVEC2					s;			---> CHANGE : DEVIENT egal � pwp0->p !
			NVEC2					e;			---> CHANGE : SE PLACE 'AU BOUT' de se*lse
			NVEC2					se;			---> CHANGE : DEVIENT egal � pwp0->u !
			Nf32					lse;		---> ne change pas !
			*/

			// Expression de pwp1 ( p et u )  dans une base directe ayant pour origine le centre du cercle 1 et i = se
			NVEC2f32 op, u2, p2;
			NVEC2f32 i, j;

			i = m_pBestPair->se;
			j.x = -i.y;
			j.y = i.x;

			op.x = pwp1->p.x - m_pBestPair->pcircle1->o.x;
			op.y = pwp1->p.y - m_pBestPair->pcircle1->o.y;

			p2.x = NVec2DotProduct(&op, &i);
			p2.y = NVec2DotProduct(&op, &j);
			u2.x = NVec2DotProduct(&pwp1->u, &i);
			u2.y = NVec2DotProduct(&pwp1->u, &j);

			// mise � jour de la paire avec un beta0 = 0
			m_pBestPair->beta0 = 0.0f;
			m_pBestPair->s = pwp0->p;
			m_pBestPair->se = pwp0->u;
			m_pBestPair->e.x = m_pBestPair->s.x + m_pBestPair->se.x * m_pBestPair->lse;
			m_pBestPair->e.y = m_pBestPair->s.y + m_pBestPair->se.y * m_pBestPair->lse;

			m_pBestPair->pcircle1->o.x = m_pBestPair->e.x - m_pBestPair->se.y * m_pBestPair->pcircle1->r * m_pBestPair->pcircle1->sgn;
			m_pBestPair->pcircle1->o.y = m_pBestPair->e.y + m_pBestPair->se.x * m_pBestPair->pcircle1->r * m_pBestPair->pcircle1->sgn;

			// repositionnement de pwp1 pour conserver la coh�rence avbec le cercle 1
			i = m_pBestPair->se;
			j.x = -i.y;
			j.y = i.x;

			pwp1->p.x = m_pBestPair->pcircle1->o.x + i.x * p2.x + j.x * p2.y;
			pwp1->p.y = m_pBestPair->pcircle1->o.y + i.y * p2.x + j.y * p2.y;

			pwp1->u.x = i.x * u2.x + j.x * u2.y;
			pwp1->u.y = i.y * u2.x + j.y * u2.y;

			adjusted++;
		}

		if (NABS(m_pBestPair->beta1) * m_pBestPair->pcircle1->r < arc_length_threshold)
		{
			// on ajuste pwp1 !
			pwp1->p = m_pBestPair->e;
			pwp1->u = m_pBestPair->se;

			// mise � jour de la paire avec un beta1 = 0
			m_pBestPair->beta1 = 0.0f;

			adjusted++;
		}
	}

	// Extract Primitives:
	NLPATH_BARC barc;
	NVEC2f32 es;
	NVEC2f32 v;
	NVEC2f32 i, j;
	Nf32 cosbo2, sinbo2;
	es.x = -m_pBestPair->se.x;
	es.y = -m_pBestPair->se.y;

	barc.m_BBeta = m_pBestPair->beta0;
	barc.m_BRadius = m_pBestPair->pcircle0->r;
	barc.m_BCenter = m_pBestPair->pcircle0->o;
	barc.m_Icr = pwp0->icr_A;
	barc.m_Sgn = (Nf32)m_pBestPair->pcircle0->sgn;

	// EXTRACTION DES PRIMITIVES ISSUE DU PREMIER ARC DE CERCLE DE CONSTRUCTION
	if ((m_pBestPair->beta0 >= NLPATH_SECTION_BETA_THRESHOLD) && (barc.m_Icr != 0.0f))
	{
		// l'arc de cercle est trop "ouvert". Il doit �tre scind� en deux.
		// beta0 represente la "distance angulaire" s�parant p0 de s en tournant dans le sens du cercle0.
		i.x = pwp0->p.x - m_pBestPair->pcircle0->o.x;
		i.y = pwp0->p.y - m_pBestPair->pcircle0->o.y;
		NVec2Normalize(&i);
		j = pwp0->u;

		cosbo2 = cos(m_pBestPair->beta0 / 2.0f);
		sinbo2 = sin(m_pBestPair->beta0 / 2.0f);
		v.x = i.x * cosbo2 + j.x * sinbo2;
		v.y = i.y * cosbo2 + j.y * sinbo2;

		barc.m_B = pwp0->p;
		barc.m_BTgt = pwp0->u;
		barc.m_D.x = m_pBestPair->pcircle0->o.x + v.x * m_pBestPair->pcircle0->r;
		barc.m_D.y = m_pBestPair->pcircle0->o.y + v.y * m_pBestPair->pcircle0->r;
		barc.m_DTgt.x = v.y * m_pBestPair->pcircle0->sgn;
		barc.m_DTgt.y = -v.x * m_pBestPair->pcircle0->sgn;
		nb = barc.extractPrimitives(pgeom);

		barc.m_B = barc.m_D;
		barc.m_BTgt.x = -barc.m_DTgt.x;
		barc.m_BTgt.y = -barc.m_DTgt.y;
		barc.m_D = m_pBestPair->s;
		barc.m_DTgt = es;
		nb += barc.extractPrimitives(pgeom);

		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_A(m_uid, nb);
	}
	else if (m_pBestPair->beta0 != 0.0f)
	{
		barc.m_B = pwp0->p;
		barc.m_BTgt = pwp0->u;
		barc.m_D = m_pBestPair->s;
		barc.m_DTgt = es;
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_A(m_uid, barc.extractPrimitives(pgeom));
	}
	else //(m_pBestPair->beta0 == 0.0f)
	{
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_A(m_uid, 0);
	}

	// EXTRACTION DES PRIMITIVES ISSUE DU SEGMENT TANGENT RELIANT LES DEUX ARCS DE CERCLES DE CONSTRUCTION
	if (m_pBestPair->lse != 0.0f)
	{
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_B(m_uid, NL_ExtractSegmentPrimitives(&m_pBestPair->s, &m_pBestPair->e, 0, pgeom));
	}
	else
	{
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_B(m_uid, 0);
	}

	// EXTRACTION DES PRIMITIVES ISSUE DU SECOND ARC DE CERCLE DE CONSTRUCTION
	barc.m_BBeta = m_pBestPair->beta1;
	barc.m_BRadius = m_pBestPair->pcircle1->r;
	barc.m_BCenter = m_pBestPair->pcircle1->o;
	barc.m_Icr = pwp1->icr_C;
	barc.m_Sgn = (Nf32)m_pBestPair->pcircle1->sgn;

	if ((m_pBestPair->beta1 >= NLPATH_SECTION_BETA_THRESHOLD) && (barc.m_Icr != 0.0f))
	{
		// l'arc de cercle est trop "ouvert". Il doit �tre scind� en deux.
		// beta0 represente la "distance angulaire" s�parant p0 de s en tournant dans le sens du cercle0.
		i.x = pwp1->p.x - m_pBestPair->pcircle1->o.x;
		i.y = pwp1->p.y - m_pBestPair->pcircle1->o.y;
		NVec2Normalize(&i);
		j = pwp1->u;

		cosbo2 = cos(m_pBestPair->beta1 / 2.0f);
		sinbo2 = sin(m_pBestPair->beta1 / 2.0f);
		v.x = i.x * cosbo2 - j.x * sinbo2;
		v.y = i.y * cosbo2 - j.y * sinbo2;

		barc.m_B = m_pBestPair->e;
		barc.m_BTgt = m_pBestPair->se;
		barc.m_D.x = m_pBestPair->pcircle1->o.x + v.x * m_pBestPair->pcircle1->r;
		barc.m_D.y = m_pBestPair->pcircle1->o.y + v.y * m_pBestPair->pcircle1->r;
		barc.m_DTgt.x = v.y * m_pBestPair->pcircle1->sgn;
		barc.m_DTgt.y = -v.x * m_pBestPair->pcircle1->sgn;
		nb = barc.extractPrimitives(pgeom);

		barc.m_B = barc.m_D;
		barc.m_BTgt.x = -barc.m_DTgt.x;
		barc.m_BTgt.y = -barc.m_DTgt.y;
		barc.m_D = pwp1->p;
		barc.m_DTgt.x = -pwp1->u.x;
		barc.m_DTgt.y = -pwp1->u.y;
		nb += barc.extractPrimitives(pgeom);

		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_C(m_uid, nb);
	}
	else if (m_pBestPair->beta1 != 0.0f)
	{
		barc.m_B = m_pBestPair->e;
		barc.m_BTgt = m_pBestPair->se;
		barc.m_D = pwp1->p;
		barc.m_DTgt.x = -pwp1->u.x;
		barc.m_DTgt.y = -pwp1->u.y;
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_C(m_uid, barc.extractPrimitives(pgeom));
	}
	else //(m_pBestPair->beta1 == 0.0f)
	{
		m_uid = SET_NLPATH_SECTION_PRIMITIVES_NB_C(m_uid, 0);
	}

	m_uid = SET_NLPATH_SECTION_VALIDITY(m_uid, 1);

	return adjusted; // 0,1 ou 2 selon le nombre d'ajustement(s) effectu�(s) [ 1 ajustement = suppression d'un arc de cercle, jug� trop court ]
}

void NLPATH_SECTION::drawBuildingCircles(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR *pcol)
{
	// Tra�age des 4 cercles de construction utilis� par la section
	NVEC3f32 c, d;
	c.z = 0.0f;
	d.z = 0.0f;
	NVEC2f32 xtd = {0.05f, 0.05f};

	Nchar txt[32];
	NUT_DRAWTEXT drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.1f;
	drawtxt.Color = *pcol;

	NUT_DRAW_ELLIPSE ellipse;
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	ellipse.Color = *pcol;
	ellipse.SliceNb = 127;
	ellipse.Center.z = 0.0f;
	FLAG_ON(ellipse.Flags, FLAG_NUT_DRAW_ELLIPSE_DASHLINE);

	// Tracage des 2 Way points associ�s � la section et des vecteurs "u" associ�s.
	c.x = pwp0->p.x;
	c.y = pwp0->p.y;
	NUT_Draw_Cross(&c, &xtd, pcol);
	d.x = c.x + pwp0->u.x * m_circle[0].r;
	d.y = c.y + pwp0->u.y * m_circle[0].r;
	NUT_Draw_Segment(&c, &d, pcol);

	NUT_Draw_Text("pwp0", &c, &drawtxt);
	NUT_Draw_Text("pwp0->u", &d, &drawtxt);

	c.x = pwp1->p.x;
	c.y = pwp1->p.y;
	NUT_Draw_Cross(&c, &xtd, pcol);
	d.x = c.x + pwp1->u.x * m_circle[0].r;
	d.y = c.y + pwp1->u.y * m_circle[0].r;
	NUT_Draw_Segment(&c, &d, pcol);

	NUT_Draw_Text("pwp1", &c, &drawtxt);
	NUT_Draw_Text("pwp1->u", &d, &drawtxt);

	// Tracage des rayons des cercles, soit :
	//			pour les 2 premiers cercles: le segment [pwp0, centre du cercle]
	//			pour les 2 cercles suivants: le segment [pwp1, centre du cercle]
	// notes:
	// pour simplifier le tracage on reliera directement les centres des cercles 0,1 et 2,3.
	c.x = m_circle[0].o.x;
	c.y = m_circle[0].o.y;
	d.x = m_circle[1].o.x;
	d.y = m_circle[1].o.y;
	NUT_Draw_Segment(&c, &d, pcol);

	c.x = m_circle[2].o.x;
	c.y = m_circle[2].o.y;
	d.x = m_circle[3].o.x;
	d.y = m_circle[3].o.y;
	NUT_Draw_Segment(&c, &d, pcol);

	// Tracage des cercles et de leur centre
	for (Nu32 i = 0; i < 4; i++)
	{
		ellipse.Extents.x = m_circle[i].r;
		ellipse.Extents.y = m_circle[i].r;
		ellipse.Center.x = m_circle[i].o.x;
		ellipse.Center.y = m_circle[i].o.y;
		NUT_Draw_Ellipse(&ellipse);
		c.x = m_circle[i].o.x;
		c.y = m_circle[i].o.y;
		NUT_Draw_Cross(&c, &xtd, pcol);

		if (m_circle[i].sgn == -1)
			sprintf(txt, "o%d(-)", i);
		else
			sprintf(txt, "o%d(+)", i);

		NUT_Draw_Text(txt, &c, &drawtxt);
	}
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine 0 � 4 des 4 building paires ( ... de cercles de construction reli�s entre eux par une tangente ) contenus dans la section.
 *			\b ATTENTION! Il est possible qu'une paire ne soit pas valide, simplement car elle n'aura pas �t� d�finie, par exemple quand la section reliant 2 waypoints
 *			a choisi le segment ligne droite plut�t que la combinaison "Arc / segment / Arc". Dans ce cas, cette pair ne sera pas dessin�e m�me si "what" le pr�cise.
 *	@param	pwp0 Une building pair ne stocke pas les 2 waypoints desquels elle est issue. Pour la tracer il faut donc les lui rappeler. pwp0 est le premier.
 *	@param	pwp1 Second way point utilis� lors de la construction de la building pair.
 *	@param	pcol Couleur utilis�e pour dessiner.
 *	@param	what Les valeurs ( 0 ou 1 ) des Bits 0,1,2,3 sp�cifient quelles building pairs doivent �tre dessin�es. ( BIT_0 � 1 = dessine la pair 0, BIT_1 � 1 = dessine la pair 1, ... )

 */
// ------------------------------------------------------------------------------------------
void NLPATH_SECTION::drawBuildingPairs(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR *pcolcircles, const NCOLOR *pcolpair, const NCOLOR *pcolalpha, const NCOLOR *pcolbeta, const Nu32 what)
{
	// Tracage des Building Pair
	for (Nu32 i = 0; i < 4; i++)
	{
		if (what & (1 << i))
		{
			m_pair[i].draw(pwp0, pwp1, pcolpair);

			if (ISFLAG_ON(what, BIT_4)) // FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_ALPHA_ANGLES ---> BIT_4
				m_pair[i].drawAlphaAngles(pcolalpha);

			if (ISFLAG_ON(what, BIT_5)) // FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_BETA_ANGLES ---> BIT_5
				m_pair[i].drawBetaAngles(pwp0, pcolbeta);

			if (ISFLAG_ON(what, BIT_6)) // FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_LINES ---> BIT_6
				m_pair[i].drawPointsAndLines(pcolpair, pcolcircles);
		}
	}
}

/*
void NLPATH_SECTION::drawCpairs(const NLPATH_WAYPOINT *pwp0, const NCOLOR * pcol)
{
	for (Nu32 i = 0; i < 4; i++)
	{
		if (m_pair[i].type == NLPATH_SECTION_BPAIR::TYPE_C)
		{
			m_pair[i].draw(pwp0,pcol);
		}
	}
}

void NLPATH_SECTION::drawSpairs(const NLPATH_WAYPOINT *pwp0, const NCOLOR * pcol)
{
	for (Nu32 i = 0; i < 4; i++)
	{
		if (m_pair[i].type == NLPATH_SECTION_BPAIR::TYPE_S)
		{
			m_pair[i].draw(pwp0,pcol);
		}
	}
}

void NLPATH_SECTION::drawBestPair(const NLPATH_WAYPOINT *pwp0, const NCOLOR * pcol)
{
	m_pBestPair->draw(pwp0,pcol);
}
*/