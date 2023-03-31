#pragma once
#include "../../../N/NType.h"
#include "../NLRobotPose.h"
#include "../Trajectory/NLTrajectoryPoint.h"


#define EXTENSION_NLRAMSETE_TXT		".txt"
#define SIGNATURE_NLRAMSETE		"ramsete"

typedef struct NLRAMSETEOUTPUT	NLRAMSETEOUTPUT;
struct NLRAMSETEOUTPUT
{
	NLRAMSETEOUTPUT() : m_velocity(0.0f), m_angularVelocity(0.0f) {}
	void null() { m_velocity = 0.0f; m_angularVelocity = 0.0f; }
	// computed result values
	Nf32	m_velocity;
	Nf32	m_angularVelocity;
};


#define NLRAMSETE_Compute_k(zeta,b,vref,wref)						( 2.0f * (zeta) * sqrt(NPOW2(wref) + (b) * NPOW2(vref)))
#define NLRAMSETE_Compute_velocity(k,coserra,errx,vref)				( (vref) * (coserra) + (k) * (errx))
#define NLRAMSETE_Compute_angularVelocity(k,b,erra,erry,vref,wref)	( (wref) + (k) * (erra) + (b) * (vref) * NSinc(erra) * (erry))

// A propos de m_b et m_zeta:
//
// Controls Engineering in the FIRST Robotics Competition 
//	 Graduate - level control theory for high schoolers
//					Tyler Veness
// page 90:
//		b and zeta are tuning parameters where b > 0 and zeta element of (0, 1).Larger values of b make convergence
//		more aggressive(like a proportional term), and larger values of zeta provide more damping "
//
#define	DEFAULT_NLRAMSETEPARAMS_B		2.0f
#define DEFAULT_NLRAMSETEPARAMS_ZETA	0.7f


#ifdef _NEDITOR
// forward declaration of NLPathWorkbench ( NLRAMSETE is also included into NLPathWorkbench )
class NLPATH_WORKBENCH;
#endif

class NLRAMSETE
{
public:
	// Membres
	enum PARAM_ID
	{
		_B = 0,		///< index of \e m_b member in "m_param[]" NLRAMESETEPARAMS static table
		_ZETA = 1			///< index of \e m_zeta member in "m_param[]" NLRAMESETEPARAMS static table
	};

	NLRAMSETE() :	m_b(DEFAULT_NLRAMSETEPARAMS_B), m_zeta(DEFAULT_NLRAMSETEPARAMS_ZETA){}
	NLRAMSETE(const Nf32 b, const Nf32 zeta) :	m_b(b), m_zeta(zeta){}
	NLRAMSETEOUTPUT* compute(NLRAMSETEOUTPUT* pout, const Nf32 vref, const Nf32 wref, const Nf32 erra, const Nf32 cos_erra, const Nf32 errx, const Nf32 erry);
#ifdef _NEDITOR
	Nu32	read(NLPATH_WORKBENCH* pwb); // fonction read "sp�ciale" qui copie les donn�es du path workbench pass� en param�tre plutot
#endif									 // ... que de les lire dans un fichier. Cela permet au code du simulateur de rester tr�s proche du code "r�el" du robot	
										 // ... tout en permettant de recup�rer directement les donn�es du path workbench. 	
	Nu32	read(FILE* pfile);
	Nu32	write(FILE* pfile);

	Nu32 	importTxt(const Nchar* ptxtfilename);
	union
	{
		struct
		{
			Nf32	m_b;
			Nf32	m_zeta;
		};
		
		Nf32 m_param[2];	 ///< an another way to access Ramsete Params values with \b NLRAMSETEPARAM_ID @see NLRAMSETEPARAM_ID			
	};
};
