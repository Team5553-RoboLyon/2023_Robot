#include "../../../N/NErrorHandling.h"
#include "../../../N/NMath.h"
#include "../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../N/Utilities/Draw/NUT_DrawPencil.h"

#include "NLVstageArrayTools.h"



// j'en suis là
Nu32 NLVstageArrayTools::insertVelocityStage(NARRAY* parray, NLVSTAGE* pnewvs, const Nu32 mode)
{
	NErrorIf(!pnewvs, NERROR_NULL_POINTER);
	NErrorIf(parray->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	if (!parray->Size)
	{
		NArrayPushBack(parray, (NBYTE*)pnewvs);
	}
	else
	{
		NARRAY	tmp_out_array;
		Nu32	i;
		Nu32	_brk = 0;
		NSetupArray(&tmp_out_array,parray->Size + 1, sizeof(NLVSTAGE));

		pnewvs->resetCutters();
		NLVSTAGE* pvstage = (NLVSTAGE*)parray->pFirst;
		for (i = 0; i < parray->Size; i++, pvstage++)
		{
			// La fonction cutOut est configurée pour:
			//			1/ Ne pusher sur "tmp_out_array" que les parties à gauche ou à droite du cookiecutter
			//			2/ Forcer la découpe systematique que pnewvs aie une vitesse plus élevée ou plus basse ...
			if (ISFLAG_ON(pvstage->cutOut(&tmp_out_array, pnewvs, MAKE_CUTOUT_CFG(0, FLAG_CUTOUT_FORCE | FLAG_CUTOUT_LEFT | FLAG_CUTOUT_RIGHT)), FLAG_CUTOUT_RIGHT))
			{
				// Tout ou partie de pvstage a été poussé sur "tmp_out_array" en étant à droite de pnewvs ...
				// tous les pvstage suivants ne pouvant être qu'encore plus à droite, on sort !
				_brk = 1;
				break;
			}
		}

		if (_brk)
		{
			// On insère pnewvs juste avant le dernier vstage inséré ( et qui était donc à droite, soit après, pnewvdesc )
			NInsertArrayElement(&tmp_out_array, tmp_out_array.Size - 1, (NBYTE*)pnewvs);

			// Reste-t'il des NLVSTAGE à insérer ? ( car peut-être que le 'break' (_brk = 1 ) c'est produit sur le dernier Vstage ... 
			if (i < (parray->Size - 1))
			{
				// Cela signifie qu'on est sorti de la boucle "for" via un break sans attendre la condition de sortie 'officielle' càd : (i == parray->Size)
				// le ième vstage de l'array 'parray' a bien été cutOut/inséré mais la boucle a été interrompue juste après.
				// et on termine l'insertion de tous les suivants de manière plus "véloce" ...
				NArrayBufferPushBack(&tmp_out_array, pvstage + 1, parray->Size - 1 - i);
			}
		}
		else
		{
			// On insère pnewvs à la fin ... tout simplement
			// En effet, si _brk == 0 alors cela signifie que tous les Vstages ont été "poussés"  sur 'tmp_out_array' sans qu'aucun ne "dépasse" à droite de pnewvs ...
			NArrayPushBack(&tmp_out_array, (NBYTE*)pnewvs);
		}

		// pour finir on swap les contenus ( pour éviter une copie ;) )
		NSwapArrayContent(parray, &tmp_out_array);
		NClearArray(&tmp_out_array, NULL);
	}

	return parray->Size;
}

Nu32 NLVstageArrayTools::slice(NARRAY* pout, const NARRAY* pin, Nf32 s0, Nf32 s1, const Nbool binsert_start_end)
{
	NErrorIf(pin == pout, NERROR_INCONSISTENT_VALUES);	// pin et pout doivent être 2 arrays differents !
	NErrorIf(!pin, NERROR_NULL_POINTER);
	NErrorIf(!pout, NERROR_NULL_POINTER);
	NErrorIf(pout->Size, NERROR_ARRAY_IS_NOT_EMPTY);
	NErrorIf(pin->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(pout->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(s0 >= s1, NERROR_INCONSISTENT_VALUES);

	if (!pin->Size)
		return 0;

	NLVSTAGE* pv0 = (NLVSTAGE*)pin->pFirst;
	NLVSTAGE* pv1 = pv0 + pin->Size - 1;
	NErrorIf(pv1 != (NLVSTAGE*)NGetLastArrayPtr(pin), NERROR_SYSTEM_CHECK);

	// Si l'intervalle [s0,s1] est entierement avant ou entierement après l'intervalle [pv0->m_s0, pv1->m_s1] ...
	if (s1 <= pv0->m_s0 || (s0 >= pv1->m_s1))
		return 0;

	// L'intervalle [s0,s1] ne saurait être plus 'large' que l'intervalle [pv0->m_s0, pv1->m_s1] ...
	s0 = NMAX(s0, pv0->m_s0);
	s1 = NMIN(s1, pv1->m_s1);

	// A partir d'ici nous sommes surs que ...
	// L'intervalle [s0,s1] est inclus dans l'intervalle [pv0->m_s0, pv1->m_s1]
	// On cherche maintenant à encadrer plus finement [s0,s1]
	while (pv0->m_s1 < s0) { pv0++; };	
	while (pv1->m_s0 > s1) { pv1--; };

	// A partir d'ici on a:
	//
	//	POUR PV0 et S0
	//
	//				cas #0													cas #1														cas #2
	//
	//	m_s0					m_s1			|				m_s0					m_s1			|						m_s0					m_s1
	//	 +----------o------------+				|				 o-----------------------+				|				 o		 +-----------------------+	
	//				|							|				 |										|				 |
	//			   s0							|				s0										|				s0
	//											|														|
	//											|														|
	//	POUR PV1 et S1
	//
	//				cas #0													cas #1														cas #2
	//											|														|
	//	m_s0					m_s1			|				m_s0					m_s1			|						m_s0					m_s1
	//	 +----------o------------+				|				 +-----------------------+				|				 		 +-----------------------+			o
	//				|							|										 |				|															|
	//			   s1							|										s1				|															s1
	//											|														|	

	// Pour Pv0 et Pv1, dans le cas #2 la fonction slice ... slice dans le vide ! ( c'est à dire entre 2 Vstages, soit une portion de clothoide reliant 1 segment et un arc de cercle.)
	// Mais nous sommes egalement surs que pour s0 il y a un pv0-1 et pour s1 il y a un pv1+1
	// On peut donc calculer:
	//									1/ le point d'intersection entre [pv0-1->m_s1,pv0->m_s0] et la verticale passant par s0
	//									2/ le point d'intersection entre [pv1->m_s1,pv1+1->m_s0] et la verticale passant par s1
	// ... et créer un vstage "point" pour chacun d'eux.
	NLVSTAGE	vstage;
	Nu32		first;
	Nu32		size = (pv1 - pv0) + 1;
	Nf32		v0, v1;
	NLVSTAGE* pvout0;
	NLVSTAGE* pvout1;
	// Etape 1/5: Start point
	if (binsert_start_end)
	{
		vstage.force(s0, s0, 0.0f);
		NArrayPushBack(pout, (NBYTE*)&vstage);
	}

	// Etape 2/5: cas#2 pour pv0/s0
	// cas #2 pour pv0,s0?
	if (s0 < pv0->m_s0)
	{
		NErrorIf(pv0 - 1 < (NLVSTAGE*)pin->pFirst, NERROR_SYSTEM_CHECK);
		v0 = pv0->m_v + ((s0 - pv0->m_s0) / ((pv0 - 1)->m_s1 - pv0->m_s0)) * ((pv0 - 1)->m_v - pv0->m_v); // simple thales
		vstage.force(s0, s0, v0);
		NArrayPushBack(pout, (NBYTE*)&vstage);
	}

	// Etape 3/5:
	if (size)
	{
		first = pout->Size;
		NErrorIf((pv0 + size - 1) > ((NLVSTAGE*)pin->pFirst + pin->Size - 1), NERROR_SYSTEM_CHECK);
		NArrayBufferPushBack(pout, pv0, size);
		pvout0 = (NLVSTAGE*)NGetArrayPtr(pout, first);
		//pvout1 = pvout0 + pout->Size - 1;
		pvout1 = pvout0 + size - 1;
		// Dans les cas autres que le cas 2, l'abscisse m_s0 de pv0 devient s0 ! Normal on a s0 appartient à [pv0->m_s0, pv0->m_s1] et on commence par s0 !
		if (!(s0 < pv0->m_s0))
		{
			pvout0->m_s0 = s0;
		}
	}

	// Etape 4/5: cas#2 pour pv1/s1
	// cas #2 pour pv1,s1?
	if (s1 > pv1->m_s1)
	{
		v1 = pv1->m_v + ((s1 - pv1->m_s1) / ((pv1 + 1)->m_s0 - pv1->m_s1)) * ((pv1 + 1)->m_v - pv1->m_v); // simple thales
		vstage.force(s1, s1, v1);
		NArrayPushBack(pout, (NBYTE*)&vstage);
	}
	else
	{
		NErrorIf(!size, NERROR_SYSTEM_CHECK);
		// Dans les cas autres que le cas 2, l'abscisse m_s1 de pv1 devient s1 ! Normal on a s1 appartient à [pv1->m_s0, pv1->m_s1] et on termine par s1 !
		pvout1->m_s1 = s1;
	}

	// Etape 5/5: End point
	if (binsert_start_end)
	{
		vstage.force(s1, s1, 0.0f);
		NArrayPushBack(pout, (NBYTE*)&vstage);
	}

	return 1;
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Combine 2 arrays de NLVSTAGE en 1 seul.
 *			Pour se faire, les vstages de chaque array sont utilisés comme cookiecutters pour découper ceux de l'autre array.
 *			Ainsi les Vstages de pin0 sont découpés par ceux de pin1
 *			Ensuite les Vstages de pin1 sont découpés par les VStages résultant de la découpe précédente.
 *			Pour finir l'ensemble de ces Vstages sont fusionnés en 1 seule Array avec la certitude qu'il n'y a pas 
 *			juxtaposition de 2 NLVStages.
 *
 *	@param	pout est l'array où sera stocké la combinaison des 2 autres.
 *	@param	pin0 est le premier array de NLVSTAGE
 *	@param	pin1 est le second array de NLVSTAGE
 *	@param	in0_cutters_desc décrit comment construire les cutters des NLVSTAGE de pin0 quand ceux ci sont utilisés comme cookiecutters.
 *			Il y a deux valeurs possibles:	\b COMBINE_RESET_CUTTERS	les cutters seront simplement RESET ( et donc verticaux ).
 *											\b COMBINE_CHAINED_CUTTERS	les cutters seront construits par "chainage".  
 *	@param	in1_cutters_desc décrit comment construire les cutters des NLVSTAGE de pin1 quand ceux ci sont utilisés comme cookiecutters.
 *			Il y a deux valeurs possibles:	\b COMBINE_RESET_CUTTERS	les cutters seront simplement RESET ( et donc verticaux ).
 *											\b COMBINE_CHAINED_CUTTERS	les cutters seront construits par "chainage".

 *	@return	le nombre de NLVSTAGE créés et pushés sur pout:
 */
 // ------------------------------------------------------------------------------------------
Nu32 NLVstageArrayTools::combine(NARRAY* pout, const NARRAY* pin0, const NARRAY* pin1, const Nu32 in0_cutters_desc, const Nu32 in1_cutters_desc)
{
	NErrorIf(pin0 == pout, NERROR_INCONSISTENT_VALUES);	// pin0 et pout doivent être 2 arrays differents !
	NErrorIf(pin1 == pout, NERROR_INCONSISTENT_VALUES);	// pin1 et pout doivent être 2 arrays differents !
	NErrorIf(pin0 == pin1, NERROR_INCONSISTENT_VALUES);	// pin0 et pin1 doivent être 2 arrays differents !
	NErrorIf(pin0 && pin0->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(pin1 && pin1->ElementSize != sizeof(NLVSTAGE), NERROR_ARRAY_WRONG_ELEMENT_SIZE);

	NARRAYBOUNDS		bounds;
	bounds.ElementSize = sizeof(NLVSTAGE);
	bounds.Capacity = 0;
	bounds.Size = 0;

#ifndef _DEBUG
	if (NCheckArrayIntegrity(pout, &bounds, NTRUE) < 0)
		return 0;
#endif

#ifdef _DEBUG
	NErrorIf(NCheckArrayIntegrity(pout, &bounds,NTRUE) <  0, NERROR_ARRAY_CORRUPTED);
	NErrorIf(pout->Size, NERROR_ARRAY_IS_NOT_EMPTY);
#endif

	Nu32 initial_pout_size = pout->Size;

//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	|
//	|	C)	FUSION DES VSTAGES ISSUS DU PATH ET DES VSTAGES UTILISATEUR
// 	|
// 	|
	NLVSTAGE* pvs;
	NLVSTAGE* pcookiecutter;
	NLVSTAGE	vs;
	Nu32		i, j;
	if (!pin0->Size)
	{
		if (!pin1->Size)
		{
			return 0; // normalement pout est initialement 'vide' et aucun element n'a été ajouté.
		}
		else
		{
			// en debug nous sommes sûrs que pout->Size = 0 !
			NCopyArray(pout, pin1);
			return pin1->Size;
		}
	}
	else
	{
		if (!pin1->Size)
		{
			// en debug nous sommes sûrs que pout->Size = 0 !
			NCopyArray(pout, pin0);
			return pin0->Size;
		}
		/*
		else ...  ( pin0->Size != 0 && pin1->Size != 0 )
		{
			Le cas où pin0->Size et pin1->Size sont tous les 2 non nulls.
			est simplement traité juste en dessous, en dehors du test.
		}
		*/
	}

	/*
	A FAIRE ABSOLUMENT A L'EXTERIEUR DE CETTE FONCTION !!!!
	pvs = (NLVSTAGE*)plimited_user_vstages_array->pFirst;
	for (i = 0; i < plimited_user_vstages_array->Size; i++, pvs++)
	{
		pvs->computeAdjustedVelocity(pklim->m_v);
	}
	*/

	//	a)	On commence par découper les NLVSTAGE de pin0 avec les NLVSTAGES de pin1 comme cookiecutters.
	//		Pour ne conserver que les portions effectives, c'est à dire les portions "non-masquées" des NLVSTAGE de pin0 par un NLVSTAGE de pin1.
	//		Les portions de NLVSTAGE masquées ( = cookies ) totalement ou en partie, sont retirées.
	NARRAY	trimed_0_array;
	NSetupArray(&trimed_0_array, pin0->Size + 1, sizeof(NLVSTAGE)); // capacité estimée très aproximative ...
	pvs = (NLVSTAGE*)pin0->pFirst;
	for (i = 0; i < pin0->Size; i++, pvs++)
	{
		NArrayPushBack(&trimed_0_array, (NBYTE*)pvs);
		pcookiecutter = (NLVSTAGE*)pin1->pFirst;
		for (j = 0; j < pin1->Size; j++, pcookiecutter++)
		{
			// Preparation des cutters de cookiecutter:
			if (in1_cutters_desc == COMBINE_CHAINED_CUTTERS)
			{
				pcookiecutter->setCutters((j == 0) ? NULL : pcookiecutter - 1, (j == pin1->Size - 1) ? NULL : pcookiecutter + 1);
			}
			else
			{
				NErrorIf(in1_cutters_desc != COMBINE_RESET_CUTTERS, NERROR_SYSTEM_GURU_MEDITATION);
				pcookiecutter->resetCutters();
			}

			if (trimed_0_array.Size)
			{
				NArrayPopBack(&trimed_0_array, (NBYTE*)&vs);

				// La fonction cutOut est configurée pour:
				//			1/ Ne pusher sur "trimed_0_array" que les parties à gauche ou à droite du cookiecutter
				//			2/ pusher le vstage tout entier sur "trimed_user_vs_array" en cas de coupe non initiée et retourner alors "FLAG_CUTOUT_RIGHT"
				if (ISFLAG_OFF(vs.cutOut(&trimed_0_array, pcookiecutter, MAKE_CUTOUT_CFG(FLAG_CUTOUT_RIGHT, FLAG_CUTOUT_LEFT | FLAG_CUTOUT_RIGHT)), FLAG_CUTOUT_RIGHT))
					break;
			}
			else
				break;
		}
	}
	//	b)	Ensuite, on fait l'inverse, c'est à dire qu'on va découper les NLVStages de pin1 pour ne conserver que les portions "non-masquées" par les NLStage de trimed_0_array
	//		( soit ceux de pin0 fraichement découpés )
	NARRAY	trimed_1_array;
	NSetupArray(&trimed_1_array, pin1->Size + 1, sizeof(NLVSTAGE)); // capacité estimée très aproximative ...
	pvs = (NLVSTAGE*)pin1->pFirst;
	for (i = 0; i < pin1->Size; i++, pvs++)
	{
		NArrayPushBack(&trimed_1_array, (NBYTE*)pvs);
		pcookiecutter = (NLVSTAGE*)trimed_0_array.pFirst;
		for (j = 0; j < trimed_0_array.Size; j++, pcookiecutter++)
		{
			// Preparation des cutters de cookiecutter:
			if (in0_cutters_desc == COMBINE_CHAINED_CUTTERS)
			{
				pcookiecutter->setCutters((j == 0) ? NULL : pcookiecutter - 1, (j == trimed_0_array.Size - 1) ? NULL : pcookiecutter + 1);
			}
			else
			{
				NErrorIf(in0_cutters_desc != COMBINE_RESET_CUTTERS, NERROR_SYSTEM_GURU_MEDITATION);
				pcookiecutter->resetCutters();
			}
			if (trimed_1_array.Size)
			{
				NArrayPopBack(&trimed_1_array, (NBYTE*)&vs);
				// La fonction cutOut est configurée pour:
				//			1/ Ne pusher sur "trimed_1_array" que les parties à gauche ou à droite du cookiecutter
				//			2/ pusher le vstage tout entier sur "trimed_user_vs_array" en cas de coupe non initiée et retourner alors "FLAG_CUTOUT_RIGHT"
				if (ISFLAG_OFF(vs.cutOut(&trimed_1_array, pcookiecutter, MAKE_CUTOUT_CFG(FLAG_CUTOUT_RIGHT, FLAG_CUTOUT_LEFT | FLAG_CUTOUT_RIGHT)), FLAG_CUTOUT_RIGHT))
					break;
			}
			else
				break;
		}
	}

	//	c)	On fusionne les 2 arrays de trims en un seul
	//		En prenant soin au passage de rassembler les trims contigus qui auraient la même vitesse, ce qui est tout à fait possible lors de cette fusion
	NLVSTAGE* ptrimed_0 = (NLVSTAGE*)trimed_0_array.pFirst;			// Pointeur VSTAGE "utilisateur" découpé
	NLVSTAGE* ptrimed_0_out = ptrimed_0 + trimed_0_array.Size;		// Valeur "hors tableau" ( = max + 1 )

	NLVSTAGE* ptrimed_1 = (NLVSTAGE*)trimed_1_array.pFirst;			// Pointeur VSTAGE "computé (à partir du path)" découpé
	NLVSTAGE* ptrimed_1_out = ptrimed_1 + trimed_1_array.Size;		// Valeur "hors tableau" ( = max + 1 )
	while (1)
	{
		// Si ptrimed_1 est hors tableau ...
		if (ptrimed_1 == ptrimed_1_out)
		{
			// Si ptrimed_0 est hors tableau ...
			if (ptrimed_0 == ptrimed_0_out)
			{
				// C'est qu'on a terminé !
				break;
			}
			else
			{
				// Il ne reste plus qu'à "empiler" les ptrimed_0 qui restent jusqu'au dernier !
				ptrimed_0->aggregate(pout);
				ptrimed_0++;
				// ---> continue
			}
		}
		// ... Ptrimed_1 est valide ( il n'est pas 'hors tableau' )
		// Si ptrimed_0 est hors tableau
		else if (ptrimed_0 == ptrimed_0_out)
		{
			// Il ne reste plus qu'à "empiler" les ptrimed_1 qui restent jusqu'au dernier !
			ptrimed_1->aggregate(pout);
			ptrimed_1++;
			// ---> continue
		}
		// ptrimed_0 et ptrimed_1 sont valides tous les deux, il faut choisir lequel des deux insére ...
		else
		{
			// Les deux ont exactement la même abscisse départ
			// Cela n'est possible que si au moins un des deux est un point ( m_s0 = m_s1 ).
			// Notes: Si les deux sont des points alors 
			if (ptrimed_1->m_s0 == ptrimed_0->m_s0)
			{
				// ptrimed_0 est un point
				if (ptrimed_0->m_s1 == ptrimed_0->m_s0)
				{
					// ptrimed_1 est un point egalement !
					if (ptrimed_1->m_s1 == ptrimed_1->m_s0)
					{
						// Les deux VStages sont donc des points !
						// Dans ce cas de figure inédit et normalement impossible tant qu'on autorise pas les user vstage POINT.
						// Cependant, au cas ou cela évolue ... on gere le cas de manière simple et efficace:
						// On ne conserve que le plus bas des deux !
						if(ptrimed_0->m_v <ptrimed_1->m_v)
							ptrimed_0->aggregate(pout);
						else
							ptrimed_1->aggregate(pout);
						ptrimed_0++;
						ptrimed_1++;
					}
					else
					{
						// On insère d'abord le point puis l'autre !
						ptrimed_0->aggregate(pout);
						ptrimed_1->aggregate(pout);
						ptrimed_0++;
						ptrimed_1++;
					}
				}
				// ptrimed_0 n'est pas un point
				else
				{
					// Si aucun des deux n'est un point alors il s'agit d'une erreur !
					NErrorIf(ptrimed_1->m_s1 != ptrimed_1->m_s0, NERROR_SYSTEM_GURU_MEDITATION);

					// On considere donc, en mode normal que ptrimed_1 EST obligatoirement 1 point ...
					// on ne le teste pas et on assumme donc que :  ptrimed_1->m_s1 = ptrimed_1->m_s0
					// ptrimed_1 est un point egalement !
					// On insère d'abord le point puis l'autre !
					ptrimed_1->aggregate(pout);
					ptrimed_0->aggregate(pout);
					ptrimed_1++;
					ptrimed_0++;
				}
				// ---> continue
			}
			else if (ptrimed_0->m_s0 < ptrimed_1->m_s0)
			{
				NErrorIf(ptrimed_1->m_s0 < ptrimed_0->m_s1, NERROR_SYSTEM_GURU_MEDITATION);

				// Le Trim 0 est situé AVANT le Trim 1
				ptrimed_0->aggregate(pout);
				ptrimed_0++;
				// ---> continue
			}
			else//( ptrimed_1->m_s0 < ptrimed_0->m_s0)
			{
				// On checke quand meme qu'il n'y a pas de recouvrement:
				//
				//					0->m_s0							0->m_s1								|
				//					+-------------------------------+									|___ RECOUVREMENT !!! = ERREUR !!!
				//		1->m_s0							1->m_s1											|
				//		+-------------------------------+												|

				//											0->m_s0							0->m_s1		|
				//											+-------------------------------+			|___ OK !!
				//		1->m_s0							1->m_s1											|
				//		+-------------------------------+												|
				NErrorIf(ptrimed_0->m_s0 < ptrimed_1->m_s1, NERROR_SYSTEM_GURU_MEDITATION);

				// Le Trim de Vstage arrive AVANT le Trim de UserKey
				ptrimed_1->aggregate(pout);
				ptrimed_1++;
				// ---> continue
			}
		}
	}
	NClearArray(&trimed_0_array, NULL);
	NClearArray(&trimed_1_array, NULL);


//	NErrorIf((NGetLastArrayPtr(pout)) && ((NLVSTAGE*)NGetLastArrayPtr(pout))->m_s1 > m_pPath->m_geometry.m_ds, NERROR_VALUE_OUTOFRANGE); // supérieure à l'abscisse maximale
//	|
//	|
//	+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	return (pout->Size - initial_pout_size);
}
