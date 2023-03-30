#pragma once

#include "lib/N/NType.h"
#include "lib/N/Core/NVersion.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Maths/NVec3f32.h"
#include "NLMotorCharacterization.h"

#ifdef _NEDITOR
#endif


#define FLAG_NLCHARACTERIZATION_TABLE_ROW_IS_INVERTED			BIT_0	
typedef struct NLCHARACTERIZATION_TABLE_ROW	NLCHARACTERIZATION_TABLE_ROW;
struct NLCHARACTERIZATION_TABLE_ROW
{
	Nu32 read(FILE* pfile) { return (fread(this, sizeof(NLCHARACTERIZATION_TABLE_ROW), 1, pfile) == 1) ? 1 : 0; };
	Nu32 write(FILE* pfile) { return (fwrite(this, sizeof(NLCHARACTERIZATION_TABLE_ROW), 1, pfile) == 1) ? 1 : 0; };

	Nu16						m_flags;
	Nu8							m_gearboxId;					// de 0 � n
	Nu8							m_motorId;						// 'gearbox relative' c'est � dire que pour chaque Gearbox les moteurs sont num�rot�s de 0 � n
	Nchar						m_motorName[8];					// nom du moteur 7 lettres max (+ caractere null de fin ).
	NLMOTOR_CHARACTERIZATION	m_characterization;				// Kv, Ka, Intercept en forward et backward.
	Nf32						m_gearRatio;					// Gearbox ratio = Driven / Drive 
	Nf32						m_angularVelocityScaleFactor;	// = traction wheel radius / Gearbox ratio 
																//		Kangv = kv*m_angularVelocityScaleFactor
																//		Kanga = ka*m_angularVelocityScaleFactor
	Nf32						m_forwardVelocityMax;			// m/s		
	Nf32						m_forwardAccelerationMax;		// m/s�	
	Nf32						m_backwardVelocityMax;			// m/s	
	Nf32						m_backwardAccelerationMax;		// m/s�	

};

typedef struct NLCHARACTERIZATION_TABLE	NLCHARACTERIZATION_TABLE;
struct NLCHARACTERIZATION_TABLE
{	
	NLCHARACTERIZATION_TABLE();
	NLCHARACTERIZATION_TABLE(const Nu8 capacity);

	~NLCHARACTERIZATION_TABLE();

	Nu32 read(FILE* pfile);
	Nu32 write(FILE* pfile);

	Nu32 load(const Nchar *pfilename);
	Nu32 save(const Nchar* pfilename);

	Nu32 importTxt(const Nchar *pfilename, const Nbool bclear_table_before_loading=NFALSE);
	NLMOTOR_CHARACTERIZATION*	get(NLMOTOR_CHARACTERIZATION *pdst, const Nu8 table_index, const Nbool angular_velocity_scaled_coef);
	NLMOTOR_CHARACTERIZATION*	get(NLMOTOR_CHARACTERIZATION *pdst, const Nu8 gearboxid,const Nu8 motorid, const Nbool angular_velocity_scaled_coef);
	NLMOTOR_CHARACTERIZATION*	get(NLMOTOR_CHARACTERIZATION *pdst, const Nchar *pmotorname, const Nbool angular_velocity_scaled_coef);

	Nu32						getGearBoxNb() { return m_gearBoxNb; };
	Nu32						getMotorNb(const Nu8 gearboxid);
	NARRAY	m_table;
	Nu8		m_gearBoxNb;
	Nu8		m_availableNu8;
	Nu16	m_availableNu16;
};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define EXTENSION_NLCHARACTERIZATION_TABLE_TXT		".txt"	// extention des noms de fichiers de characterization (format texte)
#define EXTENSION_NLCHARACTERIZATION_TABLE_BIN		".czt"	// extention des noms de fichiers de characterization (format binaire)
#define SIGNATURE_NLCHARACTERIZATION_TABLE_TXT		"characterization\n"
#define SIGNATURE_NLCHARACTERIZATION_TABLE_BIN		NMAKESIGNATURE('C','R','Z','T')
#define VERSION_NLCHARACTERIZATION_TABLE_HEADER		NMAKEVERSION(0,0,0)				// >>> 2022/05/22
typedef struct
{
	NARRAYBOUNDS	m_tableBounds;
	Nu8				m_gearBoxNb;
	Nu8				m_availableNu8;
	Nu16			m_availableNu16;
}NLCHARACTERIZATION_TABLE_HEADER;
