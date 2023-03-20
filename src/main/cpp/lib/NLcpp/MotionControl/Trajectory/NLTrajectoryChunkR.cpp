/*
#include "lib/NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPackGetPointPersistentResult.h"
#include "NLTrajectoryChunkR.h"



NLTRAJECTORY_CHUNK_R::NLTRAJECTORY_CHUNK_R()
{
	m_pathPoint.null();
	NSetupArray(&m_kinsArray, 0, sizeof(NLKIN));
}


NLTRAJECTORY_CHUNK_R::~NLTRAJECTORY_CHUNK_R()
{
	NClearArray(&m_kinsArray, NLclearKinInArrayCallBack);
}

void NLTRAJECTORY_CHUNK_R::selfDuplicate(void* palready_allocated_memory)
{
	NLTRAJECTORY_CHUNK_R* pduplicatedchunkt = new(palready_allocated_memory)NLTRAJECTORY_CHUNK_R;
	NCopyArray(&pduplicatedchunkt->m_kinsArray, &m_kinsArray);
	pduplicatedchunkt->m_pathPoint = m_pathPoint;
	pduplicatedchunkt->timeRange(m_t0, m_t1);
	pduplicatedchunkt->abscissaRange(m_s0, m_s1);
}

void NLTRAJECTORY_CHUNK_R::initializePersistentData(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* ppersist)
{
	ppersist->m_pChunkR_Kin = (NLKIN*)m_kinsArray.pFirst;
}

void NLTRAJECTORY_CHUNK_R::getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const NLPATH_GEOMETRY* ppathgeometry, const Nf32 t)
{
	presult->m_result.m_p = m_pathPoint.p;
	presult->m_result.m_k = NF32_MAX; // Modification sp�cifique au CHUNK_R qui decrit ce qui se passe autour d'un point fixe
	presult->m_result.m_u = m_pathPoint.u;

	NErrorIf(!NIsValidArrayElementPtr(&m_kinsArray, (NBYTE*)presult->m_pChunkR_Kin), NERROR_SYSTEM_GURU_MEDITATION);
	NLKIN* pkin = presult->m_pChunkR_Kin;
	if (t <= m_t0)
	{
		NErrorIf(pkin != (NLKIN*)m_kinsArray.pFirst, NERROR_INCONSISTENT_DATA);
		presult->m_result.m_kin = *pkin;
	}
	else // (t > m_t0 && t <= m_t1)
	{
		NErrorIf(!(t > m_t0 && t <= m_t1), NERROR_VALUE_OUTOFRANGE);
		while (pkin->m_t < t) { pkin++; }
		NLKIN* pk0 = pkin - 1;
		presult->m_result.m_kin.from(pk0, pkin->m_j, t - pk0->m_t);
	}
	presult->m_pChunkR_Kin = pkin;
}

Nu32 NLTRAJECTORY_CHUNK_R::read(FILE* pfile)
{
	return 0;
}

Nu32 NLTRAJECTORY_CHUNK_R::write(FILE* pfile)
{
	return 0;
}

void NLTRAJECTORY_CHUNK_R::draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
	if (ISFLAG_ON(p2docs->m_Flags, FLAG_NL2DOCS_DASHBOARD_MP_CHUNK_R))
	{
		NLKIN* pk = (NLKIN*)m_kinsArray.pFirst; pk++;
		for (Nu32 i = 1; i < m_kinsArray.Size; i++, pk++)
		{
			pk->draw(p2docs, pickpack, (pk - 1));
		}
	}
}
*/
/*
Nu32 NLTRAJECTORY_CHUNK_R::read(NLPATH_WORKBENCH* pwb)
{
	return 0;
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Renvoie la date t associ�e � l'abscisse s pass�e en parametre de fonction.
 *			Un chunkR est particulier puisqu'il d�crit ce qui se passe quand le robot
 *			reste en place sur un point. Il est alors soit � l'arr�t pour une dur�e m_t1 - m_t0
 *			soit en train de tourner sur lui-m�me.
 *			On notera que pour un chunk R on a systematiquement m_s0 = m_s1 !
 *
 *			Rmq: Pour un chunkR la version de getTime renvoyant un intervalle de temps est plus appropri�e.
 *
 *	@param	s abscisse curviligne dont on veut connaitre la date
 *
 *	@return la date t en secondes associ�e � l'abscisse s, dans le domaine couvert par le chunk.
 *			Ainsi, si s est inf�rieur ou �gale � m_s0 (= m_s1) alors la fonction retournera la valeur de m_t0
 *			et retournera m_t1 sinon.
 */
// ------------------------------------------------------------------------------------------
/*
Nf32 NLTRAJECTORY_CHUNK_R::getTime(const Nf32 s)
{

	return (s <= m_s0) ? m_t0 : m_t1;
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule l'intervalle de temps ( start, end ) associ� � l'abscisse curviligne s.
 *			Un chunkR est particulier puisqu'il d�crit ce qui se passe quand le robot
 *			reste en place sur un point. Il est alors soit � l'arr�t pour une dur�e m_t1 - m_t0
 *			soit en train de tourner sur lui-m�me.
 *			On notera que pour un chunk R on a systematiquement m_s0 = m_s1 !
 *
 *	@param	ptimerange est un pointeur sur une structure NINTERVALf32 qui sera 'remplie' par la fonction.
 *	@param	s est l'abscisse curviligne pour laquelle on veut connaitre l'intervalle de temps associ�.
 *	@return	Th�oriquement les fonctions NLTRAJECTORY_CHUNK.getTime() retournent une valeur parmis les 4 suivantes:
 * 				NLTRAJECTORY_CHUNK_GETTIME_UNIQUE		(= 0 )	s est associ� � une date unique t l'intervalle retourn� est [t,t]
 *			 	NLTRAJECTORY_CHUNK_GETTIME_INTERVAL		(= 1 )	s est associ� � un intervalle de temps, l'intervalle retourn� est [start,end]
 * 				NLTRAJECTORY_CHUNK_GETTIME_BEFORE		(=-1 )	s est situ�e 'avant' chunkT et n'est n'associ�e � aucune date de chunkT. l'intervalle retourn� est [thischunkT.m_t0,thischunkT.m_t0]
 * 				NLTRAJECTORY_CHUNK_GETTIME_AFTER		(=-2 )	s est situ�e 'apr�s' chunkT et n'est n'associ�e � aucune date de chunkT. l'intervalle retourn� est [thischunkT.m_t1,thischunkT.m_t1]
 *
 *			Plus sp�cifiquement, la fonction getTime de NLTRAJECTORY_CHUNK_R ne renverra jamais la valeur NLTRAJECTORY_CHUNK_GETTIME_UNIQUE car s
 *			ne peut �tre que soit avant le CHUNK_T (la fonction renvoie alors NLTRAJECTORY_CHUNK_GETTIME_BEFORE), soit apr�s (NLTRAJECTORY_CHUNK_GETTIME_AFTER), soit
 *			incluse dans l'intervalle couvert [m_s0,m_s1] par le chunk, dans ce dernier cas et parceque le chunkR EST un chunkR , S est associ� � toutes les valeurs de t
 *			incluses dans l'intervalle [m_t0,m_t1]
 *			et dans ce cas la valeur de retour sera donc NLTRAJECTORY_CHUNK_GETTIME_INTERVAL.
 */
// ------------------------------------------------------------------------------------------
/*
Ns32 NLTRAJECTORY_CHUNK_R::getTime(NINTERVALf32* ptimerange, const Nf32 s)
{
	if (s < m_s0)
	{
		ptimerange->start = ptimerange->end = m_t0;
		return NLTRAJECTORY_CHUNK_GETTIME_BEFORE;
	}
	else if (s > m_s0)
	{
		ptimerange->start = ptimerange->end = m_t1;
		return NLTRAJECTORY_CHUNK_GETTIME_AFTER;
	}
	else
	{
		ptimerange->start	= m_t0;
		ptimerange->end		= m_t1;
		return NLTRAJECTORY_CHUNK_GETTIME_INTERVAL;
	}
}
*/
/*
void NLTRAJECTORY_CHUNK_R::build(const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid, const NLPATH_POINT* ppathpoint)
{
	// Cette fonction doit faire une copie d'un certains nombre de KIN d'une array � une autre.
	// Cette copie va �tre effectu�e simplement comme la copie d'un bloc de m�moire. Cependant cela suppose que la structure KIN soit
	// compos�e de simples donn�es ( aucune allocation nested !!! ), ce qui est le cas au moment o� ces lignes sont �crites.
	// En debug, le check ci-dessous devrait nous garantir un CRASH au cas o� la structure NLKIN viendrait � �voluer.
	NErrorIf(NLclearKinInArrayCallBack != NULL, NERROR_SYSTEM_CHECK);
	NErrorIf(m_kinsArray.Size, NERROR_ARRAY_IS_NOT_EMPTY);

	// ******************************* DEBUG
	NLKIN buff[64];
	NLKIN* pkin = NULL;
	Nu32 buffid = 0;
	for (Nu32 i = firstkinid; i < lastkinid; i++,buffid ++)
	{
		pkin = (NLKIN*)NGetArrayPtr(pkinsarray, i);
		buff[buffid] = *pkin;
	}
	// ******************************* DEBUG


	NArrayBufferPushBack(&m_kinsArray, (void*)NGetArrayPtr(pkinsarray, firstkinid), lastkinid - firstkinid + 1);

	// Dans un chunkR les KIN r�f�renc�s ne modifient pas l'abscisse curviligne.
	// Ces KINs d�crivent ou des rotation sur place ou des pauses ! Ils repr�sentent les mouvements potentiels de la roue DROITE

	m_pathPoint	= *ppathpoint;
	m_s0 = m_s1 = m_pathPoint.s;
	m_t0 = ((NLKIN*)NGetFirstArrayPtr(&m_kinsArray))->m_t;
	m_t1 = ((NLKIN*)NGetLastArrayPtr(&m_kinsArray))->m_t;
}
*/
