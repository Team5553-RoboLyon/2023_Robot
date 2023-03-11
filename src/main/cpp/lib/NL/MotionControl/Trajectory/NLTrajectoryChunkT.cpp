/*
#include "lib/NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPackGetPointPersistentResult.h"
#include "NLTrajectoryChunkT.h"

// NLTRAJECTORY_CHUNK_T
NLTRAJECTORY_CHUNK_T::NLTRAJECTORY_CHUNK_T()
{
	NSetupArray(&m_trajectoryPointDescArray, DEFAULT_TRAJECTORYPOINTDESC_ARRAY_CAPACITY, sizeof(NLTRJPOINT_DESC));
}

NLTRAJECTORY_CHUNK_T::~NLTRAJECTORY_CHUNK_T()
{
	NClearArray(&m_trajectoryPointDescArray, NLclearNLTrajectoryPointDescInArrayCallBack);
}

void NLTRAJECTORY_CHUNK_T::selfDuplicate(void* palready_allocated_memory)
{
	NLTRAJECTORY_CHUNK_T* pduplicatedchunkt = new(palready_allocated_memory)NLTRAJECTORY_CHUNK_T;
	NCopyArray(&pduplicatedchunkt->m_trajectoryPointDescArray, &m_trajectoryPointDescArray);
	pduplicatedchunkt->timeRange(m_t0, m_t1);
	pduplicatedchunkt->abscissaRange(m_s0, m_s1);
}

void NLTRAJECTORY_CHUNK_T::initializePersistentData(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* ppersist)
{
	ppersist->m_pChunkT_TrajectoryPointDesc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
}

void NLTRAJECTORY_CHUNK_T::getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const NLPATH_GEOMETRY* ppathgeometry, const Nf32 t)
{
	NErrorIf(t <= m_t0 || t > m_t1, NERROR_VALUE_OUTOFRANGE);
	NLTRJPOINT_DESC*	pp0;

	NErrorIf(!NIsValidArrayElementPtr(&m_trajectoryPointDescArray, (NBYTE*)presult->m_pChunkT_TrajectoryPointDesc), NERROR_SYSTEM_GURU_MEDITATION);
	NLTRJPOINT_DESC* ppdesc = presult->m_pChunkT_TrajectoryPointDesc;
	if (t <= m_t0)
	{
		NErrorIf(ppdesc != (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst, NERROR_INCONSISTENT_VALUES);
		presult->m_result.m_kin = ppdesc->m_kin;
	}
	else // (t > m_t0 && t < m_t1)
	{
		NErrorIf(!(t > m_t0 && t <= m_t1), NERROR_VALUE_OUTOFRANGE);
		while (ppdesc->m_kin.m_t < t) { ppdesc++; }
		pp0 = ppdesc - 1;

		presult->m_result.m_kin.from(&pp0->m_kin, ppdesc->m_kin.m_j, t - pp0->m_kin.m_t);
	}
	ppathgeometry->getPathPoint((NLPATH_POINT*)presult, ppdesc->m_pPathPoint1, ppdesc->m_pPrimitive, presult->m_result.m_kin.m_s);
	presult->m_pChunkT_TrajectoryPointDesc = ppdesc;
}

Nu32 NLTRAJECTORY_CHUNK_T::read(FILE* pfile)
{
	return 0;
}

Nu32 NLTRAJECTORY_CHUNK_T::write(FILE* pfile)
{
	return 0;
}

void NLTRAJECTORY_CHUNK_T::draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
	//if (ISFLAG_ON(p2docs->m_Flags, FLAG_NL2DOCS_DASHBOARD_MP_CHUNK_T))
	//{
		NLTRJPOINT_DESC* pdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst; pdsc++;
		for (Nu32 i = 1; i < m_trajectoryPointDescArray.Size; i++, pdsc++)
		{
			pdsc->m_kin.draw(p2docs, pickpack, &(pdsc - 1)->m_kin);
		}
	//}
}
*/
/*
Nu32 NLTRAJECTORY_CHUNK_T::read(NLPATH_WORKBENCH* pwb)
{
	return 0;
}
*/
/*
Nf32 NLTRAJECTORY_CHUNK_T::getTime(const Nf32 s)
{
	if (s <= m_s0)
	{
		return m_t0;
	}
	else if (s < m_s1)
	{
		NLTRJPOINT_DESC* pp = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
		Nf32	p, q, sqrt_dlt;

		while (pp->m_kin.m_s < s)
			pp++;

		NLTRJPOINT_DESC* pp0 = pp - 1;
		if (!pp->m_kin.m_j)
		{
			if (!pp0->m_kin.m_a)
			{
				//	NErrorIf(pk->m_a, NERROR_SYSTEM_CHECK);				// impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				//	NErrorIf(pk0->m_v != pk->m_v, NERROR_SYSTEM_CHECK); // impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...

				NErrorIf(!pp->m_kin.m_v, NERROR_SYSTEM_CHECK);			// Dans ce cas, la vitesse courante et donc constante ne saurait �tre nulle !
				return pp0->m_kin.m_t + (s - pp0->m_kin.m_s) / pp0->m_kin.m_v;
			}
			else
			{
				// Quelque soit le signe de "pk0->m_a" seule la racine " R2 = (-B + sqrt(delta) )/2A " est applicable � notre situation ( cf etude et tableau de signe )
				return pp0->m_kin.m_t + (-pp0->m_kin.m_v + sqrt(NPOW2(pp0->m_kin.m_v) - 2.0f * pp0->m_kin.m_a * (pp0->m_kin.m_s - s))) / pp0->m_kin.m_a;
			}

			// C'est une phase 2 qui m�ne � pk !
			// On recherche donc une valeur de t solution de l'�quation:
			//	s = pk0->s + pk0->m_v*t + pk0->m_a*t*t/2.0f
			//
			//	On a donc delta = NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) discriminant de l'�quation.
			//  Pour avoir au moins une solution, il faut que delta soit >=0
			//						delta	>=	0
			//	[1]					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0
			//						NPOW2(pk0->m_v) >=	2.0f*pk0->m_a*(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) >=	(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) - pk0->m_s >=	- s
			//	[2]				   -NPOW2(pk0->m_v)/(2.0f*pk0->m_a) + pk0->m_s <=	s
			//
			/ *
			if (pk0->m_a > 0.0f)
			{
				// Il y a TOUJOURS deux racines mais une seule positive !
				// ------------------------------------------------------
				// Dans [1] on voit bien que si pk0->m_a > 0 alors, - 2.0f*pk0->m_a*(pk0->m_s - s)	> 0, car (pk0->m_s - s) < 0 !
				// on a aussi NPOW2(pk0->m_v) >= 0, Donc
				//					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0 est TOUJOURS VRAI
				//
				//						delta  =	NPOW2(pk0->m_v) + qqchose ( avec qqchose = + 2.0f*NABS( pk0->m_a*(pk0->m_s - s) ) )
				//						donc,
				//						delta  >	NPOW2(pk0->m_v)
				// on aura donc TOUJOURS
				//						sqrt(delta) > pk0->m_v
				// donc,
				//						sqrt(delta) - pk0->m_v  > 0
				//
				// Donc,
				//						R2 =  (-pk0->m_v + sqrt(delta) ) / pk0->m_a		SERA TOUJOURS POSITIVE ( avec pk0->m_a positive )
				//
				// R2 = (-pk0->m_v + sqrt(delta) ) / pk0->m_a
				//
				// Quand � R1 = (-pk0->m_v - sqrt(delta) ) / pk0->m_a
				// On voit bien qu'elle sera TOUJOURS n�gative pour les m�mes raisons ,
				// c'est � dire
				//						-pk0->m_v - sqrt(delta) < 0, TOUJOURS ! donc ...
				//
				// Donc on a bien une seule racine positive et donc possible: R+ !
				return (-pk0->m_v + sqrt( NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) ) / pk0->m_a );
			}
			else // pk0->m_a < 0.0f
			{
				// Il est possible que delta soit negatif ou null !
				// ------------------------------------------------
				// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
				// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
				return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)) / pk0->m_a);
			}
			* /
		}
		else if (pp->m_kin.m_j > 0.0f)
		{
			if (!pp0->m_kin.m_a)
			{
				p = (6.0f * pp0->m_kin.m_v) / pp->m_kin.m_j;
				q = (6.0f * (pp0->m_kin.m_s - s)) / pp->m_kin.m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				return pp0->m_kin.m_t + (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
			}
			else if (!pp->m_kin.m_a)
			{
				p = (6.0f * pp->m_kin.m_v) / pp->m_kin.m_j;
				q = (6.0f * (s - pp->m_kin.m_s)) / pp->m_kin.m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				return pp->m_kin.m_t - (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
			}
			else // pp0->m_kin.m_a != 0 ET pp->m_kin.m_a != 0
			{
				Nf32 j2 = NPOW2(pp->m_kin.m_j);
				Nf32 a02 = NPOW2(pp0->m_kin.m_a);
				p = (6.0f * pp0->m_kin.m_v) / pp->m_kin.m_j - (3.0f * a02) / j2;
				q = (2.0f * a02 * pp0->m_kin.m_a) / (j2 * pp->m_kin.m_j) - (6.0f * pp0->m_kin.m_a * pp0->m_kin.m_v) / j2 + (6.0f * (pp0->m_kin.m_s - s)) / pp->m_kin.m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				Nf32 r = (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
				return pp0->m_kin.m_t + r - pp0->m_kin.m_a / pp->m_kin.m_j;
			}
		}
		else // pp->m_kin.m_j < 0.0f
		{
			if (!pp0->m_kin.m_a)
			{
				p = (6.0f * pp0->m_kin.m_v) / pp->m_kin.m_j;
				q = (6.0f * (pp0->m_kin.m_s - s)) / pp->m_kin.m_j;
				return pp0->m_kin.m_t + 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3);
			}
			else if (!pp->m_kin.m_a)
			{
				p = (6.0f * pp->m_kin.m_v) / pp->m_kin.m_j;
				q = (6.0f * (s - pp->m_kin.m_s)) / pp->m_kin.m_j;
				return pp->m_kin.m_t - 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3);
			}
			else // pp0->m_kin.m_a != 0 ET pp->m_kin.m_a != 0
			{
				Nf32 j2 = NPOW2(pp->m_kin.m_j);
				Nf32 a02 = NPOW2(pp0->m_kin.m_a);
				p = (6.0f * pp0->m_kin.m_v) / pp->m_kin.m_j - (3.0f * a02) / j2;
				q = (2.0f * a02 * pp0->m_kin.m_a) / (j2 * pp->m_kin.m_j) - (6.0f * pp0->m_kin.m_a * pp0->m_kin.m_v) / j2 + (6.0f * (pp0->m_kin.m_s - s)) / pp->m_kin.m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f * NPOW3(p)) / 27.0f);
				Nf32 r = 2.0f * sqrt(-p / 3.0f) * cos((1.0f / 3.0f) * acos(((3.0f * q) / (2.0f * p)) * sqrt(3.0f / -p)) + 4.0f * NF32_PI_3);
				return pp0->m_kin.m_t + r - pp0->m_kin.m_a / pp->m_kin.m_j;
			}
		}
	}
	else // s >= m_s1
	{
		return m_t1;
	}
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule l'intervalle de temps ( start, end ) associ� � l'abscisse curviligne s.
 *
 *	@param	ptimerange est un pointeur sur une structure NINTERVALf32 qui sera 'remplie' par la fonction.
 *	@param	s est l'abscisse curviligne pour laquelle on veut connaitre l'intervalle de temps associ�.
 *	@return	Th�oriquement les fonctions NLTRAJECTORY_CHUNK.getTime() retournent une valeur parmis les 4 suivantes:
 * 				NLTRAJECTORY_CHUNK_GETTIME_UNIQUE		(= 0 )	s est associ� � une date unique t l'intervalle retourn� est [t,t]
 *			 	NLTRAJECTORY_CHUNK_GETTIME_INTERVAL		(= 1 )	s est associ� � un intervalle de temps, l'intervalle retourn� est [start,end]
 * 				NLTRAJECTORY_CHUNK_GETTIME_BEFORE		(=-1 )	s est situ�e 'avant' chunkT et n'est n'associ�e � aucune date de chunkT. l'intervalle retourn� est [thischunkT.m_t0,thischunkT.m_t0]
 * 				NLTRAJECTORY_CHUNK_GETTIME_AFTER		(=-2 )	s est situ�e 'apr�s' chunkT et n'est n'associ�e � aucune date de chunkT. l'intervalle retourn� est [thischunkT.m_t1,thischunkT.m_t1]
 *
 *			Plus sp�cifiquement, la fonction getTime de NLTRAJECTORY_CHUNK_T ne renverra jamais la valeur NLTRAJECTORY_CHUNK_GETTIME_INTERVAL car s
 *			ne peut �tre que soit avant le CHUNK_T (la fonction renvoie alors NLTRAJECTORY_CHUNK_GETTIME_BEFORE), soit apr�s (NLTRAJECTORY_CHUNK_GETTIME_AFTER), soit
 *			incluse dans l'intervalle couvert [m_s0,m_s1] par le chunk, dans ce dernier cas et parceque le chunkT EST un chunkT , S est associ� � une seule et unique valeur de t
 *			... dans ce dernier cas la valeur de retour sera donc NLTRAJECTORY_CHUNK_GETTIME_UNIQUE.
 */
// ------------------------------------------------------------------------------------------
/*
Ns32 NLTRAJECTORY_CHUNK_T::getTime(NINTERVALf32* ptimerange, const Nf32 s)
{
	if (s < m_s0)
	{
		ptimerange->start = ptimerange->end = m_t0;
		return NLTRAJECTORY_CHUNK_GETTIME_BEFORE;
	}
	else if( s>m_s1)
	{
		ptimerange->start = ptimerange->end = m_t1;
		return NLTRAJECTORY_CHUNK_GETTIME_AFTER;
	}
	else
	{
		ptimerange->start = ptimerange->end = getTime(s);
		return NLTRAJECTORY_CHUNK_GETTIME_UNIQUE;
	}
}

void NLTRAJECTORY_CHUNK_T::build(const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid, const NLPATH_GEOMETRY* ppathgeometry)
{
	// TODO !!! Attention la fonction de construction de chunk T (chunkt->build ) peux g�n�rer de NLTRJPOINT_DESC en fin de chunk dont l'abscisse
	// sera plus grande que la longueur totale du chemin !!! Cela est du � l'impr�cision potentielle du calcul des KIN, car pour eux, le s est le resultat
	// de l'integration de la vitesse, de l'acceleration et du jerk au cours du temps ...
	// Pour l'instant ce cas n'est pas g�r� !!! IL VA FALLOIR LE FAIRE !!!  Sinon plantage assur� avec la fonction NLPATH_GEOMETRY::getPathPoint


	NErrorIf(pkinsarray->ElementSize != sizeof(NLKIN), NERROR_WRONG_ARRAY_SIZE);
	NErrorIf(firstkinid >= pkinsarray->Size, NERROR_INDEX_OUTOFRANGE);
	NErrorIf(lastkinid >= pkinsarray->Size, NERROR_INDEX_OUTOFRANGE);
	NErrorIf(lastkinid <= firstkinid, NERROR_INCONSISTENT_VALUES);

	// ******************************* DEBUG
	NLKIN buff[64];
	NLKIN* pkin = NULL;
	Nu32 buffid = 0;
	for (Nu32 i = firstkinid; i < lastkinid; i++, buffid++)
	{
		pkin = (NLKIN*)NGetArrayPtr(pkinsarray, i);
		buff[buffid] = *pkin;
	}
	// ******************************* DEBUG

	NResizeArray(&m_trajectoryPointDescArray, 0, NULL, NLclearNLTrajectoryPointDescInArrayCallBack);

	// setup pk0, pk1
	NLKIN* pk = (NLKIN*)NGetArrayPtr(pkinsarray,firstkinid);// (NLKIN*)pkinsarray->pFirst;
	NLKIN* plastk = (NLKIN*)NGetArrayPtr(pkinsarray, lastkinid);// pk + pkinsarray->Size - 1;
	NLKIN* pprvk = pk;
	NErrorIf(plastk->m_s == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);

	NErrorIf(((NLPATH_POINT*)NGetLastArrayPtr(&ppathgeometry->m_pathPointsArray))->s < pk->m_s, NERROR_SYSTEM_GURU_MEDITATION);

	timeRange(pk->m_t, plastk->m_t);
	abscissaRange(pk->m_s, plastk->m_s);

	// Recherche de pp le point du chemin situ� juste apr�s le premier Kin ( = pk ).
	// Et donc par la m�me occasion la primitive SUR laquelle se trouve pk.
	NLPATH_PRIMITIVE* pprim = (NLPATH_PRIMITIVE*)ppathgeometry->m_primitivesArray.pFirst;
	NLPATH_POINT*	pp = (NLPATH_POINT*)(ppathgeometry->m_pathPointsArray.pFirst) + 1;
	NLPATH_POINT*	plastp = pp + ppathgeometry->m_pathPointsArray.Size - 2;

	while (pp->s <= pk->m_s) { pp++; pprim++; }
	NErrorIf(pp > plastp, NERROR_VALUE_OUTOFRANGE);
	Nf32			pps = pp->s;

	NLTRJPOINT_DESC		trjpointdsc;
	NLTRJPOINT_DESC*	pushed_trjpointdsc = NULL;

	while (1)
	{
		if (pk->m_s < pps)
		{
			trjpointdsc.m_kin			= *pk;
			trjpointdsc.m_pPathPoint1	= pp;
			trjpointdsc.m_pPrimitive	= pprim;
			pushed_trjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

			if (pk < plastk)
			{
				pprvk = pk;
				pk++;
			}
			else
				break;
		}
		else if (pk->m_s == pps)
		{
			trjpointdsc.m_kin			= *pk;
			trjpointdsc.m_pPathPoint1	= pp;
			trjpointdsc.m_pPrimitive	= pprim;
			pushed_trjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

			if (pp < plastp)
			{
				pp++;
				pprim++;
				pps = pp->s;
			}
			else
			{
				pps = NF32_MAX;
			}

			if (pk < plastk)
			{
				pprvk = pk;
				pk++;
			}
			else
				break;
		}
		else // pk->ms > pps
		{
			NErrorIf(pk->m_s <= pps, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pps == NF32_MAX, NERROR_INCONSISTENT_VALUES);

			trjpointdsc.m_kin.atS(pprvk, pk, pps);
			trjpointdsc.m_pPathPoint1 = pp;
			trjpointdsc.m_pPrimitive = pprim;
			pushed_trjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

			if (pp < plastp)
			{
				pp++;
				pprim++;
				pps = pp->s;
			}
			else
			{
				pps = NF32_MAX;
			}
		}
	}
}
*/