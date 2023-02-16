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
 *	@brief	Recup�re l'index de l'action attach�e � une fen�tre action
 *
 *	@param	hwnd est un handle de fen�tre (qui doit absolument �tre de type ActionWinClass )
 *	@return	l'index de l'action attach�e.
 *			NVOID si hwnd est NULL	
 *	 		valeur ind�termin�e si la classe de fen�tre de hwnd n'est pas ActionWinClass
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
	// a)	Application des valeurs et Activation des contr�les en fonction du type de l'action:
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


	// On demande � enti�rement retracer la fen�tre, background compris	
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
	// a)	Application des valeurs et Activation des contr�les en fonction du type de l'action:
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
 *	@brief	Fct callback appel�e par la fct "EnumChildWindows"
 *			Cette fonction callback est appel�e pour chaque fen�tre enfant ( et n'intervient concretement que si cette fen�tre est une fen�tre action ).
 * 			Elle a pour but d'associer une action � cette fen�tre action, d�j� cr��e.
 *			Elle d�truit automatiquement la fen�tre si l'index de l'action pass� en param�tre [ '((ECW_ACTION_DATA*)lParam).m_actionIdx0' ] est 
 *			En dehors des limites de l'array des actions pass� lui aussi en param�tre.
 * 
 *	@param	hwnd est un handle de fen�tre (une des fen�tres enfants de la fen�tre m�re pass�e en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interpr�t�s comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action � partir de laquelle il faut se mettre � jour.
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	non utilis�.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions o� l'action d'index 'm_actionIdx0' est stock�e.
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
 *	@brief	Fct callback appel�e par la fct "EnumChildWindows"
 *			La ligne "KEYRESTART" vient s'ins�rer "parmis" la liste des fen�tres Action, en fonction de la valeur dur�e, fixant le timestamp de red�marrage.
 *			Tel que les choses sont faites, il est possible pour une clef de d�finir une date de red�marrage avant que toutes ces actions soient r�alis�es.
 *			Bien s�r, cela peut engendrer des incoh�rences ( certaines actiones n�c�ssitant d'�tre � l'arr�t, au sens translation le long du chemin, comme l'action de rotation )
 *			qui sont prise en charge par le fonction NLTRAJECTORY::build qui disable tout simplement ces actions.
 *			Le r�le de cette fonction est de g�rer la position des fen�tres action en fonction de la ligne "KEYRESTART" dont la position relative �volue car 
 *			la dur�e de la KEY est modifi�e.
 *			Ainsi les fen�tres action vont se repositionner pour laisser une nouvelle place � la ligne keyrestart qui sera d�plac�e juste apr�s 
 *			( en dehors de cette fonction callback bien evidement)	
 * 
 *	@param	hwnd est un handle de fen�tre (une des fen�tres enfants de la fen�tre m�re pass�e en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interpr�t�s comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action � partir de laquelle il faut se mettre � jour.
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	non utilis�.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions o� l'action d'index 'm_actionIdx0' est stock�e.
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
 *	@brief	Fct callback appel�e par la fct "EnumChildWindows"
 * 			Les actions sont class�es selon l'ordre croissant de leur timeShift. ( de 0 � xx sec ). Ainsi la premiere action de l'array ( index0 )
 *			a le timeshift le plus petit, et la derni�re le timeshift le plus grand.
 * 			Lors de la modification du TimeShift d'une action, (edition directe, moveUp ou moveDown) les actions sont r�ordonn�es dans l'array pour pr�server cet ordre croissant. 
 *			Il est n�c�ssaire de faire la m�me chose avec la liste des fen�tres actions affich�es et de les ordonner de haut en bas. C'est le r�le de cette fonction.
 * 			Cette fonction "d�place verticalement" les fen�tres Action, SANS EN MODIFIER LE CONTENU !
 * 			� l'exeption de la fen�tre action qui a �t� effectivement d�plac�e ( car le time shift de son action modifi�)
 *			et pour qui les contr�les d'�dition affichant "timeshift et timestamp" sont remis � jour. ( appel � la fonction 'SetWindowText' pour ces 2 contr�les )
 *			En plus de replacer les fen�tres action de haut en bas dans l'ordre croissant de leur timeshift, cette fonction met � jour les index des actions r�f�renc�s
 *			par chaque fen�tre action.
 *			En effet, les fen�tres actionref�rencent toujours les index des actions tels qu'ils �taient avant le d�placement de l'action.
 *			Ce d�placement dans l'array de l'action au timeshit edit�,  peut se voir comme une suppression de cette action suivit de sa r�insertion � une autre place. 	
 *			Cette fonction r�.�value l'index de l'action referenc�e par la fen�tre action et le met � jour pour prendre en compte cette "suppression+insertion".		
 * 
 *	@param	hwnd est un handle de fen�tre (une des fen�tres enfants de la fen�tre m�re pass�e en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interpr�t�s comme suit:
 * 
 *					((ECW_ACTION_DATA*)lParam)->m_pathWorkBench pointeur vers le Workbench courant.
 * 					((ECW_ACTION_DATA*)lParam)->m_keyIndex		Index de la clef 'parente' des actions en cours de traitement
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index de l'action avant update ( avant d�placement ).
 *	 				((ECW_ACTION_DATA*)lParam)->m_actionIdx1	contient l'index de l'action apr�s update ( apr�s d�placement ).
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

			// L'action attach�e � la fen�tre est celle qui a �t� d�plac�e !
			// Son d�placement correspond  � la diff�rence entre sa position finale et sa position initiale.

			//Nbool bistheone = 0;
			if (pcreation_data->m_actionIndex == pparams->m_actionIdx0)
			{
				//bistheone = 1;
				ridx = pparams->m_actionIdx1 - pparams->m_actionIdx0;
			}
			else
			{
				// On avance pas � pas, en 2 temps:
				//	1/	REMOVE:		Si l'action attach�e � cette fen�tre �tait situ�e apr�s la position initiale de l'action d�plac�e.
				//					Alors son index est diminu� de 1.
				//  2/	REINSERT:	Si l'action attach�e � cette fen�tre est situ�e apr�s la position finale de l'action d�plac�e ( r�ins�r�e ).
				//					Alors son index est augment� de 1.
				//	La somme de ces 2 d�placements donne le d�placement total subit par l'action attach� � cette fen�tre lors du 
				//	D�placement de l'action 'pparams->m_actionIdx0' � l'index 'pparams->m_actionIdx1'.
				ridx += (pcreation_data->m_actionIndex > pparams->m_actionIdx0) ? -1 : 0;
				ridx += ((pcreation_data->m_actionIndex + ridx) >= pparams->m_actionIdx1) ? 1 : 0;
			}

			/*
			// Un d�placement d'index se r�percute sur un d�placement de la fen�tre vertical:
			if(ridx)
			{
				pcreation_data->m_actionIndex += ridx;

				// Vu qu'il y a d�placement il y a de grande chance que le timeshift aie �t� modifi�
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
 *	@brief	Fct callback appel�e par la fct "EnumChildWindows" ...
 *			... Juste apr�s la suppression d'une action de l'array des actions et de la fenetre action associ�e.			
 *			Les fen�tres action actuellement en place ref�rencent toujours les index des actions tels qu'ils �taient avant la suppression.
 *			( sauf la fen�tre action qui �tait associ�e � l'action supprim�e et qui est normalement supprim�e �galement )
 *			Cette fonction callback est appel�e pour chacune des fen�tre enfant de la BDD keyProperties, et donc pour chacune des fen�tres actions.
 *			Ainsi, chaque fen�tre action referencant un index obsolete (> index de l'action supprim�e) est mise � jour ( index -= 1 ) afin
 *			de continuer � r�f�rencer la m�me structure action ( elle aussi d�cal�e de -1 apr�s l'insertion)
 *			par mesure de pr�caution cette fonction callback v�rifie aussi la presence d'une fenetre referencant un index identique � celui de l'action supprim�e
 *			ce qui revient � dire que la fen�tre action associ�e, qui aurait du �tre supprim�e, ne l'a pas �t� ! ... Si cela arrive l'index r�f�renc� par cette fen�tre 
 *			prend la valeur de NVOID !!!! ( et CRASH en debug )
 * 
 *	@param	hwnd est un handle de fen�tre (une des fen�tres enfants de la fen�tre m�re pass�e en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interpr�t�s comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index ( �valu� avant suppression ) de l'action supprim�e.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions o� l'action supprim�e �tait stock�e.
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
			else if (pcreation_data->m_actionIndex == pparams->m_actionIdx0) // la fen�tre va �tre supprim�e, mais pas ici.
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
 *	@brief	Fct callback appel�e par la fct "EnumChildWindows" ...
 *			... Juste apr�s l'insertion d'une action dans l'array des actions, et juste avant la cr�ation de la fenetre action associ�e.			
 *			Les fen�tres action actuellement en place ref�rencent toujours les index des actions tels qu'ils �taient avant l'insertion.
 *			Cette fonction callback est appel�e pour chacune des fen�tre enfant de la BDD keyProperties, et donc pour chacune des fen�tres actions.
 *			Ainsi, chaque fen�tre action referencant un index obsolete (>= index de l'action ins�r�e) est mise � jour ( index += 1 ) afin
 *			de continuer � r�f�rencer la m�me structure action ( elle aussi d�cal�e de +1 apr�s l'insertion)
 * 
 *	@param	hwnd est un handle de fen�tre (une des fen�tres enfants de la fen�tre m�re pass�e en param de EnumChildWindows )
 *	@param	lParam est ici un pointeur (ECW_ACTION_DATA*) dont les membres seront ici interpr�t�s comme suit:
 *					((ECW_ACTION_DATA*)lParam)->m_actionIdx0	contient l'index ( �valu� apr�s insertion ) de l'action ins�r�e.
 * 					((ECW_ACTION_DATA*)lParam)->m_pActionArray	contient un pointeur vers l'array des actions o� l'action ins�r�e est stock�e.
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
 *	@brief	Apr�s avoir supprim� une action de l'array des actions, il est n�c�ssaire de
 *			mettre � jour le tableau des fen�tres actions.
 *			Cette fonction doit �tre appel�e apr�s les suppressions de l'action et de la fen�tre Action associ�e.
 *			Le r�le de cette fonction est de mettre � jour les index des actions associ�es aux fen�tres actions en place
 *			( car la suppression de l'action a modifi� certains index de l'array ) et de modifier la position de ces fen�tres actions existantes,
 *			plus particuli�rement celles r�f�rencant des actions situ�es "apr�s" l'action supprim�e.
 *			Ainsi, apr�s l'appel � cette fonction, les fen�tres action situ�e "avant" (dont l'index n'a pas chang�) la fen�tre action supprim�e
 *			ne changent pas et restent � leur place alors que les fen�tres action situ�es "apr�s" ( dont l'index a �t� d�cal� de -1) "remontent"
 *			d'un "�tage". La place laiss�e vide par la fen�tre action supprim�e est ainsi combl�.
 *
 *	@param	hwnd est le handle de la  fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyidx est l'index de la clef en cours d'�dition
 *	@param	erasedactionidx est l'index (avant suppression) de l'action qui vient d'�tre supprim�e.
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterActionErase(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx, const Nu32 erasedactionidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pwb;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= erasedactionidx;
	ecw_params.m_actionIdx1		= NVOID;				// non utilis�
	EnumChildWindows(hwnd, afterActionErase_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Apr�s avoir ajout�/ins�r� une action dans l'array des actions, il est n�c�ssaire de
 *			mettre � jour le tableau des fen�tres actions et d'ajouter une fen�tre action.
 *			Cette fonction doit �tre appel�e apr�s la cr�ation de la nouvelle action dans l'array ET avant la cr�ation de la fen�tre Action qui sera associ�e
 *			� cette nouvelle Action. 
 *			Le r�le de cette fonction est de mettre � jour les index des actions associ�es aux fen�tres actions en place 
 *			( car l'insertion de la nouvelle action a modifi� certains index de l'array ) 
 *			et de modifier la position de ces fen�tres actions existantes, plus particuli�rement celles r�f�rencant des actions situ�es "apr�s" l'action ins�r�e. 
 *			Ainsi, apr�s l'appel � cette fonction, la place de la future fen�tre action est pr�te, elle n'a plus qu'� �tre cr��e et
 *			mise en place, dans le "trou" laiss� libre " entre" les fen�tres action situ�e "avant" (dont l'index n'a pas chang�)
 *			et les fen�tres action situ�es "apr�s" ( dont l'index a �t� d�cal� de +1).
 *
 *	@param	hwnd est le handle de la  fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyidx est l'index de la clef en cours d'�dition
 *	@param	newactionidx est l'index de l'action qui vient d'�tre cr��e et ajout�e � l'array des actions de la clef.
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterActionInsertion(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyidx, const Nu32 newactionidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pcurrent_workbench;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= newactionidx;
	ecw_params.m_actionIdx1		= NVOID;				// non utilis�
	EnumChildWindows(hwnd, afterActionInsertion_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Lors de l'�dition du timeshift d'une action, celle ci peut-�tre d�plac�e dans l'array des actions.
 *			En effet les actions sont class�es selon l'ordre croissant de leur timeShift. ( de 0 � xx sec ).
 *			Ainsi, lorsqu'une action se retrouve "d�plac�e" ( Edition du TimeShift, moveUp,moveDown) il est n�c�ssaire de r�percuter
 *			ce "d�placement" sur la liste des fen�tres actions affich�es. C'est le r�le de cette fonction.
 *			"updateActionsTable_afterActionIndexShifting" ne fait que d�placer les fen�tres Action, sans en modifier le contenu
 *			comme le fait la fonction 'updateActionsTable_afterKeyDurationUpdate', � l'exeption de la fen�tre action qui a �t� d�plac�e
 *			et pour qui le contenu "timeshift et timestamp" est mis � jour.
 *				
 *	@param	hwnd est le handle de la  fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyidx est l'index de la "nouvelle" clef en cours d'�dition
 *  @param	idx0 est l'index de l'action avant shifting ( avant d�placement ).
 *  @param	idx1 est l'index de l'action apr�s shifting ( apr�s d�placement ).
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
 *	@brief	Lorsque l'utilisateur s�lectionne une nouvelle Key pour en �diter le contenu, la
 *			fen�tre keyproperties se met compl�tement � jour.	
 *			Toutes les actions de la key pr�c�dente sont remplac�es par les actions de la nouvelle key.
 *			Le r�le de cette fonction est de r�aliser cette m�j en d�truisant et recr�ant un minimum de fen�tres action
 *			et en essayant de reutiliser les fen^tres action existantes en leur reaffectant un nouveau contenu � afficher/editer.
 *			Si besoin des fe�tres actions sont d�truites et d'autres recr�es.
 *
 *	@param	hwnd est le handle de la  fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyidx est l'index de la "nouvelle" clef en cours d'�dition
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterKeyIndexUpdate(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pwb;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= 0;
	ecw_params.m_actionIdx1		= NVOID;				// non utilis�
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
 *	@brief	Replace les fen�tres actions les unes par rapport aux autres.
 *			L'appel � cette fonction ne doit se faire que si le nombre d'actions n'a pas chang�.
 *			L'ordre des actions les unes par rapport aux autres reste identique.
 *			Cependant, la modification de la dur�e de la key, et donc la position de la ligne Keyrestart
 *			peut devoir changer et venir s'ins�rer entre 2 autres actions, ou compl�tement en bas de liste.
 *			En cons�quence les positions des fen�tres action peut �tre modifi� et doit �tre r�-�valu�.
 *			Le contenu des fen�tres actions est mis � jour, pour prendre en compte une potentielle d�sactivation ( PRECHECK_DISABLE ).
 *			suite � l amodification de la dur�e de la key.
 * 
 *	@param	hwnd est le handle de la fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyidx est l'index de la clef en cours d'�dition
 *
 *
 */
 // ------------------------------------------------------------------------------------------
void updateActionsTable_afterKeyDurationUpdate(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyidx)
{
	ECW_ACTION_DATA	ecw_params;
	ecw_params.m_pathWorkBench	= pcurrent_workbench;
	ecw_params.m_keyIndex		= keyidx;
	ecw_params.m_actionIdx0		= NVOID;				// non utilis�
	ecw_params.m_actionIdx1		= NVOID;				// non utilis�
	EnumChildWindows(hwnd, updateActionPosition_Callback, (LPARAM)&ecw_params);
	setKeyRestartPosition(hwnd, ecw_params.getKeyPtr());
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	Cr�e une nouvelle fen�tre Action associ�e � un index d'action et la place � la bonne 
 *			hauteur dans le tableau des fen�tres Actions. Attention cette fonction ne se charge pas 
 *			de d�placer les autres fen�tres action d�j� en place.
 * 
 *	@param	hwnd est le handle de la fen�tre keyProperties qui poss�dent toutes les fen�tres Actions en enfants.
 *	@param	pcurrent_workbench est un pointeur sur le WorkBench en cours d'�dition.
 *	@param	keyid est l'index de la clef en cours d'�dition
 *	@param	insertedactionindex est l'index de l'action qui vient d'�tre ajout�e ( element de l'array des action de la clef keyid )
 * 
 *	@return	le handle de la fen�tre action qui vient d'�tre cr��e et positionn�e.
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
 *	@brief	Cr�e les controls sp�cifiques aux actions rotate
 *			Chaque Action poss�de sa propre fenetre d'�dition. Chaque fenetre d'�dition d'action contient des controles, certains diff�rents selon l'ID de l'action.
 *			Cette fonction cr��e uniquement les contr�les sp�cifique � une action de type rotation.
 *
 *	@param	x		abscisse du premier contr�le sp�cifique de rotation ( le + � gauche, soit le contr�le d'�dition de l'angle de rotation ) 
 *	@param	y		ordonn�e du premier contr�le sp�cifique de rotation ( le + � gauche, soit le contr�le d'�dition de l'angle de rotation ) 
 *	@param	angle	valeur de l'angle de rotation � afficher par le controle Edit associ�
 *	@param	ratio	valeur du ratio de vitesse de rotation � afficher par le controle Edit associ�
 *	@param	hparent	handle de la fen�tre parent des contr�les qui vont �tre cr��s (i-e handle de la fen�tre action)
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
 *	@brief	Cr�e les controls sp�cifiques aux actions sendmessage
 *
 * ## Details
 * Chaque Action poss�de sa propre fenetre de controle. Chaque fenetre de controle contient diff�rents controles selon l'ID de l'action.
 * Les premiers controles, relatifs au TimeStamp( et Time shift ) de chaque action sont cr��s pour chacune d'elle.
 *
 *	@param	pdata
 *	@param	hparent
 *  @param	hinst
 *
 *	@return	la position horizontale (int) � laquelle devrait �tre dessin� le contr�le suivant
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
	NErrorIf(NLTRJACTION_NUMBER_OF_ID != 6, NERROR_SYSTEM_CHECK); // signifie qu'un changement de nbre d'ID de type d'action � eut lieu mais qu'il n'a pas �t� compl�tement

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
			// On demande � la fenetre parent de nous donner le handle de fenetre de "l'action courante"
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
		// 1/ Recup'et copie des donn�es de creation ( allocation et stockage dans la fenetre )
		// -----------------------------------------------------------------------------------------
		pcreation_data = (WND_KEYACTION_CREATE_DATA*)Nmalloc(sizeof(WND_KEYACTION_CREATE_DATA));
		*pcreation_data = *(WND_KEYACTION_CREATE_DATA*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLong(hwnd, GWL_USERDATA, (LONG)pcreation_data);
		
		// 2/ Creation des fen�tres Action:
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


		//	(a.3) Check Box :	"Auto" c'est � dire, prise en compte du timing de l'action dan sle calcul de la dur�e globale de la clef
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

		//	(b) Puis les contr�les specifiques de chaque action
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
					// pr�-�valuation de "paction->m_core.m_timeStamp"
					// paction->m_core.m_timeStamp est calcul� par la m�thode "NLTRAJECTORY::build" ( via un appel � la fonction "preComputeAction" )
					// Afin de donner et afficher une valeur imm�diatement on proc�de ici � une �valuation.
					// En effet, on sait que : 
					//								paction->m_core.m_timeStamp = pkey->m_timeStamp.start + paction->m_core.m_timeShift;
					// Ici on ne modifie que m_timeShift ...
					// En cons�quence on a :  paction->m_core.m_timeStamp = paction->m_core.m_timeStamp - previous_timeShift + new_timeShift
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
						// l'index de l'action n'a pas chang� mais potentiellement, son timeshift ayant �volu�, sa position relative par rapport
						// � la ligne Key restart � peut �tre chang� !
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

	// Messages Priv�s locaux:
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

