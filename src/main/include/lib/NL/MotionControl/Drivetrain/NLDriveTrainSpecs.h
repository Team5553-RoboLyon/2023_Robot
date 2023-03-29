#pragma once

#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Core/NVersion.h"

//#include "../../../N/File/NFile.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/NL/MotionControl/NLKinLimits.h"

#define EXTENSION_NLDRIVETRAINSPECS_TXT		".txt"
#define SIGNATURE_NLDRIVETRAINSPECS			"drivetrainspecs"
//					-----	-----	-----					+
//				+---------------------------+				|			+
//				|			 Left			|				|			|
//				|			  Y				|				|			|
//				|			  ^ 			|				|			|
//		 Back <-|			  +-> X			|-> Front	AxleTrack	Frame Width
//				|							|				|			|
//				|							|				|			|
//				|			Right			|				|			|
//				+---------------------------+				|			+
//					-----	-----	-----					+
	//				  +<--wheelbase-->+	
//				+<-------FrameLength------->+		
//													
#ifdef _NEDITOR
#define DEFAULT_NLDRIVETRAINSPECS_WHEELBASE					0.609f		// Distance entre le centre de la roue avant et le centre de la roue arri�re
#define DEFAULT_NLDRIVETRAINSPECS_WHEEL_WIDTH				0.0127f		// Largeur d'une roue [=0.5 inch]

#define DEFAULT_NLDRIVETRAINSPECS_FRAME_LENGTH				0.840f		// Longueur de la base	[ from Back to Front ] = size.x
#define DEFAULT_NLDRIVETRAINSPECS_FRAME_WIDTH				0.560f		// Largeur de la base	[ from Left to Right ] = size.y
#define DEFAULT_NLDRIVETRAINSPECS_FRAME_HEIGHT				0.70f		// Hauteur ( du robot ) [ from Bottom to Top ] = size.z
#endif

#ifdef _NEDITOR
// forward declaration of NLPathWorkbench ( to include it into SPACK, which is also included into NLPathWorkbench )
class NLPATH_WORKBENCH;
#endif
class NLDRIVETRAINSPECS
{
	friend class NLTRAJECTORY_PACK;
public:
	NLDRIVETRAINSPECS() { Nmem0(this, NLDRIVETRAINSPECS); };
	NLDRIVETRAINSPECS(const Nf32 mass, const NVEC3f32 *pcenterofmass,const Nf32 axletrack,const Nf32 wheelrad, const Nf32 staticfriction, const NLKINLIMITS *plimits);
	NLDRIVETRAINSPECS(const Nf32 mass, const NVEC3f32 *pcenterofmass, const Nf32 axletrack, const Nf32 wheelrad, const Nf32 staticfriction, const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max);
	NLDRIVETRAINSPECS(const Nf32 mass, const Nf32 centerofmass_x, const Nf32 centerofmass_y, const Nf32 centerofmass_z, const Nf32 axletrack, const Nf32 wheelrad, const Nf32 staticfriction, const Nf32 velocity_max, const Nf32 accel_max, const Nf32 jerk_max);

#ifdef _NEDITOR
	Nu32	read(NLPATH_WORKBENCH* pwb); // fonction read "sp�ciale" qui copie les donn�es du path workbench pass� en param�tre plutot
#endif									 // ... que de les lire dans un fichier. Cela permet au code du simulateur de rester tr�s proche du code "r�el" du robot	
										 // ... tout en permettant de recup�rer directement les donn�es du path workbench. 	
	Nu32	read(FILE* pfile);
	Nu32	write(FILE* pfile);
	Nu32	importTxt(const Nchar* ptxtfilename);

	//~NLDRIVETRAINSPECS();
	void	setStaticFriction(const Nf32 static_friction) { m_staticFriction = static_friction; updateTurnInertiaCoefs();};
	void	setCenterOfMass(const NVEC3f32* pcm) { if (pcm) { m_centerOfMass = *pcm; } else { NVec3Null(&m_centerOfMass); }updateTurnInertiaCoefs();};
	void	setCenterOfMass(const Nf32 x, const Nf32 y, const Nf32 z) { m_centerOfMass.x = x;  m_centerOfMass.x = y; m_centerOfMass.x = z; updateTurnInertiaCoefs();};
	void	setCenterOfMassX(const Nf32 x) { m_centerOfMass.x = x;updateTurnInertiaCoefs(); };
	void	setCenterOfMassY(const Nf32 y) { m_centerOfMass.y = y; updateTurnInertiaCoefs(); };
	void	setCenterOfMassZ(const Nf32 z) { m_centerOfMass.z = z; updateTurnInertiaCoefs(); };

	Nf32		getStaticFriction() { return m_staticFriction; };
	NVEC3f32*	getCenterOfMass(NVEC3f32* pcm) { *pcm = m_centerOfMass; return pcm; };

	Nf32		getVelocity(const Nf32 k, const Nf32 max_cruise_velocity)const;
	Nu32		compare(const NLDRIVETRAINSPECS* pdts);

	NLKINLIMITS		m_limits;			// Limites Cin�matiques de la base roulante.
	
	Nf32			m_mass;				// Masse du robot ( en kg)
	Nf32			m_weight;			// poids du Robot ( en Newton).

	Nf32			m_axleTrack;		// distance (en m�tres) entre le point de contact au sol de la roue centrale gauche et le point de contact au sol de la roue centrale droite.

	Nf32			m_wheelRadius;		// rayon des roues (en M�tres). On assume que toutes les roues du robot on un rayon identique.
#ifdef _NEDITOR
	Nf32			m_wheelBase;		// Distance entre le point de contact au sol de la roue avant et de la roue arriere sur un m�me c�t�
	Nf32			m_wheelWidth;		// largeur des roues
	NVEC3f32		m_size;				// Taille de la base.
										// !!! WARNING !!!
										//		m_size.x repr�sente la longueur du Robot [dist.entre l'arri�re et l'avant]
										//		m_size.y repr�sente la largeur du Robot  [dist.entre la gauche et � droite]
										//		m_size.z repr�sente la hauteur du Robot  [dist.entre le sol et le point le plus haut du robot] 
										//		... L'axe z du repere pointe vers le haut 
										//		... ( Z = 0 signifie au niveau du sol ! Z > 0 repr�sente la hauteur au dessus du sol)
#endif

private:
	void	updateTurnInertiaCoefs();
	Nf32			m_staticFriction;	// coefficient de friction Static des "tractions wheels" sur le sol.
	NVEC3f32		m_centerOfMass;		// position du CG (en m�tres) dans un repere direct dont l'origine se trouve au milieu du segment [axleTrack].
										// !!! WARNING !!!
										//		L'axe x du repere pointe vers l'avant du robot
										//		L'axe y du repere pointe vers la roue gauche
										//		L'axe z du repere pointe vers le haut ( Z = 0 signifie au niveau du sol ! Z > 0 repr�sente la hauteur au dessus du sol)

	Nf32			m_rightTurnInertiaCoef;
	Nf32			m_leftTurnInertiaCoef;
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
// File Extensions
#define FILE_EXTENSION_NLDTSPECS				".rbs"							// RoBot Specifications
// File Signatures
#define FILE_SIGNATURE_NLDTSPECS				NMAKESIGNATURE('R','B','L','S')	// RoBo'Lyon Specifications

#define VERSION_NLDTSPECS_HEADER				NMAKEVERSION(0,0,0)				// >>> 2021/04/03
#define CONSTANT_NLDTSPECS_HEADER_NAME_SIZE		32
typedef struct
{
	Nchar			m_name[CONSTANT_NLDTSPECS_HEADER_NAME_SIZE];			// Nom du robot sur 32 char.max ( 31 + null )

	// NKINLIMITS:
	Nf32			m_limits_v;
	Nf32			m_limits_a;
	Nf32			m_limits_j;

	Nf32			m_staticFriction;	
	Nf32			m_mass;				
	Nf32			m_weight;			
	NVEC3f32		m_centerOfMass;		
										
										
										

	Nf32			m_axleTrack;		
	Nf32			m_wheelRadius;		

	Nu32			m_editorData;		
}NLDTSPECS_HEADER;

typedef struct
{
	Nf32			m_wheelBase;		
	Nf32			m_wheelWidth;		
	NVEC3f32		m_size;				
}NLDTSPECS_HEADER_NEDITOR;

