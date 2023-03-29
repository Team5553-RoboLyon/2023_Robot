#pragma once
#include "../../../N/NType.h"
#include "../../../N/Maths/NMath.h"
#include "../../../N/Maths/NMatrix.h"
#include "../../../N/Core/NVersion.h"

//#include "../../../N/Maths/NMatrix3x3.h"
#include "../NLKinLimits.h"
#include "../DriveTrain/NLDriveTrainSpecs.h"
#include "./Geometry/NLPathGeometry.h"

class NLPATH
{
public:
	NLPATH();
	NLPATH(const Nu32 pathpoints_capacity, const Nu32 primitives_capacity);
	~NLPATH();

	Nu32	write(FILE* pfile);
	Nu32	read(FILE* pfile);

	Nf32 calculateMaxCruiseVelocity(const NLKINLIMITS* pklim) const;
	Nu32 buildVStages(NARRAY* pout_vstagedesc_array, const NLDRIVETRAINSPECS* pdtspecs, const NLKINLIMITS* pklim) const;

	NVEC2f32* convertGlobalPositionToLocal(NVEC2f32* pres, const NVEC2f32* pglobal_position)const;
	NVEC2f32* convertGlobalDirectionToLocal(NVEC2f32* pres, const NVEC2f32* pglobal_direction)const;
	NVEC2f32* convertLocalPositionToGlobal(NVEC2f32* pres, const NVEC2f32* plocal_position)const;
	NVEC2f32* convertLocalDirectionToGlobal(NVEC2f32* pres, const NVEC2f32* plocal_direction)const;

#ifdef _NEDITOR
	inline void draw(const Nf32 path_width) { m_geometry.draw(&m_matrix, path_width); }
#endif
	//NVEC3f32			m_origin; // Position de l'origine	
	//NMATRIX3x3		m_matrix; // matrice de rotation 3D
	NMATRIX				m_matrix; // Matrice de transformation 3D 4x4
	NLPATH_GEOMETRY		m_geometry;
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLPATH_HEADER			NMAKEVERSION(0,0,0)				// >>> 2021/07/31 (2021/04/05)
typedef struct
{
	//NVEC3f32			m_origin; // Position de l'origine	
	//NMATRIX3x3		m_matrix; // matrice de rotation 3D
	NMATRIX				m_matrix; // Matrice de transformation 3D 4x4
}NLPATH_HEADER;
