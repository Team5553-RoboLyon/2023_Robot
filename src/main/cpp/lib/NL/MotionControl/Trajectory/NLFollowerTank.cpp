#include "lib/N/Miscellaneous/NString.h"
// #include "../../../N/Utilities/NUT_Logging.h"
// #include "../../../N/Utilities/Draw/NUT_Draw.h"
// #include "../../../N/Utilities/Draw/NUT_DrawPencil.h"
#include "lib/NL/MotionControl/DriveTrain/NLOdometry.h"
#include "lib/NL/MotionControl/Drivetrain/Characterization/NLCharacterizationTable.h"
// #include "../../NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
// #include "../../NLLogsChannels.h"

#include "lib/NL/MotionControl/Trajectory/NLFollowerTank.h"

#ifdef _NEDITOR
Nu32 NLFOLLOWER_TANK::read(NLPATH_WORKBENCH *pwb)
{
	if (!m_driveTrainSpecifications.read(pwb))
		return 0;
	return m_ramsete.read(pwb);
}
#endif

Nu32 NLFOLLOWER_TANK::read(FILE *pfile)
{

	// 1) lecture Version
	Nu32 version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	// 2) lecture Header
	// NLFOLLOWER_TANK_HEADER	header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLFOLLOWER_TANK_HEADER):
		/*
		if (fread(&header, sizeof(NLFOLLOWER_TANK_HEADER), 1, pfile) != 1)
			return 0;
		? = header.?;
		*/
		// 3) lecture data
		if (!m_driveTrainSpecifications.read(pfile))
			return 0;
		return m_ramsete.read(pfile);

	default:
		break;
	}
	return 0;
}

Nu32 NLFOLLOWER_TANK::write(FILE *pfile)
{
	// 1) ecriture Version
	Nu32 version_u32 = VERSION_NLFOLLOWER_TANK_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) ecriture Header
	/*
	NLFOLLOWER_TANK_HEADER	header;
	header.? = ?;
	if (fwrite(&header, sizeof(NLFOLLOWER_TANK_HEADER), 1, pfile) != 1)
		return 0;
	*/
	// 3) ecriture data
	if (!m_driveTrainSpecifications.write(pfile))
		return 0;

	return m_ramsete.write(pfile);
}

Nu32 NLFOLLOWER_TANK::save(const Nchar *pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(pfilename, EXTENSION_NLFOLLOWER_TANK_BIN))
		return 0;

	// 0) Ouverture du fichier en ecriture
	FILE *pfile = fopen(pfilename, "wb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Ecriture Signature
	Nu32 _u32 = SIGNATURE_NLFOLLOWER_TANK_BIN;
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		return 0;
	}
	// 2) Ecriture :
	if (!write(pfile))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_WRITE_ERROR);
		return 0;
	}
	fclose(pfile);
	return 1;
}

Nu32 NLFOLLOWER_TANK::load(const Nchar *pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(pfilename, EXTENSION_NLFOLLOWER_TANK_BIN))
		return 0;

	// 0) Ouverture du fichier en lecture
	FILE *pfile = fopen(pfilename, "rb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Lecture Signature
	Nu32 _u32;
	if (fread(&_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}
	if (!NSIGNATURECMP(_u32, SIGNATURE_NLFOLLOWER_TANK_BIN))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_UNKNOWN_SIGNATURE);
		return 0;
	}
	// 2) Lecture :
	if (!read(pfile))
	{
		fclose(pfile);
		NErrorIf(1, NERROR_FILE_READ_ERROR);
		return 0;
	}

	fclose(pfile);
	return 1;
}

Nu32 NLFOLLOWER_TANK::importTxt(const Nchar *ptxtfilename)
{

	NErrorIf(!ptxtfilename, NERROR_NULL_POINTER);
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(ptxtfilename, ".txt"))
		return 0;

	FILE *pfile;
	Nchar tempstring[1024];
	Nchar name[32];
	Nchar *pstr;

	pfile = fopen(ptxtfilename, "r"); // ouverture du fichier
	fseek(pfile, 0, SEEK_SET);		  // on se place au debut du fichier

	// recuperer la siganture du fichier
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_String_AfterLabel(pstr, "signature= ", name);
	if (!strcmp(name, SIGNATURE_NLRAMSETE))
	{
		fclose(pfile);
		return m_ramsete.importTxt(ptxtfilename);
	}
	else if (!strcmp(name, SIGNATURE_NLDRIVETRAINSPECS))
	{
		fclose(pfile);
		return m_driveTrainSpecifications.importTxt(ptxtfilename);
	}
#ifdef _DEBUG
	else
	{
		NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
#endif

	return 1;
	// NLCHARACTERIZATION_TABLE	characterization_table(4) ... is going to be clear.
}

void NLFOLLOWER_TANK::initialize(const NLTRAJECTORY_PACK *ppack)
{

	m_currentTime = 0.0f;
	m_leftDistance = 0.0f;
	m_rightDistance = 0.0f;
	m_estimatedPose.reset();
	m_persistent.initialize(ppack);

	// posted Messages
	NErrorIf(ppack->m_postedMessagesArray.Size < 2, NERROR_SYSTEM_CHECK);
	m_pToMessage = (NLTRJ_POSTED_MESSAGE *)ppack->m_postedMessagesArray.pFirst; // On positionne le To sur le premier message ( le message d'amorce, donc un Fake )
	m_pFromMessage = m_pToMessage + 1;											// On positionne le From ( donc la "tete de lecture" ) sur le premier VRAI message ( donc APRES m_pToMessage )

	m_driveTrainSpecifications = ppack->m_driveTrainSpecifications;
	m_ramsete = ppack->m_ramsete;

#ifdef _NEDITOR
	NEraseRingBuffer(&m_errorBuffer);
#endif
}

void NLFOLLOWER_TANK::estimate(const Nf32 left_encoder_rev_count, const Nf32 right_encoder_rev_count, const Nf32 gyro_angle)
{
	Nf32 l = left_encoder_rev_count * m_driveTrainSpecifications.m_wheelRadius * NF32_2PI;
	Nf32 r = right_encoder_rev_count * m_driveTrainSpecifications.m_wheelRadius * NF32_2PI;

	Nf32 leftmove = l - m_leftDistance;
	Nf32 rightmove = r - m_rightDistance;
	// mise a jour de la position et de l'angle "estimes" du robot.
	Nf32 v = IS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE(m_persistent.m_pTrjPointDsc->m_flags) ? NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(rightmove, leftmove) : NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(leftmove, rightmove);

#ifdef _NEDITOR
	m_leftMove = leftmove;
	m_rightMove = rightmove;
#endif

	// Nf32 w = NLODOMETRY_DRIVETRAIN_W_FROM_WHEELS(left_mov, right_mov, pspecs->m_axleTrack);

	m_leftDistance = l;
	m_rightDistance = r;

	// methode simplifiee pour de petites valeurs de w ou on considere que le deplacement du robot est un petit segment de droite
	// m_angle = _pipi(external_angle); //modulo2PI(m_estimatedAngle);
	// Nf32 deca = IS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE(m_persistent.m_pTrjPointDsc->m_flags) ? NF32_PI : 0.0f;
	m_estimatedPose.m_angle = NModulo2PIf32(gyro_angle /* + deca*/);
	m_estimatedPose.m_position.x += cos(m_estimatedPose.m_angle) * v;
	m_estimatedPose.m_position.y += sin(m_estimatedPose.m_angle) * v;
}

void NLFOLLOWER_TANK::updateTarget(NLTRAJECTORY_PACK *ppack, const Nf32 dt)
{
	NErrorIf(!NIsValidArrayElementPtr(&ppack->m_trajectoryPointDescArray, (NBYTE *)m_persistent.m_pTrjPointDsc), NERROR_SYSTEM_GURU_MEDITATION);

	m_currentTime += dt;
	ppack->getPoint(&m_persistent, NMIN(m_currentTime, ppack->m_dt));

#ifdef _NUT_LOGGING
	if (m_currentTime < 10.0f) // On s'en tient a 10 sec de logs max.
	{
		Nchar buff[128];
		NUT_EnableLoggingChannel(LOGS_CHANNEL_SIMUL, TRUE);
		NUT_Logging(LOGS_CHANNEL_SIMUL, "+ NLFOLLOWER_TANK::estimate(...) currentTime = %.4f\tp.x = %.4f\tp.y = %.4f\t%s +\n", m_currentTime, m_persistent.m_result.m_p.x, m_persistent.m_result.m_p.y, m_persistent.m_result.m_kin.print(buff));
	}
#endif

	// Mise a jour de la liste des posted messages
	while (m_pToMessage->m_timeStamp <= m_currentTime)
	{
		m_pToMessage++;
	}
	NErrorIf(m_pToMessage > (NLTRJ_POSTED_MESSAGE *)NGetLastArrayPtr(&ppack->m_postedMessagesArray), NERROR_SYSTEM_CHECK);
}

NLFOLLOWER_TANK_OUTPUT *NLFOLLOWER_TANK::compute()
{
	// ------------------------------------------------------------------------------------------------------------------------------
	//
	// La matrice du robot:
	// Calculee a partir de la pose estimee du Robot. PAr estimee on veut dire, calculee a partir des differentes mesures a notre disposition,
	// a savoir; Encodeurs et Gyroscope.
	//
	//
	// Pour rappel: ( cf NLDriveTrainSpecs.h )
	//
	//					-----	-----	-----
	//				+---------------------------+
	//				|			 Left			|							  ^
	//				|			  Y				|							  j
	//				|			  ^ 			|							  |
	//		 Back <-|			  +-> X			|-> Front	avec donc,        +-- i ->
	//				|							|
	//				|							|
	//				|			Right			|
	//				+---------------------------+
	//					-----	-----	-----

	Nf32 co, si;
	// Nf32 deca = IS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE(m_persistent.m_pTrjPointDsc->m_flags) ? NF32_PI : 0.0f;

	// NCosSin(m_estimatedPose.m_angle+deca, &co, &si);
	co = cos(m_estimatedPose.m_angle);
	si = sin(m_estimatedPose.m_angle);
	NVEC2f32 i, j;

	if (IS_NLTRJPOINT_DESC_SET_DTMODE_FORWARD(m_persistent.m_pTrjPointDsc->m_flags))
	{
		i.x = co;
		j.x = -si; // k.x = 0
		i.y = si;
		j.y = co; // k.y = 0
		//	i.z = 0			j.z =  0	// k.z = 1
	}
	else
	{
		i.x = -co;
		j.x = si; // k.x = 0
		i.y = -si;
		j.y = -co; // k.y = 0
		//	i.z = 0			j.z =  0	// k.z = 1
	}

	// ------------------------------------------------------------------------------------------------------------------------------
	//
	// Le point a suivre ( target point ) est ici:
	//														m_persistent.m_result
	//

	// ------------------------------------------------------------------------------------------------------------------------------
	//
	// Cordonnees de la position cible dans la 'base du robot'
	// Ces 'coordonnees' representent l'erreur en position du robot, existant entre sa position courante estimee et la position cible.
	//
	NVEC2f32 p, errp;
	p.x = m_persistent.m_result.m_p.x - m_estimatedPose.m_position.x;
	p.y = m_persistent.m_result.m_p.y - m_estimatedPose.m_position.y;

	errp.x = p.x * i.x + p.y * i.y; // = NVec2DotProduct(&p, &i);
	errp.y = p.x * j.x + p.y * j.y; // = NVec2DotProduct(&p, &j);

	// ------------------------------------------------------------------------------------------------------------------------------
	//
	// Recherche de l'erreur angulaire.
	// Part 1/2 : Angle existant entre la direction courante estimee et la tangente a la trajectoire au point considere.
	//
	Nf32 erra, cos_erra;
	Nf32 vref;
	Nf32 wref;

	// if (m_targetPoint.m_k == NF32_MAX)
	switch (GET_NLTRJPOINT_DESC_KTYPE(m_persistent.m_pTrjPointDsc->m_flags))
	{
	case FLAGS_NLTRJPOINT_DESC_KTYPE_SPOT:
	{
		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Recherche de l'erreur angulaire.
		// Part 2/2:
		// La courbure est infinie ( k = Nf32_max ), cela signifie que nous sommes en mode SPIN, en train de tourner autour d'un point fixe.
		// Dans ce cas particulier, les distances parcourues par les roues droite et gauche correspondent aux longueurs d'arc mesurees depuis le debut du SPIN.
		// 1/ premiere approche:
		//	Comme nous savons être en mode SPIN, nus savons que le robot tourne sur lui-même:
		//  on a donc A = Sr / ( e/2 )
		//			  A = 2*Sr/e
		// 2/ deuxieme approche:
		// ... En partant des formules de base d'odometrie on a:
		//			Sr = ( R + e/2 ) * A
		//			Sl = ( R - e/2 ) * A
		//
		//			Avec	R rayon de courbure du 'virage'
		//					e entraxe
		//					A angle parcouru
		//
		//			Sr - Sl = ( R + e/2 ) * A - ( R - e/2 ) * A
		//			Sr - Sl = R*A + e/2 * A - R*A + e/2 * A
		//			Sr - Sl = e/2 * A + e/2 * A
		//			Sr - Sl = e*A
		//
		// Dans notre cas particulier de SPIN Sr = -Sl ( les roues droite et gauche tournent a la même vitesse mais en sens oppose )
		//
		//			on a donc 2*Sr = e*A
		//
		//			A = 2*Sr/e
		//
		Nf32 ang = 2.0f * m_persistent.m_result.m_kin.m_s / m_driveTrainSpecifications.m_axleTrack;
		Nf32 c, s;
		// NCosSin(ang, &c, &s);
		c = cos(ang);
		s = sin(ang);
		NVEC2f32 u;
		u.x = c * m_persistent.m_result.m_u.x - s * m_persistent.m_result.m_u.y;
		u.y = c * m_persistent.m_result.m_u.y + s * m_persistent.m_result.m_u.x;
		// NVec2Normalize(&u);
		cos_erra = NVec2DotProduct(&u, &i);
		cos_erra = NCLAMP(-1.0f, cos_erra, 1.0f);
		erra = acosf(cos_erra);
		Nf32 d2 = NVec2DotProduct(&u, &j);
		if (d2 < 0.0f)
			erra = -erra;

		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Vitesse de Ref. et Vitesse angulaire de ref. ( = vitesses cibles )
		//
		vref = 0.0f;
		wref = 2.0f * m_persistent.m_result.m_kin.m_v / m_driveTrainSpecifications.m_axleTrack;

		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Accelerations ...
		//
		m_output.m_rightAcceleration = m_persistent.m_result.m_kin.m_a;
		m_output.m_leftAcceleration = -m_persistent.m_result.m_kin.m_a;

		printf("erreur angulaire : %.4f		angle : %.4f // Ks : %.4f	// Kv : %.4f	// Ka : %.4f	// Kt : %.4f	// VREF: %.4f	//WREF: %.4f\n", NRADtoDEG(erra), NRADtoDEG(ang), m_persistent.m_result.m_kin.m_s, m_persistent.m_result.m_kin.m_v, m_persistent.m_result.m_kin.m_a, m_persistent.m_result.m_kin.m_t, vref, wref);
	}
	break; // case FLAGS_NLTRJPOINT_DESC_KTYPE_SPOT:

	case FLAGS_NLTRJPOINT_DESC_KTYPE_BACK_AND_FORTH:
	{
	}
	break; // case FLAGS_NLTRJPOINT_DESC_KTYPE_BACK_AND_FORTH:

	case FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING:
	{
		cos_erra = NVec2DotProduct(&m_persistent.m_result.m_u, &i);
		cos_erra = NCLAMP(-1.0f, cos_erra, 1.0f);
		erra = acosf(cos_erra);
		Nf32 d2 = NVec2DotProduct(&m_persistent.m_result.m_u, &j);
		if (d2 < 0.0f)
			erra = -erra;
		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Verification de "l'erreur"( errp.x, errp.y, erra ) par rapport a la "tolerance"( tol.x, tol.y, tola ) fixee.
		//
		// TODO.

		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Vitesse de Ref. et Vitesse angulaire de ref. ( = vitesses cibles )
		//
		vref = m_persistent.m_result.m_kin.m_v;
		wref = m_persistent.m_result.m_kin.m_v * m_persistent.m_result.m_k;

		// ------------------------------------------------------------------------------------------------------------------------------
		//
		// Accelerations ...
		//
		// avec k on a R = 1/k et avec R on a la distribution gauche droite
		if (m_persistent.m_result.m_k == 0.0f)
		{
			m_output.m_leftAcceleration = m_persistent.m_result.m_kin.m_a;
			m_output.m_rightAcceleration = m_persistent.m_result.m_kin.m_a;
		}
		else
		{
			// todo accel ici !
			Nf32 ek_2 = (m_persistent.m_result.m_k * m_driveTrainSpecifications.m_axleTrack) / 2.0f;
			m_output.m_leftAcceleration = m_persistent.m_result.m_kin.m_a * (1.0f - ek_2);
			m_output.m_rightAcceleration = m_persistent.m_result.m_kin.m_a * (1.0f + ek_2);

			/*
			Nf32 ek_2 = m_persistent.m_result.m_k* m_driveTrainSpecifications.m_axleTrack*0.5f;
			if (IS_NLTRJPOINT_DESC_SET_DTMODE_FORWARD(m_persistent.m_pTrjPointDsc->m_flags))
			{
				m_output.m_leftAcceleration  = m_persistent.m_result.m_kin.m_a * (1.0f - ek_2);
				m_output.m_rightAcceleration = m_persistent.m_result.m_kin.m_a * (1.0f + ek_2);
			}
			else
			{
				m_output.m_leftAcceleration		= m_persistent.m_result.m_kin.m_a * (1.0f + ek_2);
				m_output.m_rightAcceleration	= m_persistent.m_result.m_kin.m_a * (1.0f - ek_2);
			}
			*/

			/*
			Nf32 r = 1.0f / m_persistent.m_result.m_k;
			m_output.m_leftAcceleration = (r - m_driveTrainSpecifications.m_axleTrack * 0.5f) * m_persistent.m_result.m_k * m_persistent.m_result.m_kin.m_a;
			m_output.m_rightAcceleration = (r + m_driveTrainSpecifications.m_axleTrack * 0.5f) * m_persistent.m_result.m_k * m_persistent.m_result.m_kin.m_a;
			*/
		}
	}
	break; // case FLAGS_NLTRJPOINT_DESC_KTYPE_TRAVELING:

	default:
		NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
		break;
	} // switch (GET_NLTRJPOINT_DESC_CONFIG(m_persistent.m_pTrjPointDsc->m_flags))

	// ------------------------------------------------------------------------------------------------------------------------------
	//
	// RAMSETE
	// Et c'est pa'ti pou' la fo'mu'e incomp'ehensib' mais que tou' l' monde dit quelle est supey'
	// ... !!! en fêt' a y est je tou' comp'i :) !!!!!!
	//
	Nf32 _k = 2.0f * m_ramsete.m_zeta * sqrt(NPOW2(wref) + m_ramsete.m_b * NPOW2(vref));
	Nf32 _v = vref * cos_erra + _k * (errp.x);
	Nf32 _w = wref + _k * erra + m_ramsete.m_b * vref * NSinc(erra) * (errp.y);

	//_v = vref;
	//_omega = wref;
	// m_output.m_rightAcceleration = 0.0f;
	// m_output.m_leftAcceleration = 0.0f;

	if (IS_NLTRJPOINT_DESC_SET_DTMODE_REVERSE(m_persistent.m_pTrjPointDsc->m_flags))
	{
		m_output.m_rightVelocity = -NLODOMETRY_L_WHEEL_FROM_DRIVETRAIN(_w, _v, m_driveTrainSpecifications.m_axleTrack);
		m_output.m_leftVelocity = -NLODOMETRY_R_WHEEL_FROM_DRIVETRAIN(_w, _v, m_driveTrainSpecifications.m_axleTrack);

		Nf32 a = m_output.m_rightAcceleration;
		m_output.m_rightAcceleration = -m_output.m_leftAcceleration;
		m_output.m_leftAcceleration = -a;
	}
	else
	{
		m_output.m_leftVelocity = NLODOMETRY_L_WHEEL_FROM_DRIVETRAIN(_w, _v, m_driveTrainSpecifications.m_axleTrack);
		m_output.m_rightVelocity = NLODOMETRY_R_WHEEL_FROM_DRIVETRAIN(_w, _v, m_driveTrainSpecifications.m_axleTrack);
	}

#ifdef _NEDITOR
	NLFOLLOWER_TANK_DRAWDATA dd;
	dd.m_v = _v;
	dd.m_a = (m_output.m_rightAcceleration + m_output.m_leftAcceleration) / 2.0f;
	dd.m_w = _w;
	dd.m_alpha = (m_output.m_rightAcceleration - m_output.m_leftAcceleration) / m_driveTrainSpecifications.m_axleTrack;
	dd.m_errorPosition = errp;
	dd.m_errorAngle = erra;
	dd.m_encoderLeft = m_leftDistance;
	dd.m_encoderRight = m_rightDistance;
	dd.m_encoderLeftVariation = m_leftMove;
	dd.m_encoderRightVariation = m_rightMove;
	dd.m_output = m_output;
	dd.m_time = m_currentTime;
	pushError(&dd);
#endif

	return &m_output;
}
Nu32 NLFOLLOWER_TANK::getMessage(NLTRJ_POSTED_MESSAGE *msg)
{
	if (m_pFromMessage < m_pToMessage)
	{
		*msg = *m_pFromMessage;
		m_pFromMessage++;

		return NTRUE;
	}
	else
	{
		msg->null();
		return NFALSE;
	}
}
#ifdef _NEDITOR

void NLFOLLOWER_TANK::drawDashBoard(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack)
{
	if (m_errorBuffer.Size)
	{

		/*0		4 green napier
		1		1 orange
		2	    3 laser lemon
		3		8 blue azure
		4		11 blue violet
		5		14 ivory*/

		Nu8 colredir[] = {
			5, //	m_v
			1, //	m_a
			3, //	m_w
			2, //	m_alpha
			0, //	m_errorPosition.x
			4, //  m_errorPosition.y
			1, //	m_errorAngle
			0, //	m_encoderLeft
			4, //	m_encoderRight
			0, //	m_encoderLeftVariation
			4, //	m_encoderRightVariation
			0, //	m_output.m_leftVelocity
			1, //	m_output.m_leftAcceleration
			4, //	m_output.m_rightVelocity
			2  //	m_output.m_rightAcceleration

		};
		NLFOLLOWER_TANK_DRAWDATA *perr;
		NUTDRAWVERTEX p;
		p.Position_3f.z = 0.0f;

		for (Nu32 prm = 0; prm < 15; prm++) // 15, not 16 because the 16th element ( idx 15 ) is time
		{
			if (ISFLAG_ON(p2docs->m_userFlags, 1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + prm))) // 8 + prm car les BITs 8 a 31 de "p2docs->m_Flags" sont disponibles et que prm E [0,15[
			{
				p2docs->getModulatedColor(&p.Color0_4f, colredir[prm], pickpack);
				perr = (NLFOLLOWER_TANK_DRAWDATA *)NGetRingBufferHeadRelativeIndexPtr(&m_errorBuffer, 0);

				if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
					p.Position_3f.x = p2docs->transformX(perr->m_time);
				else
					p.Position_3f.x = p2docs->transformX((perr->m_encoderLeft + perr->m_encoderRight) / 2.0f);

				p.Position_3f.y = p2docs->transformY(perr->m_params[prm]);
				NUT_DrawPencil_From(&p);

				for (Nu32 i = 1; i < m_errorBuffer.Size; i++)
				{
					perr = (NLFOLLOWER_TANK_DRAWDATA *)NGetRingBufferHeadRelativeIndexPtr(&m_errorBuffer, i);

					if (ISFLAG_ON(p2docs->m_userFlags, FLAG_NL2DOCS_DASHBOARD_MP_FT))
						p.Position_3f.x = p2docs->transformX(perr->m_time);
					else
						p.Position_3f.x = p2docs->transformX((perr->m_encoderLeft + perr->m_encoderRight) / 2.0f);

					p.Position_3f.y = p2docs->transformY(perr->m_params[prm]);
					NUT_DrawPencil_LineTo(&p);
				}
			}
		}
	}
}
#endif
#ifdef _NEDITOR
void NLFOLLOWER_TANK::pushError(const NLFOLLOWER_TANK_DRAWDATA *pdd)
{
	NRingBufferPushBack_OverwriteOldest(&m_errorBuffer, pdd);
}
#endif