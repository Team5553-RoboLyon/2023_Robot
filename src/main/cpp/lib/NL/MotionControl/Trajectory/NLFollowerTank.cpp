#include "../../../N/NString.h"
#include "../../../N/Utilities/NUT_Logging.h"

#include "../DriveTrain/NLOdometry.h"
#include "../DriveTrain/Characterization/NLCharacterizationTable.h"
#include "../../NLLogsChannels.h"
#include "NLFollowerTank.h"

Nu32 NLFOLLOWER_TANK::read(NLPATH_WORKBENCH* pwb)
{
	if (!m_driveTrainSpecifications.read(pwb))
		return 0;
	return m_ramsete.read(pwb);
}

Nu32 NLFOLLOWER_TANK::read(FILE* pfile)
{

	// 1) lecture Version
	Nu32	version_u32;
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

Nu32 NLFOLLOWER_TANK::write(FILE* pfile)
{
	// 1) écriture Version
	Nu32	version_u32 = VERSION_NLFOLLOWER_TANK_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) écriture Header
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

Nu32 NLFOLLOWER_TANK::save(const Nchar* pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(pfilename, EXTENSION_NLFOLLOWER_TANK_BIN))
		return 0;

	// 0) Ouverture du fichier en ecriture
	FILE* pfile = fopen(pfilename, "wb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Ecriture Signature
	Nu32	_u32 = SIGNATURE_NLFOLLOWER_TANK_BIN;
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

Nu32 NLFOLLOWER_TANK::load(const Nchar* pfilename)
{
	/* -----------------------------------------------------------------------------------------------------------------
	 *
	 *  Check extension
	 *
	 */
	if (!NStrCheckEnd(pfilename, EXTENSION_NLFOLLOWER_TANK_BIN))
		return 0;

	// 0) Ouverture du fichier en lecture
	FILE* pfile = fopen(pfilename, "rb");
	NErrorIf(!pfile, NERROR_FILE_OPENING_ERROR);
	if (!pfile)
		return 0;

	// 1) Lecture Signature
	Nu32	_u32;
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

Nu32 NLFOLLOWER_TANK::importTxt(const Nchar* ptxtfilename)
{

	NErrorIf(!ptxtfilename, NERROR_NULL_POINTER);
	/* -----------------------------------------------------------------------------------------------------------------
	*
	*  Check extension
	*
	*/
	if (!NStrCheckEnd(ptxtfilename, ".txt"))
		return 0;

	FILE	*pfile;
	Nchar	tempstring[1024];
	Nchar	name[32];
	Nchar	*pstr;

	pfile = fopen(ptxtfilename, "r");	// ouverture du fichier
	fseek(pfile, 0, SEEK_SET);			// on se place au début du fichier

	// recupérer la siganture du fichier
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


void NLFOLLOWER_TANK::initialize(const NLTRAJECTORY_PACK* ppack)
{
	m_currentTime	= 0.0f;
	m_leftDistance	= 0.0f;
	m_rightDistance = 0.0f;
	m_estimatedPose.reset();
	m_persistent.initialize(ppack);
}

void NLFOLLOWER_TANK::estimate(const Nf32 left_encoder_rev_count, const Nf32 right_encoder_rev_count, const Nf32 gyro_angle)
{
	Nf32 l			= left_encoder_rev_count	* m_driveTrainSpecifications.m_wheelRadius * NF32_2PI;
	Nf32 r			= right_encoder_rev_count	* m_driveTrainSpecifications.m_wheelRadius * NF32_2PI;
	Nf32 left_mov	= l - m_leftDistance;
	Nf32 right_mov	= r - m_rightDistance;
	
	m_leftDistance	= l;
	m_rightDistance	= r;

	// mise à jour de la position et de l'angle "estimés" du robot.
	Nf32 v = ISFLAG_ON(m_flags, FLAG_REVERSE_MODE) ? NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(-left_mov, -right_mov) : NLODOMETRY_DRIVETRAIN_V_FROM_WHEELS(left_mov, right_mov);
	//Nf32 w = NLODOMETRY_DRIVETRAIN_W_FROM_WHEELS(left_mov, right_mov, pspecs->m_axleTrack);

	// méthode simplifiée pour de petites valeurs de w où on considère que le déplacement du robot est un petit segment de droite
	// m_angle = _pipi(external_angle); //modulo2PI(m_estimatedAngle);
	
	m_estimatedPose.m_angle			 = NModulo2PIf32(gyro_angle);
	m_estimatedPose.m_position.x	+= cos(m_estimatedPose.m_angle) * v;
	m_estimatedPose.m_position.y	+= sin(m_estimatedPose.m_angle) * v;
}

void NLFOLLOWER_TANK::updateTarget(NLTRAJECTORY_PACK *ppack, const Nf32 dt)
{
	NErrorIf(!NIsValidArrayElementPtr(&ppack->m_trajectoryPointDescArray, (NBYTE*)m_persistent.m_pTrjPointDsc), NERROR_SYSTEM_GURU_MEDITATION);

	m_currentTime += dt;
	ppack->getPoint( &m_persistent, NMIN(m_currentTime, ppack->m_dt) );
	m_targetPoint = m_persistent.m_result;


#ifdef _NUT_LOGGING
	if (m_currentTime < 10.0f) // On s'en tient à 10 sec de logs max.
	{
		Nchar buff[128];
		NUT_EnableLoggingChannel(LOGS_CHANNEL_SIMUL, TRUE);
		NUT_Logging(LOGS_CHANNEL_SIMUL, "+ NLFOLLOWER_TANK::estimate(...) currentTime = %.4f\tp.x = %.4f\tp.y = %.4f\t%s +\n", m_currentTime, m_persistent.m_result.m_p.x, m_persistent.m_result.m_p.y, m_persistent.m_result.m_kin.print(buff));
	}
#endif

}

const NLFOLLOWER_TANK_OUTPUT* NLFOLLOWER_TANK::compute()
{
	// ------------------------------------------------------------------------------------------------------------------------------
	// 
	// La matrice du robot:
	//
	Nf32		co, si;
	NCosSin(m_estimatedPose.m_angle, &co, &si);
	NVEC2f32 i, j;

	i.x = co;		j.x = -si;	// k.x = 0
	i.y = si;		j.y = co;	// k.y = 0
//	i.z = 0			j.z =  0	// k.z = 1


	// ------------------------------------------------------------------------------------------------------------------------------
	// 
	// Cordonnées de la position cible dans la 'base du robot'
	// Ces 'coordonnées' représentent l'erreur en position du robot, existant entre sa position courante estimée et la position cible.
	// 
	NVEC2f32	p, errp;
	p.x = m_targetPoint.m_p.x - m_estimatedPose.m_position.x;
	p.y = m_targetPoint.m_p.y - m_estimatedPose.m_position.y;

	errp.x = p.x * i.x + p.y * i.y; // = NVec2DotProduct(&p, &i);
	errp.y = p.x * j.x + p.y * j.y; // = NVec2DotProduct(&p, &j);

	// ------------------------------------------------------------------------------------------------------------------------------
	// 
	// Recherche de l'erreur angulaire. 
	// Part 1/2 : Angle existant entre la direction courante estimée et la tangente à la trajectoire au point considéré.
	//
	Nf32 erra, cos_erra;
	Nf32 vref;
	Nf32 wref;

	//if (m_targetPoint.m_k == NF32_MAX)
	if(ISFLAG_ON(m_persistent.m_pTrjPointDsc->m_flags, FLAG_NLTRJPOINT_DESC_KTYPE_SPOT))
	{
		// ------------------------------------------------------------------------------------------------------------------------------
		// 
		// Recherche de l'erreur angulaire. 
		// Part 2/2:
		// La courbure est infinie ( k = Nf32_max ), cela signifie que nous sommes en mode SPIN, en train de tourner autour d'un point fixe.
		// Dans ce cas particulier, les distances parcourues par les roues droite et gauche correspondent aux longueurs d'arc mesurées depuis le début du SPIN.
		// 1/ première approche:
		//	Comme nous savons être en mode SPIN, nus savons que le robot tourne sur lui-même:
		//  on a donc A = Sr / ( e/2 ) 
		//			  A = 2*Sr/e
		// 2/ deuxième approche:
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
		// Dans notre cas particulier de SPIN Sr = -Sl ( les roues droite et gauche tournent à la même vitesse mais en sens opposé )
		//			
		//			on a donc 2*Sr = e*A
		//			
		//			A = 2*Sr/e
		//
		Nf32 ang = 2.0f * m_targetPoint.m_kin.m_s / m_driveTrainSpecifications.m_axleTrack;
		Nf32 c, s;
		//NCosSin(ang, &c, &s);
		c = cos(ang);
		s = sin(ang);
		NVEC2f32 u;
		u.x = c * m_targetPoint.m_u.x - s * m_targetPoint.m_u.y;
		u.y = c * m_targetPoint.m_u.y + s * m_targetPoint.m_u.x;
		//NVec2Normalize(&u);
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
		wref = 2.0f * m_targetPoint.m_kin.m_v / m_driveTrainSpecifications.m_axleTrack;

		// ------------------------------------------------------------------------------------------------------------------------------
		// 
		// Accelerations ...
		// 
		m_output.m_rightAcceleration	=  m_targetPoint.m_kin.m_a;
		m_output.m_leftAcceleration		= -m_targetPoint.m_kin.m_a;

		printf("erreur angulaire : %.4f		angle : %.4f // Ks : %.4f	// Kv : %.4f	// Ka : %.4f	// Kt : %.4f	// VREF: %.4f	//WREF: %.4f\n", NRADtoDEG(erra), NRADtoDEG(ang), m_targetPoint.m_kin.m_s, m_targetPoint.m_kin.m_v, m_targetPoint.m_kin.m_a, m_targetPoint.m_kin.m_t, vref, wref);

	}
	else // ptargetstate->m_k != NF32_MAX
	{
		cos_erra = NVec2DotProduct(&m_targetPoint.m_u, &i);
		cos_erra = NCLAMP(-1.0f, cos_erra, 1.0f);
		erra = acosf(cos_erra);
		Nf32 d2 = NVec2DotProduct(&m_targetPoint.m_u, &j);
		if (d2 < 0.0f)
			erra = -erra;
		// ------------------------------------------------------------------------------------------------------------------------------
		// 
		// Vérification de "l'erreur"( errp.x, errp.y, erra ) par rapport à la "tolérance"( tol.x, tol.y, tola ) fixée.
		//
		// TODO.

		// ------------------------------------------------------------------------------------------------------------------------------
		// 
		// Vitesse de Ref. et Vitesse angulaire de ref. ( = vitesses cibles )
		//
		vref = m_targetPoint.m_kin.m_v;
		wref = m_targetPoint.m_kin.m_v * m_targetPoint.m_k;

		// ------------------------------------------------------------------------------------------------------------------------------
		// 
		// Accelerations ...
		// 
		// avec k on a R = 1/k et avec R on a la distribution gauche droite
		if (m_targetPoint.m_k == 0.0f)
		{
			m_output.m_leftAcceleration		= m_targetPoint.m_kin.m_a;
			m_output.m_rightAcceleration	= m_targetPoint.m_kin.m_a;
		}
		else
		{
			Nf32 r = 1.0f / m_targetPoint.m_k;
			m_output.m_leftAcceleration = (r  - m_driveTrainSpecifications.m_axleTrack * 0.5f) * m_targetPoint.m_k * m_targetPoint.m_kin.m_a;
			m_output.m_rightAcceleration = (r + m_driveTrainSpecifications.m_axleTrack * 0.5f) * m_targetPoint.m_k * m_targetPoint.m_kin.m_a;
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------
	// 
	// RAMSETE
	// Et c'est pa'ti pou' la fo'mu'e incomp'éhensib' mais que tou' l' monde dit quelle est supey'
	// ... !!! en fêt' a y est jé tou' comp'i :) !!!!!!
	//
	Nf32 _k = 2.0f * m_ramsete.m_zeta * sqrt(NPOW2(wref) + m_ramsete.m_b * NPOW2(vref));
	Nf32 _v = vref * cos_erra + _k * (errp.x);
	Nf32 _omega = wref + _k * erra + m_ramsete.m_b * vref * NSinc(erra) * (errp.y);

	if (ISFLAG_ON(m_flags, FLAG_REVERSE_MODE))
	{
		m_output.m_leftVelocity		= -NLODOMETRY_L_WHEEL_FROM_DRIVETRAIN(_omega, _v, m_driveTrainSpecifications.m_axleTrack);
		m_output.m_rightVelocity	= -NLODOMETRY_R_WHEEL_FROM_DRIVETRAIN(_omega, _v, m_driveTrainSpecifications.m_axleTrack);
		m_output.m_leftAcceleration = -m_output.m_leftAcceleration;
		m_output.m_rightAcceleration= -m_output.m_rightAcceleration;
	}
	else
	{
		m_output.m_leftVelocity		= NLODOMETRY_L_WHEEL_FROM_DRIVETRAIN(_omega, _v, m_driveTrainSpecifications.m_axleTrack);
		m_output.m_rightVelocity	= NLODOMETRY_R_WHEEL_FROM_DRIVETRAIN(_omega, _v, m_driveTrainSpecifications.m_axleTrack);
	}

	return &m_output;
}
