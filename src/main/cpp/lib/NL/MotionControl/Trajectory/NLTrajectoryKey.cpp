#include "../../../N/NErrorHandling.h"
#include "../../../N/Core/NLimits.h"
#include "../../../N/Core/NSafeConversion.h"
#include "../../../N/NMemory.h"
#include "../../../N/NFlags.h"
#include "../../../N/Containers/NArray.h"
#include "../../../N/Maths/NIntervalf32.h"

#ifdef _NEDITOR
#include "../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../N/Utilities/Draw/NUT_DrawPencil.h"

#endif

#include "../Path/NLPath.h"
#include "NLTrajectoryAction.h"
#include "NLTrajectoryKey.h"
#include "NLTrajectoryPointDesc.h"
#include "../MotionProfiles/NLSmotionProfile.h"
/*
NLTRJKEY::NLTRJKEY()
{
	Nmem0(this, NLTRJKEY);
	// par défaut, et sans plus amples précision, une clef nouvelle est considérée comme un stop et lockée ( non supprimable )
	FLAG_ON(m_flags, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP);
	// m_singleAction.m_core.m_id = NLTRJACTION_ID_NULL;
	NSetupArray(&m_actionsArray, DEFAULT_NLTRJKEY_ACTIONSARRAY_CAPACITY, sizeof(NLTRJACTION));
}

NLTRJKEY::NLTRJKEY(const Nf32 s, const Nu32 flags)
{
	Nmem0(this, NLTRJKEY);
	m_s = s;	// Attention ! Aucun test de validité n'est effectué sur cette valeur qui, pour être valide, doit être comprise entre 0 ( début du chemin ) et S ( abscisse max )
	m_flags = flags;
	NSetupArray(&m_actionsArray, DEFAULT_NLTRJKEY_ACTIONSARRAY_CAPACITY, sizeof(NLTRJACTION));
}

NLTRJKEY::~NLTRJKEY()
{
	NErrorIf(ISFLAG_ON(m_flags, FLAG_NLTRJKEY_LOCKED), NERROR_UNAUTHORIZED_ACTION);
	NClearArray(&m_actionsArray, NULL);
}
*/
NLTRJKEY* NLTRJKEY::setup()
{
	Nmem0(this, NLTRJKEY);
	m_timeStamp.start	= -1.0f;	// |_	Définir les valeurs par defaut à -1 permettra de savoir par la suite si elles ont été updatées ...
	m_timeStamp.end		= -1.0f;	// |	... en cas d'update, start >= end et start >=0 et end >=0 		

	// par défaut, et sans plus amples précision, une clef nouvelle est considérée comme un stop et lockée ( non supprimable )
	FLAG_ON(m_flags, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP);
	// m_singleAction.m_core.m_id = NLTRJACTION_ID_NULL;
	NSetupArray(&m_actionsArray, DEFAULT_NLTRJKEY_ACTIONSARRAY_CAPACITY, sizeof(NLTRJACTION));
	return this;
}

NLTRJKEY* NLTRJKEY::setup(const Nf32 s, const Nu32 flags)
{
	Nmem0(this, NLTRJKEY);
	m_timeStamp.start	= -1.0f;	// |_	Définir les valeurs par defaut à -1 permettra de savoir par la suite si elles ont été updatées ...
	m_timeStamp.end		= -1.0f;	// |	... en cas d'update, start >= end et start >=0 et end >=0 		

	m_s = s;	// Attention ! Aucun test de validité n'est effectué sur cette valeur qui, pour être valide, doit être comprise entre 0 ( début du chemin ) et S ( abscisse max )
	m_flags = _SafeNu32ToNu16(flags);
	NSetupArray(&m_actionsArray, DEFAULT_NLTRJKEY_ACTIONSARRAY_CAPACITY, sizeof(NLTRJACTION));
	return this;
}

void NLTRJKEY::clear()
{
	NErrorIf(ISFLAG_ON(m_flags, FLAG_NLTRJKEY_LOCKED), NERROR_UNAUTHORIZED_ACTION);
	NClearArray(&m_actionsArray, NLclearTrjActionInArrayCallBack);
}

Nu32 NLTRJKEY::write(FILE* pfile)
{
	NLTRAJECTORYKEY_HEADER header;
	// 1) écriture Version
	Nu32	version_u32 = VERSION_NLTRAJECTORYKEY_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) écriture Header
	header.m_s				= m_s;
	header.m_flags			= m_flags;
	header.m_linkedPointId	= m_linkedPointId;
	header.m_timeStamp		= m_timeStamp;

	NGetArrayBounds(&header.m_actionsArrayBounds, &m_actionsArray);

	if (fwrite(&header, sizeof(NLTRAJECTORYKEY_HEADER), 1, pfile) != 1)
		return 0;
	if (m_actionsArray.Size)
	{
		if (fwrite(m_actionsArray.pFirst, m_actionsArray.ElementSize, m_actionsArray.Size, pfile) != m_actionsArray.Size)
			return 0;
	}

	return 1;
}

Nu32 NLTRJKEY::read(FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	NLTRAJECTORYKEY_HEADER header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLTRAJECTORYKEY_HEADER):
		if (fread(&header, sizeof(NLTRAJECTORYKEY_HEADER), 1, pfile) != 1)
			return 0;
		m_flags			= header.m_flags;
		m_s				= header.m_s;
		m_linkedPointId = header.m_linkedPointId;
		m_timeStamp		= header.m_timeStamp;
		// Recherche de potentiels pbs sur les arrays.
		// ('NIsArrayCorruptedOrInconsistent' se charge de faire un setup auto en cas de array full of ZEROS)
		if (NIsArrayCorruptedOrInconsistent(&m_actionsArray, &header.m_actionsArrayBounds, NTRUE))
			return 0;

		// preparation de l'array 'm_actionsArray' . 

		// ! on charge en un seul bloc car on sait que NLTRJACTION ne contient aucune dépendences et/ou "nested" 
		// memory allocations.
		if (m_actionsArray.Size)
		{
		// Ainsi la mémoire nécéssaire pour stocker les n actions a charger est allouée ( ou pas si déjà fait )
 		NResizeArray(&m_actionsArray, header.m_actionsArrayBounds.Size, NULL, NLclearTrjActionInArrayCallBack);
			if (fread(m_actionsArray.pFirst, m_actionsArray.ElementSize, m_actionsArray.Size, pfile) != m_actionsArray.Size)
				return 0;
		}
		return 1;

	default:
		break;
	}
	return 0;
}
/*
NLTRJACTION* NLTRJKEY::pushBackAction(const NLTRJACTION* paction)
{
	if (paction)
	{
		return (NLTRJACTION*)NArrayPushBack(&m_actionsArray, (NBYTE*)paction);
	}
	else
	{
		NLTRJACTION nullaction;
		setupAction(&nullaction, NLTRJACTION_ID_NULL);
		return (NLTRJACTION*)NArrayPushBack(&m_actionsArray, (NBYTE*)&nullaction);
	}
}
*/
NLTRJACTION* NLTRJKEY::insertAction(const NLTRJACTION* paction)
{
	NErrorIf(paction && paction->m_core.m_timeShift < 0.0f, NERROR_UNAUTHORIZED_VALUE);

	NLTRJACTION action;

	if (paction)
		action = *paction;
	else
		setupAction(&action, NLTRJACTION_ID_NULL);

	if (m_actionsArray.Size)
	{
		if(paction)
		{
			if (NIsValidArrayElementPtr(&m_actionsArray, (NBYTE*)paction))
				return (NLTRJACTION*)NInsertArrayElement(&m_actionsArray, paction - (NLTRJACTION*)m_actionsArray.pFirst, (NBYTE*)&action);
			else if(action.m_core.m_timeShift == 0.0f)
				return (NLTRJACTION*)NInsertArrayElement(&m_actionsArray, 0, (NBYTE*)&action);
			else
			{
				NLTRJACTION* pac = (NLTRJACTION*)m_actionsArray.pFirst;
				for (Nu32 i = 0; i < m_actionsArray.Size; i++, pac++)
				{
					if (action.m_core.m_timeShift <= pac->m_core.m_timeShift)
						return (NLTRJACTION*)NInsertArrayElement(&m_actionsArray, i, (NBYTE*)&action);
				}
				return (NLTRJACTION*)NArrayPushBack(&m_actionsArray, (NBYTE*)&action);
			}
		}
		else
		{
			return (NLTRJACTION*)NInsertArrayElement(&m_actionsArray, 0 , (NBYTE*)&action);
		}
	}
	else
	{
		return (NLTRJACTION*)NArrayPushBack(&m_actionsArray, (NBYTE*)&action);
	}
}



static void _swapaction(NLTRJACTION* p1, NLTRJACTION* p2)
{
	NLTRJACTION		tmp;

	tmp = *p1; 
	*p1 = *p2; 
	*p2 = tmp;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief		Déplace une action de une place( dans l'array ) vers la droite.
 *				Les actions sont triées dans l'ordre croissant de leur timeShift. 
 *				Donc déplacer une action vers la droite revient potentiellement à augementer son timeshift ...
 *				... et surtout à veiller à ce que son timeShift, après déplacement soit >= au timeshift de l'action qui se trouvera juste avant elle.
 *				Cette action qui se trouvera alors juste avant elle est en fait celle qui est après elle avant le déplacement.
 *				Avant le déplacement, on fera donc simplement:
 *												action.m_timeShift = (action+1).m_timeshift
 *
 * 	@param		actionidx	index de l'action à déplacer.
 *
 * @return		Nouvel index de l'action après déplacement.
 *				= actionidx + 1 en cas de succès.
 *				= actionidx en cas d'échec
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLTRJKEY::moveActionDown(Nu32 actionidx)
{
	if ((actionidx < m_actionsArray.Size) && (m_actionsArray.Size>1) && (actionidx < m_actionsArray.Size-1))
	{
		NLTRJACTION* p0 = (NLTRJACTION*)m_actionsArray.pFirst + actionidx;
		p0->m_core.m_timeShift = (p0+1)->m_core.m_timeShift;
		p0->m_core.m_timeStamp = m_timeStamp.start + p0->m_core.m_timeShift;
		_swapaction(p0, p0 + 1) ;
		return actionidx + 1;
	}
	else
		return actionidx;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief		Déplace une action de une place( dans l'array ) vers la gauche.
 *				Les actions sont triées dans l'ordre croissant de leur timeShift.
 *				Donc déplacer une action vers la gauche revient potentiellement à diminuer son timeshift ...
 *				... et surtout à veiller à ce que son timeShift, après déplacement soit <= au timeshift de l'action qui se trouvera juste après elle.
 *				Cette action qui se trouvera alors juste après elle est en fait celle qui est avant elle avant le déplacement.
 *				Avant le déplacement, on fera donc simplement:
 *												action.m_timeShift = (action-1).m_timeshift
 *
 * 	@param		actionidx	index de l'action à déplacer.
 *
 * @return		Nouvel index de l'action après déplacement.
 *				= actionidx - 1 en cas de succès.
 *				= actionidx en cas d'échec
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLTRJKEY::moveActionUp(Nu32 actionidx)
{
	if ((actionidx< m_actionsArray.Size)&&(m_actionsArray.Size > 1) && (actionidx > 0))
	{
		NLTRJACTION* p0 = (NLTRJACTION*)m_actionsArray.pFirst + actionidx;
		p0->m_core.m_timeShift = (p0 - 1)->m_core.m_timeShift;
		p0->m_core.m_timeStamp = m_timeStamp.start + p0->m_core.m_timeShift;
		_swapaction(p0, p0 - 1);
		return actionidx - 1;
	}
	else
		return actionidx;
}

NLTRJACTION* NLTRJKEY::updatetActionTimeShift(NLTRJACTION* paction, const Nf32 timeshift)
{
	NErrorIf(timeshift < 0.0f, NERROR_UNAUTHORIZED_VALUE);
	NErrorIf(!NIsValidArrayElementPtr(&m_actionsArray, (NBYTE*)paction), NERROR_ARRAY_ADRESS_BEYOND_LIMITS);

	if (paction->m_core.m_timeShift != timeshift)
	{
		paction->m_core.m_timeShift = timeshift;
		NLTRJACTION* pfirst = (NLTRJACTION*)m_actionsArray.pFirst;
		NLTRJACTION* plast = (NLTRJACTION*)NGetLastArrayPtr(&m_actionsArray);
		while ((paction > pfirst) && (paction->m_core.m_timeShift < (paction - 1)->m_core.m_timeShift)) { _swapaction(paction, paction - 1); paction = paction - 1; }
		while ((paction < plast) && (paction->m_core.m_timeShift >= (paction + 1)->m_core.m_timeShift)) { _swapaction(paction, paction + 1); paction = paction + 1; }
	
		paction->m_core.m_timeStamp = m_timeStamp.start + paction->m_core.m_timeShift;
	}
	return paction;
}

Nu32 NLTRJKEY::updatetActionTimeShift(const Nu32 actionidx, const Nf32 timeshift)
{
	NErrorIf(timeshift < 0.0f, NERROR_UNAUTHORIZED_VALUE);
	NErrorIf(actionidx>=m_actionsArray.Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);

	NLTRJACTION* pfirst		= (NLTRJACTION*)m_actionsArray.pFirst;
	NLTRJACTION* plast		= pfirst + m_actionsArray.Size - 1;
	NLTRJACTION* paction	= pfirst + actionidx;

	if (paction->m_core.m_timeShift != timeshift)
	{
		paction->m_core.m_timeShift = timeshift;
		while ((paction > pfirst) && (paction->m_core.m_timeShift < (paction - 1)->m_core.m_timeShift)) { _swapaction(paction, paction - 1); paction = paction - 1; }
		while ((paction < plast) && (paction->m_core.m_timeShift > (paction + 1)->m_core.m_timeShift)) { _swapaction(paction, paction + 1); paction = paction + 1; }

		paction->m_core.m_timeStamp = m_timeStamp.start + paction->m_core.m_timeShift;
	}
	return (paction-pfirst);
}
/*
void NLTRJKEY::SetLifeSpan(const Nf32 lifespan)
{
	m_lifeSpan = lifespan;

	NLTRJACTION* paction = (NLTRJACTION*)m_actionsArray.pFirst;
	for (Nu32 i = 0; i < m_actionsArray.Size; i++, paction++)
	{
		if(paction->m_core.m_timeShift >= m_lifeSpan)
			FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_OUT_OF_KEY_LIFESPAN);
		else
			FLAG_OFF(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_OUT_OF_KEY_LIFESPAN);
	}
}
*/
void NLTRJKEY::setDuration(const Nbool bauto, const Nf32 duration)
{
	if (bauto)
	{
		FLAG_ON(m_flags, FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN); 
	}
	else
	{
		FLAG_OFF(m_flags, FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN);
		m_timeStamp.end = m_timeStamp.start + duration;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief		Vérifie la cohérence de tous les paramètres liés aux actions de cette clef.
 *				Lors de cette vérification certaines actions pourront être désactivées ( mais pas supprimées ! )
 *				Une action pourra cumuler plusieurs flags de désactivation.
 *				Une action déjà désactivée ( avec déjà au moins 1 flag disable ON ) ne pourra pas être la cause de désactivation d'une autre action.
 *				Les cas traités sont:
 *						-	Vérification des SetCoreMode successifs ( FORWARD et REVERSE ) donc certains peuvent être potentiellement inutiles.	
 *							( recherche d'une annulation avant effet et recherche d'une répétition )
 *						- 	Vérification des Overlappings d'action "bloquantes" ( c'est à dire prenant le contrôle de la DT ).
 *							( 2 actions ne peuvent pas prendre en même temps le contrôle de la DT, si cela arrive la première action à prendre le contrôle le conserve,
 *							la/les suivantes sont désactivées )
 *			
 * 	@param		psmp_array est un pointeur sur NARRAY ou sont placés les motion profile de rotation precomputés.
 * 	@param		t est la date à laquelle se situe la clef. en Sortie de fonction on aura m_timeStamp.start = t
 *	@param		pdtspecs est un pointeur sur les specs de la DriveTrain ( l'entraxe est nécéssaire pour le calcul des actions impliquant une rotation ).
 *
 * @return		le nombre total d'actions disable
 *				
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLTRJKEY::preCompute(const Nf32 t, const NLDRIVETRAINSPECS* pdtspecs)
{
	Nu32 i;

	Nu32 disable_count						= 0;
	Nu32 current_coremode					= 0;
	NLTRJACTION* paction0					= NULL;
	NLTRJACTION* paction					= (NLTRJACTION*)m_actionsArray.pFirst;
	Nf32 f									= 0.0f;
	Nf32 keydt								= 0.0f;
	NINTERVALf32 timeinterval;
	timeinterval.start						= 0.0f;
	timeinterval.end						= 0.0f;


	for (i = 0; i < m_actionsArray.Size; i++, paction++)
	{
		// 0)	* Reset all Precheck disable flags ( all to OFF ) ... le flag Disable USER n'est bien sur pas concerné, il ne fait pas partie des flags Precheck.
		//		*
		SET_FLAGS(paction->m_core.m_flags, MASK_NLTRJACTION_DISABLE_PRECHECK, 0);


		// 1)	* Précalcul des données "à calculer" de l'action ( timeStamp, potentiel motionprofile associé ... )
		//		* Ce précalcul est effectué avant même de verifier si l'action est enable ( ou si elle est disable pour une autre raison que les prechecks )
		//		* ... ainsi même une action disable manuellement est "pre-compute" ( pour de potentiels affichages informatifs dans l'interface )
		preComputeAction(paction, t, pdtspecs);

		// 1.b)	* Si l'action est disable : ON L'IGNORE et on passe à la suivante.
		//		* on note que si ici une action est disable, alors que tous ses flags PRECHECK ont été remis à ZERO quelques lignes plus tôt, c'est que le
		//		* flag FLAG_NLTRJACTION_DISABLE_BY_USER est ON.
		if (ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_BY_USER))
		{
			NErrorIf(FLAGS_TEST(paction->m_core.m_flags, MASK_NLTRJACTION_DISABLE, FLAG_NLTRJACTION_DISABLE_BY_USER), NERROR_INCONSISTENT_FLAGS); // seul le flag disable by user devrait être actif.
			disable_count++;
			continue;
		}

		// 1.c)	* Si l'action est inefficace: ON L'IGNORE et on passe à la suivante. 
		//		  "l'efficacité" de l'action: Une action possède potentiellement un certains nombre de paramètres pouvant prendre des valeurs
		//		  qui rendent dans certains cas, l'action inefficace. 
		//		  Par exemple une action de rotation de 0° est jugée inefficace dans le sens où elle ne changera rien à l'état du robot. 
		//		  Même chose si la vitesse de rotation est nulle !
		if (!isActionEfficient(paction))
		{
			FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_PRECHECK_INEFFICIENT);
			disable_count++;
			continue;
		}

		// 2)	* Vérifier les overlap ...
		//		* Deux actions bloquant la Drive Train ne peuvent pas le faire en même temps !
		//		* Si une action essaye de bloquer la DT alors que celle-ci est déjà bloquée, alors l'action est désactivée pour overlapping.
		if (IS_NLTRJACTION_COMMAND(paction->m_core.m_flags))
		{
			if (paction->m_core.m_timeShift < timeinterval.end)
			{
				FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_PRECHECK_OVERLAPPING);
				disable_count ++;
				continue;
			}
			else
			{
				// Toutes les actions bloquant la DT  sont prises en compte pour la mise à jour de l'intervalle de bloquage courant.
				// y compris les actions de type setcoremode qui bloque la DT sur une periode de temps 'NULLE' ( instantannée ) !
				// Cela permet en fin de boucle de lire dans 'current_lockinterval.end' la date de fin de bloquage par l'ensemble des actions bloquants la DriveTrain.
				timeinterval.start	= paction->m_core.m_timeShift;
				timeinterval.end	= paction->m_core.m_timeShift + getActionCommandTime(paction);
			}
		}

		// 3)	* Vérifier les set core mode successifs ...
		//		* Lorsque 2 actions (ou plus) de type SetDriveTrainMode se suivent "directement" c'est à dire sans être séparées par d'autres actions "COMMAND",
		//		* alors la 2eme action setcore mode annule la première sans que celle-ci ai pu avoir un réel effet sur le robot.
		//		* Une action setcore mode a normalement pour effet de créer un nouveau chunkR. Pour éviter la creation de ChunkR inutiles, nous allons rechercher et "désactiver" 
		//		* ces Actions SetCoreMode inutiles.
		if (IS_NLTRJPOINT_DESC_SET_DTMODE(paction->m_core.m_flags))
		{
			if (paction0)
			{
				// Si 'paction0' est non null c'est que:
				//		-	au moins une action "setcoremode" est déjà apparue avant paction.
				//			ET	
				//		-	il n'y a pas eut d'action, entre 'paction0' et paction, bloquant la DT sur une periode de temps non nulle. 
				//			( et donc créant des kins dans le chunkR au set core mode défini par 'paction0').
				// En conséquence,
				// paction va à nouveau définir le Core Mode, et donc annuler le set core mode de 'paction0' sans que celui-ci ai pu avoir un réel effet sur le robot.
				// 'paction0' est donc jugé inutile.
				// (paction reste toujours enable ! C'est 'paction0' qui est désactivée donc on poursuit les prechecks sur paction...  )
				FLAG_ON(paction0->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_PRECHECK_USELESS);
				disable_count ++;

				// 'paction' devient 'paction0', c'est à dire la dernière action setcoremode active
				// et le coremode courant et actif devient bien sûr, celui porté par paction.
				paction0			= paction;
				current_coremode	= (paction->m_core.m_flags & MASK_NLTRJPOINT_DESC_DTMODE);
			}
			else // ... paction0 est NULLE
			{
				// Si paction0 est nulle c'est que:
				//		-	il n'y a pas encore eut d'action setcoremode.		( ! DANS CE CAS : current_coremode == 0 )
				//			OU
				//		-	au moins une action "setcoremode" est déjà apparue avant paction, MAIS DEPUIS il y a eut au moins une action bloquant la DT ...
				//			... sur une periode de temps non nulle.				( ! DANS CE CAS : current_coremode != 0 )	
				//
				//		On peut rencontré 3 cas:
				//				a- le coremode porté par action est identique au coremode courant. 
				//				b- le coremode porté par action est différent du coremode courant, le core mode courant est NULL. ( paction est la première action setcoremode rencontrée )
				// 				c- le coremode porté par action est différent du coremode courant, le core mode courant est non NULL. ( il y a déjà eu une action setcoremode ACTIVE )
				// 
				//		Dans le cas ou le coremode porté par action est identique au coremode courant (cas a), paction sera jugée comme inutile car elle redefini le coremode courant à l'identique.
				//		Sinon dans les deux autres cas ( cas b et c ), paction est utile et ACTIVE.
				if ( (paction->m_core.m_flags & MASK_NLTRJPOINT_DESC_DTMODE) == current_coremode )
				{
					FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_PRECHECK_REPEAT);
					disable_count ++;
					continue;
				}
				else
				{
					// 'paction' devient 'paction0', c'est à dire la dernière action setcoremode active
					// et le coremode courant et actif devient bien sûr, celui porté par paction.
					paction0			= paction;
					current_coremode	= (paction->m_core.m_flags & MASK_NLTRJPOINT_DESC_DTMODE);
				}
			}
		}
		else if(ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_COMMAND_LONG_RUNNING))
		{
			// paction  bloque la DT sur une periode de temps non nulle ( et donc va créer des Kins, "remplissant ainsi" le chunkR courant).
			// En conséquence, le potentiel Set core mode précédent ( porté par paction0 ) est utile puisque c'est lui qui défini le Core Mode courant.
			// Du coup, on ACTIVE définitivement l'action qui était pointée par 'paction0' en ecrivant 'paction0' = NULL
			paction0			= NULL;
		}
	}

	// Key Time Stamp:
	if (ISFLAG_ON(m_flags, FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN))
	{
		m_timeStamp.start	= t;
		keydt = 0.0f;
		
		paction = (NLTRJACTION*)m_actionsArray.pFirst;
		for (i = 0; i < m_actionsArray.Size; i++, paction++)
		{
			if (IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
			{
				if (paction->m_core.m_flags&(/*FLAG_NLTRJACTION_NEED_KEY_STOP | */FLAG_NLTRJACTION_WAITED_FOR_RESTART))
				{
					f = paction->m_core.m_timeShift + getActionCommandTime(paction);
					keydt = NMAX(f, keydt);
				}
			}
		}
		m_timeStamp.end = m_timeStamp.start + keydt;
	}
	else
	{
		keydt = m_timeStamp.end - m_timeStamp.start;	// f représente la "durée" de la clef tel quelle était définie "avant" l'appel à la fonction NLTRJKEY::preCompute.
		m_timeStamp.start = t;					// t représente le nouveau timestamp.start
		m_timeStamp.end = t + keydt;				// on maintient la durée tel quelle était définie avant, on ajuste donc m_timeStamp.end
	}

	// Passe finale sur les actions qui pourraient être "en dehors" de la durée de la key
	paction = (NLTRJACTION*)m_actionsArray.pFirst;
	for (i = 0; i < m_actionsArray.Size; i++, paction++)
	{
		if (IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
		{
			if (ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_NEED_KEY_STOP))
			{
				if ((paction->m_core.m_timeShift + getActionCommandTime(paction)) > keydt )
				{
					FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_DISABLE_PRECHECK_OUT_OF_KEY_STOP_TIMESPAN);
					disable_count++;
				}
			}
		}
	}



	// Renvoie le nombre total d'actions disable.
	return disable_count;
}
/*
void NLTRJKEY::promote(NARRAY* pkin_out, NARRAY* psmp, const NLDRIVETRAINSPECS* pdtspecs)
{
	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(m_flags), NERROR_INCONSISTENT_FLAGS);
	NLTRJACTION* paction = (NLTRJACTION*)m_actionsArray.pFirst;
	for (Nu32 i = 0; i < m_actionsArray.Size; i++,paction++)
	{
		promoteAction(paction, pdtspecs, pkin_out);
	}

}
*/
/*
NLPROMOTE_KEY_ACTION_CFG NLTRJKEY::promote(NARRAY* pkin_out, const NLPROMOTE_KEY_ACTION_CFG context_cfg, const NARRAY* psmp_array)
{

	//	* **********************************************************************************************************************************************************
	//	*
	//	* a)	Pour quelques vérifications de + ...
	//	*
	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(m_flags), NERROR_INCONSISTENT_FLAGS);														// seule une key STOP peut prétendre à la promotion	
	NErrorIf(!IS_NLTRJKEY_ENABLE(m_flags), NERROR_INCONSISTENT_FLAGS);															// Aucune key disable ne devrait être appelée à promote!	
	
	NErrorIf(!IS_NLPROMOTE_KEY_ACTION_RESULTS_CONTINUE(context_cfg), NERROR_INCONSISTENT_FLAGS);								// On ne passe ici que si il y a des choses à faire !
	NErrorIf(m_actionsArray.Size && (GET_NLPROMOTEACTION_RESULTS_ACTION_IDX(context_cfg) > m_actionsArray.Size - 1), NERROR_ARRAY_INDEX_BEYOND_LIMITS);	// la première action à traiter par ce promote est hors array !
	
	NErrorIf(pkin_out->ElementSize		!= sizeof(NLKIN), NERROR_ARRAY_WRONG_ELEMENT_SIZE);										// pkin_out, doit être un array de NLKIN
	NErrorIf(psmp_array->ElementSize	!= sizeof(NLSMOTIONPROFILE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);							// psmp_array, doit être un array de NLSMOTIONPROFILE 

	//	* **********************************************************************************************************************************************************
	//	*
	//	* b)	La promotion d'une clef se passe à plusieurs niveaux.
	//	*					NLTRAJECTORY::build		prend en charge la configuration du contexte.
	//  *											pour chaque NLTRJKEY active.
	//	*																appelle la méthode NLTRJKEY::promote
	//	*																	(cette fonction) effectue la promotion de chaque action contenue dans cette NLTRJKEY en appelant la méthode PromoteAction.
	//  *																	Le cas échéant
	//	*					
 	//	*		In fine, le but est de générer les points de trajectoires ( description de ... ) correspondant aux ordres ( les actions ) contenus dans la clefs.
	//	*		Par exemple, une clef contenant une action "Rotate" va générer un certains nombre de Points de trajectoire ( tous à la même position mais avec des valeurs Kins traduisant la rotation attendue ).
	//	*		
	//	*		Ce processus à plusieurs niveau
	//	*												KEY:Promote --> ACtion
	Nu32 firstactionid = GET_NLPROMOTEACTION_RESULTS_ACTION_IDX(context_cfg);
	NLPROMOTE_KEY_ACTION_CFG promote_cfg	= 0;
	Nu32 kidx				= pkin_out->Size;
	Nu32 iskin				= FALSE;
	
	NLTRJACTION* paction	= (NLTRJACTION*)m_actionsArray.pFirst + firstactionid;
	for (Nu32 i = firstactionid; i < m_actionsArray.Size; i++, paction++)
	{
		//	b.1)	promote action fait 2 choses: 
		//				- Elle vérifie que la configuration recquise par l'action est en phase avec la configuration du context actuel (DT/KTYPE). Si ce n'est pas le cas elle demande à l'updater.
		//				- Elle effectue la promotion des Kins de l'action (SSI la configuration du contexte actuel est conforme. )
		promote_cfg = promoteAction(pkin_out, paction, context_cfg, psmp_array);

		//	b.2)	Promote action, en retour, peut demander une mise à jour de la configuration du context.
		if (IS_NLPROMOTE_KEY_ACTION_RESULTS_REQUEST_CTXTCFG_UPDATE(promote_cfg))
		{
			//		Si c'est le cas, on construit une valeur de retour incluant:
			//		  [ promote_cfg ]	- le context attendu ( 4 premiers bits de promote_cfg )
			//				[ iskin ]	- un FLAG indiquant si des KINS ont déjà été créés par une/des promotions réussies d'actions précédentes AU COURS DE CE CYCLE. 
			//		 		 [ kidx ]	- L'index du premier KIN ajouté 
			//				 [ TRUE ]	- un FLAG à TRUE pour demander à prendre en compte le nouveau contexte attendu.	
			//				 [ TRUE ]	- un FLAG à TRUE pour indiquer qu'il reste des actions ( au moins celle en cours ) à traiter.
			//					[ i ]	- L'index de l'action à partir du quel le prochain CYCLE de promotion devra démarrer ( ... lors du prochain passage dans cette fonction )
			return MAKE_NLPROMOTE_KEY_ACTION_RESULTS(i,TRUE,TRUE, kidx, iskin, promote_cfg);
		}
		else if (IS_NLPROMOTE_KEY_ACTION_RESULTS_CREATE_KIN(promote_cfg))
		{
			iskin = 1;
		}
	}

	// Passer ici signifie que toutes les actions de la clef ont été traitées.
	// 
	return MAKE_NLPROMOTE_KEY_ACTION_RESULTS(0, FALSE, FALSE,kidx, iskin, 0);
}
*/
/*
Nu32 NLTRJKEY::oldpromote(NARRAY* pkin_out, Nu32* pmainparams, const NARRAY* psmp_array, const Nu32 firstactionid, const NLDRIVETRAINSPECS* pdtspecs)
{
	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(m_flags), NERROR_INCONSISTENT_FLAGS);
	NErrorIf(pkin_out->ElementSize != sizeof(NLKIN), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(psmp_array->ElementSize != sizeof(NLSMOTIONPROFILE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	

	if (!m_actionsArray.Size)
		return NVOID;

	NINTERVALf32	subtimeline;
	Nf32			subtimestamp		= 0.0f;
	Nf32			maintimestamp		= 0.0f;
	Nf32			actiontimelength	= 0.0f;
	Nu32			mainparams = *pmainparams;
	Nu32			i = NVOID;

	subtimeline.start	= 0.0f;
	subtimeline.end		= 0.0f;

	NErrorIf(firstactionid >= m_actionsArray.Size ,NERROR_SYSTEM_GURU_MEDITATION);
	NLTRJACTION* paction = (NLTRJACTION*)NGetArrayPtr(&m_actionsArray, firstactionid);
	for ( i = firstactionid; i < m_actionsArray.Size; i++, paction++)
	{
		if (!IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
			continue;

		/ ***********************************************************************************************************************************************************
		* 
		* 0)	Est-il nécéssaire ( et possible ) de changer le Mode de fonctionnement principal (Core)  ?
		*		Seules les actions REVERSE et FORWARD changent le Mode de fonctionnement: Elles switchent le sens des moteurs !
		*		!	Une action flaggée "FLAG_NLTRJACTION_SET_CORE_MODE" modifie les paramètres du chunk associé, il faudra donc potentiellement en créer un
		*			nouveau pour prendre en compte effectivement ces modifications de paramètres.
		*		!	Une action flaggée "FLAG_NLTRJACTION_SET_CORE_MODE" ne devrait pas:
		*					Etre flaggée comme pouvant être relative à la timeline secondaire (FLAG_NLTRJACTION_SUBTIMELINE_TIMESHIFT_CAPABILITIES)
		*					Etre flaggée comme étant relative à la timeline secondaire (FLAG_NLTRJACTION_SUBTIMELINE_TIMESHIFT)
		************************************************************************************************************************************************************* /
		if ( ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_ID_SET_CORE_MODE) )
		{
			// pour le moment seules 2 actions possèdent ce FLAG set to ON: 
			//																	NLTRJACTION_ID_REVERSE_MODE
			//																	NLTRJACTION_ID_FORWARD_MODE
			//
			NErrorIf(paction->m_core.m_id != NLTRJACTION_ID_REVERSE_MODE && paction->m_core.m_id != NLTRJACTION_ID_FORWARD_MODE, NERROR_SYSTEM_GURU_MEDITATION);
			// ... Et, aucune Action avec  FLAG_NLTRJACTION_SET_CORE_MODE set to ON ne peut prétendre être relative à la timeline secondaire.
			NErrorIf(ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_ID_SUBTIMELINE_TIMESHIFT_AUTHORIZED), NERROR_SYSTEM_GURU_MEDITATION);
			NErrorIf(ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_SUBTIMELINE_TIMESHIFT_ACTIVATED), NERROR_SYSTEM_GURU_MEDITATION);
			

			//PROMOTE_PARAMS_CHUNK_MODE new_core_mode = PROMOTE_PARAMS_CHUNK_MODE::NONE;
			// Pour créer "new_core_mode" on se sert de "FLAG_NLTRJACTION_ID_CORE_MODE_REVERSE" comme mask pour que seul l'état de ce BIT ...
			// ... soit transféré depuis "paction->m_core.m_flags", tous les autres BITS étant donc forcés à FALSE.

			Nu32 new_core_mode = (Nu32)(paction->m_core.m_flags & FLAG_NLTRJACTION_ID_CORE_MODE_REVERSE);
			
			switch (paction->m_core.m_id)
			{
				case NLTRJACTION_ID_REVERSE_MODE:
				{
					//new_core_mode = PROMOTE_PARAMS_CHUNK_MODE::REVERSE;
					FLAG_ON(new_core_mode, FLAG_NLTRJKEY_PROMOTE_PARAMS_CORE_MODE_REVERSE);
					break;
				}
				case NLTRJACTION_ID_FORWARD_MODE:
				{
					//new_core_mode = PROMOTE_PARAMS_CHUNK_MODE::FORWARD;
					FLAG_OFF(new_core_mode, FLAG_NLTRJKEY_PROMOTE_PARAMS_CORE_MODE_REVERSE);
					break;
				}
				default:
					NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
					break;
			}

			// Si le mode requis par l'action en cours de traitement EST DIFFERENT du mode actuellement défini pour le chunk ...
			//if (new_core_mode != (PROMOTE_PARAMS_CHUNK_MODE)NGET_NLTRJKEY_PROMOTE_PARAMS_CHUNK_MODE(mainparams))
			if (new_core_mode != (mainparams & FLAG_NLTRJKEY_PROMOTE_PARAMS_CORE_MODE_REVERSE))
			{
				// a)	Il faudra potentiellement changer de chunk si les fonctionnalités "CORE" ont déjà été sollicitées. 
				//		( ... dans ce chunk, comme l'usage des moteurs via une rotation ).
				if (ISFLAG_ON(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_CORE_MODE_USED))
				{
					*pmainparams = mainparams;
					return i;
				}
				// b)	Sinon il suffit juste de redéfinir le mode du Chunk en cours de promotion/création.
				//		et continuer la promotion, avec pour étape suivante et normale, la définition du timestamp de l'action.
				else
				{
					//NSET_NLTRJKEY_PROMOTE_PARAMS_CHUNK_MODE(mainparams, new_core_mode);
					SET_FLAGS(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_CORE_MODE_REVERSE, new_core_mode);
				}
			}
			// Si par contre, le mode requis par l'action en cours est identique au mode actuellement défini alors, ...
			// ... rien de particulier, on continue et on prendra en compte le potentiel time shift de l'action.
			/ *
			else
			{
			}
			* /
		}
		/ ***********************************************************************************************************************************************************
		* 
		* 1)	TimeStamp ( of Action start ) et Kin Promotion:
		*			Calcul de la "date" à laquelle l'action démarre soit sur la timeline principale soit sur la timeline secondaire si elle existe.
		*			Rappel: la timeline secondaire (subtimeline) est unique et ouverte par une action possedant une timelength.
		*							
		*					0------>1-------------->7--------------------->8		L'action 0 a un timestamp sur l'origine de la maintimeline.
		*							|												L'action 1 a un timestamp sur la maintimeline.	
		*							|												Les actions 2 à 6 sont sur la subtimeline ( 3,4 et 5 ayant le même timestamp )	
		*							+--2--3----->6									Les actions 2 à 6 se déclenchent donc en parallèle de l'éxécution de l'action 1 ( pendant son timelength )
		*								  4	
		*								  5	
		************************************************************************************************************************************************************* /
		if (	ISFLAG_ON(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_SUBTIMELINE_IS_OPEN) &&
				ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_SUBTIMELINE_TIMESHIFT_ACTIVATED) )
		{
			paction->m_core.m_timeStamp = subtimestamp + paction->m_core.m_timeShift;
			//actiontimelength = promoteAction(paction, pdtspecs, pkin_out);

			subtimestamp = paction->m_core.m_timeStamp + actiontimelength;

			// La time line secondaire se ferme dès qu'une action est time shiftée hors de l'intervalle de la subtimeline.
			// On notera que pour le moment la seule action pouvant se placer sur la subtimeline a une timelength nulle ( NLTRJACTION_ID_SEND_MESSAGE ).
			// Cependant l'algo. pourras supporter d'éventuelles nouvelles actions pouvant se placer sur la subtimeline et ayant une timelength non nulle !
			if ( subtimestamp > subtimeline.end )
			{
				// Le timestamp de l'action se retrouve après la fin de l'intervalle de la subtimeline.
				// Il est donc "comme appartenant" à la timeline principale.
				NErrorIf(maintimestamp < subtimeline.end, NERROR_SYSTEM_GURU_MEDITATION);
				maintimestamp = subtimestamp;

				FLAG_OFF(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_SUBTIMELINE_IS_OPEN);
				subtimestamp = 0.0f;
				subtimeline.start = 0.0f;
				subtimeline.end = 0.0f;
			}
		}
		else
		{
			paction->m_core.m_timeStamp = maintimestamp + paction->m_core.m_timeShift;
			//actiontimelength = promoteAction(paction, pdtspecs, pkin_out);

			maintimestamp = paction->m_core.m_timeStamp + actiontimelength;

			// La time line secondaire se ferme dès qu'une action est referencée sur la timeline principale.
			FLAG_OFF(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_SUBTIMELINE_IS_OPEN);
			subtimestamp = 0.0f;
			subtimeline.start	= 0.0f;
			subtimeline.end		= 0.0f;
		}

		/ ***********************************************************************************************************************************************************
		* 
		*  2)	SubTimeLine START:
		* 
		*
		************************************************************************************************************************************************************* /
		if (ISFLAG_ON(paction->m_core.m_id, FLAG_NLTRJACTION_ID_START_SUBTIMELINE) && (actiontimelength!=0.0f) )
		{
			//	Il ne devrait pas être possible de se retrouver dans cette situation:
			//	En effet, seule une action sur maintimeline ( et donc avec subtimeline fermée ) est censée pouvoir "ouvrir" la subtimeline.
			//	or, si la subtimeline est ouverte cela signifie que "paction" EST SUR subtimeline ! 
			// RAPPEL: 
			//		La time line secondaire se ferme dès qu'une action est referencée sur la timeline principale.
			//
			NErrorIf(ISFLAG_ON(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_SUBTIMELINE_IS_OPEN), NERROR_SYSTEM_GURU_MEDITATION);

			FLAG_ON(mainparams, FLAG_NLTRJKEY_PROMOTE_PARAMS_SUBTIMELINE_IS_OPEN);
			subtimestamp = paction->m_core.m_timeStamp;
			subtimeline.start	= subtimestamp;
			subtimeline.end		= subtimestamp + actiontimelength;
		}
	}
	*pmainparams = mainparams;
	return NVOID; // NVOID pour stipuler : C'est terminé !
}
*/
Nu32 NLTRJKEY::SpotRequirement()
{
	NLTRJACTION* paction = (NLTRJACTION*)m_actionsArray.pFirst;
	for (Nu32 i = 0; i < m_actionsArray.Size; i++, paction++)
	{
	//	if (IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
	//	{
			switch (paction->m_core.m_id)
			{
				case NLTRJACTION_ID_NULL:
					break;
				case NLTRJACTION_ID_FORWARD_MODE:
					break;
				case NLTRJACTION_ID_REVERSE_MODE:
					break;
				case NLTRJACTION_ID_ROTATE:
					return 1;
				case NLTRJACTION_ID_BACK_AND_FORTH:
					return 1;
				case NLTRJACTION_ID_SEND_MESSAGE:
					break;
			}
	//	}
	}
	return 0;
}

void NLTRJKEY::draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
}

void NLTRJKEY::draw(const NLPATH* ppath, const NLGFX *pgfx, const Nu32 index)
{

	NVEC2f32		u0,u,ux;
	NUTDRAWVERTEX	v0,v;
	Nf32 size		= pgfx->m_currentSize;
	v0.Position_3f.z= 0.0f;
	v.Position_3f.z = 0.0f;
	if (IS_NLTRJKEY_ENABLE(m_flags))
	{
		v.Color0_4f = v0.Color0_4f = *pgfx->getColor(0);
	}
	else
	{
		v.Color0_4f = v0.Color0_4f = *pgfx->getColor(1);
	}

	NLPATH_POINT point;
	ppath->m_geometry.getPathPoint(&point, m_s);
	ppath->convertLocalPositionToGlobal((NVEC2*)&v0.Position_3f, &point.p);
	ppath->convertLocalDirectionToGlobal(&u0, &point.u);
	
	u.x = u0.x * size;
	u.y = u0.y * size;
	ux.x= u0.x * size * 0.075f;
	ux.y= u0.y * size * 0.075f;

	// Affichage index de key ?
	if (index != NVOID)
	{
		NUT_DRAWTEXT	drawtxt_index;
		Nchar			str[8];
		sprintf(str, "%d", index);
		drawtxt_index.Color = *pgfx->getColor(5);
		drawtxt_index.Size	= pgfx->m_currentTextSize;

		v.Position_3f.x = v0.Position_3f.x - u.y*1.5f;
		v.Position_3f.y = v0.Position_3f.y + u.x*1.5f;

		NUT_Draw_Text(str, &v.Position_3f, &drawtxt_index);
	}
	
	// Partie commune
	NUT_DrawPencil_From(&v0);
	v.Position_3f.x = v0.Position_3f.x - u.y - u.x;
	v.Position_3f.y = v0.Position_3f.y + u.x - u.y;
	NUT_DrawPencil_LineTo(&v);
	v.Position_3f.x = v0.Position_3f.x - u.y + u.x;
	v.Position_3f.y = v0.Position_3f.y + u.x + u.y;
	NUT_DrawPencil_LineTo(&v);
	NUT_DrawPencil_LineTo(&v0);

	if (ISFLAG_ON(m_flags, FLAG_NLTRJKEY_STOP))
	{
		v0.Position_3f.x += -u.x*0.5f + ux.y ;
		v0.Position_3f.y += -u.y*0.5f - ux.x;
		NUT_DrawPencil_From(&v0);

		v.Position_3f.x = v0.Position_3f.x + u.x;
		v.Position_3f.y = v0.Position_3f.y + u.y;
		NUT_DrawPencil_LineTo(&v);
		v.Position_3f.x += u.y;
		v.Position_3f.y -= u.x;
		NUT_DrawPencil_LineTo(&v);
		v.Position_3f.x -= u.x;
		v.Position_3f.y -= u.y;
		NUT_DrawPencil_LineTo(&v);
		NUT_DrawPencil_LineTo(&v0);
	}
	else
	{
		v0.Position_3f.x +=   ux.y;
		v0.Position_3f.y +=  -ux.x;
		NUT_DrawPencil_From(&v0);

		u.x *= 0.75f;
		u.y *= 0.75f;

		v.Position_3f.x = v0.Position_3f.x + u.y - u.x;
		v.Position_3f.y = v0.Position_3f.y - u.x - u.y;
		NUT_DrawPencil_LineTo(&v);
		v.Position_3f.x = v0.Position_3f.x + u.y + u.x;
		v.Position_3f.y = v0.Position_3f.y - u.x + u.y;
		NUT_DrawPencil_LineTo(&v);
		NUT_DrawPencil_LineTo(&v0);
	}
}
