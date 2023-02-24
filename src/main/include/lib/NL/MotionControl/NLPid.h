#pragma once

#include "lib/N/NType.h"
#include "lib/N/NMemory.h"

typedef struct NLPID_ERROR NLPID_ERROR;
struct NLPID_ERROR
{
	inline void reset() { Nmem0(this, NLPID_ERROR); }
	inline void update(const Nf32 error)
	{
		m_previous = m_current;
		m_current = error;
		m_derivate = m_current - m_previous;
		m_integral += error;
	}
	Nf32 m_current;	 // erreur actuelle
	Nf32 m_previous; // erreur pr�c�dente
	Nf32 m_derivate; // variation d'erreur
	Nf32 m_integral; // somme des erreurs
};

typedef struct NLPID NLPID;
struct NLPID
{
	Nf32 m_kP;
	Nf32 m_kI;
	Nf32 m_kD;
	inline Nf32 command(const NLPID_ERROR *perr) { return m_kP * perr->m_current + m_kI * perr->m_integral + m_kD * perr->m_derivate; }
};
