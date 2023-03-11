#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../N/NCStandard.h"
#include "../../N/UI/NUI.h"
#include "../../N/NCamera.h"
#include "../../N/Maths/NVec3f32.h"
#include "../../N/NCoordinateSystemConversions.h"
#include "../../N/NEditor.h"
#include "../../N/Utilities/NUT_X.h"
#include "../../N/Utilities/NUT_3DMouse.h"
#include "../../N/Utilities/Maths/NUT_MathsMisc.h"
#include "../../N/Utilities/Draw/NUT_Draw.h"
#include "../../N/Utilities/Draw/NUT_DrawPencil.h"
#include "../../N/Event/NEventTouch.h"
#include "../../N/Maths/NMatrix3x3.h"
#include "../../N/Maths/NVecLimits.h"
#include "../../N/NMath.h"

#include"../../NL/MotionControl/Trajectory/NLVstage.h"
#include "../../NL/MotionControl/NLPathWorkbench.h"

#include "../../NL/NL2DOrthogonalCoordinateSystem.h"

#include "../../NL/MotionControl/Trajectory/NLTrajectoryKey.h"

#include "../App_Init.h"
#include "App_TrajectoryKey_Tools.h"

extern HWND hwnd_Explorer;


APP_TRJKEY_TOOLDATA::APP_TRJKEY_TOOLDATA()
{
	Nmem0(this, APP_TRJKEY_TOOLDATA);
	m_SnapThreshold = DEFAULT_APP_USERKEY_SNAP_S_THRESHOLD;
	m_keySpacing	= DEFAULT_APP_USERKEY_SPACING;
}

APP_TRJKEY_TOOLDATA::APP_TRJKEY_TOOLDATA(NLPATH_WORKBENCH* pwb)
{
	Nmem0(this, APP_TRJKEY_TOOLDATA);
	m_pWorkbench = pwb;
	m_SnapThreshold = DEFAULT_APP_USERKEY_SNAP_S_THRESHOLD;
	m_keySpacing = DEFAULT_APP_USERKEY_SPACING;
}

APP_TRJKEY_TOOLDATA::~APP_TRJKEY_TOOLDATA()
{
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Construit un candidat UserKey � partir d'une abscisse S.
 *			Les UserKeys sont rang�es dans un array selon l'ordre croissant de leur abscisses 
 *			ET selon certainnes r�gles de placement ( les Keys ne doivent pas �tre trop proches l'une de l'autre )
 *			Ajouter un nouvelle userkey (avec une nouvelle abscisse S ) ou d�placer une UserKey sur le chemin ( et donc changer son abscisse )
 *			sont des actions qui vont venir perturber l'ordre de l'Array et qui vont potentiellement n�c�ssiter quelques ajustement de l'abscisse
 *			nouvelle ou � modifier, pour qu'elle respecte les r�gles de proximit�.
 *			La fonction submit fait ce travail en renseignant chaque memebre de la structure APP_TRJKEY_TOOLDATA_KEY_CANDIDATE
 *						
 *					m_s				valeur de s en entr�e de fonction, potentiellement ajust�e pour respecter les contraintes de proximit� avec les autres Keys de l'array
 *					m_TrjK1Index	index actuel de la key qui va se trouver juste apr�s la Key qui portera l'abscisse m_s. 	
 * 					m_pTrjK1		adresse correspondant � m_TrjK1Index
 * 
 *			De plus elle retourne un rapport sous la forme d'un ensemble de FLAGS compact�s sur un 32 bits.
 * 
 *	@param	pkc	est un pointeur sur la structure APP_TRJKEY_TOOLDATA_KEY_CANDIDATE � remplir.
 *	
 *	@param	s est l'abscisse � consid�rer.
 * 
 *	@param	pmasked est un pointeur sur une key de l'array des keys. pmasked sera ignor�e par la fonction lors des recherches de placements.
 *			pratiquement, pkmasked est la clef dont on veut modifier l'abscisse. pkmasked peut-�tre nulle, ce qui est le cas lorsqu'on veut ins�rer une nouvelle
 *			clef d'abscisse s.
 *
 *	@return Un rapport de ce qui a �t� d�tect� (IN)  et de ce qui a �t� modifi� (OUT). Le flag ..._SUBMIT_OUT_VALID indique plus particulierement si le contenu de pkc
 *			est utilisable pour cr�er/modifier une key.
 * 
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID					La clef candidate est utilisable pour cr�er/modifier une clef. !! Ce flag est OFF dans le cas contraire !! 
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED				La clef candidate est ajust�e pour prendre en compte les crit�res/contraintes en cours.
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_NEXT_KEY	L'abscisse en entr�e est trop proche de la clef suivante (et d�j� en place).
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_PREV_KEY	L'abscisse en entr�e est trop proche de la clef pr�c�dente (et d�j� en place).
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_MERGED					L'abscisse en entr�e est confondue(=superpos�e) avec celle d'une clef d�j� en place.
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_BEFORE_START				L'abscisse en entr�e est situ�e avant l'abscisse de la premi�re clef ( donc s  <= 0.0f ! )
 *			FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_AFTER_END				L'abscisse en entr�e est situ�e apr�s l'abscisse de la derni�re clef ( donc s >= NF32MAX ! )
 *
 */
 // ------------------------------------------------------------------------------------------
Nu32 APP_TRJKEY_TOOLDATA::submit(APP_TRJKEY_TOOLDATA_KEY_CANDIDATE* pkc, const Nf32 s, const NLTRJKEY* pkmasked)
{
	// BIT_0			left
	// BIT_1			right
	// BIT_0|BIT_1		full

	Nu32	ret = 0;
	Nf32	ds;
	Nf32	sx;

	NARRAY* pkarr = m_pWorkbench->getTrajectoryKeysArray();

	// +-------------------------------------------------------------------------
	// |
	// |
	// | Recherche de pk0,pk1 tels que: pk0->s < s < pk1->s
	// |
	// |
	NLTRJKEY* pk0;
	NLTRJKEY* pk1 = (NLTRJKEY*)NGetLastArrayPtr(pkarr);
	Nu32 idk1;

	NErrorIf(pkmasked && (pkmasked >= pk1), NERROR_UNAUTHORIZED_VALUE);
	NErrorIf(pkmasked && (pkmasked <= (NLTRJKEY*)pkarr->pFirst), NERROR_UNAUTHORIZED_VALUE);

	// Recherche de:
	//					'sx' image de 's' contrainte '� l'int�rieur' du chemin
	//					'pk0', key d�j� existante dont l'abscisse 'm_s' sera situ�e juste avant 'sx'.
	//					'pk1', key d�j� existante dont l'abscisse 'm_s' sera situ�e juste apr�s 'sx'.
	//
	// s est + grand que l'abscisse de la derniere clef de l'array ... 
	// la derniere Key de l'array, peut avoir une abscisse = NF32MAX !!!! ou ( le plus souvent ) egale � la lngueur du chemin.
	if (s >= pk1->m_s)
	{
		// on place sx SUR la derni�re key (temporairement, elle sera d�cal�e quelques lignes plus bas)
		sx		= pk1->m_s;
		idk1	= pkarr->Size - 1;
		pk0		= pk1 - 1;
		if (pk0 == pkmasked) // la NLTRJKEY situ�e juste avant s EST la key ghost ( c'est � dire la clef actuellement select . ! )
			pk0--;
		ret = FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_AFTER_END;
	}
	// s est + petit que l'abscisse de la premiere clef de l'array ...
	else if (s <= 0.0f)
	{
		pk0		= (NLTRJKEY*)pkarr->pFirst;
		pk1		= pk0 + 1;
		if (pk1 == pkmasked) // la NLTRJKEY situ�e juste apr�s s EST la key masqu�e ( c'est � dire la clef actuellement select . ! )
		{
			pk1++;
			idk1 = 2;
		}
		else
		{
			idk1 = 1;
		}

		// sx est situ�e avant le d�but du chemin ( avant la premiere clef ) il faut donc la ramener "SUR le chemin" 
		// ... au minimum sa premi�re position "possible" serait "= 0" mais du coup elle serait confondue avec la premi�re clef du chemin.
		// ce qui n'est pas souhait� non plus. Donc on d�cale encore plus � droite. Pour respecter au mieux les r�gles on va 
		// quantifier ce 'glissement' de sx en regardant la distance existante entre pk1 et pk0, les 2 clefs qui entoureront sx.
		// pk0 �tant la premi�re clef du chemin ...
		// On a pk0->s = 0. Donc, dans ce cas pr�cis, l'intervalle situ� entre pk0 et pk1 � une longueur de "pk1->s".
		// On placera SX au plus pr�s possible du d�but du chemin, soit la distance minimum possible qui est "m_keySpacing"
		// On v�rifiera bien s�r que cela est possible et au cas o� on se placera � une distance inf�rieure correspondant � la moiti� de l'intervalle
		// [pk0->s, pk1->s]
		ds = pk1->m_s / 2.0f;
		sx = NMIN(ds, m_keySpacing);
		ret = FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_BEFORE_START;
	}
	// s se situe entre les abscisses de la premiere et de la derniere clef de l'array ...
	else
	{
		pk1		= (NLTRJKEY*)pkarr->pFirst;
		idk1	= 0;
		while ( (pk1->m_s < s) || (pk1== pkmasked) ) { pk1++; idk1++; } // recherche de pk1 la clef situ�e juste apr�s s ET juste apr�s pkmasked !
		NErrorIf(pk1 > (NLTRJKEY*)NGetLastArrayPtr(pkarr), NERROR_SYSTEM_CHECK); // Normalement impossible car d�s le d�part de la fonction s est ramen� entre les abscisses de la premiere et derniere clef.
		pk0		= pk1 - 1;
		if (pk0 == pkmasked) // la NLTRJKEY situ�e juste avant 'S' EST la key masqu�e ( c'est � dire la clef actuellement select . ! )
			pk0--;
		sx	= s;
	}
	NErrorIf(pk1 > (NLTRJKEY*)NGetLastArrayPtr(pkarr), NERROR_SYSTEM_CHECK); // Normalement impossible car d�s le d�part de la fonction s est ramen� entre les abscisses de la premiere et derniere clef.
	NErrorIf(pk0 < (NLTRJKEY*)NGetFirstArrayPtr(pkarr), NERROR_SYSTEM_CHECK); // Normalement impossible car d�s le d�part de la fonction s est ramen� entre les abscisses de la premiere et derniere clef.

	// CHECK KEY confondues:
	// Si sx se trouve �tre confonue avec l'abscisse de pk1 alors on le "repousse" en direction de pk0
	if (sx == pk1->m_s)
	{
		if( ret != FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_AFTER_END )
			ret |= FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_MERGED;
		
		ds = (pk1->m_s - pk0->m_s)/2.0f;
		sx -= NMIN(ds, m_keySpacing);
	}

	pkc->m_TrjK1Index = idk1;
	pkc->m_pTrjK1	  = pk1;


	// Ici nous sommes s�rs que l'abscisse sx ( ajust�e ou pas, c'est � dire �gale � s ou pas ) est comprise entre l'abscisse de la premiere et de la derniere clef de l'array.
	// Nous sommes egalement s�r que sx n'est pas confondue avec l'abscisse d'une autre clef.
	// 
	// et nous sommes donc s�rs de :
	//											pk0->s < sx < pk1->s
	// 
	// Pour terminer compl�tement l'�valuation il nous reste � �valuer si sx n'est pas trop pr�s de pk0 et/ou de pk1
	// ... et bien s�r, � stocker la position ajust�e retenue dans le le memnbre m_s du key candidate.
	// 
	NErrorIf(pk0 == pk1, NERROR_SYSTEM_GURU_MEDITATION);
	ds = pk1->m_s - pk0->m_s;
	if (ds < 2.0f * m_keySpacing)
	{
		pkc->m_s = (pk0->m_s + pk1->m_s) / 2.0f;
		ret |=	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_PREV_KEY |
				FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_NEXT_KEY |
				FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED;
	}
	else if (ds == 2.0f * m_keySpacing)
	{
		pkc->m_s = pk0->m_s + m_keySpacing;
		ret |=	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED |
				FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID;
	}
	else // ds > 2.0f * m_keySpacing
	{
		// sx est trop proche de pk0
		if (sx - pk0->m_s < m_keySpacing)
		{
			pkc->m_s = pk0->m_s + m_keySpacing;
			ret |=	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_PREV_KEY |
					FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED |
					FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID;
		}
		// sx est trop proche de pk1
		else if (pk1->m_s - sx < m_keySpacing)
		{
			pkc->m_s = pk1->m_s - m_keySpacing;
			ret |=	FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_IN_TOO_CLOSE_TO_NEXT_KEY |
					FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_ADJUSTED |
					FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID;
		}
		// sx est ... bien
		else
		{
			pkc->m_s = sx;
			ret |= FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID;
		}
	}
	return ret;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Modifie l'abscisse s de la clef pkey et la d�place dans l'array afin que l'ordre
 *			des clefs ( dans cet array ) suive l'ordre de leur abscisses respectives.
 *
 *	@param	pkey est la clef dont on veut modifier l'abscisse.
 *			Note: son abscisse n'est pas encore modifi�e !
 * 
 * 	@param	pcandidate est un pointeur sur une structure APP_TRJKEY_TOOLDATA_KEY_CANDIDATE contenant les informations
 *			n�c�ssaires au changement d'abscisse ( et de place dans l'array) de la clef pkey.
 *			pcandidate contient, entre autre: 
 *						la future abscisse 's' qui sera attribu�e � pkey.
 *						l'index et l'adresse de la clef dont l'abscisse est situ�e juste apr�s la future abscisse 's'.
 *						( apr�s le "sort", la nouvelle adresse de pkey sera situ�e juste avant celle-ci )
 * @return	La nouvelle adresse de pkey, potentiellement diff�rente de celle pass�e en entr�e de fonction.
 *			En effet, la clef � �t� potentiellement d�plac�e dans l'array ( et � donc chang� d'adresse ) 
 *			pour respecter l'ordre des abscisses.
 */
 // ------------------------------------------------------------------------------------------
NLTRJKEY* APP_TRJKEY_TOOLDATA::setSandSort(NLTRJKEY* pkey, APP_TRJKEY_TOOLDATA_KEY_CANDIDATE* pcandidate)
{
	NLTRJKEY	key;
	Nu32		size = 0;

	pkey->setS(pcandidate->m_s);

	// IL Y A 3 CAS POSSIBLES concernant la coh�rence de la position (adresse) de pkey dans l'array 
	// 
	//		L'abscisse "pcandidate->s" ( futur S de pkey ) est situ�e dans l'intervalle [ (pcandidate->m_pTrjK1-1)->s, (pcandidate->m_pTrjK1)->s ]
	//		CAS #1 : pkey, dont l'abscisse S n'est pas encore modifi�e est actuellement situ�e AVANT "pcandidate->m_pTrjK1-1"
	//		CAS #2 : pkey, dont l'abscisse S n'est pas encore modifi�e est actuellement situ�e APRES "pcandidate->m_pTrjK1-1"
	//  
	// Si la key [m_pTrjK1-1] situ�e juste avant la key [m_pTrjK1] situ�e apr�s l'abscisse S de pcandidate' est plus GRANDE que l'abscisse actuelle (non-modifi�e) de pkey
	// on est dans cette situation:
	//										on sait que :	l'abscisse de 'pcandidate->m_pTrjK1'	est situ�e  APRES la future abscisse S. 
	//														l'abscisse de 'pcandidate->m_pTrjK1-1'	est situ�e  AVANT la future abscisse S.							
	// 
	//					S actuel															pcandidate->S
	//					|																				|	
	//					|														 <--					|		 -->
	// 0 ...............x...................s...................................s.......................x...........s................ +
	//					|					|									|									|
	//				  pkey			pcandidate->m_pTrjK1-n			pcandidate->m_pTrjK1-1				pcandidate->m_pTrjK1
	//
	if ((pcandidate->m_pTrjK1 - 1) > pkey)
	{
		key = *pkey;
		size = pcandidate->m_pTrjK1 - 1 - pkey;
		memcpy(pkey, pkey + 1, sizeof(NLTRJKEY) * size );
		*(pcandidate->m_pTrjK1 - 1) = key;
		return pcandidate->m_pTrjK1 - 1;
	}
	// 
	// Si la key [m_pTrjK1-1] situ�e juste avant la key [m_pTrjK1] situ�e apr�s l'abscisse S de pcandidate' est plus PETITE que l'abscisse actuelle (non-modifi�e) de pkey
	// on est dans cette situation:
	//										on sait que :	l'abscisse de 'pcandidate->m_pTrjK1'	est situ�e  APRES la future abscisse S. 
	//														l'abscisse de 'pcandidate->m_pTrjK1-1'	est situ�e  AVANT la future abscisse S.							
	// 
	// En fait pkey, qui est donc APRES [m_pTrjK1-1] est soit EGAL � [m_pTrjK1] soit PLUS GRAND. Elle ne peut pas en effet �tre avant [m_pTrjK1] ET apr�s [m_pTrjK1-1] ...
	// Ci-dessous, pkey situ�e APRES:
	//																				pcandidate->S							S actuel			
	//																							|							|		
	//																	 <--					|		 -->				|		
	// 0 ...........................s...................................s.......................x...........s...............x........ +
	//								|									|									|				|	
	//						pcandidate->m_pTrjK1-n			pcandidate->m_pTrjK1-1				pcandidate->m_pTrjK1	   pkey	
	//
	// 
	//
	// Techniquement, pKey ne saurait �tre �gale � [m_pTrjK1] car pcandidate est normalement construit par la fonction Submit ( voir plus haut ) qui occulte la clef s�lectionn�e (pkmasked)
	// c'est � dire pkey. Cependant, l'algo ci-dessous fonctionne dans ce cas. puisqu'il calcul une taille de 0 (ZERO ) pour le bloc � copier.
	// 
	//																				pcandidate->S			S actuel							
	//																							|			|						
	//																	 <--					|		 -->|						
	// 0 ...........................s...................................s.......................x...........s........................ +
	//								|									|									|					
	//						pcandidate->m_pTrjK1-n			pcandidate->m_pTrjK1-1				pcandidate->m_pTrjK1	   
	//																										|
	//																									  pkey			
	else if ((pcandidate->m_pTrjK1 - 1) < pkey)
	{
		key = *pkey;
		size = pkey - pcandidate->m_pTrjK1;
		memcpy(pcandidate->m_pTrjK1 + 1, pcandidate->m_pTrjK1, sizeof(NLTRJKEY) * size );
		*(pcandidate->m_pTrjK1) = key;
		return pcandidate->m_pTrjK1;
	}
	// else ... ne reste que le cas ou pkey = [m_pTrjK1-1] ... dans ce cas Elle est d�j� bien plac�e dans l'array, il n'y a rien � changer.
	// En effet, en attribuant la valeur pcandidate->S � pkey ( donc � [m_pTrjK1-1] ) l'ordre des clefs et de leur abscisse respective restera valide.
	//
	//																	S actuel		pcandidate->S								
	//																	|						|							
	//																	|<--					|		 -->				
	// 0 ...........................s...................................s.......................x...........s........................ +
	//								|									|									|			
	//						pcandidate->m_pTrjK1-n			pcandidate->m_pTrjK1-1				pcandidate->m_pTrjK1	
	//																	|	
	//																   pkey

	return pkey;
}

// ------------------------------------------------------------------------------------------
// App_TrajectoryKeyTool_Insert
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT App_TrajectoryKeyTool_Insert(const NEVENT *pevent,NEDITOR_SELECTION *pselect, const Nu32 tool_param )
{
	APP_TRJKEY_TOOLDATA					*pdata;
	APP_TRJKEY_TOOLDATA_KEY_CANDIDATE	keycandidate;
	NLPATH_POINT			path_point;
	NLTRJKEY				*ptrjk;

	NVEC3f32				hitpoint;
	NVEC2f32				local_hitpoint;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;




	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
			pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

			// 2/ on recherche le point le plus proche sur le chemin
			NResetToolSelection(pselect);
			pdata->m_PersistentTrackingData.forwardSetup(&pdata->m_pWorkbench->getPath()->m_geometry, APP_USERKEY_DETECTION_DISTANCE);
			if (pdata->m_PersistentTrackingData.trackForward(&path_point, &local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
			{
				if (ISFLAG_ON(pdata->submit(&keycandidate, path_point.s), FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID))
				{
					// keycandidate.m_TrjK1Index est l'index de la key existante dont l'abscisse est situ�e juste apr�s path_point.s
					// Il convient donc de d'ins�rer une nouvelle key, d'abscisse 'path_point.s' , juste avant 'keycandidate.m_TrjK1Index'
					if (pselect->pSelected = pdata->m_pWorkbench->getTrajectory()->insertUserKey(keycandidate.m_s, keycandidate.m_TrjK1Index, pdata->m_keyCreationFlags))
					{
						pselect->SelectParam = NGetArrayIndex(pdata->m_pWorkbench->getTrajectory()->getKeysArray(), (NBYTE*)pselect->pSelected);
						pdata->m_pWorkbench->requestTrajectoryBuild();

						NErrorIf(keycandidate.m_TrjK1Index != pselect->SelectParam, NERROR_INCONSISTENT_VALUES);
						SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_INSERTED_AND_SELECTED, (WPARAM)pdata->m_pWorkbench, (LPARAM)keycandidate.m_TrjK1Index);
						return NEDITOR_RESULT_TOOL_ON;
					}
					#ifdef _DEBUG
					else
						NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
					#endif			
				}
			}
			break;

		case NTOUCH_MOVE:
			pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
			if (pselect->pSelected)
			{
				NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
				NGetEditorWorkPlaneNormalVector(&plane_dir);
				NGetEditorWorkPlanePoint(&plane_point);
				NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
				pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

				pdata->m_PersistentTrackingData.reset();
				if (pdata->m_PersistentTrackingData.trackForward(&path_point, (NVEC2f32*)&local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
				{
					ptrjk = (NLTRJKEY*)pselect->pSelected;
					if (ISFLAG_ON(pdata->submit(&keycandidate, path_point.s, ptrjk), FLAG_APP_TRJKEY_TOOLDATA_SUBMIT_OUT_VALID))
					{
						//'submit' retourne des infos de placement dans 'pcandidate', c'est � dire dans quelle 'key' du tableau il conviendrait de stocker 'path_point.s'
						// pour que l'ordre des keys dans l'array match avec l'ordre de leur abscisse.
						// ... du coup, il est possible qu'il faille modifier l'ordre des keys du tableau pour qu'il 'colle' � l'ordre des abscisses.
						// Afin d'�viter de faire un SORT complet ( genre quicksort ), on utilisera une fonction sort specifique 
						ptrjk = pdata->setSandSort(ptrjk, &keycandidate);
						pdata->m_pWorkbench->requestTrajectoryBuild();
						if (ptrjk != (NLTRJKEY*)pselect->pSelected)
						{
							pselect->pSelected = (void*)ptrjk;
							pselect->SelectParam = NGetArrayIndex(pdata->m_pWorkbench->getTrajectory()->getKeysArray(), (NBYTE*)ptrjk);
							SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_SELECTED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);
						}
						SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_MOVED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);
					}
				}
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		case NTOUCH_END:
			pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
			NResetToolSelection(pselect);
			return NEDITOR_RESULT_TOOL_ON;
			
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT App_TrajectoryKeyTool_Edit(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param)
{
	APP_TRJKEY_TOOLDATA* pdata;
	APP_TRJKEY_TOOLDATA_KEY_CANDIDATE	keycandidate;
	NLPATH_POINT			path_point;
	NLTRJKEY* ptrjk;

	NVEC3f32				hitpoint;
	NVEC2f32				local_hitpoint;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	Nf32					ds;
	Nf32					minds;
	Nu32					i;
	NARRAY* parr;

#ifdef _DEBUG
	Nu32 bdel = 0;
#endif

	switch (pevent->Type)
	{
	case NTOUCH_START:
		pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
		NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
		pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

		// 2/ on recherche le point le plus proche sur le chemin
		NResetToolSelection(pselect);
		pdata->m_PersistentTrackingData.forwardSetup(&pdata->m_pWorkbench->getPath()->m_geometry, APP_USERKEY_DETECTION_DISTANCE);
		if (pdata->m_PersistentTrackingData.trackForward(&path_point, &local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
		{
			// 3/ On recherche la clef la plus proche 
			parr = pdata->m_pWorkbench->getTrajectory()->getKeysArray();
			ptrjk = (NLTRJKEY*)parr->pFirst;
			minds = APP_USERKEY_DETECTION_DISTANCE;
			for (i = 0; i < parr->Size; i++, ptrjk++)
			{
				if (IS_NLTRJKEY_ENABLE(ptrjk->m_flags))
				{
					ds = NABS(ptrjk->m_s - path_point.s);
					if (ds < minds)
					{
						minds = ds;
						pselect->pSelected = (void*)ptrjk;
						pselect->SelectParam = i;
					}
				}
			}
			if (pselect->pSelected)
			{
				SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_SELECTED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);

			}
			return NEDITOR_RESULT_TOOL_ON;
		}
		break;

	case NTOUCH_MOVE:
		pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
		if (pselect->pSelected && IS_NLTRJKEY_UNLOCKED(((NLTRJKEY*)pselect->pSelected)->m_flags))
		{
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
			pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

			pdata->m_PersistentTrackingData.reset();
			if (pdata->m_PersistentTrackingData.trackForward(&path_point, (NVEC2f32*)&local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
			{
				ptrjk = (NLTRJKEY*)pselect->pSelected;
				pdata->submit(&keycandidate, path_point.s, ptrjk);
				ptrjk = pdata->setSandSort(ptrjk, &keycandidate);
				pdata->m_pWorkbench->requestTrajectoryBuild();
				if (ptrjk != (NLTRJKEY*)pselect->pSelected)
				{
					pselect->pSelected   = (void*)ptrjk;
					pselect->SelectParam = NGetArrayIndex(pdata->m_pWorkbench->getTrajectory()->getKeysArray(), (NBYTE*)ptrjk);
					SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_SELECTED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);
				}
				SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_MOVED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);

			}
			return NEDITOR_RESULT_TOOL_ON;
		}
		break;

	case NTOUCH_END:
		pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
		NResetToolSelection(pselect);
		return NEDITOR_RESULT_TOOL_ON;

		break;

	default:
		break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT App_TrajectoryKeyTool_Delete(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param)
{
	APP_TRJKEY_TOOLDATA* pdata;

	NLPATH_POINT			path_point;
	NLTRJKEY* ptrjk;



	NVEC3f32				hitpoint;
	NVEC2f32				local_hitpoint;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	Nf32					ds;
	Nf32					minds;
	Nu32					i;
	NARRAY* parr;

#ifdef _DEBUG
	Nu32 bdel = 0;
#endif

	switch (pevent->Type)
	{
	case NTOUCH_START:
		pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
		NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
		pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

		// 2/ on recherche le point le plus proche sur le chemin
		NResetToolSelection(pselect);
		pdata->m_PersistentTrackingData.forwardSetup(&pdata->m_pWorkbench->getPath()->m_geometry, APP_USERKEY_DETECTION_DISTANCE);
		if (pdata->m_PersistentTrackingData.trackForward(&path_point, &local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
		{
			// 3/ On recherche la clef la plus proche 
			parr = pdata->m_pWorkbench->getTrajectory()->getKeysArray();
			ptrjk = (NLTRJKEY*)parr->pFirst;
			minds = APP_USERKEY_DETECTION_DISTANCE;
			for (i = 0; i < parr->Size; i++, ptrjk++)
			{
				if (IS_NLTRJKEY_ENABLE_UNLOCKED(ptrjk->m_flags))
				{
					ds = NABS(ptrjk->m_s - path_point.s);
					if (ds < minds)
					{
						minds = ds;
						pselect->pSelected = (void*)ptrjk;
						pselect->SelectParam = i;
					}
				}
			}
			if(pselect->pSelected)
				SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_SELECTED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);

			return NEDITOR_RESULT_TOOL_ON;
		}
		break;

	case NTOUCH_MOVE:
		if (pselect->pSelected)
		{
			NResetToolSelection(pselect);
			return NEDITOR_RESULT_TOOL_ON;
		}
		break;

	case NTOUCH_END:
		if (pselect->pSelected)
		{
			pdata = (APP_TRJKEY_TOOLDATA*)tool_param;
			parr = pdata->m_pWorkbench->getTrajectory()->getKeysArray();
			NEraseArrayElementPtr(parr, (NBYTE*)pselect->pSelected, NLclearTrjKeyInArrayCallBack);
			pdata->m_pWorkbench->requestTrajectoryBuild();
			SendMessage(hwnd_Explorer, WM_MESSAGE_KEY_DELETED, (WPARAM)pdata->m_pWorkbench, (LPARAM)pselect->SelectParam);
			NResetToolSelection(pselect);
			return NEDITOR_RESULT_TOOL_ON;
		}
		break;

	default:
		break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

void App_TrajectoryKeyTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{

}


// -----------------------------------------------------------------------
#endif	//_NEDITOR
