#pragma once
#include "lib/N/NCStandard.h"

// Ce fichier est exclusivement utilis� par le code du Robot ( et simulateur robot ).
// Il n'est pas utilis� par le logiciel d'�dition des chemins ( exception faite du code de simulation contenu dans NLRobot.cpp )
// La liste des messages utilis�s par le logiciel d'�dition des chemins est issue du fichier : data/_actionmessages.txt
// Ce fichier (_actionmessages.txt) est charg� et interpr�t� � chaque nouveau lancement du logiciel d'�dition des chemins.
// Le programmeur du robot veillera � conserver la coh�rence entre l'enum ci-dessous et le contenu du fichier _actionmessages.txt
// Les 2 d�crivent le m�me ensemble de messages.

// ! Le premier message doit est �gal � 0 !
// .. le suivant � 1, puis 2, etc ...
typedef enum
{
	OPEN_INTAKE = 0,
	CLOSE_INTAKE,
	ACTIVATE_INTAKE_WHEELS,
	DEACTIVATE_INTAKE_WHEELS,
	ACTIVATE_CONVEYOR,
	DEACTIVATE_CONVEYOR,
	ACTIVATE_AIM,
	DEACTIVATE_AIM,
	ACTIVATE_SHOOTER,
	DEACTIVATE_SHOOTER
} NLACTIONMESSAGES_ENUM;