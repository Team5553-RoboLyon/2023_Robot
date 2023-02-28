#ifndef __NSTATESET_H
#define __NSTATESET_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NStateSet.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/GL/Ngl.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// a STATESET is a set of FLAGS used at rendering.
// To optimize rendering process and avoid as much as possible changing renderings state for each geometry,
// the geometries with the same STATESET are all put together into the same ACCUMULATOR.
// BY THE WAY ... each Valid STATESET match with one unique ACCUMULATOR.
//

// ***************************************************************************************
// **							Structures & Define										**
// ***************************************************************************************
// Render state set FLAGS
#define FLAG_STATESET_ZREAD BIT_0  // |_Z
#define FLAG_STATESET_ZWRITE BIT_1 // |

#define FLAG_STATESET_BLEND BIT_2	 // |
#define FLAG_STATESET_LIGHT BIT_3	 // |-APPEARANCE
#define FLAG_STATESET_TEXTURES BIT_4 // |

#define FLAG_STATESET_RESERVED_BIT_5 BIT_5 // |
#define FLAG_STATESET_RESERVED_BIT_6 BIT_6 // |-Reserved ( Unused yet, so usable but only by Engine )
#define FLAG_STATESET_RESERVED_BIT_7 BIT_7 // |

	// |
	// |-BIT_8 to BIT_31 available for user.
	// |

#define MASK_STATESET_Z (0x3)						 // BIT_0 to BIT_1 included
#define MASK_STATESET_APPEARANCE (0x1C)				 // BIT_2 to BIT_4 included
#define MASK_STATESET_Z_AND_APPEARANCE (0x1F)		 // BIT_0 to BIT_4 included
#define MASK_STATESET_APPEARANCE_AND_RESERVED (0xFC) // BIT_2 to BIT_7 included

#define MASK_STATESET_BLENDLIGHT (0xC)		 // BIT_2 and BIT_3
#define MASK_STATESET_Z_AND_BLENDLIGHT (0xF) // BIT_0 to BIT_3 included

#define MASK_STATESET_ENGINE (0xFF)		// == MASK_STATESET_Z_AND_APPEARANCE and Reserved (BIT_0 to BIT_7 included)
#define MASK_STATESET_USER (0xFFFFFF00) // BIT_8 to BIT_31 included

	typedef Nu32 NSTATESET;

	extern NSTATESET NCurrentStateSet;
	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	// Purpose of this inline function is trying to change gl states as little as possible.
	inline void NUpdateCurrentStateSet(const NSTATESET newstateset)
	{
		// ZBUFFER READ/WRITE ----------------------------------------------------------------
		if (ISFLAG_ON(newstateset, FLAG_STATESET_ZWRITE))
		{
			if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_ZWRITE))
				glDepthMask(GL_TRUE);
		}
		else
		{
			if (ISFLAG_ON(NCurrentStateSet, FLAG_STATESET_ZWRITE))
				glDepthMask(GL_FALSE);
		}

		if (ISFLAG_ON(newstateset, FLAG_STATESET_ZREAD))
		{
			if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_ZREAD))
				glEnable(GL_DEPTH_TEST);
		}
		else
		{
			if (ISFLAG_ON(NCurrentStateSet, FLAG_STATESET_ZREAD))
				glDisable(GL_DEPTH_TEST);
		}
		// TEXTURE ---------------------------------------------------------------------------
		if (ISFLAG_ON(newstateset, FLAG_STATESET_TEXTURES))
		{
			if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_TEXTURES))
			{
				glEnable(GL_TEXTURE_2D);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		else
		{
			if (ISFLAG_ON(NCurrentStateSet, FLAG_STATESET_TEXTURES))
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisable(GL_TEXTURE_2D);
			}
		}
		// BLEND mode ------------------------------------------------------------------------
		if (ISFLAG_ON(newstateset, FLAG_STATESET_BLEND))
		{
			if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_BLEND))
			{
				glEnable(GL_BLEND);
			}
		}
		else
		{
			if (ISFLAG_ON(NCurrentStateSet, FLAG_STATESET_BLEND))
			{
				glDisable(GL_BLEND);
			}
		}
		// LIGHT mode -------------------------------------------------------------------------
		if (ISFLAG_ON(newstateset, FLAG_STATESET_LIGHT))
		{
			if (ISFLAG_OFF(NCurrentStateSet, FLAG_STATESET_LIGHT))
			{
				glEnable(GL_LIGHTING);
			}
		}
		else
		{
			if (ISFLAG_ON(NCurrentStateSet, FLAG_STATESET_LIGHT))
			{
				glDisable(GL_LIGHTING);
			}
		}

		// ... And keep all the new current states --------------------------------------------
		NCurrentStateSet = newstateset;
	}
/*
inline void NResetCurrentStateSet()
{
	// ZBUFFER READ/WRITE ----------------------------------------------------------------
	FLAG_OFF( NCurrentStateSet,FLAG_STATESET_ZWRITE );		glDepthMask(GL_FALSE);
	FLAG_OFF( NCurrentStateSet,FLAG_STATESET_ZREAD  );		glDisable(GL_DEPTH_TEST);
	// TEXTURE ---------------------------------------------------------------------------
	FLAG_OFF( NCurrentStateSet,FLAG_STATESET_TEXTURES );	glDisable(GL_TEXTURE_2D); glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// BLEND mode ------------------------------------------------------------------------
	FLAG_OFF( NCurrentStateSet,FLAG_STATESET_BLEND );		glDisable(GL_BLEND);
	// LIGHT mode -------------------------------------------------------------------------
	FLAG_OFF( NCurrentStateSet,FLAG_STATESET_LIGHT );		glDisable(GL_LIGHTING);
}
*/
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + LOW LEVEL & PRIVATE functions : Setup & Clear / N_Init & N_Disable
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NSTATESET_H
