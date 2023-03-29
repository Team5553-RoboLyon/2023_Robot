#pragma once
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec2f32.h" 
#include "lib/NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"

typedef struct NLROBOTPOSE NLROBOTPOSE;
struct NLROBOTPOSE
{
	//friend class NLRAMSETE;
	//friend class NLFOLLOWER_TANK;
public:
	inline void reset() { m_position.x = 0.0f; m_position.y = 0.0f; m_angle = 0.0f; }
	inline void set(const Nf32 x, const Nf32 y, const Nf32 a) { m_position.x = x; m_position.y = y; m_angle = a; }
	inline void set(const NVEC2f32 *p, const Nf32 a) { m_position = *p; m_angle = a; }

	
	//void odometryUpdate(const NLDRIVETRAINSPECS* pspecs, const Nf32 left_mov, const Nf32 right_mov);
	//void odometryUpdate(const NLDRIVETRAINSPECS* pspecs, const Nf32 left_mov, const Nf32 right_mov,const Nf32 external_angle);

//private:
	NVEC2f32	m_position;	// position x,y ( ... du robot au sol )
	Nf32		m_angle;	// rotation du robot ( ... angle du robot )
};

