#pragma once
#include "lib/N/NCStandard.h"

// Cette petite structure est utilis�e pour stoker le nom de chaque message dans les arrays suivants:
// NARRAY  UpToDateActionMessagesLUT; (App_Init.cpp)
// NARRAY m_actionMessagesLUT; ( NLProject.h )
// Seul le nom est enregistr�.
// L'ID du message correspond en fait � son index dans l'Array !
// ----------------------------------------------------------------------------------------------------
typedef struct NLACTIONMESSAGE NLACTIONMESSAGE;
struct NLACTIONMESSAGE
{
	Nu32 read(FILE *pfile) { return (fread(this, sizeof(NLACTIONMESSAGE), 1, pfile) == 1) ? 1 : 0; };
	Nu32 write(FILE *pfile) { return (fwrite(this, sizeof(NLACTIONMESSAGE), 1, pfile) == 1) ? 1 : 0; };

	Nchar m_name[32];
};
// ----------------------------------------------------------------------------------------------------
#define NLclearNLTrajectoryActionMessageInArrayCallBack NULL
// inline void NLclearNLTrajectoryActionMessageInArrayCallBack(void* p){};