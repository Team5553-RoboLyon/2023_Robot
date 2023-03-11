#pragma once
/*
#include <stdio.h>
#include "lib/N/NType.h"
#include "lib/N/Maths/NIntervalf32.h"
#include "lib/N/Core/NVersion.h"

#include "lib/NL/Simulation/NLPathGeometry.h"
#include "lib/NL/MotionControl/NL2DOrthogonalCoordinateSystem.h"

#include "NLTrajectoryPoint.h"

//#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPackGetPointPersistentResult.h"

//#include "lib/NL/MotionControl/NLPathWorkbench.h"

//#define FLAG_NLTRAJECTORY_CHUNK_available							BIT_0
#define FLAG_NLTRAJECTORY_CHUNK_SET_CORE_MODE_REVERSE		BIT_1	//	| ATTENTION Il faut que ce soit le BIT_1 !!! Car, 
																	//	|	= FLAG_NLTRJACTION_SET_CORE_MODE_REVERSE ( = BIT_1 )
																	//	|	= FLAG_NLTRJKEY_PROMOTE_PARAMS_SET_CORE_MODE_REVERSE ( = BIT_1 )


// forward declaration:
typedef struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT		NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT;
// Valeurs de retour de la fonction getTime( NINTERVAL ... )
#define NLTRAJECTORY_CHUNK_GETTIME_UNIQUE		 0	//  l'intervalle contient 2 fois la m�me valeur. Elle est unique et valide. le chunk est de type T. et s est inclu dan sle chunk.
#define NLTRAJECTORY_CHUNK_GETTIME_INTERVAL		 1	//  l'intervalle est un VRAI intervalle avec des valeurs start et end distinctes et valides. Le chunk est de type R et S est �gal � celui du chunk (unique ).
#define NLTRAJECTORY_CHUNK_GETTIME_BEFORE		-1	// -l'intervalle contient 2 fois la m�me valeur. s se situe 'avant' le chunk. La valeur est la plus petite associ�e au chunk.
#define NLTRAJECTORY_CHUNK_GETTIME_AFTER		-2	// -l'intervalle contient 2 fois la m�me valeur. s se situe 'apr�s' le chunk. La valeur est la plus grande associ�e au chunk.
class NLTRAJECTORY_CHUNK
{
public:
	NLTRAJECTORY_CHUNK() :m_t0(0.0f), m_t1(0.0f), m_s0(0.0f), m_s1(0.0f) {	printf("Entry in Constructor of NLTRAJECTORY_CHUNK::NLTRAJECTORY_CHUNK()\n");};
	virtual ~NLTRAJECTORY_CHUNK() {		printf("Entry in Destructor of NLTRAJECTORY_CHUNK::NLTRAJECTORY_CHUNK()\n");	};				// Destructeur virtuel pur, mais n�c�ssite un corps de fonction malgr� tout ( i-e norme )
	virtual void	selfDuplicate(void* palready_allocated_memory)=0;
	virtual void getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const NLPATH_GEOMETRY* ppathgeometry, const Nf32 t)=0;
	//virtual NLRAMSETEOUTPUT* output(NLRAMSETEOUTPUT*  poutput, const NLPATH_GEOMETRY* ppathgeometry, const Nf32 t) = 0;		// M�thode virtuelle pure ...	
	virtual void	initializePersistentData(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT *ppersist) = 0;	// M�thode virtuelle pure ...	
	virtual Nu32	read(FILE *pfile) = 0;			// M�thode virtuelle pure ...
	virtual Nu32	write(FILE *pfile) = 0;			// M�thode virtuelle pure ...

#ifdef _NEDITOR	
	virtual void	draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack) = 0;// M�thode virtuelle pure ...
#endif												

													
	virtual Nf32	getTime(const Nf32 s)=0;
	virtual Ns32	getTime(NINTERVALf32* ptimerange, const Nf32 s)=0;	
																		
																		


	void timeRange(const Nf32 t0, const Nf32 t1) { m_t0 = t0; m_t1 = t1; }
	void abscissaRange(const Nf32 s0, const Nf32 s1) { m_s0 = s0; m_s1 = s1; };

	Nf32	m_t0;									// Date de d�but de chunk. 
	Nf32	m_t1;									// Date de fin de chunk. L'intervalle de temps couvert par Le CHUNK(n) est:
													//							] CHUNK(n)->m_t0, CHUNK(n)->m_t1 ]
													//							avec CHUNK(n)->m_t0 = CHUNK(n-1)->m_t1
	Nf32	m_s0;									// Abscisse curviligne de d�but de chunk.
	Nf32	m_s1;									// Abscisse curviligne de fin de chunk.
};

inline void NLclearNLTrajectoryChunkInArrayCallBack(void* p) { ((NLTRAJECTORY_CHUNK*)p)->~NLTRAJECTORY_CHUNK(); };
inline void NLclearNLTrajectoryChunkPtrInArrayCallBack(void* p) { (*(NLTRAJECTORY_CHUNK**)p)->~NLTRAJECTORY_CHUNK(); };
inline void NLcopyExNLTrajectoryChunkInArrayCallBack(void* pdst, const void* psrc) { ((NLTRAJECTORY_CHUNK*)psrc)->selfDuplicate(pdst); };
*/