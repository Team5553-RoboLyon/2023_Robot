#include "NLPathBuilderPolygonalVertex.h"

#define EPSILON_NLPATH_BUILDER_PLG_BISECTOR_LENGTH		1e-4f			// = 1/10000 �me de m�tre soit 1/10 de mm

Nf32 NLPATH_BUILDER_PLG_VERTEX::getAvailableLengthA()
{
	// Seul un vertex ins�r� dans une chaine peut �tre trait�...
	NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_REQUEST); 
	if (FLAGS_TEST(flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN))
	{
		NLPATH_BUILDER_PLG_VERTEX* pv0 = this - 1;
		NErrorIf(pv0->lB != lA, NERROR_INCONSISTENT_VALUES);
		NErrorIf((lA - pv0->l) < 0.0f, NERROR_INCONSISTENT_VALUES);
		return lA - pv0->l ;
	}
	else
	{
		// On passe ici donc le vertex est soit le premier soit le dernier ( soit les deux )
		// Si cela n'est pas le cas ... AUTO-CRASH !!!
		NErrorIf( ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST) && ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST), NERROR_INCONSISTENT_REQUEST);
		
		// En situation normale, la longueur A disponible pour un vertex FIRST ou LAST est ZERO
		return 0.0f;
	}
}

Nf32 NLPATH_BUILDER_PLG_VERTEX::getAvailableLengthB()
{
	// Seul un vertex ins�r� dans une chaine peut �tre trait�...
	NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_REQUEST);
	// On traite d'abord le cas ou le Vertex n'est ni le premier, ni le dernier.
	// Il y a donc au moins 1 vertex avant et au moins 1 vertex apr�s 'this'.
	if (FLAGS_TEST(flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN))
	{
		NLPATH_BUILDER_PLG_VERTEX* pv1 = this + 1;
		NErrorIf(pv1->lA != lB, NERROR_INCONSISTENT_VALUES);		// la longueur lA de [vertex+1] doit bien sur �tre egale � la longueur lB de [vertex]
		NErrorIf((lB - pv1->l) < 0.0f, NERROR_INCONSISTENT_VALUES); // la longueur lB de [vertex] est bien s�r suffisante pour "accueillir" la longueur l
		
		return lB - pv1->l;	// la longueur Disponible sur lB correspond � la longueur lB moins la longueur d�j� occup�e par l						
	}
	else
	{
		// On passe ici donc le vertex est soit le premier soit le dernier ( soit les deux )
		// Si cela n'est pas le cas ... AUTO-CRASH !!!
		NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST) && ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST), NERROR_INCONSISTENT_REQUEST);

		// En situation normale, la longueur B disponible pour un vertex FIRST ou LAST est ZERO
		return 0.0f;
	}
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	calcule et met � jour les valeurs suivantes:
 *			\b vA		vecteur unitaire [Vn,Vn-1]
 *			\b vB		vecteur unitaire [Vn,Vn+1]
 *			\b lA		longueur du segment [Vn,Vn-1]
 *			\b lB		longueur du segment [Vn,Vn+1]
 *			\b bisector	bissectrice de l'angle int�rieur [Vn-1,Vn]^[Vn,Vn+1]
 *			Il est important de noter que cette mise � jour s'�tend sur le vertex pr�c�dent et le vertex suivant si ils existent.
 */
 // ------------------------------------------------------------------------------------------
void NLPATH_BUILDER_PLG_VERTEX::computeBasics()
{
	NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_REQUEST); // seul un vertex ins�r� dans une chaine peut �tre calcul�...

	NLPATH_BUILDER_PLG_VERTEX *pv0, *pv1;

	// Premier cas: le vertex est "..._INSIDE_A_CHAIN" seulement ( c'est � dire ni FIRST ni LAST )
	if (FLAGS_TEST(flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN))
	{
		pv0 = this - 1;
		pv1 = this + 1;

		// Calcul des vecteur A et B partant du vertex et pointant respectivement vers le vertex precedent et le vertex suivant:
		//NVEC2f32 A, B;
		vA.x = pv0->p.x - p.x;
		vA.y = pv0->p.y - p.y;
		vB.x = pv1->p.x - p.x;
		vB.y = pv1->p.y - p.y;

		// On v�rifie qu'aucun des 2 vecteurs n'est null. Ce qui signifierait que 2 vertex sont confondus.
		// Si cela arrive, on initialise tout � ZERO.
		//		si A est null (la = 0 ) le vertex est confondu avec le precedent.
		//		si B est null (lb = 0 ) le vertex est confondu avec le suivant.
		lA = NVec2Length(&vA);
		lB = NVec2Length(&vB);
		if (lA)
		{
			// normalisation de vA
			vA.x /= lA;
			vA.y /= lA;
		}
		// else ... lA == 0 DONC vA.x = 0, vA.y = 0
		pv0->lB = lA;
		pv0->vB.x = -vA.x;
		pv0->vB.y = -vA.y;

		if (lB)
		{
			// normalisation de vA
			vB.x /= lB;
			vB.y /= lB;
		}
		// else ... lB == 0 DONC vB.x = 0, vB.y = 0
		pv1->lA = lB;
		pv1->vA.x = -vB.x;
		pv1->vA.y = -vB.y;

		updateBisector();
		pv0->updateBisector();
		pv1->updateBisector();
	}
	// second cas: le vertex est FIRST ( et donc INSIDE_A_CHAIN �galement )
	else if (ISFLAG_ON(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST))
	{
		// ... un vertex FIRST n'a pas de vertex avant lui, donc:
		vA.x = 0.0f;
		vA.y = 0.0f;
		lA   = 0.0f;

		// ... un vertex FIRST peu egalement etre LAST (ce qui signifie qu'il est unique ! )
		if (ISFLAG_ON(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST))
		{
			vB.x = 0.0f;
			vB.y = 0.0f;
			lB   = 0.0f;
		}
		// ... un vertex FIRST qui n'est pas �galement LAST possede donc un vertex valide apr�s lui ! 
		else
		{
			pv1 = this + 1;
			vB.x = pv1->p.x - p.x;
			vB.y = pv1->p.y - p.y;
			lB = NVec2Length(&vB);
			if (lB)
			{
				vB.x /= lB;
				vB.y /= lB;
			}

			// Rappel: pv(n+1)->vA = -pv(n)->vB
			pv1->lA = lB;
			pv1->vA.x = -vB.x;
			pv1->vA.y = -vB.y;
			pv1->updateBisector();
		}
		
		// un vertex First n'a pas de bisectrice associ�e ( ce qui est normal car lA = 0 ! )
		// ( on pourrait appeler la m�thode updateBisector() qui reconnaitrait ce cas, mais autant le faire direct ... )
		bisector.x = 0.0f;
		bisector.y = 0.0f;
	}
	// troisi�me cas cas: le vertex est LAST ( et donc INSIDE_A_CHAIN �galement )
	// dans ce cas nous sommes egalement sur qu'il n'est pas FIRST ! ( sinon le else if juste audessus aurait �t� valide et on ne passerait pas ici ! )
	else
	{
		NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST), NERROR_INCONSISTENT_FLAGS);

		// Comme nous sommes sur que pv n'est pas seul, et comme pv est LAST
		// alors il existe un vertex valide avant pv !
		pv0 = this - 1;
		vA.x = pv0->p.x - p.x;
		vA.y = pv0->p.y - p.y;
		lA = NVec2Length(&vA);
		if (lA)
		{
			vA.x /= lA;
			vA.y /= lA;
		}
		// Rappel: pv(n-1)->vB = -pv(n)->vA
		pv0->lB		=  lA;
		pv0->vB.x	= -vA.x;
		pv0->vB.y	= -vA.y;
		pv0->updateBisector();

		// Rappel: pv(LAST)->vB = 0 
		vB.x	= 0.0f;
		vB.y	= 0.0f;
		lB		= 0.0f;

		// Un vertex Last n'a pas de bisectrice associ�e ( ce qui est normal car lB = 0 ! )
		// ( on pourrait appeler la m�thode updateBisector() qui reconnaitrait ce cas, mais autant le faire direct ... )
		bisector.x = 0.0f;
		bisector.y = 0.0f;
	}
}

void NLPATH_BUILDER_PLG_VERTEX::updateBisector()
{
	// Ce test permet d'identifier les vertex FIRST (lA = 0.0f) LAST (lB = 0.0f ) 
	// ... et les vertex qui auraient potentiellement un lA et/ou lB null(s).
	if ((!lA) || (!lB))
	{
		// ... pas de bisectrice valide !
		bisector.x = 0.0f;
		bisector.y = 0.0f;
	}
	else // lA et lB sont tous les deux non null
	{
		// Calcul du vecteur directeur de la bissectrice
		// et de sa longueur avant normalisation
		bisector.x = vA.x + vB.x;
		bisector.y = vA.y + vB.y;
		Nf32 lbisct = NVec2Length(&bisector);

		// si cette longueur est inf�rieure � un certain seuil alors on consid�re que A et B sont colin�aires et de sens oppos�s ( A + B = 0 ... ou presque ! )
		if (lbisct < EPSILON_NLPATH_BUILDER_PLG_BISECTOR_LENGTH)
		{
			bisector.x = 0.0f;
			bisector.y = 0.0f;
		}
		// sinon ... on calcule 
		else
		{
			bisector.x /= lbisct;
			bisector.y /= lbisct;
		}
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	calcule et met � jour la valeur \b l ( longueur permettant de placer le d�but et la fin de l'arc de construction associ� au vertex )
 *			Il est important de noter que cette mise � jour s'�tend sur le vertex pr�c�dent et le vertex suivant si ils existent.
 */
 // ------------------------------------------------------------------------------------------
void NLPATH_BUILDER_PLG_VERTEX::updateInsertedBuildArcParams()
{
	// 3 variables pour d�finir la longueur disponible:
	//		available_lA		longueur disponible � gauche de ce vertex ( ou avant ) 
	//		available_lA		longueur disponible � droite de ce vertex ( ou apr�s ) 		
	//		available			minimum de 	available_lA et available_lB
	Nf32 available_lA, available_lB, available;

	// Si un vertex est seul ( ins�r� dans une chaine, mais seul )
	// notes:
	// Dans le cas d'un vertex seul dans la chaine, ce dernier poess�de les 3 flags suivants:
	//						FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN			... il est dans la chaine
	//						FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST						... il est le premier
	//						FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST						... il est le dernier	
	if (FLAGS_TEST(flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES))
	{
		return;
	}
	// Sinon, si le vertex est le Premier ( et donc qu'il y a au moins un vertex apr�s lui )
	else if (ISFLAG_ON(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST))
	{
		// En d�placant (vertex) on modifie la longueur lA du segment vA de (vertex+1) ...
		// ... �quivalant � la longueur lB du segment vB de (vertex) 
		// Rappel:	vertex->vB = -(vertex+1)->vA 
		//			vertex->lB =  (vertex+1)->lA
		//
		
		// LE VERTEX ETANT LE PREMIER DE LA CHAINE SES PARAMS l et tl restent ZERO.
		// CEPENDANT UNE MODIFICATION SUR LE PREMIER VERTEX ENGENDRE UNE MODIFICATION DES PARAMS l et tl de VERTEX+1
		NLPATH_BUILDER_PLG_VERTEX* pv1 = this + 1; // ( Nous sommes s�r qu'il y a au moins un vertex apr�s 'this' car 'this' n'est pas flagg� LAST )

		// Comme vertex est FIRST, la totalit� de vertex->lB, soit la totalit� de (vertex+1)->lA est "available"
		// pour l'expansion de (vertex+1)->l

		// Pour mettre � jour (vertex+1)->l nous devons d'abord r�cup�rer les longueurs disponibles � droite et � gauche de (vertex+1)
		// l ne pourra s'�tendre que dans la longueur disponible la plus petite ( entre celle � droite et celle � gauche )

		// ! available_lA et available_lB sont ici ceux de pv1 !
		available_lA = lB; // ce qui equivaut � available_lA =  pv1->getAvailableLengthA() = pv1->lA 
		available_lB = pv1->getAvailableLengthB();
		available = NMIN(available_lA, available_lB);

		pv1->l = NMIN(pv1->tl, available);
	}
	// Sinon, si le vertex est le dernier ( et donc qu'il y a au moins un vertex avant lui )
	else if (ISFLAG_ON(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST))
	{
		// En d�placant le dernier vertex on modifie la longueur lB du segment vB de (vertex-1) �quivalant � la longueur lA du segment vA de (vertex). 
		// Rappel: vertex->vB = (vertex+1)->vA ET vertex->lB = (vertex+1)->lA
		//
		// Comme vertex est LAST, la totalit� de vertex->lA soit la totalit� de (vertex-1)->lB est available pour l'expansion de (vertex-1)->l !
		NLPATH_BUILDER_PLG_VERTEX* pv0 = this - 1;

		// ! available_lA et available_lB sont ici ceux de pv0 !
		available_lB = lA; // ce qui equivaut � available_lB =  pv0->getAvailableLengthB() = pv0->lB 
		available_lA = pv0->getAvailableLengthA();
		available = NMIN(available_lA, available_lB);

		pv0->l = NMIN(pv0->tl, available);
	}
	// Sinon, si le vertex est en milieu de chaine ( et donc qu'il y a au moins un vertex avant lui et un apr�s lui )
	else
	{
		// En d�placant un vertex de milieu de chaine, on modifie les vecteurs vA et vB de ce vertex
		// Et se faisant on modifie potentiellement les 'l' de (vertex-1) et (vertex+1)
		NLPATH_BUILDER_PLG_VERTEX* pv0 = this - 1;
		NLPATH_BUILDER_PLG_VERTEX* pv1 = this + 1;


		// vertex - 1
		available = pv0->getAvailableLengthA();
		Nf32 l0 = NMIN(pv0->tl, available); // on defini d'abord l0 comme la valeur minimum entre la valeur cible de l pour vertex-1 et l'available distance 'avant' pv0 
		Nf32 sum = tl + l0;
		Nf32 cla;							// 'cla' est le l calcul� du c�t� lA de vertex ( soit en prenant en compte vertex - 1 )
		if (sum > lA)
		{
			l0 *= (lA / sum);
			cla = lA - l0;
		}
		else
		{
			// ... l0 reste tel quel !
			cla = tl;
		}

		// vertex + 1
		available = pv1->getAvailableLengthB();
		Nf32 l1 = NMIN(pv1->tl, available);
		sum = tl + l1;
		Nf32 clb;							// 'clb' est le l calcul� du c�t� lB de vertex ( soit en prenant en compte vertex + 1 )
		if (sum > lB)
		{
			l1 *= (lB / sum);
			clb = lB - l1;
		}
		else
		{
			// ... l1 reste tel quel !
			clb = tl;
		}

		// vertex
		l = NMIN(cla, clb); // du coup l est le minimum des valeurs calcul�es avant et apr�s le vertex

		// pour finir:
		pv0->l = l0;
		pv1->l = l1;
	}
}

void NLPATH_BUILDER_PLG_VERTEX::setPosition(const NVEC2f32* ppos)
{
	// seul un vertex ins�r� dans une chaine peut �tre calcul�...
	NErrorIf(ISFLAG_OFF(flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_REQUEST); 

	p = *ppos;
	computeBasics();
	updateInsertedBuildArcParams();
}

