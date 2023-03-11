#ifndef __NGEOMETRY_TEXTURE_COORDINATE_H
#define __NGEOMETRY_TEXTURE_COORDINATE_H
// ***************************************************************************************
// **																					**
// **							NGeometryTextureCoordinate.h							**
// **																					**
// ***************************************************************************************
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NScreenRect.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // TexCoord "UV" or "ST" 2D SPACE.
    //
    //	   0,0 ------- 1,0			+---------U
    //		|           |			|
    //		|           |			|
    //		|     +     |			|
    //		|           |			|
    //		|           |			|
    //	   0,1 --------1,1			V

    // ***************************************************************************************
    // **							  Structures & Defines									**
    // ***************************************************************************************
    // There is no specific TexCoord structure but some typedef are define.
    typedef NVEC2 NTEXCOORD_2f;
    typedef NVEC3 NTEXCOORD_3f;
    typedef NVEC2s16 NTEXCOORD_2s;

    inline void NSetTexCoord2s(NTEXCOORD_2s *ptxc, const Ns16 u, const Ns16 v)
    {
        ptxc->x = u;
        ptxc->y = v;
    }
    inline void NSetTexCoord2f(NTEXCOORD_2f *ptxc, const Nf32 u, const Nf32 v)
    {
        ptxc->x = u;
        ptxc->y = v;
    }
    inline void NSetTexCoord3f(NTEXCOORD_3f *ptxc, const Nf32 u, const Nf32 v, const Nf32 w)
    {
        ptxc->x = u;
        ptxc->y = v;
        ptxc->z = w;
    }

// To ease Texture Coordinates manipulation
#define NTxCYFacingUp(y) (1.0f - (y)) // y is in a Facing Up Base, result of this Macro is a conversion into the Used Base which is Y Facing Down
#define NTxCYFacingDown(y) (y)        // y is in a Facing Down Base, result of this Macro is ... y (because Used Base is Y Facing Down too )

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGEOMETRY_TEXTURE_COORDINATE_H
