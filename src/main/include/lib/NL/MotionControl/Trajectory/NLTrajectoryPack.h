#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NMatrix.h"
//#include "../../../N/File/NFile.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Core/NVersion.h"
#include "lib/N/Maths/NIntervalf32.h"
#include "lib/NL/MotionControl/Path/Geometry/NLPathGeometry.h"

#include "lib/NL/MotionControl/Path/NLPath.h"
//#include "lib/NL/MotionControl/Trajectory/NLTrajectoryAction.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPointDesc.h"
#include "lib/NL/MotionControl/DriveTrain/NLRamsete.h"



#define NLTRAJECTORYPACK_GETTIME_UNIQUE		 0	//  l'intervalle contient 2 fois la m�me valeur. Elle est unique et valide. Le TRJPOINT est de KTYPE travelling.
#define NLTRAJECTORYPACK_GETTIME_INTERVAL	 1	//  l'intervalle est un VRAI intervalle avec des valeurs start et end distinctes et valides. Le TRJPOINT est de KTYPE SPOT (ou BACK_AND_FORTH).
#define NLTRAJECTORYPACK_GETTIME_BEFORE		-1	// -l'intervalle contient 2 fois la m�me valeur ( = 0.0f). s se situe 'avant' le d�but de la trajectoire ( s< 0 ).
#define NLTRAJECTORYPACK_GETTIME_AFTER		-2	// -l'intervalle contient 2 fois la m�me valeur ( = m_dt). s se situe 'apr�s' la fin de la trajectoire. ( s> m_ds )

// forward declarations:
typedef struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT		NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT;

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
class NLPATH_WORKBENCH; // of NLPathWorkbench ( to include it into SPACK, which is also included into NLPathWorkbench )
#endif
class NLTRAJECTORY_PACK
{
public:
	NLTRAJECTORY_PACK();
	~NLTRAJECTORY_PACK();

#ifdef _NEDITOR
	void	erase();
	void	setup(const NLPATH* ppath, const NLDRIVETRAINSPECS* pdtspecs, const NLRAMSETE *pramsete, const NARRAY* pusedkeysarray);
	Nu32	read(NLPATH_WORKBENCH* pwb);						// Fonction read "spéciale" qui copie les données du path workbench passé en paramètre plutot
	Nu32	load(NLPATH_WORKBENCH* pwb) { return read(pwb); };	// que de les lire dans un fichier. Cela permet au code du simulateur de rester très proche 
#endif															// du code "réel" du robot	tout en permettant de recupérer directement les données du path workbench. 		

	Nu32	read(FILE* pfile);
	Nu32	write(FILE *pfile);

	Nu32	load(const Nchar* pfilename);
	Nu32	save(const Nchar* pfilename);
	void	getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const Nf32 t);

#ifdef _NEDITOR
	Nf32	getTime(const Nf32 s);
	Ns32	getTime(NINTERVALf32* ptimerange, const Nf32 s);
	Nf32	getLastTrjPtDescTime();
	NLTRJPOINT_DESC* getLastTrjPtDesc() { return (NLTRJPOINT_DESC*)NGetLastArrayPtr(&m_trajectoryPointDescArray); }
	NLTRJPOINT_DESC_CFG buildTravelingChunk(const NLTRJPOINT_DESC_CFG cfg, const NARRAY* pkinsarray);
	NLTRJPOINT_DESC_CFG buildSpotChunk(const NLTRJPOINT_DESC_CFG cfg, const Nf32 s, const NARRAY* pkinsarray);
	void postFirstActionMessage();
	void postLastActionMessage();
	void postActionMessage(const NLTRJACTION* paction, const Nf32 nxtkey_timestart);
#endif	

	NARRAY					m_trajectoryPointDescArray;
	NARRAY					m_spotsArray;
	NARRAY					m_postedMessagesArray;

	NLPATH_GEOMETRY			m_pathGeometry;
	NMATRIX					m_matrix;
	Nf32					m_dt;	// Dur�e totale n�c�ssaire pour parcourir la trajectoire
	Nf32					m_ds;	// Distance totale couverte par la trajectoire ! Attention, ds repr�sente la distance totale parcourue par le 
									// centre d'inertie et dont les abscisses successives s'enchainent dans les chunkT du pack inclus dans la trajectoire.
									// m_ds ne repr�sente pas la(les) distances parcourue(s) par les roues droites et o� gauche, qui sont d�pendantes de la courbure
									// du chemin suivi et des potentielles rotation sur place ( d�crites dans les chunkR du pack ).
	NLDRIVETRAINSPECS		m_driveTrainSpecifications;
	NLRAMSETE				m_ramsete;
};


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define EXTENSION_NLTRAJECTORY_PACK_BIN				".trk"
#define SIGNATURE_NLTRAJECTORY_PACK_BIN				NMAKESIGNATURE('T','R','J','K')
#define VERSION_NLTRAJECTORY_PACK_HEADER			NMAKEVERSION(0,0,0)				// >>> 2022/11/08	(2021/04/05)
typedef struct
{
	Nf32				m_dt;
	NMATRIX				m_matrix;
	NARRAYBOUNDS		m_spotsArrayBounds;
	NARRAYBOUNDS		m_trajectoryPointDescArrayBounds;
	NARRAYBOUNDS		m_postedMessagesArrayBounds;

}NLTRAJECTORY_PACK_HEADER;
