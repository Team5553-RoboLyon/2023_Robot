#pragma once
#include "../../../N/NCStandard.h"

// Ce fichier est exclusivement utilisé par le code du Robot ( et simulateur robot ).
// Il n'est pas utilisé par le logiciel d'édition des chemins ( exception faite du code de simulation contenu dans NLRobot.cpp )
// La liste des messages utilisés par le logiciel d'édition des chemins est issue du fichier : data/_actionmessages.txt
// Ce fichier (_actionmessages.txt) est chargé et interprété à chaque nouveau lancement du logiciel d'édition des chemins.
// Le programmeur du robot veillera à conserver la cohérence entre l'enum ci-dessous et le contenu du fichier _actionmessages.txt
// Les 2 décrivent le même ensemble de messages.

// ! Le premier message doit est égal à 0 ! 
// .. le suivant à 1, puis 2, etc ...
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
}NLACTIONMESSAGES_ENUM;