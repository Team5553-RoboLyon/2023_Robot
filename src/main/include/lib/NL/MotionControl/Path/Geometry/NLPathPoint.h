#pragma once
#include "../../../../N/NType.h"
#include "../../../../N/NMemory.h"
#include "../../../../N/Maths/NVec2f32.h"

typedef struct NLPATH_POINT NLPATH_POINT;
struct NLPATH_POINT
{
	void null() { Nmem0(this, NLPATH_POINT); };
	// friend class NLTRAJECTORY;
	// --------------------------------------------------------------------
	// Membres definis a la creation du Path par la methode NLPATH::Build() 
	NVEC2f32		p;	// Position 
	Nf32			k;	// Courbure en ce point
	NVEC2f32		u;	// Direction ( tangente en p orientee dans le sens du chemin, du debut vers la fin )
	Nf32			s;	// Abscisse curviligne
	// --------------------------------------------------------------------
	// Membre(s) mis a jour/utilises lors de la construction d'une "Trajectory" a partir du "Path" possedant ce KeyPoint ( mise a jour par la methode NLTRAJECTORY::Build(...) 
//private:
//	Nf32			m_localVelocityLimit;
};

#define NLclearNLPathPointInArrayCallBack		NULL
#define NLclearNLPathPointPtrInArrayCallBack	NULL
//inline void NLclearNLPathPointInArrayCallBack(void* p) { ((NLPATH_POINT*)p)->...; };
//inline void NLclearNLPathPointPtrInArrayCallBack(void* p) { (*(NLPATH_POINT**)p)->...; };



/*
// Version simplifiee et minimaliste d'un point de path.
// Utilisee exclusivement comme structure de resultat en retour de fonction de calcul/estimation de path point
// 	
typedef struct
{
	NVEC2f32		p;	// Position 
	Nf32			k;	// Courbure en ce point
}NLPATH_POINT_S;
*/