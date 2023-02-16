#pragma once
#include "../../../../../N/NType.h"
#include "../../../../../N/NFlags.h"

#include "../../../../../N/Maths/NVec2f32.h"
#include "../../../../../N/NErrorHandling.h"

#define FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN		BIT_0
#define FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST					BIT_1
#define FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST					BIT_2
#define MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES				(BIT_0|BIT_1|BIT_2)

//#define FLAG_NLPATH_BUILDER_PLG_VERTEX_RELATIVE_ARC_RADIUS		BIT_3



typedef struct NLPATH_BUILDER_PLG_VERTEX NLPATH_BUILDER_PLG_VERTEX;
struct NLPATH_BUILDER_PLG_VERTEX
{
	#define DEFAULT_NLPATH_BUILDER_PLG_VERTEX_L				0.0f
	#define DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ICR			0.0f

	NLPATH_BUILDER_PLG_VERTEX() :flags(0),tl(DEFAULT_NLPATH_BUILDER_PLG_VERTEX_L), l(DEFAULT_NLPATH_BUILDER_PLG_VERTEX_L), icr(DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ICR),lA(0.0f),lB(0.0f) { vA.x = vA.y = vB.x = vB.y = 0.0f; }

	/*
	inline void compute(NVEC2f32* p0, NVEC2f32* p1)
	{
		NErrorIf(!p0, NERROR_NULL_POINTER);
		NErrorIf(!p1, NERROR_NULL_POINTER);
		NVEC2f32 A, B;
		A.x = p0->x - p.x;
		A.y = p0->y - p.y;
		B.x = p1->x - p.x;
		B.y = p1->y - p.y;
		NVec2Normalize(&A);
		NVec2Normalize(&B);
		bisector.x = A.x + B.x;
		bisector.y = A.y + B.y;
		NVec2Normalize(&bisector);
	}
	*/

	Nf32 getAvailableLengthA();
	Nf32 getAvailableLengthB();

	void computeBasics();
	inline void defaultInsertedBuildArcParams() { tl = l = DEFAULT_NLPATH_BUILDER_PLG_VERTEX_L; }
	inline void defaultIcr() { icr = DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ICR; }
	void updateBisector();
	void updateInsertedBuildArcParams();
		//inline void setBisector(const NVEC2f32* pbisector) { bisector = *pbisector; }
	void setPosition(const NVEC2f32* ppos);


	Nu32			flags;
	
	// A) Données dépendant seulement de la position des vertex.
	NVEC2f32		p;				// position (x,y) du vertex
	NVEC2f32		vA;				// vecteur unitaire [Vn,Vn-1] est le vecteur null [0,0] si le vertex est le premier de la chaine
	NVEC2f32		vB;				// vecteur unitaire [Vn,Vn+1] est le vecteur null [0,0] si le vertex est le dernier de la chaine
	Nf32			lA;				// longueur du segment [Vn,Vn-1]
	Nf32			lB;				// longueur du segment [Vn,Vn+1]
	NVEC2f32		bisector;		// vecteur unitaire directeur de la bissectrice 'intérieure' ( pointe vers le centre du cercle de construction )
	
	// B) Données relatives à l'arc de cercle de construction inséré et dont le centre se situe sur la bissectrice issue de ce vertex.
	Nf32			tl;				// distance "target" ou souhaitée pour l. on a toujours l <= tl. En effet l peut se retrouver plus petite que tl à cause de l'évolution 
									// des contraintes environnantes.
	Nf32			l;				// distance mesurée 'le long' et 'dans la direction' de chaque vecteur vA et vB et mesurée depuis la position p de ce vertex.
									// cette distance permet de placer, sur A et sur B les points de début et de fin d'arc de construction.
	Nf32			icr;			// InsertedCircleRatio ( !!! il ne s'agit pas ici de l'arc de cercle de construction mais de l'arc de cercle qui sera inséré entre les deux clothoides )
									// "0.0f <= icr <= 1.0", coefficient permettant d'insérer un arc de cercle entre les les deux clothoides symétriques à sommet.
									// utilisé par le building arc de cercle lors de l'extraction des primitives.

									
	// Param
	//Nf32			rl;				// ratio
	//Nf32			rr;				// RadiusRatio: "0.0f < radiusRatio <= 1"  , permet de moduler la taille de l'arc de cercle  de construction. 
									// Le rayon des 4 cercles d'une section est modulé par le radiusRatio du 2eme Waypoint de la section ( psection->pwaypoint_to ).
};
