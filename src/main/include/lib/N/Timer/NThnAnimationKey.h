#ifndef __N_THN_ANIMATION_KEY_H
#define __N_THN_ANIMATION_KEY_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NThnAnimationKey.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NQuaternion.h"
#include "lib/N/Miscellaneous/NEasing.h"
#include "lib/N/NTransformationTree.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	typedef struct
	{
		Nf32 Time;							   // To reach this key from the previous one.
		NVEC3 Translation;					   //
		NVEC3 Scale;						   //
		NQUATERNION Rotation;				   //
		NEASING_HANDLE_ID_ENUM EasingHandleID; // NEASING_LINEAR, NEASING_IN, NEASING_OUT or NEASING_INOUT
	} NTHNANIM_KEY;

	inline void NSetThnAnimKey_Easing(NTHNANIM_KEY *pthnkey, const NEASING_HANDLE_ID_ENUM easing) { pthnkey->EasingHandleID = easing; }
	inline void NSetThnAnimKey_Time(NTHNANIM_KEY *pthnkey, const Nf32 time) { pthnkey->Time = time; }

	inline void NSetThnAnimKey_Rotation_Identity(NTHNANIM_KEY *pthnkey) { NIdentityQuaternion(&pthnkey->Rotation); }
	inline void NSetThnAnimKey_Rotation(NTHNANIM_KEY *pthnkey, const NQUATERNION *pquat) { pthnkey->Rotation = *pquat; }
	inline void NSetThnAnimKey_Rotationf(NTHNANIM_KEY *pthnkey, const Nf32 rx, const Nf32 ry, const Nf32 rz) { NQuaternionRotation(&pthnkey->Rotation, rx, ry, rz); }

	inline void NSetThnAnimKey_Scale_Identity(NTHNANIM_KEY *pthnkey) { NVec3Set(&pthnkey->Scale, 1.0f, 1.0f, 1.0f); }
	inline void NSetThnAnimKey_Scale(NTHNANIM_KEY *pthnkey, const NVEC3 *pv) { pthnkey->Scale = *pv; }
	inline void NSetThnAnimKey_Scalef(NTHNANIM_KEY *pthnkey, const Nf32 x, const Nf32 y, const Nf32 z) { NVec3Set(&pthnkey->Scale, x, y, z); }

	inline void NSetThnAnimKey_Translation_NULL(NTHNANIM_KEY *pthnkey) { NVec3Set(&pthnkey->Translation, 0, 0, 0); }
	inline void NSetThnAnimKey_Translation(NTHNANIM_KEY *pthnkey, const NVEC3 *pv) { pthnkey->Translation = *pv; }
	inline void NSetThnAnimKey_Translationf(NTHNANIM_KEY *pthnkey, const Nf32 x, const Nf32 y, const Nf32 z) { NVec3Set(&pthnkey->Translation, x, y, z); }

	inline void NSetThnAnimKey_from_Thn(NTHNANIM_KEY *pthnkey, const NTRANSFORM_HNODE *pthn, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing)
	{
		pthnkey->Time = time;
		pthnkey->EasingHandleID = easing;
		pthnkey->Translation = pthn->vTranslation;
		pthnkey->Scale = pthn->vScale;
		pthnkey->Rotation = pthn->QuatRotation;
	}
	inline void NSetThnAnimKey_Identity(NTHNANIM_KEY *pthnkey, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing)
	{
		pthnkey->Time = time;
		pthnkey->EasingHandleID = easing;
		NVec3Set(&pthnkey->Translation, 0, 0, 0);
		NVec3Set(&pthnkey->Scale, 1.0f, 1.0f, 1.0f);
		NIdentityQuaternion(&pthnkey->Rotation);
	}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __N_THN_ANIMATION_KEY_H
