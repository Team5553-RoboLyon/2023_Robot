#pragma once
/*
#include "NLTrajectoryChunk.h"
#include "NLTrajectoryPoint.h"
#include "NLTrajectoryPointDesc.h"
#include "../../NL2DOrthogonalCoordinateSystem.h"

#define DEFAULT_TRAJECTORYPOINTDESC_ARRAY_CAPACITY		4
class NLTRAJECTORY_CHUNK_T : public NLTRAJECTORY_CHUNK
{
public:
	NLTRAJECTORY_CHUNK_T();
	~NLTRAJECTORY_CHUNK_T();
	void	selfDuplicate(void* palready_allocated_memory);

	void	initializePersistentData(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* ppersist);
	void	getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const NLPATH_GEOMETRY* ppathgeometry, const Nf32 t);
	Nu32	read(FILE* pfile);			
	Nu32	write(FILE* pfile);		

#ifdef _NEDITOR	
	void	draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack);
#endif												


	Nf32	getTime(const Nf32 s);
	Ns32	getTime(NINTERVALf32* ptimerange, const Nf32 s);

	void	build(const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid, const NLPATH_GEOMETRY* ppathgeometry);
	NARRAY	m_trajectoryPointDescArray;
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLTRAJECTORY_CHUNK_T_HEADER			NMAKEVERSION(0,0,0)				// >>> 2022/??/??
typedef struct
{
}NLTRAJECTORY_CHUNK_T_HEADER;

#define NLclearNLTrajectoryChunkTInArrayCallBack		NULL
//inline void NLclearNLTrajectoryChunkTInArrayCallBack(void* p){};
*/