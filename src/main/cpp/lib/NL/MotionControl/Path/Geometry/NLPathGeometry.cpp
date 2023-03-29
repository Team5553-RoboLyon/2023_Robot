#include "lib/N/NFlags.h"
#include "lib/NL/MotionControl/Path/Geometry/NLPathGeometry.h"

#ifdef _NEDITOR
#include "../../../../N/NEditor.h"
#include "../../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../../N/Utilities/Draw/NUT_DrawPencil.h"
#endif

NLPATH_GEOMETRY::NLPATH_GEOMETRY()
{
//	m_flags = 0;
	m_ds	= 0.0f;
	NSetupArray(&m_pathPointsArray, DEFAULT_NLPATH_PATHPOINTS_ARRAYCAPACITY, sizeof(NLPATH_POINT));
	NSetupArray(&m_primitivesArray, DEFAULT_NLPATH_PRIMITIVES_ARRAYCAPACITY, sizeof(NLPATH_PRIMITIVE));

#ifdef _NEDITOR
	m_KeyPointExtend.x		=		DEFAULT_NLPATH_GEOMETRY_PATHPOINT_XEXTEND;
	m_KeyPointExtend.y		=		DEFAULT_NLPATH_GEOMETRY_PATHPOINT_YEXTEND;
	m_ClothoidDrawAccuracy	=		DEFAULT_NLPATH_GEOMETRY_CLOTHOID_DRAW_ACCURACY;
	m_ArcDrawAccuracy		=		DEFAULT_NLPATH_GEOMETRY_ARC_DRAW_ACCURACY;

	NSetColorf(&m_SegmentColor,		DEFAULT_NLPATH_GEOMETRY_SEGMENT_COLOR);
	NSetColorf(&m_ClothoidColorA,	DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORA);
	NSetColorf(&m_ClothoidColorB,	DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORB);
	NSetColorf(&m_ArcColor,			DEFAULT_NLPATH_GEOMETRY_ARC_COLOR);
	NSetColorf(&m_KeyPointColor,	DEFAULT_NLPATH_GEOMETRY_PATHPOINT_COLOR);
	NSetColorf(&m_KeyPointIDColor,	DEFAULT_NLPATH_GEOMETRY_PATHPOINTID_COLOR);
	NSetColorf(&m_WarningColor,		DEFAULT_NLPATH_GEOMETRY_WARNING_COLOR);
#endif
}

NLPATH_GEOMETRY::NLPATH_GEOMETRY(const Nu32 pathpoints_capacity, const Nu32 primitives_capacity)
{
//	m_flags = 0;
	m_ds	= 0.0f;
	NSetupArray(&m_pathPointsArray, pathpoints_capacity, sizeof(NLPATH_POINT));
	NSetupArray(&m_primitivesArray, primitives_capacity, sizeof(NLPATH_PRIMITIVE));


#ifdef _NEDITOR
	m_KeyPointExtend.x		=		DEFAULT_NLPATH_GEOMETRY_PATHPOINT_XEXTEND;
	m_KeyPointExtend.y		=		DEFAULT_NLPATH_GEOMETRY_PATHPOINT_YEXTEND;
	m_ClothoidDrawAccuracy	=		DEFAULT_NLPATH_GEOMETRY_CLOTHOID_DRAW_ACCURACY;
	m_ArcDrawAccuracy		=		DEFAULT_NLPATH_GEOMETRY_ARC_DRAW_ACCURACY;

	NSetColorf(&m_SegmentColor,		DEFAULT_NLPATH_GEOMETRY_SEGMENT_COLOR);
	NSetColorf(&m_ClothoidColorA,	DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORA);
	NSetColorf(&m_ClothoidColorB,	DEFAULT_NLPATH_GEOMETRY_CLOTHOID_COLORB);
	NSetColorf(&m_ArcColor,			DEFAULT_NLPATH_GEOMETRY_ARC_COLOR);
	NSetColorf(&m_KeyPointColor,	DEFAULT_NLPATH_GEOMETRY_PATHPOINT_COLOR);
	NSetColorf(&m_KeyPointIDColor,	DEFAULT_NLPATH_GEOMETRY_PATHPOINTID_COLOR);
	NSetColorf(&m_WarningColor,		DEFAULT_NLPATH_GEOMETRY_WARNING_COLOR);
#endif
}

NLPATH_GEOMETRY::~NLPATH_GEOMETRY()
{
	NClearArray(&m_pathPointsArray, NULL);
	NClearArray(&m_primitivesArray, NULL);
}

NLPATH_GEOMETRY::NLPATH_GEOMETRY(NLPATH_GEOMETRY const & from)
{
//	m_flags = from.m_flags;
	m_ds	= from.m_ds;
	NSetupArray(&m_pathPointsArray,  from.m_pathPointsArray.Capacity, sizeof(NLPATH_POINT));
	NSetupArray(&m_primitivesArray, from.m_primitivesArray.Capacity, sizeof(NLPATH_PRIMITIVE));

	NCopyArray(&m_pathPointsArray,  &from.m_pathPointsArray);
	NCopyArray(&m_primitivesArray, &from.m_primitivesArray);

#ifdef _NEDITOR
	m_KeyPointExtend		= from.m_KeyPointExtend;
	m_ClothoidDrawAccuracy	= from.m_ClothoidDrawAccuracy;
	m_ArcDrawAccuracy		= from.m_ArcDrawAccuracy;

	m_SegmentColor			= from.m_SegmentColor;
	m_ClothoidColorA		= from.m_ClothoidColorA;
	m_ClothoidColorB		= from.m_ClothoidColorB;
	m_ArcColor				= from.m_ArcColor;
	m_KeyPointColor			= from.m_KeyPointColor;
	m_KeyPointIDColor		= from.m_KeyPointIDColor;
	m_WarningColor			= from.m_WarningColor;
#endif

}

NLPATH_GEOMETRY& NLPATH_GEOMETRY::operator=(const NLPATH_GEOMETRY& from)
{
	// Si une des deux erreurs suivantes se produisent, il faut envisager un 'erase' des arrays de destination avant de copier.
	NErrorIf(NLclearNLPathPointInArrayCallBack != NULL, NERROR_SYSTEM_CHECK);		// Cela signifie que les path points poss�dent maintenant une call back 'clear en array', donc qu'il y a de la m�moire � d�sallouer !!! 
	NErrorIf(NLclearNLPathPrimitiveInArrayCallBack != NULL, NERROR_SYSTEM_CHECK);	// Cela signifie que les path primitives poss�dent maintenant une call back 'clear en array', donc qu'il y a de la m�moire � d�sallouer !!! 

//	m_flags = from.m_flags;
	m_ds = from.m_ds;
	NCopyArray(&m_pathPointsArray, &from.m_pathPointsArray);
	NCopyArray(&m_primitivesArray, &from.m_primitivesArray);

#ifdef _NEDITOR
	m_KeyPointExtend = from.m_KeyPointExtend;
	m_ClothoidDrawAccuracy = from.m_ClothoidDrawAccuracy;
	m_ArcDrawAccuracy = from.m_ArcDrawAccuracy;

	m_SegmentColor = from.m_SegmentColor;
	m_ClothoidColorA = from.m_ClothoidColorA;
	m_ClothoidColorB = from.m_ClothoidColorB;
	m_ArcColor = from.m_ArcColor;
	m_KeyPointColor = from.m_KeyPointColor;
	m_KeyPointIDColor = from.m_KeyPointIDColor;
	m_WarningColor = from.m_WarningColor;
#endif
	return *this;
}

Nu32 NLPATH_GEOMETRY::write(FILE* pfile)
{
	// 1) �criture Version
	Nu32	version_u32 = VERSION_NLPATH_GEOMETRY_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) �criture Header
	NLPATH_GEOMETRY_HEADER	header;
//	header.m_flags					= m_flags;
	header.m_ds						= m_ds;
	//header.m_keyPointsArraySize		= m_pathPointsArray.Size;
	//header.m_primitivesArraySize	= m_primitivesArray.Size;;
	NGetArrayBounds(&header.m_keyPointsArrayBounds, &m_pathPointsArray);
	NGetArrayBounds(&header.m_primitivesArrayBounds, &m_primitivesArray);
	if (fwrite(&header, sizeof(NLPATH_GEOMETRY_HEADER), 1, pfile) != 1)
		return 0;
	
	// 3) �criture m_pathPointsArray � la main et en une fois
	if (m_pathPointsArray.Size)
	{
		if (fwrite(m_pathPointsArray.pFirst, m_pathPointsArray.ElementSize, m_pathPointsArray.Size, pfile) != m_pathPointsArray.Size)
			return 0;
	}
	// 4) �criture m_primitivesArray � la main et en une fois
	if (m_primitivesArray.Size)
	{
		if (fwrite(m_primitivesArray.pFirst, m_primitivesArray.ElementSize, m_primitivesArray.Size, pfile) != m_primitivesArray.Size)
			return 0;
	}

	return 1;
}

Nu32 NLPATH_GEOMETRY::read(FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLPATH_GEOMETRY_HEADER	header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLPATH_GEOMETRY_HEADER):
		if (fread(&header, sizeof(NLPATH_GEOMETRY_HEADER), 1, pfile) != 1)
			return 0;
	//	m_flags	=	header.m_flags;
		m_ds	=	header.m_ds;

		// Recherche de potentiels pbs sur les arrays.
		// ('NIsArrayCorruptedOrInconsistent' se charge de faire un setup auto en cas de array full of ZEROS)
		if (NIsArrayCorruptedOrInconsistent(&m_pathPointsArray, &header.m_keyPointsArrayBounds,NTRUE))
			return 0;
		if (NIsArrayCorruptedOrInconsistent(&m_primitivesArray, &header.m_primitivesArrayBounds,NTRUE))
			return 0;


		NResizeArray(&m_pathPointsArray, header.m_keyPointsArrayBounds.Size, NULL, NULL);
		NResizeArray(&m_primitivesArray, header.m_primitivesArrayBounds.Size, NULL, NULL);

		if (m_pathPointsArray.Size)
		{
			if (fread(m_pathPointsArray.pFirst, m_pathPointsArray.ElementSize, m_pathPointsArray.Size, pfile) != m_pathPointsArray.Size)
				return 0;

		}
		if (m_primitivesArray.Size)
		{
			if (fread(m_primitivesArray.pFirst, m_primitivesArray.ElementSize, m_primitivesArray.Size, pfile) != m_primitivesArray.Size)
				return 0;
		}
		return 1;

	default:
		break;
	}
	return 0;
}


void NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT *pres, const Nf32 s)const
{
	NErrorIf(s < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(s > m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale
	// TODO !!! Attention la fonction de construction de chunk T (chunkt->build ) peux g�n�rer de NLTRJPOINT_DESC en fin de chunk dont l'abscisse
	// sera plus grande que la longueur totale du chemin !!! Cela est du � l'impr�cision potentielle du calcul des KINs, car pour eux, le s est le resultat
	// de l'integration de la vitesse, de l'acceleration et du jerk au cours du temps ...
	// Pour l'instant ce cas n'est pas g�r� !!! IL VA FALLOIR LE FAIRE !!! SINON ...
	// .... plantage assur� avec la fonction NLPATH_GEOMETRY::getPathPoint, en effet la boucle "while (pkp1->s < s) { pkp1++; pprim++; }" peut engendrer un d�passement ! 
	// .... en debug, le test en entr�e de fonction: "NErrorIf(s > m_ds, NERROR_VALUE_OUTOFRANGE)" crash si s est plus grand que l'abscisse max du path.
	// .... en release, la boucle "while (pkp1->s < s) { pkp1++; pprim++; }" g�n�rera un pkp1 hors de l'array ce qui est MAL MAL MAL !
	//
	//  Le g�rer signifie rallonger un poil le chemin en rajoutant une primitive segment et un path point pour garantir que l'abscisse du dernier KIN du motionProfile
	//  soit inf�rieure ou �gale � celle du chemin.
	//  Pourquoi le dernier Kin du MP ? Parceque le param "S" pass� lors de l'appel � getPathPoint doit toujours g�n�rer un r�sultat valide quand ce S provient d'un des KINs calcul�s
	// lors de la creation du motion profile de d�placement le long de NLPATH_GEOMETRY
	// Recherche de S
	NLPATH_POINT		*pkp1	= ((NLPATH_POINT*)m_pathPointsArray.pFirst) + 1;
	NLPATH_PRIMITIVE	*pprim	= (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	

	while (pkp1->s < s) { pkp1++; pprim++; }
	if (s == pkp1->s)
		*pres = *pkp1;
	else
		getPathPoint(pres, pkp1, pprim, s);
}
void NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT *pres, const NLPATH_POINT *pkp1, const NLPATH_PRIMITIVE *pprim,  const Nf32 s)const
{
	// NErrorIf(s > pkp1->s, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse de pkp1

	NVEC2				A;
	Nf32				ang;
	Nf32				slocal;
	Nf32				theta;
	Nf64				cf, sf, x, y;

	// v�rification de S
	/*
	#ifdef _DEBUG
	NLPATH_POINT *dbg_pkp1 = ((NLPATH_POINT*)m_pathPointsArray.pFirst) + 1;
	NLPATH_PRIMITIVE *dbg_pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	while (dbg_pkp1->s < s) { dbg_pkp1++; dbg_pprim++; }
	NErrorIf(pkp1 != dbg_pkp1, NERROR_INCONSISTENT_VALUES);
	NErrorIf(pprim != dbg_pprim, NERROR_INCONSISTENT_VALUES);
	#endif
	*/

	const NLPATH_POINT	*pkp0 = pkp1 - 1;
//	NErrorIf(s < pkp0->s, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse de pkp0


	slocal = s - pkp0->s;
	pres->s = s;

	switch (pprim->m_core.m_id)
	{
	case NLPATH_PRIMITIVE_ID_SEGMENT:
		pres->p.x = pkp0->p.x + pkp1->u.x*slocal;
		pres->p.y = pkp0->p.y + pkp1->u.y*slocal;
		pres->u = pkp1->u;
		pres->k = pkp1->k;
		return;

	case NLPATH_PRIMITIVE_ID_CLOTHOID:
		if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
		{
			// une seconde clothoide est toujours "invers�e" par rapport au sens du chemin ( elle commence avec une courbure non nulle et termine avec une courbure nulle )
			slocal = pprim->m_core.m_l - slocal;
			NFresnelIntegralsf64(slocal / pprim->m_clothoid.m_param, &cf, &sf);
			x = pprim->m_clothoid.m_param * cf;
			y = pprim->m_clothoid.m_param * sf;

			pres->p.x = (Nf32)(pkp1->p.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y);
			pres->p.y = (Nf32)(pkp1->p.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y);

			// tangente en S:
			theta = pprim->m_clothoid.m_sigma*slocal*slocal / 2.0f;
			x = cos(theta);
			y = sin(theta);
			pres->u.x = (Nf32)(-pprim->m_clothoid.m_i.x * x - pprim->m_clothoid.m_j.x * y);
			pres->u.y = (Nf32)(-pprim->m_clothoid.m_i.y * x - pprim->m_clothoid.m_j.y * y);
			pres->k = slocal * pkp0->k/*pprim->m_clothoid.m_km*/ / pprim->m_core.m_l;
		}
		else
		{
			NFresnelIntegralsf64(slocal / pprim->m_clothoid.m_param, &cf, &sf);
			x = pprim->m_clothoid.m_param * cf;
			y = pprim->m_clothoid.m_param * sf;

			pres->p.x = (Nf32)(pkp0->p.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y);
			pres->p.y = (Nf32)(pkp0->p.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y);

			// tangente en S:
			theta = pprim->m_clothoid.m_sigma*slocal*slocal / 2.0f;
			x = cos(theta);
			y = sin(theta);
			pres->u.x = (Nf32)(pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y);
			pres->u.y = (Nf32)(pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y);
			pres->k = slocal * pkp1->k /* pprim->m_clothoid.m_km*/ / pprim->m_core.m_l;
		}
		return;

	case NLPATH_PRIMITIVE_ID_ARC:
		ang = pprim->m_arc.m_phi + slocal / pprim->m_arc.m_radius;
		/*
		x = cos(ang)*pprim->m_arc.m_radius;
		y = sin(ang)*pprim->m_arc.m_radius;

		A.x = pprim->m_arc.m_i.x * x + pprim->m_arc.m_j.x * y;
		A.y = pprim->m_arc.m_i.y * x + pprim->m_arc.m_j.y * y;

		pres->p.x = pprim->m_arc.m_center.x + A.x;
		pres->p.y = pprim->m_arc.m_center.y + A.y;
		*/
		x = cos(ang);
		y = sin(ang);

		A.x = (Nf32)(pprim->m_arc.m_i.x * x + pprim->m_arc.m_j.x * y);
		A.y = (Nf32)(pprim->m_arc.m_i.y * x + pprim->m_arc.m_j.y * y);

		pres->p.x = pprim->m_arc.m_center.x + A.x * pprim->m_arc.m_radius;
		pres->p.y = pprim->m_arc.m_center.y + A.y * pprim->m_arc.m_radius;

		if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
		{
			pres->u.x = A.y;
			pres->u.y = -A.x;
		}
		else
		{
			pres->u.x = -A.y;
			pres->u.y = A.x;
		}
		pres->k = pkp1->k;

	default:
		return;
	}
}

const NLPATH_POINT* NLPATH_GEOMETRY::getPathPoint(const Nu32 index)
{
	if (index < m_pathPointsArray.Size)
		return (NLPATH_POINT*)NGetArrayPtr(&m_pathPointsArray, index);
	else
		return nullptr;
}
/*
void NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT_S *pres, const Nf32 s)const
{
	NErrorIf(s < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(s > m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale

	// Recherche de S
	NLPATH_POINT		*pkp1 = ((NLPATH_POINT*)m_pathPointsArray.pFirst) + 1;
	NLPATH_PRIMITIVE	*pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	while (pkp1->s < s) { pkp1++; pprim++; }
	getPathPoint(pres, pkp1, pprim, s);
}

void NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT_S *pres, const NLPATH_POINT *pkp1, const NLPATH_PRIMITIVE *pprim, const Nf32 s)const
{
	NErrorIf(s > pkp1->s, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse de pkp1

	// On recherche S sur le chemin
	Nf32				ang;
	Nf32				slocal;
	Nf64				cf, sf, x, y;

	// v�rification de S
	/ *
	#ifdef _DEBUG
	NLPATH_POINT *dbg_pkp1 = ((NLPATH_POINT*)m_pathPointsArray.pFirst) + 1;
	NLPATH_PRIMITIVE *dbg_pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	while (dbg_pkp1->s < s) { dbg_pkp1++; dbg_pprim++; }
	NErrorIf(pkp1 != dbg_pkp1, NERROR_INCONSISTENT_VALUES);
	NErrorIf(pprim != dbg_pprim, NERROR_INCONSISTENT_VALUES);
	#endif
	* /

	const NLPATH_POINT *pkp0 = pkp1 - 1;
	NErrorIf(s < pkp0->s, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse de pkp0

	slocal = s - pkp0->s;

	switch (pprim->m_core.m_id)
	{
	case NLPATH_PRIMITIVE_ID_SEGMENT:
		pres->p.x = pkp0->p.x + pkp0->u.x*slocal;
		pres->p.y = pkp0->p.y + pkp0->u.y*slocal;
		pres->k = pkp1->k;
		return;

	case NLPATH_PRIMITIVE_ID_CLOTHOID:
		if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
		{
			// une seconde clothoide est toujours "invers�e" par rapport au sens du chemin ( elle commence avec une courbure non nulle et termine avec une courbure nulle )
			slocal = pprim->m_core.m_l - slocal;
			NFresnelIntegralsf64(slocal / pprim->m_clothoid.m_param, &cf, &sf);
			x = pprim->m_clothoid.m_param * cf;
			y = pprim->m_clothoid.m_param * sf;

			pres->p.x = (Nf32)(pkp1->p.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y);
			pres->p.y = (Nf32)(pkp1->p.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y);
			pres->k = slocal * pkp0->k / pprim->m_core.m_l;
		}
		else
		{
			NFresnelIntegralsf64(slocal / pprim->m_clothoid.m_param, &cf, &sf);
			x = pprim->m_clothoid.m_param * cf;
			y = pprim->m_clothoid.m_param * sf;

			pres->p.x = (Nf32)(pkp0->p.x + pprim->m_clothoid.m_i.x * x + pprim->m_clothoid.m_j.x * y);
			pres->p.y = (Nf32)(pkp0->p.y + pprim->m_clothoid.m_i.y * x + pprim->m_clothoid.m_j.y * y);
			pres->k = slocal * pkp1->k / pprim->m_core.m_l;
		}
		return;

	case NLPATH_PRIMITIVE_ID_ARC:
		ang = slocal / pprim->m_arc.m_radius;
		x = cos(ang)*(Nf64)pprim->m_arc.m_radius;
		y = sin(ang)*(Nf64)pprim->m_arc.m_radius;

		pres->p.x = (Nf32)(pprim->m_arc.m_center.x + pprim->m_arc.m_i.x * x + pprim->m_arc.m_j.x * y);
		pres->p.y = (Nf32)(pprim->m_arc.m_center.y + pprim->m_arc.m_i.y * x + pprim->m_arc.m_j.y * y);
		pres->k = pkp1->k;
		return;

	default:
		return;

	}
}
*/

#ifdef _NEDITOR
void NLPATH_GEOMETRY::draw(const NMATRIX* pmx, const Nf32 path_width)
{
	NLPATH_POINT		*pkp0, *pkp1;
	NLPATH_PRIMITIVE	*pprim;
	NVEC3f32			u,vi,vj;
	NVEC3f32			v0, v1;
	NVEC3f32			vl0, vl1;
	NVEC3f32			vr0, vr1;
	NVEC3f32			vc;
	NVEC2f32			tgt;
	Nf32				half_track = path_width / 2.0f;
	Nf32				sm,s, x, y,ang;
	Nf32				xl, yl, xr, yr;
	Nf32				theta;
	Nf32				cf, sf;
	NCOLOR				left_color = { NCOLOR_PRESET3F_GREEN,1 };
	NCOLOR				right_color = { NCOLOR_PRESET3F_ORANGE_GOLD,1 };
	Nu32				i,j;

	// On ne trace un chemin ( et ses keypoints) que si il y a au moins une primitive )

	if (m_primitivesArray.Size)
	{
		Nchar			txt[8];
		NUT_DRAWTEXT	drawtxt;
		Nmem0(&drawtxt, NUT_DRAWTEXT);
		drawtxt.Size	= m_KeyPointExtend.y;
		drawtxt.Color	= m_KeyPointIDColor;

		// Tracage Keypoints
		pkp0 = (NLPATH_POINT*)m_pathPointsArray.pFirst;
		for (i = 0; i < m_pathPointsArray.Size; i++, pkp0++)
		{
			// KeyPoint marker and Id:
			NMulVector2ByMatrixO(&v0, pmx, &pkp0->p);
			NUT_Draw_Cross(&v0, &m_KeyPointExtend, &m_KeyPointColor);
			sprintf(txt, " %d", i);
			NUT_Draw_Text(txt, &v0, &drawtxt);

			// Keypoint "orthogonal basis"
			NMulVector2ByMatrix3O(&u, pmx, &pkp0->u);
			v1.x = v0.x + u.x * m_KeyPointExtend.x*2.0f;
			v1.y = v0.y + u.y * m_KeyPointExtend.x*2.0f;
			v1.z =		  u.z * m_KeyPointExtend.x*2.0f; // est suppos� �tre null
			NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
			v1.x = v0.x - u.y * m_KeyPointExtend.x * 2.0f;
			v1.y = v0.y + u.x * m_KeyPointExtend.x * 2.0f;
			v1.z =		  u.z * m_KeyPointExtend.x * 2.0f; // est suppos� �tre null

			NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
		}

		pkp0 = (NLPATH_POINT*)m_pathPointsArray.pFirst;
		pkp1 = pkp0 + 1;
		pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
		for (i = 0; i < m_primitivesArray.Size; i++, pprim++, pkp0 = pkp1, pkp1++)
		{
			switch (pprim->m_core.m_id)
			{
			case NLPATH_PRIMITIVE_ID_SEGMENT:
				/*
				v0.x = pkp0->p.x;
				v0.y = pkp0->p.y;
				v0.z = 0.0f;
				v1.x = pkp1->p.x;
				v1.y = pkp1->p.y;
				v1.z = 0.0f;
				*/
				NMulVector2ByMatrixO(&v0, pmx, &pkp0->p);
				NMulVector2ByMatrixO(&v1, pmx, &pkp1->p);
				

				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
					NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
				else
					NUT_Draw_Segment(&v0, &v1, &m_SegmentColor);

				if (half_track)
				{
					NMulVector2ByMatrix3O(&u, pmx, &pkp1->u);
					vr0.x = v0.x + u.y * half_track;
					vr0.y = v0.y - u.x * half_track;
					vr0.z = 0.0f;
					vr1.x = v1.x + u.y * half_track;
					vr1.y = v1.y - u.x * half_track;
					vr1.z = 0.0f;
					NUT_Draw_Segment(&vr0, &vr1, &right_color);
					vl0.x = v0.x - u.y * half_track;
					vl0.y = v0.y + u.x * half_track;
					vl0.z = 0.0f;
					vl1.x = v1.x - u.y * half_track;
					vl1.y = v1.y + u.x * half_track;
					vl1.z = 0.0f;
					NUT_Draw_Segment(&vl0, &vl1, &left_color);

				}
				break;

			case NLPATH_PRIMITIVE_ID_CLOTHOID:
				/*
				v0.x = pkp0->p.x;
				v0.y = pkp0->p.y;
				v0.z = 0.0f;
				*/
				NMulVector2ByMatrixO(&v0, pmx, &pkp0->p);
				sm = pprim->m_core.m_l;

				//if (half_track)
				//{
					NMulVector2ByMatrix3O(&u, pmx, &pkp0->u);
					vr0.x = v0.x + u.y * half_track;
					vr0.y = v0.y - u.x * half_track;
					vr0.z = 0.0f;
					vr1.z = 0.0f;

					vl0.x = v0.x - u.y * half_track;
					vl0.y = v0.y + u.x * half_track;
					vl0.z = 0.0f;
					vl1.z = 0.0f;
				//}

				NMulVector2ByMatrix3O(&vi, pmx, &pprim->m_clothoid.m_i);
				NMulVector2ByMatrix3O(&vj, pmx, &pprim->m_clothoid.m_j);

				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
				{
					NMulVector2ByMatrixO(&v1, pmx, &pkp1->p);
					for (j = 0; j <= m_ClothoidDrawAccuracy; j++, v0 = vc, vr0 = vr1, vl0 = vl1)
					{
						s = (sm * (Nf32)(m_ClothoidDrawAccuracy - j) / (Nf32)m_ClothoidDrawAccuracy);
						NFresnelIntegralsf32(s / pprim->m_clothoid.m_param, &cf, &sf);
						x = pprim->m_clothoid.m_param * cf;
						y = pprim->m_clothoid.m_param * sf;
						vc.x = v1.x + vi.x * x + vj.x * y;
						vc.y = v1.y + vi.y * x + vj.y * y;
						vc.z = 0.0f;
						NUT_Draw_Segment(&v0, &vc, &m_ClothoidColorB);

						if (half_track)
						{
							// tangente en s:
							theta = pprim->m_clothoid.m_sigma * s * s / 2.0f;
							x = cos(theta);
							y = sin(theta);
							tgt.x = -vi.x * x - vj.x * y;
							tgt.y = -vi.y * x - vj.y * y;

							vl1.x = vc.x - tgt.y * half_track;
							vl1.y = vc.y + tgt.x * half_track;

							vr1.x = vc.x + tgt.y * half_track;
							vr1.y = vc.y - tgt.x * half_track;
							NUT_Draw_Segment(&vl0, &vl1, &left_color);
							NUT_Draw_Segment(&vr0, &vr1, &right_color);
						}
					}
				}
				else
				{
					vc = v0;
				

					for (j = 0; j <= m_ClothoidDrawAccuracy; j++, v0 = v1, vr0 = vr1, vl0 = vl1)
					{
						s = (sm * (Nf32)j / (Nf32)m_ClothoidDrawAccuracy);
						NFresnelIntegralsf32(s / pprim->m_clothoid.m_param, &cf, &sf);	// |
						x = pprim->m_clothoid.m_param * cf;								// |-- Ref. [1.6] page 42
						y = pprim->m_clothoid.m_param * sf;								// |	
					
						v1.x = vc.x + vi.x * x + vj.x * y;
						v1.y = vc.y + vi.y * x + vj.y * y;
						v1.z = 0.0f;
						NUT_Draw_Segment(&v0, &v1, &m_ClothoidColorA);

						if (half_track)
						{
							// tangente en s:
							theta = pprim->m_clothoid.m_sigma * s * s / 2.0f;
							x = cos(theta);
							y = sin(theta);
							tgt.x = vi.x * x + vj.x * y;
							tgt.y = vi.y * x + vj.y * y;

							vl1.x = v1.x - tgt.y * half_track;
							vl1.y = v1.y + tgt.x * half_track;

							vr1.x = v1.x + tgt.y * half_track;
							vr1.y = v1.y - tgt.x * half_track;
							NUT_Draw_Segment(&vl0, &vl1, &left_color);
							NUT_Draw_Segment(&vr0, &vr1, &right_color);
						}
					}
				}
				break;

			case NLPATH_PRIMITIVE_ID_ARC:
				/*
				v0.x = pkp0->p.x;
				v0.y = pkp0->p.y;
				v0.z = 0.0f;
				*/
				NMulVector2ByMatrixO(&v0, pmx, &pkp0->p);
				//if (half_track)
				//{
					NMulVector2ByMatrix3O(&u, pmx, &pkp0->u);
					vr0.x = v0.x + u.y * half_track;
					vr0.y = v0.y - u.x * half_track;
					vr0.z = 0.0f;
					vr1.z = 0.0f;

					vl0.x = v0.x - u.y * half_track;
					vl0.y = v0.y + u.x * half_track;
					vl0.z = 0.0f;
					vl1.z = 0.0f;
				//}
				NMulVector2ByMatrixO(&vc, pmx, &pprim->m_arc.m_center);
				NMulVector2ByMatrix3O(&vi, pmx, &pprim->m_arc.m_i);
				NMulVector2ByMatrix3O(&vj, pmx, &pprim->m_arc.m_j);
				// Tra�age arc de cercle ins�r�:
				for (j = 0; j <= m_ArcDrawAccuracy; j++, v0 = v1, vr0 = vr1, vl0 = vl1)
				{
					ang = pprim->m_arc.m_phi + ((pprim->m_arc.m_omega * (Nf32)j) / (Nf32)m_ArcDrawAccuracy);
					x = cos(ang) * pprim->m_arc.m_radius;
					y = sin(ang) * pprim->m_arc.m_radius;
					v1.x = vc.x + vi.x * x + vj.x * y;
					v1.y = vc.y + vi.y * x + vj.y * y;

					if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
					{
						NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
					}
					else
					{
						NUT_Draw_Segment(&v0, &v1, &m_ArcColor);
					}

					if (half_track)
					{
						if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
						{
							xl = cos(ang) * (pprim->m_arc.m_radius + half_track);
							yl = sin(ang) * (pprim->m_arc.m_radius + half_track);
							xr = cos(ang) * (pprim->m_arc.m_radius - half_track);
							yr = sin(ang) * (pprim->m_arc.m_radius - half_track);
						}
						else
						{
							xl = cos(ang) * (pprim->m_arc.m_radius - half_track);
							yl = sin(ang) * (pprim->m_arc.m_radius - half_track);
							xr = cos(ang) * (pprim->m_arc.m_radius + half_track);
							yr = sin(ang) * (pprim->m_arc.m_radius + half_track);
						}


						vl1.x = vc.x + vi.x * xl + vj.x * yl;
						vl1.y = vc.y + vi.y * xl + vj.y * yl;

						vr1.x = vc.x + vi.x * xr + vj.x * yr;
						vr1.y = vc.y + vi.y * xr + vj.y * yr;

						NUT_Draw_Segment(&vl0, &vl1, &left_color);
						NUT_Draw_Segment(&vr0, &vr1, &right_color);
					}
				}
				break;

			default:
				break;
			}
		}
	}
}

void NLPATH_GEOMETRY::draw(const NMATRIX* pmx, const Nf32 path_width, const Nf32 s0, const Nf32 s1)
{
	NErrorIf(s0 < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(s0 > m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale
	NErrorIf(s1 < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(s1 > m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale

	if (m_primitivesArray.Size < 1)
		return;
	Nu32				j;

	// specifique Clothoid
	Nf32				s, ang;
	Nf32				theta;
	Nf32				cf, sf;
	NVEC2f32			tgt;

	//specific arc
	Nf32				ang_a;
	Nf32				ang_b;
	Nf32				ang_omega;

	// Basic position / direction transform�es dans la base 'pmx'	
	NVEC3f32			v0, v1;
	NVEC3f32			va, vb;
	NVEC3f32			u;
	v0.z = 0.0f;
	v1.z = 0.0f;
	va.z = 0.0f;
	vb.z = 0.0f;
	u.z  = 0.0f;

	// halftrack
	NVEC3f32			vla, vlb;
	NVEC3f32			vra, vrb;
	vra.z = 0.0f;
	vrb.z = 0.0f;
	vla.z = 0.0f;
	vlb.z = 0.0f;


	NVEC3f32			vo, vi, vj; // origine, axe X, axe Y
	/*
	// Pr�paration Affichage Texte
	Nchar			txt[8];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = m_KeyPointExtend.y;
	drawtxt.Color = m_KeyPointIDColor;
	*/
	// Pr�parations diverses
	NCOLOR* pcolor = &m_WarningColor;			// tmp: couleur de tracage
	Nf32	half_track = path_width / 2.0f;		// Demi largeur du path
	Nu32	jmx;								// nbre de steps pour le tracage des Arc et clothoides ( d�pend des accuracy et de la longueur d'arc � tracer ... )
	Nf32	x, y, xl, yl, xr, yr;

	// Recherche de la primitive portant s0
	NLPATH_POINT* pkp0 = NULL;
	NLPATH_POINT* pkp1 = ((NLPATH_POINT*)m_pathPointsArray.pFirst) + 1;
	NLPATH_PRIMITIVE* pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	while (pkp1->s < s0) { pkp1++; pprim++; }
	// Recul de 1 purement 'techniqie' pour 'coller' avec la boucle de traitement qui avance de 1 � chaque tour d�s le d�but du premier tour.
	pprim--; pkp1--; 

	// Initialisation des abscisses de l'intervalle de tracage 
	Nf32 sa = s0;
	Nf32 sb = s0;
	Nf32 slocal_a, slocal_b;

	while (sb < s1)
	{
		// Primitive suivante.
		pprim ++;
		pkp0 = pkp1;
		pkp1 ++;

		// D�finition des abscisses de l'intervalle de tracage 
		sa = sb;
		sb = NMIN(s1, pkp1->s);

		// tracage de l'intervalle [sa,sb] situ� sur la primitive 'pprim'
		switch (pprim->m_core.m_id)
		{
		case NLPATH_PRIMITIVE_ID_SEGMENT:
			slocal_a = sa - pkp0->s;
			slocal_b = sb - pkp0->s;

			NMulVector2ByMatrixO(&v0, pmx, &pkp0->p);
			//NMulVector2ByMatrixO(&v1, pmx, &pkp1->p);
			NMulVector2ByMatrix3O(&u, pmx, &pkp1->u);

			va.x = v0.x + slocal_a * u.x;
			va.y = v0.y + slocal_a * u.y;
			vb.x = v0.x + slocal_b * u.x;
			vb.y = v0.y + slocal_b * u.y;
			NUT_Draw_Segment(&va, &vb, pcolor);
			/*
			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
				NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
			else
				NUT_Draw_Segment(&v0, &v1, &m_SegmentColor);
			*/

			if (half_track)
			{
				vra.x = va.x + u.y * half_track;
				vra.y = va.y - u.x * half_track;
				vrb.x = vb.x + u.y * half_track;
				vrb.y = vb.y - u.x * half_track;
				NUT_Draw_Segment(&vra, &vrb, pcolor);

				vla.x = va.x - u.y * half_track;
				vla.y = va.y + u.x * half_track;
				vlb.x = vb.x - u.y * half_track;
				vlb.y = vb.y + u.x * half_track;
				NUT_Draw_Segment(&vla, &vlb, pcolor);

			//	NUT_Draw_Segment(&vla, &vra, pcolor);
			//	NUT_Draw_Segment(&vlb, &vrb, pcolor);
			}
			break;

		case NLPATH_PRIMITIVE_ID_CLOTHOID:
			NMulVector2ByMatrix3O(&vi, pmx, &pprim->m_clothoid.m_i);
			NMulVector2ByMatrix3O(&vj, pmx, &pprim->m_clothoid.m_j);

			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
			{
				// origine et abscisses 'locales'
				NMulVector2ByMatrixO(&vo, pmx, &pkp1->p);	
				slocal_a = -(sa - pkp1->s);
				slocal_b = -(sb - pkp1->s);

				// Initialisation premier point d'abscisse sb:
				NFresnelIntegralsf32(slocal_b / pprim->m_clothoid.m_param, &cf, &sf);	// |
				x = pprim->m_clothoid.m_param * cf;										// |-- Ref. [1.6] page 42
				y = pprim->m_clothoid.m_param * sf;										// |	
				vb.x = vo.x + vi.x * x + vj.x * y;
				vb.y = vo.y + vi.y * x + vj.y * y;


				if (half_track)
				{
					// tangente en s:
					theta = pprim->m_clothoid.m_sigma * slocal_b * slocal_b / 2.0f;
					x = cos(theta);
					y = sin(theta);
					tgt.x = vi.x * x + vj.x * y;
					tgt.y = vi.y * x + vj.y * y;

					vlb.x = vb.x - tgt.y * half_track;
					vlb.y = vb.y + tgt.x * half_track;
					vrb.x = vb.x + tgt.y * half_track;
					vrb.y = vb.y - tgt.x * half_track;
				}

				// nbre de segments:
				Nu32 jmx = (Nu32)(((Nf32)m_ClothoidDrawAccuracy * (sb - sa)) / pprim->m_core.m_l) + 2; // +2 � cause du test j < jmx, de tel sorte que j 'finisse' sur ...+1 !

				for (j = 0; j < jmx; j++, vb = va, vrb = vra, vlb = vla)
				{
					s = slocal_b + (slocal_a - slocal_b) * (Nf32)j / (Nf32)jmx;
					NFresnelIntegralsf32(s / pprim->m_clothoid.m_param, &cf, &sf);
					x = pprim->m_clothoid.m_param * cf;
					y = pprim->m_clothoid.m_param * sf;
					va.x = vo.x + vi.x * x + vj.x * y;
					va.y = vo.y + vi.y * x + vj.y * y;
					NUT_Draw_Segment(&vb, &va, pcolor);

					if (half_track)
					{
						// tangente en s:
						theta = pprim->m_clothoid.m_sigma * s * s / 2.0f;
						x = cos(theta);
						y = sin(theta);
						tgt.x = -vi.x * x - vj.x * y;
						tgt.y = -vi.y * x - vj.y * y;

						vla.x = va.x - tgt.y * half_track;
						vla.y = va.y + tgt.x * half_track;

						vra.x = va.x + tgt.y * half_track;
						vra.y = va.y - tgt.x * half_track;

						NUT_Draw_Segment(&vlb, &vla, pcolor);
						NUT_Draw_Segment(&vrb, &vra, pcolor);

					//	NUT_Draw_Segment(&vlb, &vrb, pcolor);
					//	NUT_Draw_Segment(&vla, &vra, pcolor);
					}
				}
			}
			else
			{
				// origine et abscisses 'locales'
				NMulVector2ByMatrixO(&vo, pmx, &pkp0->p);
				slocal_a = sa - pkp0->s;
				slocal_b = sb - pkp0->s;

				// Initialisation premier point d'abscisse sa:
				NFresnelIntegralsf32(slocal_a / pprim->m_clothoid.m_param, &cf, &sf);	// |
				x = pprim->m_clothoid.m_param * cf;										// |-- Ref. [1.6] page 42
				y = pprim->m_clothoid.m_param * sf;										// |	
				va.x = vo.x + vi.x * x + vj.x * y;
				va.y = vo.y + vi.y * x + vj.y * y;
				if (half_track)
				{
					// tangente en s:
					theta = pprim->m_clothoid.m_sigma * slocal_a * slocal_a / 2.0f;
					x = cos(theta);
					y = sin(theta);
					tgt.x = vi.x * x + vj.x * y;
					tgt.y = vi.y * x + vj.y * y;

					vla.x = va.x - tgt.y * half_track;
					vla.y = va.y + tgt.x * half_track;
					vra.x = va.x + tgt.y * half_track;
					vra.y = va.y - tgt.x * half_track;
				}

				// nbre de segments:
				jmx = (Nu32)(((Nf32)m_ClothoidDrawAccuracy*(sb-sa)) / pprim->m_core.m_l) + 2; // +2 � cause du test j < jmx, de tel sorte que j 'finisse' sur ...+1 !

				for (j = 0; j < jmx; j++, va = vb, vra = vrb, vla = vlb)
				{
					s = slocal_a + (slocal_b- slocal_a) * (Nf32)j / (Nf32)jmx;
					NFresnelIntegralsf32(s / pprim->m_clothoid.m_param, &cf, &sf);	// |
					x = pprim->m_clothoid.m_param * cf;								// |-- Ref. [1.6] page 42
					y = pprim->m_clothoid.m_param * sf;								// |	
					vb.x = vo.x + vi.x * x + vj.x * y;
					vb.y = vo.y + vi.y * x + vj.y * y;
					NUT_Draw_Segment(&va, &vb, pcolor);

					if (half_track)
					{
						// tangente en s:
						theta = pprim->m_clothoid.m_sigma * s * s / 2.0f;
						x = cos(theta);
						y = sin(theta);
						tgt.x = vi.x * x + vj.x * y;
						tgt.y = vi.y * x + vj.y * y;

						vlb.x = vb.x - tgt.y * half_track;
						vlb.y = vb.y + tgt.x * half_track;

						vrb.x = vb.x + tgt.y * half_track;
						vrb.y = vb.y - tgt.x * half_track;
						NUT_Draw_Segment(&vla, &vlb, pcolor);
						NUT_Draw_Segment(&vra, &vrb, pcolor);

					//	NUT_Draw_Segment(&vlb, &vrb, pcolor);
					//	NUT_Draw_Segment(&vla, &vra, pcolor);
					}
				}
			}
			break;

		case NLPATH_PRIMITIVE_ID_ARC:
			// origine et abscisses 'locales'
			NMulVector2ByMatrixO(&vo, pmx, &pprim->m_arc.m_center);
			NMulVector2ByMatrix3O(&vi, pmx, &pprim->m_arc.m_i);
			NMulVector2ByMatrix3O(&vj, pmx, &pprim->m_arc.m_j);

			slocal_a	= sa - pkp0->s;
			slocal_b	= sb - pkp0->s;
			ang_a		= slocal_a / pprim->m_arc.m_radius;
			ang_b		= slocal_b / pprim->m_arc.m_radius;
			ang_omega	= ang_b - ang_a;

			ang = pprim->m_arc.m_phi + ang_a;
			x = cos(ang) * pprim->m_arc.m_radius;
			y = sin(ang) * pprim->m_arc.m_radius;
			va.x = vo.x + vi.x * x + vj.x * y;
			va.y = vo.y + vi.y * x + vj.y * y;

			if (half_track)
			{
				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
				{
					xl = cos(ang) * (pprim->m_arc.m_radius + half_track);
					yl = sin(ang) * (pprim->m_arc.m_radius + half_track);
					xr = cos(ang) * (pprim->m_arc.m_radius - half_track);
					yr = sin(ang) * (pprim->m_arc.m_radius - half_track);
				}
				else
				{
					xl = cos(ang) * (pprim->m_arc.m_radius - half_track);
					yl = sin(ang) * (pprim->m_arc.m_radius - half_track);
					xr = cos(ang) * (pprim->m_arc.m_radius + half_track);
					yr = sin(ang) * (pprim->m_arc.m_radius + half_track);
				}
				vla.x = vo.x + vi.x * xl + vj.x * yl;
				vla.y = vo.y + vi.y * xl + vj.y * yl;
				vra.x = vo.x + vi.x * xr + vj.x * yr;
				vra.y = vo.y + vi.y * xr + vj.y * yr;
			}

			// nbre de segments:
			jmx = (Nu32)(((Nf32)m_ArcDrawAccuracy * ang_omega) / pprim->m_arc.m_omega) + 2; // +2 � cause du test j < jmx, de tel sorte que j 'finisse' sur ...+1 !

			// Tra�age arc de cercle ins�r�:
			for (j = 0; j <= jmx; j++, va = vb, vra = vrb, vla = vlb)
			{
				ang = pprim->m_arc.m_phi + ang_a + ((ang_omega * (Nf32)j) / (Nf32)jmx);
				x = cos(ang) * pprim->m_arc.m_radius;
				y = sin(ang) * pprim->m_arc.m_radius;
				vb.x = vo.x + vi.x * x + vj.x * y;
				vb.y = vo.y + vi.y * x + vj.y * y;
				NUT_Draw_Segment(&va, &vb, pcolor);

				/*
				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
				{
					NUT_Draw_Segment(&v0, &v1, &m_WarningColor);
				}
				else
				{
					NUT_Draw_Segment(&v0, &v1, &m_ArcColor);
				}
				*/

				if (half_track)
				{
					if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
					{
						xl = cos(ang) * (pprim->m_arc.m_radius + half_track);
						yl = sin(ang) * (pprim->m_arc.m_radius + half_track);
						xr = cos(ang) * (pprim->m_arc.m_radius - half_track);
						yr = sin(ang) * (pprim->m_arc.m_radius - half_track);
					}
					else
					{
						xl = cos(ang) * (pprim->m_arc.m_radius - half_track);
						yl = sin(ang) * (pprim->m_arc.m_radius - half_track);
						xr = cos(ang) * (pprim->m_arc.m_radius + half_track);
						yr = sin(ang) * (pprim->m_arc.m_radius + half_track);
					}
					vlb.x = vo.x + vi.x * xl + vj.x * yl;
					vlb.y = vo.y + vi.y * xl + vj.y * yl;
					vrb.x = vo.x + vi.x * xr + vj.x * yr;
					vrb.y = vo.y + vi.y * xr + vj.y * yr;
					NUT_Draw_Segment(&vla, &vlb, pcolor);
					NUT_Draw_Segment(&vra, &vrb, pcolor);

				//	NUT_Draw_Segment(&vlb, &vrb, pcolor);
				//	NUT_Draw_Segment(&vla, &vra, pcolor);
				}
			}
			break;

		default:
			break;
		}
	}
}

void NLPATH_GEOMETRY::drawCurvature(NL2DOCS* pocs, const NCOLORPICKPACK pickpack)
{
	//NUT_DRAWTEXT		drawtxt;
	NVEC3f32			txtpos;
	Nchar				txt[8];
	NLPATH_PRIMITIVE* pprim;
	Nu32				i;


	Nf32 s = 0;
	NUTDRAWVERTEX	va, vb;
	va.Color0_4f = m_SegmentColor;
	vb.Color0_4f = m_SegmentColor;
	va.Position_3f.z = 0.0f; vb.Position_3f.z = 0.0f;
	vb.Position_3f.x = pocs->getOrigin()->x;

	//Nmem0(&drawtxt, NUT_DRAWTEXT);
	//drawtxt.Size = 0.015f;
	//drawtxt.Color = m_KeyPointIDColor;
	txtpos.x = pocs->getOrigin()->x;
	txtpos.y = pocs->getOrigin()->y - pocs->getCaptionParam()->Size * 2.0f;
	txtpos.z = 0.0f;
	sprintf(txt, "%d", 0);
	NUT_Draw_Text(txt, &txtpos, pocs->getCaptionParam());

	pprim = (NLPATH_PRIMITIVE*)m_primitivesArray.pFirst;
	for (i = 0; i < m_primitivesArray.Size; i++, pprim++)
	{
		va.Position_3f.x = vb.Position_3f.x;
		s += pprim->m_core.m_l;
		vb.Position_3f.x = pocs->transformX(s);

		txtpos.x = vb.Position_3f.x;
		sprintf(txt, "%d", i + 1);
		NUT_Draw_Text(txt, &txtpos, pocs->getCaptionParam());


		switch (pprim->m_core.m_id)
		{
		case NLPATH_PRIMITIVE_ID_SEGMENT:
			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
			{
				va.Color0_4f = m_WarningColor;
				vb.Color0_4f = m_WarningColor;
			}
			else
			{
				va.Color0_4f = m_SegmentColor;
				vb.Color0_4f = m_SegmentColor;
			}

			va.Position_3f.y = pocs->transformY(0.0f);		NUT_DrawPencil_From(&va);
			vb.Position_3f.y = pocs->transformY(0.0f);		NUT_DrawPencil_LineTo(&vb);
			break;

		case NLPATH_PRIMITIVE_ID_CLOTHOID:
			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
			{
				va.Color0_4f = m_ClothoidColorB;
				vb.Color0_4f = m_ClothoidColorB;
				va.Position_3f.y = pocs->transformY(pprim->m_clothoid.m_km);	NUT_DrawPencil_From(&va);
				vb.Position_3f.y = pocs->transformY(0.0f);						NUT_DrawPencil_LineTo(&vb);
			}
			else
			{
				va.Color0_4f = m_ClothoidColorA;
				vb.Color0_4f = m_ClothoidColorA;
				va.Position_3f.y = pocs->transformY(0.0f);						NUT_DrawPencil_From(&va);
				vb.Position_3f.y = pocs->transformY(pprim->m_clothoid.m_km);	NUT_DrawPencil_LineTo(&vb);
			}
			break;

		case NLPATH_PRIMITIVE_ID_ARC:
			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_PRIMITIVE_RAW))
			{
				va.Color0_4f = m_WarningColor;
				vb.Color0_4f = m_WarningColor;
			}
			else
			{
				va.Color0_4f = m_ArcColor;
				vb.Color0_4f = m_ArcColor;
			}
			va.Position_3f.y = pocs->transformY(1.0f / pprim->m_arc.m_radius);	NUT_DrawPencil_From(&va);
			vb.Position_3f.y = pocs->transformY(1.0f / pprim->m_arc.m_radius);	NUT_DrawPencil_LineTo(&vb);
			break;

		default:
			break;
		}
	}
}
#endif //_NEDITOR