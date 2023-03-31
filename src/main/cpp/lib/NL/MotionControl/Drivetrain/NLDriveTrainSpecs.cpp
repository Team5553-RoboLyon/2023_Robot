//#include "lib/N/File/NFile.h"
#include "lib/N/Miscellaneous/NString.h"
#include "lib/N/NErrorHandling.h"
#include "lib/NL/MotionControl/NLPhysics.h"
//#include "../NLPathWorkbench.h"
#include "lib/NL/MotionControl/Drivetrain/NLDriveTrainSpecs.h"

NLDRIVETRAINSPECS::NLDRIVETRAINSPECS(const Nf32 mass, const NVEC3f32 *pcenterofmass, const Nf32 axletrack, const Nf32 wheelrad, const Nf32 staticfriction, const NLKINLIMITS *plimits)
{
	m_mass				= mass;
	m_weight			= mass*NLPHYSICS_GRAVITY;
	
	m_centerOfMass		= *pcenterofmass;
	m_axleTrack			= axletrack;
	m_wheelRadius		= wheelrad;
	m_staticFriction	= staticfriction;

	m_limits			= *plimits;

	updateTurnInertiaCoefs();

#ifdef _NEDITOR
	m_wheelBase			= DEFAULT_NLDRIVETRAINSPECS_WHEELBASE;
	m_wheelWidth		= DEFAULT_NLDRIVETRAINSPECS_WHEEL_WIDTH;
	m_size.x			= DEFAULT_NLDRIVETRAINSPECS_FRAME_LENGTH; 
	m_size.y			= DEFAULT_NLDRIVETRAINSPECS_FRAME_WIDTH;
	m_size.z			= DEFAULT_NLDRIVETRAINSPECS_FRAME_HEIGHT;
#endif		
}						  
			  
NLDRIVETRAINSPECS::NLDRIVETRAINSPECS(const Nf32 mass, const NVEC3f32 *pcenterofmass, const Nf32 axletrack, const Nf32 wheelrad, const Nf32 staticfriction, const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max)
{
	m_mass				= mass;
	m_weight			= mass * NLPHYSICS_GRAVITY;

	m_centerOfMass		= *pcenterofmass;
	m_axleTrack			= axletrack;
	m_wheelRadius		= wheelrad;
	m_staticFriction	= staticfriction;

	m_limits			= NLKINLIMITS(velocity_max,accel_max,jerk_max);

	updateTurnInertiaCoefs();

#ifdef _NEDITOR
	m_wheelBase			= DEFAULT_NLDRIVETRAINSPECS_WHEELBASE;
	m_wheelWidth		= DEFAULT_NLDRIVETRAINSPECS_WHEEL_WIDTH;
	m_size.x			= DEFAULT_NLDRIVETRAINSPECS_FRAME_LENGTH; 
	m_size.y			= DEFAULT_NLDRIVETRAINSPECS_FRAME_WIDTH;
	m_size.z			= DEFAULT_NLDRIVETRAINSPECS_FRAME_HEIGHT;
#endif
}
NLDRIVETRAINSPECS::NLDRIVETRAINSPECS(const Nf32 mass, const Nf32 centerofmass_x, const Nf32 centerofmass_y, const Nf32 centerofmass_z, const Nf32 axletrack, const Nf32 wheelrad, const Nf32 staticfriction, const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max)
{
	m_mass				= mass;
	m_weight			= mass * NLPHYSICS_GRAVITY;

	m_centerOfMass.x	= centerofmass_x;
	m_centerOfMass.y	= centerofmass_y;
	m_centerOfMass.z	= centerofmass_z;
	m_axleTrack			= axletrack;
	m_wheelRadius		= wheelrad;
	m_staticFriction	= staticfriction;

	m_limits			= NLKINLIMITS(velocity_max, accel_max, jerk_max);

	updateTurnInertiaCoefs();

#ifdef _NEDITOR
	m_wheelBase			= DEFAULT_NLDRIVETRAINSPECS_WHEELBASE;
	m_wheelWidth		= DEFAULT_NLDRIVETRAINSPECS_WHEEL_WIDTH;
	m_size.x			= DEFAULT_NLDRIVETRAINSPECS_FRAME_LENGTH; 
	m_size.y			= DEFAULT_NLDRIVETRAINSPECS_FRAME_WIDTH;
	m_size.z			= DEFAULT_NLDRIVETRAINSPECS_FRAME_HEIGHT;
#endif
}
#ifdef _NEDITOR
Nu32 NLDRIVETRAINSPECS::read(NLPATH_WORKBENCH* pwb)
{
	if (pwb)
	{
		NLDRIVETRAINSPECS* psrc = pwb->getDriveTrainSpecifications();
		if (psrc)
		{
			*this = *psrc;
			updateTurnInertiaCoefs();
			return 1;
		}
	}
	return 0;
}
#endif

Nu32 NLDRIVETRAINSPECS::write(FILE* pfile)
{
	// Write Current version Tag 
	Nu32	_u32 = VERSION_NLDTSPECS_HEADER;
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	NLDTSPECS_HEADER header;
	//NStrCopy(header.m_name,"Robot ", CONSTANT_NLDTSPECS_HEADER_NAME_SIZE);
	header.m_limits_v			= m_limits.m_v;
	header.m_limits_a			= m_limits.m_a;
	header.m_limits_j			= m_limits.m_j;
	header.m_staticFriction		= m_staticFriction;
	header.m_mass				= m_mass;
	header.m_weight				= m_weight;
	header.m_centerOfMass		= m_centerOfMass;
	header.m_axleTrack			= m_axleTrack;
	header.m_wheelRadius		= m_wheelRadius;

#ifdef _NEDITOR	
	header.m_editorData			= sizeof(NLDTSPECS_HEADER_NEDITOR);
#endif	
#ifndef _NEDITOR
	header.m_editorData			= 0;
#endif
	if (fwrite(&header, sizeof(NLDTSPECS_HEADER), 1, pfile) != 1)
		return 0;

#ifdef _NEDITOR	
	NLDTSPECS_HEADER_NEDITOR	header_editor;
	header_editor.m_wheelBase	= m_wheelBase;		// Distance entre le point de contact au sol de la roue avant et de la roue arriere sur un m�me c�t�
	header_editor.m_wheelWidth	= m_wheelWidth;		// largeur des roues
	header_editor.m_size		= m_size;			// taille de la base.
	if (fwrite(&header_editor, sizeof(NLDTSPECS_HEADER_NEDITOR), 1, pfile) != 1)
		return 0;
#endif	
	return 1;
}

Nu32 NLDRIVETRAINSPECS::importTxt(const Nchar* ptxtfilename)
{
	NErrorIf(!ptxtfilename, NERROR_NULL_POINTER);
	// Check extension
	if (!NStrCheckEnd(ptxtfilename, EXTENSION_NLDRIVETRAINSPECS_TXT))
		return 0;

	FILE* pfile;
	Nchar								tempstring[1024];
	Nchar								name[32];
	Nchar* pstr;

	pfile = fopen(ptxtfilename, "r");	// ouverture du fichier
	fseek(pfile, 0, SEEK_SET);			// on se place au d�but du fichier

	// recup�rer la siganture du fichier
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_String_AfterLabel(pstr, "signature= ", name);
	if (strcmp(name, SIGNATURE_NLDRIVETRAINSPECS))
	{
		fclose(pfile);
		return 0;
	}
	// KinLimits
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "kinlimits_v= ", &m_limits.m_v);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "kinlimits_a= ", &m_limits.m_a);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "kinlimits_j= ", &m_limits.m_j);
	m_limits.build(FLAG_AUTO);
	
	// mass & weight
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "mass= ", &m_mass);
	m_weight = m_mass * NLPHYSICS_GRAVITY;

	// CenterOfMass
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "centerofmass_x= ", &m_centerOfMass.x);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "centerofmass_y= ", &m_centerOfMass.y);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "centerofmass_z= ", &m_centerOfMass.z);

	//friction
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "staticfriction= ", &m_staticFriction);

	//axle track & wheelradius
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "axletrack= ", &m_axleTrack);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "wheelradius= ", &m_wheelRadius);
	updateTurnInertiaCoefs();

	// Editor Only: ( additionnal wheel infos  + Drive Train size )
#ifdef _NEDITOR
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "wheelbase= ", &m_wheelBase);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "wheelwidth= ", &m_wheelWidth);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "size_x= ", &m_size.x);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "size_y= ", &m_size.y);
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "size_z= ", &m_size.z);
#endif
	fclose(pfile);
	return 1;
}


Nu32 NLDRIVETRAINSPECS::read(FILE* pfile)
{
	Nu32						_u32;
	NLDTSPECS_HEADER			header;
	NLDTSPECS_HEADER_NEDITOR	header_editor;

	if (fread(&_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	switch (NGETVERSION_MAIN(_u32))
	{
	// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLDTSPECS_HEADER):
		if (fread(&header, sizeof(NLDTSPECS_HEADER), 1, pfile) != 1)
			return 0;
		//NStrCopy( ? , header.m_name, CONSTANT_NLDTSPECS_HEADER_NAME_SIZE); // TODO: ajouter un nom aux specs...
		m_limits.m_v			= header.m_limits_v;
		m_limits.m_a			= header.m_limits_a;
		m_limits.m_j			= header.m_limits_j;
		m_limits.build(FLAG_AUTO);
		m_staticFriction		= header.m_staticFriction;
		m_mass					= header.m_mass;
		m_weight				= header.m_weight;
		m_centerOfMass			= header.m_centerOfMass;
		m_axleTrack				= header.m_axleTrack;
		m_wheelRadius			= header.m_wheelRadius;

		// recalculs des coef d'inertie ( non sauvegard�s ! )
		updateTurnInertiaCoefs();

#ifdef _NEDITOR	
		if (header.m_editorData == sizeof(NLDTSPECS_HEADER_NEDITOR))
		{
			if (fread(&header_editor, sizeof(NLDTSPECS_HEADER_NEDITOR), 1, pfile) != 1)
				return 0;
			m_wheelBase			= header_editor.m_wheelBase;
			m_wheelWidth		= header_editor.m_wheelWidth;
			m_size				= header_editor.m_size;
		}
		else if (!header.m_editorData)
		{
			// Pas de valeurs "Neditor"  dans le fichier ?? hors nous sommes en mode edition ;(
			m_wheelBase		= DEFAULT_NLDRIVETRAINSPECS_WHEELBASE;
			m_wheelWidth	= DEFAULT_NLDRIVETRAINSPECS_WHEEL_WIDTH;
			m_size.x		= DEFAULT_NLDRIVETRAINSPECS_FRAME_LENGTH; 
			m_size.y		= DEFAULT_NLDRIVETRAINSPECS_FRAME_WIDTH;
			m_size.z		= DEFAULT_NLDRIVETRAINSPECS_FRAME_HEIGHT;
		}
		#ifdef _DEBUG
		else
		{
			NErrorIf(1, NERROR_FILE_UNEXPECTED_READ_VALUE);
		}
		#endif
#endif	
#ifndef _NEDITOR
		if (header.m_editorData == sizeof(NLDTSPECS_HEADER_NEDITOR))
		{
			// On lit les donn�es ... sans rien en faire ...
			if (fread(&header_editor, sizeof(NLDTSPECS_HEADER_NEDITOR), 1, pfile) != 1)
				return 0;
		}
		else if (!header.m_editorData)
		{
			return 1; // on return de suite, rien de plus � faire ...
		}
		#ifdef _DEBUG
		else
		{
			NErrorIf(1, NERROR_FILE_UNEXPECTED_READ_VALUE);
		}
		#endif
#endif
		return 1;

		// Unknown version
	default:
		NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
		return 0;
	}
	return 1;
}

// +----------------------+
// | Limites de vitesses: |
// +----------------------+
//
// Vitesse max. avant "d�rapage/glissement" [skidding]
// ---------------------------------------------------
// Se d�placer le long d'une trajectoire courbe � une vitesse trop �lev�e peut engendrer un d�rapage.
// Cela se produit quand la force centripete n'est plus suffisante pour "tenir" le v�hicule dans la trajectoire. Le v�hicule, qui "veut" aller simplement tout droit, prend le dessus et d�rape ...
//
// on note 'a'  l'Acceleration Centripete		a  = v�/R 
// on note 'Fc' la Force Centripete				Fc = ma = mv�/R
// on note 'Ff' la Force de Friction Statique	Ff = us*m*g			avec 'us' ratio de friction static, 'm' masse et 'g' gravit� ( = 9.81 )
//
// Tant que la Force de friction statique est sup�rieure � la force centrip�te il n'y a pas de d�rapage, par contre d�s que la Force centrip�te est plus forte: D�rapage
// Donc nous avons d�rapage ssi
//				Fc > Ff
//				m*v�/R > us*m*g
//				v�/R > us*g
//
//				+---------------------------------------+
//				|										|
//				|	v > sqrt( us*g*R) ---> DERAPAGE !	|
//				|										|
//				+---------------------------------------+	
//
//				Donc, si la vitesse de d�placement "v" le long du chemin est sup�rieure � cette limite,  la force statique de friction (stiction) n'est plus suffisante pour s'opposer � la force centripete et c'est le d�rapage.
//
// Vitesse max. avant "basculement" [Tilting]
// ---------------------------------------------------
// La position de son centre de gravit� peut entrainer un v�hicule (robot) � basculer lorsqu'il se d�place le long d'une trajectoire courbe � une vitesse trop �lev�e.
// La force centripete cr�e un moment d'inertie pouvant amener le robot � basculer.
//
// Le robot bascule quand M1 < M2
//				avec,	M1 = m*g*l ( l = distance horizontale du centre de gravit� vers la roue )
//			
//						M2 = Fc*h  ( h = distance verticale du centre de gravit� par rapport au sol )
//
// 
//
//				M1		< M2			
//				m*g*l	< Fc*h 
//				m*g*l	< h*m*V�/R
//				g*l		< h*v�/R
//
//				v�		> (l/h)*g*R
//				
//
//				+---------------------------------------+
//				|										|
//				|	v > sqrt( (l/h)*g*R )---> BASCULE !	|
//				|										|
//				+---------------------------------------+	
//
//				Donc, si la vitesse de d�placement "v" le long du chemin est sup�rieure � cette limite,  le moment cr�� par la force centripete l'emporte et le robot bascule.
//
// Vitesse Maximum avant basculement et  derapage
// ----------------------------------------------
//	On remarque que les deux formules de calcul de limite de vitesse (Bascule & D�rapage) sont de la m�me forme et quasi identiques � un facteur pr�s.
//
//					v > sqrt(   us * g*R )	---> DERAPAGE !
//					v > sqrt( (l/h)* g*R )	---> BASCULE !
//						
//					soit,
//					
//					VLim1 = sqrt(   us * g*R )
//					VLim2 = sqrt( (l/h)* g*R )
//
//	En pratique, nous recherchons la limite la plus basse pour �viter tous "d�sagr�ments" au robot.
//	Plut�t que de comparer les deux limites apr�s les avoir calcul�es, il est pr�f�rable de ne comparer que les deux valeurs "us" et "l/h". On �vite ainsi de calculer inutilement 1 racine carr�e.
//	Ainsi,
//					us  < l/h		( Le coefficient de friction statique est inf�rieur au rapport des coordonn�es du centre de gravit� ). 
//									La limite de vitesse "VLim1" sera la plus basse et si on va trop vite, le robot d�rapera avant de basculer.
//								
//					l/h < us		(  Le rapport des coordonn�es du centre de gravit� est inf�rieur au coefficient de friction statique ). 
//									La limite de vitesse "VLim2" sera la plus basse et si on va trop vite, le robot basculera avant de d�raper.
//
// Vitesse Maximum en courbe ( limitation moteur )
// -----------------------------------------------
// Notre base pilotable est de type TANK. Elle utilise un diff�rentiel de vitesse entre roues droites et roues gauches pour tourner.
// La boite de vitesse des roues de droites et celle des roues de gauches sont identiques. En th�orie donc, la vitesse maximum des roues droites et gauches est identique
//
//						VDMax = VGMax
//
//	... Et la vitesse maximum de la base est donn�e par 
//	
//						VMax = (VDMax + VGMax ) /2
//
// Rappel des formules utilis�es en Odometrie.
//
//				v = (vd + vg)/2				>>> Vitesse de la base � partir des vitesses des roues droites et gauches.
//
//				vg = (R - e/2) * v / R		>>> Vitesse des roues Gauches � partir de la vitesse de la base, de l'entraxe et du rayon de courbure 
//												(R-e/2) correspond au rayon du cercle surlequel se deplace la roue Gauche et (v/R) correspond � la vitesse angulaire W de la base (W est bien s�r �galement la vitesse angulaire donc de la roue gauche)
//
//				vd = (R + e/2) * v / R		>>> Vitesse des roues Droites � partir de la vitesse de la base, de l'entraxe et du rayon de courbure
//												(R+e/2) correspond au rayon du cercle surlequel se deplace la roue Droite et (v/R) correspond � la vitesse angulaire W de la base (W est bien s�r �galement la vitesse angulaire donc de la roue droite)
//
//				! Les deux pr�c�dentes formules sont bas�es sur le sch�ma suivant, avec R positif trac� depuis le centre O situ� � gauche de la roue gauche:
//				! Il conviendra donc d'adapter les signes en cas de virage � Droite ( O � droite de la roue droite )
//																 ___						 ___																																					
//																|	|						|	|																																					
//									x --------------------------|-+-|-----------+-----------|-+ |
//																|___|						|___|																																					
//									O							  g				C			  d		
//									|							  |				|			  |		
//									|							  |				|			  |		
//									 <------------------R---------------------->
//									|							  |				|			  |		
//																   <------------e------------>
//									R: Rayon de courbure
//									e: entraxe
//									O: Centre du cercle de rayon R
//									g: roue Gauche
//									C: milieu du segment [g,d] devrait coincider avec le centre de gravit�
//									d: roue Droite
//
//	A partir des �galit�s pr�c�dentes, on peut �crire:
//
//				vd = (R + e/2) * v / R		---->		v = R * vd / (R + e/2)
//
// Remplacons 'v' par 'R * vd / (R + e/2)' dans 'vg = (R - e/2) * v / R'
//
//											---->
//
//				vg = (R - e/2) * R * vd / (R + e/2) / R
//				vg = (R - e/2) * vd / (R +  e/2)
//
//				vg = vd * (R - e/2)/(R + e/2)
//
//	Remplacons 'vg' par 'vd * (R - e/2)/(R + e/2)' dans 'v = (vd + vg)/2'
//
//											---->
//
//				v = (vd + vd * (R - e / 2) / (R + e / 2)) / 2
//
//				v = vd*( 1 + (R - e/2)/(R + e/2) ) / 2
//
// La vitesse maximum possible dans un virage de rayon R est atteinte quand la roue 'ext�rieure' ( /au virage)  est � sa vitesse maximum, et donc que la roue 'int�rieure' (/au virage) est � une vitesse inf�rieure.
// On peut donc trouver, pour un rayon de courbure donn�,  la vitesse v maximum possible de la base en fonction de la vitesse maximum possible de la roue ext�rieure au virage.
//
//				VMaxenVirage = VDMax*( 1 + (R - e/2)/(R + e/2) ) / 2
//
// VDMax �tant th�oriquement �gal � VGMax on consid�rera que VMaxenVirage est la m�me que le virage tourne � droite ou � gauche.


void NLDRIVETRAINSPECS::updateTurnInertiaCoefs()
{
	// Calcul du "coefficient d'inertie"  utilis� ensuite dans le calcul de la vitesse limite locale de chaque VStage !
	NErrorIf(m_centerOfMass.z < 0.0f, NERROR_INCONSISTENT_VALUES); // Doit �tre positif !

	if (m_centerOfMass.z) // z repr�sente la hauteur !
	{
		// Rappel:	Consid�rons le rep�re XYZ dont l'origine se trouve sur la projection au sol du centre g�om�trique du robot.
		//			(par centre g�om�trique on entend le point situ� au milieu de la 'frame' du robot.)	
		//			par convention, l'axe X de ce rep�re pointe vers l'avant du robot
		//							l'axe Y pointe vers la gauche du robot ( le rep�re est direct )
		//							l'axe Z pointe vers le haut
		//
		// On connait:
		//
		//	La position du centre d'inertie dans ce rep�re (i.e m_centerOfMass):
		//	
		//							m_centerOfMass ( x,y,z )	
		//
		// Quand on tourne � GAUCHE, la roue GAUCHE est � l'int�rieur du virage. 
		// On veut connaitre la position du centre d'inertie par rapport � la roue GAUCHE le long de l'AxeY ( pointant vers la gauche) du rep�re.
		// On connait:
		// La position de la roue gauche dans le long de l'AxeY = m_axleTrack/2		Rappel: l'AxeX pointe vers l'avant du Robot
		//																					l'AxeY pointe vers la roue gauche   !!!
		//
		// on a donc,
		//				Pos. Centre d'inertie par rapport � la roue gauche = m_centerOfMass.y - m_axleTrack/2
		//
		m_leftTurnInertiaCoef = NMIN(m_staticFriction, NABS(m_centerOfMass.y - m_axleTrack * 0.5f) / m_centerOfMass.z) * NLPHYSICS_GRAVITY;

		// Quand on tourne � DROITE, la roue DROITE est � l'int�rieur du virage. 
		// On veut connaitre la position x,y du centre d'inertie par rapport � la roue DROITE.
		// on connait:
		// La position de la roue droite dans le long de l'AxeY = -m_axleTrack/2		Rappel: l'AxeX pointe vers l'avant du Robot
		//																						l'AxeY pointe vers la roue gauche   !!!
		//
		// on a donc,
		//				Pos. Centre d'inertie par rapport � la roue droite	= m_centerOfMass.y - (-m_axleTrack/2)
		//																	= m_centerOfMass.y + m_axleTrack/2	
		m_rightTurnInertiaCoef = NMIN(m_staticFriction, NABS(m_centerOfMass.y + m_axleTrack * 0.5f) / m_centerOfMass.z) * NLPHYSICS_GRAVITY;
	}
	else
	{
		m_leftTurnInertiaCoef	= m_staticFriction * NLPHYSICS_GRAVITY;
		m_rightTurnInertiaCoef	= m_leftTurnInertiaCoef;
	}
}

// ------------------------------------------------------------------------------------------
/**
 *	@brief	Calcule la vitesse maximum possible en un point de courbure k
 *
 *	@param		k est la courbure au point consid�r�
 *	@param		max_cruise_velocity est la vitesse maximum possible
 *
 *	@return		la vitesse en m/s max calcul�e en fonction de la vitesse de croisiere max, de la courbure locale et de l'entraxe.
 */
 // ------------------------------------------------------------------------------------------
 Nf32 NLDRIVETRAINSPECS::getVelocity(const Nf32 k, const Nf32 max_cruise_velocity)const
{
	Nf32 r, sqr,v;
	// k negatif signifie qu'on tourne � droite.
	if (k < 0.0f)
	{
		r = -1.0f / k;
		v = max_cruise_velocity * (1.0f + (r - m_axleTrack*0.5f ) / (r + m_axleTrack * 0.5f)) * 0.5f;
		sqr = sqrt(m_rightTurnInertiaCoef / (-k));

		v = NMIN(v, sqr);
	}
	// k positif signifie qu'on tourne � gauche.
	else if (k > 0.0f)
	{
		r = 1.0f / k;
		v = max_cruise_velocity * (1.0f + (r - m_axleTrack * 0.5f) / (r + m_axleTrack * 0.5f)) * 0.5f;
		sqr = sqrt(m_leftTurnInertiaCoef / k);

		v = NMIN(v, sqr);
	}
	// k null, siginifie qu'on va "tout droit"
	else
	{
		v = max_cruise_velocity;
	}

	return v;
}

 Nu32 NLDRIVETRAINSPECS::compare(const NLDRIVETRAINSPECS* pdts)
 {
		if(m_limits.m_v == pdts->m_limits.m_v)
			return 0;
		if (m_limits.m_a == pdts->m_limits.m_a)
			return 0;
		if (m_limits.m_j == pdts->m_limits.m_j)
			return 0;

		if(m_mass != pdts->m_mass)
			return 0;

		if(m_weight != pdts->m_weight)
			return 0;

		if(m_axleTrack != pdts->m_axleTrack)
			return 0;

		if(m_wheelRadius != pdts->m_wheelRadius)
			return 0;
#ifdef _NEDITOR
		if(m_wheelBase != pdts->m_wheelBase)
			return 0;

		if(m_wheelWidth != pdts->m_wheelWidth)
			return 0;

		if(m_size.x != pdts->m_size.x)
			return 0;
		if (m_size.y != pdts->m_size.y)
			return 0;
		if (m_size.z != pdts->m_size.z)
			return 0;
#endif

		if(m_staticFriction == pdts->m_staticFriction)
			return 0;

		if(m_centerOfMass.x != pdts->m_centerOfMass.x)
			return 0;
		if (m_centerOfMass.y != pdts->m_centerOfMass.y)
			return 0;
		if (m_centerOfMass.z != pdts->m_centerOfMass.z)
			return 0;

		if(m_rightTurnInertiaCoef != pdts->m_rightTurnInertiaCoef)
			return 0;
		if (m_leftTurnInertiaCoef != pdts->m_leftTurnInertiaCoef)
			return 0;

	 return 1;
 }

