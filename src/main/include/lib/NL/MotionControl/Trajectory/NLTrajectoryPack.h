#pragma once
#include "../../../N/NType.h"
#include "../../../N/NMatrix.h"
#include "../../../N/File/NFile.h"
#include "../../../N/Containers/NArray.h"
#include "../../../N/Core/NVersion.h"
#include "../../../N/Maths/NIntervalf32.h"
#include "../Path/Geometry/NLPathGeometry.h"

#include "../Path/NLPath.h"
#include "NLTrajectoryChunk.h"
#include "NLTrajectoryChunkT.h"
#include "NLTrajectoryChunkR.h"
#include "NLTrajectoryAction.h"
#include "NLTrajectoryPointDesc.h"
//#include "NLTrajectoryPackGetPointPersistentResult.h"
#include "../../MotionControl/DriveTrain/NLRamsete.h"



/*
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_AXLETRACK			BIT_0
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_X		BIT_1
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_Y		BIT_2
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_CENTEROFMASS_Z		BIT_3
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_STATICFRICTION		BIT_4
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_V		BIT_5
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_A		BIT_6
#define FLAG_NLTRAJECTORY_PACK_CHECK_RELEVANT_PARAMETERS_KINLIMITS_J		BIT_7
*/

#define NLTRAJECTORYPACK_GETTIME_UNIQUE		 0	//  l'intervalle contient 2 fois la même valeur. Elle est unique et valide. Le TRJPOINT est de KTYPE travelling.
#define NLTRAJECTORYPACK_GETTIME_INTERVAL	 1	//  l'intervalle est un VRAI intervalle avec des valeurs start et end distinctes et valides. Le TRJPOINT est de KTYPE SPOT (ou BACK_AND_FORTH).
#define NLTRAJECTORYPACK_GETTIME_BEFORE		-1	// -l'intervalle contient 2 fois la même valeur ( = 0.0f). s se situe 'avant' le début de la trajectoire ( s< 0 ).
#define NLTRAJECTORYPACK_GETTIME_AFTER		-2	// -l'intervalle contient 2 fois la même valeur ( = m_dt). s se situe 'après' la fin de la trajectoire. ( s> m_ds )

#ifdef _NEDITOR
#include "../../NL2DOrthogonalCoordinateSystem.h"
// forward declarations:
typedef struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT		NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT;
class NLPATH_WORKBENCH; // of NLPathWorkbench ( to include it into SPACK, which is also included into NLPathWorkbench )
#endif
class NLTRAJECTORY_PACK
{
public:
	NLTRAJECTORY_PACK();
	~NLTRAJECTORY_PACK();
	void	erase();
	void	setup(const NLPATH* ppath, const NLDRIVETRAINSPECS* pdtspecs, const NARRAY* pusedkeysarray);

#ifdef _NEDITOR
	Nu32	read(NLPATH_WORKBENCH* pwb);						// Fonction read "spéciale" qui copie les données du path workbench passé en paramètre plutot
	Nu32	load(NLPATH_WORKBENCH* pwb) { return read(pwb); };	// que de les lire dans un fichier. Cela permet au code du simulateur de rester très proche 
#endif															// du code "réel" du robot	tout en permettant de recupérer directement les données du path workbench. 		

	Nu32	read(FILE* pfile);
	Nu32	write(FILE *pfile);

	Nu32	load(const Nchar* pfilename);
	Nu32	save(const Nchar* pfilename);

	void	getPoint(NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT* presult, const Nf32 t);

	Nf32	getTime(const Nf32 s);
	Ns32	getTime(NINTERVALf32* ptimerange, const Nf32 s);
	
	Nf32	getLastTrjPtDescTime();

#ifdef _NEDITOR
//	void	drawChunksArray(NL2DOCS * p2docs, const NCOLORPICKPACK pickpack);
#endif

	NLTRJPOINT_DESC_CFG buildTravelingChunk(const NLTRJPOINT_DESC_CFG cfg, const NARRAY* pkinsarray);
	//NLTRJPOINT_DESC_CFG buildTravelingChunk(const NLTRJPOINT_DESC_CFG cfg, const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid = NVOID);
	//NLTRJPOINT_DESC_CFG buildSpotChunk(const NLTRJPOINT_DESC_CFG cfg, const Nf32 s, const NARRAY* pkinsarray, const Nu32 firstkinid, const Nu32 lastkinid = NVOID);
	NLTRJPOINT_DESC_CFG buildSpotChunk(const NLTRJPOINT_DESC_CFG cfg, const Nf32 s, const NARRAY* pkinsarray);

//	NARRAY					m_triggersArray;
	NARRAY					m_trajectoryPointDescArray;
	NARRAY					m_spotsArray;

	NLPATH_GEOMETRY			m_pathGeometry;
	NMATRIX					m_matrix;
	Nf32					m_dt;	// Durée totale nécéssaire pour parcourir la trajectoire
	Nf32					m_ds;	// Distance totale couverte par la trajectoire ! Attention, ds représente la distance totale parcourue par le 
									// centre d'inertie et dont les abscisses successives s'enchainent dans les chunkT du pack inclus dans la trajectoire.
									// m_ds ne représente pas la(les) distances parcourue(s) par les roues droites et où gauche, qui sont dépendantes de la courbure
									// du chemin suivi et des potentielles rotation sur place ( décrites dans les chunkR du pack ).
	NLDRIVETRAINSPECS		m_driveTrainSpecifications;

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
	
}NLTRAJECTORY_PACK_HEADER;
