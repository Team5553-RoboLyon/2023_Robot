#include "../../../../../N/NMemory.h"
#include "../../../../../N/NMath.h"
#include "../../../../../N/Maths/NVec2f32.h"
#include "../../../../../N/Maths/NVecLimits.h"
#include "../../../../../N/Containers/NArray.h"
#include "../../../../../N/Utilities/NUT_X.h"
#include "../../../../../N/NErrorHandling.h"

#include "NLPathBuilderPolygonalVertex.h"
#include "NLPathBuilderPolygonalChain.h"

#include "../../NLPath.h"
#include "../NLPathBuildArc.h"
#include "../NLPathBuildSegment.h"

#ifdef _NEDITOR
#include "../../../../../N/NEditor.h"
#include "../../../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../../../N/Utilities/Draw/NUT_DrawPencil.h"
#endif
/*
#define NLPATH_BUILDER_POLYGONAL_CHAIN_setup	m_flags = 0;																													\
												NSetupArray(	&m_vertexArray, DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ARRAY_CAPACITY,		sizeof(NLPATH_BUILDER_PLG_VERTEX)	);	\
												#ifdef _NEDITOR																													\
												NSetColorf(&m_HandleColor, DEFAULT_NL_HANDLE_COLOR);																			\
												NSetColorf(&m_polygonalSegmentColor, DEFAULT_NL_PATH_BUILDER_PLG_SEGMENT_COLOR);												\
												NSetColorf(&m_vertexColor, DEFAULT_NL_PATH_BUILDER_PLG_VERTEX_COLOR);															\
												NSetColorf(&m_WarningColor, DEFAULT_NL_WARNING_COLOR);																			\
												#endif
*/
NLPATH_BUILDER_POLYGONAL_CHAIN::NLPATH_BUILDER_POLYGONAL_CHAIN()
{
	m_flags = 0;
	m_handleExtend.x	= DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_XEXTEND;
	m_handleExtend.y	= DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_YEXTEND;
	m_handleLength		= DEFAULT_NL_PATH_BUILDER_PLG_CHAIN_HANDLE_LENGTH;

	NSetupArray(	&m_vertexArray, DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ARRAY_CAPACITY,		sizeof(NLPATH_BUILDER_PLG_VERTEX)	);
	NSetupArray(	&m_buildingArcArray, DEFAULT_NLPATH_BUILDER_PLG_VERTEX_ARRAY_CAPACITY,  sizeof(NLPATH_BARC));
	// les arrays contenus dans m_geometry sont setup par le constructeur de NLPAT_GEOMETRY


	#ifdef _NEDITOR
	NSetColorf(&m_HandleColor,				DEFAULT_NL_PATH_BUILDER_PLG_HANDLE_COLOR );
	NSetColorf(&m_SegmentColor,				DEFAULT_NL_PATH_BUILDER_PLG_SEGMENT_COLOR);
	NSetColorf(&m_vertexColor,				DEFAULT_NL_PATH_BUILDER_PLG_VERTEX_COLOR);
	NSetColorf(&m_WarningColor,				DEFAULT_NL_WARNING_COLOR	);
	#endif
}
/*
NLPATH_BUILDER_POLYGONAL_CHAIN::NLPATH_BUILDER_POLYGONAL_CHAIN(NLPATH* plinkedpath):NLPATH_BUILDER(plinkedpath)
{
	NLPATH_BUILDER_POLYGONAL_CHAIN();
}
*/

NLPATH_BUILDER_POLYGONAL_CHAIN::~NLPATH_BUILDER_POLYGONAL_CHAIN()
{
	NClearArray(&m_vertexArray, NULL);
	NClearArray(&m_buildingArcArray, NULL);

	//Nmem0(this, sizeof(NLPATH_BUILDER_POLYGONAL_CHAIN));
}

void NLPATH_BUILDER_POLYGONAL_CHAIN::build(NLPATH *ppath)
{
	if (m_vertexArray.Size < 2)
		return;
	/*
	NVEC2f32				B;				// Point 'B' de d�but de l'arc
	NVEC2f32				BTgt;			// Tangente � l'arc en 'B'
	NVEC2f32				D;				// Point 'D' de fin de l'arc
	NVEC2f32				DTgt;			// Tangente � l'arc en 'D'
	Nf32					Sgn;			// Signe ou sens de parcour de l'arc
	Nf32					Icr;			// Coefficient d'insertion d'un arc de cercle entre les deux clothoides � sommet issues de l'arc de cercle de construction.
											// Icr =  0 signifie pas de clothoide, l'arc de  cercle ins�r� EST l'arc de cercle de construction.
											// Icr =  1 signifie paire de clothoides de longueur maximale et PAS d'ARC de CERCLE ins�r� entre les deux.
	//
	// -----------------------------		Les valeurs ci-dessous sont utilis�es uniquement Dans le cas o� Icr = 0. ( la primitive arc de cercle qui sera cr��e �tant donc identique � l'arc de cercle de construction )
	//
	Nf32					m_BBeta;		// Longueur angulaire de l'arc de cercle de construction
	Nf32					m_BRadius;		// Rayon du cercle de construction
	NVEC2f32				m_BCenter;		// Centre du cercle de construction
	*/


	NLPATH_BARC				barc;
	NUT_2DLINE_XRESULT		xres;
	NVEC2f32				u;
	Nf32					corde;
	Nu32					i;
	NVEC2f32				A, B;
	Nf32					length;
	NARRAY					used_vertex_array;

	NLPATH_BUILDER_PLG_VERTEX	*pv = (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst;
	NLPATH_BUILDER_PLG_VERTEX	v;

	//	   Transformation dans une nouvelle base des Vertex de m_vertexArray dans un buffer de travail
	//	   Cette nouvelle base (directe) est construite � partir du premier Waypoint de telle sorte que son vecteur u soit le vecteur I ( AxeX )
	//	   Cette base correspond � la matrice associ�e au chemin.
	//	   ( rappel: le vecteur u est unitaire )
	//
	// Preparation du buffer
	NSetupArray(&used_vertex_array, m_vertexArray.Size, sizeof(NLPATH_BUILDER_PLG_VERTEX));

	// La matrice
	ppath->m_matrix.XAxis.x =  pv->vB.x;
	ppath->m_matrix.XAxis.y =  pv->vB.y;
	ppath->m_matrix.XAxis.z =  0.0f;
	ppath->m_matrix.XAxis.w =  0.0f;

	ppath->m_matrix.YAxis.x =-pv->vB.y;
	ppath->m_matrix.YAxis.y = pv->vB.x;
	ppath->m_matrix.YAxis.z = 0.0f;
	ppath->m_matrix.YAxis.w = 0.0f;

	ppath->m_matrix.ZAxis.x = 0.0f;
	ppath->m_matrix.ZAxis.y = 0.0f;
	ppath->m_matrix.ZAxis.z = 1.0f;
	ppath->m_matrix.ZAxis.w = 0.0f;

	// L'origine
	ppath->m_matrix.Origin.x= pv->p.x;
	ppath->m_matrix.Origin.y= pv->p.y;
	ppath->m_matrix.Origin.z= 0.0f;
	ppath->m_matrix.Origin.w= 1.0f;

	// ... et on exprime le premier Vertex dans la base form�e par ...lui-m�me  ( = origine ) et la matrice associ�e au path
	// Construction pas � pas manuelle de CHAQUE param�tre membre pour rester CRISTAL CLEAR
	v.p.x = 0.0f;		// |__La position x,y du premier vertex est (0,0). normal il est confondu avec l'origine dans cette base.
	v.p.y = 0.0f;		// |

	v.icr = pv->icr;    // |
	v.tl  = pv->tl;     // |-Ces 3 param�tres ne d�pendent pas de la base. Ils sont donc rigoureusement identiques � ceux des vertex exprim�s
	v.l	  = pv->l;      // |  dans la base monde.

	v.vA.x= 0.0f;		// |_Vecteur unitaire [Vn,Vn-1] est le vecteur null [0,0] car le vertex est le premier de la chaine.
	v.vA.y= 0.0f;		// |		

	v.vB.x= 1.0f;		// |_Vecteur unitaire [Vn,Vn+1] est TOUJOURS le vecteur [1,0] dans cette base !
	v.vB.y= 0.0f;		// | car, la direction du premier vertex est toujours align�e sur l'axe X ( repr�sentant la direction de l'avant du robot )

	v.lA  = 0.0f;		// --Longueur du segment [Vn,Vn-1] est nulle.

	v.lB  = pv->lB;		// --Longueur du segment [Vn,Vn+1] est conserv�e dans le changement de base.

	v.bisector.x = 0.0f;// |_Vecteur unitaire directeur de la bissectrice 'int�rieure' est NULLE car le vertex est le premier de la chaine
	v.bisector.y = 0.0f;// |

	v.flags = pv->flags;// -- Bien �videment les flags sont conserv�s d'une base � l'autre
	NArrayPushBack(&used_vertex_array, (NBYTE*)&v);

	// ... ensuite on ins�re le second vertex. Dont le calcul des coordonn�es est plus simple que pour les suivants car toujours align� horizontalement avec le premier !
	// Cet alignement syst�matique est du au fait que l'axe x de la base est le vecteur [v0,v1] normalis�.
	v.p.x = pv->lB;		// |__La position x,y du second vertex est (0,v0->lB).
	v.p.y = 0.0f;		// |  ( pv pointe encore sur le premier vertex )

	pv++;

	v.icr = pv->icr;    // |
	v.tl  = pv->tl;     // |-Ces 3 param�tres ne d�pendent pas de la base. Ils sont donc rigoureusement identiques � ceux des vertex exprim�s
	v.l   = pv->l;      // |  dans la base monde.

	v.flags = pv->flags;// -- Bien �videment les flags sont conserv�s d'une base � l'autre

	// mise � ZERO de tous les autres params.
	v.vA.x = 0.0f;		// |_Vecteur unitaire [Vn,Vn-1].
	v.vA.y = 0.0f;		// |		
	v.vB.x = 0.0f;		// |_Vecteur unitaire [Vn,Vn+1] ... sera calcul� + tard
	v.vB.y = 0.0f;		// | 
	v.lA   = 0.0f;		// --Longueur du segment [Vn,Vn-1].
	v.lB   = 0.0f;		// --Longueur du segment [Vn,Vn+1] est conserv�e dans le changement de base.
	v.bisector.x = 0.0f;// |_Vecteur unitaire directeur de la bissectrice 'int�rieure' est NULLE car le vertex est le premier de la chaine
	v.bisector.y = 0.0f;// |

	NArrayPushBack(&used_vertex_array, (NBYTE*)&v);
	pv++;
	// ... Ensuite on exprime tous les autres waypoints dans la base form�e par le premier Waypoint ( = origine ) et la matrice associ�e au path
	NVEC2f32 relp;
	for (i = 2; i < m_vertexArray.Size; i++, pv++)
	{
		v.flags = pv->flags;

		v.icr	= pv->icr;    
		v.tl	= pv->tl;     
		v.l		= pv->l;      

		relp.x = pv->p.x - ppath->m_matrix.Origin.x;
		relp.y = pv->p.y - ppath->m_matrix.Origin.y;

		v.p.x = relp.x * ppath->m_matrix.XAxis.x + relp.y * ppath->m_matrix.XAxis.y; // �quivalant �:  NVec2DotProduct(&relp, (NVEC2f32*)&ppath->m_matrix.XAxis);
		v.p.y = relp.x * ppath->m_matrix.YAxis.x + relp.y * ppath->m_matrix.YAxis.y; // �quivalant �:  NVec2DotProduct(&relp, (NVEC2f32*)&ppath->m_matrix.YAxis);
		NArrayPushBack(&used_vertex_array, (NBYTE*)&v);
	}

	pv = (NLPATH_BUILDER_PLG_VERTEX*)used_vertex_array.pFirst + 1;
	for (i = 1; i < used_vertex_array.Size; i++, pv++)
	{
		pv->computeBasics();
	}



	NEraseArray(&m_buildingArcArray, NULL);
	ppath->m_geometry.erase();

	NLPATH_BUILDER_PLG_VERTEX* pv0 = (NLPATH_BUILDER_PLG_VERTEX*)used_vertex_array.pFirst;
	NLPATH_BUILDER_PLG_VERTEX* pv1 = pv0 + 1;
	NErrorIf(ISFLAG_OFF(pv0->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST), NERROR_INCONSISTENT_FLAGS);

	// insertion du premier keyPoint ( les suivants seront ins�r�s 1 par 1 par les methodes d'extraction de  primitives )
	NLPATH_POINT* pkp = (NLPATH_POINT*)NArrayAllocBack(&ppath->m_geometry.m_pathPointsArray);
	pkp->p = pv0->p;		// position dans le plan XY
	pkp->u.x = -pv1->vA.x;	// direction en p.
	pkp->u.y = -pv1->vA.y;	// direction en p.
	pkp->s = 0.0f;			// abscisse curviligne nulle ( ... c'est le point de depart )
	pkp->k = 0.0f;			// courbure � ce point.  TODO: ajouter la possibilit� d'avoir une courbure non-nulle c'est � dire d�part non-arr�t� et que la premiere primitive soit un arc de cercle ?!?

	for (i = 1; i < used_vertex_array.Size; i++, pv0 = pv1, pv1++)
	{
		NErrorIf(ISFLAG_ON(pv1->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST), NERROR_INCONSISTENT_FLAGS);

		//1) on commence par ins�rer un segment
		length = pv1->lA - (pv0->l + pv1->l);
		//NErrorIf(length < 0.0f, NERROR_NULL_VALUE);
		if (length > 0.0001f)
		{
			A.x = pv0->p.x + pv0->vB.x * pv0->l;
			A.y = pv0->p.y + pv0->vB.y * pv0->l;
			B.x = pv1->p.x + pv1->vA.x * pv1->l;
			B.y = pv1->p.y + pv1->vA.y * pv1->l;
			NL_ExtractSegmentPrimitives(&A, &B, 0, &ppath->m_geometry);
		}


		//2) et on ins�re un arc de construction
		if (ISFLAG_OFF(pv1->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST) && pv1->l)
		{
			barc.m_B.x		= pv1->p.x + pv1->vA.x * pv1->l;
			barc.m_B.y		= pv1->p.y + pv1->vA.y * pv1->l;
			barc.m_D.x		= pv1->p.x + pv1->vB.x * pv1->l;
			barc.m_D.y		= pv1->p.y + pv1->vB.y * pv1->l;
			barc.m_BTgt.x	= -pv1->vA.x;
			barc.m_BTgt.y	= -pv1->vA.y;
			barc.m_DTgt.x	= -pv1->vB.x;
			barc.m_DTgt.y	= -pv1->vB.y;
			barc.m_Sgn		= (Nf32)NSIGN(pv1->bisector.x * (-barc.m_BTgt.y) + pv1->bisector.y * barc.m_BTgt.x);
			barc.m_Icr		= pv1->icr;

			u.x = -pv1->vA.y;
			u.y =  pv1->vA.x;
			NUT_2DLineXLine_VDir(&barc.m_B, &u, &pv1->p, &pv1->bisector, &xres);
			barc.m_BCenter = xres.I;
			barc.m_BRadius = NABS(xres.ParamCoordAB); // car u est unitaire !
			// calcul de la longueur de la corde de l'arc:
			u.x = barc.m_B.x - barc.m_D.x;
			u.y = barc.m_B.y - barc.m_D.y;
			corde = NVec2Length(&u);
			// a partir de la longueur de la corde et du rayon du cercle on calcul l'angle couvert par l'arc:
			// https://fr.wikipedia.org/wiki/Segment_circulaire
			barc.m_BBeta = 2.0f * asinf(corde / (2.0f * barc.m_BRadius));
			NArrayPushBack(&m_buildingArcArray, (NBYTE*)&barc);

			// ... et on extrait les primitives ( arc de cercle et/ou clothoides )
			barc.extractPrimitives(&ppath->m_geometry);
		}
	}

	// extrait la longueur totale du chemin ( TODO:� int�grer dans computeAndExtract )
	ppath->m_geometry.m_ds = ((NLPATH_POINT*)NGetLastArrayPtr(&ppath->m_geometry.m_pathPointsArray))->s;

}

Nu32 NLPATH_BUILDER_POLYGONAL_CHAIN::read(FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER	header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER):
		if (fread(&header, sizeof(NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER), 1, pfile) != 1)
			return 0;

		m_flags = header.m_flags;
		//header.m_availableNu8;
		//header.m_availableNu16;
		if (NIsArrayCorruptedOrInconsistent(&m_vertexArray, &header.m_vertexArrayBounds,NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
			return 0;
		if (NIsArrayCorruptedOrInconsistent(&m_buildingArcArray, &header.m_buildingArcArrayBounds,NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
			return 0;

		NResizeArray(&m_vertexArray,		header.m_vertexArrayBounds.Size, NULL, NULL);
		NResizeArray(&m_buildingArcArray,	header.m_buildingArcArrayBounds.Size, NULL, NULL);
		
		if (m_vertexArray.Size)
		{
			if (fread(m_vertexArray.pFirst, m_vertexArray.ElementSize, m_vertexArray.Size, pfile) != m_vertexArray.Size)
				return 0;
		}
		if (m_buildingArcArray.Size)
		{
			if (fread(m_buildingArcArray.pFirst, m_buildingArcArray.ElementSize, m_buildingArcArray.Size, pfile) != m_buildingArcArray.Size)
				return 0;
		}
		return 1;

	default:
		break;
	}
	return 0;
}

Nu32 NLPATH_BUILDER_POLYGONAL_CHAIN::write(FILE* pfile)
{
	// 1) �criture Version
	Nu32	version_u32 = VERSION_NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) �criture Header
	NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER	header;
	header.m_flags					= m_flags;
	//header.m_vertexArraySize		= m_vertexArray.Size;
	//header.m_buildingArcArraySize	= m_buildingArcArray.Size;
	NGetArrayBounds(&header.m_vertexArrayBounds, &m_vertexArray);
	NGetArrayBounds(&header.m_buildingArcArrayBounds, &m_buildingArcArray);

	if (fwrite(&header, sizeof(NLPATH_BUILDER_POLYGONAL_CHAIN_HEADER), 1, pfile) != 1)
		return 0;

	// 3) �criture Array of m_vertexArray � la main et en une fois
	if (m_vertexArray.Size)
	{
		if (fwrite(m_vertexArray.pFirst, m_vertexArray.ElementSize, m_vertexArray.Size, pfile) != m_vertexArray.Size)
			return 0;
	}

	// 3) �criture Array of m_buildingArcArray � la main et en une fois
	if (m_buildingArcArray.Size)
	{
		if (fwrite(m_buildingArcArray.pFirst, m_buildingArcArray.ElementSize, m_buildingArcArray.Size, pfile) != m_buildingArcArray.Size)
			return 0;
	}
	return 1;

}




NLPATH_BUILDER_PLG_VERTEX* NLPATH_BUILDER_POLYGONAL_CHAIN::pushBackVertex(const NLPATH_BUILDER_PLG_VERTEX* pv)
{
	NErrorIf(!FLAGS_TEST(pv->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, 0), NERROR_NON_NULL_VALUE);
	NLPATH_BUILDER_PLG_VERTEX *pvc = (NLPATH_BUILDER_PLG_VERTEX*)NArrayPushBack(&m_vertexArray, (NBYTE*)pv);
	updatePolygonalChainVertexFlagsIn(pvc);
	// le vertex cr�� devient naturellement le dernier de la chaine. 
	// le vertex pr�c�dent ( l'ancien dernier ) n'est plus dernier et poss�de donc maintenant egalement une bissectrice
	// sauf si, bien s�r il est premier, ce qui revient � dire:  
	// si la chaine contient au moins 3 vertex on calcule la bissectrice del'avant dernier vertex, apr�s le pushback.
	pvc->defaultInsertedBuildArcParams();
	pvc->defaultIcr();
	pvc->computeBasics();

	if (m_vertexArray.Size > 1)
	{
		(pvc - 1)->computeBasics();
		(pvc - 1)->updateInsertedBuildArcParams();
	}
	return pvc;
}

NLPATH_BUILDER_PLG_VERTEX* NLPATH_BUILDER_POLYGONAL_CHAIN::pushBackVertex(const NVEC2f32 * ppos)
{

	NLPATH_BUILDER_PLG_VERTEX	v;
	v.p		= *ppos;
	// v.defaultInsertedBuildArcParams();	|_inutile ici car ces valeurs par d�faut sont d�j� affect�es � v par son constructeur.
	// v.defaultIcr();						|

	NLPATH_BUILDER_PLG_VERTEX* pvc = (NLPATH_BUILDER_PLG_VERTEX*)NArrayPushBack(&m_vertexArray, (NBYTE*)&v);
	updatePolygonalChainVertexFlagsIn(pvc);
	// le vertex cr�� devient naturellement le dernier de la chaine. 
	// le vertex pr�c�dent ( l'ancien dernier ) n'est plus dernier et poss�de donc maintenant egalement une bissectrice
	// sauf si, bien s�r il est premier, ce qui revient � dire:  
	// si la chaine contient au moins 3 vertex on calcule la bissectrice de l'avant dernier vertex, apr�s le pushback.
	pvc->computeBasics();
	if (m_vertexArray.Size > 1)
	{
		(pvc - 1)->computeBasics();
		(pvc - 1)->updateInsertedBuildArcParams();
	}
	return pvc;
}

NLPATH_BUILDER_PLG_VERTEX* NLPATH_BUILDER_POLYGONAL_CHAIN::insertVertex(const Nu32 insertbefore_index, const NLPATH_BUILDER_PLG_VERTEX* pv)
{
	NErrorIf(!FLAGS_TEST(pv->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, 0), NERROR_NON_NULL_VALUE);
	NLPATH_BUILDER_PLG_VERTEX* pvc = (NLPATH_BUILDER_PLG_VERTEX*)NInsertArrayElement(&m_vertexArray,insertbefore_index,(NBYTE*)pv);
	updatePolygonalChainVertexFlagsIn(pvc);
	pvc->computeBasics();
	pvc->updateInsertedBuildArcParams();

	return pvc;
}

NLPATH_BUILDER_PLG_VERTEX* NLPATH_BUILDER_POLYGONAL_CHAIN::insertVertex(const Nu32 insertbefore_index, const NVEC2f32* ppos)
{
	NLPATH_BUILDER_PLG_VERTEX	v;
	NErrorIf(!FLAGS_TEST(v.flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, 0), NERROR_NON_NULL_VALUE);
	v.p = *ppos;

	NLPATH_BUILDER_PLG_VERTEX* pvc = (NLPATH_BUILDER_PLG_VERTEX*)NInsertArrayElement(&m_vertexArray, insertbefore_index, (NBYTE*)&v);
	updatePolygonalChainVertexFlagsIn(pvc);
	pvc->computeBasics();
	pvc->updateInsertedBuildArcParams();

	return pvc;
}


/*
void NLPATH_BUILDER_POLYGONAL_CHAIN::setVertexPosition(NLPATH_BUILDER_PLG_VERTEX* pvb, const NVEC2f32* ppos)
{
	NErrorIf(!NIsValidArrayElementPtr(&m_vertexArray, (NBYTE*)pvb), NERROR_ARRAY_ADRESS_BEYOND_LIMITS);

	pvb->p = *ppos;

	if (m_vertexArray.Size > 2)
	{
		NLPATH_BUILDER_PLG_VERTEX *pva, *pvc, *pv;

		if (pvb > (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst)
		{
			pva = pvb - 1;
			if (pva > (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst)
			{
				pv = pva - 1;
				pva->compute(&pv->p, &pvb->p);
			}

			NLPATH_BUILDER_PLG_VERTEX* plast = (NLPATH_BUILDER_PLG_VERTEX*)NGetLastArrayPtr(&m_vertexArray);
			if (pvb < plast)
			{
				pvc = pvb + 1;
				pvb->compute(&pva->p, &pvc->p);

				if (pvc < plast)
				{
					pv = pvc + 1;
					NErrorIf(!NIsValidArrayElementPtr(&m_vertexArray, (NBYTE*)pv), NERROR_ARRAY_ADRESS_BEYOND_LIMITS);
					pvc->compute(&pvb->p, &pv->p);
				}
			}
		}
		else // ((NBYTE*)pvb == m_vertexArray.pFirst)
		{
			NErrorIf((NBYTE*)pvb!= m_vertexArray.pFirst, NERROR_INCONSISTENT_VALUES);
			pvc = pvb + 1;
			if (pvc > (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst)
			{
				pv = pvc + 1;
				NErrorIf(!NIsValidArrayElementPtr(&m_vertexArray, (NBYTE*)pv), NERROR_ARRAY_ADRESS_BEYOND_LIMITS);
				pvc->compute(&pvb->p, &pv->p);
			}
		}
	}
}
*/
void NLPATH_BUILDER_POLYGONAL_CHAIN::eraseVertex(const Nu32 index)
{

	// +----------------------------------------------------------------------------------------------
	// |
	// |	A) SUPRESSION DU VERTEX
	// |
	// Recup�ration d'un pointeur sur le Vertex � supprimer.
	NLPATH_BUILDER_PLG_VERTEX *pv = (NLPATH_BUILDER_PLG_VERTEX * )NGetArrayPtr(&m_vertexArray, index);
	// Mise � jour des Flags Vertex ( ..._IS_INSIDE_A_CHAIN, ..._IS_FIRST, ..._IS_LAST ) pour ce vertex qui va �tre supprim� et 
	// les vertex pr�c�dent et suivant si ils existent.
	updatePolygonalChainVertexFlagsOut(pv);
	// Suppression du Vertex.
	NEraseArrayElement(&m_vertexArray, index, NULL);
	// |
	// +----------------------------------------------------------------------------------------------


	// +----------------------------------------------------------------------------------------------
	// |
	// |	APRES SUPPRESSION:
	// |		pv0 pointera vers le vertex qui �tait situ� AVANT le vertex ( avant suppression )
	// |		pv1 pointera vers le vertex qui �tait situ� APRES le vertex ( avant suppression )
	// |
	NLPATH_BUILDER_PLG_VERTEX* pv0, * pv1;
	// |
	// +----------------------------------------------------------------------------------------------


	// +----------------------------------------------------------------------------------------------
	// |
	// |	B) MISE A JOUR DES VERTEX VOISINS
	// |
	// Si apr�s suprression du vertex, l'array est vide, alors il ne reste rien � faire.
	if (!m_vertexArray.Size)
	{
		return;
	}
	// Sinon, si il reste des vertex...
	else
	{
		// Il reste des vertex dans l'array. 
		// Comme on vient de supprimer un vertex, on va chercher � mettre � jour les donn�es du vertex situ� 'avant' et du vertex situ� 'apr�s'
		// le vertex supprim�. Si ils existent, ces deux vertex se retrouvent maintenant �tre cons�cutifs.
		// On va s'interresser � la valeur de 'index' par rapport � la nouvele taille de l'array.
		// Il y a 3 possibilit�s � envisager/traiter:
		//			index == m_vertexArray.Size
		//						m_vertexArray.Size est la nouvelle taille de l'array apr�s suppression du vertex 'index'
		//						Donc, si index est maintenant �gal � 'Size' cela revient � dire que le vertex supprim� �tait le dernier de l'array qui avait � ce moment l� une taille de Size+1.
		//						Comme l'array, apr�s suppression, contient encore des vertex, pv-1 est donc maintenant le dernier vertex de l'array.
		//						Et, bien sur, il n'y a pas de (vertex + 1) c'est � dire de vertex 'apr�s...'
		//
		//			index == 0
		//						Le vertex supprim� �tait le premier de l'array. L'array contient toujours des vertex ( donc au moins 1 vertex).
		//						Du coup, maintenant, pv pointe sur le nouveau premier vertex qu'il conviendra de flagger comme tel.
		//						Et, bien sur, il n'y a pas de (vertex - 1) c'est � dire de vertex 'avant...'
		//
		//			index < m_vertexArray.Size ET index != 0
		//						m_vertexArray.Size est la nouvelle taille de l'array apr�s suppression du vertex 'index'
		//						Le vertex supprim� avait donc un index < m_vertexArray.Size + 1 ( taille avant suppression )
		//						Apr�s suppression, index reste < Size ( la nouvelle taille de l'array ) 
		//						On en conclu que pv pointe maintenant sur un vertex situ� 'dans' l'array avec un vertex avant et un vertex apr�s.
		//

		Nf32 l0, l1, sum, available;
		Nf32 ref = 0.0f;

		if (index == m_vertexArray.Size)
		{
			// Avec le d�calage du � la suppression, 'pv' pointe maintenant sur un emplacement 'vide' de l'array
			// Le vertex ( son pointeur ) situ� avant le vertex suprim� et nomm� pv0 est maintenant le nouveau dernier vertex de l'array:
			// Le vertex ( son pointeur ) situ� apr�s le vertex suprim� et nomm� pv1 est bien s�r inexistant !
			pv0 = pv - 1;
			pv1 = NULL;
			// VERIF.MISE A JOUR DES FLAGS:	la m�thode 'updatePolygonalChainVertexFlagsOut' a normalement mis tous les flags � jour pour les vertex voisins du vertex qui a �t� supprim�.
			// On v�rifie que pv0 est bien flagg� comme dernier !
			NErrorIf( ISFLAG_OFF(pv0->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST), NERROR_INCONSISTENT_FLAGS);
			pv0->computeBasics();
			// En tant que dernier vertex pv0 ne peut engendr� ni bissectrice ni arc de cercle de construction...
			pv0->tl = 0.0f;
			pv0->l = 0.0f;

			pv0->updateInsertedBuildArcParams();
		}
		else if (index == 0)
		{
			// Avec le d�calage du � la suppression, 'pv' pointe maintenant sur le nouveau premier vertex de l'array
			// Le vertex ( son pointeur ) situ� avant le vertex suprim� et nomm� pv0 est est bien s�r inexistant !
			// Le vertex ( son pointeur ) situ� apr�s le vertex suprim� et nomm� pv1 est maintenant le nouveau premier vertex de l'array !
			pv0 = NULL;
			pv1 = pv;

			// VERIF.MISE A JOUR DES FLAGS:	la m�thode 'updatePolygonalChainVertexFlagsOut' a normalement mis tous les flags � jour pour les vertex voisins du vertex qui a �t� supprim�.
			// On v�rifie que pv1 est bien flagg� comme premier !
			NErrorIf(ISFLAG_OFF(pv1->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST), NERROR_INCONSISTENT_FLAGS);
			pv1->computeBasics();
			// En tant que premier vertex pv1 ne peut engendr� ni bissectrice ni arc de cercle de construction...
			pv1->tl = 0.0f;
			pv1->l	= 0.0f;

			// Potentiellement pv1 est �galement le dernier, c'est � dire qu'il est seul ! (m_vertexArray.Size == 1)
			// Ou ...
			// L'array contient au moins 2 vertex, voir plus.
			// ... cela signifie qu'il y a un vertex situ� APRES pv1 et ce dernier peut potentiellement �tre impact� ( son l ) par le changement de statut de pv1
			// ... qui vient de devenir nouveau PREMIER vertex. La distance le s�parant de pv1 n'a pas chang� certes, mais comme maintenant pv1 est FIRST (premier)
			// il ne peut plus avoir d'arc et donc son l est ZERO, du coup la totalit� de la longueur situ�e entre les deux vertex devient ENTIEREMENT disponible pour loger
			// le 'l' de ce vertex situ� APRES.
			//
			// Dans tous les cas, la m�thode updateInsertedBuildArcParams() fera le n�c�ssaire, pour pv1 et le vertex suivant si il existe.
			pv1->updateInsertedBuildArcParams();
		}
		else // index < m_vertexArray.Size ( car ni egal � 0 ni egal � size et bien sur PAS SUPERIEUR � size ! )
		{
			NErrorIf(index > m_vertexArray.Size, NERROR_INCONSISTENT_VALUES);
			

			// Avec le d�calage du � la suppression, 'pv' pointe maintenant sur le vertex qui �tait situ� apr�s le vertex venant d'�tre supprim�.
			// Le vertex ( son pointeur ) situ� apr�s le vertex surrpim� est nomm� pv1 :
			// Le vertex ( son pointeur ) situ� avant le vertex surrpim� est nomm� pv0, il est maintenant situ� juste avant pv1 ( et nous somme sur qu'il existe !)
			pv1 = pv;
			pv0 = pv1 - 1;

			// VERIF.MISE A JOUR DES FLAGS:	la m�thode 'updatePolygonalChainVertexFlagsOut' a normalement mis tous les flags � jour pour les vertex voisins du vertex qui a �t� supprim�.
			// Il n'y a aucune v�rifs. particuli�re � pouvoir mener dans ce cas.

			// Mise � jour des valeurs de base: ( pour prendre en compte le nouveau vecteur/segment [pv0,pv1] )
			pv0->computeBasics();
			// R�cup�ration de la longueur disponible "AVANT" pv0.
			// Rappel:  l est la longueur qui positionne � gauche et � droite du vertex le d�but et la fin de l'arc de construction.
			//			L'arc commence avant le vertex, � une distance l mesur�e le long du segment [vertex,vertex-1] et ...
			//			se termine apr�s le vertex, � une distance l mesur�e le long du segment [vertex,vertex+1].
			//
			available = pv0->getAvailableLengthA();
			l0 = NMIN(pv0->tl, available); // on defini d'abord l0 comme la valeur minimum entre la valeur cible de l pour pv0 et l'available distance 'avant' pv0 

			pv1->computeBasics();					
			available = pv1->getAvailableLengthB();
			l1 = NMIN(pv1->tl, available); // on defini d'abord l1 comme la valeur minimum entre la valeur cible de l pour pv1 et l'available distance 'apres' pv1 

			NErrorIf(pv0->lB != pv1->lA, NERROR_INCONSISTENT_VALUES);
			ref = pv1->lA;
			
			// sum repr�sente la longueur n�c�ssaire pour "loger" l0 (=pv0->l) et l1(=pv1->l) sur le segment [pv0,pv1] de longueur ref ( = pv0->lB = pv1->lA )
			sum = l0 + l1;
			// Si la longueur n�c�ssaire est sup�rieure � la longeur disponible alors on r�duit l0 et l1 proportionellement.
			if (sum > ref)
			{
				if (ref)
				{
					l0 *= ref / sum;
					l1 = ref - l0;
				}
				else
				{
					l0 = 0.0f;
					l1 = 0.0f;
				}
			}
			pv0->l = l0;
			pv1->l = l1;
		}

		/*
		Nf32 l0, l1, sum,available;
		Nf32 ref = 0.0f;
		// 'index' est maintenant l'index du vertex qui �tait apr�s celui qu'on vient de suprimer
		if (index < m_vertexArray.Size)
		{
			pv1 = pv;
			pv1->computeBasics(); // pour prendre en compte le nouveau vecteur/segment pv1->vA !!!!
			available = pv1->getAvailableLengthB();
			l1 = NMIN(pv1->tl, available); // on defini d'abord l1 comme la valeur minimum entre la valeur cible de l pour pv1 et l'available distance 'apres' pv1 
			ref = pv1->lA;
		}
		else
		{
			pv1 = NULL;
			l1	= 0.0f;
		}

		if (index > 0)
		{
			pv0 = pv1 - 1;
			pv0->computeBasics(); // pour prendre en compte le nouveau vecteur/segment pv0->vB !!!!
			available = pv0->getAvailableLengthA();
			l0 = NMIN(pv0->tl, available); // on defini d'abord l0 comme la valeur minimum entre la valeur cible de l pour pv0 et l'available distance 'avant' pv0 
			ref = pv0->lB;
		}
		else
		{
			pv0 = NULL;
			l0 = 0.0f;
		}
		
		sum = l0 + l1;
		if (sum > ref)
		{
			if (ref)
			{
				l0 *= ref / sum;
				l1  = ref - l0;
			}
			else
			{
				l0 = 0.0f;
				l1 = 0.0f;
			}
		}

		if (pv0)
			pv0->l = l0;
		if (pv1)
			pv1->l = l1;
		*/
	}
	// |
	// +----------------------------------------------------------------------------------------------
}

void NLPATH_BUILDER_POLYGONAL_CHAIN::draw()
{
	FLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_VERTEX);
	FLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_SEGMENT);
	FLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_HANDLE);

	Nu32 i;
	NLPATH_BUILDER_PLG_VERTEX	*pv0, *pv1;
	NVEC3f32					v0, v1;
	NUT_DRAW_ELLIPSE			ellipse;

	const Nf32					xtd = NVec2Length(&m_handleExtend);
	// Draw le contenu, pas les outils ...
	// Tracage des KeyPoints et de leur index
	//

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_VERTEX))
	{
		pv0 = (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst;
		for (i = 0; i < m_vertexArray.Size; i++, pv0++)
		{
			v0.x = pv0->p.x;
			v0.y = pv0->p.y;
			v0.z = 0.0f;

			if (FLAGS_TEST(pv0->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN))
				NUT_Draw_Cross(&v0, &m_handleExtend, &m_vertexColor);
			else
				NUT_Draw_Cross(&v0, &m_handleExtend, &m_WarningColor);

		}
	}
	if (ISFLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_SEGMENT))
	{
		pv0 = (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst;
		pv1 = pv0 + 1;
		for (i = 1; i < m_vertexArray.Size; i++, pv0 = pv1, pv1++)
		{
			v0.x = pv0->p.x;
			v0.y = pv0->p.y;
			v0.z = 0.0f;

			v1.x = pv1->p.x;
			v1.y = pv1->p.y;
			v1.z = 0.0f;

			NUT_Draw_Segment(&v0, &v1, &m_SegmentColor);
		}
	}

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_HANDLE))
	{
		pv0 = (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst;
		for (i = 1; i < m_vertexArray.Size; i++, pv0++)
		{
			if (FLAGS_TEST(pv0->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN))
			{
				v0.x = pv0->p.x;
				v0.y = pv0->p.y;
				v0.z = 0.0f;

				v1.x = pv0->p.x - pv0->bisector.x * m_handleLength;
				v1.y = pv0->p.y - pv0->bisector.y * m_handleLength;
				v1.z = 0.0f;

				NUT_Draw_Segment(&v0, &v1, &m_HandleColor);

				Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
				ellipse.Color		= m_HandleColor;
				ellipse.Extents.x	= xtd;
				ellipse.Extents.y	= xtd;
				ellipse.SliceNb		= 16;
				ellipse.Center.z	= 0.0f;
				ellipse.Center.x	= pv0->p.x - pv0->bisector.x * m_handleLength - pv0->bisector.y * xtd;
				ellipse.Center.y	= pv0->p.y - pv0->bisector.y * m_handleLength + pv0->bisector.x * xtd;
				NUT_Draw_Ellipse(&ellipse);

				Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
				ellipse.Color		= m_HandleColor;
				ellipse.Extents.x	= m_handleExtend.x;
				ellipse.Extents.y	= m_handleExtend.y;
				ellipse.SliceNb		= 16;
				ellipse.Center.z	= 0.0f;
				ellipse.Center.x	= pv0->p.x - pv0->bisector.x * m_handleLength + pv0->bisector.y * m_handleExtend.x;
				ellipse.Center.y	= pv0->p.y - pv0->bisector.y * m_handleLength - pv0->bisector.x * m_handleExtend.y;
				NUT_Draw_Ellipse(&ellipse);
			}
		}
	}
}

void NLPATH_BUILDER_POLYGONAL_CHAIN::draw(NL2DOCS* pocs)
{
}

// update des FLags � l'insertion d'un vertex dans l'array
void NLPATH_BUILDER_POLYGONAL_CHAIN::updatePolygonalChainVertexFlagsIn(NLPATH_BUILDER_PLG_VERTEX* pv)
{
	NErrorIf(!NIsValidArrayElementPtr(&m_vertexArray, (NBYTE*)pv), NERROR_ARRAY_PTR_BEYOND_LIMITS);

	// Un vertex ins�r� dans l'array est systematiquement flagg� "... _IS_INSIDE_A_CHAIN"
	FLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN);

	// Si pv est le premier vertex de la chaine ...
	if (pv == (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst)
	{
		// ... On le flag comme tel ! 
		FLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST);
		// ... Et on 'd�-flag' syst�matiquement le vertex suivant ( qui pouvait �tre le premier avant la cr�ation/insertion de pv) 
		// si il existe, c'est � dire si il y a + de 1 vertex dans la chaine ! 
		if (m_vertexArray.Size > 1)
			FLAG_OFF((pv + 1)->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST);
	}

	// Si pv est le dernier vertex de la chaine ...
	if (pv == (NLPATH_BUILDER_PLG_VERTEX*)NGetLastArrayPtr(&m_vertexArray))
	{
		// ... On le flag comme tel ! 
		FLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST);
		// ... Et on 'd�-flag' syst�matiquement le vertex pr�c�dent ( qui pouvait �tre le dernier avant la cr�ation/insertion de pv) 
		// si il existe, c'est � dire si il y a + de 1 vertex dans la chaine !
		if (m_vertexArray.Size > 1)
			FLAG_OFF((pv - 1)->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST);
	}

}
// update des FLags � la sortie d'un vertex de l'array
void NLPATH_BUILDER_POLYGONAL_CHAIN::updatePolygonalChainVertexFlagsOut(NLPATH_BUILDER_PLG_VERTEX* pv)
{
	NErrorIf(!NIsValidArrayElementPtr(&m_vertexArray, (NBYTE*)pv), NERROR_ARRAY_PTR_BEYOND_LIMITS);

	// Un vertex retir� de l'array est systematiquement D�-flagg� "... _IS_INSIDE_A_CHAIN"
	FLAG_OFF(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN);

	// Si pv est le premier vertex de la chaine ...
	if(ISFLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST))
	{
		NErrorIf(pv!= (NLPATH_BUILDER_PLG_VERTEX*)m_vertexArray.pFirst,NERROR_ARRAY_INCONSISTENT_VALUES);
		// ... On lui retire le flag "..._IS_FIRST" ! 
		FLAG_OFF(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST);
		// ... Et on 'flag' syst�matiquement le vertex suivant ( qui deviendra le premier apr�s le retrait de pv) 
		// si il existe, c'est � dire si il y a + de 1 vertex dans la chaine ! 
		if (m_vertexArray.Size > 1)
			FLAG_ON((pv + 1)->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_FIRST);
	}

	// Si pv est le dernier vertex de la chaine ...
	if (ISFLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST))
	{
		NErrorIf(pv != (NLPATH_BUILDER_PLG_VERTEX*)NGetLastArrayPtr(&m_vertexArray), NERROR_ARRAY_INCONSISTENT_VALUES);
		// ... On lui retire le flag "..._IS_LAST" ! 
		FLAG_OFF(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST);
		// ... Et on 'flag' syst�matiquement le vertex pr�c�dent ( qui deviendra le dernier apr�s le retrait de pv) 
		// si il existe, c'est � dire si il y a + de 1 vertex dans la chaine !
		if (m_vertexArray.Size > 1)
			FLAG_ON((pv - 1)->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_LAST);
	}
}
