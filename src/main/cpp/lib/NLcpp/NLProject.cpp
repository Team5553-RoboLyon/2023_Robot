#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
#include <new>
#include "lib/N/File/NDataPacker.h"
#include "lib/N/Containers/NXNode.h"
#include "lib/N/Render/Renderable/NRenderableScanner.h"

#include "lib/NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalChain.h"
#include "lib/NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryActionMessage.h"

#include "lib/NL/NLProject.h"
/*
static void DeletePathWorkbenchComponents(void* pwb)
{
	delete ( ((NLPATH_WORKBENCH*)pwb)->getPathBuilder() );
	delete (((NLPATH_WORKBENCH*)pwb)->getPath());
	((NLPATH_WORKBENCH*)pwb)->getTrajectory()->setPath(NULL);
	delete (((NLPATH_WORKBENCH*)pwb)->getTrajectory());
	delete (((NLPATH_WORKBENCH*)pwb)->getTrajectoryStatePack());
	delete (((NLPATH_WORKBENCH*)pwb)->getTrajectoryStateSPack());
}
*/
static void DeletePathWorkbenchComponents(void *ptr)
{
	NLPATH_WORKBENCH *pwb = (NLPATH_WORKBENCH *)ptr;

	pwb->getTrajectory()->setPath(NULL);

	delete (pwb->getPathBuilder());
	delete (pwb->getPath());
	delete (pwb->getTrajectory());

	pwb->m_pPathBuilder = NULL;
	pwb->m_pPath = NULL;
	pwb->m_pTrajectory = NULL;
	pwb->m_pDriveTrainSpecifications = NULL;
}

NLPROJECT::NLPROJECT() : m_simulationDt(0.02f), m_driveTrainSpecifications(DEFAULT_NLPROJECT_DT_CHARACTERIZATION), m_pCurrentWorkbench(NULL)
{
	/*
	NLDRIVETRAINSPECS			m_pDriveTrainSpecifications;	|_ Initialization list
	NLPATH_WORKBENCH			*m_pCurrentWorkbench;			|

	NARRAY						m_workbenchArray;				|- Constructor

	*/
	NSetupArray(&m_workbenchArray, DEFAULT_INITIAL_WORKBENCH_ARRAY_CAPACITY, sizeof(NLPATH_WORKBENCH));
	NSetupArray(&m_actionMessagesLut, DEFAULT_INITIAL_ACTIONMESSAGESLUT_ARRAY_CAPACITY, sizeof(NLACTIONMESSAGE));
}

NLPROJECT::~NLPROJECT()
{
	NClearArray(&m_workbenchArray, DeletePathWorkbenchComponents);
	NClearArray(&m_actionMessagesLut, NLclearNLTrajectoryActionMessageInArrayCallBack);
}

#ifdef _NEDITOR
void NLPROJECT::initRendering()
{
	NSetUpXNodeList(&m_renderList);
	NCreateRenderableScanner((void *)&m_renderList, NRenderableScannerCallBack_RenderableAddress_XList);
	m_playground.setup(100.0f, NULL);
	m_playground.addInRenderPipeline(&m_renderList);
}

void NLPROJECT::endRendering()
{
	m_playground.removeFromRenderPipeline(&m_renderList);
	m_playground.clear();
	// La liste de rendu n'est pas vide ???!!!
	NErrorIf(NXNodeIsValid(m_renderList.pFirst, &m_renderList), NERROR_SYSTEM_CHECK);
}

void NLPROJECT::draw()
{
	m_playground.draw();
}
#endif

Nu32 NLPROJECT::createPathWorkBench(NLPATH_WORKBENCH::ID id)
{
	Nu32 index = m_workbenchArray.Size;

	NLPATH_WORKBENCH *pworkbench = (NLPATH_WORKBENCH *)NArrayAllocBack(&m_workbenchArray);

	// Setup "� la main" du Workbench
	pworkbench->setId(id);

	switch (id)
	{
	case NLPATH_WORKBENCH::ID::WB_WAYPOINTS:
		pworkbench->setPathBuilder((NLPATH_BUILDER *)new NLPATH_BUILDER_WAYPOINTS);
		break;
	case NLPATH_WORKBENCH::ID::WB_PLG_CHAIN:
		pworkbench->setPathBuilder((NLPATH_BUILDER *)new NLPATH_BUILDER_WAYPOINTS);
		break;
	default:
		break;
	}

	pworkbench->setRamseteParams(&m_ramseteParams);
	pworkbench->setDriveTrainSpecifications(&m_driveTrainSpecifications);
	pworkbench->setPath((NLPATH *)new NLPATH);
	pworkbench->setTrajectory((NLTRAJECTORY *)new NLTRAJECTORY);
	return index;
}

void NLPROJECT::erasePathWorkBench(const Nu32 index)
{
	NEraseArrayElement(&m_workbenchArray, index, DeletePathWorkbenchComponents);
}

void NLPROJECT::eraseAllPathWorkBenches()
{
	NEraseArray(&m_workbenchArray, DeletePathWorkbenchComponents);
}

void NLPROJECT::close()
{
	/*
	NLPATH* pp = new NLPATH;
	delete pp;

	NLTRAJECTORY_PACK* psp = new NLTRAJECTORY_PACK;
	delete psp;
	*/
	NEraseArray(&m_workbenchArray, DeletePathWorkbenchComponents);
}

void NLPROJECT::initActionMessagesLut(const NARRAY *plut)
{
	NCopyArray(&m_actionMessagesLut, plut);
}

Nu32 NLPROJECT::isActionMessagesLutUpToDate(const NARRAY *preflut)
{
	NErrorIf(!preflut, NERROR_NULL_POINTER);
	NErrorIf(preflut->ElementSize != m_actionMessagesLut.ElementSize, NERROR_INCONSISTENT_PARAMETERS);

	if (preflut->Size != m_actionMessagesLut.Size)
		return 0;

	NLACTIONMESSAGE *pm;
	NLACTIONMESSAGE *prf;
	pm = (NLACTIONMESSAGE *)m_actionMessagesLut.pFirst;
	prf = (NLACTIONMESSAGE *)preflut->pFirst;
	for (Nu32 i = 0; i < m_actionMessagesLut.Size; i++, pm++, prf++)
	{
		if (strcmp(pm->m_name, prf->m_name))
			return 0;
	}
	return 1;
}

void NLPROJECT::UpdateActionMessagesLut(const NARRAY *preflut)
{
}

Nu32 NLPROJECT::save(Nchar *pfilename)
{
	// Signature
	// Version
	// Header
	// DTSpecs					-> oui
	// Array of Workbenches		-> OUI
	// Array of PathBuilder		-> oui
	// Array of Path ?			-> non dans un premier temps puis ... options ?
	// Array of Trajectory ?	-> Trajectory Specs oui,  mais data calcul�s ... non dans un premier temps puis ... options ?
	// Array of StatePack ?		-> non
	// Array of State Spacks ?  -> non

	NDATAPACKER datapacker;
	NDATAPACK *pdatapack;
	NLPROJECT_HEADER headerprj;

	// 0) Ouverture du fichier en ecriture
	FILE *pfile = fopen(pfilename, "wb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;
	// 1) Ecriture Signature et Version
	Nu32 _u32 = FILE_SIGNATURE_NLPROJECT;
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}
	_u32 = VERSION_NLPROJECT_HEADER;
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}

	// 2) Ecriture du Header
	Nmem0(&headerprj, NLPROJECT_HEADER);
	NGetArrayBounds(&headerprj.m_actionMessagesLutBounds, &m_actionMessagesLut);
	if (fwrite(&headerprj, sizeof(NLPROJECT_HEADER), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}
	// 3) Ecriture DATA:
	//	  ... de m_actionMessagesLUT � la main et en une fois
	if (m_actionMessagesLut.Size)
	{
		if (fwrite(m_actionMessagesLut.pFirst, m_actionMessagesLut.ElementSize, m_actionMessagesLut.Size, pfile) != m_actionMessagesLut.Size)
			return 0;
	}
	// ... et des autres donn�es
	if (m_playground.write(pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}

	if (m_ramseteParams.write(pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}
	if (fwrite(&m_simulationDt, sizeof(Nf32), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}

	// 4) Ecriture DATA via DATAPACK:
	NSetupDataPacker(&datapacker, DPKEY_NB);
	// Setup du DataPacker
	pdatapack = NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::WORK_BENCH, sizeof(NLPATH_WORKBENCH), writeDataPack_Work_Bench, readDataPack_Work_Bench);
	NInsertDataPackNode(pdatapack, DP_ARRAY_DP_OBJ, &m_workbenchArray, m_workbenchArray.Size);
	pdatapack = NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::DT_SPECS, sizeof(NLDRIVETRAINSPECS), writeDataPack_DTSpecs, readDataPack_DTSpecs);
	NInsertDataPackNode(pdatapack, USR_OBJ, &m_driveTrainSpecifications, 1);

	NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH, sizeof(NLPATH), writeDataPack_Path, readDataPack_Path);
	NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH_BLDR_WP, sizeof(NLPATH_BUILDER_WAYPOINTS), writeDataPack_Path_Builder_Wp, readDataPack_Path_Builder_Wp);
	NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH_BLDR_PLG, sizeof(NLPATH_BUILDER_POLYGONAL_CHAIN), writeDataPack_Path_Builder_Plg, readDataPack_Path_Builder_Plg);
	NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::TRAJECTORY, sizeof(NLTRAJECTORY), writeDataPack_Trajectory, readDataPack_Trajectory);
	if (!NWriteDataPacker(&datapacker, pfile))
	{
		fclose(pfile);
		return 0;
	}
	fclose(pfile);

	NClearDataPacker(&datapacker);

	return 1;
}

Nu32 NLPROJECT::open(Nchar *pfilename)
{
	NDATAPACKER datapacker;
	NDATAPACK *pdatapack;

	// 0) Ouverture du fichier en lecture
	FILE *pfile = fopen(pfilename, "rb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;
	/*
	NARRAY tst;
	NSetupArray(&tst, 2, sizeof(NLPATH_BUILDER_WAYPOINTS));

	NLPATH_BUILDER_WAYPOINTS pb;
	NArrayPushBack(&tst, (NBYTE*)&pb);
	NArrayPushBack(&tst, (NBYTE*)&pb);
	NLPATH_BUILDER_WAYPOINTS *ppb;

	ppb = (NLPATH_BUILDER_WAYPOINTS*)tst.pFirst;
	ppb->read(pfile);

	void* pv = (void*)tst.pFirst;
	((NLPATH_BUILDER_WAYPOINTS*)pv)->read(pfile);
	*/

	// 1) Lecture Signature et Version
	Nu32 _u32;
	if (fread(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}
	if (!NSIGNATURECMP(_u32, FILE_SIGNATURE_NLPROJECT))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_UNKNOWN_SIGNATURE);
		return 0;
	}

	// Version
	if (fread(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}

	// Go!
	NLPROJECT_HEADER headerprj;
	switch (NGETVERSION_MAIN(_u32))
	{
		// Current Main Version ( all versions 0.0.x )
	case NGETVERSION_MAIN(VERSION_NLPROJECT_HEADER):
		// 2) Lecture de Header
		if (fread(&headerprj, sizeof(NLPROJECT_HEADER), 1, pfile) != 1)
		{
			fclose(pfile);
			NErrorIf(1, NERROR_FILE_READ_ERROR);
			return 0;
		}
		// 3) Lecture DATA:
		//	  ... de m_actionMessagesLUT � la main et en une fois
		if (NIsArrayCorruptedOrInconsistent(&m_actionMessagesLut, &headerprj.m_actionMessagesLutBounds, NTRUE)) // signifie qu'il y a un pb ( NARRAYCHK_INCONSISTENCY ou NARRAYCHK_CORRUPTED )
		{
			fclose(pfile);
			NErrorIf(1, NERROR_FILE_READ_ERROR);
			return 0;
		}
		NResizeArray(&m_actionMessagesLut, headerprj.m_actionMessagesLutBounds.Size, NULL, NULL);

		if (m_actionMessagesLut.Size)
		{
			if (fread(m_actionMessagesLut.pFirst, m_actionMessagesLut.ElementSize, m_actionMessagesLut.Size, pfile) != m_actionMessagesLut.Size)
			{
				fclose(pfile);
				return 0;
			}
		}
		// !!!
		// ... CETTE FONCTION NE PREND PAS EN CHARGE LA MISE A JOUR POTENTIELLE DE LA LUT DES MESSAGES.
		// !!!

		// ... et des autres donn�es
		if (m_playground.read(pfile) != 1)
		{
			fclose(pfile);
			return 0;
		}

		if (m_ramseteParams.read(pfile) != 1)
		{
			fclose(pfile);
			return 0;
		}
		if (fread(&m_simulationDt, sizeof(Nf32), 1, pfile) != 1)
		{
			fclose(pfile);
			return 0;
		}

		// 4) Lecture  DATA via DATAPACK:
		NSetupDataPacker(&datapacker, DPKEY_NB);
		// Setup du DataPacker
		pdatapack = NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::WORK_BENCH, sizeof(NLPATH_WORKBENCH), writeDataPack_Work_Bench, readDataPack_Work_Bench);
		NInsertDataPackNode(pdatapack, DP_ARRAY_DP_OBJ, &m_workbenchArray, m_workbenchArray.Size);
		pdatapack = NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::DT_SPECS, sizeof(NLDRIVETRAINSPECS), writeDataPack_DTSpecs, readDataPack_DTSpecs);
		NInsertDataPackNode(pdatapack, USR_OBJ, &m_driveTrainSpecifications, 1);

		NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH, sizeof(NLPATH), writeDataPack_Path, readDataPack_Path);
		NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH_BLDR_WP, sizeof(NLPATH_BUILDER_WAYPOINTS), writeDataPack_Path_Builder_Wp, readDataPack_Path_Builder_Wp);
		NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::PATH_BLDR_PLG, sizeof(NLPATH_BUILDER_POLYGONAL_CHAIN), writeDataPack_Path_Builder_Plg, readDataPack_Path_Builder_Plg);
		NInsertDataPack(&datapacker, NLPROJECT::DPKEY_ID::TRAJECTORY, sizeof(NLTRAJECTORY), writeDataPack_Trajectory, readDataPack_Trajectory);

		if (!NReadDataPacker(&datapacker, pfile))
		{
			fclose(pfile);
			NErrorIf(1, NERROR_FILE_READ_ERROR);
			return 0;
		}
		NClearDataPacker(&datapacker);
		break;

		// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		break;
	}

	fclose(pfile);
	return 1;
}

// TODO: Read and Write for ...
/*

*/

// Nu32(*NLSERIAL_WRITE)(void* pelement, NLSERIALIZER* pserializer);
// Nu32(*NLSERIAL_READ)(void* pelement, NLSERIALIZER* pserializer);

// Write Callbacks
Nu32 writeDataPack_DTSpecs(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLDRIVETRAINSPECS *)pl)->write(pfile);
}
Nu32 writeDataPack_Work_Bench(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLPATH_WORKBENCH *)pl)->write(pdpacker, pfile);
}

Nu32 writeDataPack_Path_Builder_Wp(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLPATH_BUILDER_WAYPOINTS *)pl)->write(pfile);
}

Nu32 writeDataPack_Path_Builder_Plg(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLPATH_BUILDER_POLYGONAL_CHAIN *)pl)->write(pfile);
}

Nu32 writeDataPack_Path(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLPATH *)pl)->write(pfile);
}

Nu32 writeDataPack_Trajectory(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	return ((NLTRAJECTORY *)pl)->write(pfile, pdpacker);
}

// Read Callbacks
Nu32 readDataPack_DTSpecs(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{

	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLDRIVETRAINSPECS *pdts = (NLDRIVETRAINSPECS *)new (pl) NLDRIVETRAINSPECS;
	return pdts->read(pfile);
}

Nu32 readDataPack_Work_Bench(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLPATH_WORKBENCH *pwb = (NLPATH_WORKBENCH *)new (pl) NLPATH_WORKBENCH;
	return pwb->read(pdpacker, pfile);
}

Nu32 readDataPack_Path_Builder_Wp(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLPATH_BUILDER_WAYPOINTS *pbwp = (NLPATH_BUILDER_WAYPOINTS *)new (pl) NLPATH_BUILDER_WAYPOINTS;
	return pbwp->read(pfile);
}

Nu32 readDataPack_Path_Builder_Plg(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLPATH_BUILDER_POLYGONAL_CHAIN *pbpc = (NLPATH_BUILDER_POLYGONAL_CHAIN *)new (pl) NLPATH_BUILDER_POLYGONAL_CHAIN;
	return pbpc->read(pfile);
}

Nu32 readDataPack_Path(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLPATH *ppth = (NLPATH *)new (pl) NLPATH;
	return ppth->read(pfile);
}

Nu32 readDataPack_Trajectory(void *pl, NDATAPACKER *pdpacker, FILE *pfile)
{
	// Inplace call to the constructor:		pas d'allocation m�moire ( d�j� faite par un malloc )
	//										Mais appel du constructeur de la classe
	NLTRAJECTORY *ptrj = (NLTRAJECTORY *)new (pl) NLTRAJECTORY;
	return ptrj->read(pfile, pdpacker);
}

/*
legacy some code of previous version of project READ
		// 3) Lecture DATA:
		// |
		// |__3.1) DriveTrain Spec:
		if (!m_driveTrainSpecifications.write(pfile))
			return 0;

		// -----------------------------------------------------



		// -----------------------------------------------------

		// |
		// |__3.2) Array of Workbench... on lit � la main pour s'affranchir de NFILE lib...
	// .d'abord la taille !
		if (fread(&wpasize, sizeof(Nu32), 1, pfile) != 1)
			return 0;
		NErrorIf(m_workbenchArray.Size, NERROR_ARRAY_NOT_EMPTY); // Normalement il y a eut un rest du projet avant !
		NResizeArray(&m_workbenchArray, wpasize, NULL, DeletePathWorkbenchComponents);
		//. Ensuite on read chaque builder, 1 � 1
		pwb = (NLPATH_WORKBENCH*)m_workbenchArray.pFirst;
		for (i = 0; i < m_workbenchArray.Size; i++, pwb++)
		{
			if (pwb->m_pPathBuilder->write(pfile) != 1)
				return 0;
		}


legacy some code of previous version of project WRITE
	// |
	// |__3.1) DriveTrain Spec:
	if (!m_driveTrainSpecifications.write(pfile))
		return 0;
	// |
	// |__3.2) Array of Workbench... on sauve � la main pour s'affranchir de NFILE lib...
	//		Vu la structure particuli�re de projet on a:
	//					pour chaque Workbench de m_workBenchArray on doit �crire:
	//									1 les params du workbench
	//									1 pathbuilder ( ses params et datas )
	//									1 trajectory  ( ses params uniquement )
	//					on n'�crit pas: ( car on pourra facilement les recalculer ... )
	//									le Path ( qui pourra �tre recalcul� � partir des params / datas de PathBuilder )
	//									les datas
	//									les States Packs
	//									les States SPacks
	//		1 pathbuilder et 1 trajectoire n'appartiennent qu'� un seul workbench
	//		du coup, pas besoin de faire un serializer complexe
	//		juste on parcourt la workbenchArray 3 fois de suite
	//					D'abord pour �crire tous les builders,
	//					puis pour �crire toutes les trajectoires,
	//					puis pour �crire tous les workbench ...

	// Enregistrement d'une Narray � la main...
	// .d'abord la taille !
	/*
	if (fwrite(&m_workbenchArray.Size, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	//. Ensuite on write chaque builder, 1 � 1
	NLPATH_WORKBENCH* pwb = (NLPATH_WORKBENCH*)m_workbenchArray.pFirst;
	for (i = 0; i < m_workbenchArray.Size; i++, pwb++)
	{
		if (pwb->m_pPathBuilder->write(pfile) != 1)
			return 0;
	}
	//. puis on write chaque trajectoires, 1 � 1
	pwb = (NLPATH_WORKBENCH*)m_workbenchArray.pFirst;
	for (i = 0; i < m_workbenchArray.Size; i++, pwb++)
	{
		if (pwb->m_pTrajectory->write(pfile,i) != 1) // on passe i comme pathindex car il y a exactement 1 path par trajectoire
			return 0;
	}

	//. puis on write chaque Workbench, 1 � 1
	pwb = (NLPATH_WORKBENCH*)m_workbenchArray.pFirst;
	for (i = 0; i < m_workbenchArray.Size; i++, pwb++)
	{
		if (pwb->write(pfile,0,i,i) != 1)
			return 0;
	}
	*/
