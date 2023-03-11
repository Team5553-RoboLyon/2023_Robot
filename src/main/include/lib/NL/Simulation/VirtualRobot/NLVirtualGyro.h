#pragma once
#include "lib/N/NFlags.h"
#include "lib/N/NType.h"
#include "lib/N/NMath.h"

#include "lib/N/Maths/NVec2f32.h"

#include "NLVirtualOutPut.h"

// Gyroscope: quelques D�rives pr�d�finies:
#define NLVIRTUAL_GYRO_DRIFT_5_DEG_PER_MIN 0.001454441043329f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_10_DEG_PER_MIN 0.002908882086657f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_15_DEG_PER_MIN 0.004363323129986f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_30_DEG_PER_MIN 0.008726646259972f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_45_DEG_PER_MIN 0.013089969389958f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_60_DEG_PER_MIN 0.017453292519943f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_90_DEG_PER_MIN 0.026179938779915f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_135_DEG_PER_MIN 0.039269908169873f // rad/sec
#define NLVIRTUAL_GYRO_DRIFT_180_DEG_PER_MIN 0.05235987755983f	// rad/sec
#define NLVIRTUAL_GYRO_DRIFT_270_DEG_PER_MIN 0.078539816339745f // rad/sec
#define NLVIRTUAL_GYRO_DRIFT_360_DEG_PER_MIN 0.10471975511966f	// rad/sec

class NLVIRTUAL_GYRO
{
	friend class NLVIRTUAL_ROBOT;
	friend class NLVGYRO;

private:
	inline void Clear() { Nmem0(this, NLVIRTUAL_GYRO); }
	inline Nf32 getAngle() { return m_angle; }
	void reset();
	void reset(const NVEC2f32 *paxis);

	void setup(const NVEC2f32 *paxis, const Nf32 drift);
	void update(const Nf32 dt);
	void draw(const NVEC3 *po);

	Nf32 m_driftSpeed;		  // vitesse de d�rive du gyro en rad/sec
	Nf32 m_drift;			  // d�rive totale
	Nf32 m_angle;			  // l'angle du gyro calcul� et stock� � chaque update ( et int�grant la d�rive )
	NVEC2f32 *m_pTrackedAxis; // pointeur sur l'axe 2D "track�"
	NVEC2f32 m_refAxis;		  // Axe 2D de reference stock�
};
