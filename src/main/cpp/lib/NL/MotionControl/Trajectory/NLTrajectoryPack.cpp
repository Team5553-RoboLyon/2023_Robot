#include "lib/N/Miscellaneous/NString.h"
#include "lib/N/Core/NSafeConversion.h"
//#include "../../../N/Utilities/NUT_Logging.h"
#include "lib/NL/MotionControl/NLKin.h"
//#include "../NLPathWorkbench.h"
//#include "../../NLLogsChannels.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPostedMessage.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPackGetPointPersistentResult.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"


NLTRAJECTORY_PACK::NLTRAJECTORY_PACK()
{
	NSetupArray(&m_trajectoryPointDescArray, 0, sizeof(NLTRJPOINT_DESC));
	NSetupArray(&m_spotsArray, 0, sizeof(NLPATH_POINT));
	NSetupArray(&m_postedMessagesArray, 0, sizeof(NLTRJ_POSTED_MESSAGE));
	
	NIdentityMatrix(&m_matrix);
	m_dt = 0.0f;
	m_ds = 0.0f;
//	m_pChunk = NULL;
}

NLTRAJECTORY_PACK::~NLTRAJECTORY_PACK()
{
	NClearArray(&m_trajectoryPointDescArray, NLclearNLTrajectoryPointDescInArrayCallBack);
	NClearArray(&m_spotsArray, NLclearNLPathPointInArrayCallBack);
	NClearArray(&m_postedMessagesArray, NLclearNLPostedMessageInArrayCallBack);
}

#ifdef _NEDITOR
// ------------------------------------------------------------------------------------------
/**
 *	@brief	'Reset' complet du pack ( ... qui est alors pr�t pour un nouveau 'build' ).
 *			Toutes les donn�es inclues dans le pack sont effac�es, cependant la place m�moire occup�e par l'array des chunks n'est pas lib�r�e.
 *			Plus precisement: 
 *					La capacit� de l'array 'm_chunksArray' reste telle quelle est, mais la taille est red�finie � ZERO.
 *					La m�moire allou�e en plus par chaque chunk inclu dans 'm_chunksArray' est lib�r�e par l'appel � la m�thode 'NLclearNLTrajectoryChunkInArrayCallBack'
 *					pour chaque chunk.
 *
 */
 // ------------------------------------------------------------------------------------------
void NLTRAJECTORY_PACK::erase()
{
	NEraseArray(&m_trajectoryPointDescArray, NLclearNLTrajectoryPointDescInArrayCallBack);
	NEraseArray(&m_spotsArray, NLclearNLPathPointInArrayCallBack);
	NEraseArray(&m_postedMessagesArray, NLclearNLPostedMessageInArrayCallBack);


	m_pathGeometry.erase();
	NIdentityMatrix(&m_matrix);
	m_dt		= 0.0f;
	m_ds		= 0.0f;
	//m_pChunk	= NULL;
}

void NLTRAJECTORY_PACK::setup(const NLPATH* ppath, const NLDRIVETRAINSPECS* pdtspecs, const NLRAMSETE* pramsete, const NARRAY* pusedpkeysarray)
{
	// Le pack "int�gre toutes les donn�es n�c�ssaires" au bon fonctionnement et � la coh�rence du suivi de trajectoire.
	// Il y a donc copie d'un certains nombre de donn�es
	erase();

	m_pathGeometry				= ppath->m_geometry;	
	m_matrix					= ppath->m_matrix;
	
	Nu32 requiredspotsarraysize = 0;
	NLTRJKEY** pptk = (NLTRJKEY**)pusedpkeysarray->pFirst;
	for (Nu32 i = 0; i < pusedpkeysarray->Size; i++, pptk++)
	{
		requiredspotsarraysize += (*pptk)->SpotRequirement();
	}
	if (m_spotsArray.Capacity < requiredspotsarraysize)
		NIncreaseArrayCapacity(&m_spotsArray, requiredspotsarraysize - m_spotsArray.Capacity );

	m_driveTrainSpecifications	= *pdtspecs;
	m_ramsete					= *pramsete;

	// Au cas o� erase ne fasse plus son travail ...
	NErrorIf(m_dt, NERROR_INCONSISTENT_VALUES);
	NErrorIf(m_ds, NERROR_INCONSISTENT_VALUES);
	//NErrorIf(m_pChunk, NERROR_INCONSISTENT_VALUES);
}
#endif

Nu32 NLTRAJECTORY_PACK::read( FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLTRAJECTORY_PACK_HEADER	header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLTRAJECTORY_PACK_HEADER):
		if (fread(&header, sizeof(NLTRAJECTORY_PACK_HEADER), 1, pfile) != 1)
			return 0;
		m_dt					= header.m_dt;
		m_matrix				= header.m_matrix;

		if (!m_ramsete.read(pfile))
			return 0;

		if (!m_driveTrainSpecifications.read(pfile))
			return 0;
		
		if (!m_pathGeometry.read(pfile))
			return 0;

	/*
		m_dtSpecsLimits			= header.m_dtSpecsLimits ;
		m_dtSpecsAxleTrack		= header.m_dtSpecsAxleTrack;
		m_dtSpecsStaticFriction = header.m_dtSpecsStaticFriction;
		m_dtSpecsCenterOfMass	= header.m_dtSpecsCenterOfMass;
		*/
		// Recherche de potentiels pbs sur les arrays.
		// ('NIsArrayCorruptedOrInconsistent' se charge de faire un setup auto en cas de array full of ZEROS)
		if (NIsArrayCorruptedOrInconsistent(&m_spotsArray, &header.m_spotsArrayBounds, NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
			return 0;
		NResizeArray(&m_spotsArray, header.m_spotsArrayBounds.Size, NULL, NULL);

		if (m_spotsArray.Size)
		{
			if (fread(m_spotsArray.pFirst, m_spotsArray.ElementSize, m_spotsArray.Size, pfile) != m_spotsArray.Size)
				return 0;
		}

		if (NIsArrayCorruptedOrInconsistent(&m_trajectoryPointDescArray, &header.m_trajectoryPointDescArrayBounds, NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
			return 0;
		NResizeArray(&m_trajectoryPointDescArray, header.m_trajectoryPointDescArrayBounds.Size, NULL, NULL);

		if (m_trajectoryPointDescArray.Size)
		{
			NLTRJPOINT_DESC_HEADER		dsc;
			NLTRJPOINT_DESC* ptpdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
			for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, ptpdsc++)
			{
				if (fread(&dsc, sizeof(NLTRJPOINT_DESC_HEADER), 1, pfile) != 1)
					return 0;

				ptpdsc->m_kin	= dsc.m_kin;
				ptpdsc->m_flags = dsc.m_flags;
				if (ISFLAG_ON(ptpdsc->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
				{
					ptpdsc->m_pPathPoint1	= (NLPATH_POINT*)m_spotsArray.pFirst + dsc.m_KeyPoint1Idx;
					ptpdsc->m_pPrimitive	= NULL;
				}
				else
				{
					ptpdsc->m_pPathPoint1	= (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst + dsc.m_KeyPoint1Idx;
					ptpdsc->m_pPrimitive	= (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst + dsc.m_PrimitiveIdx;
				}
			}
		}

		if (NIsArrayCorruptedOrInconsistent(&m_postedMessagesArray, &header.m_postedMessagesArrayBounds, NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
			return 0;
		NResizeArray(&m_postedMessagesArray, header.m_postedMessagesArrayBounds.Size, NULL, NULL);

		if (m_postedMessagesArray.Size)
		{
			if (fread(m_postedMessagesArray.pFirst, m_postedMessagesArray.ElementSize, m_postedMessagesArray.Size, pfile) != m_postedMessagesArray.Size)
				return 0;
		}

		return 1;

	default:
		break;
	}
	return 0;
}

#ifdef _NEDITOR
Nu32 NLTRAJECTORY_PACK::read(NLPATH_WORKBENCH* pwb)
{
	// 'operator =' n'est pas d�fini pour NLTRAJECTORY_PACK donc on doit faire une partie du JOB � la main
	if (pwb && pwb->m_pTrajectory)
	{
		NLTRAJECTORY_PACK* ppack = pwb->m_pTrajectory->getPack();
		m_matrix		= ppack->m_matrix;
		m_dt			= ppack->m_dt;


		m_ramsete					= ppack->m_ramsete;
		m_driveTrainSpecifications	= ppack->m_driveTrainSpecifications;
		m_pathGeometry				= ppack->m_pathGeometry;
		
		NARRAYBOUNDS bounds;
		NGetArrayBounds(&bounds, &ppack->m_spotsArray);
		if (NIsArrayCorruptedOrInconsistent(&m_spotsArray, &bounds, NTRUE))
		{
			NErrorIf(1, NERROR_ARRAY_CORRUPTED);
			return 0;
		}
		else
		{
			NCopyArray(&m_spotsArray, &ppack->m_spotsArray);
		}

		NGetArrayBounds(&bounds,&ppack->m_trajectoryPointDescArray);
		if (NIsArrayCorruptedOrInconsistent(&m_trajectoryPointDescArray, &bounds,NTRUE))
		{
			NErrorIf(1, NERROR_ARRAY_CORRUPTED);
			return 0;
		}
		else
		{
			NResizeArray(&m_trajectoryPointDescArray, bounds.Size, NULL, NULL);
			NLTRJPOINT_DESC* psrc = (NLTRJPOINT_DESC*)ppack->m_trajectoryPointDescArray.pFirst;
			NLTRJPOINT_DESC* pdst = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;


#ifdef _NUT_LOGGING
			NUT_EnableLoggingChannel(LOGS_CHANNEL_PACK, TRUE);
			NUT_Logging(LOGS_CHANNEL_PACK, "+ Check Loading PACK DATA from PathWorkBench PASSE 1+\n");

			// Spot Array
			for (Nu32 i = 0; i < ppack->m_trajectoryPointDescArray.Size; i++, psrc++)
			{
				if (ISFLAG_ON(psrc->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
					NUT_Logging(LOGS_CHANNEL_PACK, "SPOT-NLTRJPOINT_DESC[%d]\tm_pPathPoint1[%X]\tm_pPrimitive[%X] \n",i,psrc->m_pPathPoint1,psrc->m_pPrimitive);
				else
					NUT_Logging(LOGS_CHANNEL_PACK, "TRVL-NLTRJPOINT_DESC[%d]\tm_pPathPoint1[%X]\tm_pPrimitive[%X] \n", i, psrc->m_pPathPoint1, psrc->m_pPrimitive);
			}
#endif



			// Copie �l�ment par �l�ment:
			psrc = (NLTRJPOINT_DESC*)ppack->m_trajectoryPointDescArray.pFirst;
			pdst = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
			for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, psrc++, pdst++)
			{
				pdst->m_flags		= psrc->m_flags;
				pdst->m_kin			= psrc->m_kin;
				if (ISFLAG_ON(pdst->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
				{
					NErrorIf(psrc->m_pPrimitive, NERROR_NON_NULL_POINTER);
					pdst->m_pPathPoint1 = (NLPATH_POINT*)m_spotsArray.pFirst + (psrc->m_pPathPoint1 - (NLPATH_POINT*)ppack->m_spotsArray.pFirst);
					pdst->m_pPrimitive = NULL;
				}
				else
				{
					NErrorIf(!psrc->m_pPrimitive, NERROR_NON_NULL_POINTER);
					pdst->m_pPathPoint1 = (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst + (psrc->m_pPathPoint1 - (NLPATH_POINT*)ppack->m_pathGeometry.m_pathPointsArray.pFirst);
					pdst->m_pPrimitive = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst + (psrc->m_pPrimitive - (NLPATH_PRIMITIVE*)ppack->m_pathGeometry.m_primitivesArray.pFirst);
				}
			}
		}
		#ifdef _NUT_LOGGING
		NUT_EnableLoggingChannel(LOGS_CHANNEL_PACK, TRUE);
		NUT_Logging(LOGS_CHANNEL_PACK, "+ Check Loading PACK DATA from PathWorkBench +\n");
		
		// Spot Array
		NLPATH_POINT* p0, * p1;
		p0 = (NLPATH_POINT*)m_spotsArray.pFirst;
		p1 = (NLPATH_POINT*)ppack->m_spotsArray.pFirst;
		for (Nu32 i = 0; i < m_spotsArray.Size; i++, p0++, p1++)
		{
			NUT_Logging(LOGS_CHANNEL_PACK, "p[%X] (\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f) \n",	p0,p0->s,p0->p.x,p0->p.y,p0->u.x,p0->u.y,p0->k);
			NUT_Logging(LOGS_CHANNEL_PACK, "p[%X] (\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f) \n",	p1, p1->s, p1->p.x, p1->p.y, p1->u.x, p1->u.y, p1->k);
		}
		#endif

		// PostedMessages
		NGetArrayBounds(&bounds, &ppack->m_postedMessagesArray);
		if (NIsArrayCorruptedOrInconsistent(&m_postedMessagesArray, &bounds, NTRUE))
		{
			NErrorIf(1, NERROR_ARRAY_CORRUPTED);
			return 0;
		}
		else
		{
			NCopyArray(&m_postedMessagesArray, &ppack->m_postedMessagesArray);
		}

		return 1;

	}
	return 0;
}
#endif
Nu32 NLTRAJECTORY_PACK::write(FILE* pfile)
{
	// 1) �criture Version
	Nu32	version_u32 = VERSION_NLTRAJECTORY_PACK_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) �criture Header
	NLTRAJECTORY_PACK_HEADER	header;
	header.m_dt							= m_dt;
	header.m_matrix						= m_matrix;
	/*
	header.m_dtSpecsLimits				= m_dtSpecsLimits;
	header.m_dtSpecsAxleTrack			= m_dtSpecsAxleTrack;
	header.m_dtSpecsStaticFriction		= m_dtSpecsStaticFriction;
	header.m_dtSpecsCenterOfMass		= m_dtSpecsCenterOfMass;
	*/
	//header.m_keyStatesArraySize = m_keyStatesArray.Size;
	NGetArrayBounds(&header.m_spotsArrayBounds, &m_spotsArray);
	NGetArrayBounds(&header.m_trajectoryPointDescArrayBounds, &m_trajectoryPointDescArray);
	NGetArrayBounds(&header.m_postedMessagesArrayBounds, &m_postedMessagesArray);
	if (fwrite(&header, sizeof(NLTRAJECTORY_PACK_HEADER), 1, pfile) != 1)
		return 0;

	if (m_ramsete.write(pfile) != 1)
		return 0;

	if (m_driveTrainSpecifications.write(pfile) != 1)
		return 0;

	if (m_pathGeometry.write(pfile) != 1)
		return 0;

	if (m_spotsArray.Size)
	{
		if (fwrite(m_spotsArray.pFirst, m_spotsArray.ElementSize, m_spotsArray.Size, pfile) != m_spotsArray.Size)
			return 0;
	}

	// 3) �criture m_trajectoryPointDescArray � la main:
	if (m_trajectoryPointDescArray.Size)
	{
		NLTRJPOINT_DESC_HEADER		dsc;
		NLTRJPOINT_DESC* ptpdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
		for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, ptpdsc++)
		{
			dsc.m_kin			= ptpdsc->m_kin;
			dsc.m_flags			= ptpdsc->m_flags;
			if (ISFLAG_ON(dsc.m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
			{
				dsc.m_KeyPoint1Idx = ptpdsc->m_pPathPoint1 - (NLPATH_POINT*)m_spotsArray.pFirst;
				dsc.m_PrimitiveIdx = NVOID;
			}
			else
			{
				dsc.m_KeyPoint1Idx = ptpdsc->m_pPathPoint1 - (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst;
				dsc.m_PrimitiveIdx = ptpdsc->m_pPrimitive - (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;
			}

			if (fwrite(&dsc,sizeof(NLTRJPOINT_DESC_HEADER),1, pfile) != 1)
				return 0;
		}
	}

	// 4) PostedMessages:
	if (m_postedMessagesArray.Size)
	{
		if (fwrite(m_postedMessagesArray.pFirst, m_postedMessagesArray.ElementSize, m_postedMessagesArray.Size, pfile) != m_postedMessagesArray.Size)
			return 0;
	}
	return 1;
}
Nu32 NLTRAJECTORY_PACK::load(const Nchar* pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	*
	*  Check extension
	*
	*/
	if (!NStrCheckEnd(pfilename, EXTENSION_NLTRAJECTORY_PACK_BIN))
		return 0;

	// 0) Ouverture du fichier en lecture
	FILE* pfile = fopen(pfilename, "rb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Lecture Signature
	Nu32	_u32;
	if (fread(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}
	if (!NSIGNATURECMP(_u32, SIGNATURE_NLTRAJECTORY_PACK_BIN))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_UNKNOWN_SIGNATURE);
		return 0;
	}
	// 2) Lecture pack:
	if (!read(pfile))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}

	fclose(pfile);
	return 1;
}
Nu32 NLTRAJECTORY_PACK::save(const Nchar* pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(pfilename, EXTENSION_NLTRAJECTORY_PACK_BIN))
		return 0;

	// 0) Ouverture du fichier en ecriture
	FILE* pfile = fopen(pfilename, "wb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Ecriture Signature
	Nu32	_u32 = SIGNATURE_NLTRAJECTORY_PACK_BIN;
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}
	// 2) Ecriture pack:
	if (!write(pfile))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_WRITE_ERROR);
		return 0;
	}
	fclose(pfile);
	return 1;

}
/*
void NLTRAJECTORY_PACK::storeRelevantParameters(const NLDRIVETRAINSPECS* pdtspecs)
{
	m_dtSpecsAxleTrack		= pdtspecs->m_axleTrack;
	m_dtSpecsCenterOfMass	= pdtspecs->m_centerOfMass;
	m_dtSpecsStaticFriction = pdtspecs->m_staticFriction;
	m_dtSpecsLimits			= pdtspecs->m_limits;
}

Nu32 NLTRAJECTORY_PACK::checkRelevantParameters(const NLDRIVETRAINSPECS* pdtspecs)
{
	Nu32 err = 0;

	if (m_dtSpecsAxleTrack != pdtspecs->m_axleTrack)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_AXLETRACK);
	if (m_dtSpecsCenterOfMass.x != pdtspecs->m_centerOfMass.x)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_X);
	if (m_dtSpecsCenterOfMass.x != pdtspecs->m_centerOfMass.y)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_Y);
	if (m_dtSpecsCenterOfMass.x != pdtspecs->m_centerOfMass.z)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_Z);
	if (m_dtSpecsStaticFriction != pdtspecs->m_staticFriction)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_STATICFRICTION);
	if (m_dtSpecsLimits.m_v != pdtspecs->m_limits.m_v)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_V);
	if (m_dtSpecsLimits.m_a != pdtspecs->m_limits.m_a)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_A);
	if (m_dtSpecsLimits.m_j != pdtspecs->m_limits.m_j)
		FLAG_ON(err, FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_J);

	return err;
}
*/
/*
NLTRAJECTORY_POINT* NLTRAJECTORY_PACK::getState(NLTRAJECTORY_POINT * pstate, const Nf32 t)
{

	NLTRJPOINT_DESC	*pks0,*pks1;

	if (t <= 0.0f)
	{
		pks0 = (NLTRJPOINT_DESC*)m_chunksArray.pFirst;
		NLPATH_POINT* pkp0 = (pks0->m_pPathPoint1 - 1);
		
		pstate->m_kin	= pks0->m_kin;
		pstate->m_p		= pkp0->p;
		pstate->m_u		= pkp0->u;
		pstate->m_k		= pkp0->k;
	}
	else if (t < m_dt)
	{
		pks1 = (NLTRJPOINT_DESC*)m_chunksArray.pFirst;
		while (pks1->m_kin.m_t < t) { pks1++; }
		pks0 = pks1 -1;

		pstate->m_kin.from(&pks0->m_kin, pks1->m_kin.m_j, t - pks0->m_kin.m_t);
		
		NLPATH_POINT p;
		m_pathGeometry.getPathPoint(&p, pks1->m_pPathPoint1, pks1->m_pPrimitive, pstate->m_kin.m_s);

		pstate->m_p		= p.p;
		pstate->m_u		= p.u;
		pstate->m_k		= p.k;
	}
	else // t >= m_dt
	{
		pks1 = (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_keyStatesArray);
		pstate->m_kin				= pks1->m_kin;
		NLPATH_POINT *pkp1			= pks1->m_pPathPoint1;
		pstate->m_position			= pkp1->p;
		pstate->m_tangent			= pkp1->u;
		pstate->m_localCurvature	= pkp1->k;
	}
	
	return pstate;
}
*/
/*
void NLTRAJECTORY_PACK::initializePersistentData()
{
	m_pChunk = (NLTRAJECTORY_CHUNK*)m_chunksArray.pFirst;
	m_pChunk->initializePersistentData();
}
*/
//NLTRAJECTORY_GETPOINT_RESULT

void NLTRAJECTORY_PACK::getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const Nf32 t)
{
	NErrorIf(!NIsValidArrayElementPtr(&m_trajectoryPointDescArray, (NBYTE*)presult->m_pTrjPointDsc), NERROR_SYSTEM_GURU_MEDITATION);
	NErrorIf(t < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
	NErrorIf(t > m_dt, NERROR_SYSTEM_GURU_MEDITATION);

	NLTRJPOINT_DESC* ptpdsc		= presult->m_pTrjPointDsc;
	NLTRJPOINT_DESC* ptpdsc0	= NULL;
	while (ptpdsc->m_kin.m_t < t) { ptpdsc++; }
	NErrorIf(ptpdsc > (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_trajectoryPointDescArray), NERROR_SYSTEM_GURU_MEDITATION);
	ptpdsc0 = ptpdsc - 1;
	presult->m_result.m_kin.from(&ptpdsc0->m_kin, ptpdsc->m_kin.m_j, t - ptpdsc0->m_kin.m_t);
	
	NErrorIf(ptpdsc->m_pPrimitive && (!ptpdsc0->m_pPrimitive), NERROR_SYSTEM_CHECK);
	NErrorIf(ptpdsc0->m_pPrimitive && (!ptpdsc->m_pPrimitive), NERROR_SYSTEM_CHECK);


	// Si m_pPrimitive est d�finie c'est que nous sommes en face d'un NLTRJPOINT_DESC de KTYPE TRAVELLING !
	if(ptpdsc->m_pPrimitive)
		m_pathGeometry.getPathPoint((NLPATH_POINT*)presult, ptpdsc->m_pPathPoint1, ptpdsc->m_pPrimitive, presult->m_result.m_kin.m_s);
	// Sinon de KTYPE SPOT ou BACK_AND_FORTH 
	else
	{
		presult->m_result.m_p = ptpdsc->m_pPathPoint1->p;
		presult->m_result.m_k = ptpdsc->m_pPathPoint1->k;
		presult->m_result.m_u = ptpdsc->m_pPathPoint1->u;
	}
	presult->m_pTrjPointDsc = ptpdsc;
}
/*
void NLTRAJECTORY_PACK::getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const Nf32 t)
{
	NErrorIf(!NIsValidArrayElementPtr(&m_chunksArray, (NBYTE*)presult->m_pChunk), NERROR_SYSTEM_GURU_MEDITATION);

	Nf32 tt = NMIN(t, m_dt);
	NLTRAJECTORY_CHUNK* pchunk = presult->m_pChunk;
	while (pchunk->m_t1 < tt)
	{
		NErrorIf(pchunk == (NLTRAJECTORY_CHUNK*)NGetLastArrayPtr(&m_chunksArray), NERROR_SYSTEM_GURU_MEDITATION);
		pchunk = (NLTRAJECTORY_CHUNK*)((NBYTE*)pchunk + m_chunksArray.ElementSize);
		pchunk->initializePersistentData(presult);
	}
	pchunk->getPoint(presult, &m_pathGeometry, tt);
	presult->m_pChunk = pchunk;
}
*/

/*
NLTRAJECTORY_CHUNK_T* NLTRAJECTORY_PACK::AllocChunkT(const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid)
{


	NErrorIf(!m_pathGeometry.m_ds, NERROR_NULL_VALUE);						// ? |La path geometrie doit �tre valide et d�clar�e au pr�alable ! 
	NErrorIf(!m_pathGeometry.m_pathPointsArray.Size, NERROR_NULL_VALUE);		// ? |cf NLTRAJECTORY_PACK.setup(...)
	NErrorIf(!m_pathGeometry.m_primitivesArray.Size, NERROR_NULL_VALUE);	// ? |

	NLTRAJECTORY_CHUNK_T *pchunk =  new(NArrayAllocBack(&m_chunksArray)) NLTRAJECTORY_CHUNK_T;
	pchunk->build(pkinsarray, firstkinid, lastkinid, &m_pathGeometry);

	// Check de coh�rence ...
#ifdef _DEBUG
	if (m_chunksArray.Size > 1)
	{
		NLTRAJECTORY_CHUNK* ppreviouschunk = (NLTRAJECTORY_CHUNK*)NGetArrayPtr(&m_chunksArray, m_chunksArray.Size - 2);
		//NErrorIf(ppreviouschunk->m_t1 != pchunk->m_t0, NERROR_INCONSISTENT_VALUES);
		//NErrorIf(ppreviouschunk->m_s1 != pchunk->m_s0, NERROR_INCONSISTENT_VALUES);
	}
#endif

	// Le chunk venant d'�tre ajout� il est le dernier. On se base sur ses donn�es t et s de "sortie" pour d�finir la dur�e
	// et la longueur totale couverte par le Pack
	m_dt = pchunk->m_t1;
	m_ds = pchunk->m_s1;
	return pchunk;
}
*/
/*
NLTRAJECTORY_CHUNK_R* NLTRAJECTORY_PACK::AllocChunkR(const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid, const Nf32 s)
{
	NErrorIf(!m_pathGeometry.m_ds, NERROR_NULL_VALUE);						// ? |La path geometrie doit �tre valide et d�clar�e au pr�alable ! 
	NErrorIf(!m_pathGeometry.m_pathPointsArray.Size, NERROR_NULL_VALUE);		// ? |cf NLTRAJECTORY_PACK.setup(...)
	NErrorIf(!m_pathGeometry.m_primitivesArray.Size, NERROR_NULL_VALUE);	// ? |

	NLPATH_POINT pathpoint;
	m_pathGeometry.getPathPoint(&pathpoint, s);
	NLTRAJECTORY_CHUNK_R* pchunk = new(NArrayAllocBack(&m_chunksArray)) NLTRAJECTORY_CHUNK_R;
	pchunk->build(pkinsarray, firstkinid, lastkinid, &pathpoint);
	// Check de coh�rence ...
#ifdef _DEBUG
	if (m_chunksArray.Size > 1)
	{
		NLTRAJECTORY_CHUNK* ppreviouschunk = (NLTRAJECTORY_CHUNK*)NGetArrayPtr(&m_chunksArray, m_chunksArray.Size - 2);
		//NErrorIf(ppreviouschunk->m_t1 != pchunk->m_t0, NERROR_INCONSISTENT_VALUES);
		//NErrorIf(ppreviouschunk->m_s1 != pchunk->m_s0, NERROR_INCONSISTENT_VALUES);
	}
#endif
	
	// Le chunk venant d'�tre ajout� il est le dernier. On se base sur ses donn�es t et s de "sortie" pour d�finir la dur�e
	// et la longueur totale couverte par le Pack
	m_dt = pchunk->m_t1;
	m_ds = pchunk->m_s1;
	return pchunk;
}
*/
#ifdef _NEDITOR
NLTRJPOINT_DESC_CFG NLTRAJECTORY_PACK::buildTravelingChunk(const NLTRJPOINT_DESC_CFG cfg, const NARRAY* pkinsarray)
{
	NErrorIf(!IS_NLTRJPOINT_DESC_KTYPE_TRAVELING(cfg), NERROR_INCONSISTENT_PARAMETERS);

	// TODO !!! Attention cette fonction peux g�n�rer un NLTRJPOINT_DESC en fin de chunk dont l'abscisse
	// sera plus grande que la longueur totale du chemin !!! Cela est du � l'impr�cision potentielle du calcul des KIN, car pour eux, le s est le resultat
	// de l'integration de la vitesse, de l'acceleration et du jerk au cours du temps ...
	// Pour l'instant ce cas n'est pas g�r� !!! IL VA FALLOIR LE FAIRE !!!  Sinon plantage assur� avec la fonction NLPATH_GEOMETRY::getPathPoint
	NErrorIf(pkinsarray->ElementSize != sizeof(NLKIN), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(m_pathGeometry.m_pathPointsArray.Size < 2, NERROR_INCONSISTENT_VALUES);
	
	if (!IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(cfg))
	{
		// On retire toute ref. relatives � la cr�ation de Kins.
		return MAKE_NLTRJPOINT_DESC_CFG(0, NFALSE, cfg);
	}


	//	0)	* Preparation
	//		*	pointeurs sur Kin
	NLKIN* pk = (NLKIN*)pkinsarray->pFirst + GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg);
	NLKIN* pprvk = pk;
	NLKIN* plastk = (NLKIN*)pkinsarray->pFirst + pkinsarray->Size - 1;
	NErrorIf(!pk || !plastk, NERROR_NULL_POINTER);

	// ????????NErrorIf(plastk->m_s == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
	//		*	pointeurs sur �l�ments constituant la g�om�trie du chemin
	NLPATH_PRIMITIVE* pprim;
	NLPATH_POINT* pp;
	NLPATH_POINT* plastp = ((NLPATH_POINT*)(m_pathGeometry.m_pathPointsArray.pFirst)) + m_pathGeometry.m_pathPointsArray.Size - 1;
	// ????????NErrorIf(((NLPATH_POINT*)NGetLastArrayPtr(&m_pathGeometry.m_pathPointsArray))->s < plastk->m_s, NERROR_SYSTEM_GURU_MEDITATION);

	//		*	pointeur sur point de trajectoire
	NLTRJPOINT_DESC* ptrjpointdsc = (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_trajectoryPointDescArray);

	//	1)	* Initialisation
	//		*	
	if (!ptrjpointdsc)
	{
		// L'array des "trajectoryPointDesc(ription)" est vide !
		// Ce NLTRJPOINT_DESC est le premier � y �tre ins�r� !
		// Il DOIT d�marrrer par un premier Kin dont l'abscisse curviligne est 0 ! 
		// ... et bien s�r, il pointera sur la premi�re primitive du chemin associ� et sur le second pathpoint.
		// ( c'est � dire le 2eme de la paire d�finition le d�but et la fin de la primitive )
		NErrorIf(pk->m_s != 0.0f, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pk->m_v != 0.0f, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pk->m_a != 0.0f, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pk->m_j != 0.0f, NERROR_INCONSISTENT_VALUES);
		pprim = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;
		pp = (NLPATH_POINT*)(m_pathGeometry.m_pathPointsArray.pFirst) + 1;
	}
	else
	{
		// Il y a d�j� un "trajectoryPointDesc(ription)" !

		// Recherche de la primitive et du point
		if (ptrjpointdsc->m_pPrimitive)
		{
			// Chaque nouveau kin ins�r� est cens� contribuer � un d�placement du d�but � la fin du chemin, du coup, tant que le trjpointdesc est de type travelling,
			// ce qui est le cas si m_pPrimitive est non null, l'abscisse curviligne de chaque nouveau kin ins�r� ...
			// ... doit �tre sup�rieure ou �gale � la pr�c�dente, jamais inf�rieure !
			NErrorIf(pk->m_s < ptrjpointdsc->m_kin.m_s, NERROR_INCONSISTENT_VALUES);
			pprim = ptrjpointdsc->m_pPrimitive;
			pp = ptrjpointdsc->m_pPathPoint1;
		}
		else
		{
			pprim	= (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;
			pp		= (NLPATH_POINT*)(m_pathGeometry.m_pathPointsArray.pFirst) + 1;
			while ((pk->m_s > pp->s)&&(pp<plastp)) { pp++; pprim++; }
		}
	}


	//	2)	* Construction des NLTRJPOINT_DESC
	//
	NErrorIf(!IS_NLTRJPOINT_DESC_KTYPE_TRAVELING(cfg), NERROR_INCONSISTENT_FLAGS);
	NErrorIf(IS_NLTRJPOINT_DESC_KTYPE_NULL(cfg), NERROR_INCONSISTENT_FLAGS);
	Nf32			 pps = pp->s;
	NLTRJPOINT_DESC  trjpointdsc;

	trjpointdsc.m_flags = cfg & MASK_NLTRJPOINT_DESC_CONFIG;

	while (1)
	{
		if (pk->m_s < pps)
		{
			//trjpointdsc.m_flags ...	is already setup
			trjpointdsc.m_kin = *pk;
			trjpointdsc.m_pPathPoint1 = pp;
			trjpointdsc.m_pPrimitive = pprim;
			ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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
			//trjpointdsc.m_flags ...	is already setup
			trjpointdsc.m_kin = *pk;
			trjpointdsc.m_pPathPoint1 = pp;
			trjpointdsc.m_pPrimitive = pprim;
			ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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

			//trjpointdsc.m_flags ...	is already setup
			trjpointdsc.m_kin.atS(pprvk, pk, pps);
			trjpointdsc.m_pPathPoint1 = pp;
			trjpointdsc.m_pPrimitive = pprim;
			ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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
	// On retire toute ref. relatives � la cr�ation de Kins.
	return MAKE_NLTRJPOINT_DESC_CFG(0, NFALSE, cfg);
}


NLTRJPOINT_DESC_CFG NLTRAJECTORY_PACK::buildSpotChunk(const NLTRJPOINT_DESC_CFG cfg, const Nf32 s, const NARRAY* pkinsarray)
{
	if (IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(cfg))
	{
		NErrorIf(IS_NLTRJPOINT_DESC_KTYPE_TRAVELING(cfg) || IS_NLTRJPOINT_DESC_KTYPE_NULL(cfg), NERROR_INCONSISTENT_FLAGS);
		NErrorIf(!pkinsarray->Size, NERROR_ARRAY_IS_EMPTY);
		NErrorIf(GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg) >= pkinsarray->Size, NERROR_INCONSISTENT_VALUES);

		NLKIN* pk = (NLKIN*)pkinsarray->pFirst + GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg);
		NLKIN* plastk = (NLKIN*)pkinsarray->pFirst + pkinsarray->Size - 1;
		NLPATH_POINT* pspot = NULL;
		NLTRJPOINT_DESC  trjpointdsc;

		// Creation du "Point" fixe sur lequel les TrajectoryPointDesc vont se construire
		pspot = (NLPATH_POINT*)NGetLastArrayPtr(&m_spotsArray);
		if (!pspot || pspot->s != s)
		{
			NErrorIf(m_spotsArray.Size == m_spotsArray.Capacity, NERROR_ARRAY_REALLOCATION_FAILURE); // Normalement 
			pspot = (NLPATH_POINT*)NArrayAllocBack(&m_spotsArray);
			m_pathGeometry.getPathPoint(pspot, s);
		}

		while (pk <= plastk)
		{
			trjpointdsc.m_flags = cfg & MASK_NLTRJPOINT_DESC_CONFIG;
			trjpointdsc.m_kin = *pk;
			trjpointdsc.m_pPathPoint1 = pspot;
			trjpointdsc.m_pPrimitive = NULL;
			NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);
			pk++;
		}
	}
	// On retire toute ref. relatives � la cr�ation de Kins.
	return MAKE_NLTRJPOINT_DESC_CFG(0, NFALSE, cfg);
}


void NLTRAJECTORY_PACK::postFirstActionMessage()
{
	NErrorIf(m_postedMessagesArray.Size != 0, NERROR_SYSTEM_CHECK);
	
	NLTRJ_POSTED_MESSAGE* pm = (NLTRJ_POSTED_MESSAGE*)NArrayAllocBack(&m_postedMessagesArray);
	pm->m_id		= NVOID;
	pm->m_timeStamp = 0.0f;
	pm->m_user		= 0;
}

void NLTRAJECTORY_PACK::postLastActionMessage()
{
	#ifdef _DEBUG
	{
		NLTRJ_POSTED_MESSAGE* pm = (NLTRJ_POSTED_MESSAGE*)m_postedMessagesArray.pFirst;
		for (Nu32 i = 0; i < m_postedMessagesArray.Size; i++, pm++)
			NErrorIf(pm->m_timeStamp == NF32_MAX, NERROR_SYSTEM_CHECK); // Il y a d�j� un Last Message ?!!!
	}
	#endif
	
	NLTRJ_POSTED_MESSAGE* pm = (NLTRJ_POSTED_MESSAGE*)NArrayAllocBack(&m_postedMessagesArray);
	pm->m_id		= NVOID;
	pm->m_timeStamp = NF32_MAX;
	pm->m_user		= 0;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Cr�e un posted message dans l'arraydes posted messages.
 *
 *	@param	paction est un pointeur sur l'action dont le message sera issu.
 * 	@param	nxtkey_timestart est la date de d�marrage de la clef suivante. Le timestamp de l'action doit �tre inf. � cette valeur.
 *			Cela permet d'�viter le chevauchement des actions issues de 2 clefs diff�rentes. 
 */
 // ------------------------------------------------------------------------------------------
void NLTRAJECTORY_PACK::postActionMessage(const NLTRJACTION* paction ,const Nf32 nxtkey_timestart)
{
	NErrorIf(!paction, NERROR_NULL_POINTER);
	#ifdef _DEBUG
	{
		NLTRJ_POSTED_MESSAGE* dbg_ppm = (NLTRJ_POSTED_MESSAGE*)NGetLastArrayPtr(&m_postedMessagesArray);
		NErrorIf(dbg_ppm->m_timeStamp == NF32_MAX, NERROR_SYSTEM_CHECK); // Il y a un Last Message ?!!!
	}
	#endif
	
	if (IS_NLTRJACTION_ENABLE(paction->m_core.m_flags) && (paction->m_sendMessage.m_timeStamp<nxtkey_timestart))
	{
		#ifdef _DEBUG
		NLTRJ_POSTED_MESSAGE* dbg_ppm = (NLTRJ_POSTED_MESSAGE*)NGetLastArrayPtr(&m_postedMessagesArray);
		if (dbg_ppm)
			if (dbg_ppm->m_timeStamp > paction->m_core.m_timeStamp)
				NErrorIf(1, NERROR_SYSTEM_CHECK);
		#endif	

		NLTRJ_POSTED_MESSAGE* pm  = NULL;
		switch(paction->m_core.m_id)
		{
			case NLTRJACTION_ID_SEND_MESSAGE:
				pm = (NLTRJ_POSTED_MESSAGE*)NArrayAllocBack(&m_postedMessagesArray);
				pm->m_id		= paction->m_sendMessage.m_message;
				pm->m_timeStamp = paction->m_sendMessage.m_timeStamp;
				pm->m_user		= 0;
				break;

			default:
				break;
		}
	}
}


Nf32 NLTRAJECTORY_PACK::getTime(const Nf32 s)
{
	if (s <= 0.0f)
		return 0.0f;
	else if (s >= m_ds)
		return m_dt;
	else // 0 < s < m_ds
	{
		NLTRJPOINT_DESC* pp = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
		Nf32	p, q, sqrt_dlt;

		while (pp->m_kin.m_s < s) { pp++; };
		NLTRJPOINT_DESC* pp0 = pp - 1;

		// Normalement, la construction de l'array des NLTRJPOINT_DESC + le test effectu� dans le while devrait syst�matiquement "atterrir" sur un
		// NLTRJPOINT_DESC de KTYPE Travelling !
		//	s0		s1		s2		s3		s4		s5		s6		s7			Les points pg et ph sont de KTYPE SPot, les autres Travelling		
		//	+.......+.......+.......+.......+.......+.......+.......+... . . .	Si on a s tel que s4 < s <= s5
		//	pa		pb		pc		pd		pe		pf		pj		pk			on atterri sur pp = pf (s5 ) qui est de Type Travelling
		//																		pg, ph et pi sont "inaccessibles" car "masqu�" par pf
		//											+							pp0 = pp - 1 tombe sur pe(s4) qui est travelling aussi.
		//											pg							Si on a s tel que s5 < s <= s6
		//																		on atterri sur pp = pj (s6 ) qui est de Type Travelling
		//											+							pp0 = pp - 1 tombe sur pi(s5) qui doit etre /est travelling aussi.	
		//											ph							la contruction de l'array garanti que toute chunk de points SPOT
		//																		se termine pas un point de KTYPE travelling � la m�me abscisse.
		//											+
		//											pi
		NErrorIf(!IS_NLTRAJECTORY_POINT_DESC_TRAVELING(pp->m_flags), NERROR_SYSTEM_GURU_MEDITATION);	
		NErrorIf(!IS_NLTRAJECTORY_POINT_DESC_TRAVELING(pp0->m_flags), NERROR_SYSTEM_GURU_MEDITATION);

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
				// ... Quelques explications de +:
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
				/*
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
						return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f * pk0->m_a * (pk0->m_s - s)) / pk0->m_a);
					}
					else // pk0->m_a < 0.0f
					{
						// Il est possible que delta soit negatif ou null !
						// ------------------------------------------------
						// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
						// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
						return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f * pk0->m_a * (pk0->m_s - s)) / pk0->m_a);
					}

					Donc que pk0->m_a soit positif, null ou negatif, on calcule / retourne toujours de la m�me mani�re...
				*/

				return pp0->m_kin.m_t + (-pp0->m_kin.m_v + sqrt(NPOW2(pp0->m_kin.m_v) - 2.0f * pp0->m_kin.m_a * (pp0->m_kin.m_s - s))) / pp0->m_kin.m_a;
			}

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
}

Ns32 NLTRAJECTORY_PACK::getTime(NINTERVALf32* ptimerange, const Nf32 s)
{
	if (s < 0.0f)
	{
		ptimerange->start = ptimerange->range = 0.0f;
		return NLTRAJECTORYPACK_GETTIME_BEFORE;
	}
	else if (s > m_ds)
	{
		ptimerange->start = m_dt;
		ptimerange->range = 0.0f;
		return NLTRAJECTORYPACK_GETTIME_AFTER;
	}
	else // 0 <= s <= m_ds
	{
		NLTRJPOINT_DESC* pp = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
		for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, pp++)
		{
			if (ISFLAG_ON(pp->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
			{
				if (s == pp->m_pPathPoint1->s)
				{
					ptimerange->start = pp->m_kin.m_t;
					do { pp++; } while (ISFLAG_ON(pp->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT));
					#ifdef _DEBUG
					NLTRJPOINT_DESC* plast = (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_trajectoryPointDescArray);
					NErrorIf(pp > plast, NERROR_ADDRESS_OUTOFRANGE);
					NErrorIf(!IS_NLTRAJECTORY_POINT_DESC_TRAVELING(pp->m_flags), NERROR_INCONSISTENT_PARAMETERS);
					NErrorIf(pp->m_kin.m_t != (pp-1)->m_kin.m_t, NERROR_INCONSISTENT_PARAMETERS);
					#endif	
					
					ptimerange->range = pp->m_kin.m_t - ptimerange->start;
					return NLTRAJECTORYPACK_GETTIME_INTERVAL;
				}
			}
		}
		//... si on arrive ici c'est que s n'est �gal � aucun SPOT, du coup on appel getTime "classique"
		ptimerange->start	= getTime(s);
		ptimerange->range	= 0.0f;
		return NLTRAJECTORYPACK_GETTIME_UNIQUE;
	}
}
Nf32 NLTRAJECTORY_PACK::getLastTrjPtDescTime()
{
	if (m_trajectoryPointDescArray.Size)
	{
		return ((NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst + m_trajectoryPointDescArray.Size - 1)->m_kin.m_t;
	}
	else
	{
		return 0.0f; 
	} 
}
#endif

/*
void NLTRAJECTORY_PACK::drawChunksArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
	NLTRAJECTORY_CHUNK* pchunk = (NLTRAJECTORY_CHUNK*)m_chunksArray.pFirst;
	for (Nu32 i = 0; i < m_chunksArray.Size; i++, pchunk = (NLTRAJECTORY_CHUNK*)((NBYTE*)pchunk + m_chunksArray.ElementSize))
		pchunk->draw(p2docs, pickpack);
}
*/


/*
#ifdef _NEDITOR
void NLTRAJECTORY_BASIC_PACK::drawTrajectoryStateSArray(NL2DOCS * p2docs)
{
	NLTRAJECTORY_BASIC_STATE	*pstate = (NLTRAJECTORY_BASIC_STATE*)m_basicKeyStatesArray.pFirst; pstate++;

	for (Nu32 i = 1; i < m_basicKeyStatesArray.Size; i++, pstate++)
	{
		pstate->m_kin.draw(p2docs, &(pstate - 1)->m_kin);
	}
}
#endif
*/


/*
void NLTRAJECTORY_TRACKER::build(NLPATH_GEOMETRY * ppath_geometry, const NLDRIVETRAINSPECS * pdtspecs)
{
	NLDIFFERENTIAL_DRIVETRAIN_POSE	d2tp;

	NLKIN	 kin;
	NLKIN	*pkin0 = (NLKIN*)m_kinsArray.pFirst;
	NLKIN	*pkin1 = pkin0;
	Nu32	 kin1_id = 0;

	NLPATH_POINT *pkp0 = (NLPATH_POINT*)ppath->m_geometry.m_pathPointsArray.pFirst;
	NLPATH_POINT *pkp1 = pkp0 + 1;
	NLPATH_PRIMITIVE *pprim = (NLPATH_PRIMITIVE*)ppath->m_geometry.m_primitivesArray.pFirst;

	Nf32	curvature;
	Nf32	slocal;

	NResizeArray(&m_differentialDriveTrainPosesArray, 0, NULL, NULL);

	for (Nu32 i = 0; i < ppath->m_geometry.m_primitivesArray.Size; i++, pkp0 = pkp1, pkp1++, pprim++)
	{
		while ((kin1_id < m_kinsArray.Size) && (pkin1->m_s < pkp1->s))
		{
			// on recup�re la courbure en pkin->m_s
					// ( on sait que pkin->m_s est situ� "entre" pkp0 et pkp1 sur la primitive pprim ...
			if (pprim->m_core.m_id == NLPATH_PRIMITIVE_ID_CLOTHOID)
			{
				slocal = pkin1->m_s - pkp0->s;
				curvature = ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND) ? (pprim->m_core.m_l - slocal) * pkp0->k / pprim->m_core.m_l : slocal * pkp1->k / pprim->m_core.m_l;
			}
			else // pprim->m_core.m_id ==  NLPATH_PRIMITIVE_ID_SEGMENT ou pprim->m_core.m_id ==  NLPATH_PRIMITIVE_ID_ARC
			{
				curvature = pkp1->k;
			}
			d2tp.set(pkin1, curvature);
			NArrayPushBack(&m_differentialDriveTrainPosesArray, (NBYTE*)&d2tp);

			// Kin suivant
			// !!! ATTENTION !!! � la toute fin, "kin1_id = m_kinsArray.Size" et le pointeur pkin1 pointe en dehors de m_kinsArray !!!
			pkin0 = pkin1;
			pkin1++;
			kin1_id++;
		}

		// A partir d'ici nous savons:
		//  pkin1->m_s >= pkp1->s 
		//		OU
		//  kin1_id == m_kinsArray.Size, 
		//	ce qui signifie que l'abscisse curviligne du dernier Kin de m_kinsArray est inf�rieure � longueur totale du chemin...
		//	... et que le pointeur pkin1 courant est invalide ( hors array )
		if (kin1_id < m_kinsArray.Size)
		{
			// on calcule et on ins�re une pose issue de "pkp1"
			getKinAtS(&kin, pkin0, pkin1, pkp1->s);
			d2tp.set(&kin, pkp1->k);
			NArrayPushBack(&m_differentialDriveTrainPosesArray, (NBYTE*)&d2tp);
		}
		else
		{
			break;
		}
	}
	// Une derni�re chose,
	// il est possible que l'abscisse curviligne du dernier kin soit l�g�rement sup�rieure � la longueur totale du chemin.
	if (kin1_id < m_kinsArray.Size)
	{
		kin1_id++;
	}
}
*/



// ... 
	//	*****************************************************************************************************************************
	//	*
	//	*	L'abscisse du dernier Trajectory Point Desc est elle "audel�" du chemin ?
	//	*
	//	*	Les "descriptions de Point de trajectoire" et leur kins sont calcul�s pour amener le robot exactement en fin de chemin.
	//	*	En cons�quence, on s'attend � avoir: 
	//	*											ptrjpointdsc->m_kin.m_s == m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]
	//  *
	//	*	Cependant, Il est possible que les approximations de calcul des kins ( float ) cumul�es aboutissent � l'in�galit� suivante:
	//	*
	//	*											ptrjpointdsc->m_kin.m_s != m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]
	//	*	
	//	*	Dans l'absolu, cela n'est pas tr�s grave car les fonctions qui ensuite interpr�tent les "ptrjpointdsc->m_kin" le font d'une mani�re telle quelle retourne un r�sultat coh�rent.
	//	*
	//	*										par ex:	Le r�sultat calcul� par NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT *pres, const Nf32 s) avec :
	//	*													s = ptrjpointdsc->m_kin.m_s	[ s > NLPATH_GEOMETRY.m_ds ]
	// 	*												
	//	*												Sera le point pres, situ� au del� du chemin sur le prologement de la derni�re primitive du chemin.
	//	*												Sur un segment, le point sera sur le prolongement du segment.
	//	*												Sur un arc, le point sera sur le prolongement de l'arc. 	
	//	*												Sur une clothoide, le point sera sur le prolongement de la clothoide.
	//  *
	//  *	Si on tient absolument � "ptrjpointdsc->m_kin.m_s <= m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]"
	//	*	... il conviendra de ralloger le chemin "d'un petit quelquechose" pour y parvenir.
	//  *	(	le cas o� ptrjpointdsc->m_kin.m_s < m_pathGeometry.m_ds, est moins g�nant, cela revient � dire que le robot c'est arr�t� un peu avant mais SUR le chemin, alors que le cas
	//	*		o� ptrjpointdsc->m_kin.m_s > m_pathGeometry.m_ds revient � dire que le robot est HORS du chemin )
	//  *
	//	*	On rallongera donc artificiellement le chemin en lui ajoutant un segment dont la longueur sera suffisement grande pour "int�grer" le/les dernier(s) "trajectory Point Desc" ... 
	//	*

	/*
	//	* m�thode de rallongement : non test�e ! et non mise en oeuvre car pas forcement utile en fin de compte ...
	if (ptrjpointdsc->m_kin.m_s > m_pathGeometry.m_ds)
	{
		NLPATH_POINT* pof = (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst;
		NLPATH_PRIMITIVE* pof2 = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;

		pp = (NLPATH_POINT*)NArrayAllocBack(&m_pathGeometry.m_pathPointsArray);
		pprim =(NLPATH_PRIMITIVE*)NArrayAllocBack(&m_pathGeometry.m_primitivesArray);

		if ((pof != (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst)||(pof2 != (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst))
		{
			ptrjpointdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
			for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, ptrjpointdsc++)
			{
				ptrjpointdsc->m_pPathPoint1 = (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst + (ptrjpointdsc->m_pPathPoint1 - pof);
				ptrjpointdsc->m_pPrimitive = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst + (ptrjpointdsc->m_pPrimitive - pof2);
			}
			ptrjpointdsc--;
			NErrorIf(((NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst - ptrjpointdsc) != m_trajectoryPointDescArray.Size - 1, NERROR_INCONSISTENT_VALUES);
		}

		plastp = (NLPATH_POINT*)NGetArrayPtr(&m_pathGeometry.m_pathPointsArray, m_pathGeometry.m_pathPointsArray.Size - 2);
		NLPATH_PRIMITIVE* plastprim = (NLPATH_PRIMITIVE*)NGetArrayPtr(&m_pathGeometry.m_primitivesArray, m_pathGeometry.m_primitivesArray.Size - 2);
		NErrorIf(plastp != ptrjpointdsc->m_pPathPoint1, NERROR_INCONSISTENT_VALUES);
		NErrorIf(plastprim != ptrjpointdsc->m_pPrimitive, NERROR_INCONSISTENT_VALUES);

		// Setup de la primitive "artificielle"
		pprim->m_segment.m_core.m_id		= NLPATH_PRIMITIVE_ID_SEGMENT;
		pprim->m_segment.m_core.m_flags		= flags;
		pprim->m_segment.m_core.m_keyPoint0 = _SafeNu32ToNu16(m_pathGeometry.m_pathPointsArray.Size - 2);
		pprim->m_segment.m_core.m_l			= pp->s - m_pathGeometry.m_ds;

		// Setup du point "artificiel"
		pp->k	= 0.0f;
		pp->u	= plastp->u;
		pp->s	= ptrjpointdsc->m_kin.m_s;
		pp->p.x = pp->u.x * pprim->m_segment.m_core.m_l;
		pp->p.y = pp->u.y * pprim->m_segment.m_core.m_l;
	}
	*/


	// ------------------------------------------------------------------------------------------
	/**
	 *	@brief	Construit une s�rie de "Descriptions de Point de trajectoire" � partir d'un/plusieurs "kins"
	 *			... et bien s�r du chemin d�j� d�fini dans le Pack.
	 *
	 *	@param	build_flags	est une valeur 32 bits contenant:
	 *										le "Core Mode" cod� sur 1 bit ( BIT_1 )
	 *
	 *
	 *	@return	l'index de l'action attach�e.
	 *			NVOID si hwnd est NULL
	 *	 		valeur ind�termin�e si la classe de fen�tre de hwnd n'est pas ActionWinClass
	 *
	 */
	 // ------------------------------------------------------------------------------------------
	/*
	NLTRJPOINT_DESC_CFG NLTRAJECTORY_PACK::buildTravelingChunk(const NLTRJPOINT_DESC_CFG cfg, const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid)
	{
	// TODO !!! Attention cette fonction peux g�n�rer un NLTRJPOINT_DESC en fin de chunk dont l'abscisse
	// sera plus grande que la longueur totale du chemin !!! Cela est du � l'impr�cision potentielle du calcul des KIN, car pour eux, le s est le resultat
	// de l'integration de la vitesse, de l'acceleration et du jerk au cours du temps ...
	// Pour l'instant ce cas n'est pas g�r� !!! IL VA FALLOIR LE FAIRE !!!  Sinon plantage assur� avec la fonction NLPATH_GEOMETRY::getPathPoint
		NErrorIf(pkinsarray->ElementSize != sizeof(NLKIN), NERROR_WRONG_ARRAY_SIZE);
		NErrorIf(firstkinid >= pkinsarray->Size, NERROR_INDEX_OUTOFRANGE);
		NErrorIf(lastkinid <= firstkinid, NERROR_INCONSISTENT_VALUES);
		NErrorIf(m_pathGeometry.m_pathPointsArray.Size <2, NERROR_INCONSISTENT_VALUES);

		//	0)	* Preparation
		//		*	pointeurs sur Kin
		NLKIN* pk				= (NLKIN*)NGetArrayPtr(pkinsarray, firstkinid);
		NLKIN* pprvk			= pk;
		NLKIN* plastk			= NULL;
		if (lastkinid == NVOID)
		{
			plastk = (NLKIN*)NGetLastArrayPtr(pkinsarray);
		}
		else
		{
			NErrorIf(lastkinid >= pkinsarray->Size, NERROR_INDEX_OUTOFRANGE);
			plastk = (NLKIN*)NGetArrayPtr(pkinsarray, lastkinid);
		}
		NErrorIf(!pk || !plastk, NERROR_NULL_POINTER);

		// ????????NErrorIf(plastk->m_s == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
		//		*	pointeurs sur �l�ments constituant la g�om�trie du chemin
		NLPATH_PRIMITIVE* pprim;
		NLPATH_POINT* pp;
		NLPATH_POINT* plastp	= ((NLPATH_POINT*)(m_pathGeometry.m_pathPointsArray.pFirst)) + m_pathGeometry.m_pathPointsArray.Size - 1;
		// ????????NErrorIf(((NLPATH_POINT*)NGetLastArrayPtr(&m_pathGeometry.m_pathPointsArray))->s < plastk->m_s, NERROR_SYSTEM_GURU_MEDITATION);

		//		*	pointeur sur point de trajectoire
		NLTRJPOINT_DESC* ptrjpointdsc = (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_trajectoryPointDescArray);

		//	1)	* Initialisation
		//		*
		if (!ptrjpointdsc)
		{
			// L'array des "trajectoryPointDesc(ription)" est vide !
			// Ce chunk est le premier � y �tre ins�r� !
			// Il DOIT d�marrrer par un premier Kin dont l'abscisse curviligne est 0 !
			NErrorIf(pk->m_s != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pk->m_v != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pk->m_a != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pk->m_j != 0.0f, NERROR_INCONSISTENT_VALUES);
			pprim = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;
			pp = (NLPATH_POINT*)(m_pathGeometry.m_pathPointsArray.pFirst) + 1;
		}
		else
		{
			// Chaque nouveau kin ins�r� est cens� contribuer � un d�placement du d�but � la fin du chemin, du coup l'abscisse curviligne de chaque nouveau kin ins�r�
			// doit �tre sup�rieure ou �gale � la pr�c�dente, jamais inf�rieure !
			NErrorIf(pk->m_s < ptrjpointdsc->m_kin.m_s, NERROR_INCONSISTENT_VALUES);
			pprim	= ptrjpointdsc->m_pPrimitive;
			pp		= ptrjpointdsc->m_pPathPoint1;
		}


		//	2)	* Construction des NLTRJPOINT_DESC
		//
		NErrorIf(!IS_NLTRJACTION_KTYPE_TRAVELING(cfg), NERROR_INCONSISTENT_FLAGS);
		NErrorIf(IS_NLTRJACTION_KTYPE_NULL(cfg), NERROR_INCONSISTENT_FLAGS);
		Nf32					 pps = pp->s;
		NLTRJPOINT_DESC  trjpointdsc;

		trjpointdsc.m_flags = cfg & MASK_NLTRAJECTORY_POINT_DESC_CONFIG;

		while (1)
		{
			if (pk->m_s < pps)
			{
				//trjpointdsc.m_flags ...	is already setup
				trjpointdsc.m_kin			= *pk;
				trjpointdsc.m_pPathPoint1	= pp;
				trjpointdsc.m_pPrimitive	= pprim;
				ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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
				//trjpointdsc.m_flags ...	is already setup
				trjpointdsc.m_kin = *pk;
				trjpointdsc.m_pPathPoint1 = pp;
				trjpointdsc.m_pPrimitive = pprim;
				ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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

				//trjpointdsc.m_flags ...	is already setup
				trjpointdsc.m_kin.atS(pprvk, pk, pps);
				trjpointdsc.m_pPathPoint1 = pp;
				trjpointdsc.m_pPrimitive = pprim;
				ptrjpointdsc = (NLTRJPOINT_DESC*)NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);

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
		//	*****************************************************************************************************************************
		//	*
		//	*	L'abscisse du dernier Trajectory Point Desc est elle "audel�" du chemin ?
		//	*
		//	*	Les "descriptions de Point de trajectoire" et leur kins sont calcul�s pour amener le robot exactement en fin de chemin.
		//	*	En cons�quence, on s'attend � avoir:
		//	*											ptrjpointdsc->m_kin.m_s == m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]
		//  *
		//	*	Cependant, Il est possible que les approximations de calcul des kins ( float ) cumul�es aboutissent � l'in�galit� suivante:
		//	*
		//	*											ptrjpointdsc->m_kin.m_s != m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]
		//	*
		//	*	Dans l'absolu, cela n'est pas tr�s grave car les fonctions qui ensuite interpr�tent les "ptrjpointdsc->m_kin" le font d'une mani�re telle quelle retourne un r�sultat coh�rent.
		//	*
		//	*										par ex:	Le r�sultat calcul� par NLPATH_GEOMETRY::getPathPoint(NLPATH_POINT *pres, const Nf32 s) avec :
		//	*													s = ptrjpointdsc->m_kin.m_s	[ s > NLPATH_GEOMETRY.m_ds ]
		// 	*
		//	*												Sera le point pres, situ� au del� du chemin sur le prologement de la derni�re primitive du chemin.
		//	*												Sur un segment, le point sera sur le prolongement du segment.
		//	*												Sur un arc, le point sera sur le prolongement de l'arc.
		//	*												Sur une clothoide, le point sera sur le prolongement de la clothoide.
		//  *
		//  *	Si on tient absolument � "ptrjpointdsc->m_kin.m_s <= m_pathGeometry.m_ds		[ avec "ptrjpointdsc->m_kin" dernier Kin de la trajectoire ]"
		//	*	... il conviendra de ralloger le chemin "d'un petit quelquechose" pour y parvenir.
		//  *	(	le cas o� ptrjpointdsc->m_kin.m_s < m_pathGeometry.m_ds, est moins g�nant, cela revient � dire que le robot c'est arr�t� un peu avant mais SUR le chemin, alors que le cas
		//	*		o� ptrjpointdsc->m_kin.m_s > m_pathGeometry.m_ds revient � dire que le robot est HORS du chemin )
		//  *
		//	*	On rallongera donc artificiellement le chemin en lui ajoutant un segment dont la longueur sera suffisement grande pour "int�grer" le/les dernier(s) "trajectory Point Desc" ...
		//	*

		/ *
		//	* m�thode de rallongement : non test�e ! et non mise en oeuvre car pas forcement utile en fin de compte ...
		if (ptrjpointdsc->m_kin.m_s > m_pathGeometry.m_ds)
		{
			NLPATH_POINT* pof = (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst;
			NLPATH_PRIMITIVE* pof2 = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst;

			pp = (NLPATH_POINT*)NArrayAllocBack(&m_pathGeometry.m_pathPointsArray);
			pprim =(NLPATH_PRIMITIVE*)NArrayAllocBack(&m_pathGeometry.m_primitivesArray);

			if ((pof != (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst)||(pof2 != (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst))
			{
				ptrjpointdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
				for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, ptrjpointdsc++)
				{
					ptrjpointdsc->m_pPathPoint1 = (NLPATH_POINT*)m_pathGeometry.m_pathPointsArray.pFirst + (ptrjpointdsc->m_pPathPoint1 - pof);
					ptrjpointdsc->m_pPrimitive = (NLPATH_PRIMITIVE*)m_pathGeometry.m_primitivesArray.pFirst + (ptrjpointdsc->m_pPrimitive - pof2);
				}
				ptrjpointdsc--;
				NErrorIf(((NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst - ptrjpointdsc) != m_trajectoryPointDescArray.Size - 1, NERROR_INCONSISTENT_VALUES);
			}

			plastp = (NLPATH_POINT*)NGetArrayPtr(&m_pathGeometry.m_pathPointsArray, m_pathGeometry.m_pathPointsArray.Size - 2);
			NLPATH_PRIMITIVE* plastprim = (NLPATH_PRIMITIVE*)NGetArrayPtr(&m_pathGeometry.m_primitivesArray, m_pathGeometry.m_primitivesArray.Size - 2);
			NErrorIf(plastp != ptrjpointdsc->m_pPathPoint1, NERROR_INCONSISTENT_VALUES);
			NErrorIf(plastprim != ptrjpointdsc->m_pPrimitive, NERROR_INCONSISTENT_VALUES);

			// Setup de la primitive "artificielle"
			pprim->m_segment.m_core.m_id		= NLPATH_PRIMITIVE_ID_SEGMENT;
			pprim->m_segment.m_core.m_flags		= flags;
			pprim->m_segment.m_core.m_keyPoint0 = _SafeNu32ToNu16(m_pathGeometry.m_pathPointsArray.Size - 2);
			pprim->m_segment.m_core.m_l			= pp->s - m_pathGeometry.m_ds;

			// Setup du point "artificiel"
			pp->k	= 0.0f;
			pp->u	= plastp->u;
			pp->s	= ptrjpointdsc->m_kin.m_s;
			pp->p.x = pp->u.x * pprim->m_segment.m_core.m_l;
			pp->p.y = pp->u.y * pprim->m_segment.m_core.m_l;
		}
		* /
	}
	*/
	/*
	NLPATH_POINT* NLTRAJECTORY_PACK::safeSpotAllocation()
	{
		if (m_spotsArray.Size < m_spotsArray.Capacity)
		{
			return (NLPATH_POINT*)NArrayAllocBack(&m_spotsArray);
		}
		else
		{
			NLTRJPOINT_DESC  *ppdsc = (NLTRJPOINT_DESC*)m_trajectoryPointDescArray.pFirst;
			for (Nu32 i = 0; i < m_trajectoryPointDescArray.Size; i++, ppdsc++)
			{
				if(!ppdsc->m_pPrimitive)
				ppdsc->m_pPathPoint1
			}

		}
	}
	*/
	/*
	NLPROMOTE_KEY_ACTION_CFG NLTRAJECTORY_PACK::buildSpotChunk(NARRAY* pkinsarray, NARRAY * psmotionprofiles_array, NLTRJKEY* pkey, NLPROMOTE_KEY_ACTION_CFG cfg)
	{
		NErrorIf(pkinsarray->ElementSize != sizeof(NLKIN), NERROR_WRONG_ARRAY_SIZE);

		NLTRJPOINT_DESC		trjpointdsc;
		NLPATH_POINT*				pspot	= NULL;
		NLKIN*						pk		= NULL;
		NLKIN*						plastk	= NULL;
		NLPROMOTE_KEY_ACTION_CFG	nxt_cfg	= 0;

		pkey->preCompute(psmotionprofiles_array, &m_driveTrainSpecifications);

		while (IS_NLPROMOTE_KEY_ACTION_RESULTS_CONTINUE(cfg))
		{
			nxt_cfg = pkey->promote(pkinsarray, cfg, psmotionprofiles_array);

			if (IS_NLPROMOTE_KEY_ACTION_RESULTS_CREATE_KIN(nxt_cfg))
			{
				pk = (NLKIN*)NGetArrayPtr(pkinsarray, GET_NLPROMOTEACTION_RESULTS_FIRST_KIN_IDX(nxt_cfg));
				plastk = (NLKIN*)NGetLastArrayPtr(pkinsarray);
				NErrorIf(!pk || !plastk, NERROR_NULL_POINTER);
				// Creation du "Point" fixe sur lequel les TrajectoryPointDesc vont se construire
				pspot = (NLPATH_POINT*)NGetLastArrayPtr(&m_spotsArray);
				if (!pspot || pspot->s != pkey->m_s)
				{
					NErrorIf(m_spotsArray.Size == m_spotsArray.Capacity, NERROR_ARRAY_REALLOCATION_FAILURE);
					pspot = (NLPATH_POINT*)NArrayAllocBack(&m_spotsArray);
					m_pathGeometry.getPathPoint(pspot, pkey->m_s);
				}
				while (pk <= plastk)
				{
					NErrorIf(IS_NLTRJACTION_KTYPE_TRAVELING(cfg) || IS_NLTRJACTION_KTYPE_NULL(cfg), NERROR_INCONSISTENT_FLAGS);
					trjpointdsc.m_flags = cfg & MASK_NLTRAJECTORY_POINT_DESC_CONFIG;
					trjpointdsc.m_kin = *pk;
					trjpointdsc.m_pPathPoint1 = pspot;
					trjpointdsc.m_pPrimitive = NULL;
					NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);
					pk++;
				}
			}
			cfg = nxt_cfg;
		}
		return cfg;
	}
	*/
	/*
	NLTRJPOINT_DESC_CFG NLTRAJECTORY_PACK::buildSpotChunk(const NLTRJPOINT_DESC_CFG cfg, const Nf32 s, const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid)
	{
		NErrorIf(pkinsarray->ElementSize != sizeof(NLKIN), NERROR_WRONG_ARRAY_SIZE);
		NErrorIf(firstkinid >= pkinsarray->Size, NERROR_INDEX_OUTOFRANGE);
		NErrorIf(lastkinid <= firstkinid, NERROR_INCONSISTENT_VALUES);

		NLTRJPOINT_DESC  trjpointdsc;
		NLPATH_POINT* pspot = NULL;
		NLKIN* pk = (NLKIN*)NGetArrayPtr(pkinsarray, firstkinid);
		NLKIN* plastk = NULL;
		if (lastkinid == NVOID)
		{
			plastk = (NLKIN*)NGetLastArrayPtr(pkinsarray);
		}
		else
		{
			plastk = (NLKIN*)NGetArrayPtr(pkinsarray, lastkinid);
		}
		NErrorIf(!pk || !plastk, NERROR_NULL_POINTER);

		// Creation du "Point" fixe sur lequel les TrajectoryPointDesc vont se construire
		pspot = (NLPATH_POINT*)NGetLastArrayPtr(&m_spotsArray);
		if (!pspot || pspot->s != s)
		{
			NErrorIf(m_spotsArray.Size == m_spotsArray.Capacity, NERROR_ARRAY_REALLOCATION_FAILURE); // Normalement
			pspot = (NLPATH_POINT*)NArrayAllocBack(&m_spotsArray);
			m_pathGeometry.getPathPoint(pspot, s);
		}
		while (pk <= plastk)
		{
			NErrorIf(IS_NLTRJACTION_KTYPE_TRAVELING(cfg) || IS_NLTRJACTION_KTYPE_NULL(cfg), NERROR_INCONSISTENT_FLAGS);
			trjpointdsc.m_flags		 = cfg&MASK_NLTRAJECTORY_POINT_DESC_CONFIG;
			trjpointdsc.m_kin		 = *pk;
			trjpointdsc.m_pPathPoint1 = pspot;
			trjpointdsc.m_pPrimitive = NULL;
			NArrayPushBack(&m_trajectoryPointDescArray, (NBYTE*)&trjpointdsc);
			pk++;
		}
	}
	*/