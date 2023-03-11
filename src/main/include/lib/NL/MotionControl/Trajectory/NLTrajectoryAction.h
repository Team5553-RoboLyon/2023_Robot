#pragma once
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/NMemory.h"
#include "lib/NL/MotionControl/NLKin.h"
#include "lib/NL/MotionControl/NLKinTweak.h"
#include "lib/NL/MotionControl/NLKinLimits.h"

#include "lib/NL/MotionControl/MotionProfiles/NLSmotionProfile.h"

#include "lib/NL/MotionControl/Path/Geometry/NLPathPoint.h"
#include "NLSovershoot.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPointDesc.h"

// *****************************************************************************************************************************************************
// *
// * Flags de configuration ( Attention ces 4 premiers flags sont communs avec ceux de FLAG_NLTRJPOINT_DESC )
// *
#define FLAG_NLTRJACTION_KTYPE_TRAVELING BIT_0 // == FLAG_NLTRJPOINT_DESC_KTYPE_TRAVELING
#define FLAG_NLTRJACTION_KTYPE_SPOT BIT_1	   // == FLAG_NLTRJPOINT_DESC_KTYPE_SPOT
#define FLAG_NLTRJACTION_SET_DT_MODE BIT_2	   // == FLAG_NLTRJPOINT_DESC_SET_DT_MODE
#define FLAG_NLTRJACTION_DT_MODE_REVERSE BIT_3 // == FLAG_NLTRJPOINT_DESC_DT_MODE_REVERSE

/* ... En cons�quence, utiliser les macros et defines d�crits dans "NLTrajectoryPointDesc.h"
 * Mask de configuration
 * #define MASK_NLTRJACTION_KTYPE		==	MASK_NLTRAJECTORY_POINT_DESC_KTYPE	== (BIT_0|BIT_1)
 * #define MASK_NLTRJACTION_DT_MODE	==	MASK_NLTRAJECTORY_POINT_DESC_DTMODE	== (BIT_2|BIT_3)
 * #define MASK_NLTRJACTION_CONFIG		==	MASK_NLTRAJECTORY_POINT_DESC_CONFIG	== (MASK_NLTRJACTION_KTYPE|MASK_NLTRJACTION_DT_MODE)
 */
// *****************************************************************************************************************************************************

#define FLAG_NLTRJACTION_NEED_KEY_STOP BIT_4		//
#define FLAG_NLTRJACTION_COMMAND BIT_5				// Une Action dont ce flag est ON prend le contr�le de la DT. La dur�e de cette prise de contr�le peut-�tre nulle ( instantan�e )
#define FLAG_NLTRJACTION_COMMAND_LONG_RUNNING BIT_6 // ... ou non-nulle ( le flag FLAG_NLTRJACTION_COMMAND_LONG_RUNNING est alors ON )
													//  ! Deux actions ..._COMMAND ne peuvent se superposer. !

#define FLAG_NLTRJACTION_DISABLE_PRECHECK_INEFFICIENT BIT_7
#define FLAG_NLTRJACTION_DISABLE_PRECHECK_REPEAT BIT_8
#define FLAG_NLTRJACTION_DISABLE_PRECHECK_USELESS BIT_9
#define FLAG_NLTRJACTION_DISABLE_PRECHECK_OVERLAPPING BIT_10
#define FLAG_NLTRJACTION_DISABLE_PRECHECK_OUT_OF_KEY_STOP_TIMESPAN BIT_11
#define FLAG_NLTRJACTION_DISABLE_BY_USER BIT_12

#define FLAG_NLTRJACTION_WAITED_FOR_RESTART BIT_13
#define FLAG_NLTRJACTION_TRIGGER BIT_14

// Le FLAG 15 est specifique par type d'action.
// C'est � dire qu'il peut avoir une signification diff�rente  selon l'ID de l'action.
#define FLAG_NLTRJACTION_ID_ROTATE_CW BIT_15 // Pour une action Rotate, signifie que le sens de rotation sera anti-trigo ( = horaire = Clock Wise )

#define MASK_NLTRJACTION_COMMAND_DATA (BIT_5 | BIT_6)
#define MASK_NLTRJACTION_DISABLE_PRECHECK (BIT_7 | BIT_8 | BIT_9 | BIT_10 | BIT_11)
#define MASK_NLTRJACTION_DISABLE (BIT_7 | BIT_8 | BIT_9 | BIT_10 | BIT_11 | BIT_12)

#define IS_NLTRJACTION_ENABLE(flags) (FLAGS_TEST(flags, MASK_NLTRJACTION_DISABLE, 0))

#define IS_NLTRJACTION_COMMAND(flags) ISFLAG_ON(flags, FLAG_NLTRJACTION_COMMAND)
#define IS_NLTRJACTION_COMMAND_INSTANTANEOUS(flags) (FLAGS_TEST(flags, MASK_NLTRJACTION_COMMAND_DATA, FLAG_NLTRJACTION_COMMAND))
#define IS_NLTRJACTION_COMMAND_LONG_RUNNING(flags) (FLAGS_TEST(flags, MASK_NLTRJACTION_COMMAND_DATA, FLAG_NLTRJACTION_COMMAND | FLAG_NLTRJACTION_COMMAND_LONG_RUNNING))

#define NLTRJACTION_ID_NULL 0 // Cette Action n'en est pas UNE !!!
#define NLTRJACTION_ID_FORWARD_MODE 1
#define NLTRJACTION_ID_REVERSE_MODE 2
#define NLTRJACTION_ID_ROTATE 3
#define NLTRJACTION_ID_BACK_AND_FORTH 4
#define NLTRJACTION_ID_SEND_MESSAGE 5

#define NLTRJACTION_NUMBER_OF_ID 6

#define NLTRJACTION_ID_NULL_STRING "Not an Action"
#define NLTRJACTION_ID_FORWARD_MODE_STRING "Forward"
#define NLTRJACTION_ID_REVERSE_MODE_STRING "Reverse"
#define NLTRJACTION_ID_ROTATE_STRING "Rotate"
#define NLTRJACTION_ID_BACK_AND_FORTH_STRING "Back and Forth"
#define NLTRJACTION_ID_SEND_MESSAGE_STRING "Send Message"

typedef struct NLTRJACTION_CORE NLTRJACTION_CORE;
struct NLTRJACTION_CORE
{
	Nu16 m_id;
	Nu16 m_flags;
	Nf32 m_timeShift; // d�calage temporel de d�clenchement par rapport � l'Action precedente.
	Nf32 m_timeStamp;
	// Nu32	m_xData;			// lien (potentiel) vers des Donn�es externes
};
// Taille : 4 X Nu32 = 16 Bytes = 128 bits
/*
typedef struct NLTRJACTION_FORWARD_MODE NLTRJACTION_FORWARD_MODE;
struct NLTRJACTION_FORWARD_MODE
{
	Nu16	m_id;					// |
	Nu16	m_flags;				// |
	Nf32	m_timeShift;			// |--- Core Part
	Nf32	m_timeStamp;			// |
	Nu32	m_xData;				// |
};
// Taille : 4 X Nu32 = 16 Bytes = 128 bits

typedef struct NLTRJACTION_REVERSE_MODE NLTRJACTION_REVERSE_MODE;
struct NLTRJACTION_REVERSE_MODE
{
	Nu16	m_id;					// |
	Nu16	m_flags;				// |
	Nf32	m_timeShift;			// |--- Core Part
	Nf32	m_timeStamp;			// |
	Nu32	m_xData;				// |
};
// Taille : 4 X Nu32 = 16 Bytes = 128 bits
*/
typedef struct NLTRJACTION_ROTATE NLTRJACTION_ROTATE;
struct NLTRJACTION_ROTATE
{
	Nu16 m_id;		  // |
	Nu16 m_flags;	  // |
	Nf32 m_timeShift; // |--- Core Part
	Nf32 m_timeStamp; // |
					  //	Nu32	m_xData;				// |

	Nf32 m_angle;					   // Angle a parcourir en degr�s
	Nf32 m_speedRatio;				   // vitesse max des moteurs en % de la vitesse max du robot.
	NLSMOTIONPROFILE m_sMotionProfile; // Motion Profile associ� � la rotation sur place
};
// Taille : 5 X Nu32 + 32 x 32 bits = 160 bits + 1024 bits = 1184 bis [ 148 bytes ]

typedef struct NLTRJACTION_BACK_AND_FORTH NLTRJACTION_BACK_AND_FORTH;
struct NLTRJACTION_BACK_AND_FORTH
{
	Nu16 m_id;		  // |
	Nu16 m_flags;	  // |
	Nf32 m_timeShift; // |--- Core Part
	Nf32 m_timeStamp; // |
	// Nu32	m_xData;				// |

	Nf32 m_distance;   // Distance a parcourir en ligne droite depuis la position "Spot"
	Nf32 m_pause;	   // Temps de pause avant de revenir � position "Spot"
	Nf32 m_speedRatio; // vitesse max des moteurs en % de la vitesse max du robot � l'aller et au retour.
};
// Taille : 5 X Nu32 + 32 x 32 bits = 160 bits + 1024 bits = 1184 bis [ 148 bytes ]

typedef struct NLTRJACTION_SEND_MESSAGE NLTRJACTION_SEND_MESSAGE;
struct NLTRJACTION_SEND_MESSAGE
{
	Nu16 m_id;		  // |
	Nu16 m_flags;	  // |
	Nf32 m_timeShift; // |--- Core Part
	Nf32 m_timeStamp; // |
	// Nu32	m_xData;				// |

	Nu32 m_message; // ID du message
};
// Taille : 4 X Nu32 = 128 bits

typedef union NLTRJACTION NLTRJACTION;
union NLTRJACTION
{
	NLTRJACTION_CORE m_core;
	NLTRJACTION_CORE m_forwardMode;			   // NLTRJACTION_ID_FORWARD_MODE
	NLTRJACTION_CORE m_reverseMode;			   // NLTRJACTION_ID_REVERSE_MODE
	NLTRJACTION_ROTATE m_rotate;			   // NLTRJACTION_ID_ROTATE
	NLTRJACTION_BACK_AND_FORTH m_backAndForth; // NLTRJACTION_ID_BACK_AND_FORTH
	NLTRJACTION_SEND_MESSAGE m_sendMessage;	   // NLTRJACTION_ID_SEND_MESSAGE
};
// Taille : 128 bits ( MAX de ,96 et 128 )

Nu32 isActionEfficient(const NLTRJACTION *paction);
Nf32 getActionCommandTime(const NLTRJACTION *paction);
void preComputeAction(NLTRJACTION *paction, const Nf32 t0, const NLDRIVETRAINSPECS *pdtspecs);
NLTRJPOINT_DESC_CFG promoteAction(const NLTRJPOINT_DESC_CFG context_cfg, const NLTRJACTION *paction, const Nf32 t, NARRAY *pkin_out);

NLTRJACTION *setupAction(NLTRJACTION *paction, const Nu16 actionid, const Nbool bfullreset = NTRUE);
NLTRJACTION *createAction(const Nu16 actionid);
void clearAction(NLTRJACTION *paction);
void deleteAction(NLTRJACTION *paction);

#define NLclearTrjActionInArrayCallBack NULL
// inline void NLclearTrjActionInArrayCallBack(void* p){}