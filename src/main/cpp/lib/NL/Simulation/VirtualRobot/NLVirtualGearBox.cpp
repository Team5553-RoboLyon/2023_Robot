#include "../../../N/NMemory.h"
#include "../../../N/NMath.h"
#include "../../../N/File/NFile.h"
#include "../../../N/NString.h"
#include "../../../N/NErrorHandling.h"
				
#include "NLVirtualGearBox.h"


void NLVIRTUAL_GEARBOX::addDriveInput(const NLVIRTUAL_OUTPUT * pinput)
{
	if (m_drivenInputCount < CONSTANT_NLVIRTUAL_GEARBOX_MAX_NUMBER_OF_DRIVEN_INPUT)
	{
		for (Nu8 i = 0; i < m_drivenInputCount; i++)
		{
			// On vérifie qu'il n'y a pas double insertion 
			if (m_driveInput[i] == pinput)
			{
				NErrorIf(1, NERROR_VALUE_OUTOFRANGE);
				return;
			}
		}

		m_driveInput[m_drivenInputCount] = (NLVIRTUAL_OUTPUT*)pinput;
		m_drivenInputCount++;
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1, NERROR_VALUE_OUTOFRANGE);
	}
#endif
}

void NLVIRTUAL_GEARBOX::update(const Nf32 dt)
{
	Nf32	sumv = 0.0f; // somme des vitesses de tous les drivenInput ( = de tous les moteurs ) présents dans la gearbox 
	Nf32	suma = 0.0f; // somme des accélérations de tous les drivenInput ( = de tous les moteurs ) présents dans la gearbox 
	Nf32	sump = 0.0f; // somme des positions angulaires de tous les drivenInput ( = de tous les moteurs ) présents dans la gearbox 
						 // ( rappel: la position angulaire d'un driven input (moteur) représente le nombre de tours effectués par celui-ci )	
	
	// On Calcule la somme de chacune des 3 grandeurs vitesse, acceleration et position
	for (Nu8 i = 0; i < m_drivenInputCount; i++)
	{
		sumv += m_driveInput[i]->m_angularVelocity;
		suma += m_driveInput[i]->m_angularAcceleration;
		sump += m_driveInput[i]->m_angularPosition;
	}
	
	// ... Et on divise par le le nombre de driven input de la gearbox
	// ON CALCULE DONC SIMPLEMENT UNE MOYENNE pour chacune des 3 valeurs   
	// Ensuite on multiple par le invGearRatio de la gearbox pour obtenir le nombre de tours actuel, la vitesse et l'acceleration en sortie de boite.
	m_output.m_angularVelocity		= (sumv / (Nf32)m_drivenInputCount) * m_invGearRatio;
	m_output.m_angularAcceleration	= (suma / (Nf32)m_drivenInputCount) * m_invGearRatio;
	m_output.m_angularPosition		= (sump / (Nf32)m_drivenInputCount) * m_invGearRatio;
}
