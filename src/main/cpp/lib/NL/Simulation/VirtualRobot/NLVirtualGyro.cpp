#include "../../../N/NMemory.h"
#include "../../../N/NMath.h"
#include "NLVirtualGyro.h"

void NLVIRTUAL_GYRO::reset()
{
	if(m_pTrackedAxis)
		m_refAxis	= *m_pTrackedAxis;
	else
	{
		NVec2Set(&m_refAxis, 0.0f, 1.0f);
		m_pTrackedAxis = &m_refAxis;	// l'axe de ref. est par défaut désigné comme étant l'axe tracké. 
	}

	m_angle		= 0.0f;
	m_drift		= 0.0f;
}

void NLVIRTUAL_GYRO::reset(const NVEC2f32* paxis)
{
	if (paxis)
	{
		m_pTrackedAxis = (NVEC2f32*)paxis;
		m_refAxis = *m_pTrackedAxis;
	}
	else
	{
		NVec2Set(&m_refAxis, 0.0f, 1.0f);
		m_pTrackedAxis = &m_refAxis;	// l'axe de ref. est par défaut désigné comme étant l'axe tracké. 
	}

	m_angle = 0.0f;
	m_drift = 0.0f;
}

void NLVIRTUAL_GYRO::setup(const NVEC2f32* paxis, const Nf32 driftspeed)
{
	m_driftSpeed = driftspeed;
	reset(paxis);
}

void NLVIRTUAL_GYRO::update(const Nf32 dt)
{
	// Angle entre l'axe tracké et l'axe de reference
	Nf32 fdot = NVec2DotProduct(m_pTrackedAxis,&m_refAxis);
	fdot = NCLAMP(-1.0f, fdot, 1.0f);

	NVEC2f32 direct_ortho;
	direct_ortho.x = -m_refAxis.y;
	direct_ortho.y =  m_refAxis.x;
	Nf32 fdot2 = NVec2DotProduct(m_pTrackedAxis, &direct_ortho);

	// Si le signe du produit scalaire avec le vecteur orthogonal direct à l'axe de ref est négatif ...
	// cela signifie que le vecteur tracké se trouve à droite de l'axe de ref.
	if (fdot2 < 0.0f)
		m_angle = NF32_2PI - acosf(fdot);
	// ... le signe est positif ou null, le vecteur tracké est à gauche de l'axe de ref.
	else
		m_angle = acosf(fdot);

	// Ici m_angle est dans l'intervalle [0,2PI] 

	// ajout de la derive
	m_drift += m_driftSpeed * dt;
	m_angle += m_drift;
	
	// maintient de m_angle dans l'intervalle [0,2PI] 
	while (m_angle < 0.0f)		{ m_angle = NF32_2PI + m_angle; }
	while (m_angle > NF32_2PI)	{ m_angle -= NF32_2PI; }
}
