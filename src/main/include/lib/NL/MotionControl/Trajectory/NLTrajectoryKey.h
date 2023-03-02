#pragma once
#include "../../../N/NType.h"
#include "../../../N/NFlags.h"
#include "../../../N/NMemory.h"
#include "../../../N/Core/NVersion.h"
#include "../../../N/Maths/NIntervalf32.h"

#ifdef _NEDITOR
#include "../../NLGfx.h"
#endif

/*												_LOCKED		_STOP	_LINKABLE	_DISABLE_BROKEN_LINK	_DISABLE_TOO_CLOSE	_DISABLE_OUTOFPATH
		IS_NLTRJKEY_ENABLE_UNLOCKED(flags)			OFF		  x			x				OFF						OFF					OFF
		IS_NLTRJKEY_ENABLE_LINKED(flags)			x		  x			ON				OFF						OFF					OFF
		IS_NLTRJKEY_ENABLE_STOP(flags)				x		  ON		x				OFF						OFF					OFF
		IS_NLTRJKEY_ENABLE(flags)					x		  x			x				OFF						OFF					OFF

*/
#include "NLTrajectoryAction.h"

#define DEFAULT_NLTRJKEY_ACTIONSARRAY_CAPACITY 3

#define FLAG_NLTRJKEY_LOCKED BIT_0	 // la clef ne peut �tre supprim�e
#define FLAG_NLTRJKEY_STOP BIT_1	 // la clef est un stop
#define FLAG_NLTRJKEY_LINKABLE BIT_2 // la clef peut �tre reli�e � un point de chemin
// #define FLAG_NLTRJKEY_SPIN						BIT_3		// la clef contient au moins une action de rotation sur place
#define FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN BIT_4 // la dur�e

#define FLAG_NLTRJKEY_DISABLE_BROKEN_LINK BIT_13 // la clef est d�sativ�e car le lien avec le point de chemin "m_linkedPointId" n'a pu �tre r�alis�.
#define FLAG_NLTRJKEY_DISABLE_TOO_CLOSE BIT_14	 // la clef est d�sativ�e car trop proche d'une autre
#define FLAG_NLTRJKEY_DISABLE_OUTOFPATH BIT_15	 // la clef est d�sactiv�e car elle a une abscisse hors du chemin ! Ce flag est r��valu� � chaque build de trajectoire.

#define IS_NLTRJKEY_ENABLE_UNLOCKED(flags) (FLAGS_TEST(flags, FLAG_NLTRJKEY_LOCKED | FLAG_NLTRJKEY_DISABLE_BROKEN_LINK | FLAG_NLTRJKEY_DISABLE_TOO_CLOSE | FLAG_NLTRJKEY_DISABLE_OUTOFPATH, 0))

#define IS_NLTRJKEY_ENABLE_LINKED(flags) (FLAGS_TEST(flags, FLAG_NLTRJKEY_LINKABLE | FLAG_NLTRJKEY_DISABLE_BROKEN_LINK | FLAG_NLTRJKEY_DISABLE_TOO_CLOSE | FLAG_NLTRJKEY_DISABLE_OUTOFPATH, FLAG_NLTRJKEY_LINKABLE))
#define IS_NLTRJKEY_ENABLE_STOP(flags) (FLAGS_TEST(flags, FLAG_NLTRJKEY_STOP | FLAG_NLTRJKEY_DISABLE_BROKEN_LINK | FLAG_NLTRJKEY_DISABLE_TOO_CLOSE | FLAG_NLTRJKEY_DISABLE_OUTOFPATH, FLAG_NLTRJKEY_STOP))
#define IS_NLTRJKEY_ENABLE(flags) (FLAGS_TEST(flags, FLAG_NLTRJKEY_DISABLE_BROKEN_LINK | FLAG_NLTRJKEY_DISABLE_TOO_CLOSE | FLAG_NLTRJKEY_DISABLE_OUTOFPATH, 0))

#define IS_NLTRJKEY_UNLOCKED(flags) (ISFLAG_OFF(flags, FLAG_NLTRJKEY_LOCKED))

#define IS_NLTRJKEY_AUTODURATION(flags) (ISFLAG_ON(flags, FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN))

// #define IS_NLTRJKEY_SPIN(flags)				(ISFLAG_ON(flags,FLAG_NLTRJKEY_SPIN))

// #define IS_NLTRJKEY_HAS_MULTIPLE_ACTIONS(flags)		(ISFLAG_ON(flags,FLAG_NLTRJKEY_MULTIPLE_ACTIONS))

typedef struct NLTRJKEY NLTRJKEY;
struct NLTRJKEY
{
	NLTRJKEY *setup();
	NLTRJKEY *setup(const Nf32 s, const Nu32 flags = FLAG_NLTRJKEY_LOCKED);
	void clear();
	void setS(const Nf32 s) { m_s = s; };

	Nu32 write(FILE *pfile);
	Nu32 read(FILE *pfile);
	NLTRJACTION *insertAction(const NLTRJACTION *paction = NULL);

	Nu32 moveActionDown(Nu32 actionidx);
	Nu32 moveActionUp(Nu32 actionidx);

	NLTRJACTION *updatetActionTimeShift(NLTRJACTION *paction, const Nf32 timeshift);
	Nu32 updatetActionTimeShift(const Nu32 actionidx, const Nf32 timeshift);

	void eraseAction(const Nu32 action_index) { NEraseArrayElement(&m_actionsArray, action_index, NULL); };
	void eraseAllActions() { NEraseArray(&m_actionsArray, NULL); };

	//	void				SetLifeSpan(const Nf32 lifespan);
	void setDuration(const Nbool bauto = NTRUE, const Nf32 duration = 0.0f);
	Nf32 getDuration() { return (m_timeStamp.end - m_timeStamp.start); };

	// Nu32				getInitialPromoteMainParams() { return 0; };
	Nu32 preCompute(const Nf32 t, const NLDRIVETRAINSPECS *pdtspecs);

	// void					promote(NARRAY* pkin_out, const NARRAY* psmp_array,const NLDRIVETRAINSPECS *pdtspecs);
	// NLPROMOTEACTION_CFG	promote(NARRAY* pkin_out, const NLPROMOTEACTION_CFG promote_data, const NARRAY* psmp_array);
	Nu32 oldpromote(NARRAY *pkin_out, Nu32 *pmainparam, const NARRAY *psmp_array, const Nu32 firstactionid, const NLDRIVETRAINSPECS *pdtspecs);

	Nu32 SpotRequirement();

#ifdef _NEDITOR
	void draw(NL2DOCS *p2docs, const NCOLORPICKPACK pickpack);
	void draw(const NLPATH *ppath, const NLGFX *pgfx, const Nu32 index = NVOID);
#endif

	Nf32 m_s;
	Nu16 m_flags;
	Nu16 m_linkedPointId;
	NINTERVALf32 m_timeStamp;
	//	Nf32				m_stopTimeSpan;
	NARRAY m_actionsArray;
};

// Clear call back pour NArray
inline void NLclearTrjKeyInArrayCallBack(void *p)
{
	NLTRJKEY *pk = (NLTRJKEY *)p;
#ifdef _DEBUG
	FLAG_OFF(pk->m_flags, FLAG_NLTRJKEY_LOCKED); // pour passer outre le crash provoqu� en d�bug ( voir code du destructeur )
#endif
	pk->clear();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLTRAJECTORYKEY_HEADER NMAKEVERSION(0, 0, 0) // >>> 2022/04/04
typedef struct
{
	Nf32 m_s;
	Nu16 m_flags;
	Nu16 m_linkedPointId;
	NINTERVALf32 m_timeStamp;
	//	Nf32			m_lifeSpan;

	NARRAYBOUNDS m_actionsArrayBounds;
} NLTRAJECTORYKEY_HEADER;