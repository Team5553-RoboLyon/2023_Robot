
#include "../../../../../N/NFlags.h"
#include "../../../../../N/NMemory.h"
#include "../../../../../N/NMath.h"
#include "../../../../../N/Maths/NVec2f32.h"
#include "../../../../../N/Maths/NVec3f32.h"
#include "../../../../../N/Maths/NVecLimits.h"
#include "../../../../../N/NErrorHandling.h"
#include "../../../../../N/Utilities/Draw/NUT_Draw.h"

#include "NLPathWayPoint.h"
#include "NLPathSectionBuildPair.h"


void NLPATH_SECTION_BPAIR::setup(const NLPATH_WAYPOINT *pwp0, const NLPATH_SECTION_BCIRCLE *pc0, const NLPATH_WAYPOINT *pwp1, const NLPATH_SECTION_BCIRCLE *pc1)
{
	NErrorIf(pc0->r != pc1->r, NERROR_INCONSISTENT_VALUES);
	NErrorIf(pc0->r <= 0.0f || pc1->r <= 0.0f, NERROR_INCONSISTENT_VALUES);

	NVEC2		o1p1;	// Vecteur partant du centre 'o1' du cercle 1 et pointant sur le waypoint 0 situé sur le cercle 1.
	NVEC2		p1e;	// Vecteur partant du waypoint 1 situé sur le cercle 1 et pointant sur le point d'entrée 'e' également situé sur le cercle 1.

	NVEC2		o0p0;	// Vecteur partant du centre 'o0' du cercle 0 et pointant sur le waypoint 0 situé sur le cercle 0.
	NVEC2		p0s;	// Vecteur partant du waypoint 0 situé sur le cercle 0 et pointant sur le point de sortie 's' également situé sur le cercle 0.
	
	NVEC2		o0s;	// Vecteur partant du centre 'o0' du cercle 0 et pointant sur le point de sortie 's' situé sur le cercle 0.

	NVEC2		o1o0;	// Vecteur partant du centre 'o1' du cercle 1 et pointant sur le centre 'o0' du cercle 0. 
	NVEC2		o0o1;	// Vecteur partant du centre 'o0' du cercle 0 et pointant sur le centre 'o1' du cercle 1.
	Nf32	   lo0o1;	// Longueur du vecteur 'o0o1'

	NVEC2		p0p1;	// Vecteur partant du waypoint 'p0'et pointant sur le waypoint p1.
	Nf32	   lp0p1;	// Longueur du vecteur 'p0p1'

	NVEC2		i, j;	// vecteurs unitaires d'une base orthonormée pour construire les points d'entrée et de sortie en cas de type S
	NVEC2		vp;		// vecteur perpendiculaire à o0o1 et formant une base directe OU indirecte avec o0o1 selon le sens du cercle considéré lors du calcul.


	pcircle0	= (NLPATH_SECTION_BCIRCLE*)pc0;
	pcircle1	= (NLPATH_SECTION_BCIRCLE*)pc1;

// +------------------------------------------------------------------------------------+
// |																					|
// +---	1/4: Choix du type de liaison: type C ou Type S									|
// |																					|
// +------------------------------------------------------------------------------------+
//
	// Liaison de Type C
	if (pc0->sgn == pc1->sgn)
	{
		type	= TYPE_C;
		alpha	= NF32_PI_2;

		// o0o1
		o0o1.x = pc1->o.x - pc0->o.x;
		o0o1.y = pc1->o.y - pc0->o.y;
		lo0o1 = NVec2Length(&o0o1);

		// CALCUL DES POINTS D'ENTREE ET DE SORTIE 's' et 'e' situés respectivement sur le cercle 0 et le cercle 1
		//
		// Le cercle 0 est de sens direct:
		if (pc0->sgn == 1)
		{
			o0s.x =  o0o1.y * pc0->r / lo0o1;
			o0s.y = -o0o1.x * pc0->r / lo0o1;

			// point de sortie situé sur le cercle 0 ( point où la tangente touche le cercle )
			s.x = pc0->o.x + o0s.x;
			s.y = pc0->o.y + o0s.y;

			// entrée sur cercle 1
			e.x = pc1->o.x + o0s.x; // |_ Car dans le cas d'une liaison en C et parceque les deux cercles ont le même rayon, nous avons "o0s = o1e"
			e.y = pc1->o.y + o0s.y; // |
		}
		else
		// Le cercle 0 est de sens direct:
		{
			o0s.x = -o0o1.y * pc0->r / lo0o1;;
			o0s.y =  o0o1.x * pc0->r / lo0o1;;

			// sortie sur cercle 0
			s.x = pc0->o.x + o0s.x;
			s.y = pc0->o.y + o0s.y;

			// entrée sur cercle 1
			e.x = pc1->o.x + o0s.x;// |_ Car dans le cas d'une liaison en C et parceque les deux cercles int le même rayon, nous avons "o0s = o1e"
			e.y = pc1->o.y + o0s.y;// |
		}

		// CALCUL DES OUVERTURES ANGULAIRES 'beta0' et 'beta1'
		//
		// OUVERTURE ANGULAIRE beta0: angle parcouru depuis p0 jusqu'au point de sortie 's' situé sur le cercle 0.
		o0p0.x = pwp0->p.x - pc0->o.x;
		o0p0.y = pwp0->p.y - pc0->o.y;
		
		// ... Utilisation du produit scalaire de 'p0s' avec 'u0' ( partant de p0 et pointant dans la direction du robot  ) pour déterminer si la sortie 's' est 'devant' ou 'derriere' le point p0.
		p0s.x = s.x - pwp0->p.x;
		p0s.y = s.y - pwp0->p.y;
		if (NVec2DotProduct(&p0s, &pwp0->u) < 0.0f)
			beta0 = NF32_2PI - acosf(NVec2DotProduct(&o0p0, &o0s) / (pc0->r*pc0->r));
		else
			beta0 = acosf(NVec2DotProduct(&o0p0, &o0s) / (pc0->r*pc0->r));

		// OUVERTURE ANGULAIRE beta1: angle parcouru depuis le point d'entrée 'e' situé sur le cercle 1 jusq'au point p1.
		o1p1.x = pwp1->p.x - pc1->o.x;
		o1p1.y = pwp1->p.y - pc1->o.y;

		// ... Utilisation du produit scalaire de 'p1e' avec 'u1' ( partant de p1 et pointant dans la direction du robot  ) pour déterminer si l'entrée 'e' est 'devant' ou 'derriere' le point p1.
		p1e.x = e.x - pwp1->p.x;
		p1e.y = e.y - pwp1->p.y;
		if (NVec2DotProduct(&p1e, &pwp1->u) < 0.0f)
			beta1 = acosf(NVec2DotProduct(&o1p1, &o0s) / (pc1->r*pc1->r));				// !!! o1e = o0s donc on ne calcule evidement pas 'o1e' et on utilise 'o0s' !
		else
			beta1 = NF32_2PI - acosf(NVec2DotProduct(&o1p1, &o0s) / (pc1->r*pc1->r));	// !!! o1e = o0s donc on ne calcule evidement pas 'o1e' et on utilise 'o0s' !
	}
	// Liaison de Type S
	else
	{
		type = TYPE_S;
		// Soit "N" le point d'intersection des deux tangentes (M0,M1) et (M'0,M'1) ( Ref. page 54 )
		// D'apres Thales: 
		//					R0 / R1 = [o0,N]/[o1,N] = [M0,N]/[M1,N] Avec R0 et R1 les rayon des cercles considérés qui sont DIFFERENTS ! [ Dans Ref. Ils sont égaux ... :( ]
		//							
		// De plus, 		lo0o1 = [o0,N] + [o1,N]
		//
		// ainsi,	
		//					[o0,N]= lo0o1 - [o1,N]
		//					R0/R1 = ( lo0o1 - [o1,N] ) / [o1,N]
		//					R0/R1 = lo0o1 / [o1,N] - 1
		//					R0/R1 + 1 = lo0o1 / [o1,N]
		//
		//					R0/R1 + 1 = lo0o1 / [o1,N]
		//
		//					[o1,N] = lo0o1 / ( R0/R1 + 1 )
		//					et
		//					[o0,N] = lo0o1 - [o1,N]
		//
		//					alpha0 angle entre o0o1 et o0M0
		//					alpha0 = acos( R0/[o0,N] )
		//
		//					alpha1 angle entre o0o1 et o1M1
		//					alpha1 = acos( R1/[o1,N] )
		//
		//					Bien evidement, on remarque que 
		//					alpha0 = alpha1
		//
		// Ref page 54 va un peu plus loin et propose le calcul suivant pour alpha:
		//					
		//					alpha = acos( p0p1 / (2o0o1) )
		//
		// Rien de plus facile à comprendre. Ce résultat se base sur le fait que:
		//
		//					R = p0p1/4 ( regle arbitraire, mais logique, fixée par le programme )
		//					o1n = o0n = o0o1/2 ( car les deux cercles ont même rayon R )				
		//					
		// ainsi, si on repart du triangle rectangle o0,M0,N, on a
		//					
		//					cos(alpha)	= o0M0 / o0N 
		//								= R / (o0o1/2)
		//								= (p0p1/4) / (o0o1/2)
		//								= p0p1 / (2o0o1)
		// soit,
		//					alpha = acos( p0p1 / (2o0o1) )	CQFD
		//
		// Dans la version de l'algorythme	que nous proposons ici, nous laissons la possibilité aux utilisateurs de modifier la valeurs des rayons R0 et R1 ...
		// ... Selon les regles suivantes:
		//
		//					R0 = R1
		//
		//					0 < R0 < (p0p1/4)
		//
		// Les rayons R0 et R1 seront calculés comme suit:
		//
		//					R0 = R1 = (p0p1/4) * k	avec k defini comme suit:	0 < k <= 1
		//
		// en conséquence, pour le calcul de alpha nous retiendrons:
		//					
		//					alpha = acos( R0 / (o0o1/2) )
		//					soit,
		//					alpha = acos( (2*R0) / o0o1 ) 
		
		// o0o1
		o0o1.x	= pc1->o.x - pc0->o.x;
		o0o1.y	= pc1->o.y - pc0->o.y;
		lo0o1	= NVec2Length(&o0o1);

		// p0p1
		p0p1.x	= pwp1->p.x - pwp0->p.x;
		p0p1.y	= pwp1->p.y - pwp0->p.y;
		lp0p1	= NVec2Length(&p0p1);

		// alpha
		alpha = acosf( (2.0f*pc0->r) / lo0o1 );


		// CALCUL DES POINTS D'ENTREE ET DE SORTIE 's' et 'e' situés respectivement sur le cercle 0 et le cercle 1
		//
		// définition de la base (i,j) avec,
		//					i partant du centre o0 du cercle 0 et pointant vers le centre o1 du cercle 1 
		//					j vecteur orthogonal à i et forùant avec lui une base directe.
		i = o0o1;
		NVec2Normalize(&i);
		j.x = -i.y;
		j.y = i.x;
		Nf32 cosalpha = cos(alpha);
		Nf32 sinalpha = sin(alpha);

		// Le cercle 0 tourne dans le sens direct, et donc le cercle 1 dans le sens indirect.
		if (pc0->sgn == 1)
		{
			// sortie sur cercle 0
			s.x = pc0->o.x + i.x*cosalpha*pc0->r - j.x*sinalpha*pc0->r;
			s.y = pc0->o.y + i.y*cosalpha*pc0->r - j.y*sinalpha*pc0->r;

			// entrée sur cercle 1
			e.x = pc1->o.x - i.x*cosalpha*pc1->r + j.x*sinalpha*pc1->r;
			e.y = pc1->o.y - i.y*cosalpha*pc1->r + j.y*sinalpha*pc1->r;

		}
		// Le cercle 0 tourne dans le sens indirect, et donc le cercle 1 dans le sens direct.
		else
		{
			// sortie sur cercle 0
			s.x = pc0->o.x + i.x*cosalpha*pc0->r + j.x*sinalpha*pc0->r;
			s.y = pc0->o.y + i.y*cosalpha*pc0->r + j.y*sinalpha*pc0->r;

			// entrée sur cercle 1
			e.x = pc1->o.x - i.x*cosalpha*pc1->r - j.x*sinalpha*pc1->r;
			e.y = pc1->o.y - i.y*cosalpha*pc1->r - j.y*sinalpha*pc1->r;
		}

		// CALCUL DES OUVERTURES ANGULAIRES 'beta0' et 'beta1'
		//
		// OUVERTURE ANGULAIRE beta0: angle parcouru depuis p0 jusqu'au point de sortie 's' situé sur le cercle 0.
		o0p0.x = pwp0->p.x - pc0->o.x;
		o0p0.y = pwp0->p.y - pc0->o.y;
		
		// ... Utilisation du produit scalaire de 'o0p0' avec 'vp' ( vecteur perpendiculaire à o0o1   ) pour déterminer ...
		// ... de quel côté de 'o0o1' se trouve le point 'p0'
		vp.x =  o0o1.y * pc0->sgn; // |_ Selon le sens du cercle on choisira de construire une base directe ou indirecte
		vp.y = -o0o1.x * pc0->sgn; // |
		if (NVec2DotProduct(&vp, &o0p0) < 0.0f)
		{
			beta0 = NF32_2PI - acosf(NVec2DotProduct(&o0o1, &o0p0) / (pc0->r*lo0o1)) - alpha;
		}
		else
		{
			beta0 = acosf(NVec2DotProduct(&o0o1, &o0p0) / (pc0->r*lo0o1)) - alpha;
		}

		if (beta0 < 0.0f)
		{
			beta0 += NF32_2PI;
		}

		// OUVERTURE ANGULAIRE beta1: angle parcouru depuis le point d'entrée 'e' situé sur le cercle 1 jusq'au point p1.
		o1p1.x = pwp1->p.x - pc1->o.x;
		o1p1.y = pwp1->p.y - pc1->o.y;
		o1o0.x = -o0o1.x;
		o1o0.y = -o0o1.y;

		// ... Utilisation du produit scalaire de 'o0p0' avec 'vp' ( vecteur perpendiculaire à o0o1   ) pour déterminer ...
		// ... de quel côté de 'o1o0' se trouve le point 'p0'	
		vp.x = -o1o0.y *  pc1->sgn;// |_ Selon le sens du cercle on choisira de construire une base directe ou indirecte ( ici le schema est inversé par rapport à celui utilisé pour l'évaluation de beta0 )
		vp.y =  o1o0.x *  pc1->sgn;// |
		if (NVec2DotProduct(&vp, &o1p1) < 0.0f)
		{
			beta1 = NF32_2PI - acosf(NVec2DotProduct(&o1p1, &o1o0) / (pc1->r*lo0o1)) - alpha;
		}
		else
		{
			beta1 = acosf(NVec2DotProduct(&o1p1, &o1o0) / (pc1->r*lo0o1)) - alpha;
		}
		if (beta1 < 0.0f)
		{
			beta1 += NF32_2PI;
		}
	}

	// Calcul de 'se' le vecteur partant du point de sortie 's'du cercle 0 et pointant sur le point d'entrée 'e' sur le cercle 1.
	// La longueur 'lse' du vecteur 'se' ( avant normalisation ) correspond à la longueur du segment [se] tangent aux deux cercles.
	se.x	= e.x - s.x;
	se.y	= e.y - s.y;
	lse		= NVec2Normalize(&se);
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine les angles ALPHA d'une paire de cercles. Pour chaque cercle, les 2 points de contact des tangentes sont répartis symétriquement de par et d'autre de l'axe reliant le centre des 
 *			deux cercles. Si pour un cercle on appelle M0 et M1 les deux points de contact,O1 le centre de ce cercle, et O2 le centre du deuxième cercle, on a alors:
 *			ALPHA = Angle[M0,O1,O2] = -Angle[M1,O1,O2]
 *			\b ATTENTION! Cette fonction n'existe qu'en mode _NEDITOR. Dans tout autre mode ( avec _NEDITOR non défini ) la fonction est ignorée à la compilation est donc inexistante.
 *	@param	pcolalpha Couleur de tracage des angles ALPHA.

 *	@return	rien.
 */
 // ------------------------------------------------------------------------------------------
void NLPATH_SECTION_BPAIR::drawAlphaAngles(const NCOLOR * pcolalpha)
{
	NVEC3		v;


	// Angles sur cercle 0:
	if (pcircle0)
	{
		// Tracage Angle Alpha
		NVEC2 o0o1;
		o0o1.x = pcircle1->o.x - pcircle0->o.x;
		o0o1.y = pcircle1->o.y - pcircle0->o.y;
		NVec2Normalize(&o0o1);
		o0o1.x *= pcircle0->r;
		o0o1.y *= pcircle0->r;

		v.z = 0.0f;
		v.x = pcircle0->o.x;
		v.y = pcircle0->o.y;
		NUT_Draw_Angle(&v, &o0o1, alpha, pcolalpha);
		NUT_Draw_Angle(&v, &o0o1, -alpha, pcolalpha);
	}

	if (pcircle1)
	{
		NVEC2 o1o0;
		o1o0.x = pcircle0->o.x - pcircle1->o.x;
		o1o0.y = pcircle0->o.y - pcircle1->o.y;
		NVec2Normalize(&o1o0);
		o1o0.x *= pcircle1->r;
		o1o0.y *= pcircle1->r;

		v.z = 0.0f;
		v.x = pcircle1->o.x;
		v.y = pcircle1->o.y;
		NUT_Draw_Angle(&v, &o1o0, alpha, pcolalpha);
		NUT_Draw_Angle(&v, &o1o0, -alpha, pcolalpha);
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine
 *			\b ATTENTION! Cette fonction n'existe qu'en mode _NEDITOR. Dans tout autre mode ( avec _NEDITOR non défini ) la fonction est ignorée à la compilation et donc inexistante.
 *	@param	pcol Couleur.
 *	@return	rien.
 */
 // ------------------------------------------------------------------------------------------
void NLPATH_SECTION_BPAIR::drawBetaAngles(const NLPATH_WAYPOINT *pwp0, const NCOLOR *pcolbeta)
{
	NVEC3		v;


	// Angles sur cercle 0:
	if (pcircle0)
	{
		// Pour le premier cercle nous avons besoin de pwp0 car pwp0 est le point de départ. 
		// L'angle Beta0 se mesure donc depuis l'axe [o0,pwp0].
		// Le point situé sur le cercle "à la fin de l'angle" sera le point s de sortie de cercle ...

		NVEC2f32	o0pwp0;	// Vecteur partant du centre du cercle 0 ( le premier cercle de la paire ) et pointant vers le point pwp0
		o0pwp0.x = pwp0->p.x - pcircle0->o.x;
		o0pwp0.y = pwp0->p.y - pcircle0->o.y;
		NVec2Normalize(&o0pwp0);
		o0pwp0.x *= pcircle0->r;
		o0pwp0.y *= pcircle0->r;

		// Beta
		v.x = pcircle0->o.x;
		v.y = pcircle0->o.y;
		v.z = 0.0f;
		NUT_Draw_Angle(&v, &o0pwp0, pcircle0->sgn*beta0, pcolbeta);
	}

	if (pcircle1)
	{
		// Pour le second cercle nous avons besoin de e car e est le point d'entrée (de départ) sur ce cercle. 
		// L'angle Beta1 se mesure donc depuis l'axe [o1,e].
		// Le point situé sur le cercle "à la fin de l'angle" sera le point pwp1 de sortie de ce second cercle ...
		// ... c'est pourquoi nous n'avons pas besoin ici de pwp1 (mais uniquement de pwp0.)

		NVEC2f32	o1e;
		o1e.x = e.x - pcircle1->o.x;
		o1e.y = e.y - pcircle1->o.y;
		NVec2Normalize(&o1e);
		o1e.x *= pcircle1->r;
		o1e.y *= pcircle1->r;

		v.x = pcircle1->o.x;
		v.y = pcircle1->o.y;
		v.z = 0.0f;
		NUT_Draw_Angle(&v, &o1e, pcircle1->sgn*beta1, pcolbeta);
	}
}

void NLPATH_SECTION_BPAIR::drawPointsAndLines(const NCOLOR * pcolpoints, const NCOLOR *pcollines)
{
	NVEC2 xtd = { 0.05f,0.05f };
	NVEC3 c,d;
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.1f;
	drawtxt.Color = *pcolpoints;

	if (pcircle0)
	{
		c.x = s.x;
		c.y = s.y;
		c.z = 0.0f;
		NUT_Draw_Cross(&c, &xtd, pcolpoints);
		c.x -= drawtxt.Size;
		c.y -= 1.0f*drawtxt.Size;
		NUT_Draw_Text("s", &c, &drawtxt);
	}

	if (pcircle1)
	{
		c.x = e.x;
		c.y = e.y;
		c.z = 0.0f;
		NUT_Draw_Cross(&c, &xtd, pcolpoints);
		c.x -= drawtxt.Size;
		c.y -= 1.25f*drawtxt.Size;
		NUT_Draw_Text("e", &c, &drawtxt);

		if (pcircle0)
		{
			c.x = pcircle0->o.x;
			c.y = pcircle0->o.y;
			c.z = 0.0f;
			d.x = pcircle1->o.x;
			d.y = pcircle1->o.y;
			d.z = 0.0f;
			NUT_Draw_Segment(&c, &d, pcollines);
		}
	}

}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine une paire de cercle de construction avec.
 *			\b ATTENTION! Cette fonction n'existe qu'en mode _NEDITOR. Dans tout autre mode ( avec _NEDITOR non défini ) la fonction est ignorée à la compilation et donc inexistante.
 *	@param	pcol Couleur.
 *	@return	rien.
 */
// ------------------------------------------------------------------------------------------
void NLPATH_SECTION_BPAIR::draw(const NLPATH_WAYPOINT *pwp0, const NLPATH_WAYPOINT *pwp1, const NCOLOR * pcol)
{	
	NVEC2f32 o0pwp0;	// Vecteur partant du centre du cercle 0 ( le premier cercle de la paire ) et pointant vers le point pwp0
	NVEC2f32 o1pwp1;	// Vecteur partant du centre du cercle 1 ( le second cercle de la paire ) et pointant vers le point pwp1
	NVEC2f32 o0s;
	NVEC2f32 o1e;

	// Traçage des deux Arcs de cercles de construction associés à la paire
	NUT_DRAW_ELLIPSE	ellipse;
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	FLAG_ON(ellipse.Flags, FLAG_NUT_DRAW_ELLIPSE_OPEN);
	ellipse.Color = *pcol;
	ellipse.SliceNb = 256;
	ellipse.Center.z = 0.0f;

	// PREMIER ARC DE CERCLE
	//
	// L'arc de construction 0 commence en pwp0 ( le point de départ) et se termine au point de sortie S (evidement situé sur le cercle 0...)
	// Les coordonnées normalisées du vecteur o0pwp0, interprétées comme le cosinus et le sinus d'un angle via la fonction 'atanf2' permettent de définir l'angle de départ dans un cercle trigo orienté sur 
	// le "repère écran"
	if (pcircle0)
	{
		o0pwp0.x = pwp0->p.x - pcircle0->o.x;
		o0pwp0.y = pwp0->p.y - pcircle0->o.y;
		NVec2Normalize(&o0pwp0);

		// Les coordonnées normalisées du vecteur o0s, interprétées comme le cosinus et le sinus d'un angle via la fonction 'atanf2' permettent de définir l'angle de sortie ...
		o0s.x = s.x - pcircle0->o.x;
		o0s.y = s.y - pcircle0->o.y;
		NVec2Normalize(&o0s);

		if (pcircle0->sgn == 1)
		{
			ellipse.FromAngle = NRADtoDEG(atan2f(o0pwp0.y, o0pwp0.x));
			ellipse.ToAngle = NRADtoDEG(atan2f(o0s.y, o0s.x));
		}
		else
		{
			ellipse.ToAngle = NRADtoDEG(atan2f(o0pwp0.y, o0pwp0.x));
			ellipse.FromAngle = NRADtoDEG(atan2f(o0s.y, o0s.x));
		}
		ellipse.Extents.x = pcircle0->r;
		ellipse.Extents.y = pcircle0->r;
		ellipse.Center.x = pcircle0->o.x;
		ellipse.Center.y = pcircle0->o.y;
		NUT_Draw_Ellipse(&ellipse);
	}

	// DEUXIEME ARC DE CERCLE
	//
	// L'arc de construction 1 commence au point 'e', d'entrée sur le cercle 1 et se termine au point 'pwp1'
	// Les coordonnées normalisées du vecteur o1e, interprétées comme le cosinus et le sinus d'un angle via la fonction 'atanf2' permettent de définir l'angle de départ dans un cercle trigo orienté sur 
	// le "repère écran"
	if (pcircle1)
	{
		o1e.x = e.x - pcircle1->o.x;
		o1e.y = e.y - pcircle1->o.y;
		NVec2Normalize(&o1e);

		// Les coordonnées normalisées du vecteur o1pwp1, interprétées comme le cosinus et le sinus d'un angle via la fonction 'atanf2' permettent de définir l'angle de sortie ...
		o1pwp1.x = pwp1->p.x - pcircle1->o.x;
		o1pwp1.y = pwp1->p.y - pcircle1->o.y;
		NVec2Normalize(&o1pwp1);

		if (pcircle1->sgn == 1)
		{
			ellipse.FromAngle = NRADtoDEG(atan2f(o1e.y, o1e.x));
			ellipse.ToAngle = NRADtoDEG(atan2f(o1pwp1.y, o1pwp1.x));
		}
		else
		{
			ellipse.ToAngle = NRADtoDEG(atan2f(o1e.y, o1e.x));
			ellipse.FromAngle = NRADtoDEG(atan2f(o1pwp1.y, o1pwp1.x));
		}
		ellipse.Extents.x = pcircle1->r;
		ellipse.Extents.y = pcircle1->r;
		ellipse.Center.x = pcircle1->o.x;
		ellipse.Center.y = pcircle1->o.y;
		NUT_Draw_Ellipse(&ellipse);
	}
	
	// TANGENTE
	NVEC3 v0, v1;
	v0.x = s.x;		v1.x = e.x;
	v0.y = s.y;		v1.y = e.y;
	v0.z = 0.0f;	v1.z = 0.0f;
	NUT_Draw_Segment(&v0, &v1, pcol);
	
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.1f;
	drawtxt.Color = *pcol;

	if (type == TYPE_C)
	{
		v1.x = (v0.x + v1.x) / 2.0f;
		v1.y = (v0.y + v1.y) / 2.0f;
		NUT_Draw_Text("TYPE C", &v1, &drawtxt);
	}
	else if (type == TYPE_S)
	{
		v1.x = (v0.x + v1.x) / 2.0f;
		v1.y = (v0.y + v1.y) / 2.0f;
		NUT_Draw_Text("TYPE S", &v1, &drawtxt);
	}
}
