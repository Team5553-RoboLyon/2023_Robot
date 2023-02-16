#pragma once
#include "../../N/NType.h"


typedef struct NLKINTWEAK NLKINTWEAK;
struct NLKINTWEAK
{
	NLKINTWEAK():m_aHoldLocalLimitRatio(1.0f), m_jHoldLocalLimitRatio(1.0f){}
	NLKINTWEAK(const Nf32 accel_ratio, const Nf32 jrk_ratio) :m_aHoldLocalLimitRatio(accel_ratio), m_jHoldLocalLimitRatio(jrk_ratio) {}
	NLKINTWEAK(NLKINTWEAK const& tobecopied);
	NLKINTWEAK(const NLKINTWEAK* pktwk);


	Nf32		m_aHoldLocalLimitRatio;	// Ratio compris entre 0 et 1 permettant de moduler l'acceleration retenue entre, l'acceleration locale max. possible ( modèle trapezoidal ) et l'acceleration max de la DT.
	Nf32		m_jHoldLocalLimitRatio; // Ratio compris entre 0 et 1 permettant de moduler le jerk retenu entre un jerk qui aurait été modulé dans les mêmes proportions que l'acceleration et  le jerk max de la DT.
};										
