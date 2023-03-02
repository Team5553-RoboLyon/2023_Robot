/*
#include "../../N/NMemory.h"
#include "../../N/NMath.h"
#include "../../N/Maths/NVec2f32.h"
#include "../../N/Maths/NVecLimits.h"

#include "../../N/NErrorHandling.h"
#include "NLPath.h"

#ifdef _NEDITOR
#include "../../N/Utilities/Draw/NUT_Draw.h"
#include "../../N/Utilities/NUT_X.h"
#endif
*/


/*
void FresnelIntegrals(const Nf32 s,Nf32 *cf, Nf32 *sf)
{
	Nf32 f  = (1.0f + 0.926f*s) / (2.0f + 1.792f*s + 3.104f*s*s);
	Nf32 g  = 1.0f / (2.0f + 4.142f*s + 3.492f*s*s + 6.670f*s*s*s);
	Nf32 an = (NF32_PI*s*s) / 2.0f;
	Nf32 cn = cos(an);
	Nf32 sn = sin(an);

	*cf = 0.5f + f * sn - g * cn;
	*sf = 0.5f - f * cn - g * sn;
}
*/

/*
typedef struct
{
	NVEC2	p;		// point de reference ( waypoint )
	NVEC2	o;		// centre du cercle
	NVEC2	u;		// Vecteur direction tangent au cercle
	Nf32	r;		// rayon du cercle
	Ns32	s;		// Signe ou sens de parcour du cercle.
}NLPATH_BUILDCIRCLE;

#define FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA0_PI	BIT_0
#define FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA1_PI	BIT_1

typedef struct
{
	Nu16	Flags;
	Nu16	type;	// Type de transition: en S ou en C ou en A ( Aligné ).

	Nf32	alpha;  // Transition en C et S. Les "points de contact entre les cercles et les tangentes" sont situés à un angle alpha de la perpendicaulaire à Ui. (! alpha = 0 pour trans.C )
	Nf32	beta0;	// Distance angulaire entre le point P0 et le point de sortie sur le cercle C0 et en respectant le sens du cercle. 
	Nf32	beta1;  // Distance angulaire entre le point P0 et le point de sortie sur le cercle C0 et en respectant le sens du cercle. 

	NVEC2	s;		// point de sortie situé sur le cercle 0
	NVEC2	e;		// point d'entrée situé sur le cercle 1
	NVEC2	se;		// vecteur 'sortie-entrée' ( dont la longueur avant normalisation est là longueur du segment "tangente" se)
	Nf32	lse;	// longueur du vecteur 'sortie-entrée' calculé avant normalisation.

	NVEC2	f0;		// point d'intersection des tangentes utilisées pour le calcul des clothoides sur le cercle 0 -- Ref. [2.14] page 59 ( point F ) 
	NVEC2	f1;		// point d'intersection des tangentes utilisées pour le calcul des clothoides sur le cercle 1 -- Ref. [2.14] page 59 ( point F ) 
	
	Nf32	l0;		// distance [p0,f0]
	Nf32	l1;		// distance [p1,f1]
	
	Nf32	clothoid_alpha0; // angle formé par les deux tangentes à l'arc de cercle 0. Soit U0 et se
	Nf32	clothoid_alpha1; // angle formé par les deux tangentes à l'arc de cercle 1. Soit U1 et se

	Nf32	clothoid_sigma0; // Dérivée de la courbure de la clothoide au cercle 0 ( la deuxième clothoide au cercle 0 est symetrique à la première, sa dérivée de courbure est la même au signe près).
	Nf32	clothoid_sigma1; // Dérivée de la courbure de la clothoide au cercle 1 ( la deuxième clothoide au cercle 1 est symetrique à la première, sa dérivée de courbure est la même au signe près).

	Nf32	clothoid_k0;	// courbure  au point de raccordement de la clothoide au cercle 0(... avec la deuxième clothoide qui lui est symétrique ).
	Nf32	clothoid_k1;	// courbure  au point de raccordement de la clothoide au cercle 1(... avec la deuxième clothoide qui lui est symétrique ).

	// Avec cercle inséré entre les deux clothoides
	Nf32	clothoid_tetha0;	// angle entre la tangente au point de racordement des 2 clothoides symetriques et l'axe u0 -- Ref. [2.17] page 63 
	Nf32	clothoid_tetha1;	// angle entre la tangente au point de racordement des 2 clothoides symetriques et l'axe u1 -- Ref. [2.17] page 63 

	Nf32	clothoid_r0;		// rayon de l'arc de cercle inséré entre les deux clothoides à sommets symetriques  créées pour remplacer l'arc de cercle 0
	Nf32	clothoid_r1;		// rayon de l'arc de cercle inséré entre les deux clothoides à sommets symetriques  créées pour remplacer l'arc de cercle 1

	NVEC2	clothoid_o0;		// centre de l'arc de cercle inséré entre les deux clothoides à sommets symetriques  créées pour remplacer l'arc de cercle 0
	NVEC2	clothoid_o1;		// centre de l'arc de cercle inséré entre les deux clothoides à sommets symetriques  créées pour remplacer l'arc de cercle 1

	Nf32	clothoid_sigma0i; // Dérivée de la courbure de la clothoide au cercle 0 avec cercle inséré de rayon 'clothoid_r0'
	Nf32	clothoid_sigma1i; // Dérivée de la courbure de la clothoide au cercle 1 avec cercle inséré de rayon 'clothoid_r1'


	NLPATH_BUILDCIRCLE *pc0;
	NLPATH_BUILDCIRCLE *pc1;
}NLPATH_BUILDCIRCLE_PAIR;

#define		TYPE_C			0
#define		TYPE_S			1

#define		CIRCLE00		0
#define		CIRCLE01		1
#define		CIRCLE10		2
#define		CIRCLE11		3

#define		RADIUS_COEF		0.25f	// 1/4

void NLPATH_CirclePair(NLPATH_BUILDCIRCLE_PAIR *presult, const NLPATH_BUILDCIRCLE *pc0, const NLPATH_BUILDCIRCLE *pc1)
{
	NErrorIf(pc0->r != pc1->r, NERROR_INCONSISTENT_VALUES);

	NVEC2	 p0p1;
	Nf32	lp0p1;

	NVEC2	 o0o1 ,    o0p0, o0q0;
	Nf32	lo0o1;//, lo0p0,lo0q0; les longueurs des vecteurs o0p0 et o0q0 valent toutes les deux 'r' ( rayon "des" cercles) 

	NVEC2	 o1o0,  o1p1,  o1q1;
	//Nf32	lo1o1, lo1p1, lo1qq; la longueur du vecteur o1o0 est la même que celle du vecteur o0o1, les longueurs des vecteurs o1p1 et o1q1 valent toutes les deux 'r' ( rayon "des" cercles) 

	NVEC2	p0s, p1e;

	presult->pc0 = (NLPATH_BUILDCIRCLE*)pc0;
	presult->pc1 = (NLPATH_BUILDCIRCLE*)pc1;

	// +------------------------------------------------------------------------------------+
	// |																					|
	// +---	1/4: Choix du type de transition												|
	// |																					|
	// +------------------------------------------------------------------------------------+
	if (pc0->s == pc1->s)
	{
		presult->type = TYPE_C;

		// Liaison en C
		//-------------
		
		presult->alpha = 0.0f;

		// Ouvertures angulaires: beta0 et beta1
		// beta0: angle parcouru depuis p0 jusqu'au point de sortie situé sur le cercle 0.

		// o0o1
		o0o1.x	= pc1->o.x - pc0->o.x;
		o0o1.y	= pc1->o.y - pc0->o.y;
		lo0o1	= NVec2Length(&o0o1);

		if (pc0->s == 1)
		{
			o0q0.x =  o0o1.y * pc0->r/lo0o1;
			o0q0.y = -o0o1.x * pc0->r/lo0o1;

			// sortie sur cercle 0
			presult->s.x = pc0->o.x + o0q0.x;
			presult->s.y = pc0->o.y + o0q0.y;
			//lo0q0 = NVec2Length(&o0q0) = r;

			// entrée sur cercle 1
			presult->e.x = pc1->o.x + o0q0.x; // o1q1 = o0q0 !!!
			presult->e.y = pc1->o.y + o0q0.y; // o1q1 = o0q0 !!!
			//lo1q1 = lo0q0 = NVec2Length(&o0q0) = r;
		}
		else
		{
			o0q0.x = -o0o1.y * pc0->r / lo0o1;;
			o0q0.y =  o0o1.x * pc0->r / lo0o1;;
			//lo0q0 = NVec2Length(&o0q0) = r;

			// sortie sur cercle 0
			presult->s.x = pc0->o.x + o0q0.x;
			presult->s.y = pc0->o.y + o0q0.y;

			// entrée sur cercle 1
			presult->e.x = pc1->o.x + o0q0.x; // o1q1 = o0q0 !!!
			presult->e.y = pc1->o.y + o0q0.y; // o1q1 = o0q0 !!!
			//lo1q1 = lo0q0 = NVec2Length(&o0q0) = r;
		}

		// Ouvertures angulaires: beta0 et beta1
		// beta0: angle parcouru depuis p0 jusqu'au point de sortie situé sur le cercle 0.
		o0p0.x = pc0->p.x - pc0->o.x;
		o0p0.y = pc0->p.y - pc0->o.y;
		//lo0p0 = NVec2Length(&o0p0) = r;
		p0s.x = presult->s.x - pc0->p.x;
		p0s.y = presult->s.y - pc0->p.y;
		if (NVec2DotProduct(&p0s, &pc0->u) < 0.0f)
		{
			FLAG_ON(presult->Flags, FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA0_PI);
			presult->beta0 = NF32_2PI - acosf( NVec2DotProduct(&o0p0, &o0q0) / (pc0->r*pc0->r) );
		}
		else
		{
			FLAG_OFF(presult->Flags, FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA0_PI);
			presult->beta0 = acosf(NVec2DotProduct(&o0p0, &o0q0) / (pc0->r*pc0->r));
		}


		//beta1: angle parcouru depuis le point d'entrée situé sur le cercle 1 jusq'au point p1.
		o1p1.x = pc1->p.x - pc1->o.x;
		o1p1.y = pc1->p.y - pc1->o.y;
		//lo1p1 = NVec2Length(&o1p1) = r;
		o1o0.x = -o0o1.x;
		o1o0.y = -o0o1.y;
		p1e.x = presult->e.x - pc1->p.x;
		p1e.y = presult->e.y - pc1->p.y;
		o1q1 = o0q0;

		if (NVec2DotProduct(&p1e, &pc1->u) < 0.0f)
		{
			FLAG_OFF(presult->Flags, FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA1_PI);
			presult->beta1 = acosf(NVec2DotProduct(&o1p1, &o1q1) / (pc1->r*pc1->r));
		}
		else
		{
			FLAG_ON(presult->Flags, FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA1_PI);
			presult->beta1 = NF32_2PI - acosf(NVec2DotProduct(&o1p1, &o1q1) / (pc1->r*pc1->r));
		}
	}
	else
	{
		presult->type = TYPE_S;

		// Liaison en S
		//-------------
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

		// o0o1
		o0o1.x = pc1->o.x - pc0->o.x;
		o0o1.y = pc1->o.y - pc0->o.y;
		lo0o1 = NVec2Length(&o0o1);

		// p0p1
		p0p1.x = pc1->p.x - pc0->p.x;
		p0p1.y = pc1->p.y - pc0->p.y;
		lp0p1 = NVec2Length(&p0p1);

		// alpha
		presult->alpha = acosf(lp0p1 / (2.0f*lo0o1));

		// Calcul des coordonnées du "point de sortie" sur le cercle 0 et du "point d'entrée" sur le cercle 1.
		NVEC2 i, j;
		i = o0o1;
		NVec2Normalize(&i);
		j.x = -i.y;
		j.y = i.x;
		Nf32 cosalpha = cos(presult->alpha);
		Nf32 sinalpha = sin(presult->alpha);

		// Le cercle 0 tourne dans le sens direct, et donc le cercle 1 dans le sens indirect.
		if (pc0->s == 1)
		{
			// sortie sur cercle 0
			presult->s.x = pc0->o.x + i.x*cosalpha*pc0->r - j.x*sinalpha*pc0->r;
			presult->s.y = pc0->o.y + i.y*cosalpha*pc0->r - j.y*sinalpha*pc0->r;

			// entrée sur cercle 1
			presult->e.x = pc1->o.x - i.x*cosalpha*pc1->r + j.x*sinalpha*pc1->r;
			presult->e.y = pc1->o.y - i.y*cosalpha*pc1->r + j.y*sinalpha*pc1->r;

		}
		// Le cercle 0 tourne dans le sens indirect, et donc le cercle 1 dans le sens direct.
		else
		{
			// sortie sur cercle 0
			presult->s.x = pc0->o.x + i.x*cosalpha*pc0->r + j.x*sinalpha*pc0->r;
			presult->s.y = pc0->o.y + i.y*cosalpha*pc0->r + j.y*sinalpha*pc0->r;

			// entrée sur cercle 1
			presult->e.x = pc1->o.x - i.x*cosalpha*pc1->r - j.x*sinalpha*pc1->r;
			presult->e.y = pc1->o.y - i.y*cosalpha*pc1->r - j.y*sinalpha*pc1->r;
		}

		// Ouvertures angulaires: beta0 et beta1
		// beta0: angle parcouru depuis p0 jusqu'au point de sortie situé sur le cercle 0.
		o0p0.x = pc0->p.x - pc0->o.x;
		o0p0.y = pc0->p.y - pc0->o.y;

		NVEC2 h;
		h.x =  o0o1.y * presult->pc0->s;
		h.y = -o0o1.x * presult->pc0->s;
		if (NVec2DotProduct(&h, &o0p0) < 0.0f ) 
		{
			presult->beta0 = NF32_2PI - acosf(NVec2DotProduct(&o0o1, &o0p0) / (pc0->r*lo0o1)) - presult->alpha;
		}
		else
		{
			presult->beta0 = acosf(NVec2DotProduct(&o0o1, &o0p0) / (pc0->r*lo0o1)) - presult->alpha;
		}
		if (presult->beta0 < 0.0f)
		{
			presult->beta0 += NF32_2PI;
		}

//beta1: angle parcouru depuis le point d'entrée situé sur le cercle 1 jusq'au point p1.
		o1p1.x = pc1->p.x - pc1->o.x;
		o1p1.y = pc1->p.y - pc1->o.y;
		o1o0.x = -o0o1.x;
		o1o0.y = -o0o1.y;
		h.x = -o1o0.y *  presult->pc1->s;
		h.y =  o1o0.x *  presult->pc1->s;
		if (NVec2DotProduct(&h, &o1p1) < 0.0f)
		{
			presult->beta1 = NF32_2PI - acosf(NVec2DotProduct(&o1p1, &o1o0) / (pc1->r*lo0o1)) - presult->alpha;
		}
		else
		{
			presult->beta1 = acosf(NVec2DotProduct(&o1p1, &o1o0) / (pc1->r*lo0o1)) - presult->alpha;
		}
		if (presult->beta1 < 0.0f)
		{
			presult->beta1 += NF32_2PI;
		}

	}

	// Calcul de se 
	presult->se.x	= presult->s.x - presult->e.x;
	presult->se.y	= presult->s.y - presult->e.y;
	presult->lse	= NVec2Normalize(&presult->se);

	// +------------------------------------------------------------------------------------+
	// |																					|
	// +---	2/4:Remplacement des arcs de cercles par des clothoides symetriques				|
	// |																					|
	// +------------------------------------------------------------------------------------+
	
	// Cercle 0
	NUT_2DLINE_XRESULT xres;
	Nf32					cf, sf, fa;

	NUT_2DLineXLine_VDir(&presult->pc0->p, &presult->pc0->u, &presult->s, &presult->se, &xres);
	presult->f0	= xres.I;
//	NErrorIf(xres.ParamCoordAB < 0.0f, NERROR_INCONSISTENT_VALUES); // F est censé se trouvé "devant" u0, c'est à dire dans la direction pointée par u0.
	presult->l0		= NABS(xres.ParamCoordAB); // comme 'u0' est unitaire, 'xres.ParamCoordAB' represente ici la longueur algébrique du vecteur [p0,f0].
	presult->clothoid_alpha0 = acosf(NVec2DotProduct(&presult->pc0->u, &presult->se));
	
	NFresnelIntegralsf32( sqrt(1.0f - presult->clothoid_alpha0 / NF32_PI) , &cf, &sf);							// |	
	fa = cf + sf / (tanf(presult->clothoid_alpha0 / 2.0f));												// |-- Ref. [2.4] page 61
	presult->clothoid_sigma0 = (NF32_PI / (presult->l0*presult->l0)) * fa*fa;							// |

	presult->clothoid_k0	= (sqrt( NF32_PI * (NF32_PI - presult->clothoid_alpha0) )/ presult->l0) * fa;	// |-- Ref. page 62
	Nf32 check_k0			= presult->clothoid_sigma0 * sqrt((NF32_PI - presult->clothoid_alpha0) / presult->clothoid_sigma0);


	// cercle 1
	NUT_2DLineXLine_VDir(&presult->pc1->p, &presult->pc1->u, &presult->e, &presult->se, &xres);
	presult->f1	= xres.I;
//	NErrorIf(xres.ParamCoordAB > 0.0f, NERROR_INCONSISTENT_VALUES); // F est censé se trouvé "derriere" u1, c'est à dire dans la direction opposée à celle pointée par u1.
	presult->l1		= NABS(xres.ParamCoordAB); // comme 'u1' est unitaire, 'xres.ParamCoordAB' represente ici la longueur algébrique du vecteur [p1,f1].
	presult->clothoid_alpha1 = acosf(NVec2DotProduct(&presult->pc1->u, &presult->se));
	
	NFresnelIntegralsf32(sqrt(1.0f - presult->clothoid_alpha1 / NF32_PI), &cf, &sf);								// |	
	fa = cf + sf / (tanf(presult->clothoid_alpha1 / 2.0f));												// |-- Ref. [2.4] page 61
	presult->clothoid_sigma1 = (NF32_PI / (presult->l1*presult->l1)) * fa*fa;							// |

	presult->clothoid_k1	= (sqrt(NF32_PI * (NF32_PI - presult->clothoid_alpha1)) / presult->l1) * fa;	// |-- Ref. page 62
	Nf32 check_k1			= presult->clothoid_sigma1 * sqrt((NF32_PI - presult->clothoid_alpha1) / presult->clothoid_sigma1);

	// +------------------------------------------------------------------------------------+
	// |																					|
	// +---	3/4:Insertion d'un arc de cercle entre les deux clothoides.						|
	// |																					|
	// +------------------------------------------------------------------------------------+
	// Calcul du rayon du "cercle inséré"
	//NErrorIf((presult->clothoid_alpha0 <= 0.0f) || (presult->clothoid_alpha0 >= NF32_PI_2), NERROR_INCONSISTENT_VALUES); // // Ref [2.2.5] page 63, "0 < alpha < PI/2"
	presult->clothoid_tetha0 = ((NF32_PI - presult->clothoid_alpha0) / 2.0f) *0.25f; // Ref [2.2.5] page 63 AVEC " 0.0f < clothoid_insertcoef < 1.0f " permettant de faire evoluer tetha0 dans l'intervalle ]0, (PI-alpha0)/2 [
	Nf32 rac1 = sqrt(2.0f*presult->clothoid_tetha0*NF32_PI);
	Nf32 rac2 = sqrt((2.0f*presult->clothoid_tetha0)/NF32_PI);
	Nf32 costetha = cosf(presult->clothoid_tetha0);
	Nf32 sintetha = sinf(presult->clothoid_tetha0);
	NFresnelIntegralsf32(rac2, &cf, &sf);
	presult->clothoid_r0 = presult->l0 / (rac1*cf-sintetha + ( rac1*sf + costetha )/tanf(presult->clothoid_alpha0/2.0f) );
	presult->clothoid_o0.x = presult->clothoid_r0 * (rac1 * cf - sintetha);
	presult->clothoid_o0.y = presult->clothoid_r0 * (rac1 * sf + costetha);

	presult->clothoid_sigma0i = 1.0f / (2.0f*presult->clothoid_tetha0*presult->clothoid_r0*presult->clothoid_r0);
}

void NLPATH_DrawBuildCircle(NLPATH_BUILDCIRCLE *pc)
{
	NUT_DRAW_ELLIPSE	ellipse;
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	
	// Le cercle 
	ellipse.SliceNb = 32;
	ellipse.Center.z = 0.0f;
	ellipse.Center.y = pc->o.y;
	ellipse.Center.x = pc->o.x;

	ellipse.Extents.x = ellipse.Extents.y = pc->r;
	if (pc->s >= 0)
		NSetColorf(&ellipse.Color, NCOLOR_PRESET3F_GREEN, 1);
	else
		NSetColorf(&ellipse.Color, NCOLOR_PRESET3F_RED, 1);
	NUT_Draw_Ellipse(&ellipse);
}

void NLPATH_DrawCirclePair(NLPATH_BUILDCIRCLE_PAIR *ppair)
{
	NVEC2				op;
	NVEC2				oes;
	NVEC3				va, vb;
	NVEC2				xtd = { 0.5f,0.5f };
	NCOLOR				col = { NCOLOR_PRESET3F_BLUE_AQUAMARINE,1 };
	NUT_DRAW_ELLIPSE	ellipse;

	va.z = 0.0f;
	vb.z = 0.0f;


	// Tracage des points f0 et f1
	va.x = ppair->f0.x;
	va.y = ppair->f0.y;
	NUT_Draw_Cross(&va, &xtd, &col);
	va.x = ppair->f1.x;
	va.y = ppair->f1.y;
	NUT_Draw_Cross(&va, &xtd, &col);



	// tracage "smax0" ( pour verification )
	
	// sans cercle inséré
	//Nf32 smax	= sqrt((NF32_PI - ppair->clothoid_alpha0) / ppair->clothoid_sigma0);
	//Nf32 t = sqrt(NF32_PI / ppair->clothoid_sigma0);
	
	// avec cercle inséré
	Nf32 smax	= 2.0f*ppair->clothoid_tetha0*ppair->clothoid_r0;
	Nf32 t = sqrt(NF32_PI / ppair->clothoid_sigma0i);

	// check de la courbure par deux moyens de calcul... ( peut-être songer à passer en 64 bits ??? )
	//Nf32 check_curva = smax * ppair->clothoid_sigma0i;
	//Nf32 check_curvb = 1.0f/ ppair->clothoid_r0;

	Nf32 cf, sf;
	Nf32 x, y;

	NVEC2 ia, ja;
	ia = ppair->pc0->u;
	ja.x = -ia.y * ppair->pc0->s;
	ja.y =  ia.x * ppair->pc0->s;

	NVEC2 ib, jb;
	ib = ppair->se;
	jb.x = ib.y * ppair->pc0->s;
	jb.y = -ib.x  * ppair->pc0->s;


	Nf32 anglebase = (NF32_PI - ppair->clothoid_alpha0 - 2.0f*ppair->clothoid_tetha0);
	Nf32 anglearc;
	NCOLOR col2 = { NCOLOR_PRESET3F_ORANGE_GOLD,1.0f };
	// Traçage arc de cercle inséré:
	for (Nu32 ii = 0; ii <= 10; ii++)
	{
		anglearc = ( (anglebase * (Nf32)ii )/ 10.0f ) - NF32_PI_2 + ppair->clothoid_tetha0;
		x = ppair->clothoid_o0.x + cos(anglearc)*ppair->clothoid_r0;
		y = ppair->clothoid_o0.y + sin(anglearc)*ppair->clothoid_r0;
		va.x = ppair->pc0->p.x + ia.x * x + ja.x * y;
		va.y = ppair->pc0->p.y + ia.y * x + ja.y * y;
		NUT_Draw_Cross(&va, &xtd, &col2);
	}

	// Tracage cercle inséré
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	NSetColorf(&ellipse.Color, NCOLOR_PRESET3F_RED, 1);
	ellipse.Extents.x = ellipse.Extents.y = NABS(ppair->clothoid_r0);
	ellipse.SliceNb = 32;
	ellipse.Center.x = ppair->pc0->p.x + ia.x * ppair->clothoid_o0.x + ja.x * ppair->clothoid_o0.y;
	ellipse.Center.y = ppair->pc0->p.y + ia.y * ppair->clothoid_o0.x + ja.y * ppair->clothoid_o0.y;
	ellipse.Center.z = 0.0f;
	NUT_Draw_Ellipse(&ellipse);


	for (Nu32 ii = 0; ii <= 10; ii++)
	{
		NFresnelIntegralsf32( (smax*(Nf32)ii/10.0f ) / t, &cf, &sf);
		x = t * cf;
		y = t * sf;

		// clothoide #0.1 
		va.x = ppair->pc0->p.x + ia.x * x + ja.x * y;
		va.y = ppair->pc0->p.y + ia.y * x + ja.y * y;
		NUT_Draw_Cross(&va, &xtd, &col);

		// clothoide #0.2 
		va.x = ppair->s.x + ib.x * x + jb.x * y;
		va.y = ppair->s.y + ib.y * x + jb.y * y;
		NUT_Draw_Cross(&va, &xtd, &col);
	}
	// tracage "smax1" ( pour verification )
	smax = sqrt((NF32_PI - ppair->clothoid_alpha1) / ppair->clothoid_sigma1);
	t = sqrt(NF32_PI / ppair->clothoid_sigma1);
	ia.x = -ppair->pc1->u.x;
	ia.y = -ppair->pc1->u.y;
	ja.x = ia.y * ppair->pc1->s;
	ja.y = -ia.x * ppair->pc1->s;

	ib.x = -ppair->se.x;
	ib.y = -ppair->se.y;
	jb.x =  -ib.y  * ppair->pc1->s;
	jb.y =   ib.x  * ppair->pc1->s;
	


	for (Nu32 ii = 0; ii <= 10; ii++)
	{
		NFresnelIntegralsf32((smax*(Nf32)ii / 10.0f) / t, &cf, &sf);
		x = t * cf;
		y = t * sf;

		va.x = ppair->pc1->p.x + ia.x * x + ja.x * y;
		va.y = ppair->pc1->p.y + ia.y * x + ja.y * y;
		NUT_Draw_Cross(&va, &xtd, &col);

		va.x = ppair->e.x + ib.x * x + jb.x * y;
		va.y = ppair->e.y + ib.y * x + jb.y * y;
		NUT_Draw_Cross(&va, &xtd, &col);
	}




	// tracage beta0 et beta1 pour verification
	NCOLOR	colb = { NCOLOR_PRESET3F_RED_AMARANTH,1.0f };
	va.z = 0.0f;
	va.x = ppair->pc0->o.x;
	va.y = ppair->pc0->o.y;
	NVEC2 v2d;
	v2d.x = ppair->pc0->p.x - ppair->pc0->o.x;
	v2d.y = ppair->pc0->p.y - ppair->pc0->o.y;
	if( ISFLAG_ON(ppair->Flags,FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA0_PI) )
		NUT_Draw_Angle(&va, &v2d, ppair->pc0->s*ppair->beta0, &colb);
	else
		NUT_Draw_Angle(&va, &v2d, ppair->pc0->s*ppair->beta0, &col);

	va.z = 0.0f;
	va.x = ppair->pc1->o.x;
	va.y = ppair->pc1->o.y;
	v2d.x = ppair->e.x - ppair->pc1->o.x;
	v2d.y = ppair->e.y - ppair->pc1->o.y;
	if (ISFLAG_ON(ppair->Flags, FLAG_NLPATH_BUILDCIRCLE_PAIR_BETA1_PI))
		NUT_Draw_Angle(&va, &v2d, ppair->pc1->s*ppair->beta1, &colb);
	else
		NUT_Draw_Angle(&va, &v2d, ppair->pc1->s*ppair->beta1, &col);

	// Tracage Angle Alpha
	va.z = 0.0f;
	va.x = ppair->pc0->o.x;
	va.y = ppair->pc0->o.y;
	v2d.x = ppair->pc1->o.x - ppair->pc0->o.x;
	v2d.y = ppair->pc1->o.y - ppair->pc0->o.y;
	NSetColorf(&col, NCOLOR_PRESET3F_GREEN_EMERALD, 1);
	NUT_Draw_Angle(&va, &v2d, ppair->alpha, &col);
	NUT_Draw_Angle(&va, &v2d, -ppair->alpha, &col);

	va.z = 0.0f;
	va.x = ppair->pc1->o.x;
	va.y = ppair->pc1->o.y;
	v2d.x = ppair->pc0->o.x - ppair->pc1->o.x;
	v2d.y = ppair->pc0->o.y - ppair->pc1->o.y;
	NSetColorf(&col, NCOLOR_PRESET3F_GREEN_EMERALD, 1);
	NUT_Draw_Angle(&va, &v2d, ppair->alpha, &col);
	NUT_Draw_Angle(&va, &v2d, -ppair->alpha, &col);


	// Tracage des points p0 et p1
	va.z = 0.0f;
	va.x = ppair->pc0->p.x;
	va.y = ppair->pc0->p.y;
	NUT_Draw_Quad(&va, &xtd, &col);
	va.x = ppair->pc1->p.x;
	va.y = ppair->pc1->p.y;
	NUT_Draw_Quad(&va, &xtd, &col);

	// Tracage des points d'entrée et de sortie
	va.z = 0.0f;
	va.x = ppair->s.x;
	va.y = ppair->s.y;
	NUT_Draw_Cross(&va, &xtd, &col);
	va.x = ppair->e.x;
	va.y = ppair->e.y;
	NUT_Draw_Cross(&va, &xtd, &col);

	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	FLAG_ON(ellipse.Flags, FLAG_NUT_DRAW_ELLIPSE_OPEN);
	NSetColorf(&ellipse.Color, NCOLOR_PRESET3F_WHITE, 1);
	ellipse.Extents.x = ellipse.Extents.y = ppair->pc0->r; // = ppair->pc1->r
	ellipse.SliceNb = 32;
	ellipse.Center.z = 0.0f;

	// Arc 0:
	// --------------------------------
	ellipse.Center.y = ppair->pc0->o.y;
	ellipse.Center.x = ppair->pc0->o.x;

	// L'arc 0 commence en p0 ( le point de départ) ...
	op.x = ppair->pc0->p.x - ppair->pc0->o.x;
	op.y = ppair->pc0->p.y - ppair->pc0->o.y;
	NVec2Normalize(&op);
	// ... et se termine en s ( la sortie )
	oes.x = ppair->s.x - ppair->pc0->o.x;
	oes.y = ppair->s.y - ppair->pc0->o.y;
	NVec2Normalize(&oes);
	if (ppair->pc0->s == 1)
	{
		ellipse.FromAngle	= NRADtoDEG(atan2f(op.y, op.x));
		ellipse.ToAngle		= NRADtoDEG(atan2f(oes.y, oes.x));
	}
	else
	{
		ellipse.ToAngle		= NRADtoDEG(atan2f(op.y, op.x));
		ellipse.FromAngle	= NRADtoDEG(atan2f(oes.y, oes.x));
	}
	NUT_Draw_Ellipse(&ellipse);

	// Arc 1:
	// --------------------------------
	ellipse.Center.y = ppair->pc1->o.y;
	ellipse.Center.x = ppair->pc1->o.x;

	// L'arc 1 commence en e ( l'entrée ) ...
	oes.x = ppair->e.x - ppair->pc1->o.x;
	oes.y = ppair->e.y - ppair->pc1->o.y;
	NVec2Normalize(&oes);
	// et se termine en p1 ( le point d'arrivée )
	op.x = ppair->pc1->p.x - ppair->pc1->o.x;
	op.y = ppair->pc1->p.y - ppair->pc1->o.y;
	NVec2Normalize(&op);


	if (ppair->pc1->s == 1)
	{
		ellipse.FromAngle = NRADtoDEG(atan2f(oes.y, oes.x));
		ellipse.ToAngle = NRADtoDEG(atan2f(op.y, op.x));
	}
	else
	{
		ellipse.ToAngle = NRADtoDEG(atan2f(oes.y, oes.x));
		ellipse.FromAngle = NRADtoDEG(atan2f(op.y, op.x));
	}
	NUT_Draw_Ellipse(&ellipse);

	// Tangente 
	va.x = ppair->s.x;
	va.y = ppair->s.y;
	va.z = 0.0f;
	vb.x = ppair->e.x;
	vb.y = ppair->e.y;
	vb.z = 0.0f;
	NUT_Draw_Segment(&va, &vb, &ellipse.Color);

}
*/