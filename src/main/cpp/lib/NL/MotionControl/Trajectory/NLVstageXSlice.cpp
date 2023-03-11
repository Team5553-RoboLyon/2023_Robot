#include <new>
#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NLimits.h"

#include "lib/N/NMemory.h"
#include "lib/N/Maths/NIntervalf32.h"
#include "lib/NL/MotionControl/Trajectory/NLKNode.h"
#include "lib/NL/MotionControl/Trajectory/NLVstageArrayTools.h"

#include "lib/NL/MotionControl/Trajectory/NLVstage.h"

#include "lib/NL/MotionControl/Trajectory/NLVstageXSlice.h"

#include "lib/NL/NL2DOrthogonalCoordinateSystem.h"

#ifdef _NEDITOR
// #include "lib/N/NEditor.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"
#include "lib/NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#endif

NLVSTAGEXSLICE::NLVSTAGEXSLICE()
{
	NSetupArray(&m_vStageXArray, CONSTANT_NLVSTAGEXSLICE_INITIAL_VSTAGEX_ARRAYCAPACITY, sizeof(NLVSTAGEX));
	NSetupArray(&m_knodeArray, 0, sizeof(NLKNODE));

	m_RequiredKinArraySize = 0;
	m_pKey0 = NULL;
	m_pKey1 = NULL;
}

NLVSTAGEXSLICE::~NLVSTAGEXSLICE()
{
	NClearArray(&m_vStageXArray, NLClearVStageXInArrayCallBack);
	NClearArray(&m_knodeArray, NLClearKnodeInArrayCallBack);
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Cette fonction construit un NLVSTAGEXSLICE en 3 �tapes:
 *			1/ "Slicing" c'est � dire s�lection d'un intervalle de VStages situ�s entre les deux clefs pkey0 et pkey1.
 *			2/ "KinX" calculs des KinX pour chaque VstageX inclus. C'est � dire calculs des accelerations max d'entr�e et de sortie de Vstage avec V et J associ�s.
 *			3/ "alternance Floor/ceil" des Vstages. Cette alternance est garantie par l'insertion potentielle de VSTAGE de longueur nulle.
 *
 *
 *	@param	pvstage_array est l'array de VStage depuis lequel on extrait la 'slice' de VStageX.
 *	@param	plocal_limits est
 *	@param	pktweak est
 *	@param	pkey0 est
 *	@param	pkey1 est
 *
 *	@return
 */
// ------------------------------------------------------------------------------------------
Nu32 NLVSTAGEXSLICE::prepare(const NARRAY *pvstage_array, const NLKINLIMITS *plocal_limits, const NLKINTWEAK *pktweak, const NLTRJKEY *pkey0, const NLTRJKEY *pkey1)
{
	NErrorIf(pvstage_array->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(pkey0->m_flags), NERROR_INCONSISTENT_FLAGS);
	NErrorIf(!IS_NLTRJKEY_ENABLE_STOP(pkey1->m_flags), NERROR_INCONSISTENT_FLAGS);

	NResizeArray(&m_vStageXArray, 0, NULL, NLClearVStageXInArrayCallBack);

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	PHASE 0)	SETUP BASIC : key0 et key1 et v�rification de l'intervalle couvert ( i-e nbre de Vstages inclus entre key0 et key1, qui peut, potentiellement, �tre null )
	// 	|
	// 	|
	m_pKey0 = (NLTRJKEY *)pkey0;
	m_pKey1 = (NLTRJKEY *)pkey1;

	// Il est possible que la clef de d�but et de fin soit identique
	if (pkey0 == pkey1)
		return 0;
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	PHASE 1)	'SLICING' DE L'ARRAY DE VSTAGES EN ARRAY DE 'VSTAGEX'
	// 	|
	// 	|
	NLVSTAGEX *pvsx;
	NLVSTAGE *pvs;
	Nu32 i;

	NARRAY tmp_array;
	NSetupArray(&tmp_array, pvstage_array->Size + 2, sizeof(NLVSTAGE));
	NLVstageArrayTools::slice(&tmp_array, pvstage_array, pkey0->m_s, pkey1->m_s, NTRUE);

	//	Cr�ation de l'array de VStageX � partir de l'Array 'Slic�'.
	pvs = (NLVSTAGE *)tmp_array.pFirst;
	for (i = 0; i < tmp_array.Size; i++, pvs++)
	{
		// pvsx = (NLVSTAGEX*) new((void*)NArrayAllocBack0(&m_vStageXArray)) NLVSTAGEX; // Placement new
		pvsx = (NLVSTAGEX *)NArrayAllocBack0(&m_vStageXArray);
		pvsx->set(pvs);
	}
	NClearArray(&tmp_array, NULL);
	//	|
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Check interm�diaire
	NErrorIf(NGetFirstArrayPtr(&m_vStageXArray) && ((NLVSTAGEX *)NGetFirstArrayPtr(&m_vStageXArray))->m_s1 < pkey0->m_s, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(NGetLastArrayPtr(&m_vStageXArray) && ((NLVSTAGEX *)NGetLastArrayPtr(&m_vStageXArray))->m_s1 > pkey1->m_s, NERROR_VALUE_OUTOFRANGE);   // sup�rieure � l'abscisse maximale

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	PHASE 2)	FINALISATION DE L'ARRAY DES 'VSTAGEX': calcul des KinX pour chaque vstageX
	// 	|
	//	|
	NErrorIf(!m_vStageXArray.Size, NERROR_SYSTEM_CHECK);
	pvsx = (NLVSTAGEX *)m_vStageXArray.pFirst;
	for (i = 0; i < m_vStageXArray.Size - 1; i++, pvsx++)
	{
		pvsx->setKinXOut(pvsx + 1, pktweak, plocal_limits);
	}

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	PHASE 3)	FINALISATION DE L'ARRAY DES 'VSTAGEX': Alternance Floor , Ceil
	// 	|
	//	|
	//  |
	//	|	On redistribue ceil et floor au sein de l'array et si besoin on ins�re des ceils de longueur nulle,
	//	|	pour conserver l'alternance ... floor ... ceil  ... floor ...
	NLVSTAGEX vsx;
	for (i = 1; i < m_vStageXArray.Size - 1; i++)
	{
		pvsx = (NLVSTAGEX *)NGetArrayPtr(&m_vStageXArray, i);

		if (pvsx->m_kinXIn.m_a < 0.0f)
		{
			if (pvsx->m_kinXOut.m_a < 0.0f)
			{
				// In: -	Out: -		>>>		HYBRID [FLOOR,ceil] !
				//
				// Il est n�c�ssaire de diviser pvsx en 2 : 1 FLOOR et 1 CEIL de longueur nulle.
				// On va donc ins�rer un nouveau VStage dans l'Array. Comme l'insertion se fait Avant l'index de ref. on va avoir, apr�s insertion:
				//
				// pvsx actuel deviendra ceil de longueur nulle
				// vsx ins�r� sera FLOOR
				vsx = *pvsx;
				vsx.m_kinXOut.null(); // ce FLOOR donne sur le CEIL de longueur nulle ayant la m�me vitesse que lui !
				FLAG_OFF(vsx.m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				pvsx->m_s0 = pvsx->m_s1; // pvs = Vstage de longueur nulle � droite de FLOOR (vs)
				pvsx->m_kinXIn.null();	 // ce CEIL de longueur nulle se trouve juste apr�s le FLOOR ayant la m�me vitesse que lui !
				FLAG_ON(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				// insert un nouveau VStage avant le vstage i
				NInsertArrayElement(&m_vStageXArray, i, (NBYTE *)&vsx);
				i++;
			}
			else // (pvsx->m_kinXOut.m_a > 0.0f) OU ... (pvs->m_kinXOut.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
			{
				NErrorIf(pvsx->m_kinXOut.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				// ... Nous sommes s�rs � partir d'ici que 'pvsx->m_kinXOut.m_a > 0.0f'
				// In: -	Out: +		>>>		FLOOR !
				FLAG_OFF(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);
			}
		}
		else // (pvs->m_kinXIn.m_a > 0.0f) OU ... (pvs->m_kinXIn.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
		{
			NErrorIf(pvsx->m_kinXIn.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
			// ... Nous sommes s�rs � partir d'ici que 'pvs->m_kinXIn.m_a > 0.0f'
			if (pvsx->m_kinXOut.m_a < 0.0f)
			{
				// In: +	Out: -		>>>		CEIL !
				FLAG_ON(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);
			}
			else // (pvs->m_kinXOut.m_a > 0.0f) OU ... (pvs->m_kinXOut.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
			{
				NErrorIf(pvsx->m_kinXOut.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				// ... Nous sommes s�rs � partir d'ici que 'pvs->m_kinXOut.m_a > 0.0f'
				// In: +	Out: +		>>>		HYBRID [ceil,FLOOR]!
				//
				// Il est n�c�ssaire de diviser pvs en 2 : 1 CEIL de longueur nulle et 1 FLOOR.
				// On va donc ins�rer un nouveau VStage dan sl'Array. Comme l'insertion se fait Avant l'index de ref. on va avoir, apr�s insertion:
				//
				// pvs actuel deviendra FLOOR
				// vs ins�r� sera ceil de longueur nulle
				vsx = *pvsx;
				vsx.m_kinXOut.null(); // ce CEIL de longueur nulle se trouve juste avant le FLOOR ayant la m�me vitesse que lui !
				vsx.m_s1 = vsx.m_s0;  // vsx = Vstage de longueur nulle � Gauche de FLOOR (pvs)
				FLAG_ON(vsx.m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				pvsx->m_kinXIn.null(); // ce FLOOR se trouve juste apr�s le CEIL ayant la m�me vitesse que lui !
				FLAG_OFF(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				// insert un nouveau VStage avant le vstage i
				NInsertArrayElement(&m_vStageXArray, i, (NBYTE *)&vsx);
				i++;
			}
		}
	}

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	PHASE 3 CHECK )	...
	// 	|
	//	|
	//	| Nous savons que :
	//	|
	//	| a)	Il y a alternance floor,ceil.
	//	| b)	L'array des vstageX commence et termine par un floor.
	//	| c)	L'array des vstageX contient un nombre impair de vstageX. ( Size est impaire ) la r�partition �tant:
	//	|
	//	| 						VstageX floor : (size + 1) / 2
	//	| 						VstageX ceil :  (size - 1) / 2
	//	|
	//	|  Nous allons donc checker toutes ces assertions !

#ifdef _DEBUG
	// v�rification de a) en sachant qu'on commence par un floor ...
	Nu32 dbg_type = 0; // 0 floor, 1 ceil
	NLVSTAGEX *dbg_pvx = (NLVSTAGEX *)m_vStageXArray.pFirst;
	for (Nu32 dbg_i = 0; dbg_i < m_vStageXArray.Size; dbg_i++, dbg_pvx++, dbg_type = 1 - dbg_type)
	{
		NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL) && dbg_type == 0, NERROR_SYSTEM_GURU_MEDITATION);
		NErrorIf(ISFLAG_OFF(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL) && dbg_type == 1, NERROR_SYSTEM_GURU_MEDITATION);
	}

	// v�rification de b)
	dbg_pvx = (NLVSTAGEX *)m_vStageXArray.pFirst;
	NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL), NERROR_SYSTEM_GURU_MEDITATION);
	dbg_pvx += (m_vStageXArray.Size - 1);
	NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL), NERROR_SYSTEM_GURU_MEDITATION);

	// v�rification de c)
	NErrorIf(!NISODD(m_vStageXArray.Size), NERROR_SYSTEM_GURU_MEDITATION);
	Nu32 dbg_ceil_count = 0;
	Nu32 dbg_floor_count = 0;
	dbg_pvx = (NLVSTAGEX *)m_vStageXArray.pFirst;
	for (Nu32 dbg_i = 0; dbg_i < m_vStageXArray.Size; dbg_i++, dbg_pvx++)
	{
		if (ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
			dbg_ceil_count++;
		else
			dbg_floor_count++;
	}
	NErrorIf(dbg_ceil_count != (m_vStageXArray.Size - 1) / 2, NERROR_SYSTEM_GURU_MEDITATION);
	NErrorIf(dbg_floor_count != (m_vStageXArray.Size + 1) / 2, NERROR_SYSTEM_GURU_MEDITATION);
#endif
	//	|
	//	|
	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	return 0;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief
 *
 *	@param	pkin_out est l'array de sortie o� seront push�s la s�quence de kins correpondant � la solution
 *			issue de l'analyse de l'arbre associ� ( et pr�alablement construit avec NLVSTAGEXSLICE::buildSolutionTree() )
 *
 *	@return le nombre de Kins effectivement 'push�s' sur l'array de sortie
 */
// ------------------------------------------------------------------------------------------
void NLVSTAGEXSLICE::BuildSolutionTree()
{
	NErrorIf(!m_vStageXArray.Size, NERROR_NULL_SIZE);

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	1/ CONSTRUCTION DE L'ARBRE DES SOLUTIONS
	// 	|
	// 	|
	//				 Cr�ation de la file de travail � la taille maximum possible pour une utilisation optimis�e:
	//				 Allouer suffisement de m�moire pour �tre en mesure de stocker l'ensemble des pointeurs sur noeuds dans la file sans reallocation va permettre
	//				 une gestion plus efficace du FIFO sur la file.
	//				 En effet, nous allons allouer la "taille" de la file en 1 seule fois et ne jamais d�sallou� les �l�ments sortis de la file !
	//
	//				 Calcul du nombre de KNODE:
	//						soit F,		le nombre de VstageX de type floor total
	//							 K,		le nombre de Knode total
	//							 size,	le nombre de VStageX
	//
	//						On a, F = (size + 1) / 2
	//
	//						Chaque KNODE se construit entre 1 floor de depart et 1 floor d'arriv�e.
	//						La premiere paire de floor ( premier et dernier vstageX) g�n�re 1 Knode.
	//						Chaque floor suppl�mentaire g�n�re 2 Knodes suppl�mentaire ( car chaque floor supl. d�coupe l'intervalle en 2 )
	//						Donc,
	//						K = 1 + (F-1)*2
	//						K = 1 + 2*F -2
	//						K = 2*F -1
	//						K = 2*[(size+1)/2] - 1
	//						K = (size + 1 ) - 1
	//						K = size
	Ns32 diff = m_vStageXArray.Size - m_knodeArray.Capacity;
	if (diff > 0)
		NIncreaseArrayCapacity(&m_knodeArray, diff);
	NResizeArray(&m_knodeArray, 0, NULL, NULL);

	// Cr�ation d'un premier noeud qu'on ajoute simplement en fin de file
	// On g�re manuellement le nombre de noeuds de la file restant � traiter.( filesize )
	NLKNODE *pkn = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
	pkn->setup((NLVSTAGEX *)NGetFirstArrayPtr(&m_vStageXArray), (NLVSTAGEX *)NGetLastArrayPtr(&m_vStageXArray), NULL, NULL);

	NLKNODE *phead = pkn;
	Nu32 filesize = 1;

	NLVSTAGEX *pvsplit;
	NLKNODE *pknchild_left, *pknchild_right;
	NLSMOTIONSHAPE sleft, sright;
	NLSCUTOUTOFFSETS cutout;
	NLSTARTSTITCH immediate_start;
	NLSCUTINOFFSETS cutin;
	NLENDSTITCH immediate_end;

	Nf32 s_left_in, s_right_out;
	Nf32 s_left_out, s_right_in;
	Nf32 v_left_in, v_right_out;
	Nu32 mps_count = 0;	  // nombre de MotionProfile feuilles ( car d�clar� optimum et non "splitable") d�clar� SUP ou SDOWN ( donc plus simples et l�gers que le SUP_FLAT_SDOWN )
	Nu32 split_count = 0; // nombre de split effectif. ( permet de connaitre le nombre de NLKNODE feuilles soit split_count + 1 )

	// ... Et c'est parti !
	while (filesize)
	{
		// on r�cup�re le noeud le plus ancien ( celui en t�te de File )
		pkn = phead;
		filesize--;
		phead++;

		if (pvsplit = pkn->deepSolveAndGetSplitVStage())
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
			s_left_in = sleft.m_s0;
			v_left_in = sleft.m_v0;
			s_right_out = sright.m_s0 + sright.m_ds;
			v_right_out = sright.m_v1;

			// +------------------------------------------------+
			// | A) Croisement(s) ? (Motion Profile Crossing)	|
			// +------------------------------------------------+
			// ... la distance couverte par les 2 motion S shapes ( sleft et sright ) est plus grande que la distance disponible entre les 2 vstage
			if ((sleft.m_ds + sright.m_ds) > (s_right_out - s_left_in))
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
			else if (sleft.forwardOvershoot(&cutout, &immediate_start, pkn->m_pVfrom, pvsplit))
			{
				// D�passement � droite de split, ... donc calcul des contraintes associ�es
				// Cela signifie que la "distance maximum" s�parant From de Split n'est pas suffisante pour atteindre Split.V depuis From.V...
				// Du coup, Split.V est atteinte plus tard/plus loin dans l'intervalle voisin, c'est � dire dans [Split,To]...
				// En cons�quence, on consid�re que [From,Split] ne sera plus subdivisable.
				// A priori il ne devrait pas y avoir d'autres Vstages dans l'intervalle [From,Split] mais cela n'est pas non plus impossible,
				// On veillera donc � bloquer toute nouvelle subdivision, en flaggant le knode "FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION"
				pvsplit->nullPrimeFlatShape();

				pknchild_left = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
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
				sleft.m_s0 = pvsplit->m_s1 + immediate_start.m_s1Offsets.m_ds;
				sleft.m_v0 = pvsplit->m_v + immediate_start.m_s1Offsets.m_dv; // vitesse initiale
				sleft.m_v1 = pvsplit->m_v;									  // vitesse finale
				// sleft.m_jx = ...;		identique � celui d�j� pr�sent dans sleft	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sleft.m_tx = -cutout.m_dt; // Dur�e n�c�ssaire pour que l'accel. (initialement nulle) atteigne sa valeur max. ( soit m_ax ).
				sleft.m_ax = cutout.m_da;  // sleft.m_jx*sleft.m_tx;									// Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				// tmp_dbg = sleft.m_jx*sleft.m_tx;
				// NErrorIf(cutout.m_da != tmp_dbg, NERROR_SYSTEM_CHECK);
				sleft.m_vx = -cutout.m_dv;							 // Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sleft.m_tb = 0.0f;									 // Dur�e de maintient de l'acceleration maximum.
				sleft.m_sx = sleft.m_jx * NPOW3(sleft.m_tx) / 6.0f;	 // Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
																	 // ... (initialement nulle) atteind sa valeur maximum ( soit m_ax )
																	 // ! 'm_sx' est une 'portion de distance' et ne repr�sente qu'une partie de la distance
																	 // ( c'est pourquoi on parle de portion ) parcourue pendant la dur�e 'm_tx'.
																	 // ! La distance totale parcourue pendnat la dur�e 'm_tx' (i.e durant la phase montante)
																	 // �tant: s = m_sx + v0*m_tx
				sleft.m_ds = (sleft.m_v0 + sleft.m_v1) * sleft.m_tx; // Distance totale couverte pas la S Motion Shape
				pknchild_right->m_motionProfile.sRamped(&sleft, &sright, &immediate_start.m_cutIn, &pkn->m_pVto->m_stitch.m_end.m_cutOut);
			}
			else if (sright.backwardOvershoot(&cutin, &immediate_end, pvsplit, pkn->m_pVto))
			{
				// D�passement � gauche de split, ... donc calcul des contraintes associ�es
				// Cela signifie que la "distance maximum" s�parant Split de To n'est pas suffisante pour atteindre To.V depuis Split.V...
				// Du coup, cela obligerait � partir "avant split" avec une vitesse de Split.V et donc "d'envahir" l'intervalle voisin, c'est � dire [From,Split]...
				// En cons�quence, on consid�re que [Split,To] ne sera plus subdivisable.
				// A priori il ne devrait pas y avoir d'autres Vstages dans l'intervalle [Split,To] mais cela n'est pas non plus impossible, donc on veille � bloquer toute nouvelle subdivision.
				// pvsplit->m_overshootOffsets = ovs;
				pvsplit->nullPrimeFlatShape();

				pknchild_left = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
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
				sright.m_tx = -immediate_end.m_cutOut.m_dt;					 // Dur�e n�c�ssaire pour que l'accel. (initialement nulle) atteigne sa valeur max. ( soit m_ax ).
				sright.m_v0 = pvsplit->m_v;									 // vitesse initiale
				sright.m_v1 = pvsplit->m_v + immediate_end.m_s0Offsets.m_dv; // vitesse finale
				sright.m_ds = (sright.m_v0 + sright.m_v1) * sright.m_tx;	 // Distance totale couverte pas la S Motion Shape

				sright.m_s0 = pvsplit->m_s0 + immediate_end.m_s0Offsets.m_ds - sright.m_ds;
				// sright.m_jx = ...;		identique � celui d�j� pr�sent dans sright	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sright.m_ax = immediate_end.m_cutOut.m_da; // Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				sright.m_vx = -immediate_end.m_cutOut.m_dv;			   // Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sright.m_tb = 0.0f;									   // Dur�e de maintient de l'acceleration maximum.
				sright.m_sx = sright.m_jx * NPOW3(sright.m_tx) / 6.0f; // Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
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
				pknchild_left = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE *)NArrayAllocBack(&m_knodeArray);
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
				if (s_left_out >= pvsplit->m_s0)
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
				else if (sleft.m_ds) //  (s_left_out < pvsplit->m_s0)
				{
					pknchild_left->m_motionProfile.sUpFlat(&sleft, &pkn->m_pVfrom->m_stitch.m_start.m_cutIn, pvsplit->m_s0 - s_left_out);
				}
				else // .... left est FLAT car sleft.m_ds est null !!! ( rappel sleft a �t� construite  par 'sleft.forwardBuildUp' )
				{
					pknchild_left->m_motionProfile.flat(s_left_in, pvsplit->m_s0, sleft.m_v0);
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
				else if (sright.m_ds) // (s_right_in > pvsplit->m_s1)
				{
					pknchild_right->m_motionProfile.flatSdown(&sright, &pkn->m_pVto->m_stitch.m_end.m_cutOut, s_right_in - pvsplit->m_s1);
				}
				else
				{
					pknchild_right->m_motionProfile.flat(pvsplit->m_s1, s_right_out, sright.m_v0);
				}
			}
		} // ... if ( pvsplit = pkn->deepSolveAndGetSplitVStage(&local_limits) )
	}	  // ... while(flisesize)

	//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	|
	//	|	2/ RECHERCHE ET PROMOTION DE LA SOLUTION OPTIMUM DANS L'ARBRE
	// 	|
	// 	|
	//		Toutes les options �tant calcul�es et hi�rarchis�es dans l'arbre, reste maintenant � ne retenir que les meilleures solutions et � calculer/ stocker les kins associ�s.
	//		Pour se faire nous allons avoir besoin d'un buffer de travail, et pour des questions de rapidit� d'�x�cution nous allons faire en sorte d'allouer ce buffer en une seule fois ( pas de r�allocations en cours de route ) !
	//		Nous devons donc estimer la taille maximum que pourrait atteindre ce buffer pour NE JAMAIS �crire/lire � l'ext�rieur.
	//									...
	//		Estimation de la taille max. du buffer n�c�ssaire � la promotion des motion profiles en Kin
	//
	//		Nombre de Kins:	Pour rappel, la promotion d'un Motion profile en Kin ne cr��e pas le kin0 du motion profile... celui - ci �tant consid�r� comme pr�-existant !
	//						Ainsi un motion profile de type SUP se d�crit avec 4 kins :
	//						( mais seul 3 seront r�ellement cr��s )
	//																			kin0 (			t0 ) ... La promotion ne cr�e pas ce premier Kin mais assume qu'il existe d�j�.
	//																			kin1 (kin0	+	tx )
	//																			kin2 (kin1	+	tb ) ... n'existe que si tb > 0
	//																			kin3 (kin2	+	tx )
	//
	//											|		Nombre MINimum de Kin cr��s � la promotion		|		Nombre MAXimum de kin cr��s � la promotion		|
	//											+	   ( ... dans le cas ou Tb(s) est/sont Null(s) )	+														+
	//		Motion Profile	EMPTY				|							0							|							0							|
	//		Motion Profile	SDOWN				|							2	...si Tb null			|							3							|
	//		Motion Profile	FLAT				|							1							|							1							|
	//		Motion Profile	FLAT_SDOWN			|							3	...si Tb null			|							4							|
	//		Motion Profile	SUP					|							2	...si Tb null			|							3							|
	//		Motion Profile	SUP_SDOWN			|							4	...si Tb(s) null(s)		|							6							|
	//		Motion Profile	SUP_FLAT			|							3	...si Tb null			|							4							|
	//		Motion Profile	SUP_FLAT_SDOWN		|							5	...si Tb(s) null(s)		|							7							|
	//
	//		La boucle de cr�ation de motion profile pr�c�dente nous renvoie 2 compteurs:
	//						'split_count'	Le nombre total de divisions d'intervalle en deux sous-intervalles.
	//										On a donc "split_count + 1" intervalles 'feuilles' dans notre arbre.
	//						'mps_count'		Le nombre de motion profile feuille de type SUP ou SDOWN.
	//										( Ces motion profile �tant consid�r�s comme optimum solution n'ont pas �t� re-subdivis�s en 2. Ils sont feuilles � coup s�r !)
	//		On en conclut que, au maximum, et dans le cas ou la solution optimum globale descendrait jusqu'� toutes les feuilles de l'arbre, alors ces motions profile feuilles
	//		composant la solution seraient r�partis comme ceci:
	//
	//										'spli_count + 1' motion profile dont 'mps_count' motion profile de type SUP ou SDOWN
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
	m_RequiredKinArraySize = 8 * (split_count + 1) - 4 * mps_count + 1;

	/*
		NSetArrayCapacity(&m_kinsArray, 1 + 8 * (split_count + 1) - 4 * mps_count, NULL);
		// Insertion du premier Kin dans le buffer
		NLKIN* pkin = (NLKIN*)NArrayAllocBack(&m_kinsArray);
		pkin->null();
		pkn = (NLKNODE*)pknodearray->pFirst;
		Nu32 size = pkn->promote(pkin);
		NErrorIf(m_kinsArray.Size + size > m_kinsArray.Capacity, NERROR_SYSTEM_GURU_MEDITATION);
		NUpSizeArray(&m_kinsArray, size, NULL);

	#ifndef _NEDITOR
		NClearArray(&knodearray, NULL);
	#endif

		// Pour finir: longueur et dur�e de la trajectoire
		pkin = (NLKIN*)NGetLastArrayPtr(&m_kinsArray);
		m_dt = pkin->m_t;
		NErrorIf(size&& NLKIN_S_DISTINCT(m_pPath->m_geometry.m_ds, pkin->m_s), NERROR_WRONG_VALUE);	// on accepte que size == 0, c'est � dire que le promote n'ai rien fait !
																										// ce qui arrive si la vitesse max est �gale/inf�rieure au threshold utilis� dans
																										// la construction des motions profile ... [ cf. NLSMOTIONPROFILE::sRamped(...) ]

	*/

	/*
	 *
	 * LEGACY
	 *
		Nu32 size = pkin_out->Size + 1 + 8 * (split_count + 1) - 4 * mps_count;
		if (pkin_out->Capacity < size)
			NIncreaseArrayCapacity(pkin_out, size - pkin_out->Capacity);

		m_firstKinId = pkin_out->Size;
		// Insertion du premier Kin dans le buffer
		NLKIN* pkin = (NLKIN*)NArrayAllocBack(pkin_out);
		pkin->null();
		pkn = (NLKNODE*)pkin_out->pFirst;
		size = pkn->promote(pkin);
		NErrorIf(pkin_out->Size + size > pkin_out->Capacity, NERROR_SYSTEM_GURU_MEDITATION);
		NUpSizeArray(pkin_out, size, NULL);
		m_lastKinId = pkin_out->Size-1;
	*/

	/*
	#ifndef _NEDITOR
		NClearArray(&knodearray, NULL);
	#endif

		// Pour finir: longueur et dur�e de la trajectoire
		pkin = (NLKIN*)NGetLastArrayPtr(pkin_out);
		m_dt = pkin->m_t;
		NErrorIf(size&& NLKIN_S_DISTINCT(m_pPath->m_geometry.m_ds, pkin->m_s), NERROR_WRONG_VALUE);	// on accepte que size == 0, c'est � dire que le promote n'ai rien fait !
																										// ce qui arrive si la vitesse max est �gale/inf�rieure au threshold utilis� dans
																										// la construction des motions profile ... [ cf. NLSMOTIONPROFILE::sRamped(...) ]
	*/
}

NLTRJPOINT_DESC_CFG NLVSTAGEXSLICE::Promote(const NLTRJPOINT_DESC_CFG cfg, const Nf32 t, NARRAY *pkin_out)
{
	NErrorIf(m_RequiredKinArraySize == 0, NERROR_SYSTEM_GURU_MEDITATION); // < 4 ? ne serait-il pas plus avis� ? car il faut au moins 4 Kins ...
	NErrorIf(!pkin_out, NERROR_NULL_POINTER);

	if (pkin_out->Capacity < pkin_out->Size + m_RequiredKinArraySize)
		NIncreaseArrayCapacity(pkin_out, m_RequiredKinArraySize - (pkin_out->Capacity - pkin_out->Size));

	Nu32 kidx = pkin_out->Size;

	NLKIN kin;
	NLKIN *pkin = NULL;
	NLKIN *pprevkin = (NLKIN *)NGetLastArrayPtr(pkin_out);

	if (pprevkin != NULL)
	{
		if (pprevkin->m_t == t && pprevkin->m_a == 0.0f && pprevkin->m_v == 0.0f)
		{
			pkin = pprevkin;
		}
		else
		{
			kin.null();
			kin.m_t = t;
			kin.m_s = pprevkin->m_s;
			pkin = (NLKIN *)NArrayPushBack(pkin_out, (NBYTE *)&kin);
		}
	}
	else
	{
		kin.null();
		kin.m_t = t;
		pkin = (NLKIN *)NArrayPushBack(pkin_out, (NBYTE *)&kin);
	}

	Nu32 size = ((NLKNODE *)m_knodeArray.pFirst)->promote(pkin);
	NErrorIf(pkin_out->Size + size > pkin_out->Capacity, NERROR_SYSTEM_GURU_MEDITATION);
	NUpSizeArray(pkin_out, size, NULL);

	return size ? MAKE_NLTRJPOINT_DESC_CFG(kidx, TRUE, (cfg & MASK_NLTRJPOINT_DESC_DTMODE) | FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING) : MAKE_NLTRJPOINT_DESC_CFG(0, FALSE, (cfg & MASK_NLTRJPOINT_DESC_DTMODE) | FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING);
}

#ifdef _NEDITOR
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Dessine un VStagex Slice.
 *
 *	@param	p2docs
 *	@param	pickack	couleur id0	: index de la couleur d'un VStage Ceil
 *					couleur id1	: index de la couleur d'un VStage Floor
 *					couleur id5 : index de la couleur du texte d'infos
 *	@return
 */
// ------------------------------------------------------------------------------------------
void NLVSTAGEXSLICE::drawVStages(NLTRAJECTORY_PACK *ppack, NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	Nchar txt[64];
	NLVSTAGEX *pvs, *pvs0;

	NUTDRAWVERTEX va;
	va.Position_3f.z = 0.0f;

	NVEC3 v3;
	v3.z = 0.0f;

	NVEC2 xtd = {0.004f, 0.004f};

	// Preparation: texte
	NUT_DRAWTEXT drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.01f;
	drawtxt.Color = p2docs->m_color[NSTCPLT_GET_COLID_MSK(5, pickpack, NL2DOCS_COLOR_TABLE_ID_MSK)]; // meme chose que: p2docs->getColor(5,pickpack)

	// Preparation: Couleur Vstage
	NCOLOR ceilcolor, floorcolor;
	ceilcolor = p2docs->m_color[NSTCPLT_GET_COLID_MSK(0, pickpack, NL2DOCS_COLOR_TABLE_ID_MSK)];  // meme chose que: p2docs->getColor(0,pickpack)
	floorcolor = p2docs->m_color[NSTCPLT_GET_COLID_MSK(1, pickpack, NL2DOCS_COLOR_TABLE_ID_MSK)]; // meme chose que: p2docs->getColor(1,pickpack)

	// En fonction de t
	if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
	{
		//--------------------------------------------------------------------------------------
		// Repr�senter les velocity stages en fonction du temps revient � repr�senter les limitations de vitesses impos�es (par les velocity Stage) en fonction de l'�volution des positions
		// au cours du temps et donc au long du motion profile courant.
		// Dit autrement, en appliquant le motion profile, on change de position au cours du temps et pour chacune de ces positions on regarde la limitation de vitesse attenante ...
		Nf32 s, v;
		Nu32 iter = 100;
		Nu32 r;
		NINTERVALf32 interval;

		pvs0 = (NLVSTAGEX *)m_vStageXArray.pFirst;
		pvs = pvs0 + 1;
		// Traitement du premier Vstage
		if (ISFLAG_ON(pvs0->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
			va.Color0_4f = ceilcolor;
		else
			va.Color0_4f = floorcolor;

		r = ppack->getTime(&interval, pvs0->m_s0);
		va.Position_3f.x = p2docs->transformX(interval.start);
		va.Position_3f.y = p2docs->transformY(pvs0->m_v);
		NUT_DrawPencil_From(&va);
		if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
		{
			va.Position_3f.x = p2docs->transformX(interval.end);
			va.Position_3f.y = p2docs->transformY(pvs0->m_v);
			NUT_DrawPencil_LineTo(&va);
		}

		for (Nu32 j = 0; j < iter; j++)
		{
			s = pvs0->m_s0 + (pvs0->m_s1 - pvs0->m_s0) * (Nf32)j / (Nf32)iter;
			r = ppack->getTime(&interval, pvs0->m_s0);
			va.Position_3f.x = p2docs->transformX(interval.start);
			va.Position_3f.y = p2docs->transformY(pvs0->m_v);
			NUT_DrawPencil_LineTo(&va);
			if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
			{
				va.Position_3f.x = p2docs->transformX(interval.end);
				va.Position_3f.y = p2docs->transformY(pvs0->m_v);
				NUT_DrawPencil_LineTo(&va);
			}
		}

		for (Nu32 i = 1; i < m_vStageXArray.Size; i++, pvs++, pvs0 = pvs - 1)
		{
			// pvs0 to pvs
			va.Color0_4f = ceilcolor;
			r = ppack->getTime(&interval, pvs0->m_s1);
			va.Position_3f.x = p2docs->transformX(interval.start);
			va.Position_3f.y = p2docs->transformY(pvs0->m_v);
			NUT_DrawPencil_LineTo(&va);
			if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
			{
				va.Position_3f.x = p2docs->transformX(interval.end);
				va.Position_3f.y = p2docs->transformY(pvs0->m_v);
				NUT_DrawPencil_LineTo(&va);
			}

			for (Nu32 j = 0; j < iter; j++)
			{
				s = pvs0->m_s1 + (pvs->m_s0 - pvs0->m_s1) * (Nf32)j / (Nf32)iter;
				v = pvs0->m_v + (pvs->m_v - pvs0->m_v) * (Nf32)j / (Nf32)iter;

				r = ppack->getTime(&interval, s);
				va.Position_3f.x = p2docs->transformX(interval.start);
				va.Position_3f.y = p2docs->transformY(v);
				NUT_DrawPencil_LineTo(&va);
				if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
				{
					va.Position_3f.x = p2docs->transformX(interval.end);
					va.Position_3f.y = p2docs->transformY(v);
					NUT_DrawPencil_LineTo(&va);
				}
			}

			// pvs
			if (ISFLAG_ON(pvs->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
				va.Color0_4f = ceilcolor;
			else
				va.Color0_4f = floorcolor;

			r = ppack->getTime(&interval, pvs->m_s0);
			va.Position_3f.x = p2docs->transformX(interval.start);
			va.Position_3f.y = p2docs->transformY(pvs->m_v);
			NUT_DrawPencil_LineTo(&va);
			if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
			{
				va.Position_3f.x = p2docs->transformX(interval.end);
				va.Position_3f.y = p2docs->transformY(pvs->m_v);
				NUT_DrawPencil_LineTo(&va);
			}

			for (Nu32 j = 0; j < iter; j++)
			{
				s = pvs->m_s0 + (pvs->m_s1 - pvs->m_s0) * (Nf32)j / (Nf32)iter;

				r = ppack->getTime(&interval, s);
				va.Position_3f.x = p2docs->transformX(interval.start);
				va.Position_3f.y = p2docs->transformY(pvs->m_v);
				NUT_DrawPencil_LineTo(&va);
				if (r == NLTRAJECTORYPACK_GETTIME_INTERVAL)
				{
					va.Position_3f.x = p2docs->transformX(interval.end);
					va.Position_3f.y = p2docs->transformY(pvs->m_v);
					NUT_DrawPencil_LineTo(&va);
				}
			}
		}
	}
	// En fonction de S
	else
	{
		// VELOCITY STAGES

		// pvs = (NLVSTAGEX*)velocityStages_Array.pFirst;
		// for (Nu32 i = 0; i < velocityStages_Array.Size; i++, pvs++)
		pvs = (NLVSTAGEX *)m_vStageXArray.pFirst;
		if (ISFLAG_ON(pvs->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
			va.Color0_4f = ceilcolor;
		else
			va.Color0_4f = floorcolor;
		va.Position_3f.x = p2docs->transformX(pvs->m_s0);
		va.Position_3f.y = p2docs->transformY(pvs->m_v);
		NUT_DrawPencil_From(&va);

		if (pvs->m_s0 != pvs->m_s1)
		{
			va.Position_3f.x = p2docs->transformX(pvs->m_s1);
			NUT_DrawPencil_LineTo(&va);
		}
		pvs++;

		for (Nu32 i = 1; i < m_vStageXArray.Size; i++, pvs++)
		{
			if (ISFLAG_ON(pvs->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
				va.Color0_4f = ceilcolor;
			else
				va.Color0_4f = floorcolor;

			va.Position_3f.x = p2docs->transformX(pvs->m_s0);
			va.Position_3f.y = p2docs->transformY(pvs->m_v);
			NUT_DrawPencil_LineTo(&va);

			if (pvs->m_s0 != pvs->m_s1)
			{
				va.Position_3f.x = p2docs->transformX(pvs->m_s1);
				NUT_DrawPencil_LineTo(&va);
			}
		}

		// ADDITIONNAL INFOS
		/*
		if (ISFLAG_ON(p2docs->m_Flags, FLAG_NL2DOCS_DASHBOARD_MP_INFO_SUB))
		{

			pvs = (NLVSTAGEX*)m_vStageXArray.pFirst;
			for (Nu32 i = 0; i < m_vStageXArray.Size; i++, pvs++)
			{
				// ---- Stitch Offsets
				v3.x = p2docs->transformX(pvs->m_s1 + pvs->m_stitch.m_start.m_s1Offsets.m_ds);
				v3.y = p2docs->transformY(pvs->m_v + pvs->m_stitch.m_start.m_s1Offsets.m_dv);
				NUT_Draw_Cross(&v3, &xtd, &drawtxt.Color);
				sprintf(txt, "S1-Start");
				NUT_Draw_Text(txt, &v3, &drawtxt);

				v3.x = p2docs->transformX(pvs->m_s0 + pvs->m_stitch.m_end.m_s0Offsets.m_ds);
				v3.y = p2docs->transformY(pvs->m_v + pvs->m_stitch.m_end.m_s0Offsets.m_dv);
				NUT_Draw_Cross(&v3, &xtd, &drawtxt.Color);
				sprintf(txt, "S0-End");
				NUT_Draw_Text(txt, &v3, &drawtxt);

				// ---- text
				v3.x = p2docs->transformX((pvs->m_s0 + pvs->m_s1) / 2.0f);
				v3.y = p2docs->transformY(pvs->m_v - 0.05f);
				if (NABS(pvs->m_kinXIn.m_a) < NF32_MAX)
					sprintf(txt, "axIn(%.4f)", pvs->m_kinXIn.m_a);
				else
					sprintf(txt, "axIn(UNDEF)");
				NUT_Draw_Text(txt, &v3, &drawtxt);

				v3.y = p2docs->transformY(pvs->m_v - 0.1f);
				if (NABS(pvs->m_kinXOut.m_a) < NF32_MAX)
					sprintf(txt, "axOut(%.4f)", pvs->m_kinXOut.m_a);
				else
					sprintf(txt, "axOut(UNDEF)");
				NUT_Draw_Text(txt, &v3, &drawtxt);

				// ---- text
			}
		}
		*/
	}
}
void NLVSTAGEXSLICE::drawKnodes(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	NLFMOTIONSHAPE fprime;
	Nf32 ds;

	// On d�marre � 1 car le premier knode n'est pas valable ( g�n�ralement )
	// en effet il represente la premiere tentative sur l'intervalle [debut,fin] avec une vitesse nulle au debut et � la fin donc pas de Motion Profile par defaut !
	NLKNODE *pkn = ((NLKNODE *)m_knodeArray.pFirst) + 1;
	for (Nu32 i = 1; i < m_knodeArray.Size; i++, pkn++)
	{
		// Y a t'il une flat shape d'amorcage ?
		ds = pkn->m_pVfrom->m_s1Prime - pkn->m_pVfrom->m_s0Prime;
		// NErrorIf(ds < 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
		if (ds > 0.0f)
		{
			fprime.forwardVs(pkn->m_pVfrom->m_s0Prime, pkn->m_pVfrom->m_v, ds);
			fprime.drawVs(p2docs, pickpack);
			/*
			NVEC3 v;
			Nchar txt[32];
			NUT_DRAWTEXT	drawtxt;
			Nmem0(&drawtxt, NUT_DRAWTEXT);
			drawtxt.Size = 0.01f;
			NSetColorf(&drawtxt.Color, NCOLOR_PRESET3F_BLUE_OXFORD, 1);

			v.z = 0.0f;
			v.x = p2docs->transformX(fprime.m_s0 + fprime.m_ds*0.5f);
			v.y = p2docs->transformY(pkn->m_pVfrom->m_v + 0.05f);
			sprintf(txt, "XFlat ( %.4f )", fprime.m_dt);
			NUT_Draw_Text(txt, &v, &drawtxt);
			*/
		}
		pkn->m_motionProfile.pdraw(&pkn->m_motionProfile, p2docs, pickpack);
	}
}
#endif

/*
Nu32 NLVSTAGEXSLICE::buildAndSolve(const NARRAY* pvstage_array, const NLKINLIMITS* plocal_limits, const NLKINTWEAK *pktweak, const NARRAY* pkey_array, const Nu16 startkeyid, const Nu16 endkeyid)
{
	NErrorIf(pvstage_array->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(pkey_array->ElementSize != sizeof(NLTRAJECTORYKEY), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	PHASE 1)	'SLICING' DES VSTAGES FUSIONNES EN ARRAY DE 'VSTAGEX'
// 	|
// 	|
	NLTRAJECTORYKEY* pkey0 = (NLTRAJECTORYKEY*)NGetArrayPtr(pkey_array, (Nu32)startkeyid);
	NLTRAJECTORYKEY* pkey1 = (NLTRAJECTORYKEY*)NGetArrayPtr(pkey_array, (Nu32)endkeyid);

	NLVSTAGEX	*pvsx;
	NLVSTAGE	*pvs;
	Nu32 i;

	NARRAY tmp_array;
	NSetupArray(&tmp_array, pvstage_array->Size + 2, sizeof(NLVSTAGE));
	NLVstageArrayTools::slice(&tmp_array, pvstage_array, pkey0->m_s, pkey1->m_s, NTRUE);

	//	Cr�ation l'array de VStages � partir de l'Array final de trims.

	NResizeArray(&m_vStageXArray, 0, NULL, NLclearVSTAGEXSLICECallBack);
	pvs = (NLVSTAGE*)tmp_array.pFirst;
	for (i = 0; i < tmp_array.Size; i++, pvs++)
	{
		pvsx = (NLVSTAGEX*)NArrayAllocBack(&m_vStageXArray);
		pvsx->set(pvs);
	}
//	|
//	|
//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Check interm�diaire
	NErrorIf(NGetFirstArrayPtr(&m_vStageXArray) && ((NLVSTAGEX*)NGetFirstArrayPtr(&m_vStageXArray))->m_s1	< pkey0->m_s, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(NGetLastArrayPtr(&m_vStageXArray) && ((NLVSTAGEX*)NGetLastArrayPtr(&m_vStageXArray))->m_s1		> pkey1->m_s, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale

//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	PHASE 2)	FINALISATION DE L'ARRAY DES 'VSTAGEX'
// 	|
// 	|						A) calcul des KinX pour chaque vstageX
// 	|						B) alternance Floor , Ceil
//	|
	// A) calcul des KinX pour chaque vstageX
	pvsx = (NLVSTAGEX*)m_vStageXArray.pFirst;
	for (i = 0; i < m_vStageXArray.Size - 1; i++, pvsx++)
	{
		pvsx->setKinXOut(pvsx + 1, pktweak, plocal_limits);
	}

	// B) Alternance Floor, Ceil
	//		On redistribue ceil et floor au sein de l'array et si besoin on ins�re des ceils de longueur nulle,
	//		pour conserver l'alternance ... floor ... ceil  ... floor ...
	NLVSTAGEX	vsx;
	for (i = 1; i < m_vStageXArray.Size - 1; i++)
	{
		pvsx = (NLVSTAGEX*)NGetArrayPtr(&m_vStageXArray, i);

		if (pvsx->m_kinXIn.m_a < 0.0f)
		{
			if (pvsx->m_kinXOut.m_a < 0.0f)
			{
				// In: -	Out: -		>>>		HYBRID [FLOOR,ceil] !
				//
				// Il est n�c�ssaire de diviser pvsx en 2 : 1 FLOOR et 1 CEIL de longueur nulle.
				// On va donc ins�rer un nouveau VStage dans l'Array. Comme l'insertion se fait Avant l'index de ref. on va avoir, apr�s insertion:
				//
				// pvsx actuel deviendra ceil de longueur nulle
				// vsx ins�r� sera FLOOR
				vsx = *pvsx;
				vsx.m_kinXOut.null();	// ce FLOOR donne sur le CEIL de longueur nulle ayant la m�me vitesse que lui !
				FLAG_OFF(vsx.m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				pvsx->m_s0 = pvsx->m_s1;	// pvs = Vstage de longueur nulle � droite de FLOOR (vs)
				pvsx->m_kinXIn.null();	// ce CEIL de longueur nulle se trouve juste apr�s le FLOOR ayant la m�me vitesse que lui !
				FLAG_ON(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				// insert un nouveau VStage avant le vstage i
				NInsertArrayElement(&m_vStageXArray, i, (NBYTE*)&vsx);
				i++;
			}
			else // (pvsx->m_kinXOut.m_a > 0.0f) OU ... (pvs->m_kinXOut.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
			{
				NErrorIf(pvsx->m_kinXOut.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				// ... Nous sommes s�rs � partir d'ici que 'pvsx->m_kinXOut.m_a > 0.0f'
				// In: -	Out: +		>>>		FLOOR !
				FLAG_OFF(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);
			}
		}
		else // (pvs->m_kinXIn.m_a > 0.0f) OU ... (pvs->m_kinXIn.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
		{
			NErrorIf(pvsx->m_kinXIn.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
			// ... Nous sommes s�rs � partir d'ici que 'pvs->m_kinXIn.m_a > 0.0f'
			if (pvsx->m_kinXOut.m_a < 0.0f)
			{
				// In: +	Out: -		>>>		CEIL !
				FLAG_ON(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);
			}
			else // (pvs->m_kinXOut.m_a > 0.0f) OU ... (pvs->m_kinXOut.m_a == 0.0f) ce qui est normalement impossible � ce stade ( thx to _trim_aggregate )
			{
				NErrorIf(pvsx->m_kinXOut.m_a == 0.0f, NERROR_SYSTEM_GURU_MEDITATION);
				// ... Nous sommes s�rs � partir d'ici que 'pvs->m_kinXOut.m_a > 0.0f'
				// In: +	Out: +		>>>		HYBRID [ceil,FLOOR]!
				//
				// Il est n�c�ssaire de diviser pvs en 2 : 1 CEIL de longueur nulle et 1 FLOOR.
				// On va donc ins�rer un nouveau VStage dan sl'Array. Comme l'insertion se fait Avant l'index de ref. on va avoir, apr�s insertion:
				//
				// pvs actuel deviendra FLOOR
				// vs ins�r� sera ceil de longueur nulle
				vsx = *pvsx;
				vsx.m_kinXOut.null();	// ce CEIL de longueur nulle se trouve juste avant le FLOOR ayant la m�me vitesse que lui !
				vsx.m_s1 = vsx.m_s0;	// vsx = Vstage de longueur nulle � Gauche de FLOOR (pvs)
				FLAG_ON(vsx.m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				pvsx->m_kinXIn.null();	// ce FLOOR se trouve juste apr�s le CEIL ayant la m�me vitesse que lui !
				FLAG_OFF(pvsx->m_flags, FLAG_NLVSTAGEX_IS_CEIL);

				// insert un nouveau VStage avant le vstage i
				NInsertArrayElement(&m_vStageXArray, i, (NBYTE*)&vsx);
				i++;
			}
		}
	}

	// Check repartition ceil/floor
	// Nous savons que :
	//
	//	a)	Il y a alternance floor,ceil.
	//	b)	L'array des vstageX commence et termine par un floor.
	//	c)	L'array des vstageX contient un nombre impair de vstageX. ( Size est impaire ) la r�partition �tant:
	//
	//						VstageX floor : (size + 1) / 2
	//						VstageX ceil :  (size - 1) / 2
	//
	// Nous allons donc checker toutes ces assertions !
	#ifdef _DEBUG
	// v�rification de a) en sachant qu'on commence par un floor ...
	Nu32 dbg_type = 0; // 0 floor, 1 ceil
	NLVSTAGEX* dbg_pvx = (NLVSTAGEX*)m_vStageXArray.pFirst;
	for (Nu32 dbg_i = 0; dbg_i < m_vStageXArray.Size; dbg_i++, dbg_pvx++, dbg_type = 1 - dbg_type)
	{
		NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL)&&dbg_type== 0, NERROR_SYSTEM_GURU_MEDITATION);
		NErrorIf(ISFLAG_OFF(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL) && dbg_type == 1, NERROR_SYSTEM_GURU_MEDITATION);
	}

	// v�rification de b)
	dbg_pvx = (NLVSTAGEX*)m_vStageXArray.pFirst;
	NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL), NERROR_SYSTEM_GURU_MEDITATION);
	dbg_pvx += (m_vStageXArray.Size - 1);
	NErrorIf(ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL), NERROR_SYSTEM_GURU_MEDITATION);

	// v�rification de c)
	NErrorIf( !NISODD(m_vStageXArray.Size), NERROR_SYSTEM_GURU_MEDITATION );
	Nu32 dbg_ceil_count		= 0;
	Nu32 dbg_floor_count	= 0;
	dbg_pvx = (NLVSTAGEX*)m_vStageXArray.pFirst;
	for (Nu32 dbg_i = 0; dbg_i < m_vStageXArray.Size; dbg_i++, dbg_pvx++)
	{
		if (ISFLAG_ON(dbg_pvx->m_flags, FLAG_NLVSTAGEX_IS_CEIL))
			dbg_ceil_count++;
		else
			dbg_floor_count++;
	}
	NErrorIf(dbg_ceil_count  != (m_vStageXArray.Size - 1)/2, NERROR_SYSTEM_GURU_MEDITATION);
	NErrorIf(dbg_floor_count != (m_vStageXArray.Size + 1)/2, NERROR_SYSTEM_GURU_MEDITATION);
	#endif
//	|
//	|
//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	PHASE 3)	CONSTRUCTION DE L'ARBRE DES SOLUTIONS
// 	|
// 	|
//				 Cr�ation de la file de travail � la taille maximum possible pour une utilisation optimis�e:
//				 Allouer suffisement de m�moire pour �tre en mesure de stocker l'ensemble des pointeurs sur noeuds dans la file sans reallocation va permettre
//				 une gestion plus efficace du FIFO sur la file.
//				 En effet, nous allons allouer la "taille" de la file en 1 seule fois et ne jamais d�sallou� les �l�ments sortis de la file !
//
//				 Calcul du nombre de KNODE:
//						soit F,		le nombre de VstageX de type floor total
//							 K,		le nombre de Knode total
//							 size,	le nombre de VStageX
//
//						On a, F = (size + 1) / 2
//
//						Chaque KNODE se construit entre 1 floor de depart et 1 floor d'arriv�e.
//						La premiere paire de floor ( premier et dernier vstageX) g�n�re 1 Knode.
//						Chaque floor supl�mentaire g�n�re 2 Knodes suppl�mentaire ( car chaque floor supl. d�coupe l'intervalle en 2 )
//						Donc,
//						K = 1 + (F-1)*2
//						K = 1 + 2*F -2
//						K = 2*F -1
//						K = 2*[(size+1)/2] - 1
//						K = (size + 1 ) - 1
//						K = size
Ns32 diff = m_vStageXArray.Size - m_knodeArray.Capacity;
	if (diff > 0)
		NIncreaseArrayCapacity(&m_knodeArray, diff);

	// Cr�ation d'un premier noeud qu'on ajoute simplement en fin de file
	// On g�re manuellement le nombre de noeuds de la file restant � traiter.( filesize )
	NLKNODE				*phead;
	Nu32				filesize;

	NLVSTAGEX			*pvsplit;
	NLKNODE				*pknchild_left, *pknchild_right;
	NLSMOTIONSHAPE		sleft, sright;
	NLSCUTOUTOFFSETS	cutout;
	NLSTARTSTITCH		immediate_start;
	NLSCUTINOFFSETS		cutin;
	NLENDSTITCH			immediate_end;

	Nf32				s_left_in, s_right_out;
	Nf32				s_left_out, s_right_in;
	Nf32				v_left_in, v_right_out;
	Nf32				tmp_dbg;

	Nu32				mps_count	= 0;	// nombre de MotionProfile feuilles ( car d�clar� optimum et non "splitable") d�clar� SUP ou SDOWN ( donc plus simples et l�gers que le SUP_FLAT_SDOWN )
	Nu32				split_count = 0;	// nombre de split effectif. ( permet de connaitre le nombre de NLKNODE feuilles soit split_count + 1 )

	NLKNODE* pkn = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
	filesize = 1;
	pkn->setup((NLVSTAGEX*)NGetFirstArrayPtr(&m_vStageXArray), (NLVSTAGEX*)NGetLastArrayPtr(&m_vStageXArray), NULL, NULL);


	// ... Et c'est parti !
	while (filesize)
	{
		// on r�cup�re le noeud le plus ancien ( celui en t�te de File )
		pkn = phead;
		filesize--;
		phead++;

		if (pvsplit = pkn->deepSolveAndGetSplitVStage())
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
			s_left_in = sleft.m_s0;
			v_left_in = sleft.m_v0;
			s_right_out = sright.m_s0 + sright.m_ds;
			v_right_out = sright.m_v1;

			// +------------------------------------------------+
			// | A) Croisement(s) ? (Motion Profile Crossing)	|
			// +------------------------------------------------+
			// ... la distance couverte par les 2 motion S shapes ( sleft et sright ) est plus grande que la distance disponible entre les 2 vstage
			if ((sleft.m_ds + sright.m_ds) > (s_right_out - s_left_in))
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
			else if (sleft.forwardOvershoot(&cutout, &immediate_start, pkn->m_pVfrom, pvsplit))
			{
				// D�passement � droite de split, ... donc calcul des contraintes associ�es
				// Cela signifie que la "distance maximum" s�parant From de Split n'est pas suffisante pour atteindre Split.V depuis From.V...
				// Du coup, Split.V est atteinte plus tard/plus loin dans l'intervalle voisin, c'est � dire dans [Split,To]...
				// En cons�quence, on consid�re que [From,Split] ne sera plus subdivisable.
				// A priori il ne devrait pas y avoir d'autres Vstages dans l'intervalle [From,Split] mais cela n'est pas non plus impossible,
				// On veillera donc � bloquer toute nouvelle subdivision, en flaggant le knode "FLAG_NLSMOTIONPROFILE_OPTIMUM_SOLUTION"
				pvsplit->nullPrimeFlatShape();

				pknchild_left = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
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
				sleft.m_s0 = pvsplit->m_s1 + immediate_start.m_s1Offsets.m_ds;
				sleft.m_v0 = pvsplit->m_v + immediate_start.m_s1Offsets.m_dv;					// vitesse initiale
				sleft.m_v1 = pvsplit->m_v;																// vitesse finale
				// sleft.m_jx = ...;		identique � celui d�j� pr�sent dans sleft	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sleft.m_tx = -cutout.m_dt;																// Dur�e n�c�ssaire pour que l'accel. (initialement nulle) atteigne sa valeur max. ( soit m_ax ).
				sleft.m_ax = cutout.m_da; // sleft.m_jx*sleft.m_tx;									// Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				//tmp_dbg = sleft.m_jx*sleft.m_tx;
				//NErrorIf(cutout.m_da != tmp_dbg, NERROR_SYSTEM_CHECK);
				sleft.m_vx = -cutout.m_dv;																// Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sleft.m_tb = 0.0f;																		// Dur�e de maintient de l'acceleration maximum.
				sleft.m_sx = sleft.m_jx * NPOW3(sleft.m_tx) / 6.0f;											// Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
																										// ... (initialement nulle) atteind sa valeur maximum ( soit m_ax )
																										// ! 'm_sx' est une 'portion de distance' et ne repr�sente qu'une partie de la distance
																										// ( c'est pourquoi on parle de portion ) parcourue pendant la dur�e 'm_tx'.
																										// ! La distance totale parcourue pendnat la dur�e 'm_tx' (i.e durant la phase montante)
																										// �tant: s = m_sx + v0*m_tx
				sleft.m_ds = (sleft.m_v0 + sleft.m_v1) * sleft.m_tx;										// Distance totale couverte pas la S Motion Shape
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

				pknchild_left = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
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
				sright.m_v1 = pvsplit->m_v + immediate_end.m_s0Offsets.m_dv;								// vitesse finale
				sright.m_ds = (sright.m_v0 + sright.m_v1) * sright.m_tx;										// Distance totale couverte pas la S Motion Shape

				sright.m_s0 = pvsplit->m_s0 + immediate_end.m_s0Offsets.m_ds - sright.m_ds;
				// sright.m_jx = ...;		identique � celui d�j� pr�sent dans sright	// Jerk initial (et max).	Est du m�me signe que (v1 - v0) et null si v1 == v0
				sright.m_ax = immediate_end.m_cutOut.m_da;													// Acceleration Max.		Est du m�me signe que (v1 - v0) et nulle si v1 == v0

				sright.m_vx = -immediate_end.m_cutOut.m_dv;													// Vitesse acquise quand l'acceleration (initialement nulle) atteind sa valeur max. ( soit m_ax ).
				sright.m_tb = 0.0f;																			// Dur�e de maintient de l'acceleration maximum.
				sright.m_sx = sright.m_jx * NPOW3(sright.m_tx) / 6.0f;										// Portion de distance couverte pas la SShape sur une dur�e de m_tx pendant que l'acceleration
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
				pknchild_left = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
				pknchild_right = (NLKNODE*)NArrayAllocBack(&m_knodeArray);
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
				if (s_left_out >= pvsplit->m_s0)
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
					pknchild_left->m_motionProfile.flat(s_left_in, pvsplit->m_s0, sleft.m_v0);
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
				else if (sright.m_ds)// (s_right_in > pvsplit->m_s1)
				{
					pknchild_right->m_motionProfile.flatSdown(&sright, &pkn->m_pVto->m_stitch.m_end.m_cutOut, s_right_in - pvsplit->m_s1);
				}
				else
				{
					pknchild_right->m_motionProfile.flat(pvsplit->m_s1, s_right_out, sright.m_v0);
				}
			}
		} // ... if ( pvsplit = pkn->deepSolveAndGetSplitVStage(&local_limits) )
	} // ... while(flisesize)

	return m_knodeArray.Size;
}
*/