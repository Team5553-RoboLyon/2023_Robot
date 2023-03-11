#include "../../../../resource.h"
#include "../../App_Init.h"
#include "../../../N/Core/NSafeConversion.h"
#include "../../../NL/MotionControl/NLPathWorkbench.h"
#include "../../Windows/WindowUtilities.h"
#include "../../../NL/NLProject.h"
#include "../../../NL/MotionControl/Trajectory/NLTrajectoryActionMessage.h"
#include "WndProcTrajectoryKeyAction.h"


// Fonctions d'Update de la Table d'Actions			Call back							
// 
// updateActionsTable_afterActionErase				afterActionErase_Callback			setKeyRestartPosition
// updateActionsTable_afterActionInsertion			afterActionInsertion_Callback		setKeyRestartPosition
// updateActionsTable_afterActionIndexShifting		afterActionIndexShifting_Callback	setKeyRestartPosition
// updateActionsTable_afterKeyIndexUpdate			updateAction_Callback				setKeyRestartPosition
// updateActionsTable_afterKeyDurationUpdate		updateActionPosition_Callback		setKeyRestartPosition



extern NLPROJECT						Project;
extern HINSTANCE						hInstance;
extern void setKeyRestartCtrlsPos(HWND hwnd, const Ns32 y);


int getKeyRestartRelativeYShift(const NLTRJKEY* pkey, const NLTRJACTION* paction)
{
	Nf32 dt = pkey->m_timeStamp.end - pkey->m_timeStamp.start;
	if (paction->m_core.m_timeShift > dt)
		return NL_TKP_RESTART_LINE_HEIGHT;
	else
		return 0;
}

void setKeyRestartPosition(HWND hwnd, const NLTRJKEY* pkey)
{
	Ns32 y = NL_ACTIONWINDOW_Y0;
	if (pkey->m_actionsArray.Size)
	{
		Nf32 dt = pkey->m_timeStamp.end - pkey->m_timeStamp.start;
		NLTRJACTION* pfirstact	= (NLTRJACTION*)pkey->m_actionsArray.pFirst;
		NLTRJACTION* plastact	= pfirstact + pkey->m_actionsArray.Size - 1;

		if (dt < pfirstact->m_core.m_timeShift)
			y = NL_ACTIONWINDOW_Y0;
		else if (dt >= plastact->m_core.m_timeShift)
			y = NL_ACTIONWINDOW_Y0 + NL_ACTIONWINDOW_YSHIFT * pkey->m_actionsArray.Size;
		else
		{
			NLTRJACTION* pact = plastact;
			while (dt < pact->m_core.m_timeShift) { pact--; }
			NErrorIf(pact < pfirstact, NERROR_INCONSISTENT_VALUES);
			y = NL_ACTIONWINDOW_Y0 + NL_ACTIONWINDOW_YSHIFT * ( (pact - pfirstact) + 1 );
		}
	}
	setKeyRestartCtrlsPos(hwnd, y);
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Recupère l'index de l'action attachée à une fenêtre action
 *
 *	@param	hwnd est un handle de fenêtre (qui doit absolument être de type ActionWinClass )
 *	@return	l'index de l'action attachée.
 *			NVOID si hwnd est NULL	
 *	 		valeur indéterminée si la classe de fenêtre de hwnd n'est pas ActionWinClass
 * 			
 */
 // ------------------------------------------------------------------------------------------
Nu32 getHwndActionIndex(HWND hwnd)
{
	if (hwnd)
	{
#ifdef _DEBUG
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		NErrorIf(strcmp(classname, "ActionWinClass"), NERROR_INCONSISTENT_REQUEST);
#endif
		WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
		NErrorIf(pcreation_data == NULL, NERROR_NULL_POINTER);
		return pcreation_data->m_actionIndex;
	}
	else
		return NVOID;
}
/*
void displayActionTimes(NLTRJACTION* paction, HWND hwnd)
{
	Nchar txt[64];

	sprintf(txt, "%.2f ", paction->m_core.m_timeShift);
	SetWindowText(GetDlgItem(hwnd, EDIT_ACTION_TIMESHIFT), txt);

	sprintf(txt, "= %.2f sec ", paction->m_core.m_timeStamp);
	SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIME_STAMP), txt);

}

void displayActionValues(NLTRJACTION* paction, HWND hwnd)
{
	// a)	Application des valeurs et Activation des contrôles en fonction du type de l'action:
	int enable_rotate_ctrls = SW_HIDE;
	int enable_sendmessage_ctrls = SW_HIDE;

	Nchar txt[64];

	if (paction)
	{
		displayActionTimes(paction, hwnd);

		ComboBox_SetCurSel(GetDlgItem(hwnd, COMBO_ACTIONS_ID), paction->m_core.m_id);
		SetFocus(hwnd);
		
	//	EnableWindow(GetDlgItem(hwnd, AUTO_KEY_TIME_ID), TRUE);

		switch (paction->m_core.m_id)
		{
		case NLTRJACTION_ID_NULL:
			// Aucun controle specifique
			break;

		case NLTRJACTION_ID_FORWARD_MODE:
			// Aucun controle specifique
			break;

		case NLTRJACTION_ID_REVERSE_MODE:
			// Aucun controle specifique
			break;

		case NLTRJACTION_ID_ROTATE:
			enable_rotate_ctrls = SW_SHOW;

			sprintf(txt, "%.2f", paction->m_rotate.m_angle);
			SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_DEG), txt);

			sprintf(txt, "%.2f", paction->m_rotate.m_speedRatio * 100.0f);
			SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO), txt);
			break;

		case NLTRJACTION_ID_SEND_MESSAGE:
			enable_sendmessage_ctrls = SW_SHOW;

			ComboBox_SetCurSel(GetDlgItem(hwnd, COMBO_MESSAGES_ID), paction->m_sendMessage.m_message);
			SetFocus(hwnd);

			break;
		}
	}

	ShowWindow(GetDlgItem(hwnd, EDIT_ROTATION_DEG), enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, STATIC_ROTATION_DEG_UNIT), enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO), enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, STATIC_ROTATION_VELOCITY_RATIO_UNIT), enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, COMBO_MESSAGES_ID), enable_sendmessage_ctrls);


	// On demande à entièrement retracer la fenêtre, background compris	
	//InvalidateRect(hwnd, NULL, TRUE);
}
*/

#define FLAG_UDAV_TIMES				BIT_0
#define FLAG_UDAV_ACTIONID			BIT_1
#define FLAG_UDAV_SPECIFIC_CTRLS	BIT_2
#define FLAG_UDAV_STATIC_CTRLS		BIT_3

#define FLAGS_UDAV_ALL				(BIT_0|BIT_1|BIT_2|BIT_3)

void updateDisplayedActionValues(const NLTRJKEY* pkey, const NLTRJACTION* paction, HWND hwnd, const Nu32 flags)
{
	Nchar txt[64];
	if (paction && pkey)
	{
		if (ISFLAG_ON(flags, FLAG_UDAV_TIMES))
		{
			sprintf(txt, "%.2f ", paction->m_core.m_timeShift);
			SetWindowText(GetDlgItem(hwnd, EDIT_ACTION_TIMESHIFT), txt);

			sprintf(txt, "= %.2f sec ", paction->m_core.m_timeStamp);
			SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIME_STAMP), txt);

			sprintf(txt, "dt = %.2f sec ", getActionCommandTime(paction));
			SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIME_DT), txt);

			if (ISFLAG_ON(flags, FLAG_UDAV_STATIC_CTRLS))
			{
				SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_PLUS), "+");
				SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIMESHIFT_UNIT), "sec");
			}
		}

		if (ISFLAG_ON(flags, FLAG_UDAV_ACTIONID))
		{
			ComboBox_SetCurSel(GetDlgItem(hwnd, COMBO_ACTIONS_ID), paction->m_core.m_id);

			if (ISFLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_WAITED_FOR_RESTART))
				CheckDlgButton(hwnd, AUTO_KEY_TIME_ID, BST_CHECKED);
			else
				CheckDlgButton(hwnd, AUTO_KEY_TIME_ID, BST_UNCHECKED);

			if (IS_NLTRJKEY_AUTODURATION(pkey->m_flags))
				EnableWindow(GetDlgItem(hwnd, AUTO_KEY_TIME_ID), TRUE);
			else
				EnableWindow(GetDlgItem(hwnd, AUTO_KEY_TIME_ID), FALSE);
		}

		if (ISFLAG_ON(flags, FLAG_UDAV_SPECIFIC_CTRLS))
		{
			switch (paction->m_core.m_id)
			{
			case NLTRJACTION_ID_NULL:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_FORWARD_MODE:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_REVERSE_MODE:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_ROTATE:
				sprintf(txt, "%.2f", paction->m_rotate.m_angle);
				SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_DEG), txt);

				sprintf(txt, "%.2f", paction->m_rotate.m_speedRatio * 100.0f);
				SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO), txt);

				if (ISFLAG_ON(flags, FLAG_UDAV_STATIC_CTRLS))
				{
					SetWindowText(GetDlgItem(hwnd, STATIC_ROTATION_DEG_UNIT), "deg");
					SetWindowText(GetDlgItem(hwnd, STATIC_ROTATION_VELOCITY_RATIO_UNIT), "% of Max Velocity");
				}
				break;

			case NLTRJACTION_ID_SEND_MESSAGE:
				ComboBox_SetCurSel(GetDlgItem(hwnd, COMBO_MESSAGES_ID), paction->m_sendMessage.m_message);
				break;
			}
		}
	}
}


void showActionSpecificCtrls(NLTRJACTION* paction, HWND hwnd)
{
	// a)	Application des valeurs et Activation des contrôles en fonction du type de l'action:
	int enable_rotate_ctrls		 = SW_HIDE;
	int enable_sendmessage_ctrls = SW_HIDE;

	if (paction)
	{
		switch (paction->m_core.m_id)
		{
			case NLTRJACTION_ID_NULL:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_FORWARD_MODE:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_REVERSE_MODE:
				// Aucun controle specifique
				break;

			case NLTRJACTION_ID_ROTATE:
				enable_rotate_ctrls = SW_SHOW;
				break;

			case NLTRJACTION_ID_SEND_MESSAGE:
				enable_sendmessage_ctrls = SW_SHOW;
				break;
		}
	}
	ShowWindow(GetDlgItem(hwnd, EDIT_ROTATION_DEG),						enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, STATIC_ROTATION_DEG_UNIT),				enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO),			enable_rotate_ctrls);
	ShowWindow(GetDlgItem(hwnd, STATIC_ROTATION_VELOCITY_RATIO_UNIT),	enable_rotate_ctrls);

	ShowWindow(GetDlgItem(hwnd, COMBO_MESSAGES_ID),						enable_sendmessage_ctrls);
}



BOOL CALLBACK redrawActionWindows_Callback(HWND hwnd, LPARAM lParam)
{
	Nchar classname[32];
	GetClassName(hwnd, classname, 32);
	if (!strcmp(classname, "ActionWinClass"))
	{
		WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
		showActionSpecificCtrls(pcreation_data->getActionPtr(), hwnd);
		updateDisplayedActionValues(pcreation_data->getKeyPtr(), pcreation_data->getActionPtr(), hwnd, FLAGS_UDAV_ALL);
	}
	return TRUE;
}
BOOL CALLBACK enableActionAutoCheckBox_Callback(HWND hwnd, LPARAM lParam)
{
	Nchar classname[32];
	GetClassName(hwnd, classname, 32);
	if (!strcmp(classname, "ActionWinClass"))
	{
		EnableWindow(GetDlgItem(hwnd, AUTO_KEY_TIME_ID), (bool)lParam);
	}
	return TRUE;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Fct callback appelée par la fct "EnumChildWindows"
 *			Cette fonction callback est appelée pour chaque fenêtre enfant ( et n'intervient concretement que si cette fenêtre est une fenêtre action ).
 * 			Elle a pour but d'associer une action à cette fenêtre action, déjà créée.
 *			Elle détruit automatiquement la fenêtre si l'index de l'action passé en paramètre [ '((ECW_ACTION_DATA*)lParam).m_actionIdx0' ] est 
 *			En dehors des limites de l'array des actions passé lui aussi en paramètre.
 * 
 *	@param	hwnd est un handle de fenêtre (une des fenêtres enfants de la fenêtre mère passée en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interprétés comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action à partir de laquelle il faut se mettre à jour.
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	non utilisé.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions où l'action d'index 'm_actionIdx0' est stockée.
 */
 // ------------------------------------------------------------------------------------------
BOOL CALLBACK  updateAction_Callback(HWND   hwnd, LPARAM lParam)
{
	if (lParam)
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if (!strcmp(classname, "ActionWinClass"))
		{
			ECW_ACTION_DATA* pparams = (ECW_ACTION_DATA*)lParam;
			NLTRJKEY* pkey = pparams->getKeyPtr();
			
			NErrorIf(!pkey, NERROR_NULL_POINTER);

			if (pparams->m_actionIdx0 < pkey->m_actionsArray.Size)
			{
				WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
				pcreation_data->m_pathWorkBench	= pparams->m_pathWorkBench;
				pcreation_data->m_keyIndex		= pparams->m_keyIndex;
				pcreation_data->m_actionIndex	= pparams->m_actionIdx0;

				NLTRJACTION* pact = pcreation_data->getActionPtr();
				showActionSpecificCtrls(pact, hwnd);
				updateDisplayedActionValues(pkey,pact, hwnd, FLAGS_UDAV_ALL);
				SetWindowPos(hwnd, NULL, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pkey, pact) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);

				pparams->m_actionIdx0 += 1;
			}
			else
			{
				DestroyWindow(hwnd);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Fct callback appelée par la fct "EnumChildWindows"
 *			La ligne "KEYRESTART" vient s'insérer "parmis" la liste des fenêtres Action, en fonction de la valeur durée, fixant le timestamp de redémarrage.
 *			Tel que les choses sont faites, il est possible pour une clef de définir une date de redémarrage avant que toutes ces actions soient réalisées.
 *			Bien sûr, cela peut engendrer des incohérences ( certaines actiones nécéssitant d'être à l'arrêt, au sens translation le long du chemin, comme l'action de rotation )
 *			qui sont prise en charge par le fonction NLTRAJECTORY::build qui disable tout simplement ces actions.
 *			Le rôle de cette fonction est de gérer la position des fenêtres action en fonction de la ligne "KEYRESTART" dont la position relative évolue car 
 *			la durée de la KEY est modifiée.
 *			Ainsi les fenêtres action vont se repositionner pour laisser une nouvelle place à la ligne keyrestart qui sera déplacée juste après 
 *			( en dehors de cette fonction callback bien evidement)	
 * 
 *	@param	hwnd est un handle de fenêtre (une des fenêtres enfants de la fenêtre mère passée en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interprétés comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action à partir de laquelle il faut se mettre à jour.
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	non utilisé.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions où l'action d'index 'm_actionIdx0' est stockée.
 */
 // ------------------------------------------------------------------------------------------
BOOL CALLBACK  updateActionPosition_Callback(HWND   hwnd, LPARAM lParam)
{
	if (lParam)
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if (!strcmp(classname, "ActionWinClass"))
		{
			ECW_ACTION_DATA* pparams = (ECW_ACTION_DATA*)lParam;
			WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);

			NLTRJKEY* pkey = pparams->getKeyPtr();
			NLTRJACTION* pact = pcreation_data->getActionPtr();
			NErrorIf(!pkey, NERROR_NULL_POINTER);
			NErrorIf(!pact, NERROR_NULL_POINTER);
			updateDisplayedActionValues(pkey,pact, hwnd, FLAGS_UDAV_ALL);
			SetWindowPos(hwnd, NULL, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pkey, pact) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Fct callback appelée par la fct "EnumChildWindows"
 * 			Les actions sont classées selon l'ordre croissant de leur timeShift. ( de 0 à xx sec ). Ainsi la premiere action de l'array ( index0 )
 *			a le timeshift le plus petit, et la dernière le timeshift le plus grand.
 * 			Lors de la modification du TimeShift d'une action, (edition directe, moveUp ou moveDown) les actions sont réordonnées dans l'array pour préserver cet ordre croissant. 
 *			Il est nécéssaire de faire la même chose avec la liste des fenêtres actions affichées et de les ordonner de haut en bas. C'est le rôle de cette fonction.
 * 			Cette fonction "déplace verticalement" les fenêtres Action, SANS EN MODIFIER LE CONTENU !
 * 			à l'exeption de la fenêtre action qui a été effectivement déplacée ( car le time shift de son action modifié)
 *			et pour qui les contrôles d'édition affichant "timeshift et timestamp" sont remis à jour. ( appel à la fonction 'SetWindowText' pour ces 2 contrôles )
 *			En plus de replacer les fenêtres action de haut en bas dans l'ordre croissant de leur timeshift, cette fonction met à jour les index des actions référencés
 *			par chaque fenêtre action.
 *			En effet, les fenêtres actionreférencent toujours les index des actions tels qu'ils étaient avant le déplacement de l'action.
 *			Ce déplacement dans l'array de l'action au timeshit edité,  peut se voir comme une suppression de cette action suivit de sa réinsertion à une autre place. 	
 *			Cette fonction ré.évalue l'index de l'action referencée par la fenêtre action et le met à jour pour prendre en compte cette "suppression+insertion".		
 * 
 *	@param	hwnd est un handle de fenêtre (une des fenêtres enfants de la fenêtre mère passée en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interprétés comme suit:
 * 
 *					((ECW_ACTION_DATA*)lParam)->m_pathWorkBench pointeur vers le Workbench courant.
 * 					((ECW_ACTION_DATA*)lParam)->m_keyIndex		Index de la clef 'parente' des actions en cours de traitement
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action avant update ( avant déplacement ).
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	contient l'index de l'action après update ( après déplacement ).
 * 
 */
 // ------------------------------------------------------------------------------------------
BOOL CALLBACK  afterActionIndexShifting_Callback(HWND   hwnd, LPARAM lParam)
{
	// 
	if (lParam) 
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if (!strcmp(classname, "ActionWinClass"))
		{
			WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
			NErrorIf(pcreation_data == NULL, NERROR_NULL_POINTER);
			ECW_ACTION_DATA* pparams = (ECW_ACTION_DATA*)lParam;
			Ns32	ridx = 0;

			// L'action attachée à la fenêtre est celle qui a été déplacée !
			// Son déplacement correspond  à la différence entre sa position finale et sa position initiale.

			//Nbool bistheone = 0;
			if (pcreation_data->m_actionIndex == pparams->m_actionIdx0)
			{
				//bistheone = 1;
				ridx = pparams->m_actionIdx1 - pparams->m_actionIdx0;
			}
			else
			{
				// On avance pas à pas, en 2 temps:
				//	1/	REMOVE:		Si l'action attachée à cette fenêtre était située après la position initiale de l'action déplacée.
				//					Alors son index est diminué de 1.
				//  2/	REINSERT:	Si l'action attachée à cette fenêtre est située après la position finale de l'action déplacée ( réinsérée ).
				//					Alors son index est augmenté de 1.
				//	La somme de ces 2 déplacements donne le déplacement total subit par l'action attaché à cette fenêtre lors du 
				//	Déplacement de l'action 'pparams->m_actionIdx0' à l'index 'pparams->m_actionIdx1'.
				ridx += (pcreation_data->m_actionIndex > pparams->m_actionIdx0) ? -1 : 0;
				ridx += ((pcreation_data->m_actionIndex + ridx) >= pparams->m_actionIdx1) ? 1 : 0;
			}

			/*
			// Un déplacement d'index se répercute sur un déplacement de la fenêtre vertical:
			if(ridx)
			{
				pcreation_data->m_actionIndex += ridx;

				// Vu qu'il y a déplacement il y a de grande chance que le timeshift aie été modifié
				if (bistheone)
					updateDisplayedActionValues(pcreation_data->getActionPtr(), hwnd,FLAG_UDAV_TIMES);//displayActionTimes(pcreation_data->getActionPtr(), hwnd);

				SetWindowPos(hwnd, HWND_TOP, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pparams->getKeyPtr(), pcreation_data->getActionPtr()) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOSIZE);
			}
			*/
			pcreation_data->m_actionIndex += ridx;

			NLTRJKEY* pkey		= pcreation_data->getKeyPtr();
			NLTRJACTION* pac	= pcreation_data->getActionPtr();
			updateDisplayedActionValues(pkey,pac, hwnd, FLAGS_UDAV_ALL);//displayActionTimes(pcreation_data->getActionPtr(), hwnd);
			SetWindowPos(hwnd, HWND_TOP, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pkey, pac) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOSIZE);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Fct callback appelée par la fct "EnumChildWindows" ...
 *			... Juste après la suppression d'une action de l'array des actions et de la fenetre action associée.			
 *			Les fenêtres action actuellement en place reférencent toujours les index des actions tels qu'ils étaient avant la suppression.
 *			( sauf la fenêtre action qui était associée à l'action supprimée et qui est normalement supprimée également )
 *			Cette fonction callback est appelée pour chacune des fenêtre enfant de la BDD keyProperties, et donc pour chacune des fenêtres actions.
 *			Ainsi, chaque fenêtre action referencant un index obsolete (> index de l'action supprimée) est mise à jour ( index -= 1 ) afin
 *			de continuer à référencer la même structure action ( elle aussi décalée de -1 après l'insertion)
 *			par mesure de précaution cette fonction callback vérifie aussi la presence d'une fenetre referencant un index identique à celui de l'action supprimée
 *			ce qui revient à dire que la fenêtre action associée, qui aurait du être supprimée, ne l'a pas été ! ... Si cela arrive l'index référencé par cette fenêtre 
 *			prend la valeur de NVOID !!!! ( et CRASH en debug )
 * 
 *	@param	hwnd est un handle de fenêtre (une des fenêtres enfants de la fenêtre mère passée en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interprétés comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index ( évalué avant suppression ) de l'action supprimée.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions où l'action supprimée était stockée.
 */
 // ------------------------------------------------------------------------------------------
BOOL CALLBACK  afterActionErase_Callback(HWND   hwnd, LPARAM lParam)
{
	// 
	if (lParam)
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if (!strcmp(classname, "ActionWinClass"))
		{

			ECW_ACTION_DATA* pparams = (ECW_ACTION_DATA*)lParam;
			WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
			NErrorIf(pcreation_data == NULL, NERROR_NULL_POINTER);

			if (pcreation_data->m_actionIndex > pparams->m_actionIdx0)
			{
				pcreation_data->m_actionIndex -= 1;
				updateDisplayedActionValues(pcreation_data->getKeyPtr(),pcreation_data->getActionPtr(), hwnd, FLAGS_UDAV_ALL);
				SetWindowPos(hwnd, NULL, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pparams->getKeyPtr(), pcreation_data->getActionPtr()) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			else if (pcreation_data->m_actionIndex == pparams->m_actionIdx0) // la fenêtre va être supprimée, mais pas ici.
			{
				NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				pcreation_data->m_actionIndex = NVOID;
				//pcreation_data->m_pAction = NULL;
			}
			else //(pcreation_data->m_actionIndex < pparams->m_ActionIndex)
			{
				updateDisplayedActionValues(pcreation_data->getKeyPtr(),pcreation_data->getActionPtr(), hwnd, FLAGS_UDAV_ALL);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Fct callback appelée par la fct "EnumChildWindows" ...
 *			... Juste après l'insertion d'une action dans l'array des actions, et juste avant la création de la fenetre action associée.			
 *			Les fenêtres action actuellement en place reférencent toujours les index des actions tels qu'ils étaient avant l'insertion.
 *			Cette fonction callback est appelée pour chacune des fenêtre enfant de la BDD keyProperties, et donc pour chacune des fenêtres actions.
 *			Ainsi, chaque fenêtre action referencant un index obsolete (>= index de l'action insérée) est mise à jour ( index += 1 ) afin
 *			de continuer à référencer la même structure action ( elle aussi décalée de +1 après l'insertion)
 * 
 *	@param	hwnd est un handle de fenêtre (une des fenêtres enfants de la fenêtre mère passée en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interprétés comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index ( évalué après insertion ) de l'action insérée.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions où l'action insérée est stockée.
 */
 // ------------------------------------------------------------------------------------------
BOOL CALLBACK  afterActionInsertion_Callback(HWND   hwnd, LPARAM lParam)
{
	if (lParam)
	{
		Nchar classname[32];
		GetClassName(hwnd, classname, 32);
		if (!strcmp(classname, "ActionWinClass"))
		{
			WND_KEYACTION_CREATE_DATA* pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
			NErrorIf(pcreation_data == NULL, NERROR_NULL_POINTER);

			ECW_ACTION_DATA* pparams = (ECW_ACTION_DATA*)lParam;
			if (pcreation_data->m_actionIndex >= pparams->m_actionIdx0)
			{
				pcreation_data->m_actionIndex += 1;
				//pcreation_data->m_pAction = (NLTRJACTION*)NGetArrayPtr(pparams->m_pActionArray, pcreation_data->m_actionIndex);
				SetWindowPos(hwnd, NULL, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(pcreation_data->getKeyPtr(), pcreation_data->getActionPtr()) + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			else
			{
				//pcreation_data->m_pAction = (NLTRJACTION*)NGetArrayPtr(pparams->m_pActionArray, pcreation_data->m_actionIndex);
			}

			updateDisplayedActionValues(pcreation_data->getKeyPtr(), pcreation_data->getActionPtr(), hwnd, FLAGS_UDAV_ALL);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Après avoir supprimé une action de l'array des actions, il est nécéssaire de
 *			mettre à jour le tableau des fenêtres actions.
 *			Cette fonction doit être appelée après les suppressions de l'action et de la fenêtre Action associée.
 *			Le rôle de cette fonction est de mettre à jour les index des actions associées aux fenêtres actions en place
 *			( car la suppression de l'action a modifié certains index de l'array ) et de modifier la position de ces fenêtres actions existantes,
 *			plus particulièrement celles référencant des actions situées "après" l'action supprimée.
 *			Ainsi, après l'appel à cette fonction, les fenêtres action située "avant" (dont l'index n'a pas changé) la fenêtre action supprimée
 *			ne changent pas et restent à leur place alors que les fenêtres action situées "après" ( dont l'index a été décalé de -1) "remontent"
 *			d'un "étage". La place laissée vide par la fenêtre action supprimée est ainsi comblé.
 *
 *	@param	hwnd est le handle de la  fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyidx est l'index de la clef en cours d'édition
 *	@param	erasedactionidx est l'index (avant suppression) de l'action qui vient d'être supprimée.
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterActionErase(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx, const Nu32 erasedactionidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pwb;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= erasedactionidx;
	ecw_params.m_actionIdx1		= NVOID;				// non utilisé
	EnumChildWindows(hwnd, afterActionErase_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Après avoir ajouté/inséré une action dans l'array des actions, il est nécéssaire de
 *			mettre à jour le tableau des fenêtres actions et d'ajouter une fenêtre action.
 *			Cette fonction doit être appelée après la création de la nouvelle action dans l'array ET avant la création de la fenêtre Action qui sera associée
 *			à cette nouvelle Action. 
 *			Le rôle de cette fonction est de mettre à jour les index des actions associées aux fenêtres actions en place 
 *			( car l'insertion de la nouvelle action a modifié certains index de l'array ) 
 *			et de modifier la position de ces fenêtres actions existantes, plus particulièrement celles référencant des actions situées "après" l'action insérée. 
 *			Ainsi, après l'appel à cette fonction, la place de la future fenêtre action est prête, elle n'a plus qu'à être créée et
 *			mise en place, dans le "trou" laissé libre " entre" les fenêtres action située "avant" (dont l'index n'a pas changé)
 *			et les fenêtres action situées "après" ( dont l'index a été décalé de +1).
 *
 *	@param	hwnd est le handle de la  fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyidx est l'index de la clef en cours d'édition
 *	@param	newactionidx est l'index de l'action qui vient d'être créée et ajoutée à l'array des actions de la clef.
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterActionInsertion(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyidx, const Nu32 newactionidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pcurrent_workbench;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= newactionidx;
	ecw_params.m_actionIdx1		= NVOID;				// non utilisé
	EnumChildWindows(hwnd, afterActionInsertion_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Lors de l'édition du timeshift d'une action, celle ci peut-être déplacée dans l'array des actions.
 *			En effet les actions sont classées selon l'ordre croissant de leur timeShift. ( de 0 à xx sec ).
 *			Ainsi, lorsqu'une action se retrouve "déplacée" ( Edition du TimeShift, moveUp,moveDown) il est nécéssaire de répercuter
 *			ce "déplacement" sur la liste des fenêtres actions affichées. C'est le rôle de cette fonction.
 *			"updateActionsTable_afterActionIndexShifting" ne fait que déplacer les fenêtres Action, sans en modifier le contenu
 *			comme le fait la fonction 'updateActionsTable_afterKeyDurationUpdate', à l'exeption de la fenêtre action qui a été déplacée
 *			et pour qui le contenu "timeshift et timestamp" est mis à jour.
 *				
 *	@param	hwnd est le handle de la  fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyidx est l'index de la "nouvelle" clef en cours d'édition
 *  @param	idx0 est l'index de l'action avant shifting ( avant déplacement ).
 *  @param	idx1 est l'index de l'action après shifting ( après déplacement ).
 * 
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterActionIndexShifting(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx,  const Nu32 idx0, const Nu32 idx1)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pwb;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= idx0;
	ecw_params.m_actionIdx1		= idx1;	
	EnumChildWindows(hwnd, afterActionIndexShifting_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Lorsque l'utilisateur sélectionne une nouvelle Key pour en éditer le contenu, la
 *			fenêtre keyproperties se met complètement à jour.	
 *			Toutes les actions de la key précédente sont remplacées par les actions de la nouvelle key.
 *			Le rôle de cette fonction est de réaliser cette màj en détruisant et recréant un minimum de fenêtres action
 *			et en essayant de reutiliser les fen^tres action existantes en leur reaffectant un nouveau contenu à afficher/editer.
 *			Si besoin des feêtres actions sont détruites et d'autres recrées.
 *
 *	@param	hwnd est le handle de la  fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyidx est l'index de la "nouvelle" clef en cours d'édition
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterKeyIndexUpdate(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pwb;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= 0;
	ecw_params.m_actionIdx1		= NVOID;				// non utilisé
	EnumChildWindows(hwnd, updateAction_Callback, (LPARAM)&ecw_params);

	NLTRJKEY* pkey = ecw_params.getKeyPtr();
	for (Nu32 i = ecw_params.m_actionIdx0; i < pkey->m_actionsArray.Size; i++)
	{
		insertActionHwnd(hwnd, pwb, keyidx, i);
	}
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Replace les fenêtres actions les unes par rapport aux autres.
 *			L'appel à cette fonction ne doit se faire que si le nombre d'actions n'a pas changé.
 *			L'ordre des actions les unes par rapport aux autres reste identique.
 *			Cependant, la modification de la durée de la key, et donc la position de la ligne Keyrestart
 *			peut devoir changer et venir s'insérer entre 2 autres actions, ou complètement en bas de liste.
 *			En conséquence les positions des fenêtres action peut être modifié et doit être ré-évalué.
 *			Le contenu des fenêtres actions est mis à jour, pour prendre en compte une potentielle désactivation ( PRECHECK_DISABLE ).
 *			suite à l amodification de la durée de la key.
 * 
 *	@param	hwnd est le handle de la fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyidx est l'index de la clef en cours d'édition
 *
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterKeyDurationUpdate(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pcurrent_workbench;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= NVOID;				// non utilisé
	ecw_params.m_actionIdx1		= NVOID;				// non utilisé
	EnumChildWindows(hwnd, updateActionPosition_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Crée une nouvelle fenêtre Action associée à un index d'action et la place à la bonne 
 *			hauteur dans le tableau des fenêtres Actions. Attention cette fonction ne se charge pas 
 *			de déplacer les autres fenêtres action déjà en place.
 * 
 *	@param	hwnd est le handle de la fenêtre keyProperties qui possèdent toutes les fenêtres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'édition.
 *	@param	keyid est l'index de la clef en cours d'édition
 *	@param	insertedactionindex est l'index de l'action qui vient d'être ajoutée ( element de l'array des action de la clef keyid )
 * 
 *	@return	le handle de la fenêtre action qui vient d'être créée et positionnée.
 */
 // ------------------------------------------------------------------------------------------
HWND insertActionHwnd(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyidx, const Nu32 insertedactionIndex)
{
	WND_KEYACTION_CREATE_DATA	creation_data;
	creation_data.m_pathWorkBench	= pcurrent_workbench;
	creation_data.m_keyIndex		= keyidx;
	creation_data.m_actionIndex		= insertedactionIndex;

	HFONT font = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
	HWND hwndaction = CreateWindowEx(0, "ActionWinClass",
										(LPCTSTR)NULL,
										WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
										NL_ACTIONWINDOW_X0,
										NL_ACTIONWINDOW_Y0 + getKeyRestartRelativeYShift(creation_data.getKeyPtr(), creation_data.getActionPtr()) + insertedactionIndex * NL_ACTIONWINDOW_YSHIFT,
										NL_ACTIONWINDOW_WIDTH, NL_ACTIONWINDOW_HEIGHT,
										hwnd, NULL, NULL, (LPVOID)&creation_data);
	SendMessage(hwndaction, WM_SETFONT, (WPARAM)font, true);

	return hwndaction;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Crée les controls spécifiques aux actions rotate
 *			Chaque Action possède sa propre fenetre d'édition. Chaque fenetre d'édition d'action contient des controles, certains différents selon l'ID de l'action.
 *			Cette fonction créée uniquement les contrôles spécifique à une action de type rotation.
 *
 *	@param	x		abscisse du premier contrôle spécifique de rotation ( le + à gauche, soit le contrôle d'édition de l'angle de rotation ) 
 *	@param	y		ordonnée du premier contrôle spécifique de rotation ( le + à gauche, soit le contrôle d'édition de l'angle de rotation ) 
 *	@param	angle	valeur de l'angle de rotation à afficher par le controle Edit associé
 *	@param	ratio	valeur du ratio de vitesse de rotation à afficher par le controle Edit associé
 *	@param	hparent	handle de la fenêtre parent des contrôles qui vont être créés (i-e handle de la fenêtre action)
 *  @param	hinst	hinstance du module 
 *
 *
 */
 // ------------------------------------------------------------------------------------------
void _create_action_rotate_ctrls(const int x, const int y, const Nf32 angle, const Nf32 ratio, HWND hparent, HINSTANCE hinst)
{
	//	(a) D'abord la partie commune : Main Time stamp + Time shift + Action type Combo box
	HWND hwnd = CreateWindow(	WC_EDIT, TEXT(""), WS_BORDER | WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE,
					x, y,
					48, 20,
					hparent, (HMENU)EDIT_ROTATION_DEG, hinst, NULL);
	SetWindowSubclass(hwnd, subEditProcR, 0, 0);


	CreateWindow(	WC_STATIC, TEXT("deg"), WS_CHILD | SS_LEFT | WS_VISIBLE,
					x+50, y+2,
					48, 20,
					hparent, (HMENU)STATIC_ROTATION_DEG_UNIT, hinst, NULL);

	//NErrorIf((ratio > 1.0f) || (ratio <= 0.0f), NERROR_INCONSISTENT_VALUES);
	hwnd = CreateWindow(	WC_EDIT, TEXT(""), WS_BORDER | WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | WS_VISIBLE,
					x+100, y,
					48, 20,
					hparent, (HMENU)EDIT_ROTATION_VELOCITY_RATIO, hinst, NULL);
	SetWindowSubclass(hwnd, subEditProcRp, 0, 0);

	CreateWindow(	WC_STATIC, TEXT("% of Max Velocity"), WS_CHILD | SS_LEFT | WS_VISIBLE,
					x+150, y+2,
					100, 20,
					hparent, (HMENU)STATIC_ROTATION_VELOCITY_RATIO_UNIT, hinst, NULL);
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Crée les controls spécifiques aux actions sendmessage
 *
 * ## Details
 * Chaque Action possède sa propre fenetre de controle. Chaque fenetre de controle contient différents controles selon l'ID de l'action.
 * Les premiers controles, relatifs au TimeStamp( et Time shift ) de chaque action sont créés pour chacune d'elle.
 *
 *	@param	pdata
 *	@param	hparent
 *  @param	hinst
 *
 *	@return	la position horizontale (int) à laquelle devrait être dessiné le contrôle suivant
 *
 */
 // ------------------------------------------------------------------------------------------
void _create_action_sendmessage_ctrls(const int x, const int y, const Nu16 currentmessid, HWND hparent, HINSTANCE hinst)
{
	HWND hwnd = CreateWindow(	WC_COMBOBOX,
								TEXT(""),
								CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD /* | WS_OVERLAPPED*/ | WS_VISIBLE,
								x, y, 240, 256,
								hparent,
								(HMENU)COMBO_MESSAGES_ID,
								hinst,
								NULL);

	NLACTIONMESSAGE* pm = (NLACTIONMESSAGE*)Project.m_actionMessagesLut.pFirst;
	for (Nu32 i = 0; i < Project.m_actionMessagesLut.Size; i++, pm++)
	{
		ComboBox_AddString(hwnd, pm->m_name);
	}
}




LRESULT CALLBACK WndProc_TrajectoryKeyAction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	NLPATH_WORKBENCH* pwb = (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
//	NLPATH_WORKBENCH* pwb_new = nullptr;
	NErrorIf(NLTRJACTION_NUMBER_OF_ID != 6, NERROR_SYSTEM_CHECK); // signifie qu'un changement de nbre d'ID de type d'action à eut lieu mais qu'il n'a pas été complètement

	Nchar		actionname[6][32] = {	NLTRJACTION_ID_NULL_STRING,
										NLTRJACTION_ID_FORWARD_MODE_STRING,
										NLTRJACTION_ID_REVERSE_MODE_STRING,
										NLTRJACTION_ID_ROTATE_STRING,
										NLTRJACTION_ID_BACK_AND_FORTH_STRING,
										NLTRJACTION_ID_SEND_MESSAGE_STRING };
	NLTRJACTION*		paction;
	NLTRJKEY*			pkey;
	Nu32				newidx;
	Nu32				sel;
	Nf32				f;

	WND_KEYACTION_CREATE_DATA	*pcreation_data = (WND_KEYACTION_CREATE_DATA*)GetWindowLong(hwnd, GWL_USERDATA);
	
	HWND	hwndctrl;
//	Nu32	x;
//	POINT	pt;
	HDC		hdc;
	RECT	rc;
	HBRUSH	hbr;

	switch (message)
	{

	case WM_ERASEBKGND:
		{
			if (hdc = (HDC)wParam)
			{
				hbr = GetSysColorBrush(COLOR_3DFACE); ;// (HBRUSH)GetStockObject(LTGRAY_BRUSH);
				GetClientRect(hwnd, &rc);
				FillRect(hdc, &rc, hbr);
				return TRUE;
			}
		}
		break;// return DefWindowProc(hwndDlg, message, wParam, lParam);

	case WM_PAINT:
		{
			HWND hwnd_current_action = NULL;
			// On demande à la fenetre parent de nous donner le handle de fenetre de "l'action courante"
			SendMessage(GetParent(hwnd), WM_MESSAGE_GET_CURRENT_ACTION_WINDOW, (WPARAM)&hwnd_current_action, 0);
			if (hwnd == hwnd_current_action)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				// All painting occurs here, between BeginPaint and EndPaint.
				hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
				GetClientRect(hwnd, &rc);
				FrameRect(hdc, &rc, hbr);
				//FrameRect(hdc, &ps.rcPaint, hbr);
				EndPaint(hwnd, &ps);

				return 0;
			}
		}
		break;

	case WM_CREATE:
		// 1/ Recup'et copie des données de creation ( allocation et stockage dans la fenetre )
		// -----------------------------------------------------------------------------------------
		pcreation_data = (WND_KEYACTION_CREATE_DATA*)Nmalloc(sizeof(WND_KEYACTION_CREATE_DATA));
		*pcreation_data = *(WND_KEYACTION_CREATE_DATA*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)pcreation_data);
		
		// 2/ Creation des fenêtres Action:
		// -----------------------------------------------------------------------------------------
		Nchar txt[32];

		//	(a) D'abord la partie commune : Main Time stamp + Time shift + Action type Combo box

		//	(a.0) Time shift
		CreateWindow(WC_STATIC,
			TEXT("+"),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			NL_ACTIONWINDOW_LEFT_MARGIN, 7, 14, 20,
			hwnd,
			(HMENU)STATIC_ACTION_PLUS,
			hInstance, NULL);

//		sprintf(txt, "%.2f ", pcreation_data->m_pAction->m_core.m_timeShift);
		hwndctrl = CreateWindow(WC_EDIT,
			TEXT("."),
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_RIGHT,
			NL_ACTIONWINDOW_LEFT_MARGIN + 14, 5, 48, 20,
			hwnd,
			(HMENU)EDIT_ACTION_TIMESHIFT,
			hInstance, NULL);
		SetWindowSubclass(hwndctrl, subEditProcRp, 0, 0);

		CreateWindow(WC_STATIC,
			TEXT("sec"),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			NL_ACTIONWINDOW_LEFT_MARGIN + 66, 7, 18, 20,
			hwnd,
			(HMENU)STATIC_ACTION_TIMESHIFT_UNIT,
			hInstance, NULL);

		//	(a.1) Main Time stamp / Duration (dt)
		// sprintf(txt, "= %.2f sec ", pcreation_data->m_pKey->m_timeStamp.start + pcreation_data->m_pAction->m_core.m_timeShift);
		CreateWindow(	WC_STATIC, 
						TEXT("."),
						WS_CHILD | WS_VISIBLE | SS_RIGHT,
						NL_ACTIONWINDOW_LEFT_MARGIN + 84, 7, 80, 20,
						hwnd,
						(HMENU)STATIC_ACTION_TIME_STAMP,
						hInstance, NULL);
	
		CreateWindow(	WC_STATIC,
						TEXT("."),
						WS_CHILD | WS_VISIBLE | SS_RIGHT,
						NL_ACTIONWINDOW_WIDTH - NL_ACTIONWINDOW_RIGHT_MARGIN - 80, 7, 80, 20,
						hwnd,
						(HMENU)STATIC_ACTION_TIME_DT,
						hInstance, NULL);


		//	(a.3) Check Box :	"Auto" c'est à dire, prise en compte du timing de l'action dan sle calcul de la durée globale de la clef
//						... avant restart.
		hwndctrl = CreateWindow(WC_BUTTON,
								TEXT(""),
								WS_VISIBLE | WS_CHILD | BS_LEFTTEXT | BS_RIGHT | BS_AUTOCHECKBOX,
								NL_ACTIONWINDOW_LEFT_MARGIN + 170,
								5,
								12,
								20,
								hwnd,
								(HMENU)AUTO_KEY_TIME_ID,
								hInstance,
								NULL);
	
		//	(a.2) Action type Combo box
		hwndctrl	 = CreateWindow(	WC_COMBOBOX,
									TEXT("Select an Action"),
									CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD /* | WS_OVERLAPPED*/ | WS_VISIBLE,
									NL_ACTIONWINDOW_LEFT_MARGIN + 190, 5, 150, 256,
									hwnd,
									(HMENU)COMBO_ACTIONS_ID,
									hInstance, 
									NULL);

		for (Nu32 i = 0; i < NLTRJACTION_NUMBER_OF_ID; i++)
		{
			ComboBox_AddString(hwndctrl, actionname[i]);
		}
		ComboBox_SetCurSel(hwndctrl,-1);

		//	(b) Puis les contrôles specifiques de chaque action
		paction = pcreation_data->getActionPtr();
		_create_action_rotate_ctrls(354, 5, paction->m_rotate.m_angle, paction->m_rotate.m_speedRatio, hwnd, hInstance);
		_create_action_sendmessage_ctrls(354, 5, 0, hwnd, hInstance);
		showActionSpecificCtrls(paction, hwnd);
		updateDisplayedActionValues(pcreation_data->getKeyPtr(),paction, hwnd, FLAGS_UDAV_ALL);
		return TRUE;

	case WM_SETFONT:
		EnumChildWindows(hwnd, SetFont, (LPARAM)wParam);
		return FALSE;

	case WM_DESTROY:
		if(pcreation_data)
			Nfree(pcreation_data);
		return FALSE;

	case WM_LBUTTONDOWN:
		{
			SendMessage(GetParent(hwnd), WM_MESSAGE_SET_CURRENT_ACTION_WINDOW, (WPARAM)hwnd, 0);
		}
		break;
/*
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{

		}
		return 0;
*/
	case WM_CTLCOLOREDIT:
	{
		HBRUSH hbr = (HBRUSH)DefWindowProc(hwnd,message,wParam,lParam);
		if(IS_NLTRJACTION_ENABLE(pcreation_data->getActionPtr()->m_core.m_flags))
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
		else
			SetTextColor((HDC)wParam, RGB(255, 0, 0));
		return (BOOL)hbr;
	}
	return FALSE;
	
	case WM_CTLCOLORSTATIC:
	{
		HBRUSH hbr = (HBRUSH)DefWindowProc(hwnd, message, wParam, lParam);
		if (IS_NLTRJACTION_ENABLE(pcreation_data->getActionPtr()->m_core.m_flags))
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
		else
			SetTextColor((HDC)wParam, RGB(255, 0, 0));
		return (BOOL)hbr;
	}
	return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			return TRUE;

		case EDIT_ACTION_TIMESHIFT:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS://EN_UPDATE:
				
				f = GetNf32FromEditControl(GetDlgItem(hwnd, EDIT_ACTION_TIMESHIFT), 0.0f, 3600.0f, NULL);
				paction = pcreation_data->getActionPtr();

				if (f != paction->m_core.m_timeShift)
				{
					// pré-évaluation de "paction->m_core.m_timeStamp"
					// paction->m_core.m_timeStamp est calculé par la méthode "NLTRAJECTORY::build" ( via un appel à la fonction "preComputeAction" )
					// Afin de donner et afficher une valeur immédiatement on procède ici à une évaluation.
					// En effet, on sait que : 
					//								paction->m_core.m_timeStamp = pkey->m_timeStamp.start + paction->m_core.m_timeShift;
					// Ici on ne modifie que m_timeShift ...
					// En conséquence on a :  paction->m_core.m_timeStamp = paction->m_core.m_timeStamp - previous_timeShift + new_timeShift
					sprintf(txt, "= %.2f sec ", paction->m_core.m_timeStamp - paction->m_core.m_timeShift + f);
					SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIME_STAMP), txt);

					pkey = pcreation_data->getKeyPtr();
					newidx = pkey->updatetActionTimeShift(pcreation_data->m_actionIndex, f);

					pkey->preCompute(pkey->m_timeStamp.start, pcreation_data->m_pathWorkBench->getDriveTrainSpecifications());

					if (newidx != pcreation_data->m_actionIndex)
					{
						updateActionsTable_afterActionIndexShifting(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex, pcreation_data->m_actionIndex, newidx );
					}
					else
					{
						// l'index de l'action n'a pas changé mais potentiellement, son timeshift ayant évolué, sa position relative par rapport
						// à la ligne Key restart à peut être changé !
						updateActionsTable_afterKeyDurationUpdate(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex);
					}

					UpdateWindow(GetParent(hwnd));
					pcreation_data->m_pathWorkBench->requestTrajectoryBuild();
				}
				break;

			default:
				break;
			}
			return TRUE;

		case EDIT_ROTATION_DEG:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS://EN_UPDATE:
				paction = pcreation_data->getActionPtr();
				NErrorIf(paction->m_core.m_id != NLTRJACTION_ID_ROTATE, NERROR_INCONSISTENT_VALUES);

				f = GetNf32FromEditControl(GetDlgItem(hwnd, EDIT_ROTATION_DEG), -3600, 3600.0f, NULL);
				if (f != paction->m_rotate.m_angle)
				{
					paction->m_rotate.m_angle = f;
					sprintf(txt, "%.2f", f);
					SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_DEG), txt);

					pkey = pcreation_data->getKeyPtr();
					pkey->preCompute(pkey->m_timeStamp.start, pcreation_data->m_pathWorkBench->getDriveTrainSpecifications());

					updateActionsTable_afterKeyDurationUpdate(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex);
					UpdateWindow(hwnd);

					pcreation_data->m_pathWorkBench->requestTrajectoryBuild();
				}
				break;

			default:
				break;
			}
			return TRUE;

		case EDIT_ROTATION_VELOCITY_RATIO:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS://EN_UPDATE:
				paction = pcreation_data->getActionPtr();
				NErrorIf(paction->m_core.m_id != NLTRJACTION_ID_ROTATE, NERROR_INCONSISTENT_VALUES);
				f = GetNf32FromEditControl(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO) , 0.0f, 100.0f, NULL)/100.0f;
				if (f != paction->m_rotate.m_speedRatio)
				{
					paction->m_rotate.m_speedRatio = f;
					sprintf(txt, "%.2f", f*100.0f);
					SetWindowText(GetDlgItem(hwnd, EDIT_ROTATION_VELOCITY_RATIO), txt);

					pkey = pcreation_data->getKeyPtr();
					pkey->preCompute(pkey->m_timeStamp.start, pcreation_data->m_pathWorkBench->getDriveTrainSpecifications());

					updateActionsTable_afterKeyDurationUpdate(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex);
					UpdateWindow(hwnd);

					pcreation_data->m_pathWorkBench->requestTrajectoryBuild();
				}
				break;

			default:
				break;
			}
			return TRUE;

		case COMBO_ACTIONS_ID:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				sel = SendMessage((HWND)lParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (sel != CB_ERR)
				{
					paction = pcreation_data->getActionPtr();
					setupAction(paction, _SafeNu32ToNu16(sel), FALSE);

					pkey = pcreation_data->getKeyPtr();
					pkey->preCompute(pkey->m_timeStamp.start, pcreation_data->m_pathWorkBench->getDriveTrainSpecifications());

					showActionSpecificCtrls(paction, hwnd);
					//updateDisplayedActionValues(pcreation_data->getKeyPtr(), paction, hwnd, FLAG_UDAV_SPECIFIC_CTRLS);

					updateActionsTable_afterKeyDurationUpdate(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex);

					UpdateWindow(hwnd);
					pcreation_data->m_pathWorkBench->requestTrajectoryBuild();
				}
			}
			return TRUE;

		case COMBO_MESSAGES_ID:
			paction = pcreation_data->getActionPtr();
			NErrorIf(paction->m_core.m_id != NLTRJACTION_ID_SEND_MESSAGE, NERROR_INCONSISTENT_VALUES);
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				sel = SendMessage((HWND)lParam, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (sel != CB_ERR)
				{
					paction->m_sendMessage.m_message = sel;
				}
			}
			return TRUE;

		case AUTO_KEY_TIME_ID:
			paction = pcreation_data->getActionPtr();
			if (IsDlgButtonChecked(hwnd, AUTO_KEY_TIME_ID))
			{
				FLAG_ON(paction->m_core.m_flags, FLAG_NLTRJACTION_WAITED_FOR_RESTART);
			}
			else
			{
				FLAG_OFF(paction->m_core.m_flags, FLAG_NLTRJACTION_WAITED_FOR_RESTART);
			}
			
			pkey = pcreation_data->getKeyPtr();
			pkey->preCompute(pkey->m_timeStamp.start, pcreation_data->m_pathWorkBench->getDriveTrainSpecifications());

			updateActionsTable_afterKeyDurationUpdate(GetParent(hwnd), pcreation_data->m_pathWorkBench, pcreation_data->m_keyIndex);
			UpdateWindow(hwnd);
			pcreation_data->m_pathWorkBench->requestTrajectoryBuild();
			return TRUE;
		}
		break; // case WM_COMMAND:

	// Messages Privés locaux:
		/*
	case WM_MESSAGE_DISPLAY_UPDATED_ACTION_TIMESHIFT:
		{
			paction = pcreation_data->getActionPtr();
			sprintf(txt, "%.2f", paction->m_core.m_timeShift);
			SetWindowText(GetDlgItem(hwnd, EDIT_ACTION_TIMESHIFT), txt);
			sprintf(txt, "= %.2f sec ", paction->m_core.m_timeStamp);
			SetWindowText(GetDlgItem(hwnd, STATIC_ACTION_TIME_STAMP), txt);
		}
		return TRUE;
		
	case WM_MESSAGE_UPDATE_ACTION_WINDOW_CONTENT:
		*pcreation_data = *( (WND_KEYACTION_CREATE_DATA*)lParam );
		if ((bool)wParam == TRUE)
			SetWindowPos(hwnd, NULL, NL_ACTIONWINDOW_X0, NL_ACTIONWINDOW_Y0 + NL_ACTIONWINDOW_YSHIFT * pcreation_data->m_actionIndex, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOCOPYBITS);
		_apply_action_values(pcreation_data->getActionPtr(), hwnd);
		return TRUE;
		*/

	//default:
	//	break;

	//default:
	//	return TRUE;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

	}
	return FALSE;
}

