#include "../../../../../N/NMemory.h"
#include "../../../../../N/NMath.h"
#include "../../../../../N/Maths/NVec2f32.h"
#include "../../../../../N/Maths/NVecLimits.h"
#include "../../../../../N/Containers/NArray.h"
#include "../../../../../N/Utilities/NUT_X.h"
#include "../../../../../N/NErrorHandling.h"

#include "NLPathBuilderWayPoints.h"
#include "NLPathWayPoint.h"
#include "NLPathSection.h"

#include "../../NLPath.h"

#ifdef _NEDITOR
#include "../../../../../N/NEditor.h"
#include "../../../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../../../N/Utilities/Draw/NUT_DrawPencil.h"
#endif

NLPATH_BUILDER_WAYPOINTS::NLPATH_BUILDER_WAYPOINTS()
{
	m_flags = 0;

	NSetupArray(&m_wayPointsArray, DEFAULT_NLPATH_WAYPOINTS_ARRAYCAPACITY, sizeof(NLPATH_WAYPOINT));
	NSetupArray(&m_sectionsArray, (DEFAULT_NLPATH_WAYPOINTS_ARRAYCAPACITY - 1), sizeof(NLPATH_SECTION)); // le nombre de section est égal au nombre de waypoints moins 1 !

	m_heuristic = NLPATH_heuristic_shortest; // par défaut la fonction heuristic selectionnera le chemin le plus court.
	m_arcLengthThreshold = DEFAULT_NLPATH_WAYPOINTS_ARC_LENGTH_THRESHOLD;
#ifdef _NEDITOR
	// Partie additionnelle si _NEDITOR est actif
	// Ses données membres sont utilisées pour dessiner le path et ses attributs à l'écran.
	m_TgtSize			= DEFAULT_NL_WAYPOINT_TGT_SIZE;
	m_WayPointExtend.x	= DEFAULT_NL_WAYPOINT_XEXTEND;
	m_WayPointExtend.y	= DEFAULT_NL_WAYPOINT_YEXTEND;


	NSetColorf(&m_HandleColor, DEFAULT_NL_HANDLE_COLOR);
	NSetColorf(&m_WayPointPColor, DEFAULT_NL_WAYPOINT_P_COLOR);
	NSetColorf(&m_WayPointUColor, DEFAULT_NL_WAYPOINT_U_COLOR);
#endif

}
/*
NLPATH_BUILDER_WAYPOINTS::NLPATH_BUILDER_WAYPOINTS(NLPATH *plinkedpath) :NLPATH_BUILDER(plinkedpath)
{
	NLPATH_BUILDER_WAYPOINTS();
}
*/

NLPATH_BUILDER_WAYPOINTS::~NLPATH_BUILDER_WAYPOINTS()
{
	NClearArray(&m_sectionsArray, NULL);
	NClearArray(&m_wayPointsArray, NULL);
}

void NLPATH_BUILDER_WAYPOINTS::build(NLPATH *ppath)
{

	Nu32			 i;
	Nu32			 loops = 0;
	Nu32			 waypoint_ajusted = 0;
	NLPATH_WAYPOINT  wp;
	NLPATH_WAYPOINT* pwp0;
	NLPATH_WAYPOINT* pwp1;
	NLPATH_SECTION*  psection;
	NLPATH_POINT*	 pkp;

	NARRAY			used_waypoints_array;
	NVEC2f32		relp;	// position relative d'un waypoint par rapport à l'origine du chemin exprimée dans la base monde ( = p - o )

	// V0: Copie des waypoints de m_wayPointsArray dans un buffer de travail:
	//
	//		NSetupArray(&used_waypoints_array, 0, sizeof(NLPATH_WAYPOINT));
	//		NCopyArray(&used_waypoints_array, &m_wayPointsArray);

	// V1: Transformation dans une nouvelle base des waypoints de m_wayPointsArray dans un buffer de travail
	//	   Cette nouvelle base (directe) est construite à partir du premier Waypoint de telle sorte que son vecteur u soit le vecteur J
	//	   Cette base correspond à la matrice associée au chemin.
	//	   ( rappel: le vecteur u est unitaire )
	//
	// Preparation du buffer
	NSetupArray(&used_waypoints_array, m_wayPointsArray.Size, sizeof(NLPATH_WAYPOINT));

	// La matrice
	pwp0 = (NLPATH_WAYPOINT*)m_wayPointsArray.pFirst;

	ppath->m_matrix.XAxis.x =  pwp0->u.x;
	ppath->m_matrix.XAxis.y =  pwp0->u.y;
	ppath->m_matrix.XAxis.z =  0.0f;
	ppath->m_matrix.XAxis.w =  0.0f;

	ppath->m_matrix.YAxis.x	=-pwp0->u.y;
	ppath->m_matrix.YAxis.y = pwp0->u.x;
	ppath->m_matrix.YAxis.z = 0.0f;
	ppath->m_matrix.YAxis.w = 0.0f;

	ppath->m_matrix.ZAxis.x = 0.0f;
	ppath->m_matrix.ZAxis.y = 0.0f;
	ppath->m_matrix.ZAxis.z = 1.0f;
	ppath->m_matrix.ZAxis.w = 0.0f;

	ppath->m_matrix.Origin.x= pwp0->p.x;
	ppath->m_matrix.Origin.y= pwp0->p.y;
	ppath->m_matrix.Origin.z= 0.0f;
	ppath->m_matrix.Origin.w= 1.0f;

	// ... et on exprime le premier waypoint dans la base formée par ...lui-même  ( = origine ) et la matrice associée au path
	wp.icr_A = pwp0->icr_A;
	wp.icr_C = pwp0->icr_C;
	wp.rr	 = pwp0->rr;
	wp.p.x	 = 0.0f;
	wp.p.y   = 0.0f;
	wp.u.x	 = 1.0f;
	wp.u.y	 = 0.0f;
	NArrayPushBack(&used_waypoints_array, (NBYTE*)&wp);
	pwp0 ++;

	// ... Ensuite on exprime tous les autres waypoints dans la base formée par le premier Waypoint ( = origine ) et la matrice associée au path
	for (i = 1; i < m_wayPointsArray.Size; i++, pwp0++)
	{
		wp.icr_A = pwp0->icr_A;
		wp.icr_C = pwp0->icr_C;
		wp.rr	 = pwp0->rr;

		relp.x = pwp0->p.x - ppath->m_matrix.Origin.x;
		relp.y = pwp0->p.y - ppath->m_matrix.Origin.y;

		wp.p.x = relp.x * ppath->m_matrix.XAxis.x + relp.y * ppath->m_matrix.XAxis.y; // équivalant à:  NVec2DotProduct(&relp, (NVEC2f32*)&ppath->m_matrix.XAxis);
		wp.p.y = relp.x * ppath->m_matrix.YAxis.x + relp.y * ppath->m_matrix.YAxis.y; // équivalant à:  NVec2DotProduct(&relp, (NVEC2f32*)&ppath->m_matrix.YAxis);

		wp.u.x = pwp0->u.x * ppath->m_matrix.XAxis.x + pwp0->u.y * ppath->m_matrix.XAxis.y; // équivalant à:  NVec2DotProduct(&pwp0->u, (NVEC2f32*)&ppath->m_matrix.XAxis);
		wp.u.y = pwp0->u.x * ppath->m_matrix.YAxis.x + pwp0->u.y * ppath->m_matrix.YAxis.y; // équivalant à:  NVec2DotProduct(&pwp0->u, (NVEC2f32*)&ppath->m_matrix.YAxis);
		NArrayPushBack(&used_waypoints_array, (NBYTE*)&wp);
	}

	// 	Erase et (re)Construction des sections à partir des Waypoints
	NEraseArray(&m_sectionsArray, NULL);
	ppath->m_geometry.erase();
	//	NEraseArray(&m_pathPointsArray, NULL);
	//	NEraseArray(&m_primitivesArray, NULL);

	pwp0 = (NLPATH_WAYPOINT*)used_waypoints_array.pFirst;
	pwp1 = pwp0 + 1;
	// insertion du premier keyPoint ( les suivants seront insérés 1 par 1 par les methodes d'extraction de  primitives )
	pkp = (NLPATH_POINT*)NArrayAllocBack(&ppath->m_geometry.m_pathPointsArray);
	pkp->p = pwp0->p;		// position dans le plan XY
	pkp->u = pwp0->u;		// direction en p.
	pkp->s = 0.0f;			// abscisse curviligne nulle ( ... c'est le point de depart )
	pkp->k = 0.0f;			// courbure à ce point.  TODO: ajouter la possibilité d'avoir une courbure non-nulle c'est à dire départ non-arrêté et que la premiere primitive soit un arc de cercle ?!?

	for (i = 1; i < used_waypoints_array.Size; i++, pwp0 = pwp1, pwp1++)
	{
		psection = (NLPATH_SECTION*)NArrayAllocBack(&m_sectionsArray);
		psection->computeAndExtract(pwp0, pwp1, m_heuristic, &ppath->m_geometry, m_arcLengthThreshold);
	}

	// extrait la longueur totale du chemin ( TODO:à intégrer dans computeAndExtract )
	ppath->m_geometry.m_ds = ((NLPATH_POINT*)NGetLastArrayPtr(&ppath->m_geometry.m_pathPointsArray))->s;
	NClearArray(&used_waypoints_array, NULL);

	// On precise que maintenant la geometry n'est pas (plus) désynchronisée par rapport aux way points de construction.
	//FLAG_OFF(m_flags, FLAG_NLPATH_GEOMETRY_OUT_OF_SYNC);
}

#ifdef _NEDITOR
void NLPATH_BUILDER_WAYPOINTS::draw()
{
	NUT_DRAW_ARROWS arrows;
	Nmem0(&arrows, NUT_DRAW_ARROWS);
	arrows.BArrowSize = CONSTANT_NL_WAYPOINT_TANGENT_ARROWHEAD_SIZE_RATIO;
	arrows.AArrowSize = CONSTANT_NL_WAYPOINT_TANGENT_ARROWTAIL_SIZE_RATIO;
	NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FASTFORWARD);

	arrows.Color = m_WayPointUColor;

	NVEC3f32	v0, v1;
	v0.z = 0.0f;
	v1.z = 0.0f;

	NLPATH_WAYPOINT* pwp = (NLPATH_WAYPOINT*)m_wayPointsArray.pFirst;
	for (Nu32 i = 0; i < m_wayPointsArray.Size; i++, pwp++)
	{
		v0.x = pwp->p.x;
		v0.y = pwp->p.y;

		v1.x = pwp->p.x + pwp->u.x * (2.0f + pwp->rr) * (m_TgtSize / 3.0f);
		v1.y = pwp->p.y + pwp->u.y * (2.0f + pwp->rr) * (m_TgtSize / 3.0f);

		NUT_Draw_Quad(&v0, &m_WayPointExtend, &m_WayPointPColor);
		NUT_Draw_Arrows(&v0, &v1, &arrows);

		// On update Apres le draw, pour le prochain tour ! pour permettre, au premier tour de tracer avec 'BITS_NUT_DRAW_ARROWS_SHAPE_FASTFORWARD' 
		if (i == m_wayPointsArray.Size - 2)
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_HALFSQUARE);
		else
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
	}
}
void NLPATH_BUILDER_WAYPOINTS::draw(NL2DOCS* pocs)
{
}

void NLPATH_BUILDER_WAYPOINTS::drawSection(const Nu32 section_index, const NCOLOR* pcolcircles, const NCOLOR* pcolpair, const NCOLOR* pcolalpha, const NCOLOR* pcolbeta)
{
	NErrorIf(section_index >= m_sectionsArray.Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	NLPATH_SECTION* ps = &((NLPATH_SECTION*)m_sectionsArray.pFirst)[section_index];
	NLPATH_WAYPOINT* pwp0 = &((NLPATH_WAYPOINT*)m_wayPointsArray.pFirst)[section_index];
	NLPATH_WAYPOINT* pwp1 = pwp0 + 1;

	if (!FLAGS_TEST(m_flags, 0x1F, 0))
		ps->drawBuildingPairs(pwp0, pwp1, pcolcircles, pcolpair, pcolalpha, pcolbeta, m_flags);

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_BUILDER_WAYPOINTS_DRAW_BUILD_CIRCLES))
		ps->drawBuildingCircles(pwp0, pwp1, pcolcircles);
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Retourne l'index de la section dont est issue la primitive de path ayant pour index 'primitive_index'.
 *			
 *
 *	@param	pprim est un pointeur sur la primitive dont on veut connaitre la section d'origine
 *  @return l'index ( 32 bits) de la section dont est issue la primitive
 *			Si aucune section n'est trouvée la fonction retourne NVOID
 *			Le test correct à réaliser sur la valeur de retour de fonction est : if ( returned value == NVOID ) ERROR
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLPATH_BUILDER_WAYPOINTS::getSectionIndex(const Nu32 primitive_index)
{
	NLPATH_SECTION* psec = (NLPATH_SECTION*)m_sectionsArray.pFirst;
	Nu32 pcount = 0;
	for (Nu32 i = 0; i < m_sectionsArray.Size; i++, psec++)
	{
		pcount += GET_NLPATH_SECTION_PRIMITIVES_NB(psec->m_uid);
		if (primitive_index < pcount)
			return i;
	}
	// En mode DEBUG on crash ! Mais il ne s'agit pas d'une erreur, juste que l'utilisateur a passé en paramètre un index de primitive
	// trop grand et qui ne correspond pas à une section de ce builder...
	// Il peut s'agir d'une erreur de cohérence de données, c'est à dire que l'index de la primitive est celui d'une primitive appartenant à un chemin
	// différent de celui qui est/serait construit à partir de l'array de sections de ce builder ...
	NErrorIf(1, NERROR_INCONSISTENT_REQUEST); 

	return NVOID;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Retourne l'index de la section dont est issue la primitive de path ayant pour index 'primitive_index'.
 *			Si aucune section n'est trouvée la fonction retourne NVOID
 *
 *	@param	pprim est un pointeur sur la primitive dont on veut connaitre la section d'origine
 *  @return valeur 32 bits composée de l'index ( 16 bits) de la section dont est issue la primitive et la partie dans la quelle se trouve cette primitive ( 0,1 ou 2 ) codée egalement sur 16 bits
 *			[ valeur retournée = NMAKELONG( index section, partie ) ]
 *			Si aucune section n'est trouvée la fonction retourne NVOID
 *			Le test correct à réaliser sur la valeur de retour de fonction est : if ( NHIWORD( returned value ) <= 2 ) OK else ERROR
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLPATH_BUILDER_WAYPOINTS::getSectionIndexXtd(const Nu32 primitive_index)
{
	NLPATH_SECTION* psec = (NLPATH_SECTION*)m_sectionsArray.pFirst;
	Nu32 pcount = 0;
	for (Nu32 i = 0; i < m_sectionsArray.Size; i++, psec++)
	{
		pcount += GET_NLPATH_SECTION_PRIMITIVES_NB_A(psec->m_uid);
		if (primitive_index < pcount)
			return NMAKELONG(i,0); // index de section combiné avec n° de partie ( ici 0 )

		pcount += GET_NLPATH_SECTION_PRIMITIVES_NB_B(psec->m_uid);
		if (primitive_index < pcount)
			return NMAKELONG(i,1); // index de section combiné avec n° de partie ( ici 1 )

		pcount += GET_NLPATH_SECTION_PRIMITIVES_NB_C(psec->m_uid);
		if (primitive_index < pcount)
			return NMAKELONG(i,2); // index de section combiné avec n° de partie ( ici 2 )
	}
	// En mode DEBUG on crash ! Mais il ne s'agit pas d'une erreur, juste que l'utilisateur a passé en paramètre un index de primitive
	// trop grand et qui ne correspond pas à une section de ce builder...
	// Il peut s'agir d'une erreur de cohérence de données, c'est à dire que l'index de la primitive est celui d'une primitive appartenant à un chemin
	// différent de celui qui est/serait construit à partir de l'array de sections de ce builder ...
	NErrorIf(1, NERROR_INCONSISTENT_REQUEST);

	return NVOID;
}

#endif



NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::pushBackWayPoint(const NLPATH_WAYPOINT* pwp)
{
	NErrorIf(!NVec2IsUnit(&pwp->u), NERROR_UNAUTHORIZED_VALUE); // u  doit être un vecteur unitaire.

	return (NLPATH_WAYPOINT*)NArrayPushBack(&m_wayPointsArray, (NBYTE*)pwp);
}

NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::pushBackWayPoint(const NVEC2f32* ppos)
{

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u.x = 1.0f;
	wp.u.y = 0.0f;
	wp.rr =		DEFAULT_NLPATH_WAYPOINT_RR;
	wp.icr_A =	DEFAULT_NLPATH_WAYPOINT_ICR_FROM;
	wp.icr_C =	DEFAULT_NLPATH_WAYPOINT_ICR_TO;

	return (NLPATH_WAYPOINT*)NArrayPushBack(&m_wayPointsArray, (NBYTE*)&wp);
}
NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::pushBackWayPoint(const NVEC2f32* ppos, const NVEC2f32* pdirection)
{
	NErrorIf(!NVec2IsUnit(pdirection), NERROR_UNAUTHORIZED_VALUE); // pdirection  doit être un vecteur unitaire.

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u = *pdirection;
	wp.rr =		DEFAULT_NLPATH_WAYPOINT_RR;
	wp.icr_A =	DEFAULT_NLPATH_WAYPOINT_ICR_FROM;
	wp.icr_C =	DEFAULT_NLPATH_WAYPOINT_ICR_TO;

	return (NLPATH_WAYPOINT*)NArrayPushBack(&m_wayPointsArray, (NBYTE*)&wp);
}

NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::pushBackWayPoint(const NVEC2f32* ppos, const NVEC2f32* pdirection, const Nf32 radiusratio, const Nf32 icr_A, const Nf32 icr_C)
{
	NErrorIf(!NVec2IsUnit(pdirection), NERROR_UNAUTHORIZED_VALUE); // pdirection  doit être un vecteur unitaire.

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u = *pdirection;
	wp.rr = radiusratio;
	wp.icr_A = icr_A;
	wp.icr_C = icr_C;

	return (NLPATH_WAYPOINT*)NArrayPushBack(&m_wayPointsArray, (NBYTE*)&wp);
}

NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::insertWayPoint(const Nu32 insertbefore_index, const NLPATH_WAYPOINT* pwp)
{
	NErrorIf(!NVec2IsUnit(&pwp->u), NERROR_UNAUTHORIZED_VALUE); // u  doit être un vecteur unitaire.
	return (NLPATH_WAYPOINT*)NInsertArrayElement(&m_wayPointsArray, insertbefore_index, (NBYTE*)pwp);
}
NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32* ppos)
{

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u.x = 1.0f;
	wp.u.y = 0.0f;
	wp.rr =		DEFAULT_NLPATH_WAYPOINT_RR;
	wp.icr_A =	DEFAULT_NLPATH_WAYPOINT_ICR_FROM;
	wp.icr_C =	DEFAULT_NLPATH_WAYPOINT_ICR_TO;

	return (NLPATH_WAYPOINT*)NInsertArrayElement(&m_wayPointsArray, insertbefore_index, (NBYTE*)&wp);
}

NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32* ppos, const NVEC2f32* pdirection)
{
	NErrorIf(!NVec2IsUnit(pdirection), NERROR_UNAUTHORIZED_VALUE); // pdirection  doit être un vecteur unitaire.

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u = *pdirection;
	wp.rr =		DEFAULT_NLPATH_WAYPOINT_RR;
	wp.icr_A =	DEFAULT_NLPATH_WAYPOINT_ICR_FROM;
	wp.icr_C =	DEFAULT_NLPATH_WAYPOINT_ICR_TO;

	return (NLPATH_WAYPOINT*)NInsertArrayElement(&m_wayPointsArray, insertbefore_index, (NBYTE*)&wp);
}
NLPATH_WAYPOINT* NLPATH_BUILDER_WAYPOINTS::insertWayPoint(const Nu32 insertbefore_index, const NVEC2f32* ppos, const NVEC2f32* pdirection, const Nf32 radiusratio, const Nf32 icr_A, const Nf32 icr_C)
{
	NErrorIf(!NVec2IsUnit(pdirection), NERROR_UNAUTHORIZED_VALUE); // pdirection  doit être un vecteur unitaire.

	NLPATH_WAYPOINT	wp;
	wp.p = *ppos;
	wp.u = *pdirection;
	wp.rr = radiusratio;
	wp.icr_A = icr_A;
	wp.icr_C = icr_C;

	return (NLPATH_WAYPOINT*)NInsertArrayElement(&m_wayPointsArray, insertbefore_index, (NBYTE*)&wp);
}

void NLPATH_BUILDER_WAYPOINTS::eraseWaypoint(const Nu32 index)
{
	NEraseArrayElement(&m_wayPointsArray, index, NULL);
}
void NLPATH_BUILDER_WAYPOINTS::eraseWaypoint(NLPATH_WAYPOINT* pwp)
{
	NEraseArrayElementPtr(&m_wayPointsArray, (NBYTE*)pwp, NULL);
}



Nu32 NLPATH_BUILDER_WAYPOINTS::write(FILE* pfile)
{
	// 1) écriture Version
	Nu32	version_u32 = VERSION_NLPATH_BUILDER_WAYPOINTS_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) écriture Header
	NLPATH_BUILDER_WAYPOINTS_HEADER	header;
	header.m_flags							= m_flags;
	NGetArrayBounds(&header.m_wayPointsArrayBounds,&m_wayPointsArray);
	header.m_arcLengthThreshold				= m_arcLengthThreshold;
	header.m_heuristicId					= (m_heuristic == NLPATH_heuristic_shortest) ? 0:1; 
	header.m_availableNu8					= 0;
	header.m_availableNu16					= 0;
	if (fwrite(&header, sizeof(NLPATH_BUILDER_WAYPOINTS_HEADER), 1, pfile) != 1)
		return 0;

	// 3) écriture Array of WayPoints à la main et en une fois
	if (m_wayPointsArray.Size)
	{
		if (fwrite(m_wayPointsArray.pFirst, m_wayPointsArray.ElementSize, m_wayPointsArray.Size, pfile) != m_wayPointsArray.Size)
			return 0;
	}
	return 1;
}

Nu32 NLPATH_BUILDER_WAYPOINTS::read(FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLPATH_BUILDER_WAYPOINTS_HEADER	header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLPATH_BUILDER_WAYPOINTS_HEADER):
		if (fread(&header, sizeof(NLPATH_BUILDER_WAYPOINTS_HEADER), 1, pfile) != 1)
			return 0;

		m_flags						=	header.m_flags;
		m_arcLengthThreshold		=	header.m_arcLengthThreshold;
		m_heuristic					=	(header.m_heuristicId==0)? NLPATH_heuristic_shortest : NLPATH_heuristic_lessCurvy;
		//header.m_availableNu8;
		//header.m_availableNu16;
		// Recherche de potentiels pbs sur l'array.
		// ('NIsArrayCorruptedOrInconsistent' se charge de faire un setup auto en cas de array full of ZEROS)
		if (NIsArrayCorruptedOrInconsistent(&m_wayPointsArray, &header.m_wayPointsArrayBounds,NTRUE))
			return 0;
		NResizeArray(&m_wayPointsArray, header.m_wayPointsArrayBounds.Size, NULL, NULL);
		if (m_wayPointsArray.Size)
		{
			if (fread(m_wayPointsArray.pFirst, m_wayPointsArray.ElementSize, m_wayPointsArray.Size, pfile) != m_wayPointsArray.Size)
				return 0;
		}
		return 1;

	default:
		break;
	}
	return 0;
}


