#include "../../../N/NErrorHandling.h"
#include "../../../N/Core/NLimits.h"
#include "../../../N/NMemory.h"
#include "../MotionProfiles/NLSmotionProfile.h"
#include "../DriveTrain/NLDriveTrainSpecs.h"
#include "../NLKinTools.h"

#include "NLTrajectoryAction.h"

Nu32 isActionEfficient(const NLTRJACTION* paction)
{
	switch (paction->m_core.m_id)
	{
		case NLTRJACTION_ID_NULL:				// L'action Nulle n'a aucune efficacité ! Elle ne fait rien.
			return 0;																					

		case NLTRJACTION_ID_FORWARD_MODE:		// L'action FORWARD MODE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )
			return 1;

		case NLTRJACTION_ID_REVERSE_MODE:		// L'action REVERSE MODE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )
			return 1;

		case NLTRJACTION_ID_ROTATE:				// L'action ROTATE n'est efficace que si ses parametres sont tous les deux non-nulls
			if (paction->m_rotate.m_angle && paction->m_rotate.m_speedRatio)
				return 1;
			else
				return 0;

		case NLTRJACTION_ID_BACK_AND_FORTH:		// L'action Aller et retour n'est efficace que si son param' PAUSE est non null ou si ses param' distance et speedration sont tous 2 non nulls
			if ( paction->m_backAndForth.m_pause || (paction->m_backAndForth.m_distance && paction->m_backAndForth.m_speedRatio) )
				return 1;
			else 
				return 0;

		case NLTRJACTION_ID_SEND_MESSAGE:		// L'action SEND MESSAGE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )	
			return 1;							// Cela implique que "paction->m_sendMessage.m_message" est toujours considéré comme valide et "efficace"

		default:
			NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
			break;
	}
	return 0;
}

void preComputeAction(NLTRJACTION* paction, const Nf32 t0,  const NLDRIVETRAINSPECS* pdtspecs)
{
	paction->m_core.m_timeStamp = t0 + paction->m_core.m_timeShift;

	if(paction->m_core.m_id == NLTRJACTION_ID_ROTATE)
	{
		paction->m_rotate.m_sMotionProfile.initialize();

		if ((paction->m_rotate.m_angle) && (paction->m_rotate.m_speedRatio))
		{
			Nf32 arclength = NDEGtoRAD(paction->m_rotate.m_angle) * pdtspecs->m_axleTrack / 2.0f;
			if (arclength < 0.0f)
				FLAG_ON(paction->m_rotate.m_flags, FLAG_NLTRJACTION_ID_ROTATE_CW);
			else
				FLAG_OFF(paction->m_rotate.m_flags, FLAG_NLTRJACTION_ID_ROTATE_CW);
			
			NLKINLIMITS	locallimits(pdtspecs->m_limits);
			locallimits.setVelocityMax(pdtspecs->m_limits.m_v * paction->m_rotate.m_speedRatio);
			paction->m_rotate.m_sMotionProfile.sRamped(&locallimits, 0.0f, 0.0f, 0.0f, NABS(arclength), NULL, NULL);
		}
	}
}
/*
Nu32 extractPromoteActionContextConfig(const NLTRJACTION* paction, const Nu32 context_config)
{
	NErrorIf((context_config & MASK_NLTRJACTION_CONFIG) != context_config, NERROR_WRONG_VALUE);	// Context_config doit uniquement définir DT Mode et Ktype ( BIT_0 to BIT_3 ). Rien de plus !
	NErrorIf(!GET_NLTRJACTION_DT_MODE(context_config), NERROR_WRONG_VALUE);						// Context_config doit être complètement défini ! ( pas égal à zero ! )
	NErrorIf(GET_NLTRJACTION_KTYPE(context_config), NERROR_WRONG_VALUE);						// Context_config doit être complètement défini ! ( pas égal à zero ! )
	
	Nu32 rcfg = 0;

	// Si l'action ne spécifie pas de DT Mode, on défini le DT mode de la configuration de l'Action, comme étant égal au DT Mode courant( celui du context_config ).
	// Sinon on récupère le DT Mode spécifié dans les flags de l'action.
	if (GET_NLTRJACTION_DT_MODE(paction->m_core.m_flags) == 0)
		rcfg |= GET_NLTRJACTION_DT_MODE(context_config);
	else
		rcfg |= GET_NLTRJACTION_DT_MODE(paction->m_core.m_flags);

	// Si l'action ne spécifie pas de KType, on défini le KTypede la configuration de l'Action, comme étant égal au KType courant( celui du context_config ).
	// Sinon on récupère le KType spécifié dans les flags de l'action.
	if (GET_NLTRJACTION_KTYPE(paction->m_core.m_flags) == 0)
		rcfg |= GET_NLTRJACTION_KTYPE(context_config);
	else
		rcfg |= GET_NLTRJACTION_KTYPE(paction->m_core.m_flags);

	return rcfg;
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief		La promotion d'une action génère :
 *							a-	Tous les kins associé à l'action et les insère dans l'array pkin_out.
 *							b-	Les flags de configuration qui définissent le contexte dans lequel elle doit/peut effectuer sa "promotion'.
 *
 *				Selon son type, une action va faire : a et b, a ou b, ni a et ni b.
 *				Ainsi, par ID on a:							
 *																	a		b			Rmqs
 *							NLTRJACTION_ID_NULL						.		.			Ni a ni b, l'action NULL ne fait rien du tout. Sa promotion renvoie 0	
 * 							NLTRJACTION_ID_FORWARD_MODE				.		X			b, les 2 actions effectuant le setup de la DT ne génèrent aucun KIN mais (re)définissent le DT Mode.
 * 							NLTRJACTION_ID_REVERSE_MODE				.		X			b, idem NLTRJACTION_ID_FORWARD_MODE
 * 							NLTRJACTION_ID_ROTATE					X		X			a et b. l'action rotate génère des Kin et renvoie les flags de configuration 
 * 							NLTRJACTION_ID_SEND_MESSAGE				.		.			L'action send message	
 * 
 *				la structure NLTRJACTION étant une union "C-Style", la fonction promoteAction 
 *				est également et simplement "C-Style".
 * 
 * 	@param		pkin_out est un pointeur sur NARRAY ou sont placés les kins générés.
 *	@param		paction est un pointeur sur l'action à traiter. 
 *  @param		current_context_cfg	est une valeur 32 bits où seuls les 4 premiers BITS de poids faibles sont utilisés. Ces 4 bits indique le contexte dans lequel l'action
 *				effectuera sont promote. Si le contexte n'est pas compatible, aucune promotion n'est effectuée et le code d'erreur INCOMPATIBLE CONTEXT est retourné.
 *	@param		pdtspecs est un pointeur sur les specs de la DriveTrain ( l'entraxe est nécéssaire pour le calcul des actions impliquant une rotation ).
 *	@param		psmp_array est un pointeur sur NARRAY ou sont placés les motion profile de rotation precomputés.
 * 
 * @return		PROMOTEACTION_RESULTS une valeur 32 bits où sont compactées les informations suivantes:
 *							BIT_0	à BIT_15		16 flags de configuration
 *							BIT_16	à BIT_31		index du premier kin généré par la promotion dan sl'array pkin_out.
 * 
 *				à propos des 16 flags de configuration:
 *							BIT_0		|	TRUE: 1 ou plusieurs KIN ont été générés lors de la promotion		FALSE: l'action n'a généré aucun KIN
 * 
 *							BIT_1		|_	copie des BIT_1 et BIT_2 de la valeur 'flags' membre de NLTRJACTION. ( soit FLAG_NLTRJACTION_SET_DT_MODE et FLAG_NLTRJACTION_SET_DT_MODE )
 *							BIT_2		|	...le nombre sur 2 bits	[BIT_1|BIT_2] peut prendre prendre 3 valeurs utiles décrivant si l'action défini (SET) le DT Mode (ou pas) et comment elle le défini.
 * 
 *											BIT_1 |	BIT_2
 *											------+-----------------------------------------------------------
 *											FALSE	FALSE	L'action ne (re)défini pas le DT Mode					=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = 0
 *											FALSE	TRUE	L'action (re)défini pas le DT Mode en Mode FORWARD		=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = FLAG_NLTRJACTION_SET_DT_MODE
 *											TRUE	TRUE	L'action (re)défini pas le DT Mode en Mode REVERSE		=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = FLAG_NLTRJACTION_SET_DT_MODE|FLAG_NLTRJACTION_SET_DT_MODE
 *											
 *											On remarquera que BIT_2 est systématiquement TRUE en cas de (re)définition du DT Mode et systématiquement OFF dans le cas contraire.
 *																				
 */	
 // ------------------------------------------------------------------------------------------
NLTRJPOINT_DESC_CFG promoteAction(const NLTRJPOINT_DESC_CFG context_cfg, const NLTRJACTION* paction, const Nf32 t, NARRAY* pkin_out)
{
	NErrorIf((context_cfg & MASK_NLTRJPOINT_DESC_CONFIG) != context_cfg, NERROR_WRONG_VALUE);	// Context_cfg doit uniquement définir DT Mode et Ktype ( BIT_0 to BIT_3 ). Rien de plus !
	NErrorIf(!GET_NLTRJPOINT_DESC_DT_MODE(context_cfg), NERROR_WRONG_VALUE);					// Context_cfg doit être complètement défini ! ( pas égal à zero ! )
	NErrorIf(!GET_NLTRJPOINT_DESC_KTYPE(context_cfg), NERROR_WRONG_VALUE);						// Context_cfg doit être complètement défini ! ( pas égal à zero ! )

	// Extract NLPROMOTEACTION_CFG Data:
//	Nu16 kincount = GET_NLPROMOTEACTION_CFG_KEY_KIN_COUNT(context_cfg);
//	Nu16 cfg	  = GET_NLPROMOTEACTION_CFG_CONFIG(context_cfg);

	if (paction->m_core.m_id == NLTRJACTION_ID_NULL)
		return MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, context_cfg);

	if (!IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
		return MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, context_cfg);

	//	* **********************************************************************************************************************************************************
	//	*
	//	* a)	Définition de la configuration du contexte requis pas l'action:
	//	*
	Nu32 action_cfg = 0;

	//	*		Si l'action ne spécifie pas de DT Mode, on défini le DT mode de la configuration de l'Action, comme étant égal au DT Mode courant( celui du context_config ).
	//	*		Sinon on récupère le DT Mode spécifié dans les flags de l'action.
	if (GET_NLTRJPOINT_DESC_DT_MODE(paction->m_core.m_flags) == 0)
		action_cfg |= GET_NLTRJPOINT_DESC_DT_MODE(context_cfg);
	else
		action_cfg |= GET_NLTRJPOINT_DESC_DT_MODE(paction->m_core.m_flags);

	//	*		Si l'action ne spécifie pas de KType, on défini le KType de la configuration de l'Action, comme étant égal au KType courant( celui du context_config ).
	//	*		Sinon on récupère le KType spécifié dans les flags de l'action.
	if (GET_NLTRJPOINT_DESC_KTYPE(paction->m_core.m_flags) == 0)
		action_cfg |= GET_NLTRJPOINT_DESC_KTYPE(context_cfg);
	else
		action_cfg |= GET_NLTRJPOINT_DESC_KTYPE(paction->m_core.m_flags);

	//	* **********************************************************************************************************************************************************
	//	*
	//	* b)	Vérification de la configuration requise avec la configuration courante:
	//	*
	//	*		Si la configuration recquise est différente de la configuration courante, l'action renvoie immédiatement une demande de mise à jour de la configuration
	//	*		En retournant le code de retour adapté assoscié à la configuration attendue.
	/*
	if(action_cfg != context_cfg)
		return MAKE_NLPROMOTE_KEY_ACTION_RESULTS(	0,				// |_Action data:	Non utilisés ici ( cntn = FALSE )
													FALSE,			// |
													TRUE,			// Demande d'update de la config !
													0,				// |_Kin data:		Non utilisés ici ( iskin = FALSE )
													FALSE,			// |
													action_cfg);	// la config attendue !
													*/
	//	*		Sinon, c'est à dire dans le cas où la configuration du context actuel est conforme aux attentes de l'action : On continue !

	//	* **********************************************************************************************************************************************************
	//	*
	//	* c)	Promotion :
	//	*
	//	*		Toutes les actions n'effectuent pas de promotion de Kins. La suite se traite donc selon l'id de l'action ...
	//	*		

	NLKIN				*pkin;
	NLKIN				kin;
	Nu32				size = 0;
	Nf32				timeshift = 0.0f;
	Nf32				timelength = 0.0f;

	if(paction->m_core.m_id == NLTRJACTION_ID_ROTATE)
	{
		if ((paction->m_rotate.m_angle) && (paction->m_rotate.m_speedRatio))
		{
			switch (paction->m_rotate.m_sMotionProfile.m_id)
			{
				case NLSMOTIONPROFILE_ID_ENUM::SUP_SDOWN:
					size = NLSMOTIONPROFILE_SUP_SDOWN__MAX_PROMOTED_KINS;
					break;

				case NLSMOTIONPROFILE_ID_ENUM::SUP_FLAT_SDOWN:
					size = NLSMOTIONPROFILE_SUP_FLAT_SDOWN__MAX_PROMOTED_KINS;
					break;

				default:
					size = 0;
					break;
			}

			if (size)
			{
				Nu32	kidx = pkin_out->Size;
				NLKIN	kin;
				kin.null();
				kin.m_t = t;
				NEnsureArrayCapacity(pkin_out, pkin_out->Size + size + 1);
				pkin = (NLKIN*)NArrayPushBack(pkin_out, (NBYTE*)&kin);
				size = paction->m_rotate.m_sMotionProfile.ppromote(&((NLTRJACTION*)paction)->m_rotate.m_sMotionProfile, pkin);
				NUpSizeArray(pkin_out, size, NULL);
				if (ISFLAG_ON(paction->m_rotate.m_flags, FLAG_NLTRJACTION_ID_ROTATE_CW))
				{
					size += 1; // pour prendre en compte le push du premier kin
					for (Nu32 i = 0; i < size; i++, pkin++)
						pkin->reverse();
				}
				return MAKE_NLTRJPOINT_DESC_CFG(kidx, TRUE, action_cfg);
			}
		}
	}
	else if (paction->m_core.m_id == NLTRJACTION_ID_BACK_AND_FORTH)
	{

	}

	return MAKE_NLTRJPOINT_DESC_CFG(0,FALSE, action_cfg);
}

/*
#define FLAG_NLTRJACTION_ID_NEED_KEY_STOP									BIT_0	// 

#define FLAG_NLTRJACTION_ID_CORE_MODE_REVERSE								BIT_1
#define FLAG_NLTRJACTION_ID_SET_CORE_MODE									BIT_2

#define FLAG_NLTRJACTION_ID_LOCK_DRIVETRAIN									BIT_3	// Une Action dont ce flag est ON prend le contrôle de la base. La durée de cette prise de contrôle peut-être nulle ( instantanée )
#define FLAG_NLTRJACTION_ID_NON_NULL_LOCK_TIMESPAN							BIT_4	// Une Action dont ce flag est ON bloque la DT sur une certaine durée.
*/

NLTRJACTION* setupAction(NLTRJACTION* paction, const Nu16 actionid, const Nbool bfullreset )
{
	if (bfullreset)
	{
		//	Nmem0(paction, NLTRJACTION);
		paction->m_core.m_timeShift = 0.0f;
		paction->m_core.m_timeStamp = 0.0f;
	}

	// Core
	paction->m_core.m_id	= actionid;

	switch (actionid)
	{
		case NLTRJACTION_ID_NULL:
			paction->m_core.m_flags = 0;																// CFG: This action doesn't set DT Mode.
																										// CFG: This action doesn't set KType because it doesn't create any Kin.
			break;

		case NLTRJACTION_ID_FORWARD_MODE:
			paction->m_core.m_flags  = FLAG_NLTRJACTION_NEED_KEY_STOP;									// This action needs the Robot be stopped to performed.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND;										// This action is a Command. that means, it takes DT control. ( here is instantanneous, just to update DT Mode )
			paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								// By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_SET_DT_MODE;									// CFG: This action set the Drive Train Mode ( to Forward ). 
																										// CFG: This action doesn't set KType because it doesn't create any Kin.
			break;																			
																							
		case NLTRJACTION_ID_REVERSE_MODE:													
			paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									// This action needs the Robot be stopped to performed.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND;										// This action is a Command. that means, it takes DT control. ( here is instantanneous, just to update DT Mode )
			paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								// By default. This action is supposed to be integrated into the Key time span.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_SET_DT_MODE| FLAG_NLTRJACTION_DT_MODE_REVERSE;	// CFG:	This action set the Drive Train Mode( to Reverse ).
																										// CFG: This action doesn't set KType because it doesn't create any Kin.
			break;

		case NLTRJACTION_ID_ROTATE:
			paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									// This action needs the Robot be stopped to performed.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND|FLAG_NLTRJACTION_COMMAND_LONG_RUNNING;	// This action is a Command. that means, it takes DT control. ( here is long running, during all the period covered by the kins )
			paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								// By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_KTYPE_SPOT;										// CFG: Kin created by this action must be interpreted with the KType Spot.
																										// CFG: This action doesn't set DT Mode.
			if (bfullreset)
			{
				// réinitialisation des valeurs !
				paction->m_rotate.m_angle = 0.0f;
				paction->m_rotate.m_speedRatio = 1.0f;
				paction->m_rotate.m_sMotionProfile.initialize();
			}
			else
			{
				// On ne réinitialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
				// ( ne pas oublier que action est une union !)
				paction->m_rotate.m_angle		= NCLAMP(-360.0f, paction->m_rotate.m_angle, 360.0f);
				paction->m_rotate.m_speedRatio	= NCLAMP(0.0f, paction->m_rotate.m_speedRatio, 1.0f);
			}
			break;

		case NLTRJACTION_ID_BACK_AND_FORTH:
			paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									// This action needs the Robot be stopped to performed.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND|FLAG_NLTRJACTION_COMMAND_LONG_RUNNING;	// This action is a Command. that means, it takes DT control. ( here is long running, during all the period covered by the kins )
			paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								// By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
			paction->m_core.m_flags |= FLAG_NLTRJACTION_KTYPE_SPOT| FLAG_NLTRJACTION_KTYPE_TRAVELING;	// CFG: Kin created by this action must be interpreted with the KType SpotExtended.
																										// CFG: This action doesn't set DT Mode.

			if (bfullreset)
			{
				paction->m_backAndForth.m_distance = 1.0f;
				paction->m_backAndForth.m_speedRatio = 1.0f;
				paction->m_backAndForth.m_pause = 0.0f;
			}
			else
			{
				// On ne réinitialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
				// ( ne pas oublier que action est une union !)
				paction->m_backAndForth.m_distance		= NCLAMP(0.0f, paction->m_backAndForth.m_distance, 16.0f); // 16 m (distance arbitraire egal grosso modo à la longueur du terrain )
				paction->m_backAndForth.m_speedRatio	= NCLAMP(0.0f, paction->m_backAndForth.m_speedRatio, 1.0f);
				paction->m_backAndForth.m_pause			= NCLAMP(0.0f, paction->m_backAndForth.m_pause, 60.0f);
			}
			break;

		case NLTRJACTION_ID_SEND_MESSAGE:
			paction->m_core.m_flags = 0;																// CFG: This action doesn't set DT Mode.
																										// CFG: This action doesn't set KType because it doesn't create any Kin.
			if (bfullreset)
			{
				paction->m_sendMessage.m_message = 0;
			}
			else
			{
				// On ne réinitialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
				// ( ne pas oublier que action est une union !)
				paction->m_sendMessage.m_message = NCLAMP(0, paction->m_sendMessage.m_message, (NLTRJACTION_NUMBER_OF_ID-1));
			}

			break;

		default:
			NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
			break;
	}
	return paction;
}

NLTRJACTION* createAction(const Nu16 actionid)
{
	return setupAction( (NLTRJACTION*)Nmalloc(sizeof(NLTRJACTION)), actionid, NTRUE );
}

void clearAction(NLTRJACTION* paction)
{
	Nmem0(paction, NLTRJACTION);
}

void deleteAction(NLTRJACTION* paction)
{
	clearAction(paction);
	Nfree(paction);
}


Nf32 getActionCommandTime(const NLTRJACTION* paction)
{
	if(paction->m_core.m_id== NLTRJACTION_ID_ROTATE )
	{
		NErrorIf(!IS_NLTRJACTION_COMMAND_LONG_RUNNING(paction->m_core.m_flags), NERROR_INCONSISTENT_FLAGS);
		Nf32 f = paction->m_rotate.m_sMotionProfile.getEffectiveTimeSpan();
		return (f < NF32_MAX) ? f : 0.0f;	// f  = NF32_MAX signifie que le motion profile à été initialisé mais pas calculé.
											// En d'autres termes il est "vide" / "null".
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(IS_NLTRJACTION_COMMAND_LONG_RUNNING(paction->m_core.m_flags), NERROR_INCONSISTENT_FLAGS);
		// back and forth peut faire ça ... il faudrait l'implémenter !!!
	}
	#endif
	return 0.0f;
}
