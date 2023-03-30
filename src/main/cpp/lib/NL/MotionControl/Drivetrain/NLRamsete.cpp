
#include <float.h>
//#include "../../N/NMemory.h"
#include "lib/N/Maths/NMath.h"
#include "lib/N/Miscellaneous/NString.h"
#include "lib/N/NErrorHandling.h"
#include "lib/NL/MotionControl/Drivetrain/NLRamsete.h"


#ifdef _NEDITOR
//#include "../NLPathWorkbench.h"
#endif

NLRAMSETEOUTPUT* NLRAMSETE::compute(NLRAMSETEOUTPUT* pout, const Nf32 vref, const Nf32 wref, const Nf32 erra, const Nf32 cos_erra, const Nf32 errx, const Nf32 erry )
{
	Nf32 _k					= 2.0f * m_zeta * sqrt(NPOW2(wref) + m_b * NPOW2(vref));
	pout->m_velocity		= vref * cos_erra + _k * (errx);
	pout->m_angularVelocity = wref + _k * erra + m_b * vref * NSinc(erra) * (erry);

//	Nf32 _k					= NLRAMSETE_Compute_k(m_zeta, m_b, vref, wref);
//	pout->m_velocity		= NLRAMSETE_Compute_velocity(_k, cos_erra, errx, vref);
//	pout->m_angularVelocity = NLRAMSETE_Compute_angularVelocity(_k, m_b, erra, erry, vref, wref);
	return pout;
}

#ifdef _NEDITOR

Nu32 NLRAMSETE::read(NLPATH_WORKBENCH* pwb)
{
	if (pwb)
	{
		m_b		= (pwb->m_specificRamsete.m_b == NF32_MAX) ? (pwb->m_pRamsete ? pwb->m_pRamsete->m_b: DEFAULT_NLRAMSETEPARAMS_B) : pwb->m_specificRamsete.m_b;
		m_zeta	= (pwb->m_specificRamsete.m_zeta == NF32_MAX) ? (pwb->m_pRamsete ? pwb->m_pRamsete->m_zeta: DEFAULT_NLRAMSETEPARAMS_ZETA) : pwb->m_specificRamsete.m_zeta;
	}
	else
	{
		m_b		= DEFAULT_NLRAMSETEPARAMS_B;
		m_zeta	= DEFAULT_NLRAMSETEPARAMS_ZETA;
	}
	return 1;
}
#endif
Nu32 NLRAMSETE::read(FILE* pfile)
{
	if (fread(&m_b, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	if (fread(&m_zeta, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	return 1;
}
Nu32 NLRAMSETE::write(FILE* pfile)
{
	if (fwrite(&m_b, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	if (fwrite(&m_zeta, sizeof(Nf32), 1, pfile) != 1)
		return 0;
	return 1;
}

Nu32  NLRAMSETE::importTxt(const Nchar* ptxtfilename)
{
	NErrorIf(!ptxtfilename, NERROR_NULL_POINTER);
	// Check extension
	if (!NStrCheckEnd(ptxtfilename, EXTENSION_NLRAMSETE_TXT))
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
	if (strcmp(name, SIGNATURE_NLRAMSETE))
	{
		fclose(pfile);
		return 0;
	}
	// recup�rer le parametre b dans le fichier
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "b= ", &m_b);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "zeta= ", &m_zeta);
	fclose(pfile);
	return 1;
}
