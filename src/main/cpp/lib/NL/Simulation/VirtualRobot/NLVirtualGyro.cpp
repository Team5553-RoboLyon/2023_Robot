#include "../../../N/NMemory.h"
#include "../../../N/NMath.h"
#include "../../../N/Utilities/Draw/NUT_Draw.h"

#include "NLVirtualGyro.h"

void NLVIRTUAL_GYRO::reset()
{
	if (m_pTrackedAxis)
		m_refAxis = *m_pTrackedAxis;
	else
	{
		NVec2Set(&m_refAxis, 0.0f, 1.0f);
		m_pTrackedAxis = &m_refAxis; // l'axe de ref. est par d�faut d�sign� comme �tant l'axe track�.
	}

	m_angle = 0.0f;
	m_drift = 0.0f;
}

void NLVIRTUAL_GYRO::reset(const NVEC2f32 *paxis)
{
	if (paxis)
	{
		m_pTrackedAxis = (NVEC2f32 *)paxis;
		m_refAxis = *m_pTrackedAxis;
	}
	else
	{
		NVec2Set(&m_refAxis, 1.0f, 0.0f);
		m_pTrackedAxis = &m_refAxis; // l'axe de ref. est par d�faut d�sign� comme �tant l'axe track�.
	}

	m_angle = 0.0f;
	m_drift = 0.0f;
}

void NLVIRTUAL_GYRO::setup(const NVEC2f32 *paxis, const Nf32 driftspeed)
{
	m_driftSpeed = driftspeed;
	reset(paxis);
}

void NLVIRTUAL_GYRO::update(const Nf32 dt)
{
	// Angle entre l'axe track� et l'axe de reference
	Nf32 fdot = NVec2DotProduct(m_pTrackedAxis, &m_refAxis);
	fdot = NCLAMP(-1.0f, fdot, 1.0f);

	NVEC2f32 direct_ortho;
	direct_ortho.x = -m_refAxis.y;
	direct_ortho.y = m_refAxis.x;
	Nf32 fdot2 = NVec2DotProduct(m_pTrackedAxis, &direct_ortho);

	// Si le signe du produit scalaire avec le vecteur orthogonal direct � l'axe de ref est n�gatif ...
	// cela signifie que le vecteur track� se trouve � droite de l'axe de ref.
	if (fdot2 < 0.0f)
		m_angle = NF32_2PI - acosf(fdot);
	// ... le signe est positif ou null, le vecteur track� est � gauche de l'axe de ref.
	else
		m_angle = acosf(fdot);

	// Ici m_angle est dans l'intervalle [0,2PI]

	// ajout de la derive
	m_drift += m_driftSpeed * dt;
	m_angle += m_drift;

	// maintient de m_angle dans l'intervalle [0,2PI]
	while (m_angle < 0.0f)
	{
		m_angle = NF32_2PI + m_angle;
	}
	while (m_angle > NF32_2PI)
	{
		m_angle -= NF32_2PI;
	}
}

void NLVIRTUAL_GYRO::draw(const NVEC3 *po)
{
	NVEC2 j;

	NCOLOR col;
	NUT_DRAWTEXT params;
	Nchar txt[32];

	j.x = 0.0f;
	j.y = 0.04f;

	NSetColorf(&col, NCOLOR_PRESET3F_BLUE_AZURE, 1.0f);
	NUT_Draw_XYCircle(po, 0.04f, &col);

	NUT_Draw_Angle(po, &j, m_angle, &col);

	NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
	params.Size = 0.01f;
	sprintf(txt, "Angle : %.3f rad", m_angle);
	NVEC3 p;
	p.x = po->x - 0.05f;
	p.y = po->y - 0.05f;
	p.z = 0.0f;

	NUT_Draw_Text(txt, &p, &params);
}
