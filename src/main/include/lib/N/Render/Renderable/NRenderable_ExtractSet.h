#ifndef __NRENDERABLE_EXTRACTSET_H
#define __NRENDERABLE_EXTRACTSET_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NRenderable_ExtractSet.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
typedef Nu32	NEXTRACTSET;

// -------------------------------------------------------------------------------------------------
// GLOBAL VIEW
//
// Because Extract function are critical it's better to build each of them as specific as possible and avoid
// having "heavy process" inside.
// So, to manage all the extraction subtleties we have to deal with a lot of small extraction functions.
// ... and of course, we need a way, as simple as possible, for user to find the specific extraction function he wants,
// AND THAT'S THE GOAL of the NEXTRACTSET.
// NEXTRACTSET offers to user a set of flags to describe the extraction specificities he wants.
// And, thanks to simple lookup function, this combination of Flags/Values is used to retrieve the right extraction function ID.
//
// To achieve that goal N uses 2 LUTs.
//		1/NRenderableExtractFCT_LUT
//		2/NExtractSet_LUT
// The first one is a finite array of NRENDERABLE_EXTRACT_FCTs where each NRENDERABLE_EXTRACT_FCT is different than the other.
// The second one is a finite array of NEXTRACTSET_KEYs which combine 2 values, a NRENDERABLE_EXTRACT_FCTID (to lookup into into the NRenderableExtractFCT_LUT) .
// and a NEXTRACTSET.
// 
// A Nu32 NEXTRACTSET is a UNIQUE 32 bits value. ( ... so, as i said, associated with a NRENDERABLE_EXTRACT_FCTID into a NEXTRACTSET_KEY Structure ).
// One NEXTRACTSET is guarantee to be referenced only one time in the LUT, so, by the way each NEXTRACTSET_KEY is UNIQUE.
// But the same NRENDERABLE_EXTRACT_FCTID may be associated several time with different NEXTRACTSETs.
// 
// Look up process:
// The 2 LUTs are initialized at the N_Init() call.
// User may add some user NRENDERABLE_EXTRACT_FCTs in the NRenderableExtractFCT_LUT.
// The NExtractSet_LUT is completely close.
// 
// 2 ways are available to get a NRENDERABLE_EXTRACT_FCT, 
//		-with a NRENDERABLE_EXTRACT_FCTID by looking up directly into the NRenderableExtractFCT_LUT.
//		-with a NEXTRACTSET. In that case N has to research first this NEXTRACTSET into the NExtractSet_LUT
//		 by checking each NEXTRACTSET_KEY.NEXTRACTSET against incoming NEXTRACTSET. when the UNIQUE valid Key is found, 
//		 N get the associated NRENDERABLE_EXTRACT_FCTID and look up into the NRenderableExtractFCT_LUT.
// ... thats all !!!
//
// One more thing:
//		NEXTRACTSET building is not only done by user. Some part are really close to the deep Engine.
//		some others more high level. So some FLAGS/VALUES are going to be 'LOW LEVEL', that means 
//		NENGINE will manage it, user is not authorized to use them !!! ( it crashes if he try )
//		... and some other ones are for user only.
//
// -------------------------------------------------------------------------------------------------
// NEXTRACSET details:
// 
// NEXTRACSET bits/flags are organized (in term of meanings and type of using) in 2 mains "LEVELS" which are LOW LEVEL and HIGH LEVEL.
//
// LOW LEVEL (or INTERNAL) are used internally by NENGINE.
// For example the INTERNAL/LOW LEVEL "FLAG_NEXTRACTSET_MULTIGEOMETRIES" is set to ON/OFF by NENGINE objects themselves not by user.
// ... A particle emitter is going to set it to OFF at its NRENDERABLE creation because it knows it uses only one geometry.
//
// HIGH LEVEL (or EXTERNAL) are used by user.
// For example the EXTERNAL/HIGH LEVEL "FLAG_NEXTRACTSET_FRUSTUMCULLING" may be set to ON/OFF by user to specify if he wants a frustum culling or not.
// Other example the EXTERNAL/HIGH LEVEL "FLAG_NEXTRACTSET_HUD" may be set to ON/OFF by user to specify if he wants to extract the renderable geometries in
// the specific HUD Accumulator... etc.
// 
// System allows NGENGINE and USER to define their specifications both on their side, in the same NEXTRACSET.
// Example: for a particle emitter, user is going to set "FLAG_NEXTRACTSET_FRUSTUMCULLING" but NENGINE (NParticleEmitter setup function actually )
// is going to set OFF FLAG_NEXTRACTSET_MULTIGEOMETRIES on its side.
//
// The number of possible NEXTRACSET is the total number of possible combinations between these FLAGS multiply by the number of SECTIONs.
// Actually some combinations make nonsense like FLAG_NEXTRACTSET_HUD and FLAG_NEXTRACTSET_3D ( it could be one or the other but not the 2 !).
// If an required NEXTRACTSET doesn't exist ... CRASH in debug mode. 
// 
// SECTION ID
// SECTION ID uses 16 BITS of the NEXTRACSET as a Nu16 not as 16 FLAGS.
// SECTION ID is considered as a LOW LEVEL/INTERNAL value. So used by NENGINE only.
// SECTIONS are like "groups". N can manage 65535 different SECTIONS at maximum (Nu16MAX) !( Now only 7 are used ... )
// It allows the system to be more specific and by the way to use more specific  NRENDERABLE_EXTRACT_FCT.
//		NEXTRACTSET = (SECTION ID | FLAGS combination) so, thanks to this SECTION ID, it's possible to have
//		different NRENDERABLE_EXTRACT_FCT for the same FLAGS combination.
// To be more precise, NEngine uses those SECTIONID to use specific NRENDERABLE_EXTRACT_FCT for a specific type or group of Objects.
// For instance, all UIs are using a specific SECTION ID, like this, when user set FLAG_NEXTRACTSET_HUD (which is not UI ! but identify a type of rendering ) to ON
//	->for an UI Object, the NRENDERABLE_EXTRACT_FCT linked with the NEXTRACSET(_SECTION_UI,_HUD) will be used ( _SECTION_UI is going to be setup internaly by the UI Object )
//	->for a standard 3D Object, the NRENDERABLE_EXTRACT_FCT linked with the NEXTRACSET(_SECTION_STANDARD,_HUD) will be used ( _SECTION_STANDARD is going to be setup internaly by the standard Object )
// ... so, with the same user specifications, for 2 different types of objects, system is able to describe 2 different NRENDERABLE_EXTRACT_FCT.
// 
// IMPORTANT NOTES:
// If a user uses "user define extract function", he has to be aware of this and be sure to make it's definition before saving/loading
// any Renderable. 
//		
// -------------------------------------------------------------------------------------------------
// |___________|___________|___________|___________|___________|___________|___________|___________|
// |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
// |					                           |X |X |	|  |  |  |	|  |  |  |  |  |  |  |  |F |
// |				 SECTION ID                    |G |I |  |  |  |  |  |  |  |  |  |  |  |H |3 |C |
// |					                           |e |n |  |  |  |  |  |  |  |  |  |  |  |U |D |U |
// |				 0 to  65535                   |o |s |  |  |  |  |  |  |  |  |  |  |  |D |  |L |
// |					                           |m |t |  |  |  |  |  |  |  |  |  |  |  |  |  |L |
// ------------------------------------------------|-----------|-----------|-----------|-----------|
// |					LOW						   |    LOW    |    LOW    |    HIGH   |    HIGH   |
// |				  SECTION					   |	CORE   |  SPECIFIC | SPECIFIC  |    CORE   |
// ------------------------------------------------------------------------|-----------------------|
// |									LOW LEVEL						   |	  HIGH LEVEL	   |
// |										=							   |		  =			   |
// |									 INTERNAL						   |	   EXTERNAL		   |
// -------------------------------------------------------------------------------------------------
// Reminders:
// LOW LEVEL (INTERNAL)	are defined by N. User is not able to edit them.
// HIGH LEVEL (EXTERNAL)are the ones define by user at Renderable creation, mainly to specify HUD, 3D, CULLING ...
//
// -------------------------------------------------------------------------------------------------------------------------------------------
// FLAGS Details
//
// FLAG_NEXTRACTSET_MULTIINSTANCES
// Is only managed (set to ON/OFF and tested) by some specific parts of the engine managing Instances. 
// Set to OFF means SINGLEINSTANCE. (start status for all Renderable)
//
// FLAG_NEXTRACTSET_MULTIGEOMETRIES
// Is set to ON or OFF by each Renderable Owner at its own setup. the rule is really simple,
// if the Renderable will include only one geometry FLAG_NEXTRACTSET_MULTIGEOMETRIES should set to OFF, otherwise, it should set to ON.
// Means Single Geometry when it's set to OFF...
//
// FLAG_NEXTRACTSET_HUD
// Is set to ON or OFF by user at Renderable creation. The rule is really simple,
// if the flag is set to ON, Renderable geometries are going to be extracted as HUD geometries, into the unique HUD accumulator.
// if the flag is set to OFF, Renderable geometries are going to be extracted as HUD geometries, into the HUD accumulator.
//
// FLAG_NEXTRACTSET_3D
// todo
// FLAG_NEXTRACTSET_FRUSTUMCULLING
// todo
// -------------------------------------------------------------------------------------------------------------------------------------------
// SECTION ID Details
// ( Are like "groups". N can manage 65535 different SECTIONS at maximum ! Now only 7 are used ... )

// |===========================================================|
// |					SECTION ID							   |- BIT_31 to BIT_16: SECTIONID
// |===========================================================|

#define FLAG_NEXTRACTSET_MULTIGEOMETRIES			BIT_15	// |
#define FLAG_NEXTRACTSET_MULTIINSTANCES				BIT_14	// |_CORE LOW LEVEL FLAGS (Internal)- Managed by Engine Only
//#define FLAG_NEXTRACTSET_xx						BIT_13	// |
//#define FLAG_NEXTRACTSET_xx						BIT_12	// |

//#define FLAG_NEXTRACTSET_LOWSPECIFIC_xx			BIT_11	// |
//#define FLAG_NEXTRACTSET_LOWSPECIFIC_xx			BIT_10	// |_SECTION SPECIFIC LOW LEVEL FLAGS (Internal)- Managed by Engine Only 
//#define FLAG_NEXTRACTSET_LOWSPECIFIC_xx			BIT_9	// | (Each of them is redefine for each SECTION. So the same BIT will have different meaning depending its SECTION )
//#define FLAG_NEXTRACTSET_LOWSPECIFIC_xx			BIT_8	// |  

//#define FLAG_NEXTRACTSET_HIGHSPECIFIC_xx			BIT_7	// |
//#define FLAG_NEXTRACTSET_HIGHSPECIFIC_xx			BIT_6	// |_SECTION SPECIFIC HIGH LEVEL FLAGS (External)- Managed by high level engine functions and user 
//#define FLAG_NEXTRACTSET_HIGHSPECIFIC_xx			BIT_5	// | (Each of them is redefine for each SECTION. So the same BIT will have different meaning depending its SECTION )
//#define FLAG_NEXTRACTSET_HIGHSPECIFIC_xx			BIT_4	// | 

//#define FLAG_NEXTRACTSET_xx						BIT_3	// |
#define FLAG_NEXTRACTSET_HUD						BIT_2	// |_CORE HIGH LEVEL FLAGS (External)- Managed by high level engine functions and user
#define FLAG_NEXTRACTSET_3D							BIT_1	// |
#define FLAG_NEXTRACTSET_FRUSTUMCULLING				BIT_0	// |

// -------------------------------------------------------------------------------------------------------------------------------------------
// Masks to test a specific part of the NSTATESET 32 bits Value
#define MASK_NEXTRACTSET_LOWLEVEL_SECTIONID							(0xFFFF0000)// BIT_31 to BIT_16 included
#define MASK_NEXTRACTSET_LOWLEVEL_CORE								(0x0000F000)// BIT_15 to BIT_12 included
#define MASK_NEXTRACTSET_LOWLEVEL_SPECIFIC							(0x00000F00)// BIT_11 to BIT_8 included
#define MASK_NEXTRACTSET_LOWLEVEL_FLAGS								(0x0000FF00)// BIT_15 to BIT_8 included (CORE+SPECIFIC)
#define MASK_NEXTRACTSET_LOWLEVEL_ALL								(0xFFFFFF00)// BIT_31 to BIT_8 included (SECTION+CORE+SPECIFIC)

#define MASK_NEXTRACTSET_HIGHLEVEL_SPECIFIC							(0x000000F0)// BIT_7 to BIT_4 included
#define MASK_NEXTRACTSET_HIGHLEVEL_CORE								(0x0000000F)// BIT_3 to BIT_0 included
#define MASK_NEXTRACTSET_HIGHLEVEL_FLAGS							(0x000000FF)// BIT_7 to BIT_0 included (CORE+SPECIFIC)

// -------------------------------------------------------------------------------------------------------------------------------------------
// MAKE and SPLIT NEXTRACTSET
#define NMAKE_NEXTRACTSET_2P(low,high)								( (low&MASK_NEXTRACTSET_LOWLEVEL_ALL)|(high&MASK_NEXTRACTSET_HIGHLEVEL_FLAGS) )
#define NMAKE_NEXTRACTSET_3P(lsectid,lflags,hflags)					( (lsectid&MASK_NEXTRACTSET_LOWLEVEL_SECTIONID)|(lflags&MASK_NEXTRACTSET_LOWLEVEL_FLAGS)|(hflags&MASK_NEXTRACTSET_HIGHLEVEL_FLAGS) )

#define NGET_EXTRACTSET_LOWLEVEL_SECTIONID(xset)					(xset&MASK_NEXTRACTSET_LOWLEVEL_SECTIONID)							
#define NGET_EXTRACTSET_LOWLEVEL_CORE(xset)							(xset&MASK_NEXTRACTSET_LOWLEVEL_CORE)							
#define NGET_EXTRACTSET_LOWLEVEL_SPECIFIC(xset)						(xset&MASK_NEXTRACTSET_LOWLEVEL_SPECIFIC)							
#define NGET_EXTRACTSET_LOWLEVEL_FLAGS(xset)						(xset&MASK_NEXTRACTSET_LOWLEVEL_FLAGS)							
#define NGET_EXTRACTSET_LOWLEVEL(xset)								(xset&MASK_NEXTRACTSET_LOWLEVEL_ALL)							

#define NGET_EXTRACTSET_HIGHLEVEL_CORE(xset)						(xset&MASK_NEXTRACTSET_HIGHLEVEL_CORE)							
#define NGET_EXTRACTSET_HIGHLEVEL_SPECIFIC(xset)					(xset&MASK_NEXTRACTSET_HIGHLEVEL_SPECIFIC)							
#define NGET_EXTRACTSET_HIGHLEVEL_FLAGS(xset)						(xset&MASK_NEXTRACTSET_HIGHLEVEL_FLAGS)							

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------
// LOW LEVEL ID:
// SECTIONS
// max is (0xFFFF<<16)
//													(0<<16)	// means NULL, MUST STILL UNUSED !!! 
#define NEXTRACTSET_SECTION_VOID					(1<<16)	// All the known ExtractSets of this section are linked with NRENDERABLE_EXTRACT_FCT_VOID id ! 
#define NEXTRACTSET_SECTION_STANDARD				(2<<16)	// Generic Extract functions (3D/HUD). 
#define NEXTRACTSET_SECTION_UI						(3<<16)	// Extract functions for UI(Generic).
#define NEXTRACTSET_SECTION_UICHECKBOX				(4<<16)	// Extract functions for UICHECKBOX.
#define NEXTRACTSET_SECTION_UICUSTOM				(5<<16)	// Extract functions for UIGEOMCOLLECTION.


// ---------------------------------------------------------------------------------------------------
// SPECIFIC FLAGS BY SECTION (LOW LEVEL and HIGH LEVEL)
//
//		SECTION SPECIFIC LOW LEVEL ENGINE FLAGS (BIT_11, BIT_10, BIT_9 and BIT_8)
//		( Each section may use these 4 BITS in is own way. )
//		#define FLAG_NEXTRACTSET_LOWSPECIFIC_xx
//
//		SECTION SPECIFIC HIGH LEVEL ENGINE FLAGS (BIT_7, BIT_6, BIT_5 and BIT_4)
//		( Each section may use these 4 BITS in is own way. )
//		#define FLAG_NEXTRACTSET_HIGHSPECIFIC_xx	
//
// 1) NEXTRACTSET_SECTION_NULL, NEXTRACTSET_SECTION_VOID, NEXTRACTSET_SECTION_USERVOID.
//		No specific FLAGS
//
// 2) NEXTRACTSET_SECTION_STANDARD
#define FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMORDER	BIT_7	// to use Renderable geometries array order as local sorting key
#define FLAG_NEXTRACTSET_HIGHSPECIFIC_STANDARD_GEOMZ		BIT_6	// to use first Vertex Z of each geometry as geometry specific sorting Key
//																	// ( normal way is using BB Renderable Sphere as common sorting key for all the renderable geometries )
// 3) NEXTRACTSET_SECTION_UI
//		No specific FLAGS
//
// 4) NEXTRACTSET_SECTION_UICHECKBOX
//		No specific FLAGS
//
// 5) NEXTRACTSET_SECTION_UICUSTOM
#define FLAG_NEXTRACTSET_HIGHSPECIFIC_UICUSTOM_FLASH	BIT_7	// to use extraction function able to "flash" visible geometries

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NRENDERABLE_EXTRACTSET_H 

