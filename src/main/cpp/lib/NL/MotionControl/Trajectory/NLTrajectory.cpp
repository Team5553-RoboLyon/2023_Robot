#include <new>
#include "lib/N/NMemory.h"
#include "lib/N/NMath.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/File/NDataPacker.h"
#include "lib/N/Utilities/NUT_Logging.h"

#include "lib/NL/NLProject.h"
#include "lib/NL/MotionControl/NLKin.h"
#include "lib/NL/MotionControl/NLPhysics.h"
#include "lib/NL/MotionControl/Path/Geometry/NLPathGeometry.h"

#include "lib/NL/MotionControl/Trajectory/NLKNode.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectory.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryKey.h"
#include "lib/NL/MotionControl/Trajectory/NLVstage.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageX.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageXSlice.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageArrayTools.h"

#include "lib/NL/NLLogsChannels.h"

#ifdef _NEDITOR
// #include "lib/N/NEditor.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"

#include "lib/NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#endif

// #define INTEGRATE_DT(time,dt,sens)		(((sens)==1) ? (time+=(dt)):(time-=(dt)))
// #define SIGN(val,src,sign)				(((sign)<0) ? ((val)=-(src)):(val)=(src))

/*
NLMPC* NLMPC::initialize(const Nf32 v0, const Nf32 v1, const Nf32 s)
{
	Nmem0(this, NLMPC);
	m_v0	= v0;
	m_v1	= v1;
	m_s		= s;

	return this;
}
*/

NLTRAJECTORY::NLTRAJECTORY() : m_pPath(NULL), m_specificKinLimits(DEFAULT_NLKINLIMITS_VELOCITY_MAX, DEFAULT_NLKINLIMITS_ACCELERATION_MAX, DEFAULT_NLKINLIMITS_JERK_MAX, FLAG_AUTO), m_flags(0)
{
	//	NErrorIf(m_specificKinLimits.m_v || m_specificKinLimits.m_a || m_specificKinLimits.m_j, NERROR_NON_NULL_VALUE); //NLKINLIMITS constructeur � du �tre appel� ...
	/*
		m_flags = 0;
		m_pPath = NULL;
		m_dt	= 0.0f;
	*/
	NSetupArray(&m_userVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_keysArray, 0, sizeof(NLTRJKEY));

#ifdef _NEDITOR
	NSetupArray(&m_kinTArray, 0, sizeof(NLKIN));
	NSetupArray(&m_kinSArray, 0, sizeof(NLKIN));
	NSetupArray(&m_limitedpKeysArray, 0, sizeof(NLTRJKEY *)); // ATTENTION !!! Il s'agit d'un arrayde pointeurs !!!
	NSetupArray(&m_pathVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_limitedUserVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_combinedVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_vStageXSlicesArray, 0, sizeof(NLVSTAGEXSLICE));
#endif
}

NLTRAJECTORY::NLTRAJECTORY(NLPATH *ppath, const NLKINLIMITS *pklim = NULL, const NLKINTWEAK *pktwk = NULL, const Nu32 enforceflags = 0) : m_pPath(ppath), m_specificKinLimits(pklim, FLAG_AUTO), m_kinTweak(pktwk), m_flags(enforceflags & MASK_NTRJ_ENFORCE)
{

	if (pklim)
		FLAG_ON(m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS);

	NSetupArray(&m_userVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_keysArray, 0, sizeof(NLTRJKEY));

#ifdef _NEDITOR
	NSetupArray(&m_kinTArray, 0, sizeof(NLKIN));
	NSetupArray(&m_kinSArray, 0, sizeof(NLKIN));
	NSetupArray(&m_limitedpKeysArray, 0, sizeof(NLTRJKEY *)); // ATTENTION !!! Il s'agit d'un arrayde pointeurs !!!
	NSetupArray(&m_pathVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_limitedUserVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_combinedVStagesArray, 0, sizeof(NLVSTAGE));
	NSetupArray(&m_vStageXSlicesArray, 0, sizeof(NLVSTAGEXSLICE));
#endif
}

NLTRAJECTORY::~NLTRAJECTORY()
{
	NErrorIf(m_pPath, NERROR_NON_NULL_POINTER);

	NClearArray(&m_userVStagesArray, NLclearVStageInArrayCallBack);
	NClearArray(&m_keysArray, NLclearTrjKeyInArrayCallBack);

#ifdef _NEDITOR
	NClearArray(&m_kinTArray, NLclearKinInArrayCallBack);
	NClearArray(&m_kinSArray, NLclearKinInArrayCallBack);
	NClearArray(&m_vStageXSlicesArray, NLclearVStageXSliceInArrayCallBack);
	NClearArray(&m_combinedVStagesArray, NLclearVStageInArrayCallBack);
	NClearArray(&m_limitedUserVStagesArray, NLclearVStageInArrayCallBack);
	NClearArray(&m_pathVStagesArray, NLclearVStageInArrayCallBack);
	NClearArray(&m_limitedpKeysArray, NULL); // ici on ne fait rien du tout. Il s'agit d'un array de travail (regroupement). Les pointeurs stock�s pointent vers des keys cr��es par l'utilisateur.
											 // NClearArray(&m_knodeArray, NULL);
#endif
}

Nu32 NLTRAJECTORY::insertUserVelocityStage(NLVSTAGE *pnewvs)
{
	NErrorIf(!pnewvs, NERROR_NULL_POINTER);

	if (!m_userVStagesArray.Size)
	{
		NArrayPushBack(&m_userVStagesArray, (NBYTE *)pnewvs);
	}
	else
	{
		//	ARRAY temporaire des VSTAGE users
		NARRAY tmp_out_array;
		Nu32 i;
		Nu32 _brk = 0;
		NSetupArray(&tmp_out_array, m_userVStagesArray.Size + 1, sizeof(NLVSTAGE));

		pnewvs->resetCutters();
		NLVSTAGE *pvstage = (NLVSTAGE *)m_userVStagesArray.pFirst;
		for (i = 0; i < m_userVStagesArray.Size; i++, pvstage++)
		{
			// La fonction cutOut est configur�e pour:
			//			1/ Forcer la d�coupe systematique que pnewvs possede une vitesse plus �lev�e ou plus basse que celle de pvstage ...
			//			2/ Ne pusher sur "trimed_user_vs_array" que les parties � gauche ou � droite du cookiecutter
			//			notes:	Dans la fonction 'buildVelocityStages' on indique de retourner 'FLAG_CUTOUT_RIGHT' par d�faut en cas de d�coupe non-initi�e.
			//					Il serait inutile de vouloir faire la m�me chose ici car la d�coupe est forc�e et sera donc TOUJOURS INITIEE !
			//					C'est pourquoi la valeur 'nocut_default_result' de MAKECFG est � 0.
			if (ISFLAG_ON(pvstage->cutOut(&tmp_out_array, pnewvs, MAKE_CUTOUT_CFG(0, FLAG_CUTOUT_FORCE | FLAG_CUTOUT_LEFT | FLAG_CUTOUT_RIGHT)), FLAG_CUTOUT_RIGHT))
			{
				// Tout ou partie de pvstage_desc a �t� pouss� sur "tmp_out_array" en �tant � droite de pnewdesc ...
				// tous les pvstage_desc suivants ne pouvant �tre qu'encore plus � droite, on sort !
				_brk = 1;
				break;
			}
		}

		if (_brk)
		{
			// On ins�re pnewvs juste avant le dernier vstage ins�r� ( et qui �tait donc � la de pnewvs )
			NInsertArrayElement(&tmp_out_array, tmp_out_array.Size - 1, (NBYTE *)pnewvs);

			// Reste-t'il des NLVSTAGE � ins�rer ? ( car peut-�tre que le 'break' (_brk = 1 ) c'est produit sur le dernier Vstage ...
			if (i < (m_userVStagesArray.Size - 1))
			{
				// Cela signifie qu'on est sorti de la boucle "for" via un break sans attendre la condition de sortie 'officielle' c�d : (i == m_userVStagesArray.Size)
				// le i�me vstage de l'array 'm_userVStagesArray' a bien �t� trim�/ins�r� mais la boucle a �t� interrompue juste apr�s.
				// et on termine l'insertion de tous les suivants de mani�re plus "v�loce" ...
				NArrayBufferPushBack(&tmp_out_array, pvstage + 1, m_userVStagesArray.Size - 1 - i);
			}
		}
		else
		{
			// On ins�re pnewvs � la fin ... tout simplement
			// En effet, si _brk == 0 alors cela signifie que tous les Vstages ont �t� "pouss�s"  sur 'tmp_out_array' sans qu'aucun ne "d�passe" � droite de pnewvs ...
			NArrayPushBack(&tmp_out_array, (NBYTE *)pnewvs);
		}

		// pour finir on swap les contenus ( pour �viter une copie ;) )
		NSwapArrayContent(&m_userVStagesArray, &tmp_out_array);
		NClearArray(&tmp_out_array, NULL);
	}

	return m_userVStagesArray.Size;
}

NLTRJKEY *NLTRAJECTORY::insertUserKey(const Nf32 s, const Nu32 insert_before, const Nu32 keyflags)
{
	// NLTRJKEY key(s, keyflags);
	// return (NLTRJKEY*)NInsertArrayElement(&m_keysArray, insert_before, (NBYTE*)&key);
	return ((NLTRJKEY *)NArrayAllocInPlace(&m_keysArray, insert_before))->setup(s, keyflags);
}
/*
NLTRJKEY* NLTRAJECTORY::insertUserKeyStop(const Nf32 s, const Nu32 insert_before)
{
	NLTRJKEY key(s, FLAG_NLTRJKEY_STOP);
	return (NLTRJKEY*)NInsertArrayElement(&m_keysArray, insert_before, (NBYTE*)&key);
}
*/
NLTRJKEY *NLTRAJECTORY::addUserKey(const Nf32 s, const Nu32 keyflags)
{
	// Emplace setup
	return ((NLTRJKEY *)NArrayAllocBack(&m_keysArray))->setup(s, keyflags);

	// return new(NArrayAllocBack(&m_keysArray)) NLTRJKEY(s, keyflags);
}
/*
NLTRJKEY* NLTRAJECTORY::addUserKeyStop(const Nf32 s)
{
	// Emplace setup
	return new(NArrayAllocBack(&m_keysArray)) NLTRJKEY(s, FLAG_NLTRJKEY_STOP);
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule les valeurs de KIN depuis le KIN pk0 en appliquant 'jerk' sur une duree "dt"
 *
 *	@param	pdtspecs est le descriptif du DriveTrain cens� se d�placer sur le chemin.
 *  @param	ppack est un pointeur sur une classe NLTRAJECTORY_PACK qui par d�faut est null.
 *			Si il est non null, alors un pack externe de donn�es exportables est cr�� en m�me temps
 *			que la trajectoire. Ce pack contient toutes les donn�es n�c�ssaires au suivi de trajectoire par un robot ( hors specs robot )
 */
// ------------------------------------------------------------------------------------------

void NLTRAJECTORY::build(const NLDRIVETRAINSPECS *pdtspecs)
{
	NErrorIf(!m_pPath, NERROR_NULL_POINTER);
	NErrorIf(!pdtspecs, NERROR_NULL_POINTER);

	// ***************************************************************
	// *
	// *
	// * DEFINITION DES LIMITES LOCALES
	// *
	// *
	// ***************************************************************
	/*
	 * L'utilisateur peut specifier des limites diff�rentes par trajectoire.
	 * Le kinlimit final utilis� est construit � partir des limites r�elles du robot, "pdtsspecs->m_limits" et des potentielles limites sp�cifiques fix�es par l'utilisateur...
	 * ... ET int�gre bien sur la vitesse de croisi�re max !
	 */
	NLKINLIMITS local_limits;

	NErrorIf(!pdtspecs->m_limits.m_v, NERROR_NULL_VALUE);
	NErrorIf(!pdtspecs->m_limits.m_a, NERROR_NULL_VALUE);
	NErrorIf(!pdtspecs->m_limits.m_j, NERROR_NULL_VALUE);

	if (ISFLAG_ON(m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS))
	{
		NErrorIf(!m_specificKinLimits.m_v, NERROR_NULL_VALUE);
		NErrorIf(!m_specificKinLimits.m_a, NERROR_NULL_VALUE);
		NErrorIf(!m_specificKinLimits.m_j, NERROR_NULL_VALUE);

		local_limits.m_v = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_VEL) ? m_specificKinLimits.m_v : NMIN(pdtspecs->m_limits.m_v, m_specificKinLimits.m_v);
		local_limits.m_a = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_ACC) ? m_specificKinLimits.m_a : NMIN(pdtspecs->m_limits.m_a, m_specificKinLimits.m_a);
		local_limits.m_j = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_JRK) ? m_specificKinLimits.m_j : NMIN(pdtspecs->m_limits.m_j, m_specificKinLimits.m_j);
		local_limits.m_v = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_VEL) ? m_specificKinLimits.m_v : NMIN(pdtspecs->m_limits.m_v, m_specificKinLimits.m_v);

		local_limits.build(FLAG_FALLBACK_MIN);
		// FLAG_FALLBACK_MIN ... pour la forme, car en fait m_specificKinLimits et pdtspecs->m_limits ont normalement �t� construit avec .build(xx)
		// et en cons�quence ne contienent aucune valeur nulle. d'ailleurs, en mode debug, une batterie de NErrorIf v�rifie la "non-nullit�" de tous les
		// membres de ces deux structures kinlimits.
	}
	else
	{
		// NErrorIf((m_flags&MASK_NTRJ_ENFORCE), NERROR_INCONSISTENT_FLAGS); // Not an error but it seems awkward to set these flags without  FLAG_NLTRJ_SPECIFIC_KINLIMITS
		local_limits = pdtspecs->m_limits;
	}

	// Il est possible que la methode "calculateMaxCruiseVelocity" renvoie ZERO, dans le cas o� le chemin est trop court par rapport � la vitesse Max possible pass�e en entr�e.
	// Cela arrive quand le chemin est vraiment tr�s court et �galement quand la vitesse Max possible est tr�s/trop petite...
	// Du coup comme la vitesse de croisiere ne saurait �tre nulle, si elle est nulle on sort sans rien faire.
	Nf32 v = m_pPath->calculateMaxCruiseVelocity(&local_limits);
	if (!v)
		return;
	else
		local_limits.setVelocityMax(v); // set vmax as maxcruisevelocity

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|
	//	LES ETAPES DE CONSTRUCTION DE L'ARRAY
	//
	//	En entr�e nous aurons 2 ARRAYS diff�rentes:	[A.0] m_userVStagesArray			soit l'array des Vstages Desc d�fini manuellement par l'utilisateur le long du chemin.
	//												[B.0] pvstages_prime_array		soit l'array des VStages Desc construit par l'algo. en fonction de la g�ometrie du chemin
	//	|
	//	ETAPE 1:
	//	Contraindre [A.0] aux limites du chemin :	[A.0] >>> [A.1]	pvstagesdesc_limited_array
	//																! Ne contient les Vstages points START et END correspondant au point de d�part et d'arriv�e du robot.
	//	|
	//	ETAPE 2:
	//	Construction de [B.0].  Attention, [B.0] ne contient pas les Vstages points START et END correspondant au point de d�part et d'arriv�e du robot.
	//
	//	|
	//	ETAPE 3:
	//	Fusion SMART de [A.1] et [B.0]:				[A.1] + [B.0] >>> [C.0]
	//
	//
	//	ETAPE 1:
	//		Contraindre les vstagesdesc ( vstages utilisateur ) � la taille du path
	//		L'�dition des vstagesdesv et du chemin �tant distinctes l'utilisateur a pu cr�er un chemin de longeur  L > l et placer dessus des vstagesdesc et ensuite �diter le chemin
	//		pour ramener sa longueur � l ( < L). Du coup, les vstages desc initialement cr��s peuvent, pour certains, se retrouver hors des nouvelles limites du chemin  ( plus loin que la fin ).
	//		La premi�re chose � faire est donc de s'assurer que chemin ( et computed vstages ) ET vstagedesc couvrent tous le m�me intervalle [0,longueur actuelle du chemin]
	//

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	A)	PREPARATION DES ARRAYS
	// 	|
	NARRAY *ppath_vstages_array;		 // array des vstages calcul�s � partir du chemin
	NARRAY *plimited_user_vstages_array; // array des vstages ajust�s � la longueur courante du chemin
	NARRAY *pcombined_vstages_array;	 // combinaison des vstages path et user
	NARRAY *pvstagex_slices_array;		 // array de vstageX ( �tendus ) n�c�ssaires � la recherche de la solution optimum
	NARRAY *plimited_pkeys_array;		 // array des (pointeurs sur) trajectory keys ajust�es � la longueur courante du chemin.
	NARRAY *pkint_array;				 // Array de promotion des Kin T ( T pour Traveling ... le long du chemin )
	NARRAY *pkins_array;				 // Array de promotion des Kin S ( S pour Spot - sur place )

#ifdef _NEDITOR
	NResizeArray(&m_kinTArray, 0, NULL, NULL);
	NResizeArray(&m_kinSArray, 0, NULL, NULL);
	NResizeArray(&m_pathVStagesArray, 0, NULL, NLclearVStageInArrayCallBack);
	NResizeArray(&m_limitedUserVStagesArray, 0, NULL, NLclearVStageInArrayCallBack);
	NResizeArray(&m_combinedVStagesArray, 0, NULL, NLclearVStageInArrayCallBack);
	NResizeArray(&m_vStageXSlicesArray, 0, NULL, NLclearVStageXSliceInArrayCallBack);
	NResizeArray(&m_limitedpKeysArray, 0, NULL, NULL /*NLclearTrjKeyInArrayCallBack*/); // On ne clear pas les KEYS de cet array car elles pointent sur les m�mes actions arrays que les keys de 'm_keysArray' !!!

	ppath_vstages_array = &m_pathVStagesArray;
	plimited_user_vstages_array = &m_limitedUserVStagesArray;
	pcombined_vstages_array = &m_combinedVStagesArray;
	pvstagex_slices_array = &m_vStageXSlicesArray;
	plimited_pkeys_array = &m_limitedpKeysArray;
	pkint_array = &m_kinTArray;
	pkins_array = &m_kinSArray;
#endif

#ifndef _NEDITOR
	NARRAY path_vstages_array;
	NARRAY limited_user_vstages_array;
	NARRAY combined_vstages_array;
	NARRAY vstagex_slices_array;
	NARRAY limited_pkeys_array;
	NARRAY kint_array;
	NARRAY kins_array;

	NSetupArray(&path_vstages_array, 0, sizeof(NLVSTAGE));
	NSetupArray(&limited_user_vstages_array, 0, sizeof(NLVSTAGE));
	NSetupArray(&combined_vstages_array, 0, sizeof(NLVSTAGE));
	NSetupArray(&vstagex_slices_array, 0, sizeof(NLVSTAGEXSLICE));
	NSetupArray(&limited_pkeys_array, 0, sizeof(NLTRJKEY *));
	NSetupArray(&kint_array, 0, sizeof(NLKIN));
	NSetupArray(&kins_array, 0, sizeof(NLKIN));

	ppath_vstages_array = &path_vstages_array;
	plimited_user_vstages_array = &limited_user_vstages_array;
	pcombined_vstages_array = &combined_vstages_array;
	pvstagex_slices_array = &vstagex_slices_array;
	plimited_pkeys_array = &limited_pkeys_array;
	pkint_array = &kint_array;
	pkins_array = &kins_array;
#endif
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	B)	CONSTRUCTION DES VSTAGES ISSUS DU PATH & AJUSTEMENT DE L'INTERVALLE DES VSTAGES UTILISATEUR
	// 	|
	// NResizeArray(ppath_vstages_array, 0, NULL, NULL);
	m_pPath->buildVStages(ppath_vstages_array, pdtspecs, &local_limits);
	NLVstageArrayTools::slice(plimited_user_vstages_array, &m_userVStagesArray, 0.0f, m_pPath->m_geometry.m_ds, NFALSE);
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Nu32 i, j;
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	C)	FUSION DES VSTAGES ISSUS DU PATH ET DES VSTAGES UTILISATEUR
	// 	|
	if (plimited_user_vstages_array->Size)
	{
		NLVSTAGE *pvs = (NLVSTAGE *)plimited_user_vstages_array->pFirst;
		for (i = 0; i < plimited_user_vstages_array->Size; i++, pvs++)
		{
			pvs->computeAdjustedVelocity(local_limits.m_v);
		}
		NLVstageArrayTools::combine(pcombined_vstages_array, plimited_user_vstages_array, ppath_vstages_array, COMBINE_RESET_CUTTERS, COMBINE_CHAINED_CUTTERS);
	}
	else
	{
		NCopyArray(pcombined_vstages_array, ppath_vstages_array);
	}
	NErrorIf((NGetLastArrayPtr(pcombined_vstages_array)) && ((NLVSTAGE *)NGetLastArrayPtr(pcombined_vstages_array))->m_s1 > m_pPath->m_geometry.m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale
																																												//	|
																																												//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	D)	AJUSTAGE DES TRJKEYs
	// 	|
	//	|		rappel: m_keysArray	contient, par d�faut, deux TRJKEY, celle de d�but et celle de fin de trajectoire.
	//	|				L'utilisateur a potentiellement ajout� manuellement un certain nbre de KEY additionnelles.
	//	|
	NErrorIf(m_keysArray.Size < 2, NERROR_ARRAY_WRONG_SIZE); // Il doit toujours y avoir au moins 2 clefs ! ( Start et End ! )
	NLPATH_POINT *ppp;
	NLTRJKEY *ptklast = NULL;
	NLTRJKEY *ptk = (NLTRJKEY *)m_keysArray.pFirst;
	NErrorIf(ISFLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_STOP), NERROR_INCONSISTENT_FLAGS); // la premiere clef doit TOUJOURS �tre un stop !
	NArrayPushBack(plimited_pkeys_array, (NBYTE *)&ptk);
	ptk++;
	// On ne prend pas en compte la premi�re et la derniere clef
	// qui b�nificient d'un traitement sp�cial
	for (i = 1; i < m_keysArray.Size - 1; i++, ptk++)
	{
		// Toutes les Keys dont l'abscisse actuelle est plus grande ou egale que la longueur du chemin est
		// consid�r�e comme OUT OF PATH et desactiv�e !
		// Note: pour �viter la supperposition avec la derni�re clef dont l'abscisse sera ramen�e � m_geometry.m_ds quelques lignes plus bas...
		//		 ... une key dont l'abscisse serait �gale � m_geometry.m_ds sera donc ici bien d�sactiv�e !
		if (ptk->m_s < m_pPath->m_geometry.m_ds)
		{
			FLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_OUTOFPATH);

			// On v�rifie que les keys "link�es" � un point de chemin peuvent toujours l'�tre.
			if (ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_LINKABLE))
			{
				// Une Key peut rest�e link�e � un point de chemin si l'abscisse de ce dernier reste compris dans l'intervalle:
				//
				//								(key-1).s <	point.s  < (key+1).s
				//
				// En effet cela garanti que l'attribution de l'abscissr de point.s � key ne modifiera l'ordre des keys donn� par:
				//
				//								k0.s < k1.s < k2.s < .. k(n-1).s < k(n).s
				//
				if (ppp = (NLPATH_POINT *)m_pPath->m_geometry.getPathPoint(ptk->m_linkedPointId))
				{
					if (((ptk - 1)->m_s >= ppp->s) || ((ptk + 1)->m_s <= ppp->s))
					{
						FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_BROKEN_LINK);
						continue;
					}
					FLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_BROKEN_LINK);
				}
				else
				{
					FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_BROKEN_LINK);
					continue;
				}
				// Si on passe ici c'est que Key est Link�e � point dont l'abscisse actuelle permet de continuer � respecter l'ordre des keys.
				// On peut donc affecter cette abscisse � Key
				ptk->m_s = ppp->s;
			}

			// on push un pointeur dans l'array des pointeurs sur key qui ne sont pas out of path !
			NArrayPushBack(plimited_pkeys_array, (NBYTE *)&ptk);
		}
		else // ptk->m_s >= m_pPath->m_geometry.m_ds
		{
			//	 ... une key dont l'abscisse est sup�rieure OU EGALE � m_geometry.m_ds est d�sactiv�e !
			FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_OUTOFPATH);
		}
	}
	// La derni�re Key prend syst�matiquement l'abscisse de fin de PATH !
	NErrorIf(ptk != (NLTRJKEY *)NGetLastArrayPtr(&m_keysArray), NERROR_WRONG_ADDRESS);
	NErrorIf(ISFLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_STOP), NERROR_INCONSISTENT_FLAGS); // la derniere key doit TOUJOURS �tre un stop !
	NErrorIf(ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_OUTOFPATH), NERROR_INCONSISTENT_FLAGS);
	ptk->m_s = m_pPath->m_geometry.m_ds;
	NArrayPushBack(plimited_pkeys_array, (NBYTE *)&ptk);
//	|
//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	D)	AJUSTAGE DES TRJKEYs ( suite )
// 	|
//	|		Les TRJKEYs peuvent �tre des STOPs ( ou pas ).
//	|		La distance mini entre deux TRJKEYs STOP est �gale � la longueur minimum du chemin ( calcul�e justement pour permettre de d�marrer et finir arr�t� en passant
//	|		par l'acceleration max sans la maintenir ).
//	|		Une TRJKEY STOP qui ne respecte pas ( ou plus ) cette distance mini. est d�sactiv�e, mais reste pr�sente dans les listes.
//	|
//	|		Note:	La fonction "submit" utilis�e pour cr�er/d�placer les keys veille �galement � conserver une dist. mini entre chaque key. Mais
//  |				il ne s'agit pas de la m�me chose. Pour submit cette dist. mini est arbitraire et tjrs la m�me; elle permet juste de garantir
//	|				la non superposition de 2 keys.
//	|				Ici, la distance de s�paration mini entre 2 KEYS STOP (uniquement les key STOP ) est choisie de mani�re non arbitraire
//	|				en phase avec les param�tres kinetics choisis ( accel. max )
//	|
#ifdef _DEBUG
	// On v�rifie en premier lieu que le chemin lui m�me est assez long !
	ptk = (*(NLTRJKEY **)NGetFirstArrayPtr(plimited_pkeys_array));
	ptklast = (*(NLTRJKEY **)NGetLastArrayPtr(plimited_pkeys_array));
	NErrorIf(ptklast->m_s - ptk->m_s < local_limits.m_2ss, NERROR_SYSTEM_GURU_MEDITATION);
#endif
	//  Chasse au trop pr�s:
	//							1o0 signifie mon voisin de gauche est trop pr�s de moi
	//							0o1 signifie mon voisin de droite est trop pr�s de moi
	//							1o1 signifie mes voisins de droite et de gauche sont trop pr�s de moi
	//	Chasse au 1o1 ( trop pr�s � gauche et trop pr�s � droite ... )
	NLTRJKEY *ptk0, *ptk1;

	NLTRJKEY **pptk = (NLTRJKEY **)plimited_pkeys_array->pFirst + 1;
	for (i = 1; i < plimited_pkeys_array->Size - 1; i++, pptk++)
	{
		ptk = *pptk;
		ptk0 = *(pptk - 1);
		ptk1 = *(pptk + 1);

		if (ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_STOP) && ISFLAG_ON(ptk0->m_flags, FLAG_NLTRJKEY_STOP) && ISFLAG_ON(ptk1->m_flags, FLAG_NLTRJKEY_STOP))
		{
			if (((ptk->m_s - ptk0->m_s) < local_limits.m_2ss) && (ptk1->m_s - ptk->m_s < local_limits.m_2ss))
			{
				FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE);
			}
		}
	}

	//	Chasse au 1o0 ( trop pr�s � gauche )
	pptk = (NLTRJKEY **)plimited_pkeys_array->pFirst + 1;
	for (i = 1; i < plimited_pkeys_array->Size - 1; i++, pptk++)
	{
		ptk = *pptk;
		ptk0 = *(pptk - 1);

		if (ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_STOP))
		{
			if (ISFLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE) && ((ptk->m_s - ptk0->m_s) < local_limits.m_2ss))
			{
				FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE);
			}
		}
	}
	//	Chasse au 0o1 ( trop pr�s � droite ): 1 seul candidat possible, l'avant derniere key
	if (plimited_pkeys_array->Size > 2)
	{
		pptk = (NLTRJKEY **)NGetLastArrayPtr(plimited_pkeys_array) - 1;
		ptk = *pptk;
		ptk1 = *(pptk + 1);

		if (ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_STOP))
		{
			if (ISFLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE) && ((ptk1->m_s - ptk->m_s) < local_limits.m_2ss))
			{
				FLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE);
			}
		}
	}

	//  V�rifications en DEBUG
#ifdef _DEBUG
	pptk = (NLTRJKEY **)plimited_pkeys_array->pFirst + 1;
	for (i = 1; i < plimited_pkeys_array->Size - 1; i++, pptk++)
	{
		ptk = *pptk;
		ptk0 = *(pptk - 1);
		ptk1 = *(pptk + 1);
		// La key est active
		if (ISFLAG_OFF(ptk->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE))
		{
			// La key est un stop
			if (ISFLAG_ON(ptk->m_flags, FLAG_NLTRJKEY_STOP))
			{
				// Son voisin de gauche qui est un stop et est actif et trop pres !
				if ((ISFLAG_ON(ptk0->m_flags, FLAG_NLTRJKEY_STOP)) &&
					(ISFLAG_OFF(ptk0->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE)) &&
					((ptk->m_s - ptk0->m_s) < local_limits.m_2ss))
				{
					NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				}

				// Son voisin de droite qui est un stop et est actif et trop pres !
				if ((ISFLAG_ON(ptk1->m_flags, FLAG_NLTRJKEY_STOP)) &&
					(ISFLAG_OFF(ptk1->m_flags, FLAG_NLTRJKEY_DISABLE_TOO_CLOSE)) &&
					((ptk1->m_s - ptk->m_s) < local_limits.m_2ss))
				{
					NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				}
			}
		}
		// La key est inactive
		else
		{
			// Ses voisins de gauche et de droite ne sont pas trop pr�s !
			if (((ptk->m_s - ptk0->m_s) >= local_limits.m_2ss) && (ptk1->m_s - ptk->m_s >= local_limits.m_2ss))
			{
				NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
			}
		}
	}
#endif
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	E)	'SLICING' DES VSTAGES FUSIONNES EN ARRAY DE 'VSTAGEX'
	// 	|		!!! Ce slicing est effectu� avec les KEY STOP uniquement !!!
	//	|
	NErrorIf(plimited_pkeys_array->Size < 2, NERROR_INCONSISTENT_FLAGS); // il doit y avoir au minimum 2 clefs ( d�but / fin )
	NLVSTAGEXSLICE *pvsxslice;
	pptk = (NLTRJKEY **)plimited_pkeys_array->pFirst;

	ptk0 = *pptk;
	ptk1 = *(pptk + 1);
	pptk++;

	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(ptk0->m_flags), NERROR_INCONSISTENT_FLAGS); // la premiere clef doit TOUJOURS �tre un stop actif !
	for (i = 1; i < plimited_pkeys_array->Size; i++, pptk++, ptk1 = *pptk)
	{
		if (IS_NLTRJKEY_ENABLE_STOP(ptk1->m_flags))
		{
			pvsxslice = (NLVSTAGEXSLICE *)new (NArrayAllocBack0(pvstagex_slices_array)) NLVSTAGEXSLICE; // Placement new
			pvsxslice->prepare(pcombined_vstages_array, &local_limits, &m_kinTweak, ptk0, ptk1);
			ptk0 = ptk1; // ptk0 devient ptk1 SSI un prepare � eut lieu !!! ...
						 // ... C'est � dire SSI ptk1 est une Key STOP Active ( = non disable )!
		}
	}
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	F)	SOLVE and PROMOTE
	//  |		A l'issue de ce processus, TOUS les kins seront cr��s.
	//  |		Des kins sont cr��s:
	//	|					pour parcourir la distance couverte par chaque Slice.
	//  |					pour effectuer les rotations sur place potentiellement pr�sentes dans chaque clef STOP.
	// 	|
	//  |		Les Kins cr��s par un Slice seront interpr�t�s comme ceux du centre d'inertie du robot, les valeurs appliqu�es � chaque GearBox �tant d�duites
	//  |		'� la vol�e' par une application simple des formules d'Odom�trie.
	//	|
	//	|		Les Kins cr��s par une KEY STOP ( ... donc par une/des action(s) de rotation pr�sente(s) 'dans' cette KEY STOP ) seront interpr�t�s comme ceux de la GearBox
	//	|		situ�e � droite du Robot. Les valeurs appliqu�es � la GearBox de gauche �tant simplement "oppos�es".
	//	|
	//	|		Pour permettre au robot d'interpr�ter correctement les Kins, ceux ci sont regroup�s en 'CHUNK'.
	//	|					Les Chunks de type T devant �tre interpret�s comme ceux du centre d'inertie du robot ( Translation le long du chemin )...
	//	|					Ceux de type R comme ceux de la GearBox de droite ( Rotation sur place ).
	//	|
	NUT_EnableLoggingChannel(LOGS_CHANNEL_TRJBUILD, TRUE);
	NUT_Logging(LOGS_CHANNEL_TRJBUILD, "START KIN PROMOTION ++++++++++++++++++++++++++++\n");
	Nu32 t_first = 0;
	NLKIN *pkin = NULL;
	NLTRJPOINT_DESC_CFG cfg;
	m_pack.setup(m_pPath, pdtspecs, plimited_pkeys_array);
	pvsxslice = (NLVSTAGEXSLICE *)pvstagex_slices_array->pFirst;
	if (pvsxslice)
	{
		//	Insertion du premier KIN dans chacun des 2 arrays.
		//((NLKIN*)NArrayAllocBack(pkins_array))->null();		// array temporaire pour Kins "Spot"

		// On commence par promouvoir les kins potentielements issus d'une ou plusieurs Actions de rotation pr�sentes dans la KEY 0.
		// ( Il est possible qu'il n'y est aucune rotation dans KEY0 et donc aucune cr�ation de Kin )
		NErrorIf(!pvsxslice->m_pKey0, NERROR_NULL_POINTER);

		/*
		* Previous
		current_cfg = MAKE_NLPROMOTE_KEY_ACTION_RESULTS(	0,			// Index de l'action � partir de laquelle NLTRJKEY::promote devra commencer � boucler ( on initialise donc � 0 !)
															TRUE,		// TRUE pour signifier ici "continue" ... � parcourir la liste des action. ( on initialise, donc le sens ici est plut�t "d�marre" ! )
															FALSE,		//
															0,			// |_ FALSE pour indiquer qu'il n'y a pas de KIN ins�r� dans l'array des kins, et donc pas de NLTRJPOINTDESC � cr�er. ( pas encore )
															FALSE,		// |  et 0 comme valeur par defaut pour l'index du premier KIN qui de toute facon ici ne sera pas consid�r� comme valide ( � cause de FALSE )
			FLAG_NLTRJACTION_KTYPE_SPOT| FLAG_NLTRJACTION_SET_DT_MODE);	// Configuration du contexte initial.
																		// Nous sommes � l'arr�t sur un point fixe donc KTYPE est SPOT
																		// et par d�faut DTMODE est forward ! TODO: peut-�tre permettre � user de definir le DTMODE initial ?!

		while (IS_NLPROMOTE_KEY_ACTION_RESULTS_CONTINUE(current_cfg))
		{
			next_cfg = pvsxslice->m_pKey0->promote( pkins_array, current_cfg, psmotionprofiles_array );
			if (IS_NLPROMOTE_KEY_ACTION_RESULTS_CREATE_KIN(next_cfg))
				m_pack.buildSpotChunk(current_cfg, pvsxslice->m_pKey0->m_s, pkins_array, GET_NLPROMOTEACTION_RESULTS_FIRST_KIN_IDX(next_cfg) );

			current_cfg = next_cfg;
		}
		*/
		NErrorIf(!pvsxslice->m_pKey0, NERROR_INCONSISTENT_VALUES);
		Nf32 t = 0.0f;
		pvsxslice->m_pKey0->preCompute(t, &m_pack.m_driveTrainSpecifications);
		cfg = MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, DEFAULT_KTYPE | DEFAULT_DTMODE);
		NLTRJACTION *pac = (NLTRJACTION *)pvsxslice->m_pKey0->m_actionsArray.pFirst;
		for (i = 0; i < pvsxslice->m_pKey0->m_actionsArray.Size; i++, pac++, t = m_pack.getLastTrjPtDescTime())
		{
			cfg = promoteAction(cfg, pac, t, pkins_array);
#ifdef _DEBUG
			if (IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(cfg))
			{
				NLKIN *dbg_pk = (NLKIN *)pkins_array->pFirst + GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg);
				for (Nu32 dbg_i = GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg); dbg_i < pkins_array->Size; dbg_i++, dbg_pk++)
				{
					NUT_Logging(LOGS_CHANNEL_TRJBUILD, "Slice[0].Key0::Promoted KinS[%d]\t\t t(%.4f),\ts(%.4f) ", dbg_i, dbg_pk->m_t, dbg_pk->m_s);
				}
			}
#endif
			cfg = m_pack.buildSpotChunk(cfg, pvsxslice->m_pKey0->m_s, pkins_array);
		}
		// m_timeStamp.end a d�j� �t� �valu� par NLTRJKEY::preCompute avec le precalcul des SmotionProfiles ...
		// ensuite la promotion des kins d'un Smotioprofile est cenx�e atterrir sur un dernier kin de date t �gale � la date du premier kin + SmotionProfile.m_dt
		NErrorIf(IS_NLTRJKEY_AUTODURATION(pvsxslice->m_pKey0->m_flags) && pvsxslice->m_pKey0->m_timeStamp.end != t, NERROR_INCONSISTENT_VALUES);
		// pvsxslice->m_pKey0->m_timeStamp.end = t;

		// Initialisation du premier Kin T ( et donc du premier NLTRJPOINT_DESC de KType Traveling. )
		/*
		NLKIN kin;
		kin.null();
		kin.m_t = t;
		NArrayPushBack(pkint_array, (NBYTE*)&kin);
		NErrorIf(pvsxslice->m_pKey0->m_s != 0.0f, NERROR_INCONSISTENT_VALUES);
		*/
		// TOUS LES VSTAGEXSLICE ... puis leur KEY1
		for (j = 0; j < pvstagex_slices_array->Size; j++, pvsxslice++, t = m_pack.getLastTrjPtDescTime())
		{
			// VSTAGEXSLICE
			if (pvsxslice->m_vStageXArray.Size)
			{
				pvsxslice->BuildSolutionTree();				   // 1) Analyse et construction de la solution "Kinetic"
				cfg = pvsxslice->Promote(cfg, t, pkint_array); // 2) Promotion de la solution en KIN ( dans l'array T )
#ifdef _DEBUG
				if (IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(cfg))
				{
					NLKIN *dbg_pk = (NLKIN *)pkint_array->pFirst + GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg);
					for (Nu32 dbg_i = GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg); dbg_i < pkint_array->Size; dbg_i++, dbg_pk++)
					{
						NUT_Logging(LOGS_CHANNEL_TRJBUILD, "Slice[%d]::Promoted KinT[%d]\t\t t(%.4f),\ts(%.4f) ", j, dbg_i, dbg_pk->m_t, dbg_pk->m_s);
					}
				}
#endif

				cfg = m_pack.buildTravelingChunk(cfg, pkint_array); // 3) Construction du chunk
			}

			// KEY1
			NErrorIf(!pvsxslice->m_pKey1, NERROR_NULL_POINTER);
			t = m_pack.getLastTrjPtDescTime();
			pvsxslice->m_pKey1->preCompute(t, &m_pack.m_driveTrainSpecifications);
			pac = (NLTRJACTION *)pvsxslice->m_pKey1->m_actionsArray.pFirst;
			for (i = 0; i < pvsxslice->m_pKey1->m_actionsArray.Size; i++, pac++, t = m_pack.getLastTrjPtDescTime())
			{
				cfg = promoteAction(cfg, pac, t, pkins_array);
#ifdef _DEBUG
				if (IS_NLTRJPOINT_DESC_CFG_CREATE_KIN(cfg))
				{
					NLKIN *dbg_pk = (NLKIN *)pkins_array->pFirst + GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg);
					for (Nu32 dbg_i = GET_NLTRJPOINT_DESC_CFG_FIRST_KIN_IDX(cfg); dbg_i < pkins_array->Size; dbg_i++, dbg_pk++)
					{
						NUT_Logging(LOGS_CHANNEL_TRJBUILD, "Slice[%d].Key1::Promoted KinS[%d]\t\t t(%.4f),\ts(%.4f) ", j, dbg_i, dbg_pk->m_t, dbg_pk->m_s);
					}
				}
#endif

				cfg = m_pack.buildSpotChunk(cfg, pvsxslice->m_pKey1->m_s, pkins_array);
			}
		}

		NLTRJPOINT_DESC *ppdsc = (NLTRJPOINT_DESC *)NGetLastArrayPtr(&m_pack.m_trajectoryPointDescArray);
		NErrorIf(!ppdsc, NERROR_NULL_POINTER);
		if (ppdsc->m_pPrimitive)
			m_pack.m_ds = ppdsc->m_kin.m_s;
		else
			m_pack.m_ds = ppdsc->m_pPathPoint1->s;

		m_pack.m_dt = ppdsc->m_kin.m_t;
		NErrorIf(m_pack.m_dt != t, NERROR_INCONSISTENT_VALUES);
	}
	NUT_Logging(LOGS_CHANNEL_TRJBUILD, "\nEND KIN PROMOTION ++++++++++++++++++++++++++++\n");
#ifdef _NUT_LOGGING
	NUT_Logging(LOGS_CHANNEL_TRJBUILD, "+ START ---- Check PACK DATA after creation  +\n");
	NUT_Logging(LOGS_CHANNEL_TRJBUILD, "m_pack.m_trajectoryPointDescArray.Size = %d  +\n", m_pack.m_trajectoryPointDescArray.Size);

	NLTRJPOINT_DESC *psrc = (NLTRJPOINT_DESC *)m_pack.m_trajectoryPointDescArray.pFirst;
	for (Nu32 i = 0; i < m_pack.m_trajectoryPointDescArray.Size; i++, psrc++)
	{
		if (ISFLAG_ON(psrc->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
			NUT_Logging(LOGS_CHANNEL_TRJBUILD, "SPOT-NLTRJPOINT_DESC[%d]\tm_pPathPoint1[%X]\tm_pPrimitive[%X] \n", i, psrc->m_pPathPoint1, psrc->m_pPrimitive);
		else
			NUT_Logging(LOGS_CHANNEL_TRJBUILD, "TRVL-NLTRJPOINT_DESC[%d]\tm_pPathPoint1[%X]\tm_pPrimitive[%X] \n", i, psrc->m_pPathPoint1, psrc->m_pPrimitive);
	}
	NUT_Logging(LOGS_CHANNEL_TRJBUILD, "+ END ---- Check PACK DATA after creation  +\n");
#endif

	/*
	pvsxslice = (NLVSTAGEXSLICE*)pvstagex_slices_array->pFirst;
	if (pvsxslice)
	{
		//	Insertion du premier KIN dans chacun des 2 arrays.
		((NLKIN*)NArrayAllocBack(pkint_array))->null();		// array temporaire pour Kins de chunk T
		((NLKIN*)NArrayAllocBack(pkinr_array))->null();		// array temporaire pour Kins de chunk R
		Nu32 t_first = 0;
		Nu32 r_first = 0;

		// KEY 0
		// On commence par promouvoir les kins potentielements issus d'une ou plusieurs Actions de rotation pr�sentes dans la KEY 0.
		// ( Il est possible qu'il n'y est aucune rotation dans KEY0 et donc aucune cr�ation de Kin )
		NErrorIf(!pvsxslice->m_pKey0, NERROR_NULL_POINTER);
		//pvsxslice->m_pKey0->promote(pkinr_array, pdtspecs);
		Nu32 main_params = pvsxslice->m_pKey0->getInitialPromoteMainParams();
		Nu32 action_id = 0;
		while ( pvsxslice->m_pKey0->promote(pkinr_array , &main_params, psmotionprofiles_array, action_id, pdtspecs) != NVOID)
		{
			if (pkinr_array->Size - 1 > r_first)
			{
				// De nouveaux KINS sont pr�sents dans l'array 'R' ce qui siginifie qu'au moins une action de rotation  � engendr� des Kins.
				// On cr��e en cons�quence un Chunk R � partir de ces Kins.
				m_pack.AllocChunkR(pkinr_array, r_first, pkinr_array->Size - 1, pvsxslice->m_pKey0->m_s);

				// Pour assurer une coh�rence 'temporelle' entre les chunk R et chunk T on copie le dernier KinR cr�� dans l'array des KIN T.
				// On ne conservera que la date et le Jerk du KinT ainsi cr�� � partir du dernier Kin R .
				// ... et bien sur on conserve l'abscisse de la key0 ( l'abscisse du dernier Kin R repr�sentant la distance parcourue par la roue droite lors de la/des rotations effectu�e(s) )
				// ... et on set vitesse et acceleration � 0 ( ce qui doit �tre le cas, car en sortie de chunk R on doit avoir une vitesse et une acceleration nulles ... mais, les kins �tant calcul�s ... on sera tjrs "presque � ZERO" mais pas "pile poil" � ZERO.
				pkin = (NLKIN*)NArrayPushBack(pkint_array, NGetLastArrayPtr(pkinr_array));
				pkin->m_s = pvsxslice->m_pKey0->m_s;

				NErrorIf(pkin->m_v != 0.0f, NERROR_NON_NULL_VALUE);	// juste pour check
				NErrorIf(pkin->m_a != 0.0f, NERROR_NON_NULL_VALUE);	// juste pour check
				pkin->m_v = 0.0f;
				pkin->m_a = 0.0f;
				//pkin->m_j = ;		... on conserve la valeur de J issu du KIN R
				//pkin->m_t = ;		... on conserve la valeur de t issu du KIN R ( TRES important ! Car du temps c'est �coul� entre le d�but et la fin du chunk R ! )
				//t_first = pkint_array->Size - 1;
			}
		}

		// TOUS LES VSTAGEXSLICE ... puis leur KEY1
		for (i = 0; i < pvstagex_slices_array->Size; i++, pvsxslice++)
		{
			// VSTAGEXSLICE
			if (pvsxslice->m_vStageXArray.Size)
			{
				pvsxslice->BuildSolutionTree();												// 1) Analyse et construction de la solution "Kinetic"
				t_first = pkint_array->Size - 1;
				pvsxslice->Promote(pkint_array);											// 2) Promotion de la solution en KIN ( dans l'array T )
				NErrorIf(pkint_array->Size - 1 <= t_first, NERROR_INCONSISTENT_REQUEST);
				m_pack.AllocChunkT(pkint_array, t_first, pkint_array->Size - 1);			// 3) Construction du chunk

				pkin = (NLKIN*)NArrayPushBack(pkinr_array, NGetLastArrayPtr(pkint_array));	// 4) Copie du dernier Kin T dans l'array Kint R, ainsi l'intervalle de temps
				pkin->m_s = 0.0f;															//	  du chunk T ins�r� se retrouve exprim� entre le dernier chunk R et le prochain.
				pkin->m_v = 0.0f;															//	  chuntR->t1 =	chunkT->t0
				pkin->m_a = 0.0f;															//					chunkT->T1 + chunkR+1->t0
				//pkin->m_j = ;																//	  ...
				//pkin->m_t = ;
			}

			// KEY1
			NErrorIf(!pvsxslice->m_pKey1, NERROR_NULL_POINTER);
			r_first = pkinr_array->Size - 1;

			action_id = 0;
			while ((action_id = pvsxslice->m_pKey1->promote(pkinr_array,  &main_params, psmotionprofiles_array,action_id, pdtspecs))!=NVOID)
			{
				if (pkinr_array->Size - 1 > r_first)
				{
					m_pack.AllocChunkR(pkinr_array, r_first, pkinr_array->Size - 1, pvsxslice->m_pKey1->m_s);
					pkin = (NLKIN*)NArrayPushBack(pkint_array, NGetLastArrayPtr(pkinr_array));
					pkin->m_s = pvsxslice->m_pKey1->m_s;
					pkin->m_v = 0.0f;
					pkin->m_a = 0.0f;
					//pkin->m_j = ;
					//pkin->m_t = ;
				}
			}
		}
	}
	*/
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	G)	DEFINITION DES "INTERVALLES DE DATE" couverts par chaque KEY
	// 	|		Toutes les KEYS sont trait�es, les KEY STOP et les autres.
	/*
	ptk0 = (NLTRJKEY*)m_keysArray.pFirst;
	for (i = 0; i < m_keysArray.Size; i++, ptk0++)
	{
		m_pack.getTime(&ptk0->m_timeStamp, ptk0->m_s);
	}
	*/
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//!!! n'est plus valide !!!
	/*
	ptk0 = (NLTRJKEY*)m_keysArray.pFirst;
	for (i = 0; i < m_keysArray.Size-1; i++, ptk0++)
	{
		ptk0->SetLifeSpan((ptk0 + 1)->m_timeStamp.start - ptk0->m_timeStamp.start);
	}
	*/

//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	H)	END
// 	|
// Pour finir: v�rification longueur et dur�e de la trajectoire
#ifdef _DEBUG
	Nf32 ts, tt;
	Nf32 ss, st;

	if (pkins_array->Size)
	{
		ts = ((NLKIN *)NGetLastArrayPtr(pkins_array))->m_t;
		ss = ((NLKIN *)NGetLastArrayPtr(pkins_array))->m_s;
	}
	else
	{
		ts = 0.0f;
		ss = 0.0f;
	}

	if (pkint_array->Size)
	{
		tt = ((NLKIN *)NGetLastArrayPtr(pkint_array))->m_t;
		st = ((NLKIN *)NGetLastArrayPtr(pkint_array))->m_s;
	}
	else
	{
		tt = 0.0f;
		st = 0.0f;
	}

	//	NErrorIf(NMAX(ts,tt)!=m_pack.m_dt,NERROR_INCONSISTENT_VALUES);
	//	NErrorIf(NMAX(ss, st) != m_pack.m_ds, NERROR_INCONSISTENT_VALUES);

#endif

#ifndef _NEDITOR
	NClearArray(&path_vstages_array, NULL);
	NClearArray(&limited_user_vstages_array, NULL);
	NClearArray(&merged_vstages_array, NULL);
	NClearArray(&vstagex_slices_array, NLclearVStageXSliceInArrayCallBack);
	NClearArray(&limited_keys_array, /*NLclearTrjKeyInArrayCallBack*/, NULL); // !!! On ne clear pas les KEYS de cet array car elles pointent sur les m�mes actions arrays que les keys de 'm_keysArray' !!!
	NClearArray(&kint_array, NLclearKinInArrayCallBack);
	NClearArray(&kins_array, NLclearKinInArrayCallBack);
#endif
}

void NLTRAJECTORY::setPath(NLPATH *ppath)
{

	if (ppath != m_pPath)
	{
		NEraseArray(&m_keysArray, NULL);		// Avoir si une option ne permettrait pas de conserver cet array ?
		NEraseArray(&m_userVStagesArray, NULL); // Avoir si une option ne permettrait pas de conserver cet array ?

#ifdef _NEDITOR
		NEraseArray(&m_kinTArray, NLclearKinInArrayCallBack);
		NEraseArray(&m_kinSArray, NLclearKinInArrayCallBack);

		NEraseArray(&m_pathVStagesArray, NULL);
		NEraseArray(&m_limitedUserVStagesArray, NULL);
		NEraseArray(&m_combinedVStagesArray, NULL);
		NEraseArray(&m_vStageXSlicesArray, NLclearVStageXSliceInArrayCallBack);
#endif

		// Insertion des clefs de trzjectoire ( de debut et fin de ...)
		if (ppath)
		{
			// new(NArrayAllocBack(&m_keysArray)) NLTRJKEY(0, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP);			// m_keysArray ayant �t� "erase" on (r�)insert la TRJKEY de d�but.
			// new(NArrayAllocBack(&m_keysArray)) NLTRJKEY(NF32_MAX, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP);		// m_keysArray ayant �t� "erase" on (r�)insert la TRJKEY de 'fin'.

			// 'C' style
			((NLTRJKEY *)NArrayAllocBack(&m_keysArray))->setup(0, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP);		 // m_keysArray ayant �t� "erase" on (r�)insert la TRJKEY de d�but.
			((NLTRJKEY *)NArrayAllocBack(&m_keysArray))->setup(NF32_MAX, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_STOP); // m_keysArray ayant �t� "erase" on (r�)insert la TRJKEY de 'fin'.
		}
	}
	m_pPath = ppath;
}

Nu32 NLTRAJECTORY::write(FILE *pfile, NDATAPACKER *pdpacker)
{
	Nu32 i;
	// 1) �criture Version
	Nu32 version_u32 = VERSION_NLTRAJECTORY_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) �criture Header
	NLTRAJECTORY_HEADER header;
	header.m_flags = m_flags;
	header.m_limits_v = m_specificKinLimits.m_v;
	header.m_limits_a = m_specificKinLimits.m_a;
	header.m_limits_j = m_specificKinLimits.m_j;
	header.m_kinTweak = m_kinTweak;
	header.m_pathIndex = NGetDataPackerIndex(pdpacker, m_pPath, NLPROJECT::DPKEY_ID::PATH);

	NGetArrayBounds(&header.m_keysArrayBounds, &m_keysArray);
	NGetArrayBounds(&header.m_userVStagesArrayBounds, &m_userVStagesArray);

	if (fwrite(&header, sizeof(NLTRAJECTORY_HEADER), 1, pfile) != 1)
		return 0;

	// Sauvegarde manuelle
	if (m_keysArray.Size)
	{
		NLTRJKEY *pkey = (NLTRJKEY *)m_keysArray.pFirst;
		for (i = 0; i < m_keysArray.Size; i++, pkey++)
		{
			pkey->write(pfile);
		}
	}

	return 1;
}

Nu32 NLTRAJECTORY::read(FILE *pfile, NDATAPACKER *pdpacker)
{
	// 1) lecture Version
	Nu32 version_u32;
	Nu32 i;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) lecture Header
	NLTRAJECTORY_HEADER header;

	switch (NGETVERSION_MAIN(version_u32))
	{
	// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLTRAJECTORY_HEADER):
		if (fread(&header, sizeof(NLTRAJECTORY_HEADER), 1, pfile) != 1)
			return 0;

		m_flags = header.m_flags;
		m_specificKinLimits.m_v = header.m_limits_v;
		m_specificKinLimits.m_a = header.m_limits_a;
		m_specificKinLimits.m_j = header.m_limits_j;
		m_specificKinLimits.build(FLAG_AUTO);
		m_kinTweak = header.m_kinTweak;
		m_pPath = (NLPATH *)NGetDataPackerPointer(pdpacker, header.m_pathIndex, NLPROJECT::DPKEY_ID::PATH);

		// chargement manuel
		// Recherche de potentiels pbs sur les arrays.
		// ('NIsArrayCorruptedOrInconsistent' se charge de faire un setup auto en cas de array full of ZEROS)
		if (NIsArrayCorruptedOrInconsistent(&m_keysArray, &header.m_keysArrayBounds, NTRUE))
			return 0;

		// preparation de l'array 'm_keysArray' .
		// Ainsi la m�moire n�c�ssaire pour stocker les n NLTRJKEY a charg�e est allou�e ( ou pas si d�j� fait )
		// Une clef 'vide' ( = emplie de zeros)  est pass�e en param ainsi toutes les clefs nouvellement allou�es
		// durant le processus de preparation seront juste une zone memoire emplie de zero et celles qui �taient
		// deja allou�es seront "clear" ( ainsi l'array d'actions contenu dans chaque key est full of zeros dans tous les cas )
		NLTRJKEY key0;
		Nmem0(&key0, NLTRJKEY);
		NResizeArray(&m_keysArray, header.m_keysArrayBounds.Size, (NBYTE *)&key0, NLclearTrjKeyInArrayCallBack);

		if (m_keysArray.Size)
		{
			NLTRJKEY *pkey = (NLTRJKEY *)m_keysArray.pFirst;
			for (i = 0; i < m_keysArray.Size; i++, pkey++)
			{
				pkey->read(pfile);
			}
		}
		return 1;

	default:
		break;
	}
	return 0;
}

#ifdef _NEDITOR
void NLTRAJECTORY::drawVStages(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLVSTAGEXSLICE *pvslice = (NLVSTAGEXSLICE *)m_vStageXSlicesArray.pFirst;
	if (pvslice)
	{
		for (Nu32 slice_id = 0; slice_id < m_vStageXSlicesArray.Size; slice_id++, pvslice++)
		{
			pvslice->drawVStages(&m_pack, p2docs, pickpack);
		}
	}
}
void NLTRAJECTORY::drawMotionProfile(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLTRJPOINT_DESC *ptrjdsc = (NLTRJPOINT_DESC *)m_pack.m_trajectoryPointDescArray.pFirst;
	ptrjdsc++;
	for (Nu32 i = 1; i < m_pack.m_trajectoryPointDescArray.Size; i++, ptrjdsc++)
		ptrjdsc->draw(p2docs, pickpack, ptrjdsc - 1);
}

void NLTRAJECTORY::drawKnodes(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLVSTAGEXSLICE *pvslice = (NLVSTAGEXSLICE *)m_vStageXSlicesArray.pFirst;
	for (Nu32 slice_id = 0; slice_id < m_vStageXSlicesArray.Size; slice_id++, pvslice++)
	{
		pvslice->drawKnodes(p2docs, pickpack);
	}
}
#endif

#ifdef _NEDITOR
void NLTRAJECTORY::drawReverseVelocityCheck(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NUTDRAWVERTEX va, vb, vc;
	// Preparation: Couleurs
	va.Color0_4f = *p2docs->getColor(0, pickpack);
	vb.Color0_4f = *p2docs->getColor(1, pickpack);
	vc.Color0_4f = *p2docs->getColor(2, pickpack);

	va.Position_3f.z = 0.0f;
	vb.Position_3f.z = 0.0f;
	vc.Position_3f.z = 0.0f;

	// Validation de la fonction getTime
	va.Position_3f.x = p2docs->transformX(0.0f);
	va.Position_3f.y = p2docs->transformY(0.0f);
	NUT_DrawPencil_From(&va);
	Nf32 t, v;
	NLKIN *pk0;
	NLKIN *pk;

	for (Nf32 s = 0.0f; s < m_pPath->m_geometry.m_ds; s += 0.0001f)
	{
		t = m_pack.getTime(s);

		if (t > m_pack.m_dt)
			t = m_pack.m_dt;

		pk = (NLKIN *)m_kinTArray.pFirst;
		while (pk->m_t < t)
			pk++;
		pk0 = pk - 1;

		v = pk0->m_v + pk0->m_a * (t - pk0->m_t) + 0.5f * pk->m_j * (t - pk0->m_t) * (t - pk0->m_t);
		if (pk->m_j > 0.0f)
		{
			va.Position_3f.x = p2docs->transformX(t);
			va.Position_3f.y = p2docs->transformY(v);
			NUT_DrawPencil_LineTo(&va);
		}
		else if (pk->m_j < 0.0f)
		{
			vc.Position_3f.x = p2docs->transformX(t);
			vc.Position_3f.y = p2docs->transformY(v);
			NUT_DrawPencil_LineTo(&vc);
		}
		else
		{
			vb.Position_3f.x = p2docs->transformX(t);
			vb.Position_3f.y = p2docs->transformY(v);
			NUT_DrawPencil_LineTo(&vb);
		}
	}
	// Et le dernier
	pk = (NLKIN *)NGetLastArrayPtr(&m_kinTArray);
	va.Position_3f.x = p2docs->transformX(pk->m_t);
	va.Position_3f.y = p2docs->transformY(pk->m_v);
	NUT_DrawPencil_LineTo(&va);
}

void NLTRAJECTORY::drawUserVStagesArray(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLVSTAGE *pvs = (NLVSTAGE *)m_userVStagesArray.pFirst;
	for (Nu32 i = 0; i < m_userVStagesArray.Size; i++, pvs++)
	{
		pvs->draw(p2docs, pickpack);
	}
}
void NLTRAJECTORY::drawLimitedUserVStagesArray(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLVSTAGE *pvs = (NLVSTAGE *)m_limitedUserVStagesArray.pFirst;
	for (Nu32 i = 0; i < m_limitedUserVStagesArray.Size; i++, pvs++)
	{
		pvs->draw(p2docs, pickpack);
	}
}
void NLTRAJECTORY::drawLimitedUserVStagesArray(const Nf32 width, const NCOLORPICKPACK pickpack)
{
	// On ne trace pas ici "m_userVStagesArray" qui contient potentiellement des VSTAGE_DESC d�passant � droite du chemin !!!
	NLVSTAGE *pvs = (NLVSTAGE *)m_limitedUserVStagesArray.pFirst;
	for (Nu32 i = 0; i < m_limitedUserVStagesArray.Size; i++, pvs++)
	{
		pvs->draw(m_pPath, width, pickpack);
	}
}

void NLTRAJECTORY::drawKeysArray(NL2DOCS *p2docs)
{
	NLTRJKEY *ptk = (NLTRJKEY *)m_keysArray.pFirst;
	for (Nu32 i = 0; i < m_keysArray.Size; i++, ptk++)
	{
		p2docs->transformX((Nu8)i, ptk->m_s, NL2DOCS_COORDS_STYLE_2);
	}
}

void NLTRAJECTORY::drawLimitedKeysArray(NL2DOCS *p2docs)
{
	NLTRJKEY **pptk = (NLTRJKEY **)m_limitedpKeysArray.pFirst;
	for (Nu32 i = 0; i < m_limitedpKeysArray.Size; i++, pptk++)
	{
		p2docs->transformX((Nu8)i, (*pptk)->m_s, NL2DOCS_COORDS_STYLE_2);
	}
}

void NLTRAJECTORY::drawLimitedKeysArray(const NLGFX *pgfx)
{
	NLTRJKEY **pptk = (NLTRJKEY **)m_limitedpKeysArray.pFirst;
	for (Nu32 i = 0; i < m_limitedpKeysArray.Size; i++, pptk++)
	{
		(*pptk)->draw(m_pPath, pgfx, i);
	}
}

void NLTRAJECTORY::drawMotionProfilePathKeyPointId(NL2DOCS *p2docs)
{

	NLPATH_POINT *ppp = (NLPATH_POINT *)m_pPath->m_geometry.m_pathPointsArray.pFirst;
	if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
	{
		NINTERVALf32 interval;

		for (Nu32 i = 0; i < m_pPath->m_geometry.m_pathPointsArray.Size; i++, ppp++)
		{
			switch (m_pack.getTime(&interval, ppp->s))
			{
			case NLTRAJECTORYPACK_GETTIME_UNIQUE:
				p2docs->transformX((Nu8)i, interval.start, NL2DOCS_COORDS_STYLE_2);
				break;

			case NLTRAJECTORYPACK_GETTIME_INTERVAL:
				p2docs->transformX((Nu8)i, interval.start, NL2DOCS_COORDS_STYLE_2);
				p2docs->transformX((Nu8)i, interval.end, NL2DOCS_COORDS_STYLE_2);
				break;

			case NLTRAJECTORYPACK_GETTIME_BEFORE:
				p2docs->transformX((Nu8)i, interval.start, NL2DOCS_COORDS_STYLE_2);
				break;

			case NLTRAJECTORYPACK_GETTIME_AFTER:
				p2docs->transformX((Nu8)i, interval.start, NL2DOCS_COORDS_STYLE_2);
				break;

			default:
				NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				break;
			}
		}
	}
	else
	{
		for (Nu32 i = 0; i < m_pPath->m_geometry.m_pathPointsArray.Size; i++, ppp++)
		{
			p2docs->transformX((Nu8)i, ppp->s, NL2DOCS_COORDS_STYLE_2);
		}
	}
}
#endif

#ifdef _NEDITOR
void NLTRAJECTORY::drawKinTArray(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLKIN *pk = (NLKIN *)m_kinTArray.pFirst;
	pk++;

	for (Nu32 i = 1; i < m_kinTArray.Size; i++, pk++)
	{
		pk->draw(p2docs, /*1,*/ pickpack, (pk - 1));
	}
}
#endif

//	*********************************************************************************************************************************************
//	*
//	* LEGACY
//	*
//	*
//	*********************************************************************************************************************************************
/*
void NLTRAJECTORY::getPoseAtT(NLDIFFERENTIAL_DRIVETRAIN_POSE * ppose, const Nf32 t)
{
	if (t <= 0.0f)
	{
		ppose->null();
		return;
	}
	else if (t < m_dt)
	{
		NLDIFFERENTIAL_DRIVETRAIN_POSE	*pp1 = (NLDIFFERENTIAL_DRIVETRAIN_POSE*)m_differentialDriveTrainPosesArray.pFirst;
		while (pp1->m_kin.m_t < t)
			pp1++;
		NLDIFFERENTIAL_DRIVETRAIN_POSE	*pp0 = pp1-1;
		ppose->m_kin.from(&pp0->m_kin, t - pp0->m_kin.m_t);
		ppose->m_localCurvature = pp0->m_localCurvature + (pp1->m_localCurvature - pp0->m_localCurvature)*(ppose->m_kin.m_s - pp0->m_kin.m_s) / (pp1->m_kin.m_s - pp0->m_kin.m_s);
		return;
	}
	else // t >= m_dt
	{
		*ppose = *(NLDIFFERENTIAL_DRIVETRAIN_POSE*)NGetLastArrayPtr(&m_differentialDriveTrainPosesArray);
		return;
	}
}
*/

/*
#ifdef _NEDITOR
void NLTRAJECTORY::drawKinRArray(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
	NLKIN* pk = (NLKIN*)m_kinRArray.pFirst; pk++;

	for (Nu32 i = 1; i < m_kinRArray.Size; i++, pk++)
	{
		pk->draw(p2docs, pickpack, (pk - 1));
	}
}
#endif
*/

/*
void NLTRAJECTORY::build(const NLDRIVETRAINSPECS *pdtspecs)
{
	NErrorIf(!m_pPath,		NERROR_NULL_POINTER);
	NErrorIf(!pdtspecs,		NERROR_NULL_POINTER);

	// ***************************************************************
	// *
	// *
	// * REINITIALISATION DES BUFFERS
	// *
	// *
	// ***************************************************************
#ifdef _NEDITOR
	NResizeArray(&m_knodeArray, 0, NULL, NULL);
#endif
	NResizeArray(&m_kinsArray, 0, NULL, NULL);

	// ***************************************************************
	// *
	// *
	// * DEFINITION DES LIMITES LOCALES
	// *
	// *
	// ***************************************************************
	NLKINLIMITS					local_limits;

	if (ISFLAG_ON(m_flags,FLAG_NLTRJ_SPECIFIC_KINLIMITS))
	{
		local_limits.m_v = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_VEL) ? m_specificKinLimits.m_v : NMIN(pdtspecs->m_limits.m_v, m_specificKinLimits.m_v);
		local_limits.m_a = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_ACC) ? m_specificKinLimits.m_a : NMIN(pdtspecs->m_limits.m_a, m_specificKinLimits.m_a);
		local_limits.m_j = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_JRK) ? m_specificKinLimits.m_j : NMIN(pdtspecs->m_limits.m_j, m_specificKinLimits.m_j);
		local_limits.compute();
	}
	else
	{
		//NErrorIf((m_flags&MASK_NTRJ_ENFORCE), NERROR_INCONSISTENT_FLAGS); // Not an error but it seems awkward to set these flags without  FLAG_NLTRJ_SPECIFIC_KINLIMITS
		local_limits = pdtspecs->m_limits;
	}

	local_limits.setVelocityMax(m_pPath->calculateMaxCruiseVelocity(&local_limits)); // set vmax as maxcruisevelocity
	// Il est possible que la methode "calculateMaxCruiseVelocity" renvoie ZERO, dans le cas o� le chemin est trop court par rapport � la vitesse Max possible pass�e en entr�e.
	// Cela arrive quand le chemin est vraiment tr�s court et �galement quand la vitesse Max possible est tr�s/trop petite...
	// Du coup comme la vitesse de croisiere est stock�e dans local_limit.m_v, si elle est nulle on sort sans rien faire.
	if ( (!local_limits.m_v) || (!local_limits.m_a) || (!local_limits.m_j))
		return;

	// ***************************************************************
	// *
	// *
	// * CONSTRUCTION DES PALIERS DE VITESSE
	// *
	// *
	// ***************************************************************
	if (!buildVelocityStages(pdtspecs, &local_limits))
		return;


	NErrorIf(!m_vStagesArray.Size, NERROR_ARRAY_IS_EMPTY);

	// ***************************************************************
	// *
	// *
	// * AJUSTEMENT/MODULATION DES PALIERS DE VITESSE
	// * [ Apllication des NLKUSERKEY ]
	// *
	// ***************************************************************


	// ***************************************************************
	// *
	// *
	// * CONSTRUCTION RECURSIVE DES NLKNODEs (pile explicite)
	// *
	// *
	// ***************************************************************

	// Cr�ation du nombre "maximum" de NLKNODE n�c�ssaires � la construction de cette trajectoire
	// chaque Knode est divis�s en 2 tant qu'il "couvre" au moins 3 Vstages, en cons�quence, le nombre maximum de Knodes qu'on peut cr�er de cette mani�re est
	//
	//		nombre de knode Max = 2*(nombre de Vstage) - 3
	//
	NARRAY *pknodearray;
#ifndef _NEDITOR
	NARRAY	knodearray;
	NSetupArray(&knodearray, 2 * m_vStagesArray.Size - 3, sizeof(NLKNODE));
	pknodearray = &knodearray;
#endif
#ifdef _NEDITOR
	Ns32 diff = 2 * m_vStagesArray.Size - 3 - m_knodeArray.Capacity;
	if (diff > 0)
		NIncreaseArrayCapacity(&m_knodeArray, diff);
	pknodearray = &m_knodeArray;
#endif

	// Cr�ation de la file de travail � la taille maximum possible pour une utilisation optimis�e:
	// Il est certes impossible que la totalit� des noeuds cr��s se retrouvent ensemble et en m�me temps sur la file.
	// Cependant, allouer suffisement de m�moire pour �tre en mesure de stocker l'ensemble des pointeurs sur noeuds dans la file va permettre une gestion plus efficace du FIFO sur la file.
	// En effet, nous allons g�rer manuellement la "taille" de la file et ne jamais d�sallou� les �l�ments sortis de la file !

	// Cr�ation d'un premier noeud qu'on ajoute simplement en fin de file
	// On g�re manuellement le nombre de noeuds de la file restant � traiter.( filesize )

	NLKNODE						*pkn = (NLKNODE*)NArrayAllocBack(pknodearray);
	pkn->setup((NLVSTAGEX*)NGetFirstArrayPtr(&m_vStagesArray), (NLVSTAGEX*)NGetLastArrayPtr(&m_vStagesArray), NULL, NULL);

	NLKNODE						*phead = pkn;
	Nu32						filesize = 1;

	NLVSTAGEX					*pvsplit;
	NLKNODE						*pknchild_left, *pknchild_right;
	NLSMOTIONSHAPE				sleft, sright;
	NLSCUTOUTOFFSETS			cutout;
	NLSTARTSTITCH				immediate_start;
	NLSCUTINOFFSETS				cutin;
	NLENDSTITCH					immediate_end;

	Nf32						s_left_in,s_right_out;
	Nf32						s_left_out, s_right_in;
	Nf32						v_left_in, v_right_out;
	Nf32						tmp_dbg;

	Nu32						mps_count	= 0;	// nombre de MotionProfile feuilles ( car d�clar� optimum et non "splitable") d�clar� SUP ou SDOWN ( donc plus simples et l�gers que le SUP_FLAT_SDOWN )
	Nu32						split_count = 0;	// nombre de split effectif. ( permet de connaitre le nombre de NLKNODE feuilles soit split_count + 1 )


	// ... Et c'est parti !
	while (filesize)
	{
		// on r�cup�re le noeud le plus ancien ( celui en t�te de File )
		pkn = phead;
		filesize--;
		phead ++;

		if ( pvsplit = pkn->deepSolveAndGetSplitVStage(&local_limits) )
		{
			// +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
			// |																																														|
			// | L'id�e est de Diviser ( Split ) l'intervalle "pkn" [m_pVfrom, m_pVto] en deux intervalles fils avec comme point de split le VStage "le plus bas" dans l'intervalle.					|
			// | Chacun des deux intervalles est v�rifi� pour �tre s�r qu'il est possible de rejoindre les deux bornes sans les d�passer, ni en abscisse, ni en vitesse.								|
			// | Les tests sont les suivants:																																							|
			// |	A) Croisement(s):	(Motion Profile Crossing)																																		|
			// |						On v�rifie que le "Motion Profile Minimum" de l'intervalle fils de gauche ne croise pas le "Motion Profile Minimum" de l'intervalle fils de droite.				|
			// |						On appelle "Motion Profile Minimum" la S-Shape permettant de passer directement de la vitesse de d�but d'intervalle � celle de fin d'intervalle.				|
			// |																																														|
			// |	B) D�passement(s):	(Abscissa Overshooting )																																			|
			// |																																														|
			// |	C) Plafonnement(s)	? (capping)																																														|
			// |																																														|
			// s_left_in est l'abscisse la plus � gauche de l'intervalle [from,to]et s_right_out est l'abscisse la plus � droite.

			// Abscisses des intervalles fils apr�s split ...
			// Pour l'intervalle fils [from,split]		[s_left_in, s_left_out]
			// Pour l'intervalle fils [split, to]		[s_right_in, s_right_out]

			sleft.forwardBuildUp(pkn->m_pVfrom, pvsplit->m_v);
			sright.backwardBuildDown(pvsplit->m_v, pkn->m_pVto);
			s_left_in	= sleft.m_s0;
			v_left_in	= sleft.m_v0;
			s_right_out = sright.m_s0 + sright.m_ds;
			v_right_out = sright.m_v1;

			// +------------------------------------------------+
			// | A) Croisement(s) ? (Motion Profile Crossing)	|
			// +------------------------------------------------+
			// ... la distance couverte par les 2 motion S shapes ( sleft et sright ) est plus grande que la distance disponible entre les 2 vstage
			if ( (sleft.m_ds + sright.m_ds) > (s_right_out - s_left_in ) )
			{
				// En cas de "croisement" des motion profiles : pas de SPLIT !
				// On en reste donc � la solution d�j� calcul�e pr�c�dement et stosk�e dans pkn. ( calcul�e precedement = lors du pr�c�dent passage ici avec construction de la solution minimum avec left ou right )
				continue;
			}
			// +------------------------------------------------+
			// | B) D�passement(s) ? (Abscissa Overshooting)	|
			// +------------------------------------------------+
			// ... Pas de croisement, nous sommes s�rs que les 2 profils S "tiennent", sans se percuter l'un l'autre.
			// Il est cependant possible que l'un d'eux ( pas les deux car sinon il y aurait croisement ) d�passe  � droite, ou � gauche, de Split.
			else if ( sleft.forwardOvershoot(&cutout, &immediate_start, pkn->m_pVfrom, pvsplit) )
			{
				// D�passement � droite de split, ... donc calcul des contraintes associ�es
				// Cela signifie que la "distance maximum" s�parant From de Split n'est pas suffisante pour atteindre Split.V depuis From.V...
				// Du coup, Split.V est atteinte plus tard/plus loin dans l'intervalle voisin, c'est � dire dans [Split,To]...
				// En cons�quence, on consid�re que [From,Split] ne sera plus subdivisable.
				// A priori il ne devrait pas y avoir d'autres Vstages dans l'intervalle [From,Split] mais cela n'est pas non plus impossible,
				// On veillera donc � bloquer toute nouvelle subdivision, en flaggant le knode "FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION"
				pvsplit->nullPrimeFlatShape();

				pknchild_left = (NLKNODE*)NArrayAllocBack(pknodearray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(pknodearray);
				split_count++;
				filesize += 2;

				pkn->children(pknchild_left, pknchild_right);

				pknchild_left->setup(pkn->m_pVfrom, pvsplit, pknchild_right, pkn);
				pknchild_right->setup(pvsplit, pkn->m_pVto, pknchild_left, pkn);


				// build left Child:
				// -----------------
				// La Shape de gauche construite en forward build up et donc en d�passement � droite de Vsplit devient donc le motion profile de left child.
				// On prend soin de renseigner son cutout, c'est � dire "quand" et "ou" l'arriv�e de la shape devra �tre "coup�e" ( avant son dernier kin 'classique' )
				pknchild_left->m_motionProfile.sUp(&sleft, &pkn->m_pVfrom->m_stitch.m_start.m_cutIn, &cutout);
				// On flag cette solution comme optimum pour bloquer toute subdivision future.
				FLAG_ON(pknchild_left->m_motionProfile.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION);
				mps_count++;

				// build right Child:
				// ------------------
				// Dans ce cas, nous avons un Ramped Motion Profile � droite.
				// Il est bien s�r compos� de 2 S_SHAPEs, une � sa gauche, une � sa droite !
				// La S_SHAPE de gauche (du motion profile de droite ! )
				// Elle permet le "Red�marrage" depuis le point fant�me cr�� � droite de l'intervalle de gauche (pknchild_left) ;)
				//
				sleft.m_s0 = pvsplit->m_s1	+ immediate_start.m_s1Offsets.m_ds;
				sleft.m_v0 = pvsplit->m_v	+ immediate_start.m_s1Offsets.m_dv;					// vitesse initiale
				sleft.m_v1 = pvsplit->m_v;																// vitesse finale
				// sleft.m_jx = ...;		identique � celui d�j� pr�sent dans sleft	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sleft.m_tx = -cutout.m_dt;																// Dur�e n�c�ssaire pour que l'accel. (initialement nulle) atteigne sa valeur max. ( soit m_ax ).
				sleft.m_ax =  cutout.m_da; // sleft.m_jx*sleft.m_tx;									// Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				//tmp_dbg = sleft.m_jx*sleft.m_tx;
				//NErrorIf(cutout.m_da != tmp_dbg, NERROR_SYSTEM_CHECK);
				sleft.m_vx = -cutout.m_dv;																// Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sleft.m_tb = 0.0f;																		// Dur�e de maintient de l'acceleration maximum.
				sleft.m_sx = sleft.m_jx*NPOW3(sleft.m_tx)/6.0f;											// Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
																										// ... (initialement nulle) atteind sa valeur maximum ( soit m_ax )
																										// ! 'm_sx' est une 'portion de distance' et ne repr�sente qu'une partie de la distance
																										// ( c'est pourquoi on parle de portion ) parcourue pendant la dur�e 'm_tx'.
																										// ! La distance totale parcourue pendnat la dur�e 'm_tx' (i.e durant la phase montante)
																										// �tant: s = m_sx + v0*m_tx
				sleft.m_ds = (sleft.m_v0 + sleft.m_v1)*sleft.m_tx;										// Distance totale couverte pas la S Motion Shape
				pknchild_right->m_motionProfile.sRamped(&sleft, &sright, &immediate_start.m_cutIn, &pkn->m_pVto->m_stitch.m_end.m_cutOut);
			}
			else if (sright.backwardOvershoot(&cutin, &immediate_end, pvsplit, pkn->m_pVto))
			{
				// D�passement � gauche de split, ... donc calcul des contraintes associ�es
				// Cela signifie que la "distance maximum" s�parant Split de To n'est pas suffisante pour atteindre To.V depuis Split.V...
				// Du coup, cela obligerait � partir "avant split" avec une vitesse de Split.V et donc "d'envahir" l'intervalle voisin, c'est � dire [From,Split]...
				// En cons�quence, on consid�re que [Split,To] ne sera plus subdivisable.
				// A priori il ne devrait pas y avoir d'autres Vstages dans l'intervalle [Split,To] mais cela n'est pas non plus impossible, donc on veille � bloquer toute nouvelle subdivision.
				//pvsplit->m_overshootOffsets = ovs;
				pvsplit->nullPrimeFlatShape();

				pknchild_left = (NLKNODE*)NArrayAllocBack(pknodearray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(pknodearray);
				split_count++;
				filesize += 2;

				pkn->children(pknchild_left, pknchild_right);

				pknchild_left->setup(pkn->m_pVfrom, pvsplit, pknchild_right, pkn);
				pknchild_right->setup(pvsplit, pkn->m_pVto, pknchild_left, pkn);


				// build RIGHT:
				// ------------
				pknchild_right->m_motionProfile.sDown(&sright, &cutin, &pkn->m_pVto->m_stitch.m_end.m_cutOut);

				FLAG_ON(pknchild_right->m_motionProfile.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION);
				mps_count++;

				// build LEFT:
				// -----------
				// LEFT.right
				sright.m_tx = -immediate_end.m_cutOut.m_dt;													// Dur�e n�c�ssaire pour que l'accel. (initialement nulle) atteigne sa valeur max. ( soit m_ax ).
				sright.m_v0 = pvsplit->m_v;																	// vitesse initiale
				sright.m_v1 = pvsplit->m_v	+ immediate_end.m_s0Offsets.m_dv;								// vitesse finale
				sright.m_ds = (sright.m_v0 + sright.m_v1) * sright.m_tx;										// Distance totale couverte pas la S Motion Shape

				sright.m_s0 = pvsplit->m_s0 + immediate_end.m_s0Offsets.m_ds - sright.m_ds;
				// sright.m_jx = ...;		identique � celui d�j� pr�sent dans sright	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sright.m_ax =  immediate_end.m_cutOut.m_da;													// Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				sright.m_vx = -immediate_end.m_cutOut.m_dv;													// Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sright.m_tb = 0.0f;																			// Dur�e de maintient de l'acceleration maximum.
				sright.m_sx = sright.m_jx*NPOW3(sright.m_tx) / 6.0f;										// Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
																											// ... (initialement nulle) atteind sa valeur maximum ( soit m_ax )
																											// ! 'm_sx' est une 'portion de distance' et ne repr�sente qu'une partie de la distance
																											// ( c'est pourquoi on parle de portion ) parcourue pendant la dur�e 'm_tx'.
																											// ! La distance totale parcourue pendnat la dur�e 'm_tx' (i.e durant la phase montante)
																											// �tant: s = m_sx + v0*m_tx

				pknchild_left->m_motionProfile.sRamped(&sleft, &sright, &pkn->m_pVfrom->m_stitch.m_start.m_cutIn, &immediate_end.m_cutOut);
			}

			// +----------------------------------------+
			// | C) Plafonnement(s)	? (capping)			|
			// +----------------------------------------+
			else
			{
				s_right_in = sright.m_s0;
				s_left_out = s_left_in + sleft.m_ds;

				// Ni croisement, ni d�passement, les deux intervalles fils de "pkn" sont "buildables"
				pknchild_left = (NLKNODE*)NArrayAllocBack(pknodearray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(pknodearray);
				split_count++;
				filesize += 2;

				pkn->children(pknchild_left, pknchild_right);
				pknchild_left->setup(pkn->m_pVfrom, pvsplit, pknchild_right, pkn);
				pknchild_right->setup(pvsplit, pkn->m_pVto, pknchild_left, pkn);

				// Sera-t'il possible de traverser chacun de ces 2 intervalles en d�passant la vitesse "Split.V" en cours de route ...
				// ... et en rejoingnant Split.V  � la toute fin ?
				// En fait, la seule raison qui, � partir de maintenant, peut emp�cher de le faire est le 'plafonnement' de l'intervalle.

				// +----------------------------------------+
				// | C.1) [From,Split] est-il plafonn� ?	|
				// +----------------------------------------+
				if ( s_left_out >= pvsplit->m_s0)
				{
					// Plafonnement � l'arriv�e sur l'intervalle [From,Split]. C'est � dire qu'en rejoignant directement la vitesse Split.V depuis From.V on  d�passe Split.s0.
					// Cela est autoris� certes, mais cela indique qu'il ne sera pas possible de faire mieux !
					// L'intervalle [From,Split] permet donc la construction d'un Motion Profile de type SUP ou SUP_FLAT
					pknchild_left->m_motionProfile.sUp(&sleft, &pkn->m_pVfrom->m_stitch.m_start.m_cutIn, NULL);
					pvsplit->setPrimeFlatShapeS0(s_left_out);
					FLAG_ON(pknchild_left->m_motionProfile.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION);
					mps_count++;
				}
				// +------------------------------------+
				// | D) [From,Split] Ok !				|
				// +------------------------------------+
				else if (sleft.m_ds)//  (s_left_out < pvsplit->m_s0)
				{
					pknchild_left->m_motionProfile.sUpFlat(&sleft, &pkn->m_pVfrom->m_stitch.m_start.m_cutIn, pvsplit->m_s0 - s_left_out);
				}
				else  // .... left est FLAT car sleft.m_ds est null !!! ( rappel sleft a �t� construite  par 'sleft.forwardBuildUp' )
				{
					pknchild_left->m_motionProfile.flat(s_left_in, pvsplit->m_s0,sleft.m_v0);
				}

				// +------------------------------------+
				// | E) [Split,To] est-il plafonn� ?	|
				// +------------------------------------+
				if (s_right_in <= pvsplit->m_s1)
				{
					// Plafonnement au d�part sur l'intervalle [Split,To]. C'est � dire qu'en rejoignant directement la vitesse To.V depuis Split.V on  doit commencer � d�c�l�rer avant Split.s1.
					// Cela est autoris� certes, mais cela indique qu'il ne sera pas possible de faire mieux !
					// L'intervalle [Split,To] permet donc la construction d'un Motion Profile de type SDOWN ou FLAT_SDOWN
					pknchild_right->m_motionProfile.sDown(&sright, NULL, &pkn->m_pVto->m_stitch.m_end.m_cutOut);
					pvsplit->setPrimeFlatShapeS1(s_right_in);
					FLAG_ON(pknchild_right->m_motionProfile.m_flags, FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION);
					mps_count++;
				}
				// +------------------------------------+
				// | E) [Split,To] Ok !					|
				// +------------------------------------+
				else if(sright.m_ds)// (s_right_in > pvsplit->m_s1)
				{
					pknchild_right->m_motionProfile.flatSdown(&sright, &pkn->m_pVto->m_stitch.m_end.m_cutOut , s_right_in - pvsplit->m_s1);
				}
				else
				{
					pknchild_right->m_motionProfile.flat(pvsplit->m_s1, s_right_out, sright.m_v0);
				}
			}
		} // ... if ( pvsplit = pkn->deepSolveAndGetSplitVStage(&local_limits) )
	} // ... while(flisesize)

	// Promote !
	// Toutes les options �tant calcul�es et hi�rarchis�es, reste maintenant � ne retenir que les meilleures solutions et � calculer/ stocker les kins associ�s.
	// Pour se faire nous allons avoir besoin d'un buffer de travail, et pour des questions de rapidit� d'�x�cution nous allons faire en sorte d'allouer ce buffer en une seule fois ( pas de r�allocations en cours de route ) !
	// Nous devons donc estimer la taille maximum que pourrait atteindre ce buffer pour NE JAMAIS �crire/lire � l'ext�rieur.
	//							...
	// Estimation de la taille max. du buffer n�c�ssaire � la promotion des motion profiles en Kin
	//
	// Nombre de Kins:	Pour rappel, la promotion d'un Motion profile en Kin ne cr��e pas le kin0 du motion profile... celui - ci �tant consid�r� comme pr�-existant !
	//					Ainsi un motion profile de type SUP se d�crit avec 4 kins :
	//					( mais seul 3 seront r�ellement cr��s )
	//																				kin0 (			t0 ) ... La promotion ne cr�e pas ce premier Kin mais assume qu'il existe d�j�.
	//																				kin1 (kin0	+	tx )
	//																				kin2 (kin1	+	tb ) ... n'existe que si tb > 0
	//																				kin3 (kin2	+	tx )
	//
	//										|		Nombre MINimum de Kin cr��s � la promotion		|		Nombre MAXimum de kin cr��s � la promotion		|
	//										+														+														+
	// Motion Profile	EMPTY				|							0							|							0							|
	// Motion Profile	SDOWN				|		( Tb null )			2							|							3							|
	// Motion Profile	FLAT				|							1							|							1							|
	// Motion Profile	FLAT_SDOWN			|		( Tb null )			3							|							4							|
	// Motion Profile	SUP					|		( Tb null )			2							|							3							|
	// Motion Profile	SUP_SDOWN			|		( Tb(s) null(s) )	4							|							6							|
	// Motion Profile	SUP_FLAT			|		( Tb null )			3							|							4							|
	// Motion Profile	SUP_FLAT_SDOWN		|		( Tb(s) null(s) )	5							|							7							|



	// La boucle de cr�ation de motion profile pr�c�dente nous renvoie 2 compteurs:
	//					'split_count'	Le nombre total de divisions d'intervalle en deux sous-intervalles.
	//									On a donc "split_count + 1" intervalles 'feuilles' dans notre arbre.
	//					'mps_count'		Le nombre de motion profile feuille de type SUP ou SDOWN.
	//									( Ces motion profile �tant consid�r�s comme optimum solution n'ont pas �t� re-subdivis�s en 2. Ils sont feuilles � coup s�r !)
	// On en conclut que, au maximum, et dans le cas ou la solution optimum globale descendrait jusqu'� toutes les feuilles de l'arbre, alors ces motions profile feuilles
	// composant la solution seraient r�partis comme ceci:
	//
	//					'spli_count + 1' motion profile dont 'mps_count' motion profile de type SUP ou SDOWN
	//
	// Du coup, en se basant sur le tableau livr� plus haut, le nombre total de kins cr��s pour d�crire cette solution ( descendant jusqu'aux feuilles )  est :
	//
	//		K = S * (split_count + 1)  - D * mps_count
	//
	//		avec
	//
	//		S	= nbre de kin cr��s � la promotion d'un motion profile SUP_FLAT_SDOWN pr�c�d� d'une FLAT  SHAPE ( la prime flat shape repr�sentant le chemin parcouru sous le plafond d'un NLVSTAGEX )
	//		S	= 7 + 1
	//		S	= 8
	//
	//		D	= S - nbre de kin cr��s � la promotion d'un motion profile SUP ou SDOWN  pr�c�d� d'une FLAT  SHAPE
	//		D	= 8 - (3+1)
	//		D	= 4
	//
	//		Donc pour faire simple, on estime le nombre de KIN cr��s par la promotion de (split_count + 1) motion profile(s) SUP_FLAT_SDOWN pr�c�d�s tous d'une FLAT SHAPE
	//		... et, comme on sait qu'en fait, parmis eux certains ne sont "que" des SUP ou SDOWN on retranche le nombre de KIN estim�s en trop...
	//
	//		( la capacit� max etant de K+1 car il faut ins�rer le premier kin de d�marrage ! )
	NSetArrayCapacity(&m_kinsArray, 1 + 8 * (split_count+1) - 4 * mps_count , NULL);
	// Insertion du premier Kin dans le buffer
	NLKIN *pkin = (NLKIN*)NArrayAllocBack(&m_kinsArray);
	pkin->null();
	pkn = (NLKNODE*)pknodearray->pFirst;
	Nu32 size = pkn->promote(pkin);
	NErrorIf(m_kinsArray.Size + size > m_kinsArray.Capacity, NERROR_SYSTEM_GURU_MEDITATION);
	NUpSizeArray(&m_kinsArray, size , NULL);

	#ifndef _NEDITOR
	NClearArray(&knodearray, NULL);
	#endif

	// Pour finir: longueur et dur�e de la trajectoire
	pkin = (NLKIN*)NGetLastArrayPtr(&m_kinsArray);
	m_dt = pkin->m_t;
	NErrorIf( size && NLKIN_S_DISTINCT(m_pPath->m_geometry.m_ds, pkin->m_s) , NERROR_WRONG_VALUE);	// on accepte que size == 0, c'est � dire que le promote n'ai rien fait !
																									// ce qui arrive si la vitesse max est �gale/inf�rieure au threshold utilis� dans
																									// la construction des motions profile ... [ cf. NLSMOTIONPROFILE::sRamped(...) ]
}
*/
/*
void NLTRAJECTORY::buildTrajectoryPack(NLTRAJECTORY_BASIC_PACK *ppack)
{
	if (!m_kinsArray.Size)
		return;

	NLTRAJECTORY_BASIC_STATE	state;
	NLTRAJECTORY_BASIC_STATE	*ppushed_state = NULL;
	NLKIN	 kin;
	NLKIN	*pkin0		= (NLKIN*)m_kinsArray.pFirst;
	NLKIN	*pkin1		= pkin0;
	Nu32	 kin1_id	= 0;

	NLPATH_POINT *pkp0 = (NLPATH_POINT*)m_pPath->m_geometry.m_pathPointsArray.pFirst;
	NLPATH_POINT *pkp1 = pkp0+1;
	NLPATH_PRIMITIVE *pprim =(NLPATH_PRIMITIVE*)m_pPath->m_geometry.m_primitivesArray.pFirst;

	Nf32	curvature;
	Nf32	slocal;

	ppack->m_matrix = m_pPath->m_matrix;

	NResizeArray(&ppack->m_basicKeyStatesArray, 0, NULL, NULL);

	for (Nu32 i = 0; i < m_pPath->m_geometry.m_primitivesArray.Size; i++, pkp0 = pkp1, pkp1++, pprim++)
	{
		// +------------------------------------------------------------------------------------------------------------------------------------
		// |
		// A)	Insertion de STATE pour tous les kins situ�s 'SUR' la primitive.
		//		Tant que le KIN a une abscisse S inf�rieure � celle du keypoint1 de la primitive ...
		//		... On ins�re un state issu du kin et on passe au kin suivant
		while ( (pkin1->m_s < pkp1->s) && (kin1_id < m_kinsArray.Size) )
		{
			// On recup�re la courbure en pkin->m_s.
			// On sait que la courbure est constante pour les primitives SEGMENT et ARC et quelle varie lin�airement pour les CLOTHOIDES.
			// Donc ...
			// sachant que que pkin1->m_s est situ� "entre" pkp0 et pkp1 sur la primitive pprim ...
			if (pprim->m_core.m_id == NLPATH_PRIMITIVE_ID_CLOTHOID)
			{
				slocal = pkin1->m_s - pkp0->s;
				curvature = ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND) ? (pprim->m_core.m_l - slocal) * pkp0->k / pprim->m_core.m_l : slocal * pkp1->k / pprim->m_core.m_l;
			}
			else // pprim->m_core.m_id ==  NLPATH_PRIMITIVE_ID_SEGMENT ou pprim->m_core.m_id ==  NLPATH_PRIMITIVE_ID_ARC
			{
				curvature = pkp1->k;
			}
			state.set(pkin1, curvature);
			ppushed_state = (NLTRAJECTORY_BASIC_STATE*)NArrayPushBack(&ppack->m_basicKeyStatesArray, (NBYTE*)&state);

			// Kin suivant
			pkin0 = pkin1;
			pkin1 ++;
			kin1_id ++;
		}
		// |
		// +------------------------------------------------------------------------------------------------------------------------------------

		// +------------------------------------------------------------------------------------------------------------------------------------
		// |
		// B) Le Kin en cours a une abscisse >= au keypoint1 de la primitive
		//	  Ou ...
		//	  Le Kin en cours est invalide ! ( car le dernier Kin valide avait une abscisse inf�rieure � celle du keypoint 1
		//	  ... et a �t� ins�r� en A)
		//

		// A partir d'ici nous savons:
		//  pkin1->m_s >= pkp1->s
		//		OU
		//  kin1_id == m_kinsArray.Size, ce qui signifie que ...
		//			... l'abscisse curviligne du dernier Kin de m_kinsArray est inf�rieure � longueur totale du chemin...
		//			... et que le pointeur pkin1 courant est invalide ( hors array )
		if (kin1_id < m_kinsArray.Size)
		{
			// Si le Kin en cours est valide, alors cela revient � dire qu'on a quitt� la boucle while pr�c�dente
			// car Le Kin en cours (pkin1) avait une abscisse >= au keypoint1 de la primitive
			NErrorIf(pkin1->m_s < pkp1->s, NERROR_INCONSISTENT_VALUES);
			// En cons�quence, ...
			// On calcule et on ins�re un KeyState issu de "pkp1"

			// Il y a 2 cas:
			// B.a)	Le cas ou keypoint et kin sont confondus:
			if (pkin1->m_s == pkp1->s)
			{
				state.set(pkin1, pkp1->k);

				// Un state bas� sur le kin courant �tant cr��, on passe au Kin suivant:
				// ( le state sera ins�r� quelques ligne plus bas en B.c) )
				pkin0 = pkin1;
				pkin1++;
				kin1_id++;
			}
			// B.b) Le cas ou (pkin1->m_s > pkp1->s)
			else
			{
				// SI pkin1->m_s n'est pas �gal � pkp1->s il ne peut �tre que sup�rieur.
				// En effet la boucle while situ�e plus haut... " while ((kin1_id < m_kinsArray.Size) && (pkin1->m_s < pkp1->s)) "
				// ... s'est normalement charg�e de cr�er et cr�er/pousser un state dans ce cas... donc ici: pkin1->m_s > pkp1->s
				// Et si ce n'est pas le cas, alors on crash, c'est un bug !!!
				NErrorIf(pkin1->m_s < pkp1->s, NERROR_INCONSISTENT_VALUES);

				// En cons�quence, on cr��e un kin, situ� entre kin0 et kin1, � partir de l'abscisse curvuligne de pkp1.
				getKinAtS(&kin, pkin0, pkin1, pkp1->s);
				state.set(&kin, pkp1->k);
			}
			// B.c) On ins�re le state cr�� dans la liste:
			ppushed_state = (NLTRAJECTORY_BASIC_STATE*)NArrayPushBack(&ppack->m_basicKeyStatesArray, (NBYTE*)&state);
		}
		else
		{

#ifdef _DEBUG
			// Si le Kin en cours est invalide, on sort de la boucle for...
			// Ce kin invalide ne peut �tre que celui d'index 'm_kinsArray.Size'
			NErrorIf(!(kin1_id == m_kinsArray.Size), NERROR_INCONSISTENT_VALUES);

			// Tous les kins, y compris le dernier ( v=0,a=0, ... ) ont �t� ins�r�s sous forme de keystate(s).
			// ... Certes, l'abscisse du dernier kin ( et donc du dernier keystate ) est inf�rieure � l'abscisse du dernier point du chemin !
			// ( sinon on ne passerait pas ici) ... Mais cela est du au cumul des erreurs de calcul et la diff�rence est obligatoirement infime.
			//
			// On verifie que la primitive en cours est bien la derniere du chemin.
			NErrorIf(pprim != (NLPATH_PRIMITIVE*)NGetLastArrayPtr(&m_pPath->m_geometry.m_primitivesArray), NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkp1 != (NLPATH_POINT*)NGetLastArrayPtr(&m_pPath->m_geometry.m_pathPointsArray), NERROR_INCONSISTENT_VALUES);
			// On v�rifie �galement que l'erreur sur l'abscisse finale reste tr�s petite.
			// On utilise pkin0 ici car pkin1 est invalide et pkin0 est �gal au dernier pkin1 valide ( cf boucle while plus haut )
			Nf32 final_abscissa_error = pkp1->s - pkin0->m_s;
			// Cette erreur doit �tre positive ! ...
			NErrorIf(final_abscissa_error < 0.0f, NERROR_INCONSISTENT_VALUES);
			// ... et toute petite
			NErrorIf(final_abscissa_error > NLKIN_EPSILON_S, NERROR_INCONSISTENT_VALUES);
#endif

			// On sort donc de la boucle for
			break;
		}
	}

	if (!ppushed_state)
		return;

	// Une fois sorti de la boucle for on effectue une derni�re v�rification sur kin1_id.
	// En effet, ce dernier peut-�tre:
	//			a)	Egal � m_kinsArray.Size-1 [ kin1_id == m_kinsArray.Size-1 ] :
	//				Cela signifie que le dernier Kin n'est toujours pas ins�r�. Cela s'explique par les erreurs d'approximation du aux nbres � virgule flottante.
	//				En fait ce dernier kin n'est pas ins�r� car son abscisse est plus grande que l'abscisse du dernier point du chemin. [ voir (B.b) plus haut ].
	//				On ins�re donc tout simplement un dernier state issu de ce dernier Kin. C'est imortant de le faire car le dernier state doit contenir le Kin de fin
	//				avec v=0 et a=0. Que l'abscisse de ce dernier kin soit l�g�rement sup�rieure � l'abscisse de fin de chemin est sans cons�quence.
	//				! Attention, Dans ce cas pr�cis pkp1 est hors limite car nous sommes sorti de la boucle for ... le dernier point du chemin � consid�rer est pkp0 !
	if (kin1_id == m_kinsArray.Size - 1)
	{
		NErrorIf(pkin1->m_s <= pkp0->s, NERROR_SYSTEM_GURU_MEDITATION);		// Normalement, comme le kin n'est toujours pas ins�r�, son abscisse ne peut �tre que > � celle de pkp1
		NErrorIf((pkin1->m_s - pkp0->s) > 1e-3f, NERROR_VALUE_OUTOFRANGE);	// Si la diff�rence est trop importante, il y a un probl�me autre que celui potentiellement engendr� par les erreurs d'approximation !

		state.set(pkin1, pkp0->k);
		ppushed_state = (NLTRAJECTORY_BASIC_STATE*)NArrayPushBack(&ppack->m_basicKeyStatesArray, (NBYTE*)&state);
	}
#ifdef _DEBUG
	else
	{
		//		b)	Egal � m_kinsArray.Size   [ kin1_id == m_kinsArray.Size	 ] :
		//			Cela signifie que le Dernier Kin a bien �t� ins�r� sous forme d'un keystate, lui-m�me dernier de sa liste.
		//			Pour la bonne forme on verifie juste que ce dernier keystate comprend bien un kin d'arriv�e, c'est � dire un kin avec v=0 et a =0
		//			pkin1 est invalide, comme kin_id. pkin0  correspond normalement au dernier kin de la liste.
		if (kin1_id == m_kinsArray.Size)
		{
			NErrorIf(pkin0 != (NLKIN*)NGetLastArrayPtr(&m_kinsArray), NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_v != ppushed_state->m_kin.m_v, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_v != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_a != ppushed_state->m_kin.m_a, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_a != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_s != ppushed_state->m_kin.m_s, NERROR_INCONSISTENT_VALUES);
			//	Dans cette configuration, on est sur que le dernier kin (et donc le dernier state ins�r� dont il est issu ) a une abscisse <= la fin du chemin.
			NErrorIf(ppushed_state->m_kin.m_s > m_pPath->m_geometry.m_ds, NERROR_INCONSISTENT_VALUES);
		}
		//		c) Toute autre valeur sera consid�r�e comme un BUG !
		NErrorIf((kin1_id != m_kinsArray.Size) && (kin1_id != (m_kinsArray.Size - 1)), NERROR_INCONSISTENT_VALUES);
	}
#endif

	// Pour finir on enregistre la "dur�e" et la "distance" du pack
	ppack->m_ds		= ppushed_state->m_kin.m_s;
	ppack->m_dt		= ppushed_state->m_kin.m_t;
}
*/
/*
void NLTRAJECTORY::buildTrajectoryPack(NLTRAJECTORY_PACK* ppack)
{
	if (!m_kinsArray.Size)
		return;

	NLTRJPOINT_DESC	keystate;
	NLTRJPOINT_DESC* pushed_keystate = NULL;
	NLKIN	 kin;
	NLKIN* pkin0 = (NLKIN*)m_kinsArray.pFirst;
	NLKIN* pkin1 = pkin0;
	Nu32	 kin1_id = 0;

	ppack->m_pathGeometry = m_pPath->m_geometry;
	ppack->m_matrix = m_pPath->m_matrix;
	NLPATH_POINT* pkp0 = (NLPATH_POINT*)ppack->m_pathGeometry.m_pathPointsArray.pFirst;
	NLPATH_POINT* pkp1 = pkp0 + 1;
	NLPATH_PRIMITIVE* pprim = (NLPATH_PRIMITIVE*)ppack->m_pathGeometry.m_primitivesArray.pFirst;

	NResizeArray(&ppack->m_keyStatesArray, 0, NULL, NULL);

	for (Nu32 i = 0; i < ppack->m_pathGeometry.m_primitivesArray.Size; i++, pkp0 = pkp1, pkp1++, pprim++)
	{
		// +------------------------------------------------------------------------------------------------------------------------------------
		// |
		// A)	Insertion de KEYSTATE pour tous les kins situ�s 'SUR' la primitive.
		//		Tant que le KIN a une abscisse S inf�rieure � celle du keypoint1 de la primitive ...
		//		... On ins�re un keystate issu du kin et on passe au kin suivant
		while ( (pkin1->m_s < pkp1->s) && (kin1_id < m_kinsArray.Size) )
		{
			keystate.set(pkin1, pkp1, pprim);
			pushed_keystate = (NLTRJPOINT_DESC*)NArrayPushBack(&ppack->m_keyStatesArray, (NBYTE*)&keystate);

			// Kin suivant
			pkin0 = pkin1;
			pkin1++;
			kin1_id++;
		}
		// |
		// +------------------------------------------------------------------------------------------------------------------------------------

		// +------------------------------------------------------------------------------------------------------------------------------------
		// |
		// B) Le Kin en cours a une abscisse >= au keypoint1 de la primitive
		//	  Ou ...
		//	  Le Kin en cours est invalide ! ( car le dernier Kin valide avait une abscisse inf�rieure � celle du keypoint 1
		//	  ... et a �t� ins�r� en A)
		//
		if (kin1_id < m_kinsArray.Size)
		{
			// Si le Kin en cours est valide, alors cela revient � dire qu'on a quitt� la boucle while pr�c�dente
			// car Le Kin en cours (pkin1) avait une abscisse >= au keypoint1 de la primitive
			NErrorIf(pkin1->m_s < pkp1->s, NERROR_INCONSISTENT_VALUES);
			// En cons�quence, ...
			// On calcule et on ins�re un KeyState issu de "pkp1"

			// Il y a 2 cas:
			// B.a)	Le cas ou keypoint et kin sont confondus:
			if (pkin1->m_s == pkp1->s)
			{
				keystate.set(pkin1, pkp1, pprim);

				// Un keystate bas� sur le kin courant �tant cr��, on passe au Kin suivant:
				// ( le keystate sera ins�r� quelques ligne plus bas en B.c) )
				pkin0 = pkin1;
				pkin1++;
				kin1_id++;
			}
			// B.b) Le cas ou (pkin1->m_s > pkp1->s)
			else
			{
				// SI pkin1->m_s n'est pas �gal � pkp1->s il ne peut �tre que sup�rieur.
				// En effet la boucle while situ�e plus haut... " while ((kin1_id < m_kinsArray.Size) && (pkin1->m_s < pkp1->s)) "
				// ... s'est normalement charg�e de cr�er et pousser un Keystate dans ce cas... donc ici: pkin1->m_s > pkp1->s
				// Et si ce n'est pas le cas, alors on crash, c'est un bug !!!
				NErrorIf(pkin1->m_s < pkp1->s, NERROR_INCONSISTENT_VALUES);

				// En cons�quence, on cr��e un kin, situ� entre kin0 et kin1, � partir de l'abscisse curvuligne de pkp1.
				getKinAtS(&kin, pkin0, pkin1, pkp1->s);
				keystate.set(&kin, pkp1, pprim);
			}
			// B.c) On ins�re le keystate cr�� dans la liste:
			pushed_keystate = (NLTRJPOINT_DESC*)NArrayPushBack(&ppack->m_keyStatesArray, (NBYTE*)&keystate);

		}
		else
		{

			#ifdef _DEBUG
			// Si le Kin en cours est invalide, on sort de la boucle for...
			// Ce kin invalide ne peut �tre que celui d'index 'm_kinsArray.Size'
			NErrorIf(!(kin1_id == m_kinsArray.Size), NERROR_INCONSISTENT_VALUES);

			// Tous les kins, y compris le dernier ( v=0,a=0, ... ) ont �t� ins�r�s sous forme de keystate(s).
			// ... Certes, l'abscisse du dernier kin ( et donc du dernier keystate ) est inf�rieure � l'abscisse du dernier point du chemin !
			// ( sinon on ne passerait pas ici) ... Mais cela est du au cumul des erreurs de calcul et la diff�rence est obligatoirement infime.
			//
			// On verifie que la primitive en cours est bien la derniere du chemin.
			NErrorIf(pprim != (NLPATH_PRIMITIVE*)NGetLastArrayPtr(&ppack->m_pathGeometry.m_primitivesArray), NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkp1 != (NLPATH_POINT*)NGetLastArrayPtr(&ppack->m_pathGeometry.m_pathPointsArray), NERROR_INCONSISTENT_VALUES);
			// On v�rifie �galement que l'erreur sur l'abscisse finale reste tr�s petite.
			// On utilise pkin0 ici car pkin1 est invalide et pkin0 est �gal au dernier pkin1 valide ( cf boucle while plus haut )
			Nf32 final_abscissa_error = pkp1->s - pkin0->m_s;
			// Cette erreur doit �tre positive ! ...
			NErrorIf(final_abscissa_error < 0.0f, NERROR_INCONSISTENT_VALUES);
			// ... et toute petite
			NErrorIf(final_abscissa_error > NLKIN_EPSILON_S, NERROR_INCONSISTENT_VALUES);
			#endif

			// On sort donc de la boucle for
			break;
		}
		// |
		// +------------------------------------------------------------------------------------------------------------------------------------
	}
	if (!pushed_keystate)
		return;

	// Une fois sorti de la boucle for on effectue une derni�re v�rification sur kin1_id.
	// En effet, ce dernier peut-�tre:
	//			a)	Egal � m_kinsArray.Size-1 [ kin1_id == m_kinsArray.Size-1 ] :
	//				Cela signifie que le dernier Kin n'est toujours pas ins�r�. Cela s'explique par les erreurs d'approximation du aux nbres � virgule flottante.
	//				En fait ce dernier kin n'est pas ins�r� car son abscisse est plus grande que l'abscisse du dernier point du chemin. [ voir (B.b) plus haut ].
	//				On ins�re donc tout simplement un dernier keystate issu de ce dernier Kin. C'est imortant de le faire car le dernier Keystate doit contenir le Kin de fin
	//				avec v=0 et a=0. Que l'abscisse de ce dernier kin soit l�g�rement sup�rieure � l'abscisse de fin de chemin est sans cons�quence.
	//				! Attention, Dans ce cas pr�cis pkp1 est hors limite car nous sommes sorti de la boucle for ... le dernier point du chemin � consid�rer est pkp0 !
	if (kin1_id == m_kinsArray.Size - 1)
	{
		NErrorIf(pkin1->m_s <= pkp0->s, NERROR_SYSTEM_GURU_MEDITATION);		// Normalement, comme le kin n'est toujours pas ins�r�, son abscisse ne peut �tre que > � celle de pkp1
		NErrorIf((pkin1->m_s - pkp0->s) > 1e-3f, NERROR_VALUE_OUTOFRANGE);	// Si la diff�rence est trop importante, il y a un probl�me autre que celui potentiellement engendr� par les erreurs d'approximation !

		keystate.set(pkin1, pkp0, pprim);
		pushed_keystate = (NLTRJPOINT_DESC*)NArrayPushBack(&ppack->m_keyStatesArray, (NBYTE*)&keystate);
	}
	#ifdef _DEBUG
	else
	{
		//		b)	Egal � m_kinsArray.Size   [ kin1_id == m_kinsArray.Size	 ] :
		//			Cela signifie que le Dernier Kin a bien �t� ins�r� sous forme d'un keystate, lui-m�me dernier de sa liste.
		//			Pour la bonne forme on verifie juste que ce dernier keystate comprend bien un kin d'arriv�e, c'est � dire un kin avec v=0 et a =0
		//			pkin1 est invalide, comme kin_id. pkin0  correspond normalement au dernier kin de la liste.
		if (kin1_id == m_kinsArray.Size)
		{
			NErrorIf(pkin0 != (NLKIN*)NGetLastArrayPtr(&m_kinsArray), NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_v != pushed_keystate->m_kin.m_v, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_v != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_a != pushed_keystate->m_kin.m_a, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_a != 0.0f, NERROR_INCONSISTENT_VALUES);
			NErrorIf(pkin0->m_s != pushed_keystate->m_kin.m_s, NERROR_INCONSISTENT_VALUES);
		//	Dans cette configuration, on est sur que le dernier kin (et donc le dernier keystate ins�r� dont il est issu ) a une abscisse <= la fin du chemin.
			NErrorIf(pushed_keystate->m_kin.m_s > ppack->m_pathGeometry.m_ds, NERROR_INCONSISTENT_VALUES);
		}
		//		c) Toute autre valeur sera consid�r�e comme un BUG !
		NErrorIf((kin1_id != m_kinsArray.Size) && (kin1_id != (m_kinsArray.Size - 1)), NERROR_INCONSISTENT_VALUES);
	}
	#endif

	// Pour finir on enregistre la "dur�e" du pack
	ppack->m_dt = pushed_keystate->m_kin.m_t;
}
*/
/*
Nf32 NLTRAJECTORY::getMinPathLength(const NLDRIVETRAINSPECS* pdtspecs)
{
	NErrorIf(!pdtspecs, NERROR_NULL_POINTER);
	NLKINLIMITS					local_limits;

	if (ISFLAG_ON(m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS))
	{
		local_limits.m_v = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_VEL) ? m_specificKinLimits.m_v : NMIN(pdtspecs->m_limits.m_v, m_specificKinLimits.m_v);
		local_limits.m_a = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_ACC) ? m_specificKinLimits.m_a : NMIN(pdtspecs->m_limits.m_a, m_specificKinLimits.m_a);
		local_limits.m_j = ISFLAG_ON(m_flags, FLAG_NLTRJ_ENFORCE_JRK) ? m_specificKinLimits.m_j : NMIN(pdtspecs->m_limits.m_j, m_specificKinLimits.m_j);
		local_limits.compute();
	}
	else
	{
		//NErrorIf((m_flags&MASK_NTRJ_ENFORCE), NERROR_INCONSISTENT_FLAGS); // Not an error but it seems awkward to set these flags without  FLAG_NLTRJ_SPECIFIC_KINLIMITS
		local_limits = pdtspecs->m_limits;
	}

	return local_limits.m_2ss;
}
*/

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule la date "t" correspondant � une abscisse curviligne pr�cise
 *
 *	@param	s est l'abscisse curviligne pour laquelle on veut connaitre la date t
 *	@return la date t correspondant la position s sur la trajectoire.
 */
// ------------------------------------------------------------------------------------------
/*
Nf32 NLTRAJECTORY::getTime(const Nf32 s)
{
	if (s <= 0.0f)
	{
		return 0.0f;
	}
	else if( s < m_pack.m_ds )
	{
		NLKIN	*pk0;
		NLKIN	*pk = (NLKIN*)m_kinsArray.pFirst;
		Nf32	p, q, sqrt_dlt;

		while (pk->m_s < s)
			pk++;

		pk0 = pk - 1;
		if ( !pk->m_j )
		{
			if (!pk0->m_a)
			{
			//	NErrorIf(pk->m_a, NERROR_SYSTEM_CHECK);				// impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
			//	NErrorIf(pk0->m_v != pk->m_v, NERROR_SYSTEM_CHECK); // impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...

				NErrorIf(!pk->m_v, NERROR_SYSTEM_CHECK);			// Dans ce cas, la vitesse courante et donc constante ne saurait �tre nulle !
				return pk0->m_t + (s - pk0->m_s) / pk0->m_v;
			}
			else
			{
				// Quelque soit le signe de "pk0->m_a" seule la racine " R2 = (-B + sqrt(delta) )/2A " est applicable � notre situation ( cf etude et tableau de signe )
				return pk0->m_t + (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s))) / pk0->m_a;
			}

			// C'est une phase 2 qui m�ne � pk !
			// On recherche donc une valeur de t solution de l'�quation:
			//	s = pk0->s + pk0->m_v*t + pk0->m_a*t*t/2.0f
			//
			//	On a donc delta = NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) discriminant de l'�quation.
			//  Pour avoir au moins une solution, il faut que delta soit >=0
			//						delta	>=	0
			//	[1]					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0
			//						NPOW2(pk0->m_v) >=	2.0f*pk0->m_a*(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) >=	(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) - pk0->m_s >=	- s
			//	[2]				   -NPOW2(pk0->m_v)/(2.0f*pk0->m_a) + pk0->m_s <=	s
			//
			/ *
			if (pk0->m_a > 0.0f)
			{
				// Il y a TOUJOURS deux racines mais une seule positive !
				// ------------------------------------------------------
				// Dans [1] on voit bien que si pk0->m_a > 0 alors, - 2.0f*pk0->m_a*(pk0->m_s - s)	> 0, car (pk0->m_s - s) < 0 !
				// on a aussi NPOW2(pk0->m_v) >= 0, Donc
				//					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0 est TOUJOURS VRAI
				//
				//						delta  =	NPOW2(pk0->m_v) + qqchose ( avec qqchose = + 2.0f*NABS( pk0->m_a*(pk0->m_s - s) ) )
				//						donc,
				//						delta  >	NPOW2(pk0->m_v)
				// on aura donc TOUJOURS
				//						sqrt(delta) > pk0->m_v
				// donc,
				//						sqrt(delta) - pk0->m_v  > 0
				//
				// Donc,
				//						R2 =  (-pk0->m_v + sqrt(delta) ) / pk0->m_a		SERA TOUJOURS POSITIVE ( avec pk0->m_a positive )
				//
				// R2 = (-pk0->m_v + sqrt(delta) ) / pk0->m_a
				//
				// Quand � R1 = (-pk0->m_v - sqrt(delta) ) / pk0->m_a
				// On voit bien qu'elle sera TOUJOURS n�gative pour les m�mes raisons ,
				// c'est � dire
				//						-pk0->m_v - sqrt(delta) < 0, TOUJOURS ! donc ...
				//
				// Donc on a bien une seule racine positive et donc possible: R+ !
				return (-pk0->m_v + sqrt( NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) ) / pk0->m_a );
			}
			else // pk0->m_a < 0.0f
			{
				// Il est possible que delta soit negatif ou null !
				// ------------------------------------------------
				// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
				// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
				return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)) / pk0->m_a);
			}
			* /
		}
		else if(pk->m_j > 0.0f)
		{
			if(!pk0->m_a)
			{
				p	= (6.0f * pk0->m_v) / pk->m_j;
				q	= (6.0f * (pk0->m_s - s) )/pk->m_j;
				sqrt_dlt = sqrt( NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f );
				return pk0->m_t + ( Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f) );
			}
			else if (!pk->m_a)
			{
				p = (6.0f * pk->m_v) / pk->m_j;
				q = (6.0f * (s - pk->m_s)) / pk->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				return pk->m_t - (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v)/pk->m_j - (3.0f*a02)/j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s))/pk->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
				return pk0->m_t + r - pk0->m_a / pk->m_j;
			}
		}
		else // pk->m_j < 0.0f
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk->m_j;
				q = (6.0f * (pk0->m_s - s)) / pk->m_j;
				return pk0->m_t + 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3);
			}
			else if (!pk->m_a)
			{
				p = (6.0f * pk->m_v) / pk->m_j;
				q = (6.0f * (s - pk->m_s)) / pk->m_j;
				return pk->m_t - 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3);
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v) / pk->m_j - (3.0f*a02) / j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s)) / pk->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3);
				return pk0->m_t + r - pk0->m_a / pk->m_j;
			}
		}
	}
	else // s >= m_pack.m_ds
	{
		return m_pack.m_dt;
	}
}
*/
/*
void NLTRAJECTORY::getKinAtS(NLKIN * pkin, const NLKIN *pk0, const NLKIN *pk1, const Nf32 s_from_trajectory_start)
{
	// le code est tr�s proche de celui de la fonction getTime.
	// C'est normal, puisqu'en fait on commence par trouver "t" localement � partir de "s" pour calculer ensuite les autres param�tres du kin.
	if (s_from_trajectory_start <= pk0->m_s)
	{
		*pkin = *pk0;
		return;
	}
	else if (s_from_trajectory_start < pk1->m_s)
	{
		Nf32	p, q, sqrt_dlt;

		if (!pk1->m_j)
		{
			if (!pk0->m_a)
			{
				//	NErrorIf(pk->m_a, NERROR_SYSTEM_CHECK);				// impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				//	NErrorIf(pk0->m_v != pk->m_v, NERROR_SYSTEM_CHECK); // impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				NErrorIf(!pk1->m_v, NERROR_SYSTEM_CHECK);			// Dans ce cas, la vitesse courante et donc constante ne saurait �tre nulle !
				pkin->from(pk0, (s_from_trajectory_start - pk0->m_s) / pk0->m_v);
				return;
			}
			else
			{
				// Quelque soit le signe de "pk0->m_a" seule la racine " R2 = (-B + sqrt(delta) )/2A " est applicable � notre situation ( cf etude et tableau de signe )
				pkin->from(pk0, (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s_from_trajectory_start))) / pk0->m_a);
				return;
			}

			// C'est une phase 2 qui m�ne � pk !
			// On recherche donc une valeur de t solution de l'�quation:
			//	s = pk0->s + pk0->m_v*t + pk0->m_a*t*t/2.0f
			//
			//	On a donc delta = NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) discriminant de l'�quation.
			//  Pour avoir au moins une soluiton, il faut que delta soit >=0
			//						delta	>=	0
			//	[1]					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0
			//						NPOW2(pk0->m_v) >=	2.0f*pk0->m_a*(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) >=	(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) - pk0->m_s >=	- s
			//	[2]				   -NPOW2(pk0->m_v)/(2.0f*pk0->m_a) + pk0->m_s <=	s
			//
			/ *
			if (pk0->m_a > 0.0f)
			{
				// Il y a TOUJOURS deux racines mais une seule positive !
				// ------------------------------------------------------
				// Dans [1] on voit bien que si pk0->m_a > 0 alors, - 2.0f*pk0->m_a*(pk0->m_s - s)	> 0, car (pk0->m_s - s) < 0 !
				// on a aussi NPOW2(pk0->m_v) >= 0, Donc
				//					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0 est TOUJOURS VRAI
				//
				//						delta  =	NPOW2(pk0->m_v) + qqchose ( avec qqchose = + 2.0f*NABS( pk0->m_a*(pk0->m_s - s) ) )
				//						donc,
				//						delta  >	NPOW2(pk0->m_v)
				// on aura donc TOUJOURS
				//						sqrt(delta) > pk0->m_v
				// donc,
				//						sqrt(delta) - pk0->m_v  > 0
				//
				// Donc,
				//						R2 =  (-pk0->m_v + sqrt(delta) ) / pk0->m_a		SERA TOUJOURS POSITIVE ( avec pk0->m_a positive )
				//
				// R2 = (-pk0->m_v + sqrt(delta) ) / pk0->m_a
				//
				// Quand � R1 = (-pk0->m_v - sqrt(delta) ) / pk0->m_a
				// On voit bien qu'elle sera TOUJOURS n�gative pour les m�mes raisons ,
				// c'est � dire
				//						-pk0->m_v - sqrt(delta) < 0, TOUJOURS ! donc ...
				//
				// Donc on a bien une seule racine positive et donc possible: R+ !
				return (-pk0->m_v + sqrt( NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) ) / pk0->m_a );
			}
			else // pk0->m_a < 0.0f
			{
				// Il est possible que delta soit negatif ou null !
				// ------------------------------------------------
				// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
				// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
				return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)) / pk0->m_a);
			}
			* /
		}
		else if (pk1->m_j > 0.0f)
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk1->m_j;
				q = (6.0f * (pk0->m_s - s_from_trajectory_start)) / pk1->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				pkin->from(pk0, pk1->m_j, (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)));
				return;
			}
			else if (!pk1->m_a)
			{
				p = (6.0f * pk1->m_v) / pk1->m_j;
				q = (6.0f * (s_from_trajectory_start - pk1->m_s)) / pk1->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				pkin->from(pk0, pk1->m_j, pk1->m_t - (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)) - pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk1->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v) / pk1->m_j - (3.0f*a02) / j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk1->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s_from_trajectory_start)) / pk1->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
				pkin->from(pk0, pk1->m_j, r - pk0->m_a / pk1->m_j);
				return;
			}
		}
		else // pk->m_j < 0.0f
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk1->m_j;
				q = (6.0f * (pk0->m_s - s_from_trajectory_start)) / pk1->m_j;
				pkin->from(pk0, pk1->m_j, 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3));
				return;
			}
			else if (!pk1->m_a)
			{
				p = (6.0f * pk1->m_v) / pk1->m_j;
				q = (6.0f * (s_from_trajectory_start - pk1->m_s)) / pk1->m_j;
				pkin->from(pk0, pk1->m_j, pk1->m_t - 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3) - pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk1->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v) / pk1->m_j - (3.0f*a02) / j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk1->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s_from_trajectory_start)) / pk1->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3);
				pkin->from(pk0, pk1->m_j, r - pk0->m_a / pk1->m_j);
				return;
			}
		}
	}
	else // s_from_trajectory_start >= pk1->m_s
	{
		*pkin = *pk1;
		return;
	}
}
*/
/*
void NLTRAJECTORY::getKinAtS(NLKIN * pkin, const Nf32 s_from_trajectory_start)
{
	// le code est tr�s proche de celui de la fonction getTime.
	// C'est normal, puisqu'en fait on commence par trouver "t" localement � partir de "s" pour calculer ensuite les autres param�tres du kin.
	if (s_from_trajectory_start <= 0.0f)
	{
		pkin->null();
		return;
	}
	else if (s_from_trajectory_start < m_pPath->m_geometry.m_ds)
	{
		NLKIN	*pk0;
		NLKIN	*pk = (NLKIN*)m_kinsArray.pFirst;
		//Nf32	p, q, sqrt_dlt;

		while (pk->m_s < s_from_trajectory_start)
			pk++;

		pk0 = pk - 1;
		getKinAtS(pkin, pk0, pk, s_from_trajectory_start);
/ *
		if (!pk->m_j)
		{
			if (!pk0->m_a)
			{
				//	NErrorIf(pk->m_a, NERROR_SYSTEM_CHECK);				// impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				//	NErrorIf(pk0->m_v != pk->m_v, NERROR_SYSTEM_CHECK); // impossible car si 0.m_a est nulle et m_j est null  alors seul le cas ou la vitesse est constante est possible...
				NErrorIf(!pk->m_v, NERROR_SYSTEM_CHECK);			// Dans ce cas, la vitesse courante et donc constante ne saurait �tre nulle !
				pkin->from(pk0, (s_from_trajectory_start - pk0->m_s) / pk0->m_v);
				return;
			}
			else
			{
				// Quelque soit le signe de "pk0->m_a" seule la racine " R2 = (-B + sqrt(delta) )/2A " est applicable � notre situation ( cf etude et tableau de signe )
				pkin->from(pk0, (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s_from_trajectory_start))) / pk0->m_a );
				return;
			}

			// C'est une phase 2 qui m�ne � pk !
			// On recherche donc une valeur de t solution de l'�quation:
			//	s = pk0->s + pk0->m_v*t + pk0->m_a*t*t/2.0f
			//
			//	On a donc delta = NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) discriminant de l'�quation.
			//  Pour avoir au moins une soluiton, il faut que delta soit >=0
			//						delta	>=	0
			//	[1]					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0
			//						NPOW2(pk0->m_v) >=	2.0f*pk0->m_a*(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) >=	(pk0->m_s - s)
			//						NPOW2(pk0->m_v)/(2.0f*pk0->m_a) - pk0->m_s >=	- s
			//	[2]				   -NPOW2(pk0->m_v)/(2.0f*pk0->m_a) + pk0->m_s <=	s
			//
			/ *
			if (pk0->m_a > 0.0f)
			{
				// Il y a TOUJOURS deux racines mais une seule positive !
				// ------------------------------------------------------
				// Dans [1] on voit bien que si pk0->m_a > 0 alors, - 2.0f*pk0->m_a*(pk0->m_s - s)	> 0, car (pk0->m_s - s) < 0 !
				// on a aussi NPOW2(pk0->m_v) >= 0, Donc
				//					NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)	>=	0 est TOUJOURS VRAI
				//
				//						delta  =	NPOW2(pk0->m_v) + qqchose ( avec qqchose = + 2.0f*NABS( pk0->m_a*(pk0->m_s - s) ) )
				//						donc,
				//						delta  >	NPOW2(pk0->m_v)
				// on aura donc TOUJOURS
				//						sqrt(delta) > pk0->m_v
				// donc,
				//						sqrt(delta) - pk0->m_v  > 0
				//
				// Donc,
				//						R2 =  (-pk0->m_v + sqrt(delta) ) / pk0->m_a		SERA TOUJOURS POSITIVE ( avec pk0->m_a positive )
				//
				// R2 = (-pk0->m_v + sqrt(delta) ) / pk0->m_a
				//
				// Quand � R1 = (-pk0->m_v - sqrt(delta) ) / pk0->m_a
				// On voit bien qu'elle sera TOUJOURS n�gative pour les m�mes raisons ,
				// c'est � dire
				//						-pk0->m_v - sqrt(delta) < 0, TOUJOURS ! donc ...
				//
				// Donc on a bien une seule racine positive et donc possible: R+ !
				return (-pk0->m_v + sqrt( NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s) ) / pk0->m_a );
			}
			else // pk0->m_a < 0.0f
			{
				// Il est possible que delta soit negatif ou null !
				// ------------------------------------------------
				// et c'est R- qui est valide ! Note: R+ est �galement positive mais plus grande que R1, elle repr�sente le temps qu'il faut pour REPASSER par S apr�s que la vitesse soit devenue n�gative
				// � force d'appliquer pk0->m_a ! ( et donc on revient sur nos pas pour repasser par S ... )
				return (-pk0->m_v + sqrt(NPOW2(pk0->m_v) - 2.0f*pk0->m_a*(pk0->m_s - s)) / pk0->m_a);
			}
			* /
		}
		else if (pk->m_j > 0.0f)
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk->m_j;
				q = (6.0f * (pk0->m_s - s_from_trajectory_start)) / pk->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				pkin->from(pk0, pk->m_j, (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)));
				return;
			}
			else if (!pk->m_a)
			{
				p = (6.0f * pk->m_v) / pk->m_j;
				q = (6.0f * (s_from_trajectory_start - pk->m_s)) / pk->m_j;
				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				pkin->from(pk0, pk->m_j, pk->m_t - (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f)) - pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v) / pk->m_j - (3.0f*a02) / j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s_from_trajectory_start)) / pk->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = (Ncbrtf32((-q - sqrt_dlt) / 2.0f) + Ncbrtf32((-q + sqrt_dlt) / 2.0f));
				pkin->from(pk0, pk->m_j, r - pk0->m_a / pk->m_j);
				return;
			}
		}
		else // pk->m_j < 0.0f
		{
			if (!pk0->m_a)
			{
				p = (6.0f * pk0->m_v) / pk->m_j;
				q = (6.0f * (pk0->m_s - s_from_trajectory_start)) / pk->m_j;
				pkin->from(pk0, pk->m_j,2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3));
				return;
			}
			else if (!pk->m_a)
			{
				p = (6.0f * pk->m_v) / pk->m_j;
				q = (6.0f * (s_from_trajectory_start - pk->m_s)) / pk->m_j;
				pkin->from(pk0,pk->m_j,pk->m_t - 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3)-pk0->m_t);
				return;
			}
			else // pk0->m_a != 0 ET pk->m_a != 0
			{
				Nf32 j2 = NPOW2(pk->m_j);
				Nf32 a02 = NPOW2(pk0->m_a);
				p = (6.0f*pk0->m_v) / pk->m_j - (3.0f*a02) / j2;
				q = (2.0f*a02*pk0->m_a) / (j2*pk->m_j) - (6.0f*pk0->m_a*pk0->m_v) / j2 + (6.0f*(pk0->m_s - s_from_trajectory_start)) / pk->m_j;

				sqrt_dlt = sqrt(NPOW2(q) + (4.0f*NPOW3(p)) / 27.0f);
				Nf32 r = 2.0f*sqrt(-p / 3.0f) * cos((1.0f / 3.0f)*acos(((3.0f*q) / (2.0f*p))*sqrt(3.0f / -p)) + 4.0f*NF32_PI_3);
				pkin->from(pk0,pk->m_j, r - pk0->m_a / pk->m_j);
				return;
			}
		}
* /
	}
	else // s >= m_ds
	{
		*pkin = *(NLKIN*)NGetLastArrayPtr(&m_kinsArray);
		return;
	}

}
*/
/*
void NLTRAJECTORY::getKinAtT(NLKIN * pkin, const Nf32 t)
{
	// le code est beaucoup plus simple que pour getKinAtS. Ici, pas besoin de trouver 't' il est pass� en param�tre !
	// il s'agit donc juste de calculer les valeurs du Kin en fonction de 't'!
	if (t <= 0.0f)
	{
		pkin->null();
		return;
	}
	else if (t < m_dt)
	{
		NLKIN	*pk0 = (NLKIN*)m_kinsArray.pFirst;
		while (pk0->m_t < t)
			pk0++;
		pk0-=1;
		pkin->from(pk0, t - pk0->m_t);
		return;
	}
	else // t >= m_dt
	{
		*pkin = *(NLKIN*)NGetLastArrayPtr(&m_kinsArray);
		return;
	}
}
*/

/*
Nu32 _trim_uk(NARRAY* puktrim_array, NLVSTAGE* puktrim, NLVSTAGE* pvstrim)
{
	NErrorIf(puktrim_array->ElementSize != sizeof(NLVSTAGE), NERROR_SYSTEM_GURU_MEDITATION);


	NLVSTAGE		trim;
	Nf32				ps0, ps1;
	NVEC2f32			us0, us1;
	//	a)	Le TRIM UserKey est plus bas que le TRIM vstage qui ne peut donc pas le "masquer"
	//




	{
		//	c)	On recherche ps0 et ps1 les abscisses des projections de pvs->m_s0 et pvs->m_s1 sur Trim selon les directions [pvs->m_s0,(pvs-1)->m_s1]
		us0.x = (pvstrim - 1)->m_s1 - pvstrim->m_s0;
		us0.y = (pvstrim - 1)->m_v	- pvstrim->m_v;
		ps0 = us0.y ? pvstrim->m_s0 + (us0.x * (puktrim->m_v - pvstrim->m_v)) / us0.y : pvstrim->m_s0;

		us1.x = (pvstrim + 1)->m_s0 - pvstrim->m_s1;
		us1.y = (pvstrim + 1)->m_v - pvstrim->m_v;
		ps1 = us1.y ? pvstrim->m_s1 + (us1.x * (puktrim->m_v - pvstrim->m_v)) / us1.y : pvstrim->m_s1;

		//	d)	ps0 (projection de pvs->m_s0) est-il � droite de ptrim->m_s1 ?
		if (puktrim->m_s1 <= ps0)
		{
			// Cela signifie que la totalit� du trim est situ� AVANT le "cone d'ombre" de pvs
			NArrayPushBack(puktrim_array, (NBYTE*)puktrim);
			return 0; // on retourne 0 car  ptrim est entierement � gauche du cone. Le Vstage ne peut l'affect�, ni aucun des Vstages suivants qui sont tous encore plus � droite
		}

		//	e)	ps1 (projection de pvs->m_s1) est-il � gauche de ptrim->m_s0 ?
		if (ps1 <= puktrim->m_s0)
		{
			// Cela signifie que la totalit� du trim est situ� APRES le "cone d'ombre" de pvs
			NArrayPushBack(puktrim_array, (NBYTE*)puktrim);
			return 1; // on retourn 1 car ptrim est � droite du cone et peut donc potentiellement �tre affect� par les Vstages suivants.
		}

		//	f)	Il y a intersection entre le cone d'ombre et la gauche de ptrim.
		if (ps0 >= puktrim->m_s0) // rappel: ici on a aussi ps0 < puktrim->m_s1  -> ! (d) n'est pas valid� sinon on ne serait pas ici !
		{
			//  C'est � dire que le d�but de ptrim est avant le Cone d'ombre et qu'ensuite, ptrim entre dans le cone ...
			trim.m_s0 = puktrim->m_s0;
			trim.m_s1 = ps0;
			trim.m_v = puktrim->m_v;
			NArrayPushBack(puktrim_array, (NBYTE*)&trim);
			// Pas de return ici car il reste potentiellement une partie de ptrim qui peut sortir � droite du cone ...
		}

		//	g)	Il y a intersection entre le cone d'ombre et la droite de ptrim.
		if (ps1 <= puktrim->m_s1) // rappel: ici on a aussi ps1 > ptrim->m_s0  -> ! (e) n'est pas valid� sinon on ne serait pas ici !
		{
			//  C'est � dire que la fin de ptrim est apr�s le Cone d'ombre et donc que ptrim est dans le cone et en sort
			trim.m_s0 = ps1;
			trim.m_s1 = puktrim->m_s1;
			trim.m_v = puktrim->m_v;
			NArrayPushBack(puktrim_array, (NBYTE*)&trim);
			return 1; // on retourn 1 car il reste une partie de ptrim � droite du cone et qui peut donc potentiellement �tre affect�e par les Vstages suivants.
		}

		//	h)	ptrim est ENTIEREMENT dans le cone d'ombre.
		//		En fait il n'y a rien � faire ici ptrim est enti�rement masqu� donc on ne 'pousse' rien dans l'array ...
		//		C'est pourquoi on n'effectue mais pas le test...
		/ *
		if ((ps0 <= ptrim->m_s0) && (ps1 >= ptrim->m_s1))
		{

		}
		* /
		return 0;
	}
}
*/
/*
Nu32 _trim_vs(NARRAY* pvstrim_array, NLVSTAGE* pvstrim, NLVSTAGE* puktrim)
{
	NErrorIf(pvstrim_array->ElementSize != sizeof(NLVSTAGE), NERROR_SYSTEM_GURU_MEDITATION);


	NLVSTAGE		trim;



	//	a)	Le VStage TRIM est plus bas que le UserKey TRIM  qui ne peut donc pas le "masquer"
	//
	//
	if (pvstrim->m_v < puktrim->m_v)
	{
		NArrayPushBack(pvstrim_array, (NBYTE*)pvstrim);
		return 1; // on retourn 1 car pvstrim_array  peut potentiellement �tre affect� par les puktrim suivants.
	}
	//	b)	La USERKEY (puktrim ) est "plus bas" que le VSTAGE (pvstrim)  donc il l'oculte, du moins en partie.
	//
	else //(pvstrim->m_v >= puktrim->m_v)
	{
		//	c)	Comme pour '_trim_uk' On recherche ps0 et ps1 les abscisses des projections de puktrim->m_s0 et puktrim->m_s1 sur pvsTrim
		//		... cependant, cette fois, puktrim ne forme pas un 'cone d'ombre' mais un 'tube'. La projection de puktrim->m_s0 est lpuktrim->m_s0, idem pour puktrim->m_s1
		//		... du coup on utilisera directement puktrim->m_s0 et puktrim->m_s1 en lieu et place de ps0 et ps1.
		//
		//		( voir le corps de la fonction  _trim_uk' pour une bonne compr�hension )
		//
		//	d)	puktrim->m_s0 (=ps0) est-il � droite de pvstrim->m_s1 ?
		if (pvstrim->m_s1 <= puktrim->m_s0)
		{
			// Cela signifie que la totalit� de pvstrim est situ� AVANT puktrim
			NArrayPushBack(pvstrim_array, (NBYTE*)pvstrim);
			return 0; // on retourne 0 car  pvstrim est entierement � gauche de puktrim qui ne peut l'affect�, ni aucun des puktrim suivants qui sont tous encore plus � droite.
		}

		//	e)	puktrim->m_s1(=ps1) est-il � gauche de pvstrim->m_s0 ?
		if (puktrim->m_s1 <= pvstrim->m_s0)
		{
			// Cela signifie que la totalit� du pvstrim est situ� APRES le puktrim
			NArrayPushBack(pvstrim_array, (NBYTE*)pvstrim);
			return 1; // on retourn 1 car pvstrim est � droite de puktrim et peut donc potentiellement �tre affect� par les puktrim suivants.
		}

		//	f)	Il y a intersection entre le 'tube' de puktrim et la gauche de pvstrim.
		if (puktrim->m_s0 >= pvstrim->m_s0) // rappel: ici on a aussi puktrim->m_s0 < ptrim->m_s1  -> ! (d) n'est pas valid� sinon on ne serait pas ici !
		{
			//  C'est � dire que le d�but de pvstrim est avant le 'tube' de puktrim et qu'ensuite, pvstrim entre dans le tube ...
			trim.m_s0 = pvstrim->m_s0;
			trim.m_s1 = puktrim->m_s0;
			trim.m_v = pvstrim->m_v;
			NArrayPushBack(pvstrim_array, (NBYTE*)&trim);
			// Pas de return ici car il reste potentiellement une partie de ptrim qui peut sortir � droite du cone ...
		}

		//	g)	Il y a intersection entre le tube de puktrim et la droite de pvstrim.
		if (puktrim->m_s1 <= pvstrim->m_s1) // rappel: ici on a aussi puktrim->m_s1 > ptrim->m_s0  -> ! (e) n'est pas valid� sinon on ne serait pas ici !
		{
			//  C'est � dire que la fin de pvstrim est apr�s le tube de puktrim et donc que pvstrim est dans le tube et en sort
			trim.m_s0 = puktrim->m_s1;
			trim.m_s1 = pvstrim->m_s1;
			trim.m_v = pvstrim->m_v;
			NArrayPushBack(pvstrim_array, (NBYTE*)&trim);
			return 1; // on retourn 1 car il reste une partie de pvstrim � droite du tube et qui peut donc potentiellement �tre affect�e par les puktrim suivants.
		}

		//	h)	pvstrim est ENTIEREMENT dans le 'tube' de puktrim.
		//		En fait il n'y a rien � faire ici, pvstrim est enti�rement masqu� donc on ne 'pousse' rien dans l'array ...
		//		C'est pourquoi on n'effectue mais pas le test...
		/ *
		if ((puktrim->m_s0 <= pvstrim->m_s0) && (puktrim->m_s1 >= pvstrim->m_s1))
		{

		}
		* /
		return 0;
	}
}
*/
/*
void _trim_aggregate(NARRAY* parray, NLVSTAGE* ptrim)
{
	NLVSTAGE* prev = (NLVSTAGE*)NGetLastArrayPtr(parray);
	if (prev && (prev->m_v == ptrim->m_v))
	{
		NErrorIf(ptrim->m_s1 < prev->m_s1, NERROR_SYSTEM_GURU_MEDITATION);
		prev->m_s1 = ptrim->m_s1;
	}
	else
	{
		NArrayPushBack(parray, (NBYTE*)ptrim);
	}
}
*/

/*
Nu32 NLTRAJECTORY::profile(NLKIN * pkfrom, NLKIN * pkto, NLKIN *pbuffer)
{
	return 0;
	/ *
	NErrorIf((!pkfrom) || (!pkto), NERROR_NULL_POINTER);
	NLKIN	*pk0, *pk1;
	Nf32	vp0, vp1, dp0, dp1, dtp0, dtp02, dtp1, dtp12;
	Nf32	f, dv;

	Nf32	vx, ax, jx,signed_jx,signed_ax;
	Nf32	_2va = (ax*ax) / jx;
	Nf32	vb;
	Nf32	dt, dt2;

	Nf32	_2dva,dva,dvv;
	Nf32	tm,tm0,tm1;
	NLKIN	*pkw = pbuffer; // pointeur d'�criture dans le buffer de resultats.
	NLKIN	*pkp = NULL; // pointeur d'�criture pr�c�dent.
	Ns32	next; // permet d'avancer le pointeur d'�criture dans le buffer ( vaut +1 ou -1 en fonction du sens d'�criture, +1 si FORWARD et -1 si BACKWARD)
	Nu32	forward;



	// 0.a) Overshoot ? On peut autoriser le d�passement (overshoot) sur les valeurs vitesse et acceleration en entr�e ( pkfrom ), mais pas en sortie ( pkto )
	//		En effet, on consid�re que les valeurs de sortie sont des valeurs "sous contr�le" du motion profiler, elles ne peuvent donc pas �tre hors contraintes.
	//		par contre, les valeurs d'entr�e peuvent ne pas avoir �t� pr�alablement sous-contr�le, ce qui explique cette "tol�rance".
	if ((pkto->m_v > vx) || (pkto->m_a > ax) || (pkto->m_j > jx))
		return NFALSE;

	// 0.b) R.A.F ? ('R'ien 'A' 'F'aire )
	if ((!NLKIN_V_DISTINCT(pkfrom->m_v, pkto->m_v)) && (!NLKIN_A_DISTINCT(pkfrom->m_a, pkto->m_a))) // Vitesses et accelerations initiales et finales sont confondues ( � EPSILON pr�s )
	{
		// Ecriture dans le buffer r�sultat:
		// Premier Kin
		*pkw = *pkfrom;
		// Second Kin
		pkw++;
		*pkw = *pkfrom; // TODO, ici on assume de forcer le kin destination aux valeurs exactes du KIN de d�part,
						// ... on peut voir � simplement les laisser comme elles sont avec leur "l�g�res" ( <=EPSILON) diff�rences ...
		return 2;
	}

	// 0.c) Toujours dans le m�me sens ...
	// pour faciliter les calculs, premi�re astuce: faire en sorte que le kin cible est la vitesse la plus haute des deux !
	// Si ce n'est pas le cas, on r�soud le motion profile "� l'envers" et pour �viter des copies inutiles on �crit les r�sultats dans le buffer en partant de la fin ... ;)
	if (pkto->m_v < pkfrom->m_v)
	{
		// BACKWARD !
		//next = -1; // pour d�placer le pointeur d'�criture dans le bon sens
		forward = 0;

		// D�finition "interne" du kin de d�part et d'arriv�e ( ici on inverse )
		pk0 = pkto;
		pk1 = pkfrom;

		// Ajustements li�s au "RENVERSEMENT".
		//f = pk0->m_s;
		//pk0->m_s = pk1->m_s;	pk1->m_s = f;
		//pk0->m_v = pk0->m_v;	pk1->m_v = pk1->m_v;
		pk0->m_a = -pk0->m_a;	pk1->m_a = -pk1->m_a;
		//pk0->m_j = -pk0->m_j;	pk1->m_j = -pk1->m_j;
		//pk0->m_t = pk0->m_t;	pk1->m_t = pk1->m_t;	// les temps ne change pas !
	}
	else
	{
		// FORWARD !
		//next = 1; // pour d�placer le pointeur d'�criture dans le bon sens
		forward = 1;

		// D�finition "interne" du kin de d�part et d'arriv�e ( ici on laisse tel quel )
		pk0 = pkfrom;
		pk1 = pkto;
	}

	// !!! VERITE#1:
	//		A PARTIR D'ICI, pk0->m_v est plus petite ou egale � pk1->m_v, dis autrement, nous sommes certains que la vitesse cible est �gale ou plus grande que la vitesse initiale ( consid�r�e comme initiale pour �tre pr�cis).
	//		Et ceci gr�ce au test pr�c�dent et � la mise en place potentielle d'un BACKWARD pour garantir cette VERITE#1

	// 1)	Calcul de 'tm', temps minimum pour passer de pk0->m_a � pk1->m_a et de 'dva' , diff�rentiel de vitesse g�n�r� pendant ce laps de temps.
	//		Dans le cas particulier o� les deux accelerations sont n�gatives ou nulles on calculera 'tm0' et 'tm1' temps necessaires � chaque acceleration pour rejoindre ZERO.
	if ((pk0->m_a > 0.0f) || (pk1->m_a > 0.0f))
	{
		tm	= ( pk1->m_a - pk0->m_a) / jx;
		if (tm < 0)
		{
			tm = -tm;
			signed_jx=-jx;
		}
		else
		{
			signed_jx = jx;
		}

		_2dva = (tm*(pk1->m_a + pk0->m_a)); // on conserve cette valeur qui potentiellement pourra nous �tre utile plus loin. On evite ainsi d'engendrer une "imprecision" en la  divisant par deux, puis en la remultipliant par 2 plus tard.
		dva =  _2dva/ 2.0f;
	}
	else
	{
		// Cas particulier o� les deux acc�l�rations sont n�gatives ou nulles.
		// En effet, dans ce cas on devra repasser par ZERO depuis et vers ces deux valeurs, ce qui change la m�thode de calcul de tm et dva
		tm0  = NABS(pk0->m_a / jx);
		_2dva = pk0->m_a*tm; // = 1/2 * a * t , mais on fera la division par 2 plus tard, pour n'en faire qu'une au lieu de 2 (c'est tjrs ca de gagn� ;) )

		tm1 = NABS(pk1->m_a / jx);
		_2dva = (_2dva + pk1->m_a*f); // on conserve cette valeur qui potentiellement pourra nous �tre utile plus loin. On evite ainsi d'engendrer une "imprecision" en la  divisant par deux, puis en la remultipliant par 2 plus tard.

		dva = _2dva / 2.0f;
		tm	= tm0 + tm1;

		signed_jx = jx; // les deux acc�l�rations sont n�gatives ou nulles, donc ...
	}

	// 2)	Calcul du differentiel de vitesse dvv
	dvv = pk1->m_v - pk0->m_v;

	// 3) Comparaison des deux diff�rentiels de vitesse dvv et dva
	if (!NLKIN_V_DISTINCT(dvv, dva))
	{
		// Si dvv et dva sont "presque" identiques cela revient � dire que "dv = dvv - dva = 0"( ou "presque" )
		// dvv �tant fix�, on va tr�s l�g�rement modifier le JERK pour faire coincider "dva" et "dvv" au maximum ( seule l'erreur inh�rente aux calculs en float subistera.) ...
		// ... et , par l�, tr�s l�g�rement modifier le JERK qui pourra donc �tre tr�s l�g�rement sup�rieur (ou inf�rieur) � la valeur max fix�e ( mais on le tol�re ).
		Nf32 tepsilon = (2.0f*dvv - _2dva) / (pk0->m_a + pk1->m_a);
		tm += tepsilon;
		signed_jx = (pk1->m_a - pk0->m_a) / tm;

		// Il ne reste plus qu'� construire le motion profile associ�!
		if (forward)
		{
			// FORWARD
			*pkw = *pk0;
			pkw++;

			f = tm * tm;
			pk1->m_t = pk0->m_t + tm;
			pk1->m_s = NLKIN_D(pk0->m_s, pk0->m_v,pk0->m_a, signed_jx, tm, f);
			*pkw = *pk1;
		}
		else
		{
			// BACKWARD

			pkw = pbuffer + 1; // on se place sur la derni�re position qui sera utilis�e ( pou ecrire de la fin vers le debut )
			*pkw = *pk0;
			pkw--;

			f = tm * tm;
			pk1->m_t = pk0->m_t + tm;
			pk1->m_s = NLKIN_D(pk0->m_s, pk0->m_v, pk0->m_a, signed_jx, tm, f);
			*pkw = *pk1;
		}
	}
	else if (dvv < dva ) // A�E pour le moment on ne gere pas !
	{
		return 0;
	}
	else // dvv > dva
	{
		dv = dvv - dva;

	}






	// 2) Recherche du "point" d'acceleration nulle le plus proche pour chacun des deux kin. kp0 et kp1
	//kp0.m_t = NABS( pk0->m_a / jx );
	dtp0	= pk0->m_a / jx;									// dtp0	est une dur�e ! Le temps n�c�ssaire pour amener l'acceleration pk0->m_a � ZERO avec un jerk de "jx"
																// dtp0 < 0.0f est possible !!! si pk0->m_a < 0 !!! Et c'est tr�s bien !
	dtp02	= dtp0 * dtp0;
	vp0		= pk0->m_v - (jx*dtp02) / 2.0f;						// = pk0->m_v - (pk0->m_a*pk0->m_a) / (2.0f * jx);
																// ici, le signe de dtp0 ne compte pas puisqu'on utilise son carr� ! vp0 est toujours inf�rieure � pk0->m_v !

	dtp1 = pk1->m_a / jx;										// dtp1	est une dur�e ! Le temps n�c�ssaire pour amener l'acceleration pk1->m_a � ZERO avec un jerk de "jx"
																// dtp1 < 0.0f est possible !!! si pk1->m_a < 0 !!! Et c'est tr�s bien !
	dtp12 = dtp1 * dtp1;
	vp1 = pk1->m_v + (jx*dtp12) / 2.0f;							// = pk1->m_v + (pk1->m_a*pk1->m_a) / (2.0f * jx);
																// ici, le signe de dtp1 ne compte pas puisqu'on utilise son carr� ! vp1 est toujours sup�rieure � pk1->m_v !


	// notes:	v1	> v0 [ cf 1) ],	vp0 < v0 [ cf 2) ] et vp1 > v1 [ cf 2) ] donc
	//			vp1 > vp0
	//
	//dv = vp1 - vp0;
	//vb = dv - _2va;
	//if( dv < dvmin)

	if ( vb < NLKIN_MIN_T*ax  ) // ( NLKIN_MIN_T*ax ) = vb mini prise en compte. En-dessous, on ignore car cela engendrerait un KIN d'une dur�e inf�rieure ( ou �gale) � NLKIN_MIN_T.
	{
		return NFALSE;
	}
	else
	{
		// On se retrouve dans le sch�ma simple d'un motion profile en S.
		// hop hop on construit ! Il se trouve qu'on connait "presque" tous les param�tres du motion profile


		dp0 =  - pk0->m_v*dtp0 - (jx*dtp02*dtp0) / 3.0f;// dp0 est la distance alg�brique entre "pk0" et Kin[vp0,ap0=0,]
																// ici, le signe de dtp0 entre en compte !	Si dtp0 < 0 ( pk0->m_a < 0) alors dp0 > pk0->m_s, le " Kin[vp0,ap0=0,] " est situ� "apr�s" pk0, dans son futur.
																//											Si dtp0 > 0 ( pk0->m_a > 0) alors dp0 < pk0->m_s, le " Kin[vp0,ap0=0,] " est situ� "avant" pk0, dans son pass�.

		dp1 =  pk1->m_v*dtp1 + (jx*dtp12*dtp1) / 3.0f;	// dp1 est la distance alg�brique entre "pk1" et Kin[vp1,ap1=0,]
																	// ici, le signe de dtp1 entre en compte !	Si dtp1 < 0 ( pk1->m_a < 0) alors dp1 < pk1->m_s, le "Kin[vp1,ap1=0,]" est situ� "avant" pk1, dans son pass�.
																	//											Si dtp1 > 0 ( pk1->m_a > 0) alors dp1 > pk1->m_s, le "Kin[vp1,ap1=0,]" est situ� "apr�s" pk1, dans son futur.
		// Kin 0
		*pkw = *pk0;

		pkp = pkw;
		pkw ++;

		// Kin 1
		dt	= (ax - pkp->m_a) / jx;
		dt2 = dt * dt;
		pkw->m_s = pkp->m_s + pkp->m_v * dt + (pkp->m_a*dt2)/2.0f + (jx*dt2*dt) / 6.0f;
		pkw->m_v = pkp->m_v + pkp->m_a * dt + (jx*dt2) / 2.0f;
		pkw->m_a = ax;
		//pkw->m_j = jx;
		pkw->m_t = pkp->m_t + dt;

		pkp = pkw;
		pkw++;

		// Kin 2
		dt	= vb / ax;
		dt2 = dt * dt;
		pkw->m_s = pkp->m_s + pkp->m_v * dt + (pkp->m_a*dt2) / 2.0f;
		pkw->m_v = pkp->m_v + pkp->m_a * dt;// +(jx*dt2) / 2.0f;
		pkw->m_a = ax;
		//pkw->m_j = jx;
		pkw->m_t = pkp->m_t + dt;

		pkp = pkw;
		pkw++;

		// Kin 3
		dt = NABS((ax - pk1->m_a) / jx);
		dt2 = dt * dt;
		pkw->m_s = pkp->m_s + pkp->m_v * dt + (pkp->m_a*dt2) / 2.0f - (jx*dt2*dt) / 6.0f; // -jx !
		pkw->m_v = pkp->m_v + pkp->m_a * dt -(jx*dt2) / 2.0f; // -jx
		pkw->m_a = ax - jx*dt ;
		//pkw->m_j = jx;
		pkw->m_t = pkp->m_t + dt;

		pkp = pkw;
		pkw++;

		// Kin 4
		*pkw = *pk1;
	}


	// End "d�-FLip-age ?"

	return NFALSE;
	* /
}
*/
// Seconde version
// Cr�e un profil de vitesse/Accel. pour passer de l'�tat 0  (pk0) � l'�tat  1 (pk1) de la mani�re la plus optimum ( en minimisant t et d )
// Il n'y a pas de contrainte de distance ou de temps, en cons�quence les valeurs de temps et de distance de pkto seront ajust�es pour atteindre l'objectif fix� (

/*
void NLTRAJECTORY::simpleBuild(const NLPATH * ppath, const NLDifferentialDriveBase *pdrivebase, const NLPHYSICS *pphysics, const Nu32 flags)
{
	// Garde fou:
	if ( (NABS(pdrivebase->m_initialAcceleration) > pdrivebase->m_maxAcceleration) && ISFLAG_OFF(flags, NLTRAJECTORY_SIMPLEBUILD_FLAG_MAX_ACCELERATION_OVERSHOOT_AUTHORIZED) )
	{
		return;
	}

	if ((pdrivebase->m_initialVelocity > pdrivebase->m_maxVelocity) && ISFLAG_OFF(flags, NLTRAJECTORY_SIMPLEBUILD_FLAG_MAX_VELOCITY_OVERSHOOT_AUTHORIZED))
	{
		return;
	}

	// Vitesse de croisi�re maximum
	// 1.4.1) "d" adjust et choix de "ax":
	Nf32 x0, v0, t0, t02, d;
	Nf32 ax; // l'acceleration maximum est normalement celle d�finie dans la classe "pdrivebase". Cependant, il est "posible" que la valeur mesur�e soit sup�rieure. Dans ce cas, " que faire ? "
	Nf32 vx;
	if (pdrivebase->m_initialAcceleration < 0.0f)
	{
		t0 = NABS(pdrivebase->m_initialAcceleration) / pdrivebase->m_maxJerk;															// t, temps n�c�ssaire pour que l'acceleration atteigne une valeur nulle  � partir de l'acceleration initiale.
		t02 = t0 * t0;
		v0 = pdrivebase->m_initialVelocity + pdrivebase->m_initialAcceleration * t0 + (pdrivebase->m_maxJerk*t02) / 2.0f;					// vitesse au temps t ( quand l'acceleration sera nulle )
		x0 = pdrivebase->m_initialVelocity * t0 + (pdrivebase->m_initialAcceleration*t02) / 2.0f + (pdrivebase->m_maxJerk*t02*t0) / 6.0f;	// position au temps t  ( quand l'acceleration sera nulle )

		ax = pdrivebase->m_maxAcceleration; // Pas de question � ce poser. Si l'acceleration initiale est inf�rieure � "-m_maxAcceleration" elle est de toute facon ramen�e � ZERO. Ensuite tout est sous contr�le.
											// ... on fixe alors ax = m_maxAcceleration
		vx = pdrivebase->m_maxVelocity;		// La vitesse initiale diminue jusqu'� V0 ( initial accel < 0 ). la vitesse va ensuite �tre augment�e � partir de V0 pour aller le plus haut possible sur le chemin avant de redescendre � ZERO.
											// Une fois calcul�e, cette vitesse max "hors contrainte" est alors "limit�e" � la vitesse max de la base pour ensuite calculer le Motion Profile.
											// Si la vitesse initiale de la base est sup�rieure � la vitesse max de la base et qu'on limite le motion profile avec la vitesse max de la base, alors, ...
											// La vitesse initiale du robot va reduire jusqu'� v0, le temps que l'acceleration initiale soit ramen�e � ZERO.
											// Si v0 jusqu'� cette vitesse max ! Pour la maintenir aussi longtemps que possible et d�c�l�rer jusqu'� v=0,a=0
	}
	else // (m_initialAcceleration >= 0.0f)
	{																				// on fait la m�me chose que pour le cas pr�c�dent mais ... "� l'envers" c'est � dire dans le pass�.
		t0 = pdrivebase->m_initialAcceleration / pdrivebase->m_maxJerk;				// t, temps n�c�ssaire pour que l'acceleration atteigne l'acceleration initiale depuis une valeur nulle ( temps q'uil a fallu pour que ...)
		t02 = t0 * t0;
		v0 = pdrivebase->m_initialVelocity - (pdrivebase->m_maxJerk*t02) / 2.0f;	// vitesse au temps t, dans le pass� ( quand l'acceleration ETAIT nulle )
		x0 = -v0 * t0 - (pdrivebase->m_maxJerk*t02*t0) / 6.0f;						// position au temps t, dans le pass� ( quand l'acceleration ETAIT nulle )

		ax = NMAX(pdrivebase->m_maxAcceleration, pdrivebase->m_initialAcceleration); // Si l'acceleration initiale est inf�rieure � "-m_maxAcceleration" elle est de toute facon ramen�e � ZERO. Ensuite tout est sous contr�le.
		vx = pdrivebase->m_maxVelocity;
	}

	d = ppath->m_length - x0;	// r�ajustement de la longueur du chemin en prenant en compte x0.
								// x0 > 0 signifie que le chemin consid�r� sera plus court. ce qui est normal car si x0 > 0 c'est que l'acceleration initiale est n�gative, le temps de la ramener � zero, le robot aura continu� � avancer...
								// x0 < 0 signifie que le chemin consid�r� sera plus long. ce qui est normal car si x0 < 0 c'est que l'acceleration initiale est positive, le temps, et la distance qu'il a fallu pour l'amenner � sa valeur actuelle sont pris en compte.
	Nf32 vrx,v0x1,vx1,vx2,_2va,va,vb;
	Nf32 t, t2;
	Nf32 ax2 = pdrivebase->m_maxAcceleration * pdrivebase->m_maxAcceleration;
	Nf32 jx2 = pdrivebase->m_maxJerk * pdrivebase->m_maxJerk;
	Nf32 ta = pdrivebase->m_maxAcceleration / pdrivebase->m_maxJerk;
	Nf32 tc;

	NLPATHPOINT	*ppt;
	NLKIN		*pka,*pkb;
	//NLKIN		firstkin;
	NLKIN		trashkin;

#ifdef _DEBUG
	NLKIN dbg_kin;
#endif
	// Calcul de Tb ( cf MOTION PROFILE B )
	Nf32 tb = (-2.0f*pdrivebase->m_maxJerk*v0 - 3.0f*ax2 + sqrt(2.0f*jx2*v0*v0 + ax2 * ax2 + 4.0f * pdrivebase->m_maxAcceleration * jx2 * d - 2.0f*pdrivebase->m_maxJerk*ax2*v0)) / (2.0f*pdrivebase->m_maxAcceleration*pdrivebase->m_maxJerk);

	// Notes:
	// tb repr�sente le temps d'application de l'acceleration max dans le mod�le g�n�ral: "MOTION PROFILE B".
	// tb est positif implique qu'il est donc possible d'appliquer cette acceleration max, et donc, de l'atteindre.
	// tb = 0 implique qu'il est donc possible d'atteindre l'acceleration max, mais pas de l'appliquer. Il faudra tout de suite appliqu� un JERK negatif pour atteindre une acceleration negative max ( en passant par une acceleration nulle )
	// tb < 0 signifie qu'il n'est pas possible d'atteindre l'acceleration max sur cette distance!
	//		Dans ce cas pr�cis, on va alors rechercher "v0x1" ( cf  "MOTION PROFILE B-") qui repr�sente la vitesse initiale maximum � partir de laquelle il est tout juste possible d'atteindre l'acceleration MAX
	//		pour appliquer ensuite imm�diatement un JERK n�gatif.
	//		Depuis cette vitesse initiale "V0X1" on calcule la vitesse qu'on atteindra quand l'acceleration repassera par ZERO.
	//		En effet on peut dire que, avec une vitesse initiale comprise entre ces deux vitesses, "v0x1" et "vx1" il est possible d'appliquer une p�riode de JERK+, suivi d'une p�riode de JERK-
	//		et qui garantisse qu'on "passe" en 'vx1' avec une acceleration nulle.
	//		Si� la vitesse initiale est sup�rieure � 'vx1' alors il n'est plus du tout possible d'appliquer ce mod�le. On ne peut plus faire croitre l'acc�l�ration, tout au plus on peut la maintenir un temps,
	//		jusqu'� atteindre l'abscisse curviligne maximum o� il faille d�c�l�rer.
	//		Dans ce dernier cas ( vitesse initiale > vx1 ) on recherche 'vx2' la vitesse initiale max. qu'il est possible d'avoir et qui laisse tout juste le temps de d�c�ll�rer  ( d = distance de freinage pour cette vitesse )
	//
	if (tb > 0.0f)
	{
		_2va	= (ax2 / pdrivebase->m_maxJerk);
		va		= _2va / 20.f;
		vb		= pdrivebase->m_maxAcceleration * tb;
		vrx		= v0 + _2va + vb;

		Nf32 verif_vrx = (-ax2 + sqrt(ax2*ax2 + 4 * jx2*pdrivebase->m_maxAcceleration*d + 2.0f*jx2*v0*v0 - 2 * pdrivebase->m_maxJerk*ax2*v0)) / (2.0f*pdrivebase->m_maxJerk); // from MOTION PROFILE B
		NErrorIf( NABS(vrx-verif_vrx)>0.000001f, NERROR_SYSTEM_CHECK); // that means BAD MATHS :(

		// La vitesse max. atteignable sur le chemin est inf�rieure � la vitesse max. de la base
		if (vrx <= pdrivebase->m_maxVelocity)
		{
			m_maxCruiseVelocity = vrx;	// ... la vitesse de croisiere max. est donc �gale � la vitesse max. atteignable sur le chemin

			if (pdrivebase->m_initialAcceleration < 0.0f)
			{
				// Dans ce cas, le v0 calcul� est 'apr�s' le debut du chemin, quand l'acceleration arrive � ZERO.
				// V0 est obligatoirement < m_maxCruiseVelocity car tb > 0.0f et v0 < initialvelocity ( et oui vu que initialaccel < 0, la vitesse initiale decroit jusqu'� v0 )

				//	Motion profile B/ Etat initial	t0	---------------------------------------------------------------------#
				pka = (NLKIN*)NArrayAllocBack(&m_stateArray);
				pka->set(0,pdrivebase->m_initialVelocity,pdrivebase->m_initialAcceleration, pdrivebase->m_maxJerk, 0.0f);	// 	Definition de la vitesse et de l'acceleration initiale ...
																															//	v = initial velocity / a = initial acceleration
				//	Motion profile B/PHASE 1		t0 --->t1	-------------------------------------------------------------#
				pkb = (NLKIN*)NArrayAllocBack(&m_stateArray);
				t = t0 + ta; t2 = t * t;																					//	Calcul du 2e etat kinetic en r�-utilisant certaines valeurs d�j� calcul�es ( optim.)
				pkb->m_s = NLKIN_S(0.0, pdrivebase->m_initialVelocity, pdrivebase->m_initialAcceleration, pdrivebase->m_maxJerk, t, t2);
				pkb->m_v = v0 + va;																							//	v = v0 + va
				pkb->m_a = pdrivebase->m_maxAcceleration;																	//	a = max
				pkb->m_j = 0.0f;
				pkb->m_t = t;	// = pka->m_t + t; avec "pka->m_t = 0.0f"

				#ifdef _DEBUG
				dbg_kin.integrate(pka, t0 + ta, 0.0f);																		//	t0 + ta ?
				NErrorIf(!NLKIN_S_DISTINCT(dbg_kin.m_s, pkb->m_s), NERROR_SYSTEM_CHECK);									//	soit integration d'un J+ sur 't0'  pour atteindre accel.=0, et J+ sur 'ta' pour atteindre accel.=max
				NErrorIf(!NLKIN_V_DISTINCT(dbg_kin.m_v, pkb->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_A_DISTINCT(dbg_kin.m_a, pkb->m_a), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_T_DISTINCT(dbg_kin.m_t, pkb->m_t), NERROR_SYSTEM_CHECK);
				#endif

				//	Motion profile B/PHASE 2		t1 --->t2	-------------------------------------------------------------#
				pka = (NLKIN*)NArrayAllocBack(&m_stateArray);
				t2 = tb * tb;																								//	Calcul du 3e etat kinetic en r�-utilisant certaines valeurs d�j� calcul�es ( optim.)
				pka->m_s = NLKIN_S(pkb->m_s, pkb->m_v, pkb->m_a, pkb->m_j, tb, t2);
				pka->m_v = vrx - va;																						//	v = v0 + va + vb = vrx - va
				pka->m_a = pdrivebase->m_maxAcceleration;																	//	a = max
				pka->m_j = -pdrivebase->m_maxJerk;
				pka->m_t = pkb->m_t + tb;

				#ifdef _DEBUG
				dbg_kin.integrate(pkb, tb, -pdrivebase->m_maxJerk);															// 	soit l'integration de amax (constante, avec J=0) sur une dur�e tb.
				NErrorIf(!NLKIN_S_DISTINCT(dbg_kin.m_s, pka->m_s), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_V_DISTINCT(dbg_kin.m_v,	pka->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_V_DISTINCT(v0+va+vb,	pka->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_A_DISTINCT(dbg_kin.m_a, pka->m_a), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_T_DISTINCT(dbg_kin.m_t, pka->m_t), NERROR_SYSTEM_CHECK);
				#endif

				//	Motion profile B/PHASE 3 + PHASE 5		t2 --->t3/t4 --->t5 (pas de phase 4) 	-------------------------#
				pkb = (NLKIN*)NArrayAllocBack(&m_stateArray);																//	Calcul du 4e etat kinetic par integration de l'etat 3 sur une dur�e de 2 * ta
				t = 2.0f*ta; t2 = t * t;
				pkb->m_s = NLKIN_S(pka->m_s, pka->m_v,pka->m_a,pka->m_j, t, t2);
				pkb->m_v = vrx - va;																						//	v = v0 + va + vb + va - va = vrx - va
				pkb->m_a = -pdrivebase->m_maxAcceleration;																	//	a = -max
				pkb->m_j = 0.0f;
				pkb->m_t = pka->m_t + t;

				#ifdef _DEBUG
				dbg_kin.integrate(pka, 2.0f*ta, 0.0f);																		//	ta pour passer l'accel. de max � 0, et ta pour passer l'accel de 0 � -max.
				NErrorIf(!NLKIN_S_DISTINCT(dbg_kin.m_s, pkb->m_s), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_V_DISTINCT(dbg_kin.m_v, pkb->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_A_DISTINCT(dbg_kin.m_a, pkb->m_a), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_T_DISTINCT(dbg_kin.m_t, pkb->m_t), NERROR_SYSTEM_CHECK);
				#endif

				//	Motion profile B/PHASE 6		t5 --->t6	-------------------------------------------------------------#
				tc = (v0 + vb) / pdrivebase->m_maxAcceleration;
				pka = (NLKIN*)NArrayAllocBack(&m_stateArray);																// 	Calcul du 5e etat kinetic par integration de l'etat 4 sur une dur�e de tc
				t2 = tc * tc;																								//	Calcul du 3e etat kinetic en r�-utilisant certaines valeurs d�j� calcul�es ( optim.)
				pka->m_s = NLKIN_S(pkb->m_s, pkb->m_v, pkb->m_a, pkb->m_j, tc, t2);
				pka->m_v = va;																								//	v = va
				pka->m_a = -pdrivebase->m_maxAcceleration;																	//	a = -max
				pka->m_j = pdrivebase->m_maxJerk;
				pka->m_t = pkb->m_t + tc;

				#ifdef _DEBUG
				dbg_kin.integrate(pkb, tc, pdrivebase->m_maxJerk);																//	tc, temps n�c�ssaire pour passer d'une vitesse de "va+v0+vb" � une vitesse de "va" avec la deceleration max
				NErrorIf(!NLKIN_S_DISTINCT(dbg_kin.m_s, pka->m_s), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_V_DISTINCT(dbg_kin.m_v, pka->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_A_DISTINCT(dbg_kin.m_a, pka->m_a), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_T_DISTINCT(dbg_kin.m_t, pka->m_t), NERROR_SYSTEM_CHECK);
				#endif
																															//	Motion profile B/PHASE 7		t6 --->t7
				pkb = (NLKIN*)NArrayAllocBack(&m_stateArray);																//	calcul du 6e etat kinetic par integration de l'etat 5 sur une dur�e de ta
				t2 = ta * ta;																								//	Calcul du 2e etat kinetic en r�-utilisant certaines valeurs d�j� calcul�es ( optim.)
				pkb->m_s = NLKIN_S(pka->m_s, pka->m_v, pka->m_a, pka->m_j, ta, t2);
				pkb->m_v = 0.0f;																							//	v = 0
				pkb->m_a = 0.0f;																							//	a = 0
				pkb->m_j = 0.0f;
				pkb->m_t = pka->m_t + ta;

				#ifdef _DEBUG
				dbg_kin.integrate(pka, ta, 0.0f);																			//	v = 0 / a = 0
				NErrorIf(!NLKIN_S_DISTINCT(dbg_kin.m_s, pkb->m_s), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_V_DISTINCT(dbg_kin.m_v, pkb->m_v), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_A_DISTINCT(dbg_kin.m_a, pkb->m_a), NERROR_SYSTEM_CHECK);
				NErrorIf(!NLKIN_T_DISTINCT(dbg_kin.m_t, pkb->m_t), NERROR_SYSTEM_CHECK);
				#endif
			}
			else
			{
				// dans ce cas, le v0 calcul� est 'avant' le debut du chemin, quand l'acceleration "�tait" � ZERO.
				// On check v0 contre m_maxCruiseVelocity pour d�finir la strat�gie


			}
		}
		// C'est l'inverse, la vitesse max. qu'il serait possible d'atteindre sur ce chemin est sup�rieure � la vitesse max. de la base ...
		// Il ne sera donc pas possible d'atteindre la vitesse max. que la longueur du chemin permettrait "th�oriquement" d'atteindre ...
		else
		{
			m_maxCruiseVelocity = pdrivebase->m_maxVelocity;	// ... la vitesse de croisiere max. est donc �gale � la vitesse max. de la base

		}

	}
	else if (tb == 0.0f)
	{
		vrx = v0 + (ax2 / pdrivebase->m_maxJerk);
	}
	else // tb < 0
	{
		v0x1 = sqrt((33.0f*ax2*ax2) / (4.0f*jx2) + 2.0f*pdrivebase->m_maxAcceleration*d) - (7.0f*ax2) / (2.0f*pdrivebase->m_maxJerk);  // from MOTION PROFILE B-
		NErrorIf(v0 < v0x1, NERROR_SYSTEM_CHECK); // that means BAD MATHS :(
		vx1 = v0x1 + ax2 / pdrivebase->m_maxJerk; // vx1 = v0x1 + va + va = v0x1 + 1/2at + 1/2 at = v0x1 + at = v0x1 + a�/j ( avec t = a/j)
		if (v0 <= vx1) //  v0x1 < v0 <= vx1. Il est possible d'acc�l�rer un poil jusqu'� atteindre "vx1" puis d�c�lerer jusqu'� la fin.
			vrx = vx1;
		else // l'�tat kinetic initial ne permet pas d'accelerer plus
			vrx = vx2 = (-ax2 + sqrt(ax2*ax2 + 8.0f*pdrivebase->m_maxAcceleration*jx2*d)) / (2.0f*pdrivebase->m_maxJerk); // from MOTION PROFILE D
	}

	//ppt = (NLPATHPOINT*)ppath->m_pathPointArray.pFirst;


	m_maxCruiseVelocity = NMIN(vrx, pdrivebase->m_maxVelocity);







/ *
	// 1.0) Pre calculating some data
	// -----------------------------------------------------------------------------------------------------------------------------------------------------------
	// Compute the inertia coefficient used to figure out local velocity limit in section 1.3.7.b) and 1.3.7.c)
	// Calcul le "coefficient d'inertie" utilis� pour ensuite d�finir la vitesse max.
	Nf32 inertia_coef;
	if (pdrivebase->m_centerOfMassY)
	{
		inertia_coef = NMIN(pphysics->m_stictionCoefficient, pdrivebase->m_halfAxleTrack / pdrivebase->m_centerOfMassY) * pphysics->m_gravity;
	}
	else
	{
		inertia_coef = pphysics->m_stictionCoefficient * pphysics->m_gravity;
	}

	Nf32 fradius, turn_velocity_limit,inertia_velocity_limit,localvelocitylimit;
	NLPATHPOINT* ppt = (NLPATHPOINT*)ppath->m_pathPointArray.pFirst;
	for (Nu32 i = 0; i < ppath->m_pathPointArray.Size; i++, ppt++)
	{
		fradius = NABS(ppt->m_curvatureRadius);
		turn_velocity_limit		= pdrivebase->m_maxVelocity * (1.0f + (fradius - pdrivebase->m_halfAxleTrack) / (fradius + pdrivebase->m_halfAxleTrack))*0.5f;
		inertia_velocity_limit	= sqrt(inertia_coef * fradius);
		localvelocitylimit		= NMIN( m_maxCruiseVelocity, NMIN(turn_velocity_limit, inertia_velocity_limit) );
	}
* /
}
*/