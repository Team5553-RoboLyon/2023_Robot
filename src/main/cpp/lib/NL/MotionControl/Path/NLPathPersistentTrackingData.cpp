#include "../../../N/NMemory.h"
#include "../../../N/NMath.h"
#include "../../../N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVecLimits.h"
#include "../../../N/NErrorHandling.h"

#include "NLPathPersistentTrackingData.h"

#ifdef _NEDITOR
#include "../../../N/NEditor.h"
#include "../../../N/Utilities/Draw/NUT_Draw.h"
#include "../../../N/Utilities/Draw/NUT_DrawPencil.h"
#endif

void NLPATH_PERSISTENT_TRACKING_DATA::reset()
{
	// m_flags					= ;
	// m_pPathGeometry			= ;
	// m_pointToPathMaxDist		= ;
	// m_clothoidErrorThreshold	= ;
	if (ISFLAG_ON(m_flags, FLAG_NLPATH_PERSISTENT_TRACKING_DATA_BACKWARD))
	{
		m_trackedPrimitiveIndex = m_pPathGeometry->m_primitivesArray.Size;
		;
		m_pTrackedPrimitive = (NLPATH_PRIMITIVE *)NGetLastArrayPtr(&m_pPathGeometry->m_primitivesArray); // tjrs la derni�re primitive de la liste
		m_pTrackedKeyPoint = (NLPATH_POINT *)NGetLastArrayPtr(&m_pPathGeometry->m_pathPointsArray) - 1;	 // tjrs le premier keypoint de la primitive ( in )
	}
	else
	{
		m_trackedPrimitiveIndex = 0;
		m_pTrackedPrimitive = (NLPATH_PRIMITIVE *)m_pPathGeometry->m_primitivesArray.pFirst; // tjrs la premi�re primitive de la liste
		m_pTrackedKeyPoint = (NLPATH_POINT *)m_pPathGeometry->m_pathPointsArray.pFirst + 1;	 // tjrs le second keypoint de la primitive ( out )
	}
}

void NLPATH_PERSISTENT_TRACKING_DATA::forwardSetup(const NLPATH_GEOMETRY *ppath_geometry, const Nf32 point_to_path_max_dist, const Nf32 cloth_err_threshold)
{
	NErrorIf(!ppath_geometry, NERROR_NULL_POINTER);
	m_flags = 0;
	m_pPathGeometry = (NLPATH_GEOMETRY *)ppath_geometry;
	m_trackedPrimitiveIndex = 0;
	m_pTrackedPrimitive = (NLPATH_PRIMITIVE *)ppath_geometry->m_primitivesArray.pFirst; // tjrs la premi�re primitive de la liste
	m_pTrackedKeyPoint = (NLPATH_POINT *)ppath_geometry->m_pathPointsArray.pFirst + 1;	// tjrs le second keypoint de la primitive ( out )
	m_pointToPathMaxDist = point_to_path_max_dist;
	m_clothoidErrorThreshold = cloth_err_threshold;
}
void NLPATH_PERSISTENT_TRACKING_DATA::backwardSetup(const NLPATH_GEOMETRY *ppath_geometry, const Nf32 point_to_path_max_dist, const Nf32 cloth_err_threshold)
{
	NErrorIf(!ppath_geometry, NERROR_NULL_POINTER);
	FLAG_ON(m_flags, FLAG_NLPATH_PERSISTENT_TRACKING_DATA_BACKWARD);
	m_pPathGeometry = (NLPATH_GEOMETRY *)ppath_geometry;
	m_trackedPrimitiveIndex = ppath_geometry->m_primitivesArray.Size - 1;
	m_pTrackedPrimitive = (NLPATH_PRIMITIVE *)NGetLastArrayPtr(&ppath_geometry->m_primitivesArray); // tjrs la derni�re primitive de la liste
	m_pTrackedKeyPoint = (NLPATH_POINT *)NGetLastArrayPtr(&ppath_geometry->m_pathPointsArray) - 1;	// tjrs le premier keypoint de la primitive ( in )
	m_pointToPathMaxDist = point_to_path_max_dist;
	m_clothoidErrorThreshold = cloth_err_threshold;
}

NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT NLPATH_PERSISTENT_TRACKING_DATA::trackForward(NLPATH_POINT *pout, const NVEC2f32 *ppos)
{
	TRCK_RESULT rtrn = TRCK_RESULT::NOT_DETECTED;	 // valeur de retour de la fonction
	NLPATH_PRIMITIVE *pprim = m_pTrackedPrimitive;	 // primitive en cours de traitement
	NLPATH_POINT *pkp1 = m_pTrackedKeyPoint;		 // point de fin de la primitive en cours de traitement
	NLPATH_POINT *pkp0 = pkp1 - 1;					 // point de d�but de la primitive en cours de traitement
	Nu32 i;											 // index primitive courante
	Nf32 IP_squarelength;							 // distance entre P et son image I ( projection ) sur le chemin
	Nf32 max_IP_length = m_pointToPathMaxDist;		 // MAX distance entre P (ppos) et son image I (projection) sur le chemin
	Nf32 max_IP_squarelength = NPOW2(max_IP_length); // ( MAX distance entre P et son image I ) au carr� !

	Nf32 length;
	Nf32 dot1, dot0;
	Nf32 ang;
	Nf32 f, ds;
	Nf32 cf, sf;
	Nf32 theta;

	NVEC2 A;   // pkp0 dans la base appropri�e
	NVEC2 B;   // pkp1 dans la base appropri�e
	NVEC2 P;   // ppos dans la base appropri�e
	NVEC2 I;   // Image de P ( projection)
	NVEC2 IP;  // I par rapport � P
	NVEC2 tmp; // Vecteur intermediaire de calcul

	NVEC2 bsc; // bissectrice
	Nf32 bsc_threshold;
	NVEC2 ortho_bsc; // perpendiculaire � bsc

	// clothoid
	Nf32 min;	   // abscisse curv. min sur la clothoide |_ d�finissent l'intervalle [min,max] de recherche de l'abscisse curv, de I image de P sur la clothoide.
	Nf32 max;	   // abscisse curv. max sur la clothoide |
	NVEC2 clth_o;  // Origine de construction geometrique de la clothoide ( son point de d�part, l� ou k = 0 )
	NVEC2 clth_of; // Fin de construction geometrique de la clothoide ( son point de d'arriv�e, l� ou k != 0 ) exprim�e dans la base [clth_o,i,j] de la clothoide.
	NVEC2 clth_Iu; // vecteur tangent � la clothoide en I, image de P. Ici I sera la projection de
	Nf32 clth_Is;  // Abscisse curviligne de I

	NLPATH_POINT *ptrk_kp;
	NLPATH_PRIMITIVE *ptrk_prim;
	Nu32 trk_primind;

	m_iteration = 0;

	for (i = m_trackedPrimitiveIndex; i < m_pPathGeometry->m_primitivesArray.Size; i++, pprim++, pkp0 = pkp1, pkp1++)
	{

		// recherche "SUR" la primitive
		switch (pprim->m_core.m_id)
		{
		case NLPATH_PRIMITIVE_ID_SEGMENT:
			P.x = ppos->x - pkp0->p.x;
			P.y = ppos->y - pkp0->p.y;
			dot0 = NVec2DotProduct(&P, &pkp1->u); // !!! pkp1 et pas pkp0 car pour un segment le vecteur directeur est pkp1->u !!!

			if (dot0 <= 0.0f)
			{
				// Le point le plus proche de P est pkp0
				// la distance [ppos,pkp0] vaut norme de P
				IP_squarelength = NVec2SquareLength(&P);
				if (IP_squarelength < max_IP_squarelength)
				{
					trk_primind = i;
					ptrk_prim = pprim;
					ptrk_kp = pkp1;

					pout->s = pkp0->s;
					pout->p = pkp0->p;
					pout->k = 0.0f;
					pout->u = pkp0->u; // pkp0 et pas pkp1 car ppos est consid�r� comme AVANT pkp0 !!! DONC pas vraiment sur le segment

					max_IP_squarelength = IP_squarelength;
					max_IP_length = sqrt(max_IP_squarelength);

					rtrn = rtrn = TRCK_RESULT::ON_PATH_SEGMENT_START;
				}
			}
			else if (dot0 >= pprim->m_core.m_l)
			{
				// Le point le plus proche de A est pkp1
				// [pppos,pkp1] vaut
				P.x = ppos->x - pkp1->p.x;
				P.y = ppos->y - pkp1->p.y;
				IP_squarelength = NVec2SquareLength(&P);
				if (IP_squarelength < max_IP_squarelength)
				{
					trk_primind = i;
					ptrk_prim = pprim;
					ptrk_kp = pkp1;

					pout->s = pkp1->s;
					pout->p = pkp1->p;
					pout->k = 0.0f;
					pout->u = pkp1->u;

					max_IP_squarelength = IP_squarelength;
					max_IP_length = sqrt(max_IP_squarelength);

					rtrn = TRCK_RESULT::ON_PATH_SEGMENT_END;
				}
			}
			else // 0 < dot0 < pprim->m_core.m_l
			{
				I.x = pkp0->p.x + dot0 * pkp1->u.x;
				I.y = pkp0->p.y + dot0 * pkp1->u.y;
				IP.x = I.x - ppos->x;
				IP.y = I.y - ppos->y;
				// Le point le plus proche de A est P
				// la distance [ppos,P] vaut norme de AP
				IP_squarelength = NVec2SquareLength(&IP);
				if (IP_squarelength < max_IP_squarelength)
				{
					trk_primind = i;
					ptrk_prim = pprim;
					ptrk_kp = pkp1;

					pout->s = pkp0->s + dot0;
					pout->p = I;
					pout->k = 0.0f;
					pout->u = pkp1->u;

					max_IP_squarelength = IP_squarelength;
					max_IP_length = sqrt(max_IP_squarelength);

					rtrn = TRCK_RESULT::ON_PATH_SEGMENT;
				}
			}
			break;

		case NLPATH_PRIMITIVE_ID_ARC:
			// METHODE 2: ( pour m�moire voir "LEGACY METHOD 1" plus bas dans ce fichier)
			// +--------------------------------------------------------------------------------------
			// |
			// |	Calcul Bissectrice
			// |
			// |	A :		pkp0 par rapport au centre du cercle
			// |	B :		pkp1 par rapport au centre du cercle
			// |	bsc:	vecteur bissectrice de langle AcB ( ou c est le centre du cercle )
			// |

			A.x = pkp0->p.x - pprim->m_arc.m_center.x;
			A.y = pkp0->p.y - pprim->m_arc.m_center.y;
			B.x = pkp1->p.x - pprim->m_arc.m_center.x;
			B.y = pkp1->p.y - pprim->m_arc.m_center.y;
			bsc.x = A.x + B.x;
			bsc.y = A.y + B.y;

			if (bsc.x || bsc.y)
			{
				bsc_threshold = NVec2Normalize(&bsc) / (2.0f * pprim->m_arc.m_radius);

				if (pprim->m_arc.m_omega > NF32_PI)
				{
					bsc.x = -bsc.x;
					bsc.y = -bsc.y;
					bsc_threshold = -bsc_threshold;
				}
			}
			else // bsc est le vecteur null !
			{
				NErrorIf(pprim->m_arc.m_omega != NF32_PI, NERROR_SYSTEM_GURU_MEDITATION);

				// Vu le mode de calcul de la bissectrice, bsc null revient � dire que l'angle ACB vaut PI ...
				// La bissectrice est donc un vecteur perpendiculaire � CA et CB
				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
				{
					bsc.x = A.y;
					bsc.y = -A.x;
				}
				else
				{
					bsc.x = -A.y;
					bsc.y = A.x;
				}

				NVec2Normalize(&bsc);
				bsc_threshold = 0.0f;
			}
			// |
			// |
			// +--------------------------------------------------------------------------------------

			P.x = ppos->x - pprim->m_arc.m_center.x;
			P.y = ppos->y - pprim->m_arc.m_center.y;
			length = NVec2Length(&P);
			f = length - pprim->m_arc.m_radius;
			if (NABS(f) < max_IP_length)
			{
				// 'ppos' est dans l'anneau ( potentiellement un disc si max_AP_length >= radius ) des positions situ�es � une dist. valide du cercle de centre C portant l'arc.
				if (length)
				{
					if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
					{
						ortho_bsc.x = bsc.y;
						ortho_bsc.y = -bsc.x;
					}
					else
					{
						ortho_bsc.x = -bsc.y;
						ortho_bsc.y = bsc.x;
					}

					P.x /= length;
					P.y /= length;

					dot0 = NVec2DotProduct(&P, &bsc);
					dot1 = NVec2DotProduct(&P, &ortho_bsc);

					if (dot0 > bsc_threshold)
					{
						dot0 = NCLAMP(-1.0f, dot0, 1.0f);
						ang = acosf(dot0);

						// ang = (dot1 < 0.0f) ? pprim->m_arc.m_phi + pprim->m_arc.m_omega / 2.0f - ang : pprim->m_arc.m_phi + pprim->m_arc.m_omega / 2.0f + ang;
						if (dot1 < 0.0f)
							ang = pprim->m_arc.m_omega / 2.0f - ang;
						else
							ang = pprim->m_arc.m_omega / 2.0f + ang;

						trk_primind = i;
						ptrk_prim = pprim;
						ptrk_kp = pkp1;

						pout->s = pkp0->s + ang * pprim->m_arc.m_radius;
						pout->p.x = pprim->m_arc.m_center.x + P.x * pprim->m_arc.m_radius;
						pout->p.y = pprim->m_arc.m_center.y + P.y * pprim->m_arc.m_radius;
						pout->k = pkp1->k; // pkp0->k peut �tre = 0 en cas d'absence de clothoide de transition :/
										   // alors que pkp1->k aura tjrs le 'bon k', celui qui doit �tre naturellement associ� � la primitive [pkp0,pkp1]
						if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
						{
							pout->u.x = P.y;
							pout->u.y = -P.x;
						}
						else
						{
							pout->u.x = -P.y;
							pout->u.y = P.x;
						}

						max_IP_length = NABS(f);
						max_IP_squarelength = NPOW2(max_IP_length);

						rtrn = TRCK_RESULT::ON_PATH_ARC;
					}
					else // dot0 <= bsc_threshold
					{

						if (dot1 > 0.0f)
						{
							// plus pres de B
							IP.x = ppos->x - pkp1->p.x;
							IP.y = ppos->y - pkp1->p.y;
							IP_squarelength = NVec2SquareLength(&IP);
							if (IP_squarelength < max_IP_squarelength)
							{
								trk_primind = i;
								ptrk_prim = pprim;
								ptrk_kp = pkp1;

								*pout = *pkp1;

								max_IP_squarelength = IP_squarelength;
								max_IP_length = sqrt(max_IP_squarelength);

								rtrn = TRCK_RESULT::ON_PATH_ARC_END;
							}
						}
						else if (dot1 < 0.0f)
						{
							// plus pres de A
							IP.x = ppos->x - pkp0->p.x;
							IP.y = ppos->y - pkp0->p.y;
							IP_squarelength = NVec2SquareLength(&IP);
							if (IP_squarelength < max_IP_squarelength)
							{
								trk_primind = i;
								ptrk_prim = pprim;
								ptrk_kp = pkp1;

								pout->s = pkp0->s;
								pout->p = pkp0->p;
								pout->k = pkp0->k; ///// ???? ou pkp1->k pour etre sur d'avoir ici le k de l'arc
								pout->u = pkp0->u;

								max_IP_squarelength = IP_squarelength;
								max_IP_length = sqrt(max_IP_squarelength);

								rtrn = TRCK_RESULT::ON_PATH_ARC_START;
							}
						}
						else // dot1 == 0.0f
						{
							// sur la bissectrice
							I.x = pprim->m_arc.m_center.x + bsc.x * pprim->m_arc.m_radius;
							I.y = pprim->m_arc.m_center.y + bsc.y * pprim->m_arc.m_radius;
							IP.x = ppos->x - pkp0->p.x;
							IP.y = ppos->y - pkp0->p.y;
							IP_squarelength = NVec2SquareLength(&IP);
							if (IP_squarelength < max_IP_squarelength)
							{
								trk_primind = i;
								ptrk_prim = pprim;
								ptrk_kp = pkp1;

								pout->s = pkp0->s + pprim->m_core.m_l / 2.0f;
								pout->p.x = pprim->m_arc.m_center.x + bsc.x * pprim->m_arc.m_radius;
								pout->p.y = pprim->m_arc.m_center.y + bsc.y * pprim->m_arc.m_radius;
								pout->k = pkp1->k; // pkp0->k peut �tre = 0 en cas d'absence de clothoide de transition :/
												   // alors que pkp1->k aura tjrs le 'bon k', celui qui doit �tre naturellement associ� � la primitive [pkp0,pkp1]
								if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
								{
									pout->u.x = bsc.y;
									pout->u.y = -bsc.x;
								}
								else
								{
									pout->u.x = -bsc.y;
									pout->u.y = bsc.x;
								}

								max_IP_squarelength = IP_squarelength;
								max_IP_length = sqrt(max_IP_squarelength);

								rtrn = TRCK_RESULT::ON_PATH_ARC_BISSECTOR;
							}
						}
					}
				}
				else // length == 0
				{
					// 'ppos' est confondue avec le centre du cercle, mais si nous sommes ici c'est que le centre du cercle fait partie de l'anneau/disc des positions valides...
					// comme le centre du cercle est bien �videment surla bisectrice, on consid�re que la projection de 'ppos' sur l'arc se trouve � l'intersection arc/bissectrice.
					trk_primind = i;
					ptrk_prim = pprim;
					ptrk_kp = pkp1;

					pout->s = pkp0->s + pprim->m_core.m_l / 2.0f;
					pout->p.x = pprim->m_arc.m_center.x + bsc.x * pprim->m_arc.m_radius;
					pout->p.y = pprim->m_arc.m_center.y + bsc.y * pprim->m_arc.m_radius;
					pout->k = pkp1->k; // pkp0->k peut �tre = 0 en cas d'absence de clothoide de transition :/
									   // alors que pkp1->k aura tjrs le 'bon k', celui qui doit �tre naturellement associ� � la primitive [pkp0,pkp1]
					if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
					{
						pout->u.x = bsc.y;
						pout->u.y = -bsc.x;
					}
					else
					{
						pout->u.x = -bsc.y;
						pout->u.y = bsc.x;
					}

					max_IP_length = pprim->m_arc.m_radius;
					max_IP_squarelength = NPOW2(max_IP_length);

					rtrn = TRCK_RESULT::ON_PATH_ARC_CENTER;
				}
			}
			break;

		case NLPATH_PRIMITIVE_ID_CLOTHOID:
			// Rappels:
			// min			Abscisse curv. min sur la clothoide |_ d�finissent l'intervalle [min,max] de recherche de l'abscisse curv, de I image de P sur la clothoide.
			// max			Abscisse curv. max sur la clothoide |
			// clth_o		Origine de construction geometrique de la clothoide ( son point de d�part, l� ou k = 0 )
			// clth_Iu		Vecteur tangent � la clothoide en I, image de P. Ici I sera la projection de
			// clth_Is		Abscisse curviligne de I

			// 1) On va d'abord chercher � exclure un pppos situ� � l'ext�rieur de la zone d�limit�e par la fin g�om�trique de la clothoide ( l� ou k!= 0 )
			if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
			{
				// La clothoide commence en pkp1 (k=0) et se termine en pkp0 (k!=0).
				P.x = ppos->x - pkp0->p.x;
				P.y = ppos->y - pkp0->p.y;
				dot0 = NVec2DotProduct(&P, &pkp0->u);
				if (dot0 <= 0.0f)
				{
					// La seule image I de P possible pourrait encore �tre la fin de la clothoide (k!=0) si P n'est pas trop loin d'elle.
					IP_squarelength = NVec2SquareLength(&P);
					if (IP_squarelength < max_IP_squarelength)
					{
						// Si la clothoid est PREM', alors l'origin de la clothoide ( o� k = 0 ) correspond � l'entr�e sur la clothoide dans le sens du chemin
						*pout = *pkp0;
						rtrn = rtrn = TRCK_RESULT::ON_PATH_CLOTHOID_START; // !!! ICI LA FIN GEOMETRIQUE de la clothoide correspond � l'entr�e de la clothoide dans le sens du chemin !!!
					}
					break;
				}
				/*
				else ... ppos est "du bon c�t� et reste donc, pour le moment un candidat valide ...
				*/

				// preparation du calcul de clth_of:
				clth_o = pkp1->p;			  // Origine de construction geometrique de la clothoide ( son point de d�part, l� ou k = 0 ) et fin ( l� ou k = value )
				tmp.x = pkp0->p.x - clth_o.x; // |___La fin de la construction g�om�trique de la clothoide par rapport � son Origine.
				tmp.y = pkp0->p.y - clth_o.y; // |
			}
			else
			{
				// La clothoide commence en pkp0 (k=0) et se termine en pkp1 (k!=0).
				P.x = ppos->x - pkp1->p.x;
				P.y = ppos->y - pkp1->p.y;
				dot0 = NVec2DotProduct(&P, &pkp1->u);
				if (dot0 >= 0.0f)
				{
					// La seule image I de P possible pourrait encore �tre la fin de la clothoide (k!=0) si P n'est pas trop loin d'elle.
					IP_squarelength = NVec2SquareLength(&P);
					if (IP_squarelength < max_IP_squarelength)
					{
						// Si la clothoid est PREM', alors l'origin de la clothoide ( o� k = 0 ) correspond � l'entr�e sur la clothoide dans le sens du chemin
						*pout = *pkp1;
						rtrn = rtrn = TRCK_RESULT::ON_PATH_CLOTHOID_END; // !!! ICI LA FIN GEOMETRIQUE de la clothoide correspond � la sortie de la clothoide dans le sens du chemin !!!
					}
					break;
				}
				/*
				else ... ppos est "du bon c�t� et reste donc, pour le moment un candidat valide ...
				*/

				// preparation du calcul de clth_of:
				clth_o = pkp0->p;			  // Origine de construction geometrique de la clothoide ( son point de d�part, l� ou k = 0 ) et fin ( l� ou k = value )
				tmp.x = pkp1->p.x - clth_o.x; // |___La fin de la construction g�om�trique de la clothoide par rapport � son Origine.
				tmp.y = pkp1->p.y - clth_o.y; // |
			}

			// Fin de construction geometrique de la clothoide ( son point de d'arriv�e, l� ou k != 0 ) exprim�e dans la base [clth_o,i,j] de la clothoide.
			clth_of.x = tmp.x * pprim->m_clothoid.m_i.x + tmp.y * pprim->m_clothoid.m_i.y;
			clth_of.y = tmp.x * pprim->m_clothoid.m_j.x + tmp.y * pprim->m_clothoid.m_j.y;

			// P Expression de 'ppos' dans la base i,j d'origine 'm_co' de la clothoide
			tmp.x = ppos->x - clth_o.x;
			tmp.y = ppos->y - clth_o.y;
			P.x = tmp.x * pprim->m_clothoid.m_i.x + tmp.y * pprim->m_clothoid.m_i.y; // v .  *m_pClothoid->m_i ( dot product, produit scalaire )
			P.y = tmp.x * pprim->m_clothoid.m_j.x + tmp.y * pprim->m_clothoid.m_j.y; // v .  *m_pClothoid->m_j ( dot product, produit scalaire )

			// Recherche de P dans le volume englobant des positions potentielles valides
			if (P.x < 0.0f)
			{
				// P semble �tre un mauvais candidat ... Son abscisse dans la base[clth_0, i, j] de la clothoide le place du "MAUVAIS c�t�" de l'axe j
				// P est situ� avant l'origine de la clothoide, c'est � dire avant l'extr�mit� de la clothoide o� k = 0
				// Soit, dans le sens du chemin, ... AVANT pkp0 si la clothoide est Prem'
				//								 ... APRES pkp1 si la clothoide est SECOND
				//
				// La seule image I de P possible pourrait encore �tre l'origine de la clothoide si P n'est pas trop loin d'elle.
				IP_squarelength = NVec2SquareLength(&P);
				if (IP_squarelength < max_IP_squarelength)
				{
					trk_primind = i;
					ptrk_prim = pprim;
					ptrk_kp = pkp1;

					if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
					{
						// Si la clothoid est SECOND, alors l'origin de la clothoide ( o� k = 0 ) correspond � la sortie de la clothoide dans le sens du chemin
						*pout = *pkp1;
						rtrn = rtrn = TRCK_RESULT::ON_PATH_CLOTHOID_END;
					}
					else
					{
						// Si la clothoid est PREM', alors l'origin de la clothoide ( o� k = 0 ) correspond � l'entr�e sur la clothoide dans le sens du chemin
						*pout = *pkp0;
						rtrn = rtrn = TRCK_RESULT::ON_PATH_CLOTHOID_START;
					}

					max_IP_squarelength = IP_squarelength;
					max_IP_length = sqrt(max_IP_squarelength);
				}
				break;
			}
			/*
			else  (P.x >= 0.0f) ...
			{
				P est un bon candidat ... Son abscisse dans la base [clth_0,i,j] de la clothoide le place du "bon c�t�" de l'axe j
			}
			*/
			if (P.y < -max_IP_length)
				break;

			if (P.y > (clth_of.y + max_IP_length))
				break;

			if (P.x > (clth_of.x + max_IP_length))
				break;

			// Utilisation de la m�thode Newton-Raphson pour trouver le point situ� sur la clothoide et le plus proche de 'ppos'. https://fr.wikipedia.org/wiki/M%C3%A9thode_de_Newton
			min = 0.0f;
			max = pprim->m_clothoid.m_core.m_l;
			i = 0; // compteur d'it�rations

			I.x = 0.0f;		  // |__	On place le premier projet� de P sur l'origine.
			I.y = 0.0f;		  // |
			IP = P;			  // |--	I �tant confondu avec l'origine, on a ici IP = P
							  //
			f = P.x;		  // |	f correspondant � la projection de P sur la tangente en I, et sachant que ici la tangente en I ( = origine ) n'est autre que le vecteur i de la base
							  // |	On a f = P.x
							  //
			clth_Iu.x = 1.0f; // |__	La tangente en I ( = origine ) n'est autre que le vecteur i donc : clth_Iu(1,0)
			clth_Iu.y = 0.0f; // |
							  //
			clth_Is = 0.0f;	  // |--	L'abscisse curviligne en I ( = origine ) vaut 0.

			while (NABS(f) > m_clothoidErrorThreshold)
			{
				// Rep�rage du point projet� sur la tangente [u] en p ( d'abscisse s ) par rapport au s courant.
				// le point projet� est-il avant p ( l<0 ) ou apr�s p (l>0)
				if (f < 0.0f)
				{
					max = clth_Is;
					ds = (max - min) / 2.0f;
					clth_Is += NMAX(-ds, f);
				}
				else
				{
					min = clth_Is;
					ds = (max - min) / 2.0f;
					clth_Is += NMIN(ds, f);
				}

				// On recalcul la nouvelle Image ( = I) de P � partir de sa nouvelle abscisse (=clth_Is)
				NFresnelIntegralsf32(clth_Is / pprim->m_clothoid.m_param, &cf, &sf);
				I.x = pprim->m_clothoid.m_param * cf;
				I.y = pprim->m_clothoid.m_param * sf;

				// ainsi que la tangente en I ( = clth_Iu ):
				theta = pprim->m_clothoid.m_sigma * NPOW2(clth_Is) / 2.0f;
				clth_Iu.x = cos(theta);
				clth_Iu.y = sin(theta);

				// On proj�te � nouveau P sur cette tangente en I
				IP.x = P.x - I.x;
				IP.y = P.y - I.y;
				f = IP.x * clth_Iu.x + IP.y * clth_Iu.y; // dot product = error

				// notes:
				// Si I est "parfait" alors le dot product ( = la projection de P sur la tangente en I ) est �gal � ZERO ...
				// Sinon, et c'est le r�le de cet algo on d�place I ( son abscisse curviligne ) on trouve ses coordonn�es X,Y et celle de la tangente en ce point
				// et on reteste ...
				i++;
			}

			// Dernier test sur le I trouv�, est-il suffisement proche de P ?
			IP_squarelength = NVec2SquareLength(&IP);
			if (IP_squarelength < max_IP_squarelength)
			{
				// Compute Result:
				trk_primind = i;
				ptrk_prim = pprim;
				ptrk_kp = pkp1;

				if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_CLOTHOID_SECOND))
				{
					pout->u.x = -pprim->m_clothoid.m_i.x * clth_Iu.x - pprim->m_clothoid.m_j.x * clth_Iu.y;
					pout->u.y = -pprim->m_clothoid.m_i.y * clth_Iu.x - pprim->m_clothoid.m_j.y * clth_Iu.y;
					pout->s = pkp0->s + pprim->m_clothoid.m_core.m_l - clth_Is;
				}
				else
				{
					pout->u.x = pprim->m_clothoid.m_i.x * clth_Iu.x + pprim->m_clothoid.m_j.x * clth_Iu.y;
					pout->u.y = pprim->m_clothoid.m_i.y * clth_Iu.x + pprim->m_clothoid.m_j.y * clth_Iu.y;
					pout->s = pkp0->s + clth_Is;
				}
				pout->p.x = clth_o.x + pprim->m_clothoid.m_i.x * I.x + pprim->m_clothoid.m_j.x * I.y;
				pout->p.y = clth_o.y + pprim->m_clothoid.m_i.y * I.x + pprim->m_clothoid.m_j.y * I.y;
				pout->k = pkp0->k + (pkp1->k - pkp0->k) * (pout->s / pprim->m_clothoid.m_core.m_l);

				max_IP_squarelength = IP_squarelength;
				max_IP_length = sqrt(max_IP_squarelength);

				rtrn = TRCK_RESULT::ON_PATH_CLOTHOID;
			}
			break;

		default:
			break;
		}
	}

	return rtrn;
}

// LEGACY

/*
Nbool NLPATH_PERSISTENT_TRACKING_DATA::forwardContinuousUpdate(const NVEC2f32 *ppos)
{
	NLPATH_POINT		*pkp1,*pkp0;
	NLPATH_PRIMITIVE	*pprim;
	Nu32				i;
	NVEC2				A;
	Nf32				dot1,dot0;
	Nf32				ang;

	m_iteration = 0;

	pkp1	= m_pTrackedKeyPoint;
	pprim	= m_pTrackedPrimitive;
	for (i = m_trackedPrimitiveIndex; i < m_primitiveNb; i++, pprim++, pkp1++)
	{
		A.x = ppos->x - pkp1->p.x;
		A.y = ppos->y - pkp1->p.y;
		dot1 = NVec2DotProduct(&A, &pkp1->u);
		// ppos est "avant" la "fin" de la primitive
		if (dot1 < 0.0f)
		{
			m_pTrackedPrimitive		= pprim;
			m_pTrackedKeyPoint		= pkp1;
			m_trackedPrimitiveIndex = i;

			pkp0 = pkp1 - 1;
			A.x = ppos->x - pkp0->p.x;
			A.y = ppos->y - pkp0->p.y;
			dot0 = NVec2DotProduct(&A, &pkp0->u);
			if (dot0 <= 0.0f)
			{
				m_pointOnPath = *pkp0;
				return NTRUE;
			}
			else
			{

				// recherche "SUR" la primitive
				switch (pprim->m_core.m_id)
				{
					case NLPATH_PRIMITIVE_ID_SEGMENT:
						//NErrorIf(dot0 >= 1.0f, NERROR_INCONSISTENT_VALUES);
						m_pointOnPath.s		= pkp0->s + dot0;
						m_pointOnPath.p.x	= pkp0->p.x + dot0*pkp0->u.x;
						m_pointOnPath.p.y	= pkp0->p.y + dot0*pkp0->u.y;
						m_pointOnPath.k		= 0.0f; // ! la primitive est un segment , donc ...
						//NErrorIf( (pkp0->k != 0.0f) || (pkp1->k != 0.0f), NERROR_INCONSISTENT_VALUES); ! en fait non car il est possible que cette condition ne soit pas respect�e !! A cause des Arc de cercles sans clothoides qui cr�ent une discontinuit� de courbure !!!!
						m_pointOnPath.u		= pkp0->u;
						return NTRUE;

					case NLPATH_PRIMITIVE_ID_ARC:
						A.x = ppos->x - pprim->m_arc.m_center.x;
						A.y = ppos->y - pprim->m_arc.m_center.y;
						NVec2Normalize(&A);
						dot0 = NVec2DotProduct(&A, &pprim->m_arc.m_i); // cos
						dot1 = NVec2DotProduct(&A, &pprim->m_arc.m_j); // Sin
						ang = atan2(dot1, dot0) - pprim->m_arc.m_phi;

						if (ang >= 0.0f && ang <= pprim->m_arc.m_omega)
						{
							m_pointOnPath.s		= pkp0->s + ang * pprim->m_arc.m_radius;
							m_pointOnPath.p.x	= pprim->m_arc.m_center.x + A.x*pprim->m_arc.m_radius;
							m_pointOnPath.p.y	= pprim->m_arc.m_center.y + A.y*pprim->m_arc.m_radius;
							m_pointOnPath.k		= pkp0->k;
							if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
							{
								m_pointOnPath.u.x =  A.y;
								m_pointOnPath.u.y = -A.x;
							}
							else
							{
								m_pointOnPath.u.x = -A.y;
								m_pointOnPath.u.y =  A.x;
							}
						}
						return NTRUE;

					case NLPATH_PRIMITIVE_ID_CLOTHOID:
						if ((NLPATH_CLOTHOID*)pprim != m_PersistentClothoidTrackingData.m_pClothoid)
							m_PersistentClothoidTrackingData.setup(pkp0, pkp1, (NLPATH_CLOTHOID*)pprim, 0.001f);

						m_iteration = m_PersistentClothoidTrackingData.update( ppos );
						return NTRUE;

					default:
						return NFALSE;
				}
			}
		}
		else if (dot1 == 0.0f) // ppos est sur la perpendiculaire � la tangente de la "fin" de la primitive ... ( ??? voir � ajouter une tol�rance ici  ??? )
		{
			m_pTrackedPrimitive		= pprim;
			m_pTrackedKeyPoint		= pkp1;
			m_trackedPrimitiveIndex = i;

			m_pointOnPath			= *pkp1;
			return NTRUE;
		}
		/ *
		else // dot > 0.0f
		{
			// Signifie que ppos est "apr�s" la fin de la primitive, du coup on passe � la primitive suivante ...
			// ... en laissant "naturellement" la boucle for avancer.
		}
		* /
	}

	// Si on arrive ici c'est que ppos est "apr�s" la derni�re primitive.
	// En cons�quence ...
	m_pTrackedPrimitive = pprim - 1;
	m_pTrackedKeyPoint	= pkp1 - 1;
	m_trackedPrimitiveIndex = m_primitiveNb - 1;
	// Ces valeurs permettrons, le cas �ch�ant de refaire un nouveau 'forwardUpdate'
	m_pointOnPath = *m_pTrackedKeyPoint;
	return NTRUE;
}
*/

/*
#ifdef _NEDITOR
void NLPATH_PERSISTENT_TRACKING_DATA::draw(const NVEC2f32 * pxtend, NCOLOR * pcolor)
{
	NVEC3f32	v,v1;
	v.x = m_pTrackedKeyPoint->p.x;
	v.y = m_pTrackedKeyPoint->p.y;
	v.z = 0.0f;
	NUT_Draw_Cross(&v, pxtend,pcolor);

	v.x = m_pointOnPath.p.x;
	v.y = m_pointOnPath.p.y;
	v.z = 0.0f;
	NUT_Draw_Cross(&v, pxtend, pcolor);
	v1.x = v.x + m_pointOnPath.u.x;
	v1.y = v.y + m_pointOnPath.u.y;
	v1.z = 0.0f;
	NUT_Draw_Segment(&v, &v1, pcolor);


	Nchar txt[16];
	NUT_DRAWTEXT	drawtxt;
	Nmem0(&drawtxt, NUT_DRAWTEXT);
	drawtxt.Size = 0.1f;
	drawtxt.Color = *pcolor;
	sprintf(txt, "  %d", m_iteration);
	NUT_Draw_Text(txt, &v, &drawtxt);
}


#endif
*/

/*
 * -------------------------------------------------------------------------------------------------------------------------------------------------------------------
case NLPATH_PRIMITIVE_ID_ARC:
		// METHODE 1:
			// Position par rapport au centre du cercle 'portant' l'arc:
		A.x = ppos->x - pprim->m_arc.m_center.x;
		A.y = ppos->y - pprim->m_arc.m_center.y;
		length = NVec2Normalize(&A);
		f = length - pprim->m_arc.m_radius;

		if ((NABS(f) < max_AP_length))
		{
			if (length)
			{
				dot0 = NVec2DotProduct(&A, &pprim->m_arc.m_i); // cos
				dot1 = NVec2DotProduct(&A, &pprim->m_arc.m_j); // Sin
				ang  = atan2(dot1, dot0) - pprim->m_arc.m_phi;

				if (ang >= 0.0f)
				{
					if (ang <= pprim->m_arc.m_omega)
					{
						trk_primind = i;
						ptrk_prim	= pprim;
						ptrk_kp		= pkp1;

						m_pointOnPath.s		= pkp0->s + ang * pprim->m_arc.m_radius;
						m_pointOnPath.p.x	= pprim->m_arc.m_center.x + A.x * pprim->m_arc.m_radius;
						m_pointOnPath.p.y	= pprim->m_arc.m_center.y + A.y * pprim->m_arc.m_radius;
						m_pointOnPath.k		= pkp1->k;	// pkp0->k peut �tre = 0 en cas d'absence de clothoide de transition :/
														// alors que pkp1->k aura tjrs le 'bon k', celui qui doit �tre naturellement associ� � la primitive [pkp0,pkp1]
						if (ISFLAG_ON(pprim->m_core.m_flags, FLAG_NLPATH_ARC_INDIRECT))
						{
							m_pointOnPath.u.x = A.y;
							m_pointOnPath.u.y = -A.x;
						}
						else
						{
							m_pointOnPath.u.x = -A.y;
							m_pointOnPath.u.y = A.x;
						}

						max_AP_length		= NABS(f);
						max_AP_squarelength = NPOW2(max_AP_length);
					}
					else //(ang > pprim->m_arc.m_omega)
					{
						// ppos est apr�s la fin de l'arc, cependant comme il est situ� sur l'anneau des positions situ�es � une dist. valide du cercle...
						// on cherche � voir si il ne serait pas suffisement proche de pkp1 ( on pourrait utiliser la longueur de l'arc ( ang - pprim->m_arc.m_omega) comme bonne approx.
						// mais soyons fous et pr�cis , comme on recherche une distance '� vol d'oiseau' calculons une distance '� vol d'oiseau' ...
						A.x = ppos->x - pkp1->p.x;
						A.y = ppos->y - pkp1->p.y;
						f = NVec2SquareLength(&A);
						if (f < max_AP_squarelength)
						{
							trk_primind = i;
							ptrk_prim	= pprim;
							ptrk_kp		= pkp1;

							m_pointOnPath = *pkp1;

							max_AP_squarelength = f;
							max_AP_length = sqrt(max_AP_squarelength);
						}
					}
				}
				else //(ang < 0.0f)
				{
					// l'angle est inf�rieur � 0 donc "AVANT" le d�but de l'arc.
					// ppos est avant le d�but de l'arc, cependant comme il est situ� sur l'anneau des positions situ�es � une dist. valide du cercle...
					// on cherche � voir si il ne serait pas suffisement proche de pkp0 . On proc�de donc comme precedement (  ang > pprim->m_arc.m_omega ) mais pour pkp0
					A.x = ppos->x - pkp0->p.x;
					A.y = ppos->y - pkp0->p.y;
					f = NVec2SquareLength(&A);
					if (f < max_AP_squarelength)
					{
						trk_primind = i;
						ptrk_prim	= pprim;
						ptrk_kp		= pkp1;

						m_pointOnPath = *pkp0;	// on remarque ici que la solution aura le k de pkp0 ! qui potentiellement peut-�tre null ! ( si pas de clothoide )
												// cependant cela est acceptable car le point est clairement AVANT le d�but de l'arc. Et si pkp0->k vaut ZERO c'est qu'avant le d�but de l'arc
												// se trouve un segment ...

						max_AP_squarelength = f;
						max_AP_length = sqrt(max_AP_squarelength);
					}
				}
			}
			else // length == 0 !
			{
				// ppos et le centre cu cercle sont confondus, donc A est le vecteur null !!!
				// hors  atan2(0,0) renvoie Nan donc ...
				// impossible de calculer ang dans ces conditions (  trivial )
				// Cependant si nous sommes ici c'est que ppos est sur "l'anneau" des positions situ�es � une distance valide des points du cercle "portant" l'arc �tudi�.
				// ... et plus pr�cisement dans le cas particulier ou l'anneau est un disque comprenant le centre de ce cercle,
				// car la distance max autoris�e est plus grande que le rayon !!
				// DONC, comme ppos et lecentre du cercle sont confondus, tous les points du cercle ( et donc de l'arc ) sont situ� � la m�me distance valide ( = rayon )
				// de ppos. Tous les points de l'arc sont donc solution !!!
				// Plusieurs possibilit�s s'offrre � nous:
				// 1/ ne rien retourner, et consid�rer qu'il n'y a pas de solution			:(  >>> Dommage car ppos est � une distance valide de l'arc !
				// 2/ retourner pkp0 comme solution											:/	>>> cela n'est pas faut mais si juste avant nous �tions apr�s pkp1 il y aura un 'saut'
				// 3/ retourber pkp1 comme solution											:/	>>> cela n'est pas faut mais si juste avant nous �tions avant pkp0 il y aura un 'saut'
				// 4/ retourber le point situ� sur la bissectrice de l'angle [pkp0,A,pkp1]	:/	>>> cela n'est pas faut mais  il y aura un 'saut' qu'on vienne d'un c�t� ou de l'autre
				// 5/ utiliser un historique ( position pr�c�dente ) pour choisir ...		:)	>>> on choisirait pkp0 ou pkp1 en fonction d'o� on vient ...
				//
				// Pour le moment et pour faire simple on choisira la solution 2/ et on retournera pkp0 comme solution
				// Cependant, il pourrait �tre judicieurs � l'avenir d'impl�menter la solution 5/
				trk_primind = i;
				ptrk_prim = pprim;
				ptrk_kp = pkp1;

				m_pointOnPath = *pkp0;		// on remarque ici que la solution aura le k de pkp0 ! qui potentiellement peut-�tre null ! ( si pas de clothoide )
											// cependant cela est acceptable car pppos est autant sur le tout d�but de l'arc que sur la toute fin de la primitive precedente
											// si elle existe ...Et si pkp0->k vaut ZERO c'est qu'avant le d�but de l'arc se trouve un segment ...

				max_AP_length		= pprim->m_arc.m_radius;	// nous sommes dans le cas ou length = 0 ! Donc NABS(f) = NABS( 0 - pprim->m_arc.m_radius)
																// ... et en plus, si nous sommes ici c'est �galement que (NABS(f) < max_AP_length). Donc ...
				max_AP_squarelength = NPOW2(max_AP_length);
			}
		}
		 * -------------------------------------------------------------------------------------------------------------------------------------------------------------------
		 */