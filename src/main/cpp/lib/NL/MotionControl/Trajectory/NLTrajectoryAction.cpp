#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

#include "lib/N/NMemory.h"
#include "lib/NL/MotionControl/MotionProfiles/NLSmotionProfile.h"
#include "lib/NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "lib/NL/MotionControl/NLKinTools.h"

#include "lib/NL/MotionControl/Trajectory/NLTrajectoryAction.h"

Nu32 isActionEfficient(const NLTRJACTION *paction)
{
	switch (paction->m_core.m_id)
	{
	case NLTRJACTION_ID_NULL: // L'action Nulle n'a aucune efficacit� ! Elle ne fait rien.
		return 0;

	case NLTRJACTION_ID_FORWARD_MODE: // L'action FORWARD MODE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )
		return 1;

	case NLTRJACTION_ID_REVERSE_MODE: // L'action REVERSE MODE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )
		return 1;

	case NLTRJACTION_ID_ROTATE: // L'action ROTATE n'est efficace que si ses parametres sont tous les deux non-nulls
		if (paction->m_rotate.m_angle && paction->m_rotate.m_speedRatio)
			return 1;
		else
			return 0;

	case NLTRJACTION_ID_BACK_AND_FORTH: // L'action Aller et retour n'est efficace que si son param' PAUSE est non null ou si ses param' distance et speedration sont tous 2 non nulls
		if (paction->m_backAndForth.m_pause || (paction->m_backAndForth.m_distance && paction->m_backAndForth.m_speedRatio))
			return 1;
		else
			return 0;

	case NLTRJACTION_ID_SEND_MESSAGE: // L'action SEND MESSAGE est toujours efficace, elle n'a aucun param'( qui pourrait la rendre inefficace )
		return 1;					  // Cela implique que "paction->m_sendMessage.m_message" est toujours consid�r� comme valide et "efficace"

	default:
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		break;
	}
	return 0;
}

void preComputeAction(NLTRJACTION *paction, const Nf32 t0, const NLDRIVETRAINSPECS *pdtspecs)
{
	paction->m_core.m_timeStamp = t0 + paction->m_core.m_timeShift;

	if (paction->m_core.m_id == NLTRJACTION_ID_ROTATE)
	{
		paction->m_rotate.m_sMotionProfile.initialize();

		if ((paction->m_rotate.m_angle) && (paction->m_rotate.m_speedRatio))
		{
			Nf32 arclength = NDEGtoRAD(paction->m_rotate.m_angle) * pdtspecs->m_axleTrack / 2.0f;
			if (arclength < 0.0f)
				FLAG_ON(paction->m_rotate.m_flags, FLAG_NLTRJACTION_ID_ROTATE_CW);
			else
				FLAG_OFF(paction->m_rotate.m_flags, FLAG_NLTRJACTION_ID_ROTATE_CW);

			NLKINLIMITS locallimits(pdtspecs->m_limits);
			locallimits.setVelocityMax(pdtspecs->m_limits.m_v * paction->m_rotate.m_speedRatio);
			paction->m_rotate.m_sMotionProfile.sRamped(&locallimits, 0.0f, 0.0f, 0.0f, NABS(arclength), NULL, NULL);
		}
	}
}
/*
Nu32 extractPromoteActionContextConfig(const NLTRJACTION* paction, const Nu32 context_config)
{
	NErrorIf((context_config & MASK_NLTRJACTION_CONFIG) != context_config, NERROR_WRONG_VALUE);	// Context_config doit uniquement d�finir DT Mode et Ktype ( BIT_0 to BIT_3 ). Rien de plus !
	NErrorIf(!GET_NLTRJACTION_DT_MODE(context_config), NERROR_WRONG_VALUE);						// Context_config doit �tre compl�tement d�fini ! ( pas �gal � zero ! )
	NErrorIf(GET_NLTRJACTION_KTYPE(context_config), NERROR_WRONG_VALUE);						// Context_config doit �tre compl�tement d�fini ! ( pas �gal � zero ! )

	Nu32 rcfg = 0;

	// Si l'action ne sp�cifie pas de DT Mode, on d�fini le DT mode de la configuration de l'Action, comme �tant �gal au DT Mode courant( celui du context_config ).
	// Sinon on r�cup�re le DT Mode sp�cifi� dans les flags de l'action.
	if (GET_NLTRJACTION_DT_MODE(paction->m_core.m_flags) == 0)
		rcfg |= GET_NLTRJACTION_DT_MODE(context_config);
	else
		rcfg |= GET_NLTRJACTION_DT_MODE(paction->m_core.m_flags);

	// Si l'action ne sp�cifie pas de KType, on d�fini le KTypede la configuration de l'Action, comme �tant �gal au KType courant( celui du context_config ).
	// Sinon on r�cup�re le KType sp�cifi� dans les flags de l'action.
	if (GET_NLTRJACTION_KTYPE(paction->m_core.m_flags) == 0)
		rcfg |= GET_NLTRJACTION_KTYPE(context_config);
	else
		rcfg |= GET_NLTRJACTION_KTYPE(paction->m_core.m_flags);

	return rcfg;
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief		La promotion d'une action g�n�re :
 *							a-	Tous les kins associ� � l'action et les ins�re dans l'array pkin_out.
 *							b-	Les flags de configuration qui d�finissent le contexte dans lequel elle doit/peut effectuer sa "promotion'.
 *
 *				Selon son type, une action va faire : a et b, a ou b, ni a et ni b.
 *				Ainsi, par ID on a:
 *																	a		b			Rmqs
 *							NLTRJACTION_ID_NULL						.		.			Ni a ni b, l'action NULL ne fait rien du tout. Sa promotion renvoie 0
 * 							NLTRJACTION_ID_FORWARD_MODE				.		X			b, les 2 actions effectuant le setup de la DT ne g�n�rent aucun KIN mais (re)d�finissent le DT Mode.
 * 							NLTRJACTION_ID_REVERSE_MODE				.		X			b, idem NLTRJACTION_ID_FORWARD_MODE
 * 							NLTRJACTION_ID_ROTATE					X		X			a et b. l'action rotate g�n�re des Kin et renvoie les flags de configuration
 * 							NLTRJACTION_ID_SEND_MESSAGE				.		.			L'action send message
 *
 *				la structure NLTRJACTION �tant une union "C-Style", la fonction promoteAction
 *				est �galement et simplement "C-Style".
 *
 * 	@param		pkin_out est un pointeur sur NARRAY ou sont plac�s les kins g�n�r�s.
 *	@param		paction est un pointeur sur l'action � traiter.
 *  @param		current_context_cfg	est une valeur 32 bits o� seuls les 4 premiers BITS de poids faibles sont utilis�s. Ces 4 bits indique le contexte dans lequel l'action
 *				effectuera sont promote. Si le contexte n'est pas compatible, aucune promotion n'est effectu�e et le code d'erreur INCOMPATIBLE CONTEXT est retourn�.
 *	@param		pdtspecs est un pointeur sur les specs de la DriveTrain ( l'entraxe est n�c�ssaire pour le calcul des actions impliquant une rotation ).
 *	@param		psmp_array est un pointeur sur NARRAY ou sont plac�s les motion profile de rotation precomput�s.
 *
 * @return		PROMOTEACTION_RESULTS une valeur 32 bits o� sont compact�es les informations suivantes:
 *							BIT_0	� BIT_15		16 flags de configuration
 *							BIT_16	� BIT_31		index du premier kin g�n�r� par la promotion dan sl'array pkin_out.
 *
 *				� propos des 16 flags de configuration:
 *							BIT_0		|	TRUE: 1 ou plusieurs KIN ont �t� g�n�r�s lors de la promotion		FALSE: l'action n'a g�n�r� aucun KIN
 *
 *							BIT_1		|_	copie des BIT_1 et BIT_2 de la valeur 'flags' membre de NLTRJACTION. ( soit FLAG_NLTRJACTION_SET_DT_MODE et FLAG_NLTRJACTION_SET_DT_MODE )
 *							BIT_2		|	...le nombre sur 2 bits	[BIT_1|BIT_2] peut prendre prendre 3 valeurs utiles d�crivant si l'action d�fini (SET) le DT Mode (ou pas) et comment elle le d�fini.
 *
 *											BIT_1 |	BIT_2
 *											------+-----------------------------------------------------------
 *											FALSE	FALSE	L'action ne (re)d�fini pas le DT Mode					=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = 0
 *											FALSE	TRUE	L'action (re)d�fini pas le DT Mode en Mode FORWARD		=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = FLAG_NLTRJACTION_SET_DT_MODE
 *											TRUE	TRUE	L'action (re)d�fini pas le DT Mode en Mode REVERSE		=>	returnvalue & MASK_NLTRJACTION_SET_DT_MODE = FLAG_NLTRJACTION_SET_DT_MODE|FLAG_NLTRJACTION_SET_DT_MODE
 *
 *											On remarquera que BIT_2 est syst�matiquement TRUE en cas de (re)d�finition du DT Mode et syst�matiquement OFF dans le cas contraire.
 *
 */
// ------------------------------------------------------------------------------------------
NLTRJPOINT_DESC_CFG promoteAction(const NLTRJPOINT_DESC_CFG context_cfg, const NLTRJACTION *paction, const Nf32 t, NARRAY *pkin_out)
{
	NErrorIf((context_cfg & MASK_NLTRJPOINT_DESC_CONFIG) != context_cfg, NERROR_WRONG_VALUE); // Context_cfg doit uniquement d�finir DT Mode et Ktype ( BIT_0 to BIT_3 ). Rien de plus !
	NErrorIf(!GET_NLTRJPOINT_DESC_DT_MODE(context_cfg), NERROR_WRONG_VALUE);				  // Context_cfg doit �tre compl�tement d�fini ! ( pas �gal � zero ! )
	NErrorIf(!GET_NLTRJPOINT_DESC_KTYPE(context_cfg), NERROR_WRONG_VALUE);					  // Context_cfg doit �tre compl�tement d�fini ! ( pas �gal � zero ! )

	// Extract NLPROMOTEACTION_CFG Data:
	//	Nu16 kincount = GET_NLPROMOTEACTION_CFG_KEY_KIN_COUNT(context_cfg);
	//	Nu16 cfg	  = GET_NLPROMOTEACTION_CFG_CONFIG(context_cfg);

	if (paction->m_core.m_id == NLTRJACTION_ID_NULL)
		return MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, context_cfg);

	if (!IS_NLTRJACTION_ENABLE(paction->m_core.m_flags))
		return MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, context_cfg);

	//	* **********************************************************************************************************************************************************
	//	*
	//	* a)	D�finition de la configuration du contexte requis pas l'action:
	//	*
	Nu32 action_cfg = 0;

	//	*		Si l'action ne sp�cifie pas de DT Mode, on d�fini le DT mode de la configuration de l'Action, comme �tant �gal au DT Mode courant( celui du context_config ).
	//	*		Sinon on r�cup�re le DT Mode sp�cifi� dans les flags de l'action.
	if (GET_NLTRJPOINT_DESC_DT_MODE(paction->m_core.m_flags) == 0)
		action_cfg |= GET_NLTRJPOINT_DESC_DT_MODE(context_cfg);
	else
		action_cfg |= GET_NLTRJPOINT_DESC_DT_MODE(paction->m_core.m_flags);

	//	*		Si l'action ne sp�cifie pas de KType, on d�fini le KType de la configuration de l'Action, comme �tant �gal au KType courant( celui du context_config ).
	//	*		Sinon on r�cup�re le KType sp�cifi� dans les flags de l'action.
	if (GET_NLTRJPOINT_DESC_KTYPE(paction->m_core.m_flags) == 0)
		action_cfg |= GET_NLTRJPOINT_DESC_KTYPE(context_cfg);
	else
		action_cfg |= GET_NLTRJPOINT_DESC_KTYPE(paction->m_core.m_flags);

	//	* **********************************************************************************************************************************************************
	//	*
	//	* b)	V�rification de la configuration requise avec la configuration courante:
	//	*
	//	*		Si la configuration recquise est diff�rente de la configuration courante, l'action renvoie imm�diatement une demande de mise � jour de la configuration
	//	*		En retournant le code de retour adapt� assosci� � la configuration attendue.
	/*
	if(action_cfg != context_cfg)
		return MAKE_NLPROMOTE_KEY_ACTION_RESULTS(	0,				// |_Action data:	Non utilis�s ici ( cntn = FALSE )
													FALSE,			// |
													TRUE,			// Demande d'update de la config !
													0,				// |_Kin data:		Non utilis�s ici ( iskin = FALSE )
													FALSE,			// |
													action_cfg);	// la config attendue !
													*/
	//	*		Sinon, c'est � dire dans le cas o� la configuration du context actuel est conforme aux attentes de l'action : On continue !

	//	* **********************************************************************************************************************************************************
	//	*
	//	* c)	Promotion :
	//	*
	//	*		Toutes les actions n'effectuent pas de promotion de Kins. La suite se traite donc selon l'id de l'action ...
	//	*

	NLKIN *pkin;
	NLKIN kin;
	Nu32 size = 0;
	Nf32 timeshift = 0.0f;
	Nf32 timelength = 0.0f;

	if (paction->m_core.m_id == NLTRJACTION_ID_ROTATE)
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
				Nu32 kidx = pkin_out->Size;
				NLKIN kin;
				kin.null();
				kin.m_t = t;
				NEnsureArrayCapacity(pkin_out, pkin_out->Size + size + 1);
				pkin = (NLKIN *)NArrayPushBack(pkin_out, (NBYTE *)&kin);
				size = paction->m_rotate.m_sMotionProfile.ppromote(&((NLTRJACTION *)paction)->m_rotate.m_sMotionProfile, pkin);
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

	return MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, action_cfg);
}

/*
#define FLAG_NLTRJACTION_ID_NEED_KEY_STOP									BIT_0	//

#define FLAG_NLTRJACTION_ID_CORE_MODE_REVERSE								BIT_1
#define FLAG_NLTRJACTION_ID_SET_CORE_MODE									BIT_2

#define FLAG_NLTRJACTION_ID_LOCK_DRIVETRAIN									BIT_3	// Une Action dont ce flag est ON prend le contr�le de la base. La dur�e de cette prise de contr�le peut-�tre nulle ( instantan�e )
#define FLAG_NLTRJACTION_ID_NON_NULL_LOCK_TIMESPAN							BIT_4	// Une Action dont ce flag est ON bloque la DT sur une certaine dur�e.
*/

NLTRJACTION *setupAction(NLTRJACTION *paction, const Nu16 actionid, const Nbool bfullreset)
{
	if (bfullreset)
	{
		//	Nmem0(paction, NLTRJACTION);
		paction->m_core.m_timeShift = 0.0f;
		paction->m_core.m_timeStamp = 0.0f;
	}

	// Core
	paction->m_core.m_id = actionid;

	switch (actionid)
	{
	case NLTRJACTION_ID_NULL:
		paction->m_core.m_flags = 0; // CFG: This action doesn't set DT Mode.
									 // CFG: This action doesn't set KType because it doesn't create any Kin.
		break;

	case NLTRJACTION_ID_FORWARD_MODE:
		paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;		// This action needs the Robot be stopped to performed.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND;			// This action is a Command. that means, it takes DT control. ( here is instantanneous, just to update DT Mode )
		paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART; // By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_SET_DT_MODE;		// CFG: This action set the Drive Train Mode ( to Forward ).
																		// CFG: This action doesn't set KType because it doesn't create any Kin.
		break;

	case NLTRJACTION_ID_REVERSE_MODE:
		paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									// This action needs the Robot be stopped to performed.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND;										// This action is a Command. that means, it takes DT control. ( here is instantanneous, just to update DT Mode )
		paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								// By default. This action is supposed to be integrated into the Key time span.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_SET_DT_MODE | FLAG_NLTRJACTION_DT_MODE_REVERSE; // CFG:	This action set the Drive Train Mode( to Reverse ).
																									// CFG: This action doesn't set KType because it doesn't create any Kin.
		break;

	case NLTRJACTION_ID_ROTATE:
		paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									 // This action needs the Robot be stopped to performed.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND | FLAG_NLTRJACTION_COMMAND_LONG_RUNNING; // This action is a Command. that means, it takes DT control. ( here is long running, during all the period covered by the kins )
		paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								 // By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_KTYPE_SPOT;										 // CFG: Kin created by this action must be interpreted with the KType Spot.
																									 // CFG: This action doesn't set DT Mode.
		if (bfullreset)
		{
			// r�initialisation des valeurs !
			paction->m_rotate.m_angle = 0.0f;
			paction->m_rotate.m_speedRatio = 1.0f;
			paction->m_rotate.m_sMotionProfile.initialize();
		}
		else
		{
			// On ne r�initialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
			// ( ne pas oublier que action est une union !)
			paction->m_rotate.m_angle = NCLAMP(-360.0f, paction->m_rotate.m_angle, 360.0f);
			paction->m_rotate.m_speedRatio = NCLAMP(0.0f, paction->m_rotate.m_speedRatio, 1.0f);
		}
		break;

	case NLTRJACTION_ID_BACK_AND_FORTH:
		paction->m_core.m_flags = FLAG_NLTRJACTION_NEED_KEY_STOP;									 // This action needs the Robot be stopped to performed.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_COMMAND | FLAG_NLTRJACTION_COMMAND_LONG_RUNNING; // This action is a Command. that means, it takes DT control. ( here is long running, during all the period covered by the kins )
		paction->m_core.m_flags |= FLAG_NLTRJACTION_WAITED_FOR_RESTART;								 // By default. This action is supposed to be integrated into the Key time span when this one is auto-evaluated.
		paction->m_core.m_flags |= FLAG_NLTRJACTION_KTYPE_SPOT | FLAG_NLTRJACTION_KTYPE_TRAVELING;	 // CFG: Kin created by this action must be interpreted with the KType SpotExtended.
																									 // CFG: This action doesn't set DT Mode.

		if (bfullreset)
		{
			paction->m_backAndForth.m_distance = 1.0f;
			paction->m_backAndForth.m_speedRatio = 1.0f;
			paction->m_backAndForth.m_pause = 0.0f;
		}
		else
		{
			// On ne r�initialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
			// ( ne pas oublier que action est une union !)
			paction->m_backAndForth.m_distance = NCLAMP(0.0f, paction->m_backAndForth.m_distance, 16.0f); // 16 m (distance arbitraire egal grosso modo � la longueur du terrain )
			paction->m_backAndForth.m_speedRatio = NCLAMP(0.0f, paction->m_backAndForth.m_speedRatio, 1.0f);
			paction->m_backAndForth.m_pause = NCLAMP(0.0f, paction->m_backAndForth.m_pause, 60.0f);
		}
		break;

	case NLTRJACTION_ID_SEND_MESSAGE:
		paction->m_core.m_flags = 0; // CFG: This action doesn't set DT Mode.
									 // CFG: This action doesn't set KType because it doesn't create any Kin.
		if (bfullreset)
		{
			paction->m_sendMessage.m_message = 0;
		}
		else
		{
			// On ne r�initialise pas les valeurs mais on s'assure qu'elles restent valides et acceptables.
			// ( ne pas oublier que action est une union !)
			paction->m_sendMessage.m_message = NCLAMP(0, paction->m_sendMessage.m_message, (NLTRJACTION_NUMBER_OF_ID - 1));
		}

		break;

	default:
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		break;
	}
	return paction;
}

NLTRJACTION *createAction(const Nu16 actionid)
{
	return setupAction((NLTRJACTION *)Nmalloc(sizeof(NLTRJACTION)), actionid, NTRUE);
}

void clearAction(NLTRJACTION *paction)
{
	Nmem0(paction, NLTRJACTION);
}

void deleteAction(NLTRJACTION *paction)
{
	clearAction(paction);
	Nfree(paction);
}

Nf32 getActionCommandTime(const NLTRJACTION *paction)
{
	if (paction->m_core.m_id == NLTRJACTION_ID_ROTATE)
	{
		NErrorIf(!IS_NLTRJACTION_COMMAND_LONG_RUNNING(paction->m_core.m_flags), NERROR_INCONSISTENT_FLAGS);
		Nf32 f = paction->m_rotate.m_sMotionProfile.getEffectiveTimeSpan();
		return (f < NF32_MAX) ? f : 0.0f; // f  = NF32_MAX signifie que le motion profile � �t� initialis� mais pas calcul�.
										  // En d'autres termes il est "vide" / "null".
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(IS_NLTRJACTION_COMMAND_LONG_RUNNING(paction->m_core.m_flags), NERROR_INCONSISTENT_FLAGS);
		// back and forth peut faire �a ... il faudrait l'impl�menter !!!
	}
#endif
	return 0.0f;
}
