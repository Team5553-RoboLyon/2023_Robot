#include "lib/N/NErrorHandling.h"
#include "lib/N/NMath.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/Draw/NUT_DrawPencil.h"

#include "lib/NL/MotionControl/Trajectory/NLVstage.h"


void NLVSTAGE::set(const Nf32 s0, const Nf32 s1, const Nf32 val, const NLVSTAGE::TYPE type)
{
	NErrorIf(s1 < s0, NERROR_INCONSISTENT_VALUES);
	NErrorIf(s0 < 0.0f, NERROR_INCONSISTENT_VALUES);
	NErrorIf(s1 < 0.0f, NERROR_INCONSISTENT_VALUES);
	NErrorIf(val < 0.0f, NERROR_VALUE_OUTOFRANGE);

	NErrorIf((type == TYPE::RELATIVE_VALUE) && ( (val >  1.0f) || (val < 0.0f) ), NERROR_VALUE_OUTOFRANGE);

	m_s0	= s0;
	m_s1	= s1;
	m_type	= type;
	m_value	= val;
	//m_flags		= 0;
	//m_cutter0.x	= 0; 
	//m_cutter0.y	= 0; 
	//m_cutter1.x	= 0; 
	//m_cutter1.y	= 0; 
}


void NLVSTAGE::computeAdjustedVelocity(const Nf32 velocity_max)
{

	switch (m_type)
	{
	case TYPE::FORCED:
		NErrorIf(m_v != m_value, NERROR_INCONSISTENT_VALUES);
		NErrorIf(m_v > velocity_max, NERROR_VALUE_OUTOFRANGE);
		break;

	case TYPE::RELATIVE_VALUE:
		NErrorIf(m_value > 1.0f, NERROR_VALUE_OUTOFRANGE);
		NErrorIf(m_value <= 0.0f, NERROR_VALUE_OUTOFRANGE);
		m_v = velocity_max * m_value;
		break;

	case TYPE::ABSOLUTE_VALUE:
		m_v = NMIN(m_value, velocity_max);
		break;


	default:
		m_v = velocity_max;
		break;
	}
}
void NLVSTAGE::draw(NL2DOCS* p2docs, const NCOLORPICKPACK pickpack)
{
	NUTDRAWVERTEX	va;
	NSetColorf(&va.Color0_4f, NCOLOR_PRESET3F_RED, 1.0f);
	va.Position_3f.z = 0.0f;
	va.Position_3f.x = p2docs->transformX(m_s0);
	va.Position_3f.y = p2docs->transformY(m_v);
	NUT_DrawPencil_From(&va);
	va.Position_3f.x = p2docs->transformX(m_s1);
	NUT_DrawPencil_LineTo(&va);

}

void NLVSTAGE::draw(NLPATH* ppath, const Nf32 width, const NCOLORPICKPACK pickpack)
{
	NErrorIf(m_s0 < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(m_s0 > ppath->m_geometry.m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale
	NErrorIf(m_s1 < 0.0f, NERROR_VALUE_OUTOFRANGE); // inf�rieure � l'abscisse minimale
	NErrorIf(m_s1 > ppath->m_geometry.m_ds, NERROR_VALUE_OUTOFRANGE); // sup�rieure � l'abscisse maximale

	ppath->m_geometry.draw(&ppath->m_matrix, width, m_s0, m_s1);
}

void NLVSTAGE::aggregate(NARRAY* poutput_array, const AGGREGATE_FLAGS flags)
{
	NLVSTAGE* prev = (NLVSTAGE*)NGetLastArrayPtr(poutput_array);
	if (prev && (prev->m_v == m_v))
	{
		NErrorIf(m_s1 < prev->m_s1, NERROR_SYSTEM_GURU_MEDITATION);
		
		if (	((m_s0 == prev->m_s1)	&& (ISFLAG_ON(flags, AGGREGATE_FLAGS::PRESERVE_JOINT))) ||			// Il y a un joint et on doit le ppr�server
				((m_s0 > prev->m_s1)	&& (ISFLAG_ON(flags, AGGREGATE_FLAGS::PRESERVE_GAP	)))		)		// il y a un gap et on doit le ppr�server
		{
			NArrayPushBack(poutput_array, (NBYTE*)this);
			return;
		}
		else
			prev->m_s1 = m_s1;
	}
	else
	{
		NArrayPushBack(poutput_array, (NBYTE*)this);
	}
}


// ------------------------------------------------------------------------------------------
/**
 *	@brief	D�coupe le NLVSTAGE � l'emporte pi�ce ( = pcookiecutter )
 *
 *	@param	poutput_array est l'array o� seront stock�s le/les morceaux issus de cette d�coupe.
 *	@param	pcookiecutter est 'l'emporte pi�ce' utilis� pour d�couper ce Velocity Stage.
 *	@param	mode est le mode de restitution qui est compos� de deux flags:
 *	@return	Une combinaison des BITS suivants:
 *			+ \b FLAG_CUTOUT_LEFT		repr�sente tout ou partie du Vstage situ� � GAUCHE de l'emporte piece de decoupe
 *			+ \b FLAG_CUTOUT_COOKIE		repr�sente tout ou partie du Vstage situ� '� l'int�rieur' l'emporte piece de decoupe
 *			+ \b FLAG_CUTOUT_RIGHT		repr�sente tout ou partie du Vstage situ� � DROITE de l'emporte piece de decoupe
 *			+ \b FLAG_CUTOUT_CUT		indique si une tentative de coupe a eut lieu.
 */
// ------------------------------------------------------------------------------------------
Nu32 NLVSTAGE::cutOut(NARRAY* poutput_array, const NLVSTAGE* pcookiecutter, const Nu32 cfg)
{
	NErrorIf(poutput_array->ElementSize != sizeof(NLVSTAGE), NERROR_SYSTEM_GURU_MEDITATION);
	NErrorIf(GET_CUTOUT_CFG_EXPECTED_DEFAULT_RESULT(cfg) && ISFLAG_ON(cfg, FLAG_CUTOUT_FORCE), NERROR_INCONSISTENT_FLAGS);	// Une valeur de retour par d�faut est pr�cis�e alors que la coupe est forc�e
																															// ... et que cette valeur par d�faut ne sera donc jamais retourn�e !			
	
	NLVSTAGE		trim;
	Nf32			ps0, ps1;

	//	a)	Le Cutters est "plus bas" que ce VSTAGE_DESC, donc il l'occulte, et peut le couper/masquer totalement ou en partie.
	//
	//if ((m_v >= pcookiecutter->m_v) || ISFLAG_ON((pcookiecutter->m_flags | m_flags), FLAG_NLVSTAGE_FORCE_CUT_OUT))
	if ((m_v >= pcookiecutter->m_v) || ISFLAG_ON(cfg,FLAG_CUTOUT_FORCE))
	{
	//	NErrorIf(ISFLAG_ON((pcookiecutter->m_flags | m_flags), FLAG_NLVSTAGE_FORCE_CUT_OUT) && pcookiecutter->m_cutter0.y, NERROR_INCONSISTENT_VALUES); // Normalement impossible !!!
	//	NErrorIf(ISFLAG_ON((pcookiecutter->m_flags | m_flags), FLAG_NLVSTAGE_FORCE_CUT_OUT) && pcookiecutter->m_cutter1.y, NERROR_INCONSISTENT_VALUES); // Normalement impossible !!!

		//	c)	On recherche ps0 et ps1 les abscisses des projections de pcutter->m_s0 et pcutter->m_s1 sur ce VSTAGE_DESC selon les directions/Axes fix�es par les "cutters" ( m_cutter0 et m_cutter1 )
		ps0 = pcookiecutter->m_cutter0.y ? pcookiecutter->m_s0 + (pcookiecutter->m_cutter0.x * (m_v - pcookiecutter->m_v)) / pcookiecutter->m_cutter0.y : pcookiecutter->m_s0;
		ps1 = pcookiecutter->m_cutter1.y ? pcookiecutter->m_s1 + (pcookiecutter->m_cutter1.x * (m_v - pcookiecutter->m_v)) / pcookiecutter->m_cutter1.y : pcookiecutter->m_s1;

		//	d)	ps0 (projection de pcutters->m_s0) est-il � droite de m_s1 ?
		if (m_s1 <= ps0)
		{
			if (cfg & FLAG_CUTOUT_LEFT)
			{
				// Cela signifie que la totalit� de ce NLVSTAGE est situ� AVANT le "cone en V" ou le "tube" cr�� par les 2 cutters ( DONC A GAUCHE DU CONE )
				NArrayPushBack(poutput_array, (NBYTE*)this);
				// Entierement � gauche du cone. Le cookiecutter ne peut l'affect�
				return FLAG_CUTOUT_CUT | FLAG_CUTOUT_LEFT;
			}
			else
			{
				// L'utilisateur n'� pas demander � ce que les parties situ�es � gauche du cookie cutter soit push�e ...
				return FLAG_CUTOUT_CUT;
			}
		}

		//	e)	ps1 (projection de pcutters->m_s1) est-il � gauche de m_s0 ?
		if (ps1 <= m_s0)
		{
			if (cfg & FLAG_CUTOUT_RIGHT)
			{
				// Cela signifie que la totalit� de NLVSTAGE est situ� APRES le "cone en V" ou le "tube" cr�� par les 2 cutters ( DONC A DROITE DU CONE )
				NArrayPushBack(poutput_array, (NBYTE*)this);
				return FLAG_CUTOUT_CUT | FLAG_CUTOUT_RIGHT;// ... � droite du cone et peut donc potentiellement �tre affect� par les pcutters suivants.
			}
			else
			{
				// L'utilisateur n'� pas demander � ce que les parties situ�es � droite du cookie cutter soit push�e ...
				return FLAG_CUTOUT_CUT;
			}
		}

		Nu32 _ret = FLAG_CUTOUT_CUT;
		NLVSTAGE cookie = *this;
		//	f)	Il y a intersection entre le cone  et la gauche de ce NLVSTAGE.
		if (ps0 >= m_s0) // rappel: ici on a aussi ps0 < m_s1  -> En effet, (d) n'est pas valid� ! Sinon on ne serait pas ici !
		{
			//  C'est � dire que le d�but de ce NLVSTAGE est avant le Cone de pcutters et qu'ensuite, il entre dans le cone ...
			if (cfg & FLAG_CUTOUT_LEFT)
			{
				trim = *this;
				trim.m_s1 = ps0;
				NArrayPushBack(poutput_array, (NBYTE*)&trim);
				// Pas de return ici car il reste potentiellement une partie de ce NLVSTAGE qui peut sortir � droite du cone ...
				// par contre on specifie le push back  de la partie situ�e � gauche du cone:
				_ret |= FLAG_CUTOUT_LEFT;
			}
			cookie.m_s0 = ps0;
		}

		//	g)	Il y a intersection entre le cone et la droite de ce NLVSTAGE.
		if (ps1 <= m_s1) // rappel: ici on a aussi ps1 > m_s0  -> ! (e) n'est pas valid� sinon on ne serait pas ici !
		{
			
			if (cfg & FLAG_CUTOUT_COOKIE)
			{
				cookie.m_s1 = ps1;
				NArrayPushBack(poutput_array, (NBYTE*)&cookie);
				// on specifie le push back  de la partie situ�e dans le cone:
				_ret |= FLAG_CUTOUT_COOKIE;
			}

			//  C'est � dire que la fin de ce NLVSTAGE est apr�s le Cone et donc que ce NLVSTAGE est dans le cone et en sort
			if (cfg & FLAG_CUTOUT_RIGHT)
			{
				trim = *this;
				trim.m_s0 = ps1;
				NArrayPushBack(poutput_array, (NBYTE*)&trim);
				// on specifie le push back  de la partie situ�e � droite du cone:
				_ret |= FLAG_CUTOUT_RIGHT;
			}
		}
		else if (cfg & FLAG_CUTOUT_COOKIE)
		{
			NArrayPushBack(poutput_array, (NBYTE*)&cookie);
			// on specifie le push back  de la partie situ�e dans le cone:
			_ret |= FLAG_CUTOUT_COOKIE;
		}

		return _ret; 
	}
	else if (GET_CUTOUT_CFG_EXPECTED_DEFAULT_RESULT(cfg))
	{
		NArrayPushBack(poutput_array, (NBYTE*)this);
		return GET_CUTOUT_CFG_EXPECTED_DEFAULT_RESULT(cfg);
	}

	return 0;
}

void NLVSTAGE::setCutters(const NLVSTAGE* p0, const NLVSTAGE* p1)
{
	// Nu32 _null = 0;
	if (p0)
	{
		m_cutter0.x = p0->m_s1 - m_s0;
		m_cutter0.y = p0->m_v - m_v;
	}
	else
	{
		m_cutter0.x = 0.0f;
		m_cutter0.y = 0.0f;
		//_null++;
	}

	if (p1)
	{
		m_cutter1.x = p1->m_s0 - m_s1;
		m_cutter1.y = p1->m_v - m_v;
	}
	else
	{
		m_cutter1.x = 0.0f;
		m_cutter1.y = 0.0f;
		//_null++;
	}
/*
	if(_null == 2)
		FLAG_ON(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT);	// setCutters c'est comport� comme un ResetCutters
	else
		FLAG_OFF(m_flags, FLAG_NLVSTAGE_FORCE_CUT_OUT);	// setCutters c'est comport� comme un ... setCutters	:)
*/
}

void NLVSTAGE::setCutters(const Nf32 cutter0_s, const Nf32 cutter0_v, const Nf32 cutter1_s, const Nf32 cutter1_v)
{
	m_cutter0.x = cutter0_s;
	m_cutter0.y = cutter0_v;

	m_cutter1.x = cutter1_s;
	m_cutter1.y = cutter1_v;
}


// LEGACY
/*
//	|					.																				.
//	|					.																				.
//	|					.																				.
//	|				S0	Oxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx USERKEY INTERVALLE xxxxxxxxxxxxxxxxxxxxxxxxxxxxO  S1
//	|					.																				.
//	|a1)	+-----------+																				+------------+		(a2		|_ VSTAGE_IS_BEFORE / VSTAGE_IS_AFTER
//	|b1)+----------+	.																				.	+-----------+	(b2		|
//	|					.																				.
//	|					.																				.
//	|c)					+-------------------------------------------------------------------------------+							|
//	|d)					+------------------------------------------------------------------+			.							|_ VSTAGE_IS_INCLUDED
//	|e)					.			+-------------------------------------------------------------------+							|
//	|f)					.			+------------------------------------------------------+			.							|
//	|					.																				.
//	|					.																				.
//	|g)			+---------------------------------------------------------------------------------------+							|
//	|h)			+------------------------------------------------------------------------------------------------------+			|- USERKEY_IS_INCLUDED
//	|i)					+----------------------------------------------------------------------------------------------+			|
//	|					.																				.
//	|					.																				.
//	|j)					.			+----------------------------------------------------------------------------------+			|_ INTERSECT
//	|k)			+--------------------------------------------------------------------------+			.							|
enum IRESULT
{
	INTERSECT = 0,			//
	VSTAGE_IS_BEFORE,		//
	VSTAGE_IS_AFTER,		//
	VSTAGE_IS_INCLUDED,		//
	USERKEY_IS_INCLUDED		//
};
NLVSTAGE_EDIT::IRESULT NLVSTAGE_EDIT::intersect(const NLVSTAGEX* pvs)
{
	if (pvs->m_s0 >= m_s1)
		return NLVSTAGE_EDIT::VSTAGE_IS_AFTER;

	if (pvs->m_s1 <= m_s0)
		return NLVSTAGE_EDIT::VSTAGE_IS_BEFORE;

	if (pvs->m_s0 >= m_s0)
	{
		if (pvs->m_s1 <= m_s1)
			return NLVSTAGE_EDIT::VSTAGE_IS_INCLUDED;
	}

	if (pvs->m_s0 <= m_s0)
	{
		// ! Le cas c) pourrait aussi apparaitre ici, mais il est a d�j� �t� d�tecter dan sle test juste audessus et � retourner "VSTAGE_IS_INCLUDED"
		if (pvs->m_s1 >= m_s1)
			return NLVSTAGE_EDIT::USERKEY_IS_INCLUDED;
	}

	return NLVSTAGE_EDIT::INTERSECT;
}
*/

/*
void NLVSTAGE::set(const Nf32 v, const Nf32 s0, const Nf32 s1, const NVEC2* pus0, const NVEC2* pus1)
{
	NErrorIf(s1 < s0, NERROR_INCONSISTENT_VALUES);
	NErrorIf(s0 < 0.0f, NERROR_INCONSISTENT_VALUES);
	NErrorIf(s1 < 0.0f, NERROR_INCONSISTENT_VALUES);
	NErrorIf(v  < 0.0f, NERROR_INCONSISTENT_VALUES);

	m_v = v;
	m_s0 = s0;
	m_s1 = s1;

	if (pus0)
	{
		m_us0 = *pus0;
	}
	else
	{
		m_us0.x = 0.0f;
		m_us0.y = 0.0f;
	}

	if (pus1)
	{
		m_us1 = *pus1;
	}
	else
	{
		m_us1.x = 0.0f;
		m_us1.y = 0.0f;
	}
}
*/

/*
void NLVSTAGE::set(const NLVSTAGEX* pvs, const NLVSTAGEX* pfirst, const NLVSTAGEX* plast)
{
	NErrorIf(pvs < pfirst, NERROR_INCONSISTENT_VALUES);
	NErrorIf(pvs > plast, NERROR_INCONSISTENT_VALUES);

	m_v		= pvs->m_v;
	m_s0	= pvs->m_s0;
	m_s1	= pvs->m_s1;
	/ *
	if(pvs > pfirst)
	{
		m_us0.x = (pvs - 1)->m_s1 - m_s0;
		m_us0.y = (pvs - 1)->m_v  - m_v;
	}
	else
	{
		m_us0.x = 0.0f;
		m_us0.y = 0.0f;
	}

	if (pvs < plast)
	{
		m_us1.x = (pvs + 1)->m_s0 - m_s1;
		m_us1.y = (pvs + 1)->m_v  - m_v;
	}
	else
	{
		m_us1.x = 0.0f;
		m_us1.y = 0.0f;
	}
	* /
}

void NLVSTAGE::set(NLVSTAGE_EDIT* puk)
{
	m_v		= puk->m_v;
	m_s0	= puk->m_s0;
	m_s1	= puk->m_s1;
	/ *
	m_us0.x = 0.0f;
	m_us0.y = 0.0f;
	m_us1.x = 0.0f;
	m_us1.y = 0.0f;
	* /
}
void NLVSTAGE::setUs0(const NVEC2* pus0)
{
	if (pus0)
	{
		m_us0 = *pus0;
	}
	else
	{
		m_us0.x = 0.0f;
		m_us0.y = 0.0f;
	}
}

void NLVSTAGE::setUs1(const NVEC2* pus1)
{
	if (pus1)
	{
		m_us1 = *pus1;
	}
	else
	{
		m_us1.x = 0.0f;
		m_us1.y = 0.0f;
	}
}
*/
